
/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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

// #ifdef USE_I2C
// #ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * Bh1750Resolution1   0..2    - Set BH1750 1 resolution mode
 * Bh1750Resolution2   0..2    - Set BH1750 2 resolution mode
 * Bh1750MTime1       30..255  - Set BH1750 1 MT register
 * Bh1750MTime2       30..255  - Set BH1750 2 MT register
 *
 * I2C Address: 0x23 or 0x5C
\*********************************************************************************************/
#include "mBH1750.h"

#ifdef USE_MODULE_SENSORS_BH1750

const char* mBH1750::PM_MODULE_SENSORS_BH1750_CTR = D_MODULE_SENSORS_BH1750_CTR;
const char* mBH1750::PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR = D_MODULE_SENSORS_BH1750_FRIENDLY_CTR;

int8_t mBH1750::Tasker(uint8_t function, JsonParserObject obj){
  
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
      Bh1750EverySecond();
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


void mBH1750::parse_JSONCommand(JsonParserObject obj)
{

  // JsonParserToken jtok = obj["test2"];
  //   AddLog(LOG_LEVEL_TEST, PSTR("mBH1750=%d"),jtok.getInt());
    

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

// void CmndBh1750Resolution(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Bh1750.count)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//       if (1 == XdrvMailbox.index) {
//         Settings.SensorBits1.bh1750_1_resolution = XdrvMailbox.payload;
//       } else {
//         Settings.SensorBits1.bh1750_2_resolution = XdrvMailbox.payload;
//       }
//       Bh1750SetResolution(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(Bh1750Resolution(XdrvMailbox.index -1));
//   }
// }

// void CmndBh1750MTime(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Bh1750.count)) {
//     if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
//       Bh1750_sensors[XdrvMailbox.index -1].mtreg = XdrvMailbox.payload;
//       Bh1750SetMTreg(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(Bh1750_sensors[XdrvMailbox.index -1].mtreg);
//   }
// }
}



void mBH1750::Pre_Init(void)
{
  
  if (pCONT_sup->I2cEnabled(XI2C_11)) { 
    settings.fEnableSensor = true;
  }

}


void mBH1750::Init(void)
{
  for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) {
    if (pCONT_sup->I2cActive(Bh1750.addresses[i])) { continue; }

    Bh1750_sensors[Bh1750.count].address = Bh1750.addresses[i];
    // AddLog(LOG_LEVEL_INFO, PSTR("Bh1750.count=%d"),Bh1750.count);

    if (Bh1750SetMTreg(Bh1750.count)) {
      pCONT_sup->I2cSetActiveFound(Bh1750_sensors[Bh1750.count].address, Bh1750.types);
      Bh1750.count++;
    }
  }
}


void mBH1750::EveryLoop()
{


}



uint8_t mBH1750::Bh1750Resolution(uint32_t sensor_index) {
  uint8_t settings_resolution = pCONT_set->Settings.SensorBits1.bh1750_1_resolution;
  if (1 == sensor_index) {
    settings_resolution = pCONT_set->Settings.SensorBits1.bh1750_2_resolution;
  }
  return settings_resolution;
}

bool mBH1750::Bh1750SetResolution(uint32_t sensor_index) {
  pCONT_sup->wire->beginTransmission(Bh1750_sensors[sensor_index].address);
  pCONT_sup->wire->write(Bh1750.resolution[Bh1750Resolution(sensor_index)]);
  return (!pCONT_sup->wire->endTransmission());
}

bool mBH1750::Bh1750SetMTreg(uint32_t sensor_index) {
  // DEBUG_LINE_HERE;
  pCONT_sup->wire->beginTransmission(Bh1750_sensors[sensor_index].address);
  // DEBUG_LINE_HERE;
  uint8_t data = BH1750_MEASUREMENT_TIME_HIGH | ((Bh1750_sensors[sensor_index].mtreg >> 5) & 0x07);
  // DEBUG_LINE_HERE;
  // delay(5000);
  // DEBUG_LINE_HERE;
  pCONT_sup->wire->write(data);
  // DEBUG_LINE_HERE;
  if (pCONT_sup->wire->endTransmission()) { 
  // DEBUG_LINE_HERE;
  return false; }
  // DEBUG_LINE_HERE;
  pCONT_sup->wire->beginTransmission(Bh1750_sensors[sensor_index].address);
  data = BH1750_MEASUREMENT_TIME_LOW | (Bh1750_sensors[sensor_index].mtreg & 0x1F);
  pCONT_sup->wire->write(data);
  if (pCONT_sup->wire->endTransmission()) { return false; }
  // DEBUG_LINE_HERE;
  return Bh1750SetResolution(sensor_index);
}

bool mBH1750::Bh1750Read(uint32_t sensor_index) {
  if (Bh1750_sensors[sensor_index].valid) { Bh1750_sensors[sensor_index].valid--; }

  if (2 != pCONT_sup->wire->requestFrom(Bh1750_sensors[sensor_index].address, (uint8_t)2)) { return false; }

  float level = (pCONT_sup->wire->read() << 8) | pCONT_sup->wire->read();
  float illuminance = level;
  illuminance /= (1.2 * (69 / (float)Bh1750_sensors[sensor_index].mtreg));
  if (1 == Bh1750Resolution(sensor_index)) {
    illuminance /= 2;
  }
  Bh1750_sensors[sensor_index].level = level;
  Bh1750_sensors[sensor_index].illuminance = illuminance;

  Bh1750_sensors[sensor_index].valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void mBH1750::Bh1750EverySecond(void) {
  for (uint32_t i = 0; i < Bh1750.count; i++) {
    // 1mS
    if (!Bh1750Read(i)) {
//      AddLogMissed(Bh1750.types, Bh1750.valid);
    }
  }
}



uint8_t mBH1750::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mBH1750::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();

    
  for (uint32_t sensor_index = 0; sensor_index < Bh1750.count; sensor_index++) {
    if (Bh1750_sensors[sensor_index].valid) {

    JsonBuilderI->Add("Illuminance", Bh1750_sensors[sensor_index].illuminance);
    JsonBuilderI->Add("Level", Bh1750_sensors[sensor_index].level);

      // char sensor_name[10];
      // strlcpy(sensor_name, Bh1750.types, sizeof(sensor_name));
      // if (Bh1750.count > 1) {
      //   snprintf_P(sensor_name, sizeof(sensor_name), PSTR("%s%c%02X"), sensor_name, IndexSeparator(), Bh1750_sensors[sensor_index].address);  // BH1750-23
      // }

//       if (json) {
//         ResponseAppend_P(JSON_SNS_ILLUMINANCE, sensor_name, Bh1750_sensors[sensor_index].illuminance);
// #ifdef USE_DOMOTICZ
//         if ((0 == TasmotaGlobal.tele_period) && (0 == sensor_index)) {
//           DomoticzSensor(DZ_ILLUMINANCE, Bh1750_sensors[sensor_index].illuminance);
//         }
// #endif  // USE_DOMOTICZ
// #ifdef USE_WEBSERVER
//       } else {
//         WSContentSend_PD(HTTP_SNS_ILLUMINANCE, sensor_name, Bh1750_sensors[sensor_index].illuminance);
// #endif  // USE_WEBSERVER
//       }
    }
  }


  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mBH1750::MQTTHandler_Init(){

  struct handler<mBH1750>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mBH1750::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mBH1750::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mBH1750::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mBH1750>* list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_BH1750_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif

