#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT D_TOPIC "Checking all commands mAnimatorLight::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

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
             
        ALOG_INF(PSTR("Creating new segment"));

        Segment_AppendNew(0, 0, segment_i+1);
        
        Serial.println(SEGMENT_I(segment_i).hardware_element_colour_order.data);

        if(pCONT_lAni->segments.size()!=0)
        {
          Serial.println(pCONT_lAni->segments.size());Serial.flush();
        }

      }

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

  // DEBUG_LINE_HERE;

  /**
   * @brief Important this remains above other commands, as some others rely on states being set (eg. Rgbcct user palettes)
   * 
   */
  if(jtok = obj[PM_JSON_COLOUR_PALETTE])
  {

    ALOG_INF( PSTR("PM_JSON_COLOUR_PALETTE") );

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
  //   pCONT_iLight->SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
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
  //   pCONT_iLight->SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->SEGMENT_I(segment_index).flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }


  if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER]){
    if(jtok.isStr()){
      CommandSet_HardwareColourOrderTypeByStr(jtok.getStr(), segment_index);
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
    
  }


  if(jtok = obj["paletteBlend"]){
    paletteBlend = jtok.getInt();

  }

   


  if(jtok = obj[PM_JSON_COLOUR_TYPE]){
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
//         // pCONT_iLight->SEGMENT_I(segment_index).mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;
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
//         for(uint16_t i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
//           if(pCONT_iLight->RgbColorto32bit(GetPixelColor(i))){
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

//         SEGMENT_I(0).mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;

//         SetPixelColor(val, colour, 0);

//         //animation_colours[val].DesiredColour = colour;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_PIXEL D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
//     #endif// ENABLE_LOG_LEVEL_INFO          
//       }
//       // pixel_group.mapped_array_data.length = index;

//       pCONT_iLight->ShowInterface();

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
  //     // pCONT_iLight->SEGMENT_I(segment_index).flags.fEnable_Animation ^= 1;
  //   }else{
  //     // pCONT_iLight->SEGMENT_I(segment_index).flags.fEnable_Animation = state;
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
  //     memcpy(&pCONT_iLight->animation,&pCONT_iLight->animation_stored,sizeof(pCONT_iLight->animation));// RESTORE copy of state

  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_ON_ID);
  //     pCONT_iLight->light_power_state = true;

  //     //pCONT_iLight->SEGMENT_I(segment_index).mode_id = MODE_TURN_ON_ID;
  //     data_buffer.isserviced++;
  //   }else if(strstr(onoff,"OFF")){
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIXEL  "\"onoff\"=\"OFF\""));
  //     #endif
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "MODE_TURN_OFF_ID"));
  //     #endif
  //     memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation)); // STORE copy of state
  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_OFF_ID);
  //     pCONT_iLight->light_power_state = false;

  //     //pCONT_iLight->SEGMENT_I(segment_index).mode_id = MODE_TURN_OFF_ID;
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
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(mapvalue(jtok.getInt(), 0,100, pCONT_iLight->_ct_min_range, pCONT_iLight->_ct_max_range), colour_index, segment_index);
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


// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER]){
//     if(jtok.isStr()){
//       // if((tmp_id=GetHardwareColourTypeIDbyName(jtok.getStr()))>=0){
//         CommandSet_HardwareColourOrderTypeByStr(jtok.getStr());
//         data_buffer.isserviced++;
//       // }
//     }
//     // else
//     // if(jtok.isNum()){      
//     //   CommandSet_HardwareColourOrderTypeID(jtok.getInt());
//     //   data_buffer.isserviced++;
//     // }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  // if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER2]){
  //   if(jtok.isStr()){
  //     if((tmp_id=GetHardwareColourTypeIDbyName2(jtok.getStr()))>=0){
  //       CommandSet_HardwareColourOrderTypeID2(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
  //   if(jtok.isNum()){      
  //     CommandSet_HardwareColourOrderTypeID2(jtok.getInt());
  //     data_buffer.isserviced++;
  //   }
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName2(buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

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



} // END PARSE COMMANDS


// void mAnimatorLight::CommandSet_EffectsModeID(uint8_t mode, uint8_t segment)
// {
  
// #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
//   char buffer[50];
//   mEffects->setMode(segment, mode);
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetEffectsModeName(buffer, sizeof(buffer)));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS
// #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

// } 



// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   /********************************************************************************************************************************
//   **********Flasher Function ******************************************************************************************************
//   ********************************************************************************************************************************/
// void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value){
  
//   flashersettings.function = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
//   flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   pCONT_iLight->animation.flags.animator_first_run= true; // first run, so do extra things

  
  
//   #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
//   #endif
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   char buffer[30];
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_EFFECTS, D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

// int8_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f){

//   if(f=='\0') return -2;
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR)){ return EFFECTS_FUNCTION_SOLID_COLOUR_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)){  return EFFECTS_FUNCTION_SLOW_GLOW_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR)){ return EFFECTS_FUNCTION_FIREPLACE_1D_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR)){ return EFFECTS_FUNCTION_STEPPING_PALETTE_ID; }
  
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID; }
  
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR)){ return EFFECTS_FUNCTION_TESTER_ID; }

//   return -1;

// }
// const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen){
//   return GetFlasherFunctionNamebyID(flashersettings.function, buffer, buflen);
// }
// const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen){
//   switch(id){
//     default:  snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);  break;
//     case EFFECTS_FUNCTION_STATIC_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SLOW_GLOW_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_STEPPING_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR);  break;
//     case EFFECTS_FUNCTION_SEQUENTIAL_ID:  snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR); break;
//     case EFFECTS_FUNCTION_SOLID_COLOUR_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_FIREPLACE_1D_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR);  break;

//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR);  break;
  
  
//    case EFFECTS_FUNCTION_TESTER_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR);  break;

//   }
//   return buffer;
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// int8_t mAnimatorLight::GetFlasherRegionIDbyName(const char* c){
//   if(*c=='\0') return -1;
//   if(     strcmp_P(c,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR)==0){ return EFFECTS_REGION_COLOUR_SELECT_ID; }
//   else if(strcmp_P(c,PM_EFFECTS_REGION_ANIMATE_NAME_CTR)==0){       return EFFECTS_REGION_ANIMATE_ID; }
//   return -1;
// }
// const char* mAnimatorLight::GetFlasherRegionName(char* buffer){
//   return GetFlasherRegionNamebyID(flashersettings.region, buffer);
// }
// const char* mAnimatorLight::GetFlasherRegionNamebyID(uint8_t id, char* buffer){
//   switch(id){
//     case EFFECTS_REGION_COLOUR_SELECT_ID:   memcpy_P(buffer,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR,sizeof(PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR));
//     case EFFECTS_REGION_ANIMATE_ID:         memcpy_P(buffer,PM_EFFECTS_REGION_ANIMATE_NAME_CTR,sizeof(PM_EFFECTS_REGION_ANIMATE_NAME_CTR));
//   }
//   return buffer;
// }





//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/




// void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value){
  
