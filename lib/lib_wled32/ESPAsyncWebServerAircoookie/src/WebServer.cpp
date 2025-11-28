/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "ESPAsyncWebServer.h"
#include "WebHandlerImpl.h"

#ifdef ASYNCWEBSERVER_DEBUG_TRACE
#define DEBUG_PRINTFP(fmt, ...) Serial.printf_P(PSTR("[%d]" fmt), (unsigned) millis(), ##__VA_ARGS__)
#else
#define DEBUG_PRINTFP(...)
#endif

#ifdef ASYNCWEBSERVER_NEEDS_MUTEX
struct guard_type {
  SemaphoreHandle_t _mtx;
  guard_type(SemaphoreHandle_t m) : _mtx(m) {
    xSemaphoreTake(_mtx, portMAX_DELAY);  // todo: error check
  }
  ~guard_type() {    
    xSemaphoreGive(_mtx);
  }
};
#define guard() const guard_type guard(_mutex)
#else
#define guard()
#endif

#ifndef ASYNCWEBSERVER_MINIMUM_ALLOC
#ifdef ESP8266
#define ASYNCWEBSERVER_MINIMUM_ALLOC 1024
#else
#define ASYNCWEBSERVER_MINIMUM_ALLOC 2048
#endif
#endif

#ifndef ASYNCWEBSERVER_MINIMUM_HEAP
#ifdef ESP8266
#define ASYNCWEBSERVER_MINIMUM_HEAP 2048
#else /* ESP32 */
// This is a *vastly* larger number.  The ESP32 TCP stack does a *ton* of dynamic
// allocation in the cricital path; and if we OOM the stack, it hangs up connections,
// leaking bits and pieces and leaving the available memory fragmented.
#define ASYNCWEBSERVER_MINIMUM_HEAP 8192
#endif
#endif


bool ON_STA_FILTER(AsyncWebServerRequest *request) {
  return WiFi.localIP() == request->client()->localIP();
}

bool ON_AP_FILTER(AsyncWebServerRequest *request) {
  return WiFi.localIP() != request->client()->localIP();
}


static bool minimal_send_503(AsyncClient* c) {
    const static char msg[] PROGMEM = "HTTP/1.1 503 Service Unavailable\r\nConnection: close\r\n";
#ifdef PROGMEM  
    char msg_stack[sizeof(msg)];  // stack, so we can pull it out of flash memory
    memcpy_P(msg_stack, msg, sizeof(msg));
    #define MSG_503 msg_stack
#else
    #define MSG_503 msg
#endif
    auto w = c->write(MSG_503, sizeof(MSG_503)-1, ASYNC_WRITE_FLAG_COPY);

    // assume any nonzero value is success
    DEBUG_PRINTFP("*** Sent 503 to %08X (%d), result %d\n", (intptr_t) c, c->getRemotePort(), w);
    if (w == 0) {    
      c->close(true); // sorry bud, we're really that strapped for ram  
    }
    return (w != 0);  
}

#ifdef ESP8266
static inline size_t get_heap_available() {
  return ESP.getFreeHeap();
}

static inline size_t get_heap_alloc() {
  return ESP.getMaxFreeBlockSize();
}
#else
// Platform functions don't correctly check for malloc()'s heap; at least on ESP32-WROVER 
// they incorrectly include some internal memory that is not accessible to malloc().
// Reimplement using the correct MALLOC_CAPs.
static inline size_t get_heap_available() {
  return heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_DEFAULT);
}

static inline size_t get_heap_alloc() {
  return heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_DEFAULT);
}
#endif


AsyncWebServer::AsyncWebServer(uint16_t port)
  : AsyncWebServer(IPADDR_ANY, port)
{
}

AsyncWebServer::AsyncWebServer(uint16_t port, const AsyncWebServerQueueLimits& limits)
  : AsyncWebServer(IPADDR_ANY, port, limits)
{
}


AsyncWebServer::AsyncWebServer(IPAddress addr, uint16_t port)
  : AsyncWebServer(addr, port, {0, 0, 0, 0})
{
}

AsyncWebServer::AsyncWebServer(IPAddress addr, uint16_t port, const AsyncWebServerQueueLimits& limits)
  : _queueLimits(limits)
  , _server(addr, port)
  , _rewrites([](AsyncWebRewrite* r){ delete r; })
  , _handlers([](AsyncWebHandler* h){ delete h; })
#ifdef ASYNCWEBSERVER_NEEDS_MUTEX
  , _mutex(xSemaphoreCreateMutex())
