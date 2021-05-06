#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3

const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_CTR = D_MODULE_ENERGY_PZEM004T_CTR;
const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR = D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR;

int8_t mEnergyPZEM004T::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
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

} // END function



void mEnergyPZEM004T::Pre_Init(void)
{
  if (pCONT_pins->PinUsed(GPIO_PZEM016_RX_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID)) {
    pCONT_set->runtime_var.energy_driver = D_GROUP_MODULE_ENERGY_PZEM004T_ID; // use bit logic also
    // set bit for drivers
    settings.fEnableSensor = true;
  }
}


void mEnergyPZEM004T::Init(void)
{

  modbus = new TasmotaModbus(pCONT_pins->GetPin(GPIO_PZEM016_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));

  uint8_t result = modbus->Begin(9600);

  AddLog(LOG_LEVEL_TEST, PSTR("modbus result = %d"),result);

  if (result) {
    // AddLog(LOG_LEVEL_TEST, PSTR("modbus result = %d"),result);
    // Change this to another function, that doesnt check pin, it just calls claimserial but internally checks if its being used
    // pCONT_sup->ClaimSerial();// also serial number );

    // pCONT_iEnergy->Energy.phase_count = 1;
    
    // // Set via json command, default to 1
    // #ifdef DEVICE_CONSUMERUNIT
    //   pCONT_iEnergy->Energy.phase_count = 8;
    // #endif 
    // #ifdef DEVICE_PZEM_TESTER
    //   pCONT_iEnergy->Energy.phase_count = 2;
    // #else
    // #endif

    // AddLog(LOG_LEVEL_TEST,"mEnergyPZEM004T::init");
  } else {
    pCONT_set->runtime_var.energy_driver = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
    AddLog(LOG_LEVEL_ERROR, PSTR("modbus result = %d"),result);
    // delay(3000);

  }
  pCONT_iEnergy->Energy.phase_count = 1;
  if(pCONT_iEnergy->Energy.phase_count<=MAX_ENERGY_SENSORS)
  {
    data_modbus = new DATA_MODBUS[1];//pCONT_iEnergy->Energy.phase_count];
    pCONT_set->runtime_var.energy_driver = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
  }

  // data_modbus[0].voltage = 123;
  // data_modbus[0].current = 456;
  // data_modbus[0].voltage = 123;

  // if(data_modbus)
  // {
  //   Serial.println("data_modbus");
  // }
  // else
  // {
  //   Serial.println("!data_modbus");


  // }

  // delay(5000);


  // data_modbus[0].voltage = 1;
  // data_modbus[1].voltage = 2;
  // data_modbus[2].voltage = 3;

  // Optional poll for all sensors?
  // // Else its default
  // settings.found = pCONT_iEnergy->Energy.phase_count;

  // Init the default

}



/**
 * I need to split this to read a single sensor, thus I can have different rates
 * */
void mEnergyPZEM004T::SplitTask_UpdateSensor(uint8_t device_id)
{
  // AddLog(LOG_LEVEL_TEST, PSTR("SplitTask_UpdateSensor %d %d"), device_id, transceive_mode);

  switch(transceive_mode)
  {
    default: // Restart new reading
    case TRANSCEIVE_REQUEST_READING_ID:
    case TRANSCEIVE_RESPONSE_TIMEOUT_ID:
    case TRANSCEIVE_RESPONSE_SUCCESS_ID:

      stats.start_time = millis();
      modbus->Send(pCONT_iEnergy->address[device_id][3], 0x04, 0, 10);      
      
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("modbus->Send(address=%d, func=%d, s_add=%d, 10)"), device_id, 0x04, 0, 10);

      transceive_mode = TRANSCEIVE_AWAITING_RESPONSE_ID;

    break; // Allow follow into next mode
    case TRANSCEIVE_AWAITING_RESPONSE_ID:

      if(modbus->ReceiveReady())
      {
        uint8_t modbus_buffer[30] = {0};  // At least 5 + (2 * 10) = 25
        uint8_t registers = 10;
        uint8_t error = modbus->ReceiveBuffer(modbus_buffer, registers);

        if(!error)
        {
          //AddLog_Array(LOG_LEVEL_DEBUG_MORE, PSTR("buffer"), modbus_buffer, (uint8_t)30);
          AddLog(LOG_LEVEL_DEBUG_MORE, "ReceiveCount() %d", modbus->ReceiveCount());
          // Check if response matches expected device
          // if(modbus_buffer[0] == pCONT_iEnergy->address[device_id][3]){
            AddLog(LOG_LEVEL_DEBUG, PSTR("Read SUCCESS id=%d \tvolt=%d"), device_id, (int)data_modbus[device_id].voltage);
            ParseModbusBuffer(&data_modbus[device_id], modbus_buffer);
            stats.success_reads++;
            stats.end_time = millis();
            stats.sample_time = stats.end_time - stats.start_time;
          // }else{            
          //   transceive_mode = TRANSCEIVE_RESPONSE_ERROR_ID;
          // }
          transceive_mode = TRANSCEIVE_RESPONSE_SUCCESS_ID;
          timeout = millis();
        }
        else
        {
          AddLog(LOG_LEVEL_DEBUG, "error %d %d", modbus->ReceiveReady(), error);
          transceive_mode = TRANSCEIVE_RESPONSE_ERROR_ID;
        }
      }

    break;
  }
  
}


void mEnergyPZEM004T::ParseModbusBuffer(DATA_MODBUS* mod, uint8_t* buffer){
  //           0     1     2     3     4     5     6     7     8     9           = ModBus register
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
  // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
  // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
  mod->voltage       = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                  // 6553.0 V
  mod->current       = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
  mod->active_power  = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
  mod->frequency     = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                              // 50.0 Hz
  mod->power_factor  = (float)((buffer[19] << 8) + buffer[20]) / 100.0;  
  mod->energy        = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]); 
  mod->measured_time = millis();

}

void mEnergyPZEM004T::ParseModbusBuffer(PZEM_MODBUS* mod, uint8_t* buffer){
  //           0     1     2     3     4     5     6     7     8     9           = ModBus register
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
  // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
  // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
  mod->voltage       = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                  // 6553.0 V
  mod->current       = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
  mod->active_power  = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
  mod->frequency     = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                              // 50.0 Hz
  mod->power_factor  = (float)((buffer[19] << 8) + buffer[20]) / 100.0;  
  mod->energy        = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]);    

}


void mEnergyPZEM004T::EveryLoop(){

  if(mTime::TimeReachedNonReset(&measure_time,settings.rate_measure_ms))
  {

    SplitTask_UpdateSensor(active_sensor);

    if(labs(millis()-timeout)>200)
    {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(DEBUG_INSERT_PAGE_BREAK "TRANSCEIVE_RESPONSE_TIMEOUT_ID 1"));
      transceive_mode = TRANSCEIVE_RESPONSE_TIMEOUT_ID;
      timeout = millis();
      stats.timeout_reads++;
    }

    if(transceive_mode == TRANSCEIVE_RESPONSE_SUCCESS_ID)
    {
      // If needed, advance to next sensor
      if(active_sensor < pCONT_iEnergy->Energy.phase_count)
      {
        // active_sensor++;
      }
      else
      {
        active_sensor = 0; // reset to start or remain on single sensor 
      }
    }

  }

} // END EveryLoop



uint8_t mEnergyPZEM004T::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.found);
  return JBI->End();

}


uint8_t mEnergyPZEM004T::ConstructJSON_Sensor(uint8_t json_method){

  char buffer[40];

  JBI->Start();
  for(int ii=0;ii<pCONT_iEnergy->Energy.phase_count;ii++)
  {
    JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_ENERGY_PZEM004T_V3_ID, ii, buffer, sizeof(buffer)));
      JBI->Add(D_JSON_VOLTAGE,      data_modbus[ii].voltage);
      JBI->Add(D_JSON_CURRENT,      data_modbus[ii].current);
      JBI->Add(D_JSON_ACTIVE_POWER, data_modbus[ii].active_power);
      JBI->Add(D_JSON_FREQUENCY,    data_modbus[ii].frequency);
      JBI->Add(D_JSON_POWER_FACTOR, data_modbus[ii].power_factor);
      JBI->Add(D_JSON_ENERGY,       data_modbus[ii].energy);
    JBI->Level_End();
  }

  JBI->Level_Start("stats");
    JBI->Add("success", stats.success_reads);
    JBI->Add("timeout", stats.timeout_reads);
    JBI->Add("sample_time", stats.sample_time);
    JBI->Add("start_time", stats.start_time);
    JBI->Add("end_time", stats.end_time);

  JBI->Level_End();
  return JBI->End();

}


#ifdef USE_MODULE_NETWORK_MQTT

void mEnergyPZEM004T::MQTTHandler_Init(){

  struct handler<mEnergyPZEM004T>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SENSOR_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->handler_id = MQTT_HANDLER_SENSOR_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mEnergyPZEM004T::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mEnergyPZEM004T::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mEnergyPZEM004T::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_ENERGY_PZEM004T_V3_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
