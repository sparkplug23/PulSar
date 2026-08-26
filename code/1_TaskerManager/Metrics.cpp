// #include "Metrics.h"
#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

static const TASKER_FUNCTION_TYPES TASKER_METRICS_DETAILED_TASKS[] =
{
  TASK_LOOP,
  TASK_EVERY_50_MSECOND,
  TASK_EVERY_100_MSECOND,
  TASK_EVERY_250_MSECOND,
  TASK_EVERY_SECOND,
  TASK_JSON_COMMAND_ID,
  TASK_TELEMETRY__SENDER_MQTT,
  TASK_WEB_TELEMETRY
};

static constexpr size_t TASKER_METRICS_DETAILED_TASK_COUNT = sizeof(TASKER_METRICS_DETAILED_TASKS) / sizeof(TASKER_METRICS_DETAILED_TASKS[0]);

static void TaskerMetrics_PrintJSONString(Print& out, const char* text)
{
  if(!text) return;

  while(*text)
  {
    const char c = *text++;

    switch(c)
    {
      case '\\': out.print(F("\\\\")); break;
      case '"':  out.print(F("\\\"")); break;
      case '\n': out.print(F("\\n")); break;
      case '\r': out.print(F("\\r")); break;
      case '\t': out.print(F("\\t")); break;
      default: if((uint8_t)c >= 32) out.print(c); break;
    }
  }
}

TaskerMetrics::~TaskerMetrics()
{
  DisableNow();
}

void TaskerMetrics::RequestEnable(uint16_t expected_module_count)
{
  pending_expected_module_count.store(expected_module_count, std::memory_order_relaxed);
  pending_action.store(ACTION_ENABLE, std::memory_order_release);
}

void TaskerMetrics::RequestDisable()
{
  pending_action.store(ACTION_DISABLE, std::memory_order_release);
}

void TaskerMetrics::RequestReset()
{
  pending_action.store(ACTION_RESET, std::memory_order_release);
}

void TaskerMetrics::RequestSnapshot()
{
  snapshot_ready.store(false, std::memory_order_release);
  snapshot_requested.store(true, std::memory_order_release);
  Touch();
}

bool TaskerMetrics::IsSnapshotReady() const
{
  return snapshot_ready.load(std::memory_order_acquire);
}

void TaskerMetrics::Touch()
{
  last_client_touch_ms.store(millis(), std::memory_order_relaxed);
}

void TaskerMetrics::ProcessPendingRequest()
{
  const uint8_t action = pending_action.exchange(ACTION_NONE, std::memory_order_acquire);

  switch(action)
  {
    case ACTION_ENABLE:  EnableNow(pending_expected_module_count.load(std::memory_order_relaxed)); break;
    case ACTION_DISABLE: DisableNow(); break;
    case ACTION_RESET:   ResetNow(); break;
  }
}

void TaskerMetrics::EnableNow(uint16_t expected_module_count)
{
  if(enabled)
  {
    Touch();
    return;
  }

  if(!expected_module_count) return;

  const size_t detailed_count = (size_t)expected_module_count * TASKER_METRICS_DETAILED_TASK_COUNT;

  ModuleMetrics* new_modules = new(std::nothrow) ModuleMetrics[expected_module_count];
  DetailedTaskMetrics* new_detailed_tasks = new(std::nothrow) DetailedTaskMetrics[detailed_count];

  if(!new_modules || !new_detailed_tasks)
  {
    delete[] new_modules;
    delete[] new_detailed_tasks;
    return;
  }

  modules = new_modules;
  detailed_tasks = new_detailed_tasks;
  module_count = expected_module_count;

  for(uint16_t module_index = 0; module_index < module_count; module_index++)
  {
    for(size_t task_index = 0; task_index < TASKER_METRICS_DETAILED_TASK_COUNT; task_index++)
    {
      detailed_tasks[(module_index * TASKER_METRICS_DETAILED_TASK_COUNT) + task_index].task_id = TASKER_METRICS_DETAILED_TASKS[task_index];
    }
  }

  capture_started_ms = millis();
  last_client_touch_ms.store(capture_started_ms, std::memory_order_relaxed);
  enabled = true;
}

void TaskerMetrics::DisableNow()
{
  enabled = false;
  snapshot_ready.store(false, std::memory_order_release);
  snapshot_requested.store(false, std::memory_order_release);

  delete[] modules;
  delete[] detailed_tasks;
  delete[] snapshot_modules;
  delete[] snapshot_detailed_tasks;

  modules = nullptr;
  detailed_tasks = nullptr;
  snapshot_modules = nullptr;
  snapshot_detailed_tasks = nullptr;

  module_count = 0;
  snapshot_module_count = 0;
  snapshot_capture_age_ms = 0;
  capture_started_ms = 0;

  last_client_touch_ms.store(0, std::memory_order_relaxed);
}

