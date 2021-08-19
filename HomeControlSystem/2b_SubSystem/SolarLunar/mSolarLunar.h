#ifndef MSOLARLUNAR_H
#define MSOLARLUNAR_H 0.21

#define D_UNIQUE_MODULE_SUBSYSTEM_SOLAR_LUNAR_ID 19

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

#include "3_Network/MQTT/mMQTT.h"

#include <cmath> 
/* fmod example */
#include <stdio.h>      /* printf */
#include <math.h>       /* fmod */
  
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif /* M_PI */

#include "1_TaskerManager/mTaskerInterface.h"

class mSolarLunar :
  public mTaskerInterface
{
  
  private:
  /* data */
public:
  mSolarLunar(/* args */){};

  
    void Init(void);
    void Pre_Init();
    
    int8_t Tasker(uint8_t function, JsonParserObject obj);
    
    static const char* PM_MODULE_SUBSYSTEM_SOLAR_LUNAR_CTR;
    static const char* PM_MODULE_SUBSYSTEM_SOLAR_LUNAR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SUBSYSTEM_SOLAR_LUNAR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SUBSYSTEM_SOLAR_LUNAR_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SUBSYSTEM_SOLAR_LUNAR_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSolarLunar);
    };
    #endif

    
    struct SETTINGS{
      uint8_t  fEnableSensor = false;
      // uint8_t  nSensorsFound = 0; // count of sensors found    n:number found, c:case number for switches
      // uint16_t rate_measure_ms = 1000;
      // uint8_t  group_count = 0;
    }settings;

    void parse_JSONCommand(JsonParserObject obj);

    void Update_Solar_Tracking_Data();

    struct solar_position_t
    {
      double azimuth = 0;
      double elevation = 0;
      uint32_t tUpdated_millis = 0;
      bool isvalid = false;
    }solar_position;

    solar_position_t solar_position_testing;


  // ~mSolarLunar();

    
  // double lat = 52.975;
  // double lon = -6.0494;
  // double altitude = 0;
  
  // double Az = 0.0;
  // double El = 0.0;


  void SolarAzEl(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El);

  double julian_day(time_t utc_time_point);

  
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSolarLunar> mqtthandler_settings_teleperiod;
    struct handler<mSolarLunar> mqtthandler_sensor_ifchanged;
    struct handler<mSolarLunar> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mSolarLunar>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT 


};

// mSolarLunar::mSolarLunar(/* args */)
// {
// }

// mSolarLunar::~mSolarLunar()
// {
// }

#endif // USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

#endif // HEADER GUARD

// Programed by Darin C.Koblick 2 / 17 / 2009
//
//              Darin C.Koblick 4 / 16 / 2013 Vectorized for Speed
//                                         Allow for MATLAB Datevec input in
//                                         addition to a UTC string.
//                                         Cleaned up comments and code to
//                                         avoid warnings in MATLAB editor.
//
//				Kevin Godden 9/1/2020      Ported from Matlab to C++, tried to change as little as possible.
//                                         this is a non-vectorised port.
//
//--------------------------------------------------------------------------
//
// External Function Call Sequence :
//
// double lat = 52.975;
// double lon = -6.0494;
// double altitude = 0;
//
// double Az = 0.0;
// double El = 0.0;
// SolarAzEl(time(NULL), lat, lon, 0, &amp;Az, &amp;El);
// 
// printf("Azimuth: %f\n", Az);
// printf("Elevation: %f\n", El);
//
// Or to calculate Az &amp; El for an arbitary UTC time:
//
//
// tm utc;
// tm_year is time since 1900
// utc.tm_year = y - 1900;
// Month is zero based, i.e. Jan is month 0
// utc.tm_mon = m - 1;
// utc.tm_mday = d;
// utc.tm_hour = 10;
// utc.tm_min = 16;
// utc.tm_sec = 00;
// utc.tm_isdst = 0;
// 
// Get UTC time_t val
// tim = timegm(&amp;utc);	// or _mkgmtime() on windows
// 
// double altitude = 0;
// double Az = 0.0;
// double El = 0.0;
// 
// double lat = 52.975;
// double lon = -6.0494;
// 
// SolarAzEl(tim, lat, lon, 0, &amp;Az, &amp;El);
// 
// printf("Az: %f\n", Az);
// printf("El: %f\n", El);
// 
//
// Function Description :
//
// SolarAzEl will ingest a Universal Time, and specific site location on earth
// it will then output the solar Azimuth and Elevation angles relative to that
// site.
//
// Input Description :
//
// utc_time_point : time_t containing target time for sun position calculations.
//
// Lat : Site Latitude in degrees -90:90-&gt;S(-) N(+)
//
// Lon : Site Longitude in degrees -180:180 W(-) E(+)
//
// Alt : Altitude of the site above sea level(Km)
//
// Output Description :
//  Az    Azimuth location of the sun(deg)
//  El    Elevation location of the sun(deg)
//
//
// Source References :
// Solar Position obtained from :
// http ://stjarnhimlen.se/comp/tutorial.html#5
//

































//   //
// // sun.h
// // Version timestamp: 9-26-2018, 10:36 PM
// //
// // Attribution : Copyright (c) 2018 Northern_Loki (sha256::6F290BF833967127BE26C92C8F6B1C1A3949C55A7EABCEF3ECC785CD2D38D30D)
// // License is granted under the Creative Commons Attribution-ShareAlike 4.0 International.  https://creativecommons.org/licenses/by-sa/4.0/
// //
// # pragma once
// # include <cstdlib>
// # include <math.h>
// # include <iostream>
// # include <chrono>
// # include <ctime>
// # include <boost/date_time.hpp>
// # include "Constants.h"
// # include "tz.h"

// typedef std::chrono::duration<long int, std::ratio<1l, 1000000000l>> time_duration;
// typedef std::chrono::duration<long int, std::ratio<1l, 1l>> chrono_duration;
// typedef date::zoned_time<time_duration> zoned_sun_time;
// typedef std::chrono::time_point<date::local_t, chrono_duration> chrono_sun_time;

// typedef struct
// {
//     zoned_sun_time julian_nw_equinox;
//     zoned_sun_time julian_sw_equinox;
//     zoned_sun_time julian_n_solstice;
//     zoned_sun_time julian_s_solstice;
//     time_duration LengthOfSpring;
//     time_duration LengthOfSummer;
//     time_duration LengthOfAutumn;
//     time_duration LengthOfWinter;
// } equinox_soltices_t, *equinox_soltices_p;


// class sun_time
// {
// public:
//     sun_time() {}
//     ~sun_time() {}	
    
//     bool *require_update;
//     zoned_sun_time solar_time;
//     zoned_sun_time operator()() {return solar_time;}
//     zoned_sun_time operator=(zoned_sun_time rhs) {solar_time = rhs;}
//     friend std::ostream& operator<<(std::ostream& lhs, const sun_time& rhs)
//     {
//         return (lhs << date::format("%b/%d/%Y %H:%M %Z", rhs.solar_time));
//     }
// };

// class sun_duration
// {
// public:
//     sun_duration() {}
//     ~sun_duration() {}	

//     time_duration solar_time;
//     time_duration operator()() {return solar_time;}
//     time_duration operator=(time_duration rhs) {solar_time = rhs;}
//     friend std::ostream& operator<<(std::ostream& lhs, const sun_duration& rhs)
//     {
//         return (lhs << date::format("%H:%M", rhs.solar_time));
//     }
// };

// class solar_seasons
// {
// public:
//     solar_seasons() {}
//     ~solar_seasons() {}
    
//     sun_time julian_nw_equinox;
//     sun_time julian_sw_equinox;
//     sun_time julian_n_solstice;
//     sun_time julian_s_solstice;
//     sun_duration LengthOfSpring;
//     sun_duration LengthOfSummer;
//     sun_duration LengthOfAutumn;
//     sun_duration LengthOfWinter;
// };

// class float_time
// {
// public:
//     float_time() {}
//     float_time(double value)
//         : hour(value * 24.0)
//         , minutes(((value * 24.0) - hour) * 60.0)
//         , seconds(((value * 1440.0) - (hour * 24.0 + minutes)) * 60.0)
//         , time(value) {}
//     ~float_time() {}
//     double operator()() {return time;}
//     friend std::ostream& operator<<(std::ostream& lhs, const float_time& rhs)
//     {
//         return (lhs << rhs.hour << ":" << rhs.minutes << ":" << rhs.seconds << ":");
//     }
    
