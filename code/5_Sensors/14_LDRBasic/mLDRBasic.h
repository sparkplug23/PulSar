#ifndef _mLDRBasic_H_
#define _mLDRBasic_H_

#define D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID 5014 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_LDR_BASIC

#define MAX_LDR_BASIC 2             // Max number of switches

#include "1_TaskerManager/mTaskerInterface.h"

class mLDRBasic :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mLDRBasic(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_LDR_BASIC_CTR = D_MODULE__SENSORS__LDR_BASIC__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_LDR_BASIC_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
         
    void EveryLoop();
    void EverySecond();

    struct SETTINGS{
      bool fEnableSensor = false;
    }settings;

    struct LDR_BASIC{

      uint8_t  digital_reading = 0;
      uint16_t analog_reading = 0;

    }ldr[MAX_LDR_BASIC];


    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/

    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > GetSensorCount()) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_LIGHT_LEVEL_ID);
      value->data_f.push_back(ldr[index].analog_reading);
      // value->sensor_type.push_back(SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID);
      // value->data_f.push_back(device_data[index].illuminance);
      value->sensor_id = index;
    };
        
    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    
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
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mLDRBasic>*> telemetry_list;
    struct telemetry_handler<mLDRBasic> telemetry_settings;
    struct telemetry_handler<mLDRBasic> telemetry_sensor_ifchanged;
    struct telemetry_handler<mLDRBasic> telemetry_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT




};

#endif

#endif
