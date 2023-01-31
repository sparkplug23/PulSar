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

#include "mBMP.h"

#ifdef USE_MODULE_SENSORS_BMP

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

// if (!I2cEnabled(XI2C_10)) { return false; }

//   bool result = false;

//   if (FUNC_INIT == function) {
//     BmpDetect();
//   }
//   else if (bmp_count) {
//     switch (function) {
//       case FUNC_EVERY_SECOND:
//         BmpRead();
//         break;
//       case FUNC_JSON_APPEND:
//         BmpShow(1);
//         break;
// #ifdef USE_WEBSERVER
//       case FUNC_WEB_SENSOR:
//         BmpShow(0);
//         break;
// #endif  // USE_WEBSERVER
// #ifdef USE_DEEPSLEEP
//       case FUNC_SAVE_BEFORE_RESTART:
//         BMP_EnterSleep();
//         break;
// #endif // USE_DEEPSLEEP
//     }
//   }
//   return result;



void mBME::Pre_Init(){
// XI2C_10
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



bool Bmp180Calibration(uint8_t bmp_idx)
{
  if (!bmp180_cal_data) {
    bmp180_cal_data = (bmp180_cal_data_t*)malloc(BMP_MAX_SENSORS * sizeof(bmp180_cal_data_t));
  }
  if (!bmp180_cal_data) { return false; }

  bmp180_cal_data[bmp_idx].cal_ac1 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC1);
  bmp180_cal_data[bmp_idx].cal_ac2 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC2);
  bmp180_cal_data[bmp_idx].cal_ac3 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC3);
  bmp180_cal_data[bmp_idx].cal_ac4 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC4);
  bmp180_cal_data[bmp_idx].cal_ac5 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC5);
  bmp180_cal_data[bmp_idx].cal_ac6 = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_AC6);
  bmp180_cal_data[bmp_idx].cal_b1  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_VB1);
  bmp180_cal_data[bmp_idx].cal_b2  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_VB2);
  bmp180_cal_data[bmp_idx].cal_mc  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_MC);
  bmp180_cal_data[bmp_idx].cal_md  = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_MD);

  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if (!bmp180_cal_data[bmp_idx].cal_ac1 |
      !bmp180_cal_data[bmp_idx].cal_ac2 |
      !bmp180_cal_data[bmp_idx].cal_ac3 |
      !bmp180_cal_data[bmp_idx].cal_ac4 |
      !bmp180_cal_data[bmp_idx].cal_ac5 |
      !bmp180_cal_data[bmp_idx].cal_ac6 |
      !bmp180_cal_data[bmp_idx].cal_b1 |
      !bmp180_cal_data[bmp_idx].cal_b2 |
      !bmp180_cal_data[bmp_idx].cal_mc |
      !bmp180_cal_data[bmp_idx].cal_md) {
    return false;
  }

  if ((bmp180_cal_data[bmp_idx].cal_ac1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac3 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac4 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac5 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac6 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_mc == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_md == (int16_t)0xFFFF)) {
    return false;
  }
  return true;
}

void Bmp180Read(uint8_t bmp_idx)
{
  if (!bmp180_cal_data) { return; }

  I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_CONTROL, BMP180_TEMPERATURE);
  delay(5); // 5ms conversion time
  int ut = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_RESULT);
  int32_t xt1 = (ut - (int32_t)bmp180_cal_data[bmp_idx].cal_ac6) * ((int32_t)bmp180_cal_data[bmp_idx].cal_ac5) >> 15;
  int32_t xt2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_mc << 11) / (xt1 + (int32_t)bmp180_cal_data[bmp_idx].cal_md);
  int32_t bmp180_b5 = xt1 + xt2;
  bmp_sensors[bmp_idx].bmp_temperature = ((bmp180_b5 + 8) >> 4) / 10.0f;

  I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_CONTROL, BMP180_PRESSURE3); // Highest resolution
  delay(2 + (4 << BMP180_OSS));                                 // 26ms conversion time at ultra high resolution
  uint32_t up = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BMP180_REG_RESULT);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b2 * ((b6 * b6) >> 12)) >> 11;
  int32_t x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)bmp180_cal_data[bmp_idx].cal_ac1 * 4 + x3) << BMP180_OSS) + 2) >> 2;

  x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac3 * b6) >> 13;
  x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)bmp180_cal_data[bmp_idx].cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

  int32_t p;
  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  }
  else {
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p += ((x1 + x2 + (int32_t)3791) >> 4);
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 100.0f;  // convert to mbar
}

