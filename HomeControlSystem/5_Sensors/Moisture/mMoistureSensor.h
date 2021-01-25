#ifndef _MMOISTURESENSOR_H
#define _MMOISTURESENSOR_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_MOISTURE

#include "1_TaskerManager/mTaskerManager.h"


#include "ArduinoJson.h"

class mMoistureSensor{
  public:
    // Constructor
    mMoistureSensor(){};
    // Init the class
    void init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function);
    // All SubTasks called by Tasker 

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

    #ifdef USE_MQTT
      void MQTTSendMoistureSensorIfChanged();
      void SubTasker_MQTTSender();
      void AddToJsonObject_AddHardware(JsonObject root);
    #endif

};
#endif

#endif
