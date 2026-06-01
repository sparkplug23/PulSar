#include "mFileSystem.h"

#ifdef USE_MODULE_CORE_FILESYSTEM

/************************************************************************************************
 * FILE: PFS_MinimalStorage.cpp
 *
 * SUMMARY:
 * - PFS = PulSar File Storage.
 * - Provides one simple storage API for main settings.
 * - Uses internal filesystem when mounted.
 * - Uses ESP8266 fixed-sector fallback when no filesystem is available.
 *
 * CHANGED:
 * - 31May26: Minimal sector fallback added for ESP8266 no-FS builds.
 ************************************************************************************************/

#if defined(ESP8266)
  #if defined(DISABLE_FILESYSTEM) || (defined(PULSAR_HAS_FILESYSTEM) && (PULSAR_HAS_FILESYSTEM == 0))
    #ifndef ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK
      #define ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK
    #endif
  #endif
#endif

#ifndef PFS_MINIMAL_SECTOR_BASE_ADDRESS
  #define PFS_MINIMAL_SECTOR_BASE_ADDRESS 0
#endif

#ifndef PFS_MINIMAL_SECTOR_TOTAL_SIZE
  #define PFS_MINIMAL_SECTOR_TOTAL_SIZE PFS_MINIMAL_SECTOR_SIZE
#endif

/************************************************************************************************
 * FUNCTION: PFS_Init
 ************************************************************************************************/
bool mFileSystem::PFS_Init()
{
  bool ok = false;

  if (IsMounted())
  {
    pfs_backend = PFS_BACKEND_FILESYSTEM;
    ok = true;
    ALOG_INF(PSTR(D_LOG_FILESYSTEM "PFS backend: filesystem"));
  }
  else
  {
#if defined(ESP8266) && defined(ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK)
    ok = PFS_MinimalSector_Init();

    if (ok)
    {
      pfs_backend = PFS_BACKEND_MINIMAL_SECTOR;
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "PFS backend: minimal sector"));
    }
    else
#endif
    {
      pfs_backend = PFS_BACKEND_NONE;
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "PFS backend: none"));
    }
  }

  return ok;
}

bool mFileSystem::PFS_IsAvailable() const
{
  return pfs_backend != PFS_BACKEND_NONE;
}

mFileSystem::PFSBackendType mFileSystem::PFS_GetBackendType() const
{
  return pfs_backend;
}

const char* mFileSystem::PFS_GetBackendName() const
{
  const char* name = "none";

  switch (pfs_backend)
  {
    case PFS_BACKEND_FILESYSTEM:      name = "filesystem";      break;
    case PFS_BACKEND_MINIMAL_SECTOR:  name = "minimal_sector";  break;
    default:                          name = "none";            break;
  }

  return name;
}

/************************************************************************************************
 * FUNCTION: PFS_SaveSettings
 ************************************************************************************************/
bool mFileSystem::PFS_SaveSettings(const uint8_t* data, uint32_t len)
{
  bool ok = data && len;

  if (ok && (pfs_backend == PFS_BACKEND_NONE))
  {
    ok = PFS_Init();
  }

  if (ok)
  {
    switch (pfs_backend)
    {
      case PFS_BACKEND_FILESYSTEM:
        ok = PFS_File_SaveSettings(data, len);
      break;

      case PFS_BACKEND_MINIMAL_SECTOR:
        ok = PFS_MinimalSector_Save(PFS_MINIMAL_SECTOR_SETTINGS_MAIN_OFFSET, data, len);
      break;

      default:
        ok = false;
      break;
    }
  }

  return ok;
}

/************************************************************************************************
 * FUNCTION: PFS_LoadSettings
 ************************************************************************************************/
bool mFileSystem::PFS_LoadSettings(uint8_t* data, uint32_t max_len, uint32_t* loaded_len)
{
  bool ok = data && max_len;

  if (loaded_len)
  {
    *loaded_len = 0;
  }

  if (ok && (pfs_backend == PFS_BACKEND_NONE))
  {
    ok = PFS_Init();
  }

  if (ok)
  {
    switch (pfs_backend)
    {
      case PFS_BACKEND_FILESYSTEM:
        ok = PFS_File_LoadSettings(data, max_len, loaded_len);
      break;

      case PFS_BACKEND_MINIMAL_SECTOR:
        ok = PFS_MinimalSector_Load(PFS_MINIMAL_SECTOR_SETTINGS_MAIN_OFFSET, data, max_len, loaded_len);
      break;

      default:
        ok = false;
      break;
    }
  }

  return ok;
}

/************************************************************************************************
 * FUNCTION: PFS_SettingsExists
 ************************************************************************************************/
bool mFileSystem::PFS_SettingsExists()
{
  bool exists = false;

  if (pfs_backend == PFS_BACKEND_NONE)
  {
    PFS_Init();
  }

  switch (pfs_backend)
  {
    case PFS_BACKEND_FILESYSTEM:
      exists = PFS_File_SettingsExists();
    break;

    case PFS_BACKEND_MINIMAL_SECTOR:
      exists = PFS_MinimalSector_Exists(PFS_MINIMAL_SECTOR_SETTINGS_MAIN_OFFSET);
    break;

    default:
      exists = false;
    break;
  }

  return exists;
}

