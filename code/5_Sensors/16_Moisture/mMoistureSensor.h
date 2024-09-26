#ifndef _MMOISTURESENSOR_H
#define _MMOISTURESENSOR_H 0.1

#define D_UNIQUE_MODULE_SENSORS_MOTION_ID 131

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_MOISTURE

#include "1_TaskerManager/mTaskerManager.h"


#include "ArduinoJson.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mMoistureSensor :
  public mTaskerInterface
{
  public:
    // Constructor
    mMoistureSensor(){};
    // Init the class
    void init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // All SubTasks called by Tasker 
    
    static constexpr const char* PM_MODULE_SENSORS_MOTION_CTR = D_MODULE_SENSORS_PIR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_MOTION_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_MOTION_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mMoistureSensor);
    };
    #endif



    #define MOISTURE_DETECT_INIT() pinMode(MOISTURE_DIGITAL_PIN,INPUT_PULLUP)
    #ifdef MOISTURE_ISACTIVELOW
      #define MOISTURE_DETECTED()  !digitalRead(MOISTURE_DIGITAL_PIN) //motion when LOW
    #else
      #define MOISTURE_DETECTED()  digitalRead(MOISTURE_DIGITAL_PIN)
    #endif
    #define MOISTURE_DETECTED_CTR  MOISTURE_DETECTED() ? "MOISTURE" : "NO_MOISTURE"
    //#define ONOFF_CTR(X)      X ? "ON" : "OFF"

    #define MAX_ADC_BOUNDARY 1024
    #define MIN_ADC_BOUNDARY 340


    struct MOISTURE_DETECT{
      uint8_t isactive = false;
      uint8_t state = false;
      uint8_t ischanged = false;
      uint32_t tDetectTime;
    }moisture_detect;

    uint32_t tSavedMeasure = 0;
    uint8_t rateMeasure = 10;

    #ifdef USE_MODULE_NETWORK_MQTT
      void MQTTSendMoistureSensorIfChanged();
      void SubTasker_MQTTSender();
      void AddToJsonObject_AddHardware(JsonObject root);
    #endif

};
#endif

#endif
