
#include "mSwitches.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

int8_t mSwitches::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_INIT:
      SwitchInit();
    break;
  }

  switch(function){
    case TASK_LOOP: 
      SwitchLoop();
    break;
    case TASK_EVERY_SECOND:

      AddLog(LOG_LEVEL_INFO,PSTR("swt=%s"),IsSwitchActive(0)?"On":"Off");
      // AddLog(LOG_LEVEL_INFO,PSTR("swt=%d"), digitalRead(5) );


      AddLog(LOG_LEVEL_INFO,PSTR("mode %d %d\n\r\n\r"),pCONT_set->Settings.switchmode[0], 0);
      

       
        // Serial.printf("PinUsed[29]\t\tpin=%d\n\r",pCONT_pins->GetPin(29));
        // Serial.printf("PinUsed[30]\t\tpin=%d\n\r",pCONT_pins->GetPin(30));
        // Serial.printf("PinUsed[291]\t\tpin=%d\n\r",pCONT_pins->PinUsed(GPIO_SWT1_NP_ID));
        // Serial.printf("PinUsed[302]\t\tpin=%d\n\r",pCONT_pins->PinUsed(GPIO_SWT2_NP_ID));
      // pinMode(13,INPUT);
      // pinMode(5,INPUT);
      // AddLog(LOG_LEVEL_TEST,PSTR("%d %d %d %d %d %d"),
      //   digitalRead(13),digitalRead(5),
      //   pCONT_pins->GetPin(GPIO_SWT1_NP_ID],pCONT_pins->GetPin(GPIO_SWT2_NP_ID],
      //   pCONT_pins->GetPin(GPIO_SWT1_ID],pCONT_pins->GetPin(GPIO_SWT2_ID]
      // );
      // AddLog(LOG_LEVEL_INFO,PSTR("Switches(%d) %d,%d,%d,%d,%d,%d,%d,%d"),
      // settings.switches_found,
      // switch_virtual[0],switch_virtual[1],switch_virtual[2],switch_virtual[3],
      // switch_virtual[4],switch_virtual[5],switch_virtual[6],switch_virtual[7]);
      
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

}



void mSwitches::SwitchInit(void)
{

  ALOG_HGL( PSTR("D_LOG_STARTUP" "Switches Init") );

  // Init states
  for(uint8_t pin_id=0;pin_id<MAX_SWITCHES;pin_id++){
    switches[pin_id].lastwallswitch = 1;  // Init global to virtual switch state;
    switches[pin_id].active_state_value = 1; // default is active high
    switches[pin_id].switch_virtual = switches[pin_id].lastwallswitch;
  }

  // Check all possible pin options
  settings.switches_found = 0;    
  for(uint8_t pin_id=GPIO_SWT1_ID;pin_id<GPIO_SWT1_ID+(MAX_SWITCHES*4);pin_id++){

        Serial.printf("pin=%d/%d\n\r",pin_id,GPIO_SWT1_ID+(MAX_SWITCHES*4));
    if(pCONT_pins->PinUsed(pin_id)){
        Serial.printf("PinUsed\t\tpin=%d\n\r",pin_id);
      
      switches[settings.switches_found].pin = pCONT_pins->GetPin(pin_id);

      // Standard pin, active high, with pulls 
      if(
        (pin_id >= GPIO_SWT1_ID)&&
        (pin_id < GPIO_SWT1_ID+MAX_SWITCHES)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT_PULLUP);
        switches[settings.switches_found].active_state_value = HIGH;
      }else
      // Inverted pin, active low, with pulls
      if(
        (pin_id >= GPIO_SWT1_INV_ID)&&
        (pin_id < GPIO_SWT1_INV_ID+MAX_SWITCHES)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT_PULLUP);
        switches[settings.switches_found].active_state_value = LOW;
      }else
      // Standard pin, active high, NO pulls
      if(
        (pin_id >= GPIO_SWT1_NP_ID)&&
        (pin_id < GPIO_SWT1_NP_ID+MAX_SWITCHES)
      ){
        Serial.printf("GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);
        pinMode(switches[settings.switches_found].pin, INPUT);
        switches[settings.switches_found].active_state_value = HIGH;
      }else
      // 
      if(
        (pin_id >= GPIO_SWT1_INV_NP_ID)&&
        (pin_id < GPIO_SWT1_INV_NP_ID+MAX_SWITCHES)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT);
        switches[settings.switches_found].active_state_value = LOW;
      }else{
        Serial.printf("NO MATCH GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);

      }

      // Set global now so doesn't change the saved power state on first switch check
      switches[settings.switches_found].lastwallswitch = digitalRead(switches[settings.switches_found].pin);  
      switches[settings.switches_found].switch_virtual = digitalRead(switches[settings.switches_found].pin);  
      
      #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_TST(PSTR("Switch %d %d %d"), pin_id, settings.switches_found, switches[settings.switches_found].pin);
      #endif // ENABLE_LOG_LEVEL_INFO
      
      if(settings.switches_found++ >= MAX_SWITCHES){ break; }

    } // if PinUsed

  }

  if(TickerSwitch == nullptr){
    TickerSwitch = new Ticker();
  }

  if (settings.switches_found) { 

    #ifdef ESP288
    TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );

    #else // esp32
    // TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
    //   [this](void){
    //     this->SwitchProbe();
    //   }
    // );

    TickerSwitch->attach_ms(
      SWITCH_PROBE_INTERVAL, 


      +[](mSwitches* testInstance){ testInstance->SwitchProbe();}, this); //hacky solution to be fixed

    #endif
  }

}




// /*********************************************************************************************/

void mSwitches::SwitchProbe(void)
{
  
  if (pCONT_time->uptime_seconds_nonreset < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t state_filter = pCONT_set->Settings.switch_debounce / SWITCH_PROBE_INTERVAL;   // 5, 10, 15
  uint8_t force_high = (pCONT_set->Settings.switch_debounce % 50) &1;                   // 51, 101, 151 etc
  uint8_t force_low = (pCONT_set->Settings.switch_debounce % 50) &2;                    // 52, 102, 152 etc

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {

    // if (pCONT_pins->PinUsed(GPIO_SWT1_ID,i)) {      
    if(switches[i].pin != -1){
      
      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(switches[i].pin)) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == switches[i].switch_virtual) {
            switches[i].switch_state_buf = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (switches[i].switch_state_buf < state_filter) {
          switches[i].switch_state_buf++;
          if (state_filter == switches[i].switch_state_buf) {
            switches[i].switch_virtual = 1;
          }
        }

      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == switches[i].switch_virtual) {
            switches[i].switch_state_buf = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (switches[i].switch_state_buf > 0) {
          switches[i].switch_state_buf--;
          if (0 == switches[i].switch_state_buf) {
            switches[i].switch_virtual = 0;
          }
        }
        
      } // END if digitalRead

    } // if PinUsed
  } // for all switches

  // TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
  //   [this](void){
  //     this->SwitchProbe();
  //   }
  // );
    #ifdef ESP288
    TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );

    #else // esp32

    TickerSwitch->attach_ms(
      SWITCH_PROBE_INTERVAL, 


      +[](mSwitches* testInstance){ testInstance->SwitchProbe();}, this); //hacky solution to be fixed


    //   +[this](void){
    //     this->SwitchProbe();
    //   }
    // );

    #endif

}



/**
 * @brief 
 * Actually not right for rules, I should probably make "SwitchMode_GetID_by_Name" and "GetStateNumber" together
 * 
 * @param c 
 * @return int16_t 
 */
int16_t mSettings::SwitchMode_GetID_by_Name(const char* c)
// D_DATE_TIME_SEPARATOR
// RuleCommand? I need to be able to react to trigger, or simply directly set, so needs both switchmode and getstate range, create new LIST
{
  if(*c=='\0'){    return -1; }
  if(strcasecmp_P(c,PM_SWITCHMODE_TOGGLE_CTR)==0){ return SWITCHMODE_TOGGLE_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_CTR)==0){ return SWITCHMODE_FOLLOW_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_INV_CTR)==0){ return SWITCHMODE_FOLLOW_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_CTR)==0){ return SWITCHMODE_PUSHBUTTON_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTON_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)==0){ return SWITCHMODE_PUSHBUTTON_TOGGLE_ID; }
  return -1;
}


const char* mSettings::SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){
    default:
    case SWITCHMODE_TOGGLE_ID:                memcpy_P(buffer, PM_SWITCHMODE_TOGGLE_CTR, sizeof(PM_SWITCHMODE_TOGGLE_CTR)); break;
    case SWITCHMODE_FOLLOW_ID:                memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_CTR, sizeof(PM_SWITCHMODE_FOLLOW_CTR)); break; 
    case SWITCHMODE_FOLLOW_INV_ID:            memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_INV_CTR, sizeof(PM_SWITCHMODE_FOLLOW_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_ID:            memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_INV_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_INV_ID:    memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_TOGGLE_ID:     memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)); break; 
  }
  return buffer;
}


/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void mSwitches::SwitchHandler(uint8_t mode) //mode needs removed
{
  if (pCONT_time->uptime_seconds_nonreset < 4) { return; }  
DEBUG_LINE_HERE

  uint8_t state = SWITCH_NOT_PRESSED_ID;
  uint8_t switchflag;
  // DEBUG_LINE_HERE
  // Serial.printf("Settings.switch_debounce %d\n\r", pCONT_set->Settings.switch_debounce);
  // Serial.flush();
  uint16_t loops_per_second = safeDivideWithDefault(1000, pCONT_set->Settings.switch_debounce, 20);
  // Serial.printf("loops_per_second %d\n\r", loops_per_second);
  // DEBUG_LINE_HERE
  uint8_t active_state = LOW;

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if (
      (switches[i].pin != -1) // pCONT_pins->PinUsed(GPIO_SWT1_ID,i)
      || (mode)
    ){

      if (switches[i].holdwallswitch) 
      {
        switches[i].holdwallswitch--;
        if (0 == switches[i].holdwallswitch) 
        {
         // SendKey(1, i +1, 3);           // Execute command via MQTT
        }
      }

      state = switches[i].switch_virtual;

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

      if (state != switches[i].lastwallswitch) 
      {
        switches[i].ischanged = true;

        ALOG_INF( PSTR(D_LOG_SWITCHES "#%d Changed : Level %d | %s"), 
                  i, 
                  state,
                  state==active_state?"ACTIVE":"Not Active"
        );
      
        ALOG_INF( PSTR("state%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),state,i,switches[i].lastwallswitch);
        
        switchflag = 3;
        switch (pCONT_set->Settings.switchmode[i]) {
        case SWITCHMODE_TOGGLE_ID:
          switchflag = POWER_TOGGLE;    // Toggle
          break;
        case SWITCHMODE_FOLLOW_ID:
          switchflag = state &1;        // Follow wall switch state
          break;
        case SWITCHMODE_FOLLOW_INV_ID:
          switchflag = ~state &1;       // Follow inverted wall switch state
          break;
        case SWITCHMODE_PUSHBUTTON_ID:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTON_INV_ID:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTON_TOGGLE_ID:
          if (state != switches[i].lastwallswitch) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case SWITCHMODE_PUSHBUTTONHOLD_ID:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
          }
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch) && (switches[i].holdwallswitch)) {
            switches[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTONHOLD_INV_ID:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
          }
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch) && (switches[i].holdwallswitch)) {
            switches[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        }

        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        mqtthandler_sensor_teleperiod.flags.SendNow = true;

        if (switchflag < 3) 
        {
          #ifndef ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG

            #ifdef USE_MODULE_CORE_RULES
              // Active high means start of motion always, so check for inversion
              uint8_t new_state = switches[i].active_state_value == LOW ? /*invert*/ !state : /*else, just follow*/ state;
              DEBUG_LINE_HERE
              ALOG_INF( PSTR("switchflag=%d, new_state=%d, state=%d"),switchflag,new_state,state);
    
              pCONT_rules->NewEventRun_NumArg(
                D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
                TASK_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
                i, // SWitch index
                1, // Embedded data length
                new_state); // Event has occured, save and check it            
DEBUG_LINE_HERE
            #endif
            
          #endif // ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG
        }

        switches[i].lastwallswitch = state;
        switches[i].state = state;

      }
    }
    else
    {
      switches[i].ischanged = false;
    }
  }
}

void mSwitches::SwitchLoop(void)
{
  if (settings.switches_found) {
    if(mTime::TimeReached(&switch_debounce, pCONT_set->Settings.switch_debounce)){
      SwitchHandler(0);
    }
  }
}



bool mSwitches::IsSwitchActive(uint8_t id){

  // #ifdef ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE

  if( // Is Active?
    switches[id].switch_virtual ==     // internal switch state
    switches[id].active_state_value    // logical value when the switch is considered on, ie, active low 0==0, active high 1==1 
  ){
    return true;
  }
  return false;

  // #else // ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE // PHASE OUT IF NOT NEEDED
  // // Needs to know what type the button is, low, high, no pullup etc
  // if(switches[id].lastwallswitch){
  //   return switches[id].active_state_value ? false : true;
  // }
  // return switches[id].active_state_value ? true : false;  
  // #endif // ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE

}


/********************************************************************************************/

void mSwitches::SetPullupFlag(uint16_t switch_bit)
{
  bitSet(switch_no_pullup, switch_bit);
}

uint8_t mSwitches::GetLastState(uint8_t index)
{
  return switches[index].lastwallswitch;
}

void mSwitches::SetVirtual(uint8_t index, uint8_t state)
{
  switches[index].switch_virtual = state;
}

uint8_t mSwitches::GetVirtual(uint8_t index)
{
  return switches[index].switch_virtual;
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_SENSOR_COUNT, settings.switches_found);

    // JBI->Add("pin0", switches[0].pin);
    // JBI->Add("pin1", switches[1].pin);
    // JBI->Add("pin2", switches[2].pin);
    // JBI->Add("read0", digitalRead(switches[0].pin));
    // JBI->Add("read1", digitalRead(switches[1].pin));
    // JBI->Add("read2", digitalRead(switches[2].pin));
    
  return JBI->End();

}

uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[50]; 
  
  JBI->Start();

  for(uint8_t sensor_id=0;sensor_id<settings.switches_found;sensor_id++){
    if(switches[sensor_id].ischanged || (json_level>JSON_LEVEL_IFCHANGED) ){ 
      
      JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));
        JBI->Add(D_JSON_STATE, IsSwitchActive(sensor_id));
        JBI->Add(D_JSON_STATE "_ctr", IsSwitchActive(sensor_id)?"On":"Off");


        JBI->Add("digitalRead", digitalRead(switches[sensor_id].pin));

        


        JBI->Add("ischanged", switches[sensor_id].ischanged);
        JBI->Add("lastwallswitch", switches[sensor_id].lastwallswitch);
        JBI->Add("holdwallswitch", switches[sensor_id].holdwallswitch);
        JBI->Add("switch_state_buf", switches[sensor_id].switch_state_buf);
        JBI->Add("switch_virtual", switches[sensor_id].switch_virtual);
        // JBI->Add("is_linked_to_internal_relay", switches[sensor_id].is_linked_to_internal_relay);
        JBI->Add("linked_internal_relay_id", switches[sensor_id].linked_internal_relay_id);
      JBI->Object_End();
      
    }

  }

  return JBI->End();

}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSwitches::MQTTHandler_Init(){

  struct handler<mSwitches>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"

#endif // USE_MODULE_NETWORK_MQTT



#endif