#pragma once

#ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_STYLE01

// #include "6_Lights/00_Interface/const.h"

#include "Arduino.h"


void serializeNetworks(JsonObject root);

void serializeNodes(JsonObject root);


    
void toggleOnOff();
byte scaledBri(byte in);

//udp.cpp
void notify(byte callMode, bool followUp=false);
uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri=255, bool isRGBW=false);
void realtimeLock(uint32_t timeoutMs, byte md = REALTIME_MODE_GENERIC);
void exitRealtime();
void handleNotifications();
void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w);
void refreshNodeList();
void sendSysInfoUDP();


bool deserializeConfig(JsonObject doc, bool fromFS = false);


void getStringFromJson(char* dest, const char* src, size_t len);


// #define MDNS_NAME DEVICENAME_CTR ".local"
// WLED_GLOBAL char cmDNS[] _INIT(MDNS_NAME);         pCONT_set->Settings.system_name.device

// Temp buffer
char* obuf;
uint16_t olen = 0;

bool oappend(const char* txt);
bool oappendi(int i);
void sappends(char stype, const char* key, char* val);
void sappend(char stype, const char* key, int val);


static String msgProcessor(const String& var);
void serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT);





void serveSettingsJS(AsyncWebServerRequest* request);
void serveSettings(AsyncWebServerRequest* request, bool post = false);

static String getContentType(AsyncWebServerRequest* request, String filename);
bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request);
void setStaticContentCacheHeaders(AsyncWebServerResponse *response);
bool handleFileRead(AsyncWebServerRequest* request, String path);


void serveIndex(AsyncWebServerRequest* request);
void getSettingsJS(byte subPage, char* dest);

void createEditHandler(bool enable);

void serializeSegment(JsonObject& root, mAnimatorLight::Segment_New& seg, byte id, bool forPreset = false, bool segmentBounds = true);
void serializeState(JsonObject root, bool forPreset = false, bool includeBri = true, bool segmentBounds = true, bool selectedSegmentsOnly = false);
void serializeInfo(JsonObject root);
void serializeModeNames(JsonArray root);
void serializeModeData(JsonArray root);
void serializePalettes(JsonObject root, int page);



    #define ARDUINOJSON_DECODE_UNICODE 0
    #include "3_Network/21_WebServer/AsyncJson-v6.h"
    #include "3_Network/21_WebServer/ArduinoJson-v6.h"

    
//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro



void serializeModeNames2(JsonArray arr, bool flag_get_first_name_only = true);

bool requestJSONBufferLock(uint8_t module);
void releaseJSONBufferLock();

void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);

void setPaletteColors(JsonArray json, CRGBPalette16 palette);

bool serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient = 0);

void serveJson(AsyncWebServerRequest* request);
bool deserializeSegment(JsonObject elem, byte it, byte presetId = 0);

void parseNumber(const char* str, byte* val, byte minv=0, byte maxv=255);
bool getVal(JsonVariant elem, byte* val, byte minv=0, byte maxv=255);

bool colorFromHexString(byte* rgb, const char* in);
bool deserializeState(JsonObject root, byte callMode = CALL_MODE_DIRECT_CHANGE, byte presetId = 0);

bool isIp(String str);


bool  captivePortal(AsyncWebServerRequest *request);
void  notFound(AsyncWebServerRequest *request);


void initServer();

#define JSON_PATH_STATE      1
#define JSON_PATH_INFO       2
#define JSON_PATH_STATE_INFO 3
#define JSON_PATH_NODES      4
#define JSON_PATH_PALETTES   5
#define JSON_PATH_FXDATA     6
#define JSON_PATH_NETWORKS   7
#define JSON_PATH_EFFECTS    8

// global ArduinoJson buffer
volatile uint8_t jsonBufferLock = 0;

// #ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_STYLE01

bool doReboot = false;



//realtime modes
#define REALTIME_MODE_INACTIVE    0
#define REALTIME_MODE_GENERIC     1
#define REALTIME_MODE_UDP         2
#define REALTIME_MODE_HYPERION    3
#define REALTIME_MODE_E131        4
#define REALTIME_MODE_ADALIGHT    5
#define REALTIME_MODE_ARTNET      6
#define REALTIME_MODE_TPM2NET     7
#define REALTIME_MODE_DDP         8

#ifndef WLED_MAX_BUTTONS
  #ifdef ESP8266
    #define WLED_MAX_BUTTONS 2
  #else
    #define WLED_MAX_BUTTONS 4
  #endif
#endif

#ifndef WLED_MAX_SEGNAME_LEN
  #ifdef ESP8266
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #define WLED_MAX_SEGNAME_LEN 64
  #endif
#else
  #if WLED_MAX_SEGNAME_LEN<32
    #undef WLED_MAX_SEGNAME_LEN
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #warning WLED UI does not support modified maximum segment name length!
  #endif
#endif

#define WLED_GLOBAL

// #ifndef WLED_DEFINE_GLOBAL_VARS
// # define WLED_GLOBAL extern
// # define _INIT(x)
// # define _INIT_N(x)
// #else
# define WLED_GLOBAL
# define _INIT(x) = x

//needed to ignore commas in array definitions
#define UNPACK( ... ) __VA_ARGS__
# define _INIT_N(x) UNPACK x
// #endif

#define WLED_ENABLE_JSONLIVE     // peek LED output via /json/live (WS binary peek is always enabled)

// User Interface CONFIG
#ifndef SERVERNAME
WLED_GLOBAL char serverDescription[33] _INIT("WLED");  // Name of module - use default
#else
WLED_GLOBAL char serverDescription[33] _INIT(SERVERNAME);  // use predefined name
#endif
WLED_GLOBAL bool syncToggleReceive     _INIT(false);   // UIs which only have a single button for sync should toggle send+receive if this is true, only send otherwise
WLED_GLOBAL bool simplifiedUI          _INIT(false);   // enable simplified UI
WLED_GLOBAL byte cacheInvalidate       _INIT(0);       // used to invalidate browser cache when switching from regular to simplified UI

#ifndef WLED_DISABLE_ESPNOW
WLED_GLOBAL bool enable_espnow_remote _INIT(false);
WLED_GLOBAL char linked_remote[13]   _INIT("");
WLED_GLOBAL char last_signal_src[13]   _INIT("");
#endif


WLED_GLOBAL char ntpServerName[33] _INIT("0.wled.pool.ntp.org");   // NTP server to use

// #define MDNS_NAME DEVICENAME_CTR ".local"
#define CLIENT_SSID "HACS2400"
#define CLIENT_PASS "af4d8bc9ab"
//Access point behavior
#define AP_BEHAVIOR_BOOT_NO_CONN          0     //Open AP when no connection after boot
#define AP_BEHAVIOR_NO_CONN               1     //Open when no connection (either after boot or if connection is lost)
#define AP_BEHAVIOR_ALWAYS                2     //Always open
#define AP_BEHAVIOR_BUTTON_ONLY           3     //Only when button pressed for 6 sec

// AP and OTA default passwords (for maximum security change them!)
WLED_GLOBAL char apPass[65]  _INIT(CLIENT_SSID);
WLED_GLOBAL char otaPass[33] _INIT("");

// WiFi CONFIG (all these can be changed via web UI, no need to set them here)
WLED_GLOBAL char clientSSID[33] _INIT(CLIENT_SSID);
WLED_GLOBAL char clientPass[65] _INIT(CLIENT_PASS);
// WLED_GLOBAL char cmDNS[] _INIT(MDNS_NAME);                       // mDNS address (*.local, replaced by wledXXXXXX if default is used)
WLED_GLOBAL char apSSID[33] _INIT("");                             // AP off by default (unless setup)
WLED_GLOBAL byte apChannel _INIT(1);                               // 2.4GHz WiFi AP channel (1-13)
WLED_GLOBAL byte apHide    _INIT(0);                               // hidden AP SSID
WLED_GLOBAL byte apBehavior _INIT(AP_BEHAVIOR_BOOT_NO_CONN);       // access point opens when no connection after boot by default
WLED_GLOBAL IPAddress staticIP   ;//   _INIT_N(((  0,   0,  0,  0))); // static IP of ESP
WLED_GLOBAL IPAddress staticGateway ;//_INIT_N(((  0,   0,  0,  0))); // gateway (router) IP
WLED_GLOBAL IPAddress staticSubnet;//  _INIT_N(((255, 255, 255, 0))); // most common subnet in home networks
#ifdef ARDUINO_ARCH_ESP32
WLED_GLOBAL bool noWifiSleep _INIT(true);                          // disabling modem sleep modes will increase heat output and power usage, but may help with connection issues
#else
WLED_GLOBAL bool noWifiSleep _INIT(false);
#endif

typedef enum mapping1D2D {
  M12_Pixels = 0,
  M12_pBar = 1,
  M12_pArc = 2,
  M12_pCorner = 3
} mapping1D2D_t;

// Settings sub page IDs
#define SUBPAGE_MENU              0
#define SUBPAGE_WIFI              1
#define SUBPAGE_LEDS              2
#define SUBPAGE_UI                3
#define SUBPAGE_SYNC              4
#define SUBPAGE_TIME              5
#define SUBPAGE_SEC               6
#define SUBPAGE_DMX               7
#define SUBPAGE_UM                8
#define SUBPAGE_UPDATE            9
#define SUBPAGE_2D               10
#define SUBPAGE_LOCK            251
#define SUBPAGE_PINREQ          252
#define SUBPAGE_CSS             253
#define SUBPAGE_JS              254
#define SUBPAGE_WELCOME         255

// string temp buffer (now stored in stack locally)
#ifdef ESP8266
#define SETTINGS_STACK_BUF_SIZE 2048
#else
#define SETTINGS_STACK_BUF_SIZE 3608  // warning: quite a large value for stack
#endif

#ifdef WLED_USE_ETHERNET
  #ifdef WLED_ETH_DEFAULT                                          // default ethernet board type if specified
    WLED_GLOBAL int ethernetType _INIT(WLED_ETH_DEFAULT);          // ethernet board type
  #else
    WLED_GLOBAL int ethernetType _INIT(WLED_ETH_NONE);             // use none for ethernet board type if default not defined
  #endif
#endif

// Timer mode types
#define NL_MODE_SET               0            //After nightlight time elapsed, set to target brightness
#define NL_MODE_FADE              1            //Fade to target brightness gradually
#define NL_MODE_COLORFADE         2            //Fade to target brightness and secondary color gradually
#define NL_MODE_SUN               3            //Sunrise/sunset. Target brightness is set immediately, then Sunrise effect is started. Max 60 min.

// LED CONFIG
WLED_GLOBAL bool turnOnAtBoot _INIT(true);                // turn on LEDs at power-up
WLED_GLOBAL byte bootPreset   _INIT(0);                   // save preset to load after power-up

//if true, a segment per bus will be created on boot and LED settings save
//if false, only one segment spanning the total LEDs is created,
//but not on LED settings save if there is more than one segment currently
WLED_GLOBAL bool autoSegments    _INIT(false);
WLED_GLOBAL bool correctWB       _INIT(false); // CCT color correction of RGB color
WLED_GLOBAL bool cctFromRgb      _INIT(false); // CCT is calculated from RGB instead of using seg.cct
WLED_GLOBAL bool gammaCorrectCol _INIT(true ); // use gamma correction on colors
WLED_GLOBAL bool gammaCorrectBri _INIT(false); // use gamma correction on brightness
WLED_GLOBAL float gammaCorrectVal _INIT(2.8f); // gamma correction value

WLED_GLOBAL byte col[4]    _INIT_N(({ 255, 160, 0, 0 }));  // current RGB(W) primary color. col[] should be updated if you want to change the color.
WLED_GLOBAL byte colSec[4] _INIT_N(({ 0, 0, 0, 0 }));      // current RGB(W) secondary color
// WLED_GLOBAL byte briS     _INIT(128);                     // default brightness

WLED_GLOBAL byte nightlightTargetBri _INIT(0);      // brightness after nightlight is over
WLED_GLOBAL byte nightlightDelayMins _INIT(60);
WLED_GLOBAL byte nightlightMode      _INIT(NL_MODE_FADE); // See const.h for available modes. Was nightlightFade
WLED_GLOBAL bool fadeTransition      _INIT(true);   // enable crossfading color transition
WLED_GLOBAL uint16_t transitionDelay _INIT(750);    // default crossfade duration in ms

WLED_GLOBAL byte briMultiplier _INIT(100);          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

// // User Interface CONFIG
// #ifndef SERVERNAME
// WLED_GLOBAL char serverDescription[33] _INIT("WLED");  // Name of module - use default
// #else
// WLED_GLOBAL char serverDescription[33] _INIT(SERVERNAME);  // use predefined name
// #endif

//Button type
#define BTN_TYPE_NONE             0
#define BTN_TYPE_RESERVED         1
#define BTN_TYPE_PUSH             2
#define BTN_TYPE_PUSH_ACT_HIGH    3
#define BTN_TYPE_SWITCH           4
#define BTN_TYPE_PIR_SENSOR       5
#define BTN_TYPE_TOUCH            6
#define BTN_TYPE_ANALOG           7
#define BTN_TYPE_ANALOG_INVERTED  8

// // Sync CONFIG
// WLED_GLOBAL NodesMap Nodes;
WLED_GLOBAL bool nodeListEnabled _INIT(true);
WLED_GLOBAL bool nodeBroadcastEnabled _INIT(true);

WLED_GLOBAL byte buttonType[WLED_MAX_BUTTONS]  _INIT({BTN_TYPE_PUSH});
#if defined(IRTYPE) && defined(IRPIN)
WLED_GLOBAL byte irEnabled      _INIT(IRTYPE); // Infrared receiver
#else
WLED_GLOBAL byte irEnabled      _INIT(0);     // Infrared receiver disabled
#endif
WLED_GLOBAL bool irApplyToAllSelected _INIT(true); //apply IR to all selected segments

WLED_GLOBAL uint16_t udpPort    _INIT(21324); // WLED notifier default port
WLED_GLOBAL uint16_t udpPort2   _INIT(65506); // WLED notifier supplemental port
WLED_GLOBAL uint16_t udpRgbPort _INIT(19446); // Hyperion port

WLED_GLOBAL uint8_t syncGroups    _INIT(0x01);                    // sync groups this instance syncs (bit mapped)
WLED_GLOBAL uint8_t receiveGroups _INIT(0x01);                    // sync receive groups this instance belongs to (bit mapped)
WLED_GLOBAL bool receiveNotificationBrightness _INIT(true);       // apply brightness from incoming notifications
WLED_GLOBAL bool receiveNotificationColor      _INIT(true);       // apply color
WLED_GLOBAL bool receiveNotificationEffects    _INIT(true);       // apply effects setup
WLED_GLOBAL bool receiveSegmentOptions         _INIT(false);      // apply segment options
WLED_GLOBAL bool receiveSegmentBounds          _INIT(false);      // apply segment bounds (start, stop, offset)
WLED_GLOBAL bool notifyDirect _INIT(false);                       // send notification if change via UI or HTTP API
WLED_GLOBAL bool notifyButton _INIT(false);                       // send if updated by button or infrared remote
WLED_GLOBAL bool notifyAlexa  _INIT(false);                       // send notification if updated via Alexa
WLED_GLOBAL bool notifyMacro  _INIT(false);                       // send notification for macro
WLED_GLOBAL bool notifyHue    _INIT(true);                        // send notification if Hue light changes
WLED_GLOBAL uint8_t udpNumRetries _INIT(0);                       // Number of times a UDP sync message is retransmitted. Increase to increase reliability

WLED_GLOBAL bool alexaEnabled _INIT(false);                       // enable device discovery by Amazon Echo
WLED_GLOBAL char alexaInvocationName[33] _INIT("Light");          // speech control name of device. Choose something voice-to-text can understand
WLED_GLOBAL byte alexaNumPresets _INIT(0);                        // number of presets to expose to Alexa, starting from preset 1, up to 9

WLED_GLOBAL uint16_t realtimeTimeoutMs _INIT(2500);               // ms timeout of realtime mode before returning to normal mode
WLED_GLOBAL int arlsOffset _INIT(0);                              // realtime LED offset
WLED_GLOBAL bool receiveDirect _INIT(true);                       // receive UDP realtime
WLED_GLOBAL bool arlsDisableGammaCorrection _INIT(true);          // activate if gamma correction is handled by the source
WLED_GLOBAL bool arlsForceMaxBri _INIT(false);                    // enable to force max brightness if source has very dark colors that would be black

#ifdef WLED_USE_ETHERNET
  #define E131_MAX_UNIVERSE_COUNT 20
#else
  #ifdef ESP8266
    #define E131_MAX_UNIVERSE_COUNT 9
  #else
    #define E131_MAX_UNIVERSE_COUNT 12
  #endif
#endif

//E1.31 DMX modes
#define DMX_MODE_DISABLED         0            //not used
#define DMX_MODE_SINGLE_RGB       1            //all LEDs same RGB color (3 channels)
#define DMX_MODE_SINGLE_DRGB      2            //all LEDs same RGB color and master dimmer (4 channels)
#define DMX_MODE_EFFECT           3            //trigger standalone effects of WLED (11 channels)
#define DMX_MODE_MULTIPLE_RGB     4            //every LED is addressed with its own RGB (ledCount * 3 channels)
#define DMX_MODE_MULTIPLE_DRGB    5            //every LED is addressed with its own RGB and share a master dimmer (ledCount * 3 + 1 channels)
#define DMX_MODE_MULTIPLE_RGBW    6            //every LED is addressed with its own RGBW (ledCount * 4 channels)

#ifdef WLED_ENABLE_DMX
 #ifdef ESP8266
  WLED_GLOBAL DMXESPSerial dmx;
 #else //ESP32
  WLED_GLOBAL SparkFunDMX dmx;
 #endif
WLED_GLOBAL uint16_t e131ProxyUniverse _INIT(0);                  // output this E1.31 (sACN) / ArtNet universe via MAX485 (0 = disabled)
#endif
WLED_GLOBAL uint16_t e131Universe _INIT(1);                       // settings for E1.31 (sACN) protocol (only DMX_MODE_MULTIPLE_* can span over consequtive universes)
WLED_GLOBAL uint16_t e131Port _INIT(5568);                        // DMX in port. E1.31 default is 5568, Art-Net is 6454
// WLED_GLOBAL byte e131Priority _INIT(0);                           // E1.31 port priority (if != 0 priority handling is active)
// WLED_GLOBAL E131Priority highPriority _INIT(3);                   // E1.31 highest priority tracking, init = timeout in seconds
WLED_GLOBAL byte DMXMode _INIT(DMX_MODE_MULTIPLE_RGB);            // DMX mode (s.a.)
WLED_GLOBAL uint16_t DMXAddress _INIT(1);                         // DMX start address of fixture, a.k.a. first Channel [for E1.31 (sACN) protocol]
WLED_GLOBAL uint16_t DMXSegmentSpacing _INIT(0);                  // Number of void/unused channels between each segments DMX channels
WLED_GLOBAL byte e131LastSequenceNumber[E131_MAX_UNIVERSE_COUNT]; // to detect packet loss
WLED_GLOBAL bool e131Multicast _INIT(false);                      // multicast or unicast
WLED_GLOBAL bool e131SkipOutOfSequence _INIT(false);              // freeze instead of flickering
WLED_GLOBAL uint16_t pollReplyCount _INIT(0);                     // count number of replies for ArtPoll node report

// mqtt
WLED_GLOBAL unsigned long lastMqttReconnectAttempt _INIT(0);  // used for other periodic tasks too
#ifndef WLED_DISABLE_MQTT
// WLED_GLOBAL AsyncMqttClient *mqtt _INIT(NULL);
WLED_GLOBAL bool mqttEnabled _INIT(false);
WLED_GLOBAL char mqttStatusTopic[40] _INIT("");            // this must be global because of async handlers
WLED_GLOBAL char mqttDeviceTopic[33] _INIT("");            // main MQTT topic (individual per device, default is wled/mac)
WLED_GLOBAL char mqttGroupTopic[33] _INIT("wled/all");     // second MQTT topic (for example to group devices)
WLED_GLOBAL char mqttServer[33] _INIT("");                 // both domains and IPs should work (no SSL)
WLED_GLOBAL char mqttUser[41] _INIT("");                   // optional: username for MQTT auth
WLED_GLOBAL char mqttPass[65] _INIT("");                   // optional: password for MQTT auth
WLED_GLOBAL char mqttClientID[41] _INIT("");               // override the client ID
WLED_GLOBAL uint16_t mqttPort _INIT(1883);
WLED_GLOBAL bool retainMqttMsg _INIT(false);               // retain brightness and color
#define WLED_MQTT_CONNECTED (mqtt != nullptr && mqtt->connected())
#else
#define WLED_MQTT_CONNECTED false
#endif

