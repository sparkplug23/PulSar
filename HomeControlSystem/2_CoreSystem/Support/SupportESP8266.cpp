#include "2_CoreSystem/Support/SupportESP8266.h"

#ifdef ESP8266

#include "Arduino.h"


uint32_t SupportESP8266::ESP_getChipId(void) {
  return ESP.getChipId();
}



#endif







