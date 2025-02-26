
/**
 * Basic class that holds event type only
 * The event size will be changed to dynamic later, allowing more complex rules to be save as data
 * */
#ifndef _MEVENT_H
#define _MEVENT_H

#define D_UNIQUE_MODULE_CORE_EVENTS_ID 2012 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#include <stdint.h>

#include "2_CoreSystem/12_Events/EventPackage.h"


class mEvent :
  public mTaskerInterface
{

  public:
  
    mEvent(){};
    
    static constexpr const char* PM_MODULE_CORE_EVENTS_CTR = D_MODULE_CORE_EVENTS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_EVENTS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_EVENTS_ID; }

    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0){}


    std::vector<EventPackage> events;

    
    struct XDRVMAILBOX {
      bool          grpflg;
      bool          usridx;
      uint16_t      command_code;
      uint32_t      index;
      uint32_t      data_len;
      int32_t       payload;
      char         *topic;
      char         *data;
      char         *command;
    } XdrvMailbox;

};


// #endif

#endif //ifdef USE_MODULE_CORE_RULES