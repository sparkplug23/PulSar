/*
  mHVAC.cpp - mBME

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
#ifndef MSENSORSBME_H
#define MSENSORSBME_H

#define D_UNIQUE_MODULE_SENSORS_BME_ID 143

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BME

#include <Wire.h>
// #include "5_Sensors/BME/internal/Adafruit_BME280.h"

/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60
#define BME680_CHIPID        0x61 // gas

#include "Adafruit_BME280.h"

#define BMP_MAX_SENSORS      2

// const char kBmpTypes[] PROGMEM = "BMP180|BMP280|BME280|BME680";

class Adafruit_BME280;

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"

class mBME :
  public mTaskerInterface
{
  
  public:
    mBME(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_SENSORS_BME_CTR;
    static const char* PM_MODULE_SENSORS_BME_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BME_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_BME_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BME_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mBME);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
    void AddToHardwareMessage();

    uint32_t tSavedMeasureClimate;

    void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);
    
    uint8_t fWithinLimit;
    unsigned long tWithinLimit;

    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 1
    void EveryLoop();

    // void WebAppend_Root_Status_Table_Draw();
    // void WebAppend_Root_Status_Table_Data();
    struct SENSORDATA{
      float temperature = 0;
      float humidity = 0;
      float pressure = 0;
      float altitude = 0;
      /**
       * if change exceeds this, it is a significant change
       * */
      float temperature_threshold_value;
      uint8_t isvalid=false;
      uint8_t ischanged=false;
      uint8_t ischanged_over_threshold=false;
      uint32_t ischangedtLast = millis();
      float heatIndex;
      float dewPoint;
      float cr;
      uint32_t tSavedMeasureClimate;
      uint8_t fWithinLimit;
      unsigned long tWithinLimit;
      uint8_t sReadSensor;
      Adafruit_BME280* bme = NULL;
      uint8_t i2c_address = 0x00;
    }sensor[MAX_SENSORS];

    
    uint8_t GetSensorCount(void) override
    {
      return settings.fSensorCount;
    }
    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      // Serial.printf("OVERRIDE ACCESSED DHT %d\n\r",index);Serial.println(sensor[index].instant.temperature);
      if(index > MAX_SENSORS-1) {value->type.push_back(0); return ;}
      value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
      value->data.push_back(sensor[index].temperature);
      value->data.push_back(sensor[index].humidity);
      value->sensor_id = index;
    };

        
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mBME> mqtthandler_settings_teleperiod;
    struct handler<mBME> mqtthandler_sensor_ifchanged;
    struct handler<mBME> mqtthandler_sensor_teleperiod;
 
    struct handler<mBME>* mqtthandler_list[3] = {
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