/************************************************************************************************
 * SECTION: Filesystem backend
 ************************************************************************************************/

bool mFileSystem::PFS_File_SaveSettings(const uint8_t* data, uint32_t len)
{
  bool ok = data && len;

  if (ok)
  {
    ok = SaveFile(PFS_SETTINGS_FILE_PATH, data, len);
  }

  return ok;
}

bool mFileSystem::PFS_File_LoadSettings(uint8_t* data, uint32_t max_len, uint32_t* loaded_len)
{
  bool ok = data && max_len;
  uint32_t file_size = 0;

  if (loaded_len)
  {
    *loaded_len = 0;
  }

  if (ok)
  {
    file_size = FileSize(PFS_SETTINGS_FILE_PATH);
    ok = file_size && (file_size <= max_len);
  }

  if (ok)
  {
    ok = LoadFile(PFS_SETTINGS_FILE_PATH, data, file_size);
  }

  if (ok && loaded_len)
  {
    *loaded_len = file_size;
  }

  return ok;
}

bool mFileSystem::PFS_File_SettingsExists()
{
  return FileExists(PFS_SETTINGS_FILE_PATH);
}

/************************************************************************************************
 * SECTION: Minimal sector backend
 ************************************************************************************************/

bool mFileSystem::PFS_MinimalSector_Init()
{
  bool ok = false;

#if defined(ESP8266) && defined(ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK)

  ok =
    (PFS_MINIMAL_SECTOR_BASE_ADDRESS != 0) &&
    ((PFS_MINIMAL_SECTOR_BASE_ADDRESS % PFS_MINIMAL_SECTOR_SIZE) == 0) &&
    (PFS_MINIMAL_SECTOR_TOTAL_SIZE >= PFS_MINIMAL_SECTOR_SIZE);

  if (ok)
  {
    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "PFS minimal sector base=0x%08X size=%u"),
      (unsigned)PFS_MINIMAL_SECTOR_BASE_ADDRESS,
      (unsigned)PFS_MINIMAL_SECTOR_TOTAL_SIZE
    );
  }
  else
  {
    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "PFS minimal sector unavailable base=0x%08X size=%u"),
      (unsigned)PFS_MINIMAL_SECTOR_BASE_ADDRESS,
      (unsigned)PFS_MINIMAL_SECTOR_TOTAL_SIZE
    );
  }

#endif

  return ok;
}

uint32_t mFileSystem::PFS_MinimalSector_CRC32(const uint8_t* data, uint32_t len) const
{
  uint32_t crc = 0xFFFFFFFFUL;

  for (uint32_t i = 0; i < len; i++)
  {
    crc ^= data[i];

    for (uint8_t bit = 0; bit < 8; bit++)
    {
      crc = (crc & 1) ? ((crc >> 1) ^ 0xEDB88320UL) : (crc >> 1);
    }
  }

  return ~crc;
}

/************************************************************************************************
 * FUNCTION: PFS_MinimalSector_Save
 *
 * SUMMARY:
 * - Saves one binary payload into one fixed ESP8266 flash sector/range.
 * - Header + payload are written as one aligned flash buffer.
 ************************************************************************************************/
bool mFileSystem::PFS_MinimalSector_Save(uint32_t sector_offset, const uint8_t* data, uint32_t len)
{
  bool ok = false;

#if defined(ESP8266) && defined(ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK)

  const uint32_t sector_address = PFS_MINIMAL_SECTOR_BASE_ADDRESS + sector_offset;
  const uint32_t max_payload = PFS_MINIMAL_SECTOR_SIZE - sizeof(PFSMinimalSectorHeader);
  const uint32_t write_size = ((sizeof(PFSMinimalSectorHeader) + len + 3) / 4) * 4;
  uint8_t* buffer = nullptr;

  ok =
    data &&
    len &&
    (len <= max_payload) &&
    (PFS_MINIMAL_SECTOR_BASE_ADDRESS != 0) &&
    ((sector_address % PFS_MINIMAL_SECTOR_SIZE) == 0) &&
    ((sector_offset + PFS_MINIMAL_SECTOR_SIZE) <= PFS_MINIMAL_SECTOR_TOTAL_SIZE);

  if (ok)
  {
    buffer = (uint8_t*)malloc(PFS_MINIMAL_SECTOR_SIZE);
    ok = buffer != nullptr;
  }

  if (ok)
  {
    memset(buffer, 0xFF, PFS_MINIMAL_SECTOR_SIZE);

    PFSMinimalSectorHeader header;
    memset(&header, 0xFF, sizeof(header));

    header.magic = PFS_MINIMAL_SECTOR_MAGIC;
    header.version = PFS_MINIMAL_SECTOR_VERSION;
    header.header_size = sizeof(PFSMinimalSectorHeader);
    header.data_size = len;
    header.data_crc = PFS_MinimalSector_CRC32(data, len);
    header.reserved = 0xFFFFFFFFUL;

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), data, len);

    const uint32_t sector_index = sector_address / PFS_MINIMAL_SECTOR_SIZE;

    ok = ESP.flashEraseSector(sector_index);
    delay(1);
  }

  if (ok)
  {
    ok = ESP.flashWrite(sector_address, (uint32_t*)buffer, write_size);
  }

  if (buffer)
  {
    free(buffer);
  }

  if (ok)
  {
    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "PFS minimal save offset=0x%04X len=%u"),
      (unsigned)sector_offset,
      (unsigned)len
    );
  }
  else
  {
    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "PFS minimal save failed offset=0x%04X len=%u max=%u"),
      (unsigned)sector_offset,
      (unsigned)len,
      (unsigned)max_payload
    );
  }

#endif

  return ok;
}

/************************************************************************************************
 * FUNCTION: PFS_MinimalSector_Load
 *
 * SUMMARY:
 * - Loads and validates one binary payload from one fixed ESP8266 flash sector/range.
 ************************************************************************************************/
bool mFileSystem::PFS_MinimalSector_Load(uint32_t sector_offset, uint8_t* data, uint32_t max_len, uint32_t* loaded_len)
{
  bool ok = false;

#if defined(ESP8266) && defined(ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK)

  const uint32_t sector_address = PFS_MINIMAL_SECTOR_BASE_ADDRESS + sector_offset;
  PFSMinimalSectorHeader header;
  uint8_t* buffer = nullptr;
  uint32_t padded_size = 0;

  if (loaded_len)
  {
    *loaded_len = 0;
  }

  ok =
    data &&
    max_len &&
    (PFS_MINIMAL_SECTOR_BASE_ADDRESS != 0) &&
    ((sector_address % PFS_MINIMAL_SECTOR_SIZE) == 0) &&
    ((sector_offset + PFS_MINIMAL_SECTOR_SIZE) <= PFS_MINIMAL_SECTOR_TOTAL_SIZE);

  if (ok)
  {
    memset(&header, 0xFF, sizeof(header));
    ok = ESP.flashRead(sector_address, (uint32_t*)&header, sizeof(header));
  }

  if (ok)
  {
    ok =
      (header.magic == PFS_MINIMAL_SECTOR_MAGIC) &&
      (header.version == PFS_MINIMAL_SECTOR_VERSION) &&
      (header.header_size == sizeof(PFSMinimalSectorHeader)) &&
      (header.data_size > 0) &&
      (header.data_size <= (PFS_MINIMAL_SECTOR_SIZE - sizeof(PFSMinimalSectorHeader))) &&
      (header.data_size <= max_len);
  }

  if (ok)
  {
    padded_size = ((header.data_size + 3) / 4) * 4;
    buffer = (uint8_t*)malloc(padded_size);
    ok = buffer != nullptr;
  }

  if (ok)
  {
    ok = ESP.flashRead(
      sector_address + sizeof(PFSMinimalSectorHeader),
      (uint32_t*)buffer,
      padded_size
    );
  }

  if (ok)
  {
    ok = PFS_MinimalSector_CRC32(buffer, header.data_size) == header.data_crc;
  }

  if (ok)
  {
    memcpy(data, buffer, header.data_size);

    if (loaded_len)
    {
      *loaded_len = header.data_size;
    }

    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "PFS minimal load offset=0x%04X len=%u"),
      (unsigned)sector_offset,
      (unsigned)header.data_size
    );
  }

  if (buffer)
  {
    free(buffer);
  }

#endif

  return ok;
}

bool mFileSystem::PFS_MinimalSector_Exists(uint32_t sector_offset)
{
  bool exists = false;

#if defined(ESP8266) && defined(ENABLE_FEATURE_PFS__MINIMAL_SECTOR_FALLBACK)

  const uint32_t sector_address = PFS_MINIMAL_SECTOR_BASE_ADDRESS + sector_offset;
  PFSMinimalSectorHeader header;

  bool ok =
    (PFS_MINIMAL_SECTOR_BASE_ADDRESS != 0) &&
    ((sector_address % PFS_MINIMAL_SECTOR_SIZE) == 0) &&
    ESP.flashRead(sector_address, (uint32_t*)&header, sizeof(header));

  if (ok)
  {
    exists =
      (header.magic == PFS_MINIMAL_SECTOR_MAGIC) &&
      (header.version == PFS_MINIMAL_SECTOR_VERSION) &&
      (header.header_size == sizeof(PFSMinimalSectorHeader)) &&
      (header.data_size > 0) &&
      (header.data_size <= (PFS_MINIMAL_SECTOR_SIZE - sizeof(PFSMinimalSectorHeader)));
  }

#endif

  return exists;
}

#endif // USE_MODULE_CORE_FILESYSTEM