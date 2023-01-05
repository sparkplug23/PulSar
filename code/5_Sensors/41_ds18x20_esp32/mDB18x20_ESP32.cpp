/**
 * @file mDB18x20_ESP32.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-03
 * 
 * Using tasmota method from Jan 2023, not using dallastemp lib at all going forward
 * Eventually merge this with the other method(?)
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mDB18x20_ESP32.h"

#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023

const char* mDB18x20_ESP32::PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR = D_MODULE_SENSORS__DS18X20_ESP32_2023__CTR;
const char* mDB18x20_ESP32::PM_MODULE_SENSORS__DS18X20_ESP32_2023__FRIENDLY_CTR = D_MODULE_SENSORS__DS18X20_ESP32_2023__FRIENDLY_CTR;

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };


int8_t mDB18x20_ESP32::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Ds18x20Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      // EveryLoop();
    break;
    case FUNC_EVERY_SECOND:

        Ds18x20EverySecond();

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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function



void mDB18x20_ESP32::Pre_Init(void){
  
  ALOG_INF (PSTR(D_LOG_DSB DEBUG_INSERT_PAGE_BREAK "init"));
  // delay(2000);

  DS18X20Data.gpios = 0;
  for (uint32_t pins = 0; pins < MAX_DSB_PINS; pins++) 
  {
    ALOG_INF (PSTR(D_LOG_DSB "PinUsed %d %d"), pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins), pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
    if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins)) {
      ds18x20_gpios[pins] = new OneWire(pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
      DS18X20Data.gpios++;
    }
  }

  settings.fEnableSensor = true;

}






/********************************************************************************************/

void mDB18x20_ESP32::Ds18x20Init(void) 
{
  
  Ds18x20Search();
  ALOG_INF (PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), DS18X20Data.sensors);
}

void mDB18x20_ESP32::Ds18x20Search(void) {
  uint8_t num_sensors = 0;
  uint8_t sensor = 0;

  for (uint8_t pins = 0; pins < DS18X20Data.gpios; pins++) {
    ds = ds18x20_gpios[pins];
    ds->reset_search();
    for (num_sensors; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
      if (!ds->search(sensor_new[num_sensors].address)) {
        ds->reset_search();
        break;
      }
      // If CRC Ok and Type DS18S20, DS1822, DS18B20 or MAX31850
      if ((OneWire::crc8(sensor_new[num_sensors].address, 7) == sensor_new[num_sensors].address[7]) &&
        ((sensor_new[num_sensors].address[0] == DS18S20_CHIPID) ||
          (sensor_new[num_sensors].address[0] == DS1822_CHIPID) ||
          (sensor_new[num_sensors].address[0] == DS18B20_CHIPID) ||
          (sensor_new[num_sensors].address[0] == MAX31850_CHIPID))
      ){
        sensor_new[num_sensors].alias_index = 0; // Reset
        ALOG_INF (PSTR(D_LOG_DSB "pins %d"), pins);
        sensor_new[num_sensors].pins_id = pins;
        num_sensors++;
      }
    }
  }

  for (uint32_t i = 0; i < num_sensors; i++) {
    sensor_new[i].index = i;
  }

  // Place ids into accending order
  for (uint32_t i = 0; i < num_sensors; i++) {
    for (uint32_t j = i + 1; j < num_sensors; j++) {
      if (uint32_t(sensor_new[sensor_new[i].index].address) > uint32_t(sensor_new[sensor_new[j].index].address)) 
      {
        ALOG_INF(PSTR("Swap"));
        std::swap(sensor_new[i].index, sensor_new[j].index);
      }
    }
  }
  DS18X20Data.sensors = num_sensors;
  settings.fSensorCount = num_sensors;
}

void mDB18x20_ESP32::Ds18x20Convert(void) {
  for (uint32_t i = 0; i < DS18X20Data.gpios; i++) {
    ds = ds18x20_gpios[i];
    ds->reset();
#ifdef W1_PARASITE_POWER
    // With parasite power held wire high at the end for parasitically powered devices
    ds->write(W1_SKIP_ROM, 1);        // Address all Sensors on Bus
    ds->write(W1_CONVERT_TEMP, 1);    // start conversion, no parasite power on at the end
#else
    ds->write(W1_SKIP_ROM);        // Address all Sensors on Bus
    ds->write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
#endif
//    delay(750);                     // 750ms should be enough for 12bit conv
  }
}

bool mDB18x20_ESP32::Ds18x20Read(uint8_t sensor, float &t) {
  uint8_t data[12];
  int8_t sign = 1;

  t = NAN;

  uint8_t index = sensor_new[sensor].index;
  if (sensor_new[index].valid) { sensor_new[index].valid--; }
  ds = ds18x20_gpios[sensor_new[index].pins_id];
  ds->reset();
  ds->select(sensor_new[index].address);
#ifdef W1_PARASITE_POWER
    // With parasite power held wire high at the end for parasitically powered devices
  ds->write(W1_READ_SCRATCHPAD, 1); // Read Scratchpad
#else
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad
#endif

  for (uint32_t i = 0; i < 9; i++) {
    data[i] = ds->read();
  }
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(sensor_new[index].address[0]) {
      case DS18S20_CHIPID: {
        int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
        t = pCONT_sup->ConvertTemp(tempS * 0.0625f - 0.250f);
#ifdef W1_PARASITE_POWER
        sensor_new[index].temperature = t;
#endif
        sensor_new[index].valid = SENSOR_MAX_MISS;
        return true;
      }
      case DS1822_CHIPID:
      case DS18B20_CHIPID: {
        uint16_t temp12 = (data[1] << 8) + data[0];
        if (temp12 > 2047) {
          temp12 = (~temp12) +1;
          sign = -1;
        }
        t = pCONT_sup->ConvertTemp(sign * temp12 * 0.0625f);  // Divide by 16
#ifdef W1_PARASITE_POWER
        sensor_new[index].temperature = t;
#endif
        sensor_new[index].valid = SENSOR_MAX_MISS;
        return true;
      }
      case MAX31850_CHIPID: {
        int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
        t = pCONT_sup->ConvertTemp(temp14 * 0.0625f);  // Divide by 16
#ifdef W1_PARASITE_POWER
        sensor_new[index].temperature = t;
#endif
        sensor_new[index].valid = SENSOR_MAX_MISS;
        return true;
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void mDB18x20_ESP32::Ds18x20Name(uint8_t sensor) {
//   uint8_t index = sizeof(ds18x20_chipids);
//   while (--index) {
//     if (sensor_new[sensor_new[sensor].index].address[0] == ds18x20_chipids[index]) {
//       break;
//     }
//   }
//   GetTextIndexed(DS18X20Data.name, sizeof(DS18X20Data.name), index, kDs18x20Types);
//   if (DS18X20Data.sensors > 1) {
// #ifdef DS18x20_USE_ID_AS_NAME
//     char address[17];
//     for (uint32_t j = 0; j < 3; j++) {
//       sprintf(address+2*j, "%02X", sensor_new[sensor_new[sensor].index].address[3-j]);  // Only last 3 bytes
//     }
//     snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%s"), DS18X20Data.name, IndexSeparator(), address);
// #else
// uint8_t print_ind = sensor +1;
// #ifdef DS18x20_USE_ID_ALIAS
//     if (sensor_new[sensor].alias) {
//       snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("DS18Sens"));
//       print_ind = sensor_new[sensor].alias;
//     }
// #endif
//     snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%d"), DS18X20Data.name, IndexSeparator(), print_ind);
// #endif
//   }
}

/********************************************************************************************/

void mDB18x20_ESP32::Ds18x20EverySecond(void) 
{

  if (!DS18X20Data.sensors) { return; }

  if (pCONT_time->uptime_seconds_nonreset & 1) {
    // 2mS
    // Ds18x20Search();      // Check for changes in sensors number
    Ds18x20Convert();     // Start Conversion, takes up to one second
  } else {
    float t;
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      // 12mS per device
      if (Ds18x20Read(i, t)) {   // Read temperature
        sensor_new[i].reading.val = t;
        // Serial.println(t);
      } else {
        // Ds18x20Name(i);
        // AddLogMissed(DS18X20Data.name, sensor_new[sensor_new[i].index].valid);
      }
    }
  }

}




uint8_t mDB18x20_ESP32::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
//     //JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.);
  return JsonBuilderI->End();

}


uint8_t mDB18x20_ESP32::ConstructJSON_Sensor(uint8_t json_level)
{
  
  char buffer[40];
  char title [40];  

  uint8_t corrected_sensor_id = 0;

  JsonBuilderI->Start();

  for (uint32_t sensor_id = 0; sensor_id < DS18X20Data.sensors; sensor_id++) {
    

    // if(sensor_new[sensor_id].reading.ischanged || (json_level<=JSON_LEVEL_IFCHANGED)){  

      JBI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_new[sensor_id].index, buffer, sizeof(buffer)));         
        JBI->Add(D_JSON_TEMPERATURE, sensor_new[sensor_id].reading.val);
        JBI->Add(D_JSON_ADDRESS,     sensor_new[sensor_id].address[7]);

        if(json_level >= JSON_LEVEL_DETAILED)
        {
          JBI->Add(D_JSON_ISVALID, sensor_new[sensor_id].reading.isvalid);
          JBI->Add(D_JSON_CAPTURE_UPSECONDS, sensor_new[corrected_sensor_id].reading.captureupsecs);
        }

        if(json_level >= JSON_LEVEL_DEBUG)
        {
          JBI->Add("set_address",sensor_new[sensor_id].flag.set_from_known_address);
          JBI->Add("ID", sensor_id);
          JBI->Add("Corrected_ID", corrected_sensor_id);
          JBI->Array_Start_P("Device%d", sensor_id);
            for(int a=0;a<8;a++)
            {
              JBI->Add(sensor_new[sensor_new[sensor_id].index].address[a]);
            }
          JBI->Array_End();
        }
      JBI->Level_End();  

    // }

  }

  return JsonBuilderI->End();

}






#endif // sensor enabled


