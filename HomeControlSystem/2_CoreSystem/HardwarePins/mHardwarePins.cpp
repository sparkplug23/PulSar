#include "mHardwarePins.h"

const char* mHardwarePins::PM_MODULE_CORE_HARDWAREPINS_CTR = D_MODULE_CORE_HARDWAREPINS_CTR;
const char* mHardwarePins::PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR = D_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR;



void mHardwarePins::Template_Load(){


    ReadModuleTemplateFromProgmem();
    // ParseModuleTemplate(); //probably goes elsewhere


}

int8_t mHardwarePins::Tasker(uint8_t function, JsonParserObject obj){

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // DEBUG_PRINTF("works=%d\n\r",works);
  
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
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  }

}

void mHardwarePins::parse_JSONCommand(JsonParserObject obj){}


void mHardwarePins::pre_init(){

}


void mHardwarePins::ModuleSettings_ShowTemplateLog()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),

  // char buffer[50];

  // AddLog(LOG_LEVEL_TEST,
  //   PSTR(
  //     "\n\r"
  //     //"Template: %s\n\r"
  //     "(Friendly)Name: %s\n\r"
  //     "MQTT Topic Prefix: %s\n\r"
  //     "Gpio: \n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r" //5
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r" //10
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r" //12
  //       "\t%d(%d):%d\n\r" //13
  //     "BASE : %08X (%s)\n\r"
  //   ),
  //   pCONT_set->Settings.user_template2.full_ctr,
  //   //pCONT_set->Settings.user_template2.hardware.name,
  //   pCONT_set->Settings.system_name.device,
  //   0,0,pCONT_set->Settings.user_template2.hardware.gp.io[0],
  //   1,1,pCONT_set->Settings.user_template2.hardware.gp.io[1],
  //   2,2,pCONT_set->Settings.user_template2.hardware.gp.io[2],
  //   3,3,pCONT_set->Settings.user_template2.hardware.gp.io[3],
  //   4,4,pCONT_set->Settings.user_template2.hardware.gp.io[4],
  //   5,5,pCONT_set->Settings.user_template2.hardware.gp.io[5],
  //   6,9,pCONT_set->Settings.user_template2.hardware.gp.io[6],
  //   7,10,pCONT_set->Settings.user_template2.hardware.gp.io[7],
  //   8,12,pCONT_set->Settings.user_template2.hardware.gp.io[8],
  //   9,13,pCONT_set->Settings.user_template2.hardware.gp.io[9],
  //   10,14,pCONT_set->Settings.user_template2.hardware.gp.io[10],
  //   11,15,pCONT_set->Settings.user_template2.hardware.gp.io[11],
  //   12,16,pCONT_set->Settings.user_template2.hardware.gp.io[12],
  //   pCONT_set->Settings.user_template2.base,
  //   GetModuleNameByID(pCONT_set->Settings.user_template2.base, buffer)
  // );

  DEBUG_LINE;
  
    // const char* buffer2 = GetModuleNameByID(pCONT_set->Settings.user_template2.base, buffer);
    // AddLog(LOG_LEVEL_INFO,PSTR("user_template2.base=%s"),buffer2);

}

void mHardwarePins::ModuleSettings_ShowActiveTemplate()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
  char buffer[50];

  // AddLog(LOG_LEVEL_TEST,
  //   PSTR(
  //     "\n\r"
  //     //"Template: %s\n\r"
  //     //"(Friendly)Name: %s\n\r"
  //     "MQTT Topic Prefix: %s\n\r"
  //     "Gpio: \n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //       "\t%d(%d):%d\n\r"
  //     "Module : %s\n\r"
  //   ),
  //   //pCONT_set->Settings.user_template2.full_ctr,
  //   //pCONT_set->Settings.user_template2.hardware.name,
  //   pCONT_set->Settings.system_name.device,
  //   0,0,pCONT_set->Settings.user_template2.hardware.gp.io[0],
  //   1,1,pCONT_set->Settings.user_template2.hardware.gp.io[1],
  //   2,2,pCONT_set->Settings.user_template2.hardware.gp.io[2],
  //   3,3,pCONT_set->Settings.user_template2.hardware.gp.io[3],
  //   4,4,pCONT_set->Settings.user_template2.hardware.gp.io[4],
  //   5,5,pCONT_set->Settings.user_template2.hardware.gp.io[5],
  //   6,9,pCONT_set->Settings.user_template2.hardware.gp.io[6],
  //   7,10,pCONT_set->Settings.user_template2.hardware.gp.io[7],
  //   8,12,pCONT_set->Settings.user_template2.hardware.gp.io[8],
  //   9,13,pCONT_set->Settings.user_template2.hardware.gp.io[9],
  //   10,14,pCONT_set->Settings.user_template2.hardware.gp.io[10],
  //   11,15,pCONT_set->Settings.user_template2.hardware.gp.io[11],
  //   12,16,pCONT_set->Settings.user_template2.hardware.gp.io[12],
  //   GetModuleNameByID(pCONT_set->Settings.module, buffer)
  // );

}


void mHardwarePins::ModuleSettings_FlashSerial()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
#ifdef ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
AddLog(LOG_LEVEL_TEST, PSTR("ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io)=%d"),ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));
#endif // ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  uint8_t real_gpio = 0;
  // DEBUG_PRINTF("\n\r");
  // DEBUG_PRINTF("Template: %s\n\r", pCONT_set->Settings.user_template.full_ctr);
  // DEBUG_PRINTF("Name: %s\n\r", pCONT_set->Settings.user_template2.hardware.name);
  for(uint8_t gpio=0;gpio<ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io);gpio++){
    //get proper gpio number

//change to pin getter
    real_gpio = GetPinByIndex(gpio);
    // switch(gpio){
    //   case 0: real_gpio = 0; break;
    //   case 1: real_gpio = 1; break;
    //   case 2: real_gpio = 2; break;
    //   case 3: real_gpio = 3; break;
    //   case 4: real_gpio = 4; break;
    //   case 5: real_gpio = 5; break;
    //   case 6: real_gpio = 9; break;
    //   case 7: real_gpio = 10; break;
    //   case 8: real_gpio = 12; break;
    //   case 9: real_gpio = 13; break;
    //   case 10: real_gpio = 14; break;
    //   case 11: real_gpio = 15; break;
    //   case 12: real_gpio = 16; break;
    // }

#ifdef ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    AddLog(LOG_LEVEL_TEST,PSTR("\t%d(%d):%d"),gpio,real_gpio,pCONT_set->Settings.user_template2.hardware.gp.io[gpio]);
#endif
  }
  // DEBUG_PRINTF("FLAGS: %08X\n\r",pCONT_set->Settings.user_template2.flags);
  // DEBUG_PRINTF("BASE : %08X\n\r",pCONT_set->Settings.user_template2.base);
  // DEBUG_PRINTF("\n\r\n\r"); 

}