/*********************************************************************************************\
 * BMP280 and BME280
 *
 * Programmer : BMP280/BME280 Datasheet and Adafruit with changes by Theo Arends
\*********************************************************************************************/

#define BME280_REGISTER_CONTROLHUMID  0xF2
#define BME280_REGISTER_CONTROL       0xF4
#define BME280_REGISTER_CONFIG        0xF5
#define BME280_REGISTER_PRESSUREDATA  0xF7
#define BME280_REGISTER_TEMPDATA      0xFA
#define BME280_REGISTER_HUMIDDATA     0xFD

#define BME280_REGISTER_DIG_T1        0x88
#define BME280_REGISTER_DIG_T2        0x8A
#define BME280_REGISTER_DIG_T3        0x8C
#define BME280_REGISTER_DIG_P1        0x8E
#define BME280_REGISTER_DIG_P2        0x90
#define BME280_REGISTER_DIG_P3        0x92
#define BME280_REGISTER_DIG_P4        0x94
#define BME280_REGISTER_DIG_P5        0x96
#define BME280_REGISTER_DIG_P6        0x98
#define BME280_REGISTER_DIG_P7        0x9A
#define BME280_REGISTER_DIG_P8        0x9C
#define BME280_REGISTER_DIG_P9        0x9E
#define BME280_REGISTER_DIG_H1        0xA1
#define BME280_REGISTER_DIG_H2        0xE1
#define BME280_REGISTER_DIG_H3        0xE3
#define BME280_REGISTER_DIG_H4        0xE4
#define BME280_REGISTER_DIG_H5        0xE5
#define BME280_REGISTER_DIG_H6        0xE7

typedef struct {
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;
  int16_t  dig_H2;
  int16_t  dig_H4;
  int16_t  dig_H5;
  uint8_t  dig_H1;
  uint8_t  dig_H3;
  int8_t   dig_H6;
} Bme280CalibrationData_t;

Bme280CalibrationData_t *Bme280CalibrationData = nullptr;

bool Bmx280Calibrate(uint8_t bmp_idx)
{
  //  if (I2cRead8(bmp_address, BMP_REGISTER_CHIPID) != BME280_CHIPID) return false;

  if (!Bme280CalibrationData) {
    Bme280CalibrationData = (Bme280CalibrationData_t*)malloc(BMP_MAX_SENSORS * sizeof(Bme280CalibrationData_t));
  }
  if (!Bme280CalibrationData) { return false; }

  Bme280CalibrationData[bmp_idx].dig_T1 = I2cRead16LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T1);
  Bme280CalibrationData[bmp_idx].dig_T2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T2);
  Bme280CalibrationData[bmp_idx].dig_T3 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_T3);
  Bme280CalibrationData[bmp_idx].dig_P1 = I2cRead16LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P1);
  Bme280CalibrationData[bmp_idx].dig_P2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P2);
  Bme280CalibrationData[bmp_idx].dig_P3 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P3);
  Bme280CalibrationData[bmp_idx].dig_P4 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P4);
  Bme280CalibrationData[bmp_idx].dig_P5 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P5);
  Bme280CalibrationData[bmp_idx].dig_P6 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P6);
  Bme280CalibrationData[bmp_idx].dig_P7 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P7);
  Bme280CalibrationData[bmp_idx].dig_P8 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P8);
  Bme280CalibrationData[bmp_idx].dig_P9 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_P9);
  if (BME280_CHIPID == bmp_sensors[bmp_idx].bmp_type) {  // #1051
    Bme280CalibrationData[bmp_idx].dig_H1 = I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H1);
    Bme280CalibrationData[bmp_idx].dig_H2 = I2cReadS16_LE(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H2);
    Bme280CalibrationData[bmp_idx].dig_H3 = I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H3);
    Bme280CalibrationData[bmp_idx].dig_H4 = (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H4) << 4) | (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H4 + 1) & 0xF);
    Bme280CalibrationData[bmp_idx].dig_H5 = (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H5 + 1) << 4) | (I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H5) >> 4);
    Bme280CalibrationData[bmp_idx].dig_H6 = (int8_t)I2cRead8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_DIG_H6);
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0x00);      // sleep mode since writes to config can be ignored in normal mode (Datasheet 5.4.5/6 page 27)
    // Set before CONTROL_meas (DS 5.4.3)
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROLHUMID, 0x01); // 1x oversampling
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONFIG, 0xA0);       // 1sec standby between measurements (to limit self heating), IIR filter off
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0x27);      // 1x oversampling, normal mode
  } else {
    I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_CONTROL, 0xB7);      // 16x oversampling, normal mode (Adafruit)
  }

  return true;
}

void Bme280Read(uint8_t bmp_idx)
{
  if (!Bme280CalibrationData) { return; }

  int32_t adc_T = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  int32_t vart1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1 << 1))) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_T2)) >> 11;
  int32_t vart2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1))) >> 12) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_T3)) >> 14;
  int32_t t_fine = vart1 + vart2;
  float T = (t_fine * 5 + 128) >> 8;
  bmp_sensors[bmp_idx].bmp_temperature = T / 100.0f;

  int32_t adc_P = I2cRead24(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P6;
  var2 = var2 + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P5) << 17);
  var2 = var2 + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P3) >> 8) + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Bme280CalibrationData[bmp_idx].dig_P1) >> 33;
  if (0 == var1) {
    return; // avoid exception caused by division by zero
  }
  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P7) << 4);
  bmp_sensors[bmp_idx].bmp_pressure = (float)p / 25600.0f;

  if (BMP280_CHIPID == bmp_sensors[bmp_idx].bmp_type) { return; }

  int32_t adc_H = I2cRead16(bmp_sensors[bmp_idx].bmp_address, BME280_REGISTER_HUMIDDATA);

  int32_t v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)Bme280CalibrationData[bmp_idx].dig_H4) << 20) -
    (((int32_t)Bme280CalibrationData[bmp_idx].dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r >> 12);
  bmp_sensors[bmp_idx].bmp_humidity = h / 1024.0f;
}

#ifdef USE_BME68X
/*********************************************************************************************\
 * BME68x support by Bosch https://github.com/BoschSensortec/BME68x-Sensor-API
\*********************************************************************************************/

//#define BME68X_DO_NOT_USE_FPU

#include <bme68x.h>

struct bme68x_dev *bme_dev = nullptr;
struct bme68x_conf *bme_conf = nullptr;
struct bme68x_heatr_conf *bme_heatr_conf = nullptr;

static void Bme68x_Delayus(uint32_t period, void *intf_ptr) {
  delayMicroseconds(period);
}

int8_t Bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return I2cReadBuffer(dev_addr, reg_addr, reg_data, (uint16_t)len);
}

int8_t Bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return I2cWriteBuffer(dev_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)len);
}

bool Bme680Init(uint8_t bmp_idx) {
  if (!bme_dev) {
    bme_heatr_conf = (bme68x_heatr_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_heatr_conf));
    bme_conf = (bme68x_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_conf));
    bme_dev = (bme68x_dev*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_dev));
  }
  if (!bme_dev) { return false; }

  bme_dev[bmp_idx].intf_ptr = &bmp_sensors[bmp_idx].bmp_address;
  bme_dev[bmp_idx].intf = BME68X_I2C_INTF;
  bme_dev[bmp_idx].read = Bme68x_i2c_read;
  bme_dev[bmp_idx].write = Bme68x_i2c_write;
  bme_dev[bmp_idx].delay_us = Bme68x_Delayus;
  // amb_temp can be set to 25 prior to configuring the gas sensor
  // or by performing a few temperature readings without operating the gas sensor.
  bme_dev[bmp_idx].amb_temp = 25;
  int8_t rslt = bme68x_init(&bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BME: Gas variant %d"), bme_dev[bmp_idx].variant_id);

//  rslt = bme68x_get_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
//  if (rslt != BME68X_OK) { return false; }
  // Set the temperature, pressure and humidity settings
  bme_conf[bmp_idx].os_hum = BME68X_OS_2X;
  bme_conf[bmp_idx].os_pres = BME68X_OS_4X;
  bme_conf[bmp_idx].os_temp = BME68X_OS_8X;
  bme_conf[bmp_idx].filter = BME68X_FILTER_SIZE_3;
  bme_conf[bmp_idx].odr = BME68X_ODR_NONE;          // This parameter defines the sleep duration after each profile
  rslt = bme68x_set_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  // Set the gas sensor settings
  bme_heatr_conf[bmp_idx].enable = BME68X_ENABLE;
  // Create a ramp heat waveform in 3 steps
  bme_heatr_conf[bmp_idx].heatr_temp = 320;  // degree Celsius
  bme_heatr_conf[bmp_idx].heatr_dur = 150;   // milliseconds
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heatr_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  bmp_sensors[bmp_idx].bme680_state = 0;

  return true;
}

void Bme680Read(uint8_t bmp_idx)
{
  if (!bme_dev) { return; }

  int8_t rslt = BME68X_OK;

  if (BME680_CHIPID == bmp_sensors[bmp_idx].bmp_type) {
    if (0 == bmp_sensors[bmp_idx].bme680_state) {
      // Trigger the next measurement if you would like to read data out continuously
      rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }

      // Calculate delay period in microseconds
//      del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
//      bme.delay_us(del_period, bme.intf_ptr);

      bmp_sensors[bmp_idx].bme680_state = 1;
    } else {
      bmp_sensors[bmp_idx].bme680_state = 0;

      struct bme68x_data data;
      uint8_t n_fields;
      rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }

#ifdef BME68X_DO_NOT_USE_FPU
      bmp_sensors[bmp_idx].bmp_temperature = data.temperature / 100.0f;  // Temperature in degree celsius x100
      bmp_sensors[bmp_idx].bmp_humidity = data.humidity / 1000.0f;       // Humidity in % relative humidity x1000
#else
      bmp_sensors[bmp_idx].bmp_temperature = data.temperature;          // Temperature in degree celsius
      bmp_sensors[bmp_idx].bmp_humidity = data.humidity;                // Humidity in % relative humidity
#endif
      bmp_sensors[bmp_idx].bmp_pressure = data.pressure / 100.0f;        // Pressure in Pascal (converted to hPa)
      // Avoid using measurements from an unstable heating setup
      if (data.status & BME68X_GASM_VALID_MSK) {
        bmp_sensors[bmp_idx].bmp_gas_resistance = data.gas_resistance / 1000.0f;  // Gas resistance in Ohms (converted to kOhm)
      } else {
        bmp_sensors[bmp_idx].bmp_gas_resistance = 0;
      }
    }
  }
  return;
}

#endif  // USE_BME68X

/********************************************************************************************/

void BmpDetect(void)
{
  int bmp_sensor_size = BMP_MAX_SENSORS * sizeof(bmp_sensors_t);
  if (!bmp_sensors) {
    bmp_sensors = (bmp_sensors_t*)malloc(bmp_sensor_size);
  }
  if (!bmp_sensors) { return; }
  memset(bmp_sensors, 0, bmp_sensor_size);  // Init defaults to 0

  for (uint32_t i = 0; i < BMP_MAX_SENSORS; i++) {
    if (!I2cSetDevice(bmp_addresses[i])) { continue; }
    uint8_t bmp_type = I2cRead8(bmp_addresses[i], BMP_REGISTER_CHIPID);
    if (bmp_type) {
      bmp_sensors[bmp_count].bmp_address = bmp_addresses[i];
      bmp_sensors[bmp_count].bmp_type = bmp_type;
      bmp_sensors[bmp_count].bmp_model = 0;

      bool success = false;
      switch (bmp_type) {
        case BMP180_CHIPID:
          success = Bmp180Calibration(bmp_count);
          break;
        case BME280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 2
        case BMP280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 1
          success = Bmx280Calibrate(bmp_count);
          break;
#ifdef USE_BME68X
        case BME680_CHIPID:
          bmp_sensors[bmp_count].bmp_model = 3;  // 3
          success = Bme680Init(bmp_count);
          break;
#endif  // USE_BME68X
      }
      if (success) {
        GetTextIndexed(bmp_sensors[bmp_count].bmp_name, sizeof(bmp_sensors[bmp_count].bmp_name), bmp_sensors[bmp_count].bmp_model, kBmpTypes);
        I2cSetActiveFound(bmp_sensors[bmp_count].bmp_address, bmp_sensors[bmp_count].bmp_name);
        bmp_count++;
      }
    }
  }
}

void BmpRead(void)
{
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    switch (bmp_sensors[bmp_idx].bmp_type) {
      case BMP180_CHIPID:
        Bmp180Read(bmp_idx);
        break;
      case BMP280_CHIPID:
      case BME280_CHIPID:
        Bme280Read(bmp_idx);
        break;
#ifdef USE_BME68X
      case BME680_CHIPID:
        Bme680Read(bmp_idx);
        break;
#endif  // USE_BME68X
    }
  }
}

