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
    SEGMENT_I(0).flags.fForceUpdate = true;
  }

  uint8_t segments_found = 0;


  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer])
    { 

      ALOG_INF(PSTR("Seg: \"%s\" with %d Slots Active"), buffer, segments.size());

      /**
       * @brief Add check here that only sets the segment if it is already permitted
       * 
       */
    
        // ALOG_HGL(PSTR("CHECKING segment %d|%d"), segment_i, segments.size());

      // if(segment_i > segments.size()-1)
      if(segment_i >= segments.size())
      { 
        ALOG_HGL(PSTR("Creating new segment %d|%d"),segment_i,segments.size());
        // Segment_AppendNew(0, 0, segment_i+1);
        Segment_AppendNew(0, 100, segment_i); // STRIP_SIZE_MAX
        ALOG_HGL(PSTR("size check Creating new segment %d|%d"),segment_i,segments.size());
      }
      
      data_buffer.isserviced += subparse_JSONCommand(jtok.getObject(), segment_i);

      segments_found++;

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

  uint16_t isserviced_start_count = data_buffer.isserviced; // to know if anything was serviced in this function


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
   * @brief Exit if segment has not been created to stop errors
   * 
   */
  if(!segments.size())
  {
    ALOG_ERR(PSTR("Segment Index %d exceeds max %d"), segment_index, segments.size());    
    return 0;
  }


  if(jtok = obj["PaletteMappingValues"])
  { 
    if(jtok.isArray())
    {
      uint8_t array[16];
      uint8_t arrlen = 0;
      
      SEGMENT_I(segment_index).palette_container->mapping_values.clear(); // reset old map

      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) 
      {
        if(arrlen > 16){ break; }
        SEGMENT_I(segment_index).palette_container->mapping_values.push_back(v.getFloat());
        Serial.println(SEGMENT_I(segment_index).palette_container->mapping_values[arrlen]);        
        arrlen++;      
      }      
      data_buffer.isserviced++;
    }
  }

  
  if(jtok = obj[PM_JSON_COLOUR_PALETTE])
  {
    if(jtok.isStr())
    {
      if(strcmp(jtok.getStr(),"+")==0){
        SEGMENT_I(segment_index).palette_id++;
      }else
      if(strcmp(jtok.getStr(),"-")==0){
        SEGMENT_I(segment_index).palette_id--;
      }else
      if((tmp_id=GetPaletteIDbyName((char*)jtok.getStr()))>=0){
        ALOG_DBG(PSTR("tmp_id=%d"),tmp_id);
        CommandSet_PaletteID(tmp_id, segment_index);
        data_buffer.isserviced++;
        ALOG_DBG(PSTR("SEGMENT_I(segment_index).palette_id=%d"), SEGMENT_I(segment_index).palette_id);
      }
    }else
    if(jtok.isNum()){
      CommandSet_PaletteID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)) );
  }
  

  if(jtok = obj[D_JSON_PIXELRANGE])
  { 
    if(jtok.isArray())
    {
      JsonParserArray arrobj = jtok;
      SEGMENT_I(segment_index).start = arrobj[0].getInt();
      SEGMENT_I(segment_index).stop  = arrobj[1].getInt();
      
      if(SEGMENT_I(segment_index).stop > PIXEL_RANGE_LIMIT)
      {
        ALOG_ERR( PSTR("SEGMENT_I(segment_index).stop exceeds max %d %d"),SEGMENT_I(segment_index).stop, PIXEL_RANGE_LIMIT);
        SEGMENT_I(segment_index).stop = PIXEL_RANGE_LIMIT;
      }

      ALOG_COM( PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"), SEGMENT_I(segment_index).start, SEGMENT_I(segment_index).stop );
      data_buffer.isserviced++;
    }else{
      ErrorMessage_P(ERROR_MESSAGE_TYPE_INVALID_FORMAT, PM_JSON_PIXELRANGE);
    }
  }

  /*************************************************************************
   *** {"Effects":{X:Y}}
   *************************************************************************/
  if(jtok_sub = obj[PM_JSON_EFFECTS])
  {    
    JsonParserObject jobj = jtok_sub.getObject();

    if(jtok = jobj[PM_JSON_FUNCTION])
    { 
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


    if(jtok = jobj[PM_JSON_INTENSITY])
    { 
      CommandSet_Effect_Intensity(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_INTENSITY)), jtok.getInt() );
    }
    

    if(jtok = jobj[PM_JSON_SPEED])
    { 
      CommandSet_Effect_Speed(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_SPEED)), jtok.getInt() );
    }
    

    if(jtok = jobj[D_REVERSE])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_REVERSED, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_REVERSE)), SEGMENT_I(segment_index).reverse);
    }


    if(jtok = jobj[D_MIRROR])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_MIRROR, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_MIRROR)), SEGMENT_I(segment_index).mirror);
    }


    if(jtok = jobj[D_GROUPING])
    { 
      SEGMENT_I(segment_index).grouping = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
    }

    
    if(jtok = jobj[D_DECIMATE])
    {
      SEGMENT_I(segment_index).decimate = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_DECIMATE)), SEGMENT_I(segment_index).decimate);
    }


    if(jtok = jobj[D_SPACING])
    { 
      SEGMENT_I(segment_index).spacing = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_SPACING)), SEGMENT_I(segment_index).spacing);
    }
      

    if(jtok = jobj[PM_JSON_RATE]){ // default to secs
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
      data_buffer.isserviced++;
      ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RATE)), SEGMENT_I(segment_index).cycle_time__rate_ms);
    }else
    if(jtok = jobj[PM_JSON_RATE_MS]){
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
      ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms);  
    }

      
    if (jtok = jobj["Params"]) { 
      if (jtok.isArray()) {
        JsonParserArray arrobj = jtok;
        uint8_t array[4] = {0};  // Initialize the array to store values
        arrobj.getArray(array);  // Retrieve array elements

        // Assign retrieved values to SEGMENT_I params_user
        auto& params_user = SEGMENT_I(segment_index).params_user;
        std::copy(array, array + 4, params_user);

        ALOG_COM(PSTR(D_LOG_PIXEL "Effects.Params Segment[%d] = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
        data_buffer.isserviced++;
      }
    }else{
      char keyname[10];
      for(int i=0;i<4;i++)
      {
        snprintf(keyname, sizeof(keyname), "Param%d", i);
        if(jtok = jobj[keyname])
        {
          SEGMENT_I(segment_index).params_user[i] = jtok.getInt();
          data_buffer.isserviced++;
          ALOG_COM(PSTR(D_LOG_PIXEL "Effects.Params Segment[%d] = %d,%d,%d,%d"), segment_index, SEGMENT_I(segment_index).params_user[0], SEGMENT_I(segment_index).params_user[1], SEGMENT_I(segment_index).params_user[2], SEGMENT_I(segment_index).params_user[3]);
        }
      }
    }    

  } // PM_JSON_EFFECTS


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
        if(strcmp(jtok_sub.getStr(),"RGB")==0)
        {
          encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
        }
        else
        if(strcmp(jtok_sub.getStr(),"gRGB")==0)
        {
          encoding = {PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT};
        }
      }
      else
      {
        uint16_t enc = jtok_sub.getInt();
        encoding = {enc};
      }
      ALOG_DBG(PSTR("Encoding %d"), encoding);
    }

    if(jtok_sub = jtok.getObject()["Data"])
    {
      if(jtok_sub.isArray())
      {
        ALOG_DBM(PSTR("jtok_sub length %d"), jtok_sub.size());
        uint8_t data_length = jtok_sub.size() < 255 ? jtok_sub.size() : 0;
        uint8_t array[data_length];
        uint8_t arrlen = 0;
        JsonParserArray arrobj = jtok_sub;
        for(auto v : arrobj) 
        {
          if(arrlen > 255){ break; }
          array[arrlen++] = v.getInt();
        }
        CommandSet_CustomPalette(index, encoding.data, array, data_length);
        data_buffer.isserviced++;
      }
    }
  }
  
  

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

    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(0).rgbcctcolors[0].getHue360());
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(0).rgbcctcolors[0].getBrightnessRGB());
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(1).rgbcctcolors[0].getHue360());
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(1).rgbcctcolors[0].getBrightnessRGB());


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
  #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
  
  
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

  #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  // Hue// Sat// ColourHeatMap:[style, cold_point, now_temp, hot_point]
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
  #endif // ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  

  if(jtok = obj["Override"].getObject()["Animation"].getObject()["TimeMs"])
  {  
    SEGMENT_I(segment_index).single_animation_override.time_ms = jtok.getInt();
  }

  
  if(jtok = obj["Override"].getObject()["Animation_Off"].getObject()["TimeMs"])
  {
    SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms = jtok.getInt();
    ALOG_INF(PSTR("single_animation_override_turning_off=%d"),SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms);
  }


  if(jtok = obj["Transition"].getObject()["RateMs"])
  {
    ALOG_ERR(PSTR("$.Transition.RateMs is now $.Effects.RateMs"));
  }
  

  /**
   * @brief Construct a new if object
   * Although egcolour needs to know this for the internal colour mapping, it is also needed to properly get the effects working. Should be clariefied with a better name later. ColourType should rename internal 
   * and not directly set by the user, but instead the colour order (ie type depends on what is set). ColourType should remain internal.
   * ColourType (to be renamed) for the effect itself, needs to instead have RGB, RGBW etc
   **/    
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
    ALOG_WRN(PSTR("PM_JSON_COLOUR_TYPE is in conflict with subtype, likely this needs to be the main command and subtype set internally only with no external command"));
    // ALOG_COM(PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer))); // should be internal to rgbcct
  }

  
  /**
   * @brief Segment colours
   **/
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
    // CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
    SEGMENT_I(segment_index).setBrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255));
    data_buffer.isserviced++;
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB_255]){
    SEGMENT_I(segment_index).setBrightnessRGB( jtok.getInt() );
    // CommandSet_SegColour_RgbcctColour_BrightnessRGB(jtok.getInt(), colour_index, segment_index);
    data_buffer.isserviced++;
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
  }


  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
    SEGMENT_I(segment_index).setBrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255));
    // CommandSet_SegColour_RgbcctColour_BrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
    data_buffer.isserviced++;
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // Exact kelvin
    SEGMENT_I(segment_index).setBrightnessCCT( jtok.getInt() );
    // CommandSet_SegColour_RgbcctColour_BrightnessCCT(jtok.getInt(), colour_index, segment_index);
    data_buffer.isserviced++;
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
  }
  
  // Are these commands the same?
  if(jtok = obj["TimeOnSecs"]){
    SEGMENT_I(segment_index).auto_timeoff.Start(jtok.getInt());
    ALOG_INF(PSTR("auto_timeoff %d"), SEGMENT_I(segment_index).auto_timeoff.Value());
  }
  if(jtok = obj["Light"].getObject()[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    data_buffer.isserviced++;
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs ); 
  }



  /**
   * @brief Segment Colours (ie 1-5)
   **/
  for(uint8_t colour_index=0;colour_index<6;colour_index++)
  {
    JsonParserObject seg_obj = 0;

    snprintf(buffer, sizeof(buffer), "SegColour%d", colour_index);

    // ALOG_HGL(PSTR("SEGMENT_I(segment_index).rgbcctcolors[colour_index] length %d"), RGBCCTCOLOURS_SIZE);    

    if(seg_obj = obj[buffer].getObject())
    {
      
      ALOG_COM(PSTR(D_LOG_PIXEL "SegColour[%d] = %s"), colour_index, buffer);

      // Direct way, used mostly for storage
      if(jtok = seg_obj["RGBWC"]){ 
        
          
        if(jtok.isArray()){
          uint8_t array[5];
          uint8_t arrlen = 0;
          JsonParserArray arrobj = jtok;
          for(auto v : arrobj){
            if(arrlen > 5){ break; }
            array[arrlen++] = v.getInt();
          }
          SEGMENT_I(segment_index).rgbcctcolors[colour_index] = RgbcctColor(array[0],array[1],array[2],array[3],array[4]);
        }

        data_buffer.isserviced++;
        // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K("Raw")), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getHue360());
      }
    
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
        ALOG_ERR(PSTR("D_JSON_SUBTYPE removed"));
      }
      
    
      if(jtok = seg_obj["Manual"]){ // Needs renamed, setting colour RGBCW directly

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


  #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER

  if(jtok = obj["Sequencer"].getObject()["LoadSet"])
  {
    Load_Sequencer(jtok.getInt());
  }
  if(jtok = obj["Sequencer"].getObject()["EnableTimeRestraints"])
  {
    sequencer_runtime.Enable_TimeRestraints = jtok.getInt();
  }
  if(jtok = obj["Sequencer"].getObject()["ItemTime"])
  {
    SetSequenceTimes(jtok.getInt());
  }  

  if(jtok = obj["Sequencer"].getObject()["LimitFlashing"])
  {
    sequencer_runtime.remote_openhab_limit_flashing = jtok.getInt();
  }  
  #endif // ENABLE_FEATURE_LIGHTING__SEQUENCER


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
    uint16_t stop = 100;//STRIP_SIZE_MAX;

    Serial.println();

    ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

    Segment_AppendNew(start, stop, jtok.getInt());

    // for (size_t s = 0; s < getSegmentsNum(); s++) {
    //     Segment &sg = getSegment(s);
    //     
    //     // if (sg.isSelected()) 
    //     // {

    //       
    //       // deserializeSegment(segVar, s, presetId);

    //       Segment& seg = getSegment(s);
    //       Segment prev = seg; //make a backup so we can tell if something changed

    //       // if using vectors use this code to append segment
    //       if (id >= getSegmentsNum()) {
    //         
    //         if (stop <= 0) return 0; // ignore empty/inactive segments
    //         appendSegment(Segment(0+id, getLengthTotal()));
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

    segments[jtok.getInt()].stop = 0; // to disable segment

    ALOG_INF(PSTR("getSegmentsNum() %d|%d %d"), id, getSegmentsNum(), segments[jtok.getInt()].stop);

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

    segments[jtok.getInt()].stop = 0; // to disable segment

    ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

    // remove all inactive segments (from the back)
    int deleted = 0;
    if (segments.size() <= 1) return 0;
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

// purgeSegments


  }



  if(jtok = obj["Debug"])
  {

    JsonParserObject jObj_debug = jtok.getObject();

    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option0"])
    { 


      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).params_internal.aux0 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option1"])
    { 


      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).params_internal.aux1 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }

    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option2"])
    { 
      // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).params_internal.aux2 = jtok.getInt();  
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    if(jtok = jObj_debug[PM_JSON_EFFECTS].getObject()["Option3"])
    {     // ALOG_INF( PSTR("Grouping %d %d"), jtok.getInt(), segment_index );
      SEGMENT_I(segment_index).params_internal.aux3 = jtok.getInt();  
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
            case 0: SEGMENT_I(segment_index).params_internal.aux0 = v.getInt(); break;
            case 1: SEGMENT_I(segment_index).params_internal.aux1 = v.getInt(); break;
            case 2: SEGMENT_I(segment_index).params_internal.aux2 = v.getInt(); break;
            case 3: SEGMENT_I(segment_index).params_internal.aux3 = v.getInt(); break;
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
            SEGMENT_I(segment_index).params_internal.aux0,
            SEGMENT_I(segment_index).params_internal.aux1,
            SEGMENT_I(segment_index).params_internal.aux2,
            SEGMENT_I(segment_index).params_internal.aux3
          );
      //   if(SEGMENT_I(segment_index).stop > STRIP_SIZE_MAX+1)
      //   {
      // #ifdef ENABLE_LOG_LEVEL_ERROR
      //     AddLog(LOG_LEVEL_ERROR, PSTR("SEGMENT_I(segment_index).stop exceeds max %d %d"),SEGMENT_I(segment_index).stop, STRIP_SIZE_MAX);
      // #endif //ef ENABLE_LOG_LEVEL_INFO
      //    SEGMENT_I(segment_index).stop = STRIP_SIZE_MAX+1;
      //   }


        
        data_buffer.isserviced++;
      }
    }

    

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
    


  } // end Debug options

