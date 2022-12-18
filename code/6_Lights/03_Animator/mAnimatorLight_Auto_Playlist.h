// #ifndef _AnimatorDALIGHTNEOPIXEL7_segH
// #define _AnimatorDALIGHTNEOPIXEL7_segH 7.0

// #include "1_TaskerManager/mTaskerManager.h"


//   #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST

//     void LoadMixerGroupByID(uint8_t id);
//   enum EFFECTS_FUNCTION_MIXER{
//     EFFECTS_FUNCTION_MIXER_01_ID=0,
//     // EFFECTS_FUNCTION_MIXER_02_ID,
//     // EFFECTS_FUNCTION_MIXER_03_ID,
//     // EFFECTS_FUNCTION_MIXER_04_ID,
//     // EFFECTS_FUNCTION_MIXER_05_ID,
//     // EFFECTS_FUNCTION_MIXER_06_ID,
//     // EFFECTS_FUNCTION_MIXER_07_ID,
//     // EFFECTS_FUNCTION_MIXER_08_ID,
//     // EFFECTS_FUNCTION_MIXER_09_ID,
//     // EFFECTS_FUNCTION_MIXER_10_ID,
//     // EFFECTS_FUNCTION_MIXER_11_ID,
//     // EFFECTS_FUNCTION_MIXER_12_ID,
//     // EFFECTS_FUNCTION_MIXER_13_ID,
//     // EFFECTS_FUNCTION_MIXER_14_ID, 
//     // EFFECTS_FUNCTION_MIXER_15_ID, 
//     EFFECTS_FUNCTION_MIXER_LENGTH_ID
//   };
//   #define EFFECTS_FUNCTION_MIXER_MAX EFFECTS_FUNCTION_MIXER_LENGTH_ID
//   // #define MIXER_GROUP_MAX 7

  
//   typedef union {
//     uint16_t data; // allows full manipulating
//     struct { 
//       // enable animations (pause)
//       uint16_t Enabled : 1;
//       /**
//        * 0 - None
//        * 1 - Basic multiplier
//        * 2 - Using mapped index array 
//       */
//       // uint16_t multiplier_mode_id : 4; // 2 bit : 4 levels
//       // uint16_t mapped_array_editable_or_progmem : 1;

      


//     };
//   } MIXER_SETTINGS_FLAGS;

//    typedef union {
//     uint16_t data; // allows full manipulating
//     struct { 
//       // enable animations (pause)
//       uint16_t Enabled : 1;
//       /**
//        * 0 - None
//        * 1 - Basic multiplier
//        * 2 - Using mapped index array 
//       */
//       // uint16_t multiplier_mode_id : 4; // 2 bit : 4 levels
//       // uint16_t mapped_array_editable_or_progmem : 1;
      
//       uint16_t enable_force_preset_brightness_scaler : 1; // to allow manual brightness leveling


// /**
//  * Any mixer setting that needed this, should probably just becomes its own animation, and hence just a different animation mode will be chosen
//  * "Mixer" should be distilled down to ONLY picking animation, setting brightness, intensities etc
//  * 
//  * */
//       uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Exactly_To_Palette_Choice : 1;
//       uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice : 1;



//     };
//   } MIXER_SETTINGS_GROUP_FLAGS;
  

//   struct MIXER_SETTINGS{
//    // #ifdef DEVICE_OUTSIDETREE
//     struct MODE{
//       uint8_t running_id = EFFECTS_FUNCTION_MIXER_01_ID;
//       uint16_t time_on_secs[EFFECTS_FUNCTION_MIXER_MAX]; // stores which to use
//       int16_t time_on_secs_active = 60; //signed
//       // char running_friendly_name_ctr[40];
//       struct TIMES{
//        // uint16_t enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_MAX]; // if set, this mode will only run if INSIDE the "flashy" time period
//         struct time_short flashing_starttime; //shortime
//         struct time_short flashing_endtime;
//         uint8_t skip_restricted_by_time_isactive = 0;
//       }times;
//     }mode; //active 
//     // #endif
    
//     // uint32_t tSavedMillisToChangeAt = 0;
//     uint32_t tSavedTrigger = millis();
//     // uint32_t tSavedRestart = millis();
//     // uint8_t enabled = false;
//     // uint8_t time_scaler = 6;
//     // uint32_t tSavedSendData;
//     uint8_t enabled_mixer_count = 0; // 10 max

//     // Shared loaded values to act on
//     uint8_t brightness_lower_255 = 0;
//     uint8_t brightness_higher_255 = 255;

//     MIXER_SETTINGS_FLAGS flags;


//     //int16_t time_on_secs_active = 10; //signed
//     // char running_friendly_name_ctr[100];
//     uint8_t running_id = EFFECTS_FUNCTION_MIXER_01_ID;
//     uint8_t run_time_duration_scaler_as_percentage = 100; // for debugging, running faster
      
//     struct GROUPS_SETTINGS{
//       int16_t time_on_secs_decounter = -1;
//       struct time_short starttime;
//       struct time_short endtime;
//       uint8_t enable_skip_restricted_by_time = 0;
//       uint8_t isenabled = true;

//       uint8_t ifenabled_forced_brightness_level_percentage = 100;
//       int8_t pixels_to_update_as_percentage = 10;//-1; // -1 disables, ie ALL

//       struct TRANSITION{
//         uint16_t time = 0;
//         uint16_t rate = 0;
//         uint8_t  time_unit_id = 0; // secs vs ms
//       }transition;

//       uint8_t brightness_lower_255 = 0;
//       uint8_t brightness_higher_255 = 255;

//       uint8_t animation_transition_order = 0;//TRANSITION_ORDER_INORDER_ID;
//       uint8_t flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
//       uint16_t run_time_duration_sec = 120;
//       uint8_t palette_id = 0;

//       uint8_t pixel_multiplier_id = 0;//PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
//       uint8_t pixel_multiplier_enabled = false;

//       MIXER_SETTINGS_GROUP_FLAGS flags;

//     }group[EFFECTS_FUNCTION_MIXER_MAX];

//   }mixer;

//   void init_mixer_defaults();
//   #endif //ENABLE_PIXEL_AUTOMATION_PLAYLIST

// #endif

// // #endif
