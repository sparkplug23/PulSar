/*
  mHVAC.cpp - mSensorsL3G

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
#include "mSensorsL3G.h"

// L3GD20

#ifdef USE_MODULE_SENSORS_L3G

int8_t mSensorsL3G::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

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
    //  parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
      break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mSensorsL3G::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  // in futre use array to store L3G type found (L3G_280_ID, L3G_180_ID) etc
  // if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID)){

  // if(pCONT_sup->I2cDevice(0x76) || pCONT_sup->I2cDevice(0x77)){

    // Wire = new TwoWire();//pCONT_pins->GetPin(GPIO_I2C_SCL_ID),pCONT_pins->GetPin(GPIO_I2C_SDA_ID));
  
    // sensor[settings.fSensorCount].L3G = new Adafruit_L3G280();
    // if (sensor[settings.fSensorCount].L3G->begin(0x77, pCONT_i2c->wire)) {
    //   ALOG_INF(PSTR(D_LOG_L3G "L3G280 sensor detected"));// Serial.flush();
    //   settings.fSensorCount++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_L3G "L3G280 sensor not detected"));
    // }

// Mine were LSM - 0x1D, L3G - 0x6B, BMP - 0x77
    gyro = new L3G(pCONT_i2c->wire, 0);


  if (gyro->init())
  {
    gyro->enableDefault();
    ALOG_INF(PSTR( "L3G sensor detected"));// Serial.flush();
    settings.fSensorCount++;
  }

  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "L3G Sensor Enabled"));
  }

}


void mSensorsL3G::Init(void){

  // for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
  //   sensor[sensor_id].tSavedMeasureClimate = millis();
  //   sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
  // }

  settings.measure_rate_ms = 60000;
  
}


void mSensorsL3G::EveryLoop(){
    
//  gyro.read();

//   Serial.print("G ");
//   Serial.print("X: ");
//   Serial.print((int)gyro.g.x);
//   Serial.print(" Y: ");
//   Serial.print((int)gyro.g.y);
//   Serial.print(" Z: ");
//   Serial.println((int)gyro.g.z);

    if(mTime::TimeReachedNonReset(&tSavedMeasure,settings.measure_rate_ms)){  
      // ReadSensor();
      
      gyro->read();

      //   #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
      // sensor.mag.average.x->Add(compass->m.x);
      // sensor.mag.average.y->Add(compass->m.y);
      // sensor.mag.average.z->Add(compass->m.z);
      //   #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING

      sensor.gyro.instant.x = gyro->g.x;
      sensor.gyro.instant.y = gyro->g.y;
      sensor.gyro.instant.z = gyro->g.z;

    }
//   }
  
  
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSensorsL3G::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, settings.fSensorCount);
  return JBI->End();

}

uint8_t mSensorsL3G::ConstructJSON_Sensor(uint8_t json_level){

  JBI->Start();
  
  
    JBI->Object_Start("Gyro");
      JBI->Object_Start("Instant");
        JBI->Add("x", sensor.gyro.instant.x);
        JBI->Add("y", sensor.gyro.instant.y);
        JBI->Add("z", sensor.gyro.instant.z);
      JBI->Object_End();
    JBI->Object_End();


  return JBI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsL3G::MQTTHandler_Init(){

  struct handler<mSensorsL3G>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = 1;//FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"



/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsL3G::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsL3G::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    // if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
    //   handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    // if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
    //   handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSensorsL3G::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
