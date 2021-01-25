#ifndef _MDOORSENSOR1cf2_H
#define _MDOORSENSOR1cf2_H

// Pin number ???
//#define IR_SEND_TIMER5	46

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER

#include <Arduino.h>

#include "1_TaskerManager/mTaskerManager.h"



#ifdef ESP32
   #include "IRLib32/IRremote.h"
#endif
#ifdef ESP8266
   #include "4_Drivers/IRDevices/IRLib8266/IRremoteESP8266.h"
   #include "4_Drivers/IRDevices/IRLib8266/IRsend.h"
#endif

class mIRtransceiver {//}: public IRsendRaw {

  private:

  public:
    mIRtransceiver(){};
    void init(void);
    void pre_init(void);
    
    int8_t pin = -1;

    #define ONE_BIT_TIME 1252 //+- 30 std
    #define ZERO_BIT_TIME 439 //+- 30 std

    void TransmitCode(uint32_t code, uint8_t code_bits, uint8_t repeat_count, uint8_t pin = 255); // to be moved into here
    void parse_JSONCommand(char* topic, char* payload,unsigned int length);

    int8_t Tasker(uint8_t func);


};

#endif

#endif