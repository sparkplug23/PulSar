/**
 * @file mNextionPanel.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note Initially based on oneHASP but should become its own code in due time
 *       Latest code revision with oneHASP is version 1.03 on 9Aug22

 * @desc To enable control of Nextion panels using mqtt, allowing pass through commands and improved automation (eg dimmer, regexp of colours)
 */
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

const char* mNextionPanel::PM_MODULE_DISPLAYS_NEXTION_CTR = D_MODULE_DISPLAYS_NEXTION_CTR;
const char* mNextionPanel::PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR = D_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR;

int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj){

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

  // if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();

      #ifdef ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
      if(webServer)
      {
        webServer->handleClient();
      }
      #endif // ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

    break;
    case FUNC_EVERY_SECOND:
      // EverySecond_SendScreenInfo();
      // EverySecond_FlashScreen();
      EverySecond_ActivityCheck();
    break;
    case FUNC_EVERY_MINUTE:
      // EverySecond_SendScreenInfo();
    break;
    case FUNC_EVERY_HOUR:
      // Command_SetPage(settings.page);   //temp fix
    break;
    /************
     * Network SECTION * 
    *******************/
    case FUNC_WIFI_CONNECTED:
      // wifiConnected();
      // Show_ConnectionWorking();


      
      #ifdef ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
      webServer = new WebServer(80);

      webServer->on("/", [this](void){ webHandleRoot(); });


      webServer->on("/nextion_update", [this](void){ WebPage_LCD_Update_TFT(); } );


      webServer->on(
          "/lcdupload", HTTP_POST, 
          [this](){ pCONT_nex->webServer->send(200); },          
          [this](){ webHandleLcdUpload(); }
      );
          

      webServer->on("/tftFileSize", [this](void){ webHandleTftFileSize(); } );
      webServer->on("/lcddownload", [this](void){ webHandleLcdDownload(); } );
      webServer->on("/lcdOtaSuccess", [this](void){ webHandleLcdUpdateSuccess(); } );
      webServer->on("/lcdOtaFailure", [this](void){ webHandleLcdUpdateFailure(); } );
      webServer->on("/reboot", [this](void){ webHandleReboot(); } );
      webServer->onNotFound( [this](void){ webHandleNotFound(); } );

      /**
       * @brief  Probably don't need
       * */
      webServer->on("/saveConfig", [this](void){ webHandleSaveConfig(); } );
      webServer->on("/resetConfig", [this](void){ webHandleResetConfig(); } );
      webServer->on("/resetBacklight", [this](void){ webHandleResetBacklight(); } );
      webServer->on("/firmware", [this](void){ webHandleFirmware(); } );
      webServer->on("/espfirmware", [this](void){ webHandleEspFirmware(); } );
      
      webServer->begin();

      debugPrintln(String(F("HTTP: Server started @ http://")) + WiFi.localIP().toString());
      
      #endif // ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT



    break;
    case FUNC_WIFI_DISCONNECTED:
      // wifiDisconnected();
      Show_ConnectionNotWorking();
    break;
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
      MQTTHandler_Init(); 
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      mqttConnected();
      Show_ConnectionWorking();
    break;
    case FUNC_MQTT_DISCONNECTED:
      mqttDisconnected();
      Show_ConnectionNotWorking();
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





/**
 * @brief Using a template for "working" and another for "not working"
 * hardcoding for now
 **/
void mNextionPanel::Show_ConnectionWorking()
{

// char* command = "{}";

#ifdef ENABLE_DEVFEATURE_NEXTION_DISPLAY

  nextionSendCmd("page 2");
  nextionSetAttr("p[2].b[1].txt", "\"Computer connected!\"");
  delay(1000);
  nextionSendCmd("page 1");
  nextionSetAttr("p[1].b[1].txt", "\"Computer connected!\"");

#endif

}


void mNextionPanel::Show_ConnectionNotWorking()
{

  nextionSendCmd("page message");
  nextionSetAttr("message.main.txt", "\"Show_ConnectionNotWorking\"");
  nextionSetAttr("message.main.bco", "\"#ff0000\"");
  
}



void mNextionPanel::Pre_Init(void){
 
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
  
  
}


/**
 * @brief 
 * Create a init command for the nextion panels, that can be stored in progmem
 * 
 */



void mNextionPanel::Init()
{ 

  SERIAL_NEXTION_TX.begin(115200);

  if (!nextionConnect())
  {
    if (lcdConnected)
    {
      debugPrintln(F("HMI: LCD responding but initialization wasn't completed. Continuing program load anyway."));
    }
    else
    {
      debugPrintln(F("HMI: LCD not responding, continuing program load"));
    }
  }

  // if (mdnsEnabled)
  // { // Setup mDNS service discovery if enabled
  //   hMDNSService = MDNS.addService(haspNode, "http", "tcp", 80);
  //   if (debugTelnetEnabled)
  //   {
  //     MDNS.addService(haspNode, "telnet", "tcp", 23);
  //   }
  //   MDNS.addServiceTxt(hMDNSService, "app_name", "HASwitchPlate");
  //   MDNS.addServiceTxt(hMDNSService, "app_version", String(haspVersion).c_str());
  //   MDNS.update();
  // }

  
  // init variables
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));
  // memcpy(lcdVersionQuery,"p[0].b[2].val",sizeof("p[0].b[2].val"));

  memset(flash_message.message,0,sizeof(flash_message.message));
  sprintf(flash_message.message,"%s","Awaitng a message to show");

  char page_default_command[10];
  sprintf(page_default_command,"page %d",NEXTION_DEFAULT_PAGE_NUMBER);


  #ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE
    Template_Load_Init_Display_Command();
  #endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE

  /**
   * @brief 
   * Boot message
   * 
   */
  #ifdef TEMPLATE_DEFINED_BOOT_MESSAGE
  // Command_SetPage(settings.page);




  #endif // TEMPLATE_DEFINED_BOOT_MESSAGE



  /**
   * @brief 
   * Runtime page
   * 
   */

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;

  settings.flags.EnableModule = true;

  //nextionSendCmd(page_default_command); //set page 1 as default 
  //   //nextionSendCmd("page 1"); //set page 1 as default 

  Command_SetPage(settings.page);

  


}


#ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE

void mNextionPanel::Template_Load_Init_Display_Command(){

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG, PSTR("mInterfaceLight::Template_Load()"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  // AddLog(LOG_LEVEL_TEST, PSTR("mInterfaceLight::Template_Load SettingsMerge\n\r\n\r\n\r\n\r"
  //     //Test data
  //     "%d:%d:%d:%d"
  //   ),
  //     //Testdata
  //      pCONT_set->Settings.animation_settings.xmas_controller_params[0]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[1]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[2]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[3]
  // );

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR("LIGHTING_TEMPLATE Load"));// " READ = \"%s\""), data_buffer.payload.ctr);
  ALOG_DBM( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  #endif // USE_LIGHTING_TEMPLATE

}

#endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE


void mNextionPanel::EverySecond_ActivityCheck()
{

  //  if(fOpenHABDataStreamActive_last_secs){ //only if positive
  //     if(fOpenHABDataStreamActive_last_secs++>OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS){
  //       fOpenHABDataStreamActive_last_secs = -1;
  //       fOpenHABDataStreamActive = false;
  //     }
  //   }

  if(settings.timeout_check.timeout_period==1)
  {
    ALOG_INF(PSTR("No data"));
    Show_ConnectionNotWorking();
    settings.page = -1; // unset to force update when available
  }
  else
  if(settings.timeout_check.timeout_period > 1)
  {
    settings.timeout_check.timeout_period--;
  }
  else
  {
    settings.timeout_check.timeout_period = 0; //stop
  }
    

}



void mNextionPanel::EveryLoop()
{

  // while ((WiFi.status() != WL_CONNECTED) || (WiFi.localIP().toString() == "0.0.0.0"))
  // { // Check WiFi is connected and that we have a valid IP, retry until we do.
  //   if (WiFi.status() == WL_CONNECTED)
  //   { // If we're currently connected, disconnect so we can try again
  //     WiFi.disconnect();
  //   }
  //   espWifiReconnect();
  // }

  // if (!mqttClient.connected())
  // { // Check MQTT connection
  //   debugPrintln(String(F("MQTT: not connected, connecting.")));
  //   mqttConnect();
  // }
  nextionHandleInput();     // Nextion serial communications loop
  // mqttClient.loop();        // MQTT client loop
  // ArduinoOTA.handle();      // Arduino OTA loop
  // webServer->handleClient(); // webServer loop
  // telnetHandleClient();     // telnet client loop
  // motionHandle();           // motion sensor loop
  // beepHandle();             // beep feedback loop

  // if (mdnsEnabled)
  // {
  //   MDNS.update();
  // }

  if ((millis() - statusUpdateTimer) >= statusUpdateInterval)
  { // Run periodic status update
    statusUpdateTimer = millis();
    mqttStatusUpdate();
  }

  if (((millis() - updateCheckTimer) >= updateCheckInterval) && (millis() > updateCheckFirstRun))
  { // Run periodic update check
    updateCheckTimer = millis();
    if (updateCheck())
    { // Publish new status if updateCheck() worked and reset the timer
      statusUpdateTimer = millis();
      mqttStatusUpdate();
    }
  }


  // // nextionHandleInput();     // Nextion serial communications loop
  // if (nextionHandleInput()){ // Process user input from HMI
  //   nextionProcessInput();
  // }

  // // if ((lcdVersion < 1) && (millis() <= (nextionRetryMax * nextionCheckInterval)))
  // // { // Attempt to connect to LCD panel to collect model and version info during startup
  // //   nextionConnect();
  // //   // ALOG_ERR(PSTR("Invalid response, disabling nextionConnect for now"));
  // // }
    
  // // if ((millis() > (nextionRetryMax * nextionCheckInterval)) && !startupCompleteFlag)
  // // { // We still don't have LCD info so go ahead and run the rest of the checks once at startup anyway
  // //   updateCheck();
  // //   startupCompleteFlag = true;
  // // }



  // if ((millis() - statusUpdateTimer) >= statusUpdateInterval)
  // { // Run periodic status update
  //   statusUpdateTimer = millis();
  //   mqttStatusUpdate();
  // }

  // if (((millis() - updateCheckTimer) >= updateCheckInterval) && (millis() > updateCheckFirstRun))
  // { // Run periodic update check
  //   updateCheckTimer = millis();
  //   if (updateCheck())
  //   { // Publish new status if updateCheck() worked and reset the timer
  //     statusUpdateTimer = millis();
  //     mqttStatusUpdate();
  //   }
  // }





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


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::mqttStatusUpdate()
{ // Periodically publish system status
  String mqttSensorPayload = "{";
  mqttSensorPayload += String(F("\"espVersion\":")) + String(haspVersion) + String(F(","));
  if (updateEspAvailable)
  {
    mqttSensorPayload += String(F("\"updateEspAvailable\":true,"));
  }
  else
  {
    mqttSensorPayload += String(F("\"updateEspAvailable\":false,"));
  }
  if (lcdConnected)
  {
    mqttSensorPayload += String(F("\"lcdConnected\":true,"));
  }
  else
  {
    mqttSensorPayload += String(F("\"lcdConnected\":false,"));
  }
  mqttSensorPayload += String(F("\"lcdVersion\":\"")) + String(lcdVersion) + String(F("\","));
  if (updateLcdAvailable)
  {
    mqttSensorPayload += String(F("\"updateLcdAvailable\":true,"));
  }
  else
  {
    mqttSensorPayload += String(F("\"updateLcdAvailable\":false,"));
  }
  mqttSensorPayload += String(F("\"espUptime\":")) + String(long(millis() / 1000)) + String(F(","));
  mqttSensorPayload += String(F("\"signalStrength\":")) + String(WiFi.RSSI()) + String(F(","));
  mqttSensorPayload += String(F("\"haspName\":\"")) + String(haspNode) + String(F("\","));
  mqttSensorPayload += String(F("\"haspIP\":\"")) + WiFi.localIP().toString() + String(F("\","));
  mqttSensorPayload += String(F("\"haspClientID\":\"")) + mqttClientId + String(F("\","));
  mqttSensorPayload += String(F("\"haspMac\":\"")) + String(espMac[0], HEX) + String(F(":")) + String(espMac[1], HEX) + String(F(":")) + String(espMac[2], HEX) + String(F(":")) + String(espMac[3], HEX) + String(F(":")) + String(espMac[4], HEX) + String(F(":")) + String(espMac[5], HEX) + String(F("\","));
  mqttSensorPayload += String(F("\"haspManufacturer\":\"HASwitchPlate\",\"haspModel\":\"HASPone v1.0.0\","));
  mqttSensorPayload += String(F("\"heapFree\":")) + String(ESP.getFreeHeap()) + String(F(","));
  // mqttSensorPayload += String(F("\"heapFragmentation\":")) + String(ESP.getHeapFragmentation()) + String(F(","));
  // mqttSensorPayload += String(F("\"heapMaxFreeBlockSize\":")) + String(ESP.getMaxFreeBlockSize()) + String(F(","));
  // mqttSensorPayload += String(F("\"espCore\":\"")) + String(ESP.getCoreVersion()) + String(F("\""));
  mqttSensorPayload += "}";

  // Publish sensor JSON
  // mqttClient.publish(mqttSensorTopic, mqttSensorPayload, true, 1);
  debugPrintln(String(F("MQTT OUT: '")) + mqttSensorTopic + String(F("' : '")) + mqttSensorPayload + String(F("'")));
}


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


/**
 * @brief 
 * 
 * 
 
 {
  "commands": [
    "page 5",
    "p[5].b[7].txt=\"Heating is working\"",
    "p[5].b[7].pco=2047",
    "p[5].b[7].bco=0",
    "p[5].b[11].txt=\"12.3\"",
    "p[5].b[11].pco=65500",
    "p[5].b[11].bco=0"
  ]
}

{
  "commands": [
    "page 1",
    "p[9].b[1].txt=\"hello\"",
    "p[9].b[1].bco=200"
  ]
}

{
  "commands": [
    "page 1",
    "p0.pic=1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=2047",
    "p[1].b[1].bco=0",
    "p[1].b[7].txt=\"12.3\"",
    "p[1].b[7].pco=65500",
    "p[1].b[7].bco=0",
    "p[1].b[8].txt=\"12.3\"",
    "p[1].b[8].pco=65500",
    "p[1].b[8].bco=0",
    "p[1].b[9].txt=\"12.3\"",
    "p[1].b[9].pco=65500",
    "p[1].b[9].bco=0",
    "p[1].b[10].txt=\"12.3\"",
    "p[1].b[10].pco=65500",
    "p[1].b[10].bco=0",
    "p[1].b[11].txt=\"12.3\"",
    "p[1].b[11].pco=65500",
    "p[1].b[11].bco=0",
    "p[1].b[12].txt=\"12.3\"",
    "p[1].b[12].pco=65500",
    "p[1].b[12].bco=0",
    "p[1].b[13].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0",
    "p[1].b[14].txt=\"12.3\"",
    "p[1].b[14].pco=65500",
    "p[1].b[14].bco=0",
    "p[1].b[2].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0"
  ]
}

developing conversion of colours

{
  "commands": [
    "page 1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=h120",
    "p[1].b[1].bco=0"
  ]
}




 * 
 */


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::debugPrintln(const String &debugText)
{ // Debug output line of text to our debug targets
  const String debugTimeText = "[+" + String(float(millis()) / 1000, 3) + "s] ";
  if (debugSerialEnabled)
  {
    Serial.print(debugTimeText);
    Serial.println(debugText);
    // SoftwareSerial debugSerial(-1, 1); // -1==nc for RX, 1==TX pin
    // debugSerial.begin(debugSerialBaud);
    // debugSerial.print(debugTimeText);
    // debugSerial.println(debugText);
    // debugSerial.flush();
  }
  // if (debugTelnetEnabled)
  // {
  //   if (telnetClient.connected())
  //   {
  //     telnetClient.print(debugTimeText);
  //     telnetClient.println(debugText);
  //   }
  // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::debugPrint(const String &debugText)
{ // Debug output a string to our debug targets.
  // Try to avoid using this function if at all possible.  When connected to telnet, printing each
  // character requires a full TCP round-trip + acknowledgement back and execution halts while this
  // happens.  Far better to put everything into a line and send it all out in one packet using
  // debugPrintln.
  if (debugSerialEnabled)
    Serial.print(debugText);
  {
    // SoftwareSerial debugSerial(-1, 1); // -1==nc for RX, 1==TX pin
    // debugSerial.begin(debugSerialBaud);
    // debugSerial.print(debugText);
    // debugSerial.flush();
  }
  // if (debugTelnetEnabled)
  // {
  //   if (telnetClient.connected())
  //   {
  //     telnetClient.print(debugText);
  //   }
  // }
}


  
/******************************************************************************************************************
 * SECTION: Commands
*******************************************************************************************************************/

  

//https://haswitchplate.github.io/openHASP-docs/0.6.2/design/objects/#common-properties

// int8_t mNextionPanel::CheckAndExecute_JSONCommands(){

//   // Check if instruction is for me
//   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_LIGHTS_INTERFACE_CTR)>=0){
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_LIGHTS_INTERFACE_CTR));
//     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }

void mNextionPanel::parse_JSONCommand(JsonParserObject obj){

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEXTION D_TOPIC "Checking all commands"));
//   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

//   char buffer[50];

//   // Need to parse on a copy
//   char parsing_buffer[data_buffer.payload.len+1];
//   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);


// // sprintf(parsing_buffer, "{[\"three=\\\"THREE\\\"\", \"two=\\\"TWO\\\"\"]}");

// // Serial.println(parsing_buffer);

//   JsonParser parser(parsing_buffer);
//   JsonParserObject obj = parser.getRootObject();   
//   if (!obj) { 
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
//     #endif //ENABLE_LOG_LEVEL_COMMANDS
//     return;
//   }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  

//   if(jtok = obj[PM_JSON_COLOUR_PALETTE]){
//     if(jtok.isStr()){
//       if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
//         CommandSet_PaletteID(tmp_id);
//         data_buffer.isserviced++;
//       }
//     }else
//     if(jtok.isNum()){
//       CommandSet_PaletteID(jtok.getInt());
//       data_buffer.isserviced++;
//     }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }


// int8_t mNextionPanel::parsesub_SetMulti(JsonObjectConst obj){

//   AddLog(LOG_LEVEL_INFO, PSTR("F::%s"),__FUNCTION__);

    // if(jtok = obj["commands"]){

    //     JsonParserArray array = jtok;

    //     Serial.println(array.size());

    //     for(auto val : array) {
    //         // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());

    //         // if(val.isStr()){
    //             Serial.println(val.getStr());
    //         // }else{
    //         //     Serial.println("NOT NOT val.isStr()");
    //         // }

    //         // nextionSendCmd(val.getStr());
    //     }

    // }
  
    if(jtok = obj["Display"].getObject()["SetPageIfChanged"])
    {
      // if(jtok.isStr())
      // {
      //   Command_SetPageIfChanged(jtok.getInt());
      // }
      // else
      // if(jtok.isInt())
      // {
        Command_SetPageIfChanged(jtok.getInt());
      // }
    }

  
    if(jtok = obj["commands"]){
        JsonParserArray array = jtok;
        for(auto val : array) {
            AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());
            nextionSendCmd(val.getStr());
        }
    }
  
/**
 * @brief Construct a new if object
 * I need to add check, if failed (ie due to wrong screen) then stop attempting to update the rest of the messages to stop overflow of UART
 * 
 */
    if(jtok = obj["commands_formatted"]){
        JsonParserArray array = jtok;
        for(auto val : array) {
            ALOG_DBM( PSTR("F::%s %s"),__FUNCTION__,val.getStr());
            // need to add bool for bad response
            nextionSendCmd_ContainingFormattedText(val.getStr());
            data_buffer.isserviced++;
        }
        settings.timeout_check.timeout_period = 120; //make fucntion to reset it
    }


    
    if(jtok = obj["Display"].getObject()["Println"]){
        // JsonParserArray array = jtok;
        // for(auto val : array) {
        //     // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());
        //     nextionSendCmd(val.getStr());
        // }
        Serial2.println(jtok.getStr());
    }




    
//     if(jtok = obj["commands3"]){
//         //temp fix
//         // JsonParserArray array = jtok;

//         // Serial.println(array.size());

//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
// DynamicJsonDocument doc(1000);

//  // Deserialize the JSON document
//   DeserializationError error = deserializeJson(doc, JsonBuilderI->GetBufferPtr());

//   // Test if parsing succeeds.
//   if (error) {
//     Serial.print(F("deserializeJson() failed: "));
//     Serial.println(error.f_str());
//     return;
//   }

// JsonObject obj = doc.as<JsonObject>();

// JsonArray arr = obj["commands3"];//.as<JsonArray>();

// // using C++11 syntax (preferred):
// for (JsonVariant value : arr) {
//     Serial.println(value.as<char*>());
// }
  

// //   JsonObjectArray


//         // for(auto val : array) {
//         //     // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());

//         //     // if(val.isStr()){
//         //         Serial.println(val.getStr());
//         //     // }else{
//         //     //     Serial.println("NOT NOT val.isStr()");
//         //     // }

//         //     // nextionSendCmd(val.getStr());
//         // }

//     }



    // jsmntok_t *token_next = &


// }








//   uint8_t name_num=-1,state=-1;    


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// // Incoming Namespace (replace /device/ with /group/ for group commands)
// // '[...]/device/command' -m 'dim=50' = nextionSendCmd("dim=50")
// // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
// // '[...]/device/command/page' -m '1' = nextionSendCmd("page 1")
// // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' = nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
// // '[...]/device/command/lcdupdate' -m '' = nextionStartOtaDownload("lcdFirmwareUrl")
// // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' = espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
// // '[...]/device/command/espupdate' -m '' = espStartOta("espFirmwareUrl")
// // '[...]/device/command/p[1].b[4].txt' -m '' = nextionGetAttr("p[1].b[4].txt")
// // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' = nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
// // uint8_t mNextionPanel::parse_JSONCommand(){

//   // Check if instruction is for me
//   // if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/nextion")>=0){
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
//   //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   //   fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
//   //   fOpenHABDataStreamActive = true;
//   // }else{
//   //   return 0; // not meant for here
//   // }



  
// //these need to be subtopic functions



//   // if(strstr(data_buffer.topic.ctr,"/commands")){ 
//   // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/commands",sizeof("/commands")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
//     #endif
//     isserviced += parsesub_Commands(obj);
//   }else 
//    // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
//     #endif
//     isserviced += parsesub_SetMulti(obj);
//   }else{
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
//   }  
  
  
//   // if(strstr(data_buffer.topic.ctr,"/set_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if((!obj["attribute"].isNull())&&(!obj["value"].isNull())){ 
//   //     const char* attribute = obj["attribute"];
//   //     const char* value = obj["value"];
//   //     nextionSetAttr(attribute,value);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/get_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if(!obj["attribute"].isNull()){ 
//   //     const char* attribute = obj["attribute"];
//   //     nextionGetAttr(attribute);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/nextion/flash_message")){
//   //   isserviced += parsesub_FlashMessage();
//   // }

//   // else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
//   // { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' 
//   //== nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//   //   if (strPayload == ""){
//   //     nextionStartOtaDownload(lcdFirmwareUrl);
//   //   }else{
//   //     nextionStartOtaDownload(strPayload);
//   //   }
//   // }
//   // else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
//   // { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' 
//   // == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//   //   if (strPayload == ""){
//   //     espStartOta(espFirmwareUrl);
//   //   }else{
//   //     espStartOta(strPayload);
//   //   }
//   // }

//   data_buffer.isserviced += isserviced;
 
// //  





}



std::string& replace(std::string& s, const std::string& from, const std::string& to)
{
    if(!from.empty())
        for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}




// // int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj), JsonObjectConst obj){
// //   switch(function){
// //     case FUNC_JSON_COMMAND_OBJECT:
// //       parse_JSONCommand(obj);
// //     break;
// //     case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
// //       return CheckAndExecute_JSONCommands(obj);
// //     break;
// //   }
// // }


// // int8_t mNextionPanel::CheckAndExecute_JSONCommands(JsonObjectConst obj){

// //   // Check if instruction is for me
// //   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/nextion")>=0){
// //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //       parsesub_TopicCheck_JSONCommand(obj);
// //       return FUNCTION_RESULT_HANDLED_ID;
// //   }else{
// //     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
// //   }

// // }

// int8_t mNextionPanel::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

//   AddLog(LOG_LEVEL_INFO,PSTR("mDoorBell::parsesub_TopicCheck_JSONCommand"));

//   uint8_t name_num=-1,state=-1;    


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// // Incoming Namespace (replace /device/ with /group/ for group commands)
// // '[...]/device/command' -m 'dim=50' = nextionSendCmd("dim=50")
// // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
// // '[...]/device/command/page' -m '1' = nextionSendCmd("page 1")
// // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' = nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
// // '[...]/device/command/lcdupdate' -m '' = nextionStartOtaDownload("lcdFirmwareUrl")
// // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' = espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
// // '[...]/device/command/espupdate' -m '' = espStartOta("espFirmwareUrl")
// // '[...]/device/command/p[1].b[4].txt' -m '' = nextionGetAttr("p[1].b[4].txt")
// // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' = nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
// // uint8_t mNextionPanel::parse_JSONCommand(){

//   // Check if instruction is for me
//   // if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/nextion")>=0){
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
//   //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   //   fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
//   //   fOpenHABDataStreamActive = true;
//   // }else{
//   //   return 0; // not meant for here
//   // }



  
// //these need to be subtopic functions



