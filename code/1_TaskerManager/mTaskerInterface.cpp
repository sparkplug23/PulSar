#include "mTaskerInterface.h"
#include <Arduino.h>

mTaskerInterface::~mTaskerInterface() {
  Serial.println("~mTaskerInterface() called");
}

int8_t mTaskerInterface::Tasker(uint8_t, JsonParserObject) {
  return -1;
}

PGM_P mTaskerInterface::GetModuleName() {
  return PSTR("BaseTasker");
}

uint16_t mTaskerInterface::GetModuleUniqueID() {
  return 0;
}
