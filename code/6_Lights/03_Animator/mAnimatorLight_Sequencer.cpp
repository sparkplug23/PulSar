/**
 * @file mAnimatorLight_Mixer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-13
 * 
 * Mixer is the original playlist, and will contain hardcoded functions that are called to sequence desired effects on devices.
 * This is main while playlist via json is under development, though may be useful or future methods.
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER 


void mAnimatorLight::Init_Sequencer()
{

  Load_Sequencer(0); // Load default sequences

} // Init_Sequencer


void mAnimatorLight::Load_Sequencer(uint8_t id)
{

  sequencer_item_list.clear(); // clear all sequences
  memset(&sequencer_runtime, 0, sizeof(sequencer_runtime)); // clear all sequences

  struct SEQUENCER_ITEM seq_t;
  
  #ifdef ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE
  if((id == 0)||(id == 1))
  {
    id = 1; // if default, ensure its back to 1 to other functions work

    // 1 - Static 
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 3 - Slow glows
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":30,
            "Speed":200,
            "Grouping":1,
            "Decimate":30
          },
          "Transition": {
            "TimeMs": 500,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":10
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 2 - shimmer
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Shimmer",
            "Intensity":255,
            "Speed":200,
            "Grouping":1,
            "Decimate":30
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 5 - White twinkles
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White 02",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":3,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":93
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 7
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 8
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":200,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 10000,
            "RateMs": 20000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 50,
            "RateMs": 100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    
    // 18
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Randomise 01",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":255,
            "Speed":126,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 5000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

// here good
    // 11
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Sweep Random",
            "Intensity":255,
            "Speed":127,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 12
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Fireworks 1D",
            "Intensity":255,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 13
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Fireworks 1D No Launch",
            "Intensity":255,
            "Speed":126,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 6 - starburst rainbow over white
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Rainbow 16",
          "Effects": {
            "Function":"Fireworks Starburst",
            "Intensity":255,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour1":{
            "Hue":20,
            "Sat":0,
            "BrightnessRGB":30
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    /**
     * @brief As debug method, force all seconds_on split across a total time period
     * 
     */
    // for (auto &seq_t:sequencer_item_list)
    // {
    //   seq_t.seconds_on = 120/sequencer_item_list.size();
    // }

    ALOG_INF(PSTR("Loaded Sequencer 1"));

  }else
  // Demo mode
  if(id == 2)
  {
    
    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":100,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);

    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":180,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);

    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);


    // Last
    seq_t.seconds_on = 1;
    seq_t.json_command = 
      R"=====(
        {
          "Sequencer":{"LoadSet":1}
        }
      )=====";
    sequencer_item_list.push_back(seq_t);


    ALOG_INF(PSTR("Loaded Sequencer 2"));
  }

  sequencer_runtime.seconds_remaining_on_item = 0; // Load now

  mqtthandler_automation_sequencer.flags.SendNow = true;

  #endif // ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE



  sequencer_runtime.loaded_sequence_set = id;

}

void mAnimatorLight::SetSequenceTimes(uint16_t secs)
{  
  for (auto &seq_t:sequencer_item_list)
  {
    seq_t.seconds_on = secs;
  }  
}


void mAnimatorLight::handleSequencer()
{

  uint8_t flag_reset = false;

  if(sequencer_runtime.loaded_sequence_set == 0)
  {
    // Not active, do nothing
    return;
  }

  
  if( mTime::TimeReachedNonReset(&sequencer_runtime.tSaved_Tick, 1000) ) // NonReset, so if sequence is not found, then it will retry the next index
  {

    if(sequencer_item_list.size() > 0)
    {

      if(sequencer_runtime.seconds_remaining_on_item >= 1) // still active
      {
        sequencer_runtime.seconds_remaining_on_item--;
        sequencer_runtime.tSaved_Tick = millis();
        ALOG_INF(PSTR("sequencer_runtime.seconds_remaining_on_item = %d"), sequencer_runtime.seconds_remaining_on_item);
      }else 
      if(sequencer_runtime.seconds_remaining_on_item == 0) // still active
      {      
        
        struct SEQUENCER_ITEM sequencer_item_t = sequencer_item_list[sequencer_runtime.active_sequence_index];

        ALOG_INF(PSTR("sequencer_runtime.seconds_remaining_on_item FINISHED, Trigger and move on"));
        ALOG_INF(PSTR("Json = \"%s\""), sequencer_item_t.json_command);
        pCONT->JSONCommand_Run((char*)sequencer_item_t.json_command.c_str());

        sequencer_runtime.seconds_remaining_on_item = sequencer_item_t.seconds_on; // Set timeon for THIS effect

        sequencer_runtime.active_sequence_index++; // move to next sequence
        sequencer_runtime.tSaved_Tick = millis();

        /**
         * @brief Load new
         **/
        if(sequencer_runtime.active_sequence_index >= sequencer_item_list.size())
        {
          sequencer_runtime.active_sequence_index = 0;
        }

        mqtthandler_automation_sequencer.flags.SendNow = true;
      }


      // ALOG_INF(PSTR("Json = \"%s\""), sequencer_item_t.json_command);
      sequencer_runtime.tSaved_Tick = millis();
      ALOG_INF(PSTR("Json test>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %d"), sequencer_runtime.seconds_remaining_on_item);


    }


  }


} // handleSequencer


uint8_t mAnimatorLight::ConstructJSON_Sequencer(uint8_t json_level, bool json_appending)
{

  char buffer[100] = {0};

  JsonBuilderI->Start();

    JBI->Add("Index", sequencer_runtime.active_sequence_index);

    JBI->Object_Start_F(PSTR("SequenceList"));
    for(auto seq_t:sequencer_item_list)
    {

      JBI->Add("seconds_on", seq_t.seconds_on);

      JBI->Array_Start();

        JBI->Object_Start_F(PSTR("EnabledTimes"));
          JBI->Add("Start", mTime::ConvertShortTimetoCtr(&seq_t.time_enabled.start, buffer, sizeof(buffer)) );
          JBI->Add("End",   mTime::ConvertShortTimetoCtr(&seq_t.time_enabled.end, buffer, sizeof(buffer)) );
        JBI->Object_End();


        // JBI->Add("Json", seq_t.json_command);


      JBI->Array_End();
      
    }
    JBI->Object_End();
 

  return JsonBuilderI->End();

} // ConstructJSON_Sequencer


#endif // ENABLE_FEATURE_LIGHTING__SEQUENCER

#endif //USE_MODULE_LIGHTS_ANIMATOR
