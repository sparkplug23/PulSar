
#include "_mSIM7000G.h"


#ifdef USE_MODULE_DRIVERS_MODEM_7000G



#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

void mSIM7000G::GNSS_Enable()
{
  if (!modem) { return; }
  if (!ModemInit_IsReady()) { return; }

  ALOG_INF(PSTR(D_LOG_CELLULAR "GNSS enable"));

  modem->sendAT("+SGPIO=0,4,1,1");

  if (modem->waitResponse(10000L) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "GNSS SGPIO enable failed"));
  }

  if (!modem->enableGPS())
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "GNSS modem enableGPS failed"));
    gps.enabled = false;
    return;
  }

  gps.enabled = true;
}

void mSIM7000G::GNSS_Disable()
{
  if (!modem) { return; }

  ALOG_INF(PSTR(D_LOG_CELLULAR "GNSS disable"));

  modem->disableGPS();

  modem->sendAT("+SGPIO=0,4,1,0");

  if (modem->waitResponse(10000L) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "GNSS SGPIO disable failed"));
  }

  gps.configured = false;
  gps.fix_valid = false;
}


bool mSIM7000G::GNSS_Configure()
{
  if (!modem) { return false; }
  if (!ModemInit_IsReady()) { return false; }

  ALOG_INF(PSTR(D_LOG_CELLULAR "GNSS configure"));

  // Board-level GNSS power enable path used by LilyGO/SIM7000G examples.
  modem->sendAT("+SGPIO=0,4,1,1");

  if (modem->waitResponse(10000L) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "GNSS SGPIO enable failed"));
    // Do not return here. Some boards/firmware may not require/respond to SGPIO.
  }

  if (!modem->enableGPS())
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "GNSS enableGPS failed"));
    gps.configured = false;
    return false;
  }

  gps.configured = true;
  gps.fix_valid = false;
  gps.fail_count = 0;

  ALOG_INF(PSTR(D_LOG_CELLULAR "GNSS configured"));
  return true;
}



void mSIM7000G::GNSS_Update()
{
  if (!modem) { return; }
  if (!ModemInit_IsReady()) { return; }

  // gps.enabled means desired GNSS state.
  if (!gps.enabled)
  {
    return;
  }

  const uint32_t now_ms = millis();

  // -------------------------------------------------------------------------
  // Ensure GNSS hardware/AT engine has been configured.
  // Retry slowly if configuration fails.
  // -------------------------------------------------------------------------
  if (!gps.configured)
  {
    if (!mTime::TimeReached(&gps.last_config_attempt_millis, gps.config_retry_ms))
    {
      return;
    }

    if (!GNSS_Configure())
    {
      return;
    }
  }

  if (!mTime::TimeReached(&gps.tReached_Update, gps.update_period_ms))
  {
    return;
  }

  // -------------------------------------------------------------------------
  // Local GNSS read values.
  // These are intentionally not stored as a full SIM7000G GPS_STATUS struct.
  // The accepted/common result is published into tkr_iSensors->system_location.
  // -------------------------------------------------------------------------
  float latitude           = 0.0f;
  float longitude          = 0.0f;
  float speed              = 0.0f;
  float altitude           = 0.0f;
  int   vsat               = 0;
  int   usat               = 0;
  float accuracy           = 0.0f;
  int   year               = 0;
  int   month              = 0;
  int   day                = 0;
  int   hour               = 0;
  int   minute             = 0;
  int   second             = 0;
  float course             = 0.0f;
  int   fix_mode           = 0;
  float accuracy_position  = 0.0f;
  float accuracy_vertical  = 0.0f;
  int   usat_glonass       = 0;
  int   cno_max            = 0;
  int   hpa                = 0;
  int   vpa                = 0;

  const bool fix_ok = modem->getGPS_Detailed(
    &latitude,
    &longitude,
    &speed,
    &altitude,
    &vsat,
    &usat,
    &accuracy,
    &year,
    &month,
    &day,
    &hour,
    &minute,
    &second,
    &course,
    &fix_mode,
    &accuracy_position,
    &accuracy_vertical,
    &usat_glonass,
    &cno_max,
    &hpa,
    &vpa
  );

  if (!fix_ok)
  {
    gps.fix_valid        = false;
    gps.last_fail_millis = millis();
    gps.fail_count++;

    ALOG_ERR(PSTR(D_LOG_CELLULAR "GNSS no fix"));
    return;
  }

  gps.fix_valid       = true;
  gps.last_fix_millis = millis();
  gps.fail_count      = 0;

  // -------------------------------------------------------------------------
  // Publish common location into the unified sensor/interface location state.
  // -------------------------------------------------------------------------
  sensorset_location_t& system_location = tkr_iSensors->system_location;

  system_location.isvalid             = true;
  system_location.source_id           = LOCATION_SOURCE_SIM7000G_GNSS;
  system_location.fix_type            = (uint8_t)fix_mode;
  system_location.updated_millis      = gps.last_fix_millis;
  system_location.fix_age_ms          = 0;
  system_location.is_stale            = false;

  system_location.latitude            = latitude;
  system_location.longitude           = longitude;
  system_location.speed               = speed;
  system_location.altitude            = altitude;
  system_location.course              = course;

  // Accuracy/precision.
  system_location.accuracy            = accuracy;
  system_location.accuracy_position   = accuracy_position;
  system_location.accuracy_vertical   = accuracy_vertical;
  system_location.hpa_raw             = (int16_t)hpa;
  system_location.vpa_raw             = (int16_t)vpa;

  // Do not map HPA/VPA into HDOP/VDOP. They are not dilution-of-precision values.
  system_location.hdop                = 0.0f;
  system_location.vdop                = 0.0f;

  system_location.satellites_used     = (uint8_t)usat;
  system_location.satellites_view     = (uint8_t)vsat;
  system_location.satellites_glonass  = (uint8_t)usat_glonass;
  system_location.cno_max             = (uint8_t)cno_max;

  system_location.year                = (uint16_t)year;
  system_location.month               = (uint8_t)month;
  system_location.day                 = (uint8_t)day;
  system_location.hour                = (uint8_t)hour;
  system_location.minute              = (uint8_t)minute;
  system_location.second              = (uint8_t)second;
  system_location.time_valid          = true;

  system_location.utc_time_secs = tkr_time->DateTime_UTC_ToEpochSeconds(
    system_location.year,
    system_location.month,
    system_location.day,
    system_location.hour,
    system_location.minute,
    system_location.second
  );

  
  ALOG_INF(
    PSTR(D_LOG_CELLULAR "lat=%d.%d lon=%d.%d pos=%dcm acc=%dcm v_acc=%dcm sat=%u/%u"),
    FLOAT_N(system_location.latitude),FLOAT_D(system_location.latitude),
    FLOAT_N(system_location.longitude),FLOAT_D(system_location.longitude),
    (int)(system_location.accuracy * 100.0f),
    (int)(system_location.accuracy_position * 100.0f),
    (int)(system_location.accuracy_vertical * 100.0f),
    system_location.satellites_used,
    system_location.satellites_view
  );



  #ifdef ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
  const uint16_t minimum_update_seconds = 0; // always
  tkr_time->SystemTime_Update(
    system_location.year,
    system_location.month,
    system_location.day,
    system_location.hour,
    system_location.minute,
    system_location.second,
    minimum_update_seconds
  );
  #endif
}
































































// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM7000G::SMS_GPSLocation()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
//   char convf_lat[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
//   char convf_lon[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
//   char convf_fix[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.accuracy,2,convf_fix);
  
//   sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
//   sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}


// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM7000G::SMS_GPSLocationAuto()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
//   mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("AutoSend %d secs\n"),  smsauto_gps_messages.rate_seconds);
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}





// void mSIM7000G::SMS_GPSLocation()
// {
//   if(modem)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
//     // --------TESTING SENDING SMS--------

//     // https://www.google.com/maps/dir//54.505,-6.299
//     // https://www.google.com/maps/dir//54.505044,-6.298891

//     char convf_lat[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
//     char convf_lon[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
//     char convf_fix[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.accuracy,2,convf_fix);

//     uint16_t buflen = 0;
//     char     buffer[STANDARD_SMS_CHAR_LENGTH] = {0};

// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
//       PSTR(
//         "Batt %d%% %dmV\n"
//         "WP  %d (%dm)\n"
//         "PKT Age  %d\n"
//         "Accuracy %s m\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       tkr_mavlink->pkt.battery_status.data.battery_remaining,
//       tkr_mavlink->pkt.battery_status.data.current_consumed,

//       tkr_mavlink->pkt.mission_current.data.seq,
//       tkr_mavlink->pkt.nav_controller_output.data.wp_dist,

//       millis()-tkr_mavlink->pkt.tSaved_Last_Response,
//       convf_fix,
//       convf_lat, 
//       convf_lon
//     );
//     // buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     //   PSTR(
//     //     "Accuracy %s m\n"
//     //     "https://www.google.com/maps/dir//%s,%s"
//     //   ), 
//     //   convf_fix,
//     //   convf_lat, 
//     //   convf_lon
//     // );
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferA%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//     /**
//      * @brief MAVLink Data
//      **/    
//     float mavlink_lat = tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
//     float mavlink_lon = tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;


//     char convf_lat2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
//     char convf_lon2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lat2 \"%s\""),convf_lat2);
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lon2 \"%s\""), convf_lon2);
// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//       PSTR(
//         "\n"
//         "MAV\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       convf_lat2, 
//       convf_lon2
//     );
//     #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferB%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     ALOG_INF(PSTR(D_LOG_CELLULAR "buffer \"%s\""), buffer);
    
// // DEBUG_LINE_HERE;
//     String res;
//     res = modem->sendSMS(SMS_TARGET, String(buffer));
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

// // DEBUG_LINE_HERE;
//   }
// }

void mSIM7000G::SMS_Send_TimedHeartbeat()
{
  DEBUG_LINE_HERE;
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

  DEBUG_LINE_HERE;
    char convf_lat[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.accuracy,2,convf_fix);

  DEBUG_LINE_HERE;
    uint16_t buflen = 0;
    char     buffer[STANDARD_SMS_CHAR_LENGTH];

  DEBUG_LINE_HERE;
    buflen += snprintf_P(buffer+buflen, sizeof(buffer),
      PSTR(
        // "Battery  %d mV\n"
        // "Battery  %d mA\n"
        // "Mission  %d (%dm)\n"
        // "PKT Age  %d\n"
        "Timed Heartbeat\n"
        "Accuracy %s m\n"
        "\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      // tkr_mavlink->pkt.battery_status.data.battery_remaining,
      // tkr_mavlink->pkt.battery_status.data.current_consumed,
      // tkr_mavlink->pkt.mission_current.data.seq,
      // tkr_mavlink->pkt.nav_controller_output.data.wp_dist,
      // millis()-tkr_mavlink->pkt.tSaved_Last_Response,
      convf_fix,
      convf_lat, 
      convf_lon
    );

  DEBUG_LINE_HERE;
    #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    /**
     * @brief MAVLink Data
     **/    
    char convf_lat2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(tkr_mavlink->pkt.gps_raw_int.data.lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
    char convf_lon2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(tkr_mavlink->pkt.gps_raw_int.data.lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

    // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
    //   PSTR(
    //     "\n"
    //     "MAV\n"
    //     "https://www.google.com/maps/dir//%s,%s"
    //   ), 
    //   convf_lat2, 
    //   convf_lon2
    // );
    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "buffer  %s"),buffer);
    
  DEBUG_LINE_HERE;
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");
  DEBUG_LINE_HERE;

  }
}

#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


#endif // header