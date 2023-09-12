/**
 * @brief 
 * 
 * 
 * playlists then takes presets and moves between them based on times etc
 * 
 * 
 */



// #include "mAnimatorLight.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR



// #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST

// void mAnimatorLight::init_mixer_defaults(){

//   /** Christmas Outside Tree 2020 Animation Design
//    * 
//    * Palette C1: 10: The coloured icecle lights (6-8)
//    * Palette C2: 11: As many colours as possible (15 or more, leaning orange, red and pink ie "warm")
//    * Palette C3: 12: Berry green
//    * 
//    * Section A: "Coloured, Single Multiplier, Basic"
//    * 
//    * (A-1)
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (A-2)
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      Random                                     *
//    * Multiplier: None
//    *  
//    * (A-3)
//    * Palette:    Coloured Long (User11)                     *
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      Random                                     *
//    * Multiplier: None
//    * 
//    * Section B: "Horizontal array methods"
//    *  
//    * (B-6)       "Bars in order, Most colours"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Blend   (3 seconds ie faster than above)
//    * Order:      Random
//    * Multiplier: None
//    * 
//    * (B-7)       "Bars in order, fewer colours"
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (B-8)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * //to be tried, probably needs to use a different mapping?
//    * (B-8 test to try)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
//    * Palette:    Coloured Short (User10)    --use multiplier that makes "quarters" per row
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * Twinkle with mostly off
//    * 
//    * (A-4)
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Twinkle                                    *
//    * Transition: Instant                                    *
//    * Order:      Random
//    * Multiplier: None
//    * 
//    * (C-9)       "Twinkle with mostly off ie what I do with AlternateBrightnessMax"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    *  
//    * (C-10)      "Twinkle as a pulsing effect, more on than off"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * Berry green
//    * 
//    * (D-11)      berry single pixels"
//    * Palette:    Coloured (Berry Green)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (D-12)       "berry rows inorder"
//    * Palette:    Coloured (Berry Green)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * 
//    * need to add an override brightness level, or a ratio, ie so berry can be 80% of "set brigtnhess"
//    * */

//   /** Christmas Outside Tree 2021 Animation Design
//    * 
//    * Palette C1: 10: The coloured icecle lights (6-8)
//    * Palette C2: 11: As many colours as possible (15 or more, leaning orange, red and pink ie "warm")
//    * Palette C3: 12: "Warm colours" used for 2 (or 3?) sequential picking from palette
//    * 
//    * Use a palette that will also have white, warm white, side by side... ie tree can go day white, then warm white, then all the other colours, only used in palette sequential stepper
//    * 
//    * "rows" this year, shall be made from 3 rows, to make it more noticable
//    * 
//    * animations of changing quickly are only enabled before 8pm, after, only static or slow glow
//    * 
//    * 
//    * >> Overview basic <<
//    * (1) Static, warm palette
//    * (2) Slow glow, random, slow changing
//    * (3) "Pop" changing or warm palette, (instant change)
//    * (4) "Burst pops", the nice decaying brightness pop from last year. Ie 10% randomly instant changes, while all 100% will decay to a lower brightness level (but still overall on)
//    * (5) 3 row, in order palette, have them squentially rotate up the tree (ie bands will move up the tree)
//    * 
//    * (6) twinkle effect, have single colours turn off for one cycle, ie every 1 second, new animation, just instant overwrite a pixel off, next time it will turb back on
//    * (7) twinkle effect, with the "on" this time, where it turns to white (lower the brightness of the white so its not insaley bright)
//    * (8) lighting effect with raindbow palette, have the tree mostly off, with burst of colours
//    * 
//    * (9) transition from lighting effect to static, ie, a special static version that auto flashes pixels from OFF to full set brightness
//    *       - the effect would be all pixels are set to the desired colour, but the brightness will twinkle on until they all reach the desired colour.
//    * 
//    * 
//    * 
//    * Section A: "Coloured, Single Multiplier, Basic"
//    * 
//    * (A-1)
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (A-2)
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      Random                                     *
//    * Multiplier: None
//    *  
//    * (A-3)
//    * Palette:    Coloured Long (User11)                     *
//    * Flasher:    Slow Glow
//    * Transition: Blend (1000)
//    * Order:      Random                                     *
//    * Multiplier: None
//    * 
//    * Section B: "Horizontal array methods"
//    *  
//    * (B-6)       "Bars in order, Most colours"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Blend   (3 seconds ie faster than above)
//    * Order:      Random
//    * Multiplier: None
//    * 
//    * (B-7)       "Bars in order, fewer colours"
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (B-8)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
//    * Palette:    Coloured Short (User10)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * //to be tried, probably needs to use a different mapping?
//    * (B-8 test to try)       "Bars in order, fewer colours (so movement is clearer)  - sequential movement"
//    * Palette:    Coloured Short (User10)    --use multiplier that makes "quarters" per row
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * Twinkle with mostly off
//    * 
//    * (A-4)
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Twinkle                                    *
//    * Transition: Instant                                    *
//    * Order:      Random
//    * Multiplier: None
//    * 
//    * (C-9)       "Twinkle with mostly off ie what I do with AlternateBrightnessMax"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    *  
//    * (C-10)      "Twinkle as a pulsing effect, more on than off"
//    * Palette:    Coloured Long (User11)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * Berry green
//    * 
//    * (D-11)      berry single pixels"
//    * Palette:    Coloured (Berry Green)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * (D-12)       "berry rows inorder"
//    * Palette:    Coloured (Berry Green)
//    * Flasher:    Slow Glow
//    * Transition: Instant
//    * Order:      InOrder
//    * Multiplier: None
//    * 
//    * 
//    * need to add an override brightness level, or a ratio, ie so berry can be 80% of "set brigtnhess"
//    * */

//   mixer.mode.running_id = EFFECTS_FUNCTION_MIXER_01_ID;

//   // Set time at mode
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_0_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_1_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_2_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_3_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_4_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_5_ID] = 10;
//   // mixer.mode.time_on_secs[EFFECTS_FUNCTION_MIXER_6_ID] = 10;

//   // mixer.mode.list_len = EFFECTS_FUNCTION_LENGTH_ID; //last mode

//   // mixer.tSavedMillisToChangeAt = 0; //start at 0

//   // flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

//   mixer.mode.times.flashing_starttime.hour = 16;
//   mixer.mode.times.flashing_starttime.minute = 0;
//   mixer.mode.times.flashing_starttime.second = 0;

//   mixer.mode.times.flashing_endtime.hour = 20;
//   mixer.mode.times.flashing_endtime.minute = 0;
//   mixer.mode.times.flashing_endtime.second = 0;

//   // memset(mixer.mode.running_friendly_name_ctr,0,sizeof(mixer.mode.running_friendly_name_ctr));

//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_0_ID] = false;
//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_1_ID] = false;
//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_2_ID] = false;
//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_3_ID] = false;
//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_4_ID] = false;
//   // mixer.mode.times.enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_5_ID] = false;

//   // flashersettings.random_transitions.array_index_length = 0;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 20;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 100;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
//   // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;

//   // Pushing default states for all flags
//   for(uint8_t id=0; id<EFFECTS_FUNCTION_MIXER_MAX; id++){

//     mixer.group[id].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false;
//     mixer.group[id].brightness_higher_255 = 255*0.9;  
//     mixer.group[id].brightness_lower_255  = 255*0.2;

//   }


  




//   // Use switch case to all (no arg as reset all or target a specific one)

// //notes
//   uint8_t brightness_low_value = 70;
//   // how many to update ?? 

//   // uint8_t flasher_id = 0;

//   /**
//    * Basic static or blending
//    * */

//   // Group 0 "Static, few"
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].flags.enable_force_preset_brightness_scaler = true;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].pixels_to_update_as_percentage = 100; // amount to change as percentage
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].isenabled = true;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].transition.time = 4500;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].transition.rate = 5000;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].run_time_duration_sec = 60;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].pixel_multiplier_enabled = false;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_09_ID;
//   mixer.group[EFFECTS_FUNCTION_MIXER_01_ID].starttime = {8, 1, 2, 3};

// //   // Group 1 "Random, slow, few"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].pixels_to_update_as_percentage = 30; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].isenabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].transition.time = 2500;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].transition.rate = 10000;//5000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_02_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_09_ID;

// //   // Group 2 "Random, fast, many"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].pixels_to_update_as_percentage = 30; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].transition.time = 1000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].transition.rate = 10000;//600;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_03_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_10_ID;

// // // Section B

// //   // Group 3 "random, rows, slow"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].pixels_to_update_as_percentage = 40;  //needs to be higher to increase chance of first pixels changed as they are multipled
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].transition.time = 4500;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].transition.rate = 10000;//5000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].pixel_multiplier_enabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_04_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_11_ID;

// //   // Group 4 "Random, rows, faster"
// //   // EFFECTS_FUNCTION_MIXER_05_ID
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].pixels_to_update_as_percentage = 40; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].isenabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].transition.time = 1500;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].transition.rate = 10000;//2000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].pixel_multiplier_enabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_05_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_12_ID;

// //   // Group 5 "Static/Inorder, rows, few  colours"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].isenabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].transition.time = 9500;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].transition.rate = 10000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].pixel_multiplier_enabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_06_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_09_ID;

// //   // Group 6 "Now rotate the rows"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].isenabled = true; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].transition.time = 250;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].transition.rate = 300;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].pixel_multiplier_enabled = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].flashersettings_function = EFFECTS_FUNCTION_SEQUENTIAL_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_07_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_09_ID;

// // // Berry green
// // // Group 7
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].flags.enable_force_preset_brightness_scaler = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixels_to_update_as_percentage = 100; 
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].ifenabled_forced_brightness_level_percentage = 100;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].isenabled = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.time = 2000;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.rate = 2000;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].run_time_duration_sec = 60;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixel_multiplier_enabled = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].palette.id = mPaletteI->PALETTELIST_STATIC_BERRY_GREEN_ID;

// // // // 8

// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].flags.enable_force_preset_brightness_scaler = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixels_to_update_as_percentage = 100; 
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].ifenabled_forced_brightness_level_percentage = 100;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].isenabled = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.time = 2000;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.rate = 2000;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].run_time_duration_sec = 60;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixel_multiplier_enabled = false;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].palette.id = mPaletteI->PALETTELIST_STATIC_BERRY_GREEN_ID;

// // //   // Idea
// // //   // Darker many colours, with a few (instant) bright leds changing to brighter (and thus transitioning into the new twinkle modes)
// // // //9
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].flags.enable_force_preset_brightness_scaler = true;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixels_to_update_as_percentage = 100; //1000
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].isenabled = false;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.time = 0;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.rate = 600;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].run_time_duration_sec = 60;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixel_multiplier_enabled = false;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].flashersettings_function = EFFECTS_FUNCTION_SEQUENTIAL_ID;
// // //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].palette.id = mPaletteI->PALETTELIST_STATIC_BERRY_GREEN_ID;



// // // Section C  : Twinkle

// //   // Group 10
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixels_to_update_as_percentage = 3; //1000
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.time = 950;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.rate = 1000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].flashersettings_function = EFFECTS_FUNCTION_STEPPING_PALETTE_ID;//EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_08_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_12_ID;

// // // Step thorugh

// // //too much blue
// //   // Group 11 "where palette 5 has off colours too" NEW method, move in "steps across palette" (ie show 1, 2, or 3, or 4, moving in order/randomly picking (but never repeating))
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value-10;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.time = 4950;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.rate = 5000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].run_time_duration_sec = 120;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].flashersettings_function = EFFECTS_FUNCTION_STEPPING_PALETTE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_09_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_15_ID;

// // //too much pink
// //   // Group 11 "where palette 5 has off colours too" NEW method, move in "steps across palette" (ie show 1, 2, or 3, or 4, moving in order/randomly picking (but never repeating))
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value-10;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.time = 4950;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.rate = 5000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].run_time_duration_sec = 120;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].flashersettings_function = EFFECTS_FUNCTION_STEPPING_PALETTE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_10_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_12_ID;



// //   // Group 12 "where palette 5 has off colours too"
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].flags.enable_force_preset_brightness_scaler = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;

// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].brightness_higher_255 = 255;    //I can have brightest, as less will be on
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].brightness_lower_255  = 70;  // dont have totally off
  
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].transition.time = 950;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].transition.rate = 1000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].run_time_duration_sec = 60;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_12_ID;  // palette is the desired brightness!

// // //testing
// //   // mixer.group[EFFECTS_FUNCTION_MIXER_11_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;




// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].flags.enable_force_preset_brightness_scaler = true;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].pixels_to_update_as_percentage = 100; 
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].ifenabled_forced_brightness_level_percentage = brightness_low_value;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].isenabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].transition.time = 1000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].transition.rate = 60000;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].transition.time_unit_id = TIME_UNIT_MILLISECONDS_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].run_time_duration_sec = 60*5;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_NONE_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].pixel_multiplier_enabled = false;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// //   mixer.group[EFFECTS_FUNCTION_MIXER_12_ID].palette_id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_12_ID;


//   // Animation idea #1
//   // mostly dark colours, with pulsing some brighter for a "shimmer" look

//   // Animation idea #2
//   // mostly dark colours, with pulsing some brighter for a "shimmer" look

//   mixer.enabled_mixer_count = EFFECTS_FUNCTION_MIXER_LENGTH_ID-1;//flasher_id;
//   AddLog(LOG_LEVEL_INFO, PSTR("enabled_mixer_count=%d"),mixer.enabled_mixer_count);

// }



// void mAnimatorLight::SubTask_Flasher_Animate_Mixer()
// {

//   uint8_t flag_reset = false;

//   // Run every second
//   if(mTime::TimeReachedNonReset(&mixer.tSavedTrigger,1000)||pCONT_set->fSystemRestarted)
//   {

//     if(mixer.group[mixer.running_id].time_on_secs_decounter == 0)
//     {
      
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer[%d]..time_on_secs_decounter =%d END %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter,sizeof(mixer));

//       uint8_t new_group_found = false;
//       mqtthandler_mixer_teleperiod.flags.SendNow = true;
      
//       if(mixer.running_id++>=EFFECTS_FUNCTION_MIXER_LENGTH_ID)
//       { // greater or "EQUAL", needed as its moving index before using it again
        
//         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO DEBUG_INSERT_PAGE_BREAK "mixer was %d|%d, reseting %d"),mixer.running_id,mixer.enabled_mixer_count,EFFECTS_FUNCTION_MIXER_01_ID);

//         mixer.running_id = EFFECTS_FUNCTION_MIXER_01_ID;
//       }
  
// //make check optional values for which to check with (use bit OR map?)
// //bitval = HOUR_BIT_CHECK | MINUTE_BIT_CHECK < bit check, 
// // if 0, must check //optional pass it default is 1, ie 0xFF, check all
// //if any set to 1, then it must agree, if 0, check all
// // CheckBetween_Day_DateTimesShort(start,end, bit_val)


//       if(!pCONT_time->CheckBetween_Day_DateTimesShort(&mixer.group[mixer.running_id].starttime,
//                                                    &mixer.group[mixer.running_id].endtime))
//       {
//         //add here to pass if the running_id is restricted
//         if(mixer.group[mixer.running_id].enable_skip_restricted_by_time)
//         { //if true, skip
//           do{ //skip until we reach an acceptable mixer      
//             mixer.running_id++;     
//             if(mixer.running_id>+EFFECTS_FUNCTION_MIXER_LENGTH_ID){
//               mixer.running_id = EFFECTS_FUNCTION_MIXER_01_ID;
//             }
//             AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode = WHILE LO OP MULTIPLE");
//           }while(mixer.group[mixer.running_id].enable_skip_restricted_by_time);
//           AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode");
//           //mixer.times.skip_restricted_by_time_isactive = true;
//         }else{
//           //mixer.times.skip_restricted_by_time_isactive = false;
//           AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"NOT Skipping restricted mode");
//         }
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"OUTSIDE restricted CheckBetween_Day_DateTimes");
//       }else{
//         //mixer.mode.times.skip_restricted_by_time_isactive = false;
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"INSIDE restricted CheckBetween_Day_DateTimes");
//       }


//       // Progress to next active mode IF ACTIVE
//       // for(id_next=id;id_next<EFFECTS_FUNCTION_MIXER_LENGTH_ID;id_next){
//       //   if(mixer.group[id_next].isenabled){
//       //     // IF time is valid
//       //     //if(pCONT_time->RtcTime.isvalid){
//       //       // IF is time restricted AND is not between those times THEN skip it
//       //     //   if(mixer.group[id_next].enable_skip_restricted_by_time
//       //                &&(!pCONT_time->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
//       //     //     id_next++;
//       //     //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
//       //     //     break; //finish for lop with current id_next
//       //     //   }else{
//       //          //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
//       //     //   }
//       //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
//       //          id_next++;
//       //     //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
//       //     //     break; //finish for lop with current id_next
//       //     // }// end if(isvalid)
//       //   }else{// end if(isenabled)
//       //     id_next++; // do move on
//       //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
//       //   }
//       // }// end FOR



// //       // Progress to next active mode IF ACTIVE
// //       // for(id_next=id;id_next<EFFECTS_FUNCTION_MIXER_NONE_ID;id_next){
// //        // do{
// //       //   if(mixer.group[id_next].isenabled){
// //       //     // IF time is valid
// //       //     //if(pCONT_time->RtcTime.isvalid){
// //       //       // IF is time restricted AND is not between those times THEN skip it
// //       //     //   if(mixer.group[id_next].enable_skip_restricted_by_time&&(!pCONT_time->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
// //       //     //     id_next++;
// //       //     //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
// //       //     //     break; //finish for lop with current id_next
// //       //     //   }else{
// //       //          //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
// //       //     //   }
// //       //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
// //       //          id_next++;
// //       //     //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
// //       //     //     break; //finish for lop with current id_next
// //       //     // }// end if(isvalid)
// //       //   }else{// end if(isenabled)
// //       //     id_next++; // do move on
// //       //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
// //       //   }
// //       // }// end FOR
      
// //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\"=%d"),"mixer.running_id",mixer.running_id);

// //       // if(mixer.running_id >= EFFECTS_FUNCTION_MIXER_NONE_ID){ //lop around, move forward, but retry function on next go around
// //       //   mixer.running_id = EFFECTS_FUNCTION_MIXER_0_ID;
// //       //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"mixer.running_id = EFFECTS_FUNCTION_MIXER_0_ID;");
// //       //   return; // Quit early
// //       // }
      
//       // Reset after return, so this function will be called multiple times until it passes this point
//       mixer.group[mixer.running_id].time_on_secs_decounter = -1; //STOP
//       mixer.tSavedTrigger = millis();

//       LoadMixerGroupByID(mixer.running_id);
      
//       // Next mode 
//       // mixer.running_id++;
//       // wrap around

//     }
//     else
//     { 
//       mixer.group[mixer.running_id].time_on_secs_decounter--; //if on
//       mixer.tSavedTrigger = millis(); //reset if this was reached
//       //AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "Time until MillisReached = %d"),mixer.tSavedMillisToChangeAt-millis());
//       AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer[%d]..time_on_secs_decounter = %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter);  
    
//       if(mixer.group[mixer.running_id].time_on_secs_decounter < -2)
//       {
//         mixer.group[mixer.running_id].time_on_secs_decounter = 0;
//         AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "RESET < -1 mixer[%d]..time_on_secs_decounter = %d"),mixer.running_id,mixer.group[mixer.running_id].time_on_secs_decounter);  
//       }

//     }

//   } //tSavedTrigger  

// } // SubTask_Flasher_Animate


// void mAnimatorLight::LoadMixerGroupByID(uint8_t id)
// {

//   // // Only runs if new state was found
//   // flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID; // refresh by calling init colour generator

//   // pCONT_iLight->animation.palette.id = mixer.group[id].palette_id;          
//   // pCONT_iLight->animation.transition.order_id = mixer.group[id].animation_transition_order;
//   // flashersettings.function = mixer.group[id].flashersettings_function;          
//   // mixer.group[id].time_on_secs_decounter = mixer.group[id].run_time_duration_sec*((float)mixer.run_time_duration_scaler_as_percentage/100);
//   // //mixer.time_on_secs_active = mixer.group[id].run_time_duration_sec;

//   // pixel_group.flags.multiplier_mode_id = mixer.group[id].pixel_multiplier_id;
//   // pixel_group.flags.fEnabled = mixer.group[id].pixel_multiplier_enabled;

//   // pCONT_iLight->animation.transition.rate_ms = ConvertTimeToMilliSecondsWithUnit(mixer.group[id].transition.rate,mixer.group[id].transition.time_unit_id);
//   // pCONT_iLight->animation.transition.time_ms = ConvertTimeToMilliSecondsWithUnit(mixer.group[id].transition.time,mixer.group[id].transition.time_unit_id);

//   // pCONT_iLight->animation.transition.pixels_to_update_as_percentage = mixer.group[id].pixels_to_update_as_percentage; 
//   // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);

//   // AddLog(LOG_LEVEL_TEST, PSTR("LoadMixerGroupByID strip_size_requiring_update=%d"),strip_size_requiring_update);


//   // pCONT_iLight->animation_override.fRefreshAllPixels = true;

//   // // uint8_t brightness = 0;
//   // if(mixer.group[id].flags.enable_force_preset_brightness_scaler){
//   //   // uint8_t brightness_set_255 = pCONT_iLight->getBriRGB();
//   //   // float brightness_set = (float)brightness_set_255/(float)255.0f;   // range 0 - 1
//   //   // float brightness_new = brightness_set * ((mixer.group[id].ifenabled_forced_brightness_level_percentage)/(100.0f));
//   //   // uint8_t brightness_adjusted_100 = brightness_new * 100;
//   //   uint8_t brightness_adjusted_255 = map(mixer.group[id].ifenabled_forced_brightness_level_percentage, 0,100, 0,255);
//   //   AddLog(LOG_LEVEL_TEST, PSTR("brightness_adjsuted_255=%d"),brightness_adjusted_255);
//   //   pCONT_iLight->setBriRGB(brightness_adjusted_255);
//   // }else{
//   //   //pCONT_iLight->setBriRGB(brightness_adjsuted_255);
//   // }


//   // if(mixer.group[id].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice){

