#include "mSensorsDB18.h"

#ifdef USE_MODULE_SENSORS_DS18B20


void mSensorsDB18::Pre_Init(){

  uint8_t sensor_count = 0;
  uint8_t sensor_group_count = 0;
  settings.nSensorsFound = 0;

  if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID)) {  // not set when 255
    sensor_group[sensor_group_count].pin = pCONT_set->pin[GPIO_DSB_1OF2_ID];
    sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin 1 Valid %d"),sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas->begin();
    // Get sensors connected to this pin
    sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
    //increment that we have another sensor group added IF we have sensors attached
    if(sensor_group[sensor_group_count].sensor_count){
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB1 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
      settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
      sensor_group_count++;
    }else{
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
    }
  }

  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);

  if (pCONT_pins->PinUsed(GPIO_DSB_2OF2_ID)) {  // not set when 255
    sensor_group[sensor_group_count].pin = pCONT_set->pin[GPIO_DSB_2OF2_ID];
    sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin 2 Valid %d"),sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas->begin();
    // Get sensors connected to this pin
    sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
    //increment that we have another sensor group added IF we have sensors attached
    if(sensor_group[sensor_group_count].sensor_count){
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB2 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
      settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
      sensor_group_count++;
    }else{
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
    }
  }

      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);
  if(settings.nSensorsFound){
    settings.fEnableSensor = true;
    settings.group_count = sensor_group_count;
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "settings.fEnableSensor, %d sensors"),settings.nSensorsFound);
  }

}

// need new value for group count as it is reset

void mSensorsDB18::Init(void){

  // AddLog_P(LOG_LEVEL_DEBUG,PSTR("mSensorsDB18::init"));

  // sensor group 1 exists
  uint8_t sensor_group_count = 0;
  uint8_t sensor_count = 0;
  uint8_t sensors_attached = 0;

  // // limit if needed, 
  // for(int ii=0;ii<DB18_SENSOR_MAX;ii++){
  //   sensors_attached +=  sensor_group[sensor_group_count].sensor_count;
  // }

  sensors_attached +=  sensor_group[sensor_group_count].sensor_count;
  if(sensors_attached>=DB18_SENSOR_MAX){
    sensor_group[sensor_group_count].sensor_count = DB18_SENSOR_MAX; // set limit
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_DSB "DB18_SENSOR_MAX limit reached"));
  }

  // load sensor data into their own sensor struct GROUP
  for(uint8_t sensor_group_id=0;
              sensor_group_id<settings.group_count;
              sensor_group_id++
      ){
    // load each single sensor
    uint8_t group_sensor_found = sensor_group[sensor_group_id].sensor_count;

    if(group_sensor_found){
    
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "group_sensor_found=%d %d"),sensor_group_id,group_sensor_found);

      //if sensor limit, exit
      for(uint8_t sensor_id=0;
                  sensor_id<group_sensor_found;
                  sensor_id++
          ){
        // get sensor and add to list

        if(sensor_group[sensor_group_id].dallas->getAddress(sensor[sensor_count].address,sensor_id)){
          // Remember group this sensor came from 
          sensor[sensor_count].sensor_group_id = sensor_group_id;
          // Set sensor precision      
          // sensor_group[sensor_group_id].dallas->setResolution(sensor[sensor_count].address, TEMPERATURE_PRECISION);
          AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin Group %d, count %d, sensor count %d"),sensor_group_id,sensor_id,sensor_count);
          sensor_count++; // increment how many is found
          //limit if number of sensors is reached
          if(sensor_count>DB18_SENSOR_MAX){ break; }
        }else{
          AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "getAddress failed"));        
        }
      }//end for
    }
  }

  if(!sensor_count){    

    //AddLog_P(LOG_LEVEL_ERROR,PSTR("No sensor address found"));
    return;
  }

  // Print everything stored to test
  #ifdef SPLASH_ADDRESS_ON_INIT
    for(int i=0;i<DB18_SENSOR_MAX;i++){
      printAddress(sensor[i].address);
    }
  #endif

  db18_sensors_active = sensor_count;

  
  // uint8_t buffer_length = 0;
  // memset(name_buffer,0,sizeof(name_buffer));
  // for(int ii=0;ii<db18_sensors_active;ii++){
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s%d|","test",ii);
  // }


  // // Clear names to be overwritten with null termination
  // for(int i=0;i<sensor_count;i++){
  //   memset(sensor[i].name.ctr,0,sizeof(sensor[i].name.ctr));
  // }

  // // Search if they are defined, if so, add name
  // for(int i=0;i<db18_sensors_active;i++){
  //   #ifdef SENSOR1_NAME
  //     if(memcmp(sensor[i].address,ADDRESS_S1,sizeof(ADDRESS_S1))==0){ // 0 means equal
  //       memcpy(sensor[i].name.ctr,SENSOR1_NAME,sizeof(SENSOR1_NAME));
  //     }
  //   #endif
  //   #ifdef SENSOR2_NAME
  //     if(memcmp(sensor[i].address,ADDRESS_S2,sizeof(ADDRESS_S2))==0){ // 0 means equal
  //       memcpy(sensor[i].name.ctr,SENSOR2_NAME,sizeof(SENSOR2_NAME));
  //     }
  //   #endif
  //   #ifdef SENSOR3_NAME
  //     if(memcmp(sensor[i].address,ADDRESS_S3,sizeof(ADDRESS_S3))==0){ // 0 means equal
  //       memcpy(sensor[i].name.ctr,SENSOR3_NAME,sizeof(SENSOR3_NAME));
  //     }
  //   #endif
  //   #ifdef SENSOR4_NAME
  //     if(memcmp(sensor[i].address,ADDRESS_S4,sizeof(ADDRESS_S4))==0){ // 0 means equal
  //       memcpy(sensor[i].name.ctr,SENSOR4_NAME,sizeof(SENSOR4_NAME));
  //     }
  //   #endif
  //   #ifdef SENSOR5_NAME
  //     if(memcmp(sensor[i].address,ADDRESS_S5,sizeof(ADDRESS_S5))==0){ // 0 means equal
  //       memcpy(sensor[i].name.ctr,SENSOR5_NAME,sizeof(SENSOR5_NAME));
  //     }
  //   #endif
  // }

  // //Make this work with malloc?^^^^^

  // Fill with generic name if it was not already declared
  // for(int i=0;i<db18_sensors_active;i++){
  //   if(sensor[i].name.ctr[0] == 0){
  //     sprintf(sensor[i].name.ctr,"sens%d",i);
  //   }
  // }

}//end init

//return -1 if not found
int8_t mSensorsDB18::getIDbyName(const char* tofind){

  // for(int i=0;i<db18_sensors_active;i++){
  //   if(strstr(sensor[i].name.ctr,tofind)){
  //     return i;
  //   }
  // }

  return -1; // if we get this far :: error

}


void mSensorsDB18::SplitTask_UpdateSensors(uint8_t sensor_group_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{
    
    switch(sReadSensor){
      case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
      case SPLIT_TASK_TIMEOUT_ID: default:
      case SPLIT_TASK_SEC1_ID:

        sensor_group[sensor_group_id].dallas->requestTemperatures();
        sReadSensor = SPLIT_TASK_SEC2_ID;
        break;
      case SPLIT_TASK_SEC2_ID:{
        char buffer[50];
        anychanged=false;

        // Check all sensors, if they belong to this group, get their temp
        for(int sensor_id=0;
                sensor_id<db18_sensors_active;
                sensor_id++){

          if(sensor[sensor_id].sensor_group_id == sensor_group_id){

            if((tmp_float = sensor_group[sensor_group_id].dallas->getTempC(sensor[sensor_id].address))!=DEVICE_DISCONNECTED_C){
              if(sensor[sensor_id].reading.val != tmp_float){ sensor[sensor_id].reading.ischanged = true; anychanged=true;// check if updated
              }else{ sensor[sensor_id].reading.ischanged = false; }
              sensor[sensor_id].reading.val = tmp_float;
              sensor[sensor_id].reading.isvalid = true; 
              sensor[sensor_id].reading.captureupsecs = pCONT->mt->uptime.seconds_nonreset;
              // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
              AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DB18 D_MEASURE " \"%s\" = [%d]"), pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)),(int)tmp_float);
            }else{
              sensor[sensor_id].reading.isvalid = false;
              
              // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
              AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DB18 D_MEASURE " \"%s\" = " D_FAILED), pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));
            }
          
          } // end if
        }//end for
        sReadSensor = SPLIT_TASK_DONE_ID;
        require_completion = DONE_COMPLETE;
      }break;
    } // end switch

    if(require_completion){ delay(1); }//delay required if we are going to do multiple calls

    if(abs(millis()-timeout)>=2000){
      sReadSensor = SPLIT_TASK_TIMEOUT_ID;
      break;
    }

  }while(require_completion); // loops once even if false

}//end function




