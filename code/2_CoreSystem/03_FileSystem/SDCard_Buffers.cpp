#include "mFileSystem.h"

/************************************************************************************************
 * FILE: SDCard_Buffers.cpp
 *
 * PURPOSE:
 * - Filesystem-owned high-rate SD writer.
 * - Producers push bytes/lines here.
 * - This file owns:
 *   - active SD File handle
 *   - ringbuffer
 *   - writer task
 *   - flush/close policy
 *   - dropped-byte counters
 *
 * IMPORTANT:
 * - SD log filenames must be flat.
 * - Valid input names:
 *     "sd_log_test.txt"
 *     "/sd_log_test.txt"
 *     "/sd/sd_log_test.txt"
 *
 * - Internally, SDCard_Open() still receives the virtual path:
 *     /sd/sd_log_test.txt
 *
 * - Actual file on SD root is:
 *     /sd_log_test.txt
 *
 * Date Modified: 17May26
 ************************************************************************************************/

#if defined(USE_MODULE_FILESYSTEM_SDCARD) && defined(USE_FILESYSTEM_SDCARD_BUFFERS)


/************************************************************************************************
 * SECTION: SD LOG INITIALISATION
 ************************************************************************************************/

bool mFileSystem::SDLog_Init()
{
  if (sdlog.initialised) {
    return true;
  }

  sdlog.status = SDLOG_STATUS_IDLE;

  sdlog.ringbuffer_handle = xRingbufferCreate(
    FILESYSTEM_SDLOG_RINGBUFFER_SIZE,
    RINGBUF_TYPE_BYTEBUF
  );

  if (sdlog.ringbuffer_handle == nullptr)
  {
    ALOG_ERR(PSTR(D_LOG_FILESYSTEM "SDLog_Init: xRingbufferCreate failed"));
    sdlog.status = SDLOG_STATUS_ERROR;
    return false;
  }

  sdlog.file_mutex = xSemaphoreCreateMutex();

  if (sdlog.file_mutex == nullptr)
  {
    ALOG_ERR(PSTR(D_LOG_FILESYSTEM "SDLog_Init: xSemaphoreCreateMutex failed"));
    sdlog.status = SDLOG_STATUS_ERROR;
    return false;
  }

  BaseType_t task_ok = xTaskCreatePinnedToCore(
    SDLog_WriterTask_Trampoline,
    "SDLogWriter",
    FILESYSTEM_SDLOG_WRITER_STACK_SIZE,
    this,
    FILESYSTEM_SDLOG_WRITER_PRIORITY,
    &sdlog.writer_task_handle,
    FILESYSTEM_SDLOG_WRITER_CORE
  );

  if (task_ok != pdPASS)
  {
    ALOG_ERR(PSTR(D_LOG_FILESYSTEM "SDLog_Init: writer task create failed"));
    sdlog.status = SDLOG_STATUS_ERROR;
    return false;
  }

  sdlog.task_started = true;
  sdlog.initialised = true;

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDLog_Init: OK ringbuffer=%u chunk=%u"),
    (unsigned)FILESYSTEM_SDLOG_RINGBUFFER_SIZE,
    (unsigned)FILESYSTEM_SDLOG_CHUNK_SIZE
  );

  return true;
}


/************************************************************************************************
 * SECTION: SD LOG OPEN / CLOSE
 ************************************************************************************************/

bool mFileSystem::SDLog_Open(const char* path_in, bool append)
{
  if (!path_in) {
    return false;
  }

  /************************************************************************************************
   * SECTION: FLAT FILENAME NORMALISATION
   *
   * Accepted:
   * - "sd_log_test.txt"
   * - "/sd_log_test.txt"
   * - "/sd/sd_log_test.txt"
   *
   * Rejected:
   * - "/sd/log/file.txt"
   * - "log/file.txt"
   *
   * SDCard_Open() requires the virtual SD path:
   * - "/sd/sd_log_test.txt"
   ************************************************************************************************/

  const char* flat_name = path_in;

  if (strncmp(path_in, "/sd/", 4) == 0)
  {
    flat_name = path_in + 4;
  }
  else
  if (path_in[0] == '/')
  {
    flat_name = path_in + 1;
  }

  if (!flat_name || !flat_name[0])
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Open: empty filename path=%s"), path_in);
    return false;
  }

  if (strchr(flat_name, '/') != nullptr)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Open: nested filename rejected path=%s"), path_in);
    return false;
  }

  char sd_virtual_path[128];
  snprintf_P(
    sd_virtual_path,
    sizeof(sd_virtual_path),
    PSTR("/sd/%s"),
    flat_name
  );

  if (!SDCard_IsMounted())
  {
    if (!SDCard_Mount())
    {
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Open: SD mount failed"));
      return false;
    }
  }

  if (!SDLog_Init()) {
    return false;
  }

  if (SDLog_IsOpen())
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Open: already open path=%s"), sdlog.active_path);
    return false;
  }

  if (xSemaphoreTake(sdlog.file_mutex, pdMS_TO_TICKS(1000)) != pdTRUE)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Open: mutex timeout"));
    return false;
  }

  memset(sdlog.active_path, 0, sizeof(sdlog.active_path));
  strlcpy(sdlog.active_path, sd_virtual_path, sizeof(sdlog.active_path));

  sdlog.file = SDCard_Open(sd_virtual_path, append ? FILE_APPEND : FILE_WRITE);

  if (!sdlog.file)
  {
    xSemaphoreGive(sdlog.file_mutex);

    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "SDLog_Open: file open failed input=%s virtual=%s"),
      path_in,
      sd_virtual_path
    );

    sdlog.status = SDLOG_STATUS_ERROR;
    return false;
  }

  sdlog.bytes_written = 0;
  sdlog.bytes_queued = 0;
  sdlog.bytes_dropped = 0;
  sdlog.write_failures = 0;
  sdlog.last_write_ms = millis();
  sdlog.last_flush_ms = millis();

  sdlog.open_count++;
  sdlog.status = SDLOG_STATUS_OPEN;

  xSemaphoreGive(sdlog.file_mutex);

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDLog_Open: input=%s virtual=%s"),
    path_in,
    sd_virtual_path
  );

  return true;
}


bool mFileSystem::SDLog_IsOpen() const
{
  return (sdlog.status == SDLOG_STATUS_OPEN);
}


