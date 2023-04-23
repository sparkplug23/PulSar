#ifndef _MMQTT
#define _MMQTT 0.7

#define D_UNIQUE_MODULE_NETWORK_MQTT_ID  ((3*1000)+10)

#include "2_CoreSystem/mFirmwareDefaults.h"

#ifdef USE_MODULE_NETWORK_MQTT
#include <stdint.h>

#include "2_CoreSystem/mGlobalMacros.h"

// Using MQTT to hold latest version of firmware
// Topic = group_all/
// Payload = {"latest":{"string":"0.52.1.2",number:0,"parts":{"major":0,"minor":52,"system":5,"module":10}},"lowest":{"string":"0.49.0.0",number:0,"parts":{"major":0,"minor":49,"system":3,"module":4}}}

// Void Arg Functions
#define CALL_MEMBER_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))
// (uint8_t,uint8_t) Arg Function calls

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

  MQTT_TOPIC_TYPE__DEBUG__ID,
  
  MQTT_TOPIC_TYPE_LENGTH_ID
};

enum MQTT_FREQUENCY_REDUCTION_LEVEL_IDS{
  MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID = 0, // leave it as original set
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
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR) "debug/stored_settings";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR)  "debug/minimal";    

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR) "sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR)   "power";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR)   "light/state";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR) "state";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIMED_CTR) "timed";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR) "debug";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR) "motion";

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
  uint8_t       json_level = 0;
  uint8_t       topic_type = 0;
  const char*   postfix_topic = nullptr;
  uint8_t       handler_id = 0;
  Handler_Flags flags = {0}; //#issue FrequencyRedunctionLevel
  uint8_t       (Class::*ConstructJSON_function)(uint8_t json_level, bool json_appending); // member-function to sender with two args. Extra "json_appending" will allow calling constructjsons directly and adding them to another without closing the main json object
};

#include "PubSubClient.h"
class mPubSubClient;

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/06_Support/mSupport.h"
class mSupport;

#ifdef ESP32
  #include <WiFi.h> //esp32
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>


#include "1_TaskerManager/mTaskerManager.h"

#include "1_TaskerManager/mTaskerInterface.h"
#define WILLQOS_CTR 2
#define WILLRETAIN_CTR true
#define WILLMESSAGE_ONDISCONNECT_CTR "{\"LWT\":\"Offline\"}"
#define WILLMESSAGE_ONCONNECT_CTR "{\"LWT\":\"Online\"}"


