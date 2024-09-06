

    // #ifdef USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//       if(runtime.ubx_config_status==0)
//       {

// //         // Turn off the preconfigured NMEA standard messages
// //         configNMEA( 0 );

// //         // Turn off things that may be left on by a previous build
// //         disableUBX();

// //         // Change baud to higher rate
// //         ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );

// //         delay(1000);
// // Serial1.println("921600 Working");
// // Serial1.flush();
// // Serial1.end();
// // pinMode(17, OUTPUT);
// // digitalWrite(17, HIGH);
// // delay(1000);


        
// // Serial1.begin(115200);

//         // Turn on the exact UBX messages I need
//         // send_UBX_enable_messages();


//         // gpsPort.flush();
//         // // Change to high speed
//         // gpsPort.updateBaudRate(115200); 

//         // Increase send frequency to 10hz
//         // sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
//         sendUBX( ubxRate1Hz, sizeof(ubxRate1Hz) );

//         runtime.ubx_config_status = 1; //success

//       }

      // #endif


// // Without interrupts, 912600 is too fast for polling during config. Force speed back to 9600 for config

// #ifdef ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW1
//   ALOG_INF(PSTR("GPS baud changing to 921600"));
//   // Send command on 912600 to slower speed again for config
//   gpsPort.setDebugOutput(true);
//   gpsPort.begin(921600); 
//   // Wait for serial to start
//   while (!gpsPort){};
//   // Send command to lower baud to 9600
//   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
//   gpsPort.flush();
//   // change to new slower baud for polling init phase
//   delay(100);
//   ALOG_INF(PSTR("GPS baud changing to 9600"));
//   gpsPort.updateBaudRate(9600); 
//   // confirm its working on the lower baud, set a flag
//   if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//   {
//     ALOG_INF(PSTR("GPS baud 9600 Working"));
//   }
// #endif // ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW


// #ifdef ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW2

// DEBUG_LINE_HERE;



// Serial1.begin(921600);
// Serial1.println("921600 Working");
// Serial1.flush();
// Serial1.end();
// pinMode(17, OUTPUT);
// digitalWrite(17, HIGH);
// delay(1000);

// DEBUG_LINE_HERE;

// Serial1.begin(9600);
// Serial1.println("9600 Working");
// Serial1.flush();
// Serial1.end();
// pinMode(17, OUTPUT);
// digitalWrite(17, HIGH);
// delay(1000);

// DEBUG_LINE_HERE;

// delay(4000);


//   // ALOG_INF(PSTR("GPS baud changing to 921600"));
//   // // Send command on 912600 to slower speed again for config
//   // // gpsPort.setDebugOutput(true);

  
//   // ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 912600"));
//   // DEBUG_LINE_HERE;
//   // // Turn back to slow speed
//   // // gpsPort.updateBaudRate(921600); 
//   // // gpsPort.begin(921600); 
//   // // while (!gpsPort){};

//   // gpsPort.flush(); // wait for last transmitted data to be sent 
//   // gpsPort.begin(921600);
//   // while(gpsPort.available()){
//   //   DEBUG_LINE_HERE;
//   //   gpsPort.read(); 
//   // } 



//   // DEBUG_LINE_HERE;
//   // CommandSend_NMEA_Disable_NMEAMessages(921600);
//   // DEBUG_LINE_HERE;
//   // CommandSend_UBX_Disable_NMEAMessages(921600);
//   // DEBUG_LINE_HERE;
//   // CommandSend_UBX_Disable_UBXMessages(921600);
//   // DEBUG_LINE_HERE;
  
//   // // // Wait for serial to start
//   // // while (!gpsPort){};
//   // // Send command to lower baud to 9600
//   // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
//   // // DEBUG_LINE_HERE;
//   // // gpsPort.flush();
//   // // gpsPort.end();

//   // // DEBUG_LINE_HERE;
//   // // delay(5000);
//   // // DEBUG_LINE_HERE;

//   // ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 9600"));
  
//   // gpsPort.flush(); // wait for last transmitted data to be sent 
//   // gpsPort.begin(9600);
//   // while(gpsPort.available()) gpsPort.read(); 

//   // // Serial1.begin(921600); 



