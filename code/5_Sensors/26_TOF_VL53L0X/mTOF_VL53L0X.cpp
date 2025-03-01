
// #include "mButtons.h"

// #ifdef USE_MODULE_SENSORS_BUTTONS

// const char* mButtons::PM_MODULE_SENSORS_BUTTONS_CTR = D_MODULE_SENSORS_BUTTONS_CTR;
// const char* mButtons::PM_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR = D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR;

// int8_t mButtons::Tasker(uint8_t function, JsonParserObject obj){

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case FUNC_PRE_INIT:
//       Pre_Init();
//     break;
//     case FUNC_INIT:
//       // Init();
//     break;
//   }

//   if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case FUNC_LOOP: 
//       ButtonLoop();
//     break;
//     case FUNC_EVERY_SECOND:

// // pinMode(16, INPUT_PULLDOWN_16);
// //     AddLog(LOG_LEVEL_DEV_TEST, PSTR("mButtons::Tasker %d"),digitalRead(16));

//     break;
//     // case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
//     //  // AddLog(LOG_LEVEL_DEV_TEST, PSTR("mButtons::FUNC_EVENT_INPUT_STATE_CHANGED_ID"));

//     // break;
//     /************
//      * EVENTS SECTION * 
//     *******************/
//     case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
//       // CommandSet_SDCard_OpenClose_Toggle();
   
    
//       // Event for this
//       if(tkr_rules->event_triggered.module_id == D_UNIQUE_MODULE_SENSORS_BUTTONS_ID)
//       {
   
//         ALOG_INF(PSTR("Button State Changed1 : MQTTHandler_Sender"));
   
//         // Send immediately (Button type is held in event, so ConstructJson can make the correct formatted data)
//         mqtthandler_sensor_ifchanged.flags.SendNow = true;
//         MQTTHandler_Sender();
   
//       }   


//     break;
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case FUNC_MQTT_HANDLERS_INIT:
//       MQTTHandler_Init();
//     break;
//     case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
//       MQTTHandler_Set_DefaultPeriodRate();
//     break;
//     case FUNC_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     case FUNC_MQTT_CONNECTED:
//       MQTTHandler_Set_RefreshAll();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//     /************
//      * WEB SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_WEBSERVER
//     case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
//       WebAppend_Root_Draw_Table();
//     break; 
//     case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
//       WebAppend_Root_Status_Table();
//     break;     
//     #endif // USE_MODULE_NETWORK_WEBSERVER
//   }

//   return FUNCTION_RESULT_SUCCESS_ID;

// }



/*
  xsns_45_vl53l0x.ino - VL53L0X time of flight multiple sensors support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz and Adrian Scillato

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

#ifdef USE_I2C
#ifdef USE_VL53L0X
/*********************************************************************************************\
 * VL53L0x time of flight sensor
 *
 * I2C Addres: 0x29
 *********************************************************************************************
 *
 * Note: When using multiple VL53L0X, it is required to also wire the XSHUT pin of all those sensors
 * in order to let Tasmota change by software the I2C address of those and give them an unique address
 * for operation. The sensor don't save its address, so this procedure of changing its address is needed
 * to be performed every restart. The Addresses used for this are 120 (0x78) to 127 (0x7F). In the I2c
 * Standard (https://i2cdevices.org/addresses) those addresses are used by the PCA9685, so take into
 * account they won't work together.
 *
 * The default value of VL53LXX_MAX_SENSORS is set in the file tasmota.h
 * Changing that is backwards incompatible - Max supported devices by this driver are 8
 *
 **********************************************************************************************
 *
 * How to install this sensor: https://www.st.com/resource/en/datasheet/vl53l0x.pdf
 *
 * If you are going to use long I2C wires read this:
 * https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/
 *
\*********************************************************************************************/

#define XSNS_45            45
#define XI2C_31            31  // See I2CDEVICES.md

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define VL53L0X_LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define VL53L0X_HIGH_SPEED
//#define VL53L0X_HIGH_ACCURACY

#define USE_VL_MEDIAN
#define USE_VL_MEDIAN_SIZE 5   // Odd number of samples median detection

#include <Wire.h>
#include "VL53L0X.h"

#define VL53L0X_ADDRESS 0x29
#ifndef VL53L0X_XSHUT_ADDRESS
#define VL53L0X_XSHUT_ADDRESS 0x78
#endif

VL53L0X VL53L0X_device[VL53LXX_MAX_SENSORS];
struct {
  uint16_t distance;
  uint16_t distance_prev;
  uint16_t buffer[5];
  bool ready = false;
  uint8_t index;
} Vl53l0x_data[VL53LXX_MAX_SENSORS];

bool VL53L0X_xshut = false;
bool VL53L0X_detected = false;

#ifdef USE_DEEPSLEEP
bool VL53L0X_standby = false;  // Prevent updating measurments once VL53L0X has been put to standby (just before ESP enters deepsleep)
#endif

/********************************************************************************************/

void Vl53l0Detect(void) {

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i)) {
      pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), OUTPUT);
      digitalWrite(Pin(GPIO_VL53LXX_XSHUT1, i), i==0 ? 1 : 0);
      VL53L0X_xshut = true;
    }
  }

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        if (VL53L0X_xshut) { pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), INPUT); delay(1); }
        if (!I2cSetDevice(VL53L0X_ADDRESS) && !I2cSetDevice((uint8_t)(VL53L0X_XSHUT_ADDRESS+i))) { return; } // Detection for unconfigured OR configured sensor
        if (VL53L0X_device[i].init()) {
            if (VL53L0X_xshut) { VL53L0X_device[i].setAddress((uint8_t)(VL53L0X_XSHUT_ADDRESS+i)); }
            uint8_t addr = VL53L0X_device[i].getAddress();
            if (VL53L0X_xshut) {
                I2cSetActive(addr);
                AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);
            } else {
                I2cSetActiveFound(addr, "VL53L0X");
            }
            VL53L0X_device[i].setTimeout(500);

#if defined VL53L0X_LONG_RANGE
            // lower the return signal rate limit (default is 0.25 MCPS)
            VL53L0X_device[i].setSignalRateLimit(0.1);
            // increase laser pulse periods (defaults are 14 and 10 PCLKs)
            VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
            VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif
#if defined VL53L0X_HIGH_SPEED
            // reduce timing budget to 20 ms (default is about 33 ms)
            VL53L0X_device[i].setMeasurementTimingBudget(20000);
#elif defined VL53L0X_HIGH_ACCURACY
            // increase timing budget to 200 ms
            VL53L0X_device[i].setMeasurementTimingBudget(200000);
#endif
            // Start continuous back-to-back mode (take readings as
            // fast as possible).  To use continuous timed mode
            // instead, provide a desired inter-measurement period in
            // ms (e.g. sensor.startContinuous(100)).
            VL53L0X_device[i].startContinuous();

            Vl53l0x_data[i].ready = true;
            Vl53l0x_data[i].index = 0;
            VL53L0X_detected = true;
            if (!VL53L0X_xshut) { break; }
        } else {
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_FAILED_TO_START), i+1);
        }
    }
  }
}

void Vl53l0Every_250MSecond(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
  if (VL53L0X_standby) return;
#endif
  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        uint16_t dist = VL53L0X_device[i].readRangeContinuousMillimeters();
        if ((0 == dist) || (dist > 2200)) {
            dist = 9999;
        }

#ifdef USE_VL_MEDIAN
        // store in ring buffer
        Vl53l0x_data[i].buffer[Vl53l0x_data[i].index] = dist;
        Vl53l0x_data[i].index++;
        if (Vl53l0x_data[i].index >= USE_VL_MEDIAN_SIZE) {
            Vl53l0x_data[i].index = 0;
        }

        // sort list and take median
        uint16_t tbuff[USE_VL_MEDIAN_SIZE];
        memmove(tbuff, Vl53l0x_data[i].buffer, sizeof(tbuff));
        uint16_t tmp;
        uint8_t flag;
        for (uint32_t ocnt = 0; ocnt < USE_VL_MEDIAN_SIZE; ocnt++) {
            flag = 0;
            for (uint32_t count = 0; count < USE_VL_MEDIAN_SIZE -1; count++) {
            if (tbuff[count] > tbuff[count +1]) {
                tmp = tbuff[count];
                tbuff[count] = tbuff[count +1];
                tbuff[count +1] = tmp;
                flag = 1;
            }
            }
            if (!flag) { break; }
        }
        Vl53l0x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE -1) / 2];
#else
        Vl53l0x_data[i].distance = dist;
#endif
    }
    if (!VL53L0X_xshut) { break; }
  }
}

#ifdef USE_DOMOTICZ
void Vl53l0Every_Second(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
  if (VL53L0X_standby) return;
#endif
  if (abs(Vl53l0x_data[0].distance - Vl53l0x_data[0].distance_prev) > 8) {
    Vl53l0x_data[0].distance_prev = Vl53l0x_data[0].distance;
    float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
    DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
  }
}
#endif  // USE_DOMOTICZ

void Vl53l0Show(boolean json) {
  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    char types[12] = "VL53L0X";
    if (VL53L0X_xshut) {
      snprintf_P(types, sizeof(types), PSTR("VL53L0X%c%d"), IndexSeparator(), i +1);
    }
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
      float distance = (Vl53l0x_data[i].distance == 9999) ? NAN : (float)Vl53l0x_data[i].distance / 10;  // cm
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
#endif
      }
    }
    if (VL53L0X_device[i].timeoutOccurred()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "Timeout waiting for %s"), types);
    }
    if (!VL53L0X_xshut) { break; }
  }
#ifdef USE_DOMOTICZ
  if (json && (0 == TasmotaGlobal.tele_period)){
    float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
    DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
  }
#endif  // USE_DOMOTICZ
}

#ifdef USE_DEEPSLEEP

void VL53L0EnterStandby(void) {
  if (DeepSleepEnabled()) {
    for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
      if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        if (Vl53l0x_data[i].ready) {
          // VL53L0X_device[i].stopContinuous();
          // Calling stopContinuous() does not lead to a stable standby state.
          // The current is approx. 300 µA, but should be much lower.
          // Restart is bumpy and sometimes blocks the startup sequence completely.
          VL53L0X_device[i].init();
          Vl53l0x_data[i].ready = false;
        }
      }
    }
    VL53L0X_standby = true;
  }
}

#endif // USE_DEEPSLEEP

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns45(uint32_t function) {
  if (!I2cEnabled(XI2C_31)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Vl53l0Detect();
  }
  else if (VL53L0X_detected) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Vl53l0Every_250MSecond();
        break;
#ifdef USE_DOMOTICZ
     case FUNC_EVERY_SECOND:
        Vl53l0Every_Second();
        break;
#endif  // USE_DOMOTICZ
      case FUNC_JSON_APPEND:
        Vl53l0Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Vl53l0Show(0);
        break;
#endif  // USE_WEBSERVER
#ifdef USE_DEEPSLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        VL53L0EnterStandby();
        break;
#endif // USE_DEEPSLEEP
    }
  }
  return result;
}

#endif  // USE_VL53L0X
#endif  // USE_I2C



// /********************************************************************************************/

// void mButtons::SetPullupFlag(uint8_t button_bit)
// {
//   bitSet(key_no_pullup, button_bit);
// }

// void mButtons::SetInvertFlag(uint8_t button_bit)
// {
//   bitSet(key_inverted, button_bit);
// }

// bool mButtons::ModuleEnabled()
// {
//   return settings.buttons_found;
// }

// /**
//  * Esp8266 and esp32 will have different pull resistors
//  * */
// uint8_t mButtons::GetHardwareSpecificPullMethod(uint8_t real_pin)
// {

//   #ifdef ESP8266
//   return (real_pin == 16) ? INPUT_PULLDOWN_16 : INPUT_PULLUP;
//   #else //esp32
//   return INPUT_PULLUP;
//   #endif

// }

// void mButtons::Pre_Init(void)
// {
  
//   AddLog(LOG_LEVEL_INFO, PSTR("Pre_Init"));

//   // Check all possible pin options
//   settings.buttons_found = 0;    

//   // Check for special gpio types, store their settings as bitmaps then shift gpio as normal types.

//   // else if ((mpin >= AGPIO(GPIO_KEY1_NP)) && (mpin < (AGPIO(GPIO_KEY1_NP) + MAX_KEYS))) {
//   //   ButtonPullupFlag(mpin - AGPIO(GPIO_KEY1_NP));      //  0 .. 3
//   //   mpin -= (AGPIO(GPIO_KEY1_NP) - AGPIO(GPIO_KEY1));
//   // }
//   // else if ((mpin >= AGPIO(GPIO_KEY1_INV)) && (mpin < (AGPIO(GPIO_KEY1_INV) + MAX_KEYS))) {
//   //   ButtonInvertFlag(mpin - AGPIO(GPIO_KEY1_INV));     //  0 .. 3
//   //   mpin -= (AGPIO(GPIO_KEY1_INV) - AGPIO(GPIO_KEY1));
//   // }
//   // else if ((mpin >= AGPIO(GPIO_KEY1_INV_NP)) && (mpin < (AGPIO(GPIO_KEY1_INV_NP) + MAX_KEYS))) {
//   //   ButtonPullupFlag(mpin - AGPIO(GPIO_KEY1_INV_NP));  //  0 .. 3
//   //   ButtonInvertFlag(mpin - AGPIO(GPIO_KEY1_INV_NP));  //  0 .. 3
//   //   mpin -= (AGPIO(GPIO_KEY1_INV_NP) - AGPIO(GPIO_KEY1));
//   // }

//   // Lets check each type on their own, normal, inverted etc
//   for(uint8_t sensor_index=0; sensor_index<MAX_KEYS; sensor_index++)
//   {
//     if(pCONT_pins->PinUsed(GPIO_KEY1_ID, sensor_index))
//     {
//       buttons[settings.buttons_found].pin = pCONT_pins->GetPin(GPIO_KEY1_ID, sensor_index);

//       // THIS (buttons[settings.buttons_found].pin == 16) ? INPUT_PULLDOWN_16 : INPUT_PULLUP needs moved into a function, since it differs by esp8266 and esp32?
//       pinMode(buttons[settings.buttons_found].pin, GetHardwareSpecificPullMethod(buttons[settings.buttons_found].pin)); // Note: GPIO16/D0 inversion is pulldown, not up
//       buttons[settings.buttons_found].active_state_value = GetHardwareSpecificPullMethod(buttons[settings.buttons_found].pin)==INPUT_PULLUP ? LOW : HIGH;  //if pulls up normally, then grounded is on ie low
      
      
//       buttons[settings.buttons_found].last_state = BUTTON_PRESSED_ID; // so its not "not pressed" on first call
//       // BUTTON_NOT_PRESSED_ID;
      
//       // (digitalRead(buttons[settings.buttons_found].pin)==HIGH)?BUTTON_PRESSED_ID:BUTTON_NOT_PRESSED_ID; 
      
      
//       //  state = (digitalRead(buttons[settings.buttons_found].pin) != 
//       //  bitRead(key_inverted, button_index));
    
