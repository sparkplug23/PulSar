#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

/**
 * Creates map for 2D Matrix
 * 
 * Process:
 * - Checks if ledmap.json exists, if so, continues with normal deserialization
 * - If not, creates a default mapping for the matrix based on width and height, by calling setUpMatrix() then exits
 * 
 * @param n 
 * @return true 
 * @return false 
 */
// bool mAnimatorLight::deserializeMap(uint8_t n) {
//   // 2D support creates its own ledmap (on the fly) if a ledmap.json exists it will overwrite built one.

//   char fileName[32];
//   strcpy_P(fileName, PSTR("/ledmap"));
//   if (n) sprintf(fileName +7, "%d", n);
//   strcat_P(fileName, PSTR(".json"));
//   bool isFile = FILE_SYSTEM.exists(fileName);

//   customMappingSize = 0; // prevent use of mapping if anything goes wrong
//   currentLedmap = 0;
//   if (n == 0 || isFile) interfaceUpdateCallMode = CALL_MODE_WS_SEND; // schedule WS update (to inform UI)

//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   if (!isFile && n==0 && isMatrix) {
//     setUpMatrix();
//     if (getLengthTotal() != lengthTotalBefore)
//       updatePixelBuffer(); // allocate _pixels[] to match new length
//     return false;
//   }
//   #endif

//   if (!isFile || !JBI->requestJSONBufferLock(7)) return false;

//   if (!tkr_mfile->readObjectFromFile(fileName, nullptr, tkr_mfile->pDoc)) {
//     DEBUG_PRINT(F("ERROR Invalid ledmap in ")); DEBUG_PRINTLN(fileName);
//     JBI->releaseJSONBufferLock();
//     return false; // if file does not load properly then exit
//   }

//   JsonObject root = tkr_mfile->pDoc->as<JsonObject>();
//   // if we are loading default ledmap (at boot) set matrix width and height from the ledmap (compatible with WLED MM ledmaps)
//   if (isMatrix && n == 0 && (!root[F("width")].isNull() || !root[F("height")].isNull())) {
//     Segment::maxWidth  = min(max(root[F("width")].as<int>(), 1), 128);
//     Segment::maxHeight = min(max(root[F("height")].as<int>(), 1), 128);
//   }

//   if (customMappingTable) delete[] customMappingTable;
//   customMappingTable = new uint16_t[getLengthTotal()];

//   if (customMappingTable) {
//     DEBUG_PRINT(F("Reading LED map from ")); DEBUG_PRINTLN(fileName);
//     JsonArray map = root[F("map")];
//     if (!map.isNull() && map.size()) {  // not an empty map
//       customMappingSize = min((unsigned)map.size(), (unsigned)getLengthTotal());
//       for (unsigned i=0; i<customMappingSize; i++) customMappingTable[i] = (uint16_t) (map[i]<0 ? 0xFFFFU : map[i]);
//       currentLedmap = n;
//     }
//   } else {
//     DEBUG_PRINTLN(F("ERROR LED map allocation error."));
//   }

//   JBI->releaseJSONBufferLock();
//   if (strip.getLengthTotal() != lengthTotalBefore)
//     strip.updatePixelBuffer(); // allocate _pixels[] to match new length
//   return (customMappingSize > 0);
// }


bool mAnimatorLight::deserializeMap(unsigned n) {
  char fileName[32];
  strcpy_P(fileName, PSTR("/ledmap"));
  if (n) sprintf(fileName +7, "%d", n);
  strcat_P(fileName, PSTR(".json"));
  bool isFile = FILE_SYSTEM.exists(fileName);

  customMappingSize = 0; // prevent use of mapping if anything goes wrong
  currentLedmap = 0;
  if (n == 0 || isFile) interfaceUpdateCallMode = CALL_MODE_WS_SEND; // schedule WS update (to inform UI)
  uint32_t lengthTotalBefore = getLengthTotal();

  if (!isFile && n==0 && isMatrix) {
    // 2D panel support creates its own ledmap (on the fly) if a ledmap.json does not exist
    setUpMatrix();
    if (getLengthTotal() != lengthTotalBefore)
      updatePixelBuffer(); // allocate _pixels[] to match new length
    return false;
  }

  if (!isFile || !JBI->requestJSONBufferLock(JSON_LOCK_LEDMAP)) return false;

  StaticJsonDocument<64> filter;
  filter[F("width")]  = true;
  filter[F("height")] = true;
  if (!tkr_mfile->readObjectFromFile(fileName, nullptr, tkr_mfile->pDoc, &filter)) {
    DEBUG_PRINTF_P(PSTR("ERROR Invalid ledmap in %s\n"), fileName);
    JBI->releaseJSONBufferLock();
    return false; // if file does not load properly then exit
  } else
    DEBUG_PRINTF_P(PSTR("Reading LED map from %s\n"), fileName);

  JsonObject root = tkr_mfile->pDoc->as<JsonObject>();
  // if we are loading default ledmap (at boot) set matrix width and height from the ledmap (compatible with WLED MM ledmaps)
  if (n == 0 && (!root[F("width")].isNull() || !root[F("height")].isNull())) {
    Segment::maxWidth  = min(max(root[F("width")].as<int>(), 1), 255);
    Segment::maxHeight = min(max(root[F("height")].as<int>(), 1), 255);
    isMatrix = true;
    DEBUG_PRINTF_P(PSTR("LED map width=%d, height=%d\n"), Segment::maxWidth, Segment::maxHeight);
  }

  d_free(customMappingTable);
  customMappingTable = static_cast<uint16_t*>(d_malloc(sizeof(uint16_t)*getLengthTotal())); // prefer DRAM for speed

  if (customMappingTable) {
    DEBUG_PRINTF_P(PSTR("ledmap allocated: %uB\n"), sizeof(uint16_t)*getLengthTotal());
    File f = FILE_SYSTEM.open(fileName, "r");
    f.find("\"map\":[");
    while (f.available()) { // f.position() < f.size() - 1
      char number[32];
      size_t numRead = f.readBytesUntil(',', number, sizeof(number)-1); // read a single number (may include array terminating "]" but not number separator ',')
      number[numRead] = 0;
      if (numRead > 0) {
        char *end = strchr(number,']'); // we encountered end of array so stop processing if no digit found
        bool foundDigit = (end == nullptr);
        int i = 0;
        if (end != nullptr) do {
          if (number[i] >= '0' && number[i] <= '9') foundDigit = true;
          if (foundDigit || &number[i++] == end) break;
        } while (i < 32);
        if (!foundDigit) break;
        int index = atoi(number);
        if (index < 0 || index > 65535) index = 0xFFFF; // prevent integer wrap around
        customMappingTable[customMappingSize++] = index;
        if (end != nullptr) break; // array closing ']' was in this chunk; stop before atoi() coerces trailing JSON keys into bogus entries
        if (customMappingSize >= getLengthTotal()) break;
      } else break; // there was nothing to read, stop
    }
    currentLedmap = n;
    f.close();

    #ifdef WLED_DEBUG
    DEBUG_PRINT(F("Loaded ledmap:"));
    for (unsigned i=0; i<customMappingSize; i++) {
      if (!(i%Segment::maxWidth)) DEBUG_PRINTLN();
      DEBUG_PRINTF_P(PSTR("%4d,"), customMappingTable[i] < 0xFFFFU ? customMappingTable[i] : -1);
    }
    DEBUG_PRINTLN();
    #endif
/*
    JsonArray map = root[F("map")];
    if (!map.isNull() && map.size()) {  // not an empty map
      customMappingSize = min((unsigned)map.size(), (unsigned)getLengthTotal());
      for (unsigned i=0; i<customMappingSize; i++) customMappingTable[i] = (uint16_t) (map[i]<0 ? 0xFFFFU : map[i]);
      currentLedmap = n;
    }
*/
  } else {
    DEBUG_PRINTLN(F("ERROR LED map allocation error."));
  }

  JBI->releaseJSONBufferLock();
  if (getLengthTotal() != lengthTotalBefore)
    updatePixelBuffer(); // allocate _pixels[] to match new length
  return (customMappingSize > 0);
}


// setUpMatrix() - constructs ledmap array from matrix of panels with WxH pixels
// this converts physical (possibly irregular) LED arrangement into well defined
// array of logical pixels: fist entry corresponds to left-topmost logical pixel
// followed by horizontal pixels, when mAnimatorLight::Segment::maxWidth logical pixels are added they
// are followed by next row (down) of mAnimatorLight::Segment::maxWidth pixels (and so forth)
// note: matrix may be comprised of multiple panels each with different orientation
// but ledmap takes care of that. ledmap is constructed upon initialization
// so matrix should disable regular ledmap processing
void mAnimatorLight::setUpMatrix() {

  ALOG_INF(PSTR("setUpMatrix"));

  ALOG_INF("?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????setUpMatrix()");

  // erase old ledmap, just in case.
  if (customMappingTable != nullptr) delete[] customMappingTable;
  customMappingTable = nullptr;
  customMappingSize = 0;

  // isMatrix is set in cfg.cpp or set.cpp
  if (isMatrix) {

    // calculate width dynamically because it will have gaps
    Segment::maxWidth = 1;
    Segment::maxHeight = 1;
    // temporary fix, make sure the start/stop of the segment are right
    ALOG_WRN(PSTR("Fixing segment range to match matrix, this needs resolving in the future to allow multiple matrix elements"));



    ALOG_INF("panel size %d", panel.size());
    if(panel.size()==0)
    {
      ALOG_INF("panel size 0");
    }

    for (size_t i = 0; i < panel.size(); i++) {
      Panel &p = panel[i];
      if (p.xOffset + p.width > Segment::maxWidth) {
        Segment::maxWidth = p.xOffset + p.width;
      }
      if (p.yOffset + p.height > Segment::maxHeight) {
        Segment::maxHeight = p.yOffset + p.height;
      }
    }

    ALOG_INF(PSTR("maxWidth  %d\n\r"), Segment::maxWidth);
    ALOG_INF(PSTR("maxHeight %d\n\r"), Segment::maxHeight);

    // safety check
    if (Segment::maxWidth * Segment::maxHeight > MAX_LEDS_NEO || Segment::maxWidth <= 1 || Segment::maxHeight <= 1) {
      DEBUG_PRINTLN(F("2D Bounds error."));
      isMatrix = false;
      Segment::maxWidth = _length;
      Segment::maxHeight = 1;
      panels = 0;
      panel.clear(); // release memory allocated by panels
      ALOG_INF(PSTR("panel.clear() HERE B?????????????????????????????????????????????????????"));
      resetSegments();
      return;
    }

    customMappingTable = new uint16_t[Segment::maxWidth * Segment::maxHeight];

    if (customMappingTable != nullptr) 
    {
      customMappingSize = Segment::maxWidth * Segment::maxHeight;

      // fill with empty in case we don't fill the entire matrix
      for (size_t i = 0; i< customMappingSize; i++) {
        customMappingTable[i] = (uint16_t)-1;
      }

      // we will try to load a "gap" array (a JSON file)
      // the array has to have the same amount of values as mapping array (or larger)
      // "gap" array is used while building ledmap (mapping array)
      // and discarded afterwards as it has no meaning after the process
      // content of the file is just raw JSON array in the form of [val1,val2,val3,...]
      // there are no other "key":"value" pairs in it
      // allowed values are: -1 (missing pixel/no LED attached), 0 (inactive/unused pixel), 1 (active/used pixel)
      char    fileName[32]; strcpy_P(fileName, PSTR("/2d-gaps.json")); // reduce flash footprint
      bool    isFile = FILE_SYSTEM.exists(fileName);
      size_t  gapSize = 0;
      int8_t *gapTable = nullptr;

      if (isFile && JBI->requestJSONBufferLock(20)) {
        DEBUG_PRINT(F("Reading LED gap from "));
        DEBUG_PRINTLN(fileName);
        // read the array into global JSON buffer
        if (tkr_mfile->readObjectFromFile(fileName, nullptr, tkr_mfile->pDoc)) {
          // the array is similar to ledmap, except it has only 3 values:
          // -1 ... missing pixel (do not increase pixel count)
          //  0 ... inactive pixel (it does count, but should be mapped out (-1))
          //  1 ... active pixel (it will count and will be mapped)
          JsonArray map = tkr_mfile->pDoc->as<JsonArray>();
          gapSize = map.size();
          if (!map.isNull() && gapSize >= customMappingSize) { // not an empty map
            gapTable = new int8_t[gapSize];
            if (gapTable) for (size_t i = 0; i < gapSize; i++) {
              gapTable[i] = constrain(map[i], -1, 1);
            }
          }
        }
        DEBUG_PRINTLN(F("Gaps loaded."));
        JBI->releaseJSONBufferLock();
      }

      DEBUG_LINE_HERE;

      uint16_t x, y, pix=0; //pixel
      for (size_t pan = 0; pan < panel.size(); pan++) {
        Panel &p = panel[pan];
        uint16_t h = p.vertical ? p.height : p.width;
        uint16_t v = p.vertical ? p.width  : p.height;
        for (size_t j = 0; j < v; j++){
          for(size_t i = 0; i < h; i++) {
            y = (p.vertical?p.rightStart:p.bottomStart) ? v-j-1 : j;
            x = (p.vertical?p.bottomStart:p.rightStart) ? h-i-1 : i;
            x = p.serpentine && j%2 ? h-x-1 : x;
            size_t index = (p.yOffset + (p.vertical?x:y)) * mAnimatorLight::Segment::maxWidth + p.xOffset + (p.vertical?y:x);
            if (!gapTable || (gapTable && gapTable[index] >  0)) customMappingTable[index] = pix; // a useful pixel (otherwise -1 is retained)
            if (!gapTable || (gapTable && gapTable[index] >= 0)) pix++; // not a missing pixel
          }
        }
      }

      DEBUG_LINE_HERE;

      // delete gap array as we no longer need it
      if (gapTable) delete[] gapTable;

      DEBUG_PRINT(F("Matrix ledmap:"));
      for (unsigned i=0; i<customMappingSize; i++) {
        if (!(i%mAnimatorLight::Segment::maxWidth)) DEBUG_PRINTLN();
        DEBUG_PRINTF("%4d,", customMappingTable[i]);
      }
      DEBUG_PRINTLN();

      DEBUG_LINE_HERE;
    } 
    else  // memory allocation error
    {
      DEBUG_PRINTLN(F("Ledmap alloc error."));
      isMatrix = false;
      panels = 0;
      panel.clear();
      ALOG_INF(PSTR("panel.clear() HERE A?????????????????????????????????????????????????????"));
      Segment::maxWidth = _length;
      Segment::maxHeight = 1;
      resetSegments();
    }

    DEBUG_LINE_HERE;
    
  }

  ALOG_INF(PSTR("setUpMatrix Complete")); Serial.flush();
  
}
#endif


///////////////////////////////////////////////////////////
// mAnimatorLight::Segment:: routines
///////////////////////////////////////////////////////////

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS


// pixel is clipped if it falls outside clipping range
// if clipping start > stop the clipping range is inverted
bool mAnimatorLight::Segment::isPixelXYClipped(int x, int y) const {
  if (tkr_anim->blendingStyle != TRANSITION_FADE && isInTransition() && _clipStart != _clipStop) {
    const bool invertX = _clipStart  > _clipStop;
    const bool invertY = _clipStartY > _clipStopY;
    const int  cStartX = invertX ? _clipStop   : _clipStart;
    const int  cStopX  = invertX ? _clipStart  : _clipStop;
    const int  cStartY = invertY ? _clipStopY  : _clipStartY;
    const int  cStopY  = invertY ? _clipStartY : _clipStopY;
    if (tkr_anim->blendingStyle == TRANSITION_FAIRY_DUST) {
      const unsigned width = cStopX - cStartX;          // assumes full segment width (faster than virtualWidth())
      const unsigned len = width * (cStopY - cStartY);  // assumes full segment height (faster than virtualHeight())
      if (len < 2) return false;
      const unsigned shuffled = hashInt(x + y * width) % len;
      const unsigned pos = (shuffled * 0xFFFFU) / len;
      return progress() <= pos;
    }
    if (tkr_anim->blendingStyle == TRANSITION_CIRCULAR_IN || tkr_anim->blendingStyle == TRANSITION_CIRCULAR_OUT) {
      const int cx   = (cStopX-cStartX+1) / 2;
      const int cy   = (cStopY-cStartY+1) / 2;
      const bool out = (tkr_anim->blendingStyle == TRANSITION_CIRCULAR_OUT);
      const unsigned prog = out ? progress() : 0xFFFFU - progress();
      int radius2    = max(cx, cy) * prog / 0xFFFF;
      radius2 = 2 * radius2 * radius2;
      if (radius2 == 0) return out;
      const int dx = x - cx;
      const int dy = y - cy;
      const bool outside = dx * dx + dy * dy > radius2;
      return out ? outside : !outside;
    }
    bool xInside = (x >= cStartX && x < cStopX); if (invertX) xInside = !xInside;
    bool yInside = (y >= cStartY && y < cStopY); if (invertY) yInside = !yInside;
    const bool clip = tkr_anim->blendingStyle == TRANSITION_OUTSIDE_IN ? xInside || yInside : xInside && yInside;
    return !clip;
  }
  return false;
}

void IRAM_ATTR_YN mAnimatorLight::Segment::setPixelColorXY(int x, int y, uint32_t col) const
{
  if (!isActive()) return;

  if ((unsigned)x >= vWidth() || (unsigned)y >= vHeight()) return;

  #ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS_PIXELS
  if (decimate > 1)
  {
    const uint16_t pattern_width  = vWidth();
    const uint16_t pattern_height = vHeight();

    const uint16_t grouped_width =
      ((transpose ? height() : width()) + groupLength() - 1) /
      groupLength();

    const uint16_t grouped_height =
      ((transpose ? width() : height()) + groupLength() - 1) /
      groupLength();

    const uint16_t output_width =
      mirror ? (grouped_width + 1) / 2 : grouped_width;

    const uint16_t output_height =
      mirror_y ? (grouped_height + 1) / 2 : grouped_height;

    for (uint8_t repeat_y = 0; repeat_y < decimate; repeat_y++)
    {
      const uint32_t repeated_y =
        static_cast<uint32_t>(y) +
        static_cast<uint32_t>(repeat_y) * pattern_height;

      if (repeated_y >= output_height) break;

      for (uint8_t repeat_x = 0; repeat_x < decimate; repeat_x++)
      {
        const uint32_t repeated_x =
          static_cast<uint32_t>(x) +
          static_cast<uint32_t>(repeat_x) * pattern_width;

        if (repeated_x >= output_width) break;

        setPixelColorXYRaw(
          static_cast<uint16_t>(repeated_x),
          static_cast<uint16_t>(repeated_y),
          col
        );
      }
    }

    return;
  }
  #endif

  setPixelColorXYRaw(x, y, col);
}


// /**
//  * @brief 
//  * To be determined, but by further inspection, this whole function is not used in WLED
//  * inline void     setPixelColorXYRaw(unsigned x, unsigned y, uint32_t c) const  { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; pixels[XY(x,y)] = c; }
//  * It is a small wrapper, that lets to direct pixel set. 
//  * 
//  * 
//  * @param x 
//  * @param y 
//  * @param col 
//  */
// void IRAM_ATTR mAnimatorLight::Segment::setPixelColorXY(int x, int y, uint32_t col)
// {
//   // Serial.println(__LINE__);
//   #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//   ALOG_INF(PSTR("Segment::setPixelColorXY(%d,%d|%d,%d,%d)"), x, y, R(col), G(col), B(col));
//   #endif

//   if (!isActive()) return; // not active
//   if (Segment::maxHeight==1) return; // not a matrix set-up
//   if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) return;
  
//   // Need to check, but at least normal call of setPixelColor applies it, not needing this. But I maybe need a double check/

//   bool flag_brightness_already_applied = false;
//   // This function bypassing the 1D to 2D set function that applies brightness, so we need to apply here before calling the busmanager
//   if (flag_brightness_already_applied==false) {
//     // uint8_t brightness = tkr_iLight->getBriRGB_Global();//scale8(_brightness_rgb, tkr_iLight->getBriRGB_Global());
//     uint8_t brightness = scale8(_brightness_rgb, tkr_iLight->getBriRGB_Global());
//     uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range
//     // Extract, scale, and repack in one step
//     col = RGBW32(
//       (R(col) * scale) >> 8,  // Red
//       (G(col) * scale) >> 8,  // Green
//       (B(col) * scale) >> 8,  // Blue
//       (W(col) * scale) >> 8   // White
//     );
//   }
  
//   if (reverse  ) x = virtualWidth()  - x - 1;
//   if (reverse_y) y = virtualHeight() - y - 1;
//   if (transpose) { uint16_t t = x; x = y; y = t; } // swap X & Y if segment transposed

//   x *= groupLength(); // expand to physical pixels
//   y *= groupLength(); // expand to physical pixels
//   if (x >= width() || y >= height()) return;

//   for (int j = 0; j < grouping; j++) {   // groupping vertically
  
//     for (int g = 0; g < grouping; g++) { // groupping horizontally
    
//       uint16_t xX = (x+g), yY = (y+j);
//       if (xX >= width() || yY >= height()) continue; // we have reached one dimension's end
      
//       #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//       ALOG_INF(PSTR("--------setPixelColorXY %d, %d, %d, %d, %d -- w%d h%d"), start + xX, startY + yY, R(tmpCol), G(tmpCol), B(tmpCol), width(), height());
//       #endif

//       tkr_anim->setPixelColorXY(start + xX, startY + yY, col);

//       if (mirror) { //set the corresponding horizontally mirrored pixel
//         if (transpose) tkr_anim->setPixelColorXY(start + xX, startY + height() - yY - 1, col);
//         else           tkr_anim->setPixelColorXY(start + width() - xX - 1, startY + yY, col);
//       }
//       if (mirror_y) { //set the corresponding vertically mirrored pixel
//         if (transpose) tkr_anim->setPixelColorXY(start + width() - xX - 1, startY + yY, col);
//         else           tkr_anim->setPixelColorXY(start + xX, startY + height() - yY - 1, col);
//       }
//       if (mirror_y && mirror) { //set the corresponding vertically AND horizontally mirrored pixel
//         tkr_anim->setPixelColorXY(width() - xX - 1, height() - yY - 1, col);
//       }
      
//     }
    
//   }

// }


#ifdef USE_AA_PIXELS
// anti-aliased version of setPixelColorXY()
void mAnimatorLight::Segment::setPixelColorXY(float x, float y, uint32_t col, bool aa)
{
  DEBUG_LINE_HERE;
  if (!isActive()) return; // not active
  if (x<0.0f || x>1.0f || y<0.0f || y>1.0f)
  {
  
    ALOG_ERR(PSTR("not normalized setPixelColorXY"));//: x=%f, y=%f"), x, y);
    return; // not normalized
  }

  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
DEBUG_LINE_HERE;
  float fX = x * (cols-1);
  float fY = y * (rows-1);
  if (aa) {
    DEBUG_LINE_HERE;
    uint16_t xL = roundf(fX-0.49f);
    uint16_t xR = roundf(fX+0.49f);
    uint16_t yT = roundf(fY-0.49f);
    uint16_t yB = roundf(fY+0.49f);
    float    dL = (fX - xL)*(fX - xL);
    float    dR = (xR - fX)*(xR - fX);
    float    dT = (fY - yT)*(fY - yT);
    float    dB = (yB - fY)*(yB - fY);
    uint32_t cXLYT = getPixelColorXY(xL, yT);
    uint32_t cXRYT = getPixelColorXY(xR, yT);
    uint32_t cXLYB = getPixelColorXY(xL, yB);
    uint32_t cXRYB = getPixelColorXY(xR, yB);

    if (xL!=xR && yT!=yB) {
      setPixelColorXY(xL, yT, color_blend(col, cXLYT, uint8_t(sqrtf(dL*dT)*255.0f))); // blend TL pixel
      setPixelColorXY(xR, yT, color_blend(col, cXRYT, uint8_t(sqrtf(dR*dT)*255.0f))); // blend TR pixel
      setPixelColorXY(xL, yB, color_blend(col, cXLYB, uint8_t(sqrtf(dL*dB)*255.0f))); // blend BL pixel
      setPixelColorXY(xR, yB, color_blend(col, cXRYB, uint8_t(sqrtf(dR*dB)*255.0f))); // blend BR pixel
    } else if (xR!=xL && yT==yB) {
      setPixelColorXY(xR, yT, color_blend(col, cXLYT, uint8_t(dL*255.0f))); // blend L pixel
      setPixelColorXY(xR, yT, color_blend(col, cXRYT, uint8_t(dR*255.0f))); // blend R pixel
    } else if (xR==xL && yT!=yB) {
      setPixelColorXY(xR, yT, color_blend(col, cXLYT, uint8_t(dT*255.0f))); // blend T pixel
      setPixelColorXY(xL, yB, color_blend(col, cXLYB, uint8_t(dB*255.0f))); // blend B pixel
    } else {
      setPixelColorXY(xL, yT, col); // exact match (x & y land on a pixel)
    }
  } else {
    DEBUG_LINE_HERE;
    setPixelColorXY(uint16_t(roundf(fX)), uint16_t(roundf(fY)), col);
  }
  DEBUG_LINE_HERE;
}
#endif


// returns RGBW values of pixel
uint32_t IRAM_ATTR_YN mAnimatorLight::Segment::getPixelColorXY(int x, int y) const {
  if (!isActive()) return 0; // not active
  if ((unsigned)x >= vWidth() || (unsigned)y >= vHeight()) return 0;  // if pixel would fall out of virtual segment just exit
  return getPixelColorXYRaw(x,y);
}


// // returns RGBW values of pixel
// uint32_t mAnimatorLight::Segment::getPixelColorXY(uint16_t x, uint16_t y) const
// {
//   int i = XY(x,y);
//   // if (leds) return RGBW32(leds[i].r, leds[i].g, leds[i].b, 0);
//   if (reverse  ) x = virtualWidth()  - x - 1;
//   if (reverse_y) y = virtualHeight() - y - 1;
//   if (transpose) { uint16_t t = x; x = y; y = t; } // swap X & Y if segment transposed
//   x *= groupLength(); // expand to physical pixels
//   y *= groupLength(); // expand to physical pixels
//   if (x >= width() || y >= height()) return 0;
//   return tkr_anim->getPixelColorXY(start + x, startY + y);


//   // // Serial.println(__LINE__);
//   // // DEBUG_LINE_HERE;
//   // if (!isActive()) return 0; // not active
//   // // DEBUG_LINE_HERE;
//   // if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) 
//   // {

//   //   ALOG_ERR(PSTR("OUT OF RANGE getPixelColorXY: x=%d, y=%d, virtualWidth=%d, virtualHeight=%d"), x, y, virtualWidth(), virtualHeight());

//   //   return 0;  // if pixel would fall out of virtual segment just exit

//   // }
//   // // DEBUG_LINE_HERE3

//   // if (reverse  ) x = virtualWidth()  - x - 1;
//   // // DEBUG_LINE_HERE;
//   // if (reverse_y) y = virtualHeight() - y - 1;
//   // // DEBUG_LINE_HERE;
//   // if (transpose) { uint16_t t = x; x = y; y = t; } // swap X & Y if segment transposed
//   // // DEBUG_LINE_HERE;
//   // x *= groupLength(); // expand to physical pixels
//   // // DEBUG_LINE_HERE;
//   // y *= groupLength(); // expand to physical pixels
//   // // DEBUG_LINE_HERE;
//   // if (x >= width() || y >= height()) return 0;
  
//   // // DEBUG_LINE_HERE3
//   // // Caution, should now call the stirp one (not the segment, needs renamed to be clear!!!)
//   // return tkr_anim->getPixelColorXY(start + x, startY + y);
//   // // DEBUG_LINE_HERE;
// }


#ifdef ENABLE_DEVFEATURE_LIGHT__PIXELS_BUFFER_RAW    

// 2D blurring, can be asymmetrical
void mAnimatorLight::Segment::blur2D(uint8_t blur_x, uint8_t blur_y, bool smear) {
  if (!isActive()) return; // not active
  const unsigned cols = vWidth();
  const unsigned rows = vHeight();
  const auto XY = [&](unsigned x, unsigned y){ return x + y*cols; };
  uint32_t lastnew; // not necessary to initialize lastnew and last, as both will be initialized by the first loop iteration
  uint32_t last;
  if (blur_x) {
    const uint8_t keepx = smear ? 255 : 255 - blur_x;
    const uint8_t seepx = blur_x >> 1;
    for (unsigned row = 0; row < rows; row++) { // blur rows (x direction)
      uint32_t carryover = BLACK;
      uint32_t curnew = BLACK;
      for (unsigned x = 0; x < cols; x++) {
        uint32_t cur = getPixelColorRaw(XY(x, row));
        uint32_t part = color_fade(cur, seepx);
        curnew = color_fade(cur, keepx);
        if (x > 0) {
          if (carryover) curnew = color_add(curnew, carryover);
          uint32_t prev = color_add(lastnew, part);
          // optimization: only set pixel if color has changed
          if (last != prev) setPixelColorRaw(XY(x - 1, row), prev);
        } else setPixelColorRaw(XY(x, row), curnew); // first pixel
        lastnew = curnew;
        last = cur; // save original value for comparison on next iteration
        carryover = part;
      }
      setPixelColorRaw(XY(cols-1, row), curnew); // set last pixel
    }
  }
  if (blur_y) {
    const uint8_t keepy = smear ? 255 : 255 - blur_y;
    const uint8_t seepy = blur_y >> 1;
    for (unsigned col = 0; col < cols; col++) {
      uint32_t carryover = BLACK;
      uint32_t curnew = BLACK;
      for (unsigned y = 0; y < rows; y++) {
        uint32_t cur = getPixelColorRaw(XY(col, y));
        uint32_t part = color_fade(cur, seepy);
        curnew = color_fade(cur, keepy);
        if (y > 0) {
          if (carryover) curnew = color_add(curnew, carryover);
          uint32_t prev = color_add(lastnew, part);
          // optimization: only set pixel if color has changed
          if (last != prev) setPixelColorRaw(XY(col, y - 1), prev);
        } else setPixelColorRaw(XY(col, y), curnew); // first pixel
        lastnew = curnew;
        last = cur; //save original value for comparison on next iteration
        carryover = part;
      }
      setPixelColorRaw(XY(col, rows - 1), curnew);
    }
  }
}

#else

// 2D blurring, can be asymmetrical
// void mAnimatorLight::Segment::blur2D(uint8_t blur_x, uint8_t blur_y, bool smear) {
  
//   if (!isActive()) return; // not active
//   const unsigned cols = vWidth();
//   const unsigned rows = vHeight();
//   uint32_t lastnew;
//   uint32_t last;
//   if (blur_x) {
//     const uint8_t keepx = smear ? 255 : 255 - blur_x;
//     const uint8_t seepx = blur_x >> 1;
//     for (unsigned row = 0; row < rows; row++) { // blur rows (x direction)
//       uint32_t carryover = BLACK;
//       uint32_t curnew = BLACK;
//       for (unsigned x = 0; x < cols; x++) {
//         uint32_t cur = getPixelColorXY(x, row);
//         uint32_t part = color_fade(cur, seepx);
//         curnew = color_fade(cur, keepx);
//         if (x > 0) {
//           if (carryover) curnew = color_add(curnew, carryover);
//           uint32_t prev = color_add(lastnew, part);
//           // optimization: only set pixel if color has changed
//           if (last != prev) setPixelColorXY(x - 1, row, prev);
//         } else setPixelColorXY(x, row, curnew); // first pixel
//         lastnew = curnew;
//         last = cur; // save original value for comparison on next iteration
//         carryover = part;
//       }
//       setPixelColorXY(cols-1, row, curnew); // set last pixel
//     }
//   }
//   if (blur_y) {
//     const uint8_t keepy = smear ? 255 : 255 - blur_y;
//     const uint8_t seepy = blur_y >> 1;
//     for (unsigned col = 0; col < cols; col++) {
//       uint32_t carryover = BLACK;
//       uint32_t curnew = BLACK;
//       for (unsigned y = 0; y < rows; y++) {
//         uint32_t cur = getPixelColorXY(col, y);
//         uint32_t part = color_fade(cur, seepy);
//         curnew = color_fade(cur, keepy);
//         if (y > 0) {
//           if (carryover) curnew = color_add(curnew, carryover);
//           uint32_t prev = color_add(lastnew, part);
//           // optimization: only set pixel if color has changed
//           if (last != prev) setPixelColorXY(col, y - 1, prev);
//         } else setPixelColorXY(col, y, curnew); // first pixel
//         lastnew = curnew;
//         last = cur; //save original value for comparison on next iteration
//         carryover = part;
//       }
//       setPixelColorXY(col, rows - 1, curnew);
//     }
//   }
// }


// 2D blurring, can be asymmetrical
void mAnimatorLight::Segment::blur2D(uint8_t blur_x, uint8_t blur_y, bool smear) const {
  if (!isActive()) return; // not active
  const unsigned cols = vWidth();
  const unsigned rows = vHeight();
  const auto XY = [&](unsigned x, unsigned y){ return x + y*cols; };
  if (blur_x) {
    const uint8_t keepx = smear ? 255 : 255 - blur_x;
    const uint8_t seepx = blur_x >> 1;
    for (unsigned row = 0; row < rows; row++) { // blur rows (x direction)
      // handle first pixel in row to avoid conditional in loop (faster)
      uint32_t cur = getPixelColorRaw(XY(0, row));
      uint32_t carryover = fast_color_scale(cur, seepx);
      setPixelColorRaw(XY(0, row), fast_color_scale(cur, keepx));
      for (unsigned x = 1; x < cols; x++) {
         cur = getPixelColorRaw(XY(x, row));
        uint32_t part = fast_color_scale(cur, seepx);
        cur = fast_color_scale(cur, keepx);
        cur = color_add(cur, carryover);
        setPixelColorRaw(XY(x - 1, row), color_add(getPixelColorRaw(XY(x-1, row)), part)); // previous pixel
        setPixelColorRaw(XY(x, row), cur); // current pixel
        carryover = part;
      }
    }
  }
  if (blur_y) {
    const uint8_t keepy = smear ? 255 : 255 - blur_y;
    const uint8_t seepy = blur_y >> 1;
    for (unsigned col = 0; col < cols; col++) {
      // handle first pixel in column
      uint32_t cur = getPixelColorRaw(XY(col, 0));
      uint32_t carryover = fast_color_scale(cur, seepy);
      setPixelColorRaw(XY(col, 0), fast_color_scale(cur, keepy));
      for (unsigned y = 1; y < rows; y++) {
        cur = getPixelColorRaw(XY(col, y));
        uint32_t part = fast_color_scale(cur, seepy);
        cur = fast_color_scale(cur, keepy);
        cur = color_add(cur, carryover);
        setPixelColorRaw(XY(col, y - 1), color_add(getPixelColorRaw(XY(col, y-1)), part)); // previous pixel
        setPixelColorRaw(XY(col, y), cur); // current pixel
        carryover = part;
      }
    }
  }
}

#endif










// Blends the specified color with the existing pixel color.
// void mAnimatorLight::Segment::blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend) {
//   setPixelColorXY(x, y, color_blend(getPixelColorXY(x,y), color, blend));
// }

// // Adds the specified color with the existing pixel color perserving color balance.
// void mAnimatorLight::Segment::addPixelColorXY(int x, int y, uint32_t color, bool fast) {
//   if (!isActive()) return; // not active
//   if (x >= virtualWidth() || y >= virtualHeight() || x<0 || y<0) return;  // if pixel would fall out of virtual segment just exit
//   setPixelColorXY(x, y, color_add(getPixelColorXY(x,y), color, fast));
// }

// void mAnimatorLight::Segment::fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade) {
//   if (!isActive()) return; // not active
//   setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), fade, true));
// }

// blurRow: perform a blur on a row of a rectangular matrix
// void mAnimatorLight::Segment::blurRow(uint16_t row, fract8 blur_amount) {
//   if (!isActive() || blur_amount == 0) return; // not active
//   const uint_fast16_t cols = virtualWidth();
//   const uint_fast16_t rows = virtualHeight();

//   if (row >= rows) return;
//   // blur one row
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
//   CRGB carryover = Black;
//   for (unsigned x = 0; x < cols; x++) {
//     CRGB cur = getPixelColorXY(x, row);
//     CRGB before = cur;     // remember color before blur
//     CRGB part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;
//     if (x>0) {
//       CRGB prev = CRGB(getPixelColorXY(x-1, row)) + part;
//       setPixelColorXY((int)x-1, (int)row, prev);
//     }
//     if (before != cur)         // optimization: only set pixel if color has changed
//       setPixelColorXY((int)x, (int)row, cur);
//     carryover = part;
//   }
// }

// blurCol: perform a blur on a column of a rectangular matrix
// void mAnimatorLight::Segment::blurCol(uint16_t col, fract8 blur_amount) {
//   if (!isActive() || blur_amount == 0) return; // not active
//   const uint_fast16_t cols = virtualWidth();
//   const uint_fast16_t rows = virtualHeight();

//   if (col >= cols) return;
//   // blur one column
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
//   CRGB carryover = Black;
//   for (unsigned y = 0; y < rows; y++) {
//     CRGB cur = getPixelColorXY(col, y);
//     CRGB part = cur;
//     CRGB before = cur;     // remember color before blur
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;
//     if (y>0) {
//       CRGB prev = CRGB(getPixelColorXY(col, y-1)) + part;
//       setPixelColorXY((int)col, (int)y-1, prev);
//     }
//     if (before != cur)         // optimization: only set pixel if color has changed
//       setPixelColorXY((int)col, (int)y, cur);
//     carryover = part;
//   }
// }

// 1D Box blur (with added weight - blur_amount: [0=no blur, 255=max blur])
// void mAnimatorLight::Segment::box_blur(uint16_t i, bool vertical, fract8 blur_amount) {
//   if (!isActive() || blur_amount == 0) return; // not active
//   const uint16_t cols = virtualWidth();
//   const uint16_t rows = virtualHeight();
//   const uint16_t dim1 = vertical ? rows : cols;
//   const uint16_t dim2 = vertical ? cols : rows;
//   if (i >= dim2) return;
//   const float seep = blur_amount/255.f;
//   const float keep = 3.f - 2.f*seep;
//   // 1D box blur
//   CRGB tmp[dim1];
//   for (int j = 0; j < dim1; j++) {
//     uint16_t x = vertical ? i : j;
//     uint16_t y = vertical ? j : i;
//     int16_t xp = vertical ? x : x-1;  // "signed" to prevent underflow
//     int16_t yp = vertical ? y-1 : y;  // "signed" to prevent underflow
//     uint16_t xn = vertical ? x : x+1;
//     uint16_t yn = vertical ? y+1 : y;
//     CRGB curr = getPixelColorXY(x,y);
//     CRGB prev = (xp<0 || yp<0) ? Black : getPixelColorXY(xp,yp);
//     CRGB next = ((vertical && yn>=dim1) || (!vertical && xn>=dim1)) ? Black : getPixelColorXY(xn,yn);
//     uint16_t r, g, b;
//     r = (curr.r*keep + (prev.r + next.r)*seep) / 3;
//     g = (curr.g*keep + (prev.g + next.g)*seep) / 3;
//     b = (curr.b*keep + (prev.b + next.b)*seep) / 3;
//     tmp[j] = CRGB(r,g,b);
//   }
//   for (int j = 0; j < dim1; j++) {
//     uint16_t x = vertical ? i : j;
//     uint16_t y = vertical ? j : i;
//     setPixelColorXY(x, y, tmp[j]);
//   }
// }

// blur1d: one-dimensional blur filter. Spreads light to 2 line neighbors.
// blur2d: two-dimensional blur filter. Spreads light to 8 XY neighbors.
//
//           0 = no spread at all
//          64 = moderate spreading
//         172 = maximum smooth, even spreading
//
//         173..255 = wider spreading, but increasing flicker
//
//         Total light is NOT entirely conserved, so many repeated
//         calls to 'blur' will also result in the light fading,
//         eventually all the way to black; this is by design so that
//         it can be used to (slowly) clear the LEDs to black.

// void mAnimatorLight::Segment::blur1d(fract8 blur_amount) {
//   const uint16_t rows = virtualHeight();
//   for (unsigned y = 0; y < rows; y++) blurRow(y, blur_amount);
// }

void mAnimatorLight::Segment::moveX(int delta, bool wrap) const {
  if (!isActive() || !delta) return; // not active
  const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
  const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
  const auto XY = [&](unsigned x, unsigned y){ return x + y*vW; };
  int absDelta = abs(delta);
  if (absDelta >= vW) return;
  uint32_t newPxCol[vW];
  int newDelta;
  int stop = vW;
  int start = 0;
  if (wrap) newDelta = (delta + vW) % vW; // +cols in case delta < 0
  else {
    if (delta < 0) start = absDelta;
    stop = vW - absDelta;
    newDelta = delta > 0 ? delta : 0;
  }
  for (int y = 0; y < vH; y++) {
    for (int x = 0; x < stop; x++) {
      int srcX = x + newDelta;
      if (wrap) srcX %= vW; // Wrap using modulo when `wrap` is true
      newPxCol[x] = getPixelColorRaw(XY(srcX, y));
    }
    for (int x = 0; x < stop; x++) setPixelColorRaw(XY(x + start, y), newPxCol[x]);
  }
}

void mAnimatorLight::Segment::moveY(int delta, bool wrap) const {
  if (!isActive() || !delta) return; // not active
  const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
  const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
  const auto XY = [&](unsigned x, unsigned y){ return x + y*vW; };
  int absDelta = abs(delta);
  if (absDelta >= vH) return;
  uint32_t newPxCol[vH];
  int newDelta;
  int stop = vH;
  int start = 0;
  if (wrap) newDelta = (delta + vH) % vH; // +rows in case delta < 0
  else {
    if (delta < 0) start = absDelta;
    stop = vH - absDelta;
    newDelta = delta > 0 ? delta : 0;
  }
  for (int x = 0; x < vW; x++) {
    for (int y = 0; y < stop; y++) {
      int srcY = y + newDelta;
      if (wrap) srcY %= vH; // Wrap using modulo when `wrap` is true
      newPxCol[y] = getPixelColorRaw(XY(x, srcY));
    }
    for (int y = 0; y < stop; y++) setPixelColorRaw(XY(x, y + start), newPxCol[y]);
  }
}

// move() - move all pixels in desired direction delta number of pixels
// @param dir direction: 0=left, 1=left-up, 2=up, 3=right-up, 4=right, 5=right-down, 6=down, 7=left-down
// @param delta number of pixels to move
// @param wrap around
void mAnimatorLight::Segment::move(unsigned dir, unsigned delta, bool wrap) const {
  if (delta==0) return;
  switch (dir) {
    case 0: moveX( delta, wrap);                      break;
    case 1: moveX( delta, wrap); moveY( delta, wrap); break;
    case 2:                      moveY( delta, wrap); break;
    case 3: moveX(-delta, wrap); moveY( delta, wrap); break;
    case 4: moveX(-delta, wrap);                      break;
    case 5: moveX(-delta, wrap); moveY(-delta, wrap); break;
    case 6:                      moveY(-delta, wrap); break;
    case 7: moveX( delta, wrap); moveY(-delta, wrap); break;
  }
}


void mAnimatorLight::Segment::drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t col, bool soft) const {
  if (!isActive() || radius == 0) return; // not active
  if (soft) {
    // Xiaolin Wu’s algorithm
    const int rsq = radius*radius;
    int x = 0;
    int y = radius;
    unsigned oldFade = 0;
    while (x < y) {
      float yf = sqrtf(float(rsq - x*x)); // needs to be floating point
      uint8_t fade = float(0xFF) * (ceilf(yf) - yf); // how much color to keep
      if (oldFade > fade) y--;
      oldFade = fade;
      int px, py;
      for (uint8_t i = 0; i < 16; i++) {
          int swaps = (i & 0x4 ? 1 : 0); // 0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1
          int adj =  (i < 8) ? 0 : 1;    // 0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1
          int dx = (i & 1) ? -1 : 1;     // 1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1
          int dy = (i & 2) ? -1 : 1;     // 1,  1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1
          if (swaps) {
              px = cx + (y - adj) * dx;
              py = cy + x * dy;
          } else {
              px = cx + x * dx;
              py = cy + (y - adj) * dy;
          }
          uint32_t pixCol = getPixelColorXY(px, py);
          setPixelColorXY(px, py, adj ?
              color_blend(pixCol, col, fade) :
              color_blend(col, pixCol, fade));
      }
      x++;
    }
  } else {
    // Bresenham’s Algorithm
    int d = 3 - (2*radius);
    int y = radius, x = 0;
    while (y >= x) {
    for (int i = 0; i < 4; i++) {
        int dx = (i & 1) ? -x : x;
        int dy = (i & 2) ? -y : y;
        setPixelColorXY(cx + dx, cy + dy, col);
        setPixelColorXY(cx + dy, cy + dx, col);
    }
      x++;
      if (d > 0) {
        y--;
        d += 4 * (x - y) + 10;
      } else {
        d += 4 * x + 6;
      }
    }
  }
}


// // by stepko, taken from https://editor.soulmatelights.com/gallery/573-blobs
// void mAnimatorLight::Segment::fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB col) {
//   if (!isActive() || radius == 0) return; // not active
//   const uint16_t cols = virtualWidth();
//   const uint16_t rows = virtualHeight();
//   for (int16_t y = -radius; y <= radius; y++) {
//     for (int16_t x = -radius; x <= radius; x++) {
//       if (x * x + y * y <= radius * radius &&
//           int16_t(cx)+x>=0 && int16_t(cy)+y>=0 &&
//           int16_t(cx)+x<cols && int16_t(cy)+y<rows)
//         setPixelColorXY(cx + x, cy + y, col);
//     }
//   }
// }

// by stepko, taken from https://editor.soulmatelights.com/gallery/573-blobs
void mAnimatorLight::Segment::fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t col, bool soft) const {
  if (!isActive() || radius == 0) return; // not active
  const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
  const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
  // draw soft bounding circle
  if (soft) drawCircle(cx, cy, radius, col, soft);
  // fill it
  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      if (x * x + y * y <= radius * radius &&
          int(cx)+x >= 0 && int(cy)+y >= 0 &&
          int(cx)+x < vW && int(cy)+y < vH)
        setPixelColorXY(cx + x, cy + y, col);
    }
  }
}

//line function
void mAnimatorLight::Segment::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c, bool soft) const {
  if (!isActive()) return; // not active
  const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
  const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
  if (x0 >= vW || x1 >= vW || y0 >= vH || y1 >= vH) return;

  const int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1; // x distance & step
  const int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; // y distance & step

  // single pixel (line length == 0)
  if (dx+dy == 0) {
    setPixelColorXY(x0, y0, c);
    return;
  }

  if (soft) {
    // Xiaolin Wu’s algorithm
    const bool steep = dy > dx;
    if (steep) {
      // we need to go along longest dimension
      std::swap(x0,y0);
      std::swap(x1,y1);
    }
    if (x0 > x1) {
      // we need to go in increasing fashion
      std::swap(x0,x1);
      std::swap(y0,y1);
    }
    float gradient = x1-x0 == 0 ? 1.0f : float(y1-y0) / float(x1-x0);
    float intersectY = y0;
    for (int x = x0; x <= x1; x++) {
      uint8_t keep = float(0xFF) * (intersectY-int(intersectY)); // how much color to keep
      uint8_t seep = 0xFF - keep; // how much background to keep
      int y = int(intersectY);
      if (steep) std::swap(x,y);  // temporaryly swap if steep
      // pixel coverage is determined by fractional part of y co-ordinate
      blendPixelColorXY(x, y, c, seep);
      blendPixelColorXY(x+int(steep), y+int(!steep), c, keep);
      intersectY += gradient;
      if (steep) std::swap(x,y);  // restore if steep
    }
  } else {
    // Bresenham's algorithm
    int err = (dx>dy ? dx : -dy)/2;   // error direction
    for (;;) {
      setPixelColorXY(x0, y0, c);
      if (x0==x1 && y0==y1) break;
      int e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
    }
  }
}




// // by stepko, taken from https://editor.soulmatelights.com/gallery/573-blobs
// void mAnimatorLight::Segment::fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t col, bool soft) {
//   if (!isActive() || radius == 0) return; // not active
//   const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
//   const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
//   // draw soft bounding circle
//   if (soft) drawCircle(cx, cy, radius, col, soft);
//   // pre-scale color for all pixels
//   col = color_fade(col, _segBri);
//   _colorScaled = true;
//   // fill it
//   for (int y = -radius; y <= radius; y++) {
//     for (int x = -radius; x <= radius; x++) {
//       if (x * x + y * y <= radius * radius &&
//           int(cx)+x >= 0 && int(cy)+y >= 0 &&
//           int(cx)+x < vW && int(cy)+y < vH)
//         setPixelColorXY(cx + x, cy + y, col);
//     }
//   }
//   _colorScaled = false;
// }


// void mAnimatorLight::Segment::nscale8(uint8_t scale) {
//   if (!isActive()) return; // not active
//   const uint16_t cols = virtualWidth();
//   const uint16_t rows = virtualHeight();
//   for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
//     setPixelColorXY(x, y, CRGB(getPixelColorXY(x, y)).nscale8(scale));
//   }
// }

// //line function
// void mAnimatorLight::Segment::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c) {
//   if (!isActive()) return; // not active
//   const uint16_t cols = virtualWidth();
//   const uint16_t rows = virtualHeight();
//   if (x0 >= cols || x1 >= cols || y0 >= rows || y1 >= rows) return;
//   const int16_t dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
//   const int16_t dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
//   int16_t err = (dx>dy ? dx : -dy)/2, e2;
//   for (;;) {
//     setPixelColorXY(x0,y0,c);
//     if (x0==x1 && y0==y1) break;
//     e2 = err;
//     if (e2 >-dx) { err -= dy; x0 += sx; }
//     if (e2 < dy) { err += dx; y0 += sy; }
//   }
// }

#include "font/console_font_4x6.h"
#include "font/console_font_5x8.h"
#include "font/console_font_5x12.h"
#include "font/console_font_6x8.h"
#include "font/console_font_7x9.h"



// draws a raster font character on canvas
// only supports: 4x6=24, 5x8=40, 5x12=60, 6x8=48 and 7x9=63 fonts ATM
void mAnimatorLight::Segment::drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2, int8_t rotate, bool usePalGrad) {
  if (!isActive()) return; // not active
  if (chr < 32 || chr > 126) return; // only ASCII 32-126 supported
  chr -= 32; // align with font table entries
  const int font = w*h;

  CRGB col = CRGB(color);
  CRGBPalette16 grad = CRGBPalette16(col, col2 ? CRGB(col2) : col);
  if(usePalGrad) grad = pSEGPALETTE; // selected palette as gradient

  //if (w<5 || w>6 || h!=8) return;
  for (int i = 0; i<h; i++) { // character height
    uint8_t bits = 0;
    switch (font) {
      case 24: bits = pgm_read_byte_near(&console_font_4x6[(chr * h) + i]); break;  // 5x8 font
      case 40: bits = pgm_read_byte_near(&console_font_5x8[(chr * h) + i]); break;  // 5x8 font
      case 48: bits = pgm_read_byte_near(&console_font_6x8[(chr * h) + i]); break;  // 6x8 font
      case 63: bits = pgm_read_byte_near(&console_font_7x9[(chr * h) + i]); break;  // 7x9 font
      case 60: bits = pgm_read_byte_near(&console_font_5x12[(chr * h) + i]); break; // 5x12 font
      default: return;
    }
    uint32_t c = ColorFromPalette(grad, (i+1)*255/h, 255, NOBLEND);
    // pre-scale color for all pixels
    c = color_fade(c, opacity);
    _colorScaled = true;
    for (int j = 0; j<w; j++) { // character width
      int x0, y0;
      switch (rotate) {
        case -1: x0 = x + (h-1) - i; y0 = y + (w-1) - j; break; // -90 deg
        case -2:
        case  2: x0 = x + j;         y0 = y + (h-1) - i; break; // 180 deg
        case  1: x0 = x + i;         y0 = y + j;         break; // +90 deg
        default: x0 = x + (w-1) - j; y0 = y + i;         break; // no rotation
      }
      if (x0 < 0 || x0 >= (int)vWidth() || y0 < 0 || y0 >= (int)vHeight()) continue; // drawing off-screen
      if (((bits>>(j+(8-w))) & 0x01)) { // bit set
        setPixelColorXY(x0, y0, c);
      }
    }
    _colorScaled = false;
  }
}



// // draws a raster font character on canvas
// // only supports: 4x6=24, 5x8=40, 5x12=60, 6x8=48 and 7x9=63 fonts ATM
// void mAnimatorLight::Segment::drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2, int8_t rotate) {
//   if (!isActive()) return; // not active
//   if (chr < 32 || chr > 126) return; // only ASCII 32-126 supported
//   chr -= 32; // align with font table entries
//   const uint16_t cols = virtualWidth();
//   const uint16_t rows = virtualHeight();
//   const int font = w*h;

//   CRGB col = CRGB(color);
//   CRGBPalette16 grad = CRGBPalette16(col, col2 ? CRGB(col2) : col);

//   //if (w<5 || w>6 || h!=8) return;
//   for (int i = 0; i<h; i++) { // character height
//     uint8_t bits = 0;
//     switch (font) {
//       case 24: bits = pgm_read_byte_near(&console_font_4x6[(chr * h) + i]); break;  // 5x8 font
//       case 40: bits = pgm_read_byte_near(&console_font_5x8[(chr * h) + i]); break;  // 5x8 font
//       case 48: bits = pgm_read_byte_near(&console_font_6x8[(chr * h) + i]); break;  // 6x8 font
//       case 63: bits = pgm_read_byte_near(&console_font_7x9[(chr * h) + i]); break;  // 7x9 font
//       case 60: bits = pgm_read_byte_near(&console_font_5x12[(chr * h) + i]); break; // 5x12 font
//       default: return;
//     }
//     col = ColorFromPalette(grad, (i+1)*255/h, 255, NOBLEND);
//     for (int j = 0; j<w; j++) { // character width
//       int x0, y0;
//       switch (rotate) {
//         case -1: x0 = x + (h-1) - i; y0 = y + (w-1) - j; break; // -90 deg
//         case -2:
//         case  2: x0 = x + j;         y0 = y + (h-1) - i; break; // 180 deg
//         case  1: x0 = x + i;         y0 = y + j;         break; // +90 deg
//         default: x0 = x + (w-1) - j; y0 = y + i;         break; // no rotation
//       }
//       if (x0 < 0 || x0 >= cols || y0 < 0 || y0 >= rows) continue; // drawing off-screen
//       if (((bits>>(j+(8-w))) & 0x01)) { // bit set
//         // DEBUG_LINE_HERE_MARKER;
//         setPixelColorXY(x0, y0, col);
//       }
//     }
//   }
// }

