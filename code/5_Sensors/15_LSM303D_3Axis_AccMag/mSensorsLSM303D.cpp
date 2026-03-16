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
#include "mSensorsLSM303D.h"

#ifdef USE_MODULE_SENSORS_LSM303D // accelerometer and magnetic compass

int8_t mSensorsLSM303D::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;   
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return TASKER_RESULT__SUCCESS_ID;

}


/**
 * tmp method which assumes both, force the sensors
 * Needs redone, as this is coded for the SOLO3DR leg/arm
 * */
void mSensorsLSM303D::Pre_Init()
{

  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  for (uint8_t i = 0; i < MAX_LM303D_SENSORS; i++)
  {
    sensor[i].lsm303d = new LSM303(tkr_i2c->wire, addresses[i]);

    if (sensor[i].lsm303d->init(LSM303::device_D, LSM303::sa0_high))
    {
      ALOG_INF(PSTR("LSM303D %02x sensor detected %d"), addresses[i], i);
      sensor[i].lsm303d->enableDefault();
      sensor[i].address_id = i;
      sensor[i].address = addresses[i];
      module_state.devices++;
    }
    else
    {
      delete sensor[i].lsm303d;
      sensor[i].lsm303d = nullptr;
    }
  }
 
  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}

// For LSM303D magnetometer full-scale, CTRL6 bits MFS[6:5] are:
// 0x00 = ±2 gauss
// 0x20 = ±4 gauss
// 0x40 = ±8 gauss
// 0x60 = ±12 gauss
void mSensorsLSM303D::Init(void)
{
  #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
  for (uint8_t sensor_id = 0; sensor_id < module_state.devices; sensor_id++)
  {
    sensor[sensor_id].mag.average.x = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
    sensor[sensor_id].mag.average.y = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
    sensor[sensor_id].mag.average.z = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
  }
  #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING

  settings.measure_rate_ms = 60000; // 1000/MAGNETOMETER_SAMPLES_SIZE;

  for (uint8_t sensor_id = 0; sensor_id < module_state.devices; sensor_id++)
  {
    if (sensor[sensor_id].lsm303d == nullptr) {
      continue;
    }

    /**
     * Temporary per-sensor precision/range reminder.
     * Proper version should become a later command/setter.
     */
    if (sensor_id == 0)
    {
      sensor[sensor_id].lsm303d->writeReg(LSM303::CTRL6, 0x20);  // ±4 gauss
      ALOG_INF(PSTR("LSM303D[%d] mag scale set to +/-4 gauss"), sensor_id);
    }
    else if (sensor_id == 1)
    {
      sensor[sensor_id].lsm303d->writeReg(LSM303::CTRL6, 0x00);  // ±2 gauss
      ALOG_INF(PSTR("LSM303D[%d] mag scale set to +/-2 gauss"), sensor_id);
    }
  }
}


void mSensorsLSM303D::EveryLoop(){
    
  for(int sensor_id=0; sensor_id<module_state.devices; sensor_id++)
  {
    if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasure,settings.measure_rate_ms))
    {  
      // ReadSensor();
      
      sensor[sensor_id].lsm303d->read();

      #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
      sensor.mag.average.x->Add(compass->m.x);
      sensor.mag.average.y->Add(compass->m.y);
      sensor.mag.average.z->Add(compass->m.z);
      #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING

      sensor[sensor_id].mag.instant.x = sensor[sensor_id].lsm303d->m.x;
      sensor[sensor_id].mag.instant.y = sensor[sensor_id].lsm303d->m.y;
      sensor[sensor_id].mag.instant.z = sensor[sensor_id].lsm303d->m.z;

      sensor[sensor_id].acc.instant.x = sensor[sensor_id].lsm303d->a.x;
      sensor[sensor_id].acc.instant.y = sensor[sensor_id].lsm303d->a.y;
      sensor[sensor_id].acc.instant.z = sensor[sensor_id].lsm303d->a.z;

      // getOrientation();
      
// char report[80];
//   snprintf(report, sizeof(report), "%d:%02X\tA: %6d %6d %6d    M: %6d %6d %6d",
//   sensor_id, sensor[sensor_id].address, 
//     sensor[sensor_id].lsm303d->a.x, sensor[sensor_id].lsm303d->a.y, sensor[sensor_id].lsm303d->a.z,
//     sensor[sensor_id].lsm303d->m.x, sensor[sensor_id].lsm303d->m.y, sensor[sensor_id].lsm303d->m.z);
//   Serial.println(report);

    }
  }

}


void mSensorsLSM303D::ReadSensor()
{


}


// void mSensorsLSM303D::ReadSensor_Instant()
// {

//   compass->read();

//   sensor.mag.instant.x = compass->m.x;
//   sensor.mag.instant.y = compass->m.y;
//   sensor.mag.instant.z = compass->m.z;

//   sensor.acc.instant.x = compass->a.x;
//   sensor.acc.instant.y = compass->a.y;
//   sensor.acc.instant.z = compass->a.z;

//   getOrientation();

// }




// float mSensorsLSM303D::getPitch(){
//     xyzFloat angleVal = getAngles();
//     float pitch = (atan2(angleVal.x, sqrt(abs((angleVal.x*angleVal.y + angleVal.z*angleVal.z))))*180.0)/M_PI;
//     return pitch;
// }
    
// float mSensorsLSM303D::getRoll(){
//     xyzFloat angleVal = getAngles();
//     float roll = (atan2(angleVal.y, angleVal.z)*180.0)/M_PI;
//     return roll;
// }



// xyzFloat ICM20948_WE::getGValues(){
//     xyzFloat gVal, accRawVal;
//     accRawVal = getCorrectedAccRawValues();
    
//     gVal.x = accRawVal.x * accRangeFactor / 16384.0;
//     gVal.y = accRawVal.y * accRangeFactor / 16384.0;
//     gVal.z = accRawVal.z * accRangeFactor / 16384.0;
//     return gVal;
// }

// xyzFloat mSensorsLSM303D::getAngles(){
//     xyzFloat angleVal;
//     xyzFloat gVal = getGValues();
//     if(gVal.x > 1.0){
//         gVal.x = 1.0;
//     }
//     else if(gVal.x < -1.0){
//         gVal.x = -1.0;
//     }
//     angleVal.x = (asin(gVal.x)) * 57.296;
    
//     if(gVal.y > 1.0){
//         gVal.y = 1.0;
//     }
//     else if(gVal.y < -1.0){
//         gVal.y = -1.0;
//     }
//     angleVal.y = (asin(gVal.y)) * 57.296;
    
//     if(gVal.z > 1.0){
//         gVal.z = 1.0;
//     }
//     else if(gVal.z < -1.0){
//         gVal.z = -1.0;
//     }
//     angleVal.z = (asin(gVal.z)) * 57.296;
    
//     return angleVal;
// }



/**************************************************************************/
/*!
 * @brief Compute orientation based on accelerometer and magnetometer data.
 */
/**************************************************************************/
void mSensorsLSM303D::CalculateOrientation(
  int16_t a_x, int16_t a_y, int16_t a_z, 
  int16_t m_x, int16_t m_y, int16_t m_z, 
  float* heading, float* roll, float* pitch
){
  
  xyzFloat acceleration;
  xyzFloat magnetic;

  acceleration.x = a_x;
  acceleration.y = a_y;
  acceleration.z = a_z;

  magnetic.x = m_x;
  magnetic.y = m_y;
  magnetic.z = m_z;

  struct OR{ /* Orientation sensors */
    float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90�<=roll<=90� */
    float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180�<=pitch<=180�) */
    float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359� */
  }orientation;  

  float const PI_F = 3.14159265F;

  // roll: Rotation around the X-axis. -180 <= roll <= 180
  // a positive roll angle is defined to be a clockwise rotation about the
  // positive X-axis
  //
  //                    y
  //      roll = atan2(---)
  //                    z
  //
  // where:  y, z are returned value from accelerometer sensor
  orientation.roll = (float)atan2(acceleration.y, acceleration.z);

  // pitch: Rotation around the Y-axis. -180 <= roll <= 180
  // a positive pitch angle is defined to be a clockwise rotation about the
  // positive Y-axis
  //
  //                                 -x
  //      pitch = atan(-------------------------------)
  //                    y * sin(roll) + z * cos(roll)
  //
  // where:  x, y, z are returned value from accelerometer sensor
  if(
    acceleration.y * sin(orientation.roll) +
    acceleration.z * cos(orientation.roll) ==
    0
  ){
    orientation.pitch =
        acceleration.x > 0 ? (PI_F / 2) : (-PI_F / 2);  // when exactly +- 90 ie straight up or down
  }
  else  //ascending or decending
  {
    orientation.pitch =
        (float)atan(-acceleration.x /
                    (acceleration.y * sin(orientation.roll) +
                     acceleration.z * cos(orientation.roll)));
  }

  // heading: Rotation around the Z-axis. -180 <= roll <= 180
  // a positive heading angle is defined to be a clockwise rotation about the
  // positive Z-axis
  //
  //                                       z * sin(roll) - y * cos(roll)
  //   heading =
  //   atan2(--------------------------------------------------------------------------)
  //                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z *
  //                    sin(pitch) * cos(roll))
  //
  // where:  x, y, z are returned value from magnetometer sensor
  orientation.heading =
      (float)atan2
        (magnetic.z * sin(orientation.roll) -
                       magnetic.y * cos(orientation.roll),
                   magnetic.x * cos(orientation.pitch) +
                       magnetic.y * sin(orientation.pitch) *
                           sin(orientation.roll) +
                       magnetic.z * sin(orientation.pitch) *
                           cos(orientation.roll)
        );

  // Convert angular data to degree
  *roll    = orientation.roll * 180 / PI_F;
  *pitch   = orientation.pitch * 180 / PI_F;
  *heading = orientation.heading * 180 / PI_F;
  
  // Serial.printf("heading,roll,pitch \t%f\t%f\t%f\n\r", *heading, *roll, *pitch);

  // float heading_or_rotation_around_z_axis


  // orientation.roll = (float)atan2(acceleration.y, acceleration.z);




}


// /**************************************************************************/
// /*!
//  * @brief Compute orientation based on accelerometer and magnetometer data.
//  */
// /**************************************************************************/
// bool mSensorsLSM303D::CalculateOrientation(LSM303::vector<int16_t> *acceleration, LSM303::vector<int16_t> *magnetic)
// {
  
//   // Validate input and available sensors.
//   // if (orientation == NULL || _accel == NULL || _mag == NULL)
//   //   return false;

//   // struct xyzFloat {
//   //     float x;
//   //     float y;
//   //     float z;
//   // };

//   // xyzFloat acceleration;
//   // xyzFloat magnetic;


//   // // Grab an acceleromter and magnetometer reading.
//   // compass->read();

//   // acceleration.x = compass->a.x;
//   // acceleration.y = compass->a.y;
//   // acceleration.z = compass->a.z;

//   // magnetic.x = compass->m.x;
//   // magnetic.y = compass->m.y;
//   // magnetic.z = compass->m.z;

//   struct OR /* Orientation sensors */
//   {
//     float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90�<=roll<=90� */
//     float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180�<=pitch<=180�) */
//     float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359� */
//   }orientation;
  

//   float const PI_F = 3.14159265F;

//   // roll: Rotation around the X-axis. -180 <= roll <= 180
//   // a positive roll angle is defined to be a clockwise rotation about the
//   // positive X-axis
//   //
//   //                    y
//   //      roll = atan2(---)
//   //                    z
//   //
//   // where:  y, z are returned value from accelerometer sensor
//   orientation.roll = (float)atan2(acceleration->y, acceleration->z);

//   // pitch: Rotation around the Y-axis. -180 <= roll <= 180
//   // a positive pitch angle is defined to be a clockwise rotation about the
//   // positive Y-axis
//   //
//   //                                 -x
//   //      pitch = atan(-------------------------------)
//   //                    y * sin(roll) + z * cos(roll)
//   //
//   // where:  x, y, z are returned value from accelerometer sensor
//   if (acceleration->y * sin(orientation.roll) +
//           acceleration->z * cos(orientation.roll) ==
//       0)
//   {
//     orientation.pitch =
//         acceleration->x > 0 ? (PI_F / 2) : (-PI_F / 2);
//   }
//   else
//   {
//     orientation.pitch =
//         (float)atan(-acceleration->x /
//                     (acceleration->y * sin(orientation.roll) +
//                      acceleration->z * cos(orientation.roll)));
//   }

//   // heading: Rotation around the Z-axis. -180 <= roll <= 180
//   // a positive heading angle is defined to be a clockwise rotation about the
//   // positive Z-axis
//   //
//   //                                       z * sin(roll) - y * cos(roll)
//   //   heading =
//   //   atan2(--------------------------------------------------------------------------)
//   //                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z *
//   //                    sin(pitch) * cos(roll))
//   //
//   // where:  x, y, z are returned value from magnetometer sensor
//   orientation.heading =
//       (float)atan2(magnetic->z * sin(orientation.roll) -
//                        magnetic->y * cos(orientation.roll),
//                    magnetic->x * cos(orientation.pitch) +
//                        magnetic->y * sin(orientation.pitch) *
//                            sin(orientation.roll) +
//                        magnetic->z * sin(orientation.pitch) *
//                            cos(orientation.roll));

//   // Convert angular data to degree
//   orientation.roll = orientation.roll * 180 / PI_F;
//   orientation.pitch = orientation.pitch * 180 / PI_F;
//   orientation.heading = orientation.heading * 180 / PI_F;
  
//   orientation2.roll = orientation.roll;
//   orientation2.pitch = orientation.pitch;
//   orientation2.heading = orientation.heading;

//   // Serial.printf("heading,roll,pitch \t%f\t%f\t%f\n\r", orientation2.heading, orientation2.roll, orientation2.pitch);

//   return true;
// }



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSensorsLSM303D::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, module_state.devices);
  return JBI->End();

}



uint8_t mSensorsLSM303D::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();

  char buffer[40];

  for(int sensor_id=0;sensor_id<module_state.devices;sensor_id++)
  { //db18_sensors_active
   
  //  JBI->Object_Start(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_LSM303D_ID,sensor[sensor_id].address_id,buffer,sizeof(buffer)));         
   
   JBI->Object_Start( DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer)));         
   



    JBI->Object_Start("Mag");
      // #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
      // JBI->Object_Start("Average");
      //   JBI->Add("x", sensor.mag.average.x->Mean());
      //   JBI->Add("y", sensor.mag.average.y->Mean());
      //   JBI->Add("z", sensor.mag.average.z->Mean());
      // JBI->Object_End();
      // #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING
      JBI->Object_Start("Instant");
        JBI->Add("x", sensor[sensor_id].mag.instant.x);
        JBI->Add("y", sensor[sensor_id].mag.instant.y);
        JBI->Add("z", sensor[sensor_id].mag.instant.z);
      JBI->Object_End();
    JBI->Object_End();

    // JBI->Object_Start("Orientation");
    
    //   float heading;
    //   float roll;
    //   float pitch;
    //   CalculateOrientation(
    //     sensor[sensor_id].acc.instant.x, 
    //     sensor[sensor_id].acc.instant.y, 
    //     sensor[sensor_id].acc.instant.z, 
    //     sensor[sensor_id].mag.instant.x, 
    //     sensor[sensor_id].mag.instant.y, 
    //     sensor[sensor_id].mag.instant.z, &heading, &roll, &pitch);

    //   JBI->Object_Start("Instant");
    //     JBI->Add("Heading", heading);
    //     JBI->Add("Roll", roll);
    //     JBI->Add("Pitch", pitch);
    //   JBI->Object_End();
      
    // JBI->Object_End();

    JBI->Object_End();
  }
  
  return JBI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsLSM303D::MQTTHandler_Init(){

  struct handler<mSensorsLSM303D>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = 1;//FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Sensor;
  
}

#endif // USE_MODULE_NETWORK_MQTT

#endif



// #include <Wire.h>
// #include <LSM303.h>

// LSM303 compass;

// char report[80];

// void setup()
// {
//   Serial.begin(9600);
//   Wire.begin();
//   compass.init();
//   compass.enableDefault();
// }

// void loop()
// {
//   compass.read();

//   snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
//     compass.a.x, compass.a.y, compass.a.z,
//     compass.m.x, compass.m.y, compass.m.z);
//   Serial.println(report);

//   delay(100);
// }