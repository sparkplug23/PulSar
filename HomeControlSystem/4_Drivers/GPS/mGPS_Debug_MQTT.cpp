#include "mGPS.h"

// Debug methods when designing, will not be included on logger

// For now, lets use polling for gps_parser data until I get the rest working for it
// I might want to move this into the "controller" method, or simply require it needs other drivers? cross-drivers, does this make sense??
// Using that, a uart ISR can work to get the data, with maybe using the optional FreeRTOS method for triggering a parse event after message is complete? see how gps_parser lib works


// I will inject my gps_parser data in a formatted (searchable) way into the rss/sd card data stream, matlab later will find the nearest, the interopolate from either points beside it

#ifdef USE_MODULE_DRIVERS_GPS

#ifdef ENABLE_MODULE_DRIVERS_GPS_WIRELESS_FEATURES_FOR_DEBUGGING



uint8_t mGPS::ConstructJSON_GPSPacket_Debug(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Level_Start("Millis");
      JsonBuilderI->Add("GGA",gps_parser->active_millis.GGA);
      JsonBuilderI->Add("GLL",gps_parser->active_millis.GLL);
      JsonBuilderI->Add("GSA",gps_parser->active_millis.GSA);
      JsonBuilderI->Add("GST",gps_parser->active_millis.GST);
      JsonBuilderI->Add("GSV",gps_parser->active_millis.GSV);
      JsonBuilderI->Add("RMC",gps_parser->active_millis.RMC);
      JsonBuilderI->Add("VTG",gps_parser->active_millis.VTG);
      JsonBuilderI->Add("ZDA",gps_parser->active_millis.ZDA);
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Millis2");
      JsonBuilderI->Add("GGA",millis()-gps_parser->active_millis.GGA);
      JsonBuilderI->Add("GLL",millis()-gps_parser->active_millis.GLL);
      JsonBuilderI->Add("GSA",millis()-gps_parser->active_millis.GSA);
      JsonBuilderI->Add("GST",millis()-gps_parser->active_millis.GST);
      JsonBuilderI->Add("GSV",millis()-gps_parser->active_millis.GSV);
      JsonBuilderI->Add("RMC",mTime::MillisElapsed(gps_parser->active_millis.RMC));
      JsonBuilderI->Add("VTG",millis()-gps_parser->active_millis.VTG);
      JsonBuilderI->Add("ZDA",millis()-gps_parser->active_millis.ZDA);
    JsonBuilderI->Level_End();

    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}



/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS::ConstructJSON_GPSPacket_Minimal(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Level_Start("Location");
      JsonBuilderI->Add("latitudeL", gps_result_stored.latitudeL()); 
      JsonBuilderI->Add("latitude", gps_result_stored.latitude());
      JsonBuilderI->Add("longitudeL", gps_result_stored.longitudeL());
      JsonBuilderI->Add("longitude", gps_result_stored.longitude());
    JsonBuilderI->Level_End();

    // JsonBuilderI->Level_Start("Altitude");
    //   JsonBuilderI->Add("altitude_cm", gps_result_stored.altitude_cm()); 
    //   JsonBuilderI->Add("altitude", gps_result_stored.altitude());
    //   JsonBuilderI->Add("altitude_ft", gps_result_stored.altitude_ft());
    // JsonBuilderI->Level_End();


    // JsonBuilderI->Level_Start("Speed");
    //   JsonBuilderI->Add("speed_mkn", gps_result_stored.speed_mkn()); 
    //   JsonBuilderI->Add("speed", gps_result_stored.speed());
    //   JsonBuilderI->Add("speed_kph", gps_result_stored.speed_kph());
    //   JsonBuilderI->Add("speed_metersph", gps_result_stored.speed_metersph());
    //   JsonBuilderI->Add("speed_mph", gps_result_stored.speed_mph());
    // JsonBuilderI->Level_End();

    // JsonBuilderI->Level_Start("Heading");
    //   JsonBuilderI->Add("heading_cd", gps_result_stored.heading_cd()); 
    //   JsonBuilderI->Add("heading", gps_result_stored.heading());
    // JsonBuilderI->Level_End();

    // JsonBuilderI->Level_Start("geoidHt");
    //   JsonBuilderI->Add("geoidHeight_cm", gps_result_stored.geoidHeight_cm()); 
    //   JsonBuilderI->Add("geoidHeight", gps_result_stored.geoidHeight());
    // JsonBuilderI->Level_End();

    // JsonBuilderI->Add("satellites", gps_result_stored.satellites); 

    // JsonBuilderI->Level_Start("Dilution");
    //   JsonBuilderI->Add("hdop", gps_result_stored.hdop); 
    //   JsonBuilderI->Add("vdop", gps_result_stored.vdop);
    //   JsonBuilderI->Add("pdop", gps_result_stored.pdop);
    //   JsonBuilderI->Add("lat_err", gps_result_stored.lat_err());
    //   JsonBuilderI->Add("lon_err", gps_result_stored.lon_err());
    //   JsonBuilderI->Add("alt_err", gps_result_stored.alt_err());
    //   JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
    //   JsonBuilderI->Add("hdg_err", gps_result_stored.hdg_err());
    //   JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
    //   JsonBuilderI->Add("time_err", gps_result_stored.time_err());
    // JsonBuilderI->Level_End();





    /*
  
  #ifdef GPS_FIX_LOCATION_DMS
    DMS_t latitudeDMS;
    DMS_t longitudeDMS;
  #endif

*/


    // JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS::ConstructJSON_GPSPacket_All(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

    JsonBuilderI->Level_Start("Location");
      JsonBuilderI->Add("latitudeL", gps_result_stored.latitudeL()); 
      JsonBuilderI->Add("latitude", gps_result_stored.latitude());
      JsonBuilderI->Add("longitudeL", gps_result_stored.longitudeL());
      JsonBuilderI->Add("longitude", gps_result_stored.longitude());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Altitude");
      JsonBuilderI->Add("altitude_cm", gps_result_stored.altitude_cm()); 
      JsonBuilderI->Add("altitude", gps_result_stored.altitude());
      JsonBuilderI->Add("altitude_ft", gps_result_stored.altitude_ft());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Speed");
      JsonBuilderI->Add("speed_mkn", gps_result_stored.speed_mkn()); 
      JsonBuilderI->Add("speed", gps_result_stored.speed());
      JsonBuilderI->Add("speed_kph", gps_result_stored.speed_kph());
      JsonBuilderI->Add("speed_metersph", gps_result_stored.speed_metersph());
      JsonBuilderI->Add("speed_mph", gps_result_stored.speed_mph());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Heading");
      JsonBuilderI->Add("heading_cd", gps_result_stored.heading_cd()); 
      JsonBuilderI->Add("heading", gps_result_stored.heading());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("geoidHt");
      JsonBuilderI->Add("geoidHeight_cm", gps_result_stored.geoidHeight_cm()); 
      JsonBuilderI->Add("geoidHeight", gps_result_stored.geoidHeight());
    JsonBuilderI->Level_End();

    JsonBuilderI->Add("satellites", gps_result_stored.satellites); 

    JsonBuilderI->Level_Start("Dilution");
      JsonBuilderI->Add("hdop", gps_result_stored.hdop); 
      JsonBuilderI->Add("vdop", gps_result_stored.vdop);
      JsonBuilderI->Add("pdop", gps_result_stored.pdop);
      JsonBuilderI->Add("lat_err", gps_result_stored.lat_err());
      JsonBuilderI->Add("lon_err", gps_result_stored.lon_err());
      JsonBuilderI->Add("alt_err", gps_result_stored.alt_err());
      JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
      JsonBuilderI->Add("hdg_err", gps_result_stored.hdg_err());
      JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
      JsonBuilderI->Add("time_err", gps_result_stored.time_err());
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

//https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_MessageOverview.html

/**
 * This will replace what PIC32 had, must also include the sample number? instead of sample, lets encode esp32 millis as extra parameter
 * The RSS samples of 50 rss will now also append millis from here, so when it appends the time from the ISR, add millis into it (so make special ISR for UART2)
 * Instead of "ConstructJSON_" I will also create a "ConstructRAWBytePacked_" for the sd method
 * */
uint8_t mGPS::ConstructJSON_GPSPacket_Micro(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();  

  JBI->Level_Start("SequenceNumber");
    JBI->Add("GPSCount", 0);
    JBI->Add("RSSCount", 0);
  JBI->Level_End();

  JBI->Level_Start("Quality");
    JBI->Add("Fix", gps_result_stored.status);
    JsonBuilderI->Add("satellites", gps_result_stored.satellites); 
    JBI->Add("SatelleteCount", gps_result_stored.satellites);
    JBI->Add("SatelleteThreshold", 0); //minimal fix to be considered enough
    JBI->Add("UptimeSeconds", 0); 
    JBI->Add("DowntimeSeconds", 0); 
  JBI->Level_End();

  JBI->Level_Start("Time");
    JBI->Add("UTC", "0");
    JBI->Add("TimeOfDay", 0);
    JBI->Add("TimeOfDayMillis", gps_latest.milliseconds_of_day);
    JBI->Level_Start("Parts");
      JBI->Add("hours", gps_result_stored.dateTime.hours);
      JBI->Add("minutes", gps_result_stored.dateTime.minutes);
      JBI->Add("seconds", gps_result_stored.dateTime.seconds);
      JBI->Add("day", gps_result_stored.dateTime.day);
      JBI->Add("date", gps_result_stored.dateTime.date);
      JBI->Add("month", gps_result_stored.dateTime.month);
      JBI->Add("year", gps_result_stored.dateTime.year);
      JBI->Add("dateTime_us", gps_result_stored.dateTime_us());
      JBI->Add("dateTime_ms", gps_result_stored.dateTime_ms());
    JBI->Level_End();
  JBI->Level_End();

  JBI->Level_Start("Location");
    JsonBuilderI->Add("latitudeL", gps_result_stored.latitudeL()); 
    JsonBuilderI->Add("latitude", gps_result_stored.latitude());
    JsonBuilderI->Add("longitudeL", gps_result_stored.longitudeL());
    JsonBuilderI->Add("longitude", gps_result_stored.longitude());
  JBI->Level_End();

  JBI->Level_Start("Altitude");
    JBI->Add("hMSL_mm", 0);
    JBI->Add("height_mm", 0);
      JsonBuilderI->Add("altitude_cm", gps_result_stored.altitude_cm()); 
      JsonBuilderI->Add("altitude", gps_result_stored.altitude());
      JsonBuilderI->Add("altitude_ft", gps_result_stored.altitude_ft());
  JBI->Level_End();

  JBI->Level_Start("Velocity");
    JBI->Add("speed3D", 0);
    JBI->Add("speed2D", 0);
      JsonBuilderI->Add("speed_mkn", gps_result_stored.speed_mkn()); 
      JsonBuilderI->Add("speed", gps_result_stored.speed());
      JsonBuilderI->Add("speed_kph", gps_result_stored.speed_kph());
      JsonBuilderI->Add("speed_metersph", gps_result_stored.speed_metersph());
      JsonBuilderI->Add("speed_mph", gps_result_stored.speed_mph());
  JBI->Level_End();

  JBI->Level_Start("Velocity");

      JsonBuilderI->Add("heading_cd", gps_result_stored.heading_cd()); 
      JsonBuilderI->Add("heading", gps_result_stored.heading());

  JBI->Level_End();

    JsonBuilderI->Level_Start("geoidHt");
      JsonBuilderI->Add("geoidHeight_cm", gps_result_stored.geoidHeight_cm()); 
      JsonBuilderI->Add("geoidHeight", gps_result_stored.geoidHeight());
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start("Dilution");
      JsonBuilderI->Add("hdop", gps_result_stored.hdop); 
      JsonBuilderI->Add("vdop", gps_result_stored.vdop);
      JsonBuilderI->Add("pdop", gps_result_stored.pdop);
      JsonBuilderI->Add("lat_err", gps_result_stored.lat_err());
      JsonBuilderI->Add("lon_err", gps_result_stored.lon_err());
      JsonBuilderI->Add("alt_err", gps_result_stored.alt_err());
      JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
      JsonBuilderI->Add("hdg_err", gps_result_stored.hdg_err());
      JsonBuilderI->Add("spd_err", gps_result_stored.spd_err());
      JsonBuilderI->Add("time_err", gps_result_stored.time_err());
    JsonBuilderI->Level_End();


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



uint8_t mGPS::ConstructJSON_GPSPacket_Required(uint8_t json_method){

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


  mqtthandler_ptr = &mqtthandler_gpspacket_debug; //also ifchanged together
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_DEBUG_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_Debug;

  mqtthandler_ptr = &mqtthandler_gpspacket_micro; //also ifchanged together
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MICRO_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_Micro;

  mqtthandler_ptr = &mqtthandler_gpspacket_all; //also ifchanged together
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_All;

  mqtthandler_ptr = &mqtthandler_gpspacket_minimal_teleperiod; //also ifchanged together
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_Minimal;

  // All sensor readings I had on pic32
  mqtthandler_ptr = &mqtthandler_gpspacket_required;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_REQUIRED_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGPS::ConstructJSON_GPSPacket_Required;

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mGPS::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mGPS::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mGPS::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_GPS_ID, handle, id);
  }
}





#endif

#endif