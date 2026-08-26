
#include "_mGPS_Serial.h"

#ifdef USE_MODULE_SENSORS_GPS_SERIAL


/******************************************************************************************************************
 * Web
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_WEBSERVER

void mGPS_Serial::WebPage_Root_AddHandlers()
{
  #ifdef ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE
  SPGM_CTR(PM_GPS_CONFIG_DEVICE) "/m/gpsconfig";
  tkr_web->server->on(PM_GPS_CONFIG_DEVICE, HTTP_ANY, [this](AsyncWebServerRequest *request){
    this->Serve_Submodule_GPSConfigDevice(request);
  });
  tkr_web->AddURLasApplication(GetModuleUniqueID(), PM_GPS_CONFIG_DEVICE, "Configuration");
  AddURLtoList(PM_GPS_CONFIG_DEVICE, 0 /*HTTP_ANY*/ );
  #endif

  #ifdef ENABLE_FEATURE_WEBSERVER__GPS_OVERVIEW
  SPGM_CTR(PM_GPS_OVERVIEW) "/m/gpsoverview";
  tkr_web->server->on(PM_GPS_OVERVIEW, HTTP_ANY, [this](AsyncWebServerRequest *request){
    this->Serve_Submodule_GPSOverview(request);
  });
  tkr_web->AddURLasApplication(GetModuleUniqueID(), PM_GPS_OVERVIEW, "Overview");
  AddURLtoList(PM_GPS_OVERVIEW, 0 /*HTTP_ANY*/);
  #endif
}

#ifdef ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE

void mGPS_Serial::Serve_Submodule_GPSConfigDevice(AsyncWebServerRequest* request)
{
  if (request->method() == HTTP_POST) {
    Serve_Submodule_GPSConfigDevice_Post(request);
    return;
  }

  if (request->url().endsWith(F("/s.js"))) {
    Serve_Submodule_GPSConfigDevice_JS(request);
    return;
  }

  Serve_Submodule_GPSConfigDevice_Page(request);
}


void mGPS_Serial::Serve_Submodule_GPSConfigDevice_Post(AsyncWebServerRequest* request)
{
  const String action = request->hasParam("A", true)
                          ? request->getParam("A", true)->value()
                          : "save";

  if (action == "detect_baud")
  {
    uint32_t found_baud = 0;

    if (GPS_FindWorkingBaud(found_baud))
    {
      packet_config.baud_current = found_baud;
      ALOG_INF(PSTR("GPS detect baud found %lu"), (unsigned long)found_baud);
    }
    else
    {
      ALOG_ERR(PSTR("GPS detect baud failed"));
    }

    request->redirect("/m/gpsconfig");
    return;
  }

  if (action == "apply_baud")
  {
    uint32_t baud_target = request->hasParam("BaudTarget", true)
                             ? request->getParam("BaudTarget", true)->value().toInt()
                             : 0;

    if (baud_target > 0)
    {
      packet_config.baud_target = baud_target;
      packet_config.pending_baud_apply = true;

      ALOG_INF(PSTR("GPS apply baud requested: %lu"), (unsigned long)baud_target);
    }
    else
    {
      ALOG_ERR(PSTR("GPS apply baud requested with invalid baud"));
    }

    request->redirect("/m/gpsconfig");
    return;
  }

  if (action == "save")
  {
    uint8_t rate = request->hasParam("Rate", true)
                    ? request->getParam("Rate", true)->value().toInt()
                    : 1;

    char key[8];

    // Update desired UBX state from form only
    for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++)
    {
      snprintf(key, sizeof(key), "UE%u", i);
      packet_config.ubx_nav[i].desired_enabled = request->hasParam(key, true);
    }

    // Update desired NMEA state from form only
    for (uint8_t i = 0; i < ((uint8_t)NMEAGPS::NMEAMSG_END - 1); i++)
    {
      snprintf(key, sizeof(key), "NE%u", i);
      packet_config.nmea[i].desired_enabled = request->hasParam(key, true);
    }

    packet_config.desired_rate = rate;
    packet_config.pending_apply = true;

    request->redirect("/m/gpsconfig");
    return;
  }

  request->redirect("/m/gpsconfig");
}


