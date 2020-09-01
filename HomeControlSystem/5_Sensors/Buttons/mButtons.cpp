/*
  support.ino - support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//#ifdef ESP8266

#include "mButtons.h"

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_BUTTONS


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
  buttons_found = 0;
  for (uint8_t i = 0; i < MAX_KEYS; i++) {
    if (pCONT_set->pin[GPIO_KEY1_ID +i] < 99) {
      buttons_found++;
      pinMode(pCONT_set->pin[GPIO_KEY1_ID +i], 
        bitRead(key_no_pullup, i) ? INPUT : ((16 == pCONT_set->pin[GPIO_KEY1_ID +i]) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
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
  
  if (pCONT->mt->uptime.seconds_nonreset < 4) { return; } // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t button = NOT_PRESSED;
  uint8_t button_present = 0;
  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Extent hold time factor in case of iminnent Reset command
  uint16_t loops_per_second = 1000 / pCONT_set->Settings.button_debounce;
  char scmnd[20];

 uint8_t maxdev = (pCONT_set->devices_present > MAX_KEYS) ? MAX_KEYS : pCONT_set->devices_present;
 for (uint8_t button_index = 0; button_index < maxdev; button_index++) {
  for (uint8_t button_index = 0; button_index < MAX_KEYS; button_index++) {
    button = NOT_PRESSED;
    button_present = 0;

    // if (!button_index && ((MODULE_SONOFF_DUAL == my_module_type) || (MODULE_CH4 == my_module_type))) {
    //   button_present = 1;
    //   if (dual_button_code) {
    //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON " " D_CODE " %04X"), dual_button_code);
    //     button = PRESSED;
    //     if (0xF500 == dual_button_code) {                      // Button hold
    //       holdbutton[button_index] = (loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10) -1;
    //       hold_time_extent = 1;
    //     }
    //     dual_button_code = 0;
    //   }
    // } else {
      if (pCONT_pins->PinUsed(GPIO_KEY1_ID,button_index)) {
        button_present = 1;
        button = (digitalRead(pCONT_set->pin[GPIO_KEY1_ID + button_index]) != bitRead(key_inverted, button_index));
      }
    // }

    if (button_present) {
      // XdrvMailbox.index = button_index;
      // XdrvMailbox.payload = button;
//       if (XdrvCall(FUNC_BUTTON_PRESSED)) {
//         // Serviced
//       }
//       else if (SONOFF_4CHPRO == my_module_type) {
//         if (holdbutton[button_index]) { holdbutton[button_index]--; }

//         bool button_pressed = false;
//         if ((PRESSED == button) && (NOT_PRESSED == lastbutton[button_index])) {
//           AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), button_index +1);
//           holdbutton[button_index] = loops_per_second;
//           button_pressed = true;
//         }
//         if ((NOT_PRESSED == button) && (PRESSED == lastbutton[button_index])) {
//           AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_01), button_index +1);
//           if (!holdbutton[button_index]) { button_pressed = true; }  // Do not allow within 1 second
//         }
//         if (button_pressed) {
//           if (!SendKey(0, button_index +1, POWER_TOGGLE)) {    // Execute Toggle command via MQTT if ButtonTopic is set
//             //ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
//           }
//         }
//       }
//       else {
        if ((PRESSED == button) && (NOT_PRESSED == lastbutton[button_index])) {
          // if (pCONT_set->Settings.flag_system_phaseout.button_single) {                   // Allow only single button press for immediate action
            AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_IMMEDIATE), button_index +1);
            //if (!SendKey(0, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
              #ifdef USE_MODULE_DRIVERS_RELAY
              pCONT->mry->ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
              //temp fix
              delay(200); //stop quick relay toggling
              #endif

            //}
          // } else {
          //   multipress[button_index] = (multiwindow[button_index]) ? multipress[button_index] +1 : 1;
          //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), button_index +1, multipress[button_index]);
          //   multiwindow[button_index] = loops_per_second / 2;  // 0.5 second multi press window
          // }
          // blinks = 201;
        }

        if (NOT_PRESSED == button) {
          holdbutton[button_index] = 0;
        } else {
          holdbutton[button_index]++;
          if (pCONT_set->Settings.flag_system_phaseout.button_single) {                   // Allow only single button press for immediate action
            if (holdbutton[button_index] == loops_per_second * hold_time_extent * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
             pCONT_set->Settings.flag_system_phaseout.button_single = 0;
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_SETOPTION "13 0"));  // Disable single press only
              // ExecuteCommand(scmnd, SRC_BUTTON);
            }
          } else {
            if (pCONT_set->Settings.flag_system_phaseout.button_restrict) {               // Button restriction
              if (holdbutton[button_index] == loops_per_second * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button hold
                multipress[button_index] = 0;
                // SendKey(0, button_index +1, 3);                // Execute Hold command via MQTT if ButtonTopic is set
              }
            } else {
              if (holdbutton[button_index] == loops_per_second * hold_time_extent * pCONT_set->Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
                multipress[button_index] = 0;
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_RESET " 1"));
                Serial.println("WARNING -- Disabled \"ExecuteCommand(scmnd, SRC_BUTTON);\" command from support_button");
                //ExecuteCommand(scmnd, SRC_BUTTON);
              }
            }
          }
        }

        if (!pCONT_set->Settings.flag_system_phaseout.button_single) {                    // Allow multi-press
          if (multiwindow[button_index]) {
            multiwindow[button_index]--;
          } else {
            // if (!restart_flag && !holdbutton[button_index] && (multipress[button_index] > 0) && (multipress[button_index] < MAX_BUTTON_COMMANDS +3)) {
            //   bool single_press = false;
            //   if (multipress[button_index] < 3) {              // Single or Double press
            //     if ((SONOFF_DUAL_R2 == my_module_type) || (SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
            //       single_press = true;
            //     } else  {
            //       single_press = (pCONT_set->Settings.flag_system_phaseout.button_swap +1 == multipress[button_index]);
            //       multipress[button_index] = 1;
            //     }
            //   }
            //   if ((MI_DESK_LAMP == my_module_type) && (button_index == 0) && (rotary_changed)){// && (light_power)) {
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
            //     //     if (!pCONT_set->Settings.flag_system_phaseout.button_restrict) {
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
      } // if (button_present)
    }
    lastbutton[button_index] = button;
  }
}

void mButtons::ButtonLoop(void)
{
  if (buttons_found) {
    if(mSupport::TimeReached(&tsaved_button_debounce, pCONT_set->Settings.button_debounce)){
      ButtonHandler();
    }
  }
}


int8_t mButtons::Tasker(uint8_t function){

  switch(function){
    case FUNC_INIT:
      ButtonInit();
    break;
    case FUNC_LOOP: 
      ButtonLoop();
    break;
    case FUNC_JSON_COMMAND: 
    
    break;
  }

}





/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mButtons::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mButtons::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mButtons::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mButtons::MQTTHandler_Sender(uint8_t mqtt_handler_id){

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
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_BUTTONS_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////

#endif