#pragma once


#include <stdint.h>


/***
 * 
 * Future change:
 * Events should become their own class, to hold things that happen.
 * Rules can then only be turned on when needed in the primary loop, equally, I should add a check to only go through rules if any are active with a flag
 * 
 * 
 * */

#define RULE_ENCODED_DATA_MAX_BYTES 10

struct EventPackage{
    bool isvalid = true;// for now force true, later needs set/unset when new events happen and cleared when handled
    /**
     * Buttons, Relays
     * */
    uint16_t module_id = 0; //temporary rename until I can verify all module_ids use Unique and not ENUM codes
    /**
     * InputChange, Timer
     */
    uint16_t function_id = 0;
    /**
     * ?? or buffer[0]
     * */
    uint8_t device_id = 0;
    /**
     * Values
     * */
    struct VALUES{
    uint8_t encoding = 0;
    // dynamic way of handling this in the future, ie the instance of this class creates the size?
    uint8_t data[RULE_ENCODED_DATA_MAX_BYTES]; // should this change to a vector? where would the vector save, and expand to?
    uint8_t length = 0;
    }value;
    /**
     * JSON command stored in mRuleEngine::jsonbuffer.data dlist.
     *
     * 0    = no JSON command
     * 1..N = valid JSON command slot
     *
     * The stored slot is 1-based so memset(EventPackage, 0, ...)
     * safely means "no JSON command".
     *
     * Convert to zero-based dlist index only when reading:
     *   dlist_index = json_command_slot - 1;
     */
    uint8_t json_command_slot = 0;
};