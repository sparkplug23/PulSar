
/**
 * Basic class that holds event type only
 * */


#ifndef _MEVENT_H
#define _MEVENT_H

// #define D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID 6

// #include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_CORE_RULES

#include <stdint.h>
// #include "3_Network/MQTT/mMQTT.h"
// #include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

// #include "1_TaskerManager/mTaskerInterface.h"

class mEvent
{
    
  private:
    /* Prevent others from being created */
    // RuleEngine(RuleEngine const& other) = delete;
    // RuleEngine(RuleEngine&& other) = delete;
    /* Private constructor to prevent instancing. */
    // RuleEngine(){};
    
  public:
  
    mEvent(){};

    /***
     * 
     * Future change:
     * Events should become their own class, to hold things that happen.
     * Rules can then only be turned on when needed in the primary loop, equally, I should add a check to only go through rules if any are active with a flag
     * 
     * 
     * */
    struct EVENT_PART{
      /**
       * Buttons, Relays
       * */
      uint16_t module_id;
      /**
       * InputChange, Timer
       */
      uint16_t function_id;
      /**
       * ?? or buffer[0]
       * */
      uint8_t device_id;
      /**
       * Values
       * */
      struct VALUES{
        uint8_t encoding = 0;
        uint8_t data[5];
        uint8_t length = 0;
      }value;
      /**
       * jsoncommands stored in a dlist
       * */
      int8_t json_commands_dlist_id = -1;
    };

};


// #endif

#endif //ifdef USE_MODULE_CORE_RULES