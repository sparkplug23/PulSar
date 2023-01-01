#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;

/**
 * @brief 
 * Animator should NOT have its own Tasker, but isntead should be called from light interface
 * 
 * @param function 
 * @param obj 
 * @return int8_t 
 */

int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
      break;
    case FUNC_INIT:
      Init();
      break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
      
      //EverySecond();

      // const NeoRgbcctCurrentSettings settings(200,200,200,201,202);
      // uint32_t maTotal = stripbus->CalcTotalMilliAmpere(settings);

      
//       ALOG_INF(PSTR("raw=%d"),strip->_segments_new[0].rgbcctcolors[0].raw[0]); 
//       ALOG_INF(PSTR("size=%d"),strip->_segments_new[0].rgbcctcolors.size()); 


// DEBUG_LINE_HERE;
//       strip->_segments_new[0].rgbcctcolors.push_back(RgbcctColor(11,12,13,14,15));

// DEBUG_LINE_HERE;



    }break;
    case FUNC_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
     parse_JSONCommand(obj);
    break;
    case FUNC_WIFI_CONNECTED:
      #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
      Test_Config();
      #endif
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
      CommandSet_Physical_UserInput_Buttons();
      #endif
    break;
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }// switch(command)

  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END FUNCTION




/*******************************************************************************************************************
********************************************************************************************************************
************ START OF GENERAL INIT PROCESS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mAnimatorLight::Pre_Init(void){ 
  
  // Allow runtime changes of animation size
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

  /**
   * @brief Get pins here
   **/
  #ifdef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

    int8_t pin_data = -1, pin_clock = -1;

    // for(
      uint8_t ii=0; 
      // ii<1; ii++)
    // {
      if(pCONT_pins->PinUsed(GPIO_RGB_DATA_ID, ii))
      {
        pin_data = pCONT_pins->GetPin(GPIO_RGB_DATA_ID, ii);
      }

      // if(pCONT_pins->PinUsed(GPIO_RGB_CLOCK1_ID, ii))
      // {
      //   pin_clock = pCONT_pins->GetPin(GPIO_RGB_DATA1_ID, ii);
      // }

      // Stripbus
      if(pin_data != -1)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("NeoPixelBus: Segment[%d] Pin[%d]"), 0, pin_data);
        stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, pin_data);
      }

    // }

  #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT


    #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    pCONT_ladd->neopixel_runner = new NeoPixelShowTask();///* neopixel_runner = nullptr;
    // constexpr
    // uint8_t kTaskRunnerCore = ARDUINO_RUNNING_CORE;//xPortGetCoreID(); // 0, 1 to select core
    uint8_t kTaskRunnerCore = 1;//ARDUINO_RUNNING_CORE;//xPortGetCoreID(); // 0, 1 to select core // > 1 to disable separate task
    pCONT_ladd->neopixel_runner->begin(
        [this]() {
            stripbus->Show();
        },
        kTaskRunnerCore
    );
    #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

  #ifdef ENABLE_DEVFEATURE_WS2812FX_SEGMENT_CONSTRUCTOR
  strip = new mAnimatorLight::WS2812FX();
  ALOG_INF(PSTR("SET strip = new WS2812FX()")); 
  strip->finalizeInit();
  strip->makeAutoSegments();
  #endif // ENABLE_DEVFEATURE_WS2812FX_SEGMENT_CONSTRUCTOR
  
    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

    busses = new BusManager();


    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  

}


#ifndef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 
void mAnimatorLight::Init_NeoPixelBus(int8_t pin)
{

  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_SIZE_MAX;
  #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;
  #endif
  // uint16_t strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  
  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime


#ifdef ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE

// future methods will need to parse on esp8266 and only permit certain pins, or if very low pixel count, software versions

  stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, PINSET_TEMP_METHOD_RGB_PIN_RGB);//19); 3 = rx0



#else //legacy method



  stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
  
  #endif

}
#endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT




void mAnimatorLight::Init(void){ 
    
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX;

  // stripbus->NeoRgbCurrentSettings(1,2,3);
  
  /**
   * @brief Start PixelAnimator
   * 
   */
  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE);     // I dont think this is the active one????????????????????????

  /**
   * @brief Start NeoBus
   * 
   */

  #ifndef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 
  Init_NeoPixelBus();
  #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 
  if(stripbus == nullptr)
  {
    // not configured, no pin? disable lighting
    AddLog(LOG_LEVEL_ERROR, PSTR("stripbus == nullptr, no pin or stripbus set, setting default as temporary fix"));
    settings.flags.EnableModule = false;

    /**
     * @brief 
     * Temp fix to resolve crashing errors due to stripbus being called from interface_light without setting
     * Default values
     */
    #ifdef ESP8266
    stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, 3);
    #else
    stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, 23);
    #endif
  }
  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    // stripbus->ClearTo(0);
    pCONT_iLight->ShowInterface();
  }

  /**
   * @brief Start PaletteContainer
   * 
   */
  #ifdef ENABLE_DEVFEATURE_NEWPALETTE_CONTAINER_POINTER
  ALOG_WRN(PSTR("minimum to get this working, but should still be made internal/auto the segment being produced"));
  SEGMENT_I(0).palette_container = new mPaletteContainer(100);
  #endif // ENABLE_DEVFEATURE_NEWPALETTE_CONTAINER_POINTER

  /**
   * @brief Init Types of Modes
   * 
   */  
  Init_Segments();
  #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
    init_Notifications();
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
  init_mixer_defaults();
  #endif
  #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  PhysicalController_Convert_IDs_to_Internal_Parameters();
  #endif

  settings.flags.EnableModule = true;

} //end "init"

/**
 * @brief Now, do I use "Save" going forward as simple byte packed struct, or, saving them as json encoded in memory (much larger, but more dynamic)
 * 
 */
void mAnimatorLight::Settings_Load(){
    
  pCONT_iLight->setBriRGB_Global(pCONT_set->Settings.light_settings.light_brightness_as_percentage);

#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("LOADED user_input.selected.palette_id %d"),user_input.selected.palette_id);
#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}

void mAnimatorLight::Settings_Save(){
  
  
#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;

  // PhysicalController_Convert_IDs_to_Internal_Parameters();

  pCONT_set->Settings.animation_settings.xmas_controller_params[5] = 6;

#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}



void mAnimatorLight::Settings_Default(){

  init(); //default values

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("mAnimatorLight::Settings_Default"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
    settings.flags.TemplateProvidedInProgMem = true;
  #endif
  
  if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
    //pCONT_iLight->Template_Load();

  }else{

  }

  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  /**
   * @brief To be included in 2023
   * 
   * Alternate of grouping from WLED, I also want a "dynamic_group" that uses an array to set the group width.
   * 
   * ie is group is set, and group_map is provided, then widths from group_map will be set for each group [1,2,3] will become [1, 2,2, 3,3,3] for a total of 14 pixels set
   * 
   */
  memset(&editable_mapped_array_data_array,0,sizeof(editable_mapped_array_data_array));
  
  uint16_t single_row_count_array[] = {
    31,35,39,42,43,//31,44,39,42,43,
    43,43,42,42,42,
    41,39,38,36,33,
    30,29,28,25,25,
    23,22,22,20,18,
    19,17,18,16,14,
    14,13,10,10,11,
    11,8,4,4,5,6,6,
    5,5,5,5,5,
    0,3
  }; // 0 is filled last
  uint8_t single_row_count_array_size = 49;//sizeof(single_row_count_array)/single_row_count_array[0];

  // Serial.println(single_row_count_array_size);
  // delay(5000);
  
  uint16_t count = 0;
  for(int i=0;i<single_row_count_array_size;i++){
    count += single_row_count_array[i];
  }
  single_row_count_array[single_row_count_array_size-2] = 1050-count; // use 2nd last one as filler
  

  memcpy(editable_mapped_array_data_array, single_row_count_array, sizeof(single_row_count_array));


  pixel_group.mapped_array_data.length = ArrayCountUntilNull(editable_mapped_array_data_array, (uint8_t)D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH); //values before 0

DEBUG_LINE;
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING


}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************* Helper Functions                  ************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

const char* mAnimatorLight::GetAnimationStatusCtr(char* buffer, uint8_t buflen){

  if(pCONT_iLight->animator_controller->IsAnimating()){
    snprintf_P(buffer, buflen, PSTR("Animating"));
    return buffer;
  }
  if(SEGMENT_I(0).flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int16_t until_next_millis = SEGMENT_I(strip->_segment_index_primary).transition.rate_ms-(millis()-pCONT_iLight->runtime.animation_changed_millis);
    int16_t secs_until_next_event = until_next_millis/1000;
    // secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    // char float_ctr[10];
    // dtostrf(round(secs_until_next_event),3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),pCONT_iLight->runtime.animation_changed_millis,pCONT_iLight->animation.transition.rate_ms,
    //   secs_until_next_event
    // );  
    
    if(secs_until_next_event>=0){
      snprintf_P(buffer, buflen, PSTR("Enabled (in %d secs)"), secs_until_next_event);//float_ctr);
    }else{
      snprintf_P(buffer, buflen, PSTR("Not currently running"));
      //  sprintf(buffer,PSTR("Not currently running"));//float_ctr);
    }

    // sprintf(ctr,PSTR("Enabled (%d secs)\0"),secs_until_next_event);
    // sprintf(ctr,PSTR("Enabled (123 secs)\0"),secs_until_next_event);
    // Serial.println(ctr);
    return buffer;
  }else{
    return D_CSTRING_ERROR_MESSAGE_CTR;
    // return "Paused";
  }
  return D_CSTRING_ERROR_MESSAGE_CTR;
}



RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness){

  RgbcctColor colour_adjusted_with_brightness; // No init for speed// = RgbcctColor();//0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightness);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightness);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightness);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightness);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightness);
  return colour_adjusted_with_brightness;

}

RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT){

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightnessCCT);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightnessCCT);
  return colour_adjusted_with_brightness;

}


