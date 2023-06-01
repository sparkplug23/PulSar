/**
  @brief mBaseConfig.h - minimum defines required for default builds
  @note Users "may" edit this file, but it is instead recommended to redefine them in "mUserConfig_Secret.h"

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
#ifndef MUSERCONFIG_DEFAULTS_H
#define MUSERCONFIG_DEFAULTS_H

#include "stdint.h"
#include "2_CoreSystem/mSystemConfig.h"
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"
#ifdef D_USER_MICHAEL
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_Temporary.h"
#include "0_ConfigUser/01_mFirmwareCustom_Secret_Templates.h"
#include "0_ConfigUser/02_mFirmwareCustom_Secret_DevTestbeds.h"
#endif // D_USER_MICHAEL

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * ATTENTION: Changes to this file WILL be erased with each git pull
 * 
 * "mUserConfig.h" lists all the parameters that users can set as default state when the device is reset.
 * To assert these settings after the initial flash, the SETTINGS_HOLDER should be changed to another number (1-65000)
 * Subsequent flashing will NOT erase these settings IF SETTINGS_HOLDER is not changed.
 * hh
 * Although the user can edit these values directly, it is advised to edit the "mUserConfig_Override.h" file instead, which is
 * called AFTER this file, and will overwrite any settings performed here if they are defined. 
 * 
 * All parameters contained within here can be edited by the user during runtime using mqtt or webui commands, and are stored in memory.
 * 
 * IMPORTANT: If you are not confident in changing values, please leave these as default and configure through the webui or mqtt commands, 
 * these values are intended for advanced users to create defaults for flashing multiple devices for easier configuring.
\*********************************************************************************************/

#define USE_CONFIG_OVERRIDE                      // Uncomment to use mUserConfig_Override.h file. See README.md

/*********************************************************************************************\
 * This MUST BE CHANGED for ANY parameters in this file to overwrite those saved in settings
\*********************************************************************************************/
#ifndef SETTINGS_HOLDER
#define SETTINGS_HOLDER             1              // [Reset 1] Change this value (max 32000) to load SECTION1 configuration parameters to flash
#endif

#define SERIALD Serial //serial debug
#ifndef SERIAL_DEBUG_BAUD_DEFAULT
#define SERIAL_DEBUG_BAUD_DEFAULT 115200
#endif // SERIAL_DEBUG_BAUD_DEFAULT

/*********************************************************************************************\
 * CAUTION! Only select from values defined in the comments next to (right hand side) the parameter. 
 * Some protection is added on value ranges, but it is not guarenteed especially during the development phase.
 * 
 * The names of parameters are suffixed by a shorthand for the type of value it MUST be adhered to:
 * "_CTR"     String (word or phrase) eg. "ON" or "This Value"
 * "_NUM"     Number that is an integer only (no decimel points!) eg 1 or 1023
 * "_FLT"     Float number, which is any number that MAY include a decimel, but does not need to. eg. 1 or 1.0 or 1.234
 * "_ID"      Same as "_NUM"
\*********************************************************************************************/

/*********************************************************************************************\
 *  Module Settings
\*********************************************************************************************/
#define MODULE                        MODULE_WEMOS_ID      // [Module] Select default model from mHardwareTemplates.h

#define SAVE_DATA                     1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE                    1                 // [SetOption0] Save changed power state to Flash (0 = disable, 1 = enable)
#define BOOT_LOOP_OFFSET              1                 // [SetOption36] Number of boot loops before starting restoring defaults (0 = disable, 1..200 = boot loops offset)

// Forcing this to disable until all devices are optimised so nothing is starved of CPU 
#define DISABLE_SLEEP

// #ifdef ESP32
// #define ENABLE_FEATURE_WATCHDOG_TIMER
// #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 60000
// #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
// #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
// #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
// #endif

#define ENABLE_FEATURE_DRIVERS_INTERFACE_UNIFIED_DRIVER_REPORTING


/*********************************************************************************************\
 *  Wifi
\*********************************************************************************************/
#define WIFI_IP_ADDRESS               "0.0.0.0"         // [IpAddress1] Set to 0.0.0.0 for using DHCP or enter a static IP address
#define WIFI_GATEWAY                  "192.168.1.1"     // [IpAddress2] If not using DHCP set Gateway IP address
#define WIFI_SUBNETMASK               "255.255.255.0"   // [IpAddress3] If not using DHCP set Network mask
#define WIFI_DNS                      "192.168.1.1"     // [IpAddress4] If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)

#define STA_SSID1                     ""                // [Ssid1] Wifi SSID
#define STA_PASS1                     ""                // [Password1] Wifi password
#define STA_SSID2                     ""                // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS2                     ""                // [Password2] Optional alternate AP Wifi password
#define STA_SSID3                     ""                // [Ssid2] Optional alternate AP Wifi SSID
#define STA_PASS3                     ""                // [Password2] Optional alternate AP Wifi password

#define WIFI_CONFIG_TOOL              WIFI_RETRY        // [WifiConfig] Default tool if wifi fails to connect
                                                        //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL)
#define WIFI_CONFIG_NO_SSID           WIFI_WPSCONFIG    // Default tool if wifi fails to connect and no SSID is configured
                                                        //   (WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_SERIAL)
                                                        //   *** NOTE: When WPS is disabled by USE_WPS below, WIFI_WPSCONFIG will execute WIFI_MANAGER ***
                                                        //   *** NOTE: When WIFI_MANAGER is disabled by USE_MODULE_CORE_WEBSERVER below, WIFI_MANAGER will execute WIFI_SMARTCONFIG ***
                                                        //   *** NOTE: When WIFI_SMARTCONFIG is disabled by USE_SMARTCONFIG below, WIFI_SMARTCONFIG will execute WIFI_SERIAL ***

/*********************************************************************************************\
 *  Logging
\*********************************************************************************************/
#define SYS_LOG_HOST                  ""                      // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT                  514                     // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL                 LOG_LEVEL_DEBUG_MORE    // [SysLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define SERIAL_LOG_LEVEL              LOG_LEVEL_DEBUG         // [SerialLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define SERIAL_LOG_LEVEL_DURING_BOOT  LOG_LEVEL_INFO         // [SerialLogBoot] LOG_LEVEL_TEST//LOG_LEVEL_ALL
#define WEB_LOG_LEVEL                 LOG_LEVEL_INFO          // [WebLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define TELNET_LOG_LEVEL              LOG_LEVEL_DEBUG
#define LOGTIME_DEFAULT_FORMAT        1                       //  true == short

/*********************************************************************************************\
 *  OTA
\*********************************************************************************************/
#define D_OTA_URL                       "http://minimal.bin"  // [OtaUrl]

/*********************************************************************************************\
 *  MQTT
\*********************************************************************************************/

// #ifndef DISABLE_NETWORK
// #define USE_MODULE_NETWORK_MQTT
// #define USE_MODULE_NETWORK_WIFI
// #endif // DISABLE_NETWORK

#define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED

// #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,65   //TEMPORARY FIX, WILL BE REPLACED BY ABOVE PARSED STRING
#define MQTT_HOST_DISCOVERY

#ifdef USE_NETWORK_MDNS
#define MDNS_MQTT_HOSTNAME1   "TOWER.local" 
#define MDNS_MQTT_HOSTNAME2   "rasbpi.local" 
#endif // #ifdef USE_NETWORK_MDNS

// #define MQTT_PORT                     1884              // [MqttPort] MQTT port (10123 on CloudMQTT)
#define MQTT_USER                     "DVES_USER"       // [MqttUser] MQTT user
#define MQTT_PASS                     "DVES_PASS"       // [MqttPassword] MQTT password

#ifndef   MQTT_MAX_PACKET_SIZE
#define   MQTT_MAX_PACKET_SIZE          2000
#endif // MQTT_MAX_PACKET_SIZE

