/**************
 * 
 * File contains minimal defaults of each hardcoded template, used on new builds.
 * 
 */


#pragma once


#include "2_CoreSystem/mBaseConfig.h"

#ifndef USE_NETWORK_CONFIG_TEMPLATE

#warning "USE_NETWORK_CONFIG_TEMPLATE" has not been defined -- using hardcoded defaults for network config template, which may not be ideal for all devices. Consider defining USE_NETWORK_CONFIG_TEMPLATE to use the default template stored in PROGMEM, which can be overridden by OTA updates and edited via web UI, or define your own NETWORK_CONFIG_TEMPLATE.

DEFINE_PGM_CTR(NETWORK_CONFIG_TEMPLATE)
"{"
  "\"Version\":1,"

  "\"Interface\":{"
    "\"Policy\":{"
      "\"PreferOrder\":[\"Ethernet\",\"WiFi\",\"Cellular\"],"
      "\"AllowMultipleActive\":true,"
      "\"BlockRemoteMqttWhenLocalAvailable\":true"
    "}"
  "},"

  "\"WiFi\":{"
    "\"EN\":true,"
    "\"Backoff\":[5,60,600],"

    "\"Mode\":{"
      "\"STA\":true,"
      "\"AP\":true,"
      "\"STA_AP\":true,"
      "\"APBootMins\":10,"
      "\"APOnSTAFail\":true,"
      "\"APFailDelayMins\":0,"
      "\"APAlwaysOn\":false"
    "},"

    "\"Station\":{"
      "\"Profiles\":["
        "{"
          "\"SSID\":\"" STA_SSID1 "\","
          "\"Password\":\"" STA_PASS1 "\""
        "},"
        "{"
          "\"SSID\":\"" STA_SSID2 "\","
          "\"Password\":\"" STA_PASS2 "\""
        "},"
        "{"
          "\"SSID\":\"" STA_SSID3 "\","
          "\"Password\":\"" STA_PASS3 "\""
        "}"
      "],"

      "\"IPv4\":{"
        "\"Static\":false,"
        "\"IP\":\"0.0.0.0\","
        "\"Gateway\":\"0.0.0.0\","
        "\"Subnet\":\"0.0.0.0\","
        "\"DNS1\":\"0.0.0.0\","
        "\"DNS2\":\"0.0.0.0\""
      "}"
    "},"

    "\"SoftAP\":{"
      "\"SSID\":\"" SOFTAP_SSID "\","
      "\"Password\":\"" SOFTAP_PASSWORD "\","
      "\"Channel\":1"
    "}"
  "},"

  "\"Ethernet\":{"
    "\"EN\":true,"
    "\"Backoff\":[5,30,60],"

    "\"IPv4\":{"
      "\"Static\":false,"
      "\"IP\":\"0.0.0.0\","
      "\"Gateway\":\"0.0.0.0\","
      "\"Subnet\":\"0.0.0.0\","
      "\"DNS1\":\"0.0.0.0\","
      "\"DNS2\":\"0.0.0.0\""
    "}"
  "},"

  "\"MQTT\":{"
    "\"EN\":true,"

    "\"UpdateSeconds\":{"
      "\"IfChanged\":1,"
      "\"TelePeriod\":60,"
      "\"ConfigPeriod\":60"
    "},"

    "\"Brokers\":["
      "{"
        "\"Id\":\"home\","
        "\"EN\":true,"
        "\"Host\":\"" MQTT_HOST "\","
        "\"Port\":" STR(MQTT_PORT) ","
        "\"User\":\"\","
        "\"Password\":\"\","
        "\"TopicPrefix\":\"" DEVICENAME_CTR "\","
        "\"ClientName\":\"" DEVICENAME_CTR "\","
        "\"Backoff\":[5,10,60],"
        "\"Transport\":[\"WiFi\"],"
        "\"PrefTransport\":[\"WiFi\"],"
        "\"OutgoingLevel\":3,"
        "\"OutgoingLimiterMs\":0"
      "}"
    "]"
  "}"
"}";

#endif

