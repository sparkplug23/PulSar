#pragma once

// #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI

#include <Arduino.h>

// #ifndef HEADER_GUARD_ADD_PAGES
// #define HEADER_GUARD_ADD_PAGES 

#include "webpages_generated/html_ui.h"
#ifdef WLED_ENABLE_SIMPLE_UI
  #include "webpages_generated/html_simple.h"
#endif
#include "webpages_generated/html_settings.h"
#include "webpages_generated/html_other.h"
#ifdef WLED_ENABLE_PIXART
  #include "webpages_generated/html_pixart.h"
#endif
#ifndef WLED_DISABLE_PXMAGIC
  #include "webpages_generated/html_pxmagic.h"
#endif
#include "webpages_generated/html_cpal.h"

// #endif // HEADER_GUARD_ADD_PAGES

const char JSON_mode_names[] PROGMEM = R"=====(["FX names moved"])=====";
const char JSON_palette_names[] PROGMEM = R"=====([
"Default Michael","* Random Cycle","* Color 1 this","* Colors 1&2","* Color Gradient","* Colors Only","Party","Cloud","Lava","Ocean",
"Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
"Pastel","Sunset 2","Beach","Vintage","Departure","Landscape","Beech","Sherbet","Hult","Hult 64",
"Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
"Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
"Aurora","Atlantica","C9 2","C9 New","Temperature","Aurora 2","Retro Clown","Candy","Toxy Reaf","Fairy Reaf",
"Semi Blue","Pink Candy","Red Reaf","Aqua Flash","Yelblu Hot","Lite Light","Red Flash","Blink Red","Red Shift","Red Tide",
"Candy2"
])=====";
#define PALETTE_NAMES_COUNT_WLED 64



// #endif // ENABLE_WEBSERVER_LIGHTING_WEBUI

