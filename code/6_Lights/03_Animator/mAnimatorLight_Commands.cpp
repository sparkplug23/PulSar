#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT D_TOPIC "Checking all commands mAnimatorLight::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];
  uint16_t isserviced_start_count = data_buffer.isserviced;


  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  }


  /**
   * @brief Add option to allow mqtt command to output all feedback to serial, response to mqtt, or have no output just parse it (for speed!)
   * 
   */

  uint8_t segments_found = 0;

  // uint8_t segment_index = 0;

  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer]){ 

      ALOG_INF(PSTR("Seg: \"%s\" with %d Slots Active"), buffer, pCONT_lAni->segments.size());

      /**
       * @brief Add check here that only sets the segment if it is already permitted
       * 
       */
    
      if(segment_i > pCONT_lAni->segments.size()-1){ 
             
        ALOG_HGL(PSTR("Creating new segment %d|%d"),segment_i,pCONT_lAni->segments.size());

      DEBUG_LINE_HERE;
        Segment_AppendNew(0, 0, segment_i+1);
        
      DEBUG_LINE_HERE;
        // Serial.println(SEGMENT_I(segment_i).hardware_element_colour_order.data);

        if(pCONT_lAni->segments.size()!=0)
        {
          Serial.println(pCONT_lAni->segments.size());Serial.flush();
        }

      }

      // delay(1000);

      DEBUG_LINE_HERE;

      data_buffer.isserviced += subparse_JSONCommand(jtok.getObject(), segment_i);

      segments_found++;

      // If segment commands updated, some effects may need reset
      SEGMENT_I(segment_i).call = 0; 

    }
  }

      // Serial.println(SEGMENT_I(0).hardware_element_colour_order.data);
  /**
   * @brief If no segments have been directly set, then assume default of Segment0
   **/
  if(segments_found == 0)
  {
    data_buffer.isserviced += subparse_JSONCommand(obj); // Legacy commands
  }
  
  if(data_buffer.isserviced){ //update string, move to shared place
   //add flag on this, so its not always forced
   // SetRefreshLEDs(); // implement in 1 second 
  }

  #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
  parsesub_Notifications(obj);
  #endif // ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION

  // Serial.println(SEGMENT_I(0).hardware_element_colour_order.data);

}




/**
 * @brief Allows new segment controls, by calling this function for each segment.
 *        Calling it without any set index, will default to segment of 0 index and turn the other segments off 
 * @param obj 
 */
uint8_t mAnimatorLight::subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

  /**
   * @brief Critical this is always parsed first, as many other commands rely on this value
   * 
   */
  if(segment_index == 255)
  {
    // ALOG_COM( PSTR("Segment Index assumed \"0\"") );
    segment_index = 0;

    // ALOG_INF(PSTR("Disabled assumed single palette to reset others"));

    /**
     * @brief Add flag to make this optional later 
     * Assumes by not providing a segment, only 1 segment is required so disabling the test
     */
    // for(uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
    // {
    //   _segments[i].pixel_range.start = 0;
    //   _segments[i].pixel_range.stop  = 0;
    //   // segments[i]
    //   // _segments[i].pixel_range.start = 0;
    //   // _segments[i].pixel_range.stop  = 0;
    // }
    // SEGMENT_I(0).pixel_range.start = 0;

    // SEGMENT_I(0).pixel_range.stop  = STRIP_SIZE_MAX;
    // SEGMENT_I(0).pixel_range.stop  = STRIP_SIZE_MAX;

    // purgeSegments(true); // reduce to single segment
    ALOG_DBM(PSTR("purgeSegments(true); // reduce to single segment REMOVED THIS, NEEDS BETETR HOME"));
   
  }

  /**
   * @brief 
   * 
   */

  // DEBUG_LINE_HERE;

  /**
   * @brief Important this remains above other commands, as some others rely on states being set (eg. Rgbcct user palettes)
   * 
   */
  if(jtok = obj[PM_JSON_COLOUR_PALETTE])
  {

    // ALOG_INF( PSTR("PM_JSON_COLOUR_PALETTE") );

    if(jtok.isStr()){
      /**
       * @brief First check if it is simple commands
       **/
      if(strcmp(jtok.getStr(),"+")==0){
        SEGMENT_I(segment_index).palette.id++;
      }else
      if(strcmp(jtok.getStr(),"-")==0){
        SEGMENT_I(segment_index).palette.id--;
      }else
      if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
        ALOG_DBM(PSTR("tmp_id=%d"),tmp_id);
        CommandSet_PaletteID(tmp_id, segment_index);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_PaletteID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette.id, buffer, sizeof(buffer)) );
  }

  
  if(jtok = obj[D_JSON_PIXELRANGE])
  { 
    if(jtok.isArray())
    {
      JsonParserArray arrobj = jtok;
      SEGMENT_I(segment_index).pixel_range.start = arrobj[0].getInt();
      SEGMENT_I(segment_index).pixel_range.stop  = arrobj[1].getInt();
      
      if(SEGMENT_I(segment_index).pixel_range.stop > STRIP_SIZE_MAX)
      {
        ALOG_ERR( PSTR("SEGMENT_I(segment_index).pixel_range.stop exceeds max %d %d"),SEGMENT_I(segment_index).pixel_range.stop, STRIP_SIZE_MAX);
        SEGMENT_I(segment_index).pixel_range.stop = STRIP_SIZE_MAX;
      }

      ALOG_COM( PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"), SEGMENT_I(segment_index).pixel_range.start, SEGMENT_I(segment_index).pixel_range.stop );
      data_buffer.isserviced++;
    }else{
      ErrorMessage_P(ERROR_MESSAGE_TYPE_INVALID_FORMAT, PM_JSON_PIXELRANGE);
    }
  }


  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_FUNCTION])
  { // Might move to "Mode" instead of function later
    if(jtok.isStr())
    {
      if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0)
      {
        CommandSet_Flasher_FunctionID(tmp_id, segment_index);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum())
    {
      CommandSet_Flasher_FunctionID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
  }
  

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_COLOUR_REFRESH_RATE])
  // { 
  //   CommandSet_Flasher_UpdateColourRegion_RefreshSecs(jtok.getInt(), segment_index);
  //   ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_EFFECTS,D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
  // }
  
  
  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_INTENSITY])
  { 
    CommandSet_Effect_Intensity(jtok.getInt(), segment_index);
    ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_INTENSITY)), jtok.getInt() );
  }
  

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_SPEED])
  { 
    CommandSet_Effect_Speed(jtok.getInt(), segment_index);
    ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_SPEED)), jtok.getInt() );
  }
  

  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Reverse"])
  { 
   SEGMENT_I(segment_index).setOption(SEG_OPTION_REVERSED, jtok.getInt());  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Mirror"])
  { 
   SEGMENT_I(segment_index).setOption(SEG_OPTION_MIRROR, jtok.getInt());  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Grouping"])
  { 


    ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
   SEGMENT_I(segment_index).grouping = jtok.getInt();  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Spacing"])
  { 


    ALOG_INF( PSTR("Spacing %d %d"), jtok.getInt(), segment_index );
   SEGMENT_I(segment_index).spacing = jtok.getInt();  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Opacity"])
  { 


    ALOG_INF( PSTR("Opacity %d %d"), jtok.getInt(), segment_index );
   SEGMENT_I(segment_index).opacity = jtok.getInt();  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


  if(jtok = obj[PM_JSON_EFFECTS].getObject()["SegBrightness"])
  { 
    SEGMENT_I(segment_index).seg_brightness = jtok.getInt();  
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K("SegBrightness")), SEGMENT_I(segment_index).seg_brightness);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


  
/**
 * @brief Construct a new if object
 * Reseting segment environments must happen above here where I then fill the options
 * 
 */
  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Option0"])
  { 


    // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
    SEGMENT_I(segment_index).aux0 = jtok.getInt();  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  if(jtok = obj[PM_JSON_EFFECTS].getObject()["Option1"])
  { 


    // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
    SEGMENT_I(segment_index).aux1 = jtok.getInt();  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


  if(jtok = obj[PM_JSON_ANIMATIONMODE]){
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
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj["PixelRange"]){ 
    if(jtok.isArray()){
      uint8_t array[2];
      uint8_t arrlen = 0;
      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) {
        if(arrlen > 2){ break; }

        

        switch(arrlen)
        {
          case 0:SEGMENT_I(segment_index).pixel_range.start = v.getInt(); break;
          case 1:SEGMENT_I(segment_index).pixel_range.stop  = v.getInt(); break;
        }

        array[arrlen++] = v.getInt();
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "PixelRange" " [i%d:v%d]"),arrlen-1,array[arrlen-1]);
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "PixelRange Segment[%d] = %d -> %d"),
          segment_index,
         SEGMENT_I(segment_index).pixel_range.start,
         SEGMENT_I(segment_index).pixel_range.stop
        );
        #endif



        // #endif// ENABLE_LOG_LEVEL_DEBUG          
      }

      if(SEGMENT_I(segment_index).pixel_range.stop > STRIP_SIZE_MAX+1)
      {
    #ifdef ENABLE_LOG_LEVEL_ERROR
        AddLog(LOG_LEVEL_ERROR, PSTR("SEGMENT_I(segment_index).pixel_range.stop exceeds max %d %d"),SEGMENT_I(segment_index).pixel_range.stop, STRIP_SIZE_MAX);
    #endif //ef ENABLE_LOG_LEVEL_INFO
       SEGMENT_I(segment_index).pixel_range.stop = STRIP_SIZE_MAX+1;
      }


      
      data_buffer.isserviced++;
    }
  }
  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_DIRECTION])
  // { 
  //   CommandSet_Flasher_Flags_Movement_Direction(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  

  // I want a segment_brightness value, not just global, if set, then it is used. Or, it is also scaled, and thus if set to max then global only has control, or else both can be scalers
  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[BRIGHTNESS]){  
  //   SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MIN]){ 
  //   CommandSet_Flasher_Alternate_Brightness_Min(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MAX]){ 
  //   CommandSet_Flasher_Alternate_Brightness_Max(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_AMOUNT]){ 
  //   CommandSet_Flasher_Alternate_RandomAmountPercentage(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_AGED_COLOURING]){
  //   SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }


  if(jtok = obj["paletteBlend"]){
    paletteBlend = jtok.getInt();

  }

   

  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(jtok = obj[PM_JSON_RGB_CLOCK].getObject()[PM_JSON_MANUAL_NUMBER]){
    lcd_display_show_number = jtok.getInt();
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MANUAL_NUMBER)), lcd_display_show_number);
  }
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(jtok = obj[PM_JSON_RGB_CLOCK].getObject()["ManualString"]){
    strcpy(lcd_display_show_string, jtok.getStr());
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_JSON_COMMAND_SVALUE_K("ManualString")), lcd_display_show_string);
  }
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


// Hue
// Sat
// ColourHeatMap:[style, cold_point, now_temp, hot_point]

  if(jtok = obj["ColourHeatMap_Palette"])
  {

    uint8_t style_index  = 0;
    if(jtok_sub = jtok.getObject()["Method"]) //different heatmap conversion method
    {
      style_index = jtok_sub.getInt();
      ALOG_COM(PSTR(D_JSON_COMMAND_NVALUE_K("Method")), style_index);
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

  
  






  
  // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  //   if(jtok = obj[PM_JSON_BRIGHTNESS_MIN]){ 
  //     CommandSet_Brightness_Min(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }

  //   if(jtok = obj[PM_JSON_BRIGHTNESS_MAX]){ 
  //     CommandSet_Brightness_Max(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }



  //   if(jtok = obj[PM_JSON_PALETTE_GENERATION].getObject()[PM_JSON_RANDOMISE_BRIGHTNESS_MODE]){
  //     CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }
    



  // if(jtok = obj[PM_RGB_DATA_STREAM]){  // PM_RGB_DATA_STREAM

  // // maybe to be done with a topic directly so its faster, straight buffer into setpixel

  //   // uint8_t colour_array[5];
  //   // memset(colour_array,0,sizeof(colour_array));
  //   // uint8_t jsonpair_count = jtok.size();

  //   // for(int index = 0; index < jsonpair_count; index++){
  //   //   jtok.nextOne(); //skip start of object
  //   //   Serial.println(jtok.getInt());
  //   //   colour_array[index] = jtok.getInt();
  //   // }
    
  //   // CommandSet_ActiveSolidPalette_Raw(colour_array);
  //   // data_buffer.isserviced++;

  //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   // snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),colour_array[0],colour_array[1],colour_array[2],colour_array[3],colour_array[4]);
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  //   // #endif // ENABLE_LOG_LEVEL_DEBUG
  // }



// #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL


//   if(jtok = obj["Strip"].getObject()["ClearTo"]){ 
//     if(jtok.isArray()){
//       RgbcctColor colour = RgbcctColor(0);

//       uint8_t index = 0;
//       JsonParserArray array = obj["Strip"].getObject()["ClearTo"];
//       for(auto v : array) {
//         switch(index){
//           case 0: colour.R = v.getInt();
//           case 1: colour.G = v.getInt();
//           case 2: colour.B = v.getInt();
//           case 3: colour.WC = v.getInt();
//           case 4: colour.WW = v.getInt();
//         }
//         int val = v.getInt();
//         // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
//         // editable_mapped_array_data_array[index++] = val;
//         index++;
//         // SEGMENT_I(segment_index).mode_id =  ANIMATION_MODE_MANUAL_SETPIXEL_ID;
//         // SetPixelColor(val, colour);
//         // animation_colours[val].DesiredColour = colour;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_PIXEL D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
//     #endif// ENABLE_LOG_LEVEL_INFO          
//       }
//       SetPixelColor_All(colour);
//     }

//   }

//   if(jtok = obj["Strip"].getObject()[PM_JSON_MANUAL_SETPIXEL_TO_SCENE]){ 
//     if(jtok.isArray()){
//       //testing, froce it
//       RgbcctColor colour = RgbcctColor(255,255,255);
//       uint8_t index = 0;
//       JsonParserArray array = obj["Strip"].getObject()[PM_JSON_MANUAL_SETPIXEL_TO_SCENE];//jtok;//obj[PM_JSON_MANUAL_SETPIXEL_TO_SCENE];
//       for(auto v : array) {
//         int val = v.getInt();
//         // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
//         // editable_mapped_array_data_array[index++] = val;
//         index++;

//         //Count active pixels
//         uint16_t pixels_on = 0;
//         for(uint16_t i=0;i<settings.light_size_count;i++){ 
//           if(RgbColorto32bit(GetPixelColor(i))){
//             pixels_on++;
//           }      
//         }

//         if(pixels_on <= 5){
//           colour = RgbcctColor(255,0,0);
//         }else
//         if(pixels_on <= 10){
//           colour = RgbcctColor(0,255,0);
//         }else
//         if(pixels_on <= 15){
//           colour = RgbcctColor(0,0,255);
//         }else
//         if(pixels_on <= 20){
//           colour = RgbcctColor(255,0,255);
//         }else
//         if(pixels_on <= 25){
//           colour = RgbcctColor(255,50,0);
//         }else
//         if(pixels_on <= 30){
//           colour = RgbcctColor(0,255,55);
//         }else
//         if(pixels_on <= 35){
//           colour = RgbcctColor(0,255,255);
//         }else{
//           colour = RgbcctColor(255,255,255);
//         }

//         SEGMENT_I(0).mode_id =  ANIMATION_MODE_MANUAL_SETPIXEL_ID;

//         SetPixelColor(val, colour, 0);

//         //animation_colours[val].DesiredColour = colour;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_PIXEL D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
//     #endif// ENABLE_LOG_LEVEL_INFO          
//       }
//       // pixel_group.mapped_array_data.length = index;

//       ShowInterface();

//       data_buffer.isserviced++;
//     }
//   }



// #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL




  // if(jtok = obj[PM_JSON_PIXELSGROUPED]){
  //  SEGMENT_I(segment_index).pixelgrouped = obj[CFLASH(PM_JSON_PIXELSGROUPED];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,SEGMENT_I(segment_index).pixelgrouped);
  //   #endif
  // }

  //override commands that run for one animation cycle then are cleared to 0
  // if(!obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS].isNull()){
  //   animation_override.fRefreshAllPixels = obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,animation_override.fRefreshAllPixels);
  //   #endif
  // }



  if(jtok = obj["Override"].getObject()["Animation"].getObject()["TimeMs"])
  {
  
    SEGMENT_I(segment_index).single_animation_override.time_ms = jtok.getInt();

  }


  if(jtok = obj["Override"].getObject()["Animation_Off"].getObject()["TimeMs"])
  {
  
    SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms = jtok.getInt();
    ALOG_INF(PSTR("single_animation_override_turning_off=%d"),SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms);

  }


  




// CHANGE TO USE RELAYS INSTEAD
  // if(!obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)];    
  //   uint8_t relay_state_new = pCONT_sup->GetStateNumber(onoff);
  //   uint8_t relay_state_current = 0;
  //   if(relay_state_new==STATE_NUMBER_TOGGLE_ID){
  //     // SEGMENT_I(segment_index).flags.fEnable_Animation ^= 1;
  //   }else{
  //     // SEGMENT_I(segment_index).flags.fEnable_Animation = state;
  //   }
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE),D_JSON_EXTERNAL_POWER_ONOFF,relay_state_new);
  //   #endif
  //   data_buffer.isserviced++;
  // }


  // if(!obj[CFLASH(PM_JSON_ONOFF].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_ONOFF];
  //   if(strstr(onoff,"ON")){ 
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIXEL  "\"onoff\"=\"ON\""));
  //     #endif
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "MODE_TURN_ON_ID"));
  //     #endif
  //     // Add this as "SAVE" state then "LOAD" state
  //     memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state

  //     SetAnimationProfile(ANIMATION_PROFILE_TURN_ON_ID);
  //     light_power_state = true;

  //     //SEGMENT_I(segment_index).mode_id = MODE_TURN_ON_ID;
  //     data_buffer.isserviced++;
  //   }else if(strstr(onoff,"OFF")){
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIXEL  "\"onoff\"=\"OFF\""));
  //     #endif
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "MODE_TURN_OFF_ID"));
  //     #endif
  //     memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
  //     SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
  //     light_power_state = false;

  //     //SEGMENT_I(segment_index).mode_id = MODE_TURN_OFF_ID;
  //     data_buffer.isserviced++;
  //   }else{
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIXEL D_PARSING_NOMATCH));
  //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   }
  // }

  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME]){ // default to secs
    CommandSet_Animation_Transition_Time_Ms(jtok.getInt()*1000, segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)), SEGMENT_I(segment_index).transition.time_ms);  
    #endif
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME_MS]){
    CommandSet_Animation_Transition_Time_Ms(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)), SEGMENT_I(segment_index).transition.time_ms);  
    #endif
  }
  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE]){ // default to secs
    CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE)), SEGMENT_I(segment_index).transition.rate_ms);  
    #endif
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE_MS]){
    CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)), SEGMENT_I(segment_index).transition.rate_ms);  
    #endif
  }
  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_NUMBER]){
    CommandSet_LightsCountToUpdateAsNumber(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)),SEGMENT_I(segment_index).transition.pixels_to_update_as_percentage.val);
    #endif
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_PERCENTAGE]){ 
    CommandSet_LightsCountToUpdateAsPercentage(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)),SEGMENT_I(segment_index).transition.pixels_to_update_as_number);
    #endif
  }
  

  /******************************
   * Rgbcct controller that will be added into segments (phasing out of interlight?)
   * 
   * *******************************/

  /**
   * @brief 
   * 
   * New method will assume the first Rgbcct_colors is the primary scene, since it is not likely they will be used more than once
   * 
   * However, for some effects that may use multiple (possible large amount now it is vector)
   * I need a way to encapsulate RgbcctColor(blank), Rgbcct1, 2 etc
   * Use for loop
   * 
   * If(rgbcct exactly), loop once
   * 
   * if(rgbcct1/2/3/4/5) then loop for each found. I will therefore have to loop until none are found and hence must be increasing numbers only
   * 
   */

  // if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
  //   CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // } 

  // Wled.color aka segcolour will be merged into this method to allow full colour options

  uint8_t colour_index = 0;

  for(uint8_t col_i=0;col_i<6;col_i++)
  {
    JsonParserObject seg_obj = 0;

    if(col_i==0)
    { // No index so default to 0
      snprintf(buffer, sizeof(buffer), "SegColour");
      colour_index = 0;
    }
    else
    { 
      snprintf(buffer, sizeof(buffer), "SegColour%d", col_i);
      colour_index = col_i;
    }

    if(seg_obj = obj[buffer].getObject())
    {

      if(jtok = seg_obj[PM_JSON_HUE]){ // Assume range 0-359
        CommandSet_SegColour_RgbcctColour_Hue_360(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getHue360());
      }
      
      if(jtok = seg_obj[PM_JSON_SAT]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_Sat_255(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++; 
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getSat255());
      }else
      if(jtok = seg_obj[PM_JSON_SAT_255]){ // alternate full range 0-255
        CommandSet_SegColour_RgbcctColour_Sat_255(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_SAT255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getSat255());
      }


      if(jtok = seg_obj[PM_JSON_CCT_PERCENTAGE]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(mapvalue(jtok.getInt(), 0,100, _ct_min_range, _ct_max_range), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_PERCENTAGE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getCCT());
      }else
      if(jtok = seg_obj[PM_JSON_CCT_TEMP]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_PERCENTAGE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getCCT());
      }


      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
      }else
      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_RGB_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
      }


      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
      }else
      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_CCT_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
      }


      if(jtok = seg_obj[D_JSON_SUBTYPE]){
        CommandSet_SegColour_RgbcctColour_LightSubType(jtok.getInt(), colour_index, segment_index);
        ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SUBTYPE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getSubType());
        data_buffer.isserviced++;
      }
      

      /**
       * @brief Kinda duplicate of what is stored inside rgbcctcontroller, but both must exist somehow, needs perfecting
       * OR, maybe it does not, as "colourtype" is part of bus? or maybe all 3 is needed?? yes, likely
       * 
       */
      if(jtok = seg_obj[PM_JSON_COLOUR_TYPE]){
        // if(jtok.isStr()){
        //   if((tmp_id=mPaletteI->GetColourTypeIDbyName(jtok.getStr()))>=0){
        //     CommandSet_ColourTypeID(tmp_id, segment_index);
        //     data_buffer.isserviced++;
        //   }
        // }else
        if(jtok.isNum()){
          CommandSet_ColourTypeID(jtok.getInt(), segment_index);
          data_buffer.isserviced++;
        }
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
        #endif // ENABLE_LOG_LEVEL_DEBUG
      }
      



      if(jtok = seg_obj["Manual"]){ // Assume range 0-359

        uint8_t colour_array[5];
        memset(colour_array,0,sizeof(colour_array));
        uint8_t jsonpair_count = jtok.size();

        for(int index = 0; index < jsonpair_count; index++){
          jtok.nextOne(); //skip start of object
          Serial.println(jtok.getInt());
          colour_array[index] = jtok.getInt();
        }

        CommandSet_SegColour_RgbcctColour_Manual(colour_array, jsonpair_count, colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getHue360());
      }


    }
  
  }