//pubsub.ppublish("sensors/pipe","{\"ds\":{\"temp\":\"1\"},\"us\":{\"temp\":\"2\"},\"wb\":{\"temp\":\"3\"},\"ih\":{\"temp\":\"4\"},\"tt\":{\"temp\":\"5\"},\"tb\":{\"temp\":\"6\"}}"));
// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mSensorsDB18::ConstructJSON_Sensor(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  data_buffer2.payload.len += 
  pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr+data_buffer2.payload.len,
    PSTR("{")
  );

  char name_tmp[25];
  char float_tmp[25];
  
  for(int i=0;i<db18_sensors_active;i++){
    
    // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), i, name_buffer);
    dtostrf(sensor[i].reading.val,5,pCONT_set->Settings.sensors.flags.decimal_precision,float_tmp);

    data_buffer2.payload.len += 
    pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr+data_buffer2.payload.len,
      PSTR(
        "\"%s\":{"
          "\"" D_JSON_TEMP    "\":%s,"
          "\"" D_JSON_ISVALID "\":%d"
        "},"
      ), 
      name_tmp,
      float_tmp, 
      sensor[i].reading.isvalid
    );
    
  }

  data_buffer2.payload.len += 
  pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr+data_buffer2.payload.len-1, //remove comma
    PSTR("}")
  );

  return data_buffer2.payload.len;

}