RgbcctColor mAnimatorLight::ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness){
  
  uint8_t new_brightness_255 = 0;

    new_brightness_255 = brightness;

  // AddLog(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(SEGMENT_I(0).flags.use_gamma_for_brightness)
  {
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoRgbcctColour(full_range_colour, new_brightness_255);

}



#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
mAnimatorLight& mAnimatorLight::setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE)
{
  this->anim_progress_mqtt_function_callback = anim_progress_mqtt_function_callback;
  return *this;
}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


void mAnimatorLight::StripUpdate(){

  pCONT_iLight->ShowInterface();

}



/**
 * @brief Function to load palette_id into the segment
 *   (lets just use largest possible byte value without making it dynamic for now)
 * 
 * For now, calling this function will take all data from the palette (rgb data) and 
 * move it into the array
 * 
 * Later, GetColourFromPreloadedPalette will retreive data from ram array, NEVER progmem
 * 
 * loadPalette 
 *  ** create buffer into heap, save pointer
 * 
 * unloadPalette
 *  ** delete buffer data from heap, reset to nullptr
 * 
 * should load even be here? surely into palette container
 * 
 */
void mAnimatorLight::loadPalette_Michael(uint8_t palette_id, uint8_t segment_index)
{

  // ALOG_INF(PSTR("loadPalette_Michael %d %d %d"), palette_id, segment_index, strip->_segment_index_primary);

  /**
   * @brief My palettes
   **/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_PARTY_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID)    && (palette_id < mPalette::PALETTELIST_VARIABLE_HSBID_LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID)  && (palette_id < mPalette::PALETTELIST_VARIABLE_GENERIC_LENGTH__ID))
  ){   
    mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);    
    SEGMENT_I(segment_index).palette_container->pData.assign(ptr->data, ptr->data + ptr->data_length);
    
    // ALOG_INF(PSTR("Direct load, move into container %d %d"), segment_index, ptr->data_length);
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
  ){  
      // Currently does not need to load
    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (palette_id < mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID)
  ){  

    /**
     * @brief These pri/sec/ter should really be merged into my palettes as another encoded type, thus, removing colors[x] from palette.cpp
     * 
     */
    switch(palette_id)
    {
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID: // Original WLED random
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).aux3 > new_colour_rate_ms)        
        {
          mPaletteI->targetPalette = CRGBPalette16(
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 192, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255)));
          SEGMENT_I(segment_index).aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_02__ID: // Random Hue, Slight Random Saturation (80 to 100%) ie 200/255 is 80%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).aux3 > new_colour_rate_ms)        
        {
          mPaletteI->targetPalette = CRGBPalette16(
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255)
                          );
                          
          SEGMENT_I(segment_index).aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_03__ID: // S60-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).aux3 > new_colour_rate_ms)        
        {
          mPaletteI->targetPalette = CRGBPalette16(
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255)
                          );
                          
          SEGMENT_I(segment_index).aux3 = millis();
        }
      }
      break;

      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_04__ID: // S60-S85%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).aux3 > new_colour_rate_ms)        
        {
          mPaletteI->targetPalette = CRGBPalette16(
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255)
                          );
                          
          SEGMENT_I(segment_index).aux3 = millis();
        }
      }
      break;

      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_05__ID: // S0-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).aux3 > new_colour_rate_ms)        
        {
          mPaletteI->targetPalette = CRGBPalette16(
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255)
                          );
                          
          SEGMENT_I(segment_index).aux3 = millis();
        }
      }
      break;

    
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID: 
      { //primary color only
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0])); //is this stable to do? maybe since its not a pointer but instead an instance of a class
        mPaletteI->targetPalette = CRGBPalette16(prim); 
        // length = 1;
      }
      break;
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY__ID:
      { //primary + secondary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        mPaletteI->targetPalette = CRGBPalette16(prim,prim,sec,sec); 
        // length = 4;
      }
      break;
      case mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID:
      { //primary + secondary + tertiary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        CRGB ter  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
        mPaletteI->targetPalette = CRGBPalette16(ter,sec,prim); 
        // length = 3; // 3 unique colours
      }
      break;    
      case  mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID:
      { //primary + secondary (+tert if not off), more distinct
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        if (RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]))
        {
          CRGB ter = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
          mPaletteI->targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
        } else {
          mPaletteI->targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
        }
        // length = 16;
      }
      break;

    } //end switch
    
    mPaletteI->currentPalette = mPaletteI->targetPalette; // This should be returned!
    // ALOG_INF(PSTR("LOADED currentPalette"));

  }
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   
    mPaletteI->LoadPalette_CRGBPalette16_Static(); // ie WLED22 "loadPalette"
  }

}


