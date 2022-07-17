
/**
 * @file mBH1750.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 16-bit Ambient Light sensor
 * @version 1.0
 * @date 2022-05-01
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

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





void mBH1750::Pre_Init(void)
{
  
  if (pCONT_sup->I2cEnabled(XI2C_11)) 
  {
    // settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("mBH1750::Pre_Init  FOUND"));
  }
  else
  {
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("mBH1750::Pre_Init NOT FOUND"));
    // settings.fEnableSensor = false;
  }

  // if(pCONT_sup->I2cDevice(Bh1750.addresses[0]) || pCONT_sup->I2cDevice(Bh1750.addresses[1]))
  // {
  //   AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("mBH1750::Pre_Init THIS IS .......... FOUND"));
  // }

}


void mBH1750::Init(void)
{
    settings.fEnableSensor = false;

  for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) {

    if (pCONT_sup->I2cActive(Bh1750.addresses[i])) { continue; }

    device_data[settings.sensor_count].address = Bh1750.addresses[i];
    AddLog(LOG_LEVEL_INFO, PSTR("settings.sensor_count=%d"),settings.sensor_count);

    if (Bh1750SetMTreg(settings.sensor_count)) {
      // pCONT_sup->I2cSetActiveFound(device_data[settings.sensor_count].address, Bh1750.types);
      settings.sensor_count++;
    settings.fEnableSensor = true;
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
  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  pCONT_sup->wire->write(Bh1750.resolution[Bh1750Resolution(sensor_index)]);
  return (!pCONT_sup->wire->endTransmission());
}

bool mBH1750::Bh1750SetMTreg(uint32_t sensor_index)
{

  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  uint8_t data = BH1750_MEASUREMENT_TIME_HIGH | ((device_data[sensor_index].mtreg >> 5) & 0x07);
  pCONT_sup->wire->write(data);
  if (pCONT_sup->wire->endTransmission()) { 
  return false; }
  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  data = BH1750_MEASUREMENT_TIME_LOW | (device_data[sensor_index].mtreg & 0x1F);
  pCONT_sup->wire->write(data);
  if (pCONT_sup->wire->endTransmission()) { return false; }
  return Bh1750SetResolution(sensor_index);

}

bool mBH1750::Bh1750Read(uint32_t sensor_index) {
  if (device_data[sensor_index].valid) { device_data[sensor_index].valid--; }

  if (2 != pCONT_sup->wire->requestFrom(device_data[sensor_index].address, (uint8_t)2)) 
  { 
    return false;
  }

  float level = (pCONT_sup->wire->read() << 8) | pCONT_sup->wire->read();
  float illuminance = level;
  illuminance /= (1.2 * (69 / (float)device_data[sensor_index].mtreg));
  if (1 == Bh1750Resolution(sensor_index)) {
    illuminance /= 2;
  }
  device_data[sensor_index].level = level;
  device_data[sensor_index].illuminance = illuminance;

  AddLog(LOG_LEVEL_INFO, PSTR("BH1:" "level=%d"),(int)level);

  device_data[sensor_index].valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void mBH1750::Bh1750EverySecond(void) {
  for (uint32_t i = 0; i < settings.sensor_count; i++) {
    // 1mS
    if (!Bh1750Read(i)) {
//      AddLogMissed(Bh1750.types, Bh1750.valid);
    }
  }
}



uint8_t mBH1750::ConstructJSON_Settings(uint8_t json_method)
{

  JsonBuilderI->Start();
  JsonBuilderI->Add(D_JSON_COUNT, settings.sensor_count);    
  for (uint32_t ii = 0; ii < settings.sensor_count; ii++)
  {
    JsonBuilderI->Add(D_JSON_ADDRESS, device_data[ii].address);
  }
  return JsonBuilderI->End();

}


uint8_t mBH1750::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();

    
  for (uint32_t sensor_index = 0; sensor_index < settings.sensor_count; sensor_index++) {
    if (device_data[sensor_index].valid) {

    JsonBuilderI->Add("Illuminance", device_data[sensor_index].illuminance);
    JsonBuilderI->Add("Level", device_data[sensor_index].level);

      // char sensor_name[10];
      // strlcpy(sensor_name, Bh1750.types, sizeof(sensor_name));
      // if (settings.sensor_count > 1) {
      //   snprintf_P(sensor_name, sizeof(sensor_name), PSTR("%s%c%02X"), sensor_name, IndexSeparator(), device_data[sensor_index].address);  // BH1750-23
      // }

//       if (json) {
//         ResponseAppend_P(JSON_SNS_ILLUMINANCE, sensor_name, device_data[sensor_index].illuminance);
// #ifdef USE_DOMOTICZ
//         if ((0 == TasmotaGlobal.tele_period) && (0 == sensor_index)) {
//           DomoticzSensor(DZ_ILLUMINANCE, device_data[sensor_index].illuminance);
//         }
// #endif  // USE_DOMOTICZ
// #ifdef USE_WEBSERVER
//       } else {
//         WSContentSend_PD(HTTP_SNS_ILLUMINANCE, sensor_name, device_data[sensor_index].illuminance);
// #endif  // USE_WEBSERVER
//       }
    }
  }


  return JsonBuilderI->End();
    
}



/***
 * 
 * Parse commands
 * 
 * */
void mBH1750::parse_JSONCommand(JsonParserObject obj)
{

  // JsonParserToken jtok = obj["test2"];
  //   AddLog(LOG_LEVEL_TEST, PSTR("mBH1750=%d"),jtok.getInt());
    

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

// void CmndBh1750Resolution(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= settings.sensor_count)) {
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
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= settings.sensor_count)) {
//     if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
//       device_data[XdrvMailbox.index -1].mtreg = XdrvMailbox.payload;
//       Bh1750SetMTreg(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(device_data[XdrvMailbox.index -1].mtreg);
//   }
// }


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


}




#endif

