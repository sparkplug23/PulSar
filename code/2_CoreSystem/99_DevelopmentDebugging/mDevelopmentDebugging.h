#ifndef DEVELOPMENT_DEBUGGING_H
#define DEVELOPMENT_DEBUGGING_H 0.21
  
#include <Arduino.h>

#define D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID 2099 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"

class mDevelopmentDebugging :
  public mTaskerInterface
{

  private:
  public:
    mDevelopmentDebugging(){};
    void init(void);

    struct DEBUG_DATA
    {
      float input_float1 = 0;
    }debug_data;
    void parse_JSONCommand(JsonParserObject obj);

    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR = D_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID; }
    
    void SubTask_Show_Defines_Ready_To_Phase_Out();

};
#endif