void BmpShow(bool json)
{
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
    if (bmp_sensors[bmp_idx].bmp_type) {
      float bmp_sealevel = ConvertPressureForSeaLevel(bmp_sensors[bmp_idx].bmp_pressure);
      float bmp_temperature = ConvertTemp(bmp_sensors[bmp_idx].bmp_temperature);
      float bmp_pressure = ConvertPressure(bmp_sensors[bmp_idx].bmp_pressure);

      char name[10];
      strlcpy(name, bmp_sensors[bmp_idx].bmp_name, sizeof(name));
      if (bmp_count > 1) {
        snprintf_P(name, sizeof(name), PSTR("%s%c%02X"), name, IndexSeparator(), bmp_sensors[bmp_idx].bmp_address);  // BMXXXX-XX
      }

      char pressure[33];
      dtostrfd(bmp_pressure, Settings->flag2.pressure_resolution, pressure);
      char sea_pressure[33];
      dtostrfd(bmp_sealevel, Settings->flag2.pressure_resolution, sea_pressure);

      float bmp_humidity = ConvertHumidity(bmp_sensors[bmp_idx].bmp_humidity);
      char humidity[33];
      dtostrfd(bmp_humidity, Settings->flag2.humidity_resolution, humidity);
      float f_dewpoint = CalcTempHumToDew(bmp_temperature, bmp_humidity);
      char dewpoint[33];
      dtostrfd(f_dewpoint, Settings->flag2.temperature_resolution, dewpoint);
#ifdef USE_BME68X
      char gas_resistance[33];
      dtostrfd(bmp_sensors[bmp_idx].bmp_gas_resistance, 2, gas_resistance);
#endif  // USE_BME68X

      if (json) {
        char json_humidity[80];
        snprintf_P(json_humidity, sizeof(json_humidity), PSTR(",\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_DEWPOINT "\":%s"), humidity, dewpoint);
        char json_sealevel[40];
        snprintf_P(json_sealevel, sizeof(json_sealevel), PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
#ifdef USE_BME68X
        char json_gas[40];
        snprintf_P(json_gas, sizeof(json_gas), PSTR(",\"" D_JSON_GAS "\":%s"), gas_resistance);

        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s%s}"),
          name,
          Settings->flag2.temperature_resolution, &bmp_temperature,
          (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "",
          pressure,
          (Settings->altitude != 0) ? json_sealevel : "",
          (bmp_sensors[bmp_idx].bmp_model >= 3) ? json_gas : "");
#else
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s,\"" D_JSON_PRESSURE "\":%s%s}"),
          name, Settings->flag2.temperature_resolution, &bmp_temperature, (bmp_sensors[bmp_idx].bmp_model >= 2) ? json_humidity : "", pressure, (Settings->altitude != 0) ? json_sealevel : "");
#endif  // USE_BME68X

#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == bmp_idx)) {  // We want the same first sensor to report to Domoticz in case a read is missed
          DomoticzTempHumPressureSensor(bmp_temperature, bmp_humidity, bmp_pressure);
#ifdef USE_BME68X
          if (bmp_sensors[bmp_idx].bmp_model >= 3) { DomoticzSensor(DZ_AIRQUALITY, (uint32_t)bmp_sensors[bmp_idx].bmp_gas_resistance); }
#endif  // USE_BME68X
        }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
        if (0 == TasmotaGlobal.tele_period) {
          KnxSensor(KNX_TEMPERATURE, bmp_temperature);
          KnxSensor(KNX_HUMIDITY, bmp_humidity);
        }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(name, bmp_temperature);
        if (bmp_sensors[bmp_idx].bmp_model >= 2) {
          WSContentSend_PD(HTTP_SNS_HUM, name, humidity);
          WSContentSend_PD(HTTP_SNS_DEW, name, dewpoint, TempUnit());
        }
        WSContentSend_PD(HTTP_SNS_PRESSURE, name, pressure, PressureUnit().c_str());
        if (Settings->altitude != 0) {
          WSContentSend_PD(HTTP_SNS_SEAPRESSURE, name, sea_pressure, PressureUnit().c_str());
        }
#ifdef USE_BME68X
        if (bmp_sensors[bmp_idx].bmp_model >= 3) {
          WSContentSend_PD(PSTR("{s}%s " D_GAS "{m}%s " D_UNIT_KILOOHM "{e}"), name, gas_resistance);
        }
#endif  // USE_BME68X

#endif  // USE_WEBSERVER
      }
    }
  }
}

#ifdef USE_DEEPSLEEP

void BMP_EnterSleep(void)
{
  if (DeepSleepEnabled()) {
    for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
      switch (bmp_sensors[bmp_idx].bmp_type) {
        case BMP180_CHIPID:
        case BMP280_CHIPID:
        case BME280_CHIPID:
          I2cWrite8(bmp_sensors[bmp_idx].bmp_address, BMP_REGISTER_RESET, BMP_CMND_RESET);
          break;
        default:
          break;
      }
    }
  }
}

#endif // USE_DEEPSLEEP


#endif
