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

int8_t mSensorsMPU9250::Tasker(uint8_t function, JsonParserObject obj){
  
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

    case TASK_EVERY_50_MSECOND:



    break;

    case TASK_EVERY_SECOND:

    
      // Serial.println(averaging->Mean());

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      //tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__SUCCESS_ID;

}


void mSensorsMPU9250::Pre_Init(){

  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  // char mqtt_data[300];

  // tkr_sup->I2cScan(mqtt_data, sizeof(mqtt_data));

  // Serial.println(mqtt_data);

  // delay(2000);

//   #define I2C_ADDRESS_MPU9250  0x1D
  #define I2C_ADDRESS_MPU9250  0x68
// #define ICM20948_ADDR 0x68

  // in futre use array to store bme type found (BME_280_ID, BME_180_ID) etc
  // if(tkr_pins->PinUsed(GPIO_I2C_SCL_ID) && tkr_pins->PinUsed(GPIO_I2C_SDA_ID)){

  if(tkr_i2c->I2cDevice(I2C_ADDRESS_MPU9250)){

    // Wire = new TwoWire();//tkr_pins->GetPin(GPIO_I2C_SCL_ID),tkr_pins->GetPin(GPIO_I2C_SDA_ID));

    myIMU = new ICM20948_WE(tkr_i2c->wire, I2C_ADDRESS_MPU9250);
    
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

  mag.average.x = new Averaging_Data<float>(MAGNETOMETER_SAMPLES_SIZE);
  mag.average.y = new Averaging_Data<float>(MAGNETOMETER_SAMPLES_SIZE);
  mag.average.z = new Averaging_Data<float>(MAGNETOMETER_SAMPLES_SIZE);


  // measured.averaging->SetBoundaryLimits(0,11);


// delay(4000);


    // sensor[settings.fSensorCount].bme = new Adafruit_BME280();
    // if (sensor[settings.fSensorCount].bme->begin(0x77, tkr_i2c->wire)) {
    //   ALOG_INF(PSTR(D_LOG_BME "BME280 sensor detected"));// Serial.flush();
    //   settings.fSensorCount++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BME "BME280 sensor not detected"));
  //   // }
  //   compass = new LSM303();
  // compass->init();
  // compass->enableDefault();
  module_state.devices++;


  }

  
  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
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
// ALOG_INF(PSTR("Sensor read time: %d \t %d \t %d"), millis()-tSaved, millis(), mag.average.x->index);

// }else{



  // ALOG_INF(PSTR("Sensor read time: %d \t %d \t %d"), millis()-tSaved, millis(), mag.average.x->index);

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

  // ALOG_INF(PSTR("tResetPeriod: %d"), mag.average.x->tResetPeriod);
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


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSensorsMPU9250::parse_JSONCommand(JsonParserObject obj)
{

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsMPU9250::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, GetSensorCount());
  return JBI->End();

}

uint8_t mSensorsMPU9250::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();

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
//       JBI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_BME_ID,sensor_id,buffer,sizeof(buffer)));   
//         JBI->Add(D_TEMPERATURE, sensor[sensor_id].temperature);
//         JBI->Add(D_HUMIDITY, sensor[sensor_id].humidity);
//         JBI->Add(D_PRESSURE, sensor[sensor_id].pressure);
//         JBI->Add(D_ALTITUDE, sensor[sensor_id].altitude);
//         JBI->Object_Start(D_ISCHANGEDMETHOD);
//           JBI->Add(D_TYPE, D_SIGNIFICANTLY);
//           JBI->Add(D_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
//         JBI->Object_End();  
//       JBI->Object_End();
//     }
//   }

    // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "mSensorsMPU9250::MQTTHandler_Sender %s"), JBI->GetBufferPtr());
  
  return JBI->End();

}

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsMPU9250::Telemetry_Init(){

  struct telemetry_handler<mSensorsMPU9250>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsMPU9250::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsMPU9250::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT

#endif
