/*
  Version B - Testing tasmota that avoids DallasTemp, but still relies on OneWire and works with only one pin
*/



/*
  xsns_05_ds18x20.ino - DS18x20 temperature sensor support for Tasmota

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



V2 is based on tasmota





*/

#include "mDS18X.h"

#ifdef USE_MODULE_SENSORS_DS18X_V2

const char* mDS18X::PM_MODULE_SENSORS_DB18_CTR = D_MODULE_SENSORS_DB18S20_CTR;
const char* mDS18X::PM_MODULE_SENSORS_DB18_FRIENDLY_CTR = D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;

int8_t mDS18X::Tasker(uint8_t function, JsonParserObject obj){

  int8_t function_result = 0;
  
  // some functions must run regardless
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      // Pre_Init();
    break;
    case FUNC_INIT:
      // Init();
      Ds18x20Init();
    break;
  }
  
  // Only continue to remaining functions if sensor has been detected and enabled
  // if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
     EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:{
        Ds18x20EverySecond();


// AddLog(LOG_LEVEL_INFO, PSTR("\t\t\tcount=%d"), sensor_group[0].dallas->getDeviceCount());

// uint8_t sensor_group_count = 0;
// sensor_group[sensor_group_count].pin = pCONT_pins->GetPin(GPIO_DSB_1OF2_ID);
//     sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
//     sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin 1 Valid %d"),sensor_group[sensor_group_count].pin);
//     sensor_group[sensor_group_count].dallas->begin();

//  // Start up the library
//   // sensors.begin();
  
//   // Grab a count of devices on the wire
//  uint8_t  numberOfDevices = sensor_group[sensor_group_count].dallas->getDeviceCount();
  
//   // locate devices on the bus
//   Serial.print("Locating devices...");
  
//   Serial.print("Found ");
//   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "numberOfDevices=%d"), numberOfDevices);
//   Serial.println(" devices.");

//   // report parasite power requirements
//   Serial.print("Parasite power is: "); 
//   if (sensor_group[sensor_group_count].dallas->isParasitePowerMode()) Serial.println("ON");
//   else Serial.println("OFF");

//   DeviceAddress tempDeviceAddress;
  
//   // Loop through each device, print out address
//   for(int i=0;i<numberOfDevices; i++)
//   {
//     // Search the wire for address
//     if(sensor_group[sensor_group_count].dallas->getAddress(tempDeviceAddress, i))
//     {
//       Serial.print("Found device ");
//       Serial.print(i, DEC);
//       Serial.print(" with address: ");
//       printAddress(tempDeviceAddress);
//       Serial.println();
      
//       Serial.print("Setting resolution to ");
//       Serial.println(TEMPERATURE_PRECISION, DEC);
      
//       // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
//       sensor_group[sensor_group_count].dallas->setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
      
//       Serial.print("Resolution actually set to: ");
//       Serial.print(sensor_group[sensor_group_count].dallas->getResolution(tempDeviceAddress), DEC); 
//       Serial.println();
//     }else{
//       Serial.print("Found ghost device at ");
//       Serial.print(i, DEC);
//       Serial.print(" but could not detect address. Check power and cabling");
//     }

//   }



  // for(int i=0;i<DB18_SENSOR_MAX;i++){
  //     printAddress(sensor[i].address);
  //   }
    // Init();
      // Pre_Init();
      // char buffer[100];
      // uint8_t ii = 5;
      // AddLog(LOG_LEVEL_TEST,PSTR("\n\r\n\rdb18 device name %d \"%s\""),ii,DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DB18S20_ID, ii, buffer, sizeof(buffer)));
    
    
      // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor[sensor_count%d].id = %d"),0,GetCorrectedDeviceID(0)); 
      // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor[sensor_count%d].id = %d"),1,GetCorrectedDeviceID(1)); 

    }
    break;   
    case FUNC_EVERY_MINUTE:

// Pre_Init();
      #ifdef ENABLE_DEVFEATURE_DS18X_RETRY_IF_NOT_EXPECTED_SENSOR_COUNT
        // #define D_DS18X_EXPECTED_SENSOR_COUNT 12

        if(settings.nSensorsFound != D_DS18X_EXPECTED_SENSOR_COUNT)
        {

          



        }



      #endif // ENABLE_DEVFEATURE_DS18X_RETRY_IF_NOT_EXPECTED_SENSOR_COUNT



    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID:
      Scan_ReportAsJsonBuilder();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
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

}//end function


#ifdef ESP32



void Ds18x20Init(void) {
  ds = new OneWire(Pin(GPIO_DSB));

  Ds18x20Search();
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), DS18X20Data.sensors);
}

void Ds18x20Search(void) {
  uint8_t num_sensors=0;
  uint8_t sensor = 0;

  ds->reset_search();
  for (num_sensors = 0; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
    if (!ds->search(ds18x20_sensor[num_sensors].address)) {
      ds->reset_search();
      break;
    }
    // If CRC Ok and Type DS18S20, DS1822, DS18B20 or MAX31850
    if ((OneWire::crc8(ds18x20_sensor[num_sensors].address, 7) == ds18x20_sensor[num_sensors].address[7]) &&
       ((ds18x20_sensor[num_sensors].address[0] == DS18S20_CHIPID) ||
        (ds18x20_sensor[num_sensors].address[0] == DS1822_CHIPID) ||
        (ds18x20_sensor[num_sensors].address[0] == DS18B20_CHIPID) ||
        (ds18x20_sensor[num_sensors].address[0] == MAX31850_CHIPID))) {
      num_sensors++;
    }
  }
  for (uint32_t i = 0; i < num_sensors; i++) {
    ds18x20_sensor[i].index = i;
  }
  for (uint32_t i = 0; i < num_sensors; i++) {
    for (uint32_t j = i + 1; j < num_sensors; j++) {
      if (uint32_t(ds18x20_sensor[ds18x20_sensor[i].index].address) > uint32_t(ds18x20_sensor[ds18x20_sensor[j].index].address)) {
        std::swap(ds18x20_sensor[i].index, ds18x20_sensor[j].index);
      }
    }
  }
  DS18X20Data.sensors = num_sensors;
}

void Ds18x20Convert(void) {
  ds->reset();
  ds->write(W1_SKIP_ROM);        // Address all Sensors on Bus
  ds->write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
//  delay(750);                   // 750ms should be enough for 12bit conv
}

bool Ds18x20Read(uint8_t sensor, float &t) {
  uint8_t data[12];
  int8_t sign = 1;

  t = NAN;

  uint8_t index = ds18x20_sensor[sensor].index;
  if (ds18x20_sensor[index].valid) { ds18x20_sensor[index].valid--; }

  ds->reset();
  ds->select(ds18x20_sensor[index].address);
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad

  for (uint32_t i = 0; i < 9; i++) {
    data[i] = ds->read();
  }
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(ds18x20_sensor[index].address[0]) {
      case DS18S20_CHIPID: {
        int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
        t = ConvertTemp(tempS * 0.0625f - 0.250f);
        ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
        return true;
      }
      case DS1822_CHIPID:
      case DS18B20_CHIPID: {
        uint16_t temp12 = (data[1] << 8) + data[0];
        if (temp12 > 2047) {
          temp12 = (~temp12) +1;
          sign = -1;
        }
        t = ConvertTemp(sign * temp12 * 0.0625f);  // Divide by 16
        ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
        return true;
      }
      case MAX31850_CHIPID: {
        int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
        t = ConvertTemp(temp14 * 0.0625f);  // Divide by 16
        ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
        return true;
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Name(uint8_t sensor) {
  uint8_t index = sizeof(ds18x20_chipids);
  while (--index) {
    if (ds18x20_sensor[ds18x20_sensor[sensor].index].address[0] == ds18x20_chipids[index]) {
      break;
    }
  }
  GetTextIndexed(DS18X20Data.name, sizeof(DS18X20Data.name), index, kDs18x20Types);
  if (DS18X20Data.sensors > 1) {
#ifdef DS18x20_USE_ID_AS_NAME
    char address[17];
    for (uint32_t j = 0; j < 3; j++) {
      sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[sensor].index].address[3-j]);  // Only last 3 bytes
    }
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%s"), DS18X20Data.name, IndexSeparator(), address);
#else
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%d"), DS18X20Data.name, IndexSeparator(), sensor +1);
#endif
  }
}

/********************************************************************************************/

void Ds18x20EverySecond(void) {
  if (!DS18X20Data.sensors) { return; }

  if (TasmotaGlobal.uptime & 1) {
    // 2mS
//    Ds18x20Search();      // Check for changes in sensors number
    Ds18x20Convert();     // Start Conversion, takes up to one second
  } else {
    float t;
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      // 12mS per device
      if (Ds18x20Read(i, t)) {   // Read temperature
        if (Settings->flag5.ds18x20_mean) {
          if (ds18x20_sensor[i].numread++ == 0) {
            ds18x20_sensor[i].temp_sum = 0;
          }
          ds18x20_sensor[i].temp_sum += t;
        }
      } else {
        Ds18x20Name(i);
        AddLogMissed(DS18X20Data.name, ds18x20_sensor[ds18x20_sensor[i].index].valid);
      }
    }
  }
}

void Ds18x20Show(bool json) {
  float t;

  uint8_t dsxflg = 0;
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    if (Ds18x20Read(i, t)) {           // Check if read failed
      Ds18x20Name(i);

      if (json) {
        if (Settings->flag5.ds18x20_mean) {
          if ((0 == TasmotaGlobal.tele_period) && ds18x20_sensor[i].numread) {
            t = ds18x20_sensor[i].temp_sum / ds18x20_sensor[i].numread;
            ds18x20_sensor[i].numread = 0;
          }
        }
        char address[17];
        for (uint32_t j = 0; j < 6; j++) {
          sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[i].index].address[6-j]);  // Skip sensor type and crc
        }
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
          DS18X20Data.name, address, Settings->flag2.temperature_resolution, &t);
        dsxflg++;
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (1 == dsxflg)) {
          DomoticzFloatSensor(DZ_TEMP, t);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == TasmotaGlobal.tele_period) && (1 == dsxflg)) {
          KnxSensor(KNX_TEMPERATURE, t);
        }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(DS18X20Data.name, t);
#endif  // USE_WEBSERVER
      }
    }
  }
}



#endif // ESP32



#endif // USE_MODULE_SENSORS_DS18X_V2





#ifdef ESP8266
#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature - Multiple sensors
\*********************************************************************************************/

#define XSNS_05              5

//#define USE_DS18x20_RECONFIGURE    // When sensor is lost keep retrying or re-configure
//#define DS18x20_USE_ID_AS_NAME      // Use last 3 bytes for naming of sensors

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_WRITE_EEPROM      0x48
#define W1_WRITE_SCRATCHPAD  0x4E
#define W1_READ_SCRATCHPAD   0xBE

#ifndef DS18X20_MAX_SENSORS // DS18X20_MAX_SENSORS fallback to 8 if not defined in user_config_override.h
#define DS18X20_MAX_SENSORS  8
#endif

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };

struct {
  float temperature;
  float temp_sum;
  uint16_t numread;
  uint8_t address[8];
  uint8_t index;
  uint8_t valid;
} ds18x20_sensor[DS18X20_MAX_SENSORS];

struct {
#ifdef W1_PARASITE_POWER
  uint32_t w1_power_until = 0;
  uint8_t current_sensor = 0;
#endif
  char name[17];
  uint8_t sensors = 0;
  uint8_t input_mode = 0;    // INPUT or INPUT_PULLUP (=2)
  int8_t pin = 0;            // Shelly GPIO3 input only
  int8_t pin_out = 0;        // Shelly GPIO00 output only
  bool dual_mode = false;    // Single pin mode
} DS18X20Data;

/*********************************************************************************************\
 * Embedded tuned OneWire library
\*********************************************************************************************/

#define W1_MATCH_ROM         0x55
#define W1_SEARCH_ROM        0xF0

uint8_t onewire_last_discrepancy = 0;
uint8_t onewire_last_family_discrepancy = 0;
bool onewire_last_device_flag = false;
unsigned char onewire_rom_id[8] = { 0 };

/*------------------------------------------------------------------------------------------*/

uint8_t OneWireReset(void) {
  uint8_t retries = 125;

  if (!DS18X20Data.dual_mode) {
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    pinMode(DS18X20Data.pin, OUTPUT);
    digitalWrite(DS18X20Data.pin, LOW);
    delayMicroseconds(480);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  } else {
    digitalWrite(DS18X20Data.pin_out, HIGH);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(480);
    digitalWrite(DS18X20Data.pin_out, HIGH);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  }
}

void OneWireWriteBit(uint8_t v) {
  static const uint8_t delay_low[2] = { 65, 10 };
  static const uint8_t delay_high[2] = { 5, 55 };

  v &= 1;
  if (!DS18X20Data.dual_mode) {
    digitalWrite(DS18X20Data.pin, LOW);
    pinMode(DS18X20Data.pin, OUTPUT);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin, HIGH);
  } else {
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin_out, HIGH);
  }
  delayMicroseconds(delay_high[v]);
}

uint8_t OneWire1ReadBit(void) {
  pinMode(DS18X20Data.pin, OUTPUT);
  digitalWrite(DS18X20Data.pin, LOW);
  delayMicroseconds(3);
  pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

uint8_t OneWire2ReadBit(void) {
  digitalWrite(DS18X20Data.pin_out, LOW);
  delayMicroseconds(3);
  digitalWrite(DS18X20Data.pin_out, HIGH);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

/*------------------------------------------------------------------------------------------*/

void OneWireWrite(uint8_t v) {
  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    OneWireWriteBit((bit_mask & v) ? 1 : 0);
  }
}

uint8_t OneWireRead(void) {
  uint8_t r = 0;

  if (!DS18X20Data.dual_mode) {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire1ReadBit()) {
        r |= bit_mask;
      }
    }
  } else {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire2ReadBit()) {
        r |= bit_mask;
      }
    }
  }
  return r;
}

void OneWireSelect(const uint8_t rom[8]) {
  OneWireWrite(W1_MATCH_ROM);
  for (uint32_t i = 0; i < 8; i++) {
    OneWireWrite(rom[i]);
  }
}

uint8_t OneWireSearch(uint8_t *newAddr) {
  uint8_t id_bit_number = 1;
  uint8_t last_zero = 0;
  uint8_t rom_byte_number = 0;
  uint8_t search_result = 0;
  uint8_t id_bit;
  uint8_t cmp_id_bit;
  unsigned char rom_byte_mask = 1;
  unsigned char search_direction;

  if (!onewire_last_device_flag) {
    if (!OneWireReset()) {
      onewire_last_discrepancy = 0;
      onewire_last_device_flag = false;
      onewire_last_family_discrepancy = 0;
      return false;
    }
    OneWireWrite(W1_SEARCH_ROM);
    do {
      if (!DS18X20Data.dual_mode) {
        id_bit     = OneWire1ReadBit();
        cmp_id_bit = OneWire1ReadBit();
      } else {
        id_bit     = OneWire2ReadBit();
        cmp_id_bit = OneWire2ReadBit();
      }
      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
      } else {
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;
        } else {
          if (id_bit_number < onewire_last_discrepancy) {
            search_direction = ((onewire_rom_id[rom_byte_number] & rom_byte_mask) > 0);
          } else {
            search_direction = (id_bit_number == onewire_last_discrepancy);
          }
          if (search_direction == 0) {
            last_zero = id_bit_number;
            if (last_zero < 9) {
              onewire_last_family_discrepancy = last_zero;
            }
          }
        }
        if (search_direction == 1) {
          onewire_rom_id[rom_byte_number] |= rom_byte_mask;
        } else {
          onewire_rom_id[rom_byte_number] &= ~rom_byte_mask;
        }
        OneWireWriteBit(search_direction);
        id_bit_number++;
        rom_byte_mask <<= 1;
        if (rom_byte_mask == 0) {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);
    if (!(id_bit_number < 65)) {
      onewire_last_discrepancy = last_zero;
      if (onewire_last_discrepancy == 0) {
        onewire_last_device_flag = true;
      }
      search_result = true;
    }
  }
  if (!search_result || !onewire_rom_id[0]) {
    onewire_last_discrepancy = 0;
    onewire_last_device_flag = false;
    onewire_last_family_discrepancy = 0;
    search_result = false;
  }
  for (uint32_t i = 0; i < 8; i++) {
    newAddr[i] = onewire_rom_id[i];
  }
  return search_result;
}

bool OneWireCrc8(uint8_t *addr) {
  uint8_t crc = 0;
  uint8_t len = 8;

  while (len--) {
    uint8_t inbyte = *addr++;          // from 0 to 7
    for (uint32_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) {
        crc ^= 0x8C;
      }
      inbyte >>= 1;
    }
  }
  return (crc == *addr);               // addr 8
}

/********************************************************************************************/

void Ds18x20Init(void) {
  DS18X20Data.pin = Pin(GPIO_DSB);
  DS18X20Data.input_mode = Settings->flag3.ds18x20_internal_pullup ? INPUT_PULLUP : INPUT;  // SetOption74 - Enable internal pullup for single DS18x20 sensor

  if (PinUsed(GPIO_DSB_OUT)) {
    DS18X20Data.pin_out = Pin(GPIO_DSB_OUT);
    DS18X20Data.dual_mode = true;    // Dual pins mode as used by Shelly
    pinMode(DS18X20Data.pin_out, OUTPUT);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  }

  onewire_last_discrepancy = 0;
  onewire_last_device_flag = false;
  onewire_last_family_discrepancy = 0;
  for (uint32_t i = 0; i < 8; i++) {
    onewire_rom_id[i] = 0;
  }

  uint64_t ids[DS18X20_MAX_SENSORS];
  DS18X20Data.sensors = 0;
  while (DS18X20Data.sensors < DS18X20_MAX_SENSORS) {
    if (!OneWireSearch(ds18x20_sensor[DS18X20Data.sensors].address)) {
      break;
    }
    if (OneWireCrc8(ds18x20_sensor[DS18X20Data.sensors].address) &&
       ((ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18S20_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS1822_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18B20_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == MAX31850_CHIPID))) {
      ds18x20_sensor[DS18X20Data.sensors].index = DS18X20Data.sensors;
      ids[DS18X20Data.sensors] = ds18x20_sensor[DS18X20Data.sensors].address[0];  // Chip id
      for (uint32_t j = 6; j > 0; j--) {
        ids[DS18X20Data.sensors] = ids[DS18X20Data.sensors] << 8 | ds18x20_sensor[DS18X20Data.sensors].address[j];
      }
      DS18X20Data.sensors++;
    }
  }
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    for (uint32_t j = i + 1; j < DS18X20Data.sensors; j++) {
      if (ids[ds18x20_sensor[i].index] > ids[ds18x20_sensor[j].index]) {  // Sort ascending
        std::swap(ds18x20_sensor[i].index, ds18x20_sensor[j].index);
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), DS18X20Data.sensors);
}

void Ds18x20Convert(void) {
  OneWireReset();
#ifdef W1_PARASITE_POWER
  // With parasite power address one sensor at a time
  if (++DS18X20Data.current_sensor >= DS18X20Data.sensors)
    DS18X20Data.current_sensor = 0;
  OneWireSelect(ds18x20_sensor[DS18X20Data.current_sensor].address);
#else
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
#endif
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

bool Ds18x20Read(uint8_t sensor) {
  float temperature;
  uint8_t data[9];
  int8_t sign = 1;

  uint8_t index = ds18x20_sensor[sensor].index;
  if (ds18x20_sensor[index].valid) { ds18x20_sensor[index].valid--; }
  for (uint32_t retry = 0; retry < 3; retry++) {
    OneWireReset();
    OneWireSelect(ds18x20_sensor[index].address);
    OneWireWrite(W1_READ_SCRATCHPAD);
    for (uint32_t i = 0; i < 9; i++) {
      data[i] = OneWireRead();
    }
    if (OneWireCrc8(data)) {
      switch(ds18x20_sensor[index].address[0]) {
        case DS18S20_CHIPID: {
          int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
          temperature = ConvertTemp(tempS * 0.0625f - 0.250f);
          break;
        }
        case DS1822_CHIPID:
        case DS18B20_CHIPID: {
          if (data[4] != 0x7F) {
            data[4] = 0x7F;                 // Set resolution to 12-bit
            OneWireReset();
            OneWireSelect(ds18x20_sensor[index].address);
            OneWireWrite(W1_WRITE_SCRATCHPAD);
            OneWireWrite(data[2]);          // Th Register
            OneWireWrite(data[3]);          // Tl Register
            OneWireWrite(data[4]);          // Configuration Register
            OneWireSelect(ds18x20_sensor[index].address);
            OneWireWrite(W1_WRITE_EEPROM);  // Save scratchpad to EEPROM
#ifdef W1_PARASITE_POWER
            DS18X20Data.w1_power_until = millis() + 10; // 10ms specified duration for EEPROM write
#endif
          }
          uint16_t temp12 = (data[1] << 8) + data[0];
          if (temp12 > 2047) {
            temp12 = (~temp12) +1;
            sign = -1;
          }
          temperature = ConvertTemp(sign * temp12 * 0.0625f);  // Divide by 16
          break;
        }
        case MAX31850_CHIPID: {
          int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
          temperature = ConvertTemp(temp14 * 0.0625f);  // Divide by 16
          break;
        }
      }
      ds18x20_sensor[index].temperature = temperature;
      if (Settings->flag5.ds18x20_mean) {
        if (ds18x20_sensor[index].numread++ == 0) {
          ds18x20_sensor[index].temp_sum = 0;
        }
        ds18x20_sensor[index].temp_sum += temperature;
      }
      ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
      return true;
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Name(uint8_t sensor) {
  uint8_t index = sizeof(ds18x20_chipids);
  while (--index) {
    if (ds18x20_sensor[ds18x20_sensor[sensor].index].address[0] == ds18x20_chipids[index]) {
      break;
    }
  }
  GetTextIndexed(DS18X20Data.name, sizeof(DS18X20Data.name), index, kDs18x20Types);
  if (DS18X20Data.sensors > 1) {
#ifdef DS18x20_USE_ID_AS_NAME
    char address[17];
    for (uint32_t j = 0; j < 3; j++) {
      sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[sensor].index].address[3-j]);  // Only last 3 bytes
    }
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%s"), DS18X20Data.name, IndexSeparator(), address);
#else
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%d"), DS18X20Data.name, IndexSeparator(), sensor +1);
#endif
  }
}

/********************************************************************************************/

void Ds18x20EverySecond(void) {
  if (!DS18X20Data.sensors) { return; }

#ifdef W1_PARASITE_POWER
  // skip access if there is still an eeprom write ongoing
  unsigned long now = millis();
  if (now < DS18X20Data.w1_power_until) { return; }
#endif
  if (TasmotaGlobal.uptime & 1
#ifdef W1_PARASITE_POWER
      // if more than 1 sensor and only parasite power: convert every cycle
      || DS18X20Data.sensors >= 2
#endif
  ) {
    // 2mS
    Ds18x20Convert();          // Start conversion, takes up to one second
  } else {
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      // 12mS per device
      if (!Ds18x20Read(i)) {   // Read temperature
        Ds18x20Name(i);
        AddLogMissed(DS18X20Data.name, ds18x20_sensor[ds18x20_sensor[i].index].valid);
#ifdef USE_DS18x20_RECONFIGURE
        if (!ds18x20_sensor[ds18x20_sensor[i].index].valid) {
          memset(&ds18x20_sensor, 0, sizeof(ds18x20_sensor));
          Ds18x20Init();       // Re-configure
        }
#endif  // USE_DS18x20_RECONFIGURE
      }
    }
  }
}

void Ds18x20Show(bool json) {
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    uint8_t index = ds18x20_sensor[i].index;

    if (ds18x20_sensor[index].valid) {   // Check for valid temperature
      Ds18x20Name(i);

      if (json) {
        if (Settings->flag5.ds18x20_mean) {
          if ((0 == TasmotaGlobal.tele_period) && ds18x20_sensor[index].numread) {
            ds18x20_sensor[index].temperature = ds18x20_sensor[index].temp_sum / ds18x20_sensor[index].numread;
            ds18x20_sensor[index].numread = 0;
          }
        }
        char address[17];
        for (uint32_t j = 0; j < 6; j++) {
          sprintf(address+2*j, "%02X", ds18x20_sensor[index].address[6-j]);  // Skip sensor type and crc
        }
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
          DS18X20Data.name, address, Settings->flag2.temperature_resolution, &ds18x20_sensor[index].temperature);
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          DomoticzFloatSensor(DZ_TEMP, ds18x20_sensor[index].temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          KnxSensor(KNX_TEMPERATURE, ds18x20_sensor[index].temperature);
        }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(DS18X20Data.name, ds18x20_sensor[index].temperature);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns05(uint8_t function) {
  bool result = false;

  if (PinUsed(GPIO_DSB)) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
      case FUNC_EVERY_SECOND:
        Ds18x20EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ds18x20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
#endif  // ESP8266










// /*
//   mHVAC.cpp - HVAC Controller

//   Copyright (C) 2021  Michael

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.

//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.

//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */


// /**
//  * @brief 
//  * For now I need to disable multiple pin support, as the hardwarepins_templateparsing would need to check and start multiple twowire if that was set
//  * forcing only one this time
//  * 
//  */
// void mDS18X::Pre_Init()
// {

//   uint8_t sensor_count = 0;
//   uint8_t sensor_group_count = 0;
//   settings.nSensorsFound = 0;

//   if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID)) 
//   {  // not set when 255
  
//     sensor_group[sensor_group_count].pin = pCONT_pins->GetPin(GPIO_DSB_1OF2_ID);
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB_1OF2_ID 1 Valid %d"), pCONT_pins->GetPin(GPIO_DSB_1OF2_ID));

//     sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
//     sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
//     sensor_group[sensor_group_count].dallas->begin();

//     sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
//     #ifdef ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT
//     sensor_group[sensor_group_count].sensor_count = ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT;
//     #endif
//     //increment that we have another sensor group added IF we have sensors attached
//     if(sensor_group[sensor_group_count].sensor_count){
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB1 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
//       settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
//       sensor_group_count++;
//     }else{
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
//       // delay(5000);
//     }
//   }

//   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);

//   if (pCONT_pins->PinUsed(GPIO_DSB_2OF2_ID)) {  // not set when 255
//     sensor_group[sensor_group_count].pin = pCONT_pins->GetPin(GPIO_DSB_2OF2_ID);
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB_2OF2_ID Pin 2 Valid %d"),sensor_group[sensor_group_count].pin);
    
//     sensor_group[sensor_group_count].onewire = new OneWire(sensor_group[sensor_group_count].pin);
//     sensor_group[sensor_group_count].dallas = new DallasTemperature(sensor_group[sensor_group_count].onewire);
//     sensor_group[sensor_group_count].dallas->begin();
//     // Get sensors connected to this pin
//     sensor_group[sensor_group_count].sensor_count = sensor_group[sensor_group_count].dallas->getDeviceCount();
//     #ifdef ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT
//     sensor_group[sensor_group_count].sensor_count = ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT;
//     #endif
//     //increment that we have another sensor group added IF we have sensors attached
//     if(sensor_group[sensor_group_count].sensor_count){
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "GPIO_DSB2 sensor_group[%d].sensor_count=%d"),sensor_group_count,sensor_group[sensor_group_count].sensor_count);
//       settings.nSensorsFound += sensor_group[sensor_group_count].sensor_count;
//       sensor_group_count++;
//     }else{
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "NO SENSORS FOUND"));
//     }
//   }

//   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "sensor_group_count=%d"),sensor_group_count);
//   if(settings.nSensorsFound){
//     settings.fEnableSensor = true;
//     settings.group_count = sensor_group_count;
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "settings.fEnableSensor, %d sensors"),settings.nSensorsFound);
//   }


//   //   sensor_group[0].onewire = new OneWire(15);
//   //   sensor_group[0].dallas  = new DallasTemperature(sensor_group[0].onewire);     
//   //   sensor_group[0].dallas->begin();

//   // delay(5000);

// }




// void mDS18X::Init(void){

//   // AddLog(LOG_LEVEL_DEBUG,PSTR("mDS18X::init"));

//   // sensor group 1 exists
//   uint8_t sensor_group_count = 0;
//   uint8_t sensor_count = 0;
//   uint8_t sensors_attached = 0;

//   sensors_attached +=  sensor_group[sensor_group_count].sensor_count;
//   if(sensors_attached>=DB18_SENSOR_MAX){
//     sensor_group[sensor_group_count].sensor_count = DB18_SENSOR_MAX; // set limit
//     AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DSB "DB18_SENSOR_MAX limit reached"));
//   }

//   // load sensor data into their own sensor struct GROUP
//   for(uint8_t sensor_group_id=0;
//               sensor_group_id<settings.group_count;
//               sensor_group_id++
//       ){
//     // load each single sensor
//     uint8_t group_sensor_found = sensor_group[sensor_group_id].sensor_count;

//     if(group_sensor_found){
    
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "group_sensor_found=%d %d"),sensor_group_id,group_sensor_found);

//       //if sensor limit, exit
//       for(uint8_t sensor_id=0;
//                   sensor_id<group_sensor_found;
//                   sensor_id++
//           ){
          
//           AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "FOR sensor_id=%d, group_sensor_found=%d"),sensor_id,group_sensor_found);
//         // get sensor and add to list 
        
//         if(sensor_group[sensor_group_id].dallas->getAddress(sensor[sensor_count].address,sensor_id))
//         {  //what is this then, already stored?
//           // Remember group this sensor came from 
          
//           AddLog_Array(LOG_LEVEL_INFO, "AFTER getAddress", sensor[sensor_count].address, (uint8_t)sizeof(sensor[sensor_count].address));

//           sensor[sensor_count].sensor_group_id = sensor_group_id;
//           // sensor[sensor_count].address_stored = ;

//           // Set sensor precision      

//           sensor_group[sensor_group_id].dallas->setResolution(sensor[sensor_count].address, TEMPERATURE_PRECISION);

//           // Add delay to enable setresolution to work better
//           delay(50);


//           AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "Pin Group %d, count %d, sensor count %d"),sensor_group_id,sensor_id,sensor_count);
//           // WDT_FEED();
//           // WDT_RESET();
//           sensor_count++; // increment how many is found
//           //limit if number of sensors is reached
//           if(sensor_count>DB18_SENSOR_MAX){ break; }
//         }else{
//           AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "getAddress failed"));        
//         }
//       }//end for
//     }
//   }


//   if(!sensor_count){    
//     db18_sensors_active = 0; // reset to none found so backoff will correctly happen
//     tSavedMeasureSensor = millis() + 10000; // 10 second backoff
//     ALOG_DBG(PSTR(D_LOG_DSB "None Found: Backoff 10s"));
//     return;
//   }

//   // Print everything stored to test
//   #ifdef SPLASH_ADDRESS_ON_INIT
//     for(int i=0;i<DB18_SENSOR_MAX;i++){
//       printAddress(sensor[i].address);
//     }
//   #endif

//   db18_sensors_active = sensor_count;

// }//end init

// void mDS18X::Scan_ReportAsJsonBuilder()
// {
//   // For now, just re-init the process, but likely this should instead be subdivided into its own subfunction inside init
  
//   Pre_Init();
//   Init();
//   AddLog(LOG_LEVEL_WARN, PSTR("Pre_Init|Init without template load, may cause issues. Using anyway for scan. It may require restart to fix"));
//   // I probably need to research the names here? using the template, because I do, stop "init" as this may break it

  
//   // AddLog(LOG_LEVEL_DEBUG,PSTR("DS18: Add(%d) %d,%d,%d,%d,%d,%d,%d,%d"),
//   // index,
//   //   deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
//   //   deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);

//   bool flag_found_any = false;
//   bool flag_started_object = false;


//   // Check all pins  
//   for(uint8_t sensor_group_id=0;
//               sensor_group_id<SENSOR_GROUP_MAX_COUNT;
//               sensor_group_id++
//       ){

//   if(sensor_group[sensor_group_id].dallas != nullptr)
//   {
//     uint8_t sensor_count = sensor_group[sensor_group_id].dallas->getDeviceCount();

//     if(sensor_count)
//     {
//       flag_found_any = true;
//     }

//     if(flag_found_any && !flag_started_object)
//     {
//       flag_started_object = true;
//       JBI->Level_Start(D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR);
//     }

//     AddLog(LOG_LEVEL_TEST, PSTR("sensor_count[%d]=%d"),sensor_group_id,sensor_count);

//     char buffer[40];
//     DeviceAddress address;

//     for(uint8_t sensor_id=0;
//                 sensor_id<sensor_count;
//                 sensor_id++
//     ){

//     AddLog(LOG_LEVEL_TEST, PSTR("sensor_count[%d]=%d \t\t%d"),sensor_group_id,sensor_count,sensor_id);

//       if(sensor_group[sensor_group_id].dallas->getAddress(address, sensor_id))
//       {

//         snprintf(buffer, sizeof(buffer), "pin%d_id%d", sensor_group[sensor_group_id].pin , sensor_id);
        
//         JBI->Array_Start(buffer);
//         for(int i=0;i<8;i++)
//         {
//           JBI->Add(address[i]);
//         }
//         JBI->Array_End();
      
//       }

//     }
        

//   }
//     if(flag_found_any)
//     {
//       JBI->Level_End();
//     }



// }



// }

// void mDS18X::SplitTask_UpdateSensors(uint8_t sensor_group_id, uint8_t require_completion){

//   unsigned long timeout = millis();
//   do{
    
//     switch(sReadSensor){
//       case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
//       case SPLIT_TASK_TIMEOUT_ID: default:
//       case SPLIT_TASK_SEC1_ID:

//         sensor_group[sensor_group_id].dallas->requestTemperatures();
//         sReadSensor = SPLIT_TASK_SEC2_ID;
//         break;
//       case SPLIT_TASK_SEC2_ID:{
//         char buffer[50];
//         anychanged=false;

//         // Check all sensors, if they belong to this group, get their temp
//         for(int sensor_id=0;
//                 sensor_id<db18_sensors_active;
//                 sensor_id++){

//           if(sensor[sensor_id].sensor_group_id == sensor_group_id)
//           {
//             #ifdef ENABLE_DEVFEATURE_ADDLOG_FAILED_SENSOR_WAIT_TIME
//               uint32_t wait_time = millis();
//             #endif 

//             if((tmp_float = sensor_group[sensor_group_id].dallas->getTempC(sensor[sensor_id].address))!=DEVICE_DISCONNECTED_C)
//             {
//               if(sensor[sensor_id].reading.val != tmp_float){ 
//                 sensor[sensor_id].reading.ischanged = true; 
//                 anychanged=true;// check if updated
//               }else{ 
//                 sensor[sensor_id].reading.ischanged = false; 
//               }
//               sensor[sensor_id].reading.val = tmp_float;
//               sensor[sensor_id].reading.isvalid = true; 
//               sensor[sensor_id].reading.captureupsecs = pCONT_time->uptime.seconds_nonreset;
//               // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
//               ALOG_DBM( PSTR(D_LOG_DB18 D_MEASURE "[%d|%d] %02X \"%s\" = [%d]"), sensor_group_id, sensor_id, sensor[sensor_id].address[7], DLI->GetDeviceName_WithModuleUniqueID(  GetModuleUniqueID(), sensor[sensor_id].address_id, buffer, sizeof(buffer)),(int)tmp_float);
//             }
//             else
//             {
//               sensor[sensor_id].reading.isvalid = false;
              
//               // pCONT_sup->GetTextIndexed_P(name_tmp, sizeof(name_tmp), sensor_id, name_buffer);
            
            
//               ALOG_ERR( PSTR(D_LOG_DB18 D_MEASURE "[%d|%d] %02X \"%s\" = " D_FAILED " WaitTime:%d"), sensor_group_id, sensor_id, sensor[sensor_id].address[7], DLI->GetDeviceName_WithModuleUniqueID(  GetModuleUniqueID(), sensor[sensor_id].address_id, buffer, sizeof(buffer)), millis()-wait_time);
            
            
            
//             }
          
//           } // end if
//         }//end for
//         sReadSensor = SPLIT_TASK_DONE_ID;
//         require_completion = DONE_COMPLETE;
//       }break;
//     } // end switch

//     if(require_completion){ delay(1); }//delay required if we are going to do multiple calls

//     if(abs(millis()-timeout)>=2000){
//       sReadSensor = SPLIT_TASK_TIMEOUT_ID;
//       break;
//     }

//   }while(require_completion); // loops once even if false

// }//end function


// /**
//  * Create a function that can scan for devices, and return all addresses as json formatted string (that can then be sent via serial or mqtt)
//  * Dont include the start and end chars, so this can be optional appended later as part of a larger json message
//  * */
// void mDS18X::ScanSensorAddresses_JsonObject(char* buffer, uint8_t buflen)
// {

// // uint8_t found_count = 0;
// // uint8_t found_on_pin = 0;
  
// //   snprintf_P(buffer, buflen, PSTR("{\"" D_JSON_I2CSCAN "\":\"" D_JSON_I2CSCAN_DEVICES_FOUND_AT));
  
// //     // wire->beginTransmission(address);
// //     // error = wire->endTransmission();

// //     /**
// //      * Search pin 1 if set
// //      * */
// //     if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID))
// //     {
// //       if(found_on_pin = sensor_group[0].dallas->getDeviceCount())
// //       {

// //         for (uint8_t index = 0; index < found_on_pin; index++) {
         
// //           // snprintf_P(buffer, buflen, PSTR("[%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X]"), devs, address); //as hex
// //           snprintf_P(buffer, buflen, PSTR("[%d,%d,%d,%d,%d,%d,%d,%d]"), 


          
          
          
          
// //           ); //as dec
// //  if()
// //         }


// //       }
// //     }



// //   for (uint8_t index = 1; index < DB18_SENSOR_MAX; index++) {


// //     if (0 == error) {
// //       any = 1;
// //       snprintf_P(devs, devs_len, PSTR("%s 0x%02x"), devs, address);
// //     }
// //     else if (error != 2) {  // Seems to happen anyway using this scan
// //       any = 2;
// //       // snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"Error %d at 0x%02x"), error, address);
// //       break;
// //     }
// //   }
// //   if (any) {
// //     strncat(devs, "\"}", devs_len - strlen(devs) -1);
// //   }
// //   else {
// //     snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"" D_JSON_I2CSCAN_NO_DEVICES_FOUND "\"}"));
// //   }



//   return;
// }


// //pubsub.ppublish("sensors/pipe","{\"ds\":{\"temp\":\"1\"},\"us\":{\"temp\":\"2\"},\"wb\":{\"temp\":\"3\"},\"ih\":{\"temp\":\"4\"},\"tt\":{\"temp\":\"5\"},\"tb\":{\"temp\":\"6\"}}"));
// // Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
// uint8_t mDS18X::ConstructJSON_Sensor(uint8_t json_level){

//   JBI->Start();
  
//   char buffer[40];
//   char title [40];
  
//   uint8_t corrected_sensor_id = 0;
  
//   for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){ //db18_sensors_active
    
//     if(sensor[sensor_id].reading.ischanged || (json_level<=JSON_LEVEL_IFCHANGED)){  

//       JBI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID(  GetModuleUniqueID(),sensor[sensor_id].address_id,buffer,sizeof(buffer)));         
//         JBI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].reading.val);
//         JBI->Add(D_JSON_ADDRESS,     sensor[sensor_id].address[7]);

//         if(json_level >= JSON_LEVEL_DETAILED)
//         {
//           JBI->Add(D_JSON_ISVALID, sensor[sensor_id].reading.isvalid);
//           JBI->Add(D_JSON_CAPTURE_UPSECONDS, sensor[corrected_sensor_id].reading.captureupsecs);
//         }

//         if(json_level >= JSON_LEVEL_DEBUG)
//         {
//           JBI->Add("set_address",sensor[sensor_id].flag.set_from_known_address);
//           JBI->Add("ID", sensor_id);
//           JBI->Add("Corrected_ID", corrected_sensor_id);
//         }
//       JBI->Level_End();  
//     }

//   } // END for

//   return JBI->End();

// }

// uint8_t mDS18X::ConstructJSON_Settings(uint8_t json_level){

//   char buffer[20];

//   JBI->Start();
  
//   JBI->Add("found", db18_sensors_active);

//   JBI->Level_Start("Address");

//   for(int id=0;id<settings.nSensorsFound;id++){
//     snprintf(buffer, sizeof(buffer), "sens%d_%d_%d", id, sensor[id].address[6], sensor[id].address[7]);
//     JBI->Array_AddArray(buffer, &sensor[id].address[0], 8);
//   }


//   JBI->Array_Start("getResolution");
//   // check both groups
//   for(uint8_t sensor_group_id=0;
//                   sensor_group_id<settings.group_count;
//                   sensor_group_id++
//   ){
//     // Search for id match 
//       for(int sensor_id=0;
//                 sensor_id<db18_sensors_active;
//                 sensor_id++
//       ){

//         if(sensor[sensor_id].sensor_group_id == sensor_group_id)
//         {
//           uint8_t resolution = sensor_group[sensor_group_id].dallas->getResolution(sensor[sensor_id].address);
          
//           JBI->Add(resolution);

//         }

//       }

//   }


//   JBI->Array_End();






//   JBI->Add("found_p0", sensor_group[0].sensor_count);
//   JBI->Add("found_p1", sensor_group[1].sensor_count);


//   JBI->Add("pin0", sensor_group[0].pin);
//   JBI->Add("pin1", sensor_group[1].pin);

//   JBI->Add("pindb0", pCONT_pins->GetPin(GPIO_DSB_1OF2_ID));
//   JBI->Add("pindb1", pCONT_pins->GetPin(GPIO_DSB_2OF2_ID));

//   JBI->Add("pindb0PinUsed", pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID)?"true":"false");
//   JBI->Add("pindb1PinUsed", pCONT_pins->PinUsed(GPIO_DSB_2OF2_ID)?"true":"false");

//   // JBI->Add("count1", sensor_group[0].dallas->getDeviceCount());
//   // JBI->Add("count2", sensor_group[1].dallas->getDeviceCount());




//   JBI->Level_End();

//   // JBI->Level_Start("AddressFull");

//   //   root["rate_measure_ms"] = settings.rate_measure_ms;
//   //   // root["tRateSecs"] = mqtthandler_sensor_ifchanged.tRateSecs;
    
//   //   root["sens_tele_rate"] =mqtthandler_sensor_teleperiod.tRateSecs;
//   //   root["sens_ifchanged_rate"] =mqtthandler_sensor_ifchanged.tRateSecs;
//   //   root["sett_tele_rate"] =mqtthandler_settings_teleperiod.tRateSecs;

//   //   //Add here - function that takes handler list and appends its config here by name 


//   return JBI->End();


// }

// int8_t mDS18X::FindStructIndexByAddressID(int8_t address_id){

//   int8_t struct_index_id = -1;

//   if(address_id == -1){ return -1; } // invalid/unset id

//   for(uint8_t ii=0;ii<DB18_SENSOR_MAX;ii++){
//     if(address_id == sensor[ii].address_id){
//       return ii;
//     }
//   }

//   return -1;

// }


// // function to print a device address
// void mDS18X::printAddress(DeviceAddress deviceAddress, int8_t index){
//   // for (uint8_t i = 0; i < 8; i++){// zero pad the address if necessary
//   //   if (deviceAddress[i] < 16) Serial.print("0");
//   //   Serial.print(deviceAddress[i], HEX);
//   // }
  
//   // Serial.print("\tdec: ");
//   // for (uint8_t i = 0; i < 8; i++){// zero pad the address if necessary
//   //   Serial.print(deviceAddress[i]);
//   //   if(i<7){Serial.print(",");}
//   // }
//   // Serial.println();
//   // AddLog(LOG_LEVEL_DEBUG,PSTR("printAddress (%d) %X:%X:%X:%X:%X:%X:%X:%X"),
//   // index,
//   //   deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
//   //   deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);

//   AddLog(LOG_LEVEL_DEBUG,PSTR("DS18: [%d]=> %d,%d,%d,%d,%d,%d,%d,%d"),
//   index,
//     deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
//     deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);

// }

// #ifdef USE_MODULE_NETWORK_WEBSERVER
// void mDS18X::WebAppend_Root_Status_Table_Draw(){

//   char buffer[100];

//   for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){ //add number in name? List needed? also hold user defined name?
//     JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       JBI->Append_P(PSTR("<td>DB18 %02d Temperature %s</td>"),sensor_id,DLI->GetDeviceName_WithModuleUniqueID(  GetModuleUniqueID(), sensor[sensor_id].address_id, buffer, sizeof(buffer)));
//       JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_db18","?");   
//     JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }

// }

// //append to internal buffer if any root messages table
// void mDS18X::WebAppend_Root_Status_Table_Data(){
  
//   uint8_t sensor_counter = 0;
  
//   JBI->Array_Start("tab_db18");// Class name
//   uint8_t corrected_sensor_id = 0;

//   for(int sensor_id=0;sensor_id<db18_sensors_active;sensor_id++){
    
//     char colour_ctr[10];
//     char table_row[25]; memset(table_row,0,sizeof(table_row));     

//     char value_ctr[8];
    
//     // corrected_sensor_id = GetCorrectedDeviceID(row);
    

//     pCONT_sup->dtostrfd(sensor[sensor_id].reading.val,2,value_ctr);

//     sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
    
//     if(sensor[sensor_id].reading.val<=25){
//       sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
//     }else
//     if(sensor[sensor_id].reading.val<30){ //warm
//       sprintf(colour_ctr,"%s","#fcba03");
//     }else
//     if(sensor[sensor_id].reading.val>=30){ //hot
//       sprintf(colour_ctr,"%s","#ff0000");
//     }else{
//       sprintf(colour_ctr,"%s","#ffffff");
//     }

//     JBI->Level_Start();
//       JBI->Add("id",sensor_id);
//       JBI->Add("ih",table_row);
//       JBI->Add("fc",colour_ctr);
//     JBI->Level_End();
  
//   }

//   JBI->Array_End();

// }
// #endif // USE_MODULE_NETWORK_WEBSERVER




// // Search for address, if found, store id against it in struct
// // Assumes template load AFTER init of sensors
// void mDS18X::SetIDWithAddress(uint8_t address_id, uint8_t* address_to_find){

//   // memcpy(sensor[device_id].address_stored,address_to_find,sizeof(sensor[device_id].address_stored));

//   uint8_t sensor_count = 0; // reset
//   // Address moved into struct, I need to rearrange now with ids
 
//   AddLog(LOG_LEVEL_INFO, "searching start address_id=%d", address_id);

//   AddLog_Array(LOG_LEVEL_COMMANDS, "address_to_find",address_to_find, (uint8_t)8);
//   AddLog_Array(LOG_LEVEL_COMMANDS, "address_saved",  sensor[0].address, (uint8_t)8);


//   for(uint8_t sensor_group_id=0; sensor_group_id<settings.group_count; sensor_group_id++){
    
//     AddLog(LOG_LEVEL_INFO, "\tsearching iter %d",sensor_group_id);
    
//     for(uint8_t sensor_id=0; sensor_id<sensor_group[sensor_group_id].sensor_count; sensor_id++){
//       // Check address has been set    

//       AddLog(LOG_LEVEL_INFO, "\t\tsensor_id %d",sensor_id);
      
//       if(memcmp(
//         sensor[sensor_count].address,
//         address_to_find,
//         sizeof(sensor[sensor_count].address
//         ))==0){ // 0 means equal


// //temp fix
// /**
//  * Issue#1 address_id stored in sensor struct complicates retrieving devicename from module_id, 
//  * Fix1: Remove address_id, instead making it the struct index, thus reordered contents of struct is required (maybe using address to poll sensor that is stored in struct, ie named sensor X, in index X, uses this address... if not, just append address as new struct indexes)
//  * This means, on setting name, I should search for the address of X and put it into index X.. swap?
//  * */
//         sensor[sensor_count].address_id = address_id;   
//         // sensor[original_device_id].id = sensor_count;    

//         sensor[sensor_count].flag.set_from_known_address = sensor_count;
      
      
//         AddLog_Array(LOG_LEVEL_INFO, "isconnected", sensor[sensor_count].address, (uint8_t)sizeof(sensor[sensor_count].address));
//         AddLog(LOG_LEVEL_TEST, PSTR("Searched %02d, Found %02d, Id from %d to %d %d"),
//           address_to_find[7],
//           sensor[sensor_count].address[7],
//           sensor_count,
//           address_id   ,
//           sensor[address_id].address_id     
//         );

//         break; // stop looking for more


//       }
//       else{
//         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DSB "getAddress failed - no find with search %d @ %d"),sensor[sensor_count].address[7],sensor_id);   
//       }

//       sensor_count++;


//     }
//   }

// }


// void mDS18X::EveryLoop(){
//   if(mTime::TimeReachedNonReset(&tSavedMeasureSensor,settings.rate_measure_ms)){

//     // AddLog(LOG_LEVEL_TEST, PSTR("db18_sensors_activ\t\te=%d"), db18_sensors_active);
    
//     // AddLog(LOG_LEVEL_DEBUG,PSTR("mDS18X::here2 %d %d %d"),db18_sensors_active, settings.rate_measure_ms, settings.group_count);

//     if(db18_sensors_active==0) // Retry init if failed
//     {
//       Init(); //search again
//       // set a cooldown period if sensor was not found of X seconds
//       if(db18_sensors_active==0)
//       { //still no sensor found, wait
//         tSavedMeasureSensor = millis()+10000; //30 seocnds backoff
//       }
//     }
//     else
//     {
//       // AddLog(LOG_LEVEL_DEBUG,PSTR("SplitTask_UpdateSensors"));
//         for(uint8_t sensor_group_id=0;
//                   sensor_group_id<settings.group_count;
//                   sensor_group_id++
//           ){
//         SplitTask_UpdateSensors(sensor_group_id, REQUIRE_COMPLETE); // request & measure
//         if(sReadSensor==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
//           sReadSensor=SPLIT_TASK_SUCCESS_ID;//temp way -- need another case  SUCCESS
//           tSavedMeasureSensor = millis();//Serial.print(F("[tPip-"));//update time every 60 seconds
//         }
//       }
//     }
//   }

// }//end

// #endif

