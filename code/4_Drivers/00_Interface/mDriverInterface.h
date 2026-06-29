#ifndef _MODULE_DRIVERS_INTERFACE_H
#define _MODULE_DRIVERS_INTERFACE_H

#define D_UNIQUE_MODULE_DRIVERS_INTERFACE_ID   4000 // [(Folder_Number*100)+ID_File]  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_DRIVERS_INTERFACE_ID    0    // Numerical accesending order of module within a group

#include "stdint.h"

#include "stdint.h"

// what the driver does, not how it does it (ie measures light, not what measured it)
enum drivers_type_t{
    DRIVER_TYPE_NONE_ID=0,
    DRIVER_TYPE_HVAC_ID,
    DRIVER_TYPE_COOLING_ID,

    DRIVER_TYPE_LENGTH_ID,
};


#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_INTERFACE

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DRIVERS_UNIFIED__CTR) "drivers/unified";

class mDriverInterface :
  public mTaskerInterface
{
  public:
	  mDriverInterface(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_DRIVERS_INTERFACE_CTR = D_MODULE_DRIVERS_INTERFACE_CTR;;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_INTERFACE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_INTERFACE_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // move this into settings so all devices can share it
      uint8_t tTicker_Splash_Sensors_To_Logs = 30;
      float sealevel_pressure; 
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();

    void MQTT_Report_Event_Button();

    // sensor_t

    
    void Broadcast_Event_MotionDetected(uint8_t event_type);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJson_DriverStatus(uint8_t json_level = 0, bool json_appending = true);
    // uint8_t ConstructJSON_SensorTemperatureColours(uint8_t json_level = 0, bool json_appending = true);
    // uint8_t ConstructJSON_Motion_Event(uint8_t json_level = 0, bool json_appending = true);

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;
  
  #ifdef ESP32
  WebCamCfg     webcam_config;             // 44C
  uint8_t       webcam_clk;                // 72F
  WebCamCfg2    webcam_config2;            // 460
  #endif

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mDriverInterface> mqtthandler_settings;
    // void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mDriverInterface> mqtthandler_driver_status_teleperiod;
    // void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    struct handler<mDriverInterface>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_driver_status_teleperiod
    };
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
