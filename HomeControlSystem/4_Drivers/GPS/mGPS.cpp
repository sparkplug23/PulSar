#include "mGPS.h"

/***
 * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
 * 
 * */

#ifdef USE_MODULE_DRIVERS_GPS

const char* mGPS::PM_MODULE_DRIVERS_GPS_CTR = D_MODULE_DRIVERS_GPS_CTR;
const char* mGPS::PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR = D_MODULE_DRIVERS_GPS_FRIENDLY_CTR;

// GPS_FIX  fixcpp;// = nullptr; // This holds on to the latest values
// GPS_FIX  fix_saved;// = nullptr; // This holds on to the latest values

// static NMEAGPS   gps_parser;
// static void GPSisr( uint8_t c )
// {
//   gps_parser.handle( c );

// } // GPSisr


void mGPS::pre_init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
    settings.fEnableModule = true;
//   }

  gps_parser = new NMEAGPS(); // This parses the GPS characters
//   fix = new GPS_FIX(); // This holds on to the latest values


}


void mGPS::init(void)
{

  
gps_receive_buffer.bufused = 0;
gps_receive_buffer.buflen = 200;
gps_receive_buffer.buffer = new uint8_t[gps_receive_buffer.buflen];

#ifdef ENABLE_DEVFEATURE_GPSTEST1
 
  #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
    gpsPort.begin(9600);
  #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

  DEBUG_PORT.println( F("NMEASDlog.ino started!") );
  DEBUG_PORT.print( F("fix size = ") );
  DEBUG_PORT.println( sizeof(GPS_FIX) );
  DEBUG_PORT.print( NMEAGPS_FIX_MAX );
  DEBUG_PORT.println( F(" GPS updates can be buffered.") );
  
  if (gps_parser->merging != NMEAGPS::EXPLICIT_MERGING)
    DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );

#endif
  
}





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

      // EveryLoop_InputMethod_PollingSerial_Internal();
      // EveryLoop_InputMethod_PollingSerial_Bytes();
      // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
      
      EveryLoop_PollForGPSData(Serial2);
      EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();

      #ifdef ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD


      char buffer[100] = {0};
      uint16_t buf_index = 0;
      uint16_t bytes_in_line = pCONT_uart->GetReceiveBuffer2(buffer, sizeof(buffer), '\n');

      if(bytes_in_line){
        AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"%s\""), bytes_in_line, buffer);
      }
      // else{
      //   AddLog(LOG_LEVEL_TEST, PSTR("size=%d, \"none\""), bytes_in_line);
      // }

      bool new_data = false;

      #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

    break;
    }
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



#ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
/**
 * Method A: Check Serial port directly, and read bytes into parser
 * @note timeout is used so loop is not blocking 
 * */
void mGPS::EveryLoop_InputMethod_PollingSerial_Internal()
{

  uint32_t time = millis();
      
  while (gps_parser->available( gpsPort )) {
    gps_result_parsing = gps_parser->read();

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

  while(Serial2.available())
  {
    // Read bytes in
    gps_parser->parser_byte_in(Serial2.read());

    // Check on fix status
    gps_result_parsing = gps_parser->read();
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

  while(Serial2.available())
  {
    if(buflen < 400)
    {
      buffer[buflen++] = Serial2.read();
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
      gps_parser->parser_byte_in(buffer[ii]);

      // Check on fix status
      gps_result_parsing = gps_parser->read();
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
      gps_parser->parser_byte_in(gps_receive_buffer.buffer[ii]);

      // Check on fix status
      gps_result_parsing = gps_parser->read();
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
