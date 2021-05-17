
/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Tasmota

  Copyright (C) 2021  Nuno Ferreira and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mSR04.h"

#ifdef USE_MODULE_SENSORS_SR04

const char* mSR04::PM_MODULE_SENSORS_SR04_CTR = D_MODULE_SENSORS_SR04_CTR;
const char* mSR04::PM_MODULE_SENSORS_SR04_FRIENDLY_CTR = D_MODULE_SENSORS_SR04_FRIENDLY_CTR;

int8_t mSR04::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:
      Sr04TReading();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mSR04::parse_JSONCommand(JsonParserObject obj)
{

}



void mSR04::Pre_Init(void)
{
  if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  {
    settings.fEnableSensor = true;
  }

}


void mSR04::Init(void)
{


}


void mSR04::EveryLoop()
{


}


uint8_t mSR04::Sr04TModeDetect(void)
{
  sr04_type = 0;
  if (!pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID)) { return sr04_type; }

  int sr04_echo_pin = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);
  int sr04_trig_pin = (pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID)) ? pCONT_pins->GetPin(GPIO_SR04_TRIG_ID) : pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);   // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
  sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

  if (sonar_serial->begin(9600,1)) {
    AddLog(LOG_LEVEL_TEST,PSTR("SR04: Detect mode"));

    if (sr04_trig_pin != -1) {
      sr04_type = (Sr04TMiddleValue(Sr04TMode3Distance(), Sr04TMode3Distance(), Sr04TMode3Distance()) != NO_ECHO) ? 3 : 1;
    } else {
      sr04_type = 2;
    }
  } else {
    sr04_type = 1;
  }

  if (sr04_type < 2) {
    delete sonar_serial;
    sonar_serial = nullptr;
    if (-1 == sr04_trig_pin) {
      sr04_trig_pin = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
    }
    sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, 300);
  } else {
    if (sonar_serial->hardwareSerial()) {
      // ClaimSerial();
    }
  }

  AddLog(LOG_LEVEL_INFO,PSTR("SR04: Mode %d"), sr04_type);
  return sr04_type;
}

uint16_t mSR04::Sr04TMiddleValue(uint16_t first, uint16_t second, uint16_t third)
{
  uint16_t ret = first;
  if (first > second) {
    first = second;
    second = ret;
  }

  if (third < first) {
    return first;
  } else if (third > second) {
    return second;
  } else {
    return third;
  }
}

uint16_t mSR04::Sr04TMode3Distance() {

    sonar_serial->write(0x55);
    sonar_serial->flush();

    return Sr04TMode2Distance();
}

uint16_t mSR04::Sr04TMode2Distance(void)
{
  sonar_serial->setTimeout(300);
  const char startByte = 0xff;

  if (!sonar_serial->find(startByte)) {
      AddLog(LOG_LEVEL_TEST,PSTR("SR04: No start byte"));
      return NO_ECHO;
  }

  delay(5);

  uint8_t crc = sonar_serial->read();
  //read high byte
  uint16_t distance = ((uint16_t)crc) << 8;

  //read low byte
  distance += sonar_serial->read();
  crc += distance & 0x00ff;
  crc += 0x00FF;

  //check crc sum
  if (crc != sonar_serial->read()) {
    AddLog(LOG_LEVEL_ERROR,PSTR("SR04: Reading CRC error."));
    return NO_ECHO;
  }
  AddLog(LOG_LEVEL_TEST,PSTR("SR04: Distance: %d"), distance);
  return distance;
}

void mSR04::Sr04TReading(void) {

  if (sonar_serial==nullptr && sonar==nullptr) {
    Sr04TModeDetect();
  }

  switch (sr04_type) {
    case 3:
      distance = (real64_t)(Sr04TMiddleValue(Sr04TMode3Distance(),Sr04TMode3Distance(),Sr04TMode3Distance()))/ 10; //convert to cm
      break;
    case 2:
      //empty input buffer first
      while(sonar_serial->available()) sonar_serial->read();
      distance = (real64_t)(Sr04TMiddleValue(Sr04TMode2Distance(),Sr04TMode2Distance(),Sr04TMode2Distance()))/10;
      break;
    case 1:
      distance = (real64_t)(sonar->ping_median(5))/ US_ROUNDTRIP_CM;
      break;
    default:
      distance = NO_ECHO;
  }

  return;
}





uint8_t mSR04::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mSR04::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
    
//       ResponseAppend_P(PSTR(",\"SR04\":{\"" D_JSON_DISTANCE "\":%s}"), distance_chr);

  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSR04::MQTTHandler_Init(){

  struct handler<mSR04>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSR04::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSR04::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSR04::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSR04::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSR04::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSR04::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSR04>* list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_SR04_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
