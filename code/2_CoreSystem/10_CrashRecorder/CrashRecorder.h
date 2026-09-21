
/**
 * Basic class that holds event type only
 * The event size will be changed to dynamic later, allowing more complex rules to be save as data
 * */
#ifndef _CRASH_RECORDER_H
#define _CRASH_RECORDER_H

#define D_UNIQUE_MODULE_CORE__CRASH_RECORDER__ID 2010 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE__CRASH_RECORDER

extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);


const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
const uint32_t crash_rtc_offset = 32;      // Offset in RTC memory skipping OTA used block
const uint32_t crash_dump_max_len = 31;    // Dump only 31 call addresses to satisfy max JSON length of about 600 characters

#ifdef ESP32
extern "C" void CrashRecorder__PanicHook(void);   // registered with IDF
#endif


// #ifdef ESP32
//   #include "esp_system.h"
//   #include "esp_debug_helpers.h"
//   #include "xtensa/xtensa_context.h"   // XtExcFrame
// #endif
#if defined(ARDUINO_ARCH_ESP32)
  #include "esp_system.h"
  #include "esp_debug_helpers.h"
  #include "esp_heap_caps.h"     // if you later add heap snapshot

  // Xtensa-only context frame:
  #if defined(CONFIG_IDF_TARGET_ARCH_XTENSA)
    #include "xtensa/xtensa_context.h"   // XtExcFrame
    #if defined(__has_include)
      #if __has_include("xtensa_api.h")
        #include "xtensa_api.h"
      #else
        #include "freertos/xtensa_api.h"
      #endif
    #else
      #include "freertos/xtensa_api.h"
    #endif
  #endif

  // RISC-V exception frame header name varies slightly by core/IDF.
  #if defined(CONFIG_IDF_TARGET_ARCH_RISCV)
    #if defined(__has_include)
      #if __has_include(<riscv/rvruntime-frames.h>)
        #include <riscv/rvruntime-frames.h>   // RvExcFrame
      #elif __has_include("riscv/rvruntime-frames.h")
        #include "riscv/rvruntime-frames.h"
      #endif
    #endif
  #endif
#endif




class CrashRecorder :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  CrashRecorder(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_CORE__CRASH_RECORDER__CTR = D_MODULE__CORE__CRASH_RECORDER__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE__CRASH_RECORDER__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE__CRASH_RECORDER__ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // How many devices are being managed, physical or virtual
      uint8_t mode = ModuleStatus::Initialising; // Disabled, Initialise, Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

  
    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
      #ifdef ESP32
      
        void CrashRTC_Clear(void);
        void CrashRTC_Print(void);
        void CrashRTC_PrintAndClear(void);

        // Creates: {"ESP32":{...,"CallChain":[...]}}
        bool Create_CrashDump(char* out, uint16_t out_len);

        // Writes /crash_report.json (full JSON document)
        bool CrashDump_WriteToFile(bool clear_rtc_after_write);

        // Called by panic wrappers (IRAM_ATTR safe)
        void IRAM_ATTR CaptureFromFrame(void* frame);

        // void CrashRTC_CaptureFromFrame(XtExcFrame* exc_frame);
        bool Create_CrashDumpJSON(char* out, uint16_t out_len) const;

        //         void CrashHook_Install();              // call once in Init()
        //         bool CrashRTC_HasRecord();             // true if RTC contains crash PCs
        //         void CrashRTC_Print();
        //         void CrashRTC_PrintAndClear();         // ALOG_INF print PCs then clear

        //         bool Create_CrashDump(char* out, uint16_t out_len);
        void Splash__CrashDump();

        //         void CrashRTC_Clear();
        // void CrashRTC_CaptureFromFrame(void* frame);


        void FS_EnsureCrashReportFile();
        //         bool CrashDump_WriteToFile(bool clear_rtc_after_write = true);
        bool FS_FileExists(const char* path) const;
        bool FS_WriteAll(const char* path, const char* data, size_t len) const;


      #endif

      #ifdef ESP8266
          void CrashHook_Install();
          bool CrashRTC_HasRecord();
          void CrashRTC_PrintAndClear();
      #endif

    void CmndCrash(void);
    void CmndWDT(void);
    void CmndBlockedLoop(void);
    void CrashDumpClear(void);
    
    void CrashDump(void);
    
  /************************************************************************************************
   * SECTION: ConstructJSON
   ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init();
    std::vector<struct telemetry_handler<CrashRecorder>*> telemetry_list;
    struct telemetry_handler<CrashRecorder> telemetry_settings;
    struct telemetry_handler<CrashRecorder> telemetry_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT
    
};


#endif // USE_MODULE_CORE__CRASH_RECORDER

#endif // HEADER