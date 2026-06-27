#ifndef _MODULE_SENSORS_INTERFACE_H
#define _MODULE_SENSORS_INTERFACE_H

#define D_UNIQUE_MODULE_SENSORS_INTERFACE_ID   5000 // [(Folder_Number*100)+ID_File]  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_INTERFACE_ID    0    // Numerical accesending order of module within a group


#define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

#include "custom_types.h"

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_INTERFACE

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR)     "unified/heatmap";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED_FILTERED__CTR)        "unified/filtered";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR)                 "unified";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_SYSTEM_LOCATION__CTR)         "system_location";
  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_MOTION__CTR) "event_motion";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_USER_INPUT__CTR) "event_userinput";

#ifdef USE_MODULE_LIGHTS_INTERFACE
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#endif // USE_MODULE_LIGHTS_INTERFACE

#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
#include "AveragingDataLib.h"
#endif

#include <algorithm>  // for std::find
#include <vector>
#include <stdint.h>

class mSensorsInterface :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mSensorsInterface(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_INTERFACE_CTR = D_MODULE_SENSORS_INTERFACE_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_INTERFACE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_INTERFACE_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot      
      uint8_t tTicker_Splash_Sensors_To_Logs = 30;
      float sealevel_pressure; 
    }rt;

    bool button_swap = false;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    uint32_t GetColourValueUsingMaps_FullBrightness(float value, uint8_t map_style_id = 0, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    uint32_t GetColourValueUsingMaps_AdjustedBrightness(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    void HsbToRgb(float h, float s, float v, uint8_t* r8, uint8_t* g8, uint8_t* b8);
    #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

    void EveryLoop();

    #ifdef USE_MODULE_LIGHTS_INTERFACE
    uint8_t flag_unified_sensor_colour_heatmap_type = 0;
    RgbwwColor GetColourValueUsingMaps_ForUnifiedSensor(float temperature);
    #endif // USE_MODULE_LIGHTS_INTERFACE
        

    void Broadcast_Event_MotionDetected();
    void Broadcast_Event_UserInput();


    float ConvertTemp(float c);
    char TempUnit(void);
    float ConvertPressure(float p);
    String PressureUnit(void);

    sensorset_location_t system_location; // actually, although sensor related, to compile outside of anything, should be system
    // but, sun position is here, so perhaps needs to anyway.


    void WebAppend__Sensor_Table__As_TypesRows();
    void WebAppend__Sensor_Table__As_SensorsRows_Inverted();
    void WebAppend__Sensor_Table__As_Ragged();


    #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
    struct filtered_sensor_entry_t 
    {
      uint16_t module_id;           ///< Module unique ID
      uint8_t sensor_index;         ///< Sensor index within the module
      uint16_t window_secs;         ///< Averaging window in seconds
      uint16_t sample_count;        ///< Number of samples to collect

      uint32_t tLastUpdate = 0;     ///< Last millis() timestamp we sampled
      uint32_t sample_interval_ms;  ///< Milliseconds between samples
      uint16_t desired_type_id;  ///< The sensor type we're tracking (e.g. temperature, humidity, etc.)
      String filter_name;  // NEW FIELD

      Averaging_Data<float> filter_buffer; ///< Rolling buffer for smoothing

      void Init(uint16_t module, uint8_t index, uint16_t secs, uint16_t samples, uint16_t sensor_type, const String& name)
      {
        module_id = module;
        sensor_index = index;
        window_secs = secs;
        sample_count = samples;
        desired_type_id = sensor_type;
        sample_interval_ms = (secs * 1000UL) / samples;
        filter_buffer = Averaging_Data<float>(samples);
        filter_name = name;
        tLastUpdate = 0;
      }

    };


    std::vector<filtered_sensor_entry_t> filtered_sensors;
    
    // Add this to populate from a command or at init
    void AddFilteredSensor(uint16_t module_id, uint8_t index, uint16_t secs, uint16_t samples, uint16_t sensor_type, const String& name)
    {
      filtered_sensor_entry_t entry;
      entry.Init(module_id, index, secs, samples, sensor_type, name);
      filtered_sensors.push_back(entry);
    }


    void Update_UnifiedFilteredReadings();


    // Example call in your init or setup:
    // AddFilteredSensor(5027, 0, 60, 120);

    #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

    
    #ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE

    // Default data file (single file, append-only)
    #ifndef CALIB_CAPTURE_FILENAME
      #define CALIB_CAPTURE_FILENAME  "/sensor_calib_captures.json"
    #endif

    struct calib_point_t {
      String   name;        // unified device name (e.g. "DB_04")
      uint16_t type_id;     // SENSOR_TYPE_*
      float    value;       // reading
      uint32_t utc;         // unix time (seconds)
    };

    struct calib_capture_cfg_t {
      bool     enabled = false;
      uint32_t min_sample_period_ms = 1000;   // sensor sample throttle (per loop collector)
      uint32_t dump_period_ms        = 60000; // flush-to-file cadence
      uint32_t t_last_sample_ms      = 0;
      uint32_t t_last_dump_ms        = 0;

      // Optional filter (capture only this device name)
      char* capture_name = nullptr;     // malloc'ed (or set via template)
      uint8_t capture_name_len = 0;
      bool use_name_list = false;
      std::vector<String> capture_names; // exact-include list

      // Optional “truth” channel injection later (leave placeholder)
      // char* truth_key = nullptr; float truth_value = NAN;
    };

    calib_capture_cfg_t calib_cfg;
    std::vector<calib_point_t> calib_buffer;

    // API
    void Calib_Init();
    void Calib_EveryLoop();
    void Calib_OnSecond();
    void Calib_AppendCurrentReadings();   // collect into RAM buffer
    void Calib_FlushToFile();             // append NDJSON lines
    void Calib_ClearBuffer();
    void Calib_SetCaptureName(const char* s); // alloc + set

    #endif // ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);


    /************************************************************************************************
     * SECTION: Unified Reporting : System level not reported via submodules
     ************************************************************************************************/
    
    uint8_t GetSensorCount(void) override
    {
      return 1;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index >= GetSensorCount()) {value->sensor_type.push_back(0); Serial.println("returning invalid"); return ;}
      if(system_location.isvalid)
      {
        value->sensor_type.push_back(SENSOR_TYPE_LATITUDE_ID);                 value->data_f.push_back(system_location.latitude);
        value->sensor_type.push_back(SENSOR_TYPE_LONGITUDE_ID);                value->data_f.push_back(system_location.longitude);
        value->sensor_type.push_back(SENSOR_TYPE_SPEED_ID);                    value->data_f.push_back(system_location.speed);
        value->sensor_type.push_back(SENSOR_TYPE_ALTITUDE_ID);                 value->data_f.push_back(system_location.altitude);
        value->sensor_type.push_back(SENSOR_TYPE_LOCATION_ACCURACY_ID);        value->data_f.push_back(system_location.accuracy_position);
        value->sensor_type.push_back(SENSOR_TYPE_LOCATION_SATELLITES_USED_ID); value->data_f.push_back(system_location.satellites_used);
        value->sensor_id = index;
        return;
      }
      value->sensor_type.push_back(0); value->sensor_id = 0; return; // Return nothing if we reach here
    };
   
    
    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    /****
     * Current plan Feb25
     * 
     * Interface has common topics
     * 
     * Sensors : Updated by themselves, can be parsed by openhab rule
     * Event_Motion : PIR, Camera, Ultrasound, Radar, Door
     * Event_UserInput : Switch, Button
     * 
     * 
     * 
     */
   
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_SensorTemperatureColours(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Event_Motion(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Event_UserInput(uint8_t json_level = 0, bool json_appending = true);
    #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
    uint8_t ConstructJSON_Unified_Filtered(uint8_t json_level = 0, bool json_appending = true);
    #endif
    uint8_t ConstructJSON_System_Location(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mSensorsInterface>*> mqtthandler_list;
    struct handler<mSensorsInterface> mqtthandler_settings;
    struct handler<mSensorsInterface> mqtthandler_sensor_ifchanged; // polling non-user interactive sensing
    struct handler<mSensorsInterface> mqtthandler_sensor_teleperiod;
    struct handler<mSensorsInterface> mqtthandler_sensor_temperature_colours;
    #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
    struct handler<mSensorsInterface> mqtthandler_sensor_unified_filtered;
    #endif
    struct handler<mSensorsInterface> mqtthandler_motion_event_ifchanged;
    struct handler<mSensorsInterface> mqtthandler_event_input; // events triggered by user input
    struct handler<mSensorsInterface> mqtthandler_system_location; 
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
