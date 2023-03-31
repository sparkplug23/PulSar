
/**
 * @file mGPS_Modem.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 16-bit Ambient Light sensor
 * @version 1.0
 * @date 2022-05-01
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

const char* mGPS_Modem::PM_MODULE__SENSORS_GPS_MODEM__CTR          = D_MODULE__SENSORS_GPS_MODEM__CTR;
const char* mGPS_Modem::PM_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR = D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR;

int8_t mGPS_Modem::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;
  
  // some functions must run regardless
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
      // SubTask_ReadSensor();
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
  
  return function_result;

} // END function


void mGPS_Modem::Pre_Init(void)
{
  
  // if (pCONT_sup->I2cEnabled(XI2C_11)) 
  // {
  //   ALOG_INF( PSTR(D_LOG_BH1750 "I2cEnabled(XI2C_11)") );
  // }

}


void mGPS_Modem::Init(void)
{
  
  settings.fEnableSensor = true;

  // for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) 
  // {

  //   if (pCONT_sup->I2cActive(Bh1750.addresses[i])) { continue; }

  //   device_data[settings.sensor_count].address = Bh1750.addresses[i];

  //   if (Set_MeasurementTimeRegister(settings.sensor_count)) 
  //   {
      settings.sensor_count++;
      settings.fEnableSensor = true;
  //   }

  // }

  ALOG_INF( PSTR(D_LOG_BH1750 "Count: %d"), settings.sensor_count );

}


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
    
  for (uint32_t sensor_index = 0; sensor_index < settings.sensor_count; sensor_index++) {
    // if (device_data[sensor_index].valid) {
      // JBI->Add(D_JSON_ILLUMINANCE, device_data[sensor_index].illuminance);
      // JBI->Add(D_JSON_LEVEL, device_data[sensor_index].level);
    // }
  }

  return JBI->End();
    
}



/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * Bh1750Resolution1   0..2    - Set BH1750 1 resolution mode
 * Bh1750Resolution2   0..2    - Set BH1750 2 resolution mode
 * Bh1750MTime1       30..255  - Set BH1750 1 MT register
 * Bh1750MTime2       30..255  - Set BH1750 2 MT register
 *
 * I2C Address: 0x23 or 0x5C
\*********************************************************************************************/
void mGPS_Modem::parse_JSONCommand(JsonParserObject obj)
{

  // ALOG_DBM( PSTR(D_LOG_BH1750 "parse_JSONCommand") );
  
  char buffer[50];
  JsonParserToken jtok = 0; 


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

// void CmndBh1750Resolution(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= settings.sensor_count)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//       if (1 == XdrvMailbox.index) {
//         Settings.SensorBits1.bh1750_1_resolution = XdrvMailbox.payload;
//       } else {
//         Settings.SensorBits1.bh1750_2_resolution = XdrvMailbox.payload;
//       }
//       Set_Resolution(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(Get_Resolution(XdrvMailbox.index -1));
//   }
// }

  // if(jtok = obj[PM_JSON_BRIGHTNESS]){ // Assume range 0-100
  //   CommandSet_Brt_255(mapvalue(jtok.getInt(), 0,100, 0,255));
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), brt);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

// void CmndBh1750MTime(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= settings.sensor_count)) {
//     if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
//       device_data[XdrvMailbox.index -1].mtreg = XdrvMailbox.payload;
//       Set_MeasurementTimeRegister(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(device_data[XdrvMailbox.index -1].mtreg);
//   }
// }

}

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
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

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mGPS_Modem::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mGPS_Modem::ConstructJSON_Sensor;
  
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
void mGPS_Modem::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__SENSORS_GPS_MODEM__ID, handle, id);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/






#endif