#define ENABLE_MQTT_DEBUG_TELEMETRY
#define USE_MQTT_RETAINED_VERSION_HISTORY_CHECK
#define ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT

#define D_MQTT_COMMAND                "set"
#define D_MQTT_SYNC                   "sync"


// -- MQTT - TLS ----------------------------------
  // !!! TLS uses a LOT OF MEMORY so be careful to enable other options at the same time !!!
//#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+53k code, +15k mem)
//  #define USE_MQTT_TLS_CA_CERT                   // Use LetsEncrypt Certificate from sonoff_letsencrypt.h - Not supported with core 2.3.0

// -- MQTT - Telemetry ----------------------------
#define TELE_PERIOD            300               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)
//#define TELE_ON_POWER          0                 // [SetOption59] send tele/STATE together with stat/RESULT (0 = Disable, 1 = Enable)

/*********************************************************************************************\
 *  HTTP Webserver
\*********************************************************************************************/
#define WEB_SERVER             2                 // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
#define WEB_PASSWORD           ""                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)
#define FRIENDLY_NAME          DEVICENAME_FRIENDLY_CTR   // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
// HTML hex color codes. Only 3 and 6 digit hex string values are supported!! See https://www.w3schools.com/colors/colors_hex.asp
#define COLOR_TEXT                  "#fff"       // [WebColor1] Global text color - Black
#define COLOR_BACKGROUND            "#000"       // [WebColor2] Global background color - White
#define COLOR_FORM                  "#000000"    // [WebColor3] Form background color - Greyish
#define COLOR_INPUT_TEXT            "#fff"       // [WebColor4] Input text color - Black
#define COLOR_INPUT                 "#000"       // [WebColor5] Input background color - White
#define COLOR_CONSOLE_TEXT          "#fff"       // [WebColor6] Console text color - Black
#define COLOR_CONSOLE               "#000"       // [WebColor7] Console background color - White
#define COLOR_TEXT_WARNING          "#f00"       // [WebColor8] Warning text color - Red
#define COLOR_TEXT_SUCCESS          "#008000"    // [WebColor9] Success text color - Green
#define COLOR_BUTTON_TEXT           "#000"       // [WebColor10] Button text color - White
#ifndef COLOR_BUTTON
  #define COLOR_BUTTON                "#ec7d1f"    // [WebColor11] Button color - Blueish
#endif
#ifndef COLOR_BUTTON_HOVER
  #define COLOR_BUTTON_HOVER          "#7a4111"    // [WebColor12] Button color when hovered over - Darker blueish
#endif
#define COLOR_BUTTON2                "#ec7d1f"    // [WebColor11] Button color - Orange-ish
#define COLOR_BUTTON2_HOVER          "#7a4111"    // [WebColor12] Button color when hovered over - Darker Orange-ish
#define COLOR_BUTTON_RESET          "#d43535"    // [WebColor13] Restart/Reset/Delete button color - Redish
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"    // [WebColor14] Restart/Reset/Delete button color when hovered over - Darker redish
#define COLOR_BUTTON_SAVE           "#47c266"    // [WebColor15] Save button color - Greenish
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"    // [WebColor16] Save button color when hovered over - Darker greenish
#define COLOR_TIMER_TAB_TEXT        "#000"       // [WebColor17] Config timer tab text color - White
#define COLOR_TIMER_TAB_BACKGROUND  "#999"       // [WebColor18] Config timer tab background color - Light grey
#ifndef D_COL_TEXT_MODULE_TITLE_CTR
  #define D_COL_TEXT_MODULE_TITLE_CTR "#fff"
#endif

#define PWM_MAX_SLEEP          10                // Sleep will be lowered to this value when light is on, to avoid flickering


// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1            "pool.ntp.org"       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#define NTP_SERVER2            "nl.pool.ntp.org"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#define NTP_SERVER3            "0.nl.pool.ntp.org"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST_HEMISPHERE    North             // [TimeDst] Hemisphere (0 or North, 1 or South)
#define TIME_DST_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_DST_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_DST_MONTH         Mar               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_DST_HOUR          2                 // Hour (0 to 23)
#define TIME_DST_OFFSET        +60              // Offset from UTC in minutes (-780 to +780)

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD_HEMISPHERE    North             // [TimeStd] Hemisphere (0 or North, 1 or South)
#define TIME_STD_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_STD_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_STD_MONTH         Oct               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_STD_HOUR          2                 // Hour (0 to 23)
#define TIME_STD_OFFSET        0                 // Offset from UTC in minutes (-780 to +780)

// -- Location ------------------------------------
#ifndef LATITUDE              // eg. Belfast, Northern Ireland
#define LATITUDE               54.6         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              -5.92        // [Longitude] Your location to be used with sunrise and sunset
#endif


// -- Application ---------------------------------
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led
                                                  // (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_LEDMASK            0xFFFF            // [LedMask] Assign Relay to Power led (0xFFFF is default)
#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      POWER_ALL_SAVED   // [PowerOnState] Power On Relay state
                                                 //  (POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)
#define DEFAULT_LOOP_SLEEP     20                // [Sleep] Sleep time to lower energy consumption (0 = Off, 1 - 250 mSec)

#define KEY_CHECK_TIME         1000
#define KEY_DEBOUNCE_TIME      50                // [ButtonDebounce] Number of mSeconds button press debounce time
#define KEY_HOLD_TIME          20                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message
#define SWITCH_DEBOUNCE_TIME   50                // [SwitchDebounce] Number of mSeconds switch press debounce time
#define SWITCH_MODE            SWITCHMODE_TOGGLE_ID            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE (the wall switch state)
#define WS2812_LEDS            20                // [Pixels] Number of WS2812 ledout.index to start with (max is 512)

#define TEMP_CONVERSION        0                 // [SetOption8] Return temperature in (0 = Celsius or 1 = Fahrenheit)
#define PRESSURE_CONVERSION    0                 // [SetOption24] Return pressure in (0 = hPa or 1 = mmHg)
#define TEMP_RESOLUTION        1                 // [TempRes] Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // [HumRes] Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // [PressRes] Maximum number of decimals (0 - 3) showing sensor Pressure
#define ENERGY_RESOLUTION      3                 // [EnergyRes] Maximum number of decimals (0 - 5) showing energy usage in kWh
#define CALC_RESOLUTION        3                 // [CalcRes] Maximum number of decimals (0 - 7) used in commands ADD, SUB, MULT and SCALE


#define FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES   false


#ifndef SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS
#define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 10
#endif

/*********************************************************************************************\
 *  Rules
\*********************************************************************************************/
#ifndef D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND
#define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 60
#endif

/*********************************************************************************************\
 * END OF SECTION 1
 *
 * SECTION 2
 * - Enable a feature by removing both // in front of it
 * - Disable a feature by preceding it with //
\*********************************************************************************************/

// -- Wifi Config tools ---------------------------
#define WIFI_SOFT_AP_CHANNEL   1                 // Soft Access Point Channel number between 1 and 13 as used by Wifi Manager web GUI
//#define USE_WPS                                  // Add support for WPS as initial wifi configuration tool (+33k code, 1k mem (5k mem with core v2.4.2+))
//#define USE_SMARTCONFIG                          // Add support for Wifi SmartConfig as initial wifi configuration tool (+23k code, +0.6k mem)

// -- OTA -----------------------------------------
#define USE_ARDUINO_OTA                          // Add optional support for Arduino OTA (+13k code)

// ----- WDT timer
// #define ENABLE_FEATURE_WATCHDOG_TIMER

// -- mDNS ----------------------------------------
#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem)
  #define USE_NETWORK_MDNS                    // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

