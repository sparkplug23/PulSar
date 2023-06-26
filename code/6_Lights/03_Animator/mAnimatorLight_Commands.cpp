#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::Start"));

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];
  uint16_t isserviced_start_count = data_buffer.isserviced;


  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  }


  uint8_t segments_found = 0;


  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer])
    { 

      ALOG_INF(PSTR("Seg: \"%s\" with %d Slots Active"), buffer, pCONT_lAni->segments.size());

      /**
       * @brief Add check here that only sets the segment if it is already permitted
       * 
       */
    
      if(segment_i > pCONT_lAni->segments.size()-1)
      { 
        ALOG_HGL(PSTR("Creating new segment %d|%d"),segment_i,pCONT_lAni->segments.size());
        Segment_AppendNew(0, 0, segment_i+1);
      }

    

      data_buffer.isserviced += subparse_JSONCommand(jtok.getObject(), segment_i);

      segments_found++;

      // If segment commands updated, some effects may need reset
      SEGMENT_I(segment_i).call = 0; 

    }
  }


  /**
   * @brief If no segments have been directly set, then assume default of Segment0
   **/
  if(segments_found == 0)
  {
    data_buffer.isserviced += subparse_JSONCommand(obj); // Legacy commands
  }

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::End"));

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
   **/
  if(segment_index == 255)
  {
    ALOG_COM( PSTR("Segment Index assumed \"0\"") );
    segment_index = 0;

    // purgeSegments(true); // reduce to single segment
    ALOG_DBM(PSTR("purgeSegments(true); // reduce to single segment REMOVED THIS, NEEDS BETETR HOME"));
   
  }

  /**
   * @brief Important this remains above other commands, as some others rely on states being set (eg. Rgbcct user palettes)
   **/
  if(jtok = obj[PM_JSON_COLOUR_PALETTE])
  {
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
        ALOG_DBG(PSTR("tmp_id=%d"),tmp_id);
        CommandSet_PaletteID(tmp_id, segment_index);
        data_buffer.isserviced++;
        ALOG_DBG(PSTR("SEGMENT_I(segment_index).palette.id=%d"), SEGMENT_I(segment_index).palette.id);
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


  
  
  if(jtok = obj["Effects"].getObject()["Params"])
  { 
    if(jtok.isArray())
    {
      uint8_t array[4];
      uint8_t arrlen = 0;
      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) 
      {
        if(arrlen > 4){ break; }
        switch(arrlen)
        {
          case 0: SEGMENT_I(segment_index).user_params.val0 = v.getInt(); break;
          case 1: SEGMENT_I(segment_index).user_params.val1 = v.getInt(); break;
          case 2: SEGMENT_I(segment_index).user_params.val2 = v.getInt(); break;
          case 3: SEGMENT_I(segment_index).user_params.val3 = v.getInt(); break;
        }
        arrlen++;      
      }

      AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_PIXEL "Effects.Params Segment[%d] = %d,%d,%d,%d"),
        segment_index,
        SEGMENT_I(segment_index).user_params.val0,
        SEGMENT_I(segment_index).user_params.val1,
        SEGMENT_I(segment_index).user_params.val2,
        SEGMENT_I(segment_index).user_params.val3
      );
      
      data_buffer.isserviced++;
    }
  }


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

    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(0).rgbcctcolors[0].getHue360());
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(0).rgbcctcolors[0].getBrightnessRGB());
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(1).rgbcctcolors[0].getHue360());
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(1).rgbcctcolors[0].getBrightnessRGB());


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


  if(jtok = obj["Override"].getObject()["Animation"].getObject()["TimeMs"])
  {
  
    SEGMENT_I(segment_index).single_animation_override.time_ms = jtok.getInt();

  }


  if(jtok = obj["Override"].getObject()["Animation_Off"].getObject()["TimeMs"])
  {
  
    SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms = jtok.getInt();
    ALOG_INF(PSTR("single_animation_override_turning_off=%d"),SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms);

  }

  
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


