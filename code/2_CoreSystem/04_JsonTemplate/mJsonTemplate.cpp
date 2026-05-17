#include "mJsonTemplate.h"

#ifdef USE_MODULE_CORE__JSON_TEMPLATE


/************************************************************************************************
 * FUNCTION: Tasker
 *
 * SUMMARY:
 * - mJsonTemplate no longer owns boot sequencing.
 * - Settings will call ModuleDeviceTemplate_CompileTime_Load_ModuleOnly() directly.
 * - Tasker remains only for manual/debug/template-export actions.
 *
 * CHANGED:
 * - 17May26: TASK_TEMPLATES__LOAD_MODULE kept as compatibility/manual trigger only.
 ************************************************************************************************/
int8_t mJsonTemplate::Tasker(uint8_t function, JsonParserObject obj)
{
  switch(function)
  {
    /************************************************************************************************
     * SECTION: Manual/debug module-template load
     *
     * NOTE:
     * - This should not be the normal boot-order mechanism anymore.
     * - Settings owns the load order.
     ************************************************************************************************/
    case TASK_TEMPLATES__LOAD_MODULE:
      Template_Load();
    break;

    /************************************************************************************************
     * SECTION: Development export of hardcoded templates
     ************************************************************************************************/
    #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
    case TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM:
      Templates__SaveHardcodedTemplateToFilesystem();
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
}


/************************************************************************************************
 * FUNCTION: Template_Load
 *
 * SUMMARY:
 * - Compatibility wrapper for the old TASK_TEMPLATES__LOAD_MODULE path.
 * - Loads MODULE_TEMPLATE as an override-style/manual action.
 *
 * NOTES:
 * - Normal boot should call ModuleDeviceTemplate_CompileTime_Load_ModuleOnly()
 *   directly from the Settings load path.
 *
 * CHANGED:
 * - 17May26: Converted to wrapper around new module-only compile-time loader.
 ************************************************************************************************/
void mJsonTemplate::Template_Load()
{
  ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(true);
}


/************************************************************************************************
 * FUNCTION: ReadModuleTemplateFromProgmem
 *
 * SUMMARY:
 * - Compatibility wrapper for older call sites.
 * - Loads MODULE_TEMPLATE without requiring the caller to know the new naming.
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
 *       Caller should only call this when saved settings were missing/invalid.
 *
 *   - true:
 *       Override pass.
 *       Loads only if USE_MODULE_TEMPLATE__OVERRIDE is defined.
 *
 * RETURNS:
 * - true  = module template was loaded and parsed
 * - false = no module template was loaded
 *
 * POLICY:
 * - USE_MODULE_TEMPLATE enables the compile-time module template.
 * - USE_MODULE_TEMPLATE__OVERRIDE allows the same template to be forced over saved settings.
 *
 * CHANGED:
 * - 17May26: Added explicit early module-only compile-time template loader.
 ************************************************************************************************/
bool mJsonTemplate::ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(bool override_reloading_pass)
{
  bool load_template = false;

  /************************************************************************************************
   * SECTION: Decide whether this pass should load MODULE_TEMPLATE
   ************************************************************************************************/

#ifdef USE_MODULE_TEMPLATE

  if (!override_reloading_pass)
  {
    /************************************************************************************************
     * Default/provisioning pass.
     *
     * The caller decides whether defaults are needed. If this function is called with
     * override_reloading_pass=false, then it should load MODULE_TEMPLATE.
     ************************************************************************************************/
    load_template = true;
  }

  #ifdef USE_MODULE_TEMPLATE__OVERRIDE
  if (override_reloading_pass)
  {
    /************************************************************************************************
     * Forced override pass.
     *
     * This allows debug/deployment firmware to force GPIO/module identity over saved settings
     * without changing SETTINGS_HOLDER.
     ************************************************************************************************/
    load_template = true;
  }
  #endif

#endif // USE_MODULE_TEMPLATE


  /************************************************************************************************
   * SECTION: Apply template if selected
   ************************************************************************************************/

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
  ALOG_INF(PSTR(D_LOG_SETTINGS "MODULE_TEMPLATE unavailable"));
  return false;
#endif
}


/************************************************************************************************
 * FUNCTION: ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P
 *
 * SUMMARY:
 * - Copies a PROGMEM module template into a mutable RAM buffer.
 * - Calls the early-safe direct module-template parser.
 *
 * IMPORTANT:
 * - This does not use TASK_JSON_COMMAND_ID.
 * - This is safe before GpioInit(), TASK_PRE_INIT, and TASK_INIT.
 *
 * RETURNS:
 * - true  = copied and parsed
 * - false = invalid input or template too large
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

  /************************************************************************************************
   * SECTION: Copy PROGMEM template into mutable RAM
   *
   * JsonParser mutates the buffer during parsing, so PROGMEM cannot be parsed directly.
   * Use a fixed compile-time buffer instead of variable length stack arrays.
   ************************************************************************************************/

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

  /************************************************************************************************
   * SECTION: Parse early module template
   ************************************************************************************************/

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



// int8_t mJsonTemplate::Tasker(uint8_t function, JsonParserObject obj)
// {

//   if(function == TASK_TEMPLATES__LOAD_MODULE){
//     Template_Load();
//   }

//   /************
//    * INIT SECTION * 
//   *******************/
//   switch(function){
//     case TASK_PRE_INIT:
//       // Pre_Init();
//     break;
//     #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
//     case TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM:
//       Templates__SaveHardcodedTemplateToFilesystem();
//     break;
//     #endif
//   }
    
//   return TASKER_RESULT__UNKNOWN_ID;

// } // END Tasker


// /**
//  * @brief Boot order
//  * ** from filesystem
//  * ** any hardcoded templates
//  * ** none, load as default device
//  * 
//  */
// void mJsonTemplate::Template_Load()
// {


//     ReadModuleTemplateFromProgmem();
//     // ParseModuleTemplate(); //probably goes elsewhere


// }


// /**
//  * @brief Function reads templates from progmem if available, then calls TemplateParser
//  * @note JsonParser needs to have the buffer on the stack so it can be edited (token replacing), so must be read from progmem
//  * @return bool if there was a template in progmem
//  * */
// bool mJsonTemplate::ReadModuleTemplateFromProgmem(){

// DEBUG_LINE_HERE2

//   #ifdef USE_MODULE_TEMPLATE
//     uint16_t progmem_size = sizeof(MODULE_TEMPLATE);
//     progmem_size = progmem_size>MODULE_TEMPLATE_MAX_SIZE?MODULE_TEMPLATE_MAX_SIZE:progmem_size;
//     // create parse buffer
//     char buffer[progmem_size];
//     // Read into local
// DEBUG_LINE_HERE2
//     memcpy_P(buffer,MODULE_TEMPLATE,sizeof(MODULE_TEMPLATE));
    
// DEBUG_LINE_HERE2
//     tkr_set->runtime.template_loading.status.module = mSettings::TemplateSource::HEADER_TEMPLATE;

//     #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR("MODULE_TEMPLATE Load"));// = \"%s\""), buffer);
//     ALOG_INF(PSTR("Load = \"%s\""), buffer);
//     #endif // ENABLE_LOG_LEVEL_INFO
//     #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

//     #ifdef ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
//     ALOG_INF( PSTR("mPins::ReadModuleTemplateFromProgmem [%d bytes] = \"%s\""), progmem_size, buffer);
//     #endif // ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH

// DEBUG_LINE_HERE2
//     tkr_pins->ModuleTemplate__ParseCJSONBuffer(buffer);
// DEBUG_LINE_HERE2
//     return true;
//   #else 
//     #warning "USE_MODULE_TEMPLATE not defined"
//     return false;
//   #endif // MODULE_TEMPLATE
  
// }


#endif // USE_MODULE_CORE_I2C
