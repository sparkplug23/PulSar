#include "mPinViewer.h"

#if defined(USE_MODULE_CORE_PINVIEWER) && defined(USE_MODULE_NETWORK_WEBSERVER)

/******************************************************************************************************************
 * Tasker
*******************************************************************************************************************/

int8_t mPinViewer::Tasker(uint8_t function, JsonParserObject obj)
{
  switch(function)
  {
    case TASK_PRE_INIT:
      Pre_Init();
    break;

    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running)
  {
    return TASKER_RESULT__MODULE_DISABLED_ID;
  }

  switch(function)
  {
    case TASK_LOOP:
      UpdateIfDue();
    break;

    case TASK_EVERY_100_MSECOND:
      if(settings.sampling_ms >= 100)
      {
        UpdateIfDue();
      }
    break;

    case TASK_EVERY_SECOND:
      PublishSystemTelemetry(false);
      PublishKeepAlive(false);
    break;

    case TASK_WEB_ADD_HANDLER:
      Web_AddHandlers();
    break;

    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;

    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;

    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;

    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
}


/******************************************************************************************************************
 * Init
*******************************************************************************************************************/

void mPinViewer::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 1;

  ResetRuntime();

  module_state.mode = ModuleStatus::Running;
}


void mPinViewer::Init(void)
{
  ResetRuntime();

  if(settings.sampling_ms < PINVIEWER_MIN_SAMPLING_MS)
  {
    settings.sampling_ms = PINVIEWER_MIN_SAMPLING_MS;
  }

  runtime.force_publish_all = true;
}


void mPinViewer::ResetRuntime(void)
{
  runtime.web_attached = false;
  runtime.sse_attached = false;
  runtime.force_publish_all = true;
  runtime.last_sample_ms = 0;
  runtime.last_heap_ms = 0;
  runtime.last_keepalive_ms = 0;
  runtime.publish_count = 0;
  runtime.connected_clients = 0;

  for(uint8_t pin = 0; pin < PINVIEWER_MAX_GPIO_PINS; pin++)
  {
    runtime.last_pin_state[pin] = PINVIEWER_STATE_UNKNOWN;
    runtime.last_pin_locked[pin] = false;
    runtime.last_pin_conflict[pin] = false;
  }
}


/******************************************************************************************************************
 * Runtime sampling / publishing
*******************************************************************************************************************/

void mPinViewer::UpdateIfDue(void)
{
  if(!settings.enabled) { return; }

  const uint32_t now = millis();

  if((now - runtime.last_sample_ms) < settings.sampling_ms) { return; }

  runtime.last_sample_ms = now;

  const bool force_all = runtime.force_publish_all;
  runtime.force_publish_all = false;

  SampleAndPublish(force_all);
}


void mPinViewer::SampleAndPublish(bool force_all)
{
  String json;

  const bool has_update = BuildGPIOStateJSON(json, force_all || settings.publish_unchanged);

  if(!has_update) { return; }

  runtime.publish_count++;

  if(events && settings.sse_enabled)
  {
    events->send(json.c_str(), "gpio-state", millis());
  }
}


void mPinViewer::PublishSystemTelemetry(bool force)
{
  if(!settings.enabled) { return; }
  if(!settings.publish_heap && !force) { return; }

  const uint32_t now = millis();

  if(!force && ((now - runtime.last_heap_ms) < PINVIEWER_HEAP_REPORT_MS)) { return; }

  runtime.last_heap_ms = now;

  if(events && settings.sse_enabled)
  {
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%u", ESP.getFreeHeap());
    events->send(buffer, "free_heap", millis());

    #ifdef ESP32
    snprintf(buffer, sizeof(buffer), "%u", ESP.getFreePsram());
    events->send(buffer, "free_psram", millis());
    #endif
  }
}


void mPinViewer::PublishKeepAlive(bool force)
{
  if(!settings.enabled) { return; }

  const uint32_t now = millis();

  if(!force && ((now - runtime.last_keepalive_ms) < PINVIEWER_KEEPALIVE_MS)) { return; }

  runtime.last_keepalive_ms = now;

  if(events && settings.sse_enabled)
  {
    events->send("1", "keepalive", millis());
  }
}


/******************************************************************************************************************
 * JSON builders
*******************************************************************************************************************/

bool mPinViewer::BuildGPIOStateJSON(String& json, bool force_all)
{
  json.reserve(4096);

  json = F("{\"ms\":");
  json += millis();
  json += F(",\"pins\":[");

  bool first = true;
  bool has_update = false;

  for(uint8_t real_pin = 0; real_pin < PINVIEWER_MAX_GPIO_PINS; real_pin++)
  {
    PinViewerPinMeta meta = GetPinMeta(real_pin);

    if(!meta.valid) { continue; }

    const bool changed =
      (meta.state != runtime.last_pin_state[real_pin]) ||
      (meta.locked != runtime.last_pin_locked[real_pin]) ||
      (meta.conflict != runtime.last_pin_conflict[real_pin]);

    if(force_all || changed)
    {
      AppendPinJSON(json, real_pin, true, first);

      runtime.last_pin_state[real_pin] = meta.state;
      runtime.last_pin_locked[real_pin] = meta.locked;
      runtime.last_pin_conflict[real_pin] = meta.conflict;

      has_update = true;
    }
  }

  json += F("]}");

  return has_update;
}


bool mPinViewer::BuildPinListJSON(String& json)
{
  json.reserve(8192);

  json = F("{\"pins\":[");

  bool first = true;

  for(uint8_t real_pin = 0; real_pin < PINVIEWER_MAX_GPIO_PINS; real_pin++)
  {
    PinViewerPinMeta meta = GetPinMeta(real_pin);

    if(!meta.valid) { continue; }

    AppendPinJSON(json, real_pin, true, first);
  }

  json += F("]}");

  return true;
}


bool mPinViewer::BuildGroupListJSON(String& json)
{
  json.reserve(2048);

  json = F("{\"groups\":[");

  bool first = true;

  const uint8_t group_count = Group_GetCount();

  for(uint8_t group_index = 0; group_index < group_count; group_index++)
  {
    const uint8_t group_id = Group_GetID_ByIndex(group_index);

    if(group_id == 0) { continue; }

    AppendGroupJSON(json, group_id, first);
  }

  json += F("]}");

  return true;
}


bool mPinViewer::BuildEspInfoJSON(String& json)
{
  json.reserve(512);

  json = F("{");
  json += F("\"module\":\"PinViewer\"");

  json += F(",\"chip\":\"");

  #ifdef ESP8266
    json += F("ESP8266");
  #elif defined(ESP32)
    json += F("ESP32");
  #else
    json += F("Unknown");
  #endif

  json += F("\"");

  json += F(",\"free_heap\":");
  json += ESP.getFreeHeap();

  #ifdef ESP32
  json += F(",\"free_psram\":");
  json += ESP.getFreePsram();
  #endif

  json += F(",\"sampling_ms\":");
  json += settings.sampling_ms;

  json += F(",\"publish_count\":");
  json += runtime.publish_count;

  json += F(",\"clients\":");
  json += runtime.connected_clients;

  json += F("}");

  return true;
}


bool mPinViewer::BuildSettingsJSON(String& json)
{
  json.reserve(512);

  json = F("{");

  json += F("\"enabled\":");
  json += settings.enabled ? F("true") : F("false");

  json += F(",\"web_enabled\":");
  json += settings.web_enabled ? F("true") : F("false");

  json += F(",\"sse_enabled\":");
  json += settings.sse_enabled ? F("true") : F("false");

  json += F(",\"publish_unchanged\":");
  json += settings.publish_unchanged ? F("true") : F("false");

  json += F(",\"publish_heap\":");
  json += settings.publish_heap ? F("true") : F("false");

  json += F(",\"publish_all_on_connect\":");
  json += settings.publish_all_on_connect ? F("true") : F("false");

  json += F(",\"sampling_ms\":");
  json += settings.sampling_ms;

  json += F("}");

  return true;
}


