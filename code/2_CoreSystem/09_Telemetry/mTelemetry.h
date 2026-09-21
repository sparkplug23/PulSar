#ifndef _MTELEMETRY_H_
#define _MTELEMETRY_H_

#define D_UNIQUE_MODULE_CORE_TELEMETRY_ID 2009 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/20_JSON/mJSON.h"
#include "3_Network/10_MQTT/mMQTT.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include <Ticker.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#ifdef USE_MODULE_NETWORK_WEBSERVER
#include "3_Network/21_WebServer/_WebServer.h"
#endif

#include "1_TaskerManager/mTaskerInterface.h"

#include "2_CoreSystem/09_Telemetry/Web/Gen/generated_web.h"

#ifndef SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY
#define SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY 600
#endif





class mTelemetry :
  public mTaskerInterface
{
  public:
    mTelemetry(){};    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    uint8_t serial_messages_remaining_to_send = 0;
    
    static constexpr const char* PM_MODULE_CORE_TELEMETRY_CTR = D_MODULE__CORE__TELEMETRY__CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_TELEMETRY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TELEMETRY_ID; }

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    void Serve_Web_Telemetry(AsyncWebServerRequest* request);
    void Serve_Web_Telemetry_Page(AsyncWebServerRequest* request);
    void Serve_Web_Telemetry_JS(AsyncWebServerRequest* request);
    #endif


    // Centralized template function
    template <typename HandlerType>
    void Telemetry_RefreshAll(std::vector<struct telemetry_handler<HandlerType>*>& handler_list) 
    {
      for (auto& handle : handler_list) {
          handle->flags.SendNow = true;
      }
    }

    // Centralized template function
    template <typename HandlerType>
    void Telemetry_Rate(std::vector<struct telemetry_handler<HandlerType>*>& handler_list) 
    {
      for (auto& handle : handler_list) 
      {
        if (handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
          handle->tRateSecs = 120;//GetTelePeriod_SubModule();
        else if (handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
          handle->tRateSecs = 10;//GetIfChangedPeriod_SubModule();
      }
    }


   
    uint8_t ConstructJSON_LWT_Online(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Health(uint8_t json_method = 0, bool json_appending = true);
    #ifndef FIRMWARE_MINIMAL2
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_System(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Network(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Drivers(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Sensors(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Lights(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Power(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Rules(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_Runtime(uint8_t json_method = 0, bool json_appending = true);
      uint8_t ConstructJSON_Settings_TextBuffer(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Peripherals(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Firmware(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Log(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Memory(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Network(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_MQTT(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Time(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Reboot(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_TaskerManager(uint8_t json_method = 0, bool json_appending = true);
    #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
    uint8_t ConstructJSON_Debug_Pins_GPIO(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Pins_Table(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Template(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_method = 0, bool json_appending = true);
      #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
      uint8_t ConstructJSON_Debug__Settings_Storage(uint8_t json_method = 0, bool json_appending = true);
      #endif
    #endif
    #endif // FIRMWARE_MINIMAL2

    #ifdef ENABLE_MQTT_SETTINGS_TELEMETRY
    #endif




    #ifdef USE_MODULE_NETWORK_MQTT    
      void Telemetry_Init();      
      std::vector<struct telemetry_handler<mTelemetry>*> telemetry_list;    
      telemetry_handler<mTelemetry> telemetry_lwt_online;
      telemetry_handler<mTelemetry> telemetry_health;
      #ifndef FIRMWARE_MINIMAL2
      telemetry_handler<mTelemetry> telemetry_settings;
      telemetry_handler<mTelemetry> telemetry_settings_system;
      telemetry_handler<mTelemetry> telemetry_settings_network;
      telemetry_handler<mTelemetry> telemetry_settings_drivers;
      telemetry_handler<mTelemetry> telemetry_settings_sensors;
      telemetry_handler<mTelemetry> telemetry_settings_lights;
      telemetry_handler<mTelemetry> telemetry_settings_power;
      telemetry_handler<mTelemetry> telemetry_settings_rules;
      telemetry_handler<mTelemetry> telemetry_settings_runtime;
      telemetry_handler<mTelemetry> telemetry_settings_text_buffer;
      telemetry_handler<mTelemetry> telemetry_log;
      telemetry_handler<mTelemetry> telemetry_peripherals;
      telemetry_handler<mTelemetry> telemetry_firmware;
      telemetry_handler<mTelemetry> telemetry_memory;
      telemetry_handler<mTelemetry> telemetry_network;
      telemetry_handler<mTelemetry> telemetry_mqtt;
      telemetry_handler<mTelemetry> telemetry_time;
      telemetry_handler<mTelemetry> telemetry_taskermanager;
      telemetry_handler<mTelemetry> telemetry_reboot;
      telemetry_handler<mTelemetry> telemetry_reboot_event;
      #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
        telemetry_handler<mTelemetry> telemetry_debug_pins_gpio;
        telemetry_handler<mTelemetry> telemetry_debug_pins_table;
        telemetry_handler<mTelemetry> telemetry_debug_template;
        #if defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES) || defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_PERFORMANCE)
        telemetry_handler<mTelemetry> telemetry_debug_tasker_interface_performance;
        #endif
        #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
        telemetry_handler<mTelemetry> telemetry_debug__settings_storage;
        #endif
      #endif // ENABLE_MQTT_DEBUG_TELEMETRY
      #endif // FIRMWARE_MINIMAL2
    
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif  // _MTELEMETRY_H_
