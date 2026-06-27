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

  // AddLog_Array_Block((uint8_t)LOG_LEVEL_HIGHLIGHT, "hardware.gp.io", tkr_set->Settings.user_template.hardware.gp.io, ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));

  ALOG_DBG(PSTR(D_LOG_PINS "PM_GPIOC: Starting"));

  for(int ii=0;ii<ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io);ii++){
    ALOG_DBG(PSTR("io %d=%d"),ii,tkr_set->Settings.user_template.hardware.gp.io[ii]);
  }



  if(jtok = rootObj[PM_GPIOC])
  {
    
    int8_t  real_pin = -1;
    int16_t gpio_number = -1;

    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      real_pin = GetRealPinNumberFromName(key);
      // ALOG_DBG(PSTR("KEY%d %s %d"), pair_index, key, real_pin);

      int8_t index_pin = real_pin;
      ALOG_DBG( PSTR("KEY%d %s real_pin%d->index_pin%d"), pair_index, key, real_pin, index_pin);
      
      if(index_pin>=0){
      
        jtok.nextOne();
        const char* value = jtok.getStr();
        gpio_number = GetGPIOFunctionIDbyName(value);
        ALOG_DBG(PSTR("gpio_func=%d"), gpio_number);

        if(gpio_number != -1){
          // only template pins
          tkr_set->Settings.user_template.hardware.gp.io[index_pin] = gpio_number; // non adjusted pin_number
          // FULL pin list
          // tkr_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          ALOG_DBG(PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            tkr_set->Settings.user_template.hardware.gp.io[index_pin]
          );
          #endif
          // ALOG_DBG(PSTR("pin_number/indexed=%d %d, gpio_number=%d"), pin_number, pin_number_array_index, gpio_number);
        }else{
          ALOG_ERR(PSTR("DECODE ERROR \"%s\" %d"),value, gpio_number);
        }

      }// end UsuableGPIOPin

    }

    // for (uint32_t ii=0;ii<ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io);ii++)
    // {
    //   ALOG_DBG(PSTR("io[%d] %d"),ii,tkr_set->Settings.user_template.hardware.gp.io[ii]);
    //   //this works
    // }

  }

  ALOG_DBG(PSTR(D_LOG_PINS "PM_GPIOC: Completed"));
  // DELAY_DEBUG(3000);

  // New method that assumes the function is the key, and the pin(s) are the values
  // pins may be represented as single ints, or array of ints (e.g. Lighting pins)
  if(jtok = rootObj[D_GPIO_FUNCTION])
  { 

    ALOG_DBG(PSTR("Found %s"), D_GPIO_FUNCTION);
    
    int8_t  real_pin = -1;
    int16_t gpio_function_id = -1;

    uint8_t jsonpair_count = jtok.size();
    
    ALOG_DBG(PSTR("jsonpair_count=%d"), jsonpair_count);

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++)
    {
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      // ALOG_DBG(PSTR("KEY%d %s"), pair_index, key);

      gpio_function_id = tkr_pins->GetGPIOFunctionIDbyName(key);

      ALOG_DBG(PSTR("KEY%d>> %s GPIO_ID %d"), pair_index, key, gpio_function_id);
      
      
      if(gpio_function_id>=0)
      {
    
        jtok.nextOne(); // Arrays not working with this, need a new function in parser myself
        const char* value = jtok.getStr();

        ALOG_DBG(PSTR("getType %d"), jtok.getType());
        
        real_pin = GetRealPinNumberFromName(value);
        ALOG_DBG(PSTR("Value%d>> \"%s\" %d #%d"), pair_index, value, real_pin, jtok.getInt());

        int8_t index_pin = real_pin;
        ALOG_DBG( PSTR("\t\t\treal_pin%d->index_pin%d"), real_pin, index_pin);

        if(index_pin != -1){
          // only template pins
          tkr_set->Settings.user_template.hardware.gp.io[index_pin] = gpio_function_id; // non adjusted pin_number
          // FULL pin list
          // tkr_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
          
          #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          ALOG_DBG(PSTR("hardware.gp.io[real%d/index%d] = gpio function %d SET"), 
            real_pin, 
            index_pin,
            tkr_set->Settings.user_template.hardware.gp.io[index_pin]
          );
          #endif
          // ALOG_DBG(PSTR("pin_number/indexed=%d %d, gpio_function_id=%d"), pin_number, pin_number_array_index, gpio_function_id);
        }
        else
        {
          ALOG_ERR(PSTR("DECODE ERROR \"%s\" %d"),value, gpio_function_id);
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
    ALOG_ERR(PSTR(D_LOG_CONFIG "Template Base Missing"));
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


// /**
//  * @brief Reads any template GPIOs then reads any user set GPIOs
//  * */
// void mPins::TemplateGPIOs(myio *gp)
// {

//   // Create an empty template to write gpio functions in to
//   uint16_t *dest = (uint16_t *)gp;
//     memset(dest, GPIO_NONE, sizeof(myio));

//   uint16_t src[ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io)];
//   ALOG_DBG(PSTR(D_LOG_PINS "src size %d"), ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));
  
//   // Check if active module is simply a user_module, requiring no template reads
//   if (tkr_set->Settings.module == USER_MODULE) 
//   {
//     ALOG_DBG(PSTR(D_LOG_PINS "Loading USER provided template"));    
//     memcpy(&src, &tkr_set->Settings.user_template.hardware.gp, sizeof(mycfgio)); // Simply read the user_configured gpio, skipping any progmem templates
//   } 
//   // Read templates from progmem, these will differ by esp8266, esp8285 and esp32
//   else {
//     ALOG_DBG(PSTR(D_LOG_PINS "Loading predefined template %d"), tkr_set->Settings.module);
//     #ifdef ESP8266
//       GetInternalTemplate(&src, tkr_set->Settings.module, 1);
//     #endif
//     #ifdef ESP32
//       uint32_t module = ModuleTemplate(tkr_set->Settings.module);
//       ALOG_DBG(PSTR(D_LOG_PINS "Loading ESP32 template %d"), module);
//       if(ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io) != sizeof(mycfgio))
//       {
//         ALOG_ERR(PSTR("TemplateGPIOs: Size mismatch %d != %d"), ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io), sizeof(mycfgio));
//       }
//       memcpy_P(&src, &module_template__gpio_map[module].gp, sizeof(mycfgio));
//     #endif
//   }

//   /******
//    * Dissimilar to tasmota which does dynamic copying and stores gpio, here
//    * The src moves into the destination
//    ******/
//   uint32_t j = 0;
//   for (uint8_t i = 0; i < ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io); i++) 
//   {    
//     dest[j] = src[i];    
//     ALOG_DBM(PSTR("Copying dest=%d[%d]\t index/real = %d/%d"), dest[j],j, i,ConvertIndexPinToRealPin(i));
//     j++;
//   }

//   AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO  src"), src, ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));
//   AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO dest"), dest, ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io));

// }

/**
 * @brief Reads any template GPIOs then reads any user set GPIOs
 */
/**
 * @brief Reads compact template GPIOs and expands them to physical GPIO-indexed runtime storage.
 */
void mPins::TemplateGPIOs(myio *gp)
{
  if(!gp)
  {
    ALOG_ERR(PSTR(D_LOG_PINS "TemplateGPIOs: gp is null"));
    return;
  }

  /*******************************************************************************************\
   * Clear destination/runtime pin-function storage
  \*******************************************************************************************/

  for(uint8_t real_pin = 0; real_pin < ARRAY_SIZE(gp->io); real_pin++)
  {
    gp->io[real_pin] = GPIO_NONE;
  }

  for(uint8_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
  {
    pin[real_pin].gpio_function = GPIO_NONE;
    pin[real_pin].unique_module_owner_id = 0;
    pin[real_pin].allocation.allocated = 0;
  }

  /*******************************************************************************************\
   * Load source template
   *
   * USER_MODULE:
   *   tkr_set->Settings.user_template.hardware.gp.io[] is already physical GPIO indexed.
   *   src[26] means GPIO26, src[27] means GPIO27, etc.
   *
   * Predefined/internal templates:
   *   Kept on the compact template-indexed path and expanded using TemplateIndexToRealPin().
  \*******************************************************************************************/

  uint16_t src[MAX_USER_PINS];

  for(uint8_t template_index = 0; template_index < ARRAY_SIZE(src); template_index++)
  {
    src[template_index] = GPIO_NONE;
  }

  ALOG_DBG(
    PSTR(D_LOG_PINS "TemplateGPIOs src_count=%u dest_count=%u"),
    ARRAY_SIZE(src),
    ARRAY_SIZE(gp->io)
  );

  const bool user_template_is_physical_indexed = (tkr_set->Settings.module == USER_MODULE);

  if(user_template_is_physical_indexed)
  {
    ALOG_INF(PSTR(D_LOG_PINS "Loading USER provided template"));

    memcpy(&src, &tkr_set->Settings.user_template.hardware.gp, sizeof(mycfgio));
  }
  else
  {
    ALOG_DBG(PSTR(D_LOG_PINS "Loading predefined template %u"), tkr_set->Settings.module);

    #ifdef ESP8266
    GetInternalTemplate(&src, tkr_set->Settings.module, 1);
    #endif

    #ifdef ESP32
    uint32_t module = ModuleTemplate(tkr_set->Settings.module);
    ALOG_INF(PSTR(D_LOG_PINS "Loading ESP32 template %u"), module);
    memcpy_P(&src, &module_template__gpio_map[module].gp, sizeof(mycfgio));
    #endif
  }

  /*******************************************************************************************\
   * Expand/copy source into physical GPIO-indexed destination
  \*******************************************************************************************/

  if(user_template_is_physical_indexed)
  {
    /***************************************************************************\
     * User templates are already physical GPIO indexed.
     *
     * Do not run TemplateIndexToRealPin() here.
     *
     * Required example:
     *   src[26] -> gp->io[26]
     *   src[27] -> gp->io[27]
    \***************************************************************************/

    for(uint8_t real_pin = 0; real_pin < ARRAY_SIZE(gp->io); real_pin++)
    {
      uint16_t gpio_function = GPIO_NONE;

      if(real_pin < ARRAY_SIZE(src))
      {
        gpio_function = src[real_pin];
      }

      gp->io[real_pin] = gpio_function;

      if(real_pin < MAX_GPIO_PIN)
      {
        pin[real_pin].gpio_function = gpio_function;

        if((gpio_function != GPIO_NONE) && (gpio_function != GPIO_USER))
        {
          pin[real_pin].allocation.allocated = 1;
          pin[real_pin].allocation.unavailable = 0;
          SetPinOwnerIfAllowed(real_pin, GetModuleUniqueID());
        }
      }

      ALOG_DBM(
        PSTR(D_LOG_PINS "TemplateGPIOs USER real_pin=%u gpio=%u"),
        real_pin,
        gpio_function
      );
    }
  }
  else
  {
    /***************************************************************************\
     * Internal/predefined templates use compact template indices.
     *
     * These require conversion to physical GPIO numbers.
    \***************************************************************************/

    for(uint8_t template_index = 0; template_index < ARRAY_SIZE(src); template_index++)
    {
      int8_t real_pin = TemplateIndexToRealPin(template_index);

      if(real_pin < 0)
      {
        ALOG_ERR(
          PSTR(D_LOG_PINS "TemplateGPIOs skip bad template_index=%u gpio=%u"),
          template_index,
          src[template_index]
        );
        continue;
      }

      if((uint8_t)real_pin >= ARRAY_SIZE(gp->io))
      {
        ALOG_ERR(
          PSTR(D_LOG_PINS "TemplateGPIOs real_pin OOR index=%u real_pin=%d gpio=%u"),
          template_index,
          real_pin,
          src[template_index]
        );
        continue;
      }

      gp->io[(uint8_t)real_pin] = src[template_index];

      if((uint8_t)real_pin < MAX_GPIO_PIN)
      {
        pin[(uint8_t)real_pin].gpio_function = src[template_index];

        if((src[template_index] != GPIO_NONE) && (src[template_index] != GPIO_USER))
        {
          pin[(uint8_t)real_pin].allocation.allocated = 1;
          pin[(uint8_t)real_pin].allocation.unavailable = 0;
          SetPinOwnerIfAllowed((uint8_t)real_pin, GetModuleUniqueID());
        }
      }

      ALOG_DBM(
        PSTR(D_LOG_PINS "TemplateGPIOs PRESET index=%u real_pin=%d gpio=%u"),
        template_index,
        real_pin,
        src[template_index]
      );
    }
  }

  /*******************************************************************************************\
   * Debug output
  \*******************************************************************************************/

  AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO src"), src, ARRAY_SIZE(src));
  AddLog_Array(LOG_LEVEL_INFO, PSTR("TemplateGPIO dst"), gp->io, ARRAY_SIZE(gp->io));

  ALOG_DBG(
    PSTR(D_LOG_PINS "TemplateGPIO CHECK dst[16]=%u dst[17]=%u dst[26]=%u dst[27]=%u dst[30]=%u dst[31]=%u"),
    gp->io[16],
    gp->io[17],
    gp->io[26],
    gp->io[27],
    gp->io[30],
    gp->io[31]
  );
}


bool mPins::SetPinOwnerIfAllowed(uint8_t real_pin, uint16_t new_owner_id)
{
  if(real_pin >= MAX_GPIO_PIN) return false;

  uint16_t current_owner_id = pin[real_pin].unique_module_owner_id;

  if(current_owner_id == 0)
  {
    pin[real_pin].unique_module_owner_id = new_owner_id;
    return true;
  }

  if((current_owner_id < 3000) && (new_owner_id >= 3000))
  {
    ALOG_WRN(PSTR(D_LOG_PINS "Owner protected real_pin=%u current=%u new=%u"),
      real_pin, current_owner_id, new_owner_id);
    return false;
  }

  pin[real_pin].unique_module_owner_id = new_owner_id;
  return true;
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

    AddLog_Array_Block(1, PSTR("TemplateGPIOs esp8266 template8"), template_read, ARRAY_SIZE(template_read), 20, false);

    // memcpy_P(ptr, template8, sizeof(mytmplt8285)); // copy full width
    // memcpy_P(ptr, template8, sizeof(template8));

  }else{
    uint8_t mod85 = module - MODULE_MAXMODULE_8266;
    ALOG_DBG(PSTR("TemplateGPIOs loading from esp8285 module_template__gpio_map[%d] %d"), mod85, sizeof(mytmplt8285));

    memcpy_P(&template_read, &module_template__gpio_map_ESP8285[mod85], sizeof(template_read));
    
    // memcpy_P(ptr, template8, sizeof(template8));
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: GetInternalTemplate %*_H"), sizeof(mytmplt8285), (uint8_t *)&template8);

  // // template16  = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc,Flg
  // uint16_t template16[(sizeof(mytmplt) / 2)] = { GPIO_NONE };
  // TemplateConvert(template8, template16);

  uint32_t index = 0;
  uint32_t size = sizeof(mycfgio);      // template16[module_template].gp

  // ALOG_DBG(PSTR("TemplateGPIOsA option %d, index %d, size %d %d"), option, index, size, MAX_USER_PINS);
  switch (option) {
    case 2: {
      index = (sizeof(mytmplt) / 2) -1; // template16[module_template].flag
      size = 2;  // single number, 16 bits wide (uint16_t)
      break;
    }
    case 3: {
      // ALOG_DBG(PSTR("TemplateGPIOsB option %d, index %d, size %d"), option, index, size);
      size = sizeof(mytmplt);           // template16[module_template]
      break;
    }
  }
  ALOG_INF(PSTR("TemplateGPIOs option %d, index %d, size %d"), option, index, size);
  
  memcpy(ptr, &template_read[index], size);  // Only pins get written back, not the flag (it gets called later, hence the "index" to shift to the flag byte)

//  AddLog(LOG_LEVEL_DEBUG, PSTR("FNC: GetInternalTemplate option %d, %*_V"), option, size / 2, (uint8_t *)ptr);
}
#endif  // ESP8266




void mPins::GpioInit(void)
{
  ALOG_DBG(PSTR(D_LOG_MODULE "GpioInit: Start ================================================"));

  /*******************************************************************************************\
   * Part A: Initialise runtime pin table to safe defaults
  \*******************************************************************************************/

  PinTable_InitSafeDefaults();


  /*******************************************************************************************\
   * Part B: Validate selected module or fall back to default
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Validate module or set default"));

  if(!ValidModule(tkr_set->Settings.module))
  {
    ALOG_DBG(PSTR(D_LOG_MODULE "!ValidModule"));

    uint8_t module = MODULE;

    if(!ValidModule(MODULE))
    {
      module = MODULE_DEFAULT;
    }

    tkr_set->Settings.module      = module;
    tkr_set->Settings.last_module = module;
  }
  else
  {
    ALOG_DBG(PSTR(D_LOG_MODULE "ValidModule %d"), tkr_set->Settings.module);
  }

  SetModuleType();


  /*******************************************************************************************\
   * Part C: Correct invalid GPIO functions in stored user compact template
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Correct invalid GPIO functions from user compact template"));

  for(uint8_t template_index = 0; template_index < ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io); template_index++)
  {
    const uint16_t gpio_function = tkr_set->Settings.user_template.hardware.gp.io[template_index];

    if((gpio_function == GPIO_NONE) || (gpio_function == GPIO_USER))
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "user_template.gp.io[%u]=%u, normal unassigned/user state"), template_index, gpio_function);
      continue;
    }

    if(!ValidUserGPIOFunction(tkr_set->Settings.user_template.hardware.gp.io, template_index))
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "Invalid user_template.gp.io[%u]=%u, resetting to GPIO_USER"), template_index, gpio_function);
      tkr_set->Settings.user_template.hardware.gp.io[template_index] = GPIO_USER;
    }
  }


  /*******************************************************************************************\
   * Part D: Load and decode template GPIOs
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Load TemplateGPIOs"));

  myio def_gp;
  TemplateGPIOs(&def_gp);

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  AddLog_Array(LOG_LEVEL_DEBUG, PSTR("module_pins.io"), tkr_set->Settings.module_pins.io, ARRAY_SIZE(tkr_set->Settings.module_pins.io));
  AddLog_Array(LOG_LEVEL_DEBUG, PSTR("def_gp.io"),        def_gp.io,                         ARRAY_SIZE(def_gp.io));
  #endif


  /*******************************************************************************************\
   * Part E: Build runtime.my_module.io as physical GPIO-indexed table
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Build physical-indexed runtime.my_module.io"));

  for(uint8_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
  {
    const uint16_t module_gpio   = tkr_set->Settings.module_pins.io[real_pin];
    const uint16_t template_gpio = def_gp.io[real_pin];

    tkr_set->runtime.my_module.io[real_pin] = GPIO_NONE;

    ALOG_DBG(PSTR(D_LOG_CONFIG "GPIO%u: module_pins=%u template=%u"), real_pin, module_gpio, template_gpio);


    /**************************************************************************
     * 1. Apply physical-indexed module/user override
    **************************************************************************/

    if(module_gpio == GPIO_NONE)
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "module_pins.io[%u]=GPIO_NONE"), real_pin);
    }
    else if(module_gpio == GPIO_USER)
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "module_pins.io[%u]=GPIO_USER"), real_pin);
    }
    else if(!ValidGPIO(real_pin, module_gpio))
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "Invalid module_pins.io[%u]=%u, resetting to GPIO_NONE"), real_pin, module_gpio);
      tkr_set->Settings.module_pins.io[real_pin] = GPIO_NONE;
    }
    else
    {
      tkr_set->runtime.my_module.io[real_pin] = module_gpio;

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      char buffer[50];
      ALOG_DBG(PSTR(D_LOG_CONFIG "module override GPIO%u=%u \"%s\""),
        real_pin,
        module_gpio,
        GetGPIOFunctionNamebyID(module_gpio, buffer, sizeof(buffer))
      );
      #endif
    }


    /**************************************************************************
     * 2. Apply decoded template value
    **************************************************************************/

    if(template_gpio == GPIO_USER)
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "template GPIO%u=GPIO_USER, keeping user/module value"), real_pin);
    }
    else if(template_gpio == GPIO_NONE)
    {
      tkr_set->runtime.my_module.io[real_pin] = GPIO_NONE;
      ALOG_DBG(PSTR(D_LOG_CONFIG "template GPIO%u=GPIO_NONE"), real_pin);
    }
    else if(!ValidGPIO(real_pin, template_gpio))
    {
      ALOG_DBG(PSTR(D_LOG_CONFIG "Invalid template GPIO%u=%u, ignored"), real_pin, template_gpio);
    }
    else
    {
      tkr_set->runtime.my_module.io[real_pin] = template_gpio;

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      char buffer[50];
      ALOG_DBG(PSTR(D_LOG_CONFIG "template GPIO%u=%u \"%s\""),
        real_pin,
        template_gpio,
        GetGPIOFunctionNamebyID(template_gpio, buffer, sizeof(buffer))
      );
      #endif
    }
  }


  /*******************************************************************************************\
   * Part F: Commit selected GPIO functions into pin[real_pin]
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Commit GPIO functions to pin table"));

  for(uint8_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
  {
    const uint16_t gpio_function = tkr_set->runtime.my_module.io[real_pin];
    const uint16_t adjusted_gpio = ValidPin_AdjustGPIO(real_pin, gpio_function);

    ALOG_DBG(PSTR(D_LOG_CONFIG "GPIO%u: runtime=%u adjusted=%u"), real_pin, gpio_function, adjusted_gpio);

    SetPin_GPIOFunction(real_pin, adjusted_gpio);
  }


  /*******************************************************************************************\
   * ESP8266: switch default serial TX pin, H801 compatibility
  \*******************************************************************************************/

  #ifdef ESP8266
  /*
   * Performed here for earlier possible logging
   */
  if((2 == GetPin(GPIO_HWSERIAL_TX, 0)) || (MODULE_H801 == tkr_set->runtime.my_module_type))
  {
    Serial.set_tx(2);
    flag_serial_set_tx_set = true;
    ALOG_DBG(PSTR(D_LOG_MODULE "Switch default serial TX pin (H801)"));
  }
  #endif


  /*******************************************************************************************\
   * Set unused pins as input
   *
   * Related to resetPins()
   * Doing it here reduces relay toggles at restart.
   * CRITICAL: Do not perform this on esp32, as it will cause wifi to crash (unable to scan rssi)
  \*******************************************************************************************/

  #ifdef ESP8266

  // for(uint32_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
  // {
  //   uint32_t mgpio = ValidPin_AdjustGPIO(
  //     (uint8_t)real_pin,
  //     tkr_set->runtime.my_module.io[real_pin]
  //   );

  //   ALOG_DBM(
  //     PSTR("INI: real_pin=%u mgpio=%u"),
  //     real_pin,
  //     mgpio
  //   );

  //   if(0 == mgpio)
  //   {
  //     if(!((1 == real_pin) || (3 == real_pin)))   // skip serial
  //     {
  //       if((MODULE_H801 == tkr_set->runtime.my_module_type) && (real_pin != 2))
  //       {
  //         pinMode(real_pin, INPUT);
  //       }
  //     }
  //   }
  // }

  #endif


  /*******************************************************************************************\
   * Set any latched / forced unused pins
   *
   * GPIO_UNUSED_FORCED_LOW  -> output low
   * GPIO_UNUSED_FORCED_HIGH -> output high
   *
   * This only applies the electrical latch state.
   * It does not allocate ownership. Allocation is handled later by AllocatePin().
  \*******************************************************************************************/

  ALOG_DBG(PSTR(D_LOG_MODULE "Set any latched pins"));

  for(uint32_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
  {
    const uint16_t mgpio_function = ValidPin_AdjustGPIO(real_pin, tkr_set->runtime.my_module.io[real_pin]);
    const uint16_t mgpio_base     = UGPIO(mgpio_function);

    ALOG_DBG(PSTR("INI: real_pin %u, mgpio_function %u"), real_pin, mgpio_function);

    if((mgpio_function == GPIO_UNUSED_FORCED_LOW) || (mgpio_base == GPIO_UNUSED_FORCED_LOW))
    {
      ALOG_DBG(PSTR(D_LOG_MODULE "Forced LOW real_pin=%u"), real_pin);

      pinMode(real_pin, OUTPUT);
      digitalWrite(real_pin, LOW);
    }
    else if((mgpio_function == GPIO_UNUSED_FORCED_HIGH) || (mgpio_base == GPIO_UNUSED_FORCED_HIGH))
    {
      ALOG_DBG(PSTR(D_LOG_MODULE "Forced HIGH real_pin=%u"), real_pin);

      pinMode(real_pin, OUTPUT);
      digitalWrite(real_pin, HIGH);
    }
  }


  ALOG_DBG(PSTR(D_LOG_MODULE "GpioInit: End =================================================="));
}
