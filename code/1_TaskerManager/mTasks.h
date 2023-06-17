#pragma once

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//move functions into taskermanager
enum TASKER_FUNCTION_TYPES {
  // Init stuff (in importance of boot)
  
  // FUNC_CHECK_POINTERS, //phased out

  /**
   * module load should only be things that can happen before the init phase, 
   * or, I need to call this twice, so some functions required before init and after init are handled (yes, do this)
   * */
  FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM,  // Read progmem configs if needed, read settings configuration
  FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM,  // Read progmem configs if needed, read settings configuration


  FUNC_SETTINGS_DEFAULT,   // Use defaults in code
  FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT,   // Use defaults in code
  FUNC_POINTER_INIT, // Any pointers to buffers, anything not set in this (unset nullptr) must be protected 
  
  
//   FUNC_MODULE_INIT,  // Read progmem configs if needed, read settings configuration
// "FUNC_MODULE_INIT" is phasing out, as its going to be included within PRE_INIT, which is furthermore going to be INIT_PINS

// "PRE_INIT" should be changed to PIN_INIT, and "MODULE_POINTER" should be possibly removed as its either PIN_INIT or INIT
  
  /**
   * @brief "PRE_INIT" as FUNC_INIT_GPIO_POINTERS?
   * 
   */
  FUNC_PRE_INIT,     // Configure sub modules and classes as needed
  
  
  FUNC_INIT,         // Actually complete init, read sensors, enable modules fully etc

  FUNC_INIT_DELAYED_SECONDS, // To enable modules to either intentional begin with a delayed start, or, to changed at an interval if it is possible to start again (e.g. new sensor connected)

  /**
   * Flags that are used in debugging, that will override any stored or default init states. This will be optionally called at the end of setup.
   * */
  FUNC_OVERRIDE_BOOT_INIT,

  // This will run AFTER json templates
  FUNC_POST_INIT,    // similar to FUNC_CONFIGURE_MODULES_FOR_DEVICE, will run immediately after init

  /**
   * Eg. parse_jsonCommand from template sets energy (pzem) to have X sensors during boot. Prior to leaving setup, this will be called to refresh these buffers based on the latest dynamic buffers needed
   * */
  FUNC_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID,
  

  FUNC_FUNCTION_LAMBDA_INIT,
  FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES,   // Use defaults in code
  FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE, // Load values from settings struct and overwrite module values
  FUNC_CONFIGURE_MODULES_FOR_DEVICE, // Configure any sensors that are needed for controllers, to the required settings
  FUNC_MQTT_HANDLERS_INIT,
  // FUNC_MQTT_INIT,
  
  FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD, // This is called from the above function, used to parse the object json
  
  // New trigger events which only modules to react to changes "triggers" from other modules or external sources
  FUNC_TRIGGER_EVENT_BUTTON_PRESSED,
  // FUNC_TRIGGER_
  
  
  FUNC_ON_BOOT_COMPLETE, //also used as boot percentage progress divisor : Reaches end of setup
// END OF BOOT SECTION 
  FUNC_ON_BOOT_SUCCESSFUL, //should be triggered once after device is considered stable

  // Configure sensors and drivers for device
  // Looping trigger times
  FUNC_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_250_MSECOND, 
  FUNC_EVERY_SECOND, FUNC_EVERY_SECOND_TP0MS_WINDOW, FUNC_EVERY_SECOND_TP250MS_WINDOW, FUNC_EVERY_SECOND_TP500MS_WINDOW, FUNC_EVERY_SECOND_TP750MS_WINDOW, // All run only once a second, but with delays to spread out tasks which occur at that time cycle
  FUNC_EVERY_FIVE_SECOND, //Used mainly as debugging
  FUNC_EVERY_FIVE_MINUTE,
  FUNC_EVERY_MINUTE, FUNC_EVERY_HOUR, FUNC_EVERY_MIDNIGHT, FUNC_EVERY_MIDDAY,
  // Run once from restart/uptime
  FUNC_UPTIME_10_SECONDS, //FUNC_ON_BOOT_SUCCESSFUL same time slot
  FUNC_UPTIME_30_SECONDS,
  FUNC_UPTIME_1_MINUTES,
  FUNC_UPTIME_10_MINUTES,
  FUNC_UPTIME_60_MINUTES,
  // Exact minutes to work aka time cron
  // FUNC_AT_TIME_1_MINUTE,
  // Welcome message to show on boot after X seconds with config
  // FUNC_RESTART_SPLASH_INFORMATION, //have flag that shows level of information AND make this an ifdef as a debug option

  // New list of support items to splash debug info
  FUNC_LOG__SHOW_UPTIME,

  // New method that based on time, will only show the sensor at this period to "AddLog" reducing large serial prints
  FUNC_SENSOR_SHOW_LATEST_LOGGED_ID,

  FUNC_UPDATE_OTA_BEFORE_ON_START,

  /**
   * @brief Special FUNC_X that should be used in testing, and will run with delayed start to enable OTA to be possible when boot loop would occur
   **/
  FUNC_DELAYED_START_LOOP,
  
  // FUNC_PREP_BEFORE_TELEPERIOD, 
  FUNC_JSON_APPEND, 
  
  // Saving/EEPROM related
  FUNC_SAVE_BEFORE_RESTART, //phase out
  
  /**
   * Transfer any local variables into settings struct for saving
   * */
  FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE, // Save internal module values to global settings struct, prior to saving to memory

  // New call functio similar to mqtt method, called from class loop (checked in support)
  FUNC_FUNCTION_LAMBDA_LOOP,

  // System/Status message
  FUNC_BOOT_MESSAGE, //at 10 seconds, show how the function is configured

  FUNC_JSON_COMMAND_CHECK_TOPIC_ID,
  FUNC_JSON_COMMAND_ID,  //ie check for the topic (in the future use module name are set/<moduclename>)
  /**
   * @brief This assumes JBI is configured, and assuming the buffer is valid, will continue to append information related to scanning of sensors.
   * Example: DS18XX will scan both busses and search for addresses of connected devices. Note this also may require the init process (or force it) to run again (or preinit?) so anything that truly must happen once should be in its own section 
   * 
   */
  FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID,
  // FUNC_JSON_COMMAND_START_PARSER_ID,
  // Wifi 
  FUNC_WIFI_CONNECTED, FUNC_WIFI_DISCONNECTED,
  // Mqtt
  FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_CONNECTED, FUNC_MQTT_DISCONNECTED, FUNC_MQTT_COMMAND, FUNC_MQTT_SENDER,
  FUNC_MQTT_STATUS_REFRESH_SEND_ALL,
  //FUNC_MQTT_HANDLERS_RESET, 
  FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD,
  FUNC_MQTT_CHECK_REDUNCTION_LEVEL, 
  FUNC_MQTT_DATA_ID,

  /**
   * @brief Response Tasks
   **/
  FUNC_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID,

  /**
   * @brief Rules
   **/
  FUNC_RULES_LOAD_FROM_PROGMEM_ID,
  FUNC_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID,


  /**
   * @brief NNew set of alert messages that can be created depending on the type of driver
   * e.g., via STATUS_LED, STATUS_BEEPER, STATUS_DISPLAY
   */
  FUNC_STATUS_ALERT__NETWORK_CONNECTION_LOST,



  // Events (new internal triggers, if something happens trigger another... this will become rules)
  // Ie if this event below happens, then have another ID which is called to run the triggered event
  FUNC_EVENT_MOTION_STARTED_ID,
  FUNC_EVENT_MOTION_ENDED_ID,

