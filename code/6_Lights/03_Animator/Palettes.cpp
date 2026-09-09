#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


void mAnimatorLight::Reset_CustomPalette_NamesDefault()
{
  
  char buffer[30];
  
  for (int ii=0;ii< mPaletteI->user_defined_palette_count;ii++){ 
    sprintf(buffer, D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR, ii+1);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii);
  }

}


#ifdef ENABLE_DEVFEATURE_LIGHTING__MIRROR_BYTE_PACKED_PALETTES_IN_CRGBPALETTE16

// You provide this to match your packed encoding (RGB, WRGB, etc.)
static inline CRGB decodePackedToCRGB(const uint8_t* p, uint8_t encWidth) {
  // Example for simple RGB packed data (R,G,B):
  if (encWidth >= 3) return CRGB(p[0], p[1], p[2]);
  return CRGB(0,0,0);
}

// Build a 16-entry CRGBPalette16 from an arbitrary packed palette of N colors,
// using at most 15 distinct source colors so the last color spans >= 2 entries.
//
// Rules:
//  - If N == 0: all black.
//  - If N == 1: fill all 16 with that one color.
//  - Else:
//     * M = min(N, 15) distinct picks from the source palette, sampled evenly.
//     * Partition 16 slots into M blocks:
//         - For k=0..M-2: blockLen = floor(16 / M)
//         - For k= M-1   : blockLen = 16 - sum(previous blockLens)  (absorbs remainder)
//       This guarantees the last block has length >= 2 for all M <= 15.
//     * Color of block k uses source index:
//         srcIdx = round( k * (N-1) / (M-1) )  (with k in [0..M-1])
static inline void buildCRGB16FromPacked(const std::vector<uint8_t>& packed,
                                         uint8_t encWidth,
                                         uint16_t colorsInPalette,
                                         CRGBPalette16& out)
{
  // Initialize to black
  for (uint8_t i = 0; i < 16; i++) out.entries[i] = CRGB(0,0,0);

  // Trivial cases
  if (encWidth == 0 || colorsInPalette == 0) return;

  auto decodeAt = [&](uint16_t idx) -> CRGB {
    const uint32_t off = (uint32_t)idx * encWidth;
    if (off + encWidth > packed.size()) return CRGB(0,0,0);
    return decodePackedToCRGB(&packed[off], encWidth);
  };

  if (colorsInPalette == 1) {
    const CRGB c = decodeAt(0);
    for (uint8_t i = 0; i < 16; i++) out.entries[i] = c;
    return;
  }

  // Use at most 15 distinct source colors to ensure last block >= 2 entries
  const uint8_t M = (colorsInPalette < 15) ? (uint8_t)colorsInPalette : (uint8_t)15;

  // Base block length for first M-1 blocks
  const uint8_t baseLen = (uint8_t)(16 / M);                 // floor
  uint8_t used = 0;

  // Helper to compute a rounded source index spanning [0..colorsInPalette-1]
  auto srcIndexForK = [&](uint8_t k) -> uint16_t {
    if (M <= 1) return 0; // shouldn't happen because colorsInPalette >= 2
    const uint32_t num = (uint32_t)k * (uint32_t)(colorsInPalette - 1) + (uint32_t)((M - 1) / 2);
    // "+ (M-1)/2" is a small rounding term. For M arbitrary, you can use +((M-1)>>1).
    return (uint16_t)(num / (uint32_t)(M - 1));
  };

  // Fill first M-1 blocks, each 'baseLen' entries
  for (uint8_t k = 0; k < (M - 1); k++) {
    const CRGB c = decodeAt(srcIndexForK(k));
    for (uint8_t j = 0; j < baseLen; j++) {
      if (used < 16) out.entries[used++] = c;
    }
  }

  // Last block: fill the rest (ensures >= 2 entries for M <= 15)
  {
    const CRGB cLast = decodeAt(colorsInPalette - 1);
    while (used < 16) out.entries[used++] = cLast;
  }
}

#endif

/**
 * @brief Loads a palette into RAM for the segment, handling multiple palette types.
 *
 * Loads the specified palette ID into RAM for active use within a segment. This is an event-driven
 * construction step (not per-frame). Any temporal/sensor-driven changes belong in Update_LivePalettes().
 *
 * Palette families handled here:
 * - Static CRGBPalette16 palettes (FastLED PROGMEM pointers)
 * - FastLED gradient palettes (parsed into CRGBPalette16 + stop indices)
 * - Static encoded palettes (byte-packed)
 * - Custom encoded palettes (user-defined, already in RAM)
 * - Segment-colour derived palettes (built from segcol[])
 * - Live palettes (dynamic):
 *    A) Byte-packed live palettes  : PALETTELIST_DYNAMIC__COLOUR__ID_START..__LENGTH__ID
 *       - Data buffer is owned by mPaletteI->dynamic_palettes[] and refreshed by Update_LivePalettes()
 *    B) CRGBPalette16 live palettes: PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__...
 *       - Palette data is generated/updated by Update_LivePalettes()
 *
 * @param palette_id The ID of the palette to load.
 * @param _palette_container The palette container to store the loaded data. If null, the segment's default container is used.
 */
void IRAM_ATTR mAnimatorLight::Segment::LoadPalette(uint8_t palette_id, mPaletteLoaded* _palette_container)
{
  #ifdef ENABLE_DEVFEATURE_LIGHTING__LOAD_PALETTE_ASYNC_LOCK
  while (LoadPalette_AsyncLock) { delay(1); }
  LoadPalette_AsyncLock = true;
  #endif

  DEBUG_LINE_HERE_TRACE
  DEBUG_PRINT_F("Palette ID: %d", palette_id);

  // No explicit destination means load the segment's primary palette.
  if (_palette_container == nullptr)
  {
    _palette_container = palette_loaded;
  }

  // Record the ID only in the container being loaded.
  _palette_container->loaded_palette_id = palette_id;

  // ---- Helpers (local, no ABI impact) ----
  auto setPackedPtrAndMeta = [&](mPalette::PALETTE_DATA* ptr) {
    _palette_container->pData                = ptr->data;
    _palette_container->encoded_colour_width = mPaletteI->GetEncodedColourWidth(ptr->encoding);
    _palette_container->colours_in_palette   = ptr->data.size() / _palette_container->encoded_colour_width;
  };

  auto mirrorPackedIntoCRGB16 = [&]() {
    #ifdef ENABLE_DEVFEATURE_LIGHTING__MIRROR_BYTE_PACKED_PALETTES_IN_CRGBPALETTE16
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    buildCRGB16FromPacked(_palette_container->pData,
                          _palette_container->encoded_colour_width,
                          _palette_container->colours_in_palette,
                          _palette_container->CRGB16Palette16_Palette.data);
    #endif
  };

  // ------------------------------------------------------------------
  // 1) Static CRGBPalette16 (FastLED)
  // ------------------------------------------------------------------
  if ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) &&
      (palette_id <  mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  {
    const uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    _palette_container->CRGB16Palette16_Palette.data = *fastledPalettes[palette_id_adj];
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;
    ALOG_DBM(PSTR("crgb16 %d"), palette_id);
  }
  // ------------------------------------------------------------------
  // 2) Static CRGBPalette16 Gradient (FastLED gradient table)
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) &&
           (palette_id <  mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  {
    const uint16_t gradient_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;

    byte tcp[72]; // up to 18 entries
    memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[gradient_id])), sizeof(tcp));

    // Build CRGBPalette16 from gradient
    _palette_container->CRGB16Palette16_Palette.data.loadDynamicGradientPalette(tcp);

    // Parse gradient stop indices exactly
    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
    TRGBGradientPaletteEntryUnion u;

    // Count entries (kept for parity; not required otherwise)
    uint16_t count = 0;
    do {
      u = *(ent + count);
      count++;
    } while (u.index != 255);

    u = *ent;
    int indexstart = 0;
    while (indexstart < 255) {
      indexstart = u.index;
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(u.index);
      ent++;
      u = *ent;
    }

    _palette_container->colours_in_palette = 16;
  }
  // ------------------------------------------------------------------
  // 3) Static byte-packed palettes
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) &&
           (palette_id <  mPalette::PALETTELIST_STATIC_LENGTH__ID))
  {
    const uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;

    #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
    ALOG_INF(PSTR("LOADING PALETTELIST_STATIC palette_id_adj %d %d %d"),
             palette_id_adj, palette_id, mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID);
    #endif

    mPalette::PALETTE_DATA* ptr = &mPaletteI->static_palettes[palette_id_adj];
    setPackedPtrAndMeta(ptr);
    mirrorPackedIntoCRGB16();
  }
  // ------------------------------------------------------------------
  // 4) Custom palettes (user-defined, already in RAM)
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_DYNAMIC__LENGTH__ID) &&
           (palette_id <  mPaletteI->GetPaletteListLength()))
  {
    const uint16_t palette_id_adj =
      palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;

    mPalette::PALETTE_DATA* ptr = &mPaletteI->custom_palettes[palette_id_adj];
    setPackedPtrAndMeta(ptr);
    mirrorPackedIntoCRGB16();
  }
  // ------------------------------------------------------------------
  // 5) Single segment colour (no preload needed)
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) &&
           (palette_id <  mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
  {
    _palette_container->colours_in_palette = 1;
  }
  // ------------------------------------------------------------------
  // Segment-colour derived CRGBPalette16 palettes
  // ------------------------------------------------------------------
  else if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;

    const CRGB prim = segcol[0].getU32();
    const CRGB sec  = segcol[1].getU32();
    _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim, prim, sec, sec);
  }
  else if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_THREE_123__ID)
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;

    const CRGB prim = segcol[0].getU32();
    const CRGB sec  = segcol[1].getU32();
    const CRGB ter  = segcol[2].getU32();
    _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim, sec, ter);
  }
  else if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FOUR_1234__ID)
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;

    const CRGB prim = segcol[0].getU32();
    const CRGB sec  = segcol[1].getU32();
    const CRGB ter  = segcol[2].getU32();
    const CRGB four = segcol[3].getU32();
    _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim, sec, ter, four);
  }
  else if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FIVE_12345__ID)
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;

    const CRGB prim = segcol[0].getU32();
    const CRGB sec  = segcol[1].getU32();
    const CRGB ter  = segcol[2].getU32();
    const CRGB four = segcol[3].getU32();
    const CRGB five = segcol[4].getU32();
    _palette_container->CRGB16Palette16_Palette.data =
      CRGBPalette16(prim, prim, prim, sec, sec, sec, ter, ter, ter, four, four, four, five, five, five, five);
  }
  else if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID)
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->colours_in_palette = 16;

    const CRGB prim = segcol[0].getU32();
    const CRGB sec  = segcol[1].getU32();
    const CRGB ter  = segcol[2].getU32();
    const CRGB four = segcol[3].getU32();
    const CRGB five = segcol[4].getU32();

    _palette_container->CRGB16Palette16_Palette.data =
      CRGBPalette16(prim, sec, ter, four, five,
                    prim, sec, ter, four, five,
                    prim, sec, ter, four, five,
                    five);
  }
  // ------------------------------------------------------------------
  // 7) Live palettes (dynamic) - BYTE PACKED
  //    (Update_LivePalettes() owns refresh; Load just points at the buffer)
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START) &&
           (palette_id <  mPalette::PALETTELIST_DYNAMIC__LENGTH__ID))
  {
    const uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START;

    #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
    ALOG_HGL(PSTR("LOADING LIVE(BYTEPACK) palette_id_adj %d %d %d"),
             palette_id_adj, palette_id, mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START);
    #endif

    mPalette::PALETTE_DATA* ptr = &mPaletteI->dynamic_palettes[palette_id_adj];
    setPackedPtrAndMeta(ptr);
    mirrorPackedIntoCRGB16();
  }
  // ------------------------------------------------------------------
  // 8) Live palettes (dynamic) - CRGBPALETTE16 (randomised etc.)
  //    (Seed once here; periodic refresh is only Update_LivePalettes)
  // ------------------------------------------------------------------
  else if ((palette_id >= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) &&
           (palette_id <  mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
  {
    _palette_container->CRGB16Palette16_Palette.SetDefaultIndexing();
    _palette_container->encoded_colour_width = 3;
    _palette_container->colours_in_palette   = 16;

    // Seed once via the single source of truth (no periodic timing here).
    Update_LivePalettes(palette_id);
  }

  #ifdef ENABLE_DEVFEATURE_LIGHTING__LOAD_PALETTE_ASYNC_LOCK
  LoadPalette_AsyncLock = false;
  #endif
}

/*******************************************************************************************************************************************************************************************************************
 * @description : Updates the segment's live palette containers.
 *
 *                The normal no-argument runtime call updates both the primary
 *                palette and the optional secondary palette.
 *
 *                Explicit palette-ID or preview calls update only the requested
 *                primary/preview palette and do not implicitly update palette2.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::Segment::Update_LivePalettes(
  uint16_t pal_id,
  uint16_t preview_index,
  bool preview_mode
){
  const bool default_runtime_call = (pal_id == 0xFFFF) && !preview_mode;
  const uint16_t resolved_pal_id  = (pal_id == 0xFFFF) ? palette_id : pal_id;

  Update_LivePalette(
    resolved_pal_id,
    palette_loaded,
    preview_index,
    preview_mode
  );

  if (default_runtime_call && (palette2_loaded != nullptr))
  {
    Update_LivePalette(
      palette2_loaded->loaded_palette_id,
      palette2_loaded,
      0,
      false
    );
  }
}


void mAnimatorLight::Segment::Update_LivePalette(
  uint16_t pal_id,
  mPaletteLoaded* palette_container,
  uint16_t preview_index,
  bool preview_mode
){
  // Default: use current runtime palette if pal_id == 0xFFFF
  const uint16_t pid = (pal_id == 0xFFFF) ? palette_id : pal_id;

//   ALOG_INF(PSTR("Update_LivePalette: pid=%d, preview_index=%d, preview_mode=%d"), pid, preview_index, preview_mode);

    // ------------------------------------------------------------------
// LIVE: CRGBPalette16 Randomise (Elapsed time)
// - Single source of truth for refresh timing and palette generation
// - Also enforces cycle_time__rate_ms guard each call (avoid overruns)
// ------------------------------------------------------------------
if ((pid >= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) &&
    (pid <  mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
{
  // Web preview: safest is to freeze whatever is currently loaded,
  // otherwise preview "moves" while you’re in UI.
  if (preview_mode) {
    return;
  }

  // --- Guard: ensure effect cycle time >= palette refresh interval when blending is active ---
  // (This needs to run each time, because speed/intensity can change live.)
  // if (speed != 255) {
  //   const uint32_t new_colour_rate_ms = 1000UL + (uint32_t)(live_palette.intensity * 100UL);
  //   if (new_colour_rate_ms >= cycle_time__rate_ms) {
  //     cycle_time__rate_ms = new_colour_rate_ms + 100UL;
  //   }
  // }

  const uint32_t now_ms = millis();

  // Seed exactly once after load/reset
  if (live_palette.timing1 == 0) {
    live_palette.timing1 = now_ms;
    // fallthrough: generate immediately on first call
  } else {
    const uint32_t new_colour_rate_ms = 1000UL + (uint32_t)(live_palette.intensity * 100UL);
    // ALOG_INF(PSTR("palix%d,new_colour_rate_ms=%d"),live_palette.intensity,new_colour_rate_ms);
    if ((now_ms - live_palette.timing1) < new_colour_rate_ms) return;
    live_palette.timing1 = now_ms;
  }

  // ALOG_INF(PSTR("Updating LIVE palette ID %d\t%d"), pid, live_palette.timing1);

  switch (pid)
  {
    default:
    case mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID:
      palette_loaded->CRGB16Palette16_Palette.data = CRGBPalette16(
        CHSV(hw_random8(), 255, 255),
        CHSV(hw_random8(), 255, 255),
        CHSV(hw_random8(), 255, 255),
        CHSV(hw_random8(), 255, 255)
      );
      break;

    case mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_02__ID:
      palette_loaded->CRGB16Palette16_Palette.data = CRGBPalette16(
        CHSV(hw_random8(), hw_random8(40, 100), hw_random8(220, 255)),
        CHSV(hw_random8(), hw_random8(40, 100), hw_random8(220, 255)),
        CHSV(hw_random8(), hw_random8(40, 100), hw_random8(220, 255)),
        CHSV(hw_random8(), hw_random8(40, 100), hw_random8(220, 255))
      );
      break;

    case mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_03__ID: {
      const uint8_t pastel_index = hw_random8(4);
      palette_loaded->CRGB16Palette16_Palette.data = CRGBPalette16(
        CHSV(hw_random8(), (pastel_index == 0) ? hw_random8(40, 100) : hw_random8(153, 255), 255),
        CHSV(hw_random8(), (pastel_index == 1) ? hw_random8(40, 100) : hw_random8(153, 255), 255),
        CHSV(hw_random8(), (pastel_index == 2) ? hw_random8(40, 100) : hw_random8(153, 255), 255),
        CHSV(hw_random8(), (pastel_index == 3) ? hw_random8(40, 100) : hw_random8(153, 255), 255)
      );
    } break;

    case mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_04__ID:
      palette_loaded->CRGB16Palette16_Palette.data = CRGBPalette16(
        CHSV(hw_random8(), hw_random8(100, 217), hw_random8(10, 255)),
        CHSV(hw_random8(), hw_random8(100, 217), hw_random8(10, 255)),
        CHSV(hw_random8(), hw_random8(100, 217), hw_random8(10, 255)),
        CHSV(hw_random8(), hw_random8(100, 217), hw_random8(10, 255))
      );
      break;

    case mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_05__ID:
      palette_loaded->CRGB16Palette16_Palette.data = CRGBPalette16(
        CHSV(hw_random8(), hw_random8(153, 217), hw_random8(0, 68)),
        CHSV(hw_random8(), hw_random8(153, 217), hw_random8(69, 127)),
        CHSV(hw_random8(), hw_random8(153, 217), hw_random8(127, 190)),
        CHSV(hw_random8(), hw_random8(153, 217), hw_random8(190, 255))
      );
      break;
  }

  return;
}

  // ------------------------------------------------------------------
  // LIVE: TimeReactive SegCol Blend (Minute sawtooth)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__ID)
  {
    uint8_t progress;

    if (preview_mode) {
      progress = (preview_index == 0) ? 0 : 255;
    } else {
      uint8_t s = tkr_time->RtcTime.second;
      progress = (s < 30) ? map(s, 0, 29, 0, 255) : map(s, 30, 59, 255, 0);
    }

    palette_loaded->solid_colour.colourRGBW = ColourBlend(segcol[0].colour, segcol[1].colour, progress);
    palette_loaded->solid_colour.cct        = ((uint16_t)segcol[1].cct * progress + (uint16_t)segcol[0].cct * (255 - progress)) >> 8;
    return;
  }


  // ------------------------------------------------------------------
  // LIVE: TimeReactive SegCol Blend (Hour sawtooth)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ID)
  {
    uint8_t progress;

    if (preview_mode) {
      progress = (preview_index == 0) ? 0 : 255;
    } else {
      uint8_t s = tkr_time->RtcTime.hour;
      progress = (s < 30) ? map(s, 0, 29, 0, 255) : map(s, 30, 59, 255, 0);
    }

    palette_loaded->solid_colour.colourRGBW = ColourBlend(segcol[0].colour, segcol[1].colour, progress);
    palette_loaded->solid_colour.cct        = ((uint16_t)segcol[1].cct * progress + (uint16_t)segcol[0].cct * (255 - progress)) >> 8;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: SegCol Cycle (Immediate)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_IMMEDIATE_01__ID)
  {
    uint8_t k = 0;

    if (preview_mode) {
      k = (uint8_t)preview_index;
      if (k > 4) k = 4;
    } else {
      const uint32_t now_ms = millis();
      if (live_palette.timing1 == 0) live_palette.timing1 = now_ms;

      const uint32_t T_max_ms = 25000UL;
      uint32_t T_ms = (uint32_t)live_palette.intensity * T_max_ms / 255UL;
      if (T_ms < 1000UL) T_ms = 1000UL;

      const uint32_t t_ms = (now_ms - live_palette.timing1) % T_ms;

      const uint32_t s1 = (1UL * T_ms) / 5UL;
      const uint32_t s2 = (2UL * T_ms) / 5UL;
      const uint32_t s3 = (3UL * T_ms) / 5UL;
      const uint32_t s4 = (4UL * T_ms) / 5UL;

      if      (t_ms < s1) k = 0;
      else if (t_ms < s2) k = 1;
      else if (t_ms < s3) k = 2;
      else if (t_ms < s4) k = 3;
      else                k = 4;
    }

    palette_loaded->solid_colour.colourRGBW = segcol[k].colour;
    palette_loaded->solid_colour.cct        = segcol[k].cct;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: SegCol Cycle (Blending + controllable dwell)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_BLENDING_02__ID)
  {
    if (preview_mode) {
      uint8_t k = (uint8_t)preview_index;
      if (k > 4) k = 4;

      palette_loaded->solid_colour.colourRGBW = segcol[k].colour;
      palette_loaded->solid_colour.cct        = segcol[k].cct;
      return;
    }

    const uint32_t now_ms = millis();
    if (live_palette.timing1 == 0) live_palette.timing1 = now_ms;

    const uint32_t T_max_ms = 25000UL;
    uint32_t T_ms = (uint32_t)live_palette.intensity * T_max_ms / 255UL;
    if (T_ms < 1000UL) T_ms = 1000UL;

    const uint32_t t_ms = (now_ms - live_palette.timing1) % T_ms;

    const uint32_t s1 = (1UL * T_ms) / 5UL;
    const uint32_t s2 = (2UL * T_ms) / 5UL;
    const uint32_t s3 = (3UL * T_ms) / 5UL;
    const uint32_t s4 = (4UL * T_ms) / 5UL;

    uint8_t k;
    uint32_t slot_start, slot_end;

    if      (t_ms < s1) { k = 0; slot_start = 0;  slot_end = s1; }
    else if (t_ms < s2) { k = 1; slot_start = s1; slot_end = s2; }
    else if (t_ms < s3) { k = 2; slot_start = s2; slot_end = s3; }
    else if (t_ms < s4) { k = 3; slot_start = s3; slot_end = s4; }
    else                { k = 4; slot_start = s4; slot_end = T_ms; }

    const uint32_t slot_ms = slot_end - slot_start;
    const uint32_t within_ms = t_ms - slot_start;
    const uint8_t k_next = (k == 4) ? 0 : (uint8_t)(k + 1);

    #ifndef SEGCOLOUR_CYCLE_HOLD_PCT
    #define SEGCOLOUR_CYCLE_HOLD_PCT 70
    #endif

    #ifndef SEGCOLOUR_CYCLE_BLEND_PCT
    #define SEGCOLOUR_CYCLE_BLEND_PCT 30
    #endif

    uint32_t hold_pct = (uint32_t)SEGCOLOUR_CYCLE_HOLD_PCT;
    uint32_t blend_pct = (uint32_t)SEGCOLOUR_CYCLE_BLEND_PCT;

    if (hold_pct > 100UL) hold_pct = 100UL;
    if (blend_pct > 100UL) blend_pct = 100UL;
    if (hold_pct + blend_pct > 100UL) blend_pct = 100UL - hold_pct;

    const uint32_t hold_end_ms = (slot_ms * hold_pct) / 100UL;

    uint32_t colour_out = segcol[k].colour;
    uint8_t cct_out = segcol[k].cct;

    if (!(slot_ms == 0 || within_ms < hold_end_ms || slot_ms <= hold_end_ms)) {
      const uint32_t start = hold_end_ms;
      const uint32_t denom = (slot_ms > start) ? (slot_ms - start) : 1UL;
      const uint32_t w = within_ms - start;

      uint8_t progress = map(w, 0, denom, 0, 255);

      colour_out = ColourBlend(segcol[k].colour, segcol[k_next].colour, progress);
      cct_out = ((uint16_t)segcol[k_next].cct * progress + (uint16_t)segcol[k].cct * (255 - progress)) >> 8;
    }

    palette_loaded->solid_colour.colourRGBW = colour_out;
    palette_loaded->solid_colour.cct        = cct_out;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> Solid colour of sky (samples dynamic palette at one index)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID)
  {
    #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
      const float elevation = tkr_solar->Get_Elevation();
      const float el_min = tkr_solar->Get_Elevation_Min();
      const float el_max = tkr_solar->Get_Elevation_Max();
    #else
      const float elevation = 0.0f;
      const float el_min = -45.0f;
      const float el_max = 45.0f;
    #endif

    uint16_t pal_index = 0;

    if (preview_mode) {
      const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
      pal_index = (N <= 1) ? 0 : (uint16_t)map(preview_index, 0, N - 1, 0, 255);
    } else {
      const float eval_elevation = constrain(elevation, el_min, el_max);
      pal_index = (uint16_t)constrain(mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 255.0f), 0.0f, 255.0f);
    }

    const uint16_t palette_adjusted_id_rel0 = pid - mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START;
    uint8_t* data_local = &mPaletteI->dynamic_palettes[palette_adjusted_id_rel0].data[0];

    bool force_palette_mode = false;

    const uint32_t c32 = mPaletteI->SubGet_Encoded_Palette_Colour_U32(
      data_local,
      pal_index,
      palette_loaded->encoded_colour_width,
      palette_loaded->colours_in_palette,
      mPaletteI->dynamic_palettes[palette_adjusted_id_rel0].encoding,
      nullptr,
      false,
      0,
      force_palette_mode,
      false
    );

    palette_loaded->solid_colour.colourRGBW = c32;
    palette_loaded->solid_colour.cct = 0;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> Gradient Colour Of Sky
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID)
  {
    #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
      const float elevation = tkr_solar->Get_Elevation();
      const float el_min = tkr_solar->Get_Elevation_Min();
      const float el_max = tkr_solar->Get_Elevation_Max();
    #else
      const float elevation = 0.0f;
      const float el_min = -45.0f;
      const float el_max = 45.0f;
    #endif

    float zoom_ratio = custom1 / 255.0f;
    zoom_ratio = constrain(zoom_ratio, 0.01f, 1.0f);

    const float zoom_range = (el_max - el_min) * zoom_ratio;
    float el_start = elevation - (zoom_range * 0.5f);
    float el_end = elevation + (zoom_range * 0.5f);

    el_start = constrain(el_start, el_min, el_max);
    el_end = constrain(el_end, el_min, el_max);

    uint16_t palette_start = (uint16_t)constrain(mSupport::mapfloat(el_start, el_min, el_max, 0.0f, 255.0f), 0.0f, 255.0f);
    uint16_t palette_end = (uint16_t)constrain(mSupport::mapfloat(el_end, el_min, el_max, 0.0f, 255.0f), 0.0f, 255.0f);

    if (palette_start >= palette_end) palette_start = (palette_end > 0) ? palette_end - 1 : 0;

    const uint16_t rel = pid - mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START;
    uint8_t* data_local = &mPaletteI->dynamic_palettes[rel].data[0];

    for (uint8_t i = 0; i < 16; i++)
    {
      const uint16_t idx = map(i, 0, 15, 0, 255);
      const uint16_t pal_idx = map(idx, 0, 255, palette_start, palette_end);

      uint8_t dummy_enc = 0;

      const uint32_t c32 = mPaletteI->SubGet_Encoded_Palette_Colour_U32(
        data_local,
        pal_idx,
        palette_loaded->encoded_colour_width,
        palette_loaded->colours_in_palette,
        mPaletteI->dynamic_palettes[rel].encoding,
        &dummy_enc,
        false,
        0,
        PALETTE_MODE__FORCE_GRADIENT,
        false
      );

      palette_loaded->CRGB16Palette16_Palette.data[i] = CRGB(R(c32), G(c32), B(c32));
    }

    if (preview_mode) {
      const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
      const uint8_t idx255 = (N <= 1) ? 0 : (uint8_t)map(preview_index, 0, N - 1, 0, 255);

      const CRGB c = mPaletteI->ColorFromPalette16(palette_loaded->CRGB16Palette16_Palette.data, idx255, 255, LINEARBLEND);
      palette_loaded->solid_colour.colourRGBW = RGBW32(c.r, c.g, c.b, 0);
      palette_loaded->solid_colour.cct = 0;
    }

    return;
  }

}





/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
const char* mAnimatorLight::GetPaletteNameByID(uint16_t palette_id, char* buffer, uint8_t buflen)
{

  // ALOG_INF(PSTR("GetPaletteNameByID %d"), palette_id);
  // Serial.flush();

    
  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  if(
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED))
  ){       
    // ALOG_INF(PSTR("GetPaletteNameByID  A%d"), palette_id);
    mPaletteI->GetPaletteNameByID(palette_id, buffer, buflen);
  }
  
  /**************************************************************
   * 
   * CUSTOM_PALETTE
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED) && (palette_id < mPaletteI->GetPaletteListLength()))
  ){  
  // ALOG_INF(PSTR("GetPaletteNameByID B %d"), palette_id);
    int8_t adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; //also skip the rgbcct colour names, though, they should be static?
    DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), adjusted_id, buffer, buflen);
    ALOG_DBM(PSTR("device name %s %d"),buffer, adjusted_id );
  }

  return buffer;

}



/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
int16_t mAnimatorLight::GetPaletteIDbyName(char* buffer)
{

  ALOG_DBG(PSTR("GetPaletteIDbyName %s"), buffer);

  int16_t found_id = -1;

  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  if((found_id=mPaletteI->Get_Static_PaletteIDbyName(buffer))>=0)
  {
    ALOG_DBG(PSTR("FOUND Static Name(%s) -> %d"), buffer, found_id);
  }else
  /**************************************************************
   * Dynamic User Defined Names
   * 
   * PALETTELIST_VARIABLE_HSBID__IDS
   * PALETTELIST_SEGMENT__SEGMENT_COLOUR__IDS
   * PALETTELIST_VARIABLE_GENERIC__IDS
   * 
  ***************************************************************/
  if((found_id = DLI->GetDeviceIDbyName(buffer, GetModuleUniqueID()))>=0)
  {
    ALOG_DBG(PSTR("FOUND Dynamic Name(%s) -> %d"), buffer, found_id);
  }
  else
  {
    ALOG_ERR(PSTR("ERROR NO PALETTE ID FOUND FOR Name(%s)"), buffer);

  }
  
  return found_id;

}




uint8_t mAnimatorLight::GetNumberOfColoursInUNLOADEDPalette(uint16_t palette_id)
{

  uint8_t palette_colour_count = 0;

  // ALOG_INF(PSTR("============LoadPalette %d %d %d"), palette_id, 0, tkr_anim->segment_current_index);

  /**
   * @brief PaletteList Vectors should have the length stored in it. Actual pixel count depends on encoding type
   **/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
  ){   

    uint16_t encoded_colour_width = 0;

    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    mPalette::PALETTE_DATA pal = mPaletteI->static_palettes[constrain(palette_id_adj,0,mPaletteI->static_palettes.size()-1)];

    // Serial.println(ptr->encoding.data, BIN);

    if(pal.encoding.red_enabled){ encoded_colour_width++; }
    if(pal.encoding.green_enabled){ encoded_colour_width++; }
    if(pal.encoding.blue_enabled){ encoded_colour_width++; }
    if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

    if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
    if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

    // if(pal.encoding.index_exact){ encoded_colour_width++; }
    if(pal.encoding.index_gradient){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    
    // if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }


    if(encoded_colour_width==0)
    {
      // ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
      return palette_colour_count;
    }
  
    palette_colour_count = pal.data.size()/encoded_colour_width; 
    
    // ALOG_INF(PSTR("============  data_length/encoded_width %d %d"),  pal.data.size(), encoded_colour_width);
 
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID)
  ){
    palette_colour_count = 1;
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)
  ){
    palette_colour_count = 16;
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_05__ID)
  ){  
    palette_colour_count = 16;    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  
    palette_colour_count = 1;    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START) && (palette_id < mPalette::PALETTELIST_DYNAMIC__LENGTH__ID)
  ){  
    ALOG_INF(PSTR("Temporary fix, needs its own palette count"));
    palette_colour_count = 1;    
    /**
     * @brief These are all different, doing a temporary fix for now
     * 
     */
    switch(palette_id)
    {
      default:
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_IMMEDIATE_01__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_BLENDING_02__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID:
        palette_colour_count = sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__DATA)/6;
      break;
    }
  }
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   
    palette_colour_count = 16;
  }else

  /**
   * @brief CustomPalettes
   * 
   */
  if(
    (palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED) && (palette_id < mPaletteI->GetPaletteListLength())
  ){  

    uint16_t palette_adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // adjust back into correct indexing
    uint8_t colour_width  = mPaletteI->GetEncodedColourWidth(mPaletteI->custom_palettes[palette_adjusted_id].encoding); 
    palette_colour_count = mPaletteI->custom_palettes[palette_adjusted_id].data.size()/colour_width;    
    ALOG_INF(PSTR("LoadPalette %d %d %d %d"),palette_id, palette_adjusted_id, colour_width, palette_colour_count); delay(3000);

  }
    
  return palette_colour_count;

}


void mAnimatorLight::loadCustomPalettes()
{
  byte tcp[72]; //support gradient palettes with up to 18 entries
  CRGBPalette16 targetPalette;
  customPalettes.clear(); // start fresh
  // for (int index = 0; index<10; index++) {
  //   char fileName[32];
  //   sprintf_P(fileName, PSTR("/palette%d.json"), index);

  //   StaticJsonDocument<1536> pDoc; // barely enough to fit 72 numbers
  //   if (FILE_SYSTEM.exists(fileName)) {
  //     DEBUG_PRINT(F("Reading palette from "));
  //     DEBUG_PRINTLN(fileName);

  //     if (readObjectFromFile(fileName, nullptr, &pDoc)) {
  //       JsonArray pal = pDoc[F("palette")];
  //       if (!pal.isNull() && pal.size()>7) { // not an empty palette (at least 2 entries)
  //         size_t palSize = MIN(pal.size(), 72);
  //         palSize -= palSize % 4; // make sure size is multiple of 4
  //         for (size_t i=0; i<palSize && pal[i].as<int>()<256; i+=4) {
  //           tcp[ i ] = (uint8_t) pal[ i ].as<int>(); // index
  //           tcp[i+1] = (uint8_t) pal[i+1].as<int>(); // R
  //           tcp[i+2] = (uint8_t) pal[i+2].as<int>(); // G
  //           tcp[i+3] = (uint8_t) pal[i+3].as<int>(); // B
  //           DEBUG_PRINTF("%d(%d) : %d %d %d\n", i, int(tcp[i]), int(tcp[i+1]), int(tcp[i+2]), int(tcp[i+3]));
  //         }
  //         customPalettes.push_back(targetPalette.loadDynamicGradientPalette(tcp));
  //       }
  //     }
  //   } else {
  //     break;
  //   }
  // }
}
/*WrapEdge and Discrete should be flipped, will rename into original when full conversion is done*/
uint32_t mAnimatorLight::Segment::GetPaletteColour(
  /**
   * @brief _pixel_position
   * ** [0-SEGLEN]
   * ** [0-255]   
   */
  uint16_t pixel_position,
  /**
   * @brief flag_spanned_segment
   * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
   * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
   * ** [2]: preffered
   */
  uint8_t     palette_index__format, 
  /**
   * @brief force_palette_mode flag_crgb_exact_colour
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     force_palette_mode,
  /**
   * @brief flag_wrap_hard_edge
   * ** [PALETTE_MODE__DEFAULT]
   * ** [PALETTE_MODE__FORCE_GRADIENT] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [PALETTE_MODE__FORCE_DISCRETE]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_wrap_hard_edge,
  /**
   * @brief encoded_value
   * ** [uint32_t*] : encoded value from palette
   */
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  /**
   * @brief apply_brightness
   * ** [false] : Apply brightness to the colour
   * ** [true]  : Get the "full" 255 range colour object
   */
  bool apply_brightness, // as a flag, should maybe be removed and combined with the pbri below  
  uint8_t pbri, // since 255 means not, otherwise apply

  uint8_t mcol
){
  
  DEBUG_LINE_HERE_TRACE
  if(palette_id != palette_loaded->loaded_palette_id)
  {
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }


  // uint32_t color = SEGCOLOR(mcol < NUMBER_SEGMENT_COLOURS ? mcol : 0);
  // // default palette or no RGB support on segment
  // if ((palette == 0 && mcol < NUMBER_SEGMENT_COLOURS) || !_isRGB) {
  //   return color_fade(color, pbri, true);
  // }
  
  /**
   * @brief These functions always need called as they are dynamic
   * I should make this a check here, if palette is dynamic, then load everytime
   * 
   * perhaps also add a timer here, so it has a backoff and is only called the minimum amount needed
   * ie have a new tSaved_DynamicUpdate 
   * 
   * This needs moved into its own function and called outside this.
   * SEGMENT.UpdatePalette(); and only called when palette is dynamic/live
   */
  // else // else so it only tries this if the above "if" did not occur to stop double loads
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && 
    (palette_id <= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_05__ID)
  ){
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods    
  }
  DEBUG_LINE_HERE_TRACE

  uint32_t colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
    palette_id,
    (uint8_t*)palette_loaded->pData.data(),
    pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    palette_index__format, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    force_palette_mode
  );
  #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
  white_warm_GetPaletteColour = mPaletteI->colour32_white_cold; // Bypass W2, as this is not used in RGBWW
  #endif

  // Apply brightness if needed
  // if (apply_brightness) {
  //   uint8_t brightness = scale8(_brightness_rgb, tkr_iLight->getBriRGB_Global());
  //   // ALOG_INF(PSTR("brightness getpal %d"),brightness);
  //   uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range

  //   // Extract, scale, and repack in one step
  //   colour = RGBW32(
  //     (R(colour) * scale) >> 8,  // Red
  //     (G(colour) * scale) >> 8,  // Green
  //     (B(colour) * scale) >> 8,  // Blue
  //     (W(colour) * scale) >> 8   // White
  //   );
  //   #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
  //   white_warm_GetPaletteColour = (mPaletteI->colour32_white_cold * scale) >> 8; // Rescale bypass W2
  //   #endif
  // }

  return colour;

}

/*******************************************************************************************************************************************************************************************************************
 * @description : Ensures that the requested secondary palette is loaded into
 *                the segment's optional second palette container.
 *
 *                The container is allocated only when first required and is
 *                reloaded only when the requested palette ID changes.
 ********************************************************************************************************************************************************************************************************************/
bool mAnimatorLight::Segment::EnsurePalette2Loaded(uint16_t palette_id)
{
  if (palette2_loaded == nullptr)
  {
    palette2_loaded = new(std::nothrow) mPaletteLoaded();

    if (palette2_loaded == nullptr)
    {
      ALOG_ERR(PSTR("Failed to allocate palette2_loaded"));
      return false;
    }

    /*
     * loaded_palette_id defaults to zero, which is also a valid palette ID.
     * Force the first call to load the requested palette.
     */
    palette2_loaded->loaded_palette_id = 0xFF;
  }

  if (palette2_loaded->loaded_palette_id != palette_id)
  {
    LoadPalette(palette_id, palette2_loaded);

    if (palette2_loaded->loaded_palette_id != palette_id)
    {
      ALOG_ERR(PSTR("Failed to load secondary palette %u"), palette_id);
      return false;
    }
  }

  return true;
}

/*******************************************************************************************************************************************************************************************************************
 * @description : Reads a colour from the segment's loaded secondary palette.
 *
 *                EnsurePalette2Loaded() must have been called before entering
 *                the per-pixel rendering loop.
 ********************************************************************************************************************************************************************************************************************/
uint32_t mAnimatorLight::Segment::GetPalette2Colour(
  uint16_t pixel_position,
  uint8_t palette_index__format,
  uint8_t force_palette_mode,
  uint8_t flag_wrap_hard_edge,
  uint8_t* encoded_value,
  bool apply_brightness,
  uint8_t pbri,
  uint8_t mcol
){
  if (palette2_loaded == nullptr) return 0;

  /*
   * Transitional compatibility:
   *
   * GetColourFromPreloadedPaletteBuffer_U32() still resolves CRGB palette
   * storage and other metadata through pSEGMENT.palette_loaded.
   *
   * Redirect the pointer only for this lookup. The secondary palette itself
   * is already loaded and is not rebuilt here.
   */
  mPaletteLoaded* palette_loaded_primary = palette_loaded;
  palette_loaded = palette2_loaded;

  uint32_t colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
    palette2_loaded->loaded_palette_id,
    (uint8_t*)palette2_loaded->pData.data(),
    pixel_position,
    encoded_value,
    palette_index__format,
    flag_wrap_hard_edge,
    force_palette_mode
  );

  #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
  white_warm_GetPaletteColour = mPaletteI->colour32_white_cold;
  #endif

  palette_loaded = palette_loaded_primary;

  return colour;
}

/*******************************************************************************************************************************************************************************************************************
 * @description : Releases the optional secondary palette container.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::Segment::ReleasePalette2()
{
  if (palette2_loaded == nullptr) return;

  delete palette2_loaded;
  palette2_loaded = nullptr;
}

/**
 * @brief Loads the requested palette into a temporary container when required.
 *
 * Palette types which can be resolved directly bypass LoadPalette().
 * Other palette types are loaded into a temporary mPaletteLoaded container.
 *
 * This is intended for slow-path operations such as WebUI palette previews,
 * palette enumeration, diagnostics, and occasional arbitrary palette reads.
 */
