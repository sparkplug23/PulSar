
#ifndef _TOF_VL53L0X_H_
#define _TOF_VL53L0X_H_

#define D_UNIQUE_MODULE_SENSORS__TOF_VL53L0X__ID ((5*1000)+26)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__TOF_VL53L0X



// #include "2_CoreSystem/02_Time/mTime.h"
// #include "2_CoreSystem/05_Logging/mLogging.h"

#include "1_TaskerManager/mTaskerManager.h"


// //#include "//2_CoreSystem/11_Languages/mLanguage.h"
// #include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

// #ifdef ESP32
//   #include <WiFi.h>
// #endif
// #ifdef ESP8266
//   #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
//   #include <ESP8266httpUpdate.h>
// #endif

// #include "2_CoreSystem/02_Time/mTime.h"



#define XSNS_45            45
#define XI2C_31            31  // See I2CDEVICES.md

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define VL53L0X_LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define VL53L0X_HIGH_SPEED
//#define VL53L0X_HIGH_ACCURACY

// #define USE_VL_MEDIAN
// #define USE_VL_MEDIAN_SIZE 5   // Odd number of samples median detection

#include <Wire.h>
#include "VL53L0X.h"

#define VL53L0X_ADDRESS 0x29
#ifndef VL53L0X_XSHUT_ADDRESS
#define VL53L0X_XSHUT_ADDRESS 0x78
#endif


// // BUTTON_HOLD_PRESSED_ID

// DEFINE_PGM_CTR(PM_WEB_HANDLE_DIV_NAME_BUTTON_TABLE_CTR) "button_table";

// /*********************************************************************************************\
//  * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
// \*********************************************************************************************/

// #include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mTOF_VL53L0X :
  public mTaskerInterface
{
  public:
    mTOF_VL53L0X(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};

    static constexpr const char* PM_MODULE_SENSORS__TOF_VL53L0X__CTR = D_MODULE_SENSORS__TOF_VL53L0X__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__TOF_VL53L0X__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__TOF_VL53L0X__ID; }
    
    struct ClassState
    {
        uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
        uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;




// // /*********************************************************************************************\
// //  * Button support
// // \*********************************************************************************************/

// enum ButtonStates 
// { 
//   BUTTON_PRESSED_ID, 
//   BUTTON_NOT_PRESSED_ID 
// };

uint8_t MAX_SENSORS = 2;

// #define VL53LXX_MAX_SENSORS 8     // Max number of VL53L0X sensors


VL53L0X VL53L0X_device[VL53LXX_MAX_SENSORS];
struct {
  uint16_t distance;
  uint16_t distance_prev;
  uint16_t buffer[5];
  bool ready = false;
  uint8_t index;
  bool valid = false;
  uint8_t address = 0;
} Vl53l0x_data[VL53LXX_MAX_SENSORS];

bool VL53L0X_xshut = false;
bool VL53L0X_detected = false;
bool VL53L0X_xshut_enabled = false;

#ifdef USE_DEEPSLEEP
bool VL53L0X_standby = false;  // Prevent updating measurments once VL53L0X has been put to standby (just before ESP enters deepsleep)
#endif

void ReadSensor();

uint8_t SearchForDevices();


#define XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME 10 // per datasheet it is 1.2ms



#ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  uint8_t GetSensorCount(void) override
  {
    uint8_t count = 0;
    // for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    //     if (bitRead(VL53L0X_detected, i)) {
    //         count++;
    //     }
    // }
    count = module_state.devices;
    return count;
  }
  void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
  {
    if(index > MAX_SENSORS-1) {value->sensor_type.push_back(0); return ;}
    value->timestamp = millis(); // Switches are constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
    value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_ID);
    value->data_f.push_back(Vl53l0x_data[index].distance);
    value->sensor_id = index;
  };
  #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


  uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
  uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mTOF_VL53L0X>*> mqtthandler_list;    
    struct handler<mTOF_VL53L0X> mqtthandler_settings;
    struct handler<mTOF_VL53L0X> mqtthandler_sensor_ifchanged;
  #endif // USE_MODULE_NETWORK_MQTT


};



#endif

#endif 