/**
 * @brief Need to move the segment.custom1 etc out of here, and into the effect itself so this function can be more generic. 
 * 
 * @param chr 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 * @param rotate 
 * @param solidPerChar 
 * @param horizontalGradient 
 * @param backgroundGradientHorizontal 
 */
void mAnimatorLight::Segment::drawCharacter_UsingGradientPalletes(  
    unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h,   
    int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal) {
  
  if (!isActive()) return; // not active
  if (chr < 32 || chr > 126) return; // only ASCII 32-126 supported
  chr -= 32; // align with font table entries
  const uint16_t cols = virtualWidth();
  const uint16_t rows = virtualHeight();
  const int font = w * h;

  // Fetch the background gradient color from the unloaded palette using params_user[0]
  uint16_t backgroundPaletteId = custom1; // Palette ID for background
  uint32_t backgroundColor;  // Will be fetched later based on pixel position

  // Iterate over character rows (height)
  for (int i = 0; i < h; i++) { 
    uint8_t bits = 0;
    switch (font) {
      case 24: bits = pgm_read_byte_near(&console_font_4x6[(chr * h) + i]); break;
      case 40: bits = pgm_read_byte_near(&console_font_5x8[(chr * h) + i]); break;
      case 48: bits = pgm_read_byte_near(&console_font_6x8[(chr * h) + i]); break;
      case 63: bits = pgm_read_byte_near(&console_font_7x9[(chr * h) + i]); break;
      case 60: bits = pgm_read_byte_near(&console_font_5x12[(chr * h) + i]); break;
      default: return;
    }

    for (int j = 0; j < w; j++) { // character width
      int x0, y0;
      switch (rotate) {
        case -1: x0 = x + (h - 1) - i; y0 = y + (w - 1) - j; break; // -90 deg
        case -2:
        case  2: x0 = x + j;         y0 = y + (h - 1) - i; break; // 180 deg
        case  1: x0 = x + i;         y0 = y + j;         break; // +90 deg
        default: x0 = x + (w - 1) - j; y0 = y + i;         break; // no rotation
      }

      // Skip if drawing off-screen
      if (x0 < 0 || x0 >= cols || y0 < 0 || y0 >= rows) continue;

      // Use unloaded palette for background color based on pixel position (0-255)
      uint16_t _pixel_position;
      if (backgroundGradientHorizontal) {
        // Explicitly handle the last pixel (edge case)
        if (x0 == cols - 1) {
          _pixel_position = 255;
        } else {
          _pixel_position = (x0 * 255) / (cols - 1);  // Horizontal gradient
        }
      } else {
        // Explicitly handle the last pixel (edge case)
        if (y0 == rows - 1) {
          _pixel_position = 255;
        } else {
          _pixel_position = (y0 * 255) / (rows - 1);  // Vertical gradient
        }
      }

      RgbwwColor bgCol  = tkr_anim->GetPaletteColour_WithTemporaryLoad(
        backgroundPaletteId, _pixel_position,
        PALETTE_INDEX__IS_SEGLEN_RANGE,  // Scale across the segment length
        PALETTE_WRAP_SMOOTH,
        PALETTE_MODE__DEFAULT
      ); // Get the background color from the palette

      // bgCol.setBrightness( speed );

      backgroundColor = RgbwwColorU32(bgCol);//.WithBrightness().getU32();

      // Calculate the character color based on the gradient logic
      uint32_t charColor;
      if (solidPerChar) {
        // Use a solid color for the entire character
        charColor = GetPaletteColour_Legacy(chr, PALETTE_INDEX__IS_SEGLEN_RANGE, PALETTE_WRAP_SMOOTH, PALETTE_MODE__DEFAULT);
      } else {
        // Apply a gradient either horizontally or vertically
        if (horizontalGradient) {
          charColor = GetPaletteColour_Legacy(constrain((x0 * 255 / cols), 0, 255), PALETTE_INDEX__IS_SEGLEN_RANGE, PALETTE_WRAP_SMOOTH, PALETTE_MODE__DEFAULT);
        } else {
          charColor = GetPaletteColour_Legacy(constrain((y0 * 255 / rows), 0, 255), PALETTE_INDEX__IS_SEGLEN_RANGE, PALETTE_WRAP_SMOOTH, PALETTE_MODE__DEFAULT);
        }
      }

      // Set either character or background color based on the font bit
      if (((bits >> (j + (8 - w))) & 0x01)) { // bit set, draw character
        setPixelColorXY(x0, y0, charColor);
      } else { // no bit set, draw background
        setPixelColorXY(x0, y0, backgroundColor);
      }
    }
  }
}






/*******************************************************************************************************************************************************************************************************************
 * @description :
 *  Draws an anti-aliased pixel at fractional coordinates `(x,y)` onto the current segment using **Wu’s algorithm**.
 *  Instead of lighting only a single LED, this method blends color `c` into the four nearest integer pixel locations,
 *  weighted by the fractional parts of `x` and `y`. This gives smooth sub-pixel rendering and reduces jagged motion
 *  when moving points or shapes across a low-resolution LED matrix.
 *
 * @method :
 *  - Input `(x,y)` are in 24.8 fixed-point format (upper 8 bits = integer coordinate, lower 8 bits = fractional).
 *  - Extract fractional parts `xx = x & 0xff`, `yy = y & 0xff` and their inverses.
 *  - Compute four bilinear weights using `WU_WEIGHT(a,b)`, corresponding to the contributions to the 4 neighbors:
 *      (floor(x), floor(y)), (ceil(x), floor(y)),
 *      (floor(x), ceil(y)), (ceil(x), ceil(y)).
 *  - For each of the 4 neighbors, read its current CRGB value via `getPixelColorXY()`,
 *    blend in the contribution of `c` scaled by the weight (with saturation via `qadd8`),
 *    then write it back using `setPixelColorXY()`.
 *
 * @arguments :
 *  - x (uint32_t): X coordinate in 24.8 fixed-point format.
 *  - y (uint32_t): Y coordinate in 24.8 fixed-point format.
 *  - c (CRGB)    : The RGB color to draw with.
 *
 * @returns :
 *  - void (draws in place).
 *
 * @dependencies :
 *  - Requires a valid 2D segment with `getPixelColorXY()` and `setPixelColorXY()` implemented.
 *  - Uses FastLED helpers `qadd8()` for saturating addition.
 *
 * @examples :
 *  - `wu_pixel( (5 << 8) + 128, (10 << 8) + 64, CRGB(255,0,0) );`
 *    → Draws a red dot centered at (5.5, 10.25) blended into 4 surrounding pixels.
 *
 * @changed :
 *  - Initial implementation based on Reddit u/sutaburosu, integrated here 08Sep25.
 *
 * @notes :
 *  - This is a **read-modify-write** operation: it blends into the existing buffer rather than overwriting.
 *  - Use when you need smooth sub-pixel drawing, e.g. moving points, plasma effects, or lines.
 *  - Inputs outside the active segment are ignored (`isActive()` guard at start).
 *******************************************************************************************************************************************************************************************************************/

#define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
void mAnimatorLight::Segment::wu_pixel(uint32_t x, uint32_t y, CRGB c) const {      //awesome wu_pixel procedure by reddit u/sutaburosu
  if (!isActive()) return; // not active
  // extract the fractional parts and derive their inverses
  unsigned xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int i = 0; i < 4; i++) {
    int wu_x = (x >> 8) + (i & 1);        // precalculate x
    int wu_y = (y >> 8) + ((i >> 1) & 1); // precalculate y
    CRGB led = getPixelColorXY(wu_x, wu_y);
    CRGB oldLed = led;
    led.r = qadd8(led.r, c.r * wu[i] >> 8);
    led.g = qadd8(led.g, c.g * wu[i] >> 8);
    led.b = qadd8(led.b, c.b * wu[i] >> 8);
    if (led != oldLed) setPixelColorXY(wu_x, wu_y, led); // don't repaint if same color
  }
}
#undef WU_WEIGHT

#endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS


#endif //USE_MODULE_LIGHTS_ANIMATOR
