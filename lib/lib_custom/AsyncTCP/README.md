# AsyncTCP 
[![Build Status](https://travis-ci.org/me-no-dev/AsyncTCP.svg?branch=master)](https://travis-ci.org/me-no-dev/AsyncTCP) ![](https://github.com/me-no-dev/AsyncTCP/workflows/Async%20TCP%20CI/badge.svg) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/2f7e4d1df8b446d192cbfec6dc174d2d)](https://www.codacy.com/manual/me-no-dev/AsyncTCP?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=me-no-dev/AsyncTCP&amp;utm_campaign=Badge_Grade)

## Build Options

The following symbols can be defined to change the configuration

| Name | Description | Default |
| ---- | ----------- | ------- |
| CONFIG_ASYNC_TCP_EVENT_QUEUE_SIZE | The size of the queue string TCP events to process | CONFIG_LWIP_MAX_ACTIVE_TCP * 2 == 32 |
| CONFIG_ASYNC_TCP_RUNNING_CORE | The CPU core to run the async task on | -1 (any cpu) |
| CONFIG_ASYNC_TCP_USE_WDT | If the watchdog timer should be enabled while processing TCP messages. Set to 0 to disable | 1 (enabled) | 
| CONFIG_ASYNC_TCP_TASK_STACK_SIZE | The number of 32-bit words (not bytes!) to allocate for use as the task's stack. | 8192 | 
| CONFIG_ASYNC_TCP_TASK_PRIORITY | The [priority](https://www.freertos.org/RTOS-task-priority.html) at which the created task will execute. | 3 |
| CONFIG_ASYNC_TCP_DIAGNOSTICS | Enable diagnostic functions | disabled |

## Other Parameters

| Name | Description | Default |
| ---- | ----------- | ------- |
| CONFIG_LWIP_MAX_ACTIVE_TCP | https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html#config-lwip-max-active-tcp | 16 |


## Diagnostics Functions

### Stack Highwater Mark

AsyncTCP creates a task to handle the TCP messages. This task needs to be initialize
with stack space. See [How big should the stack be?](https://www.freertos.org/FAQMem.html#StackSize)

This function gets the async_tcp task's minimum amount of remaining stack space that was 
available to the task since the task started executing - - that is the amount
of stack that remained unused when the task stack was at its greatest (deepest)
value. This is what is referred to as the stack 'high water mark'. For more information
on the stack high water mark, see See https://www.freertos.org/uxTaskGetStackHighWaterMark.html

If the value from this function is close to zero, increasing CONFIG_ASYNC_TCP_TASK_STACK_SIZE can help.

```c++
static UBaseType_t AsyncClient::getStackHighWaterMark()
```

### 

### Async TCP Library for ESP32 Arduino

[![Join the chat at https://gitter.im/me-no-dev/ESPAsyncWebServer](https://badges.gitter.im/me-no-dev/ESPAsyncWebServer.svg)](https://gitter.im/me-no-dev/ESPAsyncWebServer?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

This is a fully asynchronous TCP library, aimed at enabling trouble-free, multi-connection network environment for Espressif's ESP32 MCUs.

This library is the base for [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

## AsyncClient and AsyncServer
The base classes on which everything else is built. They expose all possible scenarios, but are really raw and require more skills to use.


