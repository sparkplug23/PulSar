#ifndef MSENSORSDHT_H
#define MSENSORSDHT_H 0.2

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_DHT

#include "stdint.h"
// #include "2_CoreSystem/Time/mTime.h"
// class mTime;
#include "DHTesp.h"
class DHTesp;
#include "1_TaskerManager/mTaskerManager.h"

#include <ArduinoJson.h>

#ifdef USE_MODULE_CORE_WEBSERVER
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif //USE_MODULE_CORE_WEBSERVER
class mSensorsDHT{

  private:
  public:
    mSensorsDHT(){};
    // Configure pins, call sensor sub classes and init if need be
    void Pre_Init(void);
    // Configure variables
    void Init(void);
    #define MAX_SENSORS 4
    int8_t pin[MAX_SENSORS] = {-1,-1,-1,-1};
    // DHTesp* dht[MAX_SENSORS];

    
  enum {
    AUTO_DETECT,
    DHT11,
    DHT22,
    AM2302,  // Packaged DHT22
    RHT03    // Equivalent to DHT22
  };

    uint8_t fEnableSensor= false;

    //#define D_MODULE_TOPIC_NAME "dht"

    // #define WEB_HANLDE_JSON_DHT_SENSOR_TABLE "/fetch/tab_dht_sensor.json"
    void EveryLoop();

    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();

    // uint8_t fSensorCount = 0; // 0 also means not enabled

    struct SETTINGS{
      uint8_t sensor_active_count = 0;
    }settings;

// struct DHTSTRUCT {
//   uint8_t     pin;
//   uint8_t     type;
//   char     stype[12];
//   uint32_t lastreadtime;
//   uint8_t  lastresult;
//   float    t = NAN;
//   float    h = NAN;
// } Dht[MAX_SENSORS];



    // uint32_t tSavedSendClimate,tSavedSendClimate1m;
    // void MQQTSendClimateTempsIfChanged();
    // void MQQTSendClimateTempsLatest1m();
    // uint8_t mSavedSendClimate1m=0;
    // //void ConstructJSON_ClimateTemps();
    // void ConstructJSON_ClimateTemps();
    // void MQQTSendClimateTempsROC1m();
    // void ConstructJSON_ClimateTempsROC1m();
    // void MQQTSendClimateTempsROC10m();
    // void ConstructJSON_ClimateTempsROC10m();
    // void SubTasker_MQTTSender();
    // uint8_t mqtt_start_data_sent = 0;
    // void MQQTSendClimateTempsLatest10m();



    uint8_t WITHINLIMITS(int minv, float var, int maxv);

     void SplitTask_UpdateClimateSensors(uint8_t sensor_id, uint8_t require_completion);;


    // char name_buffer[100]; //use single 1D klist array in memory


     int8_t Tasker(uint8_t function);

         #define DONE_COMPLETE false
         #define DONE_NOTCOMPLETE true
         #define REQUIRE_COMPLETE true
         #define DONTREQUIRE_COMPLETE false
    float tmp_float;

    struct SENSORDATA{
      DHTesp* dht;
      struct TYPE{
        float temperature;
        float humidity;
        float heatIndex;
        float dewPoint;
        float cr;       
        uint8_t isvalid=false;
        uint8_t ischanged=false;
        uint8_t ischanged_over_threshold=false; //phase out and just add changed percentage guards ie change more than 0.1 or its lowest precision
        // time later 
        uint32_t tWithinLimit;
        uint32_t tSavedMeasureClimate;
        uint32_t ischangedtLast = millis();
        uint8_t fWithinLimit;
        uint8_t sUpdateClimateSensors;
      };
      TYPE instant;
      TYPE roc10s; // rate of change
      TYPE roc1m; // rate of change
      TYPE roc10m; // rate of change
      // const char* name_ptr = nullptr;
    }sensor[MAX_SENSORS];

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
    
    struct handler<mSensorsDHT>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mSensorsDHT> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mSensorsDHT> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsDHT> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

  
};
#endif

#endif
