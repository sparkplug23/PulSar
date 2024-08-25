
#pragma once

#include <stdint.h>

#include "2_CoreSystem/06_Support/mSensorType.h"

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
    virtual int8_t Tasker(uint8_t function, JsonParserObject obj);
    virtual PGM_P GetModuleName();
    virtual uint16_t GetModuleUniqueID();
    
    #ifdef USE_DEBUG_CLASS_SIZE
    virtual uint16_t GetClassSize();
    #endif    

    /**
     * Get device count for each module that should be in module.settings
     * */
    virtual uint8_t GetSensorCount(void) {return 0;}
    
    /**
     * Push 0 if none are used, maybe internalise
     * Rename to "GetDeviceReading"
     * */
    virtual void GetSensorReading(sensors_reading_t* value, uint8_t index = 0){ value->sensor_type.push_back(0); }


    
    // mTaskerInterface() = default;
    // virtual ~mTaskerInterface() = default;


    // virtual int8_t Tasker(uint8_t function, JsonParserObject obj) = 0;
    // virtual PGM_P GetModuleName() const = 0;
    // virtual uint16_t GetModuleUniqueID() const = 0;
    
    // #ifdef USE_DEBUG_CLASS_SIZE
    // virtual uint16_t GetClassSize() const = 0;
    // #endif    

    // /**
    //  * Get device count for each module that should be in module.settings
    //  * */
    // virtual uint8_t GetSensorCount() { return 0; } // If I want to force it, but not all modules need this
    // // virtual uint8_t GetSensorCount() const { return 0; }
    
    // /**
    //  * Push 0 if none are used, maybe internalise
    //  * Rename to "GetDeviceReading"
    //  * */
    // // virtual void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) const { value->sensor_type.push_back(0); } // If I want to force it, but not all modules need this
    // virtual void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) { value->sensor_type.push_back(0); }
  
};