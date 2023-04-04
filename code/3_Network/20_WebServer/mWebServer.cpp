
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER


#include "mWebServer.h"


const char* mWebServer::PM_MODULE_NETWORK_WEBSERVER_CTR = D_MODULE_NETWORK_WEBSERVER_CTR;
const char* mWebServer::PM_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR = D_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR;


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


// // Handle all webpage sends to allow for checking of freespace
// void mWebServer::WebSend_Response(AsyncWebServerRequest *request, int code, uint8_t contentType_id, char* content_ptr){

//   // Work out memory needed relative to body we want to send?
//   // Serial.println("HERE"); Serial.flush();

//   // char content_type[30]; // Should exist until send completes, might not as its async though
//   #ifdef DEBUG_WEBSERVER_MEMORY
//     FreeMem_Usage_Before(&freemem_usage_json_shared);
//   #endif
//   // WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  
 
//   // Check if there is enough RAM space, or else respond with 
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 
//   // Contine to send requested data 
//   request->send(code, contentType_id, content_ptr); 

//    #ifdef DEBUG_WEBSERVER_MEMORY
//     FreeMem_Usage_After(&freemem_usage_json_shared);
//   #endif 

// }


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




// *********************************************************************************************
//  * Interface
// *********************************************************************************************

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

DEBUG_LINE_HERE;

  // switch(function){
  //   case FUNC_INIT:
  //     init();
  //   break;
  //   case FUNC_LOOP:
  //     PollDnsWebserver();
  //   break; 
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
  // }

  return 0;

// DEBUG_LINE_HERE;
}




#endif