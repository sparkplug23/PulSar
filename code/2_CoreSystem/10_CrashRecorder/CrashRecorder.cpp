#include "CrashRecorder.h"

#ifdef USE_MODULE_CORE__CRASH_RECORDER


#ifdef ESP32

#include <stdint.h>

#include "esp_system.h"
#include "esp_debug_helpers.h"

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

#ifndef CRASHREC_MAX_FRAMES
  #define CRASHREC_MAX_FRAMES 48
#endif

static constexpr uint32_t CRASHREC_MAGIC = 0x50554C00; // "PUL\0"
static constexpr uint32_t CRASHREC_DEPTH = 100;

// RTC retained record (no initializer, survives reset while powered)
RTC_NOINIT_ATTR volatile struct {
  uint32_t magic;
  uint32_t stack[CRASHREC_MAX_FRAMES];
  uint32_t pc;
  uint32_t exccause;
  uint32_t excvaddr;
} g_crashrec_rtc;


// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

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


// ------------------------------------------------------------
// Core capture
// ------------------------------------------------------------

extern "C" void IRAM_ATTR CrashRTC_CaptureFromFrame(XtExcFrame* exc_frame)
{
  g_crashrec_rtc.magic    = CRASHREC_MAGIC;
  g_crashrec_rtc.pc       = exc_frame ? exc_frame->pc       : 0;
  g_crashrec_rtc.exccause = exc_frame ? exc_frame->exccause : 0;
  g_crashrec_rtc.excvaddr = exc_frame ? exc_frame->excvaddr : 0;

  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) {
    g_crashrec_rtc.stack[i] = 0;
  }

  if (!exc_frame) return;

  uint32_t idx = 0;

  // Seed first backtrace frame (matches IDF logic)
  esp_backtrace_frame_t stk_frame;
  stk_frame.pc      = exc_frame->pc;
  stk_frame.sp      = exc_frame->a1;
  stk_frame.next_pc = exc_frame->a0;

  // Store first PC (processed)
  uint32_t pc0 = esp_cpu_process_stack_pc(stk_frame.pc);
  g_crashrec_rtc.stack[idx++] = pc0;

  // Validate first frame: SP sane and PC executable
  bool corrupted = !(esp_stack_ptr_is_sane(stk_frame.sp) &&
                     esp_ptr_executable((void*)pc0));

  // Walk
  uint32_t i = CRASHREC_DEPTH - 1;
  while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {

    if (!esp_backtrace_get_next_frame(&stk_frame)) {
      corrupted = true;
      break;
    }

    if (idx >= CRASHREC_MAX_FRAMES) break;

    uint32_t pc = esp_cpu_process_stack_pc(stk_frame.pc);
    g_crashrec_rtc.stack[idx++] = pc;

    // Optional: if you want to stop on non-executable PCs to reduce junk:
    // if (!esp_ptr_executable((void*)pc)) break;
  }
}


// ------------------------------------------------------------
// Linker-wrap hooks (match Tasmota)
// Build flags:
//   -Wl,--wrap=panicHandler -Wl,--wrap=xt_unhandled_exception
// ------------------------------------------------------------

