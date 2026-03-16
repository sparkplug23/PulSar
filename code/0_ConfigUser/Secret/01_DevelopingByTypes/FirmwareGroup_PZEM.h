#ifndef _CONFIG__FIRMWARE_GROUP__PZEM_H
#define _CONFIG__FIRMWARE_GROUP__PZEM_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are unique with pzem style
 *  ** So not the consumer unit, but standalone basic PZEM devices that may have additional sensors but are many for 
 *     tracking or controller power (eg treadmill, christmas lights)
 *  ** Will include 100A and 10A variants.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

//    ;;;;;;;;;;;; ESP32 ;;;;;;;;;;;;;;;;
// #define DEVICE_TESTGROUP__PZEM__SOCKET_ENERGY_01 // for use with outdoor christmas lights



//----------------------------------------


/************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************
  @brief TESTGROUP__LIGHTING
 ************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************/






#ifdef DEVICE_TESTGROUP__PZEM__SOCKET_ENERGY_01
  #define DEVICENAME_CTR          "socket_energy_01"
  #define DEVICENAME_FRIENDLY_CTR "hvac_oil_radiator #1"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

    #define ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT

    #define USE_MODULE_CORE_FILESYSTEM
    
  /***********************************
   * SECTION: System Debug Options
  ************************************/  

 
  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME

  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 
  /***********************************
   * SECTION: Storage Configs
  ************************************/  


  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  
  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    #define USE_MODULE_SENSORS_BME
  #endif

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    

  // #define USE_MODULE_SENSORS_DS18X20

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
 
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
 
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

 
  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
        
  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
    #define USE_MODULE_ENERGY_INTERFACE
  #endif
  
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
    #define USE_MODULE_ENERGY_PZEM004T_V3
      #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
    #define MAX_ENERGY_SENSORS 1
    #define MAX_PZEM004T_DEVICES 17
    #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  // #define ENABLE_DEBUGFEATURE_RELAY__TEMP_FORCE_ON_FOR_5_MINS

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{" 
      #ifdef USE_MODULE_ENERGY_PZEM004T_V3
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #endif
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"27\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"26\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_REL2_CTR  "\"" // use LED as temporary relay tester
      #endif  

    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  /***********************************
   * SECTION: TEMPLATE: Names
  ************************************/    

  #define D_DEVICE_UNIQUE_NAME "Socket"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"
  #define D_DEVICE_SENSOR_ZONE_0_NAME "OilRadiator01-BME0"
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Socket"
  
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Radiator"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,143,81,7,51,20,1,189]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"      
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_ZONE_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";


#endif

#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE_TESTGROUPS_BETA_H

