#include "mJSON.h"


char*     json_buffer = nullptr;
uint16_t* json_length = nullptr;
uint16_t  json_buffer_size = 0;


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


// Start by giving buffer location to write to
void JsonBuilder_Start(char* _buffer, uint16_t* _length, uint16_t _buffer_size)
{
  json_buffer = _buffer;
  json_length = _length;
  json_buffer_size = _buffer_size;
  JsonBuilder_Start();
}
// Start by giving buffer location to write to
// Assume it has been set, check this!
void JsonBuilder_Start()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }  
    memset(json_buffer,0,json_buffer_size);//*sizeof(uint16_t));
    *json_length = 0;
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","{");
}
bool JsonBuilder_End()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return false; }  
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","}");
    return *json_length>3?true:false; //isvalid
}


void JsonBuilder_Level_Start_P(const char* keyP, ...)
{

  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    
  *json_length += snprintf_P(&json_buffer[*json_length],json_buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  *json_length += vsnprintf_P(&json_buffer[*json_length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*json_length, keyP, arg);
  va_end(arg);

  *json_length += snprintf_P(&json_buffer[*json_length],json_buffer_size,"\":{");
}

void JsonBuilder_Level_Start(const char* key)
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":{",key);
}

void JsonBuilder_Level_Start()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"{");
}

void JsonBuilder_AddKey(const char* key)
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":",key);
}

void JsonBuilder_Level_End()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","}");
}

void JsonBuilder_Array_Start(const char* key)
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":[",key);
}
void JsonBuilder_Array_Start() // only add the bracket for manual building
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"[");
}
void JsonBuilder_Array_End()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","]");
}




// Use valist to populate the key
void JsonBuilder_Add_FP(const char* key, const char* formatP_value, ...)
{
  char* buff = data_buffer2.payload.ctr;
  uint16_t* len = &data_buffer2.payload.len;
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
void JsonBuilder_Add_FP(const char* formatP_value, ...)
{
  // Prefix comma if not first pair
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  *json_length += vsnprintf_P(&json_buffer[*json_length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*json_length, formatP_value, arg);
  va_end(arg);
}


// Use valist to populate the key
void JsonBuilder_AppendBuffer(const char* formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  *json_length += vsnprintf_P(&json_buffer[*json_length], json_buffer_size, formatP, arg);
  va_end(arg);
}




/* Null, because instance will be initialized on demand. */
JsonBuilder* JsonBuilder::instance = nullptr;

JsonBuilder* JsonBuilder::GetInstance(){
  if (instance == nullptr){
    instance = new JsonBuilder();
  }
  return instance;
}

void JsonBuilder::Start(char* _buffer, uint16_t* _length, uint16_t _buffer_size)
{
  writer.buffer = _buffer;
  writer.length = _length;
  writer.buffer_size = _buffer_size;
  Start();
}
void JsonBuilder::Start()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }  
    memset(writer.buffer,0,writer.buffer_size);
    *writer.length = 0;

    
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","{");
}
bool JsonBuilder::End()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return false; }  
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","}");
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

// template <typename T>
// void JsonBuilder::Add(const char* key, T value)
// {
//   if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0))
//     return;
  
//   if((*json_length>1)&&(json_buffer[*json_length-1]!='{')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }

//   #ifdef DEBUG_JSON_BUILDER
//   char buffer_id[50];
//   uint8_t id = getIdentifierID4(value);
//   GetIndentifierNameByID(id, buffer_id);
//   Serial.printf("%s id=%d %s \n\t", key, id, buffer_id);
//   #endif
  
//   if(is_number_type<T>::value){ 
//     *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":%d",key,value);
//   }else
//   if(is_string_type<T>::value){ 
//     *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":\"%s\"",key,value);
//   }else
//   if(is_char_type<T>::value){   
//     *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":'%c'",key,value);
//   }else
//   if(is_float_type<T>::value){ 
//     float f = 0;     memcpy(&f,&value,sizeof(f));
//     char fvalue[20]; dtostrfd2(f,3,fvalue);
//     *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":%s",key,fvalue);
//   }

//   // Serial.printf("TEST TIME = %s\n\r",json_buffer[0]);

// // }

// #ifdef TEMPLATE_JSON

// template <typename T>
// void JsonBuilder::Add_P(const char* key, T value)



// template <typename T, typename U>
// void JsonBuilder::Array_AddValue(T* value_arr, U value_arr_len)


// template <typename T, typename U>
// void JsonBuilder::Array_AddArray(const char* key, T* value_arr, U value_arr_len)


// #endif


void JsonBuilder::Array_Start(const char* key)
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":[",key);
}
void JsonBuilder::Array_Start() // only add the bracket for manual building
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    if((*json_length>1)&&(json_buffer[*json_length-1]!='{')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"[");
}
void JsonBuilder::Array_End()
{
    if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
    *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","]");
}





void JsonBuilder::Level_Start_P(const char* keyP, ...)
{

  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
    
  *json_length += snprintf_P(&json_buffer[*json_length],json_buffer_size,"\"");
  
  va_list arg;
  va_start(arg, keyP);
  *json_length += vsnprintf_P(&json_buffer[*json_length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*json_length, keyP, arg);
  va_end(arg);

  *json_length += snprintf_P(&json_buffer[*json_length],json_buffer_size,"\":{");
}

void JsonBuilder::Level_Start(const char* key)
{
  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
  *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":{",key);
}

void JsonBuilder::Level_Start()
{
  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
  *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"{");
}

void JsonBuilder::AddKey(const char* key)
{
  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
  *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"\"%s\":",key);
}

void JsonBuilder::Level_End()
{
  if((json_buffer == nullptr)||(json_length == nullptr)||(json_buffer_size == 0)) { return; }
  *json_length += snprintf(&json_buffer[*json_length],json_buffer_size,"%s","}");
}


// Use valist to populate the key
void JsonBuilder::Add_FP(const char* key, const char* formatP_value, ...)
{
  char* buff = data_buffer2.payload.ctr;
  uint16_t* len = &data_buffer2.payload.len;
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
void JsonBuilder::Add_FP(const char* formatP_value, ...)
{
  // Prefix comma if not first pair
  if((*json_length>1)&&(json_buffer[*json_length-1]!='{')&&(json_buffer[*json_length-1]!='[')){ *json_length += sprintf_P(&json_buffer[*json_length],","); }
  // Add value
  va_list arg;
  va_start(arg, formatP_value);
  *json_length += vsnprintf_P(&json_buffer[*json_length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-*json_length, formatP_value, arg);
  va_end(arg);
}


// Use valist to populate the key
void JsonBuilder::AppendBuffer(const char* formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  *json_length += vsnprintf_P(&json_buffer[*json_length], json_buffer_size, formatP, arg);
  va_end(arg);
}
