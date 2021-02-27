#include "mGPS.h"

// For now, lets use polling for gps data until I get the rest working for it
// I might want to move this into the "controller" method, or simply require it needs other drivers? cross-drivers, does this make sense??
// Using that, a uart ISR can work to get the data, with maybe using the optional FreeRTOS method for triggering a parse event after message is complete? see how gps lib works


// I will inject my gps data in a formatted (searchable) way into the rss/sd card data stream, matlab later will find the nearest, the interopolate from either points beside it

#ifdef USE_MODULE_DRIVERS_GPS

gps_fix  fixcpp;// = nullptr; // This holds on to the latest values
gps_fix  fix_saved;// = nullptr; // This holds on to the latest values

static NMEAGPS   gps;
static void GPSisr( uint8_t c )
{
  gps.handle( c );

} // GPSisr


// /*********************************************************************************************/

void mGPS::init(void)
{
  
}




void mGPS::pre_init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

  gps = new NMEAGPS(); // This parses the GPS characters
//   fix = new gps_fix(); // This holds on to the latest values



#ifdef ENABLE_DEVFEATURE_GPSTEST1
// Start the normal trace output
  // DEBUG_PORT.begin(9600);
  // while (!DEBUG_PORT)
  //   ;

  // DEBUG_PORT.print( F("ublox binary protocol example started.\n") );
  // // DEBUG_PORT << F("fix object size = ") << sizeof(gps.fix()) << '\n';
  // // DEBUG_PORT << F("ubloxGPS object size = ") << sizeof(ubloxGPS) << '\n';
  // // DEBUG_PORT << F("MyGPS object size = ") << sizeof(gps) << '\n';
  // DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );
  // DEBUG_PORT.flush();

//   // Start the UART for the GPS device
//   #ifdef NMEAGPS_INTERRUPT_PROCESSING
//     gpsPort.attachInterrupt( GPSisr );
//   #endif
//   gpsPort.begin(9600);

//   // Turn off the preconfigured NMEA standard messages
//   configNMEA( 0 );

//   // Turn off things that may be left on by a previous build
//   disableUBX();

//   #if 0
//     // Test a Neo M8 message -- should be rejected by Neo-6 and Neo7
//     ublox::cfg_nmea_v1_t test;

//     test.always_output_pos  = false; // invalid or failed
//     test.output_invalid_pos = false;
//     test.output_invalid_time= false;
//     test.output_invalid_date= false;
//     test.use_GPS_only       = false;
//     test.output_heading     = false; // even if frozen
//     test.__not_used__       = false;

//     test.nmea_version = ublox::cfg_nmea_v1_t::NMEA_V_4_0;
//     test.num_sats_per_talker_id = ublox::cfg_nmea_v1_t::SV_PER_TALKERID_UNLIMITED;

//     test.compatibility_mode = false;
//     test.considering_mode   = true;
//     test.max_line_length_82 = false;
//     test.__not_used_1__     = 0;

//     test.filter_gps    = false;
//     test.filter_sbas   = false;
//     test.__not_used_2__= 0;
//     test.filter_qzss   = false;
//     test.filter_glonass= false;
//     test.filter_beidou = false;
//     test.__not_used_3__= 0;

//     test.proprietary_sat_numbering = false;
//     test.main_talker_id = ublox::cfg_nmea_v1_t::MAIN_TALKER_ID_GP;
//     test.gsv_uses_main_talker_id = true;
//     test.beidou_talker_id[0] = 'G';
//     test.beidou_talker_id[1] = 'P';

//     DEBUG_PORT << F("CFG_NMEA result = ") << gps.send( test );
//   #endif

//   while (!gps.running())
//     if (gps.available( gpsPort ))
//       gps.read();


  DEBUG_PORT.println( F("NMEASDlog.ino started!") );
  DEBUG_PORT.print( F("fix size = ") );
  DEBUG_PORT.println( sizeof(gps_fix) );
  DEBUG_PORT.print( NMEAGPS_FIX_MAX );
  DEBUG_PORT.println( F(" GPS updates can be buffered.") );
  
  // if (gps.merging != NMEAGPS::EXPLICIT_MERGING)
  //   DEBUG_PORT.println( F("Warning: EXPLICIT_MERGING should be enabled for best results!") );

  // gpsPort.attachInterrupt( GPSisr );
  gpsPort.begin(115200);

  fixcpp.init();

  
  //  Configure the GPS.  These are commands for MTK GPS devices.  Other
  //    brands will have different commands.
  // gps.send_P( &gpsPort, F("PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0") ); // RMC only for MTK GPS devices
  // gps.send_P( &gpsPort, F("PMTK220,100") ); // 10Hz update rate for MTK GPS devices


#endif

}


int8_t mGPS::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  //if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:{


      uint32_t time = millis();
          
      while (gps->available( gpsPort )) {
        fixcpp = gps->read();

        if(fixcpp.status > gps_fix::STATUS_NONE ){

          //save tmp solution when valid
          fix_saved = fixcpp;

          // DEBUG_PORT.print( F("Location: ") );
          if (fixcpp.valid.location) {
            DEBUG_PORT.print( fixcpp.latitude(), 6 );
            DEBUG_PORT.print( ',' );
            DEBUG_PORT.print( fixcpp.longitude(), 6 );
            DEBUG_PORT.print( ',' );
            DEBUG_PORT.print( fixcpp.altitude_cm(), 6 ); DEBUG_PORT.print( "cm" );
          }

          // DEBUG_PORT.print( F(", Altitude: ") );
          if (fixcpp.valid.altitude)
            DEBUG_PORT.print( fixcpp.altitude() );

          DEBUG_PORT.println();
        }

        if(time > 1000){
          break;
        }

      }

        
// #ifdef ENABLE_DEVFEATURE_GPSTEST1


//   if (gps.available( gpsPort ))
//     // trace_all( DEBUG_PORT, gps, gps.read() );

//   // If the user types something, reset the message configuration
//   //   back to a normal set of NMEA messages.  This makes it
//   //   convenient to switch to another example program that
//   //   expects a typical set of messages.  This also saves
//   //   putting those config messages in every other example.

//   if (DEBUG_PORT.available()) {
//     do { DEBUG_PORT.read(); } while (DEBUG_PORT.available());
//     DEBUG_PORT.println( F("Stopping...") );

//     configNMEA( 1 );
//     disableUBX();
//     gpsPort.flush();
//     gpsPort.end();

//     DEBUG_PORT.println( F("STOPPED.") );
//     for (;;);
//   }

// if(gps->available( gpsPort )){
  // if(Serial2.available()){
  //   while (Serial2.available( )) {
  //     char in = Serial2.read();
  //     Serial.print(in);
  //   }
  // }
// }

uint32_t timeout = millis();


    // while (gps->available( gpsPort )) {
    //     // gps_fix  
    //     fixcpp = gps->read();

    //     // // Serial.print( millis() );
    //     // Serial.print( F(" LocationNEW: ") );
    //     // // // if (fix.valid.location) {
    //     //   DEBUG_PORT.print( fixcpp.latitude(), 6 );
    //     //   DEBUG_PORT.print( ',' );
    //     //   DEBUG_PORT.print( fixcpp.longitude(), 6 );
    //     // // }

    //     // DEBUG_PORT.print( F(", Altitude: ") );
    //     // // if (fix.valid.altitude)
    //     //   DEBUG_PORT.print( fix.altitude() );
        
    // // Serial.print( millis() );
    // // Serial.print( F(" LocationWHILE: ") );
    // // // if (fix.valid.location) {
    // //   DEBUG_PORT.print( fix2.latitude(), 6 );
    // //   DEBUG_PORT.print( ',' );
    // //   DEBUG_PORT.print( fix2.longitude(), 6 );
    // // // }

    // // DEBUG_PORT.print( F(", Altitude: ") );
    // // // if (fix.valid.altitude)
    // //   DEBUG_PORT.print( fix2.altitude() );

    // // DEBUG_PORT.println();
    //   if (gps->nmeaMessage == LAST_SENTENCE_IN_INTERVAL){


    //       DEBUG_PORT.print( fixcpp.latitude(), 6 );
    //       DEBUG_PORT.print( 'llllllllllllll,' );
    //       DEBUG_PORT.print( fixcpp.longitude(), 6 );


    //   }


    // if(mTime::TimeReached(&timeout, 2000)){
      
    //     AddLog_P(LOG_LEVEL_TEST,PSTR(" Forced blocked loop timeout!!") );
    //     AddLog_P(LOG_LEVEL_TEST,PSTR(" LocationTO: ") );
    //     // // if (fix.valid.location) {
    //       DEBUG_PORT.print( fixcpp.dateTime.seconds);


    //       DEBUG_PORT.print( fixcpp.latitude(), 6 );
    //       DEBUG_PORT.print( ',' );
    //       DEBUG_PORT.print( fixcpp.longitude(), 6 );

          



    // }

    // //     DEBUG_PORT.println();
    // }

    


    

    // #endif
    break;
    }
    case FUNC_EVERY_SECOND: {


        // Serial.print( millis() );
        // AddLog_P(LOG_LEVEL_TEST,PSTR(" LocationSEC: ") );
        // // // if (fix.valid.location) {
        //   DEBUG_PORT.print( fixcpp.latitude(), 6 );
        //   DEBUG_PORT.print( ',' );
        //   DEBUG_PORT.print( fixcpp.longitude(), 6 );
        // // }
  
    // fix = gps->read();
    // //   DEBUG_PORT.print( fix.latitude(), 6 );
    // //   DEBUG_PORT.print( ',' );
    // //   DEBUG_PORT.println( fix.longitude(), 6 );

    // //    fix = gps.read();

    // // Serial.print( millis() );
    // Serial.print( F(" LocationSECOND: ") );
    // // if (fix.valid.location) {
    //   DEBUG_PORT.print( fix->latitude(), 6 );
    //   DEBUG_PORT.print( ',' );
    //   DEBUG_PORT.print( fix->longitude(), 6 );
    // // }

    // DEBUG_PORT.print( F(", Altitude: ") );
    // // if (fix.valid.altitude)
    //   DEBUG_PORT.print( fix->altitude() );

    // DEBUG_PORT.println();


    }     
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
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
    #endif //USE_MQTT
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END Tasker


// int8_t mGPS::Tasker(uint8_t function, JsonObjectConst obj){
//   switch(function){
//     case FUNC_JSON_COMMAND_OBJECT:
//       parse_JSONCommand(obj);
//     break;
//     case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
//       return CheckAndExecute_JSONCommands(obj);
//     break;
//   }
// }
// int8_t mGPS::CheckAndExecute_JSONCommands(JsonObjectConst obj){

//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/ifan")>=0){
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//       parse_JSONCommand(obj);
//       return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }


/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS::ConstructJSON_GPSPacket_Minimal(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Level_Start("Location");
      JsonBuilderI->Add("latitudeL", fix_saved.latitudeL()); 
      JsonBuilderI->Add("latitude", fix_saved.latitude());
      JsonBuilderI->Add("longitudeL", fix_saved.longitudeL());
      JsonBuilderI->Add("longitude", fix_saved.longitude());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Altitude");
      JsonBuilderI->Add("altitude_cm", fix_saved.altitude_cm()); 
      JsonBuilderI->Add("altitude", fix_saved.altitude());
      JsonBuilderI->Add("altitude_ft", fix_saved.altitude_ft());
    JsonBuilderI->Level_End();


    JsonBuilderI->Level_Start("Speed");
      JsonBuilderI->Add("speed_mkn", fix_saved.speed_mkn()); 
      JsonBuilderI->Add("speed", fix_saved.speed());
      JsonBuilderI->Add("speed_kph", fix_saved.speed_kph());
      JsonBuilderI->Add("speed_metersph", fix_saved.speed_metersph());
      JsonBuilderI->Add("speed_mph", fix_saved.speed_mph());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Heading");
      JsonBuilderI->Add("heading_cd", fix_saved.heading_cd()); 
      JsonBuilderI->Add("heading", fix_saved.heading());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("geoidHt");
      JsonBuilderI->Add("geoidHeight_cm", fix_saved.geoidHeight_cm()); 
      JsonBuilderI->Add("geoidHeight", fix_saved.geoidHeight());
    JsonBuilderI->Level_End();

    JsonBuilderI->Add("satellites", fix_saved.satellites); 

    JsonBuilderI->Level_Start("Dilution");
      JsonBuilderI->Add("hdop", fix_saved.hdop); 
      JsonBuilderI->Add("vdop", fix_saved.vdop);
      JsonBuilderI->Add("pdop", fix_saved.pdop);
      JsonBuilderI->Add("lat_err", fix_saved.lat_err());
      JsonBuilderI->Add("lon_err", fix_saved.lon_err());
      JsonBuilderI->Add("alt_err", fix_saved.alt_err());
      JsonBuilderI->Add("spd_err", fix_saved.spd_err());
      JsonBuilderI->Add("hdg_err", fix_saved.hdg_err());
      JsonBuilderI->Add("spd_err", fix_saved.spd_err());
      JsonBuilderI->Add("time_err", fix_saved.time_err());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Millis");

      JsonBuilderI->Add("GGA",millis()-fix_saved.active_millis.GGA);
      JsonBuilderI->Add("GLL",millis()-fix_saved.active_millis.GLL);
      JsonBuilderI->Add("GSA",millis()-fix_saved.active_millis.GSA);
      JsonBuilderI->Add("GST",millis()-fix_saved.active_millis.GST);
      JsonBuilderI->Add("GSV",millis()-fix_saved.active_millis.GSV);
      JsonBuilderI->Add("RMC",millis()-fix_saved.active_millis.RMC);
      JsonBuilderI->Add("VTG",millis()-fix_saved.active_millis.VTG);
      JsonBuilderI->Add("ZDA",millis()-fix_saved.active_millis.ZDA);


    JsonBuilderI->Level_End();




    /*
  
  #ifdef GPS_FIX_LOCATION_DMS
    DMS_t latitudeDMS;
    DMS_t longitudeDMS;
  #endif

*/


    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


uint8_t mGPS::ConstructJSON_Settings(uint8_t json_method){

  // Active rgbcct palette used as scene

  //Serial.println("mGPS::ConstructJSON_Settings");

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


////////////////////// START OF MQTT /////////////////////////

void mGPS::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Settings;


  mqtthandler_ptr = &mqtthandler_gpspacket_minimal_teleperiod; //also ifchanged together
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_Minimal;

  
//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Scene;

//   mqtthandler_ptr = &mqtthandler_sensdebug_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Debug;

} //end "MQTTHandler_Init"


void mGPS::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
//   mqtthandler_scene_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mGPS::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  
} //end "MQTTHandler_Set_TelePeriod"


void mGPS::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID,
    MQTT_HANDLER_MODULE_GPSPACKET_MINIMAL_IFCHANGED_ID
    //, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mGPS>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_gpspacket_minimal_teleperiod
    //, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_DRIVERS_GPS_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, 
    sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
    mqtt_handler_id
  );

}


// void mGPS::parse_JSONCommand(JsonObjectConst obj){

  

//   if(obj.containsKey(D_JSON_FANSPEED)){
//     int speed = obj[D_JSON_FANSPEED];
//     test_val = speed;
//     if(speed>0){
//       analogWrite(pin, test_val);
//     }else
//     if(speed==1023){
//       digitalWrite(pin, HIGH);      
//     }else{
//       digitalWrite(pin, LOW);
//     }
//     // if(speed>3){
//     //   Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
//     //   speed=0; //default off
//     // }      
//     // SonoffIFanSetFanspeed(speed, false);
//     // AddLog_P(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
//     // Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
//     // isserviced++;
//   }
  
  

// }




// int8_t mGPS::Tasker_Web(uint8_t function){

//   switch(function){
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       // create command list
//       // char dlist[100]; memset(dlist,0,sizeof(dlist));
//       // pCONT_sup->AppendDList(dlist, D_JSON_LIGHTPOWER);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       // pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

//       uint8_t button_values[5] = {2, 0, 1, 2, 3}; //toggle, fanspeed0-3
          
//       // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Fan Controls"),"");

//       char button_value_ctr[10];
//       char button_key_ctr[50];
//       char button_text_ctr[30];

//       // BufferWriterI->Append_P(PSTR("{t}<tr>"));

//       if(settings.fShowManualSlider){
//         BufferWriterI->Append_P(PSTR("<div> Fan Speed </div>"), test_val);
//         BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//           WEB_HANDLE_SDCARD_SLIDER,               // c - Unique HTML id
//           PSTR("#000"), PSTR("#eee"),   // Black to White
//           4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//           0, 1023,  // Range 0/1 to 100%
//           test_val,
//           WEB_HANDLE_SDCARD_SLIDER
//         );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
//       }

//         // for(uint8_t button_id=0;button_id<5;button_id++){
//         //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR, 
//         //                             100/(button_id==0?1:4),
//         //                             button_id==0?"4":"", 
//         //                             "buttonh",
//         //                             pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
//         //                             pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
//         //                             pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls),
//         //                             ""
//         //                         );
//         //   // LightPower button gets its own row
//         //   if(button_id==0){ BufferWriterI->Append_P(PSTR("</tr><tr>")); }
//         // }
//       // BufferWriterI->Append_P(PSTR("</tr>{t2}"));

//     }break; 
//     case FUNC_WEB_COMMAND:
//       WebCommand_Parse();      
//     break;
//   }

// }



// void mGPS::WebCommand_Parse(void)
// {
//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "mRGBAnimator::WebCommand_Parse"));

//   char tmp[100];

//   uint8_t  arg_value = 0;
//  if(pCONT_web->request_web_command == nullptr){
//   Serial.println("nullptr"); 
//   return; 
// }

//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));
  
//   sprintf_P(arg_ctr,PSTR(WEB_HANDLE_SDCARD_SLIDER));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     test_val = arg_value;

//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//       analogWrite(pin, test_val);
//     // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     // SetRefreshLEDs();
//   }

// }



// uint8_t mGPS::ConstructJSON_Settings(uint8_t json_method){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P("test",0);  
//   JsonBuilderI->End();

// }

// uint8_t mGPS::ConstructJSON_Sensor(uint8_t json_method){

//   JsonBuilderI->Start();
//     JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
//     JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
//   JsonBuilderI->End();

// }




// /*********************************************************************************************************************************************
// ******** MQTT **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mGPS::MQTTHandler_Init(){

//   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Settings;

//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 600; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Sensor;

//   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"


// void mGPS::MQTTHandler_Set_fSendNow(){

//   mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_teleperiod.flags.SendNow = true;

// } //end "MQTTHandler_Init"


// void mGPS::MQTTHandler_Set_TelePeriod(){

//   // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// } //end "MQTTHandler_Set_TelePeriod"


// void mGPS::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t flag_handle_all = false, handler_found = false
//   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

//   // change switch to use array of pointers?
//   do{

//     switch(mqtt_handler_id){
//       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
//       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
//       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
//       default: handler_found=false; break; // nothing 
//     } // switch

//     // Pass handlers into command to test and (ifneeded) execute
//     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_SDCARD_ID,mqtthandler_ptr); }

//     // stop searching
//     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

//   }while(flag_handle_all);

// }

////////////////////// END OF MQTT /////////////////////////


#endif