#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


//get RGB values from color temperature in K (https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html)
void mAnimatorLight::colorKtoRGB(uint16_t kelvin, byte* rgb) //white spectrum to rgb, calc
{
  float r = 0, g = 0, b = 0;
  float temp = kelvin / 100;
  if (temp <= 66) {
    r = 255;
    g = round(99.4708025861 * log(temp) - 161.1195681661);
    if (temp <= 19) {
      b = 0;
    } else {
      b = round(138.5177312231 * log((temp - 10)) - 305.0447927307);
    }
  } else {
    r = round(329.698727446 * pow((temp - 60), -0.1332047592));
    g = round(288.1221695283 * pow((temp - 60), -0.0755148492));
    b = 255;
  } 
  //g += 12; //mod by Aircoookie, a bit less accurate but visibly less pinkish
  rgb[0] = (uint8_t) constrain(r, 0, 255);
  rgb[1] = (uint8_t) constrain(g, 0, 255);
  rgb[2] = (uint8_t) constrain(b, 0, 255);
  rgb[3] = 0;
}

// adjust RGB values based on color temperature in K (range [2800-10200]) (https://en.wikipedia.org/wiki/Color_balance)
uint32_t mAnimatorLight::colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb)
{
  //remember so that slow colorKtoRGB() doesn't have to run for every setPixelColor()
  if (lastKelvin != kelvin) colorKtoRGB(kelvin, correctionRGB);  // convert Kelvin to RGB
  lastKelvin = kelvin;
  byte rgbw[4];
  rgbw[0] = ((uint16_t) correctionRGB[0] * R(rgb)) /255; // correct R
  rgbw[1] = ((uint16_t) correctionRGB[1] * G(rgb)) /255; // correct G
  rgbw[2] = ((uint16_t) correctionRGB[2] * B(rgb)) /255; // correct B
  rgbw[3] =                                W(rgb);
  return RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]);
}

//approximates a Kelvin color temperature from an RGB color.
//this does no check for the "whiteness" of the color,
//so should be used combined with a saturation check (as done by auto-white)
//values from http://www.vendian.org/mncharity/dir3/blackbody/UnstableURLs/bbr_color.html (10deg)
//equation spreadsheet at https://bit.ly/30RkHaN
//accuracy +-50K from 1900K up to 8000K
//minimum returned: 1900K, maximum returned: 10091K (range of 8192)
uint16_t mAnimatorLight::approximateKelvinFromRGB(uint32_t rgb) {
  //if not either red or blue is 255, color is dimmed. Scale up
  uint8_t r = R(rgb), b = B(rgb);
  if (r == b) return 6550; //red == blue at about 6600K (also can't go further if both R and B are 0)

  if (r > b) {
    //scale blue up as if red was at 255
    uint16_t scale = 0xFFFF / r; //get scale factor (range 257-65535)
    b = ((uint16_t)b * scale) >> 8;
    //For all temps K<6600 R is bigger than B (for full bri colors R=255)
    //-> Use 9 linear approximations for blackbody radiation blue values from 2000-6600K (blue is always 0 below 2000K)
    if (b < 33)  return 1900 + b       *6;
    if (b < 72)  return 2100 + (b-33)  *10;
    if (b < 101) return 2492 + (b-72)  *14;
    if (b < 132) return 2900 + (b-101) *16;
    if (b < 159) return 3398 + (b-132) *19;
    if (b < 186) return 3906 + (b-159) *22;
    if (b < 210) return 4500 + (b-186) *25;
    if (b < 230) return 5100 + (b-210) *30;
                 return 5700 + (b-230) *34;
  } else {
    //scale red up as if blue was at 255
    uint16_t scale = 0xFFFF / b; //get scale factor (range 257-65535)
    r = ((uint16_t)r * scale) >> 8;
    //For all temps K>6600 B is bigger than R (for full bri colors B=255)
    //-> Use 2 linear approximations for blackbody radiation red values from 6600-10091K (blue is always 0 below 2000K)
    if (r > 225) return 6600 + (254-r) *50;
    uint16_t k = 8080 + (225-r) *86;
    return (k > 10091) ? 10091 : k;
  }
}

#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


#endif //USE_MODULE_LIGHTS_ANIMATOR