//   // // gpsPort.begin(9600); 
//   // // while (!gpsPort){};

//   // ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds"));
  
//   // DEBUG_LINE_HERE;
//   // CommandSend_NMEA_Disable_NMEAMessages(9600);
//   // DEBUG_LINE_HERE;
//   // CommandSend_UBX_Disable_NMEAMessages(9600);
//   // DEBUG_LINE_HERE;
//   // CommandSend_UBX_Disable_UBXMessages(9600);

//   // gpsPort.flush();
//   // // gpsPort.end();



//   // // change to new slower baud for polling init phase
//   // // DEBUG_LINE_HERE;
//   // // delay(100);
//   // // ALOG_INF(PSTR("GPS baud changing to 9600"));
//   // // DEBUG_LINE_HERE;
//   // // gpsPort.updateBaudRate(9600); 
//   // // confirm its working on the lower baud, set a flag
//   // DEBUG_LINE_HERE;
//   // if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//   // {
//   //   ALOG_INF(PSTR("GPS baud 9600 Working"));
//   //   delay(2000);
//   // }else{
//   //   ALOG_INF(PSTR("GPS baud 9600 NO RESPONSE"));
//   //   delay(2000);

//   // }
//   // DEBUG_LINE_HERE;
// #endif // ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW


// #ifdef ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW3

// // DEBUG_LINE_HERE;



// // DEBUG_LINE_HERE;

// // DEBUG_LINE_HERE;

// // delay(4000);


//   ALOG_INF(PSTR("GPS baud changing to 115200"));
//   // Send command on 912600 to slower speed again for config
//   // gpsPort.setDebugOutput(true);

  
//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 115200"));
//   DEBUG_LINE_HERE;
//   // Turn back to slow speed
//   // gpsPort.updateBaudRate(921600); 
//   // gpsPort.begin(921600); 
//   // while (!gpsPort){};

  
// Serial1.begin(9600);
// delay(4000);
// // while(1)
// Serial1.println("115200 Working");
// Serial1.flush();

// DEBUG_HOLD_POINT;

//   DEBUG_LINE_HERE;
//   CommandSend_NMEA_Disable_NMEAMessages(115200);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_NMEAMessages(115200);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_UBXMessages(115200);
//   DEBUG_LINE_HERE;
  
//   // // Wait for serial to start
//   // while (!gpsPort){};
//   // Send command to lower baud to 9600
//   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
//   // DEBUG_LINE_HERE;
//   // gpsPort.flush();
//   // gpsPort.end();

// Serial1.end();
// pinMode(17, OUTPUT);
// digitalWrite(17, HIGH);
// delay(1000);
//   // DEBUG_LINE_HERE;
//   // delay(5000);
//   // DEBUG_LINE_HERE;

//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 9600"));
  

//   DEBUG_LINE_HERE;
// Serial1.begin(9600);
//   DEBUG_LINE_HERE;
// Serial1.println("9600 Working");
//   DEBUG_LINE_HERE;

//   // Serial1.begin(921600); 



//   // gpsPort.begin(9600); 
//   // while (!gpsPort){};

//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds"));
  
//   DEBUG_LINE_HERE;
//   CommandSend_NMEA_Disable_NMEAMessages(9600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_NMEAMessages(9600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_UBXMessages(9600);

//   gpsPort.flush();
//   // gpsPort.end();


// // Serial1.flush();
// // Serial1.end();
// // pinMode(17, OUTPUT);
// // digitalWrite(17, HIGH);
// // delay(1000);

//   // change to new slower baud for polling init phase
//   // DEBUG_LINE_HERE;
//   // delay(100);
//   // ALOG_INF(PSTR("GPS baud changing to 9600"));
//   // DEBUG_LINE_HERE;
//   // gpsPort.updateBaudRate(9600); 
//   // confirm its working on the lower baud, set a flag
//   DEBUG_LINE_HERE;
//   if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//   {
//     ALOG_INF(PSTR("GPS baud 9600 Working"));
//     delay(2000);
//   }else{
//     ALOG_INF(PSTR("GPS baud 9600 NO RESPONSE"));
//     delay(2000);

//   }
//   // DEBUG_LINE_HERE;
// #endif // ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW


// #ifdef ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW4

// /**
//  * Test, try change 9 to 9600
//  * */
//   ALOG_INF(PSTR("GPS baud changing from 921600 to 9600"));


// // DEBUG_LINE_HERE;


// Serial1.begin(115200);
// Serial1.println("115200 Working");
// Serial1.flush();
// // Serial1.end();
// // pinMode(17, OUTPUT);
// // digitalWrite(17, HIGH);
// // delay(1000);

//   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );


//   DEBUG_HOLD_POINT;

// // DEBUG_LINE_HERE;

// // DEBUG_LINE_HERE;

// // delay(4000);


//   // Send command on 912600 to slower speed again for config
//   // gpsPort.setDebugOutput(true);

  
//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 912600"));
//   DEBUG_LINE_HERE;
//   // Turn back to slow speed
//   // gpsPort.updateBaudRate(921600); 
//   // gpsPort.begin(921600); 
//   // while (!gpsPort){};

  


//   DEBUG_LINE_HERE;
//   CommandSend_NMEA_Disable_NMEAMessages(921600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_NMEAMessages(921600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_UBXMessages(921600);
//   DEBUG_LINE_HERE;
  
//   // // Wait for serial to start
//   // while (!gpsPort){};
//   // Send command to lower baud to 9600
//   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
//   // DEBUG_LINE_HERE;
//   // gpsPort.flush();
//   // gpsPort.end();

//   DEBUG_LINE_HERE;
//   delay(5000);
//   DEBUG_LINE_HERE;

//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 9600"));
  

//   DEBUG_LINE_HERE;
// Serial1.begin(9600);
//   DEBUG_LINE_HERE;
// Serial1.println("9600 Working");
//   DEBUG_LINE_HERE;
// Serial1.flush();
// Serial1.end();
// pinMode(17, OUTPUT);
// digitalWrite(17, HIGH);
// delay(1000);

//   // Serial1.begin(921600); 



//   // gpsPort.begin(9600); 
//   // while (!gpsPort){};

//   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds"));
  
//   DEBUG_LINE_HERE;
//   CommandSend_NMEA_Disable_NMEAMessages(9600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_NMEAMessages(9600);
//   DEBUG_LINE_HERE;
//   CommandSend_UBX_Disable_UBXMessages(9600);

//   gpsPort.flush();
//   // gpsPort.end();



//   // change to new slower baud for polling init phase
//   // DEBUG_LINE_HERE;
//   // delay(100);
//   // ALOG_INF(PSTR("GPS baud changing to 9600"));
//   // DEBUG_LINE_HERE;
//   // gpsPort.updateBaudRate(9600); 
//   // confirm its working on the lower baud, set a flag
//   DEBUG_LINE_HERE;
//   if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//   {
//     ALOG_INF(PSTR("GPS baud 9600 Working"));
//     delay(2000);
//   }else{
//     ALOG_INF(PSTR("GPS baud 9600 NO RESPONSE"));
//     delay(2000);

//   }
//   // DEBUG_LINE_HERE;
// #endif // ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW





// // // delay(1000);


// // DEBUG_HOLD_POINT;




// // // DEBUG_LINE_HERE;



// // // DEBUG_LINE_HERE;

// // // DEBUG_LINE_HERE;

// // // delay(4000);


// //   ALOG_INF(PSTR("GPS baud changing to 115200"));
// //   // Send command on 912600 to slower speed again for config
// //   // gpsPort.setDebugOutput(true);

  
// //   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 115200"));
// //   DEBUG_LINE_HERE;
// //   // Turn back to slow speed
// //   // gpsPort.updateBaudRate(921600); 
// //   // gpsPort.begin(921600); 
// //   // while (!gpsPort){};

  
// // Serial1.begin(9600);
// // // delay(4000);
// // // // while(1)
// // // Serial1.println("115200 Working");
// // // Serial1.flush();

// // // DEBUG_HOLD_POINT;

// //   DEBUG_LINE_HERE;
// //   CommandSend_NMEA_Disable_NMEAMessages(9600);
// //   // DEBUG_LINE_HERE;
// //   // CommandSend_UBX_Disable_NMEAMessages(9600);
// //   // DEBUG_LINE_HERE;
// //   // CommandSend_UBX_Disable_UBXMessages(9600);
// //   // DEBUG_LINE_HERE;
  
// //   // // Wait for serial to start
// //   // while (!gpsPort){};
// //   // Send command to lower baud to 9600
// //   // ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
// //   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
// //   // DEBUG_LINE_HERE;
// //   // gpsPort.flush();
// //   // gpsPort.end();

// // // Serial1.end();
// // // pinMode(17, OUTPUT);
// // // digitalWrite(17, HIGH);
// // // delay(1000);
// //   // DEBUG_LINE_HERE;
// //   // delay(5000);
// //   // DEBUG_LINE_HERE;

// //   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds ======================= 9600"));
  

// // //   DEBUG_LINE_HERE;
// // // Serial1.begin(9600);
// // //   DEBUG_LINE_HERE;
// // // Serial1.println("9600 Working");
// // //   DEBUG_LINE_HERE;

// // //   // Serial1.begin(921600); 



// // //   // gpsPort.begin(9600); 
// // //   // while (!gpsPort){};

// // //   ALOG_INF(PSTR("GPS Disable all unwanted messages at all bauds"));
  
// // //   DEBUG_LINE_HERE;
// // //   CommandSend_NMEA_Disable_NMEAMessages(9600);
// // //   DEBUG_LINE_HERE;
// // //   CommandSend_UBX_Disable_NMEAMessages(9600);
// // //   DEBUG_LINE_HERE;
// // //   CommandSend_UBX_Disable_UBXMessages(9600);

// // //   gpsPort.flush();
// // //   // gpsPort.end();


// // // Serial1.flush();
// // // Serial1.end();
// // // pinMode(17, OUTPUT);
// // // digitalWrite(17, HIGH);
// // // delay(1000);

// //   // change to new slower baud for polling init phase
// //   // DEBUG_LINE_HERE;
// //   // delay(100);
// //   // ALOG_INF(PSTR("GPS baud changing to 9600"));
// //   // DEBUG_LINE_HERE;
// //   // gpsPort.updateBaudRate(9600); 
// //   // confirm its working on the lower baud, set a flag
// //   DEBUG_LINE_HERE;
// //   if(ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
// //   {
// //     ALOG_INF(PSTR("GPS baud 9600 Working"));
// //     delay(2000);
// //   }else{
// //     ALOG_INF(PSTR("GPS baud 9600 NO RESPONSE"));
// //     delay(2000);

// //   }

// // 22:50:57  R -> UNKNOWN ???,  Size  33,  'UNKNOWN'
// // 22:50:57  R <- UBX MON,  Size   8,  'Monitor'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> NMEA PUBX40,  Size  29,  'Config Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> UBX CFG-MSG,  Size  16,  'Messages'
// // 22:50:57  R -> NMEA PUBX41,  Size  28,  'Config Ports'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> UBX CFG,  Size  11,  'Config'
// // 22:50:57  R -> NMEA PUBX41,  Size  28,  'Config Ports'



//   // DEBUG_LINE_HERE;

//   // 
//   //   Serial1.setDebugOutput(true);
//   //     gpsPort.begin(9600); 
//   // // Make sure if its on 9600 baud to switch to 921600

//   // uint32_t baud_rates[] = {9600, 921600};

//   // for(int i=0; i<ARRAY_SIZE(baud_rates); i++)
//   // {
//   //   ALOG_TST(PSTR("GPS: Setting baud rate baud_rates[i] %d"),baud_rates[i]);
//   //   DEBUG_LINE_HERE;
//   //   // Serial.flush();
//   //   DEBUG_LINE_HERE;
//   //   // delay(1000);
//   //   // DEBUG_LINE_HERE;

//   //   // updateBaudRate

//   //   switch(i)
//   //   {
//   //     default:
//   //     case 0: 
//   //   DEBUG_LINE_HERE;
//   //     gpsPort.updateBaudRate(9600); 
//   //   DEBUG_LINE_HERE;
//   //     break;
//   //     case 1:
//   //   DEBUG_LINE_HERE; 
//   //   gpsPort.updateBaudRate(921600); 
//   //   DEBUG_LINE_HERE;
//   //   break;
//   //   }
    
//   // //   DEBUG_LINE_HERE;
//   //   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
//   // //   ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
//   //   DEBUG_LINE_HERE;
//   //   gpsPort.flush();
//   //   DEBUG_LINE_HERE;
//   //   // gpsPort.end();
//   //   DEBUG_LINE_HERE;
//   //   delay(100);
//   //   DEBUG_LINE_HERE;
//   // }


//   // // // long baud = 115200;


//   // gpsPort.begin(921600);



// /**
//  * @brief Configure messages
//  * */
// void mGPS::Init_UBX_Only()
// {

// //   runtime.ubx_config_status = 0; 

// //   // Check pins for serial have been defined before proceeeding
// //   if(!(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID))) {
// //     AddLog(LOG_LEVEL_ERROR, PSTR("No gpsPort (Serial1) pins have been set"));
// //     return;
// //   }

// //   uint32_t start_millis = millis();
// //   ALOG_TST(PSTR("GPS: Setting baud rate started"));

// //   /**
// //    * Step 1: Ensure baud is set the default for configuration
// //    * */
// //   uint32_t baud_list[] = {D_GPS_BAUD_RATE_DEFAULT, D_GPS_BAUD_RATE_FAST, 921600};

// //   for(int i=0;i<ARRAY_SIZE(baud_list);i++)
// //   {
// //     // Begin connection
// //     //unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms
// //     ALOG_TST(PSTR("Baud Test %d on TX%d, RX%d"), baud_list[i], pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID));
// //     gpsPort.begin(baud_list[i], SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
// //     // while(!gpsPort);
// //     // Send default baud command
// //     switch(D_GPS_BAUD_RATE_DEFAULT)
// //     {
// //       default:
// //       case 9600:
// //         ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
// //       break;
// //       case 115200:
// //         ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
// //       break;
// //       case 921600:
// //         ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud921600 );
// //       break;
// //     }
// //     // Complete transmission
// //     gpsPort.flush();
// //     gpsPort.end();
// //     pinMode(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), OUTPUT); digitalWrite(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID), HIGH);
// //     pinMode(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), OUTPUT); digitalWrite(pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), HIGH);
// //     // Allow time for the GPS to switch baud speeds
// //     // delay(500);
// //   }

// //   // Begin at default speed to start configuring messages
// //   gpsPort.begin(D_GPS_BAUD_RATE_DEFAULT, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
// //   // while(!gpsPort);

// //   /**
// //    * Disable unwanted signals
// //    * */
// //   CommandSend_UBX_Disable_NMEAMessages();
// //   DEBUG_LINE_HERE;
// //   CommandSend_UBX_Disable_UBXMessages();
// //   DEBUG_LINE_HERE;

// //   // configNMEA( 0 );
// //   // disableUBX();

// // // B5 62 06 00 01 00 01 08 22

// //   /**
// //    * Renable what I want
// //    * */
// //   // enableUBX();
// //   runtime.ubx_messages_confirmed_enabled = enableUBX_RequiredOnlyFor3DTracking();
// //   ALOG_TST(PSTR("runtime.ubx_messages_confirmed_enabled = %d"),runtime.ubx_messages_confirmed_enabled);

// //   /**
// //    * Change baud rate to desired
// //    * */
// //   switch(D_GPS_BAUD_RATE_FAST)
// //   {
// //     default:
// //     case 9600:
// //       ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud9600 );
// //     break;
// //     case 115200:
// //       ubx_parser->send_P( &gpsPort, (const __FlashStringHelper *) baud115200 );
// //     break;
// //   }
// //   gpsPort.flush();
// //   gpsPort.end();
// //   gpsPort.begin(D_GPS_BAUD_RATE_FAST, SERIAL_8N1, pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID));
// //   // while(!gpsPort);

// //   /**
// //    * Set the rate
// //    * */
// //   // sendUBX( ubxRate1Hz, sizeof(ubxRate1Hz) );
// //   sendUBX( ubxRate5Hz, sizeof(ubxRate5Hz) );
// //   // sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
 
// //   ALOG_TST(PSTR(DEBUG_INSERT_PAGE_BREAK "GPS: Setting baud rate finsihed, %d ms"), millis()-start_millis);

// }