/**
 * @brief Construct a new if object
 * Although egcolour needs to know this for the internal colour mapping, it is also needed to properly get the effects working. Should be clariefied with a better name later. ColourType should rename internal 
 * and not directly set by the user, but instead the colour order (ie type depends on what is set). ColourType should remain internal.
 * ColourType (to be renamed) for the effect itself, needs to instead have RGB, RGBW etc
 * 
 */
  
  if(jtok = obj[PM_JSON_COLOUR_TYPE])
  {
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
    // ALOG_COM(PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer))); // should be internal to rgbcct
  }

  
  
  

  for(uint8_t colour_index=0;colour_index<6;colour_index++)
  {
    JsonParserObject seg_obj = 0;

    snprintf(buffer, sizeof(buffer), "SegColour%d", colour_index);

    // ALOG_HGL(PSTR("SEGMENT_I(segment_index).rgbcctcolors[colour_index] length %d"), RGBCCTCOLOURS_SIZE);

    

    if(seg_obj = obj[buffer].getObject())
    {
      
      ALOG_COM(PSTR(D_LOG_PIXEL "SegColour[%d] = %s"), colour_index, buffer);

    
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
    
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(
          mapvalue(
            jtok.getInt(), 
            0,100, 
            CCT_MIN_DEFAULT, CCT_MAX_DEFAULT
            // SEGMENT_I(segment_index).rgbcctcolors[colour_index].get_CTRangeMin(), SEGMENT_I(segment_index).rgbcctcolors[colour_index].get_CTRangeMax()

          ), colour_index, segment_index);
        
    
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
      if(jtok = seg_obj[PM_JSON_COLOUR_TYPE])
      {
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
        // ALOG_COM(PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer))); // should be internal to rgbcct
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


  /**
   * @brief 
   * All PRESET options
   **/

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



/***********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************** 
 ******* NOTE: Debug commands only, not part of official commands ****************************************************************************************************************** 
 ************************************************* *********************************************************************************************************************************
 ***********************************************************************************************************************************************************************************/

#ifdef ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS



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
    //     
    //     // if (sg.isSelected()) 
    //     // {

    //       
    //       // deserializeSegment(segVar, s, presetId);

    //       Segment_New& seg = getSegment(s);
    //       Segment_New prev = seg; //make a backup so we can tell if something changed

    //       // if using vectors use this code to append segment
    //       if (id >= getSegmentsNum()) {
    //         
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
        
      }
    if (deleted) {
      
      segments.shrink_to_fit();
      if (_mainSegment >= segments.size()){
        
        setMainSegmentId(0);
      }
    }

// purgeSegments


  }



  if(jtok = obj["Debug"])
  {

    JsonParserObject jObj_debug = jtok.getObject();

    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option0"])
    { 


      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).aux0 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option1"])
    { 


      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).aux1 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }

    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option2"])
    { 
      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).aux2 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option3"])
    {     // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).aux3 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }

    if(jtok = jObj_debug["Options"]){ 
      if(jtok.isArray()){
        uint8_t array[4];
        uint8_t arrlen = 0;
        JsonParserArray arrobj = jtok;
        for(auto v : arrobj) {
          if(arrlen > 4){ break; }
          switch(arrlen)
          {
            case 0: SEGMENT_I(segment_index).aux0 = v.getInt(); break;
            case 1: SEGMENT_I(segment_index).aux1 = v.getInt(); break;
            case 2: SEGMENT_I(segment_index).aux2 = v.getInt(); break;
            case 3: SEGMENT_I(segment_index).aux3 = v.getInt(); break;
          }
          arrlen++;
          // array[arrlen++] = v.getInt();
          // #ifdef ENABLE_LOG_LEVEL_DEBUG
          // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL "PixelRange" " [i%d:v%d]"),arrlen-1,array[arrlen-1]);
          // #endif

          // #endif// ENABLE_LOG_LEVEL_DEBUG          
        }

        SEGMENT_I(segment_index).reset = false; // Do not reset if options have been set

          AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_PIXEL "AuxOptions Segment[%d] flag%d = %d,%d,%d,%d"),
            segment_index,
            SEGMENT_I(segment_index).reset,
            SEGMENT_I(segment_index).aux0,
            SEGMENT_I(segment_index).aux1,
            SEGMENT_I(segment_index).aux2,
            SEGMENT_I(segment_index).aux3
          );
      //   if(SEGMENT_I(segment_index).pixel_range.stop > STRIP_SIZE_MAX+1)
      //   {
      // #ifdef ENABLE_LOG_LEVEL_ERROR
      //     AddLog(LOG_LEVEL_ERROR, PSTR("SEGMENT_I(segment_index).pixel_range.stop exceeds max %d %d"),SEGMENT_I(segment_index).pixel_range.stop, STRIP_SIZE_MAX);
      // #endif //ef ENABLE_LOG_LEVEL_INFO
      //    SEGMENT_I(segment_index).pixel_range.stop = STRIP_SIZE_MAX+1;
      //   }


        
        data_buffer.isserviced++;
      }
    }

    

    if(jtok = obj["Bus"].getObject()["Init"])
    { 

      doInitBusses = true;

      TestCode_AddBus1();


      /**
       * @brief Test case 1
       * 
       */


    }



  } // end Debug options


