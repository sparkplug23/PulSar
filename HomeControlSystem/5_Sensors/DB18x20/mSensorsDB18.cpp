/*
  mHVAC.cpp - HVAC Controller

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
#include "mSensorsDB18.h"

#ifdef USE_MODULE_SENSORS_DS18B20

const char* mSensorsDB18::PM_MODULE_SENSORS_DB18_CTR = D_MODULE_SENSORS_DB18S20_CTR;
const char* mSensorsDB18::PM_MODULE_SENSORS_DB18_FRIENDLY_CTR = D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;

int8_t mSensorsDB18::Tasker(uint8_t function, JsonParserObject obj){

  int8_t function_result = 0;
  
  // some functions must run regardless
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }
  
  // Only continue to remaining functions if sensor has been detected and enabled
  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:{

      // Pre_Init();
      // char buffer[100];
      // uint8_t ii = 5;
      // AddLog(LOG_LEVEL_TEST,PSTR("\n\r\n\rdb18 device name %d \"%s\""),ii,DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));
    
    
      // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor[sensor_count%d].id = %d"),0,GetCorrectedDeviceID(0)); 
      // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor[sensor_count%d].id = %d"),1,GetCorrectedDeviceID(1)); 

    }
    break;   
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
  
  return function_result;

}//end function


void mSensorsDB18::Pre_Init(){

  uint8_t sensor_count = 0;
  uint8_t sensor_group_count = 0;
  settings.nSensorsFound = 0;

  if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID)) {  // not set when 255
    sensor_group[sensor_group_count].pin = pCONT_pins->GetPin(GPIO_DSB_1OF2_ID);
    sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin 1 Valid %d"),sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas->begin();
    // Get sensors connected to this pin
    sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
    #ifdef ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT
    sensor_group[sensor_group_count].sensor_count = ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT;
    #endif
    //increment that we have another sensor group added IF we have sensors attached
    if(sensor_group[sensor_group_count].sensor_count){
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB1 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
      settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
      sensor_group_count++;
    }else{
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
    }
  }

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);

  if (pCONT_pins->PinUsed(GPIO_DSB_2OF2_ID)) {  // not set when 255
    sensor_group[sensor_group_count].pin = pCONT_pins->GetPin(GPIO_DSB_2OF2_ID);
    sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin 2 Valid %d"),sensor_group[sensor_group_count].pin);
    sensor_group[sensor_group_count].dallas->begin();
    // Get sensors connected to this pin
    sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
    #ifdef ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT
    sensor_group[sensor_group_count].sensor_count = ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT;
    #endif
    //increment that we have another sensor group added IF we have sensors attached
    if(sensor_group[sensor_group_count].sensor_count){
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB2 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
      settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
      sensor_group_count++;
    }else{
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
    }
  }

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);
  if(settings.nSensorsFound){
    settings.fEnableSensor = true;
    settings.group_count = sensor_group_count;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "settings.fEnableSensor, %d sensors"),settings.nSensorsFound);
  }

  // delay(2000);

}




void mSensorsDB18::Init(void){

  // AddLog(LOG_LEVEL_DEBUG,PSTR("mSensorsDB18::init"));

  // sensor group 1 exists
  uint8_t sensor_group_count = 0;
  uint8_t sensor_count = 0;
  uint8_t sensors_attached = 0;

  sensors_attached +=  sensor_group[sensor_group_count].sensor_count;
  if(sensors_attached>=DB18_SENSOR_MAX){
    sensor_group[sensor_group_count].sensor_count = DB18_SENSOR_MAX; // set limit
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DSB "DB18_SENSOR_MAX limit reached"));
  }

  // load sensor data into their own sensor struct GROUP
  for(uint8_t sensor_group_id=0;
              sensor_group_id<settings.group_count;
              sensor_group_id++
      ){
    // load each single sensor
    uint8_t group_sensor_found = sensor_group[sensor_group_id].sensor_count;

    if(group_sensor_found){
    
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "group_sensor_found=%d %d"),sensor_group_id,group_sensor_found);

      //if sensor limit, exit
      for(uint8_t sensor_id=0;
                  sensor_id<group_sensor_found;
                  sensor_id++
          ){
        // get sensor and add to list 
        
        if(sensor_group[sensor_group_id].dallas->getAddress(sensor[sensor_count].address,sensor_id)){  //what is this then, already stored?
          // Remember group this sensor came from 
          
          //AddLog_Array(LOG_LEVEL_INFO, "AFTER getAddress", sensor[sensor_count].address, (uint8_t)sizeof(sensor[sensor_count].address));

          sensor[sensor_count].sensor_group_id = sensor_group_id;
          // sensor[sensor_count].address_stored = ;

          // Set sensor precision      

          sensor_group[sensor_group_id].dallas->setResolution(sensor[sensor_count].address, TEMPERATURE_PRECISION);


          AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin Group %d, count %d, sensor count %d"),sensor_group_id,sensor_id,sensor_count);
          WDT_FEED();
          WDT_RESET();
          sensor_count++; // increment how many is found
          //limit if number of sensors is reached
          if(sensor_count>DB18_SENSOR_MAX){ break; }
        }else{
          AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "getAddress failed"));        
        }
      }//end for
    }
  }


  if(!sensor_count){    
    // AddLog(LOG_LEVEL_ERROR,PSTR("No sensor address found"));
    return;
  }

  // Print everything stored to test
  #ifdef SPLASH_ADDRESS_ON_INIT
    for(int i=0;i<DB18_SENSOR_MAX;i++){
      printAddress(sensor[i].address);
    }
  #endif

  db18_sensors_active = sensor_count;

}//end init



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
              if(sensor[sensor_id].reading.val != tmp_float){ 
                sensor[sensor_id].reading.ischanged = true; 
                anychanged=true;// check if updated
              }else{ 
                sensor[sensor_id].reading.ischanged = false; 
              }
              sensor[sensor_id].reading.val = tmp_float;
              sensor[sensor_id].reading.isvalid = true; 
              sensor[sensor_id].reading.captureupsecs = pCONT_time->uptime.seconds_nonreset;
              // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
              AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DB18 D_MEASURE " \"%s\" = [%d]"), DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DB18S20_ID, sensor[sensor_id].address_id, buffer, sizeof(buffer)),(int)tmp_float);
            }else{
              sensor[sensor_id].reading.isvalid = false;
              
              // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
              AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DB18 D_MEASURE " \"%s\" = " D_FAILED), DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DB18S20_ID, sensor[sensor_id].address_id, buffer, sizeof(buffer)));
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

  JsonBuilderI->Start();
  
  char buffer[40];
  char title [40];
  
  uint8_t corrected_sensor_id = 0;
  
  for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){ //db18_sensors_active
    
    if(sensor[sensor_id].reading.ischanged || (json_level<=JSON_LEVEL_IFCHANGED)){  

      JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DB18S20_ID,sensor[sensor_id].address_id,buffer,sizeof(buffer)));         
        JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].reading.val);
        JsonBuilderI->Add(D_JSON_ISVALID, sensor[sensor_id].reading.isvalid);
        JsonBuilderI->Add(D_JSON_CAPTURE_UPSECONDS, sensor[corrected_sensor_id].reading.captureupsecs);

        // if(json_level <= JSON_LEVEL_DEBUG){
          JsonBuilderI->Add(D_JSON_ADDRESS, sensor[sensor_id].address[7]);
        //   JsonBuilderI->Add("ID", sensor_id);
        //   JsonBuilderI->Add("Corrected_ID", corrected_sensor_id);
        // }
      JsonBuilderI->Level_End();  
    }

  } // END for
    
  return JsonBuilderI->End();

}

uint8_t mSensorsDB18::ConstructJSON_Settings(uint8_t json_level){

  char buffer[20];

  JsonBuilderI->Start();
  
  JsonBuilderI->Add("found", db18_sensors_active);

  JsonBuilderI->Level_Start("Address");

    for(int id=0;id<settings.nSensorsFound;id++){
      snprintf(buffer, sizeof(buffer), "sens%d_%d_%d", id, sensor[id].address[6], sensor[id].address[7]);
      JsonBuilderI->Array_AddArray(buffer, &sensor[id].address[0], 8);
    }


JBI->Add("pin0", sensor_group[0].pin);
JBI->Add("pin1", sensor_group[1].pin);

JBI->Add("pindb0", pCONT_pins->GetPin(GPIO_DSB_1OF2_ID));
JBI->Add("pindb1", pCONT_pins->GetPin(GPIO_DSB_2OF2_ID));

JBI->Add("pindb0PinUsed", pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID));
JBI->Add("pindb1PinUsed", pCONT_pins->PinUsed(GPIO_DSB_2OF2_ID));

// JBI->Add("count1", sensor_group[0].dallas->getDeviceCount());
// JBI->Add("count2", sensor_group[1].dallas->getDeviceCount());




  JsonBuilderI->Level_End();


  //   root["rate_measure_ms"] = settings.rate_measure_ms;
  //   // root["tRateSecs"] = mqtthandler_sensor_ifchanged.tRateSecs;
    
  //   root["sens_tele_rate"] =mqtthandler_sensor_teleperiod.tRateSecs;
  //   root["sens_ifchanged_rate"] =mqtthandler_sensor_ifchanged.tRateSecs;
  //   root["sett_tele_rate"] =mqtthandler_settings_teleperiod.tRateSecs;

  //   //Add here - function that takes handler list and appends its config here by name 


  return JsonBuilderI->End();


}

int8_t mSensorsDB18::FindStructIndexByAddressID(int8_t address_id){

  int8_t struct_index_id = -1;

  if(address_id == -1){ return -1; } // invalid/unset id

  for(uint8_t ii=0;ii<DB18_SENSOR_MAX;ii++){
    if(address_id == sensor[ii].address_id){
      return ii;
    }
  }

  return -1;

}


// function to print a device address
void mSensorsDB18::printAddress(DeviceAddress deviceAddress, int8_t index){
  for (uint8_t i = 0; i < 8; i++){// zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  
  Serial.print("\tdec: ");
  for (uint8_t i = 0; i < 8; i++){// zero pad the address if necessary
    Serial.print(deviceAddress[i]);
    if(i<7){Serial.print(",");}
  }
  Serial.println();
  // AddLog(LOG_LEVEL_DEBUG,PSTR("printAddress %X:%X:%X:%X:%X:%X:%X:%X"),
  //   deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
  //   deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);
}

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mSensorsDB18::WebAppend_Root_Status_Table_Draw(){

  char buffer[100];

  for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){ //add number in name? List needed? also hold user defined name?
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      JsonBuilderI->Append_P(PSTR("<td>DB18 %02d Temperature %s</td>"),sensor_id,DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DB18S20_ID, sensor[sensor_id].address_id, buffer, sizeof(buffer)));
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_db18","?");   
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  }

}

//append to internal buffer if any root messages table
void mSensorsDB18::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  
  JsonBuilderI->Array_Start("tab_db18");// Class name
  uint8_t corrected_sensor_id = 0;

  for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){
    
    char colour_ctr[10];
    char table_row[25]; memset(table_row,0,sizeof(table_row));     

    char value_ctr[8];
    
    // corrected_sensor_id = GetCorrectedDeviceID(row);
    

    pCONT_sup->dtostrfd(sensor[sensor_id].reading.val,2,value_ctr);

    sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
    
    if(sensor[sensor_id].reading.val<=25){
      sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
    }else
    if(sensor[sensor_id].reading.val<30){ //warm
      sprintf(colour_ctr,"%s","#fcba03");
    }else
    if(sensor[sensor_id].reading.val>=30){ //hot
      sprintf(colour_ctr,"%s","#ff0000");
    }else{
      sprintf(colour_ctr,"%s","#ffffff");
    }

    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);
      JsonBuilderI->Add("ih",table_row);
      JsonBuilderI->Add("fc",colour_ctr);
    JsonBuilderI->Level_End();
  
  }

  JsonBuilderI->Array_End();

}
#endif // USE_MODULE_NETWORK_WEBSERVER



void mSensorsDB18::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // Using a desired address, the sensor is searched for, then index (id) is updated
  if(jtok = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR]){
     
    JsonParserArray array_group = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR];
      
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_JSON_SENSORADDRESS),array_group.size()); 
    #endif // LOG_LEVEL_COMMANDS
    
    uint8_t address_temp[8];
    uint8_t address_index = 0;
    uint8_t original_device_id = 0;
    
    for(auto group_iter : array_group) {

      JsonParserArray array_sensor_address_iter = group_iter;
      memset(address_temp,0,sizeof(address_temp));
      address_index = 0;
            
      for(auto address_id : array_sensor_address_iter) {
        int address = address_id.getInt();
        // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
        //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
        // #endif
        address_temp[address_index++] = address;
        // if(address_index>7){ break; } //error!
      }

      AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)8);
      SetIDWithAddress(original_device_id++, address_temp);
      Serial.println();

    }
    
  }

}


// Search for address, if found, store id against it in struct
// Assumes template load AFTER init of sensors
void mSensorsDB18::SetIDWithAddress(uint8_t address_id, uint8_t* address_to_find){

  // memcpy(sensor[device_id].address_stored,address_to_find,sizeof(sensor[device_id].address_stored));


  uint8_t sensor_count = 0; // reset
  // Address moved into struct, I need to rearrange now with ids

  
        AddLog(LOG_LEVEL_INFO, "searching start %d",settings.group_count);


  for(uint8_t sensor_group_id=0; sensor_group_id<settings.group_count; sensor_group_id++){
        AddLog(LOG_LEVEL_INFO, "searching iter %d",sensor_group_id);
    for(uint8_t sensor_id=0; sensor_id<sensor_group[sensor_group_id].sensor_count; sensor_id++){
      // Check address has been set    
      
      if(memcmp(sensor[sensor_count].address,address_to_find,sizeof(sensor[sensor_count].address))==0){ // 0 means equal


//temp fix
/**
 * Issue#1 address_id stored in sensor struct complicates retrieving devicename from module_id, 
 * Fix1: Remove address_id, instead making it the struct index, thus reordered contents of struct is required (maybe using address to poll sensor that is stored in struct, ie named sensor X, in index X, uses this address... if not, just append address as new struct indexes)
 * This means, on setting name, I should search for the address of X and put it into index X.. swap?
 * */
        sensor[sensor_count].address_id = address_id;   
        // sensor[original_device_id].id = sensor_count;    
      
      
        AddLog_Array(LOG_LEVEL_INFO, "isconnected", sensor[sensor_count].address, (uint8_t)sizeof(sensor[sensor_count].address));
        AddLog(LOG_LEVEL_TEST, PSTR("Searched %02d, Found %02d, Id from %d to %d %d"),
          address_to_find[7],
          sensor[sensor_count].address[7],
          sensor_count,
          address_id   ,
          sensor[address_id].address_id     
        );

        break; // stop looking for more


      }
      else{
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "getAddress failed - no find with search %d"),sensor[sensor_count].address[7]);   
      }
      sensor_count++;
    }
  }

}