bool mFileSystem::SDLog_Close()
{
  if (!sdlog.initialised) {
    return true;
  }

  if (
    (sdlog.status != SDLOG_STATUS_OPEN) &&
    (sdlog.status != SDLOG_STATUS_CLOSE_REQUESTED)
  ) {
    return true;
  }

  sdlog.status = SDLOG_STATUS_CLOSE_REQUESTED;

  uint32_t t_start = millis();

  while (sdlog.status == SDLOG_STATUS_CLOSE_REQUESTED)
  {
    delay(10);

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
    #endif

    if ((millis() - t_start) > FILESYSTEM_SDLOG_CLOSE_TIMEOUT_MS)
    {
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLog_Close: timeout, forcing close"));

      if (xSemaphoreTake(sdlog.file_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
      {
        if (sdlog.file)
        {
          sdlog.file.flush();
          sdlog.file.close();
        }

        sdlog.status = SDLOG_STATUS_CLOSED;
        sdlog.close_count++;

        xSemaphoreGive(sdlog.file_mutex);
      }

      break;
    }
  }

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDLog_Close: path=%s written=%u queued=%u dropped=%u failures=%u"),
    sdlog.active_path,
    (unsigned)sdlog.bytes_written,
    (unsigned)sdlog.bytes_queued,
    (unsigned)sdlog.bytes_dropped,
    (unsigned)sdlog.write_failures
  );

  return true;
}


bool mFileSystem::SDLog_Flush()
{
  if (!sdlog.initialised) {
    return false;
  }

  if (xSemaphoreTake(sdlog.file_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
    return false;
  }

  bool ok = false;

  if (sdlog.file)
  {
    sdlog.file.flush();
    sdlog.last_flush_ms = millis();
    ok = true;
  }

  xSemaphoreGive(sdlog.file_mutex);

  return ok;
}


/************************************************************************************************
 * SECTION: SD LOG WRITE API
 ************************************************************************************************/

bool mFileSystem::SDLog_Write(const uint8_t* data, size_t len, uint32_t timeout_ms)
{
  if (!data || !len) {
    return false;
  }

  if (!sdlog.initialised || (sdlog.status != SDLOG_STATUS_OPEN)) {
    return false;
  }

  BaseType_t res = xRingbufferSend(
    sdlog.ringbuffer_handle,
    data,
    len,
    pdMS_TO_TICKS(timeout_ms)
  );

  if (res != pdTRUE)
  {
    sdlog.bytes_dropped += len;
    return false;
  }

  sdlog.bytes_queued += len;
  return true;
}


bool mFileSystem::SDLog_Write(const char* data, uint32_t timeout_ms)
{
  if (!data) {
    return false;
  }

  return SDLog_Write(
    reinterpret_cast<const uint8_t*>(data),
    strlen(data),
    timeout_ms
  );
}


bool mFileSystem::SDLog_WriteLine(const char* line, uint32_t timeout_ms)
{
  if (!line) {
    return false;
  }

  if (!SDLog_Write(line, timeout_ms)) {
    return false;
  }

  const char newline = '\n';

  return SDLog_Write(
    reinterpret_cast<const uint8_t*>(&newline),
    1,
    timeout_ms
  );
}


/************************************************************************************************
 * SECTION: SD LOG WRITER TASK
 ************************************************************************************************/

void mFileSystem::SDLog_WriterTask_Trampoline(void* param)
{
  mFileSystem* self = static_cast<mFileSystem*>(param);

  if (!self)
  {
    vTaskDelete(nullptr);
    return;
  }

  self->SDLog_WriterTask();
}


void mFileSystem::SDLog_WriterTask()
{
  while (true)
  {
    vTaskDelay(1);

    if (!sdlog.initialised) {
      continue;
    }

    if (
      (sdlog.status != SDLOG_STATUS_OPEN) &&
      (sdlog.status != SDLOG_STATUS_CLOSE_REQUESTED)
    ) {
      continue;
    }

    size_t item_size = 0;

    uint8_t* item = static_cast<uint8_t*>(
      xRingbufferReceiveUpTo(
        sdlog.ringbuffer_handle,
        &item_size,
        pdMS_TO_TICKS(20),
        FILESYSTEM_SDLOG_CHUNK_SIZE
      )
    );

    if (item != nullptr)
    {
      if (xSemaphoreTake(sdlog.file_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
      {
        if (sdlog.file && (sdlog.status != SDLOG_STATUS_CLOSED))
        {
          size_t written = sdlog.file.write(item, item_size);

          if (written == item_size)
          {
            sdlog.bytes_written += written;
            sdlog.last_write_ms = millis();
          }
          else
          {
            sdlog.write_failures++;
            sdlog.bytes_dropped += (item_size - written);
          }
        }
        else
        {
          sdlog.write_failures++;
          sdlog.bytes_dropped += item_size;
        }

        xSemaphoreGive(sdlog.file_mutex);
      }
      else
      {
        sdlog.write_failures++;
        sdlog.bytes_dropped += item_size;
      }

      vRingbufferReturnItem(sdlog.ringbuffer_handle, item);
      continue;
    }

    /************************************************************************************************
     * SECTION: CLOSE HANDLING
     *
     * Close only happens after the writer has drained pending queued bytes.
     ************************************************************************************************/

    if (sdlog.status == SDLOG_STATUS_CLOSE_REQUESTED)
    {
      if (xSemaphoreTake(sdlog.file_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
      {
        if (sdlog.file)
        {
          sdlog.file.flush();
          sdlog.file.close();
        }

        sdlog.close_count++;
        sdlog.status = SDLOG_STATUS_CLOSED;

        xSemaphoreGive(sdlog.file_mutex);
      }
    }
  }
}


/************************************************************************************************
 * SECTION: DEV TEST, FAST SD LOGGING
 ************************************************************************************************/

#ifdef ENABLE_DEVFEATURE_FILESYSTEM__SDCARD_LOGGING_PERFORMANCE_TEST
void mFileSystem::Loop__DevTest__FastSDLogging()
{
  /************************************************************************************************
   * SECTION: LOCAL TEST OPTIONS
   *
   * Purpose:
   * - Test the existing SDLog_* API end-to-end.
   * - No direct File.write().
   * - No extra producer buffer.
   * - Existing SDLog implementation owns queueing/writer-task/flush/close behaviour.
   *
   * Filename is intentionally flat.
   ************************************************************************************************/

  static bool enable_sdlog_test = true;

  const uint32_t boot_wait_secs           = 180;
  const uint32_t test_duration_ms         = 60000UL;

  /*
   * 0 = write once every loop.
   * >0 = rate-limit producer side.
   */
  const uint32_t write_period_ms          = 0;

  /*
   * Optional explicit flush call.
   * 0 = never call SDLog_Flush() during test, only SDLog_Close() at the end.
   */
  const uint32_t forced_flush_period_ms   = 0;

  /*
   * Timeout passed into SDLog_WriteLine().
   * 0 = non-blocking queue attempt.
   */
  const uint32_t write_timeout_ms         = 0;

  /*
   * Flat filename.
   * SDLog_Open() converts this internally to:
   *   /sd/sd_log_test.txt
   */
  const char* test_path                   = "sd_log_test.txt";


  /************************************************************************************************
   * SECTION: LOCAL STATE
   ************************************************************************************************/

  enum TestState : uint8_t
  {
    TEST_IDLE = 0,
    TEST_WAIT_BOOT,
    TEST_OPEN,
    TEST_RUNNING,
    TEST_FINALISE,
    TEST_DONE,
    TEST_ERROR
  };

  static TestState state = TEST_IDLE;

  static uint32_t boot_ms = 0;
  static uint32_t test_start_ms = 0;
  static uint32_t last_write_ms = 0;
  static uint32_t last_flush_ms = 0;
  static uint32_t last_loop_ms = 0;

  static uint32_t lines_attempted = 0;
  static uint32_t lines_accepted = 0;
  static uint32_t lines_failed = 0;

  static uint32_t forced_flush_count = 0;
  static uint32_t forced_flush_fail_count = 0;

  static uint32_t generated_bytes_est = 0;

  static uint32_t api_write_time_total_us = 0;
  static uint32_t api_write_time_min_us = 0xFFFFFFFFUL;
  static uint32_t api_write_time_max_us = 0;

  static uint32_t api_flush_time_total_us = 0;
  static uint32_t api_flush_time_min_us = 0xFFFFFFFFUL;
  static uint32_t api_flush_time_max_us = 0;

  static uint32_t max_loop_dt_ms = 0;

  static uint32_t start_bytes_written = 0;
  static uint32_t start_bytes_queued = 0;
  static uint32_t start_bytes_dropped = 0;
  static uint32_t start_write_failures = 0;


  /************************************************************************************************
   * SECTION: EXIT CONDITIONS
   ************************************************************************************************/

  if (!enable_sdlog_test) {
    return;
  }

  if (state == TEST_DONE || state == TEST_ERROR) {
    return;
  }


  /************************************************************************************************
   * SECTION: ARM TEST
   ************************************************************************************************/

  if (state == TEST_IDLE)
  {
    boot_ms = millis();
    state = TEST_WAIT_BOOT;

    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "SDLogPerfTest: armed boot_wait_secs=%u duration_ms=%u write_period_ms=%u forced_flush_period_ms=%u"),
      (unsigned)boot_wait_secs,
      (unsigned)test_duration_ms,
      (unsigned)write_period_ms,
      (unsigned)forced_flush_period_ms
    );

    return;
  }


  /************************************************************************************************
   * SECTION: WAIT AFTER BOOT
   ************************************************************************************************/

  if (state == TEST_WAIT_BOOT)
  {
    if ((millis() - boot_ms) < (boot_wait_secs * 1000UL)) {
      return;
    }

    state = TEST_OPEN;
  }


  /************************************************************************************************
   * SECTION: OPEN EXISTING SD LOGGING PATH
   ************************************************************************************************/

  if (state == TEST_OPEN)
  {
    if (!SDCard_IsMounted())
    {
      if (!SDCard_Mount())
      {
        ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLogPerfTest: SD not mounted"));
        return;
      }
    }

    if (!SDLog_Open(test_path, false))
    {
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDLogPerfTest: SDLog_Open failed path=%s"), test_path);
      return;
    }

    lines_attempted = 0;
    lines_accepted = 0;
    lines_failed = 0;

    forced_flush_count = 0;
    forced_flush_fail_count = 0;

    generated_bytes_est = 0;

    api_write_time_total_us = 0;
    api_write_time_min_us = 0xFFFFFFFFUL;
    api_write_time_max_us = 0;

    api_flush_time_total_us = 0;
    api_flush_time_min_us = 0xFFFFFFFFUL;
    api_flush_time_max_us = 0;

    max_loop_dt_ms = 0;

    start_bytes_written = SDLog_GetBytesWritten();
    start_bytes_queued = SDLog_GetBytesQueued();
    start_bytes_dropped = SDLog_GetBytesDropped();
    start_write_failures = SDLog_GetWriteFailures();

    test_start_ms = millis();
    last_write_ms = test_start_ms;
    last_flush_ms = test_start_ms;
    last_loop_ms = test_start_ms;

    SDLog_WriteLine("SDLogPerfTest BEGIN", 1000);
    SDLog_WriteLine("Mode=Existing SDLog API only", 1000);
    SDLog_WriteLine("FlatName=sd_log_test.txt", 1000);
    SDLog_WriteLine("VirtualPath=/sd/sd_log_test.txt", 1000);

    ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDLogPerfTest: started flat=%s"), test_path);

    state = TEST_RUNNING;
  }


  /************************************************************************************************
   * SECTION: RUN TEST
   ************************************************************************************************/

  if (state == TEST_RUNNING)
  {
    const uint32_t now_ms = millis();

    const uint32_t loop_dt_ms = now_ms - last_loop_ms;
    last_loop_ms = now_ms;

    if (loop_dt_ms > max_loop_dt_ms) {
      max_loop_dt_ms = loop_dt_ms;
    }

    if (write_period_ms > 0)
    {
      if ((now_ms - last_write_ms) < write_period_ms) {
        return;
      }
    }

    last_write_ms = now_ms;

    char line[160];

    const int line_len_i = snprintf_P(
      line,
      sizeof(line),
      PSTR("Test Sequence millis %lu bytes %lu line %lu queued %lu written %lu dropped %lu failures %lu loopdt %lu"),
      (unsigned long)now_ms,
      (unsigned long)generated_bytes_est,
      (unsigned long)lines_attempted,
      (unsigned long)SDLog_GetBytesQueued(),
      (unsigned long)SDLog_GetBytesWritten(),
      (unsigned long)SDLog_GetBytesDropped(),
      (unsigned long)SDLog_GetWriteFailures(),
      (unsigned long)loop_dt_ms
    );

    uint16_t line_len = 0;

    if (line_len_i > 0)
    {
      line_len = (uint16_t)line_len_i;

      if (line_len >= sizeof(line)) {
        line_len = sizeof(line) - 1;
      }
    }

    lines_attempted++;
    generated_bytes_est += line_len + 1;

    const uint32_t t0_us = micros();

    const bool ok = SDLog_WriteLine(line, write_timeout_ms);

    const uint32_t dt_us = micros() - t0_us;

    api_write_time_total_us += dt_us;

    if (dt_us < api_write_time_min_us) {
      api_write_time_min_us = dt_us;
    }

    if (dt_us > api_write_time_max_us) {
      api_write_time_max_us = dt_us;
    }

    if (ok) {
      lines_accepted++;
    } else {
      lines_failed++;
    }


    /************************************************************************************************
     * SECTION: OPTIONAL EXPLICIT FLUSH TEST
     ************************************************************************************************/

    if (forced_flush_period_ms > 0)
    {
      if ((now_ms - last_flush_ms) >= forced_flush_period_ms)
      {
        const uint32_t flush_t0_us = micros();

        const bool flush_ok = SDLog_Flush();

        const uint32_t flush_dt_us = micros() - flush_t0_us;

        forced_flush_count++;
        api_flush_time_total_us += flush_dt_us;

        if (flush_dt_us < api_flush_time_min_us) {
          api_flush_time_min_us = flush_dt_us;
        }

        if (flush_dt_us > api_flush_time_max_us) {
          api_flush_time_max_us = flush_dt_us;
        }

        if (!flush_ok) {
          forced_flush_fail_count++;
        }

        last_flush_ms = now_ms;
      }
    }

    if ((now_ms - test_start_ms) >= test_duration_ms) {
      state = TEST_FINALISE;
    }

    return;
  }


  /************************************************************************************************
   * SECTION: FINALISE
   ************************************************************************************************/

  if (state == TEST_FINALISE)
  {
    const uint32_t end_ms = millis();
    const uint32_t elapsed_ms = end_ms - test_start_ms;

    const uint32_t end_bytes_queued_before_summary = SDLog_GetBytesQueued();
    const uint32_t end_bytes_written_before_summary = SDLog_GetBytesWritten();
    const uint32_t end_bytes_dropped_before_summary = SDLog_GetBytesDropped();
    const uint32_t end_write_failures_before_summary = SDLog_GetWriteFailures();

    const uint32_t delta_queued =
      end_bytes_queued_before_summary - start_bytes_queued;

    const uint32_t delta_written =
      end_bytes_written_before_summary - start_bytes_written;

    const uint32_t delta_dropped =
      end_bytes_dropped_before_summary - start_bytes_dropped;

    const uint32_t delta_write_failures =
      end_write_failures_before_summary - start_write_failures;

    const uint32_t api_write_avg_us =
      lines_attempted ? (api_write_time_total_us / lines_attempted) : 0;

    const uint32_t forced_flush_avg_us =
      forced_flush_count ? (api_flush_time_total_us / forced_flush_count) : 0;

    const uint32_t producer_bytes_per_sec =
      elapsed_ms ? ((generated_bytes_est * 1000UL) / elapsed_ms) : 0;

    const uint32_t writer_bytes_per_sec =
      elapsed_ms ? ((delta_written * 1000UL) / elapsed_ms) : 0;

    char summary[1150];

    snprintf_P(
      summary,
      sizeof(summary),
      PSTR(
        "\n"
        "SDLogPerfTest SUMMARY\n"
        "ElapsedMs=%lu\n"
        "WritePeriodMs=%lu\n"
        "WriteTimeoutMs=%lu\n"
        "LinesAttempted=%lu\n"
        "LinesAccepted=%lu\n"
        "LinesFailed=%lu\n"
        "GeneratedBytesEst=%lu\n"
        "ProducerBytesPerSec=%lu\n"
        "ApiWriteTimeTotalUs=%lu\n"
        "ApiWriteTimeAvgUs=%lu\n"
        "ApiWriteTimeMinUs=%lu\n"
        "ApiWriteTimeMaxUs=%lu\n"
        "ForcedFlushPeriodMs=%lu\n"
        "ForcedFlushCount=%lu\n"
        "ForcedFlushFailCount=%lu\n"
        "ForcedFlushTimeTotalUs=%lu\n"
        "ForcedFlushTimeAvgUs=%lu\n"
        "ForcedFlushTimeMinUs=%lu\n"
        "ForcedFlushTimeMaxUs=%lu\n"
        "MaxLoopDtMs=%lu\n"
        "SDLogQueuedDelta=%lu\n"
        "SDLogWrittenDelta=%lu\n"
        "SDLogDroppedDelta=%lu\n"
        "SDLogWriteFailuresDelta=%lu\n"
        "SDLogWriterBytesPerSec=%lu\n"
        "FreeHeap=%lu\n"
        "MinFreeHeap=%lu\n"
        "SDLogPerfTest END\n"
      ),
      (unsigned long)elapsed_ms,
      (unsigned long)write_period_ms,
      (unsigned long)write_timeout_ms,
      (unsigned long)lines_attempted,
      (unsigned long)lines_accepted,
      (unsigned long)lines_failed,
      (unsigned long)generated_bytes_est,
      (unsigned long)producer_bytes_per_sec,
      (unsigned long)api_write_time_total_us,
      (unsigned long)api_write_avg_us,
      (unsigned long)(api_write_time_min_us == 0xFFFFFFFFUL ? 0 : api_write_time_min_us),
      (unsigned long)api_write_time_max_us,
      (unsigned long)forced_flush_period_ms,
      (unsigned long)forced_flush_count,
      (unsigned long)forced_flush_fail_count,
      (unsigned long)api_flush_time_total_us,
      (unsigned long)forced_flush_avg_us,
      (unsigned long)(api_flush_time_min_us == 0xFFFFFFFFUL ? 0 : api_flush_time_min_us),
      (unsigned long)api_flush_time_max_us,
      (unsigned long)max_loop_dt_ms,
      (unsigned long)delta_queued,
      (unsigned long)delta_written,
      (unsigned long)delta_dropped,
      (unsigned long)delta_write_failures,
      (unsigned long)writer_bytes_per_sec,
      (unsigned long)ESP.getFreeHeap(),
      (unsigned long)ESP.getMinFreeHeap()
    );

    SDLog_WriteLine(summary, 1000);

    const uint32_t close_t0_us = micros();

    SDLog_Close();

    const uint32_t close_dt_us = micros() - close_t0_us;

    ALOG_INF(PSTR(D_LOG_FILESYSTEM "%s"), summary);
    ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDLogPerfTest close_dt_us=%lu"), (unsigned long)close_dt_us);

    Serial.println(summary);
    Serial.printf("SDLogPerfTest close_dt_us=%lu\n", (unsigned long)close_dt_us);

    state = TEST_DONE;
    enable_sdlog_test = false;

    return;
  }
}

#endif // ENABLE_DEVFEATURE_FILESYSTEM__SDCARD_LOGGING_PERFORMANCE_TEST

/************************************************************************************************
 * SECTION: SD LOG STATS
 ************************************************************************************************/

uint32_t mFileSystem::SDLog_GetBytesWritten() const
{
  return sdlog.bytes_written;
}


uint32_t mFileSystem::SDLog_GetBytesQueued() const
{
  return sdlog.bytes_queued;
}


uint32_t mFileSystem::SDLog_GetBytesDropped() const
{
  return sdlog.bytes_dropped;
}


uint32_t mFileSystem::SDLog_GetWriteFailures() const
{
  return sdlog.write_failures;
}


mFileSystem::SDLogStatus mFileSystem::SDLog_GetStatus() const
{
  return sdlog.status;
}


const char* mFileSystem::SDLog_GetActivePath() const
{
  return sdlog.active_path;
}


#endif // USE_MODULE_FILESYSTEM_SDCARD && USE_FILESYSTEM_SDCARD_BUFFERS