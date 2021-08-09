
#include "mSwitches.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

const char* mSwitches::PM_MODULE_SENSORS_SWITCHES_CTR = D_MODULE_SENSORS_SWITCHES_CTR;
const char* mSwitches::PM_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR = D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR;

int8_t mSwitches::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    case FUNC_INIT:
      SwitchInit();
    break;
    case FUNC_LOOP: 
      SwitchLoop();
    break;
    case FUNC_EVERY_SECOND:
   
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
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break; 
    #endif //USE_MODULE_NETWORK_WEBSERVER

  }

}



void mSwitches::SwitchInit(void)
{

  // Init states
  for(uint8_t pin_id=0;pin_id<MAX_SWITCHES;pin_id++){
    switches[pin_id].lastwallswitch = 1;  // Init global to virtual switch state;
    switches[pin_id].active_state_value = 1; // default is active high
    switches[pin_id].switch_virtual = switches[pin_id].lastwallswitch;
  }

  // Check all possible pin options
  settings.switches_found = 0;    
  for(uint8_t pin_id=GPIO_SWT1_ID;pin_id<GPIO_SWT1_ID+(MAX_SWITCHES*4);pin_id++){

        // Serial.printf("pin=%d/%d\n\r",pin_id,GPIO_SWT1_ID+(MAX_SWITCHES*4));
    if(pCONT_pins->PinUsed(pin_id)){
        // Serial.printf("PinUsed\t\tpin=%d\n\r",pin_id);
      
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
        // Serial.printf("GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);
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
        // Serial.printf("NO MATCH GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);

      }

      // Set global now so doesn't change the saved power state on first switch check
      switches[settings.switches_found].lastwallswitch = digitalRead(switches[settings.switches_found].pin);  
      
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR("Switch %d %d %d"), pin_id, settings.switches_found, switches[settings.switches_found].pin);
      #endif // ENABLE_LOG_LEVEL_INFO
      
      if(settings.switches_found++ >= MAX_SWITCHES){ break; }

    } // if PinUsed

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

  uint8_t state = SWITCH_NOT_PRESSED_ID;
  uint8_t switchflag;
  uint16_t loops_per_second = 1000 / pCONT_set->Settings.switch_debounce;
  uint8_t active_state = LOW;

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if (
      (switches[i].pin != -1) // pCONT_pins->PinUsed(GPIO_SWT1_ID,i)
      || (mode)
    ){

      if (switches[i].holdwallswitch) {
        switches[i].holdwallswitch--;
        if (0 == switches[i].holdwallswitch) {
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

        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_SWITCHES "#%d Changed : Level %d | %s"), 
                              i, 
                              state,
                              state==active_state?"ACTIVE":"Not Active"
        );
      
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("state%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),state,i,switches[i].lastwallswitch);
        
        switchflag = 3;
        switch (pCONT_set->Settings.switchmode[i]) {
        case TOGGLE:
          switchflag = POWER_TOGGLE;    // Toggle
          break;
        case FOLLOW:
          switchflag = state &1;        // Follow wall switch state
          break;
        case FOLLOW_INV:
          switchflag = ~state &1;       // Follow inverted wall switch state
          break;
        case PUSHBUTTON:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTON_INV:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case PUSHBUTTON_TOGGLE:
          if (state != switches[i].lastwallswitch) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case PUSHBUTTONHOLD:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
          }
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch) && (switches[i].holdwallswitch)) {
            switches[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTONHOLD_INV:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10;
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
          #ifdef USE_MODULE_CORE_RULES
            pCONT_rules->NewEvent(EM_MODULE_SENSORS_SWITCHES_ID, i, switchflag);
          #endif
          pCONT->Tasker_Interface(FUNC_EVENT_INPUT_STATE_CHANGED_ID);
        }

        switches[i].lastwallswitch = state;
        switches[i].state = state;

      }
    }else{
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
// Needs to know what type the button is, low, high, no pullup etc
  if(switches[id].lastwallswitch){
    return switches[id].active_state_value ? false : true;
  }
  return switches[id].active_state_value ? true : false;  
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

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.switches_found);

    // JsonBuilderI->Add("pin0", switches[0].pin);
    // JsonBuilderI->Add("pin1", switches[1].pin);
    // JsonBuilderI->Add("pin2", switches[2].pin);
    // JsonBuilderI->Add("read0", digitalRead(switches[0].pin));
    // JsonBuilderI->Add("read1", digitalRead(switches[1].pin));
    // JsonBuilderI->Add("read2", digitalRead(switches[2].pin));
    
  return JsonBuilderI->End();

}

uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level){

  char buffer[50]; 

  JsonBuilderI->Start();

  for(uint8_t sensor_id=0;sensor_id<settings.switches_found;sensor_id++){
    if(switches[sensor_id].ischanged || (json_level>JSON_LEVEL_IFCHANGED) ){ 
      
      JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_SWITCHES_ID, sensor_id, buffer, sizeof(buffer)));
        JsonBuilderI->Add(D_JSON_STATE, IsSwitchActive(sensor_id));
        JsonBuilderI->Add(D_JSON_STATE "_ctr", IsSwitchActive(sensor_id)?"On":"Off");
        JsonBuilderI->Add("ischanged", switches[sensor_id].ischanged);
        JsonBuilderI->Add("lastwallswitch", switches[sensor_id].lastwallswitch);
        JsonBuilderI->Add("holdwallswitch", switches[sensor_id].holdwallswitch);
        JsonBuilderI->Add("switch_state_buf", switches[sensor_id].switch_state_buf);
        JsonBuilderI->Add("switch_virtual", switches[sensor_id].switch_virtual);
        // JsonBuilderI->Add("is_linked_to_internal_relay", switches[sensor_id].is_linked_to_internal_relay);
        JsonBuilderI->Add("linked_internal_relay_id", switches[sensor_id].linked_internal_relay_id);
      JsonBuilderI->Level_End();
      
    }

  }

  return JsonBuilderI->End();

}


#endif