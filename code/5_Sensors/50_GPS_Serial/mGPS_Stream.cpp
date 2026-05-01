#include "_mGPS_Serial.h"

// File contains all fucntions related to getting data from the GPS module via UART

#ifdef USE_MODULE_SENSORS_GPS_SERIAL




void mGPS_Serial::Handle__Loop_Input_Stream()
{

  switch(read_gps_method)
  {
    case UBX_POLLING_BYTE_STREAM__DIRECT_SERIAL_BASIC_VIA_PARSER:
      UBX_Polling_BasicViaParser();
    break;
    case UBX_POLLING_BYTE_STREAM__DIRECT_SERIAL:
      UBX_Polling();
    break;
//     case GPS_INPUT_STREAM_METHOD_POLLING_ID:
//     {
//       #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
//         // EveryLoop_InputMethod_PollingSerial_Internal();
//         // EveryLoop_InputMethod_PollingSerial_Bytes();
//         // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
//         EveryLoop_PollForGPSData(Serial1);
//         EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();
//       #endif // USE_DEVFEATURE_GPS_POLLING_INPUT
//     }
//     break;
//     case GPS_INPUT_STREAM_METHOD_RINGBUFFERS_ID:
//     {
//       #ifdef ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//       #ifdef USE_MODULE_DRIVERS_SERIAL_UART

//       // uint16_t bytes_to_read_for_faster_parsing = 100;


//       // BufferWriterI->Clear();
//       uint16_t bytes_to_read = tkr_uart->GetRingBufferDataAndClear(1, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//       // if(strlen(BufferWriterI->GetPtr())==0){
//       //   ALOG_TST(PSTR("GPS UART%d >> [%d] \"%s\""), 1, bytes_to_read, BufferWriterI->GetPtr());
//       // }

//       bool gps_fix_reading = false;

//       // Serial.printf("bytes = %d\n\r", bytes_to_read);
//       //if any data found
//       if(bytes_to_read)
//       {  
//         char* pbuffer = BufferWriterI->GetPtr();
//         //ALOG_TST(PSTR("GPS >> [%d]"), bytes_to_read);
//         // ALOG_TST(PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
//         // Read bytes in
//         for(int ii=0;ii<bytes_to_read;ii++)
//         {
//           // nmea_parser->parser_byte_in(pbuffer[ii]);
//           ubx_parser->parser_byte_in(pbuffer[ii]);

//           // Check on fix status
//           fix_parsing = ubx_parser->read();
//           if(fix_parsing.status > GPS_FIX::STATUS_NONE)
//           {
//             gps_fix_reading = true;
//             flag_incoming_data_at_correct_runtime_baud = true;
//             // fix_valid |= fix_parsing; // Save reading
//             // Serial.printf("=============================================%d\n\r",fix_parsing.status);
//           }
//             fix_valid |= fix_parsing; // Save reading
//           // Serial.printf("status=%d\n\r",fix_parsing.status);

//           /**
//            * Manually update ISR Safe values into struct for sd output
//            * */
//           my_gps_vals.lat = fix_valid.latitudeL();
//           my_gps_vals.lon = fix_valid.longitudeL();
//           my_gps_vals.alt = fix_valid.altitude_cm();
//           my_gps_vals.speed = (uint16_t)fix_valid.speed()*100; //float to int
//           my_gps_vals.heading_cd = fix_valid.heading_cd();
//           my_gps_vals.geoidHeight_cm = fix_valid.geoidHeight_cm();
//           my_gps_vals.hours = fix_valid.dateTime.hours+1; //add daylight savings
//           my_gps_vals.minutes = fix_valid.dateTime.minutes;
//           my_gps_vals.seconds = fix_valid.dateTime.seconds;
//           my_gps_vals.dateTime_ms = fix_valid.dateTime_ms();


//           tkr_uart->special_json_part_of_gps_buflen = sprintf( tkr_uart->special_json_part_of_gps_buffer,
//             "B]],\"G\":[%d,%d,%d,%d,%d,%d,%d,%d]}@", 
//             my_gps_vals.lat,
//             my_gps_vals.lon,
//             my_gps_vals.alt,
//             my_gps_vals.speed,
//             my_gps_vals.hours,
//             my_gps_vals.minutes,
//             my_gps_vals.seconds,
//             my_gps_vals.dateTime_ms/100
//           );


// //       if(sequence_test_global == 0){
// //         JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
// //       }
// //       JBI->Add("N", sequence_test_global++);
          
//     // char special_json_part_of_gps_buffer[300];
//     // uint16_t special_json_part_of_gps_buflen = 0;




          
//             //if first fix with valud time, use this to update internal time
//             if((gps_latest.fix_used_to_update_internal_rtc == false)&&(fix_valid.dateTime.is_valid()))
//             {
//               gps_latest.fix_used_to_update_internal_rtc = true;
//               // Later, do this as a rule event              
//               tkr_time->SetUTCTime(
//                 fix_valid.dateTime.year,
//                 fix_valid.dateTime.month,
//                 fix_valid.dateTime.day,
//                 fix_valid.dateTime.hours,
//                 fix_valid.dateTime.minutes,
//                 fix_valid.dateTime.seconds
//               );
//             }

//         }

//       }


//       #endif//USE_MODULE_DRIVERS_SERIAL_UART
//       #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

//     }
//     break;
  }

// EveryLoop_InputMethod_PollingSerial_BytesToBuffer
// ISR methods, read per


}
static const uint32_t GPS_BAUD_CANDIDATES[] = {
  115200,
  230400,
  460800,
  921600,
  19200,
  38400,
  57600,
  9600
};


bool mGPS_Serial::GPS_ProbeBaud(uint32_t baud, uint32_t timeout_ms)
{
  if (port == nullptr || ubx_parser == nullptr) {
    return false;
  }

  port->updateBaudRate(baud);
  delay(20);

  // Drain stale bytes after switching baud
  while (port->available()) {
    port->read();
  }

  const uint32_t t_start = millis();

  while ((millis() - t_start) < timeout_ms)
  {
    while (port->available())
    {
      const char c = port->read();
      const NMEAGPS::decode_t res = ubx_parser->handle(c);

      if (res == NMEAGPS::DECODE_COMPLETED) {
        return true;
      }
    }

    delay(1);
  }

  return false;
}


bool mGPS_Serial::GPS_FindWorkingBaud(uint32_t& found_baud)
{
  found_baud = 0;

  for (uint8_t i = 0; i < ARRAY_SIZE(GPS_BAUD_CANDIDATES); i++)
  {
    const uint32_t test_baud = GPS_BAUD_CANDIDATES[i];

    if (GPS_ProbeBaud(test_baud, 400))
    {
      found_baud = test_baud;
      return true;
    }
  }

  return false;
}


bool mGPS_Serial::GPS_ChangeDeviceBaud(uint32_t current_baud, uint32_t new_baud)
{
  if (port == nullptr || ubx_parser == nullptr) {
    return false;
  }

  if (current_baud == new_baud) {
    return true;
  }

  // Make sure host is on currently working baud
  port->updateBaudRate(current_baud);
  delay(20);

  // Drain existing bytes before sending baud change
  while (port->available()) {
    port->read();
  }

  // Send UBX-CFG-PRT via library helper
  if (!ubx_parser->send_baud(new_baud)) {
    return false;
  }

  port->flush();
  delay(50);

  // Move host UART to new baud
  port->updateBaudRate(new_baud);
  delay(50);

  // Verify valid packets at the new baud
  return GPS_ProbeBaud(new_baud, 700);
}


bool mGPS_Serial::SetGPSModuleBaud(uint32_t desired_baud)
{
  if (port == nullptr || ubx_parser == nullptr) {
    return false;
  }

  uint32_t working_baud = 0;

  // 1) Try desired baud first
  if (GPS_ProbeBaud(desired_baud, 500))
  {
    ALOG_INF(PSTR("GPS already responding at desired baud %lu"), (unsigned long)desired_baud);
    return true;
  }

  // 2) Otherwise scan all common baud rates
  if (!GPS_FindWorkingBaud(working_baud))
  {
    ALOG_ERR(PSTR("GPS baud scan failed"));
    return false;
  }

  ALOG_INF(PSTR("GPS found at baud %lu"), (unsigned long)working_baud);

  // 3) If found baud differs from desired baud, change GPS and host to desired baud
  if (working_baud != desired_baud)
  {
    ALOG_INF(
      PSTR("Changing GPS baud %lu -> %lu"),
      (unsigned long)working_baud,
      (unsigned long)desired_baud
    );

    if (!GPS_ChangeDeviceBaud(working_baud, desired_baud))
    {
      ALOG_ERR(PSTR("GPS baud change failed"));
      return false;
    }

    ALOG_INF(PSTR("GPS now running at desired baud %lu"), (unsigned long)desired_baud);
  }

  return true;
}

const char* mGPS_Serial::GetNmeaMessageName(uint8_t nmea_msg)
{
  switch (nmea_msg)
  {
    case NMEAGPS::NMEA_GGA: return "GGA";
    case NMEAGPS::NMEA_GLL: return "GLL";
    case NMEAGPS::NMEA_GSA: return "GSA";
    case NMEAGPS::NMEA_GST: return "GST";
    case NMEAGPS::NMEA_GSV: return "GSV";
    case NMEAGPS::NMEA_RMC: return "RMC";
    case NMEAGPS::NMEA_VTG: return "VTG";
    case NMEAGPS::NMEA_ZDA: return "ZDA";
    default:                return "UNKNOWN";
  }
}

const char* mGPS_Serial::GetUbxMessageName(uint8_t cls, uint8_t id)
{
  switch (cls)
  {
    case ublox::UBX_NAV: // 0x01
      switch (id)
      {
        case ublox::UBX_NAV_POSECEF:   return "NAV-POSECEF";
        case ublox::UBX_NAV_POSLLH:    return "NAV-POSLLH";
        case ublox::UBX_NAV_STATUS:    return "NAV-STATUS";
        case ublox::UBX_NAV_DOP:       return "NAV-DOP";
        case ublox::UBX_NAV_PVT:       return "NAV-PVT";
        case ublox::UBX_NAV_ODO:       return "NAV-ODO";
        case ublox::UBX_NAV_RESETODO:  return "NAV-RESETODO";
        case ublox::UBX_NAV_VELECEF:   return "NAV-VELECEF";
        case ublox::UBX_NAV_VELNED:    return "NAV-VELNED";
        case ublox::UBX_NAV_TIMEGPS:   return "NAV-TIMEGPS";
        case ublox::UBX_NAV_TIMEUTC:   return "NAV-TIMEUTC";
        case ublox::UBX_NAV_CLOCK:     return "NAV-CLOCK";
        case ublox::UBX_NAV_TIMEGLO:   return "NAV-TIMEGLO";
        case ublox::UBX_NAV_TIMEBDS:   return "NAV-TIMEBDS";
        case ublox::UBX_NAV_TIMEGAL:   return "NAV-TIMEGAL";
        case ublox::UBX_NAV_TIMEQZSS:  return "NAV-TIMEQZSS";
        case ublox::UBX_NAV_SVINFO:    return "NAV-SVINFO";
        case ublox::UBX_NAV_SBAS:      return "NAV-SBAS";
        case ublox::UBX_NAV_ORB:       return "NAV-ORB";
        case ublox::UBX_NAV_SAT:       return "NAV-SAT";
        case ublox::UBX_NAV_COV:       return "NAV-COV";
        case ublox::UBX_NAV_GEOFENCE:  return "NAV-GEOFENCE";
        case ublox::UBX_NAV_SLAS:      return "NAV-SLAS";
        case ublox::UBX_NAV_SIG:       return "NAV-SIG";
        case ublox::UBX_NAV_AOPSTATUS: return "NAV-AOPSTATUS";
        case ublox::UBX_NAV_EOE:       return "NAV-EOE";
        default:                       return "NAV-?";
      }

    case ublox::UBX_RXM: // 0x02
      switch (id)
      {
        default: return "RXM-?";
      }

    case ublox::UBX_INF: // 0x04
      switch (id)
      {
        default: return "INF-?";
      }

    case ublox::UBX_ACK: // 0x05
      switch (id)
      {
        case ublox::UBX_ACK_NAK: return "ACK-NAK";
        case ublox::UBX_ACK_ACK: return "ACK-ACK";
        default:                 return "ACK-?";
      }

    case ublox::UBX_CFG: // 0x06
      switch (id)
      {
        case ublox::UBX_CFG_MSG:  return "CFG-MSG";
        case ublox::UBX_CFG_RST:  return "CFG-RST";
        case ublox::UBX_CFG_RATE: return "CFG-RATE";
        case ublox::UBX_CFG_NMEA: return "CFG-NMEA";
        case ublox::UBX_CFG_NAV5: return "CFG-NAV5";
        default:                  return "CFG-?";
      }

    case ublox::UBX_MON: // 0x0A
      switch (id)
      {
        case ublox::UBX_MON_VER: return "MON-VER";
        default:                 return "MON-?";
      }

    case ublox::UBX_AID: // 0x0B
      switch (id)
      {
        default: return "AID-?";
      }

    case ublox::UBX_TIM: // 0x0D
      switch (id)
      {
        default: return "TIM-?";
      }

    case ublox::UBX_HNR: // 0x28
      switch (id)
      {
        case ublox::UBX_HNR_PVT: return "HNR-PVT";
        default:                 return "HNR-?";
      }

    case ublox::UBX_NMEA: // 0xF0
      switch (id)
      {
        default: return "NMEA-STD-?";
      }

    case ublox::UBX_PUBX: // 0xF1
      switch (id)
      {
        default: return "PUBX-?";
      }

    default:
      return "UNKNOWN";
  }
}
void mGPS_Serial::Splash_Packet_Metrics()
{
  #ifdef ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS

  const uint32_t now = millis();

  SERIAL_TERMINAL_POSITION_RESET();

  SERIAL_DEBUG.println();
  SERIAL_DEBUG.println(F("--------------------------------------------------------------------------"));
  SERIAL_DEBUG.println(F("Packet Metrics"));
  SERIAL_DEBUG.println(F("--------------------------------------------------------------------------"));

  // ============================================================
  // NMEA
  // ============================================================
  SERIAL_DEBUG.println(F("NMEA"));
  SERIAL_DEBUG.println(F("Msg   Ena  Des  Count        Age(ms)"));
  SERIAL_DEBUG.println(F("----  ---  ---  -----------  -----------"));

  bool nmea_any_active = false;

  for (uint8_t nmea_msg = NMEAGPS::NMEA_UNKNOWN + 1;
       nmea_msg < NMEAGPS::NMEAMSG_END;
       nmea_msg++)
  {
    const uint8_t cfg_i = (uint8_t)(nmea_msg - 1);
    nmea_msg_state_t* msg = packet_metrics.FindNmeaMessage(nmea_msg);

    const bool enabled_now   = packet_config.nmea[cfg_i].enabled;
    const bool enabled_want  = packet_config.nmea[cfg_i].desired_enabled;

    const uint32_t count_rx  = msg ? msg->count_rx : 0;
    const uint32_t age_ms    = (msg && msg->last_seen_ms) ? (now - msg->last_seen_ms) : 0;

    if ((count_rx == 0) && !enabled_now && !enabled_want) {
      continue;
    }

    nmea_any_active = true;

    SERIAL_DEBUG.printf(
      "%-4s  %-3s  %-3s  %-11lu  %-11lu\r\n",
      GetNmeaMessageName(nmea_msg),
      enabled_now  ? "Y" : "N",
      enabled_want ? "Y" : "N",
      (unsigned long)count_rx,
      (unsigned long)age_ms
    );
  }

  if (!nmea_any_active) {
    SERIAL_DEBUG.println(F("Not active"));
  }

  SERIAL_DEBUG.println();

  // ============================================================
  // UBX
  // ============================================================
  SERIAL_DEBUG.println(F("UBX NAV"));
  SERIAL_DEBUG.println(F("Idx  Id   Name         Ena  Des  Count        Age(ms)     Len"));
  SERIAL_DEBUG.println(F("---- ---- ------------ ---  ---  -----------  ----------- -----"));

  bool ubx_any_active = false;

  for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++)
  {
    const uint8_t msg_id = ublox::UBX_NAV_LIST[i];
    ubx_msg_state_t* msg = packet_metrics.FindUbxMessage(ublox::UBX_NAV, msg_id);

    const bool enabled_now   = packet_config.ubx_nav[i].enabled;
    const bool enabled_want  = packet_config.ubx_nav[i].desired_enabled;

    const uint32_t count_rx  = msg ? msg->count_rx : 0;
    const uint32_t age_ms    = (msg && msg->last_seen_ms) ? (now - msg->last_seen_ms) : 0;
    const uint16_t last_len  = msg ? msg->last_payload_len : 0;

    if ((count_rx == 0) && !enabled_now && !enabled_want) {
      continue;
    }

    ubx_any_active = true;

    SERIAL_DEBUG.printf(
      "%-4u %02X   %-12s %-3s  %-3s  %-11lu  %-11lu %-5u\r\n",
      i,
      msg_id,
      GetUbxMessageName(ublox::UBX_NAV, msg_id),
      enabled_now  ? "Y" : "N",
      enabled_want ? "Y" : "N",
      (unsigned long)count_rx,
      (unsigned long)age_ms,
      (unsigned)last_len
    );
  }

  if (!ubx_any_active) {
    SERIAL_DEBUG.println(F("Not active"));
  }

  SERIAL_DEBUG.println(F("--------------------------------------------------------------------------"));
  SERIAL_DEBUG.println();

  #endif // ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS
}


void mGPS_Serial::UBX_ProcessPacketMetrics_OnDecodeCompleted(NMEAGPS::decode_t res)
{
  #ifdef ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS

  if (res != NMEAGPS::DECODE_COMPLETED) {
    return;
  }

  const auto &meta = ubx_parser->completed_packet_meta;

  if (meta.is_ubx) {
    packet_metrics.UpdateUbxMessage(
      meta.msg_class,
      meta.msg_id,
      meta.payload_length
    );
  } else {
    packet_metrics.UpdateNmeaMessage(meta.msg_id);
  }

  #endif
}


void mGPS_Serial::UBX_ProcessAvailableFix()
{
  if (!ubx_parser->available()) {
    return;
  }

  fix_parsing = ubx_parser->read();

  stats.packets_received++;
  stats.last_message_received_time = millis();

  if (fix_parsing.dateTime.hours) {
    ALOG_DBG(
      PSTR("GPS Par Time: %02d:%02d:%02d"),
      fix_parsing.dateTime.hours,
      fix_parsing.dateTime.minutes,
      fix_parsing.dateTime.seconds
    );
  }

  if (fix_parsing.status > gps_fix::STATUS_NONE)
  {
    fix_valid |= fix_parsing; // Merge latest payload decode into validated stored data

    ALOG_INF(
      PSTR("GPS Fix Time: %02d:%02d:%02d pkt:%d"),
      fix_valid.dateTime.hours,
      fix_valid.dateTime.minutes,
      fix_valid.dateTime.seconds,
      stats.packets_received
    );
  }
}


/*
Really only for testing of the original method
*/
void mGPS_Serial::UBX_Polling_BasicViaParser()
{

  if(ubx_parser->available(gpsPort))
  {  // feed parser from UART

    fix_parsing = ubx_parser->read(); // take completed packet
  
    if(fix_parsing.status > gps_fix::STATUS_NONE)
    {
      fix_valid |= fix_parsing; // Merge latest payload decode into validated stored data
      ALOG_INF(PSTR("GPS Fix Time: %02d:%02d:%02d pkt:%d"), fix_valid.dateTime.hours, fix_valid.dateTime.minutes, fix_valid.dateTime.seconds, stats.packets_received);
    }
    
    if((millis()-tTimeout)>100)
    {
      Serial.printf("PVT fix: %02d:%02d:%02d lat=%.6f lon=%.6f alt=%.2f sats=%d\r\n", fix_valid.dateTime.hours, fix_valid.dateTime.minutes, fix_valid.dateTime.seconds, fix_valid.latitude(), fix_valid.longitude(), fix_valid.altitude(), fix_valid.satellites);
      tTimeout = millis();
    }
  }
  
}



void mGPS_Serial::UBX_Polling()
{
  if (!gpsPort.available()) {
    return;
  }

  char c = gpsPort.read();

  // if(flag_show_incoming_gps_byte_stream) {
  //   Serial.write(c);
  // }

  NMEAGPS::decode_t res = ubx_parser->handle(c);

  UBX_ProcessPacketMetrics_OnDecodeCompleted(res);
  UBX_ProcessAvailableFix();
}



bool mGPS_Serial::Init_BeginStream()
{

  // Later with UART module integration, the type will be set here via port = pkt_uart->GetSerial(2)
  port = &Serial2;

  ubx_parser = new ubloxGPS(port); 

  uint32_t baud = 115200;
  #ifdef D_GPS_BAUD_RATE_DEFAULT
    baud = D_GPS_BAUD_RATE_DEFAULT;
  #endif

  port->begin(baud, SERIAL_8N1, 16, 17);

  packet_config.baud_current = baud;
  packet_config.baud_target = baud;
  packet_config.pending_baud_apply = false;

  if(!SetGPSModuleBaud(baud))
  {
    ALOG_ERR(PSTR("SetGPSModuleBaud failed"));
    return false;
  }
  
  return true; 

}


#ifdef ENABLE_DEVFEATURE_GPS__PARSER_TEST1

void mGPS_Serial::GPS_Incoming_Stream(Stream& stream)
{
  if (stream.available() > 0)
  {
    uint8_t c = (uint8_t)stream.read();
    Serial.print(c,HEX);
    stats.bytes_received++;
    GPS_ProcessByte(c);
  }
}
bool mGPS_Serial::GPS_ProcessByte(uint8_t c)
{
  ubx_parser.handle(c);

  if (!ubx_parser.available()) {
    return false;
  }

  fix_parsing = ubx_parser.read();

  stats.packets_received++;
  stats.last_message_received_time = millis();

  return GPS_CommitFixIfAccepted(fix_parsing);
}
bool mGPS_Serial::GPS_CommitFixIfAccepted(const gps_fix& candidate)
{
  if (candidate.status <= gps_fix::STATUS_NONE) {
    return false;
  }

  fix_valid |= candidate;
  stats.last_valid_message_received_time = millis();
  return true;
}

#endif


#ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
/**
 * Method A: Check Serial port directly, and read bytes into parser
 * @note timeout is used so loop is not blocking 
 * */
void mGPS_Serial::EveryLoop_InputMethod_PollingSerial_Internal()
{

  // uint32_t time = millis();
      
  // while (nmea_parser->available( gpsPort )) {
  //   fix_parsing = nmea_parser->read();

  //   if(fix_parsing.status > gps_fix::STATUS_NONE ){

  //     //save tmp solution when valid
  //     fix_valid = fix_parsing;

  //     DEBUG_PORT.print( F("Location: ") );
  //       DEBUG_PORT.print( fix_parsing.latitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( fix_parsing.longitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( fix_parsing.altitude_cm(), 6 ); 
  //       DEBUG_PORT.print( "cm" );

  //     DEBUG_PORT.print( F(", Altitude: ") );
  //     if (fix_parsing.valid.altitude)
  //       DEBUG_PORT.print( fix_parsing.altitude() );

  //     DEBUG_PORT.println();
  //   }

  //   if(abs(millis()-time) > 10){
  //     break;
  //   }

  // }

} // END fucntion

void mGPS_Serial::EveryLoop_InputMethod_PollingSerial_Bytes()
{

  // uint32_t timeout = millis();
  // bool bytes_waiting = false;
  // bool gps_fix_reading = false;

  // while(Serial1.available())
  // {
  //   // Read bytes in
  //   nmea_parser->parser_byte_in(Serial1.read());

  //   // Check on fix status
  //   fix_parsing = nmea_parser->read();
  //   if(fix_parsing.status > gps_fix::STATUS_NONE)
  //   {
  //     gps_fix_reading = true;
  //     fix_valid = fix_parsing; // Save reading
  //   }

  //   // Check for timeout    
  //   if(abs(millis()-timeout) > 10){
  //     break;
  //   }
  // }

}

/**
 * Reading into a buffer, then pushing that data into the parser
 * */
void mGPS_Serial::EveryLoop_InputMethod_PollingSerial_BytesToBuffer()
{

  // uint32_t timeout = millis();
  // bool bytes_waiting = false;
  // bool gps_fix_reading = false;

  // char buffer[400] = {0};
  // uint8_t buflen = 0;

  // while(Serial1.available())
  // {
  //   if(buflen < 400)
  //   {
  //     buffer[buflen++] = Serial1.read();
  //   }
  //   else
  //   {
  //     break; // exceeded buffer
  //   } 

  //   // Check for timeout    
  //   if(abs(millis()-timeout) > 10){
  //     break;
  //   }
  // }


  // //if any data found
  // if(buflen)
  // {  
  //   ALOG_TST(PSTR("buffer[%d]=\"%s\""),buflen, buffer);
  //   // Read bytes in
  //   for(int ii=0;ii<buflen;ii++)
  //   {
  //     nmea_parser->parser_byte_in(buffer[ii]);

  //     // Check on fix status
  //     fix_parsing = nmea_parser->read();
  //     if(fix_parsing.status > gps_fix::STATUS_NONE)
  //     {
  //       gps_fix_reading = true;
  //       fix_valid = fix_parsing; // Save reading
  //     }
  //   }

  // }


}

void mGPS_Serial::EveryLoop_PollForGPSData(Stream& port)
{

  // uint32_t timeout = millis();
  // while(port.available())
  // {
  //   if(gps_receive_buffer.bufused < gps_receive_buffer.buflen)
  //   {
  //     gps_receive_buffer.buffer[gps_receive_buffer.bufused++] = port.read();
  //   }
  //   else
  //   {
  //     AddLog(LOG_LEVEL_ERROR, PSTR("buffer overflow"));
  //     break; // exceeded buffer
  //   } 

  //   // Check for timeout    
  //   if(abs(millis()-timeout) > 5){
  //     break;
  //   }
  // }

}


/**
 * Reading into a buffer, then pushing that data into the parser
 * */
void mGPS_Serial::EveryLoop_InputMethod_PollingSerial_BytesFromBuffer()
{

  // bool bytes_waiting = false;
  // bool gps_fix_reading = false;

  // //if any data found
  // if(gps_receive_buffer.bufused)
  // {  
  //   // ALOG_TST(PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
  //   // Read bytes in
  //   for(int ii=0;ii<gps_receive_buffer.bufused;ii++)
  //   {
  //     nmea_parser->parser_byte_in(gps_receive_buffer.buffer[ii]);

  //     // Check on fix status
  //     fix_parsing = nmea_parser->read();
  //     if(fix_parsing.status > gps_fix::STATUS_NONE)
  //     {
  //       gps_fix_reading = true;
  //       fix_valid |= fix_parsing; // Save reading
  //     }
  //   }

  //   // Reset buffer
  //   gps_receive_buffer.bufused = 0;
  //   memset(gps_receive_buffer.buffer,0,gps_receive_buffer.buflen);

  // }

}


#endif // USE_DEVFEATURE_GPS_POLLING_INPUT



void mGPS_Serial::ApplyGPSBaud_Pending()
{
  if (!packet_config.pending_baud_apply) {
    return;
  }

  if (packet_config.baud_target == 0) {
    packet_config.pending_baud_apply = false;
    return;
  }

  if (packet_config.baud_current == packet_config.baud_target) {
    packet_config.pending_baud_apply = false;
    return;
  }

  if (SetGPSModuleBaud(packet_config.baud_target))
  {
    packet_config.baud_current = packet_config.baud_target;
    packet_config.pending_baud_apply = false;

    ALOG_INF(PSTR("GPS baud applied: %lu"), (unsigned long)packet_config.baud_current);
  }
  else
  {
    ALOG_ERR(
      PSTR("GPS baud apply failed current=%lu desired=%lu"),
      (unsigned long)packet_config.baud_current,
      (unsigned long)packet_config.baud_target
    );
  }
}



#endif // USE_MODULE_SENSORS_GPS_SERIAL