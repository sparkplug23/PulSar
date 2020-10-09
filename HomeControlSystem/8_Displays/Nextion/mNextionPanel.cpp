
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

mNextionPanel::mNextionPanel(void){}

// void mNextionPanel::init_object(mInterfaceController* _mcl){ mcl = _mcl; }

/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 *  WEBPAGE ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/


/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 * TASKER ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

void mNextionPanel::pre_init(void){
 
  // if (mcl->mset->pin[GPIO_SR04_TRIG] < 99) {  // not set when 255
  //   pin_rx = mcl->mset->pin[GPIO_SR04_TRIG];
  // }

  // if (mcl->mset->pin[GPIO_SR04_ECHO] < 99) {  // not set when 255
  //   pin_tx = mcl->mset->pin[GPIO_SR04_ECHO];
  // }

  // if((pin_rx>=0)&&(pin_tx>=0)){
  //   status_enabled = true;
  // }else{
  //   AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d %d"),pin_tx,pin_rx);
  //   status_enabled = false;
  // }

  // Check if pins are hardware serial
  if(true){
    swSer = new SoftwareSerial(D5,D6);//R-T, false, 256);
    swSer->begin(NEXTION_BAUD);
  }
  
}


void mNextionPanel::init()
{ 
  
  // init variables
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));
  memcpy(lcdVersionQuery,"p[0].b[2].val",sizeof("p[0].b[2].val"));  

  // needs non blocking
  while(!lcdConnected && (millis() < 5000)){ // Wait up to 5 seconds for serial input from LCD
    nextionHandleInput();
  }

  if(lcdConnected){
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD responding, continuing program load"));
    nextionSendCmd("connect");
  }
  else{
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD not responding, continuing program load"));
  }

  // Configure some items
  //nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\""); // QR code show wifi connect


  memset(flash_message.message,0,sizeof(flash_message.message));
  sprintf(flash_message.message,"%s","Awaitng a message to show");






  
  char page_default_command[10];
  sprintf(page_default_command,"page %d",NEXTION_DEFAULT_PAGE_NUMBER);

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;

  nextionSendCmd(page_default_command); //set page 1 as default 

  //nextionSendCmd("page 1"); //set page 1 as default 

Command_SetPage(settings.page);



}



int8_t mNextionPanel::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:
      EverySecond_SendScreenInfo();
      EverySecond_FlashScreen();

       if(fOpenHABDataStreamActive_last_secs){ //only if positive
          if(fOpenHABDataStreamActive_last_secs++>OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS){
            fOpenHABDataStreamActive_last_secs = -1;
            fOpenHABDataStreamActive = false;
          }
        }
    break;
    case FUNC_EVERY_HOUR:
      Command_SetPage(settings.page);   //temp fix
    break;
    /************
     * Network SECTION * 
    *******************/
    case FUNC_WIFI_CONNECTED:
      wifiConnected();
    break;
    case FUNC_WIFI_DISCONNECTED:
      wifiDisconnected();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_RESET:
      // Reset to the initial parameters
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      mqttConnected();
    break;
    case FUNC_MQTT_DISCONNECTED:
      mqttDisconnected();
    break;
    #endif
  }

  /************
   * WEBPAGE SECTION * 
  *******************/
  // return Tasker_Web(function);

}


int8_t mNextionPanel::Tasker(uint8_t function, JsonObjectConst obj){
  AddLog_P(LOG_LEVEL_INFO, PSTR("F::%s"),__FUNCTION__);
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_TopicCheck_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


int8_t mNextionPanel::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/nextion")>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

int8_t mNextionPanel::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

  AddLog_P(LOG_LEVEL_INFO,PSTR("mDoorBell::parsesub_TopicCheck_JSONCommand"));

  uint8_t name_num=-1,state=-1;    


////////////////////////////////////////////////////////////////////////////////////////////////////
// Incoming Namespace (replace /device/ with /group/ for group commands)
// '[...]/device/command' -m 'dim=50' = nextionSendCmd("dim=50")
// '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
// '[...]/device/command/page' -m '1' = nextionSendCmd("page 1")
// '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' = nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
// '[...]/device/command/lcdupdate' -m '' = nextionStartOtaDownload("lcdFirmwareUrl")
// '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' = espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
// '[...]/device/command/espupdate' -m '' = espStartOta("espFirmwareUrl")
// '[...]/device/command/p[1].b[4].txt' -m '' = nextionGetAttr("p[1].b[4].txt")
// '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' = nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
// uint8_t mNextionPanel::parse_JSONCommand(){

  // Check if instruction is for me
  // if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/nextion")>=0){
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
  //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  //   fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
  //   fOpenHABDataStreamActive = true;
  // }else{
  //   return 0; // not meant for here
  // }



  int8_t isserviced = 0;
//these need to be subtopic functions



  // if(strstr(data_buffer2.topic.ctr,"/commands")){ 
  // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/commands",sizeof("/commands")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
    #endif
    isserviced += parsesub_Commands(obj);
  }else 
   // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
    #endif
    isserviced += parsesub_SetMulti(obj);
  }else{
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
  }  
  
  
  // if(strstr(data_buffer2.topic.ctr,"/set_single")){
    
  //   StaticJsonDocument<300> doc;
  //   DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  //   JsonObject obj = doc.as<JsonObject>();

  //   if((!obj["attribute"].isNull())&&(!obj["value"].isNull())){ 
  //     const char* attribute = obj["attribute"];
  //     const char* value = obj["value"];
  //     nextionSetAttr(attribute,value);
  //   }

  // }else
  // // Get element (ask, no value)
  // if(strstr(data_buffer2.topic.ctr,"/get_single")){
    
  //   StaticJsonDocument<300> doc;
  //   DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  //   JsonObject obj = doc.as<JsonObject>();

  //   if(!obj["attribute"].isNull()){ 
  //     const char* attribute = obj["attribute"];
  //     nextionGetAttr(attribute);
  //   }

  // }else
  // // Get element (ask, no value)
  // if(strstr(data_buffer2.topic.ctr,"/nextion/flash_message")){
  //   isserviced += parsesub_FlashMessage();
  // }

  // else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
  // { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' 
  //== nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
  //   if (strPayload == ""){
  //     nextionStartOtaDownload(lcdFirmwareUrl);
  //   }else{
  //     nextionStartOtaDownload(strPayload);
  //   }
  // }
  // else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
  // { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' 
  // == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
  //   if (strPayload == ""){
  //     espStartOta(espFirmwareUrl);
  //   }else{
  //     espStartOta(strPayload);
  //   }
  // }

  data_buffer2.isserviced += isserviced;
 
//  return isserviced;



}




int8_t mNextionPanel::parsesub_Commands(JsonObjectConst obj){

  char command_ctr[100]; memset(command_ctr,0,sizeof(command_ctr));
  
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_TOPIC "/commands"));    

    if(!obj["page"].isNull()){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "page"));    
      settings.page = obj["page"];
      sprintf(command_ctr,"page %d",settings.page);
      nextionSendCmd(command_ctr);
    }else
    if(!obj["command"].isNull()){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "command"));    
      const char* command = obj["command"];
      nextionSendCmd(command);
    }else
    if(!obj["statusupdate"].isNull()){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "statusupdate"));    
      uint8_t statusupdate = obj["statusupdate"];
      sprintf(command_ctr,"statusupdate %d",statusupdate);
      mqtthandler_settings_teleperiod.fSendNow = true;
    }else
    if(!obj["brightness"].isNull()){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "brightness"));    
      uint8_t brightness = obj["brightness"];
      //nextionSetAttr("dim", String(brightness));
      //sprintf(command_ctr,"dims=%d",brightness);
      nextionSendCmd("dims=dim");
    }else
    if(!obj["lcdreboot"].isNull()){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));    
      uint8_t lcdreboot = obj["lcdreboot"];
      nextionReset();
    }else
    if(!obj["onoff"].isNull()){ 
      const char* onoff = obj["onoff"];
      if(strstr(onoff,"ON")){
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"onoff",D_ON);    
        nextionSendCmd("dim=dims");
      }else
      if(strstr(onoff,"OFF")){
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));     
        nextionSendCmd("dims=dim");
        nextionSetAttr("dim", "0");
      }
    }else{
      AddLog_P(LOG_LEVEL_INFO,PSTR("ELSE FOUND"));
    }

}



int8_t mNextionPanel::parsesub_SetMulti(JsonObjectConst obj){

  AddLog_P(LOG_LEVEL_INFO, PSTR("F::%s"),__FUNCTION__);

  if(!obj["commands"].isNull()){
    JsonArrayConst array = obj["commands"];
    for(JsonVariantConst val : array) {
      nextionSendCmd(val.as<const char*>());
    }
  }
  
}


/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * TIMED FUNCTIONS ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/


void mNextionPanel::EverySecond_FlashScreen(){

  if(flash_message.cShowSeconds==0){
    // Return screen to previous
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"settings.page_saved",settings.page_saved);
    Command_SetPage(settings.page_saved);
    flash_message.cShowSeconds = -1;
  }else
  if(flash_message.cShowSeconds>0){
    flash_message.cShowSeconds--;
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"flash_message.cShowSeconds",flash_message.cShowSeconds);
  }

} //end F


void mNextionPanel::EverySecond_SendScreenInfo(){

  // char uptime_ctr[40]; memset(uptime_ctr,'\0',sizeof(uptime_ctr));
  // sprintf(uptime_ctr, "%02d:%02d:%02d",
  // mcl->mt->uptime.hour,mcl->mt->uptime.minute,mcl->mt->uptime.second);

  char rtc_ctr[40]; memset(rtc_ctr,'\0',sizeof(rtc_ctr));
  sprintf(rtc_ctr, "%02d:%02d:%02d",
  pCONT_time->mtime.hour,pCONT_time->mtime.minute,pCONT_time->mtime.second);

  SetAttribute_Txt(settings.page,5,rtc_ctr);
  
  char health_ctr[40]; memset(health_ctr,'\0',sizeof(health_ctr));
  sprintf(health_ctr, "%c%c%c %d",
    WiFi.status() == WL_CONNECTED ? 'N' : 'n',
    pCONT_mqtt->pubsub->connected() ? 'M' : 'm',
    fOpenHABDataStreamActive ? 'O' : 'o',
    pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI())
  );

  SetAttribute_Txt(settings.page,6,health_ctr);

}




void mNextionPanel::EveryLoop(){

  if (nextionHandleInput()){ // Process user input from HMI
    nextionProcessInput();
  }

  if ((lcdVersion < 1) && (millis() <= (nextionRetryMax * nextionCheckInterval)))
  { // Attempt to connect to LCD panel to collect model and version info during startup
    nextionConnect();
  }
    
  if ((millis() > (nextionRetryMax * nextionCheckInterval)) && !startupCompleteFlag)
  { // We still don't have LCD info so go ahead and run the rest of the checks once at startup anyway
    updateCheck();
    startupCompleteFlag = true;
  }

  // Check if long press threshold reached
  if(screen_press.fEnableImmediateButtonTime){
    if(mSupport::TimeReachedNonReset(&screen_press.tSavedButtonONEvent,LONG_PRESS_DURATION)){
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "LONG_PRESS_DURATION reached"));
      screen_press.fEnableImmediateButtonTime=false;
      fEnableIgnoreNextOffEvent = true;
      MQTTSend_LongPressEvent();
      MQTTSend_PressEvent();
    }
  }

}



/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * Useful Commands ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

void mNextionPanel::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}

void mNextionPanel::Command_SetBrightness(uint8_t brightness_percentage){
  char ctr[10];
  sprintf(ctr,"dims=%d",brightness_percentage); //dims range 0-100
  nextionSendCmd(ctr);
  settings.brightness_percentage = brightness_percentage;
}

void mNextionPanel::Command_SetBrightness255(uint8_t brightness){
  settings.brightness_percentage = map(brightness,0,255,0,100);
  Command_SetBrightness(settings.brightness_percentage);
}

void mNextionPanel::Command_ToggleBrightness(){  
  Command_SetBrightness(settings.brightness_percentage>0?0:settings.brightness_percentage);
}


/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 *  MQTT ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/

/***
 * Add subscribes for new connection to mqtt
 * Show update on panel
 * */
void mNextionPanel::mqttConnected()
{ // MQTT connection and subscriptions

  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));

  // Show connection success
  //nextionSendCmd("page 0");

  // char display_ctr[30];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");

  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

  // hide QR code
  // nextionSendCmd("vis 3,0");

}


void mNextionPanel::mqttDisconnected(){
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
  // char display_ctr[120];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connect to%s\\rFAILED rc=%s\"",WiFi.localIP().toString(),"192.168.1.65",pCONT_mqtt->pubsub->stateCtr());

  // QR code show wifi connect
  // nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\"");
  // // show item
  // nextionSendCmd("vis 3,1");
  
  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

}


void mNextionPanel::MQTTSend_PressEvent(){

  // if(!mSupport::TimeReached(&tSaved_MQTTSend_PressEvent,200)){
  //   // Debounce and only send once per event (ie ignore release trigger following immediate trigger)
  //   return;
  // }
  
  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];
  uint32_t tSavedTimeSincePressOn = abs(millis() - screen_press.tSavedButtonONEvent);

  StaticJsonDocument<100> doc;
  JsonObject rootobj = doc.to<JsonObject>();
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");

  char event_ctr[20];
  memset(event_ctr,0,sizeof(event_ctr));
  sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
  rootobj["event"] = event_ctr;
  rootobj["value"] = (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS";
  rootobj["duration"] = tSavedTimeSincePressOn;

  memset(&data_buffer2.topic.ctr,0,sizeof(data_buffer2.topic.ctr));
  data_buffer2.payload.len = measureJson(rootobj)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  tSaved_MQTTSend_PressEvent = millis();

  pCONT_mqtt->ppublish("status/nextion/event/press",data_buffer2.payload.ctr,0);

}

void mNextionPanel::MQTTSend_LongPressEvent(){

  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];

  StaticJsonDocument<100> doc;
  JsonObject rootobj = doc.to<JsonObject>();
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");

  char event_ctr[20];
  memset(event_ctr,0,sizeof(event_ctr));
  sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
  rootobj["event"] = event_ctr;
  rootobj["value"] = "LONG_PRESS";
  rootobj["duration_threshold"] = LONG_PRESS_DURATION;

  memset(&data_buffer2,0,sizeof(data_buffer2));
  data_buffer2.payload.len = measureJson(rootobj)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  pCONT_mqtt->ppublish("status/nextion/event",data_buffer2.payload.ctr,0);
  pCONT_mqtt->ppublish("status/nextion/event/start",data_buffer2.payload.ctr,0);

}



int8_t mNextionPanel::parsesub_FlashMessage(){

  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "%s"),"parsesub_FlashMessage");

  #ifdef JSONDOCUMENT_STATIC
    StaticJsonDocument<800> doc;
  #else
    DynamicJsonDocument doc(600);
  #endif
  DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  if(error){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
    Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
    return 0;
  }
  JsonObject obj = doc.as<JsonObject>();

  int8_t tmp_id = 0;
  int8_t isserviced = 0;

  if(!obj["message"].isNull()){ 
    const char* messagectr = obj["message"];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"message",messagectr);
    sprintf(flash_message.message,"%s",messagectr);
  }

  if(!obj["time_secs"].isNull()){ 
    uint8_t time = obj["time_secs"];
    flash_message.cShowSeconds = time>60?60:time;
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"cShowSeconds",flash_message.cShowSeconds);
  }

  if(!obj["background_colour"].isNull()){ 
    uint32_t background_colour = obj["background_colour"];
    flash_message.background_colour = background_colour;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"background_colour",background_colour);
  }

  if(!obj["font_colour"].isNull()){ 
    uint32_t font_colour = obj["font_colour"];
    flash_message.font_colour = font_colour;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"font_colour",font_colour);
  }
  


  flash_message.page = PAGE_ERROR_ID;

  //Save page
  settings.page_saved = settings.page;
  //Go to message page
  Command_SetPage(flash_message.page);

  flash_message.cShowSeconds = 3;

  
  // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
  // // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
  // sprintf(display_ctr,"%s",flash_message.message);
        
    SetAttribute_FontColour(flash_message.page,1,flash_message.font_colour);
    SetAttribute_BackgroundColour(flash_message.page,1,flash_message.background_colour);
    SetAttribute_Txt(flash_message.page,1,flash_message.message);




   
  flash_message.isrunning = true;  

  return 0;
}



/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 *  FUNCTIONS ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionHandleInput()
{ // Handle incoming serial data from the Nextion panel
  // This will collect serial data from the panel and place it into the global buffer
  // nextionReturnBuffer[nextionReturnIndex]
  // Return: true if we've received a string of 3 consecutive 0xFF values
  // Return: false otherwise
  bool nextionCommandComplete = false;
  static int nextionTermByteCnt = 0;   // counter for our 3 consecutive 0xFFs
  static String hmiDebug = ""; // assemble a string for debug output

  if (serial_available())
  {
    // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEXTION " if (Serial.available())"));
    lcdConnected = true;
    byte nextionCommandByte = serial_read();

    hmiDebug += (" 0x" + String(nextionCommandByte, HEX));
    // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
    if (nextionCommandByte == 0xFF)
    {
      nextionTermByteCnt++;
      if (nextionTermByteCnt >= 3)
      { // We have received a complete command
        nextionCommandComplete = true;
        nextionTermByteCnt = 0; // reset counter
      }
    }
    else
    {
      nextionTermByteCnt = 0; // reset counter if a non-term byte was encountered
    }
    nextionReturnBuffer[nextionReturnIndex] = nextionCommandByte;
    nextionReturnIndex++;
  }
  if (nextionCommandComplete)
  {
//    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),
    //Serial.println(ConvertBytetoASCII(nextionReturnBuffer,nextionReturnIndex));
    // for(int i=0;i<nextionReturnIndex;i++){
    //   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "%d | %s"),i,String(nextionReturnBuffer[i], HEX));
    // }
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),hmiDebug.c_str());
    hmiDebug = "";
  }
  return nextionCommandComplete;
}

// char* mNextionPanel::ConvertBytetoASCII(byte* data, uint8_t len){

//   //data_in = byte format
//   //output  = needs to be c_str with null termination

//   char output[128];
//   memset(output,0,sizeof(output));

//   for(int i=0;i<len;i++){
//     Serial.print(data[i],DEC);
//     output[i] = data[i]+48;
//     Serial.println(output[i],DEC);
//   }
//   return output;
// }




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionProcessInput()
{ // Process incoming serial commands from the Nextion panel
  // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
  // tl;dr, command byte, command data, 0xFF 0xFF 0xFF

  //Serial.println("nextionProcessInput");
  StaticJsonDocument<100> doc;
  JsonObject rootobj = doc.to<JsonObject>();
  char event_ctr[30];
  //https://www.itead.cc/wiki/Nextion_Instruction_Set

  // first instructions byte
  switch(nextionReturnBuffer[0]){
    case NEXTION_COMMAND_INVALID_INSTRUCTION:
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX D_NEXTION_COMMAND D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR));   
    break;

  }
  

  if (nextionReturnBuffer[0] == 0x65)
  { // Handle incoming touch command
    // 0x65+Page ID+Component ID+TouchEvent+End
    // Return this data when the touch event created by the user is pressed.
    // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
    // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
    // Meaning: Touch Event, Page 0, Object 2, Press
    String nextionPage = String(nextionReturnBuffer[1]);
    String nextionButtonID = String(nextionReturnBuffer[2]);
    byte nextionButtonAction = nextionReturnBuffer[3];

    screen_press.page = nextionReturnBuffer[1];
    screen_press.event = nextionReturnBuffer[2];

    if (nextionButtonAction == 0x01) // ON=PRESSED
    {
      screen_press.tSavedButtonONEvent = millis();
      screen_press.fEnableImmediateButtonTime = true; 

      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_ON);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
        rootobj["event"] = event_ctr;
        rootobj["value"] = D_JSON_ON;

      memset(&data_buffer2,0,sizeof(data_buffer2));
      data_buffer2.payload.len = measureJson(rootobj)+1;
      serializeJson(doc,data_buffer2.payload.ctr);

      pCONT_mqtt->ppublish("status/nextion/event",data_buffer2.payload.ctr,0);
      pCONT_mqtt->ppublish("status/nextion/event/start",data_buffer2.payload.ctr,0);

    }
    if (nextionButtonAction == 0x00) // OFF - LET_GO
    {
      screen_press.tSavedButtonOFFEvent = millis();
      screen_press.fEnableImmediateButtonTime = false; //start timer
      screen_press.tSavedButtonONDurationEvent = screen_press.tSavedButtonOFFEvent - screen_press.tSavedButtonONEvent;
      screen_press.duration = screen_press.tSavedButtonONDurationEvent;
      
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_OFF);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
      rootobj["event"] = event_ctr;
      rootobj["value"] = D_JSON_OFF;
      rootobj["duration"] = screen_press.tSavedButtonONDurationEvent;

      memset(&data_buffer2,0,sizeof(data_buffer2));
      data_buffer2.payload.len = measureJson(rootobj)+1;
      serializeJson(doc,data_buffer2.payload.ctr);

      if(!fEnableIgnoreNextOffEvent){
        AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = NOT set"));
        pCONT_mqtt->ppublish("status/nextion/event",data_buffer2.payload.ctr,0);
        pCONT_mqtt->ppublish("status/nextion/event/end",data_buffer2.payload.ctr,0);
        MQTTSend_PressEvent();
      }else{
        fEnableIgnoreNextOffEvent = false;// reset to listen to next event
        AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = reset"));
      }


      // Now see if this object has a .val that might have been updated.  Works for sliders,
      // two-state buttons, etc, throws a 0x1A error for normal buttons which we'll catch and ignore
      // mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // mqttGetSubtopicJSON = "p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
    }
  }
  else if (nextionReturnBuffer[0] == 0x66)
  { // Handle incoming "sendme" page number
    // 0x66+PageNum+End
    // Example: 0x66 0x02 0xFF 0xFF 0xFF
    // Meaning: page 2
    String nextionPage = String(nextionReturnBuffer[1]);
    
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "[sendme Page] \"%s\""),nextionPage.c_str());

    if ((settings.page != nextionPage.toInt()) && ((nextionPage != "0") || nextionReportPage0))
    { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )
      settings.page = nextionPage.toInt();
      String mqttPageTopic = mqttStateTopic + "/page";      
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: mqttPageTopic=\"%s\" nextionPage=\"%s\""),mqttPageTopic.c_str(),nextionPage.c_str());
      //mqttClient.publish(mqttPageTopic, nextionPage);
      pCONT_mqtt->ppublish("status/nextion/event4",nextionPage.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x67)
  { // Handle touch coordinate data
    // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
    // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
    // Meaning: Coordinate (122,30), Touch Event: Press
    // issue Nextion command "sendxy=1" to enable this output
    uint16_t xCoord = nextionReturnBuffer[1];
    xCoord = xCoord * 256 + nextionReturnBuffer[2];
    uint16_t yCoord = nextionReturnBuffer[3];
    yCoord = yCoord * 256 + nextionReturnBuffer[4];
    String xyCoord = String(xCoord) + ',' + String(yCoord);
    byte nextionTouchAction = nextionReturnBuffer[5];
    if (nextionTouchAction == 0x01)
    {  
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch ON] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOn";
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/xyCoord",xyCoord.c_str(),0);
    }
    else if (nextionTouchAction == 0x00)
    {
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch OFF] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOff";
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/event6",xyCoord.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x70)
  { // Handle get string return
    // 0x70+ASCII string+End
    // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
    // Meaning: String data, ABCD1234
    String getString;
    for (int i = 1; i < nextionReturnIndex - 3; i++)
    { // convert the payload into a string
      getString += (char)nextionReturnBuffer[i];
    }
    
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [String Return] '%s'"),getString.c_str());
  
    if (mqttGetSubtopic == "")
    { // If there's no outstanding request for a value, publish to mqttStateTopic
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttStateTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
    }
    else
    { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;      
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttReturnTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x71)
  { // Handle get int return
    // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
    // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
    // Meaning: Integer data, 123
    unsigned long getInt = nextionReturnBuffer[4];
    getInt = getInt * 256 + nextionReturnBuffer[3];
    getInt = getInt * 256 + nextionReturnBuffer[2];
    getInt = getInt * 256 + nextionReturnBuffer[1];
    String getString = String(getInt);
    //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: [Int Return] '")) + getString + "'");

    if (lcdVersionQueryFlag)
    {
      lcdVersion = getInt;
      lcdVersionQueryFlag = false;
      ////AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: lcdVersion '")) + String(lcdVersion) + "'");
    }
    else if (mqttGetSubtopic == "")
    {
      //mqttClient.publish(mqttStateTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event9",getString.c_str(),0);
    }
    // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
    else
    {
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
      //mqttClient.publish(mqttReturnTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event10",getString.c_str(),0);
      String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopicJSON + String(F("\", \"value\":")) + getString + String(F("}"));
      //mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      pCONT_mqtt->ppublish("status/nextion/event11",mqttButtonJSONEvent.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
  { // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol
    String comokField;
    uint8_t comokFieldCount = 0;
    byte comokFieldSeperator = 0x2c; // ","

    for (uint8_t i = 0; i <= nextionReturnIndex; i++)
    { // cycle through each byte looking for our field seperator
      if (nextionReturnBuffer[i] == comokFieldSeperator)
      { // Found the end of a field, so do something with it.  Maybe.
        if (comokFieldCount == 2)
        {
          nextionModel = comokField;
          ////AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: nextionModel: ")) + nextionModel);
        }
        comokFieldCount++;
        comokField = "";
      }
      else
      {
        comokField += String(char(nextionReturnBuffer[i]));
      }
    }
  }

  // else if (nextionReturnBuffer[0] == 0x1A)
  // { // Catch 0x1A error, possibly from .val query against things that might not support that request
  //   // 0x1A+End
  //   // ERROR: Variable name invalid
  //   // We'll be triggering this a lot due to requesting .val on every component that sends us a Touch Off
  //   // Just reset mqttGetSubtopic and move on with life.
  //   mqttGetSubtopic = "";
  // }
  nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
}



void mNextionPanel::SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr){
  char command_ctr[100];
  sprintf(command_ctr,"p[%d].b[%d].txt=\"%s\"",page,element_id,ctr);
  serial_print_suffixed(command_ctr);  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].bco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].pco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}




void mNextionPanel::nextionSetAttr(const char* hmiAttribute, const char* hmiValue)
{ // Set the value of a Nextion component attribute
  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->print(hmiAttribute);
    swSer->print("=");
    swSer->print(utf8ascii2((char*)hmiValue));
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
  #else
    SERIAL_NEXTION_TX.print(hmiAttribute);
    SERIAL_NEXTION_TX.print("=");
    SERIAL_NEXTION_TX.print(utf8ascii2((char*)hmiValue));
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  #endif
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionGetAttr(const char* c_str)//String hmiAttribute)
{ // Get the value of a Nextion component attribute
  // This will only send the command to the panel requesting the attribute, the actual
  // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic

  char hmiattribute_ctr[100]; 
  memset(hmiattribute_ctr,0,sizeof(hmiattribute_ctr));
  sprintf(hmiattribute_ctr,"get %s",c_str);
  serial_print_suffixed(hmiattribute_ctr);
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " GET 'get %s'"),hmiattribute_ctr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}

// void mNextionPanel::nextionSendCmd_JSON(String s_str)
// { // Send a raw command to the Nextion panel
//   serial_print(utf8ascii(s_str));
//   serial_print_suffix();
//   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),s_str.c_str());
// }



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionConnect()
{
  if ((millis() - nextionCheckTimer) >= nextionCheckInterval)
  {
    static unsigned int nextionRetryCount = 0;
    if ((nextionModel.length() == 0) && (nextionRetryCount < (nextionRetryMax - 2)))
    { // Try issuing the "connect" command a few times
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion connect request"));
      nextionSendCmd("connect");
      nextionRetryCount++;
      nextionCheckTimer = millis();
    }
    else if ((nextionModel.length() == 0) && (nextionRetryCount < nextionRetryMax))
    { // If we still don't have model info, try to change nextion serial speed from 9600 to 115200
      nextionSetSpeed();
      nextionRetryCount++;
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion serial speed 115200 request"));
      nextionCheckTimer = millis();
    }
    else if ((lcdVersion < 1) && (nextionRetryCount <= nextionRetryMax))
    {
      if (nextionModel.length() == 0)
      { // one last hail mary, maybe the serial speed is set correctly now
        nextionSendCmd("connect");
      }
      //nextionSendCmd("get " + lcdVersionQuery.toString().c_str());
      lcdVersionQueryFlag = true;
      nextionRetryCount++;
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion version query"));
      nextionCheckTimer = millis();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSetSpeed()
{
  
  ////#define USE_NEXTION_SOFTWARE_SERIAL
 #ifdef USE_NEXTION_SOFTWARE_SERIAL

  Serial.println(F("HMI: No Nextion response, attempting 9600bps connection\n\n\n\n\n\n\n"));
  swSer->begin(9600);
  swSer->write(nextionSuffix, sizeof(nextionSuffix));
  swSer->print("bauds=38400");
  //swSer->print("bauds=9600");
  swSer->write(nextionSuffix, sizeof(nextionSuffix));
  swSer->flush();
  swSer->begin(38400);
  #else
  
  ////AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: No Nextion response, attempting 9600bps connection"));
  SERIAL_NEXTION_TX.begin(9600);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.print("bauds=38400");
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  
  SERIAL_NEXTION_TX.begin(115200);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.print("bauds=38400");
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  #endif

}




/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 *  INTERFACE IE FUNCTIONS THAT INTERFACE WITH HARDWARE DIRECTLY ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionReset()
{
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD");
  //digitalWrite(nextionResetPin, LOW);
  
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->print("rest");
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
    SERIAL_NEXTION_TX.print("rest");
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
    SERIAL_NEXTION_TX.flush();
  #endif

  delay(100);
  //digitalWrite(nextionResetPin, HIGH);

  unsigned long lcdResetTimer = millis();
  const unsigned long lcdResetTimeout = 5000;

  lcdConnected = false;
  while (!lcdConnected && (millis() < (lcdResetTimer + lcdResetTimeout)))
  {
    Serial.println("while-nextionReset");
    nextionHandleInput();
  }
  if (lcdConnected)
  {
    //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),(F("HMI: Rebooting LCD completed"));
    if (settings.page)
    {


char command_ctr[15];

sprintf(command_ctr,"page %d\0",settings.page);

      nextionSendCmd(command_ctr);
    }
  }
  else
  {
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ERROR: Rebooting LCD completed, but LCD is not responding.");
  }
  //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espWifiSetup()
{ // Connect to WiFi
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"WiFi Connecting\"");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// void mNextionPanel::espWifiConfigCallback(WiFiManager *myWiFiManager)
// { // Notify the user that we're entering config mode
//   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),(F("WIFI: Failed to connect to assigned AP, entering config mode"));
//   while (millis() < 800)
//   { // for factory-reset system this will be called before display is responsive. give it a second.
//     delay(10);
//   }
//   nextionSendCmd("page 0");
//   nextionSetAttr("p[0].b[1].txt", "\"Configure NEXTION:\\rAP:" + String(wifiConfigAP) + "\\rPass:" + String(wifiConfigPass) + "\\r\\r\\r\\r\\r\\r\\rWeb:192.168.4.1\"");
//   nextionSetAttr("p[0].b[3].txt", "\"WIFI:S:" + String(wifiConfigAP) + ";T:WPA;P:" + String(wifiConfigPass) + ";;\"");
//   nextionSendCmd("vis 3,1");
// }




























































// UTF8-Decoder: convert UTF8-String to extended ASCII http://playground.arduino.cc/main/Utf8ascii
// Workaround for issue here: https://github.com/home-assistant/home-assistant/issues/9528
// Nextion claims that "Unicode and UTF will not be among the supported encodings", so this should
// be safe to run against all attribute values coming in.
static byte c1; // Last character buffer
byte mNextionPanel::utf8ascii(byte ascii)
{ // Convert a single Character from UTF8 to Extended ASCII. Return "0" if a byte has to be ignored.
  if (ascii < 128)
  { // Standard ASCII-set 0..0x7F handling
    c1 = 0;
    return (ascii);
  }
  // get previous input
  byte last = c1; // get last char
  c1 = ascii;     // remember actual character
  switch (last)
  { // conversion depending on first UTF8-character
  case 0xC2:
    return (ascii);
    break;
  case 0xC3:
    return (ascii | 0xC0);
    break;
  case 0x82:
    if (ascii == 0xAC)
      return (0x80); // special case Euro-symbol
  }
  return (0); // otherwise: return zero, if character has to be ignored
}

String mNextionPanel::utf8ascii(String s)
{ // convert String object from UTF8 String to Extended ASCII
  String r = "";
  char c;
  for (uint16_t i = 0; i < s.length(); i++)
  {
    c = utf8ascii(s.charAt(i));
    if (c != 0)
      r += c;
  }
  return r;
}

void mNextionPanel::utf8ascii(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
  uint16_t k = 0;
  char c;
  for (uint16_t i = 0; i < strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c != 0)
      s[k++] = c;
  }
  s[k] = 0;
}

char* mNextionPanel::utf8ascii2(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
  uint16_t k = 0;
  char c;
  for (uint16_t i = 0; i < strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c != 0)
      s[k++] = c;
  }
  s[k] = 0;
  return s;
}
















////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espSetupOta()
{ // (mostly) boilerplate OTA setup from library examples

  // ArduinoOTA.setHostname(nextionNode);
  // ArduinoOTA.setPassword(configPassword);

  // ArduinoOTA.onStart([]() {
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update start"));
  //   nextionSendCmd("page 0");
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\"");
  // });
  // ArduinoOTA.onEnd([]() {
  //   nextionSendCmd("page 0");
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update complete"));
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rComplete!\"");
  //   //espReset();
  // });
  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rProgress: " + String(progress / (total / 100)) + "%\"");
  // });
  // ArduinoOTA.onError([](ota_error_t error) {
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: ERROR code ")) + String(error));
  //   if (error == OTA_AUTH_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Auth Failed"));
  //   else if (error == OTA_BEGIN_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Begin Failed"));
  //   else if (error == OTA_CONNECT_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Connect Failed"));
  //   else if (error == OTA_RECEIVE_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Receive Failed"));
  //   else if (error == OTA_END_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - End Failed"));
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA FAILED\"");
  //   delay(5000);
  //   nextionSendCmd("page " + String(settings.page));
  // });
  // ArduinoOTA.begin();
  // //debugPrintln(F("ESP OTA: Over the Air firmware update ready"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espStartOta(String espOtaUrl)
{ // Update ESP firmware from HTTP
  // nextionSendCmd("page 0");
  // nextionSetAttr("p[0].b[1].txt", "\"HTTP update\\rstarting...\"");
  // WiFiUDP::stopAll(); // Keep mDNS responder from breaking things

  // t_httpUpdate_return returnCode = ESPhttpUpdate.update(wifiClient, espOtaUrl);
  // switch (returnCode)
  // {
  // case HTTP_UPDATE_FAILED:
  //   //debugPrintln("ESPFW: HTTP_UPDATE_FAILED error " + String(ESPhttpUpdate.getLastError()) + " " + ESPhttpUpdate.getLastErrorString());
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rFAILED\"");
  //   break;

  // case HTTP_UPDATE_NO_UPDATES:
  //   //debugPrintln(F("ESPFW: HTTP_UPDATE_NO_UPDATES"));
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rNo update\"");
  //   break;

  // case HTTP_UPDATE_OK:
  //   //debugPrintln(F("ESPFW: HTTP_UPDATE_OK"));
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rcomplete!\\r\\rRestarting.\"");
  //   espReset();
  // }
  // delay(5000);
  // nextionSendCmd("page " + String(settings.page));
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espReset()
{
  // //debugPrintln(F("RESET: NEXTION reset"));
  // if (//mqttClient.connected())
  // {
  //   //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
  //   //mqttClient.publish(mqttSensorTopic, "{\"status\": \"unavailable\"}");
  //   //mqttClient.disconnect();
  // }
  nextionReset();
  AddLog_P(LOG_LEVEL_TEST,PSTR("mNextionPanel::espReset()"));
  // ESP.reset();
  // delay(5000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::updateCheck()
{ // firmware update check
  // HTTPClient updateClient;
  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Checking update URL: ")) + String(UPDATE_URL));
  // String updatePayload;
  // updateClient.begin(wifiClient, UPDATE_URL);
  // int httpCode = updateClient.GET(); // start connection and send HTTP header

  // if (httpCode > 0)
  // { // httpCode will be negative on error
  //   if (httpCode == HTTP_CODE_OK)
  //   { // file found at server
  //     updatePayload = updateClient.getString();
  //   }
  // }
  // else
  // {
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Update check failed: ")) + updateClient.errorToString(httpCode));
  //   return false;
  // }
  // updateClient.end();

  // DynamicJsonDocument updateJson(2048);
  // DeserializationError jsonError = deserializeJson(updateJson, updatePayload);

  // if (jsonError)
  // { // Couldn't parse the returned JSON, so bail
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: JSON parsing failed: ")) + String(jsonError.c_str()));
  //   return false;
  // }
  // else
  // {
  //   if (!updateJson["d1_mini"]["version"].isNull())
  //   {
  //     updateEspAvailableVersion = updateJson["d1_mini"]["version"].as<float>();
  //     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateEspAvailableVersion: ")) + String(updateEspAvailableVersion));
  //     espFirmwareUrl = updateJson["d1_mini"]["firmware"].as<String>();
  //     if (updateEspAvailableVersion > nextionVersion)
  //     {
  //       updateEspAvailable = true;
  //       //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New ESP version available: ")) + String(updateEspAvailableVersion));
  //     }
  //   }
  //   if (nextionModel && !updateJson[nextionModel]["version"].isNull())
  //   {
  //     updateLcdAvailableVersion = updateJson[nextionModel]["version"].as<int>();
  //     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateLcdAvailableVersion: ")) + String(updateLcdAvailableVersion));
  //     lcdFirmwareUrl = updateJson[nextionModel]["firmware"].as<String>();
  //     if (updateLcdAvailableVersion > lcdVersion)
  //     {
  //       updateLcdAvailable = true;
  //       //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New LCD version available: ")) + String(updateLcdAvailableVersion));
  //     }
  //   }
  //   //debugPrintln(F("UPDATE: Update check completed"));
  // }
  
  // return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionOtaResponse()
{ // Monitor the serial port for a 0x05 response within our timeout

  unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
  unsigned long nextionCommandTimer = millis(); // record current time for our timeout
  bool otaSuccessVal = false;
  while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
  {

    if(otatransfererror){
      AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "QUIT OTA and Restart display"));
      nextionReset();
      break;
    }

  
      #ifdef USE_NEXTION_SOFTWARE_SERIAL
      if (swSer->available())
      {
      byte inByte = swSer->read();
      #else 
      if (Serial.available())
      {
      byte inByte = Serial.read();
      #endif

      if (inByte == 0x5)
      {
        AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"inByte == 0x5");
        otaSuccessVal = true;
        otatransfererror = true;
        break;
      }
      else
      {
        otatransfererror = false;
AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%x"),inByte);
        // //debugPrintln(String(inByte, HEX));
      }
    }
    else
    {
      delay(1);
    }
  }
  // delay(50);
  return otaSuccessVal;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionStartOtaDownload(String otaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "nextionStartOtaDownload otaurl"));

  // uint32_t lcdOtaFileSize = 0;
  // String lcdOtaNextionCmd;
  // uint32_t lcdOtaChunkCounter = 0;
  // uint16_t lcdOtaPartNum = 0;
  // uint32_t lcdOtaTransferred = 0;
  // uint8_t lcdOtaPercentComplete = 0;
  // const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  // static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Attempting firmware download from: ")) + otaUrl);
  // WiFiClient lcdOtaWifi;
  // HTTPClient lcdOtaHttp;
  // lcdOtaHttp.begin(lcdOtaWifi, otaUrl);
  // int lcdOtaHttpReturn = lcdOtaHttp.GET();
  // if (lcdOtaHttpReturn > 0)
  // { // HTTP header has been sent and Server response header has been handled
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET return code:")) + String(lcdOtaHttpReturn));
  //   if (lcdOtaHttpReturn == HTTP_CODE_OK)
  //   {                                                 // file found at server
  //     int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
  //     lcdOtaFileSize = lcdOtaRemaining;
  //     static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
  //     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
  //     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

  //     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: File found at Server. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

  //     WiFiUDP::stopAll(); // Keep mDNS responder and MQTT traffic from breaking things
  //     if (//mqttClient.connected())
  //     {
  //       //debugPrintln(F("LCD OTA: LCD firmware upload starting, closing MQTT connection."));
  //       //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
  //       //mqttClient.publish(mqttSensorTopic, "{\"status\": \"unavailable\"}");
  //       //mqttClient.disconnect();
  //     }

  //     WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
  //     SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
  //     SERIAL_NEXTION_TX.flush();
  //     nextionHandleInput();
  //     String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + ",115200,0";
  //     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
  //     SERIAL_NEXTION_TX.print(lcdOtaNextionCmd);
  //     SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  //     SERIAL_NEXTION_TX.flush();

  //     if (nextionOtaResponse())
  //     {
  //       //debugPrintln(F("LCD OTA: LCD upload command accepted."));
  //     }
  //     else
  //     {
  //       //debugPrintln(F("LCD OTA: LCD upload command FAILED.  Restarting device."));
  //       espReset();
  //     }
  //     //debugPrintln(F("LCD OTA: Starting update"));
  //     lcdOtaTimer = millis();
  //     while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
  //     {                                                // Write incoming data to panel as it arrives
  //       uint16_t lcdOtaHttpSize = stream->available(); // get available data size

  //       if (lcdOtaHttpSize)
  //       {
  //         uint16_t lcdOtaChunkSize = 0;
  //         if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
  //         {
  //           lcdOtaChunkSize = lcdOtaHttpSize;
  //         }
  //         else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
  //         {
  //           lcdOtaChunkSize = lcdOtaBufferSize;
  //         }
  //         else
  //         {
  //           lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
  //         }
  //         stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
  //         SERIAL_NEXTION_TX.flush();                              // make sure any previous writes the UART have completed
  //         SERIAL_NEXTION_TX.write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
  //         lcdOtaChunkCounter += lcdOtaChunkSize;
  //         if (lcdOtaChunkCounter >= 4096)
  //         {
  //           SERIAL_NEXTION_TX.flush();
  //           lcdOtaPartNum++;
  //           lcdOtaTransferred += lcdOtaChunkCounter;
  //           lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
  //           lcdOtaChunkCounter = 0;
  //           if (nextionOtaResponse())
  //           { // We've completed a chunk
  //             //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
  //             lcdOtaTimer = millis();
  //           }
  //           else
  //           {
  //             //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
  //             //debugPrintln(F("LCD OTA: failure"));
  //             delay(2000); // extra delay while the LCD does its thing
  //             espReset();
  //           }
  //         }
  //         else
  //         {
  //           delay(20);
  //         }
  //         if (lcdOtaRemaining > 0)
  //         {
  //           lcdOtaRemaining -= lcdOtaChunkSize;
  //         }
  //       }
  //       delay(10);
  //       if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  //       { // Our timer expired so reset
  //         //debugPrintln(F("LCD OTA: ERROR: LCD upload timeout.  Restarting."));
  //         espReset();
  //       }
  //     }
  //     lcdOtaPartNum++;
  //     lcdOtaTransferred += lcdOtaChunkCounter;
  //     if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
  //     {
  //       //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
  //       uint32_t lcdOtaDelay = millis();
  //       while ((millis() - lcdOtaDelay) < 5000)
  //       { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
  //         mcl->mweb->pWebServer->handleClient();
  //         delay(1);
  //       }
  //       espReset();
  //     }
  //     else
  //     {
  //       //debugPrintln(F("LCD OTA: Failure"));
  //       espReset();
  //     }
  //   }
  // }
  // else
  // {
  //   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET failed, error code ")) + lcdOtaHttp.errorToString(lcdOtaHttpReturn));
  //   espReset();
  // }
  // lcdOtaHttp.end();
  
}









////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::configRead()
{ // Read saved config.json from SPIFFS
  //debugPrintln(F("SPIFFS: mounting SPIFFS"));
  // #ifdef ESP8266
  // if (SPIFFS.begin())
  // {
  //   if (SPIFFS.exists("/config.json"))
  //   { // File exists, reading and loading
  //     //debugPrintln(F("SPIFFS: reading /config.json"));
  //     File configFile = SPIFFS.open("/config.json", "r");
  //     if (configFile)
  //     {
  //       size_t configFileSize = configFile.size(); // Allocate a buffer to store contents of the file.
  //       std::unique_ptr<char[]> buf(new char[configFileSize]);
  //       configFile.readBytes(buf.get(), configFileSize);

  //       DynamicJsonDocument configJson(1024);
  //       DeserializationError jsonError = deserializeJson(configJson, buf.get());
  //       if (jsonError)
  //       { // Couldn't parse the saved config
  //         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"SPIFFS: [ERROR] Failed to parse /config.json: ")) + String(jsonError.c_str()));
  //       }
  //       else
  //       {
  //         // if (!configJson["mqttServer"].isNull())
  //         // {
  //         //   strcpy(mqttServer, configJson["mqttServer"]);
  //         // }
  //         // if (!configJson["mqttPort"].isNull())
  //         // {
  //         //   strcpy(mqttPort, configJson["mqttPort"]);
  //         // }
  //         // if (!configJson["mqttUser"].isNull())
  //         // {
  //         //   strcpy(mqttUser, configJson["mqttUser"]);
  //         // }
  //         // if (!configJson["mqttPassword"].isNull())
  //         // {
  //         //   strcpy(mqttPassword, configJson["mqttPassword"]);
  //         // }
  //         // if (!configJson["nextionNode"].isNull())
  //         // {
  //         //   strcpy(nextionNode, configJson["nextionNode"]);
  //         // }
  //         // if (!configJson["groupName"].isNull())
  //         // {
  //         //   strcpy(groupName, configJson["groupName"]);
  //         // }
  //         // if (!configJson["configUser"].isNull())
  //         // {
  //         //   strcpy(configUser, configJson["configUser"]);
  //         // }
  //         // if (!configJson["configPassword"].isNull())
  //         // {
  //         //   strcpy(configPassword, configJson["configPassword"]);
  //         // }
  //       //   if (!configJson["motionPinConfig"].isNull())
  //       //   {
  //       //     strcpy(motionPinConfig, configJson["motionPinConfig"]);
  //       //   }
  //         if (!configJson["debugSerialEnabled"].isNull())
  //         {
  //           if (configJson["debugSerialEnabled"])
  //           {
  //             debugSerialEnabled = true;
  //           }
  //           else
  //           {
  //             debugSerialEnabled = false;
  //           }
  //         }
  //       //   if (!configJson["debugTelnetEnabled"].isNull())
  //       //   {
  //       //     if (configJson["debugTelnetEnabled"])
  //       //     {
  //       //       debugTelnetEnabled = true;
  //       //     }
  //       //     else
  //       //     {
  //       //       debugTelnetEnabled = false;
  //       //     }
  //       //   }
  //         if (!configJson["mdnsEnabled"].isNull())
  //         {
  //           if (configJson["mdnsEnabled"])
  //           {
  //             mdnsEnabled = true;
  //           }
  //           else
  //           {
  //             mdnsEnabled = false;
  //           }
  //         }
  //         String configJsonStr;
  //         serializeJson(configJson, configJsonStr);
  //         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"SPIFFS: parsed json:")) + configJsonStr);
  //       }
  //     }
  //     else
  //     {
  //       //debugPrintln(F("SPIFFS: [ERROR] Failed to read /config.json"));
  //     }
  //   }
  //   else
  //   {
  //     //debugPrintln(F("SPIFFS: [WARNING] /config.json not found, will be created on first config save"));
  //   }
  // }
  // else
  // {
  //   //debugPrintln(F("SPIFFS: [ERROR] Failed to mount FS"));
  // }
  // #endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::configSaveCallback()
{ // Callback notifying us of the need to save config
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "%s"),"SPIFFS: Configuration changed, flagging for save");
  shouldSaveConfig = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::configSave()
{ // Save the custom parameters to config.json
  nextionSetAttr("p[0].b[1].txt", "\"Saving\\rconfig\"");
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "%s"),"SPIFFS: Saving config");
  DynamicJsonDocument jsonConfigValues(1024);
  // jsonConfigValues["mqttServer"] = mqttServer;
  // jsonConfigValues["mqttPort"] = mqttPort;
  // jsonConfigValues["mqttUser"] = mqttUser;
  // jsonConfigValues["mqttPassword"] = mqttPassword;
  // jsonConfigValues["nextionNode"] = nextionNode;
  // jsonConfigValues["groupName"] = groupName;
  // jsonConfigValues["configUser"] = configUser;
  // jsonConfigValues["configPassword"] = configPassword;
  //jsonConfigValues["motionPinConfig"] = motionPinConfig;
  // jsonConfigValues["debugSerialEnabled"] = debugSerialEnabled;
  //jsonConfigValues["debugTelnetEnabled"] = debugTelnetEnabled;
  // jsonConfigValues["mdnsEnabled"] = mdnsEnabled;
  
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "%s"),"SPIFFS: Saving config");

  
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "SPIFFS: mqttServer = \"%s\""),mqttServer);

#ifdef ESP8266
  // File configFile = SPIFFS.open("/config.json", "w");
  // if (!configFile)
  // {
  //   //debugPrintln(F("SPIFFS: Failed to open config file for writing"));
  // }
  // else
  // {
  //   serializeJson(jsonConfigValues, configFile);
  //   configFile.close();
  // }
  #endif
  shouldSaveConfig = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::configClearSaved()
{ // Clear out all local storage
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"Resetting\\rsystem...\"");
  //debugPrintln(F("RESET: Formatting SPIFFS"));
  #ifdef ESP8266
  // SPIFFS.format();
  // //debugPrintln(F("RESET: Clearing WiFiManager settings..."));

  // WiFiManager wifiManager;
  // wifiManager.resetSettings();
  #endif
  EEPROM.begin(512);
  //debugPrintln(F("Clearing EEPROM..."));
  for (uint16_t i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
  nextionSetAttr("p[0].b[1].txt", "\"Rebooting\\rsystem...\"");
  //debugPrintln(F("RESET: Rebooting device"));
  espReset();
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mNextionPanel::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  root["espVersion"] = nextionVersion;
  root["updateEspAvailable"] = updateEspAvailable ? "true" : "false";
  root["updateLcdAvailable"] = updateLcdAvailable ? "true" : "false";
  root["lcdConnected"] = lcdConnected ? "true" : "false";
  root["lcdVersion"] = lcdVersion;
  root["heapFree"] = ESP.getFreeHeap();
  #ifdef ESP8266
    root["heapFragmentation"] = ESP.getHeapFragmentation();
    root["heapFree"] = ESP.getCoreVersion();
  #endif
  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);
return 1;
}

uint8_t mNextionPanel::ConstructJSON_Sensor(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  uint8_t ischanged=false;

  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  ischanged = 1;//climate.sens1.ischanged_over_threshold;

  // if(ischanged||pCONT_mqtt->fSendAllData||pCONT_mqtt->fSendSingleFunctionData){ 
  //   JsonObject sens1 = root.createNestedObject(D_JSON_SENS1);
  //     sens1[D_JSON_TEMP] = climate.sens1.temperature;
  //     sens1[D_JSON_HUM] = climate.sens1.humidity;
  //     sens1[D_JSON_PRESSURE] = climate.sens1.pressure;
  //     sens1[D_JSON_ALTITUDE] = climate.sens1.altitude;

  //   JsonObject method = root.createNestedObject(D_JSON_ISCHANGEDMETHOD);
  //     method[D_JSON_TYPE] = D_JSON_SIGNIFICANTLY; //or "any"
  //     method[D_JSON_AGE] = round(abs(millis()-climate.sens1.ischangedtLast)/1000);
  
  // }

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  return 1;

}


uint8_t mNextionPanel::ConstructJSON_EnergyStats(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  uint8_t ischanged=false;

  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  ischanged = 1;//climate.sens1.ischanged_over_threshold;

  // if(ischanged||pCONT_mqtt->fSendAllData||pCONT_mqtt->fSendSingleFunctionData){ 
  //   JsonObject sens1 = root.createNestedObject(D_JSON_SENS1);
  //     sens1[D_JSON_TEMP] = climate.sens1.temperature;
  //     sens1[D_JSON_HUM] = climate.sens1.humidity;
  //     sens1[D_JSON_PRESSURE] = climate.sens1.pressure;
  //     sens1[D_JSON_ALTITUDE] = climate.sens1.altitude;

  //   JsonObject method = root.createNestedObject(D_JSON_ISCHANGEDMETHOD);
  //     method[D_JSON_TYPE] = D_JSON_SIGNIFICANTLY; //or "any"
  //     method[D_JSON_AGE] = round(abs(millis()-climate.sens1.ischangedtLast)/1000);
  
  // }

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  return 1;

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mNextionPanel::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = false;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = false;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;
  
  mqtthandler_ptr = &mqtthandler_energystats_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = false;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_energystats;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_EnergyStats;
  
  mqtthandler_ptr = &mqtthandler_energystats_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = false;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_energystats;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_EnergyStats;

} //end "MQTTHandler_Init"


void mNextionPanel::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;
  mqtthandler_energystats_ifchanged.fSendNow = true;
  mqtthandler_energystats_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mNextionPanel::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;
  // mqtthandler_energystats_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"



void mNextionPanel::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      case MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID:   handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_ifchanged; break;
      case MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID:  handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT_mqtt->MQTTHandler_Command(*this,D_MODULE_DISPLAYS_NEXTION_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////






































/****************************
 * 
 * 
 * Serial handlers to allow different hardware/software serial types
 * 
 * 
 **************************************/





void mNextionPanel::init_serial()
{ 



}


int mNextionPanel::serial_available()
{ 

return swSer->available();


}

int mNextionPanel::serial_read()
{ 

return swSer->read();


}
void mNextionPanel::serial_print(const char* c_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

  swSer->print(c_str);
  #else
  SERIAL_NEXTION_TX.print(command_ctr);
#endif


}

void mNextionPanel::serial_print(String s_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

  swSer->print(s_str);
  #else
  SERIAL_NEXTION_TX.print(command_ctr);
#endif


}

void mNextionPanel::serial_print_suffix(){

  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
    SERIAL_NEXTION_TX.flush();
  #endif

}
void mNextionPanel::serial_print_suffixed(const char* c_str){

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

  swSer->print(c_str);
  #else
  SERIAL_NEXTION_TX.print(command_ctr);
#endif

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  #endif

}


#endif