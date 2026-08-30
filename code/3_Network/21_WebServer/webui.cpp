#include "_WebServer.h"


#ifdef USE_MODULE_NETWORK_WEBSERVER





#ifdef ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS


bool mWebServer::WebUI_Begin(Print* response)
{
  if(!response) return false;
  if(webui.response) return false;

  webui.response = response;
  webui.first_module = true;
  webui.first_control = true;
  webui.module_open = false;
  webui.button_row_open = false;
  webui.first_option = true;

  response->print(F("{\"controls\":{\"Modules\":["));

  return true;
}


void mWebServer::WebUI_End()
{
  if(!webui.response) return;

  if(webui.button_row_open)
  {
    WebUI_AddButtonRow_End();
  }

  if(webui.module_open)
  {
    WebUI_Module_End();
  }

  webui.response->print(F("]}}"));

  webui.response = nullptr;
  webui.first_module = true;
  webui.first_control = true;
  webui.module_open = false;
  webui.button_row_open = false;
  webui.first_option = true;
}

void mWebServer::WebUI_Module_Start(uint16_t module_id, const char* module_name)
{
  if(!webui.response) return;

  if(webui.module_open)
  {
    WebUI_Module_End();
  }

  if(!webui.first_module)
  {
    webui.response->print(',');
  }

  webui.first_module = false;
  webui.first_control = true;
  webui.module_open = true;

  char display_name_buffer[50];

  const char* display_name = tkr->GetModuleNameDisplayEachWord(
    module_id,
    display_name_buffer,
    sizeof(display_name_buffer)
  );

  if(!display_name)
  {
    display_name = module_name ? module_name : "";
  }

  webui.response->print(F("{\"ID\":"));
  webui.response->print(module_id);

  webui.response->print(F(",\"Name\":\""));
  PrintJSONString(*webui.response, display_name);
  webui.response->print(F("\",\"Controls\":["));
}


void mWebServer::WebUI_Module_End()
{
  if(!webui.response || !webui.module_open) return;

  webui.response->print(F("]}"));

  webui.module_open = false;
}


class WebUIStringPrint : public Print
{
  public:

  String value;

  size_t write(uint8_t c) override
  {
    value += (char)c;
    return 1;
  }

  size_t write(const uint8_t* buffer, size_t size) override
  {
    if(!buffer || !size) return 0;

    value.reserve(value.length() + size);

    for(size_t i = 0; i < size; i++)
    {
      value += (char)buffer[i];
    }

    return size;
  }
};


bool mWebServer::WebSocket_SendControls(AsyncWebSocketClient* client)
{
  if(!client) return false;

  WebUIStringPrint output;

  output.value.reserve(2048);

  if(!WebUI_Begin(&output))
  {
    client->text(F("{\"error\":\"webui busy\"}"));
    return false;
  }

  tkr->Tasker_Interface(TASK_WEBUI_APPEND);

  WebUI_End();

  return WebSocket_SendText(
    client,
    output.value.c_str(),
    output.value.length()
  );
}



void mWebServer::WebUI_AddToggle(const char* command, uint8_t device_id, const char* name, bool state)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"toggle\",\"C\":\""));
  PrintJSONString(*webui.response, command);

  webui.response->print(F("\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(state ? 1 : 0);

  webui.response->print('}');
}


void mWebServer::WebUI_AddMomentary(const char* command, uint8_t device_id, const char* name, bool state)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"momentary\",\"C\":\""));
  PrintJSONString(*webui.response, command);

  webui.response->print(F("\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(state ? 1 : 0);

  webui.response->print('}');
}
void mWebServer::WebUI_AddTestSwitch(const char* command, uint8_t device_id, const char* name, bool physical_state)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"testswitch\",\"C\":\""));
  PrintJSONString(*webui.response, command);

  webui.response->print(F("\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(physical_state ? 1 : 0);

  webui.response->print('}');
}


void mWebServer::WebUI_AddButtonRow_Start(const char* command, uint8_t device_id, const char* name, const char* description, int32_t selected_value)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control){
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"buttons\",\"C\":\""));
  PrintJSONString(*webui.response, command);

  webui.response->print(F("\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  if(description && description[0]){
    webui.response->print(F("\",\"S\":\""));
    PrintJSONString(*webui.response, description);
  }

  webui.response->print(F("\",\"V\":"));
  webui.response->print(selected_value);

  webui.response->print(F(",\"O\":["));

  webui.button_row_open = true;
  webui.first_option = true;
}


void mWebServer::WebUI_AddButtonRow_Option(const char* name, int32_t value)
{
  if(!webui.response || !webui.button_row_open) return;

  if(!webui.first_option)
  {
    webui.response->print(',');
  }

  webui.first_option = false;

  webui.response->print(F("[\""));
  PrintJSONString(*webui.response, name ? name : "");
  webui.response->print(F("\","));
  webui.response->print(value);
  webui.response->print(']');
}


void mWebServer::WebUI_AddButtonRow_End()
{
  if(!webui.response || !webui.button_row_open) return;

  webui.response->print(F("]}"));

  webui.button_row_open = false;
  webui.first_option = true;
}

void mWebServer::WebUI_AddIndicator(uint8_t device_id, const char* name, bool state)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"indicator\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(state ? 1 : 0);

  webui.response->print('}');
}

void mWebServer::WebUI_AddValue(uint8_t device_id, const char* name, const char* value, const char* units)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"value\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":\""));
  PrintJSONString(*webui.response, value ? value : "");
  webui.response->print('"');

  if(units && units[0])
  {
    webui.response->print(F(",\"U\":\""));
    PrintJSONString(*webui.response, units);
    webui.response->print('"');
  }

  webui.response->print('}');
}

void mWebServer::WebUI_AddValue(uint8_t device_id, const char* name, int32_t value, const char* units)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"value\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(value);

  if(units && units[0])
  {
    webui.response->print(F(",\"U\":\""));
    PrintJSONString(*webui.response, units);
    webui.response->print('"');
  }

  webui.response->print('}');
}

void mWebServer::WebUI_AddValue(uint8_t device_id, const char* name, float value, const char* units, uint8_t precision)
{
  if(!webui.response || !webui.module_open) return;

  if(!webui.first_control)
  {
    webui.response->print(',');
  }

  webui.first_control = false;

  webui.response->print(F("{\"T\":\"value\",\"D\":"));
  webui.response->print(device_id);

  webui.response->print(F(",\"N\":\""));
  PrintJSONString(*webui.response, name ? name : "");

  webui.response->print(F("\",\"V\":"));
  webui.response->print(value, precision);

  if(units && units[0])
  {
    webui.response->print(F(",\"U\":\""));
    PrintJSONString(*webui.response, units);
    webui.response->print('"');
  }

  webui.response->print('}');
}


#endif // ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS





#endif