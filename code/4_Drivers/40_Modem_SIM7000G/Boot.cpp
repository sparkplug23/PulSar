#include "_mSIM7000G.h"

#ifdef USE_MODULE_DRIVERS_MODEM_7000G



bool mSIM7000G::Modem_OpenUart(uint32_t baud)
{
  WDT_Reset();

  SerialAT.end();
  delay(UART_REOPEN_DELAY_MS);

  SerialAT.begin(baud, SERIAL_8N1, PIN_RX, PIN_TX);
  SerialAT.setTimeout(AT_STREAM_TIMEOUT_MS);

  delay(UART_SETTLE_MS);

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "UART open baud=%lu rx=%u tx=%u"),
    (unsigned long)baud,
    PIN_RX,
    PIN_TX
  );

  return true;
}


void mSIM7000G::Modem_FlushUartRx(uint32_t drain_ms)
{
  const uint32_t t0 = millis();

  while ((uint32_t)(millis() - t0) < drain_ms)
  {
    WDT_Reset();

    while (SerialAT.available())
    {
      (void)SerialAT.read();
    }

    delay(1);
  }
}

void mSIM7000G::Modem_PulsePowerKey(uint32_t hold_ms)
{
  WDT_Reset();

  ALOG_WRN(
    PSTR(D_LOG_CELLULAR "PWRKEY pulse hold=%lu ms"),
    (unsigned long)hold_ms
  );

  pinMode(PWR_PIN, OUTPUT);

  digitalWrite(PWR_PIN, LOW);
  delay(hold_ms);

  digitalWrite(PWR_PIN, HIGH);
  delay(MODEM_PWRKEY_SETTLE_MS);
}

bool mSIM7000G::Modem_ATProbe(uint16_t timeout_ms)
{
  if (!modem)
  {
    return false;
  }

  WDT_Reset();

  Modem_FlushUartRx(5);

  const bool ok = modem->testAT(timeout_ms);

  if (ok)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "AT probe OK timeout=%u"), timeout_ms);
  }

  return ok;
}

bool mSIM7000G::Modem_FindResponsiveBaud(uint32_t* found_baud)
{
  if (!found_baud)
  {
    return false;
  }

  static const uint32_t baud_list[] =
  {
    MODEM_TARGET_BAUD,
    115200,
    230400,
    // 460800,    
    9600,
    57600,
    38400,
    19200
    // ,921600
  };

  const uint8_t baud_count = sizeof(baud_list) / sizeof(baud_list[0]);

  for (uint8_t baud_i = 0; baud_i < baud_count; baud_i++)
  {
    const uint32_t test_baud = baud_list[baud_i];

    // Skip duplicate target/115200 if same.
    bool duplicate = false;
    for (uint8_t prev_i = 0; prev_i < baud_i; prev_i++)
    {
      if (baud_list[prev_i] == test_baud)
      {
        duplicate = true;
        break;
      }
    }

    if (duplicate)
    {
      continue;
    }

    WDT_Reset();

    ALOG_INF(
      PSTR(D_LOG_CELLULAR "Baud probe: %lu"),
      (unsigned long)test_baud
    );

    Modem_OpenUart(test_baud);

    if (Modem_ATProbe(AT_PROBE_TIMEOUT_FAST_MS))
    {
      *found_baud = test_baud;

      ALOG_INF(
        PSTR(D_LOG_CELLULAR "Baud probe success: %lu"),
        (unsigned long)test_baud
      );

      return true;
    }
  }

  // One slower pass at target baud only, useful during boot settling.
  Modem_OpenUart(MODEM_TARGET_BAUD);

  if (Modem_ATProbe(AT_PROBE_TIMEOUT_SLOW_MS))
  {
    *found_baud = MODEM_TARGET_BAUD;

    ALOG_INF(
      PSTR(D_LOG_CELLULAR "Baud probe success slow target: %lu"),
      (unsigned long)MODEM_TARGET_BAUD
    );

    return true;
  }

  ALOG_WRN(PSTR(D_LOG_CELLULAR "Baud probe failed"));

  return false;
}

bool mSIM7000G::Modem_LockBaud(uint32_t current_baud, uint32_t target_baud)
{
  if (!modem)
  {
    return false;
  }

  WDT_Reset();

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Lock modem baud current=%lu target=%lu"),
    (unsigned long)current_baud,
    (unsigned long)target_baud
  );

  // Ensure UART is currently open at the baud where AT responds.
  Modem_OpenUart(current_baud);

  if (!Modem_ATProbe(AT_PROBE_TIMEOUT_SLOW_MS))
  {
    ALOG_ERR(
      PSTR(D_LOG_CELLULAR "Lock baud failed: no AT at current baud %lu"),
      (unsigned long)current_baud
    );

    return false;
  }

  // Disable echo first, makes later parsing cleaner.
  modem->sendAT("E0");
  modem->waitResponse(AT_CFG_TIMEOUT_MS);

  // Verbose CME errors.
  modem->sendAT("+CMEE=2");
  modem->waitResponse(AT_CFG_TIMEOUT_MS);

  // Set fixed UART baud.
  modem->sendAT("+IPR=", target_baud);

  const int8_t ipr_rc = modem->waitResponse(AT_CFG_TIMEOUT_MS);

  if (ipr_rc != 1)
  {
    ALOG_WRN(
      PSTR(D_LOG_CELLULAR "AT+IPR response not OK rc=%d, continuing to reopen target"),
      ipr_rc
    );
  }

  delay(250);

  // Reopen ESP32 UART at target baud regardless, because modem may have switched.
  Modem_OpenUart(target_baud);

  if (!Modem_ATProbe(AT_PROBE_TIMEOUT_SLOW_MS))
  {
    ALOG_ERR(
      PSTR(D_LOG_CELLULAR "Lock baud failed: no AT after target reopen %lu"),
      (unsigned long)target_baud
    );

    return false;
  }

  // Reapply config after baud switch.
  modem->sendAT("E0");
  modem->waitResponse(AT_CFG_TIMEOUT_MS);

  modem->sendAT("+CMEE=2");
  modem->waitResponse(AT_CFG_TIMEOUT_MS);

  // Save profile.
  modem->sendAT("&W");
  modem->waitResponse(AT_CFG_TIMEOUT_MS);

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Modem baud locked and verified at %lu"),
    (unsigned long)target_baud
  );

  return true;
}


bool mSIM7000G::Modem_EnsurePowerOnAndBaud(uint32_t target_baud)
{
  uint32_t found_baud = 0;

  ALOG_WRN(
    PSTR(D_LOG_CELLULAR "Ensure modem power+baud begin target=%lu"),
    (unsigned long)target_baud
  );

  flag_modem_initialized = false;
  modem_sm_.ready = false;
  modem_sm_.busy = true;
  modem_probe_.at_ok = false;

  if (gsm_client)
  {
    gsm_client->stop();
    delete gsm_client;
    gsm_client = nullptr;
  }

  // --------------------------------------------------------------------------
  // Pass 1:
  // Do not touch power. If AT responds, modem is already on.
  // --------------------------------------------------------------------------
  ALOG_INF(PSTR(D_LOG_CELLULAR "Ensure pass 1: baud scan without power toggle"));

  if (Modem_FindResponsiveBaud(&found_baud))
  {
    ALOG_INF(
      PSTR(D_LOG_CELLULAR "Modem already responsive at baud=%lu"),
      (unsigned long)found_baud
    );

    if (Modem_LockBaud(found_baud, target_baud))
    {
      return true;
    }
  }

  // --------------------------------------------------------------------------
  // Pass 2:
  // No AT response. Pulse PWRKEY once, wait for boot, scan again.
  // If modem was off, this should turn it on.
  // --------------------------------------------------------------------------
  ALOG_WRN(PSTR(D_LOG_CELLULAR "Ensure pass 2: PWRKEY pulse then boot wait"));

  SerialAT.end();
  delay(250);

  Modem_PulsePowerKey(MODEM_PWRKEY_HOLD_MS);

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Waiting modem boot %lu ms"),
    (unsigned long)MODEM_BOOT_WAIT_MS
  );

  delay(MODEM_BOOT_WAIT_MS);

  if (Modem_FindResponsiveBaud(&found_baud))
  {
    ALOG_INF(
      PSTR(D_LOG_CELLULAR "Modem responsive after first PWRKEY pulse at baud=%lu"),
      (unsigned long)found_baud
    );

    if (Modem_LockBaud(found_baud, target_baud))
    {
      return true;
    }
  }

  // --------------------------------------------------------------------------
  // Pass 3:
  // Because PWRKEY is toggle-like, if pass 2 accidentally turned a wedged/on
  // modem off, this second pulse turns it back on.
  // --------------------------------------------------------------------------
  ALOG_WRN(PSTR(D_LOG_CELLULAR "Ensure pass 3: second PWRKEY pulse then boot wait"));

  SerialAT.end();
  delay(250);

  Modem_PulsePowerKey(MODEM_PWRKEY_HOLD_MS);

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Waiting modem boot %lu ms"),
    (unsigned long)MODEM_BOOT_WAIT_MS
  );

  delay(MODEM_BOOT_WAIT_MS);

  if (Modem_FindResponsiveBaud(&found_baud))
  {
    ALOG_INF(
      PSTR(D_LOG_CELLULAR "Modem responsive after second PWRKEY pulse at baud=%lu"),
      (unsigned long)found_baud
    );

    if (Modem_LockBaud(found_baud, target_baud))
    {
      return true;
    }
  }

  ALOG_ERR(PSTR(D_LOG_CELLULAR "Ensure modem power+baud FAILED"));

  return false;
}



#endif