class mMQTT :
  public mTaskerInterface
{

  public:
    mMQTT(){};
    void init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mMQTT);
    };
    #endif

    bool flag_uptime_reached_reduce_frequency = false;

    static const char* PM_MODULE_NETWORK_MQTT_CTR;
    static const char* PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_MQTT_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_MQTT_ID; }

    // char lwt_message_ondisconnect_ctr[50];#
  
    void parse_JSONCommand(JsonParserObject obj);

    // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
    // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);

    uint8_t host_server_type = 0;


    void DiscoverServer(void);
    void Send_LWT_Online();


    enum CONNECTION_CLIENT_TYPE{
      CLIENT_TYPE_WIFI_ID = 0,
      CLIENT_TYPE_ETHERNET_ID,
      CLIENT_TYPE_CELLULAR_ID
    };

    struct CONNECTION_MAINTAINER_PARAMETERS
    {
      #ifdef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      uint8_t mqtt_client_type = CLIENT_TYPE_CELLULAR_ID; //default wifi, 1 ethernet, 2 cellular
      #else
      uint8_t mqtt_client_type = CLIENT_TYPE_WIFI_ID; //default wifi, 1 ethernet, 2 cellular
      #endif

      bool flag_start_reconnect = false;

      // #ifndef ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1
      // uint32_t tSavedLoop = millis();
      // #endif // ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1
      // uint32_t tSavedReconnectAttempt = millis()+30000;
      // uint32_t rSavedReconnectAttempt = 0;
      uint8_t cConnectionAttempts = 0; 
    }connection_maintainer;

    const char* state_ctr(void);
    void Load_New_Subscriptions_From_Function_Template();

    // wasteful
    struct SETTINGS{
    }settings; 
    struct MQTT {
      uint16_t connect_count = 0;            // MQTT re-connect count
      uint16_t retry_counter = 1;            // MQTT connection retry counter
      uint16_t downtime_counter = 0;
      uint8_t initial_connection_state = 2;  // MQTT connection messages state
      bool connected = false;                // MQTT virtual connection status
      bool allowed = false;                  // MQTT enabled and parameters valid
      bool mqtt_tls = false;                 // MQTT TLS is enabled
    } Mqtt; // to be settings

    void EveryLoop();
    void CheckConnection();
    bool MqttIsConnected();
    void MqttReconnect();    
    void MqttDisconnected(int state);
    void MqttConnected(void);
    void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len);
  
    // Special formatted sending functions (pubsub functions to be moved in here)
    void publish_ft( const char* module_name,uint8_t topic_type_id, const char* topic_postfix, const char* payload, uint8_t retain_flag);
    void publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag);
    boolean ppublish(const char* topic, const char* payload, boolean retained = false);
    boolean ppublish_device_name_prefix_P(const char* topic, const char* payload, boolean retained = false);

    void setprefixtopic(const char* _prefixtopic);

    boolean psubscribe(const char* topic);

    void MQTTHandler_Send_Formatted(uint8_t topic_type, uint8_t module_id, const char* postfix_topic_ctr);

    mPubSubClient* pubsub = nullptr;   
    
    void SetPubSubClient(Client* client);

    
    void parsesub_MQTTSettingsCommand();

    void Send_Prefixed_P(const char* topic, PGM_P formatP, ...);

    /**
     * Phase out
     * */
    template<typename T>
    void MQTTHandler_Command_Array_Group(
      T& class_ptr, uint8_t class_id,
      handler<T>** handler_ptr,  uint8_t* handler_id_list_ptr,
      uint8_t handler_ptr_length, uint8_t optional_desired_id = 0
    )
    {

      for(uint8_t id=0;id<handler_ptr_length;id++){    
        // if(handler_ptr[id]->handler_id == handler_id_list_ptr[id]){ 
          MQTTHandler_Command(class_ptr, class_id, handler_ptr[id]);
          //if(optional_desired_id == MQTT_HANDLER_ALL_ID){ break; } //stop if it was to only send this one// should be != ?
        // }
      }

    }

    /**
     * Special MQTT functions that manupulate the generic mqtthandler_list from any class
     * handler<mAnimatorLight> *mAnimatorLight::mqtthandler_list[5]
     * Future idea:
     * Issue: Unknown how to also send length or array, so for(auto&) type loops probably wont work
     * */
    // template<typename T>
    // void MQTTHandler_Flags_SendNow(handler<T>* mqtthandler_list_ptr)
    // {     
    //   for(auto& handle:mqtthandler_list_ptr){
    //     handle->flags.SendNow = true;
    //   }
    // }

    // Easy way to add to the counter
    #define D_MQTT_COMMAND_HANDLED_COUNT_INC data_buffer.isserviced++



