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

#ifdef USE_MODULE_SENSORS_LSM303D

const char* mSensorsLSM303D::PM_MODULE_SENSORS_LSM303D_CTR = D_MODULE_SENSORS_LSM303D_CTR;
const char* mSensorsLSM303D::PM_MODULE_SENSORS_LSM303D_FRIENDLY_CTR = D_MODULE_SENSORS_LSM303D_FRIENDLY_CTR;


int8_t mSensorsLSM303D::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;   
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
    //  parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
      break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


/**
 * tmp method which assumes both, force the sensors
 * */
void mSensorsLSM303D::Pre_Init()
{

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  /**
   * Arm sensor (with gyro): Keep +-2 range on mag
   * */
  sensor[0].lsm303d = new LSM303(pCONT_sup->wire, I2C_ADDRESS_LSM303D_ARM);
  sensor[0].lsm303d->init(LSM303::device_D, LSM303::sa0_high); //pulled high is defualt address
  AddLog(LOG_LEVEL_INFO, PSTR("LSM303D %02x sensor detected %d"), I2C_ADDRESS_LSM303D_ARM, 0);
  sensor[0].lsm303d->enableDefault();
  // 0x20 = 0b00100000
  // MFS = 01 (+/- 4 gauss full scale)
  sensor[0].lsm303d->writeReg(LSM303::CTRL6, 0x20);                ///////////// change here   -- forced +-4 range (default)
  sensor[0].address_id = 0;
  sensor[0].address = I2C_ADDRESS_LSM303D_ARM;
  settings.fSensorCount++;

  /**
   * Leg sensor (withOUT gyro): Extended range of +-4
   * */
  sensor[1].lsm303d = new LSM303(pCONT_sup->wire, I2C_ADDRESS_LSM303D_LEG);
  sensor[1].lsm303d->init(LSM303::device_D, LSM303::sa0_low); //pulled high is defualt address
  AddLog(LOG_LEVEL_INFO, PSTR("LSM303D %02x sensor detected %d"), I2C_ADDRESS_LSM303D_LEG, 0);
  sensor[1].lsm303d->enableDefault();
  // 0x20 = 0b00100000
  // MFS = 01 (+/- 4 gauss full scale)
  sensor[1].lsm303d->writeReg(LSM303::CTRL6, 0x00);                ///////////// change here   -- forced +-2 range (not default)
  sensor[1].address_id = 1;
  sensor[1].address = I2C_ADDRESS_LSM303D_LEG;
  settings.fSensorCount++;

 
  if(settings.fSensorCount)
  {
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR("LSM303D Sensor Enabled %d"),settings.fSensorCount);
    AddLog(LOG_LEVEL_INFO,PSTR("LSM303D Sensor Enabled settings.fSensorCount %d %d"),sensor[0].address, I2C_ADDRESS_LSM303D_ARM);
    AddLog(LOG_LEVEL_INFO,PSTR("LSM303D Sensor Enabled settings.fSensorCount %d %d"),sensor[1].address, I2C_ADDRESS_LSM303D_LEG);
  }

}


// void mSensorsLSM303D::Pre_Init()
// {

//   settings.fEnableSensor = false;
//   settings.fSensorCount = 0;

//   uint8_t addresses[] = {I2C_ADDRESS_LSM303D_1, I2C_ADDRESS_LSM303D_2};

//   if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID))
//   {

//     for(int i=0;i<sizeof(addresses);i++)
//     {
    
//       if(pCONT_sup->I2cDevice(addresses[i]))
//       {
//         sensor[settings.fSensorCount].lsm303d = new LSM303(pCONT_sup->wire, addresses[i]);
//         // if(sensor[settings.fSensorCount].lsm303d->init_addressed(addresses[i]))  // should not be needed if the address is correctly within wire
//         // if(sensor[settings.fSensorCount].lsm303d->init())  // should not be needed if the address is correctly within wire
//         if(sensor[settings.fSensorCount].lsm303d->init(LSM303::device_D, i?LSM303::sa0_low:LSM303::sa0_high))  // should not be needed if the address is correctly within wire
//         {
//           AddLog(LOG_LEVEL_INFO, PSTR("LSM303D %02x sensor detected %d"), addresses[i], i);
//           sensor[settings.fSensorCount].lsm303d->enableDefault();


//           /**
//            * Arm sensor (with gyro): Keep +-2 range on mag
//            * */
//           if(addresses[i] == I2C_ADDRESS_LSM303D_ARM)
//           {
//             // Accelerometer

//             // 0x57 = 0b 0101 0111
//             // AFS = 0 (+/- 2 g full scale)
//             // sensor[settings.fSensorCount].lsm303d->writeReg(LSM303::CTRL2, 0x00);

//             // 0x57 = 0b01010111
//             // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
//             // writeReg(CTRL1, 0x57);

//             // Magnetometer

//             // 0x64 = 0b01100100
//             // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
//             // writeReg(CTRL5, 0x64);

//             // 0x20 = 0b00100000
//             // MFS = 01 (+/- 4 gauss full scale)
//             sensor[settings.fSensorCount].lsm303d->writeReg(LSM303::CTRL6, 0x00);                ///////////// change here

//             // 0x00 = 0b00000000
//             // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
//             // writeReg(CTRL7, 0x00);
//           }


//           /**
//            * Leg sensor (withOUT gyro): Extended range of +-4
//            * */
//           if(addresses[i] == I2C_ADDRESS_LSM303D_LEG)
//           {
//             // Accelerometer

//             // 0x57 = 0b 0101 0111
//             // AFS = 0 (+/- 2 g full scale)
//             // sensor[settings.fSensorCount].lsm303d->writeReg(CTRL2, 0x00);

//             // 0x57 = 0b01010111
//             // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
//             // sensor[settings.fSensorCount].lsm303d->writeReg(CTRL1, 0x57);

//             // Magnetometer

//             // 0x64 = 0b01100100
//             // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
//             // sensor[settings.fSensorCount].lsm303d->writeReg(CTRL5, 0x64);

//             // 0x20 = 0b00100000
//             // MFS = 01 (+/- 4 gauss full scale)
//             sensor[settings.fSensorCount].lsm303d->writeReg(LSM303::CTRL6, 0x20);                ///////////// change here

//             // 0x00 = 0b00000000
//             // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
//             // sensor[settings.fSensorCount].lsm303d->writeReg(CTRL7, 0x00);
//           }









//           sensor[settings.fEnableSensor].address_id = settings.fEnableSensor;
//           sensor[settings.fEnableSensor].address = addresses[i];
//           settings.fSensorCount++;
//         }
//       }

//     }

//   }

  
//   if(settings.fSensorCount)
//   {
//     settings.fEnableSensor = true;
//     AddLog(LOG_LEVEL_INFO,PSTR("LSM303D Sensor Enabled %d"),settings.fSensorCount);
//   }

// }


void mSensorsLSM303D::Init(void){

    int sensor_id=0;
        #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
    sensor.mag.average.x = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
    sensor.mag.average.y = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
    sensor.mag.average.z = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
        #endif //  ENABLE_SENSOR_LSM303D_READING_AVERAGING
    settings.measure_rate_ms = 60000;//1000/MAGNETOMETER_SAMPLES_SIZE; // Gives samples per 1 second
  
}


void mSensorsLSM303D::EveryLoop(){
    
  for(int sensor_id=0; sensor_id<settings.fSensorCount; sensor_id++)
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


  
// char report[80];
//   snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
//     compass->a.x, compass->a.y, compass->a.z,
//     compass->m.x, compass->m.y, compass->m.z);
//   Serial.println(report);

  // Serial.println();


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

uint8_t mSensorsLSM303D::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}



uint8_t mSensorsLSM303D::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[40];

  for(int sensor_id=0;sensor_id<settings.fSensorCount;sensor_id++)
  { //db18_sensors_active
   
  //  JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_LSM303D_ID,sensor[sensor_id].address_id,buffer,sizeof(buffer)));         
   
   JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_LSM303D_ID,sensor_id,buffer,sizeof(buffer)));         
   
    JBI->Level_Start("Mag");
      // #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
      // JBI->Level_Start("Average");
      //   JBI->Add("x", sensor.mag.average.x->Mean());
      //   JBI->Add("y", sensor.mag.average.y->Mean());
      //   JBI->Add("z", sensor.mag.average.z->Mean());
      // JBI->Level_End();
      // #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING
      JBI->Level_Start("Instant");
        JBI->Add("x", sensor[sensor_id].mag.instant.x);
        JBI->Add("y", sensor[sensor_id].mag.instant.y);
        JBI->Add("z", sensor[sensor_id].mag.instant.z);
      JBI->Level_End();
    JBI->Level_End();

    // JBI->Level_Start("Orientation");
    
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

    //   JBI->Level_Start("Instant");
    //     JBI->Add("Heading", heading);
    //     JBI->Add("Roll", roll);
    //     JBI->Add("Pitch", pitch);
    //   JBI->Level_End();
      
    // JBI->Level_End();

    JBI->Level_End();
  }
  
  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsLSM303D::MQTTHandler_Init(){

  struct handler<mSensorsLSM303D>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = 1;//FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsLSM303D::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"



/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsLSM303D::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsLSM303D::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    // if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
    //   handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSensorsLSM303D::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_LSM303D_ID, handle, id);
  }
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