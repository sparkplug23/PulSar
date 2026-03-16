#ifndef _MODULE_SENSORS_SR04_H
#define _MODULE_SENSORS_SR04_H

#define D_UNIQUE_MODULE_SENSORS_SR04_ID   5017 // [(Folder_Number*100)+ID_File]  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SR04

#ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
#include "SingleEMAFilterLib.h"
#endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
#ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
#include "DoubleEMAFilterLib.h"
#endif //  ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

#ifndef SONAR_SERIAL_BAUD
  #define SONAR_SERIAL_BAUD 9600
#endif // SONAR_SERIAL_BAUD

#include <NewPing.h>
#include <TasmotaSerial.h>

/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

const uint8_t MAX_SR04 = 1; // Max number of SR04 ultrasonic sensors MUST be one, unless GPIO are increased (D_GPIO_FUNCTION_SR04_ECHO2_CTR)

class mSR04 :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mSR04(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_SR04_CTR = D_MODULE_SENSORS_SR04_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SR04_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SR04_ID; }
   
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

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

    
    #ifndef SR04_MAX_SENSOR_DISTANCE
    #define SR04_MAX_SENSOR_DISTANCE  500
    #endif

    enum Sr04CommsMode { SR04_MODE_NONE,             // No hardware detected
                        SR04_MODE_TRIGGER_ECHO,     // Mode 1 - Trigger and Echo connection
                        SR04_MODE_SER_RECEIVER,     // Mode 2 - Serial receive only
                        SR04_MODE_SER_TRANSCEIVER,  // Mode 3 - Serial transmit and receive
                        SR04_NOT_DETECTED };        // Not yet detected

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
      // }temp_adj;
      // struct CONVERSION_SETTINGS{
      //   float speed_of_sound = 0;
      //   uint8_t flag_distance_conversion_method = EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID;
      // }conversion_settings;
      #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
      
      struct CONVERSION_SETTINGS{
        float speed_of_sound = 0;
        uint8_t flag_distance_conversion_method = EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID;
      }conversion_settings;
    }readings;
    
    struct {
      float distance;
      uint8_t valid;
      uint8_t type = SR04_NOT_DETECTED;
      NewPing* sonar = nullptr;
      TasmotaSerial* sonar_serial = nullptr;
    } SR04[MAX_SR04];

    uint8_t sr04_sensor_count = 0;
    uint8_t sr04_type = 1;

    NewPing* sonar = nullptr;
    TasmotaSerial* sonar_serial = nullptr;

    void Reading(uint32_t i);
    void Config_Filters();
    void MeasureSensor();
    
    void ModeDetect(void);
    uint16_t MiddleValue(uint16_t first, uint16_t second, uint16_t third);
    uint16_t Mode3Distance(uint32_t i);
    uint16_t Mode2Distance(uint32_t i);
    void EverySecond(void);
    void EveryMinute();
    
    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    void SubTask_UpdateAmbientTemperature();
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > MAX_SR04-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_CM_ID);
      #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
      value->data_f.push_back((float)readings.average_EMA.distance_cm);
      #else
      value->data_f.push_back((float)readings.raw.distance_cm);
      #endif
      value->sensor_id = index;
    };

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mSR04>*> mqtthandler_list;
    struct handler<mSR04> mqtthandler_settings;
    struct handler<mSR04> mqtthandler_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT
    


};

#endif

#endif
