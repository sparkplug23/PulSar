#ifndef _USE_MODULE_CORE_JSON_ARDUINO_H
#define _USE_MODULE_CORE_JSON_ARDUINO_H

#define D_UNIQUE_MODULE_CORE__JSON_ARDUINO__ID 2021 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE__JSON_ARDUINO

#include "1_TaskerManager/mTaskerInterface.h"

#define ARDUINOJSON_DECODE_UNICODE 0
#include "2_CoreSystem/21_JsonArduino/AsyncJson-v6.h"
#include "2_CoreSystem/21_JsonArduino/ArduinoJson-v6.h"

#include <Arduino.h>


#if defined(ARDUINO_ARCH_ESP32)
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (1 && psramFound()) return ps_malloc(size);
    else                   return malloc(size);
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (1 && psramFound()) return ps_realloc(ptr, new_size);
    else                   return realloc(ptr, new_size);
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif


class mJsonArduino 
{


  private:
    /* Prevent others from being created */
    mJsonArduino(mJsonArduino const& other) = delete;
    mJsonArduino(mJsonArduino&& other) = delete;
    /* Private constructor to prevent instancing. */
    mJsonArduino(){};
    


  public:
    // External function to get instance
    static mJsonArduino* GetInstance();
    /* Here will be the instance stored. */
    static mJsonArduino* instance;
  
    void Init(void);
        
    
    #if defined(ARDUINO_ARCH_ESP32)
    JsonDocument *pDoc = nullptr;
    SemaphoreHandle_t jsonBufferLockMutex = xSemaphoreCreateRecursiveMutex();
    #else
    StaticJsonDocument<JSON_BUFFER_SIZE> gDoc;
    JsonDocument *pDoc = &gDoc;
    #endif
    
    // global ArduinoJson buffer
    volatile uint16_t jsonBufferLock = 0;
    bool requestJSONBufferLock(uint16_t module);
    void releaseJSONBufferLock(); // duplicate of below, need to JOIN THEM!!

    bool psramSafe = true;


};


#define mJsonArduinoI mJsonArduino::GetInstance()
#define tkr_jsona mJsonArduinoI



#endif


#endif