#endif // ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS options above to be moved into debug methods

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
  
} // END PARSE COMMANDS


        









void mAnimatorLight::CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index)
{
  
  SEGMENT_I(segment_index).colour_type__used_in_effect_generate = (ColourType)id;

  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_type__used_in_effect_generate);

}

const char* mAnimatorLight::GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen)
{
  // switch(id){  default:      
  //   case RgbcctColor::ColourType::LIGHT_TYPE__RGB__ID:    memcpy(buffer, "RGB", sizeof("RGB")); break;
  //   case RgbcctColor::ColourType::LIGHT_TYPE__RGBW__ID:    memcpy(buffer, "RGBW", sizeof("RGBW")); break;  
  // }
  return buffer;
}

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
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
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

void mAnimatorLight::CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length)
{
  mPaletteI->addCustomPalette(index, data, data_length, encoding);
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_PaletteID(uint8_t value, uint8_t segment_index)
{

  char buffer[50];

  SEGMENT_I(segment_index).palette_id = value < mPaletteI->GetPaletteListLength() ? value : 0;
  
  _segment_index_primary = segment_index;
  SEGMENT.LoadPalette(segments[segment_index].palette_id);

  // //If "id" is in the range of rgbcct, make sure to automatically make internal_rgbctt track it
  // if((value>=mPaletteI->PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID)
  // &&(value<mPaletteI->PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID))
  // {
  //   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(value, segment_index);
  // }

  // ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), mPaletteI->GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)));
  
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Time_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
// YES , remove logging inside CommandSet (just leave in json parse) so internal use will not always use serial
void mAnimatorLight::CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index){
    
  #ifndef ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
  SEGMENT_I(segment_index).time_ms = value;

  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)), SEGMENT_I(segment_index).time_ms );  
  #endif

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Rate_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index){
    
  SEGMENT_I(segment_index).cycle_time__rate_ms = value;

  #ifndef ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
  if(SEGMENT_I(segment_index).cycle_time__rate_ms < SEGMENT_I(segment_index).time_ms){ 
    SEGMENT_I(segment_index).time_ms = SEGMENT_I(segment_index).cycle_time__rate_ms;
  }
  #endif

  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms );  

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
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setCCT_Kelvin(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessRGB(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessCCT(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Manual(uint8_t* values, uint8_t value_count, uint8_t colour_index, uint8_t segment_index)
{
    
  // if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  // SEGMENT_I(segment_index).rgbcctcolors[colour_index].setChannelsRaw(values);    // must be 5 for now, will consider less later

  #ifdef ENABLE_LOG_LEVEL_INFO
  char buffer[30];
  snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  #endif // ENABLE_LOG_LEVEL_DEBUG
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
    // CommandSet_Animation_Transition_Rate_Ms(10000);
    SEGMENT_I(0).intensity = 255;
    
    SEGMENT_I(0).single_animation_override.time_ms =  SEGMENT_I(0).single_animation_override_turning_off.time_ms; // slow turn on

    ALOG_INF(PSTR("Setting override for off %d"), SEGMENT_I(0).single_animation_override.time_ms);

    SEGMENT_I(0).flags.fForceUpdate = true;

    pCONT_iLight->CommandSet_Brt_255(0);
    
  }
  else
  if(state == 1) // turn on
  {

    // CommandSet_Animation_Transition_Time_Ms(1000);

    SEGMENT_I(0).single_animation_override.time_ms = 1000; // slow turn on
    SEGMENT_I(0).flags.fForceUpdate = true;


    // CommandSet_Animation_Transition_Rate_Ms(1000);
    // CommandSet_LightsCountToUpdateAsPercentage(100);
    
    pCONT_iLight->CommandSet_Brt_255(255);

    //make sure both are set
    // CommandSet_Global_BrtRGB_255(255);
    // CommandSet_Global_BrtCCT_255(255);
    
    // CommandSet_PaletteID(10, 0);
    
    // CommandSet_Flasher_FunctionID(0 /**Add define later for "DEFAULT_EFFECT" */);//EFFECTS_FUNCTION__SOLID_COLOUR__ID);



  }

}


bool mAnimatorLight::CommandGet_LightPowerState()
{
  return 
  light_power_state
  // getBri_Global() 
  ? true : false;
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Auto_Time_Off_Secs *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Auto_Time_Off_Secs(uint16_t value){
    
  auto_off_settings.time_decounter_secs = value;

}


#endif //USE_MODULE_LIGHTS_ANIMATOR




