
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER


#include "mWebServer.h"


const char* mWebServer::PM_MODULE_NETWORK_WEBSERVER_CTR = D_MODULE_NETWORK_WEBSERVER_CTR;
const char* mWebServer::PM_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR = D_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR;

bool doReboot = false;


//realtime modes
#define REALTIME_MODE_INACTIVE    0
#define REALTIME_MODE_GENERIC     1
#define REALTIME_MODE_UDP         2
#define REALTIME_MODE_HYPERION    3
#define REALTIME_MODE_E131        4
#define REALTIME_MODE_ADALIGHT    5
#define REALTIME_MODE_ARTNET      6
#define REALTIME_MODE_TPM2NET     7
#define REALTIME_MODE_DDP         8

# define WLED_GLOBAL
# define _INIT(x) = x

byte realtimeMode = REALTIME_MODE_INACTIVE;
// User Interface CONFIG
#ifndef SERVERNAME
WLED_GLOBAL char serverDescription[33] _INIT("WLED");  // Name of module - use default
#else
WLED_GLOBAL char serverDescription[33] _INIT(SERVERNAME);  // use predefined name
#endif
WLED_GLOBAL bool syncToggleReceive     _INIT(false);   // UIs which only have a single button for sync should toggle send+receive if this is true, only send otherwise
WLED_GLOBAL bool simplifiedUI          _INIT(false);   // enable simplified UI
WLED_GLOBAL byte cacheInvalidate       _INIT(0);       // used to invalidate browser cache when switching from regular to simplified UI



// uint8_t webserver_state = HTTP_OFF;

// void mWebServer::init(void){

  
//   #ifdef DEBUG_WEBSERVER_MEMORY
//   freemem_usage_json_shared.name_ptr = freemem_usage_name_json_shared;
//   #endif



// }

// // Helper function to avoid code duplication (saves 4k Flash)
// void mWebServer::WebGetArg(AsyncWebServerRequest *request, const char* arg, char* out, size_t max)
// {
  
//   // #ifndef DISABLE_SERIAL_LOGGING
//   // P_PHASE_OUT();
//   // #endif

//   String s = request->arg(arg);
//   strlcpy(out, s.c_str(), max);

// }

// bool mWebServer::WifiIsInManagerMode(){
//   return (HTTP_MANAGER == webserver_state || HTTP_MANAGER_RESET_ONLY == webserver_state);
// }

// // void mWebServer::ShowWebSource(int source)
// // {
// //   // if ((source > 0) && (source < SRC_MAX)) {
// //   //   char stemp1[20];
// //   //   AddLog(LOG_LEVEL_DEBUG, PSTR("SRC: %s from %s"), 
// //   //   pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), source, kCommandSource), request->client().remoteIP().toString().c_str());
// //   // }
// // }

// // void mWebServer::ExecuteWebCommand(char* svalue, int source)
// // {
// //   ShowWebSource(source);
// //   //AddLog(LOG_LEVEL_DEBUG, PSTR("CODE NOT DONE %s"),"ExecuteCommand(svalue, SRC_IGNORE);");
// //   ExecuteCommand(svalue, SRC_IGNORE);
// // }


// void mWebServer::handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len,
//     size_t index, size_t total) {
//   // ESP_LOGD(TAG, "handleBody [len=%u, index=%u, total=%u]", len, index, total);

//   Serial.println("mWebServer::handleBody");

//   // If whole data is not available, do not botter making sense of it.
//   if (!data || len != total) {
//     return;
//   }

//   // `POST /sys/master`
//   if (request->method() == HTTP_POST && request->url() == "/setmaster") {
//     // if (this->set_master_(data, len)) {
//     //   request->send(200, APPLICATION_JSON, bool_response(true).c_str());
//     // } else  {
//       request->send(500);
//     // }
//     return;
//   }
// }


// void mWebServer::StartWebserver(int type, IPAddress ipweb)
// {
//   // Ensure valid refresh values
//   // if(!pCONT_set->Settings.web_refresh){ pCONT_set->Settings.web_refresh = HTTP_REFRESH_TIME; }

//   ALOG_INF(PSTR(D_LOG_DEBUG "StartWebserver"));
  
//   if(!webserver_state){
//     if(!pWebServer){    

//       #ifdef ESP8266
//         pWebServer = new AsyncWebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
//       #else
//         pWebServer = new AsyncWebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
//       #endif
            
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*"); // ALLOW CROSS SITE JAVASCRIPT ACCESS (CORS) BY DEFAULT
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST");
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "authorization");    

//       // Must add handlers BEFORE begin
//       pCONT->Tasker_Interface(FUNC_WEB_ADD_HANDLER);
//     }
//     reset_web_log_flag = false;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "StartWebserver starting..."));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     pWebServer->begin();
//   }
//   if(webserver_state != type){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER " %s%s " D_JSON_IPADDRESS " %s"), pCONT_set->my_hostname, 1 ? ".local" : "", ipweb.toString().c_str());
    
//     #endif// ENABLE_LOG_LEVEL_INFO
//     // pCONT_set->rules_flag.http_init = 1;
//   }
//   if(type){ webserver_state = type; }
// }


// void mWebServer::WebAppend_Root_Draw_Table_dList(uint8_t row_count, char const* value_handle, const char* dList_titles){
//   char listheading[50];

//   //row_count=2;

//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed(listheading, sizeof(listheading), ii, dList_titles));
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }


// void mWebServer::WebAppend_Root_Draw_Table_dList_P(uint8_t row_count, char const* value_handle, const char* dList_titles){
//   char listheading[50];

//   //row_count=2;

//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, dList_titles));
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }

// void mWebServer::WebAppend_Root_Draw_Table_Repeat_Row_Name_Numbers(uint8_t row_count, char const* value_handle, const char* dList_titles){
  
//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s %d</td>"), dList_titles);
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }

// //pass row title using array/arrlen, include prefix format
// //leave contents blank

// // void mWebServer::WebAppend_Draw_Table_FP(uint8_t row_count, char const* value_handle, const char* dList_titles){
// //   char listheading[50];
// //   BufferWriterI->Append_P(PSTR("{t}"));
// //   for(int ii=0;ii<row_count;ii++){
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
// //       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, dList_titles));
// //       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
// //   }    
// //   BufferWriterI->Append_P(PSTR("{t2}"));
// // }



// void mWebServer::WebAppend_Draw_Table_FP(const char* table_class_col2, const char* formatP_row1, ...)
// {
//   char* buff = BufferWriterI->GetBufferPtr();
//   uint16_t* len = BufferWriterI->GetLengthPtr();
//   uint16_t size = BufferWriterI->GetBufferSize();

//   BufferWriterI->Append_P(PSTR("{t}"));
//   va_list arg;
//   va_start(arg, formatP_row1);
//   //Write cell 1
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   BufferWriterI->Append_P(PSTR("<td>"));
//     *len += vsnprintf_P(&buff[*len], size, formatP_row1, arg);
//   BufferWriterI->Append_P(PSTR("</td>"));
//   //Write cell 2
//   BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),table_class_col2);
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   va_end(arg);
//   BufferWriterI->Append_P(PSTR("{t2}"));

// }




// void mWebServer::WebPage_Root_AddHandlers(){

//   /**
//    * Shared resources
//    * */
//   pCONT_web->pWebServer->on("/base_page_fill.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Base_Page_Draw(request);
//   });   
//   pWebServer->on(WEB_HANLDE_JSON_WEB_TOP_BAR, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     WebSend_JSON_WebServer_TopBar(request);    
//   });  
//   pWebServer->on(WEB_HANLDE_JSON_WEB_STATUS_POPOUT_DATA, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     WebSend_JSON_WebServer_StatusPopoutData(request);    
//   });
//   pCONT_web->pWebServer->on(WEB_HANDLE_JSON_ROOT_STATUS_ANY, HTTP_GET, [this](AsyncWebServerRequest *request){
//     WebSend_JSON_RootStatus_Table(request);
//   });

//   pCONT_web->pWebServer->on("/console_test.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Console_JSON_Data(request);
//   });  
      
//   pCONT_web->pWebServer->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     if(!index)
//       Serial.printf("BodyStart: %u\n", total);
//     Serial.printf("%s", (const char*)data);
//     if(index + len == total)
//       Serial.printf("BodyEnd: %u\n", total);
//   });

  
//   pCONT_web->pWebServer->on("/json_command.json", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     Serial.println("onRequestBody " "/json_command.json" );
//     if ((request->url() == "/json_command.json") && (request->method() == HTTP_POST))
//     {

//       //copy into buffer
//       D_DATA_BUFFER_CLEAR();
//       memcpy(data_buffer.payload.ctr,data,sizeof(char)*total);
//       data_buffer.payload.len = strlen(data_buffer.payload.ctr);
//       data_buffer.flags.source_id = DATA_BUFFER_FLAG_SOURCE_WEBUI;

//       pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);     
        
//       request->send(200, CONTENT_TYPE_APPLICATION_JSON_ID, "{\"status\":\"success\"}");

//     }
//   });

//   /**
//    * Root Page
//    * */
//   // pWebServer->redirect("/", "/main/page");
//   // pCONT_web->pWebServer->rewrite("/", WEB_HANDLE_ROOT);
//   pCONT_web->pWebServer->on(WEB_HANDLE_ROOT "/", [this](AsyncWebServerRequest *request){
//     HandlePage_Root(request);
//   });
//   pCONT_web->pWebServer->on(WEB_HANDLE_ROOT "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Root_Draw(request);
//   });
//   pCONT_web->pWebServer->on(WEB_HANDLE_ROOT "/module_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Root_Draw_Modules(request);
//   });
//   pCONT_web->pWebServer->on(WEB_HANDLE_ROOT "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_Root_UpdateURLs(request);
//   }); 

//   /**
//    * Console Page
//    * */
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_CONSOLE_PAGE, [this](AsyncWebServerRequest *request){
//     HandlePage_Console(request);
//   });
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_CONSOLE "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Console_Draw(request);
//   });
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_CONSOLE "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     // Web_Root_UpdateURLs(request);
//   }); 
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_CONSOLE "/update_data.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     // Web_Root_Draw_Modules(request); //get console log
//   });

//   /**
//    * Information Page
//    * */
//   // pWebServer->on(PM_WEBURL_PAGE_INFO, [this](AsyncWebServerRequest *request){
//   //   HandleInformation(request); 
//   // });
//   // pWebServer->on(PM_WEBURL_PAGE_INFO_LOAD_URLS, HTTP_GET, [this](AsyncWebServerRequest *request){  
//   //   Web_All_LoadTime_Minimum_URLs(request);
//   // });  
//   // pWebServer->on(PM_WEBURL_PAGE_INFO_DRAW_TABLE, [this](AsyncWebServerRequest *request){
//   //   HandleInformation_TableDraw(request); 
//   // });
//   // pWebServer->on(PM_WEBURL_PAGE_INFO_LOAD_SCRIPT, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   //Web_Info_LoadScript(request);
//   //   HandleNotFound(request);
//   // });

//   /**
//    * Upgrades
//    * */  
//   // pWebServer->on("/up", [this](AsyncWebServerRequest *request){HandleUpgradeFirmware(request); });
//   // pWebServer->on("/u1", [this](AsyncWebServerRequest *request){HandleUpgradeFirmwareStart(request); });  // OTA
//   // // pWebServer->on("/u2", HTTP_POST,
//   // //     [this](AsyncWebServerRequest *request){HandleUploadDone(request); },
//   // //     [this](AsyncWebServerRequest *request){HandleUploadLoop(request); });
//   // pWebServer->on("/u2", HTTP_OPTIONS, [this](AsyncWebServerRequest *request){HandlePreflightRequest(request); });

//   /**
//    * System Settings
//    * */  
//   // pWebServer->on("/ss", [this](AsyncWebServerRequest *request){
//   //   HandleSystemSettings(request); 
//   // });


//   /**
//    * Resources Settings
//    * */ 
//   // pWebServer->on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
//   //   // HandleFavicon(request);
//   // });
          
// //       pWebServer->on("/cm", [this](AsyncWebServerRequest *request){HandleHttpCommand(request); });
  
// // #ifndef FIRMWARE_MINIMAL
// //       pWebServer->on("/cn", [this](AsyncWebServerRequest *request){HandleConfiguration(request); });
// //       pWebServer->on("/md", [this](AsyncWebServerRequest *request){HandleModuleConfiguration(request); });
// //       pWebServer->on("/wi", [this](AsyncWebServerRequest *request){HandleWifiConfiguration(request); });
// //       pWebServer->on("/lg", [this](AsyncWebServerRequest *request){HandleLoggingConfiguration(request); });
// //       pWebServer->on("/tp", [this](AsyncWebServerRequest *request){HandleTemplateConfiguration(request); });
// //       pWebServer->on("/co", [this](AsyncWebServerRequest *request){HandleOtherConfiguration(request); });
// //       pWebServer->on("/dl", [this](AsyncWebServerRequest *request){HandleBackupConfiguration(request); });
// //       pWebServer->on("/rs", [this](AsyncWebServerRequest *request){HandleRestoreConfiguration(request); });
//       // pWebServer->on("/rt", [this](AsyncWebServerRequest *request){HandleResetConfiguration(request); });
//       pWebServer->on("/reset", [this](AsyncWebServerRequest *request){HandleReset(request); });
// //#endif
//   pWebServer->onNotFound([this](AsyncWebServerRequest *request){HandleNotFound(request); });
  
// }


// bool mWebServer::CheckWebSendFreeMemoryTooLow(uint16_t memory_needed){
//   if(!memory_needed){
//     memory_needed = 3000;
//   }
//   if(ESP.getFreeHeap()>memory_needed){
//     return true;
//   }
//   return false;
// }

// bool mWebServer::RespondWebSendFreeMemoryTooLow(AsyncWebServerRequest *request, uint16_t memory_needed){
//   if(!memory_needed){
//     memory_needed = 2000;
//   }
//   if(ESP.getFreeHeap()<memory_needed){
//     char message[45];
//     sprintf(message,PSTR("Memory too low - %d needed (%d free)"),memory_needed,ESP.getFreeHeap());
//     request->send(413, CONTENT_TYPE_TEXT_HTML_ID, message);
//     return true;
//   }
//   return false;
// }

// bool mWebServer::RespondWebSend_UnableToAllocateBuffer(AsyncWebServerRequest *request){
//   char message[45];
//   sprintf(message,PSTR("Unable to allocate buffer (FreeHeap %d)"),ESP.getFreeHeap());
//   request->send(413, CONTENT_TYPE_TEXT_HTML_ID, message);
// }


/**
 * @brief 
 * 
 * @param id 
 * @param buffer 
 * @param buflen 
 * @return char* 
 * 
 * I dont need to copy as its progmem
 */
const char* mWebServer::GetContentTypeCtrP_By_ID(uint8_t id)
{
  
  // switch(_contentType_id){
  //   default:
  //   case CONTENT_TYPE_TEXT_HTML_ID: 
  //     // snprintf_P(buffer, sizeof(PM_WEB_CONTENT_TYPE_TEXT_HTML), PM_WEB_CONTENT_TYPE_TEXT_HTML); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_HTML;
  //   break; // Necessary casts and dereferencing, just copy.
  //   case CONTENT_TYPE_TEXT_JAVASCRIPT_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT)); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT;
  //   break;
  //   case CONTENT_TYPE_APPLICATION_JSON_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT));
  //     return PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT;
  //   break;
  //   case CONTENT_TYPE_TEXT_CSS_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_TEXT_CSS, sizeof(PM_WEB_CONTENT_TYPE_TEXT_CSS)); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_CSS;
  //   break;

  // }
  

  // return buffer;

}




// Handle all webpage sends to allow for checking of freespace
void mWebServer::WebSend_Response(AsyncWebServerRequest *request, int code, uint8_t contentType_id, char* content_ptr){

  // Work out memory needed relative to body we want to send?
  // Serial.println("HERE"); Serial.flush();

  // char content_type[30]; // Should exist until send completes, might not as its async though
  #ifdef DEBUG_WEBSERVER_MEMORY
    FreeMem_Usage_Before(&freemem_usage_json_shared);
  #endif
  // WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  
 
  // Check if there is enough RAM space, or else respond with 
  // if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 
  // Contine to send requested data 
  // request->send(code, contentType_id, content_ptr); 

   #ifdef DEBUG_WEBSERVER_MEMORY
    FreeMem_Usage_After(&freemem_usage_json_shared);
  #endif 

}


// #ifdef DEBUG_WEBSERVER_MEMORY
// void mWebServer::FreeMem_Usage_Before(freemem_usage_t* freemem){
//   freemem->free_bytes = ESP.getFreeHeap();
// }
// void mWebServer::FreeMem_Usage_After(freemem_usage_t* freemem){
//   freemem->bytes_used = freemem->free_bytes - ESP.getFreeHeap();
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_ASYNC "Free \"%s\"=%d/%d %d space"), //used of previously free, additional space that remained free
//     freemem->name_ptr,
//     freemem->bytes_used,freemem->free_bytes,
//     ESP.getFreeHeap());
// }
// #endif




// void mWebServer::StopWebserver(void)
// {
//   //if(webserver_state) {
//     pWebServer->reset(); // asyncedit
//     webserver_state = HTTP_OFF;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   //}
// }

// void mWebServer::WifiManagerBegin(bool reset_only)
// {
  
//   ALOG_INF(PSTR(D_LOG_DEBUG "mWebServer::WifiManagerBegin"));

//   // setup AP
//   if (!pCONT_set->global_state.wifi_down) {
//     WiFi.mode(WIFI_AP_STA);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   } else {
//     WiFi.mode(WIFI_AP);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   }

//   StopWebserver();

//   DnsServer = new DNSServer();

//   int channel = WIFI_SOFT_AP_CHANNEL;
//   if ((channel < 1) || (channel > 13)) { channel = 1; }
//   WiFi.softAP(pCONT_set->my_hostname, nullptr, channel);

//   delay(500); // Without delay I've seen the IP address blank
//   /* Setup the DNS server redirecting all the domains to the apIP */
//   DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
//   DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

//   StartWebserver((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER), WiFi.softAPIP());

// }

// void mWebServer::PollDnsWebserver(void)
// {
//   if (DnsServer) { DnsServer->processNextRequest(); }
// }

// /*********************************************************************************************/

// bool mWebServer::WebAuthenticate(void)
// {
//   // if (pCONT_set->Settings.web_password[0] != 0 && HTTP_MANAGER_RESET_ONLY != webserver_state) {
//   //   // return pWebServer->authenticate(WEB_USERNAME, pCONT_set->Settings.web_password);
//   // } else {
//     return true;
//   // }
// }

// //bool mWebServer::HttpCheckPriviledgedAccess(bool autorequestauth = true)
// bool mWebServer::HttpCheckPriviledgedAccess()
// {

// //     bool autorequestauth = true;

// //   if (HTTP_USER == webserver_state) {
// //     HandleRoot();
// //     return false;
// //   }
// //   if (autorequestauth && !WebAuthenticate()) {
// //     pWebServer->requestAuthentication();
// //     return false;
// //   }
//   return true; // admin by default
// }



// void mWebServer::send_mP(AsyncWebServerRequest *request, int code, uint8_t content_type, const char* formatP, ...)     // Content send snprintf_P char data
// {
//   D_DATA_BUFFER_CLEAR();
//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
//   va_end(arg);  
//   request->send_P(code, content_type, data_buffer.payload.ctr);
// }

// // void mWebServer::WSContentSend_PD(AsyncWebServerRequest *request, const char* formatP, ...)    // Content send snprintf_P char data checked for decimal separator
// // {

// //   // This uses char strings. Be aware of sending %% if % is needed
// //   va_list arg;
// //   va_start(arg, formatP);
// //   int len = vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
// //   va_end(arg);

// //   if (D_DECIMAL_SEPARATOR[0] != '.') {
// //     for (int i = 0; i < len; i++) {
// //       if ('.' == data_buffer.payload.ctr[i]) {
// //         data_buffer.payload.ctr[i] = D_DECIMAL_SEPARATOR[0];
// //       }
// //     }
// //   }
// // }







// // void mWebServer::WebHexCode(uint8_t i, const char* code)
// // {
// //   char scolor[10];

// //   strlcpy(scolor, code, sizeof(scolor));
// //   char* p = scolor;
// //   if ('#' == p[0]) { p++; }  // Skip

// //   if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
// //     p[6] = p[3];  // \0
// //     p[5] = p[2];  // 3
// //     p[4] = p[2];  // 3
// //     p[3] = p[1];  // 2
// //     p[2] = p[1];  // 2
// //     p[1] = p[0];  // 1
// //   }

// //   uint32_t color = strtol(p, nullptr, 16);
// // /*
// //   if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
// //     uint32_t w = ((color & 0xF00) << 8) | ((color & 0x0F0) << 4) | (color & 0x00F);  // 00010203
// //     color = w | (w << 4);                                                            // 00112233
// //   }
// // */

// //   pCONT_set->Settings.web_color[i][0] = (color >> 16) & 0xFF;  // Red
// //   pCONT_set->Settings.web_color[i][1] = (color >> 8) & 0xFF;   // Green
// //   pCONT_set->Settings.web_color[i][2] = color & 0xFF;          // Blue
// // }

// // uint32_t mWebServer::WebColor(uint8_t i)
// // {
// //   return (pCONT_set->Settings.web_color[i][0] << 16) | (pCONT_set->Settings.web_color[i][1] << 8) | pCONT_set->Settings.web_color[i][2];
// // }

// uint32_t mWebServer::WebColor(uint8_t r, uint8_t g, uint8_t b){
//   return (r << 16) | (g << 8) | b;
// }

// const char* mWebServer::WebColorCtr(uint8_t r, uint8_t g, uint8_t b, char* buffer, uint8_t buflen){
//   snprintf_P(buffer, buflen, PSTR("#%02X%02X%02X"), r, g, b);
//   return buffer;
// }

// // const char* mWebServer::WebColorCtr(char* buffer, uint8_t r,uint8_t g,uint8_t b){
// //   sprintf(buffer,PSTR("#%02X%02X%02X\0"),r,g,b);
// //   return buffer;
// // }






// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleFavicon(AsyncWebServerRequest *request){
//   // request->send_P(200, "image/x-icon", (char*)favicon, 156);
//   // request->send_P(200, "image/x-icon", (char*)favicon, sizeof(favicon));  
//   // request->send_P(200, "image/x-icon", favicon, 156);
// }







// /*-------------------------------------------------------------------------------------------*/

// // String mWebServer::htmlEscape(String s)
// // {
// //   s.replace("&", "&amp;");
// //   s.replace("<", "&lt;");
// //   s.replace(">", "&gt;");
// //   s.replace("\"", "&quot;");
// //   s.replace("'", "&#x27;");
// //   s.replace("/", "&#x2F;");
// //   return s;
// // }
// /*-------------------------------------------------------------------------------------------*/

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleHttpCommand(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess(false)) { return; }

//   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

//   // uint8_t valid = 1;
//   // if (pCONT_set->Settings.web_password[0] != 0) {
//   //   char tmp1[sizeof(pCONT_set->Settings.web_password)];
//   //   WebGetArg(request,"user", tmp1, sizeof(tmp1));
//   //   char tmp2[sizeof(pCONT_set->Settings.web_password)];
//   //   WebGetArg(request,"password", tmp2, sizeof(tmp2));
//   //   if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, pCONT_set->Settings.web_password))) { valid = 0; }
//   // }

//   // WSContentBegin(request, 200, CT_JSON);
//   // if (valid) {
//   //   uint8_t curridx = pCONT_set->web_log_index;
//   //   String svalue = request->arg("cmnd");
//   //   if (svalue.length() && (svalue.length() < INPUT_BUFFER_SIZE)) {
//   //     ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCOMMAND);
//   //     if (pCONT_set->web_log_index != curridx) {
//   //       uint8_t counter = curridx;
//   //       WSBufferAppend_P(response, PSTR("{"));
//   //       bool cflg = false;
//   //       do {
//   //         char* tmp;
//   //         size_t len;
//   //         pCONT->mso->GetLog(counter, &tmp, &len);
//   //         if (len) {
//   //           // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
//   //           char* JSON = (char*)memchr(tmp, '{', len);
//   //           if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
//   //             size_t JSONlen = len - (JSON - tmp);
//   //             if (JSONlen > sizeof(data_buffer.payload.ctr)) { JSONlen = sizeof(data_buffer.payload.ctr); }
//   //             char stemp[JSONlen];
//   //             strlcpy(stemp, JSON +1, JSONlen -2);
//   //             WSBufferAppend_P(response, PSTR("%s%s"), (cflg) ? "," : "", stemp);
//   //             cflg = true;
//   //           }
//   //         }
//   //         counter++;
//   //         if (!counter) counter++;  // Skip 0 as it is not allowed
//   //       } while (counter != pCONT_set->web_log_index);
//   //       WSBufferAppend_P(response, PSTR("}"));
//   //     } else {
//   //       WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENABLE_WEBLOG_FOR_RESPONSE "\"}"));
//   //     }
//   //   } else {
//   //     WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENTER_COMMAND " cmnd=\"}"));
//   //   }
//   // } else {
//   //   WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
//   // }
//   // WSContentEnd(request);
// }

// /*-------------------------------------------------------------------------------------------*/




// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Other helpers
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// /* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
// bool mWebServer::CaptivePortal(AsyncWebServerRequest *request)
// {
//   // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
//   if ((WifiIsInManagerMode())){//} && !pCONT_sup->ValidIpAddress(request->hostHeader().c_str())) {
//     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));
//     // request->sendHeader(F("Location"), String("http://") + request->client().localIP().toString(), true);


//     // WSSend(302, CT_PLAIN, "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    
//     // AsyncWebServerResponse *response = request->beginResponse(302);
//     // response->addHeader("Location", "http://4.3.2.1");
//     // request->send(response);

//     // request->client().stop();  // Stop is needed because we sent no content length
//     return true;
//   }
//   return false;
// }

// /*********************************************************************************************/

// // String mWebServer::UrlEncode(const String& text)
// // {
// //   // #ifndef DISABLE_SERIAL_LOGGING
// //   // P_PHASE_OUT();
// //   // #endif
// //   const char hex[] = "0123456789ABCDEF";

// // 	String encoded = "";
// // 	int len = text.length();
// // 	int i = 0;
// // 	while (i < len)	{
// // 		char decodedChar = text.charAt(i++);

// // /*
// //     if (('a' <= decodedChar && decodedChar <= 'z') ||
// //         ('A' <= decodedChar && decodedChar <= 'Z') ||
// //         ('0' <= decodedChar && decodedChar <= '9') ||
// //         ('=' == decodedChar)) {
// //       encoded += decodedChar;
// // 		} else {
// //       encoded += '%';
// // 			encoded += hex[decodedChar >> 4];
// // 			encoded += hex[decodedChar & 0xF];
// //     }
// // */
// //     if (' ' == decodedChar) {
// //       encoded += '%';
// // 			encoded += hex[decodedChar >> 4];
// // 			encoded += hex[decodedChar & 0xF];
// //     } else {
// //       encoded += decodedChar;
// //     }

// // 	}
// // 	return encoded;
// // }

// // int mWebServer::WebSend(char *buffer)
// // {
// //   // [sonoff] POWER1 ON                                               --> Sends http://sonoff/cm?cmnd=POWER1 ON
// //   // [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
// //   // [sonoff] /any/link/starting/with/a/slash.php?log=123             --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123
// //   // [sonoff,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123

// //   char *host;
// //   char *user;
// //   char *password;
// //   char *command;
// //   int status = 1;                             // Wrong parameters

// //                                               // buffer = |  [  192.168.178.86  :  80  ,  admin  :  joker  ]    POWER1 ON   |
// //   host = strtok_r(buffer, "]", &command);     // host = |  [  192.168.178.86  :  80  ,  admin  :  joker  |, command = |    POWER1 ON   |
// //   if (host && command) {
// //     pCONT_sup->RemoveSpace(host);                        // host = |[192.168.178.86:80,admin:joker|
// //     host++;                                   // host = |192.168.178.86:80,admin:joker| - Skip [
// //     host = strtok_r(host, ",", &user);        // host = |192.168.178.86:80|, user = |admin:joker|
// //     String url = F("http://");                // url = |http://|
// //     url += host;                              // url = |http://192.168.178.86:80|

// //     command = pCONT_sup->Trim(command);                  // command = |POWER1 ON| or |/any/link/starting/with/a/slash.php?log=123|
// //     if (command[0] != '/') {
// //       url += F("/cm?");                       // url = |http://192.168.178.86/cm?|
// //       if (user) {
// //         user = strtok_r(user, ":", &password);  // user = |admin|, password = |joker|
// //         if (user && password) {
// //           char userpass[128];
// //           snprintf_P(userpass, sizeof(userpass), PSTR("user=%s&password=%s&"), user, password);
// //           url += userpass;                    // url = |http://192.168.178.86/cm?user=admin&password=joker&|
// //         }
// //       }
// //       url += F("cmnd=");                      // url = |http://192.168.178.86/cm?cmnd=| or |http://192.168.178.86/cm?user=admin&password=joker&cmnd=|
// //     }
// //     url += command;                           // url = |http://192.168.178.86/cm?cmnd=POWER1 ON|

// // //AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Uri |%s|"), url.c_str());

// // #if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
// //     HTTPClient http;
// //     if (http.begin(UrlEncode(url))) {         // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
// // #else
// //     WiFiClient http_client;
// //     HTTPClient http;
// //     if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
// // #endif
// //       int http_code = http.GET();             // Start connection and send HTTP header
// //       if (http_code > 0) {                    // http_code will be negative on error
// //         if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
// // /*
// //           // Return received data to the user - Adds 900+ bytes to the code
// //           String result = http.getString();   // File found at server - may need lot of ram or trigger out of memory!
// //           uint16_t j = 0;
// //           for (uint16_t i = 0; i < result.length(); i++) {
// //             char text = result.charAt(i);
// //             if (text > 31) {                  // Remove control characters like linefeed
// //               data_buffer.payload.ctr[j++] = text;
// //               if (j == sizeof(data_buffer.payload.ctr) -2) { break; }
// //             }
// //           }
// //           data_buffer.payload.ctr[j] = '\0';
// //           MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_WEBSEND));
// // */
// //         }
// //         status = 0;                           // No error - Done
// //       } else {
// //         status = 2;                           // Connection failed
// //       }
// //       http.end();                             // Clean up connection data
// //     } else {
// //       status = 3;                             // Host not found or connection error
// //     }
// //   }
// //   return status;
// // }

// /*********************************************************************************************/

// // bool mWebServer::JsonWebColor(const char* dataBuf)
// // {
// //   // Default (light)
// //   // {"WebColor":["#000000","#ffffff","#f2f2f2","#000000","#ffffff","#000000","#ffffff","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}
// //   // Alternative (Dark)
// //   // {"webcolor":["#eeeeee","#181818","#4f4f4f","#000000","#dddddd","#008000","#222222","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}

// //   // char dataBufLc[strlen(dataBuf) +1];
// //   // pCONT_sup->LowerCase(dataBufLc, dataBuf);
// //   // pCONT_sup->RemoveSpace(dataBufLc);
// //   // if (strlen(dataBufLc) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

// //   // StaticJsonDocument<450>  doc;
// //   // DeserializationError error =  deserializeJson(doc, dataBuf);
// //   // JsonObject obj = doc.to<JsonObject>();
// //   // if (error) { return false; }

// //   // // StaticJsonBuffer<450> jb;  // 421 from https://arduinojson.org/v5/assistant/
// //   // // JsonObject& obj = jb.parseObject(dataBufLc);
// //   // // if (!obj.success()) { return false; }

// //   // char parm_lc[10];
// //   // if (obj[pCONT_sup->LowerCase(parm_lc, D_JSON_WEBCOLOR)].isNull()) {
// //   //   for (uint8_t i = 0; i < pCONT_set->COL_LAST; i++) {
// //   //     const char* color = obj[parm_lc][i];
// //   //     if (color != nullptr) {
// //   //       WebHexCode(i, color);
// //   //     }
// //   //   }
// //   // }
// //   return true;
// // }

// enum WebCommands { CMND_WEBSERVER, CMND_WEBPASSWORD, CMND_WEBLOG, CMND_WEBREFRESH, CMND_WEBSEND, CMND_WEBCOLOR, CMND_EMULATION };
// // const char kWebCommands[] PROGMEM = D_JSON_WEBSERVER "|" D_JSON_WEBPASSWORD "|" D_JSON_WEBLOG "|" D_JSON_WEBREFRESH "|" D_JSON_WEBSEND "|" D_JSON_WEBCOLOR "|" D_JSON_EMULATION ;
// // const char kWebSendStatus[] PROGMEM = D_JSON_DONE "|" D_JSON_WRONG_PARAMETERS "|" D_JSON_CONNECT_FAILED "|" D_JSON_HOST_NOT_FOUND ;

// // bool mWebServer::WebCommand(void)
// // {
// //   char command[CMDSZ];
// //   bool serviced = true;

// // //   int command_code = pCONT_sup->GetCommandCode(command, sizeof(command), pCONT_set->XdrvMailbox.topic, kWebCommands);
// // //   if (-1 == command_code) {
// // //     serviced = false;  // Unknown command
// // //   }
// // //   if (CMND_WEBSERVER == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload >= 0) && (pCONT_set->XdrvMailbox.payload <= 2)) { pCONT_set->Settings.webserver = pCONT_set->XdrvMailbox.payload; }
// // //     if (pCONT_set->Settings.webserver) {
// // //       pCONT_sup->Response_P(PSTR("{\"" D_JSON_WEBSERVER "\":\"" D_JSON_ACTIVE_FOR " %s " D_JSON_ON_DEVICE " %s " D_JSON_WITH_IP_ADDRESS " %s\"}"),
// // //         (2 == pCONT_set->Settings.webserver) ? D_ADMIN : D_USER, pCONT_set->my_hostname, WiFi.localIP().toString().c_str());
// // //     } else {
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_sup->GetStateText(0));
// // //     }
// // //   }
// // //   else if (CMND_WEBPASSWORD == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.data_len > 0) && (pCONT_set->XdrvMailbox.data_len < sizeof(pCONT_set->Settings.web_password))) {
// // //       strlcpy(pCONT_set->Settings.web_password, (SC_CLEAR == pCONT_sup->Shortcut(pCONT_set->XdrvMailbox.data)) ? "" : (SC_DEFAULT == pCONT_sup->Shortcut(pCONT_set->XdrvMailbox.data)) ? WEB_PASSWORD : pCONT_set->XdrvMailbox.data, sizeof(pCONT_set->Settings.web_password));
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_set->Settings.web_password);
// // //     } else {
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_ASTERIX, command);
// // //     }
// // //   }
// // //   else if (CMND_WEBLOG == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload >= LOG_LEVEL_NONE) && (pCONT_set->XdrvMailbox.payload <= LOG_LEVEL_ALL)) { pCONT_set->Settings.weblog_level = pCONT_set->XdrvMailbox.payload; }
// // //     pCONT_sup->Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.weblog_level);
// // //   }
// // //   else if (CMND_WEBREFRESH == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload > 999) && (pCONT_set->XdrvMailbox.payload <= 10000)) { pCONT_set->Settings.web_refresh = pCONT_set->XdrvMailbox.payload; }
// // //     pCONT_sup->Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.web_refresh);
// // //   }
// // //   else if (CMND_WEBSEND == command_code) {
// // //     if (pCONT_set->XdrvMailbox.data_len > 0) {
// // //       uint8_t result = WebSend(request, pCONT_set->XdrvMailbox.data);
// // //       char stemp1[20];
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), result, kWebSendStatus));
// // //     }
// // //   }
// // //   else if (CMND_WEBCOLOR == command_code) {
// // //     if (pCONT_set->XdrvMailbox.data_len > 0) {
// // //       if (strstr(pCONT_set->XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
// // //         if ((pCONT_set->XdrvMailbox.data_len > 3) && (pCONT_set->XdrvMailbox.index > 0) && (pCONT_set->XdrvMailbox.index <= pCONT_set->COL_LAST)) {
// // //           pCONT_sup->WebHexCode(pCONT_set->XdrvMailbox.index -1, pCONT_set->XdrvMailbox.data);
// // //         }
// // //         else if (0 == pCONT_set->XdrvMailbox.payload) {
// // //           pCONT_set->SettingsDefaultWebColor();
// // //         }
// // //       }
// // //       else {
// // //         JsonWebColor(pCONT_set->XdrvMailbox.data);
// // //       }
// // //     }
// // //     pCONT_sup->Response_P(PSTR("{\"" D_JSON_WEBCOLOR "\":["));
// // //     for (uint8_t i = 0; i < pCONT_set->COL_LAST; i++) {
// // //       if (i) { pCONT_sup->ResponseAppend_P(PSTR(",")); }
// // //       pCONT_sup->ResponseAppend_P(PSTR("\"#%06x\""), WebColor(i));
// // //     }
// // //     pCONT_sup->ResponseAppend_P(PSTR("]}"));
// // //   }
// // // #ifdef USE_EMULATION
// // //   else if (CMND_EMULATION == command_code) {
// // //     if ((XdrvMailbox.payload >= EMUL_NONE) && (XdrvMailbox.payload < EMUL_MAX)) {
// // //       pCONT_set->Settings.flag_power.emulation = XdrvMailbox.payload;
// // //       restart_flag = 2;
// // //     }
// // //     Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.flag_power.emulation);
// // //   }
// // // #endif  // USE_EMULATION
// // //   else serviced = false;  // Unknown command

// //   return serviced;
// // }



// // void mWebServer::ExecuteCommand(char *cmnd, int source)
// // {
// // //   char stopic[CMDSZ];
// // //   char svalue[INPUT_BUFFER_SIZE];
// // //   char *start;
// // //   char *token;

// // //  //ShowFreeMem(PSTR("ExecuteCommand"));
// // //   pCONT_sup->ShowSource(source);

// // // //In the first call to the strtok() function for a given string1, the strtok() function searches for the first token in string1, 
// // // //skipping over leading delimiters. A pointer to the first token is returned.

// // //   token = strtok(cmnd, " "); // split, on each call returns next split
// // //   if (token != NULL) { //if end is not reached
// // //     start = strrchr(token, '/');   // Skip possible cmnd/sonoff/ preamble
// // //     if (start) { token = start +1; }
// // //   }
// // //   snprintf_P(stopic, sizeof(stopic), PSTR("%s"), (token == NULL) ? "" : token);
// // //   token = strtok(NULL, "");
// // // //  snprintf_P(svalue, sizeof(svalue), (token == NULL) ? "" : token);  // Fails with command FullTopic home/%prefix%/%topic% as it processes %p of %prefix%
// // //   strlcpy(svalue, (token == NULL) ? "" : token, sizeof(svalue));       // Fixed 5.8.0b
// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "ExecuteCommand" " stopic %s" " svalue %s" " strlen(svalue) %d" ),stopic,svalue,strlen(svalue));

// // //   //MqttDataHandler(stopic, (uint8_t*)svalue, strlen(svalue));
// // //   ParseAndExecuteWebCommands(stopic, (uint8_t*)svalue, strlen(svalue));
 
// // }


// /**
//  Commands have form: "<name><index> <value>" e.g."button2 1" or "slider3 123"
//  <name> is in format classname::command but can contain NO numbers (ascii string only)
// */
// // void mWebServer::ParseAndExecuteWebCommands(char* topic, uint8_t* data, unsigned int data_len)
// // {

// // //   if (data_len > MQTT_MAX_PACKET_SIZE) { return; }  // Do not allow more data than would be feasable within stack space

// // //   char *str;

// // //   char topicBuf[TOPSZ];
// // //   char dataBuf[data_len+1];
// // //   char command [CMDSZ];
// // //   char stemp1[TOPSZ];
// // //   char *p;
// // //   char *type = nullptr;
// // //   uint8_t lines = 1;
// // //   bool jsflg = false;
// // //   bool grpflg = false;
// // //   bool user_append_index = false;
// // //   uint32_t i = 0;
// // //   uint32_t index;
// // //   uint32_t address;
  
// // //   // memset(&webcommand,0,sizeof(webcommand));

// // // /******************************************************************************************************
// // //  *******Extract <name><index> as topicBuf, <value> as dataBuf using space delimeter ***********************************************************************************************
// // //  ******************************************************************************************************/
 
// // //   // "string numbervalue" - split into topic and data buffers
// // //   strlcpy(topicBuf, topic, sizeof(topicBuf));
// // //   for (i = 0; i < data_len; i++) {
// // //     if (!isspace(data[i])) { break; } 
// // //   }
// // //   data_len -= i;
// // //   memcpy(dataBuf, data +i, sizeof(dataBuf));
// // //   dataBuf[sizeof(dataBuf)-1] = 0;
  
// // //   //copy command value
// // //   memcpy(&webcommand.command_value,dataBuf,sizeof(dataBuf));

// // //   //if (topicBuf[0] != '/') { ShowSource(SRC_MQTT); } //?

// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_RECEIVED_TOPIC " %s, " D_DATA_SIZE " %d, " D_DATA " %s"), topicBuf, data_len, dataBuf);
 
// // //   // Check classname exists
// // //   if(strstr(topicBuf,"::")){
// // //     // copy classname
// // //     for (i = 0; i < strlen(topicBuf); i++) {
// // //       if (topicBuf[i]==':') { break; } 
// // //       webcommand.classname[i] = topicBuf[i];
// // //     }
// // //   }

// // //   //from i, we have the command_key_short (skip :: values)
// // //   memcpy(&webcommand.command_key_short,&topicBuf[i+2],sizeof(char)*(strlen(topicBuf)-i-2));

// // // /******************************************************************************************************
// // //  *******Extract <index> from topicBuf with isdigit ***********************************************************************************************
// // //  ******************************************************************************************************/
 
// // //   //type = strrchr(topicBuf, '/');  // Last part of received topic is always the command (type)
// // //   type = topicBuf;//, '/');  // Last part of received topic is always the command (type)

// // //   index = 1;
// // //   if (type != nullptr) {
// // //     type++;
// // //     // Convert everything to same case
// // //     for (i = 0; i < strlen(type); i++) {
// // //       //type[i] = toupper(type[i]);
// // //     }
// // //     // From end of array, move backwards checking for numbers (ie stops when it hits " " space)
// // //     while (isdigit(type[i-1])) {
// // //       i--;
// // //     }
// // //     // Move from start of number to end of array to get number
// // //     if (i < strlen(type)) {
// // //       index = atoi(type +i);
// // //       user_append_index = true;
// // //     }
// // //     type[i] = '\0';
// // //   }

// // //   // remove index from 
// // //   if(user_append_index){
// // //     webcommand.command_key_short[strlen(webcommand.command_key_short)-mSupport::NumDigits(index)] = 0;
// // //   }

// // //   //copy index
// // //   webcommand.command_index = index;

// // //   webcommand.fWaiting = true;
  
// // //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESULT D_GROUP " %d, " D_INDEX " %d, " 
// // //     D_COMMAND " %s, " D_DATA " %s"),
// // //    grpflg, index, type, dataBuf);


// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT "WEBCOMMANDS " "classname %s"),webcommand.classname);
// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_key_short %s"),webcommand.command_key_short);
// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_value %s"),webcommand.command_value);
// // //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_index %d"),webcommand.command_index);
  
// // //   pCONT->Tasker_Interface(FUNC_WEB_COMMAND);

// // } // END function
/*
 * Binary array for the Web UI.
 * gzip is used for smaller size and improved speeds.
 * 
 * Please see https://kno.wled.ge/advanced/custom-features/#changing-web-ui
 * to find out how to easily modify the web UI source!
 */
 
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

// #if defined(ESP32)
// AsyncWebServer server(80);
// #endif  // ESP32

// define flash strings once (saves flash memory)
static const char s_redirecting[] PROGMEM = "Redirecting...";
static const char s_content_enc[] PROGMEM = "Content-Encoding";
static const char s_unlock_ota [] PROGMEM = "Please unlock OTA in security settings!";
static const char s_unlock_cfg [] PROGMEM = "Please unlock settings using PIN code!";


String messageHead, messageSub;
byte optionType;

String msgProcessor(const String& var)
{
  if (var == "MSG") {
    String messageBody = messageHead;
    messageBody += F("</h2>");
    messageBody += messageSub;
    uint32_t optt = optionType;

    if (optt < 60) //redirect to settings after optionType seconds
    {
      messageBody += F("<script>setTimeout(RS,");
      messageBody +=String(optt*1000);
      messageBody += F(")</script>");
    } else if (optt < 120) //redirect back after optionType-60 seconds, unused
    {
      //messageBody += "<script>setTimeout(B," + String((optt-60)*1000) + ")</script>";
    } else if (optt < 180) //reload parent after optionType-120 seconds
    {
      messageBody += F("<script>setTimeout(RP,");
      messageBody += String((optt-120)*1000);
      messageBody += F(")</script>");
    } else if (optt == 253)
    {
      messageBody += F("<br><br><form action=/settings><button class=\"bt\" type=submit>Back</button></form>"); //button to settings
    } else if (optt == 254)
    {
      messageBody += F("<br><br><button type=\"button\" class=\"bt\" onclick=\"B()\">Back</button>");
    }
    return messageBody;
  }
  return String();
}


void serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT)
{
  messageHead = headl;
  messageSub = subl;
  optionType = optionT;

  request->send_P(code, "text/html", PAGE_msg, msgProcessor);
}


AsyncWebHandler *editHandler = nullptr;

void createEditHandler(bool enable) {
  if (editHandler != nullptr) pCONT_web->server->removeHandler(editHandler);
  if (enable) {
    #ifdef WLED_ENABLE_FS_EDITOR
      #ifdef ARDUINO_ARCH_ESP32
      editHandler = &server.addHandler(new SPIFFSEditor(WLED_FS));//http_username,http_password));
      #else
      editHandler = &server.addHandler(new SPIFFSEditor("","",WLED_FS));//http_username,http_password));
      #endif
    #else
      editHandler = &pCONT_web->server->on("/edit", HTTP_GET, [](AsyncWebServerRequest *request){
        serveMessage(request, 501, "Not implemented", F("The FS editor is disabled in this build."), 254);
      });
    #endif
  } else {
    editHandler = &pCONT_web->server->on("/edit", HTTP_ANY, [](AsyncWebServerRequest *request){
      serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_cfg), 254);
    });
  }
}

void setStaticContentCacheHeaders(AsyncWebServerResponse *response)
{
  char tmp[12];
  // https://medium.com/@codebyamir/a-web-developers-guide-to-browser-caching-cc41f3b73e7c
  #ifndef WLED_DEBUG
  //this header name is misleading, "no-cache" will not disable cache,
  //it just revalidates on every load using the "If-None-Match" header with the last ETag value
  response->addHeader(F("Cache-Control"),"no-cache");
  #else
  response->addHeader(F("Cache-Control"),"no-store,max-age=0"); // prevent caching if debug build
  #endif
  // sprintf_P(tmp, PSTR("%8d-%02x"), PROJECT_VERSION, cacheInvalidate);
  sprintf_P(tmp, PSTR("%8d-%02x"), 123, 1);
  response->addHeader(F("ETag"), tmp);
}

void serveIndex(AsyncWebServerRequest* request)
{
  // if (handleFileRead(request, "/index.htm")) return;

  // if (handleIfNoneMatchCacheHeader(request)) return;

  AsyncWebServerResponse *response;
#ifdef WLED_ENABLE_SIMPLE_UI
  if (simplifiedUI)
    response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
  else
#endif
    response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);

  response->addHeader(FPSTR(s_content_enc),"gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}


// Settings sub page IDs
#define SUBPAGE_MENU              0
#define SUBPAGE_WIFI              1
#define SUBPAGE_LEDS              2
#define SUBPAGE_UI                3
#define SUBPAGE_SYNC              4
#define SUBPAGE_TIME              5
#define SUBPAGE_SEC               6
#define SUBPAGE_DMX               7
#define SUBPAGE_UM                8
#define SUBPAGE_UPDATE            9
#define SUBPAGE_2D               10
#define SUBPAGE_LOCK            251
#define SUBPAGE_PINREQ          252
#define SUBPAGE_CSS             253
#define SUBPAGE_JS              254
#define SUBPAGE_WELCOME         255

// string temp buffer (now stored in stack locally)
#ifdef ESP8266
#define SETTINGS_STACK_BUF_SIZE 2048
#else
#define SETTINGS_STACK_BUF_SIZE 3608  // warning: quite a large value for stack
#endif

bool correctPIN = true;
char settingsPIN[5] = {"1234"};  // PIN for settings pages


// Temp buffer
char* obuf;
uint16_t olen = 0;


//macro to convert F to const
#define SET_F(x)  (const char*)F(x)

bool oappend(const char* txt)
{
  uint16_t len = strlen(txt);
  if (olen + len >= SETTINGS_STACK_BUF_SIZE)
    return false;        // buffer full
  strcpy(obuf + olen, txt);
  olen += len;
  return true;
}


bool oappendi(int i)
{
  char s[11];
  sprintf(s, "%d", i);
  return oappend(s);
}

//append a string setting to buffer
void sappends(char stype, const char* key, char* val)
{
  switch(stype)
  {
    case 's': {//string (we can interpret val as char*)
      String buf = val;
      //convert "%" to "%%" to make EspAsyncWebServer happy
      //buf.replace("%","%%");
      oappend("d.Sf.");
      oappend(key);
      oappend(".value=\"");
      oappend(buf.c_str());
      oappend("\";");
      break;}
    case 'm': //message
      oappend(SET_F("d.getElementsByClassName"));
      oappend(key);
      oappend(SET_F(".innerHTML=\""));
      oappend(val);
      oappend("\";");
      break;
  }
}


//get values for settings form in javascript
void getSettingsJS(byte subPage, char* dest)
{
  //0: menu 1: wifi 2: leds 3: ui 4: sync 5: time 6: sec
  DEBUG_PRINT(F("settings resp"));
  DEBUG_PRINTLN(subPage);
  obuf = dest;
  olen = 0;

  if (subPage <0 || subPage >10) return;

  if (subPage == SUBPAGE_MENU)
  {
  #ifndef WLED_DISABLE_2D // include only if 2D is compiled in
    oappend(PSTR("gId('2dbtn').style.display='';"));
  #endif
  #ifdef WLED_ENABLE_DMX // include only if DMX is enabled
    oappend(PSTR("gId('dmxbtn').style.display='';"));
  #endif
  }

  if (subPage == SUBPAGE_WIFI)
  {
    sappends('s',SET_F("CS"),"clientSSID");

    // byte l = strlen(clientPass);
    // char fpass[l+1]; //fill password field with ***
    // fpass[l] = 0;
    // memset(fpass,'*',l);
    // sappends('s',SET_F("CP"),fpass);

    // char k[3]; k[2] = 0; //IP addresses
    // for (int i = 0; i<4; i++)
    // {
    //   k[1] = 48+i; //ascii 0,1,2,3
    //   k[0] = 'I'; sappend('v',k,staticIP[i]);
    //   k[0] = 'G'; sappend('v',k,staticGateway[i]);
    //   k[0] = 'S'; sappend('v',k,staticSubnet[i]);
    // }

    // sappends('s',SET_F("CM"),cmDNS);
    // sappend('i',SET_F("AB"),apBehavior);
    // sappends('s',SET_F("AS"),apSSID);
    // sappend('c',SET_F("AH"),apHide);

    // l = strlen(apPass);
    // char fapass[l+1]; //fill password field with ***
    // fapass[l] = 0;
    // memset(fapass,'*',l);
    // sappends('s',SET_F("AP"),fapass);

    // sappend('v',SET_F("AC"),apChannel);
    // sappend('c',SET_F("WS"),noWifiSleep);

    // #ifndef WLED_DISABLE_ESPNOW
    // sappend('c',SET_F("RE"),enable_espnow_remote);
    // sappends('s',SET_F("RMAC"),linked_remote);
    // #else
    // //hide remote settings if not compiled
    // oappend(SET_F("document.getElementById('remd').style.display='none';"));
    // #endif

    // #ifdef WLED_USE_ETHERNET
    // sappend('v',SET_F("ETH"),ethernetType);
    // #else
    // //hide ethernet setting if not compiled in
    // oappend(SET_F("document.getElementById('ethd').style.display='none';"));
    // #endif

    // if (Network.isConnected()) //is connected
    // {
    //   char s[32];
    //   IPAddress localIP = Network.localIP();
    //   sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

    //   #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_ETHERNET)
    //   if (Network.isEthernet()) strcat_P(s ,SET_F(" (Ethernet)"));
    //   #endif
    //   sappends('m',SET_F("(\"sip\")[0]"),s);
    // } else
    // {
    //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("Not connected"));
    // }

    // if (WiFi.softAPIP()[0] != 0) //is active
    // {
    //   char s[16];
    //   IPAddress apIP = WiFi.softAPIP();
    //   sprintf(s, "%d.%d.%d.%d", apIP[0], apIP[1], apIP[2], apIP[3]);
    //   sappends('m',SET_F("(\"sip\")[1]"),s);
    // } else
    // {
    //   sappends('m',SET_F("(\"sip\")[1]"),(char*)F("Not active"));
    // }

    // #ifndef WLED_DISABLE_ESPNOW
    // if (last_signal_src[0] != 0) //Have seen an ESP-NOW Remote
    // {
    //   sappends('m',SET_F("(\"rlid\")[0]"),last_signal_src);
    // } else if (!enable_espnow_remote)
    // {
    //   sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("(Enable remote to listen)"));
    // } else 
    // {
    //   sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("None"));
    // }
    // #endif
  }

  if (subPage == SUBPAGE_LEDS)
  {
    char nS[32];

    // appendGPIOinfo();

    // set limits
    // oappend(SET_F("bLimits("));
    // oappend(itoa(WLED_MAX_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(WLED_MIN_VIRTUAL_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(MAX_LEDS_PER_BUS,nS,10)); oappend(",");
    // oappend(itoa(MAX_LED_MEMORY,nS,10));   oappend(",");
    // oappend(itoa(MAX_LEDS,nS,10));
    // oappend(SET_F(");"));

  //   sappend('c',SET_F("MS"),autoSegments);
  //   sappend('c',SET_F("CCT"),correctWB);
  //   sappend('c',SET_F("CR"),cctFromRgb);
  //   sappend('v',SET_F("CB"),strip.cctBlending);
  //   sappend('v',SET_F("FR"),strip.getTargetFps());
  //   sappend('v',SET_F("AW"),Bus::getGlobalAWMode());
  //   sappend('c',SET_F("LD"),strip.useLedsArray);

  //   for (uint8_t s=0; s < busses.getNumBusses(); s++) {
  //     Bus* bus = busses.getBus(s);
  //     if (bus == nullptr) continue;
  //     char lp[4] = "L0"; lp[2] = 48+s; lp[3] = 0; //ascii 0-9 //strip data pin
  //     char lc[4] = "LC"; lc[2] = 48+s; lc[3] = 0; //strip length
  //     char co[4] = "CO"; co[2] = 48+s; co[3] = 0; //strip color order
  //     char lt[4] = "LT"; lt[2] = 48+s; lt[3] = 0; //strip type
  //     char ls[4] = "LS"; ls[2] = 48+s; ls[3] = 0; //strip start LED
  //     char cv[4] = "CV"; cv[2] = 48+s; cv[3] = 0; //strip reverse
  //     char sl[4] = "SL"; sl[2] = 48+s; sl[3] = 0; //skip 1st LED
  //     char rf[4] = "RF"; rf[2] = 48+s; rf[3] = 0; //off refresh
  //     char aw[4] = "AW"; aw[2] = 48+s; aw[3] = 0; //auto white mode
  //     char wo[4] = "WO"; wo[2] = 48+s; wo[3] = 0; //swap channels
  //     char sp[4] = "SP"; sp[2] = 48+s; sp[3] = 0; //bus clock speed
  //     oappend(SET_F("addLEDs(1);"));
  //     uint8_t pins[5];
  //     uint8_t nPins = bus->getPins(pins);
  //     for (uint8_t i = 0; i < nPins; i++) {
  //       lp[1] = 48+i;
  //       if (pinManager.isPinOk(pins[i]) || bus->getType()>=TYPE_NET_DDP_RGB) sappend('v',lp,pins[i]);
  //     }
  //     sappend('v',lc,bus->getLength());
  //     sappend('v',lt,bus->getType());
  //     sappend('v',co,bus->getColorOrder() & 0x0F);
  //     sappend('v',ls,bus->getStart());
  //     sappend('c',cv,bus->reversed);
  //     sappend('v',sl,bus->skippedLeds());
  //     sappend('c',rf,bus->isOffRefreshRequired());
  //     sappend('v',aw,bus->getAutoWhiteMode());
  //     sappend('v',wo,bus->getColorOrder() >> 4);
  //     uint16_t speed = bus->getFrequency();
  //     if (bus->getType() > TYPE_ONOFF && bus->getType() < 48) {
  //       switch (speed) {
  //         case WLED_PWM_FREQ/3 : speed = 0; break;
  //         case WLED_PWM_FREQ/2 : speed = 1; break;
  //         default:
  //         case WLED_PWM_FREQ   : speed = 2; break;
  //         case WLED_PWM_FREQ*2 : speed = 3; break;
  //         case WLED_PWM_FREQ*3 : speed = 4; break;
  //       }
  //     } else {
  //       switch (speed) {
  //         case  1000 : speed = 0; break;
  //         case  2000 : speed = 1; break;
  //         default:
  //         case  5000 : speed = 2; break;
  //         case 10000 : speed = 3; break;
  //         case 20000 : speed = 4; break;
  //       }
  //     }
  //     sappend('v',sp,speed);
  //   }
  //   sappend('v',SET_F("MA"),strip.ablMilliampsMax);
  //   sappend('v',SET_F("LA"),strip.milliampsPerLed);
  //   if (strip.currentMilliamps)
  //   {
  //     sappends('m',SET_F("(\"pow\")[0]"),(char*)"");
  //     olen -= 2; //delete ";
  //     oappendi(strip.currentMilliamps);
  //     oappend(SET_F("mA\";"));
  //   }

  //   oappend(SET_F("resetCOM("));
  //   oappend(itoa(WLED_MAX_COLOR_ORDER_MAPPINGS,nS,10));
  //   oappend(SET_F(");"));
  //   const ColorOrderMap& com = busses.getColorOrderMap();
  //   for (uint8_t s=0; s < com.count(); s++) {
  //     const ColorOrderMapEntry* entry = com.get(s);
  //     if (entry == nullptr) break;
  //     oappend(SET_F("addCOM("));
  //     oappend(itoa(entry->start,nS,10));  oappend(",");
  //     oappend(itoa(entry->len,nS,10));  oappend(",");
  //     oappend(itoa(entry->colorOrder,nS,10));  oappend(");");
  //   }

  //   sappend('v',SET_F("CA"),briS);

  //   sappend('c',SET_F("BO"),turnOnAtBoot);
  //   sappend('v',SET_F("BP"),bootPreset);

  //   sappend('c',SET_F("GB"),gammaCorrectBri);
  //   sappend('c',SET_F("GC"),gammaCorrectCol);
  //   dtostrf(gammaCorrectVal,3,1,nS); sappends('s',SET_F("GV"),nS);
  //   sappend('c',SET_F("TF"),fadeTransition);
  //   sappend('v',SET_F("TD"),transitionDelayDefault);
  //   sappend('c',SET_F("PF"),strip.paletteFade);
  //   sappend('v',SET_F("TP"),randomPaletteChangeTime);
  //   sappend('v',SET_F("BF"),briMultiplier);
  //   sappend('v',SET_F("TB"),nightlightTargetBri);
  //   sappend('v',SET_F("TL"),nightlightDelayMinsDefault);
  //   sappend('v',SET_F("TW"),nightlightMode);
  //   sappend('i',SET_F("PB"),strip.paletteBlend);
  //   sappend('v',SET_F("RL"),rlyPin);
  //   sappend('c',SET_F("RM"),rlyMde);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addBtn("));
  //     oappend(itoa(i,nS,10));  oappend(",");
  //     oappend(itoa(btnPin[i],nS,10)); oappend(",");
  //     oappend(itoa(buttonType[i],nS,10));
  //     oappend(SET_F(");"));
  //   }
  //   sappend('c',SET_F("IP"),disablePullUp);
  //   sappend('v',SET_F("TT"),touchThreshold);
  //   sappend('v',SET_F("IR"),irPin);
  //   sappend('v',SET_F("IT"),irEnabled);
  //   sappend('c',SET_F("MSO"),!irApplyToAllSelected);
  // }

  // if (subPage == SUBPAGE_UI)
  // {
  //   sappends('s',SET_F("DS"),serverDescription);
  //   sappend('c',SET_F("ST"),syncToggleReceive);
  // #ifdef WLED_ENABLE_SIMPLE_UI
  //   sappend('c',SET_F("SU"),simplifiedUI);
  // #else
  //   oappend(SET_F("toggle('Simple');"));    // hide Simplified UI settings
  // #endif
  // }

  // if (subPage == SUBPAGE_SYNC)
  // {
  //   sappend('v',SET_F("UP"),udpPort);
  //   sappend('v',SET_F("U2"),udpPort2);
  //   sappend('v',SET_F("GS"),syncGroups);
  //   sappend('v',SET_F("GR"),receiveGroups);

  //   sappend('c',SET_F("RB"),receiveNotificationBrightness);
  //   sappend('c',SET_F("RC"),receiveNotificationColor);
  //   sappend('c',SET_F("RX"),receiveNotificationEffects);
  //   sappend('c',SET_F("SO"),receiveSegmentOptions);
  //   sappend('c',SET_F("SG"),receiveSegmentBounds);
  //   sappend('c',SET_F("SD"),notifyDirectDefault);
  //   sappend('c',SET_F("SB"),notifyButton);
  //   sappend('c',SET_F("SH"),notifyHue);
  //   sappend('c',SET_F("SM"),notifyMacro);
  //   sappend('v',SET_F("UR"),udpNumRetries);

  //   sappend('c',SET_F("NL"),nodeListEnabled);
  //   sappend('c',SET_F("NB"),nodeBroadcastEnabled);

  //   sappend('c',SET_F("RD"),receiveDirect);
  //   sappend('c',SET_F("MO"),useMainSegmentOnly);
  //   sappend('v',SET_F("EP"),e131Port);
  //   sappend('c',SET_F("ES"),e131SkipOutOfSequence);
  //   sappend('c',SET_F("EM"),e131Multicast);
  //   sappend('v',SET_F("EU"),e131Universe);
  //   sappend('v',SET_F("DA"),DMXAddress);
  //   sappend('v',SET_F("XX"),DMXSegmentSpacing);
  //   sappend('v',SET_F("PY"),e131Priority);
  //   sappend('v',SET_F("DM"),DMXMode);
  //   sappend('v',SET_F("ET"),realtimeTimeoutMs);
  //   sappend('c',SET_F("FB"),arlsForceMaxBri);
  //   sappend('c',SET_F("RG"),arlsDisableGammaCorrection);
  //   sappend('v',SET_F("WO"),arlsOffset);
  //   sappend('c',SET_F("AL"),alexaEnabled);
  //   sappends('s',SET_F("AI"),alexaInvocationName);
  //   sappend('c',SET_F("SA"),notifyAlexa);
  //   sappend('v',SET_F("AP"),alexaNumPresets);
  //   #ifdef WLED_DISABLE_ALEXA
  //   oappend(SET_F("toggle('Alexa');"));  // hide Alexa settings
  //   #endif

  //   #ifdef WLED_ENABLE_MQTT
  //   sappend('c',SET_F("MQ"),mqttEnabled);
  //   sappends('s',SET_F("MS"),mqttServer);
  //   sappend('v',SET_F("MQPORT"),mqttPort);
  //   sappends('s',SET_F("MQUSER"),mqttUser);
  //   byte l = strlen(mqttPass);
  //   char fpass[l+1]; //fill password field with ***
  //   fpass[l] = 0;
  //   memset(fpass,'*',l);
  //   sappends('s',SET_F("MQPASS"),fpass);
  //   sappends('s',SET_F("MQCID"),mqttClientID);
  //   sappends('s',"MD",mqttDeviceTopic);
  //   sappends('s',SET_F("MG"),mqttGroupTopic);
  //   sappend('c',SET_F("BM"),buttonPublishMqtt);
  //   sappend('c',SET_F("RT"),retainMqttMsg);
  //   #else
  //   oappend(SET_F("toggle('MQTT');"));    // hide MQTT settings
  //   #endif

  //   #ifndef WLED_DISABLE_HUESYNC
  //   sappend('v',SET_F("H0"),hueIP[0]);
  //   sappend('v',SET_F("H1"),hueIP[1]);
  //   sappend('v',SET_F("H2"),hueIP[2]);
  //   sappend('v',SET_F("H3"),hueIP[3]);
  //   sappend('v',SET_F("HL"),huePollLightId);
  //   sappend('v',SET_F("HI"),huePollIntervalMs);
  //   sappend('c',SET_F("HP"),huePollingEnabled);
  //   sappend('c',SET_F("HO"),hueApplyOnOff);
  //   sappend('c',SET_F("HB"),hueApplyBri);
  //   sappend('c',SET_F("HC"),hueApplyColor);
  //   char hueErrorString[25];
  //   switch (hueError)
  //   {
  //     case HUE_ERROR_INACTIVE     : strcpy_P(hueErrorString,PSTR("Inactive"));                break;
  //     case HUE_ERROR_ACTIVE       : strcpy_P(hueErrorString,PSTR("Active"));                  break;
  //     case HUE_ERROR_UNAUTHORIZED : strcpy_P(hueErrorString,PSTR("Unauthorized"));            break;
  //     case HUE_ERROR_LIGHTID      : strcpy_P(hueErrorString,PSTR("Invalid light ID"));        break;
  //     case HUE_ERROR_PUSHLINK     : strcpy_P(hueErrorString,PSTR("Link button not pressed")); break;
  //     case HUE_ERROR_JSON_PARSING : strcpy_P(hueErrorString,PSTR("JSON parsing error"));      break;
  //     case HUE_ERROR_TIMEOUT      : strcpy_P(hueErrorString,PSTR("Timeout"));                 break;
  //     default: sprintf_P(hueErrorString,PSTR("Bridge Error %i"),hueError);
  //   }

  //   sappends('m',SET_F("(\"sip\")[0]"),hueErrorString);
  //   #else
  //   oappend(SET_F("toggle('Hue');"));    // hide Hue Sync settings
  //   #endif
  //   sappend('v',SET_F("BD"),serialBaud);
  }

  // if (subPage == SUBPAGE_TIME)
  // {
  //   sappend('c',SET_F("NT"),ntpEnabled);
  //   sappends('s',SET_F("NS"),ntpServerName);
  //   sappend('c',SET_F("CF"),!useAMPM);
  //   sappend('i',SET_F("TZ"),currentTimezone);
  //   sappend('v',SET_F("UO"),utcOffsetSecs);
  //   char tm[32];
  //   dtostrf(longitude,4,2,tm);
  //   sappends('s',SET_F("LN"),tm);
  //   dtostrf(latitude,4,2,tm);
  //   sappends('s',SET_F("LT"),tm);
  //   getTimeString(tm);
  //   sappends('m',SET_F("(\"times\")[0]"),tm);
  //   if ((int)(longitude*10.) || (int)(latitude*10.)) {
  //     sprintf_P(tm, PSTR("Sunrise: %02d:%02d Sunset: %02d:%02d"), hour(sunrise), minute(sunrise), hour(sunset), minute(sunset));
  //     sappends('m',SET_F("(\"times\")[1]"),tm);
  //   }
  //   sappend('c',SET_F("OL"),overlayCurrent);
  //   sappend('v',SET_F("O1"),overlayMin);
  //   sappend('v',SET_F("O2"),overlayMax);
  //   sappend('v',SET_F("OM"),analogClock12pixel);
  //   sappend('c',SET_F("OS"),analogClockSecondsTrail);
  //   sappend('c',SET_F("O5"),analogClock5MinuteMarks);

  //   sappend('c',SET_F("CE"),countdownMode);
  //   sappend('v',SET_F("CY"),countdownYear);
  //   sappend('v',SET_F("CI"),countdownMonth);
  //   sappend('v',SET_F("CD"),countdownDay);
  //   sappend('v',SET_F("CH"),countdownHour);
  //   sappend('v',SET_F("CM"),countdownMin);
  //   sappend('v',SET_F("CS"),countdownSec);

  //   sappend('v',SET_F("A0"),macroAlexaOn);
  //   sappend('v',SET_F("A1"),macroAlexaOff);
  //   sappend('v',SET_F("MC"),macroCountdown);
  //   sappend('v',SET_F("MN"),macroNl);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addRow("));
  //     oappend(itoa(i,tm,10));  oappend(",");
  //     oappend(itoa(macroButton[i],tm,10)); oappend(",");
  //     oappend(itoa(macroLongPress[i],tm,10)); oappend(",");
  //     oappend(itoa(macroDoublePress[i],tm,10));
  //     oappend(SET_F(");"));
  //   }

  //   char k[4];
  //   k[2] = 0; //Time macros
  //   for (int i = 0; i<10; i++)
  //   {
  //     k[1] = 48+i; //ascii 0,1,2,3
  //     if (i<8) { k[0] = 'H'; sappend('v',k,timerHours[i]); }
  //     k[0] = 'N'; sappend('v',k,timerMinutes[i]);
  //     k[0] = 'T'; sappend('v',k,timerMacro[i]);
  //     k[0] = 'W'; sappend('v',k,timerWeekday[i]);
  //     if (i<8) {
  //       k[0] = 'M'; sappend('v',k,(timerMonth[i] >> 4) & 0x0F);
	// 			k[0] = 'P'; sappend('v',k,timerMonth[i] & 0x0F);
  //       k[0] = 'D'; sappend('v',k,timerDay[i]);
	// 			k[0] = 'E'; sappend('v',k,timerDayEnd[i]);
  //     }
  //   }
  // }

  // if (subPage == SUBPAGE_SEC)
  // {
  //   byte l = strlen(settingsPIN);
  //   char fpass[l+1]; //fill PIN field with 0000
  //   fpass[l] = 0;
  //   memset(fpass,'0',l);
  //   sappends('s',SET_F("PIN"),fpass);
  //   sappend('c',SET_F("NO"),otaLock);
  //   sappend('c',SET_F("OW"),wifiLock);
  //   sappend('c',SET_F("AO"),aOtaEnabled);
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F(" (build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  //   oappend(SET_F("sd=\""));
  //   oappend(serverDescription);
  //   oappend(SET_F("\";"));
  // }

  // #ifdef WLED_ENABLE_DMX // include only if DMX is enabled
  // if (subPage == SUBPAGE_DMX)
  // {
  //   sappend('v',SET_F("PU"),e131ProxyUniverse);

  //   sappend('v',SET_F("CN"),DMXChannels);
  //   sappend('v',SET_F("CG"),DMXGap);
  //   sappend('v',SET_F("CS"),DMXStart);
  //   sappend('v',SET_F("SL"),DMXStartLED);

  //   sappend('i',SET_F("CH1"),DMXFixtureMap[0]);
  //   sappend('i',SET_F("CH2"),DMXFixtureMap[1]);
  //   sappend('i',SET_F("CH3"),DMXFixtureMap[2]);
  //   sappend('i',SET_F("CH4"),DMXFixtureMap[3]);
  //   sappend('i',SET_F("CH5"),DMXFixtureMap[4]);
  //   sappend('i',SET_F("CH6"),DMXFixtureMap[5]);
  //   sappend('i',SET_F("CH7"),DMXFixtureMap[6]);
  //   sappend('i',SET_F("CH8"),DMXFixtureMap[7]);
  //   sappend('i',SET_F("CH9"),DMXFixtureMap[8]);
  //   sappend('i',SET_F("CH10"),DMXFixtureMap[9]);
  //   sappend('i',SET_F("CH11"),DMXFixtureMap[10]);
  //   sappend('i',SET_F("CH12"),DMXFixtureMap[11]);
  //   sappend('i',SET_F("CH13"),DMXFixtureMap[12]);
  //   sappend('i',SET_F("CH14"),DMXFixtureMap[13]);
  //   sappend('i',SET_F("CH15"),DMXFixtureMap[14]);
  // }
  // #endif

  // if (subPage == SUBPAGE_UM) //usermods
  // {
  //   appendGPIOinfo();
  //   oappend(SET_F("numM="));
  //   oappendi(usermods.getModCount());
  //   oappend(";");
  //   sappend('v',SET_F("SDA"),i2c_sda);
  //   sappend('v',SET_F("SCL"),i2c_scl);
  //   sappend('v',SET_F("MOSI"),spi_mosi);
  //   sappend('v',SET_F("MISO"),spi_miso);
  //   sappend('v',SET_F("SCLK"),spi_sclk);
  //   oappend(SET_F("addInfo('SDA','"));  oappendi(HW_PIN_SDA);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCL','"));  oappendi(HW_PIN_SCL);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MOSI','")); oappendi(HW_PIN_DATASPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MISO','")); oappendi(HW_PIN_MISOSPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCLK','")); oappendi(HW_PIN_CLOCKSPI); oappend(SET_F("');"));
  //   usermods.appendConfigData();
  // }

  // if (subPage == SUBPAGE_UPDATE) // update
  // {
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F("<br>("));
  //   #if defined(ARDUINO_ARCH_ESP32)
  //   oappend(ESP.getChipModel());
  //   #else
  //   oappend("esp8266");
  //   #endif
  //   oappend(SET_F(" build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  // }

  // if (subPage == SUBPAGE_2D) // 2D matrices
  // {
  //   sappend('v',SET_F("SOMP"),strip.isMatrix);
  //   #ifndef WLED_DISABLE_2D
  //   oappend(SET_F("maxPanels=")); oappendi(WLED_MAX_PANELS); oappend(SET_F(";"));
  //   oappend(SET_F("resetPanels();"));
  //   if (strip.isMatrix) {
  //     if(strip.panels>0){
  //       sappend('v',SET_F("PW"),strip.panel[0].width); //Set generator Width and Height to first panel size for convenience
  //       sappend('v',SET_F("PH"),strip.panel[0].height);
  //     }
  //     sappend('v',SET_F("MPC"),strip.panels);
  //     // panels
  //     for (uint8_t i=0; i<strip.panels; i++) {
  //       char n[5];
  //       oappend(SET_F("addPanel("));
  //       oappend(itoa(i,n,10));
  //       oappend(SET_F(");"));
  //       char pO[8] = { '\0' };
  //       snprintf_P(pO, 7, PSTR("P%d"), i);       // MAX_PANELS is 64 so pO will always only be 4 characters or less
  //       pO[7] = '\0';
  //       uint8_t l = strlen(pO);
  //       // create P0B, P1B, ..., P63B, etc for other PxxX
  //       pO[l] = 'B'; sappend('v',pO,strip.panel[i].bottomStart);
  //       pO[l] = 'R'; sappend('v',pO,strip.panel[i].rightStart);
  //       pO[l] = 'V'; sappend('v',pO,strip.panel[i].vertical);
  //       pO[l] = 'S'; sappend('c',pO,strip.panel[i].serpentine);
  //       pO[l] = 'X'; sappend('v',pO,strip.panel[i].xOffset);
  //       pO[l] = 'Y'; sappend('v',pO,strip.panel[i].yOffset);
  //       pO[l] = 'W'; sappend('v',pO,strip.panel[i].width);
  //       pO[l] = 'H'; sappend('v',pO,strip.panel[i].height);
  //     }
  //   }
  //   #else
  //   oappend(SET_F("gId(\"somp\").remove(1);")); // remove 2D option from dropdown
  //   #endif
  // }
}

