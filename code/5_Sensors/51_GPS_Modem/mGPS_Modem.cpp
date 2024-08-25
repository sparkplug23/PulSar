
/**
 * @file mGPS_Modem.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 16-bit Ambient Light sensor
 * @version 1.0
 * @date 2022-05-01
 * 
 * 
 * This file should just hold the GPS data, that is populated by the driver for the modem
 * So really should GPS_Modem simply become a driver? 
 * Though different modems may/maynot have GPS.
 * 
 * SensorInterface in this sense needs a way to hold (like mavlink log) a basic "system position data". May not be practical here.
 * 
 * 
 * For now, keep this file as simply a storage location for the GPS data and reporting. It could easily just become an interface mqtt payload/constructor.
 * 
 * 
 * 
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mGPS_Modem.h"

#ifdef USE_MODULE_SENSORS_GPS_MODEM

int8_t mGPS_Modem::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
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
    case FUNC_EVERY_SECOND:
      /**
       * @brief Temp fix, manually get from modem
       */
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
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return FUNCTION_RESULT_SUCCESS_ID;

} // END function


void mGPS_Modem::Pre_Init(void)
{
  
}


void mGPS_Modem::Init(void)
{
  
  settings.sensor_count++;
  settings.fEnableSensor = true;

  ALOG_INF( PSTR(D_LOG_BH1750 "Count: %d"), settings.sensor_count );

}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void mGPS_Modem::parse_JSONCommand(JsonParserObject obj)
{

  // ALOG_DBM( PSTR(D_LOG_BH1750 "parse_JSONCommand") );
  
  char buffer[50];
  JsonParserToken jtok = 0; 

}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mGPS_Modem::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  JBI->Add(D_JSON_COUNT, settings.sensor_count);    
  // for (uint32_t ii = 0; ii < settings.sensor_count; ii++)
  // {
  //   JBI->Add(D_JSON_ADDRESS, device_data[ii].address);
  // }
  return JBI->End();

}

uint8_t mGPS_Modem::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    
  JBI->Add("Altitude", location.altitude);
  JBI->Add("Latitude", location.latitude);
  JBI->Add("Longitude", location.longitude);

  return JBI->End();
    
}
  
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mGPS_Modem::MQTTHandler_Init()
{

  struct handler<mGPS_Modem>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mGPS_Modem::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mGPS_Modem::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mGPS_Modem::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mGPS_Modem::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mGPS_Modem::MQTTHandler_Set_DefaultPeriodRate()
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
void mGPS_Modem::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__SENSORS_GPS_MODEM__ID, handle);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT


/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/






#endif

