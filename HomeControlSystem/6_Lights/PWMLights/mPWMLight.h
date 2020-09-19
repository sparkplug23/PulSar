#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


// #include "3_Network/MQTT/mMQTT.h"
// #include "2_CoreSystem/Time/mTime.h"

// #define LST_MAX 5

// typedef unsigned long power_t;              // Power (Relay) type

// // Light color mode, either RGB alone, or white-CT alone, or both only available if ct_rgb_linked is false
// enum LightColorModes {
//   LCM_RGB = 1, LCM_CT = 2, LCM_BOTH = 3 };

  const char kTitle_TableTitles_Root_PWM[] PROGMEM = 
    "Animation Status" "|" 
    "Lights Auto Off Timer";

// struct LRgbColor {
//   uint8_t R, G, B;
// };
// const uint8_t MAX_FIXED_COLOR = 12;
// const LRgbColor kFixedColor[MAX_FIXED_COLOR] PROGMEM =
//   { 255,0,0, 0,255,0, 0,0,255, 228,32,0, 0,228,32, 0,32,228, 188,64,0, 0,160,96, 160,32,240, 255,255,0, 255,0,170, 255,255,255 };

// struct LWColor {
//   uint8_t W;
// };
// const uint8_t MAX_FIXED_WHITE = 4;
// const LWColor kFixedWhite[MAX_FIXED_WHITE] PROGMEM = { 0, 255, 128, 32 };

// struct LCwColor {
//   uint8_t C, W;
// };
// const uint8_t MAX_FIXED_COLD_WARM = 4;
// const LCwColor kFixedColdWarm[MAX_FIXED_COLD_WARM] PROGMEM = { 0,0, 255,0, 0,255, 128,128 };

// // New version of Gamma correction compute
// // Instead of a table, we do a multi-linear approximation, which is close enough
// // At low levels, the slope is a bit higher than actual gamma, to make changes smoother
// // Internal resolution is 10 bits.

// typedef struct gamma_table_t {
//   uint16_t to_src;
//   uint16_t to_gamma;
// } gamma_table_t;

// const gamma_table_t gamma_table[] = {   // don't put in PROGMEM for performance reasons
//   {    1,      1 },
//   {    4,      1 },
//   {  209,     13 },
//   {  312,     41 },
//   {  457,    106 },
//   {  626,    261 },
//   {  762,    450 },
//   {  895,    703 },
//   { 1023,   1023 },
//   { 0xFFFF, 0xFFFF }          // fail-safe if out of range
// };

// // simplified Gamma table for Fade, cheating a little at low brightness
// const gamma_table_t gamma_table_fast[] = {
//   {   384,    192 },
//   {   768,    576 },
//   {  1023,   1023 },
//   { 0xFFFF, 0xFFFF }          // fail-safe if out of range
// };

// // For reference, below are the computed gamma tables, via ledGamma()
// // for 8 bits output:
// //   0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
// //   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
// //   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,
// //   3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,
// //   6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 11,
// //  11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18,
// //  18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
// //  25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38,
// //  39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53,
// //  54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 67, 68, 69,
// //  71, 72, 73, 75, 76, 78, 79, 80, 82, 83, 85, 86, 87, 89, 90, 91,
// //  93, 94, 95, 97, 98,100,101,102,104,105,107,108,109,111,112,114,
// // 116,118,120,122,124,125,127,129,131,133,135,137,139,141,143,144,
// // 146,148,150,152,154,156,158,160,162,164,166,168,170,171,173,175,
// // 178,180,183,185,188,190,193,195,198,200,203,205,208,210,213,215,
// // 218,220,223,225,228,230,233,235,238,240,243,245,248,250,253,255
// //
// // and for 10 bits output:
// //    0,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
// //    5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
// //    8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,
// //   12,  12,  13,  13,  13,  14,  15,  16,  17,  18,  20,  21,  22,  23,  24,  25,
// //   26,  27,  28,  29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,
// //   45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,  65,  67,  68,  70,  72,
// //   74,  76,  77,  79,  81,  83,  84,  86,  88,  90,  92,  93,  95,  97,  99, 101,
// //  102, 104, 106, 110, 113, 117, 121, 124, 128, 132, 135, 139, 143, 146, 150, 154,
// //  158, 162, 166, 169, 173, 177, 180, 184, 188, 191, 195, 199, 202, 206, 210, 213,
// //  217, 221, 224, 228, 232, 235, 239, 243, 246, 250, 254, 257, 261, 267, 272, 278,
// //  283, 289, 294, 300, 305, 311, 317, 322, 328, 333, 339, 344, 350, 356, 361, 367,
// //  372, 378, 383, 389, 394, 400, 406, 411, 417, 422, 428, 433, 439, 444, 450, 458,
// //  465, 473, 480, 488, 496, 503, 511, 518, 526, 534, 541, 549, 557, 564, 572, 579,
// //  587, 595, 602, 610, 617, 627, 635, 642, 650, 657, 665, 673, 680, 688, 695, 703,
// //  713, 723, 733, 743, 753, 763, 773, 783, 793, 803, 813, 823, 833, 843, 853, 863,
// //  873, 883, 893, 903, 913, 923, 933, 943, 953, 963, 973, 983, 993,1003,1013,1023
// //
// // Output for Dimmer 0..100 values
// //   0,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,  7,  8,  8,  9,
// //  10, 10, 11, 12, 12, 13, 15, 17, 21, 23, 26, 28, 31, 34, 37,
// //  40, 43, 49, 52, 58, 61, 67, 70, 76, 79, 84, 90, 93, 99,102,
// // 110,117,128,135,146,158,166,177,184,195,202,213,221,232,239,
// // 250,261,272,289,300,317,328,344,356,372,389,400,417,428,444,
// // 458,480,496,518,534,557,579,595,617,635,657,673,695,713,743,
// // 773,793,823,843,873,893,923,943,973,993,1023



// uint16_t change8to10(uint8_t v);
// uint8_t change10to8(uint16_t v);


//       const char kListPWM_TestColours[] PROGMEM =  
//         "#ff0000" "|" 
//         "#00ff00" "|" 
//         "#0000ff" "|" 
//         "#ffffff" "|"  
//         "#FFE076" "|" ;

class mPWMLight{ //name reverse, as Interface is the linking/grouping factor
  public:
    mPWMLight(){};

//     enum RELAY_IDS{RELAY_0_ID=0,RELAY_1_ID,RELAY_2_ID,RELAY_3_ID};

    int8_t Tasker(uint8_t function);
//     void Init(void);
    
//   // are RGB and CT linked, i.e. if we set CT then RGB channels are off
//   bool     _ct_rgb_linked = true;
//   bool     _pwm_multi_channels = false;    // treat each channel as independant dimmer

// // class LightStateClass {
// //   private:
   
// //   public:
// //     LightStateClass() {
// //       //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
// //     }

// // CT min and max
// const uint16_t CT_MIN = 153;          // 6500K
// const uint16_t CT_MAX = 500;          // 2000K
// // Ranges used for Alexa
// const uint16_t CT_MIN_ALEXA = 200;    // also 5000K
// const uint16_t CT_MAX_ALEXA = 380;    // also 2600K


//      uint16_t _hue = 0;  // 0..359
//     uint8_t  _sat = 255;  // 0..255
//     uint8_t  _briRGB = 255;  // 0..255
//     // dimmer is same as _bri but with a range of 0%-100%
//     uint8_t  _r = 255;  // 0..255
//     uint8_t  _g = 255;  // 0..255
//     uint8_t  _b = 255;  // 0..255

//     uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
//     uint16_t _ct = CT_MIN;  // 153..500, default to 153 (cold white)
//     uint8_t  _wc = 255;  // white cold channel
//     uint8_t  _ww = 0;    // white warm channel
//     uint8_t  _briCT = 255;

//     uint8_t  _color_mode = LCM_RGB; // RGB by default
//     // the CT range below represents the rendered range,
//     // This is due to Alexa whose CT range is 199..383
//     // Hence setting Min=200 and Max=380 makes Alexa use the full range
//     // Please note that you can still set CT to 153..500, but any
//     // value below _ct_min_range or above _ct_max_range not change the CT
//     uint16_t _ct_min_range = CT_MIN;   // the minimum CT rendered range
//     uint16_t _ct_max_range = CT_MAX;   // the maximum CT rendered range




// // struct LIGHT {
//   uint32_t strip_timer_counter = 0;  // Bars and Gradient
//   // power_t power = 0;                      // Power<x> for each channel if SetOption68, or boolean if single light

//   uint16_t wakeup_counter = 0;

//   enum COLOUR_IDS{
//     COLOUR_RED_ID=0,
//     COLOUR_GREEN_ID,
//     COLOUR_BLUE_ID,
//     COLOUR_WHITE_ID,
//     COLOUR_WARM_WHITE_ID,
//     COLOUR_LENGTH_ID
//   };





//   uint8_t wheel = 0;
//   uint8_t random = 0;
//   uint8_t subtype = 0;                    // LST_ subtype
//   uint8_t device = 0;
//   uint8_t old_power = 1;
//   uint8_t wakeup_active = 0;
//   uint8_t wakeup_dimmer = 0;
//   uint8_t fixed_color_index = 1;
//   uint8_t pwm_offset = 0;                 // Offset in color buffer
//   // uint8_t max_scheme = LS_MAX -1;

//   bool update = true;
//   bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer

//   bool     fade_initialized = false;      // dont't fade at startup
//   bool     fade_running = false;
// // #ifdef USE_DEVICE_GROUPS
// //   uint8_t  last_scheme = 0;
// //   bool     devgrp_no_channels_out = false; // don't share channels with device group (e.g. if scheme set by other device)
// // #ifdef USE_DGR_LIGHT_SEQUENCE
// //   uint8_t  sequence_offset = 0;            // number of channel changes this light is behind the master
// //   uint8_t * channels_fifo;
// // #endif  // USE_DGR_LIGHT_SEQUENCE
// // #endif  // USE_DEVICE_GROUPS
// // #ifdef USE_LIGHT_PALETTE
// //   uint8_t  palette_count = 0;            // palette entry count
// //   uint8_t * palette;                     // dynamically allocated palette color array
// // #endif  // USE_LIGHT_PALETTE
#define LST_MAX 5
  uint16_t fade_start_10[LST_MAX] = {0,0,0,0,0};
  uint16_t fade_cur_10[LST_MAX];
  uint16_t fade_end_10[LST_MAX];         // 10 bits resolution target channel values
  uint16_t fade_duration = 0;            // duration of fade in milliseconds
  uint32_t fade_start = 0;               // fade start time in milliseconds, compared to millis()

//   uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
//   uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023
// // } Light;


// void SetPWMChannelSweepRange();


//     uint16_t pwm_tester_val = 0;
//     uint8_t pwm_tester_dir = 0;


//     uint16_t pwm_channel_test[5];


void WebCommand_Parse();

void Init();

int8_t Tasker_Web(uint8_t function);


//   uint8_t channels[5]; //rgbwcww

    uint32_t tSavedFadeProgress = 0;
    uint32_t tSavedLightSetOutputs = 0;
    uint32_t tSavedColourChange = 0;
uint32_t tSavedAutoOff = 0;

void LightSetOutputs(const uint16_t *cur_col_10);
// void LightSetOutputs(const uint8_t *cur_col_10) ;

// power_t LightPower(void);
// power_t LightPowerIRAM(void);
// uint8_t LightDevice(void);
// uint8_t setColorMode(uint8_t cm);
// void addRGBMode();
// void addCTMode();
// void getRGB(uint8_t *r, uint8_t *g, uint8_t *b);
// void getCW(uint8_t *rc, uint8_t *rw);
// void getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w);
// uint8_t getChannels(uint8_t *channels);
// void getChannelsRaw(uint8_t *channels);
// void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri);
// uint8_t getBri(void);
// inline uint8_t getBriCT();
// static inline uint8_t DimmerToBri(uint8_t dimmer);
// static uint8_t BriToDimmer(uint8_t bri);
// uint8_t getDimmer(uint32_t mode = 0);

// //     inline 
// uint16_t getCT();// const {
// //       return _ct; // 153..500, or CT_MIN..CT_MAX
// //     }

// //     // get the CT value within the range into a 10 bits 0..1023 value
// //     uint16_t getCT10bits() const {
// //       return mSupport::changeUIntScale(_ct, _ct_min_range, _ct_max_range, 0, 1023);
// //     }

// //     inline void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) {
// //       _ct_min_range = ct_min_range;
// //       _ct_max_range = ct_max_range;
// //     }

// //     inline void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) const {
// //       if (ct_min_range) { *ct_min_range = _ct_min_range; }
// //       if (ct_max_range) { *ct_max_range = _ct_max_range; }
// //     }

void WebAppend_JSON_RootPage_LiveviewPixels();

void WebAppend_Root_ControlUI();

void WebAppend_Root_Status_Table();

// void getXY(float *x, float *y);
// void setBri(uint8_t bri);
// uint8_t setBriRGB(uint8_t bri_rgb);
// uint8_t setBriCT(uint8_t bri_ct);
// inline uint8_t getBriRGB();
// void setDimmer(uint8_t dimmer);
// void setCT(uint16_t ct);
// void setCW(uint8_t c, uint8_t w, bool free_range = false);
// uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
// void setHS(uint16_t hue, uint8_t sat);
// void setChannelsRaw(uint8_t *channels);
// void setChannels(uint8_t *channels);
// // static void RgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri);
// // static void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
// // static void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
// // static void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);
// // void mat3x3(const float *mat33, const float *vec3, float *res3);
// // void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
// // void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);
// void setSubType(uint8_t sub_type);
// inline bool setCTRGBLinked(bool ct_rgb_linked);
// void setAlexaCTRange(bool alexa_ct_range);
// inline bool isCTRGBLinked();
// inline bool setPWMMultiChannel(bool pwm_multi_channels);
// inline bool isPWMMultiChannel(void);
// void debugLogs();
// void loadSettings();
// void changeCTB(uint16_t new_ct, uint8_t briCT);
// void changeDimmer(uint8_t dimmer, uint32_t mode = 0);
// void changeBri(uint8_t bri);
// void changeBriRGB(uint8_t bri);
// void changeBriCT(uint8_t bri);
// void changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
// void calcLevels(uint8_t *current_color = nullptr);
// void changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB);
// void saveSettings();
// void changeChannels(uint8_t *channels);
// uint16_t change8to10(uint8_t v);
// uint8_t change10to8(uint16_t v);
// uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr);
// uint16_t ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr);
// uint16_t ledGamma10_10(uint16_t v);
// uint16_t ledGamma10(uint8_t v);
// uint8_t ledGamma(uint8_t v);
// void LightPwmOffset(uint32_t offset);
// bool LightModuleInit(void);
// void LightCalcPWMRange(void);
// void LightUpdateColorMapping(void);
// uint8_t LightGetDimmer(uint8_t dimmer);
// void LightSetDimmer(uint8_t dimmer);
// void LightGetHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri);
// void LightGetXY(float *X, float *Y);
// void LightHsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
// uint8_t LightGetBri(uint8_t device);
// void LightSetBri(uint8_t device, uint8_t bri);
// void LightSetColorTemp(uint16_t ct);
// uint16_t LightGetColorTemp(void);
// void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value);
// char* LightGetColor(char* scolor, boolean force_hex = false);
// void LightPowerOn(void);
// void LightState(uint8_t append);
// void LightPreparePower(power_t channels = 0xFFFFFFFF);
// void LightSetPaletteEntry(void);
// void LightCycleColor(int8_t direction);
// void LightSetPower(void);
void LightAnimate(void);
bool LightApplyFade2(void);
void LightApplyPower(uint8_t new_color[LST_MAX], power_t power);
void calcGammaMultiChannels(uint16_t cur_col_10[5]);
void calcGammaBulbs(uint16_t cur_col_10[5]);
bool isChannelGammaCorrected(uint32_t channel);
bool isChannelCT(uint32_t channel);
uint16_t fadeGamma(uint32_t channel, uint16_t v);
uint16_t fadeGammaReverse(uint32_t channel, uint16_t vg);


 

// uint8_t* GetColour_RGB();    // r,g,b only
// uint8_t* GetColour_RGBCT();  // r,g,b,white and warm white
// uint8_t* GetColour_CT();     // white and warm white
// uint8_t* GetColour_White();         // white
// uint8_t* GetColour_WarmWhite();     // warm white
// uint8_t* GetColour_HSB();
// uint8_t* GetColour_Hue();
// uint8_t* GetColour_Sat();
// uint8_t* GetColour_Brightness();

// void SetColour_RGB();    // r,g,b only
// void SetColour_RGBCT();  // r,g,b,white and warm white
// void SetColour_CT();     // white and warm white
// void SetColour_White();         // white
// void SetColour_WarmWhite();     // warm white
// void SetColour_HSB();


// HsbColor GetHSBColour();
// void SetHSBColour();

// void SetColour_HSB(HsbColor hsb);
// void SetColour_Hue(uint16_t hue);
// void SetColour_Sat(uint8_t  sat);
// void SetColour_Brt(uint8_t  brt);


// void UpdateSetOutputs();


// uint8_t getColorMode();

//     void ConstructJSON_Settings(uint8_t json_method = 0);
//     void ConstructJSON_Sensor(uint8_t json_method = 0);
//     void ConstructJSON_EnergyStats(uint8_t json_method = 0);

  
//     #ifdef USE_MQTT 
//         void MQTTHandler_Init();
//         void MQTTHandler_Set_fSendNow();
//         void MQTTHandler_Set_TelePeriod();
        
//         struct handler<mInterfaceLight>* mqtthandler_ptr;
//         void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

//         const char* postfix_topic_settings = "settings";
//         struct handler<mInterfaceLight> mqtthandler_settings_teleperiod;
//         void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
        
//         const char* postfix_topic_sensors = "sensors";
//         struct handler<mInterfaceLight> mqtthandler_sensor_ifchanged;
//         struct handler<mInterfaceLight> mqtthandler_sensor_teleperiod;
//         void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);
        
//         // Extra module only handlers
//         // enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
//         //   MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
//         //   MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID,
//         //   MQTT_HANDLER_MODULE_LENGTH_ID, // id count
//         // };
//         const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
//     #endif



};
#endif

#endif




// // /*
// //   xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Tasmota

// //   Copyright (C) 2020  Theo Arends

// //   This program is free software: you can redistribute it and/or modify
// //   it under the terms of the GNU General Public License as published by
// //   the Free Software Foundation, either version 3 of the License, or
// //   (at your option) any later version.

// //   This program is distributed in the hope that it will be useful,
// //   but WITHOUT ANY WARRANTY; without even the implied warranty of
// //   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// //   GNU General Public License for more details.

// //   You should have received a copy of the GNU General Public License
// //   along with this program.  If not, see <http://www.gnu.org/licenses/>.
// // */

// // // #ifdef USE_LIGHT
// // /*********************************************************************************************\
// //  * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
// //  *
// //  * light_type  Module     Color  ColorTemp  Modules
// //  * ----------  ---------  -----  ---------  ----------------------------
// //  *  0          -                 no         (Sonoff Basic)
// //  *  1          PWM1       W      no         (Sonoff BN-SZ)
// //  *  2          PWM2       CW     yes        (Sonoff Led)
// //  *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
// //  *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
// //  *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
// //  *  9          reserved          no
// //  * 10          reserved          yes
// //  * 11          +WS2812    RGB    no         (One WS2812 RGB or RGBW ledstrip)
// //  * 12          AiLight    RGBW   no
// //  * 13          Sonoff B1  RGBCW  yes
// //  *
// //  * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
// //  * ------------  ------  ---------  ----------  -----------------
// //  *  0            yes     yes        yes         Color On/Off
// //  *  1            yes     yes        yes         Wakeup light
// //  *  2            yes     yes        no          Color cycle RGB
// //  *  3            yes     yes        no          Color cycle RBG
// //  *  4            yes     yes        no          Random RGB colors
// //  *  5            yes     no         no          Clock
// //  *  6            yes     no         no          Incandescent
// //  *  7            yes     no         no          RGB
// //  *  8            yes     no         no          Christmas
// //  *  9            yes     no         no          Hanukkah
// //  * 10            yes     no         no          Kwanzaa
// //  * 11            yes     no         no          Rainbow
// //  * 12            yes     no         no          Fire
// //  *
// // \*********************************************************************************************/

// // /*********************************************************************************************\
// //  *
// //  * Light management has been refactored to provide a cleaner class-based interface.
// //  * Also, now all values are stored as integer, no more floats that could generate
// //  * rounding errors.
// //  *
// //  * Two singletons are now used to control the state of the light.
// //  *  - light_state (LightStateClass) stores the color / white temperature and
// //  *    brightness. Use this object to READ only.
// //  *  - light_controller (LightControllerClass) is used to change light state
// //  *    and adjust all Settings and levels accordingly.
// //  *    Always use this object to change light status.
// //  *
// //  * As there have been lots of changes in light control, here is a summary out
// //  * the whole flow from setting colors to drving the PMW pins.
// //  *
// //  * 1.  To change colors, always use 'light_controller' object.
// //  *     'light_state' is only to be used to read current state.
// //  *  .a For color bulbs, set color via changeRGB() or changeHS() for Hue/Sat.
// //  *     Set the overall brightness changeBri(0..255) or changeDimmer(0..100%)
// //  *     RGB and Hue/Sat are always kept in sync. Internally, RGB are stored at
// //  *     full range (max brightness) so that when you reduce brightness and
// //  *     raise it back again, colors don't change due to rounding errors.
// //  *  .b For white bulbs with Cold/Warm colortone, use changeCW() or changeCT()
// //  *     to change color-tone. Set overall brightness separately.
// //  *     Color-tone temperature can range from 153 (Cold) to 500 (Warm).
// //  *     SetOption82 can expand the rendering from 200-380 due to Alexa reduced range.
// //  *     CW channels are stored at full brightness to avoid rounding errors.
// //  *  .c Alternatively, you can set all 5 channels at once with changeChannels(),
// //  *     in this case it will also set the corresponding brightness.
// //  *
// //  * 2.a After any change, the Settings object is updated so that changes
// //  *     survive a reboot and can be stored in flash - in saveSettings()
// //  *  .b Actual channel values are computed from RGB or CT combined with brightness.
// //  *     Range is still 0..255 (8 bits) - in getActualRGBCW()
// //  *  .c The 5 internal channels RGBWC are mapped to the actual channels supported
// //  *     by the light_type: in calcLevels()
// //  *     1 channel  - 0:Brightness
// //  *     2 channels - 0:Coldwhite 1:Warmwhite
// //  *     3 channels - 0:Red 1:Green 2:Blue
// //  *     4 chennels - 0:Red 1:Green 2:Blue 3:White
// //  *     5 chennels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
// //  *
// //  * 3.  In LightAnimate(), final PWM values are computed at next tick.
// //  *  .a If color did not change since last tick - ignore.
// //  *  .b Extend resolution from 8 bits to 10 bits, which makes a significant
// //  *     difference when applying gamma correction at low brightness.
// //  *  .c Apply Gamma Correction if LedTable==1 (by default).
// //  *     Gamma Correction uses an adaptative resolution table from 11 to 8 bits.
// //  *  .d For Warm/Cold-white channels, Gamma correction is calculated in combined mode.
// //  *     Ie. total white brightness (C+W) is used for Gamma correction and gives
// //  *     the overall light power required. Then this light power is split among
// //  *     Wamr/Cold channels.
// //  *  .e Gamma correction is still applied to 8 bits channels for compatibility
// //  *     with other non-PMW modules.
// //  *  .f Apply color balance correction from rgbwwTable[].
// //  *     Note: correction is done after Gamma correction, it is meant
// //  *     to adjust leds with different power
// //  *  .g If rgbwwTable[4] is zero, blend RGB with White and adjust the level of
// //  *     White channel according to rgbwwTable[3]
// //  *  .h Scale ranges from 10 bits to 0..PWMRange (by default 1023) so no change
// //  *     by default.
// //  *  .i Apply port remapping from Option37
// //  *  .j Invert PWM value if port is of type PMWxi instead of PMWx
// //  *  .k Apply PWM value with analogWrite() - if pin is configured
// //  *
// // \*********************************************************************************************/