//         // (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID,sensor_index)) != bitRead(key_inverted, sensor_index));
      
      
//       if(settings.buttons_found++ >= MAX_KEYS){ break; }
//     }else
//     if(pCONT_pins->PinUsed(GPIO_KEY1_INV_ID, sensor_index))
//     {
//       buttons[settings.buttons_found].pin = pCONT_pins->GetPin(GPIO_KEY1_INV_ID, sensor_index);
//       pinMode(buttons[settings.buttons_found].pin, GetHardwareSpecificPullMethod(buttons[settings.buttons_found].pin));
//       SetInvertFlag(sensor_index); 
//       buttons[settings.buttons_found].active_state_value = GetHardwareSpecificPullMethod(buttons[settings.buttons_found].pin)==INPUT_PULLUP ? LOW : HIGH;
//       buttons[settings.buttons_found].last_state = BUTTON_PRESSED_ID; // = BUTTON_NOT_PRESSED_ID;//(digitalRead(buttons[settings.buttons_found].pin)==LOW)?BUTTON_PRESSED_ID:BUTTON_NOT_PRESSED_ID; 
//       if(settings.buttons_found++ >= MAX_KEYS){ break; }
//     }else
//     if(pCONT_pins->PinUsed(GPIO_KEY1_NP_ID, sensor_index))
//     {
//       buttons[settings.buttons_found].pin = pCONT_pins->GetPin(GPIO_KEY1_NP_ID, sensor_index);
//       pinMode(buttons[settings.buttons_found].pin, INPUT);
//       SetPullupFlag(sensor_index); 
//       buttons[settings.buttons_found].active_state_value = HIGH; 
//       buttons[settings.buttons_found].last_state = BUTTON_PRESSED_ID; // = BUTTON_NOT_PRESSED_ID;//(digitalRead(buttons[settings.buttons_found].pin)==HIGH)?BUTTON_PRESSED_ID:BUTTON_NOT_PRESSED_ID;;  
//       if(settings.buttons_found++ >= MAX_KEYS){ break; }
//     }else
//     if(pCONT_pins->PinUsed(GPIO_KEY1_INV_NP_ID, sensor_index))
//     {
//       buttons[settings.buttons_found].pin = pCONT_pins->GetPin(GPIO_KEY1_INV_NP_ID, sensor_index);
//       pinMode(buttons[settings.buttons_found].pin, INPUT);
//       SetPullupFlag(sensor_index); 
//       SetInvertFlag(sensor_index); 
//       buttons[settings.buttons_found].active_state_value = LOW; 
//       buttons[settings.buttons_found].last_state = BUTTON_PRESSED_ID; // = BUTTON_NOT_PRESSED_ID;//(digitalRead(buttons[settings.buttons_found].pin)==LOW)?BUTTON_PRESSED_ID:BUTTON_NOT_PRESSED_ID;;          
//       if(settings.buttons_found++ >= MAX_KEYS){ break; }
//     }
//   }

//   tkr_set->devices_present += settings.buttons_found;
//   if(settings.buttons_found){ settings.fEnableSensor = true; }

//   // buttons_found = 0;
//   // for (uint8_t i = 0; i < MAX_KEYS; i++) {
//   //   if (pCONT_pins->PinUsed(GPIO_KEY1_ID,i)) {
//   //     buttons_found++;
//   //     AddLog(LOG_LEVEL_INFO, PSTR("buttons_found=%d"),buttons_found-1);
//   //     pinMode(pCONT_pins->GetPin(GPIO_KEY1_ID,i), 
//   //       bitRead(key_no_pullup, i) ? INPUT : ((16 == pCONT_pins->GetPin(GPIO_KEY1_ID,i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
        
//   //     AddLog(LOG_LEVEL_INFO, PSTR("buttons_found pullup=%d %d"),buttons_found-1,
//   //     bitRead(key_no_pullup, i) ? INPUT : ((16 == pCONT_pins->GetPin(GPIO_KEY1_ID,i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP)
//   //     );
//   //   }
//   // }

// }

// // uint8_t mButtons::ButtonSerial(uint8_t serial_in_byte)
// // {
// //   if (dual_hex_code) {
// //     dual_hex_code--;
// //     if (dual_hex_code) {
// //       dual_button_code = (dual_button_code << 8) | serial_in_byte;
// //       serial_in_byte = 0;
// //     } else {
// //       if (serial_in_byte != 0xA1) {
// //         dual_button_code = 0;                // 0xA1 - End of Sonoff dual button code
// //       }
// //     }
// //   }
// //   if (0xA0 == serial_in_byte) {              // 0xA0 - Start of Sonoff dual button code
// //     serial_in_byte = 0;
// //     dual_button_code = 0;
// //     dual_hex_code = 3;
// //   }

// //   return serial_in_byte;
// // }

// /*********************************************************************************************\
//  * Button handler with single press only or multi-press and hold on all buttons
// \*********************************************************************************************/

// // #ifndef ENABLE_DEVFEATURE_BUTTON_HANDLER_V2

// // void mButtons::ButtonHandler(void)
// // {
  
// //   if (tkr_time->uptime.seconds_nonreset < 4) // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
// //   {
// //     return; 
// //   } 

// //   /**
// //    * state is pressed or not, it is ntnot the logical level
// //    * so inversion matters
// //    * */
// //   uint8_t state = BUTTON_NOT_PRESSED_ID;
// //   uint8_t button_present = 0;
// //   uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Extent hold time factor in case of iminnent Reset command
// //   uint16_t loops_per_second = 1000 / tkr_set->Settings.button_debounce;
// //   // char scmnd[20];

// //  uint8_t maxdev = (tkr_set->devices_present > MAX_KEYS) ? MAX_KEYS : tkr_set->devices_present;

// //   // AddLog(LOG_LEVEL_DEV_TEST, PSTR("maxdev=%d"),maxdev);

// // //delay(1000);


// //  for (uint8_t button_index = 0; button_index < maxdev; button_index++)
// //  {
   
// //     state = BUTTON_NOT_PRESSED_ID;
// //     button_present = 0;

// //     if (pCONT_pins->PinUsed(GPIO_KEY1_ID, button_index)) {
// //       button_present = 1;
// //       state = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID,button_index)) != bitRead(key_inverted, button_index));
// //     }else
// //     if (pCONT_pins->PinUsed(GPIO_KEY1_INV_ID, button_index)) {
// //       button_present = 1;
// //       state = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_INV_ID,button_index)) != bitRead(key_inverted, button_index));
// //     }

