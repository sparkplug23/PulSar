#ifndef _mPinViewer_H_
#define _mPinViewer_H_

#define D_UNIQUE_MODULE_CORE_PINVIEWER_ID 2035

#include "1_TaskerManager/mTaskerManager.h"

#if defined(USE_MODULE_CORE_PINVIEWER) && defined(USE_MODULE_NETWORK_WEBSERVER)

#include "1_TaskerManager/mTaskerInterface.h"

#include <ESPAsyncWebServer.h>

#ifndef PINVIEWER_MAX_GPIO_PINS
  #ifdef MAX_GPIO_PIN
    #define PINVIEWER_MAX_GPIO_PINS MAX_GPIO_PIN
  #elif defined(ESP8266)
    #define PINVIEWER_MAX_GPIO_PINS 17
  #else
    #define PINVIEWER_MAX_GPIO_PINS 48
  #endif
#endif

#ifndef PINVIEWER_DEFAULT_SAMPLING_MS
  #define PINVIEWER_DEFAULT_SAMPLING_MS 250
#endif

#ifndef PINVIEWER_MIN_SAMPLING_MS
  #define PINVIEWER_MIN_SAMPLING_MS 50
#endif

#ifndef PINVIEWER_HEAP_REPORT_MS
  #define PINVIEWER_HEAP_REPORT_MS 1000
#endif

#ifndef PINVIEWER_KEEPALIVE_MS
  #define PINVIEWER_KEEPALIVE_MS 10000
#endif

#include "2_CoreSystem/35_PinViewer/Web/Gen/generated_web.h"

class mPinViewer :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/

    mPinViewer(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init(void);
    void Init(void);

    static constexpr const char* PM_MODULE_CORE_PINVIEWER_CTR = D_MODULE__CORE__PINVIEWER__CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_PINVIEWER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_PINVIEWER_ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    }module_state;


    /************************************************************************************************
     * SECTION: DATA_RUNTIME
     ************************************************************************************************/

    struct SETTINGS
    {
      bool enabled = true;
      bool web_enabled = true;
      bool sse_enabled = true;
      bool publish_unchanged = false;
      bool publish_heap = true;
      bool publish_all_on_connect = true;
      uint16_t sampling_ms = PINVIEWER_DEFAULT_SAMPLING_MS;
    }settings;

    struct RUNTIME
    {
      bool web_attached = false;
      bool sse_attached = false;
      bool force_publish_all = true;
      uint32_t last_sample_ms = 0;
      uint32_t last_heap_ms = 0;
      uint32_t last_keepalive_ms = 0;
      uint32_t publish_count = 0;
      uint32_t connected_clients = 0;
      int8_t last_pin_state[PINVIEWER_MAX_GPIO_PINS];
      bool last_pin_locked[PINVIEWER_MAX_GPIO_PINS];
      bool last_pin_conflict[PINVIEWER_MAX_GPIO_PINS];
    }runtime;

    enum PinViewerState : int8_t
    {
      PINVIEWER_STATE_UNKNOWN = -2,
      PINVIEWER_STATE_INVALID = -1,
      PINVIEWER_STATE_LOW     = 0,
      PINVIEWER_STATE_HIGH    = 1
    };

    enum PinViewerDirection : uint8_t
    {
      PINVIEWER_DIRECTION_UNKNOWN       = 0,
      PINVIEWER_DIRECTION_INPUT         = 1,
      PINVIEWER_DIRECTION_OUTPUT        = 2,
      PINVIEWER_DIRECTION_BIDIRECTIONAL = 3,
      PINVIEWER_DIRECTION_ALTERNATE     = 4
    };

    enum PinViewerShareMode : uint8_t
    {
      PINVIEWER_SHARE_UNKNOWN   = 0,
      PINVIEWER_SHARE_EXCLUSIVE = 1,
      PINVIEWER_SHARE_SHARED    = 2,
      PINVIEWER_SHARE_RESERVED  = 3,
      PINVIEWER_SHARE_INTERNAL  = 4
    };

    struct PinViewerPinMeta
    {
      uint8_t pin = 0;
      bool valid = false;
      bool usable = false;
      bool used = false;
      bool locked = false;
      bool reserved = false;
      bool conflict = false;
      uint16_t function_id = 0;
      uint16_t owner_id = 0;
      uint8_t direction_id = PINVIEWER_DIRECTION_UNKNOWN;
      uint8_t share_mode_id = PINVIEWER_SHARE_UNKNOWN;
      uint8_t user_count = 0;
      uint8_t group_id = 0;
      int8_t state = PINVIEWER_STATE_UNKNOWN;
    };


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void ResetRuntime(void);
    void Web_AddHandlers(void);
    void SSE_Init(void);

    void UpdateIfDue(void);
    void SampleAndPublish(bool force_all = false);
    void PublishSystemTelemetry(bool force = false);
    void PublishKeepAlive(bool force = false);

    bool BuildGPIOStateJSON(String& json, bool force_all);
    bool BuildPinListJSON(String& json);
    bool BuildGroupListJSON(String& json);
    bool BuildEspInfoJSON(String& json);
    bool BuildSettingsJSON(String& json);

    void AppendPinJSON(String& json, uint8_t pin, bool include_state, bool& first);
    void AppendGroupJSON(String& json, uint8_t group_id, bool& first);
    void AppendJSONString(String& json, const char* value);

    PinViewerPinMeta GetPinMeta(uint8_t pin);
    int8_t ReadPinState(uint8_t pin);

    bool IsValidPin(uint8_t real_pin);
    bool IsUsablePin(uint8_t real_pin);
    bool IsReservedPin(uint8_t real_pin);

    uint8_t GetDirectionID(uint8_t real_pin);
    uint8_t GetShareModeID(uint8_t real_pin);
    uint8_t GetUserCount(uint8_t real_pin);

    uint8_t Group_GetCount(void);
    uint8_t Group_GetID_ByIndex(uint8_t group_index);
    const char* Group_GetName_ByID(uint8_t group_id, char* buffer, uint8_t buflen);
    const char* Group_GetColour_ByID(uint8_t group_id, char* buffer, uint8_t buflen);
    uint8_t Group_GetAddressCount(uint8_t group_id);
    bool Group_GetAddress_ByIndex(uint8_t group_id, uint8_t address_index, uint8_t* address, char* module_name, uint8_t module_name_len);

    const char* GetDirectionName(uint8_t direction_id);
    const char* GetShareModeName(uint8_t share_mode_id);


    /************************************************************************************************
     * SECTION: Web Handlers
     ************************************************************************************************/

    void Web_SendRoot(AsyncWebServerRequest* request);
    void Web_SendRelease(AsyncWebServerRequest* request);
    void Web_SendSampling(AsyncWebServerRequest* request);
    void Web_SendEspInfo(AsyncWebServerRequest* request);
    void Web_SendPinModes(AsyncWebServerRequest* request);
    void Web_SendPinFunctions(AsyncWebServerRequest* request);
    void Web_SendPinAllocations(AsyncWebServerRequest* request);
    void Web_SendGroups(AsyncWebServerRequest* request);

    void SSE_OnConnect(AsyncEventSourceClient* client);

    AsyncEventSource* events = nullptr;


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);


    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);


    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mPinViewer>*> telemetry_list;
    struct telemetry_handler<mPinViewer> telemetry_settings;
    struct telemetry_handler<mPinViewer> telemetry_state_ifchanged;
    struct telemetry_handler<mPinViewer> telemetry_state_teleperiod;
    #endif
};


#endif // USE_MODULE_CORE_PINVIEWER
#endif // _mPinViewer_H_