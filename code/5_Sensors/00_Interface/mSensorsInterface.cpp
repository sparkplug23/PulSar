#include "mSensorsInterface.h" 

#ifdef USE_MODULE_SENSORS_INTERFACE 

int8_t mSensorsInterface::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
      #ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE
      Calib_Init();
      #endif
    break;
    case TASK_INIT:
      Init();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
      
      #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
        Update_UnifiedFilteredReadings();
      #endif
      #ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE
      Calib_EveryLoop();
      #endif
    break;  
    case TASK_EVERY_SECOND:{

      // Make nicer later with json command to enable and time period to show
      if(rt.tTicker_Splash_Sensors_To_Logs-- == 1)
      {
        // Measurement level feedback will be "DebugMore" and show level should be "Debug". "Info" should be reserved for essential stuff not in mqtt
        ALOG_DBM(PSTR(">>> Sensor Readings <<<"));
        tkr->Tasker_Interface(TASK_SENSOR_SHOW_LATEST_LOGGED_ID);
        rt.tTicker_Splash_Sensors_To_Logs = 120 ; // reset
      }
      
  
      #ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE
      Calib_OnSecond();
      #endif


      // for(auto& pmod:tkr->pModule)
      // {
      //   for(int sensor_id=0;sensor_id<pmod->GetSensorCount();sensor_id++)
      //   {
      //     sensors_reading_t val;
      //     pmod->GetSensorReading(&val, sensor_id);
      //     if(val.type[0])
      //     {
      //       ALOG_TST(PSTR("%S %d|%d val.data[%d]=%d"),pmod->GetModuleFriendlyName(), sensor_id, pmod->GetSensorCount(), sensor_id, (int)val.GetValue(SENSOR_TYPE_TEMPERATURE_ID));
      //     }
      //   }
      // }
     
      //   pModule[switch_index]->Tasker(function, obj);

    }break;
    case TASK_WEB_APPEND_SENSOR_TABLE_VALUES:
      WebAppend__Sensor_Table__As_Ragged();
      // WebAppend__Sensor_Table__As_SensorsRows_Inverted();
      // WebAppend__Sensor_Table__As_TypesRows();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_EVENT_MOTION_STARTED_ID:
    case TASK_EVENT_MOTION_ENDED_ID:
      Broadcast_Event_MotionDetected();
    break; 
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      Broadcast_Event_UserInput();
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      // tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function



/*********************************************************************************************\
 * mWebServer::WebAppend__Sensor_Table__As_TypesRows
 *
 * SUMMARY:
 *   Type = row, Sensor = column (wide table, your original “union” view)
 *
 * ARGUMENTS:
 *   out (Print&) : response stream
 *
 * RETURNS:
 *   void
 *
 * CHANGED:
 *   25Jan26, Initial version (no helpers, member function)
\*********************************************************************************************/
void mSensorsInterface::WebAppend__Sensor_Table__As_TypesRows()
{
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  Print* out = tkr_web->WebControls_GetPrint();
  if (!out) return;

  // ---- Discover sensors (columns) ----
  const uint8_t MAX_SENSORS2 = 16;

  struct SensorCol {
    uint16_t mod_uid;
    uint8_t  sensor_idx;     // index for GetSensorReading()
    char     name[48];
  };

  SensorCol sensors[MAX_SENSORS2];
  uint8_t sensor_count = 0;
  char namebuf[64];

  for (auto& pmod : tkr->pModule)
  {
    if (!IS_MODULE_SENSOR_SUBMODULE(pmod->GetModuleUniqueID())) continue;

    const uint8_t cnt = pmod->GetSensorCount();
    if (!cnt) continue;

    for (uint8_t sid = 0; sid < cnt; sid++)
    {
      if (sensor_count >= MAX_SENSORS2) break;

      sensors_reading_t val;
      pmod->GetSensorReading(&val, sid);
      if (!val.Valid()) continue;

      // NOTE: matches your naming convention (val.sensor_id)
      DLI->GetDeviceName_WithModuleUniqueID(
        pmod->GetModuleUniqueID(),
        val.sensor_id,
        namebuf,
        sizeof(namebuf)
      );

      sensors[sensor_count].mod_uid    = pmod->GetModuleUniqueID();
      sensors[sensor_count].sensor_idx = sid;

      strncpy(sensors[sensor_count].name, namebuf, sizeof(sensors[sensor_count].name) - 1);
      sensors[sensor_count].name[sizeof(sensors[sensor_count].name) - 1] = '\0';

      sensor_count++;
    }

    if (sensor_count >= MAX_SENSORS2) break;
  }

  // ---- Table start ----
  out->print(F("<table class=\"kv kvwide\">"));

  // Header
  out->print(F("<tr><th>Type</th>"));
  for (uint8_t c = 0; c < sensor_count; c++)
  {
    out->printf_P(PSTR("<th>%s</th>"), sensors[c].name);
  }
  out->print(F("</tr>"));

  // Rows by type (skip types with no values anywhere)
  for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  {
    uint16_t type_id_adjusted = type_id;
    if (type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID) {
      type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;
    }

    bool any = false;

    for (uint8_t c = 0; c < sensor_count && !any; c++)
    {
      // find module
      mTaskerInterface* pmod_found = nullptr;
      for (auto& pmod : tkr->pModule) {
        if (pmod->GetModuleUniqueID() == sensors[c].mod_uid) { pmod_found = pmod; break; }
      }
      if (!pmod_found) continue;

      sensors_reading_t val;
      pmod_found->GetSensorReading(&val, sensors[c].sensor_idx);

      if (val.Valid() && val.isFloatWaiting_WithSensorType(type_id_adjusted)) any = true;
    }

    if (!any) continue;

    out->print(F("<tr><td class=\"key\">"));
    out->print(GetUnifiedSensor_NameByTypeID(type_id));
    out->print(F("</td>"));

    for (uint8_t c = 0; c < sensor_count; c++)
    {
      bool has = false;
      float v = 0;

      // find module
      mTaskerInterface* pmod_found = nullptr;
      for (auto& pmod : tkr->pModule) {
        if (pmod->GetModuleUniqueID() == sensors[c].mod_uid) { pmod_found = pmod; break; }
      }

      if (pmod_found)
      {
        sensors_reading_t val;
        pmod_found->GetSensorReading(&val, sensors[c].sensor_idx);

        if (val.Valid() && val.isFloatWaiting_WithSensorType(type_id_adjusted))
        {
          has = true;
          v = val.GetFloat(type_id_adjusted);
        }
      }

      out->print(F("<td class=\"val\">"));
      if (has)
      {
        switch (type_id_adjusted)
        {
          case SENSOR_TYPE_TEMPERATURE_ID:        out->printf_P(PSTR("%.1f &deg;C"), v); break;
          case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:  out->printf_P(PSTR("%.1f %%"), v);      break;
          case SENSOR_TYPE_PRESSURE_ID:           out->printf_P(PSTR("%.0f Pa"), v);      break;
          case SENSOR_TYPE_ALTITUDE_ID:           out->printf_P(PSTR("%.1f m"), v);       break;
          case SENSOR_TYPE_GAS_RESISTANCE_ID:     out->printf_P(PSTR("%.0f &Omega;"), v); break;
          default:                                out->printf_P(PSTR("%.3f"), v);        break;
        }
      }
      else
      {
        out->print(F("<span class=\"subtle\">—</span>"));
      }
      out->print(F("</td>"));
    }

    out->print(F("</tr>"));
  }

  out->print(F("</table>"));
  #endif // USE_MODULE_NETWORK_WEBSERVER
}




/*********************************************************************************************\
 * mWebServer::WebAppend__Sensor_Table__As_SensorsRows_Inverted
 *
 * SUMMARY:
 *   Sensor = row, Type = column (short headers)
 *
 * ARGUMENTS:
 *   out (Print&) : response stream
 *
 * RETURNS:
 *   void
 *
 * CHANGED:
 *   25Jan26, Initial version (no helpers, member function)
\*********************************************************************************************/
void mSensorsInterface::WebAppend__Sensor_Table__As_SensorsRows_Inverted()
{
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  Print* out = tkr_web->WebControls_GetPrint();
  if (!out) return;

  // ----------------------------
  // Compile-time switch
  // ----------------------------
  // Put this define in a central header later. Keeping here for testing.
  #ifndef ENABLE_WEB_SENSOR_TABLE_INVERTED
  #define ENABLE_WEB_SENSOR_TABLE_INVERTED 1
  #endif

  // ----------------------------
  // Small helpers
  // ----------------------------
  auto tbl_start = [&](){ out->print(F("<table class=\"kv kvwide\">")); };
  auto tbl_end   = [&](){ out->print(F("</table>")); };

  auto th = [&](const char* s){ out->printf_P(PSTR("<th>%s</th>"), s); };

  auto print_val_with_unit = [&](uint16_t type_id, float v){
    switch (type_id)
    {
      case SENSOR_TYPE_TEMPERATURE_ID:        out->printf_P(PSTR("%.1f &deg;C"), v); break;
      case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:  out->printf_P(PSTR("%.1f %%"), v);      break;
      case SENSOR_TYPE_PRESSURE_ID:           out->printf_P(PSTR("%.0f Pa"), v);      break;
      case SENSOR_TYPE_ALTITUDE_ID:           out->printf_P(PSTR("%.1f m"), v);       break;
      case SENSOR_TYPE_GAS_RESISTANCE_ID:     out->printf_P(PSTR("%.0f &Omega;"), v); break;
      default:                                out->printf_P(PSTR("%.3f"), v);        break;
    }
  };

  auto type_short = [&](uint16_t type_id) -> const __FlashStringHelper* {
    switch (type_id)
    {
      case SENSOR_TYPE_TEMPERATURE_ID:        return F("Temp");
      case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:  return F("Hum");
      case SENSOR_TYPE_PRESSURE_ID:           return F("Press");
      case SENSOR_TYPE_ALTITUDE_ID:           return F("Alt");
      case SENSOR_TYPE_GAS_RESISTANCE_ID:     return F("Gas");
      case SENSOR_TYPE_LIGHT_LEVEL_ID:        return F("Light");
      case SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID:    return F("Lux");
      case SENSOR_TYPE_SUN_AZIMUTH_ID:        return F("SunAz");
      case SENSOR_TYPE_SUN_ELEVATION_ID:      return F("SunEl");
      // If you want this visible in inverted mode, keep it; otherwise omit it from the type list below.
      case SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID: return F("HeatRGB");
      default: return F(" ");
    }
  };

  // ----------------------------
  // Discover sensors once (union)
  // ----------------------------
  static const uint8_t MAX_SENSORS2 = 16;

  struct SensorCol {
    uint16_t mod_uid;
    uint8_t  sensor_id;   // index for GetSensorReading()
    char     name[48];
  };

  SensorCol sensors[MAX_SENSORS2];
  uint8_t sensor_count = 0;

  char namebuf[64];

  for (auto& pmod : tkr->pModule)
  {
    if (!IS_MODULE_SENSOR_SUBMODULE(pmod->GetModuleUniqueID())) continue;

    uint8_t cnt = pmod->GetSensorCount();
    if (!cnt) continue;

    for (uint8_t sid = 0; sid < cnt; sid++)
    {
      if (sensor_count >= MAX_SENSORS2) break;

      sensors_reading_t val;
      pmod->GetSensorReading(&val, sid);
      if (!val.Valid()) continue;

      // Use val.sensor_id for naming like you do elsewhere
      DLI->GetDeviceName_WithModuleUniqueID(pmod->GetModuleUniqueID(), val.sensor_id, namebuf, sizeof(namebuf));

      sensors[sensor_count].mod_uid   = pmod->GetModuleUniqueID();
      sensors[sensor_count].sensor_id = sid;

      strncpy(sensors[sensor_count].name, namebuf, sizeof(sensors[sensor_count].name) - 1);
      sensors[sensor_count].name[sizeof(sensors[sensor_count].name) - 1] = '\0';

      sensor_count++;
    }
    if (sensor_count >= MAX_SENSORS2) break;
  }

  // ----------------------------
  // Type list for inverted mode
  // (keep it short; columns stay stable)
  // ----------------------------
  // #if ENABLE_WEB_SENSOR_TABLE_INVERTED

  static const uint16_t kTypeList[] = {
    SENSOR_TYPE_TEMPERATURE_ID,
    SENSOR_TYPE_RELATIVE_HUMIDITY_ID,
    SENSOR_TYPE_PRESSURE_ID,
    SENSOR_TYPE_GAS_RESISTANCE_ID,
    SENSOR_TYPE_ALTITUDE_ID,
    SENSOR_TYPE_LIGHT_LEVEL_ID,
    SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID,
    SENSOR_TYPE_SUN_AZIMUTH_ID,
    SENSOR_TYPE_SUN_ELEVATION_ID
    // If you want the heatmap RGB string, you'd need string support in the table.
    // SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID,
  };
  static const uint8_t kTypeCount = sizeof(kTypeList) / sizeof(kTypeList[0]);

  tbl_start();

  // Header: "Sensor" + type short names
  out->print(F("<tr><th>Sensor</th>"));
  for (uint8_t i = 0; i < kTypeCount; i++) {
    out->print(F("<th>"));
    out->print(type_short(kTypeList[i]));
    out->print(F("</th>"));
  }
  out->print(F("</tr>"));

  // Rows: each sensor
  for (uint8_t s = 0; s < sensor_count; s++)
  {
    out->print(F("<tr><td class=\"key\">"));
    out->print(sensors[s].name);
    out->print(F("</td>"));

    // Find module pointer once per sensor row (avoids re-scanning for every cell)
    mTaskerInterface* pmod_found = nullptr;
    for (auto& pmod : tkr->pModule) {
      if (pmod->GetModuleUniqueID() == sensors[s].mod_uid) { pmod_found = pmod; break; }
    }

    for (uint8_t i = 0; i < kTypeCount; i++)
    {
      const uint16_t type_id = kTypeList[i];

      bool has = false;
      float v  = 0;

      if (pmod_found)
      {
        sensors_reading_t val;
        pmod_found->GetSensorReading(&val, sensors[s].sensor_id);

        // special case adjustment (match your JSON logic)
        uint16_t type_id_adjusted = type_id;
        if (type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID) {
          type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;
        }

        if (val.Valid() && val.isFloatWaiting_WithSensorType(type_id_adjusted))
        {
          has = true;
          v = val.GetFloat(type_id_adjusted);
        }
      }

      out->print(F("<td class=\"val\">"));
      if (has) print_val_with_unit(type_id, v);
      else out->print(F("<span class=\"subtle\">—</span>"));
      out->print(F("</td>"));
    }

    out->print(F("</tr>"));
  }

  tbl_end();
  #endif // USE_MODULE_NETWORK_WEBSERVER
}



/*********************************************************************************************\
 * mWebServer::WebAppend__Sensor_Table__As_Ragged
 *
 * SUMMARY:
 *   Sensor = row, only prints values the sensor actually reports.
 *   No type headers, no dashes; row width naturally matches max reported items.
 *
 * ARGUMENTS:
 *   out (Print&) : response stream
 *
 * RETURNS:
 *   void
 *
 * CHANGED:
 *   25Jan26, Initial version (no helpers, member function)
\*********************************************************************************************/
void mSensorsInterface::WebAppend__Sensor_Table__As_Ragged()
{
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  Print* out = tkr_web->WebControls_GetPrint();
  if (!out) return;

  // ---- Discover sensors (rows) ----
  const uint8_t MAX_SENSORS2 = 16;

  struct SensorRow {
    uint16_t mod_uid;
    uint8_t  sensor_idx;
    char     name[48];
  };

  SensorRow sensors[MAX_SENSORS2];
  uint8_t sensor_count = 0;
  char namebuf[64];

  for (auto& pmod : tkr->pModule)
  {
    if (!IS_MODULE_SENSOR_SUBMODULE(pmod->GetModuleUniqueID())) continue;

    const uint8_t cnt = pmod->GetSensorCount();
    if (!cnt) continue;

    for (uint8_t sid = 0; sid < cnt; sid++)
    {
      if (sensor_count >= MAX_SENSORS2) break;

      sensors_reading_t val;
      pmod->GetSensorReading(&val, sid);
      if (!val.Valid()) continue;

      DLI->GetDeviceName_WithModuleUniqueID(
        pmod->GetModuleUniqueID(),
        val.sensor_id,
        namebuf,
        sizeof(namebuf)
      );

      sensors[sensor_count].mod_uid    = pmod->GetModuleUniqueID();
      sensors[sensor_count].sensor_idx = sid;

      strncpy(sensors[sensor_count].name, namebuf, sizeof(sensors[sensor_count].name) - 1);
      sensors[sensor_count].name[sizeof(sensors[sensor_count].name) - 1] = '\0';

      sensor_count++;
    }

    if (sensor_count >= MAX_SENSORS2) break;
  }

  // ---- Probe list (ordering defines the “sentence” order) ----
  const uint16_t type_list[] = {
    SENSOR_TYPE_TEMPERATURE_ID,
    SENSOR_TYPE_RELATIVE_HUMIDITY_ID,
    SENSOR_TYPE_PRESSURE_ID,
    SENSOR_TYPE_GAS_RESISTANCE_ID,
    SENSOR_TYPE_ALTITUDE_ID,
    SENSOR_TYPE_LIGHT_LEVEL_ID,
    SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID,
    SENSOR_TYPE_SUN_AZIMUTH_ID,
    SENSOR_TYPE_SUN_ELEVATION_ID
  };
  const uint8_t type_count = sizeof(type_list) / sizeof(type_list[0]);

  // ---- Table start ----
  out->print(F("<table class=\"kv kvwide\">"));

  // minimal header for consistent styling; remove if you truly want no header row
  // out->print(F("<tr><th>Sensor</th><th></th></tr>"));

  // Rows: each sensor
  for (uint8_t s = 0; s < sensor_count; s++)
  {
    // find module once per row
    mTaskerInterface* pmod_found = nullptr;
    for (auto& pmod : tkr->pModule) {
      if (pmod->GetModuleUniqueID() == sensors[s].mod_uid) { pmod_found = pmod; break; }
    }

    sensors_reading_t val;
    bool row_valid = false;
    if (pmod_found)
    {
      pmod_found->GetSensorReading(&val, sensors[s].sensor_idx);
      row_valid = val.Valid();
    }

    out->print(F("<tr>"));

    // Sensor name
    out->print(F("<td class=\"key\">"));
    out->print(sensors[s].name);
    out->print(F("</td>"));

    if (row_valid)
    {
      for (uint8_t i = 0; i < type_count; i++)
      {
        const uint16_t type_id = type_list[i];
        uint16_t type_id_adjusted = type_id;

        if (type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID) {
          type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;
        }

        if (val.isFloatWaiting_WithSensorType(type_id_adjusted))
        {
          const float v = val.GetFloat(type_id_adjusted);

          out->print(F("<td class=\"val\">"));
          switch (type_id_adjusted)
          {
            case SENSOR_TYPE_TEMPERATURE_ID:        out->printf_P(PSTR("%.1f &deg;C"), v); break;
            case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:  out->printf_P(PSTR("%.1f %%"), v);      break;
            case SENSOR_TYPE_PRESSURE_ID:           out->printf_P(PSTR("%.0f Pa"), v);      break;
            case SENSOR_TYPE_ALTITUDE_ID:           out->printf_P(PSTR("%.1f m Alt"), v);       break;
            case SENSOR_TYPE_GAS_RESISTANCE_ID:     out->printf_P(PSTR("%.0f &Omega; Gas"), v); break;
            case SENSOR_TYPE_SUN_AZIMUTH_ID:        out->printf_P(PSTR("%.1f &deg; Az"), v);    break;
            case SENSOR_TYPE_SUN_ELEVATION_ID:      out->printf_P(PSTR("%.1f &deg; El"), v);    break;
            default:                                out->printf_P(PSTR("%.3f"), v);        break;
          }
          out->print(F("</td>"));
        }
      }
    }

    out->print(F("</tr>"));
  }

  out->print(F("</table>"));
  #endif // USE_MODULE_NETWORK_WEBSERVER
}



void mSensorsInterface::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mSensorsInterface::Init(void)
{
  rt.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  module_state.mode = ModuleStatus::Running;


  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_OILTANK_ADD
  // AddFilteredSensor(5027, 0, 60, 120);
  // TEMP: Filter for temperature sensor in module 5027, sensor index 0

  AddFilteredSensor(tkr_tof_vl1x->GetModuleUniqueID(), 0, 60, 60, SENSOR_TYPE_DISTANCE_ID, "OilReading1min");
  AddFilteredSensor(tkr_tof_vl1x->GetModuleUniqueID(), 0, 3600, 360, SENSOR_TYPE_DISTANCE_ID, "OilReading1Hr");
  

  // filtername, which allows for OH decoding.
  #endif

  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_HARDCODED_ADD
  // AddFilteredSensor(5027, 0, 60, 120);
  // TEMP: Filter for temperature sensor in module 5027, sensor index 0


  AddFilteredSensor(tkr_db18->GetModuleUniqueID(), 0, 10, 60, SENSOR_TYPE_TEMPERATURE_ID, "DB10Sec");
  AddFilteredSensor(tkr_db18->GetModuleUniqueID(), 0, 60, 60, SENSOR_TYPE_TEMPERATURE_ID, "DB1Min");
  AddFilteredSensor(tkr_bme->GetModuleUniqueID(), 0, 10, 60, SENSOR_TYPE_TEMPERATURE_ID, "BME10Sec");
  #endif

  
  DLI->AddDeviceName("SensorInterface",GetModuleUniqueID());



}


void mSensorsInterface::EveryLoop()
{

}


#ifdef USE_MODULE_LIGHTS_INTERFACE
/**
 * @brief Single function that applies colour map to any temperature sensors.
 * In InterfaceSensors, the type of applied map can be selected to be added to unified method directly
 * 
 * @param temperature 
 * @return RgbwwColor 
 */
RgbwwColor mSensorsInterface::GetColourValueUsingMaps_ForUnifiedSensor(float temperature) // flag_unified_sensor_colour_heatmap_type
{

  if(flag_unified_sensor_colour_heatmap_type==1) //adjusted
  {
    return tkr_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);
  }
  else //full colour-
  {
    return tkr_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);
  }

}
#endif // USE_MODULE_LIGHTS_INTERFACE


void mSensorsInterface::Broadcast_Event_MotionDetected()
{

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_motion_event_ifchanged.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);
  #endif // USE_MODULE_NETWORK_MQTT

}

void mSensorsInterface::Broadcast_Event_UserInput()
{
  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_event_input.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);
  #endif // USE_MODULE_NETWORK_MQTT
}


// Date Modified: 03 Oct 2025
#ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE

void mSensorsInterface::Calib_Init() {
  calib_cfg.enabled = true;                // opt-in
  calib_cfg.min_sample_period_ms = 1000;    // default 1 Hz collection
  calib_cfg.dump_period_ms = 60000;         // flush every minute
  calib_cfg.t_last_sample_ms = 0;
  calib_cfg.t_last_dump_ms = 0;
  calib_buffer.clear();
}

void mSensorsInterface::Calib_SetCaptureName(const char* s) {
  if (calib_cfg.capture_name) { free(calib_cfg.capture_name); calib_cfg.capture_name = nullptr; calib_cfg.capture_name_len = 0; }
  if (!s) return;
  size_t n = strnlen(s, 63);
  calib_cfg.capture_name = (char*)malloc(n+1);
  if (calib_cfg.capture_name) {
    memcpy(calib_cfg.capture_name, s, n); calib_cfg.capture_name[n] = '\0';
    calib_cfg.capture_name_len = (uint8_t)n;
  }
}

void mSensorsInterface::Calib_ClearBuffer() {
  calib_buffer.clear();
}

void mSensorsInterface::Calib_EveryLoop() {
  if (!calib_cfg.enabled) return;
  uint32_t now = millis();
  if (now - calib_cfg.t_last_sample_ms >= calib_cfg.min_sample_period_ms) {
    calib_cfg.t_last_sample_ms = now;
    Calib_AppendCurrentReadings();
  }
  if (now - calib_cfg.t_last_dump_ms >= calib_cfg.dump_period_ms) {
    calib_cfg.t_last_dump_ms = now;
    Calib_FlushToFile();
  }
}

void mSensorsInterface::Calib_OnSecond() {
  // reserved if later you want a per-second “truth” injector; currently not needed
}

/**
 * Traverse all sensor modules (same pattern as ConstructJSON_Sensor),
 * collect float readings with valid timestamp, push into calib_buffer.
 */// Date Modified: 04 Oct 2025
void mSensorsInterface::Calib_AppendCurrentReadings() {
#ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE
  if (!tkr_time->RtcTime.valid) {
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
    ALOG_INF(PSTR("Calib: skip — RTC invalid"));
#endif
    return;
  }

  const uint32_t utc_now = tkr_time->UtcTime();

  uint32_t modules = 0, modules_used = 0;
  uint32_t skipped_nosensors = 0, skipped_invalid = 0, skipped_filter = 0, skipped_stale = 0;
  uint32_t pushed = 0;

  char name_buf[100] = {0};

  for (auto& pmod : tkr->pModule) {
    ++modules;
    const uint32_t mid = pmod->GetModuleUniqueID();
    const bool is_sensor = IS_MODULE_SENSOR_SUBMODULE(mid);
    if (!is_sensor) {
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
      ALOG_DBM(PSTR("Calib: module 0x%08X not a sensor-submodule, skipping"), mid);
#endif
      continue;
    }

    ++modules_used;

    const uint8_t sensors_available = pmod->GetSensorCount();
    if (!sensors_available) {
      ++skipped_nosensors;
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
      ALOG_INF(PSTR("Calib: module 0x%08X has no sensors"), mid);
#endif
      continue;
    }

#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
    ALOG_INF(PSTR("Calib: module 0x%08X sensors=%u"), mid, sensors_available);
#endif

    for (int sensor_id = 0; sensor_id < sensors_available; ++sensor_id) {
      sensors_reading_t val{};
      pmod->GetSensorReading(&val, sensor_id);
      if (!val.Valid()) {
        ++skipped_invalid;
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
        ALOG_INF(PSTR("Calib: mid=0x%08X sid=%d invalid reading"), mid, sensor_id);
#endif
        continue;
      }

      // Resolve unified name once for this reading
      DLI->GetDeviceName_WithModuleUniqueID(mid, val.sensor_id, name_buf, sizeof(name_buf));

      // --- Filter (NameList or Name) — decide once per reading
      bool keep = true;
      if (calib_cfg.use_name_list) {
        keep = false;
        for (auto &s : calib_cfg.capture_names) { if (s.equals(name_buf)) { keep = true; break; } }
      } else if (calib_cfg.capture_name && calib_cfg.capture_name_len) {
        keep = (strncmp(name_buf, calib_cfg.capture_name, calib_cfg.capture_name_len) == 0);
      }
      if (!keep) {
        ++skipped_filter;
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
        ALOG_INF(PSTR("Calib: filter drop name=\"%s\""), name_buf);
#endif
        continue;
      }

      // --- Staleness check
      const uint32_t stale_s = tkr_set->Settings.unified_interface_reporting_invalid_reading_timeout_seconds;
      if (stale_s != 0 && val.timestamp) {
        const uint32_t age = utc_now - val.timestamp;
        if (age > stale_s) {
          ++skipped_stale;
#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
          ALOG_INF(PSTR("Calib: stale drop name=\"%s\" age=%u > %u s"), name_buf, age, stale_s);
#endif
          continue;
        }
      }

#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
      ALOG_INF(PSTR("Calib: keep name=\"%s\" types=%u floats=%u ts=%u"),
               name_buf, (unsigned)val.sensor_type.size(), (unsigned)val.data_f.size(), val.timestamp);
#endif

      // --- Push floats
      const size_t n = min(val.sensor_type.size(), val.data_f.size());
      for (size_t i = 0; i < n; ++i) {
        const uint16_t type_id = val.sensor_type[i];
        const float fv = val.data_f[i];

        calib_point_t cp;
        cp.name    = String(name_buf);
        cp.type_id = type_id;
        cp.value   = fv;
        cp.utc     = utc_now;

        calib_buffer.push_back(std::move(cp));
        ++pushed;

#ifdef ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE
        ALOG_INF(PSTR("Calib: push name=\"%s\" type_id=%u v=%0.6f t=%u"),
                 name_buf, type_id, fv, utc_now);
#endif
      }
    }
  }

  ALOG_INF(PSTR("Calib: modules=%u used=%u pushed=%u skip{nosensor=%u invalid=%u filter=%u stale=%u}"),
           modules, modules_used, pushed, skipped_nosensors, skipped_invalid, skipped_filter, skipped_stale);
#endif // feature
}


/**
 * Append buffered points as NDJSON lines to a single file.
 * Each line: {"t":<utc>,"name":"<dev>","type_id":N,"v":<float>}
 * Safe on power loss; MATLAB can read line-by-line easily.
 */
void mSensorsInterface::Calib_FlushToFile() {
  ALOG_INF(PSTR("Calib: Flush %u points"), (unsigned)calib_buffer.size());
  if (calib_buffer.empty()) return;

#if defined(USE_MODULE_CORE_FILESYSTEM)
  // Ensure FS mounted (assume already by your platform init)
  File f = FILE_SYSTEM.open(CALIB_CAPTURE_FILENAME, FILE_APPEND);
  if (!f) {
    // Try create
    f = FILE_SYSTEM.open(CALIB_CAPTURE_FILENAME, FILE_WRITE);
  }
  if (!f) {
    ALOG_ERR(PSTR("Calib: open fail \"%s\""), CALIB_CAPTURE_FILENAME);
    return;
  }

  // Minimal JSON per line
  for (const auto& cp : calib_buffer) {
    // NOTE: If you later want human-readable type, map type_id→name here.
    // Keeping compact for MATLAB regression.
    // {"t":1696355401,"name":"DB_04","type_id":1,"v":28.937}
    f.print('{');
      f.print(F("\"t\":"));       f.print(cp.utc); f.print(',');
      f.print(F("\"n\":\""));  f.print(cp.name); f.print(F("\","));
      f.print(F("\"i\":")); f.print(cp.type_id); f.print(F(","));
      f.print(F("\"v\":"));       f.print(cp.value, 2);
    f.println('}');
  }
  f.flush();
  f.close();
#else
  // No FS available on this build; you can redirect to MQTT or serial if needed.
  for (const auto& cp : calib_buffer) {
    ALOG_INF(PSTR("Calib NDJSON: {\"t\":%u,\"name\":\"%s\",\"type_id\":%u,\"v\":%f}"),
             cp.utc, cp.name.c_str(), cp.type_id, cp.value);
  }
#endif

  calib_buffer.clear();
}
#endif // ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mSensorsInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["MQTT"].getObject()["ScanSensors"])
	{
		JBI->Start();

		tkr->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		bool ready_to_send = JBI->End();

		if(!ready_to_send)
		{
			// Nothing was found, create new message
			JBI->Start();
				JBI->Add("SensorScan", "No Devices Found");
			ready_to_send = JBI->End();
		}


		if(ready_to_send)
		{			
    	ALOG_TST(PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
			tkr_mqtt->brokers[0]->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		}

	}

  // Date Modified: 03 Oct 2025
  #ifdef ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE
  if (JsonParserObject calib = obj["Calib"].getObject()) {
    if (auto j = calib["Enable"]) { calib_cfg.enabled = (bool)j.getInt(); }
    if (auto j = calib["Name"])   { Calib_SetCaptureName(j.getStr()); }
    if (auto j = calib["SampleMs"]) { calib_cfg.min_sample_period_ms = (uint32_t)j.getInt(); }
    if (auto j = calib["DumpSecs"]) { calib_cfg.dump_period_ms = (uint32_t)j.getInt() * 1000UL; }
    if (auto j = calib["Flush"])    { if ((bool)j.getInt()) Calib_FlushToFile(); }
    if (auto j = calib["ClearBuf"]) { if ((bool)j.getInt()) Calib_ClearBuffer(); }
    // Optional: ClearFile
    if (auto j = calib["ClearFile"]) {
      if ((bool)j.getInt()) {
        #if defined(CALIB_FS)
        FILE_SYSTEM.remove(CALIB_CAPTURE_FILENAME);
        #endif
      }
    }
    ALOG_INF(PSTR("Calib: en=%d name=%s sample=%ums dump=%ums"),
            calib_cfg.enabled,
            calib_cfg.capture_name ? calib_cfg.capture_name : "(all)",
            calib_cfg.min_sample_period_ms,
            calib_cfg.dump_period_ms);
            if (auto arr = calib["NameList"].getArray()) {
    calib_cfg.capture_names.clear();
    for (uint16_t i = 0; i < arr.size(); ++i) {
      const char* s = arr[i].getStr();
      if (s && *s) calib_cfg.capture_names.emplace_back(s);
    }
    calib_cfg.use_name_list = !calib_cfg.capture_names.empty();
  }
  }
#endif

    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}