#endif // ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS options above to be moved into debug methods

/***********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************** 
 ******* NOTE: Commands not integrated for this release ********************************************************************************************************************************** 
 ************************************************* *********************************************************************************************************************************
 ***********************************************************************************************************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
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


  #endif // ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE



  //  If command source was webui, then override changes
  if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  {
    // pCONT_lAni->segment_animation_override.time_ms = 100;
  }
  

 ALOG_DBG(PSTR(D_LOG_LIGHT D_TOPIC "Checking all commands mAnimatorLight::parse_JSONCommand EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));

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


        
void mAnimatorLight::parsesub_json_object_notification_shortcut(JsonParserObject obj)
{

    if(segments.size()>0){
      ALOG_DBG(PSTR("parsestart Segment Range [%d,%d] "), segments[0].pixel_range.start, segments[0].pixel_range.stop);
      segments[0].rgbcctcolors[0].debug_print("Segment 0 colour");
    }

    if(segments.size()>1){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[1].pixel_range.start, segments[1].pixel_range.stop);
      segments[1].rgbcctcolors[0].debug_print("Segment 1 colour");
    }

    if(segments.size()>2){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[2].pixel_range.start, segments[2].pixel_range.stop);
      segments[2].rgbcctcolors[0].debug_print("Segment 2 colour");
    }

    if(segments.size()>3){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[3].pixel_range.start, segments[3].pixel_range.stop);
      segments[3].rgbcctcolors[0].debug_print("Segment 3 colour");
    }

  // JsonParserArray arrobj = jtok;
  // for(auto v : arrobj) 
  // {
  //   // bool result = v.isObject();
  //   Serial.println(v.isObject());

  
  

    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(0).rgbcctcolors[0].getHue360());



  //   parsesub_json_object_notification_shortcut()

  //   JsonParserObject obj1 = v.getObject();
    JsonParserToken jtok1 = 0;


    // JsonParserObject obj1 = v.getObject();
    // JsonParserToken jtok1 = 0;

  uint16_t segment_index = 0;
  if(jtok1 = obj["Segment"])
  {
    std::vector<uint16_t> data;
    for(auto v : jtok1.getArray()) 
    {
      data.push_back(v.getInt());
    }
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);

    segment_index = data[0];//jtok1.getInt();
    ALOG_INF(PSTR("SegIndex segment_index===============================%d"), segment_index);

    // Create segment if needed
    // Segment_AppendNew(data[1],data[2],data[0]);

    if(segment_index > pCONT_lAni->segments.size()-1)
    { 
      ALOG_HGL(PSTR("Creating new segment %d|%d"), segment_index, pCONT_lAni->segments.size());
    segments[0].rgbcctcolors[0].debug_print(">>>>>>>>>>>>>>>>>>pre segment Segment 0 colour");
      Segment_AppendNew(data[1],data[2], segment_index+1);
    segments[0].rgbcctcolors[0].debug_print("<<<<<<<<<<<<<<<<<<post segment Segment 0 colour");
    }


    // Just in case it was made already, set the range directly
    SEGMENT_I(segment_index).pixel_range.start = data[1];
    SEGMENT_I(segment_index).pixel_range.stop = data[2];

  }


  if(jtok1 = obj["Params"])
  {
    std::vector<uint16_t> data;
    for(auto v : jtok1.getArray()) 
    {
      data.push_back(v.getInt());
    }
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    Serial.println(data[4]);

    CommandSet_Flasher_FunctionID(data[0], segment_index);

    SEGMENT_I(segment_index).user_params.val0 = data[1];
    SEGMENT_I(segment_index).user_params.val1 = data[2];
    SEGMENT_I(segment_index).user_params.val2 = data[3];
    SEGMENT_I(segment_index).user_params.val3 = data[4];
  }
  
  if(jtok1 = obj["Colour"])
  {
    std::vector<uint16_t> data;
    for(auto v : jtok1.getArray()) {
      data.push_back(v.getInt());
    }
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    Serial.println(data[4]);

    ALOG_INF(PSTR("SegIndex Colour segment_index%d"),segment_index);

    /**
     * @brief To fix error with lack of proper config on new colour, in this case, clear it prior to setting
     **/
    // SEGMENT_I(segment_index).rgbcctcolors[0].setChannelsRaw(0,0,0,0,0);
    segments[0].rgbcctcolors[0].debug_print("pre colour Segment 0 colour");
    SEGMENT_I(segment_index).rgbcctcolors[0].setChannelsRaw(data[0],data[1],data[2],data[3],data[4]);
    segments[0].rgbcctcolors[0].debug_print("post colour Segment 0 colour");

    // CommandSet_SegColour_RgbcctColour_Hue_360                (data[0],                0, segment_index);
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(segment_index).rgbcctcolors[0].getHue360());
    // CommandSet_SegColour_RgbcctColour_Sat_255      (mapvalue (data[1], 0,100, 0,255), 0, segment_index);
    // CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue (data[2], 0,100, 0,255), 0, segment_index);
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).rgbcctcolors[0].getBrightnessRGB());
  }

  

    // Serial.println(SEGMENT_I(segment_index).rgbcctcolors[0].raw[0]);
    // Serial.println(SEGMENT_I(segment_index).rgbcctcolors[0].raw[1]);
    // Serial.println(SEGMENT_I(segment_index).rgbcctcolors[0].raw[2]);
    // Serial.println(SEGMENT_I(segment_index).rgbcctcolors[0].raw[3]);
    // Serial.println(SEGMENT_I(segment_index).rgbcctcolors[0].raw[4]);

    if(segments.size()>0){
      ALOG_DBG(PSTR("parse end Segment Range [%d,%d] "), segments[0].pixel_range.start, segments[0].pixel_range.stop);
      segments[0].rgbcctcolors[0].debug_print("Segment 0 colour");
    }

    if(segments.size()>1){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[1].pixel_range.start, segments[1].pixel_range.stop);
      segments[1].rgbcctcolors[0].debug_print("Segment 1 colour");
    }

    if(segments.size()>2){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[2].pixel_range.start, segments[2].pixel_range.stop);
      segments[2].rgbcctcolors[0].debug_print("Segment 2 colour");
    }

    if(segments.size()>3){
      ALOG_DBG(PSTR("Segment Range [%d,%d] "), segments[3].pixel_range.start, segments[3].pixel_range.stop);
      segments[3].rgbcctcolors[0].debug_print("Segment 3 colour");
    }

}





void mAnimatorLight::CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index)
{
  
  SEGMENT_I(segment_index).colour_type = (RgbcctColor::ColourType)id;

  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_type);

}

const char* mAnimatorLight::GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen)
{
  // switch(id){  default:    
  
  //   case RgbcctColor::ColourType::LIGHT_TYPE__RGB__ID:    memcpy(buffer, "RGB", sizeof("RGB")); break;
  //   case RgbcctColor::ColourType::LIGHT_TYPE__RGBW__ID:    memcpy(buffer, "RGBW", sizeof("RGBW")); break;
  
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

  SEGMENT_I(segment_index).set_intensity(value);

}


void mAnimatorLight::CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index)
{

  SEGMENT_I(segment_index).set_speed(value);

}


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
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setHue360(hue_new);      
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setSat255(sat_new);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setCCT(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessRGB255(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessCCT255(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Manual(uint8_t* values, uint8_t value_count, uint8_t colour_index, uint8_t segment_index)
{
    
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setChannelsRaw(values);    // must be 5 for now, will consider less later

  #ifdef ENABLE_LOG_LEVEL_INFO
  char buffer[30];
  snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  #endif // ENABLE_LOG_LEVEL_DEBUG
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_LightSubType(uint8_t subtype, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
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

    // 


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




