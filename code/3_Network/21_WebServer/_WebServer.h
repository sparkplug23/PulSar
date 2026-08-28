#ifndef _MWEBSERVER_H
#define _MWEBSERVER_H

#define D_UNIQUE_MODULE_NETWORK_WEBSERVER_ID  3021 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

#include <Arduino.h>

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/01_Settings/mSettings.h"
#include "2_CoreSystem/08_Logging/mLogging.h"

#define Network WiFi

#include "FileEditor.h"

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

#include "mWebUrlTracker.h" // Must be included so #else blanks are inserted

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
// #define D_BUTTO

#ifndef D_CAPTIVE_PORTAL_URL_REDIRECT_PATH
#define D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/settings2/welcome"
// #define D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/settings2/wifi"
#endif


#include "3_Network/21_WebServer/Webpages/Generated/html_settings.h"
#include "3_Network/21_WebServer/Webpages/Generated/html_other.h"
#include "3_Network/21_WebServer/Webpages/Generated/debug_pages.h"
#include "3_Network/21_WebServer/Webpages/Generated/root_main.h"
#include "3_Network/21_WebServer/Webpages/Generated/submodule_assets.h"
#ifdef ESP32
  #include "3_Network/21_WebServer/Webpages/Generated/pages_console_esp32.h"
#else
  #include "3_Network/21_WebServer/Webpages/Generated/pages_console_esp8266.h"
#endif
#ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
#include "3_Network/21_WebServer/Webpages/Generated/pages_url_debugs.h"
#endif


#define JSON_PATH_WEB_STATE      1
#define JSON_PATH_WEB_INFO       2
#define JSON_PATH_WEB_STATE_INFO 3
#define JSON_PATH_WEB_NODES      4
#define JSON_PATH_WEB_PALETTES   5
#define JSON_PATH_WEB_FXDATA     6
#define JSON_PATH_WEB_NETWORKS   7
#define JSON_PATH_WEB_EFFECTS    8

    enum WebSettingsSubPage : uint8_t
    {
      MENU = 0,
      NETWORK,
      HARDWARE,
      SYSTEM,
      MODULES,
      STORAGE,
      LOGGING,
      SECURITY,

      // these are likely to be removed
      WIFI,
      UPDATE,
      TIME,

      PINREQ  = 252,
      CSS     = 253,
      JS      = 254,
      WELCOME = 255
    };


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

    struct WebApplicationURL
    {
      uint16_t module_id = 0;
      String url;
      String friendly_name;
    };
    std::vector<WebApplicationURL> application_urls;
    void AddURLasApplication(uint16_t module_id, const char* url, const char* friendly_name = nullptr);
    void AddURLasApplication(uint16_t module_id, const String& url, const char* friendly_name = nullptr);
    void HandleAPI_URLApplications(AsyncWebServerRequest* request);

    #ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
    enum class WebUIControlType : uint8_t
    {
      Toggle = 0,
      Momentary,
      TestSwitch,
      ButtonRow,
      Value,
      Indicator
    };


    struct WebUIContext
    {
      Print* response = nullptr;

      bool first_module = true;
      bool first_control = true;
      bool module_open = false;
      bool button_row_open = false;
      bool first_option = true;
    };

    WebUIContext webui;


    /**
     * Generic WebUI output helpers.
     */
    bool WebUI_Begin(Print* response);
    void WebUI_PrintJSONString(Print* response, const char* str);
    void WebUI_End();

    void WebUI_Module_Start(uint16_t module_id, const char* module_name);
    void WebUI_Module_End();

    void WebUI_AddToggle(const char* command, uint8_t device_id, const char* name, bool state);
    void WebUI_AddMomentary(const char* command, uint8_t device_id, const char* name, bool state);
    void WebUI_AddTestSwitch(const char* command, uint8_t device_id, const char* name, bool physical_state);

    void WebUI_AddButtonRow_Start(const char* command, uint8_t device_id, const char* name, const char* description = nullptr, int32_t selected_value = -1);
    void WebUI_AddButtonRow_Option(const char* name, int32_t value);
    void WebUI_AddButtonRow_End();

    void WebUI_AddValue(uint8_t device_id, const char* name, const char* value, const char* units = nullptr);
    void WebUI_AddValue(uint8_t device_id, const char* name, int32_t value, const char* units = nullptr);
    void WebUI_AddValue(uint8_t device_id, const char* name, float value, const char* units = nullptr, uint8_t precision = 2);

    void WebUI_AddIndicator(uint8_t device_id, const char* name, bool state);

    #endif // ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
    #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

    AsyncWebSocket* websocket_pages = nullptr;

    void wsEventPages(
      AsyncWebSocket *server,
      AsyncWebSocketClient *client,
      AwsEventType type,
      void *arg,
      uint8_t *data,
      size_t len
    );

    bool WebSocket_SendText(AsyncWebSocketClient* client, const char* data, size_t len);
    bool WebSocket_SendWrappedJSON(AsyncWebSocketClient* client, const char* key, const char* json, size_t json_len);
    bool WebSocket_SendSensors(AsyncWebSocketClient* client);
    bool WebSocket_SendEnergy(AsyncWebSocketClient* client);

    #ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
    bool WebSocket_SendControls(AsyncWebSocketClient* client);
    #endif

    #endif

// server library objects
AsyncWebServer* server = nullptr;


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


  

  /**************************************************************************************************
   * Generic Telemetry API
   **************************************************************************************************/

  enum class TelemetryAPIRequestMode : uint8_t
  {
    None = 0,
    Catalogue,
    Topic
  };

  struct TelemetryAPIRequest
  {
    TelemetryAPIRequestMode mode = TelemetryAPIRequestMode::None;
    char requested_key[128] = {0};
    AsyncResponseStream* catalogue_response = nullptr;
    bool catalogue_first = true;
    bool found = false;
    bool buffer_busy = false;
    uint16_t rate = 0;
    String packet;
  };

  TelemetryAPIRequest telemetry_api_request;

  uint32_t telemetry_api_json_last_used_ms = 0;
  static constexpr uint32_t TELEMETRY_API_JSON_BACKOFF_MS = 350;

  void HandleAPI_Telemetry(AsyncWebServerRequest* request);
  void TelemetryAPI_PrintJSONString(AsyncResponseStream* response, const char* str);
  bool TelemetryAPI_Construct_Begin(const char* full_key, uint16_t rate);
  void TelemetryAPI_Construct_End();

  template<typename T>
  void Telemetry_Sender(std::vector<telemetry_handler<T>*>& telemetry_list, T& class_ptr)
  {
    if(telemetry_api_request.mode == TelemetryAPIRequestMode::None) return;

    char full_key[160];

    for(auto* handle : telemetry_list)
    {
      if(!handle || !handle->key || !handle->ConstructJSON_function) continue;

      snprintf_P(full_key, sizeof(full_key), PSTR("%S/%S"), class_ptr.GetModuleName(), handle->key);

      if(telemetry_api_request.mode == TelemetryAPIRequestMode::Catalogue)
      {
        AsyncResponseStream* response = telemetry_api_request.catalogue_response;
        if(!response) return;

        if(!telemetry_api_request.catalogue_first) response->print(',');
        telemetry_api_request.catalogue_first = false;

        response->print(F("{\"topic\":\""));
        TelemetryAPI_PrintJSONString(response, full_key);
        response->printf_P(PSTR("\",\"rate\":%u}"), handle->tRateSecs);
        continue;
      }

      if(telemetry_api_request.mode == TelemetryAPIRequestMode::Topic)
      {
        if(strcmp(full_key, telemetry_api_request.requested_key)) continue;
        if(!TelemetryAPI_Construct_Begin(full_key, handle->tRateSecs)) return;

        CALL_MEMBER_FUNCTION(class_ptr, handle->ConstructJSON_function)(handle->json_level, false);

        TelemetryAPI_Construct_End();
        return;
      }
    }
  }


  /**************************************************************************************************
   * Telemetry Debug Page
   **************************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE_WEB__TELEMETRY
  void HandlePage_DebugTelemetry(AsyncWebServerRequest* request);
  #endif


  /**************************************************************************************************
   * Tasker Metrics Debug Page / API
   **************************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  void HandlePage_DebugTaskerMetrics(AsyncWebServerRequest* request);
  void HandleAPI_DebugTaskerMetrics(AsyncWebServerRequest* request);
  #endif






void serveSettingsJS(AsyncWebServerRequest* request);
void serveJson(AsyncWebServerRequest* request);

// static void extractPin(Print& settingsScript, JsonObject &obj, const char *key);
void XML_response(Print& dest);
// static void fillUMPins(Print& settingsScript, JsonObject &mods);
void appendGPIOinfo(Print& settingsScript);
void getSettingsJS(byte subPage, Print& settingsScript);


size_t printSetFormCheckbox(Print& settingsScript, const char* key, int val);
size_t printSetFormValue(Print& settingsScript, const char* key, int val);
size_t printSetFormIndex(Print& settingsScript, const char* key, int index);
size_t printSetFormValue(Print& settingsScript, const char* key, const char* val);
size_t printSetClassElementHTML(Print& settingsScript, const char* key, const int index, const char* val);
size_t printSetFormInput(Print& settingsScript, const char* key, const char* selector, int value) ;
// size_t printSetElementStyle(Print& settingsScript, const char* element_id, const char* css_prop, const char* css_val);
// size_t printToggleElementClass(Print& settingsScript,  const char* element_id,  const char* class_name,  bool enable);


size_t printTableSetCell(Print& s, const char* table_id, uint16_t row, uint8_t col, const char* val);
size_t printTableAddRow(Print& s, const char* table_id, uint16_t row, const char* c0, const char* c1, const char* c2);
size_t printTableClear(Print& s, const char* table_id);

size_t printTableSetValue(Print& s, const char* table_id, uint16_t row, const char* val);
size_t printTableSetNotes(Print& s, const char* table_id, uint16_t row, const char* val);

size_t printSetElementHTML(
  Print& settingsScript,
  const char* element_id,
  const char* val
) ;


    void SettingsPages__ParseForm(AsyncWebServerRequest *request, byte subPage);


#ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
void HandlePage_UrlList(AsyncWebServerRequest *request);
void HandlePage_UrlList_JSON(AsyncWebServerRequest *request);
#endif



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

