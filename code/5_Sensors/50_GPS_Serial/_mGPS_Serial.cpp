
#include "_mGPS_Serial.h"

#ifdef USE_MODULE_SENSORS_GPS_SERIAL

int8_t mGPS_Serial::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
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
    case TASK_LOOP:
      Handle__Loop_Input_Stream();
    break;
    case TASK_EVERY_SECOND:
      Splash_Packet_Metrics();
      ApplyPacketConfig_Pending();
      ApplyGPSBaud_Pending();
    break;
    case TASK_EVERY_MINUTE:
    
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
    /************
     * WEBUI SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif
  }

  return 1;
  
} // END Tasker



void mGPS_Serial::Pre_Init()
{

  module_state.mode = ModuleStatus::Initialising;

}


/**
 * @brief 
 * */
void mGPS_Serial::Init(void)
{
  
  memset(&fix_parsing, 0, sizeof(fix_parsing));
  memset(&fix_valid,   0, sizeof(fix_valid));

  stats.last_message_received_time = 0;
  stats.last_valid_message_received_time = 0;
  stats.packets_received = 0;
  stats.bytes_received = 0;

  rt.valid_fix_timeout_ms = 3000;
  
  Init_BeginStream();

  Set_Default_PacketConfig();

  module_state.mode = ModuleStatus::Running;

}


void mGPS_Serial::Set_Default_PacketConfig(void)
{

  packet_config.ubx_nav[8].desired_enabled = true; // UBX_NAV_VELNED
  packet_config.ubx_nav[9].desired_enabled = true; // UBX_NAV_TIMEGPS
  packet_config.ubx_nav[10].desired_enabled = true; // UBX_NAV_TIMEUTC

  packet_config.nmea[5].desired_enabled = true; // RMC

  ApplyPacketConfig_Pending(true);

}



/**
 * @brief Apply GPS packet configuration to the connected module.
 *
 * This function compares the locally tracked packet configuration against the
 * desired state and sends the required commands to the GPS module.
 *
 * Normal mode:
 *   - Only entries where enabled != desired_enabled are processed.
 *   - This is the usual runtime behaviour after the user changes checkboxes
 *     in the web UI and presses Send.
 *
 * Force/update-all mode:
 *   - Every tracked packet entry is processed, regardless of whether the local
 *     enabled flag already matches desired_enabled.
 *   - This is intended for one-shot use such as boot-time initialisation or
 *     after a power cycle, where the GPS module may not match the cached local
 *     state and we want to push a full known configuration again.
 *   - In this mode, packet_config.pending_apply is NOT used to decide whether
 *     the function runs.
 *
 * Result handling:
 *   - If a command succeeds, enabled is updated to match desired_enabled.
 *   - If a command fails, the entry remains out of sync and pending_apply is
 *     left true so the caller may retry later.
 *
 * Notes:
 *   - UBX NAV messages use enable_msg / disable_msg.
 *   - NMEA messages use ublox::configNMEA.
 *   - update_all=true does not require packet_config.pending_apply to be set.
 *
 * @param update_all
 *   false = only apply entries that are pending / mismatched
 *   true  = resend the full tracked configuration regardless of local match
 */
void mGPS_Serial::ApplyPacketConfig_Pending(bool update_all)
{
  if (!update_all)
  {
    if (!packet_config.pending_apply) {
      return;
    }
  }

  ALOG_INF(PSTR("Applying GPS packet config%s"), update_all ? " (update_all)" : "");

  bool still_pending = false;

  // --------------------------------------------------
  // UBX NAV
  // --------------------------------------------------
  for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++)
  {
    const bool needs_update =
      update_all ||
      (packet_config.ubx_nav[i].enabled != packet_config.ubx_nav[i].desired_enabled);

    if (!needs_update) {
      continue;
    }

    bool ok = false;

    if (packet_config.ubx_nav[i].desired_enabled) {
      ok = ubx_parser->enable_msg(
        ublox::UBX_NAV,
        (ublox::msg_id_t)ublox::UBX_NAV_LIST[i]
      );
    } else {
      ok = ubx_parser->disable_msg(
        ublox::UBX_NAV,
        (ublox::msg_id_t)ublox::UBX_NAV_LIST[i]
      );
    }

    if (ok) {
      packet_config.ubx_nav[i].enabled = packet_config.ubx_nav[i].desired_enabled;
    } else {
      still_pending = true;
    }
  }

  // --------------------------------------------------
  // NMEA
  // --------------------------------------------------
  for (uint8_t i = 0; i < ((uint8_t)NMEAGPS::NMEAMSG_END - 1); i++)
  {
    const bool needs_update =
      update_all ||
      (packet_config.nmea[i].enabled != packet_config.nmea[i].desired_enabled);

    if (!needs_update) {
      continue;
    }

    const NMEAGPS::nmea_msg_t msg_id = (NMEAGPS::nmea_msg_t)(i + 1);

    const uint8_t rate =
      packet_config.nmea[i].desired_enabled ? packet_config.desired_rate : 0;

    bool ok = ublox::configNMEA(*ubx_parser, msg_id, rate);

    if (ok) {
      packet_config.nmea[i].enabled = packet_config.nmea[i].desired_enabled;
    } else {
      still_pending = true;
    }
  }

  packet_config.pending_apply = still_pending;
}



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mGPS_Serial::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  JsonParserObject jobj2 = obj[GetModuleName()]; // "gps"

  if(!jobj2) return; // Only allow commands for this module

  if(jtok = jobj2["SetGPSModuleBaud"])
  {
    SetGPSModuleBaud(jtok.getInt());
  }
    
  uint8_t rate = 0;
  if(jtok = jobj2["Rate"])
  {
    rate = jtok.getInt();
  }

  if (jtok = jobj2["DefaultMode"])
  {
    Set_Default_PacketConfig();
  }

  
  if(jtok = jobj2["NMEA"].getObject()["SetRateAll"])
  {
    ALOG_INF(PSTR("NMEA-SetRateAll"));

    rate = jtok.getInt();

    for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
      ublox::configNMEA( *ubx_parser, (NMEAGPS::nmea_msg_t) i, rate );
    }
    
  }


  if(jtok = jobj2["UBX"].getObject()["SetRateAll"])
  {
    ALOG_INF(PSTR("UBX-SetRateAll"));

    rate = jtok.getInt();

    uint8_t ids[] = {
      ublox::UBX_NAV_POSLLH,
      ublox::UBX_NAV_STATUS,
      ublox::UBX_NAV_DOP,
      ublox::UBX_NAV_PVT,
      ublox::UBX_NAV_VELNED,
      ublox::UBX_NAV_TIMEGPS,
      ublox::UBX_NAV_TIMEUTC,
      ublox::UBX_NAV_SVINFO
    };

    for (uint8_t id=0; id<ARRAY_SIZE(ids); id++){
      if(rate)
        ubx_parser->enable_msg( ublox::UBX_NAV, (ublox::msg_id_t) id );
      else
        ubx_parser->disable_msg( ublox::UBX_NAV, (ublox::msg_id_t) id );
    }

  }
    
  if(jtok = jobj2["UBXPacket"])
  {
    ALOG_INF(PSTR("UBXPacket"));

    uint8_t packet_index = jtok.getInt();

    uint8_t ids[] = {
      ublox::UBX_NAV_POSLLH,
      ublox::UBX_NAV_STATUS,
      ublox::UBX_NAV_DOP,
      ublox::UBX_NAV_PVT,
      ublox::UBX_NAV_VELNED,
      ublox::UBX_NAV_TIMEGPS,
      ublox::UBX_NAV_TIMEUTC,
      ublox::UBX_NAV_SVINFO
    };

    if (packet_index < ARRAY_SIZE(ids))
    {
      uint8_t packet_id = ids[packet_index];

      ALOG_INF(PSTR("UBXPacket index=%d id=%02X rate=%d"), packet_index, packet_id, rate);

      if(rate)
        ubx_parser->enable_msg(ublox::UBX_NAV, (ublox::msg_id_t)packet_id, rate);
      else
        ubx_parser->disable_msg(ublox::UBX_NAV, (ublox::msg_id_t)packet_id);
    }
    else
    {
      ALOG_ERR(PSTR("UBXPacket index out of range: %d"), packet_index);
    }
  }


  if(jtok = jobj2["NMEA_Enabled"])
  {
    ALOG_INF(PSTR("NMEA_Enabled"));

    rate = jtok.getInt();
    
    for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
      ublox::configNMEA( *ubx_parser, (NMEAGPS::nmea_msg_t) i, rate );
    }
  }


}




/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mGPS_Serial::Telemetry_Init(){

  struct telemetry_handler<mGPS_Serial>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_gpspacket_debug; //also ifchanged together
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_DEBUG_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Debug;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_gpspacket_micro; //also ifchanged together
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MICRO_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Micro;
  telemetry_list.push_back(ptr);


ptr = &telemetry_gpspacket_all1;
ptr->tSavedLastSent = 0;
ptr->flags.PeriodicEnabled = true;
ptr->flags.SendNow = true;
ptr->tRateSecs = 1;
ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
ptr->flags.json_level = JSON_LEVEL_DETAILED;
ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL1_CTR;
ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_All1;
telemetry_list.push_back(ptr);

ptr = &telemetry_gpspacket_all2;
ptr->tSavedLastSent = 0;
ptr->flags.PeriodicEnabled = true;
ptr->flags.SendNow = true;
ptr->tRateSecs = 10;
ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
ptr->flags.json_level = JSON_LEVEL_DETAILED;
ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL2_CTR;
ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_All2;
telemetry_list.push_back(ptr);

ptr = &telemetry_gpspacket_all3;
ptr->tSavedLastSent = 0;
ptr->flags.PeriodicEnabled = true;
ptr->flags.SendNow = true;
ptr->tRateSecs = 10;
ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
ptr->flags.json_level = JSON_LEVEL_DETAILED;
ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL3_CTR;
ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_All3;
telemetry_list.push_back(ptr);

ptr = &telemetry_gpspacket_all4;
ptr->tSavedLastSent = 0;
ptr->flags.PeriodicEnabled = true;
ptr->flags.SendNow = true;
ptr->tRateSecs = 10;
ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
ptr->flags.json_level = JSON_LEVEL_DETAILED;
ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL4_CTR;
ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_All4;
telemetry_list.push_back(ptr);


  ptr = &telemetry_gpspacket_minimal_teleperiod; //also ifchanged together
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Minimal;
  telemetry_list.push_back(ptr);

  // All sensor readings I had on pic32
  ptr = &telemetry_gpspacket_required;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_REQUIRED_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Required;
  telemetry_list.push_back(ptr);

  // New from 2026

  ptr = &telemetry_nav__pvt;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10;//tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__NAV_PVT;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_NAV_PVT;
  telemetry_list.push_back(ptr);

}

#endif // USE_MODULE_NETWORK_MQTT




#endif //USE_MODULE_SENSORS_GPS_SERIAL