#include "mSPI.h"

#ifdef USE_MODULE_CORE_I2C

int8_t mSPI::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      // Pre_Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  // switch(function){
  //   /************
  //    * PERIODIC SECTION * 
  //   *******************/
  //   case TASK_EVERY_SECOND:  
  //   break;
  //    case TASK_LOOP:
  //   //   UfsExecuteCommandFileLoop();
  //   break;
  //   case TASK_EVERY_FIVE_SECOND:    
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //   break;
  //   case TASK_EVERY_MINUTE:
  //     // #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA // This will in the future only occur once an hour, or before planned boot
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     // #endif     
  //   break;  
  //   case TASK_EVERY_FIVE_MINUTE:
  //     #ifdef ENABLE_SYSTEM_SETTINGS_IN_I2C
  //     JsonFile_Save__Stored_Module();
  //     JsonFile_Save__Stored_Secure();
  //     #endif // ENABLE_SYSTEM_SETTINGS_IN_I2C
       
  //   break;
  //   /************
  //    * COMMANDS SECTION * 
  //   *******************/
  //   case TASK_JSON_COMMAND_ID:
  //     parse_JSONCommand(obj);
  //   break;
  //   /************
  //    * MQTT SECTION * 
  //   *******************/
  //   #ifdef USE_MODULE_NETWORK_MQTT
  //   case TASK_MQTT_HANDLERS_INIT:
  //     MQTTHandler_Init();
  //   break;
  //   case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
  //     MQTTHandler_Rate();
  //   break;
  //   case TASK_MQTT_SENDER:
  //     MQTTHandler_Sender();
  //   break;
  //   case TASK_MQTT_CONNECTED:
  //     MQTTHandler_RefreshAll();
  //   break;
  //   #endif //USE_MODULE_NETWORK_MQTT

  //   // case TASK_MQTT_INIT:
  //   //   if (!TasmotaGlobal.no_autoexec) {
  //   //     UfsExecuteCommandFile(TASM_FILE_AUTOEXEC);
  //   //   }
  //   //   break;
  //   // case TASK_COMMAND:
  //   //   result = DecodeCommand(kUFSCommands, kUFSCommand);
  //   //   break;
  //   #ifdef USE_WEBSERVER
  //   case TASK_WEB_ADD_MANAGEMENT_BUTTON:
  //     if (ufs_type) {
  //     WSContentSend_PD(UFS_WEB_DIR, PSTR(D_MANAGE_FILE_SYSTEM));
  //     }
  //     break;
  //     case TASK_WEB_ADD_HANDLER:
  //     //      Webserver->on(F("/ufsd"), UfsDirectory);
  //     //      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
  //     //      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
  //     Webserver->on("/ufsd", UfsDirectory);
  //     Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
  //     Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
  //     break;
  //   #endif // USE_WEBSERVER

  //   }  

} // END Tasker



#endif // USE_MODULE_CORE_I2C
