#ifndef _USE_MODULE_DRIVERS_SHELLY_DIMMER_H
#define _USE_MODULE_DRIVERS_SHELLY_DIMMER_H

#define D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID ((4*1000)+15)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER

#include "Blending_Data.h"

//tmp pin fixes
// #define Pin(GPIO_SHELLY_DIMMER_RST_INV) 1
// #define Pin(GPIO_SHELLY_DIMMER_BOOT0) 0



/*
    xdrv_45_shelly_dimmer.ino - shelly dimmer support for Tasmota

    Copyright (C) 2021  James Turton

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

// #ifdef USE_LIGHT
// #ifdef USE_SHELLY_DIMMER
/*********************************************************************************************\
 * Shelly WiFi Dimmer v1 and v2 (ESP8266 w/ separate co-processor dimmer)
 *
 * {"NAME":"Shelly Dimmer 1","GPIO":[0,3200,0,3232,5568,5600,0,0,
 * 192,0,193,288,0,4736],"FLAG":0,"BASE":18}
 * {"NAME":"Shelly Dimmer 2","GPIO":[0,3200,0,3232,5568,5600,0,0,
 * 193,0,192,0,320,4736],"FLAG":0,"BASE":18}
 *
 * https://shelly.cloud/wifi-smart-home-automation-shelly-dimmer/
 * https://shelly.cloud/products/shelly-dimmer-2-smart-home-light-controller/
\*********************************************************************************************/

#define XDRV_45                     45
#define XNRG_31                     31

#define SHELLY_DIMMER_DEBUG
// #define SHELLY_HW_DIMMING

#define SHD_LOGNAME                 "SHD: "

#ifdef SHELLY_CMDS
#define D_PRFX_SHD                  "Shd"
#define D_CMND_LEADINGEDGE          "LeadingEdge"
#define D_CMND_WARMUPBRIGHTNESS     "WarmupBrightness"
#define D_CMND_WARMUPTIME           "WarmupTime"
#endif // SHELLY_CMDS

// #define SHD_SWITCH_CMD              0x01
// #define SHD_SWITCH_FADE_CMD         0x02
// #define SHD_POLL_CMD                0x10
// #define SHD_VERSION_CMD             0x11
// #define SHD_SETTINGS_CMD            0x20
// #define SHD_WARMUP_CMD              0x21
// #define SHD_CALIBRATION1_CMD        0x30
// #define SHD_CALIBRATION2_CMD        0x31

// #define SHD_SWITCH_SIZE             2
// #define SHD_SWITCH_FADE_SIZE        6
// #define SHD_SETTINGS_SIZE           10
// #define SHD_WARMUP_SIZE             4
// #define SHD_CALIBRATION_SIZE        200

// #define SHD_START_BYTE              0x01
// #define SHD_END_BYTE                0x04

// #define SHD_BUFFER_SIZE             256
// #define SHD_ACK_TIMEOUT             200 // 200 ms ACK timeout


#define SHD_SWITCH_CMD              0x01
#define SHD_SWITCH_FADE_CMD         0x02
#define SHD_POLL_CMD                0x10
#define SHD_VERSION_CMD             0x11
#define SHD_SETTINGS_CMD            0x20
#define SHD_WARMUP_CMD              0x21
#define SHD_CALIBRATION1_CMD        0x30
#define SHD_CALIBRATION2_CMD        0x31

#define SHD_SWITCH_SIZE             2
#define SHD_SWITCH_FADE_SIZE        6
#define SHD_SETTINGS_SIZE           10
#define SHD_WARMUP_SIZE             4
#define SHD_CALIBRATION_SIZE        200

#define SHD_START_BYTE              0x01
#define SHD_END_BYTE                0x04

#define SHD_BUFFER_SIZE             256
#define SHD_ACK_TIMEOUT             200 // 200 ms ACK timeout


#ifdef SHELLY_FW_UPGRADE
#include <stm32flash.h>
#endif // SHELLY_FW_UPGRADE

#include <TasmotaSerial.h>

class mShellyDimmer :
  public mTaskerInterface
{
  private:
  public:
    mShellyDimmer(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_DRIVERS_SHELLY_DIMMER_CTR = D_MODULE_DRIVERS_SHELLY_DIMMER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_SHELLY_DIMMER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mShellyDimmer);
    };
    #endif


    void init();
    void Pre_Init();
    int8_t pin = -1;
    struct SETTINGS{
      uint8_t fEnableModule = false;
      uint8_t fShowManualSlider = false;
    }settings;


    TasmotaSerial *ShdSerial = nullptr;

    typedef struct
    {
      uint8_t version_major = 0;
      uint8_t version_minor = 0;
      uint32_t brightness = 1000;
      uint32_t power = 0;
      uint32_t fade_rate = 0;
    } SHD_DIMMER;

    struct TRANSITION
    {
      LinearBlendVariable<uint16_t>*  fader = nullptr;
      uint16_t time_ms = 0;
    }transition;


    

    // // struct SHD
    // // {
    // uint8_t *buffer = nullptr;          // Serial receive buffer
    // int byte_counter = 0;               // Index in serial receive buffer
    // uint16_t req_brightness = 0;
    // bool req_on = false;
    // SHD_DIMMER dimmer;
    // uint32_t start_time = 0;
    // uint8_t counter = 1;                // Packet counter
    // uint16_t req_fade_rate = 0;
    // uint16_t leading_edge = 2;          // Leading edge = 2 Trailing edge = 1
    // uint16_t warmup_brightness = 100;   // 10%
    // uint16_t warmup_time = 20;          // 20ms
    // #ifdef USE_ENERGY_SENSOR
    //   uint32_t last_power_check = 0;      // Time when last power was checked
    // #endif // USE_ENERGY_SENSOR
    // bool hardware_serial_active = false;
    // // } param;

    
    SHD_DIMMER dimmer;
    uint8_t *buffer = nullptr;          // Serial receive buffer
    int byte_counter = 0;               // Index in serial receive buffer
// #ifdef USE_ENERGY_SENSOR
    uint32_t last_power_check = 0;      // Time when last power was checked
// #endif // USE_ENERGY_SENSOR
    uint32_t start_time = 0;
    uint16_t req_brightness = 0;
    uint16_t req_fade_rate = 0;
    uint16_t leading_edge = 2;          // Leading edge = 2 Trailing edge = 1
    uint16_t warmup_brightness = 100;   // 10%
    uint16_t warmup_time = 20;          // 20ms
    uint8_t counter = 1;                // Packet counter
    uint8_t hw_version = 0;             // Dimmer 1 = 1 Dimmer 2 = 2
    bool present = false;
    bool req_on = false;

    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif // USE_MODULE_CORE_RULES


    uint16_t checksum(uint8_t *buf, int len);
    int check_byte();
    bool SerialSend(const uint8_t data[] = nullptr, uint16_t len = 0);
    bool SendCmd(uint8_t cmd, uint8_t *payload, uint8_t len);
    void SetBrightnessReq();
    void SetBrightnessFade();
    
    void CmndShdLeadingEdge(uint8_t edge_type);

    void SendSettings();
    void SendWarmup();
    void SendCalibration(uint16_t brightness, uint16_t func, uint16_t fade_rate);
    bool SyncState();
    void DebugState();
    bool PacketProcess(void);
    void ResetToAppMode();
    void Poll(void);
    bool SendVersion(void);
    void GetSettings(void);
    void SaveSettings(void);
    bool SerialInput(void);
    bool SetPower(void);

    bool ModuleSelected(void) ;
    bool SetChannels();

    /**
     * 0 = not running
     * 1 = turn off then set to 0 to be off
     * 2+ count down
     * */
    struct TIME_ON_DECOUNTERS{
      uint16_t seconds = 0;
      uint8_t active = false;
    }timer_decounter;     
    void SubTask_Power_Time_To_Remain_On_Seconds();
    void SetBrightness(uint16_t brightness = 0);
    void CommandSet_Timer_Decounter(uint16_t time_secs);
    uint16_t CommandGet_SecondsToRemainOn();


    int8_t Tasker_Web(uint8_t function);

    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Scene(uint8_t json_level, bool json_appending);
    uint8_t ConstructJSON_State(uint8_t json_level, bool json_appending);

    void WebCommand_Parse(void);
    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);


    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();
    struct handler<mShellyDimmer>  mqtthandler_settings_teleperiod;
    struct handler<mShellyDimmer>  mqtthandler_state_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mShellyDimmer>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_teleperiod
    };

};

#endif

#endif