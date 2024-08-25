#ifndef _CELLULAR_H
#define _CELLULAR_H

#define D_UNIQUE_MODULE__NETWORK_CELLULAR__ID  ((3*1000)+05)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_CELLULAR

// https://shop.marcomweb.it/media/kunena/attachments/42/wm02_an60_atc_timeouts_v05.pdf

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


//https://cplusplus.com/reference/cstdio/sscanf/
//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


  //https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99

#include <StreamDebugger.h>

class mCellular :
  public mTaskerInterface
{

  private:
  public:
    /**
     * @brief Constructor
     * 
     */
    mCellular(){};

    /**
     * @brief Class Naming, used for mTaskerManager
     * 
     */
    static constexpr const char* PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_CELLULAR__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__NETWORK_CELLULAR__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mCellular); };
    #endif
    
    /**
     * @brief Common Tasker functions and configuration settings
     * 
     */
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init();
    void Init(void);
    void EveryLoop();

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

    /**
     * @brief Unique features of this class
     * 
     */

    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    std::vector<struct handler<mCellular>*> mqtthandler_list;
    
    struct handler<mCellular> mqtthandler_settings_teleperiod;
    struct handler<mCellular> mqtthandler_state_ifchanged;

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



#endif //  USE_MODULE_NETWORK_CELLULAR

#endif //  _CELLULAR_H