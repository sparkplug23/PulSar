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

#define D_UNIQUE_MODULE_SENSORS_LSM303D_ID 5015 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_LSM303D

#include <Wire.h>

#include "5_Sensors/15_LSM303D_3Axis_AccMag/internal/LSM303.h"

#include "AveragingDataLib.h"

#define MAX_LM303D_SENSORS 2

class mSensorsLSM303D :
  public mTaskerInterface
{  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mSensorsLSM303D(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_LSM303D_CTR = D_MODULE_SENSORS_LSM303D_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_LSM303D_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_LSM303D_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
  
    struct xyzFloat2 {
        float x;
        float y;
        float z;
    };
    
    xyzFloat2 getAngles();
    xyzFloat2 getGValues();

    #define I2C_ADDRESS_LSM303D_1  0x1D
    #define I2C_ADDRESS_LSM303D_2  0x1E

    uint8_t addresses[2] = {I2C_ADDRESS_LSM303D_1, I2C_ADDRESS_LSM303D_2};

    #define MAGNETOMETER_SAMPLES_SIZE 10
    

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
    void EveryLoop();
    void ReadSensor();

    struct SENSORDATA
    {
      LSM303* lsm303d;
      uint32_t tSavedMeasure;
      int8_t address_id = -1; //set this manually with template, else, as 0 (check for any -1, set incremented and name "sens01")
      uint8_t address = 0x00;

      struct MAGNET_READINGS{
        #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
        struct AVERAGED{
          Averaging_Data<float>* x;
          Averaging_Data<float>* y;
          Averaging_Data<float>* z;
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
    }sensor[MAX_LM303D_SENSORS];
      
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
      value->sensor_type.push_back(SENSOR_TYPE_ORIENTATION_ID);
      value->data_f.push_back(sensor[index].mag.instant.x);
      value->data_f.push_back(sensor[index].mag.instant.y);
      value->data_f.push_back(sensor[index].mag.instant.z);
      value->sensor_type.push_back(SENSOR_TYPE_ROTATION_VECTOR_ID);
      value->data_f.push_back(sensor[index].acc.instant.x);
      value->data_f.push_back(sensor[index].acc.instant.y);
      value->data_f.push_back(sensor[index].acc.instant.z);
      value->sensor_id = index;
    };

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void CalculateOrientation(int16_t a_x, int16_t a_y, int16_t a_z, int16_t m_x, int16_t m_y, int16_t m_z, float* heading, float* roll, float* pitch);


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
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mSensorsLSM303D>*> telemetry_list;
    struct telemetry_handler<mSensorsLSM303D> telemetry_settings;
    struct telemetry_handler<mSensorsLSM303D> telemetry_sensor_ifchanged;
    struct telemetry_handler<mSensorsLSM303D> telemetry_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

};
#endif

#endif
