
#include "mSwitches.h"

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

int8_t mSwitches::Tasker(uint8_t function){

  switch(function){
    case FUNC_INIT:
      SwitchInit();
    break;
    case FUNC_LOOP: 
      SwitchLoop();
    break;
    case FUNC_EVERY_SECOND:
    
      // pinMode(13,INPUT);
      // pinMode(5,INPUT);
      // AddLog_P(LOG_LEVEL_TEST,PSTR("%d %d %d %d %d %d"),
      //   digitalRead(13),digitalRead(5),
      //   pCONT_pins->GetPin(GPIO_SWT1_NP_ID],pCONT_pins->GetPin(GPIO_SWT2_NP_ID],
      //   pCONT_pins->GetPin(GPIO_SWT1_ID],pCONT_pins->GetPin(GPIO_SWT2_ID]
      // );
      // AddLog_P(LOG_LEVEL_INFO,PSTR("Switches(%d) %d,%d,%d,%d,%d,%d,%d,%d"),
      // settings.switches_found,
      // switch_virtual[0],switch_virtual[1],switch_virtual[2],switch_virtual[3],
      // switch_virtual[4],switch_virtual[5],switch_virtual[6],switch_virtual[7]);
    break;
    case FUNC_JSON_COMMAND: 
    
    break;
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break; 
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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MQTT

  }

}

void mSwitches::SwitchInit(void)
{

  uint8_t switch_default_is_active_low = false;
  #ifdef ENABLE_SWITCH_ACTIVE_LOW
  switch_default_is_active_low = true;
  #endif // ENABLE_SWITCH_ACTIVE_LOW

  settings.switches_found = 0;
  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {

    states[i].is_active_low = switch_default_is_active_low; // default is active high
    states[i].lastwallswitch = 1;  // Init global to virtual switch state;

    if (pCONT_pins->PinUsed(GPIO_SWT1_ID,i)){
      settings.switches_found++;
      #ifdef USE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("settings.switches_found=%d"),settings.switches_found);
      #endif
      pinMode(pCONT_pins->GetPin(GPIO_SWT1_ID,i), 
        bitRead(switch_no_pullup, i) ? 
          INPUT :  // no pullup/down
          ((16 == pCONT_pins->GetPin(GPIO_SWT1_ID,i)) // GPIO16 ie D0 // PULLDOWN only possible for pin16
            ? INPUT_PULLDOWN_16 : 
              INPUT_PULLUP)   // if not 16, then pull up is only option
      );
      states[i].lastwallswitch = digitalRead(pCONT_pins->GetPin(GPIO_SWT1_ID,i));  // Set global now so doesn't change the saved power state on first switch check
    }

    states[i].switch_virtual = states[i].lastwallswitch;
  }

  if(TickerSwitch == nullptr){
    TickerSwitch = new Ticker();
  }

  if (settings.switches_found) { 
    TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );
  }

}




// /*********************************************************************************************/

void mSwitches::SwitchProbe(void)
{
  if (pCONT_time->uptime.seconds_nonreset < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t state_filter = pCONT_set->Settings.switch_debounce / SWITCH_PROBE_INTERVAL;   // 5, 10, 15
  uint8_t force_high = (pCONT_set->Settings.switch_debounce % 50) &1;                   // 51, 101, 151 etc
  uint8_t force_low = (pCONT_set->Settings.switch_debounce % 50) &2;                    // 52, 102, 152 etc

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if (pCONT_pins->PinUsed(GPIO_SWT1_ID,i)) {

      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(pCONT_pins->GetPin(GPIO_SWT1_ID +i])) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == states[i].switch_virtual) {
            states[i].switch_state_buf = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (states[i].switch_state_buf < state_filter) {
          states[i].switch_state_buf++;
          if (state_filter == states[i].switch_state_buf) {
            states[i].switch_virtual = 1;
          }
        }

      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == states[i].switch_virtual) {
            states[i].switch_state_buf = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (states[i].switch_state_buf > 0) {
          states[i].switch_state_buf--;
          if (0 == states[i].switch_state_buf) {
            states[i].switch_virtual = 0;
          }
        }
        
      } // END if digitalRead

    } // if PinUsed
  } // for all switches

  TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
    [this](void){
      this->SwitchProbe();
    }
  );

}

/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void mSwitches::SwitchHandler(uint8_t mode)
{
  if (pCONT_time->uptime.seconds_nonreset < 4) { return; }  

  uint8_t button = NOT_PRESSED;
  uint8_t switchflag;
  uint16_t loops_per_second = 1000 / pCONT_set->Settings.switch_debounce;

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if ((pCONT_pins->GetPin(GPIO_SWT1_ID +i] < 99) || (mode)) {

      if (states[i].holdwallswitch) {
        states[i].holdwallswitch--;
        if (0 == states[i].holdwallswitch) {
         // SendKey(1, i +1, 3);           // Execute command via MQTT
        }
      }

      button = states[i].switch_virtual;

      // enum SwitchModeOptions {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, MAX_SWITCH_OPTION};

      // device  = Relay number 1 and up
      // state 0 = POWER_OFF = Relay Off
      // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
      // state 2 = POWER_TOGGLE = Toggle relay
      // state 3 = POWER_BLINK = Blink relay
      // state 4 = POWER_BLINK_STOP = Stop blinking relay
      // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
      // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
      // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
      // state 16 = POWER_SHOW_STATE = Show power state

      if (button != states[i].lastwallswitch) {
        states[i].ischanged = true;
        AddLog_P(LOG_LEVEL_TEST,PSTR("button%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),button,i,states[i].lastwallswitch);
        switchflag = 3;
        switch (pCONT_set->Settings.switchmode[i]) {
        case TOGGLE:
          switchflag = POWER_TOGGLE;                // Toggle
          break;
        case FOLLOW:
          switchflag = button &1;        // Follow wall switch state
          break;
        case FOLLOW_INV:
          switchflag = ~button &1;       // Follow inverted wall switch state
          break;
        case PUSHBUTTON:
          if ((PRESSED == button) && (NOT_PRESSED == states[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTON_INV:
          if ((NOT_PRESSED == button) && (PRESSED == states[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case PUSHBUTTON_TOGGLE:
          if (button != states[i].lastwallswitch) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case PUSHBUTTONHOLD:
          if ((PRESSED == button) && (NOT_PRESSED == states[i].lastwallswitch)) {
            states[i].holdwallswitch = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
          }
          if ((NOT_PRESSED == button) && (PRESSED == states[i].lastwallswitch) && (states[i].holdwallswitch)) {
            states[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTONHOLD_INV:
          if ((NOT_PRESSED == button) && (PRESSED == states[i].lastwallswitch)) {
            states[i].holdwallswitch = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
          }
          if ((PRESSED == button) && (NOT_PRESSED == states[i].lastwallswitch) && (states[i].holdwallswitch)) {
            states[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        }

        mqtthandler_sensor_ifchanged.flags.SendNow = true;

        if (switchflag < 3) {
          #ifdef USE_MODULE_DRIVERS_RELAY
          // AddLog_P(LOG_LEVEL_TEST,PSTR("ExecuteCommandPower")); 
          // if(is_linked_to_internal_relay){}
          // if(linked_internal_relay_id>=0)
          // pCONT_mry->ExecuteCommandPower(linked_internal_relay_id, switchflag, SRC_SWITCH);
          pCONT_mry->ExecuteCommandPower(i, switchflag, SRC_SWITCH);
          #endif    
        }

        states[i].lastwallswitch = button;
        states[i].state = button;

      }
    }else{
      
        states[i].ischanged = false;
    }
  }
}

void mSwitches::SwitchLoop(void)
{
  if (settings.switches_found) {
    if(mTime::TimeReached(&switch_debounce, SWITCH_DEBOUNCE_TIME)){//pCONT_set->Settings.switch_debounce)){
      SwitchHandler(0);
    }
  }
}

void mSwitches::WebAppend_Root_Draw_Table(){

  const char kTitle_TableTitles_Root[] = 
    "Switch 0" "|" 
    "Switch 1" "|" 
    "Switch 2" "|" 
    "Switch 3" "|" 
    "Switch 4" "|" 
    "Switch 5" "|" 
    "Switch 6" "|" 
    "Switch 7" "|" 
    "Switch 8" "|" ;

  pCONT_web->WebAppend_Root_Draw_Table_dList(settings.switches_found,"switch_table", kTitle_TableTitles_Root);

}

//append to internal buffer if any root messages table
void mSwitches::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("switch_table");// Class name
  for(int row=0;row<settings.switches_found;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      JsonBuilderI->Add_FV("ih","\"%s\"", IsSwitchActive(row)?"On":"Off");
      if(IsSwitchActive(row)){
        JsonBuilderI->Add("fc","#00ff00");
      }else{
        JsonBuilderI->Add("fc","#ff0000");
      }
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  
}


bool mSwitches::IsSwitchActive(uint8_t id){
// Needs to know what type the button is, low, high, no pullup etc
  if(states[id].lastwallswitch){
    return states[id].is_active_low ? false : true;
  }
  return states[id].is_active_low ? true : false;  
}


// /********************************************************************************************/

void mSwitches::SwitchPullupFlag(uint16 switch_bit)
{
  bitSet(switch_no_pullup, switch_bit);
}

uint8_t mSwitches::SwitchLastState(uint8_t index)
{
  return states[index].lastwallswitch;
}

void mSwitches::SwitchSetVirtual(uint8_t index, uint8_t state)
{
  states[index].switch_virtual = state;
}

uint8_t mSwitches::SwitchGetVirtual(uint8_t index)
{
  return states[index].switch_virtual;
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.switches_found);
  return JsonBuilderI->End();

}

uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[50]; 

  for(uint8_t sensor_id=0;sensor_id<settings.switches_found;sensor_id++){
    if(states[sensor_id].ischanged || (json_level>JSON_LEVEL_IFCHANGED) ){ 
      
      JsonBuilderI->Level_Start(pCONT_set->GetDeviceName(D_MODULE_SENSORS_SWITCHES_ID, sensor_id, buffer, sizeof(buffer)));
        JsonBuilderI->Add(D_JSON_STATE, IsSwitchActive(sensor_id));
        JsonBuilderI->Add(D_JSON_STATE "_ctr", IsSwitchActive(sensor_id)?"On":"Off");
        JsonBuilderI->Add("ischanged", states[sensor_id].ischanged);
        JsonBuilderI->Add("lastwallswitch", states[sensor_id].lastwallswitch);
        JsonBuilderI->Add("holdwallswitch", states[sensor_id].holdwallswitch);
        JsonBuilderI->Add("switch_state_buf", states[sensor_id].switch_state_buf);
        JsonBuilderI->Add("switch_virtual", states[sensor_id].switch_virtual);
        JsonBuilderI->Add("is_linked_to_internal_relay", states[sensor_id].is_linked_to_internal_relay);
        JsonBuilderI->Add("linked_internal_relay_id", states[sensor_id].linked_internal_relay_id);
      JsonBuilderI->Level_End();
      
    }

  }

  return JsonBuilderI->End();

}



/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mSwitches::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSwitches::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSwitches::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSwitches::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSwitches>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_SWITCHES_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}


////////////////////// END OF MQTT /////////////////////////

#endif