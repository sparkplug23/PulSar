#ifndef _MNEXTIONPANEL2_H
#define _MNEXTIONPANEL2_H 0.2

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


// #include <ArduinoJson.h>
// class mTaskerManager;

#ifdef ESP32
#define SERIAL_NEXTION_RX Serial2 //only in 32
#define SERIAL_NEXTION_TX Serial2
#else
#include <SoftwareSerial.h>
// #define SERIAL_NEXTION_RX Serial
// #define SERIAL_NEXTION_TX Serial
#endif

#define NEXTION_BAUD 38400


#include "2_CoreSystem/Time/mTime.h"

// Features to add
/*
- Flash error messages (error screen/page)
  -- user configurable options, error time, what to show
  -- font colour, background colour, flash, override brightness to full
  -- time period (only show during daytime)
- Flash "show" message via json format on special page
    {show_secs:5,message:"Heating OFF \n Water Temperature 43*c"}



*/



// #if defined(ESP8266) && !defined(D5)
// #define D5 (14)
// #define D6 (12)
// #define D7 (13)
// #define D8 (15)
// #endif

// #ifdef ESP32
// #define BAUD_RATE 57600
// #else
// #define BAUD_RATE 57600
// #endif
#define USE_NEXTION_SOFTWARE_SERIAL



class mNextionPanel{
  private:
    //#define D_MODULE_TOPIC_NAME "nextion"
  public:
    mNextionPanel();
    void pre_init();

    

// int8_t Tasker(uint8_t function, JsonObjectConst obj);
// int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
// void parsesub_TopicCheck_JSONCommand(JsonObjectConst obj);


// void parsesub_Commands(JsonObjectConst obj);
// void parsesub_SetMulti(JsonObjectConst obj);

  //#define USE_NEXTION_SOFTWARE_SERIAL

// Reminder: the buffer size optimizations here, in particular the isrBufSize that only accommodates
// a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial adapter gets read
// before another write is performed. Block writes with a size greater than 1 would usually fail. 

#ifdef USE_NEXTION_SOFTWARE_SERIAL 
SoftwareSerial* swSer;
#endif

#ifndef NEXTION_DEFAULT_PAGE_NUMBER
  #define NEXTION_DEFAULT_PAGE_NUMBER 2
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

// void nextionSendCmd_JSON(String s_str);

void Command_SetBrightness(uint8_t brightness_percentage);
void Command_SetBrightness255(uint8_t brightness);
void Command_ToggleBrightness();



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
  uint8_t page = 0;
  uint8_t page_saved = 0; //used to return to after message is flashed
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



void EverySecond_FlashScreen();

#define PAGE_COUNT_MAX 9
uint8_t page_count_user_set = PAGE_COUNT_MAX;

uint32_t tTest = millis();



    int8_t Tasker(uint8_t function);
    void init(void);

    void parse_JSONCommand();

    #define LONG_PRESS_DURATION 500
    uint8_t fEnableImmediateButtonTime = false;
    uint8_t fEnableIgnoreNextOffEvent = false;
    
    uint32_t tSavedSendHealth = 0;
    uint16_t healthsecs = 0;
    uint32_t uptime_seconds_nonreset = 0;

    uint32_t tSavedUptime2 = 0;

    uint32_t tSavedButtonONDurationEvent,tSavedButtonOFFEvent,tSavedButtonONEvent;

    void EveryLoop();

    String debugPrintln(String str);

    uint8_t otatransfererror = false;

    void wifiDisconnected();
    void wifiConnected();


    // OPTIONAL: Assign default values here.
    // char wifiSSID[32];// = ""; // Leave unset for wireless autoconfig. Note that these values will be lost
    // char wifiPass[64];// = ""; // when updating, but that's probably OK because they will be saved in EEPROM.

    // ////////////////////////////////////////////////////////////////////////////////////////////////////
    // // These defaults may be overwritten with values saved by the web interface
    // char mqttServer[64];// = "192.168.1.65";
    // char mqttPort[6];// = "1883";
    // char mqttUser[32];// = "";
    // char mqttPassword[32];//;//] = "";
    // char nextionNode[16];// = DEVICENAME_CTR;
    // char groupName[16];// = "plates";
    // char configUser[32];// = "admin";
    // char configPassword[32];// = "";
    //char motionPinConfig[3] = "0";
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    char* ConvertBytetoASCII(byte* data, uint8_t len);
    const float nextionVersion = 0.38;                     // Current NEXTION software release version
    byte nextionReturnBuffer[128];                      // Byte array to pass around data coming from the panel
    uint8_t nextionReturnIndex = 0;                     // Index for nextionReturnBuffer
    uint8_t nextionActivePage = 1;                      // Track active LCD page
    bool lcdConnected = false;                          // Set to true when we've heard something from the LCD
    char wifiConfigPass[9];                             // AP config password, always 8 chars + NUL
    char wifiConfigAP[19];                              // AP config SSID, nextionNode + 3 chars
    bool shouldSaveConfig = false;                      // Flag to save json config to SPIFFS
    bool nextionReportPage0 = false;                    // If false, don't report page 0 sendme
    const unsigned long updateCheckInterval = 43200000; // Time in msec between update checks (12 hours)
    unsigned long updateCheckTimer = 0;                 // Timer for update check
    const unsigned long nextionCheckInterval = 5000;    // Time in msec between nextion connection checks
    unsigned long nextionCheckTimer = 10;                // Timer for nextion connection checks
    unsigned int nextionRetryMax = 50;                   // Attempt to connect to panel this many times
    bool updateEspAvailable = false;                    // Flag for update check to report new ESP FW version
    float updateEspAvailableVersion;                    // Float to hold the new ESP FW version number
    bool updateLcdAvailable = false;                    // Flag for update check to report new LCD FW version
    bool debugSerialEnabled = true;                     // Enable USB serial debug output
    //bool debugTelnetEnabled = false;                    // Enable telnet debug output
    bool debugSerialD8Enabled = true;                   // Enable hardware serial debug output on pin D8
    //const unsigned long telnetInputMax = 128;           // Size of user input buffer for user telnet session
    //bool motionEnabled = false;                         // Motion sensor is enabled
    bool mdnsEnabled = true;                            // mDNS enabled
    //uint8_t motionPin = 0;                              // GPIO input pin for motion sensor if connected and enabled
    //bool motionActive = false;                          // Motion is being detected
    // const unsigned long motionLatchTimeout = 30000;     // Latch time for motion sensor
    // const unsigned long motionBufferTimeout = 1000;     // Latch time for motion sensor
    unsigned long lcdVersion = 0;                       // Int to hold current LCD FW version number
    unsigned long updateLcdAvailableVersion;            // Int to hold the new LCD FW version number
    bool lcdVersionQueryFlag = false;                   // Flag to set if we've queried lcdVersion
    char lcdVersionQuery[20];// = "p[0].b[2].val";     // Object ID for lcdVersion in HMI
    bool startupCompleteFlag = false;                   // Startup process has completed
    const long statusUpdateInterval = 300000;           // Time in msec between publishing MQTT status updates (5 minutes)
    long statusUpdateTimer = 0;                         // Timer for update check
    const unsigned long connectTimeout = 300;           // Timeout for WiFi and MQTT connection attempts in seconds
    const unsigned long reConnectTimeout = 15;          // Timeout for WiFi reconnection attempts in seconds
    byte espMac[6];                                     // Byte array to store our MAC address
    // const uint16_t mqttMaxPacketSize = 4096;            // Size of buffer for incoming MQTT message
    // String mqttClientId;                                // Auto-generated MQTT ClientID
    String mqttGetSubtopic;                             // MQTT subtopic for incoming commands requesting .val
    String mqttGetSubtopicJSON;                         // MQTT object buffer for JSON status when requesting .val
    String mqttStateTopic;                              // MQTT topic for outgoing panel interactions
    // String mqttStateJSONTopic;                          // MQTT topic for outgoing panel interactions in JSON format