// //     // AddLog(LOG_LEVEL_DEV_TEST, PSTR("state=%s[%d]%d:%d"),state==BUTTON_PRESSED_ID?"pressed":"NOTpressed",button_index,state,buttons[button_index].last_state);

// //           // AddLog(LOG_LEVEL_DEV_TEST, PSTR("state=%d:%d"),
// //           // // state==BUTTON_PRESSED_ID?"pressed":"NOTpressed",
// //           // // button_index,
          
// //           // state,
// //           // buttons[button_index].last_state
          
// //           // );


// //     if (button_present) {

// //       buttons[button_index].isactive = state;
      
// //       // if (pCONT->Tasker_Interface(FUNC_BUTTON_PRESSED)) {
// //       //   // Serviced internally    //causes state not to work right now, tasker is returning true always
// //       // }
// //       // else 
// //       // {

// //       // new PRESS, was not previously pressed
// //       if ((BUTTON_PRESSED_ID == state) && (BUTTON_NOT_PRESSED_ID == buttons[button_index].last_state)) {
// //         // if (tkr_set->Settings.flag_system.button_single) {                   // Allow only single button press for immediate action
        
// //           // AddLog(LOG_LEVEL_DEV_TEST, PSTR("state=%d:%d"),
// //           // // state==BUTTON_PRESSED_ID?"pressed":"NOTpressed",
// //           // // button_index,
          
// //           // state,
// //           // buttons[button_index].last_state
          
// //           // );


// //         // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_IMMEDIATE), button_index);
// //         //if (!SendKey(0, button_index, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
            
// //         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BUTTONS "#%d Changed : Level %d | %s " D_IMMEDIATE), 
// //                               button_index, 
// //                               state,
// //                               state==BUTTON_PRESSED_ID?"ACTIVE":"Not Active"
// //         );

// //         // delay(1000);
      
// //         #ifdef USE_MODULE_NETWORK_MQTT
// //         mqtthandler_sensor_ifchanged.flags.SendNow = true;
// //         #endif // USE_MODULE_NETWORK_MQTT     
  
// //        // Type method
// //             // AddLog(LOG_LEVEL_INFO,PSTR("tsaved_button_debounce=%d"),tsaved_button_debounce);
// //             // tsaved_button_debounce = millis() + KEY_CHECK_TIME; // Push next read into future // move time forward by 1 second
// //             // AddLog(LOG_LEVEL_INFO,PSTR("tsaved_button_debounce=%d"),tsaved_button_debounce);


// //         // #ifdef USE_MODULE_CORE_RULES
// //         // tkr_rules->NewEvent(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID,button_index,state);
// //         // #endif
// //         // pCONT->Tasker_Interface(FUNC_EVENT_INPUT_STATE_CHANGED_ID);


// //    tkr_rules->NewEventRun(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, button_index, state); // Event has occured, save and check it            
          
// //           //}
// //         // } else {
// //         //   buttons[button_index].multipress = (multiwindow[button_index]) ? buttons[button_index].multipress +1 : 1;
// //         //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), button_index +1, buttons[button_index].multipress);
// //         //   multiwindow[button_index] = loops_per_second / 2;  // 0.5 second multi press window
// //         // }
// //         // blinks = 201;
// //       }

// // //held buttons
// //       if (BUTTON_NOT_PRESSED_ID == state) {
// //         buttons[button_index].hold_timer = 0;
// //       } else {
// //         buttons[button_index].hold_timer++;
// //         if (tkr_set->Settings.flag_system.button_single) {                   // Allow only single button press for immediate action
// //           if (buttons[button_index].hold_timer == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // Button held for factor times longer
// //             tkr_set->Settings.flag_system.button_single = 0;
// //             // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_SETOPTION "13 0"));  // Disable single press only
// //             // ExecuteCommand(scmnd, SRC_BUTTON);
// //           }
// //         } else {
// //           if (tkr_set->Settings.flag_system.button_restrict) {               // Button restriction
// //             if (buttons[button_index].hold_timer == loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // Button hold
// //               buttons[button_index].multipress = 0;
// //               // SendKey(0, button_index, 3);                // Execute Hold command via MQTT if ButtonTopic is set
// //             }
// //           } else {
// //             if (buttons[button_index].hold_timer == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // Button held for factor times longer
// //               buttons[button_index].multipress = 0;
// //               // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_JSON_RESET " 1"));
// //               // Serial.println("WARNING -- Disabled \"ExecuteCommand(scmnd, SRC_BUTTON);\" command from support_button");
// //               //ExecuteCommand(scmnd, SRC_BUTTON);
// //             }
// //           }
// //         }
// //       }

