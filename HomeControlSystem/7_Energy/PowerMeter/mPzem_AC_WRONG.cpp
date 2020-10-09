// #include "mPzem_AC.h"

// #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS

// void mPzem_AC::Pre_Init(void)
// {
//   if (pCONT_pins->PinUsed(GPIO_PZEM004_RX_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID)) {
//     pCONT_set->energy_flg = pCONT_iEnergy->ENERGY_MODULE_PZEM004TV3_ID;
//     settings.fEnableSensor = true;
//   }
// }

// void mPzem_AC::Init(void)
// {
//   // PzemAcModbus = new TasmotaModbus(pCONT_set->pin[GPIO_PZEM016_RX_ID], pCONT_set->pin[GPIO_PZEM0XX_TX_ID]);
//   // uint8_t result = PzemAcModbus->Begin(9600);
//   // if (result) {
//   //   //if (2 == result) { ClaimSerial(); }   // If serial0 is used, disable logging
    
//   // } else {
//   // }

// DEBUG_LINE;

//    // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
//   PzemSerial = new TasmotaSerial(pCONT_pins->GetPin(GPIO_PZEM004_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID), 1);
//   if (PzemSerial->begin(9600)) {
//     if (PzemSerial->hardwareSerial()) {
//     //   ClaimSerial();
//     // DEBUG_LINE_LED;
    
//     AddLog_P(LOG_LEVEL_TEST,"NOT mPzem_AC::hardwareSerial");
//     }
// DEBUG_LINE;
//     #ifdef DEVICE_CONSUMERUNIT
//       pCONT_iEnergy->Energy.phase_count = 8;  // Start off with three phases
//     #else
//       pCONT_iEnergy->Energy.phase_count = 1;  // Start off with three phases
//     #endif
//     PzemAc.phase = 0;
//     // AddLog_P(LOG_LEVEL_TEST,"mPzem_AC::init");
//     Pzem.read_state = 1;
//     Pzem.phase = 0;
//   } else {
//     // energy_flg = ENERGY_NONE;
//     pCONT_set->energy_flg = 0;
//     pCONT_set->energy_flg = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
//     AddLog_P(LOG_LEVEL_TEST,"NOT mPzem_AC::init");
//   }

// }



// uint8_t mPzem_AC::PzemCrc(uint8_t *data)
// {
//   uint16_t crc = 0;
//   for (uint32_t i = 0; i < sizeof(PZEMCommand) -1; i++) {
//     crc += *data++;
//   }
//   return (uint8_t)(crc & 0xFF);
// }

// void mPzem_AC::PzemSend(uint8_t cmd)
// {
//   DEBUG_LINE;
//   PZEMCommand pzem;

//   pzem.command = cmd;
//   pzem.addr[0] = 192;    // Address 192.168.1.1 for Tasmota legacy reason
//   pzem.addr[1] = 168;
//   pzem.addr[2] = 1;
//   pzem.addr[3] = ((PZEM_SET_ADDRESS == cmd) && Pzem.address) ? Pzem.address : PZEM_AC_DEVICE_ADDRESS + Pzem.phase;
//   pzem.data = 0;

//   AddLog_P(LOG_LEVEL_TEST,PSTR("pzem.addr[3]=%d"),pzem.addr[3]);

//   uint8_t* bytes = (uint8_t*)&pzem;
//   pzem.crc = PzemCrc(bytes);

//   PzemSerial->flush();
//   PzemSerial->write(bytes, sizeof(pzem));

//   Pzem.address = 0;
// }

// bool mPzem_AC::PzemReceiveReady(void)
// {
//   return PzemSerial->available() >= (int)sizeof(PZEMCommand);
// }

// bool mPzem_AC::PzemRecieve(uint8_t resp, float *data)
// {
//   DEBUG_LINE;
//   //  0  1  2  3  4  5  6
//   // A4 00 00 00 00 00 A4 - Set address
//   // A0 00 D4 07 00 00 7B - Voltage (212.7V)
//   // A1 00 00 0A 00 00 AB - Current (0.1A)
//   // A1 00 00 00 00 00 A1 - No current
//   // A2 00 16 00 00 00 B8 - Power (22W)
//   // A2 08 98 00 00 00 42 - Power (2200W)
//   // A2 00 00 00 00 00 A2 - No power
//   // A3 00 08 A4 00 00 4F - Energy (2.212kWh)
//   // A3 01 86 9F 00 00 C9 - Energy (99.999kWh)

//   uint8_t buffer[sizeof(PZEMCommand)] = { 0 };

//   unsigned long start = millis();
//   uint8_t len = 0;
//   while ((len < sizeof(PZEMCommand)) && (millis() - start < PZEM_DEFAULT_READ_TIMEOUT)) {
//     if (PzemSerial->available() > 0) {
//       uint8_t c = (uint8_t)PzemSerial->read();
//       if (!len && ((c & 0xF8) != 0xA0)) {  // 10100xxx
//         continue;  // fix skewed data
//       }
//       buffer[len++] = c;
//     }
//   }

//   AddLog_Array(LOG_LEVEL_DEBUG_MORE, "buffer", buffer, len);

//   if (len != sizeof(PZEMCommand)) {
//    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_DEBUG "Pzem comms timeout"));
//     return false;
//   }
//   if (buffer[6] != PzemCrc(buffer)) {
//    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_DEBUG "Pzem crc error"));
//     return false;
//   }
//   if (buffer[0] != resp) {
//    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_DEBUG "Pzem bad response"));
//     return false;
//   }

//   switch (resp) {
//     case RESP_VOLTAGE:
//       *data = (float)(buffer[1] << 8) + buffer[2] + (buffer[3] / 10.0);    // 65535.x V
//       break;
//     case RESP_CURRENT:
//       *data = (float)(buffer[1] << 8) + buffer[2] + (buffer[3] / 100.0);   // 65535.xx A
//       break;
//     case RESP_POWER:
//       *data = (float)(buffer[1] << 8) + buffer[2];                         // 65535 W
//       break;
//     case RESP_ENERGY:
//       *data = (float)((uint32_t)buffer[1] << 16) + ((uint16_t)buffer[2] << 8) + buffer[3];  // 16777215 Wh
//       break;
//   }
//   return true;
// }

// /*********************************************************************************************/

// const uint8_t pzem_commands[]  { PZEM_SET_ADDRESS, PZEM_VOLTAGE, PZEM_CURRENT, PZEM_POWER, PZEM_ENERGY };
// const uint8_t pzem_responses[] { RESP_SET_ADDRESS, RESP_VOLTAGE, RESP_CURRENT, RESP_POWER, RESP_ENERGY };

// void mPzem_AC::PzemEvery250ms(void)
// {
//   DEBUG_LINE;
//   bool data_ready = PzemReceiveReady();
//   // AddLog_P(LOG_LEVEL_TEST,PSTR("data_ready=%d"),data_ready);

//   if (data_ready) {
//     AddLog_P(LOG_LEVEL_TEST,PSTR("data_ready=%d"),data_ready);
//     float value = 0;
//     if (PzemRecieve(pzem_responses[Pzem.read_state], &value)) {
//       pCONT_iEnergy->Energy.data_valid[Pzem.phase] = 0;
//       switch (Pzem.read_state) {
//         case 1:  // Voltage as 230.2V
//           pCONT_iEnergy->Energy.voltage[Pzem.phase] = value;
//           break;
//         case 2:  // Current as 17.32A
//           pCONT_iEnergy->Energy.current[Pzem.phase] = value;
//           break;
//         case 3:  // Power as 20W
//           pCONT_iEnergy->Energy.active_power[Pzem.phase] = value;
//           break;
//         case 4:  // Total energy as 99999Wh
//           Pzem.energy += value;
//           if (Pzem.phase == pCONT_iEnergy->Energy.phase_count -1) {
//             if (Pzem.energy > Pzem.last_energy) {  // Handle missed phase
//               if (pCONT->mt->uptime.seconds_nonreset > PZEM_STABILIZE) {
//                 //EnergyUpdateTotal(Pzem.energy, false);
//               }
//               Pzem.last_energy = Pzem.energy;
//             }
//             Pzem.energy = 0;
//           }
//           break;
//       }
//       Pzem.read_state++;
//       if (5 == Pzem.read_state) {
//         Pzem.read_state = 1;
//       }

//      AddLog_P(LOG_LEVEL_TEST, PSTR("PZM: Retry %d"), 5 - Pzem.send_retry);
//     }
//   }
//   DEBUG_LINE;

//   if (0 == Pzem.send_retry || data_ready) {
//     if (1 == Pzem.read_state) {
//       // if (0 == Pzem.phase) {
//       //   Pzem.phase = pCONT_iEnergy->Energy.phase_count -1;
//       // } else {
//       //   Pzem.phase--;
//       // }

//      AddLog_P(LOG_LEVEL_TEST, PSTR("PZM: Probing address %d, Max phases %d"), Pzem.phase +1,pCONT_iEnergy->Energy.phase_count);
//     }

//     if (Pzem.address) {
//       Pzem.read_state = 0;  // Set address
//     }

//     Pzem.send_retry = 5;
    
//     AddLog_P(LOG_LEVEL_TEST, PSTR("PzemSend(pzem_commands[Pzem.read_state]) %d %d"),pzem_commands[Pzem.read_state],Pzem.read_state);
//     PzemSend(pzem_commands[Pzem.read_state]);
//   }
//   else {
//     Pzem.send_retry--;
//      AddLog_P(LOG_LEVEL_TEST, PSTR("PZM: Pzem.send_retry %d"),Pzem.send_retry);
//     // if ((pCONT_iEnergy->Energy.phase_count > 1) && (0 == Pzem.send_retry) && (uptime < PZEM_STABILIZE)) {
//     //   pCONT_iEnergy->Energy.phase_count--;  // Decrement phases if no response after retry within 30 seconds after restart
//     // }
//   }
// }



// int8_t mPzem_AC::Tasker(uint8_t function){
  
//   int8_t function_result = 0;
  
//   // some functions must run regardless
//   switch(function){
//     case FUNC_PRE_INIT:
//       Pre_Init();
//     break;
//   }

//   // Only continue to remaining functions if sensor has been detected and enabled
//   // if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case FUNC_INIT:
//       Init();
//     break;
//     /************
//      * SETTINGS SECTION * 
//     *******************/
//     case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
//       // Settings_Load();
//     break;
//     case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
//       // Settings_Save();
//     break;
//     case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
//       // Settings_Default();
//     break;
//     case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
//       // Settings_Default();
//       // pCONT_set->SettingsSave(2);
//     break;
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case FUNC_LOOP: 
//       //EveryLoop();
//     break;  
//     case FUNC_EVERY_250_MSECOND:
//       // if(PzemSerial && (pCONT_time->uptime.seconds_nonreset>4)){
//       //   PzemEvery250ms();
//       // }
//     break;
//     case FUNC_EVERY_SECOND:

    
//       if(PzemSerial && (pCONT_time->uptime.seconds_nonreset>4)){
//         PzemEvery250ms();
//       }

//      //AddLog_P(LOG_LEVEL_TEST, PSTR("pCONT_pins->GetPin(GPIO_PZEM004_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID) %d %d"),pCONT_pins->GetPin(GPIO_PZEM004_RX_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));

      
//      AddLog_P(LOG_LEVEL_TEST, PSTR("PzemSend(pzem_commands[Pzem.read_state]) %d %d"),pzem_commands[Pzem.read_state],Pzem.read_state);
//     PzemSend(PZEM_VOLTAGE);
      
//     break;
//     case FUNC_EVERY_MINUTE:
    
//       AddLog_P(LOG_LEVEL_TEST, PSTR("PzemSend(pzem_commands[Pzem.read_state]) %d %d"),pzem_commands[Pzem.read_state],Pzem.read_state);
//       PzemSend(pzem_commands[Pzem.read_state]);
//     break;
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case FUNC_COMMAND:

//     break;  
//     case FUNC_JSON_COMMAND:
//       //function_result = parse_JSONCommand();
//     break;      
//     /************
//      * WEBPAGE SECTION * 
//     *******************/
//     #ifdef USE_WEBSERVER
//     case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
//       //WebAppend_Root_Status_Table_Draw();
//     break;
//     case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
//       //WebAppend_Root_Status_Table_Data();
//     break;
//     #endif //USE_WEBSERVER
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MQTT
//     case FUNC_MQTT_HANDLERS_INIT:
//       MQTTHandler_Init(); 
//     break;
//     case FUNC_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init();
//     break;
//     case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
//       MQTTHandler_Set_TelePeriod();
//     break;
//     case FUNC_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     #endif //USE_MQTT
//   }
  
//   return function_result;

// } // END function



// uint8_t mPzem_AC::ConstructJSON_Settings(uint8_t json_method){

//   memset(&data_buffer2,0,sizeof(data_buffer2));
//   // StaticJsonDocument<400> doc;
//   // JsonObject root = doc.to<JsonObject>();

//   // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();

//   // data_buffer2.payload.len = measureJson(root)+1;
//   // serializeJson(doc,data_buffer2.payload.ctr);
  
//   // return data_buffer2.payload.len>3?true:false;

//   return 0;

// }


// uint8_t mPzem_AC::ConstructJSON_Sensor(uint8_t json_method){

//   memset(&data_buffer2,0,sizeof(data_buffer2));

//   // DynamicJsonDocument doc(MQTT_MAX_PACKET_SIZE);
//   // JsonObject root = doc.to<JsonObject>();

//   // uint8_t ischanged = false;

//   // char channel_ctr[3];
//   // memset(channel_ctr,0,sizeof(channel_ctr));
//   // // sprintf(channel_ctr,"%02d",channel);

//   return 0;

//   // //   JsonObject kwh_per_minute_obj = root.createNestedObject("kwh_stats"); 
  
//   // JsonObject channel0_obj = root.createNestedObject("channel0");
    
//   //   channel0_obj["voltage"] = pCONT_iEnergy->Energy.voltage[0];
//   //   channel0_obj["current"] = pCONT_iEnergy->Energy.current[0];
//   //   channel0_obj["active_power"] = pCONT_iEnergy->Energy.active_power[0];
//   //   channel0_obj["frequency"] = pCONT_iEnergy->Energy.frequency[0];
//   //   channel0_obj["power_factor"] = pCONT_iEnergy->Energy.power_factor[0];
//   //   channel0_obj["energy"] = PzemAc.energy;

//   // JsonObject channel1_obj = root.createNestedObject("channel1");
//   //   channel1_obj["voltage"] = pCONT_iEnergy->Energy.voltage[1];
//   //   channel1_obj["current"] = pCONT_iEnergy->Energy.current[1];
//   //   channel1_obj["active_power"] = pCONT_iEnergy->Energy.active_power[1];
//   //   channel1_obj["frequency"] = pCONT_iEnergy->Energy.frequency[1];
//   //   channel1_obj["power_factor"] = pCONT_iEnergy->Energy.power_factor[1];
//   //   channel1_obj["energy"] = PzemAc.energy;

    
//   // //   kwh_per_minute_obj["index"] = Energy.stats.kwh_per_minute_index;
//   // //   kwh_per_minute_obj["last_minutes_energy"] = Energy.stats.last_minutes_energy;
//   // //   kwh_per_minute_obj["current_energy"] = Energy.stats.current_energy;
//   // //   kwh_per_minute_obj["this_minutes_energy"] = Energy.stats.this_minutes_energy;


//   // // if(json_method >= JSON_LEVEL_DETAILED){
//   // //   JsonArray kwh_per_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_per_minute"); 
//   // //   JsonArray kwh_each_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_each_minute");  
//   // //   for (uint8_t min = 0; min < 60; min++) {
//   // //     kwh_per_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_per_minute[min]);
//   // //     kwh_each_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_each_minute[min]);
//   // //   }
//   // // }



//   // data_buffer2.payload.len = measureJson(root)+1;
//   // serializeJson(doc,data_buffer2.payload.ctr);

  
//   //   return data_buffer2.payload.len>3?true:false;
    
// }




// /*********************************************************************************************************************************************
// ******** MQTT Stuff **************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mPzem_AC::MQTTHandler_Init(){

//   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->fPeriodicEnabled = true;
//   mqtthandler_ptr->fSendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Settings;

//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->fPeriodicEnabled = true;
//   mqtthandler_ptr->fSendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
//   mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Sensor;

//   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->fPeriodicEnabled = true;
//   mqtthandler_ptr->fSendNow = true;
//   mqtthandler_ptr->tRateSecs = 1; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
//   mqtthandler_ptr->ConstructJSON_function = &mPzem_AC::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// void mPzem_AC::MQTTHandler_Set_fSendNow(){

//   mqtthandler_settings_teleperiod.fSendNow = true;
//   mqtthandler_sensor_ifchanged.fSendNow = true;
//   mqtthandler_sensor_teleperiod.fSendNow = true;

// } //end "MQTTHandler_Init"


// void mPzem_AC::MQTTHandler_Set_TelePeriod(){

//   mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// } //end "MQTTHandler_Set_TelePeriod"


// void mPzem_AC::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t flag_handle_all = false, handler_found = false;
//   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

//   do{

//     switch(mqtt_handler_id){
//       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
//       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
//       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
//       default: handler_found=false; break; // nothing 
//     } // switch

//     // Pass handlers into command to test and (ifneeded) execute
//     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_PZEM004T_MODBUS_ID,mqtthandler_ptr); }

//     // stop searching
//     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

//   }while(flag_handle_all);

// }

// ////////////////////// END OF MQTT /////////////////////////

// #endif



// // void mPzem_AC::EveryLoop(){

// //   if(mTime::TimeReachedNonReset(&measure_time,1000)&&mTime::TimeReached(&measure_time_backoff,1000/8)){ //125ms

// //     // PzemAcEverySecond();    
// //     //ReadAndParse(); 

// //     if(sReadSensor==true){//Reset if all measured
// //       measure_time.millis = millis();
// //     }

// //   }

// // } // END EveryLoop