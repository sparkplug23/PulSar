#ifndef _USE_MODULE_DRIVERS_GPS_H
#define _USE_MODULE_DRIVERS_GPS_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_DRIVERS_GPS_ID 55

#ifdef USE_MODULE_DRIVERS_GPS

#include <string.h>
#include <strings.h>

#define NMEAGPS_DERIVED_TYPES
#define NMEAGPS_PARSE_PROPRIETARY
#ifdef USE_DEVFEATURE_GPS_POLLING_INPUT
#define gpsPort Serial2
#endif // USE_DEVFEATURE_GPS_POLLING_INPUT

#include "4_Drivers/GPS_UBX/internal/configs/NeoGPS_cfg.h"
#include "4_Drivers/GPS_UBX/internal/ublox/ubxGPS.h"




// #include "4_Drivers/GPS_UBX/internal/NMEA_Parser.h"
// #include "4_Drivers/GPS_UBX/internal/types/Streamers.h"

// #include "4_Drivers/GPS_UBX/internal/ublox/ubx_cfg.h"

// #include "4_Drivers/GPS_UBX/internal/configs/NeoGPS_cfg.h"
// #include "4_Drivers/GPS_UBX/internal/ublox/ubxGPS.h"

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



class mGPS :
  public mTaskerInterface
{

  private:
  public:
    mGPS(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_DRIVERS_GPS_CTR;
    static const char* PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_GPS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_GPS_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mGPS);
    };
    #endif

    void EveryLoop_InputMethod_PollingSerial_Internal();
    void EveryLoop_InputMethod_PollingSerial_Bytes();
    void EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
    void EveryLoop_PollForGPSData(Stream& port);
    void EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();

    struct INTERNAL_BUFFER{
      uint16_t buflen = 0;
      uint16_t bufused = 0;
      uint8_t* buffer = nullptr;
    }gps_receive_buffer;
    
    /**
     * @note Holds a partial result during parsing, only to be merged with the stored fix is valid
     * */
    GPS_FIX   gps_result_parsing;
    /**
     * @note Stores a valid solution, merged from the parsing fix
     * */
    GPS_FIX   gps_result_stored;

    uint32_t tSaved_SplashFix = millis();

    NMEAGPS*  gps_parser = nullptr; // This parses the GPS characters

    void init();
    void pre_init();
    struct SETTINGS{
      uint8_t fEnableModule = false;
      uint8_t fShowManualSlider = false;
    }settings;
    
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




      
    }gps_latest;


    void CommandSet_CreateFile_WithName(char* value);
    void CommandSet_SerialPrint_FileNames(const char* value);
    void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
        
    void CommandSet_ReadFile(const char* filename);


    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Scene(uint8_t json_method);

    void WebCommand_Parse(void);


    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_Required(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_Minimal(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_Debug(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_Micro(uint8_t json_method = 0);
    uint8_t ConstructJSON_GPSPacket_All(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mGPS>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mGPS> mqtthandler_gpspacket_minimal_teleperiod;
    struct handler<mGPS> mqtthandler_gpspacket_required;
    struct handler<mGPS> mqtthandler_settings_teleperiod;
    struct handler<mGPS> mqtthandler_gpspacket_debug;
    struct handler<mGPS> mqtthandler_gpspacket_micro;
    struct handler<mGPS> mqtthandler_gpspacket_all;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_GPSPACKET_MINIMAL_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      //later also send byte packet method for testing over mqtt
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
      
    struct handler<mGPS>* mqtthandler_list[6] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_gpspacket_minimal_teleperiod,
      &mqtthandler_gpspacket_required,
      &mqtthandler_gpspacket_all,
      &mqtthandler_gpspacket_micro,
      &mqtthandler_gpspacket_debug
    };


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






















