#include "_WebServer.h"


#ifdef USE_MODULE_NETWORK_WEBSERVER



/**************************************************************************************************
 * Generic Telemetry API
 **************************************************************************************************/



void mWebServer::HandleAPI_Telemetry(AsyncWebServerRequest* request)
{
  /************************************************************************************************
   * Catalogue
   ************************************************************************************************/

  if(!request->hasParam("topic"))
  {
    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->addHeader(F("Cache-Control"), F("no-store"));
    response->print(F("{\"topics\":["));

    telemetry_api_request.mode = TelemetryAPIRequestMode::Catalogue;
    telemetry_api_request.catalogue_response = response;
    telemetry_api_request.catalogue_first = true;

    tkr->Tasker_Interface(TASK_WEB_TELEMETRY);

    telemetry_api_request.mode = TelemetryAPIRequestMode::None;
    telemetry_api_request.catalogue_response = nullptr;

    response->print(F("]}"));
    request->send(response);
    return;
  }


  /************************************************************************************************
   * JSON buffer backoff
   ************************************************************************************************/

  const uint32_t now = millis();

  if(telemetry_api_json_last_used_ms && (now - telemetry_api_json_last_used_ms) < TELEMETRY_API_JSON_BACKOFF_MS)
  {
    const uint32_t retry_ms = TELEMETRY_API_JSON_BACKOFF_MS - (now - telemetry_api_json_last_used_ms);

    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->setCode(429);
    response->addHeader(F("Cache-Control"), F("no-store"));
    response->printf_P(PSTR("{\"error\":\"json buffer backoff\",\"retry_ms\":%lu}"), retry_ms);

    request->send(response);
    return;
  }


  /************************************************************************************************
   * Requested topic
   ************************************************************************************************/

  const String requested = request->getParam("topic")->value();

  if(!requested.length() || requested.length() >= sizeof(telemetry_api_request.requested_key))
  {
    request->send(400, FPSTR(CONTENT_TYPE_JSON), F("{\"error\":\"invalid topic\"}"));
    return;
  }

  strlcpy(telemetry_api_request.requested_key, requested.c_str(), sizeof(telemetry_api_request.requested_key));

  telemetry_api_request.mode = TelemetryAPIRequestMode::Topic;
  telemetry_api_request.found = false;
  telemetry_api_request.buffer_busy = false;
  telemetry_api_request.rate = 0;
  telemetry_api_request.packet = "";

  tkr->Tasker_Interface(TASK_WEB_TELEMETRY);

  telemetry_api_request.mode = TelemetryAPIRequestMode::None;


  /************************************************************************************************
   * Shared JSON buffer busy
   ************************************************************************************************/

  if(telemetry_api_request.buffer_busy)
  {
    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->setCode(503);
    response->addHeader(F("Cache-Control"), F("no-store"));
    response->print(F("{\"error\":\"json buffer busy\",\"retry_ms\":500}"));

    request->send(response);
    return;
  }


  /************************************************************************************************
   * Topic not found
   ************************************************************************************************/

  if(!telemetry_api_request.found)
  {
    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->setCode(404);
    response->addHeader(F("Cache-Control"), F("no-store"));
    response->print(F("{\"error\":\"topic not found\"}"));

    request->send(response);
    return;
  }


  /************************************************************************************************
   * Empty packet
   ************************************************************************************************/

  if(!telemetry_api_request.packet.length())
  {
    AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

    if(!response)
    {
      request->send(500);
      return;
    }

    response->setCode(500);
    response->addHeader(F("Cache-Control"), F("no-store"));
    response->print(F("{\"error\":\"empty telemetry packet\"}"));

    request->send(response);
    return;
  }


  /************************************************************************************************
   * Reply
   ************************************************************************************************/

  AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

  if(!response)
  {
    request->send(500);
    return;
  }

  response->addHeader(F("Cache-Control"), F("no-store"));

  response->print(F("{\"topic\":\""));
  PrintJSONString(*response, telemetry_api_request.requested_key);
  response->printf_P(PSTR("\",\"rate\":%u,\"packet\":"), telemetry_api_request.rate);
  response->print(telemetry_api_request.packet);
  response->print('}');

  request->send(response);
}


bool mWebServer::TelemetryAPI_Construct_Begin(const char* full_key, uint16_t rate)
{
  telemetry_api_request.found = true;
  telemetry_api_request.rate = rate;

  if(!JBI->RequestLock(GetModuleUniqueID()))
  {
    telemetry_api_request.buffer_busy = true;
    return false;
  }

  return true;
}


void mWebServer::TelemetryAPI_Construct_End()
{
  const char* buffer = JBI->GetBuffer();

  telemetry_api_request.packet = (buffer && buffer[0]) ? buffer : "";

  JBI->ReleaseLock();

  telemetry_api_json_last_used_ms = millis();
}


/**************************************************************************************************
 * Telemetry Debug Page
 **************************************************************************************************/

#ifdef ENABLE_DEBUGFEATURE_WEB__TELEMETRY

void mWebServer::HandlePage_DebugTelemetry(AsyncWebServerRequest* request)
{
  handleStaticContent(request, F("/adv/telemetry"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_debug_telemetry_web, PAGE_debug_telemetry_web_length, true);
}

#endif



#endif