#ifndef _MWEBSERVER_H
#define _MWEBSERVER_H

#define D_UNIQUE_MODULE_NETWORK_WEBSERVER_ID  3021 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

#include <Arduino.h>
#include "2_CoreSystem/08_Logging/mLogging.h"

// #include "html_ui.h"
#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/01_Settings/mSettings.h"

#include <SPIFFSEditor.h>

#define Network WiFi

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFSEditor.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif

#ifdef ESP32
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif
#ifdef ESP8266
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif

#include <stdint.h>


const char PM_WEB_CONTENT_TYPE_TEXT_HTML[] PROGMEM = "text/html";
const char PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT[] PROGMEM = "text/javascript";
DEFINE_PGM_CTR(PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT) "application/json";
const char PM_WEB_CONTENT_TYPE_TEXT_CSS[] PROGMEM = "text/css";

// define flash strings once (saves flash memory)
static const char s_redirecting[] PROGMEM = "Redirecting...";
static const char s_content_enc[] PROGMEM = "Content-Encoding";
static const char s_unlock_ota [] PROGMEM = "Please unlock OTA in security settings!";
static const char s_unlock_cfg [] PROGMEM = "Please unlock settings using PIN code!";



#define D_WEB_HANDLE_CONSOLE "/console" // change to animator_settings
#define D_WEB_HANDLE_CONSOLE_PAGE "/PAGEconsole" // change to animator_settings
DEFINE_PGM_CTR(PM_WEB_HANDLE_CONSOLE) D_WEB_HANDLE_CONSOLE;
#define D_BUTTO

#ifndef D_CAPTIVE_PORTAL_URL_REDIRECT_PATH
#define D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/settings2/welcome"
// #define D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/settings2/wifi"
#endif


#include "3_Network/21_WebServer/Webpages/Generated/html_settings.h"
#include "3_Network/21_WebServer/Webpages/Generated/html_settings2.h"
#include "3_Network/21_WebServer/Webpages/Generated/html_other.h"
#include "3_Network/21_WebServer/Webpages/Generated/root_basic.h"
#include "3_Network/21_WebServer/Webpages/Generated/submodule_assets.h"
#include "3_Network/21_WebServer/Webpages/Generated/submodule_unified_pages.h"




// pages_console_select.h
#if defined(ESP8266)
  #include "3_Network/21_WebServer/Webpages/Generated/pages_console_esp8266.h"
#else
  #include "3_Network/21_WebServer/Webpages/Generated/pages_console_esp32.h"
#endif



// Settings sub page IDs
// THESE ALL NEED IMMEDIATE RENAMES
#define SUBPAGE_WEB_MENU              0
#define SUBPAGE_WEB_WIFI              1
#define SUBPAGE_WEB_LEDS              2
#define SUBPAGE_WEB_UI                3
#define SUBPAGE_WEB_SYNC              4
#define SUBPAGE_WEB_TIME              5
#define SUBPAGE_WEB_SEC               6
#define SUBPAGE_WEB_DMX               7
#define SUBPAGE_WEB_UM                8
#define SUBPAGE_WEB_UPDATE            9
#define SUBPAGE_WEB_2D               10
#define SUBPAGE_WEB_LOCK            251
#define SUBPAGE_WEB_PINREQ          252
#define SUBPAGE_WEB_CSS             253
#define SUBPAGE_WEB_JS              254
#define SUBPAGE_WEB_WELCOME         255

#define JSON_PATH_WEB_STATE      1
#define JSON_PATH_WEB_INFO       2
#define JSON_PATH_WEB_STATE_INFO 3
#define JSON_PATH_WEB_NODES      4
#define JSON_PATH_WEB_PALETTES   5
#define JSON_PATH_WEB_FXDATA     6
#define JSON_PATH_WEB_NETWORKS   7
#define JSON_PATH_WEB_EFFECTS    8




static const char HTTP_HEAD_START[] PROGMEM = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";

