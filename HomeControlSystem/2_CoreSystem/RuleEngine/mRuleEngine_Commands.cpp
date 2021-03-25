
#include "2_CoreSystem/RuleEngine/mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES

int8_t mRuleEngine::CheckAndExecute_JSONCommands(){

  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CORE_RULES_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_CORE_RULES_FRIENDLY_CTR));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mRuleEngine::parsesub_Rule_Part(JsonParserObject jobj, mRuleEngine::EVENT_PART* event){

  AddLog_P(LOG_LEVEL_TEST, PSTR("parsesub_Rule_Part"));

  JsonParserToken jtok;
  // JsonParserObject jobj2 = &jobj;

    // if(jtok = jobj["Module"]){
    //   AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND jobj[rule_name] 123"));    
    // } 
    int16_t matched_id = 0;

    // Reset value length to 0
    event->value.length = 0;


    if(jtok = jobj["Module"]){
      if(jtok.isStr()){
        if((matched_id=pCONT->GetModuleIDbyFriendlyName(jtok.getStr()))>=0){
          event->module_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->module_id = matched_id;
        data_buffer.isserviced++;
      }
      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module module_id = %d"),matched_id);
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["Function"]){
      if(jtok.isStr()){
        if((matched_id=pCONT_set->GetFunctionIDbyFriendlyName(jtok.getStr()))>=0){
          event->function_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->function_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module Function = %d"),matched_id);
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["DeviceName"]){
      //Use module_id to know which devicelist item to try find
      // if(jtok.isStr()){
      //   if((matched_id=pCONT_set->GetFunctionIDbyFriendlyName(jtok.getStr()))>=0){
      //     event->function_id = matched_id;
      //     data_buffer.isserviced++;
      //   }
      // }else
      if(jtok.isNum()){
        event->device_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module DeviceName = %d"),event->device_id);
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["State"]){
      if(jtok.isStr()){
        if((matched_id = pCONT_sup->GetStateNumber(jtok.getStr()))>=0){
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

      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger
    if(jtok = jobj["Value"]){
      if(jtok.isStr()){
        if((matched_id = pCONT_sup->GetStateNumber(jtok.getStr()))>=0){
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

      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger

    if(jtok = jobj["JsonCommands"]){

      AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module JsonCommands = %s"), jtok.getStr());


      // if(jsonbuffer.data != nullptr){
        if(strlen(jsonbuffer.data)<200){

          // uint16_t available_space = 255 - strlen(event->p_json_commands);

          // event->p_json_commands = &jsonbuffer.data[0];

          pCONT_sup->AppendDList(jsonbuffer.data,jtok.getStr());


          event->json_commands_dlist_id = jsonbuffer.delims_used;

          jsonbuffer.delims_used++;

          // sprintf(event->p_json_commands,"%s|", jtok.getStr());
          jsonbuffer.bytes_used += strlen(jtok.getStr());

          // char dlist[200]; memset(dlist,0,sizeof(dlist));
          // pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_PIXELS_UPDATE_PERCENTAGE);
  
          // I need to create the ability to move to add/edit buffer (like tas)
          // Rules can therefore only be created once at starttime for now          

          AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND jsonbuffer.data = %s"), jsonbuffer.data);
          AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND jsonbuffer.bytes_used = %d"), jsonbuffer.bytes_used);

          // snprintf(event->p_json_commands+strlen(event->p_json_commands),available_space,)
        }
      // }

    
    }//end trigger

//Execute test



}


void mRuleEngine::parse_JSONCommand(){

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
    #endif //ENABLE_LOG_LEVEL_COMMANDS
    return;
  } 

  JsonParserToken jtok = 0; 
  JsonParserObject jobj = 0;
  int16_t tmp_id = 0;

  uint8_t rule_index = 0;
  char rule_name[10] = {0};
  for(int rule_index=0;rule_index<2;rule_index++){
    sprintf(rule_name, "Rule%d", rule_index);
    if(jtok = obj[rule_name]){
      
      AddLog_P(LOG_LEVEL_INFO, PSTR("MATCHED Rule%d"),rule_index);

      mRuleEngine::EVENT_PART* p_event = nullptr;

      jobj = obj[rule_name].getObject()["Trigger"];
      if(!jobj.isNull()){
        parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].trigger);
      }

      jobj = obj[rule_name].getObject()["Command"];
      if(!jobj.isNull()){
        parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].command);
      }

    }

  }  







    /**
     * Get modules
     * */
    // int8_t module_id = 0;
    
    // if(jtok = jobj["Command"].getObject()["Module"]){

    //   if(jtok.isStr()){
    //     if((tmp_id=pCONT->GetModuleIDbyFriendlyName(jtok.getStr()))>=0){
    //       AddLog_P(LOG_LEVEL_INFO, PSTR("22JTOK FOUND Trigger Module tmp_id = %d"),tmp_id);
    //       p_event->module_id = tmp_id;
    //       data_buffer.isserviced++;
    //     }
    //   }else
    //   if(jtok.isNum()){
    //     AddLog_P(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module"));
    //     // get pointer to rule via rule_index
    //     // CommandSet_Rule_Module_ID(jtok.getInt(), );
    //     data_buffer.isserviced++;
    //   }
    //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    //   // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    // }//end trigger

    //Devicename will need to search the shared buffer, possibly just read it in otherwise


    //SetPower is getstate (if string), else, just put value in







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
//     pCONT_wif->EspRestart();
//   }
  
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
//   // #endif

// }



#endif // USE_MODULE_CORE_RULES