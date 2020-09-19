#include "mSensorsBME.h"

#ifdef USE_MODULE_SENSORS_BME


int8_t mSensorsBME::Tasker(uint8_t function){

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      init();
    break;
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      // Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
      // Settings_Default();
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      // Settings_Default();
      // pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;  
    case FUNC_JSON_COMMAND:
      //function_result = parse_JSONCommand();
    break;      
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
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
    #endif //USE_MQTT
  }

  return function_result;

}




void mSensorsBME::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  // in futre use array to store bme type found (BME_280_ID, BME_180_ID) etc
  if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID)){

    Wire = new TwoWire();//pCONT_pins->GetPin(GPIO_I2C_SCL_ID),pCONT_pins->GetPin(GPIO_I2C_SDA_ID));
  
    sensor[settings.fSensorCount].bme = new Adafruit_BME280();
    if (sensor[settings.fSensorCount].bme->begin(Wire)) {
      settings.fSensorCount++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_BME "BME280 sensor not detected"));
    }
  }

  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "BME Sensor Enabled"));
  }

}

void mSensorsBME::init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
    sensor[sensor_id].tSavedMeasureClimate = millis();
    sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
  }

  sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  
}

void mSensorsBME::EveryLoop(){
    
  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasureClimate,1000)){  
      // Retry init if failed
      if(!settings.fSensorCount){
        init(); //search again
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

void mSensorsBME::WebAppend_Root_Status_Table_Draw(){

  char value_ctr[8];
  uint8_t sensor_counter = 0;
    
  BufferWriterI->Append_P(PSTR("<tr>"));
    BufferWriterI->Append_P(PSTR("<td>BME%s Temperature</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr));   
  BufferWriterI->Append_P(PSTR("</tr>"));
  BufferWriterI->Append_P(PSTR("<tr>"));
    BufferWriterI->Append_P(PSTR("<td>BME%s Humidity</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr));   
  BufferWriterI->Append_P(PSTR("</tr>"));
  BufferWriterI->Append_P(PSTR("<tr>"));
    BufferWriterI->Append_P(PSTR("<td>BME%s Pressure</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr));   
  BufferWriterI->Append_P(PSTR("</tr>"));
  
}


//append to internal buffer if any root messages table
void mSensorsBME::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  char value_ctr[8];
  char colour_ctr[10];

  JsonBuilderI->Array_Start("tab_bme");// Class name
  // BufferWriterI->Append_P(PSTR("\"%s\":["),PSTR("tab_bme")); 
  for(int row=0;row<3;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        default:
        case 0:
        
          pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr);
          JsonBuilderI->Add_FP("ih",PSTR("%s&deg;%c"),value_ctr,pCONT_sup->TempUnit());

          if(sensor[sensor_counter].temperature<=25){
            sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
          }else
          if(sensor[sensor_counter].temperature>25){
            sprintf(colour_ctr,"%s","#fcba03");
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }      
          JsonBuilderI->Add_FP("fc",colour_ctr);

        break;
        case 1:    

          pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr);
          JsonBuilderI->Add_FP("ih",PSTR("%s%%"),value_ctr);
       
          if(sensor[sensor_counter].humidity>70){
            sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }
          JsonBuilderI->Add_FP("fc",colour_ctr);

        break;
        case 2:     
          
          pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr);
          JsonBuilderI->Add_FP("ih",PSTR("%s hPa"),value_ctr);
          
          if(sensor[sensor_counter].pressure>1000){
            sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }
          JsonBuilderI->Add_FP("fc",colour_ctr);

        break;
      }
      JsonBuilderI->Level_End();
 
  }
  
  JsonBuilderI->Array_End();

}


// New function that breaks things up into switch statements
// Extra argument -- "require_completion" ie loop until status SPLIT_TASK_DONE_ID
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
          (fabsf(sensor[sensor_id].temperature-sensor[sensor_id].bme->readTemperature())>0.1)||
          (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())&&(abs(millis()-sensor[sensor_id].ischangedtLast)>60000)  
        ){
          sensor[sensor_id].ischanged_over_threshold = true; // check if updated
          sensor[sensor_id].ischangedtLast = millis();
        }else{
          sensor[sensor_id].ischanged_over_threshold = false;
        }

        sensor[sensor_id].temperature = sensor[sensor_id].bme->readTemperature();
        sensor[sensor_id].humidity =    sensor[sensor_id].bme->readHumidity();
        sensor[sensor_id].pressure =    sensor[sensor_id].bme->readPressure() / 100.0f;
        sensor[sensor_id].altitude =    sensor[sensor_id].bme->readAltitude(sealevel_pressure);

        AddLog_P(LOG_LEVEL_DEBUG,      PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE),D_TEMPERATURE,  (int)sensor[sensor_id].temperature);
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE),D_HUMIDITY,    (int)sensor[sensor_id].humidity);
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE),D_PRESSURE,    (int)sensor[sensor_id].pressure);
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE),D_ALTITUDE,    (int)sensor[sensor_id].altitude);

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


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

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
      // JsonBuilderI->Level_Start_P(D_JSON_SENSOR "%02d", sensor_id+1);   
      JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(D_MODULE_SENSORS_BME_ID,sensor_id,buffer,sizeof(buffer)));   
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


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mSensorsBME::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 600;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsBME::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 120;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsBME::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsBME::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSensorsBME::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mSensorsBME::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSensorsBME::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_BME_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

#endif