extern "C" {

// Provided by linker when using --wrap
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


// taken in panic.c, but orginal array is 'static' so can't be called
// From ESP-IDF panic.c (but theirs is static). Keep local copy for JSON reason strings.
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

static constexpr uint32_t CRASHREC_NUM_EDESC = (uint32_t)(sizeof(CRASHREC_EDESC) / sizeof(CRASHREC_EDESC[0]));

static inline const char* CrashRecorder__ExcReasonStr(uint32_t exccause)
{
  return (exccause < CRASHREC_NUM_EDESC) ? CRASHREC_EDESC[exccause] : "Unknown";
}

bool CrashRecorder::Create_CrashDumpJSON(char* out, uint16_t out_len) const
{
  if (!out || out_len == 0) return false;
  out[0] = '\0';

  if (!CrashRTC_HasRecord()) return false;

  const uint32_t exccause = (uint32_t)g_crashrec_rtc.exccause;

  // Default for non-Xtensa targets (or if table not compiled in)
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
  for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) {
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
}


#endif // ESP32












































































// //////////////////////////////////////////////////////////////////////
// #ifdef ESP8266

// #endif


#ifdef ESP32

// // -----------------------------
// // RTC storage (survives reset)
// // -----------------------------

// #ifndef CRASHREC_MAX_FRAMES
//   #define CRASHREC_MAX_FRAMES  48
// #endif

// static constexpr uint32_t CRASHREC_MAGIC = 0x53415400; // "TAS\0" style marker, any constant you like

// RTC_NOINIT_ATTR volatile struct {
//   uint32_t magic;
//   uint32_t stack[CRASHREC_MAX_FRAMES];
//   uint32_t pc;
//   uint32_t exccause;
//   uint32_t excvaddr;
// } g_crashrec_rtc;


// // -----------------------------
// // Instance hook for wrapper C handlers
// // -----------------------------

// CrashRecorder* g_crashrec_this = nullptr;


// // -----------------------------
// // Forward declares to avoid missing headers
// // -----------------------------

// extern "C" {
//   // Provided by ESP-IDF / Arduino core; we just declare it.
//   uint32_t esp_cpu_process_stack_pc(uint32_t pc);

//   // Wrapping targets (the real ones)
//   void __real_panicHandler(XtExcFrame *frame);
//   void __real_xt_unhandled_exception(XtExcFrame *frame);
// }


// // -----------------------------
// // Crash capture implementation
// // -----------------------------

// void CrashRecorder::CrashRTC_CaptureFromFrame(XtExcFrame* exc_frame)
// {
//   g_crashrec_rtc.magic    = CRASHREC_MAGIC;
//   g_crashrec_rtc.pc       = exc_frame ? exc_frame->pc       : 0;
//   g_crashrec_rtc.exccause = exc_frame ? exc_frame->exccause : 0;
//   g_crashrec_rtc.excvaddr = exc_frame ? exc_frame->excvaddr : 0;

//   for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) {
//     g_crashrec_rtc.stack[i] = 0;
//   }

//   if (!exc_frame) return;

//   uint32_t idx = 0;

//   // Start frame (same as IDF panic/backtrace)
//   esp_backtrace_frame_t stk_frame;
//   stk_frame.pc      = exc_frame->pc;
//   stk_frame.sp      = exc_frame->a1;
//   stk_frame.next_pc = exc_frame->a0;
//   stk_frame.exc_frame = exc_frame;

//   // Store first PC (processed)
//   g_crashrec_rtc.stack[idx++] = esp_cpu_process_stack_pc(stk_frame.pc);

//   // Validate first frame
//   bool corrupted = !(esp_stack_ptr_is_sane(stk_frame.sp) &&
//                      (esp_ptr_executable((void*)esp_cpu_process_stack_pc(stk_frame.pc)) ||
//                       // ignore first bad PC for InstrFetchProhibited
//                       (stk_frame.exc_frame && ((XtExcFrame*)stk_frame.exc_frame)->exccause == EXCCAUSE_INSTR_PROHIBITED)));

//   const uint32_t depth = 100;
//   uint32_t i = depth - 1;

//   while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {
//     if (!esp_backtrace_get_next_frame(&stk_frame)) {
//       corrupted = true;
//       break;
//     }

//     if (idx >= CRASHREC_MAX_FRAMES) break;
//     g_crashrec_rtc.stack[idx++] = esp_cpu_process_stack_pc(stk_frame.pc);
//   }
// }


// // -----------------------------
// // Linker wrap handlers (global C, but calls into your class instance)
// // -----------------------------

// extern "C" IRAM_ATTR void __wrap_panicHandler(XtExcFrame *frame)
// {
//   ets_printf("CrashRecorder: in panicHandler wrapper\n");
//   tkr_crash_recorder->CrashRTC_CaptureFromFrame(frame);
//   __real_panicHandler(frame);
// }

// extern "C" IRAM_ATTR void __wrap_xt_unhandled_exception(XtExcFrame *frame)
// {
//   ets_printf("CrashRecorder: in __wrap_xt_unhandled_exception wrapper\n");
//  tkr_crash_recorder->CrashRTC_CaptureFromFrame(frame);
//   __real_xt_unhandled_exception(frame);
// }



// -----------------------------
// Public API
// -----------------------------

void CrashRecorder::Init()
{
  // g_crashrec_this = this;
}

bool CrashRecorder::CrashRTC_HasRecord() const
{
  return (g_crashrec_rtc.magic == CRASHREC_MAGIC);
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

// bool CrashRecorder::Create_CrashDumpJSON(char* out, uint16_t out_len) const
// {
//   if (!out || out_len == 0) return false;
//   out[0] = '\0';

//   if (!CrashRTC_HasRecord()) return false;

//   uint16_t used = 0;

//   int n = snprintf(out + used, out_len - used,
//                    "{\"ESP32\":{\"Exception\":%u,\"EPC\":\"0x%08x\",\"EXCVADDR\":\"0x%08x\",\"CallChain\":[",
//                    (unsigned)g_crashrec_rtc.exccause,
//                    (unsigned)g_crashrec_rtc.pc,
//                    (unsigned)g_crashrec_rtc.excvaddr);
//   if (n < 0 || (uint16_t)n >= (out_len - used)) return false;
//   used += (uint16_t)n;

//   bool first = true;
//   for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) {
//     uint32_t pc = g_crashrec_rtc.stack[i];
//     if (!pc) break;

//     n = snprintf(out + used, out_len - used,
//                  "%s\"0x%08x\"",
//                  first ? "" : ",",
//                  (unsigned)pc);
//     if (n < 0 || (uint16_t)n >= (out_len - used)) return false;
//     used += (uint16_t)n;
//     first = false;
//   }

//   n = snprintf(out + used, out_len - used, "]}}\n");
//   if (n < 0 || (uint16_t)n >= (out_len - used)) return false;
//   used += (uint16_t)n;

//   return true;
// }


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

#endif // ESP32


// #ifdef ESP32

// #include "esp_system.h"
// #include "xtensa/xtensa_context.h"   // XtExcFrame

// #ifndef CRASHREC_MAX_FRAMES
//   #define CRASHREC_MAX_FRAMES  48
// #endif

// // Address range filter (good enough for ESP32 app/IRAM/IROM)
// static const uint32_t CRASHREC_ADDR_MIN = 0x40000000UL;
// static const uint32_t CRASHREC_ADDR_MAX = 0x42800000UL;

// // One retained record only
// typedef struct {
//   uint32_t magic;
//   uint16_t version;
//   uint16_t count;
//   uint32_t reset_reason;
//   uint32_t pc[CRASHREC_MAX_FRAMES];
// } crashrec_rtc_t;

// static constexpr uint32_t CRASHREC_MAGIC   = 0x43525348; // 'CRSH'
// static constexpr uint16_t CRASHREC_VERSION = 1;

// RTC_NOINIT_ATTR crashrec_rtc_t g_crashrec_rtc;

// // Optional: point to active instance (lets wrapper call into member function)
// static CrashRecorder* g_crashrec_this = nullptr;

// extern "C" {
// // Provided by linker with -Wl,--wrap=esp_panic_handler
// void __real_esp_panic_handler(void* frame);
// }

// // -------- CrashRecorder member methods (NO static helpers) --------

// void CrashRecorder::CrashHook_Install()
// {
//   // With --wrap=esp_panic_handler there is nothing to "install".
//   // We just register the instance so the wrapper can call into it.
//   g_crashrec_this = this;
// }

// bool CrashRecorder::CrashRTC_HasRecord()
// {
//   if (g_crashrec_rtc.magic != CRASHREC_MAGIC) return false;
//   if (g_crashrec_rtc.version != CRASHREC_VERSION) return false;
//   if (g_crashrec_rtc.count == 0 || g_crashrec_rtc.count > CRASHREC_MAX_FRAMES) return false;
//   return true;
// }

// void CrashRecorder::CrashRTC_Clear()
// {
//   g_crashrec_rtc.magic = 0;
//   g_crashrec_rtc.version = 0;
//   g_crashrec_rtc.count = 0;
//   g_crashrec_rtc.reset_reason = 0;
//   for (uint16_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) g_crashrec_rtc.pc[i] = 0;
// }

// void CrashRecorder::CrashRTC_Print()
// {
//   if (!CrashRTC_HasRecord()) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: no record"));
//     return;
//   }

//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: reset_reason=%u frames=%u"),
//            (unsigned)g_crashrec_rtc.reset_reason,
//            (unsigned)g_crashrec_rtc.count);

//   for (uint16_t i = 0; i < g_crashrec_rtc.count; ++i) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: PC[%u]=0x%08x"),
//              (unsigned)i, (unsigned)g_crashrec_rtc.pc[i]);
//   }
// }

// void CrashRecorder::CrashRTC_PrintAndClear()
// {
//   CrashRTC_Print();
//   CrashRTC_Clear();
//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: cleared"));
// }

// // Best-effort capture: fault PC + RA + scan some of the fault stack for return addresses
// void IRAM_ATTR CrashRecorder::CrashRTC_CaptureFromFrame(void* frame)
// {
//   XtExcFrame* xf = (XtExcFrame*)frame;

//   g_crashrec_rtc.magic        = CRASHREC_MAGIC;
//   g_crashrec_rtc.version      = CRASHREC_VERSION;
//   g_crashrec_rtc.count        = 0;
//   g_crashrec_rtc.reset_reason = (uint32_t)esp_reset_reason();

//   if (!xf) return;

//   // 1) Direct registers
//   const uint32_t pc0 = (uint32_t)xf->pc;
//   const uint32_t ra0 = (uint32_t)xf->a0;
//   const uint32_t sp0 = (uint32_t)xf->a1;

//   if (pc0 >= CRASHREC_ADDR_MIN && pc0 < CRASHREC_ADDR_MAX && g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//     g_crashrec_rtc.pc[g_crashrec_rtc.count++] = pc0;

//   if (ra0 >= CRASHREC_ADDR_MIN && ra0 < CRASHREC_ADDR_MAX && g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//     g_crashrec_rtc.pc[g_crashrec_rtc.count++] = ra0;

//   // 2) Stack scan (simple filter, no extra headers)
//   // Scan up to N words from SP. Keep it bounded.
//   const uint32_t* sp = (const uint32_t*)sp0;
//   const uint32_t max_words = 320;   // ~1280 bytes
//   for (uint32_t i = 0; i < max_words && g_crashrec_rtc.count < CRASHREC_MAX_FRAMES; ++i) {
//     uint32_t v = sp[i];
//     if (v >= CRASHREC_ADDR_MIN && v < CRASHREC_ADDR_MAX) {
//       // Optional de-dup of immediate repeats
//       if (g_crashrec_rtc.count == 0 || g_crashrec_rtc.pc[g_crashrec_rtc.count - 1] != v) {
//         g_crashrec_rtc.pc[g_crashrec_rtc.count++] = v;
//       }
//     }
//   }

//   // If we captured nothing meaningful, invalidate
//   if (g_crashrec_rtc.count == 0) {
//     g_crashrec_rtc.magic = 0;
//   }
// }

// // -------- The actual wrapped panic handler --------

// extern "C" void IRAM_ATTR __wrap_esp_panic_handler(void* frame)
// {
//   if (g_crashrec_this) {
//     g_crashrec_this->CrashRTC_CaptureFromFrame(frame);
//   } else {
//     // Fallback: capture minimal without instance
//     XtExcFrame* xf = (XtExcFrame*)frame;
//     g_crashrec_rtc.magic        = CRASHREC_MAGIC;
//     g_crashrec_rtc.version      = CRASHREC_VERSION;
//     g_crashrec_rtc.count        = 0;
//     g_crashrec_rtc.reset_reason = (uint32_t)esp_reset_reason();
//     if (xf) {
//       g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)xf->pc;
//       g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)xf->a0;
//     }
//   }

//   __real_esp_panic_handler(frame);
// }

// #endif // ESP32




// #ifdef ESP32

// #include "esp_system.h"
// #include "esp_debug_helpers.h"

// #if defined __has_include && __has_include("xtensa_api.h")
//   #include "xtensa_api.h"
// #else
//   #include "freertos/xtensa_api.h"
// #endif

// #include "xtensa/xtensa_context.h"   // XtExcFrame

// #ifndef CRASHREC_MAX_FRAMES
//   #define CRASHREC_MAX_FRAMES  48
// #endif

// static constexpr uint32_t CRASHREC_MAGIC = 0x53415400;   // TASx

// RTC_NOINIT_ATTR volatile struct {
//   uint32_t magic;
//   uint32_t stack[CRASHREC_MAX_FRAMES];
//   uint32_t pc;
//   uint32_t exccause;
//   uint32_t excvaddr;
// } crash_recorder;

// extern "C" {

// void __real_panicHandler(XtExcFrame *frame);
// void __real_xt_unhandled_exception(XtExcFrame *frame);

// void IRAM_ATTR custom_crash_recorder(XtExcFrame *exc_frame)
// {
//   crash_recorder.magic    = CRASHREC_MAGIC;
//   crash_recorder.pc       = exc_frame ? exc_frame->pc       : 0;
//   crash_recorder.exccause = exc_frame ? exc_frame->exccause : 0;
//   crash_recorder.excvaddr = exc_frame ? exc_frame->excvaddr : 0;

//   for (uint32_t i = 0; i < CRASHREC_MAX_FRAMES; i++) crash_recorder.stack[i] = 0;
//   if (!exc_frame) return;

//   uint32_t idx = 0;

//   esp_backtrace_frame_t stk_frame;
//   stk_frame.pc      = exc_frame->pc;
//   stk_frame.sp      = exc_frame->a1;
//   stk_frame.next_pc = exc_frame->a0;

//   crash_recorder.stack[idx++] = (uint32_t)stk_frame.pc;

//   bool corrupted = (esp_stack_ptr_is_sane(stk_frame.sp) &&
//                     esp_ptr_executable((void*)stk_frame.pc)) ? false : true;

//   static const uint32_t depth = 100;
//   uint32_t i = depth - 1;

//   while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {
//     if (!esp_backtrace_get_next_frame(&stk_frame)) {
//       corrupted = true;
//       break;
//     }
//     crash_recorder.stack[idx++] = (uint32_t)stk_frame.pc;
//     if (idx >= CRASHREC_MAX_FRAMES) break;
//   }
// }

// void IRAM_ATTR __wrap_panicHandler(XtExcFrame *frame)
// {
//   custom_crash_recorder(frame);
//   __real_panicHandler(frame);
// }

// void IRAM_ATTR __wrap_xt_unhandled_exception(XtExcFrame *frame)
// {
//   custom_crash_recorder(frame);
//   __real_xt_unhandled_exception(frame);
// }

// } // extern "C"

// #endif // ESP32






// /////////////////////////

// #ifdef ESP32
// extern "C" {
//   #include "esp_system.h"
//   // #include "esp_freertos_hooks.h"
//   #include "xtensa/xtensa_context.h"   // defines XtExcFrame

// }

// #endif


// #ifdef ESP32

// #ifndef CRASHREC_MAX_FRAMES
//   #define CRASHREC_MAX_FRAMES  16
// #endif

// // static constexpr uint32_t CRASHREC_MAGIC   = 0x43525348; // 'CRSH'
// static constexpr uint16_t CRASHREC_VERSION = 1;

// typedef struct {
//   uint32_t magic;
//   uint16_t version;
//   uint16_t count;
//   uint32_t reset_reason;
//   uint32_t pc[CRASHREC_MAX_FRAMES];
// } crashrec_rtc_t;

// // Retained across reset (while powered). Not zeroed on boot.
// RTC_NOINIT_ATTR crashrec_rtc_t g_crashrec_rtc;

// static inline void crashrec_rtc_clear_unsafe()
// {
//   g_crashrec_rtc.magic = 0;
//   g_crashrec_rtc.version = 0;
//   g_crashrec_rtc.count = 0;
//   g_crashrec_rtc.reset_reason = 0;
//   for (uint16_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) g_crashrec_rtc.pc[i] = 0;
// }

// static inline bool crashrec_rtc_valid()
// {
//   if (g_crashrec_rtc.magic != CRASHREC_MAGIC) return false;
//   if (g_crashrec_rtc.version != CRASHREC_VERSION) return false;
//   if (g_crashrec_rtc.count == 0 || g_crashrec_rtc.count > CRASHREC_MAX_FRAMES) return false;
//   return true;
// }

// extern "C" {

// // Provided by linker when using -Wl,--wrap=esp_panic_handler
// void __real_esp_panic_handler(void* frame);

// void IRAM_ATTR __wrap_esp_panic_handler(void* frame)
// {
//   XtExcFrame* xf = (XtExcFrame*)frame;

//   g_crashrec_rtc.magic        = CRASHREC_MAGIC;
//   g_crashrec_rtc.version      = CRASHREC_VERSION;
//   g_crashrec_rtc.count        = 0;
//   g_crashrec_rtc.reset_reason = (uint32_t)esp_reset_reason();

//   if (xf) {
//     // faulting instruction PC
//     g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->pc;

//     // return address
//     if (g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//       g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->a0;

//     // optional: store SP later if you add a field for it
//     // if (g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//     //   g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->a1;
//   }

//   __real_esp_panic_handler(frame);
// }


// } // extern "C"

// #endif // ESP32




// #ifdef ESP32

// #ifndef CRASHREC_MAX_FRAMES
//   #define CRASHREC_MAX_FRAMES  16   // keep small, fast
// #endif

// static constexpr uint32_t CRASHREC_MAGIC   = 0x43525348; // 'CRSH'
// static constexpr uint16_t CRASHREC_VERSION = 1;

// // typedef struct {
// //   uint32_t magic;
// //   uint16_t version;
// //   uint16_t count;
// //   uint32_t reset_reason;
// //   uint32_t pc[CRASHREC_MAX_FRAMES];
// // } crashrec_rtc_t;

// // // Retained across reset (while powered). Not zeroed on boot.
// // RTC_NOINIT_ATTR static crashrec_rtc_t g_crashrec_rtc;

// static inline void crashrec_rtc_clear_unsafe()
// {
//   g_crashrec_rtc.magic = 0;
//   g_crashrec_rtc.version = 0;
//   g_crashrec_rtc.count = 0;
//   g_crashrec_rtc.reset_reason = 0;
//   for (uint16_t i = 0; i < CRASHREC_MAX_FRAMES; ++i) g_crashrec_rtc.pc[i] = 0;
// }

// static inline bool crashrec_rtc_valid()
// {
//   if (g_crashrec_rtc.magic != CRASHREC_MAGIC) return false;
//   if (g_crashrec_rtc.version != CRASHREC_VERSION) return false;
//   if (g_crashrec_rtc.count == 0 || g_crashrec_rtc.count > CRASHREC_MAX_FRAMES) return false;
//   return true;
// }

// extern "C" void __real_esp_panic_handler(void* frame);

// // // This replaces esp_panic_handler when linked with -Wl,--wrap=esp_panic_handler
// // extern "C" void IRAM_ATTR __wrap_esp_panic_handler(void* frame)
// // {
// //   // capture into RTC
// //   CrashRecorder__PanicHook();

// //   // continue normal panic handling (prints backtrace, reboots, etc.)
// //   __real_esp_panic_handler(frame);
// // }


// #endif // ESP32

// #ifdef ESP32

// #ifdef ESP32


// #ifdef ESP32
// extern "C" {

// // Forward to the real ESP-IDF handler (provided by the linker's --wrap)
// void __real_esp_panic_handler(void* frame);

// // Xtensa exception frame layout (Arduino ESP32 uses Xtensa for ESP32/S2/S3)
// typedef struct {
//   uint32_t pc;      // Program counter at exception
//   uint32_t ps;
//   uint32_t a0;      // return address
//   uint32_t a1;      // stack pointer at exception
//   uint32_t a2;
//   uint32_t a3;
//   uint32_t a4;
//   uint32_t a5;
//   uint32_t a6;
//   uint32_t a7;
//   uint32_t a8;
//   uint32_t a9;
//   uint32_t a10;
//   uint32_t a11;
//   uint32_t a12;
//   uint32_t a13;
//   uint32_t a14;
//   uint32_t a15;
//   uint32_t sar;
//   uint32_t exc;
//   uint32_t excvaddr;
//   uint32_t lbegin;
//   uint32_t lend;
//   uint32_t lcount;
// } XtExcFrame;

// // Your existing RTC record
// extern RTC_NOINIT_ATTR crashrec_rtc_t g_crashrec_rtc;

// void IRAM_ATTR __wrap_esp_panic_handler(void* frame)
// {
//   // Best effort: capture fault PC + caller + SP
//   XtExcFrame* xf = (XtExcFrame*)frame;

//   g_crashrec_rtc.magic        = CRASHREC_MAGIC;
//   g_crashrec_rtc.version      = CRASHREC_VERSION;
//   g_crashrec_rtc.count        = 0;
//   g_crashrec_rtc.reset_reason = (uint32_t)esp_reset_reason();

//   if (xf) {
//     // Faulting instruction address (this is what you expect to map to line 320)
//     g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->pc;

//     // Return address (often useful)
//     if (g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//       g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->a0;

//     // Optional: you can also store SP for later stack scanning
//     // (either store separately or steal a slot)
//     // if (g_crashrec_rtc.count < CRASHREC_MAX_FRAMES)
//     //   g_crashrec_rtc.pc[g_crashrec_rtc.count++] = xf->a1;
//   }

//   // Continue normal panic processing
//   __real_esp_panic_handler(frame);
// }

// } // extern "C"
// #endif



// // extern "C" void IRAM_ATTR CrashRecorder__PanicHook(void)
// // {
// //   g_crashrec_rtc.magic        = CRASHREC_MAGIC;
// //   g_crashrec_rtc.version      = CRASHREC_VERSION;
// //   g_crashrec_rtc.count        = 0;
// //   g_crashrec_rtc.reset_reason = (uint32_t)esp_reset_reason();

// //   // ---- Capture best-effort PCs using compiler builtins ----
// //   // These are cheap, always available, and safe in panic context.

// //   void* pc0 = __builtin_return_address(0);
// //   void* pc1 = __builtin_return_address(1);
// //   void* pc2 = __builtin_return_address(2);
// //   void* pc3 = __builtin_return_address(3);

// //   if (pc0) g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)pc0;
// //   if (pc1) g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)pc1;
// //   if (pc2) g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)pc2;
// //   if (pc3) g_crashrec_rtc.pc[g_crashrec_rtc.count++] = (uint32_t)pc3;

// //   if (g_crashrec_rtc.count == 0) {
// //     g_crashrec_rtc.magic = 0; // invalidate junk
// //   }
// // }

// #endif


// #endif // ESP32


// #ifdef ESP32

// void CrashRecorder::CrashHook_Install()
// {
//   // No runtime install needed when using linker wrap of esp_panic_handler.
// }

// bool CrashRecorder::CrashRTC_HasRecord()
// {
//   return crashrec_rtc_valid();
// }

// void CrashRecorder::CrashRTC_Print()
// {
//   if (!crashrec_rtc_valid()) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: no record"));
//     return;
//   }

//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: reset_reason=%u frames=%u"),
//            (unsigned)g_crashrec_rtc.reset_reason,
//            (unsigned)g_crashrec_rtc.count);

//   // Print each PC as hex. Keep it simple for now.
//   for (uint16_t i = 0; i < g_crashrec_rtc.count; ++i) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: PC[%u]=0x%08x"),
//              (unsigned)i, (unsigned)g_crashrec_rtc.pc[i]);
//   }

// }

// void CrashRecorder::CrashRTC_PrintAndClear()
// {
  
//   CrashRTC_Print();
//   crashrec_rtc_clear_unsafe();
//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: cleared"));
// }

// #endif // ESP32



// #ifdef ESP8266
// extern "C" {
// #include <cont.h>
//   extern cont_t* g_pcont;
// }

// void mSupport::DebugFreeMem(void)
// {
//   //https://www.esp8266.com/viewtopic.php?p=69937
//   //https://i.stack.imgur.com/waoHN.gif
//   register uint32_t *sp asm("a1");

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d"), 
//       ESP.getFreeHeap(), 4 * (sp - g_pcont->stack));
//     #endif// ENABLE_LOG_LEVEL_INFO
// }

// // #endif  // ARDUINO_ESP8266_RELEASE_2_x_x

// #endif // ESP8266



#ifdef ESP8266
// All version from core 2.4.2
// https://github.com/esp8266/Arduino/pull/5018
// https://github.com/esp8266/Arduino/pull/4553

extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}

void DebugFreeMem(void) {
  register uint32_t *sp asm("a1");

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_pcont->stack), XdrvMailbox.data);
}

