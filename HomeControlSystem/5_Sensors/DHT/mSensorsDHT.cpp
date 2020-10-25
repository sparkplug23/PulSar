#include "mSensorsDHT.h"

#ifdef USE_MODULE_SENSORS_DHT


void mSensorsDHT::Pre_Init(void){

  fEnableSensor = false;
  settings.sensor_active_count = 0;
  
  if (pCONT_pins->PinUsed(GPIO_DHT11_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_set->pin[GPIO_DHT11_1OF2_ID];
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT11_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_set->pin[GPIO_DHT11_2OF2_ID];
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_set->pin[GPIO_DHT22_1OF2_ID];
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_set->pin[GPIO_DHT22_2OF2_ID];
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }

  if(settings.sensor_active_count){
    fEnableSensor = true;
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "DHT Sensor Enabled"));
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
          //AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "Read error"));
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

          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DHT "temperature %d"),(int)sensor[sensor_id].instant.temperature);
      
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






int8_t mSensorsDHT::Tasker(uint8_t function){
  
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
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    #endif //USE_MODULE_CORE_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
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
    #endif //USE_MQTT
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

void mSensorsDHT::WebAppend_Root_Status_Table_Draw(){

  for(int ii=0;ii<settings.sensor_active_count;ii++){ //add number in name? List needed? also hold user defined name?
    
      char name_buffer_tmp[25];
      // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

      pCONT_set->GetDeviceName(D_MODULE_SENSORS_DHT_ID, ii, name_buffer_tmp, sizeof(name_buffer_tmp));

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


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_method){

  //   memset(&data_buffer2,0,sizeof(data_buffer2));
  //   StaticJsonDocument<400> doc;
  //   JsonObject root = doc.to<JsonObject>();

  //   root["SensorCount"] = settings.sensor_active_count;

  //   root["name_buffer"] = name_buffer;

  //   root["sens_tele_rate"] =mqtthandler_sensor_teleperiod.tRateSecs;
  //   root["sens_ifchanged_rate"] =mqtthandler_sensor_ifchanged.tRateSecs;
  //   root["sett_tele_rate"] =mqtthandler_settings_teleperiod.tRateSecs;


  //   data_buffer2.payload.len = measureJson(root)+1;
  //   serializeJson(doc,data_buffer2.payload.ctr);
    
  // return (data_buffer2.payload.len>3?1:0);

  return 0;

}



// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level){

  // clear entire mqtt packet
  memset(&data_buffer2,0,sizeof(data_buffer2));

  uint8_t ischanged=false;

  StaticJsonDocument<300> doc;
  JsonObject root = doc.to<JsonObject>();


for(uint8_t sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){

  ischanged = sensor[sensor_id].instant.ischanged;

  //if none changed, return early
  // if(!ischanged){
  //   return;
  // }name_buffer
  
  if(ischanged || (json_level>JSON_LEVEL_IFCHANGED)){

  
  char name_buffer_tmp[25];
  // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), sensor_id, name_buffer);

      pCONT_set->GetDeviceName(D_MODULE_SENSORS_DHT_ID, sensor_id, name_buffer_tmp, sizeof(name_buffer_tmp));



  //if(ischanged||pCONT->mqt->fSendAllData||pCONT->mqt->fSendSingleFunctionData){
    if(sensor[sensor_id].instant.isvalid){
    JsonObject sens1 = root.createNestedObject(name_buffer_tmp);//sensor[sensor_id].name_ptr);
    sens1["temp"] = sensor[sensor_id].instant.temperature;
    sens1["hum"] = sensor[sensor_id].instant.humidity;
    }
  //}

    JsonObject method = root.createNestedObject("ischangedmethod");
    //method["type"] = "significantly"; //or "any"
    method["value"] = 0.1;
    method["tLast"] = round(abs(millis()-sensor[0].instant.ischangedtLast)/1000);

}//if changed level




}

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);
  
  return (data_buffer2.payload.len>3?1:0);

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSensorsDHT::MQTTHandler_Init(){

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
  mqtthandler_ptr->tRateSecs = 120;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.ifchanged_secs;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSensorsDHT::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSensorsDHT::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSensorsDHT::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      // No specialised needed
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_DHT_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

#endif
