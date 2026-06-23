#ifndef _NETWORK_INTERFACE_H
#define _NETWORK_INTERFACE_H

#define D_UNIQUE_MODULE__NETWORK_INTERFACE__ID  3000 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_INTERFACE

#include "1_TaskerManager/mTaskerInterface.h"

class mInterfaceNetwork :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mInterfaceNetwork(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};
    
    static constexpr const char*    PM_MODULE__NETWORK_INTERFACE__CTR = D_MODULE__NETWORK_INTERFACE__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_INTERFACE__CTR; }
    uint16_t GetModuleUniqueID(){   return D_UNIQUE_MODULE__NETWORK_INTERFACE__ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Config(void);

    static constexpr const char* NETWORK_CONFIG_FILE_PATH = "/config__network.json";
    static constexpr uint16_t NETWORK_CONFIG_MAX_BYTES    = 8192;

    bool NetworkConfig_LoadFileAndParse(void);
    bool NetworkConfig_ParseJSONString(const String& json_string);

    #ifdef USE_NETWORK_TEMPLATE
    String NetworkConfig_LoadTemplate(void);
    bool   NetworkConfig_CreateFileFromTemplateAndParse(void);
    #endif

    void NetworkConfig_DebugPrint(const char* title, const String& data);
    void NetworkConfig_ApplyDefineDefaults(void);

    struct InterfaceBackoffConfig
    {
      uint16_t fast_step_secs = 5;
      uint16_t periodic_step_secs = 60;
      uint16_t max_secs = 600;
    };

    struct InterfaceTransportConfig
    {
      char id[16] = {0};
      char type[16] = {0};

      uint8_t enabled = 0;
      uint8_t priority = 0;

      InterfaceBackoffConfig backoff;
    };

    struct InterfaceTransportRuntime
    {
      InterfaceTransportConfig config;

      uint8_t registered = 0;
      uint8_t connected = 0;
      uint8_t usable = 0;

      Client* client = nullptr;

      uint32_t last_seen_ms = 0;
      uint32_t last_changed_ms = 0;
    };

    struct InterfacePolicyConfig
    {
      char prefer_order[4][16] = {{0}};
      uint8_t prefer_order_count = 0;

      uint8_t allow_multiple_active = 1;
      uint8_t block_remote_mqtt_when_local_available = 1;
    };

    struct InterfaceConfig
    {
      InterfacePolicyConfig policy;

      InterfaceTransportRuntime transports[4];
      uint8_t transport_count = 0;
    };

    InterfaceConfig config;

    void Interface_Config_Clear(void);
    InterfaceTransportRuntime* Interface_GetTransportById(const char* id);


    typedef struct ConnectionState
    {   
      bool isvalid = false;
      uint8_t connection_priority = 0;
      uint32_t uptime_seconds = 0;
      uint32_t downtime_seconds = 0;
    }
    ConnectionState_t;


    struct DATA
    {
      bool is_connected = false;
      ConnectionState_t wifi_state;
      ConnectionState_t ethernet_state;
      ConnectionState_t cellular_state;
      ConnectionState_t mqtt_state;
    }
    data;

    enum NETWORK_TYPE
    {
      NETWORK_TYPE_NONE = 0,
      NETWORK_TYPE_ETHERNET = 1,
      NETWORK_TYPE_WIFI = 2,
      NETWORK_TYPE_CELLULAR = 3,
      NETWORK_TYPE_ANY = 4
    };
    
    bool Connected(uint8_t type = NETWORK_TYPE_ANY);

    // ------------------------------------------------------------------
    // Network policy helpers (interface-level)
    // ------------------------------------------------------------------
    bool Network_HasLocalConnectivity(void);
    bool Network_HasExternalConnectivity(void);

    // True when device is "local only" (no external reachability)
    // i.e. block MQTT/NTP, but still allow local WebUI etc.
    bool Network_IsLocalOnly(void);

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: ConstructJSON
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mInterfaceNetwork>*> mqtthandler_list;
    struct handler<mInterfaceNetwork> mqtthandler_settings;
    struct handler<mInterfaceNetwork> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

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
// class mInterfaceNetwork  :
//   public mTaskerInterface
// {
//   public:
//     mInterfaceNetwork()
//     {
//     };

//     // Cellular(uint8_t type, uint16_t start, uint8_t aw)
//     // : _type(255)
//     // {
//     //   _type = type;
//     // };
    
//     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    
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




#endif //  USE_MODULE_NETWORK_INTERFACE

#endif //  _NETWORK_INTERFACE_H