//   //   pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = true;
//   //   pCONT_iLight->brtRGB_limits.upper = mixer.group[id].brightness_higher_255;  
//   //   pCONT_iLight->brtRGB_limits.lower = mixer.group[id].brightness_lower_255;  
//   //   pCONT_iLight->animation.flags.use_gamma_for_brightness = true;  //forced testing

//   // }else{

//   //   pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false;
//   //   pCONT_iLight->brtRGB_limits.upper = 255;   //reset full range
//   //   pCONT_iLight->brtRGB_limits.lower = 0;    //reset full range
//   //   pCONT_iLight->animation.flags.use_gamma_for_brightness = false; //forced testing

//   // }

//   //   //AddLog(LOG_LEVEL_TEST, PSTR("Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice=%d"),mixer.group[id].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice);

//   // #ifdef USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
//   //   char result[100];
//   //   char result2[120];
//   //   pCONT_sup->GetTextIndexed_P(result, sizeof(result), id, PM_OUTSIDE_TREE_MIXER_DESCRIPTION);  // should this be _P?
//   //   snprintf(result2, sizeof(result2), result, pCONT_iLight->animation.transition.time_ms.val, pCONT_iLight->animation.transition.rate_ms, mixer.group[id].pixel_multiplier_id); 
//   // AddLog(LOG_LEVEL_INFO, PSTR("Mixer \"%s\""), result2);//, pCONT_iLight->animation.transition.time_ms.val, pCONT_iLight->animation.transition.rate_ms);          
//   // #endif // USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION


  
//   // AddLog(LOG_LEVEL_TEST, PSTR("Settings%d rate_ms=%d"),id,pCONT_iLight->animation.transition.rate_ms);
//   // AddLog(LOG_LEVEL_TEST, PSTR("Settings time_ms=%d"),pCONT_iLight->animation.transition.time_ms.val);

// }




// // #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST

// uint8_t mAnimatorLight::ConstructJSON_Mixer(uint8_t json_level)
// {

//   JsonBuilderI->Start();

//     // JsonBuilderI->Object_Start(D_JSON_MIXER);
//     JsonBuilderI->Add(D_JSON_ENABLED, mixer.flags.Enabled);
//     JsonBuilderI->Add("decounter", mixer.group[mixer.running_id].time_on_secs_decounter);



    

//     // JsonBuilderI->Object_Start(D_JSON_MODE);
//     //   JsonBuilderI->Add(D_JSON_RUNNING_ID, mixer.running_id);
//     //   JsonBuilderI->Array_AddArray(D_JSON_TIME_ON_SECS, mixer.mode.time_on_secs, EFFECTS_FUNCTION_MIXER_MAX);
//     //   JsonBuilderI->Add(D_JSON_TIME_ON_SECS "_Active", mixer.mode.time_on_secs_active);
//     //   JsonBuilderI->Add("run_time_duration_scaler_as_percentage", mixer.run_time_duration_scaler_as_percentage);
//     // JsonBuilderI->Object_End();
    
//     // JsonBuilderI->Object_Start(D_JSON_GROUP);
//     //   JsonBuilderI->Add(D_JSON_TRANSITION "_" D_JSON_TIME, mixer.running_id);

        
//     //   JsonBuilderI->Array_Start(D_JSON_COLOUR_PALETTE);
//     //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].palette.id); }
//     //   JsonBuilderI->Array_End();  
//     //   JsonBuilderI->Array_Start("ifenabled_forced_brightness_level_percentage");
//     //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].ifenabled_forced_brightness_level_percentage); }
//     //   JsonBuilderI->Array_End(); 
//     //   JsonBuilderI->Array_Start("pixels_to_update_as_percentage");
//     //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].pixels_to_update_as_percentage); }
//     //   JsonBuilderI->Array_End();
//     //   JsonBuilderI->Array_Start("transition_time");
//     //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].transition.time); }
//     //   JsonBuilderI->Array_End();
//     //   JsonBuilderI->Array_Start("transition_rate");
//     //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].transition.rate); }
//     //   JsonBuilderI->Array_End();



//     // JsonBuilderI->Object_End();

//     JsonBuilderI->Add("EnabledMixerCount", mixer.enabled_mixer_count);

//     // JsonBuilderI->Object_End();

//     // JsonBuilderI->Add("update_as_percentage", pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);






// //   root["enabled"] = mixer.flags.Enabled;
// //   root["running_id"] = mixer.running_id;
// //   root["seconds"] = mixer.group[mixer.running_id].time_on_secs_decounter;//round((mixer.tSavedMillisToChangeAt-millis())/1000);
  
