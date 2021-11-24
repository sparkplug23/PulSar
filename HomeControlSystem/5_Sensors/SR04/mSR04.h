#ifndef _MODULE_SENSORS_SR04_H
#define _MODULE_SENSORS_SR04_H

#define D_UNIQUE_MODULE_SENSORS_SR04_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_SR04_ID    1    // Numerical accesending order of module within a group

// #define USE_MODULE_EXAMPLE_CLASS

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SR04

#include <NewPing.h>
#include <TasmotaSerial.h>
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/


class mSR04 :
  public mTaskerInterface
{
  public:
	  mSR04(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_SR04_CTR;
    static const char* PM_MODULE_SENSORS_SR04_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SR04_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_SR04_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SR04_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSR04);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    uint8_t sr04_type = 1;
    real64_t distance;

    NewPing* sonar = nullptr;
    TasmotaSerial* sonar_serial = nullptr;
    
    uint8_t Sr04TModeDetect(void);
    uint16_t Sr04TMiddleValue(uint16_t first, uint16_t second, uint16_t third);
    uint16_t Sr04TMode3Distance();
    uint16_t Sr04TMode2Distance(void);
    void Sr04TReading(void);

    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSR04> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mSR04> mqtthandler_sensor_ifchanged;
    struct handler<mSR04> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
