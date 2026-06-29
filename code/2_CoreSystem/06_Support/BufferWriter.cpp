#include "BufferWriter.h"

BufferWriter* BufferWriter::instance = nullptr;

BufferWriter* BufferWriter::GetInstance()
{
  if(instance == nullptr)
  {
    instance = new BufferWriter();
  }

  return instance;
}

bool BufferWriter::IsReady(void) const
{
  return (writer.buffer != nullptr && writer.buffer_size > 0);
}

bool BufferWriter::IsEmpty(void) const
{
  return (writer.length == 0);
}

bool BufferWriter::IsFull(void) const
{
  if(!IsReady())
  {
    return true;
  }

  return (writer.length >= (writer.buffer_size - 1));
}

uint16_t BufferWriter::Remaining(void) const
{
  if(!IsReady())
  {
    return 0;
  }

  if(writer.length >= writer.buffer_size)
  {
    return 0;
  }

  return writer.buffer_size - writer.length;
}

char* BufferWriter::GetBufferPtr(void)
{
  return writer.buffer;
}

const char* BufferWriter::GetBufferPtr(void) const
{
  return writer.buffer;
}

uint16_t BufferWriter::GetLength(void) const
{
  return writer.length;
}

uint16_t BufferWriter::GetBufferSize(void) const
{
  return writer.buffer_size;
}

void BufferWriter::SetLength(uint16_t length)
{
  if(!IsReady())
  {
    writer.length = 0;
    return;
  }

  if(length >= writer.buffer_size)
  {
    length = writer.buffer_size - 1;
  }

  writer.length = length;
  writer.buffer[writer.length] = '\0';
}

void BufferWriter::Start(char* buffer, uint16_t buffer_size)
{
  Start(buffer, 0, buffer_size);
}

void BufferWriter::Start(char* buffer, uint16_t length, uint16_t buffer_size)
{
  writer.buffer      = buffer;
  writer.buffer_size = buffer_size;
  writer.length      = 0;

  if(!IsReady())
  {
    return;
  }

  if(length >= writer.buffer_size)
  {
    length = writer.buffer_size - 1;
  }

  writer.length = length;
  writer.buffer[writer.length] = '\0';
}

void BufferWriter::Start(void)
{
  Clear();
}

void BufferWriter::Clear(void)
{
  if(!IsReady())
  {
    writer.length = 0;
    return;
  }

  memset(writer.buffer, 0, writer.buffer_size);
  writer.length = 0;
}

bool BufferWriter::End(void)
{
  if(!IsReady())
  {
    return false;
  }

  writer.buffer[writer.length] = '\0';

  return writer.length > 0;
}

void BufferWriter::Append(const char* text)
{
  if(!IsReady() || text == nullptr || IsFull())
  {
    return;
  }

  const uint16_t remaining = Remaining();

  if(remaining <= 1)
  {
    writer.buffer[writer.buffer_size - 1] = '\0';
    return;
  }

  const int written = snprintf(
    &writer.buffer[writer.length],
    remaining,
    "%s",
    text
  );

  if(written <= 0)
  {
    return;
  }

  if((uint16_t)written >= remaining)
  {
    writer.length = writer.buffer_size - 1;
  }
  else
  {
    writer.length += written;
  }

  writer.buffer[writer.length] = '\0';
}

void BufferWriter::Append_P(const char* formatP, ...)
{
  if(!IsReady() || formatP == nullptr || IsFull())
  {
    return;
  }

  const uint16_t remaining = Remaining();

  if(remaining <= 1)
  {
    writer.buffer[writer.buffer_size - 1] = '\0';
    return;
  }

  va_list arg;
  va_start(arg, formatP);

  const int written = vsnprintf_P(
    &writer.buffer[writer.length],
    remaining,
    formatP,
    arg
  );

  va_end(arg);

  if(written <= 0)
  {
    return;
  }

  if((uint16_t)written >= remaining)
  {
    writer.length = writer.buffer_size - 1;
  }
  else
  {
    writer.length += written;
  }

  writer.buffer[writer.length] = '\0';
}