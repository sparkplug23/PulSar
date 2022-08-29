/*
  mSensorType.h - Unified variable type for passing sensor data

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
 
#ifndef _M_SENSOR_TYPE_H
#define _M_SENSOR_TYPE_H

#include <vector>


/* Constants */
#define SENSORS_GRAVITY_EARTH                   (9.80665F) /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON                    (1.6F)      /**< The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN                     (275.0F)     /**< The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD                (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX              (60.0F) /**< Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN              (30.0F) /**< Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA            (1013.25F) /**< Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS                     (0.017453293F) /**< Degrees/s to rad/s multiplier  */
#define SENSORS_RADS_TO_DPS                     (57.29577793F) /**< Rad/s to degrees/s  multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA             (100) /**< Gauss to micro-Tesla multiplier */

// #define SENSOR_TYPE_INVALID_READING -2.0f 
// #define SENSOR_TYPE_INVALID_READING std::numeric_limits<float>::min;
#define SENSOR_TYPE_INVALID_READING -10000.0f//std::numeric_limits<float>::min;


/**
 * List of sensor types
 * */

/**
 * @brief For types that come in triples (e.g x/y/z axis, it will always be assumed the data is packed in three)
 * 
 */
typedef enum
{
  /**
   * @brief Atmosphere
   **/
  SENSOR_TYPE_TEMPERATURE_ID,
  SENSOR_TYPE_RELATIVE_HUMIDITY_ID,
  SENSOR_TYPE_PRESSURE_ID,
  /**
   * @brief Light
   **/
  SENSOR_TYPE_LIGHT_LEVEL_ID,         //remove level, instead, make sure LUX has decimel points for more precision  //SENSOR_TYPE_LIGHT_LUX_ID
  SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID, 
  /**
   * @brief IMU
   **/
  SENSOR_TYPE_ACCELEROMETER_ROLL_ID,         /**< Gravity + linear acceleration */
  SENSOR_TYPE_ACCELEROMETER_PITCH_ID,        /**< Gravity + linear acceleration */
  SENSOR_TYPE_ACCELEROMETER_HEADING_ID,      /**< Gravity + linear acceleration */
  /**
   * @brief x,y,z axis
   **/
  SENSOR_TYPE_ACCELEROMETER_ID, 
  SENSOR_TYPE_LINEAR_ACCELERATION_ID,   /**< Acceleration not including gravity */
  /**
   * @brief x,y,z axis
   **/
  SENSOR_TYPE_MAGNETIC_FIELD_ID, 
  /**
   * @brief x,y,z axis
   **/
  SENSOR_TYPE_ORIENTATION_ID,   
  SENSOR_TYPE_GYROSCOPE_ID, 
  SENSOR_TYPE_ROTATION_VECTOR_ID,
  /**
   * @brief Distance
   **/
  SENSOR_TYPE_PROXIMITY_ID,
  
  
  SENSOR_TYPE_GRAVITY_ID,
  SENSOR_TYPE_VOLTAGE_ID,
  SENSOR_TYPE_CURRENT_ID,
  SENSOR_TYPE_COLOR_ID,
  SENSOR_TYPE_SUN_AZIMUTH_ID,
  SENSOR_TYPE_SUN_ELEVATION_ID,

  SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED_ID,
  SENSOR_TYPE_GAME_ROTATION_VECTOR_ID,
  SENSOR_TYPE_GYROSCOPE_UNCALIBRATED_ID,
  SENSOR_TYPE_SIGNIFICANT_MOTION_ID,
  SENSOR_TYPE_STEP_DETECTOR_ID,
  SENSOR_TYPE_STEP_COUNTER_ID,
  SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR_ID,
  SENSOR_TYPE_HEART_RATE_ID,
  SENSOR_TYPE_TILT_DETECTOR_ID,
  SENSOR_TYPE_WAKE_GESTURE_ID,
  SENSOR_TYPE_GLANCE_GESTURE_ID,
  SENSOR_TYPE_PICK_UP_GESTURE_ID,
  SENSOR_TYPE_POSE_6DOF_ID,
  SENSOR_TYPE_STATIONARY_DETECT_ID,
  SENSOR_TYPE_MOTION_DETECT_ID,
  SENSOR_TYPE_HEART_BEAT_ID,
  SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED_ID,
  SENSOR_TYPE_HINGE_ANGLE_ID,

  
  SENSOR_TYPE_ULTRASONIC_DISTANCE_CM_ID,

  /**
   * @brief Door states
   **/
  SENSOR_TYPE_DOOR_POSITION_ID,
  SENSOR_TYPE_DOOR_LOCKED_ID,



  SENSOR_TYPE_LENGTH_ID // 256 max types
} sensors_type_t2;



/**
* @brief https://github.com/aosp-mirror/platform_hardware_libhardware/blob/master/include/hardware/sensors.h
***/

/**
 * @brief typedef struct uses vectors to pass sensor_values around with sensor_type
 * */
typedef struct
{
  std::vector<uint8_t> type;
  std::vector<float> data;
  uint8_t sensor_id;
  // Do I need these? perhaps just so I can pass that information back, or, assume detailed info like this will not be passed but accessed only directly within module
  float    max_value;                       /**< maximum value of this sensor's value in SI units */
  float    min_value;                       /**< minimum value of this sensor's value in SI units */
  float    resolution;                      /**< smallest difference between two values reported by this sensor */
  /**
   * @brief If more than one variable is used, return its value
   * @param type sensor_type identifier
   * @param index default is 1 (ie only one instance of that sensor_type), if more than 1 exist (eg multiple temp sensor readings) then indexing can return which one
   * */
  float GetValue(uint8_t type_search, uint8_t index = 1)
  {    
    for(uint8_t i=0;i<type.size();i++)
    {
      // I will want to expand this method so it can return if more than 1 of the same data_type exists
      if(type_search == type[i])
      {
        // Serial.printf("type == type[i] %d %d %d %d %d\n\r",type,type[i],i,type.size(),(int)data[1]);
        return data[i];
      }
    }
    return SENSOR_TYPE_INVALID_READING; //invalid reading
  };
  /**
   * @brief If more than one variable is used, return its value
   * @param type sensor_type identifier
   * @param index default is 1 (ie only one instance of that sensor_type), if more than 1 exist (eg multiple temp sensor readings) then indexing can return which one
   * */
  bool HasValue(uint8_t type_search, uint8_t index = 1)
  {    
    for(uint8_t i=0;i<type.size();i++)
    {
      // I will want to expand this method so it can return if more than 1 of the same data_type exists
      if(type_search == type[i])
      {
        // Serial.printf("type == type[i] %d %d %d %d %d\n\r",type,type[i],i,type.size(),(int)data[1]);
        return true;
      }
    }
    return false; //invalid reading
  };
  bool Valid()
  {
    return !data.empty();
  };
} sensors_reading_t;

#endif // _M_SENSOR_TYPE_H
