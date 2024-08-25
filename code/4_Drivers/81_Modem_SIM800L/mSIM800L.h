#ifndef _USE_MODULE_DRIVERS_MODEM_800L_H
#define _USE_MODULE_DRIVERS_MODEM_800L_H 0.3

#define D_UNIQUE_MODULE_DRIVERS_MODEM_800L_ID 45

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_MODEM_800L


// https://shop.marcomweb.it/media/kunena/attachments/42/wm02_an60_atc_timeouts_v05.pdf

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


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

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

#include <SPI.h>
#include <SD.h>
#include <Ticker.h>

#include <CStringWriter.h>

#ifdef ESP32
#include <driver/adc.h>
#endif

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

// #define UART_CELLULAR_BAUD   921600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13
#define LED_PIN     12

#define DEFAULT_AT_COMMAND_RESPONSE_WAIT 3000

#define AT_COMMAND_RESPONSE_TIMEOUT__CFUN 1000
#define AT_COMMAND_RESPONSE_TIMEOUT__CNMI 1000


#define SIM800L_IP5306_VERSION_20190610

#include <Wire.h>

#if defined(SIM800L_IP5306_VERSION_20190610)

#define MODEM_RST             5
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13
// #define LED_ON               HIGH
// #define LED_OFF              LOW

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

// // setPowerBoostKeepOn
// bool setupPMU()
// {
//     bool en = true;
//     Wire.begin(I2C_SDA, I2C_SCL);
//     Wire.beginTransmission(IP5306_ADDR);
//     Wire.write(IP5306_REG_SYS_CTL0);
//     if (en) {
//         Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
//     } else {
//         Wire.write(0x35); // 0x37 is default reg value
//     }
//     return Wire.endTransmission() == 0;
// }


#elif defined(SIM800L_AXP192_VERSION_20200327)

#define MODEM_RST            5
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define MODEM_DTR            32
#define MODEM_RI             33

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13
#define LED_ON               HIGH
#define LED_OFF              LOW


#elif defined(SIM800C_AXP192_VERSION_20200609)
// pin definitions
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       25
#define MODEM_TX             27
#define MODEM_RX             26
#define MODEM_DTR            32
#define MODEM_RI             33

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             12
#define LED_ON               LOW
#define LED_OFF              HIGH

#elif defined(SIM800L_IP5306_VERSION_20200811)


#define MODEM_RST             5
#define MODEM_PWRKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define MODEM_DTR            32
#define MODEM_RI             33

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13
#define LED_ON               HIGH
#define LED_OFF              LOW

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

// // setPowerBoostKeepOn
// bool setupPMU()
// {
//     bool en = true;
//     Wire.begin(I2C_SDA, I2C_SCL);
//     Wire.beginTransmission(IP5306_ADDR);
//     Wire.write(IP5306_REG_SYS_CTL0);
//     if (en) {
//         Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
//     } else {
//         Wire.write(0x35); // 0x37 is default reg value
//     }
//     return Wire.endTransmission() == 0;
// }

#else

#error "Please select the corresponding model"

#endif



//https://cplusplus.com/reference/cstdio/sscanf/
//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


  //https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99

#include <StreamDebugger.h>

class mSIM800L :
  public mTaskerInterface
{

  private:
  public:
    mSIM800L(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_CELLULAR__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_MODEM_800L_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mSIM800L); };
    #endif
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

    
    // StreamDebugger debugger(SerialAT, Serial);
    // TinyGsm modem(debugger);

    

    bool Modem__Running(uint16_t wait_millis = 1000);
    bool Modem__PowerUntilRunning(uint16_t wait_millis = 5000);
    bool SimNetwork__InitConfig();
    bool SimNetwork__StartConnection();
    bool SimNetwork__CheckConnection();

    bool DataNetwork__InitConfig();
    bool DataNetwork__StartConnection();
    bool DataNetwork__CheckConnection();

    bool Modem_CheckAndRestartUnresponsiveModem();


    float GetSignalQualityPower();
    float GetSignalQualityPower(int16_t signal_quality_raw);

    
    bool SendAT(const char* buffer, uint16_t wait_millis = 0);
    bool SendAT_F(uint16_t wait_millis, PGM_P formatP, ...);
    bool SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...);


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
    void SMS_GPSLocationAuto();
    void SMS_BatteryDetailed();


    struct SMSAuto_GPS_Messages{
      uint16_t rate_seconds = 0; //0 is disable
      uint32_t tSaved_LastSent = 0;
    }smsauto_gps_messages;
    void AutoSMS_Messages_Handle();


    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    
    void SMS_CommandIntoJSONCommand(char* command);




    void GPRS_UpdateConnectionState(bool state);
    
    struct GPRS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled  
      uint32_t last_comms_millis_updated = 0; 
      float signal_quality_rssi_dbm = 0;
      int16_t signal_quality_raw = 0;   
      uint32_t connected_seconds = 0;      
      uint16_t downtime_secs = 0;

      bool connected = true;


      uint16_t reconnect_init_counts = 0;

      uint16_t apn_connect_called = 0;

    }gprs;
    void GPRS_Disable();
    void ModemUpdate_GPRS();
    void Modem_Enable();

    struct SMS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled       
      std::vector<uint8_t> messages_incoming_index_list;     
    }sms;
    void SMS_Enable();
    void SMS_Disable();
    void ModemUpdate_SMS();
    void SMSReadAndEraseSavedSMS();
    
    bool Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis = 0);

    void Get_Modem_Hardware();

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

    struct SMS_PDU_Message{

      uint8_t length_of_smsc_information = 0;


    }sms_pdu_message;

    


    bool flag_modem_initialized = false;

    bool parse_ATCommands(char* buffer, uint16_t buflen, uint8_t log_level = 6);
     
    void ModemUpdate_BatteryStatus();

    void SMS_Send_TimedHeartbeat();
      


    void modemPowerOn();
    void modemPowerOff();
    void modemRestart();

    void SendATCommand_SMSFormatAscii();
    void SendATCommand_SMSFormatPDU();
    void SendATCommand_SMSImmediateForwardOverSerial();
    void SendATCommand_FunctionalityMode_Minimum();
    void SendATCommand_FunctionalityMode_Full();

    char* ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen);

    void ATParse_CMGD__CommandNameInTextDeleteMessage(char* buffer, uint8_t buflen, uint8_t response_loglevel);


    void EveryLoop();

    void Pre_Init();
    void Init(void);
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    std::vector<struct handler<mSIM800L>*> mqtthandler_list;
    
    struct handler<mSIM800L> mqtthandler_settings_teleperiod;
    struct handler<mSIM800L> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
      
    
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
// class mSIM800L  :
//   public mTaskerInterface
// {
//   public:
//     mSIM800L()
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
//       return sizeof(mSIM800L);
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

      // SendATCommand_SMSFormatAscii(); //tmp

      /**
       * @brief For debugging, lets check states
       * 
       */
      // modem->sendAT("+CMFG?"); // 1= ascii mode //expected 1
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""
    // "AT+CPMS?"     // Number of stored SMS

/*


{
  "ATCommands": [
    "AT+CMGR=1",
    "AT+CNMI=3,2,0,0,0",
    "AT+CMGL=\"REC UNREAD\""
  ]
}

*/




#endif

#endif