uint32_t FreeStack(void) {
  register uint32_t *sp asm("a1");
  return 4 * (sp - g_pcont->stack);
}

void AddLogMem(const char* function) {
  register uint32_t *sp asm("a1");
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "== %s FreeRam %d, FreeStack %d"), function, ESP.getFreeHeap(), 4 * (sp - g_pcont->stack));
}

#endif  // ESP8266
#ifdef ESP32

void DebugFreeMem(void) {
  register uint8_t *sp asm("a1");

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), sp - pxTaskGetStackStart(NULL), XdrvMailbox.data);
}

uint32_t FreeStack(void) {
  register uint8_t *sp asm("a1");
  return sp - pxTaskGetStackStart(NULL);
}

void AddLogMem(const char* function) {
  register uint8_t *sp asm("a1");
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "== %s FreeRam %d, FreeStack %d"), function, ESP.getFreeHeap(), sp - pxTaskGetStackStart(NULL));
}

#endif  // ESP8266 - ESP32

int8_t CrashRecorder::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
    //   Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
    //   BootMessage();
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
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }
  return 1;


} // END function

// void CrashRecorder::Init(void)
// {
//   module_state.mode = ModuleStatus::Running;

// #ifdef ESP32
//   CrashHook_Install();
//   // Optional: print on every boot if present (or call elsewhere later)
//   // CrashRTC_PrintAndClear();
// #endif
// }




