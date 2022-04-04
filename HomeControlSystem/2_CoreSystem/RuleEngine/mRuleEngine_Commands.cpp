
#include "2_CoreSystem/RuleEngine/mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES

void mRuleEngine::parsesub_Rule_Part(JsonParserObject jobj, mEvent::EVENT_PART* event){

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("parsesub_Rule_Part"));
    #endif //  ENABLE_LOG_LEVEL_INFO

  JsonParserToken jtok;
  // JsonParserObject jobj2 = &jobj;

    // if(jtok = jobj["Module"]){
    //   AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND jobj[rule_name] 123"));    
    // } 
    int16_t matched_id = 0;

    // Reset value length to 0
    event->value.length = 0;


    if(jtok = jobj["Module"]){
      if(jtok.isStr()){
        if((matched_id=pCONT->GetModuleIndexbyFriendlyName(jtok.getStr()))>=0){
          event->module_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->module_id = matched_id;
        data_buffer.isserviced++;
      }
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module module_id = %d"),matched_id);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
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
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module Function = %d"),matched_id);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
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
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module DeviceName = %d"),event->device_id);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
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

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
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

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger

    if(jtok = jobj["JsonCommands"]){

    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module JsonCommands = %s"), jtok.getStr());
    #endif //  ENABLE_LOG_LEVEL_INFO


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

    #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND jsonbuffer.data = %s"), jsonbuffer.data);
          AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND jsonbuffer.bytes_used = %d"), jsonbuffer.bytes_used);
    #endif // ENABLE_LOG_LEVEL_INFO

          // snprintf(event->p_json_commands+strlen(event->p_json_commands),available_space,)
        }
      // }

    
    }//end trigger

//Execute test



}


void mRuleEngine::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  JsonParserObject jobj = 0;
  int16_t tmp_id = 0;

  uint8_t rule_index = 0;
  char rule_name[10] = {0};
  for(int rule_index=0;rule_index<MAX_RULE_VARS;rule_index++){
    sprintf(rule_name, "Rule%d", rule_index);
    if(jtok = obj[rule_name]){
      
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO, PSTR("MATCHED Rule%d"),rule_index);
    #endif // ENABLE_LOG_LEVEL_INFO

      mEvent::EVENT_PART* p_event = nullptr;

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
    //       AddLog(LOG_LEVEL_INFO, PSTR("22JTOK FOUND Trigger Module tmp_id = %d"),tmp_id);
    //       p_event->module_id = tmp_id;
    //       data_buffer.isserviced++;
    //     }
    //   }else
    //   if(jtok.isNum()){
    //     AddLog(LOG_LEVEL_INFO, PSTR("JTOK FOUND Trigger Module"));
    //     // get pointer to rule via rule_index
    //     // CommandSet_Rule_Module_ID(jtok.getInt(), );
    //     data_buffer.isserviced++;
    //   }
    //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
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
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
//   // #endif

// }



#endif // USE_MODULE_CORE_RULES