/**
 * @brief 
 * 
 * Format with multiple locations? maybe should be flipped and the name added. ie
 * 
 * {"Temperature":{"Master Bedroom":23.580,"Landing":25.020}}
 * {"Humidity":{name:12,other:13} //therefore "for each key"
 * {"lum"..}
 * 
 * Step 1: Using module pointer, request all sensor readings which will be returned in sensors_reading_t
 * Step 2: Go through all sensors_reading_t values, if configured, then append their values to the JSON array with Key based on the module pointer
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  // if(tkr_time->uptime_seconds_nonreset > 30)
    // ALOG_INF(PSTR("ConstructJSON_Sensor()"));

  JBI->Start();

    // JBI->Add("Redunction", mqtthandler_sensor_ifchanged.flags.FrequencyRedunctionLevel);
  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  uint16_t type_id_adjusted = 0;

  for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:tkr->pModule)
    {
      if( (pmod->GetModuleUniqueID() >= 5000 && pmod->GetModuleUniqueID() <= 5999) )//IS_MODULE_SENSOR_SUBMODULE(  ) )
      {
        //Get any sensors in module
        uint8_t sensors_available = pmod->GetSensorCount();
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleName());

        uint16_t unified_sensor_reporting_invalid_reading_timeout_seconds = tkr_set->Settings.unified_interface_reporting_invalid_reading_timeout_seconds;
        // ALOG_WRN(PSTR("reading_timeout_seconds %d"), unified_sensor_reporting_invalid_reading_timeout_seconds);
                  
        if(sensors_available) 
        {
          // ALOG_INF( PSTR("GetSensorCount\t\t =%d\t%s"), sensors_available, pmod->GetModuleName());
          for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
          {
            sensors_reading_t val;
            pmod->GetSensorReading(&val, sensor_id);

            if(unified_sensor_reporting_invalid_reading_timeout_seconds != 0) // Check active
            {
                              
              if(val.timestamp)
              {
                if(tkr_time->RtcTime.valid) // Only enable timestamp checks when time is valid
                {
                  uint32_t sensor_elapsed_time = tkr_time->UtcTime() - val.timestamp;
                  if(sensor_elapsed_time) // If positive and NOT val.timestamp set to 0 as skipped
                  {
                    if(sensor_elapsed_time > unified_sensor_reporting_invalid_reading_timeout_seconds)
                    {
                      // ALOG_INF(PSTR("sensor_id %d val.sensor_if %d"), sensor_id, val.sensor_id);
                      #ifdef ENABLE_DEVFEATURE_UNIFIED_REPORTING_SKIPPING_INVALID_TIMEOUT_READINGS
                      ALOG_WRN(PSTR("sensor time invalid %d > %d"), sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      continue; // skip the result in this loop
                      #else
                      DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));
                      ALOG_DBM(PSTR("sensor_elapsed_time missing %S %s %d %d"), pmod->GetModuleName(), buffer, sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      #endif
                    }
                  }
                }
              }
            }
            
            if(val.Valid())
            {

              /**
               * @brief Special cases
               * 
               */
              if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              {
                type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;              
              }
              else
              {
                type_id_adjusted = type_id;
              }

              if(val.isFloatWaiting_WithSensorType(type_id_adjusted))
              {
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                /**
                 * @brief Modify for special cases
                 * 
                 */
                if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
                {

                  sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);                
                  // Convert into colour
                  float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
                  #ifdef USE_MODULE_LIGHTS_INTERFACE
                  
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  RgbwwColor colour  = GetColourValueUsingMaps_ForUnifiedSensor(temperature);

                  JBI->Add_FV(
                    DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                    PSTR("\"%02X%02X%02X\""),
                    colour.R, colour.G, colour.B
                  );
                  #endif // USE_MODULE_LIGHTS_INTERFACE

                }
                /**
                 * @brief As read from sensor
                 * 
                 */
                else
                {
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  sensor_data = val.GetFloat(type_id);
                  JBI->Add(buffer, sensor_data);
                }
                
              }

              // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              // {
              //   DEBUG_LINE_HERE;
              // Serial.println(val.GetString(type_id));
              //     sensor_data_string = val.GetString(type_id);
              // Serial.println(sensor_data_string);
              // Serial.println(sensor_data_string.c_str());
              
              // }
              
              // sensor_data_string = val.GetString(type_id);
              // if(!sensor_data_string.equals("error"))
              // {

              #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
              if(val.HasString(sensor_id))
              {
              // if((sensor_data_string = val.GetString(type_id)) != "error" )
              // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
              // {

                sensor_data_string = val.GetString(type_id);

                
                // Only add sensor type if any has been found
                if(flag_level_started != true)
                {              
                  JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                  flag_level_started = true;
                  flag_level_ended_needed = true;
                }
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                JBI->Add(buffer, sensor_data_string.c_str());
                
              }
              #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

            }
          }

        }
      } // oNLY USE SENSOR MODULES

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }

  } // END sensor_type


    JBI->Add("Rate", mqtthandler_sensor_ifchanged.tRateSecs);

  return JBI->End();
    
}


#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
uint8_t mSensorsInterface::ConstructJSON_Unified_Filtered(uint8_t json_level, bool json_appending)
{
  ALOG_INF(PSTR("ConstructJSON_Unified_Filtered()"));

  JBI->Start();

  char buffer[100] = {0};

  // Step 1: Get unique sensor types
  std::vector<uint16_t> unique_types;
  for (auto& entry : filtered_sensors) {
    if (std::find(unique_types.begin(), unique_types.end(), entry.desired_type_id) == unique_types.end()) {
      unique_types.push_back(entry.desired_type_id);
    }
  }

  // Step 2: Loop through each type and group entries
  for (auto type_id : unique_types)
  {
    bool any_sensor_added = false;

    for (auto& entry : filtered_sensors)
    {
      if (entry.desired_type_id != type_id)
        continue;

      // Get module and latest raw value
      sensors_reading_t val;
      auto* module = tkr->GetModule(entry.module_id);
      if (!module) continue;

      module->GetSensorReading(&val, entry.sensor_index);
      if (!val.Valid()) continue;

      for (size_t i = 0; i < val.sensor_type.size(); i++)
      {
        if (val.sensor_type[i] == type_id && i < val.data_f.size())
        {
          float raw_value = val.data_f[i];
          float filtered_value = entry.filter_buffer.Mean();

          // Start type-level JSON object if this is the first sensor for this type
          if (!any_sensor_added)
          {
            JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
            any_sensor_added = true;
          }

          // Sensor name → object with metadata
          DLI->GetDeviceName_WithModuleUniqueID(entry.module_id, entry.sensor_index, buffer, sizeof(buffer));
          JBI->Object_Start(entry.filter_name.c_str()); // "FilterName": {
            JBI->Add("Name", buffer); // device name inside
            JBI->Add("Raw", raw_value);
            JBI->Add("Avg", filtered_value);
            JBI->Add("Samples", static_cast<uint16_t>(entry.filter_buffer.GetRawData().size()));
            JBI->Add("WindowSec", static_cast<uint16_t>(entry.window_secs));
            JBI->Add("SamplesPerSec", (float)entry.sample_count / (float)entry.window_secs);
          JBI->Object_End(); // }

          break; // done with this sensor
        }
      }
    }

    if (any_sensor_added)
    {
      JBI->Object_End(); // Close the type block
    }
  }

  return JBI->End();
}
#endif