uint8_t mSensorsDB18::ConstructJSON_Settings(uint8_t json_level){

  // StaticJsonDocument<1000> doc;
  // JsonObject root = doc.to<JsonObject>();
  // memset(&data_buffer2,0,sizeof(data_buffer2));

  
  JsonBuilderI->Start();

  // AppendJSON_Value3(D_JSON_CHANNELS_COUNT, (uint8_t)123);
  // AppendJSON_Value3(D_JSON_CHANNELS_COUNT, (float)123.45, 2);

  // uint8_t tmp_arr[10] = {0,1,2,3,4,5,6,7,8,9};  
  // pCONT_sup->AppendJSON_Array(D_JSON_CHANNELS_COUNT, tmp_arr, sizeof(tmp_arr));

  // float tmp_arr2[10] = {0.1,1.2,2.3,3.4,4.5,5,6,7,8,9};
  // pCONT_sup->AppendJSON_Array(D_JSON_CHANNELS_COUNT, tmp_arr2, sizeof(tmp_arr2)/4, 2);


  //   root["sensor_db18_memory"] = name_buffer;

  JsonBuilderI->Add("found", db18_sensors_active);


  //   JsonObject addressobj = root.createNestedObject("address");
  //   for(int i=0;i<db18_sensors_active;i++){
  //     char tmpctr[40];  memset(tmpctr,0,sizeof(tmpctr));
  //     sprintf(tmpctr,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",sensor[i].address[0],sensor[i].address[1],sensor[i].address[2],sensor[i].address[3],sensor[i].address[4],sensor[i].address[5],sensor[i].address[6],sensor[i].address[7]);

  //     char namectr[15];  memset(namectr,0,sizeof(namectr));
  //     itoa(i,namectr,10);
  //     addressobj[namectr] = tmpctr;
  //   }

  //   #ifdef SENSOR1_NAME
  //     JsonObject predefinedobj = root.createNestedObject("predefined");
  //     char tmpctr[40];  memset(tmpctr,0,sizeof(tmpctr));
  //     sprintf(tmpctr,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",ADDRESS_S1[0],ADDRESS_S1[1],ADDRESS_S1[2],ADDRESS_S1[3],ADDRESS_S1[4],ADDRESS_S1[5],ADDRESS_S1[6],ADDRESS_S1[7]);
  //     predefinedobj[SENSOR1_NAME] = tmpctr;
  //   #endif
  //   #ifdef SENSOR2_NAME
  //     char tmpctr2[40];  memset(tmpctr2,0,sizeof(tmpctr2));
  //     sprintf(tmpctr2,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",ADDRESS_S2[0],ADDRESS_S2[1],ADDRESS_S2[2],ADDRESS_S2[3],ADDRESS_S2[4],ADDRESS_S2[5],ADDRESS_S2[6],ADDRESS_S2[7]);
  //     predefinedobj[SENSOR2_NAME] = tmpctr2;
  //   #endif
  //   #ifdef SENSOR3_NAME
  //     char tmpctr3[40];  memset(tmpctr3,0,sizeof(tmpctr3));
  //     sprintf(tmpctr3,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",ADDRESS_S3[0],ADDRESS_S3[1],ADDRESS_S3[2],ADDRESS_S3[3],ADDRESS_S3[4],ADDRESS_S3[5],ADDRESS_S3[6],ADDRESS_S3[7]);
  //     predefinedobj[SENSOR3_NAME] = tmpctr3;
  //   #endif
  //   #ifdef SENSOR4_NAME
  //     char tmpctr4[40];  memset(tmpctr4,0,sizeof(tmpctr4));
  //     sprintf(tmpctr4,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",ADDRESS_S4[0],ADDRESS_S4[1],ADDRESS_S4[2],ADDRESS_S4[3],ADDRESS_S4[4],ADDRESS_S4[5],ADDRESS_S4[6],ADDRESS_S4[7]);
  //     predefinedobj[SENSOR4_NAME] = tmpctr4;
  //   #endif
  //   #ifdef SENSOR5_NAME
  //     char tmpctr5[40];  memset(tmpctr5,0,sizeof(tmpctr5));
  //     sprintf(tmpctr5,"%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X",ADDRESS_S5[0],ADDRESS_S5[1],ADDRESS_S5[2],ADDRESS_S5[3],ADDRESS_S5[4],ADDRESS_S5[5],ADDRESS_S5[6],ADDRESS_S5[7]);
  //     predefinedobj[SENSOR5_NAME] = tmpctr5;
  //   #endif

  //   //report mqtt config
  //   root["rate_measure_ms"] = settings.rate_measure_ms;
  //   // root["tRateSecs"] = mqtthandler_sensor_ifchanged.tRateSecs;
    
  //   root["sens_tele_rate"] =mqtthandler_sensor_teleperiod.tRateSecs;
  //   root["sens_ifchanged_rate"] =mqtthandler_sensor_ifchanged.tRateSecs;
  //   root["sett_tele_rate"] =mqtthandler_settings_teleperiod.tRateSecs;

  //   //Add here - function that takes handler list and appends its config here by name 


  return JsonBuilderI->End();


}


