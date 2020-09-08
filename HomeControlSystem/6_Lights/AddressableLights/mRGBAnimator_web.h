
#ifndef rgb_web_H
#define rgb_web_H

// just contains webpage related progmem for rgb

#include "stdint.h"


#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// const char STYLE_RGB_ROOT[] PROGMEM = 
//   //width inherited from div
//   ".palette{"
//     "background:black;"
//     // "width:100%;"
//     "height:%dpx;"
//   "}"
//   ".palette_title{"
//     "background:black;"
//     // "width:100%;"
//     "height:%dpx;"
//   "}"
//   ".palette{"
//     "padding:0px;"
//     "display:inline-block;"
//   "}"
//   // ".rgb_palb{"
//   //   "padding:0px;"
//   //   "display:inline-block;"
//   //   "height:%dpx;"
//   // "}"
//   ".rgb_pal{"
//     // "background:black;"
//     //"filter: brightness(175%);"
//     "width:100%%;"
//     "height:%dpx;"
//       // "position: absolute;"
//   "}"  
// //   ".rgb_table {"
// //   "padding: 2px;"
// //   // "display: inline-block;"
// // "}"
//   // ".module_button_con{"
//   //   "padding:0px;"
//   //   //"width:100%;"
//   // "}"
// ;

  #define WEB_HANDLE_JSON_LIVEPIXELS "/fetch/rgb_livepixels.json"

  // Class/span names


#define D_WEB_HANDLE_RGB_CONTROLS "/rgb_controls" // change to animator_settings
#define D_WEB_HANDLE_RGB_CONTROLS_PAGE "/PAGErgb_controls" // change to animator_settings
DEFINE_PGM_CTR(PM_WEB_HANDLE_RGB_CONTROLS) D_WEB_HANDLE_RGB_CONTROLS;
#define D_BUTTON_NAME_RGB_CONTROLS_CTR "Lights Settings"
DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_CONTROLS_CTR) D_BUTTON_NAME_RGB_CONTROLS_CTR;

#define D_WEB_HANDLE_PALETTE_EDITOR_CTR "/palette_editor"
#define D_WEB_HANDLE_PALETTE_EDITOR_PAGE_CTR "/PAGEpalette_editor"
DEFINE_PGM_CTR(PM_WEB_HANDLE_PALETTE_EDITOR_CTR) D_WEB_HANDLE_PALETTE_EDITOR_CTR;
#define D_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR "Colour Palette Editor"
DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR) D_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR;

#define D_WEB_HANDLE_ANIMATION_MIXER_EDITOR_CTR "/animation_mixer_editor"
DEFINE_PGM_CTR(PM_WEB_HANDLE_RGB_ANIMATION_MIXER_EDITOR_CTR) D_WEB_HANDLE_ANIMATION_MIXER_EDITOR_CTR;
#define D_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR "Animation Mixer Editor"
DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR) D_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR;



  #define WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR "rgb_pal"
  #define WEB_HANDLE_RGB_COLOUR_MIXER_EDITOR "rgb_mix"
  #define WEB_HANDLE_NEXTION_FIRMWARE "nextionfirmware"
  #define D_COLOUR_PICKER "Colour Picker"

  
  #define MAX_LIVE_LEDS 50

const char HTTP_FORM_RGB_SELECTOR1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_COLOUR_PICKER "&nbsp;</b>"
  "</legend><form method='get' action='" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR "'>";
const char HTTP_FORM_RGB_LOAD_PIXELS[] PROGMEM =
  "<button name='loadpixels' type='submit' class='button bora'>" "Load Palette" "</button>";

#define D_RGB_COLOUR_SELECTOR "Colour Palette Selector"

#define D_RGB_CONTROLS "Lights Settings"


// const char HTTP_SCRIPT_MODULE1c[] PROGMEM =
//   "function x1(a){"
//     "os=a.responseText;"
//     "sk(%d,99);"
//   "}"
//   "function x2(b){"
//     "os=b.responseText;";
// const char HTTP_SCRIPT_MODULE2c[] PROGMEM =
//   "}"
//   "function sl(){"
//     "ld('rgb_pal?m=1',x1);"                     // ?m related to pWebServer->hasParam("m")
//     "ld('rgb_pal?g=1',x2);"                     // ?m related to pWebServer->hasParam("m")
//   "}"
//   "window.onload=sl;";
// const char HTTP_FORM_RGB_COLOUR_SELECTOR[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" "Colour Palette Editor" "&nbsp;</b></legend>"
//   "<form method='get' action='rgb_pal'>"
//   "<p></p><b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";


// // Complete/parts | mixer_editor
// const char HTTP_FORM_COMPLETE_MIXER_EDITOR_SELECTOR[] PROGMEM =
//   "<form method='get' action='rgb_pal'>"
//   "<p></p><b>" "Select Mixer" "</b> (Selected: %s)<br/><select id='g99' name='g99'></select><br/>";


// // Main page "lb" divs
// const char HTTP_MODULE_TEMPLATE_REPLACE3[] PROGMEM =
//   "}2%d'>%s}4";                       // }2 and }3 are used in below os.replace
// const char HTTP_MODULE_TEMPLATE_REPLACE3_NUM[] PROGMEM =
//   "}2%d'>%d}4";                       // }2 and }3 are used in below os.replace


// const char PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR[] PROGMEM =
//   "{o1}%d|%s{o2}";   

//delete
// const char HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR[] PROGMEM =
//   "{o1}%d'>%s{o2}";   
// const char HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM[] PROGMEM =
//   "{o1}%d'>%d{o2}";

DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR)  
  "{o1}%d'>%s{o2}";   
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM)
  "{o1}%d'>%d{o2}";




DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_NUM)  
  "{o1}%d'>%d{o2}";  
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_CTR)  
  "{o1}%d'>%s{o2}";   
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR)  
  "{o1}%s'>%s{o2}";   





// //<option value="0" style="color: #FF0000">Red (0)</option>
// //"var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\");"
// //}2 = \"<option value='\"
// //}3 = \")</option>\"
// const char HTTP_MODULE_TEMPLATE_REPLACE2[] PROGMEM =
// // #ifdef USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
// //   "}2%d' style='color: #%06x'>%s (%d,%d}3";                       // }2 and }3 are used in below os.replace
// // #else
// //   "}2%d'style='color:#%06x'>%s}4";                       // }2 and }3 are used in below os.replace
// // / #else
// // {sc} style colour 'style='color:#

//   "}2%d{sc}%06x'>%s}4";                       // }2 and }3 are used in below os.replace
// // #endif


// // const char HTTP_SCRIPT_MODULE_TEMPLATE_MIXER_EDITOR[] PROGMEM =
// //   "var os;"
// //   "function sk(s,g){"                     // s = value, g = id and name
// //     "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
// //     "eb('g'+g).innerHTML=o;"
// //     "eb('g'+g).value=s;"
// //   "}"
// //   "function ld(u,f){"
// //     "var x=new XMLHttpRequest();"
// //     "x.onreadystatechange=function(){"
// //       "if(this.readyState==4&&this.status==200){"
// //         "f(this);"
// //       "}"
// //     "};"
// //     "x.open('GET',u,true);"
// //     "x.send();"
// //   "}";
// // const char HTTP_SCRIPT_MODULE1_MIXER_EDITOR[] PROGMEM =
// //   "function x0(a){"
// //     "os=a.responseText;"
// //     "sk(%d,0);"
// //   "}"
// //   "function x1(a){"
// //     "os=a.responseText;"
// //     "sk(%d,1);"
// //   "}"
// //   "function x2(a){"
// //     "os=a.responseText;"
// //     "sk(%d,2);"
// //   "}"
// //   "function x3(a){"
// //     "os=a.responseText;"
// //     "sk(%d,3);"
// //   "}"
// //   "function x4(a){"
// //     "os=a.responseText;"
// //     "sk(%d,4);"
// //   "}"
// //   "function x5(a){"
// //     "os=a.responseText;"
// //     "sk(%d,5);"
// //   "}"
// //   "function x6(a){"
// //     "os=a.responseText;"
// //     "sk(%d,6);"
// //   "}"
// //   "function x7(a){"
// //     "os=a.responseText;"
// //     "sk(%d,7);"
// //   "}"
// //   "function x8(a){"
// //     "os=a.responseText;"
// //     "sk(%d,8);"
// //   "}"
// //   ;
// // const char HTTP_SCRIPT_MODULE2c_MIXER_EDITOR[] PROGMEM =
// //   "function sl(){"
// //     "ld('rgb_controls?up=1',x0);" // up = update percentage
// //     "ld('rgb_controls?po=1',x1);" // po = pixel order
// //     "ld('rgb_controls?ra=1',x2);" // ra = rate
// //     "ld('rgb_controls?pr=1',x3);" // pr = period time
// //     "ld('rgb_controls?tm=1',x4);" // tm = transition mode
// //     "ld('rgb_controls?am=1',x5);" // am = animaiton mode
// //     "ld('rgb_controls?pa=1',x6);" // pa  = palette
// //     "ld('rgb_controls?fl=1',x7);" // fl  = flasher
// //     "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id
// //   "}"
// //   "window.onload=sl;";
// const char HTTP_FORM_RGB_COLOUR_SELECTOR_MIXER_EDITOR[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
//   "<form method='get' action='rgb_controls'>"
//   "<p></p>"; //space
//   //"<b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";


  // uint8_t 
  const char kTitle_ListHeadings_Mixer_Editor[] PROGMEM = 
    "Pixels updated (%)" "|" 
    "Pixel Order" "|" 
    "Change Rate (secs)" "|" 
    "Change Period (secs)" "|" 
    "Transition Method" "|" 
    "Mode" "|" 
    "Palette" "|" 
    "Flasher" "|" 
    "Mixer (Running ID)";

  const char kTitle_TableTitles_Root[] PROGMEM = 
    "Animation Status" "|" 
    "Update Rate/Speed" "|" 
    "Update Amount" "|" 
    "Pattern" "|" 
    "Mode" "|" 
    "Flasher Function" "|"
    "Lighting Power" "|" 
    "Lights Auto Off Timer";


#define WEB_HANDLER_SCRIPT_RGBLIGHTSETTINGS_DATA_FETCHER "/script/rgblightsettings_data_fetcher_functions.js"
// const char SCRIPT_WEB_DATA_FETCHER[] PROGMEM = 



/*****
 * 
 *  Page Name: Palette Editor
 * 
 * ******/
#define WEB_HANDLER_HTTP_SCRIPT_MINIMAL_FETCH_RGBLIGHTSETTINGS "/script/minimal_rgblightsettings.js"
#define WEB_HANDLER_SCRIPT_RGBLIGHTSETTINGS_FORM "/script/rgblightsettings.js"
// const char HTTP_SCRIPT_MODULE_TEMPLATEb[] PROGMEM =
//   "var os;"
//   "function sk(s,g){"                     // s = value, g = id and name
//     "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
//     "eb('g'+g).innerHTML=o;"
//     "eb('g'+g).value=s;"
//   "}"
//   "function ld(u,f){"
//     "var x=new XMLHttpRequest();"
//     "x.onreadystatechange=function(){"
//       "if(this.readyState==4&&this.status==200){"
//         "f(this);"
//       "}"
//     "};"
//     "x.open('GET',u,true);"
//     "x.send();"
//   "}"
//   // ;
// // const char HTTP_SCRIPT_MODULE1b[] PROGMEM =
// //   "function x0(a){"
// //     "os=a.responseText;"
// //     "sk(%d,0);"
// //   "}"
// //   "function x1(a){"
// //     "os=a.responseText;"
// //     "sk(%d,1);"
// //   "}"
// //   "function x2(a){"
// //     "os=a.responseText;"
// //     "sk(%d,2);"
// //   "}"
// //   "function x3(a){"
// //     "os=a.responseText;"
// //     "sk(%d,3);"
// //   "}"
// //   "function x4(a){"
// //     "os=a.responseText;"
// //     "sk(%d,4);"
// //   "}"
// //   "function x5(a){"
// //     "os=a.responseText;"
// //     "sk(%d,5);"
// //   "}"
// //   "function x6(a){"
// //     "os=a.responseText;"
// //     "sk(%d,6);"
// //   "}"
// //   "function x7(a){"
// //     "os=a.responseText;"
// //     "sk(%d,7);"
// //   "}"
// //   "function x8(a){"
// //     "os=a.responseText;"
// //     "sk(%d,8);"
// //   "}"
// //   // ;
// // // const char HTTP_SCRIPT_MODULE2cb[] PROGMEM =
// //   "function sl(){"
// //     "ld('rgb_controls?up=1',x0);" // up = update percentage
// //     "ld('rgb_controls?po=1',x1);" // po = pixel order
// //     "ld('rgb_controls?ra=1',x2);" // ra = rate
// //     "ld('rgb_controls?pr=1',x3);" // pr = period time
// //     "ld('rgb_controls?tm=1',x4);" // tm = transition mode
// //     "ld('rgb_controls?am=1',x5);" // am = animaiton mode
// //     "ld('rgb_controls?pa=1',x6);" // pa  = palette
// //     "ld('rgb_controls?fl=1',x7);" // fl  = flasher
// //     "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id
// //   "}"

//   "window.onload=sl;";


//   const char HTTP_SCRIPT_ROOT_PALETTE_SELECTOR[] PROGMEM =
//   "var os1;"
//   // "function sk(s,g){"                     // s = value, g = id and name
//   //   "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\")</option>\").replace(/}4/g,\"</option>\");"
//   //   "eb('g'+g).innerHTML=o;"
//   //   "eb('g'+g).value=s;"
//   // "}"
//   "function send_select(u,f){"
//     "var x=new XMLHttpRequest();"
//     "x.onreadystatechange=function(){"
//       "if(this.readyState==4&&this.status==200){"
//         "f(this);"
//       "}"
//     "};"
//     "x.open('GET',u,true);"
//     "x.send();"
//   "}"
//   "function x0(a){"
//     "os1=a.responseText;"
//     // "sk(%d,0);"
//     "console.log('os1',os1);"
//   "}"
  
// //    "fSelect('rgb_controls?up=1',x0);" // up = update percentage
// //   "}"
//   // "window.onload=sl;"
//   ;

// const char HTTP_FORM_RGB_COLOUR_SELECTORb[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
//   "<form method='HTTP_SCRIPT_ROOT_PALETTE_SELECTOR' action='rgb_controls'>"
//   "<p></p>"; //space
//   //"<b>" "Select Palette" "</b> (Active: %s)<br/><select id='g99' name='g99'></select><br/>";

DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_LIST_START)
  "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
  "<form method='get' action='rgb_controls'>"
  "<p></p>";


DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_LIST_START2)
  "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
  // "<form method='get' action='PAGErgb_controls'>"
  "<form onsubmit='alert('submit!');return false'>"
  "<p></p>";


DEFINE_PGM_CTR(PM_HTTP_FORM_START_VARIABLE)
  "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>" //title
  "<form method='get' action='%s'>" //action
  "<p></p>";



// const char kButtonConfirmb[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;

  const char kTitle_ListHeadings[] PROGMEM = 
    "Pixels updated (%%)" "|" 
    "Pixel Order" "|" 
    "Change Rate (secs)" "|" 
    "Change Period (secs)" "|" 
    "Transition Method" "|" 
    "Mode" "|" 
    "Palette" "|" 
    "Flasher" "|" 
    "Mixer (Running ID)";

    #define WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR "pal_user_name"
    
  // const char kTitle_ListSelectArg[] PROGMEM = 
  //   "g_up" "|" 
  //   "Pixel Order" "|" 
  //   "Change Rate (secs)" "|" 
  //   "Change Period (secs)" "|" 
  //   "Transition Method" "|" 
  //   "Mode" "|" 
  //   "Palette" "|" 
  //   "Flasher" "|" 
  //   "Mixer (Running ID)";






#endif



/*
// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <iostream>
#include "ArduinoJson.h"

void parsesub_Settings(JsonObject& obj){
  if(!obj["test1"].as<int>() != 0) {
     std::cout << "That's working!\n";
  }
}

void parsesub_Other(JsonObject& obj){
  if(obj["test2"].as<const char*>() != nullptr) { 
     std::cout << "This too!\n";
  }
}

void parsesub_CheckAll(JsonObject& obj){
  parsesub_Settings(obj); 
  parsesub_Other(obj); 
}

int main() {
  char buffer[] = "{\"test\":1,\"test2\":\"string\"}";
  DynamicJsonDocument doc(1500);
  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  parsesub_CheckAll(obj);
  return 0;
}
*/
/*
void parsesub_Settings(JsonObjectConst obj){
  if(obj["test1"].as<int>() != 0) {
     std::cout << "That's working!\n";
  }
}

void parsesub_Other(JsonObjectConst obj){
  if(obj["test2"].as<const char*>() != nullptr) { 
     std::cout << "This too!\n";
  }
}

void parsesub_CheckAll(JsonObjectConst obj){
  parsesub_Settings(obj); 
  parsesub_Other(obj); 
}

int main() {
  char buffer[] = "{\"test1\":1,\"test2\":\"string\"}";
  DynamicJsonDocument doc(1500);
  deserializeJson(doc, buffer);
  //JsonObject obj = doc.as<JsonObject>();
  parsesub_CheckAll(doc.as<JsonObjectConst>());
  return 0;
}

*/


