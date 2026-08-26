#pragma once


#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include "mTasks.h"


#include <atomic>
#include <new>

#include "2_CoreSystem/mBaseConfig.h"           //DEFAULTS
#include "0_ConfigUser/Secret/ConfigUser.h"

// #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

class mTaskerManager;

class TaskerMetrics
{
public:

  struct TimingStats
  {
    uint64_t total_us = 0;
    uint32_t count = 0;
    uint32_t last_us = 0;
    uint32_t min_us = UINT32_MAX;
    uint32_t max_us = 0;
    uint16_t max_task_id = 0;
  };

  struct ModuleMetrics
  {
    uint16_t module_id = 0;
    TimingStats timing;
  };

  struct DetailedTaskMetrics
  {
    uint16_t module_id = 0;
    TASKER_FUNCTION_TYPES task_id = TASKER_FUNCTION_TYPES(0);
    TimingStats timing;
  };

  TaskerMetrics() = default;
  ~TaskerMetrics();

  void RequestEnable(uint16_t expected_module_count);
  void RequestDisable();
  void RequestReset();
  void RequestSnapshot();
  void Touch();

  bool IsEnabled();
  bool IsSnapshotReady() const;
  uint32_t GetCaptureAgeMs() const;

  void Record(uint16_t module_index, uint16_t module_id, TASKER_FUNCTION_TYPES task_id, uint32_t elapsed_us);
  void WriteSnapshotJSON(Print& out, mTaskerManager& manager);

private:

  enum PendingAction : uint8_t
  {
    ACTION_NONE = 0,
    ACTION_ENABLE,
    ACTION_DISABLE,
    ACTION_RESET
  };

  bool enabled = false;
  uint16_t module_count = 0;

  uint32_t capture_started_ms = 0;
  std::atomic<uint32_t> last_client_touch_ms{0};

  static constexpr uint32_t CLIENT_LEASE_TIMEOUT_MS = 30000;

  ModuleMetrics* modules = nullptr;
  DetailedTaskMetrics* detailed_tasks = nullptr;

  ModuleMetrics* snapshot_modules = nullptr;
  DetailedTaskMetrics* snapshot_detailed_tasks = nullptr;
  uint16_t snapshot_module_count = 0;
  uint32_t snapshot_capture_age_ms = 0;

  std::atomic<bool> snapshot_requested{false};
  std::atomic<bool> snapshot_ready{false};

  std::atomic<uint8_t> pending_action{ACTION_NONE};
  std::atomic<uint16_t> pending_expected_module_count{0};

  void ProcessPendingRequest();
  void ProcessSnapshotRequest();

  void EnableNow(uint16_t expected_module_count);
  void DisableNow();
  void ResetNow();
  void SnapshotNow();

  static int8_t GetDetailedTaskIndex(TASKER_FUNCTION_TYPES task_id);
  static void UpdateTiming(TimingStats& stats, TASKER_FUNCTION_TYPES task_id, uint32_t elapsed_us);
};

// #endif
