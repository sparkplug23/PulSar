#ifndef _MRADIATORFAN_H
#define _MRADIATORFAN_H 0.3

#define D_UNIQUE_MODULE_CONTROLLER_RADIATORFAN_ID 175

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN


#define FAN_RELAY_PIN RELAY_0_PIN 

#define SET_FAN_RELAY(x)  digitalWrite(FAN_RELAY_PIN,x) //opened when high
#define FAN_ON()          SET_FAN_RELAY(LOW) //change to be function later, so we can enable this for night only
#define FAN_OFF()         SET_FAN_RELAY(HIGH)
#define FAN_RELAY_INIT()  pinMode(FAN_RELAY_PIN,OUTPUT)
#define FAN_ACTIVE()      !digitalRead(FAN_RELAY_PIN) //opened when high
#define FAN_ACTIVE_CTR    FAN_ACTIVE() ? "1" : "0"


#define RAD_FAN_RELAY_ON HIGH //holds active state
#define RAD_FAN_RELAY_OFF LOW //holds active state

class mRadiatorFan :
  public mTaskerInterface
{
  public:
    mRadiatorFan(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    struct FANSETTINGS{
      uint8_t onoff=0;
      uint8_t secs;
      uint8_t savedstate = false;
      uint8_t ischanged = false;
    }fan;

    static const char* PM_MODULE_CONTROLLER_RADIATORFAN_CTR;
    static const char* PM_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_RADIATORFAN_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_RADIATORFAN_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mRadiatorFan);
    };
    #endif
    
    void SubTasker_MQTTSender();

    enum FAN_MODE{MANUAL=1,AUTO_TEMP,AUTO_REMOTE};
    uint8_t fan_mode = AUTO_TEMP;
    const char* FanModeCtr();

    uint32_t tTick,tCheckForMaxTemp,tSaved;

    void ConstructJSON_FanStatus();
    void MQQTSendFanStatus();
    void parse_JSONCommand(JsonParserObject obj);

    // void AddToJsonObject_AddHardware(JsonObject root);
    void Append_Hardware_Status_Message();

    int8_t CheckAndExecute_JSONCommands();


    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mRadiatorFan>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mRadiatorFan> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mRadiatorFan> mqtthandler_sensor_ifchanged;
    struct handler<mRadiatorFan> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif //DEVICE_RADIATORFAN

#endif
