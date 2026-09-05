#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_SETTINGS_ID 2001 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mBaseConfig.h"

#include "DataBuffer.h"

// Easy way to add to the counter
#define D_MQTT_COMMAND_HANDLED_COUNT_INC data_buffer.isserviced++


#include "2_CoreSystem/05_Pins/mPins_Templates.h"
#include "2_CoreSystem/05_Pins/mPins_Esp32.h"
#include "2_CoreSystem/05_Pins/mPins_Esp82xx.h"
#include "2_CoreSystem/05_Pins/mPins.h"

#include "1_TaskerManager/mTaskerManager.h"

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include "JsonParser.h"
#include "1_TaskerManager/mTaskerInterface.h"
#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/mSystemConstants.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  // #include "avr/pgmspace.h"
  #if (defined(__AVR__))
  #include <avr\pgmspace.h>
  #else
  #include <pgmspace.h>
  #endif
#endif


/***
 * I likely want to remove any sensor/driver name here, and limit SettingsTxt to be for system related values
 * ie remove SET_SWITCH_TXT1
 */
enum SettingsTextIndex { 
    SET_OTAURL,
    SET_HOSTNAME, SET_SYSLOG_HOST,
    SET_WEBPWD, SET_CORS,
    SET_STATE_TXT1, SET_STATE_TXT2, SET_STATE_TXT3, SET_STATE_TXT4,  // MAX_STATE_TEXT
    SET_NTPSERVER1, SET_NTPSERVER2, SET_NTPSERVER3,  // MAX_NTP_SERVERS
    SET_MEM1, SET_MEM2, SET_MEM3, SET_MEM4, SET_MEM5, SET_MEM6, SET_MEM7, SET_MEM8,
    SET_MEM9, SET_MEM10, SET_MEM11, SET_MEM12, SET_MEM13, SET_MEM14, SET_MEM15, SET_MEM16,  // MAX_RULE_MEMS
    SET_FRIENDLYNAME1, SET_FRIENDLYNAME2, SET_FRIENDLYNAME3, SET_FRIENDLYNAME4,
    SET_FRIENDLYNAME5, SET_FRIENDLYNAME6, SET_FRIENDLYNAME7, SET_FRIENDLYNAME8,  // MAX_FRIENDLYNAMES
    #ifndef ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    SET_BUTTON1, SET_BUTTON2, SET_BUTTON3, SET_BUTTON4, SET_BUTTON5, SET_BUTTON6, SET_BUTTON7, SET_BUTTON8,
    SET_BUTTON9, SET_BUTTON10, SET_BUTTON11, SET_BUTTON12, SET_BUTTON13, SET_BUTTON14, SET_BUTTON15, SET_BUTTON16,  // MAX_BUTTON_TEXT
    #endif // ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    SET_TEMPLATE_NAME,
    SET_DEV_GROUP_NAME1, SET_DEV_GROUP_NAME2, SET_DEV_GROUP_NAME3, SET_DEV_GROUP_NAME4,  // MAX_DEV_GROUP_NAMES
    SET_DEVICENAME,
    SET_TELEGRAM_TOKEN, SET_TELEGRAM_CHATID,
    #ifndef ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    #ifdef ESP8266
      SET_ADC_PARAM1,
      SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
    #endif  // ESP8266
    #ifdef ESP32
      SET_ADC_PARAM1, SET_ADC_PARAM2, SET_ADC_PARAM3, SET_ADC_PARAM4, SET_ADC_PARAM5, SET_ADC_PARAM6, SET_ADC_PARAM7, SET_ADC_PARAM8,  // MAX_ADCS
      SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
      SET_SWITCH_TXT9, SET_SWITCH_TXT10, SET_SWITCH_TXT11, SET_SWITCH_TXT12, SET_SWITCH_TXT13, SET_SWITCH_TXT14, SET_SWITCH_TXT15, SET_SWITCH_TXT16,  // MAX_SWITCHES_TXT
      SET_SWITCH_TXT17, SET_SWITCH_TXT18, SET_SWITCH_TXT19, SET_SWITCH_TXT20, SET_SWITCH_TXT21, SET_SWITCH_TXT22, SET_SWITCH_TXT23, SET_SWITCH_TXT24,  // MAX_SWITCHES_TXT
      SET_SWITCH_TXT25, SET_SWITCH_TXT26, SET_SWITCH_TXT27, SET_SWITCH_TXT28,  // MAX_SWITCHES_TXT
    #endif  // ESP32
    #endif // ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    SET_SHD_PARAM,
    SET_RGX_SSID, SET_RGX_PASSWORD,
    SET_INFLUXDB_HOST, SET_INFLUXDB_PORT, SET_INFLUXDB_ORG, SET_INFLUXDB_TOKEN, SET_INFLUXDB_BUCKET, SET_INFLUXDB_RP,
    SET_CANVAS,
    SET_TELEGRAM_FINGERPRINT,
    SET_MAX, // limit of texts stored in Settings
    // Index above are not stored in Settings and should be handled specifically in SettingText()
    #ifndef ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    SET_BUTTON17, SET_BUTTON18, SET_BUTTON19, SET_BUTTON20, SET_BUTTON21, SET_BUTTON22, SET_BUTTON23, SET_BUTTON24,
    SET_BUTTON25, SET_BUTTON26, SET_BUTTON27, SET_BUTTON28, SET_BUTTON29, SET_BUTTON30, SET_BUTTON31, SET_BUTTON32,
    #endif // ENABLE_DEVFEATURE_REMOVE__UNDESIRED_SETTINGS_TEXT_OF_SUBMODULES
    SET_FINAL_MAX
   };


enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";


enum DATABUILDER_JSON_LEVEL{ //in order of importance
  JSON_LEVEL_NONE=0,
  // Only show what has changed over mqtt
  JSON_LEVEL_IFCHANGED,
  // Used to show in serial the most basic stuff anytime it is called (ie above IFCHANGED) but not as detailed as short would be 
  // Basic and short may be the same, so remove base
  JSON_LEVEL_SHORT,
  JSON_LEVEL_DETAILED,
  JSON_LEVEL_ALL,
  JSON_LEVEL_DEBUG // Share extra info relating to the generation of the json
};


DEFINE_PGM_CTR(PM_LEVEL_NONE_CTR)        "None";
DEFINE_PGM_CTR(PM_LEVEL_IFCHANGED_CTR)   "IfChanged";
DEFINE_PGM_CTR(PM_LEVEL_SHORT_CTR)       "Short";
DEFINE_PGM_CTR(PM_LEVEL_DETAILED_CTR)    "Detailed";
DEFINE_PGM_CTR(PM_LEVEL_ALL_CTR)         "All";
DEFINE_PGM_CTR(PM_LEVEL_DEBUG_CTR)       "Debug";
  

const uint32_t settings_text_size = 699;   // Settings->text_pool[size] = Settings->display_model (2D2) - Settings->text_pool (017)
const uint8_t MAX_TUYA_FUNCTIONS = 16;
const uint8_t PARAM8_SIZE = 18;            // Number of param bytes (SetOption)


#ifdef ESP8266
extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"
// extern "C" uint32_t _SPIFFS_end;
#endif

#include "2_CoreSystem/13_DeviceNameList/mDeviceNameList.h"

#ifdef ESP8266
  #if AUTOFLASHSIZE

  #else
    extern "C" uint32_t _FS_end;
  #endif  // AUTOFLASHSIZE
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mSettings :
  public mTaskerInterface
{
  public:
    mSettings(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_CORE_SETTINGS_CTR = D_MODULE__CORE__SETTINGS__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_SETTINGS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SETTINGS_ID; }
    ~mSettings() {          }

  #ifdef ESP8266
    #if AUTOFLASHSIZE
      #include "flash_hal.h"
      // From libraries/EEPROM/EEPROM.cpp EEPROMClass
      const uint32_t SPIFFS_END = (FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
    #else
      // extern "C" uint32_t _FS_end;
      // From libraries/EEPROM/EEPROM.cpp EEPROMClass
      const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
    #endif  // AUTOFLASHSIZE
    // Version 4.2 config = eeprom area
    const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area

    // Version 5.2 allow for more flash space
    const uint8_t CFG_ROTATES = 1;//8;          // Number of flash sectors used (handles uploads)

    uint32_t settings_location = SETTINGS_LOCATION;
    // uint32_t settings_crc = 0;
    uint32_t settings_crc32 = 0;
    uint8_t *settings_buffer = nullptr;
  #endif // ESP8266

  void JsonAppend_Settings();    
  int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
  void Function_Template_Load();
  void SettingsWrite(const void *pSettings, unsigned nSettingsLen);
  int16_t GetFunctionIDbyName(const char* c);
  void SettingsInit();
  void SetFlashModeDout(void);
  void SettingsBufferFree(void);
  bool SettingsBufferAlloc(void);
  uint32_t SettingsRead(void *data, size_t size);
  void SettingsSaveAll(void);
  uint32_t GetSettingsAddress(void);
  void SettingsSave(uint8_t rotate);
  void SettingsLoad(void);
  void SettingsDelta();
  void SettingsErase(uint8_t type);
  bool SettingsEraseConfig(void) ;
  void SettingsSdkErase(void);
  void SettingsDefault(void);
  void SystemSettings_DefaultHeader(void);
  void SystemSettings_DefaultBody(void);

  bool SaveSettings__LastKnownGood(void);
  bool LoadSettings__RestoreFrom_LastKnownGood(void);

  uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size);
  uint32_t GetSettingsCrc32(void);

  void SettingsResetStd(void);
  void SettingsResetDst(void);

  typedef union {
    uint8_t data;
    struct {
      uint8_t network_down : 1;
      uint8_t mqtt_down : 1;
      uint8_t wifi_down : 1;
      uint8_t eth_down : 1;
      uint8_t spare04 : 1;
      uint8_t spare05 : 1;
      uint8_t spare06 : 1;
      uint8_t spare07 : 1;
    };
  } StateBitfield;


  typedef union {
    uint32_t data;
    struct {
      uint32_t time : 11;                    // bits 0 - 10 = minutes in a day
      uint32_t window : 4;                   // bits 11 - 14 = minutes random window
      uint32_t repeat : 1;                   // bit 15
      uint32_t days : 7;                     // bits 16 - 22 = week day mask
      uint32_t device : 4;                   // bits 23 - 26 = 16 devices
      uint32_t power : 2;                    // bits 27 - 28 = 4 power states - Off, On, Toggle, Blink or Rule
      uint32_t mode : 2;                     // bits 29 - 30 = timer modes - 0 = Scheduler, 1 = Sunrise, 2 = Sunset
      uint32_t arm : 1;                      // bit 31
    };
  } Timer;

  /**
   * SECTION
   * Templates
   * - MODULE_TEMPLATE
   * - FUNCTION_TEMPLATE
   * - LIGHTING_TEMPLATE
   * - DISPLAY_TEMPLATE
   * 
   */

  // a flag status group showing how succesful the boot was
  // 3 state template, used, succuss, fail, none

  #define MODULE_TEMPLATE_MAX_SIZE 500
  enum TemplateSource
  {
    NONE,
    HEADER_CUSTOM,   // custom
    HEADER_TEMPLATE, // from a predefined template
    FILE,            // from a file (loading into internal storage with webui or mqtt command)
    FAILED_LOAD      // error occurs when loaded
  };
  struct TemplateLoading
  {
    struct STATUS{
      uint8_t module                = TemplateSource::NONE;
      uint8_t function              = TemplateSource::NONE;
      uint8_t lighting              = TemplateSource::NONE;
      uint8_t nextion_hmi_input_map = TemplateSource::NONE;
      uint8_t rules                 = TemplateSource::NONE;
    }status;

    // Other
  };  
 


struct Template_Config{
  uint8_t flags;
  uint8_t base;       
  mytmplt hardware;   
};


struct SystemName{ 
  char          friendly[50]; // Used in titles, set by templates "FriendlyName"
  char          device[50];   // USed as mqtt topic by default, set by templates "Name"
};
#ifdef DEVICENAME_CTR
// static_assert(sizeof(DEVICENAME_CTR) - 1 <= 50, "DEVICENAME_CTR exceeds max length");
#endif 


#define PARAM8_SIZE 18            // Number of param bytes (SetOption)







/*******************************************************************************************************************************************************************
********************************************************************************************************************************************************************
** SECITON: BitFields **********************************************************************************************************************************************
********************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/

/*******************************************************************************************************************************************************************
********************************************************************************************************************************************************************
** SECTION: System Options
**
** PulSar option model
** -----------------------------------------------------------------------------------------------------------------------------------------------------------------
** Each option group contains:
**
**   bit     = compact boolean flags, stored in a bitfield
**   params  = named numeric parameters, stored as explicit typed fields
**
** SetOption IDs are now only an API/command namespace.
** They do not define how the value is stored.
**
** Suggested ranges:
**   100..199 = System
**   200..299 = Network
**   300..399 = Drivers
**   400..499 = Sensors
**   500..599 = Lights
**   600..699 = Power / energy
**   700..799 = Rules / event layer
**
** Examples:
**   SetOption100 -> sysopt_system.bit.save_state
**   SetOption101 -> sysopt_system.params.boot_loop_offset
**   SetOption200 -> sysopt_network.bit.mdns_enabled
**   SetOption201 -> sysopt_network.params.mdns_delayed_start_s
**
********************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/


// ============================================================================
// System options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t save_state               : 1;  // SetOption100 - Save power/runtime state and restore after restart
    uint32_t mqtt_enabled             : 1;  // SetOption101 - Enable MQTT
    uint32_t mqtt_power_retain        : 1;  // SetOption102 - Use MQTT retain for power messages
    uint32_t mqtt_button_retain       : 1;  // SetOption103 - Use MQTT retain for button messages
    uint32_t mqtt_switch_retain       : 1;  // SetOption104 - Use MQTT retain for switch messages
    uint32_t button_single_press_only : 1;  // SetOption105 - Support only single press for faster button response
    uint32_t interlock                : 1;  // SetOption106 - Enable relay/power interlock
    uint32_t decimal_text             : 1;  // SetOption107 - Use decimal instead of hexadecimal text output
    uint32_t ex_cors_enabled          : 1;  // SetOption108 - Enable HTTP CORS
    uint32_t compatibility_check      : 1;  // SetOption109 - Enable/disable OTA compatibility check
    uint32_t gui_no_state_text        : 1;  // SetOption110 - Disable GUI state text display

    uint32_t timers_enable            : 1;  // bit 11
    uint32_t sleep_normal             : 1;  // bit 12
    uint32_t user_esp8285_enable      : 1;  // bit 13
    uint32_t reserved14               : 1;
    uint32_t reserved15               : 1;
    uint32_t reserved16               : 1;
    uint32_t reserved17               : 1;
    uint32_t reserved18               : 1;
    uint32_t reserved19               : 1;
    uint32_t reserved20               : 1;
    uint32_t reserved21               : 1;
    uint32_t reserved22               : 1;
    uint32_t reserved23               : 1;
    uint32_t reserved24               : 1;
    uint32_t reserved25               : 1;
    uint32_t reserved26               : 1;
    uint32_t reserved27               : 1;
    uint32_t reserved28               : 1;
    uint32_t reserved29               : 1;
    uint32_t reserved30               : 1;
    uint32_t reserved31               : 1;
  };
} SystemBitField__System;


struct SystemParams__System
{
  uint8_t  boot_loop_offset;       // SetOption150 - Fastboot restart count offset before recovery stages begin
  uint16_t backlog_delay_ms;       // SetOption152 - Minimum delay between backlog commands
};


struct SystemOptions__System
{
  SystemBitField__System bit;
  SystemParams__System   param;
};


// ============================================================================
// Network options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t mdns_enabled             : 1;  // SetOption200 - Enable mDNS service
    uint32_t use_wifi_scan            : 1;  // SetOption201 - Scan WiFi networks at restart
    uint32_t use_wifi_rescan          : 1;  // SetOption202 - Periodically rescan WiFi networks
    uint32_t sleep_normal             : 1;  // SetOption203 - Use normal sleep instead of dynamic sleep
    uint32_t fast_power_cycle_disable : 1;  // SetOption204 - Disable fast power-cycle reset detection
    uint32_t network_wifi             : 1;  // SetOption205 - Enable WiFi networking
    uint32_t network_ethernet         : 1;  // SetOption206 - Enable Ethernet networking
    uint32_t dns_ipv6_priority        : 1;  // SetOption207 - Prefer IPv6 DNS when available

    uint32_t reserved08               : 1;
    uint32_t reserved09               : 1;
    uint32_t reserved10               : 1;
    uint32_t reserved11               : 1;
    uint32_t reserved12               : 1;
    uint32_t reserved13               : 1;
    uint32_t reserved14               : 1;
    uint32_t reserved15               : 1;
    uint32_t reserved16               : 1;
    uint32_t reserved17               : 1;
    uint32_t reserved18               : 1;
    uint32_t reserved19               : 1;
    uint32_t reserved20               : 1;
    uint32_t reserved21               : 1;
    uint32_t reserved22               : 1;
    uint32_t reserved23               : 1;
    uint32_t reserved24               : 1;
    uint32_t reserved25               : 1;
    uint32_t reserved26               : 1;
    uint32_t reserved27               : 1;
    uint32_t reserved28               : 1;
    uint32_t reserved29               : 1;
    uint32_t reserved30               : 1;
    uint32_t reserved31               : 1;
  };
} SystemBitField__Network;


struct SystemParams__Network
{
  uint16_t mdns_delayed_start_s;      // SetOption250 - Delay before mDNS starts
  uint16_t gratuitous_arp_s;          // SetOption251 - Interval between gratuitous ARP requests
  uint16_t wifi_rescan_interval_min;  // SetOption252 - WiFi rescan interval
};


struct SystemOptions__Network
{
  SystemBitField__Network bit;
  SystemParams__Network   param;
};


// ============================================================================
// Driver / actuator options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t buzzer_freq_mode         : 1;  // SetOption300 - Enable alternate buzzer frequency mode
    uint32_t buzzer_enable            : 1;  // SetOption301 - Enable buzzer when available
    uint32_t use_esp32_temperature    : 1;  // SetOption302 - Enable ESP32 internal temperature reporting
    uint32_t ws_clock_reverse         : 1;  // SetOption303 - Reverse WS2812/clocked LED direction where supported
    uint32_t receive_raw              : 1;  // SetOption304 - Add raw received data to JSON messages
    uint32_t rf_receive_decimal       : 1;  // SetOption305 - Use decimal RF receive data format
    uint32_t ir_receive_decimal       : 1;  // SetOption306 - Use decimal IR receive data format
    uint32_t artnet_autorun           : 1;  // SetOption307 - Start DMX ArtNet at boot
    uint32_t neopool_outputsensitive  : 1;  // SetOption308 - Output NeoPool sensitive data
    uint32_t counter_both_edges       : 1;  // SetOption309 - Count both rising and falling counter edges
    uint32_t no_power_feedback        : 1;  // SetOption310 - Disable power feedback for relays without power measurement

    uint32_t reserved11               : 1;
    uint32_t reserved12               : 1;
    uint32_t reserved13               : 1;
    uint32_t reserved14               : 1;
    uint32_t reserved15               : 1;
    uint32_t reserved16               : 1;
    uint32_t reserved17               : 1;
    uint32_t reserved18               : 1;
    uint32_t reserved19               : 1;
    uint32_t reserved20               : 1;
    uint32_t reserved21               : 1;
    uint32_t reserved22               : 1;
    uint32_t reserved23               : 1;
    uint32_t reserved24               : 1;
    uint32_t reserved25               : 1;
    uint32_t reserved26               : 1;
    uint32_t reserved27               : 1;
    uint32_t reserved28               : 1;
    uint32_t reserved29               : 1;
    uint32_t reserved30               : 1;
    uint32_t reserved31               : 1;
  };
} SystemBitField__Drivers;


struct SystemParams__Drivers
{
  uint8_t  ir_unknown_threshold;      // SetOption350 - Minimum packet size treated as meaningful UNKNOWN IR
  uint8_t  ir_tolerance_percent;      // SetOption351 - IR matching tolerance percentage
  uint16_t bistable_pulse_ms;         // SetOption352 - Pulse time for bistable/latching relays
  uint8_t  rotary_max_step;           // SetOption353 - Rotary encoder step boundary
  uint8_t  hold_ignore_s;             // SetOption354 - Ignore button/shutter changes for this many seconds
};


struct SystemOptions__Drivers
{
  SystemBitField__Drivers bit;
  SystemParams__Drivers   param;
};


// ============================================================================
// Sensor options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t button_restrict           : 1;  // SetOption400 - Control button multipress behaviour
    uint32_t temperature_conversion    : 1;  // SetOption401 - Temperature unit conversion
    uint32_t pressure_conversion       : 1;  // SetOption402 - Pressure unit conversion
    uint32_t ds18x20_internal_pullup   : 1;  // SetOption403 - Enable internal pull-up for single DS18x20
    uint32_t hx711_json_weight_change  : 1;  // SetOption404 - Publish JSON message on HX711 weight change
    uint32_t mhz19b_abc_disable        : 1;  // SetOption405 - Disable MH-Z19(B) automatic baseline correction
    uint32_t button_swap_on_single_device : 1;  // SetOption406 - Swap button behaviour on single device

    uint32_t reserved07                : 1;
    uint32_t reserved08                : 1;
    uint32_t reserved09                : 1;
    uint32_t reserved10                : 1;
    uint32_t reserved11                : 1;
    uint32_t reserved12                : 1;
    uint32_t reserved13                : 1;
    uint32_t reserved14                : 1;
    uint32_t reserved15                : 1;
    uint32_t reserved16                : 1;
    uint32_t reserved17                : 1;
    uint32_t reserved18                : 1;
    uint32_t reserved19                : 1;
    uint32_t reserved20                : 1;
    uint32_t reserved21                : 1;
    uint32_t reserved22                : 1;
    uint32_t reserved23                : 1;
    uint32_t reserved24                : 1;
    uint32_t reserved25                : 1;
    uint32_t reserved26                : 1;
    uint32_t reserved27                : 1;
    uint32_t reserved28                : 1;
    uint32_t reserved29                : 1;
    uint32_t reserved30                : 1;
    uint32_t reserved31                : 1;
  };
} SystemBitField__Sensors;


struct SystemParams__Sensors
{
  uint8_t decimal_precision;          // SetOption450 - Sensor decimal precision, 0..3
  uint16_t key_hold_time_ms;       // SetOption151 - Button/switch hold detection threshold P_HOLD_TIME
};


struct SystemOptions__Sensors
{
  SystemBitField__Sensors bit;
  SystemParams__Sensors   param;
};


// ============================================================================
// Lighting options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t pwm_multi_channels        : 1;  // SetOption500 - Enable multi-channel PWM instead of colour PWM

    uint32_t reserved01                : 1;
    uint32_t reserved02                : 1;
    uint32_t reserved03                : 1;
    uint32_t reserved04                : 1;
    uint32_t reserved05                : 1;
    uint32_t reserved06                : 1;
    uint32_t reserved07                : 1;
    uint32_t reserved08                : 1;
    uint32_t reserved09                : 1;
    uint32_t reserved10                : 1;
    uint32_t reserved11                : 1;
    uint32_t reserved12                : 1;
    uint32_t reserved13                : 1;
    uint32_t reserved14                : 1;
    uint32_t reserved15                : 1;
    uint32_t reserved16                : 1;
    uint32_t reserved17                : 1;
    uint32_t reserved18                : 1;
    uint32_t reserved19                : 1;
    uint32_t reserved20                : 1;
    uint32_t reserved21                : 1;
    uint32_t reserved22                : 1;
    uint32_t reserved23                : 1;
    uint32_t reserved24                : 1;
    uint32_t reserved25                : 1;
    uint32_t reserved26                : 1;
    uint32_t reserved27                : 1;
    uint32_t reserved28                : 1;
    uint32_t reserved29                : 1;
    uint32_t reserved30                : 1;
    uint32_t reserved31                : 1;
  };
} SystemBitField__Lights;


struct SystemParams__Lights
{
  uint8_t reserved;
};


struct SystemOptions__Lights
{
  SystemBitField__Lights bit;
  SystemParams__Lights   param;
};


// ============================================================================
// Power / energy options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t energy_weekend            : 1;  // SetOption600 - Enable weekend energy tariff behaviour
    uint32_t hardware_energy_total     : 1;  // SetOption601 - Use hardware energy total counter as reference
    uint32_t no_export_energy_today    : 1;  // SetOption602 - Do not add export energy to today's energy total
    uint32_t bistable_single_pin       : 1;  // SetOption603 - Bistable relay uses one pin instead of two pins

    uint32_t reserved04                : 1;
    uint32_t reserved05                : 1;
    uint32_t reserved06                : 1;
    uint32_t reserved07                : 1;
    uint32_t reserved08                : 1;
    uint32_t reserved09                : 1;
    uint32_t reserved10                : 1;
    uint32_t reserved11                : 1;
    uint32_t reserved12                : 1;
    uint32_t reserved13                : 1;
    uint32_t reserved14                : 1;
    uint32_t reserved15                : 1;
    uint32_t reserved16                : 1;
    uint32_t reserved17                : 1;
    uint32_t reserved18                : 1;
    uint32_t reserved19                : 1;
    uint32_t reserved20                : 1;
    uint32_t reserved21                : 1;
    uint32_t reserved22                : 1;
    uint32_t reserved23                : 1;
    uint32_t reserved24                : 1;
    uint32_t reserved25                : 1;
    uint32_t reserved26                : 1;
    uint32_t reserved27                : 1;
    uint32_t reserved28                : 1;
    uint32_t reserved29                : 1;
    uint32_t reserved30                : 1;
    uint32_t reserved31                : 1;
  };
} SystemBitField__Power;


struct SystemParams__Power
{
  uint8_t  max_power_retry;             // SetOption650 - Max retries before deciding power-limit overflow
  int16_t  over_temperature_c;          // SetOption651 - Turn all power off at or above this temperature

  uint8_t  calc_resolution;             // SetOption652 - Calculation display/reporting resolution
  uint8_t  weight_resolution;           // SetOption653 - Weight display/reporting resolution
  uint8_t  frequency_resolution;        // SetOption654 - Frequency display/reporting resolution
  uint8_t  axis_resolution;             // SetOption655 - Axis display/reporting resolution
  uint8_t  current_resolution;          // SetOption656 - Current display/reporting resolution
  uint8_t  voltage_resolution;          // SetOption657 - Voltage display/reporting resolution
  uint8_t  wattage_resolution;          // SetOption658 - Wattage display/reporting resolution
  uint8_t  emulation;                   // SetOption659 - Energy/power emulation mode
  uint8_t  energy_resolution;           // SetOption660 - Energy display/reporting resolution
  uint8_t  pressure_resolution;         // SetOption661 - Pressure display/reporting resolution
  uint8_t  humidity_resolution;         // SetOption662 - Humidity display/reporting resolution
  uint8_t  temperature_resolution;      // SetOption663 - Temperature display/reporting resolution

  uint16_t power_on_delay_ms;           // SetOption664 - Delay at power-on, milliseconds
  uint16_t power_on_delay_s;            // SetOption665 - Delay before activating relays, seconds
  uint16_t bistable_pulse_ms;           // SetOption666 - Bistable relay pulse time, milliseconds
};


struct SystemOptions__Power
{
  SystemBitField__Power bit;
  SystemParams__Power   param;
};


// ============================================================================
// Rules/event options
// ============================================================================

typedef union {
  uint32_t data;

  struct {
    uint32_t system_init        : 1;  // SetOption700 - Event: system initialised
    uint32_t system_boot        : 1;  // SetOption701 - Event: system boot completed
    uint32_t time_init          : 1;  // SetOption702 - Event: time subsystem initialised
    uint32_t time_set           : 1;  // SetOption703 - Event: valid time acquired/set

    uint32_t mqtt_connected     : 1;  // SetOption704 - Event: MQTT connected
    uint32_t mqtt_disconnected  : 1;  // SetOption705 - Event: MQTT disconnected

    uint32_t wifi_connected     : 1;  // SetOption706 - Event: WiFi connected
    uint32_t wifi_disconnected  : 1;  // SetOption707 - Event: WiFi disconnected

    uint32_t eth_connected      : 1;  // SetOption708 - Event: Ethernet connected
    uint32_t eth_disconnected   : 1;  // SetOption709 - Event: Ethernet disconnected

    uint32_t http_init          : 1;  // SetOption710 - Event: HTTP/WebUI initialised

    uint32_t shutter_moved      : 1;  // SetOption711 - Event: shutter moved
    uint32_t shutter_moving     : 1;  // SetOption712 - Event: shutter moving

    uint32_t reserved13         : 1;
    uint32_t reserved14         : 1;
    uint32_t reserved15         : 1;
    uint32_t reserved16         : 1;
    uint32_t reserved17         : 1;
    uint32_t reserved18         : 1;
    uint32_t reserved19         : 1;
    uint32_t reserved20         : 1;
    uint32_t reserved21         : 1;
    uint32_t reserved22         : 1;
    uint32_t reserved23         : 1;
    uint32_t reserved24         : 1;
    uint32_t reserved25         : 1;
    uint32_t reserved26         : 1;
    uint32_t reserved27         : 1;
    uint32_t reserved28         : 1;
    uint32_t reserved29         : 1;
    uint32_t reserved30         : 1;
    uint32_t reserved31         : 1;
  };
} SystemBitField__Rules;


struct SystemParams__Rules
{
  uint8_t reserved;
};


struct SystemOptions__Rules
{
  SystemBitField__Rules bit;
  SystemParams__Rules   param;
};




/*******************************************************************************************************************************************************************
********************************************************************************************************************************************************************
** SECITON: Next ***************************************************************************************************************************************************
********************************************************************************************************************************************************************
*******************************************************************************************************************************************************************/

// Buffer that stores names of sensors as delimeter list
#ifndef DEVICENAMEBUFFER_NAME_INDEX_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_INDEX_LENGTH 70 
#endif // DEVICENAMEBUFFER_NAME_INDEX_LENGTH
#ifndef DEVICENAMEBUFFER_NAME_BUFFER_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 400 
#endif // DEVICENAMEBUFFER_NAME_BUFFER_LENGTH
struct DeviceNameBuffer{ // size(230)
  // delimeter name list
  char name_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH];
  // index array that holds name_list sensor (class name, sensor number)
  /**
   * This needs to be the large unique identifier
   * "Unused" will be the largest possible value, instead of using signed value and setting to "-1", which makes -2->minimum wasted 
   * */
  uint16_t class_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH]  = { D_MAX_UINT16 }; //hold class id
  uint8_t  device_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH] = { D_MAX_UINT8  };  //max of X sensors per module
};


struct LoggingSettings{
  uint8_t       serial_level;           // 09E
  uint16_t      sys_port;               // 1A8
  uint8_t       sys_level;              // 1AA
  uint8_t       web_level;              // 1AC
  uint8_t       telnet_level;              // 1AC
  uint8_t       mqtt_level;              // 1AC
  uint8_t       display_serial_level; // ie the new nextion way
  uint8_t       time_isshort;   // should become short/long/only some types/none
};



struct SETTINGS {
  // Header (Minimal data load required to validate settings - order must never change)
  uint16_t      cfg_holder;                // 000 v6 header
  uint16_t      cfg_size;                  // 002
  uint32_t      save_flag;                 // 004
  uint32_t      version;                   // 008
  uint16_t      bootcount;              // 00C
  // Body (All other settings)
  
  // Visible binary debug marker. Updated immediately before /settings.txt is written.
  // Format: "utcHHMMSS:DDMMYY", example "utc142305:130526".
  char          settings_file_update_utc_ascii[18];
  char          settings_holder_ctr[5]; 
  
  // Modules
  uint16_t      bootcount_errors_only;     // E01
  uint8_t       module;                    // 474
  uint8_t       last_module;               // 399
  // Templates
  Template_Config user_template; 
  SystemName      system_name;                             // Move into SettingsText
  char room_hint[50];                                      // Move into SettingsText
  
