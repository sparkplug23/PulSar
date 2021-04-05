#include "mJSON.h"


char* dtostrfd2(double number, unsigned char prec, char *s)
{
  if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
    strcpy(s, "null");
    return s;
  } else {
    return dtostrf(number, 1, prec, s);
  }
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
uint16_t JsonBuilder::GetLength(){
  return *writer.length;
}
uint16_t* JsonBuilder::GetLengthPtr(){
  return writer.length;
}
uint16_t JsonBuilder::GetBufferSize(){
  return writer.buffer_size;
}

void JsonBuilder::Start(char* _buffer, uint16_t* _length, uint16_t _buffer_size)
{
  // PRINT_FLUSHED("JsonBuilder::Start(,,)");
  writer.buffer = _buffer;
  writer.length = _length;
  writer.buffer_size = _buffer_size;
  Start();
}
void JsonBuilder::Start()
{
  // PRINT_FLUSHED("JsonBuilder::Start()");
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) {  
    PRINT_FLUSHED("JsonBuilder::return()");
    return;
  }  
  // PRINT_FLUSHED("memset::start()");
  memset(writer.buffer,0,writer.buffer_size);
  // PRINT_FLUSHED("memset::end()");
  // Serial.println(writer.buffer_size);
  // Serial.println(DATA_BUFFER_PAYLOAD_MAX_LENGTH); Serial.flush();
  *writer.length = 0;
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"%s","{");
  // PRINT_FLUSHED("JsonBuilder::Start::end()");
}
bool JsonBuilder::End()
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return false; }  
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"%s","}");
  return strlen(writer.buffer)>3?true:false; //isvalid
}

void JsonBuilder::Append(const char* buff)
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)) { return; }  
  *writer.length += snprintf_P(&writer.buffer[*writer.length], writer.buffer_size, buff);
}

void JsonBuilder::Append_P(const char* formatP, ...)
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)) { return; }  
  va_list arg;
  va_start(arg, formatP);
  *writer.length += vsnprintf_P(&writer.buffer[*writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}


void JsonBuilder::Array_Start(const char* key)
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
    if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
    *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"\"%s\":[",key);
}
void JsonBuilder::Array_Start_P(const char* key)
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
    if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
    *writer.length += snprintf_P(&writer.buffer[*writer.length],writer.buffer_size,"\"%S\":[",key);
}
void JsonBuilder::Array_Start() // only add the bracket for manual building
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
    if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
    *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"[");
}
void JsonBuilder::Array_End()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
    *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"%s","]");
}


void JsonBuilder::Level_Start_P(const char* keyP, ...)
{

  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
  if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')&&(writer.buffer[*writer.length-1]!='[')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
    
  *writer.length += snprintf_P(&writer.buffer[*writer.length],writer.buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  *writer.length += vsnprintf_P(&writer.buffer[*writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*writer.length, keyP, arg);
  va_end(arg);

  *writer.length += snprintf_P(&writer.buffer[*writer.length],writer.buffer_size,"\":{");
}

void JsonBuilder::Level_Start(const char* key)
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
  if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')&&(writer.buffer[*writer.length-1]!='[')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"\"%s\":{",key);
}

void JsonBuilder::Level_Start()
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
  if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')&&(writer.buffer[*writer.length-1]!='[')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"{");
}

void JsonBuilder::AddKey(const char* key)
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
  if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')&&(writer.buffer[*writer.length-1]!='[')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"\"%s\":",key);
}

void JsonBuilder::Level_End()
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }
  *writer.length += snprintf(&writer.buffer[*writer.length],writer.buffer_size,"%s","}");
}


// Use valist to populate the key
void JsonBuilder::Add_P_FV(const char* key, const char* formatP_value, ...) // P = progmem key, FV = format value
{
  char* buff = data_buffer.payload.ctr;
  uint16_t* len = &data_buffer.payload.len;
  // Prefix comma if not first pair
  if((*len>1)&&(buff[*len-1]!='{')){ *len += sprintf_P(&buff[*len],","); }
  // Write key
  *len += sprintf_P(&buff[*len],"\"%S\":",key);
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  *len += vsnprintf_P(&buff[*len], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*len, formatP_value, arg);
  va_end(arg);
}

// Use valist to populate the key
void JsonBuilder::Add_FV(const char* key, const char* formatP_value, ...) // FV = format value
{
  char* buff = data_buffer.payload.ctr;
  uint16_t* len = &data_buffer.payload.len;
  // Prefix comma if not first pair
  if((*len>1)&&(buff[*len-1]!='{')){ *len += sprintf_P(&buff[*len],","); }
  // Write key
  *len += sprintf_P(&buff[*len],"\"%s\":",key);
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  *len += vsnprintf_P(&buff[*len], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*len, formatP_value, arg);
  va_end(arg);
}

// Use valist to populate the key
void JsonBuilder::Add_FV(const char* formatP_value, ...)
{
  // Prefix comma if not first pair
  if((*writer.length>1)&&(writer.buffer[*writer.length-1]!='{')&&(writer.buffer[*writer.length-1]!='[')){ *writer.length += sprintf_P(&writer.buffer[*writer.length],","); }
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  *writer.length += vsnprintf_P(&writer.buffer[*writer.length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*writer.length, formatP_value, arg);
  va_end(arg);
}


// Use valist to populate the key
void JsonBuilder::AppendBuffer(const char* formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  *writer.length += vsnprintf_P(&writer.buffer[*writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}
