
#include "2_CoreSystem/Settings/mSettings.h"


void mSettings::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  /**
   * Add command that will cause templates to be loaded overwriting settings, then commit to saving

   */
  
  if(jtok = obj[PM_JSON_SYSTEM_RESTART]){
    // if(jtok.isStr()){
    //   if((tmp_id=GetPixelHardwareTypeIDbyName(jtok.getStr()))>=0){
    //     CommandSet_PixelHardwareTypeID(tmp_id);
    //     data_buffer.isserviced++;
    //   }
    // }else
    if(jtok.isNum()){
      CommandSet_SystemRestartID(jtok.getInt());



      data_buffer.isserviced++;
    }
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }


  if(jtok = obj[PM_JSON_DEVICENAME]){ 
    // const char* onoff = jtok.getStr();
    
    // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK D_PARSING_MATCHED "%s %s"), F(D_JSON_DEVICENAME),onoff); 
    // #endif // LOG_LEVEL_COMMANDS

    char module_friendlyname_buffer[30];
    uint16_t module_id = 0;

    // Search across all module names
    for(int module_list_id=0;module_list_id<pCONT->GetClassCount();module_list_id++){

      module_id = pCONT->GetModuleUniqueIDbyVectorIndex(module_list_id);
      
      // module_list_id;//pCONT->mTasksIDs[module_list_id];
      // module_settings.list[module_list_id];

      sprintf_P(module_friendlyname_buffer,"%S",pCONT->GetModuleFriendlyName(module_list_id));
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_DEBUG, PSTR("CHECKING module_friendlyname_buffer = %s"),module_friendlyname_buffer); 
      #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS    
  
      if(jtok = obj[PM_JSON_DEVICENAME].getObject()[module_friendlyname_buffer]){ 
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_TEST, PSTR("found module_friendlyname_buffer = %s"),module_friendlyname_buffer); 
        #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
        
        JsonParserArray arr = obj[PM_JSON_DEVICENAME].getObject()[module_friendlyname_buffer];
        if(arr){  
          //Get devices already present
          uint8_t device_count = DLI->GetDeviceNameCount(module_id);
          for(uint8_t id =0;id<arr.size();id++){
            jtok = arr[id];
            const char* device_name_ctr = jtok.getStr();
            // DLI->AddDeviceName(device_name_ctr,module_id,device_count++);
            DLI->AddDeviceName(device_name_ctr,module_id,device_count++);
            #ifdef ENABLE_LOG_LEVEL_COMMANDS
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "device_name_ctr = %s"),device_name_ctr); 
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "device_count = %d"),device_count);  
            #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
          } //if array
        }//if array
      }
    } //for


  }
  
  // if(!obj[D_JSON_WIFI_AP].isNull()){
  //   uint8_t ap = obj[D_JSON_WIFI_AP];    
  //   AddLog(LOG_LEVEL_INFO, PSTR("MATCHED D_JSON_WIFI_AP %d"),ap); 
        
  //   pCONT_wif->WifiBegin(ap);
  // }



  
#ifdef ENABLE_BUFFER_STRUCT
  int8_t tmp_id = 0;

  // #ifdef JSONDOCUMENT_STATIC
  //   StaticJsonDocument<800> doc;
  // #else
  //   DynamicJsonDocument doc(600);
  // #endif
  // DeserializationError error = deserializeJson(doc, data_buffer_old.payload.ctr);
  
  // if(error){
  //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();
  
  if(!obj[F("command")].isNull()){ 
    const char* command = obj[F("command")];
    if(strstr(command,PSTR("system_send_all"))){ 
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SETTINGS D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
      //MQTTHandler_Set_fSendNow();
      data_buffer.isserviced++;
    }
    else
    if(strstr(command,PSTR("reset_bootcount"))){ 
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SETTINGS D_PARSING_MATCHED "\"command\"=\"reset_bootcount\""));
      Settings.bootcount = 0;
      SettingsSaveAll();
      data_buffer.isserviced++;
    }
    else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_SETTINGS D_PARSING_NOMATCH));
    }
  }

  
  #endif

  /*
  
//make mqtt commands to allow me to tweak and debug 
  Settings.sensors.ifchanged_secs = 10;
  Settings.sensors.ifchanged_json_level = JSON_LEVEL_IFCHANGED; //default
  Settings.sensors.teleperiod_secs = 120;
  Settings.sensors.teleperiod_json_level = JSON_LEVEL_DETAILED; //default
  Settings.sensors.flags.mqtt_retain = 1;// = JSON_METHOD_SHORT; //default
  Settings.sensors.configperiod_secs = SEC_IN_HOUR;


*/

#ifdef ENABLE_BUFFER_STRUCT
  u
  int8_t tmp_id = 0;

  // // #ifdef JSONDOCUMENT_STATIC
  // //   StaticJsonDocument<800> doc;
  // // #else
  //   DynamicJsonDocument doc(300);
  // // #endif
  // DeserializationError error = deserializeJson(doc, data_buffer_old.payload.ctr);

  // //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"parsesub_FirmwareInformation\"=\"%s\""),data_buffer_old.payload.ctr);
  
  // if(error){
  //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();
  
  // if(!obj["latest"]["parts"]["major"].isNull()){ 
  //   firmware_version.latest.part_major = obj["latest"]["parts"]["major"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"latest.parts.major\"=\"%d\""),firmware_version.latest.part_major);
  // }
  // if(!obj["latest"]["parts"]["minor"].isNull()){ 
  //   firmware_version.latest.part_minor = obj["latest"]["parts"]["minor"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"latest.parts.minor\"=\"%d\""),firmware_version.latest.part_minor);
  // }
  // if(!obj["latest"]["parts"]["system"].isNull()){ 
  //   firmware_version.latest.part_system = obj["latest"]["parts"]["system"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"latest.parts.system\"=\"%d\""),firmware_version.latest.part_system);
  // }
  // if(!obj["latest"]["parts"]["module"].isNull()){ 
  //   firmware_version.latest.part_module = obj["latest"]["parts"]["module"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"latest.parts.module\"=\"%d\""),firmware_version.latest.part_module);
  // }


  // if(!obj["lowest"]["parts"]["major"].isNull()){ 
  //   firmware_version.lowest.part_major = obj["lowest"]["parts"]["major"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"lowest.parts.major\"=\"%d\""),firmware_version.lowest.part_major);
  // }
  // if(!obj["lowest"]["parts"]["minor"].isNull()){ 
  //   firmware_version.lowest.part_minor = obj["lowest"]["parts"]["minor"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"lowest.parts.minor\"=\"%d\""),firmware_version.lowest.part_minor);
  // }
  // if(!obj["lowest"]["parts"]["system"].isNull()){ 
  //   firmware_version.lowest.part_system = obj["lowest"]["parts"]["system"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"lowest.parts.system\"=\"%d\""),firmware_version.lowest.part_system);
  // }
  // if(!obj["lowest"]["parts"]["module"].isNull()){ 
  //   firmware_version.lowest.part_module = obj["lowest"]["parts"]["module"];
  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"lowest.parts.module\"=\"%d\""),firmware_version.lowest.part_module);
  // }

//  CheckVersion - newer ready?


  // if( 
  //   (firmware_version.current.part_major<=firmware_version.latest.part_major)&&
  //     (firmware_version.current.part_minor<firmware_version.latest.part_minor)
  //     #ifdef USE_FIRMWARE_VERSION_CHECK_INCREMENTS
  //     &&(firmware_version.current.part_system<firmware_version.latest.part_system)
  //     &&(firmware_version.current.part_module<firmware_version.latest.part_module)
  //     #endif
  //   ){
  //     firmware_version.fNewVersionAvailable = true;    
  //   }else{
  //     firmware_version.fNewVersionAvailable = false;    
  //   }
    
    if(firmware_version.current.number<firmware_version.latest.number){
      firmware_version.fNewVersionAvailable = true;    
    }else{
      firmware_version.fNewVersionAvailable = false;    
    }



    //  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"firmware_version.fNewVersionAvailable\"=\"%d\""),firmware_version.fNewVersionAvailable);
  

// version less than lowest supported
  if( 
    (firmware_version.current.part_major<=firmware_version.lowest.part_major)&&
      (firmware_version.current.part_minor<firmware_version.lowest.part_minor)
      #ifdef USE_FIRMWARE_VERSION_CHECK_INCREMENTS
      &&(firmware_version.current.part_system<firmware_version.lowest.part_system)
      &&(firmware_version.current.part_module<firmware_version.lowest.part_module)
      #endif
    ){
      firmware_version.fCurrentVersionNotSupported = true;    
    }else{
      firmware_version.fCurrentVersionNotSupported = false;    
    }


      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED "\"firmware_version.fCurrentVersionNotSupported\"=\"%d\""),firmware_version.fCurrentVersionNotSupported);
  
  






  

  #endif




}




/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_SystemRestartID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSettings::CommandSet_SystemRestartID(uint8_t value){

  /**
   * 0 = invalid
   * 1 = soft restart
   * 2 = hardware wdt restart
   * 3+ is disabling things for stability ie fastboot or erasing settings to default
   * 
   * 
   * */

  // animation.transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
  // animation.transition.pixels_to_update_as_percentage = value;

#ifdef USE_MODULE_NETWORK_WIFI
   
  if(value == 1){
    pCONT_wif->EspRestart();
  }
  #endif // ifdef USE_MODULE_NETWORK_WIFI
   
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
  // #endif

}



