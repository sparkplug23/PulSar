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
#include "mSensorsDHT.h"

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
      ShowSensor_AddLog();
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
    s[i].ischanged_over_threshold = 0;

    s[i].next_poll_ms   = 0;
    s[i].backoff_ms     = DHT_BACKOFF_MIN_MS;
    s[i].last_ok_ms     = 0;
    s[i].last_change_ms = 0;
  }

  module_state.devices = 0;
  next_rescan_ms = millis() + DHT_RESCAN_PERIOD_MS;
}

bool mSensorsDHT::AddSensor(uint8_t gpio_function, DHTesp::DHT_MODEL_t model, const char* tag)
{
  if (!tkr_pins->PinUsed(gpio_function)) return false;

  const uint8_t idx = module_state.devices;
  if (idx >= MAX_DHT_SENSORS) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "MAX_DHT_SENSORS reached, skipping %s"), tag);
    return false;
  }

  int16_t pin = tkr_pins->GetPin(gpio_function);

  s[idx].dht = new DHTesp;
  s[idx].dht->setup(pin, model);

  const uint32_t now = millis();
  s[idx].next_poll_ms = now + 250;
  s[idx].backoff_ms   = DHT_BACKOFF_MIN_MS;

  // ensure flags/data are reset for this slot
  s[idx].isvalid = 0;
  s[idx].ischanged = 0;
  s[idx].ischanged_over_threshold = 0;
  s[idx].temperature = NAN;
  s[idx].humidity    = NAN;
  s[idx].heatIndex   = NAN;
  s[idx].dewPoint    = NAN;
  s[idx].cr          = NAN;
  s[idx].last_ok_ms = 0;
  s[idx].last_change_ms = 0;

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DHT "%s idx=%u pin=%u"), tag, idx, pin);

  module_state.devices++;
  return true;
}

void mSensorsDHT::Pre_Init(void)
{
  ClearSensors();

  AddSensor(GPIO_DHT11_1, DHTesp::DHT11, "DHT11_1of2");
  AddSensor(GPIO_DHT11_2, DHTesp::DHT11, "DHT11_2of2");
  AddSensor(GPIO_DHT22_1, DHTesp::DHT22, "DHT22_1of2");
  AddSensor(GPIO_DHT22_2, DHTesp::DHT22, "DHT22_2of2");

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
    changed_thr = (dt >= DHT_CHANGE_THRESH_C) || (dh >= DHT_CHANGE_THRESH_RH);
  }

  const uint32_t now = millis();

  s[i].isvalid = 1;
  s[i].ischanged = changed ? 1 : 0;
  s[i].ischanged_over_threshold = changed_thr ? 1 : 0;
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

    if (!mTime::TimeReachedNonReset(&s[i].next_poll_ms, 0)) continue;

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



void mSensorsDHT::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_DHT "\"%s\""),JBI->GetBufferPtr());

}

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SensorCount", module_state.devices);
    JBI->Array_Start("Pin");
      // JBI->Add(s[0].pin);
      // JBI->Add(pin[1]);
    JBI->Array_End();
  return JBI->End();

}

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[50];

  JBI->Start();
  JBI->Add("SensorCount", module_state.devices);
  for(uint8_t sensor_id=0;sensor_id<module_state.devices;sensor_id++){
    if(
      s[sensor_id].ischanged || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){

      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer)));   
        JBI->Add(D_TEMPERATURE, s[sensor_id].temperature);
        JBI->Add(D_HUMIDITY,    s[sensor_id].humidity);
        if(json_level >=  JSON_LEVEL_DETAILED)
        {     
          JBI->Object_Start(D_ISCHANGEDMETHOD);
            JBI->Add(D_TYPE, D_SIGNIFICANTLY);
            JBI->Add(D_AGE, (uint16_t)round((millis()-s[sensor_id].last_ok_ms)/1000));
          JBI->Object_End();   
        }
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

void mSensorsDHT::MQTTHandler_Init()
{

  struct handler<mSensorsDHT>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 
  
#endif // USE_MODULE_NETWORK_MQTT

#endif
