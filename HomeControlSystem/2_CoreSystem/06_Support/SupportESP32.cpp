#include "2_CoreSystem/06_Support/SupportESP32.h"

#ifdef ESP32

#include "Arduino.h"


uint32_t SupportESP32::ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}


// extern "C" {
//   bool esp_spiram_is_initialized(void);
// }

// // this function is a replacement for `psramFound()`.
// // `psramFound()` can return true even if no PSRAM is actually installed
// // This new version also checks `esp_spiram_is_initialized` to know if the PSRAM is initialized
// bool FoundPSRAM(void) {
// #if CONFIG_IDF_TARGET_ESP32C3
//   return psramFound();
// #else
//   return psramFound() && esp_spiram_is_initialized();
// #endif
// }

// // new function to check whether PSRAM is present and supported (i.e. required pacthes are present)
// bool UsePSRAM(void) {
//   static bool can_use_psram = CanUsePSRAM();
//   return FoundPSRAM() && can_use_psram;
// }

// void *special_malloc(uint32_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return malloc(size);
//   }
// }
// void *special_realloc(void *ptr, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return realloc(ptr, size);
//   }
// }
// void *special_calloc(size_t num, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return calloc(num, size);
//   }
// }

// // Variants for IRAM heap, which need all accesses to be 32 bits aligned
// void *special_malloc32(uint32_t size) {
//   return heap_caps_malloc(size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_realloc32(void *ptr, size_t size) {
//   return heap_caps_realloc(ptr, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_calloc32(size_t num, size_t size) {
//   return heap_caps_calloc(num, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }






#endif