// //   root["time_scaler"] = mixer.time_scaler;

// //   JsonObject timesobj = root.createNestedObject("times");

// //     // char time_on[40]; memset(time_on,0,sizeof(time_on));
// //     // sprintf(time_on, "%02d:%02d:%02d",mixer.mode.times.flashing_starttime.hour,
// //     //                                   mixer.mode.times.flashing_starttime.minute,
// //     //                                   mixer.mode.times.flashing_starttime.second);
// //     // timesobj["flashing_starttime"] = time_on;

// //     // char time_off[40]; memset(time_off,0,sizeof(time_off));
// //     // sprintf(time_off, "%02d:%02d:%02d",mixer.mode.times.flashing_endtime.hour,
// //     //                                   mixer.mode.times.flashing_endtime.minute,
// //     //                                   mixer.mode.times.flashing_endtime.second);
// //     // timesobj["flashing_endtime"] = time_off;

// //     //timesobj["skip_restricted_by_time_isactive"] = mixer.skip_restricted_by_time_isactive;

// //     timesobj["running_friendly_name_ctr"] = mixer.running_friendly_name_ctr;

//   return JsonBuilderI->End();

// }

// // #endif // #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST

// #endif //ENABLE_PIXEL_AUTOMATION_PLAYLIST


// #endif //USE_MODULE_LIGHTS_ANIMATOR




// #include "mAnimatorLight.h"

// /**
//  * @brief Temporary file
//  * 
//  * Showing different animations on a 2D matrix for large scale testing
//  * 
//  **/

// #ifdef USE_MODULE_LIGHTS_ANIMATOR

// #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER

// /**
//  * @brief Development tester of segments, where json commands to configure them all is not large enough
//  **/
// void mAnimatorLight::Test_Config()
// {
//   uint8_t seg_idx = 0;

//   uint8_t brightness = 10;

//   uint16_t row_start_iter = 0;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 500;
//   SEGMENT_I(seg_idx).transition.rate_ms = 1000;
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID, 0);
//   CommandSet_ActiveSolidPalette_Hue_360(0, 0);
//   CommandSet_ActiveSolidPalette_Sat_255(255, 0);
//   CommandSet_Global_BrtRGB_255(brightness, 0);
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_02__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 500;
//   SEGMENT_I(seg_idx).transition.rate_ms = 1000;
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_02__ID, 1);
//   CommandSet_ActiveSolidPalette_Hue_360(120, 1);
//   CommandSet_ActiveSolidPalette_Sat_255(255, 1);
//   CommandSet_Global_BrtRGB_255(brightness, 1);
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }

//   row_start_iter+=8;
//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_03__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 500;
//   SEGMENT_I(seg_idx).transition.rate_ms = 1000;
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_03__ID, 2);
//   CommandSet_ActiveSolidPalette_Hue_360(240, 2);
//   CommandSet_ActiveSolidPalette_Sat_255(255, 2);
//   CommandSet_Global_BrtRGB_255(brightness, 2);
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_04__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 500;
//   SEGMENT_I(seg_idx).transition.rate_ms = 1000;
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_04__ID, 0);
//   CommandSet_ActiveSolidPalette_Hue_360(345, 3);
//   CommandSet_ActiveSolidPalette_Sat_255(255, 3);
//   CommandSet_Global_BrtRGB_255(brightness, 3);
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   row_start_iter+=8;
//   row_start_iter+=8;


//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 0;
//   SEGMENT_I(seg_idx).transition.rate_ms = 2000;
//   SEGMENT_I(seg_idx).flags.brightness_applied_during_colour_generation = true;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_05__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__SLOW_GLOW__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 2000;
//   SEGMENT_I(seg_idx).transition.rate_ms = 3000; 
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   // // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // // SEGMENT_I(seg_idx).grouping = 1;
//   // // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
//   // // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__ROTATING_PALETTE__ID;
//   // // SEGMENT_I(seg_idx).intensity_value = 127;
//   // // SEGMENT_I(seg_idx).transition.time_ms = 500;
//   // // SEGMENT_I(seg_idx).transition.rate_ms = 1000; 
//   // // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // // row_start_iter+=8;

//   // // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // // SEGMENT_I(seg_idx).grouping = 1;
//   // // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
//   // // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__STEPPING_PALETTE__ID;
//   // // SEGMENT_I(seg_idx).intensity_value = 127;
//   // // SEGMENT_I(seg_idx).transition.time_ms = 500;
//   // // SEGMENT_I(seg_idx).transition.rate_ms = 1000; 
//   // // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // // row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_STATIC__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = PURPLE;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   row_start_iter+=8;
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID;
//   SEGMENT_I(seg_idx).intensity_value = 1;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 1);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_PERCENT__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 1);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID;
//   SEGMENT_I(seg_idx).intensity_value = 127;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 1);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(200);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   row_start_iter+=8;
//   row_start_iter+=8;


