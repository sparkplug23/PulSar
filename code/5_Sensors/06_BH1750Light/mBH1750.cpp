
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

const char* mBH1750::PM_MODULE_SENSORS_BH1750_CTR          = D_MODULE_SENSORS_BH1750_CTR;
const char* mBH1750::PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR = D_MODULE_SENSORS_BH1750_FRIENDLY_CTR;

int8_t mBH1750::Tasker(uint8_t function, JsonParserObject obj)
{
  
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
    case FUNC_EVERY_SECOND:
      SubTask_ReadSensor();
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
    ALOG_INF( PSTR(D_LOG_BH1750 "I2cEnabled(XI2C_11)") );
  }

}


void mBH1750::Init(void)
{
  
  settings.fEnableSensor = false;

  for (uint32_t i = 0; i < sizeof(Bh1750.addresses); i++) 
  {

    if (pCONT_sup->I2cActive(Bh1750.addresses[i])) { continue; }

    device_data[settings.sensor_count].address = Bh1750.addresses[i];

    if (Set_MeasurementTimeRegister(settings.sensor_count)) 
    {
      settings.sensor_count++;
      settings.fEnableSensor = true;
    }

  }

  ALOG_INF( PSTR(D_LOG_BH1750 "Count: %d"), settings.sensor_count );

}


uint8_t mBH1750::Get_Resolution(uint32_t sensor_index) 
{

  uint8_t settings_resolution = 0;

  switch(sensor_index)
  {
    default:
    case 0: settings_resolution = pCONT_set->Settings.SensorBits1.bh1750_1_resolution; break;
    case 1: settings_resolution = pCONT_set->Settings.SensorBits1.bh1750_2_resolution; break;
  }
  
  return settings_resolution;

}


bool mBH1750::Set_Resolution(uint32_t sensor_index) {
  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  pCONT_sup->wire->write(Bh1750.resolution[Get_Resolution(sensor_index)]);
  return (!pCONT_sup->wire->endTransmission());
}


bool mBH1750::Set_MeasurementTimeRegister(uint32_t sensor_index)
{

  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  uint8_t data = BH1750_MEASUREMENT_TIME_HIGH | ((device_data[sensor_index].mtreg >> 5) & 0x07);
  pCONT_sup->wire->write(data);
  if (pCONT_sup->wire->endTransmission()) { return false; }
  pCONT_sup->wire->beginTransmission(device_data[sensor_index].address);
  data = BH1750_MEASUREMENT_TIME_LOW | (device_data[sensor_index].mtreg & 0x1F);
  pCONT_sup->wire->write(data);
  if (pCONT_sup->wire->endTransmission()) { return false; }

  return Set_Resolution(sensor_index);

}


bool mBH1750::Get_SensorReading(uint32_t sensor_index) 
{

  if (device_data[sensor_index].valid) { device_data[sensor_index].valid--; }

  if (2 != pCONT_sup->wire->requestFrom(device_data[sensor_index].address, (uint8_t)2)) 
  { 
    return false;
  }

  float level = (pCONT_sup->wire->read() << 8) | pCONT_sup->wire->read();
  float illuminance = level;
  illuminance /= (1.2 * (69 / (float)device_data[sensor_index].mtreg));
  if (1 == Get_Resolution(sensor_index)) {
    illuminance /= 2;
  }
  device_data[sensor_index].level = level;
  device_data[sensor_index].illuminance = illuminance;

  ALOG_DBM( PSTR(D_LOG_BH1750 "level=%d"), device_data[sensor_index].level);

  device_data[sensor_index].valid = SENSOR_MAX_MISS;

  return true;

}


/********************************************************************************************/

void mBH1750::SubTask_ReadSensor(void) 
{

  for (uint32_t i = 0; i < settings.sensor_count; i++) 
  {
    if (!Get_SensorReading(i)) 
    {
      ALOG_ERR( PSTR(D_LOG_BH1750 "Failed Read") );
    }
  }

}



uint8_t mBH1750::ConstructJSON_Settings(uint8_t json_method)
{

  JBI->Start();
  JBI->Add(D_JSON_COUNT, settings.sensor_count);    
  for (uint32_t ii = 0; ii < settings.sensor_count; ii++)
  {
    JBI->Add(D_JSON_ADDRESS, device_data[ii].address);
  }
  return JBI->End();

}


uint8_t mBH1750::ConstructJSON_Sensor(uint8_t json_method)
{

  JBI->Start();
    
  for (uint32_t sensor_index = 0; sensor_index < settings.sensor_count; sensor_index++) {
    if (device_data[sensor_index].valid) {
      JBI->Add(D_JSON_ILLUMINANCE, device_data[sensor_index].illuminance);
      JBI->Add(D_JSON_LEVEL, device_data[sensor_index].level);
    }
  }

  return JBI->End();
    
}



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
void mBH1750::parse_JSONCommand(JsonParserObject obj)
{

  // ALOG_DBM( PSTR(D_LOG_BH1750 "parse_JSONCommand") );
  
  char buffer[50];
  JsonParserToken jtok = 0; 


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
//       Set_Resolution(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(Get_Resolution(XdrvMailbox.index -1));
//   }
// }

  // if(jtok = obj[PM_JSON_BRIGHTNESS]){ // Assume range 0-100
  //   CommandSet_Brt_255(mapvalue(jtok.getInt(), 0,100, 0,255));
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), brt);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

// void CmndBh1750MTime(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= settings.sensor_count)) {
//     if ((XdrvMailbox.payload > 30) && (XdrvMailbox.payload < 255)) {
//       device_data[XdrvMailbox.index -1].mtreg = XdrvMailbox.payload;
//       Set_MeasurementTimeRegister(XdrvMailbox.index -1);
//     }
//     ResponseCmndIdxNumber(device_data[XdrvMailbox.index -1].mtreg);
//   }
// }

}




#endif

