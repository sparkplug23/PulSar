#include "mMQTT.h"

#if defined(USE_MODULE_NETWORK_MQTT)

const char* mMQTT::PM_MODULE_NETWORK_MQTT_CTR = D_MODULE_NETWORK_MQTT_CTR;
const char* mMQTT::PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR = D_MODULE_NETWORK_MQTT_FRIENDLY_CTR;


int8_t mMQTT::Tasker(uint8_t function, JsonParserObject obj){ DEBUG_PRINT_FUNCTION_NAME;

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      Init();
    break;
  }

  if(!pCONT_set->Settings.flag_system.mqtt_enabled){ return 0; }

  switch(function){
  /************
   * PERIODIC SECTION * 
  *******************/
    case FUNC_LOOP:
      MM_EveryLoop();
      CallMQTTSenders();
    break;
    case FUNC_MQTT_CONNECTED:
    ALOG_ERR(PSTR("MQTT_CONNECTED hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"));
      Load_New_Subscriptions_From_Function_Template();
    break;
    case FUNC_NETWORK_CONNECTION_ESTABLISHED:

      /**
       * @brief If status is down, and awaiting connection, immediate try reconnect
       * */
      if(brokers.size())   
        if(brokers[0]->retry_counter)
          brokers[0]->retry_counter = 1; //retry immediate
          ALOG_HGL(PSTR(D_LOG_PUBSUB "retry_counter IMMEDIATE = %d"),brokers[0]->retry_counter);

    break;
    case FUNC_EVERY_50_MSECOND:
      MM_Every50mSecond();

      // if(brokers.size())   
      //   if(brokers[0]->retry_counter)
      //     Serial.printf(D_LOG_PUBSUB "retry_counter = %d\n\r", brokers[0]->retry_counter);

    break;
    case FUNC_EVERY_SECOND:      
      MM_EverySecond();
    break;
    case FUNC_EVERY_HOUR:
      if(brokers.size())
      {
        brokers[0]->Send_LWT_Online(); // this does work, but how about wrapping the LWT into the normal status messages? Just the LWT offline would be its own.
      }
    break;
    case FUNC_UPTIME_10_MINUTES:
      flag_uptime_reached_reduce_frequency = true;
    break;     
  } // END switch

} // END function


void mMQTT::Load_New_Subscriptions_From_Function_Template()
{

  DEBUG_LINE_HERE

  #ifdef USE_FUNCTION_TEMPLATE  
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr, FUNCTION_TEMPLATE, sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK  "Load_New_Subscriptions_From_Function_Template READ = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);
  #endif //USE_FUNCTION_TEMPLATE

  JsonParser parser(data_buffer.payload.ctr);
  JsonParserObject obj = parser.getRootObject();   
  JsonParserToken jtok = 0; 

  if(jtok = obj["MQTTSubscribe"])
  {
    if(jtok.isArray())
    {

      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) 
      {
        const char* new_topic = v.getStr();
        ALOG_INF(PSTR("New Subscribe = \"%s\""), new_topic);
        

      }

    }

  }
}


void mMQTT::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;


  if(jtok = obj["MQTTSend"])
  {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT::parse_JSONCommand MQTTSend"));
    #endif //
    JsonParserToken jtok_topic = jtok.getObject()["Topic"];
    JsonParserToken jtok_payload = jtok.getObject()["Payload"];

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT::parse_JSONCommand MQTTSend %d"),jtok_topic.size());
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT::parse_JSONCommand MQTTSend %d"),jtok_payload.size());
    #endif // 

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

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("Topic=%s"),topic_ctr);
    AddLog(LOG_LEVEL_TEST,PSTR("Payload=%s"),payload_ctr);
    AddLog(LOG_LEVEL_TEST,PSTR("buffer_escaped=%s"),buffer_escaped);
    #endif // ENABLE_LOG_LEVEL_INFO

    // ppublish(jtok_topic.getStr(),jtok_payload.getStr(),false);
    brokers[0]->pubsub->publish(topic_ctr,buffer_escaped,false);

  }


  if(jtok = obj["MQTT"].getObject()["StatusAll"]) //change all to be value
  {    
    pCONT->Tasker_Interface(FUNC_MQTT_STATUS_REFRESH_SEND_ALL);
  }


}//end function


void mMQTT::Init(void)
{

  memset(pCONT_set->Settings.mqtt.client_name,0,sizeof(pCONT_set->Settings.mqtt.client_name));
  snprintf_P(pCONT_set->Settings.mqtt.client_name,sizeof(pCONT_set->Settings.mqtt.client_name)-1,PSTR("%s-%s"),pCONT_set->Settings.system_name.device,WiFi.macAddress().c_str());

  setprefixtopic(pCONT_set->Settings.system_name.device);

  memset(pCONT_set->Settings.mqtt.lwt_topic,0,sizeof(pCONT_set->Settings.mqtt.lwt_topic));
  snprintf_P(pCONT_set->Settings.mqtt.lwt_topic,sizeof(pCONT_set->Settings.mqtt.lwt_topic)-1,PSTR("%s/status/LWT"),pCONT_set->Settings.system_name.device);

}


void mMQTT::CallMQTTSenders()
{

  if(brokers.size())
  {
    if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    {
      pCONT->Tasker_Interface(FUNC_MQTT_SENDER);
    }
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
boolean mMQTT::Publish(const char* topic, const char* payload, boolean retained)
{
  
  if(brokers.size())
  {
    if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    {
      for (auto& broker : brokers)
      {
        if(broker->pubsub->connected())
        {
          return broker->ppublish(topic,payload,retained);
        }
      }
    }
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
boolean mMQTT::Subscribe(const char* topic, uint8_t qos)
{
  
  if(brokers.size())
  {
    // if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    // {
      for (auto& broker : brokers)
      {
        if(broker->pubsub->connected())
        {
          return broker->pubsub->subscribe(topic);
        }
      }
    }
  // }

}




void mMQTT::setprefixtopic(const char* _prefixtopic){
  memset(pCONT_set->Settings.mqtt.prefixtopic,0,sizeof(pCONT_set->Settings.mqtt.prefixtopic));
  strncpy(pCONT_set->Settings.mqtt.prefixtopic,_prefixtopic,strlen(_prefixtopic));
}


char* mMQTT::TopicFormatted(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, char* buffer, uint8_t buflen)
{

  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  //can be replaced with a function call
  switch(topic_type_id){
    case MQTT_TOPIC_TYPE_SYSTEM_ID: break; // nothing
    case MQTT_TOPIC_TYPE_IFCHANGED_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_IFCHANGED_CTR); break;
    case MQTT_TOPIC_TYPE_ROC1M_ID: sprintf(topic_id_ctr,"%s/","roc1m"); break;
    case MQTT_TOPIC_TYPE_ROC10M_ID: sprintf(topic_id_ctr,"%s/","roc10m"); break;
    case MQTT_TOPIC_TYPE_TELEPERIOD_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_TELEPERIOD_CTR); break;
    case MQTT_TOPIC_TYPE__DEBUG__ID: sprintf(topic_id_ctr,"%s/","debug"); break;
    default: sprintf(topic_id_ctr,"%s/","ERROR"); break;
  }

  snprintf_P(buffer, buflen, "%s/%s/%s%S", D_TOPIC_STATUS, module_name, topic_id_ctr, topic_postfix);  //PSTR may broke this?

  return buffer;

}


#ifdef ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
// template<typename T>
// void mMQTT::MQTTHandler_AddWebURL(T& class_ptr, uint16_t class_id, handler<T>* handle)
// {

//   PGM_P module_ctr = pCONT->GetModuleFriendlyName(class_id);

//   char uri_buffer[70] = {0};
//   snprintf(uri_buffer, sizeof(uri_buffer), "/mqtt/%s/%S", D_TOPIC_STATUS, module_ctr);
//   pCONT_web->server->on(uri_buffer, HTTP_GET, [class_ptr, handle](AsyncWebServerRequest *request)
//   {
//     char handle_url[100] = {0};
//     // for(auto& handle:mqtthandler_list)
//     // {      
//     // PGM_P module_ctr = pCONT->GetModuleFriendlyName(class_id);

//     TopicFormatted(  class_ptr->GetModuleFriendlyName(),
//             handle->topic_type,
//             handle->postfix_topic, handle_url, sizeof(handle_url));  

//     ALOG_INF(PSTR("handle_url=%s"), handle_url);
//     ALOG_INF(PSTR("%s in %s?"), handle->postfix_topic, request->url().c_str());      

//     const String& incoming_uri = request->url();
//     if(incoming_uri.indexOf(handle_url) > 0)
//     {
//       ALOG_INF(PSTR("%s"), request->url().c_str());            
//       uint8_t fSendPayload = CALL_MEMBER_FUNCTION(class_ptr, handle->ConstructJSON_function)(handle->json_level, true);
//       ALOG_INF(PSTR("data_buffer.payload.ctr=%s"), data_buffer.payload.ctr);
//       request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); 
//       // break; // to stop accidental double matches, only respond once
//     }
//     // }
//   });

// }
#endif // ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED




/**
 * @brief This is not thread safe
 * 
 * @return const char* 
 */
const char* mMQTT::GetState_PCtr(int8_t state)
{
  switch(state){
    case MQTT_CONNECTION_TIMEOUT:       return PSTR("Connection Timeout");
    case MQTT_CONNECTION_LOST:          return PSTR("Connecttion Lost");
    case MQTT_CONNECT_FAILED:           return PSTR("Connect Failed");
    case MQTT_DISCONNECTED:             return PSTR("Disconnected");
    case MQTT_CONNECTED:                return PSTR("Connected");
    case MQTT_CONNECT_BAD_PROTOCOL:     return PSTR("Bad Protocol");
    case MQTT_CONNECT_BAD_CLIENT_ID:    return PSTR("Bad Client ID");
    case MQTT_CONNECT_UNAVAILABLE:      return PSTR("Unavailable");
    case MQTT_CONNECT_BAD_CREDENTIALS:  return PSTR("Bad Credentials");
    case MQTT_CONNECT_UNAUTHORIZED:     return PSTR("Unauthorized");
  }
}

#endif // USE_MODULE_NETWORK_MQTT