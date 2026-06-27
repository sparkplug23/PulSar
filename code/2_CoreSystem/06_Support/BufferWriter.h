#ifndef _BUFFERWRITER_H_
#define _BUFFERWRITER_H_

#include <stdint.h>
#include <Arduino.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Small bounded char-buffer writer.
 *
 * Can be used either as:
 *   - singleton via BufferWriterI / BWI
 *   - normal local object inside a function
 */
class BufferWriter
{
  private:
    BufferWriter(BufferWriter const& other) = delete;
    BufferWriter(BufferWriter&& other) = delete;
    BufferWriter& operator=(BufferWriter const& other) = delete;
    BufferWriter& operator=(BufferWriter&& other) = delete;

    struct WRITER_POINTERS
    {
      char*    buffer      = nullptr;
      uint16_t buffer_size = 0;
      uint16_t length      = 0;
    } writer;

    uint16_t Remaining(void) const;

  public:
    BufferWriter() = default;

    static BufferWriter* GetInstance();
    static BufferWriter* instance;

    void Start(char* buffer, uint16_t buffer_size);
    void Start(char* buffer, uint16_t length, uint16_t buffer_size);
    void Start(void);

    void Clear(void);
    bool End(void);

    bool IsReady(void) const;
    bool IsEmpty(void) const;
    bool IsFull(void) const;

    char* GetBufferPtr(void);
    const char* GetBufferPtr(void) const;

    uint16_t GetLength(void) const;
    uint16_t GetBufferSize(void) const;

    void SetLength(uint16_t length);

    void Append(const char* text);
    void Append_P(const char* formatP, ...);
};

#define BufferWriterI BufferWriter::GetInstance()
#define BWI BufferWriter::GetInstance()

#endif