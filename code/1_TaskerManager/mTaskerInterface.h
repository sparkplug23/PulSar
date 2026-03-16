
#pragma once

#include <stdint.h>

#include "2_CoreSystem/06_Support/mSensorType.h"
#include "JsonParser.h"

/**
 * @brief 
 * Superclass that all modules includes
 * Polymorphic Interface (mTaskerInterface)

    Current Approach: The mTaskerInterface is a base class that other modules inherit from. This allows the mTaskerManager to hold a collection of mTaskerInterface pointers and interact with each module through a common interface.
 **/
class mTaskerInterface
  {
  public:

    mTaskerInterface(){};
    virtual int8_t Tasker(uint8_t function, JsonParserObject obj) { return 0; }
    virtual int8_t Tasker_DevCode(uint8_t function, JsonParserObject obj) { return 0; } // optional per module place to test development code. It will be in its own file "DevCode.cpp"
    virtual PGM_P GetModuleName() { return PSTR("BaseTasker"); }
    virtual uint16_t GetModuleUniqueID() { return 0; }
    virtual ~mTaskerInterface(){};
    
    /**
     * Get device count for each module that should be in module.settings
     * */
    virtual uint8_t GetSensorCount(void) {return 0;}
    
    /**
     * Push 0 if none are used, maybe internalise
     * Rename to "GetDeviceReading"
     * 
        sensors_reading_t val;
        Must use a new one for each, or it will append them on the end of a previous one

     * */
    virtual void GetSensorReading(sensors_reading_t* value, uint8_t index = 0){ value->sensor_type.push_back(0); value->timestamp = 1; }
  
};