//#ifdef ENABLE_CRASH_RECORDING

/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end )
{
  uint32_t addr_written = 0;      // how many addresses have we already written in RTC
  uint32_t value;                 // 4 bytes buffer to write to RTC

    #ifdef ESP8266
  for (uint32_t i = stack; i < stack_end; i += 4) {
    value = *((uint32_t*) i);     // load value from stack
    if ((value >= 0x40000000) && (value < 0x40300000)) {  // keep only addresses in code area
      ESP.rtcUserMemoryWrite(crash_rtc_offset + addr_written, (uint32_t*)&value, sizeof(value));
      addr_written++;
      if (addr_written >= crash_dump_max_len) { break; }  // we store only 31 addresses
    }
  }
  value = crash_magic + addr_written;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}

// Generate a crash to test the crash recorder
// void CrashRecorder::CmndCrash(void)
// {
//   volatile uint32_t dummy;
//   dummy = *((uint32_t*) 0x00000000);
// }
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



/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/

bool CrashRecorder::CrashFlag(void)
{
  
#ifdef ESP8266
  DEBUG_PRINT_FUNCTION_NAME;
  return ((ResetReason() == REASON_EXCEPTION_RST) || (ResetReason() == REASON_SOFT_WDT_RST));// || oswatch_blocked_loop);
#endif // ESP8266

}

