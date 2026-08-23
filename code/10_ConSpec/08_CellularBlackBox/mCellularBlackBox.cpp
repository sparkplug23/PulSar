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

#include "mCellularBlackBox.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX

int8_t mCellularBlackBox::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableSensor){ return TASKER_RESULT__MODULE_DISABLED_ID; }

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



void mCellularBlackBox::Pre_Init(void)
{

}


void mCellularBlackBox::Init(void)
{
  
    settings.fEnableSensor = true;

    
}


void mCellularBlackBox::EverySecond()
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
void mCellularBlackBox::SubTask_UpdateOLED()
{
  
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[30] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  tkr_iDisp->renderer->setTextSize(1);



  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);

  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);

  char convf_altitude[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.altitude,1,convf_altitude);
  char convf_speed_kph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.speed,1,convf_speed_kph);
  char convf_speed_mph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.speed*0.621371,1,convf_speed_mph);


  char convf_accuracy[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.accuracy,1,convf_accuracy);
  char convf_usat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.usat,0,convf_usat);
  char convf_vsat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_gps->location.vsat,0,convf_vsat);

  char convf_gprs_upsecs[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_cell->gprs.connected_seconds,0,convf_gprs_upsecs);

  char convf_batt_mv[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_cell->modem_status.battery.volts_mv,0,convf_batt_mv);
  char convf_batt_per[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(tkr_cell->modem_status.battery.percentage,0,convf_batt_per);

  uint16_t gps_seconds_updated = 0;
  uint32_t gps_millis_updated = abs(millis()-tkr_gps->readings.update_seconds);
  gps_millis_updated /= 1000;
  uint32_t gprs_millis_updated = abs(millis()-tkr_cell->gprs.last_comms_millis_updated);
  gprs_millis_updated /= 1000;
 
  
  /**
   * @brief 21x8
   * 
   */

  // rintf(buffer, sizeof(buffer), "123456789123456789123"); tkr_iDisp->LogBuffer_AddRow(buffer, 0);
  snprintf(buffer, sizeof(buffer), "Lat %s", convf_lat); tkr_iDisp->LogBuffer_AddRow(buffer, 0);
  snprintf(buffer, sizeof(buffer), "Lon %s", convf_lon); tkr_iDisp->LogBuffer_AddRow(buffer, 1);
  snprintf(buffer, sizeof(buffer), "Alt %sm  Spd %smph", convf_altitude, convf_speed_mph); tkr_iDisp->LogBuffer_AddRow(buffer, 2);  
  snprintf(buffer, sizeof(buffer), "Acc %sm Fix %s|%s", convf_accuracy, convf_usat, convf_vsat); tkr_iDisp->LogBuffer_AddRow(buffer, 3);  
  snprintf(buffer, sizeof(buffer), "GPRS u%s secs", convf_gprs_upsecs);  tkr_iDisp->LogBuffer_AddRow(buffer, 4);
  snprintf(buffer, sizeof(buffer), "Bat %s mv, %s%%", convf_batt_mv, convf_batt_per);  tkr_iDisp->LogBuffer_AddRow(buffer, 5);
  snprintf(buffer, sizeof(buffer), "GT %02d:%02d:%02d", tkr_gps->location.hour, tkr_gps->location.minute, tkr_gps->location.second);  tkr_iDisp->LogBuffer_AddRow(buffer, 6);
  
  if(
    (gps_millis_updated<10)&&
    (gprs_millis_updated<10)
  ){
    snprintf(buffer, sizeof(buffer), "OH %s  %s", display_message.line7, "GOOD");  tkr_iDisp->LogBuffer_AddRow(buffer, 7);
  }else{
    snprintf(buffer, sizeof(buffer), "OH %s  m%d sec", display_message.line7, gps_millis_updated);  tkr_iDisp->LogBuffer_AddRow(buffer, 7);
  }




}


/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mCellularBlackBox::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  if(jtok = obj["OpenHAB"].getObject()["SystemTime"])
  {
    snprintf(display_message.line7, sizeof(display_message.line7), "%s", jtok.getStr());
  }
    
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mCellularBlackBox::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);    
  return JBI->End();

}

uint8_t mCellularBlackBox::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mCellularBlackBox::Telemetry_Init()
{

  struct telemetry_handler<mCellularBlackBox>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellularBlackBox::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellularBlackBox::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 


/**
 * @brief Set flag for all telemetryhandlers to send
 * */
void mCellularBlackBox::MQTTHandler_RefreshAll()
{
  for(auto& handle:telemetry_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellularBlackBox::MQTTHandler_Rate()
{
  for(auto& handle:telemetry_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mCellularBlackBox::MQTTHandler_Sender()
{
  for(auto& handle:telemetry_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX