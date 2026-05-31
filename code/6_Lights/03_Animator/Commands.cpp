#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

/**
 * @brief Ideas
 * 
 * segment should be controllable by name, and only when matched. So instead of "Segment0":{} we should be able to do "SegName":{"Name":"something", the rest} and pass the {} like we already do
 * Though, this would work since each name needs to be different. Maybe "Seg_Name" and the name part gets pulled out from _ to EOL?
 * 
 * Another idea is being able to set a "HeatmapPalette"
 * 
 * "HeatmapPalette":{
 *    "Data":[float array],
 *    "Gradient":[0 to 255 inflection points],
 *    "PaletteID":"Jet" // so Jet will be default, otherwise it takes a full gradient palette and maps it by loading as CRGB16Palette
 * }
 * 
 * Doing this vs generating the colours in openhab would make it more flexible for the same data to easily be used on different devices by just changing the Gradient.
 * Should this also be part effect?
 * 
 */
void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::Start"));

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];
  uint16_t isserviced_start_count = data_buffer.isserviced;

  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    tkr_anim->force_update();
  }

  uint8_t segments_found = 0;
  
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  /**
   * Can be either a single matrix or multiple matrices
   * One function will be used to parse a matrix object, here it will iter over them and append to the panels
   **/
  if(jtok = obj["MatrixConfig"])
  {
    ALOG_HGL(PSTR("MatrixConfig"));
    if (jtok.isArray()) 
    {

      // Reset matrix settings
      panels = 1;
      panel.clear();
      ALOG_INF(PSTR("panel.clear() HERE C?????????????????????????????????????????????????????"));
      panel.reserve(max(1U,min((size_t)panels,(size_t)WLED_MAX_PANELS)));  // pre-allocate memory for panels

      ALOG_HGL(PSTR("MatrixConfig--------------"));
      JsonParserArray arrobj = jtok;
      for (auto v : arrobj) 
      {
        ALOG_HGL(PSTR("MatrixConfig A"));
        subparse_MatrixConfig(v.getObject());
      }    
      
      // setUpMatrix(); // will check limits
      // makeAutoSegments(true); // nneeds to be performed later to make sure segments are correct
      // deserializeMap();
      ALOG_INF(PSTR("MatrixConfig Complete -- Doing now to see if this resolves conflict with segment commands erasing"));
    }
  }
  #endif
  
  // === [1] Parse "Segment0":{...seg info...},"Segment1":{...seg info...}, ... format ===
  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer])
    { 
      ALOG_INF(PSTR("Seg: \"%s\""), buffer);
      if(segment_i >= segments.size())
      { 
        uint16_t seg_size = segments.size();
        Segment_AppendNew(0, 10, segment_i);
        ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"), segment_i, segments.size()-seg_size, segments.size());
      }
      subparse_JSONCommand(jtok.getObject(), segment_i);
      segments_found++;
    }
  }

  #ifndef ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  // === [2] Optional "Segments":[{...seg info...}, {...seg info...}] array format ===
  if (segments_found == 0 && obj["Segments"].isArray()) {
    JsonParserArray seg_arr = obj["Segments"];
    for (uint8_t i = 0; i < seg_arr.size() && i < MAX_NUM_SEGMENTS; i++) {
      JsonParserObject seg_obj = seg_arr[i];      
      if (i >= segments.size()) {
        uint16_t seg_size = segments.size();
        Segment_AppendNew(0, 10, i);
        ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"), i, segments.size() - seg_size, segments.size());
      }
      subparse_JSONCommand(seg_obj, i);
      segments_found++;
    }
  }
  #else
    // === [2] Optional "Segments":[{...seg info...}, {...seg info...}] array format ===
  if (segments_found == 0 && obj["Segments"].isArray())
  {
    JsonParserArray seg_arr = obj["Segments"];

    // --- [2a] Special case: auto-expand "MatchBus" template across all valid BusConfig entries ---
    bool handled_matchbus = false;
    if (seg_arr.size() > 0)
    {
      JsonParserObject first_seg = seg_arr[0];
      JsonParserToken  pr       = first_seg[PM_PIXELRANGE];

      bool is_matchbus = false;
      if (pr && pr.isStr())
      {
        const char *s = pr.getStr();
        if (s)
        {
          // Accept both "MatchBus" and "MatchedBus" (typo-safe)
          if (!strcasecmp_P(s, PSTR("MatchBus")) || !strcasecmp_P(s, PSTR("MatchedBus")))
          {
            is_matchbus = true;
          }
        }
      }

      if (is_matchbus)
      {
        // Collect VALID busConfig indices (start < stop, start != stop)
        uint8_t valid_bus_idx[WLED_MAX_BUSSES + WLED_MIN_VIRTUAL_BUSSES];
        uint8_t valid_bus_count = 0;

        for (uint8_t b = 0; b < (WLED_MAX_BUSSES + WLED_MIN_VIRTUAL_BUSSES); b++)
        {
          BusConfig *cfg = tkr_iLight->busConfigs[b];
          if (!cfg) continue;

          uint32_t start = cfg->start;
          uint32_t stop  = (cfg->length > 0) ? (uint32_t)start + cfg->length - 1 : start;

          // Valid = start < stop and not equal (i.e. at least 2 pixels)
          if (cfg->length == 0)   continue;
          if (stop <= start)      continue;

          valid_bus_idx[valid_bus_count++] = b;
        }

        if (valid_bus_count == 0)
        {
          ALOG_ERR(PSTR(D_LOG_PIXEL "Segments/MatchBus but no valid BusConfig entries"));
        }
        else
        {
          uint8_t max_segments = min<uint8_t>(valid_bus_count, MAX_NUM_SEGMENTS);

          for (uint8_t seg_i = 0; seg_i < max_segments; seg_i++)
          {
            // Ensure segment exists
            if (seg_i >= segments.size())
            {
              uint16_t seg_size_before = segments.size();
              Segment_AppendNew(0, 10, seg_i);
              ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"),
                       seg_i, segments.size() - seg_size_before, segments.size());
            }

            // Pick template: replicate last template if fewer Segments than buses
            uint8_t templ_idx = (seg_i < seg_arr.size())
                                ? seg_i
                                : (uint8_t)(seg_arr.size() - 1);
            JsonParserObject seg_obj = seg_arr[templ_idx];

            // Parse full segment config from template
            subparse_JSONCommand(seg_obj, seg_i);

            // Now override PixelRange from corresponding valid BusConfig
            BusConfig *cfg = tkr_iLight->busConfigs[ valid_bus_idx[seg_i] ];
            uint16_t  start = cfg->start;
            uint32_t  stop32 = (cfg->length > 0)
                               ? (uint32_t)start + cfg->length - 1
                               : start;
            uint16_t  stop  = (stop32 > PIXEL_RANGE_LIMIT)
                              ? PIXEL_RANGE_LIMIT
                              : (uint16_t)stop32;

            SEGMENT_I(seg_i).start = start;
            SEGMENT_I(seg_i).stop  = stop;

            ALOG_INF(PSTR(D_LOG_PIXEL "PixelRange MatchBus: seg %d → bus %d [%u,%u] (len %u)"),
                     seg_i,
                     valid_bus_idx[seg_i],
                     SEGMENT_I(seg_i).start,
                     SEGMENT_I(seg_i).stop,
                     cfg->length);
          }

          segments_found   = max_segments;
          handled_matchbus = true;
        }
      }
    }

    // --- [2b] Normal Segments array (no MatchBus) ---
    if (!handled_matchbus)
    {
      for (uint8_t i = 0; i < seg_arr.size() && i < MAX_NUM_SEGMENTS; i++)
      {
        JsonParserObject seg_obj = seg_arr[i];
        if (i >= segments.size())
        {
          uint16_t seg_size = segments.size();
          Segment_AppendNew(0, 10, i);
          ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"),
                   i, segments.size() - seg_size, segments.size());
        }
        subparse_JSONCommand(seg_obj, i);
        segments_found++;
      }
    }
  }

  #endif

  // === [3] Check for keys that match segment names === ie the new SegmentName can be used directly to group segment commands
  if (segments_found == 0) {
    for (uint8_t i = 0; i < segments.size(); i++) {
      const char* seg_name = SEGMENT_I(i).getName();
      if (!seg_name || !*seg_name) continue;
      if (jtok = obj[seg_name]) {
        ALOG_INF(PSTR("Seg: \"%s\" (name match)"), seg_name);
        subparse_JSONCommand(jtok.getObject(), i);
        segments_found++;
      }
    }
  }


  // When no direct segment is set, assumed they are single segment (or control of first segment)
  if(segments_found == 0)
  {
    ALOG_INF(PSTR(D_LOG_NEO "Assumed main segment"));
    subparse_JSONCommand(obj, 0); // Legacy commands
  }

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::End"));

}

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
void mAnimatorLight::subparse_MatrixConfig(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

  Panel p;

  if(jtok = obj["Width"])
  {
    p.width = jtok.getInt();
  }
  if(jtok = obj["Height"])
  {
    p.height = jtok.getInt();
  }
  if(jtok = obj["StartX"])
  {
    p.xOffset = jtok.getInt();
  }
  if(jtok = obj["StartY"])
  {
    p.yOffset = jtok.getInt();
  }
  if(jtok = obj["BottomStart"])
  {
    p.bottomStart = jtok.getInt();
  }
  if(jtok = obj["RightStart"])
  {
    p.rightStart = jtok.getInt();
  }
  if(jtok = obj["Vertical"])
  {
    p.vertical = jtok.getInt();
  }
  if(jtok = obj["Serpentine"])
  {
    p.serpentine = jtok.getInt();
  }

  ALOG_INF(PSTR(
    "MatrixConfig[%d]: %dx%d, StartX:%d, StartY:%d, BottomStart:%d, RightStart:%d, Vertical:%d, Serpentine:%d"),
    panels, p.width, p.height, p.xOffset, p.yOffset, p.bottomStart, p.rightStart, p.vertical, p.serpentine
  );
  
  panel.push_back(p);
  ALOG_INF(PSTR("panels %d" ), panel.size());
  
  isMatrix = true;

}
#endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS


/**
 * @brief Allows new segment controls, by calling this function for each segment.
 *        Calling it without any set index, will default to segment of 0 index and turn the other segments off 
 * @param obj 
 */
void mAnimatorLight::subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index)
{
  ALOG_HGLT(PSTR("subparse_JSONCommand for segment %d"), segment_index);

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

  uint16_t isserviced_start_count = data_buffer.isserviced; // to know if anything was serviced in this function

  /**
   * @brief Exit if segment has not been created to stop errors
   **/
  if(!segments.size()){ 
    ALOG_ERR(PSTR("No segments created"));
    return; 
  }


  if (jtok = obj["PaletteMappingValues"]) { 
    if (jtok.isArray()) {
      // Pre-allocate space in the vector to avoid repeated memory allocation
      auto& mapping_values = SEGMENT_I(segment_index).palette->mapping_values;
      mapping_values.clear(); // reset old map
      mapping_values.reserve(16); // reserve space for 16 elements
      JsonParserArray arrobj = jtok;
      for (auto v : arrobj) {
        if (mapping_values.size() >= 16) { break; }
        mapping_values.push_back(v.getFloat());
      }
      data_buffer.isserviced++;
    }
  }

  
  if(jtok = getTokenIncludingAlias(obj, PM_COLOUR_PALETTE, "Pal"))
  {
    if(jtok.isStr())
    {
      if(strcmp(jtok.getStr(),"+")==0)
      {
        CommandSet_PaletteID( SEGMENT_I(segment_index).palette_id + 1, segment_index);
      }else
      if(strcmp(jtok.getStr(),"-")==0)
      {
        CommandSet_PaletteID( SEGMENT_I(segment_index).palette_id - 1, segment_index);
      }else
      if((tmp_id=GetPaletteIDbyName((char*)jtok.getStr()))>=0)
      {
        CommandSet_PaletteID(tmp_id, segment_index);
      }
    }else
    if(jtok.isNum()){
      CommandSet_PaletteID(jtok.getInt(), segment_index);
    }
    ALOG_INF( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)) );
    data_buffer.isserviced++;
  }


  if(jtok = getTokenIncludingAlias(obj, "Palette2", "Pal2"))
  {
    if(jtok.isStr())
    {
      if(strcmp(jtok.getStr(),"+")==0)
      {
        CommandSet_Palette2ID( SEGMENT_I(segment_index).palette2_id + 1, segment_index);
      }else
      if(strcmp(jtok.getStr(),"-")==0)
      {
        CommandSet_Palette2ID( SEGMENT_I(segment_index).palette2_id - 1, segment_index);
      }else
      if((tmp_id=GetPaletteIDbyName((char*)jtok.getStr()))>=0)
      {
        CommandSet_Palette2ID(tmp_id, segment_index);
      }
    }else
    if(jtok.isNum()){
      CommandSet_Palette2ID(jtok.getInt(), segment_index);
    }
    ALOG_INF( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)) );
    data_buffer.isserviced++;
  }

#ifndef ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  if(jtok = getTokenIncludingAlias(obj, PM_PIXELRANGE, "PR"))//obj[PM_PIXELRANGE])
  { 
    if(jtok.isArray())
    {
      JsonParserArray arrobj = jtok;
      SEGMENT_I(segment_index).start = arrobj[0].getInt();
      SEGMENT_I(segment_index).stop  = arrobj[1].getInt();
      
      if(SEGMENT_I(segment_index).stop > PIXEL_RANGE_LIMIT)
      {
        ALOG_ERR( PSTR("stop %d exceeds max %d"), SEGMENT_I(segment_index).stop, PIXEL_RANGE_LIMIT);
        SEGMENT_I(segment_index).stop = PIXEL_RANGE_LIMIT;
      }

      if(arrobj.size() == 4)
      {
        SEGMENT_I(segment_index).startY = arrobj[3].getInt();
        SEGMENT_I(segment_index).stopY  = arrobj[4].getInt();
      }

      ALOG_INF( PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"), SEGMENT_I(segment_index).start, SEGMENT_I(segment_index).stop );
      data_buffer.isserviced++;
    }
  }
  #else
    if (jtok = getTokenIncludingAlias(obj, PM_PIXELRANGE, "PR"))//obj[PM_PIXELRANGE])
  {
    if (jtok.isArray())
    {
      JsonParserArray arrobj = jtok;

      // X range
      SEGMENT_I(segment_index).start = arrobj[0].getInt();
      SEGMENT_I(segment_index).stop  = arrobj[1].getInt();

      if (SEGMENT_I(segment_index).stop > PIXEL_RANGE_LIMIT)
      {
        ALOG_ERR(PSTR("stop %d exceeds max %d"),
                 SEGMENT_I(segment_index).stop, PIXEL_RANGE_LIMIT);
        SEGMENT_I(segment_index).stop = PIXEL_RANGE_LIMIT;
      }

      // Optional Y range: [start, stop, startY, stopY]
      if (arrobj.size() >= 4)
      {
        SEGMENT_I(segment_index).startY = arrobj[2].getInt();
        SEGMENT_I(segment_index).stopY  = arrobj[3].getInt();
      }

      ALOG_INF(PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"),
               SEGMENT_I(segment_index).start,
               SEGMENT_I(segment_index).stop);
      data_buffer.isserviced++;
    }
    else
    {
      // Legacy path never used MatchBus, so non-array is an error
      ALOG_ERR(PSTR(D_LOG_PIXEL "PixelRange expects array [start,stop(,startY,stopY)]"));
    }
  }
  #endif


  if(jtok = obj["Name"])
  {
    SEGMENT_I(segment_index).NameUpdate(jtok.getStr());
    ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_NAME)), SEGMENT_I(segment_index).getName() );
    data_buffer.isserviced++;
  }

  if(jtok = obj["ColourType"])
  {
    ALOG_HGL(PSTR("CRITICAL ColourType should be inside EFFECT:ColourType"));
  }



  /*************************************************************************
   *** {"Effects":{X:Y}}
   *************************************************************************/
  if(jtok_sub = getTokenIncludingAlias(obj, PM_EFFECTS, "EF"))//obj[PM_EFFECTS])
  {    
    JsonParserObject jobj = jtok_sub.getObject();

    if(jtok = getTokenIncludingAlias(jobj, PM_FUNCTION, "FX"))
    { 
      if(jtok.isStr())
      {
        if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0)
        {
          CommandSet_Flasher_FunctionID(tmp_id, segment_index);
        }
      }else
      if(jtok.isNum())
      {
        CommandSet_Flasher_FunctionID(jtok.getInt(), segment_index);
      }
      ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_SVALUE_K(D_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
      data_buffer.isserviced++;
    }


    /**
    EFFECT_SUBTYPE removed, replaced by colourtype
    **/
    if(jtok = jobj[PM_COLOUR_TYPE])
    {
      CommandSet_Effect_ColourTypeID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    

    if(jtok = getTokenIncludingAlias(jobj, PM_INTENSITY, "IX"))
    { 
      CommandSet_Effect_Intensity(jtok.getInt(), segment_index);
      ALOG_INF( PSTR(D_LOG_PIXEL D_COMMAND_2KEY_NVALUE_K(D_EFFECTS, D_INTENSITY)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = getTokenIncludingAlias(jobj, PM_SPEED, "SX"))
    { 
      CommandSet_Effect_Speed(jtok.getInt(), segment_index);
      ALOG_INF( PSTR(D_LOG_PIXEL D_COMMAND_2KEY_NVALUE_K(D_EFFECTS, D_SPEED)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = getTokenIncludingAlias(jobj, PM_REVERSE, "RV"))
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_REVERSED, jtok.getInt());  
      ALOG_INF( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_REVERSE)), SEGMENT_I(segment_index).reverse);
      data_buffer.isserviced++;
    }


    if(jtok = getTokenIncludingAlias(jobj, PM_MIRROR, "MR"))
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_MIRROR, jtok.getInt());  
      ALOG_INF( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_MIRROR)), SEGMENT_I(segment_index).mirror);
      data_buffer.isserviced++;
    }


    if(jtok = getTokenIncludingAlias(jobj, PM_GROUPING, "GP"))
    { 
      SEGMENT_I(segment_index).grouping = jtok.getInt();  
      ALOG_INF( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
      #ifdef ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_STRIP
      if(SEGMENT_I(segment_index).grouping > (SEGMENT_I(segment_index).length()/4)){
        SEGMENT_I(segment_index).grouping = SEGMENT_I(segment_index).length()/4;
        ALOG_WRN(PSTR(D_LOG_PIXEL "Grouping overridden to %d to fit segment length"), SEGMENT_I(segment_index).grouping);
      }
      #endif
      #ifdef ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_MAX_COUNT
      if(SEGMENT_I(segment_index).grouping > (SEGMENT_I(segment_index).length()/ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_MAX_COUNT)){
        SEGMENT_I(segment_index).grouping = SEGMENT_I(segment_index).length()/ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_MAX_COUNT;
        ALOG_WRN(PSTR(D_LOG_PIXEL "Grouping overridden to %d to fit segment length"), SEGMENT_I(segment_index).grouping);
      }
      #endif
      data_buffer.isserviced++;
    }

    
    if(jtok = jobj["GroupingPerc"])
    {
      uint8_t percentage = jtok.getInt(); 
      SEGMENT_I(segment_index).grouping = map(percentage,0,100,0,SEGMENT_I(segment_index).length());    
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
      data_buffer.isserviced++;
    }


    if(jtok = getTokenIncludingAlias(jobj, PM_DECIMATE, "DC"))
    {
      SEGMENT_I(segment_index).decimate = jtok.getInt();  
      ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_DECIMATE)), SEGMENT_I(segment_index).decimate);
      data_buffer.isserviced++;
    }

    if (jtok = jobj["DecimatePerc"])
    {
      uint8_t percentage = jtok.getInt();  // Requested compute percentage (0–100)
      ALOG_INF(PSTR(D_LOG_PIXEL " DecimatePerc input = %d"), percentage);

      // --- CASE 1: 0% → Disable decimation entirely ---
      //
      // Meaning:
      // - Compute at full virtual resolution
      // - Pretend decimate does not exist
      //
      if (percentage == 0)
      {
        SEGMENT_I(segment_index).decimate = 0;   // Flag meaning "feature OFF"
        ALOG_INF(PSTR(D_LOG_PIXEL " Decimation disabled (DecimatePerc = 0)."));
        data_buffer.isserviced++;
        return;
      }

      // --- Clamp percentage to sane range (1–100 only) ---
      if (percentage > 100) percentage = 100;

      // --- CASE 2: 1–100% → Active compute percentage ---
      //
      // decimate = 100 / percentage
      //
      // Examples:
      //   100% → decimate = 1  (full res, same as 0)
      //    50% → decimate = 2  (half compute)
      //    25% → decimate = 4  (quarter compute)
      //    10% → decimate = 10 (10% compute)
      //
      // Only decimate >= 2 actually reduces resolution.
      //
      uint8_t dec = (uint8_t)((100 + (percentage / 2)) / percentage);  // rounded

      if (dec < 1) dec = 1;   // Safety: min = 1

      SEGMENT_I(segment_index).decimate = dec;

      ALOG_INF(
        PSTR(D_LOG_PIXEL " DecimatePerc = %d -> decimate = %d (compute ≈ %d%%)"),
        percentage, dec, 100 / dec
      );

      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_SPACING])
    { 
      SEGMENT_I(segment_index).spacing = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_SPACING)), SEGMENT_I(segment_index).spacing);
      data_buffer.isserviced++;
    }

      
    if(jtok = jobj[PM_OFFSET])
    { 
      SEGMENT_I(segment_index).offset = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_OFFSET)), SEGMENT_I(segment_index).offset);
      data_buffer.isserviced++;
    }
      

    if(jtok = jobj[PM_RATE]){ // default to secs
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_RATE)), SEGMENT_I(segment_index).cycle_time__rate_ms);
      data_buffer.isserviced++;
    }else
    if(jtok = getTokenIncludingAlias(jobj, PM_RATE_MS, "RA"))
    {
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms);  
      data_buffer.isserviced++;
    }


    if (jtok = jobj["Params"]) { 
      if (jtok.isArray()) {
        JsonParserArray arrobj = jtok;
        uint8_t array[4] = {0};  // Initialize the array to store values
        arrobj.getArray(array);  // Retrieve array elements

        // Assign retrieved values to SEGMENT_I params_user
        auto& params_user = SEGMENT_I(segment_index).params_user;
        std::copy(array, array + 4, params_user);

        ALOG_COM(PSTR(D_LOG_PIXEL "Params %d = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
        data_buffer.isserviced++;
      }
    }else{
      char keyname[10];
      for(int i=0;i<4;i++)
      {
        snprintf(keyname, sizeof(keyname), "Param%d", i);
        if(jtok = jobj[keyname])
        {
          auto& params_user = SEGMENT_I(segment_index).params_user;
          params_user[i] = jtok.getInt();
          ALOG_INF(PSTR(D_LOG_PIXEL "Params %d = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
          data_buffer.isserviced++;
        }
      }
    }    


    if (jtok = jobj["Custom1"]) 
    {
      SEGMENT_I(segment_index).custom1 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom1 %d"), SEGMENT_I(segment_index).custom1 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Custom2"]) 
    {
      SEGMENT_I(segment_index).custom2 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom2 %d"), SEGMENT_I(segment_index).custom2 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Custom3"]) 
    {
      uint8_t c3 = jtok.getInt();
      if(c3 > 31) // only has 5 of the 8 bits
      {
        ALOG_ERR(PSTR("Custom3 exceeds limit 31"));
        c3 = map(c3, 0,255, 0,31); // assume mapping needed
      }
      SEGMENT_I(segment_index).custom3 = c3;
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom3 %d"), SEGMENT_I(segment_index).custom3 );
      data_buffer.isserviced++;
    }

    
    if (jtok = jobj["Check1"]) 
    {
      SEGMENT_I(segment_index).check1 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check1 %d"), SEGMENT_I(segment_index).check1 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Check2"]) 
    {
      SEGMENT_I(segment_index).check2 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check2 %d"), SEGMENT_I(segment_index).check2 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Check3"]) 
    {
      SEGMENT_I(segment_index).check3 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check3 %d"), SEGMENT_I(segment_index).check3 );
      data_buffer.isserviced++;
    }


    if (jtok = jobj["PalIX"]) 
    {
      SEGMENT_I(segment_index).live_palette.intensity = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "PalIX %d"), SEGMENT_I(segment_index).live_palette.intensity );
      data_buffer.isserviced++;
    }
    

  } // PM_EFFECTS


  /**
   * @brief Example commands
   * 
    {
      "CustomPalette": {
        "Data": [
          0,      255,      255,      255,
          150,      0,      255,      0,
          255,      0,      0,      255
        ],
        "Encoding": "gRGB",
        "Index": 9
      }
    }
    {
      "CustomPalette": {
        "Data": [
          255,      255,      255,
          0,      255,      0,
          0,      0,      255
        ],
        "Encoding": "RGB",
        "Index": 8
      }
    }
   * 
   */
  if(jtok = obj["CustomPalette"])
  {
    JsonParserToken jtok_sub = 0;

    uint8_t index = 0;
    if(jtok_sub = jtok.getObject()["Index"])
    {
      index = jtok_sub.getInt();
      ALOG_DBM(PSTR("Index %d"), index);
    }

    mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX}; // 0b00000111 0x07
    if(jtok_sub = jtok.getObject()["Encoding"])
    {
      if(jtok_sub.isStr())
      {
        if(strcmp(jtok_sub.getStr(),"RGB")==0){ encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX}; }
        else if(strcmp(jtok_sub.getStr(),"gRGB")==0){ encoding = {PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT}; }
      }
      else
      {
        uint16_t enc = jtok_sub.getInt();
        encoding = {enc};
      }
      ALOG_DBG(PSTR("Encoding %d"), encoding);
    }


    if (jtok_sub = jtok.getObject()["Data"])
    {
      if (jtok_sub.isArray())
      {
        ALOG_DBM(PSTR("Data length %d"), jtok_sub.size());
        // Create a JsonParserArray object to use array-specific methods
        JsonParserArray arrobj = jtok_sub;
        // Define an array large enough to store up to 255 elements
        uint8_t array[255] = {0};
        // Use the getArray method directly to fill the array
        arrobj.getArray(array);
        // Calculate the actual length (since `getArray` stops at the array's size limit)
        uint8_t data_length = arrobj.size() < 255 ? arrobj.size() : 255;
        // Perform your command with the filled array and its length
        CommandSet_CustomPalette(index, encoding.data, array, data_length);
        data_buffer.isserviced++;
      }
    }
  }

  #ifdef ENABLE_DEVFEATURE_LIGHT__CUSTOM_PIXEL_DATA
  if (jtok = obj["CustomPixelData"])
  {
    ALOG_INF(PSTR("CustomPixelData received"));

    JsonParserToken jtok_sub = 0;

    // --- Step 1: Parse Index array ---
    if (!(jtok_sub = jtok.getObject()["Index"])) {
      ALOG_ERR(PSTR("Missing 'Index' array"));
      return;
    }
    JsonParserArray arr_index = jtok_sub;

    uint16_t data_length = arr_index.size();
    data_length = data_length < 255 ? data_length : 255;

    ALOG_INF(PSTR("Parsed Index array, size = %d"), data_length);

    uint16_t index_list[data_length];
    uint8_t  rgb_list[data_length][3];

    for (uint16_t i = 0; i < data_length; ++i) {
      index_list[i] = arr_index[i].getInt();
      ALOG_INF(PSTR("Index[%d] = %d"), i, index_list[i]);
    }

    // --- Step 2: Parse RGB array ---
    if (!(jtok_sub = jtok.getObject()["RGB"])) {
      ALOG_ERR(PSTR("Missing 'RGB' array"));
      return;
    }
    JsonParserArray arr_rgb = jtok_sub;

    if (arr_rgb.size() < data_length) {
      ALOG_ERR(PSTR("RGB array too short (%d < %d); truncating"), arr_rgb.size(), data_length);
      data_length = arr_rgb.size(); // Truncate to match available RGBs
    }

    ALOG_INF(PSTR("Parsed RGB array, size = %d"), arr_rgb.size());

    for (uint16_t i = 0; i < data_length; ++i) {
      JsonParserArray rgb = arr_rgb[i];
      rgb_list[i][0] = rgb[0].getInt();
      rgb_list[i][1] = rgb[1].getInt();
      rgb_list[i][2] = rgb[2].getInt();
      ALOG_INF(PSTR("RGB[%d] = %d,%d,%d"), i, rgb_list[i][0], rgb_list[i][1], rgb_list[i][2]);
    }

    // --- Step 3: Allocate and write data buffer ---
    size_t total_bytes = data_length * 5;
    ALOG_INF(PSTR("Preparing to write %d pixels (%d bytes)"), data_length, total_bytes);

    if (data_length > 0 && SEGMENT.allocateData(total_bytes)) {
      byte* buffer = SEGMENT.Data();
      for (uint16_t i = 0; i < data_length; ++i) {
        buffer[i*5 + 0] = index_list[i] >> 8;
        buffer[i*5 + 1] = index_list[i] & 0xFF;
        buffer[i*5 + 2] = rgb_list[i][0];
        buffer[i*5 + 3] = rgb_list[i][1];
        buffer[i*5 + 4] = rgb_list[i][2];
      }
      SEGMENT._dataLen = total_bytes;
      SEGMENT.check1 = 1;
      data_buffer.isserviced++;
      ALOG_INF(PSTR("Successfully stored %d pixels into buffer (%d bytes)"), data_length, total_bytes);
    } else {
      ALOG_ERR(PSTR("CustomPixelData failed: allocation failed"));
    }
  }
  #endif


  #ifdef ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES
  /**
   * @brief Construct a new if object
   * {
    "HeatmapPalettes": {
      "Data": [1,2,3,4],
      "Gradient": [0,100,200,255],
      "Max":4,
      "Min":1,
      "Palette":"Jet 16"
    }
  }
  * 
  */
  if (jtok = obj["HeatmapPalettes"]) 
  {
      JsonParserToken jtok_sub = 0;
      
      // Read float array (data points)
      std::vector<float> heatmap_floats;
      if (jtok_sub = jtok.getObject()["Data"]) {
          if (jtok_sub.isArray()) {
              JsonParserArray arrobj = jtok_sub;
              for (int i = 0; i < arrobj.size(); ++i) {
                  heatmap_floats.push_back(arrobj[i].getFloat());
              }
              ALOG_INF(PSTR("Heatmap data length %d"), heatmap_floats.size());
          }
      }
      
      // Read Gradient mapping (array of 0-255)
      // std::vector<uint8_t> gradient_map;
      // if (jtok_sub = jtok.getObject()["Gradient"]) {
      //     if (jtok_sub.isArray()) {
      //         JsonParserArray arrobj = jtok_sub;
      //         for (int i = 0; i < arrobj.size(); ++i) {
      //             gradient_map.push_back(arrobj[i].getInt());
      //         }
      //         ALOG_INF(PSTR("Gradient length %d"), gradient_map.size());
      //     }
      // }

      uint8_t gradient_index = mSupport::mapfloat()
      
      // Read Max and Min values for the heatmap
      float max_value = 1.0f, min_value = 0.0f;
      if (jtok_sub = jtok.getObject()["Max"]) {
          max_value = jtok_sub.getFloat();
      }
      if (jtok_sub = jtok.getObject()["Min"]) {
          min_value = jtok_sub.getFloat();
      }
      
      // Read Palette to copy from
      uint8_t palette_index = 0;
      if(jtok_sub = jtok.getObject()["Palette"])
      {
        if(jtok_sub.isStr())
        {
          if((tmp_id=GetPaletteIDbyName((char*)jtok_sub.getStr()))>=0)
          {
            palette_index = tmp_id;
          }
        }else
        if(jtok_sub.isNum()){
          palette_index = jtok_sub.getInt();
        }
        
        ALOG_INF(PSTR("Using Palette Index %d"), palette_index);
        data_buffer.isserviced++;
      }

      /**
       * @brief Since we are going to copy from an existing palette, we want to get the encoding, and then get the data.
       * Since encoding is not always used, the way forward might be to :
       * ** load palette
       * ** access what was loaded, copy/modify into CustomPalette
       * ** reload and set palette as the custom option
       */
      mPalette::PALETTE_ENCODING_DATA encoding =  mPaletteI->findPaletteEncoding(palette_index);
      if(encoding.encoded_as_crgb_palette_16)
      {
        ALOG_INF(PSTR("Encoded as CRGB16Palette"));
      }
      if( // Switch to bit masking
        (encoding.red_enabled)||
        (encoding.green_enabled)||
        (encoding.blue_enabled)||
        (encoding.white_cold_enabled)||
        (encoding.white_warm_enabled)
      ){
        ALOG_INF(PSTR("Encoded as RGBWW"));
      }
      if(encoding.index_gradient)
      {
        ALOG_INF(PSTR("Encoded with gradient that I need to think of"));
      }
      Serial.println(encoding.data, BIN);


      // if()

      // // Get the encoding for the heatmap palette
      // mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
      // if (jtok_sub = jtok.getObject()["Encoding"]) {
      //     if (jtok_sub.isStr()) {
      //         if (strcmp(jtok_sub.getStr(),"RGB")==0) {
      //             encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
      //         } else if (strcmp(jtok_sub.getStr(),"gRGB")==0) {
      //             encoding = {PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT};
      //         }
      //     } else {
      //         uint16_t enc = jtok_sub.getInt();
      //         encoding = {enc};
      //     }
      //     ALOG_DBG(PSTR("Encoding %d"), encoding);
      // }

      // // Process the heatmap data points and assign colors
      // std::vector<uint8_t> final_palette_data;

      // for (size_t i = 0; i < heatmap_floats.size(); ++i) {
      //     float normalized_value = (heatmap_floats[i] - min_value) / (max_value - min_value);
      //     uint8_t gradient_index = gradient_map[i]; // This assumes a matching 1-to-1 relation of float to gradient

      //     // If gradient_index is a float between 0-1, scale it to 0-255
      //     gradient_index = uint8_t(normalized_value * 255);

      //     // Get the corresponding color from the palette
      //     uint32_t color = GetPaletteColour_Legacy(palette_index, gradient_index, encoding);
          
      //     // Add the color to the final palette data (assuming RGB, 3 bytes per color)
      //     final_palette_data.push_back((color >> 16) & 0xFF); // Red
      //     final_palette_data.push_back((color >> 8) & 0xFF);  // Green
      //     final_palette_data.push_back(color & 0xFF);         // Blue
      // }

      // // Command to set the heatmap palette using the computed final_palette_data
      // CommandSet_HeatmapPalette(palette_index, encoding.data, final_palette_data);
  }
  #endif // ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES
  

  #if FIRMWARE_VERSION_MIN(1,1)

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION_SHORTCUT
    if(jtok = obj["LightNotif"])
    { 
      // parsesub_LightNotif(obj);
      if(jtok.isArray())
      {
        JsonParserArray arrobj = jtok;
        for(auto v : arrobj) 
        {
          // bool result = v.isObject();
          // Serial.println(v.isObject());

          parsesub_json_object_notification_shortcut(v.getObject());

          // JsonParserObject obj1 = v.getObject();
          // JsonParserToken jtok1 = 0;


          // if(jtok1 = obj1["SegIndex"])
          // {
          //   ALOG_INF(PSTR("SegIndex %d"),jtok1.getInt());
          // }

        }
      }

      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(0).segcol[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(0).segcol[0].getBrightnessRGB());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(1).segcol[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(1).segcol[0].getBrightnessRGB());


      // "LightNotif":
      //   [
      //     {
      //       "SegIndex":0,
      //       "Colour":[123,100,100],
      //       "Data":[start, stop, notif_effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //     },
      //     {
      //       "SegIndex":0,
      //       "Colour":[123,100,100],
      //       "Data":[start, stop, notif_effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //     }
      //   ]
      //     "Values":[segindex, start, stop, hue, sat, brightness, effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //   ],
      //   [
      //     "values":[start, stop, period_ms, repeats, duty_cycle, timeout_seconds],
      //     "colour":[120,100,100]
      //   ]
      // }
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
    if(jtok = obj[PM_ANIMATIONMODE]){
      if(jtok.isStr()){
        if((tmp_id=GetAnimationModeIDbyName(jtok.getStr()))>=0){
          CommandSet_AnimationModeID(tmp_id);
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){      
        CommandSet_AnimationModeID(jtok.getInt());
        data_buffer.isserviced++;
      }
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
  
  #endif // FIRMWARE_VERSION_MIN
  

  #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(jtok = obj[PM_RGB_CLOCK].getObject()[PM_MANUAL_NUMBER]){
    lcd_display_show_number = jtok.getInt();
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_MANUAL_NUMBER)), lcd_display_show_number);
  }
  
  if(jtok = obj[PM_RGB_CLOCK].getObject()["ManualString"]){
    strcpy(lcd_display_show_string, jtok.getStr());
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_SVALUE_K("ManualString")), lcd_display_show_string);
  }
  #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


  #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
  // /**
  //  * @brief When these are commanded, the "animation_mode" is switched and the setpixel will be set here directly
  //  */
  // if(jtok = obj["MQTTPixel"])
  // {

  //   JsonParserToken jtok_sub = 0;

  //   realtimeMode = ANIMATION_MODE__REALTIME_MQTT_SETPIXEL;
    
  //   // Method 1: RGB control
  //   // if(jtok_sub = jtok.getObject()["Index"])
  //   // {
  //   //   index = jtok_sub.getInt();
  //   //   ALOG_DBM(PSTR("Index %d"), index);
  //   // }

  //   ALOG_INF(PSTR("Add command which switches to pad below"));// so left/middle/row or whatever, [[fill out min/max],[fill out min/max]]

  //   // Method 2: Set the "on pixels", which uses the configured palette on repeat
  //   uint8_t index = 0;
  //   if (jtok_sub = jtok.getObject()["OnPixels"]) {
  //       if (jtok_sub.isArray()) {
  //           ALOG_COM(PSTR("is ARRAY"));

  //           SEGMENT.fill(0); // Clear all to off

  //           uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness
  //           ALOG_INF(PSTR("brightness %d"), brightness);

  //           uint32_t colour;
  //           uint16_t pixel = 0;
  //           JsonParserArray array = jtok_sub;

  //           JsonParserToken token = array;
  //           token.nextOne(); // Skip the array itself and move to the first element

  //           uint16_t count = 0; // Keep track of processed items
  //           while (token.isValid()) { // Process tokens while valid
  //               if (token.isNum()) { // Ensure the token is a number
  //                   int pixelIndex = token.getInt();
  //                   ALOG_INF(PSTR("OnPixel %d (processed %d)"), pixelIndex, count);

  //                   // Get the color from the palette
  //                   colour = SEGMENT.GetPaletteColour_Legacy(pixel++, PALETTE_INDEX__IS_SEGLEN_RANGE, PALETTE_WRAP_HARDEDGE, PALETTE_MODE__FORCE_DISCRETE, NO_ENCODED_VALUE);

  //                   brightness = 255;
  //                   // colour = RgbcctColor(255,255,255,255,255);
  //                   // Set the pixel color with brightness
  //                   SEGMENT.setPixelColor(pixelIndex, AdjustColourWithBrightness(colour,brightness));



  //                   count++;
  //               } else {
  //                   ALOG_ERR(PSTR("Invalid token type inside array"));
  //               }

  //               // Move to the next token
  //               token.nextOne();
  //           }

  //           ALOG_INF(PSTR("Processed %d pixels in total"), count);

  //           SEGMENT.cycle_time__rate_ms = FRAMETIME;
  //           SET_DIRECT_MODE();

  //           show();
  //       }
  //   }

    
  // }
 
  
//   // #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
// /**
//  * @brief Realtime pixel debug via MQTT.
//  * When these are commanded, animation_mode is switched and pixels are written directly.
//  * 
//   "MQTTPixel": {
//     "OnPixelsWithIndex": [
//       [0, 0, 65],        // paletteIndex=0, range 0..65
//       [1, 66, 131],      // paletteIndex=1, range 66..131
//       [0, 132, 134],     // paletteIndex=0, range 132..134
//       [2, 136]           // paletteIndex=2, single pixel 136
//     ],
//     "OnPixelsFill": true
//   }


//   "MQTTPixel": {
//   "BrightnessRGB": 255,
//   "ColourPalette":"Colour and White Stripe",
//   "Effects":{"Grouping":1},
//     "OnPixelsWithIndex": [
//       [0, 0, 65],
//       [1, 66, 131],
//       [0, 132, 134],
//       [2, 136]
//     ],
//     "OnPixelsFill": true
//   }

//  */
if (jtok = obj["MQTTPixel"])
{
  JsonParserToken jtok_sub = 0;
  JsonParserToken jtok_sub2 = 0;

  realtimeMode = ANIMATION_MODE__REALTIME_MQTT_SETPIXEL;

  // ============================================================
  // Method 2: OnPixels (legacy)
  // Format:
  //   "MQTTPixel": { "OnPixels": [ 1, 5, 10, ... ] }
  // Each pixel is set using the configured palette on repeat.
  // ============================================================
  uint8_t index = 0;
  if ((jtok_sub = jtok.getObject()["OnPixels"]) && jtok_sub.isArray())
  {
    ALOG_COM(PSTR("OnPixels is ARRAY"));

    SEGMENT.fill(0); // Clear all to off

    uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness
    ALOG_INF(PSTR("brightness %d"), brightness);

    uint32_t colour;
    uint16_t pixel = 0;

    JsonParserArray array = jtok_sub;
    JsonParserToken token = array;
    token.nextOne(); // move to first element

    uint16_t count = 0;
    while (token.isValid())
    {
      if (token.isNum())
      {
        int pixelIndex = token.getInt();
        ALOG_INF(PSTR("OnPixel %d (processed %d)"), pixelIndex, count);

        // Palette-driven colour, repeats across selected pixels
        colour = SEGMENT.GetPaletteColour_Legacy(
          pixel++,
          PALETTE_INDEX__IS_SEGLEN_RANGE,
          PALETTE_WRAP_HARDEDGE,
          PALETTE_MODE__FORCE_DISCRETE,
          NO_ENCODED_VALUE
        );

        // If you want global brightness applied here, keep AdjustColourWithBrightness()
        // If you want raw palette values, remove the adjust call.
        SEGMENT.setPixelColor(pixelIndex, AdjustColourWithBrightness(colour, brightness));

        count++;
      }
      else
      {
        ALOG_ERR(PSTR("Invalid token type inside OnPixels array"));
      }

      token.nextOne();
    }

    ALOG_INF(PSTR("Processed %d pixels in total"), count);

    SEGMENT.cycle_time__rate_ms = FRAMETIME;
    SET_DIRECT_MODE();
    show();
  }

//   // ============================================================
//   // Method 3: OnPixelsWithIndex (new)
//   //
//   // New payload format
//   //
//   // OnPixelsWithIndex: array of groups
//   // each group: [paletteIndex, ...pixelsOrRange...]
//   //
//   // OnPixelsFill: true (optional): treat each group’s pixels as min..max fill,
//   // so you don’t have to list everything
//   //
//   // Examples:
//   //
//   // "MQTTPixel": {
//   //   "OnPixelsWithIndex": [
//   //     [0, 0, 65],        // paletteIndex=0, range 0..65
//   //     [1, 66, 131],      // paletteIndex=1, range 66..131
//   //     [0, 132, 134],     // paletteIndex=0, range 132..134
//   //     [2, 136]           // paletteIndex=2, single pixel 136
//   //   ],
//   //   "OnPixelsFill": true
//   // }
//   //
//   // If OnPixelsFill is false / missing, you can still do:
//   //   [idx, p]                 single pixel
//   //   [idx, a, b]              range (min..max)
//   //   [idx, p1, p2, p3, ...]   explicit list (each is a pixel index)
//   // ============================================================

//   // Allows grouped pixel selection with an explicit palette index per group.
//   // Optional fill mode enables fast section colouring / wiring debug tags.
//   JsonParserToken jtok_pwi = jtok.getObject()["OnPixelsWithIndex"];

//   if (jtok_pwi && jtok_pwi.isArray())
//   {
//     // Optional fill flag
//     bool flag_fill = false;
//     if ((jtok_sub2 = jtok.getObject()["OnPixelsFill"]))
//     {
//       if (jtok_sub2.isBool()) flag_fill = jtok_sub2.getBool();
//       else if (jtok_sub2.isNum()) flag_fill = (jtok_sub2.getInt() != 0);
//     }

//     ALOG_COM(PSTR("OnPixelsWithIndex is ARRAY, fill=%d"), flag_fill);

//     SEGMENT.fill(0); // Clear all to off

//     uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness
//     ALOG_INF(PSTR("brightness %d"), brightness);

//     JsonParserArray outer = jtok_pwi;
//     JsonParserToken outerTok = outer;
//     outerTok.nextOne(); // first group

//     uint16_t groups = 0;
//     uint32_t pixels_set = 0;

//     while (outerTok.isValid())
//     {
//       if (!outerTok.isArray())
//       {
//         ALOG_ERR(PSTR("OnPixelsWithIndex entry not an array"));
//         outerTok.nextOne();
//         continue;
//       }

//       JsonParserArray groupArr = outerTok;

//       // First element in group: paletteIndex
//       JsonParserToken groupTok = groupArr;
//       groupTok.nextOne();

//       if (!groupTok.isValid() || !groupTok.isNum())
//       {
//         ALOG_ERR(PSTR("Group missing paletteIndex"));
//         outerTok.nextOne();
//         continue;
//       }

//       const uint8_t palIdx = (uint8_t)groupTok.getInt();

//       // Fetch the colour ONCE per group (exact palette entry)
//       uint32_t colour = SEGMENT.GetPaletteColour_Legacy(
//         palIdx,
//         PALETTE_INDEX__IS_EXACT_COLOUR,
//         PALETTE_WRAP_HARDEDGE,
//         PALETTE_MODE__FORCE_DISCRETE,
//         NO_ENCODED_VALUE
//       );

//       colour = AdjustColourWithBrightness(colour, brightness);

//       // Scan remaining numeric tokens to determine min/max and count
//       int32_t v_min =  2147483647;
//       int32_t v_max = -2147483647;
//       uint16_t v_count = 0;

//       groupTok.nextOne(); // move to first pixel/range value

//       while (groupTok.isValid())
//       {
//         if (groupTok.isNum())
//         {
//           int32_t v = groupTok.getInt();
//           if (v < v_min) v_min = v;
//           if (v > v_max) v_max = v;
//           v_count++;
//         }
//         else
//         {
//           ALOG_ERR(PSTR("Invalid token type in group (expected number)"));
//         }
//         groupTok.nextOne();
//       }

//       if (v_count == 0)
//       {
//         ALOG_ERR(PSTR("Group has no pixel/range values"));
//         outerTok.nextOne();
//         continue;
//       }

//       if (flag_fill)
//       {
//         // Fill min..max
//         for (int32_t p = v_min; p <= v_max; p++)
//         {
//           SEGMENT.setPixelColor((uint16_t)p, colour);
//           pixels_set++;
//         }
//         ALOG_INF(PSTR("Group %d palIdx=%d fill %d..%d"), groups, palIdx, (int)v_min, (int)v_max);
//       }
//       else
//       {
//         if (v_count == 1)
//         {
//           // Single pixel (v_min==v_max)
//           SEGMENT.setPixelColor((uint16_t)v_min, colour);
//           pixels_set++;
//           ALOG_INF(PSTR("Group %d palIdx=%d pix %d"), groups, palIdx, (int)v_min);
//         }
//         else if (v_count == 2)
//         {
//           // Range min..max
//           for (int32_t p = v_min; p <= v_max; p++)
//           {
//             SEGMENT.setPixelColor((uint16_t)p, colour);
//             pixels_set++;
//           }
//           ALOG_INF(PSTR("Group %d palIdx=%d range %d..%d"), groups, palIdx, (int)v_min, (int)v_max);
//         }
//         else
//         {
//           // Explicit list: re-iterate group tokens and set each pixel index
//           JsonParserToken groupTok2 = groupArr;
//           groupTok2.nextOne(); // paletteIndex
//           groupTok2.nextOne(); // first value after paletteIndex

//           while (groupTok2.isValid())
//           {
//             if (groupTok2.isNum())
//             {
//               int32_t p = groupTok2.getInt();
//               SEGMENT.setPixelColor((uint16_t)p, colour);
//               pixels_set++;
//             }
//             groupTok2.nextOne();
//           }

//           ALOG_INF(PSTR("Group %d palIdx=%d explicit count=%d"), groups, palIdx, v_count);
//         }
//       }

//       groups++;
//       outerTok.nextOne();
//     }

//     ALOG_INF(PSTR("OnPixelsWithIndex groups=%d pixels_set=%d"), groups, (int)pixels_set);

//     SEGMENT.cycle_time__rate_ms = FRAMETIME;
//     SET_DIRECT_MODE();
//     show();
//   }
// }



/**
New payload format

OnPixelsWithIndex: array of groups
Each group: [paletteIndex, ...pixelsOrRange...]

OnPixelsFill: true (optional)
- When true, each group is treated as min..max fill
- Avoids listing every pixel manually

Example (copy/paste directly into MQTT explorer):

{
  "MQTTPixel": {
    "OnPixelsWithIndex": [
      [0, 0, 10],
      [1, 10, 20],
      [2, 30, 40],
      [3, 50, 60]
    ],
    "OnPixelsFill": true
  }
}

 {
  "BrightnessRGB": 100,
  "ColourPalette":"Colour and White Stripe",
  "Effects":{"Grouping":1},
   "MQTTPixel": {
    "OnPixelsWithIndex": [
      [0, 0, 65],
      [2, 66, 131],
      [1, 132, 134],
      [4, 135, 199],
      [6, 200, 264],
      [1, 265, 269],
      [8, 270, 335],
      [10, 336, 401],
      [1, 402, 406],
      [12, 407, 450]
    ],
    "OnPixelsFill": false
  }
 }


  {
  "BrightnessRGB": 100,
  "ColourPalette":"RGPBY",
  "Effects":{"Grouping":1},
   "MQTTPixel": {
    "OnPixelsWithIndex": [
      [0, 0, 65],
      [4, 66, 131],
      [1, 132, 134],
      [4, 135, 199],
      [6, 200, 264],
      [4, 265, 269],
      [8, 270, 335],
      [10, 336, 401],
      [4, 402, 406],
      [12, 407, 450]
    ],
    "OnPixelsFill": false
  }
 }

{
  "BrightnessRGB": 100,
  "ColourPalette":"RGPBY",
  "Effects":{"Grouping":1},
   "MQTTPixel": {
    "OnPixelsWithIndex": [
      [0, 0, 65],
      [1, 66, 131],
      [4, 132, 134],
      [0, 135, 199],
      [1, 200, 264],
      [4, 265, 269],
      [0, 270, 335],
      [1, 336, 401],
      [4, 402, 406],
      [0, 407, 471],
      [1, 472, 537],
      [4, 538, 541],
      [0, 542, 607],
      [1, 608, 673],
      [4, 674, 677],
      [0, 678, 743],
      [1, 744, 809],
      [4, 810, 814],
      [0, 815, 880],
      [1, 881, 946],
      [4, 947, 949],
      [0, 950, 1000]
    ],
    "OnPixelsFill": false
  }
 }

If OnPixelsFill is false or missing:
[idx, p]               -> single pixel
[idx, a, b]            -> range (min..max)
[idx, p1, p2, p3...]   -> explicit pixel list
*/

JsonParserToken jtok_pwi = jtok.getObject()["OnPixelsWithIndex"];
if (jtok_pwi && jtok_pwi.isArray())
{
  // Optional fill flag
  bool flag_fill = false;
  JsonParserToken jtok_fill = jtok.getObject()["OnPixelsFill"];
  if (jtok_fill)
  {
    if (jtok_fill.isBool()) flag_fill = jtok_fill.getBool();
    else if (jtok_fill.isNum()) flag_fill = (jtok_fill.getInt() != 0);
  }

  ALOG_COM(PSTR("OnPixelsWithIndex is ARRAY, fill=%d"), flag_fill);

  SEGMENT.fill(0); // clear all pixels

  uint8_t brightness = 10;//SEGMENT.getBrightnessRGB_WithGlobalApplied();
  ALOG_INF(PSTR("brightness %d"), brightness);

  JsonParserArray outer = jtok_pwi;

  uint16_t groups = 0;
  uint32_t pixels_set = 0;

  const uint16_t outer_len = outer.size();
  for (uint16_t gi = 0; gi < outer_len; gi++)
  {
    JsonParserToken groupTok = outer[gi];
    if (!groupTok || !groupTok.isArray())
    {
      ALOG_ERR(PSTR("OnPixelsWithIndex[%d] not an array"), gi);
      continue;
    }

    JsonParserArray group = groupTok;
    const uint16_t glen = group.size();
    if (glen < 2)
    {
      ALOG_ERR(PSTR("Group[%d] too short (need >=2), len=%d"), gi, glen);
      continue;
    }

    JsonParserToken tokPal = group[0];
    if (!tokPal || !tokPal.isNum())
    {
      ALOG_ERR(PSTR("Group[%d] missing paletteIndex"), gi);
      continue;
    }

    const uint8_t palIdx = (uint8_t)tokPal.getInt();

    uint32_t colour = SEGMENT.GetPaletteColour_Legacy(
      palIdx,
      PALETTE_INDEX__IS_EXACT_COLOUR,
      PALETTE_WRAP_HARDEDGE,
      PALETTE_MODE__FORCE_DISCRETE,
      NO_ENCODED_VALUE
    );
    colour = AdjustColourWithBrightness(colour, brightness);

    int32_t v_min =  2147483647;
    int32_t v_max = -2147483647;
    uint16_t v_count = 0;

    for (uint16_t k = 1; k < glen; k++)
    {
      JsonParserToken tv = group[k];
      if (tv && tv.isNum())
      {
        int32_t v = tv.getInt();
        if (v < v_min) v_min = v;
        if (v > v_max) v_max = v;
        v_count++;
      }
      else
      {
        ALOG_ERR(PSTR("Group[%d] invalid value at k=%d (expected number)"), gi, k);
      }
    }

    if (v_count == 0)
    {
      ALOG_ERR(PSTR("Group[%d] has no numeric pixel values"), gi);
      continue;
    }

    if (flag_fill)
    {
      for (int32_t p = v_min; p <= v_max; p++)
      {
        SEGMENT.setPixelColor((uint16_t)p, colour);
        pixels_set++;
      }
      ALOG_INF(PSTR("Group %d palIdx=%d fill %d..%d"), groups, palIdx, (int)v_min, (int)v_max);
    }
    else
    {
      if (v_count == 1)
      {
        SEGMENT.setPixelColor((uint16_t)v_min, colour);
        pixels_set++;
        ALOG_INF(PSTR("Group %d palIdx=%d pix %d"), groups, palIdx, (int)v_min);
      }
      else if (v_count == 2)
      {
        for (int32_t p = v_min; p <= v_max; p++)
        {
          SEGMENT.setPixelColor((uint16_t)p, colour);
          pixels_set++;
        }
        ALOG_INF(PSTR("Group %d palIdx=%d range %d..%d"), groups, palIdx, (int)v_min, (int)v_max);
      }
      else
      {
        uint16_t set_count = 0;
        for (uint16_t k = 1; k < glen; k++)
        {
          JsonParserToken tv = group[k];
          if (tv && tv.isNum())
          {
            int32_t p = tv.getInt();
            SEGMENT.setPixelColor((uint16_t)p, colour);
            pixels_set++;
            set_count++;
          }
        }
        ALOG_INF(PSTR("Group %d palIdx=%d explicit count=%d"), groups, palIdx, set_count);
      }
    }

    groups++;
  }

  ALOG_INF(PSTR("OnPixelsWithIndex groups=%d pixels_set=%d"), groups, (int)pixels_set);

  SEGMENT.cycle_time__rate_ms = FRAMETIME;
  SET_DIRECT_MODE();
  show();
}

}

