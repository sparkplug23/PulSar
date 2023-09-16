#ifndef _MNEXTIONPANEL2_H
#define _MNEXTIONPANEL2_H 0.2

#define D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_DISPLAYS_NEXTION_ID    1    // Numerical accesending order of module within a group

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

#define SERIAL_NEXTION_RX Serial2 //only in 32
#define SERIAL_NEXTION_TX Serial2
// #else
// #include <SoftwareSerial.h>
// // #define SERIAL_NEXTION_RX Serial
// // #define SERIAL_NEXTION_TX Serial
// #endif

#define USE_HARDWARE_SERIAL_TEMP


#define NEXTION_BAUD 115200//38400

// comok 1,30601-0,NX3224T024_011R,163,61488,DE6064B7E70C6521,4194304    desksensor
// comok 1,30601-0,NX3224K024_011R,163,61489,E468549347403121,16777216   immersion_controller
// comok 1,30601-0,NX3224T024_011R,163,61488,DE6064B7E77A6F24,4194304    kitchenpanel  (Horizontal 90, Basic-NX3224T024_011)

#include "2_CoreSystem/02_Time/mTime.h"

// Features to add
/*
- Flash error messages (error screen/page)
  -- user configurable options, error time, what to show
  -- font colour, background colour, flash, override brightness to full
  -- time period (only show during daytime)
- Flash "show" message via json format on special page
    {show_secs:5,message:"Heating OFF \n Water Temperature 43*c"}
*/

#ifdef ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
#include <WiFi.h>

#ifndef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
#include <WebServer.h>
#include <WiFiManager.h>



#else

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif


const char HTTP_HEAD3[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE3[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT3[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END3[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS3[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/>";
//<form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM3[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START3[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM3[] PROGMEM      = "<br/><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END3[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK3[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED3[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect Weread to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END3[] PROGMEM             = "</div></body></html>";

#define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET ((size_t) -2)




#endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER

#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#endif


static const char HTTP_HEAD_START[] PROGMEM = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";

// // URL for auto-update check of "version.json"
// const char UPDATE_URL[] PROGMEM = "https://raw.githubusercontent.com/HASwitchPlate/HASPone/main/update/version.json";
// // Additional CSS style to match Hass theme
static const char HASP_STYLE[] PROGMEM = "<style>button{background-color:#03A9F4;}body{width:60%;margin:auto;}input:invalid{border:1px solid red;}input[type=checkbox]{width:20px;}.wrap{text-align:left;display:inline-block;min-width:260px;max-width:1000px}</style>";
// // Default link to compiled Arduino firmware image
// String espFirmwareUrl = "https://raw.githubusercontent.com/HASwitchPlate/HASPone/main/Arduino_Sketch/HASwitchPlate.ino.d1_mini.bin";
// // Default link to compiled Nextion firmware images
// String lcdFirmwareUrl = "https://raw.githubusercontent.com/HASwitchPlate/HASPone/main/Nextion_HMI/HASwitchPlate.tft";


class mNextionPanel :
  public mTaskerInterface
{
  public:
    mNextionPanel(){};
    void Pre_Init();
    
    
    static const char* PM_MODULE_DISPLAYS_NEXTION_CTR;
    static const char* PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DISPLAYS_NEXTION_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mNEXTION);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    // https://nextion.tech/instruction-set/
    /********************************************************************************************************************
     * @brief Return Codes dependent on bkcmd value being greater than 0
     * 
     *********************************************************************************************************************/
    /**
     * @NO:          1
     * @BYTE:        0x00
     * @BKCMD:       2,3
     * @LENGTH:      4
     * @MEANING:     Invalid Instruction
     * @FORMAT:      0x00 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when instruction sent by user has failed
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTION 0x00
    /**
     * @NO:          2
     * @BYTE:        0x01
     * @LENGTH:      4
     * @MEANING:     Instruction Successful
     * @FORMAT:      0x01 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when instruction sent by user was successful
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__INSTRUCTION_SUCCESSFUL 0x01
    /**
     * @NO:          3
     * @BYTE:        0x02
     * @LENGTH:      4
     * @MEANING:     Invalid Component ID
     * @FORMAT:      0x02 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when invalid Component ID or name was used
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__INVALID_COMPONENT_ID 0x02
    /**
     * @NO:          4
     * @BYTE:        0x03
     * @LENGTH:      4
     * @MEANING:     Invalid Page ID
     * @FORMAT:      0x03 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when invalid Page ID or name was used
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__INVALID_PAGE_ID 0x03
    
    


    //   5	0x04
    //     2,3	4	Invalid Picture ID	0x04 0xFF 0xFF 0xFF
    //   Returned when invalid Picture ID was used


    //   6	0x05
    //     2,3	4	Invalid Font ID	0x05 0xFF 0xFF 0xFF
    //   Returned when invalid Font ID was used


    //   7	0x06
    //     2,3	4	Invalid File Operation	0x06 0xFF 0xFF 0xFF
    //   Returned when File operation fails


    //   8	0x09
    //     2,3	4	Invalid CRC	0x09 0xFF 0xFF 0xFF
    //   Returned when Instructions with CRC validation fails their CRC check


    //   9	0x11
    //     2,3	4	Invalid Baud rate Setting	0x11 0xFF 0xFF 0xFF
    //   Returned when invalid Baud rate was used


    //   10	0x12
    //     2,3	4	Invalid Waveform ID or Channel #	0x12 0xFF 0xFF 0xFF
    //   Returned when invalid Waveform ID or Channel # was used


    //   11	0x1A
    //     2,3	4	Invalid Variable name or attribute	0x1A 0xFF 0xFF 0xFF
    //   Returned when invalid Variable name or invalid attribute was used


    //   12	0x1B
    //     2,3	4	Invalid Variable Operation	0x1B 0xFF 0xFF 0xFF
    //   Returned when Operation of Variable is invalid. ie: Text assignment t0.txt=abc or t0.txt=23, Numeric assignment j0.val=”50″ or j0.val=abc


    //   13	0x1C
    //     2,3	4	Assignment failed to assign	0x1C 0xFF 0xFF 0xFF
    //   Returned when attribute assignment failed to assign


    //   14	0x1D
    //     2,3	4	EEPROM Operation failed	0x1D 0xFF 0xFF 0xFF
    //   Returned when an EEPROM Operation has failed


    //   15	0x1E
    //     2,3	4	Invalid Quantity of Parameters	0x1E 0xFF 0xFF 0xFF
    //   Returned when the number of instruction parameters is invalid


    //   16	0x1F
    //     2,3	4	IO Operation failed	0x1F 0xFF 0xFF 0xFF
    //   Returned when an IO operation has failed


    //   17	0x20
    //     2,3	4	Escape Character Invalid	0x20 0xFF 0xFF 0xFF
    //   Returned when an unsupported escape character is used


    //   18	0x23
    //     2,3	4	Variable name too long	0x23 0xFF 0xFF 0xFF
    //   Returned when variable name is too long. Max length is 29 characters: 14 for page + “.” + 14 for component.


      

    //   /**
    //    * @brief Return Codes not affected by bkcmd value, valid in all cases
    //    * 
    //    */      

    //   /**
    //    * @NO:          19
    //    * @BYTE:        0x00
    //    * @LENGTH:      6
    //    * @MEANING:     Nextion Startup
    //    * @FORMAT:      0x00 0x00 0x00 0xFF 0xFF 0xFF
    //    * @DESCRIPTION: Returned when Nextion has started or reset. Since Nextion Editor v1.65.0, the Startup preamble is not at the firmware level but has been moved to a 
    //    *               printh statement in Program.s allowing a user to keep, modify or remove as they choose.
    //    */
    //   #define D_INSTRUCTION_SET_RETURN_CODE__NEXTION_STARTUP 0x00
    //   /**
    //    * @NO:          20
    //    * @BYTE:        0x00
    //    * @LENGTH:      6
    //    * @MEANING:     Nextion Startup
    //    * @FORMAT:      0x00 0x00 0x00 0xFF 0xFF 0xFF
    //    * @DESCRIPTION: Returned when Nextion has started or reset. Since Nextion Editor v1.65.0, the Startup preamble is not at the firmware level but has been moved to a 
    //    *               printh statement in Program.s allowing a user to keep, modify or remove as they choose.
    //    */
    //   #define D_INSTRUCTION_SET_RETURN_CODE__NEXTION_STARTUP 0x00
      
      
      
      
      
    //   20	0x24
    //     4	Serial Buffer Overflow	0x24 0xFF 0xFF 0xFF
    //   Returned when a Serial Buffer overflow occurs
    //   Buffer will continue to receive the current instruction, all previous instructions are lost.


    //   21	0x65
    //     7	Touch Event	0x65 0x00 0x01 0x01 0xFF 0xFF 0xFF
    //   Returned when Touch occurs and component’s
    //   corresponding Send Component ID is checked
    //   in the users HMI design.
    //   0x00 is page number, 0x01 is component ID,
    //   0x01 is event (0x01 Press and 0x00 Release)
    //   data: Page 0, Component 1, Pressed

    /**
     * @NO:          21
     * @BYTE:        0x65
     * @LENGTH:      7
     * @MEANING:     Touch Event
     * @FORMAT:      0x65 0x00 0x01 0x01 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when Touch occurs and component’s corresponding Send Component ID is checked in the users HMI design.
     *               0x00 is page number, 0x01 is component ID,
     *               0x01 is event (0x01 Press and 0x00 Release)
     *               data: Page 0, Component 1, Pressed
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT 0x65
    /**
     * @NO:          22
     * @BYTE:        0x66
     * @LENGTH:      7
     * @MEANING:     Current Page Number
     * @FORMAT:      0x66 0x01 0xFF 0xFF 0xFF
     * @DESCRIPTION: Returned when the sendme command is used.
     *               0x01 is current page number
     *               data: page 1
     */
    #define D_INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER 0x66



    //   22	0x66
    //     5	Current Page Number	0x66 0x01 0xFF 0xFF 0xFF
    //   Returned when the sendme command is used.
    //   0x01 is current page number
    //   data: page 1


    //   23	0x67
    //     9	Touch Coordinate (awake)	0x67 0x00 0x7A 0x00 0x1E 0x01 0xFF 0xFF 0xFF
    //   Returned when sendxy=1 and not in sleep mode
    //   0x00 0x7A is x coordinate in big endian order,
    //   0x00 0x1E is y coordinate in big endian order,
    //   0x01 is event (0x01 Press and 0x00 Release)
    //   (0x00*256+0x71,0x00*256+0x1E)
    //   data: (122,30) Pressed


    //   24	0x68
    //     9	Touch Coordinate (sleep)	0x68 0x00 0x7A 0x00 0x1E 0x01 0xFF 0xFF 0xFF
    //   Returned when sendxy=1 and exiting sleep
    //   0x00 0x7A is x coordinate in big endian order,
    //   0x00 0x1E is y coordinate in big endian order,
    //   0x01 is event (0x01 Press and 0x00 Release)
    //   (0x00*256+0x71,0x00*256+0x1E)
    //   data: (122,30) Pressed


    //   25	0x70
    //     Varied	String Data Enclosed	0x70 0x61 0x62 0x31 0x32 0x33 0xFF 0xFF 0xFF
    //   Returned when using get command for a string.
    //   Each byte is converted to char.
    //   data: ab123


    //   26	0x71
    //     8	Numeric Data Enclosed	0x71 0x01 0x02 0x03 0x04 0xFF 0xFF 0xFF
    //   Returned when get command to return a number
    //   4 byte 32-bit value in little endian order.
    //   (0x01+0x02*256+0x03*65536+0x04*16777216)
    //   data: 67305985


    //   27	0x86
    //     4	Auto Entered Sleep Mode	0x86 0xFF 0xFF 0xFF
    //   Returned when Nextion enters sleep automatically
    //   Using sleep=1 will not return an 0x86


    //   28	0x87
    //     4	Auto Wake from Sleep	0x87 0xFF 0xFF 0xFF
    //   Returned when Nextion leaves sleep automatically
    //   Using sleep=0 will not return an 0x87


    //   29	0x88
    //     4	Nextion Ready	0x88 0xFF 0xFF 0xFF
    //   Returned when Nextion has powered up and is now initialized successfully. Since Nextion Editor v1.65.0, the Nextion Ready is not at the firmware level but has been moved to a printh statement in Program.s allowing a user to keep, modify or remove as they choose.
    
    
    //   30	0x89
    //     4	Start microSD Upgrade	0x89 0xFF 0xFF 0xFF
    //   Returned when power on detects inserted microSD
    //   and begins Upgrade by microSD process


    //   31	0xFD
    //     4	Transparent Data Finished	0xFD 0xFF 0xFF 0xFF
    //   Returned when all requested bytes of Transparent
    //   Data mode have been received, and is now leaving transparent data mode (see 1.16)


    //   32	0xFE
    //     4	Transparent Data Ready	0xFE 0xFF 0xFF 0xFF
    //   Returned when requesting Transparent Data
    //   mode, and device is now ready to begin receiving
    //   the specified quantity of data (see 1.16)




    //   INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTION__ID = 0x00,
    //   INSTRUCTION_SET_RETURN_CODE__SERIAL_BUFFER_OVERFLOW__ID = 0x24,
    //   INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT__ID = 0x65,
    //   INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER__ID = 0x66,
    //   INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_AWAKE__ID = 0x67,
    //   INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_SLEEP__ID = 0x68,
    //   INSTRUCTION_SET_RETURN_CODE__STRING_DATA_ENCLOSED__ID = 0x70,
    //   INSTRUCTION_SET_RETURN_CODE__NUMERIC_DATA_ENCLODED__ID = 0x71,
    //   INSTRUCTION_SET_RETURN_CODE__AUTO_ENTERED_SLEEP_MODE__ID = 0x86,
    //   INSTRUCTION_SET_RETURN_CODE__AUTO_WAKE_FROM_SLEEP__ID = 0x87,
    //   INSTRUCTION_SET_RETURN_CODE__NEXTION_READY__ID = 0x88,
    //   INSTRUCTION_SET_RETURN_CODE__START_MICROSD_UPGRADE__ID = 0x89,
    //   INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_FINISHED__ID = 0xFD,
    //   INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_READY__ID = 0x00,
    //   INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTIONID = 0x00,

    // };


  //#define USE_NEXTION_SOFTWARE_SERIAL

  // Reminder: the buffer size optimizations here, in particular the isrBufSize that only accommodates
  // a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial adapter gets read
  // before another write is performed. Block writes with a size greater than 1 would usually fail. 

  #ifdef USE_NEXTION_SOFTWARE_SERIAL 
  SoftwareSerial* swSer;
  #endif

  #ifndef NEXTION_DEFAULT_PAGE_NUMBER
    #define NEXTION_DEFAULT_PAGE_NUMBER 9
  #endif

  int8_t pin_tx = -1;
  int8_t pin_rx = -1;
  uint8_t status_enabled = false; //new module flag that all will have to signify they are working


void init_serial();
int serial_available();
int serial_read();
void serial_print(const char* c_str);
void serial_print_suffix();
void serial_print_suffixed(const char* c_str);
void serial_print(String s_str);

void nextionGetAttr(const char* c_str);
void nextionSendCmd(const char* c_str);
void nextionSendCmd_ContainingFormattedText(const char* c_str);

// void nextionSendCmd_JSON(String s_str);

void Command_SetBrightness(uint8_t brightness_percentage);
void Command_SetBrightness255(uint8_t brightness);
void Command_ToggleBrightness();

void Template_Load_Init_Display_Command();

#ifdef USE_MODULE_NETWORK_WEBSERVER

int8_t Tasker_Web(uint8_t function);

void WebPage_Root_AddHandlers();

// void Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request);

#endif // USE_MODULE_NETWORK_WEBSERVER

#ifdef ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

#ifndef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
WebServer* webServer = nullptr;//(80);
#endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER



#endif // ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

#define WEB_HANDLE_NEXTION_PAGE_SET             "nextion_page_set"
#define WEB_HANDLE_NEXTION_SETTINGS             "nextion_settings" 
#define WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE    "nextion_brightness_toggle"
#define WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER    "nextion_brightness_slider"
#define WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR2    "rgb_pal"
#define WEB_HANDLE_RGB_COLOUR_MIXER_EDITOR2     "rgb_mix"
    

#define D_NEXTION_SETTINIGS "Nextion Settings"

#define WEB_HANDLE_NEXTION_SETTINGS "nextionsettings"
#define WEB_HANDLE_NEXTION_FIRMWARE "nextionfirmware"

#define WEB_HANDLE_NEXTION_TFTFILESIZE "tftFileSize"
#define WEB_HANDLE_NEXTION_LCDDOWNLOAD "lcddownload"
#define WEB_HANDLE_NEXTION_LCDOTASUCCESS "lcdOtaSuccess"
#define WEB_HANDLE_NEXTION_LCDOTAFAILURE "lcdOtaFailure"
#define WEB_HANDLE_NEXTION_LCDREBOOT "lcdreboot"
#define WEB_HANDLE_NEXTION_LCDUPLOAD "lcdupload"

#define D_NEXTION_SETTINGS "NEXTION Settings"
#define D_NEXTION_FIRMWARE "NEXTION Firmware"

#define PAGE_ERROR_ID 7
#define PAGE_MESSAGE_ID 7

struct SETTINGS{
  uint8_t brightness_percentage = 0;
  int8_t page = 0;
  uint8_t page_saved = 0; //used to return to after message is flashed
    // struct SETTINGS{
      struct FLAGS{
          // uint8_t EnableSceneModeWithSliders = true;
          // uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    // }settings;

    uint8_t dynamic_log_level = 9;// LOG_LEVEL_DEBUG_MORE; // used for certain addlog that may only have elevated states (to block large serial prints on recursive array prints)

    struct TimeoutCheck
    {
      uint8_t timeout_period = 120;
    }timeout_check;

}settings;

// Temporarily show message (eg connected) then return to previous screen from settings
struct FLASH_MESSAGE{
  int8_t cShowSeconds = 5;
  uint8_t page = 7;
  // uint32_t tSavedShow;
  char message[100];
  uint32_t background_colour; //24bit number
  uint32_t font_colour; //24bit number
  uint8_t isrunning = false;
}flash_message;


void parsesub_FlashMessage();

struct LAST_SCREEN_PRESS{
  uint8_t   page = 0;
  uint8_t   event = 0;
  uint8_t   fHandled = false;
  uint16_t  duration = 0;
  uint8_t   type = 0; //based on duration of press
  uint32_t  tSavedButtonONEvent = 0;
  uint32_t  tSavedButtonOFFEvent = 0;
  uint32_t  tSavedButtonONDurationEvent = 0;
  uint8_t   fEnableImmediateButtonTime = false;
}screen_press;



void EverySecond_ActivityCheck();

void EverySecond_FlashScreen();

#define PAGE_COUNT_MAX 9
uint8_t page_count_user_set = PAGE_COUNT_MAX;

uint32_t tTest = millis();


std::string& replace(std::string& s, const std::string& from, const std::string& to);

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);

    // void parse_JSONCommand();

    #define LONG_PRESS_DURATION 500
    uint8_t fEnableImmediateButtonTime = false;
    uint8_t fEnableIgnoreNextOffEvent = false;
    
    uint32_t tSavedSendHealth = 0;
    uint16_t healthsecs = 0;
    uint32_t uptime_seconds_nonreset = 0;

    uint32_t tSavedUptime2 = 0;

    uint32_t tSavedButtonONDurationEvent,tSavedButtonOFFEvent,tSavedButtonONEvent;

    void EveryLoop();

    uint8_t otatransfererror = false;

    void wifiDisconnected();
    void wifiConnected();

void Show_ConnectionWorking();
void Show_ConnectionNotWorking();


// WiFiClientSecure mqttClientSecure;        // TLS-enabled WiFiClient for MQTT
// WiFiClient wifiClient;                    // Standard WiFiClient
// MQTTClient mqttClient(mqttMaxPacketSize); // MQTT client
// ESP8266WebServer webServer(80);           // Admin web server on port 80
// ESP8266HTTPUpdateServer httpOTAUpdate;    // Arduino OTA server
// WiFiServer telnetServer(23);              // Telnet server (if enabled)
// WiFiClient telnetClient;                  // Telnet client
// MDNSResponder::hMDNSService hMDNSService; // mDNS
// EspSaveCrash SaveCrash;                   // Save crash details to flash

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // These defaults may be overwritten with values saved by the web interface
    char mqttServer[64];// = "192.168.1.65";
    char mqttPort[6];// = "1883";
    char mqttUser[32];// = "";
    char mqttPassword[32];//;//] = "";
    char nextionNode[16];// = DEVICENAME_CTR;
    char groupName[16];// = "plates";
    char configUser[32];// = "admin";
    char configPassword[32];// = "";
    char motionPinConfig[3] = "0";
char nextionBaud[7] = "115200";
char mqttFingerprint[60] = "";
char haspNode[16] = "plate01";
char hassDiscovery[128] = "homeassistant";
char wifiSSID[32] = "";
char wifiPass[64] = "";
    //////////////////////////////////////////////////////////////////////////////////////////////////

    char* ConvertBytetoASCII(byte* data, uint8_t len);
    // const float nextionVersion = 0.38;                     // Current NEXTION software release version
    byte nextionReturnBuffer[128];                      // Byte array to pass around data coming from the panel
    uint8_t nextionReturnIndex = 0;                     // Index for nextionReturnBuffer
    uint8_t nextionActivePage = 1;                      // Track active LCD page
    bool lcdConnected = false;                          // Set to true when we've heard something from the LCD
    // // char wifiConfigPass[9];                             // AP config password, always 8 chars + NUL
    // // char wifiConfigAP[19];                              // AP config SSID, nextionNode + 3 chars
    bool shouldSaveConfig = false;                      // Flag to save json config to SPIFFS
    bool nextionReportPage0 = false;                    // If false, don't report page 0 sendme
    const unsigned long updateCheckInterval = 43200000; // Time in msec between update checks (12 hours)
    unsigned long updateCheckTimer = 0;                 // Timer for update check
    // const unsigned long nextionCheckInterval = 5000;    // Time in msec between nextion connection checks
    // unsigned long nextionCheckTimer = 10;                // Timer for nextion connection checks
    // unsigned int nextionRetryMax = 50;                   // Attempt to connect to panel this many times
    bool updateEspAvailable = false;                    // Flag for update check to report new ESP FW version
    float updateEspAvailableVersion;                    // Float to hold the new ESP FW version number
    bool updateLcdAvailable = false;                    // Flag for update check to report new LCD FW version
    bool debugSerialEnabled = true;                     // Enable USB serial debug output
    bool debugTelnetEnabled = false;                    // Enable telnet debug output
    // bool debugSerialD8Enabled = true;                   // Enable hardware serial debug output on pin D8
    // //const unsigned long telnetInputMax = 128;           // Size of user input buffer for user telnet session
    // //bool motionEnabled = false;                         // Motion sensor is enabled
    bool mdnsEnabled = true;                            // mDNS enabled
    uint8_t motionPin = 0;                              // GPIO input pin for motion sensor if connected and enabled
    // //bool motionActive = false;                          // Motion is being detected
    // // const unsigned long motionLatchTimeout = 30000;     // Latch time for motion sensor
    // // const unsigned long motionBufferTimeout = 1000;     // Latch time for motion sensor
    unsigned long lcdVersion = 0;                       // Int to hold current LCD FW version number
    // unsigned long updateLcdAvailableVersion;            // Int to hold the new LCD FW version number
    bool lcdVersionQueryFlag = false;                   // Flag to set if we've queried lcdVersion
    const String lcdVersionQuery = "p[0].b[2].val";       // Object ID for lcdVersion in HMI
// bool startupCompleteFlag = false;                   // Startup process has completed
    const long statusUpdateInterval = 300000;           // Time in msec between publishing MQTT status updates (5 minutes)
    long statusUpdateTimer = 0;                         // Timer for update check
    // const unsigned long connectTimeout = 300;           // Timeout for WiFi and MQTT connection attempts in seconds
    // const unsigned long reConnectTimeout = 15;          // Timeout for WiFi reconnection attempts in seconds
    byte espMac[6];                                     // Byte array to store our MAC address
    // // // const uint16_t mqttMaxPacketSize = 4096;            // Size of buffer for incoming MQTT message
    String mqttClientId;                                // Auto-generated MQTT ClientID
    String mqttGetSubtopic;                             // MQTT subtopic for incoming commands requesting .val
    String mqttGetSubtopicJSON;                         // MQTT object buffer for JSON status when requesting .val
    String mqttStateTopic;                              // MQTT topic for outgoing panel interactions
    String mqttStateJSONTopic;                          // MQTT topic for outgoing panel interactions in JSON format

    // // // Includes my edits
    // // String mqttStateJSONTopic2;                          // MQTT topic for outgoing panel interactions in JSON format

    // // String mqttCommandTopic;                            // MQTT topic for incoming panel commands
    // // String mqttGroupCommandTopic;                       // MQTT topic for incoming group panel commands
    // // String mqttStatusTopic;                             // MQTT topic for publishing device connectivity state
    String mqttSensorTopic;                             // MQTT topic for publishing device information in JSON format
    // // String mqttLightCommandTopic;                       // MQTT topic for incoming panel backlight on/off commands
    // // String mqttLightStateTopic;                         // MQTT topic for outgoing panel backlight on/off state
    // // String mqttLightBrightCommandTopic;                 // MQTT topic for incoming panel backlight dimmer commands
    // // String mqttLightBrightStateTopic;                   // MQTT topic for outgoing panel backlight dimmer state
    // //String mqttMotionStateTopic;                        // MQTT topic for outgoing motion sensor state
    String nextionModel;                                // Record reported model number of LCD panel
    byte nextionSuffix[3];// = {0xFF, 0xFF, 0xFF};    // Standard suffix for Nextion commands
    uint32_t tftFileSize = 0;                           // Filesize for TFT firmware upload
    // //uint8_t nextionResetPin = D6;                       // Pin for Nextion power rail switch (GPIO12/D6)
unsigned long nextionAckTimer = 0;                    // Timer to track Nextion ack
bool nextionBufferOverrun = false;                    // Set to true if an overrun error was encountered
bool nextionAckEnable = false;                        // Wait for each Nextion command to be acked before continuing
bool nextionAckReceived = false;                      // Ack was received
bool beepEnabled = false;                             // Keypress beep enabled
 bool mqttTlsEnabled = false;                          // Enable MQTT client TLS connections
uint8_t nextionMaxPages = 11;                         // Maximum number of pages in Nextion project
bool mqttPingCheck = false;                           // MQTT broker ping check result
 bool ignoreTouchWhenOff = false;                      // Ignore touch events when backlight is off and instead send mqtt msg

 bool mqttPortCheck = false;                           // MQTT broke port check result
const float haspVersion = 1.05;                       // Current HASPone software release version
 bool lcdBacklightQueryFlag = false;                   // Flag to set if we've queried lcdBacklightDim
unsigned long updateCheckFirstRun = 60000;            // First-run check offset

 const unsigned long nextionAckTimeout = 1000;         // Timeout to wait for an ack before throwing error

    void MQTTSend_LongPressEvent();

    uint32_t tSaved_MQTTSend_PressEvent = millis();
    void MQTTSend_PressEvent();

    WiFiClient wifiClient;
    WiFiClient wifiMQTTClient;
    // MQTTClient* mqttClient;//(mqttMaxPacketSize);
    // ESP8266WebServer* webServer;//(80);
    // ESP8266HTTPUpdateServer httpOTAUpdate;
    // WiFiServer telnetServer(23);
    // WiFiClient telnetClient;
    // MDNSResponder::hMDNSService hMDNSService;

    //void espWifiConfigCallback(WiFiManager *myWiFiManager);
    uint32_t tSavedTest;

    uint8_t test_toggle = 0;

        
    void WebCommand_Parse(void);

    //first byte returned gives type of message
    #define NEXTION_COMMAND_INVALID_INSTRUCTION 0x1A
    #define D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR "Variable/Attribute name invalid"



    void mqttConnected();
    void mqttDisconnected();
    void mqttCallback(String &strTopic, String &strPayload);
    void mqttStatusUpdate();
    void nextionHandleInput();
    void nextionProcessInput();
    void nextionSetAttr(const char* hmiAttribute, const char* hmiValue);
    //void nextionSetAttr(String hmiAttribute, String hmiValue);
    void nextionGetAttr(String hmiAttribute);
    // void nextionSendCmd(String nextionCmd); //remove
    void nextionParseJson(const char* c_str);//String &strPayload);


#ifdef ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

    void nextionStartOtaDownload(String otaUrl);
    bool nextionOtaResponse();
    bool nextionConnect();
    void nextionSetSpeed();
    void nextionReset();
    void espWifiSetup();
    void espWifiReconnect();
//    void espWifiConfigCallback(WiFiManager *myWiFiManager);
    void espSetupOta();
    void configRead();
    void configSaveCallback();
    void configSave();
    void configClearSaved();
    
    
    void webHandleNextionFirmware();
    void webHandleNextionFirmware_PhaseOut();
    
    void webHandleLcdUpdateSuccess();
    void webHandleLcdUpdateFailure();
    void webHandleLcdDownload();
    void webHandleReboot();

    bool update_in_progress = false;
    
    void nextionOtaStartDownload(const String &lcdOtaUrl);
    void nextionUpdateProgress(const unsigned int &progress, const unsigned int &total);

void webHandleNotFound();
#endif // ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

  #ifndef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  void webHandleTftFileSize();

  void webHandleRoot();
  void WebPage_LCD_Update_TFT();
    void webHandleLcdUpload();
    void espReset();
    void webHandleFirmware();

    
    
  #endif
#ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  void webHandleTftFileSize(AsyncWebServerRequest* request);

  void webHandleRoot(AsyncWebServerRequest* request);

  void WebPage_LCD_Update_TFT(AsyncWebServerRequest *request);
  
void webHandleLcdUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void espReset();

    uint32_t transmitted_bytes = 0;

    
void webHandleFirmware(AsyncWebServerRequest *request);

#endif 

    void CommandSet_Baud(uint32_t baud);


    bool updateCheck();
    void debugPrintln(const String &debugText);
    // void debugPrint(const String &debugText);
    byte utf8ascii(byte ascii);
    String utf8ascii(String s); // to be converted to working char*
    // void utf8ascii(char *s);
    char* utf8ascii_Char(char *s);

    void HueToRgb(uint16_t hue, float* r, float* g, float* b);

    void nextionSendCmd_String(const String &nextionCmd);

    
    void HandleUpgradeFirmwareStart(void);
    void HandleUploadDone(void);
    void HandleUploadLoop(void);
    void HandlePreflightRequest(void);




    void Command_SetPage(uint8_t page);
    void Command_SetPageIfChanged(uint8_t page);

    // set or toggled via mqtt activity, maybe set true inside parse in tasker, with unset after 1 minute timeout
    #define OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS 150
    int16_t fOpenHABDataStreamActive_last_secs = 0;
    uint8_t fOpenHABDataStreamActive = false;
    uint32_t tSavedMillisOpenHABDataStreamActive;

    
    void HandleNextionSettings(void);
    void WebSave_NextionSettings(void);
    
    void SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr);
    void SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour);
    void SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour);

    void EverySecond_SendScreenInfo();
    
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_EnergyStats(uint8_t json_level = 0, bool json_appending = true);

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mNextionPanel>* ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mNextionPanel> mqtthandler_settings_teleperiod;
    struct handler<mNextionPanel> mqtthandler_sensor_ifchanged;
    struct handler<mNextionPanel> mqtthandler_sensor_teleperiod;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID,
    };
  
    struct handler<mNextionPanel>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif // USE_MODULE_DISPLAYS_NEXTION

#endif // header guard


/**
 * @brief 2023
 * 
 * 
 * Nextion should by default, send out system information of the ESP32 until it connects to wifi and OH sends the command automatically to set page
 * 
 * 
 * 
 * 



Conversion Mapping to make it easier to convert p[1] and b[23]

Use string class


conversion maps

p[index] -> p[name]
b[index] -> b[name]

pagemap: {
  “heating”, “error”, “message”

}

button_map which needs to differ on each page
array for page
0:boost_ds

^ key is search for, value is replace with

each page when I know which, the object that contains each search pattern should be pass into the translate function
=========================

short term, might still be easier to work out in OH to create a device map?


===================

super short term, for one page with heating, just manually create it all







 
 {
  "commands": [
    "page 5",
    "p[5].b[7].txt=\"Heating is working\"",
    "p[5].b[7].pco=2047",
    "p[5].b[7].bco=0",
    "p[5].b[11].txt=\"12.3\"",
    "p[5].b[11].pco=65500",
    "p[5].b[11].bco=0"
  ]
}

{
  "commands": [
    "page 1",
    "p[9].b[1].txt=\"hello\"",
    "p[9].b[1].bco=200"
  ]
}

{
  "commands": [
    "page 1",
    "p0.pic=1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=2047",
    "p[1].b[1].bco=0",
    "p[1].b[7].txt=\"12.3\"",
    "p[1].b[7].pco=65500",
    "p[1].b[7].bco=0",
    "p[1].b[8].txt=\"12.3\"",
    "p[1].b[8].pco=65500",
    "p[1].b[8].bco=0",
    "p[1].b[9].txt=\"12.3\"",
    "p[1].b[9].pco=65500",
    "p[1].b[9].bco=0",
    "p[1].b[10].txt=\"12.3\"",
    "p[1].b[10].pco=65500",
    "p[1].b[10].bco=0",
    "p[1].b[11].txt=\"12.3\"",
    "p[1].b[11].pco=65500",
    "p[1].b[11].bco=0",
    "p[1].b[12].txt=\"12.3\"",
    "p[1].b[12].pco=65500",
    "p[1].b[12].bco=0",
    "p[1].b[13].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0",
    "p[1].b[14].txt=\"12.3\"",
    "p[1].b[14].pco=65500",
    "p[1].b[14].bco=0",
    "p[1].b[2].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0"
  ]
}

developing conversion of colours

{
  "commands": [
    "page 1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=h120",
    "p[1].b[1].bco=0"
  ]
}

 * 
 */
