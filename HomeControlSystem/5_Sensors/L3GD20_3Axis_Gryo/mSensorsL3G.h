/*
  mHVAC.cpp - mSensorsL3G

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MSENSORSL3G_H
#define MSENSORSL3G_H

#define D_UNIQUE_MODULE_SENSORS_L3G_ID 143

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_L3G

// #include <Wire.h>
// #include <SPI.h>
// #include "5_Sensors/L3G/internal/Adafruit_L3G280.h"

#include "5_Sensors/L3GD20_3Axis_Gryo/internal/L3G.h"


// class Adafruit_L3G280;

// #include "1_TaskerManager/mTaskerManager.h"

// #include "1_TaskerManager/mTaskerInterface.h"

class mSensorsL3G :
  public mTaskerInterface
{
  
  public:
    mSensorsL3G(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_SENSORS_L3G_CTR;
    static const char* PM_MODULE_SENSORS_L3G_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_L3G_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_L3G_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_L3G_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSensorsL3G);
    };
    #endif

    uint32_t tSavedMeasure = millis();

L3G* gyro = nullptr;

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
    void AddToHardwareMessage();

    uint32_t tSavedMeasureClimate;

    void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);
    
    // uint8_t fWithinLimit;
    // unsigned long tWithinLimit;

    // #define REQUIRE_COMPLETE true
    // #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 1
    void EveryLoop();

    // void WebAppend_Root_Status_Table_Draw();
    // void WebAppend_Root_Status_Table_Data();
    // struct SENSORDATA{
    //   float temperature;
    //   float humidity;
    //   float pressure;
    //   float altitude;
    //   uint8_t isvalid=false;
    //   uint8_t ischanged=false;
    //   uint8_t ischanged_over_threshold=false;
    //   uint32_t ischangedtLast = millis();
    //   float heatIndex;
    //   float dewPoint;
    //   float cr;
    //   uint32_t tSavedMeasureClimate;
    //   uint8_t fWithinLimit;
    //   unsigned long tWithinLimit;
    //   uint8_t sReadSensor;
    //   Adafruit_L3G280* L3G = NULL;
    // }sensor[MAX_SENSORS];
    struct SENSORDATA
    {
        
      struct ACC_READINGS{
        struct INSTANT{
          float x;
          float y;
          float z;
        }instant;
      }gyro;
    }sensor;

    
    // uint8_t GetSensorCount(void) override
    // {
    //   return settings.fSensorCount;
    // }
    
    // void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    // {
    //   // Serial.printf("OVERRIDE ACCESSED DHT %d\n\r",index);Serial.println(sensor[index].instant.temperature);
    //   if(index > MAX_SENSORS-1) {value->type.push_back(0); return ;}
    //   value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
    //   value->type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
    //   value->data.push_back(sensor[index].temperature);
    //   value->data.push_back(sensor[index].humidity);
    //   value->sensor_id = index;
    // };



        
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSensorsL3G> mqtthandler_settings_teleperiod;
    struct handler<mSensorsL3G> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsL3G> mqtthandler_sensor_teleperiod;
 
    struct handler<mSensorsL3G>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
    #endif // USE_MODULE_NETWORK_MQTT

};
#endif

#endif