#ifndef WLED_DISABLE_HUESYNC
WLED_GLOBAL bool huePollingEnabled _INIT(false);           // poll hue bridge for light state
WLED_GLOBAL uint16_t huePollIntervalMs _INIT(2500);        // low values (< 1sec) may cause lag but offer quicker response
WLED_GLOBAL char hueApiKey[47] _INIT("api");               // key token will be obtained from bridge
WLED_GLOBAL byte huePollLightId _INIT(1);                  // ID of hue lamp to sync to. Find the ID in the hue app ("about" section)
WLED_GLOBAL IPAddress hueIP;// _INIT_N(((0, 0, 0, 0))); // IP address of the bridge
WLED_GLOBAL bool hueApplyOnOff _INIT(true);
WLED_GLOBAL bool hueApplyBri _INIT(true);
WLED_GLOBAL bool hueApplyColor _INIT(true);
#endif

WLED_GLOBAL uint16_t serialBaud _INIT(1152); // serial baud rate, multiply by 100

// Time CONFIG
WLED_GLOBAL bool ntpEnabled _INIT(false);    // get internet time. Only required if you use clock overlays or time-activated macros
WLED_GLOBAL bool useAMPM _INIT(false);       // 12h/24h clock format
WLED_GLOBAL byte currentTimezone _INIT(0);   // Timezone ID. Refer to timezones array in wled10_ntp.ino
WLED_GLOBAL int utcOffsetSecs _INIT(0);      // Seconds to offset from UTC before timzone calculation

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
#define MIN_SHOW_DELAY  15
#define DEFAULT_LED_COUNT 30

WLED_GLOBAL byte overlayCurrent _INIT(0);    // 0: no overlay 1: analog clock 2: was single-digit clock 3: was cronixie
WLED_GLOBAL byte overlayMin _INIT(0), overlayMax _INIT(DEFAULT_LED_COUNT - 1);   // boundaries of overlay mode

WLED_GLOBAL byte analogClock12pixel _INIT(0);               // The pixel in your strip where "midnight" would be
WLED_GLOBAL bool analogClockSecondsTrail _INIT(false);      // Display seconds as trail of LEDs instead of a single pixel
WLED_GLOBAL bool analogClock5MinuteMarks _INIT(false);      // Light pixels at every 5-minute position

WLED_GLOBAL bool countdownMode _INIT(false);                         // Clock will count down towards date
WLED_GLOBAL byte countdownYear _INIT(20), countdownMonth _INIT(1);   // Countdown target date, year is last two digits
WLED_GLOBAL byte countdownDay  _INIT(1) , countdownHour  _INIT(0);
WLED_GLOBAL byte countdownMin  _INIT(0) , countdownSec   _INIT(0);


WLED_GLOBAL byte macroNl   _INIT(0);        // after nightlight delay over
WLED_GLOBAL byte macroCountdown _INIT(0);
WLED_GLOBAL byte macroAlexaOn _INIT(0), macroAlexaOff _INIT(0);
WLED_GLOBAL byte macroButton[WLED_MAX_BUTTONS]        _INIT({0});
WLED_GLOBAL byte macroLongPress[WLED_MAX_BUTTONS]     _INIT({0});
WLED_GLOBAL byte macroDoublePress[WLED_MAX_BUTTONS]   _INIT({0});

// Security CONFIG
WLED_GLOBAL bool otaLock     _INIT(false);  // prevents OTA firmware updates without password. ALWAYS enable if system exposed to any public networks
WLED_GLOBAL bool wifiLock    _INIT(false);  // prevents access to WiFi settings when OTA lock is enabled
WLED_GLOBAL bool aOtaEnabled _INIT(true);   // ArduinoOTA allows easy updates directly from the IDE. Careful, it does not auto-disable when OTA lock is on
WLED_GLOBAL char settingsPIN[5] _INIT("");  // PIN for settings pages
WLED_GLOBAL bool correctPIN     _INIT(true);
WLED_GLOBAL unsigned long lastEditTime _INIT(0);

WLED_GLOBAL uint16_t userVar0 _INIT(0), userVar1 _INIT(0); //available for use in usermod

#ifdef WLED_ENABLE_DMX
  // dmx CONFIG
  WLED_GLOBAL byte DMXChannels _INIT(7);        // number of channels per fixture
  WLED_GLOBAL byte DMXFixtureMap[15] _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
  // assigns the different channels to different functions. See wled21_dmx.ino for more information.
  WLED_GLOBAL uint16_t DMXGap _INIT(10);          // gap between the fixtures. makes addressing easier because you don't have to memorize odd numbers when climbing up onto a rig.
  WLED_GLOBAL uint16_t DMXStart _INIT(10);        // start address of the first fixture
  WLED_GLOBAL uint16_t DMXStartLED _INIT(0);      // LED from which DMX fixtures start
#endif

// internal global variable declarations
// wifi
WLED_GLOBAL bool apActive _INIT(false);
WLED_GLOBAL bool forceReconnect _INIT(false);
WLED_GLOBAL uint32_t lastReconnectAttempt _INIT(0);
WLED_GLOBAL bool interfacesInited _INIT(false);
WLED_GLOBAL bool wasConnected _INIT(false);

// color
WLED_GLOBAL byte lastRandomIndex _INIT(0);        // used to save last random color so the new one is not the same

// transitions
WLED_GLOBAL bool          transitionActive        _INIT(false);
WLED_GLOBAL uint16_t      transitionDelayDefault  _INIT(transitionDelay); // default transition time (storec in cfg.json)
WLED_GLOBAL uint16_t      transitionDelayTemp     _INIT(transitionDelay); // actual transition duration (overrides transitionDelay in certain cases)
WLED_GLOBAL unsigned long transitionStartTime;
WLED_GLOBAL float         tperLast                _INIT(0.0f);            // crossfade transition progress, 0.0f - 1.0f
WLED_GLOBAL bool          jsonTransitionOnce      _INIT(false);           // flag to override transitionDelay (playlist, JSON API: "live" & "seg":{"i"} & "tt")
WLED_GLOBAL uint8_t       randomPaletteChangeTime _INIT(5);               // amount of time [s] between random palette changes (min: 1s, max: 255s)

// nightlight
WLED_GLOBAL bool nightlightActive _INIT(false);
WLED_GLOBAL bool nightlightActiveOld _INIT(false);
WLED_GLOBAL uint32_t nightlightDelayMs _INIT(10);
WLED_GLOBAL byte nightlightDelayMinsDefault _INIT(nightlightDelayMins);
WLED_GLOBAL unsigned long nightlightStartTime;
WLED_GLOBAL byte briNlT _INIT(0);                     // current nightlight brightness
WLED_GLOBAL byte colNlT[4] _INIT_N(({ 0, 0, 0, 0 }));        // current nightlight color

// brightness
WLED_GLOBAL unsigned long lastOnTime _INIT(0);
WLED_GLOBAL bool offMode             _INIT(!turnOnAtBoot);
// WLED_GLOBAL byte bri                 _INIT(briS);          // global brightness (set)
WLED_GLOBAL byte briOld              _INIT(0);             // global brightnes while in transition loop (previous iteration)
WLED_GLOBAL byte briT                _INIT(0);             // global brightness during transition
WLED_GLOBAL byte briLast             _INIT(128);           // brightness before turned off. Used for toggle function
WLED_GLOBAL byte whiteLast           _INIT(128);           // white channel before turned off. Used for toggle function

#define TOUCH_THRESHOLD 32 // limit to recognize a touch, higher value means more sensitive

// button
WLED_GLOBAL bool buttonPublishMqtt                            _INIT(false);
WLED_GLOBAL bool buttonPressedBefore[WLED_MAX_BUTTONS]        _INIT({false});
WLED_GLOBAL bool buttonLongPressed[WLED_MAX_BUTTONS]          _INIT({false});
WLED_GLOBAL unsigned long buttonPressedTime[WLED_MAX_BUTTONS] _INIT({0});
WLED_GLOBAL unsigned long buttonWaitTime[WLED_MAX_BUTTONS]    _INIT({0});
WLED_GLOBAL bool disablePullUp                                _INIT(false);
WLED_GLOBAL byte touchThreshold                               _INIT(TOUCH_THRESHOLD);

//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

WLED_GLOBAL unsigned long lastInterfaceUpdate _INIT(0);
WLED_GLOBAL byte interfaceUpdateCallMode _INIT(CALL_MODE_INIT);


// notifications
WLED_GLOBAL bool notifyDirectDefault _INIT(notifyDirect);
WLED_GLOBAL bool receiveNotifications _INIT(true);
WLED_GLOBAL unsigned long notificationSentTime _INIT(0);
WLED_GLOBAL byte notificationSentCallMode _INIT(CALL_MODE_INIT);
WLED_GLOBAL uint8_t notificationCount _INIT(0);

