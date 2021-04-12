#ifndef _USE_MODULE_DRIVERS_GPS_H
#define _USE_MODULE_DRIVERS_GPS_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_GPS


#include <string.h>
#include <strings.h>

#define NMEAGPS_DERIVED_TYPES
#define NMEAGPS_PARSE_PROPRIETARY
// #include <NeoGPS_cfg.h>
// #include <ublox/ubxGPS.h>
// #include <ublox/ubxmsg.h>
// #define gpsPort Serial2
#include <NMEAGPS.h>

//=========
#define SERIAL_PRINT_ARRAY(NAME,VALUE,LENGTH) \
          Serial.printf("\n\r%s=",NAME); \
          for(int i=0;i<LENGTH;i++){\
          Serial.print(VALUE[i]);\
          Serial.printf(",");\
          }\
          Serial.println();

#include <GPSport.h>

// //-----------------------------------------------------------------
// //  Derive a class to add the state machine for starting up:
// //    1) The status must change to something other than NONE.
// //    2) The GPS leap seconds must be received
// //    3) The UTC time must be received
// //    4) All configured messages are "requested"
// //         (i.e., "enabled" in the ublox device)
// //  Then, all configured messages are parsed and explicitly merged.

// class MyGPS : public ubloxGPS
// {
// public:

//     enum
//       {
//         GETTING_STATUS, 
//         GETTING_LEAP_SECONDS, 
//         GETTING_UTC, 
//         RUNNING
//       }
//         state NEOGPS_BF(8);

//     MyGPS( Stream *device ) : ubloxGPS( device )
//     {
//       state = GETTING_STATUS;
//     }

//     //--------------------------

//     void get_status()
//     {
//       // static bool acquiring = false;

//       // if (fix().status == gps_fix::STATUS_NONE) {
//       //   static uint32_t dotPrint;
//       //   bool            requestNavStatus = false;

//       //   if (!acquiring) {
//       //     acquiring = true;
//       //     dotPrint = millis();
//       //     DEBUG_PORT.print( F("Acquiring...") );
//       //     requestNavStatus = true;

//       //   } else if (millis() - dotPrint > 1000UL) {
//       //     dotPrint = millis();
//       //     // DEBUG_PORT << '.';

//       //     static uint8_t requestPeriod;
//       //     if ((++requestPeriod & 0x07) == 0)
//       //       requestNavStatus = true;
//       //   }

//       //   if (requestNavStatus)
//       //     // Turn on the UBX status message
//       //     enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_STATUS );

//       // } else {
//       //   if (acquiring)
//       //   //   DEBUG_PORT << '\n';
//       //   // DEBUG_PORT << F("Acquired status: ") << (uint8_t) fix().status << '\n';

//       //   #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
//       //       defined(UBLOX_PARSE_TIMEGPS)

//       //     if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//       //       DEBUG_PORT.println( F("enable TIMEGPS failed!") );

//       //     state = GETTING_LEAP_SECONDS;
//       //   #else
//       //     start_running();
//       //     state = RUNNING;
//       //   #endif
//       // }
//     } // get_status

//     //--------------------------

//     void get_leap_seconds()
//     {
//       // #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
//       //     defined(UBLOX_PARSE_TIMEGPS)

//       //   if (GPSTime::leap_seconds != 0) {
//       //     // DEBUG_PORT << F("Acquired leap seconds: ") << GPSTime::leap_seconds << '\n';

//       //     if (!disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS ))
//       //       DEBUG_PORT.println( F("disable TIMEGPS failed!") );

//       //     #if defined(UBLOX_PARSE_TIMEUTC)
//       //       if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
//       //         DEBUG_PORT.println( F("enable TIMEUTC failed!") );
//       //       state = GETTING_UTC;
//       //     #else
//       //       start_running();
//       //     #endif
//       //   }
//       // #endif

//     } // get_leap_seconds

//     //--------------------------

//     void get_utc()
//     {
//       // #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE) & \
//       //     defined(UBLOX_PARSE_TIMEUTC)

//       //   lock();
//       //     bool            safe = is_safe();
//       //     NeoGPS::clock_t sow  = GPSTime::start_of_week();
//       //     NeoGPS::time_t  utc  = fix().dateTime;
//       //   unlock();

//       //   if (safe && (sow != 0)) {
//       //     // DEBUG_PORT << F("Acquired UTC: ") << utc << '\n';
//       //     // DEBUG_PORT << F("Acquired Start-of-Week: ") << sow << '\n';

//       //     start_running();
//       //   }
//       // #endif

//     } // get_utc

//     //--------------------------

//     void start_running()
//     {
//       // bool enabled_msg_with_time = false;

//       // #if defined(UBLOX_PARSE_POSLLH)
//       //   if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
//       //     DEBUG_PORT.println( F("enable POSLLH failed!") );

//       //   enabled_msg_with_time = true;
//       // #endif

//       // #if defined(UBLOX_PARSE_PVT)
//       //   if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_PVT ))
//       //     DEBUG_PORT.println( F("enable PVT failed!") );

//       //   enabled_msg_with_time = true;
//       // #endif

//       // #if defined(UBLOX_PARSE_VELNED)
//       //   if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED ))
//       //     DEBUG_PORT.println( F("enable VELNED failed!") );

//       //   enabled_msg_with_time = true;
//       // #endif

//       // #if defined(UBLOX_PARSE_DOP)
//       //   if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP ))
//       //     DEBUG_PORT.println( F("enable DOP failed!") );
//       //   else
//       //     DEBUG_PORT.println( F("enabled DOP.") );

//       //   enabled_msg_with_time = true;
//       // #endif

//       // #if defined(UBLOX_PARSE_SVINFO)
//       //   if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_SVINFO ))
//       //     DEBUG_PORT.println( F("enable SVINFO failed!") );
        
//       //   enabled_msg_with_time = true;
//       // #endif

//       // #if defined(UBLOX_PARSE_TIMEUTC)

//       //   #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE)
//       //     if (enabled_msg_with_time &&
//       //         !disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
//       //       DEBUG_PORT.println( F("disable TIMEUTC failed!") );

//       //   #elif defined(GPS_FIX_TIME) | defined(GPS_FIX_DATE)
//       //     // If both aren't defined, we can't convert TOW to UTC,
//       //     // so ask for the separate UTC message.
//       //     if (!enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
//       //       DEBUG_PORT.println( F("enable TIMEUTC failed!") );
//       //   #endif

//       // #endif

//       // state = RUNNING;
//       // trace_header( DEBUG_PORT );

//     } // start_running

//     //--------------------------

//     bool running()
//     {
//       // switch (state) {
//       //   case GETTING_STATUS      : get_status      (); break;
//       //   case GETTING_LEAP_SECONDS: get_leap_seconds(); break;
//       //   case GETTING_UTC         : get_utc         (); break;
//       // }

//       // return (state == RUNNING);

//     } // running

// } NEOGPS_PACKED;

// // Construct the GPS object and hook it to the appropriate serial device
// static MyGPS gps( &gpsPort );

// // #ifdef NMEAGPS_INTERRUPT_PROCESSING
// //   static void GPSisr( uint8_t c )
// //   {
// //     gps.handle( c );
// //   }
// // #endif

// // //--------------------------

// // static void configNMEA( uint8_t rate )
// // {
// //   // for (uint8_t i=NMEAGPS::NMEA_FIRST_MSG; i<=NMEAGPS::NMEA_LAST_MSG; i++) {
// //   //   ublox::configNMEA( gps, (NMEAGPS::nmea_msg_t) i, rate );
// //   // }
// // }

// // //--------------------------

// // static void disableUBX()
// // {
// //   // gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
// //   // gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
// //   // gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
// //   // gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
// //   // gps.disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
// // }

/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       12
 *    D3       13
 *    CMD      15
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      14
 *    VSS      GND
 *    D0       2  (add 1K pull up after flashing)
 *    D1       4
 */

#include "FS.h"
#include "SD_MMC.h"


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR) "gpspacket_minimal";


class mGPS{

  private:
  public:
    mGPS(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    #define WEB_HANDLE_SDCARD_SLIDER "fan_speed"
    
NMEAGPS*  gps = nullptr; // This parses the GPS characters
// gps_fix*  fix_p = nullptr; // This holds on to the latest values
AVERAGING_DATA<float>* averaging;

// int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
// void parse_JSONCommand(JsonObjectConst obj);

uint16_t test_val = 0;
uint8_t dir = 0;

void init();
void pre_init();
int8_t pin = -1;
struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;



// void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
// void createDir(fs::FS &fs, const char * path);
// void removeDir(fs::FS &fs, const char * path);
// void readFile(fs::FS &fs, const char * path);
// void writeFile(fs::FS &fs, const char * path, const char * message);
// void appendFile(fs::FS &fs, const char * path, const char * message);
// void renameFile(fs::FS &fs, const char * path1, const char * path2);
// void deleteFile(fs::FS &fs, const char * path);
// void testFileIO(fs::FS &fs, const char * path);


void CommandSet_CreateFile_WithName(char* value);
void CommandSet_SerialPrint_FileNames(const char* value);
void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
    
void CommandSet_ReadFile(const char* filename);


int8_t CheckAndExecute_JSONCommands();
void parse_JSONCommand(void);

uint8_t ConstructJSON_Scene(uint8_t json_method);

void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_Minimal(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mGPS>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mGPS> mqtthandler_gpspacket_minimal_teleperiod;

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mGPS> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mGPS> mqtthandler_sensor_ifchanged;
    struct handler<mGPS> mqtthandler_sensor_teleperiod;
    
    // const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_GPSPACKET_MINIMAL_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      //later also send byte packet method for testing over mqtt
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };


};

#endif

#endif