
#ifndef MPWMLIGHT_WEB_H
#define MPWMLIGHT_WEB_H

// just contains webpage related progmem for rgb

#include "stdint.h"


#ifdef USE_MODULE_CORE_WEBSERVER
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif //USE_MODULE_CORE_WEBSERVER


DEFINE_PGM_CTR(kTitle_TableTitles_Root_PWM)
  "Animation Status" "|" 
  "Lights Auto Off Timer";

#endif

