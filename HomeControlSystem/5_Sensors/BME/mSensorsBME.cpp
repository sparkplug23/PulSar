/*
  mHVAC.cpp - mSensorsBME

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
#include "mSensorsBME.h"

#ifdef USE_MODULE_SENSORS_BME

const char* mSensorsBME::PM_MODULE_SENSORS_BME_CTR = D_MODULE_SENSORS_BME_CTR;
const char* mSensorsBME::PM_MODULE_SENSORS_BME_FRIENDLY_CTR = D_MODULE_SENSORS_BME_FRIENDLY_CTR;


int8_t mSensorsBME::Tasker(uint8_t function, JsonParserObject obj){
  
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
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
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


void mSensorsBME::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  // in futre use array to store bme type found (BME_280_ID, BME_180_ID) etc
  // if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID)){

  if(pCONT_sup->I2cDevice(0x76) || pCONT_sup->I2cDevice(0x77)){

    // Wire = new TwoWire();//pCONT_pins->GetPin(GPIO_I2C_SCL_ID),pCONT_pins->GetPin(GPIO_I2C_SDA_ID));
  
    sensor[settings.fSensorCount].bme = new Adafruit_BME280();
    if (sensor[settings.fSensorCount].bme->begin(0x76, pCONT_sup->wire)) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BME "BME280 sensor detected"));// Serial.flush();
      sensor[settings.fSensorCount].i2c_address = 0x76;
      settings.fSensorCount++;
    }else
    if (sensor[settings.fSensorCount].bme->begin(0x77, pCONT_sup->wire)) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BME "BME280 sensor detected"));// Serial.flush();
      sensor[settings.fSensorCount].i2c_address = 0x77;
      settings.fSensorCount++;
    }else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BME "BME280 sensor not detected"));
    }
  }

  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "BME Sensor Enabled"));
  }

}


void mSensorsBME::Init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
    sensor[sensor_id].tSavedMeasureClimate = millis();
    sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
    sensor[sensor_id].temperature_threshold_value = 0.1; 
  }

  settings.measure_rate_ms = 5000;
  
}


void mSensorsBME::EveryLoop(){
    
  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasureClimate,settings.measure_rate_ms)){  
      // Retry init if failed
      if(!settings.fSensorCount){
        Init(); //search again
        sensor[sensor_id].tSavedMeasureClimate = millis()+(10000); //backoff period
      }else{
        SplitTask_ReadSensor(sensor_id,DONTREQUIRE_COMPLETE);
        if(sensor[sensor_id].sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
          sensor[sensor_id].sReadSensor=SPLIT_TASK_SUCCESS_ID;
          sensor[sensor_id].tSavedMeasureClimate = millis();
        }
      }
    }
  }
  
}


void mSensorsBME::SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{

    switch(sensor[sensor_id].sReadSensor){
      case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
      case SPLIT_TASK_TIMEOUT_ID:
      case SPLIT_TASK_SEC1_ID:

        sensor[sensor_id].bme->takeForcedMeasurement(); // has no effect in normal mode
        sensor[sensor_id].isvalid = true;

        if(
          (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())||
          (sensor[sensor_id].humidity != sensor[sensor_id].bme->readHumidity())         
          ){
          sensor[sensor_id].ischanged = true; // check if updated
        }else{
          sensor[sensor_id].ischanged = false;
        }
        
        if(
          (fabsf(sensor[sensor_id].temperature-sensor[sensor_id].bme->readTemperature())>sensor[sensor_id].temperature_threshold_value)||
          (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())&&(abs(millis()-sensor[sensor_id].ischangedtLast)>60000)  
        ){
          sensor[sensor_id].ischanged_over_threshold = true;
          mqtthandler_sensor_ifchanged.flags.SendNow = true;
          sensor[sensor_id].ischangedtLast = millis();
        }else{
          sensor[sensor_id].ischanged_over_threshold = false;
        }

        sensor[sensor_id].temperature = sensor[sensor_id].bme->readTemperature();
        sensor[sensor_id].humidity =    sensor[sensor_id].bme->readHumidity();
        sensor[sensor_id].pressure =    sensor[sensor_id].bme->readPressure() / 100.0f;
        sensor[sensor_id].altitude =    sensor[sensor_id].bme->readAltitude(pCONT_iSensors->settings.sealevel_pressure);

        AddLog(LOG_LEVEL_DEBUG,      PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_TEMPERATURE,  (int)sensor[sensor_id].temperature);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_HUMIDITY,    (int)sensor[sensor_id].humidity);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_PRESSURE,    (int)sensor[sensor_id].pressure);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_ALTITUDE,    (int)sensor[sensor_id].altitude);

        sensor[sensor_id].sReadSensor = SPLIT_TASK_DONE_ID;

      break;
      case SPLIT_TASK_DONE_ID: //exiting
        fWithinLimit = 1;
      break;
      default:
      break;
    } // end switch

    if(require_completion){ //delay required if we are going to do multiple calls
    //  delay(100);
    }

    if(abs(millis()-timeout)>=2000){
      sensor[sensor_id].sReadSensor = SPLIT_TASK_TIMEOUT_ID;
      break;
    }

  }while(require_completion); // loops once even if false

}//end function



uint8_t mSensorsBME::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mSensorsBME::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[50];

  for(uint8_t sensor_id = 0;sensor_id<MAX_SENSORS;sensor_id++){
    if(sensor[sensor_id].ischanged_over_threshold || (json_level>JSON_LEVEL_IFCHANGED)){
      JsonBuilderI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_BME_ID,sensor_id,buffer,sizeof(buffer)));   
        JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].temperature);
        JsonBuilderI->Add(D_JSON_HUMIDITY, sensor[sensor_id].humidity);
        JsonBuilderI->Add(D_JSON_PRESSURE, sensor[sensor_id].pressure);
        JsonBuilderI->Add(D_JSON_ALTITUDE, sensor[sensor_id].altitude);
        JsonBuilderI->Level_Start(D_JSON_ISCHANGEDMETHOD);
          JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
          JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
        JsonBuilderI->Level_End();  
      JsonBuilderI->Level_End();
    }
  }
  
  return JsonBuilderI->End();

}

#endif