// Flip these so default is 0 to 100%, and the other is range controlled

//   if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
//     CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   } 


// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
//     CommandSet_Global_BrtCCT_255(mapvalue(jtok.getInt(), 0,100, 0,255));
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), getBriCT());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // alternate full range 0-255
//     CommandSet_Global_BrtCCT_255(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), getBriCT());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }


// // Flip these so default is 0 to 100%, and the other is range controlled
//   if(jtok = obj[PM_JSON_CCT_PERCENTAGE]){ // Assume range 0-100
//     CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(jtok.getInt(), 0,100, _ct_min_range,_ct_max_range));
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_CCT_TEMP]){ // Assume range 0-100
//     CommandSet_ActiveSolidPalette_ColourTemp(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }

//   if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
//     CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   } 



//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE]){ // default to secs
//     CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000);
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE)),SEGMENT_I(segment_index).transition.rate_ms);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE_MS]){
//     CommandSet_Animation_Transition_Rate_Ms(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),SEGMENT_I(segment_index).transition.rate_ms);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
  
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_NUMBER]){
//     CommandSet_LightsCountToUpdateAsNumber(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)),SEGMENT_I(segment_index).transition.pixels_to_update_as_percentage.val);
//     #endif
//   }else
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_PERCENTAGE]){ 
//     CommandSet_LightsCountToUpdateAsPercentage(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)),SEGMENT_I(segment_index).transition.pixels_to_update_as_number);
//     #endif
//   }
  
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_ORDER]){
//     if(jtok.isStr()){
//       if((tmp_id=GetTransitionOrderIDbyName(jtok.getStr()))>=0){
//         CommandSet_TransitionOrderID(tmp_id);
//         data_buffer.isserviced++;
//       }
//     }else
//     if(jtok.isNum()){
//       CommandSet_TransitionOrderID(jtok.getInt());
//       data_buffer.isserviced++;
//     }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_TRANSITION,D_JSON_ORDER)), GetTransitionOrderName(buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


/**
 * @brief 
 * All PRESET options
 * 
 */

  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  if(jtok = obj["Presets"].getObject()["SetPresetManual"]){  
    // void mAnimatorLight::
    // CommandSet_Mixer_RunningID(jtok.getInt());
    uint8_t val = jtok.getInt();
    mixer.running_id = val;
    LoadMixerGroupByID(val);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj["Presets"].getObject()["SetPresetManual_Testing"]){  
    // void mAnimatorLight::
    // CommandSet_Mixer_RunningID(jtok.getInt());
    uint8_t val = jtok.getInt();
    mixer.running_id = val;
    LoadPreset_ManualTesting_ByID(val);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj["Presets"].getObject()["SetPreset_ManualUserCustom"]){  
    // void mAnimatorLight::
    // CommandSet_Mixer_RunningID(jtok.getInt());
    uint8_t val = jtok.getInt();
    mixer.running_id = val;
    LoadPreset_ManualUserCustom_ByID(val);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }

  

  // #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
  //   // CommandSet_Mixer_Flags_Enabled(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
   
  // // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
  // //   mixer.time_scaler = jtok.getInt();
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // // }

  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){ 
  //   // CommandSet_Mixer_RunTimeScalerPercentage(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
  //   // CommandSet_Mixer_RunningID(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  // #endif //ENABLE_PIXEL_AUTOMATION_PLAYLIST

  #endif //ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

  

  if(jtok = obj["SegNew"].getObject()["addSegment"]){  
    // void mAnimatorLight::
    // CommandSet_Mixer_RunningID(jtok.getInt());
    // uint8_t val = jtok.getInt();
    // mixer.running_id = val;
    // LoadMixerGroupByID(val);
    // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    // #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

    uint8_t id = jtok.getInt();


    uint16_t start = id;
    uint16_t stop = STRIP_SIZE_MAX;




    Serial.println();

    ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

    Segment_AppendNew(start, stop, jtok.getInt());

    // for (size_t s = 0; s < getSegmentsNum(); s++) {
    //     Segment_New &sg = getSegment(s);
    //     DEBUG_LINE_HERE;
    //     // if (sg.isSelected()) 
    //     // {

    //       DEBUG_LINE_HERE;
    //       // deserializeSegment(segVar, s, presetId);

    //       Segment_New& seg = getSegment(s);
    //       Segment_New prev = seg; //make a backup so we can tell if something changed

    //       // if using vectors use this code to append segment
    //       if (id >= getSegmentsNum()) {
    //         DEBUG_LINE_HERE;
    //         if (stop <= 0) return 0; // ignore empty/inactive segments
    //         appendSegment(Segment_New(0+id, getLengthTotal()));
    //         id = getSegmentsNum()-1; // segments are added at the end of list
    //         ALOG_INF(PSTR("new ID %d"), id);
    //       }

    //       //didSet = true;
    //     // }
    //   }






  }

  if(jtok = obj["SegNew"].getObject()["removeSegment"]){  
    
    uint8_t start = 0;
    uint8_t stop = 100;
    uint8_t id = jtok.getInt();
    uint8_t force = 0;

    Serial.println();

    segments[jtok.getInt()].pixel_range.stop = 0; // to disable segment

    ALOG_INF(PSTR("getSegmentsNum() %d|%d %d"), id, getSegmentsNum(), segments[jtok.getInt()].pixel_range.stop);

    // remove all inactive segments (from the back)
    if(id==255){ purgeSegments(true); }
    else{        purgeSegments(false); }

  }
    if(jtok = obj["SegNew"].getObject()["removeSegment2"]){  
    // void mAnimatorLight::
    // CommandSet_Mixer_RunningID(jtok.getInt());
    // uint8_t val = jtok.getInt();
    // mixer.running_id = val;
    // LoadMixerGroupByID(val);
    // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    // #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

    uint8_t start = 0;
    uint8_t stop = 100;
    uint8_t id = jtok.getInt();
    uint8_t force = 0;

    Serial.println();

    segments[jtok.getInt()].pixel_range.stop = 0; // to disable segment

    ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

    // remove all inactive segments (from the back)
    int deleted = 0;
    if (segments.size() <= 1) return 0;
    for (size_t i = segments.size()-1; i > 0; i--)
      if (segments[i].pixel_range.stop == 0 || force) {
        DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
        deleted++;
        segments.erase(segments.begin() + i);
      }else{
        DEBUG_LINE_HERE;
      }
    if (deleted) {
      DEBUG_LINE_HERE;
      segments.shrink_to_fit();
      if (_mainSegment >= segments.size()){
        DEBUG_LINE_HERE;
        setMainSegmentId(0);
      }
    }

// purgeSegments


  }




  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  int val = 0;

  if(jtok = obj[PM_JSON_HARDWARE_TYPE])
  {
    ALOG_HGL(PSTR("Replaced by bus methods"));
    // if(jtok.isStr()){
    //   if((tmp_id=GetPixelHardwareTypeIDbyName(jtok.getStr()))>=0){
    //     CommandSet_PixelHardwareTypeID(tmp_id);
    //     data_buffer.isserviced++;
    //   }
    // }else
    // if(jtok.isNum()){
    //   CommandSet_PixelHardwareTypeID(jtok.getInt());
    //   data_buffer.isserviced++;
    // }
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  // if(jtok = obj[PM_JSON_RGB]){ // Must accept "RRGGBBCC" and "RR,GG,BB,CC"
  //   CommandSet_ActiveSolidPalette_RGB_Ctr(jtok.getStr());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // #ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  // if(jtok = obj[PM_JSON_HUE]){ // Assume range 0-359
  //   CommandSet_ActiveSolidPalette_Hue_360(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  // #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  // if(jtok = obj[PM_JSON_SAT]){ // Assume range 0-100
  //   CommandSet_ActiveSolidPalette_Sat_255(mapvalue(jtok.getInt(), 0,100, 0,255));
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), getSat());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }else
  // if(jtok = obj[PM_JSON_SAT_255]){ // alternate full range 0-255
  //   CommandSet_ActiveSolidPalette_Sat_255(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT_255)), getSat());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  if(jtok = obj[PM_JSON_BRIGHTNESS]){ // Assume range 0-100
    CommandSet_Brt_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), jtok.getInt());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_255]){ // alternate full range 0-255
    CommandSet_Brt_255(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_255)), getBri());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
    CommandSet_Global_BrtRGB_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), getBriRGB_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB_255]){ // alternate full range 0-255
    CommandSet_Global_BrtRGB_255(jtok.getInt());
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), getBriRGB_Global());
    data_buffer.isserviced++;
  }

  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
    CommandSet_Global_BrtCCT_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), getBriCCT_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // alternate full range 0-255
    CommandSet_Global_BrtCCT_255(jtok.getInt());
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), getBriCCT_Global());
    data_buffer.isserviced++;
  }


  #ifndef ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON
  /**
   * @brief Construct a new if object
   * TimeOn is not unique here, so light prefix needs to be added
   * Maybe "LightTimeOn" or "Light:{TimeOn:x}"
   * 
   */
  if(jtok = obj[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    ALOG_WRN(PSTR("ToBePhasedOut: \"TimeOn\"->\"Light:{\"TimeOn\":x}"));
    data_buffer.isserviced++;
  }
  #endif // ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON


  // New unique version to phase in
  if(jtok = obj["Light"].getObject()[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    data_buffer.isserviced++;
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs ); 
  }



  if(jtok = obj[PM_JSON_LIGHTPOWER]){
    int8_t state = 0;
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else
    if(jtok.isNum()){
      state = jtok.getInt(); 
    }
    ModifyStateNumberIfToggled(&state, light_power_state);
    DEBUG_LINE_HERE;
    CommandSet_LightPowerState(state);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_LIGHTPOWER)), light_power_state);
    #endif // ENABLE_LOG_LEVEL_DEBUG       
  }
 
  if(jtok = obj[PM_JSON_PALETTE_EDIT].getObject()[PM_JSON_COLOUR_PALETTE]){

    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog(LOG_LEVEL_TEST, PSTR("Palette: PM_JSON_PALETTE_EDIT "));
    // #endif // ENABLE_LOG_LEVEL_INFO

    JsonParserToken jtok_data = obj[PM_JSON_PALETTE_EDIT].getObject()[PM_JSON_DATA];
    if(!jtok_data.isNull()){ 

      uint8_t jsonpair_count = jtok_data.size();
      uint8_t colour_array[jsonpair_count];
      memset(colour_array,0,sizeof(colour_array));

      for(int index = 0; index < jsonpair_count; index++){
        jtok_data.nextOne(); //skip start of object
        colour_array[index] = jtok_data.getInt();
      }
      
      CommandSet_PaletteColour_RGBCCT_Raw_By_ID(jtok.getInt(), colour_array, sizeof(colour_array));
      data_buffer.isserviced++;

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_Array(LOG_LEVEL_DEBUG, "rgbcct", colour_array, sizeof(colour_array));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
  }

  
  if(jtok = obj[PM_JSON_ANIMATIONENABLE]){ 
    int8_t state = 0;
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else
    if(jtok.isNum()){
      state = jtok.getInt(); 
    }
    
    // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
    // ModifyStateNumberIfToggled(&state, animation.flags.fEnable_Animation);
    // #else
    // ModifyStateNumberIfToggled(&state, pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation);
    // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

    // CommandSet_EnabledAnimation_Flag(state);
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ANIMATIONENABLE)), animation.flags.fEnable_Animation);    
    // #endif // ENABLE_LOG_LEVEL_DEBUG

  }

  
  /**
   * @brief Construct a new if object
   * 
   PixelRange replaces this, stripsize will instead be total length of max pixel
   * 
   */
  if(jtok = obj[PM_JSON_STRIP_SIZE]){
    CommandSet_LightSizeCount(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_STRIP_SIZE)), settings.light_size_count);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  // if(jtok = obj[PM_JSON_SCENE].getObject()[PM_JSON_COLOUR]){

  //   uint8_t colour_array[5];
  //   memset(colour_array,0,sizeof(colour_array));
  //   uint8_t jsonpair_count = jtok.size();

  //   for(int index = 0; index < jsonpair_count; index++){
  //     jtok.nextOne(); //skip start of object
  //     Serial.println(jtok.getInt());
  //     colour_array[index] = jtok.getInt();
  //   }
    
  //   CommandSet_ActiveSolidPalette_Raw(colour_array);
  //   data_buffer.isserviced++;

  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),colour_array[0],colour_array[1],colour_array[2],colour_array[3],colour_array[4]);
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // //  If command source was webui, then override changes
  // if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  // {
  //   // pCONT_lAni->segment_animation_override.time_ms = 100;
  // }
  
  // if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  // {
  //   pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  // }



  if(jtok = obj["Bus"].getObject()["Init"])
  { 
    DEBUG_LINE_HERE;

    doInitBusses = true;

    TestCode_AddBus1();


    /**
     * @brief Test case 1
     * 
     */


  }





  //  If command source was webui, then override changes
  if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  {
    // pCONT_lAni->segment_animation_override.time_ms = 100;
  }
  


} // END PARSE COMMANDS