void mGPS_Serial::Serve_Submodule_GPSConfigDevice_JS(AsyncWebServerRequest* request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  response->print(F(
    "function GetV(){var d=document;"
    "window.gps_cfg_init=window.gps_cfg_init||0;"
  ));

  char buffer[96];
  char key[8];

  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetDateAndTime(DT_UTC).c_str());
  tkr_web->printSetFormValue(*response, PSTR("UT"), buffer);

  tkr_web->printSetFormValue(*response, PSTR("GPS_PARSER"), PSTR("ACTIVE"));

  snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)packet_config.baud_current);
  tkr_web->printSetFormValue(*response, PSTR("GPS_BAUD_CURRENT"), buffer);

  snprintf(buffer, sizeof(buffer), "%d", (int)fix_valid.status);
  tkr_web->printSetFormValue(*response, PSTR("GPS_FIX_STATUS"), buffer);

  snprintf(buffer, sizeof(buffer), "%u", (unsigned)fix_valid.satellites);
  tkr_web->printSetFormValue(*response, PSTR("GPS_SATS"), buffer);

  snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)(millis() - stats.last_message_received_time));
  tkr_web->printSetFormValue(*response, PSTR("GPS_LAST_AGE"), buffer);

  #ifdef ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS

  // Only apply checkbox/dropdown/rate states on first load, not every poll
  response->print(F("if(!window.gps_cfg_init){"));

    for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++)
    {
        // response->printf_P(PSTR("d.getElementById('UE%u').checked=%s;"), i,packet_config.ubx_nav[i].desired_enabled ? "true" : "false");
        char key[8];
        snprintf(key, sizeof(key), "UE%u", i);
        tkr_web->printSetFormCheckbox(*response, key, packet_config.ubx_nav[i].desired_enabled ? 1 : 0);
    }

    for (uint8_t i = 0; i < ((uint8_t)NMEAGPS::NMEAMSG_END - 1); i++)
    {
        // response->printf_P(PSTR("d.getElementById('NE%u').checked=%s;"),i,packet_config.nmea[i].desired_enabled ? "true" : "false");
        char key[8];
        snprintf(key, sizeof(key), "NE%u", i);
        tkr_web->printSetFormCheckbox(*response, key, packet_config.nmea[i].desired_enabled ? 1 : 0);
    }

    tkr_web->printSetFormValue(*response, PSTR("BaudTarget"), (int)packet_config.baud_target);

    snprintf(buffer, sizeof(buffer), "%u", (unsigned)packet_config.desired_rate);
    tkr_web->printSetFormValue(*response, PSTR("Rate"), buffer);

  response->print(F("window.gps_cfg_init=1;}"));

  // Poll-only status fields
  for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++)
  {
    ubx_msg_state_t* p = packet_metrics.FindUbxMessage(ublox::UBX_NAV, ublox::UBX_NAV_LIST[i]);

    snprintf(key, sizeof(key), "US%u", i);
    snprintf(buffer, sizeof(buffer), "%s", (p && p->count_rx) ? "Yes" : "");
    tkr_web->printSetFormValue(*response, key, buffer);

    snprintf(key, sizeof(key), "UC%u", i);
    snprintf(buffer, sizeof(buffer), "%lu", p ? (unsigned long)p->count_rx : 0UL);
    tkr_web->printSetFormValue(*response, key, buffer);

    snprintf(key, sizeof(key), "UA%u", i);
    snprintf(buffer, sizeof(buffer), "%lu", (p && p->last_seen_ms) ? (unsigned long)(millis() - p->last_seen_ms) : 0UL);
    tkr_web->printSetFormValue(*response, key, buffer);
  }

  for (uint8_t i = 0; i < ((uint8_t)NMEAGPS::NMEAMSG_END - 1); i++)
  {
    nmea_msg_state_t* p = packet_metrics.FindNmeaMessage((uint8_t)(i + 1));

    snprintf(key, sizeof(key), "NS%u", i);
    snprintf(buffer, sizeof(buffer), "%s", (p && p->count_rx) ? "Yes" : "");
    tkr_web->printSetFormValue(*response, key, buffer);

    snprintf(key, sizeof(key), "NC%u", i);
    snprintf(buffer, sizeof(buffer), "%lu", p ? (unsigned long)p->count_rx : 0UL);
    tkr_web->printSetFormValue(*response, key, buffer);

    snprintf(key, sizeof(key), "NA%u", i);
    snprintf(buffer, sizeof(buffer), "%lu", (p && p->last_seen_ms) ? (unsigned long)(millis() - p->last_seen_ms) : 0UL);
    tkr_web->printSetFormValue(*response, key, buffer);
  }

  #endif

  response->print(F("}"));
  request->send(response);
}




