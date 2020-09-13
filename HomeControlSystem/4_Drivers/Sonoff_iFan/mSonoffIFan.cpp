#include "mSonoffIFan.h"

//  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Tasmota

#ifdef USE_SONOFF_IFAN

/*********************************************************************************************\
  Sonoff iFan02 and iFan03
\*********************************************************************************************/

/*********************************************************************************************/

// bool mSonoffIFan::IsModuleIfan(void)
// {
//   return ((SONOFF_IFAN02 == my_module_type) || (SONOFF_IFAN03 == my_module_type));
// }

// uint8_t mSonoffIFan::MaxFanspeed(void)
// {
//   return MAX_FAN_SPEED;
// }

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
    uint8_t fanspeed = (uint8_t)(
      
      
      pCONT_set->power &0xF) >> 1;
    if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
    return fanspeed;
  }
}


// Probably to be handled using "light_interface?"
uint8_t mSonoffIFan::GetLightState(void)
{
  return pCONT_mry->GetRelay(0);
}
void mSonoffIFan::SetLightState(uint8_t state)
{
  pCONT_mry->SetRelay(state);
}

/*********************************************************************************************/

void mSonoffIFan::SonoffIFanSetFanspeed(uint8_t fanspeed, bool sequence)
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
  for (uint32_t i = 2; i < 5; i++) {
    uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
    pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4
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

// bool mSonoffIFan::SonoffIfanSerialInput(void)
// {
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
// }

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void mSonoffIFan::CmndFanspeed(void)
{
  // if (XdrvMailbox.data_len > 0) {
  //   if ('-' == XdrvMailbox.data[0]) {
  //     XdrvMailbox.payload = (int16_t)GetFanspeed() -1;
  //     if (XdrvMailbox.payload < 0) { XdrvMailbox.payload = MAX_FAN_SPEED -1; }
  //   }
  //   else if ('+' == XdrvMailbox.data[0]) {
  //     XdrvMailbox.payload = GetFanspeed() +1;
  //     if (XdrvMailbox.payload > MAX_FAN_SPEED -1) { XdrvMailbox.payload = 0; }
  //   }
  // }
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_FAN_SPEED)) {
  //   SonoffIFanSetFanspeed(XdrvMailbox.payload, true);
  // }
  // ResponseCmndNumber(GetFanspeed());
}

/*********************************************************************************************/

bool mSonoffIFan::SonoffIfanInit(void)
{
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain
}

void mSonoffIFan::SonoffIfanUpdate(void)
{
  // if (SONOFF_IFAN03 == my_module_type) {
  //   if (ifan_fanspeed_timer) {
  //     ifan_fanspeed_timer--;
  //     if (!ifan_fanspeed_timer) {
  //       SonoffIFanSetFanspeed(ifan_fanspeed_goal, false);
  //     }
  //   }
  // }

  // if (ifan_restart_flag && (4 == uptime) && (SONOFF_IFAN02 == my_module_type)) {  // Microcontroller needs 3 seconds before accepting commands
  //   ifan_restart_flag = false;
  //   SetDevicePower(1, SRC_RETRY);      // Sync with default power on state microcontroller being Light ON and Fan OFF
  //   SetDevicePower(power, SRC_RETRY);  // Set required power on state
  // }
}


int8_t mSonoffIFan::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  // if(function == FUNC_PRE_INIT){
  //   pre_init();
  // }else
  // if(function == FUNC_INIT){
  //   init();
  // }

  // Only continue in to tasker if module was configured properly
  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
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
    case FUNC_EVERY_SECOND:    

      // if(mTime::TimeReached(&tSaved_test, 10000)){

      //   if(test_speed++ > 3){
      //     test_speed = 0;
      //   }

      //     SonoffIFanSetFanspeed(test_speed, false);

      //     AddLog_P(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());

      // } 

    break;
    /************
     * FUNCTION HANDLER SECTION * 
    *******************/
    // case FUNC_FUNCTION_LAMBDA_INIT:
    //   FunctionHandler_Init();
    // break;
    // case FUNC_FUNCTION_LAMBDA_LOOP:       //calls from here, into support requiring no object
    //   FunctionHandler_Loop();
    // break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;  
    /************
     * MQTT SECTION * 
    *******************/
    // case FUNC_MQTT_HANDLERS_INIT:
    //   MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    // break;
    // case FUNC_MQTT_HANDLERS_RESET:
    //   // Reset to the initial parameters
    // break;
    // case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
    //   //MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    // break;
    // case FUNC_MQTT_SENDER:
    //   MQTTHandler_Sender(); //optional pass parameter
    // break;
    // case FUNC_MQTT_CONNECTED:
    //   MQTTHandler_Set_fSendNow();
    // break;


    