void mHardwarePins::ReadModuleTemplateFromProgmem(){

  uint16_t progmem_size = sizeof(MODULE_TEMPLATE);
  progmem_size = progmem_size>MODULE_TEMPLATE_MAX_SIZE?MODULE_TEMPLATE_MAX_SIZE:progmem_size;
  // create parse buffer
  char buffer[progmem_size];
  // Read into local
  memcpy_P(buffer,MODULE_TEMPLATE,sizeof(MODULE_TEMPLATE));

  #ifdef ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR("MODULE_TEMPLATE Load"));// = \"%s\""), buffer);
  AddLog(LOG_LEVEL_DEBUG, PSTR("Load = \"%s\""), buffer);
  #endif // ENABLE_LOG_LEVEL_INFO
  #endif // ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  
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
  
  //DEVICENAME/TOPICNAME/MQTTNAME
  if(jtok = obj[PM_JSON_NAME]){
    const char* name_ctr = jtok.getStr();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "system_name %s"),name_ctr);
    // snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr); 
    snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s",name_ctr);  
  }

  // Name means friendly name (max 20 chars)
  if(jtok = obj[PM_JSON_FRIENDLYNAME]){
    const char* name_ctr = jtok.getStr();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template NAME %s"),name_ctr);
    snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr);
    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template Name %s"),name_ctr);
    //snprintf(pCONT_set->Settings.user_template2.hardware.name,sizeof(pCONT_set->Settings.user_template2.hardware.name),"%s",name_ctr);
  }

  // if(jtok = obj[PM_JSON_GPIO]){ 
  //   if(jtok.isArray()){
  //     uint8_t pin_num_count = 0;
  //     JsonParserArray array = obj[PM_JSON_GPIO];
  //     for(auto v : array) {
  //       int gpio_function_id = v.getInt();
  //       // FULL pin list
  //       pCONT_set->Settings.module_pins.io[pin_num_count] = gpio_function_id; 
  //       // Only existing pins
  //       pCONT_set->Settings.user_template2.hardware.gp.io[pin_num_count] = gpio_function_id;
  //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "GPIO[%d] = %d"),pin_num_count,gpio_function_id);  
  //       pin_num_count++;        
  //   }
  //   // Array of named functions "RGB_DATA"

  //   // Future, only require the pins needed

  //   // const char* name_ctr = obj[F("GPIO")];
  //   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Template FRIENDLYNAME %s"),name_ctr);
  //   // snprintf(Settings.system_name.friendly,sizeof(Settings.system_name.friendly),"%s",name_ctr);
  // }



  if(jtok = obj[PM_JSON_GPIOC]){ 

    pCONT_set->boot_status.module_template_parse_success = true;
    // clear pins to none
    for(int ii=0;ii<ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io);ii++){
      pCONT_set->Settings.user_template2.hardware.gp.io[ii] = GPIO_NONE_ID;
    }
    
    int8_t  real_pin = -1;
    int16_t gpio_number = -1;
          // AddLog(LOG_LEVEL_INFO, PSTR("jtok.size()=%d"),jtok.size());

          // delay(3000); 


    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){
      jtok.nextOne(); //skip start of object
      const char* key = jtok.getStr();
      
      real_pin = GetGPIONumberFromName(key);
      AddLog(LOG_LEVEL_INFO, PSTR("KEY%d %s %d\n\r"), pair_index, key, real_pin);

      int8_t index_pin = ConvertRealPinToIndexPin(real_pin);
      AddLog(LOG_LEVEL_INFO, PSTR("real_pin%d->index_pin%d\n\r"), real_pin, index_pin);
      
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
          
          #ifdef ENABLE_LOG_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
          AddLog(LOG_LEVEL_INFO, PSTR("hardware.gp.io[%d/%d] = %d SET"), 
            real_pin, 
            index_pin,
            pCONT_set->Settings.user_template2.hardware.gp.io[index_pin]
          );
          #endif
          // AddLog(LOG_LEVEL_INFO, PSTR("pin_number/indexed=%d %d, gpio_number=%d"), pin_number, pin_number_array_index, gpio_number);
        }else{
          AddLog(LOG_LEVEL_ERROR, PSTR("DECODE ERROR \"%s\" %d"),value, gpio_number);
        }

      }// end UsuableGPIOPin

    }

  }
    

    
  if(jtok = obj[PM_JSON_BASE]){
    
    const char* base_ctr = jtok.getStr();
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_CONFIG "Template BASE %s"),base_ctr);

    //GetBaseIDbyName();
      // pin_number = ;

    pCONT_set->Settings.module = GetModuleIDbyName(base_ctr);//USER_MODULE;

    char buffer[40];

    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("Settings.module=%s"),GetModuleNameByID(pCONT_set->Settings.module, buffer));

    //snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s",name_ctr);

  }else{
    pCONT_set->Settings.module = USER_MODULE;
  }






  // AddLog(LOG_LEVEL_INFO, PSTR(DEBUG_INSERT_PAGE_BREAK "ENDED"));
  ModuleSettings_FlashSerial();


  DEBUG_LINE;

  // delay(5000);/

// ParseModuleTemplate();



}

//
//const char PINS_WEMOS[] PROGMEM = "D3TXD4RXD2D1flashcFLFLolD6D7D5D8D0A0";

int8_t mHardwarePins::GetGPIONumberFromName(const char* c){

  int8_t pin = -1;
  #ifdef ESP8266
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
  else if(strcmp(c,"LBI")==0){ 
    #ifdef LED_BUILTIN
    pin = LED_BUILTIN; 
    #else
    pin = -1;
    #endif   
  }
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
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetGPIONumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("GetGPIONumberFromName = %d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  #endif // ESP8266

  // #ifdef ESP32
  
  // Check for pin_array matching
  char buffer[10];
  for(uint8_t i=0; i<ARRAY_SIZE(gpio_pin_by_index); i++)
  {
    sprintf(buffer,"%d\0",gpio_pin_by_index[i]);
    
      AddLog(LOG_LEVEL_INFO, PSTR("NOOOOOOOOOmatched pin %d %d %s %s"),pin,i,c,buffer);

    if(strcmp(c,buffer)==0)
    {
      pin = gpio_pin_by_index[i];
      AddLog(LOG_LEVEL_INFO, PSTR("matched pin %d %d"),pin,i);
      break;
    }else{

    }
    
  }

  // Names for pins
  if(strcmp(c,"LBI")==0){ 
    pin = 2;
  }

  // if(pin<0)
  // {
  //   pin = -1;
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetGPIONumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
  //   #endif // ENABLE_LOG_LEVEL_COMMANDS
  // }

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR("GetGPIONumberFromName = %d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  // #endif // ESP32

  return pin;

}

// int mHardwarePins::jsoneq(const char *json, jsmntok_t *tok, const char *s) {
//   if (tok->type == JSMN_STRING && (int)strlen(s) == tok->len &&
//       strncmp(json + tok->start, s, tok->len) == 0) {
//     return 0;
//   }
//   return -1;
// }


void mHardwarePins::ParseModuleTemplate(){

  // #if newname(1,2)

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
//}

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

// // Return true/false if pin can be used (check for flash pins etc)
// int8_t mHardwarePins::UsablePinToTemplateArrayIndex(uint8_t pin)
// {
//   // Valid
//   // if ((pin >= 0) && (pin <= 5)){
//   //   AddLog(LOG_LEVEL_INFO,PSTR("0-5 pin=%d to %d"),pin, pin);
//   //   return pin;
//   // }
//   // // GPIO
//   // else if (((pin >= 6) && (pin <= 8))||(pin == 11)){
//   //   return -1; // Flash pins
//   // }
//   // // flash pins on 8265
//   // else if ((pin == 9) || (pin == 10)){ //(MODULE_WEMOS_ID == pCONT_set->Settings.module) // not on esp8625
//   //   AddLog(LOG_LEVEL_INFO,PSTR("9/10 pin=%d to %d"),pin, pin-3);
//   //   return pin-3; // Flash pins
//   // }
//   // // Valid
//   // else if ((pin >= 12) && (pin <= 16)){
//   //   AddLog(LOG_LEVEL_INFO,PSTR("12-16 pin=%d to %d"),pin, pin-5);
//   //   return pin-5;
//   // }
//   // AddLog(LOG_LEVEL_DEBUG, PSTR("UsablePinToTemplateArrayIndex: %d->%d"), 
//   //   pin,
//   //   gpio_pin_by_index[pin]
//   // );
  
//   // return gpio_pin_by_index[pin];

//   switch(pin){
//     case 0: return 0;
//     case 1: return 1;
//     case 2: return 2;
//     case 3: return 3;
//     case 4: return 4;
//     case 5: return 5;
//     case 6:
//     case 7:
//     case 8: return -1;
//     case 9: return 6;
//     case 10: return 7;
//     case 11: return -1;
//     case 12: return 8;
//     case 13: return 9;
//     case 14: return 10;
//     case 15: return 11;
//     case 16: return 12;
//   }

    
//     // i, pCONT_set->my_module.io[i], mpin);

  


//   // return -1;
// }


int8_t mHardwarePins::GetPinWithGPIO(uint16_t gpio, uint8_t index) {
  return Pin(gpio, index);
}
uint32_t mHardwarePins::GetPin(uint32_t gpio, uint32_t index) {
  return Pin(gpio, index);
}


/**
 * @brief Searches for physical external pin number that a GPIO function is attached to ie "GetFunctionAttachedToPin"
 * @note The use of IRAM_ATTR means function resides in instruction RAM, so will execute faster than without IRAM_ATTR which would have to retrieve array from flash each time into ram again (slower)
 * @param gpio sensor, driver (eg Relay)
 * @param index if more than one gpio was set (eg Relay1, Relay2)
 * @return -1 if none found, or the pin number real_pin
 */
int8_t IRAM_ATTR mHardwarePins::Pin(uint32_t gpio, uint32_t index)
{
  uint16_t real_gpio = gpio + index;

  if(real_gpio == GPIO_NONE_ID)
    return -1;

  // Get any pins with gpio attached
  for(uint16_t index_pin=0; index_pin<ARRAY_SIZE(pin_attached_gpio_functions); index_pin++)
  {
    if(pin_attached_gpio_functions[index_pin] == real_gpio) // gpio has been used for a pin
    {
      return gpio_pin_by_index[index_pin]; // real_pin
      // AddLog(LOG_LEVEL_TEST, PSTR("Pin(%d,%d)pin_attached_gpio_functions[i] == real_gpio%d"),gpio,index,real_pin);
    }
  }

  return -1; // No pin was assigned with GPIO  
}

/**
 * @brief Convert the real_pin number to its indexed position within pin_array
 * @note 
 * @param real_pin physical external pin number
 * @return index position of pin in array
 */
int8_t mHardwarePins::ConvertRealPinToIndexPin(uint8_t real_pin){
  for(int index_pin=0;index_pin<MAX_GPIO_PIN;index_pin++)
  {
    if(real_pin == gpio_pin_by_index[index_pin])
    {
      return index_pin;
    }
  }
  return -1;
}

/**
 * @brief Returns true/false is the gpio is used on any pin
 * @note 
 * @param gpio sensor, driver (eg Relay)
 * @param index if more than one gpio was set (eg Relay1, Relay2)
 * @return true/false if GPIO is found
 */
boolean mHardwarePins::PinUsed(uint32_t gpio, uint32_t index) {
  return (Pin(gpio, index) >= 0);
}

/**
 * @brief Assigns a pin with a GPIO
 * @note 
 * @param lpin The real_pin number GPIO#
 * @param gpio The actual hardware attached to the pin (eg led, relay, sensor data)
 * @return none
 */
void mHardwarePins::SetPin(uint32_t real_pin, uint32_t gpio) {

  // Adjust real_pin to internal indexpin
  int8_t internal_pin_index = ConvertRealPinToIndexPin(real_pin);

  // uint8_t pin_count = ARRAY_SIZE(gpio_pin_by_index);
  // uint8_t

  // if((internal_pin_index < MAX_USER_PINS) && (internal_pin_index>=0))
  // {
    if(internal_pin_index != 1)
{
    AddLog(LOG_LEVEL_ERROR, PSTR("SetPin real_pin=%d  internal_index=%d gpio=%d"),real_pin,internal_pin_index,gpio);
  
  
  // // Check if valid gpio
  // if(lpin<MAX_USER_PINS)
  // {
    pin_attached_gpio_functions[internal_pin_index] = gpio;
  }
  else{
    AddLog(LOG_LEVEL_ERROR, PSTR("Error SetPin %d %d<%d"),gpio,internal_pin_index,MAX_USER_PINS);
  }
}



void mHardwarePins::DigitalWrite(uint32_t gpio_pin, uint32_t state)
{
  DigitalWrite(gpio_pin, 0, state);
  // if (pCONT_set->pin[gpio_pin] < 99) {
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "DigitalWrite(%d[%d],%d)"),pCONT_set->pin[gpio_pin],gpio_pin,state);
    // digitalWrite(pCONT_set->pin[gpio_pin], state &1);
  // }
}
void mHardwarePins::DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state)
{
  // if (PinUsed(gpio_pin, index)) {
  //   digitalWrite(GetPin(gpio_pin, index), state &1);
  // }else{
  // // }
  // digitalWrite(gpio_pin, state &1);

  uint8_t real_pin = Pin(gpio_pin, index);

    AddLog(LOG_LEVEL_TEST, PSTR("DigitalWrite %d %d %d %d"),gpio_pin,index, state &1,real_pin);
  // if (PinUsed(gpio_pin, index)) {
    digitalWrite(real_pin, state &1);
  // }
}



