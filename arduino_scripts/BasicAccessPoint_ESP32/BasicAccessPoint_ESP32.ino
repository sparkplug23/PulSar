#include <WiFi.h>

// -----------------------------------------------------------------------------
// Temporary isolated AP for ESP32 lab devices
// Date Modified: 2026-06-18
// -----------------------------------------------------------------------------

static const char* AP_SSID     = "ServerLink32";
static const char* AP_PASSWORD = "af4d8bc9ab";   // Minimum 8 chars for WPA2

static const uint8_t AP_CHANNEL     = 6;
static const uint8_t AP_MAX_CLIENTS = 8;
static const bool    AP_HIDDEN      = false;

// AP-side network
IPAddress ap_ip(192, 168, 50, 1);
IPAddress ap_gateway(192, 168, 50, 1);
IPAddress ap_subnet(255, 255, 255, 0);

uint32_t last_print_ms = 0;

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("[AP] Starting temporary ESP32 access point");

  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);

  // Optional, but useful for range/stability
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  if (!WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet))
  {
    Serial.println("[AP] softAPConfig failed");
  }

  bool ok = WiFi.softAP(
    AP_SSID,
    AP_PASSWORD,
    AP_CHANNEL,
    AP_HIDDEN,
    AP_MAX_CLIENTS
  );

  if (!ok)
  {
    Serial.println("[AP] softAP failed");
    return;
  }

  Serial.println("[AP] Started");
  Serial.print("[AP] SSID: ");
  Serial.println(AP_SSID);

  Serial.print("[AP] Password: ");
  Serial.println(AP_PASSWORD);

  Serial.print("[AP] IP: ");
  Serial.println(WiFi.softAPIP());

  Serial.print("[AP] MAC: ");
  Serial.println(WiFi.softAPmacAddress());
}

void loop()
{
  const uint32_t now = millis();

  if ((now - last_print_ms) >= 5000)
  {
    last_print_ms = now;

    Serial.print("[AP] Connected clients: ");
    Serial.println(WiFi.softAPgetStationNum());
  }
}