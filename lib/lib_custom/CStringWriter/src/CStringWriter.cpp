#include "CStringWriter.h"

/* Null, because instance will be initialized on demand. */
// CStringWriter* CStringWriter::instance = nullptr;

// CStringWriter* CStringWriter::GetInstance(){
//   if (instance == nullptr){
//     instance = new CStringWriter();
//   }
//   return instance;
// }

char* CStringWriter::GetBufferPtr(){
  return m_data;
}
// uint16_t CStringWriter::GetLength(){
//   return strlen(m_data);
//   // return dataLen; // needs testing, was not working
// }

// // used in phd to force json and writer to work together
// void CStringWriter::SetLength(uint16_t len){
//   rwriter.length = len;
//   // return dataLen; // needs testing, was not working
// }


// uint16_t CStringWriter::GetLength()
// {
//   return dataLen;
// }
// uint16_t CStringWriter::GetBufferSize(){
//   return dataLen;
// }

void CStringWriter::Start(uint16_t _length)
{
  dataLen = _length;
  deallocateData();
  allocateData(_length);

}
// Does not erase, just clear the contents
void CStringWriter::Clear()
{
    if((m_data == nullptr)||(dataLen == 0)) { return; }  
    memset(m_data,0,dataLen);
    dataLen = 0;
}

bool CStringWriter::End()
{
    if((m_data == nullptr)||(dataLen == 0)) { return false; }  
    return strlen(m_data)?true:false; //isvalid
}

void CStringWriter::Append(const char* buff)
{
    if(m_data == nullptr) { return; }  
    usedLen += snprintf_P(&m_data[usedLen], dataLen, buff);
}

void CStringWriter::Append_P(const char* formatP, ...)
{
  if(m_data == nullptr) { return; }  
  va_list arg;
  va_start(arg, formatP);
  usedLen += vsnprintf_P(&m_data[usedLen], dataLen, formatP, arg);
  va_end(arg);
}



