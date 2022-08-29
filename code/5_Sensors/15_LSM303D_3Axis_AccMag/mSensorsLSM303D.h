/*
  mHVAC.cpp - mSensorsLSM303D

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
#ifndef MSENSORS_LSM303D_H
#define MSENSORS_LSM303D_H

/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low accelerometer and
magnetometer data registers. They can be converted to units of g and
gauss using the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM303D gives a magnetometer X axis reading of 1982 with
its default full scale setting of +/- 4 gauss. The M_GN specification
in the LSM303D datasheet (page 10) states a conversion factor of 0.160
mgauss/LSB (least significant bit) at this FS setting, so the raw
reading of -1982 corresponds to 1982 * 0.160 = 317.1 mgauss =
0.3171 gauss.

In the LSM303DLHC, LSM303DLM, and LSM303DLH, the acceleration data
registers actually contain a left-aligned 12-bit number, so the lowest
4 bits are always 0, and the values should be shifted right by 4 bits
(divided by 16) to be consistent with the conversion factors specified
in the datasheets.

Example: An LSM303DLH gives an accelerometer Z axis reading of -16144
with its default full scale setting of +/- 2 g. Dropping the lowest 4
bits gives a 12-bit raw value of -1009. The LA_So specification in the
LSM303DLH datasheet (page 11) states a conversion factor of 1 mg/digit
at this FS setting, so the value of -1009 corresponds to -1009 * 1 =
1009 mg = 1.009 g.
*/


#define D_UNIQUE_MODULE_SENSORS_LSM303D_ID 144

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_LSM303D

#include <Wire.h>
#include "5_Sensors/LSM303D_3Axis_AccMag/internal/LSM303.h"


class mSensorsLSM303D :
  public mTaskerInterface
{
  
  public:
    mSensorsLSM303D(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_SENSORS_LSM303D_CTR;
    static const char* PM_MODULE_SENSORS_LSM303D_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_LSM303D_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_LSM303D_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_LSM303D_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSensorsLSM303D);
    };
    #endif

    struct xyzFloat {
        float x;
        float y;
        float z;
    };
    
xyzFloat getAngles();

xyzFloat getGValues();


/**
 * Possible but not yet proven address options are:
 * 0x6b, 0x1d and 0x1E
 * */

// #ifndef I2C_ADDRESS_LSM303D
//     #define I2C_ADDRESS_LSM303D  0x6B
//   #endif

// confirmed
    #define I2C_ADDRESS_LSM303D_1  0x1D
    #define I2C_ADDRESS_LSM303D_2  0x1E

    #define I2C_ADDRESS_LSM303D_ARM  0x1D
    #define I2C_ADDRESS_LSM303D_LEG  0x1E //leg as addr was cut, so alternate address

    #define MAGNETOMETER_SAMPLES_SIZE 10
    

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
//     void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);
    
//     #define REQUIRE_COMPLETE true
//     #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 2
    void EveryLoop();
    
    void ReadSensor();
    // void ReadSensor_Instant();

    struct SENSORDATA
    {
      LSM303* lsm303d;
      uint32_t tSavedMeasure;
      int8_t address_id = -1; //set this manually with template, else, as 0 (check for any -1, set incremented and name "sens01")
      uint8_t address = 0x00;

      struct MAGNET_READINGS{
        #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
        struct AVERAGED{
          AVERAGING_DATA<float>* x;
          AVERAGING_DATA<float>* y;
          AVERAGING_DATA<float>* z;
        }average;
        #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING
        struct INSTANT{
          float x;
          float y;
          float z;
        }instant;
      }mag;
      struct ACC_READINGS{
        struct INSTANT{
          float x;
          float y;
          float z;
        }instant;
      }acc;
    }sensor[MAX_SENSORS];
      
  // struct OR2 
  // {/* Orientation sensors */
  //       // struct {
  //           float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90�<=roll<=90� */
  //           float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180�<=pitch<=180�) */
  //           float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359� */
  //       // };
  // }orientation2;
  // OR or;


//       uint8_t isvalid=false;
//       uint8_t ischanged=false;
//       uint8_t ischanged_over_threshold=false;
//       uint32_t ischangedtLast = millis();
//       float heatIndex;
//       float dewPoint;
//       float cr;
//       uint8_t fWithinLimit;
//       unsigned long tWithinLimit;
//       uint8_t sReadSensor;
//       Adafruit_BME280* bme = NULL;
    // }sensor[MAX_SENSORS];


// bool getOrientation();
// bool CalculateOrientation(LSM303::vector<int16_t> *acceleration, LSM303::vector<int16_t> *magnetic);
    
void CalculateOrientation(int16_t a_x, int16_t a_y, int16_t a_z, int16_t m_x, int16_t m_y, int16_t m_z, float* heading, float* roll, float* pitch);

//     uint8_t GetSensorCount(void) override
//     {
//       return settings.fSensorCount;
//     }
    
//     void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
//     {
//       // Serial.printf("OVERRIDE ACCESSED DHT %d\n\r",index);Serial.println(sensor[index].instant.temperature);
//       if(index > MAX_SENSORS-1) {value->type.push_back(0); return ;}
//       value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
//       value->type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
//       value->data.push_back(sensor[index].temperature);
//       value->data.push_back(sensor[index].humidity);
//       value->sensor_id = index;
//     };



        
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSensorsLSM303D> mqtthandler_settings_teleperiod;
    struct handler<mSensorsLSM303D> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsLSM303D> mqtthandler_sensor_teleperiod;
 
    struct handler<mSensorsLSM303D>* mqtthandler_list[3] = {
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
