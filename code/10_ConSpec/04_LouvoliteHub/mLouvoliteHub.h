/*
  mLouvoliteHub.h - Louvolite Blinds wireless control with SAW 433MHz transmitter

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
#ifndef HEADER_CONTROLLER_LOUVOLITE_HUB_H
#define HEADER_CONTROLLER_LOUVOLITE_HUB_H

/**
 * @file mLouvoliteHub.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * @note: When decoding signals, the remote (paired to blind!) will transmit using pattern 
 * { 20,  16, { 14, 30 }, 1, { 510,  30 }, { 14,  30 }, { 30, 14 }, false,  230 }  // 36 (Louvolite) with premable
 * The original command sends the preamble, then 10 repeated codes. The solution here sends the preamble prior to each repeated code.
 * 
 * Although a different code is send for "long button press for blind set to limit" transmitting the up/down command with 300ms gap between blocks of retransmits 
 * is enough to make the blind reach limit. Note, right now, untested with more than 1 intermediate level, so perhaps this only means go to the next level which happens to be the limit here. 
 * The second code may still need to be decoded.
 * 
 * Code is 64 bits, plus 1 CRC bit (for total of 65). Do not include the 65th bit or uint64_t bit roll over will occur.
 * 
 */
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_LOUVOLITE_HUB_ID 10004 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include <RCSwitchExtended.h>

DEFINE_PGM_CTR(PM_LOUVOLITE_COMMAND__STOP__CTR) "Stop";
DEFINE_PGM_CTR(PM_LOUVOLITE_COMMAND__UP_STEP__CTR) "Up";
DEFINE_PGM_CTR(PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR) "Down";
DEFINE_PGM_CTR(PM_LOUVOLITE_COMMAND__UP_FULL__CTR) "Open";
DEFINE_PGM_CTR(PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR) "Close";

class mLouvoliteHub :
  public mTaskerInterface
{

  private:
  public:
    mLouvoliteHub(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR = D_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_LOUVOLITE_HUB_ID; }
    
    
    #define PULSE_SHORT 280
    #define PULSE_LONG 600 

    #define D_LOUVOLITE_BUTTON_UP_BINARY_CTR    "1010110010100000111100101111011100111111000000000000101111010011"
    #define D_LOUVOLITE_BUTTON_UP_HEX           (uint64_t)0xACA0F2F73F000BD3
    #define D_LOUVOLITE_BUTTON_DOWN_BINARY_CTR  "1010110010100000111100101111011100111111000000000100001100001011"
    #define D_LOUVOLITE_BUTTON_DOWN_HEX         (uint64_t)0xACA0F2F73F00430B
    #define D_LOUVOLITE_COMMAND__STOP__BINARY_CTR  "1010110010100000111100101111011100111111000000000010001111101011"
    #define D_LOUVOLITE_COMMAND__STOP__HEX         (uint64_t)0xACA0F2F73F0023EB


/**
 * @brief 
 * 
 * My Bedroom is using remote with 2 pin soldered, channel 1
 * MasterBedroom is using new remote on channel 6
 * 
 * 
 */
    struct PAIRED_BLINDS_CODES{
      uint64_t up = 0;
      uint64_t down = 0;
      uint64_t stop = 0;
    }paired_codes[2];

    enum COMMAND_IDS{
      COMMAND__STOP__ID,
      COMMAND__UP_STEP__ID,
      COMMAND__DOWN_STEP__ID,
      COMMAND__UP_FULL__ID,
      COMMAND__DOWN_FULL__ID,
      COMMAND__LENGTH__ID
    };

    
    int16_t CommandMode_GetID_by_Name(const char* c);
    const char* CommandMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen);

    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t found = 0;
      // uint16_t rate_measure_ms = 1000;
      // // uint8_t sensor_count = 1;
      // uint8_t active_sensor = 0;
    }settings;

    uint8_t last_position_command = COMMAND__LENGTH__ID;

    void SubTask_SlowOpen_Mode();
    uint32_t tSaved_millis_mode_slow_open_start = millis();
    uint32_t tSaved_millis_mode_slow_open_last = millis();
    uint32_t tSaved_millis_mode_slow_open_increment = millis();
    uint16_t stop_delay_ms = 0;
    bool enable_slow_open = false;
    bool start_slow_open = false;

        
    void CommandSet_BlindTransmit_ByID(int8_t id);
  
    RCSwitch *transceive = nullptr;

    void Pre_Init();
    void Init();
    
    void CommandSet_BlindTransmit_Up_Step();
    void CommandSet_BlindTransmit_Down_Step();
    void CommandSet_BlindTransmit_Up_Full();
    void CommandSet_BlindTransmit_Down_Full();
    void CommandSet_BlindTransmit_Stop();

    void ReceiveCheck(void);
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();

    struct handler<mLouvoliteHub> mqtthandler_settings;
    struct handler<mLouvoliteHub> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mLouvoliteHub>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged
    };




};

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH

#endif
