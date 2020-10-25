#ifndef MSENSORSBME_H
#define MSENSORSBME_H 0.2

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_BME

#include <Wire.h>
#include <SPI.h>
#include "5_Sensors/BME/mAdafruit_Sensor.h"
#include "5_Sensors/BME/Adafruit_BME280.h"

class Adafruit_BME280;

#include "1_TaskerManager/mTaskerManager.h"


#include <ArduinoJson.h>

class mSensorsBME{
  private:
    //#define D_MODULE_TOPIC_NAME "bme"
  public:
    mSensorsBME(){};
    void Pre_Init(void);
    void init(void);
    int8_t Tasker(uint8_t function);

    float sealevel_pressure; 

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;// MODULE_STATUS_NOTSET;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
    void AddToHardwareMessage();

    uint32_t tSavedMeasureClimate;

    void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);
    // uint8_t sReadSensor;

    uint8_t fWithinLimit;
    unsigned long tWithinLimit;

    TwoWire* Wire = nullptr;

    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 1
    void EveryLoop();

    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();
    struct SENSORDATA{
      float temperature;
      float humidity;
      float pressure;
      float altitude;
      uint8_t isvalid=false;
      uint8_t ischanged=false;
      uint8_t ischanged_over_threshold=false;
      uint32_t ischangedtLast = millis();
      float heatIndex;
      float dewPoint;
      float cr;
      uint32_t tSavedMeasureClimate;
      uint8_t fWithinLimit;
      unsigned long tWithinLimit;
      uint8_t sReadSensor;
      Adafruit_BME280* bme = NULL;
    }sensor[MAX_SENSORS];

    void AddToJsonObject_AddHardware(JsonObject root);
        
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSensorsBME>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mSensorsBME> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mSensorsBME> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsBME> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};
#endif

#endif
