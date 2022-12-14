#include "mAnimatorLight.h"

/**
 * @brief Temporary file
 * 
 * To clean up the Set/Get of pixel to include MIRROR/REVERSE/MAPPED/MULTIPLIED/GROUPED
 */

#ifdef USE_MODULE_LIGHTS_ANIMATOR

void mAnimatorLight::SetPixelColor_All(RgbcctColor colour){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    SetPixelColor(pixel, colour);
  }
  pCONT_iLight->ShowInterface();
}


/**
 * Alternate SetPixelColor to make WLED conversion easier, but to be phased out later
 */
void mAnimatorLight::SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, bool brightness_needs_applied)
{
  SetPixelColor(indexPixel, RgbColor(red,green,blue), brightness_needs_applied);
}
  
  
void mAnimatorLight::SetPixelColor(uint16_t indexPixel, uint32_t color, bool brightness_needs_applied)//, uint16_t segment_length)
{
  RgbcctColor col = RgbcctColor(0);
  col.red =   (color >> 16 & 0xFF);
  col.green = (color >> 8  & 0xFF);
  col.blue =  (color       & 0xFF);
  col.W1 =    (color >> 24 & 0xFF);

/**
 * @brief For WLED conversion, brightness levels probably need set here
 * 
 */


  SetPixelColor(indexPixel, col, brightness_needs_applied);
}

/**
 * @brief USes active segment to set the correct part (mapped into start/stop)
 * 
 * Rename to SetPixelColourInSegment
 */
void mAnimatorLight::SetPixelColor(uint16_t indexPixel, RgbcctColor color_internal, bool brightness_needs_applied)//, uint16_t segment_length_change_to__segment_index)
{

  // #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  // if(segment_active_index==1)
  // ALOG_INF( PSTR("SetPixelColor=%d"), indexPixel);
  // #endif 

  // #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  // if(indexPixel<10){
    // if(pCONT_)
    // AddLog(LOG_LEVEL_INFO, PSTR("Set virtual RGB(%d)=%d,%d,%d"),indexPixel,color_internal.R, color_internal.G, color_internal.B);
  // }
  // #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS

  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t segIdx = segment_active_index;

  RgbcctColor color_hardware = color_internal;

  HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[segment_active_index].hardware_element_colour_order;
  
  // ALOG_INF( PSTR("RGB(%d)=%d,%d,%d\t %d,%d,%d,%d,%d"),indexPixel,color_internal.R, color_internal.G, color_internal.B, order.r,order.g,order.b,order.w,order.c);

  if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_hardware[order.r] = color_internal.R;  }
  if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_hardware[order.g] = color_internal.G;  }
  if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_hardware[order.b] = color_internal.B;  }
  if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_hardware[order.w] = color_internal.WW; }
  if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_hardware[order.c] = color_internal.WC; }

  /**
   * @brief If WLED effect, then brightness needs set directly (to phase this out later by asserting brightness in animation)
   * */
  if(
    // 
    brightness_needs_applied 
    // ||
  // (IsWithinLimits((uint16_t)EFFECTS_FUNCTION__WLED_STATIC__ID, (uint16_t)_segments[segment_active_index].effect_id, (uint16_t)EFFECTS_FUNCTION__WLED_LENGTH__ID))
  ){
    color_hardware = ApplyBrightnesstoRgbcctColour(color_hardware, pCONT_iLight->getBriRGB_Global());
  }


  uint16_t physical_indexPixel = 0;
  physical_indexPixel = indexPixel;

  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  // ALOG_INF( PSTR("segment_length=%d\t%d"), segment_length, indexPixel);
  ALOG_INF( PSTR("_segments[%d].groupLength()=%d"), segIdx, _segments[segIdx].groupLength());
  #endif

  // get physical pixel address (taking into account start, grouping, spacing [and offset])
  physical_indexPixel = physical_indexPixel * _segments[segIdx].groupLength();
  if (_segments[segIdx].options & REVERSE) { // is segment reversed?
    if (_segments[segIdx].options & MIRROR) { // is segment mirrored?
      physical_indexPixel = (segment_length - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
    } else {
      physical_indexPixel = (segment_length - 1) - physical_indexPixel;
    }
  }
  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  ALOG_INF( PSTR("physical_indexPixelA=%d"), physical_indexPixel);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  physical_indexPixel += _segments[segIdx].pixel_range.start;
  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  ALOG_INF( PSTR("physical_indexPixelB=%d"), physical_indexPixel);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR

  /**
   * @brief Multiplier by me is the same as WLED grouping, so random widths etc should be done here before the loop
   * 
   */
  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  ALOG_INF( PSTR("_segments[{%d}].grouping={%d}"), segIdx, _segments[segIdx].grouping);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR

  // set all the pixels in the group
  for (uint16_t j = 0; j < _segments[segIdx].grouping; j++) 
  {
  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("j=%d"), j);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR

    uint16_t indexSet = physical_indexPixel + ((_segments[segIdx].options & REVERSE) ? -j : j);
    
  // #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  // if(segment_active_index==1)
    // ALOG_INF( PSTR("indexSet{%d},start{%d},stop{%d}"),indexSet, _segments[segIdx].pixel_range.start, _segments[segIdx].pixel_range.stop);
  // #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR

    if (
      indexSet >= _segments[segIdx].pixel_range.start && 
      indexSet <  _segments[segIdx].pixel_range.stop
    ){

      if (_segments[segIdx].options & MIRROR) 
      { //set the corresponding mirrored pixel
        uint16_t indexMir = _segments[segIdx].pixel_range.stop - indexSet + _segments[segIdx].pixel_range.start - 1;          
        indexMir += _segments[segIdx].offset; // offset/phase

        if (indexMir >= _segments[segIdx].pixel_range.stop) indexMir -= segment_length;
        // if (indexMir < customMappingSize) indexMir = customMappingTable[indexMir];

        // busses.setPixelColor(indexMir, col);
        pCONT_iLight->SetPixelColourHardwareInterface(color_hardware, indexMir);

      }
      indexSet += _segments[segIdx].offset; // offset/phase

      if (indexSet >= _segments[segIdx].pixel_range.stop) indexSet -= segment_length;
      // if (indexSet < customMappingSize) indexSet = customMappingTable[indexSet];

  // #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  // if(segment_active_index==1)
      // ALOG_INF( PSTR("indexSet=%d\t R%dG%dB%d"), indexSet, color_hardware.R, color_hardware.G, color_hardware.B);
  // #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR

        // Serial.println(indexSet);
      // busses.setPixelColor(indexSet, col);
  //     if(indexPixel<10){
  //   // if(pCONT_)
  //   AddLog(LOG_LEVEL_INFO, PSTR("Set iRGB(%d|%d)=%d,%d,%d"),indexPixel,indexSet, color_hardware.R, color_hardware.G, color_hardware.B);
  // }
      pCONT_iLight->SetPixelColourHardwareInterface(color_hardware, indexSet);
    }
    else
    {
      // DEBUG_LINE_HERE;
    }
  }

}


