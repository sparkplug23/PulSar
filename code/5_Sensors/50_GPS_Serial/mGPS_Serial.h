#ifndef _USE_MODULE_SENSORS_GPS_SERIAL_H
#define _USE_MODULE_SENSORS_GPS_SERIAL_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID ((5*1000)+50)

#ifdef USE_MODULE_SENSORS_GPS_SERIAL

#include <string.h>
#include <strings.h>

#define UBLOX_PARSE_STATUS
#define UBLOX_PARSE_TIMEGPS
#define UBLOX_PARSE_TIMEUTC
#define UBLOX_PARSE_POSLLH
#define UBLOX_PARSE_DOP
#define UBLOX_PARSE_PVT
#define UBLOX_PARSE_VELNED
#define UBLOX_PARSE_SVINFO
#define UBLOX_PARSE_TIMEUTC
#define GPS_FIX_TIME

#define NMEAGPS_PARSE_SATELLITES

#include <NeoGPS_cfg.h>
#include <ublox/ubxGPS.h>
#include <GPSport.h>
#include <Streamers.h>


// const unsigned char ubxRate10Hz[] PROGMEM =
//   { 0x06,0x08,0x06,0x00,100,0x00,0x01,0x00,0x01,0x00 };


//-------------------------------------------
// U-blox NMEA text commands

const char disableRMC[] PROGMEM = "PUBX,40,RMC,0,0,0,0,0,0";
const char disableGLL[] PROGMEM = "PUBX,40,GLL,0,0,0,0,0,0";
const char disableGSV[] PROGMEM = "PUBX,40,GSV,0,0,0,0,0,0";
const char disableGSA[] PROGMEM = "PUBX,40,GSA,0,0,0,0,0,0";
const char disableGGA[] PROGMEM = "PUBX,40,GGA,0,0,0,0,0,0";
const char disableVTG[] PROGMEM = "PUBX,40,VTG,0,0,0,0,0,0";
const char disableZDA[] PROGMEM = "PUBX,40,ZDA,0,0,0,0,0,0";

const char enableRMC[] PROGMEM = "PUBX,40,RMC,0,1,0,0,0,0";
const char enableGLL[] PROGMEM = "PUBX,40,GLL,0,1,0,0,0,0";
const char enableGSV[] PROGMEM = "PUBX,40,GSV,0,1,0,0,0,0";
const char enableGSA[] PROGMEM = "PUBX,40,GSA,0,1,0,0,0,0";
const char enableGGA[] PROGMEM = "PUBX,40,GGA,0,1,0,0,0,0";
const char enableVTG[] PROGMEM = "PUBX,40,VTG,0,1,0,0,0,0";
const char enableZDA[] PROGMEM = "PUBX,40,ZDA,0,1,0,0,0,0";


// $PUBX,41,portId,inProto,outProto,baudrate,autobauding*cs<CR><LF>
// $PUBX,41,1,0007,0003,19200,0*25


const char baud9600  [] PROGMEM = "PUBX,41,1,3,3,9600,0";
const char baud38400 [] PROGMEM = "PUBX,41,1,3,3,38400,0";
const char baud57600 [] PROGMEM = "PUBX,41,1,3,3,57600,0";
const char baud115200[] PROGMEM = "PUBX,41,1,3,3,115200,0";
const char baud230400[] PROGMEM = "PUBX,41,1,3,3,230400,0";
const char baud921600[] PROGMEM = "PUBX,41,1,3,3,921600,0";


//-------------------------------------------
// U-blox UBX binary commands

const unsigned char ubxRate1Hz[] PROGMEM = 
  { 0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00 };
const unsigned char ubxRate5Hz[] PROGMEM =
  { 0x06,0x08,0x06,0x00,200,0x00,0x01,0x00,0x01,0x00 };
const unsigned char ubxRate10Hz[] PROGMEM =
  { 0x06,0x08,0x06,0x00,100,0x00,0x01,0x00,0x01,0x00 };
const unsigned char ubxRate16Hz[] PROGMEM =
  { 0x06,0x08,0x06,0x00,50,0x00,0x01,0x00,0x01,0x00 };

