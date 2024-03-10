/*
  mHVAC.cpp - mSensorsMPU9250

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
#include "mSensorsMPU9250.h"

#ifdef USE_MODULE_SENSORS_MPU9250

const char* mSensorsMPU9250::PM_MODULE_SENSORS_MPU9250_CTR = D_MODULE_SENSORS_MPU9250_CTR;
const char* mSensorsMPU9250::PM_MODULE_SENSORS_MPU9250_FRIENDLY_CTR = D_MODULE_SENSORS_MPU9250_FRIENDLY_CTR;


int8_t mSensorsMPU9250::Tasker(uint8_t function, JsonParserObject obj){
  
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

    case FUNC_EVERY_50_MSECOND:



    break;

    case FUNC_EVERY_SECOND:

    
  // Serial.println(averaging->Mean());

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
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
      break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mSensorsMPU9250::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  // char mqtt_data[300];

  // pCONT_sup->I2cScan(mqtt_data, sizeof(mqtt_data));

  // Serial.println(mqtt_data);

  // delay(2000);

//   #define I2C_ADDRESS_MPU9250  0x1D
  #define I2C_ADDRESS_MPU9250  0x68
// #define ICM20948_ADDR 0x68

  // in futre use array to store bme type found (BME_280_ID, BME_180_ID) etc
  // if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID)){

  if(pCONT_sup->I2cDevice(I2C_ADDRESS_MPU9250)){

    // Wire = new TwoWire();//pCONT_pins->GetPin(GPIO_I2C_SCL_ID),pCONT_pins->GetPin(GPIO_I2C_SDA_ID));

    myIMU = new ICM20948_WE(pCONT_sup->wire, I2C_ADDRESS_MPU9250);
    
    if (!myIMU->init()) {
      Serial.println("ICM20948 does not respond");
    }
    else {
      Serial.println("ICM20948 is connected");
    }

    if (!myIMU->initMagnetometer()) {
      Serial.println("Magnetometer does not respond");
    }
    else {
      Serial.println("Magnetometer is connected");
    }

    
  /* You can set the following modes for the magnetometer:
   * AK09916_PWR_DOWN          Power down to save energy
   * AK09916_TRIGGER_MODE      Measurements on request, a measurement is triggered by 
   *                           calling setMagOpMode(AK09916_TRIGGER_MODE)
   * AK09916_CONT_MODE_10HZ    Continuous measurements, 10 Hz rate
   * AK09916_CONT_MODE_20HZ    Continuous measurements, 20 Hz rate
   * AK09916_CONT_MODE_50HZ    Continuous measurements, 50 Hz rate
   * AK09916_CONT_MODE_100HZ   Continuous measurements, 100 Hz rate (default)
   */
  myIMU->setMagOpMode(AK09916_CONT_MODE_100HZ);


  #define MAGNETOMETER_SAMPLES_SIZE 10

  mag.average.x = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
  mag.average.y = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);
  mag.average.z = new AVERAGING_DATA<float>(MAGNETOMETER_SAMPLES_SIZE);


  // measured.averaging->SetBoundaryLimits(0,11);


// delay(4000);


    // sensor[settings.fSensorCount].bme = new Adafruit_BME280();
    // if (sensor[settings.fSensorCount].bme->begin(0x77, pCONT_sup->wire)) {
    //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BME "BME280 sensor detected"));// Serial.flush();
    //   settings.fSensorCount++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BME "BME280 sensor not detected"));
  //   // }
  //   compass = new LSM303();
  // compass->init();
  // compass->enableDefault();
settings.fSensorCount++;


  }

  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "BME Sensor Enabled"));
  }

}


void mSensorsMPU9250::Init(void){

//   for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
//     sensor[sensor_id].tSavedMeasureClimate = millis();
//     sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
//   }

//   settings.measure_rate_ms = 1000;
  
}


void mSensorsMPU9250::EveryLoop(){
    
//   for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
//     if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasureClimate,settings.measure_rate_ms)){  
//       // Retry init if failed
//       if(!settings.fSensorCount){
//         Init(); //search again
//         sensor[sensor_id].tSavedMeasureClimate = millis()+(10000); //backoff period
//       }else{
//         SplitTask_ReadSensor(sensor_id,DONTREQUIRE_COMPLETE);
//         if(sensor[sensor_id].sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
//           sensor[sensor_id].sReadSensor=SPLIT_TASK_SUCCESS_ID;
//           sensor[sensor_id].tSavedMeasureClimate = millis();
//         }
//       }
//     }
//   }
// char report[80];

uint32_t tSaved = millis();


 myIMU->readSensor(); // takes 2-3 ms per read, very small (we should be getting around here approx 800hz)
  xyzFloat magValue = myIMU->getMagValues(); // returns magnetic flux density [µT]

// if((magValue.x == 0) || (magValue.y == 0) || (magValue.z == 0))
// {
// AddLog(LOG_LEVEL_INFO, PSTR("Sensor read time: %d \t %d \t %d"), millis()-tSaved, millis(), mag.average.x->index);

// }else{



  // AddLog(LOG_LEVEL_INFO, PSTR("Sensor read time: %d \t %d \t %d"), millis()-tSaved, millis(), mag.average.x->index);

  // Serial.println("Magnetometer Data in µTesla: ");
  // Serial.print(magValue.x);
  // Serial.print("   ");
  // Serial.print(magValue.y);
  // Serial.print("   ");
  // Serial.println(magValue.z);

    mag.average.x->Add(magValue.x);
    mag.average.y->Add(magValue.y);
    mag.average.z->Add(magValue.z);

// }

    // if(mag.average.x->index==0)
    // {

  // AddLog(LOG_LEVEL_INFO, PSTR("tResetPeriod: %d"), mag.average.x->tResetPeriod);
  //     //only print when it restarts
  // Serial.print(millis());
  // Serial.print("   ");
  // Serial.print(mag.average.x->Mean());
  // Serial.print("   ");
  // Serial.print(mag.average.y->Mean());
  // Serial.print("   ");
  // Serial.println(mag.average.z->Mean());


    // }

  // Serial.print(mag.average.x->Mean());
  // Serial.print("   ");
  // Serial.print(mag.average.y->Mean());
  // Serial.print("   ");
  // Serial.println(mag.average.z->Mean());

//   compass->read();

//   snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
//     compass->a.x, compass->a.y, compass->a.z,
//     compass->m.x, compass->m.y, compass->m.z);
//   Serial.println(report);
  
}


