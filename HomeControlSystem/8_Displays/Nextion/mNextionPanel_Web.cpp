#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * Webpage Functions ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

/*
 case FUNC_WEB_ROOT_SEND_STATUS:{

      // char tmp[20];
      // sprintf(tmp,"Brightness %d %%",settings.brightness_percentage);

      // mcl->mweb->WSBufferAppend_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,tmp,"");
      // mcl->mweb->WSBufferAppend_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
      //     "c",               // c - Unique HTML idfrrrreee
      //     "#000", "#eee",//"#fff",    // Black to White
      //     4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
      //     0, 100,  // Range 0/1 to 100%
      //     settings.brightness_percentage,
      //     WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER
      // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
  
  
    }
    break;
    case FUNC_WEB_ADD_BUTTON:
      //mcl->mweb->WSBufferAppend_P(HTTP_BTN_MENU_SETTINGS);
      //mcl->mweb->WSBufferAppend_P(HTTP_BTN_MENU_NEXTION_SETTINGS); //USE self contained method outside of webserver, change rgb
    break;
    case FUNC_WEB_ADD_MAIN_BUTTON:{

      // // Power Toggle
      // mcl->mweb->WSBufferAppend_P(HTTP_TABLE100);
      // mcl->mweb->WSBufferAppend_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      // mcl->mweb->WSBufferAppend_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE, 
      //                           36, 
      //                           WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE, 
      //                           1, 
      //                           "Screen Backlight Toggle",""
      // );
      // mcl->mweb->WSBufferAppend_P(PSTR("</tr></table>"));
  
      // char stemp[100];
      // uint8_t keys_max = 9;

      // mcl->mweb->WSBufferAppend_P(HTTP_TABLE100);
      // mcl->mweb->WSBufferAppend_P(PSTR("<tr> Page Number</tr>"));
      // mcl->mweb->WSBufferAppend_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      // for (uint8_t idx = 0; idx < keys_max; idx++) {
      //   snprintf_P(stemp, sizeof(stemp), PSTR(" %d"), idx);
      //   mcl->mweb->WSBufferAppend_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE,         
      //       100 / keys_max, 
      //       WEB_HANDLE_NEXTION_PAGE_SET,
      //       idx, 
      //       (keys_max < 5) ? D_BUTTON_TOGGLE : "",
      //       (keys_max > 1) ? stemp : "");
      // }
      // mcl->mweb->WSBufferAppend_P(PSTR("</tr></table>"));


      // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_SETTINGS, "Nextion Settings");

    }
    break;
    case FUNC_WEB_ROOT_SEND_SCRIPT:{   
      // const char function[] =
      //   "function nextion_brightness(p){"
      //     "la('&" "nextion_brightness" "='+p);"                        // &t related to WebGetArg(request,"t", tmp, sizeof(tmp));
      //   "}";
      // mcl->mweb->WSBufferAppend_P(function);
      
      // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tasker %s"),"FUNC_WEB_ROOT_SEND_SCRIPT");
    }
    break;
    // case FUNC_WEB_SHOW_PARAMETERS:{
      
    //   // uint8_t fsize = 16;
    //   // char sensor_ctr[50];

    //   // char value_ctr[20];
    //   // memset(value_ctr,0,sizeof(value_ctr));

    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page", screen_press.page);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Event", screen_press.event);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Handled", screen_press.fHandled);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE_UNIT, "Duration", screen_press.duration, "ms");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_SVALUE, "Duration Name", (screen_press.duration<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Type", screen_press.type);      
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE_UNIT, "Screen Brightness", settings.brightness_percentage, "%");
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page Set", settings.page);
    //   // mcl->mweb->WSBufferAppend_P(HTTP_SNS_GENERIC_NVALUE, "Page Saved", settings.page_saved);

    // }
    // break;
    case FUNC_WEB_ADD_HANDLER:   
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_SETTINGS,      [this](){HandleNextionSettings(); }); // root for all nextion webpages

      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE,      [this](){webHandleNextionFirmware(); });   

      
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "up", [this](){webHandleNextionFirmware(); });
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "u1", [this](){HandleUpgradeFirmwareStart(); });  // URL file

      // // u2 = file upload
      // mcl->mweb->pWebServer->on("/"  "u20", HTTP_POST,[this](){HandleUploadDone(); },[this](){HandleUploadLoop(); });
      // mcl->mweb->pWebServer->on("/"  "u20", HTTP_OPTIONS, [this](){HandlePreflightRequest(); });

      
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_FIRMWARE "10",      [this](){webHandleNextionFirmware_PhaseOut(); });   



      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_TFTFILESIZE,   [this](){webHandleTftFileSize();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDDOWNLOAD,   [this](){webHandleLcdDownload();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDOTASUCCESS, [this](){webHandleLcdUpdateSuccess();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDOTAFAILURE, [this](){webHandleLcdUpdateFailure();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDREBOOT,     [this](){webHandleReboot();});
      // mcl->mweb->pWebServer->on("/" WEB_HANDLE_NEXTION_LCDUPLOAD, HTTP_POST, [this](){ mcl->mweb->pWebServer->send(200);}, [this](){webHandleLcdUpload();});
    break;
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;
    case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:
    
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "NEXTIONPanel" "}2%s"), " ");
      // //mcl->mweb->WSBufferAppend_P(PSTR("}1" "Version" "}2%s"), mSupportFloatToCStr(_ADALIGHTNEOPIXEL_H));
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "Type" "}2%s"), "WS28xx");
      // mcl->mweb->WSBufferAppend_P(PSTR("}1" "Pin" "}2%s"), "Serial0_RX");
      // mcl->mweb->WSBufferAppend_P(PSTR("}1}2&nbsp;"));  // Empty line

    break;

*/


void mNextionPanel::WebCommand_Parse(void)
{
  // char tmp[100];
  // uint8_t  arg_value = 0;

  // char arg_ctr[100]; memset(arg_ctr,0,sizeof(arg_ctr));

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_PAGE_SET);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_PAGE_SET);
  //   Command_SetPage(arg_value);
  // }

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_SLIDER);
  //   Command_SetBrightness(arg_value);
  // }

  // sprintf(arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE);
  // if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //   mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "WebCommand_Parse> Command_ToggleBrightness " D_JSON_COMMAND_NVALUE),arg_ctr,WEB_HANDLE_NEXTION_BRIGHTNESS_TOGGLE);
  //   Command_ToggleBrightness();
  // }
  
}










// TO BE DELETED
const char HTTP_HEAD2[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE2[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT2[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END2[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS2[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM2[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START2[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM2[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END2[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK2[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED2[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END2[] PROGMEM             = "</div></body></html>";
const char NEXTION_STYLE[] = "<style>button{background-color:#03A9F4;}body{width:60%;margin:auto;}input:invalid{border:1px solid red;}input[type=checkbox]{width:20px;}</style>";
// URL for auto-update "version.json"
const char UPDATE_URL[] = "http://haswitchplate.com/update/version.json";
// Default link to compiled Arduino firmware image
const char espFirmwareUrl[] = "http://haswitchplate.com/update/HASwitchPlate.ino.d1_mini.bin";
// Default link to compiled Nextion firmware images
const char lcdFirmwareUrl[] = "http://haswitchplate.com/update/HASwitchPlate.tft";
//END OF TO BE DELETED






const char HTTP_BTN_MENU_SETTINGS[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_SETTINGS "' method='get'><button>" D_NEXTION_SETTINIGS "</button></form></p>";
const char HTTP_BTN_MENU_FIRMWARE[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_FIRMWARE "' method='get'><button>" D_NEXTION_FIRMWARE "</button></form></p>";


/*****
 * 
 *  Page Name: Palette Editor
 * 
 * ******/
const char HTTP_SCRIPT_MODULE_TEMPLATEc[] PROGMEM =
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
// const char HTTP_SCRIPT_MODULE1[] PROGMEM =
//   "function x1(a){"
//     "os=a.responseText;"
//     "sk(%d,99);"
//   "}"
//   "function x2(b){"
//     "os=b.responseText;";
// const char HTTP_SCRIPT_MODULE2[] PROGMEM =
//   "}"
//   "function sl(){"
//     "ld('rgb_pal?m=1',x1);"                     // ?m related to pWebServer->hasParam("m")
//     "ld('rgb_pal?g=1',x2);"                     // ?m related to pWebServer->hasParam("m")
//   "}"
//   "window.onload=sl;";
const char HTTP_FORM_RGB_COLOUR_SELECTOR[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" "Colour Palette Editor" "&nbsp;</b></legend>"
  "<form method='get' action='rgb_pal'>"
  "<p></p><b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";



//<option value="0">Red (0)</option>
//"var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\");"
//}2 = \"<option value='\"
//}3 = \")</option>\"
// const char HTTP_MODULE_TEMPLATE_REPLACE[] PROGMEM =
//   "}2%d'>%s (%d}3";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE3[] PROGMEM =
  "}2%d'>%s}4";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE3_NUM[] PROGMEM =
  "}2%d'>%d}4";                       // }2 and }3 are used in below os.replace



// enum CTypes { CkContentTypesT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
// const char [] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";
  

  
/*****
 * 
 *  Page Name: Palette Editor
 * 
 * ******/
const char HTTP_SCRIPT_MODULE_TEMPLATEb[] PROGMEM =
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
const char HTTP_SCRIPT_MODULE1b[] PROGMEM =
  "function x0(a){"
    "os=a.responseText;"
    "sk(%d,0);"
  "}"
  "function x1(a){"
    "os=a.responseText;"
    "sk(%d,1);"
  "}"
  "function x2(a){"
    "os=a.responseText;"
    "sk(%d,2);"
  "}"
  "function x3(a){"
    "os=a.responseText;"
    "sk(%d,3);"
  "}"
  "function x4(a){"
    "os=a.responseText;"
    "sk(%d,4);"
  "}"
  "function x5(a){"
    "os=a.responseText;"
    "sk(%d,5);"
  "}"
  "function x6(a){"
    "os=a.responseText;"
    "sk(%d,6);"
  "}"
  "function x7(a){"
    "os=a.responseText;"
    "sk(%d,7);"
  "}"
  "function x8(a){"
    "os=a.responseText;"
    "sk(%d,8);"
  "}"
  ;
const char HTTP_SCRIPT_MODULE2b[] PROGMEM =
  "function sl(){"
    "ld('rgb_controls?up=1',x0);" // up = update percentage
    "ld('rgb_controls?po=1',x1);" // po = pixel order
    "ld('rgb_controls?ra=1',x2);" // ra = rate
    "ld('rgb_controls?pr=1',x3);" // pr = period time
    "ld('rgb_controls?tm=1',x4);" // tm = transition mode
    "ld('rgb_controls?am=1',x5);" // am = animaiton mode
    "ld('rgb_controls?pa=1',x6);" // pa  = palette
    "ld('rgb_controls?fl=1',x7);" // fl  = flasher
    "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id
  "}"
  "window.onload=sl;";
const char HTTP_FORM_RGB_COLOUR_SELECTORb[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
  "<form method='get' action='rgb_controls'>"
  "<p></p>"; //space
  //"<b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";


const char kButtonConfirmb[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;


//struct ITEM_1{
  const char kTitle_AmountNumber[] PROGMEM = 
    "1" "|" "2" "|" "3";
  //uint8_t kValue_AmountPercentage[6] = {1,10,25,50,75,100};

  // uint8_t 
  const char kTitle_ListHeadings[] PROGMEM = 
    "Pixels updated (%)" "|" 
    "Pixel Order" "|" 
    "Change Rate (secs)" "|" 
    "Change Period (secs)" "|" 
    "Transition Method" "|" 
    "Mode" "|" 
    "Palette" "|" 
    "Flasher" "|" 
    "Mixer (Running ID)";


// enum ButtonTitle {
//   BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
//   BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_CONSOLE,
//   BUTTON_MODULE, BUTTON_WIFI, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, 
//   BUTTON_RESTORE, 
//   BUTTON_COLOUR_CONTROLS, BUTTON_COLOUR_PALETTE_EDITOR, BUTTON_COLOUR_MIXER_EDITOR,
//   BUTTON_NEXTION_SETTINGS };
// const char kButtonTitle[] PROGMEM =
//   D_RESTART "|" D_RESET_CONFIGURATION "|"
//   D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_CONSOLE "|"
//   D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION  "|"
//   "Colour Controls" "|" "Colour Palette Editor" "|" "Mixer Editor"
//   "|" "Nextion Settings";
// const char kButtonAction[] PROGMEM =
//   ".|rt|"
//   ".|cn|in|up|cs|"
//   "md|wi|lg|co|tp|dl|rs|"
//   "rgb_controls" "|" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR2 "|" WEB_HANDLE_RGB_COLOUR_MIXER_EDITOR2
//   "|" WEB_HANDLE_NEXTION_SETTINGS;


#define D_COLOUR_PICKER "Colour Picker"

const char HTTP_FORM_RGB_SELECTOR1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_COLOUR_PICKER "&nbsp;</b>"
  "</legend><form method='get' action='" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR2 "'>";
const char HTTP_FORM_RGB_SELECTOR2[] PROGMEM =
  "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br/><input id='lh' name='lh' placeholder='" SYS_LOG_HOST "' value='%s'></p>"
  "<p><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br/><input id='lp' name='lp' placeholder='" STR(SYS_LOG_PORT) "' value='%d'></p>"
  "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br/><input id='lt' name='lt' placeholder='" STR(TELE_PERIOD) "' value='%d'></p>";

// const char HTTP_FORM_END[] PROGMEM =
//   "<br/>"
//   "<button name='save' type='submit' class='buttonh bgrn'>" D_SAVE "</button>"
//   "</form></fieldset>";
const char HTTP_FORM_RGB_LOAD_PIXELS[] PROGMEM =
  "<button name='loadpixels' type='submit' class='buttonh bgrn'>" "Load Pixels" "</button>";


const char kRGBPickerTitles[] PROGMEM = "Colour 0" "|" "Colour 1" "|" "Colour 2";
const char kRGBPickerItems[] PROGMEM = "Red" "|" "Green" "|" "Blue";

#define D_RGB_COLOUR_SELECTOR "Colour Palette Selector"
#define D_RGB_CONTROLS "Lights Settings"
#define D_NEXTION_SETTINGS "Nextion Settings"


const char HTTP_BTN_MENU_NEXTION_SETTINGS[] PROGMEM =
  "<p><form action='" WEB_HANDLE_NEXTION_SETTINGS "' method='get'><button>" D_NEXTION_SETTINGS "</button></form></p>";



void mNextionPanel::WebSave_NextionSettings(void)
{
  char tmp[100]; memset(tmp,0,sizeof(tmp));
  uint8_t  arg_value = 0;
  char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

  // // check palette selector
  // for(int ii=0;ii<WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE;ii++){
  //   sprintf(arg_ctr,"g%d\0",ii);
  //   if (mcl->mweb->pWebServer->hasParam(arg_ctr)) {
  //     mcl->mweb->WebGetArg(arg_ctr, tmp, sizeof(tmp));
  //     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
  //     update_all = true; //refresh all
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

  //     switch(ii){
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELSUPDATED_PERCENTAGE");
  //         // from mapped value to real value
  //         arg_value = animation.transition.PROGMEM pixels_to_update_as_percentage_map[arg_value];
  //         animation.transition.pixels_to_update_as_percentage = constrain(arg_value,0,100);
  //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage);
  //         // Also convert to number equivalent
  //         animation.transition.pixels_to_update_as_number = map(animation.transition.pixels_to_update_as_percentage,0,100,0,ledout.length);
  //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER,animation.transition.pixels_to_update_as_number);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELORDER");
  //         animation.transition.order = arg_value; // no map
  //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"RATE");
  //         animation.transition.rate_ms = arg_value*1000; //seconds to milliseconds
  //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.rate_ms);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PERIOD");
  //         animation.transition.time_ms = arg_value*1000; //seconds to milliseconds
  //         // If period > rate, increase period to rate
  //         animation.transition.time_ms = animation.transition.time_ms>animation.transition.rate_ms?animation.transition.rate_ms:animation.transition.time_ms;
  //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms); 
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"TRANSITIONMETHOD");
  //         animation.transition.method = arg_value;
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetTransitionMethodName());      
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"ANIMATIONMODE");
  //         animation.mode = arg_value;
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetAnimationModeName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PALETTE");
  //         animation.palette = arg_value;
  //         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteName());
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_EFFECTS: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
  //         flashersettings.function = arg_value;  
  //         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;  //restart animation/function
  //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "flasher.function = %d"),flashersettings.function);
  //       break;
  //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
  //         mixer.running_id = arg_value;          
  //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.mode.running_id = %d"),mixer.running_id);
  //       break;
  //       default: 
  //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "DEFAULT hasParam(\"%s\")=%d"),arg_ctr,arg_value);
  //       break;
  //     } //switch
  //   } //if
  // } //for

  // animation.fForceUpdate = true; // update leds now

} // end function

void mNextionPanel::HandleNextionSettings(void)
{

  // //if (!HttpCheckPriviledgedAccess()) { return; }

  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

  // if (mcl->mweb->pWebServer->hasParam("save")) {
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
  //   WebSave_NextionSettings();
  //   mcl->mweb->HandleRoot(); // return to main menu
  //   return;
  // }

  // // Send Tab name
  // mcl->mweb->WSStartAppend_P(D_NEXTION_SETTINGS);
  // // Send primary handler calls (m=1) and replace scripts
  // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE_TEMPLATEb);
  // // Send functions for callback
  // // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE1b, 
  // //                             1,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
  // //                             1,//animation.transition.order,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
  // //                             1,//(uint8_t)round(animation.transition.rate_ms/1000),    // "ld('rgb_controls?ra=1',x2);" // ra = rate
  // //                             1,//(uint8_t)round(animation.transition.time_ms/1000),    // "ld('rgb_controls?pr=1',x3);" // pr = period time
  // //                             1,//animation.transition.method,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
  // //                             1,//animation.mode,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
  // //                             1,//animation.palette,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
  // //                             1,//flashersettings.function,                             // "ld('rgb_controls?fl=1',x7);" // fl  = flasher  
  // //                             1//mixer.running_id                                 // "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id                          
  // //                           ); // Send lists
  // // Send functions part 2
  // mcl->mweb->WSBufferAppend_P(HTTP_SCRIPT_MODULE2b);
  // // Send style
  // mcl->mweb->WSContentSendStyleb();

  // // Buttons that will stay
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_FIRMWARE, "Firmware");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDREBOOT, "LCD Reboot");

  // mcl->mweb->WSBufferAppend_P(PSTR("<div></div>"));            // 5px padding
  // mcl->mweb->WSBufferAppend_P(PSTR("<div> Debug URLs normally called indirectly</div>"));            // 5px padding

  // // Extra handlers not usually called directly
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_TFTFILESIZE, "TFT File Size");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDDOWNLOAD, "LCD Download");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDOTASUCCESS, "LCD OTA Success");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDOTAFAILURE, "LCD OTA Failure");
  // mcl->mweb->WSBufferAppend_P(HTTP_BTN_MAIN_VARIABLE_ACTION_NAME, WEB_HANDLE_NEXTION_LCDUPLOAD, "LCD Upload");

  // mcl->mweb->WSContentSpaceButton(BUTTON_MAIN);
  // mcl->mweb->WSContentStop();

}



/***
 * Add subscribes for new connection to mqtt
 * Show update on panel
 * */
void mNextionPanel::wifiConnected()
{ // MQTT connection and subscriptions

  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));

  // Show connection success
  //nextionSendCmd("page 0");

  // char display_ctr[30];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");

  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "FUNC_WIFI_CONNECTED"));
      
      // nextionSendCmd("page 0");

      // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
      // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
  
      // nextionSetAttr("p[0].b[1].txt", display_ctr);

      // // hide QR code
      // nextionSendCmd("vis 3,0");
      // nextionSendCmd("page 5");
  // hide QR code
  // nextionSendCmd("vis 3,0");

}


void mNextionPanel::wifiDisconnected(){
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
  // char display_ctr[120];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connect to%s\\rFAILED rc=%s\"",WiFi.localIP().toString(),"192.168.1.65",pCONT_mqtt->pubsub->stateCtr());

  // QR code show wifi connect
  // nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\"");
  // // show item
  // nextionSendCmd("vis 3,1");
  
  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

  
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "FUNC_WIFI_DISCONNECTED"));

      // nextionSendCmd("page 0");

      // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
      // sprintf(display_ctr,"\"WiFi DISConnected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
  
      // nextionSetAttr("p[0].b[1].txt", display_ctr);

      // // hide QR code
      // nextionSendCmd("vis 3,1");



}






//   if (mdnsEnabled)
//   { // Setup mDNS service discovery if enabled
//     hMDNSService = MDNS.addService(nextionNode, "http", "tcp", 80);
//     // if (debugTelnetEnabled)
//     // {
//     //   MDNS.addService(nextionNode, "telnet", "tcp", 23);
//     // }
//     MDNS.addServiceTxt(hMDNSService, "app_name", "HASwitchPlate");
//     MDNS.addServiceTxt(hMDNSService, "app_version", String(nextionVersion).c_str());
//     MDNS.update();
//   }

//   if ((configPassword[0] != '\0') && (configUser[0] != '\0'))
//   { // Start the webserver with our assigned password if it's been configured...
//     httpOTAUpdate.setup(&webServer, "/update", configUser, configPassword);
//   }
//   else
//   { // or without a password if not
//     httpOTAUpdate.setup(&webServer, "/update");
//   }

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNotFound()
{ 
  
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNotFound"));
  // // webServer 404
  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending 404 to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = "File Not Found\n\n";
  // httpMessage += "URI: ";
  // httpMessage += mcl->mweb->pWebServer->uri();
  // httpMessage += "\nMethod: ";
  // httpMessage += (mcl->mweb->pWebServer->method() == HTTP_GET) ? "GET" : "POST";
  // httpMessage += "\nArguments: ";
  // httpMessage += mcl->mweb->pWebServer->args();
  // httpMessage += "\n";
  // for (uint8_t i = 0; i < mcl->mweb->pWebServer->args(); i++)
  // {
  //   httpMessage += " " + mcl->mweb->pWebServer->argName(i) + ": " + mcl->mweb->pWebServer->arg(i) + "\n";
  // }
  // mcl->mweb->pWebServer->send(404, "text/plain", httpMessage);
}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleSaveConfig()
{ 
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleSaveConfig"));// http://plate01/saveConfig
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
//     {
//       return mcl->mweb->pWebServer->requestAuthentication();
//     }
//   }
//   //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /saveConfig page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
//   String httpMessage = FPSTR(HTTP_HEAD2);
//   httpMessage.replace("{v}", String(nextionNode));
//   httpMessage += FPSTR(HTTP_SCRIPT2);
//   httpMessage += FPSTR(HTTP_STYLE2);
//   httpMessage += String(NEXTION_STYLE);

//   bool shouldSaveWifi = false;
//   // Check required values
//   if (mcl->mweb->pWebServer->arg("wifiSSID") != "" && mcl->mweb->pWebServer->arg("wifiSSID") != String(WiFi.SSID()))
//   { // Handle WiFi update
//     shouldSaveConfig = true;
//     shouldSaveWifi = true;
//     mcl->mweb->pWebServer->arg("wifiSSID").toCharArray(wifiSSID, 32);
//     if (mcl->mweb->pWebServer->arg("wifiPass") != String("**"))
//     {
//       mcl->mweb->pWebServer->arg("wifiPass").toCharArray(wifiPass, 64);
//     }
//   }
//   if (mcl->mweb->pWebServer->arg("mqttServer") != "" && mcl->mweb->pWebServer->arg("mqttServer") != String(mqttServer))
//   { // Handle mqttServer
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttServer").toCharArray(mqttServer, 64);
//   }
//   if (mcl->mweb->pWebServer->arg("mqttPort") != "" && mcl->mweb->pWebServer->arg("mqttPort") != String(mqttPort))
//   { // Handle mqttPort
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttPort").toCharArray(mqttPort, 6);
//   }
//   if (mcl->mweb->pWebServer->arg("nextionNode") != "" && mcl->mweb->pWebServer->arg("nextionNode") != String(nextionNode))
//   { // Handle nextionNode
//     shouldSaveConfig = true;
//     String lowerHaspNode = mcl->mweb->pWebServer->arg("nextionNode");
//     lowerHaspNode.toLowerCase();
//     lowerHaspNode.toCharArray(nextionNode, 16);
//   }
//   if (mcl->mweb->pWebServer->arg("groupName") != "" && mcl->mweb->pWebServer->arg("groupName") != String(groupName))
//   { // Handle groupName
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("groupName").toCharArray(groupName, 16);
//   }
//   // Check optional values
//   if (mcl->mweb->pWebServer->arg("mqttUser") != String(mqttUser))
//   { // Handle mqttUser
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttUser").toCharArray(mqttUser, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("mqttPassword") != String("***"))
//   { // Handle mqttPassword
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("mqttPassword").toCharArray(mqttPassword, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("configUser") != String(configUser))
//   { // Handle configUser
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("configUser").toCharArray(configUser, 32);
//   }
//   if (mcl->mweb->pWebServer->arg("configPassword") != String("****"))
//   { // Handle configPassword
//     shouldSaveConfig = true;
//     mcl->mweb->pWebServer->arg("configPassword").toCharArray(configPassword, 32);
//   }
// //   if (mcl->mweb->pWebServer->arg("motionPinConfig") != String(motionPinConfig))
// //   { // Handle motionPinConfig
// //     shouldSaveConfig = true;
// //     mcl->mweb->pWebServer->arg("motionPinConfig").toCharArray(motionPinConfig, 3);
// //   }
//   if ((mcl->mweb->pWebServer->arg("debugSerialEnabled") == String("on")) && !debugSerialEnabled)
//   { // debugSerialEnabled was disabled but should now be enabled
//     shouldSaveConfig = true;
//     debugSerialEnabled = true;
//   }
//   else if ((mcl->mweb->pWebServer->arg("debugSerialEnabled") == String("")) && debugSerialEnabled)
//   { // debugSerialEnabled was enabled but should now be disabled
//     shouldSaveConfig = true;
//     debugSerialEnabled = false;
//   }
// //   if ((mcl->mweb->pWebServer->arg("debugTelnetEnabled") == String("on")) && !debugTelnetEnabled)
// //   { // debugTelnetEnabled was disabled but should now be enabled
// //     shouldSaveConfig = true;
// //     debugTelnetEnabled = true;
// //   }
// //   else if ((mcl->mweb->pWebServer->arg("debugTelnetEnabled") == String("")) && debugTelnetEnabled)
// //   { // debugTelnetEnabled was enabled but should now be disabled
// //     shouldSaveConfig = true;
// //     debugTelnetEnabled = false;
// //   }
//   if ((mcl->mweb->pWebServer->arg("mdnsEnabled") == String("on")) && !mdnsEnabled)
//   { // mdnsEnabled was disabled but should now be enabled
//     shouldSaveConfig = true;
//     mdnsEnabled = true;
//   }
//   else if ((mcl->mweb->pWebServer->arg("mdnsEnabled") == String("")) && mdnsEnabled)
//   { // mdnsEnabled was enabled but should now be disabled
//     shouldSaveConfig = true;
//     mdnsEnabled = false;
//   }

//   if (shouldSaveConfig)
//   { // Config updated, notify user and trigger write to SPIFFS
//     httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String(nextionNode) + String(F("</h1>"));
//     httpMessage += String(F("<br/>Saving updated configuration values and restarting device"));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

//     configSave();
//     if (shouldSaveWifi)
//     {
//       //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"CONFIG: Attempting connection to SSID: ")) + mcl->mweb->pWebServer->arg("wifiSSID"));
//       espWifiSetup();
//     }
//     espReset();
//   }
//   else
//   { // No change found, notify user and link back to config page
//     httpMessage += String(F("<meta http-equiv='refresh' content='3;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String(nextionNode) + String(F("</h1>"));
//     httpMessage += String(F("<br/>No changes found, returning to <a href='/'>home page</a>"));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
//   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleResetConfig()
{ // http://plate01/resetConfig
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleResetConfig"));
  //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /resetConfig page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // //httpMessage.replace("{v}", String("nextionNode"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);

  // if (mcl->mweb->pWebServer->arg("confirm") == "yes")
  // { // User has confirmed, so reset everything
  //   httpMessage += String(F("<h1>"));
  //   httpMessage += String("nextionNode");
  //   httpMessage += String(F("</h1><b>Resetting all saved settings and restarting device into WiFi AP mode</b>"));
  //   httpMessage += FPSTR(HTTP_END2);
  //   mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  //   delay(1000);
  //   configClearSaved();
  // }
  // else
  // {
  //   httpMessage += String(F("<h1>Warning</h1><b>This process will reset all settings to the default values and restart the device.  You may need to connect to the WiFi AP displayed on the panel to re-configure the device before accessing it again."));
  //   httpMessage += String(F("<br/><hr><br/><form method='get' action='resetConfig'>"));
  //   httpMessage += String(F("<br/><br/><button type='submit' name='confirm' value='yes'>reset all settings</button></form>"));
  //   httpMessage += String(F("<br/><hr><br/><form method='get' action='/'>"));
  //   httpMessage += String(F("<button type='submit'>return home</button></form>"));
  //   httpMessage += FPSTR(HTTP_END2);
  //   mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNextionFirmware()
{ 
//   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNextionFirmware"));
  
// const char HTTP_FORM_UPG2[] =
//   "<div id='f1' name='f1' style='display:block;'>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
//   "<form method='get' action='u10'>"
//   "<br/><b>" D_OTA_URL "</b><br/><input id='o' name='o' placeholder='OTA_URL' value='%s'><br/>"
//   "<br/><button type='submit'>" D_START_UPGRADE "</button></form>"
//   "</fieldset><br/><br/>"
//   "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";

// const char HTTP_FORM_RST_UPG2[] =
//   "<form method='post' action='u20' enctype='multipart/form-data'>"
//   "<br/><input type='file' name='u20'><br/>"
//   "<br/><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
//   "</fieldset>"
//   "</div>";

//   mcl->mweb->WSStartAppend_P("Firmware Upgrade");
//   mcl->mweb->WSContentSendStyle();
//   mcl->mweb->WSBufferAppend_P(HTTP_FORM_UPG2, mcl->mset->Settings.ota_url);
//   mcl->mweb->WSBufferAppend_P(HTTP_FORM_RST_UPG2, D_UPGRADE);
//   mcl->mweb->WSContentSpaceButton(BUTTON_MAIN);
//   mcl->mweb->WSContentStop();

}


void mNextionPanel::webHandleNextionFirmware_PhaseOut()
{ 
  
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "void mNextionPanel::webHandleNextionFirmware_PhaseOut()"));
  // // http://plate01/firmware
  // // if (configPassword[0] != '\0')
  // // { //Request HTTP auth if configPassword is set
  // //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  // //   {
  // //     return mcl->mweb->pWebServer->requestAuthentication();
  // //   }
  // // }
  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /firmware page to client connected from: "))) ;//mcl->mweb->pWebServer->client().remoteIP().toString());

  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleNextionFirmware"));
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // //httpMessage.replace("{v}", (String("nextionNode") + " update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" firmware</h1>"));

  // // Display main firmware page
  // // HTTPS Disabled pending resolution of issue: https://github.com/esp8266/Arduino/issues/4696
  // // Until then, using a proxy host at http://haswitchplate.com to deliver unsecured firmware images from GitHub
  // httpMessage += String(F("<form method='get' action='/espfirmware'>"));
  // if (updateEspAvailable)
  // {
  //   httpMessage += String(F("<font color='green'><b>NEXTION ESP8266 update available!</b></font>"));
  // }
  // httpMessage += String(F("<br/><b>Update ESP8266 from URL</b><small><i> http only</i></small>"));
  // httpMessage += String(F("<br/><input id='espFirmwareURL' name='espFirmware' value='")) + espFirmwareUrl + "'>";
  // httpMessage += String(F("<br/><br/><button type='submit'>Update ESP from URL</button></form>"));

  // httpMessage += String(F("<br/><form method='POST' action='/update' enctype='multipart/form-data'>"));
  // httpMessage += String(F("<b>Update ESP8266 from file</b><input type='file' id='espSelect' name='espSelect' accept='.bin'>"));
  // httpMessage += String(F("<br/><br/><button type='submit' id='espUploadSubmit' onclick='ackEspUploadSubmit()'>Update ESP from file</button></form>"));

  // httpMessage += String(F("<br/><br/><hr><h1>WARNING!</h1>"));
  // httpMessage += String(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the NEXTION will need to be manually power cycled which might mean a trip to the breaker box. "));
  // httpMessage += String(F("After a power cycle, the LCD will display an error message until a successful firmware update has completed.<br/>"));

  // httpMessage += String(F("<br/><hr><form method='get' action='lcddownload'>"));
  // if (updateLcdAvailable)
  // {
  //   httpMessage += String(F("<font color='green'><b>NEXTION LCD update available!</b></font>"));
  // }
  // httpMessage += String(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  // httpMessage += String(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='")) + lcdFirmwareUrl + "'>";
  // httpMessage += String(F("<br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  // httpMessage += String(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  // httpMessage += String(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  // httpMessage += String(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

  // // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
  // httpMessage += String(F("<script>function handleLcdFileSelect(evt) {"));
  // httpMessage += String(F("var uploadFile = evt.target.files[0];"));
  // httpMessage += String(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
  // httpMessage += String(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
  // httpMessage += String(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
  // httpMessage += String(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
  // httpMessage += String(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
  // httpMessage += String(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
  // httpMessage += String(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
  // httpMessage += String(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  // httpMessage += FPSTR(HTTP_END2);

  // Serial.println(httpMessage);
  
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleEspFirmware()
{ 
  
  AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleEspFirmware"));
  // http://plate01/espfirmware
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }

  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /espfirmware page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " ESP update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='60;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>"));
  // httpMessage += String("nextionNode") + " ESP update";
  // httpMessage += String(F("</h1>"));
  // httpMessage += "<br/>Updating ESP firmware from: " + String(mcl->mweb->pWebServer->arg("espFirmware"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // //debugPrintln("ESPFW: Attempting ESP firmware update from: " + String(mcl->mweb->pWebServer->arg("espFirmware")));
  // espStartOta(mcl->mweb->pWebServer->arg("espFirmware"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpload()
{ // http://plate01/lcdupload
  // Upload firmware to the Nextion LCD via HTTP upload

//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "webHandleLcdUpload"));

//   // if (configPassword[0] != '\0')
//   // { //Request HTTP auth if configPassword is set
//   //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
//   //   {
//   //     return mcl->mweb->pWebServer->requestAuthentication();
//   //   }
//   // }

//   static uint32_t lcdOtaTransferred = 0;
//   static uint32_t lcdOtaRemaining;
//   static uint16_t lcdOtaParts;
//   const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
//   static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

//   HTTPUpload &upload = mcl->mweb->pWebServer->upload();

//   if (tftFileSize == 0)
//   {
    
//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "LCD OTA: FAILED, no filesize sent."));
  
//     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: FAILED, no filesize sent.")));
//     String httpMessage = FPSTR(HTTP_HEAD2);
//     // httpMessage.replace("{v}", (String("nextionNode") + " LCD update"));
//     httpMessage += FPSTR(HTTP_SCRIPT2);
//     httpMessage += FPSTR(HTTP_STYLE2);
//     httpMessage += String(NEXTION_STYLE);
//     httpMessage += String(F("<meta http-equiv='refresh' content='5;url=/' />"));
//     httpMessage += FPSTR(HTTP_HEAD_END2);
//     httpMessage += String(F("<h1>")) + String("nextionNode") + " LCD update FAILED</h1>";
//     httpMessage += String(F("No update file size reported.  You must use a modern browser with Javascript enabled."));
//     httpMessage += FPSTR(HTTP_END2);
//     mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
//   }
//   else if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
//   { // Our timer expired so reset
  
//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: LCD upload timeout.  Restarting.");
//    //debugPrintln(F("LCD OTA: ERROR: LCD upload timeout.  Restarting."));
//     //espReset();
//   }
//   else if (upload.status == UPLOAD_FILE_START)
//   {
//     //WiFiUDP::stopAll(); // Keep mDNS responder from breaking things

//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: LCD upload timeout.  Restarting.");
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Attempting firmware upload");
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%s"),"LCD OTA: upload.filename: ",upload.filename.c_str());
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d"),"LCD OTA: TFTfileSize: ",tftFileSize);

//     lcdOtaRemaining = tftFileSize;
//     lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: File upload beginning. Size ",lcdOtaRemaining," bytes in ",lcdOtaParts," 4k chunks.");

//     serial_print_suffix(); // Send empty command to LCD

//     nextionHandleInput();

//     String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + ",38400,0";
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%s"),"LCD OTA: Sending LCD upload command: ",lcdOtaNextionCmd.c_str());

//     char lcdOtaNextionCmd_ctr[100];
//     sprintf(lcdOtaNextionCmd_ctr,"whmi-wri %d,38400,0",tftFileSize);

//     serial_print_suffixed(lcdOtaNextionCmd_ctr);
    
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),lcdOtaNextionCmd_ctr);
    
//     if (nextionOtaResponse()){
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: LCD upload command accepted");
//     }else{
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: LCD upload command FAILED.");
//       espReset();
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_WRITE)
//   { // Handle upload data
//     static int lcdOtaChunkCounter = 0;
//     static uint16_t lcdOtaPartNum = 0;
//     static int lcdOtaPercentComplete = 0;
//     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
//     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
//     uint16_t lcdOtaUploadIndex = 0;
//     int32_t lcdOtaPacketRemaining = upload.currentSize;

//     while (lcdOtaPacketRemaining > 0)
//     { // Write incoming data to panel as it arrives
//       // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
//       uint16_t lcdOtaChunkSize = 0;
//       if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter))){
//         lcdOtaChunkSize = lcdOtaPacketRemaining;
//       }else 
//       if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter))){
//         lcdOtaChunkSize = lcdOtaBufferSize;
//       }else
//       {
//         lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
//       }

//       for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
//       { // Load up the UART buffer
//         lcdOtaBuffer[i] = upload.buf[lcdOtaUploadIndex];
//         lcdOtaUploadIndex++;
//       }

//       //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "len=%d"),lcdOtaChunkSize);

//       #ifdef USE_NEXTION_SOFTWARE_SERIAL
//         swSer->flush();                              // Clear out current UART buffer
//         swSer->write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
//       #else
//         SERIAL_NEXTION_TX.flush();                              // Clear out current UART buffer
//         SERIAL_NEXTION_TX.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
//       #endif

//       lcdOtaChunkCounter += lcdOtaChunkSize;
//       lcdOtaTransferred += lcdOtaChunkSize;
//       if (lcdOtaChunkCounter >= 4096)
//       {
//   //#define USE_NEXTION_SOFTWARE_SERIAL
// #ifdef USE_NEXTION_SOFTWARE_SERIAL
//         swSer->flush();
//         #else
//         SERIAL_NEXTION_TX.flush();
//         #endif


//         lcdOtaPartNum++;
//         lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
//         lcdOtaChunkCounter = 0;
//         if (nextionOtaResponse())
//         {
//           AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: Part ",lcdOtaPartNum," OK, ",lcdOtaPercentComplete,"% complete");
//         }
//         else
//         {
//           AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%d%s%d%s"),"LCD OTA: Part ",lcdOtaPartNum," FAILED, ",lcdOtaPercentComplete,"% complete");
//           AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s%"),"EXIT EARLY?");

//         }
//       }
//       else
//       {
//         delay(10);
//       }
//       if (lcdOtaRemaining > 0)
//       {
//         lcdOtaRemaining -= lcdOtaChunkSize;
//       }
//       if (lcdOtaPacketRemaining > 0)
//       {
//         lcdOtaPacketRemaining -= lcdOtaChunkSize;
//       }
//     }

//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       {
//         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaSuccess");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//       else
//       {
//         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_END)
//   { // Upload completed
//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       { // YAY WE DID IT
//         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaSuccess");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//       else
//       {
//         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//         mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//         mcl->mweb->pWebServer->send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           mcl->mweb->pWebServer->handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//     }
//   }
//   else if (upload.status == UPLOAD_FILE_ABORTED)
//   { // Something went kablooey
//     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
//     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//     mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//     mcl->mweb->pWebServer->send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       mcl->mweb->pWebServer->handleClient();
//       delay(1);
//     }
//     espReset();
//   }
//   else
//   { // Something went weird, we should never get here...
//     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: upload.status returned: ")) + String(upload.status));
//     //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Failure"));
//     mcl->mweb->pWebServer->sendHeader("Location", "/lcdOtaFailure");
//     mcl->mweb->pWebServer->send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       mcl->mweb->pWebServer->handleClient();
//       delay(1);
//     }
//     espReset();
//   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateSuccess()
{ // http://plate01/lcdOtaSuccess
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // // }
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdUpdateSuccess"));
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcdOtaSuccess page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update success"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" LCD update success</h1>"));
  // httpMessage += String(F("Restarting HASwitchPlate to apply changes..."));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateFailure()
{ // http://plate01/lcdOtaFailure
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdUpdateFailure"));
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcdOtaFailure page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update failed"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='15;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F(" LCD update failed :(</h1>"));
  // httpMessage += String(F("Restarting HASwitchPlate to reset device..."));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "httpMessage=%s"),httpMessage.c_str());

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdDownload()
{ // http://plate01/lcddownload
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleLcdDownload"));
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /lcddownload page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " LCD update"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>"));
  // httpMessage += String("nextionNode") + " LCD update";
  // httpMessage += String(F("</h1>"));
  // httpMessage += "<br/>Updating LCD firmware from: " + String(mcl->mweb->pWebServer->arg("lcdFirmware"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);

  // nextionStartOtaDownload(mcl->mweb->pWebServer->arg("lcdFirmware"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleTftFileSize()
{ // http://plate01/tftFileSize
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // }
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleTftFileSize"));
  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /tftFileSize page to client connected from: ")) + mcl->mweb->pWebServer->client().remoteIP().toString());

  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " TFT Filesize"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "%s"),httpMessage.c_str());
  // tftFileSize = mcl->mweb->pWebServer->arg("tftFileSize").toInt();
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "tftFileSize%d"),tftFileSize);


  // //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"WEB: tftFileSize: ")) + String(tftFileSize));



}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleReboot()
{ // http://plate01/reboot
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!mcl->mweb->pWebServer->authenticate(configUser, configPassword))
  //   {
  //     return mcl->mweb->pWebServer->requestAuthentication();
  //   }
  // // }
  // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "webHandleReboot"));
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HTTP: Sending /reboot page to client connected from: ");//) + mcl->mweb->pWebServer->client().remoteIP().toString());
  // String httpMessage = FPSTR(HTTP_HEAD2);
  // // httpMessage.replace("{v}", (String("nextionNode") + " NEXTION reboot"));
  // httpMessage += FPSTR(HTTP_SCRIPT2);
  // httpMessage += FPSTR(HTTP_STYLE2);
  // httpMessage += String(NEXTION_STYLE);
  // httpMessage += String(F("<meta http-equiv='refresh' content='10;url=/' />"));
  // httpMessage += FPSTR(HTTP_HEAD_END2);
  // httpMessage += String(F("<h1>")) + String("nextionNode") + String(F("</h1>"));
  // httpMessage += String(F("<br/>Rebooting device"));
  // httpMessage += FPSTR(HTTP_END2);
  // mcl->mweb->pWebServer->send(200, "text/html", httpMessage);
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"RESET: Rebooting device");
  // nextionSendCmd("page 0");
  // nextionSetAttr("p[0].b[1].txt", "\"Rebooting...\"");
  // // espReset();
}


// String mNextionPanel::debugPrintln(String str){
//   Serial.println(str);
// }



void mNextionPanel::HandleUpgradeFirmwareStart(void)
{
  // Upload via URL


  // //if (!HttpCheckPriviledgedAccess()) { return; }

  // char command[sizeof(mcl->mset->Settings.ota_url) + 10];  // OtaUrl

  // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  // mcl->mwif->WifiConfigCounter();

  // char otaurl[sizeof(mcl->mset->Settings.ota_url)];
  // WebGetArg(request,"o", otaurl, sizeof(otaurl));
  // if (strlen(otaurl)) {
  //   snprintf_P(command, sizeof(command), PSTR(D_JSON_OTAURL " %s"), otaurl);
  //   ExecuteWebCommand(command, SRC_WEBGUI);
  // }

  // WSStartAppend_P(PM_INFORMATION);
  // WSBufferAppend_P(HTTP_SCRIPT_RELOAD_OTA);
  // WSContentSendStyle();
  // WSBufferAppend_P(PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
  // WSBufferAppend_P(HTTP_MSG_RSTRT);
  // WSContentSpaceButton(BUTTON_MAIN);
  // WSContentStop();

  // snprintf_P(command, sizeof(command), PSTR(D_JSON_UPGRADE " 1"));
  // ExecuteWebCommand(command, SRC_WEBGUI);

}

void mNextionPanel::HandleUploadDone(void)
{
//   if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];

  // mcl->mwif->WifiConfigCounter();
  // mcl->mset->restart_flag = 0;
  //MqttRetryCounter(0);

//   mcl->mwif->WSStartAppend_P(PM_INFORMATION);
//   if (!upload_error) {
//     mcl->mwif->WSBufferAppend_P(HTTP_SCRIPT_RELOAD_OTA);  // Refesh main web ui after OTA upgrade
//   }
//   mcl->mwif->WSContentSendStyle();
//   mcl->mwif->WSBufferAppend_P(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
//   if (upload_error) {
// //    WSBufferAppend_P(PSTR(COLOR_TEXT_WARNING "'>" D_FAILED "</font></b><br/><br/>"));
//     mcl->mwif->WSBufferAppend_P(PSTR("%06x'>" D_FAILED "</font></b><br/><br/>"), mcl->msup->WebColor(mcl->mset->COL_TEXT_WARNING));
// #ifdef USE_RF_FLASH
//     if (upload_error < 14) {
// #else
//     if (upload_error < 10) {
// #endif
//       mcl->msup->GetTextIndexed_P(error, sizeof(error), upload_error -1, kUploadErrors);
//     } else {
//       snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), upload_error);
//     }
//     mcl->mwif->WSBufferAppend_P(error);
//     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_UPLOAD ": %s"), error);
//     mcl->mset->stop_flash_rotate = mcl->mset->Settings.flag_system.stop_flash_rotate;
//   } else {
//     mcl->mwif->WSBufferAppend_P(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), mcl->msup->WebColor(mcl->mset->COL_TEXT_SUCCESS));
//     mcl->mwif->WSBufferAppend_P(HTTP_MSG_RSTRT);
//     mcl->mwif->ShowWebSource(SRC_WEBGUI);
//     mcl->mset->restart_flag = 2;  // Always restart to re-enable disabled features during update
//   }
//   //SettingsBufferFree();
//   mcl->mwif->WSBufferAppend_P(PSTR("</div><br/>"));
//   mcl->mwif->WSContentSpaceButton(BUTTON_MAIN);
//   mcl->mwif->WSContentStop();

}

void mNextionPanel::HandleUploadLoop(void)
{

    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_UPLOAD ": %s"), "HandleUploadLoop");

//   // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
//   bool _serialoutput = (LOG_LEVEL_DEBUG <= mcl->mset->seriallog_level);

//   if (HTTP_USER == webserver_state) { return; }
//   if (upload_error) {
    
//     #ifdef ESP8266
//       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
//     #endif
//     return;
//   }

//   HTTPUpload& upload = pWebServer->upload();

//   if (UPLOAD_FILE_START == upload.status) {
//     mcl->mset->restart_flag = 60;
//     if (0 == upload.filename.c_str()[0]) {
//       upload_error = 1;  // No file selected
//       return;
//     }
//     //SettingsSave(1);  // Free flash for upload
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
//     if (UPL_SETTINGS == upload_file_type) {
//       if (!mcl->mset->SettingsBufferAlloc()) {
//         upload_error = 2;  // Not enough space
//         return;
//       }
//     } else {
//      // MqttRetryCounter(60);
// #ifdef USE_EMULATION
//       //UdpDisconnect();
// #endif  // USE_EMULATION
// #ifdef USE_ARILUX_RF
//       //AriluxRfDisable();  // Prevent restart exception on Arilux Interrupt routine
// #endif  // USE_ARILUX_RF
//       //if (mcl->mset->Settings.flag_system.mqtt_enabled) MqttDisconnect();
//       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      
//           #ifdef ESP8266
//       if (!Update.begin(maxSketchSpace)) {         //start with max available size

// //        if (_serialoutput) Update.printError(Serial);
// //        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
// //          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
// //        }

//         upload_error = 2;  // Not enough space
//         return;
//       }
//       #endif
//     }
//     upload_progress_dot_count = 0;
//   } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
//     if (0 == upload.totalSize) {
//       if (UPL_SETTINGS == upload_file_type) {
//         config_block_count = 0;
//       }
//       else {
// // #ifdef USE_RF_FLASH
// //         if ((SONOFF_BRIDGE == mcl->mset->my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
// //           Update.end();              // End esp8266 update session
// //           upload_file_type = UPL_EFM8BB1;
// //
// //           upload_error = SnfBrUpdateInit();
// //           if (upload_error != 0) { return; }
// //         } else
// // #endif  // USE_RF_FLASH
//         {
//           if (upload.buf[0] != 0xE9) {
//             upload_error = 3;  // Magic byte is not 0xE9
//             return;
//           }
//           uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
//           #ifdef ESP8266
//           if(bin_flash_size > ESP.getFlashChipRealSize()) {
//             upload_error = 4;  // Program flash size is larger than real flash size
//             return;
//           }
//           #endif
// //          upload.buf[2] = 3;  // Force DOUT - ESP8285
//         }
//       }
//     }
//     if (UPL_SETTINGS == upload_file_type) {
//       if (!upload_error) {
//         if (upload.currentSize > (sizeof(mcl->mset->Settings) - (config_block_count * HTTP_UPLOAD_BUFLEN))) {
//           upload_error = 9;  // File too large
//           return;
//         }
//         memcpy(mcl->mset->settings_buffer + (config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
//         config_block_count++;
//       }
//     }
// // #ifdef USE_RF_FLASH
// //     else if (UPL_EFM8BB1 == upload_file_type) {
// //       if (efm8bb1_update != nullptr) {    // We have carry over data since last write, i. e. a start but not an end
// //         ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
// //         free(efm8bb1_update);
// //         efm8bb1_update = nullptr;
// //         if (result != 0) {
// //           upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid
// //           return;
// //         }
// //       }
// //       ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
// //       if (result < 0) {
// //         upload_error = abs(result);
// //         return;
// //       } else if (result > 0) {
// //         if ((size_t)result > upload.currentSize) {
// //           // Offset is larger than the buffer supplied, this should not happen
// //           upload_error = 9;  // File too large - Failed to decode RF firmware
// //           return;
// //         }
// //         // A remnant has been detected, allocate data for it plus a null termination byte
// //         size_t remnant_sz = upload.currentSize - result;
// //         efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
// //         if (efm8bb1_update == nullptr) {
// //           upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
// //           return;
// //         }
// //         memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
// //         // Add null termination at the end of of remnant buffer
// //         efm8bb1_update[remnant_sz] = '\0';
// //       }
// //     }
// // #endif  // USE_RF_FLASH
//     else {  // firmware
    
//           #ifdef ESP8266
//       if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
//         upload_error = 5;  // Upload buffer miscompare
//         return;
//       }
//       #endif
//       if (_serialoutput) {
//         Serial.printf(".");
//         upload_progress_dot_count++;
//         if (!(upload_progress_dot_count % 80)) { Serial.println(); }
//       }
//     }
//   } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
//     if (_serialoutput && (upload_progress_dot_count % 80)) {
//       Serial.println();
//     }
//     if (UPL_SETTINGS == upload_file_type) {
//       if (config_xor_on_set) {
//         for (uint16_t i = 2; i < sizeof(mcl->mset->Settings); i++) {
//           mcl->mset->settings_buffer[i] ^= (config_xor_on_set +i);
//         }
//       }
//       bool valid_settings = false;
//       unsigned long buffer_version = mcl->mset->settings_buffer[11] << 24 | mcl->mset->settings_buffer[10] << 16 | mcl->mset->settings_buffer[9] << 8 | mcl->mset->settings_buffer[8];
//       if (buffer_version > 0x06000000) {
//         uint16_t buffer_size = mcl->mset->settings_buffer[3] << 8 | mcl->mset->settings_buffer[2];
//         uint16_t buffer_crc = mcl->mset->settings_buffer[15] << 8 | mcl->mset->settings_buffer[14];
//         uint16_t crc = 0;
//         for (uint16_t i = 0; i < buffer_size; i++) {
//           if ((i < 14) || (i > 15)) { crc += mcl->mset->settings_buffer[i]*(i+1); }  // Skip crc
//         }
//         valid_settings = (buffer_crc == crc);
//       } else {
//         valid_settings = (mcl->mset->settings_buffer[0] == CONFIG_FILE_SIGN);
//       }
//       if (valid_settings) {
//         //SystemSettings_DefaultBody();
//         memcpy((char*)&mcl->mset->Settings +16, mcl->mset->settings_buffer +16, sizeof(mcl->mset->Settings) -16);
//         mcl->mset->Settings.version = buffer_version;  // Restore version and auto upgrade after restart
//         //SettingsBufferFree();
//       } else {
//         upload_error = 8;  // File invalid
//         return;
//       }
//     }
// #ifdef USE_RF_FLASH
//     else if (UPL_EFM8BB1 == upload_file_type) {
//       // RF FW flash done
//       upload_file_type = UPL_TASMOTA;
//     }
// #endif  // USE_RF_FLASH
//     else {
//           #ifdef ESP8266
//       if (!Update.end(true)) { // true to set the size to the current progress
//         if (_serialoutput) { Update.printError(Serial); }
//         upload_error = 6;  // Upload failed. Enable logging 3
//         return;
//       }
//       #endif
//     }
//     if (!upload_error) {
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
//     }
//   } else if (UPLOAD_FILE_ABORTED == upload.status) {
//     mcl->mset->restart_flag = 0;
//     //MqttRetryCounter(0);
//     upload_error = 7;  // Upload aborted
//           #ifdef ESP8266
//     if (UPL_TASMOTA == upload_file_type) { Update.end(); }
//     #endif
//   }
//   delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void mNextionPanel::HandlePreflightRequest(void)
{
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  // mcl->mwif->pWebServer->sendHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  // mcl->mwif->WSSend(200, CT_HTML, "");
}

#endif //#ifdef USE_MODULE_DISPLAYS_NEXTION