#ifndef _MMQTT
#define _MMQTT 0.7

#define D_UNIQUE_MODULE_NETWORK_MQTT_ID  3010 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/00_FirmwareDefaults/mFirmwareDefaults.h" 

#ifdef USE_MODULE_NETWORK_MQTT
#include <stdint.h>

#include "2_CoreSystem/mGlobalMacros.h"



// Void Arg Functions
#define CALL_MEMBER_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))

enum TOPIC_TYPE_IDS
{
  MQTT_TOPIC_TYPE_SYSTEM_ID=0, 
  MQTT_TOPIC_TYPE_IFCHANGED_ID,
  // MQTT_TOPIC_TYPE_ROC1M_ID,
  // MQTT_TOPIC_TYPE_ROC10M_ID,
  MQTT_TOPIC_TYPE_SENSOR_TELEPERIOD_ID,   // 1 second
  MQTT_TOPIC_TYPE_SETTINGS_TELEPERIOD_ID, //show settings, but 1 hour increments
  MQTT_TOPIC_LWT_ONLINE_ID,
  MQTT_TOPIC_TYPE_TELEPERIOD_ID,
  MQTT_TOPIC_TYPE__DEBUG__ID,  
  MQTT_TOPIC_TYPE_LENGTH_ID=8
};

enum MQTT_FREQUENCY_REDUCTION_LEVEL_IDS
{
  MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID = 0, // leave it as original set
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_1_MINUTES_ID = 1,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID = 2,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_60_MINUTES_ID = 3,
  MQTT_FREQUENCY_REDUCTION_LEVEL_LENGTH_ID = 8
};

// Put all shared ones here, eg "settings" and "sensor", with specific inside module.h when needed
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LWT_ONLINE_CTR)   "LWT";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR)       "health";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR)     "settings";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_SYSTEM_CTR)  "settings/system";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_NETWORK_CTR) "settings/network";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_DRIVERS_CTR) "settings/drivers";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_SENSORS_CTR) "settings/sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_LIGHTS_CTR)  "settings/lights";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_POWER_CTR)   "settings/power";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_RULES_CTR)   "settings/rules";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_RUNTIME_CTR) "settings/runtime";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_TEXT_BUFFER_CTR) "settings/text_buffer";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR)          "log";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_PERIPHERALS_CTR)   "peripherals";   
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR)     "firmware";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR)       "memory";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR)      "network";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR)         "mqtt";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR)         "time";     
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TASKERMANAGER_CTR) "taskermanager";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR)       "reboot/last";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR) "reboot/event";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_DEVICES_CTR)      "debug/devices";   
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_GPIO_CTR)                 "debug/gpio";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_TABLE_CTR)                "debug/pintable";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR)                  "debug/template";     
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR)    "debug/stored_settings";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE)  "debug/tasker";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR)          "debug/settings_storage";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR) "sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR)   "power";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR)   "light/state";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR)   "state";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIMED_CTR)   "timed";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR)   "debug";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR)  "motion";

DEFINE_PGM_CTR(PM_MQTT_LWT_PAYLOAD_ONLINE)      "{\"LWT\":\"Online\"}";
DEFINE_PGM_CTR(PM_MQTT_LWT_PAYLOAD_FORMATED)    "{\"LWT\":\"Offline\",\"ResetReason\":\"%s\",\"Uptime\":\"%s\"}";
DEFINE_PGM_CTR(PM_MQTT_LWT_TOPIC_FORMATED)      "%s/status/LWT";

#ifndef MQTT_MAX_BROKERS
#define MQTT_MAX_BROKERS 4
#endif

enum MQTT_HANDLER_PRIORITY_IDS
{
  MQTT_HANDLER_PRIORITY_BACKGROUND_ID = 0,
  MQTT_HANDLER_PRIORITY_NORMAL_ID     = 1,
  MQTT_HANDLER_PRIORITY_IMPORTANT_ID  = 2,
  MQTT_HANDLER_PRIORITY_CRITICAL_ID   = 3
};

// move to telemetry header
typedef union {
  uint16_t data;
  struct {
    uint16_t PeriodicEnabled          : 1;
    uint16_t SendNow                  : 1;
    uint16_t FrequencyRedunctionLevel : 2;
    uint16_t retain                   : 1;
    uint16_t json_level               : 3;
    uint16_t topic_type               : 3;
    uint16_t priority                 : 2;
    uint16_t reserved                 : 3;
  };
} Handler_Flags;


template <typename Class>
struct telemetry_handler {
  uint32_t      tSavedLastSent = 0;
  uint16_t      tRateSecs = 1;
  uint8_t       json_level = 0;
  uint8_t       topic_type = 0;
  const char*   key = nullptr;
  Handler_Flags flags = {0};
  uint8_t       (Class::*ConstructJSON_function)(uint8_t json_level, bool json_appending);

  void Send()
  {
    flags.SendNow = true;
  }
};

#include "PubSubClient.h"
class PubSubClient;

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>

#include "1_TaskerManager/mTaskerInterface.h"
#define WILLQOS_CTR 2
#define WILLRETAIN_CTR true

enum ConnectionClient_t
{
  CLIENT_TYPE_WIFI_ID = 0,
  CLIENT_TYPE_ETHERNET_ID,
  CLIENT_TYPE_CELLULAR_ID
};

#define ENABLE_DEBUG_MQTT__INCLUDE_NEW_CONNECTION_SPLASH


class MQTTConnection
{
  public:
  
    char      host_address[33];
    uint8_t   status = 0;
    uint16_t  port;
    char      user[33] = {0};
    char      password[33] = {0};
    char      prefix_topic[50]; // "<devicename>/"
    char      client_name[80]; 
    uint16_t  retry = 5; //default

    Client* network_client = nullptr;
    PubSubClient* pubsub = nullptr; //to be made private

    ConnectionClient_t client_type = CLIENT_TYPE_WIFI_ID; //default wifi, 1 ethernet, 2 cellular   
    uint32_t tSaved_LastOutGoingTopic = 0;

    uint16_t connect_count = 0;            // MQTT re-connect count
    uint16_t retry_counter = 1;            // MQTT connection retry counter
    uint16_t retry_counter_start_value = 5; // will be set 5 for wifi, 60 for LTE    
    uint16_t downtime_counter = 0;
    uint32_t uptime_seconds = 0;
    bool connected = false;                // MQTT virtual connection status
    bool allowed = false;                  // MQTT enabled and parameters valid
    bool mqtt_tls = false;                 // MQTT TLS is enabled
    bool flag_start_reconnect = false;
    uint8_t cConnectionAttempts = 0; 
    uint8_t host_server_type = 0;
    char id[16] = {0};
    #ifndef MQTT_TRANSPORT_LIST_MAX
      #define MQTT_TRANSPORT_LIST_MAX 3
      #endif

      bool en = true;

      uint16_t backoff_fast_secs = 5;
      uint16_t backoff_periodic_secs = 10;
      uint16_t backoff_max_secs = 60;

      ConnectionClient_t transport[MQTT_TRANSPORT_LIST_MAX];
      uint8_t transport_count = 0;

      ConnectionClient_t pref_transport[MQTT_TRANSPORT_LIST_MAX];
      uint8_t pref_transport_count = 0;

      uint8_t outgoing_level = 3;
      uint16_t outgoing_limiter_ms = 0;

      uint16_t keepalive_secs = 60;


    /**
     * @brief 
     * 0: Disabled
     * 1: Enabled and connected
     * 2: Enabled, not connected
     *  
     */
    struct interface_priority_flags_s{
      uint8_t energy = 0;
      uint8_t light = 0;
    }interface_reporting_priority;
  

    struct DEBUG{
      #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
      uint32_t payload_publish_sent = 0;
      uint32_t payload_publish_missed = 0;
      float    payload_publish_success_percentage = 0;
      #endif
    }debug_stats;

    MQTTConnection()
    {
      host_address[0] = '\0';
      prefix_topic[0] = '\0';
      client_name[0] = '\0';

      port = 1883;
      client_type = CLIENT_TYPE_WIFI_ID;

      network_client = nullptr;
      pubsub = nullptr;

      retry = 5;
      retry_counter = 1;
      retry_counter_start_value = 5;

      connected = false;
      allowed = false;
    }

    /**
     * @brief Minimal for new connection
     * 
     * @param client_in 
     * @param host_address 
     * @param _port 
     * @param _type 
     */
    MQTTConnection(Client* _client,
               const char* _host_address,
               uint16_t _port,
               ConnectionClient_t _type,
               const char* username = nullptr,
               const char* password = nullptr)
    {
      network_client = _client;
      port = _port;
      client_type = _type;

      // Safe string copy (3rd arg must be a format string)
      if (_host_address) {
        snprintf(host_address, sizeof(host_address), "%s", _host_address);
      } else {
        host_address[0] = '\0';
      }

      // Build PubSub after host/port are set (SetPubSubClient should use these)
      SetPubSubClient(_client);

      Serial.printf("MQTT::CreateConnection broker_url B %s\r\n", host_address);
      Serial.flush();

      if (username && password) {
        SetCredentials((char*)username, (char*)password);
      }
    }



    MQTTConnection(IPAddress addr, uint16_t _port, Client& client)
    {
      port = _port;
      client_type = CLIENT_TYPE_WIFI_ID; // or pass in if you use this ctor
      network_client = &client;

      Serial.println("Creating new PubSubClient");
      pubsub = new PubSubClient(addr, _port, client);
    }


    void SetCredentials(char* _user, char* _password)
    {

      char password_copy[5];
      snprintf(password_copy, sizeof(password_copy), _password);

      Serial.printf("MQTT SetCredentials %s,%s##\n\r",_user,password_copy); // Only show start of password

      if((strlen(_user) > sizeof(user)) || (strlen(_password) > sizeof(password))) 
      {
      Serial.println("Error");
      return;
      }
          
      strlcpy(user, _user, sizeof(user));
      strlcpy(password, _password, sizeof(password));
    }


    void SetReConnectBackoffTime(uint8_t _sec)
    {
      retry = _sec;
    }


    void SetClientName(char* _client_name)
    {
      snprintf(client_name, sizeof(client_name), _client_name);
    }


    void SetTopicPrefix(char* _prefix_topic)
    {
      snprintf(prefix_topic, sizeof(prefix_topic), _prefix_topic);
    }

    

    void TaskerHandlers();

    void EverySecond();

    void EveryLoop(){}

    void Send_LWT_Online();
    
    bool MqttIsConnected() { return pubsub && pubsub->connected(); }


    // Faster than doing socket open, try this prior to sending
    bool NetworkConnected()
    {
      if(!network_client) return false;
      return network_client->connected();
    }
    
    
    void MqttConnected(void);
    boolean subscribe_device(const char* topic);
    boolean subscribe(const char* topic);
    void MqttReconnect();  
    void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len);     
    void MqttDisconnected(int state);
    void SetPubSubClient(Client* client);
    bool MQTTHandler_Send_Formatted_UniqueID(uint8_t topic_type, uint16_t unique_id, const char* postfix_topic_ctr, bool retain_flag = false);
    void Send_Prefixed_P(const char* topic, PGM_P formatP, ...);
    bool publish_ft( const char* module_name,uint8_t topic_type_id, const char* topic_postfix, const char* payload, uint8_t retain_flag);
    void publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag);

    boolean publish_device(const char* topic, const char* payload, boolean retained = false);
    boolean publish_device_P(const char* topic, const char* payload, boolean retained = false);
    
    #ifdef ENABLE_DEBUG_MQTT__INCLUDE_NEW_CONNECTION_SPLASH
    String Debug_BuildConnectionInfo(void)
    {
      String msg;

      msg.reserve(512);

      msg += F("MQTTConnection");
      msg += F("\r\n  Id: ");
      msg += id[0] ? id : "<none>";

      msg += F("\r\n  Enabled: ");
      msg += en ? F("true") : F("false");

      msg += F("\r\n  Allowed: ");
      msg += allowed ? F("true") : F("false");

      msg += F("\r\n  Connected: ");
      msg += MqttIsConnected() ? F("true") : F("false");

      msg += F("\r\n  Host: ");
      msg += host_address[0] ? host_address : "<none>";

      msg += F("\r\n  Port: ");
      msg += port;

      msg += F("\r\n  User: ");
      msg += user[0] ? user : "<none>";

      msg += F("\r\n  TopicPrefix: ");
      msg += prefix_topic[0] ? prefix_topic : "<none>";

      msg += F("\r\n  ClientName: ");
      msg += client_name[0] ? client_name : "<none>";

      msg += F("\r\n  ClientType: ");
      msg += client_type;

      msg += F("\r\n  NetworkClient: ");
      msg += network_client ? F("set") : F("null");

      msg += F("\r\n  PubSubClient: ");
      msg += pubsub ? F("set") : F("null");

      msg += F("\r\n  Retry: ");
      msg += retry;

      msg += F("\r\n  Backoff: [");
      msg += backoff_fast_secs;
      msg += F(", ");
      msg += backoff_periodic_secs;
      msg += F(", ");
      msg += backoff_max_secs;
      msg += F("]");

      msg += F("\r\n  KeepAliveSecs: ");
      msg += keepalive_secs;

      msg += F("\r\n  OutgoingLevel: ");
      msg += outgoing_level;

      msg += F("\r\n  OutgoingLimiterMs: ");
      msg += outgoing_limiter_ms;

      msg += F("\r\n  Transport: [");

      for(uint8_t i = 0; i < transport_count; i++)
      {
        if(i){ msg += F(", "); }
        msg += transport[i];
      }

      msg += F("]");

      msg += F("\r\n  PrefTransport: [");

      for(uint8_t i = 0; i < pref_transport_count; i++)
      {
        if(i){ msg += F(", "); }
        msg += pref_transport[i];
      }

      msg += F("]");

      msg += F("\r\n  ConnectCount: ");
      msg += connect_count;

      msg += F("\r\n  Attempts: ");
      msg += cConnectionAttempts;

      msg += F("\r\n  UptimeSeconds: ");
      msg += uptime_seconds;

      msg += F("\r\n  DowntimeCounter: ");
      msg += downtime_counter;

      return msg;
    }
    void Debug_PrintConnectionInfo(void)
    {
      Serial.println(Debug_BuildConnectionInfo());
      Serial.flush();
    }
    #endif
};



class mMQTTManager :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mMQTTManager(){};
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    char test[30];

    static constexpr const char* PM_MODULE_NETWORK_MQTT_CTR = D_MODULE_NETWORK_MQTT_CTR;;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_MQTT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_MQTT_ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      // uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Default_Module(void);
    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM

    typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
      uint8_t data;                           // Allow bit manipulation using SetOption
      struct {                                
        uint8_t unified_module_interface_reporting__suppress_submodule_configperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t unified_module_interface_reporting__suppress_submodule_teleperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t unified_module_interface_reporting__suppress_submodule_ifchangedperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t reserved : 6;          // bit 1              - SetOption1  - 
      };
    } SysBitfield_Options;


    struct MODULE_STORAGE{ // these will be saved and recovered on boot

      uint8_t mqtt_retain : 1;               // bit 0              - SetOption0  - Save power state and use after restart
      uint16_t      configperiod_secs;
      uint16_t      teleperiod_secs;
      uint16_t      ifchanged_secs;
      // Percentage of possible values that signify a large enough change has occured worth reporting.
      // A value of '0' percent means anything at all
      uint8_t       ifchanged_change_percentage_threshold;
      uint8_t       teleperiod_json_level;
      uint8_t       ifchanged_json_level;
      uint8_t       teleperiod_retain_flag;
      SysBitfield_Options options;
    }dt;

    std::vector<MQTTConnection*> brokers;

    WiFiClient* mqtt_client = nullptr;

    bool flag_mqtt_realtime_reduced_rates = false;

    void EnableRealtimeReducedMQTTRates();
    uint16_t GetRealtimeReducedMQTTRate(uint16_t unique_id, uint16_t current_rate_secs, uint8_t priority);


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    uint16_t GetConfigPeriod(); // main
    uint16_t GetConfigPeriod_SubModule();
    uint16_t GetTelePeriod(); // main
    uint16_t GetTelePeriod_SubModule();
    uint16_t GetIfChangedPeriod(); // main
    uint16_t GetIfChangedPeriod_SubModule();
    void Start_Connection();

    void subparse_JSONCommand__Broker(JsonParserObject obj, uint8_t broker_i);

    #ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS

    void MqttPublishPayloadPrefixTopic_P(uint32_t prefix, const char* subtopic, const char* payload, uint32_t binary_length, bool retained);
    void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic, bool retained);
    void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, bool retained);
    void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic);
    #define ResponseData() data_buffer.topic.ctr
    void MqttPublishPowerBlinkState(uint32_t device);

    #endif
    
    void Handle__ServiceBrokerConnects_With_Transports(void);

    void Send_LWT_To_Any_Connected_Brokers();

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    void Default_Module__Connection_WiFi();
    
    boolean Publish(const char* topic, const char* payload, boolean retained = false);
    boolean Subscribe(const char* topic, uint8_t qos = 0);

    void MQTTSubscribe();
    
    
    int8_t CreateConnection(Client* client_in,
                        const char* host_address,
                        uint16_t port,
                        ConnectionClient_t type,
                        const char* username = nullptr,
                        const char* password = nullptr)
    {
      int8_t found_index = -1;
      uint8_t idx = 0;

      for (auto* con : brokers)
      {
        if (con && con->host_address[0])
        {
          Serial.printf("Checking \"%s\" against existing \"%s\"\r\n", host_address, con->host_address);

          if ((strcmp(con->host_address, host_address) == 0) &&
              (con->port == port) &&
              (con->client_type == type))
          {
            found_index = (int8_t)idx;
            Serial.printf("found existing connection with \"%s\" Index:%d\r\n", host_address, found_index);
            break;
          }
        }
        idx++;
      }

      if (found_index >= 0)
      {
        // Optional: update underlying client pointer if caller passed a new one
        brokers[found_index]->network_client = client_in;
        brokers[found_index]->SetPubSubClient(client_in);

        if (username && password)
          brokers[found_index]->SetCredentials((char*)username, (char*)password);

        Serial.printf("MQTT::CreateConnection REUSE Host:\"%s\", Port:%u, Index:%d\r\n",
                      host_address, port, found_index);
        return found_index;
      }

      // Add
      MQTTConnection* c = new MQTTConnection(client_in, (char*)host_address, port, type,
                                            (char*)username, (char*)password);
      brokers.push_back(c);

      const int8_t new_index = (int8_t)(brokers.size() - 1);
      Serial.printf("MQTT::CreateConnection ADD Host:\"%s\", Port:%u, Index:%d\r\n",
                    host_address, port, new_index);
      return new_index;
    }

    void EnsureDefaultBroker_FromDefines(void);


    bool AnyBrokerConnected();
    


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
        if(!con){ continue; }
        if(!con->pubsub){ continue; }

        if(con->pubsub->loop())
        {
          con->flag_start_reconnect = false;
          con->retry_counter = 0;
        }
        else
        {
          if(!con->retry_counter)
          {
            con->flag_start_reconnect = true;
            con->retry_counter = 5;
          }
        }
      }
    }
    

    boolean subscribe_device(const char* topic)
    {
      uint8_t count = 0;
      for(auto& con:brokers)
      {
        count += con->subscribe_device(topic);
      }
      return (count == brokers.size()) ? true : false;
    }


    bool flag_uptime_reached_reduce_frequency = false;

    uint32_t     rate_limit_send_delay = 0; // 0 means no delay, otherwise it will delay sending if its been less than this amount of milliseconds since last send, this is to help with bursty data and mqtt traffic management


    uint32_t tSaved_LastStagedMqttSend = 0;


    // const char* GetState_PCtr(int8_t state);
    void Load_New_Subscriptions_From_Function_Template();
    
    void CallMQTTSenders();


    /**
     * Special MQTT functions that manupulate the generic telemetry_list from any class
     * telemetry_handler<mAnimatorLight> *mAnimatorLight::telemetry_list[5]
     * Future idea:
     * Issue: Unknown how to also send length or array, so for(auto&) type loops probably wont work
     * */
    // template<typename T>
    // void MQTTHandler_Flags_SendNow(std::vector<struct telemetry_handler<T>*> telemetry_list)
    // {     
    //   for(auto& handle:telemetry_list){
    //     handle->flags.SendNow = true;
    //   }
    // }


    // template<typename T>
    // void MQTTHandler_Sender(std::vector<struct telemetry_handler<T>*>& handler_list, T& class_ptr) {
    //     for (auto& handle : handler_list) {
    //         // Serial.printf("MQTTHandler_Sender");
    //         MQTTHandler_Command_UniqueID(class_ptr, class_ptr.GetModuleUniqueID(), handle);
    //     }
    // }

    /**
   * @brief Runs the MQTT send pass for a module's registered MQTT handlers.
   *
   * Each module owns a list of MQTT handler descriptors. A handler decides when
   * its own payload is due using its normal timing state, for example
   * `tRateSecs`, `tSavedLastSent`, `SendNow`, and the periodic trigger logic
   * inside `MQTTHandler_Command_UniqueID()`.
   *
   * Without staging, this sender iterates the whole handler list in one pass.
   * Therefore, if several handlers become due at the same time, several MQTT
   * payloads may be published back-to-back during a single `TASK_TELEMETRY__SENDER_MQTT`
   * call. That is acceptable for normal modules, but can create burst traffic for
   * telemetry modules where many hourly/status payloads mature together.
   *
   * The optional `staged_backoff_ms` argument adds a sender-level drain limit.
   * When non-zero, this wrapper first checks the MQTT manager's shared staged
   * send timer. If the backoff period has not expired, the whole send pass is
   * skipped. If the backoff period has expired, the handler list is scanned until
   * one handler actually publishes a payload. After one successful publish, the
   * staged timer is updated and the function returns immediately.
   *
   * This does not replace the per-handler timing checks. It only controls how
   * quickly already-due handlers are drained from the list. In other words:
   *
   *   - per-handler timing decides whether a specific payload is due;
   *   - staged backoff decides whether another due payload may be sent now.
   *
   * This keeps normal MQTT behaviour unchanged when `staged_backoff_ms == 0`,
   * while allowing telemetry-heavy modules to opt in to staged publishing:
   *
   * @code
   *   tkr_mqtt->MQTTHandler_Sender(
   *       telemetry_list,
   *       *this,
   *       MQTT_TELEMETRY_STAGED_BACKOFF_MS
   *   );
   * @endcode
   *
   * If `ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS` is defined, it is applied
   * here as a legacy/default sender-level pacing value only when the caller has
   * not supplied an explicit staged backoff. This keeps send-rate limiting at the
   * wrapper/drain-control layer rather than inside the per-handler publish
   * function.
   *
   * The staged timer is intentionally held by the MQTT manager rather than by
   * each handler. This makes the backoff a sender-level policy and prevents
   * bursts across any module/list that opts into staged sending, while leaving
   * unstaged MQTT handlers unaffected unless the legacy/default compile-time
   * limit is enabled.
   *
   * @tparam T Module class type owning the MQTT handlers.
   * @param handler_list List of MQTT handler descriptors to service.
   * @param class_ptr Module instance used to construct payloads and resolve the
   *                  module unique ID.
   * @param staged_backoff_ms Optional minimum spacing, in milliseconds, between
   *                          successful staged MQTT publishes. Use 0 to disable
   *                          staging and preserve the original full-list pass,
   *                          unless a compile-time default limit is enabled.
   */
  template<typename T>
  void Telemetry_Sender(
      std::vector<struct telemetry_handler<T>*>& handler_list,
      T& class_ptr,
      uint32_t staged_backoff_ms = 0
  )
  {

    /*
    * Legacy/default sender pacing.
    *
    * This used to be handled deeper in the broker-send path. It is cleaner here:
    * this wrapper controls whether another handler may be drained from the list,
    * while MQTTHandler_Command_UniqueID() only decides whether one handler should
    * publish and whether that publish succeeded.
    *
    * Only apply the compile-time value if the caller did not pass an explicit
    * staged backoff.
    */
    #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
    if (staged_backoff_ms == 0) {
      staged_backoff_ms = ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS;
    }
    #endif

    if (staged_backoff_ms) {
      if (ABS_FUNCTION(millis() - tSaved_LastStagedMqttSend) < staged_backoff_ms) {
        return;
      }
    }

    for (auto& handle : handler_list) {
      const bool sent = MQTTHandler_Command_UniqueID(
        class_ptr,
        class_ptr.GetModuleUniqueID(),
        handle
      );

      if (sent && staged_backoff_ms) {
        tSaved_LastStagedMqttSend = millis();
        return;
      }
    }
  }

  
bool IsAnyBrokerConnected() const
{
  for (const auto& con : brokers) {
    if (con->uptime_seconds) {
      return true;
    }
  }
  return false;
}

// template<typename T>
// void ServicePeriodicTrigger(telemetry_handler<T>* handler_ptr)
// {
//   if (!handler_ptr->flags.PeriodicEnabled) {
//     return;
//   }

//   const uint32_t now = millis();

//   // First send after boot / init
//   if (handler_ptr->tSavedLastSent == 0) {
//     handler_ptr->tSavedLastSent = now;
//     handler_ptr->flags.SendNow = true;
//     return;
//   }

//   if (ABS_FUNCTION(now - handler_ptr->tSavedLastSent) < (handler_ptr->tRateSecs * 1000UL)) {
//     return;
//   }

//   handler_ptr->tSavedLastSent = now;
//   handler_ptr->flags.SendNow = true;

//   #ifndef ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE
//   if (flag_uptime_reached_reduce_frequency &&
//       (handler_ptr->flags.FrequencyRedunctionLevel > MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID)) {
//     handler_ptr->tRateSecs = handler_ptr->tRateSecs < 120 ? 120 : handler_ptr->tRateSecs;
//   }
//   #endif
// }

template<typename T>
void ServicePeriodicTrigger(telemetry_handler<T>* handler_ptr)
{
  if (!handler_ptr->flags.PeriodicEnabled) return;

  const uint32_t now = millis();

  // First service after boot/init: always send once so MQTT/logging state is populated.
  if (handler_ptr->tSavedLastSent == 0)
  {
    handler_ptr->tSavedLastSent = now;
    handler_ptr->flags.SendNow = true;
    return;
  }

  // Explicit/event-triggered sends bypass the periodic timer.
  if (handler_ptr->flags.SendNow) return;

  if (ABS_FUNCTION(now - handler_ptr->tSavedLastSent) < (handler_ptr->tRateSecs * 1000UL)) return;

  handler_ptr->tSavedLastSent = now;
  handler_ptr->flags.SendNow = true;

  #ifndef ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE
  if (flag_uptime_reached_reduce_frequency && handler_ptr->flags.FrequencyRedunctionLevel > MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID)
    handler_ptr->tRateSecs = max<uint16_t>(handler_ptr->tRateSecs, 120);
  #endif
}


  /**
   * @brief Checks one MQTT handler and publishes its payload if required.
   *
   * This function services a single MQTT handler descriptor. The handler owns the
   * timing and trigger state for one MQTT payload type, including periodic
   * scheduling, `SendNow`, JSON construction level, topic postfix, retain flag,
   * and topic type.
   *
   * The function returns `true` only when the handler produced a payload and that
   * payload was successfully published to at least one connected broker.
   *
   * It returns `false` when:
   *   - the handler pointer is invalid;
   *   - no broker is connected;
   *   - the handler is not currently due to send;
   *   - the handler's JSON construction function reports no payload;
   *   - all broker publish attempts fail.
   *
   * The returned value is used by higher-level sender wrappers to decide whether
   * a staged/backoff send pass should stop after one successful publish. This
   * keeps the send pacing policy outside this function. This function is only
   * responsible for servicing one handler and reporting whether it actually sent.
   *
   * @tparam T Module class type owning the MQTT handler.
   * @param class_ptr Reference to the module instance used to construct the JSON
   *                  payload.
   * @param unique_id Numerical module/device ID used to form the MQTT topic.
   * @param handler_ptr Pointer to the MQTT handler descriptor to service.
   * @return true if a payload was successfully published to at least one broker.
   * @return false if no payload was sent successfully.
   */
  template<typename T>
  bool MQTTHandler_Command_UniqueID(T& class_ptr, uint16_t unique_id, telemetry_handler<T>* handler_ptr)
  {
    if (handler_ptr == nullptr) {
      #ifdef DEBUG_NEWORK_MQTT
      Serial.println("handler_ptr == nullptr");
      Serial.flush();
      #endif
      return false;
    }

    if (!IsAnyBrokerConnected()) {
      #ifdef DEBUG_NEWORK_MQTT
      Serial.println("MQTTHandler_Command::Blocked");
      #endif
      return false;
    }

    #ifdef ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
    if (handler_ptr->tRateSecs == 0) handler_ptr->flags.PeriodicEnabled = false;
    #endif

    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    if (flag_mqtt_realtime_reduced_rates)
      handler_ptr->tRateSecs = GetRealtimeReducedMQTTRate(unique_id, handler_ptr->tRateSecs, handler_ptr->flags.priority);
    #endif

    ServicePeriodicTrigger(handler_ptr);

    if (!handler_ptr->flags.SendNow) {
      return false;
    }

    const uint8_t fSendPayload =
      CALL_MEMBER_FUNCTION(class_ptr, handler_ptr->ConstructJSON_function)(
        handler_ptr->flags.json_level,
        true
      );

    if (!fSendPayload) {
      return false;
    }

    #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
    Serial.printf(
      "MQTTHandler_Command::SendNow::fSendPayload::key\t=%S %d\n\r",
      handler_ptr->key,
      unique_id
    );
    Serial.flush();
    #endif

    bool any_broker_sent_successfully = false;

    for (auto& con : brokers) {
      const uint32_t tSaved_SendTime = millis();

      const bool packet_successfully_sent =
        con->MQTTHandler_Send_Formatted_UniqueID(
          handler_ptr->flags.topic_type,
          unique_id,
          handler_ptr->key,
          handler_ptr->flags.retain
        );

      #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_SEND_TIMING
      Serial.printf(
        "MQTTHandler_Send_Formatted %lums\n\r",
        static_cast<unsigned long>(millis() - tSaved_SendTime)
      );
      #endif

      if (packet_successfully_sent) {
        any_broker_sent_successfully = true;

        #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
        con->debug_stats.payload_publish_sent++;
        con->debug_stats.payload_publish_success_percentage =
          static_cast<float>(con->debug_stats.payload_publish_sent) /
          static_cast<float>(
            con->debug_stats.payload_publish_missed +
            con->debug_stats.payload_publish_sent
          );
        #endif
      }
      else {
        #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
        con->debug_stats.payload_publish_missed++;
        con->debug_stats.payload_publish_success_percentage =
          static_cast<float>(con->debug_stats.payload_publish_sent) /
          static_cast<float>(
            con->debug_stats.payload_publish_missed +
            con->debug_stats.payload_publish_sent
          );
        #endif
      }
    }

    if (!any_broker_sent_successfully) {
      return false;
    }

    handler_ptr->flags.SendNow = false;
    handler_ptr->tSavedLastSent = millis();

    return true;
  }

};

#endif

#endif //  USE_MODULE_NETWORK_MQTT