#endif
  , _requestQueue(LinkedList<AsyncWebServerRequest*>::OnRemove {})
  , _queueActive(false)
{
  _catchAllHandler = new AsyncCallbackWebHandler();
  if(_catchAllHandler == NULL)
    return;
  _server.onClient([this](void *s, AsyncClient* c){
    if(c == NULL)
      return;

    auto heap_avail = get_heap_available();
    auto heap_alloc = get_heap_alloc();

    if ((heap_avail < ASYNCWEBSERVER_MINIMUM_HEAP)
        || (heap_alloc < ASYNCWEBSERVER_MINIMUM_ALLOC)) {
      // Protect ourselves from crashing - just abandon this request.
      DEBUG_PRINTFP("*** Dropping client %08X (%d): %d, %d/%d\n", (intptr_t) c, c->getRemotePort(), _requestQueue.length(), heap_alloc, heap_avail);
      c->close(true);
      delete c;
      return;
    }

    guard();
    auto queue_length = _requestQueue.length();

    if (((_queueLimits.nMax > 0) && (queue_length >= _queueLimits.nMax))
        || ((_queueLimits.queueHeapRequired > 0) && (heap_avail < _queueLimits.queueHeapRequired))
    ) {
      // Don't even allocate anything we can avoid.  Tell the client we're in trouble with a static response.
      DEBUG_PRINTFP("*** Rejecting client %08X (%d): %d, %d/%d\n", (intptr_t) c, c->getRemotePort(), _requestQueue.length(), heap_alloc, heap_avail);
      c->setNoDelay(true);
      c->onDisconnect([](void*r, AsyncClient* rc){
        DEBUG_PRINTFP("*** Client %08X (%d) disconnected\n", (intptr_t)rc, rc->getRemotePort());
        delete rc;  // There is almost certainly something wrong with this - it's not OK to delete a function object while it's running
      });
      c->onAck([](void *, AsyncClient* rc, size_t s, uint32_t ){  
        if (s) rc->close(true);
      });
      c->onData([](void*, AsyncClient* rc, void*, size_t){
        rc->onData({});
        minimal_send_503(rc);
      });
      return;
    }

    c->setRxTimeout(3);

    AsyncWebServerRequest *r = new AsyncWebServerRequest((AsyncWebServer*)s, c);
    if(r == NULL){
      c->close(true);
      c->free();
      delete c;
      return;
    }
    
    
    _requestQueue.add(r);
  }, this);
}

AsyncWebServer::~AsyncWebServer(){
  reset();  
  end();
  if(_catchAllHandler) delete _catchAllHandler;
}

AsyncWebRewrite& AsyncWebServer::addRewrite(AsyncWebRewrite* rewrite){
  _rewrites.add(rewrite);
  return *rewrite;
}

bool AsyncWebServer::removeRewrite(AsyncWebRewrite *rewrite){
  return _rewrites.remove(rewrite);
}

AsyncWebRewrite& AsyncWebServer::rewrite(const char* from, const char* to){
  return addRewrite(new AsyncWebRewrite(from, to));
}

AsyncWebHandler& AsyncWebServer::addHandler(AsyncWebHandler* handler){
  _handlers.add(handler);
  return *handler;
}

bool AsyncWebServer::removeHandler(AsyncWebHandler *handler){
  return _handlers.remove(handler);
}

void AsyncWebServer::begin(){
  _server.setNoDelay(true);
  _server.begin();
}

void AsyncWebServer::end(){
  _server.end();
}

#if ASYNC_TCP_SSL_ENABLED
void AsyncWebServer::onSslFileRequest(AcSSlFileHandler cb, void* arg){
  _server.onSslFileRequest(cb, arg);
}

void AsyncWebServer::beginSecure(const char *cert, const char *key, const char *password){
  _server.beginSecure(cert, key, password);
}
#endif

void AsyncWebServer::_handleDisconnect(AsyncWebServerRequest *request){
  delete request;
}

void AsyncWebServer::_rewriteRequest(AsyncWebServerRequest *request){
  for(const auto& r: _rewrites){
    if (r->match(request)){
      request->_url = r->toUrl();
      request->_addGetParams(r->params());
    }
  }
}

void AsyncWebServer::_attachHandler(AsyncWebServerRequest *request){
  for(const auto& h: _handlers){
    if (h->filter(request) && h->canHandle(request)){
      request->setHandler(h);
      return;
    }
  }
  
  request->addInterestingHeader("ANY");
  request->setHandler(_catchAllHandler);
}


AsyncCallbackWebHandler& AsyncWebServer::on(String uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody){
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(std::move(uri));
  handler->setMethod(method);
  handler->onRequest(onRequest);
  handler->onUpload(onUpload);
  handler->onBody(onBody);
  addHandler(handler);
  return *handler;
}

AsyncCallbackWebHandler& AsyncWebServer::on(String uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload){
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(std::move(uri));
  handler->setMethod(method);
  handler->onRequest(onRequest);
  handler->onUpload(onUpload);
  addHandler(handler);
  return *handler;
}

AsyncCallbackWebHandler& AsyncWebServer::on(String uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest){
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(std::move(uri));
  handler->setMethod(method);
  handler->onRequest(onRequest);
  addHandler(handler);
  return *handler;
}

AsyncCallbackWebHandler& AsyncWebServer::on(String uri, ArRequestHandlerFunction onRequest){
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(std::move(uri));
  handler->onRequest(onRequest);
  addHandler(handler);
  return *handler;
}

