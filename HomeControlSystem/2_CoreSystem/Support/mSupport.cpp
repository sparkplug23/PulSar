
#include "2_CoreSystem/Support/mSupport.h"

const char* mSupport::PM_MODULE_CORE_SUPPORT_CTR = D_MODULE_CORE_SUPPORT_CTR;
const char* mSupport::PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR = D_MODULE_CORE_SUPPORT_FRIENDLY_CTR;



int8_t mSupport::Tasker(uint8_t function, JsonParserObject obj){

  // DEBUG_PRINT_FUNCTION_NAME_TEST;


  switch(function){
    case FUNC_INIT:
      // fSendTemplatesOnce = true;

      
      #ifdef ESP8266
        randomSeed(analogRead(0));
      #else
        // randomSeed(analogRead(34)); //esp32
      #endif


    break;
    case FUNC_LOOP: {

       
  // for(int n=0; n<10; ++n) {

  
  //   // DEBUG_PRINTF("%d = %d\n\r",n, GetNormalDistributionRandom(100,2,90,100));
  //   DEBUG_PRINTF("%d = %d\n\r",n, GetRandomSaturationVariation(100,5,95,100));

  //   // (100,10,90,100)

  // }

  // delay(3000);
    

    }break;
    case FUNC_EVERY_SECOND:{


  // char mqtt_data[300];
  // pCONT_sup->I2cScan(mqtt_data, sizeof(mqtt_data));
  // Serial.println(mqtt_data);
    //add back that I divide supports into subtasks to split tasks evenly across a second

      // #ifdef ENABLE_DEVFEATURE_TESTING_LONG_LOOPS
      //   return 0;
      // #endif
      PerformEverySecond();
    }break;
    case FUNC_EVERY_FIVE_MINUTE:
      //CmndCrash();
    break;
    case FUNC_ON_BOOT_SUCCESSFUL:



//move into another FUNC_BOOT_SUCCESS (or make success only happen after 10 seconds)
  // if (BOOT_LOOP_TIME == pCONT_time->uptime.seconds_nonreset) { //might need cast to be the same
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("mSupport::BOOT_LOOP_TIME == pCONT_time->uptime.seconds_nonreset"));
    #endif // ENABLE_LOG_LEVEL_INFO
    pCONT_set->RtcReboot.fast_reboot_count = 0;
    pCONT_set->RtcRebootSave();
    // pCONT_set->Settings.bootcount++;              // Moved to here to stop flash writes during start-up

    // delay(5000);
    
  // }

    break;

    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;



    case FUNC_WIFI_CONNECTED:
      ArduinoOTAInit();
    break;

  }
  
}





// Use overloading to get variabel type
uint8_t getIdentifierID(uint8_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint8_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint16_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint16_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint32_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint32_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(float x){ return IDENTIFIER_FLOAT_ID; }
uint8_t getIdentifierID(float* x){ return IDENTIFIER_FLOAT_ID; }
uint8_t getIdentifierID(char* x){ return IDENTIFIER_STRING_ID; }
uint8_t getIdentifierID(const char* x){ return IDENTIFIER_STRING_ID; }
    

void mSupport::AppendDList(char* buffer, const char* to_add){
  sprintf(buffer+strlen(buffer), "%s|", to_add);
}
void mSupport::AppendDList(char* buffer, uint16_t buflen, const char* formatP, ...)
{
  uint16_t length = strlen(buffer);
  if(length >= buflen){ return; }
  va_list arg;
  Serial.println(buffer);
  va_start(arg, formatP);  
  length += vsnprintf(buffer+length, buflen, formatP, arg);
  va_end(arg);  
  length += snprintf(buffer+length, buflen, "|");
}


// #ifdef ENABLE_DEVFEATURE_OTA_METHOD

#ifdef USE_ARDUINO_OTA
/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

void mSupport::ArduinoOTAInit(void)
{

  // add flag to only proceed if not set
  if(ota_init_success){ return; }

  #ifndef TEST_OTA_ISSUE
    ArduinoOTA.setHostname(pCONT_set->my_hostname);
  #endif
  ArduinoOTA.onStart([this]()
  {
    // #ifdef ESP8266
      //pCONT_set->SettingsSave(1);  // Free flash for OTA update
      //#ifdef USE_MODULE_NETWORK_WEBSERVER
        // if (pCONT_set->Settings.webserver) { 
          //pCONT_web->StopWebserver(); 
          // /}
      // #endif  // USE_MODULE_NETWORK_WEBSERVER
      //if (pCONT_set->Settings.flag_system.mqtt_enabled) { MqttDisconnect(); }
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "OTA " D_UPLOAD_STARTED));
    #endif// ENABLE_LOG_LEVEL_INFO
    // #endif
    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;
    
    // #ifdef ESP32
    //   pinMode(2,OUTPUT);
    //   // timerWrite(timerwdt, 0); //reset timer (feed watchdog)
    // #endif

    // Stop server otherwise OTA can fail
    // pCONT_web->StopWebserver();

    delay(100);       // Allow time for message xfer
  });

  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
  {
    if (pCONT_set->seriallog_level >= LOG_LEVEL_DEBUG) { // for when hardware serial is in use for modules

      uint8_t progress_now = (progress/(total/100));
      if(arduino_ota_progress_dot_count != progress_now){
        Serial.println(progress_now);
        arduino_ota_progress_dot_count = progress_now;
      }
      #ifdef ESP8266
      ESP.wdtFeed();
      #endif // ESP8266
    }
    
  });

  ArduinoOTA.onError([this](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[30];
    memset(error_str,0,sizeof(error_str));

    switch (error) {
      case OTA_AUTH_ERROR:    strncpy_P(error_str, PSTR("OTA_AUTH_ERROR"), sizeof(error_str)); break;    
      case OTA_BEGIN_ERROR:   strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_CONNECT_ERROR: sprintf(error_str, PSTR("Connect Error")); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR:     strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. %d " D_RESTARTING), error_str,ESP.getFreeSketchSpace());
    #endif // ENABLE_LOG_LEVEL_INFO
  
    if(error != OTA_BEGIN_ERROR)
      ESP.restart(); //should only reach if the first failed

  });

  ArduinoOTA.onEnd([this]()
  {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "OTA " D_SUCCESSFUL ". " D_RESTARTING));
    #endif
    ESP.restart();
	});

  ArduinoOTA.begin();
  ota_init_success = true;
  
   #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA mSUPPORT METHOD " D_ENABLED " " D_PORT " 8266"));
  #endif
}

void mSupport::ArduinoOtaLoop(void)
{
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  // Note async stuff can still occur, so I need to disable them
  while (arduino_ota_triggered){ ArduinoOTA.handle(); }
}

// #endif // ENABLE_DEVFEATURE_OTA_METHOD
#endif  // USE_ARDUINO_OTA


bool mSupport::JsonLevelFlagCheck(uint8_t json_level_testing, uint8_t json_level_set, uint8_t ischanged){

  // If we have exceeded minimal json level, true
  if(json_level_testing >= json_level_set){
    return true;
  }
  // If ischanged is set, check ifchanged
  if(ischanged){
    if(json_level_testing >= JSON_LEVEL_IFCHANGED){
      return true;
    }
  }
  return false;

}

char* mSupport::dtostrfd(double number, unsigned char prec, char *s)
{
  if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
    strcpy(s, "null");
    return s;
  } else {
    return dtostrf(number, 1, prec, s);
  }
}


//make template later
uint8_t mSupport::GetNormalDistributionRandom(uint8_t mean, uint8_t standard_deviation, uint8_t constrained_min, uint8_t constrained_max){

  std::random_device rd{};
  std::mt19937 gen{rd()};
  uint8_t result = 0;

  // values near the mean are the most likely
  // standard deviation affects the dispersion of generated values from the mean
  std::normal_distribution<> d{mean,standard_deviation};

  // std::map<int, int> hist{};
  // uint32_t array[10] = {0};
  // for(int n=0; n<10000; ++n) {
  //     array[round(d(gen))]++;
  // }

  result = round(d(gen));

  // If these are not equal, then apply contraint
  if(constrained_min != constrained_max){
    result = constrain(result, constrained_min, constrained_max);
  }

  return result; 

}





const char* mSupport::GetVersionBranchTypeNameByID(uint8_t id){
  return 0;
  // switch(id){
  //   case FIRMWARE_VERSION_TYPE_RELEASE_ID :           return PSTR("Release") ;
  //   case FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID : return PSTR("Release Candidate") ;     
  //   case FIRMWARE_VERSION_TYPE_BETA_ID :           return PSTR("Beta") ;
  //   case FIRMWARE_VERSION_TYPE_DEVELOPING_ID :        return PSTR("Development") ;
  // }
}

char mSupport::GetVersionBranchTypeCharNameByID(uint8_t id){
  switch(id){
    case FIRMWARE_VERSION_TYPE_RELEASE_ID : return 'R' ;
    case FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID : return 'C' ;     
    case FIRMWARE_VERSION_TYPE_BETA_ID : return 'B' ;
    case FIRMWARE_VERSION_TYPE_DEVELOPING_ID : return 'D' ;
  }
}

void mSupport::init_FirmwareVersion(){

  // Parse version for printing
  // snprintf_P(pCONT_set->my_version, sizeof(pCONT_set->my_version), PSTR("%d.%d.%d"), PROJECT_VERSION >> 24 & 0xff, PROJECT_VERSION >> 16 & 0xff, PROJECT_VERSION >> 8 & 0xff);  // Release version 6.3.0
  // if (PROJECT_VERSION & 0xff) {  // Development or patched version 6.3.0.10
  //   snprintf_P(pCONT_set->my_version, sizeof(pCONT_set->my_version), PSTR("%s.%d"), pCONT_set->my_version, PROJECT_VERSION & 0xff);
  // }

  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("pCONT_set->my_version=%s"),pCONT_set->my_version);
//DEBUG_PRINTF("pCONT_set->my_version=%s\n\r",pCONT_set->my_version);

  // Version Current
  pCONT_set->firmware_version.current.part_branch = (PROJECT_VERSION >> 30) & 0x03;
  pCONT_set->firmware_version.current.part_major =  (PROJECT_VERSION >> 24) & 0x3F;
  pCONT_set->firmware_version.current.part_minor =  (PROJECT_VERSION >> 16) & 0xff;
  pCONT_set->firmware_version.current.part_system = (PROJECT_VERSION >> 8 ) & 0xff;
  pCONT_set->firmware_version.current.part_module = (PROJECT_VERSION      ) & 0xff;
  pCONT_set->firmware_version.current.number =       PROJECT_VERSION;

  // Display Version Output

  // DEBUG_PRINTF("firmware_version.current = %X\n\r",PROJECT_VERSION);
    
  // char firmware_current[40];
  memset(pCONT_set->firmware_version.current.name_ctr,0,sizeof(pCONT_set->firmware_version.current.name_ctr));
  sprintf_P(pCONT_set->firmware_version.current.name_ctr,PSTR("%c%d.%d.%d.%d"),
      pCONT_sup->GetVersionBranchTypeCharNameByID(pCONT_set->firmware_version.current.part_branch),
      pCONT_set->firmware_version.current.part_major,
      pCONT_set->firmware_version.current.part_minor,
      pCONT_set->firmware_version.current.part_system,
      pCONT_set->firmware_version.current.part_module
  );

  
  // char message_version1[100];
  // sprintf(message_version1,PSTR("%s %c%d.%d.%d.%d%s"),
  //   PROJECT_NAME_CTR,
  //   pCONT_sup->GetVersionBranchTypeCharNameByID(pCONT_set->firmware_version.current.part_branch),
  //   pCONT_set->firmware_version.current.part_major,
  //   pCONT_set->firmware_version.current.part_minor,
  //   pCONT_set->firmware_version.current.part_system,
  //   pCONT_set->firmware_version.current.part_module,
  //   pCONT_set->firmware_version.fNewVersionAvailable ? " Update Available" : ""  
  // );


  //DEBUG_PRINTF("firmware_version.current = %s\n\r",firmware_current);
  // AddLog(LOG_LEVEL_INFO,PSTR("firmware_version.current = %s"),firmware_current);

  // char code_image[20];
  // snprintf_P(pCONT_set->my_image, sizeof(pCONT_set->my_image), PSTR("(%s)"), 
  //   GetTextIndexed_P(code_image, sizeof(code_image), CODE_IMAGE, kCodeImage));
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("pCONT_set->my_image=%s,CODE_IMAGE=%s"),pCONT_set->my_image,"CODE_IMAGE",CODE_IMAGE);

  // Serial.flush();

}


