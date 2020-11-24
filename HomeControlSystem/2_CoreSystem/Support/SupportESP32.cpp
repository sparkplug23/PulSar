#include "2_CoreSystem/Support/SupportESP32.h"

#ifdef ESP32

#include "Arduino.h"


uint32_t SupportESP32::ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}



#endif







