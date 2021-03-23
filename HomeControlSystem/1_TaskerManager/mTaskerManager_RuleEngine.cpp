#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEVFEATURE_RULE_ENGINE

/**
 * 
 * Linking
 * 
 * */

  // struct SOURCE_PARAMETERS{
    
  // }source_parameters;
/**
 *  Case 1: Switch 1, then Relay 1
 * 
 *  source_id=switches, param1=switch index, destination_id = relays, param1 = relay1, param2 = state
 * 
 * */

// void mTaskerManager::InitRules(){




// }


// All events here will only trigger based of function calls, when those occur happen throughout code
void mTaskerManager::Tasker_Rules_Interface(uint16_t function_input){


//maybe need to return rule(s) handled then leave taasker_interface


  for (int rule_index=0;rule_index<2;rule_index++){

    // Check this rule must act of the function
    if(mrules->rules[rule_index].trigger.function_id == function_input){

      AddLog_P(LOG_LEVEL_TEST, PSTR("MATCHED Tasker_Rules_Interface rule%d"),rule_index);

      mrules->rules_active_index = rule_index;

      // Also check switch_index against rule index
      if(mrules->rules[rule_index].trigger.device_id == mrules->event_triggered.device_id){
        AddLog_P(LOG_LEVEL_TEST, PSTR("MATCHED Event.index rule%d"),mrules->event_triggered.device_id);
        AddLog_P(LOG_LEVEL_TEST, PSTR("Rule %d Triggered"),rule_index);
        
        
      // char message[50];
      // memset(message,0,sizeof(message));
      // sprintf_P(message,PSTR("{\"Rule\":%d,\"EventIndex\":%d}"), rule_index, Event.index);
      // pCONT_mqtt->ppublish("status/debug/rules",message,false); //reconnect message

        Tasker_Interface(
          mrules->rules[rule_index].command.function_id, // function the previous trigger is linked to
          mrules->rules[rule_index].command.module_id, //target module
          true  // runnig a rule, so don't call this loop back into this function
        );

        // Populate any jsoncommands to be executed
        if(mrules->rules[rule_index].command.json_commands_dlist_id>=0){

          D_DATA_BUFFER_CLEAR();
          pCONT_sup->GetTextIndexed(
            data_buffer.payload.ctr, 
            sizeof(data_buffer.payload.ctr), 
            pCONT_rules->rules[0].command.json_commands_dlist_id, 
            pCONT_rules->jsonbuffer.data
          ); 
          data_buffer.payload.len += strlen(data_buffer.payload.ctr);

          AddLog_P(LOG_LEVEL_TEST,PSTR("FUNC_JSON_COMMAND_ID mrules=%s"),data_buffer.payload.ctr);

          pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
        }

      }

    }


  }










  
}

void mTaskerManager::Tasker_Rules_Init(){

  // struct BUTTON_PRESSED_EVENT{
  //   uint8_t index = 0;
  //   uint8_t state_now = 0;
  //   uint8_t state_prev = 0;
  //   uint8_t press_type = 0; //single (0), multiple (1-10), long (11)
  // }button_event;

  // pCONT->mrules->AllocateEventBuffer(pCONT->mrules->rules[0].)

  // D_RULE_BASIC_01_INDEX_INDEX_ID,
  // D_RULE_BASIC_01_INDEX_STATE_ID,
  // D_RULE_BASIC_01_INDEX_ID,
  // D_RULE_BASIC_01_INDEX_ID,
  // D_RULE_BASIC_01_INDEX_ID
  // rules[0].trigger.encoding = 0; // type switch
  // rules[0].trigger.value.data = reinterpret_cast<uint8_t*>(&button_event);


  //test 1: switch 1 = relay 1, switch 2 = relay 2

  // mrules->rules[0].trigger.module_id = D_MODULE_SENSORS_SWITCHES_ID;
  // mrules->rules[0].trigger.function_id = FUNC_EVENT_INPUT_STATE_CHANGED;
  // mrules->rules[0].trigger.device_id = 0;
  // mrules->rules[0].trigger.value.data[0] = 2; //switch type toggle
  // #ifdef D_MODULE_DRIVERS_RELAY_ID
  // mrules->rules[0].command.module_id = D_MODULE_DRIVERS_RELAY_ID;
  // mrules->rules[0].command.function_id = FUNC_EVENT_SET_POWER;
  // mrules->rules[0].command.device_id = 0;
  // mrules->rules[0].command.value.data[0] = 2; //toggle
  // mrules->rules[0].command.value.data[2] = 100; //on level, 100%
  // #endif // D_MODULE_DRIVERS_RELAY_ID
  // #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  // mrules->rules[0].command.module_id = D_MODULE_DRIVERS_SHELLY_DIMMER_ID;
  // mrules->rules[0].command.function_id = FUNC_EVENT_SET_POWER;
  // mrules->rules[0].command.index = 0;
  // mrules->rules[0].command.state = 2; //toggle
  // #endif // USE_MODULE_DRIVERS_SHELLY_DIMMER


  // mrules->rules[1].trigger.module_id = D_MODULE_SENSORS_SWITCHES_ID;
  // mrules->rules[1].trigger.function_id = FUNC_EVENT_INPUT_STATE_CHANGED;
  // mrules->rules[1].trigger.device_id = 1; //switch1
  // mrules->rules[1].trigger.value.data[0] = 2; //switch type toggle
  // #ifdef D_MODULE_DRIVERS_RELAY_ID
  // mrules->rules[1].command.module_id = D_MODULE_DRIVERS_RELAY_ID;
  // mrules->rules[1].command.function_id = FUNC_EVENT_SET_POWER;
  // mrules->rules[1].command.device_id = 1; //relay0
  // mrules->rules[1].command.value.data[0] = 2; //toggle
  // mrules->rules[1].command.value.data[2] = 100; //on level, 100%
  // #endif // D_MODULE_DRIVERS_RELAY_ID
  // #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  // mrules->rules[1].command.module_id = D_MODULE_DRIVERS_SHELLY_DIMMER_ID;
  // mrules->rules[1].command.function_id = FUNC_EVENT_SET_POWER;
  // mrules->rules[1].command.index = 0;
  // mrules->rules[1].command.state = 2; //toggle
  // #endif // USE_MODULE_DRIVERS_SHELLY_DIMMER


  
  // rules[1].trigger.module_id = D_MODULE_SENSORS_SWITCHES_ID;
  // rules[1].trigger.function_id = FUNC_EVENT_INPUT_STATE_CHANGED;
  // // rules[1].trigger.encoding = 0; // type switch
  // // rules[1].trigger.value.data = reinterpret_cast<uint8_t*>(&button_event);
  // rules[1].trigger.index = 1;
  // rules[1].trigger.state = 2; //toggle
  // #ifdef D_MODULE_DRIVERS_RELAY_ID
  // rules[1].command.module_id = D_MODULE_DRIVERS_RELAY_ID;
  // rules[1].command.function_id = FUNC_EVENT_SET_POWER;
  // rules[1].command.index = 0;
  // rules[1].command.state = 2; //toggle
  // #endif // D_MODULE_DRIVERS_RELAY_ID
  // #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  // rules[1].command.module_id = D_MODULE_DRIVERS_SHELLY_DIMMER_ID;
  // rules[1].command.function_id = FUNC_EVENT_SET_POWER;
  // rules[1].command.index = 1;
  // rules[1].command.state = 2; //toggle
  // #endif // USE_MODULE_DRIVERS_SHELLY_DIMMER
  






  
}



#endif// ENABLE_DEVFEATURE_RULE_ENGINE