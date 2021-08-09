#include "mSensorColours.h"

// In the future will map different sensors to palettes (some special in here, or others, in here "heatmaps... or maybe move to normal palette and still call a heatmap")
// Thus, I can display light or sound into palettes



#ifdef USE_MODULE_CONTROLLER_SENSORCOLOURS

const char* mSensorColours::PM_MODULE_CONTROLLER_SENSOR_COLOURS_CTR = D_MODULE_CONTROLLER_SENSOR_COLOURS_CTR;
const char* mSensorColours::PM_MODULE_CONTROLLER_SENSOR_COLOURS_FRIENDLY_CTR = D_MODULE_CONTROLLER_SENSOR_COLOURS_FRIENDLY_CTR;

void mSensorColours::init(void)
{
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain

  // analogWrite(pin, pwm_range_min);

}


void mSensorColours::Pre_Init(){
  
  // if(pCONT_pins->PinUsed(GPIO_FAN_PWM1_ID)) {  // not set when 255
  //   pin = pCONT_pins->GetPin(GPIO_FAN_PWM1_ID);
  //   pinMode(pin, OUTPUT);
  //   settings.fEnableModule = true;
  // }

}

int8_t mSensorColours::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:  
      EverySecond();
    break;
    case FUNC_EVERY_250_MSECOND:
      // SpeedRefresh();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker



void mSensorColours::EverySecond(){

  // uint8_t buffer[100];

  //temp method
  
  uint8_t sensor_count = pCONT_msdb18->db18_sensors_active;


//AddLog(LOG_LEVEL_ERROR, PSTR("sensor_count=%d"),sensor_count);
//delay(2000);

  if((sensor_count>=DB18_SENSOR_MAX)||(sensor_count==0)){ return; }

// return ;

  // float temps[sensor_count];
  // uint8_t indexes[sensor_count];

  sensor_count = 6; // only map first 6 sensors


  // uint8_t sensor_ids[] = {}



  RgbColor colour;

  // RGB Gradient with 255 indexes
  uint8_t index = 0;
  encoded_gradient_temp_array[index++] = sensor_count;
  encoded_gradient_temp_array[index++] = 5;
  encoded_gradient_temp_array[index++] = 0;
  encoded_gradient_temp_array[index++] = 0;
  encoded_gradient_temp_array[index++] = 0;

  char name[30];

  int test_temp[sensor_count];


AddLog(LOG_LEVEL_TEST, PSTR("val=%d %d %d %d %d %d"),
  (int)pCONT_msdb18->sensor[0].reading.val,
  (int)pCONT_msdb18->sensor[1].reading.val,
  (int)pCONT_msdb18->sensor[2].reading.val,
  (int)pCONT_msdb18->sensor[3].reading.val,
  (int)pCONT_msdb18->sensor[4].reading.val,
  (int)pCONT_msdb18->sensor[5].reading.val);

AddLog(LOG_LEVEL_TEST, PSTR("add=%d %d %d %d %d %d"),
  pCONT_msdb18->sensor[0].address[7],
  pCONT_msdb18->sensor[1].address[7],
  pCONT_msdb18->sensor[2].address[7],
  pCONT_msdb18->sensor[3].address[7],
  pCONT_msdb18->sensor[4].address[7],
  pCONT_msdb18->sensor[5].address[7]);

  for(uint8_t ii=0;ii<sensor_count;ii++){

    // pCONT_msdb18->sensor[ii].reading.val = 20+(5*ii);
    
    int8_t device_id = -1;
    uint16_t unique_module_id = pCONT->GetModuleUniqueIDbyVectorIndex(EM_MODULE_SENSORS_DB18S20_ID);


    //temp solution
    switch(ii){
      case 0: sprintf(name, "TankPosition00\0"); break;
      case 1: sprintf(name, "TankPosition20\0"); break;
      case 2: sprintf(name, "TankPosition40\0"); break;
      case 3: sprintf(name, "TankPosition60\0"); break;
      case 4: sprintf(name, "TankPosition80\0"); break;
      case 5: sprintf(name, "TankPosition100\0"); break;
    }

    int16_t device_id_found = DLI->GetDeviceIDbyName(name, device_id, unique_module_id);

    // AddLog(LOG_LEVEL_INFO,PSTR("device_id_found = %d"),device_id_found);
    AddLog(LOG_LEVEL_INFO,PSTR("device_id_found = %s %d %d"),name,device_id_found,(int)pCONT_msdb18->sensor[device_id_found].reading.val);


  // Check for matches with variables names  
  // if ((
    // index_found = pCONT_sup->GetDListIDbyNameCtr(buffer, sizeof(buffer), c, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr)) >= 0) {
  //   // index_found = STATE_NUMBER_OFF_ID;    
  //     AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("index_found = %d"),index_found);    
  //     return index_found;
  // }
  int8_t true_struct_id = pCONT_msdb18->FindStructIndexByAddressID(device_id_found);

  AddLog(LOG_LEVEL_TEST, PSTR("device_id_found true_struct_id %d %d"),device_id_found,true_struct_id);

  if(true_struct_id != -1){
    
    test_temp[ii] = (int)pCONT_msdb18->sensor[true_struct_id].reading.val;
    colour = //RgbColor(ii,0,0);//
    pCONT_iLight->GetColourValueUsingMaps(pCONT_msdb18->sensor[true_struct_id].reading.val,0);

    // switch(ii){
    //   case 0: colour = HsbColor(pCONT_iLight->HueN2F(0),1.0f, 1.0f); break;
    //   case 1: colour = HsbColor(pCONT_iLight->HueN2F(120),1.0f, 1.0f); break;
    //   case 2: colour = HsbColor(pCONT_iLight->HueN2F(240),1.0f, 1.0f); break;
    //   case 3: colour = HsbColor(pCONT_iLight->HueN2F(330),1.0f, 1.0f); break;
    //   case 4: colour = HsbColor(pCONT_iLight->HueN2F(30),1.0f, 1.0f); break;
    //   case 5: colour = HsbColor(pCONT_iLight->HueN2F(180),1.0f, 1.0f); break;
    // }


    // AddL

    encoded_gradient_temp_array[index++] = map(ii, 0,5, 0,255);
    encoded_gradient_temp_array[index++] = colour.R;
    encoded_gradient_temp_array[index++] = colour.G;
    encoded_gradient_temp_array[index++] = colour.B;
  }

  }

  //remove the last unneeded iter
  // index--;

  AddLog(LOG_LEVEL_ERROR, PSTR("index=%d"),index);
  AddLog_Array(LOG_LEVEL_TEST, "test_temp", test_temp, 6);
  // delay(5000);


  pCONT_iLight->CommandSet_PaletteColour_RGBCCT_Raw_By_ID(mPaletteI->PALETTELIST_VARIABLE_GENERIC_01_ID, encoded_gradient_temp_array, index);
  pCONT_iLight->CommandSet_PaletteID(mPaletteI->PALETTELIST_VARIABLE_GENERIC_01_ID);
  pCONT_iLight->CommandSet_TransitionOrderID(TRANSITION_ORDER_INORDER_ID);

  // AddLog(LOG_LEVEL_TEST, PSTR("buffer=%s"),buffer);

  // AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_RELAY_CONTROLS is disabled"));
  
  AddLog_Array(LOG_LEVEL_TEST, "encoded_gradient_temp_array", encoded_gradient_temp_array, index);
  
  // if(pCONT_time->uptime.seconds_nonreset > 60){

  
  // //print result over mqtt so I can test
  // char buffer[300]; memset(&buffer, 0, sizeof(buffer));
  // for(uint8_t ii=0;ii<index;ii++){
  //   sprintf(buffer+strlen(buffer), "%d,", encoded_gradient_temp_array[ii]);
  // }
  // pCONT_mqtt->Send_Prefixed_P(PSTR("debug/1"),buffer);

  //  memset(&buffer, 0, sizeof(buffer));
  // for(uint8_t ii=0;ii<6;ii++){
  //   sprintf(buffer+strlen(buffer), "%d,", test_temp[ii]);
  // }
  // pCONT_mqtt->Send_Prefixed_P(PSTR("debug/2"), buffer);
  // pCONT_mqtt->ppublish("/debug", buffer, false);



}


uint8_t mSensorColours::ConstructJSON_Settings(uint8_t json_method){
  
  JsonBuilderI->Start();
    JsonBuilderI->Add_P("test",0);  
  JsonBuilderI->End();

}

uint8_t mSensorColours::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
    JBI->Array_Start("encoded_gradient_temp_array");
      for(int i=0;i<ARRAY_SIZE(encoded_gradient_temp_array);i++)
      {
        JBI->Add(encoded_gradient_temp_array[i]);
      }
    JBI->Array_End();
  JsonBuilderI->End();

}




/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSensorColours::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorColours::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorColours::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorColours::ConstructJSON_Sensor;
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorColours::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorColours::MQTTHandler_Set_TelePeriod()
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
void mSensorColours::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_SENSORCOLOURS_ID, handle, id);
  }
}


#endif