//   // if(strstr(data_buffer.topic.ctr,"/commands")){ 
//   // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/commands",sizeof("/commands")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
//     #endif
//     isserviced += parsesub_Commands(obj);
//   }else 
//    // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
//     #endif
//     isserviced += parsesub_SetMulti(obj);
//   }else{
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
//   }  
  
  
//   // if(strstr(data_buffer.topic.ctr,"/set_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if((!obj["attribute"].isNull())&&(!obj["value"].isNull())){ 
//   //     const char* attribute = obj["attribute"];
//   //     const char* value = obj["value"];
//   //     nextionSetAttr(attribute,value);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/get_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if(!obj["attribute"].isNull()){ 
//   //     const char* attribute = obj["attribute"];
//   //     nextionGetAttr(attribute);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/nextion/flash_message")){
//   //   isserviced += parsesub_FlashMessage();
//   // }

//   // else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
//   // { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' 
//   //== nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//   //   if (strPayload == ""){
//   //     nextionStartOtaDownload(lcdFirmwareUrl);
//   //   }else{
//   //     nextionStartOtaDownload(strPayload);
//   //   }
//   // }
//   // else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
//   // { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' 
//   // == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//   //   if (strPayload == ""){
//   //     espStartOta(espFirmwareUrl);
//   //   }else{
//   //     espStartOta(strPayload);
//   //   }
//   // }

//   data_buffer.isserviced += isserviced;
 
// //  



// }




// int8_t mNextionPanel::parsesub_Commands(JsonObjectConst obj){

//   char command_ctr[100]; memset(command_ctr,0,sizeof(command_ctr));
  
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_TOPIC "/commands"));    

//     if(!obj["page"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "page"));    
//       settings.page = obj["page"];
//       sprintf(command_ctr,"page %d",settings.page);
//       nextionSendCmd(command_ctr);
//     }else
//     if(!obj["command"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "command"));    
//       const char* command = obj["command"];
//       nextionSendCmd(command);
//     }else
//     if(!obj["statusupdate"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "statusupdate"));    
//       uint8_t statusupdate = obj["statusupdate"];
//       sprintf(command_ctr,"statusupdate %d",statusupdate);
//       mqtthandler_settings_teleperiod.flags.SendNow = true;
//     }else
//     if(!obj["lcdreboot"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));    
//       uint8_t lcdreboot = obj["lcdreboot"];
//       nextionReset();
//     }else
//     if(!obj["onoff"].isNull()){ 
//       const char* onoff = obj["onoff"];
//       if(strstr(onoff,"ON")){
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"onoff",D_ON);    
//         nextionSendCmd("dim=dims");
//       }else
//       if(strstr(onoff,"OFF")){
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));     
//         nextionSendCmd("dims=dim");
//         nextionSetAttr("dim", "0");
//       }
//     }else{
//       AddLog(LOG_LEVEL_INFO,PSTR("ELSE FOUND"));
//     }

// }





// int8_t mNextionPanel::parsesub_FlashMessage(){

//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "%s"),"parsesub_FlashMessage");

//   // #ifdef JSONDOCUMENT_STATIC
//   //   StaticJsonDocument<800> doc;
//   // #else
//   //   DynamicJsonDocument doc(600);
//   // #endif
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   // if(error){
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
//   //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
//   //   return 0;
//   // }
//   // JsonObject obj = doc.as<JsonObject>();

//   // int8_t tmp_id = 0;
  

//   // if(!obj["message"].isNull()){ 
//   //   const char* messagectr = obj["message"];
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"message",messagectr);
//   //   sprintf(flash_message.message,"%s",messagectr);
//   // }

//   // if(!obj["time_secs"].isNull()){ 
//   //   uint8_t time = obj["time_secs"];
//   //   flash_message.cShowSeconds = time>60?60:time;
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"cShowSeconds",flash_message.cShowSeconds);
//   // }

//   // if(!obj["background_colour"].isNull()){ 
//   //   uint32_t background_colour = obj["background_colour"];
//   //   flash_message.background_colour = background_colour;
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"background_colour",background_colour);
//   // }

//   // if(!obj["font_colour"].isNull()){ 
//   //   uint32_t font_colour = obj["font_colour"];
//   //   flash_message.font_colour = font_colour;
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"font_colour",font_colour);
//   // }
  


//   // flash_message.page = PAGE_ERROR_ID;

//   // //Save page
//   // settings.page_saved = settings.page;
//   // //Go to message page
//   // Command_SetPage(flash_message.page);

//   // flash_message.cShowSeconds = 3;

  
//   // // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
//   // // // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
//   // // sprintf(display_ctr,"%s",flash_message.message);
        
//   //   SetAttribute_FontColour(flash_message.page,1,flash_message.font_colour);
//   //   SetAttribute_BackgroundColour(flash_message.page,1,flash_message.background_colour);
//   //   SetAttribute_Txt(flash_message.page,1,flash_message.message);




   
//   // flash_message.isrunning = true;  

//   return 0;
// }


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void mqttProcessInput(String &strTopic, String &strPayload)
// { // Handle incoming commands from MQTT

//   // strTopic: homeassistant/haswitchplate/devicename/command/p[1].b[4].txt
//   // strPayload: "Lights On"
//   // subTopic: p[1].b[4].txt

//   // Incoming Namespace (replace /device/ with /group/ for group commands)
//   // '[...]/device/command' -m '' == No command requested, respond with mqttStatusUpdate()
//   // '[...]/device/command' -m 'dim=50' == nextionSendCmd("dim=50")
//   // '[...]/device/command/json' -m '["dim=5", "page 1"]' == nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   // '[...]/device/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//   // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//   // '[...]/device/brightness/set' -m '50' == nextionSendCmd("dims=50")
//   // '[...]/device/light/switch' -m 'OFF' == nextionSendCmd("dims=0")
//   // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//   // '[...]/device/command/statusupdate' -m '' == mqttStatusUpdate()
//   // '[...]/device/command/discovery' -m '' == call mqttDiscovery()
//   // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' == nextionOtaStartDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//   // '[...]/device/command/lcdupdate' -m '' == nextionOtaStartDownload("lcdFirmwareUrl")
//   // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//   // '[...]/device/command/espupdate' -m '' == espStartOta("espFirmwareUrl")
//   // '[...]/device/command/beep' -m '100,200,3' == beep on for 100msec, off for 200msec, repeat 3 times
//   // '[...]/device/command/hassdiscovery' -m 'homeassistant' == hassDiscovery = homeassistant
//   // '[...]/device/command/nextionmaxpages' -m '11' == nextionmaxpages = 11
//   // '[...]/device/command/nextionbaud' -m '921600' == nextionBaud = 921600
//   // '[...]/device/command/debugserialenabled' -m 'true' == enable serial debug output
//   // '[...]/device/command/debugtelnetenabled' -m 'true' == enable telnet debug output
//   // '[...]/device/command/mdnsenabled' -m 'true' == enable mDNS responder
//   // '[...]/device/command/beepenabled' -m 'true' == enable beep output on keypress
//   // '[...]/device/command/ignoretouchwhenoff' -m 'true' == disable actions on keypress

//   debugPrintln(String(F("MQTT IN: '")) + strTopic + String(F("' : '")) + strPayload + String(F("'")));

//   if (((strTopic == mqttCommandTopic) || (strTopic == mqttGroupCommandTopic)) && (strPayload == ""))
//   {                     // '[...]/device/command' -m '' = No command requested, respond with mqttStatusUpdate()
//     mqttStatusUpdate(); // return status JSON via MQTT
//   }
//   else if (strTopic == mqttCommandTopic || strTopic == mqttGroupCommandTopic)
//   { // '[...]/device/command' -m 'dim=50' == nextionSendCmd("dim=50")
//     nextionSendCmd(strPayload);
//   }
//   else if (strTopic == (mqttCommandTopic + "/page") || strTopic == (mqttGroupCommandTopic + "/page"))
//   { // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//     if (strPayload == "")
//     {
//       nextionSendCmd("sendme");
//     }
//     else
//     {
//       nextionActivePage = strPayload.toInt();
//       nextionSendCmd("page " + strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/json") || strTopic == (mqttGroupCommandTopic + "/json"))
//   { // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//     if (strPayload != "")
//     {
//       nextionParseJson(strPayload); // Send to nextionParseJson()
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/statusupdate") || strTopic == (mqttGroupCommandTopic + "/statusupdate"))
//   {                     // '[...]/device/command/statusupdate' == mqttStatusUpdate()
//     mqttStatusUpdate(); // return status JSON via MQTT
//   }
//   else if (strTopic == (mqttCommandTopic + "/discovery") || strTopic == (mqttGroupCommandTopic + "/discovery"))
//   {                  // '[...]/device/command/discovery' == mqttDiscovery()
//     mqttDiscovery(); // send Home Assistant discovery message via MQTT
//   }
//   else if (strTopic == (mqttCommandTopic + "/hassdiscovery") || strTopic == (mqttGroupCommandTopic + "/hassdiscovery"))
//   {                                             // '[...]/device/command/hassdiscovery' -m 'homeassistant' == hassDiscovery = homeassistant
//     strPayload.toCharArray(hassDiscovery, 128); // set hassDiscovery to value provided in payload
//     configSave();
//     mqttDiscovery(); // send Home Assistant discovery message on new discovery topic via MQTT
//   }
//   else if ((strTopic == (mqttCommandTopic + "/nextionmaxpages") || strTopic == (mqttGroupCommandTopic + "/nextionmaxpages")) && (strPayload.toInt() < 256) && (strPayload.toInt() > 0))
//   {                                       // '[...]/device/command/nextionmaxpages' -m '11' == nextionmaxpages = 11
//     nextionMaxPages = strPayload.toInt(); // set nextionMaxPages to value provided in payload
//     configSave();
//     mqttDiscovery(); // send Home Assistant discovery message via MQTT
//   }
//   else if ((strTopic == (mqttCommandTopic + "/nextionbaud") || strTopic == (mqttGroupCommandTopic + "/nextionbaud")) &&
//            ((strPayload.toInt() == 2400) ||
//             (strPayload.toInt() == 4800) ||
//             (strPayload.toInt() == 9600) ||
//             (strPayload.toInt() == 19200) ||
//             (strPayload.toInt() == 31250) ||
//             (strPayload.toInt() == 38400) ||
//             (strPayload.toInt() == 57600) ||
//             (strPayload.toInt() == 115200) ||
//             (strPayload.toInt() == 230400) ||
//             (strPayload.toInt() == 250000) ||
//             (strPayload.toInt() == 256000) ||
//             (strPayload.toInt() == 512000) ||
//             (strPayload.toInt() == 921600)))
//   {                                         // '[...]/device/command/nextionbaud' -m '921600' == nextionBaud = 921600
//     strPayload.toCharArray(nextionBaud, 7); // set nextionBaud to value provided in payload
//     nextionAckEnable = false;
//     nextionSendCmd("bauds=" + strPayload); // send baud rate to nextion
//     nextionAckEnable = true;
//     Serial.flush();
//     Serial2.flush();
//     Serial.end();
//     Serial2.end();
//     Serial.begin(atoi(nextionBaud));  // Serial - LCD RX (after swap), debug TX
//     Serial2.begin(atoi(nextionBaud)); // Serial2 - LCD TX, no RX
//     Serial.swap();                    // Swap to allow hardware UART comms to LCD
//     configSave();
//   }
//   else if (strTopic == (mqttCommandTopic + "/debugserialenabled") || strTopic == (mqttGroupCommandTopic + "/debugserialenabled"))
//   {                                             // '[...]/device/command/debugserialenabled' -m 'true' == enable serial debug output
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       debugSerialEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       debugSerialEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/debugtelnetenabled") || strTopic == (mqttGroupCommandTopic + "/debugtelnetenabled"))
//   {                                             // '[...]/device/command/debugtelnetenabled' -m 'true' == enable telnet debug output
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       debugTelnetEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       debugTelnetEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/mdnsenabled") || strTopic == (mqttGroupCommandTopic + "/mdnsenabled"))
//   {                                             // '[...]/device/command/mdnsenabled' -m 'true' == enable mDNS responder
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       mdnsEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       mdnsEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/beepenabled") || strTopic == (mqttGroupCommandTopic + "/beepenabled"))
//   {                                             // '[...]/device/command/beepenabled' -m 'true' == enable beep output on keypress
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       beepEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       beepEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/ignoretouchwhenoff") || strTopic == (mqttGroupCommandTopic + "/ignoretouchwhenoff"))
//   {                                             // '[...]/device/command/ignoretouchwhenoff' -m 'true' == disable actions on keypress
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       ignoreTouchWhenOff = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       ignoreTouchWhenOff = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
//   { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' == nextionOtaStartDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//     if (strPayload == "")
//     {
//       nextionOtaStartDownload(lcdFirmwareUrl);
//     }
//     else
//     {
//       nextionOtaStartDownload(strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
//   { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//     if (strPayload == "")
//     {
//       espStartOta(espFirmwareUrl);
//     }
//     else
//     {
//       espStartOta(strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/reboot") || strTopic == (mqttGroupCommandTopic + "/reboot"))
//   { // '[...]/device/command/reboot' == reboot microcontroller
//     debugPrintln(F("MQTT: Rebooting device"));
//     espReset();
//   }
//   else if (strTopic == (mqttCommandTopic + "/lcdreboot") || strTopic == (mqttGroupCommandTopic + "/lcdreboot"))
//   { // '[...]/device/command/lcdreboot' == reboot LCD panel
//     debugPrintln(F("MQTT: Rebooting LCD"));
//     nextionReset();
//   }
//   else if (strTopic == (mqttCommandTopic + "/factoryreset") || strTopic == (mqttGroupCommandTopic + "/factoryreset"))
//   { // '[...]/device/command/factoryreset' == clear all saved settings
//     configClearSaved();
//   }
//   else if (strTopic == (mqttCommandTopic + "/beep") || strTopic == (mqttGroupCommandTopic + "/beep"))
//   { // '[...]/device/command/beep' == activate beep function
//     String mqttvar1 = getSubtringField(strPayload, ',', 0);
//     String mqttvar2 = getSubtringField(strPayload, ',', 1);
//     String mqttvar3 = getSubtringField(strPayload, ',', 2);

//     beepOnTime = mqttvar1.toInt();
//     beepOffTime = mqttvar2.toInt();
//     beepCounter = mqttvar3.toInt();
//   }
//   else if (strTopic == (mqttCommandTopic + "/crashtest"))
//   { // '[...]/device/command/crashtest' -m 'divzero' == divide by zero
//     if (strPayload == "divzero")
//     {
//       debugPrintln(String(F("DEBUG: attempt to divide by zero")));
//       int result, zero;
//       zero = 0;
//       result = 1 / zero;
//       debugPrintln(String(F("DEBUG: div zero result: ")) + String(result));
//     }
//     else if (strPayload == "nullptr")
//     { // '[...]/device/command/crashtest' -m 'nullptr' == dereference a null pointer
//       debugPrintln(String(F("DEBUG: attempt to dereference null pointer")));
//       int *nullPointer = NULL;
//       debugPrintln(String(F("DEBUG: dereference null pointer: ")) + String(*nullPointer));
//     }
//     else if (strPayload == "wdt")
//     { // '[...]/device/command/crashtest' -m 'wdt' == trigger soft WDT
//       debugPrintln(String(F("DEBUG: enter tight loop and cause WDT")));
//       while (true)
//       {
//       }
//     }
//   }
//   else if (strTopic.startsWith(mqttCommandTopic) && (strPayload == ""))
//   { // '[...]/device/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//     String subTopic = strTopic.substring(mqttCommandTopic.length() + 1);
//     mqttGetSubtopic = "/" + subTopic;
//     nextionGetAttr(subTopic);
//   }
//   else if (strTopic.startsWith(mqttGroupCommandTopic) && (strPayload == ""))
//   { // '[...]/group/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//     String subTopic = strTopic.substring(mqttGroupCommandTopic.length() + 1);
//     mqttGetSubtopic = "/" + subTopic;
//     nextionGetAttr(subTopic);
//   }
//   else if (strTopic.startsWith(mqttCommandTopic))
//   { // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//     String subTopic = strTopic.substring(mqttCommandTopic.length() + 1);
//     nextionSetAttr(subTopic, strPayload);
//   }
//   else if (strTopic.startsWith(mqttGroupCommandTopic))
//   { // '[...]/group/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//     String subTopic = strTopic.substring(mqttGroupCommandTopic.length() + 1);
//     nextionSetAttr(subTopic, strPayload);
//   }
//   else if (strTopic == mqttLightBrightCommandTopic)
//   { // change the brightness from the light topic
//     nextionSetAttr("dim", strPayload);
//     nextionSetAttr("dims", "dim");
//     lcdBacklightDim = strPayload.toInt();
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : '")) + strPayload + String(F("'")));
//     mqttClient.publish(mqttLightBrightStateTopic, strPayload, true, 0);
//   }
//   else if (strTopic == mqttLightCommandTopic && strPayload == "OFF")
//   { // set the panel dim OFF from the light topic, saving current dim level first
//     nextionSetAttr("dims", "dim");
//     nextionSetAttr("dim", "0");
//     lcdBacklightOn = 0;
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//     mqttClient.publish(mqttLightStateTopic, "OFF", true, 0);
//   }
//   else if (strTopic == mqttLightCommandTopic && strPayload == "ON")
//   { // set the panel dim ON from the light topic, restoring saved dim level
//     nextionSetAttr("dim", "dims");
//     nextionSetAttr("sleep", "0");
//     lcdBacklightOn = 1;
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//     mqttClient.publish(mqttLightStateTopic, "ON", true, 0);
//   }
//   else if (strTopic == mqttStatusTopic && strPayload == "OFF")
//   { // catch a dangling LWT from a previous connection if it appears
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'ON'")));
//     mqttClient.publish(mqttStatusTopic, "ON", true, 0);
//     mqttClient.publish(mqttStateJSONTopic, String(F("{\"event_type\":\"hasp_device\",\"event\":\"online\"}")));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F(" : {\"event_type\":\"hasp_device\",\"event\":\"online\"}")));
//   }
// }


/****
 * Example MQTT Commands
 * 
 * 
 * 
 * 
 * 
 * 
{
  "commands": [
    "p[9].b[1].txt=\"hello\""
  ]
}

{
  "commands": [
    "page 5"
  ]
}


 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */


void mNextionPanel::SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr){
  char command_ctr[100];
  sprintf(command_ctr,"p[%d].b[%d].txt=\"%s\"",page,element_id,ctr);
  serial_print_suffixed(command_ctr);  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].bco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].pco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
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
  // DEBUG_LINE_HERE_PAUSE;
    SERIAL_NEXTION_TX.print(hmiAttribute);
    SERIAL_NEXTION_TX.print("=");
  // DEBUG_LINE_HERE_PAUSE;
    // SERIAL_NEXTION_TX.print(utf8ascii((char*)hmiValue));
    SERIAL_NEXTION_TX.print(utf8ascii((String)hmiValue));
  // DEBUG_LINE_HERE_PAUSE;
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  #endif
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionSetAttr(const String &hmiAttribute, const String &hmiValue)
// { // Set the value of a Nextion component attribute
//   Serial2.print(hmiAttribute);
//   Serial2.print("=");
//   Serial2.print(hmiValue);
//   Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial2.flush();
//   debugPrintln(String(F("HMI OUT: '")) + hmiAttribute + "=" + hmiValue + String(F("'")));
//   if (nextionAckEnable)
//   {
//     nextionAckReceived = false;
//     nextionAckTimer = millis();

//     while ((!nextionAckReceived) || (millis() - nextionAckTimer > nextionAckTimeout))
//     {
//       nextionHandleInput();
//     }
//     if (!nextionAckReceived)
//     {
//       debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else
//   {
//     nextionHandleInput();
//   }
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionGetAttr(const char* c_str)//String hmiAttribute)
{ // Get the value of a Nextion component attribute
  // This will only send the command to the panel requesting the attribute, the actual
  // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic

  char hmiattribute_ctr[100]; 
  memset(hmiattribute_ctr,0,sizeof(hmiattribute_ctr));
  sprintf(hmiattribute_ctr,"get %s",c_str);
  serial_print_suffixed(hmiattribute_ctr);
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " GET 'get %s'"),hmiattribute_ctr);
}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionGetAttr(const String &hmiAttribute)
// { // Get the value of a Nextion component attribute
//   // This will only send the command to the panel requesting the attribute, the actual
//   // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic
//   Serial2.print("get ");
//   Serial2.print(hmiAttribute);
//   Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial2.flush();
//   debugPrintln(String(F("HMI OUT: 'get ")) + hmiAttribute + String(F("'")));
//   if (nextionAckEnable)
//   {
//     nextionAckReceived = false;
//     nextionAckTimer = millis();

//     while ((!nextionAckReceived) || (millis() - nextionAckTimer > nextionAckTimeout))
//     {
//       nextionHandleInput();
//     }
//     if (!nextionAckReceived)
//     {
//       debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else
//   {
//     nextionHandleInput();
//   }
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog(settings.dynamic_log_level,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd_String(const String &nextionCmd)
{ // Send a raw command to the Nextion panel
  SERIAL_NEXTION_TX.print(nextionCmd);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  debugPrintln(String(F("HMI OUT: ")) + nextionCmd);

  if (nextionAckEnable)
  {
    nextionAckReceived = false;
    nextionAckTimer = millis();

    while ((!nextionAckReceived) && (millis() - nextionAckTimer < nextionAckTimeout))
    {
      nextionHandleInput();
    }
    if (!nextionAckReceived)
    {
      debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
      String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
      // mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
    }
  }
  else
  {
    nextionHandleInput();
  }
}






void mNextionPanel::HueToRgb(uint16_t hue, float* r, float* g, float* b)
{
    float h = hue/360.0f;
    float s = 1.0f;
    float v = 1.0f;

    if (s == 0.0f)
    {
        *r = *g = *b = v; // achromatic or black
    }
    else
    {
        if (h < 0.0f)
        {
            h += 1.0f;
        }
        else if (h >= 1.0f)
        {
            h -= 1.0f;
        }
        h *= 6.0f;
        int i = (int)h;
        float f = h - i;
        float q = v * (1.0f - s * f);
        float p = v * (1.0f - s);
        float t = v * (1.0f - s * (1.0f - f));
        switch (i)
        {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
        }
    }

    *r *= 255;
    *g *= 255;
    *b *= 255;

}

void mNextionPanel::nextionSendCmd_ContainingFormattedText(const char* c_str)
{ // Send a raw command to the Nextion panel

//move format checks here
  char conversion_buffer[100] = {0};

  snprintf(conversion_buffer, sizeof(conversion_buffer), "%s", c_str);

  // AddLog(LOG_LEVEL_INFO, PSTR("before %s"),conversion_buffer);

  uint8_t command_length = strlen(conversion_buffer);  // use this later to check if "matched index + key + 3 data hue bytes" = "length" for vailidity check

  uint16_t converted_565_number = 0;
  uint16_t hue_input = 0;


  char* pos_start_of_token = nullptr;
  char* pos_start_of_response_field = nullptr;

  if((pos_start_of_token=strstr(conversion_buffer, "co=h"))!=nullptr) //pco and bco, returns pointer to substring
  {              
    // AddLog(LOG_LEVEL_INFO, PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    float r,g,b;
    
    HueToRgb(hue_input, &r,&g,&b);

    uint8_t red = (int)r;
    uint8_t green = (int)g;
    uint8_t blue = (int)b;

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));


    // AddLog(LOG_LEVEL_INFO, PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  else
  if((pos_start_of_token=strstr(conversion_buffer, "co=#"))!=nullptr) //pco and bco, returns pointer to substring, "#" (capital) for byte packed rgb
  {               
    // AddLog(LOG_LEVEL_INFO, PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    // hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    
    // AddLog(LOG_LEVEL_INFO, PSTR("pos_start_of_response_field = %s"), pos_start_of_response_field);

    uint32_t colour32bit = 0;
    if(pos_start_of_response_field[0]=='#'){ colour32bit = (long) strtol( &pos_start_of_response_field[1], NULL, 16);
    }else{ colour32bit = (long) strtol( &pos_start_of_response_field[0], NULL, 16); }

    // RgbColor rgb;
    // rgb.R = colour32bit >> 16; //RGB
    // rgb.G = colour32bit >> 8 & 0xFF; //RGB
    // rgb.B = colour32bit & 0xFF; //RGB


    // float r,g,b;
    
    // HueToRgb(hue_input, &r,&g,&b);

    uint8_t red   = colour32bit >> 16; //RGB
    uint8_t green = colour32bit >> 8 & 0xFF; //RGB
    uint8_t blue  = colour32bit & 0xFF; //RGB

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));

// 
    // AddLog(LOG_LEVEL_INFO, PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  
  
  
  
  else{
    // AddLog(LOG_LEVEL_INFO, PSTR("NO match"));

  }

  nextionSendCmd(conversion_buffer);
}


/******************************************************************************************************************
 * SECTION: HardwareInterface
*******************************************************************************************************************/


// Functions which do the majority of the work, for now placed in here to make it easier to build up how the thing works
// Likely this will be moved back in to panel


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionReset()
{
  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD");
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
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD completed");
    if (settings.page)
    {


char command_ctr[15];

sprintf(command_ctr,"page %d\0",settings.page);

      nextionSendCmd(command_ctr);
    }
  }
  else
  {
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ERROR: Rebooting LCD completed, but LCD is not responding.");
  }
  //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionReset()
// {
//   debugPrintln(F("HMI: Rebooting LCD"));
//   digitalWrite(nextionResetPin, LOW);
//   Serial2.print("rest");
//   Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial2.flush();
//   delay(100);
//   digitalWrite(nextionResetPin, HIGH);

//   unsigned long lcdResetTimer = millis();
//   const unsigned long lcdResetTimeout = 5000;

//   lcdConnected = false;
//   while (!lcdConnected && (millis() < (lcdResetTimer + lcdResetTimeout)))
//   {
//     nextionHandleInput();
//   }
//   if (lcdConnected)
//   {
//     debugPrintln(F("HMI: Rebooting LCD completed"));
//     if (nextionActivePage)
//     {
//       nextionSendCmd("page " + String(nextionActivePage));
//     }
//   }
//   else
//   {
//     debugPrintln(F("ERROR: Rebooting LCD completed, but LCD is not responding."));
//   }
//   mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//   debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
// }



// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void espReset()
// {
//   debugPrintln(F("RESET: HASPone reset"));
//   if (mqttClient.connected())
//   {
//     mqttClient.publish(mqttStateJSONTopic, String(F("{\"event_type\":\"hasp_device\",\"event\":\"offline\"}")));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F(" : {\"event_type\":\"hasp_device\",\"event\":\"offline\"}")));
//     mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//     mqttClient.disconnect();
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
//   }
//   debugPrintln(F("HMI: Rebooting LCD"));
//   digitalWrite(nextionResetPin, LOW);
//   Serial2.print("rest");
//   Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial2.flush();
//   delay(500);
//   ESP.reset();
//   delay(5000);
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionConnect()
{


  // if ((millis() - nextionCheckTimer) >= nextionCheckInterval)
  // {
  //   static unsigned int nextionRetryCount = 0;
  //   if ((nextionModel.length() == 0) && (nextionRetryCount < (nextionRetryMax - 2)))
  //   { // Try issuing the "connect" command a few times
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion connect request"));
  //     nextionSendCmd("connect");
  //     nextionRetryCount++;
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((nextionModel.length() == 0) && (nextionRetryCount < nextionRetryMax))
  //   { // If we still don't have model info, try to change nextion serial speed from 9600 to 115200
  //     nextionSetSpeed();
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion serial speed 115200 request"));
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((lcdVersion < 1) && (nextionRetryCount <= nextionRetryMax))
  //   {
  //     if (nextionModel.length() == 0)
  //     { // one last hail mary, maybe the serial speed is set correctly now
  //       nextionSendCmd("connect");
  //     }
  //     //nextionSendCmd("get " + lcdVersionQuery.toString().c_str());
  //     lcdVersionQueryFlag = true;
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion version query"));
  //     nextionCheckTimer = millis();
  //   }
  // }


  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));

  const unsigned long nextionCheckTimeout = 2000; // Max time in msec for nextion connection check
  unsigned long nextionCheckTimer = millis();     // Timer for nextion connection checks

  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));

  nextionSendCmd("connect"); // 
  // S:comok 1,30601-0,NX3224T024_011R,163,61488,DE6064B7E70C6521,4194304ÿÿÿ // response from connect


  if (!lcdConnected)
  { // Check for some traffic from our LCD
    debugPrintln(F("HMI: Waiting for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      nextionHandleInput();
    }
  }

  if (!lcdConnected)
  { // No response from the display using the configured speed, so scan all possible speeds
    nextionSetSpeed();

    nextionCheckTimer = millis(); // Reset our timer
    debugPrintln(F("HMI: Waiting again for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
      nextionHandleInput();
    }
    if (!lcdConnected)
    {
      debugPrintln(F("HMI: LCD connection timed out"));
      return false;
    }
  }

  // Query backlight status.  This should always succeed under simulation or non-HASPone HMI
  lcdBacklightQueryFlag = true;
  debugPrintln(F("HMI: Querying LCD backlight status"));
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  nextionSendCmd("get dim");
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdBacklightQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdBacklightQueryFlag)
  { // Our flag is still set, meaning we never got a response.
    debugPrintln(F("HMI: LCD backlight query timed out"));
    lcdBacklightQueryFlag = false;
    return false;
  }

  // We are now communicating with the panel successfully.  Enable ACK checking for all future commands.
  nextionAckEnable = true;
  nextionSendCmd("bkcmd=3");

  // This check depends on the HMI having been designed with a version number in the object
  // defined in lcdVersionQuery.  It's OK if this fails, it just means the HMI project is
  // not utilizing the version capability that the HASPone project makes use of.
  lcdVersionQueryFlag = true;
  debugPrintln(F("HMI: Querying LCD firmware version number"));
  nextionSendCmd_String("get " + lcdVersionQuery);
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdVersionQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdVersionQueryFlag)
  { // Our flag is still set, meaning we never got a response.  This should only happen if
    // there's a problem.  Non-HASPone projects should pass this check with lcdVersion = 0
    debugPrintln(F("HMI: LCD version query timed out"));
    lcdVersionQueryFlag = false;
    return false;
  }

  if (nextionModel.length() == 0)
  { // Check for LCD model via `connect`.  The Nextion simulator does not support this command,
    // so if we're running under that environment this process should timeout.
    debugPrintln(F("HMI: Querying LCD model information"));
    nextionSendCmd("connect");
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && (nextionModel.length() == 0))
    {
      nextionHandleInput();
    }
  }
  return true;


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
  
  ////AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: No Nextion response, attempting 9600bps connection"));
  // SERIAL_NEXTION_TX.begin(9600);
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.print("bauds=38400");
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.flush();
  // SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  
  // SERIAL_NEXTION_TX.begin(115200);
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.print("bauds=38400");
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.flush();
  // SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  #endif

}

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionSetSpeed()
// {
//   debugPrintln(String(F("HMI: No Nextion response, attempting to set serial speed to ")) + String(nextionBaud));
//   for (unsigned int nextionSpeedsIndex = 0; nextionSpeedsIndex < nextionSpeedsLength; nextionSpeedsIndex++)
//   {
//     debugPrintln(String(F("HMI: Sending bauds=")) + String(nextionBaud) + " @" + String(nextionSpeeds[nextionSpeedsIndex]) + " baud");
//     Serial2.flush();
//     Serial2.begin(nextionSpeeds[nextionSpeedsIndex]);
//     Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial2.print("bauds=" + String(nextionBaud));
//     Serial2.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial2.flush();
//   }
//   Serial2.begin(atoi(nextionBaud));
// }




// ////////////////////////////////////////////////////////////////////////////////////////////////////
// bool mNextionPanel::nextionHandleInput()
// { // Handle incoming serial data from the Nextion panel
//   // This will collect serial data from the panel and place it into the global buffer
//   // nextionReturnBuffer[nextionReturnIndex]
//   // Return: true if we've received a string of 3 consecutive 0xFF values
//   // Return: false otherwise
//   bool nextionCommandComplete = false;
//   static int nextionTermByteCnt = 0;   // counter for our 3 consecutive 0xFFs
//   static String hmiDebug = ""; // assemble a string for debug output

//   if (serial_available())
//   {
//     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION " if (Serial.available())"));
//     lcdConnected = true;
//     byte nextionCommandByte = serial_read();

//     hmiDebug += (" 0x" + String(nextionCommandByte, HEX));
//     // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
//     if (nextionCommandByte == 0xFF)
//     {
//       nextionTermByteCnt++;
//       if (nextionTermByteCnt >= 3)
//       { // We have received a complete command
//         nextionCommandComplete = true;
//         nextionTermByteCnt = 0; // reset counter
//       }
//     }
//     else
//     {
//       nextionTermByteCnt = 0; // reset counter if a non-term byte was encountered
//     }
//     nextionReturnBuffer[nextionReturnIndex] = nextionCommandByte;
//     nextionReturnIndex++;
//   }
//   if (nextionCommandComplete)
//   {
//   //  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),
//     //Serial.println(ConvertBytetoASCII(nextionReturnBuffer,nextionReturnIndex));
//     // for(int i=0;i<nextionReturnIndex;i++){
//     //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "%d | %s"),i,String(nextionReturnBuffer[i], HEX));
//     // }
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),hmiDebug.c_str());
//     hmiDebug = "";

    
// DEBUG_LINE_HERE;
// Serial.println(hmiDebug);

//   }



//   return nextionCommandComplete;
// }



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionHandleInput()
{ // Handle incoming serial data from the Nextion panel
  // This will collect serial data from the panel and place it into the global buffer
  // nextionReturnBuffer[nextionReturnIndex]
  unsigned long handlerTimeout = millis() + 100;
  bool nextionCommandComplete = false;
  static uint8_t nextionTermByteCnt = 0; // counter for our 3 consecutive 0xFFs

  while (SERIAL_NEXTION_RX.available() && !nextionCommandComplete && (millis() < handlerTimeout))
  {
    byte nextionCommandByte = SERIAL_NEXTION_RX.read();
    Serial.write(nextionCommandByte);
    if (nextionCommandByte == 0xFF)
    { // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
      nextionTermByteCnt++;
      if (nextionTermByteCnt >= 3)
      { // We have received a complete command
        lcdConnected = true;
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
    if (nextionCommandComplete)
    {
      nextionAckReceived = true;
      nextionProcessInput();
    }
    yield();
  }


  if (millis() > handlerTimeout)
  {
    debugPrintln(String(F("HMI ERROR: nextionHandleInput timeout")));
  }

}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionProcessInput()
{ // Process incoming serial commands from the Nextion panel
  // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
  // tl;dr, command byte, command data, 0xFF 0xFF 0xFF

  Serial.println("nextionProcessInput");
  DEBUG_LINE_HERE;
  
  char event_ctr[30];
  //https://www.itead.cc/wiki/Nextion_Instruction_Set

  // first instructions byte
  switch(nextionReturnBuffer[0]){
    case NEXTION_COMMAND_INVALID_INSTRUCTION:
      //AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX D_NEXTION_COMMAND D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR));   
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
  DEBUG_LINE_HERE;
  
      screen_press.tSavedButtonONEvent = millis();
      screen_press.fEnableImmediateButtonTime = true; 

      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_ON);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

      JsonBuilderI->Start();
        JsonBuilderI->Add("event", event_ctr);
        JsonBuilderI->Add("value", D_JSON_ON);
      JsonBuilderI->End();

      pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
      pCONT_mqtt->ppublish("status/nextion/event/start",JsonBuilderI->GetBufferPtr(),0);

    }
    if (nextionButtonAction == 0x00) // OFF - LET_GO
    {
  DEBUG_LINE_HERE;
  
      screen_press.tSavedButtonOFFEvent = millis();
      screen_press.fEnableImmediateButtonTime = false; //start timer
      screen_press.tSavedButtonONDurationEvent = screen_press.tSavedButtonOFFEvent - screen_press.tSavedButtonONEvent;
      screen_press.duration = screen_press.tSavedButtonONDurationEvent;
      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_OFF);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
      
      JsonBuilderI->Start();
        JsonBuilderI->Add("event", event_ctr);
        JsonBuilderI->Add("value", D_JSON_OFF);
        JsonBuilderI->Add("duration", screen_press.tSavedButtonONDurationEvent);
      JsonBuilderI->End();

      if(!fEnableIgnoreNextOffEvent){
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = NOT set"));
        pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
        pCONT_mqtt->ppublish("status/nextion/event/end",JsonBuilderI->GetBufferPtr(),0);
        MQTTSend_PressEvent();
      }else{
        fEnableIgnoreNextOffEvent = false;// reset to listen to next event
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = reset"));
      }


      // Now see if this object has a .val that might have been updated.  Works for sliders,
      // two-state buttons, etc, throws a 0x1A error for normal buttons which we'll catch and ignore
      // mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // mqttGetSubtopicJSON = "p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
    }
  }
  else if (nextionReturnBuffer[0] == 0x66)
  { 
  DEBUG_LINE_HERE;
  // Handle incoming "sendme" page number
    // 0x66+PageNum+End
    // Example: 0x66 0x02 0xFF 0xFF 0xFF
    // Meaning: page 2
    String nextionPage = String(nextionReturnBuffer[1]);
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "[sendme Page] \"%s\""),nextionPage.c_str());

    if ((settings.page != nextionPage.toInt()) && ((nextionPage != "0") || nextionReportPage0))
    { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )
      settings.page = nextionPage.toInt();
      String mqttPageTopic = mqttStateTopic + "/page";      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: mqttPageTopic=\"%s\" nextionPage=\"%s\""),mqttPageTopic.c_str(),nextionPage.c_str());
      //mqttClient.publish(mqttPageTopic, nextionPage);
      pCONT_mqtt->ppublish("status/nextion/event4",nextionPage.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x67)
  { 
  DEBUG_LINE_HERE;
  // Handle touch coordinate data
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
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch ON] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOn";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/xyCoord",xyCoord.c_str(),0);
    }
    else if (nextionTouchAction == 0x00)
    {
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch OFF] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOff";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/event6",xyCoord.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x70)
  { 
  DEBUG_LINE_HERE;
  // Handle get string return
    // 0x70+ASCII string+End
    // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
    // Meaning: String data, ABCD1234
    String getString;
    for (int i = 1; i < nextionReturnIndex - 3; i++)
    { // convert the payload into a string
      getString += (char)nextionReturnBuffer[i];
    }
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [String Return] '%s'"),getString.c_str());
  
    if (mqttGetSubtopic == "")
    { // If there's no outstanding request for a value, publish to mqttStateTopic
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttStateTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
    }
    else
    { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttReturnTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x71)
  { 
  DEBUG_LINE_HERE;
  // Handle get int return
    // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
    // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
    // Meaning: Integer data, 123
    unsigned long getInt = nextionReturnBuffer[4];
    getInt = getInt * 256 + nextionReturnBuffer[3];
    getInt = getInt * 256 + nextionReturnBuffer[2];
    getInt = getInt * 256 + nextionReturnBuffer[1];
    String getString = String(getInt);
    //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: [Int Return] '")) + getString + "'");

    if (lcdVersionQueryFlag)
    {
      lcdVersion = getInt;
      lcdVersionQueryFlag = false;
      ////AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: lcdVersion '")) + String(lcdVersion) + "'");
    }
    else if (mqttGetSubtopic == "")
    {
  DEBUG_LINE_HERE;
      //mqttClient.publish(mqttStateTopic, getString);
      Serial.println(getString);
      pCONT_mqtt->ppublish("status/nextion/event9",getString.c_str(),0);
    }
    // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
    else
    {
  DEBUG_LINE_HERE;
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
      //mqttClient.publish(mqttReturnTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event10",getString.c_str(),0);
      String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopicJSON + String(F("\", \"value\":")) + getString + String(F("}"));
      //mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      pCONT_mqtt->ppublish("status/nextion/event11",mqttButtonJSONEvent.c_str(),0);
      mqttGetSubtopic = "";
    }
  DEBUG_LINE_HERE;

  }
  else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
  { 
  DEBUG_LINE_HERE;
  // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol

    ALOG_INF(PSTR("Catch 'comok' response to 'connect' command \"%s\""), nextionReturnBuffer);

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
          // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: nextionModel: ")) + nextionModel);
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
  
  // Process complete incoming serial command from the Nextion panel
//   // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
//   // tl;dr: command byte, command data, 0xFF 0xFF 0xFF

//   if (nextionReturnBuffer[0] == 0x01)
//   { // 	Instruction Successful - quietly ignore this as it will be returned after every command issued,
//     //  and processing it + spitting out serial output is a huge drag on performance if serial debug is enabled.

//     // debugPrintln(String(F("HMI IN: [Instruction Successful] 0x")) + String(nextionReturnBuffer[0], HEX));
//     // if (mqttClient.connected())
//     // {
//     //   String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Instruction Successful\"}"));
//     //   mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//     //   debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     // }
//     nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
//     return;                 // skip the rest of the tests below and return immediately
//   }

//   debugPrintln(String(F("HMI IN: [")) + String(nextionReturnIndex) + String(F(" bytes]: ")) + printHex8(nextionReturnBuffer, nextionReturnIndex));

//   if (nextionReturnBuffer[0] == 0x00 && nextionReturnBuffer[1] == 0x00 && nextionReturnBuffer[2] == 0x00)
//   { // Nextion Startup
//     debugPrintln(String(F("HMI IN: [Nextion Startup] 0x00 0x00 0x00")));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x00 0x00 0x00\",\"return_code_description\":\"Nextion Startup\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x00)
//   { // Invalid Instruction
//     debugPrintln(String(F("HMI IN: [Invalid Instruction] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Instruction\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x02)
//   { // Invalid Component ID
//     debugPrintln(String(F("HMI IN: [Invalid Component ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Component ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x03)
//   { // Invalid Page ID
//     debugPrintln(String(F("HMI IN: [Invalid Page ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Page ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x04)
//   { // Invalid Picture ID
//     debugPrintln(String(F("HMI IN: [Invalid Picture ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Picture ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x05)
//   { // Invalid Font ID
//     debugPrintln(String(F("HMI IN: [Invalid Font ID	] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Font ID	\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x06)
//   { // Invalid File Operation
//     debugPrintln(String(F("HMI IN: [Invalid File Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid File Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x09)
//   { // Invalid CRC
//     debugPrintln(String(F("HMI IN: [Invalid CRC] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid CRC\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x11)
//   { // Invalid Baud rate Setting
//     debugPrintln(String(F("HMI IN: [Invalid Baud rate Setting] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Baud rate Setting\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x12)
//   { // Invalid Waveform ID or Channel #
//     debugPrintln(String(F("HMI IN: [Invalid Waveform ID or Channel #] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Waveform ID or Channel #\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1A)
//   { // Invalid Variable name or attribute
//     debugPrintln(String(F("HMI IN: [Invalid Variable name or attribute] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable name or attribute\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1B)
//   { // Invalid Variable Operation
//     debugPrintln(String(F("HMI IN: [Invalid Variable Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1C)
//   { // Assignment failed to assign
//     debugPrintln(String(F("HMI IN: [Assignment failed to assign] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Assignment failed to assign\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1D)
//   { // EEPROM Operation failed
//     debugPrintln(String(F("HMI IN: [EEPROM Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"EEPROM Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1E)
//   { // Invalid Quantity of Parameters
//     debugPrintln(String(F("HMI IN: [Invalid Quantity of Parameters] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Quantity of Parameters\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1F)
//   { // IO Operation failed
//     debugPrintln(String(F("HMI IN: [IO Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"IO Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x20)
//   { // Escape Character Invalid
//     debugPrintln(String(F("HMI IN: [Escape Character Invalid] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Escape Character Invalid\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x23)
//   { // Variable name too long
//     debugPrintln(String(F("HMI IN: [Variable name too long] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Variable name too long\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x24)
//   { // Serial Buffer Overflow
//     debugPrintln(String(F("HMI IN: [Serial Buffer Overflow] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Serial Buffer Overflow\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }

//   else if (nextionReturnBuffer[0] == 0x65)
//   { // Handle incoming touch command
//     // 0x65+Page ID+Component ID+TouchEvent+End
//     // Return this data when the touch event created by the user is pressed.
//     // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
//     // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
//     // Meaning: Touch Event, Page 0, Object 2, Press
//     String nextionPage = String(nextionReturnBuffer[1]);
//     String nextionButtonID = String(nextionReturnBuffer[2]);
//     byte nextionButtonAction = nextionReturnBuffer[3];

//     if (nextionButtonAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Button ON] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_press_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "ON");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'ON'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//       }
//       if (beepEnabled)
//       {
//         beepOnTime = 500;
//         beepOffTime = 100;
//         beepCounter = 1;
//       }
//       if (rebootOnp0b1 && (nextionPage == "0") && (nextionButtonID == "1"))
//       {
//         debugPrintln(String(F("HMI IN: p[0].b[1] pressed during HASPone configuration, rebooting.")));
//         espReset();
//       }
//     }
//     else if (nextionButtonAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Button OFF] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_release_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "OFF");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'OFF'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_release\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"OFF\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//           // Now see if this object has a .val that might have been updated.  Works for sliders,
//           // two-state buttons, etc, returns 0 for normal buttons
//           mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
//           // This right here is dicey.  We're done w/ this command so reset the index allowing this to be kinda-reentrant
//           // because the call to nextionGetAttr is going to call us back.
//           nextionReturnIndex = 0;
//           nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
//         }
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x66)
//   { // Handle incoming "sendme" page number
//     // 0x66+PageNum+End
//     // Example: 0x66 0x02 0xFF 0xFF 0xFF
//     // Meaning: page 2
//     String nextionPage = String(nextionReturnBuffer[1]);
//     debugPrintln(String(F("HMI IN: [sendme Page] '")) + nextionPage + String(F("'")));
//     if ((nextionPage != "0") || nextionReportPage0)
//     { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )

//       if (mqttClient.connected())
//       {
//         String mqttButtonJSONEvent = String(F("{\"event\":\"page\",\"value\":")) + nextionPage + String(F("}"));
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         String mqttPageTopic = mqttStateTopic + "/page";
//         debugPrintln(String(F("MQTT OUT: '")) + mqttPageTopic + String(F("' : '")) + nextionPage + String(F("'")));
//         mqttClient.publish(mqttPageTopic, nextionPage, false, 0);
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x67 || nextionReturnBuffer[0] == 0x68)
//   { // Handle touch coordinate data
//     // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
//     // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
//     // Meaning: Coordinate (122,30), Touch Event: Press
//     // issue Nextion command "sendxy=1" to enable this output
//     // 0x68 is the same, but returned when the screen touch has awakened the screen from sleep
//     uint16_t xCoord = nextionReturnBuffer[1];
//     xCoord = xCoord * 256 + nextionReturnBuffer[2];
//     uint16_t yCoord = nextionReturnBuffer[3];
//     yCoord = yCoord * 256 + nextionReturnBuffer[4];
//     String xyCoord = String(xCoord) + String(',') + String(yCoord);
//     byte nextionTouchAction = nextionReturnBuffer[5];
//     if (nextionTouchAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Touch ON] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOn";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"ON\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//     else if (nextionTouchAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Touch OFF] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOff";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"OFF\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x70)
//   { // Handle get string return
//     // 0x70+ASCII string+End
//     // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
//     // Meaning: String data, ABCD1234
//     String getString;
//     for (int i = 1; i < nextionReturnIndex - 3; i++)
//     { // convert the payload into a string
//       getString += (char)nextionReturnBuffer[i];
//     }
//     debugPrintln(String(F("HMI IN: [String Return] '")) + getString + String(F("'")));
//     if (mqttClient.connected())
//     {
//       if (mqttGetSubtopic == "")
//       { // If there's no outstanding request for a value, publish to mqttStateTopic
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//       else
//       { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":\"")) + getString + String(F("\"}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttGetSubtopic = "";
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x71)
//   { // Handle get int return
//     // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
//     // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
//     // Meaning: Integer data, 123
//     long getInt = nextionReturnBuffer[4];
//     getInt = getInt * 256 + nextionReturnBuffer[3];
//     getInt = getInt * 256 + nextionReturnBuffer[2];
//     getInt = getInt * 256 + nextionReturnBuffer[1];
//     String getString = String(getInt);
//     debugPrintln(String(F("HMI IN: [Int Return] '")) + getString + String(F("'")));

//     if (lcdVersionQueryFlag)
//     {
//       lcdVersion = getInt;
//       lcdVersionQueryFlag = false;
//       debugPrintln(String(F("HMI IN: lcdVersion '")) + String(lcdVersion) + String(F("'")));
//     }
//     else if (lcdBacklightQueryFlag)
//     {
//       lcdBacklightDim = getInt;
//       lcdBacklightQueryFlag = false;
//       if (lcdBacklightDim > 0)
//       {
//         lcdBacklightOn = 1;
//       }
//       else
//       {
//         lcdBacklightOn = 0;
//       }
//       debugPrintln(String(F("HMI IN: lcdBacklightDim '")) + String(lcdBacklightDim) + String(F("'")));
//     }
//     else if (mqttGetSubtopic == "")
//     {
//       if (mqttClient.connected())
//       {
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//     }
//     // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//     else
//     {
//       if (mqttClient.connected())
//       {
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":")) + getString + String(F("}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//       mqttGetSubtopic = "";
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
//   { // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol
//     String comokField;
//     uint8_t comokFieldCount = 0;
//     byte comokFieldSeperator = 0x2c; // ","

//     for (uint8_t i = 0; i <= nextionReturnIndex; i++)
//     { // cycle through each byte looking for our field seperator
//       if (nextionReturnBuffer[i] == comokFieldSeperator)
//       { // Found the end of a field, so do something with it.  Maybe.
//         if (comokFieldCount == 2)
//         {
//           nextionModel = comokField;
//           debugPrintln(String(F("HMI IN: nextionModel: ")) + nextionModel);
//         }
//         comokFieldCount++;
//         comokField = "";
//       }
//       else
//       {
//         comokField += String(char(nextionReturnBuffer[i]));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x86)
//   { // Returned when Nextion enters sleep automatically. Using sleep=1 will not return an 0x86
//     // 0x86+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 0;
//       mqttClient.publish(mqttLightStateTopic, "OFF", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"ON\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x87)
//   { // Returned when Nextion leaves sleep automatically. Using sleep=0 will not return an 0x87
//     // 0x87+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 1;
//       mqttClient.publish(mqttLightStateTopic, "ON", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//       mqttClient.publish(mqttLightBrightStateTopic, String(lcdBacklightDim), true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : ")) + String(lcdBacklightDim));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"OFF\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x88)
//   { // Returned when Nextion powers on
//     // 0x88+End
//     debugPrintln(F("HMI: Nextion panel connected."));
//   }

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


/******************************************************************************************************************
 * SECTION: Helper Functions
*******************************************************************************************************************/


/**
 * @brief This function going forward should only send the command if the page is not already set, so will need a way to poll this
 * 
 * @param page 
 */
void mNextionPanel::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}

void mNextionPanel::Command_SetPageIfChanged(uint8_t page){
  if(settings.page == page) {return;}
  Command_SetPage(page);
}


// void mNextionPanel::Command_SetPageIfChanged(char* page){
//   if(settings.page == page) {return;}
//   Command_SetPage(page);
// }
// void mNextionPanel::Command_SetPage(char* page){
//   settings.page = page;
//   nextionSendCmd(page);
// }


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



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espWifiSetup()
{ // Connect to WiFi
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"WiFi Connecting\"");
}


// UTF8-Decoder: convert UTF8-String to extended ASCII http://playground.arduino.cc/main/Utf8ascii
// Workaround for issue here: https://github.com/home-assistant/home-assistant/issues/9528
// Nextion claims that "Unicode and UTF will not be among the supported encodings", so this should
// be safe to run against all attribute values coming in.
byte mNextionPanel::utf8ascii(byte ascii)
{ 
    
    
static byte c1; // Last character buffer
    // Convert a single Character from UTF8 to Extended ASCII. Return "0" if a byte has to be ignored.
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

// void mNextionPanel::utf8ascii(char *s)
// { // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
//   uint16_t k = 0;
//   char c;
//   for (uint16_t i = 0; i < strlen(s); i++)
//   {
//     c = utf8ascii(s[i]);
//     if (c != 0)
//       s[k++] = c;
//   }
//   s[k] = 0;
// }

char* mNextionPanel::utf8ascii_Char(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)

AddLog(LOG_LEVEL_ERROR, PSTR("mNextionPanel::utf8ascii2(char *s) crashing function"));

  // uint16_t k = 0;
  // char c;
  // for (uint16_t i = 0; i < strlen(s); i++)
  // {
  //   c = utf8ascii(s[i]);
  //   if (c != 0)
  //     s[k++] = c;
  // }
  // s[k] = 0;
  // return s;
  // return nullptr;
  return s;
}



void mNextionPanel::EverySecond_FlashScreen(){

  if(flash_message.cShowSeconds==0){
    // Return screen to previous
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"settings.page_saved",settings.page_saved);
    Command_SetPage(settings.page_saved);
    flash_message.cShowSeconds = -1;
  }else
  if(flash_message.cShowSeconds>0){
    flash_message.cShowSeconds--;
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"flash_message.cShowSeconds",flash_message.cShowSeconds);
  }

} //end F

// https://seithan.com/Easy-Nextion-Library/Use-Nextion-General-View/
void mNextionPanel::EverySecond_SendScreenInfo(){

  char rtc_ctr[40]; memset(rtc_ctr,'\0',sizeof(rtc_ctr));
  sprintf(rtc_ctr, "%02d:%02d:%02d\n\r",
  pCONT_time->RtcTime.hour,pCONT_time->RtcTime.minute,pCONT_time->RtcTime.second);

  SetAttribute_Txt(settings.page,1,rtc_ctr);
  
  // char health_ctr[40]; memset(health_ctr,'\0',sizeof(health_ctr));
  // sprintf(health_ctr, "%c%c%c %d",
  //   WiFi.status() == WL_CONNECTED ? 'N' : 'n',
  //   pCONT_mqtt->pubsub->connected() ? 'M' : 'm',
  //   fOpenHABDataStreamActive ? 'O' : 'o',
  //   0
  //   // pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI())
  // );

  // Serial.println(health_ctr);

  // SetAttribute_Txt(settings.page,5,health_ctr);

}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void mqttConnect()
// { // MQTT connection and subscriptions

//   static bool mqttFirstConnect = true; // For the first connection, we want to send an OFF/ON state to
//                                        // trigger any automations, but skip that if we reconnect while
//                                        // still running the sketch
//   rebootOnp0b1 = true;
//   static uint8_t mqttReconnectCount = 0;
//   unsigned long mqttConnectTimer = 0;
//   const unsigned long mqttConnectTimeout = 5000;

//   // Check to see if we have a broker configured and notify the user if not
//   if (strcmp(mqttServer, "") == 0)
//   {
//     nextionSendCmd("page 0");
//     nextionSetAttr("p[0].b[1].font", "6");
//     nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rConfigure MQTT:\\rhttp://" + WiFi.localIP().toString() + "\"");
//     while (strcmp(mqttServer, "") == 0)
//     { // Handle other stuff while we're waiting for MQTT to be configured
//       yield();
//       nextionHandleInput();     // Nextion serial communications loop
//       ArduinoOTA.handle();      // Arduino OTA loop
//       webServer->handleClient(); // webServer loop
//       telnetHandleClient();     // telnet client loop
//       motionHandle();           // motion sensor loop
//       beepHandle();             // beep feedback loop
//     }
//   }

//   if (mqttTlsEnabled)
//   { // Create MQTT service object with TLS connection
//     mqttClient.begin(mqttServer, atoi(mqttPort), mqttClientSecure);
//     if (strcmp(mqttFingerprint, "") == 0)
//     {
//       debugPrintln(String(F("MQTT: Configuring MQTT TLS connection without fingerprint validation.")));
//       mqttClientSecure.setInsecure();
//     }
//     else
//     {
//       debugPrintln(String(F("MQTT: Configuring MQTT TLS connection with fingerprint validation.")));
//       mqttClientSecure.allowSelfSignedCerts();
//       mqttClientSecure.setFingerprint(mqttFingerprint);
//     }
//     mqttClientSecure.setBufferSizes(512, 512);
//   }
//   else
//   { // Create MQTT service object without TLS connection
//     debugPrintln(String(F("MQTT: Configuring MQTT connection without TLS.")));
//     mqttClient.begin(mqttServer, atoi(mqttPort), wifiClient);
//   }

//   mqttClient.onMessage(mqttProcessInput); // Setup MQTT callback function

//   // MQTT topic string definitions
//   mqttStateTopic = "hasp/" + String(haspNode) + "/state";
//   mqttStateJSONTopic = "hasp/" + String(haspNode) + "/state/json";
//   mqttCommandTopic = "hasp/" + String(haspNode) + "/command";
//   mqttGroupCommandTopic = "hasp/" + String(groupName) + "/command";
//   mqttStatusTopic = "hasp/" + String(haspNode) + "/status";
//   mqttSensorTopic = "hasp/" + String(haspNode) + "/sensor";
//   mqttLightCommandTopic = "hasp/" + String(haspNode) + "/light/switch";
//   mqttLightStateTopic = "hasp/" + String(haspNode) + "/light/state";
//   mqttLightBrightCommandTopic = "hasp/" + String(haspNode) + "/brightness/set";
//   mqttLightBrightStateTopic = "hasp/" + String(haspNode) + "/brightness/state";
//   mqttMotionStateTopic = "hasp/" + String(haspNode) + "/motion/state";

//   const String mqttCommandSubscription = mqttCommandTopic + "/#";
//   const String mqttGroupCommandSubscription = mqttGroupCommandTopic + "/#";
//   const String mqttLightSubscription = mqttLightCommandTopic + "/#";
//   const String mqttLightBrightSubscription = mqttLightBrightCommandTopic + "/#";

//   // Generate an MQTT client ID as haspNode + our MAC address
//   mqttClientId = String(haspNode) + "-" + String(espMac[0], HEX) + String(espMac[1], HEX) + String(espMac[2], HEX) + String(espMac[3], HEX) + String(espMac[4], HEX) + String(espMac[5], HEX);
//   nextionSendCmd("page 0");
//   nextionSetAttr("p[0].b[1].font", "6");
//   nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rMQTT Connecting:\\r " + String(mqttServer) + "\"");
//   if (mqttTlsEnabled)
//   {
//     debugPrintln(String(F("MQTT: Attempting connection to broker ")) + String(mqttServer) + String(F(" on port ")) + String(mqttPort) + String(F(" with TLS enabled as clientID ")) + mqttClientId);
//   }
//   else
//   {
//     debugPrintln(String(F("MQTT: Attempting connection to broker ")) + String(mqttServer) + String(F(" on port ")) + String(mqttPort) + String(F(" with TLS disabled as clientID ")) + mqttClientId);
//   }

//   // Set keepAlive, cleanSession, timeout
//   mqttClient.setOptions(30, true, mqttConnectTimeout);

//   // declare LWT
//   mqttClient.setWill(mqttStatusTopic.c_str(), "OFF", true, 1);

//   while (!mqttClient.connected())
//   { // Loop until we're connected to MQTT
//     mqttConnectTimer = millis();
//     mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword, false);

//     if (mqttClient.connected())
//     { // Attempt to connect to broker, setting last will and testament
//       // Update panel with MQTT status
//       nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rMQTT Connected:\\r " + String(mqttServer) + "\"");
//       debugPrintln(F("MQTT: connected"));

//       // Reset our diagnostic booleans
//       mqttPingCheck = true;
//       mqttPortCheck = true;

//       // Subscribe to our incoming topics
//       if (mqttClient.subscribe(mqttCommandSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttCommandSubscription);
//       }
//       if (mqttClient.subscribe(mqttGroupCommandSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttGroupCommandSubscription);
//       }
//       if (mqttClient.subscribe(mqttLightSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttLightSubscription);
//       }
//       if (mqttClient.subscribe(mqttLightBrightSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttLightBrightSubscription);
//       }

//       // Publish discovery configuration
//       mqttDiscovery();

//       // Publish backlight status
//       if (lcdBacklightOn)
//       {
//         debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//         mqttClient.publish(mqttLightStateTopic, "ON", true, 1);
//       }
//       else
//       {
//         debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//         mqttClient.publish(mqttLightStateTopic, "OFF", true, 1);
//       }
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : ")) + String(lcdBacklightDim));
//       mqttClient.publish(mqttLightBrightStateTopic, String(lcdBacklightDim), true, 1);

//       if (mqttFirstConnect)
//       { // Force any subscribed clients to toggle OFF/ON when we first connect to
//         // make sure we get a full panel refresh at power on.  Sending OFF,
//         // "ON" will be sent by the mqttStatusTopic subscription action below.
//         mqttFirstConnect = false;
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + "' : 'OFF'");
//         mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//       }

//       if (mqttClient.subscribe(mqttStatusTopic))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttStatusTopic);
//       }
//       mqttClient.loop();
//     }
//     else
//     { // Retry until we give up and restart after connectTimeout seconds
//       mqttReconnectCount++;
//       if (mqttReconnectCount * mqttConnectTimeout * 6 > (connectTimeout * 1000))
//       {
//         debugPrintln(String(F("MQTT: connection attempt ")) + String(mqttReconnectCount) + String(F(" failed with rc: ")) + String(mqttClient.returnCode()) + String(F(" and error: ")) + String(mqttClient.lastError()) + String(F(". Restarting device.")));
//         espReset();
//       }
//       yield();
//       webServer->handleClient();

//       String mqttCheckResult = "Ping: FAILED";
//       String mqttCheckResultNextion = "MQTT Check...";

//       debugPrintln(String(F("MQTT: connection attempt ")) + String(mqttReconnectCount) + String(F(" failed with rc ")) + String(mqttClient.returnCode()) + String(F(" and error: ")) + String(mqttClient.lastError()));
//       nextionSetAttr("p[0].b[1].txt", String(F("\"WiFi Connected!\\r ")) + String(WiFi.SSID()) + String(F("\\rIP: ")) + WiFi.localIP().toString() + String(F("\\r\\rMQTT Failed:\\r ")) + String(mqttServer) + String(F("\\rRC: ")) + String(mqttClient.returnCode()) + String(F("   Error: ")) + String(mqttClient.lastError()) + String(F("\\r")) + mqttCheckResultNextion + String(F("\"")));

//       mqttPingCheck = Ping.ping(mqttServer, 4);
//       yield();
//       webServer->handleClient();
//       mqttPortCheck = wifiClient.connect(mqttServer, atoi(mqttPort));
//       yield();
//       webServer->handleClient();

//       mqttCheckResultNextion = "Ping: ";
//       if (mqttPingCheck)
//       {
//         mqttCheckResult = "Ping: SUCCESS";
//         mqttCheckResultNextion = "Ping: ";
//       }
//       if (mqttPortCheck)
//       {
//         mqttCheckResult += " Port: SUCCESS";
//         mqttCheckResultNextion += " Port: ";
//       }
//       else
//       {
//         mqttCheckResult += " Port: FAILED";
//         mqttCheckResultNextion += " Port: ";
//       }
//       debugPrintln(String(F("MQTT: connection checks: ")) + mqttCheckResult + String(F(". Trying again in 30 seconds.")));
//       nextionSetAttr("p[0].b[1].txt", String(F("\"WiFi Connected!\\r ")) + String(WiFi.SSID()) + String(F("\\rIP: ")) + WiFi.localIP().toString() + String(F("\\r\\rMQTT Failed:\\r ")) + String(mqttServer) + String(F("\\rRC: ")) + String(mqttClient.returnCode()) + String(F("   Error: ")) + String(mqttClient.lastError()) + String(F("\\r")) + mqttCheckResultNextion + String(F("\"")));

//       while (millis() < (mqttConnectTimer + (mqttConnectTimeout * 6)))
//       {
//         yield();
//         nextionHandleInput();     // Nextion serial communications loop
//         ArduinoOTA.handle();      // Arduino OTA loop
//         webServer->handleClient(); // webServer loop
//         telnetHandleClient();     // telnet client loop
//         motionHandle();           // motion sensor loop
//         beepHandle();             // beep feedback loop
//       }
//     }
//   }
//   rebootOnp0b1 = false;
//   if (nextionActivePage < 0)
//   { // We never picked up a message giving us a page number, so we'll just go to the default page
//     debugPrintln(String(F("DEBUG: NextionActivePage not received from MQTT, setting to 0")));
//     String mqttButtonJSONEvent = String(F("{\"event\":\"page\",\"value\":0}"));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent, false, 0);
//     String mqttPageTopic = mqttStateTopic + "/page";
//     debugPrintln(String(F("MQTT OUT: '")) + mqttPageTopic + String(F("' : '0'")));
//     mqttClient.publish(mqttPageTopic, "0", false, 0);
//   }
// }


// ////////////////////////////////////////////////////////////////////////////////
// String printHex8(byte *data, uint8_t length)
// { // returns input bytes as printable hex values in the format 0x01 0x23 0xFF

//   String hex8String;
//   for (int i = 0; i < length; i++)
//   {
//     hex8String += "0x";
//     if (data[i] < 0x10)
//     {
//       hex8String += "0";
//     }
//     hex8String += String(data[i], HEX);
//     if (i != (length - 1))
//     {
//       hex8String += " ";
//     }
//   }
//   // hex8String.toUpperCase();
//   return hex8String;
// }
// // 


/******************************************************************************************************************
 *SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mNextionPanel::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
    JBI->Add("lcdConnected", lcdConnected);
  JsonBuilderI->End();

}

uint8_t mNextionPanel::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){
DEBUG_LINE_HERE;
  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
  JsonBuilderI->End();

}


/******************************************************************************************************************
 * Serial Comms
*******************************************************************************************************************/

  
//serial related stuff for easier clean up




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

#ifdef USE_NEXTION_SOFTWARE_SERIAL
return swSer->available();
#else
return SERIAL_NEXTION_RX.available();
#endif


}

int mNextionPanel::serial_read()
{ 

#ifdef USE_NEXTION_SOFTWARE_SERIAL
return swSer->read();
#else
return SERIAL_NEXTION_RX.read();
#endif

}

void mNextionPanel::serial_print(const char* c_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

  swSer->print(c_str);
  #else
  SERIAL_NEXTION_TX.print(c_str);
#endif


}

void mNextionPanel::serial_print(String s_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
  swSer->print(s_str);
  #else
  SERIAL_NEXTION_TX.print(s_str);
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
  SERIAL_NEXTION_TX.print(c_str);
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


/******************************************************************************************************************
 * SECTION: MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mNextionPanel::MQTTHandler_Init(){

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mNextionPanel::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mNextionPanel::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief Check all handlers if they require action
 * */
void mNextionPanel::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DISPLAYS_NEXTION_ID, handle, id);
  }
}
#endif// USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * SECTION: WebServer
*******************************************************************************************************************/



#ifdef USE_MODULE_NETWORK_WEBSERVER

int8_t mNextionPanel::Tasker_Web(uint8_t function){



  switch(function){  
    
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;

    // case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
    //   // #ifdef USE_MODULE_LIGHTS_ANIMATOR
    //   // pCONT_ladd->
    //   WebAppend_Root_Draw_Table();
    
    // //  WebAppend_Root_Draw_RGBLive();
    //   // #endif
      
    // break; 
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
    //   JsonBuilderI->Add(WEB_HANDLE_LIVEPIXELS_SHARED_JSON,liveview.refresh_rate);
    //   JsonBuilderI->Add("/draw/palette_selector.json",-500);
    // break;
    
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:

    //   //if(pCONT_iLight->animation.mode_id == pCONT_iLight->ANIMATION_MODE_EFFECTS_ID){
    //     WebAppend_Root_RGBPalette();
    //   //}
    //   WebAppend_Root_Sliders();  //move webui stuff into here, as "scenes" will soon be replaced by colour[5].. make it a struct, but read/write using bytes when need by pointer of struct

    // break;
    // case FUNC_WEB_APPEND_ROOT_BUTTONS:
    //   WebAppend_Root_ControlUI();
    // break;
    // case FUNC_WEB_COMMAND:
    //   WebCommand_Parse();
    // break;  
  }

  return 0;

}



void mNextionPanel::WebPage_Root_AddHandlers(){
  
  // if ((configPassword[0] != '\0') && (configUser[0] != '\0'))
  // { // Start the webserver with our assigned password if it's been configured...
  //   httpOTAUpdate.setup(&webServer, "/update", configUser, configPassword);
  // }
  // else
  // { // or without a password if not
  //   httpOTAUpdate.setup(&webServer, "/update");
  // }

  // webServer->on("/", webHandleRoot);
  // webServer->on("/saveConfig", webHandleSaveConfig);
  // webServer->on("/resetConfig", webHandleResetConfig);
  // webServer->on("/resetBacklight", webHandleResetBacklight);
  // webServer->on("/firmware", webHandleFirmware);
  // webServer->on("/espfirmware", webHandleEspFirmware);
  // webServer->on(
  //     "/lcdupload", HTTP_POST, []()
  //     { webServer->send(200); },
  //     webHandleLcdUpload);
  // webServer->on("/tftFileSize", webHandleTftFileSize);
  // webServer->on("/lcddownload", webHandleLcdDownload);
  // webServer->on("/lcdOtaSuccess", webHandleLcdUpdateSuccess);
  // webServer->on("/lcdOtaFailure", webHandleLcdUpdateFailure);
  // webServer->on("/reboot", webHandleReboot);
  // webServer->onNotFound(webHandleNotFound);
  // webServer->begin();
  // debugPrintln(String(F("HTTP: Server started @ http://")) + WiFi.localIP().toString());

  // espSetupOta(); // Start OTA firmware update

  // motionSetup(); // Setup motion sensor if configured

  // mqttConnect(); // Connect to MQTT

  // if (mdnsEnabled)
  // { // Setup mDNS service discovery if enabled
  //   hMDNSService = MDNS.addService(haspNode, "http", "tcp", 80);
  //   if (debugTelnetEnabled)
  //   {
  //     MDNS.addService(haspNode, "telnet", "tcp", 23);
  //   }
  //   MDNS.addServiceTxt(hMDNSService, "app_name", "HASwitchPlate");
  //   MDNS.addServiceTxt(hMDNSService, "app_version", String(haspVersion).c_str());
  //   MDNS.update();
  // }

  /**
   * Root Page 
   * */
  // pCONT_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
  //   WebSend_JSON_RootPage_LiveviewPixels(request); 
  // });
  
  
  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
  /**
   * RGB LIGHT SETTINGS
   * */
  // pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS_PAGE, [this](AsyncWebServerRequest *request){ //"/page"
  //   HandlePage_RGBLightSettings(request); 
  // });
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", [this](AsyncWebServerRequest *request){ //"/page"
//     // HandlePage_RGBLightSettings(request); 

//     Serial.printf("GET\n\r\n\r\n\r\n\r\n\r\n\r");

//     // request->client->

// //  char* buffertemp = request->_tempObject;

// // WebSave_RGBControls(request);
// // int params = request->params();
// // for(int i=0;i<params;i++){
// //   AsyncWebParameter* p = request->getParam(i);
// //   if(p->isFile()){ //p->isPost() is also true
// //     Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
// //   } else if(p->isPost()){
// //     Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
// //   } else {
// //     Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
// //   }
// // }


//   request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,"PAGE_ROOT");//PAGE_rgb_colour_palette_editor);


//   });

  //D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls"
  // pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", HTTP_POST, [this](AsyncWebServerRequest *request){


  //   int params = request->params();
  //   for(int i=0;i<params;i++){
  //     AsyncWebParameter* p = request->getParam(i);
  //     if(p->isFile()){
  //       Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
  //     } else 
  //     if(p->isPost()){
  //       Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
  //     }
  //      else {
  //       Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
  //     }
  //   }

  //   request->send(200, CONTENT_TYPE_TEXT_HTML_ID, "end");

  //   //  if (request->method() == HTTP_POST && request->url() == "/michael") {
  //   //    Serial.println("HTTP_POST && request->url()");
  //   //   // Shoudl be already handled by handleBody(..) at this point.
  //   //   return;
  //   // }
  //   // Web_Base_Page_Draw(request);
  // });
  #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES

//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", HTTP_POST, [](AsyncWebServerRequest *request){
//     //nothing and dont remove it
//   }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     Serial.println("onRequestBody " D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls" );
//     if ((request->url() == D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls") && //"/rest/api/v2/test") &&
//         (request->method() == HTTP_POST))
//     {
      
// #ifdef ENABLE_DEVFEATURE_ARDUINOJSON
//         const size_t        JSON_DOC_SIZE   = 512U;
//         DynamicJsonDocument jsonDoc(JSON_DOC_SIZE);
        
//         if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char*)data))
//         {
//             JsonObject obj = jsonDoc.as<JsonObject>();

//             pCONT_ladd->parse_JSONCommand(jsonDoc.as<JsonObjectConst>());

//             for (JsonPair keyValue : obj) {
//               // AddLog(LOG_LEVEL_INFO, PSTR("key[\"%s\"]=%s"),keyValue.key().c_str(),keyValue.value().as<char*>());
//               if(keyValue.value().as<char*>()){
//                 Serial.print(keyValue.key().c_str()); Serial.print("\t"); Serial.println(keyValue.value().as<char*>());
//               }else{
//                 Serial.print(keyValue.key().c_str()); Serial.print("\t"); Serial.println(keyValue.value().as<int>());
//               }
//             }

//         }
        
//         request->send(200, CONTENT_TYPE_APPLICATION_JSON_ID, "{ \"status\": 0 }");
        
// #endif // ENABLE_DEVFEATURE_ARDUINOJSON
//     }
//   });

//   #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_RGBLightSettings_Draw(request);
//   });
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_RGBLightSettings_UpdateURLs(request);
//   }); 
//   pCONT_web->pWebServer->on("/fetch/animation_control_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_RGBLightSettings_ListOptions(request);
//   }); 


//   /**
//    * PALETTE EDTIOR
//    * */
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_PAGE_CTR, [this](AsyncWebServerRequest *request){ //"/page"
//     HandlePage_PaletteEditor(request); 
//   });
//   pCONT_web->pWebServer->on("/fetch/colour_palette_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_PaletteEditor_ListOptions(request);
//   }); 
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_CTR "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_PaletteEditor_UpdateURLs(request);
//   }); 
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_CTR "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_PaletteEditor_Draw(request);
//   });
//   #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES


} //add handlers


  // #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
void mNextionPanel::Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request){
  
  // JsonBuilderI->Start();
  //   JsonBuilderI->Level_Start("function");
  //     JsonBuilderI->Level_Start("Parse_Urls");        
  //       JsonBuilderI->Add("/fetch/animation_control_list_options.json",-1);
  //     JsonBuilderI->Level_End();
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->End();

  // request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
}



/*
 case FUNC_WEB_ROOT_SEND_STATUS:{

      // char tmp[20];
      // sprintf(tmp,"Brightness %d %%",settings.brightness_percentage);

      // mcl->mweb->WSBufferAppend_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,tmp,"");
      // mcl->mweb->WSBufferAppend_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
      //     "c",               // c - Unique HTML idfrrrreee
      //     "#000", "#eee",//"#fff",    // Black to White
      //     4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
      //     0, 100,  // Range 0/1 to 100%
      //     settings.brightness_percentage,
      //     WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER
      // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
  
  
    }
    break;
    case FUNC_WEB_ADD_BUTTON:
      //mcl->mweb->WSBufferAppend_P(HTTP_BTN_MENU_SETTINGS);
      //mcl->mweb->WSBufferAppend_P(HTTP_BTN_MENU_NEXTION_SETTINGS); //USE self contained method outside of webserver, change rgb
    break;
    case FUNC_WEB_ADD_MAIN_BUTTON:{

      // // Power Toggle
      // mcl->mweb->WSBufferAppend_P(HTTP_TABLE100);
      // mcl->mweb->WSBufferAppend_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      // mcl->mweb->WSBufferAppend_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE, 
      //                           36, 
      //                           WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE, 
      //                           1, 
      //                           "Screen Backlight Toggle",""
      // );
      // mcl->mweb->WSBufferAppend_P(PSTR("</tr></table>"));
  
      // char stemp[100];
      // uint8_t keys_max = 9;

      // mcl->mweb->WSBufferAppend_P(HTTP_TABLE100);
      // mcl->mweb->WSBufferAppend_P(PSTR("<tr> Page Number</tr>"));
      // mcl->mweb->WSBufferAppend_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      // for (uint8_t idx = 0; idx < keys_max; idx++) {
      //   snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
      //   mcl->mweb->WSBufferAppend_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE,         
      //       100 / keys_max, 
      //       WEB_HANDLE_NEXTION_PAGE_SET,
      //       idx, 
      //       (keys_max < 5) ? D_BUTTON_TOGGLE : "",
      //       (keys_max > 1) ? stemp : "");
      // }
      // mcl->mweb->WSBufferAppend_P(PSTR("</tr></table>"));


      // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_SETTINGS, "Nextion Settings");

    }
    break;
    case FUNC_WEB_ROOT_SEND_SCRIPT:{   
      // const char function[] =
      //   "function nextion_brightness(p){"
      //     "la('&" "nextion_brightness" "='+p);"                        // &t related to WebGetArg(request,"t", tmp, sizeof(tmp));
      //   "}";
      // mcl->mweb->WSBufferAppend_P(function);
      
      // AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tasker %s"),"FUNC_WEB_ROOT_SEND_SCRIPT");
    }
    break;
    // case FUNC_WEB_SHOW_PARAMETERS:{
      
    //   // uint8_t fsize = 16;
    //   // char sensor_ctr[50];

    //   // char value_ctr[20];
    //   // memset(value_ctr,0,sizeof(value_ctr));

    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page", screen_press.page);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Event", screen_press.event);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Handled", screen_press.fHandled);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE_UNIT, "Duration", screen_press.duration, "ms");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_SVALUE, "Duration Name", (screen_press.duration<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Type", screen_press.type);      
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE_UNIT, "Screen Brightness", settings.brightness_percentage, "%");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page Set", settings.page);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page Saved", settings.page_saved);

    // }
    // break;
    case FUNC_WEB_ADD_HANDLER:   
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_SETTINGS,      [this](){HandleNextionSettings(); }); // root for all nextion webpages

      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE,      [this](){webHandleNextionFirmware(); });   

      
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "up", [this](){webHandleNextionFirmware(); });
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "u1", [this](){HandleUpgradeFirmwareStart(); });  // URL file

      // // u2 = file upload
      // mcl->mweb->pWebServer->on("/"  "u20", HTTP_POST,[this](){HandleUploadDone(); },[this](){HandleUploadLoop(); });
      // mcl->mweb->pWebServer->on("/"  "u20", HTTP_OPTIONS, [this](){HandlePreflightRequest(); });

      
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "10",      [this](){webHandleNextionFirmware_PhaseOut(); });   



      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_TFTFILESIZE,   [this](){webHandleTftFileSize();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDDOWNLOAD,   [this](){webHandleLcdDownload();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDOTASUCCESS, [this](){webHandleLcdUpdateSuccess();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDOTAFAILURE, [this](){webHandleLcdUpdateFailure();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDREBOOT,     [this](){webHandleReboot();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDUPLOAD, HTTP_POST, [this](){ mcl->mweb->pWebServer->send(200);}, [this](){webHandleLcdUpload();});
    break;
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;
    case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:
    
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "NEXTIONPanel" "}2%s"), " ");
      // //mcl->mweb->WSBufferAppend_P(PSTR("}1" "Version" "}2%s"), mSupportFloatToCStr(_ADALIGHTNEOPIXEL_H));
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "Type" "}2%s"), "WS28xx");
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "Pin" "}2%s"), "Serial0_RX");
      // mcl->mweb->WSBufferAppend_P(PSTR("}1}2&nbsp;"));  // Empty line

    break;

*/


void mNextionPanel::WebCommand_Parse(void)
{
  // char tmp[100];
  // uint8_t  arg_value = 0;

  // char arg_ctr[100]; memset(arg_ctr,0,sizeof(arg_ctr));

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_PAGE_SET);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_PAGE_SET);
  //   Command_SetPage(arg_value);
  // }

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER);
  //   Command_SetBrightness(arg_value);
  // }

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> Command_ToggleBrightness " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE);
  //   Command_ToggleBrightness();
  // }
  
}










// TO BE DELETED
const char HTTP_HEAD2[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE2[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT2[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END2[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS2[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM2[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START2[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM2[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END2[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK2[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED2[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END2[] PROGMEM             = "</div></body></html>";
const char NEXTION_STYLE[] = "<style>button{background-color:#03A9F4;}body{width:60%;margin:auto;}input:invalid{border:1px solid red;}input[type=checkbox]{width:20px;}</style>";
// URL for auto-update "version.json"
const char UPDATE_URL[] = "http://haswitchplate.com/update/version.json";
// Default link to compiled Arduino firmware image
const char espFirmwareUrl[] = "http://haswitchplate.com/update/HASwitchPlate.ino.d1_mini.bin";
// Default link to compiled Nextion firmware images
const char lcdFirmwareUrl[] = "http://haswitchplate.com/update/HASwitchPlate.tft";
//END OF TO BE DELETED






const char HTTP_BTN_MENU_SETTINGS[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_SETTINGS "' method='get'><button>" D_NEXTION_SETTINIGS "</button></form></p>";
const char HTTP_BTN_MENU_FIRMWARE[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_FIRMWARE "' method='get'><button>" D_NEXTION_FIRMWARE "</button></form></p>";


/*****
 * 
 *  Page Name: Palette Editor
 * 
 * ******/
const char HTTP_SCRIPT_MODULE_TEMPLATEc[] PROGMEM =
  "var os;"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=s;"
  "}"
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}";
// const char HTTP_SCRIPT_MODULE1[] PROGMEM =
//   "function x1(a){"
//     "os=a.responseText;"
//     "sk(%d,99);"
//   "}"
//   "function x2(b){"
//     "os=b.responseText;";
// const char HTTP_SCRIPT_MODULE2[] PROGMEM =
//   "}"
//   "function sl(){"
//     "ld('rgb_pal?m=1',x1);"                     // ?m related to pWebServer->hasParam("m")
//     "ld('rgb_pal?g=1',x2);"                     // ?m related to pWebServer->hasParam("m")
//   "}"
//   "window.onload=sl;";
const char HTTP_FORM_RGB_COLOUR_SELECTOR[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" "Colour Palette Editor" "&nbsp;</b></legend>"
  "<form method='get' action='rgb_pal'>"
  "<p></p><b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";



//<option value="0">Red (0)</option>
//"var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\");"
//}2 = \"<option value='\"
//}3 = \")</option>\"
// const char HTTP_MODULE_TEMPLATE_REPLACE[] PROGMEM =
//   "}2%d'>%s (%d}3";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE3[] PROGMEM =
  "}2%d'>%s}4";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE3_NUM[] PROGMEM =
  "}2%d'>%d}4";                       // }2 and }3 are used in below os.replace



// enum CTypes { CkContentTypesT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
// const char [] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";
  

  
/*****
 * 
 *  Page Name: Palette Editor
 * 
 * ******/
const char HTTP_SCRIPT_MODULE_TEMPLATEb[] PROGMEM =
  "var os;"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=s;"
  "}"
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}";
const char HTTP_SCRIPT_MODULE1b[] PROGMEM =
  "function x0(a){"
    "os=a.responseText;"
    "sk(%d,0);"
  "}"
  "function x1(a){"
    "os=a.responseText;"
    "sk(%d,1);"
  "}"
  "function x2(a){"
    "os=a.responseText;"
    "sk(%d,2);"
  "}"
  "function x3(a){"
    "os=a.responseText;"
    "sk(%d,3);"
  "}"
  "function x4(a){"
    "os=a.responseText;"
    "sk(%d,4);"
  "}"
  "function x5(a){"
    "os=a.responseText;"
    "sk(%d,5);"
  "}"
  "function x6(a){"
    "os=a.responseText;"
    "sk(%d,6);"
  "}"
  "function x7(a){"
    "os=a.responseText;"
    "sk(%d,7);"
  "}"
  "function x8(a){"
    "os=a.responseText;"
    "sk(%d,8);"
  "}"
  ;
const char HTTP_SCRIPT_MODULE2b[] PROGMEM =
  "function sl(){"
    "ld('rgb_controls?up=1',x0);" // up = update percentage
    "ld('rgb_controls?po=1',x1);" // po = pixel order
    "ld('rgb_controls?ra=1',x2);" // ra = rate
    "ld('rgb_controls?pr=1',x3);" // pr = period time
    "ld('rgb_controls?tm=1',x4);" // tm = transition mode
    "ld('rgb_controls?am=1',x5);" // am = animaiton mode
    "ld('rgb_controls?pa=1',x6);" // pa  = palette
    "ld('rgb_controls?fl=1',x7);" // fl  = flasher
    "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id
  "}"
  "window.onload=sl;";
const char HTTP_FORM_RGB_COLOUR_SELECTORb[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
  "<form method='get' action='rgb_controls'>"
  "<p></p>"; //space
  //"<b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";


const char kButtonConfirmb[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;


//struct ITEM_1{
  const char kTitle_AmountNumber[] PROGMEM = 
    "1" "|" "2" "|" "3";
  //uint8_t kValue_AmountPercentage[6] = {1,10,25,50,75,100};

  // uint8_t 
  const char kTitle_ListHeadings[] PROGMEM = 
    "Pixels updated (%)" "|" 
    "Pixel Order" "|" 
    "Change Rate (secs)" "|" 
    "Change Period (secs)" "|" 
    "Transition Method" "|" 
    "Mode" "|" 
    "Palette" "|" 
    "Flasher" "|" 
    "Mixer (Running ID)";


// enum ButtonTitle {
//   BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
//   BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_CONSOLE,
//   BUTTON_MODULE, BUTTON_WIFI, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, 
//   BUTTON_RESTORE, 
//   BUTTON_COLOUR_CONTROLS, BUTTON_COLOUR_PALETTE_EDITOR, BUTTON_COLOUR_MIXER_EDITOR,
//   BUTTON_NEXTION_SETTINGS };
// const char kButtonTitle[] PROGMEM =
//   D_RESTART "|" D_RESET_CONFIGURATION "|"
//   D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_CONSOLE "|"
//   D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION  "|"
//   "Colour Controls" "|" "Colour Palette Editor" "|" "Mixer Editor"
//   "|" "Nextion Settings";
// const char kButtonAction[] PROGMEM =
//   ".|rt|"
//   ".|cn|in|up|cs|"
//   "md|wi|lg|co|tp|dl|rs|"
//   "rgb_controls" "|" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR2 "|" WEB_HANDLE_RGB_COLOUR_MIXER_EDITOR2
//   "|" WEB_HANDLE_NEXTION_SETTINGS;


#define D_COLOUR_PICKER "Colour Picker"

const char HTTP_FORM_RGB_SELECTOR1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_COLOUR_PICKER "&nbsp;</b>"
  "</legend><form method='get' action='" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR2 "'>";
const char HTTP_FORM_RGB_SELECTOR2[] PROGMEM =
  "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' placeholder='" SYS_LOG_HOST "' value='%s'></p>"
  "<p><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' placeholder='" STR(SYS_LOG_PORT) "' value='%d'></p>"
  "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' placeholder='" STR(TELE_PERIOD) "' value='%d'></p>";

// const char HTTP_FORM_END[] PROGMEM =
//   "<br/>"
//   "<button name='save' type='submit' class='buttonh bgrn'>" D_SAVE "</button>"
//   "</form></fieldset>";
const char HTTP_FORM_RGB_LOAD_PIXELS[] PROGMEM =
  "<button name='loadpixels' type='submit' class='buttonh bgrn'>" "Load Pixels" "</button>";


const char kRGBPickerTitles[] PROGMEM = "Colour 0" "|" "Colour 1" "|" "Colour 2";
const char kRGBPickerItems[] PROGMEM = "Red" "|" "Green" "|" "Blue";

#define D_RGB_COLOUR_SELECTOR "Colour Palette Selector"
#define D_RGB_CONTROLS "Lights Settings"
#define D_NEXTION_SETTINGS "Nextion Settings"


const char HTTP_BTN_MENU_NEXTION_SETTINGS[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_SETTINGS "' method='get'><button>" D_NEXTION_SETTINGS "</button></form></p>";



void mNextionPanel::WebSave_NextionSettings(void)
{
  char tmp[100]; memset(tmp,0,sizeof(tmp));
  uint8_t  arg_value = 0;
  char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

  // // check palette selector
  // for(int ii=0;ii<WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE;ii++){
  //   sprintf(arg_ctr,"g%d\0",ii);
  //   if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //     mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //     update_all = true; //refresh all
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

  //     switch(ii){
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELSUPDATED_PERCENTAGE");
  //         // from mapped value to real value
  //         arg_value = animation.transition.PROGMEM pixels_to_update_as_percentage_map[arg_value];
  //         animation.transition.pixels_to_update_as_percentage = constrain(arg_value,0,100);
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage);
  //         // Also convert to number equivalent
  //         animation.transition.pixels_to_update_as_number = map(animation.transition.pixels_to_update_as_percentage,0,100,0,ledout.length);
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER,animation.transition.pixels_to_update_as_number);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELORDER");
  //         animation.transition.order = arg_value; // no map
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"RATE");
  //         animation.transition.rate_ms = arg_value*1000; //seconds to milliseconds
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.rate_ms);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PERIOD");
  //         animation.transition.time_ms = arg_value*1000; //seconds to milliseconds
  //         // If period > rate, increase period to rate
  //         animation.transition.time_ms = animation.transition.time_ms>animation.transition.rate_ms?animation.transition.rate_ms:animation.transition.time_ms;
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms); 
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"TRANSITIONMETHOD");
  //         animation.transition.method = arg_value;
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetTransitionMethodName());      
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"ANIMATIONMODE");
  //         animation.mode = arg_value;
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetAnimationModeName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PALETTE");
  //         animation.palette = arg_value;
  //         AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_EFFECTS: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
  //         flashersettings.function = arg_value;  
  //         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;  //restart animation/function
  //         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "flasher.function = %d"),flashersettings.function);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
  //         mixer.running_id = arg_value;          
  //         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.mode.running_id = %d"),mixer.running_id);
  //       break;
  //       default: 
  //         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "DEFAULT hasParam(\"%s\")=%d"),arg_ctr,arg_value);
  //       break;
  //     } //switch
  //   } //if
  // } //for

  // animation.fForceUpdate = true; // update leds now

} // end function

void mNextionPanel::HandleNextionSettings(void)
{

  // //if (!HttpCheckPriviledgedAccess()) { return; }

  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

  // if (mcl->mweb->pWebServer->hasParam("save")) {
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
  //   WebSave_NextionSettings();
  //   mcl->mweb->HandleRoot(); // return to main menu
  //   return;
  // }

  // // Send Tab name
  // mcl->mweb->WSStartAppend_P(D_NEXTION_SETTINGS);
  // // Send primary handler calls (m=1) and replace scripts
  // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE_TEMPLATEb);
  // // Send functions for callback
  // // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE1b, 
  // //                             1,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
  // //                             1,//animation.transition.order,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
  // //                             1,//(uint8_t)round(animation.transition.rate_ms/1000),    // "ld('rgb_controls?ra=1',x2);" // ra = rate
  // //                             1,//(uint8_t)round(animation.transition.time_ms/1000),    // "ld('rgb_controls?pr=1',x3);" // pr = period time
  // //                             1,//animation.transition.method,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
  // //                             1,//animation.mode,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
  // //                             1,//animation.palette,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
  // //                             1,//flashersettings.function,                             // "ld('rgb_controls?fl=1',x7);" // fl  = flasher  
  // //                             1//mixer.running_id                                 // "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id                          
  // //                           ); // Send lists
  // // Send functions part 2
  // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE2b);
  // // Send style
  // mcl->mweb->WSContentSendStyleb();

  // // Buttons that will stay
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_FIRMWARE, "Firmware");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDREBOOT, "LCD Reboot");

  // mcl->mweb->WSBufferAppend_P(PSTR("<div></div>"));            // 5px padding
  // mcl->mweb->WSBufferAppend_P(PSTR("<div> Debug URLs normally called indirectly</div>"));            // 5px padding

  // // Extra handlers not usually called directly
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_TFTFILESIZE, "TFT File Size");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDDOWNLOAD, "LCD Download");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDOTASUCCESS, "LCD OTA Success");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDOTAFAILURE, "LCD OTA Failure");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDUPLOAD, "LCD Upload");

  // mcl->mweb->WSContentSpaceButton(BUTTON_MAIN);
  // mcl->mweb->WSContentStop();

}



/***
 * Add subscribes for new connection to mqtt
 * Show update on panel
 * */
void mNextionPanel::wifiConnected()
{ // MQTT connection and subscriptions

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));

  // Show connection success
  //nextionSendCmd("page 0");

  // char display_ctr[30];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");

  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "FUNC_WIFI_CONNECTED"));
      
      // nextionSendCmd("page 0");

      // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
      // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
  
      // nextionSetAttr("p[0].b[1].txt", display_ctr);

      // // hide QR code
      // nextionSendCmd("vis 3,0");
      // nextionSendCmd("page 5");
  // hide QR code
  // nextionSendCmd("vis 3,0");

}


void mNextionPanel::wifiDisconnected(){
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
  // char display_ctr[120];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connect to%s\\rFAILED rc=%s\"",WiFi.localIP().toString(),"192.168.1.65",pCONT_mqtt->pubsub->stateCtr());

  // QR code show wifi connect
  // nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\"");
  // // show item
  // nextionSendCmd("vis 3,1");
  
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

  
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "FUNC_WIFI_DISCONNECTED"));

      // nextionSendCmd("page 0");

      // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
      // sprintf(display_ctr,"\"WiFi DISConnected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
  
      // nextionSetAttr("p[0].b[1].txt", display_ctr);

      // // hide QR code
      // nextionSendCmd("vis 3,1");



}






//   if (mdnsEnabled)
//   { // Setup mDNS service discovery if enabled
//     hMDNSService = MDNS.addService(nextionNode, "http", "tcp", 80);
//     // if (debugTelnetEnabled)
//     // {
//     //   MDNS.addService(nextionNode, "telnet", "tcp", 23);
//     // }
//     MDNS.addServiceTxt(hMDNSService, "app_name", "HASwitchPlate");
//     MDNS.addServiceTxt(hMDNSService, "app_version", String(nextionVersion).c_str());
//     MDNS.update();
//   }

//   if ((configPassword[0] != '\0') && (configUser[0] != '\0'))
//   { // Start the webserver with our assigned password if it's been configured...
//     httpOTAUpdate.setup(&webServer, "/update", configUser, configPassword);
//   }
//   else
//   { // or without a password if not
//     httpOTAUpdate.setup(&webServer, "/update");
//   }

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNotFound()
{ 
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNotFound"));
  // // webServer 404
  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending 404 to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = "File Not Found\n\n";
  // httpMessage += "URI: ";
  // httpMessage += mcl->mweb->pWebServer->uri();
  // httpMessage += "\nMethod: ";
  // httpMessage += (mcl->mweb->pWebServer->method() == HTTP_GET) ? "GET" : "POST";
  // httpMessage += "\nArguments: ";
  // httpMessage += mcl->mweb->pWebServer->args();
  // httpMessage += "\n";
  // for (uint8_t i = 0; i < mcl->mweb->pWebServer->args(); i++)
  // {
  //   httpMessage += " " + mcl->mweb->pWebServer->argName(i) + ": " + mcl->mweb->pWebServer->arg(i) + "\n";
  // }
  // mcl->mweb->pWebServer->send(404, "text/plain", httpMessage);
}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleSaveConfig()
{ 
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleSaveConfig"));// http://plate01/saveConfig
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
//     {
//       return mcl->mweb->pWebServer->requestAuthentication();
//     }
//   }
//   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /saveConfig page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
//   String httpMessage = FPSTR(HTTP_HEAD2);
//   httpMessage.replace("{v}", String(nextionNode));
//   httpMessage += FPSTR(HTTP_SCRIPT2);
//   httpMessage += FPSTR(HTTP_STYLE2);
//   httpMessage += String(NEXTION_STYLE);

//   bool shouldSaveWifi = false;
//   // Check required values
//   if (mcl->mweb->pWebServer->arg("wifiSSID") != "" && mcl->mweb->pWebServer->arg("wifiSSID") != String(WiFi.SSID()))
//   { // Handle WiFi update
//     shouldSaveConfig = true;
//     shouldSaveWifi = true;
//     mcl->mweb->pWebServer->arg("wifiSSID").toCharArray(wifiSSID, 32);
//     if (mcl->mweb->pWebServer->arg("wifiPass") != String("**"))
//     {
//       mcl->mweb->pWebServer->arg("wifiPass").toCharArray(wifiPass, 64);
//     }
//   }
//   if (mcl->mweb->pWebServer->arg("mqttServer") != "" && mcl->mweb->pWebServer->arg("mqttServer") != String(mqttServer))
//   { // Handle mqttServer
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttServer").toCharArray(mqttServer, 64);
//   }
//   if (mcl->mweb->pWebServer->arg("mqttPort") != "" && mcl->mweb->pWebServer->arg("mqttPort") != String(mqttPort))
//   { // Handle mqttPort
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttPort").toCharArray(mqttPort, 6);
//   }
//   if (mcl->mweb->pWebServer->arg("nextionNode") != "" && mcl->mweb->pWebServer->arg("nextionNode") != String(nextionNode))
//   { // Handle nextionNode
//     shouldSaveConfig = true;
//     String lowerHaspNode = mcl->mweb->pWebServer->arg("nextionNode");
//     lowerHaspNode.toLowerCase();
//     lowerHaspNode.toCharArray(nextionNode, 16);
//   }
//   if (mcl->mweb->pWebServer->arg("groupName") != "" && mcl->mweb->pWebServer->arg("groupName") != String(groupName))
//   { // Handle groupName
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("groupName").toCharArray(groupName, 16);
//   }
//   // Check optional values
//   if (mcl->mweb->pWebServer->arg("mqttUser") != String(mqttUser))
//   { // Handle mqttUser
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttUser").toCharArray(mqttUser, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("mqttPassword") != String("***"))
//   { // Handle mqttPassword
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttPassword").toCharArray(mqttPassword, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("configUser") != String(configUser))
//   { // Handle configUser
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("configUser").toCharArray(configUser, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("configPassword") != String("****"))
//   { // Handle configPassword
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("configPassword").toCharArray(configPassword, 32);
//   }
// //   if (mcl->mweb->pWebServer->arg("motionPinConfig") != String(motionPinConfig))
// //   { // Handle motionPinConfig
// //     shouldSaveConfig = true;
// //     mcl->mweb->pWebServer->arg("motionPinConfig").toCharArray(motionPinConfig, 3);
// //   }
//   if ((mcl->mweb->pWebServer->arg("debugSerialEnabled") == String("on")) && !debugSerialEnabled)
//   { // debugSerialEnabled was disabled but should now be enabled
//     shouldSaveConfig = true;
//     debugSerialEnabled = true;
//   }
//   else if ((mcl->mweb->pWebServer->arg("debugSerialEnabled") == String("")) && debugSerialEnabled)
//   { // debugSerialEnabled was enabled but should now be disabled
//     shouldSaveConfig = true;
//     debugSerialEnabled = false;
//   }
// //   if ((mcl->mweb->pWebServer->arg("debugTelnetEnabled") == String("on")) && !debugTelnetEnabled)
// //   { // debugTelnetEnabled was disabled but should now be enabled
// //     shouldSaveConfig = true;
// //     debugTelnetEnabled = true;
// //   }
// //   else if ((mcl->mweb->pWebServer->arg("debugTelnetEnabled") == String("")) && debugTelnetEnabled)
// //   { // debugTelnetEnabled was enabled but should now be disabled
// //     shouldSaveConfig = true;
// //     debugTelnetEnabled = false;
// //   }
//   if ((mcl->mweb->pWebServer->arg("mdnsEnabled") == String("on")) && !mdnsEnabled)
//   { // mdnsEnabled was disabled but should now be enabled
//     shouldSaveConfig = true;
//     mdnsEnabled = true;
//   }
//   else if ((mcl->mweb->pWebServer->arg("mdnsEnabled") == String("")) && mdnsEnabled)
//   { // mdnsEnabled was enabled but should now be disabled
//     shouldSaveConfig = true;
//     mdnsEnabled = false;
//   }

//   if (shouldSaveConfig)
//   { // Config updated, notify user and trigger write to SPIFFS
//     httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String(nextionNode) + String(F("</h1>"));
//     httpMessage += String(F("<br/>Saving updated configuration values and restarting device"));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

//     configSave();
//     if (shouldSaveWifi)
//     {
//       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"CONFIG: Attempting connection to SSID: ")) + mcl->mweb->pWebServer->arg("wifiSSID"));
//       espWifiSetup();
//     }
//     espReset();
//   }
//   else
//   { // No change found, notify user and link back to config page
//     httpMessage += String(F("<meta http-equiv='refresh' content='3;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String(nextionNode) + String(F("</h1>"));
//     httpMessage += String(F("<br/>No changes found, returning to <a href='/'>home page</a>"));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
//   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleResetConfig()
{ // http://plate01/resetConfig
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleResetConfig"));
  //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /resetConfig page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // //httpMessage.replace("{v}", String("nextionNode"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);

  // if (mcl->mweb->pWebServer->arg("confirm") == "yes")
  // { // User has confirmed, so reset everything
  //   httpMessage += String(F("<h1>"));
  //   httpMessage += String("nextionNode");
  //   httpMessage += String(F("</h1><b>Resetting all saved settings and restarting device into WiFi AP mode</b>"));
  //   httpMessage += FPSTR(HTTP_END2);
  //   mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  //   delay(1000);
  //   configClearSaved();
  // }
  // else
  // {
  //   httpMessage += String(F("<h1>Warning</h1><b>This process will reset all settings to the default values and restart the device.  You may need to connect to the WiFi AP displayed on the panel to re-configure the device before accessing it again."));
  //   httpMessage += String(F("<br/><hr><br/><form method='get' action='resetConfig'>"));
  //   httpMessage += String(F("<br/><br/><button type='submit' name='confirm' value='yes'>reset all settings</button></form>"));
  //   httpMessage += String(F("<br/><hr><br/><form method='get' action='/'>"));
  //   httpMessage += String(F("<button type='submit'>return home</button></form>"));
  //   httpMessage += FPSTR(HTTP_END2);
  //   mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNextionFirmware()
{ 
//   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNextionFirmware"));
  
// const char HTTP_FORM_UPG2[] =
//   "<div id='f1' name='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
//   "<form method='get' action='u10'>"
//   "<br/><b>" D_OTA_URL "</b><br/><input id='o' name='o' placeholder='OTA_URL' value='%s'><br/>"
//   "<br/><button type='submit'>" D_START_UPGRADE "</button></form>"
//   "</fieldset><br/><br/>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";

// const char HTTP_FORM_RST_UPG2[] =
//   "<form method='post' action='u20' enctype='multipart/form-data'>"
//   "<br/><input type='file' name='u20'><br/>"
//   "<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
//   "</fieldset>"
//   "</div>";

//   mcl->mweb->WSStartAppend_P("Firmware Upgrade");
//   mcl->mweb->WSContentSendStyle();
//   mcl->mweb->WSBufferAppend_P(HTTP_FORM_UPG2, mcl->mset->Settings.ota_url);
//   mcl->mweb->WSBufferAppend_P(HTTP_FORM_RST_UPG2, D_UPGRADE);
//   mcl->mweb->WSContentSpaceButton(BUTTON_MAIN);
//   mcl->mweb->WSContentStop();

}


void mNextionPanel::webHandleNextionFirmware_PhaseOut()
{ 
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "void mNextionPanel::webHandleNextionFirmware_PhaseOut()"));
  // // http://plate01/firmware
  // // if (configPassword[0] != '\0')
  // // { //Request HTTP auth if configPassword is set
  // //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  // //   {
  // //     return mcl->mweb->pWebServer->requestAuthentication();
  // //   }
  // // }
  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /firmware page to client connected from: "))) ;//mcl->mweb->pWebServer->client().remoteIP().toString());

  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNextionFirmware"));
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // //httpMessage.replace("{v}", (String("nextionNode") + " update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" firmware</h1>"));

  // // Display main firmware page
  // // HTTPS Disabled pending resolution of issue: https://github.com/esp8266/Arduino/issues/4696
  // // Until then, using a proxy host at http://haswitchplate.com to deliver unsecured firmware images from GitHub
  // httpMessage += String(F("<form method='get' action='/espfirmware'>"));
  // if (updateEspAvailable)
  // {
  //   httpMessage += String(F("<font color='green'><b>NEXTION ESP8266 update available!</b></font>"));
  // }
  // httpMessage += String(F("<br/><b>Update ESP8266 from URL</b><small><i> http only</i></small>"));
  // httpMessage += String(F("<br/><input id='espFirmwareURL' name='espFirmware' value='")) + espFirmwareUrl + "'>";
  // httpMessage += String(F("<br/><br/><button type='submit'>Update ESP from URL</button></form>"));

  // httpMessage += String(F("<br/><form method='POST' action='/update' enctype='multipart/form-data'>"));
  // httpMessage += String(F("<b>Update ESP8266 from file</b><input type='file' id='espSelect' name='espSelect' accept='.bin'>"));
  // httpMessage += String(F("<br/><br/><button type='submit' id='espUploadSubmit' onclick='ackEspUploadSubmit()'>Update ESP from file</button></form>"));

  // httpMessage += String(F("<br/><br/><hr><h1>WARNING!</h1>"));
  // httpMessage += String(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the NEXTION will need to be manually power cycled which might mean a trip to the breaker box. "));
  // httpMessage += String(F("After a power cycle, the LCD will display an error message until a successful firmware update has completed.<br/>"));

  // httpMessage += String(F("<br/><hr><form method='get' action='lcddownload'>"));
  // if (updateLcdAvailable)
  // {
  //   httpMessage += String(F("<font color='green'><b>NEXTION LCD update available!</b></font>"));
  // }
  // httpMessage += String(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  // httpMessage += String(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='")) + lcdFirmwareUrl + "'>";
  // httpMessage += String(F("<br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  // httpMessage += String(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  // httpMessage += String(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  // httpMessage += String(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

  // // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
  // httpMessage += String(F("<script>function handleLcdFileSelect(evt) {"));
  // httpMessage += String(F("var uploadFile = evt.target.files[0];"));
  // httpMessage += String(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
  // httpMessage += String(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
  // httpMessage += String(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
  // httpMessage += String(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
  // httpMessage += String(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
  // httpMessage += String(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
  // httpMessage += String(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
  // httpMessage += String(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  // httpMessage += FPSTR(HTTP_END2);

  // Serial.println(httpMessage);
  
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleEspFirmware()
{ 
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleEspFirmware"));
  // http://plate01/espfirmware
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }

  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /espfirmware page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " ESP update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='60;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>"));
  // httpMessage += String("nextionNode") + " ESP update";
  // httpMessage += String(F("</h1>"));
  // httpMessage += "<br/>Updating ESP firmware from: " + String(mcl->mweb->pWebServer->arg("espFirmware"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // //debugPrintln("ESPFW: Attempting ESP firmware update from: " + String(mcl->mweb->pWebServer->arg("espFirmware")));
  // espStartOta(mcl->mweb->pWebServer->arg("espFirmware"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpload()
{ // http://plate01/lcdupload
  // Upload firmware to the Nextion LCD via HTTP upload

//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "webHandleLcdUpload"));

//   // if (configPassword[0] != '\0')
//   // { //Request HTTP auth if configPassword is set
//   //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
//   //   {
//   //     return mcl->mweb->pWebServer->requestAuthentication();
//   //   }
//   // }

//   static uint32_t lcdOtaTransferred = 0;
//   static uint32_t lcdOtaRemaining;
//   static uint16_t lcdOtaParts;
//   const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
//   static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

//   HTTPUpload &upload = mcl->mweb->pWebServer->upload();

//   if (tftFileSize == 0)
//   {
    
//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "LCD OTA: FAILED, no filesize sent."));
  
//     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: FAILED, no filesize sent.")));
//     String httpMessage = FPSTR(HTTP_HEAD2);
//     // httpMessage.replace("{v}", (String("nextionNode") + " LCD update"));
//     httpMessage += FPSTR(HTTP_SCRIPT2);
//     httpMessage += FPSTR(HTTP_STYLE2);
//     httpMessage += String(NEXTION_STYLE);
//     httpMessage += String(F("<meta http-equiv='refresh' content='5;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String("nextionNode") + " LCD update FAILED</h1>";
//     httpMessage += String(F("No update file size reported.  You must use a modern browser with Javascript enabled."));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
//   }
//   else if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
//   { // Our timer expired so reset
  
//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: LCD upload timeout.  Restarting.");
//    //debugPrintln(F("LCD OTA: ERROR: LCD upload timeout.  Restarting."));
//     //espReset();
//   }
//   else if (upload.status == UPLOAD_FILE_START)
//   {
//     //WiFiUDP::stopAll(); // Keep mDNS responder from breaking things

//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: LCD upload timeout.  Restarting.");
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Attempting firmware upload");
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%s"),"LCD OTA: upload.filename: ",upload.filename.c_str());
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d"),"LCD OTA: TFTfileSize: ",tftFileSize);

//     lcdOtaRemaining = tftFileSize;
//     lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: File upload beginning. Size ",lcdOtaRemaining," bytes in ",lcdOtaParts," 4k chunks.");

//     serial_print_suffix(); // Send empty command to LCD

//     nextionHandleInput();

//     String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + ",38400,0";
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%s"),"LCD OTA: Sending LCD upload command: ",lcdOtaNextionCmd.c_str());

//     char lcdOtaNextionCmd_ctr[100];
//     sprintf(lcdOtaNextionCmd_ctr,"whmi-wri %d,38400,0",tftFileSize);

//     serial_print_suffixed(lcdOtaNextionCmd_ctr);
    
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),lcdOtaNextionCmd_ctr);
    
//     if (nextionOtaResponse()){
//       AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: LCD upload command accepted");
//     }else{
//       AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: LCD upload command FAILED.");
//       espReset();
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_WRITE)
//   { // Handle upload data
//     static int lcdOtaChunkCounter = 0;
//     static uint16_t lcdOtaPartNum = 0;
//     static int lcdOtaPercentComplete = 0;
//     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
//     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
//     uint16_t lcdOtaUploadIndex = 0;
//     int32_t lcdOtaPacketRemaining = upload.currentSize;

//     while (lcdOtaPacketRemaining > 0)
//     { // Write incoming data to panel as it arrives
//       // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
//       uint16_t lcdOtaChunkSize = 0;
//       if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter))){
//         lcdOtaChunkSize = lcdOtaPacketRemaining;
//       }else 
//       if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter))){
//         lcdOtaChunkSize = lcdOtaBufferSize;
//       }else
//       {
//         lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
//       }

//       for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
//       { // Load up the UART buffer
//         lcdOtaBuffer[i] = upload.buf[lcdOtaUploadIndex];
//         lcdOtaUploadIndex++;
//       }

//       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "len=%d"),lcdOtaChunkSize);

//       #ifdef USE_NEXTION_SOFTWARE_SERIAL
//         swSer->flush();                              // Clear out current UART buffer
//         swSer->write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
//       #else
//         SERIAL_NEXTION_TX.flush();                              // Clear out current UART buffer
//         SERIAL_NEXTION_TX.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
//       #endif

//       lcdOtaChunkCounter += lcdOtaChunkSize;
//       lcdOtaTransferred += lcdOtaChunkSize;
//       if (lcdOtaChunkCounter >= 4096)
//       {
//   //#define USE_NEXTION_SOFTWARE_SERIAL
// #ifdef USE_NEXTION_SOFTWARE_SERIAL
//         swSer->flush();
//         #else
//         SERIAL_NEXTION_TX.flush();
//         #endif


//         lcdOtaPartNum++;
//         lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
//         lcdOtaChunkCounter = 0;
//         if (nextionOtaResponse())
//         {
//           AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: Part ",lcdOtaPartNum," OK, ",lcdOtaPercentComplete,"% complete");
//         }
//         else
//         {
//           AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: Part ",lcdOtaPartNum," FAILED, ",lcdOtaPercentComplete,"% complete");
//           AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%"),"EXIT EARLY?");

//         }
//       }
//       else
//       {
//         delay(10);
//       }
//       if (lcdOtaRemaining > 0)
//       {
//         lcdOtaRemaining -= lcdOtaChunkSize;
//       }
//       if (lcdOtaPacketRemaining > 0)
//       {
//         lcdOtaPacketRemaining -= lcdOtaChunkSize;
//       }
//     }

//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       {
//         //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaSuccess");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//       else
//       {
//         //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_END)
//   { // Upload completed
//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       { // YAY WE DID IT
//         //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaSuccess");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//       else
//       {
//         //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//     }
//   }
//   else if (upload.status == UPLOAD_FILE_ABORTED)
//   { // Something went kablooey
//     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
//     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//     mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//     mcl->mweb->pWebServer->send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       mcl->mweb->pWebServer->handleClient();
//       delay(1);
//     }
//     espReset();
//   }
//   else
//   { // Something went weird, we should never get here...
//     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: upload.status returned: ")) + String(upload.status));
//     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//     mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//     mcl->mweb->pWebServer->send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       mcl->mweb->pWebServer->handleClient();
//       delay(1);
//     }
//     espReset();
//   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateSuccess()
{ // http://plate01/lcdOtaSuccess
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // // }
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdUpdateSuccess"));
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcdOtaSuccess page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update success"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" LCD update success</h1>"));
  // httpMessage += String(F("Restarting HASwitchPlate to apply changes..."));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateFailure()
{ // http://plate01/lcdOtaFailure
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdUpdateFailure"));
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcdOtaFailure page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update failed"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" LCD update failed :(</h1>"));
  // httpMessage += String(F("Restarting HASwitchPlate to reset device..."));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "httpMessage=%s"),httpMessage.c_str());

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdDownload()
{ // http://plate01/lcddownload
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdDownload"));
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcddownload page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>"));
  // httpMessage += String("nextionNode") + " LCD update";
  // httpMessage += String(F("</h1>"));
  // httpMessage += "<br/>Updating LCD firmware from: " + String(mcl->mweb->pWebServer->arg("lcdFirmware"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // nextionStartOtaDownload(mcl->mweb->pWebServer->arg("lcdFirmware"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleReboot()
{ // http://plate01/reboot
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // // }
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleReboot"));
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /reboot page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " NEXTION reboot"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='10;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F("</h1>"));
  // httpMessage += String(F("<br/>Rebooting device"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"RESET: Rebooting device");
  // nextionSendCmd("page 0");
  // nextionSetAttr("p[0].b[1].txt", "\"Rebooting...\"");
  // // espReset();
}


// String mNextionPanel::debugPrintln(String str){
//   Serial.println(str);
// }



void mNextionPanel::HandleUpgradeFirmwareStart(void)
{
  // Upload via URL


  // //if (!HttpCheckPriviledgedAccess()) { return; }

  // char command[sizeof(mcl->mset->Settings.ota_url) + 10];  // OtaUrl

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  // mcl->mwif->WifiConfigCounter();

  // char otaurl[sizeof(mcl->mset->Settings.ota_url)];
  // WebGetArg(request,"o", otaurl, sizeof(otaurl));
  // if (strlen(otaurl)) {
  //   snprintf_P(command, sizeof(command), PSTR(D_JSON_OTAURL " %s"), otaurl);
  //   ExecuteWebCommand(command, SRC_WEBGUI);
  // }

  // WSStartAppend_P(PM_INFORMATION);
  // WSBufferAppend_P(HTTP_SCRIPT_RELOAD_OTA);
  // WSContentSendStyle();
  // WSBufferAppend_P(PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
  // WSBufferAppend_P(HTTP_MSG_RSTRT);
  // WSContentSpaceButton(BUTTON_MAIN);
  // WSContentStop();

  // snprintf_P(command, sizeof(command), PSTR(D_JSON_UPGRADE " 1"));
  // ExecuteWebCommand(command, SRC_WEBGUI);

}

void mNextionPanel::HandleUploadDone(void)
{
//   if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  // mcl->mwif->WifiConfigCounter();
  // mcl->mset->restart_flag = 0;
  //MqttRetryCounter(0);

//   mcl->mwif->WSStartAppend_P(PM_INFORMATION);
//   if (!upload_error) {
//     mcl->mwif->WSBufferAppend_P(HTTP_SCRIPT_RELOAD_OTA);  // Refesh main web ui after OTA upgrade
//   }
//   mcl->mwif->WSContentSendStyle();
//   mcl->mwif->WSBufferAppend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
//   if (upload_error) {
// //    WSBufferAppend_P(PSTR(COLOR_TEXT_WARNING "'>" D_FAILED "</font></b><br/><br/>"));
//     mcl->mwif->WSBufferAppend_P(PSTR("%06x'>" D_FAILED "</font></b><br/><br/>"), mcl->msup->WebColor(mcl->mset->COL_TEXT_WARNING));
// #ifdef USE_RF_FLASH
//     if (upload_error < 14) {
// #else
//     if (upload_error < 10) {
// #endif
//       mcl->msup->GetTextIndexed_P(error, sizeof(error), upload_error -1, kUploadErrors);
//     } else {
//       snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), upload_error);
//     }
//     mcl->mwif->WSBufferAppend_P(error);
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_UPLOAD ": %s"), error);
//     mcl->mset->stop_flash_rotate = mcl->mset->Settings.flag_system.stop_flash_rotate;
//   } else {
//     mcl->mwif->WSBufferAppend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), mcl->msup->WebColor(mcl->mset->COL_TEXT_SUCCESS));
//     mcl->mwif->WSBufferAppend_P(HTTP_MSG_RSTRT);
//     mcl->mwif->ShowWebSource(SRC_WEBGUI);
//     mcl->mset->restart_flag = 2;  // Always restart to re-enable disabled features during update
//   }
//   //SettingsBufferFree();
//   mcl->mwif->WSBufferAppend_P(PSTR("</div><br/>"));
//   mcl->mwif->WSContentSpaceButton(BUTTON_MAIN);
//   mcl->mwif->WSContentStop();

}

void mNextionPanel::HandleUploadLoop(void)
{

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_UPLOAD ": %s"), "HandleUploadLoop");

//   // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
//   bool _serialoutput = (LOG_LEVEL_DEBUG <= mcl->mset->seriallog_level);

//   if (HTTP_USER == webserver_state) { return; }
//   if (upload_error) {
    
//     #ifdef ESP8266
//       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
//     #endif
//     return;
//   }

//   HTTPUpload& upload = pWebServer->upload();

//   if (UPLOAD_FILE_START == upload.status) {
//     mcl->mset->restart_flag = 60;
//     if (0 == upload.filename.c_str()[0]) {
//       upload_error = 1;  // No file selected
//       return;
//     }
//     //SettingsSave(1);  // Free flash for upload
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
//     if (UPL_SETTINGS == upload_file_type) {
//       if (!mcl->mset->SettingsBufferAlloc()) {
//         upload_error = 2;  // Not enough space
//         return;
//       }
//     } else {
//      // MqttRetryCounter(60);
// #ifdef USE_EMULATION
//       //UdpDisconnect();
// #endif  // USE_EMULATION
// #ifdef USE_ARILUX_RF
//       //AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
// #endif  // USE_ARILUX_RF
//       //if (mcl->mset->Settings.flag_system.mqtt_enabled) MqttDisconnect();
//       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      
//           #ifdef ESP8266
//       if (!Update.begin(maxSketchSpace)) {         //start with max available size

// //        if (_serialoutput) Update.printError(Serial);
// //        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
// //          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
// //        }

//         upload_error = 2;  // Not enough space
//         return;
//       }
//       #endif
//     }
//     upload_progress_dot_count = 0;
//   } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
//     if (0 == upload.totalSize) {
//       if (UPL_SETTINGS == upload_file_type) {
//         config_block_count = 0;
//       }
//       else {
// // #ifdef USE_RF_FLASH
// //         if ((SONOFF_BRIDGE == mcl->mset->my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
// //           Update.end();              // End esp8266 update session
// //           upload_file_type = UPL_EFM8BB1;
// //
// //           upload_error = SnfBrUpdateInit();
// //           if (upload_error != 0) { return; }
// //         } else
// // #endif  // USE_RF_FLASH
//         {
//           if (upload.buf[0] != 0xE9) {
//             upload_error = 3;  // Magic byte is not 0xE9
//             return;
//           }
//           uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
//           #ifdef ESP8266
//           if(bin_flash_size > ESP.getFlashChipRealSize()) {
//             upload_error = 4;  // Program flash size is larger than real flash size
//             return;
//           }
//           #endif
// //          upload.buf[2] = 3;  // Force DOUT - ESP8285
//         }
//       }
//     }
//     if (UPL_SETTINGS == upload_file_type) {
//       if (!upload_error) {
//         if (upload.currentSize > (sizeof(mcl->mset->Settings) - (config_block_count * HTTP_UPLOAD_BUFLEN))) {
//           upload_error = 9;  // File too large
//           return;
//         }
//         memcpy(mcl->mset->settings_buffer + (config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
//         config_block_count++;
//       }
//     }
// // #ifdef USE_RF_FLASH
// //     else if (UPL_EFM8BB1 == upload_file_type) {
// //       if (efm8bb1_update != nullptr) {    // We have carry over data since last write, i. e. a start but not an end
// //         ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
// //         free(efm8bb1_update);
// //         efm8bb1_update = nullptr;
// //         if (result != 0) {
// //           upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid
// //           return;
// //         }
// //       }
// //       ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
// //       if (result < 0) {
// //         upload_error = abs(result);
// //         return;
// //       } else if (result > 0) {
// //         if ((size_t)result > upload.currentSize) {
// //           // Offset is larger than the buffer supplied, this should not happen
// //           upload_error = 9;  // File too large - Failed to decode RF firmware
// //           return;
// //         }
// //         // A remnant has been detected, allocate data for it plus a null termination byte
// //         size_t remnant_sz = upload.currentSize - result;
// //         efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
// //         if (efm8bb1_update == nullptr) {
// //           upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
// //           return;
// //         }
// //         memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
// //         // Add null termination at the end of of remnant buffer
// //         efm8bb1_update[remnant_sz] = '\0';
// //       }
// //     }
// // #endif  // USE_RF_FLASH
//     else {  // firmware
    
//           #ifdef ESP8266
//       if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
//         upload_error = 5;  // Upload buffer miscompare
//         return;
//       }
//       #endif
//       if (_serialoutput) {
//         Serial.printf(".");
//         upload_progress_dot_count++;
//         if (!(upload_progress_dot_count % 80)) { Serial.println(); }
//       }
//     }
//   } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
//     if (_serialoutput && (upload_progress_dot_count % 80)) {
//       Serial.println();
//     }
//     if (UPL_SETTINGS == upload_file_type) {
//       if (config_xor_on_set) {
//         for (uint16_t i = 2; i < sizeof(mcl->mset->Settings); i++) {
//           mcl->mset->settings_buffer[i] ^= (config_xor_on_set +i);
//         }
//       }
//       bool valid_settings = false;
//       unsigned long buffer_version = mcl->mset->settings_buffer[11] << 24 | mcl->mset->settings_buffer[10] << 16 | mcl->mset->settings_buffer[9] << 8 | mcl->mset->settings_buffer[8];
//       if (buffer_version > 0x06000000) {
//         uint16_t buffer_size = mcl->mset->settings_buffer[3] << 8 | mcl->mset->settings_buffer[2];
//         uint16_t buffer_crc = mcl->mset->settings_buffer[15] << 8 | mcl->mset->settings_buffer[14];
//         uint16_t crc = 0;
//         for (uint16_t i = 0; i < buffer_size; i++) {
//           if ((i < 14) || (i > 15)) { crc += mcl->mset->settings_buffer[i]*(i+1); }  // Skip crc
//         }
//         valid_settings = (buffer_crc == crc);
//       } else {
//         valid_settings = (mcl->mset->settings_buffer[0] == CONFIG_FILE_SIGN);
//       }
//       if (valid_settings) {
//         //SystemSettings_DefaultBody();
//         memcpy((char*)&mcl->mset->Settings +16, mcl->mset->settings_buffer +16, sizeof(mcl->mset->Settings) -16);
//         mcl->mset->Settings.version = buffer_version;  // Restore version and auto upgrade after restart
//         //SettingsBufferFree();
//       } else {
//         upload_error = 8;  // File invalid
//         return;
//       }
//     }
// #ifdef USE_RF_FLASH
//     else if (UPL_EFM8BB1 == upload_file_type) {
//       // RF FW flash done
//       upload_file_type = UPL_TASMOTA;
//     }
// #endif  // USE_RF_FLASH
//     else {
//           #ifdef ESP8266
//       if (!Update.end(true)) { // true to set the size to the current progress
//         if (_serialoutput) { Update.printError(Serial); }
//         upload_error = 6;  // Upload failed. Enable logging 3
//         return;
//       }
//       #endif
//     }
//     if (!upload_error) {
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
//     }
//   } else if (UPLOAD_FILE_ABORTED == upload.status) {
//     mcl->mset->restart_flag = 0;
//     //MqttRetryCounter(0);
//     upload_error = 7;  // Upload aborted
//           #ifdef ESP8266
//     if (UPL_TASMOTA == upload_file_type) { Update.end(); }
//     #endif
//   }
//   delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void mNextionPanel::HandlePreflightRequest(void)
{
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  // mcl->mwif->WSSend(200, CT_HTML, "");
}

#endif // USE_MODULE_NETWORK_WEBSERVER


/******************************************************************************************************************
 * SECTION: Update Service
*******************************************************************************************************************/


// OTA updates for display



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espSetupOta()
{ // (mostly) boilerplate OTA setup from library examples

  // ArduinoOTA.setHostname(nextionNode);
  // ArduinoOTA.setPassword(configPassword);

  // ArduinoOTA.onStart([]() {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update start"));
  //   nextionSendCmd("page 0");
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\"");
  // });
  // ArduinoOTA.onEnd([]() {
  //   nextionSendCmd("page 0");
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update complete"));
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rComplete!\"");
  //   //espReset();
  // });
  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rProgress: " + String(progress / (total / 100)) + "%\"");
  // });
  // ArduinoOTA.onError([](ota_error_t error) {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: ERROR code ")) + String(error));
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
bool mNextionPanel::updateCheck()
{ // firmware update check
  // HTTPClient updateClient;
  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Checking update URL: ")) + String(UPDATE_URL));
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
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Update check failed: ")) + updateClient.errorToString(httpCode));
  //   return false;
  // }
  // updateClient.end();

  // DynamicJsonDocument updateJson(2048);
  // DeserializationError jsonError = deserializeJson(updateJson, updatePayload);

  // if (jsonError)
  // { // Couldn't parse the returned JSON, so bail
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: JSON parsing failed: ")) + String(jsonError.c_str()));
  //   return false;
  // }
  // else
  // {
  //   if (!updateJson["d1_mini"]["version"].isNull())
  //   {
  //     updateEspAvailableVersion = updateJson["d1_mini"]["version"].as<float>();
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateEspAvailableVersion: ")) + String(updateEspAvailableVersion));
  //     espFirmwareUrl = updateJson["d1_mini"]["firmware"].as<String>();
  //     if (updateEspAvailableVersion > nextionVersion)
  //     {
  //       updateEspAvailable = true;
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New ESP version available: ")) + String(updateEspAvailableVersion));
  //     }
  //   }
  //   if (nextionModel && !updateJson[nextionModel]["version"].isNull())
  //   {
  //     updateLcdAvailableVersion = updateJson[nextionModel]["version"].as<int>();
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateLcdAvailableVersion: ")) + String(updateLcdAvailableVersion));
  //     lcdFirmwareUrl = updateJson[nextionModel]["firmware"].as<String>();
  //     if (updateLcdAvailableVersion > lcdVersion)
  //     {
  //       updateLcdAvailable = true;
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New LCD version available: ")) + String(updateLcdAvailableVersion));
  //     }
  //   }
  //   //debugPrintln(F("UPDATE: Update check completed"));
  // }
  
  // return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// bool mNextionPanel::nextionOtaResponse()
// { // Monitor the serial port for a 0x05 response within our timeout

//   unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
//   unsigned long nextionCommandTimer = millis(); // record current time for our timeout
//   bool otaSuccessVal = false;
//   while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
//   {

//     if(otatransfererror){
//       AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "QUIT OTA and Restart display"));
//       nextionReset();
//       break;
//     }

  
//       #ifdef USE_NEXTION_SOFTWARE_SERIAL
//       if (swSer->available())
//       {
//       byte inByte = swSer->read();
//       #else 
//       if (Serial.available())
//       {
//       byte inByte = Serial.read();
//       #endif

//       if (inByte == 0x5)
//       {
//         AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"inByte == 0x5");
//         otaSuccessVal = true;
//         otatransfererror = true;
//         break;
//       }
//       else
//       {
//         otatransfererror = false;
// AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%x"),inByte);
//         // //debugPrintln(String(inByte, HEX));
//       }
//     }
//     else
//     {
//       delay(1);
//     }
//   }
//   // delay(50);
//   return otaSuccessVal;
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionStartOtaDownload(String otaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "nextionStartOtaDownload otaurl"));

  // uint32_t lcdOtaFileSize = 0;
  // String lcdOtaNextionCmd;
  // uint32_t lcdOtaChunkCounter = 0;
  // uint16_t lcdOtaPartNum = 0;
  // uint32_t lcdOtaTransferred = 0;
  // uint8_t lcdOtaPercentComplete = 0;
  // const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  // static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Attempting firmware download from: ")) + otaUrl);
  // WiFiClient lcdOtaWifi;
  // HTTPClient lcdOtaHttp;
  // lcdOtaHttp.begin(lcdOtaWifi, otaUrl);
  // int lcdOtaHttpReturn = lcdOtaHttp.GET();
  // if (lcdOtaHttpReturn > 0)
  // { // HTTP header has been sent and Server response header has been handled
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET return code:")) + String(lcdOtaHttpReturn));
  //   if (lcdOtaHttpReturn == HTTP_CODE_OK)
  //   {                                                 // file found at server
  //     int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
  //     lcdOtaFileSize = lcdOtaRemaining;
  //     static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
  //     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
  //     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: File found at Server. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

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
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
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
  //             //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
  //             lcdOtaTimer = millis();
  //           }
  //           else
  //           {
  //             //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
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
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
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
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET failed, error code ")) + lcdOtaHttp.errorToString(lcdOtaHttpReturn));
  //   espReset();
  // }
  // lcdOtaHttp.end();
  
}

