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
      // // DefaultRuleForModule();
      // MQTTHandler_Set_RefreshAll();
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

/**
 * @brief For development, print the Rule out and what it means using basic and advanced (with words)
 * 
 * show trigger and command (not event)
 */
void mRuleEngine::ShowRuleAddLogByIndex(uint8_t show_type)
{

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("1"
      "\n\rIndex:\t %d\n\r"
      "Trigger>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
      "Command>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
    "2"),
    pCONT_rules->rules_active_index,
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.module_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.function_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.device_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[0], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[1], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[2], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[3], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[4],
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.module_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.function_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[1], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[2], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[3], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[4]

    );

    #endif //  ENABLE_LOG_LEVEL_INFO
    
}

// Event (not command or trigger)
void mRuleEngine::ShowRuleEvent_AddLog(uint8_t show_type)
{

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO, PSTR(""
      "\n\r\n\r"
      "Event>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
    ""),
    event_triggered.module_id, 
    event_triggered.function_id, 
    event_triggered.device_id, 
    event_triggered.value.data[0], 
    event_triggered.value.data[1], 
    event_triggered.value.data[2], 
    event_triggered.value.data[3], 
    event_triggered.value.data[4]
  );
    
    #endif //  ENABLE_LOG_LEVEL_INFO
}


/**
 * @brief Version 2 of triggering rules, adding the event but also include the type of function. Thie function can choose to pass that on, ignore or change the function type 
 * 
 * @param _module_id 
 * @param function_event 
 * @param _index 
 * @param _state 
 */
void mRuleEngine::NewEventRun(uint16_t _module_id, uint16_t function_event, uint8_t _index, uint8_t _state)
{
  Reset();
  event_triggered.module_id = _module_id;
  event_triggered.device_id = _index;
  event_triggered.function_id = function_event;
  event_triggered.value.data[0] = _state;

  
      // ALOG_ERR(PSTR("NewEventRun_NumArg Exceeded"));

  /**
   * @brief Mess version for now, added to here
   *  */

  // Legacy option, pass to all tasker_interfaces. This allows hard coded things to happen. ie Time runs out, turn relay off
  pCONT->Tasker_Interface(function_event);
  // New method to check the rules
  Tasker_Rules_Interface(function_event);

}


/**
 * @brief Version 2 of triggering rules, adding the event but also include the type of function. Thie function can choose to pass that on, ignore or change the function type 
 * 
 * @param _module_id 
 * @param function_event 
 * @param _index 
 * @param _data_length
 * @param ... Each byte of the data is its own argument which depends on the type of event (another method might use vectors) 
 */
void mRuleEngine::NewEventRun_NumArg(uint16_t _module_id, uint16_t function_event, uint8_t _index, uint8_t _data_length, ...)
{
  Reset();
  event_triggered.module_id = _module_id;
  event_triggered.function_id = function_event;
  event_triggered.device_id = _index;
  
  va_list arg;
  
  va_start(arg, _data_length);
  for(int i = 0; i < _data_length; i++) {

    if(i>D_RULE_DATA_VALUE_MAX_LENGTH-1){
      ALOG_ERR(PSTR("NewEventRun_NumArg Exceeded"));
      break;
    }

    event_triggered.value.data[i] = va_arg(arg, int);

  }
  va_end(arg);

  AddLog_Array_Int(LOG_LEVEL_HIGHLIGHT, "data", event_triggered.value.data, ARRAY_SIZE(event_triggered.value.data));

  // DEBUG_LINE_HERE; ALOG_INF(PSTR("event_triggered.module_id = %d"), event_triggered.module_id);

  // ShowRuleEvent_AddLog();

  // va_start(arg, formatP);
  // vsnprintf_P(pCONT_set->log_data, sizeof(pCONT_set->log_data), formatP, arg);
  // va_end(arg);

  /**
   * @brief Mess version for now, added to here
   *  */

  // Legacy option, pass to all tasker_interfaces. This allows hard coded things to happen. ie Time runs out, turn relay off
  pCONT->Tasker_Interface(function_event);
  // New method to check the rules
  Tasker_Rules_Interface(function_event);

}

void NewEventRun_Arg(uint16_t _module_id=0, uint16_t function_event = 0, uint8_t _index=0, ...); 

uint8_t mRuleEngine::GetConfiguredCount()
{
  uint8_t count = 0;
  for(uint8_t i=0;i<D_MAX_RULES;i++)
  {
    if(rules[i].flag_configured){ count++; }
  }
  return count;
}


uint8_t mRuleEngine::GetEnabledCount()
{
  uint8_t count = 0;
  for(uint8_t i=0;i<D_MAX_RULES;i++)
  {
    if(rules[i].flag_enabled){ count++; }
  }
  return count;
}


// All events here will only trigger based of function calls, when those occur happen throughout code
void mRuleEngine::Tasker_Rules_Interface(uint16_t function_input){


  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("\n\r\n\r\n\rMATCHED Tasker_Rules_Interface function_input%d"),function_input);
  #endif // ENABLE_LOG_LEVEL_INFO

  //maybe need to return rule(s) handled then leave taasker_interface


  for (int rule_index=0;rule_index<D_MAX_RULES;rule_index++)
  {

    // Only run if configured and enabled
    if(rules[rule_index].flag_configured && rules[rule_index].flag_enabled)
    {

    // Check this rule must act of the function
    if(rules[rule_index].trigger.function_id == function_input){

    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED function_input[%d] to rule[%d]"),function_input,rule_index);
    #endif // ENABLE_LOG_LEVEL_INFO

      rules_active_index = rule_index;

      /**
       * @brief Checking if all 3 criteria match can be one if statement. Or, 3 consequentive (not nested) for debugging and only running when matches == 3. easier to read
       * 
       */

      // Also check switch_index against rule index
      if(rules[rule_index].trigger.device_id == event_triggered.device_id)
      {

    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED trigger.device_id[%d] to rule[%d]"),rules[rule_index].trigger.device_id,rule_index);
    #endif// ENABLE_LOG_LEVEL_INFO

        // char message[50];
        // memset(message,0,sizeof(message));
        // sprintf_P(message,PSTR("{\"Rule\":%d,\"EventIndex\":%d}"), rule_index, Event.index);
        // pCONT_mqtt->ppublish("status/debug/rules",message,false); //reconnect message

        /**
         * Check if module_id has matched rule
         * */
        if(rules[rule_index].trigger.module_id == event_triggered.module_id)
        {
        
    #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES "MATCHED trigger.module_id[%d] to rule[%d]"),rules[rule_index].trigger.module_id,rule_index);         
          AddLog(LOG_LEVEL_TEST, PSTR("Rule %d Triggered"),rule_index);
          AddLog(LOG_LEVEL_TEST, PSTR("Trying to target module %d \"%s\""),rules[rule_index].command.module_id, pCONT->GetModuleFriendlyName(rules[rule_index].command.module_id));
        
    #endif// ENABLE_LOG_LEVEL_INFO

          // Populate any jsoncommands to be executed, this takes precident over "State" controls
          if(rules[rule_index].command.json_commands_dlist_id>0)
          {

            D_DATA_BUFFER_CLEAR();
            pCONT_sup->GetTextIndexed(
              data_buffer.payload.ctr, 
              sizeof(data_buffer.payload.ctr), 
              pCONT_rules->rules[rule_index].command.json_commands_dlist_id, 
              pCONT_rules->jsonbuffer.data
            ); 
            data_buffer.payload.len += strlen(data_buffer.payload.ctr);

    #ifdef ENABLE_LOG_LEVEL_INFO
            AddLog(LOG_LEVEL_TEST,PSTR("FUNC_JSON_COMMAND_ID mrules=%s"),data_buffer.payload.ctr);

    #endif // ENABLE_LOG_LEVEL_INFO
            pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
          }
          else // Execute normal state/value method if no jsoncommand was used
          {

            /**
             * If destination.state is "2", then "NewEvent" needs to be inverted from trigger
             * */
            // if(rules[rule_index].command.value.data[0]==2)
            // {

            // }
            
            // #ifdef ENABLE_RULES_TRIGGER_METHOD_V2
              pCONT->Tasker_Interface(
                rules[rule_index].command.function_id, // function the previous trigger is linked to
                rules[rule_index].command.module_id //target module
              );
            // #else // should not be calling itself again, so basic call only needed
            // pCONT->Tasker_Interface(
            //   rules[rule_index].command.function_id, // function the previous trigger is linked to
            //   rules[rule_index].command.module_id, //target module
            //   true  // runnig a rule, so don't call this loop back into this function
            // );
            // #endif // ENABLE_RULES_TRIGGER_METHOD_V2


    #ifdef ENABLE_LOG_LEVEL_ERROR
            AddLog(LOG_LEVEL_TEST, PSTR("Execute Tasker_Interface(func=%d,module=%d,SourceIsRule=%d)"),
              rules[rule_index].command.function_id, // function the previous trigger is linked to
              rules[rule_index].command.module_id, //target module
              true  // runnig a rule, so don't call this loop back into this function
            );

    #endif // ENABLE_LOG_LEVEL_INFO
          }

        } // trigger.module_id

      }
      else
      {

    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_RULES ".............NOMATCH trigger.device_id[%d] to rule[%d]"),rules[rule_index].trigger.device_id,rule_index);

    #endif // ENABLE_LOG_LEVEL_INFO
      }

    }

    } // configured and enabled

  } // for loop
 
}


void mRuleEngine::DefaultRuleForModule(){

// #ifndef USE_RULES_TEMPLATE // if no rules defined, then check for preset defaults by hardware type
rules_active_index = 0;

#ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
  if(pCONT_set->Settings.module == MODULE_SONOFF_4CHPRO_ID){
    DefaultRule_Sonoff_4CHPRO();
  }else
#endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
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

/**
 * @brief: Function that will append a rule to the current list
 * 
 * Perhaps this needs added into each module, ie the way the rule is encoded/decoded is contained within the module eg RF433 
 * */
bool mRuleEngine::AppendEventToRules(mEvent::EVENT_PART* trigger_new, mEvent::EVENT_PART* command_new) // CHANGE TO ADD RULE
{

  uint8_t rule_count = GetConfiguredCount();
  uint8_t new_index = rule_count;

  if(rule_count>D_MAX_RULES){ return false; } //block new rules

  // #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)

  // TBD: Add check for unique rule

  // Clear rule
  memset(&rules[rule_count].trigger, 0, sizeof(mEvent::EVENT_PART));
  memset(&rules[rule_count].command, 0, sizeof(mEvent::EVENT_PART));

  // Copy rule
  memcpy(&rules[rule_count].trigger, trigger_new, sizeof(mEvent::EVENT_PART));
  memcpy(&rules[rule_count].command, command_new, sizeof(mEvent::EVENT_PART));
  // Activate rule
  rules[rule_count].flag_configured = true;
  rules[rule_count].flag_enabled = true;
  // If succesfully added
  return true;
  // pCONT_rules->rules_active_index++;

}



#endif // USE_MODULE_CORE_RULES