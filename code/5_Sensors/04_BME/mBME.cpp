/**
 * @file mBME.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief BME280 Temperature, Humidity and Pressure sensor 
 * @version 1.0
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mBME.h"

#ifdef USE_MODULE_SENSORS_BME

const char* mBME::PM_MODULE_SENSORS_BME_CTR = D_MODULE_SENSORS_BME_CTR;
const char* mBME::PM_MODULE_SENSORS_BME_FRIENDLY_CTR = D_MODULE_SENSORS_BME_FRIENDLY_CTR;


int8_t mBME::Tasker(uint8_t function, JsonParserObject obj)
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
    case FUNC_EVERY_SECOND:


      // AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("wire A/C %d/%d"), pCONT_sup->wire->getPin_Data(), pCONT_sup->wire->getPin_Clock());



    break;
    case FUNC_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
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
    // case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
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


void mBME::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  uint8_t addresses[2] = {I2C_ADDRESS_BME280_1, I2C_ADDRESS_BME280_2};

  for(uint8_t i=0;i<ARRAY_SIZE(addresses);i++)
  {

    if(pCONT_sup->I2cDevice(addresses[i]))
    {
    
      sensor[settings.fSensorCount].bme = new Adafruit_BME280();

      if (sensor[settings.fSensorCount].bme->begin(addresses[i], pCONT_sup->wire)) 
      {
        sensor[settings.fSensorCount].i2c_address = addresses[i];
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BME "BME280 sensor detected %02X"), sensor[settings.fSensorCount].i2c_address);
        settings.fSensorCount++;
      }
      else
      {
        // delete sensor[settings.fSensorCount].bme; // possible memeory leak, might need to delete then set nullptr?
        sensor[settings.fSensorCount].bme = nullptr; //ensure it is not enabled
      }
      
    }

  }
    
  #ifdef ESP32
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("getErrorText =\"%s\""), pCONT_sup->wire->getErrorText(pCONT_sup->wire->lastError()));
  #endif 

  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "BME Sensor Enabled"));
  }

}


void mBME::Init(void)
{

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++)
  {    
    sensor[sensor_id].tSavedMeasureClimate = millis();
    sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
    sensor[sensor_id].temperature_threshold_value = 0.1; 
  }

  settings.measure_rate_ms = 5000;
  
}


void mBME::EveryLoop(){
    
  for (uint8_t  
            sensor_id=0;
            sensor_id<settings.fSensorCount;
            sensor_id++
  ){
    if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasureClimate,settings.measure_rate_ms)){  
      // Retry init if failed
      if(!settings.fSensorCount){
        Init(); //search again
        sensor[sensor_id].tSavedMeasureClimate = millis()+(10000); //backoff period
      }else{

        if(sensor[sensor_id].bme != nullptr) // Only run if bme is configured
        {
          SplitTask_ReadSensor(sensor_id,DONTREQUIRE_COMPLETE);
          if(sensor[sensor_id].sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
            sensor[sensor_id].sReadSensor=SPLIT_TASK_SUCCESS_ID;
            sensor[sensor_id].tSavedMeasureClimate = millis();
          }
        }
      }
    }
  }
  
}


/**
 * @brief 
 * USe DEVFEATURE to rewrite this into a cleaner method
 * 
 * @param sensor_id 
 * @param require_completion 
 */
void mBME::SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{

    switch(sensor[sensor_id].sReadSensor){
      case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
      case SPLIT_TASK_TIMEOUT_ID:
      case SPLIT_TASK_SEC1_ID:
      {

        // sensor[sensor_id].bme->takeForcedMeasurement(); // has no effect in normal mode

        float read_temperature = sensor[sensor_id].bme->readTemperature(); // only read once per cycle, otherwise each time will poll it
        float read_humidity    = sensor[sensor_id].bme->readHumidity();
        float read_pressure    = sensor[sensor_id].bme->readPressure();
        float read_altitude    = sensor[sensor_id].bme->readAltitude(pCONT_iSensors->settings.sealevel_pressure);

        if(read_temperature == NAN) // Invalid reading
        {
          ALOG_ERR( PSTR(D_LOG_BME "Invalid Reading") );
          sensor[sensor_id].isvalid = false;
          return; // end early
        }else
        {
          sensor[sensor_id].isvalid = true;
        }


        if(
          (sensor[sensor_id].temperature != read_temperature)||
          (sensor[sensor_id].humidity != read_humidity)         
          ){
          sensor[sensor_id].ischanged = true; // check if updated
        }else{
          sensor[sensor_id].ischanged = false;
        }
        
        if(
          (fabsf(sensor[sensor_id].temperature-read_temperature)>sensor[sensor_id].temperature_threshold_value)||
          (sensor[sensor_id].temperature != read_temperature)&&(abs(millis()-sensor[sensor_id].ischangedtLast)>60000)  
        ){
          sensor[sensor_id].ischanged_over_threshold = true;
          mqtthandler_sensor_ifchanged.flags.SendNow = true;
          sensor[sensor_id].ischangedtLast = millis();
        }else{
          sensor[sensor_id].ischanged_over_threshold = false;
        }

        sensor[sensor_id].temperature = read_temperature;
        sensor[sensor_id].humidity =    read_humidity;
        sensor[sensor_id].pressure =    read_pressure / 100.0f;
        sensor[sensor_id].altitude =    read_altitude;

        ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_TEMPERATURE,  (int)sensor[sensor_id].temperature);
        ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_HUMIDITY,    (int)sensor[sensor_id].humidity);
        ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_PRESSURE,    (int)sensor[sensor_id].pressure);
        ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_ALTITUDE,    (int)sensor[sensor_id].altitude);

        sensor[sensor_id].sReadSensor = SPLIT_TASK_DONE_ID;

      }
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

void mBME::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_BME "\"%s\""),JBI->GetBufferPtr());

}



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mBME::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mBME::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();

  char buffer[50];

  for(uint8_t sensor_id = 0;sensor_id<settings.fSensorCount;sensor_id++){
    if(
      sensor[sensor_id].ischanged_over_threshold || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){
      JsonBuilderI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));   
        JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].temperature);
        JsonBuilderI->Add(D_JSON_HUMIDITY, sensor[sensor_id].humidity);
        JsonBuilderI->Add(D_JSON_PRESSURE, sensor[sensor_id].pressure);
        JsonBuilderI->Add(D_JSON_ALTITUDE, sensor[sensor_id].altitude);
        if(json_level >=  JSON_LEVEL_DETAILED)
        {          
          JsonBuilderI->Level_Start(D_JSON_ISCHANGEDMETHOD);
            JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
            JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
          JsonBuilderI->Level_End();  
        }
      JsonBuilderI->Level_End();
    }
  }
  
  return JsonBuilderI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mBME::MQTTHandler_Init()
{

  struct handler<mBME>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mBME::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mBME::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mBME::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_BME_ID, handle, id);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/

// #ifdef USE_MODULE_NETWORK_WEBSERVER
// void mBME::WebAppend_Root_Status_Table_Draw(){

//   char value_ctr[8];
//   uint8_t sensor_counter = 0;
    
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   //   BufferWriterI->Append_P(PSTR("<td>BME%s Temperature</td>"), "280");
//   //   BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr));   
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   //   BufferWriterI->Append_P(PSTR("<td>BME%s Humidity</td>"), "280");
//   //   BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr));   
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   //   BufferWriterI->Append_P(PSTR("<td>BME%s Pressure</td>"), "280");
//   //   BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr));   
//   // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  
// }


// //append to internal buffer if any root messages table
// void mBME::WebAppend_Root_Status_Table_Data(){
  
//   // uint8_t sensor_counter = 0;
//   // char value_ctr[8];
//   // char colour_ctr[10];

//   // JsonBuilderI->Array_Start("tab_bme");// Class name
//   // // BufferWriterI->Append_P(PSTR("\"%s\":["),PSTR("tab_bme")); 
//   // for(int row=0;row<3;row++){
//   //   JsonBuilderI->Level_Start();
//   //     JsonBuilderI->Add("id",row);
//   //     switch(row){
//   //       default:
//   //       case 0:
        
//   //         pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr);
//   //         JsonBuilderI->Add_FV("ih",PSTR("\"%s&deg;%c\""),value_ctr,pCONT_sup->TempUnit());

//   //         if(sensor[sensor_counter].temperature<=25){
//   //           sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
//   //         }else
//   //         if(sensor[sensor_counter].temperature>25){
//   //           sprintf(colour_ctr,"%s","#fcba03");
//   //         }else{
//   //           sprintf(colour_ctr,"%s","#ffffff");
//   //         }      
//   //         JsonBuilderI->Add("fc",colour_ctr);

//   //       break;
//   //       case 1:    

//   //         pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr);
//   //         JsonBuilderI->Add_FV("ih",PSTR("\"%s%%\""),value_ctr);
       
//   //         if(sensor[sensor_counter].humidity>70){
//   //           sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
//   //         }else{
//   //           sprintf(colour_ctr,"%s","#ffffff");
//   //         }
//   //         JsonBuilderI->Add("fc",colour_ctr);

//   //       break;
//   //       case 2:     
          
//   //         pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr);
//   //         JsonBuilderI->Add_FV("ih",PSTR("\"%s hPa\""),value_ctr);
          
//   //         if(sensor[sensor_counter].pressure>1000){
//   //           sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
//   //         }else{
//   //           sprintf(colour_ctr,"%s","#ffffff");
//   //         }
//   //         JsonBuilderI->Add("fc",colour_ctr);

//   //       break;
//   //     }
//   //     JsonBuilderI->Level_End();
 
//   // }
  
//   // JsonBuilderI->Array_End();

// }
// #endif // USE_MODULE_NETWORK_WEBSERVER




#endif