// Choose auto x when you want to work with copies.
// Choose auto &x when you want to work with original items and may modify them.
// Choose auto const &x when you want to work with original items and will not modify them.

    /****
     * "class-less" Pointer Member function that takes the struct handler which contains if/when a mqtt payload should
     * be sent. If a payload should sent, the formatted publish function is called.
     * */

    /**
     * @brief Function will take the struct storing the mqtt handler options, and check the variables, sending if required
     * @param class_ptr Pointer to the instance of a class
     * @param class_id  Numerical (Enum) value of the class, which is used to acquire the mqtt topic
     * @param handler_ptr Pointer to the handler struct with timing and function to be called
     * @param optional_desired_id Optional handler_id, if set (>=0) then only matching handler IDs will be sent. Used for group triggering by a type.
     * @note  Optional desired_id will check if the handler id was set, and if it does not match, will return without servicing handler
     * */
    template<typename T>
    void MQTTHandler_Command(T& class_ptr, uint8_t class_id, handler<T>* handler_ptr, int8_t optional_desired_id = -1)
    {
      
      // Sanity check
      if(handler_ptr == nullptr)
      {
        return;
      }
      
      #ifdef ENABLE_ADVANCED_DEBUGGING
        #ifndef DISABLE_SERIAL_LOGGING
        // AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_TEST " MQQTHandler_System_Sender %d"),status_id);
        //Serial.printf("1MQTTHandler_Command::postfix_topic=%s %d\n\r",handler_ptr->postfix_topic, class_id);
        #endif
      #endif
      
      // // #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
      // if(class_id == 12)
      // {
      //   Serial.printf("MQTTHandler_Command::postfix_topic=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
      // }
      // // #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
      #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
      Serial.printf("MQTTHandler_Command::postfix_topic=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
      #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

      if(handler_ptr->flags.PeriodicEnabled){
        if(ABS_FUNCTION(millis()-handler_ptr->tSavedLastSent)>=handler_ptr->tRateSecs*1000){ 
          handler_ptr->tSavedLastSent=millis();
          handler_ptr->flags.SendNow = true;

          #ifndef ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

          //handler_ptr->flags.FrequencyRedunctionLevel = 1;
          if(flag_uptime_reached_reduce_frequency && (handler_ptr->flags.FrequencyRedunctionLevel > MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID)){
            // handler_ptr->flags.FrequencyRedunctionLevel = 0; // reset to don't change            
            //Serial.println("flag_uptime_reached_reduce_frequency");            
            handler_ptr->tRateSecs = handler_ptr->tRateSecs < 120 ? 120 : handler_ptr->tRateSecs; //only reduce if new rate is longer
          }

          #endif


        }
      }

      if(handler_ptr->flags.SendNow){ 
        handler_ptr->flags.SendNow = false;
        handler_ptr->tSavedLastSent = millis();    

        // Serial.printf("mqtthandler_sensor_ifchanged=%d\n\r",mqtthandler_list_ptr[1]->tRateSecs);
        // Generate the JSON payload, to the level of detail needed.
        //CALL_MEMBER_FUNCTION(class_ptr,handler_ptr->function_sender)(handler_ptr->json_level);
        //CALL_MEMBER_FUNCTION_WITH_ARG(class_ptr,handler_ptr->function_sender,handler_ptr->topic_type,handler_ptr->json_level);
          
        // use return flag for if mqtt needs sent, useful with ifchanged
        uint8_t fSendPayload = (class_ptr.*handler_ptr->ConstructJSON_function)(handler_ptr->json_level, true); //fSendPayload as uint8_t is what I return, cant be length > 255
         
        // if(fSendPayload > 1){ 
        //   // Serial.printf("fSendPayload \"%s\"\n\r", fSendPayload); Serial.flush();
        //   fSendPayload= 0; 
        // } // to fix when return was not used

        // Serial.printf("fSendPayload=%d\n\r",fSendPayload); Serial.flush();
        
        // Serial.printf("DEBUG HERE: ");
        // Serial.print(__FILE__);
        // Serial.println(__LINE__);
        // Serial.flush();

        // Send MQTT payload with structured output
        if(fSendPayload){ 
          // Serial.printf("fSendPayload SendNow postfix_topic=%s %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush();
          MQTTHandler_Send_Formatted(handler_ptr->topic_type,class_id,handler_ptr->postfix_topic); 
          // Serial.printf("ENDfSendPayload SendNow postfix_topic=%s %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush();
        }
        // Serial.printf("DEBUG HERE: ");
        // Serial.print(__FILE__);
        // Serial.println(__LINE__);
        // Serial.flush();
      }
      // Check if this needs reducing
      if(handler_ptr->flags.FrequencyRedunctionLevel){

      }

    };

};
#endif

#endif //  USE_MODULE_NETWORK_MQTT