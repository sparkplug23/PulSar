#ifndef _MULTRASONICSENSOR_H
#define _MULTRASONICSENSOR_H 0.7

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_ULTRASONICS

#include "3_Network/MQTT/mMQTT.h"

#include <math.h>

#include <ArduinoJson.h>
#include "3_Network/WebServer/mWebServer.h"

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define USE_MQTT_ULTRASONIC

class mUltraSonicSensor{

  public:
    //#define D_MODULE_TOPIC_NAME "ultrasonic"

    mUltraSonicSensor(){};

    int8_t pin_trig = -1;
    int8_t pin_echo = -1;
    
    void Pin_Config();

    struct TRIGGER_EVENTS{
      uint16_t trigger_cm_max=400;
      uint16_t trigger_cm_min=40;
      uint8_t ispresent = false;
      uint32_t tSavedCheck = millis();
    }object_detected_static;   
    
    void MQQTSendObjectDetected(void);
    void MQQTDataBuilder_ObjectDetected();
    
    /**
     * Uses an array (wrapping) to store the the state of the sensor distance where 
     * true is within threshold and false is not.
     * */
    void AddPresenceEventStatusSum(uint8_t status);
    /**
     * Returns sum of the array.
     * */
    uint8_t GetPresenceEventStatusSum();
    /**
     * Variables relating to event status
     * */
    #define PRESENCE_EVENT_STATUS_SUM_MAX 3
    struct PRESENCE_EVENT_STATUS_SUM{
      uint8_t arr[PRESENCE_EVENT_STATUS_SUM_MAX];
      uint8_t idx = 0;
    }presence_event_status_sum;

    struct PRESENCE_DETECT{
      uint8_t isactive = false;
      uint8_t wasactive = false; // previous state
      uint8_t state = false;
      uint8_t ischanged = false;
      uint32_t tDetectTime;
      char detected_rtc_ctr[10];
    }presence_detect;

    uint8_t WITHINLIMITS(int minv, float var, int maxv);
    int8_t Tasker(uint8_t function);
    void init(void);
    uint32_t tUpdateOilReading;
    uint32_t tSaved;
    float GetSpeedOfSoundInMetres();
    float stored_speedofsound_inmetres=0;
    uint32_t tCheckTime = millis(), tReadStoredLitres=millis();

    int GetDurationReading();
    int GetDurationReading2();
    float GetDistanceMMReading();
    float GetDistanceCMReading();
    void SubTask_UltraSonicAverage();
    // float GetOilHeightMMReading();
    // float GetOilHeightCMReading();
    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();

    void MQQTSend_1m_Detailed(void);
    void MQQTSend_UltraSonicBasic(void);

    uint8_t fEnableSensor = false;

    #define ADCSENSORS_MAX 60

    #define ADCSENSORS_SMOOTHSLOW_NUMREADINGS ADCSENSORS_MAX // @1hz ie 1 minute
    #define ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS ADCSENSORS_MAX // @ EVERY 60 SECONDS hz ie 1 hour

    uint16_t pressure;
    // void SubTask_CalculateOilVolume();
    // void SubTask_CalculateOilVolumeAdjusted();
    void MQQTDataBuilder_TestingArray();
    void MQQTDataBuilder_UltraSonic2();
    float GetOilHeightMMReadingAdjustedFromTemp();
    float GetDistanceMMReadingAdjustedForTemp();
    float GetDistanceMMReadingAdjustedForTemp(int duration);
    float GetDistanceCMReadingAdjustedForTemp();

    void MQQTDataBuilder_UltraSonicBasic();
    void MQQTSendUltraSonicBasic();

    #define WEB_HANDLE_JSON_ULT_SENSOR_TABLE "/fetch/tab_ult_sensor.json"
    void WebPage_Root_AddHandlers();
    void WebSend_JSON_Table(AsyncWebServerRequest *request);
    void ConstructRoot_JSON_Table(JsonObject root);


    // void ConstructJSON_Detailed();
    // void ConstructJSON_Basic();

    uint8_t fUpdateCalculations = false;
    int8_t parse_JSONCommand();


    struct ULTRASONICSENSOR{
        uint32_t tReadLast;
        uint32_t tUltraSonicSensorReadLast;
        int duration;
        float temperature;
        uint32_t tPermitTempUpdate = 0;
        float speedofsound;
        uint8_t isvalid = false;
        uint8_t ischanged = false;
        struct ACCURACY{
          uint32_t insidecount;
          uint32_t outsidecount;
          float percent = 0;
        }accuracy;
        struct THRESHOLD{
          uint8_t setpercent = 0; //percent of original
          uint8_t narrowpercent = 0; // high precision PERCENT
          uint8_t widepercent = 0; // open to more wide values
          float lowervalue = 0; // open to more wide values
          float uppervalue = 0; // open to more wide values
          float ratio_pos = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
          float ratio_neg = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
          float relative = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
          uint8_t insidecount = 0;
          uint8_t outsidecount = 0;
        }threshold;
        struct SETTINGS{
          uint16_t measure_rate_ms = 2000;
          uint16_t blocking_time_ms = 500;
          uint16_t duration_limit_max = 40000;
          uint16_t duration_limit_min = 100;
        }settings;
    }ultrasonic;

    struct AVERAGED{
      struct SENSOR{
        uint8_t isvalid = false;  //only valid after its been ran once completely
        uint8_t ischanged = false;
        struct CAPTURED{
           float readings[ADCSENSORS_MAX];      // the readings from the analog input
           int readIndex = 0;              // the index of the current reading
           float total = 0;                  // the running total
           float average = 0;                // the average
           uint32_t tSaved;
           uint32_t tLastChanged;
       }captured;
       struct AVERAGING{
          float deviationarray[ADCSENSORS_MAX];
          float deviationsum = 0;
          float deviationaverage=0;
          int isoutlier[ADCSENSORS_MAX];
          int outliercount = 0;
          int usablecount = 0;
          int usableratio = 0;
          uint32_t tLastChanged;
       }averaging;
       struct FINAL{ // dont store cm, only mm
          float distance_cm;
          float distance_mm;
          uint32_t tLastChanged;
        }final;
      };
      struct SENSOR instant;
      struct SENSOR smooth_1m;
      struct SENSOR smooth_1hr;
      struct SENSOR smooth_1hr_from_1m;
      struct SENSOR* ptr;
    }averaged;


    void SubTask_RecordLitresOverDays();
    uint32_t tRecordLitresOverDays;

uint8_t ConstructJSON_Settings(uint8_t json_level);
uint8_t ConstructJSON_Sensors(uint8_t json_level);
uint8_t ConstructJSON_SensorsAveraged(uint8_t json_level);

  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mUltraSonicSensor>* mqtthandler_ptr = nullptr;
    const char* postfix_topic_settings = "settings\0";
    struct handler<mUltraSonicSensor> mqtthandler_settings_teleperiod;
    const char* postfix_topic_sensors = "sensors\0";
    struct handler<mUltraSonicSensor> mqtthandler_sensor_ifchanged;
    struct handler<mUltraSonicSensor> mqtthandler_sensor_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_AVERAGED_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_AVERAGED_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    const char* postfix_topic_averaged = "sensors/averaged\0";
    struct handler<mUltraSonicSensor> mqtthandler_averaged_ifchanged;
    struct handler<mUltraSonicSensor> mqtthandler_averaged_teleperiod;
    
  //#endif


};
#endif

#endif
