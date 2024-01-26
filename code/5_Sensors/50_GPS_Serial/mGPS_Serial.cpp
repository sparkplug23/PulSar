/**
 * @file mGPS_Serial.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mGPS_Serial.h"

#ifdef USE_MODULE_SENSORS_GPS_SERIAL

const char* mGPS_Serial::PM_MODULE_SENSORS__GPS_SERIAL_CTR = D_MODULE_SENSORS__GPS_SERIAL__CTR;
const char* mGPS_Serial::PM_MODULE_SENSORS__GPS_SERIAL_FRIENDLY_CTR = D_MODULE_SENSORS__GPS_SERIAL__FRIENDLY_CTR;

// #ifdef USE_DEVFEATURE_UBLOX_GLOBAL
// static ubloxGPS ubx_parser(&Serial2);// = nullptr; // This parses the GPS characters
// #endif



class MyGPS : public ubloxGPS
{
public:

    enum
      {
        GETTING_STATUS, 
        GETTING_LEAP_SECONDS, 
        GETTING_UTC, 
        RUNNING
      }
        state NEOGPS_BF(8);

    MyGPS( Stream *device ) : ubloxGPS( device )
    {
      state = GETTING_STATUS;
    }

    //--------------------------

    void get_status()
    {
      static bool acquiring = false;

  DEBUG_LINE_HERE;
      if (fix().status == gps_fix::STATUS_NONE) {
  DEBUG_LINE_HERE;
        static uint32_t dotPrint;
        bool            requestNavStatus = false;

        if (!acquiring) {
          acquiring = true;
          dotPrint = millis();
          DEBUG_PORT.print( F("Acquiring...") );
          requestNavStatus = true;

        } else if (millis() - dotPrint > 1000UL) {
          dotPrint = millis();
          DEBUG_PORT << '.';

          static uint8_t requestPeriod;
          if ((++requestPeriod & 0x07) == 0)
            requestNavStatus = true;
        }

  DEBUG_LINE_HERE;
        if (requestNavStatus)
          // Turn on the UBX status message
          enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_STATUS );

  DEBUG_LINE_HERE;
      } else {
  DEBUG_LINE_HERE;
        if (acquiring)
          DEBUG_PORT << '\n';
        DEBUG_PORT << F("Acquired status: ") << (uint8_t) fix().status << '\n';

        #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
            defined(UBLOX_PARSE_TIMEGPS)

          if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
            DEBUG_PORT.println( F("enable TIMEGPS failed!") );

          state = GETTING_LEAP_SECONDS;
        #else
          start_running();
          state = RUNNING;
        #endif
      }
  DEBUG_LINE_HERE;
    } // get_status

    //--------------------------

    void get_leap_seconds()
    {
      #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
          defined(UBLOX_PARSE_TIMEGPS)

        if (GPSTime::leap_seconds != 0) {
          DEBUG_PORT << F("Acquired leap seconds: ") << GPSTime::leap_seconds << '\n';

          if (!disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
            DEBUG_PORT.println( F("disable TIMEGPS failed!") );

          #if defined(UBLOX_PARSE_TIMEUTC)
            if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
              DEBUG_PORT.println( F("enable TIMEUTC failed!") );
            state = GETTING_UTC;
          #else
            start_running();
          #endif
        }
      #endif

    } // get_leap_seconds

    //--------------------------

    void get_utc()
    {
      #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
          defined(UBLOX_PARSE_TIMEUTC)

        lock();
          bool            safe = is_safe();
          NeoGPS::clock_t sow  = GPSTime::start_of_week();
          NeoGPS::time_t  utc  = fix().dateTime;
        unlock();

        if (safe && (sow != 0)) {
          DEBUG_PORT << F("Acquired UTC: ") << utc << '\n';
          DEBUG_PORT << F("Acquired Start-of-Week: ") << sow << '\n';

          start_running();
        }
      #endif

    } // get_utc

    //--------------------------

    void start_running()
    {
      pCONT_gps->enableUBX();

      state = RUNNING;
      trace_header( DEBUG_PORT );

    } // start_running

    //--------------------------

    bool running()
    {
  DEBUG_LINE_HERE;
      switch (state) {
        case GETTING_STATUS      : get_status      (); break;
        case GETTING_LEAP_SECONDS: get_leap_seconds(); break;
        case GETTING_UTC         : get_utc         (); break;
      }
  DEBUG_LINE_HERE;

      return (state == RUNNING);

    } // running

} NEOGPS_PACKED;


// Construct the GPS object and hook it to the appropriate serial device
static MyGPS gps( &gpsPort );


//--------------------------

static void configNMEA( uint8_t rate )
{
  for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
    ublox::configNMEA( gps, (NMEAGPS::nmea_msg_t) i, rate );
  }
}

//--------------------------

static void disableUBX()
{
  gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
}






int8_t mGPS_Serial::Tasker(uint8_t function, JsonParserObject obj){


  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init();
  // ubx_parser.PrintDevice("Pre_Init done");
  }else
  if(function == FUNC_INIT){
    Init();
  // ubx_parser.PrintDevice("init done");
  }

  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
     // Handle_Connection_And_Configuration();

        // ALOG_INF(PSTR("mGPS_Serial::Tasker"));
      /**
       * @10hz, I only need to check buffer every 100ms for new full data, this will allow the other 100ms to be used
       * Doing every 7 ms should mean each time spend here will be shorter parsing
       * */
      // if(mTime::TimeReached(&tSaved_parse_gps, 100))
      // {
      //   ReadGPSStream();
      // }


      // ubx_parser.PrintDevice("func loop");

      if(started_successfully)
      {


        // // Use advanced character-oriented methods to echo received characters to the Serial Monitor window.
        // if (gpsPort.available()) {

        //   char c = gpsPort.read();

        //   Serial.write(c);

        //   ubx_parser.handle( c );
        //   if (ubx_parser.available()) {
        //     fix_parsing = ubx_parser.read();

        //     if(fix_parsing.status > gps_fix::STATUS_NONE)
        //     {
        //       fix_valid |= fix_parsing; // Merge latest payload decode into validated stored data
        //       ALOG_INF(PSTR("GPS Fix Time: %02d:%02d:%02d"), fix_valid.dateTime.hours, fix_valid.dateTime.minutes, fix_valid.dateTime.seconds);
        //     }
        //   }
        // }

        
        // if (gps.available( gpsPort )) {
        //   trace_all( DEBUG_PORT, gps, gps.read() );
        // }


         if (gpsPort.available()) {

          char c = gpsPort.read();

          // Serial.write(c);

          gps.handle( c );
          if (gps.available()) {
            fix_parsing = gps.read();

            if(fix_parsing.dateTime.hours)
              ALOG_INF(PSTR("GPS Par Time: %02d:%02d:%02d"), fix_parsing.dateTime.hours, fix_parsing.dateTime.minutes, fix_parsing.dateTime.seconds);

            if(fix_parsing.status > gps_fix::STATUS_NONE)
            {
              fix_valid |= fix_parsing; // Merge latest payload decode into validated stored data
              ALOG_INF(PSTR("GPS Fix Time: %02d:%02d:%02d"), fix_valid.dateTime.hours, fix_valid.dateTime.minutes, fix_valid.dateTime.seconds);
            }
          }
        }




      }
    
    break;
    case FUNC_EVERY_SECOND:
      #ifndef DISABLE_SERIAL_LOGGING
      Splash_Latest_Fix(&Serial);
      #endif
    break;
    case FUNC_UPTIME_30_SECONDS:


// Serial.println("start_running");
//   // tkr_Serial->HWSerial2->print("TEST");
  
//   // // Turn off the preconfigured NMEA standard messages
//   configNMEA( 0 );
//   DEBUG_LINE_HERE;

//   // Turn off things that may be left on by a previous build
// //   disableUBX();

//   state = INIT;
//         DeviceConfigure();



  // DEBUG_LINE_HERE;
  // // Turn off the preconfigured NMEA standard messages
  // configNMEA( 0 );

  // DEBUG_LINE_HERE;
  // // Turn off things that may be left on by a previous build
  // disableUBX();

  // DEBUG_LINE_HERE;
  // while (!gps.running())
  // {
  // DEBUG_LINE_HERE;
  //   if (gps.available( gpsPort ))
  //   {
  // DEBUG_LINE_HERE;
  //     gps.read();
  // DEBUG_LINE_HERE;

  //   }
  // }


  // started_successfully = true;

  // // ubx_parser.PrintDevice("init doneA");
  // while(!running())
  // {
  //   // ubx_parser.PrintDevice("init doneB");
  //   if (ubx_parser.available( *tkr_Serial->HWSerial2 ))
  //   {
  //   // ubx_parser.PrintDevice("init doneC");
  //     ubx_parser.read();
  //   // ubx_parser.PrintDevice("init doneD");
  //   }

  // }


// start_running();
// Serial.println("start_running");

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
} // END Tasker


/**
 * @brief Function will try to set the gps hardware messages and baud if data has not been received or timeout has occured
 * */
void mGPS_Serial::Handle_Connection_And_Configuration()
{


}



void mGPS_Serial::ReadGPSStream()
{
  // EveryLoop_PollForGPSData(Serial2);
  // EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();


//   switch(settings.read_gps_method)
//   {
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
//       uint16_t bytes_to_read = pCONT_uart->GetRingBufferDataAndClear(1, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//       // if(strlen(BufferWriterI->GetPtr())==0){
//       //   AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 1, bytes_to_read, BufferWriterI->GetPtr());
//       // }

//       bool gps_fix_reading = false;

//       // Serial.printf("bytes = %d\n\r", bytes_to_read);
//       //if any data found
//       if(bytes_to_read)
//       {  
//         char* pbuffer = BufferWriterI->GetPtr();
//         //AddLog(LOG_LEVEL_TEST, PSTR("GPS >> [%d]"), bytes_to_read);
//         // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
//         // Read bytes in
//         for(int ii=0;ii<bytes_to_read;ii++)
//         {
//           // nmea_parser->parser_byte_in(pbuffer[ii]);
//           ubx_parser.parser_byte_in(pbuffer[ii]);

//           // Check on fix status
//           fix_parsing = ubx_parser.read();
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


//           pCONT_uart->special_json_part_of_gps_buflen = sprintf( pCONT_uart->special_json_part_of_gps_buffer,
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
//               pCONT_time->SetUTCTime(
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
//   }

}


void mGPS_Serial::Splash_Latest_Fix(Stream* out)
{

  out->print( state);
  out->print( ',' );
  out->print( fix_valid.dateTime.seconds);
  out->print( ',' );
  out->print( fix_valid.latitude(), 6 );
  out->print( ',' );
  out->print( fix_valid.longitude(), 6 );
  out->print( ',' );
  out->print( fix_valid.altitude_cm(), 6 );  
  out->print( "cm, " );
  out->print( fix_valid.speed_mph(), 6 );  
  out->print( "mph" );
  out->print( F(", Alt: ") );
  if (fix_valid.valid.altitude)
    out->print( fix_valid.altitude() );
  out->println();

}





void mGPS_Serial::get_status()
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  // ALOG_INF(PSTR("mGPS_Serial::get_status()"));

  static bool acquiring = false;
  // ubx_parser.PrintDevice("init get_status1");

  if (ubx_parser.fix().status == gps_fix::STATUS_NONE) {
  // ubx_parser.PrintDevice("init get_status2");
    static uint32_t dotPrint;
    bool            requestNavStatus = false;

    if (!acquiring) {
      acquiring = true;
      dotPrint = millis();
      DEBUG_PORT.print( F("Acquiring...") );
      requestNavStatus = true;

    } else if (millis() - dotPrint > 1000UL) {
      dotPrint = millis();
      DEBUG_PORT << '.';

      static uint8_t requestPeriod;
      if ((++requestPeriod & 0x07) == 0)
        requestNavStatus = true;
    }

    if (requestNavStatus)
      // Turn on the UBX status message
      ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_STATUS );

  } else {
  ubx_parser.PrintDevice("init get_status3");
    if (acquiring)
      DEBUG_PORT << '\n';
    DEBUG_PORT << F("Acquired status: ") << (uint8_t) ubx_parser.fix().status << '\n';

    #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
        defined(UBLOX_PARSE_TIMEGPS)

      DEBUG_LINE_HERE;
  ubx_parser.PrintDevice("init get_status3a");
      if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS )){
        DEBUG_PORT.println( F("enable TIMEGPS failed!") );



  ubx_parser.PrintDevice("init get_status3b");
      }
      DEBUG_LINE_HERE;

      state = GETTING_LEAP_SECONDS;
    #else
      start_running();
      state = RUNNING;
    #endif
  }

  
  // ubx_parser.PrintDevice("init get_status4");
#endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

} // get_status

//--------------------------

void mGPS_Serial::get_leap_seconds()
{
  // ALOG_INF(PSTR("mGPS_Serial::get_leap_seconds()"));

#ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
      defined(UBLOX_PARSE_TIMEGPS)

    if (GPSTime::leap_seconds != 0) {
      DEBUG_PORT << F("Acquired leap seconds: ") << GPSTime::leap_seconds << '\n';

      if (!ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
        DEBUG_PORT.println( F("disable TIMEGPS failed!") );

      #if defined(UBLOX_PARSE_TIMEUTC)
        if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
          DEBUG_PORT.println( F("enable TIMEUTC failed!") );
        state = GETTING_UTC;
      #else
        start_running();
      #endif
    }
  #endif
  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

} // get_leap_seconds

//--------------------------

void mGPS_Serial::get_utc()
{
  // ALOG_INF(PSTR("mGPS_Serial::get_utc()"));
#ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
      defined(UBLOX_PARSE_TIMEUTC)

    ubx_parser.lock();
      bool            safe = ubx_parser.is_safe();
      NeoGPS::clock_t sow  = GPSTime::start_of_week();
      NeoGPS::time_t  utc  = ubx_parser.fix().dateTime;
    ubx_parser.unlock();

    if (safe && (sow != 0)) {
      DEBUG_PORT << F("Acquired UTC: ") << utc << '\n';
      DEBUG_PORT << F("Acquired Start-of-Week: ") << sow << '\n';

      start_running();
    }
  #endif
#endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

} // get_utc

//--------------------------

void mGPS_Serial::start_running()
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  // ALOG_INF(PSTR("mGPS_Serial::start_running()"));

  bool enabled_msg_with_time = false;

  #if defined(UBLOX_PARSE_POSLLH)
    if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
      DEBUG_PORT.println( F("enable POSLLH failed!") );
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_PVT)
    if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_PVT ))
      DEBUG_PORT.println( F("enable PVT failed!") );
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_VELNED)
    if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED ))
      DEBUG_PORT.println( F("enable VELNED failed!") );
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_DOP)
    if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP ))
      DEBUG_PORT.println( F("enable DOP failed!") );
    else
      DEBUG_PORT.println( F("enabled DOP.") );
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_SVINFO)
    if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_SVINFO ))
      DEBUG_PORT.println( F("enable SVINFO failed!") );    
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_TIMEUTC)

    #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE)
      if (enabled_msg_with_time &&
          !ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
        DEBUG_PORT.println( F("disable TIMEUTC failed!") );
    #elif defined(GPS_FIX_TIME) | defined(GPS_FIX_DATE)
      // If both aren't defined, we can't convert TOW to UTC,
      // so ask for the separate UTC message.
      if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
        DEBUG_PORT.println( F("enable TIMEUTC failed!") );
    #endif

  #endif

  state = RUNNING;
  // trace_header( DEBUG_PORT );

  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

} // start_running

//--------------------------


bool mGPS_Serial::DeviceConfigure()
{
#ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  if(state == INIT)
  {

      state = GETTING_STATUS;
      ALOG_INF(PSTR(D_LOG_GPS "Configuring GPS: Init Started"));
    
      // Start by setting the GPS device to its default condition
          
      // // Turn off the preconfigured NMEA standard messages
      configNMEA( 0 );
      DEBUG_LINE_HERE;

      // Turn off things that may be left on by a previous build
      disableUBX();


      ubx_parser.PrintDevice("init doneA");
      while(!running())
      {
        // ubx_parser.PrintDevice("init doneB");
        if (ubx_parser.available( *tkr_Serial->HWSerial2 ))
        {
        // ubx_parser.PrintDevice("init doneC");
          ubx_parser.read();
        }

        WDT_Reset();

      }


      ALOG_INF(PSTR(D_LOG_GPS "Configuring GPS: Init Complete"));
      ubx_parser.PrintDevice("init doneD");

  }
#endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
}



bool mGPS_Serial::running()
{
  // ALOG_INF(PSTR("mGPS_Serial::running()"));

  // delay(500);

  // ubx_parser.PrintDevice("init doneE");

  switch (state) {
    case GETTING_STATUS      : get_status      (); break;
    case GETTING_LEAP_SECONDS: get_leap_seconds(); break;
    case GETTING_UTC         : get_utc         (); break;
  }


  // ubx_parser.PrintDevice("init doneE2");
  return (state == RUNNING);

} // running



void mGPS_Serial::Pre_Init(){

  settings.fEnableModule = true;

  DEBUG_LINE_HERE;
  #ifdef ENABLE_GPS_PARSER_NMEA
  nmea_parser = new NMEAGPS(); 
  #endif
  DEBUG_LINE_HERE;
  #ifdef ENABLE_GPS_PARSER_UBX
    #ifdef ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS
  // if(m_device == NULL){ Serial.println("m_device is NULL"); Serial.flush(); }
    // ubx_parser = new ubloxGPS( tkr_Serial->HWSerial2 ); 
    #else
    ubx_parser = new ubloxGPS(&Serial2); 
    #endif
  #endif
  DEBUG_LINE_HERE;
#ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  ubx_parser.PrintDevice("pre init end");
#endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  state = NOT_STARTED;

}


/**
 * @brief 
 * */
void mGPS_Serial::Init(void)
{
  
  /**
   * UBX protocol
   * */
  // Init_UBX_Only2();
  // Init_UBX_Only_Requires_PowerCycle();

  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
  // Finsihed with manual control, start ISRs
  pCONT_uart->flag_init_buffers_and_start_isrs = true;
  #endif

  DEBUG_LINE_HERE;

  // DEBUG_PORT.print( F("ublox binary protocol example started.\n") );
  // DEBUG_PORT << F("fix object size = ") << sizeof(ubx_parser.fix()) << '\n';
  // DEBUG_PORT << F("ubloxGPS object size = ") << sizeof(ubloxGPS) << '\n';
  // // DEBUG_PORT << F("MyGPS object size = ") << sizeof(gps2) << '\n';
  // DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );
  // DEBUG_PORT.flush();
  
  // gpsPort.flush();
  // DEBUG_LINE_HERE;
  // gpsPort.end(); delay(100);
  // DEBUG_LINE_HERE;


  // gpsPort.begin(9600);//, SERIAL_8N1, 22, 23);
  // DEBUG_LINE_HERE;
  // while(!gpsPort)
  // {
  // DEBUG_LINE_HERE;
  //   ALOG_INF(PSTR("Waiting for GPS Serial to start"));
  // }
  // DEBUG_LINE_HERE;

  // delay(3000);
  gpsPort.begin(9600);


  DEBUG_LINE_HERE;
  DEBUG_LINE_HERE;

  // tkr_Serial->HWSerial2->print("TEST");


  
  // // // Turn off the preconfigured NMEA standard messages
  // configNMEA( 0 );
  // DEBUG_LINE_HERE;

  // // Turn off things that may be left on by a previous build
  // disableUBX();


  // ubx_parser.PrintDevice("init doneA");
  // while(!running())
  // {
  //   ubx_parser.PrintDevice("init doneB");
  //   if (ubx_parser.available( *tkr_Serial->HWSerial2 ))
  //   {
  //   ubx_parser.PrintDevice("init doneC");
  //     ubx_parser.read();
  //   ubx_parser.PrintDevice("init doneD");
  //   }

  // }


  // gpsPort.begin( 9600, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID) );

}



/**
 * @brief Configure messages
 * Test 2, try set and listen for a response on each baud, find the correct one
 * */
void mGPS_Serial::Init_UBX_Only2()
{

#ifdef ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  runtime.ubx_config_status = 0; 
  uint32_t found_baud = 0;

  // Check pins for serial have been defined before proceeeding
  if(!(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID))) {
    AddLog(LOG_LEVEL_ERROR, PSTR("No gpsPort (Serial1) pins have been set"));
    return;
  }

  uint32_t start_millis = millis();
  AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate started"));

  /**
   * Step 1: Ensure baud is set the default for configuration
   * Last one is the desired speed!!
   * */
  uint32_t baud_list[] = {D_GPS_BAUD_RATE_DEFAULT, 115200, D_GPS_BAUD_RATE_FAST};

  for(int i=0;i<ARRAY_SIZE(baud_list);i++)
  {
    gpsPort.flush();
    // Begin connection
    //unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms
    AddLog(LOG_LEVEL_TEST, PSTR("Baud Test %d on TX%d, RX%d"), baud_list[i], pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID));
    gpsPort.begin(baud_list[i], SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
    // while(!gpsPort);
    // Send default baud command
    while(gpsPort.available()) gpsPort.read(); 

    switch(D_GPS_BAUD_RATE_FAST)
    {
      default:
      case 9600:
        ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
      break;
      case 115200:
        ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
      break;
      case 230400:
        ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud230400 );
      break;
      case 921600:
        ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
      break;
    }

    // if (!ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
    // {
    //   // Good response, then save good baud and leave loop
    //   found_baud = baud_list[i];
    //   AddLog(LOG_LEVEL_TEST, PSTR("found_baud = %d %d"), found_baud, baud_list[i]);

    //   break;

    //   // If found baud, then lets stop, as its where we need to send the commands to


    // }else{
      
    //   AddLog(LOG_LEVEL_TEST, PSTR("NOT found_baud = %d %d"), found_baud, baud_list[i]);
    // }
      //DEBUG_PORT.println( F("enable POSLLH failed!") );



    // switch(D_GPS_BAUD_RATE_DEFAULT)
    // {
    //   default:
    //   case 9600:
    //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
    //   break;
    //   case 115200:
    //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
    //   break;
    // }
    // Complete transmission
    // gpsPort.end();
    // pinMode(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), OUTPUT); digitalWrite(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), HIGH);
    // pinMode(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), OUTPUT); digitalWrite(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), HIGH);
    // // Allow time for the GPS to switch baud speeds
    // delay(500);
  }

  //   gpsPort.flush();
  // // Begin at default speed to start configuring messages
  // gpsPort.begin(D_GPS_BAUD_RATE_DEFAULT, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
  // // while(!gpsPort);
  //   while(gpsPort.available()) gpsPort.read(); 

  /**
   * Disable unwanted signals
   * */
  CommandSend_UBX_Disable_NMEAMessages();
  DEBUG_LINE_HERE;
  CommandSend_UBX_Disable_UBXMessages();
  DEBUG_LINE_HERE;

  // configNMEA( 0 );
  // disableUBX();

  /**
   * Renable what I want
   * */
  // enableUBX();
  runtime.ubx_messages_confirmed_enabled = enableUBX_RequiredOnlyFor3DTracking();
  AddLog(LOG_LEVEL_TEST, PSTR("runtime.ubx_messages_confirmed_enabled = %d"),runtime.ubx_messages_confirmed_enabled);

  // sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );

  /**
   * Change baud rate to desired
   * */
  // switch(D_GPS_BAUD_RATE_FAST)
  // {
  //   default:
  //   case 9600:
  //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
  //   break;
  //   case 115200:
  //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
  //   break;
  //   case 230400:
  //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud230400 );
  //   break;
  //   case 921600:
  //     ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
  //   break;
  // }
  // gpsPort.flush();
  // // gpsPort.end();
  // gpsPort.begin(D_GPS_BAUD_RATE_FAST, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
  // // while(!gpsPort);
  // while(gpsPort.available()) gpsPort.read(); 

  /**
   * Set the rate
   * */
  // sendUBX( ubxRate1Hz, sizeof(ubxRate1Hz) );
  // // sendUBX( ubxRate5Hz, sizeof(ubxRate5Hz) );
  // sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
 
  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "GPS: Setting baud rate finsihed, %d ms"), millis()-start_millis);



#endif // ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE


}


/**
 * @brief Configure messages
 * Test 2, try set and listen for a response on each baud, find the correct one
 * Test 2, this assumes its at 9600 as default, then pushes it up to max
 * */
void mGPS_Serial::Init_UBX_Only_Requires_PowerCycle()
{


#ifdef ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE
  runtime.ubx_config_status = 0; 
  uint32_t found_baud = 0;

  // Check pins for serial have been defined before proceeeding
  if(!(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID))) {
    AddLog(LOG_LEVEL_ERROR, PSTR("No gpsPort (Serial1) pins have been set"));
    return;
  }

  uint32_t start_millis = millis();
  AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate started"));

  /**
   * Change from default to max
   * */
  gpsPort.flush();
  AddLog(LOG_LEVEL_TEST, PSTR("Baud Test %d on TX%d, RX%d"), 9600, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID));
  gpsPort.begin(9600, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
  while(gpsPort.available()){
    gpsPort.read(); //clear buffers
    Serial.print(".");
  }
  ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
  delay(1000);

  /**
   * Change seraial baud to higher rate now
   * */
  gpsPort.flush();
  AddLog(LOG_LEVEL_TEST, PSTR("Baud Test %d on TX%d, RX%d"), 921600, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID));
  gpsPort.begin(921600, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
  while(gpsPort.available()){
    gpsPort.read(); //clear buffers
    Serial.print(".");
  }  

  /**
   * Disable unwanted signals
   * */
  CommandSend_UBX_Disable_NMEAMessages();
  DEBUG_LINE_HERE;

  // Enabling all anyway, so disable none
  // CommandSend_UBX_Disable_UBXMessages();
  // DEBUG_LINE_HERE;

  // configNMEA( 0 );
  // disableUBX();

  /**
   * Renable what I want
   * */
  // enableUBX();
  runtime.ubx_messages_confirmed_enabled = enableUBX_RequiredOnlyFor3DTracking();
  AddLog(LOG_LEVEL_TEST, PSTR("runtime.ubx_messages_confirmed_enabled = %d"),runtime.ubx_messages_confirmed_enabled);

  /**
   * Set the rate
   * */
  // sendUBX( ubxRate1Hz, sizeof(ubxRate1Hz) );
  // // sendUBX( ubxRate5Hz, sizeof(ubxRate5Hz) );
  sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
 
  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "GPS: Setting baud rate finsihed, %d ms"), millis()-start_millis);


#endif // ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

}



void mGPS_Serial::changeBaud( const char *textCommand, unsigned long baud )
{
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableRMC );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableGLL );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableGSV );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableGSA );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableGGA );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableVTG );
  // delay( COMMAND_DELAY );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) disableZDA );
  // delay( 500 );
  // ubx_parser.send_P( &gpsPort, (const __FlashStringHelper *) textCommand );
  // gpsPort.flush();
  // gpsPort.end();

  // DEBUG_PORT.print( F("All sentences disabled for baud rate ") );
  // DEBUG_PORT.print( baud );
  // DEBUG_PORT.println( F(" change.  Enter '1' to reenable sentences.") );
  // delay( 500 );
  // gpsPort.begin( baud );

} // changeBaud




void mGPS_Serial::configNMEA( uint8_t rate )
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
    ublox::configNMEA( ubx_parser, (NMEAGPS::nmea_msg_t) i, rate );
  }
  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
}

void mGPS_Serial::disableUBX()
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  if(ubx_parser.Device())
  { 
    Serial.println("Device good");
  }
  else
  {
    Serial.println("Device bad");
  }
  DEBUG_LINE_HERE;

  ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  ubx_parser.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
}

void mGPS_Serial::enableUBX()
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
}

uint8_t mGPS_Serial::enableUBX_RequiredOnlyFor3DTracking()
{
  #ifdef ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
  bool messages_confirmed_as_enabled_by_readback = false;
  messages_confirmed_as_enabled_by_readback += ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  messages_confirmed_as_enabled_by_readback += ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  messages_confirmed_as_enabled_by_readback += ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  messages_confirmed_as_enabled_by_readback += ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  messages_confirmed_as_enabled_by_readback += ubx_parser.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
  return messages_confirmed_as_enabled_by_readback;
  #endif // ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS
}


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

  // // If ready, print only every second
  // if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  // {

  //   DEBUG_PORT.print( fix_valid.latitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.longitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.altitude_cm(), 6 );  
  //   DEBUG_PORT.print( "cm" );
  //   DEBUG_PORT.print( F(", Altitude: ") );
  //   if (fix_valid.valid.altitude)
  //     DEBUG_PORT.print( fix_valid.altitude() );
  //   DEBUG_PORT.println();
    
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
  //   AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d]=\"%s\""),buflen, buffer);
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


  // // If ready, print only every second
  // if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  // {

  //   DEBUG_PORT.print( fix_valid.latitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.longitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.altitude_cm(), 6 );  
  //   DEBUG_PORT.print( "cm" );
  //   DEBUG_PORT.print( F(", Altitude: ") );
  //   if (fix_valid.valid.altitude)
  //     DEBUG_PORT.print( fix_valid.altitude() );
  //   DEBUG_PORT.println();
    
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
  //   // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
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


  // // If ready, print only every second
  // if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  // {

  //   DEBUG_PORT.print( fix_valid.latitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.longitude(), 6 );
  //   DEBUG_PORT.print( ',' );
  //   DEBUG_PORT.print( fix_valid.altitude_cm(), 6 );  
  //   DEBUG_PORT.print( "cm" );
  //   DEBUG_PORT.print( F(", Altitude: ") );
  //   if (fix_valid.valid.altitude)
  //     DEBUG_PORT.print( fix_valid.altitude() );
  //   DEBUG_PORT.println();
    
  // }

}


#endif // USE_DEVFEATURE_GPS_POLLING_INPUT

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mGPS_Serial::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  
  uint8_t rate = 0;
  if(jtok = obj["GPS"].getObject()["Rate"])
  {
    rate = jtok.getInt();
  }




  if(jtok = obj["GPS"].getObject()["NMEA"].getObject()["MessageName"])
  {
    const char* str = jtok.getStr();
    if(strcmp(str, "GGA")==0)
    {
      tmp_id = NMEAGPS::NMEA_GGA;
    }
    else if(strcmp(str, "GLL")==0)
    {
      tmp_id = NMEAGPS::NMEA_GLL;
    }
    else if(strcmp(str, "GSA")==0)
    {
      tmp_id = NMEAGPS::NMEA_GSA;
    }
    else if(strcmp(str, "GSV")==0)
    {
      tmp_id = NMEAGPS::NMEA_GSV;
    }
    else if(strcmp(str, "RMC")==0)
    {
      tmp_id = NMEAGPS::NMEA_RMC;
    }
    else if(strcmp(str, "VTG")==0)
    {
      tmp_id = NMEAGPS::NMEA_VTG;
    }
    else if(strcmp(str, "ZDA")==0)
    {
      tmp_id = NMEAGPS::NMEA_ZDA;
    }
    else
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("Unknown NMEA message name \"%s\""), str);
    }

    ublox::configNMEA( gps, (NMEAGPS::nmea_msg_t) tmp_id, rate );


  }



  if(jtok = obj["GPS"].getObject()["UBX"].getObject()["MessageName"])
  {
    const char* str = jtok.getStr();
    if(strcmp(str, "POSLLH")==0)
    {
      tmp_id = ublox::UBX_NAV_POSLLH;
    }
    else if(strcmp(str, "STATUS")==0)
    {
      tmp_id = ublox::UBX_NAV_STATUS;
    }
    else if(strcmp(str, "DOP")==0)
    {
      tmp_id = ublox::UBX_NAV_DOP;
    }
    else if(strcmp(str, "ODO")==0)
    {
      tmp_id = ublox::UBX_NAV_ODO;
    }
    else if(strcmp(str, "PVT")==0)
    {
      tmp_id = ublox::UBX_NAV_PVT;
    }
    else if(strcmp(str, "VELNED")==0)
    {
      tmp_id = ublox::UBX_NAV_VELNED;
    }
    else if(strcmp(str, "TIMEGPS")==0)
    {
      tmp_id = ublox::UBX_NAV_TIMEGPS;
    }
    else if(strcmp(str, "TIMEUTC")==0)
    {
      tmp_id = ublox::UBX_NAV_TIMEUTC;
    }
    else if(strcmp(str, "SVINFO")==0)
    {
      tmp_id = ublox::UBX_NAV_SVINFO;
    }
    else if(strcmp(str, "PVT")==0)
    {
      tmp_id = ublox::UBX_NAV_PVT;
    }
    else
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("Unknown UBX message name \"%s\""), str);
    }

    if(rate)
    {
      gps.enable_msg( ublox::UBX_NAV, (ublox::msg_id_t) tmp_id );
      AddLog(LOG_LEVEL_ERROR, PSTR("enable_msg UBX message %d"), tmp_id);
    }
    else
    {
      gps.disable_msg( ublox::UBX_NAV, (ublox::msg_id_t) tmp_id );
      AddLog(LOG_LEVEL_ERROR, PSTR("disable_msg UBX message %d"), tmp_id);
    }


  }

  if(jtok = obj["GPS"].getObject()["Parser"])
  {
    started_successfully = jtok.getInt();
    AddLog(LOG_LEVEL_INFO, PSTR("started_successfully %d"), started_successfully);
  }


  if(jtok = obj["GPS"].getObject()["NMEA"].getObject()["SetRateAll"])
  {
    AddLog(LOG_LEVEL_INFO, PSTR("NMEA-SetRateAll"));

    rate = jtok.getInt();

    for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
      ublox::configNMEA( gps, (NMEAGPS::nmea_msg_t) i, rate );
    }
    
  }


  if(jtok = obj["GPS"].getObject()["UBX"].getObject()["SetRateAll"])
  {
    AddLog(LOG_LEVEL_INFO, PSTR("UBX-SetRateAll"));

    rate = jtok.getInt();

    uint8_t ids[] = {
      ublox::UBX_NAV_POSLLH,
      ublox::UBX_NAV_STATUS,
      ublox::UBX_NAV_DOP,
      ublox::UBX_NAV_ODO,
      ublox::UBX_NAV_PVT,
      ublox::UBX_NAV_VELNED,
      ublox::UBX_NAV_TIMEGPS,
      ublox::UBX_NAV_TIMEUTC,
      ublox::UBX_NAV_SVINFO,
      ublox::UBX_NAV_PVT
    };

    for (uint8_t id=0; id<10; id++){
      if(rate)
        gps.enable_msg( ublox::UBX_NAV, (ublox::msg_id_t) id );
      else
        gps.disable_msg( ublox::UBX_NAV, (ublox::msg_id_t) id );
    }
    
  }


/*
{
  "GPS": {
    "Rate":0,
    "NMEA": {
      "MessageName": "RMC",
      "SetRateAll":0
    },
    "Parser":1
  }
}
*/

    

}


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
  
  JsonBuilderI->Start();  

  // #ifdef ENABLE_GPS_PARSER_NMEA
    JsonBuilderI->Object_Start("Location");
      JsonBuilderI->Add("latitudeL", fix_valid.latitudeL()); 
      JsonBuilderI->Add("latitude", fix_valid.latitude());
      JsonBuilderI->Add("longitudeL", fix_valid.longitudeL());
      JsonBuilderI->Add("longitude", fix_valid.longitude());
    JsonBuilderI->Object_End();

    JsonBuilderI->Object_Start("Altitude");
      JsonBuilderI->Add("altitude_cm", fix_valid.altitude_cm()); 
      JsonBuilderI->Add("altitude", fix_valid.altitude());
      JsonBuilderI->Add("altitude_ft", fix_valid.altitude_ft());
    JsonBuilderI->Object_End();


    JsonBuilderI->Object_Start("Speed");
      JsonBuilderI->Add("speed_mkn", fix_valid.speed_mkn()); 
      JsonBuilderI->Add("speed", fix_valid.speed());
      JsonBuilderI->Add("speed_kph", fix_valid.speed_kph());
      JsonBuilderI->Add("speed_metersph", fix_valid.speed_metersph());
      JsonBuilderI->Add("speed_mph", fix_valid.speed_mph());
    JsonBuilderI->Object_End();

    JsonBuilderI->Object_Start("Heading");
      JsonBuilderI->Add("heading_cd", fix_valid.heading_cd()); 
      JsonBuilderI->Add("heading", fix_valid.heading());
    JsonBuilderI->Object_End();

    JsonBuilderI->Object_Start("geoidHt");
      JsonBuilderI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
      JsonBuilderI->Add("geoidHeight", fix_valid.geoidHeight());
    JsonBuilderI->Object_End();

    JsonBuilderI->Add("satellites", fix_valid.satellites); 

    JsonBuilderI->Object_Start("Dilution");
      JsonBuilderI->Add("hdop", fix_valid.hdop); 
      JsonBuilderI->Add("vdop", fix_valid.vdop);
      JsonBuilderI->Add("pdop", fix_valid.pdop);
      JsonBuilderI->Add("lat_err", fix_valid.lat_err());
      JsonBuilderI->Add("lon_err", fix_valid.lon_err());
      JsonBuilderI->Add("alt_err", fix_valid.alt_err());
      JsonBuilderI->Add("spd_err", fix_valid.spd_err());
      JsonBuilderI->Add("hdg_err", fix_valid.hdg_err());
      JsonBuilderI->Add("spd_err", fix_valid.spd_err());
      JsonBuilderI->Add("time_err", fix_valid.time_err());
    JsonBuilderI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA
  
  return JsonBuilderI->End();

}


/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS_Serial::ConstructJSON_GPSPacket_All(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JsonBuilderI->Start();  

//   #ifdef ENABLE_GPS_PARSER_NMEA
//     JsonBuilderI->Object_Start("Location");
//       JsonBuilderI->Add("latitudeL", fix_valid.latitudeL()); 
//       JsonBuilderI->Add("latitude", fix_valid.latitude());
//       JsonBuilderI->Add("longitudeL", fix_valid.longitudeL());
//       JsonBuilderI->Add("longitude", fix_valid.longitude());
//     JsonBuilderI->Object_End();

//     JsonBuilderI->Object_Start("Altitude");
//       JsonBuilderI->Add("altitude_cm", fix_valid.altitude_cm()); 
//       JsonBuilderI->Add("altitude", fix_valid.altitude());
//       JsonBuilderI->Add("altitude_ft", fix_valid.altitude_ft());
//     JsonBuilderI->Object_End();

//     JsonBuilderI->Object_Start("Speed");
//       JsonBuilderI->Add("speed_mkn", fix_valid.speed_mkn()); 
//       JsonBuilderI->Add("speed", fix_valid.speed());
//       JsonBuilderI->Add("speed_kph", fix_valid.speed_kph());
//       JsonBuilderI->Add("speed_metersph", fix_valid.speed_metersph());
//       JsonBuilderI->Add("speed_mph", fix_valid.speed_mph());
//     JsonBuilderI->Object_End();

//     JsonBuilderI->Object_Start("Heading");
//       JsonBuilderI->Add("heading_cd", fix_valid.heading_cd()); 
//       JsonBuilderI->Add("heading", fix_valid.heading());
//     JsonBuilderI->Object_End();

//     JsonBuilderI->Object_Start("geoidHt");
//       JsonBuilderI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
//       JsonBuilderI->Add("geoidHeight", fix_valid.geoidHeight());
//     JsonBuilderI->Object_End();

//     JsonBuilderI->Add("satellites", fix_valid.satellites); 

//     JsonBuilderI->Object_Start("Dilution");
//       JsonBuilderI->Add("hdop", fix_valid.hdop); 
//       JsonBuilderI->Add("vdop", fix_valid.vdop);
//       JsonBuilderI->Add("pdop", fix_valid.pdop);
//       JsonBuilderI->Add("lat_err", fix_valid.lat_err());
//       JsonBuilderI->Add("lon_err", fix_valid.lon_err());
//       JsonBuilderI->Add("alt_err", fix_valid.alt_err());
//       JsonBuilderI->Add("spd_err", fix_valid.spd_err());
//       JsonBuilderI->Add("hdg_err", fix_valid.hdg_err());
//       JsonBuilderI->Add("spd_err", fix_valid.spd_err());
//       JsonBuilderI->Add("time_err", fix_valid.time_err());
//     JsonBuilderI->Object_End();




//   #endif// ENABLE_GPS_PARSER_NMEA

//     /*
  
//   #ifdef GPS_FIX_LOCATION_DMS
//     DMS_t latitudeDMS;
//     DMS_t longitudeDMS;
//   #endif

// */


    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}

//https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_MessageOverview.html

/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Micro(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JsonBuilderI->Start();  

  // #ifdef ENABLE_GPS_PARSER_NMEA
  // JBI->Object_Start("SequenceNumber");
  //   JBI->Add("GPSCount", 0);
  //   JBI->Add("RSSCount", 0);
  // JBI->Object_End();

  // JBI->Object_Start("Quality");
  //   JBI->Add("Fix", fix_valid.status);
  //   JsonBuilderI->Add("satellites", fix_valid.satellites); 
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
  //   // JsonBuilderI->Add("latitudeL", fix_valid.latitudeL()); 
  //   JsonBuilderI->Add("latitude", fix_valid.latitude());
  //   // JsonBuilderI->Add("longitudeL", fix_valid.longitudeL());
  //   JsonBuilderI->Add("longitude", fix_valid.longitude());
  // JBI->Object_End();

  // JBI->Object_Start("Altitude");
  //   JBI->Add("hMSL_mm", 0);
  //   JBI->Add("height_mm", 0);
  //     JsonBuilderI->Add("altitude_cm", fix_valid.altitude_cm()); 
  //     JsonBuilderI->Add("altitude", fix_valid.altitude());
  //     JsonBuilderI->Add("altitude_ft", fix_valid.altitude_ft());
  // JBI->Object_End();

  // JBI->Object_Start("Velocity");
  //   JBI->Add("speed3D", 0);
  //   JBI->Add("speed2D", 0);
  //     JsonBuilderI->Add("speed_mkn", fix_valid.speed_mkn()); 
  //     JsonBuilderI->Add("speed", fix_valid.speed());
  //     JsonBuilderI->Add("speed_kph", fix_valid.speed_kph());
  //     JsonBuilderI->Add("speed_metersph", fix_valid.speed_metersph());
  //     JsonBuilderI->Add("speed_mph", fix_valid.speed_mph());
  // JBI->Object_End();

  // JBI->Object_Start("Velocity");

  //     JsonBuilderI->Add("heading_cd", fix_valid.heading_cd()); 
  //     JsonBuilderI->Add("heading", fix_valid.heading());

  // JBI->Object_End();

  //   JsonBuilderI->Object_Start("geoidHt");
  //     JsonBuilderI->Add("geoidHeight_cm", fix_valid.geoidHeight_cm()); 
  //     JsonBuilderI->Add("geoidHeight", fix_valid.geoidHeight());
  //   JsonBuilderI->Object_End();

  //   JsonBuilderI->Object_Start("Dilution");
  //     JsonBuilderI->Add("hdop", fix_valid.hdop); 
  //     JsonBuilderI->Add("vdop", fix_valid.vdop);
  //     JsonBuilderI->Add("pdop", fix_valid.pdop);
  //     JsonBuilderI->Add("lat_err", fix_valid.lat_err());
  //     JsonBuilderI->Add("lon_err", fix_valid.lon_err());
  //     JsonBuilderI->Add("alt_err", fix_valid.alt_err());
  //     JsonBuilderI->Add("spd_err", fix_valid.spd_err());
  //     JsonBuilderI->Add("hdg_err", fix_valid.hdg_err());
  //     JsonBuilderI->Add("spd_err", fix_valid.spd_err());
  //     JsonBuilderI->Add("time_err", fix_valid.time_err());
  //   JsonBuilderI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA

  return JsonBuilderI->End();

}

uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Debug(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JsonBuilderI->Start();  

  // #ifdef ENABLE_GPS_PARSER_NMEA
  //   JsonBuilderI->Object_Start("Millis");
  //     JsonBuilderI->Add("GGA",nmea_parser->active_millis.GGA);
  //     JsonBuilderI->Add("GLL",nmea_parser->active_millis.GLL);
  //     JsonBuilderI->Add("GSA",nmea_parser->active_millis.GSA);
  //     JsonBuilderI->Add("GST",nmea_parser->active_millis.GST);
  //     JsonBuilderI->Add("GSV",nmea_parser->active_millis.GSV);
  //     JsonBuilderI->Add("RMC",nmea_parser->active_millis.RMC);
  //     JsonBuilderI->Add("VTG",nmea_parser->active_millis.VTG);
  //     JsonBuilderI->Add("ZDA",nmea_parser->active_millis.ZDA);
  //   JsonBuilderI->Object_End();

  //   JsonBuilderI->Object_Start("Millis2");
  //     JsonBuilderI->Add("GGA",millis()-nmea_parser->active_millis.GGA);
  //     JsonBuilderI->Add("GLL",millis()-nmea_parser->active_millis.GLL);
  //     JsonBuilderI->Add("GSA",millis()-nmea_parser->active_millis.GSA);
  //     JsonBuilderI->Add("GST",millis()-nmea_parser->active_millis.GST);
  //     JsonBuilderI->Add("GSV",millis()-nmea_parser->active_millis.GSV);
  //     JsonBuilderI->Add("RMC",mTime::MillisElapsed(nmea_parser->active_millis.RMC));
  //     JsonBuilderI->Add("VTG",millis()-nmea_parser->active_millis.VTG);
  //     JsonBuilderI->Add("ZDA",millis()-nmea_parser->active_millis.ZDA);
  //   JsonBuilderI->Object_End();

  // #endif // ENABLE_GPS_PARSER_NMEA


  //   JsonBuilderI->Object_Start("UBX_Parsed_Millis");
  //     JsonBuilderI->Add("status",millis()-gps.debug_millis_last_parsed.status);
  //     JsonBuilderI->Add("posllh",millis()-gps.debug_millis_last_parsed.posllh);
  //     JsonBuilderI->Add("pvt",millis()-gps.debug_millis_last_parsed.pvt);
  //     JsonBuilderI->Add("dop",millis()-gps.debug_millis_last_parsed.dop);
  //     JsonBuilderI->Add("velned",millis()-gps.debug_millis_last_parsed.velned);
  //     JsonBuilderI->Add("timegps",mTime::MillisElapsed(gps.debug_millis_last_parsed.timegps));
  //     JsonBuilderI->Add("timeutc",millis()-gps.debug_millis_last_parsed.timeutc);
  //     JsonBuilderI->Add("svinfo",millis()-gps.debug_millis_last_parsed.svinfo);
  //   JsonBuilderI->Object_End();




    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


uint8_t mGPS_Serial::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS_Serial::ConstructJSON_Settings");

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
    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}



uint8_t mGPS_Serial::ConstructJSON_GPSPacket_Required(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS_Serial::ConstructJSON_Settings");

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
    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


// Debug methods when designing, will not be included on logger

// For now, lets use polling for nmea_parser data until I get the rest working for it
// I might want to move this into the "controller" method, or simply require it needs other drivers? cross-drivers, does this make sense??
// Using that, a uart ISR can work to get the data, with maybe using the optional FreeRTOS method for triggering a parse event after message is complete? see how nmea_parser lib works


// I will inject my nmea_parser data in a formatted (searchable) way into the rss/sd card data stream, matlab later will find the nearest, the interopolate from either points beside it



////////////////////// START OF MQTT /////////////////////////

#ifdef USE_MODULE_NETWORK_MQTT

void mGPS_Serial::MQTTHandler_Init(){

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_Settings;


  ptr = &mqtthandler_gpspacket_debug; //also ifchanged together
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_DEBUG_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Debug;

  ptr = &mqtthandler_gpspacket_micro; //also ifchanged together
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MICRO_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Micro;

  ptr = &mqtthandler_gpspacket_all; //also ifchanged together
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_All;

  ptr = &mqtthandler_gpspacket_minimal_teleperiod; //also ifchanged together
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Minimal;

  // All sensor readings I had on pic32
  ptr = &mqtthandler_gpspacket_required;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_REQUIRED_CTR;
  ptr->ConstructJSON_function = &mGPS_Serial::ConstructJSON_GPSPacket_Required;

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mGPS_Serial::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mGPS_Serial::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief Check all handlers if they require action
 * */
void mGPS_Serial::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__SENSORS_GPS_SERIAL__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT






#endif
