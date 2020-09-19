
// KEEP - useful

//   sonoff.h - Master header file for Sonoff-Tasmota

//   Copyright (C) 2019  Theo Arends

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.

//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.

//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.


//#ifdef ESP8266
#ifndef _MSWITCHES_H_
#define _MSWITCHES_H_



#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

// #ifdef ESP8266
// #define SWITCH_V2
// #ifdef SWITCH_V2
// /*********************************************************************************************\
//  * Switch support with input filter
//  *
//  * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
// \*********************************************************************************************/

#include <Ticker.h>



#include "1_ConfigUser/mUserConfig.h"


#include "2_CoreSystem/Time/mTime.h"
#include "2_CoreSystem/Logging/mLogging.h"


#include <ArduinoJson.h>
#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/mHardwareTemplates.h"

#include "3_Network/MQTT/mMQTT.h"


#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/Time/mTime.h"


#define SWITCH_PROBE_INTERVAL 10   // Time in milliseconds between switch input probe

#define MAX_SWITCHES 8             // Max number of switches

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>


extern uint8_t switch_state_buf[MAX_SWITCHES];// = { 0 };
extern   uint8_t switch_virtual[MAX_SWITCHES];       // Virtual switch states
extern Ticker* TickerSwitch;// = nullptr;


enum SwitchStates { PRESSED, NOT_PRESSED };

class mSwitches{
  public:
    mSwitches(){};
    
    int8_t Tasker(uint8_t function);
    void init(void);



bool IsSwitchActive(uint8_t id);

void WebAppend_Root_Draw_Table();
void WebAppend_Root_Status_Table();

  uint32_t tSavedSwitchProbe = millis();
    uint32_t switch_debounce = 0;          // Switch debounce timer
    uint16_t switch_no_pullup = 0;              // Switch pull-up bitmask flags
    uint8_t lastwallswitch[MAX_SWITCHES];       // Last wall switch states
    uint8_t holdwallswitch[MAX_SWITCHES] = { 0 };  // Timer for wallswitch push button hold
    uint8_t switches_found = 0;
    
    void SwitchPullupFlag(uint16 switch_bit);
    uint8_t SwitchLastState(uint8_t index);
    void SwitchSetVirtual(uint8_t index, uint8_t state);
    uint8_t SwitchGetVirtual(uint8_t index);


void SwitchHandler(uint8_t mode);
void SwitchInit(void);
    void SwitchLoop();
static void SwitchProbe(void);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSwitches>* mqtthandler_ptr;
    const char* postfix_topic_settings = "settings";
    struct handler<mSwitches> mqtthandler_settings_teleperiod;
    const char* postfix_topic_sensors = "sensors";
    struct handler<mSwitches> mqtthandler_sensor_ifchanged;
    struct handler<mSwitches> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    

};



#endif

#endif  // _SONOFF_H_
//#endif