#define JSON_PATH_STATE      1
#define JSON_PATH_INFO       2
#define JSON_PATH_STATE_INFO 3
#define JSON_PATH_NODES      4
#define JSON_PATH_PALETTES   5
#define JSON_PATH_FXDATA     6
#define JSON_PATH_NETWORKS   7
#define JSON_PATH_EFFECTS    8


const char JSON_mode_names[] PROGMEM = R"=====(["FX names moved"])=====";
const char JSON_palette_names[] PROGMEM = R"=====([
"Default","* Random Cycle","* Color 1","* Colors 1&2","* Color Gradient","* Colors Only","Party","Cloud","Lava","Ocean",
"Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
"Pastel","Sunset 2","Beach","Vintage","Departure","Landscape","Beech","Sherbet","Hult","Hult 64",
"Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
"Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
"Aurora","Atlantica","C9 2","C9 New","Temperature","Aurora 2","Retro Clown","Candy","Toxy Reaf","Fairy Reaf",
"Semi Blue","Pink Candy","Red Reaf","Aqua Flash","Yelblu Hot","Lite Light","Red Flash","Blink Red","Red Shift","Red Tide",
"Candy2"
])=====";

//Un-comment any file types you need
static String getContentType(AsyncWebServerRequest* request, String filename){
  if(request->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".json")) return "application/json";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
//  else if(filename.endsWith(".xml")) return "text/xml";
//  else if(filename.endsWith(".pdf")) return "application/x-pdf";
//  else if(filename.endsWith(".zip")) return "application/x-zip";
//  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(AsyncWebServerRequest* request, String path){
  DEBUG_PRINTLN("WS FileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  if(path.indexOf("sec") > -1) return false;
  String contentType = getContentType(request, path);
  /*String pathWithGz = path + ".gz";
  if(WLED_FS.exists(pathWithGz)){
    request->send(WLED_FS, pathWithGz, contentType);
    return true;
  }*/
  // if(WLED_FS.exists(path)) {
  //   request->send(WLED_FS, path, contentType);
  //   return true;
  // }
  return false;
}


void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (!correctPIN) {
    if (final) request->send(500, "text/plain", FPSTR(s_unlock_cfg));
    return;
  }
  if (!index) {
    String finalname = filename;
    if (finalname.charAt(0) != '/') {
      finalname = '/' + finalname; // prepend slash if missing
    }

    request->_tempFile = WLED_FS.open(finalname, "w");
    DEBUG_PRINT(F("Uploading "));
    DEBUG_PRINTLN(finalname);
    if (finalname.equals("/presets.json"))
    { 
      // presetsModifiedTime = toki.second();
    }
  }
  if (len) {
    request->_tempFile.write(data,len);
  }
  if (final) {
    request->_tempFile.close();
    if (filename.indexOf(F("cfg.json")) >= 0) { // check for filename with or without slash
      doReboot = true;
      request->send(200, "text/plain", F("Configuration restore successful.\nRebooting..."));
    } else {
      if (filename.indexOf(F("palette")) >= 0 && filename.indexOf(F(".json")) >= 0)
      {
        // strip.loadCustomPalettes();
      }
      request->send(200, "text/plain", F("File Uploaded!"));
    }
    cacheInvalidate++;
  }
}


