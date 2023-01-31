#include "mGPS.h"

/***
 * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
 * */

#ifdef USE_MODULE_DRIVERS_GPS

const char* mGPS::PM_MODULE_DRIVERS_GPS_CTR = D_MODULE_DRIVERS_GPS_CTR;
const char* mGPS::PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR = D_MODULE_DRIVERS_GPS_FRIENDLY_CTR;

int8_t mGPS::Tasker(uint8_t function, JsonParserObject obj){
  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init();
  }else
  if(function == FUNC_INIT){
    Init();
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
     // Handle_Connection_And_Configuration();

      /**
       * @10hz, I only need to check buffer every 100ms for new full data, this will allow the other 100ms to be used
       * Doing every 7 ms should mean each time spend here will be shorter parsing
       * */
      if(mTime::TimeReached(&tSaved_parse_gps, 100))
      {
        ReadGPSStream();
      }
    
    break;
    case FUNC_EVERY_SECOND:

      #ifndef DISABLE_SERIAL_LOGGING
      // Splash_Latest_Fix(&Serial);
      #endif
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
void mGPS::Handle_Connection_And_Configuration()
{


}



void mGPS::ReadGPSStream()
{

  switch(settings.read_gps_method)
  {
    case GPS_INPUT_STREAM_METHOD_POLLING_ID:
    {
      #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
        // EveryLoop_InputMethod_PollingSerial_Internal();
        // EveryLoop_InputMethod_PollingSerial_Bytes();
        // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
        EveryLoop_PollForGPSData(Serial1);
        EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();
      #endif // USE_DEVFEATURE_GPS_POLLING_INPUT
    }
    break;
    case GPS_INPUT_STREAM_METHOD_RINGBUFFERS_ID:
    {
      #ifdef ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
      #ifdef USE_MODULE_DRIVERS_SERIAL_UART

      // uint16_t bytes_to_read_for_faster_parsing = 100;


      // BufferWriterI->Clear();
      uint16_t bytes_to_read = pCONT_uart->GetRingBufferDataAndClear(1, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
      // if(strlen(BufferWriterI->GetPtr())==0){
      //   AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 1, bytes_to_read, BufferWriterI->GetPtr());
      // }

      bool gps_fix_reading = false;

      // Serial.printf("bytes = %d\n\r", bytes_to_read);
      //if any data found
      if(bytes_to_read)
      {  
        char* pbuffer = BufferWriterI->GetPtr();
        //AddLog(LOG_LEVEL_TEST, PSTR("GPS >> [%d]"), bytes_to_read);
        // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
        // Read bytes in
        for(int ii=0;ii<bytes_to_read;ii++)
        {
          // nmea_parser->parser_byte_in(pbuffer[ii]);
          ubx_parser->parser_byte_in(pbuffer[ii]);

          // Check on fix status
          gps_result_parsing = ubx_parser->read();
          if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
          {
            gps_fix_reading = true;
            flag_incoming_data_at_correct_runtime_baud = true;
            // gps_result_stored |= gps_result_parsing; // Save reading
            // Serial.printf("=============================================%d\n\r",gps_result_parsing.status);
          }
            gps_result_stored |= gps_result_parsing; // Save reading
          // Serial.printf("status=%d\n\r",gps_result_parsing.status);

          /**
           * Manually update ISR Safe values into struct for sd output
           * */
          my_gps_vals.lat = gps_result_stored.latitudeL();
          my_gps_vals.lon = gps_result_stored.longitudeL();
          my_gps_vals.alt = gps_result_stored.altitude_cm();
          my_gps_vals.speed = (uint16_t)gps_result_stored.speed()*100; //float to int
          my_gps_vals.heading_cd = gps_result_stored.heading_cd();
          my_gps_vals.geoidHeight_cm = gps_result_stored.geoidHeight_cm();
          my_gps_vals.hours = gps_result_stored.dateTime.hours+1; //add daylight savings
          my_gps_vals.minutes = gps_result_stored.dateTime.minutes;
          my_gps_vals.seconds = gps_result_stored.dateTime.seconds;
          my_gps_vals.dateTime_ms = gps_result_stored.dateTime_ms();


          pCONT_uart->special_json_part_of_gps_buflen = sprintf( pCONT_uart->special_json_part_of_gps_buffer,
            "B]],\"G\":[%d,%d,%d,%d,%d,%d,%d,%d]}@", 
            my_gps_vals.lat,
            my_gps_vals.lon,
            my_gps_vals.alt,
            my_gps_vals.speed,
            my_gps_vals.hours,
            my_gps_vals.minutes,
            my_gps_vals.seconds,
            my_gps_vals.dateTime_ms/100
          );


//       if(sequence_test_global == 0){
//         JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
//       }
//       JBI->Add("N", sequence_test_global++);
          
    // char special_json_part_of_gps_buffer[300];
    // uint16_t special_json_part_of_gps_buflen = 0;




          
            //if first fix with valud time, use this to update internal time
            if((gps_latest.fix_used_to_update_internal_rtc == false)&&(gps_result_stored.dateTime.is_valid()))
            {
              gps_latest.fix_used_to_update_internal_rtc = true;
              // Later, do this as a rule event              
              pCONT_time->SetUTCTime(
                gps_result_stored.dateTime.year,
                gps_result_stored.dateTime.month,
                gps_result_stored.dateTime.day,
                gps_result_stored.dateTime.hours,
                gps_result_stored.dateTime.minutes,
                gps_result_stored.dateTime.seconds
              );
            }

        }

      }


      #endif//USE_MODULE_DRIVERS_SERIAL_UART
      #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

    }
    break;
  }

}

/**
 * @brief 
 * */
void mGPS::Splash_Latest_Fix(Stream* out)
{
  out->print( gps_result_stored.dateTime.seconds);
  out->print( ',' );
  out->print( gps_result_stored.latitude(), 6 );
  out->print( ',' );
  out->print( gps_result_stored.longitude(), 6 );
  out->print( ',' );
  out->print( gps_result_stored.altitude_cm(), 6 );  
  out->print( "cm" );
  out->print( F(", Altitude: ") );
  if (gps_result_stored.valid.altitude)
    out->print( gps_result_stored.altitude() );
  out->println();

}

/**
 * @brief 
 * */
void mGPS::Pre_Init(){

  settings.fEnableModule = true;
  #ifdef ENABLE_GPS_PARSER_NMEA
  nmea_parser = new NMEAGPS(); 
  #endif
  #ifdef ENABLE_GPS_PARSER_UBX
  ubx_parser = new ubloxGPS(&Serial1); 
  #endif

}

/**
 * @brief 
 * */
void mGPS::Init(void)
{
  
  /**
   * UBX protocol
   * */
  // Init_UBX_Only2();
  Init_UBX_Only_Requires_PowerCycle();

  // Finsihed with manual control, start ISRs
  pCONT_uart->flag_init_buffers_and_start_isrs = true;
  
}



/**
 * @brief Configure messages
 * Test 2, try set and listen for a response on each baud, find the correct one
 * */
void mGPS::Init_UBX_Only2()
{

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
        ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
      break;
      case 115200:
        ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
      break;
      case 230400:
        ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud230400 );
      break;
      case 921600:
        ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
      break;
    }

    // if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
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
    //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
    //   break;
    //   case 115200:
    //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
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
  //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
  //   break;
  //   case 115200:
  //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
  //   break;
  //   case 230400:
  //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud230400 );
  //   break;
  //   case 921600:
  //     ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
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

}


/**
 * @brief Configure messages
 * Test 2, try set and listen for a response on each baud, find the correct one
 * Test 2, this assumes its at 9600 as default, then pushes it up to max
 * */
void mGPS::Init_UBX_Only_Requires_PowerCycle()
{

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
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
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

}



void mGPS::configNMEA( uint8_t rate )
{
  for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
    ublox::configNMEA( ubx_parser, (NMEAGPS::nmea_msg_t) i, rate );
  }
}


void mGPS::changeBaud( const char *textCommand, unsigned long baud )
{
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableRMC );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGLL );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGSV );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGSA );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGGA );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableVTG );
  // delay( COMMAND_DELAY );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableZDA );
  // delay( 500 );
  // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) textCommand );
  // gpsPort.flush();
  // gpsPort.end();

  // DEBUG_PORT.print( F("All sentences disabled for baud rate ") );
  // DEBUG_PORT.print( baud );
  // DEBUG_PORT.println( F(" change.  Enter '1' to reenable sentences.") );
  // delay( 500 );
  // gpsPort.begin( baud );

} // changeBaud


/**
 * Send commands to disable NMEA sentences, via NMEA protocol, on request baud
 * @param baud The rate to transmit the messages over
 * */
void mGPS::CommandSend_NMEA_Disable_NMEAMessages()//uint32_t baud)
{  
  DEBUG_LINE_HERE;
  // gpsPort.updateBaudRate(baud);
  DEBUG_LINE_HERE;
  // Wait for serial to start
  DEBUG_LINE_HERE;
  // while (!gpsPort){};
  // Send command to lower baud to 9600
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableRMC );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) enableGLL );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGSV );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGSA );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableGGA );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableVTG );
  delay( COMMAND_DELAY );
  ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) disableZDA );
  gpsPort.flush();

}


/**
 * Send commands to disable NMEA sentences, via NMEA protocol, on request baud
 * @param baud The rate to transmit the messages over
 * */
void mGPS::CommandSend_UBX_Disable_NMEAMessages()//uint32_t baud)
{  
  
  // gpsPort.updateBaudRate(baud);
  // Wait for serial to start
  // while (!gpsPort){};
  // Send command to lower baud to 9600
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableGLL, sizeof(ubxDisableGLL) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableGSV, sizeof(ubxDisableGSV) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableGSA, sizeof(ubxDisableGSA) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableGGA, sizeof(ubxDisableGGA) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableVTG, sizeof(ubxDisableVTG) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableZDA, sizeof(ubxDisableZDA) );
    delay( COMMAND_DELAY );
  sendUBX( ubxDisableRMC, sizeof(ubxDisableRMC) );

  // gpsPort.flush();

}

/**
 * Send commands to disable NMEA sentences, via NMEA protocol, on request baud
 * @param baud The rate to transmit the messages over
 * */
void mGPS::CommandSend_UBX_Disable_UBXMessages()//uint32_t baud)
{  
  
  // gpsPort.updateBaudRate(baud);
  // Wait for serial to start
  // while (!gpsPort){};
  // Send command to lower baud to 9600
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
  // gpsPort.flush();

}


#endif