    // // Includes my edits
    // String mqttStateJSONTopic2;                          // MQTT topic for outgoing panel interactions in JSON format

    // String mqttCommandTopic;                            // MQTT topic for incoming panel commands
    // String mqttGroupCommandTopic;                       // MQTT topic for incoming group panel commands
    // String mqttStatusTopic;                             // MQTT topic for publishing device connectivity state
    // String mqttSensorTopic;                             // MQTT topic for publishing device information in JSON format
    // String mqttLightCommandTopic;                       // MQTT topic for incoming panel backlight on/off commands
    // String mqttLightStateTopic;                         // MQTT topic for outgoing panel backlight on/off state
    // String mqttLightBrightCommandTopic;                 // MQTT topic for incoming panel backlight dimmer commands
    // String mqttLightBrightStateTopic;                   // MQTT topic for outgoing panel backlight dimmer state
    //String mqttMotionStateTopic;                        // MQTT topic for outgoing motion sensor state
    String nextionModel;                                // Record reported model number of LCD panel
    byte nextionSuffix[3];// = {0xFF, 0xFF, 0xFF};    // Standard suffix for Nextion commands
    uint32_t tftFileSize = 0;                           // Filesize for TFT firmware upload
    //uint8_t nextionResetPin = D6;                       // Pin for Nextion power rail switch (GPIO12/D6)

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
    bool nextionHandleInput();
    void nextionProcessInput();
    void nextionSetAttr(const char* hmiAttribute, const char* hmiValue);
    //void nextionSetAttr(String hmiAttribute, String hmiValue);
    void nextionGetAttr(String hmiAttribute);
    void nextionSendCmd(String nextionCmd);
    void nextionParseJson(const char* c_str);//String &strPayload);
    void nextionStartOtaDownload(String otaUrl);
    bool nextionOtaResponse();
    void nextionConnect();
    void nextionSetSpeed();
    void nextionReset();
    void espWifiSetup();
    void espWifiReconnect();
//    void espWifiConfigCallback(WiFiManager *myWiFiManager);
    void espSetupOta();
    void espStartOta(String espOtaUrl);
    void espReset();
    void configRead();
    void configSaveCallback();
    void configSave();
    void configClearSaved();
    void webHandleNotFound();
    void webHandleSaveConfig();
    void webHandleResetConfig();
    void webHandleNextionFirmware();
    void webHandleNextionFirmware_PhaseOut();
    void webHandleEspFirmware();
    void webHandleLcdUpload();
    void webHandleLcdUpdateSuccess();
    void webHandleLcdUpdateFailure();
    void webHandleLcdDownload();
    void webHandleTftFileSize();
    void webHandleReboot();
    bool updateCheck();
    // void debugPrintln(String debugText);
    // void debugPrint(String debugText);
    byte utf8ascii(byte ascii);
    String utf8ascii(String s);
    void utf8ascii(char *s);
    char* utf8ascii2(char *s);

    
void HandleUpgradeFirmwareStart(void);
void HandleUploadDone(void);
void HandleUploadLoop(void);
void HandlePreflightRequest(void);




    void Command_SetPage(uint8_t page);

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
    
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_EnergyStats(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mNextionPanel>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mNextionPanel> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mNextionPanel> mqtthandler_sensor_ifchanged;
    struct handler<mNextionPanel> mqtthandler_sensor_teleperiod;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    // const char* postfix_topic_energystats = "energystats";
    struct handler<mNextionPanel> mqtthandler_energystats_ifchanged;
    struct handler<mNextionPanel> mqtthandler_energystats_teleperiod;
  //#endif

  

};
#endif

#endif



/************
 * Page 0 - welcome screen
 * 
 * 
 * 
 * 
 * Page 1 - bedroom lights (2by4 buttons)
 * 
 * 
 * 
 * 
 * Page 2 - kitchen lights (2by4 buttons)
 * 
 * 
 * 
 * 
 * Page 3 - landing lights (2by4 buttons)
 * 
 * 
 * 
 * Page 4
 * 
 * 
 * 
 * Page 5 - heating layout
 * 
 * 
 * */