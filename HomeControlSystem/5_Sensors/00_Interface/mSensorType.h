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

/**
 * List of sensor types
 * */

typedef enum
{
  SENSOR_TYPE_NONE_ID=0,
  SENSOR_TYPE_TEMPERATURE_ID,
  SENSOR_TYPE_RELATIVE_HUMIDITY_ID,
  SENSOR_TYPE_ACCELEROMETER_ROLL_ID,         /**< Gravity + linear acceleration */
  SENSOR_TYPE_ACCELEROMETER_PITCH_ID,        /**< Gravity + linear acceleration */
  SENSOR_TYPE_ACCELEROMETER_HEADING_ID,      /**< Gravity + linear acceleration */
  SENSOR_TYPE_MAGNETIC_FIELD_ID, 
  SENSOR_TYPE_ORIENTATION_ID, 
  SENSOR_TYPE_GYROSCOPE_ID, 
  SENSOR_TYPE_LIGHT_ID, 
  SENSOR_TYPE_PRESSURE_ID,
  SENSOR_TYPE_PROXIMITY_ID,
  SENSOR_TYPE_GRAVITY_ID,
  SENSOR_TYPE_LINEAR_ACCELERATION_ID,   /**< Acceleration not including gravity */
  SENSOR_TYPE_ROTATION_VECTOR_ID,
  SENSOR_TYPE_VOLTAGE_ID,
  SENSOR_TYPE_CURRENT_ID,
  SENSOR_TYPE_COLOR_ID,
  SENSOR_TYPE_LENGTH_ID
} sensors_type_t2;

/**
 * @brief typedef struct uses vectors to pass sensor_values around with sensor_type
 * */
typedef struct
{
  std::vector<uint8_t> type;
  std::vector<float> data;
  uint8_t sensor_id;
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
    return -1; //invalid reading
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
