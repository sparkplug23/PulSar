#ifndef _MWEBSERVER_H
#define _MWEBSERVER_H 0.3

#include "1_ConfigUser/mUserConfig.h"

#ifdef DEBUG_WEBSERVER_MEMORY
typedef struct  FREEMEM_HANDLER{
  uint16_t      bytes_used;
  uint16_t      free_bytes;
  const char*   name_ptr;
}freemem_usage_t;
#endif


#ifdef USE_WEBSERVER

#include <Arduino.h>
#include "2_CoreSystem/Logging/mLogging.h"

// #include "3_Network/WebServer/WebPages_Progmem.h"
#include "html_ui.h"

#include <ArduinoJson.h>
#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/Settings/mSettings.h"

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
  #include <HTTPClient.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266HTTPClient.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif

#include <DNSServer.h>
#include "2_CoreSystem/Time/mTime.h"
#include "2_CoreSystem/Settings/mSettings.h"

#define HTTP_RESTART_RECONNECT_TIME           9000       // milliseconds
#define HTTP_OTA_RESTART_RECONNECT_TIME       20000      // milliseconds

#define D_DEFAULT_HTML_VALUE ""

#define WEBSEND_FREEMEMORY_START_LIMIT 2000 //1800 (1744 is known minimum)

enum DEVICE_CONTROL_BUTTON_IDS{
  DEVICE_CONTROL_BUTTON_OFF_ID=0,
  DEVICE_CONTROL_BUTTON_ON_ID=1,
  DEVICE_CONTROL_BUTTON_TOGGLE_ID=2
};

#define SEND_MINIMAL_STYLESHEET
#define SEND_MINIMAL_STYLESHEET1
// #define SEND_MINIMAL_STYLESHEET2


#define D_WEBURL_PAGE_INFO "/info"
DEFINE_PGM_CTR(PM_WEBURL_PAGE_INFO) D_WEBURL_PAGE_INFO;

#define D_WEBURL_LOAD_SCRIPT "/load_script.js"
DEFINE_PGM_CTR(PM_WEBURL_LOAD_SCRIPT) D_WEBURL_LOAD_SCRIPT;
#define D_WEBURL_LOAD_URLS "/load_urls.js"
DEFINE_PGM_CTR(PM_WEBURL_LOAD_URLS) D_WEBURL_LOAD_URLS;
#define D_WEBURL_DRAW_TABLE "/draw_table.json"
DEFINE_PGM_CTR(PM_WEBURL_DRAW_TABLE) D_WEBURL_DRAW_TABLE;


#define D_WEBURL_PAGE_INFO_LOAD_SCRIPT          D_WEBURL_PAGE_INFO D_WEBURL_LOAD_SCRIPT
DEFINE_PGM_CTR(PM_WEBURL_PAGE_INFO_LOAD_SCRIPT) D_WEBURL_PAGE_INFO_LOAD_SCRIPT;
#define D_WEBURL_PAGE_INFO_LOAD_URLS            D_WEBURL_PAGE_INFO D_WEBURL_LOAD_URLS
DEFINE_PGM_CTR(PM_WEBURL_PAGE_INFO_LOAD_URLS)   D_WEBURL_PAGE_INFO_LOAD_URLS;
#define D_WEBURL_PAGE_INFO_DRAW_TABLE           D_WEBURL_PAGE_INFO D_WEBURL_DRAW_TABLE
DEFINE_PGM_CTR(PM_WEBURL_PAGE_INFO_DRAW_TABLE)  D_WEBURL_PAGE_INFO_DRAW_TABLE;



/*
 * favicon
 */
const uint8_t favicon[] PROGMEM = {
  0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x00,
  0x18, 0x00, 0x86, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x89, 0x50,
  0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48,
  0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x06,
  0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x00, 0x4D, 0x49,
  0x44, 0x41, 0x54, 0x38, 0x8D, 0x63, 0xFC, 0xFF, 0xFF, 0x3F, 0x03, 0xB1,
  0x80, 0xD1, 0x9E, 0x01, 0x43, 0x31, 0x13, 0xD1, 0xBA, 0x71, 0x00, 0x8A,
  0x0D, 0x60, 0x21, 0xA4, 0x00, 0xD9, 0xD9, 0xFF, 0x0F, 0x32, 0x30, 0x52,
  0xDD, 0x05, 0xB4, 0xF1, 0x02, 0xB6, 0xD0, 0xA6, 0x99, 0x0B, 0x68, 0x1F,
  0x0B, 0xD8, 0x42, 0x9E, 0xAA, 0x2E, 0xA0, 0xD8, 0x00, 0x46, 0x06, 0x3B,
  0xCC, 0xCC, 0x40, 0xC8, 0xD9, 0x54, 0x75, 0x01, 0xE5, 0x5E, 0x20, 0x25,
  0x3B, 0x63, 0x03, 0x00, 0x3E, 0xB7, 0x11, 0x5A, 0x8D, 0x1C, 0x07, 0xB4,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
//const char HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&%s=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:

const char HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\\\"&%s=%d\\\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
const char HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE[] PROGMEM = 
  "<td style='width:%d%%'><button class='%s' onclick='la(\\\"&%s=%d\\\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
const char HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR[] PROGMEM = 
  "{sw}%d%%'{bc}'%s{oc}la(\\\"&%s=%d\\\");'>%s%s{bc2}";  // {bc} "><button class="


const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%%'>";
const char HTTP_TABLEEND[] PROGMEM =
  "</table>";

const char HTTP_MSG_SLIDER_GRADIENT[] PROGMEM =
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc2(\\\"%c\\\",%d,value)'>"
  "</div>";
const char HTTP_MSG_SLIDER_GRADIENT2[] PROGMEM =
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc2(\\\"%s\\\",%d,value)'>"
  "</div>";
const char HTTP_MSG_SLIDER_GRADIENT3[] PROGMEM =
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc3(\\\"%s\\\",value)'>"
  "</div>";
const char HTTP_MSG_SLIDER_TITLE_JUSTIFIED[] PROGMEM =
  "<div><span class='p'>%s</span><span class='q'>%s</span></div>";
const char HTTP_BTN_MAIN_VARIABLE_ACTION_NAME[] PROGMEM =
  "<p><form action='%s' method='get'><button>%s</button></form></p>";

  



enum UploadTypes { UPL_TASMOTA, UPL_SETTINGS, UPL_EFM8BB1 };

#ifdef ESP8266
const char PM_HTTP_HEAD3[] PROGMEM =
#elif ESP32
const char HTTP_HEAD2[] PROGMEM =
#endif
  "<!DOCTYPE html><html lang=\"" D_HTML_LANGUAGE "\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<title>%s - %s</title>";

  
const char HTTP_SCRIPT_COUNTER[] PROGMEM =
  "var cn=180;"                           // seconds
  "function u(){"
    "if(cn>=0){"
      "eb('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "window.onload=u;";
  
#define WEB_HANDLER_SCRIPT_ROOT_MICHAEL "/script/root_tas.js"
const char HTTP_SCRIPT_ROOT_MICHAEL[] PROGMEM =
  "var x=null,lt,to,tp,pc='';"            // x=null allow for abortion
  "function eb(s){"
    "return document.getElementById(s);"  // Save code space
  "}"
  // "function gcbid(s){"
  //   "return document.getElementsByClassName(s);"  // Save code space
  // "}"
  "function wl(f){"                       // Execute multiple window.onload
    "window.addEventListener('load',f);"
  "}"
  "function qs(s){"                       // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function start(p){"   //runs just once on loading
    "la(p);"
  "}"
  "function la(p){"
    "var a=' ';"
    "if(la.arguments.length==1){"
      "a=p;"
      "console.log(p);"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText"
          ".replace(/{t}/g,\"<table style='width:100%%'>\")"  // Main top table
          ".replace(/{s}/g,\"<tr><th>\")"
          ".replace(/{m}/g,\"</th><td>\")"
          ".replace(/{e}/g,\"</td></tr>\")"
          ".replace(/{t2}/g,\"</table>\")"
          ".replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "x.open('GET','./?m=1'+a,true);"       // ?m related to pWebServer->hasParam("m")
    "x.send();"
    "lt=setTimeout(la,%d);"               // Settings.web_refresh
  "}"
  "function lb(p){"
    "la('&d='+p);"                        // &d related to WebGetArg(request,"d", tmp, sizeof(tmp));
  "}"
  "function lc(p){"
    "la('&t='+p);"                        // &t related to WebGetArg(request,"t", tmp, sizeof(tmp));
  "}"
  // "wl(la);"
  "window.onload=start();"
;

#define WEB_HANDLER_HTTP_SCRIPT_SLIDERS "/script/root_sliders.js"
const char HTTP_SCRIPT_SLIDERS[] PROGMEM =
"function lc2(v,i,p){"
  "if(eb('s')){"                        // Check if Saturation is in DOM otherwise javascript fails on la()
    "if(v=='h'||v=='d'){"               // Hue or Brightness changed so change Saturation colors too
      "var sl=eb('sl4').value;"
      "eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';"
    "}"
  "}"
  "la('&'+v+i+'='+p);"
"}"
"function lc3(v,p){"
  "if(eb('s')){"                        // Check if Saturation is in DOM otherwise javascript fails on la()
    "if(v=='h'||v=='d'){"               // Hue or Brightness changed so change Saturation colors too
      "var sl=eb('sl4').value;"
      "eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';"
    "}"
  "}"
  "la('&'+v+'='+p);"
"}"
;

const char HTTP_SCRIPT_WIFI[] PROGMEM =
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}";
const char HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE[] PROGMEM =
  "<p><b>%s</b> (%s)<br/><input id='%s' name='%s' placeholder='%s' value='%s'></p>";

#define STR_HELPER2(x) #x
#ifndef STR2
#define STR2(x) STR_HELPER2(x)
#endif

// const char HTTP_SCRIPT_RELOAD[] PROGMEM =
//   "setTimeout(function(){location.href='.';}," STR2(HTTP_RESTART_RECONNECT_TIME) ");";
// // Local OTA upgrade requires more time to complete cp: before web ui should be reloaded
// const char HTTP_SCRIPT_RELOAD_OTA[] PROGMEM =
//   "setTimeout(function(){location.href='.';}," STR2(HTTP_OTA_RESTART_RECONNECT_TIME) ");";

const char HTTP_SCRIPT_CONSOL[] PROGMEM =

//WARNING -- repeated code
"var x=null,lt,to,tp,pc='';"            // x=null allow for abortion
  "function eb(s){"
    "return document.getElementById(s);"  // Save code space
  "}"
  "function wl(f){"                       // Execute multiple window.onload
    "window.addEventListener('load',f);"
  "}"
  "function qs(s){"                       // Alias to save code space
    "return document.querySelector(s);"
  "}"


//WARNING -- repetaed code from tas, to be joined
  "var sn=0;"                             // Scroll position
  "var idsc=0;"                             // Get most of weblog initially
  "function l(p){"                        // Console log and command service
    "var c,o,t;"
    "clearTimeout(lt);"
    "o='';"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"
      "o='&c1='+encodeURIComponent(c.value);"
      "c.value='';"
      "t.scrollTop=sn;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var z,d;"
          "d=x.responseText.split(/}1/);"  // Field separator
          "idsc=d.shift();"
          "if(d.shift()==0){t.value='';}"
          "z=d.shift();"
          "if(z.length>0){t.value+=z;}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
        "}"
      "};"
      "x.open('GET','cs?c2='+idsc+o,true);"  // Related to pWebServer->hasParam("c2") and WebGetArg(request,"c2", stmp, sizeof(stmp))
      "x.send();"
    "}"
    "lt=setTimeout(l,%d);"
    "return false;"
  "}"
  "window.onload=l;";

const char HTTP_MODULE_TEMPLATE_REPLACE[] PROGMEM =
  "}2%d'>%s (%d}3";                       // }2 and }3 are used in below os.replace

const char HTTP_SCRIPT_MODULE_TEMPLATE[] PROGMEM =
  "var os;"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=s;"
  "}"
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}";

const char HTTP_SCRIPT_TEMPLATE[] PROGMEM =
  "var c;"                                // Need a global for BASE
  "function x1(b){"
    "var i,j,g,k,o;"
    "o=b.responseText.split(/}1/);"       // Field separator
    "k=o.shift();"                        // Template name
    "if(eb('s1').value==''){"
      "eb('s1').value=k;"                 // Set NAME if not yet set
    "}"
    "os=o.shift();"                       // Complete GPIO sensor list
    "g=o.shift().split(',');"             // Array separator
    "j=0;"
    "for(i=0;i<13;i++){"                  // Supports 13 GPIOs
      "if(6==i){j=9;}"
      "if(8==i){j=12;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}"
    "g=o.shift();"
    "for(i=0;i<" STR2(GPIO_FLAG_USED) ";i++){"
      "p=(g>>i)&1;"
      "eb('c'+i).checked=p;"              // Set FLAG checkboxes
    "}"
    "if(" STR2(USER_MODULE) "==c){"
      "g=o.shift();"
      "eb('g99').value=g;"                // Set BASE for initial select
    "}"
  "}"
  "function st(t){"
    "c=t;"                                // Needed for initial BASE select
    "var a='tp?t='+t;"
    "ld(a,x1);"                           // ?t related to WebGetArg(request,"t", stemp, sizeof(stemp));
  "}"
  "function x2(a){"
    "os=a.responseText;"
    "sk(17,99);"                          // 17 = WEMOS
    "st(" STR2(USER_MODULE) ");"
  "}"
  "window.onload=ld('tp?m=1',x2);";       // ?m related to pWebServer->hasParam("m")

const char HTTP_SCRIPT_MODULE1[] PROGMEM =
  "function x1(a){"
    "os=a.responseText;"
    "sk(%d,99);"
  "}"
  "function x2(b){"
    "os=b.responseText;";

const char HTTP_SCRIPT_MODULE2[] PROGMEM =
  "}"
  "function sl(){"
    "ld('md?m=1',x1);"                     // ?m related to pWebServer->hasParam("m")
    "ld('md?g=1',x2);"                     // ?m related to pWebServer->hasParam("m")
  "}"
  "window.onload=sl;";

//phase out, not using this method
const char HTTP_SCRIPT_INFO_BEGIN[] PROGMEM =
  "function i(){"
    "var s,o=\"";
const char HTTP_SCRIPT_INFO_END[] PROGMEM =
    "\";"                                   // "}1" and "}2" means do not use "}x" in Information text
    "s=o.replace(/}1/g,\"</td></tr><tr><th>\").replace(/}2/g,\"</th><td>\");"
    "eb('i').innerHTML=s;"
  "}"
  "window.onload=i;";


const char HTTP_HEAD_STYLE1[] PROGMEM =
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:#%06x;}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  "input{width:100%%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  "select{width:100%%;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "textarea{resize:none;width:98%%;height:318px;padding:5px;overflow:auto;background:#%06x;color:#%06x;}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:verdana;background:#%06x;}"  // COLOR_BACKGROUND
  "td{padding:0px;}"
//   ;
// const char HTTP_HEAD_STYLE2[] PROGMEM =
  "button{border:0;border-radius:0.3rem;background:#%06x;color:#%06x;line-height:2.4rem;font-size:1.2rem;width:100%%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  "button:hover{background:#%06x;}"  // COLOR_BUTTON_HOVER
  ".bred{background:#%06x;}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:#%06x;}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{background:#%06x;}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:#%06x;}"  // COLOR_BUTTON_SAVE_HOVER
  ".bora{background:#%06x;}"  // COLOR_BUTTON2
  ".bora:hover{background:#%06x;}"  // COLOR_BUTTON2
  ".bform1{width:33%%;line-height:1.7rem}"
  "a{text-decoration:none;}"
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"; 

  
const char PM_HTTP_HEAD_STYLE1_MINIMAL[] PROGMEM =
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:#%06x;}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  // "input{width:100%%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  // "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  // "select{width:100%%;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  // "textarea{resize:none;width:98%%;height:318px;padding:5px;overflow:auto;background:#%06x;color:#%06x;}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:verdana;background:#%06x;}"  // COLOR_BACKGROUND
  //"td{padding:0px;}"
//   ;
// const char HTTP_HEAD_STYLE2[] PROGMEM =
  "button{border:0;border-radius:0.3rem;background:#%06x;color:#%06x;line-height:2.4rem;font-size:1.2rem;width:100%%;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  //"button:hover{background:#%06x;}"  // COLOR_BUTTON_HOVER
  // ".bred{background:#%06x;}"  // COLOR_BUTTON_RESET
  // ".bred:hover{background:#%06x;}"  // COLOR_BUTTON_RESET_HOVER
  // ".bgrn{background:#%06x;}"  // COLOR_BUTTON_SAVE
  // ".bgrn:hover{background:#%06x;}"  // COLOR_BUTTON_SAVE_HOVER
  // ".bora{background:#%06x;}"  // COLOR_BUTTON2
  // ".bora:hover{background:#%06x;}"  // COLOR_BUTTON2
  //"a{text-decoration:none;}"
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ; 
const char HTTP_STYLE_TITLE_CONTAINER_HEAD[] PROGMEM = 
  "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;padding-top:30px'>"  // COLOR_TEXT
#ifdef FIRMWARE_MINIMAL
  "<div style='text-align:center;color:#%06x;'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"  // COLOR_TEXT_WARNING
#endif
  "<div style='text-align:center;'><noscript>" D_NOSCRIPT "<br/></noscript>"
  //"<h3><font color='#%06x'>%s</font></h3>"
  "<h2><font color='#%06x'>%s</font></h2>"
  //"<h2>%s</h2>"
  "</div>"; 


/**
 *  Js to be developed
 *  Using a url list "refresh_urls" in sequential order will ask for new data from the esp,
 * Must be sequential, so only one is asked at a time
 * 
*/
// Init - send urls needed to load the pages divs, with rates of -1 (meaning only load once)
// Have 1 url be "Get new urls for fetching"

// #define WEB_HANDLE_ROOT "/main"
#define WEB_HANDLE_ROOT ""

#define WEB_HANDLER_STYLE_WEB_BASIC_ROOT "/style/web_basic_root.css"
#define WEB_HANLDE_JSON_WEB_TOP_BAR "/web_top_bar.json"
#define WEB_HANLDE_JSON_WEB_STATUS_POPOUT_DATA "/status_popout_data.json"

#define WEB_HANDLE_JSON_ROOT_STATUS_ANY "/root_status_any.json"



#define D_WEB_HANDLE_CONSOLE "/console" // change to animator_settings
#define D_WEB_HANDLE_CONSOLE_PAGE "/PAGEconsole" // change to animator_settings
DEFINE_PGM_CTR(PM_WEB_HANDLE_CONSOLE) D_WEB_HANDLE_CONSOLE;
#define D_BUTTON_NAME_CONSOLE_CTR "Console"
DEFINE_PGM_CTR(PM_BUTTON_NAME_CONSOLE_CTR) D_BUTTON_NAME_CONSOLE_CTR;



#define WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER_URLS_RATES_VAR "/script/root_data_fetcher_urls.js"
#define WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER_RUNTIME_URLS_RATES_VAR "/runtime/data_urls.json"
// const char SCRIPT_WEB_DATA_FETCHER_URLS_RATES_VAR[] PROGMEM = 
//   "const dfurls = [%s];"
//   "var dfrates = [%s];";

#define WEB_HANDLER_SCRIPT_INFORMATION_DATA_FETCHER_URLS_RATES_VAR "/script/info_data_fetcher_urls.js"

#define WEB_HANDLER_HTTP_SCRIPT_MINIMAL_LOAD_FETCHER_ROOT "/script/root_minimal.js"
// const char SCRIPT_MINIMAL_LOAD_FETCHER[] PROGMEM = 
// "function Parse_AddScript(d){" //start of parse json
//   "var s=document.createElement('script');" //s = script
//   "s.innerHTML = d;"
//   "console.log(s);"
//   "document.head.appendChild(s);"
// "}"
// "var url_sc=[%s];"
// "FLS();" // function load script
// "function FLS(){"
//   // "for(var i=0;i<url_sc.length<i++){"
//   "url_sc.forEach(function(u,i){" //url
//     "fetch(u)"
//     ".then((r)=>{"
//       "return r.text();"
//     "})"
//     ".then((d)=>{"
//       // "console.log(d);"
//       "Parse_AddScript(d);"
//     "});"
//   "});" //foreach
// "}"  //function
// ;

#define WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER "/script/root_data_fetcher_functionsbeta.js"
// const char SCRIPT_WEB_DATA_FETCHER[] PROGMEM = 
//   // "const dfurls = ['/div/rgb_drawdiv_root.json'];"
//   // "var dfrates = [-1];" 
//   // "const dfurls = [%s];"
//   // "var dfrates = [%s];"
  
//   "function TimeElapsed(t,e){"
//     "var n=new Date();"
//     "if(n-t>e){"
//       "return 1;"
//     "}"
//     "return 0;"
//   "}"
//   "var t=new Date();"
//   "var tSaved=[];"
//   "var tf=0;"//failure
//   "var fl=[];"//first load
//   "var ps_count=0;"// new, records how many packets have been sent and only allows more to go when limit is not hit
//   "var tURLfetch;"// = setTimeout(URLfetch,1000);" //setTimeout(URLfetch, 500); // slow start
//   "init_t();"// Init time array based on urls
//   "function init_t(){"
//     "for (var i=0;i<dfurls.length;i++){"
//       "tSaved[i] = t;"
//       "fl[i]=1;"
//     "}"
//   "}"
//   "URLfetch();"           //GOT IT, here, add this as its own script to init the call of the new json values
//   "function URLfetch(){"

//     "dfurls.forEach(function(u,i){" //url
//         // "console.log('ps_count',ps_count);"
//         // "console.log('url',u);"
//         "var f=1,t=tSaved[i],r=dfrates[i];" //default is run
//         //if disabled
//         "if(r==0){f=0;}" //flag to 0
//         //if(data refreshing types) and (first run) then run after 1 second
//         "if((r>0)&&(fl[i]==1)){"
//           "r=1000;"
//           "console.log('r=1000;');"
//         "}"
//         "if(r<0){" // run once
//           "r*=-1;" //make positive for Timereached
//         "}"  

//         "if(f){" //time should be checked
//             "if(TimeElapsed(t,r)){"
//               //"console.log('true');"
//               "if(ps_count<4){"//dont continue
//                 "ps_count++;" // requesting
//                 //"console.log('fb ps_count',ps_count);"
//                 "var suc=fetchUwT(u);"//,3000);"
//                 "if((suc==1)||(tf==1)){"  // temp fix, true/false, clear only if succesful
//                   "fl[i]=0;"   //should only clear if success and hence retried on next call
//                   //"console.log('fl[i]=0');"
//                 "}"
//                 "else{"
//                   //"console.log('fl[i]=ELSE');"
//                 "}"
//                 // "console.log('suc',suc);"
//                 "if((dfrates[i]<0)"
//                 //"&&(suc==1)"
//                 "){"// disable     
//                   "dfrates[i]=0;"
//                 "}else{"       //reset
//                   "tSaved[i]=new Date();"
//                 "}"
//               "}"//end ps_count
//             "}"//time elapsed
//         "}"//end f()
//     "});"
//     "clearTimeout(tURLfetch);"
//     "tURLfetch = setTimeout(URLfetch,1);"
//   "}"
//   "var controller = new AbortController();"
//   "var signal = controller.signal;"
//   "function fetchUwT(u){"//},timeout){"
//       "new Promise((resolve,reject)=>{"
//         // Set timeout timer
//         "let timer = setTimeout("
//           "()=>{"
//               "reject(new Error('TO'));"
//               "controller.abort();"
//               "controller = new AbortController();"
//               "signal = controller.signal;"
//               "ps_count--;" // failed, but cleared
//               "tf=0;"
//           "}," // 
//           "3000"//timeout"
//         ");"
//         "fetch(u,{"
//             "method:'get',"
//             "signal:signal,"
//           "})"
//           ".then((r)=>{"
//               // "return r.json();"
//               //"console.log(`Fetch complete. (Not aborted)`);"
//               "if(u.includes('.json')){return r.json();}"
//               "else{return r.text();}"//.json();"
//             "},"
//             "err=>reject(err)"
//           ")"
//           ".then((d)=>{"
//             //"console.log(d);"
//             "ps_count--;" // Successful packet back
//             //"console.log('fa ps_count',ps_count);"
//             //check for u type, call different function
//             "if(u.includes('.json')){"
//               //"d=d.json();"
//               // "console.log('json found');"
//               //"console.log(d);"
//               "Parse_JsonData(d);"
//             "}else "
//             "if(u.includes('.js')){" //json must be after js
//               //"console.log('js found');"
//               "Parse_AddScript(d);"
//             "}else "
//             "if(u.includes('.css')){" //json must be after js
//               //"console.log('css found');"
//               "Parse_AddStyle(d);"
//             "}"
//             "tf=1;"//success
//             //"console.log('success');"
//           "})"
//           ".catch(function(err){"
//             //"console.error(`Err:${err}`);"
//             "tf=0;"//success
//             //"console.log('!success');"
//           "})"
//           ".finally(function(){"
//             "clearTimeout(timer);"
//           "});" //close fetch           
//       "});" //close promise
//       //"console.log('finally1',tf);"
//       "return tf;"
//       //"console.log('finally2',tf);"
//     "}"//close fetchUwT
// ;

#define WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER_BETA "/script/root_data_fetcher_functionsbeta.js"
// const char SCRIPT_WEB_DATA_FETCHER_BETA[] PROGMEM = 
  // "const dfurls = ['/div/rgb_drawdiv_root.json'];"
  // "var dfrates = [-1];" 
  // "const dfurls = [%s];"
  // "var dfrates = [%s];"
  
//   "function TimeElapsed(t,e){"
//     "var n=new Date();"
//     "if(n-t>e){"
//       "return 1;"
//     "}"
//     "return 0;"
//   "}"
//   "var t=new Date();"
//   "var tSaved=[];"
//   "var tf=0;"//failure
//   "var fl=[];"//first load
//   "var ps_count=0;"// new, records how many packets have been sent and only allows more to go when limit is not hit
//   "var tURLfetch;"// = setTimeout(URLfetch,1000);" //setTimeout(URLfetch, 500); // slow start
//   "init_t();"// Init time array based on urls
//   "function init_t(){"
//     "for (var i=0;i<dfurls.length;i++){"
//       "tSaved[i] = t;"
//       "fl[i]=1;"
//     "}"
//   "}"
//   "URLfetch();"           //GOT IT, here, add this as its own script to init the call of the new json values
//   "function URLfetch(){"

//     "dfurls.forEach(function(u,i){" //url
//         // "console.log('ps_count',ps_count);"
//         // "console.log('url',u);"
//         "var f=1,t=tSaved[i],r=dfrates[i];" //default is run
//         //if disabled
//         "if(r==0){f=0;}" //flag to 0
//         //if(data refreshing types) and (first run) then run after 1 second
//         "if((r>0)&&(fl[i]==1)){"
//           "r=1000;"
//           "console.log('r=1000;');"
//         "}"
//         "if(r<0){" // run once
//           "r*=-1;" //make positive for Timereached
//         "}"  

//         "if(f){" //time should be checked
//             "if(TimeElapsed(t,r)){"
//               //"console.log('true');"
//               "if(ps_count<4){"//dont continue
//                 "ps_count++;" // requesting
//                 //"console.log('fb ps_count',ps_count);"
//                 "var suc=fetchUwT(u);"//,3000);"
//                 "if((suc==1)||(tf==1)){"  // temp fix, true/false, clear only if succesful
//                   "fl[i]=0;"   //should only clear if success and hence retried on next call
//                   //"console.log('fl[i]=0');"
//                 "}"
//                 "else{"
//                   //"console.log('fl[i]=ELSE');"
//                 "}"
//                 // "console.log('suc',suc);"
//                 "if((dfrates[i]<0)"
//                 //"&&(suc==1)"
//                 "){"// disable     
//                   "dfrates[i]=0;"
//                 "}else{"       //reset
//                   "tSaved[i]=new Date();"
//                 "}"
//               "}"//end ps_count
//             "}"//time elapsed
//         "}"//end f()
//     "});"
//     "clearTimeout(tURLfetch);"
//     "tURLfetch = setTimeout(URLfetch,1);"
//   "}"
//   "var controller = new AbortController();"
//   "var signal = controller.signal;"
//   "function fetchUwT(u){"//},timeout){"
//       "new Promise((resolve,reject)=>{"
//         // Set timeout timer
//         "let timer = setTimeout("
//           "()=>{"
//               "reject(new Error('TO'));"
//               "controller.abort();"
//               "controller = new AbortController();"
//               "signal = controller.signal;"
//               "ps_count--;" // failed, but cleared
//               "tf=0;"
//           "}," // 
//           "3000"//timeout"
//         ");"
//         "fetch(u,{"
//             "method:'get',"
//             "signal:signal,"
//           "})"
//           ".then((r)=>{"
//               // "return r.json();"
//               //"console.log(`Fetch complete. (Not aborted)`);"
//               "if(u.includes('.json')){return r.json();}"
//               "else{return r.text();}"//.json();"
//             "},"
//             "err=>reject(err)"
//           ")"
//           ".then((d)=>{"
//             //"console.log(d);"
//             "ps_count--;" // Successful packet back
//             //"console.log('fa ps_count',ps_count);"
//             //check for u type, call different function
//             "if(u.includes('.json')){"
//               //"d=d.json();"
//               // "console.log('json found');"
//               //"console.log(d);"
//               "Parse_JsonData(d);"
//             "}else "
//             "if(u.includes('.js')){" //json must be after js
//               //"console.log('js found');"
//               "Parse_AddScript(d);"
//             "}else "
//             "if(u.includes('.css')){" //json must be after js
//               //"console.log('css found');"
//               "Parse_AddStyle(d);"
//             "}"
//             "tf=1;"//success
//             //"console.log('success');"
//           "})"
//           ".catch(function(err){"
//             //"console.error(`Err:${err}`);"
//             "tf=0;"//success
//             //"console.log('!success');"
//           "})"
//           ".finally(function(){"
//             "clearTimeout(timer);"
//           "});" //close fetch           
//       "});" //close promise
//       //"console.log('finally1',tf);"
//       "return tf;"
//       //"console.log('finally2',tf);"
//     "}"//close fetchUwT
// ;


#define WEB_HANDLER_SCRIPT_WEB_PARSE_ADDSCRIPTSTYLE_FUNCTION "/script/fParse_scriptstyle.js"
// const char SCRIPT_WEB_PARSE_ADDSCRIPTSTYLE_FUNCTION[] PROGMEM = 
// // Parse function for handling divs/styles
// // "function Parse_AddScript(d){" //start of parse json
// //   "var s=document.createElement('script');" //s = script
// //   "s.innerHTML = d;"
// //   // "console.log(s);"
// //   "document.head.appendChild(s);"
// // "}"
// "function Parse_AddStyle(d){" //start of parse json
//   "var s=document.createElement('style');" //s = script
//   "s.innerHTML = d;"
//   // "console.log(s);"
//   "document.head.appendChild(s);"
// "}"
// "function ihr(x){"
//     "return x"
//     ".replace(/{o1}/g,\"<option value='\")"
//     ".replace(/{o2}/g,\"</option>\")"
//     ".replace(/{f1}/g,\"<font color='\")"
//     ".replace(/{f2}/g,\"</font>\")"
//     ".replace(/{sc}/g,\"'style='color:#\")"
//     ".replace(/{sw}/g,\"<td style='width:\")"
//     ".replace(/{bc}/g,\"><button class=\")"
//     ".replace(/{bc2}/g,\"</button></td>\")"
//     ".replace(/{t}/g,\"<table style='width:100%'>\")"
//     ".replace(/{t2}/g,\"</table>\")"
//     ".replace(/{oc}/g,\"'onclick='\")"
//     ".replace(/{dc}/g,\"<div class=\")"

//     //to fix
//     ".replace(/}1/g,\"</td></tr><tr><th>\")"
//     ".replace(/}2/g,\"</th><td>\")"
//     ";"
//   "}"
// ;


#define WEB_HANDLER_SCRIPT_WEB_PARSE_JSONDATA_FUNCTION "/script/parse_jsondata.js"
// Priority 3rd teir (runs during runtime, not during load)
// const char SCRIPT_WEB_PARSE_JSONDATA_FUNCTION[] PROGMEM = 
// // Parse function for handling divs/styles
// "function Parse_JsonData(json){" //start of parse json
//     // "console.log('ParseJson_Web');"
//   // Change properties
//   // "if(json.hasOwnProperty('rate')){"
//   //   "millis_w=json.rate;"
//   // "}"
//   // "if(json.hasOwnProperty('urls')){"
//   //   "urlsw=json.urls;"
//   // "}"
//   // "if(json.hasOwnProperty('mdraw')){"
//   //   // "urlsw = json.mdraw;" //multiple draws on ih (rows though?) or send one row
//   // "}"
  
// //         "if(json.hasOwnProperty('setvar')){" //formatted ih types
// //           // "console.log('setvar matched');"
// //           // "console.log(m);"


// // //move into its function
// //           "var m=json.setvar[0];"

// //           "if(m.hasOwnProperty('urls_data_fetch')){"

// //             "var n=m.urls_data_fetch;"
// //             // "console.log(n);"
// //             "for(i=0;i<n.length;i++){"
// //               // "console.log(n[i]);"
// //               "dfurls[i]=n[i];"
// //               // "console.log(dfurls[i]);"
// //             "}"
// //           "}"
// //           "if(m.hasOwnProperty('urls_data_rates')){"
// //             "var n=m.urls_data_rates;"
// //             // "console.log(n);"
// //             "for(i=0;i<n.length;i++){"
// //               // "console.log(n[i]);"
// //               "dfrates[i]=n[i];"
// //               // "console.log(dfurls[i]);"
// //             "}"
// //           "}"

// //         "}"


// // Add extra setting that draw inner X times

//     // "console.log(json);"
//     // "console.log(json['rgb_table_draw'].length);"
//     // "document.getElementsByClassName('rgb_table_draw')[0].innerHTML='test';"

//   // Change document/class properties
//   "for(c in json){"
//     // "console.log('c',c);"
//     // "console.log('j.l',json.length);"
//     "for(i=0;i<json[c].length;i++){"
//         // "console.log('length',json[c].length);"
//       "var id=0;" 
//       "if(json[c][i].hasOwnProperty('id')){"
//         "id=json[c][i].id;"
//         // "console.log(id);"
//       "}"
//       // Check if 255 meaning all
//       // "var setallamount = 0;" //default 0 does nothing
//       // "if(id == 255){setallamount = document.getElementsByClassName(c).length; id = 0;}"
//       // "for (j=0;j<=setallamount;j++){" //runs all if true
//       // "if(setallamount==0){j=id;}" //override 0
//         "if(json[c][i].hasOwnProperty('ih')){"
//           "document.getElementsByClassName(c)[id].innerHTML=json[c][i].ih;"  
//         "}"
//         "if(json[c][i].hasOwnProperty('evl')){"
//           "document.getElementById(c).value=json[c][i].evl;"  
//         "}"
//         "if(json[c][i].hasOwnProperty('ihr')){" //innerHTML with tasmota replace function eg = HR(); HR sent elsewhere
//           "document.getElementsByClassName(c)[id].innerHTML=ihr(json[c][i].ihr);" 
//         "}"
//         "if(json[c][i].hasOwnProperty('eihr')){" //innerHTML with tasmota replace function eg = HR(); HR sent elsewhere
//           // "console.log('iihr',ihr(json[c][i].eihr));"
//           "document.getElementById(c).innerHTML=ihr(json[c][i].eihr);" 
//         "}"
//         "if(json[c][i].hasOwnProperty('evl')){" //innerHTML with tasmota replace function eg = HR(); HR sent elsewhere
//           "document.getElementById(c).value=json[c][i].evl;"//ihr(json[c][i].iihr);" 
//           "console.log('value',document.getElementById(c).value);"
//         "}"
//         "if(json[c][i].hasOwnProperty('bc')){"
//           "document.getElementsByClassName(c)[id].style.backgroundColor=json[c][i].bc;"  
//           // "console.log('bc',json[c][i].bc);"
//         "}"
//         "if(json[c][i].hasOwnProperty('fc')){"
//           "document.getElementsByClassName(c)[id].style.color=json[c][i].fc;"  
//         "}"        
//         "if(json[c][i].hasOwnProperty('bclg')){" //background linear gradient, no stops
//             // "console.log('bclg');"
//           //bcg = style.background is linear, bclg is json array
//           "var s='linear-gradient(90deg,';"
//           "var l=json[c][i].bclg.length;"
//           "for(j=0;j<l;j++){"
//             "s+='#'+json[c][i].bclg[j];"
//             "if(j<l-1){s+=','}"
//           "}"
//           "s+=')';"
//           // #ifdef DEBUG_WEBPAGE_SCRIPTS_1
//             // "console.log(s);"
//           // #endif
//           "document.getElementsByClassName(c)[id].style.background=s;"
//         "}"
//         // // fixing the 0% error position
//         "if(json[c][i].hasOwnProperty('bcbl')){" //formatted ih types // "console.log('bcbl');"
//           //bcg = style.background is linear, bclg is json array
//           "var s='linear-gradient(90deg,';"
//           "var l=json[c][i].bcbl.length;"
//           "var ld,pos;"
//           "for(j=0;j<l;j++){"
//             "ld=json[c][i].bcbl[j];"
//               "pos=(j+1)*100/l;"
//             "s+='#'+ld+' 0,#'+ld+' '+pos+'%';"
//             "if(j<l-1){s+=','}"
//           "}"
//           "s+=')';"
//           // #ifdef DEBUG_WEBPAGE_SCRIPTS_1
//             //  "console.log(s);"
//           // #endif
//           "document.getElementsByClassName(c)[id].style.background=s;"
//             //  "console.log('s',document.getElementsByClassName(c)[id].style.background);"
//         "}"

//         // "console.log('c',c);"
        
//         // call fancy add functions directly from json 
//         "if(c=='function'){"
//            "if(json[c][i].hasOwnProperty('f_as')){" //formatted ih types
//             "console.log('Parse_AddScript');"
//             "Parse_AddScript(json[c][i].f_as);"
//           "}"
//         "}"


//       //"}" //end for set all
//     //   "if(id==255){break;}" //leave early if we set all  
//     "}" //end for
//   "}" //end for
// "}" //end parse
// ; //end of parsejson

  
const char HTML_DRAW_TOP_CONTAINER[] PROGMEM =
  "<div class='tb_gp'><span class='tb_it'></span><span class='tb_it'></span><span class='tb_it'></span></div>";

const char HTTP_MSG_SLIDER_HUE1[] PROGMEM =
  "<div><span class='p'>Hue 0</span><span class='q'>360</span></div>"
  "<div><input type='range' min='0' max='360' value='%d' onchange='lc(value)'></div>";

const char HTTP_MSG_SLIDER_SAT1[] PROGMEM =
  "<div><span class='p'>Saturation 0%</span><span class='q'>100%</span></div>"
  "<div><input type='range' min='0' max='100' value='%d' onchange='lc(value)'></div>";

const char HTTP_MSG_SLIDER_BRIGHTNESS1[] PROGMEM =
  "<div><span class='p'>Brightness 0%</span><span class='q'>100%</span></div>"
  "<div><input type='range' min='0' max='100' value='%d' onchange='lc(value)'></div>";

const char HTTP_MSG_SLIDER1[] PROGMEM =
  "<div><span class='p'>" D_COLDLIGHT "</span><span class='q'>" D_WARMLIGHT "</span></div>"
  "<div><input type='range' min='153' max='500' value='%d' onchange='lc(value)'></div>";
const char HTTP_MSG_SLIDER2[] PROGMEM =
  "<div><span class='p'>" D_DARKLIGHT "</span><span class='q'>" D_BRIGHTLIGHT "</span></div>"
  "<div><input type='range' min='1' max='100' value='%d' onchange='lb(value)'></div>";
const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br/><div style='text-align:center;'>" D_DEVICE_WILL_RESTART "</div><br/>";

const char HTTP_FORM_LOGIN[] PROGMEM =
  "<fieldset>"
  "<form method='post' action='/'>"
  "<p><b>" D_USER "</b><br/><input name='USER1' placeholder='" D_USER "'></p>"
  "<p><b>" D_PASSWORD "</b><br/><input name='PASS1' type='password' placeholder='" D_PASSWORD "'></p>"
  "<br/>"
  "<button>" D_OK "</button>"
  "</form></fieldset>";

const char HTTP_FORM_TEMPLATE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='tp'>";
const char HTTP_FORM_TEMPLATE_FLAG[] PROGMEM =
  "<p></p>"  // Keep close so do not use <br/>
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_FLAGS "&nbsp;</b></legend><p>"
  "<input id='c0' name='c0' type='checkbox'><b>" D_ALLOW_ADC0 "</b><br/>"
  "<input id='c1' name='c1' type='checkbox'><b>" D_ALLOW_ADC0_TEMP "</b><br/>"
  "</p></fieldset>";

const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='md'>"
  "<p></p><b>" D_MODULE_TYPE "</b> (%s)<br/><select id='g99' name='g99'></select><br/>"
  "<br/><table>";

const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='wi'>"
  "<p><b>" D_AP1_SSID "</b> (" STA_SSID1 ")<br/><input id='s1' name='s1' placeholder='" STA_SSID1 "' value='%s'></p>"
  "<p><b>" D_AP1_PASSWORD "</b><br/><input id='p1' name='p1' type='password' placeholder='" D_AP1_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
  "<p><b>" D_AP2_SSID "</b> (" STA_SSID2 ")<br/><input id='s2' name='s2' placeholder='" STA_SSID2 "' value='%s'></p>"
  "<p><b>" D_AP2_PASSWORD "</b><br/><input id='p2' name='p2' type='password' placeholder='" D_AP2_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
  "<p><b>" D_HOSTNAME "</b> (%s)<br/><input id='h' name='h' placeholder='%s' value='%s'></p>";

// const char HTTP_FORM_LOG1[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b>"
//   "</legend><form method='get' action='lg'>";
// const char HTTP_FORM_LOG2[] PROGMEM =
//   "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' placeholder='" SYS_LOG_HOST "' value='%s'></p>"
//   "<p><b>" D_SYSLOG_PORT "</b> (" STR2(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' placeholder='" STR2(SYS_LOG_PORT) "' value='%d'></p>"
//   "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR2(TELE_PERIOD) ")<br/><input id='lt' name='lt' placeholder='" STR2(TELE_PERIOD) "' value='%d'></p>";

// const char HTTP_FORM_OTHER[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='get' action='co'>"
//   "<p></p>"
//   "<fieldset><legend><b>&nbsp;" D_TEMPLATE "&nbsp;</b></legend>"
//   "<p><input id='t1' name='t1' placeholder='" D_TEMPLATE "' value='%s'></p>"
//   "<p><input id='t2' name='t2' type='checkbox'%s><b>" D_ACTIVATE "</b></p>"
//   "</fieldset>"
//   "<br/>"
//   "<b>" D_WEB_ADMIN_PASSWORD "</b><br/><input id='wp' name='wp' type='password' placeholder='" D_WEB_ADMIN_PASSWORD "' value='" D_ASTERIX "'><br/>"
//   "<br>"
//   "<input id='b1' name='b1' type='checkbox'%s><b>" D_MQTT_ENABLE "</b><br/>"
//   "<br/>";

//delee these
const char HTTP_FORM_END[] PROGMEM =
  "<br/>"
  "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";
const char HTTP_FORM_END_NOSAVE[] PROGMEM =
  "<br/>"
  "</form></fieldset>";


DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE)
  "<br/>"
  "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";
DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_END_NO_SAVE)
  "<br/>"
  "</form></fieldset>";


// DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE)
//   "<br/>"
//   "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
//   "</form></fieldset>";


// field name, 
DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_START_VARIABLE_JSON)
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>"
  "<form id='%s' method='post' action='%s'>"
  "<p></p>";
DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE_VARIABLE_JSON)
  "<br/>"
  "<button name='save' type='submit' class='button %s'>%s</button>"
  "</form></fieldset>";


#include "AsyncJson.h"
#include "ArduinoJson.h"

// const char HTTP_FORM_RST[] PROGMEM =
//   "<div id='f1' name='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_RESTORE_CONFIGURATION "&nbsp;</b></legend>";
// const char HTTP_FORM_UPG[] PROGMEM =
//   "<div id='f1' name='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
//   "<form method='get' action='u1'>"
//   "<br/><b>" D_OTA_URL "</b><br/><input id='o' name='o' placeholder='OTA_URL' value='%s'><br/>"
//   "<br/><button type='submit'>" D_START_UPGRADE "</button></form>"
//   "</fieldset><br/><br/>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
// const char HTTP_FORM_RST_UPG[] PROGMEM =
//   "<form method='post' action='u2' enctype='multipart/form-data'>"
//   "<br/><input type='file' name='u2'><br/>"
//   "<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
//   "</fieldset>"
//   "</div>"
//   "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";

// const char HTTP_FORM_CMND[] PROGMEM =
//   "<br/><textarea readonly id='t1' name='t1' cols='340' wrap='off'></textarea><br/><br/>"
//   "<form method='get' onsubmit='return l(1);'>"
//   "<input id='c1' name='c1' placeholder='" "Enter task and optional subtasks" "' autofocus><br/>"
//   //  "<br/><button type='submit'>Send command</button>"
//   "</form>";

const char HTTP_FORM_CMND2[] PROGMEM =
  "<form method='get' onsubmit='return l(1);'>"
  "<input id='c12b' name='c12b' placeholder='" "Enter json formatted command" "' autofocus><br/>"
    "<br/><button type='submit'>Send command</button>"
  "</form>";

  
const char HTTP_FORM_CONSOLE[] PROGMEM =
  "<br/><textarea readonly id='t1' name='t1' cols='340' wrap='off'></textarea><br/><br/>"
 ;

  

const char HTTP_COUNTER[] PROGMEM =
  "<br/><div id='t' name='t' style='text-align:center;'></div>";

// const char HTTP_END[] PROGMEM =
//   "<div style='text-align:right;font-size:11px;'><hr/><a href='https://bit.ly/tasmota' target='_blank' style='color:#aaa;'>Home Control System %s </a></div>"
//   "</div>"
//   "</body>"
//   "</html>";

// const char HTTP_END_WITH_UPDATE[] PROGMEM =
//   "<div style='text-align:right;font-size:11px;'><hr/><a href='%s' target='_blank' style='color:#%06x;'>%s %s (%s) </a></div>"
//   "</div>"
//   "</body>"
//   "</html>";

// const char PM_HTTP_END_WITH_UPDATE_NOEND[] PROGMEM =
//   "<div style='text-align:right;font-size:11px;'><hr/><a href='%s' target='_blank' style='color:#%06x;'>%s (%s) </a></div>"
//   "</div>";