//   flashersettings.update_colour_region.refresh_secs = value; 
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   ********** Palette_Generation_Randomise_Brightness_Enable ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Palette_Generation_Randomise_Brightness_Mode(uint8_t value){

//   //instead of simple on and off, it will contain a list of modes
  
//     // 0, 1, 2
//     // 0 = off, 1 = between range, 2 = switch between max and min randomly, 3 = switch between max and min alternatively 

//   pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = value;
// pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = value; // phase out, as its probably not needed to have both methods
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_PALETTE_GENERATION, D_JSON_RANDOMISE_BRIGHTNESS_MODE)), value);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** HardwareColourType (RGB) *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_HardwareColourOrderTypeByStr(const char* c, uint8_t segment_index)
{

  if(segment_index >= pCONT_lAni->segments.size()){ return; }

  if(!c){ return; }
  if(strlen(c)<=5){
    ALOG_DBM( PSTR("Valid Length"));
  }else{
    AddLog(LOG_LEVEL_ERROR, PSTR("INVALID Length"));
    return;
  }

  SEGMENT_I(segment_index).hardware_element_colour_order.red = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
  SEGMENT_I(segment_index).hardware_element_colour_order.green = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
  SEGMENT_I(segment_index).hardware_element_colour_order.blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
  SEGMENT_I(segment_index).hardware_element_colour_order.white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
  SEGMENT_I(segment_index).hardware_element_colour_order.white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;

  for(uint8_t index=0;index<strlen(c);index++)
  {
    // if(c[index]==0){ break; }
    if((c[index]=='R')||(c[index]=='r')){
      SEGMENT_I(segment_index).hardware_element_colour_order.red = index;
    }else
    if((c[index]=='G')||(c[index]=='g')){
      SEGMENT_I(segment_index).hardware_element_colour_order.green = index;
    }else
    if((c[index]=='B')||(c[index]=='b')){
      SEGMENT_I(segment_index).hardware_element_colour_order.blue = index;
    }else
    if((c[index]=='C')||(c[index]=='c')){
      SEGMENT_I(segment_index).hardware_element_colour_order.white_cold = index;
    }else
    if((c[index]=='W')||(c[index]=='w')){
      SEGMENT_I(segment_index).hardware_element_colour_order.white_warm = index;
    }
  }

  /**
   * @brief A DEVFEATURE where is only C or W is set, then the white channel colour temperature should be disabled (or temp set to give the right priority)
   * This should be handled later internal of rgbcct_controller as a rgb(single channel white) type
   **/
  // if(SEGMENT_I(segment_index).hardware_element_colour_order.c == D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE)
  // {
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR("Temporary Fix: Disable \"C\" channel"));
  //   CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(100, 0,100, pCONT_iLight->_ct_min_range, pCONT_iLight->_ct_max_range), segment_index);
  // }else
  // if(SEGMENT_I(segment_index).hardware_element_colour_order.w == D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE)
  // {
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR("Temporary Fix: Disable \"W\" channel"));
  //   CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(0, 0,100, pCONT_iLight->_ct_min_range, pCONT_iLight->_ct_max_range), segment_index);
  // }


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_DBM( PSTR("segment_index%d == R=%d, G=%d, B=%d, CW=%d, WW=%d \n\r dec%d \n\r %X"),
    segment_index,
    SEGMENT_I(segment_index).hardware_element_colour_order.red,
    SEGMENT_I(segment_index).hardware_element_colour_order.green,
    SEGMENT_I(segment_index).hardware_element_colour_order.blue,
    SEGMENT_I(segment_index).hardware_element_colour_order.white_cold,
    SEGMENT_I(segment_index).hardware_element_colour_order.white_warm,
    SEGMENT_I(segment_index).hardware_element_colour_order.data,
    SEGMENT_I(segment_index).hardware_element_colour_order.data
  );
  // Serial.println(SEGMENT_I(segment_index).hardware_element_colour_order.data, BIN);
  #endif

}



void mAnimatorLight::CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index)
{

  // switch(id)
  // {
  //   default:

  //   case 4:SEGMENT_I(segment_index).colour_type = 

  //   case 4:SEGMENT_I(segment_index).colour_type = RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID;
  //   case 4:SEGMENT_I(segment_index).colour_type = RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID;
  // }
  
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
//     pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
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
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_COLOUR_PALETTE)),SEGMENT_I(segment_index).palette.id);
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), mPaletteI->GetPaletteNameByID(SEGMENT_I(segment_index).palette.id, buffer, sizeof(buffer)));
  #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

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

  pCONT_iLight->_briRGB_Global = bri;
  pCONT_iLight->setBriRGB_Global(bri);
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
  pCONT_iLight->setBriCT_Global(bri);
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
// void mInterfaceLight::CommandSet_ActiveSolidPalette_ColourTemp_Percentage(uint8_t percentage) {

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
// void mInterfaceLight::InternalSet_ActiveSolidPalette_ColourTemp(uint16_t ct) {
//   rgbcct_controller.setCCT(ct);
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// }


#endif //USE_MODULE_LIGHTS_ANIMATOR




