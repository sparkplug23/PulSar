#ifndef _MMQTT
#define _MMQTT 0.7

#define D_UNIQUE_MODULE_NETWORK_MQTT_ID  ((3*1000)+10)

#include "2_CoreSystem/mFirmwareDefaults.h"

#include "0_ConfigUser/03_mFirmware_Secret__ActiveDevelopment.h"

#ifdef USE_MODULE_NETWORK_MQTT
#include <stdint.h>

#include "2_CoreSystem/mGlobalMacros.h"

// Void Arg Functions
#define CALL_MEMBER_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))

// Easy way to add to the counter
#define D_MQTT_COMMAND_HANDLED_COUNT_INC data_buffer.isserviced++

#define DEFAULT_MQTT_SYSTEM_BOOT_RATE_SECS 1
#define DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS 120
#define DEFAULT_MQTT_SENSOR_MINIMAL_RATE_SECS 10
#define DEFAULT_MQTT_DRIVER_MINIMAL_RATE_SECS 10


// #ifdef ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
#define CODE_BLOCK__MQTTHandler_AddWebURL_PayloadRequests()  \
  char uri_buffer[70] = {0};\
  snprintf(uri_buffer, sizeof(uri_buffer), "/mqtt/%s/%S", D_TOPIC_STATUS, GetModuleFriendlyName());\
  pCONT_web->server->on(uri_buffer, HTTP_GET,\
  [this](AsyncWebServerRequest *request)\
    {\
      char handle_url[100] = {0};       \
      for(auto& handle:mqtthandler_list)\
      {      \
        pCONT_mqtt->TopicFormatted( GetModuleFriendlyName(), handle->topic_type, handle->postfix_topic, handle_url, sizeof(handle_url)); \
        ALOG_INF(PSTR("handle_url \"%s\" -> \"%s\""), request->url().c_str(), handle_url);\
        const String& incoming_uri = request->url();\
        if(incoming_uri.indexOf(handle_url) > 0)\
        {        \
          uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);\
          ALOG_INF(PSTR("data_buffer=%s"), data_buffer.payload.ctr);\
          request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); \
          break;\
        }\
      }\
    }\
  );\
// #endif // ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS

// global enum for types of mqtt handlers, this can be asked for as a system command
// Note: "MQTT_HANDLER_LENGTH_ID" is used in module/classes as the start position/index for specialised handlers
// enum MQTT_HANDLER_IDS{
//   // Ability to ask for everything
//   MQTT_HANDLER_ALL_ID = 0,
//   // Core/shared handlers
//   MQTT_HANDLER_SETTINGS_ID,
//   MQTT_HANDLER_SENSOR_IFCHANGED_ID,
//   MQTT_HANDLER_SENSOR_TELEPERIOD_ID,   
//   MQTT_HANDLER_OUTPUT_IFCHANGED_ID, //drivers primary output
//   MQTT_HANDLER_OUTPUT_TELEPERIOD_ID,    
//   // Count of total handlers and starting point for other modules
//   MQTT_HANDLER_LENGTH_ID 
// };

// global enum for types of mqtt handlers, this can be asked for as a system command
//System has no ifchanged/teleperiod extra level
enum TOPIC_TYPE_IDS{
  MQTT_TOPIC_TYPE_SYSTEM_ID=0, 
  MQTT_TOPIC_TYPE_IFCHANGED_ID,
  MQTT_TOPIC_TYPE_ROC1M_ID,
  MQTT_TOPIC_TYPE_ROC10M_ID,
  MQTT_TOPIC_TYPE_SENSOR_TELEPERIOD_ID,   // 1 second
  MQTT_TOPIC_TYPE_SETTINGS_TELEPERIOD_ID, //show settings, but 1 hour increments
  MQTT_TOPIC_LWT_ONLINE_ID,
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
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LWT_ONLINE_CTR)   "LWT";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR)       "health";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR)     "settings";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR)          "log";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR)     "firmware";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR)       "memory";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR)      "network";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR)         "mqtt";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR)         "time";     
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR)       "reboot";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR) "reboot/event";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_DEVICES_CTR)      "debug/devices";   
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR)           "debug/pins";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR)       "debug/template";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR) "debug/moduleinterface";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR) "debug/stored_settings";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR)  "debug/minimal";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE) "debug/tasker_interface_performance";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR) "debug/settings_storage";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR) "sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR)   "power";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR)   "light/state";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR) "state";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIMED_CTR) "timed";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR) "debug";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR) "motion";


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
  #ifdef ENABLE_PHASEOUT__MQTT_HANDLER_ID
  uint8_t       handler_id = 0; //phase out, this is not used for anything
  #endif
  Handler_Flags flags = {0}; //#issue FrequencyRedunctionLevel
  uint8_t       (Class::*ConstructJSON_function)(uint8_t json_level, bool json_appending); // member-function to sender with two args. Extra "json_appending" will allow calling constructjsons directly and adding them to another without closing the main json object
};

#include "PubSubClient.h"
class PubSubClient;

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


enum ConnectionClient_t{
  CLIENT_TYPE_WIFI_ID = 0,
  CLIENT_TYPE_ETHERNET_ID,
  CLIENT_TYPE_CELLULAR_ID
};


// #define MQTT_CONNECTION_CONNECTED_CALLBACK_SIGNATURE std::function<bool(void)> callback




// Contains instance of each connection
class MQTTConnection
{
  public:
  
    uint16_t connect_count = 0;            // MQTT re-connect count
    uint16_t retry_counter = 1;            // MQTT connection retry counter
    
    uint16_t downtime_counter = 0;
    uint32_t uptime_seconds = 0;

    // uint8_t initial_connection_state = 2;  // MQTT connection messages state
    bool connected = false;                // MQTT virtual connection status
    bool allowed = false;                  // MQTT enabled and parameters valid
    bool mqtt_tls = false;                 // MQTT TLS is enabled
    bool flag_start_reconnect = false;
    uint8_t cConnectionAttempts = 0; 
    uint8_t host_server_type = 0;


    char broker_url[100] = {0}; // ip or url, always convert and store IP as string to make this the same method for all types
    uint16_t port = 0; // 0 means not used

    ConnectionClient_t client_type = CLIENT_TYPE_WIFI_ID; //default wifi, 1 ethernet, 2 cellular
   

    uint32_t tSaved_LastOutGoingTopic = 0; //this needs to become inside instance, so multiple connections dont block others

    struct DEBUG{
      #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
      uint32_t payload_publish_sent = 0;
      uint32_t payload_publish_missed = 0;
      float    payload_publish_success_percentage = 0;
      #endif

    }debug_stats;

    
    // MQTT_CONNECTION_CONNECTED_CALLBACK_SIGNATURE;
    // void setCallback(MQTT_CONNECTION_CONNECTED_CALLBACK_SIGNATURE);
    
    Client* network_client = nullptr;


    PubSubClient* pubsub = nullptr; //to be made private

    MQTTConnection(Client* client_in, char* url, uint16_t _port, ConnectionClient_t _type)
    {
      
      Serial.printf("MQTT::CreateConnection \n\r"); Serial.flush();

      network_client = client_in;
      Serial.printf("MQTT::CreateConnection \n\r"); Serial.flush();
      SetPubSubClient(client_in);
      Serial.printf("MQTT::CreateConnection \n\r"); Serial.flush();
      port = _port;
      client_type = _type;
      Serial.printf("MQTT::CreateConnection \n\r"); Serial.flush();
      snprintf(broker_url, sizeof(broker_url), url);

      Serial.printf("MQTT::CreateConnection \n\r"); Serial.flush();
    };

    MQTTConnection(IPAddress addr, uint16_t port, Client& client)
    {
      Serial.println("Creating new PubSubClient");
      pubsub = new PubSubClient(addr, port, client);
    };

    void TaskerHandlers();

    void EverySecond();

    void EveryLoop()
    {
      
    }

    void Send_LWT_Online();
    
    bool MqttIsConnected()
    {
      return pubsub->connected();
    }

    // Faster than doing socket open, try this prior to sending
    bool NetworkConnected()
    {
      if(!network_client) return false;
      return network_client->connected();
    }
    
    
    void MqttConnected(void);
    boolean psubscribe(const char* topic);
    void MqttReconnect();  

    void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len);  
    
    void MqttDisconnected(int state);
    void SetPubSubClient(Client* client);
    bool MQTTHandler_Send_Formatted(uint8_t topic_type, uint16_t module_id, const char* postfix_topic_ctr);
    void Send_Prefixed_P(const char* topic, PGM_P formatP, ...);
    bool publish_ft( const char* module_name,uint8_t topic_type_id, const char* topic_postfix, const char* payload, uint8_t retain_flag);
    void publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag);

    

    boolean ppublish(const char* topic, const char* payload, boolean retained = false);
    
    boolean ppublish_device_name_prefix_P(const char* topic, const char* payload, boolean retained = false);
    
};



class mMQTT :
  public mTaskerInterface
{

  public:
    mMQTT(){};
    void Init(void);
    static const char* PM_MODULE_NETWORK_MQTT_CTR;
    static const char* PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_MQTT_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_MQTT_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mMQTT);     };
    #endif
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void parse_JSONCommand(JsonParserObject obj);

    std::vector<MQTTConnection*> brokers;


    boolean Publish(const char* topic, const char* payload, boolean retained = false);
    boolean Subscribe(const char* topic, uint8_t qos = 0);

    void MQTTSubscribe();
    

    char* TopicFormatted(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, char* buffer, uint8_t buflen);
    /**
     * @brief Create a Connection, only if it is new. Checks are needed
     * 
     * returns if new connection has been added
     * @return true 
     * @return false 
     */
    bool CreateConnection(Client* client_in, char* url, uint16_t _port, ConnectionClient_t type)
    {

      // DEBUG_LINE_HERE;
      
          Serial.printf("CreateConnectionn\r\n\r\n\r\n\r\n\r\n");
          
          Serial.printf("CreateConnectionn\r\n\r\n\r\n\r\n\r\n");
          Serial.flush();


      /**
       * @brief Check if url already exists
       **/
      int8_t found_index = -1;
      uint8_t search_index = 0;
      for(auto& con:brokers)
      {
        Serial.printf("Checking \"%s\" against existing \"%s\"\n\r\r\n\r\n\r\n\r\n\r\n", url, con->broker_url);
        Serial.flush();
        if(strcmp(con->broker_url, url)==0)
        {
          // Serial.printf("found existing connection with \"%s\" Index:%d\n\r", url, search_index);
        search_index++;
        }
        else
        {
          // Serial.printf("NOT found, needs to add \"%s\" Index:%d\n\r", url, search_index);
        }
      }

      if(search_index==0) // None found
      {
        Serial.printf("Adding new MQTTConnection \"%s\" Index:%d brokers_size:%d\n\r", url, search_index, brokers.size());
        Serial.flush();
        brokers.push_back(new MQTTConnection(client_in, url, _port, type));
      }

      Serial.printf("MQTT::CreateConnection Host:\"%s\", Port:%d, Index:%d\n\r", url, _port, search_index);
Serial.flush();
    }

    


    void MM_EveryLoop()
    {
      for(auto& con:brokers)
      {
        con->EveryLoop();
      }
      
    }


    void MM_EverySecond()
    {
      for(auto& con:brokers)
      {
        con->EverySecond();
      }
    }

    void MM_Every50mSecond()
    {
      for(auto& con:brokers)
      {

        if(con->pubsub->loop())
        {
          // connected maintained by loop call. Same as pubsub->connected(), this does so by checking network client too!
          
          // cancel reconnect
            con->flag_start_reconnect = false;
            con->retry_counter = 0;

        }
        else
        {
          // Serial.println(D_LOG_PUBSUB "loop() Not Connected");

          /**
           * @brief Check if not waiting to reconnect
           **/
          if(!con->retry_counter)
          {
            con->flag_start_reconnect = true;
            con->retry_counter = 5;
          }
          
        }
      }
    }
    

    boolean psubscribe(const char* topic)
    {
      uint8_t count = 0;
      for(auto& con:brokers)
      {
        count += con->psubscribe(topic);
      }
      return (count == brokers.size()) ? true : false;
    }


    bool flag_uptime_reached_reduce_frequency = false;

    const char* GetState_PCtr(int8_t state);
    void Load_New_Subscriptions_From_Function_Template();
    
    void setprefixtopic(const char* _prefixtopic);

    void CallMQTTSenders();


    /**
     * Special MQTT functions that manupulate the generic mqtthandler_list from any class
     * handler<mAnimatorLight> *mAnimatorLight::mqtthandler_list[5]
     * Future idea:
     * Issue: Unknown how to also send length or array, so for(auto&) type loops probably wont work
     * */
    template<typename T>
    void MQTTHandler_Flags_SendNow(std::vector<struct handler<T>*> mqtthandler_list)
    {     
      for(auto& handle:mqtthandler_list){
        handle->flags.SendNow = true;
      }
    }

    /**
     * @brief Function will take the struct storing the mqtt handler options, and check the variables, sending if required
     * @param class_ptr Pointer to the instance of a class
     * @param class_id  Numerical (Enum) value of the class, which is used to acquire the mqtt topic
     * @param handler_ptr Pointer to the handler struct with timing and function to be called
     * @param optional_desired_id Optional handler_id, if set (>=0) then only matching handler IDs will be sent. Used for group triggering by a type.
     * @note  Optional desired_id will check if the handler id was set, and if it does not match, will return without servicing handler
     * */
    template<typename T>
    void MQTTHandler_Command(T& class_ptr, uint16_t class_id, handler<T>* handler_ptr)
    {
      
      // Sanity check
      if(handler_ptr == nullptr){
        Serial.println("handler_ptr == nullptr"); Serial.flush(); 
        return;
      }


      /**
       * @brief Only permit function to run if at least ONE broker is connected
       */
      bool is_any_broker_connected = false;
      for(auto& con:brokers)
      {
        if(con->uptime_seconds)
        {
          is_any_broker_connected = true;
        }
      }
      if(!is_any_broker_connected)
      {
        Serial.println("MQTTHandler_Command::Blocked");
        return;
      }
            
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
        
        // #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
        // Serial.printf("MQTTHandler_Command::SendNow::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
        // #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

        uint8_t fSendPayload = CALL_MEMBER_FUNCTION(class_ptr,handler_ptr->ConstructJSON_function)(handler_ptr->json_level, true);

        bool packet_successfully_sent = false;
         
        // Send MQTT payload with structured output
        if(fSendPayload)
        { 

          #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
          Serial.printf("MQTTHandler_Command::SendNow::fSendPayload::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
          #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

          // Send on each connected broker
          for(auto& con:brokers)
          {

            /*
            * prior to sending, check if limit time has passed
            */
            #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
            if(labs(millis()-con->tSaved_LastOutGoingTopic)>ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS)
            {
            #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

              uint32_t tSaved_SendTime = millis();

              packet_successfully_sent = con->MQTTHandler_Send_Formatted(handler_ptr->topic_type,class_id,handler_ptr->postfix_topic); 

              #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
              Serial.printf("^^^^^^^^^^^^^^^^^^^ MQTTHandler_Send_Formatted %dms \n\r", millis()-tSaved_SendTime );
              #endif
              
            #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
              if(packet_successfully_sent)
              {
                #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
                Serial.printf("^^^^^^^^^^^^^^^^^^^ packet_successfully_sent %s DONE \n\r",handler_ptr->postfix_topic);
                #endif
                #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
                con->debug_stats.payload_publish_sent++;
                con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
                #endif

                handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
                handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once

              }
              else
              {
                #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
                Serial.printf("------------------- packet_successfully_sent %s ERROR\n\r",handler_ptr->postfix_topic);
                #endif
                #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
                con->debug_stats.payload_publish_missed++;
                con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
                #endif
              }
            }else{
                // Serial.printf("------------------- packet_successfully_sent %s LIMIT\n\r",handler_ptr->postfix_topic);
            }
            #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
            Serial.printf("last sent %d\n\r", millis()-con->tSaved_LastOutGoingTopic);
            #endif
          
            #else
          
              handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
              handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once
          
            #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

          }

        }
        

      }
    };

    
    #ifdef ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
    /**
     * @brief Function will take the struct storing the mqtt handler options, and check the variables, sending if required
     * @param class_ptr Pointer to the instance of a class
     * @param class_id  Numerical (Enum) value of the class, which is used to acquire the mqtt topic
     * @param handler_ptr Pointer to the handler struct with timing and function to be called
     * @param optional_desired_id Optional handler_id, if set (>=0) then only matching handler IDs will be sent. Used for group triggering by a type.
     * @note  Optional desired_id will check if the handler id was set, and if it does not match, will return without servicing handler
     * */
    template<typename T>
    void MQTTHandler_AddWebURL(T& class_ptr, uint16_t class_id, handler<T>* handle)
    {
      PGM_P module_ctr = pCONT->GetModuleFriendlyName(class_id);

      char uri_buffer[70] = {0};
      snprintf(uri_buffer, sizeof(uri_buffer), "/mqtt/%s/%S", D_TOPIC_STATUS, module_ctr);
      pCONT_web->server->on(uri_buffer, HTTP_GET, [class_ptr, handle](AsyncWebServerRequest *request)
      {
        char handle_url[100] = {0};
        // // for(auto& handle:mqtthandler_list)
        // // {      
        // // PGM_P module_ctr = pCONT->GetModuleFriendlyName(class_id);

        // TopicFormatted(  class_ptr->GetModuleFriendlyName(),
        //         handle->topic_type,
        //         handle->postfix_topic, handle_url, sizeof(handle_url));  

        // // ALOG_INF(PSTR("handle_url=%s"), handle_url);
        // // ALOG_INF(PSTR("%s in %s?"), handle->postfix_topic, request->url().c_str());      

        // const String& incoming_uri = request->url();
        // if(incoming_uri.indexOf(handle_url) > 0)
        // {
        //   // ALOG_INF(PSTR("%s"), request->url().c_str());            
        //   uint8_t fSendPayload = CALL_MEMBER_FUNCTION(class_ptr, handle->ConstructJSON_function)(handle->json_level, true);
        //   // ALOG_INF(PSTR("data_buffer.payload.ctr=%s"), data_buffer.payload.ctr);
        //   request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); 
        //   // break; // to stop accidental double matches, only respond once
        // }
        // }
      });
    }
    #endif // ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED

};




#endif

#endif //  USE_MODULE_NETWORK_MQTT