  // --------------------------------------------------------------------------
  // System option groups
  // --------------------------------------------------------------------------
  // Each group contains:
  //   bit   = compact boolean flags
  //   param = named numeric parameters
  // --------------------------------------------------------------------------
  SystemOptions__System  sysopt_system;
  SystemOptions__Network sysopt_network;
  SystemOptions__Drivers sysopt_drivers;
  SystemOptions__Sensors sysopt_sensors;
  SystemOptions__Lights  sysopt_lights;
  SystemOptions__Power   sysopt_power;
  SystemOptions__Rules   sysopt_rules;



  int16_t       save_data;                 // 014
  myio          module_pins;                     // 484     
  uint8_t       baudrate;                  // 09D  // saved as (/300) value. ie 9600/300 => 32, 115200=>384?? I want to change this to full uint32_t for higher speed bauds
  uint8_t       rule_stop;                 // 1A7
  LoggingSettings logging;
  DeviceNameBuffer        device_name_buffer; // this might need moving and then changed to use vectors, it would actually remove the need for delims
  uint8_t       enable_sleep;        // E03
  char          serial_delimiter;          // 451
  uint8_t       sbaudrate;                 // 452
  uint8_t       sleep;                     // 453
  uint8_t       setoption_255[PARAM8_SIZE]; // https://tasmota.github.io/docs/Commands/#setoptions "aka param"
  // Core
  uint16_t      unified_interface_reporting_invalid_reading_timeout_seconds; // 0 is ignored, anything else is the seconds of age above which a sensor should not be reporting (ie is invalid)