#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
void mSensorsInterface::Update_UnifiedFilteredReadings()
{
    uint32_t now = millis();

    for (auto& entry : filtered_sensors)
    {
        if (now - entry.tLastUpdate < entry.sample_interval_ms) {
            continue; // Not time to sample this sensor yet
        }

        // Find module by unique ID
        auto* module = tkr->GetModule(entry.module_id);
        if (!module) {
            continue; // Module not found
        }

        sensors_reading_t val;
        module->GetSensorReading(&val, entry.sensor_index);

        if (!val.Valid()) {
            continue; // Skip invalid reading
        }

        // Look for the desired sensor_type within the reading
        for (size_t i = 0; i < val.sensor_type.size(); i++) {
            ALOG_DBM(PSTR("sensor_type[%d] = %d"), i, val.sensor_type[i]);

            if (val.sensor_type[i] == entry.desired_type_id && i < val.data_f.size()) {
                entry.filter_buffer.Add(val.data_f[i]);
                // Serial.print(val.data_f[i]); Serial.print(" ");
                // Serial.println(entry.filter_buffer.Mean());
                entry.tLastUpdate = now;
                break; // Only add the first match
            }
        }
    }
}
#endif





/**
 * @brief For any connected temperature sensors, display their colour as a full brightness rgb colour
 * */
uint8_t mSensorsInterface::ConstructJSON_SensorTemperatureColours(uint8_t json_level, bool json_appending){


  JBI->Start();

  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  // for (
    uint16_t type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:tkr->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%S"), sensors_available, pmod->GetModuleName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Object_Start(PM_TEMPERATURE_HEATMAP_RGBSTRING);//PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              // sensor_data = tkr_debug->debug_data.input_float1;//map(tkr_debug->debug_data.input_float1, 0,59, 0,70);

              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP // I need to make a version that does not require lighting at all, so simple colour conversion
              uint32_t colour  = GetColourValueUsingMaps_FullBrightness(temperature, 0);
              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

              // ALOG_INF(PSTR("%s %d \"%02X%02X%02X\""), buffer, (int)(sensor_data*1000), R32(colour), G32(colour), B32(colour));

              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }





  // for (
    // uint16_t
    type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:tkr->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Level_Start_P(PM_TEMPERATURE_HEATMAP_ADJUSTED_BRIGHTNESS_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
              uint32_t colour  = GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP


              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }









    






  // } // END sensor_type

  return JBI->End();

}



#ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
/**
 * @brief future global way to map colours using palette
 * 
 * Option in the future to apply different colours to the map.
 * 
 * Global temp colours reporting should have flag otpion to pick between full/adjusted brightness inside primary unified sensor.
 * I should still produce them all for the sub mqtt colours
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return uint32_t colour packed W,R,G,B 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_AdjustedBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 10;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;
  
  HsbToRgb(hue/360.0f,1.0f,1.0f,&r,&g,&b);

  return RGBW32(r,g,b,w);



}

/**
 * @brief maximum sat/brt, hue changes
 * 
 * In the future, simply create "profiles" that have common temp to colour mappings
 * ie "Hot Water", "Room Comfort"
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_FullBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 100;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

  // Generate Hue and Brt values
  if(value<15){
    hue = 240;
    brt = 100;
  }else
  if((value>=15)&&(value<25)){  // BLUE to GREEN
    hue = mSupport::mapfloat(value, 15,25, 240,120);
  }else
  if((value>=25)&&(value<30)){  //         GREEN to YELLOW
    hue = mSupport::mapfloat(value, 25,30, 120,60);
  }else
  if((value>=30)&&(value<35)){  //                  YELLOW to ORANGE
    hue = mSupport::mapfloat(value, 30,35, 60,17);
  }else
  if((value>=35)&&(value<40)){  //                            ORANGE to RED
    hue = mSupport::mapfloat(value, 35,40, 17,0);
  }else
  if((value>=40)&&(value<50)){  //                                      RED
    hue = 0;
  }else
  if((value>=50)&&(value<60)){  //                                      RED to PINK
    hue = mSupport::mapfloat(value, 50,60, 360,340);
  }else
  if(value>=60){   // PINK
    hue = 340;
  }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;

  // hue = 180;

  // char buffer[20];
  // mSupport::float2CString(value, JSON_VARIABLE_FLOAT_PRECISION_LENGTH, buffer);       
  // ALOG_INF(PSTR("value=%s, hue=%d"), buffer, hue);
  
  HsbToRgb((float)hue/360.0f,1.0f,1.0f,&r,&g,&b);
  // HsbToRgb(0.0f,1.0f,1.0f,&r,&g,&b);

  return  RGBW32(r,g,b,w);

}


void mSensorsInterface::HsbToRgb(float h, float s, float v, uint8_t* r8, uint8_t* g8, uint8_t* b8)
{
  float r, g, b;

  if (s == 0.0f)
  {
    r = g = b = v; // achromatic or black
  }
  else
  {
    if (h < 0.0f)
    {
      h += 1.0f;
    }
    else if (h >= 1.0f)
    {
      h -= 1.0f;
    }
    h *= 6.0f;
    int i = (int)h;
    float f = h - i;
    float q = v * (1.0f - s * f);
    float p = v * (1.0f - s);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
    }
  }

  *r8 = r * 255;
  *g8 = g * 255;
  *b8 = b * 255;

}

#endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

float mSensorsInterface::ConvertTemp(float c)
{
  float result = c;
  // if (!isnan(c) && Settings.sysopt_system.bit.temperature_conversion) {
  //   result = c * 1.8 + 32;  // Fahrenheit
  // }
  return result;
}


char mSensorsInterface::TempUnit(void)
{
  return (tkr_set->Settings.sysopt_sensors.bit.temperature_conversion) ? 'F' : 'C';
}


float mSensorsInterface::ConvertPressure(float p)
{
  // float result = p;

  // if (!isnan(p) && Settings.sysopt_system.bit.pressure_conversion) {
  //   result = p * 0.75006375541921;  // mmHg
  // }
  // return result;
}

String mSensorsInterface::PressureUnit(void)
{
  // return (Settings.sysopt_system.bit.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}




/***
 * 
 * The idea is that this function, is the unified place that reports motion
 * But other subfunctions should still report their own motion etc
 * In this case, the "PIR module" should be used instead of SWT and rules, PIR_1 is the new way, not SWT and rule
 * 
 */
uint8_t mSensorsInterface::ConstructJSON_Event_Motion(uint8_t json_level, bool json_appending){

  char buffer[100];

  JBI->Start();

  /**
   * @brief Motion Event : PIR module
   **/
  #ifdef USE_MODULE_SENSORS_PIR
  if(tkr_rules->event_triggered.module_id == tkr_motion->GetModuleUniqueID())
  {
    uint16_t device_id   = tkr_rules->event_triggered.device_id;
    uint16_t state_id = tkr_rules->event_triggered.value.data[0];  

    JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( tkr_motion->GetModuleUniqueID(), device_id, buffer, sizeof(buffer))); 
    JBI->Add("Time", tkr_time->GetTimeStr(tkr_time->Rtc.local_time).c_str());
    JBI->Add("UTCTime", tkr_time->Rtc.local_time);
    JBI->Add(D_EVENT, state_id ? "detected": "over");
    JBI->Add("Sensor", tkr_motion->GetModuleName());

  }
  #endif // USE_MODULE_SENSORS_PIR

  return JBI->End();
    
}



uint8_t mSensorsInterface::ConstructJSON_Event_UserInput(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(!tkr_rules->event_triggered.isvalid) return JBI->End();

  JBI->Start();

  JBI->Add("Source", tkr->GetModuleName( tkr_rules->event_triggered.module_id ));
  JBI->Add("Name", DLI->GetDeviceName_WithModuleUniqueID( tkr_rules->event_triggered.module_id, tkr_rules->event_triggered.device_id, buffer, sizeof(buffer)));
  JBI->Add("Value", tkr_rules->event_triggered.value.data[0]);

  char state[100];

  #ifdef USE_MODULE_SENSORS_SWITCHES
  if(tkr_rules->event_triggered.module_id == tkr_switch->GetModuleUniqueID())
  {
    tkr_switch->GetStateName(tkr_rules->event_triggered.value.data[0], state, sizeof(state));
  }
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
  if(tkr_rules->event_triggered.module_id == tkr_button->GetModuleUniqueID())
  {
    tkr_button->GetStateName(tkr_rules->event_triggered.value.data[0], tkr_rules->event_triggered.value.data[1], state, sizeof(state)); // data0=type, data1=presses
  }
  #endif

  JBI->Add("State", state);
  JBI->Add("LocalTime", tkr_time->GetTime().c_str());


  return JBI->End();
    
}


uint8_t mSensorsInterface::ConstructJSON_System_Location(uint8_t json_level, bool json_appending)
{

  char buffer[100] = {0};

  JBI->Start();

  const sensorset_location_t& loc = system_location;

  JBI->Add("SystemUptime", tkr_time->uptime_seconds_nonreset);
  
  // -------------------------------------------------------------------------
  // Validity / source
  // -------------------------------------------------------------------------
  JBI->Add("Valid",        loc.isvalid);
  JBI->Add("SourceID",     loc.source_id);
  JBI->Add("FixType",      loc.fix_type);
  JBI->Add("FixQuality",   loc.fix_quality);

  if(loc.isvalid)
  {

  // -------------------------------------------------------------------------
  // Timing
  // -------------------------------------------------------------------------
  JBI->Add("UpdatedMs",    loc.updated_millis);
  JBI->Add("FixAgeMs",     loc.fix_age_ms);
  JBI->Add("Stale",        loc.is_stale);
  JBI->Add("TimeValid",    loc.time_valid);

  JBI->Add("Year",         loc.year);
  JBI->Add("Month",        loc.month);
  JBI->Add("Day",          loc.day);
  JBI->Add("Hour",         loc.hour);
  JBI->Add("Minute",       loc.minute);
  JBI->Add("Second",       loc.second);
  JBI->Add("UTCSeconds",   loc.utc_time_secs);

  char utc_buffer[32];
  snprintf_P(
    utc_buffer,
    sizeof(utc_buffer),
    PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ"),
    loc.year,
    loc.month,
    loc.day,
    loc.hour,
    loc.minute,
    loc.second
  );
  JBI->Add("UTC", utc_buffer);

  // -------------------------------------------------------------------------
  // Position
  // -------------------------------------------------------------------------
  JBI->Add("Latitude",     loc.latitude);
  JBI->Add("Longitude",    loc.longitude);
  JBI->Add("Altitude",     loc.altitude);

  char convf_lat[TBUFFER_SIZE_FLOAT];  mSupport::float2CString(system_location.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat); 
  char convf_lon[TBUFFER_SIZE_FLOAT];  mSupport::float2CString(system_location.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];  mSupport::float2CString(system_location.accuracy,2,convf_fix);
  snprintf_P(buffer, sizeof(buffer),   PSTR("https://www.google.com/maps/dir//%s,%s"), convf_lat, convf_lon);
  JBI->Add("url", buffer);

  // -------------------------------------------------------------------------
  // Motion
  // -------------------------------------------------------------------------
  JBI->Add("Speed",        loc.speed);
  JBI->Add("Course",       loc.course);

  }

  // -------------------------------------------------------------------------
  // Accuracy / dilution / precision
  // -------------------------------------------------------------------------
  JBI->Add("Accuracy",           loc.accuracy);
  JBI->Add("AccuracyPosition",   loc.accuracy_position);
  JBI->Add("AccuracyVertical",   loc.accuracy_vertical);

  JBI->Add("HDOP",               loc.hdop);
  JBI->Add("VDOP",               loc.vdop);
  JBI->Add("PDOP",               loc.pdop);

  JBI->Add("HPARaw",             loc.hpa_raw);
  JBI->Add("VPARaw",             loc.vpa_raw);

  // -------------------------------------------------------------------------
  // Satellite summary
  // -------------------------------------------------------------------------
  JBI->Add("SatellitesUsed",     loc.satellites_used);
  JBI->Add("SatellitesView",     loc.satellites_view);
  JBI->Add("SatellitesGPS",      loc.satellites_gps);
  JBI->Add("SatellitesGLONASS",  loc.satellites_glonass);
  JBI->Add("CNOMax",             loc.cno_max);

  // -------------------------------------------------------------------------
  // Selection metadata
  // -------------------------------------------------------------------------
  JBI->Add("Priority",           loc.priority);

  return JBI->End();
}

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* ptr;

  ALOG_INF(PSTR("MQTTHandler_Init size %d"), mqtthandler_list.size()  );
 
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_temperature_colours;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_SensorTemperatureColours;
  mqtthandler_list.push_back(ptr);
  
  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
  ptr = &mqtthandler_sensor_unified_filtered;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED_FILTERED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Unified_Filtered;
  mqtthandler_list.push_back(ptr);
  #endif

  //motion events
  ptr = &mqtthandler_motion_event_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_MOTION__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Event_Motion;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_event_input;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_USER_INPUT__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Event_UserInput;
  mqtthandler_list.push_back(ptr);


  ptr = &mqtthandler_system_location; 
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_SYSTEM_LOCATION__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_System_Location;
  mqtthandler_list.push_back(ptr);


} 

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif
