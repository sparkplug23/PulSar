
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  //if(!lcdConnected){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:
      // EverySecond_SendScreenInfo();
      //EverySecond_FlashScreen();

       if(fOpenHABDataStreamActive_last_secs){ //only if positive
          if(fOpenHABDataStreamActive_last_secs++>OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS){
            fOpenHABDataStreamActive_last_secs = -1;
            fOpenHABDataStreamActive = false;
          }
        }
    break;
    case FUNC_EVERY_MINUTE:
      EverySecond_SendScreenInfo();
    break;
    case FUNC_EVERY_HOUR:
      Command_SetPage(settings.page);   //temp fix
    break;
    /************
     * Network SECTION * 
    *******************/
    // case FUNC_WIFI_CONNECTED:
    //   wifiConnected();
    // break;
    // case FUNC_WIFI_DISCONNECTED:
    //   wifiDisconnected();
    // break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  //return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

}





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
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d %d"),pin_tx,pin_rx);
  //   status_enabled = false;
  // }

  #ifdef USE_HARDWARE_SERIAL_TEMP
    SERIAL_NEXTION_RX.begin(NEXTION_BAUD);
  #endif


  // Check if pins are hardware serial
  // if(true){
    // swSer = new SoftwareSerial(14,12);//D5,D6);//R-T, false, 256);
    // swSer->begin(NEXTION_BAUD);
  // }
  
  Serial.println("DONE::pre_init");
  Serial.flush();
}


void mNextionPanel::init()
{ 
  
  // init variables
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));
  memcpy(lcdVersionQuery,"p[0].b[2].val",sizeof("p[0].b[2].val"));

// nextionSendCmd("page 4");

  // needs non blocking
  while(!lcdConnected && (millis() < 5000)){ // Wait up to 5 seconds for serial input from LCD
    nextionHandleInput();
  }

  // Serial.println("mNextionPanel::nextionHandleInput done");
  // Serial.flush();
  // delay(5000);
  if(lcdConnected){
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD responding, continuing program load"));
    nextionSendCmd("connect");
  }
  else{
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD not responding, continuing program load"));
  }

  // Configure some items
  //nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\""); // QR code show wifi connect


  memset(flash_message.message,0,sizeof(flash_message.message));
  sprintf(flash_message.message,"%s","Awaitng a message to show");

  char page_default_command[10];
  sprintf(page_default_command,"page %d",NEXTION_DEFAULT_PAGE_NUMBER);

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;

  //nextionSendCmd(page_default_command); //set page 1 as default 
  //   //nextionSendCmd("page 1"); //set page 1 as default 

  Command_SetPage(settings.page);

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
    if(mTime::TimeReachedNonReset(&screen_press.tSavedButtonONEvent,LONG_PRESS_DURATION)){
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "LONG_PRESS_DURATION reached"));
      screen_press.fEnableImmediateButtonTime=false;
      fEnableIgnoreNextOffEvent = true;
      MQTTSend_LongPressEvent();
      MQTTSend_PressEvent();
    }
  }

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

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));

  // Show connection success
  //nextionSendCmd("page 0");

  // char display_ctr[30];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");

  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

  // hide QR code
  // nextionSendCmd("vis 3,0");

}


void mNextionPanel::mqttDisconnected(){
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
  // char display_ctr[120];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connect to%s\\rFAILED rc=%s\"",WiFi.localIP().toString(),"192.168.1.65",pCONT_mqtt->pubsub->stateCtr());

  // QR code show wifi connect
  // nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\"");
  // // show item
  // nextionSendCmd("vis 3,1");
  
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

}


void mNextionPanel::MQTTSend_PressEvent(){

  if(!mTime::TimeReached(&tSaved_MQTTSend_PressEvent,200)){
    // Debounce and only send once per event (ie ignore release trigger following immediate trigger)
    return;
  }
  
  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];
  uint32_t tSavedTimeSincePressOn = abs(millis() - screen_press.tSavedButtonONEvent);

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");
  
  JsonBuilderI->Start();

    char event_ctr[20]; memset(event_ctr,0,sizeof(event_ctr));
    sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
    JsonBuilderI->Add("event", event_ctr);
    
    JsonBuilderI->Add("value", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JsonBuilderI->Add("duration", tSavedTimeSincePressOn);
  JsonBuilderI->End();


  tSaved_MQTTSend_PressEvent = millis();

  pCONT_mqtt->ppublish("status/nextion/event/press",JsonBuilderI->GetBufferPtr(),false);

}

void mNextionPanel::MQTTSend_LongPressEvent(){

  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");

  char event_ctr[20];
  memset(event_ctr,0,sizeof(event_ctr));
  sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

  JsonBuilderI->Start();
    JsonBuilderI->Add("event", event_ctr);
    JsonBuilderI->Add("value", "LONG_PRESS");
    JsonBuilderI->Add("duration_threshold", LONG_PRESS_DURATION);
  JsonBuilderI->End();

  pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
  pCONT_mqtt->ppublish("status/nextion/event/start",JsonBuilderI->GetBufferPtr(),0);

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mNextionPanel::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
  JsonBuilderI->End();

}

uint8_t mNextionPanel::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
  JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mNextionPanel::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mNextionPanel::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;
  mqtthandler_energystats_ifchanged.flags.SendNow = true;
  mqtthandler_energystats_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mNextionPanel::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;
  // mqtthandler_energystats_teleperiod.tRateSecs = mcl->mset->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mNextionPanel::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mNextionPanel>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_DISPLAYS_NEXTION_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}


////////////////////// END OF MQTT /////////////////////////

#endif