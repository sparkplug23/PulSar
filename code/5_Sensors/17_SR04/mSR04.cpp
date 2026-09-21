
#include "mSR04.h"

#ifdef USE_MODULE_SENSORS_SR04

int8_t mSR04::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  
  switch(function){

    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
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
    case TASK_EVERY_SECOND:
      // EverySecond();

      // if (tkr_pins->PinUsed(GPIO_SR04_ECHO_ID, GPIO_ANY)) 
      // { 
        for (uint32_t i = 0; i < MAX_SR04; i++) 
        {
          // ALOG_INF(PSTR("type %d"),SR04[i].type);
          if (SR04[i].type) 
          {
            Reading(i);
            ALOG_INF(PSTR(D_LOG_SR04 "Distance: %d mm"),(int)SR04[0].distance*10);
          }
        }
      // }

    break;
    case TASK_EVERY_MINUTE:
      EveryMinute();
    break;
    case TASK_UPTIME_1_MINUTES:
      #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
      readings.average_EMA.alpha = 2.0f / (200.0f-1.0f); // 60 samples, slower
      #endif
      Config_Filters();
    break;
    /************
     * COMMANDS SECTION * 
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
  
  return function_result;

} // END function



void mSR04::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  if (tkr_pins->PinUsed(GPIO_SR04_TRIG) && tkr_pins->PinUsed(GPIO_SR04_ECHO))
  {
    module_state.mode = ModuleStatus::Running;
    module_state.devices++;
  }
}


void mSR04::Config_Filters()
{

  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  /*  As we said in the previous post, in the exponential type filter the amount of smoothing is controlled through the alpha factor. It is not easy to find a simple relationship with the cut-off frequency, among other things, because the sampling frequency is unknown and variable.
      What we do know is that the lower the alpha factor, the greater the smoothing of the signal, and therefore, the lower the cut-off frequency. Remember that, as a negative point, the delay between the filtered signal and the original signal will also be greater.
      In a bandpass and stop band filter we will have two alpha factors that we will call Low and High that correspond, respectively, with the lower and upper cut frequency.
      Below we will see the effect of these factors with the results of the bandpass filter for different combinations of alpha Low and High applied to the same signal.
      These are the results for an alpha Low of 0.025 and alpha High of 0.5. Note that the lower filter has removed the bias after a few oscillations, while the upper filter has softened some of the high-frequency noise
  */
  readings.average_EMA.filter  = new SingleEMAFilter<float>(readings.average_EMA.alpha);  // alpha = 2/(N-1) so for N=60 ie 1/sec for 1 minute
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.filter = new DoubleEMAFilter<float>(readings.average_DEMA.alpha1, readings.average_DEMA.alpha2);
  #endif // ENABLE_DEVFEATURE_SR04_SINGLE_EMA_FILTERING

  telemetry_settings.flags.SendNow = true;

}


void mSR04::Init(void)
{

  int sr04_echo_pin = tkr_pins->Pin(GPIO_SR04_ECHO, 0);
  int sr04_trig_pin = tkr_pins->Pin(GPIO_SR04_TRIG, 0); 
  AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04: Init Detect mode pins TX%d, RX%d"), sr04_trig_pin, sr04_echo_pin);

  if (sonar_serial==nullptr && sonar==nullptr) {
    ModeDetect();
  }

  ALOG_TST(PSTR(D_LOG_SR04 "ModeDetect Complete"));

  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  // Default weight values
  readings.average_EMA.alpha = 2.0f / (5.0f-1.0f); // faster
  // readings.average_EMA.alpha = 2.0f / (60.0f-1.0f); // 60 samples, slower
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.alpha1 = 2.0f / (100.0f-1.0f);
  readings.average_DEMA.alpha2 = 1-(2.0f / (100.0f-1.0f));
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

  Config_Filters();

  #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  readings.conversion_settings.flag_distance_conversion_method = EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_TEMPERATURE_COMPENSATED__ID;
  readings.temp_adj.flag_enabled = true;
  #endif

}


void mSR04::ModeDetect(void) {
  for (uint32_t i = 0; i < MAX_SR04; i++) {
    SR04[i].type = SR04_MODE_NONE;
    if (!tkr_pins->PinUsed(GPIO_SR04_ECHO, i)){ 
      ALOG_TST(PSTR("Sr04: No ECHO %d"),i);
      continue; 
    }
    sr04_sensor_count++;
    int sr04_echo_pin = tkr_pins->Pin(GPIO_SR04_ECHO, i);
    int sr04_trig_pin = tkr_pins->Pin(GPIO_SR04_TRIG, i);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_TRIG as -1
    SR04[i].sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

    AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04: ADetect mode pins TX%d, RX%d"), sr04_trig_pin, sr04_echo_pin);

    if (SR04[i].sonar_serial && SR04[i].sonar_serial->begin(9600)) {
      // DEBUG_SENSOR_LOG(PSTR("SR4: Detect mode"));
      AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04:B Detect mode pins TX%d, RX%d"), sr04_trig_pin, sr04_echo_pin);

      if (tkr_pins->PinUsed(GPIO_SR04_TRIG, i)) {
        SR04[i].type = (MiddleValue(Mode3Distance(i), Mode3Distance(i), Mode3Distance(i)) != 0) ? SR04_MODE_SER_TRANSCEIVER : SR04_MODE_TRIGGER_ECHO;
      } else {
        SR04[i].type = (MiddleValue(Mode2Distance(i), Mode2Distance(i), Mode2Distance(i)) != 0) ? SR04_MODE_SER_RECEIVER : SR04_MODE_TRIGGER_ECHO;
      }
    } else {
      SR04[i].type = SR04_MODE_TRIGGER_ECHO;
    }

    ALOG_INF(PSTR("type = %d"), SR04[i].type);

    if (SR04[i].type < SR04_MODE_SER_RECEIVER) {
      ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
      if (SR04[i].sonar_serial) {
        ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
        delete SR04[i].sonar_serial;
        ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
        SR04[i].sonar_serial = nullptr;
        ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
      }
      ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
      sr04_trig_pin = (tkr_pins->PinUsed(GPIO_SR04_TRIG, i)) ? tkr_pins->Pin(GPIO_SR04_TRIG,i ) : tkr_pins->Pin(GPIO_SR04_ECHO, i);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
      SR04[i].sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, SR04_MAX_SENSOR_DISTANCE);
      delay(100); // give time to inizialise, preventing ping_median fails
      ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
      if (!SR04[i].sonar || !SR04[i].sonar->ping_median(5)) {
        SR04[i].type = SR04_MODE_NONE;
      }
      ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
    } else {
      ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
      if (SR04[i].sonar_serial) {
        ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
        if (SR04[i].sonar_serial->hardwareSerial()) {
          ALOG_INF(PSTR("NewPing %d"), __LINE__); Serial.println();
          tkr_sup->ClaimSerial();// should be within serial module, not support!
          AddLog(LOG_LEVEL_INFO,PSTR("ClaimSerial NotEnabled %d %d"), sr04_trig_pin, sr04_echo_pin);
        }
  #ifdef ESP32
        AddLog(LOG_LEVEL_DEBUG, PSTR("SR4-%d: Serial UART%d"), i+1, SR04[i].sonar_serial->getUart());
  #endif
      }
    }

    AddLog(LOG_LEVEL_INFO,PSTR("SR4-%d: Mode %d"), i+1, SR04[i].type);
  }
}


uint16_t mSR04::MiddleValue(uint16_t first, uint16_t second, uint16_t third)
{
  uint16_t ret = first;
  if (first > second) {
    first = second;
    second = ret;
  }

  if (third < first) {
    return first;
  } else if (third > second) {
    return second;
  } else {
    return third;
  }
}


uint16_t mSR04::Mode2Distance(uint32_t i) {
  uint8_t buffer[4];                  // Accommodate either 2 or 4 bytes of data
  uint32_t buffer_idx = 0;
  uint32_t end = millis() + 100;
  while (millis() < end) {
    if (SR04[i].sonar_serial->available() && (buffer_idx < sizeof(buffer))) {
      buffer[buffer_idx++] = SR04[i].sonar_serial->read();
      end = millis() + 10;
    }
    delay(1);
  }
  if (SR04_MODE_NONE == SR04[i].type) {  // Only log during detection
    AddLog(LOG_LEVEL_DEBUG, PSTR("SR4-%d: Received '%*_H'"), i+1, buffer_idx, buffer);
  }

  uint32_t distance = 0;
  if (buffer_idx > 2) {               // JSN-SR04T serial has four bytes
    // FF00FAF9
    uint8_t crc = buffer[0];
    crc += buffer[1];
    crc += buffer[2];
    if (crc == buffer[3]) {           // Check crc sum
      distance = (buffer[1] << 8) + buffer[2];
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("SR4-%d: CRC error"), i+1);
    }
  }
  else if (buffer_idx > 1) {          // US-100 serial has no CRC
    // 00FA = 250 millimeter
    distance = (buffer[0] << 8) + buffer[1];
  }

  return distance;
}


uint16_t mSR04::Mode3Distance(uint32_t i) {
  SR04[i].sonar_serial->write(0x55);
  SR04[i].sonar_serial->flush();

  return Mode2Distance(i);
}

void mSR04::EverySecond(void)
{

  MeasureSensor();

}


void mSR04::EveryMinute(void)
{
  #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  SubTask_UpdateAmbientTemperature();
  #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
}

void mSR04::Reading(uint32_t i) {
  if (tkr_time->uptime_seconds_nonreset < 3) { return; }
  if (SR04[i].valid) {
    SR04[i].valid--;
  } else {
    SR04[i].distance = 0;
  }

  float distance;
  switch (SR04[i].type) {
    case SR04_NOT_DETECTED:
      ModeDetect();
      SR04[i].valid = (SR04[i].type) ? SENSOR_MAX_MISS : 0;
      break;
    case SR04_MODE_SER_TRANSCEIVER:
      distance = (float)(MiddleValue(Mode3Distance(i), Mode3Distance(i), Mode3Distance(i))) / 10;  // Convert to cm
      break;
    case SR04_MODE_SER_RECEIVER:
      //empty input buffer first
      while(SR04[i].sonar_serial->available()) { SR04[i].sonar_serial->read(); }
      distance = (float)(MiddleValue(Mode2Distance(i), Mode2Distance(i), Mode2Distance(i))) / 10;  // Convert to cm
      break;
    case SR04_MODE_TRIGGER_ECHO:
      distance = (float)(SR04[i].sonar->ping_median(5)) / US_ROUNDTRIP_CM;
      break;
    default:
      distance = 0;
  }

  if (distance) {
    SR04[i].distance = distance;
    SR04[i].valid = SENSOR_MAX_MISS;
  }

}

void mSR04::MeasureSensor()
{
  
  switch (sr04_type)
  {
    case 3:
      // readings.distance_cm = (float)(MiddleValue(Mode3Distance(),Mode3Distance(),Mode3Distance()))/ 10; //convert to cm
      break;
    case 2:
      while(sonar_serial->available()) sonar_serial->read(); // empty input buffer first
      // readings.distance_cm = (float)(MiddleValue(Mode2Distance(),Mode2Distance(),Mode2Distance()))/10;
      break;
    case 1:
      readings.raw.ping_value = sonar->ping_median(1);
      Serial.println(readings.raw.ping_value);
      readings.raw.distance_cm = GetDistanceFromPing(readings.raw.ping_value);
      break;
    default:
      readings.raw.ping_value = NO_ECHO;
  }
  
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  readings.average_EMA.distance_cm  = readings.average_EMA.filter->AddValue(readings.raw.distance_cm);
  #endif
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.distance_cm = readings.average_DEMA.filter->AddValue(readings.raw.distance_cm);
  #endif

  ALOG_TST(PSTR("GetDistanceFromPing %d (cm)"), (int)readings.raw.distance_cm);

  telemetry_sensor_ifchanged.flags.SendNow = true;

}

float mSR04::GetDistanceFromPing(uint32_t ping_value)
{
  if(ping_value==0){ return 0; }
  float distance_cm = 0;
  
  switch(readings.conversion_settings.flag_distance_conversion_method)
  {
    default:
    case EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID:
      distance_cm = (float)(ping_value)/ US_ROUNDTRIP_CM;
    break;
    case EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_DEFAULT__ID:
    {
      readings.conversion_settings.speed_of_sound = 343.0f;
      float speed_of_sound_cm_per_us = readings.conversion_settings.speed_of_sound/10000.0f;
      distance_cm = (float)(ping_value)*(speed_of_sound_cm_per_us/2.0f);
    }
    break;
    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    case EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_TEMPERATURE_COMPENSATED__ID:
    {
      // readings.temp_adj.ambient_temperature = 23.0f;
      // need to check flag here
      readings.conversion_settings.speed_of_sound = 331.3 + (0.606 * readings.temp_adj.ambient_temperature); 
      float speed_of_sound_cmps = readings.conversion_settings.speed_of_sound/100.0f;
      distance_cm = (float)(ping_value)*(speed_of_sound_cmps/2.0f/100.0f);
    }
    break;
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  }
  
  return distance_cm;

}



/**
 * @brief Check if any sensor reports temperature and has friendly name "AmbientTemperature"
 * 
 **/
#ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
void mSR04::SubTask_UpdateAmbientTemperature()
{

  ALOG_INF(PSTR("SubTask_UpdateAmbientTemperature"));

  /**
   * @brief I need a way to make this runtime safe
   **/

  float temperature = 0;
  uint8_t  device_id = 0; //ambient tank should be 0

  sensors_reading_t reading;
  tkr->GetModule(tkr_db18->GetModuleUniqueID())->GetSensorReading(&reading, device_id);
  temperature = reading.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);

  Serial.printf("temperature=%f\n\r", temperature);

  readings.temp_adj.ambient_temperature = temperature;

}
#endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSR04::parse_JSONCommand(JsonParserObject obj)
{


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_TST(PSTR(D_LOG_LIGHT "mSR04::parse_JSONCommand %d"),obj.isNull());
  #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  int val = 0;

  float a1 = 0;
  float a2 = 0;

  if(jtok = obj["SetAlpha1"])
  {
    a1= jtok.getFloat();
  }

  if(jtok = obj["SetAlpha2"])
  {
    a2= jtok.getFloat();
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    readings.average_EMA.filter->SetAlpha(a1);
    #endif
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    readings.average_DEMA.filter->SetAlpha(a1,a2);
    #endif
    telemetry_settings.flags.SendNow = true;
  }

  if(jtok = obj["SR04"].getObject()["ConversionMethod"])
  {
    readings.conversion_settings.flag_distance_conversion_method = jtok.getInt();
    ALOG_TST(PSTR("ConversionMethod %d"),readings.conversion_settings.flag_distance_conversion_method);
    
    telemetry_settings.flags.SendNow = true;
  }

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mSR04::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add(D_TYPE, sr04_type);
    
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    JBI->Object_Start("Filtered_EMA");
      JBI->Add("alpha", readings.average_EMA.alpha);
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    JBI->Object_Start("Filtered_DEMA");
      JBI->Add("alpha1", readings.average_DEMA.alpha1);
      JBI->Add("alpha2", readings.average_DEMA.alpha1);
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

  return JBI->End();
}


uint8_t mSR04::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add("Ping", readings.raw.ping_value);
    JBI->Add(D_DISTANCE "_cm", readings.raw.distance_cm);

    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    JBI->Object_Start("Filtered_EMA");
      JBI->Add(D_DISTANCE "_cm", readings.average_EMA.distance_cm);

      if (readings.average_EMA.filter)
      {
        // Serial.println(readings.average_EMA.filter->GetLowPass());
        // Serial.println(readings.average_EMA.filter->GetHighPass());
        // float low = readings.average_EMA.filter->GetLowPass();
        JBI->Add("GetLowPass", readings.average_EMA.filter->GetLowPass());
        JBI->Add("GetHighPass", readings.average_EMA.filter->GetHighPass());
      }
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA

    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    JBI->Object_Start("Filtered_DEMA");
      JBI->Add(D_DISTANCE "_cm", readings.average_DEMA.distance_cm);

      if (readings.average_DEMA.filter)
      {
        // Serial.println(readings.average_DEMA.filter->GetBandPass());
        // Serial.println(readings.average_DEMA.filter->GetBandStop());
        JBI->Add("GetBandPass", readings.average_DEMA.filter->GetBandPass());
        JBI->Add("GetBandStop", readings.average_DEMA.filter->GetBandStop());
      }
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    JBI->Object_Start("Temperature");
      JBI->Add("Enabled", readings.temp_adj.flag_enabled);
      JBI->Add("Ambient", readings.temp_adj.ambient_temperature);
    JBI->Object_End();

    JBI->Object_Start("Conversion");
      JBI->Add("speed_of_sound", readings.conversion_settings.speed_of_sound);
      JBI->Add("flag_distance_conversion_method", readings.conversion_settings.flag_distance_conversion_method);
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION

  return JBI->End();
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSR04::Telemetry_Init(){

  struct telemetry_handler<mSR04>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSR04::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSR04::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT

#endif