// // Disable specific NMEA sentences
// const unsigned char ubxDisableGGA[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableGLL[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableGSA[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableGSV[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableRMC[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableVTG[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01 };
// const unsigned char ubxDisableZDA[] PROGMEM =
//   { 0x06,0x01,0x08,0x00,0xF0,0x08,0x00,0x00,0x00,0x00,0x00,0x01 };

// Disable specific NMEA sentences
const unsigned char ubxDisableGGA[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableGLL[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableGSA[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableGSV[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableRMC[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableVTG[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01 };
const unsigned char ubxDisableZDA[] PROGMEM =
  { 0x06,0x01,0x08,0x00,0xF0,0x08,0x00,0x00,0x00,0x00,0x00,0x01 };


// static const uint8_t ubxReset[] __PROGMEM =
//   { ublox::UBX_CFG, ublox::UBX_CFG_RST,
//     UBX_MSG_LEN(ublox::cfg_reset_t), 0,               // word length MSB is 0
//     0,0,                                              // clear bbr section
//     ublox::cfg_reset_t::CONTROLLED_SW_RESET_GPS_ONLY, // reset mode
//     0x00                                              // reserved
//   };


// #include "5_Sensors/50_GPS_Serial/internal/ublox/ubx_cfg.h"

// #include "5_Sensors/50_GPS_Serial/internal/configs/NeoGPS_cfg.h"
// #include "5_Sensors/50_GPS_Serial/internal/ublox/ubxGPS.h"

//======================================================================
//  Program: ublox.ino
//
//  Prerequisites:
//     1) You have a ublox GPS device
//     2) PUBX.ino works with your device
//     3) You have installed the ubxGPS.* and ubxmsg.* files.
//     4) At least one UBX message has been enabled in ubxGPS.h.
//     5) Implicit Merging is disabled in NMEAGPS_cfg.h.
//
//  Description:  This program parses UBX binary protocal messages from
//     ublox devices.  It shows how to acquire the information necessary
//     to use the GPS Time-Of-Week in many UBX messages.  As an offset
//     from midnight Sunday morning (GPS time), you also need the current 
//     UTC time (this is *not* GPS time) and the current number of GPS 
//     leap seconds.
//
//  Serial is for debug output to the Serial Monitor window.


// #define SERIAL_PRINT_ARRAY(NAME,VALUE,LENGTH) \
//           Serial.printf("\n\r%s=",NAME); \
//           for(int i=0;i<LENGTH;i++){\
//           Serial.print(VALUE[i]);\
//           Serial.printf(",");\
//           }\
//           Serial.println();

#define MILLISECONDS_IN_DAY 86400000
#define DEBUG_PORT Serial 

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR) "gpspacket_minimal";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_REQUIRED_CTR) "gpspacket_required";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_DEBUG_CTR)    "debug/parsing";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL_CTR)    "debug/all";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MICRO_CTR)    "micro";



// #ifndef NMEAGPS_DERIVED_TYPES
//   #error You must "#define NMEAGPS_DERIVED_TYPES" in NMEAGPS_cfg.h!
// #endif

// #if !defined(UBLOX_PARSE_STATUS)  & !defined(UBLOX_PARSE_TIMEGPS) & \
//     !defined(UBLOX_PARSE_TIMEUTC) & !defined(UBLOX_PARSE_POSLLH)  & \
//     !defined(UBLOX_PARSE_DOP)     & !defined(UBLOX_PARSE_PVT)     & \
//     !defined(UBLOX_PARSE_VELNED)  & !defined(UBLOX_PARSE_SVINFO)  & \
//     !defined(UBLOX_PARSE_HNR_PVT)

//   // #error No UBX binary messages enabled: no fix data available.

// #endif

// #ifndef NMEAGPS_RECOGNIZE_ALL
//   //  Resetting the messages with ublox::configNMEA requires that
//   //    all message types are recognized (i.e., the enum has all
//   //    values).
//   // #error You must "#define NMEAGPS_RECOGNIZE_ALL" in NMEAGPS_cfg.h!
// #endif

// // Need class that can force virtual method to the right parsing/decode method
// class MyGPS : public ubloxGPS
// {
// public:

//     MyGPS( Stream *device ) : ubloxGPS( device )
//     {
//       // state = GETTING_STATUS;
//     }
// };

// // Construct the GPS object and hook it to the appropriate serial device
// // static MyGPS gps( &gpsPort );




