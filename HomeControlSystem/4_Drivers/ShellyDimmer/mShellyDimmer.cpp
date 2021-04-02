#include "mShellyDimmer.h"

//  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Tasmota

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER


// /*********************************************************************************************/

void mShellyDimmer::init(void)
{
  
    ModuleSelected();
  // analogWriteFreq(25000);
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain

}

void mShellyDimmer::pre_init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }
    
  #ifdef SHELLY_DIMMER_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Shelly Dimmer Driver Starting Tx %d Rx %d"), Pin(GPIO_TXD), Pin(GPIO_RXD));
  #endif  // SHELLY_DIMMER_DEBUG

  Shd.buffer = (uint8_t *)malloc(SHD_BUFFER_SIZE);
  if (Shd.buffer != nullptr)
  {
    // ShdSerial = new TasmotaSerial(Pin(D_GPIO_SERIAL_RXD_ID), Pin(GPIO_TXD), 2, 0, SHD_BUFFER_SIZE);

    //check for defined hardware serial
    if((!pCONT_pins->PinUsed(GPIO_HWSERIAL0_RX_ID))&&(!pCONT_pins->PinUsed(GPIO_HWSERIAL0_TX_ID))){
      return;
    }

    ShdSerial = new TasmotaSerial(pCONT_pins->GetPin(GPIO_HWSERIAL0_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL0_TX_ID), 2, 0, SHD_BUFFER_SIZE);
    if (ShdSerial->begin(115200))
    {

      Shd.hardware_serial_active = true;

      if (ShdSerial->hardwareSerial())
        pCONT_sup->ClaimSerial();

      ShdSerial->flush();

      ResetToAppMode();
      bool got_version = SendVersion();
      AddLog_P(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Shelly Dimmer Co-processor Version v%u.%u"), Shd.dimmer.version_major, Shd.dimmer.version_minor);
      GetSettings();
      SaveSettings();

      SendSettings();
      SyncState();
    }
  }

}


int8_t mShellyDimmer::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:  
      Poll();    
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;
    case FUNC_SET_DEVICE_POWER:
      SetPower();
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case FUNC_EVENT_SET_POWER_ID:
      RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES
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
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT

  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker


#ifdef USE_MODULE_CORE_RULES

void mShellyDimmer::RulesEvent_Set_Power(){

  
      AddLog_P(LOG_LEVEL_TEST, PSTR("MATCHED RulesEvent_Set_Power"));

      uint8_t relay_index = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;

      uint8_t relay_state = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];

      uint8_t brightness_on_value = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[1];

      if(relay_state==2){
        if(Shd.req_brightness){
          Shd.req_brightness = 0;
        }else{
          Shd.req_brightness = map(brightness_on_value,0,100,0,1000);
        }
      }

      
    // Shd.req_brightness = map(relay_state,0,100,0,1000);
    SetBrightness();


          // pCONT_mry->ExecuteCommandPower(relay_index, relay_state, SRC_IGNORE);




}
#endif // USE_MODULE_CORE_RULES


uint8_t mShellyDimmer::ConstructJSON_Settings(uint8_t json_method){

  // Active rgbcct palette used as scene

  //ShdSerial.println("mShellyDimmer::ConstructJSON_Settings");

  char buffer[30];
  
  JsonBuilderI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JsonBuilderI->Add_P(PM_JSON_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JsonBuilderI->Add_P(PM_JSON_HUE, rgbcct_controller.getHue360());
    // JsonBuilderI->Add_P(PM_JSON_SAT, rgbcct_controller.getSat255());
    // JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JsonBuilderI->Add_P(PM_JSON_TIME, 1000);

    JBI->Add("serial_active",Shd.hardware_serial_active);

    JBI->Add("version_major",Shd.dimmer.version_major);
    JBI->Add("version_minor",Shd.dimmer.version_minor);
    JBI->Add("brightness",Shd.dimmer.brightness);
    JBI->Add("power",Shd.dimmer.power);
    JBI->Add("fade_rate",Shd.dimmer.fade_rate);



    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}

uint8_t mShellyDimmer::ConstructJSON_State(uint8_t json_method){
  
  JsonBuilderI->Start();  

    JBI->Add_P(PM_JSON_BRIGHTNESS, map(Shd.dimmer.brightness,0,1000,0,100));

  return JsonBuilderI->End();

}


////////////////////// START OF MQTT /////////////////////////

void mShellyDimmer::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Settings;

  
  mqtthandler_ptr = &mqtthandler_state_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_State;

//   mqtthandler_ptr = &mqtthandler_sensdebug_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Debug;

} //end "MQTTHandler_Init"


void mShellyDimmer::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
//   mqtthandler_scene_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mShellyDimmer::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  
} //end "MQTTHandler_Set_TelePeriod"


void mShellyDimmer::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID
    //, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mShellyDimmer>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod
    //, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_DRIVERS_SHELLY_DIMMER_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, 
    sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
    mqtt_handler_id
  );

}


/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

bool mShellyDimmer::SerialSend(const uint8_t data[], uint16_t len)
{
  int retries = 3;

#ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "Tx Packet:"));
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)data, len);
#endif  // SHELLY_DIMMER_DEBUG

  while (retries--)
  {
      ShdSerial->write(data, len);
      ShdSerial->flush();

      // wait for any response
      uint32_t snd_time = millis();
      while(abs(millis()-snd_time)<SHD_ACK_TIMEOUT)
      // while (TimePassedSince(snd_time) < SHD_ACK_TIMEOUT)
      {
          if (SerialInput())
              return true;

          delay(1);
      }

      // timeout
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "ShdSerial send timeout"));
  }
  return false;

}


bool mShellyDimmer::SendCmd(uint8_t cmd, uint8_t *payload, uint8_t len)
{
  uint8_t data[4 + 72 + 3]; // maximum payload for 0x30 packet is 72
  uint16_t chksm;
  uint8_t pos = 0;

  data[0] = SHD_START_BYTE;
  data[1] = Shd.counter++;
  data[2] = cmd;
  data[3] = len;

  pos += 4;

  if (payload)
  {
    memcpy(data + 4, payload, len);
    pos += len;
  }

  // calculate checksum from id and onwards
  chksm = checksum(data + 1, 3 + len);
  data[pos++] = chksm >> 8;
  data[pos++] = chksm & 0xff;
  data[pos++] = SHD_END_BYTE;

  return SerialSend(data, pos);
}


void mShellyDimmer::SetBrightness()
{
  // Payload format:
  // [0-1] Brightness (%) * 10

  uint8_t payload[SHD_SWITCH_SIZE];

  payload[0] = Shd.req_brightness & 0xff;
  payload[1] = Shd.req_brightness >> 8;

  SendCmd(SHD_SWITCH_CMD, payload, SHD_SWITCH_SIZE);
}


void mShellyDimmer::SetBrightnessFade()
{
  uint16_t delta = 0;
  if (Shd.req_brightness > Shd.dimmer.brightness)
    delta = (Shd.req_brightness - Shd.dimmer.brightness) * 0.8;
  else
      delta = (Shd.dimmer.brightness - Shd.req_brightness) * 0.8;

  // Payload format:
  // [0-1] Brightness (%) * 10
  // [2-3] Delta brightness (%) * 8
  // [4-5] 0?? ToDo(jamesturton): Find out what this word is!

  uint8_t payload[SHD_SWITCH_FADE_SIZE];

  payload[0] = Shd.req_brightness & 0xff;
  payload[1] = Shd.req_brightness >> 8;

  payload[2] = delta & 0xff;
  payload[3] = delta >> 8;

  payload[4] = 0;
  payload[5] = 0;

  SendCmd(SHD_SWITCH_FADE_CMD, payload, SHD_SWITCH_FADE_SIZE);
}


void mShellyDimmer::SendSettings()
{
  // as specified in STM32 assembly
  uint16_t fade_rate = Shd.req_fade_rate;
  if (fade_rate > 100)
      fade_rate = 100;

  // Payload format:
  // [0-1] Brightness (%) * 10
  // [2-3] Leading / trailing edge (1=leading, 2=trailing) ToDo(jamesturton): Are there any other values this can take?
  // [4-5] Fade rate (units unknown) ToDo(jamesturton): Find fade rate units
  // [6-7] Warm up brightness (%) * 10
  // [8-9] Warm up duration (ms)

  uint8_t payload[SHD_SETTINGS_SIZE];

  payload[0] = Shd.req_brightness & 0xff;
  payload[1] = Shd.req_brightness >> 8;

  payload[2] = Shd.leading_edge & 0xff;
  payload[3] = Shd.leading_edge >> 8;

  payload[4] = fade_rate & 0xff;
  payload[5] = fade_rate >> 8;

  payload[6] = Shd.warmup_brightness & 0xff;
  payload[7] = Shd.warmup_brightness >> 8;

  payload[8] = Shd.warmup_time & 0xff;
  payload[9] = Shd.warmup_time >> 8;

  SendCmd(SHD_SETTINGS_CMD, payload, SHD_SETTINGS_SIZE);
}

void mShellyDimmer::SendWarmup()
{
  // Payload format:
  // [0-1] Warm up brightness (%) * 10
  // [2-3] Warm up duration (ms)

  uint8_t payload[SHD_WARMUP_SIZE];

  payload[0] = Shd.warmup_brightness & 0xff;
  payload[1] = Shd.warmup_brightness >> 8;

  payload[2] = Shd.warmup_time & 0xff;
  payload[3] = Shd.warmup_time >> 8;

  SendCmd(SHD_WARMUP_CMD, payload, SHD_WARMUP_SIZE);
}


void mShellyDimmer::SendCalibration(uint16_t brightness, uint16_t func, uint16_t fade_rate)
{
  // Payload format:
  // ??? ToDo(jamesturton): Find calibration payload format!
  uint8_t payload[SHD_CALIBRATION_SIZE];

  memset(payload, 0, sizeof(payload));

  SendCmd(SHD_CALIBRATION1_CMD, payload, SHD_CALIBRATION_SIZE);
  SendCmd(SHD_CALIBRATION2_CMD, payload, SHD_CALIBRATION_SIZE);
}


bool mShellyDimmer::SyncState()
{
#ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "ShdSerial %p"), ShdSerial);
  AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set Brightness Want %d, Is %d"), Shd.req_brightness, Shd.dimmer.brightness);
  AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set Fade Want %d, Is %d"), Settings.light_speed, Shd.dimmer.fade_rate);
#endif  // SHELLY_DIMMER_DEBUG

  if (!ShdSerial)
      return false;

#ifdef SHELLY_HW_DIMMING
  // TODO(jamesturton): HW dimming seems to conflict with SW dimming. See how
  // we can disbale SW dimming when using HW dimming.
  if (Settings.light_speed != Shd.dimmer.fade_rate)
  {
      ShdSetBrightnessFade();
      ShdDebugState();
  }
  else
#endif // SHELLY_HW_DIMMING
  if (Shd.req_brightness != Shd.dimmer.brightness)
  {
    SetBrightness();
    DebugState();
  }

  return true;
}

void mShellyDimmer::DebugState()
{
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "MCU v%d.%d, Brightness:%d(%d%%), Power:%d, Fade:%d"),
                            Shd.dimmer.version_major, Shd.dimmer.version_minor,
                            Shd.dimmer.brightness,
                            changeUIntScale(Shd.dimmer.brightness, 0, 1000, 0, 100),
                            Shd.dimmer.power,
                            Shd.dimmer.fade_rate);
#endif  // SHELLY_DIMMER_DEBUG
}

bool mShellyDimmer::PacketProcess(void)
{
  uint8_t pos = 0;
  uint8_t id, cmd, len;
  bool ret = false;

  if (Shd.buffer[pos++] != SHD_START_BYTE)
    return false;

  id = Shd.buffer[pos++];
  cmd = Shd.buffer[pos++];
  len = Shd.buffer[pos++];

  switch (cmd)
  {
    case SHD_POLL_CMD:{
      // 1 when returning fade_rate, 0 when returning wattage, brightness?
      uint16_t unknown_0 = Shd.buffer[pos + 1] << 8 |
              Shd.buffer[pos + 0];

      uint16_t brightness = Shd.buffer[pos + 3] << 8 |
              Shd.buffer[pos + 2];

      uint32_t wattage_raw = Shd.buffer[pos + 7] << 24 |
              Shd.buffer[pos + 6] << 16 |
              Shd.buffer[pos + 5] << 8 |
              Shd.buffer[pos + 4];

      uint32_t voltage_raw = Shd.buffer[pos + 11] << 24 |
              Shd.buffer[pos + 10] << 16 |
              Shd.buffer[pos + 9] << 8 |
              Shd.buffer[pos + 8];

      uint32_t current_raw = Shd.buffer[pos + 15] << 24 |
              Shd.buffer[pos + 14] << 16 |
              Shd.buffer[pos + 13] << 8 |
              Shd.buffer[pos + 12];

      uint32_t fade_rate = Shd.buffer[pos + 16];

      float wattage = 0;
      if (wattage_raw > 0)
          wattage = 880373 / (float)wattage_raw;

      float voltage = 0;
      if (voltage_raw > 0)
          voltage = 347800 / (float)voltage_raw;

      float current = 0;
      if (current_raw > 0)
          current = 1448 / (float)current_raw;

// #ifdef USE_ENERGY_SENSOR
//                 Energy.active_power[0] = wattage;
//                 Energy.voltage[0] = voltage;
//                 Energy.current[0] = current;
//                 Energy.apparent_power[0] = voltage * current;
//                 if ((voltage * current) > wattage)
//                     Energy.reactive_power[0] = sqrt((voltage * current) * (voltage * current) - wattage * wattage);
//                 else
//                     Energy.reactive_power[0] = 0;
//                 if (wattage > (voltage * current))
//                     Energy.power_factor[0] = 1;
//                 else if ((voltage * current) == 0)
//                     Energy.power_factor[0] = 0;
//                 else
//                     Energy.power_factor[0] = wattage / (voltage * current);

//                 if (Shd.last_power_check > 10 && Energy.active_power[0] > 0)
//                 {
//                     float kWhused = (float)Energy.active_power[0] * (Rtc.utc_time - Shd.last_power_check) / 36;
// #ifdef SHELLY_DIMMER_DEBUG
//                     AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Adding %i mWh to todays usage from %lu to %lu"), (int)(kWhused * 10), Shd.last_power_check, Rtc.utc_time);
// #endif  // USE_ENERGY_SENSOR
//                     Energy.kWhtoday += kWhused;
//                     EnergyUpdateToday();
//                 }
//                 Shd.last_power_check = Rtc.utc_time;
// #endif  // USE_ENERGY_SENSOR

#ifdef SHELLY_DIMMER_DEBUG
          AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "ShdPacketProcess: Brightness:%d Power:%lu Voltage:%lu Current:%lu Fade:%d"), brightness, wattage_raw, voltage_raw, current_raw, fade_rate);
#endif  // SHELLY_DIMMER_DEBUG
          Shd.dimmer.brightness = brightness;
          Shd.dimmer.power = wattage_raw;
          Shd.dimmer.fade_rate = fade_rate;
      }
      break;
  case SHD_VERSION_CMD:
      {
#ifdef SHELLY_DIMMER_DEBUG
          AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "ShdPacketProcess: Version: %u.%u"), Shd.buffer[pos + 1], Shd.buffer[pos]);
#endif  // SHELLY_DIMMER_DEBUG
          Shd.dimmer.version_minor = Shd.buffer[pos];
          Shd.dimmer.version_major = Shd.buffer[pos + 1];
      }
      break;
  case SHD_SWITCH_CMD:
  case SHD_SWITCH_FADE_CMD:
  case SHD_SETTINGS_CMD:
  case SHD_WARMUP_CMD:
  case SHD_CALIBRATION1_CMD:
  case SHD_CALIBRATION2_CMD:
      {
          ret = (Shd.buffer[pos] == 0x01);
      }
      break;
  }

  return ret;
}

void mShellyDimmer::ResetToAppMode()
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Request co-processor reset in app mode"));
#endif  // SHELLY_DIMMER_DEBUG

  // pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
  // digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);
  // pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
  // digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), LOW);

  pinMode(PIN_GPIO_SHELLY_DIMMER_RST_INV, OUTPUT);
  digitalWrite(PIN_GPIO_SHELLY_DIMMER_RST_INV, LOW);

  pinMode(PIN_GPIO_SHELLY_DIMMER_BOOT0, OUTPUT);
  digitalWrite(PIN_GPIO_SHELLY_DIMMER_BOOT0, LOW);

  delay(50);

    // clear in the receive buffer
    while (Serial.available())
        Serial.read();


  digitalWrite(PIN_GPIO_SHELLY_DIMMER_RST_INV, HIGH); // pull out of reset
  delay(50); // wait 50ms fot the co-processor to come online
}

void mShellyDimmer::Poll(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Poll"));
  #endif  // SHELLY_DIMMER_DEBUG

  if(!ShdSerial)
    return;

  SendCmd(SHD_POLL_CMD, 0, 0);
  SyncState();
}