// function to print a device address
void mSensorsDB18::printAddress(DeviceAddress deviceAddress, int8_t index){
  for (uint8_t i = 0; i < 8; i++){// zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.println();
  // AddLog_P(LOG_LEVEL_DEBUG,PSTR("printAddress %X:%X:%X:%X:%X:%X:%X:%X"),
  //   deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
  //   deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);
}

// function to print a device's resolution
// void mSensorsDB18::printResolution(DallasTemperature* sensors, DeviceAddress deviceAddress){
//   Serial.print("Resolution: ");
//   Serial.println(sensors->getResolution(deviceAddress));
// }



void mSensorsDB18::WebAppend_Root_Status_Table_Draw(){

  char buffer[100];

  for(int ii=0;ii<db18_sensors_active;ii++){ //add number in name? List needed? also hold user defined name?
    JsonBuilderI->Append_P(PSTR("<tr>"));

    // char name_buffer_tmp[25];
    // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

    



      JsonBuilderI->Append_P(PSTR("<td>DB18 %02d Temperature %s</td>"),ii,pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
      AddLog_P(LOG_LEVEL_INFO,PSTR("buffer=%s"),buffer);
      JsonBuilderI->Append_P(PSTR("<td><div class='%s'>%s</div></td>"),"tab_db18","?");   
    JsonBuilderI->Append_P(PSTR("</tr>"));
  }

}


//append to internal buffer if any root messages table
void mSensorsDB18::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;

  // JsonBuilderI->Append_P(PSTR("\"%s\":["),PSTR("tab_db18")); 
  
  JsonBuilderI->Array_Start("tab_db18");// Class name

  for(int row=0;row<db18_sensors_active;row++){
    
    char float_ctr[10];
    char colour_ctr[10];
    char table_row[25]; memset(table_row,0,sizeof(table_row));     

    char value_ctr[8];
    pCONT_sup->dtostrfd(sensor[row].reading.val,2,value_ctr);

    sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
    
    if(sensor[row].reading.val<=25){
      sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
    }else
    if(sensor[row].reading.val<30){ //warm
      sprintf(colour_ctr,"%s","#fcba03");
    }else
    if(sensor[row].reading.val>=30){ //hot
      sprintf(colour_ctr,"%s","#ff0000");
    }else{
      sprintf(colour_ctr,"%s","#ffffff");
    }

    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      JsonBuilderI->Add("ih",table_row);
      JsonBuilderI->Add("fc",colour_ctr);
    JsonBuilderI->Level_End();

    // JsonBuilderI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
    //   table_row, colour_ctr
    // );
  
  }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // JsonBuilderI->Append_P(PSTR("],")); 

  
  JsonBuilderI->Array_End();

}


int8_t mSensorsDB18::Tasker(uint8_t function){

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
      Init();
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
    case FUNC_EVERY_SECOND:{
      // char buffer[100];
      // uint8_t ii = 5;
      // AddLog_P(LOG_LEVEL_TEST,PSTR("\n\r\n\rdb18 device name %d \"%s\""),ii,pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));
      // ii = 6;
      // AddLog_P(LOG_LEVEL_TEST,PSTR("\n\r\n\rdb18 device name %d \"%s\""),ii,pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));



    }
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
  }
  
  return function_result;

}//end function



void mSensorsDB18::EveryLoop(){
  if(mSupport::TimeReachedNonReset(&tSavedMeasurePipe,settings.rate_measure_ms)){
    
    AddLog_P(LOG_LEVEL_DEBUG,PSTR("mSensorsDB18::here2 %d %d %d"),db18_sensors_active, settings.rate_measure_ms, settings.group_count);

    if(!db18_sensors_active){ // Retry init if failed
      Init(); //search again
    }else{
      

      // AddLog_P(LOG_LEVEL_DEBUG,PSTR("SplitTask_UpdateSensors"));
        for(uint8_t sensor_group_id=0;
                  sensor_group_id<settings.group_count;
                  sensor_group_id++
          ){
        SplitTask_UpdateSensors(sensor_group_id, REQUIRE_COMPLETE); // request & measure
        if(sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
          sReadSensor=SPLIT_TASK_SUCCESS_ID;//temp way -- need another case  SUCCESS
          tSavedMeasurePipe = millis();//Serial.print(F("[tPip-"));//update time every 60 seconds
        }
      }
    }
  }

}//end




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mSensorsDB18::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true; // DEBUG CHANGE
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSensorsDB18::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mSensorsDB18::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSensorsDB18::MQTTHandler_Sender(uint8_t mqtt_handler_id){

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
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_DB18S20_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////














#endif
