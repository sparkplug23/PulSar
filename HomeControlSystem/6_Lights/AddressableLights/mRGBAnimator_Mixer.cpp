#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE



#ifdef ENABLE_PIXEL_FUNCTION_MIXER

void mRGBAnimator::init_mixer_defaults(){

  /** Christmas Outside Tree 2020 Animation Design
   * 
   * Palette C1: 10: The coloured icecle lights (6-8)
   * Palette C2: 11: As many colours as possible (15 or more, leaning orange, red and pink ie "warm")
   * Palette C3: 12: Berry green
   * 
   * Section A: "Coloured, Single Multiplier, Basic"
   * 
   * (A-1)
   * Palette:    Coloured Short (User10)
   * Flasher:    Slow Glow
   * Transition: Blend (1000)
   * Order:      InOrder
   * Multiplier: None
   * 
   * (A-2)
   * Palette:    Coloured Short (User10)
   * Flasher:    Slow Glow
   * Transition: Blend (1000)
   * Order:      Random                                     *
   * Multiplier: None
   *  
   * (A-3)
   * Palette:    Coloured Long (User11)                     *
   * Flasher:    Slow Glow
   * Transition: Blend (1000)
   * Order:      Random                                     *
   * Multiplier: None
   * 
   * Section B: "Horizontal array methods"
   *  
   * (B-6)       "Bars in order, Most colours"
   * Palette:    Coloured Long (User11)
   * Flasher:    Slow Glow
   * Transition: Blend   (3 seconds ie faster than above)
   * Order:      Random
   * Multiplier: None
   * 
   * (B-7)       "Bars in order, fewer colours"
   * Palette:    Coloured Short (User10)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * (B-8)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
   * Palette:    Coloured Short (User10)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * //to be tried, probably needs to use a different mapping?
   * (B-8 test to try)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
   * Palette:    Coloured Short (User10)    --use multiplier that makes "quarters" per row
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * Twinkle with mostly off
   * 
   * (A-4)
   * Palette:    Coloured Long (User11)
   * Flasher:    Twinkle                                    *
   * Transition: Instant                                    *
   * Order:      Random
   * Multiplier: None
   * 
   * (C-9)       "Twinkle with mostly off ie what I do with AlternateBrightnessMax"
   * Palette:    Coloured Long (User11)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   *  
   * (C-10)      "Twinkle as a pulsing effect, more on than off"
   * Palette:    Coloured Long (User11)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * Berry green
   * 
   * (D-11)      berry single pixels"
   * Palette:    Coloured (Berry Green)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * (D-12)       "berry rows inorder"
   * Palette:    Coloured (Berry Green)
   * Flasher:    Slow Glow
   * Transition: Instant
   * Order:      InOrder
   * Multiplier: None
   * 
   * 
   * need to add an override brightness level, or a ratio, ie so berry can be 80% of "set brigtnhess"
   * */


  mixer.mode.running_id = FLASHER_FUNCTION_MIXER_1_ID;

  // Set time at mode
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_0_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_1_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_2_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_3_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_4_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_5_ID] = 10;
  // mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_6_ID] = 10;

  // mixer.mode.list_len = FLASHER_FUNCTION_LENGTH_ID; //last mode

  // mixer.tSavedMillisToChangeAt = 0; //start at 0

  // flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

  mixer.mode.times.flashing_starttime.hour = 16;
  mixer.mode.times.flashing_starttime.minute = 0;
  mixer.mode.times.flashing_starttime.second = 0;

  mixer.mode.times.flashing_endtime.hour = 20;
  mixer.mode.times.flashing_endtime.minute = 0;
  mixer.mode.times.flashing_endtime.second = 0;

  // memset(mixer.mode.running_friendly_name_ctr,0,sizeof(mixer.mode.running_friendly_name_ctr));

  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_0_ID] = false;
  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_1_ID] = false;
  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_2_ID] = false;
  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_3_ID] = false;
  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_4_ID] = false;
  // mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_5_ID] = false;

  // flashersettings.random_transitions.array_index_length = 0;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 20;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 100;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;

  // Use switch case to all (no arg as reset all or target a specific one)

//notes
  uint8_t brightness_low_value = 75;
  // how many to update ?? 

  uint8_t flasher_id = 0;

  // Group 1 "Static, few"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; // amount to change as percentage
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 5000;
  mixer.group[flasher_id].transition.rate = 10000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_09_ID;

  // Group 2 "Random, slow, few"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 2500;
  mixer.group[flasher_id].transition.rate = 5000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_09_ID;

  // Group 3 "Random, fast, many"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 30; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = false;
  mixer.group[flasher_id].transition.time = 0;
  mixer.group[flasher_id].transition.rate = 600;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_10_ID;

// Section B

  // Group 4 "random, rows, slow"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 20; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = false;
  mixer.group[flasher_id].transition.time = 5003;
  mixer.group[flasher_id].transition.rate = 5003;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = true;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_11_ID;

  // Group 5 "Random, rows, faster"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 2; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 500;
  mixer.group[flasher_id].transition.rate = 1000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = true;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_11_ID;

  // Group 6 "Static/Inorder, rows, few  colours"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 10000;
  mixer.group[flasher_id].transition.rate = 10000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = true;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_09_ID;

  // Group 7 "Now rotate the rows"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 500;
  mixer.group[flasher_id].transition.rate = 500;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = true;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SEQUENTIAL_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_09_ID;

// Berry green
// Group 8
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = 100;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 2006;
  mixer.group[flasher_id].transition.rate = 2006;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = true;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_BERRY_GREEN_ID;

// 9

  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = 100;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = true;
  mixer.group[flasher_id].transition.time = 2000;
  mixer.group[flasher_id].transition.rate = 2000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_BERRY_GREEN_ID;



// Section C  : Twinkle

  // Group 10
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 3; //1000
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = false;
  mixer.group[flasher_id].transition.time = 1000;
  mixer.group[flasher_id].transition.rate = 1000;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_10_ID;

  // Group 10 "where palette 5 has off colours too" NEW method, move in "steps across palette" (ie show 1, 2, or 3, or 4, moving in order/randomly picking (but never repeating))
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 100; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = false;
  mixer.group[flasher_id].transition.time = 200;
  mixer.group[flasher_id].transition.rate = 400;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_10_ID;



  // Group 10 "where palette 5 has off colours too"
  mixer.group[flasher_id].flags.enable_force_preset_brightness_scaler = true;
  mixer.group[flasher_id].pixels_to_update_as_percentage = 30; 
  mixer.group[flasher_id].ifenabled_forced_brightness_level_percentage = brightness_low_value;
  mixer.group[flasher_id].time_on_secs_decounter = -1;
  mixer.group[flasher_id].starttime = {0, 17, 0, 0};
  mixer.group[flasher_id].endtime   = {0, 18, 0, 0};
  mixer.group[flasher_id].enable_skip_restricted_by_time = 0;
  mixer.group[flasher_id].isenabled = false;
  mixer.group[flasher_id].transition.time = 600;
  mixer.group[flasher_id].transition.rate = 600;
  mixer.group[flasher_id].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
  mixer.group[flasher_id].run_time_duration_sec = 60;
  mixer.group[flasher_id].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
  mixer.group[flasher_id].pixel_multiplier_enabled = false;
  mixer.group[flasher_id].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
  mixer.group[flasher_id].flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  mixer.group[flasher_id++].palette_id = pCONT_iLight->PALETTELIST_STATIC_CHRISTMAS_05_ID;



  mixer.enabled_mixer_count = flasher_id-1;
  AddLog_P(LOG_LEVEL_INFO, PSTR("enabled_mixer_count=%d"),mixer.enabled_mixer_count);


}



void mRGBAnimator::SubTask_Flasher_Animate_Mixer(){

  uint8_t flag_reset = false;

  // Run every second
  if(mTime::TimeReachedNonReset(&mixer.tSavedTrigger,1000)||pCONT_set->fSystemRestarted){

    if(mixer.group[mixer.running_id].time_on_secs_decounter == 0){ 
      
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer[%d]..time_on_secs_decounter =%d END %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter,sizeof(mixer));

      uint8_t new_group_found = false;
      mqtthandler_mixer_teleperiod.flags.SendNow = true;
      
      if(mixer.running_id++>mixer.enabled_mixer_count){ // greater or "EQUAL", needed as its moving index before using it again
        mixer.running_id = FLASHER_FUNCTION_MIXER_1_ID;
      }
  
//make check optional values for which to check with (use bit OR map?)
//bitval = HOUR_BIT_CHECK | MINUTE_BIT_CHECK < bit check, 
// if 0, must check //optional pass it default is 1, ie 0xFF, check all
//if any set to 1, then it must agree, if 0, check all
// CheckBetween_Day_DateTimesShort(start,end, bit_val)


      // if(!pCONT_time->CheckBetween_Day_DateTimesShort(&mixer.group[mixer.running_id].starttime,
      //                                              &mixer.group[mixer.running_id].endtime)){
      //   //add here to pass if the running_id is restricted
      //   if(mixer.group[mixer.running_id].enable_skip_restricted_by_time){ //if true, skip
      //     do{ //skip until we reach an acceptable mixer      
      //       mixer.running_id++;     
      //       if(mixer.running_id>+FLASHER_FUNCTION_MIXER_LENGTH_ID){
      //         mixer.running_id = FLASHER_FUNCTION_MIXER_1_ID;
      //       }
      //       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode = WHILE LO OP MULTIPLE");
      //     }while(mixer.group[mixer.running_id].enable_skip_restricted_by_time);
      //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode");
      //     //mixer.times.skip_restricted_by_time_isactive = true;
      //   }else{
      //     //mixer.times.skip_restricted_by_time_isactive = false;
      //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"NOT Skipping restricted mode");
      //   }
      //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"OUTSIDE restricted CheckBetween_Day_DateTimes");
      // }else{
      //   //mixer.mode.times.skip_restricted_by_time_isactive = false;
      //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"INSIDE restricted CheckBetween_Day_DateTimes");
      // }


      // Progress to next active mode IF ACTIVE
      // for(id_next=id;id_next<FLASHER_FUNCTION_MIXER_LENGTH_ID;id_next){
      //   if(mixer.group[id_next].isenabled){
      //     // IF time is valid
      //     //if(pCONT->mt->mtime.isvalid){
      //       // IF is time restricted AND is not between those times THEN skip it
      //     //   if(mixer.group[id_next].enable_skip_restricted_by_time
      //                &&(!pCONT->mt->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
      //     //     id_next++;
      //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
      //     //     break; //finish for lop with current id_next
      //     //   }else{
      //          //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
      //     //   }
      //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
      //          id_next++;
      //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
      //     //     break; //finish for lop with current id_next
      //     // }// end if(isvalid)
      //   }else{// end if(isenabled)
      //     id_next++; // do move on
      //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
      //   }
      // }// end FOR



//       // Progress to next active mode IF ACTIVE
//       // for(id_next=id;id_next<FLASHER_FUNCTION_MIXER_NONE_ID;id_next){
//        // do{
//       //   if(mixer.group[id_next].isenabled){
//       //     // IF time is valid
//       //     //if(pCONT->mt->mtime.isvalid){
//       //       // IF is time restricted AND is not between those times THEN skip it
//       //     //   if(mixer.group[id_next].enable_skip_restricted_by_time&&(!pCONT->mt->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
//       //     //     id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
//       //     //     break; //finish for lop with current id_next
//       //     //   }else{
//       //          //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
//       //     //   }
//       //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
//       //          id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
//       //     //     break; //finish for lop with current id_next
//       //     // }// end if(isvalid)
//       //   }else{// end if(isenabled)
//       //     id_next++; // do move on
//       //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
//       //   }
//       // }// end FOR
      
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\"=%d"),"mixer.running_id",mixer.running_id);

//       // if(mixer.running_id >= FLASHER_FUNCTION_MIXER_NONE_ID){ //lop around, move forward, but retry function on next go around
//       //   mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;
//       //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;");
//       //   return; // Quit early
//       // }
      
      // Reset after return, so this function will be called multiple times until it passes this point
      mixer.group[mixer.running_id].time_on_secs_decounter = -1; //STOP
      mixer.tSavedTrigger = millis();

      LoadMixerGroupByID(mixer.running_id);
      
      // Next mode 
      // mixer.running_id++;
      // wrap around

    }else{ 
      mixer.group[mixer.running_id].time_on_secs_decounter--; //if on
      mixer.tSavedTrigger = millis(); //reset if this was reached
      //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "Time until MillisReached = %d"),mixer.tSavedMillisToChangeAt-millis());
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer[%d]..time_on_secs_decounter = %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter);  
    
      if(mixer.group[mixer.running_id].time_on_secs_decounter < -2){
        mixer.group[mixer.running_id].time_on_secs_decounter = 0;
        AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "RESET < -1 mixer[%d]..time_on_secs_decounter = %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter);  
      }
    }

  } //tSavedTrigger  

} // SubTask_Flasher_Animate


