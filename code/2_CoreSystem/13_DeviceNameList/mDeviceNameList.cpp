#include "mDeviceNameList.h"

DeviceNameList* DeviceNameList::instance = nullptr;

DeviceNameList::DeviceNameList()
{
}

// DeviceNameList* DeviceNameList::GetInstance()
// {
//   if(instance == nullptr)
//   {
//     instance = new DeviceNameList();
//   }

//   return instance;
// }

DeviceNameList* DeviceNameList::GetInstance()
{
  ALOG_INF(PSTR("DLI GetInstance enter"));

  if(instance == nullptr)
  {
    ALOG_INF(PSTR("DLI instance null, before new"));
    instance = new DeviceNameList();
    ALOG_INF(PSTR("DLI instance after new = %p"), instance);
  }

  ALOG_INF(PSTR("DLI GetInstance return = %p"), instance);
  return instance;
}


bool DeviceNameList::BuffersReady() const
{
  return
    (name_buffer.ptr != nullptr) &&
    (name_buffer.length > 0) &&
    (number_buffer.unique_group_ids != nullptr) &&
    (number_buffer.index_ids != nullptr) &&
    (number_buffer.length > 0);
}

void DeviceNameList::Init_NameBuffer(char* buffer, uint16_t size)
{
  name_buffer.ptr    = buffer;
  name_buffer.length = size;
}

void DeviceNameList::Init_NumberBuffer(uint16_t* group_ids, uint8_t* index_ids, uint8_t length)
{
  number_buffer.unique_group_ids        = group_ids;
  number_buffer.index_ids               = index_ids;
  number_buffer.length                  = length;
  number_buffer.currently_used_indexed  = 0;
}

void DeviceNameList::Init(
  char* name_buffer_ptr,
  uint16_t name_buffer_length,
  uint16_t* number_buffer_unique_group_ids,
  uint8_t* number_buffer_index_ids,
  uint8_t number_buffer_length
)
{
  Init_NameBuffer(name_buffer_ptr, name_buffer_length);
  Init_NumberBuffer(number_buffer_unique_group_ids, number_buffer_index_ids, number_buffer_length);
  ClearBuffers();
}

void DeviceNameList::ClearBuffers()
{
  if(name_buffer.ptr != nullptr && name_buffer.length > 0)
  {
    memset(name_buffer.ptr, 0, name_buffer.length);
  }

  if(number_buffer.unique_group_ids != nullptr && number_buffer.length > 0)
  {
    memset(number_buffer.unique_group_ids, D_MAX_UINT16, number_buffer.length * sizeof(uint16_t));
  }

  if(number_buffer.index_ids != nullptr && number_buffer.length > 0)
  {
    memset(number_buffer.index_ids, D_MAX_UINT8, number_buffer.length * sizeof(uint8_t));
  }

  number_buffer.currently_used_indexed = 0;
}

uint8_t DeviceNameList::GetLengthIndexMax() const
{
  return number_buffer.length;
}

uint8_t DeviceNameList::GetLengthIndexUsed() const
{
  return number_buffer.currently_used_indexed;
}

uint16_t DeviceNameList::GetNameBuffer_Length() const
{
  if(name_buffer.ptr == nullptr)
  {
    return 0;
  }

  return strlen(name_buffer.ptr);
}

uint16_t DeviceNameList::GetNameBufferItemCount()
{
  if(name_buffer.ptr == nullptr)
  {
    return 0;
  }

  if(name_buffer.ptr[0] == '\0')
  {
    return 0;
  }

  return CountCharInCtr(name_buffer.ptr, '|');
}

bool DeviceNameList::GetNameAtIndex(uint16_t index, char* destination, size_t destination_size)
{
  if(destination == nullptr || destination_size == 0)
  {
    return false;
  }

  destination[0] = '\0';

  if(name_buffer.ptr == nullptr)
  {
    return false;
  }

  GetTextIndexed(destination, destination_size, index, name_buffer.ptr);

  return destination[0] != '\0';
}

char* DeviceNameList::GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  if(destination == nullptr || destination_size == 0)
  {
    return destination;
  }

  destination[0] = '\0';

  if(haystack == nullptr)
  {
    return destination;
  }

  const char* read = haystack;

  for(uint16_t current_index = 0; current_index <= index; current_index++)
  {
    size_t write_index = 0;

    while(*read != '\0' && *read != '|')
    {
      if(current_index == index && write_index < (destination_size - 1))
      {
        destination[write_index++] = *read;
      }

      read++;
    }

    if(current_index == index)
    {
      destination[write_index] = '\0';
      return destination;
    }

    if(*read == '|')
    {
      read++;
    }
    else
    {
      break;
    }
  }

  destination[0] = '\0';
  return destination;
}

