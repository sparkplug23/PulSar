// #include "mGPS.h"

// /***
//  * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
//  * 
//  * */

// #ifdef USE_MODULE_DRIVERS_GPS

// const char* mGPS::PM_MODULE_DRIVERS_GPS_CTR = D_MODULE_DRIVERS_GPS_CTR;
// const char* mGPS::PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR = D_MODULE_DRIVERS_GPS_FRIENDLY_CTR;


// int8_t mGPS::Tasker(uint8_t function, JsonParserObject obj){

//   /************
//    * INIT SECTION * 
//   *******************/
//   if(function == FUNC_PRE_INIT){
//     pre_init();
//   }else
//   if(function == FUNC_INIT){
//     init();
//   }

//   if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case FUNC_LOOP:{

//       #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
//         // EveryLoop_InputMethod_PollingSerial_Internal();
//         // EveryLoop_InputMethod_PollingSerial_Bytes();
//         // EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
//         EveryLoop_PollForGPSData(Serial2);
//         EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();
//       #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

//       #ifdef ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

//       BufferWriterI->Clear();
//       uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(2, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//       if(strlen(BufferWriterI->GetPtr())){
//         // AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 2, bytes_in_line, BufferWriterI->GetPtr());
//       }
          
//       bool bytes_waiting = false;
//       bool gps_fix_reading = false;

//       //if any data found
//       if(strlen(BufferWriterI->GetPtr()))
//       {  
//         char* pbuffer = BufferWriterI->GetPtr();
//         // AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d|%d]=\"%s\""),gps_receive_buffer.bufused, gps_receive_buffer.buflen, gps_receive_buffer.buffer);
//         // Read bytes in
//         for(int ii=0;ii<BufferWriterI->GetLength();ii++)
//         {
//           gps_parser->parser_byte_in(pbuffer[ii]);

//           // Check on fix status
//           gps_result_parsing = gps_parser->read();
//           if(gps_result_parsing.status > GPS_FIX::STATUS_NONE)
//           {
//             gps_fix_reading = true;
//             gps_result_stored |= gps_result_parsing; // Save reading
//           }
//         }

//         // Reset buffer
//         // gps_receive_buffer.bufused = 0;
//         // memset(gps_receive_buffer.buffer,0,gps_receive_buffer.buflen);

//       }


//       // If ready, print only every second
//       if(mTime::TimeReached(&tSaved_SplashFix, 1000))// || gps_fix_reading)
//       {

//         DEBUG_PORT.print( gps_result_stored.latitude(), 6 );
//         DEBUG_PORT.print( ',' );
//         DEBUG_PORT.print( gps_result_stored.longitude(), 6 );
//         DEBUG_PORT.print( ',' );
//         DEBUG_PORT.print( gps_result_stored.altitude_cm(), 6 );  
//         DEBUG_PORT.print( "cm" );
//         DEBUG_PORT.print( F(", Altitude: ") );
//         if (gps_result_stored.valid.altitude)
//           DEBUG_PORT.print( gps_result_stored.altitude() );
//         DEBUG_PORT.println();
        
//       }

//       #endif // ENABLE_DEVFEATURE_GPS_INTERRUPT_METHOD

//       }
//     break;
//     // }
//     case FUNC_EVERY_SECOND: {

//     }     
//     break;
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case FUNC_JSON_COMMAND_ID:
//       parse_JSONCommand(obj);
//     break;
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case FUNC_MQTT_HANDLERS_INIT:
//     case FUNC_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init();
//     break;
//     case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
//       MQTTHandler_Set_TelePeriod();
//     break;
//     case FUNC_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     case FUNC_MQTT_CONNECTED:
//       MQTTHandler_Set_fSendNow();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//   }
  
//   /************
//    * WEBPAGE SECTION * 
//   *******************/
  
//   #ifdef USE_MODULE_NETWORK_WEBSERVER
//   return Tasker_Web(function);
//   #endif // USE_MODULE_NETWORK_WEBSERVER

// } // END Tasker


// void mGPS::pre_init(){

//   settings.fEnableModule = true;
//   gps_parser = new NMEAGPS(); 

// }


// void mGPS::init(void)
// {
 
//   #ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
//     gps_receive_buffer.bufused = 0;
//     gps_receive_buffer.buflen = 200;
//     gps_receive_buffer.buffer = new uint8_t[gps_receive_buffer.buflen];
//     gpsPort.begin(9600);
//   #endif // USE_DEVFEATURE_GPS_POLLING_INPUT

//   DEBUG_PORT.println( F("NMEASDlog.ino started!") );
//   DEBUG_PORT.print( F("fix size = ") );
//   DEBUG_PORT.println( sizeof(GPS_FIX) );
//   DEBUG_PORT.print( NMEAGPS_FIX_MAX );
//   DEBUG_PORT.println( F(" GPS updates can be buffered.") );
  
//   if (gps_parser->merging != NMEAGPS::EXPLICIT_MERGING)
//     DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );
  
// }



// #endif
