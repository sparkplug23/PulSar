#include "mMQTT.h"

#if defined(USE_MODULE_NETWORK_MQTT)

int8_t mMQTTManager::Tasker(uint8_t function, JsonParserObject obj){ DEBUG_PRINT_FUNCTION_NAME;

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_INIT:
      Init();
    break;    
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
      return 1;
    break;
  }

  if(!tkr_set->Settings.sysopt_system.bit.mqtt_enabled){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  if(!tkr_interface_network->Network_HasExternalConnectivity()) return TASKER_RESULT__MODULE_DISABLED_ID;

  switch(function){
  /************
   * PERIODIC SECTION * 
  *******************/
    case TASK_LOOP:
      MM_EveryLoop();
      CallMQTTSenders();
    break;
    case TASK_MQTT_CONNECTED:
      Load_New_Subscriptions_From_Function_Template();
    break;    
    case TASK_NETWORK_CONNECTED__WIFI:
    case TASK_NETWORK_CONNECTED__ETHERNET:
      Handle__ServiceBrokerConnects_With_Transports();
    break;    
    case TASK_EVERY_50_MSECOND:
      MM_Every50mSecond();
    break;
    case TASK_EVERY_SECOND:  
      Handle__ServiceBrokerConnects_With_Transports();
      MM_EverySecond();  
    break;
    case TASK_EVERY_HOUR:
      Send_LWT_To_Any_Connected_Brokers();
    break;
    case TASK_UPTIME_10_MINUTES:
      flag_uptime_reached_reduce_frequency = true;
    break;     
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
    case TASK_FILESYSTEM__RESET__MODULE_DATA__ID:
      Default_Module();
    break;
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      // Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
    #endif // USE_MODULE_CORE_FILESYSTEM
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_SUBSCRIBE:
      MQTTSubscribe();
    break;
  } // END switch

  return TASKER_RESULT__SUCCESS_ID;

} // END function


void mMQTTManager::Send_LWT_To_Any_Connected_Brokers()
{
  uint8_t br = brokers.size();
  for(uint8_t i = 0; i < br; i++)
  {
    brokers[i]->Send_LWT_Online(); // this does work, but how about wrapping the LWT into the normal status messages? Just the LWT offline would be its own.
  }
}

void mMQTTManager::Start_Connection()
{
  ALOG_HGL(PSTR("Start MQTTConnection with WiFi NEW WAY"));

  mqtt_client = new WiFiClient();

  const int8_t idx = tkr_mqtt->CreateConnection(mqtt_client, MQTT_HOST, MQTT_PORT, CLIENT_TYPE_WIFI_ID);
  if (idx < 0) { ALOG_ERR(PSTR("MQTT CreateConnection failed")); return; }

  MQTTConnection* c = tkr_mqtt->brokers[(size_t)idx];
  if (!c) { ALOG_ERR(PSTR("MQTT broker null")); return; }

  c->SetCredentials(MQTT_USER, MQTT_PASS);
  c->SetReConnectBackoffTime(MQTT_RETRY_SECS);

  uint8_t mac[6]; WiFi.macAddress(mac);
  char client_name[100];
  snprintf_P(client_name, sizeof(client_name), PSTR("%s-%02X:%02X:%02X"),
            tkr_set->Settings.system_name.device, mac[3], mac[4], mac[5]);

  c->SetClientName(client_name);
  c->SetTopicPrefix(tkr_set->Settings.system_name.device);

  return;
}

void mMQTTManager::Handle__ServiceBrokerConnects_With_Transports(void)
{
  ALOG_DBG(PSTR(D_LOG_MQTT "Handle__ServiceBrokerConnects_With_Transports brokers=%u"), brokers.size());

  if(!brokers.size())
  {
    ALOG_DBG(PSTR(D_LOG_MQTT "Handle__ServiceBrokerConnects_With_Transports skipped: no brokers"));
    return;
  }

  for(uint8_t broker_i = 0; broker_i < brokers.size(); broker_i++)
  {
    MQTTConnection* broker = brokers[broker_i];

    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] service begin ptr=%p"), broker_i, broker);

    if(!broker)
    {
      ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] skipped: null broker pointer"), broker_i);
      continue;
    }

    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] state id=%s en=%u allowed=%u host=%s port=%u net=%p pubsub=%p"),
      broker_i,
      broker->id,
      broker->en,
      broker->allowed,
      broker->host_address,
      broker->port,
      broker->network_client,
      broker->pubsub
    );

    if(!broker->en)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] skipped: EN false"), broker_i);
      continue;
    }

    if(!broker->allowed)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] skipped: allowed false"), broker_i);
      continue;
    }

    if(!broker->host_address[0])
    {
      ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] skipped: empty host"), broker_i);
      continue;
    }

    if(broker->network_client && broker->pubsub)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] skipped: already has transport client_type=%u net=%p pubsub=%p"),
        broker_i,
        broker->client_type,
        broker->network_client,
        broker->pubsub
      );
      continue;
    }

    if(broker->network_client && !broker->pubsub)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] partial bind: network_client set but pubsub null"), broker_i);
    }

    if(!broker->network_client && broker->pubsub)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] partial bind: pubsub set but network_client null"), broker_i);
    }

    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] bind required id=%s host=%s"),
      broker_i,
      broker->id,
      broker->host_address
    );

    const bool use_pref_transport = broker->pref_transport_count > 0;
    const uint8_t transport_count = use_pref_transport ? broker->pref_transport_count : broker->transport_count;

    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport source=%s pref_count=%u transport_count=%u selected_count=%u"),
      broker_i,
      use_pref_transport ? "PrefTransport" : "Transport",
      broker->pref_transport_count,
      broker->transport_count,
      transport_count
    );

    if(transport_count == 0)
    {
      ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] skipped: no configured transports"), broker_i);
      continue;
    }

    bool transport_attached = false;

    for(uint8_t transport_i = 0; transport_i < transport_count; transport_i++)
    {
      ConnectionClient_t transport_type = use_pref_transport ? broker->pref_transport[transport_i] : broker->transport[transport_i];

      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] checking transport[%u]=%u"),
        broker_i,
        transport_i,
        transport_type
      );

      Client* client_ptr = nullptr;
      bool client_owned_by_mqtt = false;

      switch(transport_type)
      {
        case CLIENT_TYPE_WIFI_ID:
        {
          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport[%u] type=WiFi"), broker_i, transport_i);

          #ifdef USE_MODULE_NETWORK_WIFI

          if(!tkr_wifi)
          {
            ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] WiFi skipped: tkr_wifi null"), broker_i);
            break;
          }

          if(!tkr_wifi->WiFi_HasExternalConnectivity())
          {
            ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] WiFi skipped: no external connectivity"), broker_i);
            break;
          }

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] WiFi healthy: allocating WiFiClient"), broker_i);

          client_ptr = new WiFiClient();

          if(!client_ptr)
          {
            ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] WiFiClient allocation failed"), broker_i);
            break;
          }

          broker->client_type = CLIENT_TYPE_WIFI_ID;
          client_owned_by_mqtt = true;

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] WiFiClient allocated ptr=%p"), broker_i, client_ptr);

          #else

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] WiFi skipped: USE_MODULE_NETWORK_WIFI not compiled"), broker_i);

          #endif
        }
        break;

        case CLIENT_TYPE_ETHERNET_ID:
        {
          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport[%u] type=Ethernet"), broker_i, transport_i);

          #ifdef USE_MODULE_NETWORK_ETHERNET

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Ethernet compiled but bind path not implemented"), broker_i);

          // Later:
          // if(tkr_eth && tkr_eth->Ethernet_HasExternalConnectivity())
          // {
          //   client_ptr = new EthernetClient();
          //   broker->client_type = CLIENT_TYPE_ETHERNET_ID;
          // }

          #else

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Ethernet skipped: NETWORK_ETHERNET not compiled"), broker_i);

          #endif
        }
        break;

        case CLIENT_TYPE_CELLULAR_ID:
        {
          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport[%u] type=Cellular"), broker_i, transport_i);

          #ifdef USE_MODULE_NETWORK_CELLULAR

          if(!tkr_cellular)
          {
            ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] Cellular skipped: tkr_cellular null"), broker_i);
            break;
          }

          if(!tkr_cellular->Cellular_HasExternalConnectivity())
          {
            ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Cellular skipped: no external connectivity"), broker_i);
            break;
          }

          client_ptr = tkr_cellular->Cellular_GetNetworkClient();

          if(!client_ptr)
          {
            ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] Cellular skipped: no network client"), broker_i);
            break;
          }

          broker->client_type = CLIENT_TYPE_CELLULAR_ID;
          client_owned_by_mqtt = false; // modem/cellular owns TinyGsmClient

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Cellular client ready ptr=%p"), broker_i, client_ptr);

          #else

          ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Cellular skipped: USE_MODULE_NETWORK_CELLULAR not compiled"), broker_i);

          #endif
        }
        break;

        default:
        {
          ALOG_WRN(PSTR(D_LOG_MQTT "Broker[%u] transport[%u] skipped: unknown transport type %u"),
            broker_i,
            transport_i,
            transport_type
          );
        }
        break;
      }

      if(!client_ptr)
      {
        ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport[%u]=%u produced no client"),
          broker_i,
          transport_i,
          transport_type
        );
        continue;
      }

      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] attaching transport type=%u client=%p"),
        broker_i,
        transport_type,
        client_ptr
      );

      broker->network_client = client_ptr;
      broker->SetPubSubClient(client_ptr);

      if(!broker->pubsub)
      {
        ALOG_ERR(PSTR(D_LOG_MQTT "Broker[%u] SetPubSubClient failed: pubsub null"), broker_i);

        if(client_owned_by_mqtt)
        {
          delete client_ptr;
        }

        broker->network_client = nullptr;
        broker->client_type = CLIENT_TYPE_WIFI_ID;

        continue;
      }

      broker->retry = broker->backoff_fast_secs;
      broker->retry_counter_start_value = broker->backoff_fast_secs;
      broker->retry_counter = 1;
      broker->flag_start_reconnect = true;

      transport_attached = true;

      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] transport attached type=%u retry immediate net=%p pubsub=%p"),
        broker_i,
        broker->client_type,
        broker->network_client,
        broker->pubsub
      );

      #ifdef ENABLE_DEBUG_MQTT__INCLUDE_NEW_CONNECTION_SPLASH
      if (tkr_set->Settings.logging.serial_level > LOG_LEVEL_DEBUG)
        broker->Debug_PrintConnectionInfo();
      #endif

      break;
    }

    if(!transport_attached)
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] no transport attached id=%s"), broker_i, broker->id);

      broker->retry_counter = 0;
      broker->flag_start_reconnect = false;
    }
  }
}

void mMQTTManager::EnsureDefaultBroker_FromDefines(void)
{
  if(brokers.size() > 0)
  {
    ALOG_DBG(PSTR(D_LOG_MQTT "Define broker skipped: brokers already configured=%u"), brokers.size());
    return;
  }

  #ifndef MQTT_HOST
  ALOG_WRN(PSTR(D_LOG_MQTT "Define broker skipped: MQTT_HOST not defined"));
  return;
  #endif

  ALOG_WRN(PSTR(D_LOG_MQTT "No MQTT brokers configured, creating broker from defines"));

  MQTTConnection* broker = new MQTTConnection();

  if(!broker)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "Define broker allocation failed"));
    return;
  }

  snprintf(broker->id, sizeof(broker->id), "home");
  snprintf(broker->host_address, sizeof(broker->host_address), "%s", MQTT_HOST);

  #ifdef MQTT_PORT
  broker->port = MQTT_PORT;
  #else
  broker->port = 1883;
  #endif

  broker->en = true;
  broker->allowed = true;

  #ifdef MQTT_USER
  snprintf(broker->user, sizeof(broker->user), "%s", MQTT_USER);
  #else
  broker->user[0] = '\0';
  #endif

  #ifdef MQTT_PASS
  snprintf(broker->password, sizeof(broker->password), "%s", MQTT_PASS);
  #else
  broker->password[0] = '\0';
  #endif

  #ifdef DEVICENAME_CTR
  broker->SetTopicPrefix(DEVICENAME_CTR);
  broker->SetClientName(DEVICENAME_CTR);
  #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
  char tb_name_ctr[64]; // Temporary buffer for modified name
  snprintf(tb_name_ctr, sizeof(tb_name_ctr), "tb_%s", DEVICENAME_CTR);
  broker->SetTopicPrefix(tb_name_ctr);
  broker->SetClientName(tb_name_ctr);
  #endif
  #else
  broker->SetTopicPrefix(tkr_set->Settings.system_name.device);
  broker->SetClientName(tkr_set->Settings.system_name.device);
  #endif

  broker->transport_count = 0;
  broker->pref_transport_count = 0;

  #ifdef USE_MODULE_NETWORK_WIFI
  broker->transport[broker->transport_count++] = CLIENT_TYPE_WIFI_ID;
  broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_WIFI_ID;
  #endif

  #ifdef USE_MODULE_NETWORK_ETHERNET
  broker->transport[broker->transport_count++] = CLIENT_TYPE_ETHERNET_ID;
  broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_ETHERNET_ID;
  #endif

  #if !defined(USE_MODULE_NETWORK_WIFI) && !defined(USE_MODULE_NETWORK_ETHERNET) && defined(USE_MODULE_NETWORK_CELLULAR)
  broker->transport[broker->transport_count++] = CLIENT_TYPE_CELLULAR_ID;
  broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_CELLULAR_ID;
  #endif

  if(broker->transport_count == 0)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "Define broker failed: no compiled transport"));
    delete broker;
    return;
  }

  broker->retry = broker->backoff_fast_secs;
  broker->retry_counter_start_value = broker->backoff_fast_secs;
  broker->retry_counter = 0;
  broker->flag_start_reconnect = false;

  brokers.push_back(broker);

  ALOG_DBG(
    PSTR(D_LOG_MQTT "Define broker added id=%s host=%s port=%u transports=%u"),
    broker->id,
    broker->host_address,
    broker->port,
    broker->transport_count
  );
}


/**
 * @brief Should be called from Init now, its the new way of setting up as a new device with default settings
 * 
 */
void mMQTTManager::Default_Module()
{
  ALOG_DBG(PSTR(D_LOG_MQTT "Default_Module"));
  
  // memset((uint8_t*)&dt, 0, sizeof(dt));
  
  dt.ifchanged_secs = SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS; // ifchanged etc timing should be moved into mqtt substruct
  dt.ifchanged_json_level = JSON_LEVEL_IFCHANGED; //default
  dt.teleperiod_secs = 120;
  dt.teleperiod_json_level = JSON_LEVEL_DETAILED; //default
  dt.mqtt_retain = 1;// = JSON_METHOD_SHORT; //default
  dt.configperiod_secs = SEC_IN_HOUR;

  #ifdef ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
  dt.options.unified_module_interface_reporting__suppress_submodule_configperiod = 1;
  dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod = 1;
  dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod = 1;
  #else
  dt.options.unified_module_interface_reporting__suppress_submodule_configperiod = 0;
  dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod = 0;
  dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod = 0;
  #endif

  strlcpy(test, "TEST MESSAGE FROM INIT", sizeof(test));

  uint8_t idx = 0;

  /**
   * @brief Init with a wifi one
   **/
  Default_Module__Connection_WiFi();

  
  // strlcpy(dt.connection[idx].host_address, MQTT_HOST, sizeof(dt.connection[idx].host_address));
  // dt.connection[idx].port = MQTT_PORT;
  // strlcpy(dt.connection[idx].user, MQTT_USER, sizeof(dt.connection[idx].user));
  // strlcpy(dt.connection[idx].pwd, MQTT_PASS, sizeof(dt.connection[idx].pwd));
  // dt.connection[idx].retry = MQTT_RETRY_SECS;
  // snprintf_P(dt.connection[idx].client, sizeof(dt.connection[idx].client), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str());
  // strlcpy(dt.connection[idx].prefixtopic, tkr_set->Settings.system_name.device, sizeof(dt.connection[idx].prefixtopic));
  // dt.connection[idx].status = 1;
    
  // ALOG_DBG(PSTR("Ghost_address: %s"), dt.connection[idx].host_address);
  // ALOG_DBG(PSTR("port: %d"), dt.connection[idx].port);
  // ALOG_DBG(PSTR("user: %s"), dt.connection[idx].user);
  // ALOG_DBG(PSTR("pwd: %s"), dt.connection[idx].pwd);
  // ALOG_DBG(PSTR("retry: %d"), dt.connection[idx].retry);
  // ALOG_DBG(PSTR("client: %s"), dt.connection[idx].client);
  // ALOG_DBG(PSTR("prefixtopic: %s"), dt.connection[idx].prefixtopic);
  // ALOG_DBG(PSTR("status: %d"), dt.connection[idx].status);

  #ifdef ENABLE_FEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
  rate_limit_send_delay = ENABLE_FEATURE__MQTT_ENABLE_SENDING_LIMIT_MS;
  #endif

}


void mMQTTManager::Default_Module__Connection_WiFi()
{

  uint8_t broker_index_next = brokers.size();

  uint8_t idx = broker_index_next; //size with index from 0, will already give next slot

  // tkr_mqtt->CreateConnection(mqtt_client, MQTT_HOST, MQTT_PORT, CLIENT_TYPE_WIFI_ID, tkr_mqtt->dt.connection[idx].client, tkr_mqtt->dt.connection[idx].prefixtopic);
      

  // MQTTConnection* con = brokers[idx];

  // strlcpy(con->host_address, MQTT_HOST, sizeof(con->host_address));
  // con->port = MQTT_PORT;
  // strlcpy(con->user, MQTT_USER, sizeof(con->user));
  // strlcpy(con->pwd, MQTT_PASS, sizeof(con->pwd));
  // con->retry = MQTT_RETRY_SECS;
  // snprintf_P(con->client_name, sizeof(con->client_name), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str());
  // strlcpy(con->prefix_topic, tkr_set->Settings.system_name.device, sizeof(con->prefix_topic));
  // con->status = 1;
    
  // ALOG_DBG(PSTR("Ghost_address: %s"), con->host_address);
  // ALOG_DBG(PSTR("port: %d"), con->port);
  // ALOG_DBG(PSTR("user: %s"), con->user);
  // ALOG_DBG(PSTR("pwd: %s"), con->pwd);
  // ALOG_DBG(PSTR("retry: %d"), con->retry);
  // ALOG_DBG(PSTR("client: %s"), con->client_name);
  // ALOG_DBG(PSTR("prefixtopic: %s"), con->prefix_topic);
  // ALOG_DBG(PSTR("status: %d"), con->status);



}


#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

void mMQTTManager::Save_Module()
{
  ALOG_DBG(PSTR(D_LOG_MQTT "Save_Module"));
  tkr_mfile->ByteFile_Save("/mqtt" FILE_EXTENSION_BIN, (uint8_t*)&dt, sizeof(dt));
}

void mMQTTManager::Load_Module(bool erase)
{
  ALOG_DBG(PSTR(D_LOG_MQTT "Load_Module"));
  // tkr_mfile->ByteFile_Load("/mqtt" FILE_EXTENSION_BIN, (uint8_t*)&dt, sizeof(dt));
}

#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
#endif // USE_MODULE_CORE_FILESYSTEM


void mMQTTManager::MQTTSubscribe()
{

  // Probably needs moved into main parsing, if this does not need redoing with each new connection, parsing it once may be enough. Or else, reparsing again.
  #ifdef USE_FUNCTION_TEMPLATE
  uint16_t progmem_size = sizeof(FUNCTION_TEMPLATE);
  char buffer[progmem_size];
  memcpy_P(buffer,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));

  JsonParser parser(buffer);
  JsonParserObject rootObj = parser.getRootObject();   
  if (!rootObj) 
  {
    ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
    return;
  } 
  else
  {
    // ALOG_DBG(PSTR("Deserialization Success with \"%s\""), buffer);
  }
  
  JsonParserToken jtok = 0;
  JsonParserObject obj1 = 0;
  char item_name[100] = {0};

  // ALOG_DBG(PSTR("ObjectNameID size %d"), rootObj["ObjectNameID"].size());

  if(jtok = rootObj["MQTTSubscribe"])
  {

    for(auto val : jtok.getArray()) 
    {
      const char* val_string = val.getStr();
      ALOG_DBG(PSTR("val = %s"), val_string);
  
      Subscribe(val_string, 0);

    }
      
  }

  #endif // USE_FUNCTION_TEMPLATE



}

/**
 * @brief 
 * New methods to get the periods indirectly, so other logic can modify them
 * SubModule: When interface is set to periority, the submodule values will be supressed
 */
uint16_t mMQTTManager::GetConfigPeriod()
{
  return dt.configperiod_secs;
}
uint16_t mMQTTManager::GetConfigPeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_configperiod)
    return 3600; // disable submodule config
  else
    return dt.configperiod_secs;
}
uint16_t mMQTTManager::GetTelePeriod()
{
  return dt.teleperiod_secs;
}
uint16_t mMQTTManager::GetTelePeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod)
    return 3600; // reduced to once an hour
  else
    return dt.teleperiod_secs;  
}
uint16_t mMQTTManager::GetIfChangedPeriod()
{
  return dt.ifchanged_secs;
}
uint16_t mMQTTManager::GetIfChangedPeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod)
    return 0; // disable submodule ifchanged
  else
    return dt.ifchanged_secs;  
}


#ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
void mMQTTManager::MqttPublishPayloadPrefixTopic_P(uint32_t prefix, const char* subtopic, const char* payload, uint32_t binary_length, bool retained) {

ALOG_DBG(PSTR("MqttPublishPayloadPrefixTopic_P"));

/*
  Publish <prefix>/<device>/<RESULT or <subtopic>> payload string or binary when binary_length set with optional retained

  prefix 0 = cmnd using subtopic
  prefix 1 = stat using subtopic
  prefix 2 = tele using subtopic
  prefix 4 = cmnd using subtopic or RESULT
  prefix 5 = stat using subtopic or RESULT
  prefix 6 = tele using subtopic or RESULT
*/
  SHOW_FREE_MEM(PSTR("MqttPublishPayloadPrefixTopic_P"));
/*
  char romram[64];                      // Claim 64 bytes from 4k stack
  snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings->flag.mqtt_response) ? S_RSLT_RESULT : subtopic);  // SetOption4 - Switch between MQTT RESULT or COMMAND
  UpperCase(romram, romram);

  prefix &= 3;
  char stopic[TOPSZ];                   // Claim TOPSZ bytes from 4k stack
  GetTopic_P(stopic, prefix, TasmotaGlobal.mqtt_topic, romram);
  MqttPublishPayload(stopic, payload, binary_length, retained);
*/
  // Reduce important stack usage by 200 bytes but adding 52 bytes code
  // char *romram = (char*)malloc(64);     // Claim 64 bytes from 20k heap
  // strcpy_P(romram, ((prefix > 3) && !Settings->flag.mqtt_response) ? S_RSLT_RESULT : subtopic);
  // UpperCase(romram, romram);

  // prefix &= 3;
  // char *htopic = (char*)malloc(TOPSZ);  // Claim TOPSZ bytes from 16k heap
  // GetTopic_P(htopic, prefix, TasmotaGlobal.mqtt_topic, romram);
  // char stopic[strlen_P(htopic) +1];     // Claim only strlen_P bytes from 4k stack
  // strcpy_P(stopic, htopic);
  // free(htopic);                         // Free 16k heap from TOPSZ bytes
  // free(romram);                         // Free 16k heap from 64 bytes
  // MqttPublishPayload(stopic, payload, binary_length, retained);

  Publish("status/debug/test", payload, false);

  

}

void mMQTTManager::MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic, bool retained) {
ALOG_DBG(PSTR("MqttPublishPrefixTopic_P"));
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string with optional retained
  SHOW_FREE_MEM(PSTR("MqttPublishPrefixTopic_P"));

  MqttPublishPayloadPrefixTopic_P(prefix, subtopic, ResponseData(), 0, retained);
}

void mMQTTManager::MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, bool retained) {
ALOG_DBG(PSTR("MqttPublishPrefixTopicRulesProcess_P"));
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string with optional retained
  //   then process rules
  SHOW_FREE_MEM(PSTR("MqttPublishPrefixTopicRulesProcess_P"));

  MqttPublishPrefixTopic_P(prefix, subtopic, retained);
  // XdrvRulesProcess(0);
}

void mMQTTManager::MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string no retained
  //   then process rules
  MqttPublishPrefixTopicRulesProcess_P(prefix, subtopic, false);
}


void mMQTTManager::MqttPublishPowerBlinkState(uint32_t device) {
  char scommand[33];

  // if ((device < 1) || (device > TasmotaGlobal.devices_present)) {
  //   device = 1;
  // }
  #ifdef USE_MODULE_DRIVERS_RELAY
  tkr_sup->Response_P(PSTR("{\"%s\":\"" D_BLINK " %s\"}"), 
    tkr_sup->GetPowerDevice(scommand, device, sizeof(scommand), tkr_set->Settings.sysopt_system.bit.device_index_enable), tkr_sup->GetStateText(bitRead(tkr_relay->rt.bitpacked.blink_mask, device -1)));  // SetOption26 - Switch between POWER or POWER1
  #endif 
  
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, "Power");//S_RSLT_POWER);
}

#endif // ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS


void mMQTTManager::Load_New_Subscriptions_From_Function_Template()
{

  DEBUG_LINE_HERE

  // if(data_buffer.requestLock(GetModuleUniqueID())) {
  //   ALOG_ERR(PSTR("Load_New_Subscriptions_From_Function_Template could not get data buffer lock"));
  //   return;
  // }

  // #ifdef USE_FUNCTION_TEMPLATE  
  // data_buffer.ClearDeep();
  // memcpy_P(data_buffer.payload.ctr, FUNCTION_TEMPLATE, sizeof(FUNCTION_TEMPLATE));
  // data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
  // ALOG_DBG(PSTR(DEBUG_INSERT_PAGE_BREAK  "Load_New_Subscriptions_From_Function_Template READ = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);
  // #endif //USE_FUNCTION_TEMPLATE

  // JsonParser parser(data_buffer.payload.ctr);
  // JsonParserObject obj = parser.getRootObject();   
  // JsonParserToken jtok = 0; 

  // if(jtok = obj["MQTTSubscribe"])
  // {
  //   if(jtok.isArray())
  //   {

  //     JsonParserArray arrobj = jtok;
  //     for(auto v : arrobj) 
  //     {
  //       const char* new_topic = v.getStr();
  //       ALOG_DBM(PSTR("New Subscribe = \"%s\""), new_topic);
        

  //     }

  //   }

  // }

  // data_buffer.releaseLock();

}


void mMQTTManager::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  ALOG_DBG(PSTR(D_LOG_MQTT "mMQTTManager::parse_JSONCommand"));

  
  JsonParserObject jobj = 0; 

  if(!(jobj = obj[D_MODULE_NETWORK_MQTT_CTR].getObject()))
  {
    // ALOG_ERR(PSTR(D_LOG_MQTT "No MQTT object found"));
    return;
  }
  
  if(jtok = jobj["Brokers"])
  {

    ALOG_DBG(PSTR(D_LOG_MQTT "Parsing Brokers from JSONCommand"));

    uint8_t broker_i = 0;

    for(auto broker_jtok : jtok.getArray())
    {
      if(broker_i >= MQTT_MAX_BROKERS)
      {
        ALOG_WRN(PSTR(D_LOG_MQTT "Brokers ignored, max reached %u"), MQTT_MAX_BROKERS);
        break;
      }

      while(brokers.size() <= broker_i)
      {
        brokers.push_back(new MQTTConnection());
      }

      subparse_JSONCommand__Broker(broker_jtok.getObject(), broker_i);

      broker_i++;
    }

    // Remove stale broker objects if this JSON has fewer brokers than before.
    while(brokers.size() > broker_i)
    {
      MQTTConnection* broker = brokers.back();

      if(broker)
      {
        if(broker->pubsub)
        {
          delete broker->pubsub;
          broker->pubsub = nullptr;
        }

        if(broker->network_client)
        {
          delete broker->network_client;
          broker->network_client = nullptr;
        }

        delete broker;
      }

      brokers.pop_back();
    }

    ALOG_DBG(PSTR(D_LOG_MQTT "Brokers loaded %u"), brokers.size());
  }
  else
  {
    // ALOG_DBG(PSTR(D_LOG_MQTT "No Brokers found in JSONCommand"));
    // return;
  }
  

  ALOG_DBG(PSTR(D_LOG_MQTT "mMQTTManager::parse_JSONCommand2"));

  uint8_t connection_idx = 0; // should be like Segments, assumes 0 when only 1

  if(jtok = obj["MQTT"].getObject()["RetrySecs"])
  {
    // dt.connection[0].retry = jtok.getInt();
  }

  if(jtok = obj["MQTTSend"])
  {
    ALOG_DBG(PSTR("mMQTTManager::parse_JSONCommand MQTTSend"));
    
    JsonParserToken jtok_topic = jtok.getObject()["Topic"];
    JsonParserToken jtok_payload = jtok.getObject()["Payload"];

    ALOG_DBG(PSTR("mMQTTManager::parse_JSONCommand MQTTSend %d"),jtok_topic.size());
    ALOG_DBG(PSTR("mMQTTManager::parse_JSONCommand MQTTSend %d"),jtok_payload.size());
   
    char topic_ctr[100] = {0};
    char payload_ctr[300] = {0};

    snprintf(topic_ctr, sizeof(topic_ctr), jtok_topic.getStr());
    snprintf(payload_ctr, sizeof(payload_ctr), jtok_payload.getStr());

    char buffer_escaped[200] = {0};
    uint8_t len  = 0;
    for(int i=0;i<strlen(payload_ctr);i++){
      if(payload_ctr[i] == '~'){
        len+=sprintf(buffer_escaped+len,"\"");
      }else{    
        buffer_escaped[len++] = payload_ctr[i];
      }
    }

    ALOG_DBG(PSTR("Topic=%s"),topic_ctr);
    ALOG_DBG(PSTR("Payload=%s"),payload_ctr);
    ALOG_DBG(PSTR("buffer_escaped=%s"),buffer_escaped);
    
    // publish_device(jtok_topic.getStr(),jtok_payload.getStr(),false);
    brokers[0]->pubsub->publish(topic_ctr,buffer_escaped,false);

  }


  if(jtok = obj["MQTT"].getObject()["StatusAll"]) //change all to be value
  {    
    tkr->Tasker_Interface(TASK_MQTT_STATUS_REFRESH_SEND_ALL);
  }


  JsonParserToken jtok_sub = 0; 
  if(jtok = obj["MQTTUpdateSeconds"])
  {
    if(jtok_sub = jtok.getObject()["IfChanged"])
    {
      dt.ifchanged_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds IfChanged %d"), dt.ifchanged_secs);
    }
    if(jtok_sub = jtok.getObject()["TelePeriod"])
    {
      dt.teleperiod_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds TelePeriod %d"), dt.teleperiod_secs);
    }
    if(jtok_sub = jtok.getObject()["ConfigPeriod"])
    {
      dt.configperiod_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds ConfigPeriod %d"), dt.configperiod_secs);
    }
    tkr->Tasker_Interface(TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD);
  }



}//end function


void mMQTTManager::subparse_JSONCommand__Broker(JsonParserObject obj, uint8_t broker_i)
{

  ALOG_DBG(PSTR(D_LOG_MQTT "subparse_JSONCommand__Broker %u"), broker_i);

  if(!obj){ return; }
  if(broker_i >= MQTT_MAX_BROKERS){ return; }
  if(broker_i >= brokers.size()){ return; }

  MQTTConnection* broker = brokers[broker_i];

  if(!broker){ return; }

  JsonParserToken jtok = 0;

  // Reset config fields, but do not delete runtime object.
  broker->en = true;
  broker->allowed = false;

  broker->host_address[0] = '\0';
  broker->user[0] = '\0';
  broker->password[0] = '\0';
  broker->prefix_topic[0] = '\0';
  broker->client_name[0] = '\0';

  broker->port = 1883;

  broker->backoff_fast_secs = 5;
  broker->backoff_periodic_secs = 10;
  broker->backoff_max_secs = 60;

  broker->transport_count = 0;
  broker->pref_transport_count = 0;

  broker->outgoing_level = 3;
  broker->outgoing_limiter_ms = 0;
  broker->keepalive_secs = 60;

  ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] reset to defaults"), broker_i);

  if(jtok = obj["Id"])
  {
    snprintf(broker->id, sizeof(broker->id), "%s", jtok.getStr());
    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Id=%s"), broker_i, broker->id);
  }

  if(jtok = obj["EN"])
  {
    broker->en = jtok.getBool();
    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] EN=%u"), broker_i, broker->en);
  }

  if(jtok = obj["Host"])
  {
    snprintf(broker->host_address, sizeof(broker->host_address), "%s", jtok.getStr());
    ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] Host=%s"), broker_i, broker->host_address);
  }

  if(jtok = obj["Port"])
  {
    broker->port = jtok.getInt();
  }

  if(jtok = obj["User"])
  {
    snprintf(broker->user, sizeof(broker->user), "%s", jtok.getStr());
  }

  if(jtok = obj["Password"])
  {
    snprintf(broker->password, sizeof(broker->password), "%s", jtok.getStr());
  }

  if(jtok = obj["TopicPrefix"])
  {
    snprintf(broker->prefix_topic, sizeof(broker->prefix_topic), "%s", jtok.getStr());
  }

  if(jtok = obj["ClientName"])
  {
    snprintf(broker->client_name, sizeof(broker->client_name), "%s", jtok.getStr());
  }

  if(jtok = obj["Backoff"])
  {
    uint8_t backoff_i = 0;

    for(auto val : jtok.getArray())
    {
      if(backoff_i == 0)
      {
        broker->backoff_fast_secs = val.getInt();
      }
      else if(backoff_i == 1)
      {
        broker->backoff_periodic_secs = val.getInt();
      }
      else if(backoff_i == 2)
      {
        broker->backoff_max_secs = val.getInt();
        break;
      }

      backoff_i++;
    }
  }

  if(jtok = obj["Transport"])
  {
    broker->transport_count = 0;

    for(auto val : jtok.getArray())
    {
      if(broker->transport_count >= MQTT_TRANSPORT_LIST_MAX){ break; }

      const char* transport_ctr = val.getStr();

      if(!transport_ctr){ continue; }

      if(strcasecmp(transport_ctr, "Ethernet") == 0)
      {
        broker->transport[broker->transport_count++] = CLIENT_TYPE_ETHERNET_ID;
      }
      else if(strcasecmp(transport_ctr, "WiFi") == 0)
      {
        broker->transport[broker->transport_count++] = CLIENT_TYPE_WIFI_ID;
      }
      else if(strcasecmp(transport_ctr, "Cellular") == 0)
      {
        broker->transport[broker->transport_count++] = CLIENT_TYPE_CELLULAR_ID;
      }
      else
      {
        ALOG_WRN(PSTR(D_LOG_MQTT "Broker[%u] unknown transport %s"), broker_i, transport_ctr);
      }
    }
  }

  if(jtok = obj["PrefTransport"])
  {
    broker->pref_transport_count = 0;

    for(auto val : jtok.getArray())
    {
      if(broker->pref_transport_count >= MQTT_TRANSPORT_LIST_MAX){ break; }

      const char* transport_ctr = val.getStr();

      if(!transport_ctr){ continue; }

      if(strcasecmp(transport_ctr, "Ethernet") == 0)
      {
        broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_ETHERNET_ID;
      }
      else if(strcasecmp(transport_ctr, "WiFi") == 0)
      {
        broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_WIFI_ID;
      }
      else if(strcasecmp(transport_ctr, "Cellular") == 0)
      {
        broker->pref_transport[broker->pref_transport_count++] = CLIENT_TYPE_CELLULAR_ID;
      }
      else
      {
        ALOG_WRN(PSTR(D_LOG_MQTT "Broker[%u] unknown pref transport %s"), broker_i, transport_ctr);
      }
    }
  }

  if(jtok = obj["OutgoingLevel"])
  {
    broker->outgoing_level = jtok.getInt();
  }

  if(jtok = obj["OutgoingLimiterMs"])
  {
    broker->outgoing_limiter_ms = jtok.getInt();
  }

  if(broker->pref_transport_count == 0)
  {
    broker->pref_transport_count = broker->transport_count;

    for(uint8_t i=0; i<broker->transport_count; i++)
    {
      broker->pref_transport[i] = broker->transport[i];
    }
  }

  if(broker->backoff_periodic_secs < broker->backoff_fast_secs)
  {
    broker->backoff_periodic_secs = broker->backoff_fast_secs;
  }

  if(broker->backoff_max_secs < broker->backoff_periodic_secs)
  {
    broker->backoff_max_secs = broker->backoff_periodic_secs;
  }

  broker->retry = broker->backoff_fast_secs;
  broker->retry_counter_start_value = broker->backoff_fast_secs;

  broker->allowed = broker->en && broker->host_address[0];

  ALOG_DBG(PSTR(D_LOG_MQTT "Broker[%u] id=%s en=%u host=%s port=%u allowed=%u"),
    broker_i,
    broker->id,
    broker->en,
    broker->host_address,
    broker->port,
    broker->allowed
  );

  if (tkr_set->Settings.logging.serial_level > LOG_LEVEL_DEBUG)
    broker->Debug_PrintConnectionInfo();
}

void mMQTTManager::Init(void)
{
  Default_Module();
  
}


bool mMQTTManager::AnyBrokerConnected(void)
{
  if(!brokers.size())
  {
    return false;
  }

  for(uint8_t broker_i = 0; broker_i < brokers.size(); broker_i++)
  {
    MQTTConnection* broker = brokers[broker_i];

    if(!broker) { continue; }
    if(!broker->en) { continue; }
    if(!broker->allowed) { continue; }
    if(!broker->uptime_seconds) { continue; }
    if(broker->downtime_counter != 0) { continue; }
    if(!broker->MqttIsConnected()) { continue; }

    return true;
  }

  return false;
}


void mMQTTManager::CallMQTTSenders()
{
  if(AnyBrokerConnected())
  {
    tkr->Tasker_Interface(TASK_MQTT_SENDER);
  }
}


/**
 * @brief Transmit on any connected brokers with connection checking
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean mMQTTManager::Publish(const char* topic, const char* payload, boolean retained)
{
  DEBUG_LINE_HERE3
  Serial.println("mMQTTManager::Publish"); Serial.flush();
  DEBUG_LINE_HERE3
  if(brokers.size())
  {
    DEBUG_LINE_HERE3
    if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    {
      DEBUG_LINE_HERE3
      for (auto& broker : brokers)
      {
        DEBUG_LINE_HERE3
        if(broker->pubsub->connected())
        {
          DEBUG_LINE_HERE3
          return broker->publish_device(topic,payload,retained);
        }
        DEBUG_LINE_HERE3
      }
    }
  }
  DEBUG_LINE_HERE3

  return false; // No broker connected or not available, return false

}



/**
 * @brief Transmit on any connected brokers with connection checking
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean mMQTTManager::Subscribe(const char* topic, uint8_t qos)
{

  ALOG_INF(PSTR(D_LOG_MQTT "Subscribe %s"), topic);
  
  if(brokers.size()) 
  {
    for (auto& broker : brokers)
    {
      if(broker->pubsub->connected())
      {
        return broker->pubsub->subscribe(topic);
      }
    }
  }

}

#endif // USE_MODULE_NETWORK_MQTT