const char PM_HTTP_END_WITH_UPDATE_NOEND[] PROGMEM =
  // "<div style='text-align:left;font-size:11px;'><hr/><a>%s</a></div>"
  "<div style='text-align:right;font-size:11px;'><hr/><a href='%s' target='_blank' style='color:#%06x;'>%s</a></div>"
  ;


#define D_WEB_HANDLER_STYLE_TOPBAR "/style/web_notif_bar.css"
DEFINE_PGM_CTR(PM_WEB_HANDLER_STYLE_TOPBAR) D_WEB_HANDLER_STYLE_TOPBAR;
DEFINE_PGM_CTR(PM_STYLE_TOPBAR)
{
  ".tb_con{"
    "position:fixed;"
    "top:0;"
    "left:0;"
    "right:0;"
    "padding:0px;"
  "}"
  ".tb_gp{"
    "width:100%;"
    "background:#222;"
    "display:flex;"
    "justify-content:space-between;"
    "height:auto;"
  "}"
  ".tb_it {"
    "color: #fff;" //white text
    //"background: green;"
    "padding: 5px 10px;" //tb & lr
    // "border-radius: 50%;"
    "position: relative;"
    // "top: -10px;"
    "font-size:1em"
  "}"
};



enum ButtonTitle {
  BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
  BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_CONSOLE,BUTTON_SYSTEM_SETTINGS,
  BUTTON_MODULE, BUTTON_WIFI, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, 
  BUTTON_RESTORE
  };
