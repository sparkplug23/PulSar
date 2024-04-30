// #defines for my credentials and passwards to NOT sync to github.
// This file will override those found in "mSystemConfig"
#ifndef mUserConfigSecret_Example_H
#define mUserConfigSecret_Example_H

#include "2_CoreSystem/mGlobalMacros.h"
// #include "6_Lights/00_Interface/palettes.h"
#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM // is nothing, as "const" is enough in esp32 to push to progmem space
#endif
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"



// Wifi

#undef  STA_SSID1
#define STA_SSID1            "**********"                // [Ssid1] Wifi SSID
#undef  STA_PASS1
#define STA_PASS1            "**********"                // [Password1] Wifi password

#undef  STA_SSID2
#define STA_SSID2            "**********"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS2
#define STA_PASS2            "**********"                // [Password2] Optional alternate AP Wifi password

#undef  STA_SSID3
#define STA_SSID3            "**********"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS3
#define STA_PASS3            "**********"                // [Password2] Optional alternate AP Wifi password

// MQTT

// eg. `192,168,1,65`
#define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,65

// Belfast
#define LATITUDE               54.5973         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              5.9301          // [Longitude] Your location to be used with sunrise and sunset


/************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ****** CUSTOM USERS - For Deployment by others for testing ************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/

/**
 * Uncomment (by removing the double backslash `\\` for ONE line only) to select the active example
 * */
// #define DEVICE_CUSTOM_USER_01
// #define DEVICE_CUSTOM_USER_02


/************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ****** Configuring ************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/



#endif // mUserConfigSecret_Example