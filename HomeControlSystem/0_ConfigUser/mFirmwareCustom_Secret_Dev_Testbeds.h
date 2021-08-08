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
**/
// #define DEVICE_TESTBED_SHELLY1_01
// #define DEVICE_TESTBED_SHELLY2_DIMMER
#define DEVICE_TESTBED_SHELLY2P5_01
// #define DEVICE_ESP32_DEVKIT_BASIC
// #define DEVICE_ESP32_WEBCAM1

// Include the home devices, which should ONLY contain final hardware
#include "0_ConfigUser/mFirmwareCustom_Secret_Home.h"


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Device Type: Shelly 2.5
 * Modules:   
 *  - 2 Mains Relays
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

  #define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_PRESENCE_DETECTION_TRIGGERING_TIMED_OUTPUTS

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Test"

  // #define ENABLE_DEVFEATURE_RULES_MQTT_FASTER_SECS

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */

  //{"NAME":"Shelly 2.5","GPIO":[
    
  /*
  320,0,32,0,224,193,0,0,640,192,608,225,3456,4736
  */
  //],"FLAG":0,"BASE":18}
  // #define USE_MODULE_SENSORS_PRESENCE   // to be added

  // #define USE_MODULE_ENERGY_INTERFACE
  // #define USE_MODULE_ENERGY_ADE7953

  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
 // #define USE_MODULE_SENSORS_MOTION   //phasing out??

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2
  #define USE_MODULE_DRIVERS_INTERFACE

  // #define ENABLE_DEVFEATURE_RELAY_TIME_SCHEDULED_DEFAULT_ON  
    
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
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Switch1 Toggle = Relay1 Power Toggle
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":1,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":1,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the presence detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
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
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_PRESENCE_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // 
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Motion\","
        "\"Function\":\"MotionStarted\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":1,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Motion\","
        "\"Function\":\"MotionStarted\","
        "\"DeviceName\":1,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":1,"
        "\"State\":1,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_PRESENCE_DETECTION_TRIGGERING_TIMED_OUTPUTS



  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
  //       "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
  //       "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "},"
  //   "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D15:45:00\",\"OffTime\":\"00D00:00:00\"},"
  //   "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
  // "}";

  // #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"StateChanged\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "}"
  //   "},"
  //   "\"Rule1\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"StateChanged\","
  //       "\"DeviceName\":1,"
  //       "\"State\":2"
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "}"
  //   "}"
  // "}";

//   // #define ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING
//   #define LOG_BUFFER_SIZE  1000

//   #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     "\"Rule0\":{" //switch example
//       "\"Trigger\":{"
//       //module_id
//         "\"Module\":\"Motion\","    //sensor
//         //?
//         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
//         //[1]
//         "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
//         //[0]
//         "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         // set power?
//         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
//         //1
//         "\"DeviceName\":0," //number, name, or all
//         //2
//         "\"State\":1,"//setpower
//         // Append other "normal" commands here? this would need storing
//         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
//         "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""

//         //relay:timeon needs a way to specify which device
        
//         // {"PowerName":0,"PowerState":1,"Relay":{"TimeOn":5}}


//         //relay 
//       "}"
//     "},"
//     "\"Rule1\":{" //switch example
//       "\"Trigger\":{"
//       //module_id
//         "\"Module\":\"Motion\","    //sensor
//         //?
//         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
//         //[1]
//         "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
//         //[0]
//         "\"State\":1" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         // set power?
//         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
//         //1
//         "\"DeviceName\":1," //number, name, or all
//         //2
//         "\"State\":1,"//setpower
//         // Append other "normal" commands here? this would need storing
//         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
        
//         // "\"JsonCommands\":\"{\\\"PowerName\\\":5,\\\"Relay\\\":{\\\"TimeOn\\\":12}}\""
//         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"{\\\\\"PowerName\\\\\":0,\\\\\"PowerState\\\\\":2}\\\"}}\""
//         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"hello\\\"}\""
//         "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight4/set/relays\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2,~TimeOn~:10}\\\"}}\""
 
// //  {"MQTTSend":
// //   {
// //     "Topic":"kitchenlight5/set/relay",
// //     "Payload":
// //       "{"PowerName":0,"PowerState":2}"
// //   }
// // }




//         // "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
//         //relay 
//       "}"
//     "}"
    

//   "}";




#endif



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif


#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

