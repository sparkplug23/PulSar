/*
  mIRRemote - infra red support

  Copyright (C) 2022  Heiko Krupp, Lazar Obradovic, Theo Arends and Michael Doone

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

#ifndef HEADER_MODULE_DRIVERS_IRREMOTE_H
#define HEADER_MODULE_DRIVERS_IRREMOTE_H
  
#define D_UNIQUE_MODULE_DRIVERS_IRREMOTE_ID 135

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_IRREMOTE

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

// #include <IRremoteESP8266.h>
// #include <IRutils.h>


/*********************************************************************************************\
 * IR Send
\*********************************************************************************************/

// #include <IRsend.h>


// #ifdef USE_IR_RECEIVE
/*********************************************************************************************\
 * IR Receive
\*********************************************************************************************/

const bool IR_RCV_SAVE_BUFFER = false;         // false = do not use buffer, true = use buffer for decoding

#ifndef IR_TIME_AVOID_DUPLICATE
#define IR_TIME_AVOID_DUPLICATE 50           // Milliseconds
#endif  // IR_TIME_AVOID_DUPLICATE

// #include <IRrecv.h>

// #endif



/*********************************************************************************************\
 * Class used to make a compact IR Raw format.
 *
 * We round timings to the closest 10ms value,
 * and store up to last 26 values with seen.
 * A value already seen is encoded with a letter indicating the position in the table.
\*********************************************************************************************/

class IRRawTable {
public:
  IRRawTable() : timings() {}   // zero initialize the array

  int32_t getTimingForLetter(uint8_t l) const {
    l = toupper(l);
    if ((l < 'A') || (l > 'Z')) { return -1; }
    return timings[l - 'A'];
  }
  uint8_t findOrAdd(uint16_t t) {
    if (0 == t) { return 0; }

    for (uint32_t i=0; i<26; i++) {
      if (timings[i] == t) { return i + 'A'; }
      if (timings[i] == 0) { timings[i] = t; break; }   // add new value
    }
    return 0;   // not found
  }
  void add(uint16_t t) {
    if (0 == t) { return; }

    for (uint32_t i=0; i<26; i++) {
      if (timings[i] == 0) { timings[i] = t; break; }   // add new value
    }
  }

protected:
  uint16_t timings[26];
};

class mIRRemote :
  public mTaskerInterface
{

  private:
  public:
    mIRRemote(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_DRIVERS_IRREMOTE_CTR;
    static const char* PM_MODULE_DRIVERS_IRREMOTE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_IRREMOTE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_IRREMOTE_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_IRREMOTE_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mIRRemote);
    };
    #endif
    
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

// IRsend *irsend = nullptr;
    
// #ifdef USE_IR_RECEIVE

// IRrecv *irrecv = nullptr;

// #endif

unsigned long ir_lasttime = 0;


/*********************************************************************************************\
 * IR Remote send and receive using IRremoteESP8266 library
\*********************************************************************************************/

// #define XDRV_05             5


// Receiving IR while sending at the same time (i.e. receiving your own signal) was dsiabled in #10041
// At the demand of @pilaGit, you can `#define IR_RCV_WHILE_SENDING 1` to bring back this behavior
#ifndef IR_RCV_WHILE_SENDING
#define IR_RCV_WHILE_SENDING  0
#endif

enum IrErrors { IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_PROTO_UNSUPPORTED };

// const char kIrRemoteCommands[] PROGMEM = "|" D_CMND_IRSEND ;





// const uint8_t MAX_LEDS = 8;                 // Max number of leds

/**
 * @brief 
 * Definitely need to move to "mpin" since LED1-8 is a hard limit of 8, hence I can turn 1-8 as status type
 * Or else LED1 will always be "status" for combined mqtt/wifi connected
 * 
 * 
 * StatusLED
 * - Single blinks = wifi
 * - Double blinks = mqtt
 * - Solid LED = Power state
 * 
 */


void SubTask_Status_LEDs();
struct STATUS_LEDs{
  uint32_t tSaved_Update = millis();
  bool flag_enabled = false;

  // I need a way to enable much slow (than 1 second)
  uint16_t freq = 0; // flashes per 1 second


}status_leds;




#define MAX_LEDS 8

/**
 * @brief Bitpacked LEDs
 * 
 */
uint8_t leds_present = 0;                   // Max number of LED supported
uint8_t led_inverted = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t led_power = 0;                      // LED power state
uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))

  bool GetState(uint8_t index);
  void SetState(uint8_t index);


    struct LEDS
    {    
    
      int8_t pin = -1; // -1 is not active

    }leds[MAX_LEDS];

    void IrReceiveInit();


    void EveryLoop();

    void Pre_Init();
    void Init(void);
    
    void parse_JSONCommand(JsonParserObject obj);

    void UpdateStatusBlink();
    
    void UpdateLedPowerAll();
    void SetLedPowerIdx(uint32_t led, uint32_t state);
    void SetLedPower(uint32_t state);
    void SetLedPowerAll(uint32_t state);
    void SetLedLink(uint32_t state);


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mIRRemote> mqtthandler_settings_teleperiod;
    struct handler<mIRRemote> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;  

    struct handler<mIRRemote>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

    
};

#endif // USE_MODULE_DRIVERS_IRREMOTE

#endif