char* mSupport::GetVersionColour(char* buffer){

  if(pCONT_set->firmware_version.fCurrentVersionNotSupported){ //most important
    sprintf_P(buffer,"%s","#ff0000");
  }else
  if(pCONT_set->firmware_version.fNewVersionAvailable){
    sprintf_P(buffer,"%s","#ffef00");
  }else{
    sprintf_P(buffer,"%s","#00ff00");
  }
  return buffer;

}

float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// checks for 0 before dividing
int32_t mSupport::safeDivideInt(int32_t num, int32_t den)
{
  return (num!=0)?num/den:0;
}

// uint8_t mSupport::WITHINLIMITS(int minv, float var, int maxv){
//   if(isnan(var)){
//     return false;
//   }
//   return ((var>=minv)&&(var<=maxv)) ? 1 : 0;
// }

float mSupport::roundfloat(float in, uint8_t dec){
  //float f = round(in*(10*dec))/(10*dec);
  float f = round(in*(10*dec));
  f /= (10*dec);///100;
  return f;
}


// uint8_t mSupport::WITHINLIMITS(int minv, float var, int maxv){
//   //if((var>=minv)&&(var<=maxv)&&(!isnan(var))){

//   //Serial.println(isnan(var));
//   if(isnan(var)){
//     return false;
//   }

//   if((var>=minv)&&(var<=maxv)){
//       return true;
//   }else{
//       return false;
//   }

//   //return  ? 1 : 0;  //ternary operator (condition) ? (if_true) : (if_false)
// }




int mSupport::mSearchCtrIndexOf(const char* toSearch, const char* toFind){

  char *p = strstr(toSearch,toFind);

  if(p != NULL){
    return abs(toSearch - p); // get the position
  }else{
    return -1; //if null, it doesnt exist
  }

}


bool mSupport::SetTopicMatch(const char* toSearch, const char* set_topic_path){

  char buffer[60];
  snprintf(buffer, sizeof(buffer), "set/%s", set_topic_path);

  char *p = strstr(toSearch,buffer);

  if(p != NULL){
    // return abs(toSearch - p); // get the position
    return true;
  }else{
    // return -1; //if null, it doesnt exist
    return false;
  }

}

bool mSupport::SetTopicMatch_P(const char* toSearch, const char* set_topic_path){

  char buffer[60];
  snprintf_P(buffer, sizeof(buffer), PSTR("set/%S"), set_topic_path);

  char *p = strstr(toSearch,buffer);

  if(p != NULL){
    // return abs(toSearch - p); // get the position
    return true;
  }else{
    // return -1; //if null, it doesnt exist
    return false;
  }

}




// mSearchCtrIndexOf(const char* toSearch, const char* toFind){

//   char *p = strstr(toSearch,toFind);

//   if(p != NULL){
//     return abs(toSearch - p); // get the position
//   }else{
//     return -1; //if null, it doesnt exist
//   }

// }

// const char* mSupport::FloatToCStr(float f){
//   char str[10];
//   memset(str,0,sizeof(str));
//   dtostrf( f, 3, 4, str );
  
//   //sprintf(str,"%f",f);
//   return str;
// }


  // #ifdef ESP8266
  //   switch(ESP.getResetInfoPtr()->reason){
  //     // Good boot
  //     case REASON_DEFAULT_RST:
  //     case REASON_SOFT_RESTART:
  //     case REASON_DEEP_SLEEP_AWAKE:
  //       ota_startup_period_ms = 1000;
  //     break;
  //     // Bad boot
  //     case REASON_EXCEPTION_RST:
  //     case REASON_WDT_RST:
  //     case REASON_SOFT_WDT_RST:
  //     case REASON_EXT_SYS_RST: // not bad, but reset button will allow ota update
  //       ota_startup_period_ms = 10000;
  //     break;
  //   }
  // #endif

//return -1 if not found

int16_t mSupport::FindNearestValueIndexUInt8(uint8_t* tosearch, uint8_t tosearch_len, uint8_t tofind){

  //quick check
  for(int i=0;i<tosearch_len;i++){
    if(tosearch[i]==tofind){
      //AddLog(LOG_LEVEL_DEBUG,PSTR("if(tosearch[i]==tofind) || %d==%d"),tosearch[i],i);
      return i; //return index
    }else{
      //AddLog(LOG_LEVEL_DEBUG,PSTR("ELSE if(tosearch[i]==tofind) || %d==%d"),tosearch[i],i);
    }
  }

  //find nearest
  int16_t smallest_value=1000;//start with largest
  int16_t smallest_index = -1;
  int16_t value;
  for(int i=0;i<tosearch_len;i++){

    value = abs(tosearch[i]-tofind);
    //check smallest
    if(value<smallest_value){
      smallest_index = i;
      smallest_value = value;
    }
  }

  return smallest_index;

}

int32_t mSupport::FindNearestValueIndexUInt16(uint16_t* tosearch, uint16_t tosearch_len, uint16_t tofind){

  //quick check
  for(int i=0;i<tosearch_len;i++){
    if(tosearch[i]==tofind){
      //AddLog(LOG_LEVEL_DEBUG,PSTR("if(tosearch[i]==tofind) || %d==%d"),tosearch[i],i);
      return i; //return index
    }else{
      //AddLog(LOG_LEVEL_DEBUG,PSTR("ELSE if(tosearch[i]==tofind) || %d==%d"),tosearch[i],i);
    }
  }

  //find nearest
  int16_t smallest_value=1000;//start with largest
  int16_t smallest_index = -1;
  int16_t value;
  for(int i=0;i<tosearch_len;i++){

    value = abs(tosearch[i]-tofind);
    //check smallest
    if(value<smallest_value){
      smallest_index = i;
      smallest_value = value;
    }
  }

  return smallest_index;

}



// // Time elapsed function that updates the time when true
// bool mTime::TimeReached(uint32_t* tSaved, uint32_t ElapsedTime){
//   if(abs(millis()-*tSaved)>=ElapsedTime){ *tSaved=millis();
//     return true;
//   }
//   return false;
// }

// // Time elapsed function that updates the time when true  SAME AS REACHED, POSSIBLY BETTER NAME
// bool mTime::TimeElapsed(uint32_t* tSaved, uint32_t ElapsedTime){
//   if(abs(millis()-*tSaved)>=ElapsedTime){ *tSaved=millis();
//     return true;
//   }
//   return false;
// }





// // Time elapsed function that updates the time when true
// bool mTime::TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime){
//   if(abs(millis()-*tSaved)>=ElapsedTime){
//     return true;
//   }
//   return false;
// }

// // Time elapsed function that updates the time when true
// // WARNING: doesnt allow for wrap around
// bool mSupport::MillisReached(uint32_t* tTarget){
//   if(millis()>*tTarget){
//     return true;
//   }
//   return false;
// }



int mSupport::mSearchNCtrIndexOf(const char* toSearch, int length, const char* toFind){

  //Serial.print("mSearchStringForIndoxOf=toSearch>");Serial.println(toSearch);
  //Serial.print("toFind>");Serial.println(toFind);
  //Serial.println("Remaining strstr(toSearch,toFind)>");  Serial.println(strncmp(toSearch,toFind,length));

  //char *p = strncmp(toSearch,toFind,length+1);

  char *p = strstr(toSearch,toFind);

  if(p != NULL){
    return abs(toSearch - p); // get the position
  }else{
    return -1; //if null, it doesnt exist
  }

}


int16_t mSupport::findClosetArrayIndex_float(float* array, uint8_t array_len, float desired){





  return -1;
}

/*
char* mHVAC::append_str(char *here, const char *s) {
  int tmp = 0;
    while((*here++ = *s++)){tmp++;};
    Serial.println(tmp);
    Serial.println(here[0]);
    return here;//-1;
}

char* mHVAC::append_ul(char *here, unsigned long u) {
    char buf[20];       // we "just know" this is big enough

    return append_str(here, ultoa(u, buf, 10));
}

*/



int mSupport::memsearch(const char* dataset, int datasetLength, const char* target, int targetLen){
  //Serial.print(datasetLength);Serial.print("-");Serial.println(targetLen);
    for(int i = 0; i < datasetLength; i++){
        if(dataset[i] == target[0]){
            int found = 1;
            for(int j = 0; j < targetLen; j++){
              int k = i + j; //Serial.print(dataset[j]);Serial.println(target[k]);
              if(k >= datasetLength || target[j] != dataset[k]){
                      found = 0;
                      //Serial.println("FOUND 0");
                      break;
              }
            }
            if(found){
            //Serial.println("RETURN I");
            return i;
          }
        }
    }//Serial.println("RETURN -1");
    return -1;
}


void mSupport::PrintDebugger(char *in, unsigned char length){
  for(int i = 0;i<length;i++){
    Serial.print(F("i> "));Serial.print(i);Serial.print(F(" v> "));Serial.println(in[i]);
  }
}

int mSupport::NumDigits(int x)
{
    x = abs(x);
    return (x < 10 ? 1 :
        (x < 100 ? 2 :
        (x < 1000 ? 3 :
        (x < 10000 ? 4 :
        (x < 100000 ? 5 :
        (x < 1000000 ? 6 :
        (x < 10000000 ? 7 :
        (x < 100000000 ? 8 :
        (x < 1000000000 ? 9 :
        10)))))))));
}

uint16_t mSupport::NumCtr2Num(char* numctr, uint8_t numberlength){

  uint16_t numout=0;

  for(int digit=0;digit<numberlength;digit++){
    numout += (numctr[digit]-48)*pow(10,numberlength-digit-1);
  }

  return numout;
}


char *mSupport::strtolower(char *str)
{
    unsigned char *mystr = (unsigned char *)str;

    while (*mystr) {
        *mystr = tolower(*mystr);
        mystr++;
    }
    return str;
}

// Currently mqtt buffer, later shared buffer from this class
void mSupport::MQTTCommand_Add(const char* topic, const char* payload){; // Write command into mpkt struct and set as waiting

  // Clear mqtt packet with expectation to execute
  memset(&data_buffer.payload,0,sizeof(data_buffer.payload));

  data_buffer.topic.len = strlen(topic);
  data_buffer.payload.len = strlen(payload);

  memcpy(&data_buffer.topic.ctr,topic,data_buffer.topic.len);
  memcpy(&data_buffer.payload.ctr,payload,data_buffer.payload.len);
        
}

