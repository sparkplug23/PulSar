#include "mRuleEngine.h"

// /* Null, because instance will be initialized on demand. */
// RuleEngine* RuleEngine::instance = nullptr;

// RuleEngine* RuleEngine::GetInstance(){
//   if (instance == nullptr){
//     instance = new RuleEngine();
//     // PRINT_FLUSHED("instance = new RuleEngine()");
//   }
//   return instance;
// }


int8_t mRuleEngine::Tasker(uint8_t function){

    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT ));

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
    //   Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
//   if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    // /************
    //  * INIT SECTION * 
    // *******************/
    // case FUNC_INIT:
    //   init();
    // break;
    // /************
    //  * SETTINGS SECTION * 
    // *******************/
    // case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   // Settings_Load();
    // break;
    // case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   // Settings_Save();
    // break;
    // case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    //   // Settings_Default();
    // break;
    // case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   // Settings_Default();
    //   // pCONT_set->SettingsSave(2);
    // break;
    // /************
    //  * PERIODIC SECTION * 
    // *******************/
    // case FUNC_LOOP: 
    //   EveryLoop();
    // break;  
    // case FUNC_EVERY_SECOND:
    //   AddLog_P(LOG_LEVEL_TEST, PSTR("Read"));   
    // break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;   
    // /************
    //  * WEBPAGE SECTION * 
    // *******************/
    // #ifdef USE_MODULE_CORE_WEBSERVER
    // case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    // #endif //USE_MODULE_CORE_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
      break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    #endif //USE_MQTT
  }

  return function_result;

}


uint8_t mRuleEngine::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add(PM_JSON_DEVICES_CONNECTED, settings.relays_connected);

    char buffer[200];

    char name[10]={0};

    for(uint8_t id=0;id<2;id++){

        sprintf(name, "Rule%d", id);

      JsonBuilderI->Level_Start_P(name);//pCONT_set->GetDeviceName(D_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   

        JsonBuilderI->Level_Start_P("Source");
            JsonBuilderI->Add("module_id", rules[id].trigger.module_id);
            JsonBuilderI->Add("function_id", rules[id].trigger.function_id);
            JBI->Array_AddArray("params", rules[id].trigger.value.data, 5);
            // if(rules[id].trigger.p_json_commands!=nullptr){
            //     JBI->Add("json", rules[id].trigger.p_json_commands);
            // }
        JsonBuilderI->Level_End(); 

        JsonBuilderI->Level_Start_P("Destination");
            JsonBuilderI->Add("module_id", rules[id].command.module_id);
            JsonBuilderI->Add("function_id", rules[id].command.function_id);
            JBI->Array_AddArray("params", rules[id].command.value.data, 5);
            // if(rules[id].command.p_json_commands!=nullptr){

            if(rules[id].command.json_commands_dlist_id>=0){
            //     JBI->Add("json", rules[id].command.p_json_commands);


                // pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), rules[id].command.json_commands_dlist_id, jsonbuffer.data);  // should this be _P?
                //     JBI->Add("json", buffer); 

                    
                char buffer_unescaped[100] = {0};
                char buffer_escaped[100] = {0};
                uint8_t len  = 0;
                
                pCONT_sup->GetTextIndexed(
                    buffer_unescaped, 
                    sizeof(buffer_unescaped), 
                    pCONT_rules->rules[0].command.json_commands_dlist_id, 
                    pCONT_rules->jsonbuffer.data
                ); 

                for(int i=0;i<strlen(buffer_unescaped);i++){
                    if(buffer_unescaped[i] == '\"'){
                    len+=sprintf(buffer_escaped+len,"\\\"");
                    }else{    
                    buffer_escaped[len++] = buffer_unescaped[i];
                    }
                }
                JBI->Add("json_es", buffer_escaped); 

            }
        JsonBuilderI->Level_End(); 


      JsonBuilderI->Level_End(); 
    }
  
  JsonBuilderI->End();

}

/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mRuleEngine::MQTTHandler_Init(){

  struct handler<mRuleEngine>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//SEC_IN_HOUR; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Settings;

//   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Sensor;

//   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Sensor;

  
//   mqtthandler_ptr = &mqtthandler_scheduled_teleperiod;
//   mqtthandler_ptr->handler_id = MQTT_HANDLER_SCHEDULED_TELEPERIOD_ID;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCHEDULED_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Scheduled;


  
} //end "MQTTHandler_Init"


void mRuleEngine::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_teleperiod.flags.SendNow = true;
//   mqtthandler_scheduled_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mRuleEngine::MQTTHandler_Set_TelePeriod(){

//   mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_scheduled_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mRuleEngine::MQTTHandler_Sender(uint8_t mqtt_handler_id){
  
  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    // MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    // MQTT_HANDLER_SENSOR_TELEPERIOD_ID,
    // MQTT_HANDLER_SCHEDULED_TELEPERIOD_ID
  };
  
  struct handler<mRuleEngine>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    // &mqtthandler_sensor_ifchanged,
    // &mqtthandler_sensor_teleperiod,
    // &mqtthandler_scheduled_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_CORE_RULES_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}