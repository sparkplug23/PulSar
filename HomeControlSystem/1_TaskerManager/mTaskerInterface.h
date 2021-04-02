
#pragma once

#include <stdint.h>

class mTaskerInterface
  {
  public:

    mTaskerInterface(){};
    virtual int8_t Tasker(uint8_t);
    virtual PGM_P GetModuleName();
    virtual PGM_P GetModuleFriendlyName();
    virtual uint8_t GetModuleUniqueID();
    #ifdef USE_DEBUG_CLASS_SIZE
    virtual uint16_t GetClassSize();
    #endif

};