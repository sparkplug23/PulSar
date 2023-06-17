#ifndef _NETWORK_INTERFACE_H
#define _NETWORK_INTERFACE_H

#define D_UNIQUE_MODULE__NETWORK_INTERFACE__ID  ((3*1000)+00)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_INTERFACE

#include "1_TaskerManager/mTaskerInterface.h"

class mInterfaceNetwork :
  public mTaskerInterface
{

  private:
  public:
    mInterfaceNetwork(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char*                     PM_MODULE__NETWORK_INTERFACE__CTR;
    static const char*                     PM_MODULE__NETWORK_INTERFACE__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_INTERFACE__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE__NETWORK_INTERFACE__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){   return D_UNIQUE_MODULE__NETWORK_INTERFACE__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){ return sizeof(mInterfaceNetwork);    };
    #endif
    void Pre_Init();
    void Init(void);
    void EveryLoop();
    
    struct SETTINGS
    {
      uint8_t fEnableSensor = false;
    }
    settings;


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

    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    #ifdef USE_MODULE_NETWORK_MQTT
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mInterfaceNetwork>  mqtthandler_settings_teleperiod;
    struct handler<mInterfaceNetwork>  mqtthandler_state_ifchanged;

    struct handler<mInterfaceNetwork>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

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

//     #ifdef USE_DEBUG_CLASS_SIZE
//     uint16_t GetClassSize(){
//       return sizeof(mInterfaceNetwork);
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




#endif //  USE_MODULE_NETWORK_INTERFACE

#endif //  _NETWORK_INTERFACE_H