#include "mStatusLEDs.h"

//  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Tasmota

#ifdef USE_MODULE_CONTROLLER_STATUS_LEDS

//Uses 433mhz radios, relays, buttons

/*********************************************************************************************\
  Sonoff iFan02 and iFan03
\*********************************************************************************************/

/*********************************************************************************************/

// // bool mStatusLEDs::IsModuleIfan(void)
// // {
// //   return ((SONOFF_IFAN02 == my_module_type) || (SONOFF_IFAN03 == my_module_type));
// // }

// // uint8_t mStatusLEDs::MaxFanspeed(void)
// // {
// //   return MAX_FAN_SPEED;
// // }

// uint8_t mStatusLEDs::GetFanspeed(void)
// {
//   // if (ifan_fanspeed_timer) {
//   //   return ifan_fanspeed_goal;                     // Do not show sequence fanspeed
//   // } else {
//   //   /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
//   //     000x = 0
//   //     001x = 1
//   //     011x = 2
//   //     101x = 3 (ifan02) or 100x = 3 (ifan03)
//   //   */
//   //   uint8_t fanspeed = (uint8_t)( pCONT_set->power &0xF) >> 1;
//   //   if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
//   //   return fanspeed;
//   // }

//   // Map and round it?
//   return set_fan_speed;

// }




// // Probably to be handled using "light_interface?"
// uint8_t mStatusLEDs::GetLightState(void)
// {
//   // return pCONT_mry->CommandGet_Relay_Power(0);
//   return 0;
// }
// void mStatusLEDs::SetLightState(uint8_t state)
// {
//   // pCONT_mry->CommandSet_Relay_Power(state);
// }

// /*********************************************************************************************/

// void mStatusLEDs::SetFanspeed(uint8_t fanspeed, bool sequence)
// {
//   ifan_fanspeed_timer = 0;                         // Stop any sequence
//   ifan_fanspeed_goal = fanspeed;

//   uint8_t fanspeed_now = GetFanspeed();

//   if (fanspeed == fanspeed_now) { return; }

//   uint8_t fans;// = kIFan02Speed[fanspeed];
//   // if (SONOFF_IFAN03 == my_module_type) {
//   //   if (sequence) {
//   //     fanspeed = kIFan03Sequence[fanspeed_now][ifan_fanspeed_goal];
//   //     if (fanspeed != ifan_fanspeed_goal) {
//   //       if (0 == fanspeed_now) {
//   //         ifan_fanspeed_timer = 20;                // Need extra time to power up fan
//   //       } else {
//   //         ifan_fanspeed_timer = 2;
//   //       }
//   //     }
//   //   }
//   //   fans = kIFan03Speed[fanspeed];
//   // // }
//   // for (uint32_t i = 2; i < 5; i++) {
//   //   uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
//   //   pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4
//   //   fans >>= 1;
//   // }

// }

// /*********************************************************************************************/

// // void mStatusLEDs::SonoffIfanReceived(void)
// // {
// //   char svalue[32];

// //   uint8_t mode = serial_in_buffer[3];
// //   uint8_t action = serial_in_buffer[6];

// //   if (4 == mode) {
// //     if (action < 4) {
// //       // AA 55 01 04 00 01 00 06 - Fan 0
// //       // AA 55 01 04 00 01 01 07 - Fan 1
// //       // AA 55 01 04 00 01 02 08 - Fan 2
// //       // AA 55 01 04 00 01 03 09 - Fan 3
// //       if (action != GetFanspeed()) {
// //         snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
// //         ExecuteCommand(svalue, SRC_REMOTE);
// // #ifdef USE_BUZZER
// //         BuzzerEnabledBeep((action) ? action : 1, (action) ? 1 : 4);  // Beep action times
// // #endif
// //       }
// //     } else {
// //       // AA 55 01 04 00 01 04 0A - Light
// //       ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
// //     }
// //   }
// //   if (6 == mode) {
// //     // AA 55 01 06 00 01 01 09 - Buzzer
// //     Settings.flag3.buzzer_enable = !Settings.flag3.buzzer_enable;  // SetOption67 - Enable buzzer when available
// //   }
// //   if (7 == mode) {
// //     // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
// // #ifdef USE_BUZZER
// //     BuzzerEnabledBeep(4, 1);                       // Beep four times
// // #endif
// //   }

// //   // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
// //   // AA 55 01 04 00 00 05
// //   serial_in_buffer[5] = 0;                      // Ack
// //   serial_in_buffer[6] = 0;                      // Crc
// //   for (uint32_t i = 0; i < 7; i++) {
// //     if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
// //     Serial.write(serial_in_buffer[i]);
// //   }
// // }

// // bool mStatusLEDs::SonoffIfanSerialInput(void)
// // {
// //   if (SONOFF_IFAN03 == my_module_type) {
// //     if (0xAA == serial_in_byte) {               // 0xAA - Start of text
// //       serial_in_byte_counter = 0;
// //       ifan_receive_flag = true;
// //     }
// //     if (ifan_receive_flag) {
// //       serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
// //       if (serial_in_byte_counter == 8) {
// //         // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
// //         // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
// //         // AA 55 01 04 00 01 00 06 - Fan 0
// //         // AA 55 01 04 00 01 01 07 - Fan 1
// //         // AA 55 01 04 00 01 02 08 - Fan 2
// //         // AA 55 01 04 00 01 03 09 - Fan 3
// //         // AA 55 01 04 00 01 04 0A - Light
// //         // AA 55 01 06 00 01 01 09 - Buzzer
// //         // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
// //         AddLogSerial(LOG_LEVEL_DEBUG);
// //         uint8_t crc = 0;
// //         for (uint32_t i = 2; i < 7; i++) {
// //           crc += serial_in_buffer[i];
// //         }
// //         if (crc == serial_in_buffer[7]) {
// //           SonoffIfanReceived();
// //           ifan_receive_flag = false;
// //           return true;
// //         }
// //       }
// //       serial_in_byte = 0;
// //     }
// //     return false;
// //   }
// // }

// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void mStatusLEDs::CmndFanspeed(void)
// {
//   // if (XdrvMailbox.data_len > 0) {
//   //   if ('-' == XdrvMailbox.data[0]) {
//   //     XdrvMailbox.payload = (int16_t)GetFanspeed() -1;
//   //     if (XdrvMailbox.payload < 0) { XdrvMailbox.payload = MAX_FAN_SPEED -1; }
//   //   }
//   //   else if ('+' == XdrvMailbox.data[0]) {
//   //     XdrvMailbox.payload = GetFanspeed() +1;
//   //     if (XdrvMailbox.payload > MAX_FAN_SPEED -1) { XdrvMailbox.payload = 0; }
//   //   }
//   // }
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_FAN_SPEED)) {
//   //   SetFanspeed(XdrvMailbox.payload, true);
//   // }
//   // ResponseCmndNumber(GetFanspeed());
// }

// /*********************************************************************************************/

// void mStatusLEDs::init(void)
// {
//   // if (SONOFF_IFAN03 == my_module_type) {
//   //   SetSerial(9600, TS_SERIAL_8N1);
//   // }
//   // return false;  // Continue init chain

//   analogWrite(pin, pwm_range_min);

// }

// void mStatusLEDs::SonoffIfanUpdate(void)
// {
//   // if (SONOFF_IFAN03 == my_module_type) {
//     if (ifan_fanspeed_timer) {
//       ifan_fanspeed_timer--;
//       if (!ifan_fanspeed_timer) {
//         SetFanspeed(ifan_fanspeed_goal, false);
//       }
//     }
//   // }

//   // if (ifan_restart_flag && (4 == pCONT_time->uptime.seconds_nonreset) && (SONOFF_IFAN02 == my_module_type)) {  // Microcontroller needs 3 seconds before accepting commands
//   //   ifan_restart_flag = false;
//   //   SetDevicePower(1, SRC_RETRY);      // Sync with default power on state microcontroller being Light ON and Fan OFF
//   //   SetDevicePower(pCONT_set->power, SRC_RETRY);  // Set required power on state
//   // }
// }


// void mStatusLEDs::pre_init(){
  
//   if(pCONT_pins->PinUsed(GPIO_FAN_PWM1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_PWM1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

// }

int8_t mStatusLEDs::Tasker(uint8_t function){

  // /************
  //  * INIT SECTION * 
  // *******************/
  // if(function == FUNC_PRE_INIT){
  //   pre_init();
  // }else
  // if(function == FUNC_INIT){
  //   init();
  // }

  // // Only continue in to tasker if module was configured properly
  // // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  // switch(function){
  //   /************
  //    * PERIODIC SECTION * 
  //   *******************/
  //   case FUNC_EVERY_SECOND:    

  //   break;
  //   case FUNC_EVERY_250_MSECOND:
  //     SonoffIfanUpdate();
  //   break;
  //   /************
  //    * COMMANDS SECTION * 
  //   *******************/
  //   case FUNC_COMMAND:

  //   break;  
  //     //case FUNC_SERIAL:
  //     //result = SonoffIfanSerialInput();
  //     //break;
  //   /************
  //    * MQTT SECTION * 
  //   *******************/
  //   case FUNC_MQTT_HANDLERS_INIT:
  //     MQTTHandler_Init();
  //   break;
  //   case FUNC_MQTT_HANDLERS_RESET:
  //     MQTTHandler_Init();
  //   break;
  //   case FUNC_MQTT_SENDER:
  //     MQTTHandler_Sender(); //optional pass parameter
  //   break;
  //   case FUNC_MQTT_CONNECTED:
  //     MQTTHandler_Set_fSendNow();
  //   break;
  // }
  
  // /************
  //  * WEBPAGE SECTION * 
  // *******************/
  
  // #ifdef USE_MODULE_CORE_WEBSERVER
  // return Tasker_Web(function);
  // #endif // USE_MODULE_CORE_WEBSERVER

} // END Tasker


// int8_t mStatusLEDs::Tasker(uint8_t function, JsonObjectConst obj){
//   switch(function){
//     case FUNC_JSON_COMMAND_OBJECT:
//       parse_JSONCommand(obj);
//     break;
//     case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
//       return CheckAndExecute_JSONCommands(obj);
//     break;
//   }
// }
// int8_t mStatusLEDs::CheckAndExecute_JSONCommands(JsonObjectConst obj){

//   // Check if instruction is for me
//   // if(mSupport::CheckSetTopicIsModulebyID())
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/pwmfan")>=0){
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand(obj);
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }

// void mStatusLEDs::parse_JSONCommand(JsonObjectConst obj){

//   // Serial.println("mStatusLEDs::parsesub_Commands(JsonObjectConst obj)");

  

//   if(obj.containsKey(D_JSON_LIGHTPOWER)){
//     int light = obj[D_JSON_LIGHTPOWER];
//     // if(light == 2){
//     //   SetLightState(!GetLightState());
//     // }else{
//     //   SetLightState(light);      
//     // }
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_LIGHTPOWER,GetLightState()?"On":"Off");
//     // Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_LIGHTPOWER,D_TOGGLE);
//     // isserviced++;  
//   }

//   // FanSpeed = 0,1,2,3 ie off,low,med,high
//   if(obj.containsKey(D_JSON_FANSPEED)){
//     int speed = obj[D_JSON_FANSPEED];
//     if(speed>3){
//       Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
//       speed=0; //default off
//     }      

//     int pwm_val = map(speed, 0, 3, pwm_range_min, pwm_range_max);

//     set_fan_speed  = speed;
//     set_fan_pwm = pwm_val;

//     analogWrite(pin,pwm_val);


//     // SetFanspeed(speed, false);
//     // AddLog_P(LOG_LEVEL_INFO,PSTR("GetFanspeed TEST=%d"),GetFanspeed());
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANPWM,pwm_val);
//     // Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
//     // isserviced++;
//   }

//   //FanPWM = 0-1023
//   if(obj.containsKey(D_JSON_FANPWM)){
//     int pwm_val = obj[D_JSON_FANPWM];
//     // if(speed>3){
//     //   Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANPWM,D_PARSING_NOMATCH);
//     //   speed=0; //default off
//     // }      
    
//     set_fan_speed  = map(pwm_val, pwm_range_min, pwm_range_max, 0, 3);;
//     set_fan_pwm = pwm_val;

//     analogWrite(pin,set_fan_pwm);

//     // SetFanspeed(speed, false);
//     AddLog_P(LOG_LEVEL_INFO,PSTR("GetFanspeed TEST=%d"),GetFanspeed());
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANPWM,set_fan_pwm);
//     Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANPWM,set_fan_pwm);
//     // isserviced++;
//   }

//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
  
  

// }




// int8_t mStatusLEDs::Tasker_Web(uint8_t function){



//   switch(function){
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       // create command list
//       char dlist[100]; memset(dlist,0,sizeof(dlist));
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

      
//       // Slider (put tester flag around this as optional)
      
//       BufferWriterI->Append_P(PSTR("<div> Fan PWM </div>"));
//       BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//         WEB_HANDLE_PWM_SLIDER,               // c - Unique HTML id
//         PSTR("#ddd"), PSTR("#eee"),   // Black to White
//         4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//         0, 1023,  // Range 0/1 to 100%
//         set_fan_pwm,
//         WEB_HANDLE_PWM_SLIDER "tobechanged"
//       );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));


//       uint8_t button_values[4] = {0, 1, 2, 3}; //toggle, fanspeed0-3
          
//       BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Fan Speed"),"");

//       char button_value_ctr[10];
//       char button_key_ctr[50];
//       char button_text_ctr[30];

//       BufferWriterI->Append_P(PSTR("{t}<tr>"));
//         for(uint8_t button_id=0;button_id<4;button_id++){
//           BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2, 
//                                     100/4,
//                                     "", 
//                                     "buttonh",
//                                     pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
//                                     pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
//                                     pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls_pwm),
//                                     ""
//                                   );
//         }
//       BufferWriterI->Append_P(PSTR("</tr>{t2}"));

// //new json template test

//       // create command list
//       char dlist2[200]; memset(dlist2,0,sizeof(dlist2));
//       // pCONT_sup->AppendDList(dlist2, "{\\\"device\\\":\\\"Driveway Light\\\",\\\"onoff\\\":\\\"%s\\\"}");
      
//       pCONT_sup->AppendDList(dlist2, sizeof(dlist2), "{\\\"" D_JSON_LIGHTPOWER "\\\":\\\"%s\\\",\\\"" D_JSON_FANSPEED "\\\":\\\"%s\\\"}","2","%s");
//       pCONT_sup->AppendDList(dlist2, sizeof(dlist2), "{\\\"device\\\":\\\"%s\\\",\\\"onoff\\\":\\\"%s\\\"}","Garden Light","%s");
//       // pCONT_sup->AppendDList(dlist2, sizeof(dlist2), "%s %s","Garden Light 2","test");
//       // pCONT_sup->AppendDList_P(dlist2, sizeof(dlist2), "{\\\"device\\\":\\\"%s\\\",\\\"onoff\\\":\\\"%s\\\"}\0","Garden Light 2","%s");

//       Serial.println(dlist2);

//       pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), 0, dlist2);
//       AddLog_P(LOG_LEVEL_INFO, PSTR("button_key_ctr dlist2 %s"), button_key_ctr);

//       pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), 1, dlist2);
//       AddLog_P(LOG_LEVEL_INFO, PSTR("button_key_ctr dlist2 %s"), button_key_ctr);

//       // break;
//       uint8_t button_values2[2] = {2, 2}; //toggle, fanspeed0-3
          
//       BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("JSON Test"),"");

//       BufferWriterI->Append_P(PSTR("{t}<tr>"));
//         for(uint8_t button_id=0;button_id<2;button_id++){
//           /*"<td{sw1}
//           %d%%'{cs}
//           %s'{bc}'
//           %s'{djt}
//           %s'{va}
//           %s'>
//           %s
//           %s{bc2}"*/
//           BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_KEY_TEMPLATED_VARIABLE_INSERTS_HANDLE_IHR2, 
//                                     100/4,
//                                     "", 
//                                     "buttonh",
//                                     // dlist2,
//                                     pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist2), 
//                                     pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values2[button_id]),
//                                     pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls_pwm),
//                                     ""
//                                   );
//         }
//       BufferWriterI->Append_P(PSTR("</tr>{t2}"));







//     }break;
//   }

// }



// uint8_t mStatusLEDs::ConstructJSON_Settings(uint8_t json_method){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P("test",0);  
//   JsonBuilderI->End();

// }

// uint8_t mStatusLEDs::ConstructJSON_Sensor(uint8_t json_method){

//   JsonBuilderI->Start();
//     // JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
//     JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
//     JsonBuilderI->Add_P(D_JSON_FANPWM, set_fan_pwm);  
//   JsonBuilderI->End();

// }




// /*********************************************************************************************************************************************
// ******** MQTT **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mStatusLEDs::MQTTHandler_Init(){

//   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mStatusLEDs::ConstructJSON_Settings;

//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mStatusLEDs::ConstructJSON_Sensor;

//   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mStatusLEDs::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// void mStatusLEDs::MQTTHandler_Set_fSendNow(){

//   mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_teleperiod.flags.SendNow = true;

// } //end "MQTTHandler_Init"


// void mStatusLEDs::MQTTHandler_Set_TelePeriod(){

//   // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// } //end "MQTTHandler_Set_TelePeriod"


// void mStatusLEDs::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t mqtthandler_list_ids[] = {
//     MQTT_HANDLER_SETTINGS_ID, 
//     MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
//     MQTT_HANDLER_SENSOR_TELEPERIOD_ID
//   };
  
//   struct handler<mSensorsDHT>* mqtthandler_list_ptr[] = {
//     &mqtthandler_settings_teleperiod,
//     &mqtthandler_sensor_ifchanged,
//     &mqtthandler_sensor_teleperiod
//   };

//   pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_DHT_ID,
//     mqtthandler_list_ptr, mqtthandler_list_ids,
//     sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
//     mqtt_handler_id
//   );
//   uint8_t flag_handle_all = false, handler_found = false
//   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

//   // change switch to use array of pointers?
//   do{

//     switch(mqtt_handler_id){
//       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
//       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
//       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
//       default: handler_found=false; break; // nothing 
//     } // switch

//     // Pass handlers into command to test and (ifneeded) execute
//     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_CUSTOM_PWM_FAN_ID,mqtthandler_ptr); }

//     // stop searching
//     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

//   }while(flag_handle_all);

// }

// ////////////////////// END OF MQTT /////////////////////////


#endif