// const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNotFound()
{ // webServer 404
  debugPrintln(String(F("HTTP: Sending 404 to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);//FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " 404");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(404, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>404: File Not Found</h1>One of us appears to have done something horribly wrong-><hr/><b>URI: </b>"));
  webServer->sendContent(webServer->uri());
  webServer->sendContent(F("<br/><b>Method: </b>"));
  webServer->sendContent((webServer->method() == HTTP_GET) ? F("GET") : F("POST"));
  webServer->sendContent(F("<br/><b>Arguments: </b>"));
  webServer->sendContent(String(webServer->args()));
  for (uint8_t i = 0; i < webServer->args(); i++)
  {
    webServer->sendContent(F("<br/><b>"));
    webServer->sendContent(String(webServer->argName(i)));
    webServer->sendContent(F(":</b> "));
    webServer->sendContent(String(webServer->arg(i)));
  }
  webServer->sendContent("");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleRoot()
{ // http://plate01/
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  debugPrintln(String(F(DEBUG_INSERT_PAGE_BREAK "HTTP: Sending root page to client connected from: ")) + webServer->client().remoteIP().toString());

  ALOG_INF(PSTR("webHandleRoot();"));

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode));
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent(httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F("</h1>"));

  webServer->sendContent(F("<form method='POST' action='saveConfig'>"));
  webServer->sendContent(F("<b>WiFi SSID</b> <i><small>(required)</small></i><input id='wifiSSID' required name='wifiSSID' maxlength=32 placeholder='WiFi SSID' value='"));
  webServer->sendContent(WiFi.SSID());
  webServer->sendContent(F("'><br/><b>WiFi Password</b> <i><small>(required)</small></i><input id='wifiPass' required name='wifiPass' type='password' maxlength=64 placeholder='WiFi Password' value='********'>"));
  webServer->sendContent(F("<br/><br/><b>HASPone Node Name</b> <i><small>(required. lowercase letters, numbers, and _ only)</small></i><input id='haspNode' required name='haspNode' maxlength=15 placeholder='HASPone Node Name' pattern='[a-z0-9_]*' value='"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F("'><br/><b>Group Name</b> <i><small>(required)</small></i><input id='groupName' required name='groupName' maxlength=15 placeholder='Group Name' value='"));
  webServer->sendContent(groupName);
  webServer->sendContent(F("'><br/><br/><b>MQTT Broker</b> <i><small>(required, IP address is preferred)</small></i><input id='mqttServer' required name='mqttServer' maxlength=127 placeholder='mqttServer' value='"));
  if (strcmp(mqttServer, "") != 0)
  {
    webServer->sendContent(mqttServer);
  }
  webServer->sendContent(F("'><br/><b>MQTT Port</b> <i><small>(required)</small></i><input id='mqttPort' required name='mqttPort' type='number' maxlength=5 placeholder='mqttPort' value='"));
  if (strcmp(mqttPort, "") != 0)
  {
    webServer->sendContent(mqttPort);
  }
  webServer->sendContent(F("'><br/><b>MQTT User</b> <i><small>(optional)</small></i><input id='mqttUser' name='mqttUser' maxlength=127 placeholder='mqttUser' value='"));
  if (strcmp(mqttUser, "") != 0)
  {
    webServer->sendContent(mqttUser);
  }
  webServer->sendContent(F("'><br/><b>MQTT Password</b> <i><small>(optional)</small></i><input id='mqttPassword' name='mqttPassword' type='password' maxlength=127 placeholder='mqttPassword' value='"));
  if (strcmp(mqttUser, "") != 0)
  {
    webServer->sendContent(F("********"));
  }
  webServer->sendContent(F("'>"));

  webServer->sendContent(F("<br/><b>MQTT TLS enabled:</b><input id='mqttTlsEnabled' name='mqttTlsEnabled' type='checkbox'"));
  if (mqttTlsEnabled)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><b>MQTT TLS Fingerpint</b> <i><small>(leave blank to disable fingerprint checking)</small></i><input id='mqttFingerprint' name='mqttFingerprint' maxlength=59 minlength=59 placeholder='01:02:03:04:05:06:07:08:09:0A:0B:0C:0D:0E:0F:10:11:12:13:14' value='"));
  if (strcmp(mqttFingerprint, "") != 0)
  {
    webServer->sendContent(mqttFingerprint);
  }
  webServer->sendContent(F("'>"));

  webServer->sendContent(F("<br/><br/><b>HASPone Admin Username</b> <i><small>(optional)</small></i><input id='configUser' name='configUser' maxlength=31 placeholder='Admin User' value='"));
  if (strcmp(configUser, "") != 0)
  {
    webServer->sendContent(configUser);
  }
  webServer->sendContent(F("'><br/><b>HASPone Admin Password</b> <i><small>(optional)</small></i><input id='configPassword' name='configPassword' type='password' maxlength=31 placeholder='Admin User Password' value='"));
  if (strcmp(configPassword, "") != 0)
  {
    webServer->sendContent(F("********"));
  }
  webServer->sendContent(F("'><br/><br/><b>Home Assistant discovery topic</b> <i><small>(required, should probably be \"homeassistant\")</small></i><input id='hassDiscovery' name='hassDiscovery' maxlength=127 placeholder='homeassistant' value='"));
  if (strcmp(hassDiscovery, "") != 0)
  {
    webServer->sendContent(hassDiscovery);
  }
  webServer->sendContent(F("'><br/><b>Nextion project page count</b> <i><small>(required, probably \"11\")</small></i><input id='nextionMaxPages' required name='nextionMaxPages' type='number' maxlength=2 placeholder='nextionMaxPages' value='"));
  if (nextionMaxPages != 0)
  {
    webServer->sendContent(String(nextionMaxPages));
  }
  webServer->sendContent(F("'><br/><hr>"));
  // Big menu of possible serial speeds
  if ((lcdVersion != 1) && (lcdVersion != 2))
  { // HASPone lcdVersion 1 and 2 have `bauds=115200` in the pre-init script of page 0.  Don't show this option if either of those two versions are running.
    webServer->sendContent(F("<b>LCD Serial Speed:&nbsp;</b><select id='nextionBaud' name='nextionBaud'>"));

uint8_t nextionSpeedsLength = 5;
    for (unsigned int nextionSpeedsIndex = 0; nextionSpeedsIndex < nextionSpeedsLength; nextionSpeedsIndex++)
    {
      char nextionSpeedItem[sizeof(unsigned long) * 3 + 2];
      snprintf(nextionSpeedItem, sizeof nextionSpeedItem, "%ld", nextionSpeedsIndex);//nextionSpeeds[nextionSpeedsIndex]);
      webServer->sendContent(F("<option value='"));
      webServer->sendContent(nextionSpeedItem);
      webServer->sendContent(F("'"));
      if (strcmp(nextionSpeedItem, nextionBaud) == 0)
      {
        webServer->sendContent(F(" selected"));
      }
      webServer->sendContent(F(">"));
      webServer->sendContent(nextionSpeedItem);
      webServer->sendContent(F("</option>"));
    }
    webServer->sendContent(F("</select><br/>"));
  }

  webServer->sendContent(F("<b>USB serial debug output enabled:</b><input id='debugSerialEnabled' name='debugSerialEnabled' type='checkbox'"));
  if (debugSerialEnabled)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><b>Telnet debug output enabled:</b><input id='debugTelnetEnabled' name='debugTelnetEnabled' type='checkbox'"));
  if (debugTelnetEnabled)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><b>mDNS enabled:</b><input id='mdnsEnabled' name='mdnsEnabled' type='checkbox'"));
  if (mdnsEnabled)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><b>Motion Sensor Pin:&nbsp;</b><select id='motionPinConfig' name='motionPinConfig'><option value='0'"));
  if (!motionPin)
  {
    webServer->sendContent(F(" selected"));
  }
  webServer->sendContent(F(">disabled/not installed</option><option value='D0'"));
  // if (motionPin == D0)
  // {
  //   webServer->sendContent(F(" selected"));
  // }
  // webServer->sendContent(F(">D0</option><option value='D1'"));
  // if (motionPin == D1)
  // {
  //   webServer->sendContent(F(" selected"));
  // }
  webServer->sendContent(F(">D1</option></select>"));
  webServer->sendContent(F("<br/><b>Keypress beep enabled on D2:</b><input id='beepEnabled' name='beepEnabled' type='checkbox'"));
  if (beepEnabled)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><b>Ignore touchevents when backlight is off:</b><input id='ignoreTouchWhenOff' name='ignoreTouchWhenOff' type='checkbox'"));
  if (ignoreTouchWhenOff)
  {
    webServer->sendContent(F(" checked='checked'"));
  }
  webServer->sendContent(F("><br/><hr><button type='submit'>save settings</button></form>"));

  if (updateEspAvailable)
  {
    String espFirmwareUrl = "espFirmwareUrl";
    webServer->sendContent(F("<br/><hr><font color='green'><center><h3>HASPone Update available!</h3></center></font>"));
    webServer->sendContent(F("<form method='get' action='espfirmware'>"));
    webServer->sendContent(F("<input id='espFirmwareURL' type='hidden' name='espFirmware' value='"));
    webServer->sendContent(espFirmwareUrl);
    webServer->sendContent(F("'><button type='submit'>update HASPone to v"));
    webServer->sendContent(String(updateEspAvailableVersion));
    webServer->sendContent(F("</button></form>"));
  }

  webServer->sendContent(F("<hr><form method='get' action='firmware'>"));
  webServer->sendContent(F("<button type='submit'>update firmware</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='reboot'>"));
  webServer->sendContent(F("<button type='submit'>reboot device</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='resetBacklight'>"));
  webServer->sendContent(F("<button type='submit'>reset lcd backlight</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='resetConfig'>"));
  webServer->sendContent(F("<button type='submit'>factory reset settings</button></form>"));

  webServer->sendContent(F("<hr><b>MQTT Status: </b>"));
  // if (mqttClient.connected())
  // { // Check MQTT connection
  //   webServer->sendContent(F("Connected"));
  // }
  // else
  // {
    webServer->sendContent(F("<font color='red'><b>Disconnected</b></font><br/><b>MQTT return code:</b> "));
    webServer->sendContent("String(mqttClient.returnCode())");
    webServer->sendContent(F("<br/><b>MQTT last error:</b> "));
    webServer->sendContent("String(mqttClient.lastError())");
    webServer->sendContent(F("<br/><b>MQTT broker ping check:</b> "));
    if (mqttPingCheck)
    {
      webServer->sendContent(F("<font color='green'>SUCCESS</font>"));
    }
    else
    {
      webServer->sendContent(F("<font color='red'>FAILED</font>"));
    }
    webServer->sendContent(F("<br/><b>MQTT broker port check:</b> "));
    if (mqttPortCheck)
    {
      webServer->sendContent(F("<font color='green'>SUCCESS</font>"));
    }
    else
    {
      webServer->sendContent(F("<font color='red'>FAILED</font>"));
    }
  // }
  webServer->sendContent(F("<br/><b>MQTT ClientID: </b>"));
  if (mqttClientId != "")
  {
    webServer->sendContent(mqttClientId);
  }
  webServer->sendContent(F("<br/><b>HASPone FW Version: </b>"));
  webServer->sendContent(String(haspVersion));
  webServer->sendContent(F("<br/><b>LCD Model: </b>"));
  if (nextionModel != "")
  {
    webServer->sendContent(nextionModel);
  }
  webServer->sendContent(F("<br/><b>LCD FW Version: </b>"));
  webServer->sendContent(String(lcdVersion));
  webServer->sendContent(F("<br/><b>LCD Active Page: </b>"));
  webServer->sendContent(String(nextionActivePage));
  webServer->sendContent(F("<br/><b>LCD Serial Speed: </b>"));
  webServer->sendContent(nextionBaud);
  webServer->sendContent(F("<br/><b>CPU Frequency: </b>"));
  webServer->sendContent(String(ESP.getCpuFreqMHz()));
  webServer->sendContent(F("MHz"));
  webServer->sendContent(F("<br/><b>Sketch Size: </b>"));
  webServer->sendContent(String(ESP.getSketchSize()));
  webServer->sendContent(F(" bytes"));
  webServer->sendContent(F("<br/><b>Free Sketch Space: </b>"));
  webServer->sendContent(String(ESP.getFreeSketchSpace()));
  webServer->sendContent(F(" bytes"));
  webServer->sendContent(F("<br/><b>Heap Free: </b>"));
  webServer->sendContent(String(ESP.getFreeHeap()));
  // webServer->sendContent(F("<br/><b>Heap Fragmentation: </b>"));
  // webServer->sendContent(String(ESP.getHeapFragmentation()));
  // webServer->sendContent(F("<br/><b>ESP core version: </b>"));
  // webServer->sendContent(ESP.getCoreVersion());
  webServer->sendContent(F("<br/><b>IP Address: </b>"));
  webServer->sendContent(WiFi.localIP().toString());
  webServer->sendContent(F("<br/><b>Signal Strength: </b>"));
  webServer->sendContent(String(WiFi.RSSI()));
  webServer->sendContent(F("<br/><b>Uptime: </b>"));
  webServer->sendContent(String(long(millis() / 1000)));
  // webServer->sendContent(F("<br/><b>Last reset: </b>"));
  // webServer->sendContent(ESP.getResetInfo());
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionOtaStartDownload(const String &lcdOtaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  uint32_t lcdOtaFileSize = 0;
  String lcdOtaNextionCmd;
  uint32_t lcdOtaChunkCounter = 0;
  uint16_t lcdOtaPartNum = 0;
  uint32_t lcdOtaTransferred = 0;
  uint8_t lcdOtaPercentComplete = 0;
  const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for lcdOtaTimeout

  HTTPClient lcdOtaHttp;
  WiFiClientSecure lcdOtaWifiSecure;
  WiFiClient lcdOtaWifi;
  if (lcdOtaUrl.startsWith(F("https")))
  {
    debugPrintln("LCDOTA: Attempting firmware update from HTTPS host: " + lcdOtaUrl);

    lcdOtaHttp.begin(lcdOtaWifiSecure, lcdOtaUrl);
    lcdOtaWifiSecure.setInsecure();
    // lcdOtaWifiSecure.setBufferSizes(512, 512);
  }
  else
  {
    debugPrintln("LCDOTA: Attempting firmware update from HTTP host: " + lcdOtaUrl);
    lcdOtaHttp.begin(lcdOtaWifi, lcdOtaUrl);
  }

  int lcdOtaHttpReturn = lcdOtaHttp.GET();
  if (lcdOtaHttpReturn > 0)
  { // HTTP header has been sent and Server response header has been handled
    debugPrintln(String(F("LCDOTA: HTTP GET return code:")) + String(lcdOtaHttpReturn));
    if (lcdOtaHttpReturn == HTTP_CODE_OK)
    {                                                 // file found at server
      int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
      lcdOtaFileSize = lcdOtaRemaining;
      static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
      static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
      static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

      debugPrintln(String(F("LCDOTA: File found at Server. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

      // WiFiUDP::stop(); // Keep mDNS responder and MQTT traffic from breaking things
      // if (mqttClient.connected())
      // {
      //   debugPrintln(F("LCDOTA: LCD firmware upload starting, closing MQTT connection."));
      //   mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
      //   debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
      //   mqttClient.disconnect();
      // }

      WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
      Serial2.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
      Serial2.flush();
      nextionHandleInput();
      String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + "," + String(nextionBaud) + ",0";
      debugPrintln(String(F("LCDOTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
      Serial2.print(lcdOtaNextionCmd);
      Serial2.write(nextionSuffix, sizeof(nextionSuffix));
      Serial2.flush();

      if (nextionOtaResponse())
      {
        debugPrintln(F("LCDOTA: LCD upload command accepted."));
      }
      else
      {
        debugPrintln(F("LCDOTA: LCD upload command FAILED.  Restarting device."));
        espReset();
      }
      debugPrintln(F("LCDOTA: Starting update"));
      lcdOtaTimer = millis();
      while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
      {                                                // Write incoming data to panel as it arrives
        uint16_t lcdOtaHttpSize = stream->available(); // get available data size

        if (lcdOtaHttpSize)
        {
          uint16_t lcdOtaChunkSize = 0;
          if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaHttpSize;
          }
          else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaBufferSize;
          }
          else
          {
            lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
          }
          stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
          Serial2.flush();                              // make sure any previous writes the UART have completed
          Serial2.write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
          lcdOtaChunkCounter += lcdOtaChunkSize;
          if (lcdOtaChunkCounter >= 4096)
          {
            Serial2.flush();
            lcdOtaPartNum++;
            lcdOtaTransferred += lcdOtaChunkCounter;
            lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
            lcdOtaChunkCounter = 0;
            if (nextionOtaResponse())
            { // We've completed a chunk
              debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
              lcdOtaTimer = millis();
            }
            else
            {
              debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
              debugPrintln(F("LCDOTA: failure"));
              delay(2000); // extra delay while the LCD does its thing
              espReset();
            }
          }
          else
          {
            delay(20);
          }
          if (lcdOtaRemaining > 0)
          {
            lcdOtaRemaining -= lcdOtaChunkSize;
          }
        }
        delay(10);
        if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
        { // Our timer expired so reset
          debugPrintln(F("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
          espReset();
        }
      }
      lcdOtaPartNum++;
      lcdOtaTransferred += lcdOtaChunkCounter;
      if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
      {
        debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + String(F(" of ")) + String(tftFileSize) + String(F(" bytes.")));
        uint32_t lcdOtaDelay = millis();
        debugPrintln(F("LCDOTA: Waiting 5 seconds to allow LCD to apply updates we've sent."));
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          yield();
        }
        espReset();
      }
      else
      {
        debugPrintln(String(F("LCDOTA: Failure, lcdOtaTransferred: ")) + String(lcdOtaTransferred) + String(F(" lcdOtaFileSize: ")) + String(lcdOtaFileSize));
        espReset();
      }
    }
  }
  else
  {
    debugPrintln(String(F("LCDOTA: HTTP GET failed, error code ")) + lcdOtaHttp.errorToString(lcdOtaHttpReturn));
    espReset();
  }
  lcdOtaHttp.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionOtaResponse()
{                                               // Monitor the serial port for a 0x05 response within our timeout
  unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
  unsigned long nextionCommandTimer = millis(); // record current time for our timeout
  bool otaSuccessVal = false;
  while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
  {
    if (Serial2.available())
    {
      byte inByte = Serial2.read();
      if (inByte == 0x5)
      {
        otaSuccessVal = true;
        break;
      }
    }
    else
    {
      delay(1);
    }
  }
  return otaSuccessVal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionUpdateProgress(const unsigned int &progress, const unsigned int &total)
{
  uint8_t progressPercent = (float(progress) / float(total)) * 100;
  nextionSetAttr("p[0].b[4].val", String(progressPercent).c_str());
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espStartOta(const String &espOtaUrl)
{ // Update ESP firmware from HTTP/HTTPS URL

  // nextionSetAttr("p[0].b[1].txt", "\"\\rHASPone update:\\r\\r\\r \"");
  // nextionSendCmd("page 0");
  // nextionSendCmd("vis 4,1");

  // WiFiUDP::stopAll(); // Keep mDNS responder from breaking things
  // delay(1);
  // ESPhttpUpdate.rebootOnUpdate(false);
  // ESPhttpUpdate.onProgress(nextionUpdateProgress);
  // t_httpUpdate_return espOtaUrlReturnCode;
  // if (espOtaUrl.startsWith(F("https")))
  // {
  //   debugPrintln(String(F("ESPFW: Attempting firmware update from HTTPS host: ")) + espOtaUrl);
  //   WiFiClientSecure wifiEspOtaClientSecure;
  //   wifiEspOtaClientSecure.setInsecure();
  //   wifiEspOtaClientSecure.setBufferSizes(512, 512);
  //   espOtaUrlReturnCode = ESPhttpUpdate.update(wifiEspOtaClientSecure, espOtaUrl);
  // }
  // else
  // {
  //   debugPrintln(String(F("ESPFW: Attempting firmware update from HTTP host: ")) + espOtaUrl);
  //   espOtaUrlReturnCode = ESPhttpUpdate.update(wifiClient, espOtaUrl);
  // }

  // switch (espOtaUrlReturnCode)
  // {
  // case HTTP_UPDATE_FAILED:
  //   debugPrintln(String(F("ESPFW: HTTP_UPDATE_FAILED error ")) + String(ESPhttpUpdate.getLastError()) + " " + ESPhttpUpdate.getLastErrorString());
  //   nextionSendCmd("vis 4,0");
  //   nextionSetAttr("p[0].b[1].txt", "\"HASPone update:\\r FAILED\\rerror: " + ESPhttpUpdate.getLastErrorString() + "\"");
  //   break;

  // case HTTP_UPDATE_NO_UPDATES:
  //   debugPrintln(F("ESPFW: HTTP_UPDATE_NO_UPDATES"));
  //   nextionSendCmd("vis 4,0");
  //   nextionSetAttr("p[0].b[1].txt", "\"HASPone update:\\rNo update\"");
  //   break;

  // case HTTP_UPDATE_OK:
  //   debugPrintln(F("ESPFW: HTTP_UPDATE_OK"));
  //   nextionSetAttr("p[0].b[1].txt", "\"\\rHASPone update:\\r\\r Complete!\\rRestarting.\"");
  //   nextionSendCmd("vis 4,1");
  //   delay(1000);
  //   espReset();
  // }
  // delay(1000);
  // nextionSendCmd("page " + String(nextionActivePage));
}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleFirmware()
// { // http://plate01/firmware
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /firmware page to client connected from: ")) + webServer->client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Firmware updates");
//   webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer->send(200, "text/html", httpHeader);
//   webServer->sendContent_P(HTTP_SCRIPT);
//   webServer->sendContent_P(HTTP_STYLE);
//   webServer->sendContent_P(HASP_STYLE);
//   webServer->sendContent_P(HTTP_HEAD_END);

//   webServer->sendContent(F("<h1>"));
//   webServer->sendContent(haspNode);
//   webServer->sendContent(F(" Firmware updates</h1><b>Note:</b> If updating firmware for both the ESP8266 and the Nextion LCD, you'll want to update the ESP8266 first followed by the Nextion LCD<br/><hr/>"));

//   // Display main firmware page
//   webServer->sendContent(F("<form method='get' action='/espfirmware'>"));
//   if (updateEspAvailable)
//   {
//     webServer->sendContent(F("<font color='green'><b>HASPone ESP8266 update available!</b></font>"));
//   }
//   webServer->sendContent(F("<br/><b>Update ESP8266 from URL</b>"));
//   webServer->sendContent(F("<br/><input id='espFirmwareURL' name='espFirmware' value='"));
//   webServer->sendContent(espFirmwareUrl);
//   webServer->sendContent(F("'><br/><br/><button type='submit'>Update ESP from URL</button></form>"));

//   webServer->sendContent(F("<br/><form method='POST' action='/update' enctype='multipart/form-data'>"));
//   webServer->sendContent(F("<b>Update ESP8266 from file</b><input type='file' id='espSelect' name='espSelect' accept='.bin'>"));
//   webServer->sendContent(F("<br/><br/><button type='submit' id='espUploadSubmit' onclick='ackEspUploadSubmit()'>Update ESP from file</button></form>"));

//   webServer->sendContent(F("<br/><br/><hr><h1>WARNING!</h1>"));
//   webServer->sendContent(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the LCD will display an error message until a successful firmware update has completed. "));
//   webServer->sendContent(F("<br/><br/><i>Note: Failed LCD firmware updates on HASPone hardware prior to v1.0 may require a hard power cycle of the device, via a circuit breaker or by physically disconnecting the device.</i>"));

//   webServer->sendContent(F("<br/><hr><form method='get' action='lcddownload'>"));
//   if (updateLcdAvailable)
//   {
//     webServer->sendContent(F("<font color='green'><b>HASPone LCD update available!</b></font>"));
//   }
//   webServer->sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
//   webServer->sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
//   webServer->sendContent(lcdFirmwareUrl);
//   webServer->sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

//   webServer->sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
//   webServer->sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
//   webServer->sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

//   // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
//   webServer->sendContent(F("<script>function handleLcdFileSelect(evt) {"));
//   webServer->sendContent(F("var uploadFile = evt.target.files[0];"));
//   webServer->sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
//   webServer->sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
//   webServer->sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
//   webServer->sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
//   webServer->sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
//   webServer->sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
//   webServer->sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
//   webServer->sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

//   webServer->sendContent_P(HTTP_END);
//   webServer->sendContent("");
// }


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdUpdateSuccess()
// { // http://plate01/lcdOtaSuccess
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcdOtaSuccess page to client connected from: ")) + webServer->client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update success");
//   webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer->send(200, "text/html", httpHeader);
//   webServer->sendContent_P(HTTP_SCRIPT);
//   webServer->sendContent_P(HTTP_STYLE);
//   webServer->sendContent_P(HASP_STYLE);
//   webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
//   webServer->sendContent_P(HTTP_HEAD_END);
//   webServer->sendContent(F("<h1>"));
//   webServer->sendContent(haspNode);
//   webServer->sendContent(F(" LCD update success</h1>"));
//   webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
//   webServer->sendContent_P(HTTP_END);
//   webServer->sendContent("");
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdUpdateFailure()
// { // http://plate01/lcdOtaFailure
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcdOtaFailure page to client connected from: ")) + webServer->client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update failed");
//   webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer->send(200, "text/html", httpHeader);
//   webServer->sendContent_P(HTTP_SCRIPT);
//   webServer->sendContent_P(HTTP_STYLE);
//   webServer->sendContent_P(HASP_STYLE);
//   webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
//   webServer->sendContent_P(HTTP_HEAD_END);
//   webServer->sendContent(F("<h1>"));
//   webServer->sendContent(haspNode);
//   webServer->sendContent(F(" LCD update failed :(</h1>"));
//   webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
//   webServer->sendContent_P(HTTP_END);
//   webServer->sendContent("");
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdDownload()
// { // http://plate01/lcddownload
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcddownload page to client connected from: ")) + webServer->client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update");
//   webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer->send(200, "text/html", httpHeader);
//   webServer->sendContent_P(HTTP_SCRIPT);
//   webServer->sendContent_P(HTTP_STYLE);
//   webServer->sendContent_P(HASP_STYLE);
//   webServer->sendContent_P(HTTP_HEAD_END);
//   webServer->sendContent(F("<h1>"));
//   webServer->sendContent(haspNode);
//   webServer->sendContent(F(" LCD update</h1>"));
//   webServer->sendContent(F("<br/>Updating LCD firmware from: "));
//   webServer->sendContent(webServer->arg("lcdFirmware"));
//   webServer->sendContent_P(HTTP_END);
//   webServer->sendContent("");
//   nextionOtaStartDownload(webServer->arg("lcdFirmware"));
// }


void mNextionPanel::webHandleTftFileSize()
{ // http://plate01/tftFileSize
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /tftFileSize page to client connected from: %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " TFT Filesize");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_HEAD_END);
  tftFileSize = webServer->arg("tftFileSize").toInt();
  
  ALOG_INF(PSTR(D_LOG_NEXTION "Received tftFileSize: %d"), tftFileSize);

}



/**
 * @brief 
 * 
 */
void mNextionPanel::webHandleLcdUpload()
{ // http://plate01/lcdupload
  // Upload firmware to the Nextion LCD via HTTP upload

  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR("webHandleLcdUpload"));

  static uint32_t lcdOtaTransferred = 0;
  static uint32_t lcdOtaRemaining;
  static uint16_t lcdOtaParts;
  const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  HTTPUpload &upload = webServer->upload();

  if (tftFileSize == 0)
  {
    debugPrintln(String(F("LCDOTA: FAILED, no filesize sent.")));
    String httpHeader = FPSTR(HTTP_HEAD_START);
    httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD update error");
    webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
    webServer->send(200, "text/html", httpHeader);
    webServer->sendContent_P(HTTP_SCRIPT);
    webServer->sendContent_P(HTTP_STYLE);
    webServer->sendContent_P(HASP_STYLE);
    webServer->sendContent(F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
    webServer->sendContent_P(HTTP_HEAD_END);
    webServer->sendContent(F("<h1>"));
    webServer->sendContent(haspNode);
    webServer->sendContent(F(" LCD update FAILED</h1>"));
    webServer->sendContent(F("No update file size reported. You must use a modern browser with Javascript enabled."));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent("");
  }
  else if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  { // Our timer expired so reset
    debugPrintln(F("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
    espReset();
  }
  else if (upload.status == UPLOAD_FILE_START)
  {
    // WiFiUDP::stopAll(); // Keep mDNS responder from breaking things
    // WiFiUDP::stop(); // Keep mDNS responder from breaking things
  
    ALOG_INF(PSTR("(upload.status == UPLOAD_FILE_START)"));

    debugPrintln(String(F("LCDOTA: Attempting firmware upload")));
    debugPrintln(String(F("LCDOTA: upload.filename: ")) + String(upload.filename));
    debugPrintln(String(F("LCDOTA: TFTfileSize: ")) + String(tftFileSize));

    lcdOtaRemaining = tftFileSize;
    lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
    debugPrintln(String(F("LCDOTA: File upload beginning. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));
    
    Serial2.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
    Serial2.flush();
    nextionHandleInput();

    String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(nextionBaud) + ",0";
    debugPrintln(String(F("LCDOTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
    Serial2.print(lcdOtaNextionCmd);
    Serial2.write(nextionSuffix, sizeof(nextionSuffix));
    Serial2.flush();

    
    if (nextionOtaResponse())
    {
      debugPrintln(F("LCDOTA: LCD upload command accepted"));
    }
    else
    {
      debugPrintln(F("LCDOTA: LCD upload command FAILED."));
      espReset();
    }
    lcdOtaTimer = millis();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  { // Handle upload data
    static int lcdOtaChunkCounter = 0;
    static uint16_t lcdOtaPartNum = 0;
    static int lcdOtaPercentComplete = 0;
    static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
    static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
    uint16_t lcdOtaUploadIndex = 0;
    int32_t lcdOtaPacketRemaining = upload.currentSize;

    while (lcdOtaPacketRemaining > 0)
    { 
      
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif
                
      // Write incoming data to panel as it arrives
      // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
      uint16_t lcdOtaChunkSize = 0;
      if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaPacketRemaining;
      }
      else if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaBufferSize;
      }
      else
      {
        lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
      }

      for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
      { // Load up the UART buffer
        lcdOtaBuffer[i] = upload.buf[lcdOtaUploadIndex];
        lcdOtaUploadIndex++;
      }
      Serial2.flush();                              // Clear out current UART buffer
      Serial2.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
      lcdOtaChunkCounter += lcdOtaChunkSize;
      lcdOtaTransferred += lcdOtaChunkSize;
      if (lcdOtaChunkCounter >= 4096)
      {
        Serial2.flush();
        lcdOtaPartNum++;
        lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
        lcdOtaChunkCounter = 0;
        if (nextionOtaResponse())
        {
          debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
        }
        else
        {
          debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
        }
      }
      else
      {
        delay(10);
      }
      if (lcdOtaRemaining > 0)
      {
        lcdOtaRemaining -= lcdOtaChunkSize;
      }
      if (lcdOtaPacketRemaining > 0)
      {
        lcdOtaPacketRemaining -= lcdOtaChunkSize;
      }
    }

    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      {
        debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
        webServer->sendHeader("Location", "/lcdOtaSuccess");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          delay(1);
        }
        espReset();
      }
      else
      {
        debugPrintln(F("LCDOTA: Failure"));
        webServer->sendHeader("Location", "/lcdOtaFailure");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 1000)
        { // extra 1sec delay for client to grab failure page
          webServer->handleClient();
          delay(1);
        }
        espReset();
      }
    }
    lcdOtaTimer = millis();
  }
  else if (upload.status == UPLOAD_FILE_END)
  { // Upload completed
    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      { // YAY WE DID IT
        debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
        webServer->sendHeader("Location", "/lcdOtaSuccess");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          yield();
        }
        espReset();
      }
      else
      {
        debugPrintln(F("LCDOTA: Failure"));
        webServer->sendHeader("Location", "/lcdOtaFailure");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 1000)
        { // extra 1sec delay for client to grab failure page
          webServer->handleClient();
          yield();
        }
        espReset();
      }
    }
  }
  else if (upload.status == UPLOAD_FILE_ABORTED)
  { // Something went kablooey
    debugPrintln(F("LCDOTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
    debugPrintln(F("LCDOTA: Failure"));
    webServer->sendHeader("Location", "/lcdOtaFailure");
    webServer->send(303);
    uint32_t lcdOtaDelay = millis();
    while ((millis() - lcdOtaDelay) < 1000)
    { // extra 1sec delay for client to grab failure page
      webServer->handleClient();
      yield();
    }
    espReset();
  }
  else
  { // Something went weird, we should never get here...
    debugPrintln(String(F("LCDOTA: upload.status returned: ")) + String(upload.status));
    debugPrintln(F("LCDOTA: Failure"));
    webServer->sendHeader("Location", "/lcdOtaFailure");
    webServer->send(303);
    uint32_t lcdOtaDelay = millis();
    while ((millis() - lcdOtaDelay) < 1000)
    { // extra 1sec delay for client to grab failure page
      webServer->handleClient();
      yield();
    }
    espReset();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateSuccess()
{ // http://plate01/lcdOtaSuccess
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
  debugPrintln(String(F("HTTP: Sending /lcdOtaSuccess page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update success");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update success</h1>"));
  webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateFailure()
{ // http://plate01/lcdOtaFailure
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  debugPrintln(String(F("HTTP: Sending /lcdOtaFailure page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update failed");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update failed :(</h1>"));
  webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdDownload()
{ // http://plate01/lcddownload
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  debugPrintln(String(F("HTTP: Sending /lcddownload page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update</h1>"));
  webServer->sendContent(F("<br/>Updating LCD firmware from: "));
  webServer->sendContent(webServer->arg("lcdFirmware"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
  nextionOtaStartDownload(webServer->arg("lcdFirmware"));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleReboot()
{ // http://plate01/reboot
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
  debugPrintln(String(F("HTTP: Sending /reboot page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " reboot");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='10;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" Reboot</h1>"));
  webServer->sendContent(F("<br/>Rebooting device"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"Rebooting...\"");
  espReset();
}

void mNextionPanel::espReset(){
esp_restart();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleResetBacklight()
{ // http://plate01/resetBacklight
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  debugPrintln(String(F("HTTP: Sending /resetBacklight page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Backlight reset");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='3;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F("</h1>"));
  webServer->sendContent(F("<br/>Resetting backlight to 100%"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
  debugPrintln(F("HTTP: Resetting backlight to 100%"));
  nextionSetAttr("dims", "100");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleFirmware()
{ // http://plate01/firmware

  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  

  debugPrintln(String(F("HTTP: Sending /firmware page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Firmware updates");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" Firmware updates</h1><b>Note:</b> If updating firmware for both the ESP8266 and the Nextion LCD, you'll want to update the ESP8266 first followed by the Nextion LCD<br/><hr/>"));

  // Display main firmware page
  webServer->sendContent(F("<form method='get' action='/espfirmware'>"));
  if (updateEspAvailable)
  {
    webServer->sendContent(F("<font color='green'><b>HASPone ESP8266 update available!</b></font>"));
  }
  webServer->sendContent(F("<br/><b>Update ESP8266 from URL</b>"));
  webServer->sendContent(F("<br/><input id='espFirmwareURL' name='espFirmware' value='"));
  webServer->sendContent("espFirmwareUrl");
  webServer->sendContent(F("'><br/><br/><button type='submit'>Update ESP from URL</button></form>"));

  webServer->sendContent(F("<br/><form method='POST' action='/update' enctype='multipart/form-data'>"));
  webServer->sendContent(F("<b>Update ESP8266 from file</b><input type='file' id='espSelect' name='espSelect' accept='.bin'>"));
  webServer->sendContent(F("<br/><br/><button type='submit' id='espUploadSubmit' onclick='ackEspUploadSubmit()'>Update ESP from file</button></form>"));

  webServer->sendContent(F("<br/><br/><hr><h1>WARNING!</h1>"));
  webServer->sendContent(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the LCD will display an error message until a successful firmware update has completed. "));
  webServer->sendContent(F("<br/><br/><i>Note: Failed LCD firmware updates on HASPone hardware prior to v1.0 may require a hard power cycle of the device, via a circuit breaker or by physically disconnecting the device.</i>"));

  webServer->sendContent(F("<br/><hr><form method='get' action='lcddownload'>"));
  if (updateLcdAvailable)
  {
    webServer->sendContent(F("<font color='green'><b>HASPone LCD update available!</b></font>"));
  }
  webServer->sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  webServer->sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
  webServer->sendContent("lcdFirmwareUrl");
  webServer->sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  webServer->sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  webServer->sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  webServer->sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

  // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
  webServer->sendContent(F("<script>function handleLcdFileSelect(evt) {"));
  webServer->sendContent(F("var uploadFile = evt.target.files[0];"));
  webServer->sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
  webServer->sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
  webServer->sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
  webServer->sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
  webServer->sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
  webServer->sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
  webServer->sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
  webServer->sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

}


/**
 * @brief New 2023 version of my nextion code
 * Basic webui page that will permit uploading TFT files to update the panel
 * 
 * Ideally, this page can easily be integrated with NETWORK_WEBSERVER on the unified system
 * 
 */
void mNextionPanel::WebPage_LCD_Update_TFT()
{ 
  
  // if (configPassword[0] != '\0')  // Keep! I will want to integrate this in the future too
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR("HTTP: Sending /firmware page to client connected from:  %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " Firmware updates");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(pCONT_set->Settings.system_name.friendly);
  webServer->sendContent(F(" Nextion Firmware Update</h1>"));
  
  /**
   * @brief Title on pagw
   **/
  webServer->sendContent(F("<br/><br/><hr><h1>WARNING!</h1>"));
  webServer->sendContent(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the LCD will display an error message until a successful firmware update has completed. "));
  webServer->sendContent(F("<br/><br/><i>Note: Failed LCD firmware updates on HASPone hardware prior to v1.0 may require a hard power cycle of the device, via a circuit breaker or by physically disconnecting the device.</i>"));


  /**
   * @brief Update with URL only
   **/
  webServer->sendContent(F("<br/><hr><form method='get' action='lcddownload'>"));
  if(updateLcdAvailable){ webServer->sendContent(F("<font color='green'><b>LCD update available!</b></font>")); }
  webServer->sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  webServer->sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
  webServer->sendContent("No Url Set");
  webServer->sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  /**
   * @brief Update with File
   **/
  webServer->sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  webServer->sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  webServer->sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

  /**
   * @brief Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
   **/
  webServer->sendContent(F("<script>function handleLcdFileSelect(evt) {"));
  webServer->sendContent(F("var uploadFile = evt.target.files[0];"));
  webServer->sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
  webServer->sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
  webServer->sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
  webServer->sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
  webServer->sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
  webServer->sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
  webServer->sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
  webServer->sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

}



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleSaveConfig()
{ // http://plate01/saveConfig
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  debugPrintln(String(F("HTTP: Sending /saveConfig page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Saving configuration");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  bool shouldSaveWifi = false;
  // Check required values
  if ((webServer->arg("wifiSSID") != "") && (webServer->arg("wifiSSID") != String(WiFi.SSID())))
  { // Handle WiFi SSID
    shouldSaveConfig = true;
    shouldSaveWifi = true;
    webServer->arg("wifiSSID").toCharArray(wifiSSID, 32);
  }
  if ((webServer->arg("wifiPass") != String(wifiPass)) && (webServer->arg("wifiPass") != String("********")))
  { // Handle WiFi password
    shouldSaveConfig = true;
    shouldSaveWifi = true;
    webServer->arg("wifiPass").toCharArray(wifiPass, 64);
  }

  if (webServer->arg("haspNode") != "" && webServer->arg("haspNode") != String(haspNode))
  { // Handle haspNode
    shouldSaveConfig = true;
    String lowerHaspNode = webServer->arg("haspNode");
    lowerHaspNode.toLowerCase();
    lowerHaspNode.toCharArray(haspNode, 16);
  }
  if (webServer->arg("groupName") != "" && webServer->arg("groupName") != String(groupName))
  { // Handle groupName
    shouldSaveConfig = true;
    webServer->arg("groupName").toCharArray(groupName, 16);
  }

  if (webServer->arg("mqttServer") != "" && webServer->arg("mqttServer") != String(mqttServer))
  { // Handle mqttServer
    shouldSaveConfig = true;
    webServer->arg("mqttServer").toCharArray(mqttServer, 128);
  }
  if (webServer->arg("mqttPort") != "" && webServer->arg("mqttPort") != String(mqttPort))
  { // Handle mqttPort
    shouldSaveConfig = true;
    webServer->arg("mqttPort").toCharArray(mqttPort, 6);
  }
  if (webServer->arg("mqttUser") != String(mqttUser))
  { // Handle mqttUser
    shouldSaveConfig = true;
    webServer->arg("mqttUser").toCharArray(mqttUser, 128);
  }
  if (webServer->arg("mqttPassword") != String("********"))
  { // Handle mqttPassword
    shouldSaveConfig = true;
    webServer->arg("mqttPassword").toCharArray(mqttPassword, 128);
  }
  if ((webServer->arg("mqttTlsEnabled") == String("on")) && !mqttTlsEnabled)
  { // mqttTlsEnabled was disabled but should now be enabled
    shouldSaveConfig = true;
    mqttTlsEnabled = true;
  }
  else if ((webServer->arg("mqttTlsEnabled") == String("")) && mqttTlsEnabled)
  { // mqttTlsEnabled was enabled but should now be disabled
    shouldSaveConfig = true;
    mqttTlsEnabled = false;
  }
  if (webServer->arg("mqttFingerprint") != String(mqttFingerprint))
  { // Handle mqttFingerprint
    shouldSaveConfig = true;
    webServer->arg("mqttFingerprint").toCharArray(mqttFingerprint, 60);
  }
  if (webServer->arg("configUser") != String(configUser))
  { // Handle configUser
    shouldSaveConfig = true;
    webServer->arg("configUser").toCharArray(configUser, 32);
  }
  if (webServer->arg("configPassword") != String("********"))
  { // Handle configPassword
    shouldSaveConfig = true;
    webServer->arg("configPassword").toCharArray(configPassword, 32);
  }
  if (webServer->arg("hassDiscovery") != String(hassDiscovery))
  { // Handle hassDiscovery
    shouldSaveConfig = true;
    webServer->arg("hassDiscovery").toCharArray(hassDiscovery, 128);
  }
  if ((webServer->arg("nextionMaxPages") != String(nextionMaxPages)) && (webServer->arg("nextionMaxPages").toInt() < 256) && (webServer->arg("nextionMaxPages").toInt() > 0))
  {
    shouldSaveConfig = true;
    nextionMaxPages = webServer->arg("nextionMaxPages").toInt();
  }
  if (webServer->arg("nextionBaud") != String(nextionBaud))
  { // Handle nextionBaud
    shouldSaveConfig = true;
    webServer->arg("nextionBaud").toCharArray(nextionBaud, 7);
  }
  if (webServer->arg("motionPinConfig") != String(motionPinConfig))
  { // Handle motionPinConfig
    shouldSaveConfig = true;
    webServer->arg("motionPinConfig").toCharArray(motionPinConfig, 3);
  }
  if ((webServer->arg("debugSerialEnabled") == String("on")) && !debugSerialEnabled)
  { // debugSerialEnabled was disabled but should now be enabled
    shouldSaveConfig = true;
    debugSerialEnabled = true;
  }
  else if ((webServer->arg("debugSerialEnabled") == String("")) && debugSerialEnabled)
  { // debugSerialEnabled was enabled but should now be disabled
    shouldSaveConfig = true;
    debugSerialEnabled = false;
  }
  if ((webServer->arg("debugTelnetEnabled") == String("on")) && !debugTelnetEnabled)
  { // debugTelnetEnabled was disabled but should now be enabled
    shouldSaveConfig = true;
    debugTelnetEnabled = true;
  }
  else if ((webServer->arg("debugTelnetEnabled") == String("")) && debugTelnetEnabled)
  { // debugTelnetEnabled was enabled but should now be disabled
    shouldSaveConfig = true;
    debugTelnetEnabled = false;
  }
  if ((webServer->arg("mdnsEnabled") == String("on")) && !mdnsEnabled)
  { // mdnsEnabled was disabled but should now be enabled
    shouldSaveConfig = true;
    mdnsEnabled = true;
  }
  else if ((webServer->arg("mdnsEnabled") == String("")) && mdnsEnabled)
  { // mdnsEnabled was enabled but should now be disabled
    shouldSaveConfig = true;
    mdnsEnabled = false;
  }
  if ((webServer->arg("beepEnabled") == String("on")) && !beepEnabled)
  { // beepEnabled was disabled but should now be enabled
    shouldSaveConfig = true;
    beepEnabled = true;
  }
  else if ((webServer->arg("beepEnabled") == String("")) && beepEnabled)
  { // beepEnabled was enabled but should now be disabled
    shouldSaveConfig = true;
    beepEnabled = false;
  }
  if ((webServer->arg("ignoreTouchWhenOff") == String("on")) && !ignoreTouchWhenOff)
  { // ignoreTouchWhenOff was disabled but should now be enabled
    shouldSaveConfig = true;
    ignoreTouchWhenOff = true;
  }
  else if ((webServer->arg("ignoreTouchWhenOff") == String("")) && ignoreTouchWhenOff)
  { // ignoreTouchWhenOff was enabled but should now be disabled
    shouldSaveConfig = true;
    ignoreTouchWhenOff = false;
  }

  if (shouldSaveConfig)
  { // Config updated, notify user and trigger write to SPIFFS

    webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
    webServer->sendContent_P(HTTP_HEAD_END);
    webServer->sendContent(F("<h1>"));
    webServer->sendContent(haspNode);
    webServer->sendContent(F("</h1>"));
    webServer->sendContent(F("<br/>Saving updated configuration values and restarting device"));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent(F(""));
    // configSave();
    if (shouldSaveWifi)
    {
      debugPrintln(String(F("CONFIG: Attempting connection to SSID: ")) + webServer->arg("wifiSSID"));
      // espWifiConnect();
    }
    // espReset();
  }
  else
  { // No change found, notify user and link back to config page
    webServer->sendContent(F("<meta http-equiv='refresh' content='3;url=/' />"));
    webServer->sendContent_P(HTTP_HEAD_END);
    webServer->sendContent(F("<h1>"));
    webServer->sendContent(haspNode);
    webServer->sendContent(F("</h1>"));
    webServer->sendContent(F("<br/>No changes found, returning to <a href='/'>home page</a>"));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent(F(""));
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleResetConfig()
{ // http://plate01/resetConfig
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  debugPrintln(String(F("HTTP: Sending /resetConfig page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Resetting configuration");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  if (webServer->arg("confirm") == "yes")
  { // User has confirmed, so reset everything
    webServer->sendContent(F("<h1>"));
    webServer->sendContent(haspNode);
    webServer->sendContent(F("</h1><b>Resetting all saved settings and restarting device into WiFi AP mode</b>"));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent("");
    delay(100);
    // configClearSaved();
  }
  else
  {
    webServer->sendContent(F("<h1>Warning</h1><b>This process will reset all settings to the default values and restart the device.  You may need to connect to the WiFi AP displayed on the panel to re-configure the device before accessing it again."));
    webServer->sendContent(F("<br/><hr><br/><form method='get' action='resetConfig'>"));
    webServer->sendContent(F("<br/><br/><button type='submit' name='confirm' value='yes'>reset all settings</button></form>"));
    webServer->sendContent(F("<br/><hr><br/><form method='get' action='/'>"));
    webServer->sendContent(F("<button type='submit'>return home</button></form>"));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent("");
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleEspFirmware()
{ // http://plate01/espfirmware
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  debugPrintln(String(F("HTTP: Sending /espfirmware page to client connected from: ")) + webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " ESP8266 firmware update");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='60;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" ESP8266 firmware update</h1>"));
  webServer->sendContent(F("<br/>Updating ESP firmware from: "));
  webServer->sendContent(webServer->arg("espFirmware"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

  debugPrintln("ESPFW: Attempting ESP firmware update from: " + String(webServer->arg("espFirmware")));
  espStartOta(webServer->arg("espFirmware"));
}




#endif