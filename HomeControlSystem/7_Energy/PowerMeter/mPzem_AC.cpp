#include "mPzem_AC.h"

#ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS

void mPzem_AC::Pre_Init(void)
{
  if (pCONT_pins->PinUsed(GPIO_PZEM016_RX_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID)) {
    pCONT_set->energy_flg = pCONT_iEnergy->ENERGY_MODULE_PZEM004TV3_ID;
    settings.fEnableSensor = true;
  }
}


void mPzem_AC::Init(void)
{
  PzemAcModbus = new TasmotaModbus(pCONT_pins->GetPin(GPIO_PZEM016_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));
  uint8_t result = PzemAcModbus->Begin(9600);
  if (result) {
    if (2 == result) { pCONT_sup->ClaimSerial(); }   // If serial0 is used, disable logging
    #ifdef DEVICE_CONSUMERUNIT
      pCONT_iEnergy->Energy.phase_count = 8;  // Start off with three phases
    #endif
    PzemAc.phase = 0;
    // AddLog_P(LOG_LEVEL_TEST,"mPzem_AC::init");
  } else {
    pCONT_set->energy_flg = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
  }
}


void mPzem_AC::ReadAndParse(void)
{
  //AddLog_P(LOG_LEVEL_TEST, PSTR("ReadAndParse"));

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
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("PAC: PzemAc %d error %d"), PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, error);
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

        //temp fix
        pCONT_iEnergy->Energy.energy += pCONT_iEnergy->Energy.energy2[PzemAc.phase];
        PzemAc.energy += pCONT_iEnergy->Energy.energy2[PzemAc.phase];  


// move to when sensor reading is done
        // Records energy when all phases have been measured, remove this?
        if (PzemAc.phase == pCONT_iEnergy->Energy.phase_count -1) {
          sReadSensor = true; // last phase reached
          if (PzemAc.energy > PzemAc.last_energy) {  // Handle missed phase
            if (pCONT->mt->uptime.seconds_nonreset > PZEM_AC_STABILIZE) {
              //pCONT_iEnergy->EnergyUpdateTotal(PzemAc.energy, false);
              AddLog_P(LOG_LEVEL_INFO,PSTR("PzemAc.energy=%d"),(int)PzemAc.energy);
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
      AddLog_P(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)PzemAc.address=%d)"),(uint16_t)PzemAc.address);
      PzemAc.address_step--;
    } else {
      PzemAcModbus->Send(PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, 0x04, 0, 10);      
      AddLog_P(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(ADD%d+phase%d, 0x04, 0, 10)"),PZEM_AC_DEVICE_ADDRESS,PzemAc.phase);
    }

  }
  else {
    AddLog_P(LOG_LEVEL_INFO,PSTR("PzemAcModbus->Send(PZEM) PzemAc.send_retry%d"),PzemAc.send_retry);
    PzemAc.send_retry--;
    if ((pCONT_iEnergy->Energy.phase_count > 1) && (0 == PzemAc.send_retry) && (pCONT->mt->uptime.seconds_nonreset < PZEM_AC_STABILIZE)) {
      //pCONT_iEnergy->Energy.phase_count--;  // Decrement phases if no response after retry within 30 seconds after restart
    }
  }

}


void mPzem_AC::ParseModbusBuffer(PZEM_MODBUS* mod, uint8_t* buffer){
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


bool mPzem_AC::PzemAcCommand(void)
{
  bool serviced = true;

  // if (CMND_MODULEADDRESS == pCONT_iEnergy->Energy.command_code) {
  //   PzemAc.address = XdrvMailbox.payload;  // Valid addresses are 1, 2 and 3
  //   PzemAc.address_step = ADDR_SEND;
  // }
  // else serviced = false;  // Unknown command

  return serviced;
}

void mPzem_AC::EveryLoop(){

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



int8_t mPzem_AC::Tasker(uint8_t function){
  
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
      //WebAppend_Root_Status_Table_Draw();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      //WebAppend_Root_Status_Table_Data();
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

} // END function



uint8_t mPzem_AC::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  // StaticJsonDocument<400> doc;
  // JsonObject root = doc.to<JsonObject>();

  // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  
  // return data_buffer2.payload.len>3?true:false;

  return 0;

}


uint8_t mPzem_AC::ConstructJSON_Sensor(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  // DynamicJsonDocument doc(MQTT_MAX_PACKET_SIZE);
  // JsonObject root = doc.to<JsonObject>();

  // uint8_t ischanged = false;

  // char channel_ctr[3];
  // memset(channel_ctr,0,sizeof(channel_ctr));
  // // sprintf(channel_ctr,"%02d",channel);

  return 0;

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



  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  
  //   return data_buffer2.payload.len>3?true:false;
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mPzem_AC::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mPzem_AC::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mPzem_AC::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mPzem_AC::MQTTHandler_Sender(uint8_t mqtt_handler_id){

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
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_PZEM004T_MODBUS_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

#endif
