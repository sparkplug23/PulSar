/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR



#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

void mCellular::GPS_Enable()
{
  if(modem)
  {
    // Set SIM7000G GPIO4 LOW ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,1");
    if(modem->waitResponse(10000L) != 1) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,1 false "));
    }

    modem->enableGPS();
    gps.enabled = true;
  }
}


void mCellular::GPS_Disable()
{
  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,0");
  if (modem->waitResponse(10000L) != 1) {
    ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,0 false "));
  }
  modem->disableGPS();
  gps.enabled = false;
}


void mCellular::ModemUpdate_GPS()
{
  if(mTime::TimeReached(&gps.tReached_Update, 1000)) // Not sending any faster so no point reading it faster
  {
    if(gps.enabled)
    {
      
      if(modem->getGPS_Detailed(
        &gps.latitude, 
        &gps.longitude, 
        &gps.speed, 
        &gps.altitude, 
        &gps.vsat, 
        &gps.usat, 
        &gps.accuracy, 
        &gps.year, 
        &gps.month, 
        &gps.day, 
        &gps.hour, 
        &gps.minute, 
        &gps.second, 
        &gps.course, 
        &gps.FixMode, 
        &gps.accuracy_position, 
        &gps.accuracy_vertical, 
        &gps.usat_glonass, 
        &gps.cno_max, 
        &gps.HPA, 
        &gps.VPA
      )) 
      {
        // ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100));
        
        #ifdef USE_MODULE_SENSORS_GPS_MODEM
        pCONT_gps->readings.update_seconds = millis();
        pCONT_gps->location.latitude  = gps.latitude;
        pCONT_gps->location.longitude = gps.longitude;
        pCONT_gps->location.speed = gps.speed;
        pCONT_gps->location.altitude = gps.altitude;
        pCONT_gps->location.accuracy = gps.accuracy;
        #endif // USE_MODULE_SENSORS_GPS_MODEM

        #ifdef ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
        uint16_t minimum_update_seconds = 0; //always
        pCONT_time->SystemTime_Update(gps.year, gps.month, gps.day, gps.hour, gps.minute, gps.second, minimum_update_seconds);
        #endif
      
      }else{
        ALOG_ERR(PSTR(D_LOG_CELLULAR "GPS NO FIX"));
        // GPS_Enable(); //need a backoff timer but otherwise should force it
      }

            
        
    }
  }

}




// https://www.google.com/maps?ll=xx.x,-x.xx
void mCellular::SMS_GPSLocation()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}


// https://www.google.com/maps?ll=xx.x,-x.xx
void mCellular::SMS_GPSLocationAuto()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("AutoSend %d secs\n"),  smsauto_gps_messages.rate_seconds);
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



/**
 * @brief New method that takes in SMS formatted as "Command:Key:Value" so test case is Command:SMS_GPS_Auto:1 or Command:SMS_GPS_Something:"string", ie key is assumed string but value must have its type added, "string", number, [array]
 * 
 * @param sms_command 
 */
void mCellular::SMS_CommandIntoJSONCommand(char* sms_command)
{

  /**
   * @brief SMS message will contain the repeat rate, and its handled in another call (in seconds)
   * 
   */
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_command >>%s<<"), sms_command);

  const char* delims = ":"; //space + , "GPSAuto 123" where the space splits the command and number
  char* tok = strtok(sms_command, delims); // I believe all delims are replaced by NULL?

  if(tok)
  {  
    // for (int i=0;i<5;i++)
    // {
    //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
    //   tok = strtok(NULL, delims);
    // }


    // tok = strtok(NULL, delims); // "Command" skip
    
    tok = strtok(NULL, delims); // Consume first split string and advance token (i.e. skip Command)
    char key[100];
    snprintf(key, sizeof(key), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "key=%s"),key);

    tok = strtok(NULL, delims);     
    char value[100];
    snprintf(value, sizeof(value), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "value=%s"),value);

    // Read into local
    D_DATA_BUFFER_CLEAR();

    sprintf(data_buffer.payload.ctr, "{\"%s\":%s}", key, value);

    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

    ALOG_COM( PSTR(DEBUG_INSERT_PAGE_BREAK  "SMS->JsonCommandBuffer = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);

    pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

    ALOG_INF(PSTR(D_LOG_CELLULAR "JsonCommand Finished"));

  }

    // tok




}




void mCellular::SMS_BatteryDetailed()
{

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending Battery"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



// void mCellular::SMS_GPSLocation()
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
//       pCONT_mavlink->pkt.battery_status.data.battery_remaining,
//       pCONT_mavlink->pkt.battery_status.data.current_consumed,

//       pCONT_mavlink->pkt.mission_current.data.seq,
//       pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,

//       millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
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
//     float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
//     float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;


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

void mCellular::SMS_Send_TimedHeartbeat()
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
    mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.accuracy,2,convf_fix);

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
      // pCONT_mavlink->pkt.battery_status.data.battery_remaining,
      // pCONT_mavlink->pkt.battery_status.data.current_consumed,
      // pCONT_mavlink->pkt.mission_current.data.seq,
      // pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,
      // millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
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
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
    char convf_lon2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

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

#endif // USE_MODULE_NETWORK_CELLULAR