// // URL for auto-update check of "version.json"
// const char UPDATE_URL[] PROGMEM = "https://raw.githubusercontent.com/HASwitchPlate/HASPone/main/update/version.json";
// // Additional CSS style to match Hass theme
static const char HASP_STYLE[] PROGMEM = "<style>button{background-color:#03A9F4;}body{width:60%;margin:auto;}input:invalid{border:1px solid red;}input[type=checkbox]{width:20px;}.wrap{text-align:left;display:inline-block;min-width:260px;max-width:1000px}</style>";

const char HTTP_HEAD3[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE3[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char  HTTP_SCRIPT3[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
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



#include "1_TaskerManager/mTaskerInterface.h"


    #define ARDUINOJSON_DECODE_UNICODE 0
    #include "3_Network/21_WebServer/AsyncJson-v6.h"
    #include "3_Network/21_WebServer/ArduinoJson-v6.h"

class mWebServer : 
// public AsyncWebHandler, 
public mTaskerInterface{
  public:
    mWebServer(){


      
    };
    
    static constexpr const char* PM_MODULE_NETWORK_WEBSERVER_CTR = D_MODULE_NETWORK_WEBSERVER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_WEBSERVER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_WEBSERVER_ID; }

    void createEditHandler(bool enable);

    void Server_Start();

    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    #ifndef MDNS_NAME
      #define MDNS_NAME DEFAULT_MDNS_NAME
    #endif
    char cmDNS[33];// = MDNS_NAME;                // mDNS address (*.local, replaced by wledXXXXXX if default is used)



    enum CONTENT_TYPE_IDS{
      // 0 is unset
      CONTENT_TYPE_TEXT_HTML_ID=1,
      CONTENT_TYPE_TEXT_JAVASCRIPT_ID,
      CONTENT_TYPE_APPLICATION_JSON_ID,
      CONTENT_TYPE_TEXT_CSS_ID,
    };




// server library objects
AsyncWebServer* server = nullptr;

AsyncWebSocket* websocket_lights = nullptr;


AsyncWebHandler *editHandler = nullptr;

  #ifndef ESP8266
    #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET

    void HandlePage_Console_WebSocket(AsyncWebServerRequest *request);

    AsyncWebSocket* websocket_console = nullptr;
    void sendConsoleWs(AsyncWebSocketClient *client = nullptr);
    void handleConsoleWs();

    void wsEventConsole(AsyncWebSocket *server,
                        AsyncWebSocketClient *client,
                        AwsEventType type,
                        void *arg,
                        uint8_t *data,
                        size_t len);

    // Date Modified: 02Jan26
    struct PendingJsonCommand
    {
      char*    ptr = nullptr;
      uint16_t len = 0;

      inline bool has() const { return (ptr != nullptr) && (len > 0); }

      inline bool create(uint16_t new_len)
      {
        clear();
        if (!new_len) return false;

        ptr = (char*)malloc(new_len + 1);
        if (!ptr) {
          len = 0;
          return false;
        }

        len = new_len;
        ptr[len] = '\0';
        return true;
      }

      inline void clear()
      {
        if (ptr) {
          free(ptr);
          ptr = nullptr;
        }
        len = 0;
      }
    };


    static PendingJsonCommand pending_cmd;

    uint16_t      wsConsoleLiveClientId = 0;
    unsigned long wsConsoleLastPushTime = 0;
    uint8_t wsConsoleLastIdx = 0;   // 0 = sync pending
    #define WS_CONSOLE_INTERVAL 50

    #endif
  #endif


void serveSettingsJS(AsyncWebServerRequest* request);
void serveJson(AsyncWebServerRequest* request);

static void extractPin(Print& settingsScript, JsonObject &obj, const char *key);
void XML_response(Print& dest);
static void fillUMPins(Print& settingsScript, JsonObject &mods);
void appendGPIOinfo(Print& settingsScript);
void getSettingsJS(byte subPage, Print& settingsScript);


size_t printSetFormCheckbox(Print& settingsScript, const char* key, int val);
size_t printSetFormValue(Print& settingsScript, const char* key, int val);
size_t printSetFormIndex(Print& settingsScript, const char* key, int index);
size_t printSetFormValue(Print& settingsScript, const char* key, const char* val);
size_t printSetClassElementHTML(Print& settingsScript, const char* key, const int index, const char* val);
size_t printSetFormInput(Print& settingsScript, const char* key, const char* selector, int value) ;
size_t printSetElementStyle(Print& settingsScript, const char* element_id, const char* css_prop, const char* css_val);
size_t printToggleElementClass(Print& settingsScript,  const char* element_id,  const char* class_name,  bool enable);



    void SettingsPages__ParseForm(AsyncWebServerRequest *request, byte subPage);

// ---- System Controls (polling containers) ----
void HandlePage_SystemControls(AsyncWebServerRequest *request);
void HandlePage_SystemControls_C1(AsyncWebServerRequest *request);
void HandlePage_SystemControls_C2(AsyncWebServerRequest *request);
void HandlePage_SystemControls_C3(AsyncWebServerRequest *request);

// Active append stream context for Tasker-based append (shared within request scope)
AsyncResponseStream* web_controls_stream = nullptr;
uint8_t web_controls_container_id = 0;

// Helpers modules may call (via tkr_web pointer) while handling TASK_WEB_APPEND_* events
inline Print* WebControls_GetPrint() { return (Print*)web_controls_stream; }
inline uint8_t WebControls_GetContainerId() const { return web_controls_container_id; }
size_t WebUI_Print_SectionBegin(Print& out, const char* title);
size_t WebUI_Print_SectionEnd(Print& out);

size_t WebUI_Print_TableBegin(Print& out);
size_t WebUI_Print_TableEnd(Print& out);

size_t WebUI_Print_KV_Float(Print& out, const char* key, float value, uint8_t decimals, const char* units);
size_t WebUI_Print_KV_U32(Print& out, const char* key, uint32_t value, const char* units);
size_t WebUI_Print_KV_Str(Print& out, const char* key, const char* value);
size_t WebUI_Print_TableHeaderRow_Begin(Print& out);
size_t WebUI_Print_TableHeaderCell(Print& out, const char* text, bool is_first_blank = false);
size_t WebUI_Print_TableHeaderRow_End(Print& out);

size_t WebUI_Print_RowBegin(Print& out, const char* key);
size_t WebUI_Print_CellText(Print& out, const char* text);
size_t WebUI_Print_CellFloat(Print& out, float value, uint8_t decimals, const char* units);
size_t WebUI_Print_CellDash(Print& out);
size_t WebUI_Print_RowEnd(Print& out);






/**
 * @brief WebUI: Settings Pages (POST)
void serveSettings(AsyncWebServerRequest* request, bool post = false);
 */
void SettingsPages_POST(AsyncWebServerRequest* request);
void SettingsPages_GET(AsyncWebServerRequest* request);




void serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT);




void handleStaticContent(AsyncWebServerRequest *request, const String &path, int code, const String &contentType, const uint8_t *content, size_t len, bool gzip = true, uint16_t eTagSuffix = 0);


    bool reset_web_log_flag = false;                  // Reset web console log
    uint8_t fConsole_active = false;
    // Date Modified: 01Jan26
bool reset_web_log_flag_console2 = true;


unsigned long lastEditTime =  0;
   
bool HttpCheckPriviledgedAccess();
    
    void WebPage_Root_AddHandlers();
    
    
    #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
    void HandlePage_Console_Poll(AsyncWebServerRequest *request);
    void HandleConsoleRefresh(AsyncWebServerRequest *request);
    #endif

    bool WebGetArg(AsyncWebServerRequest *request, const char* arg, char* out, size_t max);
    

bool captivePortal(AsyncWebServerRequest *request);
byte cacheInvalidate       = 0;       // used to invalidate browser cache when switching from regular to simplified UI

// void setStaticContentCacheHeaders(AsyncWebServerResponse *response); //old
void setStaticContentCacheHeaders(AsyncWebServerResponse *response, int code = 200, uint16_t eTagSuffix = 0); // new
void generateEtag(char *etag, uint16_t eTagSuffix = 0);

bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest *request, int code, uint16_t eTagSuffix = 0);

bool isIp(String str);



    
};

#endif
#endif

