
#include "mHBridge.h"
#ifdef USE_MODULE_DRIVERS_HBRIDGE

// Should I add variable of "progress" into this, so motor can run from progress to final position (or time limit), but
// calculate progress external to module aka ADC readings (it holds range and measures). Blinds controller interfaces both modules.


void mHBridge::pre_init(void){

  init_success = true; // Begins true, anything bad sets to false

  // MOTOR SETTINGS (A)

  if(pCONT_pins->PinUsed(GPIO_HBRIDGE_L9110_IA_ID) && pCONT_pins->PinUsed(GPIO_HBRIDGE_L9110_OA_ID)){
    
    
    pin_IA = pCONT_set->pin[GPIO_HBRIDGE_L9110_IA_ID];
    pinMode(pin_IA,OUTPUT);
    digitalWrite(pin_IA,LOW); //STOP THE MOTOR
    
    pin_OB = pCONT_set->pin[GPIO_HBRIDGE_L9110_OA_ID];
    pinMode(pin_OB,OUTPUT);
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IA Invalid %d"),pin_IA);
    init_success = false;
  }

  // if(pCONT_set->pin[GPIO_HBRIDGE_L9110_OA_ID] < 99) {
  // }else{
  //   AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_OB Invalid %d"),pin_IB);
  //   init_success = false;
  // }

  if((pin_IA>-1) && (pin_OA>-1)){
    motorA.fEnabled = true;
  }else{
    motorA.fEnabled = false;
  }

  // MOTOR SETTINGS (B)

  if(pCONT_set->pin[GPIO_HBRIDGE_L9110_IB_ID] < 99) {
    pin_IB = pCONT_set->pin[GPIO_HBRIDGE_L9110_IB_ID];
    pinMode(pin_IB,OUTPUT);
    digitalWrite(pin_IB,LOW); //STOP THE MOTOR
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
    init_success = false;
  }

  if(pCONT_set->pin[GPIO_HBRIDGE_L9110_OB_ID] < 99) {
    pin_OB = pCONT_set->pin[GPIO_HBRIDGE_L9110_OB_ID];
    pinMode(pin_OB,OUTPUT);
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_OB Invalid %d"),pin_OB);
    init_success = false;
  }

  // if(pin_IB && pin_OB){
  //   motorB.fEnabled = true;
  // }else{
  //   motorB.fEnabled = false;
  // }

  // if(pCONT_set->pin[GPIO_HBRIDGE_ANALOG_SENSE] < 99) {
  //   pin_AnalogSense = pCONT_set->pin[GPIO_HBRIDGE_ANALOG_SENSE];
  //   pinMode(pin_AnalogSense,INPUT);
  // }else{
  //   AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin pin_IB Invalid %d"),pin_IB);
  // }

}


void mHBridge::init(void){

}

int8_t mHBridge::Tasker(uint8_t function){ 

  switch(function){
    case FUNC_PRE_INIT:
      pre_init();
    break;
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: 
      //SubTasker_ADC_Measure();
    break;
    case FUNC_MQTT_SENDER:
      //SubTasker_MQTTSender();
    break;
    case FUNC_JSON_COMMAND:
      parse_JSONCommand();
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
  digitalWrite(pin_IA,LOW);
  digitalWrite(pin_IB,LOW);
}


void mHBridge::MoveMotorPulse(uint8_t direction, uint16_t milliseconds){

  if(direction == 1){
    digitalWrite(pin_IA,HIGH);
    digitalWrite(pin_IB,LOW);
    delay(milliseconds);
    SetStop();
  }else{
    digitalWrite(pin_IA,LOW);
    digitalWrite(pin_IB,HIGH);
    delay(milliseconds);
    SetStop();
  }

}


int8_t mHBridge::parse_JSONCommand(){//mpkt_t* mpt, char* topic, char* payload){
  
  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/motor")>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_BLINDS));
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return 0; // not meant for here
  }

  int8_t device_id,user_id,schedule_id;
  uint8_t timeon,tempset;

  char* payload = data_buffer2.payload.ctr;

  StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  DeserializationError error = deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();

  uint8_t name_num=-1,state=-1;

  if(obj.containsKey("manual")){ //pCONT->mso->println("obj[\"manual\"] int");
    const char* command = obj["manual"];
    
    if(strstr(command,"up")){  //pCONT->mso->println("MATCHED>> up");
      digitalWrite(pin_IA,LOW);
      digitalWrite(pin_IB,HIGH);
      delay(300);
      SetStop();
    }else if(strstr(command,"down")){ // pCONT->mso->println("MATCHED>> down");
      digitalWrite(pin_IA,HIGH);
      digitalWrite(pin_IB,LOW);
      delay(300);
      SetStop();
    }else if(strstr(command,"vals")){ //pCONT->mso->print("MATCHED>> vals");  

      int motorspeed = obj["speed"];
      int motordirection = obj["direction"];
      int motorduration = obj["duration"];

      // pCONT->mso->print("speed = ");   pCONT->mso->println(motorspeed);  
      // pCONT->mso->print("direction = ");   pCONT->mso->println(motordirection);  
      // pCONT->mso->print("duration = ");   pCONT->mso->println(motorduration);  

      if(motordirection){
        SetMotorUp(motorspeed);
      }else{
        SetMotorDown(motorspeed);
      }

      delay(motorduration);

      SetStop();

    }

  }else 
  if(obj.containsKey("percentage")){// pCONT->mso->println("MATCHED>> obj[\"percentage\"]");
    
    // pCONT->mso->print("percentageBEFORE");
    int percentage = obj["percentage"];
    // pCONT->mso->print("percentage=");
    // pCONT->mso->println(percentage);
    
    // SetBlindPosition(percentage);    
    
  }else{
    // pCONT->mso->println("NOT obj[\"name\"] int");
  }


}


void mHBridge::AddToJsonObject_AddHardware(JsonObject root){


}


#endif

//697