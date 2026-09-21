#include "mESP32Temperature.h"

#ifdef USE_MODULE_SENSORS_ESP32_TEMPERATURE

int8_t mESP32Temperature::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION
   *******************/
  switch(function)
  {
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running)
  {
    return TASKER_RESULT__MODULE_DISABLED_ID;
  }

  switch(function)
  {
    /************
     * PERIODIC SECTION
     *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION
     *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return 1;

} // END function


void mESP32Temperature::Pre_Init(void)
{

  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  sensor.supported = IsSupported();

  if(sensor.supported)
  {
    module_state.devices = 1;
    module_state.mode = ModuleStatus::Running;

    AddLog(LOG_LEVEL_INFO, PSTR("ESP32Temp: Running"));
  }
  else
  {
    module_state.devices = 0;
    module_state.mode = ModuleStatus::Disabled;

    AddLog(LOG_LEVEL_INFO, PSTR("ESP32Temp: Not supported on this target"));
  }

}


void mESP32Temperature::Init(void)
{

  if(module_state.mode != ModuleStatus::Running)
  {
    return;
  }

  float value_c = NAN;

  if(ReadTemperature(&value_c))
  {
    sensor.reading.val = value_c;
    sensor.reading.isvalid = D_SENSOR_VALID_TIMEOUT_SECS__ESP32_TEMP;
    sensor.reading.ischanged = true;
    sensor.utc_measured_timestamp = tkr_time->UtcTime();
  }

  /***
   * Testing: some devices are basic, and single, and it should be possible to set their device name once/automatically
   */
  DLI->AddDeviceName("ChipIC",GetModuleUniqueID());

}


void mESP32Temperature::BootMessage(void)
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  ALOG_IMP(PSTR("ESP32Temp: %ssupported"),sensor.supported?"":"Un");
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


bool mESP32Temperature::IsSupported(void)
{

  #if defined(ESP32)

    /*
     * Classic ESP32 / ESP32-WROOM:
     *   Treat as unsupported unless explicitly overridden.
     *
     * ESP32-S2/S3/C3/C6/H2 etc:
     *   temperatureRead() is normally exposed by Arduino-ESP32.
     */

    #if defined(CONFIG_IDF_TARGET_ESP32)

      #ifdef ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32
      return true;
      #else
      return false;
      #endif

    #else

      return true;

    #endif

  #else

    return false;

  #endif

}


bool mESP32Temperature::ReadTemperature(float* value_c)
{

  if(value_c == nullptr)
  {
    return false;
  }

  *value_c = NAN;

  if(!sensor.supported)
  {
    return false;
  }

  #if defined(ESP32)

    #if defined(CONFIG_IDF_TARGET_ESP32) && !defined(ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32)

      return false;

    #else

      float t = temperatureRead();

      if(isnan(t))
      {
        return false;
      }

      /*
       * Optional crude sanity clamp.
       * Chip temperature can be high, but values far outside this range are likely wrong.
       */
      if((t < -40.0f) || (t > 150.0f))
      {
        return false;
      }

      *value_c = t;
      return true;

    #endif

  #else

    return false;

  #endif

}


void mESP32Temperature::EverySecond(void)
{

  float value_c = NAN;

  if(ReadTemperature(&value_c))
  {
    sensor.reading.ischanged = (value_c != sensor.reading.val) ? true : false;
    sensor.reading.val = value_c;
    sensor.reading.isvalid = D_SENSOR_VALID_TIMEOUT_SECS__ESP32_TEMP;
    sensor.utc_measured_timestamp = tkr_time->UtcTime();

    ALOG_DBG(PSTR("ESP32Temp: %d.%02dC"), (int)sensor.reading.val, abs((int)(sensor.reading.val * 100) % 100));
  }
  else
  {
    if(sensor.reading.isvalid)
    {
      sensor.reading.isvalid--;
    }

    sensor.reading.ischanged = false;
  }

}


/******************************************************************************************************************
 * @SECTION: ConstructJson
 *******************************************************************************************************************/


uint8_t mESP32Temperature::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add_P("Supported", sensor.supported);
    JBI->Add_P("NumDevices", module_state.devices);

    #if defined(CONFIG_IDF_TARGET_ESP32)
      JBI->Add_P("Target", "ESP32");
    #elif defined(CONFIG_IDF_TARGET_ESP32S2)
      JBI->Add_P("Target", "ESP32S2");
    #elif defined(CONFIG_IDF_TARGET_ESP32S3)
      JBI->Add_P("Target", "ESP32S3");
    #elif defined(CONFIG_IDF_TARGET_ESP32C3)
      JBI->Add_P("Target", "ESP32C3");
    #elif defined(CONFIG_IDF_TARGET_ESP32C6)
      JBI->Add_P("Target", "ESP32C6");
    #elif defined(CONFIG_IDF_TARGET_ESP32H2)
      JBI->Add_P("Target", "ESP32H2");
    #else
      JBI->Add_P("Target", "Unknown");
    #endif

  return JBI->End();

}


uint8_t mESP32Temperature::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Object_Start("ESP32");

      JBI->Add_P("ChipTempC", sensor.reading.val);
      JBI->Add_P(PM_VALID, sensor.reading.isvalid);

      if(json_level >= JSON_LEVEL_DETAILED)
      {
        JBI->Add_P("Supported", sensor.supported);
        JBI->Add_P("Utc", sensor.utc_measured_timestamp);
      }

      if(json_level >= JSON_LEVEL_DEBUG)
      {
        JBI->Add_P(PM_ID, 0);
        JBI->Add_P("Changed", sensor.reading.ischanged);
      }

    JBI->Object_End();

  return JBI->End();

}


/******************************************************************************************************************
 * @SECTION: Commands
 *******************************************************************************************************************/


void mESP32Temperature::parse_JSONCommand(JsonParserObject obj)
{
    
}


/******************************************************************************************************************
 * @SECTION: MQTT
 *******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mESP32Temperature::Telemetry_Init(void)
{

  struct telemetry_handler<mESP32Temperature>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mESP32Temperature::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mESP32Temperature::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_SENSORS_ESP32_TEMPERATURE