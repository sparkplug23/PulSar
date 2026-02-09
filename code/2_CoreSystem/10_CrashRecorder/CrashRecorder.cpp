#include "CrashRecorder.h"

#ifdef USE_MODULE_CORE__CRASH_RECORDER

// ------------------------------------------------------------
// CrashRecorder_lowlevel.cpp  (or keep in CrashRecorder.cpp)
// Low-level crash capture + RTC retention.
// Layout:
//   #ifdef ESP8266 ... #endif
//   #ifdef ESP32
//     #if CONFIG_IDF_TARGET_ESP32 || ESP32S2 || ESP32S3   (Xtensa)
//     #elif CONFIG_IDF_TARGET_ESP32C2 || ESP32C3 || ESP32C6 (RISC-V)
//     #endif
//   #endif
//
// Build flags (ESP32 only):
//   -Wl,--wrap=panicHandler -Wl,--wrap=xt_unhandled_exception
//
// Date Modified: 02Feb26
// ------------------------------------------------------------

#include <stdint.h>

#if defined(ESP32)
  #include "esp_system.h"
  #include "esp_debug_helpers.h"
#endif

#ifndef CRASHREC_MAX_FRAMES
  #define CRASHREC_MAX_FRAMES 48
#endif

#ifndef CRASHREC_DEPTH
  #define CRASHREC_DEPTH 100
#endif

static constexpr uint32_t CRASHREC_MAGIC = 0x50554C00; // "PUL\0"

// RTC retained record (no initializer, survives reset while powered)
#if defined(ESP32)
RTC_NOINIT_ATTR volatile struct {
  uint32_t magic;
  uint32_t stack[CRASHREC_MAX_FRAMES];
  uint32_t pc;
  uint32_t exccause;
  uint32_t excvaddr;
} g_crashrec_rtc;
#endif

// ------------------------------------------------------------
// Common helpers (ESP32)
// ------------------------------------------------------------
#if defined(ESP32)
static inline void IRAM_ATTR CrashRTC_Clear_Unsafe()
{
  g_crashrec_rtc.magic = 0;
  g_crashrec_rtc.pc = 0;
  g_crashrec_rtc.exccause = 0;
  g_crashrec_rtc.excvaddr = 0;
  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) {
    g_crashrec_rtc.stack[i] = 0;
  }
}

static inline bool IRAM_ATTR CrashRTC_HasRecord()
{
  return (g_crashrec_rtc.magic == CRASHREC_MAGIC);
}
#endif


// ============================================================================
// ESP8266
// ============================================================================
#ifdef ESP8266

// NOTE: ESP8266 RTC user memory is different to ESP32 RTC_NOINIT_ATTR.
// This is a placeholder scaffold to keep your requested layout.
// Implement with ESP.rtcUserMemoryRead/Write + rst_info callback as you had before.

bool CrashRTC_HasRecord_8266()
{
  // TODO: implement (magic in RTC user memory)
  return false;
}

void CrashRTC_Clear_Unsafe_8266()
{
  // TODO: implement (clear RTC user memory region)
}

#endif  // ESP8266


// ============================================================================
// ESP32 (Xtensa: ESP32 / ESP32S2 / ESP32S3)
// ============================================================================
#ifdef ESP32

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3

#if defined(__has_include)
  #if __has_include("xtensa_api.h")
    #include "xtensa_api.h"
  #else
    #include "freertos/xtensa_api.h"
  #endif
#else
  #include "freertos/xtensa_api.h"
#endif

#include "xtensa/xtensa_context.h"   // XtExcFrame

// ------------------------------------------------------------------
// Reason strings (Xtensa). Copied from ESP-IDF panic.c (theirs is static)
// ------------------------------------------------------------------
static const char* const CRASHREC_EDESC[] = {
  "IllegalInstruction", "Syscall", "InstructionFetchError", "LoadStoreError",
  "Level1Interrupt", "Alloca", "IntegerDivideByZero", "PCValue",
  "Privileged", "LoadStoreAlignment", "res", "res",
  "InstrPDAddrError", "LoadStorePIFDataError", "InstrPIFAddrError", "LoadStorePIFAddrError",
  "InstTLBMiss", "InstTLBMultiHit", "InstFetchPrivilege", "res",
  "InstrFetchProhibited", "res", "res", "res",
  "LoadStoreTLBMiss", "LoadStoreTLBMultihit", "LoadStorePrivilege", "res",
  "LoadProhibited", "StoreProhibited", "res", "res",
  "Cp0Dis", "Cp1Dis", "Cp2Dis", "Cp3Dis",
  "Cp4Dis", "Cp5Dis", "Cp6Dis", "Cp7Dis"
};
static constexpr uint32_t CRASHREC_NUM_EDESC =
  (uint32_t)(sizeof(CRASHREC_EDESC) / sizeof(CRASHREC_EDESC[0]));

static inline const char* CrashRecorder__ExcReasonStr(uint32_t exccause)
{
  return (exccause < CRASHREC_NUM_EDESC) ? CRASHREC_EDESC[exccause] : "Unknown";
}

// ------------------------------------------------------------------
// Core capture (Xtensa)
// ------------------------------------------------------------------
// extern "C" void IRAM_ATTR CrashRTC_CaptureFromFrame(XtExcFrame* exc_frame)
// {
//   g_crashrec_rtc.magic    = CRASHREC_MAGIC;
//   g_crashrec_rtc.pc       = exc_frame ? exc_frame->pc       : 0;
//   g_crashrec_rtc.exccause = exc_frame ? exc_frame->exccause : 0;
//   g_crashrec_rtc.excvaddr = exc_frame ? exc_frame->excvaddr : 0;

//   for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) {
//     g_crashrec_rtc.stack[i] = 0;
//   }

//   if (!exc_frame) return;

//   uint32_t idx = 0;

//   esp_backtrace_frame_t stk_frame;
//   stk_frame.pc      = exc_frame->pc;
//   stk_frame.sp      = exc_frame->a1;
//   stk_frame.next_pc = exc_frame->a0;

//   const uint32_t pc0 = esp_cpu_process_stack_pc(stk_frame.pc);
//   g_crashrec_rtc.stack[idx++] = pc0;

//   bool corrupted = !(esp_stack_ptr_is_sane(stk_frame.sp) &&
//                      esp_ptr_executable((void*)pc0));

//   uint32_t i = (CRASHREC_DEPTH > 0) ? (CRASHREC_DEPTH - 1) : 0;
//   while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {

//     if (!esp_backtrace_get_next_frame(&stk_frame)) {
//       corrupted = true;
//       break;
//     }

//     if (idx >= CRASHREC_MAX_FRAMES) break;

//     const uint32_t pc = esp_cpu_process_stack_pc(stk_frame.pc);
//     g_crashrec_rtc.stack[idx++] = pc;
//   }
// }

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ARCH_XTENSA)

extern "C" void IRAM_ATTR CrashRTC_CaptureFromFrame(XtExcFrame* exc_frame)
{
  g_crashrec_rtc.magic    = CRASHREC_MAGIC;
  g_crashrec_rtc.pc       = exc_frame ? exc_frame->pc       : 0;
  g_crashrec_rtc.exccause = exc_frame ? exc_frame->exccause : 0;
  g_crashrec_rtc.excvaddr = exc_frame ? exc_frame->excvaddr : 0;

  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) g_crashrec_rtc.stack[i] = 0;
  if (!exc_frame) return;

  uint32_t idx = 0;

  esp_backtrace_frame_t stk_frame;
  stk_frame.pc      = exc_frame->pc;
  stk_frame.sp      = exc_frame->a1;
  stk_frame.next_pc = exc_frame->a0;

  const uint32_t pc0 = esp_cpu_process_stack_pc(stk_frame.pc);
  g_crashrec_rtc.stack[idx++] = pc0;

  bool corrupted = !(esp_stack_ptr_is_sane(stk_frame.sp) && esp_ptr_executable((void*)pc0));

  uint32_t i = (CRASHREC_DEPTH > 0) ? (CRASHREC_DEPTH - 1) : 0;
  while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {
    if (!esp_backtrace_get_next_frame(&stk_frame)) { corrupted = true; break; }
    if (idx >= CRASHREC_MAX_FRAMES) break;
    const uint32_t pc = esp_cpu_process_stack_pc(stk_frame.pc);
    g_crashrec_rtc.stack[idx++] = pc;
  }
}

