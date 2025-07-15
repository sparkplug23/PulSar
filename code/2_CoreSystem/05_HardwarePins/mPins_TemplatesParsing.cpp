#include "mPins.h"

/**
 * @brief Parses the template
 * @param buffer template as string 
 * */
void mPins::ModuleTemplate__ParseCJSONBuffer(char* buffer){

  JsonParserToken jtok = 0; 

  JsonParser parser(buffer);
  JsonParserObject rootObj = parser.getRootObject();   
  if (!rootObj) 
  {
    ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
    return;
  }
  if(jtok = rootObj[PM_NAME])
  {
    const char* name_ctr = jtok.getStr();
    #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
      char tb_name_ctr[64]; // Temporary buffer for modified name
      snprintf(tb_name_ctr, sizeof(tb_name_ctr), "tb_%s", name_ctr);
      name_ctr = tb_name_ctr; // Update name_ctr to point to modified name
      ALOG_COM(PSTR(D_LOG_CONFIG "system_name [Debug Clone] %s"), name_ctr);
    #else
      ALOG_COM(PSTR(D_LOG_CONFIG "system_name %s"), name_ctr);
    #endif
    snprintf(tkr_set->Settings.system_name.device, sizeof(tkr_set->Settings.system_name.device), "%s", name_ctr);
  }

  if(jtok = rootObj[PM_FRIENDLYNAME])
  {
    const char* name_ctr = jtok.getStr();
    #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
      char tb_friendly_ctr[64]; // Temporary buffer for modified friendly name
      snprintf(tb_friendly_ctr, sizeof(tb_friendly_ctr), "tb_%s", name_ctr);
      name_ctr = tb_friendly_ctr; // Update name_ctr to point to modified friendly name
      ALOG_COM(PSTR(D_LOG_CONFIG "Template NAME [Debug Clone] %s"), name_ctr);
    #else
      ALOG_COM(PSTR(D_LOG_CONFIG "Template NAME %s"), name_ctr);
    #endif
    snprintf(tkr_set->Settings.system_name.friendly, sizeof(tkr_set->Settings.system_name.friendly), "%s", name_ctr);
  }


  /**
   * @brief If either GPIO method is configured then clear the previous
   * 
   * @param rootObj 
   */
  if((rootObj[PM_GPIOC])||(rootObj[D_GPIO_FUNCTION]))
  {
    // tkr_set->runtime.boot_status.module_template_parse_success = true;
    for(int ii=0;ii<ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io);ii++){
      tkr_set->Settings.user_template.hardware.gp.io[ii] = GPIO_NONE;
    }
  }

  ALOG_INF(PSTR(D_LOG_PINS "PM_GPIOC: Starting"));

  if(jtok = rootObj[PM_GPIOC])
  {
    
    int8_t  real_pin = -1;
    int16_t gpio_number = -1;

    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      real_pin = GetRealPinNumberFromName(key);
      // ALOG_INF(PSTR("KEY%d %s %d"), pair_index, key, real_pin);

      int8_t index_pin = ConvertRealPinToIndexPin(real_pin);
      ALOG_INF( PSTR("KEY%d %s real_pin%d->index_pin%d"), pair_index, key, real_pin, index_pin);
      
      if(index_pin>=0){
      
        jtok.nextOne();
        const char* value = jtok.getStr();
        gpio_number = GetGPIOFunctionIDbyName(value);
        ALOG_INF(PSTR("gpio_func=%d"), gpio_number);

        if(gpio_number != -1){
          // only template pins
          tkr_set->Settings.user_template.hardware.gp.io[index_pin] = gpio_number; // non adjusted pin_number
          // FULL pin list
          // tkr_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          ALOG_INF(PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            tkr_set->Settings.user_template.hardware.gp.io[index_pin]
          );
          #endif
          // ALOG_INF(PSTR("pin_number/indexed=%d %d, gpio_number=%d"), pin_number, pin_number_array_index, gpio_number);
        }else{
          ALOG_ERR(PSTR("DECODE ERROR \"%s\" %d"),value, gpio_number);
        }

      }// end UsuableGPIOPin

    }

    // for (uint32_t ii=0;ii<ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io);ii++)
    // {
    //   ALOG_INF(PSTR("io[%d] %d"),ii,tkr_set->Settings.user_template.hardware.gp.io[ii]);
    //   //this works
    // }

  }

  ALOG_INF(PSTR(D_LOG_PINS "PM_GPIOC: Completed"));
  // DELAY_DEBUG(3000);

  // New method that assumes the function is the key, and the pin(s) are the values
  // pins may be represented as single ints, or array of ints (e.g. Lighting pins)
  if(jtok = rootObj[D_GPIO_FUNCTION])
  { 

    ALOG_INF(PSTR("Found %s"), D_GPIO_FUNCTION);
    
    int8_t  real_pin = -1;
    int16_t gpio_function_id = -1;

    uint8_t jsonpair_count = jtok.size();
    
    ALOG_INF(PSTR("jsonpair_count=%d"), jsonpair_count);

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++)
    {
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      // ALOG_INF(PSTR("KEY%d %s"), pair_index, key);

      gpio_function_id = tkr_pins->GetGPIOFunctionIDbyName(key);

      ALOG_INF(PSTR("KEY%d>> %s GPIO_ID %d"), pair_index, key, gpio_function_id);
      
      
      if(gpio_function_id>=0)
      {
    
        jtok.nextOne(); // Arrays not working with this, need a new function in parser myself
        const char* value = jtok.getStr();

        ALOG_INF(PSTR("getType %d"), jtok.getType());
        
        real_pin = GetRealPinNumberFromName(value);
        ALOG_INF(PSTR("Value%d>> \"%s\" %d #%d"), pair_index, value, real_pin, jtok.getInt());

        int8_t index_pin = ConvertRealPinToIndexPin(real_pin);
        ALOG_INF( PSTR("\t\t\treal_pin%d->index_pin%d"), real_pin, index_pin);

        if(index_pin != -1){
          // only template pins
          tkr_set->Settings.user_template.hardware.gp.io[index_pin] = gpio_function_id; // non adjusted pin_number
          // FULL pin list
          // tkr_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          ALOG_INF(PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            tkr_set->Settings.user_template.hardware.gp.io[index_pin]
          );
          #endif
          // ALOG_INF(PSTR("pin_number/indexed=%d %d, gpio_function_id=%d"), pin_number, pin_number_array_index, gpio_function_id);
        }
        else
        {
          #ifdef ENABLE_LOG_LEVEL_ERROR
          AddLog(LOG_LEVEL_ERROR, PSTR("DECODE ERROR \"%s\" %d"),value, gpio_function_id);
          #endif // ENABLE_LOG_LEVEL_COMMANDS
        }

      }// end
      else{
        ALOG_ERR(PSTR("KEY%d>> %s GPIO_ID %d"), pair_index, key, gpio_function_id);

      }

    }

    // delay(3000);

  }
  
    
  if(jtok = rootObj[PM_BASE])
  {    
    const char* base_ctr = jtok.getStr();
    ALOG_INF(PSTR(D_LOG_CONFIG "Template BASE Searching \"%s\""), base_ctr);
    int16_t module_result = GetModuleIDbyName(base_ctr);
    if(module_result >= -1)
    {
      tkr_set->Settings.module = module_result;
      ALOG_INF(PSTR(D_LOG_CONFIG "Template BASE Found %d"), tkr_set->Settings.module);
    }
  }
  else
  {
    tkr_set->Settings.module = USER_MODULE;
  }

  
  if(jtok = rootObj["RoomHint"])
  {
    const char* name_ctr = jtok.getStr();
    ALOG_COM(PSTR(D_LOG_CONFIG "Template RoomHint %s"), name_ctr);
    snprintf(tkr_set->Settings.room_hint, sizeof(tkr_set->Settings.room_hint), "%s", name_ctr);
  }

}


#ifdef CONFIG_IDF_TARGET_ESP32
// Conversion table from gpio template to physical gpio
const uint8_t Esp32TemplateToPhy[MAX_USER_PINS] = { ESP32_TEMPLATE_TO_PHY };
#endif // CONFIG_IDF_TARGET_ESP32


/**
 * @brief Reads any template GPIOs then reads any user set GPIOs
 * */
void mPins::TemplateGPIOs(myio *gp)
{

  // Create an empty template to write gpio functions in to
  uint16_t *dest = (uint16_t *)gp;
    memset(dest, GPIO_NONE, sizeof(myio));

  uint16_t src[ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io)];
  ALOG_INF(PSTR(D_LOG_PINS "src size %d"), ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));
  
  // Check if active module is simply a user_module, requiring no template reads
  if (tkr_set->Settings.module == USER_MODULE) 
  {
    ALOG_INF(PSTR(D_LOG_PINS "Loading USER provided template"));    
    memcpy(&src, &tkr_set->Settings.user_template.hardware.gp, sizeof(mycfgio)); // Simply read the user_configured gpio, skipping any progmem templates
  } 
  // Read templates from progmem, these will differ by esp8266, esp8285 and esp32
  else {
    ALOG_INF(PSTR(D_LOG_PINS "Loading predefined template %d"), tkr_set->Settings.module);
    #ifdef ESP8266
      GetInternalTemplate(&src, tkr_set->Settings.module, 1);
    #endif
    #ifdef ESP32
      uint32_t module = ModuleTemplate(tkr_set->Settings.module);
      ALOG_INF(PSTR(D_LOG_PINS "Loading ESP32 template %d"), module);
      if(ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io) != sizeof(mycfgio))
      {
        ALOG_ERR(PSTR("TemplateGPIOs: Size mismatch %d != %d"), ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io), sizeof(mycfgio));
      }
      memcpy_P(&src, &module_template__gpio_map[module].gp, sizeof(mycfgio));
    #endif
  }

  /******
   * Dissimilar to tasmota which does dynamic copying and stores gpio, here
   * The src moves into the destination
   ******/
  uint32_t j = 0;
  for (uint8_t i = 0; i < ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io); i++) 
  {    
    dest[j] = src[i];    
    ALOG_DBM(PSTR("Copying dest=%d[%d]\t index/real = %d/%d"), dest[j],j, i,ConvertIndexPinToRealPin(i));
    j++;
  }

  AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO  src"), src, ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));
  AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO dest"), dest, ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));

}

#ifdef ESP8266
void mPins::GetInternalTemplate(void* ptr, uint32_t module, uint32_t option) 
{
  // Getting the enum index, which has both esp8266 and esp8285 modules linked
  uint8_t module_template = pgm_read_byte(module_template__ids + module);

  // AddLog(LOG_LEVEL_INFO, PSTR("DBG: Template %d, Option %d"), module_template, option);

  // template8 = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc
  // uint8_t template8[sizeof(mytmplt8285)] = { GPIO_NONE };
  // if (module_template < TMP_WEMOS) {
  //   memcpy_P(&template8, &kModules8266[module_template], 6);
  //   memcpy_P(&template8[8], &kModules8266[module_template].gp.io[6], 6);
  // } else {
  //   memcpy_P(&template8, &kModules8285[module_template - TMP_WEMOS], sizeof(template8));
  // }

  /***
   * 
   * {
  "Cam": {
    "Resolution":10
  },
  "Brightness":100,
  "Debug":{"GpioInits":1, "ShowGPIOTemplates":1},
  "logging":{"TelnetLevel":12}
}
   */

  // esp8266 needs to be read in two chunks, since we are skipping gpio9/10 and setting them to GPIO_NONE

// Serial.printf("%d sizeof(mytmplt8285) = %d\n", module_template, (sizeof(mytmplt) / 2));

  // uint16_t template_read[sizeof(mycfgio)] = { GPIO_NONE };
  uint16_t template_read[(sizeof(mytmplt) / 2)] = { GPIO_NONE };


  if(module < MODULE_MAXMODULE_8266)
  {
    ALOG_INF(PSTR("TemplateGPIOs loading from esp8266 module_template__gpio_map[%d]"), module);
    // memcpy_P(ptr, &module_template__gpio_map_ESP8266[module].gp, sizeof(mytmplt8266));

    memcpy_P(&template_read, &module_template__gpio_map_ESP8266[module_template], 6 * sizeof(uint16_t));
    memcpy_P(&template_read[8], &module_template__gpio_map_ESP8266[module_template].gp.io[6], 6 * sizeof(uint16_t));

    AddLog_Array_Block(7, PSTR("TemplateGPIOs esp8266 template8"), template_read, ARRAY_SIZE(template_read), 20, false);

    // memcpy_P(ptr, template8, sizeof(mytmplt8285)); // copy full width
    // memcpy_P(ptr, template8, sizeof(template8));

  }else{
    uint8_t mod85 = module - MODULE_MAXMODULE_8266;
    ALOG_INF(PSTR("TemplateGPIOs loading from esp8285 module_template__gpio_map[%d] %d"), mod85, sizeof(mytmplt8285));

    memcpy_P(&template_read, &module_template__gpio_map_ESP8285[mod85], sizeof(template_read));
    
    // memcpy_P(ptr, template8, sizeof(template8));
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: GetInternalTemplate %*_H"), sizeof(mytmplt8285), (uint8_t *)&template8);

  // // template16  = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc,Flg
  // uint16_t template16[(sizeof(mytmplt) / 2)] = { GPIO_NONE };
  // TemplateConvert(template8, template16);

  uint32_t index = 0;
  uint32_t size = sizeof(mycfgio);      // template16[module_template].gp

  // ALOG_INF(PSTR("TemplateGPIOsA option %d, index %d, size %d %d"), option, index, size, MAX_USER_PINS);
  switch (option) {
    case 2: {
      index = (sizeof(mytmplt) / 2) -1; // template16[module_template].flag
      size = 2;  // single number, 16 bits wide (uint16_t)
      break;
    }
    case 3: {
      // ALOG_INF(PSTR("TemplateGPIOsB option %d, index %d, size %d"), option, index, size);
      size = sizeof(mytmplt);           // template16[module_template]
      break;
    }
  }
  ALOG_INF(PSTR("TemplateGPIOs option %d, index %d, size %d"), option, index, size);
  
  memcpy(ptr, &template_read[index], size);  // Only pins get written back, not the flag (it gets called later, hence the "index" to shift to the flag byte)

//  AddLog(LOG_LEVEL_DEBUG, PSTR("FNC: GetInternalTemplate option %d, %*_V"), option, size / 2, (uint8_t *)ptr);
}
#endif  // ESP8266


/**
 * Takes GPIO/pins configured to internal module and configures pins output
 * */
void mPins::GpioInit(void)
{

  ALOG_INF(PSTR(D_LOG_MODULE "GpioInit: Start ================================================"));
  
  uint16_t mgpio;

  /**
   * Part A: Checking module or setting to default based on chipset
   * */
  ALOG_INF(PSTR(D_LOG_MODULE "Validate module or set default"));
  if (!ValidModule(tkr_set->Settings.module)) 
  {
    ALOG_INF(PSTR(D_LOG_MODULE "!ValidModule"));
    uint8_t module = MODULE;
    if (!ValidModule(MODULE))
    { 
      module = MODULE_DEFAULT;
    }
    tkr_set->Settings.module = module;
    tkr_set->Settings.last_module = module;
  }else{
    ALOG_INF(PSTR(D_LOG_MODULE "ValidModule %d"),tkr_set->Settings.module);
  }

  SetModuleType();


  /**
   * Part C: Correcting for invalid gpio functions
   * */
  ALOG_INF(PSTR(D_LOG_MODULE "Correcting for invalid gpio functions from user template"));
  for (uint8_t i = 0; i < ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io); i++) {
    if(!ValidUserGPIOFunction(tkr_set->Settings.user_template.hardware.gp.io,i))
    {
      ALOG_INF(PSTR(D_LOG_CONFIG "InValidUserGPIOFunction %d %d"), i, tkr_set->Settings.user_template.hardware.gp.io[i]);
      tkr_set->Settings.user_template.hardware.gp.io[i] = GPIO_USER;  // Fix not supported sensor ids in template
    }
  }
  

  /**
   * @brief Part D: Read any template GPIO values. Function name needs changed!
   * 
   */
  ALOG_INF(PSTR(D_LOG_MODULE "Load TemplateGPIOs"));
  myio def_gp;
  TemplateGPIOs(&def_gp); // Get template values

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  AddLog_Array(LOG_LEVEL_INFO, PSTR("module_pins.io"), tkr_set->Settings.module_pins.io, ARRAY_SIZE(tkr_set->Settings.module_pins.io));
  AddLog_Array(LOG_LEVEL_INFO, PSTR("def_gp.io"), def_gp.io, ARRAY_SIZE(def_gp.io));
  #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  
  /**
   * @brief For all possible GPIO physical pins, populate gpio function if desired from TEMPLATE
   **/
  ALOG_INF(PSTR(D_LOG_MODULE "Sanitise GPIOs"));
  for (uint8_t i = 0; i < ARRAY_SIZE(tkr_set->Settings.module_pins.io); i++) 
  { //all pins
    
    ALOG_DBM(PSTR(D_LOG_CONFIG "%d=module_pins.io[%d]"),tkr_set->Settings.module_pins.io[i],i);

    uint16_t gpio = tkr_set->Settings.module_pins.io[i];

    ALOG_DBM(PSTR(D_LOG_CONFIG "module_pins.io[%d]=%d"),i,tkr_set->Settings.module_pins.io[i]);
    
    // If out of range, reset to none
    if(!ValidUserGPIOFunction(tkr_set->Settings.module_pins.io,i)){
      tkr_set->Settings.module_pins.io[i] = GPIO_NONE;             // Fix not supported sensor ids in module
      ALOG_DBM(PSTR(D_LOG_CONFIG "Unsupported module_pins.io %d being reset to GPIO_NONE"),i);
    }
    // Set any user pins 
    else if (tkr_set->Settings.module_pins.io[i] > GPIO_NONE) {
      tkr_set->runtime.my_module.io[i] = tkr_set->Settings.module_pins.io[i];
      ALOG_DBM(PSTR(D_LOG_CONFIG "my_module.io[i] = %d"),i,tkr_set->Settings.module_pins.io[i]);
    }

    // Set any pins set in template
    if ((def_gp.io[i] >= GPIO_NONE) && (def_gp.io[i] < GPIO_USER)) { //ADDED >= to also copy NONE 
      tkr_set->runtime.my_module.io[i] = def_gp.io[i];
      #ifndef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
      if(tkr_set->runtime.my_module.io[i] > GPIO_NONE){
        #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
        #ifdef ENABLE_LOG_LEVEL_INFO
          char buffer[50];
          ALOG_DBM(PSTR(D_LOG_CONFIG "mio[i]=gio[i] %d %d index/real %d/%d \"%s\""), tkr_set->runtime.my_module.io[i], def_gp.io[i], i,ConvertIndexPinToRealPin(i), GetGPIOFunctionNamebyID(tkr_set->runtime.my_module.io[i], buffer, sizeof(buffer))
        );
        #endif // ENABLE_LOG_LEVEL_INFO
        #ifndef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
      }
      #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    }
    else{
      ALOG_INF(PSTR(D_LOG_CONFIG "Invalid IO in def_gp.io[%d]=%d"),i,def_gp.io[i]);
    }

  }

  
  /** 
   * PArt E: Checking for bitSet/Inverted etc, but I am expecting this to move into submodules
   *  Take module io and configure pins
   * Unlike Tas, each pin function has its unique name maintained (ie SWT_INV for switch inverted maintained its ID, and it not saved simply as INV then shifted back to standard SWT. Internal classes must handle this)
   * */
  ALOG_INF(PSTR(D_LOG_MODULE "SetPin modes"));
  for (uint8_t index = 0; index < ARRAY_SIZE(tkr_set->runtime.my_module.io); index++) 
  {
    uint8_t real_pin = ConvertIndexPinToRealPin(index);
    uint16_t gpio = tkr_set->runtime.my_module.io[index];
    mgpio = ValidPin_AdjustGPIO(index, gpio);
    ALOG_INF( PSTR("DBG: %d real_pin=%d moduleIO=%d  mgpio=%d"), index, real_pin, tkr_set->Settings.module_pins.io[index], mgpio);

    if(mgpio){ SetPin(real_pin, mgpio); }                  // Anything above GPIO_NONE and below GPIO_SENSOR_END 
    
  }//end for

  #ifdef ESP8266
  ALOG_INF(PSTR(D_LOG_MODULE "Switch default serial TX pin (H801)"));
  /**
   * @brief The check for H801 should be phased out here, as its hardware specific, really the template should force this to happen anyway as TX==2
   * 2023: Just check if the HWSerial is set to 2 then do it here
   * 
   */
    if ((2 == GetPin(GPIO_HWSERIAL0_TX)) || (MODULE_H801 == tkr_set->runtime.my_module_type)) { 
      DEBUG_LINE_HERE;
      Serial.set_tx(2); 
      flag_serial_set_tx_set = true;
      
      }
  #endif

  ALOG_INF(PSTR(D_LOG_MODULE "Set PWM defaults"));
  analogWriteRange(tkr_set->Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(tkr_set->Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
  
  // Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
  // Doing it here solves relay toggles at restart.
  ALOG_INF(PSTR(D_LOG_MODULE "Set unused pins as input"));
  for (uint32_t i = 0; 
                i < ARRAY_SIZE(tkr_set->runtime.my_module.io);
                i++
  ){
    
    uint32_t mgpio = ValidPin_AdjustGPIO(i, tkr_set->runtime.my_module.io[i]);
    
    ALOG_DBM( PSTR("INI: gpio pin %d, mgpio %d"), i, mgpio);
    
    if (((i < 6) || (i > 11)) && (0 == mgpio))
    {  // Skip SPI flash interface

      if (!((1 == i) || (3 == i))) {             // Skip serial
        #ifdef ESP8266
        if((MODULE_H801 == tkr_set->runtime.my_module_type) && (i !=2 ))
        {
          pinMode(i, INPUT);
        }
        #endif
      }
    }
  }


  /**
   * @brief New to help with inactive IC data lines, set esp32 outputs to either high or low for enable pins of ICs
   **/
  ALOG_INF(PSTR(D_LOG_MODULE "Set any latched pins"));
  for (uint32_t i = 0; 
                i < ARRAY_SIZE(tkr_set->runtime.my_module.io);
                i++
  ){
    
    uint32_t mgpio_function = ValidPin_AdjustGPIO(i, tkr_set->runtime.my_module.io[i]);    

    ALOG_DBM( PSTR("INI: gpio pin %d, mgpio %d"), i, mgpio_function);

    if(mgpio_function == GPIO_UNUSED_FORCED_LOW)
    {

      if (((i < 6) || (i > 11)) && (0 == mgpio))
      {  // Skip SPI flash interface
        if (!((1 == i) || (3 == i))) {             // Skip serial
          #ifdef ESP8266
          if((MODULE_H801 == tkr_set->runtime.my_module_type) && (i !=2 ))
          {
            pinMode(i, OUTPUT);
            digitalWrite(i, LOW);
          }
          #endif
        }
      }

      uint8_t real_pin = ConvertIndexPinToRealPin(i);
      pinMode(real_pin, OUTPUT);
      digitalWrite(real_pin, LOW);

    }else
    if(mgpio_function == GPIO_UNUSED_FORCED_HIGH)
    {
      if (((i < 6) || (i > 11)) && (0 == mgpio))
      {  // Skip SPI flash interface
        if (!((1 == i) || (3 == i))) {             // Skip serial
          #ifdef ESP8266
          if((MODULE_H801 == tkr_set->runtime.my_module_type) && (i !=2 ))
          {
            pinMode(i, OUTPUT);
            digitalWrite(i, HIGH);
          }
          #endif
        }
      }
      
      uint8_t real_pin = ConvertIndexPinToRealPin(i);
      pinMode(real_pin, OUTPUT);
      digitalWrite(real_pin, HIGH);

    }
    
  }

  GpioInitPwm();
  
  ALOG_INF(PSTR(D_LOG_MODULE "GpioInit: Complete ================================================"));

}


void mPins::GpioInitPwm(void) {
  // for (uint32_t pin = 0; pin < MAX_GPIO_PIN; pin++) {
  //   analog_write_state[pin] = -1;  // No PWM pin (could be GPIO_PWM or GPIO_LED)
  // }
  // for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
  //   if (PinUsed(GPIO_PWM1, i)) {
  //     pinMode(Pin(GPIO_PWM1, i), OUTPUT);
  //     if (i < TasmotaGlobal.light_type) {
  //       // force PWM GPIOs to low or high mode if belongs to the light (always <5), see #7165
  //       AnalogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range : 0);
  //     } else {
  //       TasmotaGlobal.pwm_present = true;
  //       if (i < MAX_PWMS_LEGACY) {
  //         AnalogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - Settings->pwm_value[i] : Settings->pwm_value[i]);
  //       } else {
  //         AnalogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - Settings->pwm_value_ext[i] : Settings->pwm_value_ext[i]);
  //       }
  //     }
  //   }
  // }

#ifdef USE_PWM
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1, i)) {
      pinMode(Pin(GPIO_PWM1, i), OUTPUT);

      #ifdef ESP32
      analogAttach(Pin(GPIO_PWM1, i),i);
      // analogWriteFreqRange(i,tkr_set->Settings.pwm_frequency,tkr_set->Settings.pwm_range);
        analogWrite(Pin(GPIO_PWM1, i), bitRead(tkr_set->runtime.pwm_inverted, i) ? tkr_set->Settings.pwm_range : 0);
      tkr_set->runtime.pwm_present = true;
      #endif

      #ifdef ESP8266
      // if (tkr_set->Settings.light_settings.type) {      // force PWM GPIOs to low or high mode, see #7165
        analogWrite(Pin(GPIO_PWM1, i), bitRead(tkr_set->runtime.pwm_inverted, i) ? tkr_set->Settings.pwm_range : 0);
      // } else {
      //   tkr_set->pwm_present = true;
      //   analogWrite(Pin(GPIO_PWM1_ID, i), bitRead(tkr_set->pwm_inverted, i) ? tkr_set->Settings.pwm_range - tkr_set->Settings.pwm_value[i] : tkr_set->Settings.pwm_value[i]);
      // }
      #endif // ESP8266
    
    }
    
  }
  #endif // USE_PWM

}
