
#pragma once

enum DATA_BUFFER_FLAG_SOURCE_IDS
{
  DATA_BUFFER_FLAG_SOURCE_MQTT=0,
  DATA_BUFFER_FLAG_SOURCE_WEBUI
};


typedef union 
{
  uint16_t data;
  struct { 
    // 3 bits (9 values)
    // (0) DATA_BUFFER_FLAG_SOURCE_MQTT  // Defaulted to 0
    // (1) DATA_BUFFER_FLAG_SOURCE_WEBUI
    uint16_t source_id : 4;
    // Waiting
    uint16_t waiting : 1;
    // Encoding format
    uint16_t encoded_type_id : 1; // json,raw
    uint16_t reserved : 10;
  };
} DATA_BUFFER_FLAGS;


#include <Arduino.h>  // millis(), delay(), Serial

// Keep your existing macros
#define ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
// #define ENABLE_DEVFEATURE_DATABUFFER_LOCK

#define DATA_BUFFER_TOPIC_MAX_LENGTH    100
#define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000 

struct DATA_BUFFER {
  struct TOPIC {
    char     ctr[DATA_BUFFER_TOPIC_MAX_LENGTH];
    uint16_t length_used = 0;
  } topic;

  struct PAYLOAD {
    char     ctr[DATA_BUFFER_PAYLOAD_MAX_LENGTH];
    uint16_t length_used = 0;
  } payload;

  uint16_t isserviced  = 0;   // Set to 0 on new mqtt
  uint16_t moduleLock  = 0;
  bool     delayedJSONCommandWaiting  = false;
  DATA_BUFFER_FLAGS flags;

  // ---------- Lock API ----------
  inline bool requestLock(uint16_t module, uint32_t timeout_ms = 1000) {
  #ifdef ENABLE_DEVFEATURE_DATABUFFER_LOCK
    const unsigned long now = millis();

    // This assumption here is another http thread must release itself to permit this function to proceed
    while (moduleLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

    if (millis() - now >= timeout_ms) {
      #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
        Serial.printf(PSTR("ERROR: Locking data buffer failed! (%u)\r\n"), (unsigned)moduleLock);
      #endif
      return false;
    }

    moduleLock = module ? module : 255;

    #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
      Serial.printf(PSTR("DATA buffer requestLock =======================================> (%u)\r\n"), (unsigned)moduleLock);
    #endif

    return true;
  #else
    (void)module; (void)timeout_ms;
    return true;
  #endif
  }

  inline bool tryLock(uint16_t module)
  {
    if (moduleLock != 0)
    {      
      #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
        Serial.printf(PSTR("DATA buffer tryLock BLOCKED <<<<<<<<<<< (%u)\r\n"), (unsigned)moduleLock);
      #endif
      return false;
    }
    moduleLock = module ? module : 255;
    #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
      Serial.printf(PSTR("DATA buffer tryLock =======================================> (%u)\r\n"), (unsigned)moduleLock);
    #endif
    return true;
  }


  inline void releaseLock() {
    #ifdef ENABLE_DEVFEATURE_DATABUFFER_LOCK
      #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
        Serial.printf(PSTR("DATA buffer released (%u)\r\n"), (unsigned)moduleLock);
      #endif
      moduleLock = 0;
      delayedJSONCommandWaiting = 0;
    #endif
  }

  bool IsDelayedJSONCommandWaiting() {
    return delayedJSONCommandWaiting;
  }

  void ClearDeep() {
    // memset(this, 0, sizeof(DATA_BUFFER)); // cant do this, its destroying itself
    memset(&topic, 0, sizeof(TOPIC));
    memset(&payload, 0, sizeof(PAYLOAD));
    ClearSoft();
  }

  void ClearSoft() {
    Serial.printf("DATA buffer ClearSoft <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
    topic.ctr[0] = 0;
    topic.length_used = 0;
    payload.ctr[0] = 0;
    payload.length_used = 0;
    isserviced = 0;
  }

};
// Keep your existing global instance
extern DATA_BUFFER data_buffer;