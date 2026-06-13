#include "mCamera.h"

#ifdef USE_MODULE_DRIVERS__CAMERA

int8_t mCamera::Tasker(uint8_t function, JsonParserObject obj)
{
  // return 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
      return 1;
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }
  
  switch(function)
  {
    case TASK_LOOP:
      EveryLoop();
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___CAMERA
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___CAMERA
    #endif // USE_MODULE_CORE_FILESYSTEM
    case TASK_WEB_ADD_HANDLER:
    
      break;
    case TASK_WEB_ADD_MAIN_BUTTON:
      Web_AddStreamButton();
    break;
    case TASK_EVERY_SECOND:

      if(!psramFound())
      {  
        Serial.printf("esp_spiram_is_initialized(): %d\n", esp_spiram_is_initialized());
        Serial.printf("ESP.getPsramSize(): %d\n", ESP.getPsramSize());            
        AddLog(LOG_LEVEL_INFO, "PSRAM: Found=%d Useable=%d", SupportESP32::FoundPSRAM(), SupportESP32::UsePSRAM());
        AddLog(LOG_LEVEL_INFO, "Heap free: %d, PSRAM free: %d", ESP.getFreeHeap(), ESP.getFreePsram());
      }

      // AddLog(LOG_LEVEL_INFO, "PSRAM: Found=%d Useable=%d CanUsePSRAM=%d", 
      //   SupportESP32::FoundPSRAM(), 
      //   SupportESP32::UsePSRAM(), SupportESP32::CanUsePSRAM());
      //   AddLog(LOG_LEVEL_INFO, "Heap free: %d, PSRAM free: %d", 
      //     ESP.getFreeHeap(), 
      //     ESP.getFreePsram());

      // Bring up / tear down stream server based on network state
      if (!tkr_set->runtime.global_state.network_down) {
        if (tkr_iDrivers->webcam_config.stream && !rt.CamServer) {
          ALOG_INF(PSTR(D_LOG_CAMERA "Network up, starting stream server"));
          Stream_SetEnabled(1);
        }
      } else {
        if (rt.CamServer) {
          ALOG_INF(PSTR(D_LOG_CAMERA "Network down, stopping stream server"));
          Stream_SetEnabled(0);
        }
      }

                
      Stats_Update();
    break;
    case TASK_UPTIME_30_SECONDS:
    // case TASK_NETWORK_CONNECTED__WIFI:
      ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "TASK_UPTIME_30_SECONDS"));

      tkr_iDrivers->webcam_config.stream=1;
      ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "\n\r\t\t\tCAM: Stream_SetEnabled STARTED"));
      Stream_SetEnabled(tkr_iDrivers->webcam_config.stream);

    break;
    case TASK_UPDATE_OTA_BEFORE_ON_START:

      ALOG_INF(PSTR(D_LOG_CAMERA "Suspending webcam task before OTA update"));
      // vTaskSuspend(rt.taskHandle);
      // esp_camera_deinit(); // gracefully release memory from the driver
      // rt.up = false;

      // vTaskDelete(rt.taskHandle);
      // rt.taskHandle = nullptr;
      Driver_SuspendForOTA();
      ALOG_INF(PSTR(D_LOG_CAMERA "Suspended webcam task before OTA update"));

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      // tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;

} // END function


#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___CAMERA

void mRelays::Save_Module()
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Save_Module"));
  tkr_mfile->ByteFile_Save("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

void mRelays::Load_Module(bool erase)
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Load_Module"));
  tkr_mfile->ByteFile_Load("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___CAMERA
#endif // USE_MODULE_CORE_FILESYSTEM


#endif // USE_MODULE_DRIVERS__CAMERA
