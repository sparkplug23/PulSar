/*
* Copyright (C) 2008 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software< /span>
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* Update by K. Townsend (Adafruit Industries) for lighter typedefs, and
 * extended sensor support to include color, voltage and current */
 
#ifndef _ADAFRUIT_SENSOR_H
#define _ADAFRUIT_SENSOR_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <vector>

/* Intentionally modeled after sensors.h in the Android API:
 * https://github.com/android/platform_hardware_libhardware/blob/master/include/hardware/sensors.h */

/* Constants */
#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON              (1.6F)                  /**< The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN               (275.0F)                /**< The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 /**< Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 /**< Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              /**< Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**< Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**< Gauss to micro-Tesla multiplier */

/** Sensor types */

// Am I OR'ing these as options? ie 16 bit to describe what sensor has

typedef enum
{
  //none=0
  SENSOR2_TYPE_AMBIENT_TEMPERATURE=1,
  SENSOR2_TYPE_RELATIVE_HUMIDITY,

  SENSOR_TYPE_ACCELEROMETER,//         = (1),   /**< Gravity + linear acceleration */
  SENSOR_TYPE_MAGNETIC_FIELD,//        = (2),
  SENSOR_TYPE_ORIENTATION ,//          = (3),
  SENSOR_TYPE_GYROSCOPE,//             = (4),
  SENSOR_TYPE_LIGHT    ,//             = (5),
  SENSOR_TYPE_PRESSURE   ,//           = (6),
  SENSOR_TYPE_PROXIMITY  ,//           = (8),
  SENSOR_TYPE_GRAVITY          ,//     = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION ,//  = (10),  /**< Acceleration not including gravity */
  SENSOR_TYPE_ROTATION_VECTOR   ,//    = (11),
  SENSOR_TYPE_VOLTAGE     ,//          = (15),
  SENSOR_TYPE_CURRENT        ,//       = (16),
  SENSOR_TYPE_COLOR     ,//            = (17)
  SENSOR_TYPE_LENGTH

  // SENSOR_TYPE_AMBIENT_TEMPERATURE   = 0x01, //bit0
  // SENSOR_TYPE_RELATIVE_HUMIDITY     = 0x02, //bit1
  // SENSOR_TYPE_ACCELEROMETER         = (5),   /**< Gravity + linear acceleration */
  // SENSOR_TYPE_MAGNETIC_FIELD        = (8),
  // SENSOR_TYPE_ORIENTATION           = (4),
  // SENSOR_TYPE_GYROSCOPE             = (8),
  // SENSOR_TYPE_LIGHT                 = (16),
  // SENSOR_TYPE_PRESSURE              = (32),
  // SENSOR_TYPE_PROXIMITY             = (64),
  // SENSOR_TYPE_GRAVITY               = (128),
  // SENSOR_TYPE_LINEAR_ACCELERATION   = (256),  /**< Acceleration not including gravity */
  // SENSOR_TYPE_ROTATION_VECTOR       = (512),
  // SENSOR_TYPE_VOLTAGE               = (4096),
  // SENSOR_TYPE_CURRENT               = (8192),
  // SENSOR_TYPE_COLOR                 = (16384),
  // SENSOR_TYPE_COLOR1                 = (32768)
} sensors_type_t2;

// /** struct sensors_vec_s is used to return a vector in a common format. */
// typedef struct {
//     union {
//         float v[3];
//         struct {
//             float x;
//             float y;
//             float z;
//         };
//         /* Orientation sensors */
//         struct {
//             float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90�<=roll<=90� */
//             float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180�<=pitch<=180�) */
//             float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359� */
//         };
//     };
//     int8_t status;
//     uint8_t reserved[3];
// } sensors_vec_t;

// /** struct sensors_color_s is used to return color data in a common format. */
// typedef struct {
//     union {
//         float c[3];
//         /* RGB color space */
//         struct {
//             float r;       /**< Red component */
//             float g;       /**< Green component */
//             float b;       /**< Blue component */
//         };
//     };
//     uint32_t rgba;         /**< 24-bit RGBA value */
// } sensors_color_t;

/* Sensor event (36 bytes) */
/** struct sensor_event_s is used to provide a single sensor event in a common format. */


typedef struct
{
    
    std::vector<uint8_t> type_list;
    std::vector<float> data;
    uint8_t sensor_id;
    /**
     * @brief If more than one variable is used, return its value
     * */
    float GetValue(uint8_t type)
    {
      // Serial.printf("size() %d\n\r",type_list.size());
      
      for(uint8_t i=0;i<type_list.size();i++)
      {
        if(type == type_list[i])
        {
          Serial.printf("type == type_list[i] %d %d %d %d %d\n\r",type,type_list[i],i,type_list.size(),(int)data[1]);
          return data[i];
        }
      }
      return 234;
    };


    // uint32_t type = 0;                          
    // /**
    //  * @note If more than one sensor value is recorded, save each type in an array
    //  * */
    // uint8_t type_list[4];
    // union
    // { //Oh, these overlap, so its only a size of 4 bytes
    //   float           data[4];
    //   float           temperature;          /**< temperature is in degrees centigrade (Celsius) */
    //   float           pressure;             /**< pressure in hectopascal (hPa) */
    //   float           relative_humidity;    /**< relative humidity in percent */
    //   // sensors_vec_t   acceleration;         /**< acceleration values are in meter per second per second (m/s^2) */
    //   // sensors_vec_t   magnetic;             /**< magnetic vector values are in micro-Tesla (uT) */
    //   // sensors_vec_t   orientation;          /**< orientation values are in degrees */
    //   // sensors_vec_t   gyro;                 /**< gyroscope values are in rad/s */
    //   // float           distance;             /**< distance in centimeters */
    //   // float           light;                /**< light in SI lux units */
    //   // float           current;              /**< current in milliamps (mA) */
    //   // float           voltage;              /**< voltage in volts (V) */    
    // };



} sensors_reading_t;

/* Sensor details (40 bytes) */
/** struct sensor_s is used to describe basic information about a specific sensor. */
// typedef struct
// {
//     char     name[12];                        /**< sensor name */
//     int32_t  version;                         /**< version of the hardware + driver */
//     int32_t  sensor_id;                       /**< unique sensor identifier */
//     int32_t  type;                            /**< this sensor's type (ex. SENSOR_TYPE_LIGHT) */
//     float    max_value;                       /**< maximum value of this sensor's value in SI units */
//     float    min_value;                       /**< minimum value of this sensor's value in SI units */
//     float    resolution;                      /**< smallest difference between two values reported by this sensor */
//     int32_t  min_delay;                       /**< min delay in microseconds between events. zero = not a constant rate */
// } sensor_t;

// class Adafruit_Sensor {
//  public:
//   // Constructor(s)
//   Adafruit_Sensor() {}
//   virtual ~Adafruit_Sensor() {}

//   // These must be defined by the subclass
//   virtual void enableAutoRange(bool enabled) {};
//   virtual bool getEvent(sensors_event_t*) = 0;
//   // virtual void getSensor(sensor_t*) = 0;
  
//  private:
//   bool _autoRange;
// };

#endif