#endif // ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL

  // #endif // ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL


  #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
  /**
   * @brief Handles MQTT commands for setting pixel arrays with row mappings.
   * 
   * {
    "BrightnessRGB": 255,
    "Effects":{
      "Param0":1605,
      "Param1":1610
    },
    "MQTTPixel": {
      "OnPixels": [
        [1605],   // left side
        [1606,1607,1608,1609,1611], //centres
        [2100] //right side
      ]
    }
  }

  {
    "BrightnessRGB": 255,
    "MQTTPixel": {
      "OnPixels": [
          [0, 31, 63, 98, 134, 173, 214, 257, 302, 352, 400, 451, 502, 553, 604, 658, 710, 763, 815, 867, 920, 971, 
          1022, 1071, 1119, 1168, 1215, 1260, 1303, 1345, 1385, 1423, 1461, 1498, 1535, 1572, 1607, 1640, 1671, 1702, 1732, 1759, 1785, 1820, 1841, 1860, 
          1877, 1892, 1904, 1913, 1922, 1930, 1938, 1945, 1952, 1958, 1964, 1969, 1974, 1978],
          [15, 47, 80, 116, 153, 193, 235, 279, 327, 376, 425, 476, 527, 578, 631, 684, 736, 789, 841, 893, 945, 996, 1046, 1095, 1144, 1191, 1237, 1281, 
          1324, 1365, 1404, 1442, 1480, 1516, 1553, 1589, 1623, 1655, 1686, 1717, 1745, 
          1772, 1802, 1829, 1850, 1868, 1884, 1898, 1917, 1926, 1934, 1941, 1948, 1955, 1961, 1967, 1971],
          [30, 62, 97, 133, 172, 213, 256, 301, 351, 399, 450, 501, 552, 603, 657, 709, 762, 814, 866, 919, 970, 1021, 1070, 1118, 1167, 1214, 1259, 1302, 1344,
          1384, 1422, 1460, 1497, 1534, 1571, 1606, 1639, 1670, 1701, 1731, 1758, 1784, 1819, 1840, 1859, 
          1876, 1891, 1903, 1912, 1921, 1929, 1937, 1944, 1951, 1957, 1963, 1968, 1973, 2099]
      ]
    }
  }
  */
  if (jtok = obj["MQTTPixelArrays"]) {

    JsonParserToken jtok_sub = 0;

    ALOG_INF(PSTR("MQTTPixelArrays"));

    realtimeMode = ANIMATION_MODE__REALTIME_MQTT_SETPIXEL;

    if (jtok.isArray()) {
      ALOG_INF(PSTR("Processing MQTTPixelArrays"));

      SEGMENT.fill(0); // Clear all pixels

      uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness

      JsonParserArray arrayOfArrays = jtok;
      JsonParserToken outerToken = arrayOfArrays;

      outerToken.nextOne(); // Move to the first array in the array of arrays
      ALOG_INF(PSTR("Iterating over outer array of MQTTPixelArrays"));

      uint16_t outerCount = 0; // Track outer array items processed
      uint16_t paletteIndex = 0; // Start from palette index 0

      while (outerToken.isValid()) { // Iterate through the outer array
        ALOG_INF(PSTR("Outer token type: %d"), outerToken.getType());

        // Process only valid arrays
        if (outerToken.isArray()) {
          JsonParserArray innerArray = outerToken;
          JsonParserToken innerToken = innerArray;

          innerToken.nextOne(); // Move to the first element in the inner array

          int startPixel = -1, midPixel = -1, endPixel = -1;
          uint16_t pixelCount = 0;

          ALOG_INF(PSTR("Processing inner array"));

          // Process inner array tokens to get start, middle, and end pixels
          while (innerToken.isValid()) {
            if (innerToken.isNum()) { // Check if the token is a number
              int pixelIndex = innerToken.getInt();
              ALOG_INF(PSTR("Inner token type: %d, value: %d"), innerToken.getType(), pixelIndex);

              if (pixelCount == 0)
                  startPixel = pixelIndex; // Start pixel
              else if (pixelCount == 1)
                  midPixel = pixelIndex; // Mid (center) pixel
              else if (pixelCount == 2)
                  endPixel = pixelIndex; // End pixel

              pixelCount++;
            }

            // Stop processing after three valid elements
            if (pixelCount == 3)
              break;

            innerToken.nextOne(); // Move to the next token in the inner array
          }

          if (pixelCount == 3) { // Ensure all three points are provided
            ALOG_INF(PSTR("Setting row: Start %d, Mid %d, End %d using palette index %d"), startPixel, midPixel, endPixel, paletteIndex);

            // Set pixels for the row
            for (int i = startPixel; i <= endPixel; i++) 
            {
              
              uint32_t colour = 0;

              if (i == midPixel) {
                // Center pixel is white
                colour = RGBW32(255, 255, 255, 255);
              }else
              if (i == endPixel) {
                // Right pixel is white, is easier contrast between start of row and end of row
                colour = RGBW32(255, 255, 255, 255);
              } else {
                // Get color from the palette using the current palette index
                colour = SEGMENT.GetPaletteColour_Legacy(paletteIndex, PALETTE_INDEX__IS_SEGLEN_RANGE, PALETTE_WRAP_HARDEDGE, PALETTE_MODE__FORCE_DISCRETE, NO_ENCODED_VALUE);
              }

              // Apply brightness and set the pixel
              SEGMENT.setPixelColor(i, AdjustColourWithBrightness(colour,brightness));
            }

            paletteIndex++; // Increment palette index for the next row

          } else {
              ALOG_ERR(PSTR("Invalid row format. Expected exactly 3 pixels (start, mid, end). Parsed Count=%d, Start=%d, Mid=%d, End=%d"),
                      pixelCount, startPixel, midPixel, endPixel);
          }

          outerCount++;
        } else {
            ALOG_INF(PSTR("Skipping non-array token in outer array: Type=%d"), outerToken.getType());
        }

        outerToken.nextOne(); // Move to the next token in the outer array
      }

      ALOG_INF(PSTR("Processed %d rows in total"), outerCount);

      SEGMENT.cycle_time__rate_ms = FRAMETIME;
      SET_DIRECT_MODE();

      show();
    } else {
      ALOG_ERR(PSTR("MQTTPixelArrays is not a valid array"));
    }
  }
  #endif // ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL


  #if FIRMWARE_VERSION_MIN(0,126)
  #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  // This is an extension of the custom palette, but is autogenerated based on values based in to offload colour conversion for openhab etc
  // Hue// Sat// ColourHeatMap:[style, cold_point, now_temp, hot_point]
  if(jtok = obj["ColourHeatMap_Palette"])
  {

    uint8_t style_index  = 0;
    if(jtok_sub = jtok.getObject()["Method"]) //different heatmap conversion method
    {
      style_index = jtok_sub.getInt();
      ALOG_COM(PSTR(D_COMMAND_NVALUE_K("Method")), style_index);
    }

    if(jtok_sub = jtok.getObject()["Value"])
    {
      if(jtok_sub.isArray())
      {
        ALOG_COM(PSTR("is ARRAY"));

        float array_val[4] = {0}; uint8_t array_ii = 0;
        JsonParserArray array = jtok_sub;
        for(auto v : array)
        {
          ALOG_COM(PSTR("v=%d"), (int)v.getFloat());
          array_val[array_ii++] = v.getFloat();
        }
        CommandSet_ColourHeatMap_Palette(array_val, array_ii, style_index); // Dont pass palette, then assumes active        
      }

    }
  }
  #endif // ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  #endif // FIRMWARE_VERSION_MIN
  

  if(jtok_sub = obj[PM_OVERRIDE])
  {  

    if(jtok = jtok_sub.getObject()["Animation"].getObject()["TimeMs"])
    {
      SEGMENT_I(segment_index).single_animation_override.time_ms = jtok.getInt();
      data_buffer.isserviced++;
    }

    if(jtok = jtok_sub.getObject()["Animation_Off"].getObject()["TimeMs"])
    {
      SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL D_COMMAND_SVALUE_NVALUE_K("Animation_Off","TimeMs")), SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms);
      data_buffer.isserviced++;
    }

  }


  if(jtok = obj[PM_BRIGHTNESS_RGB]){ // Range 0-100
    uint8_t brightness = map(jtok.getInt(), 0,100, 0,255);
    #ifdef ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_BRIGHTNESS_BLOCKED
    ALOG_WRN(PSTR("Brightness RGB command limited due to preset testing mode"));
    brightness = brightness > 10 ? 10 : brightness;
    #endif
    SEGMENT_I(segment_index).setBrightnessRGB( brightness );
    ALOG_INF(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_RGB_255]){
    SEGMENT_I(segment_index).setBrightnessRGB( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_BRIGHTNESS_CCT]){ // Range 0-100
    SEGMENT_I(segment_index).setBrightnessCCT( map(jtok.getInt(), 0,100, 0,255) );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_CCT_255]){
    SEGMENT_I(segment_index).setBrightnessCCT( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }


  if(jtok = obj["Preset"].getObject()["Load"]){
    uint8_t ps = jtok.getInt();
    // b) preset ID only or preset that does not change state (use embedded cycling limits if they exist in getVal())
    #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
    unloadPlaylist();          // applying a preset unloads the playlist, to stop any already running playlist
    #endif
    applyPreset(ps, CALL_MODE_DIRECT_CHANGE); // async load from file system (only preset ID was specified)
  }


  // Per segment time off
  if(jtok = obj["TimeOnSecs"]){
    SEGMENT_I(segment_index).auto_timeoff.Start(jtok.getInt());
    ALOG_INF(PSTR("auto_timeoff %d"), SEGMENT_I(segment_index).auto_timeoff.Value());
  }


  /**
   * @brief Segment Colours (ie 1-5)
   **/
  for(uint8_t colour_index=0;colour_index<5;colour_index++)
  {
    JsonParserObject seg_obj = 0;

    snprintf(buffer, sizeof(buffer), "SegColour%d", colour_index);

    // ALOG_HGL(PSTR("SEGMENT_I(segment_index).segcol[colour_index] length %d"), NUMBER_SEGMENT_COLOURS);    

    if(seg_obj = obj[buffer].getObject())
    {
      
      ALOG_COM(PSTR(D_LOG_PIXEL "SegColour[%d] = %s"), colour_index, buffer);

      if(jtok = seg_obj["RGBWC"])
      {
                
        if(jtok.isArray()){
          uint8_t array[5];
          uint8_t arrlen = 0;
          JsonParserArray arrobj = jtok;
          for(auto v : arrobj){
            if(arrlen > 5){ break; }
            array[arrlen++] = v.getInt();
          }
          SEGMENT_I(segment_index).segcol[colour_index].colour = RgbwwColor(array[0],array[1],array[2],array[3],array[4]);
        }

        data_buffer.isserviced++;
        // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K("Raw")), SEGMENT_I(segment_index).segcol[colour_index].getHue360());
      }
      if(jtok = seg_obj["RGB"])
      {
                
        if(jtok.isArray()){
          uint8_t array[3];
          uint8_t arrlen = 0;
          JsonParserArray arrobj = jtok;
          for(auto v : arrobj){
            if(arrlen > 3){ break; }
            array[arrlen++] = v.getInt();
          }
          RgbwwColor current = SEGMENT_I(segment_index).segcol[colour_index].colour; // Keep current WW/CW
          SEGMENT_I(segment_index).segcol[colour_index].colour = RgbwwColor(array[0],array[1],array[2],current.CW, current.WW);
        }

        data_buffer.isserviced++;
        // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K("Raw")), SEGMENT_I(segment_index).segcol[colour_index].getHue360());
      }

      #if FIRMWARE_VERSION_MAX(0, 230)
      if(jtok = seg_obj["Manual"]){ // Needs renamed, setting colour RGBCW directly
        ALOG_ERR(PSTR("Use RGBCW command"));
      }
      #endif

    
      if(jtok = seg_obj[PM_HUE]){ // Range 0-359
        CommandSet_SegColour_RgbcctColour_Hue_360(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(segment_index).segcol[colour_index].getHue());
      }

      
      if(jtok = seg_obj[PM_SAT]){ // Range 0-100
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_Sat_255( (uint8_t)value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_SAT)), SEGMENT_I(segment_index).segcol[colour_index].getSaturation());
        data_buffer.isserviced++; 
      }else
      if(jtok = seg_obj[PM_SAT_255]){ // Full Range 0-255
        CommandSet_SegColour_RgbcctColour_Sat_255(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_SAT255)), SEGMENT_I(segment_index).segcol[colour_index].getSaturation());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_CCT_PERCENTAGE]){ // Assume range 0-100    
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, CCT_MIN_DEFAULT,CCT_MAX_DEFAULT); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin( value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_CCT_PERCENTAGE)), SEGMENT_I(segment_index).segcol[colour_index].getCCT());
        data_buffer.isserviced++;
      }else
      if(jtok = seg_obj[PM_CCT_TEMP]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_CCT_PERCENTAGE)), SEGMENT_I(segment_index).segcol[colour_index].getCCT());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_BRIGHTNESS_RGB]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessRGB());
      }else
      if(jtok = seg_obj[PM_BRIGHTNESS_RGB_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB_255)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessRGB());
      }

    
      if(jtok = seg_obj[PM_BRIGHTNESS_CCT]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessCCT());
      }else
      if(jtok = seg_obj[PM_BRIGHTNESS_CCT_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT_255)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessCCT());
      }
     
    }
  
  }
 

  /***********************************************************************************************************************************************************************************
   *********************************************************************************************************************************************************************************** 
  ******* NOTE: Debug commands only, not part of official commands ****************************************************************************************************************** 
  ************************************************* *********************************************************************************************************************************
  ***********************************************************************************************************************************************************************************/

  if(jtok = obj["Debug"])
  {

    JsonParserObject jObj_debug = jtok.getObject();

    #ifdef ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    if(jtok = obj["Bus"].getObject()["Init"])
    { 
      doInitBusses = true;
      TestCode_AddBus1();   
    }
    if(jtok = obj["Bus"].getObject()["Bus16Test"])
    { 
      doInitBusses = true;
      TestCode_Add16ParallelBus1();
    }

    #endif // ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    
      
    if(jtok = jObj_debug["addSegment"]){
      uint8_t id = jtok.getInt();
      uint16_t start = id;
      uint16_t stop = 100;
      ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());
      Segment_AppendNew(start, stop, jtok.getInt());
    }


    if(jtok = jObj_debug["removeSegment"]){  
      
      uint8_t start = 0;
      uint8_t stop = 100;
      uint8_t id = jtok.getInt();
      uint8_t force = 0;

      segments[jtok.getInt()].stop = 0; // to disable segment

      ALOG_INF(PSTR("getSegmentsNum() %d|%d %d"), id, getSegmentsNum(), segments[jtok.getInt()].stop);

      // remove all inactive segments (from the back)
      if(id==255){ purgeSegments(true); }
      else{        purgeSegments(false); }

    }

    if(jtok = jObj_debug["removeSegment2"]){  

      uint8_t start = 0;
      uint8_t stop = 100;
      uint8_t id = jtok.getInt();
      uint8_t force = 0;

      Serial.println();

      segments[jtok.getInt()].stop = 0; // to disable segment

      ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

      // remove all inactive segments (from the back)
      int deleted = 0;
      if (segments.size() <= 1) return;
      for (size_t i = segments.size()-1; i > 0; i--)
        if (segments[i].stop == 0 || force) {
          DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
          deleted++;
          segments.erase(segments.begin() + i);
        }else{
          
        }
      if (deleted) {
        
        segments.shrink_to_fit();
        if (_mainSegment >= segments.size()){
          
          setMainSegmentId(0);
        }
      }

    }

    if(jtok = jObj_debug["PlaylistsPSFromPSN"])
    { 
      ALOG_INF(PSTR("Debug: forcing ScanPresetsFile_GeneratePlaylistIDsFromPSN()"));
      ScanPresetsFile_GeneratePlaylistIDsFromPSN_2();
    }


  } // end Debug options

  #ifdef ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
  
  if (jtok = obj["Standby"]) {
    ALOG_INF(PSTR("Standby command FOUND"));
    JsonParserObject jDbg = jtok.getObject();
    JsonParserToken  jtok2 = 0;

    uint16_t fadeMs = 0;
    if (jtok2 = jDbg["FadeMs"]) {
      fadeMs = (uint16_t)jtok2.getInt();
      standby.fade_override_ms = fadeMs;   // store override for this wake cycle
      ALOG_INF(PSTR("Standby: FadeMs=%u"), (unsigned)fadeMs);
    }

    if (jtok2 = jDbg["Start"]) {
      uint8_t callMode = CALL_MODE_NO_NOTIFY;
      if (jtok2.getInt()) {
        Standby_Start(/*fadeMs=*/0, callMode);
        // If you want: cancel remain-awake because we explicitly entered standby
        Standby_CancelRemainAwake();
      }
    }

    if (jtok2 = jDbg["Stop"]) {
      uint8_t callMode = CALL_MODE_NO_NOTIFY;
      if (jtok2.getInt()) {
        // Use fade override if provided in this command
        uint16_t f = standby.fade_override_ms ? standby.fade_override_ms : 0;
        Standby_Stop(f, callMode);
        // Do NOT start any timer here; Stop is a pure "leave standby"
      }
    }

    // Wake semantics:
    //   0  → cancel any pending remain-awake (no state change)
    //   1  → if in standby, leave now (with optional FadeMs), no auto return
    //   >1 → leave standby if active (with FadeMs), then stay awake for N seconds, auto-enter standby
    if (jtok2 = jDbg["Wake"]) {
      uint32_t secs = jtok2.getInt();

      if (secs == 0) {
        ALOG_INF(PSTR("Standby: Wake<=0 → cancel remain-awake"));
        Standby_CancelRemainAwake();
        // no state change
      } else if (secs == 1) {
        if (standby.active) {
          uint16_t f = standby.fade_override_ms ? standby.fade_override_ms : 0;
          ALOG_INF(PSTR("Standby: Wake==1 → leave standby now (fade=%u)"), (unsigned)f);
          Standby_Stop(f, CALL_MODE_NO_NOTIFY);
        } else {
          ALOG_INF(PSTR("Standby: Wake==1 while already awake → no-op"));
        }
        Standby_CancelRemainAwake(); // no auto return
      } else { // secs > 1
        ALOG_INF(PSTR("Wake s>1 "));
        uint16_t f = standby.fade_override_ms ? standby.fade_override_ms : 0;
        if (standby.active) {
          ALOG_INF(PSTR("Standby: Wake %ld s → leave standby now (fade=%u)"), (long)secs, (unsigned)f);
          Standby_Stop(f, CALL_MODE_NO_NOTIFY);
        } else {
          ALOG_INF(PSTR("Standby: Wake %ld s while already awake"), (long)secs);
        }
        //Serial.println("here3");Serial.flush();
        Standby_SetRemainAwake(secs);
        ALOG_INF(PSTR("Standby: remain-awake started: %u s"), (unsigned)secs);
      }

      SEGMENT_I(0).single_animation_override.time_ms = 2000;

    }

    if (jtok2 = jDbg["Init"]) {
      if (jtok2.getInt()) Standby_Init();
    }

    if (jDbg["SetProfile"]) {
      const char* s = nullptr;
      auto t = jDbg["SetProfile"]; JsonParserObject o = t.getObject();
      if (o && o["json"]) s = o["json"].getStr();
      if (s) {
        ALOG_INF(PSTR("Debug:SetProfile len=%u"), (unsigned)strlen(s));
        Standby_SetProfileFromJson(s);
      }
    }

    if (jDbg["StandbySaveCurrentAsProfile"]) {
      DynamicJsonDocument d(12*1024);
      JsonObject root = d.to<JsonObject>();
      serializeState(root, /*forPreset=*/true, /*includeBri=*/true, /*segmentBounds=*/true, /*selectedOnly=*/false);
      String tmp; serializeJson(d, tmp);
      Standby_SetProfileFromJson(tmp.c_str());
      ALOG_INF(PSTR("Debug:StandbySaveCurrentAsProfile saved (%u bytes)"), (unsigned)tmp.length());
    }

    Serial.println("ABOUT TO WAIT FOR STABILITY");
    //delay(4000);
  }

  if (jtok = obj["Debug"])
  {

    ALOG_INF(PSTR("Debug command FOUND"));

    JsonParserObject jDbg = jtok.getObject();
    JsonParserToken jtok2 = 0; 
  // ---------------- Save/Load full STATE snapshot (debug only) ----------------
    if (jtok2 = jDbg["SaveState"]) {
      bool includeBounds = true;
      bool includeBri    = true;
      bool selectedOnly  = false;
      bool fullGlobals   = false;

    //   auto t = jDbg["SaveState"]; JsonParserObject o = t.getObject();
    //   if (o) {
    //     if (o["includeBounds"]) includeBounds = (bool)o["includeBounds"].getInt();
    //     if (o["includeBri"])    includeBri    = (bool)o["includeBri"].getInt();
    //     if (o["selectedOnly"])  selectedOnly  = (bool)o["selectedOnly"].getInt();
    //     if (o["fullGlobals"])   fullGlobals   = (bool)o["fullGlobals"].getInt();
    //   }
    //   ALOG_DBG(PSTR("Debug:SaveState bounds=%d bri=%d sel=%d full=%d"),
    //            includeBounds, includeBri, selectedOnly, fullGlobals);
      if(jtok2.getInt())
      FileSave__State(includeBounds, includeBri, selectedOnly, fullGlobals);
    }

    if (jtok2 = jDbg["LoadState"]) {
      uint8_t callMode = CALL_MODE_NO_NOTIFY;
    //   auto t = jDbg["LoadState"]; JsonParserObject o = t.getObject();
    //   if (o && o["callMode"]) callMode = (uint8_t)o["callMode"].getInt();
    //   ALOG_DBG(PSTR("Debug:LoadState callMode=%u"), callMode);
      if(jtok2.getInt())
      FileLoad__State(callMode);
    }

  }

  #endif

  /**
   * @brief 
   * # Issue : Caused effects to reset when non lighting commands happened
   * Fix is to only reset effects if lighting commands are updated
   */
  if(isserviced_start_count != data_buffer.isserviced)
  {
    ALOG_INF(PSTR("isserviced_start_count %d data_buffer.isserviced %d"), isserviced_start_count, data_buffer.isserviced);
    // If segment commands updated, some effects may need reset
    SEGMENT_I(segment_index).call = 0; 
  }

  //  If command source was webui, then override changes
  if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  {
    // segment_animation_override.time_ms = 100;
  }

  ALOG_HGLB(PSTR("REACHED END OF SEGMENT PARSING %d"), data_buffer.isserviced);
  
} // END PARSE COMMANDS


        



#ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
/**
 * @brief Method for setting colour of rgbcct palette using a value within a range. Use case: colour map ambient temperature into colour hue
 * 
 * @param array_val   : [min_point, current, max_point] . alternate style may use multiple "inflection" points (ie not linear with multi slope conversions)
 * @param array_length 
 * @param style_index : default 0   : 10 to 20 celsius is cyan 180 to red 0
 * @param palette_id  : default 255 : use the active palette id
 */
void mAnimatorLight::CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index, uint8_t palette_id)
{

  for(uint8_t ii=0;ii<array_length;ii++){ Serial.printf("%d=%f\n\r", ii, array_val[ii]); }

  switch(style_index)
  {
    /**
     * @brief 
     * In this case, 10 to 20 Celsius will be from hue 180 to hue 0 (cyan to red)
       Then above or below those ranges, will remain fixed at the red/cyan
     */
    default:
    case 0:
    {

      float hue_f = mSupport::mapfloat(array_val[1], array_val[0], array_val[2], 180.0f, 0.0f);
        hue_f = constrain(hue_f, 0.0f, 180.0f);
      uint16_t hue_i = (uint16_t)hue_f;

      // CommandSet_ActiveSolidPalette_Hue_360(hue_i);
      // CommandSet_ActiveSolidPalette_Sat_255(255);

    }
    break;
    /**
     * @brief 
     * In this case, 10 to 20 Celsius will be from hue 180 to hue 0 (cyan to red)
       Then above or below those ranges, will go slightly pink and dark blue as solid colours
     */
    case 1:
    
      float hue_f = mSupport::mapfloat(array_val[1], array_val[0], array_val[2], 180.0f, 0.0f);

      Serial.println(hue_f);

      if(array_val[1] < array_val[0]) // force dark blue
      {
        ALOG_INF(PSTR("Force Dark Blue"));
        hue_f = 240.0f;
      }else
      if(array_val[1] > array_val[2]) // force slightly hot pink
      {
        ALOG_INF(PSTR("Force Hot Pink"));
        hue_f = 350.0f;
      }
      else // Just constrain within range
      {
        ALOG_INF(PSTR("Constrain"));
        hue_f = constrain(hue_f, 0.0f, 180.0f);
      }

      uint16_t hue_i = (uint16_t)hue_f;

      ALOG_COM(PSTR("Hue = %d"), hue_i);

      // CommandSet_ActiveSolidPalette_Hue_360(hue_i);
      // CommandSet_ActiveSolidPalette_Sat_255(255);


    break;
  }

}
#endif // ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE

#ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
void mAnimatorLight::CommandSet_AnimationModeID(uint8_t value){

  uint8_t segment_index = 0;

  char buffer[60];

  SEGMENT_I(0).animation_mode_id = value;  // this is wrong
          
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
  #endif

}
const char* mAnimatorLight::GetAnimationModeName(char* buffer, uint16_t buflen){
  return GetAnimationModeNameByID(  SEGMENT_I(0).animation_mode_id, buffer, buflen);
}
const char* mAnimatorLight::GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen){
  switch(id){
    default:
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    case ANIMATION_MODE_REALTIME_ADALIGHT:     memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;
    #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
      case ANIMATION_MODE_NOTIFICATIONS_ID: memcpy_P(buffer, PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR,sizeof(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)); break;
    #endif
    case ANIMATION_MODE__EFFECTS:       memcpy_P(buffer, PM_ANIMATION_MODE_EFFECTS_NAME_CTR, sizeof(PM_ANIMATION_MODE_EFFECTS_NAME_CTR)); break;
    #ifdef ENABLE_PIXEL_FUNCTION_WLED_EFFECTS
    case ANIMATION_MODE_WLED_ID:       memcpy_P(buffer, PM_ANIMATION_MODE_WLED_NAME_CTR, sizeof(PM_ANIMATION_MODE_WLED_NAME_CTR)); break;
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    case ANIMATION_MODE_MANUAL_SETPIXEL_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR , sizeof(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR)); break;
    #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  }
  return buffer;
} 
int8_t mAnimatorLight::GetAnimationModeIDbyName(const char* c){

  if(*c=='\0'){
    return -1;
  }
  #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
  if(strcmp_P(c,PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)==0){  return ANIMATION_MODE_NOTIFICATIONS_ID; }
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  if(strstr_P(c,PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)){      return ANIMATION_MODE_REALTIME_ADALIGHT; }
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  if(strcmp_P(c,PM_ANIMATION_MODE_EFFECTS_NAME_CTR)==0){        return ANIMATION_MODE__EFFECTS; }
  // #ifdef ENABLE_PIXEL_FUNCTION_WLED_EFFECTS
  // if(strcmp_P(c,PM_ANIMATION_MODE_EFFECTS_NAME_CTR)==0){        return ANIMATION_MODE_WLED_ID; }
  // #endif
  return -1;
}
#endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE


void mAnimatorLight::CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index)
{
  SEGMENT_I(segment_index).intensity = value;
}


void mAnimatorLight::CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index)
{
  SEGMENT_I(segment_index).speed = value;
}


void mAnimatorLight::CommandSet_Effect_ColourTypeID(uint8_t id, uint8_t segment_index)
{  
  SEGMENT_I(segment_index).colour_width__used_in_effect_generate = (ColourType)id;
  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_width__used_in_effect_generate);
}


void mAnimatorLight::CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length)
{
  mPaletteI->addCustomPalette(index, data, data_length, encoding);
}


void mAnimatorLight::CommandSet_PaletteID(uint16_t value, uint8_t segment_index)
{
  char buffer[50];
  SEGMENT_I(segment_index).palette_id = value < mPaletteI->GetPaletteListLength() ? value : 0;  
  segment_current_index = segment_index;
  SEGMENT_I(segment_index).live_palette.timing1 = 0; // reset timing to force immediate load
  SEGMENT.LoadPalette(segments[segment_index].palette_id);
}

void mAnimatorLight::CommandSet_Palette2ID(uint16_t value, uint8_t segment_index)
{
  SEGMENT_I(segment_index).live_palette.timing1 = 0; // reset timing to force immediate load
  char buffer[50];
  SEGMENT_I(segment_index).palette2_id = value < mPaletteI->GetPaletteListLength() ? value : 0;  
  // No Load
}


void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index)
{    
  if(!value) value = MIN_SHOW_DELAY; // default to MIN_SHOW_DELAY when zero
  SEGMENT_I(segment_index).cycle_time__rate_ms = value;
  ALOG_DBM( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_NVALUE_K(D_TRANSITION, D_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms );  
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= NUMBER_SEGMENT_COLOURS){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setHue(hue_new);      
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= NUMBER_SEGMENT_COLOURS){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setSaturation(sat_new);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= NUMBER_SEGMENT_COLOURS){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setCCT_Kelvin(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= NUMBER_SEGMENT_COLOURS){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setBrightnessRGB(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= NUMBER_SEGMENT_COLOURS){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setBrightnessCCT(brightness);    
}



#endif //USE_MODULE_LIGHTS_ANIMATOR