uint32_t IRAM_ATTR mAnimatorLight::GetPaletteColour_WithTemporaryLoad(
  uint16_t palette_id,
  uint16_t _pixel_position,
  bool     flag_spanned_segment,
  bool     flag_wrap_hard_edge,
  bool     flag_crgb_exact_colour,
  uint8_t* encoded_value,
  bool     flag_request_is_for_full_visual_output
){
  /**
   * @brief Directly handle certain palette types that don't need loading.
   * These palette types are handled directly, bypassing the LoadPalette function.
   */
  if (
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
  ) {
    DEBUG_LINE_HERE_TRACE

    return mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
      palette_id,
      nullptr, // No buffer required for these types
      _pixel_position,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      flag_request_is_for_full_visual_output
    );
  }

  /**
   * @brief Load is required for other palette types, so we call LoadPalette.
   */
  mPaletteLoaded palette_container_temp = mPaletteLoaded();

  SEGMENT.LoadPalette(palette_id, &palette_container_temp);

  return mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
    palette_id,
    palette_container_temp.pData.data(),
    _pixel_position,
    encoded_value,
    flag_spanned_segment,
    flag_wrap_hard_edge,
    flag_crgb_exact_colour,
    flag_request_is_for_full_visual_output
  );
}




//  uint32_t mAnimatorLight::ColorFromPaletteRedirect(const CRGBPalette16& /*pal_ignored*/, uint8_t index, uint8_t brightness, TBlendType blendType)
//     {
//       const uint16_t pal_id = SEGMENT.palette_id;

