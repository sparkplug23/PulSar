/*
  mPZEM004T.h - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael

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
*/
#ifndef HEADER_DRIVERS_RF433_RCSWITCH_EXTENDED_H
#define HEADER_DRIVERS_RF433_RCSWITCH_EXTENDED_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID ((4*1000)+10) 

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include <RCSwitchExtended.h>

class mRCSwitch :
  public mTaskerInterface
{

  private:
  public:
    mRCSwitch(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_DRIVERS_RF433_RCSWITCH_CTR;
    static const char* PM_MODULE_DRIVERS_RF433_RCSWITCH_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_RF433_RCSWITCH_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_RF433_RCSWITCH_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mRCSwitch);
    };
    #endif

    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t found = 0;
      // uint16_t rate_measure_ms = 1000;
      // // uint8_t sensor_count = 1;
      // uint8_t active_sensor = 0;
    }settings;

    #define D_JSON_RF_PROTOCOL "Protocol"
    #define D_JSON_RF_BITS "Bits"
    #define D_JSON_RF_DATA "Data"
    #define D_JSON_RF_PULSE "Pulse"


    #define RF_TIME_AVOID_DUPLICATE 1000  // Milliseconds
  
    RCSwitch *mySwitch = nullptr;//RCSwitch();

    void Pre_Init();
    void Init();
    
    void SubTask_SendCommand_Up();
    void SubTask_SendCommand_Up_PreTrain();
    void SubTask_SendCommand_Up_Block(int repeats);
    void SubTask_SendCommand_Up_Block2(int repeats);

    void ReceiveCheck(void);
    void parse_JSONCommand(JsonParserObject obj);

    struct RECEIVED_PACKET
    {
      uint32_t data = 0;
      uint16_t bit_length = 0;
      int16_t  protocol = -1;
      uint16_t delay = 0;
      uint32_t received_time_millis = 0;
      uint32_t received_utc_time;
    }rx_pkt;

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_State(uint8_t json_method = 0);
    
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mRCSwitch> mqtthandler_settings_teleperiod;
    struct handler<mRCSwitch> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
      
    struct handler<mRCSwitch>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };




};

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH

#endif
