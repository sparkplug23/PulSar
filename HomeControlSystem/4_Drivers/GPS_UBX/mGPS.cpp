#include "mGPS.h"

/***
 * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
 * 
 * */

#ifdef USE_MODULE_DRIVERS_GPS

const char* mGPS::PM_MODULE_DRIVERS_GPS_CTR = D_MODULE_DRIVERS_GPS_CTR;
const char* mGPS::PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR = D_MODULE_DRIVERS_GPS_FRIENDLY_CTR;

int8_t mGPS::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:{

    // #ifdef USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
      if(runtime.ubx_config_status==0)
      {

        // Turn off the preconfigured NMEA standard messages
        configNMEA( 0 );

        // Turn off things that may be left on by a previous build
        disableUBX();

        // Turn on the exact UBX messages I need
        send_UBX_enable_messages();

        // Change baud to higher rate
        ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );

        delay(100);
        gpsPort.flush();
        // Change to high speed
        gpsPort.updateBaudRate(921600); 

        // Increase send frequency to 10hz
        sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
        // sendUBX( ubxRate1Hz, sizeof(ubxRate1Hz) );

        runtime.ubx_config_status = 1; //success

      }

      // #endif

      #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
        // EveryLoop_InputMethod_PollingSerial_Internal();
        // EveryLoop_InputMethod_PollingSerial_Bytes();
        // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
        EveryLoop_PollForGPSData(Serial2);
        EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();
      #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

      #ifdef ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
      #ifdef USE_MODULE_DRIVERS_SERIAL_UART

      BufferWriterI->Clear();
      uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(2, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
      // if(strlen(BufferWriterI->GetPtr())==0){
      //   // AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 2, bytes_in_line, BufferWriterI->GetPtr());
      //   AddLog(LOG_LEVEL_TEST, PSTR("GPS >> [%d]"), bytes_in_line);
      // }
          
      bool bytes_waiting = false;
      bool gps_fix_reading = false;

      //if any data found
      if(bytes_in_line)
      {  
        char* pbuffer = BufferWriterI->GetPtr();
        // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
        // Read bytes in
        for(int ii=0;ii<bytes_in_line;ii++)
        {
          // nmea_parser->parser_byte_in(pbuffer[ii]);
          gps.parser_byte_in(pbuffer[ii]);

          // Check on fix status
          gps_result_parsing = gps.read();
          if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
          {
            gps_fix_reading = true;
            gps_result_stored |= gps_result_parsing; // Save reading
          }
        }

      }


      #endif//USE_MODULE_DRIVERS_SERIAL_UART

      // If ready, print only every second
      if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
      {

        DEBUG_PORT.print( gps_result_stored.dateTime.seconds);
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( gps_result_stored.latitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( gps_result_stored.longitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( gps_result_stored.altitude_cm(), 6 );  
        DEBUG_PORT.print( "cm" );
        DEBUG_PORT.print( F(", Altitude: ") );
        if (gps_result_stored.valid.altitude)
          DEBUG_PORT.print( gps_result_stored.altitude() );
        DEBUG_PORT.println();
        
      }

      #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

      }
    break;
    // }
    case FUNC_EVERY_SECOND: {

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
  // For now, we are hard coding to UBX only
  // #ifdef ENABLE_GPS_PARSER_NMEA
  nmea_parser = new NMEAGPS(); 
  // #endif
  #ifdef ENABLE_GPS_PARSER_UBX
  ubx_parser = new ubloxGPS(&Serial2); 
  #endif

  // gps = new MyGPS(&Serial2);

}


  /**
   * init method must handle both UBX and NMEA, perhaps checking and switching if required?
   * */

void mGPS::init(void)
{

  // #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT_UBX
    // gpsPort.begin(9600);
  // #endif

  #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
    // gps_receive_buffer.bufused = 0;
    // gps_receive_buffer.buflen = 200;
    // gps_receive_buffer.buffer = new uint8_t[gps_receive_buffer.buflen];
  #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

  DEBUG_PORT.println( F("NMEASDlog.ino started!") );
  DEBUG_PORT.print( F("fix size = ") );
  DEBUG_PORT.println( sizeof(GPS_FIX) );
  DEBUG_PORT.print( NMEAGPS_FIX_MAX );
  DEBUG_PORT.println( F(" GPS updates can be buffered.") );
  
  // This should be performed in loop? once the method is known
  #ifdef ENABLE_GPS_PARSER_NMEA
  if (nmea_parser->merging != NMEAGPS::EXPLICIT_MERGING)
    DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );
  #endif // ENABLE_GPS_PARSER_NMEA
  if (ubx_parser->merging != NMEAGPS::EXPLICIT_MERGING)
    DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );
  

  /**
   * UBX protocol
   * */
  
  // Start the normal trace output
  // DEBUG_PORT.begin(9600);
  // while (!DEBUG_PORT)
  //   ;

  runtime.ubx_config_status = 0; 

  DEBUG_PORT.print( F("ublox binary protocol example started.\n") );
  // DEBUG_PORT << F("fix object size = ") << sizeof(gps.fix()) << '\n';
  // DEBUG_PORT << F("ubloxGPS object size = ") << sizeof(ubloxGPS) << '\n';
  // DEBUG_PORT << F("MyGPS object size = ") << sizeof(gps) << '\n';
  // DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );
  DEBUG_PORT.flush();

  #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
  gpsPort.begin(9600); // this only matters for polling
  // gpsPort.begin(921600);
  #endif


  // Lets try force the baud rate early

  /**
   * Send to baud on 9600 
   * */
//   gpsPort.begin(9600);

uint32_t start_millis = millis();

AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate started"));


// bool baud_found = false;
// uint16_t baud_rate_responsed = 0;

// gpsPort.end();

//   uint32_t baud_rates[] = {9600, 921600};

//   for(int i=0; i<ARRAY_SIZE(baud_rates); i++)
//   {
//     AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate baud_rates[i] %d"),baud_rates[i]);
//     DEBUG_LINE_HERE;
//     Serial.flush();
//     DEBUG_LINE_HERE;
//     delay(1000);
//     DEBUG_LINE_HERE;

//     switch(i)
//     {
//       default:
//       case 0: gpsPort.begin(9600); break;
//       case 1: gpsPort.begin(921600); break;
//     }
    
//     DEBUG_LINE_HERE;
//     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
//     DEBUG_LINE_HERE;
//     gpsPort.flush();
//     DEBUG_LINE_HERE;
//     gpsPort.end();
//     DEBUG_LINE_HERE;
//     delay(100);
//     DEBUG_LINE_HERE;
//   }

// Without interrupts, 912600 is too fast for polling during config. Force speed back to 9600 for config

#ifdef ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW
  // Send command on 912600 to slower speed again for config
  gpsPort.setDebugOutput(true);
  gpsPort.begin(921600); 
  // Wait for serial to start
  while (!gpsPort){};
  // Send command to lower baud to 9600
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
  gpsPort.flush();
  // change to new slower baud for polling init phase
  delay(100);
  gpsPort.updateBaudRate(9600); 
  // confirm its working on the lower baud, set a flag
  if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
  {
    AddLog(LOG_LEVEL_INFO, PSTR("GPS baud 921600 Working"));
  }
#endif // ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW


  //   Serial2.setDebugOutput(true);
  //     gpsPort.begin(9600); 
  // // Make sure if its on 9600 baud to switch to 921600

  // uint32_t baud_rates[] = {9600, 921600};

  // for(int i=0; i<ARRAY_SIZE(baud_rates); i++)
  // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate baud_rates[i] %d"),baud_rates[i]);
  //   DEBUG_LINE_HERE;
  //   // Serial.flush();
  //   DEBUG_LINE_HERE;
  //   // delay(1000);
  //   // DEBUG_LINE_HERE;

  //   // updateBaudRate

  //   switch(i)
  //   {
  //     default:
  //     case 0: 
  //   DEBUG_LINE_HERE;
  //     gpsPort.updateBaudRate(9600); 
  //   DEBUG_LINE_HERE;
  //     break;
  //     case 1:
  //   DEBUG_LINE_HERE; 
  //   gpsPort.updateBaudRate(921600); 
  //   DEBUG_LINE_HERE;
  //   break;
  //   }
    
  // //   DEBUG_LINE_HERE;
  //   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
  // //   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
  //   DEBUG_LINE_HERE;
  //   gpsPort.flush();
  //   DEBUG_LINE_HERE;
  //   // gpsPort.end();
  //   DEBUG_LINE_HERE;
  //   delay(100);
  //   DEBUG_LINE_HERE;
  // }


  // // // long baud = 115200;


  // gpsPort.begin(921600);


  AddLog(LOG_LEVEL_TEST, PSTR("GPS: Setting baud rate finsihed, %d ms"), millis()-start_millis);

}


void mGPS::configNMEA( uint8_t rate )
{
  for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
    ublox::configNMEA( ubx_parser, (NMEAGPS::nmea_msg_t) i, rate );
  }
}




#endif