void TaskerMetrics::ResetNow()
{
  if(!enabled || !modules || !detailed_tasks) return;

  for(uint16_t module_index = 0; module_index < module_count; module_index++) modules[module_index].timing = TimingStats{};

  const size_t detailed_count = (size_t)module_count * TASKER_METRICS_DETAILED_TASK_COUNT;
  for(size_t i = 0; i < detailed_count; i++) detailed_tasks[i].timing = TimingStats{};

  capture_started_ms = millis();
  last_client_touch_ms.store(capture_started_ms, std::memory_order_relaxed);
}

bool TaskerMetrics::IsEnabled()
{
  ProcessPendingRequest();
  ProcessSnapshotRequest();

  if(!enabled) return false;

  const uint32_t now = millis();
  const uint32_t last_touch = last_client_touch_ms.load(std::memory_order_relaxed);

  if((now - last_touch) > CLIENT_LEASE_TIMEOUT_MS)
  {
    DisableNow();
    return false;
  }

  return true;
}

uint32_t TaskerMetrics::GetCaptureAgeMs() const
{
  if(!enabled || !capture_started_ms) return 0;
  return millis() - capture_started_ms;
}

int8_t TaskerMetrics::GetDetailedTaskIndex(TASKER_FUNCTION_TYPES task_id)
{
  for(size_t i = 0; i < TASKER_METRICS_DETAILED_TASK_COUNT; i++)
  {
    if(TASKER_METRICS_DETAILED_TASKS[i] == task_id) return (int8_t)i;
  }

  return -1;
}

void TaskerMetrics::UpdateTiming(TimingStats& stats, TASKER_FUNCTION_TYPES task_id, uint32_t elapsed_us)
{
  stats.last_us = elapsed_us;
  stats.total_us += elapsed_us;
  stats.count++;

  if(elapsed_us < stats.min_us) stats.min_us = elapsed_us;

  if(elapsed_us > stats.max_us)
  {
    stats.max_us = elapsed_us;
    stats.max_task_id = static_cast<uint16_t>(task_id);
  }
}

void TaskerMetrics::Record(uint16_t module_index, uint16_t module_id, TASKER_FUNCTION_TYPES task_id, uint32_t elapsed_us)
{
  if(!enabled || !modules || module_index >= module_count) return;

  ModuleMetrics& module = modules[module_index];
  module.module_id = module_id;
  UpdateTiming(module.timing, task_id, elapsed_us);

  const int8_t detail_index = GetDetailedTaskIndex(task_id);
  if(detail_index < 0 || !detailed_tasks) return;

  DetailedTaskMetrics& detail = detailed_tasks[(module_index * TASKER_METRICS_DETAILED_TASK_COUNT) + detail_index];
  detail.module_id = module_id;
  UpdateTiming(detail.timing, task_id, elapsed_us);
}

void TaskerMetrics::WriteSnapshotJSON(Print& out, mTaskerManager& manager)
{
  if(!snapshot_ready.load(std::memory_order_acquire) || !snapshot_modules || !snapshot_detailed_tasks)
  {
    out.print(F("{\"ready\":false}"));
    return;
  }

  Touch();

  const uint32_t age_ms = snapshot_capture_age_ms;
  const uint16_t count_modules = snapshot_module_count;
  const size_t detailed_count = (size_t)count_modules * TASKER_METRICS_DETAILED_TASK_COUNT;

  uint16_t module_records = 0;
  uint16_t detail_records = 0;

  for(uint16_t i = 0; i < count_modules; i++) if(snapshot_modules[i].timing.count) module_records++;
  for(size_t i = 0; i < detailed_count; i++) if(snapshot_detailed_tasks[i].timing.count) detail_records++;

  out.printf_P(PSTR("{\"ready\":true,\"enabled\":%s,\"capture_age_ms\":%lu,\"lease_timeout_ms\":%lu,\"module_records\":%u,\"module_capacity\":%u,\"detail_records\":%u,\"detail_capacity\":%u,\"modules\":["),
    enabled ? "true" : "false",
    age_ms,
    CLIENT_LEASE_TIMEOUT_MS,
    module_records,
    count_modules,
    detail_records,
    (unsigned)detailed_count
  );

  bool first = true;

  for(uint16_t i = 0; i < count_modules; i++)
  {
    const ModuleMetrics& item = snapshot_modules[i];
    if(!item.timing.count) continue;

    if(!first) out.print(',');
    first = false;

    char module_name[64] = {0};
    manager.GetModuleNameDisplayEachWord(item.module_id, module_name, sizeof(module_name));

    const uint32_t avg_us = (uint32_t)(item.timing.total_us / item.timing.count);
    const uint32_t min_us = item.timing.min_us == UINT32_MAX ? 0 : item.timing.min_us;
    const double load_pct = age_ms ? (double)item.timing.total_us / ((double)age_ms * 10.0) : 0.0;
    const uint64_t us_per_second = age_ms ? (item.timing.total_us * 1000ULL) / age_ms : 0;

    char max_task_name[64] = {0};
    const char* max_task_name_ptr = manager.GetTaskName(item.timing.max_task_id);
    if(max_task_name_ptr) snprintf_P(max_task_name, sizeof(max_task_name), PSTR("%S"), max_task_name_ptr);

    out.printf_P(PSTR("{\"module_id\":%u,\"name\":\""), item.module_id);
    TaskerMetrics_PrintJSONString(out, module_name);
    out.printf_P(PSTR("\",\"calls\":%lu,\"last_us\":%lu,\"avg_us\":%lu,\"min_us\":%lu,\"max_us\":%lu,\"max_task_id\":%u,\"max_task_name\":\""),
      item.timing.count,
      item.timing.last_us,
      avg_us,
      min_us,
      item.timing.max_us,
      item.timing.max_task_id
    );
    TaskerMetrics_PrintJSONString(out, max_task_name);
    out.printf_P(PSTR("\",\"total_us\":%llu,\"us_per_second\":%llu,\"load_pct\":%.4f}"),
      (unsigned long long)item.timing.total_us,
      (unsigned long long)us_per_second,
      load_pct
    );
  }

  out.print(F("],\"details\":["));
  first = true;

  for(size_t i = 0; i < detailed_count; i++)
  {
    const DetailedTaskMetrics& item = snapshot_detailed_tasks[i];
    if(!item.timing.count) continue;

    if(!first) out.print(',');
    first = false;

    char module_name[64] = {0};
    manager.GetModuleNameDisplayEachWord(item.module_id, module_name, sizeof(module_name));

    char task_name[64] = {0};
    const char* task_name_ptr = manager.GetTaskName((uint16_t)item.task_id);
    if(task_name_ptr) snprintf_P(task_name, sizeof(task_name), PSTR("%S"), task_name_ptr);

    const uint32_t avg_us = (uint32_t)(item.timing.total_us / item.timing.count);
    const uint32_t min_us = item.timing.min_us == UINT32_MAX ? 0 : item.timing.min_us;
    const double load_pct = age_ms ? (double)item.timing.total_us / ((double)age_ms * 10.0) : 0.0;

    out.printf_P(PSTR("{\"module_id\":%u,\"module_name\":\""), item.module_id);
    TaskerMetrics_PrintJSONString(out, module_name);
    out.printf_P(PSTR("\",\"task_id\":%u,\"task_name\":\""), (uint16_t)item.task_id);
    TaskerMetrics_PrintJSONString(out, task_name);
    out.printf_P(PSTR("\",\"calls\":%lu,\"last_us\":%lu,\"avg_us\":%lu,\"min_us\":%lu,\"max_us\":%lu,\"total_us\":%llu,\"load_pct\":%.4f}"),
      item.timing.count,
      item.timing.last_us,
      avg_us,
      min_us,
      item.timing.max_us,
      (unsigned long long)item.timing.total_us,
      load_pct
    );
  }

  out.print(F("]}"));
}

void TaskerMetrics::ProcessSnapshotRequest()
{
  if(!snapshot_requested.exchange(false, std::memory_order_acquire)) return;
  SnapshotNow();
}

void TaskerMetrics::SnapshotNow()
{
  snapshot_ready.store(false, std::memory_order_release);

  delete[] snapshot_modules;
  delete[] snapshot_detailed_tasks;

  snapshot_modules = nullptr;
  snapshot_detailed_tasks = nullptr;
  snapshot_module_count = 0;

  if(!enabled || !modules || !detailed_tasks || !module_count) return;

  const size_t detailed_count = (size_t)module_count * TASKER_METRICS_DETAILED_TASK_COUNT;

  snapshot_modules = new(std::nothrow) ModuleMetrics[module_count];
  snapshot_detailed_tasks = new(std::nothrow) DetailedTaskMetrics[detailed_count];

  if(!snapshot_modules || !snapshot_detailed_tasks)
  {
    delete[] snapshot_modules;
    delete[] snapshot_detailed_tasks;

    snapshot_modules = nullptr;
    snapshot_detailed_tasks = nullptr;

    return;
  }

  memcpy(snapshot_modules, modules, sizeof(ModuleMetrics) * module_count);
  memcpy(snapshot_detailed_tasks, detailed_tasks, sizeof(DetailedTaskMetrics) * detailed_count);

  snapshot_module_count = module_count;
  snapshot_capture_age_ms = GetCaptureAgeMs();

  snapshot_ready.store(true, std::memory_order_release);
}

#endif