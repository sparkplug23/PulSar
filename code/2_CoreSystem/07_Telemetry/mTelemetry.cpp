/**
 * @file    mTelemetry.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Generate JSON of coresystem values
 * @version 1.0
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "2_CoreSystem/07_Telemetry/mTelemetry.h"

/**
 * @brief 
 * 
 * v0.110.25.28         RAM 47876     Flash 531632 == for sidelight
 * 
 * 
 */

const char* mTelemetry::PM_MODULE_CORE_TELEMETRY_CTR = D_MODULE_CORE_TELEMETRY_CTR;
const char* mTelemetry::PM_MODULE_CORE_TELEMETRY_FRIENDLY_CTR = D_MODULE_CORE_TELEMETRY_FRIENDLY_CTR;

int8_t mTelemetry::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
    case FUNC_INIT:
      Init();
    break;
  }

  switch(function){
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
      break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEGPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER
  }

}


void mTelemetry::parse_JSONCommand(JsonParserObject obj){}

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mTelemetry::WebPage_Root_AddHandlers(){

  /**
   *Add telemetry calls-- retrieve tele by http request and return as text 
   **/ 
  /* 
  pCONT_web->pWebServer->on("/status/telemetry/health.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Health(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/settings.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    ConstructJSON_Settings(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/firmware.json", HTTP_GET, [this](AsyncWebServerRequest *request){    
    ConstructJSON_Firmware(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/log.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Log(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/memory.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Memory(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/network.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Network(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/mqtt.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_MQTT(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/time.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Time(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);
  });
  pCONT_web->pWebServer->on("/status/telemetry/devices.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Devices(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/reboot.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Reboot(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/minimal.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Minimal(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/pins.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Pins(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/template.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Template(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/moduleinterface.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_ModuleInterface(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });*/

}
#endif //  #ifdef USE_MODULE_NETWORK_WEBSERVER






void mTelemetry::Init(){
  
  #ifdef ENABLE_DEVFEATURE_HARDWARE_STATUS
  memset(&hardwarestatus,0,sizeof(hardwarestatus));
  hardwarestatus.len += 0;//sprintf(hardwarestatus.ctr,"Restarted");
  #endif// ENABLE_DEVFEATURE_HARDWARE_STATUS

}













