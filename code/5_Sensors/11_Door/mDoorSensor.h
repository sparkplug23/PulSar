#ifndef _MDOORSENSOR_H
#define _MDOORSENSOR_H 0.2

#define D_UNIQUE_MODULE_SENSORS_DOOR_ID 5011 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DOOR

DEFINE_PGM_CTR(PM_EVENT_DOOR_OPENED_CTR) D_OPENED;
DEFINE_PGM_CTR(PM_EVENT_DOOR_CLOSED_CTR) D_CLOSED;

#include "1_TaskerManager/mTaskerInterface.h"

class mDoorSensor :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mDoorSensor(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_DOOR_CTR = D_MODULE_SENSORS_DOOR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DOOR_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DOOR_ID; }
   
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


    uint8_t     IsDoorOpen();
    const char* IsDoorOpen_Ctr(char* buffer, uint8_t buflen);
    
    struct DOOR_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t wasactive = false; //last state
      uint8_t ischanged = false;
      uint32_t tSaved;
      uint32_t tDetectTimeforDebounce;
      uint32_t detected_time = 0;   // LocalTime() style timestamp
      uint32_t tDetectTime = 0;
      uint32_t tEndedTime = 0;
    }door_detect;

    void EveryLoop();

    struct LOCK_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t ischanged = false;
      uint32_t tSaved = 0;
      uint32_t tDetectTimeforDebounce = 0;
      uint32_t detected_time = 0;   // LocalTime() style timestamp
    }lock_detect;
    void MQTTSendDoorLockIfChanged();
    uint8_t IsLock_Locked();
     
    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      value->sensor_id = index;
      value->sensor_type.push_back(SENSOR_TYPE_DOOR_POSITION_ID);
      value->data_f.push_back(door_detect.isactive);
      value->sensor_type.push_back(SENSOR_TYPE_DOOR_LOCKED_ID);
      value->data_f.push_back(lock_detect.isactive);
      value->sensor_type.push_back(0);
    }
      
    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj){};

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
    std::vector<struct handler<mDoorSensor>*> mqtthandler_list;
    struct handler<mDoorSensor> mqtthandler_settings;
    struct handler<mDoorSensor> mqtthandler_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