void mGPS_Serial::Serve_Submodule_GPSConfigDevice_Page(AsyncWebServerRequest* request)
{
  AsyncWebServerResponse* response =
    request->beginResponse_P(
      200,
      FPSTR(CONTENT_TYPE_HTML),
      WEB_5_Sensors_50_GPS_Serial_gps_config_device_htm,
      WEB_5_Sensors_50_GPS_Serial_gps_config_device_htm_length
    );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}

#endif // ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE


#ifdef ENABLE_FEATURE_WEBSERVER__GPS_OVERVIEW

void mGPS_Serial::Serve_Submodule_GPSOverview(AsyncWebServerRequest* request)
{
  if (request->method() == HTTP_POST) {
    request->redirect("/m/gpsoverview");
    return;
  }

  if (request->url().endsWith(F("/s.js"))) {
    Serve_Submodule_GPSOverview_JS(request);
    return;
  }

  Serve_Submodule_GPSOverview_Page(request);
}

void mGPS_Serial::Serve_Submodule_GPSOverview_Page(AsyncWebServerRequest* request)
{
  AsyncWebServerResponse* response =
    request->beginResponse_P(
      200,
      FPSTR(CONTENT_TYPE_HTML),
      WEB_5_Sensors_50_GPS_Serial_gps_overview_htm,
      WEB_5_Sensors_50_GPS_Serial_gps_overview_htm_length
    );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}

void mGPS_Serial::Serve_Submodule_GPSOverview_JS(AsyncWebServerRequest* request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  response->print(F(
    "function GetV(){var d=document;"
    "window.gps_overview_init=window.gps_overview_init||0;"
  ));

  char buffer[128];
  char notes[256];
  uint16_t row_i = 0;

  // ------------------------------------------------------------
  // Header / title
  // ------------------------------------------------------------
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetDateAndTime(DT_UTC).c_str());
  tkr_web->printSetFormValue(*response, PSTR("UT"), buffer);
  tkr_web->printSetElementHTML(*response, "page_title", "GPS Overview");

  // ------------------------------------------------------------
  // First-load row creation
  // ------------------------------------------------------------
  response->print(F("if(!window.gps_overview_init){"));
  tkr_web->printTableClear(*response, "gps");

  tkr_web->printTableAddRow(*response, "gps", row_i++, "Latitude",   "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Longitude",  "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "HDOP",  "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "VDOP",  "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Altitude cm", "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Altitude",    "", "m");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Speed",  "", "mph");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Satellites",  "", "");
  tkr_web->printTableAddRow(*response, "gps", row_i++, "Time",  "", "");

  
  response->print(F("window.gps_overview_init=1;}"));

  // ------------------------------------------------------------
  // Poll-only cell value updates
  // ------------------------------------------------------------
  row_i = 0;

  dtostrf(fix_valid.latitude(), 0, 7, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  dtostrf(fix_valid.longitude(), 0, 7, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i, buffer);

  snprintf(
    notes,
    sizeof(notes),
    "<a href='https://maps.google.com/?q=%.7f,%.7f' target='_blank'>Open map</a>",
    (double)fix_valid.latitude(),
    (double)fix_valid.longitude()
    );
    tkr_web->printTableSetNotes(*response, "gps", row_i, notes);
  row_i++;
  
  dtostrf(fix_valid.hdop/1000.0f, 0, 2, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);
  dtostrf(fix_valid.vdop/1000.0f, 0, 2, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  snprintf(buffer, sizeof(buffer), "%ld", (long)fix_valid.altitude_cm());
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  dtostrf(fix_valid.altitude(), 0, 2, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  dtostrf(fix_valid.speed_mph(), 0, 2, buffer);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  snprintf(buffer, sizeof(buffer), "%d", (unsigned long)fix_valid.satellites);
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%d", fix_valid.dateTime.hours, fix_valid.dateTime.minutes, fix_valid.dateTime.seconds, fix_valid.dateTime_ms());
  tkr_web->printTableSetValue(*response, "gps", row_i++, buffer);

  response->print(F("}"));
  request->send(response);
}

#endif // ENABLE_FEATURE_WEBSERVER__GPS_OVERVIEW

#endif // USE_MODULE_NETWORK_WEBSERVER

#endif // USE_MODULE_SENSORS_GPS_SERIAL