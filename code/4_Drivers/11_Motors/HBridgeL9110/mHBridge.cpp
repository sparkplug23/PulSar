
#include "mHBridge.h"
#ifdef USE_MODULE_DRIVERS_HBRIDGE

void mHBridge::Pre_Init(void){

  init_success = true; 

  // MOTOR SETTINGS (A)
  if(pCONT_pins->PinUsed(GPIO_HBRIDGE_L9110_IA_ID) && pCONT_pins->PinUsed(GPIO_HBRIDGE_L9110_OA_ID)){   
    pin_A_IA = pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_IA_ID];
    pinMode(pin_A_IA,OUTPUT);
    digitalWrite(pin_A_IA,LOW); //STOP THE MOTOR
    pin_A_IB = pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_OA_ID];
    pinMode(pin_A_IB,OUTPUT);
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IA Invalid %d"),pin_A_IA);
    init_success = false;
  }

  // if(pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_OA_ID] < 99) {
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

  // if(pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_IB_ID] < 99) {
  //   pin_IB = pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_IB_ID];
  //   pinMode(pin_IB,OUTPUT);
  //   digitalWrite(pin_IB,LOW); //STOP THE MOTOR
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
  //   init_success = false;
  // }

  // if(pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_OB_ID] < 99) {
  //   pin_A_IB = pCONT_pins->GetPin(GPIO_HBRIDGE_L9110_OB_ID];
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

  // if(pCONT_pins->GetPin(GPIO_HBRIDGE_ANALOG_SENSE] < 99) {
  //   pin_AnalogSense = pCONT_pins->GetPin(GPIO_HBRIDGE_ANALOG_SENSE];
  //   pinMode(pin_AnalogSense,INPUT);
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
  // }

}


void mHBridge::init(void){

}

int8_t mHBridge::Tasker(uint8_t function, JsonParserObject obj){ 

  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      init();
    break;
    case TASK_EVERY_SECOND:
      AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IA %d"),pin_A_IA);
      AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_A_IB %d"),pin_A_IB);
    break;
    case TASK_LOOP: 
      //SubTasker_ADC_Measure();
    break;
    case TASK_MQTT_SENDER:
      //SubTasker_MQTTSender();
    break;
  }

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
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
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

//   if(obj.containsKey("manual")){ //pCONT->mso->println("obj[\"manual\"] int");
//     const char* command = obj["manual"];
    
//     if(strstr(command,"up")){  //pCONT->mso->println("MATCHED>> up");
//       digitalWrite(pin_A_IA,LOW);
//       digitalWrite(pin_A_IB,HIGH);
//       delay(300);
//       SetStop();
//     }else if(strstr(command,"down")){ // pCONT->mso->println("MATCHED>> down");
//       digitalWrite(pin_A_IA,HIGH);
//       digitalWrite(pin_A_IB,LOW);
//       delay(300);
//       SetStop();
//     }else if(strstr(command,"vals")){ //pCONT->mso->print("MATCHED>> vals");  

//       int motorspeed = obj["speed"];
//       int motordirection = obj["direction"];
//       int motorduration = obj["duration"];

//       // pCONT->mso->print("speed = ");   pCONT->mso->println(motorspeed);  
//       // pCONT->mso->print("direction = ");   pCONT->mso->println(motordirection);  
//       // pCONT->mso->print("duration = ");   pCONT->mso->println(motorduration);  

//       if(motordirection){
//         SetMotorUp(motorspeed);
//       }else{
//         SetMotorDown(motorspeed);
//       }

//       delay(motorduration);

//       SetStop();

//     }

//   }else 
//   if(obj.containsKey("percentage")){// pCONT->mso->println("MATCHED>> obj[\"percentage\"]");
    
//     // pCONT->mso->print("percentageBEFORE");
//     int percentage = obj["percentage"];
//     // pCONT->mso->print("percentage=");
//     // pCONT->mso->println(percentage);
    
//     // SetBlindPosition(percentage);    
    
//   }else{
//     // pCONT->mso->println("NOT obj[\"name\"] int");
//   }


// }


#endif

//697