// //       #ifdef ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
// //       if (!tkr_set->Settings.flag_system.button_single) {                    // Allow multi-press
// //         if (multiwindow[button_index]) {
// //           multiwindow[button_index]--;
// //         } else {
// //           // if (!restart_flag && !buttons[button_index].holdbutton && (buttons[button_index].multipress > 0) && (buttons[button_index].multipress < MAX_BUTTON_COMMANDS +3)) {
// //           //   bool single_press = false;
// //           //   if (buttons[button_index].multipress < 3) {              // Single or Double press
// //           //     if ((SONOFF_DUAL_R2 == my_module_type) || (SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
// //           //       single_press = true;
// //           //     } else  {
// //           //       single_press = (tkr_set->Settings.flag_system.button_swap +1 == buttons[button_index].multipress);
// //           //       buttons[button_index].multipress = 1;
// //           //     }
// //           //   }
// //           //   if ((MI_DESK_LAMP == my_module_type) && (button_index == 0) && (rotary_changed)){// && (light_power_state)) {
// //           //     rotary_changed = 0;                            // Color temp changed, no need to turn of the light
// //           //   } else {
// //           //     // if (single_press && SendKey(0, button_index + buttons[button_index].multipress, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
// //           //     //   // Success
// //           //     // } else {
// //           //     //   if (buttons[button_index].multipress < 3) {          // Single or Double press
// //           //     //     if (WifiState() > WIFI_RESTART) {          // WPSconfig, Smartconfig or Wifimanager active
// //           //     //       restart_flag = 1;
// //           //     //     } else {
// //           //     //       //ExecuteCommandPower(button_index + buttons[button_index].multipress, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
// //           //     //     }
// //           //     //   } else {                                     // 3 - 7 press
// //           //     //     if (!tkr_set->Settings.flag_system.button_restrict) {
// //           //     //       snprintf_P(scmnd, sizeof(scmnd), kCommands[buttons[button_index].multipress -3]);
// //           //     //       // ExecuteCommand(scmnd, SRC_BUTTON);
// //           //     //     }
// //           //     //   }
// //           //     // }
// //           //   }
// //           //   buttons[button_index].multipress = 0;
// //           // }
// //         }
// //       }
// //       #endif // ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
// //       // }//if serviced, single button, 4chan
// //     } // if (button_present)
    
// //     buttons[button_index].last_state = state;
// //   }
// // }

// // #endif // ENABLE_DEVFEATURE_BUTTON_HANDLER_V2

// // #ifdef ENABLE_DEVFEATURE_BUTTON_HANDLER_V2

// /*********************************************************************************************\
//  * Button handler with single press only or multi-press and hold on all buttons
//  *
//  * ButtonDebounce (50) - Debounce time in mSec
//  * SetOption1  (0)     - If set do not execute commands WifiConfig and Reset
//  * SetOption11 (0)     - If set perform single press action on double press and reverse (on two relay devices only)
//  * SetOption13 (0)     - If set act on single press only
//  * SetOption73 (0)     - Decouple button from relay and send just mqtt topic
// \*********************************************************************************************/

// /**
//  * @brief 
//  * Button data event with structure
//  * <length of data>,<state>,<type ie single/multi/hold><count>
//  * 
//  */

// void mButtons::ButtonHandler(void) {


//   if (tkr_time->uptime.seconds_nonreset < 4) { return; }                     // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

//   uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;             // Extent hold time factor in case of iminnent Reset command
//   uint16_t loops_per_second = 1000 / tkr_set->Settings.button_debounce;  // ButtonDebounce (50) - How often is the button polled? = 20 per second
//   char scmnd[20];

//   for (uint8_t id = 0; id < MAX_KEYS; id++) 
//   {
//     uint8_t state = BUTTON_NOT_PRESSED_ID;
//     uint8_t button_present = 0;

// //     if (pCONT_pins->PinUsed(GPIO_KEY1_ID, id)) {
// //       button_present = 1;
// // // #ifdef ESP32
// // // #ifndef CONFIG_IDF_TARGET_ESP32C3
// // //       if (bitRead(Button.touch_mask, id)) {          // Touch
// // //         uint32_t _value = touchRead(Pin(GPIO_KEY1_ID, id));
// // //         state = NOT_PRESSED;
// // //         if (_value != 0) {                                     // Probably read-error
// // //           if (_value < TOUCH_BUTTON.pin_threshold) {
// // //             if (++Button.touch_hits[id] > TOUCH_BUTTON.hit_threshold) {
// // //               if (!bitRead(TOUCH_BUTTON.calibration, id+1)) {
// // //                 state = PRESSED;
// // //               }
// // //             }
// // //           } else {
// // //             Button.touch_hits[id] = 0;
// // //           }
// // //         } else {
// // //           Button.touch_hits[id] = 0;
// // //         }
// // //         if (bitRead(TOUCH_BUTTON.calibration, id+1)) {
// // //           AddLog(LOG_LEVEL_INFO, PSTR("PLOT: %u, %u, %u,"), id+1, _value, Button.touch_hits[id]);  // Button number (1..4), value, continuous hits under threshold
// // //         }
// // //       } else
// // // #endif  // not ESP32C3
// // // #endif  // ESP32
// //       {                                                 // Normal button
// //         state = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID, id)) != bitRead(Button.inverted_mask, id));
// //       }
// //     }
    
//     if (pCONT_pins->PinUsed(GPIO_KEY1_ID, id)) {
//       button_present = 1;
//       state = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID,id)) != bitRead(key_inverted, id));
//     }else
//     if (pCONT_pins->PinUsed(GPIO_KEY1_INV_ID, id)) {
//       button_present = 1;
//       state = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_INV_ID,id)) != bitRead(key_inverted, id));
//     }

    
//     // ALOG_INF(PSTR("state =%d"), state);


// // #ifdef USE_ADC
// //     else if (PinUsed(GPIO_ADC_BUTTON, id)) {
// //       button_present = 1;
// //       state = AdcGetButton(Pin(GPIO_ADC_BUTTON, id));
// //     }
// //     else if (PinUsed(GPIO_ADC_BUTTON_INV, id)) {
// //       button_present = 1;
// //       state = AdcGetButton(Pin(GPIO_ADC_BUTTON_INV, id));
// //     }
// // #endif  // USE_ADC


//     if (button_present) 
//     {

//       /**
//        * @brief If button state has changed
//        **/
//       if (
//         (BUTTON_PRESSED_ID == state) && 
//         (BUTTON_NOT_PRESSED_ID == buttons[id].last_state)
//       ){

//         if(tkr_set->Settings.flag_system.button_single) // SetOption13 (0) - Allow only single button press for immediate action
//         {           
//           AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BUTTONS "#%d Changed : Level %d | %s " D_IMMEDIATE), id, state, state==BUTTON_PRESSED_ID?"ACTIVE":"Not Active" );          
//           // <length of data>,<state>,<type ie single/multi/hold><count>  
//           tkr_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, id, 2, state, INPUT_TYPE_SINGLE_PRESS_ID); // 1 press event
//         }
//         else
//         {
//           buttons[id].press_counter = (buttons[id].window_timer) ? buttons[id].press_counter +1 : 1;
//           AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), id +1, buttons[id].press_counter);
//           buttons[id].window_timer = loops_per_second / 2;  // 0.5 second multi press window
//           // No immediate action here -- awaiting for multiple presses            
//         }
                

//       } // if button state changed


//       /**
//        * @brief If the button is not pressed, keep the hold_timer count reset 
//        **/
//       if (BUTTON_NOT_PRESSED_ID == state) {
//         buttons[id].hold_timer = 0;
//       }
//       /**
//        * @brief If button is still held, then increment the hold timer and check if the timeout has been reached
//        * This will be called every buttonloop
//        **/
//       else 
//       {
//         buttons[id].hold_timer++;

//         if (tkr_set->Settings.flag_system.button_single)
//         {           // SetOption13 (0) - Allow only single button press for immediate action
        
//           if (buttons[id].hold_timer == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button held for factor times longer
//             // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_SETOPTION "13 0"));  // Disable single press only             // ExecuteCommand(scmnd, SRC_BUTTON); 
//           // <length of data>,<state>,<type ie single/multi/hold><count>  
//             tkr_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, id, 2, state, INPUT_TYPE_SINGLE_HOLD_ID);    // ERROR - Not sure what this section will do, long press no multi?
//           }
//         } 
//         else 
//         /**
//          * @brief Checking for a long press
//          **/
//         {

//           if (buttons[id].hold_timer == loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button hold
//             buttons[id].press_counter = 0;
          
//             AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BUTTONS "#%d Changed : Level %d | %s " "LongPress of (%d) seconds"), id, state, state==BUTTON_PRESSED_ID?"ACTIVE":"Not Active", tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10 );            
          
//             // 3 = button pressed state, presses of button, type is long press?
//             // <length of data>,<state>,<type ie single/multi/hold><count>  
//             tkr_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, id, 2, state, INPUT_TYPE_SINGLE_HOLD_ID); 
//           }
//           // Long pressed not yet reached 
//           else {
//             if (tkr_set->Settings.flag_system.button_restrict) {     // SetOption1 (0) - Control button multipress
//               if (buttons[id].hold_timer > loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_IGNORE] / 10) {
//                 buttons[id].hold_timer = 0;     // Reset button hold counter to stay below hold trigger
//                 buttons[id].press_counter = 0;  // Discard button press to disable functionality
//               }
//             } else {
//               if ((buttons[id].hold_timer == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10)) {  // SetOption32 (40) - Button held for factor times longer
//                 buttons[id].press_counter = 0;
//                 ALOG_INF(PSTR(D_LOG_BUTTONS "ENABLE_DEVFEATURE_DISABLE_BUTTON_CAN_RESET_DEVICE id=%d"), id);
//                 #ifndef ENABLE_DEVFEATURE_DISABLE_BUTTON_CAN_RESET_DEVICE
//                 // snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
//                 // ExecuteCommand(scmnd, SRC_BUTTON);                
//                 ALOG_INF(PSTR(D_LOG_BUTTONS D_CMND_RESET " 1"));                
//                 tkr_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, id, 2, state, INPUT_TYPE_SINGLE_HOLD_ID);    // Resetting command
//                 #endif // ENABLE_DEVFEATURE_DISABLE_BUTTON_CAN_RESET_DEVICE
//               }
//             }
//           }
//         } // if (tkr_set->Settings.flag_system.button_single)
//       }

//     // #ifdef ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
//       if (!tkr_set->Settings.flag_system.button_single) {            // SetOption13 (0) - Allow multi-press
//         if (buttons[id].window_timer) {
//           buttons[id].window_timer--;
//         } else {

//           /**
//            * @brief Hold timer is counting (pressed) and press count is within range
//            * */
//           if (
//             !tkr_set->restart_flag && 
//             !buttons[id].hold_timer && 
//             (buttons[id].press_counter > 0) && (buttons[id].press_counter < 7)
//           ){
//             /**
//              * @brief What to do with multiple key presses
//              **/
//             // <length of data>,<state>,<type ie single/multi/hold><count>  
//             // Single or Multiple Events 

//             tkr_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, id, 3, state, buttons[id].press_counter == 1 ? INPUT_TYPE_SINGLE_PRESS_ID : INPUT_TYPE_MULTIPLE_PRESS_ID, buttons[id].press_counter);

//             buttons[id].press_counter = 0;
//           }
//         }
//       }
//       // #endif // ENABLE_DEVFEATURE_BUTTON_MULTIPRESS

//     }
//     buttons[id].last_state = state;
//   }
// }

// // #endif // ENABLE_DEVFEATURE_BUTTON_HANDLER_V2





// void mButtons::ButtonLoop(void)
// {

//   if (settings.buttons_found) {
//     if(mTime::TimeReached(&tsaved_button_debounce, tkr_set->Settings.button_debounce)){
//       ButtonHandler();
//     }
//   }

// }



// bool mButtons::IsButtonActive(uint8_t id){
// // Needs to know what type the button is, low, high, no pullup etc
//   if(buttons[id].isactive){ 
//     return true;
//   }
//   return false;

// }

// char* mButtons::IsButtonActiveCtr(uint8_t id, char* buffer, uint8_t buflen){
//   if(IsButtonActive(id)){
//     snprintf_P(buffer, buflen, "%s", PM_PRESSED);
//   }else{
//     snprintf_P(buffer, buflen, "%s", PM_NOT_PRESSED);
//   }
//   return buffer;
// }



  
// /******************************************************************************************************************
//  * Commands
// *******************************************************************************************************************/

  
// /******************************************************************************************************************
//  * ConstructJson
// *******************************************************************************************************************/

// uint8_t mButtons::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

//   JsonBuilderI->Start();
//     JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.buttons_found);
//   return JsonBuilderI->End();

// }

// uint8_t mButtons::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

//   JsonBuilderI->Start();
//     // JsonBuilderI->Array_AddArray("lastbutton", lastbutton, sizeof(lastbutton));

//     JBI->Level_Start("ButtonPressed");
//       JBI->Add("IsButtonActiveCtr", buttons[0].isactive);
//     JBI->Level_End();


//     /**
//      * @brief New method to show type of press (short/long/multi)
//      **/
//     JBI->Level_Start("Event"); // asumes only one button at a time, will need nicer formatting later (arrays?)
//       JBI->Add("ID", tkr_rules->event_triggered.device_id);
//       // JBI->Add("func", tkr_rules->event_triggered.function_id);
//       // JBI->Array_AddArray("data1", tkr_rules->event_triggered.value.data, tkr_rules->event_triggered.value.length);

//       // [state][type][opt. count]
//       if(tkr_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_PRESS_ID)
//       {
//         JBI->Add("type", "Single");
//         JBI->Add("count", 1);
//       }
//       if(tkr_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_HOLD_ID)
//       {
//         JBI->Add("type", "Hold");
//         JBI->Add("count", tkr_rules->event_triggered.value.data[2]);
//       }
//       if(tkr_rules->event_triggered.value.data[1] == INPUT_TYPE_MULTIPLE_PRESS_ID)
//       {
//         JBI->Add("type", "Multiple");
//         JBI->Add("count", tkr_rules->event_triggered.value.data[2]);
//       }

//     JBI->End();



//     JBI->Add("pin",    buttons[0].pin);
//     JBI->Add("dpin",   digitalRead(buttons[0].pin));
//     JBI->Array_Start("bit_set_invert");
//       for(int i=0;i<MAX_KEYS;i++){ JBI->Add(bitRead(key_inverted, i)); }
//     JBI->Array_End();
//     JBI->Array_Start("state");
//       for(int i=0;i<MAX_KEYS;i++){ JBI->Add(buttons[i].state); }
//     JBI->Array_End();

//   return JsonBuilderI->End();

// }
  
// /******************************************************************************************************************
//  * MQTT
// *******************************************************************************************************************/

// #ifdef USE_MODULE_NETWORK_MQTT

// void mButtons::MQTTHandler_Init(){

//   struct handler<mButtons>* ptr;

//   ptr = &mqtthandler_settings_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_MIN; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mButtons::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_MIN; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 10; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mButtons::MQTTHandler_Set_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mButtons::MQTTHandler_Set_DefaultPeriodRate()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = tkr_set->Settings.sensors.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = tkr_set->Settings.sensors.ifchanged_secs;
//   }
// }

// /**
//  * @brief MQTTHandler_Sender
//  * */
// void mButtons::MQTTHandler_Sender(uint8_t id)
// {    
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_BUTTONS_ID, handle, id);
//   }
// }
  
// #endif// USE_MODULE_NETWORK_MQTT

// /******************************************************************************************************************
//  * WebServer
// *******************************************************************************************************************/

//     #ifdef USE_MODULE_NETWORK_WEBSERVER

// void mButtons::WebAppend_Root_Draw_Table(){

//   tkr_web->WebAppend_Root_Draw_Table_Repeat_Row_Name_Numbers(settings.buttons_found,"button_table", "Button");


// }

// //append to internal buffer if any root messages table
// void mButtons::WebAppend_Root_Status_Table(){

//   char buffer[20];

//   JsonBuilderI->Array_Start_P(PM_WEB_HANDLE_DIV_NAME_BUTTON_TABLE_CTR);// Class name
//   for(int row=0;row<settings.buttons_found;row++){
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,row);
//       JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,IsButtonActiveCtr(row, buffer, sizeof(buffer)));//"\"%s\"", IsButtonActiveCtr(row, buffer, sizeof(buffer)));
//       if(IsButtonActive(row)){
//         JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#00ff00"); //make webcolours dlist in progmem!
//       }else{
//         JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#ff0000");
//       }
    
//     JsonBuilderI->Level_End();
//   }
//   JsonBuilderI->Array_End();
  
// }

//     #endif // USE_MODULE_NETWORK_WEBSERVER






// #endif