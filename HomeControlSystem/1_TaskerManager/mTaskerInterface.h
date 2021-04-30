
#pragma once

#include <stdint.h>

#include "5_Sensors/_Interface/mSensorType.h"


class mTaskerInterface
  {
  public:

    mTaskerInterface(){};
    virtual int8_t Tasker(uint8_t function, JsonParserObject obj);
    virtual PGM_P GetModuleName();
    virtual PGM_P GetModuleFriendlyName();
    virtual uint8_t GetModuleUniqueID();
    #ifdef USE_DEBUG_CLASS_SIZE
    virtual uint16_t GetClassSize();
    #endif
    // virtual float test123(){return 0;};// = 0;
    // virtual void test1234(sensors_event_t* x){}//return 0;};// = 0;
    
    /**
     * Push 0 if none are used, maybe internalise
     * */
    virtual void GetSensorReading(sensors_reading_t* value, uint8_t index = 0){ value->type_list.push_back(0); }
    
    
    // virtual void parse_JSONCommand(JsonParserObject obj);

};