void mSensorsDB18::EveryLoop(){
  if(mTime::TimeReachedNonReset(&tSavedMeasureSensor,settings.rate_measure_ms)){
    
    // AddLog(LOG_LEVEL_DEBUG,PSTR("mSensorsDB18::here2 %d %d %d"),db18_sensors_active, settings.rate_measure_ms, settings.group_count);

    if(!db18_sensors_active){ // Retry init if failed
      Init(); //search again
      // set a cooldown period if sensor was not found of X seconds
      if(db18_sensors_active==0){ //still no sensor found, wait
        tSavedMeasureSensor = millis()+10000; //30 seocnds backoff
      }
    }else{
      // AddLog(LOG_LEVEL_DEBUG,PSTR("SplitTask_UpdateSensors"));
        for(uint8_t sensor_group_id=0;
                  sensor_group_id<settings.group_count;
                  sensor_group_id++
          ){
        SplitTask_UpdateSensors(sensor_group_id, REQUIRE_COMPLETE); // request & measure
        if(sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
          sReadSensor=SPLIT_TASK_SUCCESS_ID;//temp way -- need another case  SUCCESS
          tSavedMeasureSensor = millis();//Serial.print(F("[tPip-"));//update time every 60 seconds
        }
      }
    }
  }

}//end




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSensorsDB18::MQTTHandler_Init(){

  struct handler<mSensorsDB18>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true; // DEBUG CHANGE
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  #ifdef DEVICE_IMMERSIONSENSOR //temp fix
  mqtthandler_ptr->tRateSecs = 1; 
  #endif
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsDB18::ConstructJSON_Sensor;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsDB18::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsDB18::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSensorsDB18::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_DB18S20_ID, handle, id);
  }
}

#endif
