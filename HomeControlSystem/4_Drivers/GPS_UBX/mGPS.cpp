#include "mGPS.h"

/***
 * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
 * 
 * */

#ifdef USE_MODULE_DRIVERS_GPS

const char* mGPS::PM_MODULE_DRIVERS_GPS_CTR = D_MODULE_DRIVERS_GPS_CTR;
const char* mGPS::PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR = D_MODULE_DRIVERS_GPS_FRIENDLY_CTR;

#define DEBUG_PORT Serial

//-----------------------------------------------------------------
//  Derive a class to add the state machine for starting up:
//    1) The status must change to something other than NONE.
//    2) The GPS leap seconds must be received
//    3) The UTC time must be received
//    4) All configured messages are "requested"
//         (i.e., "enabled" in the ublox device)
//  Then, all configured messages are parsed and explicitly merged.

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

      if (fix().status == GPS_FIX::STATUS_NONE) {
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
          enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_STATUS );

      } else {
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
        }else{
          Serial.println("GPSTime::leap_seconds == 0)");
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
      bool enabled_msg_with_time = false;

      #if defined(UBLOX_PARSE_POSLLH)
        if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
          DEBUG_PORT.println( F("enable POSLLH failed!") );

        enabled_msg_with_time = true;
      #endif

      #if defined(UBLOX_PARSE_PVT)
        if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_PVT ))
          DEBUG_PORT.println( F("enable PVT failed!") );

        enabled_msg_with_time = true;
      #endif

      #if defined(UBLOX_PARSE_VELNED)
        if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED ))
          DEBUG_PORT.println( F("enable VELNED failed!") );

        enabled_msg_with_time = true;
      #endif

      #if defined(UBLOX_PARSE_DOP)
        if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP ))
          DEBUG_PORT.println( F("enable DOP failed!") );
        else
          DEBUG_PORT.println( F("enabled DOP.") );

        enabled_msg_with_time = true;
      #endif

      #if defined(UBLOX_PARSE_SVINFO)
        if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_SVINFO ))
          DEBUG_PORT.println( F("enable SVINFO failed!") );
        
        enabled_msg_with_time = true;
      #endif

      #if defined(UBLOX_PARSE_TIMEUTC)

        #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE)
          if (enabled_msg_with_time &&
              !disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
            DEBUG_PORT.println( F("disable TIMEUTC failed!") );

        #elif defined(GPS_FIX_TIME) | defined(GPS_FIX_DATE)
          // If both aren't defined, we can't convert TOW to UTC,
          // so ask for the separate UTC message.
          if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
            DEBUG_PORT.println( F("enable TIMEUTC failed!") );
        #endif

      #endif

      state = RUNNING;
      // trace_header( DEBUG_PORT );

    } // start_running

    //--------------------------

    bool running()
    {
      // Serial.printf("bool running() %d\n\r", state);
      switch (state) {
        case GETTING_STATUS      : get_status      (); break;
        case GETTING_LEAP_SECONDS: get_leap_seconds(); break;
        case GETTING_UTC         : get_utc         (); break;
      }

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

//--------------------------

void setup1()
{
  // Start the normal trace output
  // DEBUG_PORT.begin(9600);
  // while (!DEBUG_PORT)
  //   ;

  DEBUG_PORT.print( F("ublox binary protocol example started.\n") );
  // DEBUG_PORT << F("fix object size = ") << sizeof(gps.fix()) << '\n';
  // DEBUG_PORT << F("ubloxGPS object size = ") << sizeof(ubloxGPS) << '\n';
  // DEBUG_PORT << F("MyGPS object size = ") << sizeof(gps) << '\n';
  // DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );
  DEBUG_PORT.flush();

  gpsPort.begin(9600);
  // gpsPort.begin(921600);

  // Turn off the preconfigured NMEA standard messages
  configNMEA( 0 );

  // Turn off things that may be left on by a previous build
  disableUBX();

  // while (!gps.running())
  //   if (gps.available( gpsPort ))
  //     gps.read();
}

//--------------------------

    // /**
    //  * @note Holds a partial result during parsing, only to be merged with the stored fix is valid
    //  * */
    // GPS_FIX   gps_result_parsing;
    // /**
    //  * @note Stores a valid solution, merged from the parsing fix
    //  * */
    // GPS_FIX   gps_result_stored;


void loop1()
{

  // Serial.println("void loop1()");

  uint32_t time = millis();
      
  while (gps.available( gpsPort )) {
  Serial.println("void loop1()");
    pCONT_gps->gps_result_parsing = gps.read();

        DEBUG_PORT.print( pCONT_gps->gps_result_parsing.longitude(), 6 );

    if(pCONT_gps->gps_result_parsing.status > GPS_FIX::STATUS_NONE ){

      //save tmp solution when valid
      pCONT_gps->gps_result_stored = pCONT_gps->gps_result_parsing;

      DEBUG_PORT.print( F("Location: ") );
        DEBUG_PORT.print( pCONT_gps->gps_result_parsing.latitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( pCONT_gps->gps_result_parsing.longitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( pCONT_gps->gps_result_parsing.altitude_cm(), 6 ); 
        DEBUG_PORT.print( "cm" );

      DEBUG_PORT.print( F(", Altitude: ") );
      if (pCONT_gps->gps_result_parsing.valid.altitude)
        DEBUG_PORT.print( pCONT_gps->gps_result_parsing.altitude() );

      DEBUG_PORT.println();
    }

    if(abs(millis()-time) > 100){
      break;
    }

  }
  // // while (!gps.running())
  // //   if (gps.available( gpsPort ))
  // //     gps.read();

  // uint32_t time = millis();

  // while (gps.available( gpsPort )) {
  // // if (gps.available( gpsPort ))
  //   // trace_all( DEBUG_PORT, gps, gps.read() );
  // //   Serial << 
  // // outs << fix;  
  // gps_result_parsing = gps.read();
  // // if(gps_result_parsing.satellites != 0)
  // // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("gps_result_parsing.satellites=%d"),gps_result_parsing.satellites);
  // // }

  //   if(gps_result_parsing.status > GPS_FIX::STATUS_NONE ){

  //     //save tmp solution when valid
  //     gps_result_stored = gps_result_parsing;

  //     DEBUG_PORT.print( F("Location: ") );
  //       DEBUG_PORT.print( gps_result_parsing.latitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.longitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.altitude_cm(), 6 ); 
  //       DEBUG_PORT.print( "cm" );

  //     DEBUG_PORT.print( F(", Altitude: ") );
  //     if (gps_result_parsing.valid.altitude)
  //       DEBUG_PORT.print( gps_result_parsing.altitude() );

  //     DEBUG_PORT.println();

  //   }
  // // }

  //     if(abs(millis()-time) > 10){
  //       break;
  //     }
  // //   AddLog(LOG_LEVEL_TEST, PSTR("sat=%d"),gps.satellites);

  // // If the user types something, reset the message configuration
  // //   back to a normal set of NMEA messages.  This makes it
  // //   convenient to switch to another example program that
  // //   expects a typical set of messages.  This also saves
  // //   putting those config messages in every other example.

  // // if (DEBUG_PORT.available()) {
  // //   do { DEBUG_PORT.read(); } while (DEBUG_PORT.available());
  // //   DEBUG_PORT.println( F("Stopping...") );

  // //   configNMEA( 1 );
  // //   disableUBX();
  // //   gpsPort.flush();
  // //   gpsPort.end();

  // //   DEBUG_PORT.println( F("STOPPED.") );
  // //   for (;;);
  // // }
  // }
}

void loop2()
{

  
  // // Serial.println("void loop1()");

  // uint32_t time = millis();
      
  // while (gps.available( gpsPort )) {
  // Serial.println("void loop1()");
  //   gps_result_parsing = gps.read();

  //       DEBUG_PORT.print( gps_result_parsing.longitude(), 6 );

  //   if(gps_result_parsing.status > GPS_FIX::STATUS_NONE ){

  //     //save tmp solution when valid
  //     gps_result_stored = gps_result_parsing;

  //     DEBUG_PORT.print( F("Location: ") );
  //       DEBUG_PORT.print( gps_result_parsing.latitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.longitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.altitude_cm(), 6 ); 
  //       DEBUG_PORT.print( "cm" );

  //     DEBUG_PORT.print( F(", Altitude: ") );
  //     if (gps_result_parsing.valid.altitude)
  //       DEBUG_PORT.print( gps_result_parsing.altitude() );

  //     DEBUG_PORT.println();
  //   }

  //   if(abs(millis()-time) > 100){
  //     break;
  //   }

  // }
  // // while (!gps.running())
  // //   if (gps.available( gpsPort ))
  // //     gps.read();

  // uint32_t time = millis();

  // while (gps.available( gpsPort )) {
  // // if (gps.available( gpsPort ))
  //   // trace_all( DEBUG_PORT, gps, gps.read() );
  // //   Serial << 
  // // outs << fix;  
  // gps_result_parsing = gps.read();
  // // if(gps_result_parsing.satellites != 0)
  // // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("gps_result_parsing.satellites=%d"),gps_result_parsing.satellites);
  // // }

  //   if(gps_result_parsing.status > GPS_FIX::STATUS_NONE ){

  //     //save tmp solution when valid
  //     gps_result_stored = gps_result_parsing;

  //     DEBUG_PORT.print( F("Location: ") );
  //       DEBUG_PORT.print( gps_result_parsing.latitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.longitude(), 6 );
  //       DEBUG_PORT.print( ',' );
  //       DEBUG_PORT.print( gps_result_parsing.altitude_cm(), 6 ); 
  //       DEBUG_PORT.print( "cm" );

  //     DEBUG_PORT.print( F(", Altitude: ") );
  //     if (gps_result_parsing.valid.altitude)
  //       DEBUG_PORT.print( gps_result_parsing.altitude() );

  //     DEBUG_PORT.println();

  //   }
  // // }

  //     if(abs(millis()-time) > 10){
  //       break;
  //     }
  // //   AddLog(LOG_LEVEL_TEST, PSTR("sat=%d"),gps.satellites);

  // // If the user types something, reset the message configuration
  // //   back to a normal set of NMEA messages.  This makes it
  // //   convenient to switch to another example program that
  // //   expects a typical set of messages.  This also saves
  // //   putting those config messages in every other example.

  // // if (DEBUG_PORT.available()) {
  // //   do { DEBUG_PORT.read(); } while (DEBUG_PORT.available());
  // //   DEBUG_PORT.println( F("Stopping...") );

  // //   configNMEA( 1 );
  // //   disableUBX();
  // //   gpsPort.flush();
  // //   gpsPort.end();

  // //   DEBUG_PORT.println( F("STOPPED.") );
  // //   for (;;);
  // // }
  // }
}



int8_t mGPS::Tasker(uint8_t function, JsonParserObject obj){

          // Serial.println(DEBUG_INSERT_PAGE_BREAK "mGPS::Tasker");
  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
    setup1();

  }

  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

          // Serial.println( "ngs.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABL");


  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:{

  if (!gps.running())
    if (gps.available( gpsPort ))
      gps.read();


// loop1();
loop2();

if (gps.available( gpsPort ))
{

GPS_FIX fix_tmp = gps.read();

  trace_all( DEBUG_PORT, gps, fix_tmp );
  gps_result_stored |= fix_tmp;
  Serial.println("gps.read().latitudeL()");
  Serial.println(fix_tmp.latitudeL());
  Serial.println(gps_result_stored.latitudeL());
}

if(gps.read().latitudeL() != 0)
{
Serial.println(gps.read().latitudeL());
}


      // #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
      //   // EveryLoop_InputMethod_PollingSerial_Internal();
      //   // EveryLoop_InputMethod_PollingSerial_Bytes();
      //   // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
      //   EveryLoop_PollForGPSData(Serial2);
      //   EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();
      // #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

      // #ifdef ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

      // BufferWriterI->Clear();
      // uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(2, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
      // if(strlen(BufferWriterI->GetPtr())){
      //   // AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 2, bytes_in_line, BufferWriterI->GetPtr());
      // }
          
      // bool bytes_waiting = false;
      // bool gps_fix_reading = false;

      // //if any data found
      // if(strlen(BufferWriterI->GetPtr()))
      // {  
      //   char* pbuffer = BufferWriterI->GetPtr();
      //   // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
      //   // Read bytes in
      //   for(int ii=0;ii<BufferWriterI->GetLength();ii++)
      //   {
      //     gps_parser->parser_byte_in(pbuffer[ii]);

      //     // Check on fix status
      //     gps_result_parsing = gps_parser->read();
      //     if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
      //     {
      //       gps_fix_reading = true;
      //       gps_result_stored |= gps_result_parsing; // Save reading
      //     }
      //   }

      //   // Reset buffer
      //   // gps_receive_buffer.bufused = 0;
      //   // memset(gps_receive_buffer.buffer,0,gps_receive_buffer.buflen);

      // }


      // // If ready, print only every second
      // if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
      // {

      //   DEBUG_PORT.print( gps_result_stored.latitude(), 6 );
      //   DEBUG_PORT.print( ',' );
      //   DEBUG_PORT.print( gps_result_stored.longitude(), 6 );
      //   DEBUG_PORT.print( ',' );
      //   DEBUG_PORT.print( gps_result_stored.altitude_cm(), 6 );  
      //   DEBUG_PORT.print( "cm" );
      //   DEBUG_PORT.print( F(", Altitude: ") );
      //   if (gps_result_stored.valid.altitude)
      //     DEBUG_PORT.print( gps_result_stored.altitude() );
      //   DEBUG_PORT.println();
        
      // }

      // #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

      }
    break;
    // }
    case FUNC_EVERY_SECOND: {

        //   Serial.println("gps.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH )" );

        // if (!gps.enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
        // {
        //   Serial.println( F("enable POSLLH failed!") );
        // }
        // else{
        //   Serial.println( F("enable POSLLH success!") );
        // }
    }     
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


void mGPS::pre_init(){

  settings.fEnableModule = true;
  #ifdef ENABLE_GPS_PARSER_NMEA
  gps_parser = new NMEAGPS(); 
  #endif

}


void mGPS::init(void)
{
 
  #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
    gps_receive_buffer.bufused = 0;
    gps_receive_buffer.buflen = 200;
    gps_receive_buffer.buffer = new uint8_t[gps_receive_buffer.buflen];
    gpsPort.begin(9600);
  #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

  DEBUG_PORT.println( F("NMEASDlog.ino started!") );
  DEBUG_PORT.print( F("fix size = ") );
  DEBUG_PORT.println( sizeof(GPS_FIX) );
  DEBUG_PORT.print( NMEAGPS_FIX_MAX );
  DEBUG_PORT.println( F(" GPS updates can be buffered.") );
  
  #ifdef ENABLE_GPS_PARSER_NMEA
  if (gps_parser->merging != NMEAGPS::EXPLICIT_MERGING)
    DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );
  #endif // ENABLE_GPS_PARSER_NMEA

}



#endif
