#pragma once

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST

#include <vector>
#include <Arduino.h>
#include <stdint.h>

struct WebUrlEntry
{
  String url;
  int method;
  uint16_t port;

  WebUrlEntry(const String& url_, int method_, uint16_t port_ = 80)
    : url(url_), method(method_), port(port_) {}
};

class WebUrlTracker
{
public:
  std::vector<WebUrlEntry> urls;

  void Add(const char* url, int method, uint16_t port = 80)
  {
    urls.emplace_back(String(url), method, port);
  }

  void Add(const String& url, int method, uint16_t port = 80)
  {
    urls.emplace_back(url, method, port);
  }

  void Clear()
  {
    urls.clear();
  }
};

extern WebUrlTracker gWebUrlTracker;

#define AddURLtoList(url, method)              gWebUrlTracker.Add((url), (method), 80)
#define AddURLtoListwPort(url, method, port)   gWebUrlTracker.Add((url), (method), (port))

#else

#define AddURLtoList(url, method)              do {} while(0)
#define AddURLtoListwPort(url, method, port)   do {} while(0)

#endif