// #ifndef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
RgbcctColor mAnimatorLight::GetPixelColor(uint16_t indexPixel)
{
  
// DEBUG_LINE_HERE;

  /**
   * @brief Construct a new if object Assume in the future this must be checked elsewhere so to not slow it down on EACH pixel
   * */
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_DBM( PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }
  // if(indexPixel > STRIP_SIZE_MAX){  // Temp fix for WLED animations
  //   ALOG_ERR( PSTR("indexPixel = %d, return 0"), indexPixel );
  //   return RgbcctColor(0);
  // }

  uint16_t physical_indexPixel = 0;  
  physical_indexPixel = indexPixel;

  // get physical pixel
  physical_indexPixel = physical_indexPixel * _segments[segment_active_index].groupLength();  // should this be happening???

  if (IS_REVERSE) {
    if (IS_MIRROR) physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
    else           physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) - physical_indexPixel;
  }

  physical_indexPixel += _segments[segment_active_index].pixel_range.start;

/**
 * @brief If segment has any length, execute
 * 
 */
  if (_segments[segment_active_index].length_wled()) {
    /* offset/phase */
    physical_indexPixel += _segments[segment_active_index].offset;

    if(physical_indexPixel >= _segments[segment_active_index].pixel_range.stop)
    {
      physical_indexPixel -= _segments[segment_active_index].length_wled();
    }


  }

  
// if (i < customMappingSize) i = customMappingTable[i];

// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// // if (i >= _length) return 0; //length here is not the same as the segment length?
// // if (i >= _segments[segment_active_index].length_wled()) return RgbcctColor(255,0,0,0,0); //length here is not the same as the segment length?
// #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022

// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// //stop overflow
// if(physical_indexPixel >= STRIP_SIZE_MAX){ physical_indexPixel = 0; }
// #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
  
  // return busses.getPixelColor(i);
  RgbcctColor color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(physical_indexPixel);
  RgbcctColor color_internal = color_hardware; // To catch white element if present

  HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[segment_active_index].hardware_element_colour_order;

  if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.r] = color_hardware.R;  }
  if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.g] = color_hardware.G;  }
  if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.b] = color_hardware.B;  }
  if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.w] = color_hardware.WW; }
  if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.c] = color_hardware.WC; }



  // Serial.printf("Bi=%d\to=%d\n\r",indexPixel, physical_indexPixel);

  // if(indexPixel<10){
    // AddLog(LOG_LEVEL_INFO, PSTR("RGB{s%d||v%d|p%d}=internal{%d,%d,%d}"),segment_active_index,indexPixel,physical_indexPixel, color_internal.R, color_internal.G, color_internal.B);
  // }

  return color_internal;

}
// #endif // NOT ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022







// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// RgbcctColor mAnimatorLight::GetPixelColor(uint16_t indexPixel)
// {
  
// // DEBUG_LINE_HERE;

//   /**
//    * @brief Construct a new if object Assume in the future this must be checked elsewhere so to not slow it down on EACH pixel
//    * */
//   if(stripbus == nullptr){    
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     ALOG_DBM( PSTR(D_LOG_NEO "stripbus == nullptr"));
//     #endif
//   }

//   if(indexPixel > STRIP_SIZE_MAX){  // Temp fix for WLED animations
//     ALOG_ERR( PSTR("indexPixel = %d, return 0"), indexPixel );

//     // indexPixel = indexPixel % STRIP_SIZE_MAX;// SEGMENT.virtualLength();

//     return RgbcctColor(0);
//   }


// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022

//   uint32_t physical_indexPixel = 0;
//   physical_indexPixel = indexPixel;

//   uint32_t groupLength = _segments[segment_active_index].groupLength();
//   uint32_t wled_length32 = _segments[segment_active_index].length_wled();

//   // get physical pixel
//   physical_indexPixel = physical_indexPixel * groupLength;  // should this be happening???

//   Serial.printf("Ai=%d\to=%d\n\r",indexPixel, physical_indexPixel);

// #else
// uint16_t physical_indexPixel = 0;  
// physical_indexPixel = indexPixel;
// #endif





// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022

//   if (IS_REVERSE) {
//     if (IS_MIRROR) physical_indexPixel = (wled_length32 - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
//     else           physical_indexPixel = (wled_length32 - 1) - physical_indexPixel;
//   }

// #else
//   if (IS_REVERSE) {
//     if (IS_MIRROR) physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
//     else           physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) - physical_indexPixel;
//   }
// #endif




//   physical_indexPixel += _segments[segment_active_index].pixel_range.start;

// /**
//  * @brief If segment has any length, execute
//  * 
//  */
//   if (_segments[segment_active_index].length_wled()) {
//     /* offset/phase */
//     physical_indexPixel += _segments[segment_active_index].offset;

//     if(physical_indexPixel >= _segments[segment_active_index].pixel_range.stop)
//     {
//       physical_indexPixel -= _segments[segment_active_index].length_wled();
//     }


//   }

// // if(physical_indexPixel>100)//STRIP_SIZE_MAX)
// // // {  
// //   AddLog(LOG_LEVEL_INFO, PSTR("HERE {%d|%d}"),  _segments[0].pixel_range.start, _segments[0].pixel_range.stop);
//   // AddLog(LOG_LEVEL_INFO, PSTR("STRIP_SIZE_MAX RGB{%d|%d}"),indexPixel,physical_indexPixel);
//   // if(physical_indexPixel>300)
//   // {
// // DEBUG_LINE_HERE_PAUSE;
// // Serial.printf("CRASH=%s\n\r", 0);
// //     float dmp = 2.0f/0.0f; // crash it!
//   // }
// // } 
  
// // if (i < customMappingSize) i = customMappingTable[i];

// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// // if (i >= _length) return 0; //length here is not the same as the segment length?
// // if (i >= _segments[segment_active_index].length_wled()) return RgbcctColor(255,0,0,0,0); //length here is not the same as the segment length?
// #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022

