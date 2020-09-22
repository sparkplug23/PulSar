#include "mRelays.h"

#ifdef USE_MODULE_DRIVERS_RELAY

//minimal
/*
1) MQTT control, including minutes on
2) Report relay status correctly
3) Button controls work, toggling
4) Save power state in settings
5) Restore power state from settings on boot
6) Enable mode to start with, on, off, as saved
7) Schedule automatic tod on/off, include duty cycle (with variable for scele, seconds, minutes, hours)


add table (optional flag to turn this on/off)
Relay Name:    ontime, offtime, timeon, last controlled by



*/





//
  // I should add a relay "type" ie, external power, internal power, light






void mRelays::init(void){

  relays_connected = pCONT_set->devices_present; //phase out

  relays_connected = RELAYS_CONNECTED;
  
  SetPowerOnState();
}


//overload fix when only one parameter is called
// int8_t mRelays::Tasker2(uint8_t function, uint8_t param1){  
//   JsonObjectConst dummy; return Tasker(function, dummy);
// }
// template<typename T>
// int8_t mRelays::Tasker2(uint8_t function, T param1){  


// }

// template<typename T>
// void f()
// {
//     // BARK;
// }

// template void f<int>();

// template<typename T>
// void mRelays::ftest()
// {
//     // BARK;
//     Serial.println("bark\n\rbark\n\rbark\n\rbark\n\rbark\n\rbark\n\rbark\n\r");
// }

// template<typename T>
// void mRelays::ftest2(T value)
// {
//     // BARK;
//     Serial.print("bark\n\rbark\n\rbark\n\rbark\n\rbark\n\rbark\n\rbark\n\r");
// }



// int8_t mRelays::Tasker2(uint8_t function, uint8_t param1){  
//   JsonObjectConst dummy; return Tasker(function, dummy);
// }
// template <typename T>
// int8_t mRelays::Tasker2(uint8_t function, T param1){

//   switch(function){
//     case FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD:
//       //parsesub_TopicCheck_JSONCommand(param1);
//     break;
//     case FUNC_INIT:
//       init();
//     break;
//   }

// }


// Used for timed on or off events
int8_t mRelays::Tasker(uint8_t function){

  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      //pre_init();
    break;
    case FUNC_INIT:
      init();
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      // if(mSupport::TimeReached(&tSavedTest,1000)){
      //   // SetAllPower(POWER_TOGGLE,SRC_IGNORE);
      // }
    break;     
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;
    case FUNC_JSON_COMMAND:
      // function_result = CheckAndExecute_JSONCommand();
    break;
    // case FUNC_SET_POWER:
    //   // LightSetPower();
    // break;    
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTConnected();
    break;
    #endif    
    /************
     * WEBPAGE SECTION * 
    *******************/
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;  
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Draw_PageTable();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:
      WebAppend_Root_Add_Buttons();
    break;
  } // end switch
} // END function

int8_t mRelays::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_TopicCheck_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


void mRelays::MQTTConnected(){
  MQTTHandler_Set_fSendNow();
}

void mRelays::WebAppend_Root_Add_Buttons(){
  DEBUG_LINE;
  char relay_handle_ctr[10]; 
  char buffer[30];

Serial.println("WebAppend_Root_Add_Buttons");

  JsonBuilderI->Append_P(PSTR("%s"),PSTR("{t}<tr>"));
  for(uint8_t relay_id=0;relay_id<relays_connected;relay_id++){
    memset(relay_handle_ctr,0,sizeof(relay_handle_ctr));
    sprintf(relay_handle_ctr,"reltog%02d",relay_id);
    JsonBuilderI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE, 
                                  100/relays_connected, 
                                  "buttonh ""reltog",
                                  relay_handle_ctr, 
                                  DEVICE_CONTROL_BUTTON_TOGGLE_ID, 
                                  GetRelayNamebyIDCtr(relay_id, buffer, sizeof(buffer)), ""
                                );
  }
  JsonBuilderI->Append_P("%s",PSTR("</tr>{t2}"));
}