// -- Time ----------------------------------------
#define USE_RTC
//#define USE_TIMERS                               // Add support for up to 16 timers (+2k2 code)
//  #define USE_TIMERS_WEB                         // Add timer webpage support (+4k5 code)
#define USE_SUNRISE                            // Add support for Sunrise and sunset tools (+16k)
#define SUNRISE_DAWN_ANGLE DAWN_NORMAL       // Select desired Dawn Angle from (DAWN_NORMAL, DAWN_CIVIL, DAWN_NAUTIC, DAWN_ASTRONOMIC)

// -- I2C sensors ---------------------------------
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define I2CDRIVERS_0_31        0xFFFFFFFF          // Enable I2CDriver0  to I2CDriver31
#define I2CDRIVERS_32_63       0xFFFFFFFF          // Enable I2CDriver32 to I2CDriver63
#define I2CDRIVERS_64_95       0xFFFFFFFF          // Enable I2CDriver64 to I2CDriver95

#define USE_PWM // NEEDED FOR H801

// -- HTTP ----------------------------------------
// #define USE_WEBSERVER                            // Enable web server and Wi-Fi Manager (+66k code, +8k mem)
  #define WEB_PORT             80                // Web server Port for User and Admin mode


// -- IR Remote features - all protocols from IRremoteESP8266 --------------------------
// IR Full Protocols mode is now activated through USE_IR_REMOTE_FULL
//
// Code impact of IR full protocols is +90k code, 3k mem

// -- IR Remote features - subset of IR protocols --------------------------
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 (+4k3 code, 0k3 mem, 48 iram)
  #define IR_SEND_INVERTED          false        // Invert the output. (default = false) e.g. LED is illuminated when GPIO is LOW rather than HIGH.
                                                 // Setting inverted to something other than the default could easily destroy your IR LED if you are overdriving it.
                                                 // Unless you REALLY know what you are doing, don't change this.
  #define IR_SEND_USE_MODULATION    true         // Do we do frequency modulation during transmission? i.e. If not, assume a 100% duty cycle.

  // ====> IR Full protocols are enabled with the line below
  // #define USE_IR_REMOTE_FULL                     // Support all IR protocols from IRremoteESP8266
  //
  // WARNING: if you change any IR configuration, you need to clear Platform.io cache
  //          currently the include change detection does not work for these parameters
  //          This is not an issue if you compile with gitpod or pre-compiled binaries
  //          since there is no cache of previously compiled libraries
  // ====>
  //
  // You can also disable specific protocols in user_config_override
  // Example: disable some HVAC protocols to reduce flash size
  // #define DECODE_WHIRLPOOL_AC  false
  // #define SEND_WHIRLPOOL_AC    false
  // #define DECODE_SAMSUNG_AC    false
  // #define SEND_SAMSUNG_AC      false
  // ...

  // When using 'USE_IR_REMOTE_FULL', parameters below
  // (USE_IR_REMOTE, USE_IR_RECEIVE, USE_IR_HVAC...) are IGNORED.
  #define USE_IR_SEND_NEC                        // Support IRsend NEC protocol
  #define USE_IR_SEND_RC5                        // Support IRsend Philips RC5 protocol
  #define USE_IR_SEND_RC6                        // Support IRsend Philips RC6 protocol

  // Enable IR devoder via GPIO `IR Recv` - always enabled if `USE_IR_REMOTE_FULL`
  #define USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)
    #define IR_RCV_BUFFER_SIZE      100          // Max number of packets allowed in capture buffer (default 100 (*2 bytes ram))
    #define IR_RCV_TIMEOUT          15           // Number of milli-Seconds of no-more-data before we consider a message ended (default 15)
    #define IR_RCV_MIN_UNKNOWN_SIZE 6            // Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
    #define IR_RCV_WHILE_SENDING    0            // Turns on receiver while sending messages, i.e. receive your own. This is unreliable and can cause IR timing issues
    #define IR_RCV_TOLERANCE        25           // Base tolerance percentage for matching incoming IR messages (default 25, max 100)


// Anything added by this file must "undef" then "ifdef" to replace it
#ifdef USE_CONFIG_OVERRIDE
#include "0_ConfigUser/G1_mUserConfig_Secret.h"
#endif


#endif


