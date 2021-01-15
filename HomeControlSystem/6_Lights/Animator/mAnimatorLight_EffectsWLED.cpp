// #include "mRGBAnimator.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR


// #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS




// /*
//  * Color wipe function
//  * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
//  * if (bool rev == true) then LEDs are turned off in reverse order
//  */
// uint16_t WS2812FX::color_wipe(bool rev, bool useRandomColors) {
//   uint32_t cycleTime = 750 + (255 - _segments[_segment_index].speed)*150;
//   uint32_t perc = now % cycleTime;
//   uint16_t prog = (perc * 65535) / cycleTime;
//   bool back = (prog > 32767);
//   if (back) {
//     prog -= 32767;
//     if (_segment_runtimes[_segment_index].step == 0) _segment_runtimes[_segment_index].step = 1;
//   } else {
//     if (_segment_runtimes[_segment_index].step == 2) _segment_runtimes[_segment_index].step = 3; //trigger color change
//   }

//   if (useRandomColors) {
//     if (_segment_runtimes[_segment_index].call == 0) {
//       _segment_runtimes[_segment_index].aux0 = random8();
//       _segment_runtimes[_segment_index].step = 3;
//     }
//     if (_segment_runtimes[_segment_index].step == 1) { //if flag set, change to new random color
//       _segment_runtimes[_segment_index].aux1 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
//       _segment_runtimes[_segment_index].step = 2;
//     }
//     if (_segment_runtimes[_segment_index].step == 3) {
//       _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux1);
//       _segment_runtimes[_segment_index].step = 0;
//     }
//   }

//   uint16_t ledIndex = (prog * _virtualSegmentLength) >> 15;
//   uint16_t rem = 0;
//   rem = (prog * _virtualSegmentLength) * 2; //mod 0xFFFF
//   rem /= (_segments[_segment_index].intensity +1);
//   if (rem > 255) rem = 255;

//   uint32_t col1 = useRandomColors? color_wheel(_segment_runtimes[_segment_index].aux1) : SEGCOLOR(1);
//   for (uint16_t i = 0; i < _virtualSegmentLength; i++)
//   {
//     uint16_t index = (rev && back)? _virtualSegmentLength -1 -i : i;
//     uint32_t col0 = useRandomColors? color_wheel(_segment_runtimes[_segment_index].aux0) : color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    
//     if (i < ledIndex) 
//     {
//       setPixelColor(index, back? col1 : col0);
//     } else
//     {
//       setPixelColor(index, back? col0 : col1);
//       if (i == ledIndex) setPixelColor(index, color_blend(back? col0 : col1, back? col1 : col0, rem));
//     }
//   } 
//   return FRAMETIME;
// }


// /*
//  * Lights all LEDs one after another.
//  */
// uint16_t WS2812FX::mode_color_wipe(void) {
//   return color_wipe(false, false);
// }

// /*
//  * Lights all LEDs one after another. Turns off opposite
//  */
// uint16_t WS2812FX::mode_color_sweep(void) {
//   return color_wipe(true, false);
// }


// /*
//  * Turns all LEDs after each other to a random color.
//  * Then starts over with another color.
//  */
// uint16_t WS2812FX::mode_color_wipe_random(void) {
//   return color_wipe(false, true);
// }


// /*
//  * Random color introduced alternating from start and end of strip.
//  */
// uint16_t WS2812FX::mode_color_sweep_random(void) {
//   return color_wipe(true, true);
// }








// #endif //USE_MODULE_LIGHTS_ANIMATOR




// #endif// ENABLE_PIXEL_FUNCTION_EFFECTS