//     int hour;
//     int minutes;
//     int seconds;
//     double time;
// };

// class length_of_day
// {
// public:
//     length_of_day() {}
//     length_of_day(double daylength)
//         : hours(daylength * 60.0)
//         , minutes(((daylength * 60.0) - hours) * 60.0)
//         , seconds(((daylength * 3600.0) - (hours * 60.0 + minutes)) * 60.0)
//         , length(daylength) {}
//     ~length_of_day() {}
//     double operator()() {return length;}
//     friend std::ostream& operator<<(std::ostream& lhs, const length_of_day& rhs)
//     {
//         return (lhs << rhs.hours << " hours " << rhs.minutes << " minutes " << rhs.seconds << " seconds");
//     }
    
//     int hours;
//     int minutes;
//     int seconds;
//     double length;
// };

// class sun
// {
// public:
//     sun();
//     sun(const double &latitude, const double &longitude);
//     sun(std::string name, const double &latitude, const double &longitude);
//     sun(const double &latitude, const double &longitude, std::tm &timeinfo);
//     sun(std::string name, const double &latitude, const double &longitude, std::tm &timeinfo);
//     sun(std::string name, const double &latitude, const double &longitude, zoned_sun_time &timeinfo);
//     ~sun();
//     void set(const double &latitude, const double &longitude, std::tm &timeinfo);
//     void set(std::string name, const double &latitude, const double &longitude, std::tm &timeinfo);
//     void set(std::string name, const double &latitude, const double &longitude, zoned_sun_time &timeinfo);
// 	void set(std::string name, const double &latitude, const double &longitude, zoned_sun_time &timeinfo, bool irrad);
// 	void operator()(std::string name, const double &latitude, const double &longitude, zoned_sun_time &timeinfo);
// 	void operator()(std::string name, const double &latitude, const double &longitude, zoned_sun_time &timeinfo, bool irrad);
//     void perform_solar_calculations();
// 	void perform_solar_calculations_irrad();
//     void set_julian(unsigned long epoch);
//     void set_julian(zoned_sun_time &timeinfo);
//     double calculate_julian_time(double time, bool unix_time);
//     double calculate_julian_time(zoned_sun_time &timeinfo, bool unix_time);
//     double calculate_julian_century(double julian);
//     double calculate_mean_geom_longitude(double julian_century);
//     double calculate_mean_geom_anomoly(double julian_century);
//     double calculate_eccentric_earth_orbit(double julian_century);
//     double calculate_center_of_sun(double julian_century, double geom_anomoly);
//     double calculate_true_longitude(double geom_mean, double geom_anomoly);
//     double calculate_true_anomoly(double sun_center, double geom_anomoly);
//     double calculate_rad_vector(double true_anomoly, double eccentric_earth);
//     double calculate_apparent_longitude(double julian_century, double true_longitude);
//     double calculate_mean_obliquity_ecliptic(double julian_century);
//     double calculate_obliquity_correction(double julian_century, double mean_obliquity_ecliptic);
//     double calculate_right_ascension(double apparent_longitude, double obliquity_correction);
//     double calculate_declination(double apparent_longitude, double obliquity_correction);
//     double calculate_solar_noon(double observer_longitude, double EoT);
//     double calculate_hour_angle(double solar_time);
// 	double calculate_solar_time(double local_longitude, double time, double EoT);
//     double calculate_EoT(double obliquity_correction, double geom_mean_longitude, double geom_mean_anomoly, double eccentric_earth_orbit);
//     double calculate_sunset_time(double solar_noon, double hour_angle_sunrise);
//     double calculate_sunrise_time(double solar_noon, double hour_angle_sunrise);
//     double calculate_sunrise_hour_angle(float latitude, double declination);
// 	double calculate_sunset_hour_angle(double sunrise_hour_angle);
//     double calculate_daylength(double sunrise_hour_angle);
//     double calculate_zenith_angle(double sec_z_inverse);
// 	double calculate_inverse_secant_z(double observer_latitude, double declincation, double hour_angle);
//     double calculate_solar_elevation(double zenith_angle);
//     double calculate_solar_azimuth_angle(double observer_latitude, double zenith, double hour_angle, double declination);
//     double calculate_atmospheric_refraction(double elevation);
//     double calculate_earth_nutation(double julian_century);
//     double tune_soltice_equinox(double julian_estimate, double degrees);
//     void calculate_soltice_equinox(zoned_sun_time *timeinfo, equinox_soltices_p solar_seasons);
//     void set_length_of_seasons(zoned_sun_time *timeinfo);

// 	double time_since_midnight(zoned_sun_time *timeinfo);
//     double get_offset_from_UTC(bool adjust_for_dst);
//     boost::posix_time::ptime float_to_ptime(double value);
//     zoned_sun_time from_julian_date(double julian);
//     date::local_seconds to_local_time(std::tm const& t);
//     date::sys_seconds to_sys_time(std::tm const& t);
// 	chrono_duration fp_days_to_chrono(double fp_days);
    
//     std::string name;
//     zoned_sun_time *zone_time;
//     float observer_longitude;
//     float observer_latitude;
//     double julian;
//     double julian_century;
//     double geom_mean_longitude;
//     double geom_mean_anomoly;
//     double eccentric_earth_orbit;
//     double sun_center;
//     double true_longitude;
//     double true_anomoly;
//     double rad_vector;
//     double apparent_longitude;
//     double mean_obliquity_ecliptic;
//     double mean_obliquity_correction;
//     double right_ascension;
//     double sunrise_hour_angle;
// 	double sunset_hour_angle;
//     double hour_angle;
// 	double sec_z_inverse;
//     double zenith_angle;
//     double solar_elevation;
//     double declincation;
//     double solar_azimuth_angle;
//     double atmospheric_refraction;
//     double solar_elevation_corrected;
//     double EoT;
//     double solar_time;
//     double solar_noon;
//     double sunset;
// 	double time_midnight;
//     boost::posix_time::ptime sunset_time;
//     double sunrise;
//     boost::posix_time::ptime sunrise_time;
//     length_of_day daylength;
//     solar_seasons seasons;
    
// private:
//     boost::posix_time::ptime local_time;
//     boost::posix_time::ptime utc_time;
//     std::time_t time_epoch;
//     char timezone[8];
//     boost::posix_time::time_duration timezone_offset; 
// };



// // #include <Arduino.h>

// // #define D_UNIQUE_MODULE_CORE_TIME_ID 5


// // typedef struct datetime{
// //   uint8_t Wday; // week day [0-7]
// //   uint8_t Mday; // month day [0-30]
// //   uint16_t Yday; // year day [0-365]
// //   uint16_t year;  // year 20XX
// //   uint8_t month;  // [1-12]
// //   uint8_t week;   // [1-52]
// //   uint8_t hour;   // [0-23]
// //   uint8_t minute; // [0-59]
// //   uint8_t second; // [0-59]

// //   //generate these with a function START
// //   uint32_t Dseconds; //seconds into day from midnight [0-86400]
// //   uint32_t Wseconds; // seconds into week [0-604800]
// //   uint32_t Yseconds; // seconds into year [0-31536000]
// //   //generate these with a function END


// //   uint32_t seconds_nonreset; // seconds counting infinitely [0-1e+32]

// //   //PHASE OUT
// //   char hhmmss_ctr[10]; //phase out into function with formatting

// //   //from tas
  
// //   unsigned long days;
// //   unsigned long valid;


// //   uint8_t isvalid = false;
// // }datetime_t;

// // // used for basic time info only for week period - RENAME "time_t"
// // typedef struct time_short{
// //   uint8_t Wday; // week day [0-7]
// //   uint8_t hour;   // [0-23]
// //   uint8_t minute; // [0-59]
// //   uint8_t second; // [0-59]
// // }time_short_t;

// // // used for basic time info only for week period - RENAME "time_t"
// // typedef struct weektime{
// //   uint8_t  Wday; // week day [0-7]
// //   uint8_t  hour;   // [0-23]
// //   uint8_t  minute; // [0-59]
// //   uint8_t  second; // [0-59]
// //   uint16_t millis; // millis into 1 second [0-1000]
// // }weektime_t;




// // // 7 bytes
// // typedef struct TIMEREACHED_HANDLER{
// //   uint32_t millis = 0;
// //   uint8_t run = false; // run immediately
// //   // uint8_t interval_is_millis = true; //else seconds
// //   // maximum of 65 seconds (65535 millis)
// //   // uint16_t interval = 0;  // if not 0, then use this instead of what is passed in
// // }timereached_t;

// // #define SEC2CENT 3153600000
// // #define SEC2DEC 315360000
// // #define SEC2YR 31536000
// // #define SEC2MONTH 2592000
// // #define SEC2WEEK 604800
// // #define SEC2DAY 86400
// // #define SEC2HOUR 3600
// // #define SEC2MIN 60

// // #define SEC_IN_CENT 3153600000
// // #define SEC_IN_DEC 315360000
// // #define SEC_IN_YR 31536000
// // #define SEC_IN_MONTH 2592000
// // #define SEC_IN_WEEK 604800
// // #define SEC_IN_DAY 86400
// // #define SEC_IN_HOUR 3600
// // #define SEC_IN_MIN 60

// // enum TIME_UNITS_IDS{
// //   TIME_UNIT_SECONDS_ID = 0,
// //   TIME_UNIT_MILLISECONDS_ID
// // };


// // template <typename TIME, typename UNIT>
// // uint32_t ConvertTimeToMilliSecondsWithUnit(TIME time_secs, UNIT unit){
// //   if(unit == TIME_UNIT_SECONDS_ID){
// //     time_secs *= 1000;
// //   }
// //   return time_secs;
// // }


// // #include <EEPROM.h>
// // #define EEPROM_ADDR_REBOOT_COUNTER 0

// // #include <WiFiUdp.h>

// // #include "1_TaskerManager/mTaskerManager.h"

// // #include "2_CoreSystem/Settings/mSettings.h"

// // #ifdef ESP8266
// //   extern "C" {
// //     #include "sntp.h"
// //   }
// // #endif
// // #include <Ticker.h>

// // #include "1_TaskerManager/mTaskerInterface.h"

// // class mTime :
// //   public mTaskerInterface
// // {

// //   private:
// //   public:
// //     mTime(){};
// //     void init(void);
// //     void UpdateStoredRTCVariables();
// //     void initUpTime();
// //     void UpdateUpTime();
// //     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

// //     static const char* PM_MODULE_CORE_TIME_CTR;
// //     static const char* PM_MODULE_CORE_TIME_FRIENDLY_CTR;
// //     PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME_CTR; }
// //     PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_TIME_FRIENDLY_CTR; }
// //     uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TIME_ID; }
    
// //     #ifdef USE_DEBUG_CLASS_SIZE
// //     uint16_t GetClassSize(){
// //       return sizeof(mTime);
// //     };
// //     #endif
// //     void parse_JSONCommand(JsonParserObject obj);


// //     void  SetUTCTime(
// //           uint16_t year,
// //           uint8_t month,
// //           uint8_t day,
// //           uint8_t hour,
// //           uint8_t minute,
// //           uint8_t second
// //         );

// //     uint32_t uptime_seconds_nonreset = 0; //test new uptime with comparison of breaktime
// //     uint32_t boottime_epoch;

    
// //     uint32_t GetTimeOfDay_Seconds(void);
// //     static const char* ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds, char* buffer, uint8_t buflen);
// //     // static const char* ConvertShortTime_HHMMSS(time_short_t* time, char* buffer, uint8_t buflen);
    
// //     static const char* ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen);

// //     static const char* ConvertU32TimetoCtr(uint32_t* _time, char* buffer, uint8_t buflen);

// //     // static const char* ConvertShortTime_HHMMSS(uint32_t* _time, char* buffer, uint8_t buflen);
// //     static time_short_t Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr);
    
// //     // Used to keep the last midnight time, this stops multiple midnight runs at the same midnight
// //     uint16_t lastday_run = 0;
    
// //     char* GetDateAndTimeCtr(uint8_t time_type, char* buffer, uint8_t buflen);

// //     time_short_t GetTimeShortNow();
// //     uint32_t GetTimeShortNowU32();

// //     #ifdef USE_SUNRISE
// //       uint32_t JulianDate(const datetime_t &now);      
// //       // Force value in the 0..pi2 range
// //       float InPi(float x);
// //       float TimeFormula(float *DK, uint32_t Tdays) ;
// //       void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down);
// //       // void ApplyTimerOffsets(Timer *duskdawn);
// //       char* GetSunTimeAtHorizon(uint32_t dawn, char* buffer, uint8_t buflen);
// //       uint16_t SunMinutes(uint32_t dawn);
// //     #endif //  USE_SUNRISE


// //     uint32_t tSavedUptime;

// //     uint8_t fTimeSet = false;

// // //     const char* GetUTCTimeCtr(char* buffer = nullptr);
// // //     const char* GetLocalTimeCtr(char* buffer = nullptr);
    
// // //     const char* GetTimeZoneCtr(char* buffer = nullptr);

    
// // // const char* GetStartDSTTimeCtr(char* buffer, uint8_t buflen);
// // // const char* GetEndDSTTimeCtr(char* buffer, uint8_t buflen);


// // //     const char* GetSunriseCtr(char* buffer, uint8_t buflen);
// // //     const char* GetSunsetCtr(char* buffer, uint8_t buflen);


// //     uint32_t ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss);
// //     uint8_t CheckBetweenSOD(uint32_t start, uint32_t end);

// //     // #define DAYLIGHT_SAVINGS_ON
// //     // #ifdef DAYLIGHT_SAVINGS_ON
// //     //   #define NTP_OFFSET_WITH_DLS 1 * 60 * 60 // In seconds
// //     // #else
// //     //   #define NTP_OFFSET_WITHOUT_DLS 0 * 60 * 60 // In seconds
// //     #define NTP_OFFSET 60 * 60
// //     // #endif?
// //     bool fEnabled_DayLightSavings = false;
// //     #define NTP_INTERVAL 60 * 1000 // In miliseconds
// //     #define NTP_ADDRESS "0.pool.ntp.org" //US address
// //     // #define NTP_ADDRESS "europe.pool.ntp.org"
// //     //#define NTP_ADDRESS "0.europe.pool.ntp.org" //US address

// //     struct SETTINGS{
// //       uint8_t timeclient_is_started = false;
// //     }settings;

// //     bool CheckOrStartNTPService();
// //     void TickRTCVariablesWithUptime();

    
// // void RtcSync(void);

// //     // WiFiUDP ntpUDP;
// //     // NTPClient* timeClient;//(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// //     void ResetRebootCounter();

// //     uint32_t UpTime(void);
// //     int8_t Tasker_InternetTime(void);

// //     enum MONTH_NUM{MONTH_JANUARY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};
// //     //enum WDAY_NUM{WDAY_MONDAY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};

// //     uint16_t GetDayOfYear(uint8_t day, uint8_t month);
// //     uint16_t GetDaysInMonth(uint8_t month);
// //     datetime_t GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2);

// //     void PrintDateTime(datetime_t dt);
// //     void PrintDateTime(time_short_t dt);
// //     void DateTimeWeek2HHMMSS(datetime_t* dt, uint8_t* hour, uint8_t* minute, uint8_t* second);

// //     uint8_t CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end);
// //     uint8_t CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end);
// //     int8_t  CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end);

// //     const char* getFormattedUptime(char* buffer, uint8_t buflen);

// //     // datetime_t RtcTime;
// //     datetime_t RtcTime;


// //     static bool TimeReached(uint32_t* tSaved, uint32_t ElapsedTime);
// //     static bool TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime);
// //     static bool TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
// //     static bool TimeReached(TIMEREACHED_HANDLER* tSaved);
// //     static bool MillisReached(uint32_t* tTarget);
// //     static bool TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
// //     static uint32_t MillisElapsed(uint32_t* tSaved);
// //     static uint32_t MillisElapsed(uint32_t tSaved);

// //     timereached_t testtime;

// //     // /*********************************************************************************************\
// //     //  * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
// //     //  *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
// //     // \*********************************************************************************************/

// //     const uint32_t SECS_PER_MIN = 60UL;
// //     const uint32_t SECS_PER_HOUR = 3600UL;
// //     const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
// //     const uint32_t MINS_PER_HOUR = 60UL;

// //     void RtcSecond();
    
// // uint32_t RuleToTime(TimeRule r, int yr);

// // // void BreakTime(uint32_t time_input, struct TIME_T &tm);
// // // uint32_t MakeTime(struct TIME_T &tm);
// // uint32_t MakeTime(datetime_t &tm);

// // uint32_t RtcMillis(void) ;

// // void RtcSetTime(uint32_t epoch);

// // uint32_t MinutesUptime(void);
// // String GetUptime(void);
// // uint32_t MinutesPastMidnight(void);

// // String GetDuration(uint32_t time);

// // void WifiPollNtp() ;
// // uint32_t WifiGetNtp(void);

// //     #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))
    
// //     #ifdef ESP8266
// //     Ticker* TickerRtc = nullptr;
// //     #endif //esp8266
// //     #ifdef ESP32
// //     uint32_t tSavedTicker_ESP32 = millis();
// //     #endif // ESP32

// //     uint8_t kDaysInMonth[12];// = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
// //     // char kMonthNamesEnglish[37];// = "JanFebMarAprMayJunJulAugSepOctNovDec";

// // void BreakTime(uint32_t time_input, datetime_t &tm);

// // uint8_t hour(uint32_t time);
// // uint8_t minute(uint32_t time);
// // uint8_t second(uint32_t time);
// // int hourFormat12(time_t t);

// // struct RTC {
// //   uint32_t utc_time = 0;
// //   uint32_t local_time = 0;
// //   uint32_t daylight_saving_time = 0;
// //   uint32_t standard_time = 0;
// //   uint32_t ntp_time = 0;  //previous local time?
// //   uint32_t midnight = 0;
// //   uint32_t restart_time = 0;
// //   uint32_t millis = 0;
// //   uint32_t last_sync = 0;
// //   int32_t time_timezone = 0;
// //   uint8_t ntp_sync_minute = 0;
// //   bool midnight_now = false;
// //   bool user_time_entry = false;               // Override NTP by user setting

// //   uint32_t ntp_last_active = 0;
  
// // } Rtc;


// //     // uint32_t utc_time = 0;
// //     // uint32_t local_time = 0;
// //     // uint32_t daylight_saving_time = 0;
// //     // uint32_t standard_time = 0;
// //     // uint32_t ntp_time = 0;
// //     // uint32_t midnight = 0;
// //     // uint32_t restart_time = 0;
// //     // int32_t  time_timezone = 0;
// //     // uint8_t  midnight_now = 0;
// //     // uint8_t  ntp_sync_minute = 0;
    
// //     uint32_t LocalTime(void);
// //     uint32_t Midnight(void);
    
// //     bool  IsDst(void);


// //     void RtcInit(void);


// //     datetime_t GetTimefromCtr(const char* c);

// //     uint16_t DaysInThisMonth();

// //     unsigned char ntp_attempt_count;

// //     // struct UPTIME{
// //     //   uint8_t seconds;
// //     //   uint8_t minute;
// //     //   uint16_t hours;
// //     //   uint16_t days;
// //     //   uint8_t rebootcount;
// //     //   uint32_t nonresetseconds=0;
// //     //   char ddhhmmss_ctr[13];
// //     // }uptime;

// //     struct REBOOTINFO{
// //       uint8_t count=0;
// //     }rebootinfo;


// //     datetime_t uptime;

// //     uint8_t CheckBetweenDateTimes(datetime_t* start, datetime_t* end);

// //     uint8_t AddSecondsToDateTime(datetime_t* start, uint32_t seconds);

// //     uint8_t CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window = 0);

// //     uint32_t GetSecondsOfDayFromDateTime(datetime_t* dt_t);

// //     const char* getFormattedTime(datetime_t* dt_t, char* buffer, uint8_t buflen);
// //     const char* getFormattedDateTime(datetime_t* dt_t, char* buffer, uint8_t buflen);
    
// //     const char* GetBuildDateAndTime(char* buffer, uint8_t buflen);
// //     const char* GetDT(uint32_t time, char* buffer, uint8_t buflen);

// //     const char* GetDOWShortctr(uint8_t dow, char* buffer);
// //     const char* GetDOWLongctr(uint8_t dow, char* buffer);

// //     unsigned char tSavedMinute;
// //     unsigned char tSavedMinuteHeatingTimers;
// //     int cSavedMinutes;//counts minutes
// //     int tSavedMinuteTemps;
// //     unsigned long tSavedStoreRTCUpdate;

// // };
// #endif
