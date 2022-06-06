
#ifndef _MBUTTON_H_
#define _MBUTTON_H_

#define D_UNIQUE_MODULE_SENSORS_BUTTONS_ID ((5*1000)+02)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BUTTONS



#include "2_CoreSystem/02_Time/mTime.h"
#include "2_CoreSystem/05_Logging/mLogging.h"

#include "1_TaskerManager/mTaskerManager.h"


//#include "//2_CoreSystem/11_Languages/mLanguage.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/02_Time/mTime.h"

enum ButtonStates { 
  BUTTON_PRESSED_ID, 
  BUTTON_NOT_PRESSED_ID };





// BUTTON_HOLD_PRESSED_ID

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
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BUTTONS_ID; }
    
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


#ifndef MAX_KEYS
#define MAX_KEYS 8                 // Max number of keys or buttons
#endif // MAX_KEYS

uint8_t GetHardwareSpecificPullMethod(uint8_t real_pin);

// unsigned long button_debounce = 0;          // Button debounce timer

struct BUTTONS{
  uint16_t hold_timer =0;      // Timer for button hold
  uint8_t window_timer = 0;//multiwindow = 0;      // Max time between button presses to record press count
  uint8_t press_counter = 0;//multipress = 0;       // Number of button presses within multiwindow
  
  // uint8_t lastbutton_active_state = BUTTON_NOT_PRESSED_ID;  // Last button states
  uint8_t last_state = BUTTON_NOT_PRESSED_ID;  // Last button states


  uint8_t active_state_value = false; //defualt active high

  /**
   * @note isactive will always signify active or not
   * */
  uint8_t  isactive     = false;
  uint8_t  state     = BUTTON_NOT_PRESSED_ID;
  // uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  int8_t pin = -1; // -1 is not active

}buttons[MAX_KEYS];

/**
 * @brief Need button event, so its stored as last event
 * Single/Multi/Hold
 * 
 * If the event is reported BEFORE rules_event can clear, then it can be used in the json mqtt message
 * 
 * 
 **/
// struct BUTTON_EVENT{
//   uint8_t type_id = INPUT_TYPE_LENGTH_ID;
//   uint8_t device_id = 0;
//   uint8_t state = 0;
//   uint8_t count = 0;
// }last_event;


uint8_t dual_hex_code = 0;                  // Sonoff dual input flag
uint8_t key_no_pullup = 0x00;                  // key no pullup flag (1 = no pullup)
uint8_t key_inverted = 0x00; // Must be set to 0, 8 bits wide                   // Key inverted flag (1 = inverted)
// uint8_t buttons_found = 0;                  // Number of buttons found flag

// timereached_t tsaved_button_debounce;
uint32_t tsaved_button_debounce;

uint16_t dual_button_code = 0;              // Sonoff dual received code

void SetPullupFlag(uint8_t button_bit);
void SetInvertFlag(uint8_t button_bit);
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
  
  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
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
  #endif // USE_MODULE_NETWORK_MQTT


};



#endif

#endif  // _SONOFF_H_
//#endif