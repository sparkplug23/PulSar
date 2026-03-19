
#include "mHBridge.h"
#ifdef USE_MODULE_DRIVERS_HBRIDGE

void mHBridge::Pre_Init(void){

  init_success = true; 

  // MOTOR SETTINGS (A)
  if(tkr_pins->PinUsed(GPIO_HBRIDGE_L9110_IA) && tkr_pins->PinUsed(GPIO_HBRIDGE_L9110_OA)){   
    pin_A_IA = tkr_pins->GetPin(GPIO_HBRIDGE_L9110_IA);
    pinMode(pin_A_IA,OUTPUT);
    digitalWrite(pin_A_IA,LOW); //STOP THE MOTOR
    pin_A_IB = tkr_pins->GetPin(GPIO_HBRIDGE_L9110_OA);
    pinMode(pin_A_IB,OUTPUT);
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IA Invalid %d"),pin_A_IA);
    init_success = false;
  }

  // if(tkr_pins->GetPin(GPIO_HBRIDGE_L9110_OA_ID] < 99) {
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IB Invalid %d"),pin_IB);
  //   init_success = false;
  // }

  if((pin_A_IA>-1) && (pin_A_IB>-1)){
    motorA.fEnabled = true;
  }else{
    motorA.fEnabled = false;
  }

  // MOTOR SETTINGS (B)

  // if(tkr_pins->GetPin(GPIO_HBRIDGE_L9110_IB_ID] < 99) {
  //   pin_IB = tkr_pins->GetPin(GPIO_HBRIDGE_L9110_IB_ID];
  //   pinMode(pin_IB,OUTPUT);
  //   digitalWrite(pin_IB,LOW); //STOP THE MOTOR
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
  //   init_success = false;
  // }

  // if(tkr_pins->GetPin(GPIO_HBRIDGE_L9110_OB_ID] < 99) {
  //   pin_A_IB = tkr_pins->GetPin(GPIO_HBRIDGE_L9110_OB_ID];
  //   pinMode(pin_A_IB,OUTPUT);
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IB Invalid %d"),pin_A_IB);
  //   init_success = false;
  // }

  // if(pin_IB && pin_A_IB){
  //   motorB.fEnabled = true;
  // }else{
  //   motorB.fEnabled = false;
  // }

  // if(tkr_pins->GetPin(GPIO_HBRIDGE_ANALOG_SENSE] < 99) {
  //   pin_AnalogSense = tkr_pins->GetPin(GPIO_HBRIDGE_ANALOG_SENSE];
  //   pinMode(pin_AnalogSense,INPUT);
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
  // }

}


void mHBridge::Init(void){

}

int8_t mHBridge::Tasker(uint8_t function, JsonParserObject obj){ 

  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_EVERY_SECOND:
      AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IA %d"),pin_A_IA);
      AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IB %d"),pin_A_IB);
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  }

  return TASKER_RESULT__UNKNOWN_ID;

}


void mHBridge::SetMotorUp(uint8_t speed){

// #ifdef USE_MOTOR_PWM
//   int speed2 = map(speed,0,100,0,255);

//   // the number of the LED pin
//   const int ledPin = MOTOR_IA_PIN;  // 16 corresponds to GPIO16

//   // setting PWM properties
//   const int freq = 1000;
//   const int ledChannel = 0;
//   const int resolution = 8;

//   ledcSetup(ledChannel, freq, resolution);
  
//   // attach the channel to the GPIO to be controlled
//   ledcAttachPin(ledPin, ledChannel);

//   uint8_t dutyCycle = speed;

//   ledcWrite(ledChannel, dutyCycle);
// #else
//   digitalWrite(BLINDS_MOTOR_IA_PIN,HIGH);
// #endif
//   digitalWrite(BLINDS_MOTOR_IB_PIN,LOW);

}



void mHBridge::SetMotorDown(uint8_t speed){

// #ifdef USE_MOTOR_PWM
//   int speed2 = map(speed,0,100,0,255);

//   // the number of the LED pin
//   const int ledPin = MOTOR_IB_PIN;  // 16 corresponds to GPIO16

//   // setting PWM properties
//   const int freq = 1000;
//   const int ledChannel = 1;
//   const int resolution = 8;

//   ledcSetup(ledChannel, freq, resolution);
  
//   // attach the channel to the GPIO to be controlled
//   ledcAttachPin(ledPin, ledChannel);

//   uint8_t dutyCycle = 255-speed;

//   ledcWrite(ledChannel, dutyCycle);

// #else
//   digitalWrite(BLINDS_MOTOR_IA_PIN,LOW);
// #endif
//   digitalWrite(BLINDS_MOTOR_IB_PIN,HIGH);
}



void mHBridge::SetStop(){
  digitalWrite(pin_A_IA,LOW);
  digitalWrite(pin_A_IB,LOW);
}


void mHBridge::MoveMotorPulse(uint8_t direction, uint16_t milliseconds){

  if(direction == 1){
    digitalWrite(pin_A_IA,HIGH);
    digitalWrite(pin_A_IB,LOW);
    delay(milliseconds);
    SetStop();
  }else{
    digitalWrite(pin_A_IA,LOW);
    digitalWrite(pin_A_IB,HIGH);
    delay(milliseconds);
    SetStop();
  }

}


// int8_t mHBridge::parse_JSONCommand(){//mpkt_t* mpt, char* topic, char* payload){
  
//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/motor")>=0){
//     ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_BLINDS));
//     tkr->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return 0; // not meant for here
//   }

//   int8_t device_id,user_id,schedule_id;
//   uint8_t timeon,tempset;

//   char* payload = data_buffer.payload.ctr;

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   DeserializationError error = deserializeJson(doc, payload);
//   JsonObject obj = doc.as<JsonObject>();

//   uint8_t name_num=-1,state=-1;

//   if(obj.containsKey("manual")){ //tkr->mso->println("obj[\"manual\"] int");
//     const char* command = obj["manual"];
    
//     if(strstr(command,"up")){  //tkr->mso->println("MATCHED>> up");
//       digitalWrite(pin_A_IA,LOW);
//       digitalWrite(pin_A_IB,HIGH);
//       delay(300);
//       SetStop();
//     }else if(strstr(command,"down")){ // tkr->mso->println("MATCHED>> down");
//       digitalWrite(pin_A_IA,HIGH);
//       digitalWrite(pin_A_IB,LOW);
//       delay(300);
//       SetStop();
//     }else if(strstr(command,"vals")){ //tkr->mso->print("MATCHED>> vals");  

//       int motorspeed = obj["speed"];
//       int motordirection = obj["direction"];
//       int motorduration = obj["duration"];

//       // tkr->mso->print("speed = ");   tkr->mso->println(motorspeed);  
//       // tkr->mso->print("direction = ");   tkr->mso->println(motordirection);  
//       // tkr->mso->print("duration = ");   tkr->mso->println(motorduration);  

//       if(motordirection){
//         SetMotorUp(motorspeed);
//       }else{
//         SetMotorDown(motorspeed);
//       }

//       delay(motorduration);

//       SetStop();

//     }

//   }else 
//   if(obj.containsKey("percentage")){// tkr->mso->println("MATCHED>> obj[\"percentage\"]");
    
//     // tkr->mso->print("percentageBEFORE");
//     int percentage = obj["percentage"];
//     // tkr->mso->print("percentage=");
//     // tkr->mso->println(percentage);
    
//     // SetBlindPosition(percentage);    
    
//   }else{
//     // tkr->mso->println("NOT obj[\"name\"] int");
//   }


// }



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mHBridge::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
    
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mHBridge::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
  return JBI->End();

}


uint8_t mHBridge::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Object_Start(D_RFRECEIVED);
    
    JBI->Object_End();
  
  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mHBridge::MQTTHandler_Init()
{

  struct handler<mHBridge>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mHBridge::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mHBridge::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT





#endif