void mRelays::WebAppend_Root_Draw_PageTable(){

  char buffer[50];
  
  DEBUG_LINE;
  if(settings.fShowTable){
    for(int ii=0;ii<relays_connected;ii++){
      BufferWriterI->Append_P(PSTR("<tr>"));
        BufferWriterI->Append_P(PSTR("<td>%s</td>"), GetRelayNamebyIDCtr(ii,buffer,sizeof(buffer)));
        BufferWriterI->Append_P(PSTR("<td><div class='%s'>%s</div></td>"),"relpow_tab","?");   
      BufferWriterI->Append_P(PSTR("</tr>"));
    }    
  }

}


//append to internal buffer if any root messages table
void mRelays::WebAppend_Root_Status_Table(){
  
  if(settings.fShowTable){
    // pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("relpow_tab")); 
    
  JsonBuilderI->Array_Start("relpow_tab");// Class name

    for(int row=0;row<relays_connected;row++){
      
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);

      switch(row){
        default:
        JsonBuilderI->Add("ih",GetRelay(row) ? PSTR("ON") : PSTR("Off"));
          // pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"ih\":\"%s\"},"),row,
          //   GetRelay(row) ? PSTR("ON") : PSTR("Off")
          // );
        break;
      }
    }
    // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
    // pCONT_web->AppendBuffer_PI2(PSTR("]")); 
    // pCONT_web->AppendBuffer_PI2(PSTR(","));
    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_Start("reltog");// Class name

  // pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("reltog")); 
  for(int row=0;row<relays_connected;row++){
    
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);

    switch(row){
      default:
        char buffer[20]; memset(buffer,0,sizeof(buffer));
        JsonBuilderI->Add("ih",GetRelayNameWithStateLongbyIDCtr(row, buffer));
        JsonBuilderI->Add("bc",GetRelay(row) ? "#00ff00" : "#ee2200");
        
        // pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"bc\":\"%s\",\"ih\":\"%s\"},"),row,
        //   GetRelay(row) ? "#00ff00" : "#ee2200",
        //   GetRelayNameWithStateLongbyIDCtr(row, buffer)       
        // );
      break;
    }
    
    JsonBuilderI->Level_End();
  }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // pCONT_web->AppendBuffer_PI2(PSTR("]")); 
  // pCONT_web->AppendBuffer_PI2(PSTR(","));
  
  JsonBuilderI->Array_End();
  

}


void mRelays::WebPage_Root_SendStatus(){

  /****
   *  Show LED power 
   * ****/
  // uint8_t onoff = fAnyLEDsOnOffCount;
  // uint8_t fsize = 32;
  // char onoff_ctr[30];
  // if(onoff){
  //   sprintf(onoff_ctr,PSTR("Lights ON (%d)"),fAnyLEDsOnOffCount);
  // }else{
  //   sprintf(onoff_ctr,PSTR("Lights OFF"));
  // }
  // pCONT_web->WSBufferAppend_PI2(PSTR("<tr>"));
  // pCONT_web->WSBufferAppend_PI2(HTTP_DEVICE_STATE, 
  //   100 / 1, //FULL WIDTH 1 item for length
  //   onoff ? "bold" : "normal", 
  //   fsize, 
  //   onoff_ctr
  // );
  // pCONT_web->WSBufferAppend_PI2(PSTR("</tr>"));

  // // Animation info
  // memset(onoff_ctr,0,sizeof(onoff_ctr));
  // fsize = 20;
      
}


void mRelays::WebCommand_Parse(void)
{
  
  if(pCONT_web->request_web_command == nullptr){ return; }

  char tmp[100];

  uint8_t  arg_value = 0;
  char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

  for(uint8_t relay_id=0;relay_id<relays_connected;relay_id++){
    
    sprintf(arg_ctr,"%s%02d","reltog",relay_id);

    if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
      pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
      arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
      // animation.brightness = BrtN2F(arg_value);
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %d"),arg_ctr,arg_value,relay_id);
      // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
      // SetRefreshLEDs();

      ExecuteCommandPower(relay_id,POWER_TOGGLE,SRC_MQTT);
    }

  }


}



/*********************************************************************************************
* MQTT ******************************************************************
*************************************************************************************************/


//relay:1,[1,2],"all",["fountain,bulbs,led"]
// void mRelays::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/relays")>=0){
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return; // not meant for here
//   }

//   // AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer2.payload.ctr);

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
//   JsonObjectConst obj = doc.as<JsonObject>();

//   parsesub_TopicCheck_JSONCommand(obj);

// }


int8_t mRelays::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/relays")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mRelays::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

  AddLog_P(LOG_LEVEL_INFO,PSTR("mRelays::parsesub_TopicCheck_JSONCommand"));

  uint8_t name_num=-1,state=-1;    

  if(!obj[F(D_JSON_NAME)].isNull()){
    if(const char* value = obj["name"]){
      name_num = GetRelayIDbyName(value);
    }else 
    if(obj["name"].is<int>()){
      name_num  = obj["name"];
    }
  }
  // else{
  //   name_num = 0; // assume relay 0
  // }

  if(!obj[F(D_JSON_ONOFF)].isNull()){
    if(const char* value = obj["onoff"]){
      state = pCONT_sup->GetStateNumber(value);
    }else 
    if(obj["onoff"].is<int>()){
      state  = obj["onoff"];
    }    
  }

  if((state>-1)&&(name_num>-1)){
    SetRelay(name_num,state);
  }


  
  // if(!obj[D_JSON_POWER].isNull()){
  //   if(const char* value = obj[D_JSON_POWER]){
  //     state = pCONT_sup->GetStateNumber(value);
  //   }else 
  //   if(obj[D_JSON_POWER].is<int>()){
  //     state  = obj[D_JSON_POWER];
  //     if(state==2){
  //       state ^= GetRelay(name_num); //invert
  //     }
  //   }    
  // }

  // if(state>-1){
  //   SetRelay(name_num,state);
  // }


  //create parsing function
  // id = ParseJsonObjectValue(function, key, )

}


/*********************************************************************************************
* Input & Output Interface ******************************************************************
*************************************************************************************************/

//tmp fix
void mRelays::SetRelay(uint8_t state){
  SetRelay(0,state);
}

  //flip args around so the single arg means state of relay 0
void mRelays::SetRelay(uint8_t num, uint8_t state){

  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " " D_JSON_COMMAND_NVALUE " " D_JSON_COMMAND_NVALUE)
    ,"SetRelay","num",num,"state",state);     
  
  //error patch  within vaible range
  if(!((num>=0)&&(num<=relays_connected))){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " Command Invalid"),"SetRelay");
    return;
  }

  if(GetRelay(num)==state){
    relay_status[num].ischanged = false;
    return;
  }else{
    relay_status[num].ischanged = true;
  }

  relay_status[num].onoff = state;

  if(state){ relay_status[num].ontime = pCONT->mt->mtime;
  }else{ relay_status[num].offtime = pCONT->mt->mtime; }


  ExecuteCommandPower(num,state,SRC_MQTT);

}

uint8_t mRelays::GetRelay(uint8_t num){
  // num--;
  return bitRead(pCONT_set->power, num);
}

/*********************************************************************************************
* HELPER & CONVERSION FUNCTIONS ******************************************************************
*************************************************************************************************/

const char* mRelays::GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  DEBUG_LINE;
  if(device_id >= relays_connected){ 
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_RELAYS "device_id >= relays_connected %d %d"),device_id,relays_connected);
    return PM_SEARCH_NOMATCH; 
  }
  DEBUG_LINE;
  return pCONT_set->GetDeviceName(D_MODULE_DRIVERS_RELAY_ID, device_id, buffer, buffer_length);
}

const char* mRelays::GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  
  char onoffctr[5];
  switch(GetRelay(device_id)){
    case 0: sprintf(onoffctr,"%s","OFF"); break;
    case 1: sprintf(onoffctr,"%s","ON");  break;
  }

  char buffer_internal[50];
  sprintf(buffer, "%s %s", GetRelayNamebyIDCtr(device_id, buffer_internal), onoffctr);

  return buffer;
}



int8_t mRelays::GetRelayIDbyName(const char* c){
  if(c=='\0'){ return -1; }  

  int8_t device_id;
  int8_t class_id = D_MODULE_DRIVERS_RELAY_ID;

  int16_t device_id_found = pCONT_set->GetDeviceIDbyName(c,pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
  AddLog_P(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

  return device_id_found;

}





/********************************************************************************************/

void mRelays::SetLatchingRelay(power_t lpower, uint32_t state)
{
  // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On

  if (state && !pCONT_set->latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    pCONT_set->latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
    pCONT_pins->DigitalWrite(GPIO_REL1_ID + port, bitRead(rel_inverted, port) ? !state : state);
  }
}

void mRelays::SetDevicePower(power_t rpower, uint32_t source)
{

  DEBUG_LINE;

  pCONT_sup->ShowSource(source);
  pCONT_set->last_source = source;
DEBUG_LINE;
  
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS "SetDevicePower(%d,%d)"),rpower,source);

  if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {  // All on and stay on
    pCONT_set->power = (1 << pCONT_set->devices_present);// -1;
    rpower = pCONT_set->power;
  }

  // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
  // if (pCONT_set->Settings.flag_system_phaseout.interlock) {        
  //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
  //     power_t mask = 1;
  //     uint32_t count = 0;
  //     for (uint32_t j = 0; j < pCONT_set->devices_present; j++) {
  //       if ((pCONT_set->Settings.interlock[i] & mask) && (rpower & mask)) {
  //         count++;
  //       }
  //       mask <<= 1;
  //     }
  //     if (count > 1) {
  //       mask = ~pCONT_set->Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
  //       power &= mask;
  //       rpower &= mask;
  //     }
  //   }
  // }
DEBUG_LINE;
  
  if (rpower) {                           // Any power set
    last_power = rpower;
  }

  // PHASE OUT and replace with something else
  // pCONT_set->XdrvMailbox.index = rpower;
  // pCONT->Tasker_Interface(FUNC_SET_POWER);               // Signal power state
  // pCONT_set->XdrvMailbox.index = rpower;
  // pCONT_set->XdrvMailbox.payload = source;

  // if (pCONT->Tasker_Interface(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
  //   // Serviced
  // }
  // else if ((MODULE_SONOFF_DUAL == pCONT_set->my_module_type) || (MODULE_CH4 == pCONT_set->my_module_type)) {
  //   Serial.write(0xA0);
  //   Serial.write(0x04);
  //   Serial.write(rpower &0xFF);
  //   Serial.write(0xA1);
  //   Serial.write('\n');
  //   Serial.flush();
  // }
  // else if (MODULE_EXS_RELAY == pCONT_set->my_module_type) {
  //   SetLatchingRelay(rpower, 1);
  // }
  // else {


    #ifdef USE_VIRTUAL_REMOTE_URL_RELAY

      char remote_url[100];
      // URL
      // sprintf(remote_url, "http://%s/json_command.json", VIRTUAL_DEVICE_MDNS_NAME);
      //MQTT
      sprintf(remote_url, "%s/set/relays", VIRTUAL_DEVICE_MDNS_NAME);

      power_t state = rpower &1;
      uint8_t state_level = bitRead(rel_inverted, 0) ? !state : state;

      // State  is controlled remotely, so we can only toggle, until I use HTTP_GET to remain in sync
      char remote_command[100];
      sprintf(remote_command,"{\"relay\":0,\"onoff\":2}");//,state_level);

      AddLog_P(LOG_LEVEL_INFO, PSTR("Sending USE_VIRTUAL_REMOTE_URL_RELAY"));

      pCONT_mqtt->ppublish(remote_url,remote_command,false);

      // AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
      // response->addHeader("Server","ESP Async Web Server");
      // request->send(response);

      return; // not local control

    #endif




    for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
      power_t state = rpower &1;
      if (i < MAX_RELAYS) {
        AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "i=%d,state=%d"),i,state);
        pCONT_pins->DigitalWrite(GPIO_REL1_ID +i, bitRead(rel_inverted, i) ? !state : state);

        // #ifdef ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS // leave mums, work on another
        //   uint8_t led_pin = 13;
        //   pinMode(led_pin,OUTPUT);
        //   digitalWrite(led_pin,!state);
        //   led_pin = 2;
        //   pinMode(led_pin,OUTPUT);
        //   digitalWrite(led_pin,!state);
        // #endif

      }else{
        AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "ELSE i=%d,state=%d"),i,state);
      }
      rpower >>= 1;
    }
  //}
}

void mRelays::RestorePower(bool publish_power, uint32_t source)
{
  if (pCONT_set->power != last_power) {
    SetDevicePower(last_power, source);
    if (publish_power) {
      mqtthandler_sensor_teleperiod.fSendNow = true;
      mqtthandler_sensor_ifchanged.fSendNow = true;
    }
  }
}


void mRelays::SetAllPower(uint32_t state, uint32_t source)
{
  // state 0 = POWER_OFF = Relay Off
  // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
  // state 2 = POWER_TOGGLE = Toggle relay
  // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
  // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
  // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
  // state 16 = POWER_SHOW_STATE = Show power state

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if ((state >= POWER_OFF) && (state <= POWER_TOGGLE)) {
    power_t all_on = (1 << pCONT_set->devices_present);// -1;
    switch (state) {
    case POWER_OFF:
      pCONT_set->power = 0;
      break;
    case POWER_ON:
      pCONT_set->power = all_on;
      break;
    case POWER_TOGGLE:
      pCONT_set->power ^= all_on;                    // Complement current state
    }
    SetDevicePower(pCONT_set->power, source);
  }
  if (publish_power) {
    mqtthandler_sensor_teleperiod.fSendNow = true;
    mqtthandler_sensor_ifchanged.fSendNow = true;
  }
}

void mRelays::SetPowerOnState(void)
{
  // if (MODULE_MOTOR == pCONT_set->my_module_type) {
  //   pCONT_set->Settings.poweronstate = POWER_ALL_ON;   // Needs always on else in limbo!
  // }
  // if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {
  //   SetDevicePower(1, SRC_RESTART);
  // } else {
  //   if ((pCONT_sup->ResetReason() == REASON_DEFAULT_RST) || (pCONT_sup->ResetReason() == REASON_EXT_SYS_RST)) {
      switch (pCONT_set->Settings.poweronstate) {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        pCONT_set->power = 0;
        SetDevicePower(pCONT_set->power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        pCONT_set->power = (1 << pCONT_set->devices_present);// -1;
        SetDevicePower(pCONT_set->power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        pCONT_set->power = (pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) )) ^ POWER_MASK;
        if (pCONT_set->Settings.flag_system_phaseout.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(pCONT_set->power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        pCONT_set->power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) );
        if (pCONT_set->Settings.flag_system_phaseout.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(pCONT_set->power, SRC_RESTART);
        }
        break;
      }
    // } else {
    //   power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) );
    //   if (pCONT_set->Settings.flag_system_phaseout.save_state) {    // SetOption0 - Save power state and use after restart
    //     SetDevicePower(pCONT_set->power, SRC_RESTART);
    //   }
    // }
  //}

  // Issue #526 and #909
  // for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
  //   if (!pCONT_set->Settings.flag_network_phaseout.no_power_feedback) {  // SetOption63 - Don't scan relay pCONT_set->power state at restart - #5594 and #5663
  //     if ((i < MAX_RELAYS) && (pCONT_set->pin[GPIO_REL1 +i] < 99)) {
  //       bitWrite(pCONT_set->power, i, digitalRead(pCONT_set->pin[GPIO_REL1 +i]) ^ bitRead(pCONT_set->rel_inverted, i));
  //     }
  //   }
  //   if ((i < MAX_PULSETIMERS) && (bitRead(pCONT_set->power, i) || (POWER_ALL_OFF_PULSETIME_ON == pCONT_set->Settings.poweronstate))) {
  //     pCONT_sup->SetPulseTimer(i, pCONT_set->Settings.pulse_timer[i]);
  //   }
  // }
  blink_powersave = pCONT_set->power;
}



// void mRelays::ExecuteCommandPowerZeroIndex(uint32_t device, uint32_t state, uint32_t source){
//   AddLog_P(LOG_LEVEL_WARN, PSTR("Temporary fix ExecuteCommandPowerZeroIndex"));
//   ExecuteCommandPower(device+1, state, source);
// }

void mRelays::ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{

//device++; // I am using 0 index, tasmota used 1
// AddLog_P(LOG_LEVEL_WARN,PSTR(D_LOG_RELAYS "ExecuteCommandPower forcing \"device++\" index shift"));

// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  pCONT_sup->ShowSource(source);

// #ifdef USE_SONOFF_IFAN
//   if (IsModuleIfan()) {
//     blink_mask &= 1;                 // No blinking on the fan relays
//     Settings.flag_system_phaseout.interlock = 0;     // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
//     Settings.pulse_timer[1] = 0;     // No pulsetimers on the fan relays
//     Settings.pulse_timer[2] = 0;
//     Settings.pulse_timer[3] = 0;
//   }
// #endif  // USE_SONOFF_IFAN

  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_RELAYS "ExecuteCommandPower(device%d,state%d,source%d)=devices_present%d"),device,state,source,pCONT_set->devices_present);

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                      // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  if (
    // (device < 1) || 
  (device > pCONT_set->devices_present)) {
    device = 0;
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_RELAYS DEBUG_INSERT_PAGE_BREAK "device>1\tfall back to single relay"));
  }
  active_device = device;

  // if (device <= MAX_PULSETIMERS) {
  //   SetPulseTimer(device, 0);
  // }
  // power_t mask = 1 << (device);        // Device to control

  // Indexing is now from 0!!
  power_t mask = 1 << device;        // Device to control
  if (state <= POWER_TOGGLE) {
    // if ((blink_mask & mask)) {
    //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    //   MqttPublishPowerBlinkState(device);
    // }
    #ifdef USE_RELAY_INTERLOCKS
    // if (Settings.flag_system_phaseout.interlock &&        // CMND_INTERLOCK - Enable/disable interlock
    //     !interlock_mutex &&
    //     ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(power & mask)))
    //    ) {
    //   interlock_mutex = true;                           // Clear all but masked relay in interlock group if new set requested
    //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
    //     if (Settings.interlock[i] & mask) {             // Find interlock group
    //       for (uint32_t j = 0; j < devices_present; j++) {
    //         power_t imask = 1 << j;
    //         if ((Settings.interlock[i] & imask) && (power & imask) && (mask != imask)) {
    //           ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
    //           delay(50);                                // Add some delay to make sure never have more than one relay on
    //         }
    //       }
    //       break;                                        // An interlocked relay is only present in one group so quit
    //     }
    //   }
    //   interlock_mutex = false;
    // }
    #endif

    switch (state) {
    case POWER_OFF: {
      pCONT_set->power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      pCONT_set->power |= mask;
      break;
    case POWER_TOGGLE:
      pCONT_set->power ^= mask;
    }
    SetDevicePower(pCONT_set->power, source);

    // if (device <= MAX_PULSETIMERS) {  // Restart PulseTime if powered On
    //   SetPulseTimer(device , (((POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? ~power : power) & mask) ? Settings.pulse_timer[device -1] : 0);
    // }
  }
  // else if (POWER_BLINK == state) {
  //   if (!(blink_mask & mask)) {
  //     blink_powersave = (blink_powersave & (POWER_MASK ^ mask)) | (power & mask);  // Save state
  //     blink_power = (power >> (device ))&1;  // Prep to Toggle
  //   }
  //   blink_timer = millis() + 100;
  //   blink_counter = ((!Settings.blinkcount) ? 64000 : (Settings.blinkcount *2)) +1;
  //   blink_mask |= mask;  // Set device mask
  //   MqttPublishPowerBlinkState(device);
  //   return;
  // }
  // else if (POWER_BLINK_STOP == state) {
  //   bool flag = (blink_mask & mask);
  //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
  //   MqttPublishPowerBlinkState(device);
  //   if (flag) {
  //     ExecuteCommandPower(device, (blink_powersave >> (device ))&1, SRC_IGNORE);  // Restore state
  //   }
  //   return;
  // }

  if (publish_power) {
    mqtthandler_sensor_teleperiod.fSendNow = true;
    mqtthandler_sensor_ifchanged.fSendNow = true;
  }
}


uint8_t mRelays::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  // root["tbd"] = 0;
  root["relays_connected"] = relays_connected;

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  return 1;
}


uint8_t mRelays::ConstructJSON_Sensor(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  uint16_t length = 0;
  DEBUG_LINE;

  char buffer[50];

  for(int device_id=0;device_id<relays_connected;device_id++){
  DEBUG_LINE;

    if(relay_status[device_id].ischanged||(json_level>JSON_LEVEL_IFCHANGED)){ relay_status[device_id].ischanged=false;
      
      const char* relay_name = GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer));
      DEBUG_LINE;

      AddLog_P(LOG_LEVEL_DEBUG,PSTR("relay_name=%s"),relay_name);
      
      length += pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr+length,
        PSTR(
          "%c"
            "\"%s\":{"
              "\"" "onoff"          "\":%d,"
              "\"" "onoff_ctr"      "\":\"%s\","
              "\"" "friendlyname"   "\":\"%s\","
              "\"" "ontime"         "\":\"%02d:%02d:%02d\","
              "\"" "offtime"        "\":\"%02d:%02d:%02d\""
            "}"
        ),
        length>0?',':'{',
        relay_name,
        GetRelay(device_id),
        GetRelay(device_id)?"ON":"OFF",
        relay_name,
        relay_status[device_id].ontime.hour,relay_status[device_id].ontime.minute,relay_status[device_id].ontime.second,
        relay_status[device_id].offtime.hour,relay_status[device_id].offtime.minute,relay_status[device_id].offtime.second
      );
    }
  }
  
  // things were written, close it
  if(length){ // relay before  
    length += pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr+length, PSTR("}"));
  }

  return strlen(data_buffer2.payload.ctr)?1:0; // use strlen as indicator to send, this will have false positives until arduinojson is phased out

}



/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mRelays::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mRelays::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mRelays::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mRelays::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mRelays::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mRelays::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mRelays::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  // change switch to use array of pointers?
  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_RELAY_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

#endif



// template void mRelays::ftest<int>();
// template void mRelays::ftest2<int>(int value);
// template void mRelays::ftest2<char>(char value);


// template int8_t mRelays::Tasker2<JsonObjectConst>(uint8_t function, JsonObjectConst param1);
// template int8_t mRelays::Tasker2<uint8_t>(uint8_t function, uint8_t param1);



