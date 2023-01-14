/*
  mADCInternal.cpp
  Description: Uses the internal ADC for measurements of A0 on ESP8266, and all ESP32 channels

  Due to the significant changes between esp82/32, for develop, the esp32 will be develop and optimised as its own module. 
  This will allow esp8266 simplier hardware to be optimised, mapping and vectoring to be optimised without the complexity of interrupts and potentially DMA (though esp32 dma will likely be its optimised version for audio sampling)

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

/**
 * Special method, which will use interrupt on pin change, to cause a new adc value recording
 * Test 1:
 *  - use physical manual pin change, to cause a adc reading from both adc_pins and display result on serial
 * 
 * 
 * */
#include "mADCInternal.h"

#ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266

const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_CTR = D_MODULE_SENSORS_ADC_INTERNAL_CTR;
const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR = D_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR;

int8_t mADCInternal::Tasker(uint8_t function, JsonParserObject obj)
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
    {

    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
    //  parse_JSONCommand(obj);
    break;
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


void mADCInternal::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;


  settings.fEnableSensor = 1; // forced for now




  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }

}


void mADCInternal::Init(void){


}

void mADCInternal::EveryLoop()
{

  // Serial.println(analogRead(0));
    

}


void mADCInternal::Update_Channel1_ADC_Readings()
{

// #ifdef ESP32
//   #ifdef ENABLE_SMOOTHING_ON_ADC_READING
//   for(int i = 0;i<settings.fSensorCount; i++)
//   {
//     samples.clear();

//     // collect samples from ADC
//     while(samples.size() < readings[i].adcSampleCount_){
//       samples.push_back(adc1_get_raw((adc1_channel_t)adc_config[i].channel_id));
//       ets_delay_us(1); // too long!?
//     }
//     // average the collected samples
//     readings[i].adc_level = (std::accumulate(samples.begin(), samples.end(), 0) / samples.size());
//     // AddLog(LOG_LEVEL_TEST, PSTR("readings[%d].adc_level = %d"),i,readings[i].adc_level);

//     readings[i].samples_between_resets++;

//     // Add nice function that adds reading into memory
//     if(readings[i].stored_values.index < STORED_VALUE_ADC_MAX){
//       readings[i].stored_values.adc[readings[i].stored_values.index++] = readings[i].adc_level;
//     }else{
//       // AddLog(LOG_LEVEL_TEST, PSTR("readings[%d].stored_values.index = %d OVERFLOW"),i,readings[i].stored_values.index);
//       // readings[i].stored_values.index = 0;
//     }

//   }
//   #else
//     readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
//     ets_delay_us(1);
//     readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);
//     AddLog(LOG_LEVEL_TEST, PSTR("adc_level = \t%d\t%d"),readings[0].adc_level,readings[1].adc_level);
//   #endif

// #endif // ESP32




}


uint8_t mADCInternal::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mADCInternal::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();


    JBI->Add("raw", analogRead(0));

  char buffer[50];

  
    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // AddLog(LOG_LEVEL_TEST, PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );

    // Update_Channel1_ADC_Readings();
    
  // JBI->Array_Start("chADC1");
  // for(int i=0;i<2;i++){
  //   JBI->Add(readings[i].adc_level);
  // }
  // JBI->Array_End();

  // // JBI->Array_Start("ADC1");
  // // for(int i=0;i<8;i++){
  // //   JBI->Add(adc1_get_raw((adc1_channel_t)i));
  // // }
  // // JBI->Array_End();

  // JBI->Array_Start("stored_values.index");
  // for(int i=0;i<2;i++){
  //   JBI->Add(readings[0].stored_values.index);
  // }
  // JBI->Array_End();
  // JBI->Array_Start("samples_between_resets");
  // for(int i=0;i<2;i++){
  //   JBI->Add(readings[i].samples_between_resets);
  // }
  // JBI->Array_End();

  // uint16_t send_size = 0;
  // send_size = 10; //STORED_VALUE_ADC_MAX

  // JBI->Array_Start("adc0");
  // for(int i=0;i<send_size;i++){
  //   JBI->Add(readings[0].stored_values.adc[i]);
  // }
  // JBI->Array_End();



  // JBI->Array_Start("adc1");
  // for(int i=0;i<send_size;i++){
  //   JBI->Add(readings[1].stored_values.adc[i]);
  // }
  // JBI->Array_End();


  


  
  return JsonBuilderI->End();

}


#ifdef USE_MODULE_NETWORK_MQTT

