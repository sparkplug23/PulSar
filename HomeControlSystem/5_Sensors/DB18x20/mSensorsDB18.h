#ifndef MSENSORSDB_H
#define MSENSORSDB_H 0.3

#define D_UNIQUE_MODULE_SENSORS_DB18_ID 127

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DS18B20

#define REQUIRESALARMS false //turn off

#include "DallasTemperature.h"
#include "OneWire.h"

#include "1_TaskerManager/mTaskerManager.h"


#define TEMPERATURE_PRECISION 11 //highest

#define AUTO_FIND_DB_SENSOR

#include "1_TaskerManager/mTaskerInterface.h"

class mSensorsDB18 :
  public mTaskerInterface
{

  private:
  public:
    mSensorsDB18(){};
    void Init(void);
    void Pre_Init();
    
    static const char* PM_MODULE_SENSORS_DB18_CTR;
    static const char* PM_MODULE_SENSORS_DB18_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DB18_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_DB18_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DB18_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSensorsDB18);
    };
    #endif

    struct SETTINGS{
      uint8_t  fEnableSensor = true;
      uint8_t  nSensorsFound = 0; // count of sensors found    n:number found, c:case number for switches
      uint16_t rate_measure_ms = 1000;
      uint8_t  group_count = 0;
    }settings;

    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand();

    void SetIDWithAddress(uint8_t device_id, uint8_t* address_to_find);


    void SplitTask_UpdateSensors(uint8_t sensor_id, uint8_t require_completion);
    uint8_t sReadSensor;
    uint32_t tSavedMeasureSensor;
    uint8_t fWithinLimit;

    int8_t Tasker(uint8_t function);

    void EveryLoop();


    float tmp_float;
    int ii;

    // Predefines by device
    #define SPLASH_ADDRESS_ON_INIT

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();
    #endif // USE_MODULE_NETWORK_WEBSERVER


    // #ifdef DEVICE_MYBEDROOM
    //   #define SENSOR1_NAME "inside"
    //   DeviceAddress ADDRESS_S1 = {0x28,0xFF,0x64,0x99,0x1D,0xC2,0x7C,0x2B};
    //   #define SENSOR2_NAME "outside"
    //   DeviceAddress ADDRESS_S2 = {0x28,0xAA,0xDD,0x65,0x1D,0x13,0x02,0x71};
    //   #define DB18_SENSOR_MAX 2
    // #endif


    // #ifdef DEVICE_RADIATORFAN
    //   #define SENSOR1_NAME "inside"
    //   DeviceAddress ADDRESS_S1 = {0x28,0xAA,0xE7,0x99,0x1D,0x13,0x02,0x2B};
    //   #define SENSOR2_NAME "outside"
    //   DeviceAddress ADDRESS_S2 = {0x28,0xAA,0xDD,0x65,0x1D,0x13,0x02,0x71};
    //   #define DB18_SENSOR_MAX 2
    // #endif
    // #ifdef DEVICE_OILFURNACE
    //   // #define SENSOR1_NAME "inflow"
    //   // DeviceAddress ADDRESS_S1 = {0x28,0xAA,0x43,0x03,0x1E,0x13,0x02,0x19};
    //   // #define SENSOR2_NAME "outflow"
    //   // DeviceAddress ADDRESS_S2 = {0x28,0xFF,0xA9,0x78,0x35,0x16,0x04,0xF0};
    //   // #define SENSOR3_NAME "outside"
    //   // DeviceAddress ADDRESS_S3 = {0x28,0x8F,0x51,0x07,0x33,0x14,0x01,0xBD};
    //   // #define SENSOR4_NAME "tank1"      // Dual sensors as backup
    //   // DeviceAddress ADDRESS_S4 = {0x28,0xFF,0x64,0x1D,0xCD,0xC9,0xA8,0xCB};
    //   // #define SENSOR5_NAME "tank2"      // Dual sensors as backup
    //   // DeviceAddress ADDRESS_S5 = {0x28,0xFF,0x64,0x1D,0xCD,0xF8,0xF8,0xF9};
    //   // #define DB18_SENSOR_MAX 5
    // #endif
    // #ifdef DEVICE_COFFEE
    //   #define SENSOR1_NAME "heater"
    //   DeviceAddress ADDRESS_S1 = {0x28,0xFF,0xD0,0xFF,0xC1,0x17,0x04,0xE6};
    //   #define DB18_SENSOR_MAX 1
    // #endif
    // https://www.rapidtables.com/convert/number/ascii-hex-bin-dec-converter.html
    
    
    // Device Address: 28FF641DCDC9A8CB Temp C: 25.94 Temp F: 78.69
    // Requesting temperatures...DONE
    // Device Address: 28FF641DCDF8F8F9 Temp C: 25.62 Temp F: 78.12
    // Device Address: 28FF641DCDC9A8CB Temp C: 25.94 Temp F: 78.69


    #ifndef DB18_SENSOR_MAX
      #define DB18_SENSOR_MAX 12
    #endif
    uint8_t db18_sensors_active = 0;// also used as "id"
    uint8_t anychanged=false; // doesnt work inside struct aray


    struct DB18_SENSOR_INFO{ //single sensor data
      DeviceAddress address = {0}; //phase out
      // uint8_t address_stored[8];
      int8_t sensor_group_id = -1; // which pin it comes from 
      int8_t address_id = -1; //set this manually with template, else, as 0 (check for any -1, set incremented and name "sens01")
      struct READING{
        float val;
        uint8_t isvalid;
        uint8_t ischanged;
        uint32_t captureupsecs; //sensor age
      }reading;
    }sensor[DB18_SENSOR_MAX];


    struct SENSOR_GROUP_INFO{ // from a single pin
      uint8_t fSensorFound = false;
      uint8_t sensor_count = 0;
      int8_t  pin = -1;
      OneWire* onewire = nullptr;
      DallasTemperature* dallas = nullptr;
    }sensor_group[2];

    // these dont make sense
    #define DONE_COMPLETE false
    #define DONE_NOTCOMPLETE true
    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    
int8_t FindStructIndexByAddressID(int8_t address_id);

    // uint8_t GetCorrectedDeviceIDforGetDeviceName(uint8_t id_desired);
    
    // uint8_t GetCorrectedDeviceID(uint8_t id_desired);

    void ThermoPrintAddresses(void);
    void init_sensor(DallasTemperature* sensors);
    void printAddress(DeviceAddress deviceAddress, int8_t index = -1);
    void printTemperature(DallasTemperature* sensors, DeviceAddress deviceAddress);
    void printResolution(DallasTemperature* sensors, DeviceAddress deviceAddress);
    void printData(DeviceAddress deviceAddress);
    void PrintPipSensors(HardwareSerial* hs);
  
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mSensorsDB18> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mSensorsDB18> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsDB18> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif




};
#endif

#endif