//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;       row_start_iter+=8;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS__ID;
//   SEGMENT_I(seg_idx).intensity_value = 255;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(255);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;       row_start_iter+=8;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID;
//   SEGMENT_I(seg_idx).intensity_value = 100;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(255);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;       row_start_iter+=8;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID;
//   SEGMENT_I(seg_idx).intensity_value = 100;
//   SEGMENT_I(seg_idx).transition.time_ms = 25;
//   SEGMENT_I(seg_idx).transition.rate_ms = 25;
//   SEGMENT_I(seg_idx).set_speed(255);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = crgb_to_col(CRGB(100,100,100)); // fades to black
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;       row_start_iter+=8;
//   SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   SEGMENT_I(seg_idx).grouping = 1;
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID;
//   SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_RAIN__ID;
//   SEGMENT_I(seg_idx).intensity_value = 100;
//   SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   SEGMENT_I(seg_idx).set_speed(255);
//   SEGMENT_I(seg_idx).colors[0] = RED;
//   SEGMENT_I(seg_idx).colors[1] = GREEN;
//   SEGMENT_I(seg_idx).colors[2] = BLUE;
//   seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;

//   row_start_iter+=8;
//   row_start_iter+=8;


//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 255;
//   // SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).set_speed(255);
//   // SEGMENT_I(seg_idx).colors[0] = RED;
//   // SEGMENT_I(seg_idx).colors[1] = GREEN;
//   // SEGMENT_I(seg_idx).colors[2] = BLUE;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // row_start_iter+=8;

//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_CANDLE_MULTI__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 255;
//   // SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).set_speed(255);
//   // SEGMENT_I(seg_idx).colors[0] = RED;
//   // SEGMENT_I(seg_idx).colors[1] = GREEN;
//   // SEGMENT_I(seg_idx).colors[2] = BLUE;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // row_start_iter+=8;

//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_FIRE_FLICKER__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 255;
//   // SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).set_speed(255);
//   // SEGMENT_I(seg_idx).colors[0] = RED;
//   // SEGMENT_I(seg_idx).colors[1] = GREEN;
//   // SEGMENT_I(seg_idx).colors[2] = BLUE;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // row_start_iter+=8;
  
//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 255;
//   // SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).set_speed(255);
//   // SEGMENT_I(seg_idx).colors[0] = RED;
//   // SEGMENT_I(seg_idx).colors[1] = GREEN;
//   // SEGMENT_I(seg_idx).colors[2] = BLUE;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // row_start_iter+=8;


//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__FIREPLACE_1D_01__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 255;
//   // SEGMENT_I(seg_idx).transition.time_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).transition.rate_ms = FRAMETIME_MS;
//   // SEGMENT_I(seg_idx).set_speed(255);
//   // SEGMENT_I(seg_idx).colors[0] = RED;
//   // SEGMENT_I(seg_idx).colors[1] = GREEN;
//   // SEGMENT_I(seg_idx).colors[2] = BLUE;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   // row_start_iter+=8;




// //last one so I know the others are working
//   // SEGMENT_I(seg_idx).pixel_range.start = row_start_iter;
//   // SEGMENT_I(seg_idx).pixel_range.stop  = row_start_iter+8;
//   // SEGMENT_I(seg_idx).grouping = 1;
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_MIRROR, 0);
//   // SEGMENT_I(seg_idx).setOption(SEG_OPTION_REVERSED, 0);
//   // SEGMENT_I(seg_idx).palette.id = mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
//   // SEGMENT_I(seg_idx).effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//   // SEGMENT_I(seg_idx).intensity_value = 127;
//   // SEGMENT_I(seg_idx).transition.time_ms = 500;
//   // SEGMENT_I(seg_idx).transition.rate_ms = 1000;
//   // seg_idx++; if(seg_idx>MAX_NUM_SEGMENTS){ ALOG_ERR(PSTR("seg_idx{%d}>MAX_NUM_SEGMENTS{%d}"),seg_idx,MAX_NUM_SEGMENTS); return; }
//   row_start_iter+=8;


//   pCONT_iLight->CommandSet_Global_BrtRGB_255(brightness);


// }

// #endif // USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER


// #endif //USE_MODULE_LIGHTS_ANIMATOR