const char kButtonTitle[] PROGMEM =
  D_RESTART "|" D_RESET_CONFIGURATION "|"
  D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_CONSOLE "|" D_SYSTEM_SETTINGS "|"
  D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION  "|"
  ;
const char kButtonAction[] PROGMEM =
  ".|rt|"
  ".|cn|in|up|"
  D_WEB_HANDLE_CONSOLE_PAGE
  "|ss|"
  "md|wi|lg|co|tp|dl|rs|"
  ;


const char kButtonConfirm[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;

enum CTypes { CT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
// const char kContentTypes[] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";

// const char kLoggingOptions[] PROGMEM = D_SERIAL_LOG_LEVEL "|" D_WEB_LOG_LEVEL "|" D_SYS_LOG_LEVEL;
// const char kLoggingLevels[] PROGMEM = D_NONE "|" D_ERROR "|" D_INFO "|" D_DEBUG "|" D_MORE_DEBUG;

// const char kEmulationOptions[] PROGMEM = D_NONE "|" D_BELKIN_WEMO "|" D_HUE_BRIDGE;

const char kUploadErrors[] PROGMEM =
  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "|" D_UPLOAD_ERR_5 "|" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9
#ifdef USE_RF_FLASH
  "|" D_UPLOAD_ERR_10 "|" D_UPLOAD_ERR_11 "|" D_UPLOAD_ERR_12 "|" D_UPLOAD_ERR_13
#endif
  ;

const uint16_t DNS_PORT = 53;
enum HttpOptions {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER, HTTP_MANAGER_RESET_ONLY};











class mWebServer : public AsyncWebHandler{
  public:
    mWebServer(){};
    
    int8_t Tasker(uint8_t function);
    void init(void);

    
  void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len,
    size_t index, size_t total) override;



    
    void WebAppend_Root_Draw_Table_dList(uint8_t row_count, char const* value_handle, const char* dList_titles);

void WebAppend_Draw_Table_FP(const char* table_class_col2, const char* formatP_row1, ...);

    // String chunk_buffer = "";                         // Could be max 2 * CHUNKED_BUFFER_SIZE
    int minimum_signal_quality = -1;
    bool remove_duplicate_access_points = true;
    bool reset_web_log_flag = false;                  // Reset web console log
    uint8_t upload_error = 0;
    uint8_t upload_file_type;
    uint8_t upload_progress_dot_count;
    uint8_t config_block_count = 0;
    uint8_t config_xor_on = 0;

    uint16_t web_handle_load_delay = 1;
    uint16_t GetWebLoadNextTimeSlot();

    //const uint8_t CONFIG_FILE_XOR = 0x5A;       // Configuration file xor (0 = No Xor)
    uint8_t config_xor_on_set = 0x5A;

    #ifndef WEB_HANDLE_LOAD_DELAY 
      #define WEB_HANDLE_LOAD_DELAY 110
    #endif

    DNSServer *DnsServer;

    #ifdef ESP8266
      AsyncWebServer *pWebServer;
      // AsyncWebServer server(80);
    #endif
    #ifdef ESP32
    WebServer *pWebServer;
    #endif

    #ifndef WIFI_SOFT_AP_CHANNEL
      #define WIFI_SOFT_AP_CHANNEL                  1          // Soft Access Point Channel number between 1 and 11 as used by SmartConfig web GUI
    #endif

    const uint16_t CHUNKED_BUFFER_SIZE = 400;                // Chunk buffer size (should be smaller than half mqtt_date size)
    #define HTTP_REFRESH_TIME 1000                // milliseconds

    void WebSend_JSON_RootStatus_Table(AsyncWebServerRequest *request);    
     
     
void Console_JSON_Data(AsyncWebServerRequest *request);


void WebAppend_Root_Draw_PageTitleFields();
    void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Draw_ModuleTable();
void WebAppend_Root_Draw_PageButtons();
void Web_Root_Draw_Modules(AsyncWebServerRequest *request);


void Web_Console_Draw(AsyncWebServerRequest *request);

    int8_t flag_web_Add_handler_delayed_decounter = 5;

    AsyncWebServerRequest *request_web_command = nullptr;

    void WebGetArg(AsyncWebServerRequest *request, const char* arg, char* out, size_t max);
    void HandleFavicon(AsyncWebServerRequest *request);
    void WebSend_SendRuntimeURLs(AsyncWebServerRequest *request);

    // void send_mP(AsyncWebServerRequest *request, int code, const char* content_type, const char* formatP, ...)  ;

    void send_mP(AsyncWebServerRequest *request, int code, uint8_t content_type, const char* formatP, ...)  ;

    // Held for other modules to use, must be set prior to calling
    AsyncResponseStream *response_stream = NULL; //tbd
    char* buffer_writer_ctr = NULL;
    char* response_buffer = nullptr;
    uint16_t buffer_writer_len = 0;

    



    
uint16_t sprintf_P2(char* buffer, const char* formatP, ...);

    void WSButtonAppend2(char** buffer, uint8_t title_index);
    void WSSpaceButtonAppend2(char** buffer, uint8_t title_index);

    /**
     * This pointer to a pointer is used outside of this function to write into the WSBufferAppend_P
     * output buffer, it must be initialised before Tasker_Interface calls and recovered after? (maybe not)
     * The length it can write to is defined below
     * This method is temporary, since multiple simultaneous calls can overwrite this same pointer (same for large memory buffer)
     * */
    char** buffer_writer_internal = nullptr; 
    uint16_t buffer_writer_internal_length = 0; 
    #define BUFFER_WRITER_LENGTH DATA_BUFFER_PAYLOAD_MAX_LENGTH

    uint16_t WSContentSend_PI(const char* formatP, ...);
    uint16_t WSBufferAppend_PI2(const char* formatP, ...);
    
    void RemoveBufferSpaces_PI2(uint16_t bytes_to_remove);


void SendDrawDiv_RootTableShowParameters(AsyncWebServerRequest *request);

void SendDrawDiv_Information_Table(AsyncWebServerRequest *request);

uint16_t AppendBuffer_P2(char** buffer, const char* formatP, ...);
uint16_t AppendBuffer_PI2(const char* formatP, ...);


    bool RespondWebSend_UnableToAllocateBuffer(AsyncWebServerRequest *request);

   void WebSend_Root_Fetcher_URLs(AsyncWebServerRequest *request);
   void WebSend_Information_Fetcher_URLs(AsyncWebServerRequest *request);
   void WebSend_Root_ModuleButtons(AsyncWebServerRequest *request);

   

uint16_t AppendBuffer_nP2(char** buffer, uint16_t size, const char* formatP, ...);
uint16_t AppendBuffer_nPI2(uint16_t size, const char* formatP, ...);
void Web_All_LoadTime_Minimum_URLs(AsyncWebServerRequest *request);
void Web_All_LoadTime_Minimum_URLs2(AsyncWebServerRequest *request);



//append to internal buffer if any root messages table
void WebAppend_Root_Draw_TopBar();
void WebAppend_Root_Draw_ModuleButtons();
void WebAppend_Page_InformationTable();


void Web_Root_RunTime_URLs(AsyncWebServerRequest *request);
void Web_Root_LoadTime_URLs(AsyncWebServerRequest *request);
void Web_Root_LoadTime_URLs2(AsyncWebServerRequest *request);
void Web_Root_Draw(AsyncWebServerRequest *request);
void Web_Root_LoadScript(AsyncWebServerRequest *request);
void Web_Info_LoadScript(AsyncWebServerRequest *request);
void Web_Root_RunTimeScript(AsyncWebServerRequest *request);
void Web_Root_FillOptions(AsyncWebServerRequest *request);
void WebAppend_Root_FillOptions_Controls();
void WebAppend_Root_FillOptions_Controls_Selected();
void WebAppend_Root_Draw_Controls();

void Web_Root_UpdateURLs(AsyncWebServerRequest *request);

    
// For debugging only
struct FREEMEM_USAGE{
  uint16_t WebSend_JSON_WebServer_TopBar;
  uint16_t WebSend_SendRuntimeURLs;
  uint16_t Handle_Style_WebMinimalRoot;
}freemem_usage;


#ifdef DEBUG_WEBSERVER_MEMORY

  
  const char*     freemem_usage_name_json_shared = "json_shared\0";
  freemem_usage_t freemem_usage_json_shared;



  
#endif

void HandleReset(AsyncWebServerRequest *request);

// void WebSend_Response(AsyncWebServerRequest *request, int code, const char* contentType, char* content_ptr);
// void WebSend_Response_P(AsyncWebServerRequest *request, int code, const char* contentType, char* content_ptr);

void WebSend_Response(AsyncWebServerRequest *request, int code, uint8_t contentType, char* content_ptr);
void WebSend_Response_P(AsyncWebServerRequest *request, int code, uint8_t contentType, char* content_ptr);

#ifdef DEBUG_WEBSERVER_MEMORY
void FreeMem_Usage_Before(freemem_usage_t* memory_location);
void FreeMem_Usage_After(freemem_usage_t* memory_location);
#endif


// New async methods
    void WSBufferAppend_SystemVersionBar(AsyncResponseStream *response);
    void WSBufferAppend_SystemVersionBar(char** buffer);
    void WRHeaderAdd(AsyncResponseStream *response);
    // void WSContentSend_PI(const char* formatP, ...);

// Old methods to phase out
    static bool WifiIsInManagerMode();
    void ShowWebSource(int source);
    void ExecuteWebCommand(char* svalue, int source);
    void StartWebserver(int type, IPAddress ipweb);
    void StopWebserver(void);
    void WifiManagerBegin(bool reset_only);
    void PollDnsWebserver(void);
    bool WebAuthenticate(void);
    //bool HttpCheckPriviledgedAccess(bool autorequestauth = true);
    bool HttpCheckPriviledgedAccess();
    // void WSHeaderSend(AsyncWebServerRequest *request);
    void WSSend(AsyncWebServerRequest *request, int code, int ctype, const String& content);
    // void WSContentBegin(AsyncWebServerRequest *request, int code, int ctype);
    void WSContentBegin_Async(AsyncWebServerRequest *request, AsyncResponseStream *response, int code, int ctype);
    
    void WSBufferAppend_P(AsyncResponseStream *response, const char* formatP, ...);     // Content send snprintf_P char data;
    // void _WSContentSend(AsyncWebServerRequest *request, const String& content);
    // void WSContentFlush(AsyncWebServerRequest *request);
    // void _WSContentSendBuffer(AsyncWebServerRequest *request);
    void WSBufferAppend_P(AsyncWebServerRequest *request, const char* formatP, ...);
    void WSContentSend_PD(AsyncWebServerRequest *request, const char* formatP, ...);
    void WSStartAppend_P2(char** buffer, const char* title);
    void WSStartAppend_P2(char** buffer, const char* title, bool auth);
    // void WSStartAppend_P(AsyncWebServerRequest *request, const char* title);
    void WSStartAppend_P_Async(AsyncWebServerRequest *request, AsyncResponseStream *response, const char* title, bool auth);
    void WSStartAppend_P_Async(AsyncWebServerRequest *request, AsyncResponseStream *response, const char* title);
    
    void WSContentStop_Async(AsyncWebServerRequest *request, AsyncResponseStream *response);
    
    uint16_t WSContentButton_AsyncB(char *buffer, uint8_t title_index);
    // void WSContentButton_AsyncB2(buffer_t* buffer, uint8_t title_index);
    uint16_t WSContentSpaceButton_AsyncB(char *buffer, uint8_t title_index);
    uint16_t WSContentSendStyle(char *buffer);
    uint16_t WSContentSendStyle_P_NEW(char *buffer, const char* formatP, ...);

    // void WSContentStop_AsyncB2(buffer_t* buffer);
    // void WSContentSpaceButton_AsyncB2(buffer_t* buffer, uint8_t title_index);   
    
    void WebSend_JSON_WebServer_TopBar(AsyncWebServerRequest *request);
    void WebSend_JSON_WebServer_TopBar2(AsyncWebServerRequest *request);
    

void WebSend_JSON_WebServer_StatusPopoutData(AsyncWebServerRequest *request);
    
    void HandleSystemSettings(AsyncWebServerRequest *request);

    void HandleUpgradeFirmwareStart(AsyncWebServerRequest *request);
    void WSContentSendStyle_P(AsyncResponseStream *response,const char* formatP, ...);
    void WSContentSendStyle_Pb(AsyncResponseStream *response,const char* formatP, ...);
    void WSContentSendStyleb(AsyncResponseStream *response);
    void WSContentButton(AsyncResponseStream *response,uint8_t title_index);
    void WSContentSpaceButton(AsyncResponseStream *response,uint8_t title_index);
    void WSContentEnd(AsyncWebServerRequest *request);
    void WSContentEnd_Async(AsyncWebServerRequest *request, AsyncResponseStream *response);
    void WSContentStop(AsyncWebServerRequest *request);
    void WebRestart(AsyncWebServerRequest *request,uint8_t type);
    void HandleWifiLogin(AsyncWebServerRequest *request);
    void HandleRoot(AsyncWebServerRequest *request);

    uint16_t SendPage_Root(char* buffer);
    // void SendPage_Root2(buffer_t* buffer);


    void HandleRoot_Top3(AsyncWebServerRequest *request);
    void HandleRoot_Top4(AsyncWebServerRequest *request);
    void HandlePage_Root(AsyncWebServerRequest *request);
    
    void HandlePage_Console(AsyncWebServerRequest *request);



    void Web_Root_LoadScript_Test(AsyncWebServerRequest *request);
    
    void BuildPage_Root_Test();

    
    void SendPage_Root3(AsyncResponseStream *response);


    void BuildPage_Root(char** buffer);


    void Handle_Style_TopBar(AsyncWebServerRequest *request);

    // 0 means use default maximum
    bool CheckWebSendFreeMemoryTooLow(uint16_t memory_needed = 0);

    bool RespondWebSendFreeMemoryTooLow(AsyncWebServerRequest *request, uint16_t memory_needed = 0);



    void SendDrawDiv_Root2(AsyncWebServerRequest *request);

    void Handle_Style_WebMinimalRoot(AsyncWebServerRequest *request);
void WebPage_Root_AddHandlers();

    void WebSend_JSON_RootPage_Parameters_TopBarContainer(AsyncWebServerRequest *request);

    
    // uint16_t WSBufferAppend_P(char** buffer, const char* formatP, ...);
    uint16_t WSBufferAppend_P2(char** buffer, const char* formatP, ...);



    void BufferWriter_Start();
    void BufferWriter_Append_P(char** buffer, const char* formatP, ...);
    
    
    
    void WSContentSend_P_AsyncB_Internal(const char* formatP, ...);
    uint16_t WSContentStop_AsyncB(char *buffer);
    
    // uint16_t AppendBuffer_P(buffer_t *buffer, const char* formatP, ...);

    void WSContentSend_P_AsyncB_Internal2(const char* formatP, ...);     // Content send snprintf_P char data

    // void WSContentSend_P_AsyncB2(buffer_t* buffer, const char* formatP, ...);

    // uint8_t WSContentSendStyle2(buffer_t* buffer, uint8_t section_part = 255); //255 = all;
    // void WSContentSendStyle_P_NEW2(buffer_t* buffer, const char* formatP, ...);

uint16_t WSContentSend_Single_P2(char* buffer, const char* formatP, ...)   ;
    
    // void SetBufferBeforeTasker2(buffer_t* buffer);
    // void GetBufferAfterTasker2(buffer_t* buffer);

    void PrintToResponse_P(AsyncResponseStream *response, const char* formatP, ...);     // Content send snprintf_P char data


    // Space to save current instance of buffer so Tasker_Interface can access it
    // buffer_t buff_tasker;
    
    void WebHexCode(uint8_t i, const char* code);
    uint32_t WebColor(uint8_t i);
uint32_t WebColor(uint8_t r,uint8_t g,uint8_t b);
const char* WebColorCtr(uint8_t r,uint8_t g,uint8_t b, char* buffer);
// const char* WebColorCtr(char* buffer, uint8_t r,uint8_t g,uint8_t b);

    // uint8_t testarray[200];
    void HandleRoot_Top(AsyncWebServerRequest *request);

    void SetBufferBeforeTasker(char* buffer, uint16_t bytes_to_send);
    uint16_t GetBufferAfterTasker();


    uint8_t handleroot_section = 0;
    
    void handleQuery(AsyncWebServerRequest *request);
    
    uint16_t handleRoot_Sectioned(char* buf, uint16_t len);

    bool HandleRootStatusRefresh(AsyncWebServerRequest *request);
    void HandleConfiguration(AsyncWebServerRequest *request);
    void HandleTemplateConfiguration(AsyncWebServerRequest *request);
    void TemplateSaveSettings();
    void HandleModuleConfiguration(AsyncWebServerRequest *request);
    void ModuleSaveSettings(AsyncWebServerRequest *request);
    String htmlEscape(String s);
    void HandleWifiConfiguration(AsyncWebServerRequest *request);
    void WifiSaveSettings(AsyncWebServerRequest *request);
    void HandleLoggingConfiguration(AsyncWebServerRequest *request);
    void LoggingSaveSettings(AsyncWebServerRequest *request);
    void HandleOtherConfiguration(AsyncWebServerRequest *request);
    
    void OtherSaveSettings(AsyncWebServerRequest *request);
    void HandleBackupConfiguration(AsyncWebServerRequest *request);
    void HandleResetConfiguration(AsyncWebServerRequest *request);
    void HandleRestoreConfiguration(AsyncWebServerRequest *request);


    void HandleInformation(AsyncWebServerRequest *request);
    
    void WebAppend_Start_Head_P(const char* title, bool auth);
    void WebAppend_Start_Head_P(const char* title);
    void WebAppend_Minimal_Style();
    void WebAppend_Title();
    void WebAppend_Button_Spaced(uint8_t title_index);
    void WebAppend_Button(uint8_t title_index);
    void WebAppend_Button_Spaced(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr = nullptr, const char* confirm_action_ctr = nullptr);
    void WebAppend_Button(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr = nullptr, const char* confirm_action_ctr = nullptr);


    void WebAppend_Button2(const char* button_title_ctr, const char* action_ctr, const char* button_css_extra_style_ctr);

    void WebAppend_SystemVersionBar();
    void WebAppend_Script_P(PGM_P url);


void HandleInformation_TableDraw(AsyncWebServerRequest *request);

void Web_Base_Page_Draw(AsyncWebServerRequest *request);


    void HandleUpgradeFirmware(AsyncWebServerRequest *request);
    void HandleUploadDone(AsyncWebServerRequest *request);
    void HandleUploadLoop(AsyncWebServerRequest *request);
    void HandlePreflightRequest(AsyncWebServerRequest *request);
    void HandleHttpCommand(AsyncWebServerRequest *request);
    void HandleConsole(AsyncWebServerRequest *request);
    void HandleConsoleRefresh(AsyncWebServerRequest *request);
    void HandleNotFound(AsyncWebServerRequest *request);
    bool CaptivePortal(AsyncWebServerRequest *request);
    String UrlEncode(const String& text);
    int WebSend(char *buffer);
    bool JsonWebColor(const char* dataBuf);
    bool WebCommand(void);

    
  void ExecuteCommand(char *cmnd, int source);

  void ParseAndExecuteWebCommands(char* topic, uint8_t* data, unsigned int data_len);

  // struct WEBCOMMANDS{
  //   char classname[30];
  //   char command_key_short[30]; //short hand command to match against, 
  //   uint8_t command_index = 0; //short hand command to match against, 
  //   char command_value[30];
  //   uint8_t fWaiting = false;
  // }webcommand;

  uint8_t fConsole_active = false;
  uint8_t fConsole_history = false;

  uint32_t tSavedLoop;


};

#endif
#endif