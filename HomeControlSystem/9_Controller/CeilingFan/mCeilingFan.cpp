#include "mCeilingFan.h"


// move this as a controller, with options to use IR sender OR sonoff ifan03 controls


#ifdef USE_MODULE_CUSTOM_CEILING_FAN

int8_t mCeilingFan::Tasker(uint8_t function){

  switch(function){
    case FUNC_INIT:
      //init();
    break;
    case FUNC_LOOP: 
    break;
    case FUNC_JSON_COMMAND: 
      parse_JSONCommand();
    break;
    case FUNC_MQTT_SENDER:
      //SubTasker_MQTTSender();
    break;
    case FUNC_JSON_HARDWARE_APPEND:
      // AddToHardwareMessage();
    break;
    case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:
      // pCONT_web->WSBufferAppend_P(PSTR("}1" D_MODULE_CUSTOM_CEILINGFAN_CTR "}2%s"), " ");
      // // pCONT_web->WSBufferAppend_P(PSTR("}1" "Version" "}2%s"), mSupportFloatToCStr(_MCEILINGFAN_H));
      // pCONT_web->WSBufferAppend_P(PSTR("}1" "Pin" "}2%s"), "D5");
      // pCONT_web->WSBufferAppend_P(PSTR("}1}2&nbsp;"));  // Empty line
    break;

    /************
   * WEBPAGE SECTION * 
  *******************/
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;     
    case FUNC_WEB_SHOW_PARAMETERS:
      //WebPage_Root_SendStatus();
    break;
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      //WebAppend_Root_Draw_PageTable();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      //WebAppend_Root_Status_Table();
    break;
    case FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS:
      //WebAppend_Root_Add_Main_Buttons();
    break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:{
      char button_handle_ctr[10]; 
      char buffer[20];
      uint8_t button_count = 5;
      uint8_t button_event = 0;
      
      pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Ceiling Fan Controls"),"");

      pCONT_web->AppendBuffer_PI2(PSTR("%s"),PSTR("{t}<tr>"));
      for(uint8_t button_id=0;button_id<button_count;button_id++){
        memset(button_handle_ctr,0,sizeof(button_handle_ctr));
        sprintf(button_handle_ctr,"fanbut%d",button_id);

        switch(button_id){
          case 0: button_event = DEVICE_CONTROL_BUTTON_TOGGLE_ID; break;
          case 1: button_event = DEVICE_CONTROL_BUTTON_ON_ID; break;
          case 2: button_event = DEVICE_CONTROL_BUTTON_ON_ID; break;
          case 3: button_event = DEVICE_CONTROL_BUTTON_ON_ID; break;
          case 4: button_event = DEVICE_CONTROL_BUTTON_ON_ID; break;
        }

        pCONT_web->AppendBuffer_PI2(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
                                      100/button_count, 
                                      "fanbut",
                                      button_handle_ctr, 
                                      button_event, 
                                      pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), button_id, kListFanControls),
                                      ""
                                    );
      }
      pCONT_web->AppendBuffer_PI2("%s",PSTR("</tr>{t2}"));
    }break;


  }
}


void mCeilingFan::WebCommand_Parse(void)
{
  
  if(pCONT_web->request_web_command == nullptr){ return; }

  char tmp[100];

//  selectorlist.amount = 0;
  uint8_t  arg_value = 0;

  char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

  // // check palette selector
  // sprintf(arg_ctr,"relay0_toggle\0");
  // if (pCONT_web->pWebServer->hasParam("relay0_toggle")) {
  //   pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   // animation.palette = arg_value;

  //   ExecuteCommandPower(0,POWER_TOGGLE,SRC_MQTT);

  //   // update_all = true; //refresh all
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_RELAYS "hasParam test"));//,arg_ctr,arg_value);
  // }

  for(uint8_t button_id=0;button_id<5;button_id++){
    
    sprintf(arg_ctr,"%s%d","fanbut",button_id);

    if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
      pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
      arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
      // animation.brightness = BrtN2F(arg_value);
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %d"),arg_ctr,arg_value,button_id);
      // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
      // SetRefreshLEDs();

      // New command structure, pair json commands AND allow webcommand checks beside them/instead of?
      // Add function to read source if(!obj[[]]) now becomes if(Command(new method, object)) pass in both, null if not, parse command and return value
      // ExecuteCommand(class_id,arg_value,button_id,SRC_WEBCOMMAND)

      switch(button_id){
        case 0: pCONT->mir->TransmitCode(CEILINGFAN_TOGGLE,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;        
        case 1: pCONT->mir->TransmitCode(CEILINGFAN_SPEED0,24,50,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
        case 2: pCONT->mir->TransmitCode(CEILINGFAN_SPEED1,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
        case 3: pCONT->mir->TransmitCode(CEILINGFAN_SPEED2,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
        case 4: pCONT->mir->TransmitCode(CEILINGFAN_SPEED3,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
      }

      // ExecuteCommandPower(button_id,POWER_TOGGLE,SRC_MQTT);
    }

  }


}


void mCeilingFan::WebAppend_Root_Draw_PageTable(){
  
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
void mCeilingFan::WebAppend_Root_Status_Table(){
  
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



// add main page controls

    // // CREATE FAN SPEEDS
    // WSBufferAppend_P(PSTR("<tr>"));
    // //   if (SONOFF_IFAN02 == pCONT_set->my_module_type) {
    //     // WSBufferAppend_P(HTTP_DEVICE_CONTROL, 36, 1, D_BUTTON_TOGGLE, "");
    //     // for (uint8_t i = 0; i < MAX_FAN_SPEED; i++) {
    //     //   snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i);
    //     //   WSBufferAppend_P(HTTP_DEVICE_CONTROL, 16, i +2, stemp, "");
    //     // }
    // //   } else {
    //    for (uint8_t idx = 1; idx <= pCONT_set->devices_present; idx++) {
    //      snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
    //      WSBufferAppend_P(HTTP_DEVICE_CONTROL, 
    //           100 / pCONT_set->devices_present, 
    //           idx, 
    //           (pCONT_set->devices_present < 5) ? D_BUTTON_TOGGLE : "",
    //           (pCONT_set->devices_present > 1) ? stemp : "");
    //     }
    // // }
    //     WSBufferAppend_P(PSTR("</tr></table>"));
    // // }

//{"light":"x"}//x=dimmer,toggle
//{"fan":"x"}//x=0,1,2,3
int8_t mCeilingFan::parse_JSONCommand(){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/ceilingfan")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_CEILINGFAN));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return 0; // not meant for here
  }

  int8_t isserviced = 0;

  StaticJsonDocument<100> doc;
  DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  if(error){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_CEILINGFAN D_ERROR_JSON_DESERIALIZATION));
    Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
    return 0;
  }
  JsonObject obj = doc.as<JsonObject>();

  if(obj.containsKey(mSupport::strtolower(D_LIGHT))){
    const char* tempctr = obj[mSupport::strtolower(D_LIGHT)];
    // if(strcasestr(tempctr,mSupport::strtolower(D_TOGGLE))){ 
    if(strstr(tempctr,mSupport::strtolower(D_TOGGLE))){ 
      pCONT->mir->TransmitCode(CEILINGFAN_TOGGLE,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID));
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_LIGHT,D_TOGGLE);
      Response_mP(S_JSON_COMMAND_SVALUE, D_LIGHT,D_TOGGLE);
      isserviced++;
    }else 
    if(strstr(tempctr,mSupport::strtolower(D_DIMMER))){ 
      pCONT->mir->TransmitCode(CEILINGFAN_DIMM,24,80,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID));
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_LIGHT,D_DIMMER);
      Response_mP(S_JSON_COMMAND_SVALUE, D_LIGHT,D_TOGGLE);
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_NOMATCH));
      Response_mP(S_JSON_COMMAND_SVALUE, D_LIGHT,D_PARSING_NOMATCH);
    }
  }

  if(obj.containsKey(mSupport::strtolower(D_FAN))){
    int speed = obj[mSupport::strtolower(D_FAN)];
    if(speed>3){
      Response_mP(S_JSON_COMMAND_SVALUE, D_LIGHT,D_PARSING_NOMATCH);
      speed=0; //default off
    }      
    switch(speed){
      default:
      case 0: pCONT->mir->TransmitCode(CEILINGFAN_SPEED0,24,50,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
      case 1: pCONT->mir->TransmitCode(CEILINGFAN_SPEED1,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
      case 2: pCONT->mir->TransmitCode(CEILINGFAN_SPEED2,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
      case 3: pCONT->mir->TransmitCode(CEILINGFAN_SPEED3,24,10,pCONT_pins->GetPin(GPIO_FAN_IRSEND_ID)); break;
    }
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_FAN,speed);
    Response_mP(S_JSON_COMMAND_NVALUE,D_FAN,speed);
    isserviced++;
  }
  
  return isserviced;

}

// void mCeilingFan::AddToJsonObject_AddHardware(JsonObject root){

//   JsonObject objceilingfan = root.createNestedObject(D_TASKNAME_CEILINGFAN);
//     objceilingfan["type"] = "IR";
//     objceilingfan["pin"] = IR_TRANSMITTER_PIN;

// }


// Serialise and append from offset position
void mCeilingFan::AddToHardwareMessage(){

  // StaticJsonDocument<200> doc;
  // JsonObject root = doc.to<JsonObject>();

  // JsonObject classobj = root.createNestedObject(D_TASKNAME_CEILINGFAN);
  //   classobj["version"] = _MCEILINGFAN_H;
  //   classobj["type"] = "IR";

  //   JsonObject pinsobj = classobj.createNestedObject("pins");
  //     pinsobj["DOUT"] = "IR_TRANSMITTER_PIN";

  // uint16_t index_start = data_buffer2.payload.len;
  // uint16_t index_end = 0;
  // char     output[200];  memset(output,0,sizeof(output));

  // serializeJson(doc,output); // {"classname":...."pins":{..}}

  // if(index_start==HARDWARE_STATUS_OFFSET_INDEX){
  //   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DEBUG "CF First call"));
  //   memcpy(&data_buffer2.payload.ctr[index_start],output,sizeof(char)*measureJson(root));
  // }else{
  //   // {"classname":...."pins":{..}} -> {"classname":...."pins":{..},x} so } becomes ,
  //   data_buffer2.payload.ctr[index_start-1] = ',';
  //   // skip first {
  //   memcpy(&data_buffer2.payload.ctr[index_start],&output[1],sizeof(char)*measureJson(root)-1);
  // }

  // index_end = strlen(&data_buffer2.payload.ctr[index_start]);

  // // save index
  // data_buffer2.payload.len = index_start+index_end;

  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_DEBUG D_TASKNAME_CEILINGFAN " HRW " "[%d|%d|%d] \"%s\""),
  //   index_start, //start index
  //   index_end, //end index
  //   data_buffer2.payload.len,
  //   &data_buffer2.payload.ctr[index_start]); //appended

}







#endif
