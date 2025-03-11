
#ifndef _TOF_VL53L1X_H_
#define _TOF_VL53L1X_H_

#define D_UNIQUE_MODULE_SENSORS__TOF_VL53L1X__ID ((5*1000)+27)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__TOF_VL53L1X

/*********************************************************************************************\
 * VL53L1X
 *
 * Source:
 *
 * I2C Address: 0x29
 *********************************************************************************************
 *
 * Note: When using multiple VL53L0X, it is required to also wire the XSHUT pin of all those sensors
 * in order to let Tasmota change by software the I2C address of those and give them an unique address
 * for operation. The sensor don't save its address, so this procedure of changing its address is needed
 * to be performed every restart. The Addresses used for this are 120 (0x78) to 127 (0x7F). In the I2c
 * Standard (https://i2cdevices.org/addresses) those addresses are used by the PCA9685.
 * The base address (0x78) can be changed as a compile option with #define VL53L1X_XSHUT_ADDRESS 0xNN in
 * your user_config_override.h
 *
 * The default value of VL53LXX_MAX_SENSORS is set in the file tasmota.h
 * Changing that is backwards incompatible - Max supported devices by this driver are 8
 *********************************************************************************************
 * The following settings can be overriden
 *
 *
\*********************************************************************************************/

#define XSNS_77     77
#define XI2C_54     54  // See I2CDEVICES.md

#include "VL53L1X.h"

#define VL53L1X_ADDRESS 0x29
#ifndef VL53L1X_XSHUT_ADDRESS
#define VL53L1X_XSHUT_ADDRESS 0x78
#endif

#ifndef VL53L1X_DISTANCE_MODE
#define VL53L1X_DISTANCE_MODE Long
#endif


#include "1_TaskerManager/mTaskerInterface.h"

class mTOF_VL53L1X :
  public mTaskerInterface
{
  public:
    mTOF_VL53L1X(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};

    static constexpr const char* PM_MODULE_SENSORS__TOF_VL53L1X__CTR = D_MODULE_SENSORS__TOF_VL53L1X__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__TOF_VL53L1X__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__TOF_VL53L1X__ID; }
    
    struct ClassState
    {
        uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
        uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    uint8_t MAX_SENSORS = 2;

    #define XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME 10 // per datasheet it is 1.2ms

    VL53L1X vl53l1x_device[VL53LXX_MAX_SENSORS];

    struct {
        uint16_t distance = 0;
        bool valid = false;
        uint8_t address = 0;
        uint8_t mode = 0; // 0=single-shot, 1=continuous
    } vl53l1x_data[VL53LXX_MAX_SENSORS];

    uint32_t tSaved_ReadSensor = 0;

    uint8_t VL53L1X_xshut_bitmapped = 0;
    uint8_t VL53L1X_detected_bitmapped = 0;
    bool VL53L1X_xshut_enabled = false;
    void ReadSensor();

    uint8_t SearchForDevices();
    
    bool SwitchDeviceAddress(uint8_t device_id, uint8_t new_address) ;

    void Loop();

    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
        uint8_t count = 0;
        for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
            if (bitRead(VL53L1X_detected_bitmapped, i)) {
                count++;
            }
        }
        return count;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
        if(index > MAX_SENSORS-1) {value->sensor_type.push_back(0); return ;}
        value->timestamp = millis(); // Switches are constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
        value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_ID);
        value->data_f.push_back(vl53l1x_data[index].distance);
        value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    #ifdef USE_MODULE_NETWORK_MQTT
        void MQTTHandler_Init();
        std::vector<struct handler<mTOF_VL53L1X>*> mqtthandler_list;    
        struct handler<mTOF_VL53L1X> mqtthandler_settings;
        struct handler<mTOF_VL53L1X> mqtthandler_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT


};



#endif

#endif 