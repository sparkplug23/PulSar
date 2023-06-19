#ifndef _MCEILINGFAN_H
#define _MCEILINGFAN_H

#define D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID ((9*1000)+03)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN


class mSonoffIFan :
  public mTaskerInterface
{

  private:
  public:
    mSonoffIFan(){};
    void Init();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);

    static const char* PM_MODULE_CONTROLLER_CEILINGFAN_CTR;
    static const char* PM_MODULE_CONTROLLER_CEILINGFAN_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CEILINGFAN_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_CEILINGFAN_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mSonoffIFan);    };
    #endif

    struct SETTINGS{
      uint8_t fEnableModule = false;
    }settings;

    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ifan_fanspeed_timer = 0;
    uint8_t ifan_fanspeed_goal = 0;
    bool ifan_receive_flag = false;
    bool ifan_restart_flag = true;
        
    #define MAX_FAN_SPEED   4
    const uint8_t kIFan03Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
    const uint8_t kIFan03Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};

    Decounter<uint16_t>* time_on = new Decounter<uint16_t>();

    uint8_t GetLightState(void);
    void SetLightState(uint8_t state);

    uint8_t GetFanspeed(void);
    void SetFanSpeed(uint8_t fanspeed, bool sequence);

    void SonoffIfanReceived(void);
    bool SerialInput(void);
    void SpeedRefresh(void);

    void EverySecond();

    void RulesEvent_Set_Speed();
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Power(uint8_t json_level = 0, bool json_appending = true);

    #ifdef USE_MODULE_NETWORK_MQTT
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    std::vector<struct handler<mSonoffIFan>*> mqtthandler_list;

    struct handler<mSonoffIFan> mqtthandler_settings_teleperiod;
    struct handler<mSonoffIFan> mqtthandler_power_ifchanged;
    struct handler<mSonoffIFan> mqtthandler_power_teleperiod;

    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif