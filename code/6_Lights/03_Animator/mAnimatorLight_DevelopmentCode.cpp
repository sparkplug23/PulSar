#include "mAnimatorLight.h"

/**
 * @brief Temporary file
 * 
 * To clean up the Set/Get of pixel to include MIRROR/REVERSE/MAPPED/MULTIPLIED/GROUPED
 */

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER

/**
 * @brief Development tester of segments, where json commands to configure them all is not large enough
 **/
void mAnimatorLight::Test_Config()
{
  uint8_t seg_idx = 0;

  uint8_t brightness = 10;

  uint16_t row_start_iter = 0;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 500;
  _segments[seg_idx].transition.rate_ms = 1000;
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID, 0);
  CommandSet_ActiveSolidPalette_Hue_360(0, 0);
  CommandSet_ActiveSolidPalette_Sat_255(255, 0);
  CommandSet_BrtRGB_255(brightness, 0);
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_RGBCCT_COLOUR_02__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 500;
  _segments[seg_idx].transition.rate_ms = 1000;
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_02__ID, 1);
  CommandSet_ActiveSolidPalette_Hue_360(120, 1);
  CommandSet_ActiveSolidPalette_Sat_255(255, 1);
  CommandSet_BrtRGB_255(brightness, 1);
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }

  row_start_iter+=8;
  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_RGBCCT_COLOUR_03__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 500;
  _segments[seg_idx].transition.rate_ms = 1000;
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_03__ID, 2);
  CommandSet_ActiveSolidPalette_Hue_360(240, 2);
  CommandSet_ActiveSolidPalette_Sat_255(255, 2);
  CommandSet_BrtRGB_255(brightness, 2);
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_RGBCCT_COLOUR_04__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 500;
  _segments[seg_idx].transition.rate_ms = 1000;
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_04__ID, 0);
  CommandSet_ActiveSolidPalette_Hue_360(345, 3);
  CommandSet_ActiveSolidPalette_Sat_255(255, 3);
  CommandSet_BrtRGB_255(brightness, 3);
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  row_start_iter+=8;
  row_start_iter+=8;


  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 0;
  _segments[seg_idx].transition.rate_ms = 2000;
  _segments[seg_idx].flags.brightness_applied_during_colour_generation = true;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_05__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__SLOW_GLOW__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 2000;
  _segments[seg_idx].transition.rate_ms = 3000; 
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  // // _segments[seg_idx].pixel_range.start = row_start_iter;
  // // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // // _segments[seg_idx].grouping = 1;
  // // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // // _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
  // // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__ROTATING_PALETTE__ID;
  // // _segments[seg_idx].intensity_value = 127;
  // // _segments[seg_idx].transition.time_ms = 500;
  // // _segments[seg_idx].transition.rate_ms = 1000; 
  // // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // // row_start_iter+=8;

  // // _segments[seg_idx].pixel_range.start = row_start_iter;
  // // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // // _segments[seg_idx].grouping = 1;
  // // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // // _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
  // // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__STEPPING_PALETTE__ID;
  // // _segments[seg_idx].intensity_value = 127;
  // // _segments[seg_idx].transition.time_ms = 500;
  // // _segments[seg_idx].transition.rate_ms = 1000; 
  // // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // // row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_STATIC__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = PURPLE;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  row_start_iter+=8;
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID;
  _segments[seg_idx].intensity_value = 1;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 1);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_PERCENT__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 1);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID;
  _segments[seg_idx].intensity_value = 127;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 1);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(200);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  row_start_iter+=8;
  row_start_iter+=8;


  _segments[seg_idx].pixel_range.start = row_start_iter;       row_start_iter+=8;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS__ID;
  _segments[seg_idx].intensity_value = 255;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(255);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;       row_start_iter+=8;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID;
  _segments[seg_idx].intensity_value = 100;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(255);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;       row_start_iter+=8;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID;
  _segments[seg_idx].intensity_value = 100;
  _segments[seg_idx].transition.time_ms = 25;
  _segments[seg_idx].transition.rate_ms = 25;
  _segments[seg_idx].set_speed(255);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = crgb_to_col(CRGB(100,100,100)); // fades to black
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  _segments[seg_idx].pixel_range.start = row_start_iter;       row_start_iter+=8;
  _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  _segments[seg_idx].grouping = 1;
  _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
  _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_RAIN__ID;
  _segments[seg_idx].intensity_value = 100;
  _segments[seg_idx].transition.time_ms = FRAMETIME;
  _segments[seg_idx].transition.rate_ms = FRAMETIME;
  _segments[seg_idx].set_speed(255);
  _segments[seg_idx].colors[0] = RED;
  _segments[seg_idx].colors[1] = GREEN;
  _segments[seg_idx].colors[2] = BLUE;
  seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;

  row_start_iter+=8;
  row_start_iter+=8;


  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID;
  // _segments[seg_idx].intensity_value = 255;
  // _segments[seg_idx].transition.time_ms = FRAMETIME;
  // _segments[seg_idx].transition.rate_ms = FRAMETIME;
  // _segments[seg_idx].set_speed(255);
  // _segments[seg_idx].colors[0] = RED;
  // _segments[seg_idx].colors[1] = GREEN;
  // _segments[seg_idx].colors[2] = BLUE;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // row_start_iter+=8;

  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_CANDLE_MULTI__ID;
  // _segments[seg_idx].intensity_value = 255;
  // _segments[seg_idx].transition.time_ms = FRAMETIME;
  // _segments[seg_idx].transition.rate_ms = FRAMETIME;
  // _segments[seg_idx].set_speed(255);
  // _segments[seg_idx].colors[0] = RED;
  // _segments[seg_idx].colors[1] = GREEN;
  // _segments[seg_idx].colors[2] = BLUE;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // row_start_iter+=8;

  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_FIRE_FLICKER__ID;
  // _segments[seg_idx].intensity_value = 255;
  // _segments[seg_idx].transition.time_ms = FRAMETIME;
  // _segments[seg_idx].transition.rate_ms = FRAMETIME;
  // _segments[seg_idx].set_speed(255);
  // _segments[seg_idx].colors[0] = RED;
  // _segments[seg_idx].colors[1] = GREEN;
  // _segments[seg_idx].colors[2] = BLUE;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // row_start_iter+=8;
  
  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID;
  // _segments[seg_idx].intensity_value = 255;
  // _segments[seg_idx].transition.time_ms = FRAMETIME;
  // _segments[seg_idx].transition.rate_ms = FRAMETIME;
  // _segments[seg_idx].set_speed(255);
  // _segments[seg_idx].colors[0] = RED;
  // _segments[seg_idx].colors[1] = GREEN;
  // _segments[seg_idx].colors[2] = BLUE;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // row_start_iter+=8;


  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__FIREPLACE_1D_01__ID;
  // _segments[seg_idx].intensity_value = 255;
  // _segments[seg_idx].transition.time_ms = FRAMETIME;
  // _segments[seg_idx].transition.rate_ms = FRAMETIME;
  // _segments[seg_idx].set_speed(255);
  // _segments[seg_idx].colors[0] = RED;
  // _segments[seg_idx].colors[1] = GREEN;
  // _segments[seg_idx].colors[2] = BLUE;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  // row_start_iter+=8;




//last one so I know the others are working
  // _segments[seg_idx].pixel_range.start = row_start_iter;
  // _segments[seg_idx].pixel_range.stop  = row_start_iter+8;
  // _segments[seg_idx].grouping = 1;
  // _segments[seg_idx].setOption(SEG_OPTION_MIRROR, 0);
  // _segments[seg_idx].setOption(SEG_OPTION_REVERSED, 0);
  // _segments[seg_idx].palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
  // _segments[seg_idx].effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
  // _segments[seg_idx].intensity_value = 127;
  // _segments[seg_idx].transition.time_ms = 500;
  // _segments[seg_idx].transition.rate_ms = 1000;
  // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
  row_start_iter+=8;


  pCONT_iLight->CommandSet_BrtRGB_255(brightness);


}

#endif // USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER


#endif //USE_MODULE_LIGHTS_ANIMATOR



