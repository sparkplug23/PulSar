
#pragma once

#include <stdint.h>

class mTaskerInterface
  {
  public:

    mTaskerInterface(){};
    virtual int8_t Tasker(uint8_t);
    virtual PGM_P GetModuleName();
    virtual PGM_P GetModuleFriendlyName();

    // Does this create a new instance? I would want the previous instance back
    // mTaskerInterface* create(String name){
    //     if(name == D_MODULE_SENSORS_MOTION_FRIENDLY_CTR)
    //     {
    //         return new mMotionSensor();
    //     }
    // }

};