// effects
WLED_GLOBAL byte effectCurrent _INIT(0);
WLED_GLOBAL byte effectSpeed _INIT(128);
WLED_GLOBAL byte effectIntensity _INIT(128);
WLED_GLOBAL byte effectPalette _INIT(0);
WLED_GLOBAL bool stateChanged _INIT(false);

// network
WLED_GLOBAL bool udpConnected _INIT(false), udp2Connected _INIT(false), udpRgbConnected _INIT(false);

// ui style
WLED_GLOBAL bool showWelcomePage _INIT(false);

//Hue error codes
#define HUE_ERROR_INACTIVE        0
#define HUE_ERROR_UNAUTHORIZED    1
#define HUE_ERROR_LIGHTID         3
#define HUE_ERROR_PUSHLINK      101
#define HUE_ERROR_JSON_PARSING  250
#define HUE_ERROR_TIMEOUT       251
#define HUE_ERROR_ACTIVE        255

// hue
WLED_GLOBAL byte hueError _INIT(HUE_ERROR_INACTIVE);
// WLED_GLOBAL uint16_t hueFailCount _INIT(0);
WLED_GLOBAL float hueXLast _INIT(0), hueYLast _INIT(0);
WLED_GLOBAL uint16_t hueHueLast _INIT(0), hueCtLast _INIT(0);
WLED_GLOBAL byte hueSatLast _INIT(0), hueBriLast _INIT(0);
WLED_GLOBAL unsigned long hueLastRequestSent _INIT(0);
WLED_GLOBAL bool hueAuthRequired _INIT(false);
WLED_GLOBAL bool hueReceived _INIT(false);
WLED_GLOBAL bool hueStoreAllowed _INIT(false), hueNewKey _INIT(false);

// countdown
WLED_GLOBAL unsigned long countdownTime _INIT(1514764800L);
WLED_GLOBAL bool countdownOverTriggered _INIT(true);

//timer
WLED_GLOBAL byte lastTimerMinute  _INIT(0);
WLED_GLOBAL byte timerHours[10]     _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
WLED_GLOBAL int8_t timerMinutes[10] _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
WLED_GLOBAL byte timerMacro[10]     _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
//weekdays to activate on, bit pattern of arr elem: 0b11111111: sun,sat,fri,thu,wed,tue,mon,validity
WLED_GLOBAL byte timerWeekday[10]   _INIT_N(({ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }));
//upper 4 bits start, lower 4 bits end month (default 28: start month 1 and end month 12)
WLED_GLOBAL byte timerMonth[8]     _INIT_N(({28,28,28,28,28,28,28,28}));
WLED_GLOBAL byte timerDay[8]       _INIT_N(({1,1,1,1,1,1,1,1}));
WLED_GLOBAL byte timerDayEnd[9]		_INIT_N(({31,31,31,31,31,31,31,31}));

//improv
WLED_GLOBAL byte improvActive _INIT(0); //0: no improv packet received, 1: improv active, 2: provisioning
WLED_GLOBAL byte improvError _INIT(0);

//playlists
WLED_GLOBAL int16_t currentPlaylist _INIT(-1);
//still used for "PL=~" HTTP API command
WLED_GLOBAL byte presetCycCurr _INIT(0);
WLED_GLOBAL byte presetCycMin _INIT(1);
WLED_GLOBAL byte presetCycMax _INIT(5);

//realtime override modes
#define REALTIME_OVERRIDE_NONE    0
#define REALTIME_OVERRIDE_ONCE    1
#define REALTIME_OVERRIDE_ALWAYS  2

// realtime
WLED_GLOBAL byte realtimeOverride _INIT(REALTIME_OVERRIDE_NONE);
WLED_GLOBAL IPAddress realtimeIP;// _INIT_N(((0, 0, 0, 0)));
WLED_GLOBAL unsigned long realtimeTimeout _INIT(0);
WLED_GLOBAL uint8_t tpmPacketCount _INIT(0);
WLED_GLOBAL uint16_t tpmPayloadFrameSize _INIT(0);
WLED_GLOBAL bool useMainSegmentOnly _INIT(false);


#endif // ENABLE_DEVFEATURE_LIGHT__WEBUI_STYLE01
