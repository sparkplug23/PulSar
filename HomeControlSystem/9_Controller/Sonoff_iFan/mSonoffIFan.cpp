#include "mSonoffIFan.h"

// iFan03 Hardware
// 4 Relays (1 Light + 3 Fan)
// 433 mhz controller via serial sMCU

#ifdef USE_MODULE_CUSTOM_SONOFF_IFAN

int8_t mSonoffIFan::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
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

    break;
    case FUNC_EVERY_250_MSECOND:
      SpeedRefresh();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;
    case FUNC_SERIAL:
      SerialInput();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
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



/*********************************************************************************************/

uint8_t mSonoffIFan::GetFanspeed(void)
{
  if (ifan_fanspeed_timer) {
    return ifan_fanspeed_goal;                     // Do not show sequence fanspeed
  } else {
    /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
      000x = 0
      001x = 1
      011x = 2
      101x = 3 (ifan02) or 100x = 3 (ifan03)
    */
    uint8_t fanspeed = (uint8_t)( pCONT_set->power &0xF) >> 1;
    if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
    return fanspeed;
  }
}


// Probably to be handled using "light_interface?"
uint8_t mSonoffIFan::GetLightState(void)
{
  return pCONT_mry->CommandGet_Relay_Power(0);
}
void mSonoffIFan::SetLightState(uint8_t state)
{
  pCONT_mry->CommandSet_Relay_Power(state);
}

/*********************************************************************************************/

void mSonoffIFan::SetFanSpeed(uint8_t fanspeed, bool sequence)
{
  ifan_fanspeed_timer = 0;                         // Stop any sequence
  ifan_fanspeed_goal = fanspeed;

  uint8_t fanspeed_now = GetFanspeed();

  if (fanspeed == fanspeed_now) { return; }

  uint8_t fans;// = kIFan02Speed[fanspeed];
  // if (SONOFF_IFAN03 == my_module_type) {
  //   if (sequence) {
  //     fanspeed = kIFan03Sequence[fanspeed_now][ifan_fanspeed_goal];
  //     if (fanspeed != ifan_fanspeed_goal) {
  //       if (0 == fanspeed_now) {
  //         ifan_fanspeed_timer = 20;                // Need extra time to power up fan
  //       } else {
  //         ifan_fanspeed_timer = 2;
  //       }
  //     }
  //   }
    fans = kIFan03Speed[fanspeed];
  // }
  for (uint32_t i = 1; i < 4; i++) {
    uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
    pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4 (index 1,2,3 skipping 0)
    fans >>= 1;
  }

}

/*********************************************************************************************/

// void mSonoffIFan::SonoffIfanReceived(void)
// {
//   char svalue[32];

//   uint8_t mode = serial_in_buffer[3];
//   uint8_t action = serial_in_buffer[6];

//   if (4 == mode) {
//     if (action < 4) {
//       // AA 55 01 04 00 01 00 06 - Fan 0
//       // AA 55 01 04 00 01 01 07 - Fan 1
//       // AA 55 01 04 00 01 02 08 - Fan 2
//       // AA 55 01 04 00 01 03 09 - Fan 3
//       if (action != GetFanspeed()) {
//         snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
//         ExecuteCommand(svalue, SRC_REMOTE);
// #ifdef USE_BUZZER
//         BuzzerEnabledBeep((action) ? action : 1, (action) ? 1 : 4);  // Beep action times
// #endif
//       }
//     } else {
//       // AA 55 01 04 00 01 04 0A - Light
//       ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
//     }
//   }
//   if (6 == mode) {
//     // AA 55 01 06 00 01 01 09 - Buzzer
//     Settings.flag3.buzzer_enable = !Settings.flag3.buzzer_enable;  // SetOption67 - Enable buzzer when available
//   }
//   if (7 == mode) {
//     // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
// #ifdef USE_BUZZER
//     BuzzerEnabledBeep(4, 1);                       // Beep four times
// #endif
//   }

//   // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
//   // AA 55 01 04 00 00 05
//   serial_in_buffer[5] = 0;                      // Ack
//   serial_in_buffer[6] = 0;                      // Crc
//   for (uint32_t i = 0; i < 7; i++) {
//     if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
//     Serial.write(serial_in_buffer[i]);
//   }
// }

bool mSonoffIFan::SerialInput(void)
{
//   if (SONOFF_IFAN03 == my_module_type) {
//     if (0xAA == serial_in_byte) {               // 0xAA - Start of text
//       serial_in_byte_counter = 0;
//       ifan_receive_flag = true;
//     }
//     if (ifan_receive_flag) {
//       serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
//       if (serial_in_byte_counter == 8) {
//         // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
//         // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
//         // AA 55 01 04 00 01 00 06 - Fan 0
//         // AA 55 01 04 00 01 01 07 - Fan 1
//         // AA 55 01 04 00 01 02 08 - Fan 2
//         // AA 55 01 04 00 01 03 09 - Fan 3
//         // AA 55 01 04 00 01 04 0A - Light
//         // AA 55 01 06 00 01 01 09 - Buzzer
//         // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
//         AddLogSerial(LOG_LEVEL_DEBUG);
//         uint8_t crc = 0;
//         for (uint32_t i = 2; i < 7; i++) {
//           crc += serial_in_buffer[i];
//         }
//         if (crc == serial_in_buffer[7]) {
//           SonoffIfanReceived();
//           ifan_receive_flag = false;
//           return true;
//         }
//       }
//       serial_in_byte = 0;
//     }
//     return false;
//   }
}


/*********************************************************************************************/

void mSonoffIFan::init(void)
{
  if(pCONT_set->my_module_type == MODULE_SONOFF_IFAN03_ID){
  //   SetSerial(9600, TS_SERIAL_8N1);
    settings.fEnableModule = true;
  }
}

void mSonoffIFan::SpeedRefresh(void)
{
  // if (SONOFF_IFAN03 == my_module_type) {
    if (ifan_fanspeed_timer) {
      ifan_fanspeed_timer--;
      if (!ifan_fanspeed_timer) {
        SetFanSpeed(ifan_fanspeed_goal, false);
      }
    }
  // }

  // if (ifan_restart_flag && (4 == pCONT_time->uptime.seconds_nonreset) && (SONOFF_IFAN02 == my_module_type)) {  // Microcontroller needs 3 seconds before accepting commands
  //   ifan_restart_flag = false;
  //   SetDevicePower(1, SRC_RETRY);      // Sync with default power on state microcontroller being Light ON and Fan OFF
  //   SetDevicePower(pCONT_set->power, SRC_RETRY);  // Set required power on state
  // }
}





uint8_t mSonoffIFan::ConstructJSON_Settings(uint8_t json_method){
  
  JsonBuilderI->Start();
    JsonBuilderI->Add_P("test",0);  
  JsonBuilderI->End();

}

uint8_t mSonoffIFan::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
    JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
  JsonBuilderI->End();

}




/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSonoffIFan::MQTTHandler_Init(){

  struct handler<mSonoffIFan>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSonoffIFan::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSonoffIFan::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSonoffIFan::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSonoffIFan>* list_p[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_CUSTOM_SONOFF_IFAN_ID,
    list_p, list_ids, sizeof(list_p)/sizeof(list_p[0]), mqtt_handler_id
  );

}

#endif
