
#ifndef _HLK_LD2410_H_
#define _HLK_LD2410_H_

#define D_UNIQUE_MODULE_SENSORS__HLK_LD2410__ID   5028

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410

#include <TasmotaSerial.h>

#include "1_TaskerManager/mTaskerInterface.h"

#define LD2410_BUFFER_SIZE               TM_SERIAL_BUFFER_SIZE  // 64
#define LD2410_MAX_GATES                 9

#define LD2410_CMND_START_CONFIGURATION  0xFF
#define LD2410_CMND_END_CONFIGURATION    0xFE
#define LD2410_CMND_SET_DISTANCE         0x60
#define LD2410_CMND_READ_PARAMETERS      0x61
#define LD2410_CMND_START_ENGINEERING    0x62
#define LD2410_CMND_END_ENGINEERING      0x63
#define LD2410_CMND_SET_SENSITIVITY      0x64
#define LD2410_CMND_GET_FIRMWARE         0xA0
#define LD2410_CMND_SET_BAUDRATE         0xA1
#define LD2410_CMND_FACTORY_RESET        0xA2
#define LD2410_CMND_REBOOT               0xA3
#define LD2410_CMND_SET_BLUETOOTH        0xA4
#define LD2410_CMND_GET_BLUETOOTH_MAC    0xA5

class mHLK_LD2410 :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mHLK_LD2410(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};

    static constexpr const char* PM_MODULE_SENSORS__HLK_LD2410__CTR = D_MODULE_SENSORS__RADAR_HLK_LD2410__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__HLK_LD2410__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__HLK_LD2410__ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

     struct MODULE_RUNTIME{
      uint8_t *buffer;
      uint16_t moving_distance;
      uint16_t static_distance;
      uint16_t detect_distance;
      uint16_t no_one_duration;
      uint8_t moving_sensitivity[LD2410_MAX_GATES +1];
      uint8_t static_sensitivity[LD2410_MAX_GATES +1];
      uint8_t max_moving_distance_gate;
      uint8_t max_static_distance_gate;
      uint8_t moving_energy;
      uint8_t static_energy;
      uint8_t step;
      uint8_t retry;
      uint8_t settings;
      uint8_t byte_counter;
      bool valid_response;
      uint8_t set_engin_mode;
      uint8_t web_engin_mode;
      struct {
        uint8_t moving_gate_energy[LD2410_MAX_GATES +1];
        uint8_t static_gate_energy[LD2410_MAX_GATES +1];
        uint8_t light;
        uint8_t out_pin;
      } engineering;
    } rt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
            
    const uint8_t LD2410_config_header[4] = {0xFD, 0xFC, 0xFB, 0xFA};
    const uint8_t LD2410_config_footer[4] = {0x04, 0x03, 0x02, 0x01};
    const uint8_t LD2410_target_header[4] = {0xF4, 0xF3, 0xF2, 0xF1};
    const uint8_t LD2410_target_footer[4] = {0xF8, 0xF7, 0xF6, 0xF5};

    #ifndef LD2410_DEFAULT_SERIAL_NUMBER
    #define LD2410_DEFAULT_SERIAL_NUMBER 2
    #endif

    TasmotaSerial *LD2410Serial = nullptr;

    uint32_t ToBcd(uint32_t value);
    void Ld1410HandleTargetData(void);
    void Ld1410HandleConfigData(void);
    void PollSensor(void);  
    void SendCommand(uint32_t command, uint8_t *val = nullptr, uint32_t val_len = 0);  
    void SetConfigMode(void);
    void SetMaxDistancesAndNoneDuration(uint32_t max_moving_distance_range, uint32_t max_static_distance_range, uint32_t no_one_duration);
    void SetGateSensitivity(uint32_t gate, uint32_t moving_sensitivity, uint32_t static_sensitivity);
    void SetAllSensitivity(uint32_t sensitivity);
    void SetBaudrate(uint32_t index);
    void Every100MSecond(void);
    void Show_SensorReading(void);
    void Detect(void);
    bool CheckHeaderMatch(const uint8_t *header, uint32_t offset);

    /************************************************************************************************
     * SECTION: UNIFIED_SENSOR_REPORTING
     ************************************************************************************************/

    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
        if(index >= module_state.devices) {value->sensor_type.push_back(0); return ;}
        value->timestamp = millis();

        value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_MOVING_ID);
        value->data_f.push_back(rt.moving_distance);
        value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_STATIC_ID);
        value->data_f.push_back(rt.static_distance);
        value->sensor_type.push_back(SENSOR_TYPE_STRENGTH_MOVING_ID);
        value->data_f.push_back(rt.moving_energy);
        value->sensor_type.push_back(SENSOR_TYPE_STRENGTH_STATIC_ID);
        value->data_f.push_back(rt.static_energy);
        value->sensor_type.push_back(SENSOR_TYPE_DISTANCE_DETECT_ID);
        value->data_f.push_back(rt.detect_distance);

        value->sensor_id = index;
    };



    
    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);
    void CmndDuration(uint16_t val);
    void CmndMovingSensitivity(uint8_t* values, uint32_t length);
    void CmndStaticSensitivity(uint8_t* values, uint32_t length);
    void CmndEnableEngineeringMode(bool val);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
      void Telemetry_Init();
      std::vector<struct telemetry_handler<mHLK_LD2410>*> telemetry_list;    
      struct telemetry_handler<mHLK_LD2410> telemetry_settings;
      struct telemetry_handler<mHLK_LD2410> telemetry_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif 