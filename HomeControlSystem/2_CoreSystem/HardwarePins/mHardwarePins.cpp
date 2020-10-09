#include "mHardwarePins.h"

void mHardwarePins::Template_Load(){


    ReadModuleTemplateFromProgmem();
    ParseModuleTemplate(); //probably goes elsewhere


}

int8_t mHardwarePins::Tasker(uint8_t function){

  if(function == FUNC_TEMPLATE_MODULE_LOAD){
    Template_Load();
  }

  switch(function){
    case FUNC_PRE_INIT:
      pre_init();
    break;
    case FUNC_EVERY_MINUTE:
      //ModuleSettings_ShowTemplateLog();
      //ModuleSettings_ShowActiveTemplate();
    break;
  }

}

void mHardwarePins::pre_init(){

}


void mHardwarePins::ModuleSettings_ShowTemplateLog()
{
  // AddLog_P(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),

  char buffer[50];

  AddLog_P(LOG_LEVEL_TEST,
    PSTR(
      "\n\r"
      "Template: %s\n\r"
      "(Friendly)Name: %s\n\r"
      "MQTT Topic Prefix: %s\n\r"
      "Gpio: \n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r" //5
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r" //10
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r" //12
        "\t%d(%d):%d\n\r" //13
      "BASE : %08X (%s)\n\r"
    ),
    pCONT_set->Settings.user_template2.full_ctr,
    pCONT_set->Settings.user_template2.hardware.name,
    pCONT_set->Settings.system_name.device,
    0,0,pCONT_set->Settings.user_template2.hardware.gp.io[0],
    1,1,pCONT_set->Settings.user_template2.hardware.gp.io[1],
    2,2,pCONT_set->Settings.user_template2.hardware.gp.io[2],
    3,3,pCONT_set->Settings.user_template2.hardware.gp.io[3],
    4,4,pCONT_set->Settings.user_template2.hardware.gp.io[4],
    5,5,pCONT_set->Settings.user_template2.hardware.gp.io[5],
    6,9,pCONT_set->Settings.user_template2.hardware.gp.io[6],
    7,10,pCONT_set->Settings.user_template2.hardware.gp.io[7],
    8,12,pCONT_set->Settings.user_template2.hardware.gp.io[8],
    9,13,pCONT_set->Settings.user_template2.hardware.gp.io[9],
    10,14,pCONT_set->Settings.user_template2.hardware.gp.io[10],
    11,15,pCONT_set->Settings.user_template2.hardware.gp.io[11],
    12,16,pCONT_set->Settings.user_template2.hardware.gp.io[12],
    pCONT_set->Settings.user_template2.base,
    GetModuleNameByID(pCONT_set->Settings.user_template2.base, buffer)
  );

  DEBUG_LINE;
  
    // const char* buffer2 = GetModuleNameByID(pCONT_set->Settings.user_template2.base, buffer);
    // AddLog_P(LOG_LEVEL_INFO,PSTR("user_template2.base=%s"),buffer2);

}

void mHardwarePins::ModuleSettings_ShowActiveTemplate()
{
  // AddLog_P(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
  char buffer[50];

  AddLog_P(LOG_LEVEL_TEST,
    PSTR(
      "\n\r"
      //"Template: %s\n\r"
      "(Friendly)Name: %s\n\r"
      "MQTT Topic Prefix: %s\n\r"
      "Gpio: \n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
        "\t%d(%d):%d\n\r"
      "Module : %s\n\r"
    ),
    //pCONT_set->Settings.user_template2.full_ctr,
    pCONT_set->Settings.user_template2.hardware.name,
    pCONT_set->Settings.system_name.device,
    0,0,pCONT_set->Settings.user_template2.hardware.gp.io[0],
    1,1,pCONT_set->Settings.user_template2.hardware.gp.io[1],
    2,2,pCONT_set->Settings.user_template2.hardware.gp.io[2],
    3,3,pCONT_set->Settings.user_template2.hardware.gp.io[3],
    4,4,pCONT_set->Settings.user_template2.hardware.gp.io[4],
    5,5,pCONT_set->Settings.user_template2.hardware.gp.io[5],
    6,9,pCONT_set->Settings.user_template2.hardware.gp.io[6],
    7,10,pCONT_set->Settings.user_template2.hardware.gp.io[7],
    8,12,pCONT_set->Settings.user_template2.hardware.gp.io[8],
    9,13,pCONT_set->Settings.user_template2.hardware.gp.io[9],
    10,14,pCONT_set->Settings.user_template2.hardware.gp.io[10],
    11,15,pCONT_set->Settings.user_template2.hardware.gp.io[11],
    12,16,pCONT_set->Settings.user_template2.hardware.gp.io[12],
    GetModuleNameByID(pCONT_set->Settings.module, buffer)
  );

}


void mHardwarePins::ModuleSettings_FlashSerial()
{
  // AddLog_P(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
  // uint8_t real_gpio = 0;
  // Serial.printf("\n\r");
  // Serial.printf("Template: %s\n\r", pCONT_set->Settings.user_template2.full_ctr);
  // Serial.printf("Name: %s\n\r", pCONT_set->Settings.user_template2.hardware.name);
  // for(uint8_t gpio=0;gpio<sizeof(pCONT_set->Settings.user_template2.hardware.gp.io);gpio++){
  //   //get proper gpio number

  //   switch(gpio){
  //     case 0: real_gpio = 0; break;
  //     case 1: real_gpio = 1; break;
  //     case 2: real_gpio = 2; break;
  //     case 3: real_gpio = 3; break;
  //     case 4: real_gpio = 4; break;
  //     case 5: real_gpio = 5; break;
  //     case 6: real_gpio = 9; break;
  //     case 7: real_gpio = 10; break;
  //     case 8: real_gpio = 12; break;
  //     case 9: real_gpio = 13; break;
  //     case 10: real_gpio = 14; break;
  //     case 11: real_gpio = 15; break;
  //     case 12: real_gpio = 16; break;
  //   }

  //   Serial.printf("\t%d(%d):%d\n\r",gpio,real_gpio,pCONT_set->Settings.user_template2.hardware.gp.io[gpio]);
  // }
  // Serial.printf("FLAGS: %08X\n\r",pCONT_set->Settings.user_template2.flags);
  // Serial.printf("BASE : %08X\n\r",pCONT_set->Settings.user_template2.base);
  // Serial.printf("\n\r\n\r"); 

}

void mHardwarePins::ReadModuleTemplateFromProgmem(){

  uint16_t size_source = sizeof(MODULE_TEMPLATE);
  uint16_t size_dest = sizeof(pCONT_set->Settings.user_template2.full_ctr);

  AddLog_P(LOG_LEVEL_INFO, PSTR("source=%d, dest=%d"),size_source,size_dest);

  size_source = size_source > size_dest ? size_source : size_dest;
  memcpy_P(pCONT_set->Settings.user_template2.full_ctr,MODULE_TEMPLATE,size_source);

  AddLog_P(LOG_LEVEL_INFO, PSTR("MODULE_TEMPLATE READ = \"%s\""),pCONT_set->Settings.user_template2.full_ctr);

  #ifdef ENABLE_DEBUG_BOOT_DELAYS
    delay(1000);
  #endif

}

//
//const char PINS_WEMOS[] PROGMEM = "D3TXD4RXD2D1flashcFLFLolD6D7D5D8D0A0";

int8_t mHardwarePins::GetGPIONumberFromName(const char* c){

  int8_t pin = -1;
  //nodemcu/wemos named
  if     (strcmp(c,"D3")==0){ pin = 0; }
  else if(strcmp(c,"TX")==0){ pin = 1; }
  else if(strcmp(c,"D4")==0){ pin = 2; }
  else if(strcmp(c,"RX")==0){ pin = 3; }
  else if(strcmp(c,"D2")==0){ pin = 4; }
  else if(strcmp(c,"D1")==0){ pin = 5; }
  else if(strcmp(c,"SD2")==0){ pin = 9; }
  else if(strcmp(c,"SD3")==0){ pin = 10; }
  else if(strcmp(c,"D6")==0){ pin = 12; }
  else if(strcmp(c,"D7")==0){ pin = 13; }
  else if(strcmp(c,"D5")==0){ pin = 14; }
  else if(strcmp(c,"D8")==0){ pin = 15; }
  else if(strcmp(c,"D0")==0){ pin = 16; }
  // numbered
  else if(strcmp(c,"0")==0){ pin = 0; }
  else if(strcmp(c,"1")==0){ pin = 1; }
  else if(strcmp(c,"2")==0){ pin = 2; }
  else if(strcmp(c,"3")==0){ pin = 3; }
  else if(strcmp(c,"4")==0){ pin = 4; }
  else if(strcmp(c,"5")==0){ pin = 5; }
  else if(strcmp(c,"9")==0){ pin = 9; }
  else if(strcmp(c,"10")==0){ pin = 10; }
  else if(strcmp(c,"12")==0){ pin = 12; }
  else if(strcmp(c,"13")==0){ pin = 13; }
  else if(strcmp(c,"14")==0){ pin = 14; }
  else if(strcmp(c,"15")==0){ pin = 15; }
  else if(strcmp(c,"16")==0){ pin = 16; }
  else{
    pin = -1;
    AddLog_P(LOG_LEVEL_ERROR, PSTR("\t\tGetGPIONumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
  }

  AddLog_P(LOG_LEVEL_TEST, PSTR("GetGPIONumberFromName = %d"), pin);

  return pin;

}


void mHardwarePins::ParseModuleTemplate(){

  StaticJsonDocument<400> doc;
  DeserializationError error = deserializeJson(doc, pCONT_set->Settings.user_template2.full_ctr);
  
  if(error){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
    return;
  }
  JsonObject obj = doc.as<JsonObject>();
  
  //DEVICENAME/TOPICNAME/MQTTNAME
  // Keep split for now, but work towards better solution
  if(!obj[F("NAME")].isNull()){ 
    const char* name_ctr = obj[F("NAME")];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Settings.system_name.device Template MQTTName %s"),name_ctr);
    // snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  
  
    snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s",name_ctr);
  

  
  }
  // Name means friendly name (max 20 chars)
  if(!obj[F("FRIENDLYNAME")].isNull()){   //name is friendly name
  //   const char* name_ctr = obj[F("NAME")];
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "PHASE INTO FRIENDLYNAME Template Name %s"),name_ctr);
  //   // snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  // }
  // if(!obj[F("FRIENDLYNAME")].isNull()){    //maybe phase this one out? its the new "NAME"
    const char* name_ctr = obj[F("FRIENDLYNAME")];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template NAME %s"),name_ctr);
    snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr); //dont set directly?

    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template Name %s"),name_ctr);
    snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  
  }

  if(!obj[F("GPIO")].isNull()){ 
    
    // Array of numbers (0-255)
    uint8_t pin_num_count = 0;
    JsonArray array = obj[F("GPIO")];
    for(JsonVariant v : array){
        int gpio_function_id = v.as<int>();
        // FULL pin list
        pCONT_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
        // Only existing pins
        pCONT_set->Settings.user_template2.hardware.gp.io[pin_num_count] = gpio_function_id;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_CONFIG "GPIO[%d] = %d"),pin_num_count,gpio_function_id);  
        pin_num_count++;        
    }
    // Array of named functions "RGB_DATA"

    // Future, only require the pins needed

    // const char* name_ctr = obj[F("GPIO")];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template FRIENDLYNAME %s"),name_ctr);
    // snprintf(Settings.system_name.friendly,sizeof(Settings.system_name.friendly),"%s",name_ctr);
  }

  
  // if(!obj[F("GPIOC")].isNull()){ 
  if(obj.containsKey(F("GPIOC"))){ 
    AddLog_P(LOG_LEVEL_INFO, PSTR("GPIOC FOUND"));
    JsonObject list = obj[F("GPIOC")];
    // get key and value
    pCONT_set->boot_status.module_template_parse_success = true;
    // clear pins to none
    for(int ii=0;ii<sizeof(pCONT_set->Settings.user_template2.hardware.gp.io);ii++){
      pCONT_set->Settings.user_template2.hardware.gp.io[ii] = GPIO_NONE_ID;
    }
    
    int8_t  pin_number = -1;
    int16_t gpio_number = -1;

    for (JsonPair kv : list) {
      pin_number = GetGPIONumberFromName(kv.key().c_str());

      int8_t pin_number_array_index = UsablePinToTemplateArrayIndex(pin_number);
      
      if(pin_number_array_index>=0){
        gpio_number = GetGPIOFunctionIDbyName(kv.value());
        if(gpio_number != -1){
          // only template pins
          pCONT_set->Settings.user_template2.hardware.gp.io[pin_number_array_index] = gpio_number; // non adjusted pin_number
          // FULL pin list
          // pCONT_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 

          AddLog_P(LOG_LEVEL_INFO, PSTR("hardware.gp.io[%d/%d] = %d SET"), 
            pin_number, pin_number_array_index,

            pCONT_set->Settings.user_template2.hardware.gp.io[pin_number_array_index]
          );
          
          // AddLog_P(LOG_LEVEL_INFO, PSTR("pin_number/indexed=%d %d, gpio_number=%d"), pin_number, pin_number_array_index, gpio_number);
      
        }else{
          AddLog_P(LOG_LEVEL_INFO, PSTR("DECODE ERROR \"%s\""),kv.value());
        }

        }// end UsuableGPIOPin
    }

    AddLog_Array(LOG_LEVEL_TEST, "hardware.gp.io", (uint8_t *)&pCONT_set->Settings.user_template2.hardware.gp.io, (uint8_t) sizeof(pCONT_set->Settings.user_template2.hardware.gp.io));

  } // end GPIOC

  
  if(!obj[F("BASE")].isNull()){ 
    const char* base_ctr = obj[F("BASE")];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template BASE %s"),base_ctr);

    //GetBaseIDbyName();
      // pin_number = ;

    pCONT_set->Settings.module = GetModuleIDbyName(base_ctr);//USER_MODULE;

    char buffer[40];

    AddLog_P(LOG_LEVEL_TEST,PSTR("Settings.module=%s"),GetModuleNameByID(pCONT_set->Settings.module, buffer));

    //snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr);

  }else{
    pCONT_set->Settings.module = USER_MODULE;
  }


    
  ModuleSettings_FlashSerial();

  DEBUG_LINE;

}

// bool mHardwarePins::JsonTemplate(const char* dataBuf)
// {
//   // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}

//   // if (strlen(dataBuf) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

//   // StaticJsonDocument<350> doc;
//   // DeserializationError error =  deserializeJson(doc, dataBuf);
//   // JsonObject obj = doc.to<JsonObject>();

//   // //StaticJsonBuffer<350> jb;  // 331 from https://arduinojson.org/v5/assistant/
//   // //JsonObject& obj = jb.parseObject(dataBuf);

//   // if (error) { return false; }
//   // //if (!obj.success()) { return false; }

//   // // All parameters are optional allowing for partial changes
//   // const char* name = obj[D_JSON_NAME];
//   // if (name != nullptr) {
//   //   strlcpy(pCONT_set->Settings.user_template2.hardware.name, name, sizeof(pCONT_set->Settings.user_template2.hardware.name));
//   // }
//   // if (obj[D_JSON_GPIO].isNull()) {
//   //   for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
//   //     pCONT_set->Settings.user_template2.hardware.gp.io[i] = obj[D_JSON_GPIO][i] | 0;
//   //   }
//   // }
//   // if (obj[D_JSON_FLAG].isNull()) {
//   //   uint8_t flag = obj[D_JSON_FLAG] | 0;
//   //   memcpy(&pCONT_set->Settings.user_template2.hardware.flag, &flag, sizeof(gpio_flag));
//   // }
//   // if (obj[D_JSON_BASE].isNull()) {
//   //   uint8_t base = obj[D_JSON_BASE];
//   //   if ((0 == base) || !ValidTemplateModule(base -1)) { base = 18; }
//   //   pCONT_set->Settings.user_template2.base = base -1;  // Default WEMOS
//   // }
//   return true;
// }

// Return true/false if pin can be used (check for flash pins etc)
bool mHardwarePins::UsuableGPIOPin(uint8_t pin)
{
  // Valid
  if ((pin >= 0) && (pin <= 5)){
    return true;
  }
  // GPIO
  else if (((pin >= 6) && (pin <= 8))||(pin == 11)){
    return false; // Flash pins
  }
  // flash pins on 8265
  else if ((pin == 9) || (pin == 10)){ //(MODULE_WEMOS_ID == pCONT_set->Settings.module) // not on esp8625
    return true; // Flash pins
  }
  // Valid
  else if ((pin >= 12) && (pin <= 16)){
    return true;
  }
  
  return false;
}

// Return true/false if pin can be used (check for flash pins etc)
int8_t mHardwarePins::UsablePinToTemplateArrayIndex(uint8_t pin)
{
  // Valid
  // if ((pin >= 0) && (pin <= 5)){
  //   AddLog_P(LOG_LEVEL_INFO,PSTR("0-5 pin=%d to %d"),pin, pin);
  //   return pin;
  // }
  // // GPIO
  // else if (((pin >= 6) && (pin <= 8))||(pin == 11)){
  //   return -1; // Flash pins
  // }
  // // flash pins on 8265
  // else if ((pin == 9) || (pin == 10)){ //(MODULE_WEMOS_ID == pCONT_set->Settings.module) // not on esp8625
  //   AddLog_P(LOG_LEVEL_INFO,PSTR("9/10 pin=%d to %d"),pin, pin-3);
  //   return pin-3; // Flash pins
  // }
  // // Valid
  // else if ((pin >= 12) && (pin <= 16)){
  //   AddLog_P(LOG_LEVEL_INFO,PSTR("12-16 pin=%d to %d"),pin, pin-5);
  //   return pin-5;
  // }

  switch(pin){
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 5;
    case 6:
    case 7:
    case 8: return -1;
    case 9: return 6;
    case 10: return 7;
    case 11: return -1;
    case 12: return 8;
    case 13: return 9;
    case 14: return 10;
    case 15: return 11;
    case 16: return 12;
  }
  


  return -1;
}




/*********************************************************************************************\
 * GPIO Module and Template management
 * 
 * 







Move to "mHardwarePins?" START













\*********************************************************************************************/

// #ifndef ARDUINO_ESP8266_RELEASE_2_3_0  // Fix core 2.5.x ISR not in IRAM Exception
// uint32_t Pin(uint32_t gpio, uint32_t index) ICACHE_RAM_ATTR;
// #endif


uint32_t mHardwarePins::GetPin(uint32_t gpio, uint32_t index) {
  return Pin(gpio, index);
}
// phase out for Getpin above
uint32_t mHardwarePins::Pin(uint32_t gpio, uint32_t index) {
// #ifdef ESP8266
  uint16_t real_gpio = gpio + index;
// #else  // ESP32
//   uint16_t real_gpio = (gpio << 5) + index;
// #endif  // ESP8266 - ESP32
  // for (uint32_t i = 0; i < ARRAY_SIZE(gpio_pin); i++) {
  //   if (gpio_pin[i] == real_gpio) {
  //     return i;              // Pin number configured for gpio
  //   }
  // }

  #ifdef USE_PIN_NEWMETHOD
  for (uint32_t i = 0; i < sizeof(pCONT_set->pin); i++) {
    // needs new pin array
    if (pCONT_set->pin[i] == real_gpio) {
      Serial.printf("Pin number configured for gpio %d\n\r",i);
      return i;              // Pin number configured for gpio
    }
  }
  return 99;                 // No pin used for gpio
  #else
    // return 

    return pCONT_set->pin[real_gpio];


  #endif



  
}

boolean mHardwarePins::PinUsed(uint32_t gpio, uint32_t index) {
  return (Pin(gpio, index) < 99); //<99 used, else not used
}

void mHardwarePins::SetPin(uint32_t lpin, uint32_t gpio) {
  pCONT_set->gpio_pin_new[lpin] = gpio; // remember pin in new method

  //temp fix, remember it in old for legacy
  //#ifndef USE_NEW_GPIO_METHOD
  pCONT_set->pin[gpio] = lpin;
  //#endif

}


void mHardwarePins::DigitalWrite(uint32_t gpio_pin, uint32_t state)
{
  if (pCONT_set->pin[gpio_pin] < 99) {
        // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "DigitalWrite(%d[%d],%d)"),pCONT_set->pin[gpio_pin],gpio_pin,state);
    digitalWrite(pCONT_set->pin[gpio_pin], state &1);
  }
}

uint8_t mHardwarePins::ModuleNr()
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == pCONT_set->Settings.module) ? 0 : pCONT_set->Settings.module +1;
}

bool mHardwarePins::ValidTemplateModule(uint8_t index)
{
  for (uint8_t i = 0; i < sizeof(kModuleNiceList); i++) {
    if (index == pgm_read_byte(kModuleNiceList + i)) {
      return true;
    }
  }
  return false;
}

bool mHardwarePins::ValidModule(uint8_t index)
{
  if (index == USER_MODULE) { return true; }
  return ValidTemplateModule(index);
}

bool mHardwarePins::ValidUserGPIOFunction(uint8_t* pin_array, uint8_t index)
{
  // Outside valid gpio function range
  if((pin_array[index] >= GPIO_SENSOR_END_ID) && (pin_array[index] < GPIO_USER_ID)){
      return false;
    }else{
      return true;
    }
}

const char* mHardwarePins::ModuleName(char* buffer, uint8_t buflen)
{
  return AnyModuleName(pCONT_set->Settings.module, buffer, buflen);
}
const char* mHardwarePins::AnyModuleName(uint8_t index, char* buffer, uint8_t buflen)
{

// ERROR // if(buffer == nullptr){ buffer = pCONT_sup->global_buffer; }
// dont save to nullptr, either take nullptr or gloabl
 
 // char* pointer;
 // if(buffer == nullptr){ pointer = pCONT_sup->global_buffer; }
 // if(buffer != nullptr){ pointer = buffer;// = pCONT_sup->global_buffer; }

 
  if (USER_MODULE == index) {
  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE == index"));
    return pCONT_set->Settings.user_template2.hardware.name; //returns pointer
  } else {
    // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE != %d index %s"),index,kModules[index].name);
    // return kModules[index].name;

    return pCONT_sup->GetTextIndexed_P(buffer, buflen, index, kModules_Name_list);

    


  }
}




const char* mHardwarePins::AnyModuleName2(uint8_t index)
{
  if (USER_MODULE == index) {
  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE == index"));
    return pCONT_set->Settings.user_template2.hardware.name;
  } else {
  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE != index %s"),kModules[index].name);
    return "";//FPSTR(kModules[index].name);
  }
}


void mHardwarePins::ModuleGpios(myio *gp)
{

  uint8_t *dest = (uint8_t *)gp;
  memset(dest, GPIO_NONE_ID, sizeof(myio));

  uint8_t src[sizeof(mycfgio)];
  if (USER_MODULE == pCONT_set->Settings.module) {
    memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));
    AddLog_P(LOG_LEVEL_TEST, PSTR("ModuleGpios memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));"));
  } else {
    memcpy_P(&src, &kModules[pCONT_set->Settings.module].gp, sizeof(mycfgio));
  }
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)&src, sizeof(mycfgio));

  AddLog_Array(LOG_LEVEL_TEST, "ModuleGpios::gp", (uint8_t *)&src, (uint8_t) sizeof(mycfgio));

  uint8_t j = 0;
  for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

  AddLog_Array(LOG_LEVEL_TEST, "ModuleGpios::gp", (uint8_t *)dest, (uint8_t)sizeof(mycfgio));
//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)gp, sizeof(myio));
}

void mHardwarePins::ModuleGpios(myio *gp, uint8_t module_id)
{

//  Uses settings.module and copies into gpio struct

  uint8_t *dest = (uint8_t *)gp;
  memset(dest, GPIO_NONE_ID, sizeof(myio));

  uint8_t src[sizeof(mycfgio)];
  if (USER_MODULE == module_id) {
    
DEBUG_LINE;
    memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));
  } else {
DEBUG_LINE;
AddLog_P(LOG_LEVEL_DEBUG,PSTR("&kModules[module_id] %d"),module_id);
    memcpy_P(&src, &kModules[module_id].gp, sizeof(mycfgio));
  }
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81
DEBUG_LINE;
 AddLog_Array(LOG_LEVEL_DEBUG, "src", src, (uint8_t)sizeof(mycfgio));

  uint8_t j = 0;
  for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

 AddLog_Array(LOG_LEVEL_DEBUG, "gp", (uint8_t *)gp, (uint8_t)sizeof(myio));
//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)gp, sizeof(myio));
}

gpio_flag mHardwarePins::ModuleFlag()
{
  gpio_flag flag;

  if (USER_MODULE == pCONT_set->Settings.module) {
    flag = pCONT_set->Settings.user_template2.hardware.flag;
  } else {
    memcpy_P(&flag, &kModules[pCONT_set->Settings.module].flag, sizeof(gpio_flag));
  }

  return flag;
}

void mHardwarePins::ModuleDefault(uint8_t module)
{
  if (USER_MODULE == module) { module = MODULE_WEMOS_ID; }  // Generic
  pCONT_set->Settings.user_template2.base = module;
  memcpy_P(&pCONT_set->Settings.user_template2.hardware, &kModules[module], sizeof(mytmplt));
}

void mHardwarePins::SetModuleType()
{
  pCONT_set->my_module_type = (USER_MODULE == pCONT_set->Settings.module) ? pCONT_set->Settings.user_template2.base : pCONT_set->Settings.module;
}

uint8_t mHardwarePins::ValidPin(uint8_t pin, uint8_t gpio)
{
  uint8_t result = gpio;

  DEBUG_LINE;
  if (((pin > 5) && (pin < 9)) || (11 == pin)) {
    result = GPIO_NONE_ID;  // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }
  DEBUG_LINE;
  // need to add my other boards here
  if ((MODULE_WEMOS_ID == pCONT_set->Settings.module) && (!pCONT_set->Settings.flag_network_phaseout.user_esp8285_enable)) {
    if ((pin == 9) || (pin == 10)) { result = GPIO_NONE_ID; }  // Disable possible flash GPIO9 and GPIO10
  }
  DEBUG_LINE;
  return result;
}

bool mHardwarePins::ValidGPIO(uint8_t pin, uint8_t gpio)
{
  return (GPIO_USER_ID == ValidPin(pin, gpio));  // Only allow GPIO_USER pins
}

bool mHardwarePins::GetUsedInModule(uint8_t val, uint8_t *arr)
{
  int offset = 0;

  if (!val) { return false; }  // None

#ifdef USE_MODULE_SENSORS_BUTTONS
  if ((val >= GPIO_KEY1_ID) && (val < GPIO_KEY1_ID + MAX_KEYS)) {
    offset = (GPIO_KEY1_NP_ID - GPIO_KEY1_ID);
  }
  if ((val >= GPIO_KEY1_NP_ID) && (val < GPIO_KEY1_NP_ID + MAX_KEYS)) {
    offset = -(GPIO_KEY1_NP_ID - GPIO_KEY1_ID);
  }
  if ((val >= GPIO_KEY1_INV_ID) && (val < GPIO_KEY1_INV_ID + MAX_KEYS)) {
    offset = -(GPIO_KEY1_INV_ID - GPIO_KEY1_ID);
  }
  if ((val >= GPIO_KEY1_INV_NP_ID) && (val < GPIO_KEY1_INV_NP_ID + MAX_KEYS)) {
    offset = -(GPIO_KEY1_INV_NP_ID - GPIO_KEY1_ID);
  }
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  if ((val >= GPIO_SWT1_ID) && (val < GPIO_SWT1_ID + MAX_SWITCHES)) {
    offset = (GPIO_SWT1_NP_ID - GPIO_SWT1_ID);
  }
  if ((val >= GPIO_SWT1_NP_ID) && (val < GPIO_SWT1_NP_ID + MAX_SWITCHES)) {
    offset = -(GPIO_SWT1_NP_ID - GPIO_SWT1_ID);
  }
#endif
  if ((val >= GPIO_REL1_ID) && (val < GPIO_REL1_ID + MAX_RELAYS)) {
    offset = (GPIO_REL1_INV_ID - GPIO_REL1_ID);
  }
  if ((val >= GPIO_REL1_INV_ID) && (val < GPIO_REL1_INV_ID + MAX_RELAYS)) {
    offset = -(GPIO_REL1_INV_ID - GPIO_REL1_ID);
  }

  if ((val >= GPIO_LED1_ID) && (val < GPIO_LED1_ID + MAX_LEDS)) {
    offset = (GPIO_LED1_INV_ID - GPIO_LED1_ID);
  }
  if ((val >= GPIO_LED1_INV_ID) && (val < GPIO_LED1_INV_ID + MAX_LEDS)) {
    offset = -(GPIO_LED1_INV_ID - GPIO_LED1_ID);
  }

  if ((val >= GPIO_PWM1_ID) && (val < GPIO_PWM1_ID + MAX_PWMS)) {
    offset = (GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
  }
  if ((val >= GPIO_PWM1_INV_ID) && (val < GPIO_PWM1_INV_ID + MAX_PWMS)) {
    offset = -(GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
  }

  if ((val >= GPIO_CNTR1_ID) && (val < GPIO_CNTR1_ID + MAX_COUNTERS)) {
    offset = (GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
  }
  if ((val >= GPIO_CNTR1_NP_ID) && (val < GPIO_CNTR1_NP_ID + MAX_COUNTERS)) {
    offset = -(GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
  }

  for (uint8_t i = 0; i < MAX_GPIO_PIN; i++) {
    if (arr[i] == val) { return true; }
    if (arr[i] == val + offset) { return true; }
  }
  return false;
}




/*********************************************************************************************\
 * GPIO Module and Template management
 * 
 * 







Move to "mHardwarePins?" END













\*********************************************************************************************/
void mHardwarePins::TemplateJson()
{
  // Response_P(PSTR("{\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), pCONT_set->Settings.user_template.name);
  // for (uint8_t i = 0; i < sizeof(pCONT_set->Settings.user_template2.hardware.gp); i++) {
  //   ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", pCONT_set->Settings.user_template2.hardware.gp.io[i]);
  // }
  // ResponseAppend_P(PSTR("],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), pCONT_set->Settings.user_template.flag, pCONT_set->Settings.user_template_base +1);
}



void mHardwarePins::GpioInit(void)
{

// delay(2000);

  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MODULE "GpioInit"));
  uint8_t mpin;

  /**
   * Correcting for invalid module
   * */
  if (!ValidModule(pCONT_set->Settings.module)) {
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_MODULE "!ValidModule"));
    uint8_t module = MODULE;
    if (!ValidModule(MODULE)) { module = MODULE_NODEMCU_ID; }
    pCONT_set->Settings.module = module;
    pCONT_set->Settings.last_module = module;
  }else{
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_MODULE "ValidModule"));
  }
  SetModuleType();

  /**
   * Module changes
   * */
  if (pCONT_set->Settings.module != pCONT_set->Settings.last_module) {
    pCONT_set->baudrate = APP_BAUDRATE;
  }

  /**
   * Correcting for invalid gpio functions
   * */
  for (uint8_t i = 0; i < sizeof(pCONT_set->Settings.user_template2.hardware.gp); i++) {
    if(!ValidUserGPIOFunction(pCONT_set->Settings.user_template2.hardware.gp.io,i)){
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "!ValidUserGPIOFunction %d"),i);
      pCONT_set->Settings.user_template2.hardware.gp.io[i] = GPIO_USER_ID;  // Fix not supported sensor ids in template    }
    }
  }

  myio def_gp;
  ModuleGpios(&def_gp); // Get template values
  for (uint8_t i = 0; i < sizeof(pCONT_set->Settings.module_pins); i++) { //all 17 pins
    // If out of range, reset to none
    if(!ValidUserGPIOFunction(pCONT_set->Settings.module_pins.io,i)){
      pCONT_set->Settings.module_pins.io[i] = GPIO_NONE_ID;             // Fix not supported sensor ids in module
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "Unsupported module_pins.io %d being reset to GPIO_NONE"),i);
    }
    // Set any user pins 
    else if (pCONT_set->Settings.module_pins.io[i] > GPIO_NONE_ID) {
      pCONT_set->my_module.io[i] = pCONT_set->Settings.module_pins.io[i];
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "my_module.io[i] = Settings.module_pins.io[%d]"),i);
    }
    // Set any pins set in template
    if ((def_gp.io[i] > GPIO_NONE_ID) && (def_gp.io[i] < GPIO_USER_ID)) {
      pCONT_set->my_module.io[i] = def_gp.io[i];
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "my_module.io[i] = def_gp.io[i]; %d %d %d"),pCONT_set->my_module.io[i],def_gp.io[i],i);
    }
    // else{
    //   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "ELSE my_module.io[i] = def_gp.io[i]; %d %d %d"),pCONT_set->my_module.io[i],def_gp.io[i],i);
    // }

  }
  pCONT_set->my_module_flag = ModuleFlag();

  AddLog_Array(LOG_LEVEL_TEST, "my_module.io", (uint8_t *)&pCONT_set->my_module.io, (uint8_t)sizeof(pCONT_set->my_module.io));

  // Reset all pins to be "unset"
  for (uint16_t i = 0; i < GPIO_MAX_ID; i++) {
    pCONT_set->pin[i] = 99;
  }

//   #ifdef ESP8266
//   if ((Settings.my_adc0 >= ADC0_END) && (Settings.my_adc0 < ADC0_USER)) {
//     Settings.my_adc0 = ADC0_NONE;                   // Fix not supported sensor ids in module
//   }
//   else if (Settings.my_adc0 > ADC0_NONE) {
//     my_adc0 = Settings.my_adc0;                     // Set User selected Module sensors
//   }
//   my_module_flag = ModuleFlag();
//   uint32_t template_adc0 = my_module_flag.data &15;
//   if ((template_adc0 > ADC0_NONE) && (template_adc0 < ADC0_USER)) {
//     my_adc0 = template_adc0;                        // Force Template override
//   }
// #endif

  // Take module io and configure pins
  for (uint8_t i = 0; i < sizeof(pCONT_set->my_module.io); i++) {
    mpin = ValidPin(i, pCONT_set->my_module.io[i]);

    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("DBG: gpio pin %02d, mpin %d"), i, mpin);

    if (mpin) {

      #ifdef USE_MODULE_SENSORS_SWITCHES
      if ((mpin >= GPIO_SWT1_NP_ID) && (mpin < (GPIO_SWT1_NP_ID + MAX_SWITCHES))) {
        pCONT->mswh->SwitchPullupFlag(mpin - GPIO_SWT1_NP_ID);
        mpin -= (GPIO_SWT1_NP_ID - GPIO_SWT1_ID);
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("#ifdef USE_MODULE_SENSORS_SWITCHES %d mpin=%d"), i, mpin);
      }else
      #endif
      #ifdef USE_MODULE_SENSORS_BUTTONS
      if ((mpin >= GPIO_KEY1_NP_ID) && (mpin < (GPIO_KEY1_NP_ID + MAX_KEYS))) {
        pCONT->mbtn->ButtonPullupFlag(mpin - GPIO_KEY1_NP_ID);       //  0 .. 3
        mpin -= (GPIO_KEY1_NP_ID - GPIO_KEY1_ID);
      }
      else if ((mpin >= GPIO_KEY1_INV_ID) && (mpin < (GPIO_KEY1_INV_ID + MAX_KEYS))) {
        pCONT->mbtn->ButtonInvertFlag(mpin - GPIO_KEY1_INV_ID);      //  0 .. 3
        mpin -= (GPIO_KEY1_INV_ID - GPIO_KEY1_ID);
      }
      else if ((mpin >= GPIO_KEY1_INV_NP_ID) && (mpin < (GPIO_KEY1_INV_NP_ID + MAX_KEYS))) {
        pCONT->mbtn->ButtonPullupFlag(mpin - GPIO_KEY1_INV_NP_ID);   //  0 .. 3
        pCONT->mbtn->ButtonInvertFlag(mpin - GPIO_KEY1_INV_NP_ID);   //  0 .. 3
        mpin -= (GPIO_KEY1_INV_NP_ID - GPIO_KEY1_ID);
      }else
      #endif

      #ifdef USE_MODULE_DRIVERS_RELAY
      if ((mpin >= GPIO_REL1_INV_ID) && (mpin < (GPIO_REL1_INV_ID + MAX_RELAYS))) {
        bitSet(pCONT->mry->rel_inverted, mpin - GPIO_REL1_INV_ID);
        mpin -= (GPIO_REL1_INV_ID - GPIO_REL1_ID);
      }
      #endif

       if ((mpin >= GPIO_LED1_INV_ID) && (mpin < (GPIO_LED1_INV_ID + MAX_LEDS))) {
        bitSet(pCONT_set->led_inverted, mpin - GPIO_LED1_INV_ID);
        mpin -= (GPIO_LED1_INV_ID - GPIO_LED1_ID);
      }
      else if ((mpin >= GPIO_PWM1_INV_ID) && (mpin < (GPIO_PWM1_INV_ID + MAX_PWMS))) {
        bitSet(pCONT_set->pwm_inverted, mpin - GPIO_PWM1_INV_ID);
        mpin -= (GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
      }
      else if ((mpin >= GPIO_CNTR1_NP_ID) && (mpin < (GPIO_CNTR1_NP_ID + MAX_COUNTERS))) {
        bitSet(pCONT_set->counter_no_pullup, mpin - GPIO_CNTR1_NP_ID);
        mpin -= (GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
      }

     
      #ifdef USE_PWM
      // else if ((mpin >= AGPIO(GPIO_PWM1_INV_ID)) && (mpin < (AGPIO(GPIO_PWM1_INV_ID) + MAX_PWMS))) {
      //   bitSet(pCONT_set->pwm_inverted, mpin - AGPIO(GPIO_PWM1_INV_ID));
      //   mpin -= (AGPIO(GPIO_PWM1_INV_ID) - AGPIO(GPIO_PWM1_ID));
      // }
      #endif


    }
  
    #ifdef USE_LEGACY_PIN_METHOD
      if (mpin){ 
        pCONT_set->pin[mpin] = i;

        AddLog_P(LOG_LEVEL_INFO, PSTR("\n\rpin[%d] = %d \tmpin=%d\n\r"), mpin,pCONT_set->pin[mpin],i);

      }
    #endif
    
    //new way
    if(mpin){ SetPin(i, mpin); }                  // Anything above GPIO_NONE and below GPIO_SENSOR_END 
    
  }//end fof


//start each pin

  if ((2 == GetPin(GPIO_SERIAL_TX_ID)) || (MODULE_H801_ID == pCONT_set->my_module_type)) {
    Serial.set_tx(2);
  }

  // #ifdef USE_SERIAL_ALTERNATE_TX
  //   Serial.set_tx(2);
  //   Serial.println("WARNING: Untested"); Serial.flush();
  // #endif

  DEBUG_LINE; 
  //delay(2000);
  
// while(1);


// leave as default for testing
  // analogWriteRange(pCONT_set->Settings.pwm_range);      // Default is 1023 (Arduino.h)
  // analogWriteFreq(pCONT_set->Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)

#ifdef USE_SPI
  spi_flg = ((((pin[GPIO_SPI_CS] < 99) && (pin[GPIO_SPI_CS] > 14)) || (pin[GPIO_SPI_CS] < 12)) || (((pin[GPIO_SPI_DC] < 99) && (pin[GPIO_SPI_DC] > 14)) || (pin[GPIO_SPI_DC] < 12)));
  if (spi_flg) {
    for (uint16_t i = 0; i < GPIO_MAX; i++) {
      if ((pin[i] >= 12) && (pin[i] <=14)) pin[i] = 99;
    }
    my_module.io[12] = GPIO_SPI_MISO;
    pin[GPIO_SPI_MISO] = 12;
    my_module.io[13] = GPIO_SPI_MOSI;
    pin[GPIO_SPI_MOSI] = 13;
    my_module.io[14] = GPIO_SPI_CLK;
    pin[GPIO_SPI_CLK] = 14;
  }
  soft_spi_flg = ((pin[GPIO_SSPI_CS] < 99) && (pin[GPIO_SSPI_SCLK] < 99) && ((pin[GPIO_SSPI_MOSI] < 99) || (pin[GPIO_SSPI_MOSI] < 99)));
#endif  // USE_SPI
// #else // ESP32
//   analogWriteFreqRange(0, Settings.pwm_frequency, Settings.pwm_range);

// #ifdef USE_SPI
//   spi_flg = (PinUsed(GPIO_SPI_CLK) && (PinUsed(GPIO_SPI_MOSI) || PinUsed(GPIO_SPI_MISO)));
//   soft_spi_flg = (PinUsed(GPIO_SSPI_SCLK) && (PinUsed(GPIO_SSPI_MOSI) || PinUsed(GPIO_SSPI_MISO)));
// #endif  // USE_SPI
// #endif  // ESP8266 - ESP32

//new
// Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
  // Doing it here solves relay toggles at restart.
  for (uint32_t i = 0; i < sizeof(pCONT_set->my_module.io); i++){//ARRAY_SIZE(my_module.io); i++) {
  DEBUG_LINE;
    uint32_t mpin = ValidPin(i, pCONT_set->my_module.io[i]);
    
  DEBUG_LINE;
   AddLog_P(LOG_LEVEL_DEBUG, PSTR("INI: gpio pin %d, mpin %d"), i, mpin);
    if (((i < 6) || (i > 11)) && (0 == mpin)) {  // Skip SPI flash interface

      // if((i == 2)&&(pCONT_set->))

      #ifndef USE_SERIAL_ALTERNATE_TX
        if (!((1 == i) || (3 == i))) {             // Skip serial
          pinMode(i, INPUT);
        }
      #endif
    }
  }


// #ifdef USE_I2C
//   pCONT_set->i2c_flg = ((pCONT_set->pin[GPIO_I2C_SCL] < 99) && (pCONT_set->pin[GPIO_I2C_SDA] < 99));
//   if (pCONT_set->i2c_flg) { Wire.begin(pCONT_set->pin[GPIO_I2C_SDA], pCONT_set->pin[GPIO_I2C_SCL]); }
// #endif  // USE_I2C

  DEBUG_LINE; 




  /**
   *  Use pins to configure lights present
   * */
  pCONT_set->devices_present = 0;
  pCONT_set->Settings.light_settings.type = LT_BASIC;                     // Use basic PWM control if SetOption15 = 0
  // for a light type, func_module should see light as basic and return servicec
  AddLog_P(LOG_LEVEL_DEBUG,PSTR("Tasker_Interface(FUNC_MODULE_INIT)"));
  pCONT->Tasker_Interface(FUNC_MODULE_INIT); 




  // if (XdrvCall(FUNC_MODULE_INIT)) {
  //   // Serviced
  // }
  // else if (YTF_IR_BRIDGE == my_module_type) {
  //   ClaimSerial();  // Stop serial loopback mode
  // }
  // else if (SONOFF_DUAL == my_module_type) {
  //   Settings.flag_system_phaseout.mqtt_serial = 0;
  //   devices_present = 2;
  //   baudrate = 19200;
  // }
  // else if (CH4 == my_module_type) {
  //   Settings.flag_system_phaseout.mqtt_serial = 0;
  //   devices_present = 4;
  //   baudrate = 19200;
  // }
  // else if (SONOFF_SC == my_module_type) {
  //   Settings.flag_system_phaseout.mqtt_serial = 0;
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

// Set PWM immediately to limit unknown states
#ifdef USE_PWM
 for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1_ID, i)) {
      pinMode(Pin(GPIO_PWM1_ID, i), OUTPUT);
      
// #ifdef ESP32
//       analogAttach(Pin(GPIO_PWM1, i),i);
//       analogWriteFreqRange(i,Settings.pwm_frequency,Settings.pwm_range);
// #endif

      if (pCONT_set->Settings.light_settings.type) {      // force PWM GPIOs to low or high mode, see #7165
        analogWrite(Pin(GPIO_PWM1_ID, i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range : 0);
      } else {
        pCONT_set->pwm_present = true;
        analogWrite(Pin(GPIO_PWM1_ID, i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - pCONT_set->Settings.pwm_value[i] : pCONT_set->Settings.pwm_value[i]);
      }
    }
  }
  #endif



    #ifdef USE_MODULE_DRIVERS_RELAY
    // Configure relay pins
    for (uint8_t i = 0; i < MAX_RELAYS; i++) {
      if (PinUsed(GPIO_REL1_ID,i)) {
        pinMode(pCONT_set->pin[GPIO_REL1_ID +i], OUTPUT);
        pCONT_set->devices_present++;
        // if (MODULE_EXS_RELAY == pCONT_set->my_module_type) {
        //   digitalWrite(pCONT_set->pin[GPIO_REL1 +i], bitRead(pCONT->mry->rel_inverted, i) ? 1 : 0);
        //   if (i &1) { pCONT_set->devices_present--; }
        // }
      }
    }
    #endif
  // }

  // for (uint8_t i = 0; i < MAX_LEDS; i++) {
  //   if (PinUsed(GPIO_LED1_ID,i)) {
  //     pinMode(Pin(GPIO_LED1_ID,i), OUTPUT);
  //     digitalWrite(Pin(GPIO_LED1_ID,i), bitRead(pCONT_set->led_inverted, i));
  //   }
  // }
  
  for (uint32_t i = 0; i < MAX_LEDS; i++) {
    if (PinUsed(GPIO_LED1_ID, i)) {
// #ifdef USE_ARILUX_RF
//       if ((3 == i) && (leds_present < 2) && !PinUsed(GPIO_ARIRFSEL)) {
//         SetPin(Pin(GPIO_LED1, i), AGPIO(GPIO_ARIRFSEL));  // Legacy support where LED4 was Arilux RF enable
//       } else {
// #endif
        pinMode(Pin(GPIO_LED1_ID, i), OUTPUT);
        pCONT_set->leds_present++;
        digitalWrite(Pin(GPIO_LED1_ID, i), bitRead(pCONT_set->led_inverted, i));
// #ifdef USE_ARILUX_RF
//       }
// #endif
    }
  }
  if (PinUsed(GPIO_LEDLNK_ID)) {
    pinMode(Pin(GPIO_LEDLNK_ID), OUTPUT);
    digitalWrite(Pin(GPIO_LEDLNK_ID), pCONT_set->ledlnk_inverted);
  }

#ifdef USE_WS2812

//bring back, part of light types

  if (!pCONT_set->Settings.light_settings.type && (PinUsed(GPIO_RGB_DATA_ID))){  // RGB led
    pCONT_set->devices_present++;
    pCONT_set->Settings.light_settings.type = LT_WS2812;
  }
#endif  // USE_WS2812
// #ifdef USE_SM16716
//   if (SM16716_ModuleSelected()) {
//     Settings.light_settings.type += 3;
//     Settings.light_settings.type |= LT_SM16716;
//   }
// #endif  // ifdef USE_SM16716

  DEBUG_LINE;
// Serial.flush();
// while(1);
  // Basic PWM controls (PWM1-6)
  if (!pCONT_set->Settings.light_settings.type) {
    for (uint8_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
      if (PinUsed(GPIO_PWM1_ID,i)) {
        pCONT_set->pwm_present = true;
        pinMode(GetPin(GPIO_PWM1_ID,i), OUTPUT);
        analogWrite(GetPin(GPIO_PWM1_ID,i), bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - pCONT_set->Settings.pwm_value[i] : pCONT_set->Settings.pwm_value[i]);
        Serial.println(F("Setting analogWrite with basic light type"));
      }
    }
  }

  #ifndef DISABLE_SERIAL_LOGGING
  Serial.printf("Settings.light_settings.type=%d\n\r",pCONT_set->Settings.light_settings.type);
  #endif

  pCONT_sup->SetLedPower(pCONT_set->Settings.ledstate &8);
  pCONT_sup->SetLedLink(pCONT_set->Settings.ledstate &8);

  // pCONT->Tasker_Interface(FUNC_PRE_INIT); //called outside of this for better transparency
}




int16_t mHardwarePins::GetModuleIDbyName(const char* c){
  if(c=='\0'){ return -1; }
  if(strcmp_P(c,PM_MODULE_NAME_NODEMCU_CTR)==0){              return MODULE_NODEMCU_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_CTR)==0){    return MODULE_SONOFF_BASIC_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_H801_CTR)==0){            return MODULE_H801_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_MAGICHOME_CTR)==0){       return MODULE_MAGICHOME_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SHELLY1_CTR)==0){         return MODULE_SHELLY1_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SHELLY2P5_CTR)==0){         return MODULE_SHELLY2P5_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_IFAN03_CTR)==0){         return MODULE_SONOFF_IFAN03_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_USERMODULE_CTR)==0){      return USER_MODULE; }

  return -1;

}


const char* mHardwarePins::GetModuleNameByID(uint8_t id, char* buffer){
  switch(id){
    default:
    case MODULE_NODEMCU_ID:  memcpy_P(buffer, PM_MODULE_NAME_NODEMCU_CTR, sizeof(PM_MODULE_NAME_NODEMCU_CTR)); break;
    case MODULE_SONOFF_BASIC_ID:   memcpy_P(buffer, PM_MODULE_NAME_SONOFF_BASIC_CTR, sizeof(PM_MODULE_NAME_SONOFF_BASIC_CTR)); break;
    case MODULE_SONOFF_IFAN03_ID:   memcpy_P(buffer, PM_MODULE_NAME_SONOFF_IFAN03_CTR, sizeof(PM_MODULE_NAME_SONOFF_IFAN03_CTR)); break;
    case MODULE_H801_ID:   memcpy_P(buffer, PM_MODULE_NAME_H801_CTR, sizeof(PM_MODULE_NAME_H801_CTR)); break;
    case MODULE_MAGICHOME_ID:   memcpy_P(buffer, PM_MODULE_NAME_MAGICHOME_CTR, sizeof(PM_MODULE_NAME_MAGICHOME_CTR)); break;
    case MODULE_SHELLY1_ID:   memcpy_P(buffer, PM_MODULE_NAME_SHELLY1_CTR, sizeof(PM_MODULE_NAME_SHELLY1_CTR)); break;
    case MODULE_SHELLY2P5_ID:   memcpy_P(buffer, PM_MODULE_NAME_SHELLY2P5_CTR, sizeof(PM_MODULE_NAME_SHELLY2P5_CTR)); break;
    case USER_MODULE:    memcpy_P(buffer, PM_MODULE_NAME_USERMODULE_CTR, sizeof(PM_MODULE_NAME_USERMODULE_CTR)); break;

  }
  DEBUG_LINE;
  // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("%s=GetModuleNameByID"),buffer);
  return buffer;
}


// Function: Checks for gpio function as stored in progmem, returning its identifier if true, else -1
// Returns : #>=0:id found, -1: not found
// Notes: "else" not included in if structure as return is used 
//
int16_t mHardwarePins::GetGPIOFunctionIDbyName(const char* c){
  if(c=='\0'){ return -1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_NONE_CTR)==0){       return GPIO_NONE_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_DATA_CTR)==0){   return GPIO_RGB_DATA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_CLOCK_CTR)==0){  return GPIO_RGB_CLOCK_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_1_CTR)==0){  return GPIO_DHT22_1OF2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_2_CTR)==0){  return GPIO_DHT22_2OF2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_1_CTR)==0){  return GPIO_DSB_1OF2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_2_CTR)==0){  return GPIO_DSB_2OF2_ID; }

  
  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_NP_CTR)==0){  return GPIO_SWT1_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_NP_CTR)==0){  return GPIO_SWT2_NP_ID; }

  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_CTR)==0){  return GPIO_SWT1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_CTR)==0){  return GPIO_SWT2_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_INV_CTR)==0){  return GPIO_REL1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_INV_CTR)==0){  return GPIO_REL2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_INV_CTR)==0){  return GPIO_REL3_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_INV_CTR)==0){  return GPIO_REL4_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_CTR)==0){       return GPIO_REL1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_CTR)==0){       return GPIO_REL2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_CTR)==0){       return GPIO_REL3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_CTR)==0){       return GPIO_REL4_ID; }

  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_PWM1_CTR)==0){  return GPIO_FAN_PWM1_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SCL_CTR)==0){  return GPIO_I2C_SCL_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SDA_CTR)==0){  return GPIO_I2C_SDA_ID; }

  // #ifdef USE_MODULE_SENSORS_MOTION
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_CTR)==0){  return GPIO_PIR_1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_CTR)==0){  return GPIO_PIR_2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_CTR)==0){  return GPIO_PIR_3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_NP_CTR)==0){  return GPIO_PIR_1_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_NP_CTR)==0){  return GPIO_PIR_2_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_NP_CTR)==0){  return GPIO_PIR_3_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){  return GPIO_PIR_1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){  return GPIO_PIR_2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){  return GPIO_PIR_3_INV_ID; }
  // #endif
  // #ifdef USE_MODULE_SENSORS_DOOR
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_DETECT_CTR)==0){  return GPIO_DOOR_OPEN_ID; }
  // #endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_INPUT_CTR)==0){  return GPIO_CHIME_INPUT_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_RINGER_CTR)==0){  return GPIO_CHIME_RINGER_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_IRSEND_CTR)==0){  return GPIO_IRSEND_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_IRSEND_CTR)==0){  return GPIO_FAN_IRSEND_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM1_CTR)==0){  return GPIO_PWM1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM2_CTR)==0){  return GPIO_PWM2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM3_CTR)==0){  return GPIO_PWM3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM4_CTR)==0){  return GPIO_PWM4_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM5_CTR)==0){  return GPIO_PWM5_ID; }

  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_CTR)==0){  return GPIO_LEDLNK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_INV_CTR)==0){  return GPIO_LEDLNK_INV_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_OPEN_CTR)==0){  return GPIO_DOOR_OPEN_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_LOCK_CTR)==0){  return GPIO_DOOR_LOCK_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)==0){  return GPIO_SERIAL_DEBUG_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)==0){  return GPIO_SERIAL_DEBUG_RX_ID; }


  


  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED1_INV_CTR)==0){  return GPIO_LED1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED2_INV_CTR)==0){  return GPIO_LED2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED3_INV_CTR)==0){  return GPIO_LED3_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED4_INV_CTR)==0){  return GPIO_LED4_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED1_CTR)==0){  return GPIO_LED1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED2_CTR)==0){  return GPIO_LED2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED3_CTR)==0){  return GPIO_LED3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LED4_CTR)==0){  return GPIO_LED4_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_LED5_CTR)==0){  return GPIO_LED5_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_LED5_INV_CTR)==0){  return GPIO_LED5_INV_ID; }

  // #if defined(USE_MODULE_SENSORS_PZEM004T_MODBUS) || defined(USE_PZEM_DC)
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){  return GPIO_PZEM0XX_TX_ID; }
  // #endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){  return GPIO_PZEM004_RX_ID; }
  // #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM016_RX_CTR)==0){  return GPIO_PZEM016_RX_ID; }
  // #endif
  // #ifdef USE_MODULE_SENSORS_ULTRASONICS
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){  return GPIO_SR04_ECHO_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){  return GPIO_SR04_TRIG_ID; }
  // #endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_INV_CTR)==0){  return GPIO_KEY1_INV_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){  return GPIO_NEXTION_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){  return GPIO_NEXTION_RX_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_CTR)==0){  return GPIO_KEY1_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){  return GPIO_HBRIDGE_L9110_IA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){  return GPIO_HBRIDGE_L9110_IB_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){  return GPIO_HBRIDGE_L9110_OA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){  return GPIO_HBRIDGE_L9110_OB_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_ANALOG_POSITION_CTR)==0){  return GPIO_ANALOG_POSITION_ID; }


  return -1;

}



// actually use buffer and return it
const char* mHardwarePins::GetGPIOFunctionNamebyID_P(uint8_t id, char* buffer){

  if(buffer == nullptr){ return 0; }

  // memcpy_P(GetGPIOFunctionNamebyID_P(id)  
  //strcpy_P

}

PGM_P mHardwarePins::GetGPIOFunctionNamebyID_P(uint8_t id){
  switch(id){
    default:           return PM_SEARCH_NOMATCH;
    case GPIO_NONE_ID: return PM_GPIO_FUNCTION_NONE_CTR;
    // case GPIO_NONE_ID: return PM_GPIO_NONE_CTR;

//       GPIO_NONE_ID,         // Not used
//   GPIO_KEY1_ID,           // Buttons
//   GPIO_KEY1_NP_ID,
//   GPIO_KEY1_INV_ID,
//   GPIO_KEY1_INV_NP_ID,
//   GPIO_KEY2_ID,
//   GPIO_KEY2_NP_ID,
//   GPIO_KEY2_INV_ID,
//   GPIO_KEY2_INV_NP_ID,
//   GPIO_KEY3_ID,
//   GPIO_KEY3_NP_ID,
//   GPIO_KEY3_INV_ID,
//   GPIO_KEY3_INV_NP_ID,
//   GPIO_KEY4_ID,
//   GPIO_KEY4_NP_ID,
//   GPIO_KEY4_INV_ID,
//   GPIO_KEY4_INV_NP_ID,
      // case  GPIO_SWT1_ID: return PM_GPIO_FUNCTION_SWT1_CTR;            // User connected external switches
      case  GPIO_SWT1_NP_ID: return PM_GPIO_FUNCTION_SWT1_NP_CTR;        
//   GPIO_SWT1_NP_ID,
//   GPIO_SWT2_ID,
      case  GPIO_SWT2_NP_ID: return PM_GPIO_FUNCTION_SWT2_NP_CTR;        
//   GPIO_SWT3_ID,
//   GPIO_SWT3_NP_ID,
//   GPIO_SWT4_ID,
//   GPIO_SWT4_NP_ID,
//   GPIO_SWT5_ID,
//   GPIO_SWT5_NP_ID,
//   GPIO_SWT6_ID,
//   GPIO_SWT6_NP_ID,
//   GPIO_SWT7_ID,
//   GPIO_SWT7_NP_ID,
//   GPIO_SWT8_ID,
//   GPIO_SWT8_NP_ID,
      case  GPIO_REL1_ID: return PM_GPIO_FUNCTION_REL1_CTR;           // Relays
      case  GPIO_REL1_INV_ID: return PM_GPIO_FUNCTION_REL1_INV_CTR;           // Relays
      case  GPIO_REL2_ID: return PM_GPIO_FUNCTION_REL2_CTR;           // Relays
      case  GPIO_REL2_INV_ID: return PM_GPIO_FUNCTION_REL2_INV_CTR;           // Relays


      case  GPIO_FAN_PWM1_ID: return PM_GPIO_FUNCTION_FAN_PWM1_CTR;           // Relays
//   GPIO_REL2_ID,
//   GPIO_REL2_INV_ID,
//   GPIO_REL3_ID,
//   GPIO_REL3_INV_ID,
//   GPIO_REL4_ID,
//   GPIO_REL4_INV_ID,
//   GPIO_REL5_ID,
//   GPIO_REL5_INV_ID,
//   GPIO_REL6_ID,
//   GPIO_REL6_INV_ID,
//   GPIO_REL7_ID,
//   GPIO_REL7_INV_ID,
//   GPIO_REL8_ID,
//   GPIO_REL8_INV_ID,
//   GPIO_LED1_ID,           // Leds
//   GPIO_LED1_INV_ID,
//   GPIO_LED2_ID,
//   GPIO_LED2_INV_ID,
//   GPIO_LED3_ID,
//   GPIO_LED3_INV_ID,
//   GPIO_LED4_ID,
//   GPIO_LED4_INV_ID,
//   GPIO_PWM1_ID,           // RGB   Red   or C  Cold White
//   GPIO_PWM1_INV_ID,
//   GPIO_PWM2_ID,           // RGB   Green or CW Warm White
//   GPIO_PWM2_INV_ID,
//   GPIO_PWM3_ID,           // RGB   Blue
//   GPIO_PWM3_INV_ID,
//   GPIO_PWM4_ID,           // RGBW  (Cold) White
//   GPIO_PWM4_INV_ID,
//   GPIO_PWM5_ID,           // RGBCW Warm White
//   GPIO_PWM5_INV_ID,
//   GPIO_CNTR1_ID,          // Counters
//   GPIO_CNTR1_NP_ID,
//   GPIO_CNTR2_ID,
//   GPIO_CNTR2_NP_ID,
//   GPIO_CNTR3_ID,
//   GPIO_CNTR3_NP_ID,
//   GPIO_CNTR4_ID,
//   GPIO_CNTR4_NP_ID,
//   GPIO_SERIAL_TX_ID,            // Serial interface
//   GPIO_SERIAL_RX_ID,            // Serial interface
// #ifdef USE_I2C
//   GPIO_I2C_SCL_ID,        // I2C SCL
//   GPIO_I2C_SDA_ID,        // I2C SDA
// #endif
// #ifdef USE_SPI
//   GPIO_SPI_CS_ID,         // SPI Chip Select
//   GPIO_SPI_DC_ID,         // SPI Data Direction
//   GPIO_SSPI_MISO_ID,      // Software SPI Master Input Slave Output
//   GPIO_SSPI_MOSI_ID,      // Software SPI Master Output Slave Input
//   GPIO_SSPI_SCLK_ID,      // Software SPI Serial Clock
//   GPIO_SSPI_CS_ID,        // Software SPI Chip Select
//   GPIO_SSPI_DC_ID,        // Software SPI Data or Command
// #endif
// #ifdef USE_DISPLAY
//   GPIO_BACKLIGHT_ID,      // Display backlight control
// #endif
//   GPIO_DHT11_1OF2_ID,          // DHT11
//   GPIO_DHT11_2OF2_ID,          // DHT11
//   GPIO_DHT22_1OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
//   GPIO_DHT22_2OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
//   GPIO_SI7021_ID,         // iTead SI7021
// #if defined(USE_DS18B20) || defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
//   GPIO_DSB_1OF2_ID,            // Single wire DS18B20 or DS18S20
//   GPIO_DSB_2OF2_ID,            // Single wire DS18B20 or DS18S20
// #endif
// #ifdef USE_WS2812
//   GPIO_RGB_DATA_ID,         // WS2812 Led string
//   GPIO_RGB_CLOCK_ID,         // WS2812 Led string
// #endif
// #ifdef USE_IR_REMOTE
//   GPIO_IRSEND_ID,         // IR remote
// #ifdef USE_IR_RECEIVE
//   GPIO_IRRECV_ID,         // IR receiver
// #endif
// #endif
// #ifdef USE_RC_SWITCH
//   GPIO_RFSEND_ID,         // RF transmitter
//   GPIO_RFRECV_ID,         // RF receiver
// #endif
// #ifdef USE_RF_SENSOR
//   GPIO_RF_SENSOR_ID,      // Rf receiver with sensor decodingGPIO_RF_433MHZ_RX_ID
// #endif
// #ifdef USE_SR04
//   GPIO_SR04_TRIG_ID,      // SR04 Trigger pin
//   GPIO_SR04_ECHO_ID,      // SR04 Echo pin
// #endif
// #ifdef USE_TM1638
//   GPIO_TM16CLK_ID,        // TM1638 Clock
//   GPIO_TM16DIO_ID,        // TM1638 Data I/O
//   GPIO_TM16STB_ID,        // TM1638 Strobe
// #endif
// #ifdef USE_HX711
//   GPIO_HX711_SCK_ID,      // HX711 Load Cell clock
//   GPIO_HX711_DAT_ID,      // HX711 Load Cell data
// #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
//   GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
//   GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
//   GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
//   GPIO_HLW_CF_ID,         // HLW8012 CF power
//   GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
// #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_I2C) && defined(USE_ADE7953)
//   GPIO_ADE7953_IRQ_ID,    // ADE7953 IRQ
// #endif
//   GPIO_CSE7766_TX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
//   GPIO_CSE7766_RX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
// #if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
//   GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
// #endif
// // #if defined(USE_PZEM004T) || defined(USE_PZEM_AC) || defined(USE_PZEM_DC)
//   GPIO_PZEM0XX_TX_ID,     // PZEM0XX Serial interface
// // #endif
// // #ifdef USE_PZEM004T
//   GPIO_PZEM004_RX_ID,     // PZEM004T Serial interface
// // #endif
// // #ifdef USE_PZEM_AC
//   GPIO_PZEM016_RX_ID,     // PZEM-014_ID,016 Serial Modbus interface
// // #endif
// // #ifdef USE_PZEM_DC
//   GPIO_PZEM017_RX_ID,     // PZEM-003_ID,017 Serial Modbus interface
// // #endif
// #ifdef USE_SDM120
//   GPIO_SDM120_TX_ID,      // SDM120 Serial interface
//   GPIO_SDM120_RX_ID,      // SDM120 Serial interface
// #endif
// #ifdef USE_SDM630
//   GPIO_SDM630_TX_ID,      // SDM630 Serial interface
//   GPIO_SDM630_RX_ID,      // SDM630 Serial interface
// #endif
// #ifdef USE_SERIAL_BRIDGE
//   GPIO_SBR_TX_ID,         // Serial Bridge Serial interface
//   GPIO_SBR_RX_ID,         // Serial Bridge Serial interface
// #endif
// #ifdef USE_MHZ19
//   GPIO_MHZ_TXD_ID,        // MH-Z19 Serial interface
//   GPIO_MHZ_RXD_ID,        // MH-Z19 Serial interface
// #endif
// #ifdef USE_SENSEAIR
//   GPIO_SAIR_TX_ID,        // SenseAir Serial interface
//   GPIO_SAIR_RX_ID,        // SenseAir Serial interface
// #endif
// #ifdef USE_NOVA_SDS
//   GPIO_SDS0X1_TX_ID,      // Nova Fitness SDS011 Serial interface
//   GPIO_SDS0X1_RX_ID,      // Nova Fitness SDS011 Serial interface
// #endif
// #ifdef USE_PMS5003
//   GPIO_PMS5003_ID,        // Plantower PMS5003 Serial interface
// #endif
// #ifdef USE_TX20_WIND_SENSOR
//   GPIO_TX20_TXD_BLACK_ID, // TX20 Transmission Pin
// #endif
// #ifdef USE_MP3_PLAYER
//   GPIO_MP3_DFR562_ID,     // RB-DFR-562_ID, DFPlayer Mini MP3 Player Serial interface
// #endif
// #ifdef USE_TUYA_DIMMER
//   GPIO_TUYA_TX_ID,        // Tuya Serial interface
//   GPIO_TUYA_RX_ID,        // Tuya Serial interface
// #endif
// #ifdef USE_AZ7798
//   GPIO_AZ_TXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
//   GPIO_AZ_RXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
// #endif
// #ifdef USE_PN532_HSU
//   GPIO_PN532_TXD_ID,      // PN532 HSU Tx
//   GPIO_PN532_RXD_ID,      // PN532 HSU Rx
// #endif
// #ifdef USE_MGC3130
//   GPIO_MGC3130_XFER_ID,
//   GPIO_MGC3130_RESET_ID,
// #endif
// #ifdef USE_MAX31855
//   GPIO_MAX31855CS_ID,     // MAX31855 Serial interface
//   GPIO_MAX31855CLK_ID,    // MAX31855 Serial interface
//   GPIO_MAX31855DO_ID,     // MAX31855 Serial interface
// #endif
//   GPIO_DI_ID,             // my92x1 PWM input
//   GPIO_DCKI_ID,           // my92x1 CLK input
// #ifdef USE_SM16716
//   GPIO_SM16716_CLK_ID,    // SM16716 CLOCK
//   GPIO_SM16716_DAT_ID,    // SM16716 DATA
//   GPIO_SM16716_SEL_ID,    // SM16716 SELECT
// #endif // USE_SM16716
// #ifdef ROTARY_V1
//   GPIO_ROT1A_ID,          // Rotary switch1 A Pin
//   GPIO_ROT1B_ID,          // Rotary switch1 B Pin
//   GPIO_ROT2A_ID,          // Rotary switch2 A Pin
//   GPIO_ROT2B_ID,          // Rotary switch2 B Pin
// #endif
// #ifdef USE_ARILUX_RF
//   GPIO_ARIRFRCV_ID,       // AliLux RF Receive input
// #endif
// #ifdef USE_HRE
//   GPIO_HRE_CLOCK_ID,
//   GPIO_HRE_DATA
// #endif
// #ifdef USE_MODULE_SENSORS_DOOR
    case GPIO_DOOR_OPEN_ID: return PM_GPIO_FUNCTION_DOOR_OPEN_CTR;
// #endif
// #ifdef USE_SENSOR_DOOR_LOCK
//   GPIO_DOOR_LOCK_ID,
// #endif
// #ifdef USE_MODULE_DRIVERS_HBRIDGE
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_ANALOG_SENSE_ID,
// #endif

    case GPIO_PIR_1_ID: return PM_GPIO_FUNCTION_PIR_1_CTR;
    case GPIO_PIR_1_NP_ID: return PM_GPIO_FUNCTION_PIR_1_NP_CTR;
    case GPIO_PIR_1_INV_ID: return PM_GPIO_FUNCTION_PIR_1_INV_CTR;

    case GPIO_PIR_2_ID: return PM_GPIO_FUNCTION_PIR_2_CTR;
    case GPIO_PIR_2_NP_ID: return PM_GPIO_FUNCTION_PIR_2_NP_CTR;
    case GPIO_PIR_2_INV_ID: return PM_GPIO_FUNCTION_PIR_2_INV_CTR;

    case GPIO_PIR_3_ID: return PM_GPIO_FUNCTION_PIR_3_CTR;
    case GPIO_PIR_3_NP_ID: return PM_GPIO_FUNCTION_PIR_3_NP_CTR;
    case GPIO_PIR_3_INV_ID: return PM_GPIO_FUNCTION_PIR_3_INV_CTR;


    
    case GPIO_KEY1_ID: return PM_GPIO_FUNCTION_KEY1_CTR;
    
    case GPIO_KEY1_INV_ID: return PM_GPIO_FUNCTION_KEY1_INV_CTR;

    case GPIO_LEDLNK_INV_ID: return PM_GPIO_FUNCTION_LEDLNK_INV_CTR;

  }
}





