# AsyncTCP changelog

Newest at top

## 2021-09-02 - Check for error on connect and return false if the connection failed

**Commit**: [c4d43765217a67f8394bf946336ce55a13aef31a](https://github.com/pbolduc/AsyncTCP/commit/c4d43765217a67f8394bf946336ce55a13aef31a)

Original fix, see [BlueAndi/AsyncTCP](https://github.com/BlueAndi/AsyncTCP/commit/ce2e7949d9694a8b10379c39d101ce55c2a8a287)'s commit

* _tcp_connect can return ESP_OK or an error. This change will return false if the _tcp_connect is not successful. Applications could wait for a long time if they believe the connection was successful.

## 2021-09-01 - Added CHANGELOG.md

* added this document

## 2021-09-01 - Added config features and ack timeout

**Commit**: [1874f8b2ac8b25cdcb45f78be467e1f961472f2c](https://github.com/pbolduc/AsyncTCP/commit/1874f8b2ac8b25cdcb45f78be467e1f961472f2c)

* Updated README.md to describe new configuration items below
* Added #define to configure the TCP event queue size
* Added #define to configure the async task stack size
* Added #define to configure the async task priority
* Added optional #define to expose diagnostic information (not tested yet)
* Implemented 'ack timeout 4' disconnects fixes from Maurice Makaay and the esphome project
  * https://github.com/OttoWinter/AsyncTCP/pull/4

## 2021-08-31 - Fix memory leak and potential crash on low memory

**Commit**: [c5379e598e2c24a703fd00978ad78e1da9925888](https://github.com/pbolduc/AsyncTCP/commit/c5379e598e2c24a703fd00978ad78e1da9925888)

* Memory Leak - In the case the TCP message queue is full, in `_remove_events_with_arg`, if a dequeued packet cannot be requeued, it needs to be freed to prevent memory leak.
* Crash on low memory - various handlers attempt to allocate a `lwip_event_packet_t` object to be added to the queue. Added checks to see if malloc failed to avoid crash with LoadProhibited error.

## 2021-08-19 - Make watchdog config independent of running core

**Commit**: [4cd288686d11e2e7f74782cac0d40b6494c97033](https://github.com/pbolduc/AsyncTCP/commit/4cd288686d11e2e7f74782cac0d40b6494c97033)

Define `CONFIG_ASYNC_TCP_USE_WDT=0` to disable the WDT usage. WDT adds 30-200us per event.

## 2019-10-17 - Base Forked Version

**Commit**: [ca8ac5f919d02bea07b474531981ddbfd64de97c](https://github.com/me-no-dev/AsyncTCP/commit/ca8ac5f919d02bea07b474531981ddbfd64de97c)

This version is based on orginal [me-no-dev/AsyncTCP](https://github.com/me-no-dev/AsyncTCP) repository.
