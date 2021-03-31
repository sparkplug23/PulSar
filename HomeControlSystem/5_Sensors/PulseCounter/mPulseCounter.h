#ifndef mPulseCounter_H
#define mPulseCounter_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_PULSE_COUNTER

#include "stdint.h"

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/Support/mSupport.h"

#ifdef USE_MODULE_CORE_WEBSERVER
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif //USE_MODULE_CORE_WEBSERVER

#include "1_TaskerManager/mTaskerInterface.h"

class mPulseCounter :
  public mTaskerInterface
{

  private:
  public:
    mPulseCounter();
    // Configure pins, call sensor sub classes and init if need be
    void Pre_Init(void);
    // Configure variables
    void Init(void);
    #define MAX_SENSORS 4
    int8_t pin[MAX_SENSORS] = {-1,-1,-1,-1};
    
    uint8_t fEnableSensor= false;

    //#define D_MODULE_TOPIC_NAME "pulsecounter"

    // #define WEB_HANLDE_JSON_DHT_SENSOR_TABLE "/fetch/tab_dht_sensor.json"
    void EveryLoop();

    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();


    uint8_t fSensorCount = 0; // 0 also means not enabled


    uint8_t WITHINLIMITS(int minv, float var, int maxv);

     void SplitTask_UpdatePulseCounterSensors(uint8_t sensor_id, uint8_t require_completion);;


    char name_buffer[100]; //use single 1D klist array in memory


     int8_t Tasker(uint8_t function);

    #define DONE_COMPLETE false
    #define DONE_NOTCOMPLETE true
    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    float tmp_float;

    struct SETTINGS{
      uint16_t max_reading_ms = 2000;
      uint16_t min_reading_ms = 5;
    }settings;

    struct SENSORDATA{
      //DHTesp* dht;
      struct TYPE{
        uint16_t timing_rise_ms = 0;
        uint16_t timing_fall_ms = 0;
        uint8_t state = 0;

        uint8_t isvalid=false;
        uint8_t ischanged=false;
        uint8_t ischanged_over_threshold=false; //phase out and just add changed percentage guards ie change more than 0.1 or its lowest precision
        // time later 
        uint32_t tWithinLimit;
        uint32_t tSavedMeasureClimate;
        uint32_t ischangedtLast = millis();
        uint8_t fWithinLimit;
        uint8_t sUpdateClimateSensors;

  uint8_t  isactive = false;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  uint32_t tDeltaTime;
  char     friendly_name_ctr[15];
  char     detected_rtc_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED



      };
      TYPE instant;
      TYPE roc10s; // rate of change
      TYPE roc1m; // rate of change
      TYPE roc10m; // rate of change
      // const char* name_ptr = nullptr;
    }sensor[MAX_SENSORS];
    
// typedef struct pir_detect_s{
//   uint8_t  state = false;
//   uint8_t  isactive = false;
//   uint8_t  ischanged = false;
//   uint32_t tDetectTime;
//   uint32_t tEndedTime;
//   char     friendly_name_ctr[15];
//   char     detected_rtc_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED
// }pir_detect_t;

    
uint8_t Change_Detected(uint8_t sensor_id);

    const char* sensor_unset_ctr = "unset";

    void WebPage_Root_AddHandlers();
    void WebPage_Root_SendStatus();



    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mPulseCounter>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mPulseCounter> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mPulseCounter> mqtthandler_sensor_ifchanged;
    struct handler<mPulseCounter> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

  
};
#endif

#endif
