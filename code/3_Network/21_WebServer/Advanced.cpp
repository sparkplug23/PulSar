#include "_WebServer.h"


#ifdef USE_MODULE_NETWORK_WEBSERVER



#ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

void mWebServer::HandlePage_DebugTaskerMetrics(AsyncWebServerRequest* request)
{
  handleStaticContent(
    request,
    F("/adv/tasker"),
    200,
    FPSTR(CONTENT_TYPE_HTML),
    PAGE_debug_telemetry_metrics_web,
    PAGE_debug_telemetry_metrics_web_length,
    true
  );
}


void mWebServer::HandleAPI_DebugTaskerMetrics(AsyncWebServerRequest* request)
{
  const String action = request->hasParam("action") ? request->getParam("action")->value() : F("data");

  if(action == F("enable"))
  {
    tkr->metrics.RequestEnable(tkr->GetClassCount());
    tkr->metrics.Touch();

    request->send(200, FPSTR(CONTENT_TYPE_JSON), F("{\"ok\":1}"));
    return;
  }

  if(action == F("disable"))
  {
    tkr->metrics.RequestDisable();

    request->send(200, FPSTR(CONTENT_TYPE_JSON), F("{\"ok\":1}"));
    return;
  }

  if(action == F("reset"))
  {
    tkr->metrics.Touch();
    tkr->metrics.RequestReset();

    request->send(200, FPSTR(CONTENT_TYPE_JSON), F("{\"ok\":1}"));
    return;
  }

  if(action == F("touch"))
  {
    tkr->metrics.Touch();

    request->send(204);
    return;
  }

  if(action == F("snapshot"))
  {
    tkr->metrics.Touch();
    tkr->metrics.RequestSnapshot();

    request->send(200, FPSTR(CONTENT_TYPE_JSON), F("{\"ok\":1}"));
    return;
  }

  if(action == F("data"))
  {
    if(!tkr->metrics.IsSnapshotReady())
    {
      request->send(202, FPSTR(CONTENT_TYPE_JSON), F("{\"ready\":false}"));
      return;
    }

    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->addHeader(F("Cache-Control"), F("no-store"));

    tkr->metrics.WriteSnapshotJSON(*response, *tkr);

    request->send(response);
    return;
  }

  request->send(400, FPSTR(CONTENT_TYPE_JSON), F("{\"error\":\"unknown action\"}"));
}

#endif



#ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS



#ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

bool mWebServer::WebSocket_SendText(AsyncWebSocketClient* client, const char* data, size_t len)
{
  if(!client || !data || !len) return false;

  AsyncWebSocketBuffer buffer(len);

  if(!buffer)
  {
    return false;
  }

  memcpy(buffer.data(), data, len);

  client->text(std::move(buffer));

  return true;
}


bool mWebServer::WebSocket_SendWrappedJSON(AsyncWebSocketClient* client, const char* key, const char* json, size_t json_len)
{
  if(!client || !key || !key[0]) return false;

  static const char empty_json[] = "{}";

  if(!json || !json_len)
  {
    json = empty_json;
    json_len = sizeof(empty_json) - 1;
  }

  const size_t key_len = strlen(key);
  const size_t total_len = key_len + json_len + 5;

  AsyncWebSocketBuffer buffer(total_len);

  if(!buffer)
  {
    return false;
  }

  uint8_t* out = reinterpret_cast<uint8_t*>(buffer.data());
  size_t pos = 0;

  out[pos++] = '{';
  out[pos++] = '"';

  memcpy(out + pos, key, key_len);
  pos += key_len;

  out[pos++] = '"';
  out[pos++] = ':';

  memcpy(out + pos, json, json_len);
  pos += json_len;

  out[pos++] = '}';

  client->text(std::move(buffer));

  return true;
}


bool mWebServer::WebSocket_SendSensors(AsyncWebSocketClient* client)
{
  if(!client) return false;

  #ifdef USE_MODULE_SENSORS_INTERFACE

  if(!JBI->requestJSONBufferLock(GetModuleUniqueID()))
  {
    client->text(F("{\"error\":\"json buffer busy\"}"));
    return false;
  }

  bool data_to_send = tkr_iSensors->ConstructJSON_Sensor();

  bool result;

  if(data_to_send)
  {
    result = WebSocket_SendWrappedJSON(
      client,
      "sensors",
      JBI->GetBufferPtr(),
      JBI->GetLength()
    );
  }
  else
  {
    result = WebSocket_SendWrappedJSON(client, "sensors", "{}", 2);
  }

  JBI->releaseJSONBufferLock();

  return result;

  #else

  return WebSocket_SendWrappedJSON(client, "sensors", "{}", 2);

  #endif
}


bool mWebServer::WebSocket_SendEnergy(AsyncWebSocketClient* client)
{
  if(!client) return false;

  #ifdef USE_MODULE_ENERGY_INTERFACE

  if(!JBI->requestJSONBufferLock(GetModuleUniqueID()))
  {
    client->text(F("{\"error\":\"json buffer busy\"}"));
    return false;
  }

  bool data_to_send = tkr_iEnergy->ConstructJSON_Sensor();

  bool result;

  if(data_to_send)
  {
    result = WebSocket_SendWrappedJSON(
      client,
      "energy",
      JBI->GetBufferPtr(),
      JBI->GetLength()
    );
  }
  else
  {
    result = WebSocket_SendWrappedJSON(client, "energy", "{}", 2);
  }

  JBI->releaseJSONBufferLock();

  return result;

  #else

  return WebSocket_SendWrappedJSON(client, "energy", "{}", 2);

  #endif
}


void mWebServer::wsEventPages(
  AsyncWebSocket *server,
  AsyncWebSocketClient *client,
  AwsEventType type,
  void *arg,
  uint8_t *data,
  size_t len
)
{
  if(type == WS_EVT_CONNECT)
  {
    return;
  }

  if(type == WS_EVT_DISCONNECT)
  {
    return;
  }

  if(type == WS_EVT_ERROR)
  {
    return;
  }

  if(type == WS_EVT_PONG)
  {
    return;
  }

  if(type != WS_EVT_DATA)
  {
    return;
  }

  AwsFrameInfo* info = reinterpret_cast<AwsFrameInfo*>(arg);

  if(!info || !info->final || info->index != 0 || info->len != len || info->opcode != WS_TEXT)
  {
    client->text(F("{\"error\":\"multipart unsupported\"}"));
    return;
  }


  /********************************************************************
   * Fixed webpage data requests.
   *
   * These deliberately bypass JsonParser/TASK_JSON_COMMAND_ID.
   * The webpage controls the exact compact request strings.
   ********************************************************************/

  static const char request_controls[] = "{\"request\":\"controls\"}";
  static const char request_sensors[]  = "{\"request\":\"sensors\"}";
  static const char request_energy[]   = "{\"request\":\"energy\"}";

  #ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
  if(len == sizeof(request_controls) - 1 && !memcmp(data, request_controls, len))
  {
    WebSocket_SendControls(client);
    return;
  }
  #endif

  if(len == sizeof(request_sensors) - 1 && !memcmp(data, request_sensors, len))
  {
    WebSocket_SendSensors(client);
    return;
  }

  if(len == sizeof(request_energy) - 1 && !memcmp(data, request_energy, len))
  {
    WebSocket_SendEnergy(client);
    return;
  }


  /********************************************************************
   * Everything else on the webpage socket is a normal PulSar JSON
   * command and therefore uses the existing single JSON parse path.
   ********************************************************************/

  if(len < 2 || data[0] != '{' || data[len - 1] != '}')
  {
    client->text(F("{\"error\":\"invalid message\"}"));
    return;
  }

  if(len >= sizeof(data_buffer.payload.ctr))
  {
    client->text(F("{\"error\":\"command too large\"}"));
    return;
  }

  if(!data_buffer.requestLock(GetModuleUniqueID()))
  {
    client->text(F("{\"error\":\"command buffer busy\"}"));
    return;
  }

  data_buffer.ClearSoft();

  memcpy(data_buffer.payload.ctr, data, len);

  data_buffer.payload.ctr[len] = '\0';
  data_buffer.payload.length_used = len;

  const int8_t result = tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

  data_buffer.releaseLock();

  if(result != TASKER_RESULT__SUCCESS_ID)
  {
    client->text(F("{\"error\":\"json command failed\"}"));
    return;
  }


  /********************************************************************
   * A command was completed.
   * Immediately return authoritative post-command control state.
   ********************************************************************/

  #ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
  WebSocket_SendControls(client);
  #else
  client->text(F("{\"ok\":1}"));
  #endif
}

#endif // ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

#endif // ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS

#endif