void mSupport::SlowAllTemplatesOnSerial(){

// // User template
//   DynamicJsonDocument doc(300);
//   JsonObject root = doc.to<JsonObject>();

//   char buffer[50];

//   root["name"] = pCONT_pins->AnyModuleName(pCONT_set->Settings.module, buffer, sizeof(buffer));
//   root["module_id"] = pCONT_set->Settings.module;

//   myio cmodule;
//   pCONT_pins->TemplateGPIOs(&cmodule,pCONT_set->Settings.module);

//   JsonArray gpio_arr = root.createNestedArray("GPIO");
//     for(uint8_t i=0;i<sizeof(cmodule);i++){ 
//       gpio_arr.add(cmodule.io[i]); 
//     }
  
//   D_DATA_BUFFER_CLEAR();
//   serializeJson(doc,data_buffer.payload.ctr);

//     char topic2[100];
//     sprintf_P(topic2,PSTR("status/templates/my_module"));
    
//     pCONT_mqtt->ppublish(topic2,data_buffer.payload.ctr,false);

//   AddLog_NoTime(LOG_LEVEL_TEST,PSTR("%s"), data_buffer.payload.ctr);


//   for (uint8_t mod = 0; mod < sizeof(kModuleNiceList_IDS); mod++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
      
//     DynamicJsonDocument doc2(700);
//     JsonObject root2 = doc2.to<JsonObject>();
//     uint8_t midx = pgm_read_byte(kModuleNiceList_IDS + mod);

//     myio cmodule;
//     pCONT_pins->TemplateGPIOs(&cmodule,midx);
  
//     JsonArray gpio_arr2 = root2.createNestedArray("GPIO");
//     for(int ii=0;ii<sizeof(cmodule);ii++){ 
//       gpio_arr2.add(cmodule.io[ii]); 
//     }
//     root2["name"] = pCONT_pins->AnyModuleName(midx, buffer, sizeof(buffer));
//     root2["mod"] = mod;
//     root2["midx"] = midx;

//       char stemp[20];

//     // JsonArray gpio_named_arr = root2.createNestedArray("GPIO_named");
//     // for(int ii=0;ii<sizeof(cmodule);ii++){ 
//     //   char stemp2[20];
//     //   pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), cmodule.io[ii], kSensorNames);
//     //   memcpy(stemp2,stemp,sizeof(stemp));
//     //   gpio_named_arr.add(stemp2);
//     // }
  
//     D_DATA_BUFFER_CLEAR();
//     serializeJson(doc2,data_buffer.payload.ctr);

//     char topic[100];
//     sprintf_P(topic,PSTR("status/templates/%02d"),mod);
    
//     pCONT->mqt->ppublish(topic,data_buffer.payload.ctr,false);
    
//     //delay(100);

//     AddLog_NoTime(LOG_LEVEL_TEST,PSTR("%s"),data_buffer.payload.ctr);

  // }









  // Response_P(PSTR("{\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), pCONT_set->Settings.user_template.name);
  // for (uint8_t i = 0; i < sizeof(pCONT_set->Settings.user_template2.hardware.gp); i++) {
  //   ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", pCONT_set->Settings.user_template2.hardware.gp.io[i]);
  // }
  // ResponseAppend_P(PSTR("],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), pCONT_set->Settings.user_template.flag, pCONT_set->Settings.user_template_base +1);
}



uint16_t mSupport::WriteBuffer_P(char* buffer, const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  int length = vsnprintf_P(buffer, DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
  va_end(arg);
  return length;
}

void mSupport::WriteBuffer_P(char* buffer, uint16_t* length, const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  *length += vsnprintf_P(&buffer[*length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
  va_end(arg);
  return;
}


// Assumes primary buffer
uint16_t mSupport::WriteBuffer_P(const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  char* buffer = data_buffer.payload.ctr;
  uint16_t length = strlen(buffer);
  uint16_t bytes_written = vsnprintf_P(&buffer[length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-length, formatP, arg);
  va_end(arg);
  return bytes_written;
}






// temporarily have response appends too 

int mSupport::Response_P(const char* format, ...)     // Content send snprintf_P char data
{
  //BufferWriter
  D_DATA_BUFFER_CLEAR();

  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), format, args);
  va_end(args);
  return len;
}

int mSupport::ResponseAppend_P(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int mlen = strlen(data_buffer.payload.ctr);
  int len = vsnprintf_P(data_buffer.payload.ctr + mlen, sizeof(data_buffer.payload.ctr) - mlen, format, args);
  va_end(args);
  return len + mlen;
}






uint32_t mSupport::ResetReason(void)
{
  /*
    user_interface.h
    REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
    REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
    REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
    REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
    REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
    REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
    REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
  */
 
#ifdef ESP8266
  return resetInfo.reason;
#else
  return 0;

#endif// ESP8266
}

void mSupport::SetPulseTimer(uint32_t index, uint32_t time)
{
  //pCONT_set->pulse_timer[index] = (time > 111) ? millis() + (1000 * (time - 100)) : (time > 0) ? millis() + (100 * time) : 0L;
}

uint32_t mSupport::GetPulseTimer(uint32_t index)
{
  // long time = TimePassedSince(pCONT_set->pulse_timer[index]);
  // if (time < 0) {
  //   time *= -1;
  //   return (time > 11100) ? (time / 1000) + 100 : (time > 0) ? time / 100 : 0;
  // }
  return 0;
}

String mSupport::GetResetReason(void)
{
  #ifdef ESP8266
  // char buff[32];
  // if (oswatch_blocked_loop) {
  //   strncpy_P(buff, PSTR(D_JSON_BLOCKED_LOOP), sizeof(buff));
  //   return String(buff);
  // } else {
    return ESP.getResetReason();
  // }
  #else
    return PSTR("Unsupported");
  #endif
}

const char* mSupport::GetResetReason(char* buffer, uint8_t buflen)
{
  #ifdef ESP8266
  // if (oswatch_blocked_loop) {
  //   strncpy_P(buffer, PSTR(D_JSON_BLOCKED_LOOP), buflen);
  // } else {
    sprintf(buffer, "%s", ESP.getResetReason().c_str());
  // }
  #else
    sprintf(buffer, "%s", "Unsupported");
  #endif
  return buffer;
}

bool mSupport::OsWatchBlockedLoop(void)
{
  //return oswatch_blocked_loop;
}

/*********************************************************************************************\
 * Miscellaneous
\*********************************************************************************************/

//#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// Functions not available in 2.3.0

// http://clc-wiki.net/wiki/C_standard_library:string.h:memchr
void* memchr(const void* ptr, int value, size_t num)
{
  unsigned char *p = (unsigned char*)ptr;
  while (num--) {
    if (*p != (unsigned char)value) {
      p++;
    } else {
      return p;
    }
  }
  return 0;
}

// http://clc-wiki.net/wiki/C_standard_library:string.h:strcspn
// Get span until any character in string
size_t strcspn(const char *str1, const char *str2)
{
  size_t ret = 0;
  while (*str1) {
    if (strchr(str2, *str1)) {  // Slow
      return ret;
    } else {
      str1++;
      ret++;
    }
  }
  return ret;
}

// https://opensource.apple.com/source/Libc/Libc-583/stdlib/FreeBSD/strtoull.c
// Convert a string to an unsigned long long integer
#ifndef __LONG_LONG_MAX__
#define __LONG_LONG_MAX__ 9223372036854775807LL
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1)
#endif

// unsigned long long strtoull(const char *__restrict nptr, char **__restrict endptr, int base)
// {
//   const char *s = nptr;
//   char c;
//   do { c = *s++; } while (isspace((unsigned char)c));                         // Trim leading spaces

//   int neg = 0;
//   if (c == '-') {                                                             // Set minus flag and/or skip sign
//     neg = 1;
//     c = *s++;
//   } else {
//     if (c == '+') {
//       c = *s++;
//     }
//   }

//   if ((base == 0 || base == 16) && (c == '0') && (*s == 'x' || *s == 'X')) {  // Set Hexadecimal
//     c = s[1];
//     s += 2;
//     base = 16;
//   }
//   if (base == 0) { base = (c == '0') ? 8 : 10; }                              // Set Octal or Decimal

//   unsigned long long acc = 0;
//   int any = 0;
//   if (base > 1 && base < 37) {
//     unsigned long long cutoff = ULLONG_MAX / base;
//     int cutlim = ULLONG_MAX % base;
//     for ( ; ; c = *s++) {
//       if (c >= '0' && c <= '9')
//         c -= '0';
//       else if (c >= 'A' && c <= 'Z')
//         c -= 'A' - 10;
//       else if (c >= 'a' && c <= 'z')
//         c -= 'a' - 10;
//       else
//         break;

//       if (c >= base)
//         break;

//       if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
//         any = -1;
//       else {
//         any = 1;
//         acc *= base;
//         acc += c;
//       }
//     }
//     if (any < 0) {
//       acc = ULLONG_MAX;                                                       // Range error
//     }
//     else if (any && neg) {
//       acc = -acc;
//     }
//   }

//   if (endptr != nullptr) { *endptr = (char *)(any ? s - 1 : nptr); }

//   return acc;
// }
// //#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

//Get span until single character in string
size_t mSupport::strchrspn(const char *str1, int character)
{
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}

// Function to return a substring defined by a delimiter at an index
char* mSupport::subStr(char* dest, char* str, const char *delim, int index)
{
  // char *act;
  // char *sub = nullptr;
  // char *ptr;
  // int i;

  // // Since strtok consumes the first arg, make a copy
  // strncpy(dest, str, strlen(str)+1);
  // for (i = 1, act = dest; i <= index; i++, act = nullptr) {
  //   sub = strtok_r(act, delim, &ptr);
  //   if (sub == nullptr) break;
  // }
  // sub = Trim(sub);
  // return sub;
}

double mSupport::CharToDouble(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  while ((*pt != '\0') && isblank(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt=='+') { pt++; }            // Skip any sign

  double left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  double right = 0;
  if (*pt == '.') {
    pt++;
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0;
    }
  }

  double result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}

int mSupport::TextToInt(char *str)
{
  char *p;
  uint8_t radix = 10;
  if ('#' == str[0]) { //for conversion of RGB #rrggbb
    radix = 16;
    str++;
  }
  return strtol(str, &p, radix);
}

// char* mSupport::ulltoa(unsigned long long value, char *str, int radix)
// {
// //   char digits[64];
// //   char *dst = str;
// //   int i = 0;
// //   int n = 0;

// // //  if (radix < 2 || radix > 36) { radix = 10; }

// //   do {
// //     n = value % radix;
// //     digits[i++] = (n < 10) ? (char)n+'0' : (char)n-10+'A';
// //     value /= radix;
// //   } while (value != 0);

// //   while (i > 0) { *dst++ = digits[--i]; }

// //   *dst = 0;

// //   return str;
// }



char* mSupport::Unescape(char* buffer, uint16_t* size)
{
//   uint8_t* read = (uint8_t*)buffer;
//   uint8_t* write = (uint8_t*)buffer;
//   int16_t start_size = *size;
//   int16_t end_size = *size;
//   uint8_t che = 0;

// //  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)buffer, *size);

//   while (start_size > 0) {
//     uint8_t ch = *read++;
//     start_size--;
//     if (ch != '\\') {
//       *write++ = ch;
//     } else {
//       if (start_size > 0) {
//         uint8_t chi = *read++;
//         start_size--;
//         end_size--;
//         switch (chi) {
//           case '\\': che = '\\'; break;  // 5C Backslash
//           case 'a': che = '\a'; break;   // 07 Bell (Alert)
//           case 'b': che = '\b'; break;   // 08 Backspace
//           case 'e': che = '\e'; break;   // 1B Escape
//           case 'f': che = '\f'; break;   // 0C Formfeed
//           case 'n': che = '\n'; break;   // 0A Linefeed (Newline)
//           case 'r': che = '\r'; break;   // 0D Carriage return
//           case 's': che = ' ';  break;   // 20 Space
//           case 't': che = '\t'; break;   // 09 Horizontal tab
//           case 'v': che = '\v'; break;   // 0B Vertical tab
//           case 'x': {
//             uint8_t* start = read;
//             che = (uint8_t)strtol((const char*)read, (char**)&read, 16);
//             start_size -= (uint16_t)(read - start);
//             end_size -= (uint16_t)(read - start);
//             break;
//           }
//           case '"': che = '\"'; break;   // 22 Quotation mark
// //          case '?': che = '\?'; break;   // 3F Question mark
//           default : {
//             che = chi;
//             *write++ = ch;
//             end_size++;
//           }
//         }
//         *write++ = che;
//       }
//     }
//   }
//   *size = end_size;

// //  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)buffer, *size);

//   return buffer;
}

char* mSupport::RemoveSpace(char* p)
{
//   char* write = p;
//   char* read = p;
//   char ch = '.';

//   while (ch != '\0') {
//     ch = *read++;
//     if (!isspace(ch)) {
//       *write++ = ch;
//     }
//   }
// //  *write = '\0';  // Removed 20190223 as it buffer overflows on no isspace found - no need either
//   return p;
}

char* mSupport::LowerCase(char* dest, const char* source)
{
  // char* write = dest;
  // const char* read = source;
  // char ch = '.';

  // while (ch != '\0') {
  //   ch = *read++;
  //   *write++ = tolower(ch);
  // }
  // return dest;
}

char* mSupport::UpperCase(char* dest, const char* source)
{
  // char* write = dest;
  // const char* read = source;
  // char ch = '.';

  // while (ch != '\0') {
  //   ch = *read++;
  //   *write++ = toupper(ch);
  // }
  // return dest;
}

char* mSupport::UpperCase_P(char* dest, const char* source)
{
  // char* write = dest;
  // const char* read = source;
  // char ch = '.';

  // while (ch != '\0') {
  //   ch = pgm_read_byte(read++);
  //   *write++ = toupper(ch);
  // }
  // return dest;
}

char* mSupport::Trim(char* p)
{
  // while ((*p != '\0') && isblank(*p)) { p++; }  // Trim leading spaces
  // char* q = p + strlen(p) -1;
  // while ((q >= p) && isblank(*q)) { q--; }   // Trim trailing spaces
  // q++;
  // *q = '\0';
  // return p;
  if (*p != '\0') {
    while ((*p != '\0') && isblank(*p)) { p++; }  // Trim leading spaces
    char* q = p + strlen(p) -1;
    while ((q >= p) && isblank(*q)) { q--; }   // Trim trailing spaces
    q++;
    *q = '\0';
  }
  return p;
}

char* mSupport::NoAlNumToUnderscore(char* dest, const char* source)
{
  // char* write = dest;
  // const char* read = source;
  // char ch = '.';

  // while (ch != '\0') {
  //   ch = *read++;
  //   *write++ = (isalnum(ch) || ('\0' == ch)) ? ch : '_';
  // }
  // return dest;
}

void mSupport::SetShortcut(char* str, uint8_t action)
{
  // if ('\0' != str[0]) {     // There must be at least one character in the buffer
  //   str[0] = '0' + action;  // SC_CLEAR, SC_DEFAULT, SC_USER
  //   str[1] = '\0';
  // }
}

uint8_t mSupport::Shortcut(const char* str)
{
  // uint8_t result = 10;

  // if ('\0' == str[1]) {    // Only allow single character input for shortcut
  //   if (('"' == str[0]) || ('0' == str[0])) {
  //     result = SC_CLEAR;
  //   } else {
  //     result = atoi(str);  // 1 = SC_DEFAULT, 2 = SC_USER
  //     if (0 == result) {
  //       result = 10;
  //     }
  //   }
  // }
  // return result;
}

bool mSupport::ValidIpAddress(const char* str)
{
  const char* p = str;

  while (*p && ((*p == '.') || ((*p >= '0') && (*p <= '9')))) { p++; }
  return (*p == '\0');
}

bool mSupport::ParseIp(uint32_t* addr, const char* str)
{
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;

  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(str, nullptr, 10);        // Convert byte
    str = strchr(str, '.');
    if (str == nullptr || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  return (3 == i);
}


// Function to parse & check if version_str is newer than our currently installed version.
bool mSupport::NewerVersion(char* version_str)
{
  uint32_t version = 0;
  uint8_t i = 0;
  char *str_ptr;
  char* version_dup = strdup(version_str);  // Duplicate the version_str as strtok_r will modify it.

  if (!version_dup) {
    return false;  // Bail if we can't duplicate. Assume bad.
  }
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(PROJECT_VERSION); str = strtok_r(nullptr, ".", &str_ptr), i++) {
    int field = atoi(str);
    // The fields in a version string can only range from 0-255.
    if ((field < 0) || (field > 255)) {
      free(version_dup);
      return false;
    }
    // Shuffle the accumulated bytes across, and add the new byte.
    version = (version << 8) + field;
    // Check alpha delimiter after 1.2.3 only
    if ((2 == i) && isalpha(str[strlen(str)-1])) {
      field = str[strlen(str)-1] & 0x1f;
      version = (version << 8) + field;
      i++;
    }
  }
  free(version_dup);  // We no longer need this.
  // A version string should have 2-4 fields. e.g. 1.2, 1.2.3, or 1.2.3a (= 1.2.3.1).
  // If not, then don't consider it a valid version string.
  if ((i < 2) || (i > sizeof(PROJECT_VERSION))) {
    return false;
  }
  // Keep shifting the parsed version until we hit the maximum number of tokens.
  // PROJECT_VERSION stores the major number of the version in the most significant byte of the uint32_t.
  while (i < sizeof(PROJECT_VERSION)) {
    version <<= 8;
    i++;
  }
  // Now we should have a fully constructed version number in uint32_t form.
  return (version > PROJECT_VERSION);
}

// char* mSupport::GetPowerDevice(char* dest, uint8_t idx, size_t size, uint8_t option)
// {
//   // char sidx[8];

//   // strncpy_P(dest, S_RSLT_POWER, size);                // POWER
//   // if ((devices_present + option) > 1) {
//   //   snprintf_P(sidx, sizeof(sidx), PSTR("%d"), idx);  // x
//   //   strncat(dest, sidx, size - strlen(dest) -1);      // POWERx
//   // }
//   // return dest;
// }

// char* mSupport::GetPowerDevice(char* dest, uint8_t idx, size_t size)
// {
//   // return GetPowerDevice(dest, idx, size, 0);
// }

float mSupport::ConvertTemp(float c)
{
  // float result = c;

  // if (!isnan(c) && Settings.flag_system.temperature_conversion) {
  //   result = c * 1.8 + 32;  // Fahrenheit
  // }
  // return result;
}

char mSupport::TempUnit(void)
{
  return (pCONT_set->Settings.flag_system.temperature_conversion) ? 'F' : 'C';
}

float mSupport::ConvertPressure(float p)
{
  // float result = p;

  // if (!isnan(p) && Settings.flag_system.pressure_conversion) {
  //   result = p * 0.75006375541921;  // mmHg
  // }
  // return result;
}

String mSupport::PressureUnit(void)
{
  // return (Settings.flag_system.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}

void mSupport::SetGlobalValues(float temperature, float humidity)
{
  // global_update = uptime;
  // global_temperature = temperature;
  // global_humidity = humidity;
}

void mSupport::ResetGlobalValues(void)
{
  // if ((uptime - global_update) > GLOBAL_VALUES_VALID) {  // Reset after 5 minutes
  //   global_update = 0;
  //   global_temperature = 0;
  //   global_humidity = 0;
  // }
}


uint32_t mSupport::SqrtInt(uint32_t num)
{
  // if (num <= 1) {
  //   return num;
  // }

  // uint32_t x = num / 2;
  // uint32_t y;
  // do {
  //   y = (x + num / x) / 2;
  //   if (y >= x) {
  //     return x;
  //   }
  //   x = y;
  // } while (true);
}

uint32_t mSupport::RoundSqrtInt(uint32_t num)
{
  // uint32_t s = SqrtInt(4 * num);
  // if (s & 1) {
  //   s++;
  // }
  // return s / 2;
}


// sprintf that returns the pointer to buffer instead of length for inline use
char* mSupport::p_snprintf(char* buffer, uint16_t buflen, const char* formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(buffer, buflen, formatP, arg);
  va_end(arg);
  return buffer;
}


// }

char* mSupport::GetTextIndexed_P(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);  //Makes sure byte is the right size
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}

char* mSupport::GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {

      // (addr) (*(const uint8_t *)(addr))

      ch = *read; //get vlaue from pointer
      read++; // move pointer forward

      // ch = pgm_read_byte(read++);  //pads

      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}


char* mSupport::GetTextIndexedTemp(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  uint16_t index_hay = 0;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {

      // (addr) (*(const uint8_t *)(addr))

      ch = read[index_hay++];  //pads
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}



int16_t mSupport::SearchForTextIndexedID(const char* name_tofind, const char* haystack, int8_t* class_id, int8_t* device_id)
{
  const char* read = haystack;
  int16_t position = -1;

  // Search for substring
  char *p_start_of_found = strstr(haystack,name_tofind);

  // returns 
  if(p_start_of_found == NULL){
      // AddLog(LOG_LEVEL_INFO,PSTR("p_start_of_found == NULL \n\r%s \n\r%s \n\r%s"),name_tofind,haystack);
    return -1;
    // position = abs(haystack - p_start_of_found); // get the position
  }
  // else{
  //   position = -1; //if null, it doesnt exist
  // }

  uint8_t delimeter_count = 0;
  uint16_t haystack_index = 0;
  uint16_t haystack_length = strlen(haystack);
  // char ch = '.';
  //search for delimeters between found index and start ie indexed list places
  while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
    // Count delimeters
    if(*read == '|'){
      delimeter_count++;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO,PSTR("%s\n\r found %s %d"),haystack,read,delimeter_count);
    #endif// ENABLE_LOG_LEVEL_INFO

      //use this and check class ID, then use it to return device_id


    }
    read++; //move pointer along
  }

//strcasecmp
// if(delimeter_count>0){

      // AddLog(LOG_LEVEL_INFO,PSTR("class %d  device %d"),Settings.device_name_buffer.class_id[delimeter_count],Settings.device_name_buffer.device_id[delimeter_count]);
// }

  return delimeter_count;
}



int mSupport::GetDListIDbyNameCtr_P(char* destination, size_t destination_size, const char* needle, const char* haystack)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}

int mSupport::GetDListIDbyNameCtr(char* destination, size_t destination_size, const char* needle, const char* haystack)
{

  Serial.println("GetDListIDbyNameCtr");Serial.flush();
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = *read;
      read++;
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}


/**
 * New method to allow command names to have multiple options, if seperated by "|"
 * A related function will also allow retrieving just the first option name, IF multiple exists
 * */
int8_t mSupport::GetCommandID( const char* needle, const char* haystack, char* destination, size_t destination_size)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  // const char* read = haystack;
  // char* write = destination;

  // while (true) {
  //   result++;
  //   size_t size = destination_size -1;
  //   write = destination;
  //   char ch = '.';
  //   while ((ch != '\0') && (ch != '|')) {
  //     // ch = pgm_read_byte(read++);
  //     ch = *read; // untested
  //     read++;
  //     if (size && (ch != '|'))  {
  //       *write++ = ch;
  //       size--;
  //     }
  //   }
  //   *write = '\0';
  //   if (!strcasecmp(needle, destination)) {
  //     break;
  //   }
  //   if (0 == ch) {
  //     result = -1;
  //     break;
  //   }
  // }
  return result;

}
/**
 * Returns true or false if the command exists
 * */
bool mSupport::CheckCommand_P(const char* needle, const char* haystack)
{
  return GetCommandID_P(needle, haystack)>=0;
}
/*

 * I also want to check if the entire thing matches, ie "one|two" should allow "one", "two" AND "one|two"
 */
int8_t mSupport::GetCommandID_P(const char* needle, const char* haystack, char* destination, size_t destination_size)
{
  if((needle == nullptr)||(haystack == nullptr)) return false;
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;
  //tmp fix, internal buffer to be removed
  if(destination == nullptr){
    // AddLog(LOG_LEVEL_WARN, PSTR("Should this be removed?? destination == nullptr"));
    char buffer_tmp[50];
    destination = buffer_tmp;
    destination_size = 50;
  }
  
  /**
   * @brief First check the whole message for a match (ie "A|B" will match when multiple commands are used in template)
   * check for exact WHOLE match
   * 
   * This may cause stability errors
   **/
  if(!strcasecmp_P(needle, haystack)) { //works v#.103.#.#
    // AddLog(LOG_LEVEL_INFO, PSTR("CHECKING WHOLE Should this be removed?? destination == nullptr"));
    // delay(5000);
    return 1;
  }

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;

}



int mSupport::GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}






int8_t mSupport::GetStateNumber(const char *state_text)
{
  char command[50];
  int8_t state_number = STATE_NUMBER_INVALID_ID;

  if (GetCommandCode(command, sizeof(command), state_text, kOptionOff) >= 0) {
    state_number = STATE_NUMBER_OFF_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionOn) >= 0) {
    state_number = STATE_NUMBER_ON_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionToggle) >= 0) {
    state_number = STATE_NUMBER_TOGGLE_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionBlink) >= 0) {
    state_number = STATE_NUMBER_BLINK_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionBlinkOff) >= 0) {
    state_number = STATE_NUMBER_BLINK_OFF_ID;
  } else
  if (GetCommandCode(command, sizeof(command), state_text, kOptionIncrement) >= 0) {
    state_number = STATE_NUMBER_INCREMENT_ID;
  } else
  if (GetCommandCode(command, sizeof(command), state_text, kOptionDecrement) >= 0) {
    state_number = STATE_NUMBER_DECREMENT_ID;
  } else  
  if (GetCommandCode(command, sizeof(command), state_text, kOptionFollow) >= 0) {
    state_number = STATE_NUMBER_FOLLOW_ID;
  } else  
  if (GetCommandCode(command, sizeof(command), state_text, kOptionFollowInv) >= 0) {
    state_number = STATE_NUMBER_FOLLOW_INV_ID;
  } else  
  { // c_str to number
    state_number = (!strlen(state_text)) ? 0 : atoi(state_text);
  }

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR("%d=GetStateNumber(%s)"),state_number, state_text);
  #endif// ENABLE_LOG_LEVEL_INFO

  return state_number;

}

/**
 * @brief Since multipe results exist, always return the first as defult
 * 

  // uint8_t default_index_as_first = 0;
 * @param state_text 
 * @return char* 
 */
char* mSupport::GetState_Name_by_ID(uint8_t id, char* buffer, uint8_t buflen) 
{
  switch(id)
  {
    default:
    case STATE_NUMBER_OFF_ID:         pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionOff); break;
    case STATE_NUMBER_ON_ID:          pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionOn); break;
    case STATE_NUMBER_TOGGLE_ID:      pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionToggle); break;
    case STATE_NUMBER_BLINK_ID:       pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionBlink); break;
    case STATE_NUMBER_BLINK_OFF_ID:   pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionBlinkOff); break;
    case STATE_NUMBER_INCREMENT_ID:   pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionIncrement); break;
    case STATE_NUMBER_DECREMENT_ID:   pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionDecrement); break;
    case STATE_NUMBER_FOLLOW_ID:      pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionFollow); break;
    case STATE_NUMBER_FOLLOW_INV_ID:  pCONT_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionFollowInv); break;
  }
  return buffer;
}














void mSupport::SetSerialBaudrate(int baudrate)
{
  // pCONT_set->Settings.baudrate = baudrate / 1200;
  // if (Serial.baudRate() != baudrate) {
  //   //if (seriallog_level) {
  //     AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SET_BAUDRATE_TO " %d"), baudrate);
  //   //}
  //   delay(100);
  //   Serial.flush();
  //   //Serial.begin(baudrate, serial_config);
  //   delay(10);
  //   Serial.println();
  // }
}

void mSupport::ClaimSerial(void)
{
  // serial_local = true;
  // AddLog(LOG_LEVEL_INFO, PSTR("SNS: Hardware Serial"));
  // SetSeriallog(LOG_LEVEL_NONE);
  // baudrate = Serial.baudRate();
  // Settings.baudrate = baudrate / 1200;
}

void mSupport::SerialSendRaw(char *codes)
{
  char *p;
  char stemp[3];
  uint8_t code;

  int size = strlen(codes);

  while (size > 0) {
    strlcpy(stemp, codes, sizeof(stemp));
    code = strtol(stemp, &p, 16);
    Serial.write(code);
    size -= 2;
    codes += 2;
  }
}

uint32_t mSupport::GetHash(const char *buffer, size_t size)
{
  uint32_t hash = 0;
  for (uint16_t i = 0; i <= size; i++) {
    hash += (uint8_t)*buffer++ * (i +1);
  }
  return hash;
}

void mSupport::ShowSource(int source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("SRC: %s"), GetTextIndexed_P(stemp1, sizeof(stemp1), source, kCommandSource));
    #endif// ENABLE_LOG_LEVEL_INFO
  }
}





char* mSupport::GetOtaUrl(char *otaurl, size_t otaurl_size)
{
  // if (strstr(pCONT_set->Settings.ota_url, "%04d") != nullptr) {     // OTA url contains placeholder for chip ID
  
  //         #ifdef ESP8266
  //   snprintf(otaurl, otaurl_size, pCONT_set->Settings.ota_url, ESP.getChipId() & 0x1fff);
  //   #endif
  // }
  // else if (strstr(pCONT_set->Settings.ota_url, "%d") != nullptr) {  // OTA url contains placeholder for chip ID
  
  //         #ifdef ESP8266
  //   snprintf_P(otaurl, otaurl_size, pCONT_set->Settings.ota_url, ESP.getChipId());
  //   #endif
  // }
  // else {
  //   strlcpy(otaurl, pCONT_set->Settings.ota_url, otaurl_size);
  // }
  return 0;//otaurl;
}



void mSupport::SleepDelay(uint32_t mseconds) {
  if (mseconds) {
    for (uint32_t wait = 0; wait < mseconds; wait++) {
      delay(1);
      if (Serial.available()) { break; }  // We need to service serial buffer ASAP as otherwise we get uart buffer overrun
    }
  } else {
    delay(0);
  }
}



// /********************************************************************************************

void mSupport::PerformEverySecond(void)
{
  // AddLog(LOG_LEVEL_TEST, PSTR("mSupport::PerformEverySecond"));
  // pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
  // pCONT_sup->activity.loop_counter=0;
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("LOOPSEC = %d"), pCONT_sup->activity.loop_counter);

  ResetGlobalValues();

  Handle_OTA_URLS();

  Handle_Check_Power_Saving();

  //If restart is ordered, of type reset, save network, erase settings, set defaults, reload network, complete restart
  CheckResetConditions();

  // Wifi keep alive to send Gratuitous ARP
  //wifiKeepAlive();


// #ifdef ESP32
//   if (11 == TasmotaGlobal.uptime) {  // Perform one-time ESP32 houskeeping
//     ESP_getSketchSize();             // Init sketchsize as it can take up to 2 seconds
//   }
// #endif
//     //if (!pCONT_set->global_state.wifi_down) { MqttCheck(); }

// /*
// if (!global_state.network_down) {
// #ifdef FIRMWARE_MINIMAL
//       if (1 == RtcSettings.ota_loader) {
//         RtcSettings.ota_loader = 0;
//         ota_state_flag = 3;
//       }
// #endif  // FIRMWARE_MINIMAL

// #ifdef USE_DISCOVERY
//       StartMdns();
// #endif  // USE_DISCOVERY

// #ifdef USE_MODULE_NETWORK_WEBSERVER
//       if (Settings.webserver) {

// #ifdef ESP8266
//         StartWebserver(Settings.webserver, WiFi.localIP());
// #else  // ESP32
// #ifdef USE_ETHERNET
//         StartWebserver(Settings.webserver, (EthernetLocalIP()) ? EthernetLocalIP() : WiFi.localIP());
// #else
//         StartWebserver(Settings.webserver, WiFi.localIP());
// #endif
// #endif

// #ifdef USE_DISCOVERY
// #ifdef USE_NETWORK_MDNS
//         MdnsAddServiceHttp();
// #endif  // USE_NETWORK_MDNS
// #endif  // USE_DISCOVERY
//       } else {
//         StopWebserver();
//       }
// #ifdef USE_EMULATION
//     if (Settings.flag2.emulation) { UdpConnect(); }
// #endif  // USE_EMULATION
// #endif  // USE_MODULE_NETWORK_WEBSERVER

// #ifdef USE_DEVICE_GROUPS
//       DeviceGroupsStart();
// #endif  // USE_DEVICE_GROUPS

// #ifdef USE_KNX
//       if (!knx_started && Settings.flag.knx_enabled) {  // CMND_KNX_ENABLED
//         KNXStart();
//         knx_started = true;
//       }
// #endif  // USE_KNX

//       MqttCheck();
//     } else {
// #ifdef USE_EMULATION
//       UdpDisconnect();
// #endif  // USE_EMULATION

// #ifdef USE_DEVICE_GROUPS
//       DeviceGroupsStop();
// #endif  // USE_DEVICE_GROUPS

// #ifdef USE_KNX
//       knx_started = false;
// #endif  // USE_KNX
//     }
// */

// DEBUG_LINE_HERE;



}


void mSupport::Handle_Check_Power_Saving()
{

    /*if (save_data_counter && (backlog_pointer == backlog_index)) {
      save_data_counter--;
      if (save_data_counter <= 0) { Serial.print("if (save_data_counter <= 0)="); Serial.println(save_data_counter);
        if (Settings.flag_system.save_state) {
          power_t mask = POWER_MASK;
          for (uint8_t i = 0; i < MAX_PULSETIMERS; i++) {
            if ((Settings.pulse_timer[i] > 0) && (Settings.pulse_timer[i] < 30)) {  // 3 seconds
              mask &= ~(1 << i);
            }
          }
          if (!((Settings.power &mask) == (power &mask))) {
            Settings.power = power;
          }
        } else {
          Settings.power = 0;
        }
        Serial.println("SettingsSave(0); from sonoff.ino");
        SettingsSave(0);
        save_data_counter = Settings.save_data; Serial.println("save_data_counter = Settings.save_data; 2");
      }
    }*/
  }