// #ifdef USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD


int8_t mHardwarePins::GetPinByIndex(uint8_t index)
{
  return gpio_pin_by_index[index];
}

int8_t mHardwarePins::GetPinIndexedLocation(uint8_t pin_number)
{
  for(uint8_t index = 0;index<MAX_USER_PINS;index++)
  {
    if(GetPinByIndex(index) == pin_number)
    {
      return index;
    }
  }
  return -1;
}

bool mHardwarePins::SetPinFunction(int8_t gpio_pin_number, int8_t pin_function)
{
  int8_t gpio_pin_index_location = GetPinIndexedLocation(gpio_pin_number);
  if(gpio_pin_index_location>=0) // Valid pin option
  {
    pin_attached_gpio_functions[gpio_pin_index_location] = pin_function;
    return true;
  }
  return false;
}

// #endif // USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD

uint8_t mHardwarePins::ModuleNr()
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == pCONT_set->Settings.module) ? 0 : pCONT_set->Settings.module +1;
}

bool mHardwarePins::ValidTemplateModule(uint8_t index)
{
  for (uint8_t i = 0; i < sizeof(kModuleNiceList_IDS); i++) {
    if (index == pgm_read_byte(kModuleNiceList_IDS + i)) {
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

bool mHardwarePins::ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index)
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
  // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE == index"));
    return D_NO_MATCH_CTR;//pCONT_set->Settings.user_template2.hardware.name; //returns pointer
  } else {
    // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE != %d index %s"),index,kModules[index].name);
    // return kModules[index].name;

//Change to use function

return GetModuleNameByID(index, buffer);
    // return pCONT_sup->GetTextIndexed_P(buffer, buflen, index, kModules_Name_list_ctr);

    


  }
}



#ifdef ESP8266
// void GetInternalTemplate(void* ptr, uint32_t module, uint32_t option) {
//   uint8_t module_template = pgm_read_byte(kModuleTemplateList + module);

// //  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Template %d, Option %d"), module_template, option);

//   // template8 = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc
//   uint8_t template8[sizeof(mytmplt8285)] = { GPIO_NONE };
//   if (module_template < TMP_WEMOS) {
//     memcpy_P(&template8, &kModules8266[module_template], 6);
//     memcpy_P(&template8[8], &kModules8266[module_template].gp.io[6], 6);
//   } else {
//     memcpy_P(&template8, &kModules8285[module_template - TMP_WEMOS], sizeof(template8));
//   }

// //  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)&template8, sizeof(mytmplt8285));

//   // template16  = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc,Flg
//   uint16_t template16[(sizeof(mytmplt) / 2)] = { GPIO_NONE };
//   TemplateConvert(template8, template16);

//   uint32_t index = 0;
//   uint32_t size = sizeof(mycfgio);      // template16[module_template].gp
//   switch (option) {
//     case 2: {
//       index = (sizeof(mytmplt) / 2) -1; // template16[module_template].flag
//       size = 2;
//       break;
//     }
//     case 3: {
//       size = sizeof(mytmplt);           // template16[module_template]
//       break;
//     }
//   }
//   memcpy(ptr, &template16[index], size);

// //  AddLog(LOG_LEVEL_DEBUG, PSTR("FNC: GetInternalTemplate option %d"), option);
// //  AddLogBufferSize(LOG_LEVEL_DEBUG, (uint8_t *)ptr, size / 2, 2);
// }
#endif  // ESP8266

const char* mHardwarePins::AnyModuleName2(uint8_t index)
{
  if (USER_MODULE == index) {
  // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE == index"));
    return D_NO_MATCH_CTR;//pCONT_set->Settings.user_template2.hardware.name;
  } else {
  // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_HTTP "USER_MODULE != index %s"),kModules[index].name);
    return D_NO_MATCH_CTR;//"";//FPSTR(kModules[index].name);
  }
}


void mHardwarePins::TemplateGPIOs(myio *gp)
{

  uint16_t *dest = (uint16_t *)gp;
  memset(dest, GPIO_NONE_ID, sizeof(myio));

  uint16_t src[ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io)];
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("src size =%d"),ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io));

    // sizeof(mycfgio)];
  if (USER_MODULE == pCONT_set->Settings.module) {
    memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR("USER_MODULE TemplateGPIOs memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));"));
    #endif // ENABLE_LOG_LEVEL_INFO
  } else {
    
    // AddLog(LOG_LEVEL_DEBUG, PSTR("TemplateGPIOs memcpy_P(&src, &kModules[pCONT_set->Settings.module].gp, sizeof(mycfgio));"));
    // delay(3000);
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
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint16_t *)&src, sizeof(mycfgio));

  //AddLog_Array(LOG_LEVEL_DEBUG_MORE, "TemplateGPIOs::gp", (uint16_t *)&src, (uint16_t) sizeof(mycfgio));

  uint8_t j = 0;
  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    
    AddLog(LOG_LEVEL_DEBUG, PSTR("TemplateGPIOs %d\t%d\t%d\t%d"),dest[j],j,src[i],i);
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

  // AddLog_Array(LOG_LEVEL_DEBUG_MORE, "TemplateGPIOs::gp", (uint8_t *)dest, (uint8_t)sizeof(mycfgio));
  // AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)gp, sizeof(myio));
}

void mHardwarePins::TemplateGPIOs(myio *gp, uint8_t module_id)
{

//  Uses settings.module and copies into gpio struct

  // uint8_t *dest = (uint8_t *)gp;
  // memset(dest, GPIO_NONE_ID, sizeof(myio));

  // uint8_t src[sizeof(mycfgio)];
  
  uint16_t *dest = (uint16_t *)gp;
  memset(dest, GPIO_NONE_ID, sizeof(myio));

  uint16_t src[ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io)];
  if (USER_MODULE == module_id) {
    
DEBUG_LINE;
    memcpy(&src, &pCONT_set->Settings.user_template2.hardware.gp, sizeof(mycfgio));
  } else {
DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
AddLog(LOG_LEVEL_DEBUG,PSTR("&kModules[module_id] %d"),module_id);
    #endif // ENABLE_LOG_LEVEL_INFO
    memcpy_P(&src, &kModules[module_id].gp, sizeof(mycfgio));
  }
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81
DEBUG_LINE;
//  AddLog_Array(LOG_LEVEL_DEBUG, "src", src, (uint8_t)sizeof(mycfgio));

  uint8_t j = 0;
  for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

//  AddLog_Array(LOG_LEVEL_DEBUG, "gp", (uint8_t *)gp, (uint8_t)sizeof(myio));
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

  // if (USER_MODULE == module) { module = WEMOS; }  // Generic
  // Settings.user_template_base = module;

// #ifdef ESP32
//   module = ModuleTemplate(module);
// #endif

//   char name[TOPSZ];
//   SettingsUpdateText(SET_TEMPLATE_NAME, GetTextIndexed(name, sizeof(name), module, kModuleNames));
// #ifdef ESP8266
//   GetInternalTemplate(&Settings.user_template, module, 3);
// #endif  // ESP8266
// #ifdef ESP32
//   memcpy_P(&Settings.user_template, &kModules[module], sizeof(mytmplt));
// #endif  // ESP32
}

void mHardwarePins::SetModuleType()
{
  pCONT_set->my_module_type = (USER_MODULE == pCONT_set->Settings.module) ? pCONT_set->Settings.user_template2.base : pCONT_set->Settings.module;
}

uint8_t mHardwarePins::ValidPin_AdjustGPIO(uint8_t pin, uint8_t gpio)
{
  uint8_t result = gpio;

  #ifdef ESP8266

  DEBUG_LINE;
  if (((pin > 5) && (pin < 9)) || (11 == pin)) {
    result = GPIO_NONE_ID;  // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }
  DEBUG_LINE;
  // need to add my other boards here
  if ((MODULE_WEMOS_ID == pCONT_set->Settings.module) && (!pCONT_set->Settings.flag_network.user_esp8285_enable)) {
    if ((pin == 9) || (pin == 10)) { result = GPIO_NONE_ID; }  // Disable possible flash GPIO9 and GPIO10
  }
  DEBUG_LINE;

  #endif

  return result;
}

bool mHardwarePins::ValidGPIO(uint8_t pin, uint8_t gpio)
{
  return (GPIO_USER_ID == ValidPin_AdjustGPIO(pin, gpio));  // Only allow GPIO_USER pins
}

bool mHardwarePins::GetUsedInModule(uint8_t val, uint8_t *arr)
{
  int offset = 0;

  if (!val) { return false; }  // None

  // if ((val >= GPIO_REL1_ID) && (val < GPIO_REL1_ID + MAX_RELAYS)) {
  //   offset = (GPIO_REL1_INV_ID - GPIO_REL1_ID);
  // }
  // if ((val >= GPIO_REL1_INV_ID) && (val < GPIO_REL1_INV_ID + MAX_RELAYS)) {
  //   offset = -(GPIO_REL1_INV_ID - GPIO_REL1_ID);
  // }

  // if ((val >= GPIO_LED1_ID) && (val < GPIO_LED1_ID + MAX_LEDS)) {
  //   offset = (GPIO_LED1_INV_ID - GPIO_LED1_ID);
  // }
  // if ((val >= GPIO_LED1_INV_ID) && (val < GPIO_LED1_INV_ID + MAX_LEDS)) {
  //   offset = -(GPIO_LED1_INV_ID - GPIO_LED1_ID);
  // }

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

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MODULE "GpioInit"));
  
    #endif // ENABLE_LOG_LEVEL_INFO
  // uint8_t mpin;
  uint16_t mgpio;
  #ifdef ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD
  // New method stores "function in pin array" instead of "pin in function array"
  uint16_t pin_function = GPIO_NONE_ID;
  #endif // ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

  /**
   * Correcting for invalid module
   * */
  if (!ValidModule(pCONT_set->Settings.module)) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_MODULE "!ValidModule"));
    #endif // ENABLE_LOG_LEVEL_INFO
    uint8_t module = MODULE;
    if (!ValidModule(MODULE)) { module = MODULE_WEMOS_ID; }
    pCONT_set->Settings.module = module;
    pCONT_set->Settings.last_module = module;
  }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_MODULE "ValidModule %d"),pCONT_set->Settings.module);
    #endif // ENABLE_LOG_LEVEL_INFO
  }
  SetModuleType();

  /**
   * Module changes
   * */
  if (pCONT_set->Settings.module != pCONT_set->Settings.last_module) {
    //pCONT_set->baudrate = APP_BAUDRATE;
  }

  /**
   * Correcting for invalid gpio functions
   * */
  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io); i++) {
    if(!ValidUserGPIOFunction(pCONT_set->Settings.user_template2.hardware.gp.io,i)){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CONFIG "!ValidUserGPIOFunction %d"),i);
    #endif // ENABLE_LOG_LEVEL_INFO
      pCONT_set->Settings.user_template2.hardware.gp.io[i] = GPIO_USER_ID;  // Fix not supported sensor ids in template    }
    }
  }

  myio def_gp;
  TemplateGPIOs(&def_gp); // Get template values

   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "ARRAY_SIZE%d"),ARRAY_SIZE(pCONT_set->Settings.module_pins.io));


   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "def_gp[%d]=%d"),2,def_gp.io[2]);
    


  for (uint8_t i = 0; i < ARRAY_SIZE(pCONT_set->Settings.module_pins.io); i++) { //all pins
    
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "%d=module_pins.io[%d]"),pCONT_set->Settings.module_pins.io[i],i);
    // #endif // ENABLE_LOG_LEVEL_INFO

    uint16_t gpio = pCONT_set->Settings.module_pins.io[i];
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "module_pins.io[%d]=%d"),i,pCONT_set->Settings.module_pins.io[i]);
    
    // If out of range, reset to none
    if(!ValidUserGPIOFunction(pCONT_set->Settings.module_pins.io,i)){
      pCONT_set->Settings.module_pins.io[i] = GPIO_NONE_ID;             // Fix not supported sensor ids in module
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "Unsupported module_pins.io %d being reset to GPIO_NONE"),i);
    #endif // ENABLE_LOG_LEVEL_INFO
    }
    // Set any user pins 
    else if (pCONT_set->Settings.module_pins.io[i] > GPIO_NONE_ID) {
      pCONT_set->my_module.io[i] = pCONT_set->Settings.module_pins.io[i];
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "my_module.io[i] = %d"),i,pCONT_set->my_module.io[i]);
    #endif // ENABLE_LOG_LEVEL_INFO
    }

    // Set any pins set in template
    if ((def_gp.io[i] >= GPIO_NONE_ID) && (def_gp.io[i] < GPIO_USER_ID)) { //ADDED >= to also copy NONE 
      pCONT_set->my_module.io[i] = def_gp.io[i];
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CONFIG "my_module.io[i] = def_gp.io[i]; %d %d %d \"%S\""),
        pCONT_set->my_module.io[i],
        def_gp.io[i],
        i,
        GetGPIOFunctionNamebyID_P(pCONT_set->my_module.io[i])
      );
    #endif // ENABLE_LOG_LEVEL_INFO
    }
    else{
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_CONFIG "Invalid IO in def_gp.io[%d]=%d"),i,def_gp.io[i]);
    }

  }
  pCONT_set->my_module_flag = ModuleFlag();

  // AddLog_Array(LOG_LEVEL_DEBUG_MORE, "my_module.io", (uint8_t *)&pCONT_set->my_module.io, (uint8_t)sizeof(pCONT_set->my_module.io));

  // Reset all pins to be "unset"
  // for (uint16_t i = 0; i < GPIO_MAX_ID; i++) {
  //   pCONT_set->pin[i] = 99;
  // }

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
  for (uint8_t pin = 0; pin < ARRAY_SIZE(pCONT_set->my_module.io); pin++) {

  // uint8_t real_pin = UsablePinToTemplateArrayIndex(pCONT_set->my_module.io[i]);

  // pin needs adjusting to indexed value

  // uint8_t i = pin;
  // if()

  uint16_t gpio = pCONT_set->my_module.io[pin];
  // uint8_t pin = i;
  AddLog(LOG_LEVEL_DEBUG, PSTR("pin=%d, gpio=%d"), pin, gpio);

  // // Get GPIO if pin is set
  
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: %d %d %d"), i, pCONT_set->my_module.io[i], mgpio);

    mgpio = ValidPin_AdjustGPIO(pin, gpio);

    //mgpio is really the gpio attached to pin if allowed
    
  // #ifdef ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD
  //   // pin_function = pCONT_set->my_module.io[i]; //function number
  //   // int8_t gpio_pin_number = mgpio;  //0+ if valid
  //   // SetPinFunction(gpio_pin_number, pin_function);  
  // #endif // ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: pin=%d moduleIO=%d  mgpio=%d"), pin, pCONT_set->my_module.io[pin], mgpio);

  //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   // if(mgpio){
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: io %02d, mgpio %d"), i, mgpio);
  //   // }
  //   // #endif // ENABLE_LOG_LEVEL_DEBUG

// Phasing section out : moving into their modules
    if (mgpio) {    
      
      //PWM
      if ((mgpio >= GPIO_PWM1_INV_ID) && (mgpio < (GPIO_PWM1_INV_ID + MAX_PWMS))) {
        bitSet(pCONT_set->pwm_inverted, mgpio - GPIO_PWM1_INV_ID);
        mgpio -= (GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
      }

      //COUNTERS
      else if ((mgpio >= GPIO_CNTR1_NP_ID) && (mgpio < (GPIO_CNTR1_NP_ID + MAX_COUNTERS))) {
        bitSet(pCONT_set->counter_no_pullup, mgpio - GPIO_CNTR1_NP_ID);
        mgpio -= (GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
      }

  

    }    
    //new way
    if(mgpio){ SetPin(pin, mgpio); }                  // Anything above GPIO_NONE and below GPIO_SENSOR_END 
    
  }//end fof

  #ifdef ESP8266
    if ((2 == GetPin(GPIO_HWSERIAL0_TX_ID)) || (MODULE_H801_ID == pCONT_set->my_module_type)) { 
      Serial.set_tx(2); 
      flag_serial_set_tx_set = true;
      
      }
  #endif

// leave as default for testing
  analogWriteRange(pCONT_set->Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(pCONT_set->Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
  // #endif

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

//new
// Set any non-used GPIO to INPUT - Related to resetPins() in support_legacy_cores.ino
  // Doing it here solves relay toggles at restart.
  for (uint32_t i = 0; i < ARRAY_SIZE(pCONT_set->my_module.io);
  
  
  // sizeof(pCONT_set->my_module.io); 
  i++){//ARRAY_SIZE(my_module.io); i++) {
  DEBUG_LINE;
    uint32_t mgpio = ValidPin_AdjustGPIO(i, pCONT_set->my_module.io[i]);
    
  DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("INI: gpio pin %d, mgpio %d"), i, mgpio);
    #endif // ENABLE_LOG_LEVEL_INFO
    if (((i < 6) || (i > 11)) && (0 == mgpio)) {  // Skip SPI flash interface

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

#ifdef USE_I2C
  pCONT_set->i2c_enabled = (PinUsed(GPIO_I2C_SCL_ID) && PinUsed(GPIO_I2C_SDA_ID));
  if (pCONT_set->i2c_enabled)
  { 
    if(pCONT_sup->wire == nullptr)
    {
      #ifdef ESP8266
      pCONT_sup->wire = new TwoWire();
      #else
      pCONT_sup->wire = new TwoWire(0);
      #endif
    }
    pCONT_sup->wire->begin(GetPin(GPIO_I2C_SDA_ID), GetPin(GPIO_I2C_SCL_ID));
  } // i2c_enabled
#endif  // USE_I2C
// #endif // ESP8266



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

// Set PWM immediately to limit unknown states
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
  // if (PinUsed(GPIO_LEDLNK_ID)) {
  //   pinMode(Pin(GPIO_LEDLNK_ID), OUTPUT);
  //   digitalWrite(Pin(GPIO_LEDLNK_ID), pCONT_set->ledlnk_inverted);
  // }

#ifdef USE_WS2812

//bring back, part of light types

  if (PinUsed(GPIO_RGB_DATA_ID)){  // RGB led
    pCONT_set->devices_present++;
    pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE;
    // AddLog(LOG_LEVEL_TEST, PSTR("Settings.light_settings.type && (PinUsed(GPIO_RGB_DATA_ID))"));
  }else{
    // AddLog(LOG_LEVEL_TEST, PSTR("NOT Settings.light_settings.type && (PinUsed(GPIO_RGB_DATA_ID))"));
  }
#endif  // USE_WS2812



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

  pCONT_sup->SetLedPower(pCONT_set->Settings.ledstate &8);
  pCONT_sup->SetLedLink(pCONT_set->Settings.ledstate &8);

  // pCONT->Tasker_Interface(FUNC_PRE_INIT); //called outside of this for better transparency
}




int16_t mHardwarePins::GetModuleIDbyName(const char* c){
  if(c=='\0'){ return -1; }
  if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_CTR)==0){    return MODULE_SONOFF_BASIC_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_H801_CTR)==0){            return MODULE_H801_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_MAGICHOME_CTR)==0){       return MODULE_MAGICHOME_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SHELLY1_CTR)==0){         return MODULE_SHELLY1_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SHELLY2P5_CTR)==0){         return MODULE_SHELLY2P5_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SHELLY_DIMMER2_CTR)==0){         return MODULE_SHELLY_DIMMER2_ID; }
  else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_IFAN03_CTR)==0){         return MODULE_SONOFF_IFAN03_ID; }

  #ifdef ESP32
  else if(strcmp_P(c,PM_MODULE_NAME_CAM_AITHINKER_CTR)==0){         return MODULE_CAM_AITHINKER_ID; }
  #endif

  else if(strcmp_P(c,PM_MODULE_NAME_USERMODULE_CTR)==0){      return USER_MODULE; }

  return -1;

}

//change to include buffer length limit
const char* mHardwarePins::GetModuleNameByID(uint8_t id, char* buffer){
  switch(id){
    default:
    #ifdef ESP8266
    case MODULE_SONOFF_BASIC_ID:   memcpy_P(buffer, PM_MODULE_NAME_SONOFF_BASIC_CTR, sizeof(PM_MODULE_NAME_SONOFF_BASIC_CTR)); break;
    case MODULE_SONOFF_IFAN03_ID:   memcpy_P(buffer, PM_MODULE_NAME_SONOFF_IFAN03_CTR, sizeof(PM_MODULE_NAME_SONOFF_IFAN03_CTR)); break;
    case MODULE_H801_ID:   memcpy_P(buffer, PM_MODULE_NAME_H801_CTR, sizeof(PM_MODULE_NAME_H801_CTR)); break;
    case MODULE_MAGICHOME_ID:   memcpy_P(buffer, PM_MODULE_NAME_MAGICHOME_CTR, sizeof(PM_MODULE_NAME_MAGICHOME_CTR)); break;
    case MODULE_SHELLY1_ID:   memcpy_P(buffer, PM_MODULE_NAME_SHELLY1_CTR, sizeof(PM_MODULE_NAME_SHELLY1_CTR)); break;
    case MODULE_SHELLY2P5_ID:   memcpy_P(buffer, PM_MODULE_NAME_SHELLY2P5_CTR, sizeof(PM_MODULE_NAME_SHELLY2P5_CTR)); break;
    case MODULE_SHELLY_DIMMER2_ID:   memcpy_P(buffer, PM_MODULE_NAME_SHELLY_DIMMER2_CTR, sizeof(PM_MODULE_NAME_SHELLY_DIMMER2_CTR)); break;
    #endif  // ESP8266
    #ifdef ESP32
    case MODULE_CAM_AITHINKER_ID:   memcpy_P(buffer, PM_MODULE_NAME_CAM_AITHINKER_CTR, sizeof(PM_MODULE_NAME_CAM_AITHINKER_CTR)); break;
    #endif // ESP32
    case USER_MODULE:    memcpy_P(buffer, PM_MODULE_NAME_USERMODULE_CTR, sizeof(PM_MODULE_NAME_USERMODULE_CTR)); break;

  }
  DEBUG_LINE;
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("%s=GetModuleNameByID"),buffer);
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

  else if(strcmp_P(c,PM_GPIO_FUNCTION_OLED_RESET_CTR)==0){  return GPIO_OLED_RESET_ID; }

  // #ifdef USE_MODULE_SENSORS_MOTION
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_CTR)==0){  return GPIO_PIR_1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_CTR)==0){  return GPIO_PIR_2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_CTR)==0){  return GPIO_PIR_3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_NP_CTR)==0){  return GPIO_PIR_1_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_NP_CTR)==0){  return GPIO_PIR_2_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_NP_CTR)==0){  return GPIO_PIR_3_NP_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_NP_INV_CTR)==0){  return GPIO_PIR_1_NP_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_NP_INV_CTR)==0){  return GPIO_PIR_2_NP_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_NP_INV_CTR)==0){  return GPIO_PIR_3_NP_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){  return GPIO_PIR_1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){  return GPIO_PIR_2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){  return GPIO_PIR_3_INV_ID; }
  // #endif
  // #ifdef USE_MODULE_SENSORS_DOOR
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_DETECT_CTR)==0){  return GPIO_DOOR_OPEN_ID; }
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

  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)==0){  return GPIO_HWSERIAL0_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)==0){  return GPIO_HWSERIAL0_RX_ID; }
#ifdef ESP32
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_TX_ID; }
#endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)==0){  return GPIO_GPS_SERIAL0_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)==0){  return GPIO_GPS_SERIAL0_RX_ID; }
  #ifdef ESP32
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR)==0){  return GPIO_GPS_SERIAL1_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR)==0){  return GPIO_GPS_SERIAL1_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR)==0){  return GPIO_GPS_SERIAL2_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR)==0){  return GPIO_GPS_SERIAL2_RX_ID; }
  #endif

  
  //else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_CTR)==0){  return GPIO_LEDLNK_ID; }
  //else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_INV_CTR)==0){  return GPIO_LEDLNK_INV_ID; }


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

  // #if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_DC)
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){  return GPIO_PZEM0XX_TX_ID; }
  // #endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){  return GPIO_PZEM004_RX_ID; }
  // #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM016_RX_CTR)==0){  return GPIO_PZEM016_RX_ID; }
  // #endif
  // #ifdef USE_MODULE_SENSORS_ULTRASONICS
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){  return GPIO_SR04_ECHO_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){  return GPIO_SR04_TRIG_ID; }
  // #endif
  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_INV_CTR)==0){  return GPIO_KEY1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY2_INV_CTR)==0){  return GPIO_KEY2_INV_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_CTR)==0){  return GPIO_KEY1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY2_CTR)==0){  return GPIO_KEY2_ID; }

  
  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_NP_CTR)==0){  return GPIO_KEY1_NP_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){  return GPIO_NEXTION_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){  return GPIO_NEXTION_RX_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_CTR)==0){  return GPIO_KEY1_ID; }


  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){  return GPIO_HBRIDGE_L9110_IA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){  return GPIO_HBRIDGE_L9110_IB_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){  return GPIO_HBRIDGE_L9110_OA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){  return GPIO_HBRIDGE_L9110_OB_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_ANALOG_POSITION_CTR)==0){  return GPIO_ANALOG_POSITION_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)==0){  return GPIO_ADE7953_IRQ_ID; }

  #ifdef ESP32
  // Webcam
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR)==0){  return GPIO_WEBCAM_DATA1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR)==0){  return GPIO_WEBCAM_DATA2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR)==0){  return GPIO_WEBCAM_DATA3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR)==0){  return GPIO_WEBCAM_DATA4_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR)==0){  return GPIO_WEBCAM_DATA5_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR)==0){  return GPIO_WEBCAM_DATA6_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR)==0){  return GPIO_WEBCAM_DATA7_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR)==0){  return GPIO_WEBCAM_DATA8_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)==0){   return GPIO_WEBCAM_XCLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)==0){   return GPIO_WEBCAM_PCLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)==0){   return GPIO_WEBCAM_HREF_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)==0){  return GPIO_WEBCAM_VSYNC_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)==0){   return GPIO_WEBCAM_SIOD_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)==0){   return GPIO_WEBCAM_SIOC_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)==0){   return GPIO_WEBCAM_PWDN_ID; }



  #endif // ESP32


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

    case GPIO_RGB_DATA_ID: return PM_GPIO_FUNCTION_RGB_DATA_CTR;

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
    case  GPIO_REL3_ID: return PM_GPIO_FUNCTION_REL3_CTR;           // Relays
    case  GPIO_REL3_INV_ID: return PM_GPIO_FUNCTION_REL3_INV_CTR;           // Relays
    case  GPIO_REL4_ID: return PM_GPIO_FUNCTION_REL4_CTR;           // Relays
    case  GPIO_REL4_INV_ID: return PM_GPIO_FUNCTION_REL4_INV_CTR;           // Relays


      case  GPIO_FAN_PWM1_ID: return PM_GPIO_FUNCTION_FAN_PWM1_CTR;           // Relays

      case  GPIO_OLED_RESET_ID: return PM_GPIO_FUNCTION_OLED_RESET_CTR;           // Relays

    case GPIO_LED1_ID:      return PM_GPIO_FUNCTION_LED1_CTR;
    case GPIO_LED1_INV_ID:  return PM_GPIO_FUNCTION_LED1_INV_CTR;
    case GPIO_LED2_ID:      return PM_GPIO_FUNCTION_LED2_CTR;
    case GPIO_LED2_INV_ID:  return PM_GPIO_FUNCTION_LED2_INV_CTR;
    case GPIO_LED3_ID:      return PM_GPIO_FUNCTION_LED3_CTR;
    case GPIO_LED3_INV_ID:  return PM_GPIO_FUNCTION_LED3_INV_CTR;
    case GPIO_LED4_ID:      return PM_GPIO_FUNCTION_LED4_CTR;
    case GPIO_LED4_INV_ID:  return PM_GPIO_FUNCTION_LED4_INV_CTR;

    case GPIO_PWM1_ID:      return PM_GPIO_FUNCTION_PWM1_CTR;
    case GPIO_PWM1_INV_ID:  return PM_GPIO_FUNCTION_PWM1_INV_CTR;
    case GPIO_PWM2_ID:      return PM_GPIO_FUNCTION_PWM2_CTR;
    case GPIO_PWM2_INV_ID:  return PM_GPIO_FUNCTION_PWM2_INV_CTR;
    case GPIO_PWM3_ID:      return PM_GPIO_FUNCTION_PWM3_CTR;
    case GPIO_PWM3_INV_ID:  return PM_GPIO_FUNCTION_PWM3_INV_CTR;
    case GPIO_PWM4_ID:      return PM_GPIO_FUNCTION_PWM4_CTR;
    case GPIO_PWM4_INV_ID:  return PM_GPIO_FUNCTION_PWM4_INV_CTR;
    case GPIO_PWM5_ID:      return PM_GPIO_FUNCTION_PWM5_CTR;
    case GPIO_PWM5_INV_ID:  return PM_GPIO_FUNCTION_PWM5_INV_CTR;


//   GPIO_CNTR1_ID,          // Counters
//   GPIO_CNTR1_NP_ID,
//   GPIO_CNTR2_ID,
//   GPIO_CNTR2_NP_ID,
//   GPIO_CNTR3_ID,
//   GPIO_CNTR3_NP_ID,
//   GPIO_CNTR4_ID,
//   GPIO_CNTR4_NP_ID,
//   GPIO_HWSERIAL0_TX_ID,            // Serial interface
//   GPIO_SERIAL_RX_ID,            // Serial interface
#ifdef USE_I2C
  case GPIO_I2C_SCL_ID: return PM_GPIO_FUNCTION_I2C_SCL_CTR;
  case GPIO_I2C_SDA_ID: return PM_GPIO_FUNCTION_I2C_SDA_CTR;
#endif
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
  case GPIO_DHT11_1OF2_ID: return PM_GPIO_FUNCTION_DHT11_1_CTR;
  case GPIO_DHT11_2OF2_ID: return PM_GPIO_FUNCTION_DHT11_2_CTR;
  case GPIO_DHT22_1OF2_ID: return PM_GPIO_FUNCTION_DHT22_1_CTR;
  case GPIO_DHT22_2OF2_ID: return PM_GPIO_FUNCTION_DHT22_2_CTR;
//   GPIO_SI7021_ID,         // iTead SI7021
// #if defined(USE_DS18B20) || defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
//   GPIO_DSB_1OF2_ID,            // Single wire DS18B20 or DS18S20
//   GPIO_DSB_2OF2_ID,            // Single wire DS18B20 or DS18S20

  case GPIO_DSB_1OF2_ID: return PM_GPIO_FUNCTION_DS18X20_1_CTR;
  case GPIO_DSB_2OF2_ID: return PM_GPIO_FUNCTION_DS18X20_2_CTR;
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
    case  GPIO_ADE7953_IRQ_ID: return PM_GPIO_FUNCTION_ADE7953_IRQ_CTR;        // ADE7953 IRQ
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

//GPS?
//  case GPIO_GPS_SERIAL0_RX

  case GPIO_NEXTION_RX_ID: return PM_GPIO_FUNCTION_NEXTION_RX_CTR;
  case GPIO_NEXTION_TX_ID: return PM_GPIO_FUNCTION_NEXTION_TX_CTR;


  #ifdef ESP32
  // Webcam
  case GPIO_WEBCAM_DATA1_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR;
  case GPIO_WEBCAM_DATA2_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR;
  case GPIO_WEBCAM_DATA3_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR;
  case GPIO_WEBCAM_DATA4_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR;
  case GPIO_WEBCAM_DATA5_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR;
  case GPIO_WEBCAM_DATA6_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR;
  case GPIO_WEBCAM_DATA7_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR;
  case GPIO_WEBCAM_DATA8_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR;
  case GPIO_WEBCAM_XCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR;
  case GPIO_WEBCAM_PCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR;
  case GPIO_WEBCAM_HREF_ID: return PM_GPIO_FUNCTION_WEBCAM_HREF_CTR;
  case GPIO_WEBCAM_VSYNC_ID: return PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR;
  case GPIO_WEBCAM_SIOD_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR;
  case GPIO_WEBCAM_SIOC_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR;
  case GPIO_WEBCAM_PWDN_ID: return PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR;



  #endif // ESP32


//    case GPIO_LEDLNK_INV_ID: return PM_GPIO_FUNCTION_LEDLNK_INV_CTR;

  }
}





