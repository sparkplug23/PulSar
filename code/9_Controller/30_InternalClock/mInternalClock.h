#ifndef _CONTROLLER_INTERNAL_CLOCK_H
#define _CONTROLLER_INTERNAL_CLOCK_H

#define D_UNIQUE_MODULE_CONTROLLER_INTERNAL_CLOCK_ID ((9*1000)+30)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_INTERNAL_CLOCK

//const char TEST_DLIST[] PROGMEM = {D_JSON_LIGHTPOWER "|" D_JSON_FANSPEED"|" D_JSON_FANSPEED"|" D_JSON_FANSPEED"|" D_JSON_FANSPEED};
// DEFINE_PGM_CTR(kListFanControls) "Light|Off|Low|Medium|High";

const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)
const uint8_t kIFan03Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
const uint8_t kIFan03Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};


class mInternalClock :
  public mTaskerInterface
{

  private:
  public:
    mInternalClock(){};
    void init();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);

    static constexpr const char* PM_MODULE_CONTROLLER_CEILINGFAN_CTR = D_MODULE_CONTROLLER_CEILINGFAN_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CEILINGFAN_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mInternalClock);
    };
    #endif
    struct SETTINGS{
      uint8_t fEnableModule = false;
    }settings;

    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ifan_fanspeed_timer = 0;
    uint8_t ifan_fanspeed_goal = 0;
    bool ifan_receive_flag = false;
    bool ifan_restart_flag = true;


    uint8_t GetLightState(void);
    void SetLightState(uint8_t state);

    uint8_t GetFanspeed(void);
    void SetFanSpeed(uint8_t fanspeed, bool sequence);
    void SonoffIfanReceived(void);
    bool SerialInput(void);
    void SpeedRefresh(void);

    void RulesEvent_Set_Speed();
    
    void WebCommand_Parse(void);
    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Power(uint8_t json_level = 0, bool json_appending = true);

  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();

    struct handler<mInternalClock> mqtthandler_settings_teleperiod;
    struct handler<mInternalClock> mqtthandler_power_ifchanged;
    struct handler<mInternalClock> mqtthandler_power_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mInternalClock>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_power_ifchanged,
      &mqtthandler_power_teleperiod
    };

};

#endif

#endif