  // Webserver
  uint8_t       webserver;                 // 1AB
  uint16_t      web_refresh;               // 7CC
  // Time
  int8_t        timezone;                  // 016
  uint8_t       timezone_minutes;          // 66D 
  int8_t        timezone2;                  // 016
  uint8_t       timezone_minutes2;          // 66D
  
  int16_t       toffset[2];                // 30E

  // Previously other char arrays followed this memory space that was reserved as "overflow" fom text pool to be read in another format
  // From now on, the text pool must be the hardcoded full length
  char          text_pool[settings_text_size];            // 017  Size is settings_text_size
  
  // Pulse Counter
  uint16_t      pulse_timer[MAX_PULSETIMERS];  // 57C
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  // Sensors
  uint16_t      button_debounce;           // 542
  uint16_t      switch_debounce;           // 66E
  uint8_t       switchmode[8];
  
  // Drivers
  uint16_t      ledmask;                   // 7BC
  uint8_t       ledstate;                  // 2FB
  uint8_t       ledpwm_mask;               // E8F  
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40
  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint32_t      monitors;                  // 7A0
  uint16_t      pwm_range;                 // 342
  uint16_t      pwm_frequency;             // 2E6
  uint16_t      rf_duplicate_time;         // 522
  // Power
  unsigned long power;                     // 2E8
  uint8_t       poweronstate;              // 398
  power_t       interlock[MAX_INTERLOCKS_SET];  // 4D0 MAX_INTERLOCKS = MAX_RELAYS / 2
  
  // Energy
  
  // Displays
  uint32_t      i2c_drivers[3];            // FEC
  uint64_t      rf_protocol_mask;          // FA8
  uint32_t      power_lock;                // F9C
  uint32_t      bootcount_reset_time;      // FD4
  // uint32_t      ipv4_address[5];           // 544
  // uint32_t      ipv4_rgx_address;          // 558
  // uint32_t      ipv4_rgx_subnetmask;       // 55C
  // uint16_t      dns_timeout;               // 4C8
  #ifdef ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  char local_time_ascii_debug[20];
  #endif
  // E00 - FFF (4095 ie eeprom size) free locations
  uint32_t      cfg_timestamp;
  uint32_t      cfg_crc32;                 // 32 bit CRC, must remain at last 4 bytes
} Settings;



  void TestSettingsLoad();
  void TestSettings_ShowLocal_Header();
    
  struct FIRMWARE_VERSION{
    struct type{
      uint32_t number;
      uint8_t part_branch = 0;
      uint8_t part_major = 0;
      uint8_t part_minor = 0;
      uint8_t part_system = 0;
      uint8_t part_module = 0;
      char name_ctr[15];
    };
    struct type current;
    struct type latest;
    struct type lowest;
    uint8_t fNewVersionAvailable = false;
    uint8_t fCurrentVersionNotSupported = false;
  };

  #ifdef ESP8266
  SerialConfig serial_config = SERIAL_8N1;    // Serial interface configuration 8 data bits, No parity, 1 stop bit
  #endif

  struct FASTBOOT_RUNTIME_FLAGS
  {
    bool disable_rules              = false;
    bool disable_sensors            = false;
    bool disable_drivers            = false;
    bool disable_module_config_load = false;
    bool disable_templates          = false;

    bool force_safe_compiled_config = false;
    bool factory_ap_recovery        = false;
    bool blocking_safe_mode         = false;
  };

  // These are used only at runtime, and not saved eg TasmotaGlobals
  struct RUNTIME_GLOBALS{
    uint8_t sleep;                              // Current copy of Settings.sleep
    uint32_t tSavedUpdateLoopStatistics;
    uint8_t light_driver;                     // Light module configured
    uint8_t light_type;                       // Light types
    TemplateLoading template_loading;
    FIRMWARE_VERSION firmware_version;    
    uint32_t pulse_timer[MAX_PULSETIMERS];    // Power off timer
    power_t power = 0;                          // Current copy of Settings.power
    power_t power_latching;                   // Current state of single pin latching power
    int ota_state_flag = 0;                     // OTA state flag
    int ota_result = 0;                         // OTA result
    int restart_flag = 0;                       // Sonoff restart flag
    int blinks = 201;                           // Number of LED blinks
    uint32_t loop_load_avg = 0;                 // Indicative loop load average
    char *ota_url;                              // OTA url string pointer
    int16_t save_data_counter;                  // Counter and flag for config save to Flash
    uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
    uint8_t backlog_index = 0;                  // Command backlog index
    uint8_t backlog_pointer = 0;                // Command backlog pointer
    uint8_t blinkspeed = 1;                     // LED blink rate
    uint8_t settings_save_decounter_seconds_delayed_save = 0;    
    uint8_t active_device = 1;                  // Active device in ExecuteCommandPower
    uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
    uint8_t counter_no_pullup = 0;              // Counter input pullup flag (1 = No pullup)
    uint8_t serial_in_byte;                     // Received byte
    uint8_t seriallog_level;                    // Current copy of Settings.seriallog_level
    uint8_t syslog_level;                       // Current copy of Settings.syslog_level
    uint8_t seriallog_level_during_boot;
    uint8_t my_module_type;                     // Current copy of Settings.module or user template type
    uint8_t last_source = 0;                    // Last command source
    uint8_t mdns_delayed_start = 0;             // mDNS delayed start
    
    bool blinkstate = false;                    // LED state
    bool pwm_present = false;                   // Any PWM channel configured with SetOption15 0
    bool i2c_enabled = false;                       // I2C configured
    #ifdef ESP32
    bool i2c_enabled_2 = false;                       // I2C configured, second controller on ESP32, Wire1
    #endif
    bool camera_initialized;                  // For esp32-webcam, to be used in discovery
    bool spi_flg = false;                       // SPI configured
    bool soft_spi_flg = false;                  // Software SPI configured
    bool ntp_force_sync = false;                // Force NTP sync
    uint8_t power_on_delay = 0;                   // Delay relay power on to reduce power surge (SetOption47)

    uint32_t blink_timer;                     // Power cycle timer
    uint16_t blink_counter;                   // Number of blink cycles
  
    myio my_module;                             // Active copy of Module GPIOs (17 x 8 bits)
    gpio_flag my_module_flag;                   // Active copy of Module GPIO flags
    StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)
    char my_hostname[50];                       // Composed Wifi hostname
    uint8_t flag_boot_complete = false;
    int wifi_state_flag = 0;         // WIFI_RESTART Wifi state flag    
    bool settings_lkg = false;  // Settings saved as Last Known Good
    uint8_t enable_web_logging_filtering = false;
    uint8_t enable_serial_logging_filtering = false;    
    bool settings_holder_hardcorded_stored_changed = false; // if true, other files may want to reset too

    FASTBOOT_RUNTIME_FLAGS fastboot;
  }runtime;

  #define RESET_BOOT_STATUS() memset(&tkr_set->runtime.boot_status,0,sizeof(tkr_set->runtime.boot_status))

  void CommandSet_SystemRestartID(uint8_t value);

  int16_t     SwitchMode_GetID_by_Name(const char* c);
  const char* SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen);

  #define RESPONSE_MESSAGE_BUFFER_SIZE 100
  char response_msg[RESPONSE_MESSAGE_BUFFER_SIZE] = {0};

  const char* Get_Json_Level_Name(uint8_t id);

  void SettingsUpdateFileWriteTimeAscii(void);

  uint32_t GetSettingsTextLen(void);
  bool SettingsUpdateFinished(void);
  bool SettingsUpdateText(uint32_t index, const char* replace_me);
  char* SettingsText(uint32_t index);

};

#endif  // _SETTINGS_H_

