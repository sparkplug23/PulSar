#include "mJsonTemplate.h"

#ifdef USE_MODULE_CORE__JSON_TEMPLATE

/************************************************************************************************
 * FUNCTION: Tasker
 *
 * SUMMARY:
 * - mJsonTemplate no longer owns boot sequencing.
 * - SettingsLoad() owns early MODULE_TEMPLATE load order.
 * - setup() owns late template timing after module init.
 * - Tasker remains for manual/debug/template-export actions.
 *
 * CHANGED:
 * - 17May26: TASK_TEMPLATES__LOAD_MODULE retained only as manual compatibility path.
 ************************************************************************************************/
int8_t mJsonTemplate::Tasker(uint8_t function, JsonParserObject obj)
{
  switch(function)
  {
    /************************************************************************************************
     * Early module template path
     *
     * Compatibility path for old TASK_TEMPLATES__LOAD_MODULE callers.
     * This loads only MODULE_TEMPLATE using the direct early-safe module parser.
     * It does not dispatch JSON commands.
     ************************************************************************************************/
    case TASK_TEMPLATES__LOAD_MODULE:
    {
      Template_Load();
    }
    break;


    /************************************************************************************************
     * Normal post-init compile-time default templates
     *
     * This is the important late/default path.
     *
     * It applies templates such as:
     * - FUNCTION_TEMPLATE
     * - LIGHTING_TEMPLATE
     * - RULES_TEMPLATE
     * - DRIVERS_TEMPLATE
     * - SENSORS_TEMPLATE
     * - NETWORK_TEMPLATE
     * - SYSTEM_TEMPLATE
     *
     * This is not an override pass.
     * It runs after modules have completed init, so TASK_JSON_COMMAND_ID handlers exist.
     ************************************************************************************************/
    case TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM:
    {
      ModuleDeviceTemplate_CompileTime_Load_Late(false);
    }
    break;


    /************************************************************************************************
     * Development override pass
     *
     * This is intentionally separate from normal post-init defaults.
     * It only applies templates that have matching *_OVERRIDE defines.
     ************************************************************************************************/
    case TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
    {
      ModuleDeviceTemplate_CompileTime_Load_Late(true);
    }
    break;


    #ifdef ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
    case TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM:
    {
      #ifdef USE_MODULE_CORE_FILESYSTEM
      Templates__SaveHardcodedTemplateToFilesystem();
      #endif
    }
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
}


/************************************************************************************************
 * FUNCTION: Template_Load
 *
 * SUMMARY:
 * - Compatibility wrapper for old TASK_TEMPLATES__LOAD_MODULE path.
 * - Loads only MODULE_TEMPLATE using default/provisioning semantics.
 * - This is the early module/device identity path.
 *
 * IMPORTANT:
 * - This does not load LIGHTING_TEMPLATE or other late templates.
 * - Late templates are handled by TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM.
 * - Override templates are handled by TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM.
 ************************************************************************************************/
void mJsonTemplate::Template_Load()
{
  ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false);
}

/************************************************************************************************
 * FUNCTION: ReadModuleTemplateFromProgmem
 *
 * SUMMARY:
 * - Compatibility wrapper for older direct call sites.
 * - Loads MODULE_TEMPLATE using default/provisioning semantics.
 *
 * CHANGED:
 * - 17May26: Converted to wrapper around ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false).
 ************************************************************************************************/
bool mJsonTemplate::ReadModuleTemplateFromProgmem()
{
  return ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false);
}


/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_Load_ModuleOnly
 *
 * SUMMARY:
 * - Loads only MODULE_TEMPLATE.
 * - This is the early boot compile-time template path.
 * - It uses the direct module-template parser, not TASK_JSON_COMMAND_ID.
 *
 * ARGUMENTS:
 * - override_reloading_pass:
 *   - false:
 *       Default/provisioning pass.
 *       SettingsLoad() calls this only when saved settings were missing/invalid.
 *
 *   - true:
 *       Override pass.
 *       Loads only if USE_MODULE_TEMPLATE__OVERRIDE is defined.
 *
 * RETURNS:
 * - true  = module template was loaded and parsed
 * - false = no module template was loaded
 *
 * CHANGED:
 * - 17May26: Added explicit early module-only compile-time template loader.
 ************************************************************************************************/
bool mJsonTemplate::ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(bool override_reloading_pass)
{
  bool load_template = false;

  #ifdef USE_MODULE_TEMPLATE
  if (!override_reloading_pass)
  {
    load_template = true;
  }

    #ifdef USE_MODULE_TEMPLATE__OVERRIDE
    if (override_reloading_pass)
    {
      load_template = true;
    }
    #endif
  #endif

  if (!load_template)
  {
    ALOG_INF(
      PSTR(D_LOG_SETTINGS "MODULE_TEMPLATE skipped override_pass=%u"),
      (unsigned)override_reloading_pass
    );

    return false;
  }

  #ifdef USE_MODULE_TEMPLATE
  return ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P(
    override_reloading_pass ? "MODULE_TEMPLATE__OVERRIDE" : "MODULE_TEMPLATE",
    MODULE_TEMPLATE,
    sizeof(MODULE_TEMPLATE)
  );
  #else
  return false;
  #endif
}


/***
 * Used paired with SettingsDefault during boot
 */
void mJsonTemplate::ModuleDeviceTemplate__LoadDefault()
{
  #ifdef USE_MODULE_TEMPLATE
  ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P(
    "MODULE_TEMPLATE",
    MODULE_TEMPLATE,
    sizeof(MODULE_TEMPLATE)
  );
  #endif
}


/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P
 *
 * SUMMARY:
 * - Copies a PROGMEM module template into mutable RAM.
 * - Calls the early-safe direct module-template parser.
 *
 * IMPORTANT:
 * - This does not use TASK_JSON_COMMAND_ID.
 * - This is safe before GpioInit(), TASK_PRE_INIT, and TASK_INIT.
 *
 * CHANGED:
 * - 17May26: Added safe bounded copy and direct parser dispatch.
 ************************************************************************************************/
bool mJsonTemplate::ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P(
  const char* template_name,
  const char* payload_p,
  size_t payload_size
)
{
  if (!template_name || !payload_p || payload_size == 0)
  {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "ModuleTemplate Apply failed, invalid args"));
    return false;
  }

  char buffer[MODULE_TEMPLATE_MAX_SIZE];

  if (payload_size >= sizeof(buffer))
  {
    ALOG_ERR(
      PSTR(D_LOG_SETTINGS "ModuleTemplate %s too large payload=%u max=%u"),
      template_name,
      (unsigned)payload_size,
      (unsigned)(sizeof(buffer) - 1)
    );

    tkr_set->runtime.template_loading.status.module = mSettings::TemplateSource::FAILED_LOAD;
    return false;
  }

  memset(buffer, 0x00, sizeof(buffer));

  memcpy_P(buffer, payload_p, payload_size);
  buffer[sizeof(buffer) - 1] = '\0';

  ALOG_INF(
    PSTR(D_LOG_SETTINGS "ModuleTemplate %s load bytes=%u"),
    template_name,
    (unsigned)strlen(buffer)
  );

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  ALOG_INF(PSTR(D_LOG_SETTINGS "ModuleTemplate %s = %s"), template_name, buffer);
  #endif

  tkr_set->runtime.template_loading.status.module = mSettings::TemplateSource::HEADER_TEMPLATE;

  tkr_pins->ModuleTemplate__ParseCJSONBuffer(buffer);

  return true;
}


/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P
 *
 * SUMMARY:
 * - Copies a PROGMEM JSON template into data_buffer.
 * - Dispatches through TASK_JSON_COMMAND_ID.
 *
 * IMPORTANT:
 * - This is for late templates only.
 * - It should run after modules are initialised and command parsers are available.
 *
 * CHANGED:
 * - 17May26: Added generic late-template JSON-command dispatcher.
 ************************************************************************************************/
bool mJsonTemplate::ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
  const char* template_name,
  const char* payload_p,
  size_t payload_size
)
{
  if (!template_name || !payload_p || payload_size == 0)
  {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "JsonTemplate Apply failed, invalid args"));
    return false;
  }

  data_buffer.ClearDeep();

  const size_t buffer_max = sizeof(data_buffer.payload.ctr) - 1;
  size_t copy_len = payload_size;

  if (copy_len > buffer_max)
  {
    ALOG_ERR(
      PSTR(D_LOG_SETTINGS "JsonTemplate %s too large payload=%u max=%u"),
      template_name,
      (unsigned)payload_size,
      (unsigned)buffer_max
    );

    return false;
  }

  memcpy_P(data_buffer.payload.ctr, payload_p, copy_len);
  data_buffer.payload.ctr[copy_len] = '\0';
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  ALOG_INF(
    PSTR(D_LOG_SETTINGS "JsonTemplate %s load bytes=%u"),
    template_name,
    (unsigned)data_buffer.payload.length_used
  );

  #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  ALOG_INF(PSTR(D_LOG_SETTINGS "JsonTemplate %s = %s"), template_name, data_buffer.payload.ctr);
  #endif

  tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

  return true;
}


/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_Load_Late
 *
 * SUMMARY:
 * - Loads late compile-time templates through TASK_JSON_COMMAND_ID.
 * - Late templates are not early GPIO/module identity templates.
 * - They must run after module init, when command parsers are available.
 *
 * ARGUMENTS:
 * - override_reloading_pass:
 *   - false:
 *       Default/provisioning pass.
 *       setup() should call this only when SettingsLoad() generated defaults.
 *
 *   - true:
 *       Override pass.
 *       Each template only loads if its matching *_OVERRIDE define exists.
 *
 * CHANGED:
 * - 17May26: Added late compile-time template loader.
 ************************************************************************************************/
bool mJsonTemplate::ModuleDeviceTemplate_CompileTime_Load_Late(bool override_reloading_pass)
{
  bool any_template_loaded = false;
  bool load_template = false;

  /************************************************************************************************
   * SECTION: FUNCTION_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_FUNCTION_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_FUNCTION_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "FUNCTION_TEMPLATE__OVERRIDE" : "FUNCTION_TEMPLATE",
      FUNCTION_TEMPLATE,
      sizeof(FUNCTION_TEMPLATE)
    )) {
      tkr_set->runtime.template_loading.status.function = mSettings::TemplateSource::HEADER_TEMPLATE;
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: LIGHTING_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_LIGHTING_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_LIGHTING_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "LIGHTING_TEMPLATE__OVERRIDE" : "LIGHTING_TEMPLATE",
      LIGHTING_TEMPLATE,
      sizeof(LIGHTING_TEMPLATE)
    )) {
      tkr_set->runtime.template_loading.status.lighting = mSettings::TemplateSource::HEADER_TEMPLATE;
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: RULES_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_RULES_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_RULES_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "RULES_TEMPLATE__OVERRIDE" : "RULES_TEMPLATE",
      RULES_TEMPLATE,
      sizeof(RULES_TEMPLATE)
    )) {
      tkr_set->runtime.template_loading.status.rules = mSettings::TemplateSource::HEADER_TEMPLATE;
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: NEXTION_HMI_INPUT_MAP_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_NEXTION_HMI_INPUT_MAP_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_NEXTION_HMI_INPUT_MAP_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "NEXTION_HMI_INPUT_MAP_TEMPLATE__OVERRIDE" : "NEXTION_HMI_INPUT_MAP_TEMPLATE",
      NEXTION_HMI_INPUT_MAP_TEMPLATE,
      sizeof(NEXTION_HMI_INPUT_MAP_TEMPLATE)
    )) {
      tkr_set->runtime.template_loading.status.nextion_hmi_input_map = mSettings::TemplateSource::HEADER_TEMPLATE;
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: DISPLAY_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_DISPLAY_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_DISPLAY_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "DISPLAY_TEMPLATE__OVERRIDE" : "DISPLAY_TEMPLATE",
      DISPLAY_TEMPLATE,
      sizeof(DISPLAY_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: DISPLAY_HMI_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_DISPLAY_HMI_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_DISPLAY_HMI_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "DISPLAY_HMI_TEMPLATE__OVERRIDE" : "DISPLAY_HMI_TEMPLATE",
      DISPLAY_HMI_TEMPLATE,
      sizeof(DISPLAY_HMI_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: DRIVERS_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_DRIVERS_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_DRIVERS_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "DRIVERS_TEMPLATE__OVERRIDE" : "DRIVERS_TEMPLATE",
      DRIVERS_TEMPLATE,
      sizeof(DRIVERS_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: SENSORS_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_SENSORS_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_SENSORS_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "SENSORS_TEMPLATE__OVERRIDE" : "SENSORS_TEMPLATE",
      SENSORS_TEMPLATE,
      sizeof(SENSORS_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: CONTROLLER_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_CONTROLLER_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_CONTROLLER_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "CONTROLLER_TEMPLATE__OVERRIDE" : "CONTROLLER_TEMPLATE",
      CONTROLLER_TEMPLATE,
      sizeof(CONTROLLER_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: NETWORK_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_NETWORK_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_NETWORK_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "NETWORK_TEMPLATE__OVERRIDE" : "NETWORK_TEMPLATE",
      NETWORK_TEMPLATE,
      sizeof(NETWORK_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: SYSTEM_TEMPLATE
   ************************************************************************************************/
  #ifdef USE_SYSTEM_TEMPLATE
  load_template = false;

  if (!override_reloading_pass) {
    load_template = true;
  }

    #ifdef USE_SYSTEM_TEMPLATE__OVERRIDE
    if (override_reloading_pass) {
      load_template = true;
    }
    #endif

  if (load_template)
  {
    if (ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
      override_reloading_pass ? "SYSTEM_TEMPLATE__OVERRIDE" : "SYSTEM_TEMPLATE",
      SYSTEM_TEMPLATE,
      sizeof(SYSTEM_TEMPLATE)
    )) {
      any_template_loaded = true;
    }
  }
  #endif

  ALOG_INF(
    PSTR(D_LOG_SETTINGS "LateCompileTimeTemplates override_pass=%u loaded=%u"),
    (unsigned)override_reloading_pass,
    (unsigned)any_template_loaded
  );

  return any_template_loaded;
}

/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_DevelopmentOverridePass
 *
 * SUMMARY:
 * - Performs the late compile-time template pass after module init.
 * - This is a development/testing enforcement path.
 * - It is separate from normal post-init default config loading.
 *
 * POLICY:
 * - Default late templates:
 *   - Applied only when SettingsLoad() rebuilt/generated settings from defaults.
 *
 * - Override late templates:
 *   - Always attempted.
 *   - ModuleDeviceTemplate_CompileTime_Load_Late(true) internally applies only templates
 *     with matching *_OVERRIDE defines.
 *
 * NOTES:
 * - This uses TASK_JSON_COMMAND_ID internally.
 * - Therefore, this must run after modules have completed PRE_INIT / INIT / POST_INIT.
 * - Do not use this for early GPIO/device identity.
 *
 * CHANGED:
 * - 18May26: Added explicit development late-template enforcement wrapper.
 ************************************************************************************************/
void mJsonTemplate::ModuleDeviceTemplate_CompileTime_DevelopmentOverridePass()
{
  /************************************************************************************************
   * SECTION: Late default template pass
   *
   * Only apply normal late templates when SettingsLoad() rebuilt defaults.
   ************************************************************************************************/

  if (tkr_set->runtime.settings_holder_hardcorded_stored_changed)
  {
    ModuleDeviceTemplate_CompileTime_Load_Late(false);
  }

  /************************************************************************************************
   * SECTION: Late override template pass
   *
   * Always call this in development pass.
   * Individual templates only apply if their *_OVERRIDE define is enabled.
   ************************************************************************************************/

  ModuleDeviceTemplate_CompileTime_Load_Late(true);
}

#ifdef ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
/************************************************************************************************
 * FUNCTION: Templates__SaveProgmemTemplateToFilesystem_P
 *
 * SUMMARY:
 * - Saves one compile-time PROGMEM JSON template to the internal filesystem.
 * - This lives in mJsonTemplate because it is template-owned diagnostic/export logic.
 * - The internal filesystem is accessed through the normal core filesystem build path so the
 *   exported files are visible from /edit.
 *
 * ARGUMENTS:
 * - file_path:
 *   - Internal filesystem path.
 *   - Must be flat/root-level for /edit visibility.
 *   - Example:
 *       "/orig_template_function.json"
 *
 * - template_name:
 *   - Log/debug name of the template.
 *
 * - payload_p:
 *   - PROGMEM pointer to the template JSON.
 *
 * - payload_size:
 *   - sizeof(TEMPLATE_ARRAY).
 *   - The final null terminator is not written to the file.
 *
 * RETURNS:
 * - true  = file written
 * - false = failed
 *
 * CHANGED:
 * - 17May26: Added mJsonTemplate-owned compile-time template export helper.
 ************************************************************************************************/
bool mJsonTemplate::Templates__SaveProgmemTemplateToFilesystem_P(
  const char* file_path,
  const char* template_name,
  const char* payload_p,
  size_t payload_size
)
{
  if (!file_path || !template_name || !payload_p || payload_size == 0)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Template export skipped, invalid args"));
    return false;
  }

  /************************************************************************************************
   * SECTION: Validate flat internal filesystem path
   *
   * These files are intended for /edit, so keep them root-level.
   * Valid:
   *   /orig_template_function.json
   *
   * Invalid:
   *   orig_template_function.json
   *   /templates/orig_template_function.json
   ************************************************************************************************/

  if (file_path[0] != '/')
  {
    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "Template export failed, path must start with / template=%s file=%s"),
      template_name,
      file_path
    );

    return false;
  }

  if (strchr(file_path + 1, '/') != nullptr)
  {
    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "Template export failed, path must be flat template=%s file=%s"),
      template_name,
      file_path
    );

    return false;
  }

  /************************************************************************************************
   * SECTION: Validate filesystem module pointer
   *
   * mJsonTemplate owns the template logic, but the filesystem module owns FS initialisation.
   ************************************************************************************************/

  #ifndef USE_MODULE_CORE_FILESYSTEM
  ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Template export failed, USE_MODULE_CORE_FILESYSTEM disabled"));
  return false;
  #else

  if (!tkr_mfile)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Template export failed, tkr_mfile is null"));
    return false;
  }

  /************************************************************************************************
   * SECTION: Open target file
   *
   * FILE_SYSTEM is the internal filesystem used by /edit.
   * Use write/truncate mode so each export reflects the current firmware build exactly.
   ************************************************************************************************/

  File file = FILE_SYSTEM.open(file_path, "w");

  if (!file)
  {
    ALOG_ERR(
      PSTR(D_LOG_FILESYSTEM "Template export failed open template=%s file=%s"),
      template_name,
      file_path
    );

    return false;
  }

  /************************************************************************************************
   * SECTION: Write PROGMEM payload in chunks
   *
   * sizeof(TEMPLATE) includes the terminating '\0'.
   * Do not write the terminator to the JSON file.
   ************************************************************************************************/

  size_t bytes_to_write = payload_size;

  if (bytes_to_write > 0) {
    bytes_to_write--;
  }

  size_t bytes_written_total = 0;

  uint8_t buffer[128];

  while (bytes_written_total < bytes_to_write)
  {
    size_t chunk_len = bytes_to_write - bytes_written_total;

    if (chunk_len > sizeof(buffer)) {
      chunk_len = sizeof(buffer);
    }

    memcpy_P(buffer, payload_p + bytes_written_total, chunk_len);

    size_t written = file.write(buffer, chunk_len);

    if (written != chunk_len)
    {
      file.close();

      ALOG_ERR(
        PSTR(D_LOG_FILESYSTEM "Template export short write template=%s file=%s written=%u expected=%u"),
        template_name,
        file_path,
        (unsigned)written,
        (unsigned)chunk_len
      );

      return false;
    }

    bytes_written_total += chunk_len;
  }

  file.flush();
  file.close();

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "Template exported template=%s file=%s bytes=%u"),
    template_name,
    file_path,
    (unsigned)bytes_written_total
  );

  return true;

  #endif // USE_MODULE_CORE_FILESYSTEM
}


/************************************************************************************************
 * FUNCTION: Templates__SaveHardcodedTemplateToFilesystem
 *
 * SUMMARY:
 * - Exports all compile-time templates enabled in this build into the internal filesystem.
 * - Output files are flat root-level files for direct viewing/editing through /edit.
 *
 * OUTPUT FILES:
 * - /orig_template_module.json
 * - /orig_template_function.json
 * - /orig_template_lighting.json
 * - /orig_template_rules.json
 * - /orig_template_nextion_hmi_input_map.json
 * - /orig_template_display.json
 * - /orig_template_display_hmi.json
 * - /orig_template_drivers.json
 * - /orig_template_sensors.json
 * - /orig_template_controller.json
 * - /orig_template_network.json
 * - /orig_template_system.json
 *
 * NOTES:
 * - Only templates enabled by their USE_*_TEMPLATE define are exported.
 * - This is a diagnostic/export utility only.
 * - It does not load or apply templates.
 *
 * CHANGED:
 * - 17May26: Moved hardcoded template export into mFileSystem.
 ************************************************************************************************/
void mJsonTemplate::Templates__SaveHardcodedTemplateToFilesystem()
{
  ALOG_HGL(PSTR(D_LOG_FILESYSTEM "Templates__SaveHardcodedTemplateToFilesystem"));

  uint8_t exported_count = 0;

  #ifdef USE_MODULE_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_module.json",
    "MODULE_TEMPLATE",
    MODULE_TEMPLATE,
    sizeof(MODULE_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_FUNCTION_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_function.json",
    "FUNCTION_TEMPLATE",
    FUNCTION_TEMPLATE,
    sizeof(FUNCTION_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_LIGHTING_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_lighting.json",
    "LIGHTING_TEMPLATE",
    LIGHTING_TEMPLATE,
    sizeof(LIGHTING_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_RULES_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_rules.json",
    "RULES_TEMPLATE",
    RULES_TEMPLATE,
    sizeof(RULES_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_NEXTION_HMI_INPUT_MAP_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_nextion_hmi_input_map.json",
    "NEXTION_HMI_INPUT_MAP_TEMPLATE",
    NEXTION_HMI_INPUT_MAP_TEMPLATE,
    sizeof(NEXTION_HMI_INPUT_MAP_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_DISPLAY_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_display.json",
    "DISPLAY_TEMPLATE",
    DISPLAY_TEMPLATE,
    sizeof(DISPLAY_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_DISPLAY_HMI_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_display_hmi.json",
    "DISPLAY_HMI_TEMPLATE",
    DISPLAY_HMI_TEMPLATE,
    sizeof(DISPLAY_HMI_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_DRIVERS_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_drivers.json",
    "DRIVERS_TEMPLATE",
    DRIVERS_TEMPLATE,
    sizeof(DRIVERS_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_SENSORS_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_sensors.json",
    "SENSORS_TEMPLATE",
    SENSORS_TEMPLATE,
    sizeof(SENSORS_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_CONTROLLER_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_controller.json",
    "CONTROLLER_TEMPLATE",
    CONTROLLER_TEMPLATE,
    sizeof(CONTROLLER_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_NETWORK_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_network.json",
    "NETWORK_TEMPLATE",
    NETWORK_TEMPLATE,
    sizeof(NETWORK_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif


  #ifdef USE_SYSTEM_TEMPLATE
  if (Templates__SaveProgmemTemplateToFilesystem_P(
    "/orig_template_system.json",
    "SYSTEM_TEMPLATE",
    SYSTEM_TEMPLATE,
    sizeof(SYSTEM_TEMPLATE)
  )) {
    exported_count++;
  }
  #endif

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "Template export complete count=%u"),
    (unsigned)exported_count
  );
}

#endif // ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

#endif // Header
