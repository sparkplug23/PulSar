#ifndef _MCEILINGFAN_H
#define _MCEILINGFAN_H 0.3

#define D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID 179

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN

//const char TEST_DLIST[] PROGMEM = {D_JSON_LIGHTPOWER "|" D_JSON_FANSPEED"|" D_JSON_FANSPEED"|" D_JSON_FANSPEED"|" D_JSON_FANSPEED};
// DEFINE_PGM_CTR(kListFanControls) "Light|Off|Low|Medium|High";

const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)
const uint8_t kIFan03Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
const uint8_t kIFan03Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};


class mSonoffIFan :
  public mTaskerInterface
{

  private:
  public:
    mSonoffIFan(){};
    void init();
    int8_t Tasker(uint8_t function);
    int8_t Tasker_Web(uint8_t function);

    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);

    static const char* PM_MODULE_CONTROLLER_CEILINGFAN_CTR;
    static const char* PM_MODULE_CONTROLLER_CEILINGFAN_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CEILINGFAN_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_CEILINGFAN_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSonoffIFan);
    };
    #endif
    struct SETTINGS{
      uint8_t fEnableModule = false;
    }settings;

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

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mSonoffIFan> mqtthandler_settings_teleperiod;
    struct handler<mSonoffIFan> mqtthandler_sensor_ifchanged;
    struct handler<mSonoffIFan> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif