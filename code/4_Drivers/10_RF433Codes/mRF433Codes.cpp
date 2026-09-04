#include "mRF433Codes.h"

#ifdef USE_MODULE_DRIVERS_RF433_CODES

int8_t mRF433Codes::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function)
  {
    /************
     * INIT SECTION
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function)
  {
    /************
     * PERIODIC SECTION
    *******************/
    case TASK_EVERY_50_MSECOND:
      ReceiveCheck();
    break;

    case TASK_UPDATE_OTA_BEFORE_ON_START:
      ALOG_INF(PSTR("disableReceive"));
      if(mySwitch != nullptr)
      {
        mySwitch->disableReceive();
      }
    break;

    case TASK_EVERY_FIVE_SECOND:
    break;

    /************
     * COMMANDS SECTION
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    /************
     * TELEMETRY SECTION
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      #ifdef USE_MODULE_NETWORK_MQTT
      Telemetry_Init();
      #endif
    break;

    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      #ifdef USE_MODULE_NETWORK_MQTT
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
      #endif
    break;

    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      #ifdef USE_MODULE_NETWORK_MQTT
      tkr_tele->Telemetry_Rate(telemetry_list);
      #endif
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

  return TASKER_RESULT__UNKNOWN_ID;

}


void mRF433Codes::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mRF433Codes::Init(void)
{

  if(tkr_pins->PinUsed(GPIO_RF_433MHZ_RX))
  {

    if(tkr_set->Settings.rf_duplicate_time < 10)
    {
      tkr_set->Settings.rf_duplicate_time = RF_TIME_AVOID_DUPLICATE;
    }

    pinMode(tkr_pins->GetPin(GPIO_RF_433MHZ_RX), INPUT);

    if(mySwitch == nullptr)
    {
      mySwitch = new RCSwitch();
    }

    ALOG_INF(PSTR("mRF433Codes RX: %d"), tkr_pins->GetPin(GPIO_RF_433MHZ_RX));

    mySwitch->enableReceive(tkr_pins->GetPin(GPIO_RF_433MHZ_RX));

    tkr_set->Settings.rf_protocol_mask = (1ULL << 1) - 1;

    #ifndef ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START
    mySwitch->setReceiveProtocolMask(tkr_set->Settings.rf_protocol_mask);
    #endif

    module_state.mode = ModuleStatus::Running;
  }

  if(tkr_pins->PinUsed(GPIO_RF_433MHZ_TX))
  {

    if(mySwitch == nullptr)
    {
      mySwitch = new RCSwitch();
    }

    mySwitch->enableTransmit(tkr_pins->GetPin(GPIO_RF_433MHZ_TX));

    ALOG_INF(PSTR("mRF433Codes TX: %d"), tkr_pins->GetPin(GPIO_RF_433MHZ_TX));

    module_state.mode = ModuleStatus::Running;
  }

}


void mRF433Codes::ReceiveCheck(void)
{

  if(mySwitch == nullptr){ return; }
  if(tkr_time->uptime_seconds_nonreset < 10){ return; }

  if(mySwitch->available())
  {

    unsigned long long data = mySwitch->getReceivedValue();
    unsigned int bits = mySwitch->getReceivedBitlength();
    int protocol = mySwitch->getReceivedProtocol();
    int delay = mySwitch->getReceivedDelay();

    ALOG_INF(PSTR("RFR: Data 0x%llX, Bits %d, Protocol %d, Delay %d"), data, bits, protocol, delay);

    uint32_t now = millis();

    if((now - rx_pkt.received_time_millis > tkr_set->Settings.rf_duplicate_time) && (data > 0))
    {
      rx_pkt.received_time_millis = now;

      rx_pkt.data = (uint32_t)data;
      rx_pkt.bit_length = bits;
      rx_pkt.protocol = protocol;
      rx_pkt.delay = delay;
      rx_pkt.received_utc_time = tkr_time->UtcTime();

      #ifdef USE_MODULE_NETWORK_MQTT
      telemetry_state_ifchanged.flags.SendNow = true;
      #endif

      ALOG_TST(PSTR("RFR: Data 0x%llX, Bits %d, Protocol %d, Delay %d"), data, bits, protocol, delay);
    }

    mySwitch->resetAvailable();
  }
}


/******************************************************************************************************************
 * RF Transmission
*******************************************************************************************************************/

bool mRF433Codes::ParseTimingData(JsonParserToken data_token, std::vector<uint16_t>& timings)
{

  timings.clear();

  if(!data_token || !data_token.isArray())
  {
    ALOG_ERR(PSTR("RF433: Data must be an array"));
    return false;
  }

  JsonParserArray data_array = data_token.getArray();

  if(data_array.size() == 0)
  {
    ALOG_ERR(PSTR("RF433: Data array is empty"));
    return false;
  }

  timings.reserve(data_array.size());

  for(auto value_token : data_array)
  {
    uint32_t value = value_token.getUInt();

    if((value == 0) || (value > UINT16_MAX))
    {
      ALOG_ERR(PSTR("RF433: Invalid timing value %u"), value);
      timings.clear();
      return false;
    }

    timings.push_back((uint16_t)value);
  }

  return true;
}


bool mRF433Codes::SendRawCode(JsonParserObject code_obj, bool use_multiples)
{

  if(mySwitch == nullptr)
  {
    ALOG_ERR(PSTR("RF433: Transmitter is not initialised"));
    return false;
  }

  std::vector<uint16_t> timings;

  if(!ParseTimingData(code_obj["Data"], timings)){
    return false;
  }

  uint8_t start_level = HIGH;
  uint8_t repeats = 1;
  uint32_t pre_frame_low_us = 0;

  JsonParserToken jtok = 0;

  if(jtok = code_obj["StartLevel"])
  {
    start_level = jtok.getUInt() ? HIGH : LOW;
  }

  if(jtok = code_obj["Repeat"])
  {
    repeats = jtok.getUInt();
    if(repeats == 0){ repeats = 1; }
  }

  if(jtok = code_obj["PreFrameLowUs"])
  {
    pre_frame_low_us = jtok.getUInt();
  }

  mySwitch->setRepeatTransmit(repeats);

  if(use_multiples)
  {
    uint16_t base_pulse_us = 0;

    if(jtok = code_obj["BasePulseUs"])
    {
      base_pulse_us = jtok.getUInt();
    }

    if(base_pulse_us == 0)
    {
      ALOG_ERR(PSTR("RF433: RawMultiples requires BasePulseUs"));
      return false;
    }

    ALOG_INF(
      PSTR("RF433: Data[17..23]=%u,%u,%u,%u,%u,%u,%u"),      
      timings[17],
      timings[18],
      timings[19],
      timings[20],
      timings[21],
      timings[22],
      timings[23]
    );

    mySwitch->sendRawMultiples(timings.data(),timings.size(),base_pulse_us,start_level,pre_frame_low_us);
  }
  else
  {
    mySwitch->sendRaw(timings.data(),timings.size(),start_level,pre_frame_low_us);
  }

  return true;
}


bool mRF433Codes::SendProtocolCode(JsonParserObject code_obj)
{

  if(mySwitch == nullptr)
  {
    ALOG_ERR(PSTR("RF433: Transmitter is not initialised"));
    return false;
  }

  JsonParserToken jtok = 0;

  uint64_t data = 0;
  uint16_t bits = 0;
  uint8_t protocol = 1;
  uint8_t repeats = 10;
  uint16_t pulse_us = 0;

  if(jtok = code_obj["Data"])
  {
    if(jtok.isStr())
    {
      data = strtoull(jtok.getStr(), nullptr, 0);
    }
    else
    {
      data = jtok.getULong();
    }
  }
  else
  {
    ALOG_ERR(PSTR("RF433: Protocol requires Data"));
    return false;
  }

  if(jtok = code_obj["Bits"])
  {
    bits = jtok.getUInt();
  }

  if(bits == 0)
  {
    ALOG_ERR(PSTR("RF433: Protocol requires Bits"));
    return false;
  }

  if(jtok = code_obj["Protocol"])
  {
    protocol = jtok.getUInt();
    if(protocol == 0){ protocol = 1; }
  }

  if(jtok = code_obj["Repeat"])
  {
    repeats = jtok.getUInt();
    if(repeats == 0){ repeats = 1; }
  }

  if(jtok = code_obj["PulseUs"])
  {
    pulse_us = jtok.getUInt();
  }
  else if(jtok = code_obj["Pulse"])
  {
    pulse_us = jtok.getUInt();
  }

  if(pulse_us)
  {
    mySwitch->setProtocol(protocol, pulse_us);
  }
  else
  {
    mySwitch->setProtocol(protocol);
  }

  mySwitch->setRepeatTransmit(repeats);
  mySwitch->send(data, bits);

  return true;
}


bool mRF433Codes::SendCodeObject(JsonParserObject code_obj, const char* source_name)
{

  if(!code_obj)
  {
    ALOG_ERR(PSTR("RF433: Invalid code object"));
    return false;
  }

  JsonParserToken type_token = code_obj["Type"];

  if(!type_token || !type_token.isStr())
  {
    ALOG_ERR(PSTR("RF433: Code has no Type"));
    return false;
  }

  const char* type = type_token.getStr();
  bool result = false;

  if(strcasecmp(type, "RawMultiples") == 0)
  {
    result = SendRawCode(code_obj, true);
  }
  else if(strcasecmp(type, "Raw") == 0)
  {
    result = SendRawCode(code_obj, false);
  }
  else if(strcasecmp(type, "Protocol") == 0)
  {
    result = SendProtocolCode(code_obj);
  }
  else
  {
    ALOG_ERR(PSTR("RF433: Unsupported code Type \"%s\""), type);
    return false;
  }

  if(result)
  {
    if(source_name)
    {
      ALOG_INF(PSTR("RF433: Sent \"%s\" using %s"), source_name, type);
    }
    else
    {
      ALOG_INF(PSTR("RF433: Sent direct code using %s"), type);
    }
  }

  return result;
}


bool mRF433Codes::SendCodeByName(const char* code_name)
{

  if(code_name == nullptr || code_name[0] == '\0')
  {
    ALOG_ERR(PSTR("RF433: CodeName is empty"));
    return false;
  }

  #ifndef USE_MODULE_CORE_FILESYSTEM
    ALOG_ERR(PSTR("RF433: CodeName lookup requires filesystem"));
    return false;
  #else

  if(tkr_mfile == nullptr || !tkr_mfile->IsMounted())
  {
    ALOG_ERR(PSTR("RF433: Filesystem is not mounted"));
    return false;
  }

  if(!tkr_mfile->FileExists(RF433_CODES_FILE_PATH))
  {
    ALOG_ERR(PSTR("RF433: File not found: %s"), RF433_CODES_FILE_PATH);
    return false;
  }

  String json = tkr_mfile->LoadString(RF433_CODES_FILE_PATH);

  if(json.length() == 0)
  {
    ALOG_ERR(PSTR("RF433: File is empty: %s"), RF433_CODES_FILE_PATH);
    return false;
  }

  char* parsing_buffer = new char[json.length() + 1];

  if(parsing_buffer == nullptr)
  {
    ALOG_ERR(PSTR("RF433: Unable to allocate JSON parsing buffer"));
    return false;
  }

  memcpy(parsing_buffer, json.c_str(), json.length() + 1);

  JsonParser parser(parsing_buffer);
  JsonParserObject root_obj = parser.getRootObject();

  if(!root_obj)
  {
    ALOG_ERR(PSTR("RF433: Invalid JSON in %s"), RF433_CODES_FILE_PATH);
    delete[] parsing_buffer;
    return false;
  }

  JsonParserObject codes_obj = root_obj["Codes"].getObject();

  if(!codes_obj)
  {
    ALOG_ERR(PSTR("RF433: Missing Codes object in %s"), RF433_CODES_FILE_PATH);
    delete[] parsing_buffer;
    return false;
  }

  JsonParserObject code_obj = codes_obj[code_name].getObject();

  if(!code_obj)
  {
    ALOG_ERR(PSTR("RF433: CodeName \"%s\" not found"), code_name);
    delete[] parsing_buffer;
    return false;
  }

  bool result = SendCodeObject(code_obj, code_name);

  delete[] parsing_buffer;

  return result;

  #endif
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mRF433Codes::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0;

  #ifdef ENABLE_FEATURE_DRIVERS__RF433_TRANSMIT_BITBANG_TEST
  if(jtok = obj["TransmitTest"])
  {
    TransmitTest();
  }
  #endif


  /**
   * File lookup:
   * {"rf433":{"CodeName":"UP"}}
   * {"rf433":{"CodeName":"Bedroom-UP"}}
   *
   * Direct RawMultiples:
   * {"rf433":{"Type":"RawMultiples","BasePulseUs":333,"StartLevel":1,"Repeat":5,"Data":[15,7,5,2]}}
   *
   * Direct Raw:
   * {"rf433":{"Type":"Raw","StartLevel":1,"Repeat":5,"Data":[4995,2331,1665,666]}}
   *
   * Direct protocol:
   * {"rf433":{"Type":"Protocol","Protocol":36,"Bits":64,"Repeat":5,"Data":"0x123456789ABCDEF0"}}
   */
  if(jtok = obj[D_MODULE_DRIVERS__RF433_CODES_CTR])
  {
    if(jtok.isObject())
    {
      JsonParserObject rf_obj = jtok.getObject();
      JsonParserToken code_name_token = rf_obj["CodeName"];

      if(code_name_token && code_name_token.isStr())
      {
        if(SendCodeByName(code_name_token.getStr()))
        {
          data_buffer.isserviced++;
        }
      }
      else
      {
        if(SendCodeObject(rf_obj))
        {
          data_buffer.isserviced++;
        }
      }
    }
  }


  if(jtok = obj["RfMask"])
  {
    if(jtok.isNum() && mySwitch != nullptr)
    {
      mySwitch->setReceiveProtocolMask(jtok.getULong());

      #ifdef USE_MODULE_NETWORK_MQTT
      telemetry_settings.flags.SendNow = true;
      #endif

      ALOG_TST(PSTR("RfMask = %llu / %llu"), jtok.getULong(), mySwitch->getReceiveProtocolMask());
    }
  }


  if(jtok = obj["RfSend"])
  {
    if(jtok.isNum() && mySwitch != nullptr)
    {
      mySwitch->send(jtok.getULong(), 24);
      ALOG_TST(PSTR("RfSend = %llu / 24"), jtok.getULong());
    }
  }

}


#ifdef ENABLE_FEATURE_DRIVERS__RF433_TRANSMIT_BITBANG_TEST
/***
 * Keeping this scratch area to allow future manual timing tests when creating possible protocols.
 */
void mRF433Codes::TransmitTest()
{
  // static constexpr uint8_t TX_PIN = 22;

  // /**
  //  * ComfyShade / ZN-115T
  //  *
  //  * One captured DOWN frame, normalized to T = 333 us.
  //  * Array starts HIGH and alternates HIGH/LOW.
  //  */
  // static const uint16_t down_command_raw[] = {

  //   4995,2331,1665,666,333,1332,666,2331,666,333,
  //   333,1332,333,333,666,666,999,1332,666,333,
  //   333,2331,333,3663,333,999,333,999,333,1998,
  //   666,1998,666,333,666,1665,666,1998,666,333,
  //   333,1332,999,666,333,666,666,333,666,333

  // };

  // pinMode(TX_PIN, OUTPUT);

  // digitalWrite(TX_PIN, LOW);
  // delayMicroseconds(5000);

  // for(uint8_t repeat = 0; repeat < 5; repeat++)
  // {
  //   for(uint16_t i = 0; i < ARRAY_SIZE(down_command_raw); i++)
  //   {
  //     digitalWrite(TX_PIN, (i & 1) ? LOW : HIGH);
  //     delayMicroseconds(down_command_raw[i]);
  //   }

  //   digitalWrite(TX_PIN, LOW);
  // }

  // digitalWrite(TX_PIN, LOW);

  /*********************************************
   * Test 2a: Comfy Blinds UP WORKING
   *********************************************/
  // static constexpr uint8_t TX_PIN = 22;

  // static const uint16_t up_command[] = {
  //     15,7,5,2,1,4,2,7,2,1,
  // 1,4,1,1,2,2,3,5,2,8,
  // 1,8,2,5,1,3,1,6,2,6,
  // 2,1,2,5,2,6,2,1,1,4,
  // 3,1,1,1,1,2,1,2,1,1
  // };

  // pinMode(TX_PIN,OUTPUT);
  // digitalWrite(TX_PIN,LOW);
  // delayMicroseconds(5000);

  // for(uint8_t repeat=0; repeat<10; repeat++)
  // {
  //   for(uint16_t i=0; i<ARRAY_SIZE(up_command); i++)
  //   {
  //     digitalWrite(TX_PIN,(i & 1) ? LOW : HIGH);
  //     delayMicroseconds((uint32_t)up_command[i] * 333);
  //   }
  // }

  // digitalWrite(TX_PIN,LOW);

  
  /*********************************************
   * Test 2b: Comfy Blinds UP WORKING
   *********************************************/
  static constexpr uint8_t TX_PIN = 22;

static const uint16_t stop_command[] = {
  15,7,5,2,1,4,2,7,2,1,
  1,4,1,1,2,2,3,4,1,1,
  1,8,1,9,1,1,1,3,1,3,
  1,6,2,6,2,1,2,5,2,6,
  2,1,1,4,3,4,1,4,1,1
};

pinMode(TX_PIN,OUTPUT);
digitalWrite(TX_PIN,LOW);
delayMicroseconds(5000);

for(uint8_t repeat=0; repeat<10; repeat++)
{
  for(uint16_t i=0; i<ARRAY_SIZE(stop_command); i++)
  {
    digitalWrite(TX_PIN,(i & 1) ? LOW : HIGH);
    delayMicroseconds((uint32_t)stop_command[i] * 333);
  }
}

digitalWrite(TX_PIN,LOW);
}
#endif


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mRF433Codes::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  if(mySwitch != nullptr)
  {
    JBI->Add("RfMask", mySwitch->getReceiveProtocolMask());
  }

  return JBI->End();

}


uint8_t mRF433Codes::ConstructJSON_State(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  JBI->Object_Start(D_RFRECEIVED);

    JBI->Add(D_DATA, rx_pkt.data);
    JBI->Add(D_RF_BITS, rx_pkt.bit_length);
    JBI->Add(D_RF_PROTOCOL, rx_pkt.protocol);
    JBI->Add(D_RF_PULSE, rx_pkt.delay);
    JBI->Add(D_MILLIS, rx_pkt.received_time_millis);
    JBI->Add(D_TIME, tkr_time->GetTime().c_str());

  JBI->Object_End();

  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mRF433Codes::Telemetry_Init()
{

  struct telemetry_handler<mRF433Codes>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 120;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRF433Codes::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRF433Codes::ConstructJSON_State;
  telemetry_list.push_back(ptr);

}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_DRIVERS_RF433_CODES