#endif



// ------------------------------------------------------------------
// Linker-wrap hooks (Xtensa) — match Tasmota
// Build flags:
//   -Wl,--wrap=panicHandler -Wl,--wrap=xt_unhandled_exception
// ------------------------------------------------------------------
extern "C" {
  void __real_panicHandler(XtExcFrame *frame);
  void __real_xt_unhandled_exception(XtExcFrame *frame);

  void IRAM_ATTR __wrap_panicHandler(XtExcFrame *frame)
  {
    ets_printf("wrap_panicHandler\n");
    CrashRTC_CaptureFromFrame(frame);
    __real_panicHandler(frame);
  }

  void IRAM_ATTR __wrap_xt_unhandled_exception(XtExcFrame *frame)
  {
    ets_printf("wrap_xt_unhandled_exception\n");
    CrashRTC_CaptureFromFrame(frame);
    __real_xt_unhandled_exception(frame);
  }
} // extern "C"


// ============================================================================
// ESP32 (RISC-V: ESP32C2 / ESP32C3 / ESP32C6)
// ============================================================================
#elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6

#include <riscv/rvruntime-frames.h>  // RvExcFrame

// ------------------------------------------------------------------
// Reason strings (RISC-V). Mirrors panic_arch.c style strings.
// (Exact wording can differ by IDF version; this is the common set.)
// ------------------------------------------------------------------
static const char* const CRASHREC_RV_REASONS[] = {
  "Instruction address misaligned",
  "Instruction access fault",
  "Illegal instruction",
  "Breakpoint",
  "Load address misaligned",
  "Load access fault",
  "Store address misaligned",
  "Store access fault",
  "Environment call from U-mode",
  "Environment call from S-mode",
  "",
  "Environment call from M-mode",
  "Instruction page fault",
  "Load page fault",
  "",
  "Store page fault",
};
static constexpr uint32_t CRASHREC_NUM_RV_REASONS =
  (uint32_t)(sizeof(CRASHREC_RV_REASONS) / sizeof(CRASHREC_RV_REASONS[0]));

static inline const char* CrashRecorder__ExcReasonStr(uint32_t exccause)
{
  return (exccause < CRASHREC_NUM_RV_REASONS) ? CRASHREC_RV_REASONS[exccause] : "Unknown";
}

// // ------------------------------------------------------------------
// // Core capture (RISC-V)
// // NOTE: there is no esp_backtrace_get_next_frame() path like Xtensa;
// // we do a lightweight stack scan like Tasmota.
// // ------------------------------------------------------------------
// extern "C" void IRAM_ATTR CrashRTC_CaptureFromFrame(void* exc_frame)
// {
//   const RvExcFrame* regs = (const RvExcFrame*)exc_frame;

//   g_crashrec_rtc.magic    = CRASHREC_MAGIC;
//   g_crashrec_rtc.pc       = regs ? (uint32_t)regs->mepc   : 0;
//   g_crashrec_rtc.exccause = regs ? (uint32_t)regs->mcause : 0;
//   g_crashrec_rtc.excvaddr = regs ? (uint32_t)regs->mtval  : 0;

//   for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) {
//     g_crashrec_rtc.stack[i] = 0;
//   }

//   if (!regs) return;

//   uint32_t idx = 0;

//   // First frame: return address (RA) is the most useful seed.
//   g_crashrec_rtc.stack[idx++] = (uint32_t)regs->ra;

//   // Lightweight scan of stack memory for code addresses.
//   // Code region filter used by Tasmota for C3:
//   //   0x40000000 .. 0x42800000 (keeps only executable-ish addresses)
//   const uint32_t* sp = (const uint32_t*)regs->sp;
//   for (uint32_t n = 0; n < 320 && idx < CRASHREC_MAX_FRAMES; ++n, ++sp) {
//     const uint32_t v = *sp;
//     if ((v >= 0x40000000U) && (v < 0x42800000U)) {
//       g_crashrec_rtc.stack[idx++] = v;
//     }
//   }
// }

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ARCH_RISCV)

static inline bool IRAM_ATTR CrashRec_IsPlausiblePC(uint32_t v)
{
  // Conservative “code-ish” ranges for ESP32-C3/C6 flash/iram mappings.
  // Keeps noise down and avoids storing random stack values.
  return (v >= 0x40000000U && v < 0x44000000U);
}

extern "C" void IRAM_ATTR CrashRTC_CaptureFromFrame(void* exc_frame)
{
  g_crashrec_rtc.magic    = CRASHREC_MAGIC;
  g_crashrec_rtc.pc       = 0;
  g_crashrec_rtc.exccause = 0;
  g_crashrec_rtc.excvaddr = 0;
  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) g_crashrec_rtc.stack[i] = 0;

  if (!exc_frame) return;

  // If RvExcFrame is available, use it; otherwise we can’t safely decode regs.
  #if defined(RISCV_RVRUNTIME_FRAMES_H) || defined(_RVRUNTIME_FRAMES_H_) || defined(RISCV_RVRUNTIME_FRAMES)
    const RvExcFrame* regs = (const RvExcFrame*)exc_frame;
    g_crashrec_rtc.pc       = (uint32_t)regs->mepc;
    g_crashrec_rtc.exccause = (uint32_t)regs->mcause;
    g_crashrec_rtc.excvaddr = (uint32_t)regs->mtval;

    uint32_t idx = 0;

    // Seed with mepc and ra if plausible.
    if (CrashRec_IsPlausiblePC((uint32_t)regs->mepc) && idx < CRASHREC_MAX_FRAMES) g_crashrec_rtc.stack[idx++] = (uint32_t)regs->mepc;
    if (CrashRec_IsPlausiblePC((uint32_t)regs->ra)   && idx < CRASHREC_MAX_FRAMES) g_crashrec_rtc.stack[idx++] = (uint32_t)regs->ra;

    // Bounded scan of stack for plausible return addresses.
    const uint32_t* sp = (const uint32_t*)regs->sp;
    for (uint32_t n = 0; n < 256 && idx < CRASHREC_MAX_FRAMES; ++n) {
      uint32_t v = sp[n];
      if (CrashRec_IsPlausiblePC(v)) g_crashrec_rtc.stack[idx++] = v;
    }
  #else
    // No known frame layout; record “something happened” only.
    g_crashrec_rtc.pc = 0;
  #endif
}

#endif


// ------------------------------------------------------------------
// Linker-wrap hooks (RISC-V)
// Build flags:
//   -Wl,--wrap=panicHandler -Wl,--wrap=xt_unhandled_exception
// Signatures are void* on these targets.
// ------------------------------------------------------------------
// extern "C" {
//   void __real_panicHandler(void *frame);
//   void __real_xt_unhandled_exception(void *frame);

//   void IRAM_ATTR __wrap_panicHandler(void *frame)
//   {
//     ets_printf("wrap_panicHandler\n");
//     CrashRTC_CaptureFromFrame(frame);
//     __real_panicHandler(frame);
//   }

//   void IRAM_ATTR __wrap_xt_unhandled_exception(void *frame)
//   {
//     ets_printf("wrap_xt_unhandled_exception\n");
//     CrashRTC_CaptureFromFrame(frame);
//     __real_xt_unhandled_exception(frame);
//   }
// } // extern "C"

// #if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ARCH_RISCV)

extern "C" {
  void __real_panicHandler(void *frame);

  void IRAM_ATTR __wrap_panicHandler(void *frame)
  {
    CrashRTC_CaptureFromFrame(frame);
    __real_panicHandler(frame);
  }
}

// #endif



#else
  // Unknown ESP32 target
#endif  // target split

#endif  // ESP32



bool CrashRecorder::Create_CrashDumpJSON(char* out, uint16_t out_len) const
{
#if defined(ESP32)
  if (!out || out_len == 0) return false;
  out[0] = '\0';

  if (!CrashRTC_HasRecord()) return false;

  const uint32_t exccause = (uint32_t)g_crashrec_rtc.exccause;

  const char* reason = "Unknown";
  reason = CrashRecorder__ExcReasonStr(exccause);

  uint16_t used = 0;

  int n = snprintf(
    out + used, out_len - used,
    "{\"ESP32\":{\"Exception\":%u,\"Reason\":\"%s\",\"EPC\":\"0x%08x\",\"EXCVADDR\":\"0x%08x\",\"CallChain\":[",
    (unsigned)exccause,
    reason,
    (unsigned)g_crashrec_rtc.pc,
    (unsigned)g_crashrec_rtc.excvaddr
  );
  if (n < 0 || (uint16_t)n >= (out_len - used)) return false;
  used += (uint16_t)n;

  bool first = true;
  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) {
    const uint32_t pc = g_crashrec_rtc.stack[i];
    if (!pc) break;

    n = snprintf(out + used, out_len - used,
                 "%s\"0x%08x\"",
                 first ? "" : ",",
                 (unsigned)pc);
    if (n < 0 || (uint16_t)n >= (out_len - used)) return false;
    used += (uint16_t)n;
    first = false;
  }

  n = snprintf(out + used, out_len - used, "]}}\n");
  if (n < 0 || (uint16_t)n >= (out_len - used)) return false;

  return true;
#else
  (void)out; (void)out_len;
  return false;
#endif
}


void CrashRecorder::CrashRTC_Clear()
{
  g_crashrec_rtc.magic = 0;
  g_crashrec_rtc.pc = 0;
  g_crashrec_rtc.exccause = 0;
  g_crashrec_rtc.excvaddr = 0;
  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) {
    g_crashrec_rtc.stack[i] = 0;
  }
}

void CrashRecorder::CrashRTC_Print()
{
  if (!CrashRTC_HasRecord()) {
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: no record"));
    return;
  }

  ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: exccause=%u pc=0x%08x excvaddr=0x%08x"),
           (unsigned)g_crashrec_rtc.exccause,
           (unsigned)g_crashrec_rtc.pc,
           (unsigned)g_crashrec_rtc.excvaddr);

  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) {
    uint32_t pc = g_crashrec_rtc.stack[i];
    if (!pc) break;
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: PC[%u]=0x%08x"), (unsigned)i, (unsigned)pc);
  }
}

void CrashRecorder::CrashRTC_PrintAndClear()
{
  CrashRTC_Print();
  CrashRTC_Clear();
  ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: cleared"));
}


// -----------------------------
// Filesystem helpers (adapt FILE_SYSTEM to your FS instance)
// -----------------------------

bool CrashRecorder::FS_FileExists(const char* path) const
{
  if (!path) return false;
  File f = FILE_SYSTEM.open(path, "r");
  if (!f) return false;
  f.close();
  return true;
}

bool CrashRecorder::FS_WriteAll(const char* path, const char* data, size_t len) const
{
  if (!path || !data) return false;

  File f = FILE_SYSTEM.open(path, "w");
  if (!f) return false;

  size_t written = f.write((const uint8_t*)data, len);
  f.close();
  return (written == len);
}

void CrashRecorder::FS_EnsureCrashReportFile()
{
  const char* path = "/crash_report.json";

  if (FS_FileExists(path)) {
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashReport: exists %s"), path);
    return;
  }

  const char* placeholder = "{\"ESP32\":{}}\n";
  bool ok = FS_WriteAll(path, placeholder, strlen(placeholder));

  ALOG_INF(PSTR(D_LOG_DEBUG "CrashReport: create placeholder %s %s"),
           path, ok ? "OK" : "FAIL");
}