void mPinViewer::AppendPinJSON(String& json, uint8_t real_pin, bool include_state, bool& first)
{
  if(!first) { json += ','; }
  first = false;

  PinViewerPinMeta meta = GetPinMeta(real_pin);

  char function_name[32];
  char owner_name[32];
  char group_name[32];
  char group_colour[12];

  tkr_pins->mPins::PinTable_GetFunctionName(real_pin, function_name, sizeof(function_name));
  tkr_pins->mPins::PinTable_GetOwnerName(real_pin, owner_name, sizeof(owner_name));
  tkr_pins->mPins::PinTable_GetGroupName(real_pin, group_name, sizeof(group_name));
  Group_GetColour_ByID(meta.group_id, group_colour, sizeof(group_colour));

  json += F("{\"pin\":");
  json += meta.pin;

  json += F(",\"valid\":");
  json += meta.valid ? F("true") : F("false");

  json += F(",\"usable\":");
  json += meta.usable ? F("true") : F("false");

  json += F(",\"used\":");
  json += meta.used ? F("true") : F("false");

  if(include_state)
  {
    json += F(",\"state\":");
    json += meta.state;
  }

  json += F(",\"function_id\":");
  json += meta.function_id;

  json += F(",\"function\":\"");
  AppendJSONString(json, function_name);
  json += F("\"");

  json += F(",\"owner_id\":");
  json += meta.owner_id;

  json += F(",\"owner\":\"");
  AppendJSONString(json, owner_name);
  json += F("\"");

  json += F(",\"direction_id\":");
  json += meta.direction_id;

  json += F(",\"direction\":\"");
  json += GetDirectionName(meta.direction_id);
  json += F("\"");

  json += F(",\"share_mode_id\":");
  json += meta.share_mode_id;

  json += F(",\"share\":\"");
  json += GetShareModeName(meta.share_mode_id);
  json += F("\"");

  json += F(",\"user_count\":");
  json += meta.user_count;

  json += F(",\"group_id\":");
  json += meta.group_id;

  json += F(",\"group\":\"");
  AppendJSONString(json, group_name);
  json += F("\"");

  json += F(",\"colour\":\"");
  AppendJSONString(json, group_colour);
  json += F("\"");

  json += F(",\"locked\":");
  json += meta.locked ? F("true") : F("false");

  json += F(",\"reserved\":");
  json += meta.reserved ? F("true") : F("false");

  json += F(",\"conflict\":");
  json += meta.conflict ? F("true") : F("false");

  json += F(",\"allocated\":");
  json += meta.used ? F("true") : F("false");

  json += F("}");
}


void mPinViewer::AppendGroupJSON(String& json, uint8_t group_id, bool& first)
{
  if(!first) { json += ','; }
  first = false;

  char group_name[32];
  char group_colour[12];

  Group_GetName_ByID(group_id, group_name, sizeof(group_name));
  Group_GetColour_ByID(group_id, group_colour, sizeof(group_colour));

  json += F("{\"group_id\":");
  json += group_id;

  json += F(",\"name\":\"");
  AppendJSONString(json, group_name);
  json += F("\"");

  json += F(",\"colour\":\"");
  AppendJSONString(json, group_colour);
  json += F("\"");

  json += F(",\"addresses\":[");

  bool first_address = true;

  const uint8_t address_count = Group_GetAddressCount(group_id);

  for(uint8_t address_index = 0; address_index < address_count; address_index++)
  {
    uint8_t address = 0;
    char module_name[32];

    if(!Group_GetAddress_ByIndex(group_id, address_index, &address, module_name, sizeof(module_name)))
    {
      continue;
    }

    if(!first_address) { json += ','; }
    first_address = false;

    json += F("{\"address\":\"0x");

    if(address < 16) { json += '0'; }
    json += String(address, HEX);

    json += F("\",\"module\":\"");
    AppendJSONString(json, module_name);
    json += F("\"}");
  }

  json += F("]}");
}


void mPinViewer::AppendJSONString(String& json, const char* value)
{
  if(!value) { return; }

  while(*value)
  {
    switch(*value)
    {
      case '"':  json += F("\\\""); break;
      case '\\': json += F("\\\\"); break;
      case '\n': json += F("\\n");  break;
      case '\r': json += F("\\r");  break;
      case '\t': json += F("\\t");  break;
      default:   json += *value;    break;
    }

    value++;
  }
}


/******************************************************************************************************************
 * Pin metadata
*******************************************************************************************************************/

mPinViewer::PinViewerPinMeta mPinViewer::GetPinMeta(uint8_t real_pin)
{
  PinViewerPinMeta meta;

  meta.pin = real_pin;
  meta.valid = IsValidPin(real_pin);

  if(!meta.valid)
  {
    meta.state = PINVIEWER_STATE_INVALID;
    return meta;
  }

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  meta.usable = IsUsablePin(real_pin);
  meta.used = p.IsAllocated();
  meta.locked = p.IsLocked();
  meta.reserved = IsReservedPin(real_pin);
  meta.conflict = p.HasConflict();

  meta.function_id = p.FunctionID();
  meta.owner_id = p.OwnerID();
  meta.direction_id = GetDirectionID(real_pin);
  meta.share_mode_id = GetShareModeID(real_pin);
  meta.user_count = GetUserCount(real_pin);
  meta.group_id = p.GroupID();

  meta.state = ReadPinState(real_pin);

  return meta;
}


int8_t mPinViewer::ReadPinState(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) { return PINVIEWER_STATE_INVALID; }

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  if(p.allocation.sensitive_to_probe) { return PINVIEWER_STATE_UNKNOWN; }
  if(p.physical.flash_reserved) { return PINVIEWER_STATE_UNKNOWN; }
  if(p.physical.usb_reserved) { return PINVIEWER_STATE_UNKNOWN; }
  if(p.allocation.unavailable) { return PINVIEWER_STATE_UNKNOWN; }

  return digitalRead(real_pin) ? PINVIEWER_STATE_HIGH : PINVIEWER_STATE_LOW;
}


bool mPinViewer::IsValidPin(uint8_t real_pin)
{
  return real_pin < MAX_GPIO_PIN;
}


bool mPinViewer::IsUsablePin(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) return false;

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  return p.IsUsable();
}


bool mPinViewer::IsReservedPin(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) return false;

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  return p.IsReserved();
}


uint8_t mPinViewer::GetDirectionID(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) return PINVIEWER_DIRECTION_UNKNOWN;

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  if(p.physical.input_only)  return PINVIEWER_DIRECTION_INPUT;
  if(p.physical.output_only) return PINVIEWER_DIRECTION_OUTPUT;

  return PINVIEWER_DIRECTION_UNKNOWN;
}


uint8_t mPinViewer::GetShareModeID(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) return PINVIEWER_SHARE_UNKNOWN;

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  if(p.allocation.shared) return PINVIEWER_SHARE_SHARED;
  if(p.allocation.allocated) return PINVIEWER_SHARE_EXCLUSIVE;
  if(p.allocation.unavailable) return PINVIEWER_SHARE_RESERVED;

  return PINVIEWER_SHARE_UNKNOWN;
}


uint8_t mPinViewer::GetUserCount(uint8_t real_pin)
{
  if(!IsValidPin(real_pin)) return 0;

  const mPins::PinTable& p = tkr_pins->pin[real_pin];

  return p.allocation.allocated ? 1 : 0;
}


uint8_t mPinViewer::Group_GetCount(void)
{
  uint8_t count = 0;
  uint8_t groups[MAX_GPIO_PIN];

  for(uint8_t i = 0; i < ARRAY_SIZE(groups); i++)
  {
    groups[i] = 0;
  }

  for(uint8_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
  {
    const uint8_t group_id = tkr_pins->pin[real_pin].GroupID();

    if(group_id == 0)
    {
      continue;
    }

    bool found = false;

    for(uint8_t i = 0; i < count; i++)
    {
      if(groups[i] == group_id)
      {
        found = true;
        break;
      }
    }

    if(!found)
    {
      groups[count++] = group_id;
    }
  }

  return count;
}


uint8_t mPinViewer::Group_GetID_ByIndex(uint8_t group_index)
{
  uint8_t count = 0;
  uint8_t groups[MAX_GPIO_PIN];

  for(uint8_t i = 0; i < ARRAY_SIZE(groups); i++)
  {
    groups[i] = 0;
  }

  for(uint8_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
  {
    const uint8_t group_id = tkr_pins->pin[real_pin].GroupID();

    if(group_id == 0)
    {
      continue;
    }

    bool found = false;

    for(uint8_t i = 0; i < count; i++)
    {
      if(groups[i] == group_id)
      {
        found = true;
        break;
      }
    }

    if(!found)
    {
      if(count == group_index)
      {
        return group_id;
      }

      groups[count++] = group_id;
    }
  }

  return 0;
}


const char* mPinViewer::Group_GetName_ByID(uint8_t group_id, char* buffer, uint8_t buflen)
{
  if((!buffer) || (buflen == 0)) return "";

  buffer[0] = '\0';

  if(group_id == 0)
  {
    snprintf(buffer, buflen, "None");
    return buffer;
  }

  return tkr_pins->GetGPIOFunctionNamebyID(PGPIO(group_id), buffer, buflen);
}


const char* mPinViewer::Group_GetColour_ByID(uint8_t group_id, char* buffer, uint8_t buflen)
{
  if((!buffer) || (buflen == 0)) return "";

  buffer[0] = '\0';

  if(group_id == 0)
  {
    snprintf(buffer, buflen, "#808080");
    return buffer;
  }

  switch(group_id % 8)
  {
    case 0:  snprintf(buffer, buflen, "#4E79A7"); break;
    case 1:  snprintf(buffer, buflen, "#F28E2B"); break;
    case 2:  snprintf(buffer, buflen, "#E15759"); break;
    case 3:  snprintf(buffer, buflen, "#76B7B2"); break;
    case 4:  snprintf(buffer, buflen, "#59A14F"); break;
    case 5:  snprintf(buffer, buflen, "#EDC948"); break;
    case 6:  snprintf(buffer, buflen, "#B07AA1"); break;
    default: snprintf(buffer, buflen, "#FF9DA7"); break;
  }

  return buffer;
}


uint8_t mPinViewer::Group_GetAddressCount(uint8_t group_id)
{
  (void)group_id;
  return 0;
}


bool mPinViewer::Group_GetAddress_ByIndex(uint8_t group_id, uint8_t address_index, uint8_t* address, char* module_name, uint8_t module_name_len)
{
  (void)group_id;
  (void)address_index;

  if(address)
  {
    *address = 0;
  }

  if((module_name) && (module_name_len > 0))
  {
    module_name[0] = '\0';
  }

  return false;
}


const char* mPinViewer::GetDirectionName(uint8_t direction_id)
{
  switch(direction_id)
  {
    case PINVIEWER_DIRECTION_INPUT:         return "input";
    case PINVIEWER_DIRECTION_OUTPUT:        return "output";
    case PINVIEWER_DIRECTION_BIDIRECTIONAL: return "bidirectional";
    case PINVIEWER_DIRECTION_ALTERNATE:     return "alternate";
    default:                                return "unknown";
  }
}


const char* mPinViewer::GetShareModeName(uint8_t share_mode_id)
{
  switch(share_mode_id)
  {
    case PINVIEWER_SHARE_EXCLUSIVE: return "exclusive";
    case PINVIEWER_SHARE_SHARED:    return "shared";
    case PINVIEWER_SHARE_RESERVED:  return "reserved";
    case PINVIEWER_SHARE_INTERNAL:  return "internal";
    default:                        return "unknown";
  }
}


/******************************************************************************************************************
 * Web / SSE
*******************************************************************************************************************/

void mPinViewer::Web_AddHandlers(void)
{
  if(runtime.web_attached) { return; }

  ALOG_INF(PSTR("PinViewer Web_AddHandlers"));


  SPGM_CTR(PM_URL_PINVIEWER_RELEASE) "/pinv/release";
  tkr_web->server->on(PM_URL_PINVIEWER_RELEASE, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/release"));
    Web_SendRelease(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_RELEASE, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_SAMPLING) "/pinv/smp";
  tkr_web->server->on(PM_URL_PINVIEWER_SAMPLING, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/smp"));
    Web_SendSampling(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_SAMPLING, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_SYSTEM) "/pinv/sys";
  tkr_web->server->on(PM_URL_PINVIEWER_SYSTEM, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/sys"));
    Web_SendEspInfo(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_SYSTEM, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_MODES) "/pinv/mode";
  tkr_web->server->on(PM_URL_PINVIEWER_MODES, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/mode"));
    Web_SendPinModes(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_MODES, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_FUNCTIONS) "/pinv/func";
  tkr_web->server->on(PM_URL_PINVIEWER_FUNCTIONS, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/func"));
    Web_SendPinFunctions(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_FUNCTIONS, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_ALLOCATIONS) "/pinv/alloc";
  tkr_web->server->on(PM_URL_PINVIEWER_ALLOCATIONS, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/alloc"));
    Web_SendPinAllocations(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_ALLOCATIONS, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER_GROUPS) "/pinv/groups";
  tkr_web->server->on(PM_URL_PINVIEWER_GROUPS, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv/groups"));
    Web_SendGroups(request);
  });
  AddURLtoList(PM_URL_PINVIEWER_GROUPS, HTTP_GET);


  SPGM_CTR(PM_URL_PINVIEWER) "/pinv";
  tkr_web->server->on(PM_URL_PINVIEWER, HTTP_GET, [this](AsyncWebServerRequest* request){
    ALOG_INF(PSTR("PinViewer route: /pinv"));
    Web_SendRoot(request);
  });
  AddURLtoList(PM_URL_PINVIEWER, HTTP_GET);


  SSE_Init();

  runtime.web_attached = true;
}


void mPinViewer::SSE_Init(void)
{
  if(runtime.sse_attached) { return; }

  SPGM_CTR(PM_URL_PINVIEWER_EVENTS) "/pinv/events";

  ALOG_INF(PSTR("PinViewer SSE_Init route: /pinv/events"));

  events = new AsyncEventSource(PM_URL_PINVIEWER_EVENTS);

  events->onConnect([this](AsyncEventSourceClient* client){
    SSE_OnConnect(client);
  });

  tkr_web->server->addHandler(events);
  AddURLtoList(PM_URL_PINVIEWER_EVENTS, HTTP_GET);

  runtime.sse_attached = true;
}


void mPinViewer::Web_SendRoot(AsyncWebServerRequest* request)
{
  AsyncWebServerResponse* response =
    request->beginResponse_P(
      200,
      FPSTR(CONTENT_TYPE_HTML),
      WEB_2_CoreSystem_35_PinViewer_pinviewer_main_htm,
      WEB_2_CoreSystem_35_PinViewer_pinviewer_main_htm_length
    );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}


void mPinViewer::Web_SendRelease(AsyncWebServerRequest* request)
{
  request->send(200, "application/json", "{\"name\":\"PulSar PinViewer\",\"version\":\"0.4-mPins::PinTable\"}");
}


void mPinViewer::Web_SendSampling(AsyncWebServerRequest* request)
{
  if(request->hasParam("ms"))
  {
    uint16_t value = request->getParam("ms")->value().toInt();

    if(value < PINVIEWER_MIN_SAMPLING_MS)
    {
      value = PINVIEWER_MIN_SAMPLING_MS;
    }

    settings.sampling_ms = value;
    runtime.force_publish_all = true;
  }

  String json;
  json.reserve(64);
  json = F("{\"sampling_ms\":");
  json += settings.sampling_ms;
  json += F("}");

  request->send(200, "application/json", json);
}


void mPinViewer::Web_SendEspInfo(AsyncWebServerRequest* request)
{
  String json;
  BuildEspInfoJSON(json);
  request->send(200, "application/json", json);
}


void mPinViewer::Web_SendPinModes(AsyncWebServerRequest* request)
{
  String json;
  BuildPinListJSON(json);
  request->send(200, "application/json", json);
}


void mPinViewer::Web_SendPinFunctions(AsyncWebServerRequest* request)
{
  String json;
  BuildPinListJSON(json);
  request->send(200, "application/json", json);
}


void mPinViewer::Web_SendPinAllocations(AsyncWebServerRequest* request)
{
  String json;
  BuildPinListJSON(json);
  request->send(200, "application/json", json);
}


void mPinViewer::Web_SendGroups(AsyncWebServerRequest* request)
{
  String json;
  BuildGroupListJSON(json);
  request->send(200, "application/json", json);
}


void mPinViewer::SSE_OnConnect(AsyncEventSourceClient* client)
{
  runtime.connected_clients++;

  if(!client) { return; }

  client->send("connected", "status", millis());

  if(settings.publish_all_on_connect)
  {
    String json;
    BuildGPIOStateJSON(json, true);
    client->send(json.c_str(), "gpio-state", millis());
  }

  PublishSystemTelemetry(true);
  PublishKeepAlive(true);
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mPinViewer::parse_JSONCommand(JsonParserObject obj)
{
  if(!obj) { return; }

  JsonParserToken jtok = 0;

  jtok = obj["PinViewerEnabled"];
  if(jtok)
  {
    settings.enabled = jtok.getBool();
  }

  jtok = obj["PinViewerSamplingMS"];
  if(jtok)
  {
    uint16_t value = jtok.getInt();

    if(value < PINVIEWER_MIN_SAMPLING_MS)
    {
      value = PINVIEWER_MIN_SAMPLING_MS;
    }

    settings.sampling_ms = value;
  }

  jtok = obj["PinViewerPublishUnchanged"];
  if(jtok)
  {
    settings.publish_unchanged = jtok.getBool();
  }

  jtok = obj["PinViewerPublishHeap"];
  if(jtok)
  {
    settings.publish_heap = jtok.getBool();
  }

  jtok = obj["PinViewerForcePublish"];
  if(jtok)
  {
    if(jtok.getBool())
    {
      runtime.force_publish_all = true;
    }
  }
}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mPinViewer::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  (void)json_level;
  (void)json_appending;

  JBI->Start();

  JBI->Add("Enabled", settings.enabled);
  JBI->Add("WebEnabled", settings.web_enabled);
  JBI->Add("SSEEnabled", settings.sse_enabled);
  JBI->Add("SamplingMS", settings.sampling_ms);
  JBI->Add("PublishUnchanged", settings.publish_unchanged);
  JBI->Add("PublishHeap", settings.publish_heap);
  JBI->Add("PublishAllOnConnect", settings.publish_all_on_connect);

  return JBI->End();
}


uint8_t mPinViewer::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  (void)json_level;
  (void)json_appending;

  uint16_t valid_count = 0;
  uint16_t used_count = 0;
  uint16_t conflict_count = 0;
  uint16_t locked_count = 0;

  for(uint8_t real_pin = 0; real_pin < PINVIEWER_MAX_GPIO_PINS; real_pin++)
  {
    if(!IsValidPin(real_pin)) { continue; }

    valid_count++;

    const mPins::PinTable& p = tkr_pins->pin[real_pin];

    if(p.IsAllocated()) { used_count++; }
    if(p.HasConflict()) { conflict_count++; }
    if(p.IsLocked())    { locked_count++; }
  }

  JBI->Start();

  JBI->Add("ValidPins", valid_count);
  JBI->Add("UsedPins", used_count);
  JBI->Add("Conflicts", conflict_count);
  JBI->Add("Locked", locked_count);
  JBI->Add("PublishCount", runtime.publish_count);
  JBI->Add("Clients", runtime.connected_clients);

  return JBI->End();
}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mPinViewer::MQTTHandler_Init()
{
  struct handler<mPinViewer>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mPinViewer::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPinViewer::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPinViewer::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_CORE_PINVIEWER