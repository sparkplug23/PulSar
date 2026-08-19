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
#ifndef HEADER_DRIVERS_RF433_CODES_EXTENDED_H
#define HEADER_DRIVERS_RF433_CODES_EXTENDED_H
  
#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE__DRIVERS__RF433_CODES_ID 4010 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_DRIVERS_RF433_CODES

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include <RCSwitchExtended.h>

class mRF433Codes :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mRF433Codes(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_DRIVERS__RF433_CODES_CTR = D_MODULE_DRIVERS__RF433_CODES_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__RF433_CODES_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DRIVERS__RF433_CODES_ID; }
   
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    bool Default_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM
    
    struct RECEIVED_PACKET
    {
      uint32_t data = 0;
      uint16_t bit_length = 0;
      int16_t  protocol = -1;
      uint16_t delay = 0;
      uint32_t received_time_millis = 0;
      uint32_t received_utc_time;
    }rx_pkt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
    
    RCSwitch *mySwitch = nullptr;

    #define RF_TIME_AVOID_DUPLICATE 1000  // Milliseconds
    #define D_RF_PROTOCOL           "Protocol"
    #define D_RF_BITS               "Bits"
    #define D_RF_DATA               "Data"
    #define D_RF_PULSE              "Pulse"

    void ReceiveCheck(void);

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

     void SubTask_SendCommand_Up();
     void SubTask_SendCommand_Up_PreTrain();
     void SubTask_SendCommand_Up_Block(int repeats);
     void SubTask_SendCommand_Up_Block2(int repeats);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT 
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mRF433Codes>*> telemetry_list;
    struct telemetry_handler<mRF433Codes> telemetry_settings;
    struct telemetry_handler<mRF433Codes> telemetry_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif // USE_MODULE_DRIVERS_RF433_CODES

#endif
