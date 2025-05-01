#include "2_CoreSystem/06_Support/SupportESP8266.h"

#ifdef ESP8266

#include "Arduino.h"


uint32_t SupportESP8266::ESP_ResetInfoReason(void) {
  // return resetInfo.reason;
  return ESP.getResetInfoPtr()->reason;
}

uint32_t SupportESP8266::ESP_getChipId(void) {
  return ESP.getChipId();
}



#endif







