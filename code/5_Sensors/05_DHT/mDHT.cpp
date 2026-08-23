/*
  mSensorsDHT.cpp - mSensorsDHT

  Copyright (C) 2025  Michael

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
#include "mDHT.h"

#ifdef USE_MODULE_SENSORS_DHT

int8_t mSensorsDHT::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!module_state.mode){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    case TASK_LOOP:
      EveryLoop();
    break;
    case TASK_SENSOR_SHOW_LATEST_LOGGED_ID:
      ConstructJSON_Sensor(JSON_LEVEL_SHORT);
      ALOG_INF(PSTR(D_LOG_DHT "\"%s\""),JBI->GetBufferPtr());
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
  } // END switch

  return TASKER_RESULT__SUCCESS_ID;
  
}// END Tasker

void mSensorsDHT::ClearSensors(void)
{
  for (uint8_t i = 0; i < MAX_DHT_SENSORS; i++) {
    if (s[i].dht) {
      delete s[i].dht;
      s[i].dht = nullptr;
    }

    s[i].temperature = NAN;
    s[i].humidity    = NAN;
    s[i].heatIndex   = NAN;
    s[i].dewPoint    = NAN;
    s[i].cr          = NAN;

    s[i].isvalid = 0;
    s[i].ischanged = 0;

    s[i].next_poll_ms   = 0;
    s[i].backoff_ms     = DHT_BACKOFF_MIN_MS;
    s[i].last_ok_ms     = 0;
    s[i].last_change_ms = 0;
  }

  module_state.devices = 0;
  next_rescan_ms = millis() + DHT_RESCAN_PERIOD_MS;
}

bool mSensorsDHT::AddSensor(uint16_t gpio_base, uint8_t index, DHTesp::DHT_MODEL_t model, const char* tag)
{
  if (!tkr_pins->PinUsed(gpio_base, index)) return false;

  const uint8_t sensor_i = module_state.devices;
  if (sensor_i >= MAX_DHT_SENSORS) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "MAX_DHT_SENSORS reached, skipping %s[%u]"), tag, index);
    return false;
  }

  const int16_t pin = tkr_pins->GetPin(gpio_base, index);
  if (pin < 0) {
    AddLog(LOG_LEVEL_WARNING, PSTR(D_LOG_DHT "%s[%u] configured but pin invalid"), tag, index);
    return false;
  }

  s[sensor_i].dht = new DHTesp;
  if (!s[sensor_i].dht) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "%s[%u] DHTesp allocation failed"), tag, index);
    return false;
  }

  s[sensor_i].gpio_base  = gpio_base;
  s[sensor_i].gpio_index = index;
  s[sensor_i].pin        = pin;

  s[sensor_i].dht->setup(pin, model);

  const uint32_t now = millis();
  s[sensor_i].next_poll_ms = now + 250;
  s[sensor_i].backoff_ms   = DHT_BACKOFF_MIN_MS;

  // ensure flags/data are reset for this slot
  s[sensor_i].isvalid = 0;
  s[sensor_i].ischanged = 0;
  s[sensor_i].temperature = NAN;
  s[sensor_i].humidity    = NAN;
  s[sensor_i].heatIndex   = NAN;
  s[sensor_i].dewPoint    = NAN;
  s[sensor_i].cr          = NAN;
  s[sensor_i].last_ok_ms = 0;
  s[sensor_i].last_change_ms = 0;

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DHT "%s[%u] sensor_i=%u pin=%d"), tag, index, sensor_i, pin);

  module_state.devices++;
  return true;
}

void mSensorsDHT::Pre_Init(void)
{
  ClearSensors();

  for (uint8_t dht_i = 0; dht_i < MAX_DHT_SENSORS_PER_MODEL; dht_i++) {
    AddSensor(GPIO_DHT11, dht_i, DHTesp::DHT11, "DHT11");
  }

  for (uint8_t dht_i = 0; dht_i < MAX_DHT_SENSORS_PER_MODEL; dht_i++) {
    AddSensor(GPIO_DHT22, dht_i, DHTesp::DHT22, "DHT22");
  }

  if (module_state.devices) {
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DHT "Enabled (%u sensor%s)"),
           module_state.devices,
           module_state.devices == 1 ? "" : "s");
  } else {
    module_state.mode = ModuleStatus::Disabled;
    AddLog(LOG_LEVEL_WARNING, PSTR(D_LOG_DHT "No sensors configured"));
  }
}

void mSensorsDHT::Init(void)
{
  const uint32_t now = millis();
  for (uint8_t i = 0; i < module_state.devices; i++) {
    s[i].next_poll_ms = now + 250;
  }
}

bool mSensorsDHT::PollOne(uint8_t i)
{
  if (i >= module_state.devices) return false;
  if (!s[i].dht) return false;

  TempAndHumidity v = s[i].dht->getTempAndHumidity();

  if (s[i].dht->getStatus() != 0 || isnan(v.temperature) || isnan(v.humidity)) {
    s[i].isvalid = 0;
    return false;
  }

  const bool had_prev = (s[i].isvalid != 0);

  bool changed = true;
  bool changed_thr = true;

  if (had_prev) {
    const float dt = fabsf(s[i].temperature - v.temperature);
    const float dh = fabsf(s[i].humidity    - v.humidity);
    changed     = (dt > 0.0f) || (dh > 0.0f);
  }

  const uint32_t now = millis();

  s[i].isvalid = 1;
  s[i].ischanged = changed ? 1 : 0;
  s[i].last_ok_ms = now;
  if (changed_thr) s[i].last_change_ms = now;

  s[i].temperature = v.temperature;
  s[i].humidity    = v.humidity;

  ComfortState cf;
  s[i].heatIndex = s[i].dht->computeHeatIndex(v.temperature, v.humidity);
  s[i].dewPoint  = s[i].dht->computeDewPoint(v.temperature, v.humidity);
  s[i].cr        = s[i].dht->getComfortRatio(cf, v.temperature, v.humidity);

  return true;
}

void mSensorsDHT::EveryLoop(void)
{
  const uint32_t now = millis();

  // optional: rescan if none configured/found
  if (!module_state.devices) {
    if (mTime::TimeReachedNonReset(&next_rescan_ms, DHT_RESCAN_PERIOD_MS)) {
      Pre_Init();
    }
    return;
  }

  for (uint8_t i = 0; i < module_state.devices; i++) {

    if (!mTime::TimeReachedNonReset(&s[i].next_poll_ms, 500)) continue;

    const bool ok = PollOne(i);

    if (ok) {
      s[i].backoff_ms = DHT_BACKOFF_MIN_MS;
      s[i].next_poll_ms = now + DHT_POLL_PERIOD_MS;
    } else {
      AddLog(LOG_LEVEL_WARNING, PSTR(D_LOG_DHT "Read failed idx=%u status=%d"),
             i, s[i].dht ? s[i].dht->getStatus() : -1);

      uint32_t b = s[i].backoff_ms;
      b = (b < (DHT_BACKOFF_MAX_MS / 2)) ? (b * 2) : DHT_BACKOFF_MAX_MS;
      if (b < DHT_BACKOFF_MIN_MS) b = DHT_BACKOFF_MIN_MS;
      s[i].backoff_ms = b;

      s[i].next_poll_ms = now + s[i].backoff_ms;
    }
  }
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SensorCount", GetSensorCount());
    JBI->Array_Start("Pin");
      for (uint8_t sensor_i = 0; sensor_i < GetSensorCount(); sensor_i++) {
        JBI->Add(s[sensor_i].pin);
      }
    JBI->Array_End();
  return JBI->End();

}

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[50];

  JBI->Start();
  JBI->Add("SensorCount", GetSensorCount());
  for(uint8_t sensor_id=0;sensor_id<GetSensorCount();sensor_id++){
    if(
      s[sensor_id].ischanged || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){

      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer)));   
        JBI->Add(D_TEMPERATURE, s[sensor_id].temperature);
        JBI->Add(D_HUMIDITY,    s[sensor_id].humidity);
      JBI->Object_End(); 
    }else{
      ALOG_INF(PSTR(D_LOG_DHT "Skipping sensor_id=%u no change"), sensor_id);
    }

  }
    
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsDHT::Telemetry_Init()
{

  struct telemetry_handler<mSensorsDHT>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);
  
} 
  
#endif // USE_MODULE_NETWORK_MQTT

#endif