  // Energy
  FUNC_ENERGY_RESET,
  // Calls to run drivers when sensors change rather than waiting on loop
  FUNC_SENSOR_UPDATED,
  //
  FUNC_STATUS_MESSAGE_APPEND, FUNC_JSON_HARDWARE_APPEND, // Messages
  FUNC_SET_CHANNELS, 
  /**
   * @brief Webpage
   * 
   */
  FUNC_WEB_ADD_MAIN_BUTTON, 
  FUNC_WEB_ADD_BUTTON, 
  FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS, //system settings 
  FUNC_WEB_ADD_HANDLER, //root page
  FUNC_WEB_ROOT_SEND_STYLE, // Send any style for root page
  FUNC_WEB_ROOT_SEND_SCRIPT,  
  FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING, 
  FUNC_WEB_ROOT_SEND_BODY,   // send once on load 
  FUNC_WEB_ROOT_SEND_STATUS,     //the tas way
  FUNC_WEB_ADD_ROOT_SHOWS,
  FUNC_WEB_ADD_JSON_FETCH_RESULT, // As part of fetch
  FUNC_WEB_ADD_JSON_DATA_FETCH_URL, // Adding urls that will be requested for as part of jsonparse
  FUNC_WEB_ADD_ROOT_TABLE_ROWS,
  FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER,
  FUNC_WEB_ADD_ROOT_SCRIPT,
  FUNC_WEB_ADD_ROOT_STYLE,  
  FUNC_WEB_APPEND_RUNTIME_ROOT_URLS,  
  FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED, //send table data
  FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED,
  FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS,
  FUNC_WEB_APPEND_ROOT_BUTTONS,
  FUNC_WEB_PAGEINFORMATION_SEND_MODULE, //send parts of page info
  // parital sub functions that need to be address later by special "interface tasker"
  FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON, //reply with liveview json data "WEB_RESPOND"=func group, "LIGHT"=interface, LIVEVIEW_JSON=type
  FUNC_WEB_COMMAND,   
  FUNC_WEB_SYSTEM_INFO,  //WEBSERVER

  /**
   * @brief Debugging
   * 
   */
  FUNC_DEBUG_CONFIGURE,

/**
 * @note Display driver interface (I could check if within range, it direct goes to that interface sub tasks)
 * */
  FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT, FUNC_DISPLAY_EVERY_50_MSECOND, 
  //FUNC_DISPLAY_EVERY_SECOND,
  FUNC_DISPLAY_REFRESH_SHOW_ID,
  FUNC_DISPLAY_MODEL, FUNC_DISPLAY_MODE, FUNC_DISPLAY_POWER,
  FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_DRAW_FRAME,
  FUNC_DISPLAY_DRAW_HLINE, FUNC_DISPLAY_DRAW_VLINE, FUNC_DISPLAY_DRAW_LINE,
  FUNC_DISPLAY_DRAW_CIRCLE, FUNC_DISPLAY_FILL_CIRCLE,
  FUNC_DISPLAY_DRAW_RECTANGLE, FUNC_DISPLAY_FILL_RECTANGLE,
  FUNC_DISPLAY_TEXT_SIZE, FUNC_DISPLAY_FONT_SIZE, FUNC_DISPLAY_ROTATION, FUNC_DISPLAY_DRAW_STRING,
  FUNC_DISPLAY_DIM, FUNC_DISPLAY_BLINKRATE,
#ifdef USE_UFILESYS
  FUNC_DISPLAY_BATCH,
#endif
  FUNC_DISPLAY_NUMBER, FUNC_DISPLAY_FLOAT, FUNC_DISPLAY_NUMBERNC, FUNC_DISPLAY_FLOATNC,
  FUNC_DISPLAY_RAW, FUNC_DISPLAY_LEVEL, FUNC_DISPLAY_SEVENSEG_TEXT, FUNC_DISPLAY_SEVENSEG_TEXTNC,
  FUNC_DISPLAY_SCROLLDELAY, FUNC_DISPLAY_CLOCK, FUNC_DISPLAY_SCROLLTEXT,

  /**
   * Group of "Events" functions, which are called when sections of the code execute
   * */
  FUNC_EVENT_INPUT_STATE_CHANGED_ID,
  FUNC_EVENT_SET_POWER_ID,
  FUNC_EVENT_SET_SPEED_ID,

  
  FUNC_SET_POWER, FUNC_SET_DEVICE_POWER, FUNC_SHOW_SENSOR,
  FUNC_RULES_PROCESS, FUNC_SERIAL, FUNC_FREE_MEM, FUNC_BUTTON_PRESSED,
  FUNC_SET_POWER_ON_ID,
  FUNC_SET_POWER_OFF_ID,

  FUNC_LIGHT_POWER_ON,
  FUNC_LIGHT_POWER_OFF,
  FUNC_LIGHT_UPDATE_OUTPUT,


  FUNC_CELLULAR_CONNECTION_ESTABLISHED, 
  FUNC_CELLULAR_CONNECTION_LOST,
  FUNC_NETWORK_CONNECTION_ESTABLISHED, // Generalised for any connection type (wifi, cellular, ethernet)
  FUNC_NETWORK_CONNECTION_LOST,


  FUNC_FINALLY_END_OF_LOOP, // Ran at the end of each loop, used to reset flags that should have been handled eg motion

  FUNC_LENGTH
};

#ifdef ENABLE_DEBUG_FUNCTION_NAMES

DEFINE_PGM_CTR(PM_FUNC_POINTER_INIT_CTR)                            D_FUNC_POINTER_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_TEMPLATE_LOAD_CTR)                           D_FUNC_TEMPLATE_LOAD_CTR;
// DEFINE_PGM_CTR(PM_FUNC_MODULE_INIT_CTR)                             D_FUNC_MODULE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_PRE_INIT_CTR)                                D_FUNC_PRE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_INIT_CTR)                                    D_FUNC_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR)            D_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
DEFINE_PGM_CTR(PM_FUNC_LOOP_CTR)                                    D_FUNC_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_50_MSECOND_CTR)                        D_FUNC_EVERY_50_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_100_MSECOND_CTR)                       D_FUNC_EVERY_100_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_250_MSECOND_CTR)                       D_FUNC_EVERY_250_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_SECOND_CTR)                            D_FUNC_EVERY_SECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MINUTE_CTR)                            D_FUNC_EVERY_MINUTE_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_HOUR_CTR)                              D_FUNC_EVERY_HOUR_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDNIGHT_CTR)                          D_FUNC_EVERY_MIDNIGHT_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDDAY_CTR)                            D_FUNC_EVERY_MIDDAY_CTR;
DEFINE_PGM_CTR(PM_FUNC_ON_SUCCESSFUL_BOOT_CTR)                      D_FUNC_ON_SUCCESSFUL_BOOT_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_SECONDS_CTR)                       D_FUNC_UPTIME_10_SECONDS_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_1_MINUTES_CTR)                        D_FUNC_UPTIME_1_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_MINUTES_CTR)                       D_FUNC_UPTIME_10_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_60_MINUTES_CTR)                       D_FUNC_UPTIME_60_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_RESTART_SPLASH_INFORMATION_CTR)              D_FUNC_RESTART_SPLASH_INFORMATION_CTR;
DEFINE_PGM_CTR(PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR)                  D_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_APPEND_CTR)                             D_FUNC_JSON_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SAVE_BEFORE_RESTART_CTR)                     D_FUNC_SAVE_BEFORE_RESTART_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_DEFAULT_CTR)                        D_FUNC_SETTINGS_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR)     D_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR)     D_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR)        D_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR)        D_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_INIT_CTR)                    D_FUNC_FUNCTION_LAMBDA_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR)                    D_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_CTR)                                 D_FUNC_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_SENSOR_CTR)                          D_FUNC_COMMAND_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_DRIVER_CTR)                          D_FUNC_COMMAND_DRIVER_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_CTR)                            D_FUNC_JSON_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_OBJECT_CTR)                     D_FUNC_JSON_COMMAND_OBJECT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_CONNECTED_CTR)                          D_FUNC_WIFI_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_DISCONNECTED_CTR)                       D_FUNC_WIFI_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SUBSCRIBE_CTR)                          D_FUNC_MQTT_SUBSCRIBE_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_INIT_CTR)                               D_FUNC_MQTT_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_CONNECTED_CTR)                          D_FUNC_MQTT_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_DISCONNECTED_CTR)                       D_FUNC_MQTT_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_COMMAND_CTR)                            D_FUNC_MQTT_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SENDER_CTR)                             D_FUNC_MQTT_SENDER_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_RESET_CTR)                     D_FUNC_MQTT_HANDLERS_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_INIT_CTR)                      D_FUNC_MQTT_HANDLERS_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR)        D_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_POWER_CTR)                               D_FUNC_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_DEVICE_POWER_CTR)                        D_FUNC_SET_DEVICE_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SHOW_SENSOR_CTR)                             D_FUNC_SHOW_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_RULES_PROCESS_CTR)                           D_FUNC_RULES_PROCESS_CTR;
DEFINE_PGM_CTR(PM_FUNC_SERIAL_CTR)                                  D_FUNC_SERIAL_CTR;
DEFINE_PGM_CTR(PM_FUNC_FREE_MEM_CTR)                                D_FUNC_FREE_MEM_CTR;
DEFINE_PGM_CTR(PM_FUNC_BUTTON_PRESSED_CTR)                          D_FUNC_BUTTON_PRESSED_CTR;
DEFINE_PGM_CTR(PM_FUNC_ENERGY_RESET_CTR)                            D_FUNC_ENERGY_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_SENSOR_UPDATED_CTR)                          D_FUNC_SENSOR_UPDATED_CTR;
DEFINE_PGM_CTR(PM_FUNC_STATUS_MESSAGE_APPEND_CTR)                   D_FUNC_STATUS_MESSAGE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_HARDWARE_APPEND_CTR)                    D_FUNC_JSON_HARDWARE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_CHANNELS_CTR)                            D_FUNC_SET_CHANNELS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR)                     D_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_CTR)                          D_FUNC_WEB_ADD_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR)          D_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_HANDLER_CTR)                         D_FUNC_WEB_ADD_HANDLER_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STYLE_CTR)                     D_FUNC_WEB_ROOT_SEND_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR)                    D_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR) D_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_BODY_CTR)                      D_FUNC_WEB_ROOT_SEND_BODY_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STATUS_CTR)                    D_FUNC_WEB_ROOT_SEND_STATUS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR)                      D_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR)               D_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR)             D_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR)                 D_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR)                     D_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_STYLE_CTR)                      D_FUNC_WEB_ADD_ROOT_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR)           D_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR)          D_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR)            D_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR)           D_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR)  D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR)     D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR)        D_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR)                 D_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR)         D_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_COMMAND_CTR)                             D_FUNC_WEB_COMMAND_CTR;
// DEFINE_PGM_CTR(PM_FUNC_CHECK_POINTERS_CTR)                          D_FUNC_CHECK_POINTERS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_SYSTEM_INFO_CTR)                         D_FUNC_WEB_SYSTEM_INFO_CTR;
DEFINE_PGM_CTR(PM_FUNC_DEBUG_CONFIGURE_CTR)                         D_FUNC_DEBUG_CONFIGURE_CTR;

#endif // ENABLE_DEBUG_FUNCTION_NAMES

DEFINE_PGM_CTR(PM_FUNC_EVENT_MOTION_STARTED_CTR)  D_FUNC_EVENT_MOTION_STARTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVENT_MOTION_ENDED_CTR)  D_FUNC_EVENT_MOTION_ENDED_CTR;

DEFINE_PGM_CTR(PM_FUNC_EVENT_INPUT_STATE_CHANGED_CTR)   D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVENT_SET_POWER_CTR)   D_FUNC_EVENT_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVENT_SET_SPEED_CTR)   D_FUNC_EVENT_SET_SPEED_CTR;
