

#include "mInterface.h"

#ifdef USE_MODULE_NETWORK_INTERFACE

void mInterfaceNetwork::Init(void)
{
  Interface_Config_Clear();

  module_state.mode = ModuleStatus::Running;
}


void mInterfaceNetwork::Load_Config(void)
{
  ALOG_DBG(PSTR(D_LOG_NETWORK "Load_Config"));

  // This module no longer owns JSON template/file loading.
  //
  // Network JSON config should be handled by mJsonTemplate using:
  //
  //   USE_NETWORK_TEMPLATE
  //   NETWORK_TEMPLATE
  //
  // That path should decide whether to apply PROGMEM template, filesystem
  // template, version override, etc.
  //
  // This Init_Defaults path is now only for final hardcoded define fallback.

  NetworkConfig_ApplyDefineDefaults();
}


void mInterfaceNetwork::NetworkConfig_ApplyDefineDefaults(void)
{
  #ifdef USE_MODULE_NETWORK_MQTT
  if(tkr_mqtt)
  {
    tkr_mqtt->EnsureDefaultBroker_FromDefines();
  }
  else
  {
    ALOG_WRN(PSTR(D_LOG_NETWORK "Define defaults: tkr_mqtt null"));
  }
  #endif
}


// -----------------------------------------------------------------------------
// Legacy/private network-config file path disabled
// Date Modified: 23Jun26
// -----------------------------------------------------------------------------

bool mInterfaceNetwork::NetworkConfig_LoadFileAndParse(void)
{
  ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_LoadFileAndParse not used - handled by mJsonTemplate"));
  return false;
}


bool mInterfaceNetwork::NetworkConfig_ParseJSONString(const String& json_string)
{
  (void)json_string;

  ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_ParseJSONString not used - handled by mJsonTemplate"));
  return false;
}


#ifdef USE_NETWORK_TEMPLATE

String mInterfaceNetwork::NetworkConfig_LoadTemplate(void)
{
  ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_LoadTemplate not used - use NETWORK_TEMPLATE via mJsonTemplate"));
  return String();
}


bool mInterfaceNetwork::NetworkConfig_CreateFileFromTemplateAndParse(void)
{
  ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_CreateFileFromTemplateAndParse not used - handled by mJsonTemplate"));
  return false;
}

#endif


void mInterfaceNetwork::Interface_Config_Clear(void)
{
  memset(&config, 0, sizeof(config));

  config.policy.allow_multiple_active = 1;
  config.policy.block_remote_mqtt_when_local_available = 1;
}


mInterfaceNetwork::InterfaceTransportRuntime* mInterfaceNetwork::Interface_GetTransportById(const char* id)
{
  if(!id)
  {
    return nullptr;
  }

  for(uint8_t transport_i = 0; transport_i < config.transport_count; transport_i++)
  {
    if(strcmp(config.transports[transport_i].config.id, id) == 0)
    {
      return &config.transports[transport_i];
    }
  }

  return nullptr;
}


void mInterfaceNetwork::NetworkConfig_DebugPrint(const char* title, const String& data)
{
  (void)title;
  (void)data;

  ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_DebugPrint not used - handled by mJsonTemplate"));
}

#endif // USE_MODULE_NETWORK_INTERFACE



// #include "mInterface.h"

// #ifdef USE_MODULE_NETWORK_INTERFACE

// void mInterfaceNetwork::Init(void)
// {
//   Interface_Config_Clear();
  
//   module_state.mode = ModuleStatus::Running;
// }

// void mInterfaceNetwork::Load_Config(void)
// {
//   ALOG_INF(PSTR(D_LOG_NETWORK "Load_Config"));

//   bool config_loaded = false;

//   // ------------------------------------------------------------------
//   // 1. Filesystem config, if available and present
//   // ------------------------------------------------------------------
//   #ifdef USE_MODULE_CORE_FILESYSTEM
//   if(tkr_mfile)
//   {
//     if(tkr_mfile->FileExists(NETWORK_CONFIG_FILE_PATH))
//     {
//       ALOG_INF(
//         PSTR(D_LOG_NETWORK "Network config file found: %s len=%u"),
//         NETWORK_CONFIG_FILE_PATH,
//         tkr_mfile->FileSize(NETWORK_CONFIG_FILE_PATH)
//       );

//       config_loaded = NetworkConfig_LoadFileAndParse();

//       if(!config_loaded)
//       {
//         ALOG_ERR(PSTR(D_LOG_NETWORK "Network config file exists but parse failed"));
//       }
//     }
//     else
//     {
//       ALOG_WRN(PSTR(D_LOG_NETWORK "Network config file missing: %s"), NETWORK_CONFIG_FILE_PATH);
//     }
//   }
//   else
//   {
//     ALOG_WRN(PSTR(D_LOG_NETWORK "Filesystem module pointer null"));
//   }
//   #else
//   ALOG_INF(PSTR(D_LOG_NETWORK "Filesystem module not compiled"));
//   #endif

//   // ------------------------------------------------------------------
//   // 2. Compiled template, if available
//   // ------------------------------------------------------------------
//   #ifdef USE_NETWORK_TEMPLATE
//   if(!config_loaded)
//   {
//     config_loaded = NetworkConfig_CreateFileFromTemplateAndParse();

//     if(!config_loaded)
//     {
//       ALOG_ERR(PSTR(D_LOG_NETWORK "Compiled network template failed"));
//     }
//   }
//   #else
//   if(!config_loaded)
//   {
//     ALOG_INF(PSTR(D_LOG_NETWORK "No compiled network template, using define defaults"));
//   }
//   #endif

//   // ------------------------------------------------------------------
//   // 3. Define defaults
//   // ------------------------------------------------------------------
//   NetworkConfig_ApplyDefineDefaults();
// }

// void mInterfaceNetwork::NetworkConfig_ApplyDefineDefaults(void)
// {
//   #ifdef USE_MODULE_NETWORK_MQTT
//   if(tkr_mqtt)
//   {
//     tkr_mqtt->EnsureDefaultBroker_FromDefines();
//   }
//   else
//   {
//     ALOG_WRN(PSTR(D_LOG_NETWORK "Define defaults: tkr_mqtt null"));
//   }
//   #endif
// }


// bool mInterfaceNetwork::NetworkConfig_LoadFileAndParse(void)
// {
//   #ifdef USE_MODULE_CORE_FILESYSTEM

//   if(!tkr_mfile)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "NetworkConfig_LoadFileAndParse failed: tkr_mfile null"));
//     return false;
//   }

//   if(!tkr_mfile->FileExists(NETWORK_CONFIG_FILE_PATH))
//   {
//     return false;
//   }

//   const size_t file_size = tkr_mfile->FileSize(NETWORK_CONFIG_FILE_PATH);

//   if(file_size == 0)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "Network config file empty: %s"), NETWORK_CONFIG_FILE_PATH);
//     return false;
//   }

//   if(file_size > NETWORK_CONFIG_MAX_BYTES)
//   {
//     ALOG_ERR(
//       PSTR(D_LOG_NETWORK "Network config too large: %s size=%u max=%u"),
//       NETWORK_CONFIG_FILE_PATH,
//       file_size,
//       NETWORK_CONFIG_MAX_BYTES
//     );
//     return false;
//   }

//   char* file_buffer = static_cast<char*>(malloc(file_size + 1));

//   if(!file_buffer)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "Network config malloc failed size=%u"), file_size + 1);
//     return false;
//   }

//   memset(file_buffer, 0, file_size + 1);

//   const bool loaded = tkr_mfile->LoadFile(
//     NETWORK_CONFIG_FILE_PATH,
//     reinterpret_cast<uint8_t*>(file_buffer),
//     file_size
//   );

//   if(!loaded)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "Failed to load %s"), NETWORK_CONFIG_FILE_PATH);
//     free(file_buffer);
//     return false;
//   }

//   file_buffer[file_size] = '\0';

//   String file_json = String(file_buffer);

//   free(file_buffer);

//   if(file_json.length() == 0)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "Loaded network config empty: %s"), NETWORK_CONFIG_FILE_PATH);
//     return false;
//   }

//   NetworkConfig_DebugPrint("NETWORK_CONFIG_FILE", file_json);

//   return NetworkConfig_ParseJSONString(file_json);

//   #else

//   return false;

//   #endif
// }

// bool mInterfaceNetwork::NetworkConfig_ParseJSONString(const String& json_string)
// {
//   if(json_string.length() == 0)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "Network config JSON empty"));
//     return false;
//   }

//   if(json_string.length() >= DATA_BUFFER_PAYLOAD_MAX_LENGTH)
//   {
//     ALOG_ERR(
//       PSTR(D_LOG_NETWORK "Network config JSON too large len=%u max=%u"),
//       json_string.length(),
//       DATA_BUFFER_PAYLOAD_MAX_LENGTH
//     );
//     return false;
//   }

//   data_buffer.ClearDeep();

//   const uint32_t copy_len =
//     (json_string.length() < DATA_BUFFER_PAYLOAD_MAX_LENGTH - 1) ?
//     json_string.length() :
//     DATA_BUFFER_PAYLOAD_MAX_LENGTH - 1;

//   memcpy(data_buffer.payload.ctr, json_string.c_str(), copy_len);
//   data_buffer.payload.ctr[copy_len] = '\0';
//   data_buffer.payload.length_used = copy_len;

//   ALOG_INF(
//     PSTR(DEBUG_INSERT_PAGE_BREAK D_LOG_NETWORK "NETWORK_CONFIG READ = \"%u|%s\""),
//     data_buffer.payload.length_used,
//     data_buffer.payload.ctr
//   );

//   tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

//   return true;
// }




// #ifdef USE_NETWORK_TEMPLATE

// String mInterfaceNetwork::NetworkConfig_LoadTemplate(void)
// {
//   return String(FPSTR(NETWORK_TEMPLATE));
// }


// bool mInterfaceNetwork::NetworkConfig_CreateFileFromTemplateAndParse(void)
// {
//   String template_json = NetworkConfig_LoadTemplate();

//   if(template_json.length() == 0)
//   {
//     ALOG_ERR(PSTR(D_LOG_NETWORK "NETWORK_TEMPLATE empty"));
//     return false;
//   }

//   NetworkConfig_DebugPrint("NETWORK_TEMPLATE", template_json);

//   #ifdef USE_MODULE_CORE_FILESYSTEM
//   if(tkr_mfile)
//   {
//     if(tkr_mfile->SaveFile(
//         NETWORK_CONFIG_FILE_PATH,
//         reinterpret_cast<const uint8_t*>(template_json.c_str()),
//         template_json.length()
//       ))
//     {
//       ALOG_INF(
//         PSTR(D_LOG_NETWORK "Created network config file from template: %s len=%u"),
//         NETWORK_CONFIG_FILE_PATH,
//         template_json.length()
//       );
//     }
//     else
//     {
//       ALOG_ERR(PSTR(D_LOG_NETWORK "Failed to create %s from template"), NETWORK_CONFIG_FILE_PATH);
//     }
//   }
//   #endif

//   return NetworkConfig_ParseJSONString(template_json);
// }

// #endif

// void mInterfaceNetwork::Interface_Config_Clear(void)
// {
//   memset(&config, 0, sizeof(config));

//   config.policy.allow_multiple_active = 1;
//   config.policy.block_remote_mqtt_when_local_available = 1;
// }


// mInterfaceNetwork::InterfaceTransportRuntime* mInterfaceNetwork::Interface_GetTransportById(const char* id)
// {
//   if (!id) { return nullptr; }

//   for (uint8_t transport_i = 0; transport_i < config.transport_count; transport_i++)
//   {
//     if (strcmp(config.transports[transport_i].config.id, id) == 0)
//     {
//       return &config.transports[transport_i];
//     }
//   }

//   return nullptr;
// }


// void mInterfaceNetwork::NetworkConfig_DebugPrint(const char* title, const String& data)
// {
//   if (!title) { title = "NetworkConfig"; }

//   ALOG_INF(PSTR(D_LOG_NETWORK "---- %s START len=%u ----"), title, data.length());

//   const uint16_t chunk_size = 192;

//   uint16_t data_len = data.length() > 200 ? 200 : data.length();

//   for (uint32_t index = 0; index < data_len; index += chunk_size)
//   {
//     const uint32_t remaining = data_len - index;
//     const uint16_t this_chunk = (remaining > chunk_size) ? chunk_size : remaining;

//     for (uint16_t i = 0; i < this_chunk; i++)
//     {
//       Serial.write(data[index + i]);
//     }
//   }

//   Serial.println();

//   ALOG_INF(PSTR(D_LOG_NETWORK "---- %s END ----"), title);
// }






// #endif