#ifndef _BUFFERWRITER_H_
#define _BUFFERWRITER_H_

#include <stdint.h>
#include <Arduino.h>

class BufferWriter{
    
  private:
    /* Prevent others from being created */
    BufferWriter(BufferWriter const& other) = delete;
    BufferWriter(BufferWriter&& other) = delete;
    /* Private constructor to prevent instancing. */
    BufferWriter(){};
    
    struct WRITER_POINTERS{
        char*     buffer = nullptr;
        uint16_t  buffer_size = 0;
        uint16_t* length = nullptr;
    }writer;

  public:
    // External function to get instance
    static BufferWriter* GetInstance();
    /* Here will be the instance stored. */
    static BufferWriter* instance;    

    void Start(char* _buffer, uint16_t* _length, uint16_t _buffer_size);
    void Start();
    void Clear();
    bool End();
    char* GetPtr();
    uint16_t* GetLengthPtr();
    uint16_t GetLength();
    uint16_t GetBufferSize();
    // void SetLength(uint16_t len);

    //FUTURE, create iterator that gets values

    template <typename T>
    T GetLength(){
        return writer.length;
    };

    void Append(const char* buff);
    void Append_P(const char* formatP, ...);

};

#define BufferWriterI BufferWriter::GetInstance()

#endif