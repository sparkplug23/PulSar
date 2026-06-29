#include "mRuleEngine.h"


#ifdef USE_MODULE_CORE_RULES

int8_t mRuleEngine::Tasker(uint8_t function, JsonParserObject obj){

    // ALOG_INF(PSTR(D_LOG_APPLICATION D_BOOT_COUNT ));

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case TASK_PRE_INIT:
    //   Pre_Init();
    break;
    case TASK_INIT:
      // DefaultRuleForModule();

      #ifdef ENABLE_RULES_ENGINE__TEST_BUTTON0_RELAY0
      Preset_Link_ButtonToggleRelay(0,0);
      Preset_Link_ButtonToggleRelay(1,1);
      Preset_Link_SwitchToggleRelay(0,2);
      Preset_Link_ButtonToggleRelay(5,3);
      #endif


    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
//   if(!settings.fEnableSensor){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    // /************
    //  * INIT SECTION * 
    // *******************/
    // case TASK_INIT:
    //   init();
    // break;
    // /************
    //  * SETTINGS SECTION * 
    // *******************/
    // case TASK_INIT_LOAD_MODULE_CONFIG_FROM_FILESYSTEM: 
    //   // Settings_Load();
    // break;
    // case TASK_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   // Settings_Save();
    // break;
    // case TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   // Settings_Default();
    //   // tkr_set->SettingsSave(2);
    // break;
    // /************
    //  * PERIODIC SECTION * 
    // *******************/
    // case TASK_LOOP: 
    //   EveryLoop();
    // break;  
    case TASK_EVERY_SECOND:
      // ALOG_TST(PSTR("DefaultRuleForModule"));   
      // // DefaultRuleForModule();
      // MQTTHandler_RefreshAll();

      // if(){ tkr->Tasker_Interface(TASK_RULES_LOAD_FROM_PROGMEM_ID) };
      if(tkr_time->uptime_seconds_nonreset == D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND){ RulesLoad_From_Progmem(); }

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // /************
    //  * WEBPAGE SECTION * 
    // *******************/
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // case TASK_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    // #endif //USE_MODULE_NETWORK_WEBSERVER
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
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return function_result;

}

/**
 * @brief Needs updating to appendrules to add them instead of simply setting by indexing
 * Each "rule" should therefore become its own vector struct
 * 
 */
void mRuleEngine::RulesLoad_From_Progmem()
{

  ALOG_INF(PSTR(D_LOG_RULES "RulesLoad_From_Progmem--------------------"));

  // DELAY_DEBUG(5000);

  DefaultRuleForModule();
  
  #ifdef USE_RULES_TEMPLATE
  // Read into local
  data_buffer.ClearDeep();
  memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  ALOG_INF( PSTR("RULES_TEMPLATE Load = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);

  tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
  
  //IF TASKER RESULT WAS TRUE, THEN SUCCESS
  // tkr_set->runtime.boot_status.rules_template_parse_success = 1;

  mqtthandler_settings.flags.SendNow = true;


  #endif //USE_RULES_TEMPLATE

  
}



/**
 * @brief For development, print the Rule out and what it means using basic and advanced (with words)
 * 
 * show trigger and command (not event)
 */
void mRuleEngine::ShowRuleAddLogByIndex(uint8_t show_type)
{

    ALOG_INF(PSTR("1"
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
    tkr_rules->rules_active_index,
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.module_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.function_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.device_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.value.data[0], 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.value.data[1], 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.value.data[2], 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.value.data[3], 
    tkr_rules->rules[tkr_rules->rules_active_index].trigger.value.data[4],
    tkr_rules->rules[tkr_rules->rules_active_index].command.module_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].command.function_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].command.device_id, 
    tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[0], 
    tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[1], 
    tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[2], 
    tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[3], 
    tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[4]

    );
    
}

// Event (not command or trigger)
void mRuleEngine::ShowRuleEvent_AddLog(uint8_t show_type)
{

  ALOG_INF(PSTR(""
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
    
}


void mRuleEngine::AddLog_DebugRule(EventPackage* rule)
{

  ALOG_INF(PSTR(""
      "\n\r\n\r"
      "Event>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
    ""),
    rule->module_id, 
    rule->function_id, 
    rule->device_id, 
    rule->value.data[0], rule->value.data[1], rule->value.data[2], rule->value.data[3], rule->value.data[4]
  );
    
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
  tkr->Tasker_Interface(function_event);
  // New method to check the rules
  Tasker_Rules_Interface(function_event);

}


/**
 * @brief Version 2 of triggering rules, adding the event but also include the type of function. Thie function can choose to pass that on, ignore or change the function type 
 * 
 * @param _module_id     : Unique ID of the module "GetModuleUniqueID()"
 * @param function_event : TASK_ID of the event
 * @param _index         : Index of the device/sensor of that module when multiple
 * @param _data_length   : Length of the data, 
 * @param ... Each byte of the data is its own argument which depends on the type of event (another method might use vectors) 
 * @return If the Tasker reports but event is handled, then the module calling it can leave it as handled
 */
bool mRuleEngine::NewEventRun_NumArg(uint16_t _module_id, uint16_t function_event, uint8_t _index, uint8_t _data_length, ...)
{
  Reset();
  event_triggered.module_id = _module_id;
  event_triggered.function_id = function_event;
  event_triggered.device_id = _index;
  
  char buffer[50];
  DLI->GetDeviceName_WithModuleUniqueID( event_triggered.module_id, event_triggered.device_id, buffer, sizeof(buffer));

  ALOG_INF(PSTR("Swithmoded to %d"),tkr_set->Settings.switchmode[0]);


  ALOG_INF( PSTR(D_LOG_RULES "NewEventRun_NumArg\n\r\t\t\t\tModule [%d\t%S],\n\r\t\t\t\tTask   [%d\t%S],\n\r\t\t\t\tIndex  [%d\t%s]"), 
    event_triggered.module_id,   tkr->GetModuleName(event_triggered.module_id),
    event_triggered.function_id, tkr->GetTaskName(event_triggered.function_id),
    event_triggered.device_id,   buffer
  );
 
  /***
   * Unpack data bytes
   */
  va_list arg;
  va_start(arg, _data_length);
  for(int i = 0; i < _data_length; i++) 
  {
    if(i < RULE_ENCODED_DATA_MAX_BYTES)
      event_triggered.value.data[i] = va_arg(arg, int);
  }
  va_end(arg);
  AddLog_Array(LOG_LEVEL_INFO, PSTR(D_LOG_RULES "\tdata"), event_triggered.value.data, _data_length);

  uint8_t task_handled = 0;

  /**
   * @brief Call RULES
   **/
  Tasker_Rules_Interface(function_event);

  /**
   * @brief Call all Taskers for hardcoded triggers
   **/
  tkr->Tasker_Interface(function_event);

  return task_handled ? true : false;

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
bool mRuleEngine::Tasker_Rules_Interface(uint16_t function_input){

  bool task_handled = false;

  //maybe need to return rule(s) handled then leave taasker_interface

  // D_MAX_RULES
  for (int rule_index=0;rule_index<D_MAX_RULES;rule_index++)
  {

    if(!rules[rule_index].flag_configured){ continue; } // Skip if not configured

    ALOG_INF(PSTR(D_LOG_RULES "Rule %d........"), rule_index);

    #ifdef ENABLE_DEBUGLOG_RULES_ENGINE
    AddLog_DebugRule(&event_triggered);
    AddLog_DebugRule(&rules[rule_index].trigger);
    AddLog_DebugRule(&rules[rule_index].command);
    #endif

    // Only run if configured and enabled
    if(rules[rule_index].flag_configured && rules[rule_index].flag_enabled)
    {

    // Check this rule must act of the function
    if(rules[rule_index].trigger.function_id == function_input)
    {

      ALOG_INF(PSTR(D_LOG_RULES "R%d: MATCHED function_input[%d]"), rule_index, function_input);

      rules_active_index = rule_index;

      /**
       * @brief Checking if all 3 criteria match can be one if statement. Or, 3 consequentive (not nested) for debugging and only running when matches == 3. easier to read
       * 
       */

      // Also check switch_index against rule index
      if(rules[rule_index].trigger.device_id == event_triggered.device_id)
      {

        ALOG_INF( PSTR(D_LOG_RULES "R%d: MATCHED device_id[%d]"), rule_index, rules[rule_index].trigger.device_id  );

        // char message[50];
        // memset(message,0,sizeof(message));
        // sprintf_P(message,PSTR("{\"Rule\":%d,\"EventIndex\":%d}"), rule_index, Event.index);
        // tkr_mqtt->publish_device("status/debug/rules",message,false); //reconnect message
        // ^^ Add this later as a topic so I can know when rule triggers

        /**
         * Check if module_id has matched rule
         * */
        if(rules[rule_index].trigger.module_id == event_triggered.module_id)
        {
               
          ALOG_INF(PSTR(D_LOG_RULES "R%d: MATCHED module_id[%d] : Triggered"), rule_index, rules[rule_index].trigger.module_id);

          // Populate any jsoncommands to be executed, this takes precident over "State" controls
          if(HasJsonCommand(&rules[rule_index].command))
          {
            ALOG_INF(PSTR(D_LOG_RULES "Json command detected"));

            data_buffer.ClearDeep();

            tkr_sup->GetTextIndexed(
              data_buffer.payload.ctr,
              sizeof(data_buffer.payload.ctr),
              JsonCommandDListIndex(&rules[rule_index].command),
              tkr_rules->jsonbuffer.data
            );

            data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

            ALOG_INF(PSTR("TASK_JSON_COMMAND_ID mrules=%s"), data_buffer.payload.ctr);

            tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
          }
          else
          {
            ALOG_INF(PSTR("Execute Tasker_Interface(func=%d,module=%d,SourceIsRule=%d)"),
                  rules[rule_index].command.function_id,
                  rules[rule_index].command.module_id,
                  true
            );

            tkr->Tasker_Interface(
              rules[rule_index].command.function_id
            );
          }

        } // trigger.module_id

      }
      else
      {

        ALOG_INF( PSTR(D_LOG_RULES "R%d: FAILED match device_id[%d=|=%d]"), rule_index, rules[rule_index].trigger.device_id, event_triggered.device_id);
        
      }

    }

    } // configured and enabled

  } // for loop

  return task_handled;
 
}


void mRuleEngine::DefaultRuleForModule(){

rules_active_index = 0;

#ifdef ESP8266
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    if(tkr_set->Settings.module == mPins::MODULE_SONOFF_4CHPRO){
      DefaultRule_Sonoff_4CHPRO();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_4CHPRO
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_R2
    if(tkr_set->Settings.module == mPins::MODULE_SONOFF_BASIC){
      DefaultRule_Sonoff_Basic_R2();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_BASIC_R2
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_R2
    if(tkr_set->Settings.module == mPins::MODULE_SONOFF_BASIC_NODEMCU){
      DefaultRule_Sonoff_Basic_R2();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_BASIC_R2
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    if(tkr_set->Settings.module == mPins::MODULE_SONOFF_IFAN03){
      DefaultRule_Sonoff_iFan03();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    if(tkr_set->Settings.module == mPins::MODULE_SHELLY_DIMMER2){
      DefaultRule_Shelly_Dimmer2();
    }
  #endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    #ifndef USE_MODULE_TEMPLATE_SHELLY_2P5_FORCED_DISABLED
    else
    if(tkr_set->Settings.module == mPins::MODULE_SHELLY2P5){
      DefaultRule_Shelly_2p5();
    }
    #endif
  #endif // USE_MODULE_TEMPLATE_SHELLY_2P5
#endif

}

/**
 * @brief: Function that will append a rule to the current list
 * 
 * Perhaps this needs added into each module, ie the way the rule is encoded/decoded is contained within the module eg RF433 
 * */
bool mRuleEngine::AppendEventToRules(EventPackage* trigger_new, EventPackage* command_new)
{
  uint8_t rule_count = GetConfiguredCount();

  if(rule_count >= D_MAX_RULES){ return false; }

  memset(&rules[rule_count].trigger, 0, sizeof(EventPackage));
  memset(&rules[rule_count].command, 0, sizeof(EventPackage));

  memcpy(&rules[rule_count].trigger, trigger_new, sizeof(EventPackage));
  memcpy(&rules[rule_count].command, command_new, sizeof(EventPackage));

  rules[rule_count].flag_configured = true;
  rules[rule_count].flag_enabled = true;

  return true;
}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mRuleEngine::parsesub_Rule_Part(JsonParserObject jobj, EventPackage* event)
{

  ALOG_TST(PSTR("parsesub_Rule_Part"));

  JsonParserToken jtok;
  // JsonParserObject jobj2 = &jobj;

    // if(jtok = jobj["Module"]){
    //   ALOG_INF(PSTR("JTOK FOUND jobj[rule_name] 123"));    
    // } 
    uint16_t matched_id = 0;

    // Reset value length to 0
    event->value.length = 0;


    if(jtok = jobj["Module"]){
      if(jtok.isStr()){
        // if((matched_id=tkr->GetModule_UniqueID_byName(jtok.getStr()))>=0){
        if((matched_id=tkr->GetModuleID(jtok.getStr(), true))>0){
          event->module_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->module_id = matched_id;
        data_buffer.isserviced++;
      }
      
      ALOG_INF(PSTR("Trigger Module module_id = %d"),matched_id);
    
    }//end trigger


    if(jtok = jobj["Function"]){
      if(jtok.isStr()){
        if((matched_id=tkr_set->GetFunctionIDbyName(jtok.getStr()))>=0){
          event->function_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->function_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      
      ALOG_INF(PSTR("Trigger Module Function = %d"),matched_id);
    
    }//end trigger


    if(jtok = jobj["DeviceName"]){
      //Use module_id to know which devicelist item to try find
      // if(jtok.isStr()){
      //   if((matched_id=tkr_set->GetFunctionIDbyFriendlyName(jtok.getStr()))>=0){
      //     event->function_id = matched_id;
      //     data_buffer.isserviced++;
      //   }
      // }else
      if(jtok.isNum()){
        event->device_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      
      ALOG_INF(PSTR("Trigger Module DeviceName = %d"),event->device_id);
    
    }//end trigger


    if(jtok = jobj["State"]){
      if(jtok.isStr()){
        if((matched_id = tkr_sup->GetStateNumber(jtok.getStr()))>=0){
          event->value.data[0] = matched_id;
          event->value.length++;// = 0;
          event->value.encoding = ENCODING_BYTES_ID;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->value.data[0] = jtok.getInt();
        event->value.length++;// = 0;
        event->value.encoding = ENCODING_BYTES_ID;
        data_buffer.isserviced++;
      }

      // Use state here to also set encoding, as it will know what the value is eg float = 4 bytes

      ALOG_INF(PSTR("Trigger Module State = %d"),event->value.data[0]);
    
    }//end trigger
    if(jtok = jobj["Value"]){
      if(jtok.isStr()){
        if((matched_id = tkr_sup->GetStateNumber(jtok.getStr()))>=0){
          event->value.data[1] = matched_id;
          event->value.length++;// = 0;
          event->value.encoding = ENCODING_BYTES_ID;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->value.data[1] = jtok.getInt();
        event->value.length++;// = 0;
        event->value.encoding = ENCODING_BYTES_ID;
        data_buffer.isserviced++;
      }

      // Use state here to also set encoding, as it will know what the value is eg float = 4 bytes

      ALOG_INF(PSTR("Trigger Module State = %d"),event->value.data[0]);
    
    }//end trigger

    // if(jtok = jobj["JsonCommands"]){

    //   ALOG_INF(PSTR("JTOK FOUND Trigger Module JsonCommands = %s"), jtok.getStr());


    //   // if(jsonbuffer.data != nullptr){
    //     if(strlen(jsonbuffer.data) < sizeof(jsonbuffer.data)){

    //       // uint16_t available_space = 255 - strlen(event->p_json_commands);

    //       // event->p_json_commands = &jsonbuffer.data[0];

    //       tkr_sup->AppendDList_Single(jsonbuffer.data,sizeof(jsonbuffer.data),jtok.getStr());


    //       event->json_commands_dlist_id = jsonbuffer.delims_used;

    //       jsonbuffer.delims_used++;

    //       // sprintf(event->p_json_commands,"%s|", jtok.getStr());
    //       jsonbuffer.bytes_used += strlen(jtok.getStr());
  
    //       // I need to create the ability to move to add/edit buffer (like tas)
    //       // Rules can therefore only be created once at starttime for now          

    //       ALOG_INF(PSTR("JTOK FOUND jsonbuffer.data = %s"), jsonbuffer.data);
    //       ALOG_INF(PSTR("JTOK FOUND jsonbuffer.bytes_used = %d"), jsonbuffer.bytes_used);

    //       // snprintf(event->p_json_commands+strlen(event->p_json_commands),available_space,)
    //     }
    //   // }

    
    // }//end trigger


    if (jtok = jobj["JsonCommands"]) {

      const char* s = jtok.getStr();
      if (!s) s = "";

      ALOG_INF(PSTR("Trigger Module JsonCommands = %s"), s);

      const size_t buflen = sizeof(jsonbuffer.data);
      const size_t len    = strnlen(jsonbuffer.data, buflen);

      if (len < buflen - 2) { // need space for '|' + '\0' at least

        tkr_sup->AppendDList_Single(jsonbuffer.data, buflen, s);

        jsonbuffer.delims_used++;
        event->json_command_slot = jsonbuffer.delims_used;

        jsonbuffer.bytes_used += strlen(s); // OK if you accept "requested" not "actually appended"

        ALOG_INF(PSTR("data = %s"), jsonbuffer.data);
        ALOG_INF(PSTR("bytes_used = %u"), (unsigned)jsonbuffer.bytes_used);
        ALOG_INF(PSTR("json_command_slot = %u"), event->json_command_slot);
      }
    }
    

//Execute test



}

void mRuleEngine::parse_JSONCommand(JsonParserObject obj)
{
  JsonParserToken  jtok      = 0;
  JsonParserToken  jtok_sub  = 0;
  JsonParserObject jobj      = 0;

  // ============================================================
  // [A] New format: "Rules": [ { "Name": "...", "Trigger": {...}, "Command": {...} }, ... ]
  // ============================================================
  uint8_t rules_found = 0;
  if (obj["Rules"].isArray()) {
    JsonParserArray rule_arr = obj["Rules"];
    for (uint8_t i = 0; i < rule_arr.size() && i < MAX_RULE_VARS; i++) {
      JsonParserObject robj = rule_arr[i];
      if (robj.isNull()) { continue; }

      // Optional: read/display rule name (store if your struct supports it)
      if (JsonParserToken jn = robj["Name"]) {
        ALOG_INF(PSTR("RULE[%u] Name: %s"), i, jn.getStr());
        // TODO: if you have storage, copy it: strlcpy(rules[i].name, jn.getStr(), sizeof(rules[i].name));
      }

      // Trigger
      jobj = robj["Trigger"];
      if (!jobj.isNull()) {
        parsesub_Rule_Part(jobj, &tkr_rules->rules[i].trigger);
        rules[i].flag_configured = true;
        rules[i].flag_enabled    = true;
        ALOG_INF(PSTR("RULE[%u] Trigger parsed -> enabled"), i);
      }

      // Command
      jobj = robj["Command"];
      if (!jobj.isNull()) {
        parsesub_Rule_Part(jobj, &tkr_rules->rules[i].command);
        ALOG_INF(PSTR("RULE[%u] Command parsed"), i);
      }

      rules_found++;
    }

    if (rules_found) {
      #ifdef USE_MODULE_NETWORK_MQTT
      mqtthandler_settings.flags.SendNow = true;
      #endif
    }
  }

  // ============================================================
  // [B] Legacy format fallback: "Rule0", "Rule1", ...
  //     Only if no "Rules" array parsed above.
  // ============================================================
  if (rules_found == 0) {
    char rule_name[10] = {0};
    for (uint8_t rule_index = 0; rule_index < MAX_RULE_VARS; rule_index++) {
      sprintf(rule_name, "Rule%d", rule_index);
      if ((jtok = obj[rule_name])) {

        ALOG_INF(PSTR("MATCHED %s"), rule_name);

        // Trigger
        jobj = obj[rule_name].getObject()["Trigger"];
        if (!jobj.isNull()) {
          parsesub_Rule_Part(jobj, &tkr_rules->rules[rule_index].trigger);
          rules[rule_index].flag_configured = true;
          rules[rule_index].flag_enabled    = true;
        }

        // Command
        jobj = obj[rule_name].getObject()["Command"];
        if (!jobj.isNull()) {
          parsesub_Rule_Part(jobj, &tkr_rules->rules[rule_index].command);
        }

        #ifdef USE_MODULE_NETWORK_MQTT
        mqtthandler_settings.flags.SendNow = true;
        #endif
      }
    }
  }

  // ============================================================
  // [C] AddRule support (unchanged except for minor tidy)
  // ============================================================
  if ((jtok = obj["AddRule"])) {
    ALOG_INF(PSTR(D_LOG_RULES "AddRule"));

    if ((jtok_sub = jtok.getObject()["Default"])) {
      if (jtok_sub.isArray()) {
        JsonParserArray array = jtok_sub;
        for (auto& object : array) {
          AppendRule_FromDefault_UsingName((char*)object.getStr());
        }
      }
    }

    #ifdef USE_MODULE_NETWORK_MQTT
    mqtthandler_state_ifchanged.flags.SendNow = true;
    #endif
  }

}



void mRuleEngine::AppendRule_FromDefault_UsingName(const char* name)
{

  if(strcmp(name, "Switch1Change->Relay1Follow")==0)
  {
  
    ALOG_INF(PSTR(D_LOG_RULES "AddRule Relay1Follow NEW Switch1Change->Relay1Follow"));

    EventPackage* p_event = nullptr;

    if(tkr_rules->rules_active_index>D_MAX_RULES){ return; } //block new rules

    #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)
    
    // Trigger0
    tkr_rules->rules[tkr_rules->rules_active_index].flag_enabled = true;   
    tkr_rules->rules[tkr_rules->rules_active_index].flag_configured = true;   
    p_event = &tkr_rules->rules[tkr_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = STATE_NUMBER_TOGGLE_ID;  // Toggled 
    // Command0
    p_event = &tkr_rules->rules[tkr_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = STATE_NUMBER_FOLLOW_ID;  // Toggle

    tkr_rules->rules_active_index++;

    #endif // defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)

  }

  // if(strcmp(name, "Switch1ON->Relay1On2Hrs?Switch1OFF->Relay1Off")==0)
  // {



  ALOG_INF(PSTR(D_LOG_RULES "AppendRule_FromDefault_UsingName=%d"), tkr_rules->rules_active_index);


}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_SystemRestartID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mRuleEngine::CommandSet_Rule(uint8_t value){

//   /**
//    * 0 = invalid
//    * 1 = soft restart
//    * 2 = hardware wdt restart
//    * 3+ is disabling things for stability ie fastboot or erasing settings to default
//    * 
//    * 
//    * */

//   // animation.transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
//   // animation.transition.pixels_to_update_as_percentage = value;

//   if(value == 1){
//     tkr_wifi->EspRestart();
//   }
  
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_TRANSITION,D_PIXELS_UPDATE_PERCENTAGE)), value);

// }

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mRuleEngine::ConstructJSON_Settings(uint8_t json_method, bool json_appending){


  JBI->Start();
    // JBI->Add(PM_DEVICES_CONNECTED, settings.relays_connected);

    char buffer[200];

    char name[10]={0};

      JBI->Object_Start("Settings");
        JBI->Add("Default",settings.loaded_default_for_moduled);
      JBI->Object_End();

    for(uint8_t id=0;id<GetConfiguredCount();id++){

        sprintf(name, "Rule%d", id);

      // JBI->Object_Start("Settings");
      
      // JBI->Object_End();

      JBI->Level_Start_P(name);//DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   

        JBI->Add("Conf", rules[id].flag_configured);
        JBI->Add("Enab", rules[id].flag_enabled);
        JBI->Level_Start_P("Source");
          JBI->Add("module_id", rules[id].trigger.module_id);
          JBI->Add("function_id", rules[id].trigger.function_id);
          JBI->Add("device_id", rules[id].trigger.device_id);
          JBI->Add("json_slot", rules[id].trigger.json_command_slot);
          JBI->Array_AddArray("params", rules[id].trigger.value.data, 5);
          JBI->Add("len", rules[id].trigger.value.length);
        JBI->Object_End(); 

        JBI->Level_Start_P("Destination");
          JBI->Add("module_id", rules[id].command.module_id);
          JBI->Add("function_id", rules[id].command.function_id);
          JBI->Add("device_id", rules[id].command.device_id);
          JBI->Add("json_slot", rules[id].command.json_command_slot);
          JBI->Array_AddArray("params", rules[id].command.value.data, 5);
          JBI->Add("len", rules[id].command.value.length);

          if(HasJsonCommand(&rules[id].command))
          {
            char buffer_unescaped[D_COMMAND_BUFFER_LENGTH] = {0};
            char buffer_escaped[D_COMMAND_BUFFER_LENGTH + 50] = {0};
            uint8_t len = 0;

            tkr_sup->GetTextIndexed(
              buffer_unescaped,
              sizeof(buffer_unescaped),
              JsonCommandDListIndex(&rules[id].command),
              tkr_rules->jsonbuffer.data
            );

            for(uint16_t i = 0; i < strlen(buffer_unescaped); i++)
            {
              if(buffer_unescaped[i] == '\"')
              {
                len += sprintf(buffer_escaped + len, "\\\"");
              }
              else
              {
                buffer_escaped[len++] = buffer_unescaped[i];
              }
            }

            JBI->Add("json_es", buffer_escaped);
          }
        JBI->Object_End(); 


      JBI->Object_End(); 
    }

    // JBI->Add("data", jsonbuffer.data); 
    JBI->Add("size",sizeof(rules));

    JBI->Add("ConfigCount", GetConfiguredCount());
    // JBI->Add("ConfigCount2", rule_count2);
    JBI->Add("EnabledCount", GetEnabledCount());

      // JBI->Add("jsonbuffer_data", jsonbuffer.data);    
      JBI->Add("jsonbuffer_bytes_used", jsonbuffer.bytes_used);    
  
  return JBI->End();

}

uint8_t mRuleEngine::ConstructJSON_State(uint8_t json_method, bool json_appending){

  char buffer[100];
  JBI->Start();

    JBI->Object_Start("EventTriggered");

      JBI->Add("DeviceID", event_triggered.device_id);
      JBI->Add("FunctionID", event_triggered.function_id);    
      JBI->Add("ModuleID", event_triggered.module_id);    




      

    JBI->Object_End();



  return JBI->End();
}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mRuleEngine::MQTTHandler_Init()
{

  struct handler<mRuleEngine>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif // USE_MODULE_CORE_RULES