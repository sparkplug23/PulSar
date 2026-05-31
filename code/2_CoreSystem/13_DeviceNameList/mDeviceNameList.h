#ifndef _MDEVICENAMELIST_H
#define _MDEVICENAMELIST_H

#include <stdint.h>
#include <Arduino.h>

#include "2_CoreSystem/08_Logging/mLogging.h"

/**
 * @brief Runtime helper for the device-name list.
 *
 * Storage is still owned externally, usually in Settings. This class only keeps
 * pointers to the external name buffer and index arrays.
 */
class DeviceNameList
{
  private:
    DeviceNameList();
    DeviceNameList(DeviceNameList const& other) = delete;
    DeviceNameList(DeviceNameList&& other) = delete;

    struct NAMEBUFFER
    {
      char*    ptr    = nullptr;
      uint16_t length = 0;
    } name_buffer;

    struct INDEXES
    {
      /**
       * @note Also known as class IDs or module unique IDs.
       */
      uint16_t* unique_group_ids = nullptr;

      /**
       * @note Also known as device IDs, index within the module/group.
       */
      uint8_t* index_ids = nullptr;

      /**
       * Shared maximum element count for both index arrays.
       */
      uint8_t length = 0;

      /**
       * Number of valid index entries currently registered.
       */
      uint8_t currently_used_indexed = 0;
    } number_buffer;

    bool BuffersReady() const;
    bool GetNameAtIndex(uint16_t index, char* destination, size_t destination_size);

  public:
    static DeviceNameList* GetInstance();
    static DeviceNameList* instance;

    void Init_NameBuffer(char* buffer, uint16_t size);
    void Init_NumberBuffer(uint16_t* group_ids, uint8_t* index_ids, uint8_t length);

    /**
     * @brief Kept for old call-sites with existing typo.
     */
    void Init_NummberBuffer(uint16_t* group_ids, uint8_t* index_ids, uint8_t length)
    {
      Init_NumberBuffer(group_ids, index_ids, length);
    }

    void Init(
      char* name_buffer_ptr,
      uint16_t name_buffer_length,
      uint16_t* number_buffer_unique_group_ids,
      uint8_t* number_buffer_index_ids,
      uint8_t number_buffer_length
    );

    void ClearBuffers();

    uint8_t GetLengthIndexMax() const;
    uint8_t GetLengthIndexUsed() const;
    uint16_t GetNameBuffer_Length() const;
    uint16_t GetNameBufferItemCount();

    char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack);

    int8_t AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);
    int8_t RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);

    bool GetModuleAndSensorIDs(const char* module_name, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id);
    bool GetModuleAndSensorIDs(uint16_t module_id, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id);

    const char* GetDeviceName_WithModuleUniqueID(
      int16_t unique_module_id,
      int8_t device_id,
      char* buffer,
      uint16_t buffer_size,
      bool flag_respond_nomatch_if_not_found = false
    );

    int8_t GetDeviceIDbyName(int16_t* class_id, int8_t* device_id, char* name_tofind);
    int16_t GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int16_t* class_id = nullptr);
    int16_t GetDeviceIDbyName(const char* name_tofind, int16_t class_id = -1);

    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
    int8_t GetDeviceNameCount(int16_t class_id);
    uint16_t CountCharInCtr(const char* tosearch, char tofind);
};

#define DeviceNameListI DeviceNameList::GetInstance()
#define DLI DeviceNameListI

#endif