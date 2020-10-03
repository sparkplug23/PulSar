
#include "mSwitches.h"

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

uint8_t switch_state_buf[MAX_SWITCHES] = { 0 };
uint8_t switch_virtual[MAX_SWITCHES]; 
Ticker* TickerSwitch = nullptr;


void mSwitches::SwitchInit(void)
{

  switches_found = 0;
  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    lastwallswitch[i] = 1;  // Init global to virtual switch state;
    if (pCONT_set->pin[GPIO_SWT1_ID +i] < 99) {
      switches_found++;
      // AddLog_P(LOG_LEVEL_TEST,PSTR("switches_found=%d"),switches_found);
      pinMode(pCONT_set->pin[GPIO_SWT1_ID +i], bitRead(switch_no_pullup, i) ? INPUT : ((16 == pCONT_set->pin[GPIO_SWT1_ID +i]) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
      lastwallswitch[i] = digitalRead(pCONT_set->pin[GPIO_SWT1_ID +i]);  // Set global now so doesn't change the saved power state on first switch check
    }
    switch_virtual[i] = lastwallswitch[i];
  }

  TickerSwitch = new Ticker();

  if (switches_found) { TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe); }
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
      //pins->GetPin

      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(pCONT_set->pin[GPIO_SWT1_ID +i])) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == switch_virtual[i]) {
            switch_state_buf[i] = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (switch_state_buf[i] < state_filter) {
          switch_state_buf[i]++;
          if (state_filter == switch_state_buf[i]) {
            switch_virtual[i] = 1;
          }
        }

      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == switch_virtual[i]) {
            switch_state_buf[i] = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (switch_state_buf[i] > 0) {
          switch_state_buf[i]--;
          if (0 == switch_state_buf[i]) {
            switch_virtual[i] = 0;
          }
        }
        
      }


    }
  }
//       AddLog_P(LOG_LEVEL_TEST,PSTR("mSwitches::SwitchProbe=%d\r\n\r"),digitalRead(pCONT_set->pin[GPIO_SWT1_ID]));

// pinMode(5,INPUT);

//       AddLog_P(LOG_LEVEL_TEST,PSTR("mSwitches::SwitchProbe=%d\r\n\r"),digitalRead(5));


  TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe);  // Re-arm as core 2.3.0 does only support ONCE mode
}

/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void mSwitches::SwitchHandler(uint8_t mode)
{
  if (pCONT_time->uptime.seconds_nonreset < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
                                 // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t button = NOT_PRESSED;
  uint8_t switchflag;
  uint16_t loops_per_second = 1000 / pCONT_set->Settings.switch_debounce;

  // AddLog_Array(LOG_LEVEL_TEST,"switch_virtual",switch_virtual,(uint8_t)sizeof(switch_virtual));

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if ((pCONT_set->pin[GPIO_SWT1_ID +i] < 99) || (mode)) {

      if (holdwallswitch[i]) {
        holdwallswitch[i]--;
        if (0 == holdwallswitch[i]) {
         // SendKey(1, i +1, 3);           // Execute command via MQTT
        }
      }

      button = switch_virtual[i];

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

      if (button != lastwallswitch[i]) {
        AddLog_P(LOG_LEVEL_TEST,PSTR("button%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),button,i,lastwallswitch[i]);
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
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i])) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTON_INV:
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i])) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case PUSHBUTTON_TOGGLE:
          if (button != lastwallswitch[i]) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case PUSHBUTTONHOLD:
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i])) {
            holdwallswitch[i] = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
          }
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i]) && (holdwallswitch[i])) {
            holdwallswitch[i] = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTONHOLD_INV:
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i])) {
            holdwallswitch[i] = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
          }
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i]) && (holdwallswitch[i])) {
            holdwallswitch[i] = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        }

        mqtthandler_sensor_ifchanged.flags.SendNow = true;

        if (switchflag < 3) {
          // if (!SendKey(1, i +1, switchflag)) {  // Execute command via MQTT
          AddLog_P(LOG_LEVEL_TEST,PSTR("ExecuteCommandPower")); 
          #ifdef USE_MODULE_DRIVERS_RELAY
          pCONT_mry->ExecuteCommandPower(i, switchflag, SRC_SWITCH);  // Execute command internally (if i < devices_present)
          // }
          #endif


    
        }

        lastwallswitch[i] = button;
      }
    }
  }
}

void mSwitches::SwitchLoop(void)
{
  if (switches_found) {
    if(mTime::TimeReached(&switch_debounce, SWITCH_DEBOUNCE_TIME)){//pCONT_set->Settings.switch_debounce)){
      SwitchHandler(0);
    }
  }
}

// #endif  // SWITCH_V2
// #endif


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
      //   pCONT_set->pin[GPIO_SWT1_NP_ID],pCONT_set->pin[GPIO_SWT2_NP_ID],
      //   pCONT_set->pin[GPIO_SWT1_ID],pCONT_set->pin[GPIO_SWT2_ID]
      // );
      // AddLog_P(LOG_LEVEL_INFO,PSTR("Switches(%d) %d,%d,%d,%d,%d,%d,%d,%d"),
      // switches_found,
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
      MQTTHandler_Init(); 
    break;
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

  pCONT_web->WebAppend_Root_Draw_Table_dList(switches_found,"switch_table", kTitle_TableTitles_Root);

}

//append to internal buffer if any root messages table
void mSwitches::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("switch_table");// Class name
  for(int row=0;row<switches_found;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      JsonBuilderI->Add_FP("ih","\"%s\"", IsSwitchActive(row)?"Pressed":"NOT Pressed");
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
  if(lastwallswitch[id] == HIGH){
    return true;
  }
  return false;

}


// /********************************************************************************************/

void mSwitches::SwitchPullupFlag(uint16 switch_bit)
{
  bitSet(switch_no_pullup, switch_bit);
}

uint8_t mSwitches::SwitchLastState(uint8_t index)
{
  return lastwallswitch[index];
}

void mSwitches::SwitchSetVirtual(uint8_t index, uint8_t state)
{
  switch_virtual[index] = state;
}

uint8_t mSwitches::SwitchGetVirtual(uint8_t index)
{
  return switch_virtual[index];
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, switches_found);
  return JsonBuilderI->End();

}

uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  JsonBuilderI->Array_AddArray("lastwallswitch", lastwallswitch, sizeof(lastwallswitch));
  
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
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSwitches::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mSwitches::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSwitches::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      // No specialised needed
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_SWITCHES_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////

#endif