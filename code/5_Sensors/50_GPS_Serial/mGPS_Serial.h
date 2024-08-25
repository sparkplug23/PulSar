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
#define GPS_FIX_DATE

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


class mGPS_Serial :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mGPS_Serial(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_SENSORS__GPS_SERIAL_CTR = D_MODULE_SENSORS__GPS_SERIAL__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__GPS_SERIAL_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID; }    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mGPS_Serial);    };
    #endif
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot
      uint32_t last_message = 0;
      uint8_t valid_timeout_seconds = 0;
    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void RecheckConnection(void);

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Required(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Minimal(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Debug(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Micro(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mGPS_Serial>*> mqtthandler_list;
    struct handler<mGPS_Serial> mqtthandler_settings_teleperiod;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_minimal_teleperiod;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_required;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_all;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_micro;
    struct handler<mGPS_Serial> mqtthandler_gpspacket_debug;
    #endif // USE_MODULE_NETWORK_MQTT



    void EveryLoop_InputMethod_PollingSerial_Internal();
    void EveryLoop_InputMethod_PollingSerial_Bytes();
    void EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
    void EveryLoop_PollForGPSData(Stream& port);
    void EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();


    bool flag_show_incoming_gps_byte_stream = false;
    uint32_t sequence_test_global = 0;
    uint8_t flag_incoming_data_at_correct_runtime_baud = false;        
    const uint32_t COMMAND_DELAY = 500;
    void changeBaud( const char *textCommand, unsigned long baud );
    void send_UBX_enable_messages();
    void configNMEA( uint8_t rate );
    void disableUBX();
    void enableUBX();
    uint8_t enableUBX_RequiredOnlyFor3DTracking();

    void Init_UBX_Only_Requires_PowerCycle();

    void sendUBX( const unsigned char *progmemBytes, size_t len );

    struct STATS{
      uint32_t last_message_received_time = 0;
      uint32_t last_valid_message_received_time = 0;
      uint32_t packets_received = 0;
    }stats;
        

    struct MY_GPS_VALS{  //remove this, store more of anything. Make it dynamic, so memory is not held unless desired.
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


    // #ifdef ENABLE_GPS_PARSER_NMEA
    // NMEAGPS*  nmea_parser = nullptr; // This parses the GPS characters
    // #endif // ENABLE_GPS_PARSER_NMEA
    // #ifdef ENABLE_GPS_PARSER_UBX
    // #ifndef USE_DEVFEATURE_UBLOX_GLOBAL
    // ubloxGPS*  ubx_parser = nullptr; // This parses the GPS characters
    // #endif // USE_DEVFEATURE_UBLOX_GLOBAL
    // #endif
    // ubloxGPS*  gps_ublox = nullptr; // This parses the GPS characters
    // #ifdef USE_DEVFEATURE__UBLOX_TEST_CLASS
    // ubloxGPS*  ubx_test = nullptr; // This parses the GPS characters
    // #endif



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

    uint8_t read_gps_method = GPS_INPUT_STREAM_METHOD_RINGBUFFERS_ID; 

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

    uint8_t ConstructJSON_Scene(uint8_t json_level, bool json_appending);


};

#endif

#endif

