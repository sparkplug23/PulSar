
// #ifndef rgb_web_H
// #define rgb_web_H



// #ifdef USE_MODULE_NETWORK_WEBSERVER

// #include "stdint.h"


// // #include <ESPAsyncTCP.h>
// // //?#include <ESPAsyncWebServer.h>

// #ifdef ESP32
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #elif defined(ESP8266)
// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #endif

// #define WEB_HANDLE_JSON_LIVEPIXELS "/fetch/rgb_livepixels.json"

// #define D_WEB_HANDLE_RGB_CONTROLS "/rgb_controls" // change to animator_settings
// #define D_WEB_HANDLE_RGB_CONTROLS_PAGE "/PAGErgb_controls" // change to animator_settings
// DEFINE_PGM_CTR(PM_WEB_HANDLE_RGB_CONTROLS) D_WEB_HANDLE_RGB_CONTROLS;
// #define D_BUTTON_NAME_RGB_CONTROLS_CTR "Lights Settings"
// DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_CONTROLS_CTR) D_BUTTON_NAME_RGB_CONTROLS_CTR;

// #define D_WEB_HANDLE_PALETTE_EDITOR_CTR "/palette_editor"
// #define D_WEB_HANDLE_PALETTE_EDITOR_PAGE_CTR "/PAGEpalette_editor"
// DEFINE_PGM_CTR(PM_WEB_HANDLE_PALETTE_EDITOR_CTR) D_WEB_HANDLE_PALETTE_EDITOR_CTR;
// #define D_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR "Colour Palette Editor"
// DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR) D_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR;

// #define D_WEB_HANDLE_ANIMATION_MIXER_EDITOR_CTR "/animation_mixer_editor"
// DEFINE_PGM_CTR(PM_WEB_HANDLE_RGB_ANIMATION_MIXER_EDITOR_CTR) D_WEB_HANDLE_ANIMATION_MIXER_EDITOR_CTR;
// #define D_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR "Animation Mixer Editor"
// DEFINE_PGM_CTR(PM_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR) D_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR;


//   #define D_CLASSNAME_BUTTON_ANIMATIONENABLE_CTR "animation_state"
//   #define D_CLASSNAME_BUTTON_LIGHTPOWER_CTR "light_power_state"

//   #define WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR "rgb_pal"
//   #define WEB_HANDLE_RGB_COLOUR_MIXER_EDITOR "rgb_mix"
//   #define WEB_HANDLE_NEXTION_FIRMWARE "nextionfirmware"
//   #define D_COLOUR_PICKER "Colour Picker"




  
//   #define MAX_LIVE_LEDS 50

// // const char HTTP_FORM_RGB_SELECTOR1[] PROGMEM =
// //   "<fieldset><legend><b>&nbsp;" D_COLOUR_PICKER "&nbsp;</b>"
// //   "</legend><form method='get' action='" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR "'>";
// // const char HTTP_FORM_RGB_LOAD_PIXELS[] PROGMEM =
// //   "<button name='loadpixels' type='submit' class='buttonh bora'>" "Load Palette" "</button>";

// #define D_RGB_COLOUR_SELECTOR "Colour Palette Selector"

// #define D_RGB_CONTROLS "Lights Settings"

 
//   const char kTitle_ListHeadings_Mixer_Editor[] PROGMEM = 
//     "Pixels updated (%)" "|" 
//     "Pixel Order" "|" 
//     "Change Rate (secs)" "|" 
//     "Change Period (secs)" "|" 
//     "Transition Method" "|" 
//     "Mode" "|" 
//     "Palette" "|" 
//     "Flasher" "|" 
//     "Mixer (Running ID)";

//   const char kTitle_TableTitles_Root[] PROGMEM = 
//     "Animation Status" "|" 
//     "Lights Auto Off Timer" "|" 
//     "Update Rate/Speed" "|" 
//     "Update Amount" "|" 
//     "Pattern" "|" 
//     "Mode" "|" 
//     "Flasher Function" "|"
//     "Lighting Power" ;

// #define WEB_HANDLER_SCRIPT_RGBLIGHTSETTINGS_DATA_FETCHER "/script/rgblightsettings_data_fetcher_functions.js"

// /*****
//  * 
//  *  Page Name: Palette Editor
//  * 
//  * ******/
// #define WEB_HANDLER_HTTP_SCRIPT_MINIMAL_FETCH_RGBLIGHTSETTINGS "/script/minimal_rgblightsettings.js"
// #define WEB_HANDLER_SCRIPT_RGBLIGHTSETTINGS_FORM "/script/rgblightsettings.js"

// DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_LIST_START)
//   "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
//   "<form method='get' action='rgb_controls'>"
//   "<p></p>";

// DEFINE_PGM_CTR(PM_HTTP_FORM_SELECTOR_LIST_START2)
//   "<fieldset><legend><b>&nbsp;" "Animation Controls" "&nbsp;</b></legend>"
//   // "<form method='get' action='PAGErgb_controls'>"
//   "<form onsubmit='alert('submit!');return false'>"
//   "<p></p>";

// DEFINE_PGM_CTR(PM_HTTP_FORM_START_VARIABLE)
//   "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>" //title
//   "<form method='get' action='%s'>" //action
//   "<p></p>";

// // const char kTitle_ListHeadings[] PROGMEM = 
// //     "Pixels updated (%%)" "|" 
// //     "Pixel Order" "|" 
// //     "Change Rate (secs)" "|" 
// //     "Change Period (secs)" "|" 
// //     "Transition Method" "|" 
// //     "Mode" "|" 
// //     "Palette" "|" 
// //     "Flasher" "|" 
// //     "Mixer (Running ID)";

//     #define WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR "pal_user_name"
    
// #endif


// #endif // USE_MODULE_NETWORK_WEBSERVER