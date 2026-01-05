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

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

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
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
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

  return FUNCTION_RESULT_UNKNOWN_ID;

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

// ALOG_INF(PSTR("SubTask_UpdateOLED"));

//     DEBUG_LINE_HERE_MILLIS

//   tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
//   char buffer[100] = {0};
//   char buffer_f[100] = {0};
//   char buffer_n[100] = {0};
  
//   snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetTime().c_str() );

//   float sensor_data = -1;

//   /**
//    * @brief Add each sensor on new line
//    */
   
//   uint8_t sensors_available = 4;//tkr_db18->GetSensorCount();

//   int8_t line = -1;

//   for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//   {
//     line = -1;
//     sensors_reading_t val;
//     #ifdef USE_MODULE_ENERGY_PZEM004T_V3
//     tkr_pzem->GetSensorReading(&val, sensor_id);
//     if(val.Valid())
//     {

//       sensor_data = val.GetFloat(SENSOR_TYPE_ACTIVE_POWER_ID);        
//       DLI->GetDeviceName_WithModuleUniqueID( tkr_pzem->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

//       /**
//        * @brief Check for name and replace with OLED friendly short name
//        * 
//        */
//       if(strcmp(buffer_n, D_DEVICE_HEATER_0_NAME)==0)
//       {
//         memset(buffer_n, 0, sizeof(buffer_n));
//         sprintf(buffer_n, "%s", "Hai");
//         line = 0;
//       }else 
//       if(strcmp(buffer_n, D_DEVICE_HEATER_1_NAME)==0)
//       {
//         memset(buffer_n, 0, sizeof(buffer_n));
//         sprintf(buffer_n, "%s", "Flr");
//         line = 1;
//       }
//       else 
//       if(strcmp(buffer_n, D_DEVICE_HEATER_2_NAME)==0)
//       {
//         memset(buffer_n, 0, sizeof(buffer_n));
//         sprintf(buffer_n, "%s", "Fan");
//         line = 2;
//       }else 
//       if(strcmp(buffer_n, D_DEVICE_HEATER_3_NAME)==0)
//       {
//         memset(buffer_n, 0, sizeof(buffer_n));
//         sprintf(buffer_n, "%s", "Oil");
//         line = 3;
//       }

//       if(line >= 0)
//       {
//         snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
//         tkr_iDisp->LogBuffer_AddRow(buffer, line);
//       }

//     }
//     #endif

//   }

//   if(line =- 1) // no valid readings
//   {
//     memset(buffer_n, 0, sizeof(buffer_n));
//     sprintf(buffer_n, "%s", "PZEM d/c");
//     line = 0;
//     tkr_iDisp->LogBuffer_AddRow(buffer, line);

//   }

//   DEBUG_LINE_HERE_MILLIS

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
  tkr_web->server->on("/m/serverresetrelays", HTTP_ANY, [this](AsyncWebServerRequest *request){
    this->Serve_Submodule_ServerResetRelays(request);
  });

}

void mServerResetRelays::Serve_Submodule_ServerResetRelays(AsyncWebServerRequest* request)
{
  const bool post = (request->method() == HTTP_POST);

  // 1) POST: parse + apply
  if (post)
  {
    // Use shared parsing helpers later (tkr_web->...)
    // For now: direct param example

    String action = request->hasParam("A", true) ? request->getParam("A", true)->value() : "save";

    if (action.startsWith("pulse"))
    {
      int relay = action.substring(5).toInt(); // pulse1..pulse4
      int pulse_ms = request->hasParam("PT", true) ? request->getParam("PT", true)->value().toInt() : 300;

      rt.rel[relay].time_last_changed = millis();
      mqtthandler_state_ifchanged.Send();

      // TODO: call your relay driver here
      // tkr_relays->Pulse(relay, pulse_ms);

      tkr_web->serveMessage(request, 200, PSTR("Action executed"), PSTR("Redirecting..."), 1);
      return;
    }

    // Save config example
    // String r1 = request->hasParam("R1", true) ? request->getParam("R1", true)->value() : "";
    // ...

    tkr_web->serveMessage(request, 200, PSTR("Settings saved."), FPSTR(s_redirecting), 1);
    return;
  }

  // 2) GET: serve page
  AsyncWebServerResponse* response =
      request->beginResponse_P(200, FPSTR(CONTENT_TYPE_HTML), WEB_10_ConSpec_13_ServerResetRelays_test_htm, WEB_10_ConSpec_13_ServerResetRelays_test_htm_length);
  response->addHeader(F("Content-Encoding"), F("gzip"));
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


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mServerResetRelays::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mServerResetRelays::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mServerResetRelays::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS