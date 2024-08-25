#include "mSolarLunar.h"

#ifdef USE_MODULE_SENSORS_SOLAR_LUNAR

int8_t mSolarLunar::Tasker(uint8_t function, JsonParserObject obj)
{

	// +- 6% of elevation is dust/dawn

  int8_t function_result = 0;
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }
  
  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
	case FUNC_EVERY_50_MSECOND:
	{
		Update_Solar_Tracking_Data();
	}
	break;
    case FUNC_EVERY_SECOND:{

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
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

}


// solar_position_t mSolarLunar::Get_Solar_Tracking_Data(uint32_t utc_time)
// {


// }


void mSolarLunar::Pre_Init()
{

	settings.fEnableSensor = true; 

}


void mSolarLunar::Init()
{


}


void mSolarLunar::Update_Solar_Tracking_Data()
{

	// if(!pCONT_time->RtcTime.isvalid)
	// {
	// 	return; // Cant compute, invalid data
	// }

	float altitude = 0;
	float lat = pCONT_set->Settings.sensors.latitude;
	float lon = pCONT_set->Settings.sensors.longitude;

	double elevation_tmp = 0;

	SolarAzEl(pCONT_time->Rtc.utc_time, lat, lon, 0, &solar_position.azimuth, &elevation_tmp);

	/**
	 * Get direction of change ie sunrise or sunset
	 * */
	if(elevation_tmp > solar_position.elevation)
	{
		solar_position.direction.is_ascending = true;
	}else{
		solar_position.direction.is_ascending = false;
	}
	solar_position.elevation = elevation_tmp+elevation_adjusted_for_debugging;


	/**
	 * @brief Can only be set when the time has been set first
	 **/
	if(pCONT_time->RtcTime.valid)
	{
		solar_position.isvalid = true;
	}else{
		solar_position.isvalid = false;
	}

	/**
	 * @brief Update period of day based on sun elevation
	 **/
	if((solar_position.elevation>5)&&(solar_position.direction.is_ascending == false)) // 
	{
		// time of day is "Daytime-Increasing"
		// Daytime-Decreasing


	}

	if((solar_position.elevation>5)&&(solar_position.direction.is_ascending == false)) // 
	{

	}

	solar_position.tUpdated_millis = millis();

}

/**
 * @brief Calculate sun position (azimuth and elevation)
 * @param utc_time_point utc_time (32 bit) eg time right now (can use days range to for animate days pattern)
 * @param latitude 
 * @param longitude 
 * @param altitude
 * @param azimuth 
 * @param elevation 
 * @note
 * Calculations will be performed with double-percision (64 bit float)
 * Values returned only need float-precision (32 bit)
 * */
void mSolarLunar::SolarAzEl(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El)
{
	
	double jd = julian_day(utc_time_point);

	double d = jd - 2451543.5;
	
	// Keplerian Elements for the Sun(geocentric)
	double w = 282.9404 + 4.70935e-5*d; // (longitude of perihelion degrees)
	// a = 1.000000; % (mean distance, a.u.)
	double e = 0.016709 - 1.151e-9*d; // (eccentricity)
	double M = fmod(356.0470 + 0.9856002585*d, 360.0); // (mean anomaly degrees)
		
	double L = w + M; // (Sun's mean longitude degrees)

	double oblecl = 23.4393 - 3.563e-7*d; // (Sun's obliquity of the ecliptic)

	// auxiliary angle
	double  E = M + (180 / M_PI)*e*sin(M*(M_PI / 180))*(1 + e*cos(M*(M_PI / 180)));

	// rectangular coordinates in the plane of the ecliptic(x axis toward perhilion)
	double x = cos(E*(M_PI / 180)) - e;
	double y = sin(E*(M_PI / 180))*sqrt(1 - pow(e, 2));

	// find the distance and true anomaly
	double r = sqrt(pow(x,2) + pow(y,2));
	double v = atan2(y, x)*(180 / M_PI);

	// find the longitude of the sun
	double lon = v + w;

	// compute the ecliptic rectangular coordinates
	double xeclip = r*cos(lon*(M_PI / 180));
	double yeclip = r*sin(lon*(M_PI / 180));
	double zeclip = 0.0;

	//rotate these coordinates to equitorial rectangular coordinates
	double xequat = xeclip;
	double yequat = yeclip*cos(oblecl*(M_PI / 180)) + zeclip * sin(oblecl*(M_PI / 180));
	double zequat = yeclip*sin(23.4406*(M_PI / 180)) + zeclip * cos(oblecl*(M_PI / 180));

	// convert equatorial rectangular coordinates to RA and Decl:
	r = sqrt(pow(xequat, 2) + pow(yequat, 2) + pow(zequat, 2)) - (Alt / 149598000); //roll up the altitude correction
	double RA = atan2(yequat, xequat)*(180 / M_PI);

	double delta = asin(zequat / r)*(180 / M_PI);
	
	// Get UTC representation of time / C++ Specific
	time_t utc_time = (time_t)pCONT_time->Rtc.utc_time;
	tm *ptm = gmtime(&utc_time); 
        
	double UTH = (double)ptm->tm_hour + (double)ptm->tm_min / 60 + (double)ptm->tm_sec / 3600;

	// Calculate local siderial time
	double GMST0 = fmod(L + 180, 360.0) / 15;

	double SIDTIME = GMST0 + UTH + Lon / 15;
	
	// Replace RA with hour angle HA
	double HA = (SIDTIME*15 - RA);

	// convert to rectangular coordinate system
	x = cos(HA*(M_PI / 180))*cos(delta*(M_PI / 180));

	y = sin(HA*(M_PI / 180))*cos(delta*(M_PI / 180));
	double z = sin(delta*(M_PI / 180));

	// rotate this along an axis going east - west.
	double xhor = x*cos((90 - Lat)*(M_PI / 180)) - z*sin((90 - Lat)*(M_PI / 180));

	double yhor = y;
	double zhor = x*sin((90 - Lat)*(M_PI / 180)) + z*cos((90 - Lat)*(M_PI / 180));

	// float azimuth_f = atan2(yhor, xhor)*(180 / M_PI) + 180;
	// float elevation_f = asin(zhor)*(180 / M_PI);
	
	// // Find the h and AZ
	// *Az = azimuth_f;
	// *El = elevation_f;
	
	*Az = atan2(yhor, xhor)*(180 / M_PI) + 180;
	*El =  asin(zhor)*(180 / M_PI);

}

double mSolarLunar::julian_day(time_t utc_time_point) 
{
	/**
	 * Method A
	 * */
	struct tm* tm = gmtime(&utc_time_point); // Extract UTC Time

	double year = tm->tm_year + 1900;
	double month = tm->tm_mon + 1;
	double day = tm->tm_mday;
	double hour = tm->tm_hour;
	double min = tm->tm_min;
	double sec = tm->tm_sec;

	if (month == 2) {
		year -= 1;
		month += 12;
	}

	double jd = 
		floor(365.25*(year + 4716.0)) + floor(30.6001*(month + 1.0)) + 2.0 -
		floor(year / 100.0) + floor(floor(year / 100.0) / 4.0) + day - 1524.5 +
		(hour + min / 60 + sec / 3600) / 24;

	return jd;


	/**
	 * Method B
	 * */
	//   uint32_t Year = pCONT_time->RtcTime.year;             // Year ex:2020
	//   uint32_t Month = pCONT_time->RtcTime.month;            // 1..12
	//   uint32_t Day = pCONT_time->RtcTime.Wday;     // 1..31
	//   uint32_t Julian;                          // Julian day number

	//   if (Month <= 2) {
	//     Month += 12;
	//     Year -= 1;
	//   }
	//   // Warning, this formula works only for the 20th century, afterwards be are off by 1 day - which does not impact Sunrise much
	//   // Julian = (1461 * Year + 6884472) / 4 + (153 * Month - 457) / 5 + Day -1 -13;
	//   Julian = (1461 * Year + 6884416) / 4 + (153 * Month - 457) / 5 + Day;   // -1 -13 included in 6884472 - 14*4 = 6884416
	//   return Julian;

}





void mSolarLunar::parse_JSONCommand(JsonParserObject obj)
{

	
  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  if(jtok = obj["SolarElevation"]){
    solar_position_testing.elevation = (double)jtok.getFloat();

	Serial.printf("solar_position_testing.elevation=%f\n\r",solar_position_testing.elevation);

	// AddLog(LOG_LEVEL_INFO, PSTR("SolarElevation=%d %d"),(int)solar_position_testing.elevation,jtok.getInt() );
	// delay(3000);
  }

  if(jtok = obj["SolarElevationAdjusted"]){
    elevation_adjusted_for_debugging = (double)jtok.getFloat();

	Serial.printf("elevation_adjusted_for_debugging=%f\n\r",elevation_adjusted_for_debugging);

	// AddLog(LOG_LEVEL_INFO, PSTR("SolarElevation=%d %d"),(int)solar_position_testing.elevation,jtok.getInt() );
	// delay(3000);
  }


	#ifdef ENABLE_DEBUGFEATURE__SENSOR_SOLARLUNAR
  if(jtok = obj["DebugSolar"].getObject()["Enabled"])
  {
    debug.enabled = jtok.getBool();
  }

  if(jtok = obj["DebugSolar"].getObject()["Elevation"])
  {
    debug.elevation = jtok.getFloat();
  }

  if(jtok = obj["DebugSolar"].getObject()["Azimuth"])
  {
    debug.azimuth = jtok.getFloat();
  }
	#endif // ENABLE_DEBUGFEATURE__SENSOR_SOLARLUNAR
  
  

}

uint8_t mSolarLunar::ConstructJSON_Sensor(uint8_t json_method, bool json_appending){

  JBI->Start();

	if(solar_position.isvalid)
	{
		JBI->Add("Az",(float)solar_position.azimuth);
		JBI->Add("El",(float)solar_position.elevation);
		JBI->Add("age",millis()-solar_position.tUpdated_millis);
		JBI->Add("elevation_adjusted_for_debugging", elevation_adjusted_for_debugging);


		// time until/from
		/**
		 * @brief 
		 * 
		 * sunrise
		 * sunset
		 * dust
		 * dawn
		 * culmination
		 * daylight duration
		 * 
		 * 
		 */
	}

  return JBI->End();

}

uint8_t mSolarLunar::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  char buffer[20];

  JBI->Start();
	JBI->Add("mSolarLunar",0);
  return JBI->End();

}



#ifdef USE_MODULE_NETWORK_MQTT

void mSolarLunar::MQTTHandler_Init()
{

  struct handler<mSolarLunar>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSolarLunar::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSolarLunar::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSolarLunar::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSolarLunar::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSolarLunar::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}



#endif // USE_MODULE_NETWORK_MQTT



#endif // USE_MODULE_SENSORS_SOLAR_LUNAR
