/**
 * @file mDB18x20.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief DallasTemperature DB18X20
 * @version 1.0
 * @date 2022-04-20
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
#include "mDB18x20.h"

#ifdef USE_MODULE_SENSORS_DS18X20

constexpr uint8_t mDB18x20::ds18x20_chipids[]; // Definition outside the class is required for c++14 or older. c++17 can remove this with "inline"

int8_t mDB18x20::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Ds18x20Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID:
      Scan_ReportAsJsonBuilder();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }
  return 1;


} // END function


void mDB18x20::Pre_Init(void)
{
  
  module_state.mode = ModuleStatus::Initialising;

  module_state.pins_used = 0;
  for (uint8_t pins = 0; pins < MAX_DSB_PINS; pins++) 
  {
    ALOG_INF (PSTR(D_LOG_DSB "PinUsed %d %d"), tkr_pins->PinUsed(GPIO_DSB_1, pins), tkr_pins->GetPin(GPIO_DSB_1, pins));
    if (tkr_pins->PinUsed(GPIO_DSB_1, pins)) 
    {
      ds18x20_gpios[pins] = new OneWire(tkr_pins->GetPin(GPIO_DSB_1, pins));
      ALOG_INF(PSTR(D_LOG_DSB "pins_used %d"), module_state.pins_used);
      module_state.pins_used++;
    }
  }

  if(module_state.pins_used)
  {
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DB18 "Running"));
  }

}


void mDB18x20::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100] = {0};
  if(module_state.devices)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", module_state.devices);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<module_state.devices; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d:\"%s\", ", sensor_id, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_DB18 "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


void mDB18x20::Ds18x20Init(void) 
{
  Ds18x20Search();
  ALOG_INF(PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), module_state.devices);
}


void mDB18x20::Ds18x20Search(void)
{
  uint8_t sensor_count = 0;

  // Optional: if you want a fresh scan each time
  sensor_vector.clear();
  sensor_vector.reserve(DS18X20_MAX_SENSORS * module_state.pins_used);

  for (uint8_t pins = 0; pins < module_state.pins_used; pins++)
  {
    ds = ds18x20_gpios[pins];
    ds->reset_search();

    // NOTE: This limits TOTAL sensors across all pins to DS18X20_MAX_SENSORS,
    // matching your original behaviour. If you want per-pin, say so.
    while (sensor_count < DS18X20_MAX_SENSORS)
    {
      uint8_t search_address[8] = {0};

      if (!ds->search(search_address))
      {
        ds->reset_search();
        break;
      }

      // ---- Build temp sensor, then push only if valid ----
      sensors s{}; // IMPORTANT: value-init (zero + default initializers)
      memcpy(s.address, search_address, sizeof(search_address));
      s.device_name_index = -1;
      s.pins_id = (int8_t)pins;
      s.resolution = 12;
      s.utc_measured_timestamp = 0;

      const bool crc_ok = (OneWire::crc8(s.address, 7) == s.address[7]);
      const uint8_t id = s.address[0];
      const bool type_ok =
          (id == DS18S20_CHIPID) ||
          (id == DS1822_CHIPID)  ||
          (id == DS18B20_CHIPID) ||
          (id == MAX31850_CHIPID);

      if (crc_ok && type_ok)
      {
        sensor_vector.push_back(s);
        sensor_count++;
      }
      // else: ignore (do not push)
    }
  }

  // Assign indices (sequential)
  for (uint32_t i = 0; i < sensor_vector.size(); i++)
  {
    sensor_vector[i].index = (uint8_t)i;
  }

  // Sort sensors by ROM (address bytes), ascending
  // (no std::sort dependency; simple O(N^2) is fine for small N)
  for (uint32_t i = 0; i < sensor_vector.size(); i++)
  {
    for (uint32_t j = i + 1; j < sensor_vector.size(); j++)
    {
      if (memcmp(sensor_vector[i].address, sensor_vector[j].address, 8) > 0)
      {
        std::swap(sensor_vector[i], sensor_vector[j]);
      }
    }
  }

  // Re-assign indices after sort
  for (uint32_t i = 0; i < sensor_vector.size(); i++)
  {
    sensor_vector[i].index = (uint8_t)i;
  }

  module_state.devices = (uint8_t)sensor_vector.size();
  ALOG_DBG(PSTR(D_LOG_DSB "sensor_count %d"), module_state.devices);

  if (module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }
}



void mDB18x20::Ds18x20Convert(void) 
{
  for (uint32_t i = 0; i < module_state.pins_used; i++) {
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

bool mDB18x20::Ds18x20Read(uint8_t sensor, float &t) 
{
  uint8_t data[12];
  int8_t sign = 1;

  t = NAN;

  uint8_t index = sensor_vector[sensor].index;

  ds = ds18x20_gpios[sensor_vector[index].pins_id];
  ds->reset();
  ds->select(sensor_vector[index].address);
  #ifdef W1_PARASITE_POWER
    // With parasite power held wire high at the end for parasitically powered devices
  ds->write(W1_READ_SCRATCHPAD, 1); // Read Scratchpad
  #else
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad
  #endif

  for (uint32_t i = 0; i < 9; i++) 
  {
    data[i] = ds->read();
  }

  if (OneWire::crc8(data, 8) == data[8]) 
  {
    switch(sensor_vector[index].address[0]) 
    {
      case DS18S20_CHIPID: {
        int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
        t = tkr_iSensors->ConvertTemp(tempS * 0.0625f - 0.250f);
        #ifdef W1_PARASITE_POWER
        sensor_vector[index].temperature = t;
        #endif
        return true;
      }
      case DS1822_CHIPID:
      case DS18B20_CHIPID: 
      {
        uint16_t temp12 = (data[1] << 8) + data[0];
        if (temp12 > 2047) {
          temp12 = (~temp12) +1;
          sign = -1;
        }
        t = tkr_iSensors->ConvertTemp(sign * temp12 * 0.0625f);  // Divide by 16
        #ifdef W1_PARASITE_POWER
        sensor_vector[index].temperature = t;
        #endif
        return true;
      }
      case MAX31850_CHIPID: 
      {
        int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
        t = tkr_iSensors->ConvertTemp(temp14 * 0.0625f);         // Divide by 16
        #ifdef W1_PARASITE_POWER
        sensor_vector[index].temperature = t;
        #endif
        return true;
      }
    }
  }
  ALOG_DBG( PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR " %d a%d"), index, sensor_vector[index].address[7]);
  return false;
}


void mDB18x20::EverySecond(void) 
{

  // Check for sensors if none was found
  if (module_state.devices==0) 
  {     
    Ds18x20Search();      // Check for changes in sensors number
    return; 
  }

  if (tkr_time->uptime_seconds_nonreset & 1) 
  {
    Ds18x20Convert();     // Start Conversion, takes up to one second
  } else {
    float t;
    for (uint32_t i = 0; i < module_state.devices; i++) {
      // 12mS per device
      if (Ds18x20Read(i, t)) {   // Read temperature
        sensor_vector[i].reading.ischanged = (t != sensor_vector[i].reading.val)?true:false;
        sensor_vector[i].reading.isvalid   = D_SENSOR_VALID_TIMEOUT_SECS;
        sensor_vector[i].reading.val = t;
        sensor_vector[i].utc_measured_timestamp = tkr_time->UtcTime();
        ALOG_DBG(PSTR(D_LOG_DSB "Read i%02d a%03d Alias%02d =-> %d"), i, sensor_vector[i].address[7], sensor_vector[i].device_name_index, (int)t);
      } else {
        if (sensor_vector[i].reading.isvalid) { sensor_vector[i].reading.isvalid--; }
        ALOG_ERR(PSTR(D_LOG_DB18 "Missed Reading %d"), sensor_vector[i].reading.isvalid, D_SENSOR_VALID_TIMEOUT_SECS - sensor_vector[i].reading.isvalid);
      }
    }
  }

}


void mDB18x20::SetDeviceNameID_WithAddress(const char* device_name, uint8_t device_name_index, uint8_t* array_val, uint8_t array_len)
{

  AddLog_Array(LOG_LEVEL_INFO, "Value", array_val, array_len);
  ALOG_INF(PSTR(D_LOG_DSB "Searching to find device with address"));

  int8_t search_id = 0;
  for (uint32_t i = 0; i < module_state.devices; i++) 
  {
    // AddLog_Array("Searching Saved", sensor_vector[i].address, 8);
    if(ArrayCompare(array_val,8, sensor_vector[i].address,8))
    {
      AddLog_Array(LOG_LEVEL_INFO, "MATCHED Searching Saved", sensor_vector[i].address, 8);
      sensor_vector[i].device_name_index = device_name_index;
      break;
    }

  }

}


void mDB18x20::Scan_ReportAsJsonBuilder()
{
  
  // Pre_Init();
  // Ds18x20Init();  
  bool flag_found_any = false;
  bool flag_started_object = false;

}


/******************************************************************************************************************
 * @SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mDB18x20::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    JBI->Add_P("NumDevices", module_state.devices);

    JBI->Array_Start("DeviceNameIndex");
    for (uint8_t i = 0; i < module_state.devices; i++){ JBI->Add(sensor_vector[i].device_name_index); }
    JBI->Array_End();

    JBI->Array_Start("AddressList");
    for (uint8_t i = 0; i < module_state.devices; i++) 
    {
      JBI->Array_Start();
      for (uint8_t a = 0; a < 8; a++){ JBI->Add(sensor_vector[i].address[a]); }
      JBI->Array_End();
    }
    JBI->Array_End();

  return JBI->End();

}


uint8_t mDB18x20::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  
  char buffer[40];
  int8_t alias_i = -1;
  bool flag_send_data = true;

  JBI->Start();

  for (uint8_t sensor_id = 0; sensor_id < module_state.devices; sensor_id++) 
  {
    flag_send_data = true; // default 
    if((json_level == JSON_LEVEL_IFCHANGED) && (!sensor_vector[sensor_id].reading.ischanged))
    {
      flag_send_data = false;
    }

    if(flag_send_data)
    {

      alias_i = sensor_vector[sensor_id].device_name_index;

      // To make sure when no alias is set, ie is -1, instead set as the loop index to get the generic name
      if(alias_i == -1) alias_i = sensor_id;
      // ALOG_INF(PSTR(D_LOG_DSB "Sensor %d Alias %d"), sensor_id, alias_i);
  
      JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), alias_i, buffer, sizeof(buffer)));         
        JBI->Add(PM_TEMPERATURE, sensor_vector[sensor_id].reading.val);
        JBI->Add(PM_ADDRESS,     sensor_vector[sensor_id].address[7]);

        if(json_level >= JSON_LEVEL_DETAILED)
        {
          JBI->Add(PM_VALID, sensor_vector[sensor_id].reading.isvalid);
          uint32_t age_ms = millis() - sensor_vector[sensor_id].utc_measured_timestamp;
          JBI->Add(PM_TIME_ELASPED, age_ms);
        }

        if(json_level >= JSON_LEVEL_DEBUG)
        {
          JBI->Add(PM_ID, sensor_id);
          JBI->Array_Start_P(PSTR("Device%d"), sensor_id);
            for(int a=0;a<8;a++){ JBI->Add(sensor_vector[sensor_id].address[a]); }
          JBI->Array_End();
        }
      JBI->Object_End(); 

    }

  }

  return JBI->End();

}


#ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
uint8_t mDB18x20::ConstructJSON_Debug(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  
  JBI->Object_Start("Addresses");
  for(int sens_i=0;sens_i<DS18X20_MAX_SENSORS;sens_i++)
  {
    JBI->Array_Start_P("%0d", sens_i);
    for (uint32_t i = 0; i < 8; i++) 
    {
      JBI->Add(sensor_vector[sens_i].address[i]);
    }
    JBI->Array_End();
  }
  JBI->Object_End();

  return JBI->End();

}
#endif // ENABLE_DEBUG_MQTT_CHANNEL_DB18X20


/******************************************************************************************************************
 * @SECTION: Commands
*******************************************************************************************************************/


void mDB18x20::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;


  /**
   * @brief New method iters through key/object pair that contains device friendly name/expected address
   *        A uint8_t saves that index, so looking up the name of the address will require the correct index (or left as -1 to unknown)
   * 
   */
  if(jtok = obj[PM_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_CTR])
  {

    ALOG_INF(PSTR("Setting Address")); delay (2000);

    JsonParserArray array_group = jtok; 
    ALOG_COM(PSTR(PM_SENSORADDRESS));

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
    
  }

}


/******************************************************************************************************************
 * @SECTION: MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mDB18x20::MQTTHandler_Init(){

  struct handler<mDB18x20>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDB18x20::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
  ptr = &mqtthandler_debug; // Each sensor should have its own debug channel for extra output only when needed
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE__DEBUG__ID;
  ptr->json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDB18x20::ConstructJSON_Debug;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_MQTT_CHANNEL_DB18X20

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif // sensor enabled