void mSupport::Handle_OTA_URLS()
{

    if (pCONT_set->ota_state_flag && (pCONT_set->backlog_pointer == pCONT_set->backlog_index)) {
      pCONT_set->ota_state_flag--;
      if (2 == pCONT_set->ota_state_flag) {
        // pCONT_set->ota_url = pCONT_set->Settings.ota_url;
        pCONT_set->RtcSettings.ota_loader = 0;  // Try requested image first
        // pCONT_set->ota_retry_counter = OTA_ATTEMPTS;
        
        #ifdef ESP8266
          ESPhttpUpdate.rebootOnUpdate(false);
        #endif
DEBUG_LINE_HERE;
        pCONT_set->SettingsSave(1);  // Free flash for OTA update
        
// DEBUG_LINE_HERE;
      }
      
// DEBUG_LINE_HERE;
      if (pCONT_set->ota_state_flag <= 0) {

// DEBUG_LINE_HERE;
#ifdef USE_MODULE_NETWORK_WEBSERVER
        // if (pCONT_set->Settings.webserver) //StopWebserver();
#endif  // USE_MODULE_NETWORK_WEBSERVER
// #ifdef USE_ARILUX_RF
//         AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
// #endif  // USE_ARILUX_RF
        pCONT_set->ota_state_flag = 92;
        pCONT_set->ota_result = 0;
        // pCONT_set->ota_retry_counter--;


        // if (pCONT_set->ota_retry_counter) {
          // strlcpy(pCONT_set->data_buffer.payload.ctr, 
//           // GetOtaUrl(pCONT_set->log_data, sizeof(pCONT_set->log_data)), sizeof(pCONT_set->data_buffer.payload.ctr));
// strlcpy(mqtt_data, GetOtaUrl(log_data, sizeof(log_data)), sizeof(mqtt_data));
// #ifndef FIRMWARE_MINIMAL
//           if (RtcSettings.ota_loader) {
//             // OTA File too large so try OTA minimal version
//             char *bch = strrchr(mqtt_data, '/');                       // Only consider filename after last backslash prevent change of urls having "-" in it
//             if (bch == nullptr) { bch = mqtt_data; }                   // No path found so use filename only
// /*
//             char *ech = strrchr(bch, '.');                             // Find file type in filename (none, .bin or .gz)
//             if ((ech != nullptr) && (0 == strncasecmp_P(ech, PSTR(".GZ"), 3))) {
//               char *fch = ech;
//               *fch = '\0';
//               ech = strrchr(bch, '.');                                 // Find file type .bin.gz
//               *fch = '.';
//             }
// */
//             char *ech = strchr(bch, '.');                              // Find file type in filename (none, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz)
//             if (ech == nullptr) { ech = mqtt_data + strlen(mqtt_data); }  // Point to '/0' at end of mqtt_data becoming an empty string

// //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("OTA: File type [%s]"), ech);

//             char ota_url_type[strlen(ech) +1];
//             strncpy(ota_url_type, ech, sizeof(ota_url_type));          // Either empty, .ino.bin, .ino.bin.gz, .bin, .bin.gz or .gz

//             char *pch = strrchr(bch, '-');                             // Find last dash (-) and ignore remainder - handles tasmota-DE
//             if (pch == nullptr) { pch = ech; }                         // No dash so ignore filetype
//             *pch = '\0';                                               // mqtt_data = http://domus1:80/api/arduino/tasmota
//             snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s-" D_JSON_MINIMAL "%s"), mqtt_data, ota_url_type);  // Minimal filename must be filename-minimal
//           }
// #endif  // FIRMWARE_MINIMAL


//           // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "%s"), pCONT_set->data_buffer.payload.ctr);
//           #ifdef ESP8266
// #if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
//           ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(data_buffer.payload.ctr));
// #else
//           // If using core stage or 2.5.0+ the syntax has changed
//           WiFiClient OTAclient;
//           pCONT_set->ota_result = (HTTP_UPDATE_FAILED != ESPhttpUpdate.update(OTAclient, pCONT_set->data_buffer.payload.ctr));
// #endif
// #endif
//           if (!pCONT_set->ota_result) {
// #ifndef FIRMWARE_MINIMAL

//           #ifdef ESP8266
//             int ota_error = ESPhttpUpdate.getLastError();
//             #endif
// //            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "Ota error %d"), ota_error);
//             // if ((HTTP_UE_TOO_LESS_SPACE == ota_error) || (HTTP_UE_BIN_FOR_WRONG_FLASH == ota_error)) {
//             //   pCONT_set->RtcSettings.ota_loader = 1;  // Try minimal image next
//             // }
// #endif  // FIRMWARE_MINIMAL
//             pCONT_set->ota_state_flag = 2;    // Upgrade failed - retry
//           }
//         }
      }
      if (90 == pCONT_set->ota_state_flag) {  // Allow MQTT to reconnect
        pCONT_set->ota_state_flag = 0;
        if (pCONT_set->ota_result) {
         pCONT_set->SetFlashModeDout();      // Force DOUT for both ESP8266 and ESP8285
          // Response_mP(PSTR(D_JSON_SUCCESSFUL ". " D_JSON_RESTARTING));
        } else {
          
          // #ifdef ESP8266
          // Response_mP(PSTR(D_JSON_FAILED " %s"), ESPhttpUpdate.getLastErrorString().c_str());
          // #endif
        }
        pCONT_set->restart_flag = 2;          // Restart anyway to keep memory clean webserver
        //MqttPublishPrefixTopic_P(STAT, PSTR(D_JSON_UPGRADE));
      }
    }
}

void mSupport::CheckResetConditions()
{
    if (pCONT_set->restart_flag && (pCONT_set->backlog_pointer == pCONT_set->backlog_index)) {
      if ((214 == pCONT_set->restart_flag) || (215 == pCONT_set->restart_flag) || (216 == pCONT_set->restart_flag)) {
// Backup current SSIDs and Passwords



                                      // Loading in new network configs only if they are considered save and correct

        // char storage_wifi[sizeof(pCONT_set->Settings.sta_ssid) +
        //                   sizeof(pCONT_set->Settings.sta_pwd)];
        // char storage_mqtt[sizeof(pCONT_set->Settings.mqtt_host) +
        //                   sizeof(pCONT_set->Settings.mqtt_port) +
        //                   sizeof(pCONT_set->Settings.mqtt_client) +
        //                   sizeof(pCONT_set->Settings.mqtt_user) +
        //                   sizeof(pCONT_set->Settings.mqtt_pwd) +
        //                   sizeof(pCONT_set->Settings.mqtt_topic)];
        // memcpy(storage_wifi, pCONT_set->Settings.sta_ssid, sizeof(storage_wifi));     // Backup current SSIDs and Passwords

        // // Backup current SSIDs and Passwords
        // char storage_ssid1[strlen(SettingsText(SET_STASSID1)) +1];
        // strncpy(storage_ssid1, SettingsText(SET_STASSID1), sizeof(storage_ssid1));
        // char storage_ssid2[strlen(SettingsText(SET_STASSID2)) +1];
        // strncpy(storage_ssid2, SettingsText(SET_STASSID2), sizeof(storage_ssid2));
        // char storage_pass1[strlen(SettingsText(SET_STAPWD1)) +1];
        // strncpy(storage_pass1, SettingsText(SET_STAPWD1), sizeof(storage_pass1));
        // char storage_pass2[strlen(SettingsText(SET_STAPWD2)) +1];
        // strncpy(storage_pass2, SettingsText(SET_STAPWD2), sizeof(storage_pass2));

        // char storage_mqtthost[strlen(SettingsText(SET_MQTT_HOST)) +1];
        // strncpy(storage_mqtthost, SettingsText(SET_MQTT_HOST), sizeof(storage_mqtthost));
        // char storage_mqttuser[strlen(SettingsText(SET_MQTT_USER)) +1];
        // strncpy(storage_mqttuser, SettingsText(SET_MQTT_USER), sizeof(storage_mqttuser));
        // char storage_mqttpwd[strlen(SettingsText(SET_MQTT_PWD)) +1];
        // strncpy(storage_mqttpwd, SettingsText(SET_MQTT_PWD), sizeof(storage_mqttpwd));
        // char storage_mqtttopic[strlen(SettingsText(SET_MQTT_TOPIC)) +1];
        // strncpy(storage_mqtttopic, SettingsText(SET_MQTT_TOPIC), sizeof(storage_mqtttopic));
        // uint16_t mqtt_port = Settings.mqtt_port;

        // if (216 == pCONT_set->restart_flag) {
        //   memcpy(storage_mqtt, pCONT_set->Settings.mqtt_host, sizeof(storage_mqtt));  // Backup mqtt host, port, client, username and password
        // }
        if ((215 == pCONT_set->restart_flag) || (216 == pCONT_set->restart_flag)) {
          pCONT_set->SettingsErase(0);  // Erase all flash from program end to end of physical flash
        }
        pCONT_set->SettingsDefault();
        // memcpy(pCONT_set->Settings.sta_ssid, storage_wifi, sizeof(storage_wifi));     // Restore current SSIDs and Passwords
        // if (216 == pCONT_set->restart_flag) {
        //   memcpy(pCONT_set->Settings.mqtt_host, storage_mqtt, sizeof(storage_mqtt));  // Restore the mqtt host, port, client, username and password
        //   strlcpy(pCONT_set->Settings.mqtt_client, MQTT_CLIENT_ID, sizeof(pCONT_set->Settings.mqtt_client));  // Set client to default
        // }
        pCONT_set->restart_flag = 2;
      }



      else if (213 == pCONT_set->restart_flag) {
        pCONT_set->SettingsSdkErase();  // Erase flash SDK parameters
        pCONT_set->restart_flag = 2;
      }
      else if (212 == pCONT_set->restart_flag) {
        pCONT_set->SettingsErase(0);    // Erase all flash from program end to end of physical flash
        pCONT_set->restart_flag = 211;
      }
      if (211 == pCONT_set->restart_flag) {
        pCONT_set->SettingsDefault();
        pCONT_set->restart_flag = 2;
      }
      // pCONT_set->SettingsSaveAll();
      pCONT_set->restart_flag--;
      if (pCONT_set->restart_flag <= 0) 
      {

        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION "pCONT_set->restart_flag <= 0 " D_RESTARTING));
        #endif// ENABLE_LOG_LEVEL_INFO
    
        #ifdef USE_MODULE_NETWORK_WIFI   
        // THIS should be moved into this class
        pCONT_wif->EspRestart();        
        #endif USE_MODULE_NETWORK_WIFI

      }
    }


    }



// Force a float value between two ranges, and adds or substract the range until we fit
float mSupport::ModulusRangef(float f, float a, float b) {
  if (b <= a) { return a; }       // inconsistent, do what we can
  float range = b - a;
  float x = f - a;                // now range of x should be 0..range
  x = fmodf(x, range);            // actual range is now -range..range
  if (x < 0.0f) { x += range; }   // actual range is now 0..range
  return x + a;                   // returns range a..b
}


// #ifdef ESP8266
// void SerialInput(void)
// {
//   while (Serial.available()) {
// //    yield();
//     delay(0);
// //     serial_in_byte = Serial.read();

// // /*-------------------------------------------------------------------------------------------*\
// //  * Sonoff dual and ch4 19200 baud serial interface
// // \*-------------------------------------------------------------------------------------------*/
// //     if ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
// //      // serial_in_byte = ButtonSerial(serial_in_byte);
// //     }

// // /*-------------------------------------------------------------------------------------------*/

// //     // if (XdrvCall(FUNC_SERIAL)) {
// //     //   serial_in_byte_counter = 0;
// //     //   Serial.flush();
// //     //   return;
// //     // }

// // /*-------------------------------------------------------------------------------------------*/

// //     if (serial_in_byte > 127 && !Settings.flag_system.mqtt_serial_raw) {                // Discard binary data above 127 if no raw reception allowed
// //       serial_in_byte_counter = 0;
// //       Serial.flush();
// //       return;
// //     }
// //     if (!Settings.flag_system.mqtt_serial) {                                            // SerialSend active
// //       if (isprint(serial_in_byte)) {                                             // Any char between 32 and 127
// //         if (serial_in_byte_counter < INPUT_BUFFER_SIZE -1) {                     // Add char to string if it still fits
// //           serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
// //         } else {
// //           serial_in_byte_counter = 0;
// //         }
// //       }
// //     } else {
// //       if (serial_in_byte || Settings.flag_system.mqtt_serial_raw) {                     // Any char between 1 and 127 or any char (0 - 255)
// //         if ((serial_in_byte_counter < INPUT_BUFFER_SIZE -1) &&                   // Add char to string if it still fits and ...
// //             ((isprint(serial_in_byte) && (128 == Settings.serial_delimiter)) ||  // Any char between 32 and 127
// //             ((serial_in_byte != Settings.serial_delimiter) && (128 != Settings.serial_delimiter)) ||  // Any char between 1 and 127 and not being delimiter
// //               Settings.flag_system.mqtt_serial_raw)) {                                  // Any char between 0 and 255
// //           serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
// //           serial_polling_window = millis();
// //         } else {
// //           serial_polling_window = 0;                                             // Reception done - send mqtt
// //           break;
// //         }
// //       }
// //     }

// // /*-------------------------------------------------------------------------------------------*\
// //  * Sonoff SC 19200 baud serial interface
// // \*-------------------------------------------------------------------------------------------*/
// //     if (SONOFF_SC == my_module_type) {
// //       if (serial_in_byte == '\x1B') {                                            // Sonoff SC status from ATMEGA328P
// //         serial_in_buffer[serial_in_byte_counter] = 0;                            // Serial data completed
// //         //SonoffScSerialInput(serial_in_buffer);
// //         serial_in_byte_counter = 0;
// //         Serial.flush();
// //         return;
// //       }
// //     }

// /*-------------------------------------------------------------------------------------------*

//     // else if (!Settings.flag_system.mqtt_serial && (serial_in_byte == '\n')) {
//     //   serial_in_buffer[serial_in_byte_counter] = 0;                              // Serial data completed
//     //   seriallog_level = (Settings.seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings.seriallog_level;
//     //   AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), serial_in_buffer);
//     //   ExecuteCommand(serial_in_buffer, SRC_SERIAL);
//     //   serial_in_byte_counter = 0;
//     //   serial_polling_window = 0;
//     //   Serial.flush();
//     //   return;
//     // }
//   }

