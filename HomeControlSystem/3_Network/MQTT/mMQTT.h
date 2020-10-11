#ifndef _MMQTT
#define _MMQTT 0.7

#include <stdint.h>
#include "0_ConfigUser/mUserConfig.h"

// Using MQTT to hold latest version of firmware
// Topic = group_all/
// Payload = {"latest":{"string":"0.52.1.2",number:0,"parts":{"major":0,"minor":52,"system":5,"module":10}},"lowest":{"string":"0.49.0.0",number:0,"parts":{"major":0,"minor":49,"system":3,"module":4}}}

// Void Arg Functions
#define CALL_MEMBER_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))
// (uint8_t,uint8_t) Arg Function calls
//#define CALL_MEMBER_FUNCTION_WITH_ARG(object,ptrToMember,X,Y)  ((object).*(ptrToMember)(X,Y))

// From before removing isserviced
// RAM:   [=====     ]  47.5% (used 38916 bytes from 81920 bytes)
// Flash: [=====     ]  47.8% (used 489856 bytes from 1023984 bytes)

#define DEFAULT_MQTT_SYSTEM_BOOT_RATE_SECS 1
#define DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS 120
#define DEFAULT_MQTT_SENSOR_MINIMAL_RATE_SECS 10
#define DEFAULT_MQTT_DRIVER_MINIMAL_RATE_SECS 10

// global enum for types of mqtt handlers, this can be asked for as a system command
// Note: "MQTT_HANDLER_LENGTH_ID" is used in module/classes as the start position/index for specialised handlers
enum MQTT_HANDLER_IDS{
  // Ability to ask for everything
  MQTT_HANDLER_ALL_ID = 0,
  // Core/shared handlers
  MQTT_HANDLER_SETTINGS_ID,
  MQTT_HANDLER_SENSOR_IFCHANGED_ID,
  MQTT_HANDLER_SENSOR_TELEPERIOD_ID,   
  MQTT_HANDLER_OUTPUT_IFCHANGED_ID, //drivers primary output
  MQTT_HANDLER_OUTPUT_TELEPERIOD_ID,    
  // Count of total handlers and starting point for other modules
  MQTT_HANDLER_LENGTH_ID 
};

// global enum for types of mqtt handlers, this can be asked for as a system command
//System has no ifchanged/teleperiod extra level
enum TOPIC_TYPE_IDS{
  MQTT_TOPIC_TYPE_SYSTEM_ID=0, 
  MQTT_TOPIC_TYPE_IFCHANGED_ID,
  MQTT_TOPIC_TYPE_ROC1M_ID,
  MQTT_TOPIC_TYPE_ROC10M_ID,
  MQTT_TOPIC_TYPE_SENSOR_TELEPERIOD_ID,   // 1 second
  MQTT_TOPIC_TYPE_SETTINGS_TELEPERIOD_ID, //show settings, but 1 hour increments
  MQTT_TOPIC_TYPE_TELEPERIOD_ID,
  
  MQTT_TOPIC_TYPE_LENGTH_ID
};

enum MQTT_FREQUENCY_REDUCTION_LEVEL_IDS{
  MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID = 0,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_1_MINUTES_ID = 1,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID = 2,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_60_MINUTES_ID = 3,
  MQTT_FREQUENCY_REDUCTION_LEVEL_LENGTH_ID = 4
};

#define MQTT_TOPIC_TYPE_IFCHANGED_CTR "ifchanged" //send single items that have changed within 1 second
#define MQTT_TOPIC_TYPE_TELEPERIOD_CTR "tele"     //send blocks of information, with another parameter controlling the size of the block (important, all)

// Put all shared ones here, eg "settings" and "sensor", with specific inside module.h when needed
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR)       "health";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR)     "settings";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR)          "log";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR)     "firmware";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR)       "memory";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR)      "network";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR)         "mqtt";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR)         "time";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEVICES_CTR)      "devices";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR)       "reboot";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR) "reboot/event";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR)           "debug/pins";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR)       "debug/template";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR) "debug/moduleinterface";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR)  "debug/minimal";    

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR)
"sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR)
"power";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR) "state";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIMED_CTR) "timed";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR) "debug";

// enum MQTT_FREQUENCY_REDUNCTION_LEVEL_IDS{
//   MQTT_FREQUENCY_REDUNCTION_LEVEL_NONE_ID=0,
//   MQTT_FREQUENCY_REDUNCTION_LEVEL_LOW_ID,     //1 minute
//   MQTT_FREQUENCY_REDUNCTION_LEVEL_MEDIUM_ID,  //10 minutes
//   MQTT_FREQUENCY_REDUNCTION_LEVEL_HIGH_ID     //60 minutes
// };

typedef union {
  uint8_t data;
  struct {
    uint8_t PeriodicEnabled : 1; 
    uint8_t SendNow         : 1;
    uint8_t FrequencyRedunctionLevel  : 2;     // 0= no redunction, maintain level, 1 = reduce after 1 minute, 1= 10 minutes, 2= 60 minutes
    
    uint8_t reserved : 4; 
  };
}Handler_Flags;

template <typename Class>
struct handler {
  uint32_t      tSavedLastSent = 0;
  uint16_t      tRateSecs = 1;
  // uint8_t       flags.PeriodicEnabled = true;
  // uint8_t       flags.SendNow = true;
  uint8_t       json_level = 0;
  uint8_t       topic_type = 0;
  const char*   postfix_topic;
  uint8_t       module_id = 0;
  uint8_t       handler_id = 0;
  Handler_Flags flags;
  uint8_t       (Class::*ConstructJSON_function)(uint8_t json_level); // member-function to sender with one args
};

#include "mPubSubClient.h"
class mPubSubClient;

#include "0_ConfigUser/mUserConfig.h"
#include "2_CoreSystem/Support/mSupport.h"
class mSupport;

#include <ArduinoJson.h>
#ifdef ESP32
  #include <WiFi.h> //esp32
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>

#include "1_TaskerManager/mInterfaceController.h"

#define WILLQOS_CTR 2
#define WILLRETAIN_CTR true
#define WILLMESSAGE_ONDISCONNECT_CTR "{\"LWT\":\"Offline\"}"
#define WILLMESSAGE_ONCONNECT_CTR "{\"LWT\":\"Online\"}"


class mMQTT{

  public:
    mMQTT(){};
    void init(void);
    int8_t Tasker(uint8_t function);

    bool flag_uptime_reached_reduce_frequency = false;

    // char lwt_message_ondisconnect_ctr[50];

    void DiscoverServer(void);

    struct CONNECTION_MAINTAINER_PARAMETERS{
      uint32_t tSavedLoop = millis();
      uint32_t tSavedReconnectAttempt = millis()+30000;
      uint32_t rSavedReconnectAttempt = 0;
      uint8_t cConnectionAttempts = 0; 
    }connection_maintainer;


    // wasteful
    struct SETTINGS{
      char lwt_topic[30];
      char client_name[40]; 
      char hostname_ctr[20];
      char prefixtopic[50]; // "<devicename>/User extras?"
    }settings;