bool mShellyDimmer::SendVersion(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Sending version command"));
  #endif  // SHELLY_DIMMER_DEBUG
  return SendCmd(SHD_VERSION_CMD, 0, 0);
}

void mShellyDimmer::GetSettings(void)
{
  char parameters[32];
  // Shd.req_brightness      = 0;
  Shd.leading_edge        = 0;
  // Shd.req_fade_rate       = 0;
  Shd.warmup_brightness   = 0;
  Shd.warmup_time         = 0;
//     if (strstr(SettingsText(SET_SHD_PARAM), ",") != nullptr)
//     {
// #ifdef SHELLY_DIMMER_DEBUG
//         AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Loading params: %s"), SettingsText(SET_SHD_PARAM));
// #endif  // SHELLY_DIMMER_DEBUG
//         // Shd.req_brightness      = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 1));
//         Shd.leading_edge        = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 2));
//         // Shd.req_fade_rate       = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 3));
//         Shd.warmup_brightness   = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 4));
//         Shd.warmup_time         = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 5));
//     }
}

void mShellyDimmer::SaveSettings(void)
{
  char parameters[32];
  snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d"),
              Shd.req_brightness, Shd.leading_edge, Shd.req_fade_rate, Shd.warmup_brightness, Shd.warmup_time);
  // SettingsUpdateText(SET_SHD_PARAM, parameters);
}


bool mShellyDimmer::SerialInput(void)
{
  while (ShdSerial->available())
  {
    yield();
    uint8_t serial_in_byte = ShdSerial->read();
    Shd.buffer[Shd.byte_counter] = serial_in_byte;

    int check = check_byte();

    if (check > 1)
    {
      // finished
#ifdef SHELLY_DIMMER_DEBUG
      Shd.byte_counter++;
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "Rx Packet:"));
      AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Shd.buffer, Shd.byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
      Shd.byte_counter = 0;

      PacketProcess();

      return true;
    }
    else if (check == 0)
    {
      // wrong data
#ifdef SHELLY_DIMMER_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Byte %i of received data frame is invalid. Rx Packet:"), Shd.byte_counter);
      Shd.byte_counter++;
      AddLogBuffer(LOG_LEVEL_DEBUG_MORE, Shd.buffer, Shd.byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
      Shd.byte_counter = 0;
    }
    else
    {
        Shd.byte_counter++;
    }

  }
  return false;
}

/*********************************************************************************************\
 * Helper Functions
\*********************************************************************************************/

uint16_t mShellyDimmer::checksum(uint8_t *buf, int len)
{
    uint16_t chksm = 0;
    for (uint8_t i = 0; i < len; i++)
        chksm += buf[i];
    return chksm;
}

int mShellyDimmer::check_byte()
{
    uint8_t index = Shd.byte_counter;
    uint8_t byte = Shd.buffer[index];

    if (index == 0)
        return byte == SHD_START_BYTE;

    if (index < 4)
        return 1;

    uint8_t data_length = Shd.buffer[3];
    if ((4 + data_length + 3) > SHD_BUFFER_SIZE)
        return 0;

    if (index < 4 + data_length + 1)
        return 1;

    if (index == 4 + data_length + 1)
    {
        uint16_t chksm = (Shd.buffer[index - 1] << 8 | Shd.buffer[index]);
        uint16_t chksm_calc = checksum(&Shd.buffer[1], 3 + data_length);
        if (chksm != chksm_calc)
        {
#ifdef SHELLY_DIMMER_DEBUG
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Checksum: %x calculated: %x"), chksm, chksm_calc);
#endif  // SHELLY_DIMMER_DEBUG
            return 0;
        }

        return 1;
    }

    if (index == 4 + data_length + 2 && byte == SHD_END_BYTE)
        return index;

    return 0;
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

bool mShellyDimmer::ModuleSelected(void) {
  // if (PinUsed(GPIO_SHELLY_DIMMER_BOOT0) && PinUsed(GPIO_SHELLY_DIMMER_RST_INV)) {
    // TasmotaGlobal.devices_present++;
    // TasmotaGlobal.light_type = LT_SERIAL1;

    // Shd.present = true;
    settings.fEnableModule = true;
  // }
  return settings.fEnableModule;//Shd.present;
}

bool mShellyDimmer::SetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "SetChannels:"));
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t *)XdrvMailbox.data, XdrvMailbox.data_len);
#endif  // SHELLY_DIMMER_DEBUG

    // uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    // // Use dimmer_hw_min and dimmer_hw_max to constrain our values if the light should be on
    // if (brightness > 0)
    //     brightness = changeUIntScale(brightness, 0, 255, Settings.dimmer_hw_min * 10, Settings.dimmer_hw_max * 10);
    // Shd.req_brightness = brightness;

    // ShdDebugState();

    return SyncState();
}

bool mShellyDimmer::SetPower(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Set Power, Power 0x%02x"), XdrvMailbox.index);
  #endif  // SHELLY_DIMMER_DEBUG

  Shd.req_on = 1;//(bool)XdrvMailbox.index;
  return SyncState();
}



/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef SHELLY_CMDS

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // Prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdLeadingEdge(void)
{
    if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1)
    {
        Shd.leading_edge = 2 - XdrvMailbox.payload;
        Settings.shd_leading_edge = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        if (Shd.leading_edge == 1)
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to trailing edge"));
        else
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to leading edge"));
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_leading_edge);
}

void CmndShdWarmupBrightness(void)
{
    if ((10 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 100))
    {
        Shd.warmup_brightness = XdrvMailbox.payload * 10;
        Settings.shd_warmup_brightness = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup brightness to %d%%"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_brightness);
}

void CmndShdWarmupTime(void)
{
    if ((20 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 200))
    {
        Shd.warmup_time = XdrvMailbox.payload;
        Settings.shd_warmup_time = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup time to %dms"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_time);
}

#endif // SHELLY_CMDS

/*********************************************************************************************\
 * SHD firmware update Functions
\*********************************************************************************************/

#ifdef SHELLY_FW_UPGRADE

void ShdResetToDFUMode()
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Request co-processor reset in dfu mode"));
#endif  // SHELLY_DIMMER_DEBUG

    pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);

    pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), HIGH);

    delay(50);

    // clear in the receive buffer
    while (ShdSerial.available())
        ShdSerial.read();

    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), HIGH); // pull out of reset
    delay(50); // wait 50ms fot the co-processor to come online
}

bool ShdUpdateFirmware(uint8_t* data, uint32_t size)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Update firmware"));
#endif  // SHELLY_DIMMER_DEBUG

    bool ret = true;
    stm32_t *stm = stm32_init(&ShdSerial, STREAM_SERIAL, 1);
    if (stm)
    {
        off_t   offset = 0;
        uint8_t   buffer[256];
        unsigned int  len;
        const uint8_t *p_st = data;
        uint32_t  addr, start, end;
        stm32_err_t s_err;

#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "STM32 erase memory"));
#endif  // SHELLY_DIMMER_DEBUG

        stm32_erase_memory(stm, 0, STM32_MASS_ERASE);

        addr = stm->dev->fl_start;
        end = addr + size;
        while(addr < end && offset < size)
        {
            uint32_t left = end - addr;
            len   = sizeof(buffer) > left ? left : sizeof(buffer);
            len   = len > size - offset ? size - offset : len;

            if (len == 0)
            {
                break;
            }

            memcpy(buffer, p_st, sizeof(buffer));  // We need 4-byte bounadry flash access
            p_st += sizeof(buffer);

            s_err = stm32_write_memory(stm, addr, buffer, len);
            if (s_err != STM32_ERR_OK)
            {
                ret = false;
                break;
            }

            addr  += len;
            offset  += len;
        }
        stm32_close(stm);
    }
    return ret;
}

bool ShdPresent(void) {
  return Shd.present;
}

uint32_t ShdFlash(uint8_t* data, size_t size) {
#ifdef SHELLY_DIMMER_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Updating firmware v%u.%u with %u bytes"), Shd.dimmer.version_major, Shd.dimmer.version_minor, size);
#endif  // SHELLY_DIMMER_DEBUG

  ShdSerial.end();
  ShdSerial.begin(115200, SERIAL_8E1);
  ShdResetToDFUMode();
  bool error = !ShdUpdateFirmware(data, size);  // Allow flash access without ESP.flashRead
  ShdSerial.end();
  ShdResetToAppMode();
  ShdSerial.begin(115200, SERIAL_8N1);
  ShdSendVersion();

  return error;
}

#endif // SHELLY_FW_UPGRADE



#endif
