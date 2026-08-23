/**

  Copyright (C) 2022    Michael Doone

  @note Relys on all other modules (no timers), and acts in place of rules

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "Custom.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT

int8_t mCustom::Tasker(uint8_t function, JsonParserObject obj){

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
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif 
  }

  return TASKER_RESULT__UNKNOWN_ID;

}//end



void mCustom::Pre_Init(void)
{

}


void mCustom::Init(void)
{
  
  module_state.mode = ModuleStatus::Running;

}


void mCustom::EverySecond()
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
void mCustom::SubTask_UpdateOLED()
{
  
  tkr_iDisp->display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetTime(DT_LOCAL_TIME).c_str());
  tkr_iDisp->LogBuffer_AddRow(buffer, 3);

  // #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306

  float sensor_data = -1;

  /**
   * @brief Add each sensor on new line
   */
   
  uint8_t sensors_available = tkr_bme->GetSensorCount();
  for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  {
    sensors_reading_t val;
    tkr_bme->GetSensorReading(&val, sensor_id);
    if(val.Valid())
    {

      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);        
      DLI->GetDeviceName_WithModuleUniqueID( tkr_bme->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

      /**
       * @brief Check for name and replace with OLED friendly short name
       **/
      if(strcmp(buffer_n, D_DEVICE_SENSOR_BME_LONG_WIRE_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "AMB");
      }
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
      tkr_iDisp->LogBuffer_AddRow(buffer, 0);
    
    }

  }

  {
    uint8_t sensors_available = tkr_db18->GetSensorCount();
    for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
    {
      sensors_reading_t val;
      tkr_db18->GetSensorReading(&val, sensor_id);
      if(val.Valid())
      {

        sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);        
        DLI->GetDeviceName_WithModuleUniqueID( tkr_db18->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

        if(strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_01_NAME)==0)
        {
          memset(buffer_n, 0, sizeof(buffer_n));
          sprintf(buffer_n, "%s", " 2m");
          snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
          tkr_iDisp->LogBuffer_AddRow(buffer, 1);
        }else 
        if(strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_02_NAME)==0)
        {
          memset(buffer_n, 0, sizeof(buffer_n));
          sprintf(buffer_n, "%s", " 3m");
          snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
          tkr_iDisp->LogBuffer_AddRow(buffer, 2);
        }

      
      }

    }
  }













  // #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}


/******************************************************************************************************************
 * mCustom_Commands.cpp
*******************************************************************************************************************/


void mCustom::parse_JSONCommand(JsonParserObject obj)
{

    
}


/******************************************************************************************************************
 * mCustom_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mCustom::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
   
  return JBI->End();

}

uint8_t mCustom::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mCustom_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mCustom::Telemetry_Init()
{

  struct telemetry_handler<mCustom>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCustom::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCustom::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE