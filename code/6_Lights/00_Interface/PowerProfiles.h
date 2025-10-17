// Date Modified: 10 Oct 2025
#pragma once
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────────────
// Profile schema (uniform, integer-only, PROGMEM-ready)
// ─────────────────────────────────────────────────────────────────────────────
enum : uint8_t { SUPPLY_5V = 5, SUPPLY_12V = 12, SUPPLY_24V = 24 };

struct LEDCurrentRequirements {
  uint8_t  supply_v;                // 5, 12, 24
  uint16_t rated_len_px;            // length used for normalization/anchors

  uint16_t I_idle_mA;               // controller + strip idle (mA)
  uint8_t  I_standby_mA_per_led;    // per-LED standby (mA/LED), e.g. 1..2

  // Per-channel slopes @ 255, normalized to 100 px: {R,G,B,W1,W2}
  uint16_t k_mA_per100[5];

  // All-on (RGB / RGBW / RGBWW) slope @ 255 per 100 px (0 ⇒ unused)
  uint16_t k_full_mA_per100;

  // Piecewise anchors (mA @ {0,50,100,200} px) for quick realtime:
  uint16_t I_1ch_mA[4];             // single-channel @ 255
  uint16_t I_full_mA[4];            // all channels @ 255

  // Concurrency compression (permille 0..1000), Bullet-type non-additivity
  uint16_t alpha_permille;
};

// ─────────────────────────────────────────────────────────────────────────────
// Profile names for UI / JSON selection (pipe-delimited, PROGMEM)
// Order must match g_led_profile_table below
// ─────────────────────────────────────────────────────────────────────────────
static const char PROGMEM kLEDProfileNames[] =
  "Pebble 12V RGB|"
  "Pebble 5V RGB|"
  "Bullet 12V RGB|"
  "Bullet 5V RGB|"
  "WS2812 5V RGB|"
  "SK6812 5V RGBW|"
  "WS2815 12V RGBWW";

// ─────────────────────────────────────────────────────────────────────────────
// Profiles (PROGMEM). Values are placeholders unless noted.
// Fill with your measured constants later.
// ─────────────────────────────────────────────────────────────────────────────
static const LEDCurrentRequirements PROGMEM kProfile_Pebble_12V_RGB = {
  /*supply_v*/  SUPPLY_12V, 
  /*rated_len_px*/200,
  /*I_idle_mA*/ 49,                // from your lab ~0.049 A (Pebble 12V)
  /*I_standby*/ 1,                 // default 1 mA/LED (tune if needed)
  /*k_per100*/  { 865, 865, 865, 0, 0 }, // ≈8.65 mA/px → 865 mA per 100 px (R/G/B equal)
  /*k_full*/    0,                 // additive behavior; leave 0 unless you fit
  /*I1ch*/      { 0,  450,  865, 1700 }, // rough anchors mA @{0,50,100,200} (placeholder)
  /*Ifull*/     { 0, 1300, 2600, 5200 }, // rough all-on anchors (placeholder)
  /*alpha‰*/    0                  // near-additive
};

static const LEDCurrentRequirements PROGMEM kProfile_Pebble_5V_RGB = {
  SUPPLY_5V, 200,
  /*I_idle*/    30,  /*standby*/ 1,
  /*k_per100*/  { 900, 900, 900, 0, 0 }, // placeholder
  /*k_full*/    0,
  /*I1ch*/      { 0,  450,  900, 1800 }, // placeholder
  /*Ifull*/     { 0, 1350, 2700, 5400 }, // placeholder
  /*alpha‰*/    0
};

static const LEDCurrentRequirements PROGMEM kProfile_Bullet_12V_RGB = {
  SUPPLY_12V, 100,
  /*I_idle*/    202,               // ~0.202 A from your table (Bullet 12V idle)
  /*standby*/   1,
  /*k_per100*/  { 867, 867, 867, 0, 0 },  // ≈8.67 mA/px → 867 mA/100 px (R/G/B equal)
  /*k_full*/    2324,              // empirical all-on slope (~23.24 mA/px → 2324 mA/100px), placeholder from earlier fit
  /*I1ch*/      { 0,  430,  867, 0 },     // 200 px not applicable for rated_len=100
  /*Ifull*/     { 0, 1160, 2324, 0 },     // anchors aligned to rated_len
  /*alpha‰*/    120                // slight non-additivity (tune from lab)
};

static const LEDCurrentRequirements PROGMEM kProfile_Bullet_5V_RGB = {
  SUPPLY_5V, 100,
  /*I_idle*/    120, /*standby*/ 1,
  /*k_per100*/  { 950, 950, 950, 0, 0 }, // placeholder
  /*k_full*/    2500,                    // placeholder
  /*I1ch*/      { 0,  475,  950, 0 },
  /*Ifull*/     { 0, 1250, 2500, 0 },
  /*alpha‰*/    150
};

static const LEDCurrentRequirements PROGMEM kProfile_WS2812_5V_RGB = {
  SUPPLY_5V, 150,
  /*I_idle*/    25,  /*standby*/ 1,
  /*k_per100*/  { 900, 900, 900, 0, 0 }, // placeholder
  /*k_full*/    2700,                    // classic 60mA/px @ full white ⇒ 6000 mA/100px (but many strips are lower); placeholder conservative
  /*I1ch*/      { 0,  450,  900, 1350 }, // @{0,50,100,150} mapped into 0/50/100/200 bins; last is extrapolated
  /*Ifull*/     { 0, 1500, 3000, 4500 }, // placeholder safer-than-typical
  /*alpha‰*/    100
};

static const LEDCurrentRequirements PROGMEM kProfile_SK6812_5V_RGBW = {
  SUPPLY_5V, 150,
  /*I_idle*/    25,  /*standby*/ 1,
  /*k_per100*/  { 850, 850, 850, 950, 0 }, // W1 present, W2=0 (RGBW)
  /*k_full*/    3400,                       // placeholder
  /*I1ch*/      { 0,  425,  850, 1275 },
  /*Ifull*/     { 0, 1700, 3400, 5100 },
  /*alpha‰*/    120
};

static const LEDCurrentRequirements PROGMEM kProfile_WS2815_12V_RGBWW = {
  SUPPLY_12V, 100,
  /*I_idle*/    60,  /*standby*/ 2,        // WS2815 idle higher; standby ~2 mA/LED typical
  /*k_per100*/  { 700, 700, 700, 800, 800 }, // placeholders (regulator losses)
  /*k_full*/    3000,                        // placeholder all-on slope
  /*I1ch*/      { 0,  350,  700, 0 },
  /*Ifull*/     { 0, 1500, 3000, 0 },
  /*alpha‰*/    180
};

// ─────────────────────────────────────────────────────────────────────────────
// Registry: array of pointers to profiles (PROGMEM), and a count
// Keep order in sync with kLEDProfileNames
// ─────────────────────────────────────────────────────────────────────────────
static const LEDCurrentRequirements* const PROGMEM g_led_profile_table[] = {
  &kProfile_Pebble_12V_RGB,
  &kProfile_Pebble_5V_RGB,
  &kProfile_Bullet_12V_RGB,
  &kProfile_Bullet_5V_RGB,
  &kProfile_WS2812_5V_RGB,
  &kProfile_SK6812_5V_RGBW,
  &kProfile_WS2815_12V_RGBWW,
};

static constexpr uint8_t kLEDProfileCount = sizeof(g_led_profile_table) / sizeof(g_led_profile_table[0]);
