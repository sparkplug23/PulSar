#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_TB_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_TB_H

/*********************************************************************************************\
 * Testbeds: Development devices to get specialised hardware working in their default minimum/basic operation
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
#define DISABLE_WEBSERVER
#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

/**
 *  TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- 
 * Special hardware, shelly, sonoff
**/
// #define DEVICE_TESTBED_SHELLY1_01
// #define DEVICE_TESTBED_SHELLY2P5_01
// #define DEVICE_TESTBED_SHELLY2_DIMMER 

/**
 * Generic hardware eg nodemcu
 * */
// #define DEVICE_TESTBED_MOTION

// #define DEVICE_ESP32_DEVKIT_BASIC
// #define DEVICE_ESP32_WEBCAM1

// Include the home devices, which should ONLY contain final hardware
#include "0_ConfigUser/mFirmwareCustom_Secret_Home.h"


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
#ifdef DEVICE_TESTBED_SHELLY2P5_01
  #define DEVICENAME_CTR          "testbed_shelly25_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly 2.5 #01"

  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power0"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Power1"
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
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D20:01:00\",\"OffTime\":\"00D05:00:00\"}"
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


#endif // DEVICE_NAME



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
#ifdef DEVICE_TESTBED_SHELLY1_01
  #define DEVICENAME_CTR          "testbed_shelly1_01"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Tester"

  #define FORCE_TEMPLATE_LOADING
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

  #define USE_MODULE_NETWORKS_MQTT

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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\""
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
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"}"
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


#ifdef DEVICE_TESTBED_SHELLY2_DIMMER
  #define DEVICENAME_CTR          "testbed_shellydimmer_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly Dimmer 2"
  
  #define FORCE_TEMPLATE_LOADING
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

/**
 * Testing motion triggers and rules
 * PIR and doppler, door locks 
 * */
#ifdef DEVICE_TESTBED_MOTION
  #define DEVICENAME_CTR            "testbed_motion"
  #define DEVICENAME_FRIENDLY_CTR   "testbed_motion ESP8266"
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR "\","
      "\"D7\":\"" D_GPIO_FUNCTION_SWT2_CTR "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Motion0"
  #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "Motion1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
    
  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
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
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "}"
  "}";

#endif



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif


#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

