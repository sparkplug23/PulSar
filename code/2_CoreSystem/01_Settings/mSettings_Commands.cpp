
#include "2_CoreSystem/01_Settings/mSettings.h"


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



  /**
   * Debug options
   * */
  if(jtok = obj["SettingSave"])
  {
    SettingsSaveAll();
  }
  if(jtok = obj["SettingLoad"])
  {
    SettingsLoad();
  }



  // ENABLE_DEVFEATURE_SAVE_REBOOT_COMMAND_FOR_SETTINGS_TESTING


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
      ALOG_DBM( PSTR("CHECKING module_friendlyname_buffer = %s"),module_friendlyname_buffer); 
      #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS    
  
      if(jtok = obj[PM_JSON_DEVICENAME].getObject()[module_friendlyname_buffer]){ 
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        ALOG_DBM( PSTR("found module_friendlyname_buffer = %s"),module_friendlyname_buffer); 
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
            ALOG_DBM( PSTR(D_LOG_RELAYS "device_name_ctr = %s"),device_name_ctr); 
            ALOG_DBM( PSTR(D_LOG_RELAYS "device_count = %d"),device_count);  
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


// {"ModuleName":255,

// "DeviceID":[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8],

// "ClassID":[12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,130,130,130,130,130,130,130,130,130,130,130,130,132,132,132,132,132,132,132,132,132,132,132,132,143,132,0,0,0,0,0,0,0,0],


// "Buffer":"User Test 00|User Test 01|User Test 02|User Test 03|User Test 04|User Test 05|User Test 06|User Test 07|User Test 08|User Test 09|Solid Rgbcct 10|Solid Rgbcct 11|Solid Rgbcct 12|Solid Rgbcct 13|Solid Rgbcct 14|Encoded 15|Mains|Cooker|Immersion|Washing Machine|Dishwasher|Pump Shower|Heating|Tumble Dryer|Garage|Bathroom Shower|Main Sockets|Kitchen Sockets|Mains|Cooker|Immersion|Washing Machine|Dishwasher|Pump Shower|Heating|Tumble Dryer|Garage|Bathroom Shower|Main Sockets|Kitchen Sockets|Downstairs Toilet|Downstairs Toilet|","I2C_Scan":{"I2CScan":"not on esp32 yet"}}




  
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
      //MQTTHandler_Set_RefreshAll();
      data_buffer.isserviced++;
    }
    else
    if(strstr(command,PSTR("reset_bootcount"))){ 
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SETTINGS D_PARSING_MATCHED "\"command\"=\"reset_bootcount\""));
      Settings.bootcount = 0;
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("Resetting Settings.bootcount"));
      SettingsSaveAll();
      data_buffer.isserviced++;
    }
    else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_SETTINGS D_PARSING_NOMATCH));
    }
  }

  
  #endif

  JsonParserToken jtok_sub = 0; 
  if(jtok = obj["MQTTUpdateSeconds"])
  {
    if(jtok_sub = jtok.getObject()["IfChanged"])
    {
      Settings.sensors.ifchanged_secs = jtok_sub.getInt();
      AddLog(LOG_LEVEL_TEST, PSTR("MQTTUpdateSeconds IfChanged %d"),Settings.sensors.ifchanged_secs);
    }
    if(jtok_sub = jtok.getObject()["TelePeriod"])
    {
      Settings.sensors.teleperiod_secs = jtok_sub.getInt();
      AddLog(LOG_LEVEL_TEST, PSTR("MQTTUpdateSeconds TelePeriod %d"),Settings.sensors.teleperiod_secs);
    }
    if(jtok_sub = jtok.getObject()["ConfigPeriod"])
    {
      Settings.sensors.configperiod_secs = jtok_sub.getInt();
      AddLog(LOG_LEVEL_TEST, PSTR("MQTTUpdateSeconds ConfigPeriod %d"),Settings.sensors.configperiod_secs);
    }
    pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD);
  }

  if(jtok = obj["BootCount"])
  {
    Settings.bootcount = jtok.getInt();
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("BootCount %d"),Settings.bootcount);
  }




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
   * 2 = save reboot, save settings, then restart
   * 3 = reboot into captive portal mode
   * 4 = reboot and wait for 1 minute for OTA capture
   * 5 = force crash with hardware WDT
   * 6 = reboot and reset to default setting (not wifi)
   * 7 (requires second command) = forced reset and all settings including wifi
   * 3+ is disabling things for stability ie fastboot or erasing settings to default
   * 
   * 
   * */

  // animation.transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
  // animation.transition.pixels_to_update_as_percentage = value;

#ifdef USE_MODULE_NETWORK_WIFI
   
  if(value == 1){
    
#ifndef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
    pCONT_wif->EspRestart();
#endif // ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

  }else

  if(value == 2){
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("REBOOT TEST" DEBUG_INSERT_PAGE_BREAK));
    #endif 

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("Current bootcount is %d"), Settings.bootcount);
    #endif //  ENABLE_LOG_LEVEL_INFO

    pCONT_set->TestSettings_ShowLocal_Header();
    pCONT_set->TestSettingsLoad();

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("Modying bootcount to %d"), Settings.bootcount++);
#endif 

    pCONT_set->SettingsSaveAll();

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("Settings should be saved now to %d"), Settings.bootcount);
#endif

    pCONT_set->TestSettings_ShowLocal_Header();
    pCONT_set->TestSettingsLoad();

    // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_wif->EspRestart(); is 5 seconds"));

    // delay(5000);


#ifndef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
    pCONT_wif->EspRestart();
#endif // ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
  }
  
  #endif // ifdef USE_MODULE_NETWORK_WIFI
   
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
  // #endif

}



