
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

      // AddLog(LOG_LEVEL_INFO,PSTR("swt=%s"),IsSwitchActive(0)?"On":"Off");
      // AddLog(LOG_LEVEL_INFO,PSTR("swt=%d"), digitalRead(5) );
      
      
      


   
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
      MQTTHandler_Set_RefreshAll();
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
      switches[settings.switches_found].switch_virtual = digitalRead(switches[settings.switches_found].pin);  
      
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
// Bonus: After thinking about it for a second, you could use a lambda instead of creating the function (note you need to have a + sign before the lambda in order for it to work as a function pointer). This would look like:

// #include <Arduino.h>
// #include <Test.h>
// #include <functional>

// // for ESP8266: https://github.com/esp8266/Arduino/blob/master/libraries/Ticker/src/Ticker.h
// // for ESP32:   https://github.com/espressif/arduino-esp32/blob/master/libraries/Ticker/src/Ticker.h
// #include <Ticker.h>

// Ticker ticker;

// void Test::start(){
//   ticker.once(5, +[](Test* testInstance) { testInstance->onTickerCallback(); }, this);
// }

// void Test::onTickerCallback() {
//   doSomething();
// }

// void Test::doSomething() {
//   Serial.println("Hello");
// }
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

        ALOG_DBM( PSTR(D_LOG_SWITCHES "#%d Changed : Level %d | %s"), 
                              i, 
                              state,
                              state==active_state?"ACTIVE":"Not Active"
        );
      
        ALOG_DBM( PSTR("state%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),state,i,switches[i].lastwallswitch);
        
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
            
            ALOG_DBM( PSTR("switchflag=%d, new_state=%d, state=%d"),switchflag,new_state,state);


            // #ifdef ENABLE_RULES_TRIGGER_METHOD_V2
              // pCONT_rules->NewEventRun(E M_MODULE_SENSORS_SWITCHES_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, i, new_state); // Event has occured, save and check it            
             pCONT_rules->NewEventRun_NumArg(
               D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
               FUNC_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
               i, // SWitch index
               1, // Embedded data length
               new_state); // Event has occured, save and check it            



            // #else // OLD METHOD, to delete
            //   pCONT_rules->NewEvent(E M_MODULE_SENSORS_SWITCHES_ID, i, new_state); // Event has occured, save and check it
            //   pCONT->Tasker_Interface(FUNC_EVENT_INPUT_STATE_CHANGED_ID); // This should maybe be rolled into "NewEvent" so NewEvent of switch would automtically call this        
            // #endif

          #endif
          
          // This is the manual way of starting rules, but probably should be phased out?

          /**
           * @brief 
           * The "NewEvent" if rules are enabled should automatically fire checking of rules
           * 
           */

          #endif // ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG
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
      
      JsonBuilderI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));
        JsonBuilderI->Add(D_JSON_STATE, IsSwitchActive(sensor_id));
        JsonBuilderI->Add(D_JSON_STATE "_ctr", IsSwitchActive(sensor_id)?"On":"Off");


        JsonBuilderI->Add("digitalRead", digitalRead(switches[sensor_id].pin));

        


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