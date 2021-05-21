#include "BufferWriter.h"

/* Null, because instance will be initialized on demand. */
BufferWriter* BufferWriter::instance = nullptr;

BufferWriter* BufferWriter::GetInstance(){
  if (instance == nullptr){
    instance = new BufferWriter();
  }
  return instance;
}

char* BufferWriter::GetPtr(){
  return writer.buffer;
}
uint16_t BufferWriter::GetLength(){
  return strlen(writer.buffer);
  // return *writer.length; // needs testing, was not working
}
uint16_t* BufferWriter::GetLengthPtr(){
  return writer.length;
}
uint16_t BufferWriter::GetBufferSize(){
  return writer.buffer_size;
}

void BufferWriter::Start(char* _buffer, uint16_t* _length, uint16_t _buffer_size)
{
  writer.buffer = _buffer;
  writer.length = _length;
  writer.buffer_size = _buffer_size;
  Start();
}
void BufferWriter::Clear()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }  
    memset(writer.buffer,0,writer.buffer_size);
    *writer.length = 0;
}
void BufferWriter::Start()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return; }  
    memset(writer.buffer,0,writer.buffer_size);
    *writer.length = 0;
}
bool BufferWriter::End()
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)||(writer.buffer_size == 0)) { return false; }  
    return strlen(writer.buffer)?true:false; //isvalid
}

void BufferWriter::Append(const char* buff)
{
    if((writer.buffer == nullptr)||(writer.length == nullptr)) { return; }  
    *writer.length += snprintf_P(&writer.buffer[*writer.length], writer.buffer_size, buff);
}

void BufferWriter::Append_P(const char* formatP, ...)
{
  if((writer.buffer == nullptr)||(writer.length == nullptr)) { return; }  
  va_list arg;
  va_start(arg, formatP);
  *writer.length += vsnprintf_P(&writer.buffer[*writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}



