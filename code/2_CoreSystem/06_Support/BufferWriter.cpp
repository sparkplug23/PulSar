#include "BufferWriter.h"

/* Null, because instance will be initialized on demand. */
BufferWriter* BufferWriter::instance = nullptr;

BufferWriter* BufferWriter::GetInstance(){
  if (instance == nullptr){
    instance = new BufferWriter();
  }
  return instance;
}

char* BufferWriter::GetBufferPtr(){
  return writer.buffer;
}
uint16_t BufferWriter::GetLength(){
  return strlen(writer.buffer);
  // return *writer.length; // needs testing, was not working
}

// // used in phd to force json and writer to work together
// void BufferWriter::SetLength(uint16_t len){
//   rwriter.length = len;
//   // return *writer.length; // needs testing, was not working
// }


uint16_t BufferWriter::GetBufferSize(){
  return writer.buffer_size;
}

void BufferWriter::Start(char* _buffer, uint16_t _length, uint16_t _buffer_size)
{
  writer.buffer = _buffer;
  writer.length = _length;
  writer.buffer_size = _buffer_size;
  Serial.printf("DEBUG HERE: "); Serial.print(__FILE__); Serial.println(__LINE__); Serial.flush();
  Start();
  Serial.printf("DEBUG HERE: "); Serial.print(__FILE__); Serial.println(__LINE__); Serial.flush();
}
void BufferWriter::Clear()
{
  Serial.printf("DEBUG HERE: "); Serial.print(__FILE__); Serial.println(__LINE__); Serial.flush();
    if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }  
    // memset(writer.buffer,0,writer.buffer_size);
    writer.length = 0;
  Serial.printf("DEBUG HERE: "); Serial.print(__FILE__); Serial.println(__LINE__); Serial.flush();
}
// void BufferWriter::Start()
// {
//     if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return; }  

// // Serial.println(DATA_BUFFER_PAYLOAD_MAX_LENGTH);
// Serial.println(writer.buffer_size);


//     // memset(writer.buffer,0,writer.buffer_size);
//     Clear();
//     writer.length = 0;
// }



void BufferWriter::Start()
{
  // If writer is a global/static, this is safe.
  // If it's a pointer/singleton, ensure it is valid before calling Start().




  // Serial.printf("BW Start: payload=%p sizeof(topic)=%u\n",
  //               (void*)writer.buffer,
  //               (unsigned)sizeof(writer.buffer),

  // // Clear via the real fixed buffers, not via any pointer/size pair
  // memset(writer.buffer,   0, writer.buffer_size);

  // writer.topic.length_used   = 0;
  // writer.payload.length_used = 0;
  writer.length = 0; // if you have a separate aggregate length
}




bool BufferWriter::End()
{
    if((writer.buffer == nullptr)||(writer.buffer_size == 0)) { return false; }  
    return strlen(writer.buffer)?true:false; //isvalid
}

void BufferWriter::Append(const char* buff)
{
    if(writer.buffer == nullptr) { return; }  
    writer.length += snprintf_P(&writer.buffer[writer.length], writer.buffer_size, buff);
}

void BufferWriter::Append_P(const char* formatP, ...)
{
  if(writer.buffer == nullptr) { return; }  
  va_list arg;
  va_start(arg, formatP);
  writer.length += vsnprintf_P(&writer.buffer[writer.length], writer.buffer_size, formatP, arg);
  va_end(arg);
}



