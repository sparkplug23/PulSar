#include "_mGPS_Serial.h"

// For clarity, moving json construction into own file

#ifdef USE_MODULE_SENSORS_GPS_SERIAL


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/




/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Minimal(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JBI->Start();  

  
      JBI->Add("packets", stats.packets_received); 
      JBI->Add("last", millis()-stats.last_message_received_time); 

  // #ifdef ENABLE_GPS_PARSER_NMEA
    JBI->Object_Start("Location");
      JBI->Add("latitudeL", fix_valid.latitudeL()); 
      JBI->Add("latitude", fix_valid.latitude());
      JBI->Add("longitudeL", fix_valid.longitudeL());
      JBI->Add("longitude", fix_valid.longitude());
    JBI->Object_End();

    JBI->Object_Start("Altitude");
      JBI->Add("altitude_cm", fix_valid.altitude_cm()); 
      JBI->Add("altitude", fix_valid.altitude());
      JBI->Add("altitude_ft", fix_valid.altitude_ft());
    JBI->Object_End();


    JBI->Object_Start("Speed");
      JBI->Add("speed_mkn", fix_valid.speed_mkn()); 
      JBI->Add("speed", fix_valid.speed());
      JBI->Add("speed_kph", fix_valid.speed_kph());
      JBI->Add("speed_metersph", fix_valid.speed_metersph());
      JBI->Add("speed_mph", fix_valid.speed_mph());
    JBI->Object_End();

    // JBI->Object_Start("Heading");
    //   JBI->Add("heading_cd", fix_valid.heading_cd()); 
    //   JBI->Add("heading", fix_valid.heading());
    // JBI->Object_End();

    // JBI->Object_Start("geoidHt");
    //   JBI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
    //   JBI->Add("geoidHeight", fix_valid.geoidHeight());
    // JBI->Object_End();

    // JBI->Add("satellites", fix_valid.satellites); 

    // JBI->Object_Start("Dilution");
    //   JBI->Add("hdop", fix_valid.hdop); 
    //   JBI->Add("vdop", fix_valid.vdop);
    //   JBI->Add("pdop", fix_valid.pdop);
    //   JBI->Add("lat_err", fix_valid.lat_err());
    //   JBI->Add("lon_err", fix_valid.lon_err());
    //   JBI->Add("alt_err", fix_valid.alt_err());
    //   JBI->Add("spd_err", fix_valid.spd_err());
    //   JBI->Add("hdg_err", fix_valid.hdg_err());
    //   JBI->Add("spd_err", fix_valid.spd_err());
    //   JBI->Add("time_err", fix_valid.time_err());
    // JBI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA
  
  return JBI->End();

}


/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
// uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All(uint8_t json_level, bool json_appending){

//   char buffer[30];
  
//   JBI->Start();  

// //   #ifdef ENABLE_GPS_PARSER_NMEA
// //     JBI->Object_Start("Location");
// //       JBI->Add("latitudeL", fix_valid.latitudeL()); 
// //       JBI->Add("latitude", fix_valid.latitude());
// //       JBI->Add("longitudeL", fix_valid.longitudeL());
// //       JBI->Add("longitude", fix_valid.longitude());
// //     JBI->Object_End();

// //     JBI->Object_Start("Altitude");
// //       JBI->Add("altitude_cm", fix_valid.altitude_cm()); 
// //       JBI->Add("altitude", fix_valid.altitude());
// //       JBI->Add("altitude_ft", fix_valid.altitude_ft());
// //     JBI->Object_End();

// //     JBI->Object_Start("Speed");
// //       JBI->Add("speed_mkn", fix_valid.speed_mkn()); 
// //       JBI->Add("speed", fix_valid.speed());
// //       JBI->Add("speed_kph", fix_valid.speed_kph());
// //       JBI->Add("speed_metersph", fix_valid.speed_metersph());
// //       JBI->Add("speed_mph", fix_valid.speed_mph());
// //     JBI->Object_End();

// //     JBI->Object_Start("Heading");
// //       JBI->Add("heading_cd", fix_valid.heading_cd()); 
// //       JBI->Add("heading", fix_valid.heading());
// //     JBI->Object_End();

// //     JBI->Object_Start("geoidHt");
// //       JBI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
// //       JBI->Add("geoidHeight", fix_valid.geoidHeight());
// //     JBI->Object_End();

// //     JBI->Add("satellites", fix_valid.satellites); 

// //     JBI->Object_Start("Dilution");
// //       JBI->Add("hdop", fix_valid.hdop); 
// //       JBI->Add("vdop", fix_valid.vdop);
// //       JBI->Add("pdop", fix_valid.pdop);
// //       JBI->Add("lat_err", fix_valid.lat_err());
// //       JBI->Add("lon_err", fix_valid.lon_err());
// //       JBI->Add("alt_err", fix_valid.alt_err());
// //       JBI->Add("spd_err", fix_valid.spd_err());
// //       JBI->Add("hdg_err", fix_valid.hdg_err());
// //       JBI->Add("spd_err", fix_valid.spd_err());
// //       JBI->Add("time_err", fix_valid.time_err());
// //     JBI->Object_End();




// //   #endif// ENABLE_GPS_PARSER_NMEA

// //     /*
  
// //   #ifdef GPS_FIX_LOCATION_DMS
// //     DMS_t latitudeDMS;
// //     DMS_t longitudeDMS;
// //   #endif

// // */


//     // JBI->Add_P(PM_TIME_MS, animation.time_ms);
//   return JBI->End();

// }


uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All1(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  // ----------------------------------------------------------
  // Status
  // ----------------------------------------------------------
  JBI->Object_Start("Status");
    JBI->Add("status", (uint8_t)fix_valid.status);

    #ifdef GPS_FIX_DATE
    JBI->Add("valid_date", fix_valid.valid.date);
    #endif

    #ifdef GPS_FIX_TIME
    JBI->Add("valid_time", fix_valid.valid.time);
    #endif

    JBI->Add("valid_status", fix_valid.valid.status);

    #if defined(GPS_FIX_LOCATION) || defined(GPS_FIX_LOCATION_DMS)
    JBI->Add("valid_location", fix_valid.valid.location);
    #endif

    #ifdef GPS_FIX_ALTITUDE
    JBI->Add("valid_altitude", fix_valid.valid.altitude);
    #endif

    #ifdef GPS_FIX_SPEED
    JBI->Add("valid_speed", fix_valid.valid.speed);
    #endif

    #ifdef GPS_FIX_VELNED
    JBI->Add("valid_velned", fix_valid.valid.velned);
    #endif

    #ifdef GPS_FIX_HEADING
    JBI->Add("valid_heading", fix_valid.valid.heading);
    #endif

    #ifdef GPS_FIX_SATELLITES
    JBI->Add("valid_satellites", fix_valid.valid.satellites);
    #endif

    #ifdef GPS_FIX_HDOP
    JBI->Add("valid_hdop", fix_valid.valid.hdop);
    #endif

    #ifdef GPS_FIX_VDOP
    JBI->Add("valid_vdop", fix_valid.valid.vdop);
    #endif

    #ifdef GPS_FIX_PDOP
    JBI->Add("valid_pdop", fix_valid.valid.pdop);
    #endif

    #ifdef GPS_FIX_LAT_ERR
    JBI->Add("valid_lat_err", fix_valid.valid.lat_err);
    #endif

    #ifdef GPS_FIX_LON_ERR
    JBI->Add("valid_lon_err", fix_valid.valid.lon_err);
    #endif

    #ifdef GPS_FIX_ALT_ERR
    JBI->Add("valid_alt_err", fix_valid.valid.alt_err);
    #endif

    #ifdef GPS_FIX_SPD_ERR
    JBI->Add("valid_spd_err", fix_valid.valid.spd_err);
    #endif

    #ifdef GPS_FIX_HDG_ERR
    JBI->Add("valid_hdg_err", fix_valid.valid.hdg_err);
    #endif

    #ifdef GPS_FIX_TIME_ERR
    JBI->Add("valid_time_err", fix_valid.valid.time_err);
    #endif

    #ifdef GPS_FIX_GEOID_HEIGHT
    JBI->Add("valid_geoidHeight", fix_valid.valid.geoidHeight);
    #endif
  JBI->Object_End();

  // ----------------------------------------------------------
  // Date / Time
  // ----------------------------------------------------------
  #if defined(GPS_FIX_DATE) || defined(GPS_FIX_TIME)
  JBI->Object_Start("DateTime");
    #ifdef GPS_FIX_DATE
    JBI->Add("year",  fix_valid.dateTime.year);
    JBI->Add("month", fix_valid.dateTime.month);
    JBI->Add("date",  fix_valid.dateTime.date);
    #endif

    #ifdef GPS_FIX_TIME
    JBI->Add("hours",      fix_valid.dateTime.hours);
    JBI->Add("minutes",    fix_valid.dateTime.minutes);
    JBI->Add("seconds",    fix_valid.dateTime.seconds);
    JBI->Add("centisecs",  fix_valid.dateTime_cs);
    JBI->Add("millis",     fix_valid.dateTime_ms());
    JBI->Add("micros",     fix_valid.dateTime_us());
    #endif
  JBI->Object_End();
  #endif

  return JBI->End();
}



uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All2(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  // ----------------------------------------------------------
  // Location
  // ----------------------------------------------------------
  #ifdef GPS_FIX_LOCATION
  JBI->Object_Start("Location");
    JBI->Add("latitudeL",  fix_valid.latitudeL());
    JBI->Add("latitude",   fix_valid.latitude());
    JBI->Add("longitudeL", fix_valid.longitudeL());
    JBI->Add("longitude",  fix_valid.longitude());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Location DMS
  // ----------------------------------------------------------
  #ifdef GPS_FIX_LOCATION_DMS
  JBI->Object_Start("LocationDMS");
    JBI->Add("lat_deg",     fix_valid.latitudeDMS.degrees);
    JBI->Add("lat_min",     fix_valid.latitudeDMS.minutes);
    JBI->Add("lat_sec",     fix_valid.latitudeDMS.secondsF());
    JBI->Add("lat_hemisphere", (uint8_t)fix_valid.latitudeDMS.hemisphere);

    JBI->Add("lon_deg",     fix_valid.longitudeDMS.degrees);
    JBI->Add("lon_min",     fix_valid.longitudeDMS.minutes);
    JBI->Add("lon_sec",     fix_valid.longitudeDMS.secondsF());
    JBI->Add("lon_hemisphere", (uint8_t)fix_valid.longitudeDMS.hemisphere);
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Altitude
  // ----------------------------------------------------------
  #ifdef GPS_FIX_ALTITUDE
  JBI->Object_Start("Altitude");
    JBI->Add("alt_whole",      fix_valid.alt.whole);
    JBI->Add("alt_frac",       fix_valid.alt.frac);
    JBI->Add("altitude_cm",    fix_valid.altitude_cm());
    JBI->Add("altitude",       fix_valid.altitude());
    JBI->Add("altitude_ft",    fix_valid.altitude_ft());
  JBI->Object_End();
  #endif

  return JBI->End();
}


uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All3(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  // ----------------------------------------------------------
  // Speed
  // ----------------------------------------------------------
  #ifdef GPS_FIX_SPEED
  JBI->Object_Start("Speed");
    JBI->Add("spd_whole",       fix_valid.spd.whole);
    JBI->Add("spd_frac",        fix_valid.spd.frac);
    JBI->Add("speed_mkn",       fix_valid.speed_mkn());
    JBI->Add("speed",           fix_valid.speed());
    JBI->Add("speed_kph",       fix_valid.speed_kph());
    JBI->Add("speed_metersph",  fix_valid.speed_metersph());
    JBI->Add("speed_mph",       fix_valid.speed_mph());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Heading
  // ----------------------------------------------------------
  #ifdef GPS_FIX_HEADING
  JBI->Object_Start("Heading");
    JBI->Add("hdg_whole",   fix_valid.hdg.whole);
    JBI->Add("hdg_frac",    fix_valid.hdg.frac);
    JBI->Add("heading_cd",  fix_valid.heading_cd());
    JBI->Add("heading",     fix_valid.heading());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Velocity NED
  // ----------------------------------------------------------
  #ifdef GPS_FIX_VELNED
  JBI->Object_Start("VelocityNED");
    JBI->Add("north_cmps",  fix_valid.velocity_north);
    JBI->Add("east_cmps",   fix_valid.velocity_east);
    JBI->Add("down_cmps",   fix_valid.velocity_down);
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // DOP
  // ----------------------------------------------------------
  #if defined(GPS_FIX_HDOP) || defined(GPS_FIX_VDOP) || defined(GPS_FIX_PDOP)
  JBI->Object_Start("Dilution");
    #ifdef GPS_FIX_HDOP
    JBI->Add("hdop", fix_valid.hdop);
    #endif
    #ifdef GPS_FIX_VDOP
    JBI->Add("vdop", fix_valid.vdop);
    #endif
    #ifdef GPS_FIX_PDOP
    JBI->Add("pdop", fix_valid.pdop);
    #endif
  JBI->Object_End();
  #endif

  return JBI->End();
}





uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All4(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  // ----------------------------------------------------------
  // Errors
  // ----------------------------------------------------------
  #if defined(GPS_FIX_LAT_ERR) || defined(GPS_FIX_LON_ERR) || defined(GPS_FIX_ALT_ERR) || defined(GPS_FIX_SPD_ERR) || defined(GPS_FIX_HDG_ERR) || defined(GPS_FIX_TIME_ERR)
  JBI->Object_Start("Errors");
    #ifdef GPS_FIX_LAT_ERR
    JBI->Add("lat_err_cm", fix_valid.lat_err_cm);
    JBI->Add("lat_err",    fix_valid.lat_err());
    #endif

    #ifdef GPS_FIX_LON_ERR
    JBI->Add("lon_err_cm", fix_valid.lon_err_cm);
    JBI->Add("lon_err",    fix_valid.lon_err());
    #endif

    #ifdef GPS_FIX_ALT_ERR
    JBI->Add("alt_err_cm", fix_valid.alt_err_cm);
    JBI->Add("alt_err",    fix_valid.alt_err());
    #endif

    #ifdef GPS_FIX_SPD_ERR
    JBI->Add("spd_err_mmps", fix_valid.spd_err_mmps);
    JBI->Add("spd_err",      fix_valid.spd_err());
    #endif

    #ifdef GPS_FIX_HDG_ERR
    JBI->Add("hdg_errE5", fix_valid.hdg_errE5);
    JBI->Add("hdg_err",   fix_valid.hdg_err());
    #endif

    #ifdef GPS_FIX_TIME_ERR
    JBI->Add("time_err_ns", fix_valid.time_err_ns);
    JBI->Add("time_err",    fix_valid.time_err());
    #endif
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Geoid height
  // ----------------------------------------------------------
  #ifdef GPS_FIX_GEOID_HEIGHT
  JBI->Object_Start("GeoidHeight");
    JBI->Add("geoid_whole",      fix_valid.geoidHt.whole);
    JBI->Add("geoid_frac",       fix_valid.geoidHt.frac);
    JBI->Add("geoidHeight_cm",   fix_valid.geoidHeight_cm());
    JBI->Add("geoidHeight",      fix_valid.geoidHeight());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Satellites
  // ----------------------------------------------------------
  #ifdef GPS_FIX_SATELLITES
  JBI->Add("satellites", fix_valid.satellites);
  #endif

  return JBI->End();
}

uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All(uint8_t json_level, bool json_appending)
{
  char buffer[30];

  JBI->Start();

  // ----------------------------------------------------------
  // Status
  // ----------------------------------------------------------
  JBI->Object_Start("Status");
    JBI->Add("status", (uint8_t)fix_valid.status);

    #ifdef GPS_FIX_DATE
    JBI->Add("valid_date", fix_valid.valid.date);
    #endif

    #ifdef GPS_FIX_TIME
    JBI->Add("valid_time", fix_valid.valid.time);
    #endif

    JBI->Add("valid_status", fix_valid.valid.status);

    #if defined(GPS_FIX_LOCATION) || defined(GPS_FIX_LOCATION_DMS)
    JBI->Add("valid_location", fix_valid.valid.location);
    #endif

    #ifdef GPS_FIX_ALTITUDE
    JBI->Add("valid_altitude", fix_valid.valid.altitude);
    #endif

    #ifdef GPS_FIX_SPEED
    JBI->Add("valid_speed", fix_valid.valid.speed);
    #endif

    #ifdef GPS_FIX_VELNED
    JBI->Add("valid_velned", fix_valid.valid.velned);
    #endif

    #ifdef GPS_FIX_HEADING
    JBI->Add("valid_heading", fix_valid.valid.heading);
    #endif

    #ifdef GPS_FIX_SATELLITES
    JBI->Add("valid_satellites", fix_valid.valid.satellites);
    #endif

    #ifdef GPS_FIX_HDOP
    JBI->Add("valid_hdop", fix_valid.valid.hdop);
    #endif

    #ifdef GPS_FIX_VDOP
    JBI->Add("valid_vdop", fix_valid.valid.vdop);
    #endif

    #ifdef GPS_FIX_PDOP
    JBI->Add("valid_pdop", fix_valid.valid.pdop);
    #endif

    #ifdef GPS_FIX_LAT_ERR
    JBI->Add("valid_lat_err", fix_valid.valid.lat_err);
    #endif

    #ifdef GPS_FIX_LON_ERR
    JBI->Add("valid_lon_err", fix_valid.valid.lon_err);
    #endif

    #ifdef GPS_FIX_ALT_ERR
    JBI->Add("valid_alt_err", fix_valid.valid.alt_err);
    #endif

    #ifdef GPS_FIX_SPD_ERR
    JBI->Add("valid_spd_err", fix_valid.valid.spd_err);
    #endif

    #ifdef GPS_FIX_HDG_ERR
    JBI->Add("valid_hdg_err", fix_valid.valid.hdg_err);
    #endif

    #ifdef GPS_FIX_TIME_ERR
    JBI->Add("valid_time_err", fix_valid.valid.time_err);
    #endif

    #ifdef GPS_FIX_GEOID_HEIGHT
    JBI->Add("valid_geoidHeight", fix_valid.valid.geoidHeight);
    #endif
  JBI->Object_End();

  // ----------------------------------------------------------
  // Date / Time
  // ----------------------------------------------------------
  #if defined(GPS_FIX_DATE) || defined(GPS_FIX_TIME)
  JBI->Object_Start("DateTime");
    #ifdef GPS_FIX_DATE
    JBI->Add("year",  fix_valid.dateTime.year);
    JBI->Add("month", fix_valid.dateTime.month);
    JBI->Add("date",  fix_valid.dateTime.date);
    #endif

    #ifdef GPS_FIX_TIME
    JBI->Add("hours",      fix_valid.dateTime.hours);
    JBI->Add("minutes",    fix_valid.dateTime.minutes);
    JBI->Add("seconds",    fix_valid.dateTime.seconds);
    JBI->Add("centisecs",  fix_valid.dateTime_cs);
    JBI->Add("millis",     fix_valid.dateTime_ms());
    JBI->Add("micros",     fix_valid.dateTime_us());
    #endif
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Location
  // ----------------------------------------------------------
  #ifdef GPS_FIX_LOCATION
  JBI->Object_Start("Location");
    JBI->Add("latitudeL",  fix_valid.latitudeL());
    JBI->Add("latitude",   fix_valid.latitude());
    JBI->Add("longitudeL", fix_valid.longitudeL());
    JBI->Add("longitude",  fix_valid.longitude());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Location DMS
  // ----------------------------------------------------------
  #ifdef GPS_FIX_LOCATION_DMS
  JBI->Object_Start("LocationDMS");
    JBI->Add("lat_deg",     fix_valid.latitudeDMS.degrees);
    JBI->Add("lat_min",     fix_valid.latitudeDMS.minutes);
    JBI->Add("lat_sec",     fix_valid.latitudeDMS.secondsF());
    JBI->Add("lat_hemisphere", (char)fix_valid.latitudeDMS.hemisphere);

    JBI->Add("lon_deg",     fix_valid.longitudeDMS.degrees);
    JBI->Add("lon_min",     fix_valid.longitudeDMS.minutes);
    JBI->Add("lon_sec",     fix_valid.longitudeDMS.secondsF());
    JBI->Add("lon_hemisphere", (char)fix_valid.longitudeDMS.hemisphere);
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Altitude
  // ----------------------------------------------------------
  #ifdef GPS_FIX_ALTITUDE
  JBI->Object_Start("Altitude");
    JBI->Add("alt_whole",      fix_valid.alt.whole);
    JBI->Add("alt_frac",       fix_valid.alt.frac);
    JBI->Add("altitude_cm",    fix_valid.altitude_cm());
    JBI->Add("altitude",       fix_valid.altitude());
    JBI->Add("altitude_ft",    fix_valid.altitude_ft());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Speed
  // ----------------------------------------------------------
  #ifdef GPS_FIX_SPEED
  JBI->Object_Start("Speed");
    JBI->Add("spd_whole",       fix_valid.spd.whole);
    JBI->Add("spd_frac",        fix_valid.spd.frac);
    JBI->Add("speed_mkn",       fix_valid.speed_mkn());
    JBI->Add("speed",           fix_valid.speed());
    JBI->Add("speed_kph",       fix_valid.speed_kph());
    JBI->Add("speed_metersph",  fix_valid.speed_metersph());
    JBI->Add("speed_mph",       fix_valid.speed_mph());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Heading
  // ----------------------------------------------------------
  #ifdef GPS_FIX_HEADING
  JBI->Object_Start("Heading");
    JBI->Add("hdg_whole",   fix_valid.hdg.whole);
    JBI->Add("hdg_frac",    fix_valid.hdg.frac);
    JBI->Add("heading_cd",  fix_valid.heading_cd());
    JBI->Add("heading",     fix_valid.heading());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Velocity NED
  // ----------------------------------------------------------
  #ifdef GPS_FIX_VELNED
  JBI->Object_Start("VelocityNED");
    JBI->Add("north_cmps",  fix_valid.velocity_north);
    JBI->Add("east_cmps",   fix_valid.velocity_east);
    JBI->Add("down_cmps",   fix_valid.velocity_down);
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // DOP
  // ----------------------------------------------------------
  #if defined(GPS_FIX_HDOP) || defined(GPS_FIX_VDOP) || defined(GPS_FIX_PDOP)
  JBI->Object_Start("Dilution");
    #ifdef GPS_FIX_HDOP
    JBI->Add("hdop", fix_valid.hdop);
    #endif
    #ifdef GPS_FIX_VDOP
    JBI->Add("vdop", fix_valid.vdop);
    #endif
    #ifdef GPS_FIX_PDOP
    JBI->Add("pdop", fix_valid.pdop);
    #endif
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Errors
  // ----------------------------------------------------------
  #if defined(GPS_FIX_LAT_ERR) || defined(GPS_FIX_LON_ERR) || defined(GPS_FIX_ALT_ERR) || defined(GPS_FIX_SPD_ERR) || defined(GPS_FIX_HDG_ERR) || defined(GPS_FIX_TIME_ERR)
  JBI->Object_Start("Errors");
    #ifdef GPS_FIX_LAT_ERR
    JBI->Add("lat_err_cm", fix_valid.lat_err_cm);
    JBI->Add("lat_err",    fix_valid.lat_err());
    #endif

    #ifdef GPS_FIX_LON_ERR
    JBI->Add("lon_err_cm", fix_valid.lon_err_cm);
    JBI->Add("lon_err",    fix_valid.lon_err());
    #endif

    #ifdef GPS_FIX_ALT_ERR
    JBI->Add("alt_err_cm", fix_valid.alt_err_cm);
    JBI->Add("alt_err",    fix_valid.alt_err());
    #endif

    #ifdef GPS_FIX_SPD_ERR
    JBI->Add("spd_err_mmps", fix_valid.spd_err_mmps);
    JBI->Add("spd_err",      fix_valid.spd_err());
    #endif

    #ifdef GPS_FIX_HDG_ERR
    JBI->Add("hdg_errE5", fix_valid.hdg_errE5);
    JBI->Add("hdg_err",   fix_valid.hdg_err());
    #endif

    #ifdef GPS_FIX_TIME_ERR
    JBI->Add("time_err_ns", fix_valid.time_err_ns);
    JBI->Add("time_err",    fix_valid.time_err());
    #endif
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Geoid height
  // ----------------------------------------------------------
  #ifdef GPS_FIX_GEOID_HEIGHT
  JBI->Object_Start("GeoidHeight");
    JBI->Add("geoid_whole",      fix_valid.geoidHt.whole);
    JBI->Add("geoid_frac",       fix_valid.geoidHt.frac);
    JBI->Add("geoidHeight_cm",   fix_valid.geoidHeight_cm());
    JBI->Add("geoidHeight",      fix_valid.geoidHeight());
  JBI->Object_End();
  #endif

  // ----------------------------------------------------------
  // Satellites
  // ----------------------------------------------------------
  #ifdef GPS_FIX_SATELLITES
  JBI->Add("satellites", fix_valid.satellites);
  #endif

  return JBI->End();
}

//https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_MessageOverview.html

/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Micro(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JBI->Start();  

  // #ifdef ENABLE_GPS_PARSER_NMEA
  // JBI->Object_Start("SequenceNumber");
  //   JBI->Add("GPSCount", 0);
  //   JBI->Add("RSSCount", 0);
  // JBI->Object_End();

  // JBI->Object_Start("Quality");
  //   JBI->Add("Fix", fix_valid.status);
  //   JBI->Add("satellites", fix_valid.satellites); 
  //   JBI->Add("SatelleteCount", fix_valid.satellites);
  //   JBI->Add("SatelleteThreshold", 0); //minimal fix to be considered enough
  //   JBI->Add("UptimeSeconds", 0); 
  //   JBI->Add("DowntimeSeconds", 0); 
  // JBI->Object_End();

  // JBI->Object_Start("Time");
  //   JBI->Add("UTC", "0");
  //   JBI->Add("TimeOfDay", 0);
  //   JBI->Add("TimeOfDayMillis", gps_latest.milliseconds_of_day);
  //   JBI->Object_Start("Parts");
  //     JBI->Add("hours", fix_valid.dateTime.hours);
  //     JBI->Add("minutes", fix_valid.dateTime.minutes);
  //     JBI->Add("seconds", fix_valid.dateTime.seconds);
  //     JBI->Add("day", fix_valid.dateTime.day);
  //     JBI->Add("date", fix_valid.dateTime.date);
  //     JBI->Add("month", fix_valid.dateTime.month);
  //     JBI->Add("year", fix_valid.dateTime.year);
  //     JBI->Add("dateTime_us", fix_valid.dateTime_us());
  //     JBI->Add("dateTime_ms", fix_valid.dateTime_ms());
  //   JBI->Object_End();
  // JBI->Object_End();

  // JBI->Object_Start("Location");
  //   // JBI->Add("latitudeL", fix_valid.latitudeL()); 
  //   JBI->Add("latitude", fix_valid.latitude());
  //   // JBI->Add("longitudeL", fix_valid.longitudeL());
  //   JBI->Add("longitude", fix_valid.longitude());
  // JBI->Object_End();

  // JBI->Object_Start("Altitude");
  //   JBI->Add("hMSL_mm", 0);
  //   JBI->Add("height_mm", 0);
  //     JBI->Add("altitude_cm", fix_valid.altitude_cm()); 
  //     JBI->Add("altitude", fix_valid.altitude());
  //     JBI->Add("altitude_ft", fix_valid.altitude_ft());
  // JBI->Object_End();

  // JBI->Object_Start("Velocity");
  //   JBI->Add("speed3D", 0);
  //   JBI->Add("speed2D", 0);
  //     JBI->Add("speed_mkn", fix_valid.speed_mkn()); 
  //     JBI->Add("speed", fix_valid.speed());
  //     JBI->Add("speed_kph", fix_valid.speed_kph());
  //     JBI->Add("speed_metersph", fix_valid.speed_metersph());
  //     JBI->Add("speed_mph", fix_valid.speed_mph());
  // JBI->Object_End();

  // JBI->Object_Start("Velocity");

  //     JBI->Add("heading_cd", fix_valid.heading_cd()); 
  //     JBI->Add("heading", fix_valid.heading());

  // JBI->Object_End();

  //   JBI->Object_Start("geoidHt");
  //     JBI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
  //     JBI->Add("geoidHeight", fix_valid.geoidHeight());
  //   JBI->Object_End();

  //   JBI->Object_Start("Dilution");
  //     JBI->Add("hdop", fix_valid.hdop); 
  //     JBI->Add("vdop", fix_valid.vdop);
  //     JBI->Add("pdop", fix_valid.pdop);
  //     JBI->Add("lat_err", fix_valid.lat_err());
  //     JBI->Add("lon_err", fix_valid.lon_err());
  //     JBI->Add("alt_err", fix_valid.alt_err());
  //     JBI->Add("spd_err", fix_valid.spd_err());
  //     JBI->Add("hdg_err", fix_valid.hdg_err());
  //     JBI->Add("spd_err", fix_valid.spd_err());
  //     JBI->Add("time_err", fix_valid.time_err());
  //   JBI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA

  return JBI->End();

}

uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Debug(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JBI->Start();  

  // #ifdef ENABLE_GPS_PARSER_NMEA
  //   JBI->Object_Start("Millis");
  //     JBI->Add("GGA",nmea_parser->active_millis.GGA);
  //     JBI->Add("GLL",nmea_parser->active_millis.GLL);
  //     JBI->Add("GSA",nmea_parser->active_millis.GSA);
  //     JBI->Add("GST",nmea_parser->active_millis.GST);
  //     JBI->Add("GSV",nmea_parser->active_millis.GSV);
  //     JBI->Add("RMC",nmea_parser->active_millis.RMC);
  //     JBI->Add("VTG",nmea_parser->active_millis.VTG);
  //     JBI->Add("ZDA",nmea_parser->active_millis.ZDA);
  //   JBI->Object_End();

  //   JBI->Object_Start("Millis2");
  //     JBI->Add("GGA",millis()-nmea_parser->active_millis.GGA);
  //     JBI->Add("GLL",millis()-nmea_parser->active_millis.GLL);
  //     JBI->Add("GSA",millis()-nmea_parser->active_millis.GSA);
  //     JBI->Add("GST",millis()-nmea_parser->active_millis.GST);
  //     JBI->Add("GSV",millis()-nmea_parser->active_millis.GSV);
  //     JBI->Add("RMC",mTime::MillisElapsed(nmea_parser->active_millis.RMC));
  //     JBI->Add("VTG",millis()-nmea_parser->active_millis.VTG);
  //     JBI->Add("ZDA",millis()-nmea_parser->active_millis.ZDA);
  //   JBI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA


  //   JBI->Object_Start("UBX_Parsed_Millis");
  //     JBI->Add("status",millis()-gps.debug_millis_last_parsed.status);
  //     JBI->Add("posllh",millis()-gps.debug_millis_last_parsed.posllh);
  //     JBI->Add("pvt",millis()-gps.debug_millis_last_parsed.pvt);
  //     JBI->Add("dop",millis()-gps.debug_millis_last_parsed.dop);
  //     JBI->Add("velned",millis()-gps.debug_millis_last_parsed.velned);
  //     JBI->Add("timegps",mTime::MillisElapsed(gps.debug_millis_last_parsed.timegps));
  //     JBI->Add("timeutc",millis()-gps.debug_millis_last_parsed.timeutc);
  //     JBI->Add("svinfo",millis()-gps.debug_millis_last_parsed.svinfo);
  //   JBI->Object_End();




    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}


uint8_t mGPS_Serial::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS_Serial::ConstructJSON_Settings");

  char buffer[30];
  
  JBI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JBI->Add_P(PM_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JBI->Add_P(PM_HUE, rgbcct_controller.getHue360());
    // JBI->Add_P(PM_SAT, rgbcct_controller.getSat255());
    // JBI->Add_P(PM_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JBI->Add_P(PM_TIME, 1000);
    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}



uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Required(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS_Serial::ConstructJSON_Settings");

  char buffer[30];
  
  JBI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JBI->Add_P(PM_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JBI->Add_P(PM_HUE, rgbcct_controller.getHue360());
    // JBI->Add_P(PM_SAT, rgbcct_controller.getSat255());
    // JBI->Add_P(PM_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JBI->Add_P(PM_TIME, 1000);
    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}
















uint8_t mGPS_Serial::ConstructJSON_NAV_PVT(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS_Serial::ConstructJSON_Settings");

  char buffer[30];
  
  JBI->Start();  

    JBI->Add("secs",fix_valid.dateTime.seconds);

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JBI->Add_P(PM_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JBI->Add_P(PM_HUE, rgbcct_controller.getHue360());
    // JBI->Add_P(PM_SAT, rgbcct_controller.getSat255());
    // JBI->Add_P(PM_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    // JBI->Add_P(PM_TIME, 1000);
    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}
  

#endif