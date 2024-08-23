#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_TEMPLATES_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_TEMPLATES_H

/*********************************************************************************************\
 * ExampleDevices: 
 *  ** File should contain functional examples of devices to be used as templates
 *  ** All testbeds should have esp32/esp82 appends where appropriate to have dual compiling
 *  ** Other than proof-testing a SINGLE (ie do not make multiple templates in this file) device, any development
 *     of code should be copied into the "DevTesting" file to be worked on. Only when stable, should the config be copied
 *     back into this file to be used as a template for a production device
 * 
 * Rename folder and devicenames below into "Example"
 * 
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/04_HardwareTemplates/mHardwareTemplates.h"

/**
 * TESTBED_PRODUCTION
 *  ** Bought from a manufacturer
**/

// #define DEVICE_TEMPLATE_SHELLY1_01
// #define DEVICE_TEMPLATE_SHELLY2P5_01
// #define DEVICE_TEMPLATE_SHELLY2_DIMMER 
// #define DEVICE_TEMPLATE_SONOFF_4CHPRO 

/**
 * TESTBED_CUSTOM
 *  ** Built using NodeMCU/Doit DevKit
**/
// #define DEVICE_TEMPLATE_RJ45_STANDARD_CONFIG_ALL_SENSORS
// #define DEVICE_EXAMPLE_TEMPLATE__433MHZ_SAW_RF

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--- TESTBED_PRODUCTION ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * Device Type: Shelly 1
 * Modules:   
 *  - 2 Mains " D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "
 *  - 2 Mains Inputs
 *  - Energy monitoring (Voltage, Current and power)
 *  - Reset button on PCB
 *  - LED
 * Notes: Requires neutral
 * 
 * Default configuration
 * Mode A (Inputs are switches eg basic wall switch)
 * Mode B (Inputs are motion events eg security light)
 * 
 * */
#ifdef DEVICE_TEMPLATE_SHELLY1_01
  #define DEVICENAME_CTR          "testbed_shelly1_01"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Tester"
  #define DEVICENAME_ROOMHINT_CTR   "Outdoor|Driveway"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORK_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  
  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
    
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle" 
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D21:00:00\",\"OffTime\":\"00D05:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
  
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * Example
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS




#endif


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Device Type: Shelly 2.5
 * Modules:   
 *  - 2 Mains " D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "
 *  - 2 Mains Inputs
 *  - Energy monitoring (Voltage, Current and power)
 *  - Reset button on PCB
 *  - LED
 * Notes: Requires neutral
 * 
 * Default configuration
 * Mode A (Inputs are switches eg basic wall switch)
 * Mode B (Inputs are motion events eg security light)
 * 
 * */
#ifdef DEVICE_TEMPLATE_SHELLY2P5_01
  #define DEVICENAME_CTR          "testbed_shelly25_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly 2.5 #01"
  #define DEVICENAME_ROOMHINT_CTR   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  // #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_C_SWITCHES_ARE_MOTION_DETECTION_REPORTING_ONLY_OUTPUTS_ARE_REMOTE_CONTROLLED //sidedoor and garage new method

  // #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
    // #define ENABLE_DRIVERS_RELAYS_TIME_LOCKS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORK_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power0"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Power1"
  #define D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "Switch0"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Motion0"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Motion1"  
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"    
    "}"   
    #ifdef ENABLE_DRIVERS_RELAYS_TIME_LOCKS
    ","
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D20:01:00\",\"OffTime\":\"00D05:00:00\"}"
    #endif // ENABLE_DRIVERS_RELAYS_TIME_LOCKS
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Switch1 Toggle = Relay1 Power Toggle
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
   * 
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * 
   * Example
   * 
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * 
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * 
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * 
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * 
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch1 HIGH = Relay1 Power ON for Timed seconds
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        // Module and Function omitted, as not needed, for this example
        "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    // Switch1 HIGH = Motion1 Event Started, ie report as motion with motion name
    "\"Rule3\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule4\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  
  /**
   * New option: No local control, but rule for converting switch into motion event on mqtt
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
   * 
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * 
   * Example
   * 
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * 
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * 
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * 
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * 
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_C_SWITCHES_ARE_MOTION_DETECTION_REPORTING_ONLY_OUTPUTS_ARE_REMOTE_CONTROLLED
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Motion Event = Switch0
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "},"
    // Motion Event = Switch1
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS


#endif // DEVICE_NAME


#ifdef DEVICE_TEMPLATE_SHELLY2_DIMMER
  #define DEVICENAME_CTR          "testbed_shellydimmer_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly Dimmer 2"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_SHELLY_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define DISABLE_SERIAL_LOGGING //temp measure
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Stairs"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Landing"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\"," 
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"   
  "}";

#endif



#ifdef DEVICE_TEMPLATE_SONOFF_4CHPRO
  #define DEVICENAME_CTR          "testbed_4chpro"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff 4CH Pro"
  #define DEVICENAME_ROOMHINT_CTR  "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_RELAY

  // #define USE_MODULE_CONTROLLER_SONOFF_4CHPRO

  #define USE_MODULE_DRIVERS_RF433_RCSWITCH

  // default key# = relay#
  // RF Key433 using rules to match patterns then need limit to how often RF matches 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_4CHPRO_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
 
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0,"
        "\"State\":\"Toggle\""
      "}"
    "},"
    "\"Rule1\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1,"
        "\"State\":\"Toggle\""
      "}"
    "}"
    "},"
    "\"Rule2\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":2,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":2,"
        "\"State\":\"Toggle\""
      "}"
    "}"
    "},"
    "\"Rule3\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":3,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":3,"
        "\"State\":\"Toggle\""
      "}"
    "}"
  "}";

  /**
   * @brief Remote 433 Number 1
   ***
    1L:  Data 0xA37348 (10711880), Bits 24, Protocol 1, Delay 238
    1R:  Data 0xA3734C (10711884), Bits 24, Protocol 1, Delay 239
    2L:  Data 0xA37344 (10711876), Bits 24, Protocol 1, Delay 239
    2R:  Data 0xA37349 (10711881), Bits 24, Protocol 1, Delay 239
    3L:  Data 0xA37342 (10711874), Bits 24, Protocol 1, Delay 244
    3R:  Data 0xA37345 (10711877), Bits 24, Protocol 1, Delay 240
    4L:  Data 0xA37341 (10711873), Bits 24, Protocol 1, Delay 238
    4R:  Data 0xA37343 (10711875), Bits 24, Protocol 1, Delay 239

    rules packing

    function: tasker_func for "received rf"
    deviceid: int list
    values: [
      protocol int value
      delay number (2 bits)
      bits size eg 24 or 32 bits (and hence next bits)
      data1
      data2
      data3]

    For now, instead of handling via rules, lets do this via openhab (ie RF code triggers a press and "expire" resets it after X seconds)

   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0x3194B8 (3249336), Bits 24, Protocol 1, Delay 243
    1R:  Data 0x3194BC (3249340), Bits 24, Protocol 1, Delay 242     
    2L:  Data 0x3194B4 (3249332), Bits 24, Protocol 1, Delay 241  
    2R:  Data 0x3194B9 (3249337), Bits 24, Protocol 1, Delay 241    
    3L:  Data 0x3194B2 (3249330), Bits 24, Protocol 1, Delay 241
    3R:  Data 0x3194B5 (3249333), Bits 24, Protocol 1, Delay 242
    4L:  Data 0x3194B1 (3249329), Bits 24, Protocol 1, Delay 241
    4R:  Data 0x3194B3 (3249331), Bits 24, Protocol 1, Delay 241                           
   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0x6730C8 (6762696), Bits 24, Protocol 1, Delay 240
    1R:  Data 0x6730CC (6762700), Bits 24, Protocol 1, Delay 240
    2L:  Data 0x6730C4 (6762692), Bits 24, Protocol 1, Delay 241
    2R:  Data 0x6730C9 (6762697), Bits 24, Protocol 1, Delay 241
    3L:  Data 0x6730C2 (6762690), Bits 24, Protocol 1, Delay 240
    3R:  Data 0x6730C5 (6762693), Bits 24, Protocol 1, Delay 240
    4L:  Data 0x6730C1 (6762689), Bits 24, Protocol 1, Delay 242
    4R:  Data 0x6730C3 (6762691), Bits 24, Protocol 1, Delay 241 
   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0xE632B8 (15086264), Bits 24, Protocol 1, Delay 237 
    1R:  Data 0xE632BC (15086268), Bits 24, Protocol 1, Delay 236 
    2L:  Data 0xE632B4 (15086260), Bits 24, Protocol 1, Delay 234 
    2R:  Data 0xE632B9 (15086265), Bits 24, Protocol 1, Delay 236 
    3L:  Data 0xE632B2 (15086258), Bits 24, Protocol 1, Delay 235 
    3R:  Data 0xE632B5 (15086261), Bits 24, Protocol 1, Delay 235 
    4L:  Data 0xE632B1 (15086257), Bits 24, Protocol 1, Delay 235 
    4R:  Data 0xE632B3 (15086259), Bits 24, Protocol 1, Delay 236 
   */


#endif


/*
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--- TESTBED_CUSTOM ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


/**
 * Basic version of measurement system
 * GPS will be recorded at 10Hz, and logged to SD card in json format for matlab parsing
 * */
#ifdef DEVICE_TEMPLATE_OLED_SH1106
  #define DEVICENAME_CTR            "testbed_oled_sh1106"
  #define DEVICENAME_FRIENDLY_CTR   "TestBed SH1106"
  #define DEVICENAME_ROOMHINT_CTR   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 
       * OLED
       * */
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      /** 
       * BUILTIN LED
       * */
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_TEMPLATE_OLED_SH1106



/**
 * @brief New single device to run with all standard devices I may connect to the RJ45
 * Assumes PIR on w/g pin
 * Nextion on brown pair
 * All I2C sensors on blue pair
 * Include DS18B20 on b/w pin
 * 9axis on I2C too
 * 
 */
#ifdef DEVICE_TEMPLATE_RJ45_STANDARD_CONFIG_ALL_SENSORS
  #define DEVICENAME_CTR          "testbed_rj45_gpio_toggle"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" "BLUE_WHITE_B_W" "\","
        "\"" "WHITE_BLUE_W_B" "\""
      "]"
    "}"
  "}";

  // #define INSERT_CODE_INIT
  // #define INSERT_CODE_EVERY_SECOND digitalWrite(4,!digitalRead(4)); digitalWrite(5,!digitalRead(5)); 

  // WILL ADD THIS toggle via rules (per second) in the future
  
#endif



/**
 * @brief 
 * 
 * 
 */

#ifdef DEVICE_TEMPLATE_ESPCAM
  #define DEVICENAME_CTR          "testbed_rj45_gpio_toggle"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" "BLUE_WHITE_B_W" "\","
        "\"" "WHITE_BLUE_W_B" "\""
      "]"
    "}"
  "}";

  // #define INSERT_CODE_INIT
  // #define INSERT_CODE_EVERY_SECOND digitalWrite(4,!digitalRead(4)); digitalWrite(5,!digitalRead(5)); 

  // WILL ADD THIS toggle via rules (per second) in the future
  
#endif





/**
 * @brief 
 * Connected via header
 * 
 */
#ifdef DEVICE_TEMPLATE_FONA_800L_BASIC
  #define DEVICENAME_CTR          "testbed_rj45_gpio_toggle"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" "BLUE_WHITE_B_W" "\","
        "\"" "WHITE_BLUE_W_B" "\""
      "]"
    "}"
  "}";

  // #define INSERT_CODE_INIT
  // #define INSERT_CODE_EVERY_SECOND digitalWrite(4,!digitalRead(4)); digitalWrite(5,!digitalRead(5)); 

  // WILL ADD THIS toggle via rules (per second) in the future
  
#endif


/**
 * @brief 
 * Connected via header
 * 
 */
#ifdef DEVICE_TEMPLATE_LILYGO_SIM7000G
  #define DEVICENAME_CTR          "testbed_rj45_gpio_toggle"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_DRIVERS__CELLULAR_SIM7000
    // #define ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" "BLUE_WHITE_B_W" "\","
        "\"" "WHITE_BLUE_W_B" "\""
      "]"
    "}"
  "}";

  // #define INSERT_CODE_INIT
  #define INSERT_CODE_EVERY_SECOND digitalWrite(4,!digitalRead(4)); digitalWrite(5,!digitalRead(5)); 

  // WILL ADD THIS toggle via rules (per second) in the future
  
#endif




/**
 * New heating controller, designed to work from single device to multizone system
 * For development of hvac with pzem to monitor power
 * 
 * Taken from testbeds for insitu development and openhab integration, this will remain the primary hvac dev code (unless I use a 4 LED tester)
 * Long USB will be used for always debugging
 * 
 * To include:
 *  - BME? If I could place this on the plug into the wall, it would be elevated heat sensing? with 1m cable
 *  - Optional Ds18 that can be added to oil heater
 *  - Relay output (requiring level shifter)
 *  - PZEM at 5v, via level shifter
 *  - Power with mains internal? (possible danger with sensors?) -- 
 *  - Since pzem, this needs to esp32, but good practive for house hvac
 * 
 * Use two zones, so second zone can be a remote tester
 * 
 * 
 * Feb23 new box design
 * 
 * My own glands, 16A relays
 * Add OLED inside box (that can be viewed through it)
 * PZEM004T optional
 * ESP32 inside box
 * Use 3pin for DB18S20 external sensor as optional
 * May need shifter for relay and for PZEM (use cheap type for bidirectional and slow speed)
 * 
 * Metal button through box (immersion one)
 *  ** Each press adds 15 minutes
 *  ** hold turns it off again
 * 
 * Audio Jack screw 3pin to allow external DS18B20 sensor option
 * 
 * 
 * */
#ifdef DEVICE_TEMPLATE_HVAC_PORTABLE_WITHOUT_ENERGY //(later another with) 
  #define DEVICENAME_CTR          "hvac_template_hvac_portable"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Oil Radiator with Energy Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  //Add special debug method here that will count loglevel by type so "error" messages can be counted and shared every X seconds on terminal
  #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION
  #define ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS   
  
  #define USE_MODULE_SENSORS_INTERFACE  
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #define USE_MODULE_SENSORS_REMOTE_DEVICE

  #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "Bedroom"
  
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_HVAC
    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
    #define HEATING_DEVICE_MAX 2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_ENERGY_PZEM004T_V3
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"19\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"21\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"2\":\""  D_GPIO_FUNCTION_REL2_INV_CTR  "\""  // Using Builtin LED
      #endif  
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "DriverZone0"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "DriverZone1"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Zone0"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Zone1"

  #define D_DEVICE_SENSOR_ZONE_0_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_1_NAME REMOTE_SENSOR_JSON_NAME // tESTING REMOTE SENSOR VIA MQTT (LATER OPTIONS SHOULD INCLUDE DIRECT udp) "BME0"

  #define D_DEVICE_SENSOR_BME_0_NAME "BME0"
  #define D_DEVICE_SENSOR_BME_1_NAME "BME1"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Radiator"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,143,81,7,51,20,1,189]"

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Zone0"
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Power"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\""
      "],"      
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
      "],"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_ZONE_0_NAME "\","
        "\"" D_DEVICE_SENSOR_ZONE_1_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "}"
  "}";
  
#endif




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TEMPLATE__ESP8266_LOW_MEMORY
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "template_esp8266_low_memory"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_TEMPLATE__ESP8266_LOW_MEMORY


#ifdef DEVICE_EXAMPLE_TEMPLATE__433MHZ_SAW_RF
  #define DEVICENAME_CTR          "testbed_rcs_ext"
  #define DEVICENAME_FRIENDLY_CTR "Testbed 433MHz RCSwitch Extended"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define DEVICENAME_ROOMHINT_CTR "Example"

  // #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    #define ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif


#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_TEMPLATES_H