// Size of buffer for API JSON object (increase for more segments)
#ifdef ESP8266
  #define JSON_BUFFER_SIZE 10240
#else
  #define JSON_BUFFER_SIZE 24576
#endif

#define ARDUINOJSON_DECODE_UNICODE 0
#include "AsyncJson-v6.h"
#include "ArduinoJson-v6.h"

// ESP32-WROVER features SPI RAM (aka PSRAM) which can be allocated using ps_malloc()
// we can create custom PSRAMDynamicJsonDocument to use such feature (replacing DynamicJsonDocument)
// The following is a construct to enable code to compile without it.
// There is a code thet will still not use PSRAM though:
//    AsyncJsonResponse is a derived class that implements DynamicJsonDocument (AsyncJson-v6.h)
#if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (psramFound()) return ps_malloc(size); // use PSRAM if it exists
    else              return malloc(size);    // fallback
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (psramFound()) return ps_realloc(ptr, new_size); // use PSRAM if it exists
    else              return realloc(ptr, new_size);    // fallback
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif

// global ArduinoJson buffer
StaticJsonDocument<JSON_BUFFER_SIZE> doc;
volatile uint8_t jsonBufferLock = 0;

// void serializeSegment(JsonObject& root, Segment& seg, byte id, bool forPreset = false, bool segmentBounds = true);
void serializeState(JsonObject root, bool forPreset = false, bool includeBri = true, bool segmentBounds = true, bool selectedSegmentsOnly = false);
void serializeInfo(JsonObject root);
void serializeModeNames(JsonArray root);
void serializeModeData(JsonArray root);

//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
bool requestJSONBufferLock(uint8_t module)
{
  unsigned long now = millis();

  while (jsonBufferLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

  if (millis()-now >= 1000) {
    DEBUG_PRINT(F("ERROR: Locking JSON buffer failed! ("));
    DEBUG_PRINT(jsonBufferLock);
    DEBUG_PRINTLN(")");
    return false; // waiting time-outed
  }

  jsonBufferLock = module ? module : 255;
  DEBUG_PRINT(F("JSON buffer locked. ("));
  DEBUG_PRINT(jsonBufferLock);
  DEBUG_PRINTLN(")");
  // fileDoc = &doc;  // used for applying presets (presets.cpp)
  // doc.clear();
  return true;
}

bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request)
{
  AsyncWebHeader* header = request->getHeader("If-None-Match");
  if (header && header->value() == String(PROJECT_VERSION)) {
    request->send(304);
    return true;
  }
  return false;
}


// void serializeSegment(JsonObject& root, Segment& seg, byte id, bool forPreset, bool segmentBounds)
// {
//   root["id"] = id;
//   if (segmentBounds) {
//     root["start"] = seg.start;
//     root["stop"] = seg.stop;
//     if (strip.isMatrix) {
//       root[F("startY")] = seg.startY;
//       root[F("stopY")]  = seg.stopY;
//     }
//   }
//   if (!forPreset) root["len"] = seg.stop - seg.start;
//   root["grp"]    = seg.grouping;
//   root[F("spc")] = seg.spacing;
//   root[F("of")]  = seg.offset;
//   root["on"]     = seg.on;
//   root["frz"]    = seg.freeze;
//   byte segbri    = seg.opacity;
//   root["bri"]    = (segbri) ? segbri : 255;
//   root["cct"]    = seg.cct;
//   root[F("set")] = seg.set;

//   if (segmentBounds && seg.name != nullptr) root["n"] = reinterpret_cast<const char *>(seg.name); //not good practice, but decreases required JSON buffer

//   // to conserve RAM we will serialize the col array manually
//   // this will reduce RAM footprint from ~300 bytes to 84 bytes per segment
//   char colstr[70]; colstr[0] = '['; colstr[1] = '\0';  //max len 68 (5 chan, all 255)
//   const char *format = strip.hasWhiteChannel() ? PSTR("[%u,%u,%u,%u]") : PSTR("[%u,%u,%u]");
//   for (size_t i = 0; i < 3; i++)
//   {
//     byte segcol[4]; byte* c = segcol;
//     segcol[0] = R(seg.colors[i]);
//     segcol[1] = G(seg.colors[i]);
//     segcol[2] = B(seg.colors[i]);
//     segcol[3] = W(seg.colors[i]);
//     char tmpcol[22];
//     sprintf_P(tmpcol, format, (unsigned)c[0], (unsigned)c[1], (unsigned)c[2], (unsigned)c[3]);
//     strcat(colstr, i<2 ? strcat(tmpcol, ",") : tmpcol);
//   }
//   strcat(colstr, "]");
//   root["col"] = serialized(colstr);

//   root["fx"]  = seg.mode;
//   root["sx"]  = seg.speed;
//   root["ix"]  = seg.intensity;
//   root["pal"] = seg.palette;
//   root["c1"]  = seg.custom1;
//   root["c2"]  = seg.custom2;
//   root["c3"]  = seg.custom3;
//   root["sel"] = seg.isSelected();
//   root["rev"] = seg.reverse;
//   root["mi"]  = seg.mirror;
//   #ifndef WLED_DISABLE_2D
//   if (strip.isMatrix) {
//     root["rY"] = seg.reverse_y;
//     root["mY"] = seg.mirror_y;
//     root[F("tp")] = seg.transpose;
//   }
//   #endif
//   root["o1"]  = seg.check1;
//   root["o2"]  = seg.check2;
//   root["o3"]  = seg.check3;
//   root["si"]  = seg.soundSim;
//   root["m12"] = seg.map1D2D;
// }

void serializeState(JsonObject root, bool forPreset, bool includeBri, bool segmentBounds, bool selectedSegmentsOnly)
{
//   if (includeBri) {
//     root["on"] = (bri > 0);
//     root["bri"] = briLast;
//     root[F("transition")] = transitionDelay/100; //in 100ms
//   }

//   if (!forPreset) {
//     if (errorFlag) {root[F("error")] = errorFlag; errorFlag = ERR_NONE;} //prevent error message to persist on screen

//     root["ps"] = (currentPreset > 0) ? currentPreset : -1;
    root[F("pl")] = "currentPlaylist";

//     usermods.addToJsonState(root);

//     JsonObject nl = root.createNestedObject("nl");
//     nl["on"] = nightlightActive;
//     nl["dur"] = nightlightDelayMins;
//     nl["mode"] = nightlightMode;
//     nl[F("tbri")] = nightlightTargetBri;
//     if (nightlightActive) {
//       nl[F("rem")] = (nightlightDelayMs - (millis() - nightlightStartTime)) / 1000; // seconds remaining
//     } else {
//       nl[F("rem")] = -1;
//     }

//     JsonObject udpn = root.createNestedObject("udpn");
//     udpn["send"] = notifyDirect;
//     udpn["recv"] = receiveNotifications;

//     root[F("lor")] = realtimeOverride;
//   }

//   root[F("mainseg")] = strip.getMainSegmentId();

//   JsonArray seg = root.createNestedArray("seg");
//   for (size_t s = 0; s < strip.getMaxSegments(); s++) {
//     if (s >= strip.getSegmentsNum()) {
//       if (forPreset && segmentBounds && !selectedSegmentsOnly) { //disable segments not part of preset
//         JsonObject seg0 = seg.createNestedObject();
//         seg0["stop"] = 0;
//         continue;
//       } else
//         break;
//     }
//     Segment &sg = strip.getSegment(s);
//     if (forPreset && selectedSegmentsOnly && !sg.isSelected()) continue;
//     if (sg.isActive()) {
//       JsonObject seg0 = seg.createNestedObject();
//       serializeSegment(seg0, sg, s, forPreset, segmentBounds);
//     } else if (forPreset && segmentBounds) { //disable segments not part of preset
//       JsonObject seg0 = seg.createNestedObject();
//       seg0["stop"] = 0;
//     }
//   }
}

void serializeInfo(JsonObject root)
{
  root[F("ver")] = "versionString";
  root[F("vid")] = PROJECT_VERSION;
  //root[F("cn")] = WLED_CODENAME;

  JsonObject leds = root.createNestedObject("leds");
  leds[F("count")] = 123;//strip.getLengthTotal();
  leds[F("pwr")] = 123;//strip.currentMilliamps;
  // leds["fps"] = strip.getFps();
  // leds[F("maxpwr")] = (strip.currentMilliamps)? strip.ablMilliampsMax : 0;
  // leds[F("maxseg")] = strip.getMaxSegments();
  //leds[F("actseg")] = strip.getActiveSegmentsNum();
  //leds[F("seglock")] = false; //might be used in the future to prevent modifications to segment config

  #ifndef WLED_DISABLE_2D
  // if (strip.isMatrix) {
  //   JsonObject matrix = leds.createNestedObject("matrix");
  //   matrix["w"] = 1;//Segment::maxWidth;
  //   matrix["h"] = 2;//Segment::maxHeight;
  // }
  #endif

  uint8_t totalLC = 0;
  JsonArray lcarr = leds.createNestedArray(F("seglc"));
  // size_t nSegs = strip.getSegmentsNum();
  // for (size_t s = 0; s < nSegs; s++) {
  //   if (!strip.getSegment(s).isActive()) continue;
  //   uint8_t lc = strip.getSegment(s).getLightCapabilities();
  //   totalLC |= lc;
  //   lcarr.add(lc);
  // }

  leds["lc"] = totalLC;

  // leds[F("rgbw")] = strip.hasRGBWBus(); // deprecated, use info.leds.lc
  leds[F("wv")]   = totalLC & 0x02;     // deprecated, true if white slider should be displayed for any segment
  leds["cct"]     = totalLC & 0x04;     // deprecated, use info.leds.lc

  #ifdef WLED_DEBUG
  JsonArray i2c = root.createNestedArray(F("i2c"));
  i2c.add(i2c_sda);
  i2c.add(i2c_scl);
  JsonArray spi = root.createNestedArray(F("spi"));
  spi.add(spi_mosi);
  spi.add(spi_sclk);
  spi.add(spi_miso);
  #endif

  root[F("str")] = syncToggleReceive;

  root[F("name")] = serverDescription;
  root[F("udpport")] = 123;//udpPort;
  root["live"] = (bool)realtimeMode;
  root[F("liveseg")] = -1;//useMainSegmentOnly ? strip.getMainSegmentId() : -1;  // if using main segment only for live

  switch (realtimeMode) {
    case REALTIME_MODE_INACTIVE: root["lm"] = ""; break;
    case REALTIME_MODE_GENERIC:  root["lm"] = ""; break;
    case REALTIME_MODE_UDP:      root["lm"] = F("UDP"); break;
    case REALTIME_MODE_HYPERION: root["lm"] = F("Hyperion"); break;
    case REALTIME_MODE_E131:     root["lm"] = F("E1.31"); break;
    case REALTIME_MODE_ADALIGHT: root["lm"] = F("USB Adalight/TPM2"); break;
    case REALTIME_MODE_ARTNET:   root["lm"] = F("Art-Net"); break;
    case REALTIME_MODE_TPM2NET:  root["lm"] = F("tpm2.net"); break;
    case REALTIME_MODE_DDP:      root["lm"] = F("DDP"); break;
  }

  // if (realtimeIP[0] == 0)
  // {
  //   root[F("lip")] = "";
  // } else {
  //   root[F("lip")] = realtimeIP.toString();
  // }

  #ifdef WLED_ENABLE_WEBSOCKETS
  root[F("ws")] = ws.count();
  #else
  root[F("ws")] = -1;
  #endif

  // root[F("fxcount")] = strip.getModeCount();
  // root[F("palcount")] = strip.getPaletteCount();
  // root[F("cpalcount")] = strip.customPalettes.size(); //number of custom palettes

  JsonArray ledmaps = root.createNestedArray(F("maps"));
  // for (size_t i=0; i<WLED_MAX_LEDMAPS; i++) {
  //   if ((ledMaps>>i) & 0x00000001U) {
  //     JsonObject ledmaps0 = ledmaps.createNestedObject();
  //     ledmaps0["id"] = i;
  //     #ifndef ESP8266
  //     if (i && ledmapNames[i-1]) ledmaps0["n"] = ledmapNames[i-1];
  //     #endif
  //   }
  // }

  JsonObject wifi_info = root.createNestedObject("wifi");
  wifi_info[F("bssid")] = WiFi.BSSIDstr();
  int qrssi = WiFi.RSSI();
  wifi_info[F("rssi")] = qrssi;
  wifi_info[F("signal")] = 126;//getSignalQuality(qrssi);
  wifi_info[F("channel")] = WiFi.channel();

  JsonObject fs_info = root.createNestedObject("fs");
  fs_info["u"] = 123;//fsBytesUsed / 1000;
  fs_info["t"] = 123;//fsBytesTotal / 1000;
  fs_info[F("pmt")] = 123;//presetsModifiedTime;

  root[F("ndc")] =123;// nodeListEnabled ? (int)Nodes.size() : -1;

  #ifdef ARDUINO_ARCH_ESP32
  #ifdef WLED_DEBUG
    wifi_info[F("txPower")] = (int) WiFi.getTxPower();
    wifi_info[F("sleep")] = (bool) WiFi.getSleep();
  #endif
  #if !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3)
    root[F("arch")] = "esp32";
  #else
    root[F("arch")] = ESP.getChipModel();
  #endif
  root[F("core")] = ESP.getSdkVersion();
  //root[F("maxalloc")] = ESP.getMaxAllocHeap();
  #ifdef WLED_DEBUG
    root[F("resetReason0")] = (int)rtc_get_reset_reason(0);
    root[F("resetReason1")] = (int)rtc_get_reset_reason(1);
  #endif
  root[F("lwip")] = 0; //deprecated
  #else
  root[F("arch")] = "esp8266";
  root[F("core")] = ESP.getCoreVersion();
  //root[F("maxalloc")] = ESP.getMaxFreeBlockSize();
  #ifdef WLED_DEBUG
    root[F("resetReason")] = (int)ESP.getResetInfoPtr()->reason;
  #endif
  root[F("lwip")] = LWIP_VERSION_MAJOR;
  #endif

  root[F("freeheap")] = ESP.getFreeHeap();
  #if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM)
  if (psramFound()) root[F("psram")] = ESP.getFreePsram();
  #endif
  // root[F("uptime")] = millis()/1000 + rolloverMillis*4294967;

  // usermods.addToJsonInfo(root);

  uint16_t os = 0;
  #ifdef WLED_DEBUG
  os  = 0x80;
    #ifdef WLED_DEBUG_HOST
    os |= 0x0100;
    if (!netDebugEnabled) os &= ~0x0080;
    #endif
  #endif
  #ifndef WLED_DISABLE_ALEXA
  os += 0x40;
  #endif

  //os += 0x20; // indicated now removed Blynk support, may be reused to indicate another build-time option

  #ifdef USERMOD_CRONIXIE
  os += 0x10;
  #endif
  #ifndef WLED_DISABLE_FILESYSTEM
  os += 0x08;
  #endif
  #ifndef WLED_DISABLE_HUESYNC
  os += 0x04;
  #endif
  #ifdef WLED_ENABLE_ADALIGHT
  os += 0x02;
  #endif
  #ifndef WLED_DISABLE_OTA
  os += 0x01;
  #endif
  root[F("opt")] = os;

  root[F("brand")] = "WLED";
  root[F("product")] = F("FOSS");
  // root["mac"] = escapedMac;
  char s[16] = "";
  // if (Network.isConnected())
  // {
  //   IPAddress localIP = Network.localIP();
  //   sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  // }
  root["ip"] = s;
}

// void setPaletteColors(JsonArray json, CRGBPalette16 palette)
// {
//     for (int i = 0; i < 16; i++) {
//       JsonArray colors =  json.createNestedArray();
//       CRGB color = palette[i];
//       colors.add(i<<4);
//       colors.add(color.red);
//       colors.add(color.green);
//       colors.add(color.blue);
//     }
// }

// void setPaletteColors(JsonArray json, byte* tcp)
// {
//     TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
//     TRGBGradientPaletteEntryUnion u;

//     // Count entries
//     uint16_t count = 0;
//     do {
//         u = *(ent + count);
//         count++;
//     } while ( u.index != 255);

//     u = *ent;
//     int indexstart = 0;
//     while( indexstart < 255) {
//       indexstart = u.index;

//       JsonArray colors =  json.createNestedArray();
//       colors.add(u.index);
//       colors.add(u.r);
//       colors.add(u.g);
//       colors.add(u.b);

//       ent++;
//       u = *ent;
//     }
// }

void serializePalettes(JsonObject root, int page)
{
  byte tcp[72];
  #ifdef ESP8266
  int itemPerPage = 5;
  #else
  int itemPerPage = 8;
  #endif

  // int palettesCount = 12;//strip.getPaletteCount();
  // int customPalettes = 12;//strip.customPalettes.size();

  // int maxPage = (palettesCount + customPalettes -1) / itemPerPage;
  // if (page > maxPage) page = maxPage;

  // int start = itemPerPage * page;
  // int end = start + itemPerPage;
  // if (end > palettesCount + customPalettes) end = palettesCount + customPalettes;

  // root[F("m")] = maxPage; // inform caller how many pages there are
  JsonObject palettes  = root.createNestedObject("p");

  // for (int i = start; i < end; i++) {
  //   JsonArray curPalette = palettes.createNestedArray(String(i>=palettesCount ? 255 - i + palettesCount : i));
  //   switch (i) {
  //     case 0: //default palette
  //       setPaletteColors(curPalette, PartyColors_p);
  //       break;
  //     case 1: //random
  //         curPalette.add("r");
  //         curPalette.add("r");
  //         curPalette.add("r");
  //         curPalette.add("r");
  //       break;
  //     case 2: //primary color only
  //       curPalette.add("c1");
  //       break;
  //     case 3: //primary + secondary
  //       curPalette.add("c1");
  //       curPalette.add("c1");
  //       curPalette.add("c2");
  //       curPalette.add("c2");
  //       break;
  //     case 4: //primary + secondary + tertiary
  //       curPalette.add("c3");
  //       curPalette.add("c2");
  //       curPalette.add("c1");
  //       break;
  //     case 5: //primary + secondary (+tert if not off), more distinct
  //       curPalette.add("c1");
  //       curPalette.add("c1");
  //       curPalette.add("c1");
  //       curPalette.add("c1");
  //       curPalette.add("c1");
  //       curPalette.add("c2");
  //       curPalette.add("c2");
  //       curPalette.add("c2");
  //       curPalette.add("c2");
  //       curPalette.add("c2");
  //       curPalette.add("c3");
  //       curPalette.add("c3");
  //       curPalette.add("c3");
  //       curPalette.add("c3");
  //       curPalette.add("c3");
  //       curPalette.add("c1");
  //       break;
  //     case 6: //Party colors
  //       setPaletteColors(curPalette, PartyColors_p);
  //       break;
  //     case 7: //Cloud colors
  //       setPaletteColors(curPalette, CloudColors_p);
  //       break;
  //     case 8: //Lava colors
  //       setPaletteColors(curPalette, LavaColors_p);
  //       break;
  //     case 9: //Ocean colors
  //       setPaletteColors(curPalette, OceanColors_p);
  //       break;
  //     case 10: //Forest colors
  //       setPaletteColors(curPalette, ForestColors_p);
  //       break;
  //     case 11: //Rainbow colors
  //       setPaletteColors(curPalette, RainbowColors_p);
  //       break;
  //     case 12: //Rainbow stripe colors
  //       setPaletteColors(curPalette, RainbowStripeColors_p);
  //       break;
  //     default:
  //       {
  //       if (i>=palettesCount) {
  //         setPaletteColors(curPalette, strip.customPalettes[i - palettesCount]);
  //       } else {
  //         memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[i - 13])), 72);
  //         setPaletteColors(curPalette, tcp);
  //       }
  //       }
  //       break;
  //   }
  // }
}

void serializeNetworks(JsonObject root)
{
  JsonArray networks = root.createNestedArray(F("networks"));
  int16_t status = WiFi.scanComplete();

  switch (status) {
    case WIFI_SCAN_FAILED:
      WiFi.scanNetworks(true);
      return;
    case WIFI_SCAN_RUNNING:
      return;
  }

  for (int i = 0; i < status; i++) {
    JsonObject node = networks.createNestedObject();
    node["ssid"]    = WiFi.SSID(i);
    node["rssi"]    = WiFi.RSSI(i);
    node["bssid"]   = WiFi.BSSIDstr(i);
    node["channel"] = WiFi.channel(i);
    node["enc"]     = WiFi.encryptionType(i);
  }

  WiFi.scanDelete();

  if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
    WiFi.scanNetworks(true);
  }
}

void serializeNodes(JsonObject root)
{
  JsonArray nodes = root.createNestedArray("nodes");

  // for (NodesMap::iterator it = Nodes.begin(); it != Nodes.end(); ++it)
  // {
  //   if (it->second.ip[0] != 0)
  //   {
  //     JsonObject node = nodes.createNestedObject();
  //     node[F("name")] = it->second.nodeName;
  //     node["type"]    = it->second.nodeType;
  //     node["ip"]      = it->second.ip.toString();
  //     node[F("age")]  = it->second.age;
  //     node[F("vid")]  = it->second.build;
  //   }
  // }
}

// deserializes mode data string into JsonArray
void serializeModeData(JsonArray fxdata)
{
  char lineBuffer[128];
  // for (size_t i = 0; i < strip.getModeCount(); i++) {
  //   strncpy_P(lineBuffer, strip.getModeData(i), 127);
  //   if (lineBuffer[0] != 0) {
  //     char* dataPtr = strchr(lineBuffer,'@');
  //     if (dataPtr) fxdata.add(dataPtr+1);
  //     else         fxdata.add("");
  //   }
  // }
}

// deserializes mode names string into JsonArray
// also removes effect data extensions (@...) from deserialised names
void serializeModeNames(JsonArray arr) {
  char lineBuffer[128];
  // for (size_t i = 0; i < strip.getModeCount(); i++) {
  //   strncpy_P(lineBuffer, strip.getModeData(i), 127);
  //   if (lineBuffer[0] != 0) {
  //     char* dataPtr = strchr(lineBuffer,'@');
  //     if (dataPtr) *dataPtr = 0; // terminate mode data after name
  //     arr.add(lineBuffer);
  //   }
  // }
}

void serveJson(AsyncWebServerRequest* request)
{
  byte subJson = 0;
  const String& url = request->url();
  if      (url.indexOf("state") > 0) subJson = JSON_PATH_STATE;
  else if (url.indexOf("info")  > 0) subJson = JSON_PATH_INFO;
  else if (url.indexOf("si")    > 0) subJson = JSON_PATH_STATE_INFO;
  else if (url.indexOf("nodes") > 0) subJson = JSON_PATH_NODES;
  else if (url.indexOf("eff")   > 0) subJson = JSON_PATH_EFFECTS;
  else if (url.indexOf("palx")  > 0) subJson = JSON_PATH_PALETTES;
  else if (url.indexOf("fxda")  > 0) subJson = JSON_PATH_FXDATA;
  else if (url.indexOf("net")   > 0) subJson = JSON_PATH_NETWORKS;
  #ifdef WLED_ENABLE_JSONLIVE
  else if (url.indexOf("live")  > 0) {
    serveLiveLeds(request);
    return;
  }
  #endif
  else if (url.indexOf("pal") > 0) {
    request->send_P(200, "application/json", JSON_palette_names);
    return;
  }
  else if (url.indexOf("cfg") > 0 && handleFileRead(request, "/cfg.json")) {
    return;
  }
  else if (url.length() > 6) { //not just /json
    request->send(501, "application/json", F("{\"error\":\"Not implemented\"}"));
    return;
  }

  if (!requestJSONBufferLock(17)) {
    request->send(503, "application/json", F("{\"error\":3}"));
    return;
  }
  AsyncJsonResponse *response = new AsyncJsonResponse(&doc, subJson==JSON_PATH_FXDATA || subJson==JSON_PATH_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary

  JsonVariant lDoc = response->getRoot();

  switch (subJson)
  {
    // case JSON_PATH_STATE:
    //   serializeState(lDoc); break;
    // case JSON_PATH_INFO:
    //   serializeInfo(lDoc); break;
    // case JSON_PATH_NODES:
    //   serializeNodes(lDoc); break;
    // case JSON_PATH_PALETTES:
    //   serializePalettes(lDoc, request->hasParam("page") ? request->getParam("page")->value().toInt() : 0); break;
    // case JSON_PATH_EFFECTS:
    //   serializeModeNames(lDoc); break;
    case JSON_PATH_FXDATA:
      serializeModeData(lDoc); break;
    case JSON_PATH_NETWORKS:
      serializeNetworks(lDoc); break;
    default: //all
      JsonObject state = lDoc.createNestedObject("state");
      serializeState(state);
      // JsonObject info = lDoc.createNestedObject("info");
      // serializeInfo(info);
      // if (subJson != JSON_PATH_STATE_INFO)
      // {
      //   JsonArray effects = lDoc.createNestedArray(F("effects"));
      //   serializeModeNames(effects); // remove WLED-SR extensions from effect names
      //   lDoc[F("palettes")] = serialized((const __FlashStringHelper*)JSON_palette_names);
      // }
      //lDoc["m"] = lDoc.memoryUsage(); // JSON buffer usage, for remote debugging
  }

  DEBUG_PRINTF("JSON buffer size: %u for request: %d\n", lDoc.memoryUsage(), subJson);

  size_t len = response->setLength();
  DEBUG_PRINT(F("JSON content length: ")); DEBUG_PRINTLN(len);

  request->send(response);
  // releaseJSONBufferLock();
}


void serveSettingsJS(AsyncWebServerRequest* request)
{
  char buf[SETTINGS_STACK_BUF_SIZE+37];
  buf[0] = 0;
  byte subPage = request->arg(F("p")).toInt();
  if (subPage > 10) {
    strcpy_P(buf, PSTR("alert('Settings for this request are not implemented.');"));
    request->send(501, "application/javascript", buf);
    return;
  }
  if (subPage > 0 && !correctPIN && strlen(settingsPIN)>0) {
    strcpy_P(buf, PSTR("alert('PIN incorrect.');"));
    request->send(403, "application/javascript", buf);
    return;
  }
  strcat_P(buf,PSTR("function GetV(){var d=document;"));
  getSettingsJS(subPage, buf+strlen(buf));  // this may overflow by 35bytes!!!
  strcat_P(buf,PSTR("}"));
  request->send(200, "application/javascript", buf);
}


void serveSettings(AsyncWebServerRequest* request, bool post = false);
void serveSettings(AsyncWebServerRequest* request, bool post)
{
  byte subPage = 0, originalSubPage = 0;
  const String& url = request->url();

  if (url.indexOf("sett") >= 0)
  {
    if      (url.indexOf(".js")  > 0) subPage = SUBPAGE_JS;
    else if (url.indexOf(".css") > 0) subPage = SUBPAGE_CSS;
    else if (url.indexOf("wifi") > 0) subPage = SUBPAGE_WIFI;
    else if (url.indexOf("leds") > 0) subPage = SUBPAGE_LEDS;
    else if (url.indexOf("ui")   > 0) subPage = SUBPAGE_UI;
    else if (url.indexOf("sync") > 0) subPage = SUBPAGE_SYNC;
    else if (url.indexOf("time") > 0) subPage = SUBPAGE_TIME;
    else if (url.indexOf("sec")  > 0) subPage = SUBPAGE_SEC;
    else if (url.indexOf("dmx")  > 0) subPage = SUBPAGE_DMX;
    else if (url.indexOf("um")   > 0) subPage = SUBPAGE_UM;
    else if (url.indexOf("2D")   > 0) subPage = SUBPAGE_2D;
    else if (url.indexOf("lock") > 0) subPage = SUBPAGE_LOCK;
  }
  else if (url.indexOf("/update") >= 0) subPage = SUBPAGE_UPDATE; // update page, for PIN check
  //else if (url.indexOf("/edit")   >= 0) subPage = 10;
  else subPage = SUBPAGE_WELCOME;

  if (!correctPIN && strlen(settingsPIN) > 0 && (subPage > 0 && subPage < 11)) {
    originalSubPage = subPage;
    subPage = SUBPAGE_PINREQ; // require PIN
  }

  // // if OTA locked or too frequent PIN entry requests fail hard
  // if ((subPage == SUBPAGE_WIFI && wifiLock && otaLock) || (post && !correctPIN && millis()-lastEditTime < PIN_RETRY_COOLDOWN))
  // {
  //   serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_ota), 254); return;
  // }

  if (post) { //settings/set POST request, saving
    // if (subPage != SUBPAGE_WIFI || !(wifiLock && otaLock)) handleSettingsSet(request, subPage);

    char s[32];
    char s2[45] = "";

    switch (subPage) {
      case SUBPAGE_WIFI   : strcpy_P(s, PSTR("WiFi")); strcpy_P(s2, PSTR("Please connect to the new IP (if changed)")); break;//forceReconnect = true; break;
      case SUBPAGE_LEDS   : strcpy_P(s, PSTR("LED")); break;
      case SUBPAGE_UI     : strcpy_P(s, PSTR("UI")); break;
      case SUBPAGE_SYNC   : strcpy_P(s, PSTR("Sync")); break;
      case SUBPAGE_TIME   : strcpy_P(s, PSTR("Time")); break;
      case SUBPAGE_SEC    : strcpy_P(s, PSTR("Security")); break;//if (doReboot) strcpy_P(s2, PSTR("Rebooting, please wait ~10 seconds...")); break;
      case SUBPAGE_DMX    : strcpy_P(s, PSTR("DMX")); break;
      case SUBPAGE_UM     : strcpy_P(s, PSTR("Usermods")); break;
      case SUBPAGE_2D     : strcpy_P(s, PSTR("2D")); break;
      case SUBPAGE_PINREQ : strcpy_P(s, correctPIN ? PSTR("PIN accepted") : PSTR("PIN rejected")); break;
    }

    if (subPage != SUBPAGE_PINREQ) strcat_P(s, PSTR(" settings saved."));

    if (subPage == SUBPAGE_PINREQ && correctPIN) {
      subPage = originalSubPage; // on correct PIN load settings page the user intended
    } else {
      if (!s2[0]) strcpy_P(s2, s_redirecting);

      // bool redirectAfter9s = (subPage == SUBPAGE_WIFI || ((subPage == SUBPAGE_SEC || subPage == SUBPAGE_UM) && doReboot));
      // serveMessage(request, 200, s, s2, redirectAfter9s ? 129 : (correctPIN ? 1 : 3));
      return;
    }
  }

  AsyncWebServerResponse *response;
  switch (subPage)
  {
    case SUBPAGE_WIFI    : response = request->beginResponse_P(200, "text/html", PAGE_settings_wifi, PAGE_settings_wifi_length); break;
    case SUBPAGE_LEDS    : response = request->beginResponse_P(200, "text/html", PAGE_settings_leds, PAGE_settings_leds_length); break;
    case SUBPAGE_UI      : response = request->beginResponse_P(200, "text/html", PAGE_settings_ui,   PAGE_settings_ui_length);   break;
    case SUBPAGE_SYNC    : response = request->beginResponse_P(200, "text/html", PAGE_settings_sync, PAGE_settings_sync_length); break;
    case SUBPAGE_TIME    : response = request->beginResponse_P(200, "text/html", PAGE_settings_time, PAGE_settings_time_length); break;
    case SUBPAGE_SEC     : response = request->beginResponse_P(200, "text/html", PAGE_settings_sec,  PAGE_settings_sec_length);  break;
    #ifdef WLED_ENABLE_DMX
    case SUBPAGE_DMX     : response = request->beginResponse_P(200, "text/html", PAGE_settings_dmx,  PAGE_settings_dmx_length);  break;
    #endif
    case SUBPAGE_UM      : response = request->beginResponse_P(200, "text/html", PAGE_settings_um,   PAGE_settings_um_length);   break;
    case SUBPAGE_UPDATE  : response = request->beginResponse_P(200, "text/html", PAGE_update,        PAGE_update_length);        break;
    #ifndef WLED_DISABLE_2D
    case SUBPAGE_2D      : response = request->beginResponse_P(200, "text/html", PAGE_settings_2D,   PAGE_settings_2D_length);   break;
    #endif
    case SUBPAGE_LOCK    : {
      correctPIN = !strlen(settingsPIN); // lock if a pin is set
      createEditHandler(correctPIN);
      serveMessage(request, 200, strlen(settingsPIN) > 0 ? PSTR("Settings locked") : PSTR("No PIN set"), FPSTR(s_redirecting), 1);
      return;
    }
    case SUBPAGE_PINREQ  : response = request->beginResponse_P(200, "text/html", PAGE_settings_pin,  PAGE_settings_pin_length);  break;
    case SUBPAGE_CSS     : response = request->beginResponse_P(200, "text/css",  PAGE_settingsCss,   PAGE_settingsCss_length);   break;
    case SUBPAGE_JS      : serveSettingsJS(request); return;
    case SUBPAGE_WELCOME : response = request->beginResponse_P(200, "text/html", PAGE_welcome,       PAGE_welcome_length);       break;
    default:  response = request->beginResponse_P(200, "text/html", PAGE_settings,      PAGE_settings_length);      break;
  }
  response->addHeader(FPSTR(s_content_enc),"gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}

#define MDNS_NAME DEVICENAME_CTR ".local"
char cmDNS[] = MDNS_NAME;           

//Is this an IP?
bool isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

bool captivePortal(AsyncWebServerRequest *request)
{
  if (ON_STA_FILTER(request)) return false; //only serve captive in AP mode
  String hostH;
  if (!request->hasHeader("Host")) return false;
  hostH = request->getHeader("Host")->value();

  if (!isIp(hostH) && hostH.indexOf("wled.me") < 0 && hostH.indexOf(cmDNS) < 0) {
    DEBUG_PRINTLN("Captive portal");
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://4.3.2.1"));
    request->send(response);
    return true;
  }
  return false;
}

//This function provides a json with info on the number of LEDs connected
// it is needed by artemis to know how many LEDs to display on the surface
void handleConfig(AsyncWebServerRequest *request)
{
DEBUG_LINE_HERE;
  String config = (String)"{\
  \"channels\": [\
    {\
      \"channel\": 1,\
      \"leds\": 123\
    },\
    {\
      \"channel\": 2,\
      \"leds\": " + "0" + "\
    },\
    {\
      \"channel\": 3,\
      \"leds\": " + "0" + "\
    },\
    {\
      \"channel\": 4,\
      \"leds\": " + "0" + "\
    }\
  ]\
}";

Serial.println(config);
DEBUG_LINE_HERE;

  request->send(200, "application/json", config);
DEBUG_LINE_HERE;
}


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


bool otaLock = false;

void initServer()
{
  //CORS compatiblity
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

#ifdef WLED_ENABLE_WEBSOCKETS
  #ifndef WLED_DISABLE_2D
  pCONT_web->server->on("/liveview2D", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveviewws2D, PAGE_liveviewws2D_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif
#endif
  pCONT_web->server->on("/liveview", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveview, PAGE_liveview_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });

  //settings page
  pCONT_web->server->on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    serveSettings(request);
  });

  // "/settings/settings.js&p=x" request also handled by serveSettings()

  pCONT_web->server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", PAGE_settingsCss, PAGE_settingsCss_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });

  pCONT_web->server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!handleFileRead(request, "/favicon.ico"))
    {
      request->send_P(200, "image/x-icon", favicon, 156);
    }
  });

  pCONT_web->server->on("/welcome", HTTP_GET, [](AsyncWebServerRequest *request){
    serveSettings(request);
  });

  pCONT_web->server->on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    serveMessage(request, 200,F("Rebooting now..."),F("Please wait ~10 seconds..."),129);
    // doReboot = true;
  });

  pCONT_web->server->on("/settings", HTTP_POST, [](AsyncWebServerRequest *request){
    serveSettings(request, true);
  });

  pCONT_web->server->on("/json", HTTP_GET, [](AsyncWebServerRequest *request){
    serveJson(request);
  });

  // AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/json", [](AsyncWebServerRequest *request) {
  //   bool verboseResponse = false;
  //   bool isConfig = false;

  //   if (!requestJSONBufferLock(14)) return;

  //   DeserializationError error = deserializeJson(doc, (uint8_t*)(request->_tempObject));
  //   JsonObject root = doc.as<JsonObject>();
  //   if (error || root.isNull()) {
  //     releaseJSONBufferLock();
  //     request->send(400, "application/json", F("{\"error\":9}")); // ERR_JSON
  //     return;
  //   }
  //   if (root.containsKey("pin")) checkSettingsPIN(root["pin"].as<const char*>());

  //   const String& url = request->url();
  //   isConfig = url.indexOf("cfg") > -1;
  //   if (!isConfig) {
  //     /*
  //     #ifdef WLED_DEBUG
  //       DEBUG_PRINTLN(F("Serialized HTTP"));
  //       serializeJson(root,Serial);
  //       DEBUG_PRINTLN();
  //     #endif
  //     */
  //     verboseResponse = deserializeState(root);
  //   } else {
  //     if (!correctPIN && strlen(settingsPIN)>0) {
  //       request->send(403, "application/json", F("{\"error\":1}")); // ERR_DENIED
  //       releaseJSONBufferLock();
  //       return;
  //     }
  //     verboseResponse = deserializeConfig(root); //use verboseResponse to determine whether cfg change should be saved immediately
  //   }
  //   releaseJSONBufferLock();

  //   if (verboseResponse) {
  //     if (!isConfig) {
  //       serveJson(request); return; //if JSON contains "v"
  //     } else {
  //       doSerializeConfig = true; //serializeConfig(); //Save new settings to FS
  //     }
  //   }
  //   request->send(200, "application/json", F("{\"success\":true}"));
  // }, JSON_BUFFER_SIZE);
  // pCONT_web->server->addHandler(handler);

  pCONT_web->server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });

  pCONT_web->server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });

  pCONT_web->server->on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)ESP.getFreeHeap());
  });

#ifdef WLED_ENABLE_USERMOD_PAGE
  pCONT_web->server->on("/u", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_usermod, PAGE_usermod_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
#endif

  pCONT_web->server->on("/teapot", HTTP_GET, [](AsyncWebServerRequest *request){
    serveMessage(request, 418, F("418. I'm a teapot."), F("(Tangible Embedded Advanced Project Of Twinkling)"), 254);
  });

  pCONT_web->server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {},
        [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                      size_t len, bool final) {handleUpload(request, filename, index, data, len, final);}
  );

#ifdef WLED_ENABLE_SIMPLE_UI
  pCONT_web->server->on("/simple.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/simple.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
#endif

  pCONT_web->server->on("/iro.js", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", iroJs, iroJs_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });

  pCONT_web->server->on("/rangetouch.js", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", rangetouchJs, rangetouchJs_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });

  // createEditHandler(correctPIN);

#ifndef WLED_DISABLE_OTA
  //init ota page
  pCONT_web->server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_GET \"/update\""));
    // if (otaLock) {
    //   serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_ota), 254);
    // } else
      serveSettings(request); // checks for "upd" in URL and handles PIN
  });

  pCONT_web->server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_POST \"/update\""));
    // if (!correctPIN) {
    //   serveSettings(request, true); // handle PIN page POST request
    //   return;
    // }
    if (Update.hasError() || otaLock) {
      serveMessage(request, 500, F("Update failed!"), F("Please check your file and retry!"), 254);
    } else {
      serveMessage(request, 200, F("Update successful!"), F("Rebooting..."), 131);
      // doReboot = true;
    }
  },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if (!correctPIN || otaLock) return;
    if(!index){
      DEBUG_PRINTLN(F("OTA Update Start"));
      // WLED::instance().disableWatchdog();
      // usermods.onUpdateBegin(true); // notify usermods that update is about to begin (some may require task de-init)
      // lastEditTime = millis(); // make sure PIN does not lock during update
      #ifdef ESP8266
      Update.runAsync(true);
      #endif
      Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000);
    }
    if(!Update.hasError()) Update.write(data, len);
    if(final){
      if(Update.end(true)){
        DEBUG_PRINTLN(F("Update Success"));
      } else {
        DEBUG_PRINTLN(F("Update Failed"));
        // usermods.onUpdateBegin(false); // notify usermods that update has failed (some may require task init)
        // WLED::instance().enableWatchdog();
      }
    }
  });
#else
  pCONT_web->server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    serveMessage(request, 501, "Not implemented", F("OTA updating is disabled in this build."), 254);
  });
#endif


  #ifdef WLED_ENABLE_DMX
  pCONT_web->server->on("/dmxmap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", PAGE_dmxmap     , dmxProcessor);
  });
  #else
  pCONT_web->server->on("/dmxmap", HTTP_GET, [](AsyncWebServerRequest *request){
    // serveMessage(request, 501, "Not implemented", F("DMX support is not enabled in this build."), 254);
  });
  #endif

  pCONT_web->server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // if (captivePortal(request)) return;
    bool showWelcomePage = false;
    if (!showWelcomePage || request->hasArg(F("sliders"))){
      serveIndex(request);
    } else {
      serveSettings(request);
    }
  });

  #ifdef WLED_ENABLE_PIXART
  pCONT_web->server->on("/pixart.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/pixart.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pixart, PAGE_pixart_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif

  #ifndef WLED_DISABLE_PXMAGIC
  pCONT_web->server->on("/pxmagic.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/pxmagic.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pxmagic, PAGE_pxmagic_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif

  pCONT_web->server->on("/cpal.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/cpal.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_cpal, PAGE_cpal_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });

  #ifdef WLED_ENABLE_WEBSOCKETS
  pCONT_web->server->addHandler(&ws);
  #endif


  // pCONT->Tasker_Interface(FUNC_WEB_ADD_HANDLER);/
  pCONT->Tasker_Interface(FUNC_WEB_ADD_HANDLER);

  //called when the url is not defined here, ajax-in; get-settings
  pCONT_web->server->onNotFound([](AsyncWebServerRequest *request){
    DEBUG_PRINTLN("Not-Found HTTP call:");
    DEBUG_PRINTLN("URI: " + request->url());
    if (captivePortal(request)) return;

    //make API CORS compatible
    if (request->method() == HTTP_OPTIONS)
    {
      AsyncWebServerResponse *response = request->beginResponse(200);
      response->addHeader(F("Access-Control-Max-Age"), F("7200"));
      request->send(response);
      return;
    }

    // if(handleSet(request, request->url())) return;
    // #ifndef WLED_DISABLE_ALEXA
    // if(espalexa.handleAlexaApiCall(request)) return;
    // #endif
    // if(handleFileRead(request, request->url())) return;
    AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
}







// *********************************************************************************************
//  * Interface
// *********************************************************************************************

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

// DEBUG_LINE_HERE;

  switch(function)
  {
    case FUNC_INIT:
      server = new AsyncWebServer(80);
    break;
  }


  switch(function){
    case FUNC_INIT:
      // init();


// DEBUG_LINE_HERE;
          
//       //CORS compatiblity
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

// DEBUG_LINE_HERE;
//       server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send(200, "text/plain", (String)millis());
//       });

// DEBUG_LINE_HERE;
//       server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){ 
//         handleConfig(request);
//       });

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     // if (captivePortal(request)) return;
//     // if (!showWelcomePage || request->hasArg(F("sliders"))){
//     //   serveIndex(request);
//     // } else {
//     //   serveSettings(request);
//     // }

//       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
    
// DEBUG_LINE_HERE;
//       request->send(response);
//   });

//     server.onNotFound(notFound);
// DEBUG_LINE_HERE;
// //     //called when the url is not defined here, ajax-in; get-settings
// //     server.onNotFound([](AsyncWebServerRequest *request){
// //       // DEBUG_PRINTLN("Not-Found HTTP call:");
// //       // DEBUG_PRINTLN("URI: " + request->url());
// //       // if (captivePortal(request)) return;

// //       // //make API CORS compatible
// //       // if (request->method() == HTTP_OPTIONS)
// //       // {
// //       //   AsyncWebServerResponse *response = request->beginResponse(200);
// //       //   response->addHeader(F("Access-Control-Max-Age"), F("7200"));
// //       //   request->send(response);
// //       //   return;
// //       // }

// // DEBUG_LINE_HERE;
// //       // if(handleSet(request, request->url())) return;
// //       // #ifndef WLED_DISABLE_ALEXA
// //       // if(espalexa.handleAlexaApiCall(request)) return;
// //       // #endif
// //       // if(handleFileRead(request, request->url())) return;
// //       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
// //       // response->addHeader(FPSTR(s_content_enc),"gzip");
// //       // setStaticContentCacheHeaders(response);
// // DEBUG_LINE_HERE;
// //       request->send(response);
// //     });

// DEBUG_LINE_HERE;

//     server.begin();

    break;
  //   case FUNC_LOOP:
  //     PollDnsWebserver();
  //   break; 
  case FUNC_WIFI_CONNECTED:

// DEBUG_LINE_HERE;
// return 0;
    // server.begin();
    
initServer();

// DEBUG_LINE_HERE;
          
//       //CORS compatiblity
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

// DEBUG_LINE_HERE;
//       server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send(200, "text/plain", (String)millis());
//       });

// DEBUG_LINE_HERE;
//       server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){ 
//         handleConfig(request);
//       });

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     // if (captivePortal(request)) return;
//     // if (!showWelcomePage || request->hasArg(F("sliders"))){
//       serveIndex(request);
//     // } else {
//     //   serveSettings(request);
//     // }
//   });

// //       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
    
// // DEBUG_LINE_HERE;
// //       request->send(response);
// //   });

//     // server.onNotFound(notFound);
// DEBUG_LINE_HERE;
//     //called when the url is not defined here, ajax-in; get-settings
//     server.onNotFound([](AsyncWebServerRequest *request){
//       DEBUG_PRINTLN("Not-Found HTTP call:");
//       DEBUG_PRINTLN("URI: " + request->url());
//       // if (captivePortal(request)) return;

//       //make API CORS compatible
//       if (request->method() == HTTP_OPTIONS)
//       {
//         AsyncWebServerResponse *response = request->beginResponse(200);
//         response->addHeader(F("Access-Control-Max-Age"), F("7200"));
//         request->send(response);
//         return;
//       }

//       // if(handleSet(request, request->url())) return;
//       // #ifndef WLED_DISABLE_ALEXA
//       // if(espalexa.handleAlexaApiCall(request)) return;
//       // #endif
//       // if(handleFileRead(request, request->url())) return;
//       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
//       response->addHeader(FPSTR(s_content_enc),"gzip");
//       setStaticContentCacheHeaders(response);
// DEBUG_LINE_HERE;
//       request->send(response);
//     });

DEBUG_LINE_HERE;

    pCONT_web->server->begin();

DEBUG_LINE_HERE;



  break;
  //   case FUNC_EVERY_SECOND:

  //   break;
  //   case FUNC_WEB_ADD_HANDLER:
  //     WebPage_Root_AddHandlers();
  //   break;
  //   case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
  //       JsonBuilderI->Add("/module_draw.json",-1); 
  //       // JsonBuilderI->Add("/web_top_bar.json",1000); 
  //       JsonBuilderI->Add("/root_status_any.json",1100);
  //   break;
  //   case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:
  //     //set value to force sending all below
  //   //no break
  //   case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:


  //   break;
  }

  return 0;

// DEBUG_LINE_HERE;
}






#endif