void CrashRecorder::CrashDump_AddJson(void)
{
  
// #ifdef ESP8266
//   char buffer[30];

//   DEBUG_PRINT_FUNCTION_NAME;
//   tkr_sup->WriteBuffer_P(PSTR("\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":[\"%08x\",\"%08x\",\"%08x\"],\"EXCVADDR\":\"%08x\",\"DEPC\":\"%08x\""),
//     resetInfo.exccause,        // Exception Cause
//     tkr_sup->GetResetReason(buffer, sizeof(buffer)),  // Reset Reason
//     resetInfo.epc1,            // Exception Progam Counter
//     resetInfo.epc2,            // Exception Progam Counter - High-Priority Interrupt 1
//     resetInfo.epc3,            // Exception Progam Counter - High-Priority Interrupt 2
//     resetInfo.excvaddr,        // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
//     resetInfo.depc             // Double Exception Program Counter
//   );

//   uint32_t value;
//   ESP.rtcUserMemoryRead(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
//   if (crash_magic == (value & 0xFFFFFF00)) {
//     tkr_sup->WriteBuffer_P(PSTR(",\"CallChain\":["));
//     uint32_t count = value & 0x3F;
//     for (uint32_t i = 0; i < count; i++) {
//       ESP.rtcUserMemoryRead(crash_rtc_offset +i, (uint32_t*)&value, sizeof(value));
//       if (i > 0) { tkr_sup->WriteBuffer_P(PSTR(",")); }
//       tkr_sup->WriteBuffer_P(PSTR("\"%08x\""), value);
//     }
//     tkr_sup->WriteBuffer_P(PSTR("]"));
//   }
//   #endif // ESP8266
  
}

#ifdef ESP32

bool CrashRecorder::Create_CrashDump(char* out, uint16_t out_len)
{
  // Keep old symbol alive, but output the new unified JSON.
  return Create_CrashDumpJSON(out, out_len);
}

#endif // ESP32

#ifdef ESP32

void CrashRecorder::Splash__CrashDump()
{
  char buffer[500];

  if (!Create_CrashDump(buffer, sizeof(buffer))) {
    ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: none"));
    return;
  }

  ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump %s"), buffer);
}



#endif // ESP32

#ifdef ESP32

// bool CrashRecorder::FS_FileExists(const char* path)
// {
//   if (!path) return false;

//   File f = FILE_SYSTEM.open(path, "r");   // replace WLED_FS with LittleFS/SPIFFS/your FS instance
//   if (!f) return false;
//   f.close();
//   return true;
// }

// bool CrashRecorder::FS_WriteAll(const char* path, const char* data, size_t len)
// {
//   if (!path || !data) return false;

//   File f = FILE_SYSTEM.open(path, "w");   // truncate/write
//   if (!f) return false;

//   const size_t written = f.write((const uint8_t*)data, len);
//   f.close();

//   return (written == len);
// }

#endif // ESP32
#ifdef ESP32

// void CrashRecorder::FS_EnsureCrashReportFile()
// {
//   const char* path = "/crash_report.json";