bool CrashRecorder::CrashDump_WriteToFile(bool clear_rtc_after_write)
{
  const char* path = "/crash_report.json";

  char json[900];
  if (!Create_CrashDumpJSON(json, sizeof(json))) {
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: no RTC record"));
    return false;
  }

  bool ok = FS_WriteAll(path, json, strlen(json));
  ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: write %s %s"), path, ok ? "OK" : "FAIL");

  if (ok && clear_rtc_after_write) {
    CrashRTC_Clear();
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: cleared after write"));
  }

  return ok;
}





int8_t CrashRecorder::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
    break;
    case TASK_INIT:
    
    break;
  }

  // if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
    //   EverySecond();
    break;
    case TASK_ON_BOOT_SUCCESSFUL:    
    #ifdef ESP32
      FS_EnsureCrashReportFile();
      CrashDump_WriteToFile(false); //keeping crash reports for now as we debug
    #endif
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      // tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }
  return 1;


} // END function


__attribute__((noinline)) void CrashRecorder::CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((volatile uint32_t*)0x00000000);
}


// Do an infinite loop to trigger WDT watchdog
void CrashRecorder::CmndWDT(void)
{
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
}

// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
void CrashRecorder::CmndBlockedLoop(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  while (1) {
    delay(1000);
  }
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashRecorder::CrashDumpClear(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  return;
    #ifdef ESP8266
  uint32_t value = 0;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}


void CrashRecorder::Splash__CrashDump()
{
  // Size: pick one that always fits your JSON worst-case (48 frames + reason strings).
  // If you already validated 900 elsewhere, use the same here for consistency.
  char buffer[900];

  if (!Create_CrashDumpJSON(buffer, sizeof(buffer))) {
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: none"));
    return;
  }

  ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump %s"), buffer);
}


/******************************************************************************************************************
 * @SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t CrashRecorder::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  return JBI->End();

}


uint8_t CrashRecorder::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  
  JBI->Start();
  
  JBI->Object_Start(PM_CRASHDUMP);
    char buffer[900];
    Create_CrashDumpJSON(buffer, sizeof(buffer));
    JBI->Add(buffer);
  JBI->Object_End();

  return JBI->End();

}


/******************************************************************************************************************
 * @SECTION: Commands
*******************************************************************************************************************/


void CrashRecorder::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  JsonParserObject jobj = 0; 
  
  if(!(jobj = obj[GetModuleName()].getObject()))
  {
    ALOG_DBM(PSTR(D_LOG_RADAR_LD2410 "No Command"));
    return;
  }

  if(jtok = jobj["Print"]){
    ALOG_INF(PSTR("CrashPrint command received"));
    CrashRTC_Print();
  }

  if(jtok = jobj["PrintAndClear"]){
    ALOG_INF(PSTR("CrashPrintAndClear command received"));
    CrashRTC_PrintAndClear();
  }

  if (jtok = jobj["CrashDump"]) {
    ALOG_INF(PSTR("CrashDump command received"));
    Splash__CrashDump();
  }

  if (jtok = jobj["CreateCrashDumpJSON"]) {
    ALOG_INF(PSTR("CreateCrashDumpJSON command received"));
    // optional: print the JSON fragment to log for visibility
    Splash__CrashDump();
    // write crash_report.json (clears RTC after successful write)
    CrashDump_WriteToFile(false);
  }

  if(jtok = jobj["ForceCrash"])
  {
    switch(jtok.getInt())
    {
      case 1:
        ALOG_INF(PSTR("CrashForce command received"));
        CmndCrash();    // force a crash
      break;
      case 2:
        ALOG_INF(PSTR("WDTForce command received"));
        CmndWDT();
      break;
      case 3:
        ALOG_INF(PSTR("BlockedLoopForce command received"));
        CmndBlockedLoop();
      break;
    }
    ALOG_INF(PSTR("ForceCrash command received"));
  }
  
}


/******************************************************************************************************************
 * @SECTION: MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void CrashRecorder::MQTTHandler_Init(){

  struct handler<CrashRecorder>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &CrashRecorder::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = 600;
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &CrashRecorder::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CORE__CRASH_RECORDER