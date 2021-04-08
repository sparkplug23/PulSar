#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3

const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_CTR = D_MODULE_ENERGY_PZEM004T_CTR;
const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR = D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR;

void mEnergyPZEM004T::Pre_Init(void)
{
  #ifdef ESP8266
  if (pCONT_pins->PinUsed(GPIO_PZEM016_RX_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID)) {
    pCONT_set->runtime_value.energy_driver = pCONT_iEnergy->D_GROUP_MODULE_ENERGY_PZEM004T_ID;
    settings.fEnableSensor = true;
  }
  #else
  pCONT_set->runtime_value.energy_driver = pCONT_iEnergy->D_GROUP_MODULE_ENERGY_PZEM004T_ID;
  settings.fEnableSensor = true;
  #endif//
}


void mEnergyPZEM004T::Init(void)
{

  #ifdef ESP8266
  PzemAcModbus = new TasmotaModbus(pCONT_pins->GetPin(GPIO_PZEM016_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));
  #endif

  #ifdef USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2
  PzemAcModbus = new TasmotaModbus(16, 17);
  #endif


  uint8_t result = PzemAcModbus->Begin(9600);
    AddLog(LOG_LEVEL_TEST, PSTR("PzemAcModbus result = %d"),result);
  if (result) {
    // AddLog(LOG_LEVEL_TEST, PSTR("PzemAcModbus result = %d"),result);
    if (2 == result) { pCONT_sup->ClaimSerial(); }   // If serial0 is used, disable logging
    #ifdef DEVICE_CONSUMERUNIT
      pCONT_iEnergy->Energy.phase_count = 8;
    #endif 
    #ifdef DEVICE_PZEM_TESTER
      pCONT_iEnergy->Energy.phase_count = 2;
    #endif

    PzemAc.phase = 0;
    // AddLog(LOG_LEVEL_TEST,"mEnergyPZEM004T::init");
  } else {
    pCONT_set->runtime_value.energy_driver = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
  }

  

}


void mEnergyPZEM004T::ReadAndParse(void)
{
  //AddLog(LOG_LEVEL_TEST, PSTR("ReadAndParse"));

  bool data_ready = PzemAcModbus->ReceiveReady();
    
  if (data_ready) {
    uint8_t modbus_buffer[30];  // At least 5 + (2 * 10) = 25

    uint8_t registers = 10;
    if (ADDR_RECEIVE == PzemAc.address_step) {
      registers = 2;     // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
      PzemAc.address_step--;
    }
    uint8_t error = PzemAcModbus->ReceiveBuffer(modbus_buffer, registers);
    //AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, PzemAcModbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("PAC: PzemAc %d error %d"), PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, error);
    } else {
      pCONT_iEnergy->Energy.data_valid[PzemAc.phase] = 0;
      if (10 == registers) {

        ParseModbusBuffer(&pzem_modbus[PzemAc.phase], modbus_buffer);  
        
        // Move into energy module
        pCONT_iEnergy->Energy.voltage[PzemAc.phase]       = pzem_modbus[PzemAc.phase].voltage;   
        pCONT_iEnergy->Energy.current[PzemAc.phase]       = pzem_modbus[PzemAc.phase].current;  
        pCONT_iEnergy->Energy.active_power[PzemAc.phase]  = pzem_modbus[PzemAc.phase].active_power; 
        pCONT_iEnergy->Energy.frequency[PzemAc.phase]     = pzem_modbus[PzemAc.phase].frequency;  
        pCONT_iEnergy->Energy.power_factor[PzemAc.phase]  = pzem_modbus[PzemAc.phase].power_factor;  
        pCONT_iEnergy->Energy.energy2[PzemAc.phase]       = pzem_modbus[PzemAc.phase].energy;  

        // Serial.println(pCONT_iEnergy->Energy.voltage[PzemAc.phase]);

        //temp fix
        pCONT_iEnergy->Energy.energy += pCONT_iEnergy->Energy.energy2[PzemAc.phase];
        PzemAc.energy += pCONT_iEnergy->Energy.energy2[PzemAc.phase];  


// move to when sensor reading is done
        // Records energy when all phases have been measured, remove this?
        if (PzemAc.phase == pCONT_iEnergy->Energy.phase_count -1) {
          sReadSensor = true; // last phase reached
          if (PzemAc.energy > PzemAc.last_energy) {  // Handle missed phase
            if (pCONT_time->uptime.seconds_nonreset > PZEM_AC_STABILIZE) {
              //pCONT_iEnergy->EnergyUpdateTotal(PzemAc.energy, false);
              AddLog(LOG_LEVEL_INFO,PSTR("PzemAc.energy=%d"),(int)PzemAc.energy);
            }
            PzemAc.last_energy = PzemAc.energy;
          }
          PzemAc.energy = 0;
        }
      }
    }
  }

  if (0 == PzemAc.send_retry || data_ready) {
    if (0 == PzemAc.phase) {
      PzemAc.phase = pCONT_iEnergy->Energy.phase_count -1;
    } else {
      PzemAc.phase--; //remove? I dont want to remove phases
    }
    PzemAc.send_retry = ENERGY_WATCHDOG;

    // Send for new value
    if (ADDR_SEND == PzemAc.address_step) {
      PzemAcModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)PzemAc.address);
      AddLog(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)PzemAc.address=%d)"),(uint16_t)PzemAc.address);
      PzemAc.address_step--;
    } else {
      PzemAcModbus->Send(PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, 0x04, 0, 10);      
      AddLog(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(ADD%d+phase%d, 0x04, 0, 10)"),PZEM_AC_DEVICE_ADDRESS,PzemAc.phase);
    }

  }
  else {
    AddLog(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(PZEM) PzemAc.send_retry%d"),PzemAc.send_retry);
    PzemAc.send_retry--;
    if ((pCONT_iEnergy->Energy.phase_count > 1) && (0 == PzemAc.send_retry) && (pCONT_time->uptime.seconds_nonreset < PZEM_AC_STABILIZE)) {
      //pCONT_iEnergy->Energy.phase_count--;  // Decrement phases if no response after retry within 30 seconds after restart
    }
  }

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


bool mEnergyPZEM004T::PzemAcCommand(void)
{
  bool serviced = true;

  // if (CMND_MODULEADDRESS == pCONT_iEnergy->Energy.command_code) {
  //   PzemAc.address = XdrvMailbox.payload;  // Valid addresses are 1, 2 and 3
  //   PzemAc.address_step = ADDR_SEND;
  // }
  // else serviced = false;  // Unknown command

  return serviced;
}

void mEnergyPZEM004T::EveryLoop(){

  if(mTime::TimeReachedNonReset(&measure_time,1000)&&
     mTime::TimeReached(&measure_time_backoff,(900/pCONT_iEnergy->Energy.phase_count))
  ){ //125ms
   
    ReadAndParse(); 

    if(sReadSensor==true){//Reset if all measured
      measure_time.millis = millis();
      sReadSensor = false;
    }

  }

} // END EveryLoop



int8_t mEnergyPZEM004T::Tasker(uint8_t function){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
  }

  // DEBUG_OTA_FLASH_BLOCKER_UNTIL_STABLE_RETURN_ZERO();

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      Init();
    break;
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



uint8_t mEnergyPZEM004T::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();

  JsonBuilderI->Add(D_JSON_CHANNELCOUNT,         0);
  

  return JsonBuilderI->End();

}


uint8_t mEnergyPZEM004T::ConstructJSON_Sensor(uint8_t json_method){


  JsonBuilderI->Start();

  JsonBuilderI->Add(D_JSON_VOLTAGE, pzem_modbus[0].voltage);
  

  return JsonBuilderI->End();


  // D_DATA_BUFFER_CLEAR();

  // // DynamicJsonDocument doc(MQTT_MAX_PACKET_SIZE);
  // // JsonObject root = doc.to<JsonObject>();

  // // uint8_t ischanged = false;

  // // char channel_ctr[3];
  // // memset(channel_ctr,0,sizeof(channel_ctr));
  // // // sprintf(channel_ctr,"%02d",channel);

  // return 0;

  // //   JsonObject kwh_per_minute_obj = root.createNestedObject("kwh_stats"); 
  
  // JsonObject channel0_obj = root.createNestedObject("channel0");
    
  //   channel0_obj["voltage"] = pCONT_iEnergy->Energy.voltage[0];
  //   channel0_obj["current"] = pCONT_iEnergy->Energy.current[0];
  //   channel0_obj["active_power"] = pCONT_iEnergy->Energy.active_power[0];
  //   channel0_obj["frequency"] = pCONT_iEnergy->Energy.frequency[0];
  //   channel0_obj["power_factor"] = pCONT_iEnergy->Energy.power_factor[0];
  //   channel0_obj["energy"] = PzemAc.energy;

  // JsonObject channel1_obj = root.createNestedObject("channel1");
  //   channel1_obj["voltage"] = pCONT_iEnergy->Energy.voltage[1];
  //   channel1_obj["current"] = pCONT_iEnergy->Energy.current[1];
  //   channel1_obj["active_power"] = pCONT_iEnergy->Energy.active_power[1];
  //   channel1_obj["frequency"] = pCONT_iEnergy->Energy.frequency[1];
  //   channel1_obj["power_factor"] = pCONT_iEnergy->Energy.power_factor[1];
  //   channel1_obj["energy"] = PzemAc.energy;

    
  // //   kwh_per_minute_obj["index"] = Energy.stats.kwh_per_minute_index;
  // //   kwh_per_minute_obj["last_minutes_energy"] = Energy.stats.last_minutes_energy;
  // //   kwh_per_minute_obj["current_energy"] = Energy.stats.current_energy;
  // //   kwh_per_minute_obj["this_minutes_energy"] = Energy.stats.this_minutes_energy;


  // // if(json_method >= JSON_LEVEL_DETAILED){
  // //   JsonArray kwh_per_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_per_minute"); 
  // //   JsonArray kwh_each_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_each_minute");  
  // //   for (uint8_t min = 0; min < 60; min++) {
  // //     kwh_per_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_per_minute[min]);
  // //     kwh_each_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_each_minute[min]);
  // //   }
  // // }



  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  
  //   return data_buffer.payload.len>3?true:false;
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mEnergyPZEM004T::MQTTHandler_Init(){

  struct handler<mEnergyPZEM004T>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mEnergyPZEM004T::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mEnergyPZEM004T::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mEnergyPZEM004T::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mEnergyPZEM004T>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_ENERGY_PZEM004T_V3_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

////////////////////// END OF MQTT /////////////////////////

#endif
