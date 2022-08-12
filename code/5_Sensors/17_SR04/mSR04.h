#ifndef _MODULE_SENSORS_SR04_H
#define _MODULE_SENSORS_SR04_H

#define D_UNIQUE_MODULE_SENSORS_SR04_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_SR04_ID    1    // Numerical accesending order of module within a group

// #define USE_MODULE_EXAMPLE_CLASS

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SR04


#ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
#include "SingleEMAFilterLib.h"
#endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
#ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
#include "DoubleEMAFilterLib.h"
#endif //  ENABLE_DEVFEATURE_SR04_FILTERING_DEMA



#include <NewPing.h>
#include <TasmotaSerial.h>
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/


class mSR04 :
  public mTaskerInterface
{
  public:
	  mSR04(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_SR04_CTR;
    static const char* PM_MODULE_SENSORS_SR04_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SR04_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_SR04_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SR04_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSR04);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    enum EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__IDS{
      EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID,
      EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_DEFAULT__ID,
      EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_TEMPERATURE_COMPENSATED__ID,
      EM_DISTANCE_PING_CONVERSION_METHOD__LENGTH__ID
    };
    /**
     * @brief Get the Distance From Ping object
     * New function will convert using different methods from ping to distance in cm
     * DISTANCE_PING_CONVERSION_METHOD_BASIC_ID,
     * DISTANCE_PING_CONVERSION_METHOD_SPEED_OF_SOUND_DEFAULT_ID,
     * DISTANCE_PING_CONVERSION_METHOD_SPEED_OF_SOUND_MANUAL_ID,
     * 
     * @param ping_value 
     * @return float 
     */
    float GetDistanceFromPing(uint32_t ping_value);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    uint8_t sr04_type = 1;

    /**
     * @brief Change, I want to save the distance to the filters, but just have the GetDistance applied first (not after)
     **/
    struct READINGS {
      uint8_t processing_type = 1; // 0 = raw, 1 = EMA, 2 = DEMA, 3 = SMA
      struct RAW{
        uint32_t ping_value = 0;
        float    distance_cm = 0;
      }raw;
      #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
      struct SingleEMAFilter_VALUES{
        float distance_cm = 0;
        float alpha = 0;
        SingleEMAFilter<float>* filter = nullptr;
      }average_EMA;
      #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
      #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
      struct DoubleEMAFilter_VALUES{
        float distance_cm = 0;
        float alpha1 = 0;
        float alpha2 = 0;
        DoubleEMAFilter<float>* filter = nullptr;
      }average_DEMA;
      #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
      #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
      struct TEMPERATURE_ADJUSTMENT{
        bool flag_enabled = false;
        float ambient_temperature = 0;
      }temp_adj;
      struct CONVERSION_SETTINGS{
        float speed_of_sound = 0;
        uint8_t flag_distance_conversion_method = EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID;
      }conversion_settings;
      #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    }readings;

    // doubleEMAFilter(0.025, 0.1);
    // SingleEMAFilter<float>* filter = nullptr;// doubleEMAFilter(0.025, 0.1);
    
    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    void SubTask_UpdateAmbientTemperature();
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION

    // int getMeasure();

    void Config_Filters();

    void MeasureSensor();


    NewPing* sonar = nullptr;
    TasmotaSerial* sonar_serial = nullptr;
    
    uint8_t ModeDetect(void);
    uint16_t MiddleValue(uint16_t first, uint16_t second, uint16_t third);
    uint16_t Mode3Distance();
    uint16_t Mode2Distance(void);
    void EverySecond(void);
    void EveryMinute();

    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSR04> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mSR04> mqtthandler_sensor_ifchanged;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    
    struct handler<mSR04>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged
    };

    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
