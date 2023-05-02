#ifndef _CELLULAR_H
#define _CELLULAR_H

#define D_UNIQUE_MODULE__NETWORK_CELLULAR__ID  ((3*1000)+05)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_CELLULAR


#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

#define TINY_GSM_MODEM_SIM7000

// debug inside library
#define TINY_GSM_DEBUG Serial

#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    true
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true

// set GSM PIN, if any
#define GSM_PIN ""

#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>

#ifdef ESP32
#include <driver/adc.h>
#endif

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

#define UART_CELLULAR_BAUD   921600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13
#define LED_PIN     12

#define DEFAULT_AT_COMMAND_RESPONSE_WAIT 5000


//https://cplusplus.com/reference/cstdio/sscanf/
//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


  //https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99


class mCellular :
  public mTaskerInterface
{

  private:
  public:
    mCellular(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE__NETWORK_CELLULAR__CTR;
    static const char* PM_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_CELLULAR__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__NETWORK_CELLULAR__ID; }
    
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mCellular);
    };
    #endif
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

    TinyGsm* modem = nullptr;
    TinyGsmClient* gsm_client = nullptr;

    int counter = 0, lastIndex = 0, numberOfPieces = 24;
    String pieces[24], input;


    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    struct GPS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled            
      float latitude = 54.5;// Latitude
      float longitude = -6.0;// Longitude
      float speed = 0;// Speed Over Ground. Unit is knots.
      float altitude = 0;// MSL Altitude. Unit is meters
      int vsat = 0;// GNSS Satellites in View
      int usat = 0;// GNSS Satellites Used
      float accuracy = 0;// Horizontal Dilution Of Precision
      int year = 0; // Four digit year
      int month = 0;// Two digit month
      int day = 0;// Two digit day
      int hour = 0;// Two digit hour
      int minute = 0;// Two digit minute
      int second = 0;// 6 digit second with subseconds
      
      float course      = 0;
      int   FixMode     = 0;
      float accuracy_position    = 0;
      float accuracy_vertical    = 0;
      int   usat_glonass = 0;
      int   cno_max = 0;
      int   HPA = 0;
      int   VPA = 0;


    }gps;
    void GPS_Enable();
    void GPS_Disable();
    void ModemUpdate_GPS();
    void SMS_GPSLocation();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    struct GPRS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled     
      uint32_t connected_seconds = 0;      
      uint32_t last_comms_millis_updated = 0; 
      float signal_quality_rssi_dbm = 0;
      uint16_t signal_quality_raw = 0;
    }gprs;
    void GPRS_Enable();
    void GPRS_Disable();
    void ModemUpdate_GPRS();
    void GPRS_Connect();


    struct SMS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled            
    }sms;
    void SMS_Enable();
    void SMS_Disable();
    void ModemUpdate_SMS();


    struct DATA
    {
      struct batt_status
      {
        uint16_t volts_mv    = 0;
        int8_t   percentage = 0;
        uint8_t  charge_state = 0;
        bool isvalid = false;
      }
      battery;

    }modem_status;

    
    void ModemUpdate_BatteryStatus();
      

    void ArduinoNetworkTest();
    void ArduinoExample_GPRSConnect();
    void ArduinoExample_GPRSConnect1();


    void modemPowerOn();
    void modemPowerOff();
    void modemRestart();

    void SendATCommand_SMSFormatAscii();
    void SendATCommand_SMSImmediateForwardOverSerial();
    void SendATCommand_FunctionalityMode_Minimum();
    void SendATCommand_FunctionalityMode_Full();

    void Handler_ModemResponses();
    void Handler_ModemResponses_Fast();
    char* ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen);


    void EveryLoop();

    void TestCode_RunAll();

    void Pre_Init();
    void Init(void);
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mCellular> mqtthandler_settings_teleperiod;
    struct handler<mCellular> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;  

    struct handler<mCellular>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

    
};



// ================
// /**
//  * @brief 
//  * 
//  * Short term ignoring this file for direct access to sub folder
//  * Longer, perhaps creating a parent class that future Celullar stuff can easily call indirectly
//  * 
//  */


// //parent class of BusDigital, BusPwm, and BusNetwork
// class mCellular  :
//   public mTaskerInterface
// {
//   public:
//     mCellular()
//     {
//     };

//     // Cellular(uint8_t type, uint16_t start, uint8_t aw)
//     // : _type(255)
//     // {
//     //   _type = type;
//     // };
    
//     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

//     #ifdef USE_DEBUG_CLASS_SIZE
//     uint16_t GetClassSize(){
//       return sizeof(mCellular);
//     };
//     #endif
    
//     struct SETTINGS{
//       uint8_t fEnableSensor = false;
//       uint8_t leds_found = 0;
//     }settings;


//     // virtual ~Cellular() {}

//     // virtual void     show() = 0;
//     // virtual bool     canShow() { return true; }
//     // virtual void     setStatusPixel(uint32_t c) {}
    
//     // virtual void     setPixelColorNew(uint16_t pix, uint16_t c) = 0;
//     // virtual uint16_t getPixelColorNew(uint16_t pix) { return 0; }

//     // virtual void     cleanup() = 0;
//     // virtual uint8_t  skippedLeds() { return 0; }

//     // private:
//     //     uint8_t _type = 0;

// };


// // class BusDigital : public Cellular {
// //   public:
// //     BusDigital(uint8_t &bc, uint8_t nr, const uint8_t &com);

// //     inline void show();
// //     bool canShow();
// //     void setStatusPixel(uint32_t c);

// //     void setPixelColorNew(uint16_t pix, uint16_t c);
// //     uint16_t getPixelColorNew(uint16_t pix);


// //     uint8_t skippedLeds() 
// //     {
// //       return _skip;
// //     }

// //     void reinit();

// //     void cleanup();

// //     ~BusDigital() {
// //       cleanup();
// //     }

// //   private:
// //     uint8_t _skip = 0;
// // };




#endif //  USE_MODULE_NETWORK_CELLULAR

#endif //  _CELLULAR_H