// void mSensorsMPU9250::SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion){

//   unsigned long timeout = millis();
//   do{

//     switch(sensor[sensor_id].sReadSensor){
//       case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
//       case SPLIT_TASK_TIMEOUT_ID:
//       case SPLIT_TASK_SEC1_ID:

//         sensor[sensor_id].bme->takeForcedMeasurement(); // has no effect in normal mode
//         sensor[sensor_id].isvalid = true;

//         if(
//           (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())||
//           (sensor[sensor_id].humidity != sensor[sensor_id].bme->readHumidity())         
//           ){
//           sensor[sensor_id].ischanged = true; // check if updated
//         }else{
//           sensor[sensor_id].ischanged = false;
//         }
        
//         if(
//           (fabsf(sensor[sensor_id].temperature-sensor[sensor_id].bme->readTemperature())>0.1)||
//           (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())&&(abs(millis()-sensor[sensor_id].ischangedtLast)>60000)  
//         ){
//           sensor[sensor_id].ischanged_over_threshold = true;
//           mqtthandler_sensor_ifchanged.flags.SendNow = true;
//           sensor[sensor_id].ischangedtLast = millis();
//         }else{
//           sensor[sensor_id].ischanged_over_threshold = false;
//         }

//         sensor[sensor_id].temperature = sensor[sensor_id].bme->readTemperature();
//         sensor[sensor_id].humidity =    sensor[sensor_id].bme->readHumidity();
//         sensor[sensor_id].pressure =    sensor[sensor_id].bme->readPressure() / 100.0f;
//         sensor[sensor_id].altitude =    sensor[sensor_id].bme->readAltitude(pCONT_iSensors->settings.sealevel_pressure);

//         AddLog(LOG_LEVEL_DEBUG,      PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_TEMPERATURE,  (int)sensor[sensor_id].temperature);
//         ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_HUMIDITY,    (int)sensor[sensor_id].humidity);
//         ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_PRESSURE,    (int)sensor[sensor_id].pressure);
//         ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_ALTITUDE,    (int)sensor[sensor_id].altitude);

//         sensor[sensor_id].sReadSensor = SPLIT_TASK_DONE_ID;

//       break;
//       case SPLIT_TASK_DONE_ID: //exiting
//         fWithinLimit = 1;
//       break;
//       default:
//       break;
//     } // end switch

//     if(require_completion){ //delay required if we are going to do multiple calls
//     //  delay(100);
//     }

//     if(abs(millis()-timeout)>=2000){
//       sensor[sensor_id].sReadSensor = SPLIT_TASK_TIMEOUT_ID;
//       break;
//     }

//   }while(require_completion); // loops once even if false

// }//end function


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSensorsMPU9250::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mSensorsMPU9250::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[50];

  JBI->Object_Start("Mag");
  JBI->Object_Start("Average");

  JBI->Add("x", mag.average.x->Mean());
  JBI->Add("y", mag.average.y->Mean());
  JBI->Add("z", mag.average.z->Mean());

    JBI->Object_End();

JBI->Add("reset", mag.average.x->tResetPeriod);


  JBI->Array_Start("Av_X");

  for(int i=0;i<mag.average.x->data_v.size();i++)
  {
    float val = mag.average.x->data_v[i];
  JBI->Add(val);
  }

  JBI->Array_End();

    JBI->Object_End();

//   for(uint8_t sensor_id = 0;sensor_id<MAX_SENSORS;sensor_id++){
//     if(sensor[sensor_id].ischanged_over_threshold || (json_level>JSON_LEVEL_IFCHANGED)){
//       JsonBuilderI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_BME_ID,sensor_id,buffer,sizeof(buffer)));   
//         JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].temperature);
//         JsonBuilderI->Add(D_JSON_HUMIDITY, sensor[sensor_id].humidity);
//         JsonBuilderI->Add(D_JSON_PRESSURE, sensor[sensor_id].pressure);
//         JsonBuilderI->Add(D_JSON_ALTITUDE, sensor[sensor_id].altitude);
//         JsonBuilderI->Object_Start(D_JSON_ISCHANGEDMETHOD);
//           JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
//           JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
//         JsonBuilderI->Object_End();  
//       JsonBuilderI->Object_End();
//     }
//   }

    // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "mSensorsMPU9250::MQTTHandler_Sender %s"), JBI->GetBufferPtr());
  
  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsMPU9250::MQTTHandler_Init(){

  struct handler<mSensorsMPU9250>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsMPU9250::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsMPU9250::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = 1;//FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsMPU9250::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"



/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsMPU9250::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsMPU9250::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSensorsMPU9250::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_MPU9250_ID, handle);
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