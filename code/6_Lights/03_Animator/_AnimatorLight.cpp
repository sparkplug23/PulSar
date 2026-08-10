#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
mAnimatorLight* tkr_extern_lAni = nullptr; // Define the global instance
#endif

int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = TASKER_RESULT__SUCCESS_ID;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();  
    break;
    case TASK_INIT:
      Init();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }
  
  switch(function){
    /************
     * SYSTEM SECTION * 
    *******************/    
    case TASK_RESTART_SET_DO_FINAL_CLEANUP: 
      #ifdef ENABLE_FEATURE_LIGHTING__WEBUI      
      websocket_lights->closeAll(1012);
      #endif
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:{
      EverySecond_AutoOff(); 

      #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
      updateInterfaces(CALL_MODE_WS_SEND); //tmp fix sending here
      #endif 

      #ifdef ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
      Serial.printf_P(PSTR("FPS: %f\n\r"), getFpsFloat());
      #endif

      #ifdef ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
      EverySecond_Standby();
      #endif

    }break;
    case TASK_LOOP:
      EveryLoop();
    break;     
    case TASK_EVERY_250_MSECOND:
    break;   
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__HANDLE_FILE_CHANGES_FROM_EDIT_URL__ID:
      Handle_FileSave_Edits();
    break;
    #endif
    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
    #ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
    KeyInput__ControlLights();
    #endif
    break;
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif
    /************
     * WEBUI SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif
    #endif
  }

  return function_result;

} // END FUNCTION


void mAnimatorLight::updatePixelBuffer()
{
  uint32_t requiredMem = getLengthTotal() * sizeof(uint32_t);

  ALOG_INF(PSTR("UPDATE START: global=%p required=%u"), (void*)_pixels, requiredMem);

  for (uint8_t seg_i = 0; seg_i < segments.size(); seg_i++)
  {
    ALOG_INF(PSTR("BEFORE FREE: seg=%u pixels=%p len=%u global=%p"), seg_i, (void*)segments[seg_i].getPixels(), segments[seg_i].length(), (void*)_pixels);
  }

  p_free(_pixels);
  _pixels = nullptr;

  for (uint8_t seg_i = 0; seg_i < segments.size(); seg_i++)
  {
    ALOG_INF(PSTR("AFTER FREE: seg=%u pixels=%p len=%u"), seg_i, (void*)segments[seg_i].getPixels(), segments[seg_i].length());
  }

  _pixels = static_cast<uint32_t*>(
    allocate_buffer(
      requiredMem,
      BFRALLOC_ENFORCE_PSRAM |
      BFRALLOC_NOBYTEACCESS |
      BFRALLOC_CLEAR
    )
  );

  _pixels_length = requiredMem;

  ALOG_INF(PSTR("AFTER ALLOC: global=%p required=%u"), (void*)_pixels, requiredMem);

  for (uint8_t seg_i = 0; seg_i < segments.size(); seg_i++)
  {
    Segment& seg = segments[seg_i];

    const uintptr_t srcStart = reinterpret_cast<uintptr_t>(seg.getPixels());
    const uintptr_t srcEnd   = srcStart + seg.length() * sizeof(uint32_t);
    const uintptr_t dstStart = reinterpret_cast<uintptr_t>(_pixels);
    const uintptr_t dstEnd   = dstStart + requiredMem;

    const bool overlap = srcStart < dstEnd && dstStart < srcEnd;
    const intptr_t delta = static_cast<intptr_t>(dstStart) - static_cast<intptr_t>(srcStart);

    ALOG_INF(PSTR("AFTER ALLOC CHECK: seg=%u src=%p dst=%p deltaBytes=%d deltaPixels=%d overlap=%u"), seg_i, (void*)srcStart, (void*)dstStart, (int)delta, (int)(delta / sizeof(uint32_t)), overlap);
  }
}


void mAnimatorLight::Handle_FileSave_Edits()
{
  // If nothing pending, bail early (paranoid guard; optional if caller already checks)
  if (!SPIFFSEditor::Check_AnyFilesEdited()) return;

  if (SPIFFSEditor::Check_FileEditedIs(F("presets.json"))) {
    static bool use_new_parser = false;   // toggles each time

    uint32_t t_start = millis();

    // if (!use_new_parser) {
    //   ALOG_INF(PSTR("presets.json updated, running ScanPresetsFile_GeneratePlaylistIDsFromPSN()"));
    //   ScanPresetsFile_GeneratePlaylistIDsFromPSN();
    // } else {
      ALOG_INF(PSTR("presets.json updated, running ScanPresetsFile_GeneratePlaylistIDsFromPSN_2()"));
      ScanPresetsFile_GeneratePlaylistIDsFromPSN_2();
    // }

    uint32_t t_elapsed = millis() - t_start;
    ALOG_INF(PSTR("Playlist rescan (%s) took %u ms"),
             use_new_parser ? PSTR("v2") : PSTR("v1"),
             (unsigned)t_elapsed);

    use_new_parser = !use_new_parser; // toggle for next time
  }

  // Future:
  // if (SPIFFSEditor::Check_FileEditedIs(F("some_other.json"))) { ... }
}


#ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
/**
 * @brief KeySet1: single button, palette and brightness
 * KeySet2: dual button (as??)
 * 
 */
void mAnimatorLight::KeyInput__ControlLights()
{
  ALOG_INF(PSTR("KeyInput__ControlLights"));

  if(!keyinput_control.mode) return;

  switch(keyinput_control.mode)
  {
    case 1:


    break; 
  }



}
#endif // ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS


void mAnimatorLight::Save_Module()
{

  // Should probably just use the serialiseState function here instead of duplicating code

  ALOG_INF(PSTR("mAnimatorLight::Save_Module()"));

  char buffer[120] = {0};

  /********************************************************************
   * Byte Data
   * ******************************************************************/


  /********************************************************************
   * Json Data
   * ******************************************************************/

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();

  #ifdef ENABLE_FEATURE_FILESYSTEM__ADD_TIMESTAMP_ON_SAVE_FILES
    JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str() );
  #endif // ENABLE_FEATURE_FILESYSTEM__ADD_TIMESTAMP_ON_SAVE_FILES

  uint8_t bus_appended = 0;


  for(uint8_t seg_i = 0; seg_i< getSegmentsNum(); seg_i++)
  {
    JBI->Object_Start_F("Segment%d",seg_i);      

      JBI->Array_Start("PixelRange");
        JBI->Add(segments[seg_i].start);
        JBI->Add(segments[seg_i].stop);
      JBI->Array_End();

      JBI->Add("ColourPalette", GetPaletteNameByID(segments[seg_i].palette_id, buffer, sizeof(buffer)) );

      JBI->Object_Start_F("Effects");      
        JBI->Add("Function",  GetFlasherFunctionNamebyID(segments[seg_i].effect_id, buffer, sizeof(buffer), true));
        JBI->Add("Speed",     segments[seg_i].speed );
        JBI->Add("Intensity", segments[seg_i].intensity );
      JBI->Object_End();
      JBI->Object_Start_F("Transition");
        JBI->Add("RateMs",  segments[seg_i].cycle_time__rate_ms );
      JBI->Object_End();

      for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
      {
        JBI->Array_Start_P(PSTR("SegColour%d"), seg_col);
          // for(uint8_t p=0;p<5;p++){ JBI->Add(segments[seg_i].segcol[seg_col][p]); }
        JBI->Array_End();
      }

    JBI->Object_End();
  }

  JBI->End();

  char filename_json[50];
  snprintf_P(filename_json, sizeof(filename_json), "/lgt_%S.json", GetModuleName());

  tkr_mfile->JSONFile_Save(filename_json, JBI->GetBuffer(), JBI->GetBufferLength());

  JBI->ReleaseLock();


}




// void mAnimatorLight::Init_Busses()
// {

//   uint32_t mem = 0;

//     /*****************************************************************************
//      * Detect type of NPB methods
//     ******************************************************************************/
//    #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
//    // determine if it is sensible to use parallel I2S outputs on ESP32 (i.e. more than 5 outputs = 1 I2S + 4 RMT)
//    bool useParallel = false;
//    #if defined(ARDUINO_ARCH_ESP32) && !defined(ARDUINO_ARCH_ESP32S2) && !defined(ARDUINO_ARCH_ESP32S3) && !defined(ARDUINO_ARCH_ESP32C3)
//    unsigned digitalCount = 0;
//    unsigned maxLedsOnBus = 0;
//    unsigned maxChannels = 0;
//    for (unsigned i = 0; i < BusManager::getNumBusses(); i++) {
//      if (tkr_iLight->busConfigs[i] == nullptr) break;
//      if (!Bus::isDigital(tkr_iLight->busConfigs[i]->type)) continue;
//      if (!Bus::is2Pin(tkr_iLight->busConfigs[i]->type)) {
//        digitalCount++;
//        unsigned channels = Bus::getNumberOfChannels(tkr_iLight->busConfigs[i]->type);
//        if (tkr_iLight->busConfigs[i]->length > maxLedsOnBus) maxLedsOnBus = tkr_iLight->busConfigs[i]->length;
//        if (channels > maxChannels) maxChannels  = channels;
//      }
//    }
//    DEBUG_PRINTF_P(PSTR("Maximum LEDs on a bus: %u\n\rDigital buses: %u\n\r"), maxLedsOnBus, digitalCount);
//    /**
//     * Assign to use parallel only when pixels per bus are low, and channels are more than 2
//     * Will use combined I2Sx2 + RMTx8 when pixels per bus are more than 300
//     * Default is 300 per output, but override is allowed in special cases (eg limited pins but want 1 higher output)
//     * 
//     */
//    if (maxLedsOnBus <= BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S && digitalCount > 2) 
//    {  // I will want >2, as I0 and I1 are for 2 pins only, then immediately switch to parallel
//      DEBUG_PRINTF_P(PSTR("Switching to parallel I2S\n\r"));
//      useParallel = true;
//      BusManager::useParallelOutput(true);
//      BusManager::setRequiredChannels(digitalCount);
//      mem = BusManager::memUsage(maxChannels, maxLedsOnBus, 8); // use alternate memory calculation (hse to be used *after* useParallelOutput())
//    }else
//    if (maxLedsOnBus > BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S && digitalCount > 2) 
//    {
//      ALOG_ERR(PSTR("Parallel is required to avoid RMT, but per bus count exceeded. Using anyway for now (%d,%d)"), maxLedsOnBus, digitalCount);
//      BusManager::setRequiredChannels(digitalCount);
//      BusManager::useParallelOutput(true);
//    }
//    else{
//      ALOG_INF(PSTR("Parallel is not required for %d channels"), digitalCount);
//      BusManager::setRequiredChannels(digitalCount);
//      BusManager::useParallelOutput(false);
//    }
//    #endif
//  #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//  DELAY_DEBUG(1000);

//  /*****************************************************************************
//   * Create NPB methods
//  ******************************************************************************/
//  for (uint8_t i = 0; i < BusManager::getNumBusses(); i++) 
//  {
//    if (tkr_iLight->busConfigs[i] == nullptr) break;
//    // mem += BusManager::memUsage(*tkr_iLight->busConfigs[i]);

//    #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
//    if (useParallel && i < 16) {
//      // if for some unexplained reason the above pre-calculation was wrong, update
//      unsigned memT = BusManager::memUsage(*tkr_iLight->busConfigs[i]); // includes x8 memory allocation for parallel I2S
//      if (memT > mem) mem = memT; // if we have unequal LED count use the largest
//    } 
//    else
//    #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
//    {
//      mem += BusManager::memUsage(*tkr_iLight->busConfigs[i]); // includes global buffer
//    }

//    if (mem <= MAX_LED_MEMORY) 
//    {        
//      BusManager::add(*tkr_iLight->busConfigs[i]);        
//    }
//    else
//    {        
//      ALOG_ERR(PSTR("MEMORY ISSUE"));        
//    }
//    delete tkr_iLight->busConfigs[i]; 
//    tkr_iLight->busConfigs[i] = nullptr;
//  }
//  ALOG_INF(PSTR("Memory used: %d"), mem);

  
// }


bool mAnimatorLight::LightingBusConfig_CheckPinsAvailable(
  const BusConfig& bus_config,
  uint8_t bus_i,
  uint8_t lighting_digital_index,
  uint8_t lighting_clock_index,
  uint8_t lighting_pwm_index,
  uint8_t lighting_onoff_index,
  bool override_existing
)
{
  uint8_t pin_count = Bus::getNumberOfPins(bus_config.type);

  if (pin_count == 0)
  {
    return true;
  }

  uint8_t digital_offset = 0;
  uint8_t clock_offset   = 0;
  uint8_t pwm_offset     = 0;
  uint8_t onoff_offset   = 0;

  for (uint8_t pin_i = 0; pin_i < pin_count; pin_i++)
  {
    const int16_t pin = bus_config.pins[pin_i];

    if (pin < 0)
    {
      ALOG_ERR(PSTR("LGT: CheckPins invalid pin bus=%u pin_i=%u pin=%d"), bus_i, pin_i, pin);
      return false;
    }

    if (pin >= MAX_GPIO_PIN)
    {
      ALOG_ERR(PSTR("LGT: CheckPins pin OOR bus=%u pin_i=%u pin=%d"), bus_i, pin_i, pin);
      return false;
    }

    const uint8_t real_pin = (uint8_t)pin;

    uint16_t gpio_base = GPIO_NONE;
    uint8_t gpio_index = 0;

    if (Bus::isDigital(bus_config.type))
    {
      if (Bus::is2Pin(bus_config.type) && (pin_i == 1))
      {
        gpio_base = GPIO_LIGHTING_CLOCK;
        gpio_index = lighting_clock_index + clock_offset;
        clock_offset++;
      }
      else
      {
        gpio_base = GPIO_LIGHTING_DIGITAL;
        gpio_index = lighting_digital_index + digital_offset;
        digital_offset++;
      }
    }
    else
    {
      gpio_base = GPIO_LIGHTING_PWM;
      gpio_index = lighting_pwm_index + pwm_offset;
      pwm_offset++;
    }

    const uint16_t packed_id = tkr_pins->GPIOPacked_Make(gpio_base, gpio_index);

    if (!override_existing && tkr_pins->pin[real_pin].IsAllocated())
    {
      ALOG_ERR(PSTR("LGT: CheckPins pin already allocated bus=%u pin_i=%u pin=%u current=0x%04X owner=%u new_base=%u new_index=%u new=0x%04X"), bus_i, pin_i, real_pin, tkr_pins->pin[real_pin].gpio_function, tkr_pins->pin[real_pin].unique_module_owner_id, gpio_base, gpio_index, packed_id);
      return false;
    }

    if (override_existing && tkr_pins->pin[real_pin].IsAllocated())
    {
      ALOG_WRN(PSTR("LGT: CheckPins override allowed bus=%u pin_i=%u pin=%u current=0x%04X owner=%u new_base=%u new_index=%u new=0x%04X"), bus_i, pin_i, real_pin, tkr_pins->pin[real_pin].gpio_function, tkr_pins->pin[real_pin].unique_module_owner_id, gpio_base, gpio_index, packed_id);
    }

    ALOG_DBM(PSTR("LGT: CheckPins OK bus=%u pin_i=%u pin=%u gpio_base=%u gpio_index=%u packed=0x%04X override=%u"), bus_i, pin_i, real_pin, gpio_base, gpio_index, packed_id, override_existing);
  }

  return true;
}


bool mAnimatorLight::Lighting_AllocatePin_WithOverride(
  uint8_t real_pin,
  uint16_t packed_id,
  uint16_t owner_id,
  bool override_existing
)
{
  mPins::PinAllocationFlags flags;
  flags.data = 0;
  flags.grouped = 1;
  flags.shared = 0;
  flags.sensitive_to_probe = 1;

  if (!override_existing)
  {
    return tkr_pins->AllocatePin(real_pin, packed_id, owner_id, flags);
  }

  if (tkr_pins->pin[real_pin].IsAllocated())
  {
    ALOG_WRN(PSTR("LGT: AllocatePin override clearing pin=%u current=0x%04X owner=%u"), real_pin, tkr_pins->pin[real_pin].gpio_function, tkr_pins->pin[real_pin].unique_module_owner_id);

    tkr_pins->pin[real_pin].allocation.data = 0;
    tkr_pins->pin[real_pin].gpio_function = GPIO_NONE;
    tkr_pins->pin[real_pin].unique_module_owner_id = 0;
  }

  return tkr_pins->AllocatePin(real_pin, packed_id, owner_id, flags);
}


void mAnimatorLight::LightingBusConfig_AllocatePins(
  const BusConfig& bus_config,
  uint8_t bus_i,
  uint8_t& lighting_digital_index,
  uint8_t& lighting_clock_index,
  uint8_t& lighting_pwm_index,
  uint8_t& lighting_onoff_index,
  bool override_existing
)
{
  uint8_t pin_count = Bus::getNumberOfPins(bus_config.type);

  if (pin_count == 0)
  {
    return;
  }

  for (uint8_t pin_i = 0; pin_i < pin_count; pin_i++)
  {
    const int16_t pin = bus_config.pins[pin_i];

    if (pin < 0)
    {
      ALOG_ERR(PSTR("LGT: AllocatePins invalid pin bus=%u pin_i=%u pin=%d"), bus_i, pin_i, pin);
      continue;
    }

    if (pin >= MAX_GPIO_PIN)
    {
      ALOG_ERR(PSTR("LGT: AllocatePins pin OOR bus=%u pin_i=%u pin=%d"), bus_i, pin_i, pin);
      continue;
    }

    const uint8_t real_pin = (uint8_t)pin;

    uint16_t gpio_base = GPIO_NONE;
    uint8_t* gpio_index_ptr = nullptr;

    if (Bus::isDigital(bus_config.type))
    {
      if (Bus::is2Pin(bus_config.type) && (pin_i == 1))
      {
        gpio_base = GPIO_LIGHTING_CLOCK;
        gpio_index_ptr = &lighting_clock_index;
      }
      else
      {
        gpio_base = GPIO_LIGHTING_DIGITAL;
        gpio_index_ptr = &lighting_digital_index;
      }
    }
    else
    {
      gpio_base = GPIO_LIGHTING_PWM;
      gpio_index_ptr = &lighting_pwm_index;
    }

    if (!gpio_index_ptr)
    {
      ALOG_ERR(PSTR("LGT: AllocatePins no gpio_index_ptr bus=%u pin_i=%u pin=%u"), bus_i, pin_i, real_pin);
      continue;
    }

    const uint8_t gpio_index = *gpio_index_ptr;
    const uint16_t packed_id = tkr_pins->GPIOPacked_Make(gpio_base, gpio_index);

    bool allocated = Lighting_AllocatePin_WithOverride(
      real_pin,
      packed_id,
      GetModuleUniqueID(),
      override_existing
    );

    if (!allocated)
    {
      ALOG_ERR(PSTR("LGT: AllocatePin failed bus=%u pin_i=%u pin=%u gpio_base=%u gpio_index=%u packed=0x%04X override=%u"), bus_i, pin_i, real_pin, gpio_base, gpio_index, packed_id, override_existing);
      continue;
    }

    ALOG_INF(PSTR("LGT: AllocatePin bus=%u pin_i=%u pin=%u gpio_base=%u gpio_index=%u packed=0x%04X owner=%u override=%u"), bus_i, pin_i, real_pin, gpio_base, gpio_index, packed_id, GetModuleUniqueID(), override_existing);

    (*gpio_index_ptr)++;
  }
}


void mAnimatorLight::Init_Busses()
{

  uint32_t mem = 0;

  /*
   * Debug / migration override.
   *
   * false:
   *   Pin allocation check can block bus creation if a physical pin is already owned.
   *
   * true:
   *   Bus creation is allowed to continue and AllocatePin is allowed to force ownership.
   */
  bool enable_allocatepin_override = false;

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING_ALLOCATEPIN_OVERRIDE
  enable_allocatepin_override = true;
  #endif

  /*
   * Running logical lighting allocation indexes.
   *
   * These map the created light buses onto the new generic GPIO roles:
   *   GPIO_LIGHTING_DIGITAL  1..16
   *   GPIO_LIGHTING_CLOCK    1..4
   *   GPIO_LIGHTING_PWM      1..10
   *   GPIO_LIGHTING_ONOFF    1..5
   *
   * Note: indexes here are internal zero-based indexes.
   */
  uint8_t lighting_digital_index = 0;
  uint8_t lighting_clock_index   = 0;
  uint8_t lighting_pwm_index     = 0;
  uint8_t lighting_onoff_index   = 0;

    /*****************************************************************************
     * Detect type of NPB methods
    ******************************************************************************/
   #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
   // determine if it is sensible to use parallel I2S outputs on ESP32 (i.e. more than 5 outputs = 1 I2S + 4 RMT)
   bool useParallel = false;
   #if defined(ARDUINO_ARCH_ESP32) && !defined(ARDUINO_ARCH_ESP32S2) && !defined(ARDUINO_ARCH_ESP32S3) && !defined(ARDUINO_ARCH_ESP32C3)
   unsigned digitalCount = 0;
   unsigned maxLedsOnBus = 0;
   unsigned maxChannels = 0;
   for (const auto &bus : tkr_iLight->busConfigs) {
  //  for (unsigned i = 0; i < WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES; i++) {
  //     if (tkr_iLight->busConfigs[i] == nullptr) break;
      if (!Bus::isDigital(bus.type)) continue;
      if (!Bus::is2Pin(bus.type)) {
        digitalCount++;
        unsigned channels = Bus::getNumberOfChannels(bus.type);
        if (bus.count > maxLedsOnBus) maxLedsOnBus = bus.count;
        if (channels > maxChannels) maxChannels  = channels;
      }
   }
   DEBUG_PRINTF_P(PSTR("Maximum LEDs on a bus: %u\n\rDigital buses: %u\n\r"), maxLedsOnBus, digitalCount);
   /**
    * Assign to use parallel only when pixels per bus are low, and channels are more than 2
    * Will use combined I2Sx2 + RMTx8 when pixels per bus are more than 300
    * Default is 300 per output, but override is allowed in special cases (eg limited pins but want 1 higher output)
    * 
    */
   if (maxLedsOnBus <= BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S && digitalCount > 2) 
   {  // I will want >2, as I0 and I1 are for 2 pins only, then immediately switch to parallel
     DEBUG_PRINTF_P(PSTR("Switching to parallel I2S\n\r"));
     useParallel = true;
     BusManager::useParallelOutput(true);
     BusManager::setRequiredChannels(digitalCount);
     mem = 0;//BusManager::memUsage(maxChannels, maxLedsOnBus, 8); // use alternate memory calculation (hse to be used *after* useParallelOutput())
   }else
   if (maxLedsOnBus > BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S && digitalCount > 2) 
   {
     ALOG_ERR(PSTR("Parallel is required to avoid RMT, but per bus count exceeded. Using anyway for now (%d,%d)"), maxLedsOnBus, digitalCount);
     BusManager::setRequiredChannels(digitalCount);
     BusManager::useParallelOutput(true);
   }
   else{
     ALOG_INF(PSTR("Parallel is not required for %d channels"), digitalCount);
     BusManager::setRequiredChannels(digitalCount);
     BusManager::useParallelOutput(false);
   }
   #endif
 #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

 
  for (auto &bus : tkr_iLight->busConfigs) {
    // assign bus types: call to getI() determines bus types/drivers, allocates and tracks polybus channels
    // store the result in iType for later use during bus creation (getI() must only be called once per BusConfig)
    // note: this needs to be determined for all buses prior to creating them as it also determines parallel I2S usage
    bus.iType = BusManager::getI(bus.type, bus.pins, bus.driverType);
  }

 /*****************************************************************************
  * Create NPB methods
 ******************************************************************************/
uint8_t i = 0;
   for (const auto &bus : tkr_iLight->busConfigs) {
//  for (uint8_t i = 0; i < tkr_iLight->busConfigs.size(); i++) 
//  {
  //  if (tkr_iLight->busConfigs[i] == nullptr) break;
   // mem += BusManager::memUsage(*tkr_iLight->busConfigs[i]);

   #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
   if (useParallel && i < 16) {
     // if for some unexplained reason the above pre-calculation was wrong, update
     unsigned memT = bus.memUsage(); // includes x8 memory allocation for parallel I2S
     if (memT > mem) mem = memT; // if we have unequal LED count use the largest
   } 
   else
   #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
   {
     mem += bus.memUsage(); // includes global buffer
   }

   ALOG_INF(PSTR("memUsage %d"),mem);

   if (mem <= MAX_LED_MEMORY) 
   {
     bool pins_available = LightingBusConfig_CheckPinsAvailable(
       bus,
       i,
       lighting_digital_index,
       lighting_clock_index,
       lighting_pwm_index,
       lighting_onoff_index,
       enable_allocatepin_override
     );

     if (pins_available)
     {
       BusManager::add(bus);

       LightingBusConfig_AllocatePins(
         bus,
         i,
         lighting_digital_index,
         lighting_clock_index,
         lighting_pwm_index,
         lighting_onoff_index,
         enable_allocatepin_override
       );
     }
     else
     {
       ALOG_ERR(PSTR("LGT: Bus add skipped, pin allocation check failed bus=%u override=%u"), i, enable_allocatepin_override);
     }
   }
   else
   {        
     ALOG_ERR(PSTR("MEMORY ISSUE"));        
   }
  //  delete tkr_iLight->busConfigs[i]; 
  //  tkr_iLight->busConfigs[i] = nullptr;

   i++;
 }
 ALOG_INF(PSTR("Memory used: %d"), mem);


 tkr_pins->PinTable_SerialPrint("after bus set");

  
}



void mAnimatorLight::EveryLoop()
{
    
    
  if (doInitBusses) 
  {

    doInitBusses = false;
    ALOG_INF(PSTR("Re-init busses"));
        
    bool aligned = checkSegmentAlignment(); //see if old segments match old bus(ses)
    BusManager::removeAll();

    ALOG_INF(PSTR("checkSegmentAlignment()-----------------------------------------aligned %d"), aligned);

    Init_Busses();

    
    finalizeInit(); // also loads default ledmap if present

    uint8_t bri = 128; // temporary brightness value
    
    BusManager::setBrightness(bri); // fix re-initialised bus' brightness #4005
    
    if (aligned) makeAutoSegments();
    else fixInvalidSegments();
    
    BusManager::setBrightness(bri); // fix re-initialised bus' brightness

    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    /***
     * Matrix can only be loaded after the busses have been created, and segments have been created.
     * An easy way to do this to ensure order is perhaps have "isMatrix" not started (ie not loading panels)
     * until this section of code runs (Which would only be called after segments have been created)
     */
    if(panel.size()) // if there are panels waiting to be loading
    {
      ALOG_INF(PSTR("PANELS: %d"), panel.size());    
      setUpMatrix();
      makeAutoSegments();
      if(SEGMENT.stopY > 1000){ ALOG_ERR(PSTR("SEGMENT.stopY > 1000")); SEGMENT.stopY = 33;} // debug issue
    }

    /***
     * Either 1D or 2D custom maps can now be loaded after busses and any 2d matrix have been started
     */
    if (loadLedmap >= 0) {
      ALOG_HGL(PSTR("Loading LED map."));
      deserializeMap(loadLedmap); // load custom LED map
      loadLedmap = -1;
    }

    

    #endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

      
    // allocate frame buffer after matrix has been set up (gaps!)
    updatePixelBuffer();


    ALOG_INF(PSTR("Segment count: %d"), getSegmentsNum());
    doSerializeConfig = true;
    // serializeConfig(); // in WLED This saved everything to json memory
  }
    
  #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
  handleWs();
  #endif

  handleTransitions();

  // if (doSerializeConfig) serializeConfig();

  // Tmp fix to set brightness
  // BusManager::setBrightness( tkr_iLight->getBriRGB_Global() ); // fix re-initialised bus' brightness

  if (doReboot && !doInitBusses) // if busses have to be inited & saved, wait until next iteration
    reset();
    
      // ALOG_INF(PSTR("Loop1a"));Serial.flush();
  // This should be removed, as realtime mode will cause this to switch anyway
  /**
   * @brief If RealTime modes first
   * - when realtime is used, effects are not to keep performance up and since the realtime aspect (network) needs priority, it is not going to be
   *   sustainable to also have internal animations running 
   * 
   * realtimeModes will always be loaded into the first segment, with other segments paused when realtimeMode is active
   */
  switch(realtimeMode)
  {
    case ANIMATION_MODE__EFFECTS:{ // Effects created on device, local control

      #ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
      SubTask_Demo();
      #endif

      /**
       * @brief Issue fix?
       * When a playlist change happens, it will request a load.
       * This load happens in preset, which must occur prior to Effect being called.
       * This may fix the corruption issue when changing playlist while in effects mode.
       * 
       * ie grp 150 tto 1 (via jsoncommand), but effect happened before this completed since preset to load is a flag, not a hard change
       * 
       */
      #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
      SubTask_Playlist();
      #endif

      #ifdef ENABLE_FEATURE_LIGHTS__PRESETS
      SubTask_Presets();
      #endif

      // ALOG_INF(PSTR("Loop1b"));Serial.flush();
      #ifdef ENABLE_FEATURE_LIGHTING__EFFECTS
      DEBUG_LIGHTING__START_TIME_RECORDING(1)
      SubTask_Effects();
      DEBUG_LIGHTING__SAVE_TIME_RECORDING(1, lighting_time_critical_logging.segment_effects); 
      #endif
      // ALOG_INF(PSTR("Loop1c"));Serial.flush();


    }break;
    #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MODES
    case ANIMATION_MODE__REALTIME_MQTT_SETPIXEL:
      SubTask_RealTime_SetPixel();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_REALTIME_UDP
    case ANIMATION_MODE__REALTIME_UDP:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_HYPERION
    case ANIMATION_MODE__REALTIME_HYPERION:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_E131
    case ANIMATION_MODE__REALTIME_E131:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_ADALIGHT
    case ANIMATION_MODE__REALTIME_ADALIGHT:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_ARTNET
    case ANIMATION_MODE__REALTIME_ARTNET:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_TPM2NET
    case ANIMATION_MODE__REALTIME_TPM2NET:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_DDP
    case ANIMATION_MODE__REALTIME_DDP:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
    case ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE:
      SubTask_AnimationMode__InternalControlFromAnotherModule();
    break;
    #endif
    
  } // END switch

  yield();

}

#ifdef ENABLE_FEATURE_LIGHTS__ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
/**
 * @brief Function should call the Bus::Show based on the effect_period time
 * It will assume the setPixelColor has been called, and brightness has been already applied
 * 
 * Note: This currently does not respect segments, and thus causes issues when effects are running at the same time. It needs fixing.
 * 
 */
void mAnimatorLight::SubTask_AnimationMode__InternalControlFromAnotherModule()
{
  
  uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  effect_start_time = nowUp + timebase;
  if (nowUp - _lastShow < MIN_SHOW_DELAY) return;
  bool doShow = false;

  _isServicing = true;

  if(doShow)
  {
    yield();
    show();
    _lastShow = nowUp;
  }   

  _isServicing = false;

}
#endif


void mAnimatorLight::BootMessage()
{
  

}


void mAnimatorLight::Pre_Init(void)
{

  #ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
  // On preinit, make sure to init the local class pointer to the global instance
  tkr_extern_lAni = this;
  #endif

}


void mAnimatorLight::handleTransitions() {
  //handle still pending interface update
  updateInterfaces(interfaceUpdateCallMode);

  if (transitionActive && tkr_anim->getTransition() > 0) {
    int ti = millis() - transitionStartTime;
    int tr = tkr_anim->getTransition();
    if (ti/tr) {
      tkr_anim->setTransitionMode(false); // stop all transitions
      // restore (global) transition time if not called from UDP notifier or single/temporary transition from JSON (also playlist)
      if (jsonTransitionOnce) tkr_anim->setTransition(transitionDelay);
      transitionActive = false;
      jsonTransitionOnce = false;
      applyFinalBri();
      return;
    }
    byte briTO = briT;
    uint8_t bri = tkr_iLight->_briRGB_Global;
    int deltaBri = (int)bri - (int)briOld;
    briT = briOld + (deltaBri * ti / tr);
    if (briTO != briT) applyBri();
  }
}



//scales the brightness with the briMultiplier factor
byte  mAnimatorLight::scaledBri(byte in)
{
  unsigned val = ((unsigned)in*briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


//applies global temporary brightness (briT) to strip
void  mAnimatorLight::applyBri() {
  if (realtimeOverride || !(realtimeMode && arlsForceMaxBri))
  {
    //DEBUG_PRINTF_P(PSTR("Applying strip brightness: %d (%d,%d)\n"), (int)briT, (int)bri, (int)briOld);
    tkr_anim->setBrightness(briT);
  }
}


//applies global brightness and sets it as the "current" brightness (no transition)
void  mAnimatorLight::applyFinalBri() {
  uint8_t bri = tkr_iLight->_briRGB_Global;
  briOld = bri;
  briT = bri;
  applyBri();
  tkr_anim->trigger(); // force one last update
}


//called after every state changes, schedules interface updates, handles brightness transition and nightlight activation
//unlike colorUpdated(), does NOT apply any colors or FX to segments
void mAnimatorLight::stateUpdated(byte callMode) {
  //call for notifier -> 0: init 1: direct change 2: button 3: notification 4: nightlight 5: other (No notification)
  //                     6: fx changed 7: hue 8: preset cycle 9: blynk 10: alexa 11: ws send only 12: button preset
  // setValuesFromFirstSelectedSeg();  // a much better approach would be to use main segment: setValuesFromMainSeg()

  uint8_t bri = tkr_iLight->_briRGB_Global;

  if (bri != briOld || stateChanged) {
    if (stateChanged) currentPreset = 0; //something changed, so we are no longer in the preset

    if (callMode != CALL_MODE_NOTIFICATION && callMode != CALL_MODE_NO_NOTIFY) notify(callMode);
    // if (bri != briOld && nodeBroadcastEnabled) sendSysInfoUDP(); // update on state

    //set flag to update ws and mqtt
    interfaceUpdateCallMode = callMode;
  } else {
    if (nightlightActive && !nightlightActiveOld && callMode != CALL_MODE_NOTIFICATION && callMode != CALL_MODE_NO_NOTIFY) {
      notify(CALL_MODE_NIGHTLIGHT);
      interfaceUpdateCallMode = CALL_MODE_NIGHTLIGHT;
    }
  }

  unsigned long now = millis();
  if (callMode != CALL_MODE_NO_NOTIFY && nightlightActive && (nightlightMode == NL_MODE_FADE || nightlightMode == NL_MODE_COLORFADE)) {
    briNlT = bri;
    nightlightDelayMs -= (now - nightlightStartTime);
    nightlightStartTime = now;
  }
  if (briT == 0) {
    if (callMode != CALL_MODE_NOTIFICATION) resetTimebase(); //effect start from beginning
  }

  if (bri > 0) briLast = bri;

  //deactivate nightlight if target brightness is reached
  if (bri == nightlightTargetBri && callMode != CALL_MODE_NO_NOTIFY && nightlightMode != NL_MODE_SUN) nightlightActive = false;

  // notify usermods of state change
  // UsermodManager::onStateChange(callMode);

  if (getTransition() == 0) {
    jsonTransitionOnce = false;
    transitionActive = false;
    applyFinalBri();
  } else {
    if (transitionActive) {
      briOld = briT;
    } else if (bri != briOld || stateChanged)
      setTransitionMode(true); // force all segments to transition mode
    transitionActive = true;
    transitionStartTime = now;
  }
  stateChanged = false;
}






#ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MODES 
void mAnimatorLight::SubTask_RealTime_SetPixel()
{
  // Do nothing
}
#endif


void mAnimatorLight::Reset_CustomPalette_NamesDefault()
{
  
  char buffer[30];
  
  for (int ii=0;ii< mPaletteI->user_defined_palette_count;ii++){ 
    sprintf(buffer, D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR, ii+1);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii);
  }

}


void mAnimatorLight::EverySecond_AutoOff()
{

  for (Segment &seg : segments) 
  {
  
    if(seg.auto_timeoff.UpdateTick())
    {
      ALOG_INF( PSTR(D_LOG_LED D_COMMAND_NVALUE_K("Running Value")), seg.auto_timeoff.Value());
    }

    if(seg.auto_timeoff.IsLastTick())
    {
      ALOG_INF(PSTR("Segment Turn OFF"));
      // Set intensity to make all LEDs refresh
      seg.intensity = 255;
      seg.single_animation_override.time_ms = seg.single_animation_override_turning_off.time_ms;// 1000; // slow and smooth turn off
      seg.cycle_time__rate_ms = seg.single_animation_override.time_ms+10; // exceed the time to turn off to let it cycle through

      ALOG_INF(PSTR("Setting override for off %d"), seg.single_animation_override.time_ms);

      tkr_anim->force_update();

      // seg.setBrightnessRGB(0);
      // seg.setBrightnessCCT(0);    
    }

  }

} // END EverySecond_AutoOff

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
inline void mAnimatorLight::Segment::Update_LivePalettes(
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
  if (speed != 255) {
    const uint32_t new_colour_rate_ms = 1000UL + (uint32_t)(live_palette.intensity * 100UL);
    if (new_colour_rate_ms >= cycle_time__rate_ms) {
      cycle_time__rate_ms = new_colour_rate_ms + 100UL;
    }
  }

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
    float progress;

    if (preview_mode) {
      // Expect preview_index = 0 or 1 only
      progress = (preview_index == 0) ? 0.0f : 1.0f;
    } else {
      uint8_t s = tkr_time->RtcTime.second;
      progress = (s < 30) ? mSupport::mapfloat(s, 0, 29, 0.0f, 1.0f) : mSupport::mapfloat(s, 30, 59, 1.0f, 0.0f);
    }

    const RgbwwColor c1  = segcol[0].colour;
    const RgbwwColor c2  = segcol[1].colour;
    const RgbwwColor out = RgbwwColor::LinearBlend(c1, c2, progress);

    palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
    palette_loaded->solid_colour.whiteWW    = out.CW;
    return;
  }


  // ------------------------------------------------------------------
  // LIVE: TimeReactive SegCol Blend (Hour sawtooth)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ID)
  {
    float progress;

    if (preview_mode) {
      // Expect preview_index = 0 or 1 only
      progress = (preview_index == 0) ? 0.0f : 1.0f;
    } else {
      uint8_t s = tkr_time->RtcTime.hour;
      progress = (s < 30) ? mSupport::mapfloat(s, 0, 29, 0.0f, 1.0f) : mSupport::mapfloat(s, 30, 59, 1.0f, 0.0f);
    }

    const RgbwwColor c1  = segcol[0].colour;
    const RgbwwColor c2  = segcol[1].colour;
    const RgbwwColor out = RgbwwColor::LinearBlend(c1, c2, progress);

    palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
    palette_loaded->solid_colour.whiteWW    = out.CW;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: SegCol Cycle (Immediate)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_IMMEDIATE_01__ID)
  {
    uint8_t k = 0;

    if (preview_mode) {
      // serializer iterates 0..4 (after your colours_in_palette override)
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

    const RgbwwColor c = segcol[k].colour;
    palette_loaded->solid_colour.colourRGBW = RGBW32(c.R, c.G, c.B, c.WW);
    palette_loaded->solid_colour.whiteWW    = c.CW;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: SegCol Cycle (Blending + controllable dwell)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_BLENDING_02__ID)
  {
    if (preview_mode) {
      // For preview you asked for 5 discrete colours (not blended)
      uint8_t k = (uint8_t)preview_index;
      if (k > 4) k = 4;

      const RgbwwColor c = segcol[k].colour;
      palette_loaded->solid_colour.colourRGBW = RGBW32(c.R, c.G, c.B, c.WW);
      palette_loaded->solid_colour.whiteWW    = c.CW;
      return;
    }

    // Runtime: cached once per frame (your blending “HOW”)
    const uint32_t now_ms = millis();
    if (live_palette.timing1 == 0) live_palette.timing1 = now_ms;

    const uint32_t T_max_ms = 25000UL;
    uint32_t T_ms = (uint32_t)live_palette.intensity * T_max_ms / 255UL;
    if (T_ms < 1000UL) T_ms = 1000UL;

    const uint32_t t_ms = (T_ms > 0) ? ((now_ms - live_palette.timing1) % T_ms) : 0UL;

    const uint32_t s1 = (1UL * T_ms) / 5UL;
    const uint32_t s2 = (2UL * T_ms) / 5UL;
    const uint32_t s3 = (3UL * T_ms) / 5UL;
    const uint32_t s4 = (4UL * T_ms) / 5UL;

    uint8_t  k;
    uint32_t slot_start, slot_end;
    if      (t_ms < s1) { k=0; slot_start=0;  slot_end=s1; }
    else if (t_ms < s2) { k=1; slot_start=s1; slot_end=s2; }
    else if (t_ms < s3) { k=2; slot_start=s2; slot_end=s3; }
    else if (t_ms < s4) { k=3; slot_start=s3; slot_end=s4; }
    else                { k=4; slot_start=s4; slot_end=T_ms; }

    const uint32_t slot_ms   = slot_end - slot_start;
    const uint32_t within_ms = t_ms - slot_start;

    const uint8_t k_next = (k == 4) ? 0 : (uint8_t)(k + 1);

    const RgbwwColor c_curr = segcol[k].colour;
    const RgbwwColor c_next = segcol[k_next].colour;

    #ifndef SEGCOLOUR_CYCLE_HOLD_PCT
    #define SEGCOLOUR_CYCLE_HOLD_PCT   70
    #endif
    #ifndef SEGCOLOUR_CYCLE_BLEND_PCT
    #define SEGCOLOUR_CYCLE_BLEND_PCT  30
    #endif

    uint32_t hold_pct  = (uint32_t)SEGCOLOUR_CYCLE_HOLD_PCT;
    uint32_t blend_pct = (uint32_t)SEGCOLOUR_CYCLE_BLEND_PCT;
    if (hold_pct > 100UL)  hold_pct  = 100UL;
    if (blend_pct > 100UL) blend_pct = 100UL;
    if (hold_pct + blend_pct > 100UL) blend_pct = 100UL - hold_pct;

    const uint32_t hold_end_ms = (slot_ms * hold_pct) / 100UL;

    RgbwwColor out = c_curr;
    if (!(slot_ms == 0 || within_ms < hold_end_ms || slot_ms <= hold_end_ms)) {
      const uint32_t start = hold_end_ms;
      const uint32_t denom = (slot_ms > start) ? (slot_ms - start) : 1UL;
      const uint32_t w     = within_ms - start;

      float p = (float)w / (float)denom;
      if (p > 1.0f) p = 1.0f;
      out = RgbwwColor::LinearBlend(c_curr, c_next, p);
    }

    palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
    palette_loaded->solid_colour.whiteWW    = out.CW;
    return;
  }

  // other live palettes later...
  // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> Segment colour blend (Daytime)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ID)
  {
    #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
      const float elevation = tkr_solar->Get_Elevation();
      const float el_min = 0.0f;
      const float el_max = (ELEVATION_DAY_THRESHOLD != 0) ? ELEVATION_DAY_THRESHOLD : tkr_solar->Get_Elevation_Max();
    #else
      const float elevation = 0.0f;
      const float el_min = 0.0f;
      const float el_max = 10.0f;
    #endif

    float eval_elevation = elevation;

    if (preview_mode)
    {
      const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
      const float x = (N <= 1) ? 0.0f : ((float)preview_index / (float)(N - 1));
      eval_elevation = mSupport::mapfloat(x, 0.0f, 1.0f, el_min, el_max);
    }

    eval_elevation = constrain(eval_elevation, el_min, el_max);
    const float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);

    const RgbwwColor c1 = segcol[0].colour;
    const RgbwwColor c2 = segcol[1].colour;
    const RgbwwColor out = RgbwwColor::LinearBlend(c1, c2, progress);

    palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
    palette_loaded->solid_colour.whiteWW    = out.CW;
    return;
  }

  // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> Segment colour blend (Dawn/Dusk)
  // ------------------------------------------------------------------
  if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__ID)
  {
    #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
      const float elevation = tkr_solar->Get_Elevation();
      const float el_min = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : tkr_solar->Get_Elevation_Min();
      const float el_max = (ELEVATION_DAY_THRESHOLD   != 0) ? ELEVATION_DAY_THRESHOLD   : tkr_solar->Get_Elevation_Max();
    #else
      const float elevation = 0.0f;
      const float el_min = -10.0f;
      const float el_max =  10.0f;
    #endif

    float eval_elevation = elevation;

    if (preview_mode)
    {
      const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
      const float x = (N <= 1) ? 0.0f : ((float)preview_index / (float)(N - 1));
      eval_elevation = mSupport::mapfloat(x, 0.0f, 1.0f, el_min, el_max);
    }

    eval_elevation = constrain(eval_elevation, el_min, el_max);
    const float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);

    const RgbwwColor c1 = segcol[0].colour;
    const RgbwwColor c2 = segcol[1].colour;
    const RgbwwColor out = RgbwwColor::LinearBlend(c1, c2, progress);

    palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
    palette_loaded->solid_colour.whiteWW    = out.CW;
    return;
  }
  // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> White colour temperature (warm<->cold) + RGB tint
  // ------------------------------------------------------------------
  // ------------------------------------------------------------------
// LIVE: Solar Elevation White Colour Temperature (RGBWW/CW)
//   * runtime: keep true RGBWW/CW
//   * preview: WebUI is RGB-only, so fold WW+CW into RGB and zero whites
// ------------------------------------------------------------------
if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID)
{
  #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
    const float elevation = tkr_solar->Get_Elevation();
    const float el_min = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : tkr_solar->Get_Elevation_Min();
    const float el_max = (ELEVATION_DAY_THRESHOLD   != 0) ? ELEVATION_DAY_THRESHOLD   : tkr_solar->Get_Elevation_Max();
  #else
    const float elevation = 0.0f;
    const float el_min = -10.0f;
    const float el_max =  10.0f;
  #endif

  float eval_elevation = elevation;

  if (preview_mode)
  {
    // preview_index is expected 0..(N-1). map to elevation range.
    const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
    const float x = (N <= 1) ? 0.0f : ((float)preview_index / (float)(N - 1)); // 0..1
    eval_elevation = mSupport::mapfloat(x, 0.0f, 1.0f, el_min, el_max);
  }

//   mAnimatorLight::SegmentColour colour_out = 0;

//   if (eval_elevation <= el_min) {
//     colour_out.setCCT_Kelvin(CCT_MAX_DEFAULT);
//     colour_out.setRGB(0xFF, 0x52, 0x18);
//   } else if (eval_elevation >= el_max) {
//     colour_out.setCCT_Kelvin(CCT_MIN_DEFAULT);
//     colour_out.setRGB(255, 255, 255);
//   } else {
//     const float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);

//     mAnimatorLight::SegmentColour warm = 0;
//     warm.setCCT_Kelvin(CCT_MAX_DEFAULT);
//     warm.setRGB(0xFF, 0x52, 0x18);

//     mAnimatorLight::SegmentColour cold = 0;
//     cold.setCCT_Kelvin(CCT_MIN_DEFAULT);
//     cold.setRGB(255, 255, 255);

//     colour_out.colour = RgbwwColor::LinearBlend(warm.colour, cold.colour, progress);
//   }

//   const RgbwwColor out = colour_out.colour;

//   // WebUI preview is RGB-only: fold WW+CW into RGB and zero whites
//   if (preview_mode)
//   {
//     uint8_t r = out.R;
//     uint8_t g = out.G;
//     uint8_t b = out.B;

//     const uint8_t ww = out.WW;
//     const uint8_t cw = out.CW;

//     // Warm white contribution (warm bias)
//     r = qadd8(r, scale8(ww, 255));
//     g = qadd8(g, scale8(ww, 200));
//     b = qadd8(b, scale8(ww, 120));

//     // Cold white contribution (cool bias)
//     r = qadd8(r, scale8(cw, 120));
//     g = qadd8(g, scale8(cw, 200));
//     b = qadd8(b, scale8(cw, 255));

//     palette_loaded->solid_colour.colourRGBW = RGBW32(r, g, b, 0);
//     palette_loaded->solid_colour.whiteWW    = 0;
//     return;
//   }

//   // Runtime: keep real channels
//   palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
//   palette_loaded->solid_colour.whiteWW    = out.CW;
//   return;
// }


//   // ------------------------------------------------------------------
//   // LIVE: Solar Elevation -> Segment colour blend (Nighttime)
//   // ------------------------------------------------------------------
//   if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__ID)
//   {
//     #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
//       const float elevation = tkr_solar->Get_Elevation();
//       const float el_max = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : -10.0f;
//       const float el_min = tkr_solar->Get_Elevation_Min();
//     #else
//       const float elevation = 0.0f;
//       const float el_min = -30.0f;
//       const float el_max = -10.0f;
//     #endif

//     float eval_elevation = elevation;

//     if (preview_mode)
//     {
//       const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
//       const float x = (N <= 1) ? 0.0f : ((float)preview_index / (float)(N - 1));
//       eval_elevation = mSupport::mapfloat(x, 0.0f, 1.0f, el_min, el_max);
//     }

//     eval_elevation = constrain(eval_elevation, el_min, el_max);
//     const float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);

//     const RgbwwColor c1 = segcol[0].colour;
//     const RgbwwColor c2 = segcol[1].colour;
//     const RgbwwColor out = RgbwwColor::LinearBlend(c1, c2, progress);

//     palette_loaded->solid_colour.colourRGBW = RGBW32(out.R, out.G, out.B, out.WW);
//     palette_loaded->solid_colour.whiteWW    = out.CW;
mAnimatorLight::SegmentColour colour_out;

if (eval_elevation <= el_min)
{
  colour_out.setCCT_Kelvin(CCT_MAX_DEFAULT);
  colour_out.setRGB(0xFF, 0x52, 0x18);
}
else if (eval_elevation >= el_max)
{
  colour_out.setCCT_Kelvin(CCT_MIN_DEFAULT);
  colour_out.setRGB(255, 255, 255);
}
else
{
  const float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);
  const uint8_t blend = static_cast<uint8_t>(constrain(progress, 0.0f, 1.0f) * 255.0f);

  mAnimatorLight::SegmentColour warm;
  warm.setCCT_Kelvin(CCT_MAX_DEFAULT);
  warm.setRGB(0xFF, 0x52, 0x18);

  mAnimatorLight::SegmentColour cold;
  cold.setCCT_Kelvin(CCT_MIN_DEFAULT);
  cold.setRGB(255, 255, 255);

  colour_out.colour = color_blend(warm.colour, cold.colour, blend);
  colour_out.cct = static_cast<uint8_t>((static_cast<uint16_t>(warm.cct) * (255U - blend) + static_cast<uint16_t>(cold.cct) * blend) / 255U);
}

if (preview_mode)
{
  uint8_t r = R(colour_out.colour);
  uint8_t g = G(colour_out.colour);
  uint8_t b = B(colour_out.colour);

  const uint8_t white = W(colour_out.colour);
  const uint8_t cct = colour_out.cct;

  const uint8_t cw = static_cast<uint8_t>((static_cast<uint16_t>(white) * (static_cast<uint16_t>(cct) + 1U)) >> 8);
  const uint8_t ww = static_cast<uint8_t>((static_cast<uint16_t>(white) * (256U - static_cast<uint16_t>(cct))) >> 8);

  r = qadd8(r, ww);
  g = qadd8(g, scale8(ww, 200));
  b = qadd8(b, scale8(ww, 120));

  r = qadd8(r, scale8(cw, 120));
  g = qadd8(g, scale8(cw, 200));
  b = qadd8(b, cw);

  palette_loaded->solid_colour.colourRGBW = RGBW32(r, g, b, 0);
  palette_loaded->solid_colour.whiteWW = 0;
  return;
}

palette_loaded->solid_colour.colourRGBW = colour_out.colour;
palette_loaded->solid_colour.whiteWW = colour_out.cct;

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
      const float el_max =  45.0f;
    #endif

    uint16_t pal_index = 0;

    if (preview_mode)
    {
      const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
      pal_index = (N <= 1) ? 0 : (uint16_t)((uint32_t)preview_index * 255UL / (uint32_t)(N - 1));
    }
    else
    {
      float eval_elevation = constrain(elevation, el_min, el_max);
      pal_index = (uint16_t)mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 255.0f);
      pal_index = constrain(pal_index, 0, 255);
    }

    // Uses your existing "data" palette + encoder path (same as old switch case)
    const uint16_t palette_adjusted_id_rel0 = pid - mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START;
    uint8_t* data_local = &mPaletteI->dynamic_palettes[palette_adjusted_id_rel0].data[0];

    bool force_palette_mode = false;

    uint32_t c32 = mPaletteI->SubGet_Encoded_Palette_Colour_U32(
      data_local,
      pal_index,
      palette_loaded->encoded_colour_width,
      palette_loaded->colours_in_palette,
      mPaletteI->dynamic_palettes[palette_adjusted_id_rel0].encoding,
      nullptr,
      false,
      /*rescale wrap*/ 0,
      /*force*/ force_palette_mode,
      false
    );

    palette_loaded->solid_colour.colourRGBW = c32;
    palette_loaded->solid_colour.whiteWW    = 0;
    return;
  }

    // ------------------------------------------------------------------
  // LIVE: Solar Elevation -> Gradient colour of sky (preview only)
  // ------------------------------------------------------------------
  // ------------------------------------------------------------------
// LIVE: Solar Elevation - Gradient Colour Of Sky (CRGBPalette16 preloaded)
// ------------------------------------------------------------------
if (pid == mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID)
{
  // Ensure these are sane for your CRGBPalette16 path
  // palette_loaded->colours_in_palette = 16;
  // palette_loaded->encoded_colour_width = 3; // RGB

  // Preview: just emit a single slice into solid_colour (serializer GET path reads solid_colour)
  if (preview_mode)
  {
    // preview_index is 0..(N-1) from serializer. If your WebUI sets "2", it'll be 0/1.
    const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
    const uint8_t  idx255 = (N <= 1) ? 0 : (uint8_t)((preview_index * 255U) / (uint16_t)(N - 1));

    // For preview you can either:
    // A) sample the already-built CRGB16 palette_loaded (preferred if runtime has built it)
    // B) build it here too (safe). We'll do B for correctness.
  }

  // --- compute el_min/el_max/elevation as you already do ---
  #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
    const float elevation = tkr_solar->Get_Elevation();
    const float el_min = tkr_solar->Get_Elevation_Min();
    const float el_max = tkr_solar->Get_Elevation_Max();
  #else
    const float elevation = 0.0f;
    const float el_min = -45.0f;
    const float el_max =  45.0f;
  #endif

  // Zoom window (same logic you had)
  float zoom_ratio = custom1 / 255.0f;
  zoom_ratio = constrain(zoom_ratio, 0.01f, 1.0f);

  float zoom_range = (el_max - el_min) * zoom_ratio;
  float el_start = elevation - (zoom_range * 0.5f);
  float el_end   = elevation + (zoom_range * 0.5f);

  el_start = constrain(el_start, el_min, el_max);
  el_end   = constrain(el_end,   el_min, el_max);

  uint16_t palette_start = (uint16_t)mSupport::mapfloat(el_start, el_min, el_max, 0.0f, 255.0f);
  uint16_t palette_end   = (uint16_t)mSupport::mapfloat(el_end,   el_min, el_max, 0.0f, 255.0f);

  if (palette_start >= palette_end) palette_start = (palette_end > 0) ? (palette_end - 1) : 0;

  // Build CRGBPalette16: 16 samples across the zoomed region
  for (uint8_t i = 0; i < 16; i++)
  {
    const uint16_t idx = (uint16_t)((i * 255U) / 15U); // 0..255
    const uint16_t pal_idx = (uint16_t)mSupport::mapfloat((float)idx, 0.0f, 255.0f, (float)palette_start, (float)palette_end);

    // Your existing encoded palette_loaded data lives in `data` for this dynamic palette_loaded.
    // Use your existing decoder to fetch RGB at pal_idx.
    // NOTE: flag_request_is_for_full_visual_output=false here; we’re generating the underlying palette_loaded.
    uint8_t dummy_enc = 0;
    const uint16_t rel = (uint16_t)(pid - mPalette::PALETTELIST_DYNAMIC__COLOUR__ID_START);
    uint8_t* data_2 = &mPaletteI->dynamic_palettes[rel].data[0];

    const uint32_t c32 = mPaletteI->SubGet_Encoded_Palette_Colour_U32(
      data_2,
      (uint16_t)constrain(pal_idx, 0, 255),
      palette_loaded->encoded_colour_width,
      palette_loaded->colours_in_palette,
      mPaletteI->dynamic_palettes[rel].encoding,
      &dummy_enc,
      false,
      0,
      PALETTE_MODE__FORCE_GRADIENT,
      false
    );

    // Store into CRGBPalette16
    palette_loaded->CRGB16Palette16_Palette.data[i] = CRGB(R(c32), G(c32), B(c32));
  }

  if (preview_mode)
  {
    const uint16_t N = (palette_loaded && palette_loaded->colours_in_palette) ? palette_loaded->colours_in_palette : 16;
    const uint8_t idx255 = (N <= 1) ? 0 : (uint8_t)((preview_index * 255U) / (uint16_t)(N - 1));

    const CRGB c = mPaletteI->ColorFromPaletteU32(palette_loaded->CRGB16Palette16_Palette.data, idx255, 255, LINEARBLEND);
    palette_loaded->solid_colour.colourRGBW = RGBW32(c.r, c.g, c.b, 0);
    palette_loaded->solid_colour.whiteWW    = 0;
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



void mAnimatorLight::Init_Segments()
{ 

  //reset segment runtimes
  ALOG_ERR(PSTR("Phase out: duplicate code"));
  
  //reset segment runtimes
  for (Segment &seg : segments) 
  {
    // ALOG_ERR(PSTR("seg.markForReset();A"));  
    seg.markForReset();
    seg.resetIfRequired();
  }

  for (Segment &seg : segments) 
  {    
    // ALOG_ERR(PSTR("seg.markForReset();B"));

    seg.grouping_set(1);

    // SEGMENT_I(i).setOption(SEG_OPTION_ON, 1);
    // SEGMENT_I(i).opacity = 255;    
  }
  
}


// void mAnimatorLight::Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index)
// {

//   ALOG_DBG(PSTR("Segment_AppendNew:: getSegmentsNum() new index %d of count:%d"), seg_index, getSegmentsNum());

//   if (seg_index >= getSegmentsNum()) 
//   {

//     ALOG_DBG(PSTR("Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
    

//     char buffer[100];
//     sprintf(buffer, "Segment %d", seg_index);

//     Segment seg = Segment(start_pixel, stop_pixel, buffer);

//     appendSegment(seg);
//     seg_index = getSegmentsNum()-1; // segments are added at the end of list, -1 for index of LENGTH minus 1
//     ALOG_DBG(PSTR("Segment_AppendNew::new seg_index %d"), seg_index);
      
//   }
//   else
//   {  
//     ALOG_DBG(PSTR("ELSEEEEEEEEEEEEEEEEEEEE Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
//   }

// }
void mAnimatorLight::Segment_AppendNew(
  uint16_t start_pixel,
  uint16_t stop_pixel,
  uint8_t seg_index
)
{
  ALOG_DBG(    PSTR("Segment_AppendNew:: getSegmentsNum() new index %d of count:%d"),    seg_index,    getSegmentsNum()  );

  if (seg_index >= getSegmentsNum())
  {
    ALOG_DBG(      PSTR("Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "),      seg_index,      getSegmentsNum()    );

    char buffer[100];
    snprintf(
      buffer,
      sizeof(buffer),
      "Segment %d",
      seg_index
    );

    // Removed:
    // Segment seg = Segment(start_pixel, stop_pixel, buffer);
    // appendSegment(seg);

    segments.emplace_back(
      start_pixel,
      stop_pixel,
      buffer
    );

    seg_index = getSegmentsNum() - 1;

    ALOG_DBG(      PSTR("Segment_AppendNew::new seg_index %d"),      seg_index    );
  }
  else
  {
    ALOG_DBG(      PSTR("ELSEEEEEEEEEEEEEEEEEEEE Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "),      seg_index,     getSegmentsNum()    );
  }
}

/**
 * @brief Disables animator neopixel and configures so effect function will be called again
 * 
 * @param seg_i 
 */
void mAnimatorLight::SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i)
{
  // SEGMENT_I(seg_i).anim_function_callback = nullptr; // disabled
  // SEGMENT_I(seg_i).transitional = true;              // Set that basic manual animator will be called
  // SEGMENT_I(seg_i).cycle_time__rate_ms = FRAMETIME;
}


/***
  Uses std::find: This ensures we search effects.id to see if id already exists before modifying an entry.
  Ensures id aligns with index: By locating the existing index via std::distance, we can update only the correct entry.
  Correctly increments effects.count: Now, effects.count reflects the actual number of effects stored, preventing inconsistencies due to sparse enums.

  This ensures that id is only used as an identifier, and the true vector index is correctly managed.
 */
// #ifdef ENABLE_EFFECT_DESCRIPTIONS
// void mAnimatorLight::addEffect(uint8_t id, EffectFunction function, const char* effect_config, const char* effect_description, uint8_t development_stage)
// #else
// void mAnimatorLight::addEffect(uint8_t id, EffectFunction function, const char* effect_config, uint8_t development_stage)
// #endif
// {
//   // Find the index in effects.id where the given id exists
//   auto it = std::find(effects.id.begin(), effects.id.end(), id);

//   if (it != effects.id.end()) {
//     // If found, update the existing entry
//     size_t index = std::distance(effects.id.begin(), it);
//     effects.function[index] = function;
//     effects.config[index] = effect_config;
//     effects.development_stage[index] = development_stage;
//     #ifdef ENABLE_EFFECT_DESCRIPTIONS
//     effects.description[index] = effect_description;
//     #endif
//   } else {
//     // If not found, append a new entry
//     effects.function.push_back(function);
//     effects.config.push_back(effect_config);
//     effects.development_stage.push_back(development_stage);
//     #ifdef ENABLE_EFFECT_DESCRIPTIONS
//     effects.description.push_back(effect_description);
//     #endif
//     effects.id.push_back(id);
//     effects.count = effects.id.size();
//   }
//   ALOG_INF(PSTR())
// }

// #ifdef ENABLE_EFFECT_DESCRIPTIONS
// void mAnimatorLight::addEffect(uint8_t id, EffectFunction function, const char* effect_config, const char* effect_description, uint8_t development_stage)
// #else
// void mAnimatorLight::addEffect(uint8_t id, EffectFunction function, const char* effect_config, uint8_t development_stage)
// #endif
// {
//   char function_pointer_hex[(sizeof(EffectFunction) * 2u) + 1u];
//   const uint8_t* function_pointer_bytes = reinterpret_cast<const uint8_t*>(&function);
//   for (size_t byte_index = 0; byte_index < sizeof(EffectFunction); byte_index++) {
//     snprintf(&function_pointer_hex[byte_index * 2u], 3u, "%02X", function_pointer_bytes[byte_index]);
//   }
//   function_pointer_hex[sizeof(EffectFunction) * 2u] = '\0';

//   // Find the index in effects.id where the given id exists
//   auto it = std::find(effects.id.begin(), effects.id.end(), id);

//   if (it != effects.id.end()) {
//     // If found, update the existing entry
//     size_t index = std::distance(effects.id.begin(), it);
//     effects.function[index] = function;
//     effects.config[index] = effect_config;
//     effects.development_stage[index] = development_stage;
//     // #ifdef ENABLE_EFFECT_DESCRIPTIONS
//     // effects.description[index] = nullptr;//effect_description;
//     // #endif

//     ALOG_INF(PSTR("FX REG UPDATE: id=%u index=%u stage=%u fn_size=%u fn_raw=%s config=%p count=%u"), id, static_cast<unsigned>(index), development_stage, static_cast<unsigned>(sizeof(EffectFunction)), function_pointer_hex, static_cast<const void*>(effect_config), static_cast<unsigned>(effects.id.size()));
//   } else {
//     // If not found, append a new entry
//     effects.function.push_back(function);
//     effects.config.push_back(effect_config);
//     effects.development_stage.push_back(development_stage);
//     // #ifdef ENABLE_EFFECT_DESCRIPTIONS
//     // effects.description.push_back(nullptr);//effect_description);
//     // #endif
//     effects.id.push_back(id);
//     // effects.count = effects.id.size();

//     const size_t index = effects.id.size() - 1u;

//     ALOG_INF(PSTR("FX REG ADD: id=%u index=%u stage=%u fn_size=%u fn_raw=%s config=%p count=%u"), id, static_cast<unsigned>(index), development_stage, static_cast<unsigned>(sizeof(EffectFunction)), function_pointer_hex, static_cast<const void*>(effect_config), static_cast<unsigned>(effects.id.size()));
//   }
// }

#ifdef ENABLE_EFFECT_DESCRIPTIONS
void mAnimatorLight::addEffect(uint16_t id, EffectFunction function, const char* effect_config, const char* effect_description, uint8_t development_stage)
#else
void mAnimatorLight::addEffect(uint16_t id, EffectFunction function, const char* effect_config, uint8_t development_stage)
#endif
{
  const size_t function_count = effects.function.size();
  const size_t config_count = effects.config.size();
  const size_t stage_count = effects.development_stage.size();
  const size_t id_count = effects.id.size();

  bool vectors_aligned =
    function_count == id_count &&
    config_count == id_count &&
    stage_count == id_count;

  #ifdef ENABLE_EFFECT_DESCRIPTIONS
  const size_t description_count = effects.description.size();
  vectors_aligned = vectors_aligned && description_count == id_count;
  #endif

  if (!vectors_aligned)
  {
    ALOG_ERR(
      PSTR("FX REG CORRUPT BEFORE: fn=%u cfg=%u stage=%u id=%u"
        #ifdef ENABLE_EFFECT_DESCRIPTIONS
        " desc=%u"
        #endif
      ),
      static_cast<unsigned>(function_count),
      static_cast<unsigned>(config_count),
      static_cast<unsigned>(stage_count),
      static_cast<unsigned>(id_count)
      #ifdef ENABLE_EFFECT_DESCRIPTIONS
      , static_cast<unsigned>(description_count)
      #endif
    );

    return;
  }

  auto it = std::find(effects.id.begin(), effects.id.end(), id);

  if (it != effects.id.end())
  {
    const size_t index = static_cast<size_t>(std::distance(effects.id.begin(), it));

    if (index >= effects.function.size() ||
        index >= effects.config.size() ||
        index >= effects.development_stage.size()
        #ifdef ENABLE_EFFECT_DESCRIPTIONS
        || index >= effects.description.size()
        #endif
       )
    {
      ALOG_ERR(
        PSTR("FX REG UPDATE OOB: id=%u index=%u fn=%u cfg=%u stage=%u ids=%u"
          #ifdef ENABLE_EFFECT_DESCRIPTIONS
          " desc=%u"
          #endif
        ),
        id,
        static_cast<unsigned>(index),
        static_cast<unsigned>(effects.function.size()),
        static_cast<unsigned>(effects.config.size()),
        static_cast<unsigned>(effects.development_stage.size()),
        static_cast<unsigned>(effects.id.size())
        #ifdef ENABLE_EFFECT_DESCRIPTIONS
        , static_cast<unsigned>(effects.description.size())
        #endif
      );

      return;
    }

    effects.function[index] = function;
    effects.config[index] = effect_config;
    effects.development_stage[index] = development_stage;

    #ifdef ENABLE_EFFECT_DESCRIPTIONS
    effects.description[index] = effect_description;
    #endif

    ALOG_INF(
      PSTR("FX REG UPDATE: id=%u index=%u stage=%u count=%u"),
      id,
      static_cast<unsigned>(index),
      development_stage,
      static_cast<unsigned>(effects.id.size())
    );
  }
  else
  {
    effects.function.push_back(function);
    effects.config.push_back(effect_config);
    effects.development_stage.push_back(development_stage);

    #ifdef ENABLE_EFFECT_DESCRIPTIONS
    effects.description.push_back(effect_description);
    #endif

    effects.id.push_back(id);

    const size_t new_count = effects.id.size();

    bool append_valid =
      effects.function.size() == new_count &&
      effects.config.size() == new_count &&
      effects.development_stage.size() == new_count;

    #ifdef ENABLE_EFFECT_DESCRIPTIONS
    append_valid = append_valid && effects.description.size() == new_count;
    #endif

    if (!append_valid)
    {
      ALOG_ERR(
        PSTR("FX REG CORRUPT AFTER ADD: id=%u fn=%u cfg=%u stage=%u ids=%u"
          #ifdef ENABLE_EFFECT_DESCRIPTIONS
          " desc=%u"
          #endif
        ),
        id,
        static_cast<unsigned>(effects.function.size()),
        static_cast<unsigned>(effects.config.size()),
        static_cast<unsigned>(effects.development_stage.size()),
        static_cast<unsigned>(effects.id.size())
        #ifdef ENABLE_EFFECT_DESCRIPTIONS
        , static_cast<unsigned>(effects.description.size())
        #endif
      );

      return;
    }

    const size_t index = new_count - 1u;

    ALOG_INF(
      PSTR("FX REG ADD: id=%u index=%u stage=%u count=%u"),
      id,
      static_cast<unsigned>(index),
      development_stage,
      static_cast<unsigned>(new_count)
    );
  }

}




void mAnimatorLight::SubTask_Effects()
{
// ALOG_INF(PSTR("eFX GUARD=%08X"), effects_guard);
  uint32_t nowUp = millis();            // Be aware, millis() rolls over every 49 days
  unsigned long elapsed = nowUp - _lastShow;
  bool timeToShow = (elapsed >= _frametime);                        // all segments are running at the same speed
  if (_triggered || _targetFps == FPS_UNLIMITED) timeToShow = true; // unlimited mode = no frametime; strip.trigger() can overrule timing

  effect_start_time = nowUp + timebase;                               // common time base for all effects
  if (!timeToShow) return;                              // too early for service
  if (_suspend || elapsed <= MIN_FRAME_DELAY) return;   // keep wifi alive - no matter if triggered or unlimited

  // Do not write pixel buffers while asynchronous output is active
  if (isUpdating()) return;

  _isServicing = true;
  bool doShow = _triggered;    // true if ≥1 active segment was processed (and strip was not suspended mid-loop), or trigger received → triggers show()
  for (size_t i = 0; i < segments.size(); i++) {
    Segment &seg = segments[i];
    segment_index = i;

    // seg.effect_id = 1;

    // tkr_iLight->_briRGB_Global = 255;
    // tkr_iLight->_briCT_Global = 255;
    // seg.opacity = 255;


    // Process transition (also pre-calculates progress value)
    seg.handleTransition();

    // Reset effect runtime data if requested
    seg.resetIfRequired();
    
    if (seg.isActive())
    {
      doShow = true;

      if(!seg.freeze)
      {
        ALOG_DBM_IF(seg.name, PSTR("Segment if %s [%d,%d]"), seg.name, seg.start, seg.stop);

        #ifdef ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES
        const bool is_personal_palette =
          (seg.palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (seg.palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID)
          ||
          (seg.palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED) && (seg.palette_id < mPaletteI->GetPaletteListLength());
        frame_use_gamma_correction = !is_personal_palette;
        #endif
        #ifdef ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION
        frame_use_gamma_correction = false;
        #endif

        /*
        * Current transition progress:
        *   0       = transition just started
        *   0xFFFF  = no transition or transition complete
        */
        const uint16_t prog = seg.progress();

        /*
        * Select the current/new segment as the active effect context.
        */
        _currentSegment = &seg;
      // _virtualSegmentLength = seg.virtualLength();

        // seg.UpdateBrightness();   // TO REMOVE
        seg.Update_LivePalettes();

        /*
        * Prepare virtual dimensions, colours and palette.
        * For FADE transitions this also prepares transitioned colours/palette.
        */
        seg.beginDraw(prog);

        #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
        seg.performance.effect_build_us = micros();
        #endif
        #ifdef ENABLE_EFFECTS_TIMING_DEBUG_GPIO
        DEBUG_PIN1_SET(LOW);
        #endif

        // seg.animation_has_anim_callback = false;

        // ALOG_INF(PSTR("seg.effect_id %d/%d"), seg.effect_id, effects.function.size());

        #ifdef ENABLE_DEVFEATURE_LIGHT__SERIAL_SHOW_PRE_EFFECT_CALL
        Serial.println("Pre Effect Call -------------------------------------------------------------");
        #endif

        // seg.next_time = nowUp + FRAMETIME;
        // ALOG_INF(PSTR("FX CALL: index=%u registered_id=%u fn=%p"), seg.effect_id, effects.id[seg.effect_id], *(void**)(&effects.function[seg.effect_id]));
        // (this->*effects.function[seg.effect_id])();

        (this->*effects.function[seg.effect_id])(); // Call Effect Function (passes and returns nothing)
        
        seg.call++;

        /*
        * Run the old effect when transition blending requires a full old segment (ie when colour blending alone does not replicate pre/transitioning mapping to a new effect)
        * Code below only runs when we are transitioning into a new segment?
        * The old effect writes into segO->pixels[].
        * blendSegment() later combines segO->pixels[] and seg.pixels[].
        */
        Segment *segO = seg.getOldSegment();
        if (segO && segO->isActive() && (seg.effect_id != segO->effect_id || blendingStyle != TRANSITION_FADE ||
            (segO->name != seg.name && segO->name && seg.name && strncmp(segO->name, seg.name, WLED_MAX_SEGNAME_LEN) != 0))) {
          Segment::modeBlend(true);         // set flag for beginDraw() to blend colors and palette
          segO->beginDraw(prog);            // set up palette & colors (also sets draw dimensions), parent segment has transition progress
          _currentSegment = segO;           // set current segment
          // workaround for on/off transition to respect blending style
          (this->*effects.function[segO->effect_id])();
          segO->call++;                     // increment old mode run counter
          Segment::modeBlend(false);        // unset flag
        }
      } // END freeze
    } // END isActive
    
  } // END segments iterate


  /*
   * Restore a safe active-segment context.
   */
  segment_index = 0;
  _currentSegment = &segments[0]; // safe fallback to prevent stale pointer

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
  if ((_targetFps != FPS_UNLIMITED) && (millis() - nowUp > _frametime))  {
    ALOG_DBG(PSTR("Slow effects %u/%d"), (unsigned)(millis() - nowUp), (int)_frametime);
  }
  #endif

  /*
   * show() clears global _pixels[], calls blendSegment() for every segment,
   * writes global _pixels[] into BusManager and transmits the frame.
   */
  if (doShow)
  {
    yield();
    show();
    _lastShow = nowUp;
  }

  _triggered = false;
  _isServicing = false;

}


// https://en.wikipedia.org/wiki/Blend_modes but using a for top layer & b for bottom layer
static uint8_t _top       (uint8_t a, uint8_t b) { return a; } // function unused
static uint8_t _bottom    (uint8_t a, uint8_t b) { return b; } // function unused
static uint8_t _add       (uint8_t a, uint8_t b) { unsigned t = a + b; return t > 255 ? 255 : t; } // function unused
static uint8_t _subtract  (uint8_t a, uint8_t b) { return b > a ? (b - a) : 0; }
static uint8_t _difference(uint8_t a, uint8_t b) { return b > a ? (b - a) : (a - b); }
static uint8_t _average   (uint8_t a, uint8_t b) { return (a + b) >> 1; }
#if defined(ESP8266) || defined(CONFIG_IDF_TARGET_ESP32C3)
static uint8_t _multiply  (uint8_t a, uint8_t b) { return ((a * b) + 255) >> 8; } // faster than division on C3 but slightly less accurate
#else
static uint8_t _multiply  (uint8_t a, uint8_t b) { return (a * b) / 255; } // origianl uses a & b in range [0,1]
#endif
static uint8_t _divide    (uint8_t a, uint8_t b) { return a > b ? (b * 255) / a : 255; }
static uint8_t _lighten   (uint8_t a, uint8_t b) { return a > b ? a : b; }
static uint8_t _darken    (uint8_t a, uint8_t b) { return a < b ? a : b; }
static uint8_t _screen    (uint8_t a, uint8_t b) { return 255 - _multiply(~a,~b); } // 255 - (255-a)*(255-b)/255
static uint8_t _overlay   (uint8_t a, uint8_t b) { return b < 128 ? 2 * _multiply(a,b) : (255 - 2 * _multiply(~a,~b)); }
static uint8_t _hardlight (uint8_t a, uint8_t b) { return a < 128 ? 2 * _multiply(a,b) : (255 - 2 * _multiply(~a,~b)); }
#if defined(ESP8266) || defined(CONFIG_IDF_TARGET_ESP32C3)
static uint8_t _softlight (uint8_t a, uint8_t b) { return (((b * b * (255 - 2 * a))) + ((2 * a * b + 256) << 8)) >> 16; } // Pegtop's formula (1 - 2a)b^2
#else
static uint8_t _softlight (uint8_t a, uint8_t b) { return (b * b * (255 - 2 * a) + 255 * 2 * a * b) / (255 * 255); } // Pegtop's formula (1 - 2a)b^2 + 2ab
#endif
static uint8_t _dodge     (uint8_t a, uint8_t b) { return _divide(~a,b); }
static uint8_t _burn      (uint8_t a, uint8_t b) { return ~_divide(a,~b); }
static uint8_t _stencil   (uint8_t a, uint8_t b) { return a ? a : b; } // function unused
static uint8_t _dummy     (uint8_t a, uint8_t b) { return a; } // dummy (same as _top) to fill the function list and make it safe from OOB access

#define BLENDMODES  17 // number of blend modes must match "bm" in index.js, all cases must be handled in segblend() @ blendSegment()

// void mAnimatorLight::blendSegment(const Segment &topSegment) const {
  
//   // DEBUG_PRINT_LN("blend start");

//   typedef uint8_t(*FuncType)(uint8_t, uint8_t);
//   // function pointer array: fill with _dummy if using special case: avoid OOB access and always provide a valid path
//   // note: making the function array static const uses more ram and comes at no significant speed gain
//   FuncType funcs[] = {
//     _dummy,      _dummy,     _dummy,    _subtract,
//     _difference, _average,   _dummy,    _divide,
//     _lighten,    _darken,    _screen,   _overlay,
//     _hardlight,  _softlight, _dodge,    _burn,
//     _dummy
//   };

//   const size_t blendMode = topSegment.blendMode < BLENDMODES ? topSegment.blendMode : 0; // default to top if unsupported mode
//   const auto segblend = [&](uint32_t t, uint32_t b){
//     // use direct calculations/returns for simple/frequent modes (faster)
//     switch (blendMode) {
//       case 0 : return t;                   // top
//       case 1 : return b;                   // bottom
//       case 2 : return Segment::color_add(t,b,true); // add with preserve color ratio to avoid color clipping
//       case 6 : return RGBW32(_multiply(R(t),R(b)), _multiply(G(t),G(b)), _multiply(B(t),B(b)), _multiply(W(t),W(b))); // multiply (7% faster than lambda at 100bytes flash cost)
//       case 16: return t ? t : b;           // stencil (use top layer if not black, else bottom)
//     }
//     // default: use function pointer from array
//     const auto func = funcs[blendMode];
//     return RGBW32(func(R(t),R(b)), func(G(t),G(b)), func(B(t),B(b)), func(W(t),W(b)));
//   };

//   const int     length     = topSegment.length();     // physical segment length (counts all pixels in 2D segment)
//   const int     width      = topSegment.width();
//   const int     height     = topSegment.height();
//   //const uint32_t bgColor   = topSegment.colors[1]; // background color (unused, could add it to stencil mode if requested)
//   const auto    XY         = [](int x, int y){ return x + y*Segment::maxWidth; };
//   const size_t  matrixSize = Segment::maxWidth * Segment::maxHeight;
//   const size_t  startIndx  = XY(topSegment.start, topSegment.startY);
//   const size_t  stopIndx   = startIndx + length;
//   uint8_t       opacity    = topSegment.currentBri(); // returns transitioned opacity for style FADE
//   uint8_t       cct        = topSegment.currentCCT();
//   if (gammaCorrectCol) opacity = gamma8inv(opacity); // use inverse gamma on brightness for correct color scaling after gamma correction (see #5343 for details)

//   const Segment *segO = topSegment.getOldSegment();
//   const bool hasGrouping = topSegment.groupLength() != 1;

//   opacity = 255;

// //   if (topSegment.progress() > 63000U)
// // {
// //   Serial.printf(
// //     "BLEND IN prog=%u bri=%u opacity=%u old=%08lX new=%08lX\n",
// //     topSegment.progress(),
// //     topSegment.currentBri(),
// //     opacity,
// //     (unsigned long)(segO ? segO->getPixelColorRaw(0) : 0),
// //     (unsigned long)topSegment.getPixelColorRaw(0)
// //   );
// // }


//   // ALOG_INF(PSTR("seg0 %d, blendStyle %d, hasGrouping %d, mirror %d, mirror_y %d"), segO, blendingStyle, hasGrouping, topSegment.mirror, topSegment.mirror_y);
//   // ALOG_INF(
//   //   PSTR(
//   //     "segO=%p len%d, transition=%u blendStyle=%u fade=%u "
//   //     "grouping=%u mirror=%u mirrorY=%u isMatrix=%u"
//   //   ),
//   //   (void*)segO,
//   //   length,
//   //   (unsigned)topSegment.isInTransition(),
//   //   (unsigned)blendingStyle,
//   //   (unsigned)TRANSITION_FADE,
//   //   (unsigned)hasGrouping,
//   //   (unsigned)topSegment.mirror,
//   //   (unsigned)topSegment.mirror_y,
//   //   (unsigned)isMatrix
//   // );

//   // fast path: handle the default case - no transitions, no grouping/spacing, no mirroring, no CCT
//   if (!segO && blendingStyle == TRANSITION_FADE && !hasGrouping && !topSegment.mirror && !topSegment.mirror_y) {
//     // DEBUG_PRINT_LN("Fast Path");
//     if (isMatrix && stopIndx <= matrixSize && !_pixelCCT) {
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS // WLED_DISABLE_2D
//       // Calculate pointer steps to avoid 'if' and 'XY()' inside loops
//       int x_inc = 1;
//       int y_inc = Segment::maxWidth;
//       int start_offset = XY(topSegment.start, topSegment.startY);
      
//       // adjust starting position and steps based on Reverse/Transpose
//       // note: transpose is handled in separate loop so it is still fast and no branching is needed in default path
//       if (!topSegment.transpose) {
//         if (topSegment.reverse)   { start_offset += (width - 1); x_inc = -1; }
//         if (topSegment.reverse_y) { start_offset += (height - 1) * Segment::maxWidth; y_inc = -Segment::maxWidth; }
        
//         for (int y = 0; y < height; y++) {
//           uint32_t* pRow = &_pixels[start_offset + y * y_inc];
//           const int y_width = y * width;
//           for (int x = 0; x < width; x++) {
//             uint32_t* p = pRow + x * x_inc;
//             uint32_t c_a = topSegment.getPixelColorRaw(x + y_width);
//             *p = Segment::color_blend(*p, segblend(c_a, *p), opacity);
//           }
//         }
//       } else { // transposed
        
//         for (int y = 0; y < height; y++) {
//           const int px = topSegment.reverse ? (height - y - 1) : y;  // source pixel: swap y into x, reverse if needed
//           for (int x = 0; x < width; x++) {
//             const int py = topSegment.reverse_y ? (width  - x - 1) : x;  // source pixel: swap x into y, reverse if needed
//             const uint32_t c_a = topSegment.getPixelColorRaw(px + py * height); // height = virtual width
//             const size_t idx = XY(topSegment.start + x, topSegment.startY + y); // write logical (non swapped) pixel coordinate
//             _pixels[idx] = Segment::color_blend(_pixels[idx], segblend(c_a, _pixels[idx]), opacity);
//           }
//         }
//       }
//       return;
// #endif
//     } else 
//     if (!isMatrix) {
//       // DEBUG_PRINT_LN("1D Fast Path");
//       // 1D fast path, include CCT as it is more common on 1D setups
//       // pixels[] -> _pixels[]
//       uint32_t* strip = _pixels;
//       int start = topSegment.start;
//       int off   = topSegment.offset;
//       // ALOG_INF(PSTR("stop %d"), topSegment.stop);
//       for (int i = 0; i < length; i++) {
//         uint32_t c_a = topSegment.getPixelColorRaw(i);
//         int p = topSegment.reverse ? (length - i - 1) : i;
//         int idx = start + p + off;
//         if (idx >= topSegment.stop) idx -= length;
//         strip[idx] = Segment::color_blend(strip[idx], segblend(c_a, strip[idx]), opacity);
//         // strip[idx] = c_a;
//         // ALOG_INF(PSTR("idx i %d %d"), idx, i);
//         // if (_pixelCCT) _pixelCCT[idx] = cct;

//       }
//       return;
//     }
//   }else{
//     // DEBUG_PRINT_LN("!Fast Path");
//   }


//   // slow path: handle transitions, grouping/spacing, segments with clipping and CCT pixels
//   Segment::setClippingRect(0, 0);  // disable clipping by default
//   const unsigned progress = topSegment.progress();
//   const unsigned progInv  = 0xFFFFU - progress;
//   const unsigned dw = (blendingStyle==TRANSITION_OUTSIDE_IN ? progInv : progress) * width / 0xFFFFU + 1;
//   const unsigned dh = (blendingStyle==TRANSITION_OUTSIDE_IN ? progInv : progress) * height / 0xFFFFU + 1;
//   const unsigned orgBS = blendingStyle;
//   if (width*height == 1) tkr_anim->blendingStyle = TRANSITION_FADE; // disable style for single pixel segments (use fade instead)
//   switch (tkr_anim->blendingStyle) {
//     case TRANSITION_CIRCULAR_IN: // (must set entire segment, see isPixelXYClipped())
//     case TRANSITION_CIRCULAR_OUT:// (must set entire segment, see isPixelXYClipped())
//     case TRANSITION_FAIRY_DUST:  // fairy dust (must set entire segment, see isPixelXYClipped())
//       Segment::setClippingRect(0, width, 0, height);
//       break;
//     case TRANSITION_SWIPE_RIGHT: // left-to-right
//     case TRANSITION_PUSH_RIGHT:  // left-to-right
//       Segment::setClippingRect(0, dw, 0, height);
//       break;
//     case TRANSITION_SWIPE_LEFT:  // right-to-left
//     case TRANSITION_PUSH_LEFT:   // right-to-left
//       Segment::setClippingRect(width - dw, width, 0, height);
//       break;
//     case TRANSITION_OUTSIDE_IN:   // corners
//       Segment::setClippingRect((width + dw)/2, (width - dw)/2, (height + dh)/2, (height - dh)/2); // inverted!!
//       break;
//     case TRANSITION_INSIDE_OUT:  // outward
//       Segment::setClippingRect((width - dw)/2, (width + dw)/2, (height - dh)/2, (height + dh)/2);
//       break;
//     case TRANSITION_SWIPE_DOWN:  // top-to-bottom (2D)
//     case TRANSITION_PUSH_DOWN:   // top-to-bottom (2D)
//       Segment::setClippingRect(0, width, 0, dh);
//       break;
//     case TRANSITION_SWIPE_UP:    // bottom-to-top (2D)
//     case TRANSITION_PUSH_UP:     // bottom-to-top (2D)
//       Segment::setClippingRect(0, width, height - dh, height);
//       break;
//     case TRANSITION_OPEN_H:      // horizontal-outward (2D) same look as INSIDE_OUT on 1D
//       Segment::setClippingRect((width - dw)/2, (width + dw)/2, 0, height);
//       break;
//     case TRANSITION_OPEN_V:      // vertical-outward (2D)
//       Segment::setClippingRect(0, width, (height - dh)/2, (height + dh)/2);
//       break;
//     case TRANSITION_SWIPE_TL:    // TL-to-BR (2D)
//     case TRANSITION_PUSH_TL:     // TL-to-BR (2D)
//       Segment::setClippingRect(0, dw, 0, dh);
//       break;
//     case TRANSITION_SWIPE_TR:    // TR-to-BL (2D)
//     case TRANSITION_PUSH_TR:     // TR-to-BL (2D)
//       Segment::setClippingRect(width - dw, width, 0, dh);
//       break;
//     case TRANSITION_SWIPE_BR:    // BR-to-TL (2D)
//     case TRANSITION_PUSH_BR:     // BR-to-TL (2D)
//       Segment::setClippingRect(width - dw, width, height - dh, height);
//       break;
//     case TRANSITION_SWIPE_BL:    // BL-to-TR (2D)
//     case TRANSITION_PUSH_BL:     // BL-to-TR (2D)
//       Segment::setClippingRect(0, dw, height - dh, height);
//       break;
//   }

//   if (isMatrix && stopIndx <= matrixSize) {
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//     const int nCols = topSegment.virtualWidth();
//     const int nRows = topSegment.virtualHeight();
//     const int oCols = segO ? segO->virtualWidth() : nCols;
//     const int oRows = segO ? segO->virtualHeight() : nRows;

//     const auto setMirroredPixel = [&](int x, int y, uint32_t c, uint8_t o) {
//       const int baseX = topSegment.start  + x;
//       const int baseY = topSegment.startY + y;
//       size_t indx = XY(baseX, baseY); // absolute address on strip
//       _pixels[indx] = Segment::color_blend(_pixels[indx], segblend(c, _pixels[indx]), o);
//       if (_pixelCCT) _pixelCCT[indx] = cct;
//       // Apply mirroring if enabled
//       if (topSegment.mirror || topSegment.mirror_y) {
//         const int mirrorX = topSegment.start  + width  - x - 1;
//         const int mirrorY = topSegment.startY + height - y - 1;
//         const size_t idxMX = XY(topSegment.transpose ? baseX : mirrorX, topSegment.transpose ? mirrorY : baseY);
//         const size_t idxMY = XY(topSegment.transpose ? mirrorX : baseX, topSegment.transpose ? baseY : mirrorY);
//         const size_t idxMM = XY(mirrorX, mirrorY);
//         if (topSegment.mirror)                        _pixels[idxMX] = tkr_anim->color_blend(_pixels[idxMX], segblend(c, _pixels[idxMX]), o);
//         if (topSegment.mirror_y)                      _pixels[idxMY] = tkr_anim->color_blend(_pixels[idxMY], segblend(c, _pixels[idxMY]), o);
//         if (topSegment.mirror && topSegment.mirror_y) _pixels[idxMM] = tkr_anim->color_blend(_pixels[idxMM], segblend(c, _pixels[idxMM]), o);
//         if (_pixelCCT) {
//           if (topSegment.mirror)                        _pixelCCT[idxMX] = cct;
//           if (topSegment.mirror_y)                      _pixelCCT[idxMY] = cct;
//           if (topSegment.mirror && topSegment.mirror_y) _pixelCCT[idxMM] = cct;
//         }
//       }
//     };

//     // if we blend using "push" style we need to "shift" canvas to left/right/up/down
//     unsigned offsetX = (blendingStyle == TRANSITION_PUSH_UP   || blendingStyle == TRANSITION_PUSH_DOWN)  ? 0 : progInv * nCols / 0xFFFFU;
//     unsigned offsetY = (blendingStyle == TRANSITION_PUSH_LEFT || blendingStyle == TRANSITION_PUSH_RIGHT) ? 0 : progInv * nRows / 0xFFFFU;
//     const unsigned groupLen = topSegment.groupLength();
//     bool applyReverse = topSegment.reverse || topSegment.reverse_y || topSegment.transpose;
//     int pushOffsetX = 0, pushOffsetY = 0;
//     // if we blend using "push" style we need to "shift" canvas to left/right/up/down
//     switch (blendingStyle) {
//       case TRANSITION_PUSH_RIGHT: pushOffsetX = offsetX; break;
//       case TRANSITION_PUSH_LEFT:  pushOffsetX = -offsetX + nCols; break;
//       case TRANSITION_PUSH_DOWN:  pushOffsetY = offsetY; break;
//       case TRANSITION_PUSH_UP:    pushOffsetY = -offsetY + nRows; break;
//       case TRANSITION_PUSH_TL:    pushOffsetX = offsetX;            pushOffsetY = offsetY; break;           // unused
//       case TRANSITION_PUSH_TR:    pushOffsetX = -offsetX + nCols;   pushOffsetY = offsetY; break;           // unused
//       case TRANSITION_PUSH_BR:    pushOffsetX = -offsetX + nCols;   pushOffsetY = -offsetY + nRows; break;  // unused
//       case TRANSITION_PUSH_BL:    pushOffsetX = offsetX;            pushOffsetY = -offsetY + nRows; break;  // unused
//     }
//     // we only traverse new segment, not old one
//     for (int r = 0; r < nRows; r++) for (int c = 0; c < nCols; c++) {
//       const bool clipped = topSegment.isPixelXYClipped(c, r);
//       // if segment is in transition and pixel is clipped take old segment's pixel and opacity
//       const Segment *seg = clipped && segO ? segO : &topSegment;  // pixel is never clipped for FADE
//       int vCols = seg == segO ? oCols : nCols;         // old segment may have different dimensions
//       int vRows = seg == segO ? oRows : nRows;         // old segment may have different dimensions
//       int x = c;
//       int y = r;
//       if (pushOffsetX != 0) x = (x + pushOffsetX) % nCols;
//       if (pushOffsetY != 0) y = (y + pushOffsetY) % nRows;
//       uint32_t c_a = BLACK;
//       if (x < vCols && y < vRows) c_a = seg->getPixelColorRaw(x + y*vCols); // will get clipped pixel from old segment or unclipped pixel from new segment
//       // if (segO && blendingStyle == TRANSITION_FADE
//       //   && (topSegment.effect_id != segO->effect_id || (segO->name != topSegment.name && segO->name && topSegment.name && strncmp(segO->name, topSegment.name, WLED_MAX_SEGNAME_LEN) != 0))
//       //   && x < oCols && y < oRows) {
//       //   // we need to blend old segment using fade as pixels are not clipped
//       //   c_a = color_blend16(c_a, segO->getPixelColorRaw(x + y*oCols), progInv);
//       // } else if (blendingStyle != TRANSITION_FADE) {


//       if (segO && blendingStyle == TRANSITION_FADE && x < oCols && y < oRows) {
//         // we need to blend old segment using fade as pixels are not clipped
//         c_a = color_blend16(c_a, segO->getPixelColorRaw(x + y*oCols), progInv);
//       } else if (blendingStyle != TRANSITION_FADE) {

//         // if we have global brightness change (not On/Off change) we will ignore transition style and just fade brightness (see led.cpp)
//         // workaround for On/Off transition
//         // (bri != briT) && !bri => from On to Off
//         // (bri != briT) &&  bri => from Off to On
//         uint8_t bri = tkr_iLight->_briRGB_Global;
//         if ((briOld == 0 || bri == 0) && ((!clipped && (bri != briT) && !bri) || (clipped && (bri != briT) && bri))) c_a = BLACK;
//       }
//       // map it into frame buffer
//       x = c;  // restore coordiates if we were PUSHing
//       y = r;
//       if (applyReverse) {
//         if (topSegment.reverse  ) x = nCols - x - 1;
//         if (topSegment.reverse_y) y = nRows - y - 1;
//         if (topSegment.transpose) std::swap(x,y); // swap X & Y if segment transposed
//       }
//       // expand pixel
//       if (groupLen == 1) {
//         setMirroredPixel(x, y, c_a, opacity);
//       } else {
//         // handle grouping and spacing
//         x *= groupLen; // expand to physical pixels
//         y *= groupLen; // expand to physical pixels
//         const int maxX = std::min(x + topSegment.grouping, width);
//         const int maxY = std::min(y + topSegment.grouping, height);
//         while (y < maxY) {
//           int _x = x;
//           while (_x < maxX) setMirroredPixel(_x++, y, c_a, opacity);
//           y++;
//         }
//       }
//     }
// #endif
//   } else {

//     // ALOG_INF(PSTR("SLOW PATH----------------------------"));

//     // 1D Slow Path
//     const int nLen = topSegment.virtualLength();
//     const int oLen = segO ? segO->virtualLength() : nLen;

//     const auto setMirroredPixel = [&](int i, uint32_t c, uint8_t o) {
//       int indx = topSegment.start + i;
//       // Apply mirroring
//       if (topSegment.mirror) {
//         unsigned indxM = topSegment.stop - i - 1;
//         indxM += topSegment.offset; // offset/phase
//         if (indxM >= topSegment.stop) indxM -= length; // wrap
//         _pixels[indxM] = Segment::color_blend(_pixels[indxM], segblend(c, _pixels[indxM]), o);
//         if (_pixelCCT) _pixelCCT[indxM] = cct;
//       }
//       indx += topSegment.offset; // offset/phase
//       if (indx >= topSegment.stop) indx -= length; // wrap
//       _pixels[indx] = Segment::color_blend(_pixels[indx], segblend(c, _pixels[indx]), o);
//       if (_pixelCCT) _pixelCCT[indx] = cct;

// //       if (i == 0 && topSegment.progress() > 63000U)
// // {
// //   Serial.printf(
// //     "SLOW OUT prog=%u source=%08lX out=%08lX RGBW=%u,%u,%u,%u\n",
// //     topSegment.progress(),
// //     (unsigned long)c,
// //     (unsigned long)_pixels[indx],
// //     R(_pixels[indx]),
// //     G(_pixels[indx]),
// //     B(_pixels[indx]),
// //     W(_pixels[indx])
// //   );
// // }

//     };

//     // if we blend using "push" style we need to "shift" canvas to left/right/
//     unsigned offsetI = progInv * nLen / 0xFFFFU;

//     for (int k = 0; k < nLen; k++) {
//       const bool clipped = topSegment.isPixelClipped(k);
//       // if segment is in transition and pixel is clipped take old segment's pixel and opacity
//       const Segment *seg = clipped && segO ? segO : &topSegment;  // pixel is never clipped for FADE
//       const int vLen = seg == segO ? oLen : nLen;
//       int i = k;
//       // if we blend using "push" style we need to "shift" canvas to left or right
//       switch (tkr_anim->blendingStyle) {
//         case TRANSITION_PUSH_RIGHT: i = (i + offsetI) % nLen;        break;
//         case TRANSITION_PUSH_LEFT:  i = (i - offsetI + nLen) % nLen; break;
//       }
//       uint32_t c_a = BLACK;
//       if (i < vLen) c_a = seg->getPixelColorRaw(i); // will get clipped pixel from old segment or unclipped pixel from new segment
//       // if (segO && tkr_anim->blendingStyle == TRANSITION_FADE && topSegment.effect_id != segO->effect_id && i < oLen) {
//         if (segO && tkr_anim->blendingStyle == TRANSITION_FADE && i < oLen)
//         {
//           // static uint32_t last_transition_pixel_log = 0;
//           // if (millis() - last_transition_pixel_log >= 500)
//           // {
//           //   last_transition_pixel_log = millis();
//           //   ALOG_INF(PSTR("TRANS PIXEL BLEND progress=%u inverse=%u new=%08X old=%08X out=%08X"), topSegment.progress(), progInv, c_a, segO->getPixelColorRaw(i), color_blend16(c_a, segO->getPixelColorRaw(i), progInv));
//           // }
//         // we need to blend old segment using fade as pixels are not clipped
//         c_a = color_blend16(c_a, segO->getPixelColorRaw(i), progInv);
//       } else if (tkr_anim->blendingStyle != TRANSITION_FADE) {
//         // if we have global brightness change (not On/Off change) we will ignore transition style and just fade brightness (see led.cpp)
//         // workaround for On/Off transition
//         // (bri != briT) && !bri => from On to Off
//         // (bri != briT) &&  bri => from Off to On
//         uint8_t bri = tkr_iLight->_briRGB_Global;
//         if ((briOld == 0 || bri == 0) && ((!clipped && (bri != briT) && !bri) || (clipped && (bri != briT) && bri))) c_a = BLACK;
//       }
//       // map into frame buffer
//       i = k; // restore index if we were PUSHing
//       if (topSegment.reverse) i = nLen - i - 1; // is segment reversed?
//       // expand pixel
//       i *= topSegment.groupLength();
//       // set all the pixels in the group
//       const int maxI = std::min(i + topSegment.grouping, length); // make sure to not go beyond physical length
//       while (i < maxI) setMirroredPixel(i++, c_a, opacity);
//     }
//   }

//   tkr_anim->blendingStyle = orgBS;
//   Segment::setClippingRect(0, 0);             // disable clipping for overlays
// }





/*******************************************************************************************************************************************************************************************************************
 * @brief Composes one rendered segment into the global frame buffer.
 *
 * HIGH-LEVEL RENDERING ARCHITECTURE
 * ---------------------------------
 *
 * The lighting pipeline is split into distinct stages:
 *
 *   1. Effect execution
 *      - An effect runs against a Segment.
 *      - The effect uses the segment's virtual geometry:
 *
 *          virtualLength()
 *          virtualWidth()
 *          virtualHeight()
 *
 *      - These values define the logical effect domain after grouping, mirroring,
 *        matrix mapping, and optional pixel decimation have been considered.
 *
 *   2. Segment-local rendering
 *      - Effects write colours through Segment::setPixelColor() or
 *        Segment::setPixelColorXY().
 *      - The rendered colours are stored in the segment-local pixels[] buffer.
 *      - Segment opacity and global brightness are not normally applied here.
 *
 *   3. Segment composition
 *      - blendSegment() reads the segment-local pixels[] buffer.
 *      - It maps those logical segment pixels onto the physical/global _pixels[]
 *        frame buffer.
 *      - This stage handles:
 *
 *          segment position
 *          reverse
 *          transpose
 *          grouping
 *          spacing
 *          mirroring
 *          clipping
 *          transition styles
 *          old/new segment blending
 *          segment opacity
 *          CCT metadata
 *          segment blend modes
 *          pixel decimation expansion
 *
 *   4. Global output
 *      - show() reads the completed global _pixels[] frame.
 *      - Colours are mapped to their physical buses.
 *      - Gamma correction may be applied.
 *      - Bus-level/global brightness is applied by the bus layer.
 *      - The final bus data is transmitted to the LEDs.
 *
 *
 * SEGMENT BUFFER VERSUS GLOBAL FRAME BUFFER
 * -----------------------------------------
 *
 * The Segment::pixels[] buffer contains the colours produced by the effect.
 *
 * The mAnimatorLight::_pixels[] buffer is the complete device-wide frame that
 * will ultimately be sent to all buses.
 *
 * blendSegment() is therefore the boundary between:
 *
 *      logical segment rendering
 *
 * and:
 *
 *      physical/global frame composition
 *
 * Several segments may overlap the same global pixels. The selected blend mode
 * determines how the top segment is combined with the existing global pixel.
 *
 *
 * SEGMENT OPACITY
 * ---------------
 *
 * Segment opacity is applied here rather than inside effect rendering.
 *
 *     topSegment.currentBri()
 *
 * returns the effective segment opacity, including any active opacity transition.
 *
 * The colour is combined with the existing global frame using:
 *
 *     Segment::color_blend(existing, segblend(segmentColour, existing), opacity)
 *
 * The bus/global brightness is a separate later stage and must not be folded
 * into this function.
 *
 *
 * SEGMENT BLEND MODES
 * -------------------
 *
 * segblend() applies the configured top-versus-bottom blend mode before opacity
 * is applied.
 *
 * Examples include:
 *
 *     top
 *     bottom
 *     add
 *     subtract
 *     difference
 *     multiply
 *     screen
 *     overlay
 *     stencil
 *
 * The "top" colour is the current segment colour. The "bottom" colour is the
 * pixel already present in the global _pixels[] buffer.
 *
 *
 * FAST PATH
 * ---------
 *
 * The fast path is used when the segment can be copied directly without the
 * expensive transition/grouping/mirroring logic.
 *
 * Conditions include:
 *
 *     no old transition segment
 *     fade-style composition
 *     no grouping or spacing
 *     no mirroring
 *     no mirror_y
 *
 * Separate fast paths exist for 2D and 1D.
 *
 *
 * 2D FAST PATH
 * ------------
 *
 * The 2D fast path copies the already-rendered segment buffer directly into the
 * matrix region of _pixels[].
 *
 * It supports:
 *
 *     reverse X
 *     reverse Y
 *     transpose
 *
 * Pointer increments are pre-calculated so the inner pixel loops do not need
 * repeated coordinate mapping or branching.
 *
 * Pixel decimation is deliberately not expanded here.
 *
 * Reason:
 *
 *     Segment::setPixelColorXY() is responsible for creating the complete raw
 *     segment buffer used by the normal non-transition path.
 *
 * Therefore the fast path reads physical/raw segment pixels directly with:
 *
 *     getPixelColorRaw(...)
 *
 * It must not remap those reads through virtualWidth(), virtualHeight(), modulo,
 * or another decimation transform, otherwise already-expanded segment data would
 * be transformed a second time.
 *
 *
 * 1D FAST PATH
 * ------------
 *
 * The 1D fast path copies the full raw segment buffer into the correct physical
 * output range.
 *
 * It handles:
 *
 *     segment start
 *     segment offset
 *     reverse
 *     wrap at segment stop
 *
 * Pixel decimation is deliberately not expanded here either.
 *
 * For a normal completed frame, the raw segment buffer is expected to already
 * contain the replicated decimated pattern.
 *
 * Example:
 *
 *     physical segment length = 100
 *     decimate               = 2
 *     virtualLength()        = 50
 *
 * The effect calculates logical pixels:
 *
 *     0 ... 49
 *
 * Segment::setPixelColor() stores them as:
 *
 *     raw[0 ... 49]  = calculated pattern
 *     raw[50 ... 99] = repeated pattern
 *
 * The fast path therefore performs a direct physical read:
 *
 *     getPixelColorRaw(i)
 *
 * for:
 *
 *     i = 0 ... length()-1
 *
 * No additional decimation mapping belongs in this path.
 *
 *
 * SLOW PATH
 * ---------
 *
 * The slow path is used when composition requires more than a direct copy.
 *
 * Typical reasons include:
 *
 *     an active old/new segment transition
 *     grouping
 *     spacing
 *     mirroring
 *     clipping
 *     non-fade transition styles
 *     CCT output
 *     matrix edge clipping
 *
 * The slow path operates primarily in logical/virtual segment coordinates and
 * expands those coordinates into physical output positions.
 *
 * This distinction is important for pixel decimation.
 *
 *
 * TRANSITION MODEL
 * ----------------
 *
 * During a transition, the Segment may retain an old Segment snapshot:
 *
 *     segO = topSegment.getOldSegment()
 *
 * The old and new segment can differ in:
 *
 *     effect
 *     palette
 *     opacity
 *     dimensions
 *     grouping
 *     mirroring
 *     decimation
 *
 * Depending on the transition style, the output pixel may come from:
 *
 *     the new segment
 *     the old segment
 *     a blend of both
 *     black during an on/off transition workaround
 *
 * Fade transitions blend old and new colours continuously using progress.
 *
 * Swipe, push, circular, inside-out, outside-in and related transitions use the
 * clipping rectangle to decide whether a logical position currently belongs to
 * the old or new segment.
 *
 *
 * CLIPPING RECTANGLE
 * ------------------
 *
 * Segment::setClippingRect() configures the current transition region.
 *
 * The clipping rectangle is expressed in logical segment coordinates.
 *
 * Depending on the transition style, it may represent:
 *
 *     the revealed region
 *     the hidden region
 *     an inverted centre region
 *     a moving edge
 *     a rectangular approximation of a circular transition
 *
 * Segment::isPixelClipped() and Segment::isPixelXYClipped() use this state when
 * deciding whether the old or new segment supplies the source colour.
 *
 * Clipping is reset before returning from the function.
 *
 *
 * 2D SLOW PATH
 * ------------
 *
 * The 2D slow path works in logical matrix coordinates:
 *
 *     c = logical column
 *     r = logical row
 *
 * New and old segment dimensions are stored separately:
 *
 *     nCols / nRows
 *     oCols / oRows
 *
 * This is necessary because a transition may occur while matrix dimensions or
 * segment geometry are changing.
 *
 * Source colours are read from the selected segment using its own logical width:
 *
 *     x + y * vCols
 *
 * Push transitions temporarily shift the logical source coordinates.
 *
 * Reverse, reverse_y and transpose are applied after the source colour has been
 * selected and before the result is written to the global frame.
 *
 *
 * 2D GROUPING
 * -----------
 *
 * A logical matrix pixel can represent a rectangular physical block.
 *
 * For grouping greater than one:
 *
 *     logical x -> x * groupLength()
 *     logical y -> y * groupLength()
 *
 * The colour is then written over the configured grouping area while respecting
 * the physical segment width and height.
 *
 * Spacing is represented by groupLength() but is not filled.
 *
 *
 * 2D MIRRORING
 * ------------
 *
 * setMirroredPixel(x, y, ...) writes the primary physical matrix coordinate and,
 * when enabled, its mirrored equivalents.
 *
 * Possible outputs include:
 *
 *     original
 *     mirror X
 *     mirror Y
 *     mirror X + Y
 *
 * Transpose affects which axis is considered when deriving mirrored positions.
 *
 *
 * 2D PIXEL DECIMATION
 * -------------------
 *
 * Pixel decimation reduces the logical dimensions seen by the effect:
 *
 *     virtualWidth()
 *     virtualHeight()
 *
 * The effect therefore calculates only a smaller logical tile.
 *
 * During the normal fast path, setPixelColorXY() is expected to have already
 * expanded that tile into the complete raw segment buffer, so no decimation
 * mapping is applied there.
 *
 * During the slow path, however, output is generated directly from the reduced
 * logical transition domain. The reduced tile must therefore be repeated into
 * the complete physical matrix region while composing the frame.
 *
 * Decimation handling in this function belongs in the 2D slow-path expansion
 * stage, where logical x/y positions are converted into one or more physical
 * matrix positions.
 *
 * Conceptually:
 *
 *     logical source tile:
 *
 *         virtualWidth() x virtualHeight()
 *
 *     repeated output:
 *
 *         repeat tile across physical width
 *         repeat tile across physical height
 *
 * Boundary checks must use the full physical segment width and height.
 *
 * Decimation must not alter the matrix row stride used by the physical/global
 * frame buffer. Physical writes always use Segment::maxWidth through XY().
 *
 *
 * 1D SLOW PATH
 * ------------
 *
 * The 1D slow path works over:
 *
 *     nLen = topSegment.virtualLength()
 *
 * This is the logical effect length, not necessarily the physical segment
 * length.
 *
 * For every logical index k:
 *
 *     1. Determine whether the pixel is clipped.
 *     2. Select the old or new segment.
 *     3. Apply push-transition source offset if required.
 *     4. Read the logical source colour.
 *     5. Blend old and new colours for fade transitions.
 *     6. Restore the original logical output index.
 *     7. Apply reverse.
 *     8. Expand grouping.
 *     9. Expand decimation.
 *    10. Apply mirroring.
 *    11. Write to the global _pixels[] frame.
 *
 *
 * 1D GROUPING
 * -----------
 *
 * Grouping expands one logical source pixel into multiple adjacent physical
 * pixels:
 *
 *     physicalStart = logicalIndex * groupLength()
 *
 * Pixels from:
 *
 *     physicalStart
 *
 * through:
 *
 *     physicalStart + grouping - 1
 *
 * receive the same colour.
 *
 * Spacing pixels are left untouched by this grouped write.
 *
 *
 * 1D MIRRORING
 * ------------
 *
 * setMirroredPixel(i, ...) converts the segment-relative physical index into an
 * absolute global frame index.
 *
 * It applies:
 *
 *     segment start
 *     segment offset
 *     wrap at segment stop
 *     optional mirror about the segment end
 *
 * The mirrored write is performed in the full physical segment domain.
 *
 *
 * 1D PIXEL DECIMATION
 * -------------------
 *
 * Decimation reduces the logical length used by the effect:
 *
 *     physical segment length = length()
 *     logical effect length   = virtualLength()
 *
 * Example:
 *
 *     length()        = 100
 *     decimate        = 2
 *     virtualLength() = 50
 *
 * The effect computes:
 *
 *     logical pixels 0 ... 49
 *
 * The expected physical result is:
 *
 *     physical 0  ... 49 = logical 0 ... 49
 *     physical 50 ... 99 = logical 0 ... 49
 *
 * In the normal fast path, Segment::setPixelColor() has already replicated that
 * data into the full raw segment buffer.
 *
 * In the transition slow path, blendSegment() works from the reduced logical
 * domain and writes directly to the global frame. Replication must therefore be
 * performed here as part of physical output expansion.
 *
 * The correct order is:
 *
 *     logical index
 *         -> reverse in logical domain
 *         -> grouping expansion
 *         -> decimation repeat offset
 *         -> mirror/offset/global write
 *
 * A repeated physical block starts at:
 *
 *     groupedLogicalIndex
 *       + repeatIndex * physicalPatternLength
 *
 * where:
 *
 *     physicalPatternLength = virtualLength() * groupLength()
 *
 * The write must stop at the full physical segment length:
 *
 *     length()
 *
 * The decimation expansion belongs only in the slow-path physical output stage.
 *
 * Raw source reads in the slow path normally remain in the reduced logical
 * domain:
 *
 *     seg->getPixelColorRaw(i)
 *
 * They should not automatically use modulo against the full physical length.
 *
 * A separate old-segment mapping may be required when the old and new segments
 * have different virtual lengths or decimation factors. Bounds must always be
 * checked using the dimensions belonging to the segment being read.
 *
 *
 * WHY DECIMATION IS HANDLED DIFFERENTLY IN FAST AND SLOW PATHS
 * ------------------------------------------------------------
 *
 * Fast path:
 *
 *     effect
 *       -> setPixelColor()/setPixelColorXY()
 *       -> full raw segment buffer already expanded
 *       -> direct raw physical copy
 *
 * Slow transition path:
 *
 *     effect
 *       -> reduced logical segment pattern
 *       -> transition operates in logical coordinates
 *       -> blendSegment() performs physical expansion
 *       -> repeated decimated pattern written into global frame
 *
 * Applying decimation in both places would duplicate the transform.
 *
 * Omitting it from the slow path would leave only the first logical pattern
 * region updated during a transition, with remaining physical regions staying
 * black or retaining stale data until the transition ends.
 *
 *
 * OLD SEGMENT AND DECIMATION
 * --------------------------
 *
 * The old segment snapshot may have a different decimation value from the new
 * segment.
 *
 * Source dimensions must therefore be selected from the actual source segment:
 *
 *     seg == segO ? old dimensions : new dimensions
 *
 * Fade blending must read the old segment using:
 *
 *     old virtual length
 *     old virtual width
 *     old virtual height
 *     old decimation configuration
 *
 * Physical output expansion is based on the current top segment because the new
 * segment defines the active destination region.
 *
 *
 * CCT HANDLING
 * ------------
 *
 * The segment's transitioned CCT value is read once:
 *
 *     topSegment.currentCCT()
 *
 * When a physical pixel is written, the matching _pixelCCT entry is updated if
 * the CCT buffer exists.
 *
 * Mirrored physical pixels receive the same CCT value as the original.
 *
 *
 * BLEND STYLE RESTORATION
 * -----------------------
 *
 * Single-pixel segments temporarily force fade mode because geometric transition
 * styles have no meaningful spatial extent.
 *
 * The original blend style is saved in orgBS and restored before returning.
 *
 *
 * IMPORTANT INVARIANTS
 * --------------------
 *
 * 1. length(), width() and height() describe the full physical segment domain.
 *
 * 2. virtualLength(), virtualWidth() and virtualHeight() describe the logical
 *    effect domain.
 *
 * 3. getPixelColorRaw() performs no mapping, bounds translation, grouping,
 *    mirroring or decimation.
 *
 * 4. Fast paths read the already-expanded raw segment buffer directly.
 *
 * 5. Slow paths operate in logical coordinates and perform physical expansion.
 *
 * 6. Decimation is expanded in the slow path, not re-applied to fast-path raw
 *    reads.
 *
 * 7. Segment opacity is applied here.
 *
 * 8. Global/bus brightness is applied later by the bus output layer.
 *
 * 9. Physical output bounds use length(), width() and height(), never the
 *    reduced virtual dimensions.
 *
 * 10. Old-segment source bounds use the old segment's own virtual dimensions.
 *
 *
 * SUMMARY
 * -------
 *
 * blendSegment() is the central segment-composition stage:
 *
 *     effect-generated logical segment pixels
 *         -> transition selection/blending
 *         -> grouping/spacing
 *         -> decimation expansion
 *         -> reverse/transpose/mirroring
 *         -> segment blend mode
 *         -> segment opacity
 *         -> global frame buffer
 *
 * The completed global frame is subsequently passed to the bus layer, where
 * gamma correction, global brightness, current limiting, colour order and final
 * hardware output are handled.
 *******************************************************************************************************************************************************************************************************************/
void mAnimatorLight::blendSegment(const Segment &topSegment) const {
  
  // DEBUG_PRINT_LN("blend start");

  typedef uint8_t(*FuncType)(uint8_t, uint8_t);
  // function pointer array: fill with _dummy if using special case: avoid OOB access and always provide a valid path
  // note: making the function array static const uses more ram and comes at no significant speed gain
  FuncType funcs[] = {
    _dummy,      _dummy,     _dummy,    _subtract,
    _difference, _average,   _dummy,    _divide,
    _lighten,    _darken,    _screen,   _overlay,
    _hardlight,  _softlight, _dodge,    _burn,
    _dummy
  };

  const size_t blendMode = topSegment.blendMode < BLENDMODES ? topSegment.blendMode : 0; // default to top if unsupported mode
  const auto segblend = [&](uint32_t t, uint32_t b){
    // use direct calculations/returns for simple/frequent modes (faster)
    switch (blendMode) {
      case 0 : return t;                   // top
      case 1 : return b;                   // bottom
      case 2 : return Segment::color_add(t,b,true); // add with preserve color ratio to avoid color clipping
      case 6 : return RGBW32(_multiply(R(t),R(b)), _multiply(G(t),G(b)), _multiply(B(t),B(b)), _multiply(W(t),W(b))); // multiply (7% faster than lambda at 100bytes flash cost)
      case 16: return t ? t : b;           // stencil (use top layer if not black, else bottom)
    }
    // default: use function pointer from array
    const auto func = funcs[blendMode];
    return RGBW32(func(R(t),R(b)), func(G(t),G(b)), func(B(t),B(b)), func(W(t),W(b)));
  };

  const int     length     = topSegment.length();     // physical segment length (counts all pixels in 2D segment)
  const int     width      = topSegment.width();
  const int     height     = topSegment.height();
  //const uint32_t bgColor   = topSegment.colors[1]; // background color (unused, could add it to stencil mode if requested)
  const auto    XY         = [](int x, int y){ return x + y*Segment::maxWidth; };
  const size_t  matrixSize = Segment::maxWidth * Segment::maxHeight;
  const size_t  startIndx  = XY(topSegment.start, topSegment.startY);
  const size_t  stopIndx   = startIndx + length;
  uint8_t       opacity    = topSegment.currentBri(); // returns transitioned opacity for style FADE
  uint8_t       cct        = topSegment.currentCCT();
  if (gammaCorrectCol) opacity = gamma8inv(opacity); // use inverse gamma on brightness for correct color scaling after gamma correction (see #5343 for details)

  const Segment *segO = topSegment.getOldSegment();
  const bool hasGrouping = topSegment.groupLength() != 1;

  opacity = 255;

//   if (topSegment.progress() > 63000U)
// {
//   Serial.printf(
//     "BLEND IN prog=%u bri=%u opacity=%u old=%08lX new=%08lX\n",
//     topSegment.progress(),
//     topSegment.currentBri(),
//     opacity,
//     (unsigned long)(segO ? segO->getPixelColorRaw(0) : 0),
//     (unsigned long)topSegment.getPixelColorRaw(0)
//   );
// }


  // ALOG_INF(PSTR("seg0 %d, blendStyle %d, hasGrouping %d, mirror %d, mirror_y %d"), segO, blendingStyle, hasGrouping, topSegment.mirror, topSegment.mirror_y);
  // ALOG_INF(
  //   PSTR(
  //     "segO=%p len%d, transition=%u blendStyle=%u fade=%u "
  //     "grouping=%u mirror=%u mirrorY=%u isMatrix=%u"
  //   ),
  //   (void*)segO,
  //   length,
  //   (unsigned)topSegment.isInTransition(),
  //   (unsigned)blendingStyle,
  //   (unsigned)TRANSITION_FADE,
  //   (unsigned)hasGrouping,
  //   (unsigned)topSegment.mirror,
  //   (unsigned)topSegment.mirror_y,
  //   (unsigned)isMatrix
  // );

  // fast path: handle the default case - no transitions, no grouping/spacing, no mirroring, no CCT
  if (!segO && blendingStyle == TRANSITION_FADE && !hasGrouping && !topSegment.mirror && !topSegment.mirror_y) {
    // DEBUG_PRINT_LN("Fast Path");
    if (isMatrix && stopIndx <= matrixSize && !_pixelCCT) {
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS // WLED_DISABLE_2D
      // Calculate pointer steps to avoid 'if' and 'XY()' inside loops
      int x_inc = 1;
      int y_inc = Segment::maxWidth;
      int start_offset = XY(topSegment.start, topSegment.startY);
      
      // adjust starting position and steps based on Reverse/Transpose
      // note: transpose is handled in separate loop so it is still fast and no branching is needed in default path
      if (!topSegment.transpose) {
        if (topSegment.reverse)   { start_offset += (width - 1); x_inc = -1; }
        if (topSegment.reverse_y) { start_offset += (height - 1) * Segment::maxWidth; y_inc = -Segment::maxWidth; }
        
        for (int y = 0; y < height; y++) {
          uint32_t* pRow = &_pixels[start_offset + y * y_inc];
          const int y_width = y * width;
          for (int x = 0; x < width; x++) {
            uint32_t* p = pRow + x * x_inc;
            uint32_t c_a = topSegment.getPixelColorRaw(x + y_width);
            *p = Segment::color_blend(*p, segblend(c_a, *p), opacity);
          }
        }
      } else { // transposed
        
        for (int y = 0; y < height; y++) {
          const int px = topSegment.reverse ? (height - y - 1) : y;  // source pixel: swap y into x, reverse if needed
          for (int x = 0; x < width; x++) {
            const int py = topSegment.reverse_y ? (width  - x - 1) : x;  // source pixel: swap x into y, reverse if needed
            const uint32_t c_a = topSegment.getPixelColorRaw(px + py * height); // height = virtual width
            const size_t idx = XY(topSegment.start + x, topSegment.startY + y); // write logical (non swapped) pixel coordinate
            _pixels[idx] = Segment::color_blend(_pixels[idx], segblend(c_a, _pixels[idx]), opacity);
          }
        }
      }
      return;
#endif
    } else 
    if (!isMatrix) {
      // DEBUG_PRINT_LN("1D Fast Path");
      // 1D fast path, include CCT as it is more common on 1D setups
      // pixels[] -> _pixels[]
      uint32_t* strip = _pixels;
      int start = topSegment.start;
      int off   = topSegment.offset;
      // ALOG_INF(PSTR("stop %d"), topSegment.stop);
      for (int i = 0; i < length; i++) {
        uint32_t c_a = topSegment.getPixelColorRaw(i);
        int p = topSegment.reverse ? (length - i - 1) : i;
        int idx = start + p + off;
        if (idx >= topSegment.stop) idx -= length;
        strip[idx] = Segment::color_blend(strip[idx], segblend(c_a, strip[idx]), opacity);

        // if(idx==0) ALOG_INF(PSTR("strip %d %d,%d,%d"), idx, R(strip[idx]), G(strip[idx]), B(strip[idx]));
        // strip[idx] = c_a;
        // ALOG_INF(PSTR("idx i %d %d"), idx, i);
        // if (_pixelCCT) _pixelCCT[idx] = cct;

      }
      return;
    }
  }else{
    // DEBUG_PRINT_LN("!Fast Path");
  }


  // slow path: handle transitions, grouping/spacing, segments with clipping and CCT pixels
  Segment::setClippingRect(0, 0);  // disable clipping by default
  const unsigned progress = topSegment.progress();
  const unsigned progInv  = 0xFFFFU - progress;
  const unsigned dw = (blendingStyle==TRANSITION_OUTSIDE_IN ? progInv : progress) * width / 0xFFFFU + 1;
  const unsigned dh = (blendingStyle==TRANSITION_OUTSIDE_IN ? progInv : progress) * height / 0xFFFFU + 1;
  const unsigned orgBS = blendingStyle;
  if (width*height == 1) tkr_anim->blendingStyle = TRANSITION_FADE; // disable style for single pixel segments (use fade instead)
  switch (tkr_anim->blendingStyle) {
    case TRANSITION_CIRCULAR_IN: // (must set entire segment, see isPixelXYClipped())
    case TRANSITION_CIRCULAR_OUT:// (must set entire segment, see isPixelXYClipped())
    case TRANSITION_FAIRY_DUST:  // fairy dust (must set entire segment, see isPixelXYClipped())
      Segment::setClippingRect(0, width, 0, height);
      break;
    case TRANSITION_SWIPE_RIGHT: // left-to-right
    case TRANSITION_PUSH_RIGHT:  // left-to-right
      Segment::setClippingRect(0, dw, 0, height);
      break;
    case TRANSITION_SWIPE_LEFT:  // right-to-left
    case TRANSITION_PUSH_LEFT:   // right-to-left
      Segment::setClippingRect(width - dw, width, 0, height);
      break;
    case TRANSITION_OUTSIDE_IN:   // corners
      Segment::setClippingRect((width + dw)/2, (width - dw)/2, (height + dh)/2, (height - dh)/2); // inverted!!
      break;
    case TRANSITION_INSIDE_OUT:  // outward
      Segment::setClippingRect((width - dw)/2, (width + dw)/2, (height - dh)/2, (height + dh)/2);
      break;
    case TRANSITION_SWIPE_DOWN:  // top-to-bottom (2D)
    case TRANSITION_PUSH_DOWN:   // top-to-bottom (2D)
      Segment::setClippingRect(0, width, 0, dh);
      break;
    case TRANSITION_SWIPE_UP:    // bottom-to-top (2D)
    case TRANSITION_PUSH_UP:     // bottom-to-top (2D)
      Segment::setClippingRect(0, width, height - dh, height);
      break;
    case TRANSITION_OPEN_H:      // horizontal-outward (2D) same look as INSIDE_OUT on 1D
      Segment::setClippingRect((width - dw)/2, (width + dw)/2, 0, height);
      break;
    case TRANSITION_OPEN_V:      // vertical-outward (2D)
      Segment::setClippingRect(0, width, (height - dh)/2, (height + dh)/2);
      break;
    case TRANSITION_SWIPE_TL:    // TL-to-BR (2D)
    case TRANSITION_PUSH_TL:     // TL-to-BR (2D)
      Segment::setClippingRect(0, dw, 0, dh);
      break;
    case TRANSITION_SWIPE_TR:    // TR-to-BL (2D)
    case TRANSITION_PUSH_TR:     // TR-to-BL (2D)
      Segment::setClippingRect(width - dw, width, 0, dh);
      break;
    case TRANSITION_SWIPE_BR:    // BR-to-TL (2D)
    case TRANSITION_PUSH_BR:     // BR-to-TL (2D)
      Segment::setClippingRect(width - dw, width, height - dh, height);
      break;
    case TRANSITION_SWIPE_BL:    // BL-to-TR (2D)
    case TRANSITION_PUSH_BL:     // BL-to-TR (2D)
      Segment::setClippingRect(0, dw, height - dh, height);
      break;
  }

  if (isMatrix && stopIndx <= matrixSize) {
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    const int nCols = topSegment.virtualWidth();
    const int nRows = topSegment.virtualHeight();
    const int oCols = segO ? segO->virtualWidth() : nCols;
    const int oRows = segO ? segO->virtualHeight() : nRows;

    const auto setMirroredPixel = [&](int x, int y, uint32_t c, uint8_t o) {
      const int baseX = topSegment.start  + x;
      const int baseY = topSegment.startY + y;
      size_t indx = XY(baseX, baseY); // absolute address on strip
      _pixels[indx] = Segment::color_blend(_pixels[indx], segblend(c, _pixels[indx]), o);
      if (_pixelCCT) _pixelCCT[indx] = cct;
      // Apply mirroring if enabled
      if (topSegment.mirror || topSegment.mirror_y) {
        const int mirrorX = topSegment.start  + width  - x - 1;
        const int mirrorY = topSegment.startY + height - y - 1;
        const size_t idxMX = XY(topSegment.transpose ? baseX : mirrorX, topSegment.transpose ? mirrorY : baseY);
        const size_t idxMY = XY(topSegment.transpose ? mirrorX : baseX, topSegment.transpose ? baseY : mirrorY);
        const size_t idxMM = XY(mirrorX, mirrorY);
        if (topSegment.mirror)                        _pixels[idxMX] = tkr_anim->color_blend(_pixels[idxMX], segblend(c, _pixels[idxMX]), o);
        if (topSegment.mirror_y)                      _pixels[idxMY] = tkr_anim->color_blend(_pixels[idxMY], segblend(c, _pixels[idxMY]), o);
        if (topSegment.mirror && topSegment.mirror_y) _pixels[idxMM] = tkr_anim->color_blend(_pixels[idxMM], segblend(c, _pixels[idxMM]), o);
        if (_pixelCCT) {
          if (topSegment.mirror)                        _pixelCCT[idxMX] = cct;
          if (topSegment.mirror_y)                      _pixelCCT[idxMY] = cct;
          if (topSegment.mirror && topSegment.mirror_y) _pixelCCT[idxMM] = cct;
        }
      }
    };

    // if we blend using "push" style we need to "shift" canvas to left/right/up/down
    unsigned offsetX = (blendingStyle == TRANSITION_PUSH_UP   || blendingStyle == TRANSITION_PUSH_DOWN)  ? 0 : progInv * nCols / 0xFFFFU;
    unsigned offsetY = (blendingStyle == TRANSITION_PUSH_LEFT || blendingStyle == TRANSITION_PUSH_RIGHT) ? 0 : progInv * nRows / 0xFFFFU;
    const unsigned groupLen = topSegment.groupLength();
    bool applyReverse = topSegment.reverse || topSegment.reverse_y || topSegment.transpose;
    int pushOffsetX = 0, pushOffsetY = 0;
    // if we blend using "push" style we need to "shift" canvas to left/right/up/down
    switch (blendingStyle) {
      case TRANSITION_PUSH_RIGHT: pushOffsetX = offsetX; break;
      case TRANSITION_PUSH_LEFT:  pushOffsetX = -offsetX + nCols; break;
      case TRANSITION_PUSH_DOWN:  pushOffsetY = offsetY; break;
      case TRANSITION_PUSH_UP:    pushOffsetY = -offsetY + nRows; break;
      case TRANSITION_PUSH_TL:    pushOffsetX = offsetX;            pushOffsetY = offsetY; break;           // unused
      case TRANSITION_PUSH_TR:    pushOffsetX = -offsetX + nCols;   pushOffsetY = offsetY; break;           // unused
      case TRANSITION_PUSH_BR:    pushOffsetX = -offsetX + nCols;   pushOffsetY = -offsetY + nRows; break;  // unused
      case TRANSITION_PUSH_BL:    pushOffsetX = offsetX;            pushOffsetY = -offsetY + nRows; break;  // unused
    }
    // we only traverse new segment, not old one
    for (int r = 0; r < nRows; r++) for (int c = 0; c < nCols; c++) {
      const bool clipped = topSegment.isPixelXYClipped(c, r);
      // if segment is in transition and pixel is clipped take old segment's pixel and opacity
      const Segment *seg = clipped && segO ? segO : &topSegment;  // pixel is never clipped for FADE
      int vCols = seg == segO ? oCols : nCols;         // old segment may have different dimensions
      int vRows = seg == segO ? oRows : nRows;         // old segment may have different dimensions
      int x = c;
      int y = r;
      if (pushOffsetX != 0) x = (x + pushOffsetX) % nCols;
      if (pushOffsetY != 0) y = (y + pushOffsetY) % nRows;
      uint32_t c_a = BLACK;
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
      if (seg->decimate > 1 && vCols > 0 && vRows > 0) {
        const int sourceX = x % vCols;
        const int sourceY = y % vRows;
        if (sourceX >= 0 && sourceY >= 0) c_a = seg->getPixelColorRaw(sourceX + sourceY*vCols);
      } else
#endif
      if (x < vCols && y < vRows) c_a = seg->getPixelColorRaw(x + y*vCols); // will get clipped pixel from old segment or unclipped pixel from new segment
      // if (segO && blendingStyle == TRANSITION_FADE
      //   && (topSegment.effect_id != segO->effect_id || (segO->name != topSegment.name && segO->name && topSegment.name && strncmp(segO->name, topSegment.name, WLED_MAX_SEGNAME_LEN) != 0))
      //   && x < oCols && y < oRows) {
      //   // we need to blend old segment using fade as pixels are not clipped
      //   c_a = color_blend16(c_a, segO->getPixelColorRaw(x + y*oCols), progInv);
      // } else if (blendingStyle != TRANSITION_FADE) {


      if (segO && blendingStyle == TRANSITION_FADE
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
          && ((segO->decimate > 1 && oCols > 0 && oRows > 0) || (x < oCols && y < oRows))
#else
          && x < oCols && y < oRows
#endif
      ) {
        // we need to blend old segment using fade as pixels are not clipped
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
        const int oldX = segO->decimate > 1 ? x % oCols : x;
        const int oldY = segO->decimate > 1 ? y % oRows : y;
        c_a = color_blend16(c_a, segO->getPixelColorRaw(oldX + oldY*oCols), progInv);
#else
        c_a = color_blend16(c_a, segO->getPixelColorRaw(x + y*oCols), progInv);
#endif
      } else if (blendingStyle != TRANSITION_FADE) {

        // if we have global brightness change (not On/Off change) we will ignore transition style and just fade brightness (see led.cpp)
        // workaround for On/Off transition
        // (bri != briT) && !bri => from On to Off
        // (bri != briT) &&  bri => from Off to On
        uint8_t bri = tkr_iLight->_briRGB_Global;
        if ((briOld == 0 || bri == 0) && ((!clipped && (bri != briT) && !bri) || (clipped && (bri != briT) && bri))) c_a = BLACK;
      }
      // map it into frame buffer
      x = c;  // restore coordiates if we were PUSHing
      y = r;
      if (applyReverse) {
        if (topSegment.reverse  ) x = nCols - x - 1;
        if (topSegment.reverse_y) y = nRows - y - 1;
        if (topSegment.transpose) std::swap(x,y); // swap X & Y if segment transposed
      }
      // expand pixel
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
      if (topSegment.decimate > 1) {
        const int repeatCols = topSegment.transpose ? nRows : nCols;
        const int repeatRows = topSegment.transpose ? nCols : nRows;
        for (int repeatY = 0; repeatY < topSegment.decimate; repeatY++) {
          for (int repeatX = 0; repeatX < topSegment.decimate; repeatX++) {
            int outX = x + repeatX * repeatCols;
            int outY = y + repeatY * repeatRows;
            if (groupLen == 1) {
              if (outX < width && outY < height) setMirroredPixel(outX, outY, c_a, opacity);
            } else {
              // handle grouping and spacing
              outX *= groupLen; // expand to physical pixels
              outY *= groupLen; // expand to physical pixels
              const int maxX = std::min(outX + topSegment.grouping, width);
              const int maxY = std::min(outY + topSegment.grouping, height);
              while (outY < maxY) {
                int _x = outX;
                while (_x < maxX) setMirroredPixel(_x++, outY, c_a, opacity);
                outY++;
              }
            }
          }
        }
      } else
#endif
      if (groupLen == 1) {
        setMirroredPixel(x, y, c_a, opacity);
      } else {
        // handle grouping and spacing
        x *= groupLen; // expand to physical pixels
        y *= groupLen; // expand to physical pixels
        const int maxX = std::min(x + topSegment.grouping, width);
        const int maxY = std::min(y + topSegment.grouping, height);
        while (y < maxY) {
          int _x = x;
          while (_x < maxX) setMirroredPixel(_x++, y, c_a, opacity);
          y++;
        }
      }
    }
#endif
  } else {

    // ALOG_INF(PSTR("SLOW PATH----------------------------"));

    // 1D Slow Path
    const int nLen = topSegment.virtualLength();
    const int oLen = segO ? segO->virtualLength() : nLen;

    const auto setMirroredPixel = [&](int i, uint32_t c, uint8_t o) {
      int indx = topSegment.start + i;
      // Apply mirroring
      if (topSegment.mirror) {
        unsigned indxM = topSegment.stop - i - 1;
        indxM += topSegment.offset; // offset/phase
        if (indxM >= topSegment.stop) indxM -= length; // wrap
        _pixels[indxM] = Segment::color_blend(_pixels[indxM], segblend(c, _pixels[indxM]), o);
        if (_pixelCCT) _pixelCCT[indxM] = cct;
      }
      indx += topSegment.offset; // offset/phase
      if (indx >= topSegment.stop) indx -= length; // wrap
      _pixels[indx] = Segment::color_blend(_pixels[indx], segblend(c, _pixels[indx]), o);
      if (_pixelCCT) _pixelCCT[indx] = cct;

//       if (i == 0 && topSegment.progress() > 63000U)
// {
//   Serial.printf(
//     "SLOW OUT prog=%u source=%08lX out=%08lX RGBW=%u,%u,%u,%u\n",
//     topSegment.progress(),
//     (unsigned long)c,
//     (unsigned long)_pixels[indx],
//     R(_pixels[indx]),
//     G(_pixels[indx]),
//     B(_pixels[indx]),
//     W(_pixels[indx])
//   );
// }

    };

    // if we blend using "push" style we need to "shift" canvas to left/right/
    unsigned offsetI = progInv * nLen / 0xFFFFU;

    for (int k = 0; k < nLen; k++) {
      const bool clipped = topSegment.isPixelClipped(k);
      // if segment is in transition and pixel is clipped take old segment's pixel and opacity
      const Segment *seg = clipped && segO ? segO : &topSegment;  // pixel is never clipped for FADE
      const int vLen = seg == segO ? oLen : nLen;
      int i = k;
      // if we blend using "push" style we need to "shift" canvas to left or right
      switch (tkr_anim->blendingStyle) {
        case TRANSITION_PUSH_RIGHT: i = (i + offsetI) % nLen;        break;
        case TRANSITION_PUSH_LEFT:  i = (i - offsetI + nLen) % nLen; break;
      }
      uint32_t c_a = BLACK;
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
      if (seg->decimate > 1 && vLen > 0) {
        c_a = seg->getPixelColorRaw(i % vLen);
      } else
#endif
      if (i < vLen) c_a = seg->getPixelColorRaw(i); // will get clipped pixel from old segment or unclipped pixel from new segment
      // if (segO && tkr_anim->blendingStyle == TRANSITION_FADE && topSegment.effect_id != segO->effect_id && i < oLen) {
        if (segO && tkr_anim->blendingStyle == TRANSITION_FADE
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
            && ((segO->decimate > 1 && oLen > 0) || i < oLen)
#else
            && i < oLen
#endif
        )
        {
          // static uint32_t last_transition_pixel_log = 0;
          // if (millis() - last_transition_pixel_log >= 500)
          // {
          //   last_transition_pixel_log = millis();
          //   ALOG_INF(PSTR("TRANS PIXEL BLEND progress=%u inverse=%u new=%08X old=%08X out=%08X"), topSegment.progress(), progInv, c_a, segO->getPixelColorRaw(i), color_blend16(c_a, segO->getPixelColorRaw(i), progInv));
          // }
        // we need to blend old segment using fade as pixels are not clipped
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
        c_a = color_blend16(c_a, segO->getPixelColorRaw(segO->decimate > 1 ? i % oLen : i), progInv);
#else
        c_a = color_blend16(c_a, segO->getPixelColorRaw(i), progInv);
#endif
      } else if (tkr_anim->blendingStyle != TRANSITION_FADE) {
        // if we have global brightness change (not On/Off change) we will ignore transition style and just fade brightness (see led.cpp)
        // workaround for On/Off transition
        // (bri != briT) && !bri => from On to Off
        // (bri != briT) &&  bri => from Off to On
        uint8_t bri = tkr_iLight->_briRGB_Global;
        if ((briOld == 0 || bri == 0) && ((!clipped && (bri != briT) && !bri) || (clipped && (bri != briT) && bri))) c_a = BLACK;
      }
      // map into frame buffer
      i = k; // restore index if we were PUSHing
      if (topSegment.reverse) i = nLen - i - 1; // is segment reversed?
      // expand pixel
      i *= topSegment.groupLength();
#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
      if (topSegment.decimate > 1) {
        const int repeatStride = nLen * topSegment.groupLength();
        for (int repeat = 0; repeat < topSegment.decimate; repeat++) {
          int outI = i + repeat * repeatStride;
          // set all the pixels in the group
          const int maxI = std::min(outI + topSegment.grouping, length); // make sure to not go beyond physical length
          while (outI < maxI) setMirroredPixel(outI++, c_a, opacity);
        }
      } else
#endif
      {
        // set all the pixels in the group
        const int maxI = std::min(i + topSegment.grouping, length); // make sure to not go beyond physical length
        while (i < maxI) setMirroredPixel(i++, c_a, opacity);
      }
    }
  }

  tkr_anim->blendingStyle = orgBS;
  Segment::setClippingRect(0, 0);             // disable clipping for overlays
}



void mAnimatorLight::show(void)
{
  if (!_pixels) {
    ALOG_ERR(PSTR("Error: no global pixel buffer"));
    return;
  }

  // Dual-core safe re-entrancy guard
  static portMUX_TYPE g_show_mux = portMUX_INITIALIZER_UNLOCKED;
  static bool g_show_inflight = false;

  portENTER_CRITICAL(&g_show_mux);
  if (g_show_inflight) {
    portEXIT_CRITICAL(&g_show_mux);
    return;
  }
  g_show_inflight = true;
  portEXIT_CRITICAL(&g_show_mux);

  // Some buses send asynchronously. Do not modify their buffers until ready.
  if (!BusManager::canAllShow()) {
    portENTER_CRITICAL(&g_show_mux);
    g_show_inflight = false;
    portEXIT_CRITICAL(&g_show_mux);
    return;
  }

  const unsigned long showNow = millis();
  const size_t diff = showNow - _lastShow;
  const size_t totalLen = getLengthTotal();

    // DEBUG_PRINT_LN("HERE2");
  /**
   * @brief Optional per-pixel CCT state.
   *
   * WLED stores RGBW in the global U32 buffer and separately retains the
   * segment-derived CCT value for each physical pixel during composition.
   */
  if ((hasCCTBus() || correctWB) && !cctFromRgb) {
    DEBUG_PRINT_LN("HERE1");
    _pixelCCT = static_cast<uint8_t*>(
      allocate_buffer(
        totalLen * sizeof(uint8_t),
        BFRALLOC_PREFER_PSRAM
      )
    );
  }


  if (_pixelCCT) memset(_pixelCCT, 127, totalLen);

    // DEBUG_PRINT_LN("HERE6a");


  for (uint8_t seg_i = 0; seg_i < segments.size(); seg_i++)
  {
    Segment &seg = segments[seg_i];

    const uintptr_t src_start = reinterpret_cast<uintptr_t>(seg.getPixels());
    const uintptr_t src_end   = src_start + (static_cast<uintptr_t>(seg.length()) * sizeof(uint32_t));

    const uintptr_t dst_start = reinterpret_cast<uintptr_t>(_pixels);
    const uintptr_t dst_end   = dst_start + (static_cast<uintptr_t>(totalLen) * sizeof(uint32_t));

    const bool overlap =
      (src_start < dst_end) &&
      (dst_start < src_end);

    const intptr_t delta_bytes =
      static_cast<intptr_t>(dst_start) -
      static_cast<intptr_t>(src_start);

    const intptr_t delta_pixels =
      delta_bytes / static_cast<intptr_t>(sizeof(uint32_t));

    // ALOG_INF(PSTR("BUFFER CHECK: seg=%u src=%p..%p dst=%p..%p deltaBytes=%d deltaPixels=%d overlap=%u"), seg_i, (void*)src_start, (void*)src_end, (void*)dst_start, (void*)dst_end, (int)delta_bytes, (int)delta_pixels, overlap);
  }

  /**
   * @brief Construct the global logical framebuffer.
   *
   * Effects have already rendered into Segment::pixels. This clears the
   * device framebuffer and composites every active segment into it.
   */
  if (realtimeMode == REALTIME_MODE_INACTIVE ||
      useMainSegmentOnly ||
      realtimeOverride > REALTIME_OVERRIDE_NONE)
  {
    memset(_pixels, 0, sizeof(uint32_t) * totalLen);

    for (Segment &seg : segments) {
      if (seg.isActive() && (seg.on || seg.isInTransition())) {

        // seg.pixels[1] = RGBW32(0,0,255,0);

        // ALOG_INF(PSTR("BEFORE BLEND: seg=%u len=%u vlen=%u period=%d"), 0, seg.length(), seg.virtualLength(), DebugFindPixelPeriod(seg.getPixels(), seg.length()));
        blendSegment(seg);


        // for (int i = 0; i < seg.length(); i++) {
        //   uint32_t c_a = seg.getPixelColorRaw(i);
        //   _pixels[i] = c_a;
        // }

        // ALOG_INF(PSTR("AFTER BLEND1: seg=%u len=%u vlen=%u period=%d"), 0, seg.length(), seg.virtualLength(), DebugFindPixelPeriod(seg.getPixels(), seg.length()));
        // ALOG_INF(PSTR("AFTER BLEND2: seg=%u totalLen=%u period=%d"), 0, totalLen, DebugFindPixelPeriod(_pixels, totalLen));

      }
    }
  }

    // DEBUG_PRINT_LN("HERE6b");

// Temporary validation override
// if (totalLen >= 5) {
  // _pixels[0] = RGBW32(0,255,   0, 0);
  // _pixels[1] = RGBW32(  0, 255,   0, 0);
  // _pixels[2] = RGBW32(  0,   0, 255, 0);
  // _pixels[15] = RGBW32(255, 255,   0, 0);
  // // _pixels[4] = RGBW32(255,   0, 128, 0);
  // // _pixels[9] = RGBW32(255,   255, 255, 0);
  // _pixels[29] = RGBW32(255,   0, 255, 0);
// }

    // DEBUG_PRINT_LN("HERE6c");

    // already wrong here, so when is _pixels modified?
        // for (int i = 0; i < totalLen; i++) {
        //   uint32_t col = _pixels[i];
        //   uint32_t cl = segments[0].getPixelColorRaw(i);
        //   ALOG_INF(PSTR("_ %d col %d,%d,%d==%d,%d,%d"),i, R(col), G(col), B(col), R(cl), G(cl), B(cl));
        // }

  /**
   * @brief Allow an external callback to modify the completed logical frame.
   *
   * This retains WLED ordering: callback after segment composition and before
   * copying the frame into the bus buffers.
   */
  show_callback callback = _callback;
  if (callback) callback();

  /**
   * @brief Copy the logical device framebuffer into the physical bus buffers.
   */
  int oldCCT = Bus::getCCT();

//   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TRANSITION_ENDPOINT_PIXEL0

// static bool transition_was_active = false;

// static uint32_t colour_previous_2 = 0;
// static uint32_t colour_previous_1 = 0;

// static uint16_t progress_previous_2 = 0xFFFF;
// static uint16_t progress_previous_1 = 0xFFFF;

// static uint8_t brightness_previous_2 = 0;
// static uint8_t brightness_previous_1 = 0;

// if (!segments.empty() && _pixels)
// {
//   Segment& debug_segment = segments[0];

//   const bool transition_active = debug_segment.isInTransition();
//   const uint16_t transition_progress = debug_segment.progress();
//   const uint8_t transition_brightness = debug_segment.currentBri();

//   /*
//    * Segment virtual pixel zero normally maps from segment.start.
//    * This is the final composed pixel immediately before physical output.
//    */
//   const uint16_t physical_pixel = debug_segment.start;
//   const uint32_t colour_current = _pixels[physical_pixel];

//   /*
//    * Detect the first output frame after the transition object disappeared.
//    *
//    * previous_2 = second-last transition frame
//    * previous_1 = final rendered transition frame
//    * current    = first non-transition frame
//    */
//   if (transition_was_active && !transition_active)
//   {
//     Serial.printf(
//       "\nTRANS END PIXEL0\n"
//       "  PREV2 prog=%5u bri=%3u col=%08lX RGBW=%3u,%3u,%3u,%3u\n"
//       "  PREV1 prog=%5u bri=%3u col=%08lX RGBW=%3u,%3u,%3u,%3u\n"
//       "  NEW   prog=%5u bri=%3u col=%08lX RGBW=%3u,%3u,%3u,%3u\n",
//       progress_previous_2,
//       brightness_previous_2,
//       (unsigned long)colour_previous_2,
//       R(colour_previous_2),
//       G(colour_previous_2),
//       B(colour_previous_2),
//       W(colour_previous_2),

//       progress_previous_1,
//       brightness_previous_1,
//       (unsigned long)colour_previous_1,
//       R(colour_previous_1),
//       G(colour_previous_1),
//       B(colour_previous_1),
//       W(colour_previous_1),

//       transition_progress,
//       transition_brightness,
//       (unsigned long)colour_current,
//       R(colour_current),
//       G(colour_current),
//       B(colour_current),
//       W(colour_current)
//     );
//   }

//   colour_previous_2 = colour_previous_1;
//   colour_previous_1 = colour_current;

//   progress_previous_2 = progress_previous_1;
//   progress_previous_1 = transition_progress;

//   brightness_previous_2 = brightness_previous_1;
//   brightness_previous_1 = transition_brightness;

//   transition_was_active = transition_active;
// }

// #endif


  // CCT is calculated from RGB when this mode is enabled.
  if (cctFromRgb) BusManager::setSegmentCCT(-1);

  #ifdef ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES
  const bool useGammaCorrection =
  frame_use_gamma_correction &&
  gammaCorrectCol &&
  !(realtimeMode && arlsDisableGammaCorrection && !realtimeOverride);
  #else
  const bool useGammaCorrection =
    gammaCorrectCol &&
    !(realtimeMode && arlsDisableGammaCorrection && !realtimeOverride);
  #endif

  for (size_t i = 0; i < totalLen; i++) {
    if (_pixelCCT) {
      if (i == 0 || _pixelCCT[i - 1] != _pixelCCT[i]) {
        BusManager::setSegmentCCT(_pixelCCT[i], correctWB);
      }
    }

    // Copy the colour. Do not modify the authoritative logical framebuffer.
    uint32_t col = _pixels[i];

    if (col > 0 && useGammaCorrection) col = gamma32(col);

    BusManager::setPixelColor(getMappedPixelIndex(i), col);
  }

  Bus::setCCT(oldCCT);

  p_free(_pixelCCT);
  _pixelCCT = nullptr;

  // ALOG_INF(PSTR("show %d,%d,%d"),R(_pixels[0]),G(_pixels[0]),B(_pixels[0]));

  /**
   * @brief Start physical bus transmission.
   *
   * Some NeoPixelBus implementations return before asynchronous transmission
   * has completed. The next effects pass is already protected by canAllShow().
   */
  BusManager::show();
  

  portENTER_CRITICAL(&g_show_mux);
  g_show_inflight = false;
  portEXIT_CRITICAL(&g_show_mux);

  if (diff > 0) {
    const size_t fpsCurr = (1000 << FPS_CALC_SHIFT) / diff;
    _cumulativeFps =
      (FPS_CALC_AVG * _cumulativeFps + fpsCurr + FPS_CALC_AVG / 2) /
      (FPS_CALC_AVG + 1);

    _lastShow = showNow;
  }

#ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
  const uint32_t elapsed = millis() - tSaved_LoopTime;
  if (elapsed > 10) Serial.printf("LoopElapsed %u(fps%u)\n\r", elapsed, 1000 / elapsed);
  tSaved_LoopTime = millis();
#endif


  //   DEBUG_PRINT_LN("HERE6d");
  // return;


}



// #define ENABLE_DEVFEATURE_LIGHTS__UPDATED_SHOW_RMT_FIX

// #ifdef ENABLE_DEVFEATURE_LIGHTS__UPDATED_SHOW_RMT_FIX

// void mAnimatorLight::show(void)
// {
//   // Dual-core safe re-entrancy guard
//   static portMUX_TYPE g_show_mux = portMUX_INITIALIZER_UNLOCKED;
//   static bool g_show_inflight = false;

//   // Acquire guard
//   portENTER_CRITICAL(&g_show_mux);
//   if (g_show_inflight) {
//     portEXIT_CRITICAL(&g_show_mux);
//     return;
//   }
//   g_show_inflight = true;
//   portEXIT_CRITICAL(&g_show_mux);

//   // Optional: gate here too (keeps show() self-contained safe)
//   // If you don't want this here because caller already checked, you can remove it.
//   if (!BusManager::canAllShow()) {
//     portENTER_CRITICAL(&g_show_mux);
//     g_show_inflight = false;
//     portEXIT_CRITICAL(&g_show_mux);
//     return;
//   }

//   // Avoid race condition: capture _callback value (but execute while guarded)
//   show_callback callback = _callback;
//   if (callback) {
//     callback();
//   }

//   const unsigned long showNow = millis();

//   // Do the actual output
//   BusManager::show();

//   // Release guard ASAP after output
//   portENTER_CRITICAL(&g_show_mux);
//   g_show_inflight = false;
//   portEXIT_CRITICAL(&g_show_mux);

//   // FPS / timing (preserve your behaviour)
//   const unsigned long diff = showNow - _lastShow;
//   if (diff > 0) {
//     const size_t fpsCurr = (1000 << FPS_CALC_SHIFT) / diff; // fixed point math
//     _cumulativeFps = (FPS_CALC_AVG * _cumulativeFps + fpsCurr + FPS_CALC_AVG / 2) / (FPS_CALC_AVG + 1);
//     _lastShow = showNow;
//   }

//   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
//   const uint32_t elapsed = millis() - tSaved_LoopTime;
//   if (elapsed > 10) Serial.printf("LoopElapsed %d(fps%d)\n\r", elapsed, 1000 / elapsed);
//   tSaved_LoopTime = millis();
//   #endif
// }

// #else

// static volatile bool g_show_inflight = false;

// void mAnimatorLight::show(void) 
// {  

//   // avoid race condition, caputre _callback value
//   show_callback callback = _callback;
//   if (callback) callback();

//   unsigned long showNow = millis();

//   if (g_show_inflight) return;
//   g_show_inflight = true;

//   BusManager::show();
  
//   g_show_inflight = false;

//   unsigned long diff = showNow - _lastShow;
//   if (diff > 0) { // skip calculation if no time has passed
//     size_t fpsCurr = (1000 << FPS_CALC_SHIFT) / diff; // fixed point math
//     _cumulativeFps = (FPS_CALC_AVG * _cumulativeFps + fpsCurr + FPS_CALC_AVG / 2) / (FPS_CALC_AVG + 1);   // "+FPS_CALC_AVG/2" for proper rounding
//     _lastShow = showNow;
//   }
//   // uint16_t fpsCurr = 200;
//   // if (diff > 0) fpsCurr = 1000 / diff;
//   // _cumulativeFps = (3 * _cumulativeFps + fpsCurr) >> 2;
//   // _lastShow = showNow;
//   // Serial.printf("%d lastshow\n\r", _lastShow);

//   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
//   // DEBUG_TIME__START
//   uint32_t elapsed = millis() - tSaved_LoopTime;
//   if(elapsed > 10) Serial.printf("LoopElapsed %d(fps%d)\n\r", elapsed, 1000/elapsed);
//   tSaved_LoopTime = millis();
//   #endif
// }
// #endif



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
    if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
    
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




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks
 * @note : Converted from WLED Effects
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint32_t mAnimatorLight::crgb_to_col(CRGB crgb)
{
  uint32_t colour = 0; // suppress White colour (MSB)
  colour = (((uint32_t)crgb.red << 16) | ((uint32_t)crgb.green << 8) | crgb.blue);
  return colour;
}


CRGB mAnimatorLight::col_to_crgb(uint32_t color)
{
  CRGB crgb;
  crgb.red =   (color >> 16 & 0xFF);
  crgb.green = (color >> 8  & 0xFF);
  crgb.blue =  (color       & 0xFF);
  return crgb;
}


mAnimatorLight& mAnimatorLight::SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE)
{
  // this->SEGMENT_I(segment_index).anim_function_callback = anim_function_callback;
  // SEGMENT_I(segment_index).animation_has_anim_callback = true;
  return *this;
}


void mAnimatorLight::StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index)
{
  uint16_t time_tmp = SEGMENT_I(segment_index).animator_blend_time_ms();

  // --- One-shot animation-time override ---
  if (SEGMENT_I(segment_index).single_animation_override.time_ms > 0)
  {
    // Save current cycle time only once so we can restore it after animation completes.
    if (SEGMENT_I(segment_index).single_animation_override.cycle_time__rate_ms__save_state == 0)
    {
      SEGMENT_I(segment_index).single_animation_override.cycle_time__rate_ms__save_state =
        SEGMENT_I(segment_index).cycle_time__rate_ms;
    }

    time_tmp = SEGMENT_I(segment_index).single_animation_override.time_ms;
    SEGMENT_I(segment_index).single_animation_override.time_ms = 0; // consume override

    // Ensure the next effect tick cannot preempt the one-shot
    const uint16_t guard = (time_tmp > 1000) ? 500 : 10;
    SEGMENT_I(segment_index).cycle_time__rate_ms = time_tmp + guard;

    ALOG_HGL(PSTR("Override: TimeMs=%u  cycle_time__rate_ms=%u  (saved=%u)"),
             time_tmp,
             SEGMENT_I(segment_index).cycle_time__rate_ms,
             SEGMENT_I(segment_index).single_animation_override.cycle_time__rate_ms__save_state);
  }

  // if (SEGMENT_I(segment_index).animation_has_anim_callback)
  // {
  //   SEGMENT_I(segment_index).animator->StartAnimation(0, time_tmp,
  //                                                     SEGMENT_I(segment_index).anim_function_callback);
  // }

  SEGMENT_I(segment_index).transitional = true;
}


/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
// uint8_t mAnimatorLight::get_random_wheel_index(uint8_t pos) {
//   uint8_t r = 0, x = 0, y = 0, d = 0;

//   while(d < 42) {
//     r = hw_random8();
//     x = abs(pos - r);
//     y = 255 - x;
//     d = MIN(x, y);
//   }
//   return r;
// }
uint8_t mAnimatorLight::get_random_wheel_index(uint8_t pos)
{
  constexpr uint8_t minimum_distance = 42;
  constexpr uint8_t maximum_attempts = 32;

  for (uint8_t attempt = 0; attempt < maximum_attempts; attempt++)
  {
    const uint8_t candidate = hw_random8();

    const uint8_t linear_distance =
      (candidate >= pos)
        ? candidate - pos
        : pos - candidate;

    const uint8_t circular_distance =
      min<uint8_t>(linear_distance, 256U - linear_distance);

    if (circular_distance >= minimum_distance)
    {
      return candidate;
    }
  }

  // Guaranteed valid fallback exactly opposite the current position.
  return pos + 128;
}

/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
// void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  

//   Segment& seg = SEGMENT_I(segment_index);

//   /*
//    * Force a transition even when restarting the same effect.
//    */
//   seg.startTransition(10000, true);





//   if(SEGMENT_I(segment_index).effect_id != value)
//   {
//     SEGMENT_I(segment_index).markForReset();
//     SEGMENT_I(segment_index).effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
//   }
//   SEGMENT_I(segment_index).aux0 = 0;//EFFECTS_REGION_COLOUR_SELECT_ID;
//   SEGMENT_I(segment_index).flags.animator_first_run = true; // first run, so do extra things

//   ALOG_DBM(PSTR("segments[segment_index].effect_id=%d"),segments[segment_index].effect_id);

//   #ifdef ENABLE_EFFECT_DESCRIPTIONS   
//   if (value < effects.description.size())
//   ALOG_INF(PSTR("description=%S"),effects.description[value]);
//   ALOG_INF(PSTR("description len=%d"),effects.description.size());
//   #endif
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   char buffer[30];
//   ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_SVALUE_SVALUE_K(D_EFFECTS, D_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

void mAnimatorLight::CommandSet_Flasher_FunctionID(
  uint16_t value,
  uint8_t segment_index
)
{
  Segment& seg = SEGMENT_I(segment_index);

  // Debug: always force a 10-second effect transition.
  // Must occur before changing or resetting the effect.
  // seg.startTransition(10000, true);

  if (seg.effect_id != value)
  {
    seg.effect_id = value;
  }

  // Restart the effect even when the same ID is received.
  // seg.markForReset();

  seg.aux0 = 0;
  seg.flags.animator_first_run = true;

  ALOG_DBM(
    PSTR("segments[segment_index].effect_id=%d"),
    seg.effect_id
  );

  #ifdef ENABLE_EFFECT_DESCRIPTIONS
  // if (value < effects.description.size()){
  //   ALOG_INF(PSTR("description=%S"), effects.description[value]);
  // }
  // ALOG_INF( PSTR("description len=%d"), effects.description.size()  );
  #endif

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];

  ALOG_COM(
    PSTR(D_LOG_PIXEL D_COMMAND_SVALUE_SVALUE_K(D_EFFECTS, D_FUNCTION)),
    GetFlasherFunctionName(buffer, sizeof(buffer))
  );
  #endif
}

int16_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f)
{
  if (f == nullptr) return -2;

  char lineBuffer[256];
  for (size_t i = 0; i < getEffectCount(); i++) {
    strncpy_P(lineBuffer, getModeData_Config(i), sizeof(lineBuffer) - 1);
    lineBuffer[sizeof(lineBuffer) - 1] = '\0'; // terminate string

    ALOG_DBM(PSTR("lineBuffer i%d id%d %s"), i, effects.id[i], lineBuffer);

    char* dataPtr = strchr(lineBuffer, '@');
    if (dataPtr) *dataPtr = '\0'; // replace name divider with null termination. Escape "name@data"

    // Check for a match with the provided name
    if (strcmp(f, lineBuffer) == 0) {
      ALOG_DBM(PSTR("GetFlasherFunctionIDbyName %s->%d"), f, i);
      // return effects.id[i]; 
      return i; // maybe this is wrong, it should not be returning the index of the loop, but the ID of the effect?
    }
  }

  return -1;
}



const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index )
{
  return GetFlasherFunctionNamebyID(SEGMENT_I(segment_index).effect_id, buffer, buflen);
}


const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint16_t id, char* buffer, uint8_t buflen, bool return_first_option_only)
{

  if(id<getEffectsAmount()){
    snprintf_P(buffer, buflen, effects.config[id]);  
    
    char* dataPtr = strchr(buffer,'@');
    if (dataPtr) *dataPtr = 0; // replace name dividor with null termination. Escape "name@data"

  }else{
    snprintf(buffer, buflen, "id_err %d", id);  
  }

  return buffer;
  
}


void mAnimatorLight::FileSystem_JsonAppend_Save_Module()
{

  uint8_t bus_appended = 0;

  char buffer[120] = {0};

  for(uint8_t seg_i = 0; seg_i< getSegmentsNum(); seg_i++)
  {
    JBI->Object_Start_F("Segment%d",seg_i);      

      JBI->Array_Start("PixelRange");
        JBI->Add(segments[seg_i].start);
        JBI->Add(segments[seg_i].stop);
      JBI->Array_End();

      JBI->Add("ColourPalette", GetPaletteNameByID(segments[seg_i].palette_id, buffer, sizeof(buffer)) );

      JBI->Object_Start_F("Effects");      
        JBI->Add("Function",  GetFlasherFunctionNamebyID(segments[seg_i].effect_id, buffer, sizeof(buffer), true));
        JBI->Add("Speed",     segments[seg_i].speed );
        JBI->Add("Intensity", segments[seg_i].intensity );
      JBI->Object_End();
      JBI->Object_Start_F("Transition");
        JBI->Add("TimeMs",  segments[seg_i].animator_blend_time_ms() );
        JBI->Add("RateMs",  segments[seg_i].cycle_time__rate_ms );
      JBI->Object_End();

      for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
      {
        JBI->Array_Start_P(PSTR("SegColour%d"), seg_col);
          for(uint8_t p=0;p<5;p++)
          { 
            JBI->Add(segments[seg_i].segcol[seg_col].getRed()); 
            JBI->Add(segments[seg_i].segcol[seg_col].getGreen()); 
            JBI->Add(segments[seg_i].segcol[seg_col].getBlue()); 
            JBI->Add(segments[seg_i].segcol[seg_col].getWhite()); 
            JBI->Add(segments[seg_i].segcol[seg_col].getCCT()); 
          }
        JBI->Array_End();
      }

    JBI->Object_End();
  }


}


bool mAnimatorLight::isAsterisksOnly(const char* str, byte maxLen)
{
  for (unsigned i = 0; i < maxLen; i++) {
    if (str[i] == 0) break;
    if (str[i] != '*') return false;
  }
  //at this point the password contains asterisks only
  return (str[0] != 0); //false on empty string
}


/**
 * @brief Blends between Rgbcct with 0 to 255 range
 * 
 * @param color1 
 * @param color2 
 * @param blend 
 * @return RgbwwColor 
 * 
 * replicates what is in the segments, but either have inside segment (other) or inside (this)
 * 
 * 
 */
RgbwwColor mAnimatorLight::ColourBlend(RgbwwColor color1, RgbwwColor color2, uint8_t blend) 
{
  Serial.println("here1");
  return RgbwwColor::LinearBlend(color1, color2, blend);//mSupport::mapfloat(blend, 0,255, 0.0f, 1.0f));
}

uint32_t mAnimatorLight::ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend) 
{
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}


///////////////////////////////////////////////////////////////////////////////
// Segment class implementation
///////////////////////////////////////////////////////////////////////////////
uint16_t mAnimatorLight::Segment::_usedSegmentData = 0U; // amount of RAM all segments use for their data[]


// unsigned      mAnimatorLight::Segment::_usedSegmentData   = 0U; // amount of RAM all segments use for their data[]
uint16_t      mAnimatorLight::Segment::maxWidth           = DEFAULT_LED_COUNT;
uint16_t      mAnimatorLight::Segment::maxHeight          = 1;
unsigned      mAnimatorLight::Segment::_vLength           = 0;
unsigned      mAnimatorLight::Segment::_vWidth            = 0;
unsigned      mAnimatorLight::Segment::_vHeight           = 0;
uint32_t      mAnimatorLight::Segment::_currentColors[NUM_COLORS] = {0,0,0};
CRGBPalette16 mAnimatorLight::Segment::_currentPalette    = CRGBPalette16();
CRGBPalette16 mAnimatorLight::Segment::_randomPalette     = generateRandomPalette();  // was CRGBPalette16(DEFAULT_COLOR);
CRGBPalette16 mAnimatorLight::Segment::_newRandomPalette  = generateRandomPalette();  // was CRGBPalette16(DEFAULT_COLOR);
uint16_t      mAnimatorLight::Segment::_lastPaletteChange = 0; // in seconds; perhaps it should be per segment
uint16_t      mAnimatorLight::Segment::_nextPaletteBlend  = 0; // in millis


bool     mAnimatorLight::Segment::_modeBlend = false;
uint16_t mAnimatorLight::Segment::_clipStart = 0;
uint16_t mAnimatorLight::Segment::_clipStop = 0;
uint8_t  mAnimatorLight::Segment::_clipStartY = 0;
uint8_t  mAnimatorLight::Segment::_clipStopY = 1;


mAnimatorLight::Segment&
mAnimatorLight::Segment::operator=(const Segment& orig)
{
  if (this == &orig)
  {
    return *this;
  }

  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  Serial.println(F("-- Copy segment assignment --"));
  #endif

  /*
   * Release destination ownership before replacing its state.
   */
  stopTransition();

  if (name)
  {
    p_free(name);
    name = nullptr;
  }

  deallocateData();
  // deallocateColourData();

  if (pixels)
  {
    p_free(pixels);
    pixels = nullptr;
  }

  if (palette_loaded)
  {
    delete palette_loaded;
    palette_loaded = nullptr;
  }

  /*
   * WLED pattern:
   * copy complete scalar/non-owning state.
   */
  memcpy(this, &orig, sizeof(Segment));

  /*
   * Detach all shallow-copied owned pointers.
   */
  _t             = nullptr;
  name           = nullptr;
  data           = nullptr;
  pixels         = nullptr;
  palette_loaded = nullptr;
  _dataLen    = 0;
  
  if (!orig.stop)
  {
    return *this;
  }

  /*
   * Deep-copy segment name.
   */
  if (orig.name)
  {
    // name = new(std::nothrow) char[strlen(orig.name) + 1];
    name = static_cast<char*>(
      allocate_buffer(
        strlen(orig.name) + 1,
        BFRALLOC_PREFER_PSRAM
      )
    );

    if (name)
    {
      strcpy(name, orig.name);
    }

  }

  /*
   * Deep-copy effect runtime data.
   */
  if (orig.data && orig._dataLen > 0)
  {
    if (allocateData(orig._dataLen))
    {
      memcpy(data, orig.data, orig._dataLen);
    }
  }

  /*
   * Deep-copy segment framebuffer.
   */
  if (orig.pixels && orig.length() > 0)
  {
    const size_t pixel_bytes =
      static_cast<size_t>(orig.length()) * sizeof(uint32_t);

    // pixels = static_cast<uint32_t*>(
    //   allocate_buffer(
    //     pixel_bytes,
    //     BFRALLOC_PREFER_PSRAM |
    //     BFRALLOC_NOBYTEACCESS
    //   )
    // );

const size_t requested_pixel_bytes =
  static_cast<size_t>(length()) * sizeof(uint32_t);

pixels = static_cast<uint32_t*>(
  allocate_buffer(
    requested_pixel_bytes,
    BFRALLOC_PREFER_PSRAM |
    BFRALLOC_NOBYTEACCESS |
    BFRALLOC_CLEAR
  )
);

size_t actual_pixel_bytes = 0;

#ifdef ESP32
if (pixels)
{
  actual_pixel_bytes = heap_caps_get_allocated_size(pixels);
}
#endif

ALOG_INF(PSTR("SEG PIXEL ALLOC obj=%p len=%u sizeofPixel=%u requested=%u actual=%u ptr=%p"), (void*)this, length(), sizeof(uint32_t), (unsigned)requested_pixel_bytes, (unsigned)actual_pixel_bytes, (void*)pixels);
ALOG_INF(PSTR("PALETTE BLOCK: ptr=%p sizeof=%u actual=%u"), (void*)palette_loaded, (unsigned)sizeof(mPaletteLoaded), palette_loaded ? (unsigned)heap_caps_get_allocated_size(palette_loaded) : 0);


    if (pixels)
    {
      memcpy(pixels, orig.pixels, pixel_bytes);
    }
    else
    {
      DEBUG_PRINTLN(F("!!! Not enough RAM for assigned segment pixel buffer !!!"));
      stop = 0;
      return *this;
    }
  }

  /*
   * PulSar addition:
   * deep-copy palette container.
   */
  if (orig.palette_loaded)
  {
    palette_loaded =
      new(std::nothrow) mPaletteLoaded(*orig.palette_loaded);
  }
  else
  {
    palette_loaded =
      new(std::nothrow) mPaletteLoaded();
  }

  return *this;
}


mAnimatorLight::Segment&
mAnimatorLight::Segment::operator=(Segment&& orig) noexcept
{
  if (this == &orig)
  {
    return *this;
  }

  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  Serial.println(F("-- Move segment assignment --"));
  #endif

  /*
   * Release destination ownership.
   */
  stopTransition();

  if (name)
  {p_free(name);
name = nullptr;
  }

  deallocateData();
  // deallocateColourData();

  if (pixels)
  {
    p_free(pixels);
    pixels = nullptr;
  }

  if (palette_loaded)
  {
    delete palette_loaded;
    palette_loaded = nullptr;
  }

  /*
   * WLED pattern:
   * transfer complete state and all owned pointers.
   */
  memcpy(this, &orig, sizeof(Segment));

  /*
   * Remove ownership from source.
   */
  orig._t             = nullptr;
  orig.name           = nullptr;
  orig.data           = nullptr;
  orig.pixels         = nullptr;
  orig.palette_loaded = nullptr;
  orig._dataLen    = 0;
  
  orig.start = 0;
  orig.stop  = 0;

  return *this;
}


// bool mAnimatorLight::Segment::allocateData(size_t len) 
// {
  
//   if (data && _dataLen == len) return true; //already allocated
//   deallocateData();
//   if (mAnimatorLight::Segment::getUsedSegmentData() + len > MAX_SEGMENT_DATA)
//   { 
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
//     effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//     return false; //not enough memory
//   }
//   // if possible use SPI RAM on ESP32
//   #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
//   if (psramFound())
//     data = (byte*) ps_malloc(len);
//   else
//   #endif

//   data = (byte*) malloc(len);

//   if (!data){
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
//     effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//     return false; //allocation failed
//   }

//   addUsedSegmentData(len);
//   _dataLen = len;
//   memset(data, 0, len);

//   DEBUG_LINE_HERE

//   return true;
// }


// void mAnimatorLight::Segment::deallocateData()
// {
//   if (!data) return;
//   free(data);
//   data = nullptr;
//   addUsedSegmentData(-_dataLen);
//   _dataLen = 0;
// }



// allocates effect data buffer on heap and initialises (erases) it
bool mAnimatorLight::Segment::allocateData(size_t len) {
  if (len == 0) return false;    // nothing to do
  if (data && _dataLen >= len) { // already allocated enough (reduce fragmentation)
    if (call == 0) {
      if (_dataLen < FAIR_DATA_PER_SEG) { // segment data is small
        DEBUG_PRINTF_P(PSTR("--   Clearing data (%d): %p\n"), len, this);
        memset(data, 0, len);  // erase buffer if called during effect initialisation
        return true; // no need to reallocate
      }
    }
    else
      return true;
  }
  #ifdef ENABLE_DEBUG_LIGHTS__ALLOCATE_DATA
  DEBUG_PRINTF_P(PSTR("--   Allocating data (%d): %p\n"), len, this);
  #endif
  // limit to MAX_SEGMENT_DATA if there is no PSRAM, otherwise prefer functionality over speed
  #ifndef BOARD_HAS_PSRAM
  if (Segment::getUsedSegmentData() + len - _dataLen > MAX_SEGMENT_DATA) {
    // not enough memory
    DEBUG_PRINTF_P(PSTR("SegmentData limit reached: %d/%d\n"), len, Segment::getUsedSegmentData());
    // errorFlag = ERR_NORAM;
    return false;
  }
  #endif

  if (data) {
    d_free(data); // free data and try to allocate again (segment buffer may be blocking contiguous heap)
    Segment::addUsedSegmentData(-_dataLen); // subtract buffer size
  }

  data = static_cast<byte*>(allocate_buffer(len, BFRALLOC_PREFER_DRAM | BFRALLOC_CLEAR)); // prefer DRAM over PSRAM for speed

  if (data) {
    Segment::addUsedSegmentData(len);
    _dataLen = len;
    #ifdef ENABLE_DEBUG_LIGHTS__ALLOCATE_DATA
    DEBUG_PRINTF_P(PSTR("---  Allocated data (%p): %d/%d -> %p\n"), this, len, Segment::getUsedSegmentData(), data);
    #endif
    return true;
  }
  // allocation failed
  DEBUG_PRINTLN(F("!!! Allocation failed. !!!"));
  // errorFlag = ERR_NORAM;
  return false;
}

void mAnimatorLight::Segment::deallocateData() {
  if (!data) { _dataLen = 0; return; }
  if ((Segment::getUsedSegmentData() > 0) && (_dataLen > 0)) { // check that we don't have a dangling / inconsistent data pointer
    #ifdef ENABLE_DEBUG_LIGHTS__ALLOCATE_DATA
    DEBUG_PRINTF_P(PSTR("---  Released data (%p): %d/%d -> %p\n"), this, _dataLen, Segment::getUsedSegmentData(), data);
    #endif
    d_free(data);
  } else {
    DEBUG_PRINTF_P(PSTR("---- Released data (%p): inconsistent UsedSegmentData (%d/%d), cowardly refusing to free nothing.\n"), this, _dataLen, Segment::getUsedSegmentData());
  }
  data = nullptr;
  Segment::addUsedSegmentData(_dataLen <= Segment::getUsedSegmentData() ? -_dataLen : -Segment::getUsedSegmentData());
  _dataLen = 0;
}


/** 
  * If reset of this segment was requested, clears runtime
  * settings of this segment.
  * Must not be called while an effect mode function is running
  * because it could access the data buffer and this method 
  * may free that data buffer.
  */
void mAnimatorLight::Segment::resetIfRequired() {
  if (!reset || !isActive()) return;
  //DEBUG_PRINTF_P(PSTR("-- Segment reset: %p\n"), this);
  if (data && _dataLen > 0) {
    if (_dataLen > (MAX_SEGMENT_DATA / tkr_anim->getMaxSegments())) deallocateData(); // do not keep large allocations
    else memset(data, 0, _dataLen);  // can prevent heap fragmentation
    #ifdef ENABLE_DEBUG_LIGHTS__ALLOCATE_DATA
    DEBUG_PRINTF_P(PSTR("-- Segment %p reset, data cleared\n"), this);
    #endif
  }
  if (pixels) for (size_t i = 0; i < length(); i++) pixels[i] = BLACK; // clear pixel buffer
  step = 0; call = 0; aux0 = 0; aux1 = 0;  aux2 = 0;  aux3 = 0;   aux4 = 0;  
  effect_init_runtime = millis(); // save when an effect first started
  reset = false;
  #ifdef WLED_ENABLE_GIF
  endImagePlayback(this);
  #endif


  // if (reset) {
    
  // ALOG_INF(PSTR(D_LOG_PIXEL "resetIfRequired AuxOptions Segment = %d,%d,%d,%d"),
  //   aux0,
  //   aux1,
  //   aux2,
  //   aux3
  // );

    // if (leds && !mAnimatorLight::Segment::_globalLeds) { free(leds); leds = nullptr; }
    //if (_t) { delete _t; _t = nullptr; transitional = false; }
    // next_time = 0; 
    step = 0; call = 0; 
    effect_init_runtime = millis(); // save when an effect first started
    
    /**
     * @brief Potential issue with WLED effects, but removing aux options from reset since they may be used as config options
     * 
     */
    aux0 = 0; aux1 = 0;  aux2 = 0;  aux3 = 0;   aux4 = 0;  
    
    live_palette.timing1 = 0;
        

    /*
    * Secondary palettes are effect-runtime resources.
    * A new effect will allocate/load one again only if required.
    */
    ReleasePalette2();

    reset = false; // setOption(SEG_OPTION_RESET, false);
    // Serial.println(DEBUG_INSERT_PAGE_BREAK "mAnimatorLight::Segment::resetIfRequired()"); //delay(5000);
  // }
}


// starting a transition has to occur before change so we get current values 1st
// note: _t is the temporary segment that holds the values transitioned from (palette, colors, brightness,...) and the current segment holds the "to" values
//       if this is a non FADE transition or an FX change, the _oldSegment is created which is a full copy of the segment before the change
void mAnimatorLight::Segment::startTransition(uint16_t dur, bool segmentCopy) {

  // ALOG_INF(PSTR("TRANS START seg=%p dur=%u copy=%u active=%u existing=%u fx=%u"), this, dur, segmentCopy, isActive(), isInTransition(), effect_id);

  if (dur == 0 || !isActive()) {

    // ALOG_ERR(PSTR("TRANS REJECTED seg=%p dur=%u active=%u"), this, dur, isActive());
    if (isInTransition()) _t->_dur = 0;

    transition_owned_by_effect = false;

    return;
  }
  if (isInTransition()) {
    if (segmentCopy && !_t->_oldSegment) {
      // already in transition but segment copy requested and not yet created
      // _t->_oldSegment = new(std::nothrow) Segment(*this); // store/copy current segment settings
        if (segmentCopy) _t->_oldSegment = new(std::nothrow) Segment(*this);

  // ALOG_INF(PSTR("TRANS OLD COPY seg=%p old=%p oldPixels=%p"), this, _t ? _t->_oldSegment : nullptr, (_t && _t->_oldSegment) ? _t->_oldSegment->pixels : nullptr);
      _t->_start = millis(); // restart transition timer
      _t->_dur   = dur;
      _t->_prevPaletteBlends = 0; // reset palette blends
      if (_t->_oldSegment) {
        _t->_oldSegment->palette = _t->_palette; // restore original palette, colors, brightness and CCT (from start of transition)
        for (unsigned i = 0; i < NUM_COLORS; i++) _t->_oldSegment->segcol[i].colour = _t->_colors[i];
        _t->_oldSegment->opacity = _t->_bri;
        _t->_oldSegment->cct     = _t->_cct;
        // if already partway through a FADE transition, set old segment's colors to current blend to avoid jumping back to original colors
        if (_t->_progress > 0) {
          // already in a transition, see comment below
          for (unsigned i = 0; i < NUM_COLORS; i++) _t->_oldSegment->segcol[i].colour = color_blend16(_t->_colors[i], segcol[i].colour, _t->_progress);
          _t->_oldSegment->opacity = currentBri(); // update "original" brightness note: _t->_progress is updated in updateTransitionProgress() so still valid here
          _t->_oldSegment->cct     = currentCCT(); // update "original" CCT (reduces jump)
        }
        // DEBUGFX_PRINTF_P(PSTR("-- Updated transition with segment copy: S=%p T(%p) O[%p] OP[%p]\n"), this, _t, _t->_oldSegment, _t->_oldSegment->pixels);
        if (!_t->_oldSegment->isActive()) stopTransition();
      }
    } else if (_t->_progress > 0) {
      // already in a transition: capture the current visual blend as the new "from" state so the incoming change does not cause a visible jump.
      // _palT already holds the intermediate blended palette and will continue blending toward the new target (see beginDraw()), so no palette action needed.
      // initial version by @blazoncek (https://github.com/blazoncek/WLED/commit/40d9812)
      for (unsigned i = 0; i < NUM_COLORS; i++) _t->_colors[i] = color_blend16(_t->_colors[i], segcol[i].colour, _t->_progress);
      _t->_bri = currentBri(); // update "original" brightness note: _t->_progress is updated in updateTransitionProgress() so still valid here
      _t->_cct = currentCCT(); // update "original" CCT (reduces jump)
      // restart transition timer only if a pure FADE transition, otherwise let the FX change or non-FADE transition finish
      // this avoids a re-start of the transition if color or brightness is changed during an ongoing FX or non-FADE transition
      if (tkr_anim->blendingStyle == TRANSITION_FADE) {
        if (_t->_oldSegment != nullptr) {
          if (_t->_oldSegment->effect_id != effect_id)
            return; // do not reset transition if this is an FX change, note: the disadvantage is that colors still jump in that case
        }
        _t->_start = millis();
        _t->_dur   = dur;
        _t->_prevPaletteBlends = 0;
      }
    }
    return;
  }

  // no previous transition running, start by allocating memory for segment copy
  _t = new(std::nothrow) Transition(dur);

  // ALOG_INF(PSTR("TRANS ALLOC seg=%p transition=%p dur=%u"), this, _t, dur);
  
  if (_t) {
    _t->_bri = on ? opacity : 0;
    _t->_cct = cct;
    _t->_palette = palette;
    #ifndef WLED_SAVE_RAM
    // loadPalette(_t->_palT, palette);
    #endif
    for (int i=0; i<NUM_COLORS; i++) _t->_colors[i] = segcol[i].colour;
    if (segmentCopy) _t->_oldSegment = new(std::nothrow) Segment(*this); // store/copy current segment settings
    if (_t->_oldSegment) {
      // DEBUG_PRINTF_P(PSTR("-- Started transition: S=%p T(%p) O[%p] OP[%p]\n"), this, _t, _t->_oldSegment, _t->_oldSegment->pixels);
      if (!_t->_oldSegment->isActive()) stopTransition();
    } else {
      // DEBUGFX_PRINTF_P(PSTR("-- Started transition without old segment: S=%p T(%p)\n"), this, _t);
    }
  };
}

// void mAnimatorLight::Segment::stopTransition() {
//   if (_t == nullptr) return; // no ongoing transition
//   // DEBUG_PRINTF_P(PSTR("-- Stopping transition: S=%p T(%p) O[%p]\n"), this, _t, _t->_oldSegment);
//   delete _t;
//   _t = nullptr;
// }
void mAnimatorLight::Segment::stopTransition()
{
  if (_t == nullptr) // no ongoing transition
  {
    transition_owned_by_effect = false;
    return;
  }

  delete _t;
  _t = nullptr;

  transition_owned_by_effect = false;
}


// sets transition progress variable (0-65535) based on time passed since transition start
// void mAnimatorLight::Segment::updateTransitionProgress() const {
//   if (isInTransition()) {
//     _t->_progress = 0xFFFF;
//     unsigned diff = millis() - _t->_start;
//     if (_t->_dur > 0 && diff < _t->_dur) _t->_progress = diff * 0xFFFFU / _t->_dur;
//   }
// }
void mAnimatorLight::Segment::updateTransitionProgress() const
{
  if (!isInTransition())
  {
    return;
  }

  _t->_progress = 0xFFFFU;

  const unsigned elapsed = millis() - _t->_start;

  if (_t->_dur > 0 && elapsed < _t->_dur)
  {
    _t->_progress =
      static_cast<uint32_t>(elapsed) * 0xFFFFU / _t->_dur;
  }
}



// will return segment's CCT during a transition
// isPreviousMode() is actually not implemented for CCT in strip.service() as WLED does not support per-pixel CCT
uint8_t mAnimatorLight::Segment::currentCCT() const {
  unsigned prog = progress();
  if (prog < 0xFFFFU) {
    if (tkr_anim->blendingStyle == TRANSITION_FADE) return (cct * prog + (_t->_cct * (0xFFFFU - prog))) / 0xFFFFU;
    //else                                   return Segment::isPreviousMode() ? _t->_cct : cct;
  }
  return cct;
}

// will return segment's opacity during a transition (blending it with old in case of FADE transition)
uint8_t mAnimatorLight::Segment::currentBri() const {
  unsigned prog = progress();
  unsigned curBri = on ? opacity : 0;
  if (prog < 0xFFFFU) {
    // this will blend opacity in new mode if style is FADE (single effect call)
    if (tkr_anim->blendingStyle == TRANSITION_FADE) curBri = (prog * curBri + _t->_bri * (0xFFFFU - prog)) / 0xFFFFU;
    else                                  curBri = Segment::isPreviousMode() ? _t->_bri : curBri;
  }
  return curBri;
}


// pre-calculate drawing parameters for faster access (based on the idea from @softhack007 from MM fork)
// and blends colors and palettes if necessary
// prog is the progress of the transition (0-65535) and is passed to the function as it may be called in the context of old segment
// which does not have transition structure
void mAnimatorLight::Segment::beginDraw(uint16_t prog) {
  setDrawDimensions();
  // load colors into _currentColors
  for (unsigned i = 0; i < NUM_COLORS; i++) _currentColors[i] = segcol[i].colour;
  // load palette into _currentPalette
  loadPalette(Segment::_currentPalette, palette);
  if (isInTransition() && prog < 0xFFFFU && tkr_anim->blendingStyle == TRANSITION_FADE) {
    // blend colors
    for (unsigned i = 0; i < NUM_COLORS; i++) _currentColors[i] = color_blend16(_t->_colors[i], segcol[i].colour, prog);
    // blend palettes
    // there are about 255 blend passes of 48 "blends" to completely blend two palettes (in _dur time)
    // minimum blend time is 100ms maximum is 65535ms
    #ifndef WLED_SAVE_RAM
    unsigned noOfBlends = ((255U * prog) / 0xFFFFU) - _t->_prevPaletteBlends;
    if (noOfBlends > 255) noOfBlends = 255; // safety check
    for (unsigned i = 0; i < noOfBlends; i++, _t->_prevPaletteBlends++) nblendPaletteTowardPalette(_t->_palT, Segment::_currentPalette, 48);
    Segment::_currentPalette = _t->_palT; // copy transitioning/temporary palette
    #else
    unsigned noOfBlends = ((255U * prog) / 0xFFFFU);
    CRGBPalette16 tmpPalette;
    loadPalette(tmpPalette, _t->_palette);
    for (unsigned i = 0; i < noOfBlends; i++) nblendPaletteTowardPalette(tmpPalette, Segment::_currentPalette, 48);
    Segment::_currentPalette = tmpPalette; // copy transitioning/temporary palette
    #endif
  }
}



// sets Segment geometry (length or width/height and grouping, spacing and offset as well as 2D mapping)
// strip must be suspended (strip.suspend()) before calling this function
// this function may call fill() to clear pixels if spacing or mapping changed (which requires setting _vWidth, _vHeight, _vLength or beginDraw())
void mAnimatorLight::Segment::setGeometry(
  uint16_t i1,
  uint16_t i2,
  uint8_t grp,
  uint8_t spc,
  uint16_t ofs,
  uint16_t i1Y,
  uint16_t i2Y,
  uint8_t m12
)
{
  // return if neither bounds nor grouping have changed
  bool boundsUnchanged = (start == i1 && stop == i2);

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  boundsUnchanged &= (startY == i1Y && stopY == i2Y); // 2D
  #endif

  boundsUnchanged &= (grouping == grp && spacing == spc); // changing grouping and/or spacing changes virtual segment length (painting dimensions)

  // if (stop && (spc > 0 || m12 != map1D2D)) clear();

  if (grp)
  {
    // prevent assignment of 0
    grouping = grp;
    spacing = spc;
  }
  else
  {
    grouping = 1;
    spacing = 0;
  }

  if (ofs < UINT16_MAX)
  {
    offset = ofs;
  }

  map1D2D = constrain(m12, 0, 7);

  if (boundsUnchanged)
  {
    return;
  }

  unsigned oldLength = length();

  DEBUGFX_PRINTF_P(PSTR("Segment geometry: %d,%d -> %d,%d [%d,%d]\n"), (int)i1, (int)i2, (int)i1Y, (int)i2Y, (int)grp, (int)spc);

  markForReset();
  stopTransition(); // we can't use transition if segment dimensions changed
  // stateChanged = true; // send UDP/WS broadcast

  // apply change immediately
  if (i2 <= i1)
  {
    // disable segment
    #ifdef WLED_ENABLE_GIF
    endImagePlayback(this);
    #endif

    deallocateData();

    p_free(pixels);
    pixels = nullptr;
    pixels_allocated_bytes = 0;

    stop = 0;
    return;
  }

  if (
    i1 < Segment::maxWidth ||
    (
      i1 >= Segment::maxWidth * Segment::maxHeight &&
      i1 < tkr_anim->getLengthTotal()
    )
  )
  {
    start = i1;
  }

  // Segment::maxWidth equals strip.getLengthTotal() for 1D
  stop =
    i2 > Segment::maxWidth * Segment::maxHeight &&
    i1 >= Segment::maxWidth * Segment::maxHeight
      ? MIN(i2, tkr_anim->getLengthTotal())
      : constrain(i2, 1, Segment::maxWidth);

  startY = 0;
  stopY = 1;

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (Segment::maxHeight > 1)
  {
    // 2D
    if (i1Y < Segment::maxHeight)
    {
      startY = i1Y;
    }

    stopY = constrain(i2Y, 1, Segment::maxHeight);
  }
  #endif

  // safety check
  if (start >= stop || startY >= stopY)
  {
    #ifdef WLED_ENABLE_GIF
    endImagePlayback(this);
    #endif

    deallocateData();

    p_free(pixels);
    pixels = nullptr;
    pixels_allocated_bytes = 0;

    stop = 0;
    return;
  }

  // allocate FX render buffer
  if (length() != oldLength)
  {
    // allocate render buffer (always entire segment), prefer IRAM/PSRAM.
    // Note: impact on FPS with PSRAM buffer is low (<2% with QSPI PSRAM) on S2/S3.

    p_free(pixels);
    pixels = nullptr;
    pixels_allocated_bytes = 0;

    const size_t requested_pixel_bytes = static_cast<size_t>(length()) * sizeof(uint32_t);

    pixels = static_cast<uint32_t*>(
      allocate_buffer(
        requested_pixel_bytes,
        BFRALLOC_PREFER_PSRAM |
        BFRALLOC_NOBYTEACCESS |
        BFRALLOC_CLEAR
      )
    );

    pixels_allocated_bytes = pixels ? requested_pixel_bytes : 0;

    size_t actual_pixel_bytes = 0;

    #ifdef ESP32
    if (pixels)
    {
      actual_pixel_bytes = heap_caps_get_allocated_size(pixels);
    }
    #endif

    ALOG_INF(PSTR("SEG PIXEL ALLOC obj=%p len=%u sizeofPixel=%u requested=%u actual=%u ptr=%p"), (void*)this, length(), sizeof(uint32_t), (unsigned)requested_pixel_bytes, (unsigned)actual_pixel_bytes, (void*)pixels);

    if (!pixels)
    {
      DEBUGFX_PRINTLN(F("!!! Not enough RAM for pixel buffer !!!"));

      #ifdef WLED_ENABLE_GIF
      endImagePlayback(this);
      #endif

      deallocateData();

      pixels_allocated_bytes = 0;

      // errorFlag = ERR_NORAM_PX;
      stop = 0;
      return;
    }
  }

  refreshLightCapabilities();
}


void mAnimatorLight::Segment::refreshGeometry()
{
  const size_t required_pixel_bytes = static_cast<size_t>(length()) * sizeof(uint32_t);

  /*
   * Disabled or invalid segment: no render buffer should exist.
   */
  if (required_pixel_bytes == 0)
  {
    if (pixels)
    {
      p_free(pixels);
      pixels = nullptr;
    }

    pixels_allocated_bytes = 0;
    return;
  }

  /*
   * Buffer already matches the current segment geometry.
   */
  if (pixels != nullptr && pixels_allocated_bytes == required_pixel_bytes)
  {
    return;
  }

  ALOG_INF(PSTR("SEG REFRESH GEOMETRY: obj=%p len=%u allocated=%u required=%u oldPixels=%p"), (void*)this, length(), (unsigned)pixels_allocated_bytes, (unsigned)required_pixel_bytes, (void*)pixels);

  p_free(pixels);
  pixels = nullptr;
  pixels_allocated_bytes = 0;

  pixels = static_cast<uint32_t*>(
    allocate_buffer(
      required_pixel_bytes,
      BFRALLOC_PREFER_PSRAM |
      BFRALLOC_NOBYTEACCESS |
      BFRALLOC_CLEAR
    )
  );

  if (!pixels)
  {
    DEBUGFX_PRINTLN(F("!!! Not enough RAM for pixel buffer !!!"));

    pixels_allocated_bytes = 0;
    stop = 0;
    return;
  }

  pixels_allocated_bytes = required_pixel_bytes;

  #ifdef ESP32
  const size_t actual_pixel_bytes = heap_caps_get_allocated_size(pixels);
  #else
  const size_t actual_pixel_bytes = required_pixel_bytes;
  #endif

  ALOG_INF(PSTR("SEG PIXEL REFRESHED: obj=%p len=%u tracked=%u actual=%u ptr=%p"), (void*)this, length(), (unsigned)pixels_allocated_bytes, (unsigned)actual_pixel_bytes, (void*)pixels);
}







void mAnimatorLight::Segment::loadPalette(CRGBPalette16 &targetPalette, uint8_t pal) {
  
  targetPalette = PartyColors_gc22;

}


void mAnimatorLight::Segment::setUp(uint16_t i1, uint16_t i2, uint8_t grp, uint8_t spc, uint16_t ofs, uint16_t i1Y, uint16_t i2Y) {
  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (start == i1 && stop == i2);
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (Segment::maxHeight>1) boundsUnchanged &= (startY == i1Y && stopY == i2Y); // 2D
  #endif
  if (boundsUnchanged
      && (!grp || (grouping_get() == grp && spacing == spc))
      && (ofs == UINT16_MAX || ofs == offset)) return;

  if (stop) fill(BLACK); //turn old segment range off
  if (i2 <= i1) { //disable segment
    stop = 0;
    // markForReset();
    return;
  }
  if (i1 < Segment::maxWidth || (i1 >= Segment::maxWidth*Segment::maxHeight && i1 < tkr_anim->getLengthTotal())) start = i1; // Segment::maxWidth equals tkr_anim->getLengthTotal() for 1D
  stop = i2 > Segment::maxWidth*Segment::maxHeight ? MIN(i2,tkr_anim->getLengthTotal()) : (i2 > Segment::maxWidth ? Segment::maxWidth : MAX(1,i2));
  startY = 0;
  stopY  = 1;
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (Segment::maxHeight>1) { // 2D
    if (i1Y < Segment::maxHeight) startY = i1Y;
    stopY = i2Y > Segment::maxHeight ? Segment::maxHeight : MAX(1,i2Y);
  }
  #endif
  if (grp) {
    grouping_set(grp);
    spacing = spc;
  }
  if (ofs < UINT16_MAX) offset = ofs;
  // markForReset();
  if (!boundsUnchanged) refreshLightCapabilities();
}



mAnimatorLight::Segment &mAnimatorLight::Segment::setColor(uint8_t slot, uint32_t c) {
  if (slot >= NUM_COLORS || c == segcol[slot].colour) return *this;
  if (!_isRGB && !_hasW) {
    if (slot == 0 && c == BLACK) return *this; // on/off segment cannot have primary color black
    if (slot == 1 && c != BLACK) return *this; // on/off segment cannot have secondary color non black
  }
  //DEBUG_PRINTF_P(PSTR("- Starting color transition: %d [0x%X]\n"), slot, c);
  startTransition(tkr_anim->getTransition(), tkr_anim->blendingStyle != TRANSITION_FADE); // start transition prior to change
  segcol[slot] = c;
  tkr_anim->stateChanged = true; // send UDP/WS broadcast
  return *this;
}

mAnimatorLight::Segment &mAnimatorLight::Segment::setCCT(uint16_t k) {
  if (k > 255) { //kelvin value, convert to 0-255
    if (k < 1900)  k = 1900;
    if (k > 10091) k = 10091;
    k = (k - 1900) >> 5;
  }
  if (cct != k) {
    //DEBUG_PRINTF_P(PSTR("- Starting CCT transition: %d\n"), k);
    startTransition(tkr_anim->getTransition(), false); // start transition prior to change (no need to copy segment)
    cct = k;
    tkr_anim->stateChanged = true; // send UDP/WS broadcast
  }
  return *this;
}

mAnimatorLight::Segment &mAnimatorLight::Segment::setOpacity(uint8_t o) {
  if (opacity != o) {
    //DEBUG_PRINTF_P(PSTR("- Starting opacity transition: %d\n"), o);
    startTransition(tkr_anim->getTransition(), tkr_anim->blendingStyle != TRANSITION_FADE); // start transition prior to change
    opacity = o;
    tkr_anim->stateChanged = true; // send UDP/WS broadcast
  }
  return *this;
}

mAnimatorLight::Segment &mAnimatorLight::Segment::setOption(uint8_t n, bool val) {
  bool prev = (options >> n) & 0x01;
  if (val == prev) return *this;
  //DEBUG_PRINTF_P(PSTR("- Starting option transition: %d\n"), n);
  if (n == SEG_OPTION_ON) startTransition(tkr_anim->getTransition(), tkr_anim->blendingStyle != TRANSITION_FADE); // start transition prior to change
  if (val) options |=   0x01 << n;
  else     options &= ~(0x01 << n);
  tkr_anim->stateChanged = true; // send UDP/WS broadcast
  return *this;
}


mAnimatorLight::Segment &mAnimatorLight::Segment::setEffect(uint16_t fx, bool loadDefaults) 
{
  char buffer[10];
  ALOG_INF(PSTR("setEffect %d %s"), fx, tkr_anim->GetFlasherFunctionNamebyID(fx, buffer, sizeof(buffer)));
  
  #ifdef ENABLE_EFFECT_DESCRIPTIONS
  char buffer_description[100];
  // ALOG_INF(PSTR("Effect Description\n\r_________\n\r%s\n\r%S\n\r_________"), tkr_anim->GetFlasherFunctionNamebyID(fx, buffer_description, sizeof(buffer_description)), tkr_anim->effects.description[fx]);
  #endif

  bool mode_changed = false;
  if (fx != effect_id) mode_changed = true;

  // Debug: always capture the current segment before changing anything, then force a 10-second transition.
  // startTransition(10000, true);

  effect_id = fx;

  if (loadDefaults) 
  {

    ALOG_INF(PSTR("setEffect LoadDefaults"));

    int16_t sOpt = -1;

    sOpt = tkr_anim->extractModeDefaults(fx, "sx");   if (sOpt >= 0) speed = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "ix");   if (sOpt >= 0) intensity = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "pal");  if (sOpt >= 0) setPalette(sOpt);

    sOpt = tkr_anim->extractModeDefaults(fx, "pal2"); if (sOpt >= 0) palette2_id = sOpt;
    
    char paletteName[32];

    if (tkr_anim->extractModeDefaults(fx, "paln", paletteName, sizeof(paletteName)))
    {
      ALOG_INF(PSTR("paln=%s"), paletteName);
      int16_t tmp_id = -1;
      if ((tmp_id = tkr_anim->GetPaletteIDbyName(paletteName)) >= 0)
      {
        ALOG_INF(PSTR("pal=%d"), tmp_id);
        palette_id = tmp_id;
        LoadPalette(palette_id);
      }
    }
    
    if (tkr_anim->extractModeDefaults(fx, "pal2n", paletteName, sizeof(paletteName)))
    {
      ALOG_INF(PSTR("pal2n=%s"), paletteName);
      int16_t tmp_id = -1;
      if ((tmp_id = tkr_anim->GetPaletteIDbyName(paletteName)) >= 0)
      {
        ALOG_INF(PSTR("pal2n=%d"), tmp_id);
        palette2_id = tmp_id;
      }
    }

    sOpt = tkr_anim->extractModeDefaults(fx, "ep");    if (sOpt >= 0) cycle_time__rate_ms = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "c1");    if (sOpt >= 0) custom1 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "c2");    if (sOpt >= 0) custom2 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "c3");    if (sOpt >= 0) custom3 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "o1");    if (sOpt >= 0) check1 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "o2");    if (sOpt >= 0) check2 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "o3");    if (sOpt >= 0) check3 = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "m12");   if (sOpt >= 0) map1D2D = constrain(sOpt, 0, 7);
    sOpt = tkr_anim->extractModeDefaults(fx, "si");    if (sOpt >= 0) soundSim = constrain(sOpt, 0, 7);
    sOpt = tkr_anim->extractModeDefaults(fx, "rev");   if (sOpt >= 0) reverse = (bool)sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "mi");    if (sOpt >= 0) mirror = (bool)sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "rY");    if (sOpt >= 0) reverse_y = (bool)sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "mY");    if (sOpt >= 0) mirror_y = (bool)sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "p0");    if (sOpt >= 0) params_user[0] = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "p1");    if (sOpt >= 0) params_user[1] = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "p2");    if (sOpt >= 0) params_user[2] = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "p3");    if (sOpt >= 0) params_user[3] = sOpt;
    sOpt = tkr_anim->extractModeDefaults(fx, "palix"); if (sOpt >= 0) live_palette.intensity = sOpt;

    for (uint8_t sidx = 0; sidx < 5; ++sidx)
    {
      char key[4];
      snprintf(key, sizeof(key), "s%u", sidx);

      char hexBuf[16] = {0};
      if (tkr_anim->extractModeDefaults(fx, key, hexBuf, sizeof(hexBuf)))
      {
        uint8_t r=0, g=0, b=0, ww=0, cw=0;
        uint32_t col_org = segcol[sidx].colour;
        r  = R(col_org); 
        g  = G(col_org); 
        b  = B(col_org); 
        ww = W(col_org); 
        cw = W(col_org);

        if (parseSegColorHex(hexBuf, r, g, b, ww, cw))
        {
          segcol[sidx] = SegmentColour(r, g, b, cw, ww);
          ALOG_INF(PSTR("s%d=%s => R=%d G=%d B=%d WW=%d CW=%d"), sidx, hexBuf, r, g, b, ww, cw);
        }
      }
    }
  }

  // flags.animator_first_run = true;
  markForReset();
  // next_time = 0;
  // tkr_anim->force_update();

  if (mode_changed) tkr_anim->stateChanged = true;
  
  return *this;
}


mAnimatorLight::Segment &mAnimatorLight::Segment::setPalette(uint8_t pal)
{
  ALOG_INF(PSTR("setPalette(%u) current=%u seg=%p"), pal, palette_id, this);

  if (pal != palette_id)
  {
    if (!transition_owned_by_effect)
    {
      startTransition(
        tkr_anim->getTransition(),
        true
      );
    }
    ALOG_INF(PSTR("setPalette transition started seg=%p oldPal=%u newPal=%u"), this, palette_id, pal);

    palette_id = pal;
    LoadPalette(palette_id);

    flags.animator_first_run = true;
    markForReset();
    // next_time = 0;
    tkr_anim->force_update();
  }
  return *this;
}


mAnimatorLight::Segment &mAnimatorLight::Segment::setName(const char *newName) {
  if (newName) {
    const int newLen = min(strlen(newName), (size_t)WLED_MAX_SEGNAME_LEN);
    if (newLen) {
      if (name) p_free(name); // free old name
      name = static_cast<char*>(allocate_buffer(newLen+1, BFRALLOC_PREFER_PSRAM));
      #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
      if (effect_id == EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID) startTransition(tkr_anim->getTransition(), true); // if the name changes in scrolling text mode, we need to copy the segment for blending
      #endif
      if (name) strlcpy(name, newName, newLen+1);
      return *this;
    }
  }
  return clearName();
}




// Parse hex like: f / ff / fff / ffffff / abcde / aabbccddeeff  (RGBWC)
// Rules:
// - strip '#' and spaces
// - odd length  => 1 hex per channel:  [R][G][B][W][C]
// - even length => 2 hex per channel: [RR][GG][BB][WW][CW]
// - up to 5 channels; missing channels default to 0
// - special case: "0" => all black (0,0,0,0,0)
bool mAnimatorLight::Segment::parseSegColorHex(const char* in,
                                               uint8_t& R, uint8_t& G,
                                               uint8_t& B, uint8_t& WW,
                                               uint8_t& CW)
{
  if (!in) return false;

  // 1) copy only hex chars (ignore '#', spaces, commas, etc.)
  char hex[16]; // enough for 10 hex digits (+NUL) = 5 channels * 2
  size_t n = 0;
  for (const char* p = in; *p && n < sizeof(hex)-1; ++p) {
    char c = *p;
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F')) {
      hex[n++] = (char)tolower(c);
    }
  }
  hex[n] = '\0';
  if (n == 0) return false;

  // special case: single char "0" means full black
  if (n == 1 && hex[0] == '0') {
    R = G = B = WW = CW = 0;
    return true;
  }

  auto hex1 = [&](size_t i)->uint8_t {
    char c = hex[i];
    return (uint8_t)((c <= '9') ? (c - '0') : (c - 'a' + 10));
  };
  auto hex2 = [&](size_t i)->uint8_t {
    return (uint8_t)((hex1(i) << 4) | hex1(i+1));
  };
  auto dup = [&](uint8_t h)->uint8_t { return (uint8_t)((h << 4) | h); };

  R = G = B = WW = CW = 0;

  if (n & 1) {
    // ODD: short form, 1 nibble per channel: RGBWC
    if (n >= 1) R  = dup(hex1(0));
    if (n >= 2) G  = dup(hex1(1));
    if (n >= 3) B  = dup(hex1(2));
    if (n >= 4) WW = dup(hex1(3));
    if (n >= 5) CW = dup(hex1(4));
  } else {
    // EVEN: long form, 2 nibbles per channel: RR GG BB WW CW
    if (n >= 2)  R  = hex2(0);
    if (n >= 4)  G  = hex2(2);
    if (n >= 6)  B  = hex2(4);
    if (n >= 8)  WW = hex2(6);
    if (n >= 10) CW = hex2(8);
  }
  return true;
}


bool mAnimatorLight::extractModeDefaults(uint16_t mode, const char* segVar, char* outBuffer, size_t bufferSize)
{
  if (mode >= getEffectCount() || !segVar || !outBuffer || bufferSize == 0) return false;

  const char* config = getModeData_Config(mode);
  if (!config) return false;

  const size_t config_length = strlen_P(config);
  constexpr size_t CONFIG_BUFFER_PADDING = 8u;
  const size_t line_buffer_size = config_length + 1u + CONFIG_BUFFER_PADDING;

  char* lineBuffer = static_cast<char*>(malloc(line_buffer_size));
  if (!lineBuffer) return false;

  memcpy_P(lineBuffer, config, config_length);
  lineBuffer[config_length] = '\0';

  char* startPtr = strrchr(lineBuffer, ';');
  if (!startPtr)
  {
    free(lineBuffer);
    return false;
  }

  const size_t key_length = strlen(segVar);
  char* searchPtr = startPtr + 1;

  while (*searchPtr)
  {
    char* keyPtr = strstr(searchPtr, segVar);
    if (!keyPtr)
    {
      free(lineBuffer);
      return false;
    }

    const bool valid_key_start = keyPtr == startPtr + 1 || *(keyPtr - 1) == ',';
    const bool valid_key_end = keyPtr[key_length] == '=';

    if (valid_key_start && valid_key_end)
    {
      char* valuePtr = keyPtr + key_length + 1u;
      char* valueEnd = strchr(valuePtr, ',');

      if (!valueEnd) valueEnd = valuePtr + strlen(valuePtr);

      const size_t value_length = static_cast<size_t>(valueEnd - valuePtr);

      if (value_length >= bufferSize)
      {
        free(lineBuffer);
        return false;
      }

      memcpy(outBuffer, valuePtr, value_length);
      outBuffer[value_length] = '\0';

      free(lineBuffer);
      return true;
    }

    searchPtr = keyPtr + key_length;
  }

  free(lineBuffer);
  return false;
}


int16_t mAnimatorLight::extractModeDefaults(uint16_t mode, const char* segVar)
{
  if (mode >= getEffectCount() || !segVar) return -1;

  const char* config = getModeData_Config(mode);
  if (!config) return -1;

  const size_t config_length = strlen_P(config);
  constexpr size_t CONFIG_BUFFER_PADDING = 8u;
  const size_t line_buffer_size = config_length + 1u + CONFIG_BUFFER_PADDING;

  char* lineBuffer = static_cast<char*>(malloc(line_buffer_size));
  if (!lineBuffer) return -1;

  memcpy_P(lineBuffer, config, config_length);
  lineBuffer[config_length] = '\0';

  char* startPtr = strrchr(lineBuffer, ';');
  if (!startPtr)
  {
    free(lineBuffer);
    return -1;
  }

  const size_t key_length = strlen(segVar);
  char* searchPtr = startPtr + 1;

  while (*searchPtr)
  {
    char* keyPtr = strstr(searchPtr, segVar);
    if (!keyPtr)
    {
      free(lineBuffer);
      return -1;
    }

    const bool valid_key_start = keyPtr == startPtr + 1 || *(keyPtr - 1) == ',';
    const bool valid_key_end = keyPtr[key_length] == '=';

    if (valid_key_start && valid_key_end)
    {
      char* valuePtr = keyPtr + key_length + 1u;
      char* valueEnd = nullptr;

      const long value = strtol(valuePtr, &valueEnd, 10);

      if (valueEnd == valuePtr || (*valueEnd != ',' && *valueEnd != '\0') || value < INT16_MIN || value > INT16_MAX)
      {
        free(lineBuffer);
        return -1;
      }

      free(lineBuffer);
      return static_cast<int16_t>(value);
    }

    searchPtr = keyPtr + key_length;
  }

  free(lineBuffer);
  return -1;
}


#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS

// 2D matrix
uint16_t mAnimatorLight::Segment::virtualWidth() const {
  const uint16_t groupLen = groupLength();

  uint16_t vWidth =
    static_cast<uint16_t>(
      ((transpose ? height() : width()) + groupLen - 1u) / groupLen
    );

  if (mirror)
  {
    vWidth = static_cast<uint16_t>((vWidth + 1u) / 2u);
  }

  if (decimate > 1u)
  {
    vWidth = static_cast<uint16_t>((vWidth + decimate - 1u) / decimate);
  }

  return vWidth;
}

uint16_t mAnimatorLight::Segment::virtualHeight() const {
  const uint16_t groupLen = groupLength();

  uint16_t vHeight =
    static_cast<uint16_t>(
      ((transpose ? width() : height()) + groupLen - 1u) / groupLen
    );

  if (mirror_y)
  {
    vHeight = static_cast<uint16_t>((vHeight + 1u) / 2u);
  }

  if (decimate > 1u)
  {
    vHeight = static_cast<uint16_t>((vHeight + decimate - 1u) / decimate);
  }

  return vHeight;
}

#else

// 2D matrix
uint16_t mAnimatorLight::Segment::virtualWidth() const {
  // 
  uint16_t groupLen = groupLength();
  uint16_t vWidth = ((transpose ? height() : width()) + groupLen - 1) / groupLen;
  // ALOG_INF(PSTR("virtualWidth() %d"), vWidth);
  if (mirror) vWidth = (vWidth + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  // 
  return vWidth;
}

uint16_t mAnimatorLight::Segment::virtualHeight() const {
  // 
  // ALOG_INF(PSTR("height %d"), height());
  uint16_t groupLen = groupLength();
  uint16_t vHeight = ((transpose ? width() : height()) + groupLen - 1) / groupLen;
  // ALOG_INF(PSTR("virtualHeight() %d"), vHeight);
  if (mirror_y) vHeight = (vHeight + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  // 
  return vHeight;
}

#endif


// Constants for mapping mode "Pinwheel"
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
// constexpr int Pinwheel_Steps_Small = 72;       // no holes up to 16x16
// constexpr int Pinwheel_Size_Small  = 16;       // larger than this -> use "Medium"
// constexpr int Pinwheel_Steps_Medium = 192;     // no holes up to 32x32
// constexpr int Pinwheel_Size_Medium  = 32;      // larger than this -> use "Big"
// constexpr int Pinwheel_Steps_Big = 304;        // no holes up to 50x50
// constexpr int Pinwheel_Size_Big  = 50;         // larger than this -> use "XL"
// constexpr int Pinwheel_Steps_XL  = 368;
// constexpr float Int_to_Rad_Small = (DEG_TO_RAD * 360) / Pinwheel_Steps_Small;  // conversion: from 0...72 to Radians
// constexpr float Int_to_Rad_Med =   (DEG_TO_RAD * 360) / Pinwheel_Steps_Medium; // conversion: from 0...192 to Radians
// constexpr float Int_to_Rad_Big =   (DEG_TO_RAD * 360) / Pinwheel_Steps_Big;    // conversion: from 0...304 to Radians
// constexpr float Int_to_Rad_XL =    (DEG_TO_RAD * 360) / Pinwheel_Steps_XL;     // conversion: from 0...368 to Radians

// constexpr int Fixed_Scale = 512;               // fixpoint scaling factor (9bit for fraction)

// // Pinwheel helper function: pixel index to radians
// static float getPinwheelAngle(int i, int vW, int vH) {
//   int maxXY = max(vW, vH);
//   if (maxXY <= Pinwheel_Size_Small)  return float(i) * Int_to_Rad_Small;
//   if (maxXY <= Pinwheel_Size_Medium) return float(i) * Int_to_Rad_Med;
//   if (maxXY <= Pinwheel_Size_Big)    return float(i) * Int_to_Rad_Big;
//   // else
//   return float(i) * Int_to_Rad_XL;
// }
// // Pinwheel helper function: matrix dimensions to number of rays
// static int getPinwheelLength(int vW, int vH) {
//   int maxXY = max(vW, vH);
//   if (maxXY <= Pinwheel_Size_Small)  return Pinwheel_Steps_Small;
//   if (maxXY <= Pinwheel_Size_Medium) return Pinwheel_Steps_Medium;
//   if (maxXY <= Pinwheel_Size_Big)    return Pinwheel_Steps_Big;
//   // else
//   return Pinwheel_Steps_XL;
// }


constexpr int Fixed_Scale = 16384; // fixpoint scaling factor (14bit for fraction)
// Pinwheel helper function: matrix dimensions to number of rays
static int getPinwheelLength(int vW, int vH) {
  // Returns multiple of 8, prevents over drawing
  return (max(vW, vH) + 15) & ~7;
}
static void setPinwheelParameters(int i, int vW, int vH, int& startx, int& starty, int* cosVal, int* sinVal, bool getPixel = false) {
  int steps = getPinwheelLength(vW, vH);
  int baseAngle = ((0xFFFF + steps / 2) / steps);  // 360° / steps, in 16 bit scale round to nearest integer
  int rotate = 0;
  if (getPixel) rotate = baseAngle / 2; // rotate by half a ray width when reading pixel color
  for (int k = 0; k < 2; k++) // angular steps for two consecutive rays
  {
    int angle = (i + k) * baseAngle + rotate;
    cosVal[k] = (cos16_t(angle) * Fixed_Scale) >> 15; // step per pixel in fixed point, cos16 output is -0x7FFF to +0x7FFF
    sinVal[k] = (sin16_t(angle) * Fixed_Scale) >> 15; // using explicit bit shifts as dividing negative numbers is not equivalent (rounding error is acceptable)
  }
  startx = (vW * Fixed_Scale) / 2; // + cosVal[0] / 4; // starting position = center + 1/4 pixel (in fixed point)
  starty = (vH * Fixed_Scale) / 2; // + sinVal[0] / 4;
}


#endif


#ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS

// 1D strip
uint16_t mAnimatorLight::Segment::virtualLength() const {
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    unsigned vW = virtualWidth();
    unsigned vH = virtualHeight();
    unsigned vLen;
    switch (map1D2D) {
      case M12_pBar:
        vLen = vH;
        break;
      case M12_pCorner:
        vLen = max(vW,vH); // get the longest dimension
        break;
      case M12_pArc:
        vLen = sqrt32_bw(vH*vH + vW*vW); // use diagonal
        break;
      case M12_sPinwheel:
        vLen = getPinwheelLength(vW, vH);
        break;
      default:
        vLen = vW * vH; // use all pixels from segment
        break;
    }
    return vLen;
  }
#endif
  unsigned groupLen = groupLength(); // is always >= 1
  unsigned vLength = (length() + groupLen - 1) / groupLen;
  if (mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED

  #ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
  if (decimate > 1u){
    vLength = (vLength + decimate - 1u) / decimate;
  }
  #endif
  
  return vLength;
}

#else


// 1D strip
uint16_t mAnimatorLight::Segment::virtualLength() const {

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D())
  {
    const unsigned vW = virtualWidth();
    const unsigned vH = virtualHeight();

    switch (map1D2D)
    {
      case M12_pBar:
        return vH;

      case M12_pCorner:
        return max(vW, vH);

      case M12_pArc:
        return sqrt32_bw(vH * vH + vW * vW);

      case M12_sPinwheel:
        return getPinwheelLength(vW, vH);

      default:
        return vW * vH;
    }
  }
  #endif

  const unsigned groupLen = groupLength();
  unsigned vLength = (length() + groupLen - 1u) / groupLen;

  if (mirror)
  {
    vLength = (vLength + 1u) / 2u;
  }

  if (decimate > 1u)
  {
    vLength = (vLength + decimate - 1u) / decimate;
  }

  return static_cast<uint16_t>(vLength);
}

#endif


#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
// maximum length of a mapped 1D segment, used in PS for buffer allocation
uint16_t mAnimatorLight::Segment::maxMappingLength() const {
  uint32_t vW = virtualWidth();
  uint32_t vH = virtualHeight();
  return max(sqrt32_bw(vH*vH + vW*vW), (uint32_t)getPinwheelLength(vW, vH)); // use diagonal
}
#endif

/**
 * @brief Set a pixel using a *normalized* floating position with optional anti-aliasing,
 *        while also encoding a “virtual strip” index into the same argument.
 *
 * HOW IT WORKS (two tricks in one):
 * 1) **Virtual-strip encoding in the integer part**  
 *    Callers pass `i` as `i = (stripNr+1)*10.0f + frac`, where:
 *      - `(stripNr+1)*10.0f` is the **integer part** and encodes which virtual strip/column this
 *        1D effect should draw on in a 2D segment. (e.g., 10.0 → strip 0, 20.0 → strip 1, …)
 *      - `frac` is the **fractional part** in [0..1], the normalized position along that strip.
 *    Inside this function we recover `vStrip = int(i/10.0f)` and then remove the integer part so
 *    that `i` only contains the fractional [0..1] position.  
 *    Later, we pack `vStrip` back into the upper 16 bits when calling the integer
 *    `setPixelColor(index | (vStrip<<16), col)`, matching WLED’s virtual-strip convention.
 *
 * 2) **Anti-aliasing (sub-pixel blending)**  
 *    When `aa == true`, we compute the exact float location `fC` along the strip and blend the
 *    color between the two nearest pixel centers. This reduces “stepping” and produces smooth
 *    motion of dots/lines at sub-pixel positions:
 *      - Find `iL` (left pixel) and `iR` (right pixel) around `fC`.
 *      - Compute weights based on squared distance to each center (simple smooth falloff).
 *      - Blend your input color with the current pixel colors at `iL` and `iR`, then write back.
 *
 * IMPORTANT NOTES:
 * - The “×10” you noticed is **not** for random smearing. It’s purely a compact channel to carry
 *   the virtual-strip index in the integer part of the float. The *fractional* part carries the
 *   normalized [0..1] position used for anti-aliasing.
 * - This relies on the companion integer overload of `setPixelColor(int, uint32_t)` to *unpack*
 *   `vStrip` from the upper 16 bits (i.e., `int vStrip = i >> 16; i &= 0xFFFF;`) and route the
 *   pixel to the correct 2D coordinate.
 * - `virtualLength()` is used as the logical length of the strip dimension this 1D effect draws on.
 */
#if defined(ENABLE_ANTIALIAS_WITH_RGBWW)
void mAnimatorLight::Segment::setPixelColor(float i, uint32_t col, bool aa)
{
  // Bail if segment is inactive.
  if (!isActive()) return;

  // --- Extract virtual-strip index from the integer part of the float --------------------------
  // The caller encodes vStrip as (stripNr+1)*10.0f added to the normalized position.
  // Example: 23.42f  -> vStrip = 2, fractional position = 0.342
  int vStrip = int(i / 10.0f);

  // Remove the integer part: keep only [0..1] normalized position along the strip.
  i -= int(i);

  // Position must be normalized; out-of-range → ignore.
  if (i < 0.0f || i > 1.0f) return;

  // Convert normalized [0..1] to float index in [0..virtualLength()-1].
  const float fC = i * (virtualLength() - 1);

  if (aa)
  {
    // ---- Anti-aliased write: blend between the two neighboring pixel centers -----------------
    // We define a small window around fC and pick the “left” and “right” integer indices.
    // The 0.49 offset gives a symmetric region around each pixel center for smoother blending.
    const uint16_t iL = roundf(fC - 0.49f);
    const uint16_t iR = roundf(fC + 0.49f);

    // Squared distance from fC to each neighbor; used as blend weights (soft falloff).
    const float dL = (fC - iL) * (fC - iL);
    const float dR = (iR - fC) * (iR - fC);

    // Fetch current colors at the neighbors (with the vStrip encoded in upper 16 bits).
    uint32_t cIL = getPixelColor(iL | (vStrip << 16));
    uint32_t cIR = getPixelColor(iR | (vStrip << 16));

    if (iR != iL)
    {
      // Left pixel: blend the new color into the existing pixel by a factor from distance.
      // Larger distance → higher alpha → less influence (simple, fast AA).
      cIL = color_blend(col, cIL, uint8_t(dL * 255.0f));
      setPixelColor(iL | (vStrip << 16), cIL);

      // Right pixel: same, with its own distance weight.
      cIR = color_blend(col, cIR, uint8_t(dR * 255.0f));
      setPixelColor(iR | (vStrip << 16), cIR);
    }
    else
    {
      // Degenerate case: fC lands exactly on a pixel center (or both round to same index).
      setPixelColor(iL | (vStrip << 16), col);
    }
  }
  else
  {
    // ---- Nearest-neighbor write: no AA, just pick the closest pixel --------------------------
    const uint16_t idx = uint16_t(roundf(fC));
    setPixelColor(idx | (vStrip << 16), col);
  }
}
#endif


#if defined(ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_DEV) || defined(ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_ALPHA)

// Reorder effects by development stage, promoting a chosen stage to the front.
// - promote_first: the Effect_DevStage to bring to the front (e.g., Effect_DevStage::Dev).
//   All entries with this stage keep their relative order (stable).
// - The remaining entries are ordered by their stage value (Release=0 … Unstable=4), stable within ties.
void mAnimatorLight::sortEffects(Effect_DevStage promote_first)
{
  const size_t n = effects.id.size();
  if (!n) { effects.count = 0; return; }

  // Sanity: all parallel vectors must match length
  if (effects.function.size() != n ||
      effects.config.size()   != n ||
      effects.development_stage.size() != n
  #ifdef ENABLE_EFFECT_DESCRIPTIONS
      || effects.description.size() != n
  #endif
  ) {
    effects.count = (uint16_t)effects.id.size();
    return;
  }

  // Indirection index [0..n-1]
  std::vector<size_t> idx;
  idx.reserve(n);
  for (size_t i = 0; i < n; ++i) idx.push_back(i);

  // Priority: promoted stage gets -1, others keep their enum value (0..4).
  // Using stable_sort => preserves insertion order within same priority.
  std::stable_sort(idx.begin(), idx.end(),
    [&](size_t a, size_t b)
    {
      const uint8_t sa = effects.development_stage[a];
      const uint8_t sb = effects.development_stage[b];
      const int pa = (sa == (uint8_t)promote_first) ? -1 : (int)sa;
      const int pb = (sb == (uint8_t)promote_first) ? -1 : (int)sb;
      if (pa != pb) return pa < pb;
      return false; // keep original order when priorities are equal
    });

  // Reorder all parallel arrays explicitly
  std::vector<uint16_t>        id_new;        id_new.reserve(n);
  std::vector<EffectFunction> fn_new;        fn_new.reserve(n);
  std::vector<const char*>    cfg_new;       cfg_new.reserve(n);
  std::vector<uint8_t>        stage_new;     stage_new.reserve(n);
#ifdef ENABLE_EFFECT_DESCRIPTIONS
  std::vector<const char*>    desc_new;      desc_new.reserve(n);
#endif

  for (size_t k = 0; k < n; ++k) {
    const size_t i = idx[k];
    id_new.push_back(effects.id[i]);
    fn_new.push_back(effects.function[i]);
    cfg_new.push_back(effects.config[i]);
    stage_new.push_back(effects.development_stage[i]);
  #ifdef ENABLE_EFFECT_DESCRIPTIONS
    desc_new.push_back(effects.description[i]);
  #endif
  }

  effects.id.swap(id_new);
  effects.function.swap(fn_new);
  effects.config.swap(cfg_new);
  effects.development_stage.swap(stage_new);
#ifdef ENABLE_EFFECT_DESCRIPTIONS
  effects.description.swap(desc_new);
#endif

  effects.count = (uint16_t)effects.id.size();
}
#endif



uint32_t WLED_O2_ATTR mAnimatorLight::Segment::getPixelColor(int i) const
{
  if (!isActive() || i < 0) return 0; // not active or invalid index

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  int vStrip = i>>16; // virtual strips are only relevant in Bar expansion mode
  i &= 0xFFFF;
#endif
  if (i >= (int)vLength()) return 0;

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
    int x = 0, y = 0;
    switch (map1D2D) {
      case M12_Pixels:
        x = i % vW;
        y = i / vW;
        break;
      case M12_pBar:
        if (vStrip > 0) { x = vStrip - 1; y = vH - i - 1; }
        else            { y = vH - i - 1; };
        break;
      case M12_pArc:
        if (i > vW && i > vH) {
          x = y = sqrt32_bw(i*i/2);
          break; // use diagonal
        }
        // otherwise fallthrough
      case M12_pCorner:
        // use longest dimension
        if (vW > vH) x = i;
        else         y = i;
        break;
      case M12_sPinwheel: {
        // not 100% accurate, returns pixel at outer edge
        int cosVal[2], sinVal[2];
        setPinwheelParameters(i, vW, vH, x, y, cosVal, sinVal, true);
        int maxX = (vW-1) * Fixed_Scale;
        int maxY = (vH-1) * Fixed_Scale;
        // trace ray from center until we hit any edge - to avoid rounding problems, we use fixed point coordinates
        while ((x < maxX)  && (y < maxY) && (x > Fixed_Scale) && (y > Fixed_Scale)) {
          x += cosVal[0]; // advance to next position
          y += sinVal[0];
        }
        x /= Fixed_Scale;
        y /= Fixed_Scale;
        break;
      }
    }
    return getPixelColorXY(x, y);
  }
#endif
  return getPixelColorRaw(i);
}



// Blends the specified color with the existing pixel color.
void mAnimatorLight::Segment::blendPixelColor(int n, uint32_t color, uint8_t blend) {
  setPixelColor(n, color_blend(getPixelColor(n), color, blend));
}

// Adds the specified color with the existing pixel color perserving color balance.
void mAnimatorLight::Segment::addPixelColor(int n, uint32_t color) {
  setPixelColor(n, color_add(getPixelColor(n), color));
}


uint8_t mAnimatorLight::Segment::differs(const Segment& b) const {
  uint8_t d = 0;
  if (start != b.start)         d |= SEG_DIFFERS_BOUNDS;
  if (stop != b.stop)           d |= SEG_DIFFERS_BOUNDS;
  if (offset != b.offset)       d |= SEG_DIFFERS_GSO;
  if (grouping != b.grouping)   d |= SEG_DIFFERS_GSO;
  if (spacing != b.spacing)     d |= SEG_DIFFERS_GSO;
  // if (opacity != b.opacity)     d |= SEG_DIFFERS_BRI;
  // if (mode != b.mode)           d |= SEG_DIFFERS_FX;
  if (speed != b.speed)         d |= SEG_DIFFERS_FX;
  if (intensity != b.intensity) d |= SEG_DIFFERS_FX;
  // if (palette != b.palette)     d |= SEG_DIFFERS_FX;
  if (custom1 != b.custom1)     d |= SEG_DIFFERS_FX;
  if (custom2 != b.custom2)     d |= SEG_DIFFERS_FX;
  if (custom3 != b.custom3)     d |= SEG_DIFFERS_FX;
  if (startY != b.startY)       d |= SEG_DIFFERS_BOUNDS;
  if (stopY != b.stopY)         d |= SEG_DIFFERS_BOUNDS;

  //bit pattern: (msb first)
  // set:2, sound:2, mapping:3, transposed, mirrorY, reverseY, [reset,] paused, mirrored, on, reverse, [selected]
  if ((options & 0b1111111111011110U) != (b.options & 0b1111111111011110U)) d |= SEG_DIFFERS_OPT;
  if ((options & 0x0001U) != (b.options & 0x0001U))                         d |= SEG_DIFFERS_SEL;
  // for (unsigned i = 0; i < NUMBER_SEGMENT_COLOURS; i++) if (colors[i] != b.colors[i])   d |= SEG_DIFFERS_COL;

  return d;
}

// void mAnimatorLight::Segment::refreshLightCapabilities() {
//   unsigned capabilities = 0;
//   unsigned segStartIdx = 0xFFFFU;
//   unsigned segStopIdx  = 0;

//   if (!isActive()) {
//     _capabilities = 0;
//     return;
//   }

//   if (start < Segment::maxWidth * Segment::maxHeight) {
//     // we are withing 2D matrix (includes 1D segments)
//     for (int y = startY; y < stopY; y++) for (int x = start; x < stop; x++) {
//       unsigned index = tkr_anim->getMappedPixelIndex(x + Segment::maxWidth * y); // convert logical address to physical
//       if (index < 0xFFFFU) {
//         if (segStartIdx > index) segStartIdx = index;
//         if (segStopIdx  < index) segStopIdx  = index;
//       }
//       if (segStartIdx == segStopIdx) segStopIdx++; // we only have 1 pixel segment
//     }
//   } else {
//     // we are on the strip located after the matrix
//     segStartIdx = start;
//     segStopIdx  = stop;
//   }

//   for (unsigned b = 0; b < BusManager::getNumBusses(); b++) {
//     Bus *bus = BusManager::getBus(b);
//     if (bus == nullptr || bus->getLength()==0) break;
//     if (!bus->isOk()) continue;
//     if (bus->getStart() >= segStopIdx) continue;
//     if (bus->getStart() + bus->getLength() <= segStartIdx) continue;

//     if (bus->hasRGB() || (tkr_anim->cctFromRgb && bus->hasCCT())) capabilities |= SEG_CAPABILITY_RGB;
//     if (!tkr_anim->cctFromRgb && bus->hasCCT())                   capabilities |= SEG_CAPABILITY_CCT;
//     if (tkr_anim->correctWB && (bus->hasRGB() || bus->hasCCT()))  capabilities |= SEG_CAPABILITY_CCT; //white balance correction (CCT slider)
//     if (bus->hasWhite()) {
//       unsigned aWM = Bus::getGlobalAWMode() == AW_GLOBAL_DISABLED ? bus->getAutoWhiteMode() : Bus::getGlobalAWMode();
//       bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
//       // if auto white calculation from RGB is active (Accurate/Brighter), force RGB controls even if there are no RGB busses
//       if (!whiteSlider) capabilities |= SEG_CAPABILITY_RGB;
//       // if auto white calculation from RGB is disabled/optional (None/Dual), allow white channel adjustments
//       if ( whiteSlider) capabilities |= SEG_CAPABILITY_W;
//     }
//   }
//   _capabilities = capabilities;
// }


void mAnimatorLight::Segment::refreshLightCapabilities() const {
  unsigned capabilities = 0;

  if (!isActive()) {
    // _capabilities = 0;
    return;
  }

  // we must traverse each pixel in segment to determine its capabilities (as pixel may be mapped)
  for (unsigned y = startY; y < stopY; y++) for (unsigned x = start; x < stop; x++) {
    unsigned index = x + Segment::maxWidth * y;
    index = tkr_anim->getMappedPixelIndex(index); // convert logical address to physical
    if (index == 0xFFFF) continue;  // invalid/missing  pixel
    for (unsigned b = 0; b < BusManager::getNumBusses(); b++) {
      const Bus *bus = BusManager::getBus(b);
      if (!bus || !bus->isOk()) break;
      if (bus->containsPixel(index)) {
        if (bus->hasRGB() || (tkr_anim->cctFromRgb && bus->hasCCT())) capabilities |= SEG_CAPABILITY_RGB;
        if (!tkr_anim->cctFromRgb && bus->hasCCT())                   capabilities |= SEG_CAPABILITY_CCT;
        if (tkr_anim->correctWB && (bus->hasRGB() || bus->hasCCT()))  capabilities |= SEG_CAPABILITY_CCT; //white balance correction (CCT slider)
        if (bus->hasWhite()) {
          unsigned aWM = Bus::getGlobalAWMode() == AW_GLOBAL_DISABLED ? bus->getAutoWhiteMode() : Bus::getGlobalAWMode();
          bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
          // if auto white calculation from RGB is active (Accurate/Brighter), force RGB controls even if there are no RGB busses
          if (!whiteSlider) capabilities |= SEG_CAPABILITY_RGB;
          // if auto white calculation from RGB is disabled/optional (None/Dual), allow white channel adjustments
          if ( whiteSlider) capabilities |= SEG_CAPABILITY_W;
        }
        break;
      }
    }
  }
  // _capabilities = capabilities;
}

/*
 * Fills segment with color
 */
// void mAnimatorLight::Segment::fill(uint32_t c) {
//   if (!isActive()) return; // not active
//   const int cols = is2D() ? vWidth() : vLength();
//   const int rows = vHeight(); // will be 1 for 1D
//   // pre-scale color for all pixels
//   c = color_fade(c, _brightness_rgb);//_segBri);
//   _colorScaled = true;
//   for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
//     if (is2D()) setPixelColorXY(x, y, c);
//     else        setPixelColor(x, c);
//   }
//   _colorScaled = false;
// }


// void mAnimatorLight::Segment::fill(RgbwwColor c)//, bool apply_brightness) 
// {
//   for(uint16_t i = 0; i < length(); i++) 
//   {
//     setPixelColor(i, c);//, apply_brightness);
//   }
// }


void mAnimatorLight::Segment::fill_ranged(uint32_t c)//, bool apply_brightness) 
{

  for(uint16_t i = start; i <= stop; i++) {
    setPixelColor(i, c);//, apply_brightness);
  }

}

/*
 * Fills segment with color
 */
void mAnimatorLight::Segment::fill(uint32_t c) const {
  if (!isActive()) return; // not active
  for (unsigned i = 0; i < length(); i++) setPixelColorRaw(i,c); // always fill all pixels (blending will take care of grouping, spacing and clipping)
}

void mAnimatorLight::Segment::fade_out(uint8_t rate) {
  if (!isActive()) return; // not active
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D

  rate = (255-rate) >> 1;
  float mappedRate = 1.0f / (float(rate) + 1.1f);

  RgbwwColor color1 = segcol[1].WithBrightness();//.getU32(); // SEGCOLOR(1); // target color
  uint32_t color = RGBW32(color1.R,color1.G,color1.B,color1.WW);//segcol[1].WithBrightness();//.getU32(); // SEGCOLOR(1); // target color
  uint32_t color_check = color;
  int w2 = W(color);
  int r2 = R(color);
  int g2 = G(color);
  int b2 = B(color);

  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
    if (color == color_check) continue; // already at target color
    int w1 = W(color);
    int r1 = R(color);
    int g1 = G(color);
    int b1 = B(color);

    int wdelta = (w2 - w1) * mappedRate;
    int rdelta = (r2 - r1) * mappedRate;
    int gdelta = (g2 - g1) * mappedRate;
    int bdelta = (b2 - b1) * mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
    else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  }
}


// fades all pixels to black using nscale8()
void mAnimatorLight::Segment::fadeToBlackBy(uint8_t fadeBy) {
  if (!isActive() || fadeBy == 0) return;   // optimization - no scaling to apply
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D

  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    if (is2D()) setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), 255-fadeBy));
    else        setPixelColor(x, color_fade(getPixelColor(x), 255-fadeBy));
  }
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 * Note: for blur_amount > 215 this function does not work properly (creates alternating pattern)
 */
void mAnimatorLight::Segment::blur(uint8_t blur_amount, bool smear) {
  if (!isActive() || blur_amount == 0) return; // optimization: 0 means "don't blur"
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // compatibility with 2D
    blur2D(blur_amount, blur_amount, smear); // symmetrical 2D blur
    //box_blur(map(blur_amount,1,255,1,3), smear);
    #endif
    return;
  }
#endif
  uint8_t keep = smear ? 255 : 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  unsigned vlength = vLength();
  uint32_t carryover = BLACK;
  uint32_t lastnew;
  uint32_t last;
  uint32_t curnew = BLACK;
  for (unsigned i = 0; i < vlength; i++) {
    uint32_t cur = getPixelColor(i);
    uint32_t part = color_fade(cur, seep);
    curnew = color_fade(cur, keep);
    if (i > 0) {
      if (carryover) curnew = color_add(curnew, carryover);
      uint32_t prev = color_add(lastnew, part);
      // optimization: only set pixel if color has changed
      if (last != prev) setPixelColor((int)i - 1, prev);
    } else setPixelColor((int)i, curnew); // first pixel
    lastnew = curnew;
    last = cur; // save original value for comparison on next iteration
    carryover = part;
  }
  setPixelColor((int)vlength - 1, curnew);
}


void mAnimatorLight::Segment::setRandomColor(byte* rgb)
{
  // lastRandomIndex = tkr_anim->getMainSegment().get_random_wheel_index(lastRandomIndex);
  // colorHStoRGB(lastRandomIndex*256,255,rgb);
}

void mAnimatorLight::Segment::colorHStoRGB(uint16_t hue, byte sat, byte* rgb) //hue, sat to rgb
{
  float h = ((float)hue)/65535.0f;
  float s = ((float)sat)/255.0f;
  int   i = floorf(h*6);
  float f = h * 6.0f - i;
  int   p = int(255.0f * (1.0f-s));
  int   q = int(255.0f * (1.0f-f*s));
  int   t = int(255.0f * (1.0f-(1.0f-f)*s));
  p = constrain(p, 0, 255);
  q = constrain(q, 0, 255);
  t = constrain(t, 0, 255);
  switch (i%6) {
    case 0: rgb[0]=255,rgb[1]=t,  rgb[2]=p;  break;
    case 1: rgb[0]=q,  rgb[1]=255,rgb[2]=p;  break;
    case 2: rgb[0]=p,  rgb[1]=255,rgb[2]=t;  break;
    case 3: rgb[0]=p,  rgb[1]=q,  rgb[2]=255;break;
    case 4: rgb[0]=t,  rgb[1]=p,  rgb[2]=255;break;
    case 5: rgb[0]=255,rgb[1]=p,  rgb[2]=q;  break;
  }
}

//get RGB values from color temperature in K (https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html)
void mAnimatorLight::Segment::colorKtoRGB(uint16_t kelvin, byte* rgb) //white spectrum to rgb, calc
{
  int r = 0, g = 0, b = 0;
  float temp = kelvin / 100.0f;
  if (temp <= 66.0f) {
    r = 255;
    g = roundf(99.4708025861f * logf(temp) - 161.1195681661f);
    if (temp <= 19.0f) {
      b = 0;
    } else {
      b = roundf(138.5177312231f * logf((temp - 10.0f)) - 305.0447927307f);
    }
  } else {
    r = roundf(329.698727446f * powf((temp - 60.0f), -0.1332047592f));
    g = roundf(288.1221695283f * powf((temp - 60.0f), -0.0755148492f));
    b = 255;
  }
  //g += 12; //mod by Aircoookie, a bit less accurate but visibly less pinkish
  rgb[0] = (uint8_t) constrain(r, 0, 255);
  rgb[1] = (uint8_t) constrain(g, 0, 255);
  rgb[2] = (uint8_t) constrain(b, 0, 255);
  rgb[3] = 0;
}

void mAnimatorLight::Segment::colorCTtoRGB(uint16_t mired, byte* rgb) //white spectrum to rgb, bins
{
  //this is only an approximation using WS2812B with gamma correction enabled
  if (mired > 475) {
    rgb[0]=255;rgb[1]=199;rgb[2]=92;//500
  } else if (mired > 425) {
    rgb[0]=255;rgb[1]=213;rgb[2]=118;//450
  } else if (mired > 375) {
    rgb[0]=255;rgb[1]=216;rgb[2]=118;//400
  } else if (mired > 325) {
    rgb[0]=255;rgb[1]=234;rgb[2]=140;//350
  } else if (mired > 275) {
    rgb[0]=255;rgb[1]=243;rgb[2]=160;//300
  } else if (mired > 225) {
    rgb[0]=250;rgb[1]=255;rgb[2]=188;//250
  } else if (mired > 175) {
    rgb[0]=247;rgb[1]=255;rgb[2]=215;//200
  } else {
    rgb[0]=237;rgb[1]=255;rgb[2]=239;//150
  }
}

// #ifndef WLED_DISABLE_HUESYNC
// void mAnimatorLight::Segment::colorXYtoRGB(float x, float y, byte* rgb) //coordinates to rgb (https://www.developers.meethue.com/documentation/color-conversions-rgb-xy)
// {
//   float z = 1.0f - x - y;
//   float X = (1.0f / y) * x;
//   float Z = (1.0f / y) * z;
//   float r = (int)255*(X * 1.656492f - 0.354851f - Z * 0.255038f);
//   float g = (int)255*(-X * 0.707196f + 1.655397f + Z * 0.036152f);
//   float b = (int)255*(X * 0.051713f - 0.121364f + Z * 1.011530f);
//   if (r > b && r > g && r > 1.0f) {
//     // red is too big
//     g = g / r;
//     b = b / r;
//     r = 1.0f;
//   } else if (g > b && g > r && g > 1.0f) {
//     // green is too big
//     r = r / g;
//     b = b / g;
//     g = 1.0f;
//   } else if (b > r && b > g && b > 1.0f) {
//     // blue is too big
//     r = r / b;
//     g = g / b;
//     b = 1.0f;
//   }
//   // Apply gamma correction
//   r = r <= 0.0031308f ? 12.92f * r : (1.0f + 0.055f) * powf(r, (1.0f / 2.4f)) - 0.055f;
//   g = g <= 0.0031308f ? 12.92f * g : (1.0f + 0.055f) * powf(g, (1.0f / 2.4f)) - 0.055f;
//   b = b <= 0.0031308f ? 12.92f * b : (1.0f + 0.055f) * powf(b, (1.0f / 2.4f)) - 0.055f;

//   if (r > b && r > g) {
//     // red is biggest
//     if (r > 1.0f) {
//       g = g / r;
//       b = b / r;
//       r = 1.0f;
//     }
//   } else if (g > b && g > r) {
//     // green is biggest
//     if (g > 1.0f) {
//       r = r / g;
//       b = b / g;
//       g = 1.0f;
//     }
//   } else if (b > r && b > g) {
//     // blue is biggest
//     if (b > 1.0f) {
//       r = r / b;
//       g = g / b;
//       b = 1.0f;
//     }
//   }
//   rgb[0] = byte(255.0f*r);
//   rgb[1] = byte(255.0f*g);
//   rgb[2] = byte(255.0f*b);
// }

// void mAnimatorLight::Segment::colorRGBtoXY(byte* rgb, float* xy) //rgb to coordinates (https://www.developers.meethue.com/documentation/color-conversions-rgb-xy)
// {
//   float X = rgb[0] * 0.664511f + rgb[1] * 0.154324f + rgb[2] * 0.162028f;
//   float Y = rgb[0] * 0.283881f + rgb[1] * 0.668433f + rgb[2] * 0.047685f;
//   float Z = rgb[0] * 0.000088f + rgb[1] * 0.072310f + rgb[2] * 0.986039f;
//   xy[0] = X / (X + Y + Z);
//   xy[1] = Y / (X + Y + Z);
// }
// #endif // WLED_DISABLE_HUESYNC

//RRGGBB / WWRRGGBB order for hex
void mAnimatorLight::Segment::colorFromDecOrHexString(byte* rgb, char* in)
{
  if (in[0] == 0) return;
  char first = in[0];
  uint32_t c = 0;

  if (first == '#' || first == 'h' || first == 'H') //is HEX encoded
  {
    c = strtoul(in +1, NULL, 16);
  } else
  {
    c = strtoul(in, NULL, 10);
  }

  rgb[0] = R(c);
  rgb[1] = G(c);
  rgb[2] = B(c);
  rgb[3] = W(c);
}

//contrary to the colorFromDecOrHexString() function, this uses the more standard RRGGBB / RRGGBBWW order
bool mAnimatorLight::Segment::colorFromHexString(byte* rgb, const char* in) {
  if (in == nullptr) return false;
  size_t inputSize = strnlen(in, 9);
  if (inputSize != 6 && inputSize != 8) return false;

  uint32_t c = strtoul(in, NULL, 16);

  if (inputSize == 6) {
    rgb[0] = (c >> 16);
    rgb[1] = (c >>  8);
    rgb[2] =  c       ;
  } else {
    rgb[0] = (c >> 24);
    rgb[1] = (c >> 16);
    rgb[2] = (c >>  8);
    rgb[3] =  c       ;
  }
  return true;
}







/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::Segment::color_wheel(uint8_t pos) { // TODO
  // if (palette_id) return color_from_palette(pos, false, true, 0);
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::Segment::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = hw_random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


//do not call this method from system context (network callback)
void mAnimatorLight::finalizeInit(void)
{

  // ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() C%d"), BusManager::getNumBusses());

  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  // // for the lack of better place enumerate ledmaps here
  // // if we do it in json.cpp (serializeInfo()) we are getting flashes on LEDs
  // // unfortunately this means we do not get updates after uploads
  // enumerateLedmaps();

  // _hasWhiteChannel = _isOffRefreshRequired = false;

  // if(tkr_iLight->bus_manager == nullptr)
  // {
  //   ALOG_ERR(PSTR("busses null"));
  //   return;
  // }

  //if busses failed to load, add default (fresh install, FS issue, ...)
  if (BusManager::getNumBusses() == 0) 
  {
    DEBUG_PRINTLN(F("No busses, init default"));
    tkr_iLight->BusManager_Create_DefaultSingleNeoPixel();
  }

  DEBUG_PRINTF("busses->getNumBusses() %d\n\r", BusManager::getNumBusses());

  _length = 0;
  for (uint8_t i=0; i<BusManager::getNumBusses(); i++) 
  {
    
    DEBUG_PRINTF("getNumBusses %d\n\r", i);

    Bus *bus = BusManager::getBus(i);
    if (!bus || !bus->isOk() || bus->getStart() + bus->getLength() > PIXEL_RANGE_LIMIT) break;
    //RGBW mode is enabled if at least one of the strips is RGBW
    _hasWhiteChannel |= bus->hasWhite();
    
    //refresh is required to remain off if at least one of the strips requires the refresh.
    _isOffRefreshRequired |= bus->isOffRefreshRequired();
    uint16_t busEnd = bus->getStart() + bus->getLength();
    
    DEBUG_PRINTF("_hasWhiteChannel %d, _isOffRefreshRequired %d, busEnd %d\n\r", _hasWhiteChannel, _isOffRefreshRequired, busEnd);

    if (busEnd > _length)
    {
      _length = busEnd;
      DEBUG_PRINTF("_length = busEnd;\n\r");
    }

    bus->begin(); //must be called after all busses are created (so parallel buses scale to the longest bus length)

    

    bus->setBrightness(scaledBri(tkr_iLight->_briRGB_Global));

    #ifdef ESP8266
    if ((!IS_BUSTYPE_DIGITAL(bus->getType()) || IS_BUSTYPE_2PIN(bus->getType()))) continue;
    uint8_t pins[5];
    if (!bus->getPins(pins)) continue;
    BusDigital* bd = static_cast<BusDigital*>(bus);
    if (pins[0] == 3) bd->reinit();
    #endif
  }

  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  
  BusManager::initializeABL(); // init brightness limiter

  
  Segment::maxWidth  = _length;
  Segment::maxHeight = 1;

  //segments are created in makeAutoSegments();
  loadCustomPalettes(); // (re)load all custom palettes
  
  
  // DEBUG_PRINT_LN("BEFORE UPDATE PIXEL BUFFER");
  // // allocate frame buffer after matrix has been set up (gaps!)
  // updatePixelBuffer();
  // DEBUG_PRINT_LN("AFTER UPDATE PIXEL BUFFER");


  // ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() D%d"), BusManager::getNumBusses());

}

// // Setter for RGB brightness, with optional global brightness parameter
// void mAnimatorLight::Segment::UpdateBrightness()
// {
//   _brightness_rgb_combined = scale8(_brightness_rgb, tkr_iLight->getBriRGB_Global());
//   _brightness_cct_combined = scale8(_brightness_cct, tkr_iLight->getBriCCT_Global());

// }




void IRAM_ATTR mAnimatorLight::setPixelColor(uint32_t i, uint32_t col) {
  // Serial.printf(" mAnimatorLight::setPixelColor[%d] = %d,%d,%d,%d)\n\r", i, col.R, col.G, col.B, col.WW);


  // if(col)
  // Serial.printf(" mAnimatorLight::setPixelColor[%d] = %d,%d,%d,%d)\n\r", i, R(col), G(col), B(col), W(col) );


  i = getMappedPixelIndex(i);
  if (i >= _length) {
    // Serial.printf("BBBBBBBBBBBvoid IRAM_ATTR mAnimatorLight::%d,%dsetPixelColor(uint32_t i, %d,%d,%d)\n\r", i ,_length,col.R, col.G, col.B);
    
    
    
    return;}
  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  Serial.printf("void IRAM_ATTR mAnimatorLight::setPixelColor(uint32_t i, %d,%d,%d)\n\r", col.R, col.G, col.B);
  #endif
  BusManager::setPixelColor(i, col);
}

uint32_t IRAM_ATTR mAnimatorLight::getPixelColor(uint32_t i) const {
  i = getMappedPixelIndex(i);
  if (i >= _length) return 0;
  return BusManager::getPixelColor(i);
}


//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages!

//fine tune power estimation constants for your setup                  
#define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
                              //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void mAnimatorLight::estimateCurrentAndLimitBri() {
  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = milliampsPerLed;

  // if(milliampsPerLed == 255) {
  //   useWackyWS2815PowerModel = true;
  //   actualMilliampsPerLed = 12; // from testing an actual strip
  // }

  // if (ablMilliampsMax < 150 || actualMilliampsPerLed == 0) { //0 mA per LED and too low numbers turn off calculation
  //   currentMilliamps = 0;
  //   busses.setBrightness(_brightness);
  //   return;
  // }

  // uint16_t pLen = getLengthPhysical();
  // uint32_t puPerMilliamp = 195075 / actualMilliampsPerLed;
  // uint32_t powerBudget = (ablMilliampsMax - MA_FOR_ESP) * puPerMilliamp; //100mA for ESP power
  // if (powerBudget > puPerMilliamp * pLen) { //each LED uses about 1mA in standby, exclude that from power budget
  //   powerBudget -= puPerMilliamp * pLen;
  // } else {
  //   powerBudget = 0;
  // }

  // uint32_t powerSum = 0;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   uint16_t len = bus->getLength();
  //   uint32_t busPowerSum = 0;
  //   for (uint16_t i = 0; i < len; i++) { //sum up the usage of each LED
  //     uint32_t c = bus->getPixelColor(i);
  //     byte r = R(c), g = G(c), b = B(c), w = W(c);

  //     if(useWackyWS2815PowerModel) { //ignore white component on WS2815 power calculation
  //       busPowerSum += (MAX(MAX(r,g),b)) * 3;
  //     } else {
  //       busPowerSum += (r + g + b + w);
  //     }
  //   }

  //   if (bus->isRgbw()) { //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
  //     busPowerSum *= 3;
  //     busPowerSum = busPowerSum >> 2; //same as /= 4
  //   }
  //   powerSum += busPowerSum;
  // }

  // uint32_t powerSum0 = powerSum;
  // powerSum *= _brightness;
  
  // if (powerSum > powerBudget) //scale brightness down to stay in current limit
  // {
  //   float scale = (float)powerBudget / (float)powerSum;
  //   uint16_t scaleI = scale * 255;
  //   uint8_t scaleB = (scaleI > 255) ? 255 : scaleI;
  //   uint8_t newBri = scale8(_brightness, scaleB);
  //   busses.setBrightness(newBri); //to keep brightness uniform, sets virtual busses too
  //   currentMilliamps = (powerSum0 * newBri) / puPerMilliamp;
  // } else {
  //   currentMilliamps = powerSum / puPerMilliamp;
  //   busses.setBrightness(_brightness);
  // }
  // currentMilliamps += MA_FOR_ESP; //add power of ESP back to estimate
  // currentMilliamps += pLen; //add standby power back to estimate
}

// turns all LEDs off and restarts ESP
void mAnimatorLight::reset()
{
  // briT = 0;
  #ifdef ENABLE_FEATURE_LIGHTS__WEBUI
  websocket_lights->closeAll(1012);
  #endif
  
  long dly = millis();
  while (millis() - dly < 450) {
    yield();        // enough time to send response to client
  }
  // applyBri();
  DEBUG_PRINTLN(F("WLED RESET ---- delete this function"));
  ESP.restart();
}

/**
 * Returns the refresh rate of the LED tkr_anim-> Useful for finding out whether a given setup is fast enough.
 * Only updates on show() or is set to 0 fps if last show is more than 2 secs ago, so accurary varies
 */
// uint16_t mAnimatorLight::getFps() {
//   if (millis() - _lastShow > 2000) return 0;
//   return _cumulativeFps +1;
// }

void mAnimatorLight::setTargetFps(uint8_t fps) {
  // if (fps > 0 && fps <= 120) _targetFps = fps;
  // _frametime = 1000 / _targetFps;

  if (fps == FPS_UNLIMITED)
  {
    _targetFps = FPS_UNLIMITED;
    _frametime = 0;
    return;
  }

  if (fps > 120) fps = 120;

  _targetFps = fps;
  _frametime = 1000U / _targetFps;
}

void mAnimatorLight::setEffect(uint8_t segid, uint16_t m) {
  
    #ifdef ENABLE_EFFECT_DESCRIPTIONS  
    // ALOG_INF(PSTR("description len=%d"),effects.description.size());
    #endif
  
  if (segid >= segments.size()) return;

  if (!getEffectCount()) return;
  if (m >= getEffectCount()) m = getEffectCount() - 1;

  if (segments[segid].animation_mode_id != m) {
    // segments[segid].startTransition(_transitionDur); // set effect transitions
    //segments[segid].markForReset();
    segments[segid].animation_mode_id = m;
    
    #ifdef ENABLE_EFFECT_DESCRIPTIONS       
    // ALOG_INF(PSTR("description len=%d"),effects.description.size());
    #endif
  }
}

//applies to all active and selected segments
void mAnimatorLight::setColor(uint8_t slot, uint32_t c) {
  if (slot >= NUMBER_SEGMENT_COLOURS) return;

  for (Segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setColor(slot, c);
    }
  }
}

void mAnimatorLight::setCCT(uint16_t k) {
  for (Segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setCCT(k);
    }
  }
}

void mAnimatorLight::setBrightness(uint8_t b, bool direct) {
  // ALOG_INF(PSTR("setBrightness(%d, %d)"),b,direct);
  // if (gammaCorrectBri) b = gamma8(b);  
  if (_brightness == b) return;
  _brightness = b;
  if (_brightness == 0) { //unfreeze all segments on power off
    for (Segment &seg : segments) {
      seg.freeze = false;
    }
  }
  BusManager::setBrightness(b);
  if (!direct){
    unsigned long t = millis();
    // if (segments[0].next_time > t + 22 && t - _lastShow > MIN_SHOW_DELAY) show(); //apply brightness change immediately if no refresh soon
    if (t - _lastShow > min(_frametime, uint16_t(FRAMETIME_FIXED))) trigger(); //apply brightness change immediately if no refresh soon, but don't speed up above 42fps
  }
}


void mAnimatorLight::setMainSegmentId(uint8_t n) {
  _mainSegment = 0;
  if (n < segments.size()) {
    _mainSegment = n;
  }
  return;
}


uint8_t mAnimatorLight::getFirstSelectedSegId() const {
  size_t i = 0;
  for (const Segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) return i;
    i++;
  }
  // if none selected, use the main segment
  return tkr_anim->getMainSegmentId();
}


uint8_t mAnimatorLight::getLastActiveSegmentId(void) const {
  for (size_t i = segments.size() -1; i > 0; i--) {
    if (segments[i].isActive()) return i;
  }
  return 0;
}


uint8_t mAnimatorLight::getActiveSegmentsNum() const {
  unsigned c = 0;
  for (const Segment &seg : segments) if (seg.isActive()) c++;
  return c;
}


uint16_t mAnimatorLight::getLengthTotal() const {
  unsigned len = Segment::maxWidth * Segment::maxHeight; // will be _length for 1D (see finalizeInit()) but should cover whole matrix for 2D
  if (isMatrix && _length > len) len = _length; // for 2D with trailing strip
  return len;
}

uint16_t mAnimatorLight::getLengthPhysical() const {
  return BusManager::getTotalLength(true);
}


//used for JSON API info.leds.rgbw. Little practical use, deprecate with info.leds.rgbw.
//returns if there is an RGBW bus (supports RGB and White, not only white)
//not influenced by auto-white mode, also true if white slider does not affect output white channel
bool mAnimatorLight::hasRGBWBus() const {
  for (size_t b = 0; b < BusManager::getNumBusses(); b++) {
    const Bus *bus = BusManager::getBus(b);
    if (!bus || !bus->isOk()) break;
    if (bus->hasRGB() && bus->hasWhite()) return true;
  }
  return false;
}

bool mAnimatorLight::hasCCTBus() const {
  if (cctFromRgb && !correctWB) return false;
  for (size_t b = 0; b < BusManager::getNumBusses(); b++) {
    const Bus *bus = BusManager::getBus(b);
    if (!bus || !bus->isOk()) break;
    if (bus->hasCCT()) return true;
  }
  return false;
}


void mAnimatorLight::purgeSegments() {
  // remove all inactive segments (from the back)
  int deleted = 0;
  if (segments.size() <= 1) return;
  for (size_t i = segments.size()-1; i > 0; i--)
    if (segments[i].stop == 0) {
      deleted++;
      segments.erase(segments.begin() + i);
    }
  if (deleted) {
    segments.shrink_to_fit();
    setMainSegmentId(0);
  }
}

mAnimatorLight::Segment& mAnimatorLight::getSegment(uint8_t id) {
  return segments[id >= segments.size() ? getMainSegmentId() : id]; // vectors
}

/**
 * @brief For turning on a new effect/segment
 * 
 * @param n 
 * @param i1 
 * @param i2 
 * @param grouping 
 * @param spacing 
 * @param offset 
 * @param startY 
 * @param stopY 
 */
void mAnimatorLight::setSegment(uint8_t n, uint16_t i1, uint16_t i2, uint8_t grouping, uint8_t spacing, uint16_t offset, uint16_t startY, uint16_t stopY) {
  if (n >= segments.size()) return;
  mAnimatorLight::Segment& seg = segments[n];

  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (seg.start == i1 && seg.stop == i2);
  if (isMatrix) {
    boundsUnchanged &= (seg.startY == startY && seg.stopY == stopY);
  }
  if (boundsUnchanged
      && (!grouping || (seg.grouping_get() == grouping && seg.spacing == spacing))
      && (offset == UINT16_MAX || offset == seg.offset)) return;

  //if (seg.stop) setRange(seg.start, seg.stop -1, BLACK); //turn old segment range off
  if (seg.stop) seg.fill(BLACK); //turn old segment range off
  if (i2 <= i1) //disable segment
  {
    // disabled segments should get removed using purgeSegments()
    DEBUG_PRINT(F("-- mAnimatorLight::Segment ")); DEBUG_PRINT(n); DEBUG_PRINTLN(F(" marked inactive."));
    seg.stop = 0;
    //if (seg.name) {
    //  delete[] seg.name;
    //  seg.name = nullptr;
    //}
    // if main segment is deleted, set first active as main segment
    if (n == _mainSegment) setMainSegmentId(0);
    // seg.markForReset();
    return;
  }
  if (isMatrix) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    if (i1 < mAnimatorLight::Segment::maxWidth) seg.start = i1;
    seg.stop = i2 > mAnimatorLight::Segment::maxWidth ? mAnimatorLight::Segment::maxWidth : i2;
    if (startY < mAnimatorLight::Segment::maxHeight) seg.startY = startY;
    seg.stopY = stopY > mAnimatorLight::Segment::maxHeight ? mAnimatorLight::Segment::maxHeight : MAX(1,stopY);

    ALOG_INF(PSTR("setSegment(%d, %d, %d, %d, %d, %d, %d, %d)"),n,i1,i2,grouping,spacing,offset,startY,stopY);
    // delay(1000);
    #endif
  } else {
    if (i1 < _length) seg.start = i1;
    seg.stop = i2 > _length ? _length : i2;
    seg.startY = 0;
    seg.stopY  = 1;
  }
  if (grouping) {
    seg.grouping_set(grouping);
    seg.spacing = spacing;
  }
  if (offset < UINT16_MAX) seg.offset = offset;
  // seg.markForReset();
  if (!boundsUnchanged) seg.refreshLightCapabilities();
}

void mAnimatorLight::restartRuntime() {
  for (Segment &seg : segments) seg.markForReset();
}


// WARNING: resetSegments(), makeAutoSegments() and fixInvalidSegments() must not be called while
// strip is being serviced (strip.service()), you must call suspend prior if changing segments outside
// loop() context
void mAnimatorLight::resetSegments() {
  if (isServicing()) return;
  segments.clear();          // destructs all Segment as part of clearing
  segments.emplace_back(0, isMatrix ? Segment::maxWidth : _length, 0, isMatrix ? Segment::maxHeight : 1);
  if(segments.size() == 0) {
    segments.emplace_back(); // if out of heap, create a default segment
    errorFlag = ERR_NORAM_PX;
  }
  segments.shrink_to_fit();  // just in case ...
  _mainSegment = 0;
}

void mAnimatorLight::makeAutoSegments(bool forceReset) {
  if (isServicing()) return;
  if (autoSegments) { //make one segment per bus
    unsigned segStarts[MAX_NUM_SEGMENTS] = {0};
    unsigned segStops [MAX_NUM_SEGMENTS] = {0};
    size_t s = 0;

    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // 2D segment is the 1st one using entire matrix
    if (isMatrix) {
      segStarts[0] = 0;
      segStops[0]  = Segment::maxWidth*Segment::maxHeight;
      s++;
    }
    #endif

    for (size_t i = s; i < BusManager::getNumBusses(); i++) {
      const Bus *bus = BusManager::getBus(i);
      if (!bus) break;

      segStarts[s] = bus->getStart();
      segStops[s]  = segStarts[s] + bus->getLength();

      #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
      if (isMatrix && segStops[s] <= Segment::maxWidth*Segment::maxHeight) continue; // ignore buses comprising matrix
      if (isMatrix && segStarts[s] < Segment::maxWidth*Segment::maxHeight) segStarts[s] = Segment::maxWidth*Segment::maxHeight;
      #endif

      //check for overlap with previous segments
      for (size_t j = 0; j < s; j++) {
        if (segStops[j] > segStarts[s] && segStarts[j] < segStops[s]) {
          //segments overlap, merge
          segStarts[j] = min(segStarts[s],segStarts[j]);
          segStops [j] = max(segStops [s],segStops [j]); segStops[s] = 0;
          s--;
        }
      }
      s++;
    }

    segments.clear();
    segments.reserve(s); // prevent reallocations
    // there is always at least one segment (but we need to differentiate between 1D and 2D)
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    if (isMatrix)
      segments.emplace_back(0, Segment::maxWidth, 0, Segment::maxHeight);
    else
    #endif
      segments.emplace_back(segStarts[0], segStops[0]);
    for (size_t i = 1; i < s; i++) {
      segments.emplace_back(segStarts[i], segStops[i]);
    }
    for (size_t i = 0; i < segments.size(); i++) {
      segments[i].segcol[i].colour = DEFAULT_COLOR; // set color to default orange on all segments
    }
    DEBUGFX_PRINTF_P(PSTR("%d auto segments created.\n"), segments.size());

  } else {

    if (forceReset || getSegmentsNum() == 0) resetSegments();
    //expand the main seg to the entire length, but only if there are no other segments, or reset is forced
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
      segments[i].setGeometry(0, Segment::maxWidth, 1, 0, 0xFFFF, 0, Segment::maxHeight);
      #else
      segments[i].setGeometry(0, _length);
      #endif
    }
  }
  _mainSegment = 0;

  fixInvalidSegments();
}

void mAnimatorLight::fixInvalidSegments() {
  if (isServicing()) return;
  //make sure no segment is longer than total (sanity check)
  for (size_t i = getSegmentsNum()-1; i > 0; i--) {
    if (isMatrix) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
      if (segments[i].start >= Segment::maxWidth * Segment::maxHeight) {
        // 1D segment at the end of matrix
        if (segments[i].start >= _length || segments[i].startY > 0 || segments[i].stopY > 1) { segments.erase(segments.begin()+i); continue; }
        if (segments[i].stop  >  _length) segments[i].stop = _length;
        continue;
      }
      if (segments[i].start >= Segment::maxWidth || segments[i].startY >= Segment::maxHeight) { segments.erase(segments.begin()+i); continue; }
      if (segments[i].stop  >  Segment::maxWidth)  segments[i].stop  = Segment::maxWidth;
      if (segments[i].stopY >  Segment::maxHeight) segments[i].stopY = Segment::maxHeight;
    #endif
    } else {
      if (segments[i].start >= _length) { segments.erase(segments.begin()+i); continue; }
      if (segments[i].stop  >  _length) segments[i].stop = _length;
    }
  }
  // if any segments were deleted free memory
  purgeSegments();
  // this is always called as the last step after finalizeInit(), update covered bus types
  for (const Segment &seg : segments)
    seg.refreshLightCapabilities();
}




// void mAnimatorLight::resetSegments2() {
//   segments.clear(); // destructs all mAnimatorLight::Segment as part of clearing
//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   segment seg = isMatrix ? mAnimatorLight::Segment(0, mAnimatorLight::Segment::maxWidth, 0, mAnimatorLight::Segment::maxHeight) : mAnimatorLight::Segment(0, _length);
//   #else
//   segment seg = mAnimatorLight::Segment(0, _length);
//   #endif
//   segments.push_back(seg);
//   _mainSegment = 0;
// }

// void mAnimatorLight::makeAutoSegments(bool forceReset) {
//   ALOG_INF(PSTR("makeAutoSegments(%d) %d==============================="),forceReset, isMatrix);
//   if (isMatrix) {
//     #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//     // only create 1 2D segment
//     if (forceReset || getSegmentsNum() == 0) resetSegments2(); // initialises 1 segment
//     else if (getActiveSegmentsNum() == 1) {
//       size_t i = getLastActiveSegmentId();
//       segments[i].start  = 0;
//       segments[i].stop   = mAnimatorLight::Segment::maxWidth;
//       segments[i].startY = 0;
//       segments[i].stopY  = mAnimatorLight::Segment::maxHeight;
//       segments[i].grouping = 1;
//       segments[i].spacing  = 0;
//       _mainSegment = i;
//       ALOG_HGL(PSTR(" WE ARE HERE WITH PAUSE %d %d %d %d"), segments[i].stop, segments[i].stopY, segments[i].start, segments[i].startY);
//       delay(4000);
//     }
//     #endif
//   } else 
//   if (autoSegments) { //make one segment per bus
//     uint16_t segStarts[MAX_NUM_SEGMENTS] = {0};
//     uint16_t segStops [MAX_NUM_SEGMENTS] = {0};
//     uint8_t s = 0;
//     for (uint8_t i = 0; i < BusManager::getNumBusses(); i++) {
//       Bus* b = BusManager::getBus(i);

//       segStarts[s] = b->getStart();
//       segStops[s]  = segStarts[s] + b->getLength();

//       //check for overlap with previous segments
//       for (size_t j = 0; j < s; j++) {
//         if (segStops[j] > segStarts[s] && segStarts[j] < segStops[s]) {
//           //segments overlap, merge
//           segStarts[j] = min(segStarts[s],segStarts[j]);
//           segStops [j] = max(segStops [s],segStops [j]); segStops[s] = 0;
//           s--;
//         }
//       }
//       s++;
//     }
//     segments.clear();
//     for (size_t i = 0; i < s; i++) {
//       mAnimatorLight::Segment seg = mAnimatorLight::Segment(segStarts[i], segStops[i]);
//       seg.selected = true;
//       segments.push_back(seg);
//     }
//     _mainSegment = 0;
//   } else 
//   {
//     if (forceReset || getSegmentsNum() == 0) resetSegments2();
//     //expand the main seg to the entire length, but only if there are no other segments, or reset is forced
//     else if (getActiveSegmentsNum() == 1) {
//       size_t i = getLastActiveSegmentId();
//       segments[i].start = 0;
//       segments[i].stop  = _length;
//       _mainSegment = 0;
//     }
//   }

//   fixInvalidSegments();
// }

// void mAnimatorLight::fixInvalidSegments() 
// {
  
//   //make sure no segment is longer than total (sanity check)
//   for (size_t i = getSegmentsNum()-1; i > 0; i--) {
    
//     // ALOG_INF(PSTR("Fixing invalid semgents %d %d %d %d"), i, segments[i].start, segments[i].stop, _length);
//     if (segments[i].start >= _length) { segments.erase(segments.begin()+i); continue; }
//     if (segments[i].stop  >  _length) segments[i].stop = _length;
//     // this is always called as the last step after finalizeInit(), update covered bus types
//     segments[i].refreshLightCapabilities();

//     #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//     // Added by me to fix bad matrix setting, the order of matrix and segment creation needs fixing
//     if(segments[i].stopY > mAnimatorLight::Segment::maxHeight) segments[i].stopY = mAnimatorLight::Segment::maxHeight;
//     if(segments[i].stop  > mAnimatorLight::Segment::maxWidth)  segments[i].stop  = mAnimatorLight::Segment::maxWidth; ///fixing X length
//     #endif

//   }
  
// }



//true if all segments align with a bus, or if a segment covers the total length
//irrelevant in 2D set-up
bool mAnimatorLight::checkSegmentAlignment() const {
  bool aligned = false;
  for (const Segment &seg : segments) {
    for (unsigned b = 0; b<BusManager::getNumBusses(); b++) {
      const Bus *bus = BusManager::getBus(b);
      if (!bus || !bus->isOk()) break;
      if (seg.start == bus->getStart() && seg.stop == bus->getStart() + bus->getLength()) aligned = true;
    }
    if (seg.start == 0 && seg.stop == _length) aligned = true;
    if (!aligned) return false;
  }
  return true;
}

// //After this function is called, setPixelColor() will use that segment (offsets, grouping, ... will apply)
// //Note: If called in an interrupt (e.g. JSON API), original segment must be restored,
// //otherwise it can lead to a crash on ESP32 because _segment_index is modified while in use by the main thread
// uint8_t mAnimatorLight::setPixelSegment(uint8_t n)
// {
//   uint8_t prevSegId = segment_index;
//   if (n < segments.size()) {
//     segment_index = n;
//     _virtualSegmentLength = segments[segment_index].virtualLength();
//   }
//   return prevSegId;
// }

void mAnimatorLight::setRange(uint16_t i, uint16_t i2, uint32_t col)
{
  if (i2 >= i)
  {
    for (uint16_t x = i; x <= i2; x++) setPixelColor(x, col);
  } else
  {
    for (uint16_t x = i2; x <= i; x++) setPixelColor(x, col);
  }
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

/*WrapEdge and Discrete should be flipped*/
uint32_t mAnimatorLight::Segment::GetPaletteColour_Legacy(
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
  uint8_t     flag_spanned_segment, 
  /**
   * @brief flag_wrap_hard_edge
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_wrap_hard_edge,
  /**
   * @brief flag_crgb_exact_colour
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_crgb_exact_colour,
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
  bool apply_brightness,
  
  uint8_t pbri,  // this needs removed, as the apply brightness above means the seg/global brightness, not an internal one. The wrapper from WLED needs to do that instead.
  // might need a u8 to describe different levels of brightness application, none, seg/global, internal

  uint8_t mcol
){
  
  DEBUG_LINE_HERE_TRACE
  if(palette_id != palette_loaded->loaded_palette_id)
  {
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }

  // if(pixel_position==0)
  //   Serial.println("PO: GetPaletteColour_Legacy");


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
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
  );
  #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
  white_warm_GetPaletteColour = mPaletteI->colour32_white_cold; // Bypass W2, as this is not used in RGBWW
  #endif

  // Apply brightness if needed
  if (apply_brightness) {
    uint8_t brightness = tkr_iLight->getBriRGB_Global();//scale8(_brightness, tkr_iLight->getBriRGB_Global());
    // ALOG_INF(PSTR("brightness getpal %d"),brightness);
    uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range

    // Extract, scale, and repack in one step
    colour = RGBW32(
      (R(colour) * scale) >> 8,  // Red
      (G(colour) * scale) >> 8,  // Green
      (B(colour) * scale) >> 8,  // Blue
      (W(colour) * scale) >> 8   // White
    );
    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    white_warm_GetPaletteColour = (mPaletteI->colour32_white_cold * scale) >> 8; // Rescale bypass W2
    #endif
  }

  return colour;

}

// RgbwwColor mAnimatorLight::Segment::GetPaletteColour_RGBWW(
//   /**
//    * @brief _pixel_position
//    * ** [0-SEGLEN]
//    * ** [0-255]   
//    */
//   uint16_t pixel_position,
//   /**
//    * @brief flag_spanned_segment
//    * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
//    * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
//    * ** [2]: preffered
//    */
//   uint8_t     flag_spanned_segment, 
//   /**
//    * @brief flag_wrap_hard_edge
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     flag_wrap_hard_edge,
//   /**
//    * @brief flag_crgb_exact_colour
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     flag_crgb_exact_colour,
//   /**
//    * @brief encoded_value
//    * ** [uint32_t*] : encoded value from palette
//    */
//   uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

//   bool apply_brightness
// ){

//   uint32_t colour32 = GetPaletteColour(
//     pixel_position,
//     flag_spanned_segment,
//     flag_wrap_hard_edge,
//     flag_crgb_exact_colour,
//     encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//     apply_brightness,
//     255,0
//   );

//   return RgbwwColor(
//     R(colour32),  // Red
//     G(colour32),  // Green
//     B(colour32),  // Blue
//     W(colour32),  // White
//     white_warm_GetPaletteColour  // Cold White
//   );

// }



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

// RgbwwColor mAnimatorLight::Segment::GetPaletteColour_RGBWW_2025(
//   /**
//    * @brief _pixel_position
//    * ** [0-SEGLEN]
//    * ** [0-255]   
//    */
//   uint16_t pixel_position,
//   /**
//    * @brief flag_spanned_segment
//    * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
//    * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
//    * ** [2]: preffered
//    */
//   uint8_t     flag_spanned_segment, 
//   /**
//    * @brief force_palette_mode flag_crgb_exact_colour
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     force_palette_mode,
//   /**
//    * @brief flag_wrap_hard_edge
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     flag_wrap_hard_edge,
//   /**
//    * @brief encoded_value
//    * ** [uint32_t*] : encoded value from palette
//    */
//   uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

//   bool apply_brightness
// ){

//   uint32_t colour32 = GetPaletteColour_Legacy(
//     pixel_position,
//     flag_spanned_segment,
//     flag_wrap_hard_edge,
//     force_palette_mode,
//     encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//     apply_brightness
//   );

//   return RgbwwColor(
//     R(colour32),  // Red
//     G(colour32),  // Green
//     B(colour32),  // Blue
//     W(colour32),  // White
//     white_warm_GetPaletteColour  // Cold White
//   );

// }


/**
 * @brief 
 * Need to make options here on how 2D calls different palettes
 * ColorFromPalette via FastLED
 * ColorFromPaletteU32 via mPalette WLED fast custom ColorFromPalette
 * ColorFromPalette_WithLoad to make sure we load, though it should be forced to reload anyway
 * GetPaletteColour
 * 
 * Should be able to use define to switch.
 * 
 */
CRGB mAnimatorLight::ColorFromPalette_WithLoad(const CRGBPalette16 &pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
  if(SEGMENT.palette_id != SEGMENT.palette_loaded->loaded_palette_id)
  {
    SEGMENT.LoadPalette(SEGMENT.palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }
  return ColorFromPalette(pal, index, brightness, blendType);
}


// /**
//  * @brief 
//  * The problem is this function will still cause a reload if a LoadPalette type is used. So preference is to use a CRGBPalette type as overdraw when needed
//  */
// RgbwwColor IRAM_ATTR mAnimatorLight::GetPaletteColour_WithTemporaryLoad(
//   uint16_t palette_id,
//   uint16_t _pixel_position,
//   bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
//   bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
//   bool     flag_crgb_exact_colour,
//   uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//   bool     flag_request_is_for_full_visual_output
// ){
//   /**
//    * @brief Directly handle certain palette types that don't need loading.
//    * These palette types are handled directly, bypassing the LoadPalette function.
//    */
//   if (
//     ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
//     ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)) ||
//     ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
//     ((palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)) ||
//     ((palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
//   ) {
//     DEBUG_LINE_HERE_TRACE
    
//     #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
//     // These palettes do not require loading into RAM. Directly call GetColourFromPreloadedPaletteBuffer_RGBWW
//     return mPaletteI->GetColourFromPreloadedPaletteBuffer_RGBWW(
//       palette_id,
//       nullptr, // No buffer required for these types
//       _pixel_position,
//       encoded_value,
//       flag_spanned_segment,
//       flag_wrap_hard_edge,
//       flag_crgb_exact_colour,
//       flag_request_is_for_full_visual_output
//     );
//     #else
//     // These palettes do not require loading into RAM. Directly call GetColourFromPreloadedPaletteBuffer_RGBWW
//     uint32_t colour32 = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
//       palette_id,
//       nullptr, // No buffer required for these types
//       _pixel_position,
//       encoded_value,
//       flag_spanned_segment,
//       flag_wrap_hard_edge,
//       flag_crgb_exact_colour,
//       flag_request_is_for_full_visual_output
//     );
//     return RgbwwColor(
//       R(colour32),  // Red
//       G(colour32),  // Green
//       B(colour32),  // Blue
//       W(colour32),  // White
//       0  // Cold White
//     );  
//     #endif
//   }

//   /**
//    * @brief Load is required for other palette types, so we call LoadPalette
//    */
//   mPaletteLoaded palette_container_temp = mPaletteLoaded();
  
//   SEGMENT.LoadPalette(palette_id, &palette_container_temp);
  
  
//   #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
//   return mPaletteI->GetColourFromPreloadedPaletteBuffer_RGBWW(
//     palette_id,
//     &palette_container_temp.pData[0],
//     _pixel_position,
//     encoded_value,
//     flag_spanned_segment,
//     flag_wrap_hard_edge,
//     flag_crgb_exact_colour,
//     flag_request_is_for_full_visual_output
//   );
//   #else
//   uint32_t colour32 = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
//     palette_id,
//     &palette_container_temp.pData[0],
//     _pixel_position,
//     encoded_value,
//     flag_spanned_segment,
//     flag_wrap_hard_edge,
//     flag_crgb_exact_colour,
//     flag_request_is_for_full_visual_output
//   );
//   return RgbwwColor(
//     R(colour32),  // Red
//     G(colour32),  // Green
//     B(colour32),  // Blue
//     W(colour32),  // White
//     0  // Cold White
//   );  

//   #endif
// }


// /*******************************************************************************************************************************************************************************************************************
//  * @description : SUCCESSOR API — GetUnloadedPaletteColour_ModeWrap
//  *                Reordered args to match GetPaletteColour so calls can rely on header defaults.
//  *                Internally maps (index_mode, palette_mode, wrap) to legacy boolean flags.
//  ********************************************************************************************************************************************************************************************************************/
// RgbwwColor IRAM_ATTR mAnimatorLight::GetUnloadedPaletteColour_ModeWrap(
//   uint16_t palette_id,
//   uint16_t index_or_pos,
//   uint8_t  palette_index_mode,           // e.g. PALETTE_INDEX__IS_EXACT_COLOUR, PALETTE_INDEX__IS_SEGLEN_RANGE
//   uint8_t  palette_mode,                 // e.g. PALETTE_MODE__DEFAULT / __EXACT / __DISCRETE, etc.
//   uint8_t  palette_wrap,                 // e.g. PALETTE_WRAP_HARDEDGE / __ON
//   uint8_t* encoded_value,                // pass NO_ENCODED_VALUE if unused
//   bool     flag_request_is_for_full_visual_output
// ){

//   // --- translate Mode/Wrap to legacy flags used by palette backends ---
//   const bool flag_spanned_segment  = (palette_index_mode == PALETTE_INDEX__IS_SEGLEN_RANGE);
//   const bool flag_wrap_hard_edge   = (palette_wrap       == PALETTE_WRAP_SMOOTH);
//   const bool flag_crgb_exact_color = (palette_mode       == PALETTE_INDEX__IS_EXACT_COLOUR);

//   // --- direct, no-load paths (static / generated palettes) ---
//   if (
//     ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID)                  && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))                 ||
//     ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)                 && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))          ||
//     ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))||
//     ((palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID)                             && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID))                  ||
//     ((palette_id >= mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID)                            && (palette_id < mPalette::PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
//   ) {
//     #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
//       return mPaletteI->GetColourFromPreloadedPaletteBuffer_RGBWW(
//         palette_id, nullptr,
//         index_or_pos, encoded_value,
//         flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_color,
//         flag_request_is_for_full_visual_output
//       );
//     #else
//       const uint32_t c32 = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
//         palette_id, nullptr,
//         index_or_pos, encoded_value,
//         flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_color,
//         flag_request_is_for_full_visual_output
//       );
//       return RgbwwColor(R(c32), G(c32), B(c32), W(c32), 0);
//     #endif
//   }

//   // --- load-once path for other palette types ---
//   mPaletteLoaded palette_container_temp;
//   SEGMENT.LoadPalette(palette_id, &palette_container_temp);

//   #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
//     return mPaletteI->GetColourFromPreloadedPaletteBuffer_RGBWW(
//       palette_id, &palette_container_temp.pData[0],
//       index_or_pos, encoded_value,
//       flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_color,
//       flag_request_is_for_full_visual_output
//     );
//   #else
//     const uint32_t c32 = mPaletteI->GetColourFromPreloadedPaletteBuffer_U32(
//       palette_id, &palette_container_temp.pData[0],
//       index_or_pos, encoded_value,
//       flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_color,
//       flag_request_is_for_full_visual_output
//     );
//     return RgbwwColor(R(c32), G(c32), B(c32), W(c32), 0);
//   #endif
// }
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


// pixel is clipped if it falls outside clipping range
// if clipping start > stop the clipping range is inverted
bool mAnimatorLight::Segment::isPixelClipped(int i) const {
  if (tkr_anim->blendingStyle != TRANSITION_FADE && isInTransition() && _clipStart != _clipStop) {
    bool invert = _clipStart > _clipStop;  // ineverted start & stop
    int start = invert ? _clipStop : _clipStart;
    int stop  = invert ? _clipStart : _clipStop;
    if (tkr_anim->blendingStyle == TRANSITION_FAIRY_DUST) {
      unsigned len = stop - start;
      if (len < 2) return false;
      unsigned shuffled = hashInt(i) % len;
      unsigned pos = (shuffled * 0xFFFFU) / len;
      return progress() <= pos;
    }
    const bool iInside = (i >= start && i < stop);
    return !iInside ^ invert; // thanks @willmmiles (https://github.com/wled/WLED/pull/3877#discussion_r1554633876)
  }
  return false;
}


void WLED_O2_ATTR mAnimatorLight::Segment::setPixelColor(int i, uint32_t col, bool flag_brightness_already_applied)
{
  
// if(i<5)
//   ALOG_INF(PSTR("Ai %d col %d,%d,%d"),i, R(col), G(col), B(col));


  #ifndef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  int vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows) REQUIRED for bouncing balls effect. Assumes this means int is 32 bit here?
  i &= 0xFFFF;
  #endif

  if (!isActive() || i < 0) 
  {
    // if(i<1) ALOG_INF(PSTR("Bi %d col %d,%d,%d"),i, R(col), G(col), B(col));
    return;
  }
    // if(i<1) ALOG_INF(PSTR("Ci %d col %d,%d,%d"),i, R(col), G(col), B(col));


  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    int vStrip = 0;
  #endif
  int vL = vLength();
  // if the 1D effect is using virtual strips "i" will have virtual strip id stored in upper 16 bits
  // in such case "i" will be > virtualLength()
  if (i >= vL) {
    // check if this is a virtual strip
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
    i &= 0xFFFF;    //truncate vstrip index
    if (i >= vL) 
    {
      ALOG_ERR(PSTR("return %d"),__LINE__);
      return;  // if pixel would still fall out of segment just exit
    }
    #else
    return;
    #endif
  }

#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    
    const int vW = vWidth();
    const int vH = vHeight();
    const auto XY = [&](unsigned x, unsigned y) { return x + y * vW; };

      // ALOG_ERR(PSTR("vH %d %d %d"),vW,vH,map1D2D);

    switch (map1D2D) {
      case M12_Pixels:
        setPixelColorRaw(XY(i % vW, i / vW), col);
        break;

      case M12_pBar:
        if (vStrip > 0) setPixelColorRaw(XY(vStrip - 1, vH - i - 1), col);
        else for (int x = 0; x < vW; x++) setPixelColorRaw(XY(x, vH - i - 1), col);
        break;

      case M12_pArc:
        if (i == 0) {
          setPixelColorRaw(XY(0, 0), col);
        } else {
          float r = i;
          float step = HALF_PI / (2.8284f * r + 4);
          for (float rad = 0.0f; rad <= (HALF_PI / 2) + step / 2; rad += step) {
            int x = roundf(sin_t(rad) * r);
            int y = roundf(cos_t(rad) * r);
            setPixelColorXY(x, y, col);
            setPixelColorXY(y, x, col);
          }
        }
        break;

      case M12_pCorner:
        for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
        for (int y = 0; y < i; y++) setPixelColorXY(i, y, col);
        break;

      case M12_sPinwheel: {
        int startX, startY, cosVal[2], sinVal[2];
        setPinwheelParameters(i, vW, vH, startX, startY, cosVal, sinVal);

        unsigned maxLineLength = max(vW, vH) + 2;
        uint16_t lineCoords[2][maxLineLength];
        int lineLength[2] = {0};
        static int prevRays[2] = {INT_MAX, INT_MAX};
        int closestEdgeIdx = INT_MAX;

        for (int lineNr = 0; lineNr < 2; lineNr++) {
          int x0 = startX;
          int y0 = startY;
          int x1 = startX + (cosVal[lineNr] << 9);
          int y1 = startY + (sinVal[lineNr] << 9);
          const int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
          const int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
          uint16_t* coordinates = lineCoords[lineNr];
          int* length = &lineLength[lineNr];

          x0 /= Fixed_Scale;
          y0 /= Fixed_Scale;

          int idx = 0;
          int err = dx + dy;

          while (true) {
            if ((unsigned)x0 >= (unsigned)vW || (unsigned)y0 >= (unsigned)vH) {
              closestEdgeIdx = min(closestEdgeIdx, idx - 2);
              break;
            }

            coordinates[idx++] = x0;
            coordinates[idx++] = y0;
            (*length)++;

            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
          }
        }

        int diff = lineLength[0] - lineLength[1];
        int longLineIdx = diff > 0 ? 0 : 1;
        int shortLineIdx = longLineIdx ? 0 : 1;

        if (diff != 0) {
          int idx = (lineLength[shortLineIdx] - 1) * 2;
          int lastX = lineCoords[shortLineIdx][idx++];
          int lastY = lineCoords[shortLineIdx][idx++];
          bool keepX = lastX == 0 || lastX == vW - 1;

          for (int d = 0; d < abs(diff); d++) {
            lineCoords[shortLineIdx][idx] = keepX ? lastX : lineCoords[longLineIdx][idx];
            idx++;
            lineCoords[shortLineIdx][idx] = keepX ? lineCoords[longLineIdx][idx] : lastY;
            idx++;
          }
        }

        closestEdgeIdx += 2;

        int max_i = getPinwheelLength(vW, vH) - 1;
        bool drawFirst = !(prevRays[0] == i - 1 || (i == 0 && prevRays[0] == max_i));
        bool drawLast  = !(prevRays[0] == i + 1 || (i == max_i && prevRays[0] == 0));

        for (int idx = 0; idx < lineLength[longLineIdx] * 2;) {
          int x1 = lineCoords[0][idx];
          int x2 = lineCoords[1][idx++];
          int y1 = lineCoords[0][idx];
          int y2 = lineCoords[1][idx++];

          int minX = min(x1, x2);
          int maxX = max(x1, x2);
          int minY = min(y1, y2);
          int maxY = max(y1, y2);

          bool alwaysDraw = (drawFirst && drawLast) ||
                            (idx > closestEdgeIdx) ||
                            (i == 0 && idx == 2) ||
                            (i == prevRays[1]);

          for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
              bool onLine1 = x == x1 && y == y1;
              bool onLine2 = x == x2 && y == y2;

              if (alwaysDraw ||
                  (!onLine1 && (!onLine2 || drawLast)) ||
                  (!onLine2 && (!onLine1 || drawFirst))) {
                setPixelColorXY(x, y, col);
              }
            }
          }
        }

        prevRays[1] = prevRays[0];
        prevRays[0] = i;
        break;
      }
    }

    return;
  }

  if (Segment::maxHeight != 1 && (width() == 1 || height() == 1)) {
    if (start < Segment::maxWidth * Segment::maxHeight) {
      int x = 0, y = 0;
      if (vHeight() > 1) y = i;
      if (vWidth() > 1) x = i;
      setPixelColorXY(x, y, col);
      return;
    }
  }
#endif

// if(i<1)
//   ALOG_INF(PSTR("i %d col %d,%d,%d"),i, R(col), G(col), B(col));

  #ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
    if (decimate > 1)
    {
      const uint16_t pattern_length = vLength();

      for (uint8_t repeat = 0; repeat < decimate; repeat++)
      {
        const uint32_t repeated_index =
          static_cast<uint32_t>(i) +
          static_cast<uint32_t>(repeat) * pattern_length;

        if (repeated_index >= length()) break;

        // ALOG_INF(PSTR("per %d, %d"), i, repeated_index);

        setPixelColorRaw(
          static_cast<uint16_t>(repeated_index),
          col
        );
      }

      return;
    }
  #endif

  setPixelColorRaw(i, col);
}



/*
 * color blend function
 */
uint32_t mAnimatorLight::Segment::color_blend(uint32_t color1, uint32_t color2, uint16_t blend, bool b16) {
  if(blend == 0)   return color1;
  uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
  if(blend == blendmax) return color2;
  uint8_t shift = b16 ? 16 : 8;

  uint32_t w1 = W(color1);
  uint32_t r1 = R(color1);
  uint32_t g1 = G(color1);
  uint32_t b1 = B(color1);

  uint32_t w2 = W(color2);
  uint32_t r2 = R(color2);
  uint32_t g2 = G(color2);
  uint32_t b2 = B(color2);

  uint32_t w3 = ((w2 * blend) + (w1 * (blendmax - blend))) >> shift;
  uint32_t r3 = ((r2 * blend) + (r1 * (blendmax - blend))) >> shift;
  uint32_t g3 = ((g2 * blend) + (g1 * (blendmax - blend))) >> shift;
  uint32_t b3 = ((b2 * blend) + (b1 * (blendmax - blend))) >> shift;

  return RGBW32(r3, g3, b3, w3);
}


RgbwwColor mAnimatorLight::Segment::color_blend(RgbwwColor color1, RgbwwColor color2, uint16_t blend, bool b16) {
  if(blend == 0)   return color1;
  uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
  if(blend == blendmax) return color2;
  uint8_t shift = b16 ? 16 : 8;

  return RgbwwColor(
    ((color2.R * blend) + (color1.R * (blendmax - blend))) >> shift,
    ((color2.G * blend) + (color1.G * (blendmax - blend))) >> shift,
    ((color2.B * blend) + (color1.B * (blendmax - blend))) >> shift,
    ((color2.WW * blend) + (color1.WW * (blendmax - blend))) >> shift,
    ((color2.CW * blend) + (color1.CW * (blendmax - blend))) >> shift
  );

}


uint32_t mAnimatorLight::Segment::color_add(RgbwwColor c1, RgbwwColor c2, bool fast)
{
  if (fast) {
    uint8_t r = c1.R;
    uint8_t g = c1.G;
    uint8_t b = c1.B;
    uint8_t w = c1.WW;
    r = qadd8(r, c2.R);
    g = qadd8(g, c2.G);
    b = qadd8(b, c2.B);
    w = qadd8(w, c2.WW);
    return RGBW32(r, g, b, w);
  } else {
    uint32_t r = c1.R + c2.R;
    uint32_t g = c1.G + c2.G;
    uint32_t b = c1.B + c2.B;
    uint32_t w = c1.WW + c2.WW;
    uint16_t max = r;
    if (g > max) max = g;
    if (b > max) max = b;
    if (w > max) max = w;
    if (max < 256) return RGBW32(r, g, b, w);
    else           return RGBW32(r * 255 / max, g * 255 / max, b * 255 / max, w * 255 / max);
  }
}


uint32_t mAnimatorLight::Segment::color_add(uint32_t c1, uint32_t c2, bool fast)
{
  if (fast) {
    uint8_t r = R(c1);
    uint8_t g = G(c1);
    uint8_t b = B(c1);
    uint8_t w = W(c1);
    r = qadd8(r, R(c2));
    g = qadd8(g, G(c2));
    b = qadd8(b, B(c2));
    w = qadd8(w, W(c2));
    return RGBW32(r,g,b,w);
  } else {
    uint32_t r = R(c1) + R(c2);
    uint32_t g = G(c1) + G(c2);
    uint32_t b = B(c1) + B(c2);
    uint32_t w = W(c1) + W(c2);
    uint16_t max = r;
    if (g > max) max = g;
    if (b > max) max = b;
    if (w > max) max = w;
    if (max < 256) return RGBW32(r, g, b, w);
    else           return RGBW32(r * 255 / max, g * 255 / max, b * 255 / max, w * 255 / max);
  }
}


RgbwwColor mAnimatorLight::Segment::color_fade(RgbwwColor c1, uint8_t amount, bool video)
{
  
  // return c1;// tmp fix



  uint8_t r = c1.R;
  uint8_t g = c1.G;
  uint8_t b = c1.B;
  uint8_t w = c1.WW;
  uint8_t cw = c1.CW;
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
    cw = scale8_video(cw, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
    cw = scale8(cw, amount);
  }
  return RgbwwColor(r, g, b, w, cw);
}
uint32_t mAnimatorLight::Segment::color_fade(uint32_t c1, uint8_t amount, bool video)
{
  
  // return c1;// tmp fix


  uint8_t r = R(c1);
  uint8_t g = G(c1);
  uint8_t b = B(c1);
  uint8_t w = W(c1);
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
  }
  return RGBW32(r, g, b, w);
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float mAnimatorLight::cos_t(float phi)
{
  float x = modd(phi, TWO_PI);
  if (x < 0) x = -1 * x;
  int8_t sign = 1;
  if (x > PI)
  {
      x -= PI;
      sign = -1;
  }
  float xx = x * x;

  float res = sign * (1 - ((xx) / (2)) + ((xx * xx) / (24)) - ((xx * xx * xx) / (720)) + ((xx * xx * xx * xx) / (40320)) - ((xx * xx * xx * xx * xx) / (3628800)) + ((xx * xx * xx * xx * xx * xx) / (479001600)));
  #ifdef WLED_DEBUG_MATH
  Serial.printf("cos: %f,%f,%f,(%f)\n",phi,res,cos(x),res-cos(x));
  #endif
  return res;
}

float mAnimatorLight::sin_t(float x) {
  float res =  cos_t(HALF_PI - x);
  #ifdef WLED_DEBUG_MATH
  Serial.printf("sin: %f,%f,%f,(%f)\n",x,res,sin(x),res-sin(x));
  #endif
  return res;
}

float mAnimatorLight::tan_t(float x) {
  float c = cos_t(x);
  if (c==0.0f) return 0;
  float res = sin_t(x) / c;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("tan: %f,%f,%f,(%f)\n",x,res,tan(x),res-tan(x));
  #endif
  return res;
}

//https://stackoverflow.com/questions/3380628
// Absolute error <= 6.7e-5
float mAnimatorLight::acos_t(float x) {
  float negate = float(x < 0);
  float xabs = std::abs(x);
  float ret = -0.0187293f;
  ret = ret * xabs;
  ret = ret + 0.0742610f;
  ret = ret * xabs;
  ret = ret - 0.2121144f;
  ret = ret * xabs;
  ret = ret + HALF_PI;
  ret = ret * sqrt(1.0f-xabs);
  ret = ret - 2 * negate * ret;
  float res = negate * PI + ret;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("acos: %f,%f,%f,(%f)\n",x,res,acos(x),res-acos(x));
  #endif
  return res;
}

float mAnimatorLight::asin_t(float x) {
  float res = HALF_PI - acos_t(x);
  #ifdef WLED_DEBUG_MATH
  Serial.printf("asin: %f,%f,%f,(%f)\n",x,res,asin(x),res-asin(x));
  #endif
  return res;
}


// //gamma 2.8 lookup table used for color correction
// uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT[256] = {
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
//     1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//     2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
//     5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
//    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
//    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
//    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
//    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
//    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
//    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
//    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
//   115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
//   144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
//   177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
//   215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
// uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT_inv[256] = {
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
//     1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//     2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
//     5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
//    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
//    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
//    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
//    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
//    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
//    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
//    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
//   115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
//   144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
//   177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
//   215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
// // uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT[256];
// // uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT_inv[256];

// // re-calculates & fills gamma table
// void mAnimatorLight::NeoGammaWLEDMethod::calcGammaTable(float gamma)
// {
//   float gamma_inv = 1.0f / gamma; // inverse gamma
//   for (size_t i = 1; i < 256; i++) {
//     gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
//     gammaT_inv[i] = (int)(powf(((float)i - 0.5f) / 255.0f, gamma_inv) * 255.0f + 0.5f);
//     //DEBUG_PRINTF_P(PSTR("gammaT[%d] = %d gammaT_inv[%d] = %d\n"), i, gammaT[i], i, gammaT_inv[i]);
//   }
//   gammaT[0] = 0;
//   gammaT_inv[0] = 0;
// }

// uint8_t mAnimatorLight::NeoGammaWLEDMethod::Correct(uint8_t value)
// {
//   // if (!gammaCorrectCol) return value;
//   return gammaT[value];
// }

// // used for color gamma correction
// uint32_t mAnimatorLight::NeoGammaWLEDMethod::Correct32(uint32_t color)
// {
//   // if (!gammaCorrectCol) return color;
//   uint8_t w = W(color);
//   uint8_t r = R(color);
//   uint8_t g = G(color);
//   uint8_t b = B(color);
//   w = gammaT[w];
//   r = gammaT[r];
//   g = gammaT[g];
//   b = gammaT[b];
//   return RGBW32(r, g, b, w);
// }

uint8_t mAnimatorLight::sin_gap(uint16_t in) {
  if (in & 0x100) return 0;
  return sin8_t(in + 192); // correct phase shift of sine so that it starts and stops at 0
}
uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

/*
 * Generates a tristate square wave w/ attac & decay
 * @param x input value 0-255
 * @param pulsewidth 0-127
 * @param attdec attack & decay, max. pulsewidth / 2
 * @returns signed waveform value
 */
int8_t mAnimatorLight::tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec) {
  int8_t a = 127;
  if (x > 127) {
    a = -127;
    x -= 127;
  }

  if (x < attdec) { //inc to max
    return (int16_t) x * a / attdec;
  }
  else if (x < pulsewidth - attdec) { //max
    return a;
  }
  else if (x < pulsewidth) { //dec to 0
    return (int16_t) (pulsewidth - x) * a / attdec;
  }
  return 0;
}



#ifdef ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD

///////////////////////////////////////////////////////////////////////////////
// Begin simulateSound (to enable audio enhanced effects to display something)
///////////////////////////////////////////////////////////////////////////////
// Currently 4 types defined, to be fine tuned and new types added
// (only 2 used as stored in 1 bit in segment options, consider switching to a single global simulation type)
typedef enum UM_SoundSimulations {
  UMS_BeatSin = 0,
  UMS_WeWillRockYou,
  UMS_10_13,
  UMS_14_3
} um_soundSimulations_t;

mAnimatorLight::um_data_t* mAnimatorLight::simulateSound(uint8_t simulationId)
{
  static uint8_t samplePeak;
  static float   FFT_MajorPeak;
  static uint8_t maxVol;
  static uint8_t binNum;

  static float    volumeSmth;
  static uint16_t volumeRaw;
  static float    my_magnitude;

  //arrays
  uint8_t *fftResult;

  static um_data_t* um_data = nullptr;

  if (!um_data) {
    //claim storage for arrays
    fftResult = (uint8_t *)malloc(sizeof(uint8_t) * 16);

    // initialize um_data pointer structure
    // NOTE!!!
    // This may change as AudioReactive usermod may change
    um_data = new um_data_t;
    um_data->u_size = 8;
    um_data->u_type = new um_types_t[um_data->u_size];
    um_data->u_data = new void*[um_data->u_size];
    um_data->u_data[0] = &volumeSmth;
    um_data->u_data[1] = &volumeRaw;
    um_data->u_data[2] = fftResult;
    um_data->u_data[3] = &samplePeak;
    um_data->u_data[4] = &FFT_MajorPeak;
    um_data->u_data[5] = &my_magnitude;
    um_data->u_data[6] = &maxVol;
    um_data->u_data[7] = &binNum;
  } else {
    // get arrays from um_data
    fftResult =  (uint8_t*)um_data->u_data[2];
  }

  uint32_t ms = millis();

  switch (simulationId) {
    default:
    case UMS_BeatSin:
      for (int i = 0; i<16; i++)
        fftResult[i] = beatsin8_t(120 / (i+1), 0, 255);
        // fftResult[i] = (beatsin8(120, 0, 255) + (256/16 * i)) % 256;
        volumeSmth = fftResult[8];
      break;
    case UMS_WeWillRockYou:
      if (ms%2000 < 200) {
        volumeSmth = hw_random8(255);
        for (int i = 0; i<5; i++)
          fftResult[i] = hw_random8(255);
      }
      else if (ms%2000 < 400) {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      else if (ms%2000 < 600) {
        volumeSmth = hw_random8(255);
        for (int i = 5; i<11; i++)
          fftResult[i] = hw_random8(255);
      }
      else if (ms%2000 < 800) {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      else if (ms%2000 < 1000) {
        volumeSmth = hw_random8(255);
        for (int i = 11; i<16; i++)
          fftResult[i] = hw_random8(255);
      }
      else {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      break;
    case UMS_10_13:
      for (int i = 0; i<16; i++)
        fftResult[i] = inoise8(beatsin8_t(90 / (i+1), 0, 200)*15 + (ms>>10), ms>>3);
        volumeSmth = fftResult[8];
      break;
    case UMS_14_3:
      for (int i = 0; i<16; i++)
        fftResult[i] = inoise8(beatsin8_t(120 / (i+1), 10, 30)*10 + (ms>>14), ms>>3);
      volumeSmth = fftResult[8];
      break;
  }

  samplePeak    = hw_random8() > 250;
  FFT_MajorPeak = 21 + (volumeSmth*volumeSmth) / 8.0f; // walk thru full range of 21hz...8200hz
  maxVol        = 31;  // this gets feedback fro UI
  binNum        = 8;   // this gets feedback fro UI
  volumeRaw = volumeSmth;
  my_magnitude = 10000.0f / 8.0f; //no idea if 10000 is a good value for FFT_Magnitude ???
  if (volumeSmth < 1 ) my_magnitude = 0.001f;             // noise gate closed - mute

  return um_data;
}

#endif // ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD


// // similar to NeoPixelBus NeoGammaTableMethod but allows dynamic changes (superseded by NPB::NeoGammaDynamicTableMethod)
// class NeoGammaWLEDMethod {
//   public:
//     static uint8_t Correct(uint8_t value);      // apply Gamma to single channel
//     static uint32_t Correct32(uint32_t color);  // apply Gamma to RGBW32 color (WLED specific, not used by NPB)
//     static void calcGammaTable(float gamma);    // re-calculates & fills gamma table
//     static inline uint8_t rawGamma8(uint8_t val) { return gammaT[val]; }  // get value from Gamma table (WLED specific, not used by NPB)
//   private:
//     static uint8_t gammaT[];
// };
// #define gamma32(c) NeoGammaWLEDMethod::Correct32(c)


// //gamma 2.8 lookup table used for color correction
// uint8_t NeoGammaWLEDMethod::gammaT[256] = {
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
//     1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//     2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
//     5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
//    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
//    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
//    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
//    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
//    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
//    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
//    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
//   115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
//   144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
//   177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
//   215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// // re-calculates & fills gamma table
// void NeoGammaWLEDMethod::calcGammaTable(float gamma)
// {
//   for (size_t i = 0; i < 256; i++) {
//     gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
//   }
// }

// uint8_t NeoGammaWLEDMethod::Correct(uint8_t value)
// {
//   if (!tkr_anim->gammaCorrectCol) return value;
//   return gammaT[value];
// }

// // used for color gamma correction
// uint32_t NeoGammaWLEDMethod::Correct32(uint32_t color)
// {
//   if (!tkr_anim->gammaCorrectCol) return color;
//   uint8_t w = W(color);
//   uint8_t r = R(color);
//   uint8_t g = G(color);
//   uint8_t b = B(color);
//   w = gammaT[w];
//   r = gammaT[r];
//   g = gammaT[g];
//   b = gammaT[b];
//   return RGBW32(r, g, b, w);
// }



float mAnimatorLight::floor_t(float x) {
  bool neg = x < 0;
  int val = x;
  if (neg) val--;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("floor: %f,%f,%f\n",x,(float)val,floor(x));
  #endif
  return val;
}

float mAnimatorLight::fmod_t(float num, float denom) {
  int tquot = num / denom;
  float res = num - tquot * denom;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("fmod: %f,%f,(%f)\n",res,fmod(num,denom),res-fmod(num,denom));
  #endif
  return res;
}



/*
 * UDP sync notifier / Realtime / Hyperion / TPM2.NET
 */

#define UDP_SEG_SIZE 36
#define SEG_OFFSET (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE))
#define WLEDPACKETSIZE (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE)+0)
#define UDP_IN_MAXSIZE 1472
#define PRESUMED_NETWORK_DELAY 3 //how many ms could it take on avg to reach the receiver? This will be added to transmitted times



void realtimeLock2(uint32_t timeoutMs, byte md);

#define WLED_GLOBAL
#define _INIT(x) = x


//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w = 0);

//scales the brightness with the briMultiplier factor
byte scaledBri2(byte in)
{
  uint16_t val = ((uint16_t)in*tkr_anim->briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


void mAnimatorLight::notify(byte callMode, bool followUp)
{
  if (!tkr_anim->udpConnected) return;
  if (!tkr_anim->syncGroups) return;
  switch (callMode)
  {
    case CALL_MODE_INIT:          return;
    case CALL_MODE_DIRECT_CHANGE: if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_BUTTON:        if (!tkr_anim->notifyButton) return; break;
    case CALL_MODE_BUTTON_PRESET: if (!tkr_anim->notifyButton) return; break;
    case CALL_MODE_NIGHTLIGHT:    if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_HUE:           if (!tkr_anim->notifyHue)    return; break;
    case CALL_MODE_PRESET_CYCLE:  if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_ALEXA:         if (!tkr_anim->notifyAlexa)  return; break;
    default: return;
  }
  byte udpOut[WLEDPACKETSIZE];
  mAnimatorLight::Segment& mainseg = tkr_anim->getMainSegment();
  udpOut[0] = 0; //0: wled notifier protocol 1: WARLS protocol
  udpOut[1] = callMode;
  udpOut[2] = tkr_iLight->getBri_Global();
  uint32_t col = mainseg.segcol[0].getU32();
  udpOut[3] = R(col);
  udpOut[4] = G(col);
  udpOut[5] = B(col);
  udpOut[6] = tkr_anim->nightlightActive;
  udpOut[7] = tkr_anim->nightlightDelayMins;
  udpOut[8] = mainseg.effect_id;
  udpOut[9] = mainseg.speed;
  udpOut[10] = W(col);
  //compatibilityVersionByte:
  //0: old 1: supports white 2: supports secondary color
  //3: supports FX intensity, 24 byte packet 4: supports transitionDelay 5: sup palette
  //6: supports timebase syncing, 29 byte packet 7: supports tertiary color 8: supports sys time sync, 36 byte packet
  //9: supports sync groups, 37 byte packet 10: supports CCT, 39 byte packet 11: per segment options, variable packet length (40+MAX_NUM_SEGMENTS*3)
  //12: enhanced effct sliders, 2D & mapping options
  udpOut[11] = 12;
  col = mainseg.segcol[1].getU32();
  udpOut[12] = R(col);
  udpOut[13] = G(col);
  udpOut[14] = B(col);
  udpOut[15] = W(col);
  udpOut[16] = mainseg.intensity;
  udpOut[17] = (tkr_anim->transitionDelay >> 0) & 0xFF;
  udpOut[18] = (tkr_anim->transitionDelay >> 8) & 0xFF;
  udpOut[19] = mainseg.palette_id;
  col = mainseg.segcol[2].getU32();
  udpOut[20] = R(col);
  udpOut[21] = G(col);
  udpOut[22] = B(col);
  udpOut[23] = W(col);

  udpOut[24] = followUp;
  uint32_t t = millis() + tkr_anim->timebase;
  udpOut[25] = (t >> 24) & 0xFF;
  udpOut[26] = (t >> 16) & 0xFF;
  udpOut[27] = (t >>  8) & 0xFF;
  udpOut[28] = (t >>  0) & 0xFF;

  //sync system time
  udpOut[29] = tkr_anim->toki.getTimeSource();
  Toki::Time tm = tkr_anim->toki.getTime();
  uint32_t unix = tm.sec;
  udpOut[30] = (unix >> 24) & 0xFF;
  udpOut[31] = (unix >> 16) & 0xFF;
  udpOut[32] = (unix >>  8) & 0xFF;
  udpOut[33] = (unix >>  0) & 0xFF;
  uint16_t ms = tm.ms;
  udpOut[34] = (ms >> 8) & 0xFF;
  udpOut[35] = (ms >> 0) & 0xFF;

  //sync groups
  udpOut[36] = tkr_anim->syncGroups;

  //Might be changed to Kelvin in the future, receiver code should handle that case
  //0: byte 38 contains 0-255 value, 255: no valid CCT, 1-254: Kelvin value MSB
  udpOut[37] = tkr_anim->hasCCTBus() ? 0 : 255; //check this is 0 for the next value to be significant
  udpOut[38] = mainseg.cct_slider;

  udpOut[39] = tkr_anim->getActiveSegmentsNum();
  udpOut[40] = UDP_SEG_SIZE; //size of each loop iteration (one segment)
  size_t s = 0, nsegs = tkr_anim->getSegmentsNum();
  for (size_t i = 0; i < nsegs; i++) {
    mAnimatorLight::Segment &selseg = tkr_anim->getSegment(i);
    if (!selseg.isActive()) continue;
    uint16_t ofs = 41 + s*UDP_SEG_SIZE; //start of segment offset byte
    udpOut[0 +ofs] = s;
    udpOut[1 +ofs] = selseg.start >> 8;
    udpOut[2 +ofs] = selseg.start & 0xFF;
    udpOut[3 +ofs] = selseg.stop >> 8;
    udpOut[4 +ofs] = selseg.stop & 0xFF;
    udpOut[5 +ofs] = selseg.grouping_get();
    udpOut[6 +ofs] = selseg.spacing;
    udpOut[7 +ofs] = selseg.offset >> 8;
    udpOut[8 +ofs] = selseg.offset & 0xFF;
    udpOut[9 +ofs] = selseg.options & 0x8F; //only take into account selected, mirrored, on, reversed, reverse_y (for 2D); ignore freeze, reset, transitional
    udpOut[10+ofs] = 255;//selseg.opacity;
    udpOut[11+ofs] = selseg.effect_id;
    udpOut[12+ofs] = selseg.speed;
    udpOut[13+ofs] = selseg.intensity;
    udpOut[14+ofs] = selseg.palette_id;
    udpOut[15+ofs] = R(selseg.segcol[0].getU32());
    udpOut[16+ofs] = G(selseg.segcol[0].getU32());
    udpOut[17+ofs] = B(selseg.segcol[0].getU32());
    udpOut[18+ofs] = W(selseg.segcol[0].getU32());
    udpOut[19+ofs] = R(selseg.segcol[1].getU32());
    udpOut[20+ofs] = G(selseg.segcol[1].getU32());
    udpOut[21+ofs] = B(selseg.segcol[1].getU32());
    udpOut[22+ofs] = W(selseg.segcol[1].getU32());
    udpOut[23+ofs] = R(selseg.segcol[2].getU32());
    udpOut[24+ofs] = G(selseg.segcol[2].getU32());
    udpOut[25+ofs] = B(selseg.segcol[2].getU32());
    udpOut[26+ofs] = W(selseg.segcol[2].getU32());
    udpOut[27+ofs] = selseg.cct_slider;
    udpOut[28+ofs] = (selseg.options>>8) & 0xFF; //mirror_y, transpose, 2D mapping & sound
    udpOut[29+ofs] = selseg.custom1;
    udpOut[30+ofs] = selseg.custom2;
    udpOut[31+ofs] = selseg.custom3 | (selseg.check1<<5) | (selseg.check2<<6) | (selseg.check3<<7);
    udpOut[32+ofs] = selseg.startY >> 8;
    udpOut[33+ofs] = selseg.startY & 0xFF;
    udpOut[34+ofs] = selseg.stopY >> 8;
    udpOut[35+ofs] = selseg.stopY & 0xFF;
    ++s;
  }

  //uint16_t offs = SEG_OFFSET;
  //next value to be added has index: udpOut[offs + 0]

  IPAddress broadcastIp;
  broadcastIp = ~uint32_t(Network.subnetMask()) | uint32_t(Network.gatewayIP());

  notifierUdp.beginPacket(broadcastIp, udpPort);
  notifierUdp.write(udpOut, WLEDPACKETSIZE);
  notifierUdp.endPacket();
  notificationSentCallMode = callMode;
  notificationSentTime = millis();
  notificationCount = followUp ? notificationCount + 1 : 0;
}


#ifdef ENABLE_FEATURE_LIGHTING__WEBUI


void realtimeLock2(uint32_t timeoutMs, byte md)
{
  if (!tkr_anim->realtimeMode && !tkr_anim->realtimeOverride) {
    uint16_t stop, start;
    if (tkr_anim->useMainSegmentOnly) {
      mAnimatorLight::Segment& mainseg = tkr_anim->getMainSegment();
      start = mainseg.start;
      stop  = mainseg.stop;
      mainseg.freeze = true;
    } else {
      start = 0;
      stop  = tkr_anim->getLengthTotal();
    }
    // clear strip/segment
    for (size_t i = start; i < stop; i++) tkr_anim->setPixelColor(i,BLACK);
    // if WLED was off and using main segment only, freeze non-main segments so they stay off
    if (tkr_anim->useMainSegmentOnly && tkr_iLight->getBri_Global() == 0) {
      for (size_t s=0; s < tkr_anim->getSegmentsNum(); s++) {
        tkr_anim->getSegment(s).freeze = true;
      }
    }
  }
  // if strip is off (bri==0) and not already in RTM
  if (tkr_anim->briT == 0 && !tkr_anim->realtimeMode && !tkr_anim->realtimeOverride) {
    tkr_anim->setBrightness(scaledBri2(tkr_anim->briLast), true);
  }

  if (tkr_anim->realtimeTimeout != UINT32_MAX) {
    tkr_anim->realtimeTimeout = (timeoutMs == 255001 || timeoutMs == 65000) ? UINT32_MAX : millis() + timeoutMs;
  }
  tkr_anim->realtimeMode = md;

  if (tkr_anim->realtimeOverride) return;
  if (tkr_anim->arlsForceMaxBri) tkr_anim->setBrightness(scaledBri2(255), true);
  if (tkr_anim->briT > 0 && md == REALTIME_MODE_GENERIC) tkr_anim->show();
}

void exitRealtime2() {
  if (!tkr_anim->realtimeMode) return;
  if (tkr_anim->realtimeOverride == REALTIME_OVERRIDE_ONCE) tkr_anim->realtimeOverride = REALTIME_OVERRIDE_NONE;
  tkr_anim->setBrightness(scaledBri2(tkr_iLight->getBri_Global()), true);
  tkr_anim->realtimeTimeout = 0; // cancel realtime mode immediately
  tkr_anim->realtimeMode = REALTIME_MODE_INACTIVE; // inform UI immediately
  tkr_anim->realtimeIP[0] = 0;
  if (tkr_anim->useMainSegmentOnly) { // unfreeze live segment again
    tkr_anim->getMainSegment().freeze = false;
  }
  // updateInterfaces(CALL_MODE_WS_SEND);
}


#define TMP2NET_OUT_PORT 65442

void sendTPM2Ack() {
  tkr_anim->notifierUdp.beginPacket(tkr_anim->notifierUdp.remoteIP(), TMP2NET_OUT_PORT);
  uint8_t response_ack = 0xac;
  tkr_anim->notifierUdp.write(&response_ack, 1);
  tkr_anim->notifierUdp.endPacket();
}


void handleNotifications()
{
  IPAddress localIP;

  //send second notification if enabled
  if(tkr_anim->udpConnected && (tkr_anim->notificationCount < tkr_anim->udpNumRetries) && ((millis()-tkr_anim->notificationSentTime) > 250)){
    // notify(tkr_anim->notificationSentCallMode,true);
  }

  if (tkr_anim->e131NewData && millis() - tkr_anim->getLastShow() > 15)
  {
    tkr_anim->e131NewData = false;
    tkr_anim->show();
  }

  //unlock strip when realtime UDP times out
  if (tkr_anim->realtimeMode && millis() > tkr_anim->realtimeTimeout) exitRealtime2();

  //receive UDP notifications
  if (!tkr_anim->udpConnected) return;

  bool isSupp = false;
  size_t packetSize = tkr_anim->notifierUdp.parsePacket();
  if (!packetSize && tkr_anim->udp2Connected) {
    packetSize = tkr_anim->notifier2Udp.parsePacket();
    isSupp = true;
  }

  //hyperion / raw RGB
  if (!packetSize && tkr_anim->udpRgbConnected) {
    packetSize = tkr_anim->rgbUdp.parsePacket();
    if (packetSize) {
      if (!tkr_anim->receiveDirect) return;
      if (packetSize > UDP_IN_MAXSIZE || packetSize < 3) return;
      tkr_anim->realtimeIP = tkr_anim->rgbUdp.remoteIP();
      DEBUG_PRINTLN( tkr_anim->rgbUdp.remoteIP());
      uint8_t lbuf[packetSize];
      tkr_anim->rgbUdp.read(lbuf, packetSize);
      realtimeLock2(tkr_anim->realtimeTimeoutMs, REALTIME_MODE_HYPERION);
      if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;
      uint16_t id = 0;
      uint16_t totalLen = tkr_anim->getLengthTotal();
      for (size_t i = 0; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, lbuf[i], lbuf[i+1], lbuf[i+2], 0);
        id++; if (id >= totalLen) break;
      }
      if (!(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) tkr_anim->show();
      return;
    }
  }

  if (!(tkr_anim->receiveNotifications || tkr_anim->receiveDirect)) return;

  localIP = Network.localIP();
  //notifier and UDP realtime
  if (!packetSize || packetSize > UDP_IN_MAXSIZE) return;
  if (!isSupp && tkr_anim->notifierUdp.remoteIP() == localIP) return; //don't process broadcasts we send ourselves

  uint8_t udpIn[packetSize +1];
  uint16_t len;
  if (isSupp) len = tkr_anim->notifier2Udp.read(udpIn, packetSize);
  else        len =  tkr_anim->notifierUdp.read(udpIn, packetSize);

  // // WLED nodes info notifications
  // if (isSupp && udpIn[0] == 255 && udpIn[1] == 1 && len >= 40) {
  //   if (!tkr_anim->nodeListEnabled || tkr_anim->notifier2Udp.remoteIP() == localIP) return;

  //   uint8_t unit = udpIn[39];
  //   NodesMap::iterator it = Nodes.find(unit);
  //   if (it == Nodes.end() && Nodes.size() < WLED_MAX_NODES) { // Create a new element when not present
  //     Nodes[unit].age = 0;
  //     it = Nodes.find(unit);
  //   }

  //   if (it != Nodes.end()) {
  //     for (size_t x = 0; x < 4; x++) {
  //       it->second.ip[x] = udpIn[x + 2];
  //     }
  //     it->second.age = 0; // reset 'age counter'
  //     char tmpNodeName[33] = { 0 };
  //     memcpy(&tmpNodeName[0], reinterpret_cast<byte *>(&udpIn[6]), 32);
  //     tmpNodeName[32]     = 0;
  //     it->second.nodeName = tmpNodeName;
  //     it->second.nodeName.trim();
  //     it->second.nodeType = udpIn[38];
  //     uint32_t build = 0;
  //     if (len >= 44)
  //       for (size_t i=0; i<sizeof(uint32_t); i++)
  //         build |= udpIn[40+i]<<(8*i);
  //     it->second.build = build;
  //   }
  //   return;
  // }

  //wled notifier, ignore if realtime packets active
  if (udpIn[0] == 0 && !tkr_anim->realtimeMode && tkr_anim->receiveNotifications)
  {
    //ignore notification if received within a second after sending a notification ourselves
    if (millis() - tkr_anim->notificationSentTime < 1000) return;
    if (udpIn[1] > 199) return; //do not receive custom versions

    //compatibilityVersionByte:
    byte version = udpIn[11];

    // if we are not part of any sync group ignore message
    if (version < 9 || version > 199) {
      // legacy senders are treated as if sending in sync group 1 only
      if (!(tkr_anim->receiveGroups & 0x01)) return;
    } else if (!(tkr_anim->receiveGroups & udpIn[36])) return;

    bool someSel = (tkr_anim->receiveNotificationBrightness || tkr_anim->receiveNotificationColor || tkr_anim->receiveNotificationEffects);

    //apply colors from notification to main segment, only if not syncing full segments
    if ((tkr_anim->receiveNotificationColor || !someSel) && (version < 11 || !tkr_anim->receiveSegmentOptions)) {
      // primary color, only apply white if intented (version > 0)
      tkr_anim->setColor(0, RGBW32(udpIn[3], udpIn[4], udpIn[5], (version > 0) ? udpIn[10] : 0));
      if (version > 1) {
        tkr_anim->setColor(1, RGBW32(udpIn[12], udpIn[13], udpIn[14], udpIn[15])); // secondary color
      }
      if (version > 6) {
        tkr_anim->setColor(2, RGBW32(udpIn[20], udpIn[21], udpIn[22], udpIn[23])); // tertiary color
        if (version > 9 && version < 200 && udpIn[37] < 255) { // valid CCT/Kelvin value
          uint16_t cct = udpIn[38];
          if (udpIn[37] > 0) { //Kelvin
            cct |= (udpIn[37] << 8);
          }
          tkr_anim->setCCT(cct);
        }
      }
    }

    bool timebaseUpdated = false;
    //apply effects from notification
    bool applyEffects = (tkr_anim->receiveNotificationEffects || !someSel);
    if (version < 200)
    {
      // if (applyEffects && currentPlaylist >= 0) unloadPlaylist();
      if (version > 10 && (tkr_anim->receiveSegmentOptions || tkr_anim->receiveSegmentBounds)) {
        uint8_t numSrcSegs = udpIn[39];
        for (size_t i = 0; i < numSrcSegs; i++) {
          uint16_t ofs = 41 + i*udpIn[40]; //start of segment offset byte
          uint8_t id = udpIn[0 +ofs];
          if (id > tkr_anim->getSegmentsNum()) break;

          mAnimatorLight::Segment& selseg = tkr_anim->getSegment(id);
          if (!selseg.isActive() || !selseg.isSelected()) continue; //do not apply to non selected segments

          uint16_t startY = 0, start  = (udpIn[1+ofs] << 8 | udpIn[2+ofs]);
          uint16_t stopY  = 1, stop   = (udpIn[3+ofs] << 8 | udpIn[4+ofs]);
          uint16_t offset = (udpIn[7+ofs] << 8 | udpIn[8+ofs]);
          if (!tkr_anim->receiveSegmentOptions) {
            selseg.setUp(start, stop, selseg.grouping_get(), selseg.spacing, offset, startY, stopY);
            continue;
          }
          //for (size_t j = 1; j<4; j++) selseg.setOption(j, (udpIn[9 +ofs] >> j) & 0x01); //only take into account mirrored, on, reversed; ignore selected
          selseg.options = (selseg.options & 0x0071U) | (udpIn[9 +ofs] & 0x0E); // ignore selected, freeze, reset & transitional
          // selseg.setOpacity(udpIn[10+ofs]);
          if (applyEffects) {
            tkr_anim->setEffect(id,  udpIn[11+ofs]);
            selseg.speed     = udpIn[12+ofs];
            selseg.intensity = udpIn[13+ofs];
            selseg.palette_id   = udpIn[14+ofs];
          }
          if (tkr_anim->receiveNotificationColor || !someSel) {
            selseg.setColor(0, RGBW32(udpIn[15+ofs],udpIn[16+ofs],udpIn[17+ofs],udpIn[18+ofs]));
            selseg.setColor(1, RGBW32(udpIn[19+ofs],udpIn[20+ofs],udpIn[21+ofs],udpIn[22+ofs]));
            selseg.setColor(2, RGBW32(udpIn[23+ofs],udpIn[24+ofs],udpIn[25+ofs],udpIn[26+ofs]));
            selseg.setCCT(udpIn[27+ofs]);
          }
          if (version > 11) {
            // when applying synced options ignore selected as it may be used as indicator of which segments to sync
            // freeze, reset & transitional should never be synced
            selseg.options = (selseg.options & 0x0071U) | (udpIn[28+ofs]<<8) | (udpIn[9 +ofs] & 0x8E); // ignore selected, freeze, reset & transitional
            if (applyEffects) {
              selseg.custom1 = udpIn[29+ofs];
              selseg.custom2 = udpIn[30+ofs];
              selseg.custom3 = udpIn[31+ofs] & 0x1F;
              selseg.check1  = (udpIn[31+ofs]>>5) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>6) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>7) & 0x1;
            }
            startY = (udpIn[32+ofs] << 8 | udpIn[33+ofs]);
            stopY  = (udpIn[34+ofs] << 8 | udpIn[35+ofs]);
          }
          if (tkr_anim->receiveSegmentBounds) {
            selseg.setUp(start, stop, udpIn[5+ofs], udpIn[6+ofs], offset, startY, stopY);
          } else {
            selseg.setUp(selseg.start, selseg.stop, udpIn[5+ofs], udpIn[6+ofs], selseg.offset, selseg.startY, selseg.stopY);
          }
        }
        tkr_anim->stateChanged = true;
      }

      // simple effect sync, applies to all selected segments
      if (applyEffects && (version < 11 || !tkr_anim->receiveSegmentOptions)) {
        for (size_t i = 0; i < tkr_anim->getSegmentsNum(); i++) {
          mAnimatorLight::Segment& seg = tkr_anim->getSegment(i);
          if (!seg.isActive() || !seg.isSelected()) continue;
          seg.setEffect(udpIn[8]);
          seg.speed =  udpIn[9];
          if (version > 2) seg.intensity = udpIn[16];
          if (version > 4) seg.setPalette(udpIn[19]);
        }
        tkr_anim->stateChanged = true;
      }

      if (applyEffects && version > 5) {
        uint32_t t = (udpIn[25] << 24) | (udpIn[26] << 16) | (udpIn[27] << 8) | (udpIn[28]);
        t += PRESUMED_NETWORK_DELAY; //adjust trivially for network delay
        t -= millis();
        tkr_anim->timebase = t;
        timebaseUpdated = true;
      }
    }

    //adjust system time, but only if sender is more accurate than self
    if (version > 7 && version < 200)
    {
      Toki::Time tm;
      tm.sec = (udpIn[30] << 24) | (udpIn[31] << 16) | (udpIn[32] << 8) | (udpIn[33]);
      tm.ms = (udpIn[34] << 8) | (udpIn[35]);
      if (udpIn[29] > tkr_anim->toki.getTimeSource()) { //if sender's time source is more accurate
        tkr_anim->toki.adjust(tm, PRESUMED_NETWORK_DELAY); //adjust trivially for network delay
        uint8_t ts = TOKI_TS_UDP;
        if (udpIn[29] > 99) ts = TOKI_TS_UDP_NTP;
        else if (udpIn[29] >= TOKI_TS_SEC) ts = TOKI_TS_UDP_SEC;
        tkr_anim->toki.setTime(tm, ts);
      } else if (timebaseUpdated && tkr_anim->toki.getTimeSource() > 99) { //if we both have good times, get a more accurate timebase
        Toki::Time myTime = tkr_anim->toki.getTime();
        uint32_t diff = tkr_anim->toki.msDifference(tm, myTime);
        tkr_anim->timebase -= PRESUMED_NETWORK_DELAY; //no need to presume, use difference between NTP times at send and receive points
        if (tkr_anim->toki.isLater(tm, myTime)) {
          tkr_anim->timebase += diff;
        } else {
          tkr_anim->timebase -= diff;
        }
      }
    }

    if (version > 3)
    {
      // tkr_anim->transitionDelayTemp = ((udpIn[17] << 0) & 0xFF) + ((udpIn[18] << 8) & 0xFF00);
    }

    tkr_anim->nightlightActive = udpIn[6];
    if (tkr_anim->nightlightActive) tkr_anim->nightlightDelayMins = udpIn[7];

    if (tkr_anim->receiveNotificationBrightness || !someSel) tkr_iLight->setBriRGB_Global( udpIn[2] );
    // stateUpdated(CALL_MODE_NOTIFICATION);
    return;
  }

  if (!tkr_anim->receiveDirect) return;

  //TPM2.NET
  if (udpIn[0] == 0x9c)
  {
    //WARNING: this code assumes that the final TMP2.NET payload is evenly distributed if using multiple packets (ie. frame size is constant)
    //if the number of LEDs in your installation doesn't allow that, please include padding bytes at the end of the last packet
    byte tpmType = udpIn[1];
    if (tpmType == 0xaa) { //TPM2.NET polling, expect answer
      sendTPM2Ack(); return;
    }
    if (tpmType != 0xda) return; //return if notTPM2.NET data

    tkr_anim->realtimeIP = (isSupp) ? tkr_anim->notifier2Udp.remoteIP() : tkr_anim->notifierUdp.remoteIP();
    realtimeLock2(tkr_anim->realtimeTimeoutMs, REALTIME_MODE_TPM2NET);
    if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;

    tkr_anim->tpmPacketCount++; //increment the packet count
    if (tkr_anim->tpmPacketCount == 1) tkr_anim->tpmPayloadFrameSize = (udpIn[2] << 8) + udpIn[3]; //save frame size for the whole payload if this is the first packet
    byte packetNum = udpIn[4]; //starts with 1!
    byte numPackets = udpIn[5];

    uint16_t id = (tkr_anim->tpmPayloadFrameSize/3)*(packetNum-1); //start LED
    uint16_t totalLen = tkr_anim->getLengthTotal();
    for (size_t i = 6; i < tkr_anim->tpmPayloadFrameSize + 4U; i += 3)
    {
      if (id < totalLen)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
      else break;
    }
    if (tkr_anim->tpmPacketCount == numPackets) //reset packet count and show if all packets were received
    {
      tkr_anim->tpmPacketCount = 0;
      tkr_anim->show();
    }
    return;
  }

  //UDP realtime: 1 warls 2 drgb 3 drgbw
  if (udpIn[0] > 0 && udpIn[0] < 5)
  {
    tkr_anim->realtimeIP = (isSupp) ? tkr_anim->notifier2Udp.remoteIP() : tkr_anim->notifierUdp.remoteIP();
    DEBUG_PRINTLN(tkr_anim->realtimeIP);
    if (packetSize < 2) return;

    if (udpIn[1] == 0)
    {
      tkr_anim->realtimeTimeout = 0;
      return;
    } else {
      realtimeLock2(udpIn[1]*1000 +1, REALTIME_MODE_UDP);
    }
    if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;

    uint16_t totalLen = tkr_anim->getLengthTotal();
    if (udpIn[0] == 1) //warls
    {
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3], 0);
      }
    } else if (udpIn[0] == 2) //drgb
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 3) //drgbw
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 4) //dnrgb
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 3)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
    } else if (udpIn[0] == 5) //dnrgbw
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 4)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);
        id++;
      }
    }
    tkr_anim->show();
    return;
  }

  // API over UDP
  udpIn[packetSize] = '\0';

  // if (requestJSONBufferLock(18)) {
  //   if (udpIn[0] >= 'A' && udpIn[0] <= 'Z') { //HTTP API
  //     String apireq = "win"; apireq += '&'; // reduce flash string usage
  //     apireq += (char*)udpIn;
  //     handle__HTTP__GET_QueryAPI(nullptr, apireq);
  //   } else if (udpIn[0] == '{') { //JSON API
  //     DeserializationError error = deserializeJson(doc, udpIn);
  //     JsonObject root = doc.as<JsonObject>();
  //     if (!error && !root.isNull()) deserializeState(root);
  //   }
  //   releaseJSONBufferLock();
  // }
}


void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w)
{
  uint16_t pix = i + tkr_anim->arlsOffset;
  if (pix < tkr_anim->getLengthTotal()) {
    if (!tkr_anim->arlsDisableGammaCorrection && gammaCorrectCol) {
      // r = tkr_anim->gamma8(r);
      // g = tkr_anim->gamma8(g);
      // b = tkr_anim->gamma8(b);
      // w = tkr_anim->gamma8(w);
    }
    if (tkr_anim->useMainSegmentOnly) {
      mAnimatorLight::Segment &seg = tkr_anim->getMainSegment();
      if (pix<seg.length()) seg.setPixelColor(pix, r, g, b, w);
    } else {
      // tkr_anim->setPixelColor(pix, r, g, b, w);
    }
  }
}


// start or stop transition for all segments
void mAnimatorLight::setTransitionMode(bool t) {
  suspend();
  waitForIt();
  for (Segment &seg : segments) seg.startTransition(t ? _transitionDur : 0);
  resume();
}


// wait until frame is over (service() has finished or time for 2 frames have passed; yield() crashes on 8266)
// the latter may, in rare circumstances, lead to incorrectly assuming strip is done servicing but will not block
// other processing "indefinitely"
// rare circumstances are: setting FPS to high number (i.e. 120) and have very slow effect that will need more
// time than 2 * _frametime (1000/FPS) to draw content
void mAnimatorLight::waitForIt() {
  unsigned long waitStart = millis();
  unsigned long maxWait = 2*getFrameTime() + 100; // TODO: this needs a proper fix for timeout! see #4779
  while (isServicing() && (millis() - waitStart < maxWait)) delay(1); // safe even when millis() rolls over
  #ifdef WLED_DEBUG
  if (millis()-waitStart >= maxWait) DEBUG_PRINTLN(F("Waited for strip to finish servicing."));
  #endif
};



/*********************************************************************************************\
   Refresh aging for remote units, drop if too old...
\*********************************************************************************************/
void refreshNodeList()
{
  // for (NodesMap::iterator it = Nodes.begin(); it != Nodes.end();) {
  //   bool mustRemove = true;

  //   if (it->second.ip[0] != 0) {
  //     if (it->second.age < 10) {
  //       it->second.age++;
  //       mustRemove = false;
  //       ++it;
  //     }
  //   }

  //   if (mustRemove) {
  //     it = Nodes.erase(it);
  //   }
  // }
}

/*********************************************************************************************\
   Broadcast system info to other nodes. (to update node lists)
\*********************************************************************************************/
void sendSysInfoUDP()
{
  if (!tkr_anim->udp2Connected) return;

  IPAddress ip = Network.localIP();
  if (!ip || ip == IPAddress(255,255,255,255)) ip = IPAddress(4,3,2,1);

  // TODO: make a nice struct of it and clean up
  //  0: 1 byte 'binary token 255'
  //  1: 1 byte id '1'
  //  2: 4 byte ip
  //  6: 32 char name
  // 38: 1 byte node type id
  // 39: 1 byte node id
  // 40: 4 byte version ID
  // 44 bytes total

  // send my info to the world...
  uint8_t data[44] = {0};
  data[0] = 255;
  data[1] = 1;

  for (size_t x = 0; x < 4; x++) {
    data[x + 2] = ip[x];
  }
  memcpy((byte *)data + 6, tkr_anim->serverDescription, 32);
  #ifdef ESP8266
  data[38] = NODE_TYPE_ID_ESP8266;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
  data[38] = NODE_TYPE_ID_ESP32C3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
  data[38] = NODE_TYPE_ID_ESP32S3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
  data[38] = NODE_TYPE_ID_ESP32S2;
  #elif defined(ARDUINO_ARCH_ESP32)
  data[38] = 32;//NODE_TYPE_ID_ESP32;
  #else
  data[38] = NODE_TYPE_ID_UNDEFINED;
  #endif
  data[39] = ip[3]; // unit ID == last IP number

  uint32_t build = PROJECT_VERSION;
  for (size_t i=0; i<sizeof(uint32_t); i++)
    data[40+i] = (build>>(8*i)) & 0xFF;

  IPAddress broadcastIP(255, 255, 255, 255);
  tkr_anim->notifier2Udp.beginPacket(broadcastIP, tkr_anim->udpPort2);
  tkr_anim->notifier2Udp.write(data, sizeof(data));
  tkr_anim->notifier2Udp.endPacket();
}


/*********************************************************************************************\
 * Art-Net, DDP, E131 output - work in progress
\*********************************************************************************************/

#define DDP_HEADER_LEN 10
#define DDP_SYNCPACKET_LEN 10

#define DDP_FLAGS1_VER 0xc0  // version mask
#define DDP_FLAGS1_VER1 0x40 // version=1
#define DDP_FLAGS1_PUSH 0x01
#define DDP_FLAGS1_QUERY 0x02
#define DDP_FLAGS1_REPLY 0x04
#define DDP_FLAGS1_STORAGE 0x08
#define DDP_FLAGS1_TIME 0x10

#define DDP_ID_DISPLAY 1
#define DDP_ID_CONFIG 250
#define DDP_ID_STATUS 251

// 1440 channels per packet
#define DDP_CHANNELS_PER_PACKET 1440 // 480 leds

//
// Send real time UDP updates to the specified client
//
// type   - protocol type (0=DDP, 1=E1.31, 2=ArtNet)
// client - the IP address to send to
// length - the number of pixels
// buffer - a buffer of at least length*4 bytes long
// isRGBW - true if the buffer contains 4 components per pixel

static       size_t sequenceNumber = 0; // this needs to be shared across all outputs
static const size_t ART_NET_HEADER_SIZE = 12;
static const byte   ART_NET_HEADER[] PROGMEM = {0x41,0x72,0x74,0x2d,0x4e,0x65,0x74,0x00,0x00,0x50,0x00,0x0e};

uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri, bool isRGBW)  {
  // if (!(apActive || interfacesInited) || !client[0] || !length) return 1;  // network not initialised or dummy/unset IP address  031522 ajn added check for ap

  WiFiUDP ddpUdp;

  switch (type) {
    case 0: // DDP
    {
      // // calculate the number of UDP packets we need to send
      // size_t channelCount = length * (isRGBW? 4:3); // 1 channel for every R,G,B value
      // size_t packetCount = ((channelCount-1) / DDP_CHANNELS_PER_PACKET) +1;

      // // there are 3 channels per RGB pixel
      // uint32_t channel = 0; // TODO: allow specifying the start channel
      // // the current position in the buffer
      // size_t bufferOffset = 0;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {
      //   if (sequenceNumber > 15) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, DDP_DEFAULT_PORT)) {  // port defined in ESPAsyncE131.h
      //     DEBUG_PRINTLN(F("WiFiUDP.beginPacket returned an error"));
      //     return 1; // problem
      //   }

      //   // the amount of data is AFTER the header in the current packet
      //   size_t packetSize = DDP_CHANNELS_PER_PACKET;

      //   uint8_t flags = DDP_FLAGS1_VER1;
      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet, set the push flag
      //     // TODO: determine if we want to send an empty push packet to each destination after sending the pixel data
      //     flags = DDP_FLAGS1_VER1 | DDP_FLAGS1_PUSH;
      //     if (channelCount % DDP_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % DDP_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   // write the header
      //   /*0*/ddpUdp.write(flags);
      //   /*1*/ddpUdp.write(sequenceNumber++ & 0x0F); // sequence may be unnecessary unless we are sending twice (as requested in Sync settings)
      //   /*2*/ddpUdp.write(isRGBW ?  DDP_TYPE_RGBW32 : DDP_TYPE_RGB24);
      //   /*3*/ddpUdp.write(DDP_ID_DISPLAY);
      //   // data offset in bytes, 32-bit number, MSB first
      //   /*4*/ddpUdp.write(0xFF & (channel >> 24));
      //   /*5*/ddpUdp.write(0xFF & (channel >> 16));
      //   /*6*/ddpUdp.write(0xFF & (channel >>  8));
      //   /*7*/ddpUdp.write(0xFF & (channel      ));
      //   // data length in bytes, 16-bit number, MSB first
      //   /*8*/ddpUdp.write(0xFF & (packetSize >> 8));
      //   /*9*/ddpUdp.write(0xFF & (packetSize     ));

      //   // write the colors, the write write(const uint8_t *buffer, size_t size)
      //   // function is just a loop internally too
      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("WiFiUDP.endPacket returned an error"));
      //     return 1; // problem
      //   }

      //   channel += packetSize;
      // }
    } break;

    case 1: //E1.31
    {
    } break;

    case 2: //ArtNet
    {
      // // calculate the number of UDP packets we need to send
      // const size_t channelCount = length * (isRGBW?4:3); // 1 channel for every R,G,B,(W?) value
      // const size_t ARTNET_CHANNELS_PER_PACKET = isRGBW?512:510; // 512/4=128 RGBW LEDs, 510/3=170 RGB LEDs
      // const size_t packetCount = ((channelCount-1)/ARTNET_CHANNELS_PER_PACKET)+1;

      // uint32_t channel = 0; 
      // size_t bufferOffset = 0;

      // sequenceNumber++;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {

      //   if (sequenceNumber > 255) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, ARTNET_DEFAULT_PORT)) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.beginPacket returned an error"));
      //     return 1; // borked
      //   }

      //   size_t packetSize = ARTNET_CHANNELS_PER_PACKET;

      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet
      //     if (channelCount % ARTNET_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % ARTNET_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   byte header_buffer[ART_NET_HEADER_SIZE];
      //   memcpy_P(header_buffer, ART_NET_HEADER, ART_NET_HEADER_SIZE);
      //   ddpUdp.write(header_buffer, ART_NET_HEADER_SIZE); // This doesn't change. Hard coded ID, OpCode, and protocol version.
      //   ddpUdp.write(sequenceNumber & 0xFF); // sequence number. 1..255
      //   ddpUdp.write(0x00); // physical - more an FYI, not really used for anything. 0..3
      //   ddpUdp.write((currentPacket) & 0xFF); // Universe LSB. 1 full packet == 1 full universe, so just use current packet number.
      //   ddpUdp.write(0x00); // Universe MSB, unused.
      //   ddpUdp.write(0xFF & (packetSize >> 8)); // 16-bit length of channel data, MSB
      //   ddpUdp.write(0xFF & (packetSize     )); // 16-bit length of channel data, LSB

      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.endPacket returned an error"));
      //     return 1; // borked
      //   }
      //   channel += packetSize;
      // }
    } break;
  }
  return 0;
}





// legacy method, applies values from col, effectCurrent, ... to selected segments
void mAnimatorLight::colorUpdated(byte callMode) {
  // applyValuesToSelectedSegs();
  // stateUpdated(callMode);
}



#endif // ENABLE_FEATURE_LIGHTING__WEBUI








#endif //USE_MODULE_LIGHTS_ANIMATOR
