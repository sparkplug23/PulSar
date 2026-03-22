/**
 * @file mServerResetRelays.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-05
 * 
 * 
 * Used for QTQ computers, on the X8 relay board. 
 * To te 3D printed, large enough to hold the power jacks out from the relays
 * Then have the OLEDs on top
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "mServerResetRelays.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS

int8_t mServerResetRelays::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;

}//end



void mServerResetRelays::Pre_Init(void)
{

}


void mServerResetRelays::Init(void)
{

  module_state.mode = ModuleStatus::Running;

}


void mServerResetRelays::EverySecond()
{
  
  SubTask_UpdateOLED();

  for(int i=0;i<8;i++)
  {
    ALOG_INF(PSTR("relay %d,%d"), i, tkr_relay->CommandGet_Relay_Power(i));
  }

}


/**
 * @brief 
 * Show basic info
 * 
 * IM: SECS min
 * IT: Immersion Temp used for shower
 * BT: Water temp used for bath
 * CL : Show UTC time for easy checking it is working / "UP": Every 5 seconds alternate between showing RTC and Uptime
 * 
 * 
 * 
 * */
void mServerResetRelays::SubTask_UpdateOLED()
{

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  
  /**
   * @brief 
   * 
   * Lines
   * 1     : "I:4.3.2.1 " IPAddress
   * 2     : "U:12:34:56" Uptime
   * 3     : "R:01000000"  Relay states as binary 00000010 ie relays 7 is on
   * 4     : "DATA FLASHED" Although not practical, since we are going softAP I cant debug, so good to know when the software was added.
   * 
   */

  memset(buffer,0,sizeof(buffer));
  // sprintf(buffer, "U:");
  sprintf(&buffer[0], "%s", tkr_time->GetUptime(buffer,sizeof(buffer)));
  tkr_iDisp->LogBuffer_AddRow(buffer, 0);

  memset(buffer,0,sizeof(buffer));
  // sprintf(buffer, "");
  IPAddress localip   = WiFi.localIP();
  if(localip[0]==192) localip[0]=2;  // Lets force short hand when its expected 192.168
  if(localip[1]==168) localip[1]=8;
  sprintf(&buffer[2], "%d.%d.%d.%d", localip[0],localip[1],localip[2],localip[3] );
  tkr_iDisp->LogBuffer_AddRow(buffer, 1);

  memset(buffer,0,sizeof(buffer));
  sprintf(buffer, "R:");
  for(int i=0;i<tkr_relay->rt.devices_present;i++){
    sprintf(&buffer[i+2], "%d", tkr_relay->CommandGet_Relay_Power(i));
  }
  tkr_iDisp->LogBuffer_AddRow(buffer, 2);

  
  memset(buffer, 0, sizeof(buffer));
  const char *s = tkr_time->GetBuildDateAndTime().c_str();
  /*
    s = "YYYY-MM-DDThh:mm:ss"
          0123456789ABCDEF
  */
  if (s && strlen(s) >= 16) {
      buffer[0]  = s[2];   // Y
      buffer[1]  = s[3];   // Y
      buffer[2]  = s[5];   // M
      buffer[3]  = s[6];   // M
      buffer[4]  = s[8];   // D
      buffer[5]  = s[9];   // D
      buffer[6]  = s[11];  // h
      buffer[7]  = s[12];  // h
      buffer[8]  = s[14];  // m
      buffer[9]  = s[15];  // m
      buffer[10] = '\0';
  } else {
      memcpy(buffer, "0000000000", 11);
  }
  tkr_iDisp->LogBuffer_AddRow(buffer, 3);


}


/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mServerResetRelays::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];
    
}


/******************************************************************************************************************
 * SECTION: Web
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mServerResetRelays::WebPage_Root_AddHandlers()
{
  SPGM_CTR(PM_SERVER_RELAYS) "/m/serverrelays";
  tkr_web->server->on(PM_SERVER_RELAYS, HTTP_ANY, [this](AsyncWebServerRequest *request){
    this->Serve_Submodule_ServerResetRelays(request);
  });
  AddURLtoList(PM_SERVER_RELAYS, HTTP_ANY);

}

void mServerResetRelays::Serve_Submodule_ServerResetRelays(AsyncWebServerRequest* request)
{
  // 1) POST: apply settings / actions
  if (request->method() == HTTP_POST) {
    Serve_Submodule_ServerResetRelays_Post(request);
    return;
  }

  // 2) GET JS: WLED-style settings JS
  if (request->url().endsWith(F("/s.js"))) {
    Serve_Submodule_ServerResetRelays_JS(request);
    return;
  }

  // 3) GET page (default)
  Serve_Submodule_ServerResetRelays_Page(request);
}


void mServerResetRelays::Serve_Submodule_ServerResetRelays_Post(AsyncWebServerRequest* request)
{
  const String action = request->hasParam("A", true)
                          ? request->getParam("A", true)->value()
                          : "save";

  ALOG_INF(PSTR("post started! %s"), action.c_str());

  // -------------------------------------------------------------------
  // 1) Command path (buttons)
  //   - A=cmd
  //   - C="RB<relay>_<cmd>"
  //     cmd: 0=Off, 1=On, 2=Toggle 5s
  // -------------------------------------------------------------------
  if (action == "cmd")
  {
    ALOG_INF(PSTR("cmd started! %s"), action.c_str());

    const String cmdTok = request->hasParam("C", true)
                            ? request->getParam("C", true)->value()
                            : "";

    ALOG_INF(PSTR("cmdTok: %s"), cmdTok.c_str());

    if (!cmdTok.startsWith("RB"))
    {
      ALOG_INF(PSTR("cmd invalid token (no RB)"));
      request->redirect("/m/serverrelays");
      return;
    }

    const int us = cmdTok.indexOf('_');
    if (us < 0)
    {
      ALOG_INF(PSTR("cmd invalid token (no underscore)"));
      request->redirect("/m/serverrelays");
      return;
    }

    const int relay_id = cmdTok.substring(2, us).toInt();
    const int cmd_id   = cmdTok.substring(us + 1).toInt();

    ALOG_INF(PSTR("relay_id=%d cmd_id=%d"), relay_id, cmd_id);

    if (relay_id < 0 || relay_id >= 8)
    {
      ALOG_INF(PSTR("relay out of range"));
      request->redirect("/m/serverrelays");
      return;
    }

    #ifdef USE_MODULE_DRIVERS_RELAY
    switch (cmd_id)
    {
      default:
      case 0: // Off
        // Preferred: explicit OFF API if you have it
        // tkr_relay->CommandSet_State(false, relay_id);

        // Fallback: if Timer_Decounter(0) means cancel and OFF in your implementation
        tkr_relay->CommandSet_Relay_Power(0, relay_id);
        break;

      case 1: // On
        // Preferred: explicit ON API if you have it
        // tkr_relay->CommandSet_State(true, relay_id);

        // If you do NOT have an "indefinite on" concept via timer,
        // do NOT guess with -1. Keep it explicit and replace this line
        // with your actual API.
        tkr_relay->CommandSet_Relay_Power(1, relay_id); // TODO: replace with proper ON
        break;

      case 2: // Toggle 5s (pulse on then auto-off)
        tkr_relay->CommandSet_Timer_Decounter(7, relay_id);
        break;
    }
    #endif

    rt.rel[relay_id].time_last_changed = millis();
    mqtthandler_state_ifchanged.Send();

    request->redirect("/m/serverrelays");
    return;
  }

  // -------------------------------------------------------------------
  // 2) Save path (names, etc.)
  //   - A=save (default)
  //   - R0..R7
  // -------------------------------------------------------------------
  if (action == "save")
  {
    #ifdef USE_MODULE_DRIVERS_RELAY
    char key[4];

    for (uint8_t relay_id = 0; relay_id < 8; relay_id++)
    {
      snprintf(key, sizeof(key), "R%u", relay_id);

      if (!request->hasParam(key, true))
        continue;

      const String name = request->getParam(key, true)->value();

      // TODO: wire to your naming store (DLI), matching your GetDeviceName_WithModuleUniqueID usage:
      // DLI->SetDeviceName_WithModuleUniqueID(tkr_relay->GetModuleUniqueID(), relay_id, name.c_str());

      rt.rel[relay_id].time_last_changed = millis();
    }

    mqtthandler_state_ifchanged.Send();
    #endif

    request->redirect("/m/serverrelays");
    return;
  }

  // -------------------------------------------------------------------
  // 3) Unknown action
  // -------------------------------------------------------------------
  ALOG_INF(PSTR("unknown action: %s"), action.c_str());
  request->redirect("/m/serverrelays");
}


void mServerResetRelays::Serve_Submodule_ServerResetRelays_JS(AsyncWebServerRequest* request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  // WLED-style: ONLY this opener
  response->print(F("function GetV(){var d=document;"));

  // ------------------------------------------------------------
  // Update time (UT)
  // ------------------------------------------------------------
  char buffer[96];
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetDateAndTime(DT_UTC).c_str());
  tkr_web->printSetFormValue(*response, PSTR("UT"), buffer);

  // ------------------------------------------------------------
  // Per-relay talkback
  // ------------------------------------------------------------
  #ifdef USE_MODULE_DRIVERS_RELAY
  char key[10];
  char val[160];

  for (uint8_t relay_id = 0; relay_id < 8; relay_id++)
  {
    // -----------------------
    // Name: R0..R7
    // -----------------------
    snprintf(key, sizeof(key), "R%u", relay_id);
    DLI->GetDeviceName_WithModuleUniqueID(
      tkr_relay->GetModuleUniqueID(),
      relay_id,
      buffer,
      sizeof(buffer)
    );
    tkr_web->printSetFormValue(*response, key, buffer);

    // -----------------------
    // Status text: T0..T7
    // -----------------------
    const uint32_t sec_on_remaining = tkr_relay->CommandGet_SecondsToRemainOn(relay_id);
    const uint8_t  isOn            = tkr_relay->CommandGet_Relay_Power(relay_id) ? 1 : 0;

    const auto &last = tkr_relay->rt.relay_status[relay_id].last;

    if(sec_on_remaining)
    {
      snprintf(
        val, sizeof(val),
        "%lu Seconds On (On:%02u:%02u:%02u|Off:%02u:%02u:%02u)",
        (unsigned long)sec_on_remaining,
        last.ontime.hour,  last.ontime.minute,  last.ontime.second,
        last.offtime.hour, last.offtime.minute, last.offtime.second
      );
    }else
    {
      snprintf(
        val, sizeof(val),
        "%s (On:%02u:%02u:%02u|Off:%02u:%02u:%02u)",
        isOn ? "ON" : "OFF",
        last.ontime.hour,  last.ontime.minute,  last.ontime.second,
        last.offtime.hour, last.offtime.minute, last.offtime.second
      );
    }


    snprintf(key, sizeof(key), "T%u", relay_id);
    tkr_web->printSetFormValue(*response, key, val);

    // -----------------------
    // Status text colour (your rule)
    //   - timer active  -> orange
    //   - else if on    -> green
    //   - else          -> red
    // -----------------------
    const char* col =
      (sec_on_remaining > 0) ? "#ff9500ff" :   // orange
      (isOn)                 ? "#00ff00ff" :   // green
                               "#ff0000ff";    // red

    response->printf_P(
      PSTR("d.getElementById('T%u').style.color='%s';"),
      relay_id,
      col
    );
  }
  #endif

  response->print(F("}"));
  request->send(response);
}





void mServerResetRelays::Serve_Submodule_ServerResetRelays_Page(AsyncWebServerRequest* request)
{
  AsyncWebServerResponse* response =
    request->beginResponse_P(
      200,
      FPSTR(CONTENT_TYPE_HTML),
      WEB_10_ConSpec_13_ServerResetRelays_server_relay_controls_htm,
      WEB_10_ConSpec_13_ServerResetRelays_server_relay_controls_htm_length
    );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}




#endif


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mServerResetRelays::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  JBI->Add("Relays",8);

  JBI->Array_Start("RelayTime");
    for(int i=0;i<4;i++){
      JBI->Add(rt.rel[i].time_last_changed);
    }
  JBI->End();

  return JBI->End();

}

uint8_t mServerResetRelays::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mServerResetRelays::MQTTHandler_Init()
{

  struct handler<mServerResetRelays>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mServerResetRelays::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mServerResetRelays::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 



#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS