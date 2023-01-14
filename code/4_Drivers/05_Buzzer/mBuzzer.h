#ifndef _MODULE_DRIVERS_BUZZER_H
#define _MODULE_DRIVERS_BUZZER_H

#define D_UNIQUE_MODULE_DRIVERS_BUZZER_ID   ((4*1000)+05)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_BUZZER

class mBuzzer :
  public mTaskerInterface
{
  public:
	  mBuzzer(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_DRIVERS_BUZZER_CTR;
    static const char* PM_MODULE_DRIVERS_BUZZER_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_BUZZER_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_BUZZER_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_BUZZER_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mBuzzer);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    

    struct BUZZER {
      uint32_t tune = 0;
      uint32_t tune_reload = 0;
      // bool active = true;
      bool enable = false;
      uint8_t inverted = 0;            // Buzzer inverted flag (1 = (0 = On, 1 = Off))
      uint8_t count = 0;               // Number of buzzes
      uint8_t mode = 0;                // Buzzer mode (0 = regular, 1 = infinite, 2 = follow LED)
      uint8_t set[2];
      uint8_t duration;
      uint8_t state = 0;
      uint8_t tune_size = 0;
      uint8_t size = 0;
      uint8_t pin = 0;
    } Buzzer;

    void BuzzerSet(uint32_t state);
    void BuzzerBeep(uint32_t count, uint32_t on, uint32_t off, uint32_t tune, uint32_t mode);
    void BuzzerSetStateToLed(uint32_t state);
    void BuzzerBeep(uint32_t count);
    void BuzzerEnabledBeep(uint32_t count, uint32_t duration);
    void BuzzerEvery100mSec();
    
    void parse_JSONCommand(JsonParserObject obj);
  

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_object_start_end_required = true);

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mBuzzer> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mBuzzer> mqtthandler_sensor_ifchanged;
    struct handler<mBuzzer> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