// //   if (Settings.flag_system.mqtt_serial && serial_in_byte_counter && (millis() > (serial_polling_window + SERIAL_POLLING))) {
// //     serial_in_buffer[serial_in_byte_counter] = 0;                                // Serial data completed
// //     if (!Settings.flag_system.mqtt_serial_raw) {
// //       Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":\"%s\"}"), serial_in_buffer);
// //     } else {
// //       Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":\""));
// //       for (int i = 0; i < serial_in_byte_counter; i++) {
// //         ResponseAppend_P(PSTR("%02x"), serial_in_buffer[i]);
// //       }
// //       ResponseAppend_P(PSTR("\"}"));
// //     }
// //     MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_SERIALRECEIVED));
// // //      XdrvRulesProcess();
// //     serial_in_byte_counter = 0;
// //   }
// }


#ifdef ESP8266
extern "C" {
extern struct rst_info resetInfo;
}
#endif // ESP8266


double mSupport::FastPrecisePow(double a, double b)
{
  // https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
  // calculate approximation with fraction of the exponent
  int e = abs((int)b);
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }
  return r * u.d;
}

float mSupport::FastPrecisePowf(const float x, const float y)
{
//  return (float)(pow((double)x, (double)y));
  return (float)FastPrecisePow(x, y);
}


void mSupport::parse_JSONCommand(JsonParserObject obj){

  // StaticJsonDocument<300> doc;
  // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  // JsonObject obj = doc.as<JsonObject>();

  // if(obj.containsKey("resetcounter")){
  //   uint8_t val = obj["resetcounter"];
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"resetcounter\":[%d]"),val);
  //   pCONT_time->ResetRebootCounter();
  //   data_buffer.isserviced++;
  // }else
  // if(obj.containsKey("loglevel")){
  //   const char* name = obj["loglevel"];
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"loglevel\":\"%s\""),name);
  //   pCONT_set->Settings.seriallog_level = pCONT->mso->SetLogLevelIDbyName(name);
  //   // Add save log here
  //   data_buffer.isserviced++;
  // }
  // else{
  //    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT D_PARSING_NOMATCH));
  // }
  

  //USeful tasmota stuff
  // else if (CMND_STATUS == command_code) {
  //     if ((payload < 0) || (payload > MAX_STATUS)) payload = 99;
  //     PublishStatus(payload);
  //     fallback_topic_flag = false;
  //     return;
  //   }
  //   else if (CMND_STATE == command_code) {
  //     data_buffer.payload.ctr[0] = '\0';
  //     MqttShowState();
  //     if (Settings.flag_network.hass_tele_on_power) {
  //       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
  //     }
  //   }
  //   else if (CMND_SLEEP == command_code) {
  //     if ((payload >= 0) && (payload < 251)) {
  //       Settings.sleep = payload;
  //       sleep = payload;
  //       WiFiSetSleepMode();
  //     }
  //     snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT, command, sleep, (Settings.flag_system.value_units) ? " " D_UNIT_MILLISECOND : "", Settings.sleep, (Settings.flag_system.value_units) ? " " D_UNIT_MILLISECOND : "");
  //   }

  // else if (CMND_RESTART == command_code) {
  //     switch (payload) {
  //     case 1:
  //       restart_flag = 2;
  //       snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_SVALUE, command, D_JSON_RESTARTING);
  //       break;
  //     case 99:
  //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
  //       EspRestart();
  //       break;
  //     default:
  //       snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_SVALUE, command, D_JSON_ONE_TO_RESTART);
  //     }
  //   }

  //  else if ((CMND_POWERONSTATE == command_code) && (Settings.module != MOTOR)) {
  //     /* 0 = Keep relays off after power on
  //      * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
  //      * 2 = Toggle relays after power on
  //      * 3 = Set relays to last saved state after power on
  //      * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
  //      * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
  //      */
  //     if ((payload >= POWER_ALL_OFF) && (payload <= POWER_ALL_OFF_PULSETIME_ON)) {
  //       Settings.poweronstate = payload;
  //       if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
  //         for (uint8_t i = 1; i <= devices_present; i++) {
  //           ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
  //         }
  //       }
  //     }
  //     snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_NVALUE, command, Settings.poweronstate);
  //   }
  
  // else if (CMND_SYSLOG == command_code) {
  //     if ((payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
  //       Settings.syslog_level = payload;
  //       syslog_level = payload;
  //       syslog_timer = 0;
  //     }
  //     snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, command, Settings.syslog_level, syslog_level);
  //   }
  //   else if (CMND_LOGHOST == command_code) {
  //     if ((data_len > 0) && (data_len < sizeof(Settings.syslog_host))) {
  //       strlcpy(Settings.syslog_host, (SC_DEFAULT == Shortcut(dataBuf)) ? SYS_LOG_HOST : dataBuf, sizeof(Settings.syslog_host));
  //     }
  //     snprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), S_JSON_COMMAND_SVALUE, command, Settings.syslog_host);
  //   }


// void mSupport::parse_SyncRefresh(){
//   //DISABLED UNTIL NEW SYNC METHOD/FLAG SYSTEM IS ESTABLISHED
//       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT D_ERROR_UNSUPPORTED));
//   // if((mSupport::memsearch(data_buffer.payload.ctr, data_buffer.payload.len,"refresh",sizeof("refresh")-1))>=0){ pCONT->mso->MessagePrint("MATCHED refresh");
//   //   //fSendAllData = true;
//   // }
// } // END function




  //else if(root["reset_wifi_counter"]){ pCONT->mso->MessagePrintln("root[reset_wifi_counter]");
  //   // wifi_reconnects_counter = 0;
  //   data_buffer.payload.isserviced++;
  // }else if(root["reboot"].as<const char*>()){ pCONT->mso->MessagePrintln("root[reboot]");
  // //make non json
  // data_buffer.payload.isserviced++;
  // }else if(root["command"].as<const char*>()){ pCONT->mso->MessagePrintln("root[command]");
  //   if(strstr(root["command"],"resetcounter")){
  //     pCONT_time->ResetRebootCounter();
  //   }else if(strstr(root["command"],"reset_wificounter")){
  //     //pCONT_time->ResetRebootCounter();
  //   }
  //   data_buffer.payload.isserviced++;
  // }else if(root["rss_scanner"].as<const char*>()){ pCONT->mso->MessagePrintln("root[command]");
  //   //RssScannerRate = root["rss_scanner"];
  //   data_buffer.payload.isserviced++;
  // }
  
  // else{
  //   pCONT->mso->MessagePrintln("NO MATCH");
  // }

  

  // if(root["resetcounter"]){ pCONT->mso->MessagePrintln("root[resetcounter]");
  //   pCONT_time->ResetRebootCounter();
  //   data_buffer.payload.isserviced++;
  // }else if(root["healthsecnormal"]){ pCONT->mso->MessagePrintln("root[healthsecnormal]");
  //   healthsecnormal = root["healthsecnormal"];
  //   data_buffer.payload.isserviced++;
  // }else if(root["reset_wifi_counter"]){ pCONT->mso->MessagePrintln("root[reset_wifi_counter]");
  //   // wifi_reconnects_counter = 0;
  //   data_buffer.payload.isserviced++;
  // }else if(root["reboot"].as<const char*>()){ pCONT->mso->MessagePrintln("root[reboot]");
  // //make non json
  // data_buffer.payload.isserviced++;
  // }else if(root["command"].as<const char*>()){ pCONT->mso->MessagePrintln("root[command]");
  //   if(strstr(root["command"],"resetcounter")){
  //     pCONT_time->ResetRebootCounter();
  //   }else if(strstr(root["command"],"reset_wificounter")){
  //     //pCONT_time->ResetRebootCounter();
  //   }
  //   data_buffer.payload.isserviced++;
  // }else if(root["rss_scanner"].as<const char*>()){ pCONT->mso->MessagePrintln("root[command]");
  //   //RssScannerRate = root["rss_scanner"];

  //   data_buffer.payload.isserviced++;
  // }else{
  //   pCONT->mso->MessagePrintln("NO MATCH");
  // }

  //fHardwareInfoSent = false; // resend since its updated

}



// phase out
// likely used so math.h was not needed for memory
uint16_t mSupport::changeUIntScale(uint16_t inum, 
                                   uint16_t ifrom_min, uint16_t ifrom_max,
                                   uint16_t ito_min, uint16_t ito_max
                                   ) {

  // guard-rails
  if (ifrom_min >= ifrom_max) {
    if (ito_min > ito_max) {
      return ito_max;
    } else {
      return ito_min;  // invalid input, return arbitrary value
    }
  }
  // convert to uint31, it's more verbose but code is more compact
  uint32_t num = inum;
  uint32_t from_min = ifrom_min;
  uint32_t from_max = ifrom_max;
  uint32_t to_min = ito_min;
  uint32_t to_max = ito_max;

  // check source range
  num = (num > from_max ? from_max : (num < from_min ? from_min : num));

  // check to_* order
  if (to_min > to_max) {
    // reverse order
    num = (from_max - num) + from_min;
    to_min = ito_max;
    to_max = ito_min;
  }

  uint32_t numerator = (num - from_min) * (to_max - to_min);
  uint32_t result;
  if (numerator >= 0x80000000L) {
    // don't do rounding as it would create an overflow
    result = numerator / (from_max - from_min) + to_min;
  } else {
    result = (((numerator * 2) / (from_max - from_min)) + 1) / 2 + to_min;
  }
  return (uint32_t) (result > to_max ? to_max : (result < to_min ? to_min : result));
}






// /*******************************************************************************************/

// #ifdef DEBUG_THEO
// void ExceptionTest(uint8_t type)
// {
// /*
// Exception (28):
// epc1=0x4000bf64 epc2=0x00000000 epc3=0x00000000 excvaddr=0x00000007 depc=0x00000000

// ctx: cont
// sp: 3fff1f30 end: 3fff2840 offset: 01a0

// >>>stack>>>
// 3fff20d0:  202c3573 756f7247 2c302070 646e4920
// 3fff20e0:  40236a6e 7954202c 45206570 00454358
// 3fff20f0:  00000010 00000007 00000000 3fff2180
// 3fff2100:  3fff2190 40107bfc 3fff3e4c 3fff22c0
// 3fff2110:  40261934 000000f0 3fff22c0 401004d8
// 3fff2120:  40238fcf 00000050 3fff2100 4021fc10
// 3fff2130:  3fff32bc 4021680c 3ffeade1 4021ff7d
// 3fff2140:  3fff2190 3fff2180 0000000c 7fffffff
// 3fff2150:  00000019 00000000 00000000 3fff21c0
// 3fff2160:  3fff23f3 3ffe8e08 00000000 4021ffb4
// 3fff2170:  3fff2190 3fff2180 0000000c 40201118
// 3fff2180:  3fff21c0 0000003c 3ffef840 00000007
// 3fff2190:  00000000 00000000 00000000 40201128
// 3fff21a0:  3fff23f3 000000f1 3fff23ec 4020fafb
// 3fff21b0:  3fff23f3 3fff21c0 3fff21d0 3fff23f6
// 3fff21c0:  00000000 3fff23fb 4022321b 00000000

// Exception 28: LoadProhibited: A load referenced a page mapped with an attribute that does not permit loads
// Decoding 14 results
// 0x40236a6e: ets_vsnprintf at ?? line ?
// 0x40107bfc: vsnprintf at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/libc_replacements.c line 387
// 0x40261934: bignum_exptmod at ?? line ?
// 0x401004d8: malloc at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266\umm_malloc/umm_malloc.c line 1664
// 0x40238fcf: wifi_station_get_connect_status at ?? line ?
// 0x4021fc10: operator new[](unsigned int) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/abi.cpp line 57
// 0x4021680c: ESP8266WiFiSTAClass::status() at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi\src/ESP8266WiFiSTA.cpp line 569
// 0x4021ff7d: vsnprintf_P(char*, unsigned int, char const*, __va_list_tag) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
// 0x4021ffb4: snprintf_P(char*, unsigned int, char const*, ...) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
// 0x40201118: atol at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
// 0x40201128: atoi at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
// 0x4020fafb: MqttDataHandler(char*, unsigned char*, unsigned int) at R:\Arduino\Work-ESP8266\Theo\sonoff\sonoff-4\sonoff/sonoff.ino line 679 (discriminator 1)
// 0x4022321b: pp_attach at ?? line ?

// 00:00:08 MQTT: tele/sonoff/INFO3 = {"Started":"Fatal exception:28 flag:2 (EXCEPTION) epc1:0x4000bf64 epc2:0x00000000 epc3:0x00000000 excvaddr:0x00000007 depc:0x00000000"}
// */
//   if (1 == type) {
//     char svalue[10];
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s"), 7);  // Exception 28 as number in string (7 in excvaddr)
//   }
// /*
// 14:50:52 osWatch: FreeRam 25896, rssi 68, last_run 0
// 14:51:02 osWatch: FreeRam 25896, rssi 58, last_run 0
// 14:51:03 CMND: exception 2
// 14:51:12 osWatch: FreeRam 25360, rssi 60, last_run 8771
// 14:51:22 osWatch: FreeRam 25360, rssi 62, last_run 18771
// 14:51:32 osWatch: FreeRam 25360, rssi 62, last_run 28771
// 14:51:42 osWatch: FreeRam 25360, rssi 62, last_run 38771
// 14:51:42 osWatch: Warning, loop blocked. Restart now
// */
//   if (2 == type) {
//     while(1) delay(1000);  // this will trigger the os watch
//   }
// }

// #endif  // DEBUG_THEO

// /*******************************************************************************************/

// void CpuLoadLoop(void)
// {
//   CPU_last_loop_time = millis();
//   if (CPU_load_check && CPU_last_millis) {
//     CPU_loops ++;
//     if ((CPU_last_millis + (CPU_load_check *1000)) <= CPU_last_loop_time) {
// #if defined(F_CPU) && (F_CPU == 160000000L)
//       int CPU_load = 100 - ( (CPU_loops*(1 + 30*sleep)) / (CPU_load_check *800) );
//       CPU_loops = CPU_loops / CPU_load_check;
//       AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(160MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
// #else
//       int CPU_load = 100 - ( (CPU_loops*(1 + 30*sleep)) / (CPU_load_check *400) );
//       CPU_loops = CPU_loops / CPU_load_check;
//       AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(80MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
// #endif
//       CPU_last_millis = CPU_last_loop_time;
//       CPU_loops = 0;
//     }
//   }
// }

// /*******************************************************************************************/

// #if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1)
// // All version before core 2.4.2
// // https://github.com/esp8266/Arduino/issues/2557

// extern "C" {
// #include <cont.h>
//   extern cont_t g_cont;
// }

// void DebugFreeMem(void)
// {
//   register uint32_t *sp asm("a1");

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d, UnmodifiedStack %d"), ESP.getFreeHeap(), 4 * (sp - g_cont.stack), cont_get_free_stack(&g_cont));//, XdrvMailbox.data);
//   // AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_cont.stack), XdrvMailbox.data);
// }

// #else
// // All version from core 2.4.2
// // https://github.com/esp8266/Arduino/pull/5018
// // https://github.com/esp8266/Arduino/pull/4553

#ifdef ESP8266
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}

void mSupport::DebugFreeMem(void)
{
  //https://www.esp8266.com/viewtopic.php?p=69937
  //https://i.stack.imgur.com/waoHN.gif
  register uint32_t *sp asm("a1");

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d"), 
      ESP.getFreeHeap(), 4 * (sp - g_pcont->stack));
    #endif// ENABLE_LOG_LEVEL_INFO
}

// #endif  // ARDUINO_ESP8266_RELEASE_2_x_x

#endif // ESP8266


// void mSupport::WDT_Begin(){}
//   // Configure WDT
//   #ifdef ESP32
//     timerwdt = timerBegin(0, 80, true);                  //timer 0, div 80
//     timerAttachInterrupt(timerwdt, &resetModule, true);  //attach callback
//     timerAlarmWrite(timerwdt, wdtTimeout * 1000, false); //set time in us
//     timerAlarmEnable(timerwdt);                          //enable interrupt
//     WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
//   #endif
//   #ifdef ESP32 // Get disabling
//     timerAlarmDisable(timerwdt);                          //enable interrupt
//     mwif.WifiConnectForced();
//     timerWrite(timerwdt, 0); //reset timer (feed watchdog)
//     timerAlarmEnable(timerwdt);                          //enable interrupt
//   #endif
// }

// WDT_Reset()
//   #ifdef ESP32
//     timerWrite(timerwdt, 0);  //reset timer (feed watchdog)   ///move into settings with different functions for eahc hardware
//   #endif // ESP32
//   #ifdef ESP8266
//     ESP.wdtFeed();            //reset timer (feed watchdog)
//   #endif // ESP8266

// /*******************************************************************************************/

// void DebugRtcDump(char* parms)
// {
//   #define CFG_COLS 16

//   uint16_t idx;
//   uint16_t maxrow;
//   uint16_t row;
//   uint16_t col;
//   char *p;

//   // |<--SDK data (256 bytes)-->|<--User data (512 bytes)-->|
//   // 000 - 0FF: SDK
//   //  000 - 01B: SDK rst_info
//   // 100 - 2FF: User
//   //  280 - 283: Tasmota RtcReboot   (Offset 100 (x 4bytes) - sizeof(RTCRBT) (x 4bytes))
//   //  290 - 2EB: Tasmota RtcSettings (Offset 100 (x 4bytes))

//   uint8_t buffer[768];
// //  ESP.rtcUserMemoryRead(0, (uint32_t*)&buffer, sizeof(buffer));
//   system_rtc_mem_read(0, (uint32_t*)&buffer, sizeof(buffer));

//   maxrow = ((sizeof(buffer)+CFG_COLS)/CFG_COLS);

//   uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
//   uint16_t mrow = strtol(p, &p, 10);

// //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

//   if (0 == mrow) {  // Default only 8 lines
//     mrow = 8;
//   }
//   if (srow > maxrow) {
//     srow = maxrow - mrow;
//   }
//   if (mrow < (maxrow - srow)) {
//     maxrow = srow + mrow;
//   }

//   for (row = srow; row < maxrow; row++) {
//     idx = row * CFG_COLS;
//     snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), idx);
//     for (col = 0; col < CFG_COLS; col++) {
//       if (!(col%4)) {
//         snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//       }
//       snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
//     }
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
//     for (col = 0; col < CFG_COLS; col++) {
// //      if (!(col%4)) {
// //        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
// //      }
//       snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
//     }
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
//     AddLogAddLog(LOG_LEVEL_INFO);
//   }
// }

// /*******************************************************************************************/

// void DebugCfgDump(char* parms)
// {
//   #define CFG_COLS 16

//   uint16_t idx;
//   uint16_t maxrow;
//   uint16_t row;
//   uint16_t col;
//   char *p;

//   uint8_t *buffer = (uint8_t *) &Settings;
//   maxrow = ((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS);

//   uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
//   uint16_t mrow = strtol(p, &p, 10);

// //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

//   if (0 == mrow) {  // Default only 8 lines
//     mrow = 8;
//   }
//   if (srow > maxrow) {
//     srow = maxrow - mrow;
//   }
//   if (mrow < (maxrow - srow)) {
//     maxrow = srow + mrow;
//   }

//   for (row = srow; row < maxrow; row++) {
//     idx = row * CFG_COLS;
//     snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), idx);
//     for (col = 0; col < CFG_COLS; col++) {
//       if (!(col%4)) {
//         snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//       }
//       snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
//     }
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
//     for (col = 0; col < CFG_COLS; col++) {
// //      if (!(col%4)) {
// //        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
// //      }
//       snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
//     }
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
//     AddLogAddLog(LOG_LEVEL_INFO);
//     delay(1);
//   }
// }

// void DebugCfgPeek(char* parms)
// {
//   char *p;

//   uint16_t address = strtol(parms, &p, 16);
//   if (address > sizeof(SYSCFG)) address = sizeof(SYSCFG) -4;
//   address = (address >> 2) << 2;

//   uint8_t *buffer = (uint8_t *) &Settings;
//   uint8_t data8 = buffer[address];
//   uint16_t data16 = (buffer[address +1] << 8) + buffer[address];
//   uint32_t data32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + data16;

//   snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), address);
//   for (uint8_t i = 0; i < 4; i++) {
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[address +i]);
//   }
//   snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
//   for (uint8_t i = 0; i < 4; i++) {
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[address +i] > 0x20) && (buffer[address +i] < 0x7F)) ? (char)buffer[address +i] : ' ');
//   }
//   snprintf_P(log_data, sizeof(log_data), PSTR("%s| 0x%02X (%d), 0x%04X (%d), 0x%0LX (%lu)"), log_data, data8, data8, data16, data16, data32, data32);
//   AddLogAddLog(LOG_LEVEL_INFO);
// }


//#ifdef ENABLE_CRASH_RECORDING

/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end )
{
  uint32_t addr_written = 0;      // how many addresses have we already written in RTC
  uint32_t value;                 // 4 bytes buffer to write to RTC

    #ifdef ESP8266
  for (uint32_t i = stack; i < stack_end; i += 4) {
    value = *((uint32_t*) i);     // load value from stack
    if ((value >= 0x40000000) && (value < 0x40300000)) {  // keep only addresses in code area
      ESP.rtcUserMemoryWrite(crash_rtc_offset + addr_written, (uint32_t*)&value, sizeof(value));
      addr_written++;
      if (addr_written >= crash_dump_max_len) { break; }  // we store only 31 addresses
    }
  }
  value = crash_magic + addr_written;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}

// Generate a crash to test the crash recorder
void mSupport::CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
}

// Do an infinite loop to trigger WDT watchdog
void mSupport::CmndWDT(void)
{
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
}

// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
void mSupport::CmndBlockedLoop(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  while (1) {
    delay(1000);
  }
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void mSupport::CrashDumpClear(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  return;
    #ifdef ESP8266
  uint32_t value = 0;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}



/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/

bool mSupport::CrashFlag(void)
{
  
#ifdef ESP8266
  DEBUG_PRINT_FUNCTION_NAME;
  return ((ResetReason() == REASON_EXCEPTION_RST) || (ResetReason() == REASON_SOFT_WDT_RST));// || oswatch_blocked_loop);
  
#endif // ESP8266

}

void mSupport::CrashDump_AddJson(void)
{
  
#ifdef ESP8266
  char buffer[30];

  DEBUG_PRINT_FUNCTION_NAME;
  pCONT_sup->WriteBuffer_P(PSTR("\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":[\"%08x\",\"%08x\",\"%08x\"],\"EXCVADDR\":\"%08x\",\"DEPC\":\"%08x\""),
    resetInfo.exccause,        // Exception Cause
    pCONT_sup->GetResetReason(buffer, sizeof(buffer)),  // Reset Reason
    resetInfo.epc1,            // Exception Progam Counter
    resetInfo.epc2,            // Exception Progam Counter - High-Priority Interrupt 1
    resetInfo.epc3,            // Exception Progam Counter - High-Priority Interrupt 2
    resetInfo.excvaddr,        // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
    resetInfo.depc             // Double Exception Program Counter
  );

  uint32_t value;
  ESP.rtcUserMemoryRead(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  if (crash_magic == (value & 0xFFFFFF00)) {
    pCONT_sup->WriteBuffer_P(PSTR(",\"CallChain\":["));
    uint32_t count = value & 0x3F;
    for (uint32_t i = 0; i < count; i++) {
      ESP.rtcUserMemoryRead(crash_rtc_offset +i, (uint32_t*)&value, sizeof(value));
      if (i > 0) { pCONT_sup->WriteBuffer_P(PSTR(",")); }
      pCONT_sup->WriteBuffer_P(PSTR("\"%08x\""), value);
    }
    pCONT_sup->WriteBuffer_P(PSTR("]"));
  }
  #endif // ESP8266
  
}

// #endif










