// #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// //stop overflow
// if(physical_indexPixel >= STRIP_SIZE_MAX){ physical_indexPixel = 0; }
// #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
  
//   // return busses.getPixelColor(i);
//   RgbcctColor color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(physical_indexPixel);
//   RgbcctColor color_internal = color_hardware; // To catch white element if present

//   HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[segment_active_index].hardware_element_colour_order;

//   if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.r] = color_hardware.R;  }
//   if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.g] = color_hardware.G;  }
//   if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.b] = color_hardware.B;  }
//   if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.w] = color_hardware.WW; }
//   if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.c] = color_hardware.WC; }



//   // Serial.printf("Bi=%d\to=%d\n\r",indexPixel, physical_indexPixel);

//   // if(indexPixel<10){
//     // AddLog(LOG_LEVEL_INFO, PSTR("RGB{s%d||%d|%d}={%d,%d,%d}"),segment_active_index,indexPixel,physical_indexPixel,color_hardware.R, color_hardware.G, color_hardware.B);
//   // }

//   return color_internal;

// }

// #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022


// #ifdef ENABLE_DEVFEATURE_GETPIXELCOLOUR_DIRECTLY_DEC2022



// RgbcctColor mAnimatorLight::GetPixelColor_FromBus(uint16_t indexPixel)
// {
  
// // DEBUG_LINE_HERE;

//   /**
//    * @brief Construct a new if object Assume in the future this must be checked elsewhere so to not slow it down on EACH pixel
//    * */
//   if(stripbus == nullptr){    
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     ALOG_DBM( PSTR(D_LOG_NEO "stripbus == nullptr"));
//     #endif
//   }
//   if(indexPixel > STRIP_SIZE_MAX){  // Temp fix for WLED animations
//     ALOG_ERR( PSTR("indexPixel = %d, return 0"), indexPixel );
//     return RgbcctColor(0);
//   }

//   uint16_t physical_indexPixel = 0;  
//   physical_indexPixel = indexPixel;

// //   if (IS_REVERSE) {
// //     if (IS_MIRROR) physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
// //     else           physical_indexPixel = (_segments[segment_active_index].length_wled() - 1) - physical_indexPixel;
// //   }

// //   physical_indexPixel += _segments[segment_active_index].pixel_range.start;

// // /**
// //  * @brief If segment has any length, execute
// //  * 
// //  */
// //   if (_segments[segment_active_index].length_wled()) {
// //     /* offset/phase */
// //     physical_indexPixel += _segments[segment_active_index].offset;

// //     if(physical_indexPixel >= _segments[segment_active_index].pixel_range.stop)
// //     {
// //       physical_indexPixel -= _segments[segment_active_index].length_wled();
// //     }


// //   }

//   // Serial.printf("Ai=%d\to=%d\n\r",indexPixel, physical_indexPixel);
  
// // if (i < customMappingSize) i = customMappingTable[i];

// // #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// // // if (i >= _length) return 0; //length here is not the same as the segment length?
// // // if (i >= _segments[segment_active_index].length_wled()) return RgbcctColor(255,0,0,0,0); //length here is not the same as the segment length?
// // #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022

// // #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
// // //stop overflow
// // if(physical_indexPixel >= STRIP_SIZE_MAX){ physical_indexPixel = 0; }
// // #endif // ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
  
//   // return busses.getPixelColor(i);
//   RgbcctColor color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(physical_indexPixel);
//   RgbcctColor color_internal = color_hardware; // To catch white element if present

//   HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[segment_active_index].hardware_element_colour_order;

//   if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.r] = color_hardware.R;  }
//   if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.g] = color_hardware.G;  }
//   if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.b] = color_hardware.B;  }
//   if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.w] = color_hardware.WW; }
//   if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){ color_internal[order.c] = color_hardware.WC; }



//   // Serial.printf("Bi=%d\to=%d\n\r",indexPixel, physical_indexPixel);

//   // if(indexPixel<10){
//     // AddLog(LOG_LEVEL_INFO, PSTR("RGB{s%d||%d|%d}={%d,%d,%d}"),segment_active_index,indexPixel,physical_indexPixel,color_hardware.R, color_hardware.G, color_hardware.B);
//   // }

//   return color_internal;

// }


// #endif



#endif //USE_MODULE_LIGHTS_ANIMATOR