//       // Immediately jump to low level colour return if possible
      
//       const bool isCRGB16 =
//         ((pal_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID)                      && (pal_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
//         ((pal_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)                     && (pal_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)) ||
//         ((pal_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)     && (pal_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
//         ((pal_id >= mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (pal_id < mPalette::PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID));

//       if (isCRGB16 && SEGMENT.palette_loaded && (SEGMENT.palette_loaded->loaded_palette_id == pal_id)) {
//         return mPalette::ColorFromPalette16(SEGMENT.palette_loaded->CRGB16Palette16_Palette.data, index, brightness, blendType);
//       }

//       // Otherwise, proceed with the general palette colour retrieval

//       const uint8_t force_mode = (blendType == NOBLEND) ? PALETTE_MODE__FORCE_DISCRETE : PALETTE_MODE__FORCE_GRADIENT;
//       const uint8_t wrap_mode = (blendType == LINEARBLEND) ? PALETTE_WRAP_SMOOTH : PALETTE_WRAP_HARDEDGE;

//       uint32_t c = SEGMENT.GetPaletteColour(index, PALETTE_INDEX__IS_255_RANGE, force_mode, wrap_mode, NO_ENCODED_VALUE, false, 255, 0);

//       if (brightness < 255) {
//         const uint16_t scale = uint16_t(brightness) + 1;
//         c = RGBW32((R(c) * scale) >> 8, (G(c) * scale) >> 8, (B(c) * scale) >> 8, (W(c) * scale) >> 8);
//       }

//       return c;
//     }


#endif