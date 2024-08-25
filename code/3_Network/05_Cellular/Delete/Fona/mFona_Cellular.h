/*
  mFona_Cellular - GPRS Sim Card Support

  Copyright (C) 2022  Michael Doone

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

#ifndef HEADER_MODULE_DRIVERS_FONA_CELLULAR_H
#define HEADER_MODULE_DRIVERS_FONA_CELLULAR_H
  
#define D_UNIQUE_MODULE_DRIVERS_FONA_CELLULAR_ID ((4*1000)+16) 

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_FONA_CELLULAR

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


/***************************************************
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
THIS CODE IS STILL IN PROGRESS!

Open up the serial console on the Arduino at 115200 baud to interact with FONA

Note that if you need to set a GPRS APN, username, and password scroll down to
the commented section below at the end of the setup() function.
*/
#include "Adafruit_FONA.h"

/**
 * @brief 
 * 
 * [Bat]
 * [GND]
 * - these are the same pins as the 2-pin JST connector. Connect to a
3.7V/4.2V Lipoly/LiIon battery. Do not power from a power supply or LDO. Use
only a battery, at least 500mA, 1000mAh+ is best since there can  be 2A spikes
of current
 * [SPKR+]
 * [SPKR-]
 *  - Direct Connections: No GPIO interface
    - Speaker: This is for connecting an external 8 ohm speaker, max 1W. You can
            configure the module to route calls and FM radio to the headset or the external
            audio. The two pins are differential so they don't have output DC blocking
            capacitors. You cannot connect this to a stereo, powered speakers or other non-differential amplifier without adding a 100uF+ blocking cap in series to the + pin
            and then not using the - pin. Instead, your amp should use GND for the -
            reference
 * [Rst]
 *  - GPIO_FUNCTION__FONA_RESET__ID
 *  - Reset: This is module hard reset pin. By default it has a high pull-up (module not
 *           in reset). If you absolutely got the module in a bad space, toggle this pin low for
 *           100ms to perform a hard reset.
 * [PS]
 *   - GPIO_FUNCTION__FONA_POWER_STATUS__ID
 * [Key]
 *   - GPIO_FUNCTION__FONA_POWER_KEY__ID
 * [RI]
 *  - GPIO_FUNCTION__FONA_RING_INDICATOR__ID
 *  - Ring Indicator: This is the Ring Indicator. It is basically the 'interrupt' out pin from the module. It is by default high and will pulse low for 120ms 
 *                    when a call is received. It can also be configured to pulse when an SMS is received.
 * [TX]
 * [RX]
 *  - GPIO_FUNCTION__FONA_UART_TX__ID
 *  - GPIO_FUNCTION__FONA_UART_RX__ID,
 *  - UART: The module uses UART to send and receive commands and data. These pins are auto-baud so whatever baud rate you send "AT" after reset or boot is
 *          the baud rate is used. RX is into the module, TX is out of the module.
 * [NS]
 *  - GPIO_FUNCTION__FONA_NETWORK_STATUS__ID
 *  - This is the Network Status pin. It pulses to signal the current status of the
module. This is also tied to the Net LED so for more detail see the LEDs section
below
 * [Vio]
   - Vio: io - THIS IS THE MOST IMPORTANT PIN! This is the pin that you MUST drive
with an external voltage from 3V-5V to set the logic level converter. The
converter also buffers the indicator LEDs so NOTHING will appear to work
unless this pin is powered! You should set the voltage to whatever voltage your
microcontroller uses for logic. A 5V micro (like Arduino) should have it be 5V, a
3V logic micro should set it to 3V.
 * [Mic+]
 * [Mic-]
 * this is for connecting an external electret microphone, it will bias
the mic with 2V. Most electrets will work just fine. No extra circuitry is required
for the mic such as a biaser or amplifier, just wire it up directly!
 * [5v]
 * [ADC]
 * [PWM]
 * 
 * 
 */


class mFona_Cellular :
  public mTaskerInterface
{

  private:
  public:
    mFona_Cellular(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_DRIVERS_FONA_CELLULAR_CTR = D_MODULE_DRIVERS_FONA_CELLULAR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_FONA_CELLULAR_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_FONA_CELLULAR_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mFona_Cellular);
    };
    #endif
    
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;



#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

//#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
//// For UNO and others without hardware serial,
//// we default to using software serial. If you want to use hardware serial
//// (because softserial isnt supported) comment out the following three lines 
//// and uncomment the HardwareSerial line
//#include <SoftwareSerial.h>
//
//#define FONA_RX 2
//#define FONA_TX 3
//
//SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
//SoftwareSerial *fonaSerial = &fonaSS;
//
//#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
HardwareSerial *fonaSerial = &Serial2;


// // Use this for FONA 800 and 808s
// Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
// // Use this one for FONA 3G
// //Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);
//#endif
// Use this for FONA 800 and 808s
Adafruit_FONA* fona = nullptr;//Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

enum stanby_status_enum_ids{
  STANDBY_STATUS__CHECK_POWER__ID,


};


struct CONNECTION_DETAILS{

int16_t rssi_dBm = -1;
bool flag_ring_indicator = false;

uint8_t standy_mode_id = 0;

}connection;


    void printMenu(void);
    int8_t CommandGet_RSSI_dBm();
    
    bool CommandSet_Power();

    
    void ISR_Ring_Indicator_Triggered();

    void Command(char command);


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
    void MQTTHandler_Sender();
    
    struct handler<mFona_Cellular> mqtthandler_settings_teleperiod;
    struct handler<mFona_Cellular> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
      

    struct handler<mFona_Cellular>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

    
};

#endif // USE_MODULE_DRIVERS_FONA_CELLULAR

#endif