class mGPS_Serial :
  public mTaskerInterface
  // , ubloxGPS
{

  private:
  public:
    mGPS_Serial(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_SENSORS__GPS_SERIAL_CTR;
    static const char* PM_MODULE_SENSORS__GPS_SERIAL_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__GPS_SERIAL_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS__GPS_SERIAL_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mGPS_Serial);
    };
    #endif

    void EveryLoop_InputMethod_PollingSerial_Internal();
    void EveryLoop_InputMethod_PollingSerial_Bytes();
    void EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
    void EveryLoop_PollForGPSData(Stream& port);
    void EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();




// //------------------------------------------------------------
// // This object parses received characters
// //   into the gps.fix() data structure

// static NMEAGPS  gps;

// //------------------------------------------------------------
// //  Define a set of GPS fix information.  It will
// //  hold on to the various pieces as they are received from
// //  an RMC sentence.  It can be used anywhere in your sketch.

// static gps_fix  fix;

//----------------------------------------------------------------
//  This function gets called about once per second, during the GPS
//  quiet time.  It's the best place to do anything that might take
//  a while: print a bunch of things, write to SD, send an SMS, etc.
//
//  By doing the "hard" work during the quiet time, the CPU can get back to
//  reading the GPS chars as they come in, so that no chars are lost.





uint32_t sequence_test_global = 0;

    // struct INTERNAL_BUFFER{
    //   uint16_t buflen = 0;
    //   uint16_t bufused = 0;
    //   uint8_t* buffer = nullptr;
    // }gps_receive_buffer;
    
    // enum
    //   {
    //     GETTING_STATUS, 
    //     GETTING_LEAP_SECONDS, 
    //     GETTING_UTC, 
    //     RUNNING
    //   }
    //     state NEOGPS_BF(8);


        uint8_t flag_incoming_data_at_correct_runtime_baud = false;

        
const uint32_t COMMAND_DELAY = 500;

void changeBaud( const char *textCommand, unsigned long baud );


// static
//  MyGPS* gps2 = nullptr;//( &gpsPort );

    // void get_status();
    // void get_leap_seconds();
    // void get_utc();
    void send_UBX_enable_messages();
    // bool running();
    void configNMEA( uint8_t rate );
    void disableUBX();
    void enableUBX();
    uint8_t enableUBX_RequiredOnlyFor3DTracking();

    void Init_UBX_Only_Requires_PowerCycle();

    void sendUBX( const unsigned char *progmemBytes, size_t len );
        

    struct MY_GPS_VALS{
      int32_t lat = 1;
      int32_t lon = 1;
      int32_t alt = 1;
      uint16_t speed = 1;
      uint16_t heading_cd = 1;
      int32_t geoidHeight_cm = 1;
      uint8_t hours = 0;
      uint8_t minutes = 0;
      uint8_t seconds = 0;
      uint16_t dateTime_ms = 0;
    }my_gps_vals;

    uint32_t tSaved_SplashFix = millis();



    enum
    {
      NOT_STARTED,
      INIT,
      GETTING_STATUS, 
      GETTING_LEAP_SECONDS, 
      GETTING_UTC, 
      RUNNING
    }
    state NEOGPS_BF(8);


    //--------------------------

    void get_status();
    void get_leap_seconds();
    void get_utc();
    void start_running();
    bool running();
    bool DeviceConfigure();

    bool started_successfully = false;


    #ifdef ENABLE_GPS_PARSER_NMEA
    NMEAGPS*  nmea_parser = nullptr; // This parses the GPS characters
    #endif // ENABLE_GPS_PARSER_NMEA
    #ifdef ENABLE_GPS_PARSER_UBX
    #ifndef USE_DEVFEATURE_UBLOX_GLOBAL
    ubloxGPS*  ubx_parser = nullptr; // This parses the GPS characters
    #endif // USE_DEVFEATURE_UBLOX_GLOBAL
    #endif
    /**
     * @note Holds a partial result during parsing, only to be merged with the stored fix is valid
     * */
    gps_fix   fix_parsing;
    /**
     * @note Stores a valid solution, merged from the parsing fix
     * */
    gps_fix   fix_valid;


    enum GPS_INPUT_STREAM_METHOD_IDS{
      GPS_INPUT_STREAM_METHOD_POLLING_ID,
      GPS_INPUT_STREAM_METHOD_RINGBUFFERS_ID,
      GPS_INPUT_STREAM_METHOD_LENGTH_ID
    };
    void ReadGPSStream();
    void Splash_Latest_Fix(Stream* out);
    void Handle_Connection_And_Configuration();

    void Init_UBX_Only();
    void Init_UBX_Only2();

    uint32_t tSaved_parse_gps = 0;



    void Init();
    void Pre_Init();
    struct SETTINGS{
      uint8_t fEnableModule = false;
      uint8_t fShowManualSlider = false;
      uint8_t read_gps_method = GPS_INPUT_STREAM_METHOD_RINGBUFFERS_ID; 
    }settings;

    struct CONNECTION_STATUS{
      /**
       * @note Any activity at the expected baud rate
       * */
      uint32_t last_message_received_time = 0;
      /**
       * @note Parsed and valid messages on the expected baud, valid fix not required
       * */
      uint32_t last_valid_message_received_time = 0;


    }connection_status;

    struct RUNTIME_VARS{
      /**
       * 0 - not yet configured
       * 1 - baud valid, data receiving
       * 2 - registers set
       * */
      uint8_t ubx_config_status = 0;

      uint8_t ubx_messages_confirmed_enabled = 0;
    }runtime;

    /**
     * @brief Updated with `gps_result_stored` to include the exact data I need
     * */
    struct GPS_DESIRED{
      uint8_t status = 0; // status = 0(invalid),1(now valid),2(previously valid)

      int32_t latitideL = 0;
      float latitude = 0;
      int32_t longitudeL = 0;
      float longitude = 0;



      uint32_t milliseconds_of_day =  MILLISECONDS_IN_DAY;

      uint8_t fix_status = 0;
      uint8_t fix_status_last = 0;

      uint8_t fix_used_to_update_internal_rtc = false;




      
    }gps_latest;






























    void CommandSet_CreateFile_WithName(char* value);
    void CommandSet_SerialPrint_FileNames(const char* value);
    void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
        
    void CommandSet_ReadFile(const char* filename);


    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Scene(uint8_t json_level, bool json_appending);

    void WebCommand_Parse(void);


    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Required(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Minimal(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Debug(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Micro(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All(uint8_t json_level = 0, bool json_appending = true);
  
  #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mGPS_Serial>* ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mGPS_Serial> mqtthandler_gpspacket_minimal_teleperiod;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_required;
    struct handler<mGPS_Serial> mqtthandler_settings_teleperiod;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_debug;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_micro;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_all;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_GPSPACKET_MINIMAL_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      //later also send byte packet method for testing over mqtt
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
      
    struct handler<mGPS_Serial>* mqtthandler_list[6] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_gpspacket_minimal_teleperiod,
      &mqtthandler_gpspacket_required,
      &mqtthandler_gpspacket_all,
      &mqtthandler_gpspacket_micro,
      &mqtthandler_gpspacket_debug
    };
    
  #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif

// #include "globals.h"
// #include "main.h"
// //#include "transceive.h"

// #define GPS_SC_1 0xB5
// #define GPS_SC_2 0x62

// #define NAV_CLASS 0x01  
// int ublox_NAV_LENGTH_idx;
// #define NAV_AOPSTATUS_ID     0x06 
// #define NAV_AOPSTATUS_LENGTH 52
// #define NAV_PVT_ID           0x07 
// #define NAV_PVT_LENGTH       84
// #define NAV_CLOCK_ID         0x22 
// #define NAV_CLOCK_LENGTH     20
// #define NAV_POSLLH_ID        0x02 
// #define NAV_POSLLH_LENGTH    28

// #define NAV_STATUS_ID        0x03 
// #define NAV_STATUS_LENGTH    16
// #define NAV_SOL_ID           0x06 
// #define NAV_SOL_LENGTH       52
// #define NAV_TIMEUTC_ID       0x21 
// #define NAV_TIMEUTC_LENGTH   20
// #define NAV_TIMEGPS_ID       0x20 
// #define NAV_TIMEGPS_LENGTH   16
// #define NAV_TIMEVELNED_ID       0x12 
// #define NAV_TIMEVELNED_LENGTH   36

// unsigned char gps_parsed_status[3];
// int flag_gps_data_ready_to_parse;
// unsigned char encodebuffer[70];
// int encodebuffer_index;
// int flag_save_gps_data;
// unsigned long encoded_sequence;
// unsigned char gpspacketwaiting;
        
// //#define unsigned char U1

// void init_gps(void);
// void SendGPStoUART(void);

// struct NAV_POSLLH{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      unsigned long iTOW_ms;
//      signed long lon_deg_scaled_e7;
//      signed long lat_deg_scaled_e7;
//      long height_mm;
//      long hMSL_mm;
//      unsigned long hAcc_mm;
//      unsigned long vAcc_mm;
// }nav_posllh;
// extern struct NAV_POSLLH *ptr_NAV_POSLLH_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_POSLLH *ptr_NAV_POSLLH_r;//Declare a pointer that is used to read point to structures

// struct NAV_TIMEUTC{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      U4 iTOW_ms;
//      U4 tAcc_ns;
//      I4 nano_ns;
//      U2 year;
//      U1 month;
//      U1 day;
//      U1 hour;
//      U1 min;
//      U1 sec;
// }nav_timeutc;
// extern struct NAV_TIMEUTC *ptr_NAV_TIMEUTC_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_TIMEUTC *ptr_NAV_TIMEUTC_r;//Declare a pointer that is used to read point to structures

// struct NAV_TIMEGPS{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      U4 iTOW_ms;
//      U4 fTOW_ns;
//      I2 week;
//      I1 leapS_s;
//      X1 valid;
//      U4 tAcc_ns;
// }nav_timegps;
// extern struct NAV_TIMEGPS *ptr_NAV_TIMEGPS_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_TIMEGPS *ptr_NAV_TIMEGPS_r;//Declare a pointer that is used to read point to structures

// struct NAV_STATUS{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      U4 iTOW_ms;
//      U1 gpsFix;
//      X1 flags;
//      X1 fixStat;
//      X1 flags2;
//      U4 ttff;
//      U4 msss;
// }nav_status;
// extern struct NAV_STATUS *ptr_NAV_STATUS_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_STATUS *ptr_NAV_STATUS_r;//Declare a pointer that is used to read point to structures

// struct NAV_SOL{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      U4 iTOW_ms;
//      I4 fTOW_ns;  
//      I2 week_weeks;
//      U1 gpsFix;
//      X1 flags;
//      I4 ecefX;
//      I4 ecefY;
//      I4 ecefZ;
//      U4 pAcc;
//      I4 ecefVX;
//      I4 ecefVY;
//      I4 ecefVZ; 
//      U4 sAcc;
//      U2 pDOP;
// }nav_sol;
// extern struct NAV_SOL *ptr_NAV_SOL_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_SOL *ptr_NAV_SOL_r;//Declare a pointer that is used to read point to structures

// struct NAV_VELNED{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 payload_length;
//      U4 iTOW_ms;
//      I4 velN;  
//      I4 velE;
//      I4 velD;
//      U4 speed3D;
//      U4 speed2D;
//      I4 heading;
// }nav_velned;
// extern struct NAV_VELNED *ptr_NAV_VELNED_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_VELNED *ptr_NAV_VELNED_r;//Declare a pointer that is used to read point to structures


// struct NAV_DESIRED{ //means I can refer to Local_GPSData as just "Local_GPSData" but the compiler sees it as "struct Local_GPSData"
//      U1 gps_length;
//      U4 iTOW_ms;
//      I4 fTOW_ns;
//      I2 week_weeks;
//      U1 gpsFix;
//      I4 lon_deg_scaled_e7;
//      I4 lat_deg_scaled_e7;
//      I4 height_mm;
//      I4 hMSL_mm;         
//      U4 speed3D;
//      U4 speed2D;
//      I4 heading;
// }nav_desired;
// extern struct NAV_DESIRED *ptr_NAV_DESIRED_w;//Declare a pointer that is used to write point to structures
// extern struct NAV_DESIRED *ptr_NAV_DESIRED_r;//Declare a pointer that is used to read point to structures




// // OLD TO REMOVE
// int flag_gps_data_ready_to_parse;
// unsigned char encodegpsbuffer[70];
// int encodegpsbuffer_index;
// int flag_save_gps_data;
// unsigned long enconded_gps_sequence;






















