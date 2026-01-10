#include "mJSON.h"


char* dtostrfd2(double number, unsigned char prec, char *s)
{

  // Already included in "dtostrf"
  // if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
  //   // strcpy(s, "null"); // error fix? turn off
  //   return s;
  // } else {
    return dtostrf(number, 1, prec, s);
  // }

  // return s;
}


char* GetIndentifierNameByID(uint8_t id, char* buffer){
    if(buffer == nullptr){ return 0;}
    switch(id){
        case IDENTIFIER_NUMBER_ID: memcpy_P(buffer, PM_IDENTIFIER_NUMBER_CTR, sizeof(PM_IDENTIFIER_NUMBER_CTR)); break;
        case IDENTIFIER_STRING_ID: memcpy_P(buffer, PM_IDENTIFIER_STRING_CTR, sizeof(PM_IDENTIFIER_STRING_CTR)); break;
        case IDENTIFIER_CHAR_ID:   memcpy_P(buffer, PM_IDENTIFIER_CHAR_CTR, sizeof(PM_IDENTIFIER_CHAR_CTR)); break;
        case IDENTIFIER_FLOAT_ID:  memcpy_P(buffer, PM_IDENTIFIER_FLOAT_CTR, sizeof(PM_IDENTIFIER_FLOAT_CTR)); break;
    }
    return buffer;
}


/* Null, because instance will be initialized on demand. */
JsonBuilder* JsonBuilder::instance = nullptr;

JsonBuilder* JsonBuilder::GetInstance(){
  if (instance == nullptr){
    instance = new JsonBuilder();
    // PRINT_FLUSHED("instance = new JsonBuilder()");
  }
  return instance;
}



char* JsonBuilder::GetBufferPtr(){
  return writer.buffer;
}
char* JsonBuilder::GetPtr(){ // To make it the same as BufferWriter
  return writer.buffer;
}
uint16_t JsonBuilder::GetLength(){
  return writer.length;
}
// uint16_t* JsonBuilder::GetLengthPtr(){
//   return writer.length;
// }
uint16_t JsonBuilder::GetBufferSize(){
  return writer.buffer_size;
}


#ifdef ENABLE_DEVFEATURE_MJSON__FLOAT_SPECIALIZATION

// Specialization for float types
template <>
void JsonBuilder::Add_P<float>(const char* key, float value) {
    if (writer.buffer == nullptr || writer.buffer_size == 0)
        return;

    if ((writer.length > 1) &&
        (writer.buffer[writer.length - 1] != '{') &&
        (writer.buffer[writer.length - 1] != '[')) {
        writer.length += snprintf(&writer.buffer[writer.length], writer.buffer_size, ",");
    }

    // Handle float precision and conversion to string
    char fvalue[20];  // Buffer to store the float as a string
    dtostrf(value, 5, 3, fvalue);  // Convert float to string with precision 5.3
    writer.length += snprintf(&writer.buffer[writer.length], writer.buffer_size, "\"%s\":%s", key, fvalue);
}

// Specialization for float types
template <>
void JsonBuilder::Add<float>(const char* key, float value) {
    if (writer.buffer == nullptr || writer.buffer_size == 0)
        return;

    if ((writer.length > 1) &&
        (writer.buffer[writer.length - 1] != '{') &&
        (writer.buffer[writer.length - 1] != '[')) {
        writer.length += snprintf(&writer.buffer[writer.length], writer.buffer_size, ",");
    }

    // Handle float precision and conversion to string
    char fvalue[20];  // Buffer to store the float as a string
    dtostrf(value, 5, 3, fvalue);  // Convert float to string with precision 5.3
    writer.length += snprintf(&writer.buffer[writer.length], writer.buffer_size, "\"%s\":%s", key, fvalue);
}
#endif // ENABLE_DEVFEATURE_MJSON__FLOAT_SPECIALIZATION






//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
// bool mAnimatorLight::requestJSONBufferLock(uint16_t module)
// {
//   unsigned long now = millis();

//   // This assumption here is another http thread must release itself to permit this function to proceed
//   while (jsonBufferLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

//   if (millis()-now >= 1000) {
//     // DEBUG_PRINT(F("ERROR: Locking JSON buffer failed! ("));
//     // DEBUG_PRINT(jsonBufferLock);
//     // DEBUG_PRINTLN(")");
//     return false; // waiting time-outed
//   }

//   jsonBufferLock = module ? module : 255;
//   // DEBUG_PRINT(F("LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOCKED    JSON buffer locked. ("));
//   // DEBUG_PRINT(jsonBufferLock);
//   // DEBUG_PRINTLN(")");
//   #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   // tkr_mfile->gDoc = &doc;  // used for applying presets (presets.cpp)
//   tkr_mfile->pDoc->clear();
//   #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   return true;
// }

#if defined(ESP8266)
  #include <Arduino.h>
  #include <coredecls.h>   // declares can_yield()
#endif



//threading/network callback details: https://github.com/wled-dev/WLED/pull/2336#discussion_r762276994
bool JsonBuilder::requestJSONBufferLock(uint16_t moduleID)
{
  #ifdef USE_MODULE_CORE_FILESYSTEM
  if (tkr_mfile->pDoc == nullptr) {
    DEBUG_PRINTLN(F("ERROR: JSON buffer not allocated!"));
    return false;
  }
  #endif
  
#if defined(ARDUINO_ARCH_ESP32)
  // Use a recursive mutex type in case our task is the one holding the JSON buffer.
  // This can happen during large JSON web transactions.  In this case, we continue immediately
  // and then will return out below if the lock is still held.
  if (xSemaphoreTakeRecursive(tkr_mfile->jsonBufferLockMutex, 250) == pdFALSE) return false;  // timed out waiting
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
    DEBUG_PRINTF_P(PSTR("ERROR: Locking JSON buffer (%d) failed! (still locked by %d)\n"), moduleID, jsonBufferLock);
#ifdef ARDUINO_ARCH_ESP32
    xSemaphoreGiveRecursive(tkr_mfile->jsonBufferLockMutex);
#endif
    return false;
  }

  jsonBufferLock = moduleID ? moduleID : 255;
  DEBUG_PRINTF_P(PSTR("JSON locked (%d)\n\r"), jsonBufferLock);
  #ifdef USE_MODULE_CORE_FILESYSTEM
  tkr_mfile->pDoc->clear();
  #endif
  return true;
}

void  JsonBuilder::releaseJSONBufferLock()
{
  DEBUG_PRINTF_P(PSTR("JSON released (%d)\n\r"), jsonBufferLock);
  jsonBufferLock = 0;
#ifdef ARDUINO_ARCH_ESP32
  xSemaphoreGiveRecursive(tkr_mfile->jsonBufferLockMutex);
#endif  
}




void JsonBuilder::Write(const char* buff)
{
  if(writer.buffer == nullptr) { return; }  
  writer.length += snprintf_P(&writer.buffer[writer.length], writer.buffer_size, buff);
}

void JsonBuilder::Write_P(const char* formatP, ...)
{
  if((writer.buffer == nullptr)) { return; }  
  va_list arg;
  va_start(arg, formatP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}


void JsonBuilder::Start(char* _buffer, uint16_t _length, uint16_t _buffer_size)
{
  PRINT_FLUSHED("JsonBuilder::Start(,,)");
  writer.buffer = _buffer;
  writer.length = _length;
  writer.buffer_size = _buffer_size;
  Start();
}
void JsonBuilder::Start(bool override_lock)
{
  // I should introduce a lock here, to prevent multiple calls with async methods like WLED web
  // It should return false when it is already in use


  // PRINT_FLUSHED("JsonBuilder::Start()");
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) {  
    PRINT_FLUSHED("JsonBuilder::return()");
    Serial.println(writer.buffer == nullptr ? "writer.buffer == nullptr" : "writer.buffer_size != nullptr");
    Serial.println(writer.buffer_size);
    return;
  }  
  
  // PRINT_FLUSHED("memset::start()");
  // memset(writer.buffer,0,writer.buffer_size);
  // PRINT_FLUSHED("memset::end()");
  // Serial.println(writer.buffer_size);
  // Serial.println(DATA_BUFFER_PAYLOAD_MAX_LENGTH); Serial.flush();
  writer.length = 0;
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s","{");

// Serial.println(writer.buffer);
// Serial.println(writer.length);

//   PRINT_FLUSHED("JsonBuilder::Start::end()");
}

// for speed
void JsonBuilder::Start_NoMemClear()
{
  // PRINT_FLUSHED("JsonBuilder::Start()");
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) {  
    //PRINT_FLUSHED("JsonBuilder::return()");
    return;
  }  
  // PRINT_FLUSHED("memset::start()");
  // memset(writer.buffer,0,writer.buffer_size);
  // PRINT_FLUSHED("memset::end()");
  // Serial.println(writer.buffer_size);
  // Serial.println(DATA_BUFFER_PAYLOAD_MAX_LENGTH); Serial.flush();
  writer.length = 0;
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s","{");
  // PRINT_FLUSHED("JsonBuilder::Start::end()");
}
bool JsonBuilder::End()
{
  // Serial.println("JsonBuilder::End()");
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return false; }  
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s","}");
  return strlen(writer.buffer)>3?true:false; //isvalid
}



void JsonBuilder::Append(const char* buff)
{
  if((writer.buffer == nullptr)) { return; }  
  writer.length += snprintf_P(&writer.buffer[writer.length], writer.buffer_size, buff);
}

void JsonBuilder::Append_P(const char* formatP, ...)
{
  if((writer.buffer == nullptr)) { return; }  
  va_list arg;
  va_start(arg, formatP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}


void JsonBuilder::Array_Start(const char* key)
{
    if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
    if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
    writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":[",key);
}

void JsonBuilder::Array_Start_P(const char* keyP, ...)
{

  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }

    if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }

  writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-writer.length, keyP, arg);
  va_end(arg);

  writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\":[");
}



void JsonBuilder::Array_Start() // only add the bracket for manual building
{
    if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
    if(
      ((writer.length>1)&&(writer.buffer[writer.length-1]!='{')) &&  
      ((writer.length>1)&&(writer.buffer[writer.length-1]!='['))     // To permit Array of Arrays
    ){ 
      writer.length += sprintf_P(&writer.buffer[writer.length],","); 
    }
    writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"[");
}

void JsonBuilder::Array_End()
{
    if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
    writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s","]");
}


void JsonBuilder::Object_Start_P(const char* keyP, ...)
{

  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
    
  writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-writer.length, keyP, arg);
  va_end(arg);

  writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\":{");
}

void JsonBuilder::Object_Start_F(const char* keyP, ...)
{

  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
    
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  writer.length += vsnprintf(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-writer.length, keyP, arg);
  va_end(arg);

  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\":{");
}

void JsonBuilder::Object_Start(const char* key)
{
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":{",key);
}

void JsonBuilder::Object_Start()
{
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"{");
}

void JsonBuilder::AddKey(const char* key)
{
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":",key);
}

void JsonBuilder::Object_End()
{
  if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }
  writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s","}");
}


// Use valist to populate the key
void JsonBuilder::Add_P_FV(const char* key, const char* formatP_value, ...) // P = progmem key, FV = format value
{
  // Prefix comma if not first pair
  if((writer.length>1)&&(writer.buffer[writer.length-1]!='{')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  // Write key
  writer.length += sprintf_P(&writer.buffer[writer.length],"\"%S\":",key);
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-writer.length, formatP_value, arg);
  va_end(arg);
}

// Use valist to populate the key
void JsonBuilder::Add_FV(const char* key, const char* formatP_value, ...) // FV = format value
{
  // DEBUG_LINE_HERE;
  // Prefix comma if not first pair
  if(
    (writer.length>1)&&
    (writer.buffer[writer.length-1]!='{')
  ){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  // Write key
  // DEBUG_LINE_HERE;
  // DEBUG_LINE_HERE_VALUE(writer.length);
  // writer.length += snprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, "\"%s\":", key);
  writer.length += sprintf_P(&writer.buffer[writer.length], "\"%s\":", key);
  // Add value
  va_list arg;
  // DEBUG_LINE_HERE;
  va_start(arg, formatP_value);
  // DEBUG_LINE_HERE;
  writer.length += vsnprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP_value, arg);
  // writer.length += vsprintf_P(&writer.buffer[writer.length], formatP_value, arg);
  
  // DEBUG_LINE_HERE;
  va_end(arg);
  // DEBUG_LINE_HERE;
}

// Use valist to populate the key
void JsonBuilder::Add_FV(const char* formatP_value, ...)
{
  // Prefix comma if not first pair
  if(
    (writer.length>1)&&(writer.buffer[writer.length-1]!='{')&&(writer.buffer[writer.length-1]!='[')){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-writer.length, formatP_value, arg);
  va_end(arg);
}


// Use valist to populate the key
void JsonBuilder::AppendBuffer(const char* formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}


// Function to estimate the number of key-value pairs in a JSON buffer
int JsonBuilder::estimateJsonKeyValuePairs(const char* json, size_t len) {
    int keyValuePairs = 0;  // Initialize the key-value pair count
    int nestingLevel = 0;   // Track the nesting level
    bool inString = false;  // Track whether we're inside a string

    for (size_t i = 0; i < len; ++i) {
        char c = json[i];

        // Toggle inString flag when encountering a double quote
        if (c == '"') {
            inString = !inString;
        }

        // If we're inside a string, skip the rest of the loop
        if (inString) continue;

        // Increase nesting level when encountering an opening brace
        if (c == '{') {
            nestingLevel++;
        }

        // Decrease nesting level when encountering a closing brace
        if (c == '}') {
            nestingLevel--;
        }

        // If we encounter a colon at any nesting level, count it as a key-value pair
        if (c == ':' && nestingLevel > 0) {
            keyValuePairs++;
        }
    }

    return keyValuePairs;
}