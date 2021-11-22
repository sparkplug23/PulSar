#ifndef MSENSORSDB_H
#define MSENSORSDB_H 0.3

#define D_UNIQUE_MODULE_SENSORS_DB18_ID 127

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DS18X

#define REQUIRESALARMS false //turn off

#include <DallasTemperature.h>
#include <OneWire.h>

#include "1_TaskerManager/mTaskerManager.h"


#define TEMPERATURE_PRECISION 11 //highest

#define AUTO_FIND_DB_SENSOR

#include "1_TaskerManager/mTaskerInterface.h"

class mDS18X :
  public mTaskerInterface
{

  private:
  public:
    mDS18X(){};
    void Init(void);
    void Pre_Init();
    
    static const char* PM_MODULE_SENSORS_DB18_CTR;
    static const char* PM_MODULE_SENSORS_DB18_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DB18_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_DB18_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DB18_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDS18X);
    };
    #endif

    struct SETTINGS{
      uint8_t  fEnableSensor = true;
      uint8_t  nSensorsFound = 0; // count of sensors found    n:number found, c:case number for switches
      uint16_t rate_measure_ms = 1000;
      uint8_t  group_count = 0;
    }settings;

    void parse_JSONCommand(JsonParserObject obj);

    void SetIDWithAddress(uint8_t device_id, uint8_t* address_to_find);

    void ScanSensorAddresses_JsonObject(char* buffer, uint8_t buflen);

    void SplitTask_UpdateSensors(uint8_t sensor_id, uint8_t require_completion);
    uint8_t sReadSensor;
    uint32_t tSavedMeasureSensor;
    uint8_t fWithinLimit;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    void EveryLoop();


    float tmp_float;
    int ii;

    // Predefines by device
    #define SPLASH_ADDRESS_ON_INIT

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();
    #endif // USE_MODULE_NETWORK_WEBSERVER

    #ifndef DB18_SENSOR_MAX
      #define DB18_SENSOR_MAX 12
    #endif
    uint8_t db18_sensors_active = 0;// also used as "id"
    uint8_t anychanged=false; // doesnt work inside struct aray


    struct DB18_SENSOR_INFO{ //single sensor data
      DeviceAddress address = {0}; //phase out
      // uint8_t address_stored[8];
      int8_t sensor_group_id = -1; // which pin it comes from 

      /**
       * @brief method is not right, I need to align this with devicename ids, so sensor[id] where id matches devicename should be configured
       * */
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

    
    
    uint8_t GetSensorCount(void) override
    {
      return settings.nSensorsFound;
    }
    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      // Serial.println("OVERRIDE ACCESSED DB18");
      if(index > DB18_SENSOR_MAX-1) {value->type.push_back(0); return ;}
      value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data.push_back(sensor[index].reading.val);
      value->sensor_id = index;
    };




  
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    // void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    struct handler<mDS18X> mqtthandler_settings_teleperiod;
    struct handler<mDS18X> mqtthandler_sensor_ifchanged;
    struct handler<mDS18X> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
    struct handler<mDS18X>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };
    
  //#endif




};
#endif

#endif




// // Data wire is plugged into port 2 on the Arduino
// #define ONE_WIRE_BUS 22
//   sensor_group[sensor_group_count].pin = ONE_WIRE_BUS;//pCONT_pins->GetPin(GPIO_DSB_1OF2_ID);
//   sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);//sensor_group[sensor_group_count].pin);
//   sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
//         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin test 1 Valid %d"),sensor_group[sensor_group_count].pin);
//     sensor_group[sensor_group_count].dallas->begin();
  
// // // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
// // OneWire oneWire(ONE_WIRE_BUS);

// // // Pass our oneWire reference to Dallas Temperature. 
// // DallasTemperature sensors(&oneWire);
  

// // while(1){
  
//   // locate devices on the bus
//   Serial.print("Locating devices...");
//   Serial.print("Found ");
//   Serial.print(sensor_group[sensor_group_count].dallas->getDeviceCount(), DEC);
//   Serial.println(" devices.");
  
//   DeviceAddress insideThermometer;

//   if (!sensor_group[sensor_group_count].dallas->getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");

//   // show the addresses we found on the bus
//   Serial.print("Device 0 Address: ");
//   for (uint8_t i = 0; i < 8; i++)
//   {
//     // zero pad the address if necessary
//     if (insideThermometer[i] < 16) Serial.print("0");
//     Serial.print(insideThermometer[i], HEX);
//   }
//   Serial.println();

//   sensor_group[sensor_group_count].dallas->requestTemperatures(); // Send the command to get temperatures
//   float tempC = sensor_group[sensor_group_count].dallas->getTempCByIndex(0);
//   Serial.print("1Temperature for the device 1 (index 0) is: ");
//   Serial.println(tempC);

//   DEBUG_DELAY(2000);
// // };