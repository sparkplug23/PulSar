
#ifndef _MBUTTON_H_
#define _MBUTTON_H_



#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_BUTTONS



#include "2_CoreSystem/Time/mTime.h"
#include "2_CoreSystem/Logging/mLogging.h"


#include <ArduinoJson.h>
#include "1_TaskerManager/mTaskerManager.h"


#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/mHardwareTemplates.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/Time/mTime.h"

enum ButtonStates { PRESSED, NOT_PRESSED };

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>

class mButtons{
  public:
    mButtons(){};
    
    int8_t Tasker(uint8_t function);
    void init(void);

// /*********************************************************************************************\
//  * Button support
// \*********************************************************************************************/

#define MAX_KEYS 4                 // Max number of keys or buttons

// unsigned long button_debounce = 0;          // Button debounce timer
uint16_t holdbutton[MAX_KEYS] = { 0 };      // Timer for button hold
uint16_t dual_button_code = 0;              // Sonoff dual received code


uint8_t lastbutton[MAX_KEYS] = { NOT_PRESSED, NOT_PRESSED, NOT_PRESSED, NOT_PRESSED };  // Last button states
uint8_t multiwindow[MAX_KEYS] = { 0 };      // Max time between button presses to record press count
uint8_t multipress[MAX_KEYS] = { 0 };       // Number of button presses within multiwindow

uint8_t dual_hex_code = 0;                  // Sonoff dual input flag
uint8_t key_no_pullup = 0;                  // key no pullup flag (1 = no pullup)
uint8_t key_inverted = 0;                   // Key inverted flag (1 = inverted)
uint8_t buttons_found = 0;                  // Number of buttons found flag

// timereached_t tsaved_button_debounce;
uint32_t tsaved_button_debounce;

void ButtonPullupFlag(uint8 button_bit);
void ButtonInvertFlag(uint8 button_bit);
void ButtonInit(void);
uint8_t ButtonSerial(uint8_t serial_in_byte);
void ButtonHandler(void);
void ButtonLoop(void);



void WebAppend_Root_Draw_Table();
void WebAppend_Root_Status_Table();
bool IsButtonActive(uint8_t id);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mButtons>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mButtons> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mButtons> mqtthandler_sensor_ifchanged;
    struct handler<mButtons> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif


};



#endif

#endif  // _SONOFF_H_
//#endif