AsyncStaticWebHandler& AsyncWebServer::serveStatic(String uri, fs::FS& fs, String path, const char* cache_control){
  AsyncStaticWebHandler* handler = new AsyncStaticWebHandler(std::move(uri), fs, std::move(path), cache_control);
  addHandler(handler);
  return *handler;
}

void AsyncWebServer::onNotFound(ArRequestHandlerFunction fn){
  _catchAllHandler->onRequest(fn);
}

void AsyncWebServer::onFileUpload(ArUploadHandlerFunction fn){
  _catchAllHandler->onUpload(fn);
}

void AsyncWebServer::onRequestBody(ArBodyHandlerFunction fn){
  _catchAllHandler->onBody(fn);
}

void AsyncWebServer::reset(){
  _rewrites.free();
  _handlers.free();
  
  if (_catchAllHandler != NULL){
    _catchAllHandler->onRequest(NULL);
    _catchAllHandler->onUpload(NULL);
    _catchAllHandler->onBody(NULL);
  }
}

size_t AsyncWebServer::numClients(){
  guard();
  return _requestQueue.length();
}

size_t AsyncWebServer::queueLength(){
  guard();
  size_t count = 0U;
  for(const auto& client: _requestQueue) { if (client->_parseState >= 200) ++count; };
  return count;
}

void AsyncWebServer::processQueue(){  
  // Consider the state of the requests in the queue.
  // Requests in STATE_END have already been handled; we can assume any heap they need has already been allocated.
  // Requests in STATE_QUEUED are pending.  Each iteration we consider the first one.
  // We always allow one request, regardless of heap state.
#ifdef ASYNCWEBSERVER_DEBUG_TRACE
  size_t count = 0, active = 0, queued = 0;
#endif

  {
    guard();
    if (_queueActive) return; // already in progress
    _queueActive = true;

#ifdef ASYNCWEBSERVER_DEBUG_TRACE
    for(auto element: _requestQueue) {
      ++count;
      if (element->_parseState == 100) ++active;
      if (element->_parseState == 200) ++queued;
    }
#endif    
  }

  DEBUG_PRINTFP("Queue: %d entries, %d running, %d queued\n", count, active, queued);

  do { 
    auto heap_ok = get_heap_available() > _queueLimits.requestHeapRequired;
    auto alloc_ok = get_heap_alloc() > ASYNCWEBSERVER_MINIMUM_ALLOC;
    size_t active_entries = 0;
    AsyncWebServerRequest* next_queued_request = nullptr;

    {
      // Get a queued entry while holding the lock
      guard();
      for(auto entry: _requestQueue) {
        if (entry->_parseState == 100) {
          ++active_entries;
        } else if ((entry->_parseState == 200) && !next_queued_request) {
          next_queued_request = entry;
        };
      }
    }

    if (!next_queued_request) break;  // all done
    if ((_queueLimits.nParallel > 0) && (active_entries >= _queueLimits.nParallel)) break; // lots running
    if ((active_entries > 0) && (!heap_ok || !alloc_ok)) {
      DEBUG_PRINTFP("Can't queue more, heap %d alloc %d\n", heap_ok, alloc_ok);
      break;
    }    
    next_queued_request->_handleRequest();
  } while(1); // as long as we have memory and queued requests

  {
    guard();
    for(auto entry: _requestQueue) {
      // Un-defer requests
      if (entry->_parseState == 201) entry->_parseState = 200;
    }
    _queueActive = false;
  }
}

void AsyncWebServer::_dequeue(AsyncWebServerRequest *request){
  {
    DEBUG_PRINTFP("Removing %08X from queue\n", (intptr_t) request);
    guard();    
    _requestQueue.remove(request);
  }
  processQueue();
}

void AsyncWebServer::setQueueLimits(const AsyncWebServerQueueLimits& limits) {
  guard();
  _queueLimits = limits;
}

void AsyncWebServer::printStatus(Print& dest){
  dest.print(F("Web server status: "));
#ifdef ASYNCWEBSERVER_NEEDS_MUTEX
  // Print to a local buffer while we hold the lock
  DynamicBuffer dbuf(2048);
  if (dbuf.size() == 0) {
    dest.println(F("print buffer failure"));
    return;
  };
  BufferPrint<DynamicBuffer> print_dest(dbuf);
#else
  auto& print_dest = dest;
#endif  
  {
    guard();
    if (_requestQueue.isEmpty()) {
      print_dest.print(F(" Idle\n"));
    } else {
      for(const auto& entry: _requestQueue) {
        print_dest.printf_P(PSTR("\n- Request %X [%X], state %d"), (intptr_t) entry, (intptr_t) entry->_client, entry->_parseState);
        if (entry->_response) {
          auto r = entry->_response;
          print_dest.printf_P(PSTR(" -- Response %X, state %d, [%d %d - %d %d %d]"), (intptr_t) r, r->_state, r->_headLength, r->_contentLength, r->_sentLength, r->_ackedLength, r->_writtenLength);
        }
      }
      print_dest.write('\n');
    }
  }
  #ifdef ASYNCWEBSERVER_NEEDS_MUTEX
  print_dest.write('\0'); // null terminate
  dest.print(print_dest.data());
  #endif
}
