/*
  mHVAC.cpp - mSensorsDHT

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
#include "mSensorsDHT.h"

#ifdef USE_MODULE_SENSORS_DHT

const char* mSensorsDHT::PM_MODULE_SENSORS_DHT_CTR = D_MODULE_SENSORS_DHT_CTR;
const char* mSensorsDHT::PM_MODULE_SENSORS_DHT_FRIENDLY_CTR = D_MODULE_SENSORS_DHT_FRIENDLY_CTR;

int8_t mSensorsDHT::Tasker(uint8_t function, JsonParserObject obj){
  
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
    case FUNC_LOOP:
      EveryLoop();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
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
      MQTTHandler_Init(); 
    break;
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
  } // END switch
  
}// END Tasker



void mSensorsDHT::Pre_Init(void){

  settings.fEnableSensor = false;
  settings.sensor_active_count = 0;
  
  if (pCONT_pins->PinUsed(GPIO_DHT11_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT11_1OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT11_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT11_2OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT22_1OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }else{
    
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DHT "DHT Sensor 1 not found"));
    // delay(2000);
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT22_2OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  else{
    
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DHT "DHT Sensor 1 not found"));
    // delay(2000);
  }
  // {"flag_serial_set_tx_set":0,"GPIO":{"DHT22_1":25},
  // "pin_attached_gpio_functions":
  // [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
  // "user_template_io":
  // [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
  // "getpin":
  // [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]}

  if(settings.sensor_active_count){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "DHT Sensor Enabled"));
  }

}

void mSensorsDHT::Init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    sensor[sensor_id].instant.tSavedMeasureClimate = millis();
    sensor[sensor_id].instant.temperature = -1;
    sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_NOT_RUNNING_ID;
    // sensor[sensor_id].name_ptr = sensor_unset_ctr;
  }
  
}

/**
 * Needs complete rewrite
 * */
void mSensorsDHT::SplitTask_UpdateClimateSensors(uint8_t sensor_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{

    //if(!fWithinLimit){
      unsigned long tmp = (abs(millis()-sensor[sensor_id].instant.tWithinLimit));
      if(tmp<=1000){
        break;
      }
      // else{
      //   Serial.print("retryingtoosoon-"); Serial.println(tmp);
      // }
    //}

    switch(sensor[sensor_id].instant.sUpdateClimateSensors){
      case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
        // do nothing
      //break;
      default:
      case SPLIT_TASK_TIMEOUT_ID:
      case SPLIT_TASK_SEC1_ID:{

        TempAndHumidity newValues = sensor[sensor_id].dht->getTempAndHumidity();
        ComfortState cf;

        // Check if any reads failed and exit early (to try again).
        if (sensor[sensor_id].dht->getStatus() != 0) {
          AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "Read error"));
          sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_ERROR_ID;
          sensor[sensor_id].instant.isvalid = false;
        }else{
          sensor[sensor_id].instant.isvalid = true;

          if(sensor[sensor_id].instant.temperature != newValues.temperature){
            sensor[sensor_id].instant.ischanged = true; // check if updated

          }else{
            sensor[sensor_id].instant.ischanged = false;
          }

          if((fabsf(sensor[sensor_id].instant.temperature-newValues.temperature)>0.1)||
            (sensor[sensor_id].instant.temperature != newValues.temperature)&&(abs(millis()-sensor[sensor_id].instant.ischangedtLast)>60000)){
            sensor[sensor_id].instant.ischanged_over_threshold = true; // check if updated
            sensor[sensor_id].instant.ischangedtLast = millis();
          }else{
            sensor[sensor_id].instant.ischanged_over_threshold = false;
          }

          sensor[sensor_id].instant.temperature = newValues.temperature;  
          sensor[sensor_id].instant.humidity = newValues.humidity;

          //make precision as stored setting
          // dtostrf(sensor[sensor_id].instant.temperature,3,1,sensor[sensor_id].temperature_ctr);
          // dtostrf(sensor[sensor_id].humidity,3,1,sensor[sensor_id].humidity_ctr);

          sensor[sensor_id].instant.heatIndex = sensor[sensor_id].dht->computeHeatIndex(newValues.temperature, newValues.humidity);
          sensor[sensor_id].instant.dewPoint = sensor[sensor_id].dht->computeDewPoint(newValues.temperature, newValues.humidity);
          sensor[sensor_id].instant.cr = sensor[sensor_id].dht->getComfortRatio(cf, newValues.temperature, newValues.humidity);

          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "temperature %d"),(int)sensor[sensor_id].instant.temperature);
      
          sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_DONE_ID;
        }
        
      }
      break;
      case SPLIT_TASK_DONE_ID: //exiting
        sensor[sensor_id].instant.fWithinLimit = 1;
      break;
      break;
    } // end switch

    if(require_completion){ //delay required if we are going to do multiple calls
    //  delay(100);
    }

    if(abs(millis()-timeout)>=2000){
      // Serial.println("if(abs(millis()-timeout)>=2000){");
      sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_TIMEOUT_ID;
      break;
    }
    // else{
      
    //   Serial.println("ELSE if(abs(millis()-timeout)>=2000){");
    // }

  }while(require_completion); // loops once even if false

  DEBUG_LINE;

}//end function

void mSensorsDHT::EveryLoop(){
  
  for (int sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){

    if(mTime::TimeReachedNonReset(&sensor[sensor_id].instant.tSavedMeasureClimate,1000)){
      if(!settings.sensor_active_count){ // Retry init if lost after found during boot
        init(); //search again
      }else{
          SplitTask_UpdateClimateSensors(sensor_id,DONTREQUIRE_COMPLETE);
          if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
            sensor[sensor_id].instant.sUpdateClimateSensors=SPLIT_TASK_SUCCESS_ID;
            sensor[sensor_id].instant.tSavedMeasureClimate = millis();
          }else
          if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_ERROR_ID){ 
            sensor[sensor_id].instant.tSavedMeasureClimate = millis();//+5000; //backoff for 5 seconds
          }
      }

    }
  }//end for

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JBI->Add("SensorCount", settings.sensor_active_count);
    JBI->Array_Start("Pin");
      JBI->Add(pin[0]);
      JBI->Add(pin[1]);
    JBI->Array_End();
  return JsonBuilderI->End();

}


// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level){

  char buffer[50];

  JsonBuilderI->Start();
  for(uint8_t sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){
    if((sensor[sensor_id].instant.ischanged || (json_level>JSON_LEVEL_IFCHANGED))
      //  &&(sensor[sensor_id].instant.isvalid)
      ){

      JsonBuilderI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   
        JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].instant.temperature);
        JsonBuilderI->Add(D_JSON_HUMIDITY,    sensor[sensor_id].instant.humidity);
        JsonBuilderI->Level_Start(D_JSON_ISCHANGEDMETHOD);
          JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
          JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].instant.ischangedtLast)/1000));
        JsonBuilderI->Level_End();   
      JsonBuilderI->Level_End(); 
    }

  }
  //   for(int dht_id=0;dht_id<2;dht_id++){
  //   JBI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DHT_ID, dht_id, name_buffer_tmp, sizeof(name_buffer_tmp)));
  //     JBI->Add(D_JSON_TEMPERATURE, pCONT_dht->sensor[dht_id].instant.temperature);
  //     JBI->Add(D_JSON_HUMIDITY, pCONT_dht->sensor[dht_id].instant.humidity);
  //     JBI->Add(D_JSON_ISVALID, pCONT_dht->sensor[dht_id].instant.isvalid);
  //     JBI->Add(D_JSON_ISCHANGED, pCONT_dht->sensor[dht_id].instant.ischanged);
  //     // json1["iserrored"] = pCONT->mhs->climate.ptr->iserrored;
  //     // json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT->mhs->climate.ptr->raw.captureupsecs);
  //     // json1["heatindex"] = pCONT->mhs->climate.ptr->raw.heatIndex; // DONT KNOW WHAT THEY ARE
  //     // json1["dewpoint"] = pCONT->mhs->climate.ptr->raw.dewPoint;
  //   JBI->Level_End();
  // }
  
  return JsonBuilderI->End();

}


void mSensorsDHT::MQTTHandler_Init(){

  struct handler<mSensorsDHT>* mqtthandler_ptr;
  
  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10;//pCONT_set->Settings.sensors.ifchanged_secs;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsDHT::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsDHT::MQTTHandler_Set_TelePeriod()
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
void mSensorsDHT::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_DHT_ID, handle, id);
  }
}

#endif
