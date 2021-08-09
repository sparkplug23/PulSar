#include "mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES


const char* mRuleEngine::PM_MODULE_CORE_RULES_CTR = D_MODULE_CORE_RULES_CTR;
const char* mRuleEngine::PM_MODULE_CORE_RULES_FRIENDLY_CTR = D_MODULE_CORE_RULES_FRIENDLY_CTR;


// /* Null, because instance will be initialized on demand. */
// RuleEngine* RuleEngine::instance = nullptr;

// RuleEngine* RuleEngine::GetInstance(){
//   if (instance == nullptr){
//     instance = new RuleEngine();
//     // PRINT_FLUSHED("instance = new RuleEngine()");
//   }
//   return instance;
// }


int8_t mRuleEngine::Tasker(uint8_t function, JsonParserObject obj){

    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT ));

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
    //   Pre_Init();
    break;
    case FUNC_INIT:
      DefaultRuleForModule();
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
    case FUNC_EVERY_SECOND:
      // AddLog(LOG_LEVEL_TEST, PSTR("DefaultRuleForModule"));   
      // DefaultRuleForModule();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // /************
    //  * WEBPAGE SECTION * 
    // *******************/
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    // #endif //USE_MODULE_NETWORK_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
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
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return function_result;

}



// All events here will only trigger based of function calls, when those occur happen throughout code
void mRuleEngine::Tasker_Rules_Interface(uint16_t function_input){


  // AddLog(LOG_LEVEL_TEST, PSTR("MATCHED Tasker_Rules_Interface function_input%d"),function_input);
//maybe need to return rule(s) handled then leave taasker_interface


  for (int rule_index=0;rule_index<D_MAX_RULES;rule_index++){

    // Check this rule must act of the function
    if(rules[rule_index].trigger.function_id == function_input){

      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED function_input[%d] to rule[%d]"),function_input,rule_index);

      rules_active_index = rule_index;

      // Also check switch_index against rule index
      if(rules[rule_index].trigger.device_id == event_triggered.device_id)
      {

        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED trigger.device_id[%d] to rule[%d]"),rules[rule_index].trigger.device_id,rule_index);

        // char message[50];
        // memset(message,0,sizeof(message));
        // sprintf_P(message,PSTR("{\"Rule\":%d,\"EventIndex\":%d}"), rule_index, Event.index);
        // pCONT_mqtt->ppublish("status/debug/rules",message,false); //reconnect message

        /**
         * Check if module_id has matched rule
         * */
        if(rules[rule_index].trigger.module_id == event_triggered.module_id)
        {
        
          AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED trigger.module_id[%d] to rule[%d]"),rules[rule_index].trigger.module_id,rule_index);
         
          AddLog(LOG_LEVEL_TEST, PSTR("Rule %d Triggered"),rule_index);
          AddLog(LOG_LEVEL_TEST, PSTR("Trying to target module %d \"%s\""),rules[rule_index].command.module_id, pCONT->GetModuleFriendlyName(rules[rule_index].command.module_id));
        

          // Populate any jsoncommands to be executed, this takes precident over "State" controls
          if(rules[rule_index].command.json_commands_dlist_id>=0)
          {

            D_DATA_BUFFER_CLEAR();
            pCONT_sup->GetTextIndexed(
              data_buffer.payload.ctr, 
              sizeof(data_buffer.payload.ctr), 
              pCONT_rules->rules[rule_index].command.json_commands_dlist_id, 
              pCONT_rules->jsonbuffer.data
            ); 
            data_buffer.payload.len += strlen(data_buffer.payload.ctr);

            AddLog(LOG_LEVEL_TEST,PSTR("FUNC_JSON_COMMAND_ID mrules=%s"),data_buffer.payload.ctr);

            pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
          }
          else // Execute normal state/value method if no jsoncommand was used
          {
            
            pCONT->Tasker_Interface(
              rules[rule_index].command.function_id, // function the previous trigger is linked to
              rules[rule_index].command.module_id, //target module
              true  // runnig a rule, so don't call this loop back into this function
            );


            AddLog(LOG_LEVEL_TEST, PSTR("Execute Tasker_Interface(func=%d,module=%d,SourceIsRule=%d)"),
              rules[rule_index].command.function_id, // function the previous trigger is linked to
              rules[rule_index].command.module_id, //target module
              true  // runnig a rule, so don't call this loop back into this function
            );

          }

        } // trigger.module_id

      }
      else
      {

        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES ".............NOMATCH trigger.device_id[%d] to rule[%d]"),rules[rule_index].trigger.device_id,rule_index);

      }

    }


  }
 
}


void mRuleEngine::DefaultRuleForModule(){

// #ifndef USE_RULES_TEMPLATE // if no rules defined, then check for preset defaults by hardware type
rules_active_index = 0;

#ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  if(pCONT_set->Settings.module == MODULE_SONOFF_IFAN03_ID){
    DefaultRule_Sonoff_iFan03();
  }else
#endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
#ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  if(pCONT_set->Settings.module == MODULE_SHELLY_DIMMER2_ID){
    DefaultRule_Shelly_Dimmer2();
  }else
#endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2
#ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
  if(pCONT_set->Settings.module == MODULE_SHELLY2P5_ID){
    DefaultRule_Shelly_2p5();
  }
#endif // USE_MODULE_TEMPLATE_SHELLY_2P5

// #endif // USE_RULES_TEMPLATE // if no rules defined, then check for preset defaults by hardware type

}




uint8_t mRuleEngine::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add(PM_JSON_DEVICES_CONNECTED, settings.relays_connected);

    char buffer[200];

    char name[10]={0};

    for(uint8_t id=0;id<MAX_RULE_VARS;id++){

        sprintf(name, "Rule%d", id);

      JBI->Level_Start("Settings");
        JBI->Add("Default",settings.loaded_default_for_moduled);
      JBI->Level_End();

      JsonBuilderI->Level_Start_P(name);//DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   

        JsonBuilderI->Level_Start_P("Source");
            JsonBuilderI->Add("module_id", rules[id].trigger.module_id);
            JsonBuilderI->Add("function_id", rules[id].trigger.function_id);
            JsonBuilderI->Add("device_id", rules[id].trigger.device_id);
            JsonBuilderI->Add("json_commands_dlist_id", rules[id].trigger.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].trigger.value.data, 5);

            
            // if(rules[id].trigger.p_json_commands!=nullptr){
            //     JBI->Add("json", rules[id].trigger.p_json_commands);
            // }
        JsonBuilderI->Level_End(); 

        JsonBuilderI->Level_Start_P("Destination");
            JsonBuilderI->Add("module_id", rules[id].command.module_id);
            JsonBuilderI->Add("function_id", rules[id].command.function_id);
            JsonBuilderI->Add("device_id", rules[id].command.device_id);
            JsonBuilderI->Add("json_commands_dlist_id", rules[id].command.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].command.value.data, 5);
            // if(rules[id].command.p_json_commands!=nullptr){

            if(rules[id].command.json_commands_dlist_id>=0){
            //     JBI->Add("json", rules[id].command.p_json_commands);


                // pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), rules[id].command.json_commands_dlist_id, jsonbuffer.data);  // should this be _P?
                //     JBI->Add("json", buffer); 

                    
                char buffer_unescaped[200] = {0};
                char buffer_escaped[200] = {0};
                uint8_t len  = 0;
                
                pCONT_sup->GetTextIndexed(
                    buffer_unescaped, 
                    sizeof(buffer_unescaped), 
                    pCONT_rules->rules[id].command.json_commands_dlist_id, 
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

    // JBI->Add("data", jsonbuffer.data); 
    JBI->Add("size",sizeof(rules));
  
  JsonBuilderI->End();

}

uint8_t mRuleEngine::ConstructJSON_Sensor(uint8_t json_level){

  char buffer[100];
  JBI->Start();

    JBI->Level_Start("EventTriggered");

      JBI->Add("DeviceID", event_triggered.device_id);
      JBI->Add("FunctionID", event_triggered.function_id);    
      JBI->Add("ModuleID", event_triggered.module_id);    

    JBI->Level_End();



  JBI->End();

}


/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
void mRuleEngine::MQTTHandler_Init(){

  struct handler<mRuleEngine>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Settings;

  #ifdef ENABLE_DEVFEATURE_RULES_MQTT_FASTER_SECS
  //mqtthandler_ptr->tRateSecs = 1;
  #endif

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Sensor;

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

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mRuleEngine::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRuleEngine::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

void mRuleEngine::MQTTHandler_Sender(uint8_t id){
    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CORE_RULES_ID, handle, id);
  }

}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CORE_RULES