void mRGBAnimator::LoadMixerGroupByID(uint8_t id){

  // Only runs if new state was found
  flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // refresh by calling init colour generator

  pCONT_iLight->animation.palette_id = mixer.group[id].palette_id;          
  pCONT_iLight->animation.transition.order_id = mixer.group[id].animation_transition_order;
  flashersettings.function = mixer.group[id].flashersettings_function;          
  mixer.group[id].time_on_secs_decounter = mixer.group[id].run_time_duration_sec*((float)mixer.run_time_duration_scaler_as_percentage/100);
  //mixer.time_on_secs_active = mixer.group[id].run_time_duration_sec;

  pixel_group.flags.multiplier_mode_id = mixer.group[id].pixel_multiplier_id;
  pixel_group.flags.fEnabled = mixer.group[id].pixel_multiplier_enabled;

  pCONT_iLight->animation.transition.rate_ms.val = ConvertTimeToMilliSecondsWithUnit(mixer.group[id].transition.rate,mixer.group[id].transition.time_unit_id);
  pCONT_iLight->animation.transition.time_ms.val = ConvertTimeToMilliSecondsWithUnit(mixer.group[id].transition.time,mixer.group[id].transition.time_unit_id);

  pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = mixer.group[id].pixels_to_update_as_percentage; 




  // uint8_t brightness = 0;
  if(mixer.group[id].flags.enable_force_preset_brightness_scaler){
    // uint8_t brightness_set_255 = pCONT_iLight->getBriRGB();
    // float brightness_set = (float)brightness_set_255/(float)255.0f;   // range 0 - 1
    // float brightness_new = brightness_set * ((mixer.group[id].ifenabled_forced_brightness_level_percentage)/(100.0f));
    // uint8_t brightness_adjusted_100 = brightness_new * 100;
    uint8_t brightness_adjusted_255 = map(mixer.group[id].ifenabled_forced_brightness_level_percentage, 0,100, 0,255);
    AddLog_P(LOG_LEVEL_TEST, PSTR("brightness_adjsuted_255=%d"),brightness_adjusted_255);
    pCONT_iLight->setBriRGB(brightness_adjusted_255);
  }else{
    //pCONT_iLight->setBriRGB(brightness_adjsuted_255);
  }

  #ifdef USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
    char result[100];
    pCONT_sup->GetTextIndexed_P(result, sizeof(result), id, PM_OUTSIDE_TREE_MIXER_DESCRIPTION);  // should this be _P?
    AddLog_P(LOG_LEVEL_INFO, PSTR("Mixer routine \"%s\""), result);          
  #endif // USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  
  AddLog_P(LOG_LEVEL_TEST, PSTR("Settings%d rate_ms=%d"),id,pCONT_iLight->animation.transition.rate_ms.val);
  AddLog_P(LOG_LEVEL_TEST, PSTR("Settings time_ms=%d"),pCONT_iLight->animation.transition.time_ms.val);

}




// #ifdef ENABLE_PIXEL_FUNCTION_MIXER

uint8_t mRGBAnimator::ConstructJSON_Mixer(uint8_t json_level){

//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Mixer"));

//   DynamicJsonDocument doc(1400);
//   JsonObject root = doc.to<JsonObject>();

//   //active values

//   //all mixer values in other mqtt

//   root["enabled"] = mixer.flags.Enabled;
//   root["running_id"] = mixer.running_id;
//   root["seconds"] = mixer.group[mixer.running_id].time_on_secs_decounter;//round((mixer.tSavedMillisToChangeAt-millis())/1000);
  
//   root["time_scaler"] = mixer.time_scaler;

//   JsonObject timesobj = root.createNestedObject("times");

//     // char time_on[40]; memset(time_on,0,sizeof(time_on));
//     // sprintf(time_on, "%02d:%02d:%02d",mixer.mode.times.flashing_starttime.hour,
//     //                                   mixer.mode.times.flashing_starttime.minute,
//     //                                   mixer.mode.times.flashing_starttime.second);
//     // timesobj["flashing_starttime"] = time_on;

//     // char time_off[40]; memset(time_off,0,sizeof(time_off));
//     // sprintf(time_off, "%02d:%02d:%02d",mixer.mode.times.flashing_endtime.hour,
//     //                                   mixer.mode.times.flashing_endtime.minute,
//     //                                   mixer.mode.times.flashing_endtime.second);
//     // timesobj["flashing_endtime"] = time_off;

//     //timesobj["skip_restricted_by_time_isactive"] = mixer.skip_restricted_by_time_isactive;

//     timesobj["running_friendly_name_ctr"] = mixer.running_friendly_name_ctr;



    
//     JsonArray enable_skip_restricted_by_time_arr = timesobj.createNestedArray("enable_skip_restricted_by_time");
//     for(int i=0;i<FLASHER_FUNCTION_MIXER_MAX;i++){
//       //enable_skip_restricted_by_time_arr.add(mixer.group[i].times.enable_skip_restricted_by_time);
//     }

//     JsonArray time_on_secs_arr = timesobj.createNestedArray("time_on_secs");
//     for(int i=0;i<FLASHER_FUNCTION_MIXER_MAX;i++){
//       //time_on_secs_arr.add(mixer.mode.time_on_secs[i]);
//     }

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

// return (data_buffer.payload.len>3?1:0);
  return 0;

}

// #endif // #ifdef ENABLE_PIXEL_FUNCTION_MIXER

#endif //ENABLE_PIXEL_FUNCTION_MIXER


#endif //USE_MODULE_LIGHTS_ADDRESSABLE




