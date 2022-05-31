
/**
 * Basic class that holds event type only
 * The event size will be changed to dynamic later, allowing more complex rules to be save as data
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

    #define D_RULE_DATA_VALUE_MAX_LENGTH 10

    struct EVENT_PART{
      /**
       * Buttons, Relays
       * */
      uint16_t module_id; //temporary rename until I can verify all module_ids use Unique and not ENUM codes
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
        // dynamic way of handling this in the future, ie the instance of this class creates the size?
        uint8_t data[D_RULE_DATA_VALUE_MAX_LENGTH]; // should this change to a vector? where would the vector save, and expand to?
        uint8_t length = 0;
      }value;
      /**
       * jsoncommands stored in a dlist
       * */
      uint8_t json_commands_dlist_id = 0; // 0 = none, 1+ is the count
    };

};


// #endif

#endif //ifdef USE_MODULE_CORE_RULES