void mADCInternal::MQTTHandler_Init(){

  struct handler<mADCInternal>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mADCInternal::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mADCInternal::MQTTHandler_Set_TelePeriod()
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
void mADCInternal::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_ADC_INTERNAL_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif





    // readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
    // ets_delay_us(1);
    // readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);



    // AddLog(LOG_LEVEL_TEST, PSTR("adc_level = \t%d\t%d"),readings[0].adc_level,readings[1].adc_level);


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // int val = adc1_get_raw(ADC1_CHANNEL_4);
    // printf("4val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_7);
    // printf("7val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_6);
    // printf("6val%d\n", val );

    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // AddLog(LOG_LEVEL_TEST, PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );


    //   int read_raw;
    //   adc2_config_channel_atten( ADC2_CHANNEL_6, ADC_ATTEN_0db );

    //   esp_err_t r = adc2_get_raw( ADC2_CHANNEL_6, ADC_WIDTH_12Bit, &read_raw);
    //   if ( r == ESP_OK ) {
    //       printf("%d\n", read_raw );
    //   } else if ( r == ESP_ERR_TIMEOUT ) {
    //       printf("ADC2 used by Wi-Fi.\n");
    //   }

// #include "mSensorsAnalog.h"

// #ifdef USE_MODULE_SENSORS_ANALOG

// // Make basic class, report adc via mqtt (for tree level)

// const char* mSensorsAnalog::PM_MODULE_SENSORS_ANALOG_CTR = D_MODULE_SENSORS_ANALOG_CTR;
// const char* mSensorsAnalog::PM_MODULE_SENSORS_ANALOG_FRIENDLY_CTR = D_MODULE_SENSORS_ANALOG_FRIENDLY_CTR;



// void mSensorsAnalog::Pre_Init(void){

//   // sensors_active = 0;

//   // // Using ++ means always the lowest index will be the active sensor, ie, PIR2 can be 0 when PIR1 is not defined
//   // if (pCONT_pins->GetPin(GPIO_PIR_1OF2_ID] < 99) {  // not set when 255
//   //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_1OF2_ID];
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR1] %d"),pCONT_pins->GetPin(GPIO_PIR_1OF2_ID]);
//   // }
//   // if (pCONT_pins->GetPin(GPIO_PIR_2OF2_ID] < 99) {  // not set when 255
//   //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_2OF2_ID];
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR2] %d"),pCONT_pins->GetPin(GPIO_PIR_2OF2_ID]);
//   // }

//   // fEnableSensor = false;

//   // for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
//   //   if(pin[sensor_id]>=0){
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "Pin Set %d"),pin[sensor_id]);
//   //     pinMode(pin[sensor_id],INPUT_PULLUP);
//   //     fEnableSensor = true; // true if any are active
//   //   }else{
//   //     AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin[sensor_id]);
//   //   }
//   // }

//   pinMode(A0, INPUT);

// }


// void mSensorsAnalog::Init(){

  
// }


// void mSensorsAnalog::EveryLoop(){
  

//   // Use short timer to automatically clear event
//   //if > 1 sec
//   //clear struct
// }

// int8_t mSensorsAnalog::Tasker(uint8_t function, JsonParserObject obj){

//   // Run even when sensor is disabled (Will set fEnableSensor)
//   switch(function){
//     case FUNC_PRE_INIT:
//       // Pre_Init();
//     break;
//   }
  
//   // if(!fEnableSensor){ 
//   //   //Serial.println("!fEnableSensor"); 
//   // return 0; }

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case FUNC_INIT:
//       // Init();
//     break;
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case FUNC_LOOP:
//     // Serial.println("FUNC_LOOP");
//       // EveryLoop();
//     break;   
//     case FUNC_EVERY_SECOND:
//       // AddLog(LOG_LEVEL_TEST, PSTR("mSensorsAnalog"));
//     break;  
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     // case FUNC_JSON_COMMAND_ID:
//     //   parse_JSONCommand(obj);
//     // break;
//     // case FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD:
//     //   // parsesub_JSONCommands();
//     // break;  
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case FUNC_MQTT_HANDLERS_INIT:
//     case FUNC_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init();
//     break;
//     case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
//       MQTTHandler_Set_TelePeriod();
//     break;
//     case FUNC_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//   }

// } // END function




// // //relay:1,[1,2],"all",["fountain,bulbs,led"]
// // void mSensorsAnalog::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
// //   // Check if instruction is for me
// //   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/motion")>=0){
// //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //   }else{
// //     return; // not meant for here
// //   }

// //   // AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

// //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer.payload.ctr);

// //   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
// //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //   JsonObject obj = doc.as<JsonObject>();

// //   //parsesub_JSONCommands(&obj);

// // }


// // // void mSensorsAnalog::parsesub_JSONCommands(JsonObject& _obj){

// // //   //If its null (nothing passed), try global parameter
// // //   if(_obj == nullptr){
// // //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "obj == nullptr"));
// // //     if(pCONT_set->pObj != nullptr){
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "pCONT_set->pObj != nullptr"));
// // //       _obj = pCONT_set->pObj;
// // //     }else{
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "No Object"));
// // //       return;
// // //     }
// // //   }

// // //   // Create local dereferenced variable
// // //   JsonObject obj = (*_obj); 



// // // }




// // #ifdef USE_MODULE_NETWORK_MQTT

// /*********************************************************************************************************************************************
// ******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/


// uint8_t mSensorsAnalog::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

//   JsonBuilderI->Start();
//     JsonBuilderI->Add("analog", analogRead(A0));
//   return JsonBuilderI->End();
  
// }



// // /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

// uint8_t mSensorsAnalog::ConstructJSON_Sensor(uint8_t json_level){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Add("analog", analogRead(A0));

//     uint8_t percentage = mapvalue(analogRead(A0), 1024,320, 0,100);

//     JsonBuilderI->Add("percentage", percentage);
//     JsonBuilderI->Level_Start("limits");
//       JsonBuilderI->Add("min", 1024);    
//       JsonBuilderI->Add("max", 320);
//     JsonBuilderI->Level_End();

//   return JsonBuilderI->End();

// }


// /*********************************************************************************************************************************************
// ******** MQTT Stuff **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mSensorsAnalog::MQTTHandler_Init(){

//   struct handler<mSensorsAnalog>* ptr;

//   ptr = &mqtthandler_settings_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_HOUR; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 60; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// // void mSey

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mSensorsAnalog::MQTTHandler_Set_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mSensorsAnalog::MQTTHandler_Set_TelePeriod()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
//   }
// }

// /**
//  * @brief Check all handlers if they require action
//  * */
// void mSensorsAnalog::MQTTHandler_Sender(uint8_t id)
// {
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_ANALOG_ID, handle, id);
//   }
// }

// #endif

