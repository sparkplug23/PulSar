#include "mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES


uint8_t mRuleEngine::ConstructJSON_Settings(uint8_t json_method, bool json_object_start_end_required){


  JsonBuilderI->Start();
    // JsonBuilderI->Add(PM_JSON_DEVICES_CONNECTED, settings.relays_connected);

    char buffer[200];

    char name[10]={0};

      JBI->Level_Start("Settings");
        JBI->Add("Default",settings.loaded_default_for_moduled);
      JBI->Level_End();

    for(uint8_t id=0;id<GetConfiguredCount();id++){

        sprintf(name, "Rule%d", id);

      // JBI->Level_Start("Settings");
      
      // JBI->Level_End();

      JsonBuilderI->Level_Start_P(name);//DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   

        JBI->Add("Conf", rules[id].flag_configured);
        JBI->Add("Enab", rules[id].flag_enabled);
        JsonBuilderI->Level_Start_P("Source");
            JsonBuilderI->Add("module_id", rules[id].trigger.module_id);
            JsonBuilderI->Add("function_id", rules[id].trigger.function_id);
            JsonBuilderI->Add("device_id", rules[id].trigger.device_id);
            JsonBuilderI->Add("json", rules[id].trigger.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].trigger.value.data, 5);
            JBI->Add("len", rules[id].trigger.value.length);

            
            // if(rules[id].trigger.p_json_commands!=nullptr){
            //     JBI->Add("json", rules[id].trigger.p_json_commands);
            // }
        JsonBuilderI->Level_End(); 

        JsonBuilderI->Level_Start_P("Destination");
            JsonBuilderI->Add("module_id", rules[id].command.module_id);
            JsonBuilderI->Add("function_id", rules[id].command.function_id);
            JsonBuilderI->Add("device_id", rules[id].command.device_id);
            JsonBuilderI->Add("json", rules[id].command.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].command.value.data, 5);
            JBI->Add("len", rules[id].command.value.length);
            // if(rules[id].command.p_json_commands!=nullptr){

            if(rules[id].command.json_commands_dlist_id>0){
            //     JBI->Add("json", rules[id].command.p_json_commands);


                // pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), rules[id].command.json_commands_dlist_id, jsonbuffer.data);  // should this be _P?
                //     JBI->Add("json", buffer); 

                    
                char buffer_unescaped[D_JSON_COMMAND_BUFFER_LENGTH] = {0};
                char buffer_escaped[D_JSON_COMMAND_BUFFER_LENGTH+50] = {0};
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

    JBI->Add("ConfigCount", GetConfiguredCount());
    // JBI->Add("ConfigCount2", rule_count2);
    JBI->Add("EnabledCount", GetEnabledCount());

      // JBI->Add("jsonbuffer_data", jsonbuffer.data);    
      JBI->Add("jsonbuffer_bytes_used", jsonbuffer.bytes_used);    
  
  return JsonBuilderI->End();

}

uint8_t mRuleEngine::ConstructJSON_State(uint8_t json_method, bool json_object_start_end_required){

  char buffer[100];
  JBI->Start();

    JBI->Level_Start("EventTriggered");

      JBI->Add("DeviceID", event_triggered.device_id);
      JBI->Add("FunctionID", event_triggered.function_id);    
      JBI->Add("ModuleID", event_triggered.module_id);    




      

    JBI->Level_End();



  return JBI->End();
}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