void mAnimatorLight::TestCode_AddBus1()
{
  // pCONT_iLight->busConfigs
  uint8_t bus_index = 0;

  uint8_t defPin[] = {4};
  uint16_t start = 0;
  uint16_t length = 10;
  if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_WS2812_RGB, defPin, start, length);    
  bus_index++;

  defPin[0] = 13;
  start = 10;
  length = 10;
  if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_WS2812_RGB, defPin, start, length);    
  bus_index++;

  defPin[0] = 14;
  start = 20;
  length = 10;
  if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_WS2812_RGB, defPin, start, length);    
  bus_index++;

  defPin[0] = 27;
  start = 30;
  length = 10;
  if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_SK6812_RGBW, defPin, start, length);    
  bus_index++;

  uint8_t defPin_pwm[] = {16, 17, 5, 21, 22};
  start = 41;
  length = 1;
  if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_ANALOG_5CH, defPin_pwm, start, length);    
  bus_index++;

  // uint8_t defPin_pwm2[] = {23};
  // start = 42;
  // length = 1;
  // if (pCONT_iLight->busConfigs[bus_index] != nullptr) delete pCONT_iLight->busConfigs[bus_index];
  // pCONT_iLight->busConfigs[bus_index] = new BusConfig(BUSTYPE_ANALOG_1CH, defPin_pwm2, start, length, DEFAULT_LED_COLOR_ORDER);    
  // bus_index++;

  //13,14,27,4

}


void mAnimatorLight::CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index)
{
  
  SEGMENT_I(segment_index).colour_type = (RgbcctColor::LightSubType)id;

  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_type);

}

const char* mAnimatorLight::GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen)
{
  // switch(id){  default:    
  
  //   case RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID:    memcpy(buffer, "RGB", sizeof("RGB")); break;
  //   case RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID:    memcpy(buffer, "RGBW", sizeof("RGBW")); break;
  
  // }
  return buffer;
}
int8_t mAnimatorLight::GetColourTypeIDbyName(const char* c)
{
  if(*c=='\0'){ return -1; }
  // if(strstr_P(c,PM_TRANSITION_ORDER_RANDOM_NAME_CTR)){  return TRANSITION_ORDER__RANDOM__ID;  }
  // if(strstr_P(c,PM_TRANSITION_ORDER_INORDER_NAME_CTR)){ return TRANSITION_ORDER__INORDER__ID; }

  return -1;
}


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



void mAnimatorLight::CommandSet_AnimationModeID(uint8_t value){

  uint8_t segment_index = 0;

  char buffer[60];

  pCONT_lAni->SEGMENT_I(0).animation_mode_id = value;  // this is wrong
          
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
  #endif

}


const char* mAnimatorLight::GetAnimationModeName(char* buffer, uint16_t buflen){
  return GetAnimationModeNameByID(  pCONT_lAni->SEGMENT_I(0).animation_mode_id, buffer, buflen);
}
const char* mAnimatorLight::GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen){
  switch(id){
    default:
    // case ANIMATION_MODE_TURN_ON_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_TURN_ON_NAME_CTR , sizeof(PM_ANIMATION_MODE_TURN_ON_NAME_CTR)); break;
    // case ANIMATION_MODE_TURN_OFF_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_TURN_OFF_NAME_CTR , sizeof(PM_ANIMATION_MODE_TURN_OFF_NAME_CTR)); break;
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



// const char* mAnimatorLight::GetHardwareColourTypeName(char* buffer, uint8_t buflen, uint8_t segment_index)
// {
//   return GetHardwareColourTypeNameByID(SEGMENT_I(segment_index).mode_id, buffer, buflen, segment_index);
// }
// const char* mAnimatorLight::GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen, uint8_t segment_index){
//   sprintf(buffer, PM_SEARCH_NOMATCH);
//   // switch(id){
//   //   default:
//   //   case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
//   // }
//   return buffer;
// }



void mAnimatorLight::CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index)
{

  SEGMENT_I(segment_index).intensity_value = value;

}


void mAnimatorLight::CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index)
{

  SEGMENT_I(segment_index).speed_value = value;

}


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Flags_Movement_Direction(uint8_t value){

//   flashersettings.flags.movement_direction = value;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Brightness_Min(uint8_t value){
  
//   flashersettings.brightness_min = value;
  
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Brightness_Max(uint8_t value){
  
//   flashersettings.brightness_max = value;
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value){
  
//   shared_flasher_parameters.alternate_brightness_min = value;
  
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value){
  
//   shared_flasher_parameters.alternate_brightness_max = value;
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value){
//     shared_flasher_parameters.alternate_random_amount_as_percentage = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value){
//     animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }
//   #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/



//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

//   #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
// //  void mAnimatorLight:: CommandSet_Mixer_Flags_Enabled
// //   if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
// //     mixer.flags.Enabled = jtok.getInt();
// //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
// //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   }
   
//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

//   // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
//   //   mixer.time_scaler = jtok.getInt();
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
//   //   #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // }

//   // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){
//   // void mAnimatorLight::  CommandSet_Mixer_RunTimeScalerPercentage 
//   //   mixer.run_time_duration_scaler_as_percentage = jtok.getInt();
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
//   //   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // }
//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_PaletteID(uint8_t value, uint8_t segment_index)
{

  char buffer[50];

  SEGMENT_I(segment_index).palette.id = value < mPalette::PALETTELIST_TOTAL_LENGTH ? value : 0;
  
  _segment_index_primary = segment_index;
  LoadPalette(segments[segment_index].palette.id, segment_index);

  // //If "id" is in the range of rgbcct, make sure to automatically make internal_rgbctt track it
  // if((value>=mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)
  // &&(value<mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID))
  // {
  //   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(value, segment_index);
  // }

  // ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), mPaletteI->GetPaletteNameByID(SEGMENT_I(segment_index).palette.id, buffer, sizeof(buffer)));
  
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Time_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
// YES , remove logging inside CommandSet (just leave in json parse) so internal use will not always use serial
void mAnimatorLight::CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index){
    
  SEGMENT_I(segment_index).transition.time_ms = value;

  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)), SEGMENT_I(segment_index).transition.time_ms );  

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Rate_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index){
    
  SEGMENT_I(segment_index).transition.rate_ms = value;

  if(SEGMENT_I(segment_index).transition.rate_ms < SEGMENT_I(segment_index).transition.time_ms){ 
    SEGMENT_I(segment_index).transition.time_ms = SEGMENT_I(segment_index).transition.rate_ms;
  }

  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)), SEGMENT_I(segment_index).transition.rate_ms );  

}



