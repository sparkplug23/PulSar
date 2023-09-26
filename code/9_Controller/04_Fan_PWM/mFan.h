#ifndef _MCUSE_MODULE_CONTROLLER_FANEILINGFAN_H
#define _MCUSE_MODULE_CONTROLLER_FANEILINGFAN_H 0.3

#define D_UNIQUE_MODULE_CONTROLLER_FAN_PWM_ID 145

// PWM Fan and Sonoff Fan to be brought together? One fan controller, different output methods

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_FAN

#include "1_TaskerManager/mTaskerManager.h"

#include <Arduino.h>

#include <string.h>
#include <strings.h>

DEFINE_PGM_CTR(kListFanControls_pwm) "Off|Low|Medium|High";

class mFan :
  public mTaskerInterface
{

  private:
  public:
    mFan(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CONTROLLER_FAN_PWM_CTR;
    static const char* PM_MODULE_CONTROLLER_FAN_PWM_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_FAN_PWM_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_FAN_PWM_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_FAN_PWM_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSonoffIFan);
    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

int8_t Tasker_Web(uint8_t function);

uint8_t ifan_fanspeed_timer = 0;
uint8_t ifan_fanspeed_goal = 0;
bool ifan_receive_flag = false;
bool ifan_restart_flag = true;


void init();
void Pre_Init();

int8_t pin = -1;

uint16_t pwm_range_max = 1023;
uint16_t pwm_range_min = 1;

uint8_t set_fan_speed = 0;//0,1,2,3
uint16_t set_fan_pwm = 0;//0,1,2,3


struct SETTINGS{
  uint8_t fEnableModule = false;
}settings;

uint8_t GetLightState(void);
void SetLightState(uint8_t state);


bool IsModuleIfan(void);
uint8_t MaxFanspeed(void);
uint8_t GetFanspeed(void);
void SetFanspeed(uint8_t fanspeed, bool sequence);
void SonoffIfanReceived(void);
bool SerialInput(void);
void CmndFanspeed(void);
void SpeedRefresh(void);


    //void AddToJsonObject_AddHardware(JsonObject root);
    void AddToHardwareMessage();

    
void CommandSet_FanSpeed_Manual(uint16_t value);


  #ifdef USE_MODULE_NETWORK_WEBSERVER23
void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();
  #endif// USE_MODULE_NETWORK_WEBSERVER23


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mFan>* ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mFan> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mFan> mqtthandler_sensor_ifchanged;
    struct handler<mFan> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

  struct handler<mFan>* mqtthandler_list[3] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

};

#endif

#endif