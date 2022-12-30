/**
  mSideDoorLight.ino - RF transceiver using RcSwitch library

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

#include "mSideDoorLight.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS

const char* mSideDoorLight::PM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_CTR = D_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_CTR;
const char* mSideDoorLight::PM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_FRIENDLY_CTR = D_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_FRIENDLY_CTR;


int8_t mSideDoorLight::Tasker(uint8_t function, JsonParserObject obj){

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



void mSideDoorLight::Pre_Init(void)
{

}


void mSideDoorLight::Init(void)
{
  
    settings.fEnableSensor = true;

}



#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH