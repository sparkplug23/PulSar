
#ifndef _MBUTTON_H_
#define _MBUTTON_H_

#define D_UNIQUE_MODULE_SENSORS_BUTTONS_ID 120

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BUTTONS



#include "2_CoreSystem/Time/mTime.h"
#include "2_CoreSystem/Logging/mLogging.h"

#include "1_TaskerManager/mTaskerManager.h"


//#include "//2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/Time/mTime.h"

enum ButtonStates { BUTTON_PRESSED_ID, BUTTON_NOT_PRESSED_ID };

DEFINE_PGM_CTR(PM_WEB_HANDLE_DIV_NAME_BUTTON_TABLE_CTR) "button_table";

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mButtons :
  public mTaskerInterface
{
  public:
    mButtons(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    static const char* PM_MODULE_SENSORS_BUTTONS_CTR;
    static const char* PM_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BUTTONS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BUTTONS_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mButtons);
    };
    #endif

// /*********************************************************************************************\
//  * Button support
// \*********************************************************************************************/


    struct SETTINGS{
      uint8_t buttons_found = 0;
      uint8_t fEnableSensor = false;

    }settings;

    
bool ModuleEnabled();

#define MAX_KEYS 4                 // Max number of keys or buttons

// unsigned long button_debounce = 0;          // Button debounce timer

struct BUTTONS{
  uint16_t holdbutton =0;      // Timer for button hold
  uint8_t multiwindow = 0;      // Max time between button presses to record press count
  uint8_t multipress = 0;       // Number of button presses within multiwindow
  
  uint8_t lastbutton = BUTTON_NOT_PRESSED_ID;  // Last button states
  uint8_t active_state_value = false; //defualt active high

  /**
   * @note isactive will always signify active or not
   * */
  uint8_t  isactive     = false;
  uint8_t  state     = false;
  // uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  int8_t pin = -1; // -1 is not active

}buttons[MAX_KEYS];

uint8_t dual_hex_code = 0;                  // Sonoff dual input flag
uint8_t key_no_pullup = 0;                  // key no pullup flag (1 = no pullup)
uint8_t key_inverted = 0;                   // Key inverted flag (1 = inverted)
// uint8_t buttons_found = 0;                  // Number of buttons found flag

// timereached_t tsaved_button_debounce;
uint32_t tsaved_button_debounce;

uint16_t dual_button_code = 0;              // Sonoff dual received code

void ButtonPullupFlag(uint8_t button_bit);
void ButtonInvertFlag(uint8_t button_bit);
void Pre_Init(void);
uint8_t ButtonSerial(uint8_t serial_in_byte);
void ButtonHandler(void);
void ButtonLoop(void);

char* IsButtonActiveCtr(uint8_t id, char* buffer, uint8_t buflen);

    #ifdef USE_MODULE_NETWORK_WEBSERVER

void WebAppend_Root_Draw_Table();
void WebAppend_Root_Status_Table();

    #endif // USE_MODULE_NETWORK_WEBSERVER

bool IsButtonActive(uint8_t id);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mButtons> mqtthandler_settings_teleperiod;
    struct handler<mButtons> mqtthandler_sensor_ifchanged;
    struct handler<mButtons> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
    struct handler<mButtons>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

  //#endif


};



#endif

#endif  // _SONOFF_H_
//#endif