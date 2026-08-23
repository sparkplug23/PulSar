#ifndef _TEMPLATE__HARDWARE_SPECIFIC_H
#define _TEMPLATE__HARDWARE_SPECIFIC_H

/**
 * @brief 
 * This file contains templates for the hardware specific modules.
 * These are for consumer bought devices, ie sonoff and shelly
 * No specialisation should be changed in here, all defines are in the ini file
 */



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

// #define DEVICE_TEMPLATE_SONOFF_IFAN03
// #define DEVICE_TEMPLATE_SONOFF_BASIC_R2
// #define DEVICE_TEMPLATE_SHELLY_DIMMER_V2
// #define DEVICE_TEMPLATE_SONOFF_4CHPRO_V3

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Device Defines *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_TEMPLATE_SONOFF_IFAN03
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    
    #define MQTT_PORT     1883
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE__DRIVERS_BUZZER_BASIC
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE) // temp solution, the unedefined relay should be handled by GetDeviceName to add the unique index and not random
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"Button\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"Relay1\","
        "\"Relay2\","
        "\"Relay3\","
        "\"Relay4\""
      "]"
    "}"
  "}";


#endif



#ifdef DEVICE_TEMPLATE_SONOFF_BASIC_R2
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    
    #define MQTT_PORT     1883


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  
  #define USE_MODULE_TEMPLATE_SONOFF_BASIC_R2

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR  "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"Button\""
      "]"
    "}"
  "}";
   
  // Default Rule Defined (DefaultRule_Sonoff_Basic_R2)

#endif


#ifdef DEVICE_TEMPLATE_SHELLY_DIMMER_V2
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    
    #define MQTT_PORT     1883

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Stairs"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Landing"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
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
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\"," 
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"   
  "}";

#endif



#ifdef DEVICE_TEMPLATE_SONOFF_4CHPRO_V3
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_sonoff_4chpro"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Sonoff 4CH Pro"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Sonoff 4CH Pro"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    
    #define MQTT_PORT     1883
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_DRIVERS_LEDS
  #define USE_MODULE_DRIVERS_RELAY
  // #define USE_MODULE_DRIVERS_RF433_CODES

  // default key# = relay#
  // RF Key433 using rules to match patterns then need limit to how often RF matches 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_4CHPRO_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // {"RfReceived":{"Data":11071155,"Bits":24,"Protocol":1,"Pulse":191,"millis":83687314,"Time":"18:48:02"}} // black doorbell

#endif


#ifdef DEVICE_TEMPLATE__ESP8285__IFAN03
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "tg_boardbuild__esp8285_ifan03"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    
    #define MQTT_PORT     1883
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define ESP8266

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE__DRIVERS_BUZZER_BASIC
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE) // temp solution, the unedefined relay should be handled by GetDeviceName to add the unique index and not random
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"Button\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"Relay1\","
        "\"Relay2\","
        "\"Relay3\","
        "\"Relay4\""
      "]"
    "}"
  "}";


#endif



/**
 * @brief 
 * 
 * HVAC X1
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13| RADAR_OUT_MOT4
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *         PIR_SMALL_MOT2  |5  (fL)            25| TOF1INT
 *                    aud  |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ_MOT3
 *                         |RX0         (fL) * 34| PIR_LARGE_MOT1
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_TEMPLATE__ESP32__30A_RELAY_BOARD_CZZL
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_30a_relay_board_czzl"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template 30A Relay Board CZZL"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template 30A Relay Board CZZL"
  #endif
                 
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY  
  #define USE_MODULE_DRIVERS_LEDS

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"2\":\""  D_GPIO_REL1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"0\":\""  D_GPIO_SWT1_INV_CTR  "\","
      #endif
      "\"4\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INTERFACE_CTR"\":["
        "\"" "System" "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" "Relay" "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" "Button" "\""
      "]"
    "},"    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}"
  "}";
  
#endif




#endif // _TEMPLATE__HARDWARE_SPECIFIC_H
