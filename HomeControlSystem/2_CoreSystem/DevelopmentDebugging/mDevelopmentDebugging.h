#ifndef DEVELOPMENT_DEBUGGING_H
#define DEVELOPMENT_DEBUGGING_H 0.21
  
#include <Arduino.h>

#define D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID 15

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"

class mDevelopmentDebugging :
  public mTaskerInterface
{

  private:
  public:
    mDevelopmentDebugging(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR;
    static const char* PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDevelopmentDebugging);
    };
    #endif

    void SubTask_Show_Defines_Ready_To_Phase_Out();

};
#endif
