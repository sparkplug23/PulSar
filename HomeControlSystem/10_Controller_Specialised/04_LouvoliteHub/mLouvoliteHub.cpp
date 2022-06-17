/**
  mLouvoliteHub.ino - RF transceiver using RcSwitch library

  Copyright (C) 2022    Michael Doone

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

#include "mLouvoliteHub.h"

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB

const char* mLouvoliteHub::PM_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR = D_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR;
const char* mLouvoliteHub::PM_MODULE_CONTROLLER_LOUVOLITE_HUB_FRIENDLY_CTR = D_MODULE_CONTROLLER_LOUVOLITE_HUB_FRIENDLY_CTR;


int8_t mLouvoliteHub::Tasker(uint8_t function, JsonParserObject obj){

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

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_100_MSECOND:
      SubTask_SlowOpen_Mode();
    break;
    case FUNC_EVERY_SECOND:
      // Perhaps adding method for telling the blind to open in steps, for "slower" opening. ie, opens bottom 10% in steps, then opens fully.
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
    #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif  
  }

}//end


/**
 * @brief When commanded, the millis from start will be recorded, and each stepped movement will be relative to that start
 * 
 */
void mLouvoliteHub::SubTask_SlowOpen_Mode()
{

  if(start_slow_open == true)
  {
    uint32_t elapsed_time = millis() - tSaved_millis_mode_slow_open_start;


    // if(elapsed_time < 1000)
    // {


    // }else
    if(elapsed_time >20000)
    {


    }

    
    if(mTime::TimeReached(&tSaved_millis_mode_slow_open_increment, 2000))
    {
      CommandSet_BlindTransmit_Up_Step();
      ALOG_INF(PSTR("Up Step %d, %d"),tSaved_millis_mode_slow_open_increment,stop_delay_ms);
    }

    /**
     * @brief Stop limit reached
     **/
    if(mTime::TimeReached(&tSaved_millis_mode_slow_open_last, 1000))
    {
      CommandSet_BlindTransmit_Stop();
      ALOG_INF(PSTR("Stopping %d,%d"), tSaved_millis_mode_slow_open_last, stop_delay_ms);
      // stop_delay_ms = 1000;
      // tSaved_millis_mode_slow_open_increment = millis();
    }

    if(elapsed_time >20000)
    {
      start_slow_open = false;
      ALOG_INF(PSTR("start_slow_open = false;"));
    }
    

  }


}

void mLouvoliteHub::Pre_Init(void)
{

}


void mLouvoliteHub::Init(void)
{
  
  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_TX_ID))
  {
    transceive = new RCSwitch();
    transceive->enableTransmit(pCONT_pins->GetPin(GPIO_RF_433MHZ_TX_ID));
    transceive->setRepeatTransmit(10);
    settings.fEnableSensor = true;
  }

}


int16_t mLouvoliteHub::CommandMode_GetID_by_Name(const char* c)
{
  if(c=='\0'){    return -1; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__STOP__CTR)==0){ return COMMAND__STOP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__UP_STEP__CTR)==0){ return COMMAND__UP_STEP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR)==0){ return COMMAND__DOWN_STEP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__UP_FULL__CTR)==0){ return COMMAND__UP_FULL__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR)==0){ return COMMAND__DOWN_FULL__ID; }
  return -1;
}


const char* mLouvoliteHub::CommandMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen)
{
  switch(id){
    default:
    case COMMAND__STOP__ID:      memcpy_P(buffer, PM_LOUVOLITE_COMMAND__STOP__CTR, sizeof(PM_LOUVOLITE_COMMAND__STOP__CTR)); break;
    case COMMAND__UP_STEP__ID:   memcpy_P(buffer, PM_LOUVOLITE_COMMAND__UP_STEP__CTR, sizeof(PM_LOUVOLITE_COMMAND__UP_STEP__CTR)); break; 
    case COMMAND__DOWN_STEP__ID: memcpy_P(buffer, PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR, sizeof(PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR)); break; 
    case COMMAND__UP_FULL__ID:   memcpy_P(buffer, PM_LOUVOLITE_COMMAND__UP_FULL__CTR, sizeof(PM_LOUVOLITE_COMMAND__UP_FULL__CTR)); break; 
    case COMMAND__DOWN_FULL__ID: memcpy_P(buffer, PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR, sizeof(PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR)); break; 
  }
  return buffer;
}


#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH