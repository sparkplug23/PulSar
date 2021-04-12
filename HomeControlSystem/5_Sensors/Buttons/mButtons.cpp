
#include "mButtons.h"

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_SENSORS_BUTTONS


const char* mButtons::PM_MODULE_SENSORS_BUTTONS_CTR = D_MODULE_SENSORS_BUTTONS_CTR;
const char* mButtons::PM_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR = D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR;




// /********************************************************************************************/

void mButtons::ButtonPullupFlag(uint8 button_bit)
{
  bitSet(key_no_pullup, button_bit);
}

void mButtons::ButtonInvertFlag(uint8 button_bit)
{
  bitSet(key_inverted, button_bit);
}

void mButtons::ButtonInit(void)
{
  
  AddLog(LOG_LEVEL_INFO, PSTR("ButtonInit"));

  buttons_found = 0;
  for (uint8_t i = 0; i < MAX_KEYS; i++) {
    if (pCONT_pins->PinUsed(GPIO_KEY1_ID,i)) {
      buttons_found++;
      AddLog(LOG_LEVEL_INFO, PSTR("buttons_found=%d"),buttons_found-1);
      pinMode(pCONT_pins->GetPin(GPIO_KEY1_ID,i), 
        bitRead(key_no_pullup, i) ? INPUT : ((16 == pCONT_pins->GetPin(GPIO_KEY1_ID,i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
        
      AddLog(LOG_LEVEL_INFO, PSTR("buttons_found pullup=%d %d"),buttons_found-1,
      bitRead(key_no_pullup, i) ? INPUT : ((16 == pCONT_pins->GetPin(GPIO_KEY1_ID,i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP)
      );

    }
  }
}

uint8_t mButtons::ButtonSerial(uint8_t serial_in_byte)
{
  if (dual_hex_code) {
    dual_hex_code--;
    if (dual_hex_code) {
      dual_button_code = (dual_button_code << 8) | serial_in_byte;
      serial_in_byte = 0;
    } else {
      if (serial_in_byte != 0xA1) {
        dual_button_code = 0;                // 0xA1 - End of Sonoff dual button code
      }
    }
  }
  if (0xA0 == serial_in_byte) {              // 0xA0 - Start of Sonoff dual button code
    serial_in_byte = 0;
    dual_button_code = 0;
    dual_hex_code = 3;
  }

  return serial_in_byte;
}

/*********************************************************************************************\
 * Button handler with single press only or multi-press and hold on all buttons
\*********************************************************************************************/

void mButtons::ButtonHandler(void)
{
  
  if (pCONT_time->uptime.seconds_nonreset < 4) { 
    
    
    // Serial.println("ButtonHandler block");
    
    return; } // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t button = BUTTON_NOT_PRESSED_ID;
  uint8_t button_present = 0;
  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Extent hold time factor in case of iminnent Reset command
  uint16_t loops_per_second = 1000 / pCONT_set->Settings.button_debounce;
  // char scmnd[20];

 uint8_t maxdev = (pCONT_set->devices_present > MAX_KEYS) ? MAX_KEYS : pCONT_set->devices_present;
 for (uint8_t button_index = 0; button_index < maxdev; button_index++) {
   
    button = BUTTON_NOT_PRESSED_ID;
    button_present = 0;

  // Serial.println("mButtons::ButtonHandler");
    // if (!button_index && ((MODULE_SONOFF_DUAL == my_module_type) || (MODULE_CH4 == my_module_type))) {
    //   button_present = 1;
    //   if (dual_button_code) {
    //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON " " D_CODE " %04X"), dual_button_code);
    //     button = BUTTON_PRESSED_ID;
    //     if (0xF500 == dual_button_code) {                      // Button hold
    //       holdbutton[button_index] = (loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10) -1;
    //       hold_time_extent = 1;
    //     }
    //     dual_button_code = 0;
    //   }
    // } else {
      if (pCONT_pins->PinUsed(GPIO_KEY1_ID, button_index)) {
        button_present = 1;
        button = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID,button_index)) != bitRead(key_inverted, button_index));
      }
    // }

    // AddLog(LOG_LEVEL_TEST, PSTR("button=%d"),button);

    if (button_present) {
      
      // if (pCONT->Tasker_Interface(FUNC_BUTTON_PRESSED)) {
      //   // Serviced internally    //causes button not to work right now, tasker is returning true always
      // }
      // else 
      {
      if ((BUTTON_PRESSED_ID == button) && (BUTTON_NOT_PRESSED_ID == lastbutton[button_index])) {
        // if (pCONT_set->Settings.flag_system.button_single) {                   // Allow only single button press for immediate action
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_IMMEDIATE), button_index);
          //if (!SendKey(0, button_index, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
            mqtthandler_sensor_ifchanged.flags.SendNow = true;
            // Type method
            // AddLog(LOG_LEVEL_INFO,PSTR("tsaved_button_debounce=%d"),tsaved_button_debounce);
            // tsaved_button_debounce = millis() + KEY_CHECK_TIME; // Push next read into future // move time forward by 1 second
            // AddLog(LOG_LEVEL_INFO,PSTR("tsaved_button_debounce=%d"),tsaved_button_debounce);


            // #ifdef USE_MODULE_DRIVERS_RELAY
            //   pCONT_mry->ExecuteCommandPower(button_index, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
            //   // #ifdef ENABLE_TEMPFIX_BLOCK_BUTTON_PRESS
            //   //   AddLog(LOG_LEVEL_ERROR,PSTR("ENABLE_TEMPFIX_BLOCK_BUTTON_PRESS=%d"),1000);
            //   //   delay(1000);
            //   // #endif
            // #endif

            #ifdef USE_MODULE_CORE_RULES
            pCONT_rules->NewEvent(EM_MODULE_SENSORS_BUTTONS_ID,button_index,button);
            pCONT->Tasker_Interface(FUNC_EVENT_INPUT_STATE_CHANGED_ID);
            #endif // USE_MODULE_CORE_RULES

          //}
        // } else {
        //   multipress[button_index] = (multiwindow[button_index]) ? multipress[button_index] +1 : 1;
        //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), button_index +1, multipress[button_index]);
        //   multiwindow[button_index] = loops_per_second / 2;  // 0.5 second multi press window
        // }
        // blinks = 201;
      }

//held buttons
      if (BUTTON_NOT_PRESSED_ID == button) {
        holdbutton[button_index] = 0;
      } else {
        holdbutton[button_index]++;
        if (pCONT_set->Settings.flag_system.button_single) {                   // Allow only single button press for immediate action
          if (holdbutton[button_index] == loops_per_second * hold_time_extent * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
            pCONT_set->Settings.flag_system.button_single = 0;
            // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_SETOPTION "13 0"));  // Disable single press only
            // ExecuteCommand(scmnd, SRC_BUTTON);
          }
        } else {
          if (pCONT_set->Settings.flag_system.button_restrict) {               // Button restriction
            if (holdbutton[button_index] == loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button hold
              multipress[button_index] = 0;
              // SendKey(0, button_index, 3);                // Execute Hold command via MQTT if ButtonTopic is set
            }
          } else {
            if (holdbutton[button_index] == loops_per_second * hold_time_extent * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
              multipress[button_index] = 0;
              // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_RESET " 1"));
              // Serial.println("WARNING -- Disabled \"ExecuteCommand(scmnd, SRC_BUTTON);\" command from support_button");
              //ExecuteCommand(scmnd, SRC_BUTTON);
            }
          }
        }
      }

      #ifdef ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
      if (!pCONT_set->Settings.flag_system.button_single) {                    // Allow multi-press
        if (multiwindow[button_index]) {
          multiwindow[button_index]--;
        } else {
          // if (!restart_flag && !holdbutton[button_index] && (multipress[button_index] > 0) && (multipress[button_index] < MAX_BUTTON_COMMANDS +3)) {
          //   bool single_press = false;
          //   if (multipress[button_index] < 3) {              // Single or Double press
          //     if ((SONOFF_DUAL_R2 == my_module_type) || (SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
          //       single_press = true;
          //     } else  {
          //       single_press = (pCONT_set->Settings.flag_system.button_swap +1 == multipress[button_index]);
          //       multipress[button_index] = 1;
          //     }
          //   }
          //   if ((MI_DESK_LAMP == my_module_type) && (button_index == 0) && (rotary_changed)){// && (light_power_state)) {
          //     rotary_changed = 0;                            // Color temp changed, no need to turn of the light
          //   } else {
          //     // if (single_press && SendKey(0, button_index + multipress[button_index], POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
          //     //   // Success
          //     // } else {
          //     //   if (multipress[button_index] < 3) {          // Single or Double press
          //     //     if (WifiState() > WIFI_RESTART) {          // WPSconfig, Smartconfig or Wifimanager active
          //     //       restart_flag = 1;
          //     //     } else {
          //     //       //ExecuteCommandPower(button_index + multipress[button_index], POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
          //     //     }
          //     //   } else {                                     // 3 - 7 press
          //     //     if (!pCONT_set->Settings.flag_system.button_restrict) {
          //     //       snprintf_P(scmnd, sizeof(scmnd), kCommands[multipress[button_index] -3]);
          //     //       // ExecuteCommand(scmnd, SRC_BUTTON);
          //     //     }
          //     //   }
          //     // }
          //   }
          //   multipress[button_index] = 0;
          // }
        }
      }
      #endif // ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
      }//if serviced, single button, 4chan
    } // if (button_present)
    
    lastbutton[button_index] = button;
  }
}

void mButtons::ButtonLoop(void)
{

  if (buttons_found) {
    if(mTime::TimeReached(&tsaved_button_debounce, pCONT_set->Settings.button_debounce)){
      ButtonHandler();
    }
  }

}


int8_t mButtons::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    case FUNC_INIT:
      ButtonInit();
    break;
    case FUNC_LOOP: 
      ButtonLoop();
    break;
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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEB SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break; 
    
    #endif // USE_MODULE_NETWORK_WEBSERVER
  }

}



    #ifdef USE_MODULE_NETWORK_WEBSERVER

void mButtons::WebAppend_Root_Draw_Table(){

  pCONT_web->WebAppend_Root_Draw_Table_Repeat_Row_Name_Numbers(buttons_found,"button_table", "Button");


}

//append to internal buffer if any root messages table
void mButtons::WebAppend_Root_Status_Table(){

  char buffer[20];

  JsonBuilderI->Array_Start_P(PM_WEB_HANDLE_DIV_NAME_BUTTON_TABLE_CTR);// Class name
  for(int row=0;row<buttons_found;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,row);
      JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,IsButtonActiveCtr(row, buffer, sizeof(buffer)));//"\"%s\"", IsButtonActiveCtr(row, buffer, sizeof(buffer)));
      if(IsButtonActive(row)){
        JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#00ff00"); //make webcolours dlist in progmem!
      }else{
        JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#ff0000");
      }
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  
}

    #endif // USE_MODULE_NETWORK_WEBSERVER

bool mButtons::IsButtonActive(uint8_t id){
// Needs to know what type the button is, low, high, no pullup etc
  if(lastbutton[id] == LOW){
    return true;
  }
  return false;

}

char* mButtons::IsButtonActiveCtr(uint8_t id, char* buffer, uint8_t buflen){
  if(IsButtonActive(id)){
    snprintf_P(buffer, buflen, "%s", PM_PRESSED);
  }else{
    snprintf_P(buffer, buflen, "%s", PM_NOT_PRESSED);
  }
  return buffer;
}




/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mButtons::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, buttons_found);
  return JsonBuilderI->End();

}

uint8_t mButtons::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();
    JsonBuilderI->Array_AddArray("lastbutton", lastbutton, sizeof(lastbutton));
  return JsonBuilderI->End();

}





/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mButtons::MQTTHandler_Init(){

  struct handler<mButtons>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mButtons::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mButtons::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mButtons::MQTTHandler_Sender(uint8_t mqtt_handler_id){
  
  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mButtons>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_BUTTONS_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}


////////////////////// END OF MQTT /////////////////////////

#endif