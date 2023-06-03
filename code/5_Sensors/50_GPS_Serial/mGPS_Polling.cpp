#include "mGPS.h"

/***
 * Modes: Polling methods under development
 * 
 * */

#ifdef USE_MODULE_DRIVERS_GPS



#ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
/**
 * Method A: Check Serial port directly, and read bytes into parser
 * @note timeout is used so loop is not blocking 
 * */
void mGPS::EveryLoop_InputMethod_PollingSerial_Internal()
{

  uint32_t time = millis();
      
  while (nmea_parser->available( gpsPort )) {
    gps_result_parsing = nmea_parser->read();

    if(gps_result_parsing.status > GPS_FIX::STATUS_NONE ){

      //save tmp solution when valid
      gps_result_stored = gps_result_parsing;

      DEBUG_PORT.print( F("Location: ") );
        DEBUG_PORT.print( gps_result_parsing.latitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( gps_result_parsing.longitude(), 6 );
        DEBUG_PORT.print( ',' );
        DEBUG_PORT.print( gps_result_parsing.altitude_cm(), 6 ); 
        DEBUG_PORT.print( "cm" );

      DEBUG_PORT.print( F(", Altitude: ") );
      if (gps_result_parsing.valid.altitude)
        DEBUG_PORT.print( gps_result_parsing.altitude() );

      DEBUG_PORT.println();
    }

    if(abs(millis()-time) > 10){
      break;
    }

  }

} // END fucntion

void mGPS::EveryLoop_InputMethod_PollingSerial_Bytes()
{

  uint32_t timeout = millis();
  bool bytes_waiting = false;
  bool gps_fix_reading = false;

  while(Serial1.available())
  {
    // Read bytes in
    nmea_parser->parser_byte_in(Serial1.read());

    // Check on fix status
    gps_result_parsing = nmea_parser->read();
    if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
    {
      gps_fix_reading = true;
      gps_result_stored = gps_result_parsing; // Save reading
    }

    // Check for timeout    
    if(abs(millis()-timeout) > 10){
      break;
    }
  }

  // If ready, print only every second
  if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  {

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

}

/**
 * Reading into a buffer, then pushing that data into the parser
 * */
void mGPS::EveryLoop_InputMethod_PollingSerial_BytesToBuffer()
{

  uint32_t timeout = millis();
  bool bytes_waiting = false;
  bool gps_fix_reading = false;

  char buffer[400] = {0};
  uint8_t buflen = 0;

  while(Serial1.available())
  {
    if(buflen < 400)
    {
      buffer[buflen++] = Serial1.read();
    }
    else
    {
      break; // exceeded buffer
    } 

    // Check for timeout    
    if(abs(millis()-timeout) > 10){
      break;
    }
  }


  //if any data found
  if(buflen)
  {  
    AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d]=\"%s\""),buflen, buffer);
    // Read bytes in
    for(int ii=0;ii<buflen;ii++)
    {
      nmea_parser->parser_byte_in(buffer[ii]);

      // Check on fix status
      gps_result_parsing = nmea_parser->read();
      if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
      {
        gps_fix_reading = true;
        gps_result_stored = gps_result_parsing; // Save reading
      }
    }

  }


  // If ready, print only every second
  if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  {

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

}

void mGPS::EveryLoop_PollForGPSData(Stream& port)
{

  uint32_t timeout = millis();
  while(port.available())
  {
    if(gps_receive_buffer.bufused < gps_receive_buffer.buflen)
    {
      gps_receive_buffer.buffer[gps_receive_buffer.bufused++] = port.read();
    }
    else
    {
      AddLog(LOG_LEVEL_ERROR, PSTR("buffer overflow"));
      break; // exceeded buffer
    } 

    // Check for timeout    
    if(abs(millis()-timeout) > 5){
      break;
    }
  }

}


/**
 * Reading into a buffer, then pushing that data into the parser
 * */
void mGPS::EveryLoop_InputMethod_PollingSerial_BytesFromBuffer()
{

  bool bytes_waiting = false;
  bool gps_fix_reading = false;

  //if any data found
  if(gps_receive_buffer.bufused)
  {  
    // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
    // Read bytes in
    for(int ii=0;ii<gps_receive_buffer.bufused;ii++)
    {
      nmea_parser->parser_byte_in(gps_receive_buffer.buffer[ii]);

      // Check on fix status
      gps_result_parsing = nmea_parser->read();
      if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
      {
        gps_fix_reading = true;
        gps_result_stored |= gps_result_parsing; // Save reading
      }
    }

    // Reset buffer
    gps_receive_buffer.bufused = 0;
    memset(gps_receive_buffer.buffer,0,gps_receive_buffer.buflen);

  }


  // If ready, print only every second
  if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
  {

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

}


#endif // USE_DEVFEATURE_GPS_POLLING_INPUT

#endif
