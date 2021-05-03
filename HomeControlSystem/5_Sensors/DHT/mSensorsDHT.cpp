#include "mSensorsDHT.h"

#ifdef USE_MODULE_SENSORS_DHT

const char* mSensorsDHT::PM_MODULE_SENSORS_DHT_CTR = D_MODULE_SENSORS_DHT_CTR;
const char* mSensorsDHT::PM_MODULE_SENSORS_DHT_FRIENDLY_CTR = D_MODULE_SENSORS_DHT_FRIENDLY_CTR;


void mSensorsDHT::Pre_Init(void){

  fEnableSensor = false;
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
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT22_2OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }

  if(settings.sensor_active_count){
    fEnableSensor = true;
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
  
  // uint8_t buffer_length = 0;
  // memset(name_buffer,0,sizeof(name_buffer));
  // for(int ii=0;ii<settings.sensor_active_count;ii++){
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s%d|","sens",ii);
  // }

}

void mSensorsDHT::SplitTask_UpdateClimateSensors(uint8_t sensor_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{

    //if(!fWithinLimit){
      unsigned long tmp = (abs(millis()-sensor[sensor_id].instant.tWithinLimit));
      if(tmp<=1000){
        break;
      }else{
        //Serial.print("retryingtoosoon-"); Serial.println(tmp);
      }
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
          //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "Read error"));
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

          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DHT "temperature %d"),(int)sensor[sensor_id].instant.temperature);
      
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
      sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_TIMEOUT_ID;
      break;
    }

  }while(require_completion); // loops once even if false

  DEBUG_LINE;

}//end function



// void mSensorsDHT::GetSensorValue(sensors_reading_t* value, uint8_t index = 0)
// {

//   value->type_list.push_back(SENSOR2_TYPE_AMBIENT_TEMPERATURE);
//   value->type_list.push_back(SENSOR2_TYPE_RELATIVE_HUMIDITY);

//   value->data.push_back(sensor[index].instant.temperature);
//   value->data.push_back(sensor[index].instant.humidity);

//   value->sensor_id = index;

// }

// void mSensorsDHT::GetSensorReading(sensors_event_t* value) override{
//   // return 2;

//   value->type_list.push_back(SENSOR_TYPE_AMBIENT_TEMPERATURE);
//   value->type_list.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY);

//   value->data.push_back(12);
//   value->data.push_back(13);


// };



int8_t mSensorsDHT::Tasker(uint8_t function, JsonParserObject obj){
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
  if(!fEnableSensor){ return 0; }

  switch(function){
    case FUNC_INIT:
      Init();
    break;
    case FUNC_LOOP:
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:{
      // sensors_reading_t value;
      // GetSensorValue(&value);
      
      // AddLog(LOG_LEVEL_TEST,PSTR("value =%d"),value.type);
      // AddLog(LOG_LEVEL_TEST,PSTR("value0=%d %d"),bitRead(value.type,0),(int)value.temperature);
      // AddLog(LOG_LEVEL_TEST,PSTR("value1=%d %d"),bitRead(value.type,1),(int)value.relative_humidity);
      // AddLog(LOG_LEVEL_TEST,PSTR("value2=%d"),bitRead(value.type,2));

      // AddLog(LOG_LEVEL_TEST,PSTR("value =%d"),(int)value.GetValue(SENSOR_TYPE_AMBIENT_TEMPERATURE));


      // AddLog(LOG_LEVEL_TEST,PSTR("v_type_list =%d"),(int)value.type_list[0]);
      // AddLog(LOG_LEVEL_TEST,PSTR("v_type_list =%d"),(int)value.type_list[1]);

      // AddLog(LOG_LEVEL_TEST,PSTR("v_data0 =%d"),(int)value.v_data[0]);
      // AddLog(LOG_LEVEL_TEST,PSTR("v_data1 =%d"),(int)value.v_data[1]);

      // AddLog(LOG_LEVEL_TEST,PSTR("v_data0 =%d"),(int)value.GetValue(SENSOR_TYPE_AMBIENT_TEMPERATURE));
      // AddLog(LOG_LEVEL_TEST,PSTR("v_data1 =%d"),(int)value.GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY));

    }break;
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
  
  DEBUG_LINE;

}// END Tasker

void mSensorsDHT::EveryLoop(){
  
  for (int sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){

//temp fix
// if((sensor_id > settings.sensor_active_count)||(settings.sensor_active_count!=0)){

  
//   break;
// }


    if(mTime::TimeReachedNonReset(&sensor[sensor_id].instant.tSavedMeasureClimate,1000)){
      if(!settings.sensor_active_count){ // Retry init if lost after found during boot
        init(); //search again
      }else{
          SplitTask_UpdateClimateSensors(sensor_id,DONTREQUIRE_COMPLETE);
          if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
            sensor[sensor_id].instant.sUpdateClimateSensors=SPLIT_TASK_SUCCESS_ID;
            sensor[sensor_id].instant.tSavedMeasureClimate = millis();
          }
      }
    }
  }//end for
}

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mSensorsDHT::WebAppend_Root_Status_Table_Draw(){

  for(int ii=0;ii<settings.sensor_active_count;ii++){ //add number in name? List needed? also hold user defined name?
    
      char name_buffer_tmp[25];
      // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

      pCONT_set->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID, ii, name_buffer_tmp, sizeof(name_buffer_tmp));

      uint8_t multiline_enabled = false;


    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      JsonBuilderI->Append_P(PSTR("<td>DHT%s %s %s</td>"), "22",multiline_enabled?"Temperature":"Climate",name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
      
    if(multiline_enabled){
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
        JsonBuilderI->Append_P(PSTR("<td>DHT%s Humidity %s</td>"), "22", name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
    }
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  }
}


//append to internal buffer if any root messages table
void mSensorsDHT::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;

  // JsonBuilderI->Append_P(PSTR("\"%s\":["),PSTR("tab_dht")); 
  
  JsonBuilderI->Array_Start("tab_dht");// Class name

  for(int row=0;row<(2*settings.sensor_active_count);row++){
    switch(row%2){
      default:
      case 0:{
        
        char float_ctr[10];
        char colour_ctr[10];
        char table_row[25]; memset(table_row,0,sizeof(table_row));       

        char value_ctr[8];
        pCONT_sup->dtostrfd(sensor[sensor_counter].instant.temperature,2,value_ctr);

        sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
        
        if(sensor[sensor_counter].instant.temperature<=25){
          sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
        }else
        if(sensor[sensor_counter].instant.temperature>25){
          sprintf(colour_ctr,"%s","#fcba03");
        }else{
          sprintf(colour_ctr,"%s","#ffffff");
        }
    
        // JsonBuilderI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
        //   table_row, colour_ctr
        // );
        
        JsonBuilderI->Level_Start();
          JsonBuilderI->Add("id",row);
          JsonBuilderI->Add("ih",table_row);
          JsonBuilderI->Add("fc",colour_ctr);
        JsonBuilderI->Level_End();

      }break;
      case 1:{      

        char float_ctr[10];
        char colour_ctr[10];
        char table_row[25]; memset(table_row,0,sizeof(table_row));        
        
        char value_ctr[8];
        pCONT_sup->dtostrfd(sensor[sensor_counter].instant.humidity,2,value_ctr);

        sprintf(table_row,"%s %%",value_ctr);
        
        if(sensor[sensor_counter].instant.humidity>70){
          sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
        }else
        {
          sprintf(colour_ctr,"%s","#ffffff");
        }
    
        // JsonBuilderI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
        //   table_row, colour_ctr
        // );
        
        JsonBuilderI->Level_Start();
          JsonBuilderI->Add("id",row);
          JsonBuilderI->Add("ih",table_row);
          JsonBuilderI->Add("fc",colour_ctr);
        JsonBuilderI->Level_End();

        sensor_counter++;
      }break;
    }
  }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // JsonBuilderI->Append_P(PSTR("],")); 

  JsonBuilderI->Array_End();
}


void mSensorsDHT::WebPage_Root_AddHandlers(){

  /**
   * Pages
   * */

}

#endif // USE_MODULE_NETWORK_WEBSERVER


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JBI->Add("SensorCount", settings.sensor_active_count);
  return JsonBuilderI->End();

}



// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level){

  char buffer[50];

  JsonBuilderI->Start();
  for(uint8_t sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){
    if((sensor[sensor_id].instant.ischanged || (json_level>JSON_LEVEL_IFCHANGED))&&(sensor[sensor_id].instant.isvalid)){

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


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

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
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
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
