
#include "JsonArduino.h"

#ifdef USE_MODULE_CORE__JSON_ARDUINO


void mJsonArduino::Init()
{

    
  static bool done = false;
  if(done) { return; }
  done = true;

  /************************************************************************************************
   * SECTION: JSON BUFFER / PSRAM INITIALISATION
   *
   * SD card mounting is intentionally NOT done here.
   ************************************************************************************************/

#if defined(ARDUINO_ARCH_ESP32)
  #if !defined(BOARD_HAS_PSRAM) && !(defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3))
  if (psramFound() && ESP.getChipRevision() < 3) psramSafe = false;
  if (!psramSafe) DEBUG_PRINTLN(F("Not using PSRAM."));
  #endif

  if(!pDoc)
  {
    pDoc = new PSRAMDynamicJsonDocument((psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE);

    DEBUG_PRINTF_P(
      PSTR("JSON buffer allocated: %u\n"),
      (psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE
    );

    if (psramFound()) {
      DEBUG_PRINTF_P(
        PSTR("PSRAM: %dkB/%dkB\n"),
        ESP.getFreePsram() / 1024,
        ESP.getPsramSize() / 1024
      );
    }
  }
#endif


}



//threading/network callback details: https://github.com/wled-dev/WLED/pull/2336#discussion_r762276994
bool mJsonArduino::requestJSONBufferLock(uint16_t moduleID)
{
  #ifdef USE_MODULE_CORE_FILESYSTEM
  if (tkr_jsona->pDoc == nullptr) {
    DEBUG_PRINTLN(F("ERROR: JSON buffer not allocated!"));
    return false;
  }
  #endif
  
#if defined(ARDUINO_ARCH_ESP32)
  // Use a recursive mutex type in case our task is the one holding the JSON buffer.
  // This can happen during large JSON web transactions.  In this case, we continue immediately
  // and then will return out below if the lock is still held.
  if (xSemaphoreTakeRecursive(jsonBufferLockMutex, 250) == pdFALSE) return false;  // timed out waiting
#elif defined(ARDUINO_ARCH_ESP8266)
  // If we're in system context, delay() won't return control to the user context, so there's
  // no point in waiting.
  if (can_yield()) {
    unsigned long now = millis();
    while (jsonBufferLock && (millis()-now < 250)) delay(1); // wait for fraction for buffer lock
  }
#else
  #error Unsupported task framework - fix requestJSONBufferLock
#endif  
  // If the lock is still held - by us, or by another task
  if (jsonBufferLock) {
    ALOG_ERR(PSTR("ERROR: Locking JSON buffer (%d) failed! (still locked by %d)\n"), moduleID, jsonBufferLock);
#ifdef ARDUINO_ARCH_ESP32
    xSemaphoreGiveRecursive(jsonBufferLockMutex);
#endif
    return false;
  }

  jsonBufferLock = moduleID ? moduleID : 255;
  ALOG_DBM(PSTR("JSON locked (%d)\n\r"), jsonBufferLock);
  #ifdef USE_MODULE_CORE_FILESYSTEM
  tkr_jsona->pDoc->clear();
  #endif
  return true;
}

void  mJsonArduino::releaseJSONBufferLock()
{
  ALOG_DBM(PSTR("JSON released (%d)\n\r"), jsonBufferLock);
  jsonBufferLock = 0;
#ifdef ARDUINO_ARCH_ESP32
  xSemaphoreGiveRecursive(jsonBufferLockMutex);
#endif  
}




#endif