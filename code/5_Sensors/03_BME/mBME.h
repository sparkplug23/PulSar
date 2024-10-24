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

#define D_UNIQUE_MODULE_SENSORS_BME_ID 5003 // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BME

#include <bme68x.h>
// #include "bme68x_defs.h"

#include <Wire.h>

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

#define I2C_ADDRESS_BME280_1 0x76 // pin low
#define I2C_ADDRESS_BME280_2 0x77 // pin high


/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define USE_BME68X

#define BMP_ADDR1            0x76
#define BMP_ADDR2            0x77

#define BMP180_CHIPID        0x55 // 85 Dec
#define BMP280_CHIPID        0x58 // 88 Dec
#define BME280_CHIPID        0x60 // 96 Dec
#define BME680_CHIPID        0x61 // 97 Dec

#define BMP_REGISTER_CHIPID  0xD0

#define BMP_REGISTER_RESET   0xE0 // Register to reset to power on defaults (used for sleep)

#define BMP_CMND_RESET       0xB6 // I2C Parameter for RESET to put BMP into reset state

#define BMP_MAX_SENSORS      2

#define BMP_MAX_SENSORS      2

class Adafruit_BME280;

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"


class mBME :
  public mTaskerInterface
{
  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mBME(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_SENSORS_BME_CTR = D_MODULE_SENSORS_BME_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BME_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BME_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

    }rt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
            
    static constexpr const char* kBmpTypes = "BMP180|BMP280|BME280|BME680";

    void ShowSensor_AddLog();
        
    typedef struct {
      
      float temperature = 0;
      float humidity = 0;
      float pressure = 0;
      float altitude = 0;
      /**
       * if change exceeds this, it is a significant change
       * */
      float temperature_threshold_value;
      uint8_t isvalid  =false;
      uint8_t ischanged = false;
      uint8_t ischanged_over_threshold=false;
      uint32_t ischangedtLast = millis();
      float heatIndex;
      float dewPoint;
      float cr;
      uint32_t tSavedMeasureClimate;
      uint8_t fWithinLimit;
      unsigned long tWithinLimit;
      uint8_t sReadSensor;
      uint8_t i2c_address = 0x00;
      char bmp_name[7];       // Sensor name - "BMPXXX"
      uint8_t bmp_type;
      uint8_t bmp_model;
    #ifdef USE_BME68X
      uint8_t bme680_state;
      float bmp_gas_resistance;
    #endif  // USE_BME68X
      uint32_t utc_measured_timestamp = 0;
    } bmp_sensors_t;

    uint8_t bmp_count = 0;
    uint8_t bmp_once = 1;

    bmp_sensors_t *bmp_sensors = nullptr;

    void BmpRead(void);


    /*********************************************************************************************\
     * BMP085 and BME180
    \*********************************************************************************************/

    #define BMP180_REG_CONTROL   0xF4
    #define BMP180_REG_RESULT    0xF6
    #define BMP180_TEMPERATURE   0x2E
    #define BMP180_PRESSURE3     0xF4 // Max. oversampling -> OSS = 3

    #define BMP180_AC1           0xAA
    #define BMP180_AC2           0xAC
    #define BMP180_AC3           0xAE
    #define BMP180_AC4           0xB0
    #define BMP180_AC5           0xB2
    #define BMP180_AC6           0xB4
    #define BMP180_VB1           0xB6
    #define BMP180_VB2           0xB8
    #define BMP180_MB            0xBA
    #define BMP180_MC            0xBC
    #define BMP180_MD            0xBE

    #define BMP180_OSS           3

    typedef struct {
      int16_t  cal_ac1;
      int16_t  cal_ac2;
      int16_t  cal_ac3;
      int16_t  cal_b1;
      int16_t  cal_b2;
      int16_t  cal_mc;
      int16_t  cal_md;
      uint16_t cal_ac4;
      uint16_t cal_ac5;
      uint16_t cal_ac6;
    } bmp180_cal_data_t;

    bmp180_cal_data_t *bmp180_cal_data = nullptr;
    
    bool Bmp180Calibration(uint8_t bmp_idx);
    void Bmp180Read(uint8_t bmp_idx);

    /*********************************************************************************************\
     * BMP280 and BME280
     *
     * Programmer : BMP280/BME280 Datasheet and Adafruit with changes by Theo Arends
    \*********************************************************************************************/

    #define BME280_REGISTER_CONTROLHUMID  0xF2
    #define BME280_REGISTER_CONTROL       0xF4
    #define BME280_REGISTER_CONFIG        0xF5
    #define BME280_REGISTER_PRESSUREDATA  0xF7
    #define BME280_REGISTER_TEMPDATA      0xFA
    #define BME280_REGISTER_HUMIDDATA     0xFD

    #define BME280_REGISTER_DIG_T1        0x88
    #define BME280_REGISTER_DIG_T2        0x8A
    #define BME280_REGISTER_DIG_T3        0x8C
    #define BME280_REGISTER_DIG_P1        0x8E
    #define BME280_REGISTER_DIG_P2        0x90
    #define BME280_REGISTER_DIG_P3        0x92
    #define BME280_REGISTER_DIG_P4        0x94
    #define BME280_REGISTER_DIG_P5        0x96
    #define BME280_REGISTER_DIG_P6        0x98
    #define BME280_REGISTER_DIG_P7        0x9A
    #define BME280_REGISTER_DIG_P8        0x9C
    #define BME280_REGISTER_DIG_P9        0x9E
    #define BME280_REGISTER_DIG_H1        0xA1
    #define BME280_REGISTER_DIG_H2        0xE1
    #define BME280_REGISTER_DIG_H3        0xE3
    #define BME280_REGISTER_DIG_H4        0xE4
    #define BME280_REGISTER_DIG_H5        0xE5
    #define BME280_REGISTER_DIG_H6        0xE7

    typedef struct {
      uint16_t dig_T1;
      int16_t  dig_T2;
      int16_t  dig_T3;
      uint16_t dig_P1;
      int16_t  dig_P2;
      int16_t  dig_P3;
      int16_t  dig_P4;
      int16_t  dig_P5;
      int16_t  dig_P6;
      int16_t  dig_P7;
      int16_t  dig_P8;
      int16_t  dig_P9;
      int16_t  dig_H2;
      int16_t  dig_H4;
      int16_t  dig_H5;
      uint8_t  dig_H1;
      uint8_t  dig_H3;
      int8_t   dig_H6;
    } Bme280CalibrationData_t;

    Bme280CalibrationData_t *Bme280CalibrationData = nullptr;

    bool Bmx280Calibrate(uint8_t bmp_idx);
    void Bme280Read(uint8_t bmp_idx);

    /*********************************************************************************************\
     * BMP680
     *
     * Programmer : BMP260 Datasheet and Adafruit with changes by Theo Arends
    \*********************************************************************************************/

    struct bme68x_dev *bme_dev = nullptr;
    struct bme68x_conf *bme_conf = nullptr;
    struct bme68x_heatr_conf *bme_heatr_conf = nullptr;

    static void Bme68x_Delayus(uint32_t period, void *intf_ptr);
    bool Bme680Init(uint8_t bmp_idx);
    void Bme680Read(uint8_t bmp_idx);


    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return bmp_count;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > bmp_count-1) {value->sensor_type.push_back(0); return ;}  
      
      value->timestamp = bmp_sensors[index].utc_measured_timestamp;

      switch (bmp_sensors[index].bmp_type) 
      {
        case BME280_CHIPID:
          value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          value->data_f.push_back(bmp_sensors[index].humidity);
          // nobreak intentional
        case BMP280_CHIPID:    
        case BMP180_CHIPID:        
          value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
          value->data_f.push_back(bmp_sensors[index].temperature);
          value->sensor_type.push_back(SENSOR_TYPE_PRESSURE_ID);
          value->data_f.push_back(bmp_sensors[index].pressure);          
        break;
        case BME680_CHIPID:        
          value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
          value->data_f.push_back(bmp_sensors[index].temperature);
          value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          value->data_f.push_back(bmp_sensors[index].humidity);   
          value->sensor_type.push_back(SENSOR_TYPE_PRESSURE_ID);
          value->data_f.push_back(bmp_sensors[index].pressure);
          value->sensor_type.push_back(SENSOR_TYPE_GAS_RESISTANCE_ID);
          value->data_f.push_back(bmp_sensors[index].bmp_gas_resistance);   
        break;
      }
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

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
    std::vector<struct handler<mBME>*> mqtthandler_list;
    struct handler<mBME> mqtthandler_settings;
    struct handler<mBME> mqtthandler_sensor_ifchanged;
    struct handler<mBME> mqtthandler_sensor_teleperiod;    
    #endif // USE_MODULE_NETWORK_MQTT
 
};


#endif // USE_MODULE_SENSORS_BME
#endif //header guard
