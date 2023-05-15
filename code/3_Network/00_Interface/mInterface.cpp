/**
 * @file mInterfaceNetwork.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Handles basic connection functions (anything connected?) and possibly priority switching between them
 *        (1) Ethernet
 *        (2) WiFi
 *        (3) Cellular 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mInterface.h"

#ifdef USE_MODULE_NETWORK_INTERFACE

const char* mInterfaceNetwork::PM_MODULE__NETWORK_INTERFACE__CTR = D_MODULE__NETWORK_INTERFACE__CTR;
const char* mInterfaceNetwork::PM_MODULE__NETWORK_INTERFACE__FRIENDLY_CTR = D_MODULE__NETWORK_INTERFACE__FRIENDLY_CTR;

int8_t mInterfaceNetwork::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
    {      
      
    }
    break;
    case FUNC_EVERY_SECOND: 

    break;
    case FUNC_EVERY_FIVE_SECOND:

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
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function


void mInterfaceNetwork::Pre_Init(void)
{
  
  settings.fEnableSensor = true;

}


void mInterfaceNetwork::Init(void)
{


}


void mInterfaceNetwork::EveryLoop()
{


}


bool mInterfaceNetwork::Connected(uint8_t type)
{


  #ifdef USE_MODULE_NETWORK_WIFI
  if(
    (type == NETWORK_TYPE_WIFI) ||
    (type == NETWORK_TYPE_ANY)
  ){
    if(pCONT_wif->WifiCheckIpConnected())
    {
      return true;
    }
  }
  #endif // USE_MODULE_NETWORK_WIFI

/**
 * @brief WiFi needs to be first to fix extra AT commands when WiFi and Cellular are being used, but mqtt is via Wifi and not cellular
 * 
 */
  #ifdef USE_MODULE_NETWORK_CELLULAR
  if(
    (type == NETWORK_TYPE_CELLULAR) ||
    (type == NETWORK_TYPE_ANY)
  ){
    if(pCONT_cell->modem != nullptr)
    {
      if(pCONT_cell->modem->isGprsConnected())
      {
        return true;
      }
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR


  return false;
}

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mInterfaceNetwork::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR("GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      // GPS_Enable();
    }
  } 
    
}
  

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mInterfaceNetwork::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add(D_JSON_COUNT, settings.leds_found);
  return JBI->End();

}

uint8_t mInterfaceNetwork::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    // JBI->Level_Start("GPRS");
    //     JBI->Add("ConSec", gprs.connected_seconds);
    // JBI->Level_End();  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mInterfaceNetwork::MQTTHandler_Init()
{

  struct handler<mInterfaceNetwork>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mInterfaceNetwork::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mInterfaceNetwork::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mInterfaceNetwork::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}


/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mInterfaceNetwork::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}


/**
 * @brief MQTTHandler_Sender
 * */
void mInterfaceNetwork::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__NETWORK_INTERFACE__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_NETWORK_INTERFACE