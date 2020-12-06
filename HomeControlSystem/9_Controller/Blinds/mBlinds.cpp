
#include "mBlinds.h"

// Make custom controller, with motor a driver and sensor as analog

#ifdef USE_MODULE_CUSTOM_BLINDS



// Features to do
// Add retracking of limits, moves to both ends, checks no movement then sets them as limits
// Each 0 to 100 sit should invoke new ADC calibrated point


void mBlinds::init(void){

  //set what driver to use

}

int8_t mBlinds::Tasker(uint8_t function){ //Serial.println("mBlinds::Tasker");

  switch(function){
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: 
      //SubTasker_ADC_Measure();
    break;
    case FUNC_MQTT_SENDER:
      //SubTasker_MQTTSender();
      
      // if(abs(millis()-tSaved)>(ISACTIVE()?1000:120000)){tSaved=millis();
      //   MQQTSendFanStatus();
      // }

      // if(GetBlindsROCExceeded()&&(abs(millis()-adcreading.roc.tSavedMQTTSent)>100)){adcreading.roc.tSavedMQTTSent=millis();
      //   SendMQTT_ROCExceeded();
      // }

    break;
    // case FUNC_JSON_COMMAND:
    //   parse_JSONCommand();
    // break;
  }

}


int8_t mBlinds::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}

int8_t mBlinds::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/blinds")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_MODULE_CUSTOM_BLINDS_FRIENDLY_CTR));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mBlinds::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

  // for(int j=0;j<data_buffer.payload.len;j++){
  //   pCONT->mso->MessagePrintchar(data_buffer.payload.ctr[j]);
  // }

  // pCONT->mso->println();
  // pCONT->mso->print("\tiTOPIC[len:");
  // pCONT->mso->print(data_buffer.topic.len);
  // pCONT->mso->print("]> ");
  // pCONT->mso->print(data_buffer.topic.ctr);
  // pCONT->mso->println();
  // pCONT->mso->print("\tiPAYLOAD[len:");
  // pCONT->mso->print(data_buffer.payload.len);
  // pCONT->mso->print("]> ");
  // pCONT->mso->print(data_buffer.payload.ctr);  
  // pCONT->mso->println();

  // pCONT->mso->println();
  // pCONT->mso->print("\tpTOPIC[len:");
  // pCONT->mso->print("]> ");
  // //pCONT->mso->print(topic);
  // pCONT->mso->println();
  // pCONT->mso->print("\tpPAYLOAD[len:");
  // pCONT->mso->print("]> ");
  // pCONT->mso->print(payload);  
  // pCONT->mso->println();

//  pCONT->mqt->ppublish("payload/mrelays",data_buffer.payload.ctr,false);

  uint8_t name_num=-1,state=-1;

  if(obj.containsKey("manual")){ //pCONT->mso->println("obj[\"manual\"] int");
    const char* command = obj["manual"];
    
    if(strstr(command,"up")){  //pCONT->mso->println("MATCHED>> up");
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_BLINDS "Matched %s"),"UP");
      pCONT->mdhb->MoveMotorPulse(1,300);// 1=up,300ms
    }else if(strstr(command,"down")){  //pCONT->mso->println("MATCHED>> down");
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_BLINDS "Matched %s"),"DOWN");
      pCONT->mdhb->MoveMotorPulse(0,300);// 1=up,300ms
    }
    // else if(strstr(command,"vals")){ pCONT->mso->print("MATCHED>> vals");  

    //   // int motorspeed = obj["speed"];
    //   // int motordirection = obj["direction"];
    //   // int motorduration = obj["duration"];

    //   // pCONT->mso->print("speed = ");   pCONT->mso->println(motorspeed);  
    //   // pCONT->mso->print("direction = ");   pCONT->mso->println(motordirection);  
    //   // pCONT->mso->print("duration = ");   pCONT->mso->println(motorduration);  

    //   // if(motordirection){
    //   //   SetMotorUp(motorspeed);
    //   // }else{
    //   //   SetMotorDown(motorspeed);
    //   // }

    //   // delay(motorduration);

    //   // SetStop();

    // }

  }
  // else 
  // if(obj.containsKey("percentage")){ pCONT->mso->println("MATCHED>> obj[\"percentage\"]");
    
  //   pCONT->mso->print("percentageBEFORE");
  //   int percentage = obj["percentage"];
  //   pCONT->mso->print("percentage=");
  //   pCONT->mso->println(percentage);
    
  //   SetBlindPosition(percentage);    
    
  // }
  else{
    // pCONT->mso->println("NOT obj[\"name\"] int");
  }


  //MQQTSendFanStatus();

  /*
  Serial.println();
  Serial.println("mBlinds::parse_JSONCommand()");

  StaticJsonDocument<MQTT_MAX_PARSING_PACKET_SIZE> doc;
  DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  JsonObject obj = doc.as<JsonObject>();

  Serial.print("data_buffer.payload.len");
  Serial.println(mpt->payload.len);

  for(int j=0;j<data_buffer.payload.len;j++){
    pCONT->mso->MessagePrintchar(data_buffer.payload.ctr[j]);
  }

  pCONT->mqt->ppublish("results/topic",data_buffer.topic.ctr,false);
  pCONT->mqt->ppublish("results/payload",data_buffer.payload.ctr,false);

  uint8_t name_num=-1,state=-1;


  Serial.print("]> ");
  Serial.print(data_buffer.payload.ctr);


// Serial.print("topic>> "); Serial.println(pCONT->mqt->mqqt_mqt->mpkt.topic.ctr);
// Serial.print("Payload>> "); Serial.println(data_buffer.payload.ctr);


  //if(obj.containsKey("test1")){ 
if(strstr(data_buffer.payload.ctr,"test1")){
Serial.println("obj[\"test1\"] int");
   // name_num  = obj["name"];

   
  }else{
    Serial.println("NOT obj[\"test1\"] int");
  }

if(mSupport::mSearchCtrIndexOf(data_buffer.payload.ctr,"test2")>=0){
//  if(obj.containsKey("test2")){ 
  Serial.println("obj[\"test2\"] int");
    //name_num  = obj["name"];

    digitalWrite(MOTOR_B_PWM,LOW);
    digitalWrite(MOTOR_B_DIR,HIGH);

    delay(1000);

    SetStop();
  }else{
    Serial.println("NOT obj[\"test1\"] int");
  }


*/
  // uint8_t onoff = root["onoff"];

  // switch(onoff){
  //   case 1: FAN_ON(); break;
  //   case 0: default: FAN_OFF(); break;
  // }

  //if onoff set, remember change mode, or set via mode

  // fan.ischanged=true;
  //pCONT->mso->MessagePrint("[MATCHED] fan.ischanged to onoff = ");pCONT->mso->MessagePrintln(onoff);


}



// void mBlinds::init_ADC(void){

//   //When VDD_A is 3.3V:
//   // 0dB attenuaton (ADC_ATTEN_0db) gives full-scale voltage 1.1V
//   // 2.5dB attenuation (ADC_ATTEN_2_5db) gives full-scale voltage 1.5V
//   // 6dB attenuation (ADC_ATTEN_6db) gives full-scale voltage 2.2V
//   // 11dB attenuation (ADC_ATTEN_11db) gives full-scale voltage 3.9V (see note below)
//   adc1_config_width(ADC_WIDTH_BIT_12);
//   adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);

//   memset(&adcreading,0,sizeof(0));

// //fill
//   for(int i=0;i<READINGSTOTAL;i++){
//     adcreading.raw[i] = adc1_get_raw(ADC1_CHANNEL_4);
//   }

//   // #if defined(ARDUINO_ARCH_ESP32)
//   //   ledcAttachPin(m_nPinR, m_nChannelR);
//   //   ledcSetup(m_nChannelR, 5000, 8);
//   // #else
//   //   pinMode(m_nPinR, OUTPUT);
//   //  #endif


// }


// void mBlinds::SubTasker_ADC_Measure(void){

//   // Measure ADC range (2 values)
//   if(abs(millis()-adcreading.tSavedSampleTime)>(SAMPLE_PERIOD)){adcreading.tSavedSampleTime=millis();
//     UpdateBlindsADC();
//     UpdateBlindsROC(); //update after each measurement
//   }

//   // if(abs(millis()-adcreading.roc.tSaved)>(SAMPLE_PERIOD)){adcreading.roc.tSaved=millis();
//   //   UpdateBlindsROC();
//   // }

// }


// void mBlinds::UpdateBlindsADC(){

//   if(adcreading.index>=READINGSTOTAL-1){
//     adcreading.index = 0;
//   }

//   // new value
//   adcreading.raw[adcreading.index++] = adc1_get_raw(ADC1_CHANNEL_4);
//   // total
//   adcreading.total = 0;
//   for(int i=0;i<READINGSTOTAL;i++){ adcreading.total += adcreading.raw[i]; }
//   // average
//   adcreading.average = adcreading.total/READINGSTOTAL;

// }


// // Special function that calls UpdateBlindsADC until FULL loop (all of the array) had been calculated again
// // Used for accurate ADC readings during motor "off" period in SetBlindPosition
// void mBlinds::UpdateBlindsADC_RefreshALLUntilComplete(){

//   // Record current index
//   uint16_t start_index = adcreading.index;

//   // Call function first so index is incremented and repeat until it loops back to it again
//   do{
//     UpdateBlindsADC();
//   }while(start_index != adcreading.index);

// }




// // Returns true when ROC exceeds value, therefore, moving
// float mBlinds::UpdateBlindsROC(){
//     adcreading.roc.val = adcreading.roc.stored - adcreading.average;
//     adcreading.roc.stored = adcreading.average; //update
//     return adcreading.roc.val;
// }



// // Returns true when ROC exceeds value, therefore, moving
// uint8_t mBlinds::GetBlindsROCExceeded(){

//     if(adcreading.roc.val > BLINDS_ROC_LIMIT){
//       return true;
//     }
//     return false;

// }



// float mBlinds::PositionPercentage(){

//   // limit to within 0-100%
//   uint16_t _average = adcreading.average;

//   // _average = _average<3090?3090:_average;
//   // _average = _average>3250?3250:_average;

//   float pos = 0;
//   pos = mSupport::mapfloat(_average,3090,3250,100,0);
//   pos = constrain(pos,0,100);

//   return pos;
// }




// void mBlinds::MQQTSendFanStatus(void){
//     ConstructJSON_FanStatus();
//     if(data_buffer.payload.len){ // if something to send
//       pCONT->mqt->ppublish("status/blinds", data_buffer.payload.ctr,false);
//     }
// }
// void mBlinds::ConstructJSON_FanStatus(){

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   JsonObject root = doc.to<JsonObject>();

//   D_DATA_BUFFER_CLEAR();

//   JsonObject posobj = root.createNestedObject("position");
//     posobj["percentage"] = PositionPercentage();
//     posobj["transition_time"] = tSavedTransitionTime;
//     posobj["transition_time_limit"] = tSavedTransitionTime;

//   JsonObject adcobj = root.createNestedObject("ADC");
//     adcobj["now"] = adc1_get_raw(ADC1_CHANNEL_4);
//     adcobj["samples_total"] = READINGSTOTAL;
//     adcobj["sample_period_ms"] = SAMPLE_PERIOD;
//     adcobj["index"] = adcreading.index;
//     adcobj["total"] = adcreading.total;
//     adcobj["average"] = adcreading.average;
//     adcobj["fromboundary_lower"] = adcreading.average - motor_adc_boundary_lower;
//     adcobj["fromboundary_upper"] = motor_adc_boundary_upper - adcreading.average;
    
//   JsonObject rocobj = root.createNestedObject("ROC");
//     rocobj["val"] = adcreading.roc.val;
//     rocobj["stored"] = adcreading.roc.stored;
//     rocobj["lastupdate"] = abs(millis()-adcreading.roc.tSaved);

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

// }

// // set/event/status are the new THREE topics
// void mBlinds::SendMQTT_ROCExceeded(){

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   JsonObject root = doc.to<JsonObject>();

//   D_DATA_BUFFER_CLEAR();

//   root["roc_exceeded"] = adcreading.roc.val;
//   root["A_PIN"] = digitalRead(BLINDS_MOTOR_IA_PIN);
//   root["B_PIN"] = digitalRead(BLINDS_MOTOR_IA_PIN);
//   root["motion"] = "Up"; //up/down/stopped

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

//   if(data_buffer.payload.len){ // if something to send
//     pCONT->mqt->ppublish("status/event/blinds", data_buffer.payload.ctr,false);
//   }

// }


// // set/event/status are the new THREE topics
// void mBlinds::SendMQTT_Event(String key, String pair){

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   JsonObject root = doc.to<JsonObject>();

//   D_DATA_BUFFER_CLEAR();

//   root[key] = pair;

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

//   if(data_buffer.payload.len){ // if something to send
//     pCONT->mqt->ppublish("status/event/blinds", data_buffer.payload.ctr,false);
//   }

// }



// // set/event/status are the new THREE topics

// // Sent VERY fast, will want to limit what is sent later and the rate this works -- TESTING ONLY
// void mBlinds::SendMQTT_StatusPositionRapid(){

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   JsonObject root = doc.to<JsonObject>();

//   D_DATA_BUFFER_CLEAR();

//   JsonObject posobj = root.createNestedObject("position");
//     posobj["percentage"] = PositionPercentage();
//     posobj["transition_time"] = tSavedTransitionTime;
//     posobj["transition_time_limit"] = tSavedTransitionTime;

//   JsonObject adcobj = root.createNestedObject("ADC");
//     adcobj["now"] = adc1_get_raw(ADC1_CHANNEL_4);
//     adcobj["samples_total"] = READINGSTOTAL;
//     adcobj["sample_period_ms"] = SAMPLE_PERIOD;
//     adcobj["index"] = adcreading.index;
//     adcobj["total"] = adcreading.total;
//     adcobj["average"] = adcreading.average;
//     adcobj["fromboundary_lower"] = adcreading.average - motor_adc_boundary_lower;
//     adcobj["fromboundary_upper"] = motor_adc_boundary_upper - adcreading.average;
    
//   JsonObject rocobj = root.createNestedObject("ROC");
//     rocobj["val"] = adcreading.roc.val;
//     rocobj["stored"] = adcreading.roc.stored;
//     rocobj["lastupdate"] = abs(millis()-adcreading.roc.tSaved);
//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

//   if(data_buffer.payload.len){ // if something to send
//     pCONT->mqt->ppublish("status/blinds", data_buffer.payload.ctr,false);
//   }

// }




// void mBlinds::SetMotorUp(uint8_t speed){

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

// }



// void mBlinds::SetMotorDown(uint8_t speed){

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
// }



// void mBlinds::SetStop(){
//   digitalWrite(BLINDS_MOTOR_IA_PIN,LOW);
//   digitalWrite(BLINDS_MOTOR_IB_PIN,LOW);
// }

// void mBlinds::SetBlindPosition(uint8_t percentage){ pCONT->mso->println("f::SetBlindPosition");

//   int max_time = 7000;
//   uint32_t tSaved = millis();
//   float pos_now = 0;
//   float pos_error = 0;

//     pCONT->mso->println("pos_now\tpos_error\t");
//   // Limit motor on time
//   do{

//     pos_now = PositionPercentage();
//     pos_error = pos_now - percentage;

//     pCONT->mso->print(pos_now); pCONT->mso->print("\t");
//     pCONT->mso->print(pos_error); pCONT->mso->println("\t");

//     //end within limit
//     if(fabs(pos_error)<BLINDS_PERCENTAGE_ACCURACY){
//       SendMQTT_Event("transition","stopped early");
//       break;
//     }

//     // end if ROC is low ie stopped moving
//     if(!GetBlindsROCExceeded()){
//       SendMQTT_Event("transition","!GetBlindsROCExceeded");
//       //break;
//     }

//     if(pos_error > BLINDS_PERCENTAGE_ACCURACY){ // >5
//       SetMotorUp();
//       SendMQTT_Event("transition","motor up");
//     }else if(pos_error < (BLINDS_PERCENTAGE_ACCURACY*-1)){ // <-5
//       SetMotorDown();
//       SendMQTT_Event("transition","motor down");
//     }

//     delay(20);//+abs(pos_error)); //error controls speed
//     SetStop();
//     delay(5);
//     UpdateBlindsADC_RefreshALLUntilComplete();
//     UpdateBlindsROC();
//     //delay(5);

//     SendMQTT_StatusPositionRapid();

//     //temp method, if time longer than 7 seconds

//     tSavedTransitionTime = millis()-tSaved;
//   }while(abs(millis()-tSaved)<max_time);

//   SetStop();
//   //SendMQTT_Event("transition","stopped");

// }



// int8_t mBlinds::parse_JSONCommand(){//mpkt_t* mpt, char* topic, char* payload){

//   Serial.println("MBLINDS TEST");
  
//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/blinds")>=0){
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_BLINDS));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return 0; // not meant for here
//   }

//   int8_t device_id,user_id,schedule_id;
//   uint8_t timeon,tempset;

//   // char* payload = data_buffer.payload.ctr;

//   DynamicJsonDocument doc(100);
//   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   JsonObject obj = doc.as<JsonObject>();

//   return 0;

// }


// void mBlinds::AddToJsonObject_AddHardware(JsonObject root){

//   JsonObject obj = root.createNestedObject("fan");
//     obj["type"] = "FAN-SPST-relay";
//     obj["function"] = "on|off";
//     obj["unpoweredstate"] = "POWER_OFF-NO";
//     obj["pin"] = 0;// FAN_RELAY_PIN;

// }



// //enum FAN_MODE{MANUAL=1,AUTO_LOCAL,AUTO_REMOTE};
// const char* mBlinds::FanModeCtr(void){
//   return (fan_mode == MANUAL ? "MANUAL\0" :
//       (fan_mode == AUTO_TEMP ?  "AUTO_TEMP\0" :
//       (fan_mode == AUTO_REMOTE ?  "AUTO_REMOTE\0" :
//       "unk\0")));
// }

#endif
