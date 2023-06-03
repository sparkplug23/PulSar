#include "mHardwarePins.h"

/**
 * Temporary file for copying over functions as I completely redo
 * */


/**
 * @brief Function reads templates from progmem if available, then calls TemplateParser
 * @note JsonParser needs to have the buffer on the stack so it can be edited (token replacing), so must be read from progmem
 * @return bool if there was a template in progmem
 * */
bool mHardwarePins::ReadModuleTemplateFromProgmem(){

  #ifdef USE_MODULE_TEMPLATE
    uint16_t progmem_size = sizeof(MODULE_TEMPLATE);
    progmem_size = progmem_size>MODULE_TEMPLATE_MAX_SIZE?MODULE_TEMPLATE_MAX_SIZE:progmem_size;
    // create parse buffer
    char buffer[progmem_size];
    // Read into local
    memcpy_P(buffer,MODULE_TEMPLATE,sizeof(MODULE_TEMPLATE));

    #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("MODULE_TEMPLATE Load"));// = \"%s\""), buffer);
    AddLog(LOG_LEVEL_DEBUG, PSTR("Load = \"%s\""), buffer);
    #endif // ENABLE_LOG_LEVEL_INFO
    #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

    #ifdef ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
    ALOG_INF( PSTR("mHardwarePins::ReadModuleTemplateFromProgmem [%d bytes] = \"%s\""), progmem_size, buffer);
    #endif // ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH

    ModuleTemplateJsonParser(buffer);
    return true;
  #else 
    return false;
  #endif // MODULE_TEMPLATE
  
}

/**
 * @brief Parses the template
 * @param buffer template as string 
 * */
void mHardwarePins::ModuleTemplateJsonParser(char* buffer){

  JsonParser parser(buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR, PSTR("DeserializationError with \"%s\""),buffer);
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  } else{
    // AddLog(LOG_LEVEL_ERROR, PSTR("DeserializationSuccess with \"%s\""),buffer);
  }
  JsonParserToken jtok = 0; 
  
  if(jtok = obj[PM_JSON_NAME]){
    const char* name_ctr = jtok.getStr();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "system_name %s"),name_ctr);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    // snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr); 
    snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s",name_ctr);  
  }

  // delay(5000);

  // Name means friendly name (max 20 chars)
  if(jtok = obj[PM_JSON_FRIENDLYNAME]){
    const char* name_ctr = jtok.getStr();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template NAME %s"),name_ctr);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr);
    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template Name %s"),name_ctr);
    //snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  }

  /**
   * @brief If either GPIO method is configured then clear the previous
   * 
   * @param obj 
   */
  if((obj[PM_JSON_GPIOC])||(obj[D_JSON_GPIO_FUNCTION]))
  {
    pCONT_set->boot_status.module_template_parse_success = true;
    for(int ii=0;ii<ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io);ii++){
      pCONT_set->Settings.user_template2.hardware.gp.io[ii] = GPIO_NONE_ID;
    }
  }


  if(jtok = obj[PM_JSON_GPIOC])
  {
    
    int8_t  real_pin = -1;
    int16_t gpio_number = -1;
          // AddLog(LOG_LEVEL_INFO, PSTR("jtok.size()=%d"),jtok.size());

          // debug_debug_delay(3000); 


    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      real_pin = GetRealPinNumberFromName(key);
      ALOG_DBM(PSTR("KEY%d %s %d\n\r"), pair_index, key, real_pin);

      int8_t index_pin = ConvertRealPinToIndexPin(real_pin);
      ALOG_DBM( PSTR("real_pin%d->index_pin%d\n\r"), real_pin, index_pin);
      
      if(index_pin>=0){
      
        jtok.nextOne();
        const char* value = jtok.getStr();
        gpio_number = GetGPIOFunctionIDbyName(value);
        // AddLog(LOG_LEVEL_INFO, PSTR("gpio_number=%d\n\r"), gpio_number);

        if(gpio_number != -1){
          // only template pins
          pCONT_set->Settings.user_template2.hardware.gp.io[index_pin] = gpio_number; // non adjusted pin_number
          // FULL pin list
          // pCONT_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          AddLog(LOG_LEVEL_INFO, PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            pCONT_set->Settings.user_template2.hardware.gp.io[index_pin]
          );
          #endif
          // AddLog(LOG_LEVEL_INFO, PSTR("pin_number/indexed=%d %d, gpio_number=%d"), pin_number, pin_number_array_index, gpio_number);
        }else{
    #ifdef ENABLE_LOG_LEVEL_ERROR
          AddLog(LOG_LEVEL_ERROR, PSTR("DECODE ERROR \"%s\" %d"),value, gpio_number);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
        }

      }// end UsuableGPIOPin

    }

  }

  // New method that assumes the function is the key, and the pin(s) are the values
  // pins may be represented as single ints, or array of ints (e.g. Lighting pins)
  if(jtok = obj[D_JSON_GPIO_FUNCTION])
  { 

    ALOG_INF(PSTR("Found %s"), D_JSON_GPIO_FUNCTION);
    
    int8_t  real_pin = -1;
    int16_t gpio_function_id = -1;

    uint8_t jsonpair_count = jtok.size();
    
    AddLog(LOG_LEVEL_INFO, PSTR("jsonpair_count=%d"), jsonpair_count);

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++)
    {
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      // ALOG_INF(PSTR("KEY%d %s\n\r"), pair_index, key);

      gpio_function_id = pCONT_pins->GetGPIOFunctionIDbyName(key);

      ALOG_INF(PSTR("KEY%d %s GPIO_ID %d\n\r"), pair_index, key, gpio_function_id);
      
      
      if(gpio_function_id>=0)
      {
    
        jtok.nextOne();
        const char* value = jtok.getStr();
        
        real_pin = GetRealPinNumberFromName(value);
        ALOG_INF(PSTR("value %d %s %d\n\r"), pair_index, value, real_pin);

        int8_t index_pin = ConvertRealPinToIndexPin(real_pin);
        ALOG_INF( PSTR("real_pin%d->index_pin%d\n\r"), real_pin, index_pin);

        if(index_pin != -1){
          // only template pins
          pCONT_set->Settings.user_template2.hardware.gp.io[index_pin] = gpio_function_id; // non adjusted pin_number
          // FULL pin list
          // pCONT_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          AddLog(LOG_LEVEL_INFO, PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            pCONT_set->Settings.user_template2.hardware.gp.io[index_pin]
          );
          #endif
          // AddLog(LOG_LEVEL_INFO, PSTR("pin_number/indexed=%d %d, gpio_function_id=%d"), pin_number, pin_number_array_index, gpio_function_id);
        }
        else
        {
          #ifdef ENABLE_LOG_LEVEL_ERROR
          AddLog(LOG_LEVEL_ERROR, PSTR("DECODE ERROR \"%s\" %d"),value, gpio_function_id);
          #endif // ENABLE_LOG_LEVEL_COMMANDS
        }

      }// end

    }

    delay(3000);

  }
    
    
    
  if(jtok = obj[PM_JSON_BASE]){
    
    const char* base_ctr = jtok.getStr();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_DBM( PSTR(D_LOG_CONFIG "Template BASE %s"),base_ctr);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    pCONT_set->Settings.module = GetModuleIDbyName(base_ctr);
    char buffer[40];
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("Settings.module=%s"),GetModuleNameByID(pCONT_set->Settings.module, buffer));
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    //snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr);
  }else{
    pCONT_set->Settings.module = USER_MODULE;
  }

  
  // Name means friendly name (max 20 chars)
  if(jtok = obj["RoomHint"]){
    const char* name_ctr = jtok.getStr();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template RoomHint %s"),name_ctr);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

    snprintf(pCONT_set->Settings.room_hint,sizeof(pCONT_set->Settings.room_hint),"%s",name_ctr);


    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template Name %s"),name_ctr);
    //snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  }




  // ModuleSettings_FlashSerial();

// ParseModuleTemplate();

}

/**
 * @brief Reads any template GPIOs then reads any user set GPIOs
 * */
void mHardwarePins::TemplateGPIOs(myio *gp)
{

  // Create an empty template to write gpio functions in to
  uint16_t *dest = (uint16_t *)gp;
  memset(dest, GPIO_NONE_ID, sizeof(myio));
  // for(int i=0;i<ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io);i++){ dest[i] = i; }
  // Create a source copy too
  uint16_t src[ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io)];
  
  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR("src size =%d"),ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));
  AddLog(LOG_LEVEL_DEBUG, PSTR("pCONT_set->Settings.module =%d"),pCONT_set->Settings.module);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  // Check if active module is simply a user_module, requiring no template reads
  if (pCONT_set->Settings.module == USER_MODULE) {
    // Simply read the user_configured gpio, skipping any progmem templates
    memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));
    #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR("TemplateGPIOs Loading from user_template2"));
    #endif // ENABLE_LOG_LEVEL_INFO
    #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  } 
  // Read templates from progmem, these will differ by esp8266, esp8285 and esp32
  else {
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR("TemplateGPIOs Loading from kModules progmem"));
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    // debug_debug_delay(3000);
    #ifdef ESP8266
      // GetInternalTemplate(&src, Settings.module, 1);
      memcpy_P(&src, &kModules[pCONT_set->Settings.module].gp, sizeof(mycfgio));
      // AddLog(LOG_LEVEL_WARN, PSTR("???????FORCED Settings.module = 0 from %d"),pCONT_set->Settings.module);
    #endif  // ESP8266
    #ifdef ESP32
      // memcpy_P(&src, &kModules[ModuleTemplate(Settings.module)].gp, sizeof(mycfgio));
      // AddLog(LOG_LEVEL_WARN, PSTR("FORCED Settings.module = 0 from %d"),pCONT_set->Settings.module);
      // pCONT_set->Settings.module = 0;
      memcpy_P(&src, &kModules[pCONT_set->Settings.module].gp, sizeof(mycfgio));
    #endif  // ESP32
  }

  // For extensive debugging, print the source and destination before copying
  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_Array(LOG_LEVEL_TEST, PSTR("TemplateGPIO:src"), src,  ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));
  AddLog_Array(LOG_LEVEL_TEST, PSTR("TemplateGPIO:dst"), dest, ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  uint8_t j = 0;
  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io); i++) {    
    dest[j] = src[i];    
    // AddLog(LOG_LEVEL_DEBUG, PSTR("Copying %d\ti%d\tp%d\t%d\ti%d"), dest[j],j, ConvertIndexPinToRealPin(dest[i]), src[i],i);

    
    #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    AddLog(LOG_LEVEL_DEBUG, PSTR("Copying dest=%d[%d]\t index/real = %d/%d"), 
      dest[j],j, 
      i, ConvertIndexPinToRealPin(i)
      );
    #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

    j++;
  }

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  AddLog_Array(LOG_LEVEL_TEST, PSTR("TemplateGPIO:dst2"), dest, ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));
  #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

}



/**
 * Takes GPIO/pins configured to internal module and configures pins output
 * */
void mHardwarePins::GpioInit(void)
{

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MODULE "GpioInit"));
  #endif // ENABLE_LOG_LEVEL_INFO

  uint16_t mgpio;

  /**
   * Part A: Checking module or setting to default based on chipset
   * */
  if (!ValidModule(pCONT_set->Settings.module)) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_MODULE "!ValidModule"));
    #endif // ENABLE_LOG_LEVEL_INFO
    uint8_t module = MODULE;
    if (!ValidModule(MODULE))
    { 
      module = MODULE_WEMOS_ID; 

      //esp32 ad esp8266 have different defaults

      // #ifdef ESP8266
      // module = SONOFF_BASIC;
      // #endif  // ESP8266
      // #ifdef ESP32
      //       module = WEMOS;
      // #endif  // ESP32


    }
    pCONT_set->Settings.module = module;
    pCONT_set->Settings.last_module = module;
  }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_MODULE "ValidModule %d"),pCONT_set->Settings.module);
    #endif // ENABLE_LOG_LEVEL_INFO
  }
  SetModuleType();

  /**
   * Part B: Fallback to base baudrate if module has changed
   * */
  if (pCONT_set->Settings.module != pCONT_set->Settings.last_module) {
    //pCONT_set->baudrate = APP_BAUDRATE;
  }

  /**
   * Part C: Correcting for invalid gpio functions
   * */
  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io); i++) {
    if(!ValidUserGPIOFunction(pCONT_set->Settings.user_template2.hardware.gp.io,i)){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CONFIG "!ValidUserGPIOFunction %d"),i);
    #endif // ENABLE_LOG_LEVEL_INFO
      pCONT_set->Settings.user_template2.hardware.gp.io[i] = GPIO_USER_ID;  // Fix not supported sensor ids in template    }
    }
  }

  // DEBUG_LINE_HERE;

  /**
   * @brief Part D: Read any template GPIO values. Function name needs changed!
   * 
   */
  myio def_gp;
  TemplateGPIOs(&def_gp); // Get template values

  // DEBUG_LINE_HERE;

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "ARRAY_SIZE%d"),ARRAY_SIZE(pCONT_set->Settings.module_pins.io));
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "def_gp[%d]=%d"),20,def_gp.io[20]);
  #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    
  // DEBUG_LINE_HERE;

  /**
   * @brief For all possible GPIO physical pins, populate gpio function if desired from TEMPLATE
   * 
   */
  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.module_pins.io); i++) 
  { //all pins
    
  // DEBUG_LINE_HERE;
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "%d=module_pins.io[%d]"),pCONT_set->Settings.module_pins.io[i],i);
    // #endif // ENABLE_LOG_LEVEL_INFO

    uint16_t gpio = pCONT_set->Settings.module_pins.io[i];
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "module_pins.io[%d]=%d"),i,pCONT_set->Settings.module_pins.io[i]);
    
    // If out of range, reset to none
    if(!ValidUserGPIOFunction(pCONT_set->Settings.module_pins.io,i)){
      pCONT_set->Settings.module_pins.io[i] = GPIO_NONE_ID;             // Fix not supported sensor ids in module
      ALOG_DBM(PSTR(D_LOG_CONFIG "Unsupported module_pins.io %d being reset to GPIO_NONE"),i);
    }
    // Set any user pins 
    else if (pCONT_set->Settings.module_pins.io[i] > GPIO_NONE_ID) {
      pCONT_set->my_module.io[i] = pCONT_set->Settings.module_pins.io[i];
      ALOG_DBM(PSTR(D_LOG_CONFIG "my_module.io[i] = %d"),i,pCONT_set->my_module.io[i]);
    }

    // Set any pins set in template
    if ((def_gp.io[i] >= GPIO_NONE_ID) && (def_gp.io[i] < GPIO_USER_ID)) { //ADDED >= to also copy NONE 
      pCONT_set->my_module.io[i] = def_gp.io[i];
      #ifndef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
      if(pCONT_set->my_module.io[i] > GPIO_NONE_ID){
      #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
      #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_DBM(PSTR(D_LOG_CONFIG "mio[i]=gio[i] %d %d index/real %d/%d \"%S\""),
        pCONT_set->my_module.io[i],
        def_gp.io[i],
        i,
        ConvertIndexPinToRealPin(i),
        GetGPIOFunctionNamebyID_P(pCONT_set->my_module.io[i])
      );
      #endif // ENABLE_LOG_LEVEL_INFO
      #ifndef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
      }
      #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    }
    else{
      ALOG_DBM(PSTR(D_LOG_CONFIG "Invalid IO in def_gp.io[%d]=%d"),i,def_gp.io[i]);
    // #ifdef ENABLE_LOG_LEVEL_ERROR
    //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "Invalid IO in def_gp.io[%d]=%d"),i,def_gp.io[i]);
    // #endif // ENABLE_LOG_LEVEL_COMMANDS
    }

  }
  
  // DEBUG_LINE_HERE;
  pCONT_set->my_module_flag = ModuleFlag();
  // DEBUG_LINE_HERE;

  #ifdef USE_DEBUG_DISABLE_GLOBAL_PIN_INIT
  AddLog(LOG_LEVEL_WARN,PSTR(D_LOG_MODULE "GpioInit DISABLED with \"USE_DEBUG_DISABLE_GLOBAL_PIN_INIT\""));
  return;
  #endif // USE_DEBUG_DISABLE_GLOBAL_PIN_INIT
  
  
  // DEBUG_LINE_HERE;
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE, "my_module.io", pCONT_set->my_module.io, ARRAY_SIZE(pCONT_set->my_module.io));

  // DEBUG_LINE_HERE;
  /**
   *  Take module io and configure pins
   * Unlike Tas, each pin function has its unique name maintained (ie SWT_INV for switch inverted maintained its ID, and it not saved simply as INV then shifted back to standard SWT. Internal classes must handle this)
   * */
  for (uint8_t index = 0; index < ARRAY_SIZE(pCONT_set->my_module.io); index++) 
  {

  // DEBUG_LINE_HERE;
  // uint8_t real_pin = UsablePinToTemplateArrayIndex(pCONT_set->my_module.io[i]);

//problems starts here

    uint8_t real_pin = ConvertIndexPinToRealPin(index);

  // pin needs adjusting to indexed value

  // uint8_t i = pin;
  // if()

  uint16_t gpio = pCONT_set->my_module.io[index];
  // uint8_t pin = i;
  // AddLog(LOG_LEVEL_DEBUG, PSTR("pin=%d, gpio=%d"), pin, gpio);

  // // Get GPIO if pin is set
  
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: %d %d %d"), i, pCONT_set->my_module.io[i], mgpio);

    mgpio = ValidPin_AdjustGPIO(index, gpio);

    //mgpio is really the gpio attached to pin if allowed
    
  // #ifdef ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD
  //   // pin_function = pCONT_set->my_module.io[i]; //function number
  //   // int8_t gpio_pin_number = mgpio;  //0+ if valid
  //   // SetPinFunction(gpio_pin_number, pin_function);  
  // #endif // ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

    ALOG_DBG( PSTR("DBG: real_pin=%d moduleIO=%d  mgpio=%d"), real_pin, pCONT_set->my_module.io[index], mgpio);
    
    // Phasing section out : moving into their modules
    if (mgpio) {         
      //PWM
      if ((mgpio >= GPIO_PWM1_INV_ID) && (mgpio < (GPIO_PWM1_INV_ID + MAX_PWMS))) {
        bitSet(pCONT_set->pwm_inverted, mgpio - GPIO_PWM1_INV_ID);
        mgpio -= (GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
      } 
    }    

    
    //new way
    if(mgpio){ SetPin(real_pin, mgpio); }                  // Anything above GPIO_NONE and below GPIO_SENSOR_END 
    
  }//end fof

  // DEBUG_LINE_HERE;

  #ifdef ESP8266
  /**
   * @brief The check for H801 should be phased out here, as its hardware specific, really the template should force this to happen anyway as TX==2
   * 
   */
  if ((2 == GetPin(GPIO_HWSERIAL0_TX_ID)) || (MODULE_H801_ID == pCONT_set->my_module_type)) 
  { 
    Serial.set_tx(2); 
    flag_serial_set_tx_set = true;
  }
  #endif

  // DEBUG_LINE_HERE;


/**
 * @brief Configure PWM if needed
 * 
 */
// leave as default for testing
  analogWriteRange(pCONT_set->Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(pCONT_set->Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
  // #endif

/**
 * @brief Configure SPI if needed
 * 
 */
// #ifdef USE_SPI

//   spi_flg = ((((GetPin(GPIO_SPI_CS] < 99) && (GetPin(GPIO_SPI_CS] > 14)) || (GetPin(GPIO_SPI_CS] < 12)) || (((GetPin(GPIO_SPI_DC] < 99) && (GetPin(GPIO_SPI_DC] > 14)) || (GetPin(GPIO_SPI_DC] < 12)));
//   if (spi_flg) {
//     for (uint16_t i = 0; i < GPIO_MAX; i++) {
//       if ((GetPin(i] >= 12) && (GetPin(i] <=14)) GetPin(i] = 99;
//     }
//     my_module.io[12] = GPIO_SPI_MISO;
//     GetPin(GPIO_SPI_MISO] = 12;
//     my_module.io[13] = GPIO_SPI_MOSI;
//     GetPin(GPIO_SPI_MOSI] = 13;
//     my_module.io[14] = GPIO_SPI_CLK;
//     GetPin(GPIO_SPI_CLK] = 14;
//   }
//   soft_spi_flg = ((GetPin(GPIO_SSPI_CS] < 99) && (GetPin(GPIO_SSPI_SCLK] < 99) && ((GetPin(GPIO_SSPI_MOSI] < 99) || (GetPin(GPIO_SSPI_MOSI] < 99)));
// #endif  // USE_SPI


// #ifdef USE_SPI
// #ifdef ESP8266
//   // if (!TasmotaGlobal.soft_spi_enabled) {
//   //   bool valid_cs = (ValidSpiPinUsed(GPIO_SPI_CS) ||
//   //                    ValidSpiPinUsed(GPIO_RC522_CS) ||
//   //                    (ValidSpiPinUsed(GPIO_NRF24_CS) && ValidSpiPinUsed(GPIO_NRF24_DC)) ||
//   //                    ValidSpiPinUsed(GPIO_ILI9341_CS) ||
//   //                    ValidSpiPinUsed(GPIO_ILI9341_DC) || // there are also boards without cs
//   //                    ValidSpiPinUsed(GPIO_EPAPER29_CS) ||
//   //                    ValidSpiPinUsed(GPIO_EPAPER42_CS) ||
//   //                    ValidSpiPinUsed(GPIO_ILI9488_CS) ||
//   //                    ValidSpiPinUsed(GPIO_SSD1351_CS) ||
//   //                    ValidSpiPinUsed(GPIO_RA8876_CS) ||
//   //                    ValidSpiPinUsed(GPIO_ST7789_DC) ||  // ST7789 CS may be omitted so chk DC too
//   //                    ValidSpiPinUsed(GPIO_ST7789_CS) ||
//   //                    (ValidSpiPinUsed(GPIO_SSD1331_CS) && ValidSpiPinUsed(GPIO_SSD1331_DC)) ||
//   //                    ValidSpiPinUsed(GPIO_SDCARD_CS)
//   //                   );
//   //   // If SPI_CS and/or SPI_DC is used they must be valid
//   //   TasmotaGlobal.spi_enabled = (valid_cs) ? SPI_MOSI_MISO : SPI_NONE;
//   //   if (TasmotaGlobal.spi_enabled) {
//   //     TasmotaGlobal.my_module.io[12] = AGPIO(GPIO_SPI_MISO);
//   //     SetPin(12, AGPIO(GPIO_SPI_MISO));
//   //     TasmotaGlobal.my_module.io[13] = AGPIO(GPIO_SPI_MOSI);
//   //     SetPin(13, AGPIO(GPIO_SPI_MOSI));
//   //     TasmotaGlobal.my_module.io[14] = AGPIO(GPIO_SPI_CLK);
//   //     SetPin(14, AGPIO(GPIO_SPI_CLK));
//   //   }
//   // }
// #endif  // ESP8266
// #ifdef ESP32
//   if (PinUsed(GPIO_SPI_CS) ||
//       PinUsed(GPIO_RC522_CS) ||
//       PinUsed(GPIO_NRF24_CS) ||
//       PinUsed(GPIO_ILI9341_CS) ||
//       PinUsed(GPIO_EPAPER29_CS) ||
//       PinUsed(GPIO_EPAPER42_CS) ||
//       PinUsed(GPIO_ILI9488_CS) ||
//       PinUsed(GPIO_SSD1351_CS) ||
//       PinUsed(GPIO_RA8876_CS) ||
//       PinUsed(GPIO_ST7789_DC) ||  // ST7789 CS may be omitted so chk DC too
//       PinUsed(GPIO_ST7789_CS) ||
//       PinUsed(GPIO_SSD1331_CS) ||
//       PinUsed(GPIO_SDCARD_CS)
//      ) {
//     uint32_t spi_mosi = (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MOSI)) ? SPI_MOSI : SPI_NONE;
//     uint32_t spi_miso = (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MISO)) ? SPI_MISO : SPI_NONE;
//     TasmotaGlobal.spi_enabled = spi_mosi + spi_miso;
//   }
// #endif  // ESP32
//   AddLogSpi(1, Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_MISO));
// #endif  // USE_SPI


// #else // ESP32
//   analogWriteFreqRange(0, Settings.pwm_frequency, Settings.pwm_range);

// #ifdef USE_SPI
//   spi_flg = (PinUsed(GPIO_SPI_CLK) && (PinUsed(GPIO_SPI_MOSI) || PinUsed(GPIO_SPI_MISO)));
//   soft_spi_flg = (PinUsed(GPIO_SSPI_SCLK) && (PinUsed(GPIO_SSPI_MOSI) || PinUsed(GPIO_SSPI_MISO)));
// #endif  // USE_SPI
// #endif  // ESP8266 - ESP32


/**
 * @brief Configure digital pin
 * 
 */
//new
// Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
  // Doing it here solves relay toggles at restart.
  for (uint32_t i = 0; 
                i < ARRAY_SIZE(pCONT_set->my_module.io);
                i++
  ){
    
    uint32_t mgpio = ValidPin_AdjustGPIO(i, pCONT_set->my_module.io[i]);
    
    ALOG_DBM( PSTR("INI: gpio pin %d, mgpio %d"), i, mgpio);
    
    if (((i < 6) || (i > 11)) && (0 == mgpio))
    {  // Skip SPI flash interface

      // if((i == 2)&&(pCONT_set->))

      // #ifndef USE_SERIAL_ALTERNATE_TX
        if (!((1 == i) || (3 == i))) {             // Skip serial
          if((MODULE_H801_ID == pCONT_set->my_module_type) && (i !=2 ))
          {
            pinMode(i, INPUT);
          }
        }
      // #endif
    }
  }

/**
 * @brief Configure I2C
 * 
 */
#ifdef USE_I2C
  pCONT_set->i2c_enabled = (PinUsed(GPIO_I2C_SCL_ID) && PinUsed(GPIO_I2C_SDA_ID));
  if (pCONT_set->i2c_enabled)
  { 
    if(pCONT_sup->wire == nullptr)
    {
      #ifdef ESP8266
      pCONT_sup->wire = new TwoWire();
      pCONT_sup->wire->begin(GetPin(GPIO_I2C_SDA_ID), GetPin(GPIO_I2C_SCL_ID)); // no return to check status
      #else
      pCONT_sup->wire = new TwoWire(0);
      // pCONT_sup->wire->setPins(GetPin(GPIO_I2C_SDA_ID), GetPin(GPIO_I2C_SCL_ID));
      ALOG_DBM( PSTR("Trying to start i2c 2-wire"));
      #ifdef ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
      if(pCONT_sup->wire->begin(GetPin(GPIO_I2C_SDA_ID), GetPin(GPIO_I2C_SCL_ID)))//, 100000))
      #else
      if(pCONT_sup->wire->begin(GetPin(GPIO_I2C_SDA_ID), GetPin(GPIO_I2C_SCL_ID), 100000))
      #endif // ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
      {
        ALOG_DBM( PSTR("STARTED to start i2c 2-wire sda%d scl%d"),GetPin(GPIO_I2C_SDA_ID),GetPin(GPIO_I2C_SCL_ID));
      }
      else
      {
        ALOG_DBM( PSTR("NOT STARTED to start i2c 2-wire"));
      }
      #endif


    }

// crap, deleted the begin?

    

  } // i2c_enabled
#endif  // USE_I2C





  /**
   *  Use pins to configure lights present
   * */
  pCONT_set->devices_present = 0;
  pCONT_set->Settings.light_settings.type = 0;//LT_BASIC;                     // Use basic PWM control if SetOption15 = 0
  // for a light type, func_module should see light as basic and return servicec
  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("Tasker_Interface(FUNC_MODULE_INIT)"));
  // #endif // ENABLE_LOG_LEVEL_INFO
  // pCONT->Tasker_Interface(FUNC_MODULE_INIT); 


// I do need a MODULE_TYPE sets light type, though, this may be a pin config then light_type json option
// Perhaps a progmem json template for modules should be considered too, not just rules (or within where rules are)

// FUNC_MODULE_INIT is the same as INIT for me, as it permits funtions to turn on, this can be removed
// I do this from the main.cpp

  // if (XdrvCall(FUNC_MODULE_INIT)) {
  //   // Serviced
  // }
  // else if (YTF_IR_BRIDGE == my_module_type) {
  //   ClaimSerial();  // Stop serial loopback mode
  // }
  // else if (SONOFF_DUAL == my_module_type) {
  //   Settings.flag_system.mqtt_serial = 0;
  //   devices_present = 2;
  //   baudrate = 19200;
  // }
  // else if (CH4 == my_module_type) {
  //   Settings.flag_system.mqtt_serial = 0;
  //   devices_present = 4;
  //   baudrate = 19200;
  // }
  // else if (SONOFF_SC == my_module_type) {
  //   Settings.flag_system.mqtt_serial = 0;
  //   devices_present = 0;
  //   baudrate = 19200;
  // }
  // else if (SONOFF_BN == my_module_type) {   // PWM Single color led (White)
  //   Settings.light_settings.type = LT_PWM1;
  // }
  // else if (SONOFF_LED == my_module_type) {  // PWM Dual color led (White warm and cold)
  //   Settings.light_settings.type = LT_PWM2;
  // }
  // else if (AILIGHT == my_module_type) {     // RGBW led
  //   Settings.light_settings.type = LT_RGBW;
  // }
  // else if (SONOFF_B1 == my_module_type) {   // RGBWC led
  //   Settings.light_settings.type = LT_RGBWC;
  // }
  // else {
    if (!pCONT_set->Settings.light_settings.type) { pCONT_set->devices_present = 0; }

  /**
   *  Set PWM immediately to limit unknown states
   * **/
  #ifdef USE_PWM
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1_ID, i)) {
      pinMode(Pin(GPIO_PWM1_ID, i), OUTPUT);

      #ifdef ESP32
      analogAttach(Pin(GPIO_PWM1_ID, i),i);
      // analogWriteFreqRange(i,pCONT_set->Settings.pwm_frequency,pCONT_set->Settings.pwm_range);
      #endif

      #ifdef ESP8266
      if (pCONT_set->Settings.light_settings.type) {      // force PWM GPIOs to low or high mode, see #7165
        analogWrite(Pin(GPIO_PWM1_ID, i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range : 0);
      } else {
        pCONT_set->pwm_present = true;
        analogWrite(Pin(GPIO_PWM1_ID, i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - pCONT_set->Settings.pwm_value[i] : pCONT_set->Settings.pwm_value[i]);
      }
      #endif // ESP8266
    
    }
    
  }
  #endif // USE_PWM

  // }


/**
 * @brief Configure Serial
 * 
 */

/**
 * @brief Configure and set pwm levels
 * 
 */
  // Basic PWM controls (PWM1-6)
  if (!pCONT_set->Settings.light_settings.type) {
    for (uint8_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
      if (PinUsed(GPIO_PWM1_ID,i)) {
        pCONT_set->pwm_present = true;
        pinMode(GetPin(GPIO_PWM1_ID,i), OUTPUT);
        #ifdef ESP8266
        analogWrite(GetPin(GPIO_PWM1_ID,i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - pCONT_set->Settings.pwm_value[i] : pCONT_set->Settings.pwm_value[i]);
        Serial.println(F("Setting analogWrite with basic light type"));
        #endif // ESP8266
      }
    }
  }

  #ifndef DISABLE_SERIAL_LOGGING
  //DEBUG_PRINTF("Settings.light_settings.type=%d\n\r",pCONT_set->Settings.light_settings.type);
  #endif

  // #ifdef USE_MODULE_DRIVERS_LEDS
  // pCONT_led->SetLedPower(pCONT_set->Settings.ledstate &8);
  // pCONT_led->SetLedLink(pCONT_set->Settings.ledstate &8);
  // #endif // USE_MODULE_DRIVERS_LEDS


}