    // Special formatted sending functions (pubsub functions to be moved in here)
    void publish_ft( const char* module_name,uint8_t topic_type_id, const char* topic_postfix, const char* payload, uint8_t retain_flag);
    void publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag);
    boolean ppublish(const char* topic, const char* payload, boolean retained);

    void setprefixtopic(const char* _prefixtopic);

    boolean psubscribe(const char* topic);

    void MQTTHandler_Send_Formatted(uint8_t topic_type, uint8_t module_id, const char* postfix_topic_ctr);

    WiFiClient client;
    mPubSubClient* pubsub;
    void connect(void);
    
    void HandleMqttConfiguration(void);
    void MqttSaveSettings(void);

    void parse_JSONCommand();
    void parsesub_MQTTSettingsCommand();

    void Send_Prefixed_P(const char* topic, PGM_P formatP, ...);

    /****
     * "class-less" Pointer Member function that takes the struct handler which contains if/when a mqtt payload should
     * be sent. If a payload should sent, the formatted publish function is called.
     * */
    template<typename T>
    void MQTTHandler_Command(T& class_ptr, uint8_t class_id, handler<T>* handler_ptr)
    {
      if(handler_ptr == nullptr){
        return;
      }
      

    #ifdef ENABLE_ADVANCED_DEBUGGING
      #ifndef DISABLE_SERIAL_LOGGING
      // AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_TEST " MQQTHandler_System_Sender %d"),status_id);
      Serial.printf("MQTTHandler_Command::postfix_topic=%s %d\n\r",handler_ptr->postfix_topic, class_id);
    #endif
    #endif

      if(handler_ptr->flags.PeriodicEnabled){
        if(abs(millis()-handler_ptr->tSavedLastSent)>=handler_ptr->tRateSecs*1000){ handler_ptr->tSavedLastSent=millis();
          handler_ptr->flags.SendNow = true;
          handler_ptr->flags.SendNow = true;
          //handler_ptr->flags.FrequencyRedunctionLevel = 1;
          if(flag_uptime_reached_reduce_frequency && handler_ptr->flags.FrequencyRedunctionLevel){
            // handler_ptr->flags.FrequencyRedunctionLevel = 0; // reset to don't change
            
            //Serial.println("flag_uptime_reached_reduce_frequency");
            
            handler_ptr->tRateSecs = handler_ptr->tRateSecs < 120 ? 120 : handler_ptr->tRateSecs; //only reduce if new rate is longer
          }
        }
      }
      if(handler_ptr->flags.SendNow || handler_ptr->flags.SendNow){ handler_ptr->flags.SendNow = false;    handler_ptr->flags.SendNow = false;
        handler_ptr->tSavedLastSent = millis();    
        // Generate the JSON payload, to the level of detail needed.
        //CALL_MEMBER_FUNCTION(class_ptr,handler_ptr->function_sender)(handler_ptr->json_level);
        //CALL_MEMBER_FUNCTION_WITH_ARG(class_ptr,handler_ptr->function_sender,handler_ptr->topic_type,handler_ptr->json_level);

        // use return flag for if mqtt needs sent, useful with ifchanged
        uint8_t fSendPayload = (class_ptr.*handler_ptr->ConstructJSON_function)(handler_ptr->json_level); //fSendPayload as uint8_t is what I return, cant be length > 255

        // Serial.printf("payload_length=%d\n\r",payload_length);

        // Send MQTT payload with structured output
        if(fSendPayload){ MQTTHandler_Send_Formatted(handler_ptr->topic_type,class_id,handler_ptr->postfix_topic); }
      }
      // Check if this needs reducing
      if(handler_ptr->flags.FrequencyRedunctionLevel){

      }




    };


    // Functions to call the ConstructJson held within telemetry for the system.
    uint8_t ConstructJSON_Health(uint8_t json_level);
    uint8_t ConstructJSON_Settings(uint8_t json_level);
    // uint8_t ConstructJSON_Parameters(uint8_t json_level);
    uint8_t ConstructJSON_Firmware(uint8_t json_level);
    uint8_t ConstructJSON_Log(uint8_t json_level);
    uint8_t ConstructJSON_Memory(uint8_t json_level);
    uint8_t ConstructJSON_Network(uint8_t json_level);
    uint8_t ConstructJSON_Time(uint8_t json_level);
    uint8_t ConstructJSON_Reboot(uint8_t json_level);
    uint8_t ConstructJSON_Reboot_Event(uint8_t json_level);
    uint8_t ConstructJSON_MQTT(uint8_t json_level);
    uint8_t ConstructJSON_Devices(uint8_t json_level);
    #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
      uint8_t ConstructJSON_Debug_Pins(uint8_t json_level);
      uint8_t ConstructJSON_Debug_Template(uint8_t json_level);
      uint8_t ConstructJSON_Debug_ModuleInterface(uint8_t json_level);
      uint8_t ConstructJSON_Debug_Minimal(uint8_t json_level);
    #endif

    //use new syntax
    enum STATUS_SYSTEM_IDS{
      MQTT_HANDLER_SYSTEM_ALL_ID = 0,
      MQTT_HANDLER_SYSTEM_HEALTH_ID, // To align with "status #" type commands
      MQTT_HANDLER_SYSTEM_SETTINGS_ID,
      MQTT_HANDLER_SYSTEM_PARAMETERS_ID,
      MQTT_HANDLER_SYSTEM_LOG_ID,
      MQTT_HANDLER_SYSTEM_FIRMWARE_ID,
      MQTT_HANDLER_SYSTEM_MEMORY_ID,
      MQTT_HANDLER_SYSTEM_NETWORK_ID,
      MQTT_HANDLER_SYSTEM_MQTT_ID,
      MQTT_HANDLER_SYSTEM_TIME_ID,
      MQTT_HANDLER_SYSTEM_DEVICES_ID,
      MQTT_HANDLER_SYSTEM_REBOOT_ID,
      MQTT_HANDLER_SYSTEM_REBOOT_EVENT_ID,
      #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
        MQTT_HANDLER_SYSTEM_DEBUG_PINS_ID,
        MQTT_HANDLER_SYSTEM_DEBUG_TEMPLATE_ID,
        MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID,
        MQTT_HANDLER_SYSTEM_DEBUG_MINIMAL_ID,
      #endif
      MQTT_HANDLER_SYSTEM_SYSTEM_LENGTH_ID // last holds length      
    };

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    // Root call
    void MQQTHandler_System_Sender(uint8_t status_id = MQTT_HANDLER_SYSTEM_ALL_ID);

    handler<mMQTT>* mqtthandler_ptr;

    // move into progmem!

    // const char* postfix_topic_health = "health";
    handler<mMQTT> mqtthandler_health;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";    
    handler<mMQTT> mqtthandler_settings; //ie modes, on, off, states
    // const char* postfix_topic_log = "log";        
    handler<mMQTT> mqtthandler_log;
    // const char* postfix_topic_firmware = "firmware";        
    handler<mMQTT> mqtthandler_firmware;
    // const char* postfix_topic_memory = "memory";    
    handler<mMQTT> mqtthandler_memory;
    // const char* postfix_topic_network = "network";    
    handler<mMQTT> mqtthandler_network;
    // const char* postfix_topic_mqtt = "mqtt";    
    handler<mMQTT> mqtthandler_mqtt;
    // const char* postfix_topic_time = "time";    
    handler<mMQTT> mqtthandler_time;
    // const char* postfix_topic_devices = "devices";    
    handler<mMQTT> mqtthandler_devices; // appended all ACTIVE sensors from all sensor modules
    // const char* postfix_topic_reboot = "reboot";    
    handler<mMQTT> mqtthandler_reboot;
    // const char* postfix_topic_reboot_event = "reboot/event";    
    handler<mMQTT> mqtthandler_reboot_event;
    #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
      // const char* postfix_topic_debug_pins = "debug/pins";    
      handler<mMQTT> mqtthandler_debug_pins;
      // const char* postfix_topic_debug_template = "debug/template";    
      handler<mMQTT> mqtthandler_debug_template;
      // const char* postfix_topic_debug_moduleinterface = "debug/moduleinterface";    
      handler<mMQTT> mqtthandler_debug_moduleinterface;
      // const char* postfix_topic_debug_minimal = "debug/minimal";    
      handler<mMQTT> mqtthandler_debug_minimal;
    #endif

};
#endif