int8_t DeviceNameList::AddDeviceName(const char* name_ctr, int16_t unique_module_id, int8_t device_id)
{
  if(name_ctr == nullptr)
  {
    ALOG_ERR(PSTR("DeviceNameList::AddDeviceName null name"));
    return 0;
  }

  if(!BuffersReady())
  {
    ALOG_ERR(PSTR("DeviceNameList::AddDeviceName buffers not ready"));
    return 0;
  }

  if(name_ctr[0] == '\0')
  {
    ALOG_ERR(PSTR("DeviceNameList::AddDeviceName empty name"));
    return 0;
  }

  /**
   * If the runtime index is empty but the text buffer still has content,
   * the persisted settings buffer and runtime index state are desynchronised.
   */
  if(number_buffer.currently_used_indexed == 0 && name_buffer.ptr[0] != '\0')
  {
    ALOG_ERR(
      PSTR("DeviceNameList stale buffer detected, clearing before rebuild len=%d"),
      strlen(name_buffer.ptr)
    );

    ClearBuffers();
  }

  char existing_name[64] = {0};

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    GetNameAtIndex(i, existing_name, sizeof(existing_name));

    const bool same_module =
      (number_buffer.unique_group_ids[i] == (uint16_t)unique_module_id);

    const bool same_device =
      (number_buffer.index_ids[i] == (uint8_t)device_id);

    const bool same_id =
      same_module && same_device;

    const bool same_name =
      (strcmp(existing_name, name_ctr) == 0);

    /**
     * Exact duplicate:
     * Same module/class, same device index, same name.
     * This is allowed and ignored, so repeated parsing is idempotent.
     */
    if(same_id && same_name)
    {
      ALOG_DBM(
        PSTR("DeviceNameList duplicate ignored [%d] class=%d device=%d name=%s"),
        i,
        unique_module_id,
        device_id,
        name_ctr
      );

      return 1;
    }

    /**
     * ID conflict:
     * Same module/class and same device index, but different name.
     * This is not allowed.
     */
    if(same_id && !same_name)
    {
      ALOG_ERR(
        PSTR("DeviceNameList ID conflict [%d] class=%d device=%d existing=%s new=%s"),
        i,
        unique_module_id,
        device_id,
        existing_name,
        name_ctr
      );

      return 0;
    }

    /**
     * Name conflict:
     * Same module/class reuses the same name for a different device index.
     * Different modules/classes are allowed to reuse the same visible name.
     */
    if(same_module && same_name && !same_device)
    {
      ALOG_ERR(
        PSTR("DeviceNameList name conflict inside class [%d] class=%d name=%s existing_device=%d new_device=%d"),
        i,
        unique_module_id,
        name_ctr,
        number_buffer.index_ids[i],
        device_id
      );

      return 0;
    }
  }

  const uint8_t index = number_buffer.currently_used_indexed;

  if(index >= number_buffer.length)
  {
    ALOG_ERR(
      PSTR("DeviceNameList index full, cannot add class=%d device=%d name=%s used=%d max=%d"),
      unique_module_id,
      device_id,
      name_ctr,
      number_buffer.currently_used_indexed,
      number_buffer.length
    );

    return 0;
  }

  const uint16_t buffer_length = strlen(name_buffer.ptr);
  const uint16_t name_length   = strlen(name_ctr);

  if((buffer_length + name_length + 1 + 1) > name_buffer.length)
  {
    ALOG_ERR(
      PSTR("DeviceNameList name buffer full, cannot add class=%d device=%d name=%s len=%d max=%d"),
      unique_module_id,
      device_id,
      name_ctr,
      buffer_length,
      name_buffer.length
    );

    return 0;
  }

  snprintf(name_buffer.ptr + buffer_length, name_buffer.length - buffer_length, "%s|", name_ctr);

  number_buffer.unique_group_ids[index] = (uint16_t)unique_module_id;
  number_buffer.index_ids[index]        = (uint8_t)device_id;
  number_buffer.currently_used_indexed++;

  ALOG_INF(
    PSTR("DLI added: used=%d buf_len=%d class=%d device=%d name=%s"),
    number_buffer.currently_used_indexed,
    strlen(name_buffer.ptr),
    unique_module_id,
    device_id,
    name_ctr
  );

  return 1;
}

int8_t DeviceNameList::RemoveDeviceName(const char* name_ctr, int16_t unique_module_id, int8_t device_id)
{
  if(name_ctr == nullptr || !BuffersReady())
  {
    return 0;
  }

  char current_name[64] = {0};
  int16_t found_index = -1;

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    GetNameAtIndex(i, current_name, sizeof(current_name));

    if(
      strcmp(current_name, name_ctr) == 0 &&
      number_buffer.unique_group_ids[i] == (uint16_t)unique_module_id &&
      number_buffer.index_ids[i] == (uint8_t)device_id
    )
    {
      found_index = i;
      break;
    }
  }

  if(found_index < 0)
  {
    return 0;
  }

  char rebuilt_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH] = {0};
  uint16_t rebuilt_length = 0;

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    if(i == found_index)
    {
      continue;
    }

    GetNameAtIndex(i, current_name, sizeof(current_name));

    const uint16_t current_name_length = strlen(current_name);

    if((rebuilt_length + current_name_length + 1 + 1) > sizeof(rebuilt_buffer))
    {
      ALOG_ERR(PSTR("DeviceNameList::RemoveDeviceName rebuild buffer full"));
      return 0;
    }

    snprintf(rebuilt_buffer + rebuilt_length, sizeof(rebuilt_buffer) - rebuilt_length, "%s|", current_name);
    rebuilt_length = strlen(rebuilt_buffer);
  }

  for(uint8_t i = found_index; i < (number_buffer.currently_used_indexed - 1); i++)
  {
    number_buffer.unique_group_ids[i] = number_buffer.unique_group_ids[i + 1];
    number_buffer.index_ids[i]        = number_buffer.index_ids[i + 1];
  }

  number_buffer.currently_used_indexed--;

  number_buffer.unique_group_ids[number_buffer.currently_used_indexed] = D_MAX_UINT16;
  number_buffer.index_ids[number_buffer.currently_used_indexed]        = D_MAX_UINT8;

  memset(name_buffer.ptr, 0, name_buffer.length);
  strncpy(name_buffer.ptr, rebuilt_buffer, name_buffer.length - 1);

  return 1;
}

bool DeviceNameList::GetModuleAndSensorIDs(const char* module_name, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id)
{
  if(module_name == nullptr || sensor_name == nullptr || out_module_id == nullptr || out_sensor_id == nullptr)
  {
    return false;
  }

  if(!BuffersReady())
  {
    return false;
  }

  char current_sensor_name[64] = {0};

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    const char* current_module_name = tkr->GetModuleName(number_buffer.unique_group_ids[i]);
    GetNameAtIndex(i, current_sensor_name, sizeof(current_sensor_name));

    if(current_module_name == nullptr)
    {
      continue;
    }

    if(strcmp(current_module_name, module_name) == 0 && strcmp(current_sensor_name, sensor_name) == 0)
    {
      *out_module_id = number_buffer.unique_group_ids[i];
      *out_sensor_id = number_buffer.index_ids[i];

      ALOG_DBM(PSTR("Match found: Module ID = %d, Sensor ID = %d"), *out_module_id, *out_sensor_id);
      return true;
    }
  }

  ALOG_WRN(PSTR("No match found for Module Name = %s, Sensor = %s"), module_name, sensor_name);
  return false;
}

bool DeviceNameList::GetModuleAndSensorIDs(uint16_t module_id, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id)
{
  if(sensor_name == nullptr || out_module_id == nullptr || out_sensor_id == nullptr)
  {
    return false;
  }

  if(!BuffersReady())
  {
    return false;
  }

  char current_sensor_name[64] = {0};

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    if(number_buffer.unique_group_ids[i] != module_id)
    {
      continue;
    }

    GetNameAtIndex(i, current_sensor_name, sizeof(current_sensor_name));

    if(strcmp(current_sensor_name, sensor_name) == 0)
    {
      *out_module_id = number_buffer.unique_group_ids[i];
      *out_sensor_id = number_buffer.index_ids[i];

      ALOG_DBM(PSTR("Match found: Module ID = %d, Sensor ID = %d"), *out_module_id, *out_sensor_id);
      return true;
    }
  }

  ALOG_WRN(PSTR("No match found for Module ID = %d, Sensor = %s"), module_id, sensor_name);
  return false;
}

const char* DeviceNameList::GetDeviceName_WithModuleUniqueID(
  int16_t unique_module_id,
  int8_t device_id,
  char* buffer,
  uint16_t buffer_size,
  bool flag_respond_nomatch_if_not_found
)
{
  if(buffer == nullptr || buffer_size == 0)
  {
    return buffer;
  }

  buffer[0] = '\0';

  if(!BuffersReady())
  {
    if(flag_respond_nomatch_if_not_found)
    {
      snprintf(buffer, buffer_size, "%S", PM_SEARCH_NOMATCH);
    }

    return buffer;
  }

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    if(
      number_buffer.unique_group_ids[i] == (uint16_t)unique_module_id &&
      number_buffer.index_ids[i] == (uint8_t)device_id
    )
    {
      GetNameAtIndex(i, buffer, buffer_size);
      return buffer;
    }
  }

  if(flag_respond_nomatch_if_not_found)
  {
    snprintf(buffer, buffer_size, "%S", PM_SEARCH_NOMATCH);
  }
  else
  {
    #ifdef ENABLE_DEVFEATURE_DEVICENAMES__USE_DEVICE_ID_WHEN_NO_NAME_MATCHED
    snprintf(buffer, buffer_size, "%S_%02d", tkr->GetModuleName(unique_module_id), device_id);
    #else
    snprintf(buffer, buffer_size, "%S_Unknown_%03d", tkr->GetModuleName(unique_module_id), random(1000));
    #endif
  }

  ALOG_WRN(PSTR("DeviceName Undefined >> %s"), buffer);

  return buffer;
}

int8_t DeviceNameList::GetDeviceIDbyName(int16_t* class_id, int8_t* device_id, char* name_tofind)
{
  if(class_id == nullptr || device_id == nullptr || name_tofind == nullptr)
  {
    return 0;
  }

  int16_t found_device_id = GetDeviceIDbyName(name_tofind, -1);

  if(found_device_id < 0)
  {
    return 0;
  }

  char current_name[64] = {0};

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    GetNameAtIndex(i, current_name, sizeof(current_name));

    if(strcasecmp(name_tofind, current_name) == 0)
    {
      *class_id  = number_buffer.unique_group_ids[i];
      *device_id = number_buffer.index_ids[i];
      return 1;
    }
  }

  return 0;
}

int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int16_t* class_id)
{
  if(name_tofind == nullptr || haystack == nullptr || device_id == nullptr)
  {
    return -1;
  }

  char current_name[64] = {0};
  const uint16_t item_count = CountCharInCtr(haystack, '|');

  for(uint16_t i = 0; i < item_count && i < number_buffer.currently_used_indexed; i++)
  {
    GetTextIndexed(current_name, sizeof(current_name), i, haystack);

    if(strcasecmp(name_tofind, current_name) == 0)
    {
      *device_id = number_buffer.index_ids[i];

      if(class_id != nullptr)
      {
        *class_id = number_buffer.unique_group_ids[i];
      }

      return i;
    }
  }

  return -1;
}

int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, int16_t unique_module_id)
{
  if(name_tofind == nullptr || !BuffersReady())
  {
    return -1;
  }

  char current_name[64] = {0};

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    if(unique_module_id >= 0 && number_buffer.unique_group_ids[i] != (uint16_t)unique_module_id)
    {
      continue;
    }

    GetNameAtIndex(i, current_name, sizeof(current_name));

    if(strcasecmp(name_tofind, current_name) == 0)
    {
      return number_buffer.index_ids[i];
    }
  }

  return -1;
}

int16_t DeviceNameList::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count)
{
  if(tosearch == nullptr || occurance_count == 0)
  {
    return -1;
  }

  uint8_t count = 0;
  const uint16_t length = strlen(tosearch);

  for(uint16_t i = 0; i < length; i++)
  {
    if(tosearch[i] == tofind)
    {
      count++;

      if(count == occurance_count)
      {
        return i;
      }
    }
  }

  return -1;
}

int8_t DeviceNameList::GetDeviceNameCount(int16_t unique_module_id)
{
  if(!BuffersReady())
  {
    return 0;
  }

  int8_t count = 0;

  for(uint8_t i = 0; i < number_buffer.currently_used_indexed; i++)
  {
    if(number_buffer.unique_group_ids[i] == (uint16_t)unique_module_id)
    {
      count++;
    }
  }

  return count;
}

uint16_t DeviceNameList::CountCharInCtr(const char* tosearch, char tofind)
{
  if(tosearch == nullptr)
  {
    return 0;
  }

  uint16_t count = 0;
  const uint16_t length = strlen(tosearch);

  for(uint16_t i = 0; i < length; i++)
  {
    if(tosearch[i] == tofind)
    {
      count++;
    }
  }

  return count;
}