//     switch (function) {
//       case FUNC_EVERY_250_MSECOND:
//         SonoffIfanUpdate();
//         break;
//       case FUNC_SERIAL:
//         result = SonoffIfanSerialInput();
//         break;
//       case FUNC_COMMAND:
//         result = DecodeCommand(kSonoffIfanCommands, SonoffIfanCommand);
//         break;
//       case FUNC_MODULE_INIT:
//         result = SonoffIfanInit();
//         break;
//     }



  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  return Tasker_Web(function);

} // END Tasker


int8_t mSonoffIFan::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_Commands(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}
int8_t mSonoffIFan::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/ifan")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_Commands(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}
int8_t mSonoffIFan::parsesub_Commands(JsonObjectConst obj){

  int8_t isserviced = 0;

  if(obj.containsKey(D_JSON_LIGHTPOWER)){
    int light = obj[D_JSON_LIGHTPOWER];
    if(light == 2){
      SetLightState(!GetLightState());
    }else{
      SetLightState(light);      
    }
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_LIGHTPOWER,GetLightState()?"On":"Off");
    Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_LIGHTPOWER,D_TOGGLE);
    isserviced++;  
  }


  if(obj.containsKey(D_JSON_FANSPEED)){
    int speed = obj[D_JSON_FANSPEED];
    if(speed>3){
      Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
      speed=0; //default off
    }      
    SonoffIFanSetFanspeed(speed, false);
    AddLog_P(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
    Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
    isserviced++;
  }
  
  return isserviced;

}




int8_t mSonoffIFan::Tasker_Web(uint8_t function){
switch(function){
    case FUNC_WEB_APPEND_ROOT_BUTTONS:{
      
      uint8_t button_count = 5;
      uint8_t button_event = 0;

      // create command list
      char dlist[100]; memset(dlist,0,sizeof(dlist));
      pCONT_sup->AppendDList(dlist, D_JSON_LIGHTPOWER);
      pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
      pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
      pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
      pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

      uint8_t button_values[5] = {2, 0, 1, 2, 3}; //toggle, fanspeed0-3
          
      BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Ceiling Fan Controls"),"");

      char button_value_ctr[10];
      char button_key_ctr[50];
      char button_text_ctr[30];

      BufferWriterI->Append_P(PSTR("{t}<tr>"));
        for(uint8_t button_id=0;button_id<button_count;button_id++){
          BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_HANDLE_IHR, 
                                    100/(button_id==0?1:4), 
                                    "button_hac " "fanbut",
                                    pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
                                    pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
                                    pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls),
                                    ""
                                );
          // LightPower button gets its own row
          if(button_id==0){ BufferWriterI->Append_P(PSTR("</tr><tr>")); }
        }
      BufferWriterI->Append_P(PSTR("</tr>{t2}"));

    }break;
  }

}



void mSonoffIFan::WebAppend_Root_Draw_PageTable(){
  
  // if(settings.fShowTable){
    // for(int ii=0;ii<5;ii++){
    //   pCONT_web->AppendBuffer_PI2(PSTR("<tr>"));
    //     // pCONT_web->AppendBuffer_PI2(PSTR("<td>%s</td>"), GetRelayNamebyIDCtr(ii));
    //     pCONT_web->AppendBuffer_PI2(PSTR("<td><div class='%s'>%s</div></td>"),"cfpow_tab","?");   
    //   pCONT_web->AppendBuffer_PI2(PSTR("</tr>"));
    // }    
  // }

}


//append to internal buffer if any root messages table
void mSonoffIFan::WebAppend_Root_Status_Table(){
  
  // if(settings.fShowTable){
  //   pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("relpow_tab")); 
  //   for(int row=0;row<relays_connected;row++){
  //     switch(row){
  //       default:
  //         pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"ih\":\"%s\"},"),row,
  //           GetRelay(row) ? PSTR("ON") : PSTR("Off")
  //         );
  //       break;
  //     }
  //   }
  //   *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  //   pCONT_web->AppendBuffer_PI2(PSTR("]")); 
  //   pCONT_web->AppendBuffer_PI2(PSTR(","));
  // }

  // pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("fanbut")); 
  // for(int row=0;row<relays_connected;row++){
  //   switch(row){
  //     default:
  //       char buffer[20]; memset(buffer,0,sizeof(buffer));
  //       pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"bc\":\"%s\"},"),row,
        
  //       // ,\"ih\":\"%s\"},"),row,
  //         // "#ee2200",//GetRelay(row) ? "#00ff00" : "#ee2200",
  //         // GetRelayNameWithStateLongbyIDCtr(buffer, row)       
  //       );
  //     break;
  //   }
  // }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // pCONT_web->AppendBuffer_PI2(PSTR("]")); 
  // pCONT_web->AppendBuffer_PI2(PSTR(","));

}







#endif
