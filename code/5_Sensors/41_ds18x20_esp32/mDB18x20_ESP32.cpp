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

const char* mDB18x20_ESP32::PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR = D_MODULE_SENSORS_DB18S20_CTR;
const char* mDB18x20_ESP32::PM_MODULE_SENSORS__DS18X20_ESP32_2023__FRIENDLY_CTR = D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;

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
    case FUNC_EVERY_FIVE_SECOND:
      // pCONT_set->Function_Template_Load();

        // Ds18x20EverySecond();

    break;
    case FUNC_EVERY_SECOND:
      // Ds18x20Init();
        // AddLog_Array(LOG_LEVEL_COMMANDS, "address0", sensor_new[0].address, (uint8_t)8);
        // AddLog_Array(LOG_LEVEL_COMMANDS, "address1", sensor_new[1].address, (uint8_t)8);
        // ALOG_INF(PSTR("devices_pin1=%d"),devices_pin1);
        // ALOG_INF(PSTR("devices_pin2=%d"),devices_pin2);

        Ds18x20EverySecond();

        // Serial.println();

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



// https://www.analog.com/en/technical-articles/guidelines-for-reliable-long-line-1wire-networks.html
void mDB18x20_ESP32::Scan_ReportAsJsonBuilder()
{
  // For now, just re-init the process, but likely this should instead be subdivided into its own subfunction inside init
  
  Pre_Init();
  Ds18x20Init();
  AddLog(LOG_LEVEL_WARN, PSTR("Pre_Init|Init without template load, may cause issues. Using anyway for scan. It may require restart to fix"));
  // I probably need to research the names here? using the template, because I do, stop "init" as this may break it

  
  // AddLog(LOG_LEVEL_DEBUG,PSTR("DS18: Add(%d) %d,%d,%d,%d,%d,%d,%d,%d"),
  // index,
  //   deviceAddress[0],deviceAddress[1],deviceAddress[2],deviceAddress[3],
  //   deviceAddress[4],deviceAddress[5],deviceAddress[6],deviceAddress[7]);

  bool flag_found_any = false;
  bool flag_started_object = false;


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



}

void mDB18x20_ESP32::Pre_Init(void){
  
  ALOG_INF (PSTR(D_LOG_DSB DEBUG_INSERT_PAGE_BREAK "mDB18x20_ESP32::Pre_Init"));
  // delay(2000);

  DS18X20Data.gpios = 0;
  for (uint32_t pins = 0; pins < MAX_DSB_PINS; pins++) 
  {
    ALOG_INF (PSTR(D_LOG_DSB "PinUsed %d %d"), pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins), pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
    if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins)) {
      ds18x20_gpios[pins] = new OneWire(pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
      ALOG_INF(PSTR("DS18X20Data.gpios=%d"),DS18X20Data.gpios);
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

#ifdef ENABLE_DEVFEATURE_DS18X20_DUAL_SEARCH

void mDB18x20_ESP32::Ds18x20Search(void) {
  uint8_t num_sensors = 0;
  uint8_t sensor = 0;

  for (uint8_t pins = 0; pins < DS18X20Data.gpios; pins++) 
  {
    ds = ds18x20_gpios[pins];
    ds->reset_search();
    for (num_sensors; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
      if (!ds->search(sensor_new[num_sensors].address)) {
        
        ALOG_INF(PSTR("if (!ds->search(sensor_new[num_sensors].address))") );
        ds->reset_search();
        break;
      }else{
        
        ALOG_INF(PSTR("ELSE (!ds->search(sensor_new[num_sensors].address))"));
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

        if(pins==0){devices_pin1++;}
        if(pins==1){devices_pin2++;}

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

  // if(DS18X20Data.sensors==0){
  //   ALOG_HGL(PSTR("Expected DB18 and found none"));
  //   // delay(4000);
  // }


}

#else // ENABLE_DEVFEATURE_DS18X20_DUAL_SEARCH

void mDB18x20_ESP32::Ds18x20Search(void) {
  uint8_t num_sensors = 0;
  uint8_t sensor = 0;

  for (uint8_t pins = 0; pins < DS18X20Data.gpios; pins++) 
  {
    ds = ds18x20_gpios[pins];
    ds->reset_search();
    for (num_sensors; num_sensors < DS18X20_MAX_SENSORS; num_sensors) 
    {
      if (!ds->search(sensor_new[num_sensors].address)) 
      {
        
        // ALOG_INF(PSTR("if (!ds->search(sensor_new[num_sensors].address))") );
        ds->reset_search();
        break;
      }
      else
      {
        
        // ALOG_INF(PSTR("ELSE (!ds->search(sensor_new[num_sensors].address))"));
      }

      // If CRC Ok and Type DS18S20, DS1822, DS18B20 or MAX31850
      if ((OneWire::crc8(sensor_new[num_sensors].address, 7) == sensor_new[num_sensors].address[7]) &&
        ((sensor_new[num_sensors].address[0] == DS18S20_CHIPID) ||
          (sensor_new[num_sensors].address[0] == DS1822_CHIPID) ||
          (sensor_new[num_sensors].address[0] == DS18B20_CHIPID) ||
          (sensor_new[num_sensors].address[0] == MAX31850_CHIPID))
      ){
        sensor_new[num_sensors].alias_index = 0; // Reset
        // ALOG_INF (PSTR(D_LOG_DSB "pins %d"), pins);
        sensor_new[num_sensors].pins_id = pins;
        num_sensors++;

        if(pins==0){devices_pin1++;}
        if(pins==1){devices_pin2++;}

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
  // settings.fSensorCount = num_sensors;

  // if(DS18X20Data.sensors==0){
  //   ALOG_HGL(PSTR("Expected DB18 and found none"));
  //   // delay(4000);
  // }


}

#endif // ENABLE_DEVFEATURE_DS18X20_DUAL_SEARCH

// Looks like I will have to tell everything on a pin to convert, then wait and request their values

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
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR "=%d a%d"), index, sensor_new[index].address[7]);
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

  ALOG_INF(PSTR("DS18X20Data.sensors=%d"),DS18X20Data.sensors);

  if (!DS18X20Data.sensors) 
  {     
    Ds18x20Search();      // Check for changes in sensors number
    return; 
  }

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
        ALOG_INF(PSTR("Read i%d a%d Alias%d =-> %d"), i, sensor_new[i].address[7], sensor_new[i].device_name_index, (int)t);
      } else {
        // Ds18x20Name(i);
        // AddLogMissed(DS18X20Data.name, sensor_new[sensor_new[i].index].valid);
      }
    }
  }

}


#ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
uint8_t mDB18x20_ESP32::ConstructJSON_Debug(uint8_t json_level, bool json_object_start_end_required)
{

  JsonBuilderI->Start();
//     //JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.);

  
  JBI->Level_Start("Addresses");
  for(int sens_i=0;sens_i<DS18X20_MAX_SENSORS;sens_i++)
  {
    JBI->Array_Start_P("%0d", sens_i);
    for (uint32_t i = 0; i < 8; i++) 
    {
      JBI->Add(sensor_new[sens_i].address[i]);
    }
    JBI->Array_End();
  }
  JBI->Level_End();

  return JsonBuilderI->End();

}
#endif // ENABLE_DEBUG_MQTT_CHANNEL_DB18X20


uint8_t mDB18x20_ESP32::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

  JsonBuilderI->Start();
    JsonBuilderI->Add_P("NumDevices", DS18X20Data.sensors);

    JBI->Array_Start("DeviceNameIndex");
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) 
    {
      JBI->Add(sensor_new[i].device_name_index);
    }
    JBI->Array_End();

  return JsonBuilderI->End();

}


uint8_t mDB18x20_ESP32::ConstructJSON_Sensor(uint8_t json_level , bool json_object_start_end_required)
{
  
  char buffer[40];
  char title [40];  
  uint8_t corrected_sensor_id = 0;
  int8_t alias_i = -1;

  JsonBuilderI->Start();

  for (uint32_t sensor_id = 0; sensor_id < DS18X20Data.sensors; sensor_id++) 
  {

    alias_i = sensor_new[sensor_id].device_name_index;
 
    // if(sensor_new[sensor_id].reading.ischanged || (json_level<=JSON_LEVEL_IFCHANGED)){ 

    JBI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), alias_i, buffer, sizeof(buffer)));         
      JBI->Add(D_JSON_TEMPERATURE, sensor_new[sensor_id].reading.val);
      JBI->Add(D_JSON_ADDRESS,     sensor_new[sensor_id].address[7]);

      if(json_level >= JSON_LEVEL_DETAILED)
      {
        JBI->Add(D_JSON_ISVALID, sensor_new[sensor_id].reading.isvalid);
        JBI->Add(D_JSON_CAPTURE_UPSECONDS, sensor_new[corrected_sensor_id].reading.captureupsecs);
      }

      if(json_level >= JSON_LEVEL_DEBUG)
      {
        JBI->Add("ID", sensor_id);
        JBI->Add("Corrected_ID", corrected_sensor_id);
        JBI->Array_Start_P("Device%d", sensor_id);
        
        // for(int a=2;a<7;a++) // Skipping device and CRC values in address
        for(int a=0;a<8;a++) // All address bytes
        {
          JBI->Add(sensor_new[sensor_id].address[a]);
        }
        JBI->Array_End();
      }
    JBI->Level_End(); 

  }

  return JsonBuilderI->End();

}





void mDB18x20_ESP32::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;


  /**
   * @brief New method iters through key/object pair that contains device friendly name/expected address
   *        A uint8_t saves that index, so looking up the name of the address will require the correct index (or left as -1 to unknown)
   * 
   */
  if(jtok = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR])
  {

    // Step 1, ability to find pair count
    // Serial.printf("sizeA=%d\n\r",jtok.size());
    // JsonPair pairs = jtok;
    // Serial.printf("sizeA1=%d\n\r",pairs.size());
    // JsonPair2 pair = pairs.getObjectPair(0);
    // const char* key1 = pair.key.getStr();
    // Serial.println(key1);
    // const char* value1 = pair.value.getStr();
    // Serial.println(value1);

    
    JsonParserArray array_group = jtok; 
    ALOG_COM(PSTR("PM_JSON_SENSORADDRESS"));

    uint8_t pair_count = jtok.size();

    uint8_t address[8];
    uint8_t add_len = 0;
    char name_buf[100];
    uint8_t device_name_index = 0;

    for(int i=0;i<pair_count;i++)
    {
    
      if(i==0){ jtok.nextOne(); }// skip start 
      else    { jtok.skipArray(); } //skip previous array

      // Key needs copied because the getStr() jtok will be moved on
      snprintf(name_buf, sizeof(name_buf), "%s", jtok.getStr());
      
      jtok.nextOne();

      // Get Address from array
      array_group = jtok;
      add_len = 0;
      for(auto address_id : array_group)
      { 
        address[add_len++] = address_id.getInt(); 
      } 

      SetDeviceNameID_WithAddress(name_buf, device_name_index++, address, add_len);

    }


     
  Serial.println("====================================================================================");


    // JsonParserArray array_group = jtok;
      
    // ALOG_COM(PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), D_JSON_SENSORADDRESS, array_group.size()); 


    
  //   uint8_t address_temp[8];
  //   uint8_t address_index = 0;
  //   uint8_t original_device_id = 0;
    
    // for(auto group_iter : array_group) {

      // JsonParserArray array_sensor_address_iter = group_iter;
  //     memset(address_temp,0,sizeof(address_temp));
  //     address_index = 0;
            
      // for(auto address_id : array_group) {
        
    // ALOG_COM(PSTR(D_LOG_DB18 D_PARSING_MATCHED "Key:\"%s\"\t Value:\"%s\""), F(D_JSON_SENSORADDRESS),array_group.get); 
  //       int address = address_id.getInt();
  //       // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
  //       //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
  //       // #endif
  //       address_temp[address_index++] = address;
  //       // if(address_index>7){ break; } //error!
      // }

  //     AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)8);
  //     SetIDWithAddress(original_device_id++, address_temp);
  //     Serial.println();

  //     // DEBUG_LINE_HERE_PAUSE;

  //   }
    
  }



}



void mDB18x20_ESP32::SetDeviceNameID_WithAddress(const char* device_name, uint8_t device_name_index, uint8_t* array_val, uint8_t array_len)
{

  ALOG_INF(PSTR("Key %S"), device_name);
  AddLog_Array(LOG_LEVEL_INFO, "Value", array_val, array_len);

  ALOG_INF(PSTR("Searching to find device with address"));

  AddLog_Array("Searching Input", array_val, array_len);

  int8_t search_id = 0;
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) 
  {
    // AddLog_Array("Searching Saved", sensor_new[i].address, 8);

    if(ArrayCompare(array_val,8, sensor_new[i].address,8))
    {
      // ALOG_INF(PSTR(""));
      AddLog_Array("MATCHED Searching Saved", sensor_new[i].address, 8);
      sensor_new[i].device_name_index = device_name_index;
      break;
    }else
    {
      // ALOG_INF(PSTR("no match"));
    }

  }


}


#ifdef USE_MODULE_NETWORK_MQTT

void mDB18x20_ESP32::MQTTHandler_Init(){

  struct handler<mDB18x20_ESP32>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDB18x20_ESP32::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20_ESP32::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20_ESP32::ConstructJSON_Sensor;

  #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
  ptr = &mqtthandler_debug; //each sensor should have its own debug channel for extra output only when needed
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE__DEBUG__ID;
  ptr->json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20_ESP32::ConstructJSON_Debug;
  #endif // ENABLE_DEBUG_MQTT_CHANNEL_DB18X20

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDB18x20_ESP32::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDB18x20_ESP32::MQTTHandler_Set_TelePeriod()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mDB18x20_ESP32::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS__DS18X20_ESP32_2023__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // sensor enabled


