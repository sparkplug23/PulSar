

#ifndef _MDEVICENAMELIST_H
#define _MDEVICENAMELIST_H

#include <stdint.h>
#include <Arduino.h>

#include "2_CoreSystem/Logging/mLogging.h"

/**
 * @brief Memory is still held in settings, but internal pointers are used to write and read from
 * */
class DeviceNameList{
    
  private:
    /* Prevent others from being created */
    DeviceNameList(DeviceNameList const& other) = delete;
    DeviceNameList(DeviceNameList&& other) = delete;
    /* Private constructor to prevent instancing. */
    DeviceNameList(){};
    
    struct NAMEBUFFER{
      char* ptr;
      uint16_t length = 0;
    }name_buffer;

    struct INDEXES{
      /**
       * @note also known as class or module ids
       * */
      int16_t* unique_group_ids;
      /**
       * @note index also known as device_id, ie index within the group
       * */
      int8_t*   index_ids;
      /**
       * shared element count for both
       * */
      uint8_t   length = 0;
    }number_buffer;

  public:
    // External function to get instance
    static DeviceNameList* GetInstance();
    /* Here will be the instance stored. */
    static DeviceNameList* instance;

    void Init_NameBuffer(char* buffer, uint16_t size)
    {
      name_buffer.ptr = buffer;
      name_buffer.length = size;
    }

    void Init_NummberBuffer(int16_t* group_ids, int8_t* index_ids, uint8_t length)
    {
      number_buffer.unique_group_ids = group_ids;
      number_buffer.index_ids = index_ids;
      number_buffer.length = length;
    }

    void ClearBuffers()
    {
      memset(name_buffer.ptr,                 0, name_buffer.length); //char size
      memset(number_buffer.unique_group_ids, -1, number_buffer.length*sizeof(int16_t));
      memset(number_buffer.index_ids,        -1, number_buffer.length*sizeof(int8_t));
    }

    /**
     * @brief init the internal buffer pointers
     * @note test
     * @param name_buffer_ptr
     * @param name_buffer_length 
     * @param number_buffer_unique_group_ids 
     * @param number_buffer_index_ids 
     * @param number_buffer_length 
     * */
    void Init(char* name_buffer_ptr, uint16_t name_buffer_length, int16_t* number_buffer_unique_group_ids, int8_t* number_buffer_index_ids, uint8_t number_buffer_length)
    {
      Init_NameBuffer(name_buffer_ptr, name_buffer_length);
      Init_NummberBuffer(number_buffer_unique_group_ids, number_buffer_index_ids, number_buffer_length);
      ClearBuffers();
    }

    uint16_t GetNameBuffer_Length()
    {
      return strlen(name_buffer.ptr);
    }

    int8_t AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);
    int8_t RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);
    const char* GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size);
    int8_t GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind);
    int16_t GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id = nullptr);
    int16_t GetDeviceIDbyName(const char* name_tofind, int8_t device_id = -1, int8_t class_id = -1);

    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
    int8_t GetDeviceNameCount(int16_t class_id);
    uint16_t CountCharInCtr(const char* tosearch, char tofind);
        

};

#define DeviceNameListI DeviceNameList::GetInstance()
#define DLI DeviceNameListI


#endif // _MDEVICENAMELIST_H