//   if (FS_FileExists(path)) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashReport: exists %s"), path);
//     return;
//   }

//   const char* placeholder = "{\"valid\":0,\"note\":\"no crash recorded yet\"}\n";
//   const bool ok = FS_WriteAll(path, placeholder, strlen(placeholder));

//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashReport: create placeholder %s %s"),
//            path, ok ? "OK" : "FAIL");
// }

#endif // ESP32

#ifdef ESP32

// bool CrashRecorder::CrashDump_WriteToFile(bool clear_rtc_after_write)
// {
//   const char* path = "/crash_report.json";

//   // Build fragment: "ESP32":{...}
//   char frag[500];
//   if (!Create_CrashDump(frag, sizeof(frag))) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: no RTC record"));
//     return false;
//   }

//   // Wrap into full JSON document: { <fragment> }
//   char out[700];
//   int n = snprintf(out, sizeof(out), "{%s}\n", frag);
//   if (n < 0 || n >= (int)sizeof(out)) {
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: output buffer too small"));
//     return false;
//   }

//   const bool ok = FS_WriteAll(path, out, (size_t)n);
//   ALOG_INF(PSTR(D_LOG_DEBUG "CrashDump: write %s %s"), path, ok ? "OK" : "FAIL");

//   if (ok && clear_rtc_after_write) {
//     CrashRTC_Clear();
//     ALOG_INF(PSTR(D_LOG_DEBUG "CrashRTC: cleared after write"));
//   }

//   return ok;
// }

#endif // ESP32



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
    tkr_sup->WriteBuffer_P(PSTR(","));
    tkr_crash_recorder->CrashDump_AddJson();
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
    #ifdef ESP32
      Splash__CrashDump();
    #endif
    }


  if (jtok = jobj["CreateCrashDumpJSON"]) {
    ALOG_INF(PSTR("CreateCrashDumpJSON command received"));
  #ifdef ESP32
    // optional: print the JSON fragment to log for visibility
    Splash__CrashDump();

    // write crash_report.json (clears RTC after successful write)
    CrashDump_WriteToFile(true);
  #endif
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
  ptr->tRateSecs = 60;
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &CrashRecorder::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CORE__CRASH_RECORDER