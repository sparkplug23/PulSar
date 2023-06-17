#ifndef _CSTRINGWRITER_H_
#define _CSTRINGWRITER_H_

#include <stdint.h>
#include <Arduino.h>

// I want to have a non singleton version so I can use these inside functions nicely with buffer that only lasts inside function


class CStringWriter{
    // 
  private:
  
    
    // struct WRITER_POINTERS{
    char*     m_data = nullptr;
    // uint16_t  buffer_size = 0;
    // uint16_t* length = nullptr;
    uint16_t usedLen = 0;
    uint16_t dataLen = 0;
    // }writer;

  public:
  
    CStringWriter(uint16_t len)
    {
      allocateData(len);
      

      
    };

    ~CStringWriter()
    {
      deallocateData();
    }

    bool allocateData(uint16_t len)
    {

      Serial.println("allocateData");

      if (m_data && dataLen == len) return true; //already allocated

      m_data = (char*) malloc(len);

      if (!m_data)
      {
        return false; //allocation failed
      }

      dataLen = len;
      memset(m_data, 0, len);
      
      Serial.printf("allocateData%d \n\r", dataLen);

      return true;

    }
  
    void deallocateData() 
    {
      if (!m_data) return;
      free(m_data);
      m_data = nullptr;
      dataLen = 0;
    }


    void Start(uint16_t _length); // Can be used to resize, but includes deleting previous
    
    void Clear();
    bool End();
    char* GetBufferPtr();
    // uint16_t* GetLengthPtr();
    // uint16_t GetLength();
    // uint16_t GetBufferSize();
    // void SetLength(uint16_t len);

    //FUTURE, create iterator that gets values

    char* data(){
        return m_data;
    };
    uint16_t length(){
        return usedLen;
    };

    void Append(const char* buff);
    void Append_P(const char* formatP, ...);

};

#endif