/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightsCountToUpdateAsNumber DUPLICATED EFFECT 1/2*****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightsCountToUpdateAsNumber(uint16_t value, uint8_t segment_index)
{
  
    SEGMENT_I(segment_index).transition.pixels_to_update_as_number = value;

  // animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(value);

  // Tmp fix until I merge them - UNTESTED HOW THIS WILL AFFECT CODE
  // strip_size_requiring_update = animation.transition.pixels_to_update_as_number ;
  // AddLog(LOG_LEVEL_WARN, PSTR("strip_size_requiring_update = animation.transition.pixels_to_update_as_number ; COULD CAUSE ERROR "));
  // SetLEDOutAmountByPercentage(animation.transition.pixels_to_update_as_number)

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), value);
  #endif

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightsCountToUpdateAsPercentage DUPLICATED EFFECT 2/2*****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightsCountToUpdateAsPercentage(uint8_t value, uint8_t segment_index)
{
    
  SEGMENT_I(segment_index).transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
 
  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
  
}

uint16_t mAnimatorLight::GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage, uint8_t segment_index)
{
  if(segment_index < segments.size()){
    return mapvalue(percentage, 0,100, 0, SEGMENT_I(segment_index).length());
    #ifdef ENABLE_DEVFEATURE_UNIFIED_SEGMENT_REMOVING_MY_CODE
    return mapvalue(percentage, 0,100, 0,SEGMENT_I(segment_index).length());
    #endif
  }
  return 0;
}
uint8_t mAnimatorLight::GetPixelsToUpdateAsPercentageFromNumber(uint16_t number, uint8_t segment_index)
{
  uint8_t percentage = 0; // currently scaled to 100, but will want to scale to 255 and change name away from "percentage" to "scaler?"
  if(number != 0)
  {
    percentage = mapvalue(number ,0,SEGMENT_I(segment_index).length(), 0,100);
    ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_PIXELS_UPDATE_PERCENTAGE)), percentage);
  }
  return 1; // fallback

}



void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setHue360(hue_new);      
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setSat255(sat_new);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setCCT(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessRGB255(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessCCT255(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Manual(uint8_t* values, uint8_t value_count, uint8_t colour_index, uint8_t segment_index)
{
    
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
 SEGMENT_I(segment_index).rgbcctcolors[colour_index].setChannelsRaw(values);    // must be 5 for now, will consider less later

  #ifdef ENABLE_LOG_LEVEL_INFO
  char buffer[30];
  snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  #endif // ENABLE_LOG_LEVEL_DEBUG
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_LightSubType(uint8_t subtype, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= pCONT_lAni->SEGMENT_I(segment_index).rgbcctcolors.size()){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setSubType(subtype); 
}



/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtRGB *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index)
{
  // SEGMENT_I(segment_index).rgbcct_controller->setBrightnessRGB255(bri);
 SEGMENT_I(segment_index).flags.fForceUpdate = true;

  _briRGB_Global = bri;
  setBriRGB_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessRGB());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index) 
{
  pCONT_lAni->SEGMENT_I(segment_index).flags.fForceUpdate = true; 
  setBriCT_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessCCT255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}






/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/


// /**
//  * direct percentage version
//  * */
// void mAnimatorLight::CommandSet_ActiveSolidPalette_ColourTemp_Percentage(uint8_t percentage) {

//   CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(percentage, 0,100, _ct_min_range,_ct_max_range));

//   // rgbcct_controller.setCCT(ct);
//   // animation.flags.fForceUpdate = true;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), rgbcct_controller.getCCT());
//   // #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

// /**
//  * "InternalSet" ie direct control, does not have Addlog feedback like commandset
//  * */
// void mAnimatorLight::InternalSet_ActiveSolidPalette_ColourTemp(uint16_t ct) {
//   rgbcct_controller.setCCT(ct);
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// }







/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mAnimatorLight::CommandSet_PixelHardwareTypeID(uint8_t value){
//   char buffer[20];
//   pCONT_set->Settings.light_settings.type = value;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS
// } 
// int8_t mAnimatorLight::GetPixelHardwareTypeIDbyName(const char* c){
//   if(*c=='\0') return -1;
//   if(     strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)==0){ return LT_PWM5; }
//   else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)==0){       return LT_ADDRESSABLE_WS281X; }
//   else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_SK6812_CTR)==0){       return LT_ADDRESSABLE_SK6812; }
//   return -1;
// }
// const char* mAnimatorLight::GetPixelHardwareTypeName(char* buffer){
//   return GetPixelHardwareTypeNamebyID(pCONT_set->Settings.light_settings.type, buffer);
// }
// const char* mAnimatorLight::GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer){
//   switch(id){
//     case LT_PWM5:   memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)); break;
//     case LT_ADDRESSABLE_WS281X: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR));         break;
//     case LT_ADDRESSABLE_SK6812: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_SK6812_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_SK6812_CTR));         break;
//   }
//   return buffer;
// }

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightPowerState *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightPowerState(uint8_t state){

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_LIGHTPOWER)), light_power_state);
  #endif   

  if(state == LIGHT_POWER_STATE_OFF_ID) // turn off
  {
    // pCONT_lAni->CommandSet_Animation_Transition_Rate_Ms(10000);
    pCONT_lAni->CommandSet_LightsCountToUpdateAsPercentage(100);
    
    pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms =  pCONT_lAni->SEGMENT_I(0).single_animation_override_turning_off.time_ms; // slow turn on

ALOG_INF(PSTR("Setting override for off %d"), pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms);

    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

    CommandSet_Brt_255(0);
    
  }
  else
  if(state == 1) // turn on
  {

    // pCONT_lAni->CommandSet_Animation_Transition_Time_Ms(1000);

    pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms = 1000; // slow turn on
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;


    // pCONT_lAni->CommandSet_Animation_Transition_Rate_Ms(1000);
    // pCONT_lAni->CommandSet_LightsCountToUpdateAsPercentage(100);
    
    CommandSet_Brt_255(255);

    //make sure both are set
    // CommandSet_Global_BrtRGB_255(255);
    // CommandSet_Global_BrtCCT_255(255);
    
    // pCONT_lAni->CommandSet_PaletteID(10, 0);
    
    // pCONT_lAni->CommandSet_Flasher_FunctionID(0 /**Add define later for "DEFAULT_EFFECT" */);//pCONT_lAni->EFFECTS_FUNCTION__SOLID_COLOUR__ID);



  }

}


bool mAnimatorLight::CommandGet_LightPowerState()
{
  return 
  light_power_state
  // getBri_Global() 
  ? true : false;
}


void mAnimatorLight::CommandSet_Brt_255(uint8_t brt_new){
    
  // pCONT_lAni->SEGMENT_I(0).rgbcct_controller->setBrightness255(brt_new);
  pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  setBriRGB_Global(brt_new);
  // probably needs to check if they are linked here, or internally
  setBriCT_Global(brt_new);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getBrightness255());
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
}



/******************************************************************************************************************************
*******************************************************************************************************************************
****************** EnabledAnimation_Flag *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_EnabledAnimation_Flag(uint8_t value){

  pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation = value;

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ANIMATIONENABLE)), pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation);    
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Auto_Time_Off_Secs *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Auto_Time_Off_Secs(uint16_t value){
    
  auto_off_settings.time_decounter_secs = value;

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightSizeCount *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightSizeCount(uint16_t value){

  settings.light_size_count = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_STRIP_SIZE)), settings.light_size_count);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteColour_RGBCCT_Raw *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_PaletteColour_RGBCCT_Raw_By_ID(uint8_t palette_id, uint8_t* buffer, uint8_t buflen){

      // AddLog(LOG_LEVEL_TEST, PSTR("START[%d|%d] fMapIDs_Type=%d"),
      // palette_id,mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH_ID,

  // Serial.flush();
  // delay(3000);

  // Check if palette can be edited
  if(!mPaletteI->CheckPaletteByIDIsEditable(palette_id)){ 
    
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("Palette can not be edited"));
    #endif // ENABLE_LOG_LEVEL_INFO
      return; 
  }

  // Convert id into array index if needed
  uint8_t palette_id_adjusted_to_array_index = 0;
  // Get starting position of buffer
  uint8_t* palette_buffer = nullptr;

  if(palette_id<mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID){
    palette_id_adjusted_to_array_index = palette_id;
    palette_buffer = &pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID)*palette_id_adjusted_to_array_index];
    // Clear the entire new colour to the "unset" values
    memset(palette_buffer,COLOUR_MAP_NONE__ID,20); // change COLOUR_MAP_NONE_ID to be 0 going forward, and as "black", although considered unset
    
    // Add to select correct buffer depending on palette type
    memcpy(palette_buffer,buffer,buflen);

    // rgbcct_controller.UpdateFromExternalBuffer();
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

  }else
  if((palette_id>=mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&(palette_id<mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)){
 

    palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID;    
    palette_buffer = &pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)*palette_id_adjusted_to_array_index];
    memset(palette_buffer,0,5); // change COLOUR_MAP_NONE_ID to be 0 going forward, and as "black", although considered unset

    // Add to select correct buffer depending on palette type
    memcpy(palette_buffer,buffer,buflen);

    // pCONT_lAni->SEGMENT_I(0).rgbcct_controller->UpdateFromExternalBuffer();
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

  }else
  if((palette_id>=mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID)&&(palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID)){
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("Palette: Generic \"%d\"\n\r"),palette_id);
    #endif // ENABLE_LOG_LEVEL_INFO

    palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;    
    palette_buffer = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map;

    // AddLog(LOG_LEVEL_TEST, PSTR("Buffer len %d"),buflen);
    
    // pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH_ID-mPaletteI->PALETTELIST_VARIABLE_GENERIC_01_ID)*palette_id_adjusted_to_array_index];
    
    // Clear old buffer space
    memset(palette_buffer, 0, palette_encoded_users_colour_map_LENGTH); //200 now   
    // Write new palette data into buffer space
    memcpy(palette_buffer,buffer,buflen);
    // Parse buffer data to correctly set data parameters
    mPaletteI->init_PresetColourPalettes_User_Generic_Fill(0);

    // DEBUG_LINE_HERE;


  }


  // Update new length stored in palettelist
  mPaletteI->GetColourMapSizeByPaletteID(palette_id);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_Array(LOG_LEVEL_COMMANDS, "changed colour", palette_buffer, buflen);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "hsbid map", pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map, 20*2);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "rgbcct map", pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map, 5*5);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "encoded map", pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map, 25);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "colour id map", mPaletteI->palettelist.ptr->data, 25);
  // AddLog(LOG_LEVEL_COMMANDS, "colour map size %d", mPaletteI->palettelist.ptr->data_length);
  // #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

  // Serial.flush();
  // delay(3000);

 

}



#endif //USE_MODULE_LIGHTS_ANIMATOR




