
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION 



#define IBN 5100
#define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)



//each needs 12 bytes
//Spark type is used for popcorn, 1D fireworks, and drip
typedef struct Spark {
  float pos;
  float vel;
  uint16_t col;
  uint8_t colIndex;
} spark;


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Static
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/*
 * No blinking. Just plain old static light.
 */
uint16_t WS2812FX::mode_static(void) {
  fill(SEGCOLOR(0));
  return (_segments[_segment_index].getOption(SEG_OPTION_TRANSITIONAL)) ? FRAMETIME : 500; //update faster if in transition
}

//Speed slider sets amount of LEDs lit, intensity sets unlit
uint16_t WS2812FX::mode_static_pattern()
{
  uint16_t lit = 1 + _segments[_segment_index].speed;
  uint16_t unlit = 1 + _segments[_segment_index].intensity;
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, (drawingLit) ? color_from_palette(i, true, PALETTE_SOLID_WRAP, 0) : SEGCOLOR(1));
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  return FRAMETIME;
}

uint16_t WS2812FX::mode_tri_static_pattern()
{
  uint8_t segSize = (_segments[_segment_index].intensity >> 5) +1;
  uint8_t currSeg = 0;
  uint16_t currSegCount = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    if ( currSeg % 3 == 0 ) {
      setPixelColor(i, SEGCOLOR(0));
    } else if( currSeg % 3 == 1) {
      setPixelColor(i, SEGCOLOR(1));
    } else {
      setPixelColor(i, (SEGCOLOR(2) > 0 ? SEGCOLOR(2) : WHITE));
    }
    currSegCount += 1;
    if (currSegCount >= segSize) {
      currSeg +=1;
      currSegCount = 0;
    }
  }

  return FRAMETIME;
}

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Blink
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 * NOTE: Maybe re-rework without timer
 */
uint16_t WS2812FX::blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette) {
  uint16_t stateTime = _segment_runtimes[_segment_index].aux1;
  uint32_t cycleTime = (255 - _segments[_segment_index].speed)*20;
  uint32_t onTime = 0;
  uint32_t offTime = cycleTime;

  if (!strobe) {
    onTime = (cycleTime * _segments[_segment_index].intensity) >> 8;
    offTime = cycleTime - onTime;
  }
  
  stateTime = ((_segment_runtimes[_segment_index].aux0 & 1) == 0) ? onTime : offTime;
  stateTime += 20;
    
  if (now - _segment_runtimes[_segment_index].step > stateTime)
  {
    _segment_runtimes[_segment_index].aux0++;
    _segment_runtimes[_segment_index].aux1 = stateTime;
    _segment_runtimes[_segment_index].step = now;
  }

  uint32_t color = ((_segment_runtimes[_segment_index].aux0 & 1) == 0) ? color1 : color2;
  if (color == color1 && do_palette)
  {
    for(uint16_t i = 0; i < SEGLEN; i++) {
      setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else fill(color);

  return FRAMETIME;
}


/*
 * Normal blinking. 50% on/off time.
 */
uint16_t WS2812FX::mode_blink(void) {
  return blink(SEGCOLOR(0), SEGCOLOR(1), false, true);
}


/*
 * Classic Blink effect. Cycling through the rainbow.
 */
uint16_t WS2812FX::mode_blink_rainbow(void) {
  return blink(color_wheel(_segment_runtimes[_segment_index].call & 0xFF), SEGCOLOR(1), false, false);
}


/*
 * Classic Strobe effect.
 */
uint16_t WS2812FX::mode_strobe(void) {
  return blink(SEGCOLOR(0), SEGCOLOR(1), true, true);
}


/*
 * Classic Strobe effect. Cycling through the rainbow.
 */
uint16_t WS2812FX::mode_strobe_rainbow(void) {
  return blink(color_wheel(_segment_runtimes[_segment_index].call & 0xFF), SEGCOLOR(1), true, false);
}

/*
 * Strobe effect with different strobe count and pause, controlled by speed.
 */
uint16_t WS2812FX::mode_multi_strobe(void) {
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }
  //blink(SEGCOLOR(0), SEGCOLOR(1), true, true);

  uint16_t delay = 50 + 20*(uint16_t)(255-_segments[_segment_index].speed);
  uint16_t count = 2 * ((_segments[_segment_index].speed / 10) + 1);
  if(_segment_runtimes[_segment_index].step < count) {
    if((_segment_runtimes[_segment_index].step & 1) == 0) {
      for(uint16_t i = 0; i < SEGLEN; i++) {
        setPixelColor(i, SEGCOLOR(0));
      }
      delay = 20;
    } else {
      delay = 50;
    }
  }
  _segment_runtimes[_segment_index].step = (_segment_runtimes[_segment_index].step + 1) % (count + 1);
  return delay;
}


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * color_wipe
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

/*
 * Color wipe function
 * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
 * if (bool rev == true) then LEDs are turned off in reverse order
 */
uint16_t WS2812FX::color_wipe(bool rev, bool useRandomColors) {
  uint32_t cycleTime = 750 + (255 - _segments[_segment_index].speed)*150;
  uint32_t perc = now % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  bool back = (prog > 32767);
  if (back) {
    prog -= 32767;
    if (_segment_runtimes[_segment_index].step == 0) _segment_runtimes[_segment_index].step = 1;
  } else {
    if (_segment_runtimes[_segment_index].step == 2) _segment_runtimes[_segment_index].step = 3; //trigger color change
  }

  if (useRandomColors) {
    if (_segment_runtimes[_segment_index].call == 0) {
      _segment_runtimes[_segment_index].aux0 = random8();
      _segment_runtimes[_segment_index].step = 3;
    }
    if (_segment_runtimes[_segment_index].step == 1) { //if flag set, change to new random color
      _segment_runtimes[_segment_index].aux1 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
      _segment_runtimes[_segment_index].step = 2;
    }
    if (_segment_runtimes[_segment_index].step == 3) {
      _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux1);
      _segment_runtimes[_segment_index].step = 0;
    }
  }

  uint16_t ledIndex = (prog * SEGLEN) >> 15;
  uint16_t rem = 0;
  rem = (prog * SEGLEN) * 2; //mod 0xFFFF
  rem /= (_segments[_segment_index].intensity +1);
  if (rem > 255) rem = 255;

  uint32_t col1 = useRandomColors ? color_wheel(_segment_runtimes[_segment_index].aux1) : SEGCOLOR(1);
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint16_t index = (rev && back)? SEGLEN -1 -i : i;
    uint32_t col0 = useRandomColors? color_wheel(_segment_runtimes[_segment_index].aux0) : color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    
    if (i < ledIndex) 
    {
      setPixelColor(index, back? col1 : col0);
    } else
    {
      setPixelColor(index, back? col0 : col1);
      if (i == ledIndex) setPixelColor(index, color_blend(back? col0 : col1, back? col1 : col0, rem));
    }
  } 
  return FRAMETIME;
}


/*
 * Lights all LEDs one after another.
 */
uint16_t WS2812FX::mode_color_wipe(void) {
  return color_wipe(false, false);
}

/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
uint16_t WS2812FX::mode_color_wipe_random(void) {
  return color_wipe(false, true);
}

/*
 * Lights all LEDs one after another. Turns off opposite
 */
uint16_t WS2812FX::mode_color_sweep(void) {
  return color_wipe(true, false);
}


/*
 * Random color introduced alternating from start and end of strip->
 */
uint16_t WS2812FX::mode_color_sweep_random(void) {
  return color_wipe(true, true);
}


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Scan
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/*
 * Scan mode parent function
 */
uint16_t WS2812FX::scan(bool dual)
{
  uint32_t cycleTime = 750 + (255 - _segments[_segment_index].speed)*150;
  uint32_t perc = now % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t size = 1 + ((_segments[_segment_index].intensity * SEGLEN) >> 9);
  uint16_t ledIndex = (prog * ((SEGLEN *2) - size *2)) >> 16;

  fill(SEGCOLOR(1));

  int led_offset = ledIndex - (SEGLEN - size);
  led_offset = abs(led_offset);

  if (dual) {
    for (uint16_t j = led_offset; j < led_offset + size; j++) {
      uint16_t i2 = SEGLEN -1 -j;
      setPixelColor(i2, color_from_palette(i2, true, PALETTE_SOLID_WRAP, (SEGCOLOR(2))? 2:0));
    }
  }

  for (uint16_t j = led_offset; j < led_offset + size; j++) {
    setPixelColor(j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;
}


/*
 * Runs a single pixel back and forth.
 */
uint16_t WS2812FX::mode_scan(void) {
  return scan(false);
}


/*
 * Runs two pixel back and forth in opposite directions.
 */
uint16_t WS2812FX::mode_dual_scan(void) {
  return scan(true);
}


/*
 * K.I.T.T.
 */
uint16_t WS2812FX::mode_larson_scanner(void){
  return larson_scanner(false);
}

uint16_t WS2812FX::larson_scanner(bool dual) {
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) +8);
  uint16_t index = counter * SEGLEN  >> 16;

  fade_out(_segments[_segment_index].intensity);

  if (_segment_runtimes[_segment_index].step > index && _segment_runtimes[_segment_index].step - index > SEGLEN/2) {
    _segment_runtimes[_segment_index].aux0 = !_segment_runtimes[_segment_index].aux0;
  }
  
  for (uint16_t i = _segment_runtimes[_segment_index].step; i < index; i++) {
    uint16_t j = (_segment_runtimes[_segment_index].aux0)?i:SEGLEN-1-i;
    setPixelColor( j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }
  if (dual) {
    uint32_t c;
    if (SEGCOLOR(2) != 0) {
      c = SEGCOLOR(2);
    } else {
      c = color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    }

    for (uint16_t i = _segment_runtimes[_segment_index].step; i < index; i++) {
      uint16_t j = (_segment_runtimes[_segment_index].aux0)?SEGLEN-1-i:i;
      setPixelColor(j, c);
    }
  }

  _segment_runtimes[_segment_index].step = index;
  return FRAMETIME;
}

/*
 * Creates two Larson scanners moving in opposite directions
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/DualLarson.h
 */
uint16_t WS2812FX::mode_dual_larson_scanner(void){
  return larson_scanner(true);
}




/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Chase
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/*
 * color chase function.
 * color1 = background color
 * color2 and color3 = colors of two adjacent leds
 */
uint16_t WS2812FX::chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette) {
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) + 1);
  uint16_t a = counter * SEGLEN  >> 16;

  bool chase_random = (_segments[_segment_index].mode == FX_MODE_CHASE_RANDOM);
  if (chase_random) {
    if (a < _segment_runtimes[_segment_index].step) //we hit the start again, choose new color for Chase random
    {
      _segment_runtimes[_segment_index].aux1 = _segment_runtimes[_segment_index].aux0; //store previous random color
      _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
    }
    color1 = color_wheel(_segment_runtimes[_segment_index].aux0);
  }
  _segment_runtimes[_segment_index].step = a;

  // Use intensity setting to vary chase up to 1/2 string length
  uint8_t size = 1 + (_segments[_segment_index].intensity * SEGLEN >> 10);

  uint16_t b = a + size; //"trail" of chase, filled with color1 
  if (b > SEGLEN) b -= SEGLEN;
  uint16_t c = b + size;
  if (c > SEGLEN) c -= SEGLEN;

  //background
  if (do_palette)
  {
    for(uint16_t i = 0; i < SEGLEN; i++) {
      setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
    }
  } else fill(color1);

  //if random, fill old background between a and end
  if (chase_random)
  {
    color1 = color_wheel(_segment_runtimes[_segment_index].aux1);
    for (uint16_t i = a; i < SEGLEN; i++)
      setPixelColor(i, color1);
  }

  //fill between points a and b with color2
  if (a < b)
  {
    for (uint16_t i = a; i < b; i++)
      setPixelColor(i, color2);
  } else {
    for (uint16_t i = a; i < SEGLEN; i++) //fill until end
      setPixelColor(i, color2);
    for (uint16_t i = 0; i < b; i++) //fill from start until b
      setPixelColor(i, color2);
  }

  //fill between points b and c with color2
  if (b < c)
  {
    for (uint16_t i = b; i < c; i++)
      setPixelColor(i, color3);
  } else {
    for (uint16_t i = b; i < SEGLEN; i++) //fill until end
      setPixelColor(i, color3);
    for (uint16_t i = 0; i < c; i++) //fill from start until c
      setPixelColor(i, color3);
  }

  return FRAMETIME;
}


/*
 * Bicolor chase, more primary color.
 */
uint16_t WS2812FX::mode_chase_color(void) {
  return chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), true);
}


/*
 * Primary running followed by random color.
 */
uint16_t WS2812FX::mode_chase_random(void) {
  return chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), false);
}


/*
 * Primary, secondary running on rainbow.
 */
uint16_t WS2812FX::mode_chase_rainbow(void) {
  uint8_t color_sep = 256 / SEGLEN;
  uint8_t color_index = _segment_runtimes[_segment_index].call & 0xFF;
  uint32_t color = color_wheel(((_segment_runtimes[_segment_index].step * color_sep) + color_index) & 0xFF);

  return chase(color, SEGCOLOR(0), SEGCOLOR(1), false);
}


/*
 * Sec flashes running on prim.
 */
#define FLASH_COUNT 4
uint16_t WS2812FX::mode_chase_flash(void) {
  uint8_t flash_step = _segment_runtimes[_segment_index].call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  uint16_t delay = 10 + ((30 * (uint16_t)(255 - _segments[_segment_index].speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    if(flash_step % 2 == 0) {
      uint16_t n = _segment_runtimes[_segment_index].step;
      uint16_t m = (_segment_runtimes[_segment_index].step + 1) % SEGLEN;
      setPixelColor( n, SEGCOLOR(1));
      setPixelColor( m, SEGCOLOR(1));
      delay = 20;
    } else {
      delay = 30;
    }
  } else {
    _segment_runtimes[_segment_index].step = (_segment_runtimes[_segment_index].step + 1) % SEGLEN;
  }
  return delay;
}



/*
 * Prim flashes running, followed by random color.
 */
uint16_t WS2812FX::mode_chase_flash_random(void) {
  uint8_t flash_step = _segment_runtimes[_segment_index].call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < _segment_runtimes[_segment_index].step; i++) {
    setPixelColor(i, color_wheel(_segment_runtimes[_segment_index].aux0));
  }

  uint16_t delay = 1 + ((10 * (uint16_t)(255 - _segments[_segment_index].speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    uint16_t n = _segment_runtimes[_segment_index].step;
    uint16_t m = (_segment_runtimes[_segment_index].step + 1) % SEGLEN;
    if(flash_step % 2 == 0) {
      setPixelColor( n, SEGCOLOR(0));
      setPixelColor( m, SEGCOLOR(0));
      delay = 20;
    } else {
      setPixelColor( n, color_wheel(_segment_runtimes[_segment_index].aux0));
      setPixelColor( m, SEGCOLOR(1));
      delay = 30;
    }
  } else {
    _segment_runtimes[_segment_index].step = (_segment_runtimes[_segment_index].step + 1) % SEGLEN;

    if(_segment_runtimes[_segment_index].step == 0) {
      _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
    }
  }
  return delay;
}


/*
 * Primary running on rainbow.
 */
uint16_t WS2812FX::mode_chase_rainbow_white(void) {
  uint16_t n = _segment_runtimes[_segment_index].step;
  uint16_t m = (_segment_runtimes[_segment_index].step + 1) % SEGLEN;
  uint32_t color2 = color_wheel(((n * 256 / SEGLEN) + (_segment_runtimes[_segment_index].call & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / SEGLEN) + (_segment_runtimes[_segment_index].call & 0xFF)) & 0xFF);

  return chase(SEGCOLOR(0), color2, color3, false);
}


/*
 * theater chase function
 */
uint16_t WS2812FX::theater_chase(uint32_t color1, uint32_t color2, bool do_palette) {
  byte gap = 2 + ((255 - _segments[_segment_index].intensity) >> 5);
  uint32_t cycleTime = 50 + (255 - _segments[_segment_index].speed)*2;
  uint32_t it = now / cycleTime;
  if (it != _segment_runtimes[_segment_index].step) //new color
  {
    _segment_runtimes[_segment_index].aux0 = (_segment_runtimes[_segment_index].aux0 +1) % gap;
    _segment_runtimes[_segment_index].step = it;
  }
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i % gap) == _segment_runtimes[_segment_index].aux0) {
      if (do_palette)
      {
        setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
      } else {
        setPixelColor(i, color1);
      }
    } else {
      setPixelColor(i, color2);
    }
  }
  return FRAMETIME;
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
uint16_t WS2812FX::mode_theater_chase(void) {
  return theater_chase(SEGCOLOR(0), SEGCOLOR(1), true);
}


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
uint16_t WS2812FX::mode_theater_chase_rainbow(void) {
  return theater_chase(color_wheel(_segment_runtimes[_segment_index].step), SEGCOLOR(1), false);
}




/*
 * Tricolor chase function
 */
uint16_t WS2812FX::tricolor_chase(uint32_t color1, uint32_t color2) {
  uint32_t cycleTime = 50 + (255 - _segments[_segment_index].speed)*2;
  uint32_t it = now / cycleTime;
  uint8_t width = (1 + _segments[_segment_index].intensity/32) * 3; //value of 1-8 for each colour
  uint8_t index = it % width;
  
  for(uint16_t i = 0; i < SEGLEN; i++, index++) {
    if(index > width-1) index = 0;

    uint32_t color = color1;
    if(index > width*2/3-1) color = color_from_palette(i, true, PALETTE_SOLID_WRAP, 1);
    else if(index > width/3-1) color = color2;

    setPixelColor(SEGLEN - i -1, color);
  }

  return FRAMETIME;
}


/*
 * Tricolor chase mode
 */
uint16_t WS2812FX::mode_tricolor_chase(void) {
  return tricolor_chase(SEGCOLOR(2), SEGCOLOR(0));
}


/*
 * Running random pixels
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/RandomChase.h
 */
uint16_t WS2812FX::mode_random_chase(void)
{
  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - _segments[_segment_index].speed));
  uint32_t it = now / cycleTime;
  if (_segment_runtimes[_segment_index].step == it) return FRAMETIME;

  for(uint16_t i = SEGLEN -1; i > 0; i--) {
    setPixelColor(i, getPixelColor(i-1));
  }
  uint32_t color = getPixelColor(0);
  if (SEGLEN > 1) color = getPixelColor( 1);
  uint8_t r = random8(6) != 0 ? (color >> 16 & 0xFF) : random8();
  uint8_t g = random8(6) != 0 ? (color >> 8  & 0xFF) : random8();
  uint8_t b = random8(6) != 0 ? (color       & 0xFF) : random8();
  setPixelColor(0, r, g, b);

  _segment_runtimes[_segment_index].step = it;
  return FRAMETIME;
}


/*
 * Alternating white/red/black pixels running. PLACEHOLDER
 */
uint16_t WS2812FX::mode_circus_combustus(void) {
  return tricolor_chase(RED, WHITE);
}




/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Noise
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


uint16_t WS2812FX::mode_fillnoise8()
{
  if (_segment_runtimes[_segment_index].call == 0) _segment_runtimes[_segment_index].step = random16(12345);
  CRGB fastled_col;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i * SEGLEN, _segment_runtimes[_segment_index].step + i * SEGLEN);
    fastled_col = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[_segment_index].step += beatsin8(_segments[_segment_index].speed, 1, 6); //10,1,4

  return FRAMETIME;
}

uint16_t WS2812FX::mode_noise16_1()
{
  uint16_t scale = 320;                                      // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[_segment_index].step += (1 + _segments[_segment_index].speed/16);

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = beatsin8(11);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = _segment_runtimes[_segment_index].step/42;             // the y position becomes slowly incremented


    uint16_t real_x = (i + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (i + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = _segment_runtimes[_segment_index].step;                          // the z position becomes quickly incremented

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                         // map LED color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  return FRAMETIME;
}


uint16_t WS2812FX::mode_noise16_2()
{
  uint16_t scale = 1000;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[_segment_index].step += (1 + (_segments[_segment_index].speed >> 1));

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = _segment_runtimes[_segment_index].step >> 6;                         // x as a function of time
    uint16_t shift_y = _segment_runtimes[_segment_index].step/42;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field

    uint8_t noise = inoise16(real_x, 0, 4223) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  return FRAMETIME;
}


uint16_t WS2812FX::mode_noise16_3()
{
  uint16_t scale = 800;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[_segment_index].step += (1 + _segments[_segment_index].speed);

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = _segment_runtimes[_segment_index].step*8;  

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  return FRAMETIME;
}


//https://github.com/aykevl/ledstrip-spark/blob/master/ledstrip->ino
uint16_t WS2812FX::mode_noise16_4()
{
  CRGB fastled_col;
  uint32_t stp = (now * _segments[_segment_index].speed) >> 7;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    int16_t index = inoise16(uint32_t(i) << 12, stp);
    fastled_col = ColorFromPalette(currentPalette, index);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  return FRAMETIME;
}

// Peaceful noise that's slow and with gradually changing palettes. Does not support WLED palettes or default colours or controls.
uint16_t WS2812FX::mode_noisepal(void) {                                    // Slow noise palette by Andrew Tuline.
  uint16_t scale = 15 + (_segments[_segment_index].intensity >> 2); //default was 30
  //#define scale 30

  uint16_t dataSize = sizeof(CRGBPalette16) * 2; //allocate space for 2 Palettes
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed

  CRGBPalette16* palettes = reinterpret_cast<CRGBPalette16*>(_segment_runtimes[_segment_index].data);

  uint16_t changePaletteMs = 4000 + _segments[_segment_index].speed *10; //between 4 - 6.5sec
  if (millis() - _segment_runtimes[_segment_index].step > changePaletteMs)
  {
    _segment_runtimes[_segment_index].step = millis();

    uint8_t baseI = random8();
    palettes[1] = CRGBPalette16(CHSV(baseI+random8(64), 255, random8(128,255)), CHSV(baseI+128, 255, random8(128,255)), CHSV(baseI+random8(92), 192, random8(128,255)), CHSV(baseI+random8(92), 255, random8(128,255)));
  }

  CRGB color;

  //EVERY_N_MILLIS(10) { //(don't have to time this, effect function is only called every 24ms)
  nblendPaletteTowardPalette(palettes[0], palettes[1], 48);               // Blend towards the target palette over 48 iterations.

  if (_segments[_segment_index].palette > 0) palettes[0] = currentPalette;

  for(int i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i*scale, _segment_runtimes[_segment_index].aux0+i*scale);                // Get a value from the noise function. I'm using both x and y axis.
    color = ColorFromPalette(palettes[0], index, 255, LINEARBLEND);       // Use the my own palette.
    setPixelColor(i, color.red, color.green, color.blue);
  }

  _segment_runtimes[_segment_index].aux0 += beatsin8(10,1,4);                                        // Moving along the distance. Vary it a bit with a sine wave.

  return FRAMETIME;
}

/*
 * Effects by Andrew Tuline
 */
uint16_t WS2812FX::phased_base(uint8_t moder) {                  // We're making sine waves here. By Andrew Tuline.

  uint8_t allfreq = 16;                                          // Base frequency.
  //float* phasePtr = reinterpret_cast<float*>(_segment_runtimes[_segment_index].step);       // Phase change value gets calculated.
  static float phase = 0;//phasePtr[0];
  uint8_t cutOff = (255-_segments[_segment_index].intensity);                      // You can change the number of pixels.  AKA INTENSITY (was 192).
  uint8_t modVal = 5;//_segments[_segment_index].fft1/8+1;                         // You can change the modulus. AKA FFT1 (was 5).

  uint8_t index = now/64;                                    // Set color rotation speed
  phase += _segments[_segment_index].speed/32.0;                                   // You can change the speed of the wave. AKA SPEED (was .4)
  //phasePtr[0] = phase; 

  for (int i = 0; i < SEGLEN; i++) {
    if (moder == 1) modVal = (inoise8(i*10 + i*10) /16);         // Let's randomize our mod length with some Perlin noise.
    uint16_t val = (i+1) * allfreq;                              // This sets the frequency of the waves. The +1 makes sure that leds[0] is used.
    if (modVal == 0) modVal = 1;
    val += phase * (i % modVal +1) /2;                           // This sets the varying phase change of the waves. By Andrew Tuline.
    uint8_t b = cubicwave8(val);                                 // Now we make an 8 bit sinewave.
    b = (b > cutOff) ? (b - cutOff) : 0;                         // A ternary operator to cutoff the light.
    setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(index, false, false, 0), b));
    index += 256 / SEGLEN;
  }

  return FRAMETIME;
}


uint16_t WS2812FX::mode_phased_noise(void) {
  return phased_base(1);
}


uint16_t WS2812FX::mode_phased(void) {
  return phased_base(0);
}







/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Breathe/Fade/Pulse
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/*
 * Does the "standby-breathing" of well known i-Devices.
 */
uint16_t WS2812FX::mode_breath(void) {
  uint16_t var = 0;
  uint16_t counter = (now * ((_segments[_segment_index].speed >> 3) +10));
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
  }

  return FRAMETIME;
}


/*
 * Fades the LEDs between two colors
 */
uint16_t WS2812FX::mode_fade(void) {
  uint16_t counter = (now * ((_segments[_segment_index].speed >> 3) +10));
  uint8_t lum = triwave16(counter) >> 8;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
  }

  return FRAMETIME;
}



/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Fireworks
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



/*
 * Fireworks function.
 */
uint16_t WS2812FX::mode_fireworks() {
  fade_out(0);
  if (_segment_runtimes[_segment_index].call == 0) {
    _segment_runtimes[_segment_index].aux0 = UINT16_MAX;
    _segment_runtimes[_segment_index].aux1 = UINT16_MAX;
  }
  bool valid1 = (_segment_runtimes[_segment_index].aux0 < SEGLEN);
  bool valid2 = (_segment_runtimes[_segment_index].aux1 < SEGLEN);
  uint32_t sv1 = 0, sv2 = 0;
  if (valid1) sv1 = getPixelColor(_segment_runtimes[_segment_index].aux0);
  if (valid2) sv2 = getPixelColor(_segment_runtimes[_segment_index].aux1);
  blur(255-_segments[_segment_index].speed);
  if (valid1) setPixelColor(_segment_runtimes[_segment_index].aux0 , sv1);
  if (valid2) setPixelColor(_segment_runtimes[_segment_index].aux1, sv2);

  for(uint16_t i=0; i<MAX(1, SEGLEN/20); i++) {
    if(random8(129 - (_segments[_segment_index].intensity >> 1)) == 0) {
      uint16_t index = random(SEGLEN);
      setPixelColor(index, color_from_palette(random8(), false, false, 0));
      _segment_runtimes[_segment_index].aux1 = _segment_runtimes[_segment_index].aux0;
      _segment_runtimes[_segment_index].aux0 = index;
    }
  }
  return FRAMETIME;
}



/*
/ Fireworks in starburst effect
/ based on the video: https://www.reddit.com/r/arduino/comments/c3sd46/i_made_this_fireworks_effect_for_my_led_strips/
/ Speed sets frequency of new starbursts, intensity is the intensity of the burst
*/
#define STARBURST_MAX_FRAG 12

//each needs 64 byte
typedef struct particle {
  CRGB     color;
  uint32_t birth  =0;
  uint32_t last   =0;
  float    vel    =0;
  uint16_t pos    =-1;
  float    fragment[STARBURST_MAX_FRAG];
} star;

uint16_t WS2812FX::mode_starburst(void) {
  uint8_t numStars = 1 + (SEGLEN >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(_segment_runtimes[_segment_index].data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed to adjust chance of a burst, max is nearly always.
    if (random8((144-(_segments[_segment_index].speed >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(SEGLEN-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (_segments[_segment_index].intensity >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR(1));
  
  for (int j=0; j<numStars; j++)
  {
    if (stars[j].birth != 0) {
      float dt = (it-stars[j].last)/1000.0;

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        int var = i >> 1;
        
        if (stars[j].fragment[i] > 0) {
          //all fragments travel right, will be mirrored on other side
          stars[j].fragment[i] += stars[j].vel * dt * (float)var/3.0;
        }
      }
      stars[j].last = it;
      stars[j].vel -= 3*stars[j].vel*dt;
    }
  
    CRGB c = stars[j].color;

    // If the star is brand new, it flashes white briefly.  
    // Otherwise it just fades over time.
    float fade = 0.0f;
    float age = it-stars[j].birth;

    if (age < particleIgnition) {
      c = col_to_crgb(color_blend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(color_blend(crgb_to_col(c), SEGCOLOR(1), f));
      }
    }
    
    float particleSize = (1.0 - fade) * 2;

    for (uint8_t index=0; index < STARBURST_MAX_FRAG*2; index++) {
      bool mirrored = index & 0x1;
      uint8_t i = index >> 1;
      if (stars[j].fragment[i] > 0) {
        float loc = stars[j].fragment[i];
        if (mirrored) loc -= (loc-stars[j].pos)*2;
        int start = loc - particleSize;
        int end = loc + particleSize;
        if (start < 0) start = 0;
        if (start == end) end++;
        if (end > SEGLEN) end = SEGLEN;    
        for (int p = start; p < end; p++) {
          setPixelColor(p, c.r, c.g, c.b);
        }
      }
    }
  }
  return FRAMETIME;
}



/*
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 */

uint16_t WS2812FX::mode_exploding_fireworks(void)
{
  //allocate segment data
  uint16_t numSparks = 2 + (SEGLEN >> 1); 
  if (numSparks > 80) numSparks = 80;
  uint16_t dataSize = sizeof(spark) * numSparks;
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed

  fill(BLACK);
  
  bool actuallyReverse = _segments[_segment_index].getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity
  _segments[_segment_index].setOption(SEG_OPTION_REVERSED, _segment_runtimes[_segment_index].step);
  
  Spark* sparks = reinterpret_cast<Spark*>(_segment_runtimes[_segment_index].data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (_segments[_segment_index].speed/800000.0); // m/s/s
  gravity *= SEGLEN;
  
  if (_segment_runtimes[_segment_index].aux0 < 2) { //FLARE
    if (_segment_runtimes[_segment_index].aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness

      _segment_runtimes[_segment_index].aux0 = 1; 
    }
    
    // launch 
    if (flare->vel > 12 * gravity) {
      // flare
      setPixelColor(int(flare->pos),flare->col,flare->col,flare->col);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, SEGLEN-1);
      flare->vel += gravity;
      flare->col -= 2;
    } else {
      _segment_runtimes[_segment_index].aux0 = 2;  // ready to explode
    }
  } else if (_segment_runtimes[_segment_index].aux0 < 4) {
    /*
     * Explode!
     * 
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    int nSparks = flare->pos;
    nSparks = constrain(nSparks, 0, numSparks);
    static float dying_gravity;
  
    // initialize sparks
    if (_segment_runtimes[_segment_index].aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/SEGLEN; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      _segment_runtimes[_segment_index].aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < SEGLEN) {
          uint16_t prog = sparks[i].col;
          uint32_t spColor = (_segments[_segment_index].palette) ? color_wheel(sparks[i].colIndex) : SEGCOLOR(0);
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(color_blend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(color_blend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          setPixelColor(int(sparks[i].pos), c.red, c.green, c.blue);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      _segment_runtimes[_segment_index].aux0 = 6 + random8(10); //wait for this many frames
    }
  } else {
    _segment_runtimes[_segment_index].aux0--;
    if (_segment_runtimes[_segment_index].aux0 < 4) {
      _segment_runtimes[_segment_index].aux0 = 0; //back to flare
      _segment_runtimes[_segment_index].step = (_segments[_segment_index].intensity > random8()); //decide firing side
    }
  }

  _segments[_segment_index].setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  return FRAMETIME;  
}


//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
uint16_t WS2812FX::mode_rain()
{
  _segment_runtimes[_segment_index].step += FRAMETIME;
  if (_segment_runtimes[_segment_index].step > SPEED_FORMULA_L) {
    _segment_runtimes[_segment_index].step = 0;
    //shift all leds right
    uint32_t ctemp = getPixelColor(SEGLEN -1);
    for(uint16_t i = SEGLEN -1; i > 0; i--) {
      setPixelColor(i, getPixelColor(i-1));
    }
    setPixelColor(0, ctemp);
    _segment_runtimes[_segment_index].aux0++;
    _segment_runtimes[_segment_index].aux1++;
    if (_segment_runtimes[_segment_index].aux0 == 0) _segment_runtimes[_segment_index].aux0 = UINT16_MAX;
    if (_segment_runtimes[_segment_index].aux1 == 0) _segment_runtimes[_segment_index].aux0 = UINT16_MAX;
    if (_segment_runtimes[_segment_index].aux0 == SEGLEN) _segment_runtimes[_segment_index].aux0 = 0;
    if (_segment_runtimes[_segment_index].aux1 == SEGLEN) _segment_runtimes[_segment_index].aux1 = 0;
  }
  return mode_fireworks();
}


/*
 * Fire flicker function
 */
uint16_t WS2812FX::mode_fire_flicker(void) {
  uint32_t cycleTime = 40 + (255 - _segments[_segment_index].speed);
  uint32_t it = now / cycleTime;
  if (_segment_runtimes[_segment_index].step == it) return FRAMETIME;
  
  byte w = (SEGCOLOR(0) >> 24) & 0xFF;
  byte r = (SEGCOLOR(0) >> 16) & 0xFF;
  byte g = (SEGCOLOR(0) >>  8) & 0xFF;
  byte b = (SEGCOLOR(0)        & 0xFF);
  byte lum = (_segments[_segment_index].palette == 0) ? MAX(w, MAX(r, MAX(g, b))) : 255;
  lum /= (((256-_segments[_segment_index].intensity)/16)+1);
  for(uint16_t i = 0; i < SEGLEN; i++) {
    byte flicker = random8(lum);
    if (_segments[_segment_index].palette == 0) {
      setPixelColor(i, MAX(r - flicker, 0), MAX(g - flicker, 0), MAX(b - flicker, 0), MAX(w - flicker, 0));
    } else {
      setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, 255 - flicker));
    }
  }

  _segment_runtimes[_segment_index].step = it;
  return FRAMETIME;
}

#ifdef ENABLE_ADVANCED_EFFECTS

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Sparkle/Twinkle
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/*
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 */
uint16_t WS2812FX::mode_sparkle(void) {
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }
  uint32_t cycleTime = 10 + (255 - _segments[_segment_index].speed)*2;
  uint32_t it = now / cycleTime;
  if (it != _segment_runtimes[_segment_index].step)
  {
    _segment_runtimes[_segment_index].aux0 = random16(SEGLEN); // aux0 stores the random led index
    _segment_runtimes[_segment_index].step = it;
  }
  
  setPixelColor(_segment_runtimes[_segment_index].aux0, SEGCOLOR(0));
  return FRAMETIME;
}


/*
 * Lights all LEDs in the color. Flashes single white pixels randomly.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 */
uint16_t WS2812FX::mode_flash_sparkle(void) {
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if(random8(5) == 0) {
    _segment_runtimes[_segment_index].aux0 = random16(SEGLEN); // aux0 stores the random led index
    setPixelColor(_segment_runtimes[_segment_index].aux0, SEGCOLOR(1));
    return 20;
  } 
  return 20 + (uint16_t)(255-_segments[_segment_index].speed);
}


/*
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 */
uint16_t WS2812FX::mode_hyper_sparkle(void) {
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if(random8(5) < 2) {
    for(uint16_t i = 0; i < MAX(1, SEGLEN/3); i++) {
      setPixelColor(random16(SEGLEN), SEGCOLOR(1));
    }
    return 20;
  }
  return 20 + (uint16_t)(255-_segments[_segment_index].speed);
}






/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * TO SORT UNDER
 ****************************************************************************************************************************
 ****************************************************************************************************************************/






uint16_t WS2812FX::mode_twinkleup(void) {                 // A very short twinkle routine with fade-in and dual controls. By Andrew Tuline.
  random16_set_seed(535);                                 // The randomizer needs to be re-set each time through the loop in order for the same 'random' numbers to be the same each time through.

  for (int i = 0; i<SEGLEN; i++) {
    uint8_t ranstart = random8();                         // The starting value (aka brightness) for each pixel. Must be consistent each time through the loop for this to work.
    uint8_t pixBri = sin8(ranstart + 16 * now/(256-_segments[_segment_index].speed));
    if (random8() > _segments[_segment_index].intensity) pixBri = 0;
    setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i*20, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  return FRAMETIME;
}





/*
 * Lights all LEDs in one random color up. Then switches them
 * to the next random color.
 */
uint16_t WS2812FX::mode_random_color(void) {
  uint32_t cycleTime = 200 + (255 - _segments[_segment_index].speed)*50;
  uint32_t it = now / cycleTime;
  uint32_t rem = now % cycleTime;
  uint16_t fadedur = (cycleTime * _segments[_segment_index].intensity) >> 8;

  uint32_t fade = 255;
  if (fadedur) {
    fade = (rem * 255) / fadedur;
    if (fade > 255) fade = 255;
  }

  if (_segment_runtimes[_segment_index].call == 0) {
    _segment_runtimes[_segment_index].aux0 = random8();
    _segment_runtimes[_segment_index].step = 2;
  }
  if (it != _segment_runtimes[_segment_index].step) //new color
  {
    _segment_runtimes[_segment_index].aux1 = _segment_runtimes[_segment_index].aux0;
    _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0); //aux0 will store our random color wheel index
    _segment_runtimes[_segment_index].step = it;
  }

  fill(color_blend(color_wheel(_segment_runtimes[_segment_index].aux1), color_wheel(_segment_runtimes[_segment_index].aux0), fade));
  return FRAMETIME;
}


/*
 * Lights every LED in a random color. Changes all LED at the same time
// * to new random colors.
 */
uint16_t WS2812FX::mode_dynamic(void) {
  if (!_segment_runtimes[_segment_index].allocateData(SEGLEN)) return mode_static(); //allocation failed
  
  if(_segment_runtimes[_segment_index].call == 0) {
    for (uint16_t i = 0; i < SEGLEN; i++) _segment_runtimes[_segment_index].data[i] = random8();
  }

  uint32_t cycleTime = 50 + (255 - _segments[_segment_index].speed)*15;
  uint32_t it = now / cycleTime;
  if (it != _segment_runtimes[_segment_index].step && _segments[_segment_index].speed != 0) //new color
  {
    for (uint16_t i = 0; i < SEGLEN; i++) {
      if (random8() <= _segments[_segment_index].intensity) _segment_runtimes[_segment_index].data[i] = random8();
    }
    _segment_runtimes[_segment_index].step = it;
  }
  
  for (uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_wheel(_segment_runtimes[_segment_index].data[i]));
  }
  return FRAMETIME;
}



/*
 * Cycles all LEDs at once through a rainbow.
 */
uint16_t WS2812FX::mode_rainbow(void) {
  uint16_t counter = (now * ((_segments[_segment_index].speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  if (_segments[_segment_index].intensity < 128){
    fill(color_blend(color_wheel(counter),WHITE,128-_segments[_segment_index].intensity));
  } else {
    fill(color_wheel(counter));
  }

  return FRAMETIME;
}


/*
 * Cycles a rainbow over the entire string of LEDs.
 */
uint16_t WS2812FX::mode_rainbow_cycle(void) {
  uint16_t counter = (now * ((_segments[_segment_index].speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    //intensity/29 = 0 (1/16) 1 (1/8) 2 (1/4) 3 (1/2) 4 (1) 5 (2) 6 (4) 7 (8) 8 (16)
    uint8_t index = (i * (16 << (_segments[_segment_index].intensity /29)) / SEGLEN) + counter;
    setPixelColor(i, color_wheel(index));
  }

  return FRAMETIME;
}


/*
 * Running lights effect with smooth sine transition base.
 */
uint16_t WS2812FX::running_base(bool saw) {
  uint8_t x_scale = _segments[_segment_index].intensity >> 2;
  uint32_t counter = (now * _segments[_segment_index].speed) >> 9;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    uint8_t s = 0;
    uint8_t a = i*x_scale - counter;
    if (saw) {
      if (a < 16)
      {
        a = 192 + a*8;
      } else {
        a = map(a,16,255,64,192);
      }
    }
    s = sin8(a);
    setPixelColor(i, color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), s));
  }
  return FRAMETIME;
}


/*
 * Running lights effect with smooth sine transition.
 */
uint16_t WS2812FX::mode_running_lights(void) {
  return running_base(false);
}


/*
 * Running lights effect with sawtooth transition.
 */
uint16_t WS2812FX::mode_saw(void) {
  return running_base(true);
}


/*
 * Blink several LEDs in random colors on, reset, repeat.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 */
uint16_t WS2812FX::mode_twinkle(void) {
  fill(SEGCOLOR(1));

  uint32_t cycleTime = 20 + (255 - _segments[_segment_index].speed)*5;
  uint32_t it = now / cycleTime;
  if (it != _segment_runtimes[_segment_index].step)
  {
    uint16_t maxOn = map(_segments[_segment_index].intensity, 0, 255, 1, SEGLEN); // make sure at least one LED is on
    if (_segment_runtimes[_segment_index].aux0 >= maxOn)
    {
      _segment_runtimes[_segment_index].aux0 = 0;
      _segment_runtimes[_segment_index].aux1 = random16(); //new seed for our PRNG
    }
    _segment_runtimes[_segment_index].aux0++;
    _segment_runtimes[_segment_index].step = it;
  }
  
  uint16_t PRNG16 = _segment_runtimes[_segment_index].aux1;

  for (uint16_t i = 0; i < _segment_runtimes[_segment_index].aux0; i++)
  {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next 'random' number
    uint32_t p = (uint32_t)SEGLEN * (uint32_t)PRNG16;
    uint16_t j = p >> 16;
    setPixelColor(j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;
}


/*
 * Dissolve function
 */
uint16_t WS2812FX::dissolve(uint32_t color) {
  bool wa = (SEGCOLOR(1) != 0 && _brightness < 255); //workaround, can't compare getPixel to color if not full brightness
  
  for (uint16_t j = 0; j <= SEGLEN / 15; j++)
  {
    if (random8() <= _segments[_segment_index].intensity) {
      for (uint8_t times = 0; times < 10; times++) //attempt to spawn a new pixel 5 times
      {
        uint16_t i = random16(SEGLEN);
        if (_segment_runtimes[_segment_index].aux0) { //dissolve to primary/palette
          if (getPixelColor(i) == SEGCOLOR(1) || wa) {
            if (color == SEGCOLOR(0))
            {
              setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
            } else { setPixelColor(i, color); }     
            break; //only spawn 1 new pixel per frame per 50 LEDs
          }
        } else { //dissolve to secondary
          if (getPixelColor(i) != SEGCOLOR(1)) { setPixelColor(i, SEGCOLOR(1)); break; }
        }
      }
    }
  }

  if (_segment_runtimes[_segment_index].call > (255 - _segments[_segment_index].speed) + 15) 
  {
    _segment_runtimes[_segment_index].aux0 = !_segment_runtimes[_segment_index].aux0;
    _segment_runtimes[_segment_index].call = 0;
  }
  
  return FRAMETIME;
}


/*
 * Blink several LEDs on and then off
 */
uint16_t WS2812FX::mode_dissolve(void) {
  return dissolve(SEGCOLOR(0));
}


/*
 * Blink several LEDs on and then off in random colors
 */
uint16_t WS2812FX::mode_dissolve_random(void) {
  return dissolve(color_wheel(random8()));
}




/*
 * Android loading circle
 */
uint16_t WS2812FX::mode_android(void) {
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }

  if (_segment_runtimes[_segment_index].aux1 > ((float)_segments[_segment_index].intensity/255.0)*(float)SEGLEN)
  {
    _segment_runtimes[_segment_index].aux0 = 1;
  } else
  {
    if (_segment_runtimes[_segment_index].aux1 < 2) _segment_runtimes[_segment_index].aux0 = 0;
  }

  uint16_t a = _segment_runtimes[_segment_index].step;
  
  if (_segment_runtimes[_segment_index].aux0 == 0)
  {
    if (_segment_runtimes[_segment_index].call %3 == 1) {a++;}
    else {_segment_runtimes[_segment_index].aux1++;}
  } else
  {
    a++;
    if (_segment_runtimes[_segment_index].call %3 != 1) _segment_runtimes[_segment_index].aux1--;
  }
  
  if (a >= SEGLEN) a = 0;

  if (a + _segment_runtimes[_segment_index].aux1 < SEGLEN)
  {
    for(int i = a; i < a+_segment_runtimes[_segment_index].aux1; i++) {
      setPixelColor(i, SEGCOLOR(0));
    }
  } else
  {
    for(int i = a; i < SEGLEN; i++) {
      setPixelColor(i, SEGCOLOR(0));
    }
    for(int i = 0; i < _segment_runtimes[_segment_index].aux1 - (SEGLEN -a); i++) {
      setPixelColor(i, SEGCOLOR(0));
    }
  }
  _segment_runtimes[_segment_index].step = a;

  return 3 + ((8 * (uint32_t)(255 - _segments[_segment_index].speed)) / SEGLEN);
}


/*
 * Red - Amber - Green - Blue lights running
 */
uint16_t WS2812FX::mode_colorful(void) {
  uint32_t cols[]{0x00FF0000,0x00EEBB00,0x0000EE00,0x000077CC,0x00FF0000,0x00EEBB00,0x0000EE00};
  if (_segments[_segment_index].intensity < 127) //pastel (easter) colors
  {
    cols[0] = 0x00FF8040;
    cols[1] = 0x00E5D241;
    cols[2] = 0x0077FF77;
    cols[3] = 0x0077F0F0;
    for (uint8_t i = 4; i < 7; i++) cols[i] = cols[i-4];
  }
  
  uint32_t cycleTime = 50 + (15 * (uint32_t)(255 - _segments[_segment_index].speed));
  uint32_t it = now / cycleTime;
  if (it != _segment_runtimes[_segment_index].step)
  {
    if (_segments[_segment_index].speed > 0) _segment_runtimes[_segment_index].aux0++;
    if (_segment_runtimes[_segment_index].aux0 > 3) _segment_runtimes[_segment_index].aux0 = 0;
    _segment_runtimes[_segment_index].step = it;
  }
  
  uint16_t i = 0;
  for (i; i < SEGLEN -3; i+=4)
  {
    setPixelColor(i, cols[_segment_runtimes[_segment_index].aux0]);
    setPixelColor(i+1, cols[_segment_runtimes[_segment_index].aux0+1]);
    setPixelColor(i+2, cols[_segment_runtimes[_segment_index].aux0+2]);
    setPixelColor(i+3, cols[_segment_runtimes[_segment_index].aux0+3]);
  }
  if(i < SEGLEN)
  {
    setPixelColor(i, cols[_segment_runtimes[_segment_index].aux0]);
    
    if(i+1 < SEGLEN)
    {
      setPixelColor(i+1, cols[_segment_runtimes[_segment_index].aux0+1]);
      
      if(i+2 < SEGLEN)
      {
        setPixelColor(i+2, cols[_segment_runtimes[_segment_index].aux0+2]);
      }
    }
  }
  
  return FRAMETIME;
}


/*
 * Emulates a traffic light.
 */
uint16_t WS2812FX::mode_traffic_light(void) {
  for(uint16_t i=0; i < SEGLEN; i++)
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  uint32_t mdelay = 500;
  for (int i = 0; i < SEGLEN-2 ; i+=3)
  {
    switch (_segment_runtimes[_segment_index].aux0)
    {
      case 0: setPixelColor(i, 0x00FF0000); mdelay = 150 + (100 * (uint32_t)(255 - _segments[_segment_index].speed));break;
      case 1: setPixelColor(i, 0x00FF0000); mdelay = 150 + (20 * (uint32_t)(255 - _segments[_segment_index].speed)); setPixelColor(i+1, 0x00EECC00); break;
      case 2: setPixelColor(i+2, 0x0000FF00); mdelay = 150 + (100 * (uint32_t)(255 - _segments[_segment_index].speed));break;
      case 3: setPixelColor(i+1, 0x00EECC00); mdelay = 150 + (20 * (uint32_t)(255 - _segments[_segment_index].speed));break;
    }
  }

  if (now - _segment_runtimes[_segment_index].step > mdelay)
  {
    _segment_runtimes[_segment_index].aux0++;
    if (_segment_runtimes[_segment_index].aux0 == 1 && _segments[_segment_index].intensity > 140) _segment_runtimes[_segment_index].aux0 = 2; //skip Red + Amber, to get US-style sequence
    if (_segment_runtimes[_segment_index].aux0 > 3) _segment_runtimes[_segment_index].aux0 = 0;
    _segment_runtimes[_segment_index].step = now;
  }
  
  return FRAMETIME;
}



/*
 * Alternating pixels running function.
 */
uint16_t WS2812FX::running(uint32_t color1, uint32_t color2) {
  uint8_t pxw = 1 + (_segments[_segment_index].intensity >> 5);
  uint32_t cycleTime = 35 + (255 - _segments[_segment_index].speed);
  uint32_t it = now / cycleTime;
  if (_segments[_segment_index].speed == 0) it = 0;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i + _segment_runtimes[_segment_index].aux0) % (pxw*2) < pxw) {
      if (color1 == SEGCOLOR(0))
      {
        setPixelColor(SEGLEN -i -1, color_from_palette(SEGLEN -i -1, true, PALETTE_SOLID_WRAP, 0));
      } else
      {
        setPixelColor(SEGLEN -i -1, color1);
      }
    } else {
      setPixelColor(SEGLEN -i -1, color2);
    }
  }

  if (it != _segment_runtimes[_segment_index].step )
  {
    _segment_runtimes[_segment_index].aux0 = (_segment_runtimes[_segment_index].aux0 +1) % (pxw*2);
    _segment_runtimes[_segment_index].step = it;
  }
  return FRAMETIME;
}

/*
 * Alternating color/sec pixels running.
 */
uint16_t WS2812FX::mode_running_color(void) {
  return running(SEGCOLOR(0), SEGCOLOR(1));
}


/*
 * Alternating red/blue pixels running.
 */
uint16_t WS2812FX::mode_running_red_blue(void) {
  return running(RED, BLUE);
}


/*
 * Alternating red/green pixels running.
 */
uint16_t WS2812FX::mode_merry_christmas(void) {
  return running(RED, GREEN);
}


/*
 * Alternating orange/purple pixels running.
 */
uint16_t WS2812FX::mode_halloween(void) {
  return running(PURPLE, ORANGE);
}


/*
 * Random colored pixels running.
 */
uint16_t WS2812FX::mode_running_random(void) {
  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - _segments[_segment_index].speed));
  uint32_t it = now / cycleTime;
  if (_segment_runtimes[_segment_index].aux1 == it) return FRAMETIME;

  for(uint16_t i=SEGLEN-1; i > 0; i--) {
    setPixelColor( i, getPixelColor( i - 1));
  }

  if(_segment_runtimes[_segment_index].step == 0) {
    _segment_runtimes[_segment_index].aux0 = get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
    setPixelColor(0, color_wheel(_segment_runtimes[_segment_index].aux0));
  }

  _segment_runtimes[_segment_index].step++;
  if (_segment_runtimes[_segment_index].step > ((255-_segments[_segment_index].intensity) >> 4))
  {
    _segment_runtimes[_segment_index].step = 0;
  }

  _segment_runtimes[_segment_index].aux1 = it;
  return FRAMETIME;
}


/*
 * Firing comets from one end. "Lighthouse"
 */
uint16_t WS2812FX::mode_comet(void) {
  uint16_t counter = now * ((_segments[_segment_index].speed >>2) +1);
  uint16_t index = counter * SEGLEN >> 16;
  if (_segment_runtimes[_segment_index].call == 0) _segment_runtimes[_segment_index].aux0 = index;

  fade_out(_segments[_segment_index].intensity);

  setPixelColor( index, color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
  if (index > _segment_runtimes[_segment_index].aux0) {
    for (uint16_t i = _segment_runtimes[_segment_index].aux0; i < index ; i++) {
       setPixelColor( i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else if (index < _segment_runtimes[_segment_index].aux0 && index < 10) {
    for (uint16_t i = 0; i < index ; i++) {
       setPixelColor( i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }      
  }
  _segment_runtimes[_segment_index].aux0 = index++;

  return FRAMETIME;
}



/*
 * Gradient run base function
 */
uint16_t WS2812FX::gradient_base(bool loading) {
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) + 1);
  uint16_t pp = counter * SEGLEN >> 16;
  if (_segment_runtimes[_segment_index].call == 0) pp = 0;
  float val; //0.0 = sec 1.0 = pri
  float brd = loading ? _segments[_segment_index].intensity : _segments[_segment_index].intensity/2;
  if (brd <1.0) brd = 1.0;
  int p1 = pp-SEGLEN;
  int p2 = pp+SEGLEN;

  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    if (loading)
    {
      val = abs(((i>pp) ? p2:pp) -i);
    } else {
      val = MIN(abs(pp-i),MIN(abs(p1-i),abs(p2-i)));
    }
    val = (brd > val) ? val/brd * 255 : 255;
    setPixelColor(i, color_blend(SEGCOLOR(0), color_from_palette(i, true, PALETTE_SOLID_WRAP, 1), val));
  }

  return FRAMETIME;
}


/*
 * Gradient run
 */
uint16_t WS2812FX::mode_gradient(void) {
  return gradient_base(false);
}


/*
 * Gradient run with hard transition
 */
uint16_t WS2812FX::mode_loading(void) {
  return gradient_base(true);
}


//American Police Light with all LEDs Red and Blue 
uint16_t WS2812FX::police_base(uint32_t color1, uint32_t color2, bool all)
{
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) +1);
  uint16_t idexR = (counter * SEGLEN) >> 16;
  if (idexR >= SEGLEN) idexR = 0;

  uint16_t topindex = SEGLEN >> 1;
  uint16_t idexB = (idexR > topindex) ? idexR - topindex : idexR + topindex;
  if (_segment_runtimes[_segment_index].call == 0) _segment_runtimes[_segment_index].aux0 = idexR;
  if (idexB >= SEGLEN) idexB = 0; //otherwise overflow on odd number of LEDs

  if (all) { //different algo, ensuring immediate fill
    if (idexB > idexR) {
      fill(color2);
      for (uint16_t i = idexR; i < idexB; i++) setPixelColor(i, color1);
    } else {
      fill(color1);
      for (uint16_t i = idexB; i < idexR; i++) setPixelColor(i, color2);
    } 
  } else { //regular dot-only mode
    uint8_t size = 1 + _segments[_segment_index].intensity >> 3;
    if (size > SEGLEN/2) size = 1+ SEGLEN/2;
    for (uint8_t i=0; i <= size; i++) {
      setPixelColor(idexR+i, color1);
      setPixelColor(idexB+i, color2);
    }
    if (_segment_runtimes[_segment_index].aux0 != idexR) {
      uint8_t gap = (_segment_runtimes[_segment_index].aux0 < idexR)? idexR - _segment_runtimes[_segment_index].aux0:SEGLEN - _segment_runtimes[_segment_index].aux0 + idexR;
      for (uint8_t i = 0; i <= gap ; i++) {
        if ((idexR - i) < 0) idexR = SEGLEN-1 + i;
        if ((idexB - i) < 0) idexB = SEGLEN-1 + i;
        setPixelColor(idexR-i, color1);
        setPixelColor(idexB-i, color2);
      }
      _segment_runtimes[_segment_index].aux0 = idexR;
    }
  }
  
  return FRAMETIME;
}


//American Police Light with all LEDs Red and Blue 
uint16_t WS2812FX::mode_police_all()
{
  return police_base(RED, BLUE, true);
}


//Police Lights Red and Blue 
uint16_t WS2812FX::mode_police()
{
  fill(SEGCOLOR(1));

  return police_base(RED, BLUE, false);
}


//Police All with custom colors
uint16_t WS2812FX::mode_two_areas()
{
  return police_base(SEGCOLOR(0), SEGCOLOR(1), true);
}


//Police Lights with custom colors 
uint16_t WS2812FX::mode_two_dots()
{
  fill(SEGCOLOR(2));
  uint32_t color2 = (SEGCOLOR(1) == SEGCOLOR(2)) ? SEGCOLOR(0) : SEGCOLOR(1);

  return police_base(SEGCOLOR(0), color2, false);
}


/*
 * ICU mode
 */
uint16_t WS2812FX::mode_icu(void) {
  uint16_t dest = _segment_runtimes[_segment_index].step & 0xFFFF;
  uint8_t space = (_segments[_segment_index].intensity >> 3) +2;

  fill(SEGCOLOR(1));

  byte pindex = map(dest, 0, SEGLEN-SEGLEN/space, 0, 255);
  uint32_t col = color_from_palette(pindex, false, false, 0);

  setPixelColor(dest, col);
  setPixelColor(dest + SEGLEN/space, col);

  if(_segment_runtimes[_segment_index].aux0 == dest) { // pause between eye movements
    if(random8(6) == 0) { // blink once in a while
      setPixelColor(dest, SEGCOLOR(1));
      setPixelColor(dest + SEGLEN/space, SEGCOLOR(1));
      return 200;
    }
    _segment_runtimes[_segment_index].aux0 = random16(SEGLEN-SEGLEN/space);
    return 1000 + random16(2000);
  }

  if(_segment_runtimes[_segment_index].aux0 > _segment_runtimes[_segment_index].step) {
    _segment_runtimes[_segment_index].step++;
    dest++;
  } else if (_segment_runtimes[_segment_index].aux0 < _segment_runtimes[_segment_index].step) {
    _segment_runtimes[_segment_index].step--;
    dest--;
  }

  setPixelColor(dest, col);
  setPixelColor(dest + SEGLEN/space, col);

  return SPEED_FORMULA_L;
}


/*
 * Custom mode by Aircoookie. Color Wipe, but with 3 colors
 */
uint16_t WS2812FX::mode_tricolor_wipe(void)
{
  uint32_t cycleTime = 1000 + (255 - _segments[_segment_index].speed)*200;
  uint32_t perc = now % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t ledIndex = (prog * SEGLEN * 3) >> 16;
  uint16_t ledOffset = ledIndex;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 2));
  }
  
  if(ledIndex < SEGLEN) { //wipe from 0 to 1
    for (uint16_t i = 0; i < SEGLEN; i++)
    {
      setPixelColor(i, (i > ledOffset)? SEGCOLOR(0) : SEGCOLOR(1));
    }
  } else if (ledIndex < SEGLEN*2) { //wipe from 1 to 2
    ledOffset = ledIndex - SEGLEN;
    for (uint16_t i = ledOffset +1; i < SEGLEN; i++)
    {
      setPixelColor(i, SEGCOLOR(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - SEGLEN*2;
    for (uint16_t i = 0; i <= ledOffset; i++)
    {
      setPixelColor(i, SEGCOLOR(0));
    }
  }

  return FRAMETIME;
}


/*
 * Fades between 3 colors
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/TriFade.h
 * Modified by Aircoookie
 */
uint16_t WS2812FX::mode_tricolor_fade(void)
{
  uint16_t counter = now * ((_segments[_segment_index].speed >> 3) +1);
  uint32_t prog = (counter * 768) >> 16;

  uint32_t color1 = 0, color2 = 0;
  byte stage = 0;

  if(prog < 256) {
    color1 = SEGCOLOR(0);
    color2 = SEGCOLOR(1);
    stage = 0;
  } else if(prog < 512) {
    color1 = SEGCOLOR(1);
    color2 = SEGCOLOR(2);
    stage = 1;
  } else {
    color1 = SEGCOLOR(2);
    color2 = SEGCOLOR(0);
    stage = 2;
  }

  byte stp = prog; // % 256
  uint32_t color = 0;
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if (stage == 2) {
      color = color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), color2, stp);
    } else if (stage == 1) {
      color = color_blend(color1, color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), stp);
    } else {
      color = color_blend(color1, color2, stp);
    }
    setPixelColor(i, color);
  }

  return FRAMETIME;
}


/*
 * Creates random comets
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/MultiComet.h
 */
uint16_t WS2812FX::mode_multi_comet(void)
{
  uint32_t cycleTime = 10 + (uint32_t)(255 - _segments[_segment_index].speed);
  uint32_t it = now / cycleTime;
  if (_segment_runtimes[_segment_index].step == it) return FRAMETIME;
  if (!_segment_runtimes[_segment_index].allocateData(sizeof(uint16_t) * 8)) return mode_static(); //allocation failed
  
  fade_out(_segments[_segment_index].intensity);
  
  uint16_t* comets = reinterpret_cast<uint16_t*>(_segment_runtimes[_segment_index].data);

  for(uint8_t i=0; i < 8; i++) {
    if(comets[i] < SEGLEN) {
      uint16_t index = comets[i];
      if (SEGCOLOR(2) != 0)
      {
        setPixelColor(index, i % 2 ? color_from_palette(index, true, PALETTE_SOLID_WRAP, 0) : SEGCOLOR(2));
      } else
      {
        setPixelColor(index, color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
      }
      comets[i]++;
    } else {
      if(!random(SEGLEN)) {
        comets[i] = 0;
      }
    }
  }

  _segment_runtimes[_segment_index].step = it;
  return FRAMETIME;
}



typedef struct Oscillator {
  int16_t pos;
  int8_t  size;
  int8_t  dir;
  int8_t  speed;
} oscillator;

/*
/  Oscillating bars of color, updated with standard framerate
*/
uint16_t WS2812FX::mode_oscillate(void)
{
  uint8_t numOscillators = 3;
  uint16_t dataSize = sizeof(oscillator) * numOscillators;

  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
  
  Oscillator* oscillators = reinterpret_cast<Oscillator*>(_segment_runtimes[_segment_index].data);

  if (_segment_runtimes[_segment_index].call == 0)
  {
    oscillators[0] = {SEGLEN/4,   SEGLEN/8,  1, 1};
    oscillators[1] = {SEGLEN/4*3, SEGLEN/8,  1, 2};
    oscillators[2] = {SEGLEN/4*2, SEGLEN/8, -1, 1};
  }

  uint32_t cycleTime = 20 + (2 * (uint32_t)(255 - _segments[_segment_index].speed));
  uint32_t it = now / cycleTime;

  for(uint8_t i = 0; i < numOscillators; i++) {
    // if the counter has increased, move the oscillator by the random step
    if (it != _segment_runtimes[_segment_index].step) oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
    oscillators[i].size = SEGLEN/(3+_segments[_segment_index].intensity/8);
    if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
      oscillators[i].pos = 0;
      oscillators[i].dir = 1;
      // make bigger steps for faster speeds
      oscillators[i].speed = _segments[_segment_index].speed > 100 ? random8(2, 4):random8(1, 3);
    }
    if((oscillators[i].dir == 1) && (oscillators[i].pos >= (SEGLEN - 1))) {
      oscillators[i].pos = SEGLEN - 1;
      oscillators[i].dir = -1;
      oscillators[i].speed = _segments[_segment_index].speed > 100 ? random8(2, 4):random8(1, 3);
    }
  }

  for(uint16_t i=0; i < SEGLEN; i++) {
    uint32_t color = BLACK;
    for(uint8_t j=0; j < numOscillators; j++) {
      if(i >= oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
        color = (color == BLACK) ? SEGCOLOR(j) : color_blend(color, SEGCOLOR(j), 128);
      }
    }
    setPixelColor(i, color);
  }
 
  _segment_runtimes[_segment_index].step = it;
  return FRAMETIME;
}


uint16_t WS2812FX::mode_lightning(void)
{
  uint16_t ledstart = random16(SEGLEN);               // Determine starting location of flash
  uint16_t ledlen = 1 + random16(SEGLEN -ledstart);    // Determine length of flash (not to go beyond NUM_LEDS-1)
  uint8_t bri = 255/random8(1, 3);

  if (_segment_runtimes[_segment_index].step == 0)
  {
    _segment_runtimes[_segment_index].aux0 = random8(3, 3 + _segments[_segment_index].intensity/20); //number of flashes
    bri = 52;
    _segment_runtimes[_segment_index].aux1 = 1;
  }

  fill(SEGCOLOR(1));

  if (_segment_runtimes[_segment_index].aux1) {
    for (int i = ledstart; i < ledstart + ledlen; i++)
    {
      if (_segments[_segment_index].palette == 0)
      {
        setPixelColor(i,bri,bri,bri,bri);
      } else {
        setPixelColor(i,color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, bri));
      }
    }
    _segment_runtimes[_segment_index].aux1 = 0;
    _segment_runtimes[_segment_index].step++;
    return random8(4, 10);                                    // each flash only lasts 4-10 milliseconds
  }

  _segment_runtimes[_segment_index].aux1 = 1;
  if (_segment_runtimes[_segment_index].step == 1) return (200);                       // longer delay until next flash after the leader

  if (_segment_runtimes[_segment_index].step <= _segment_runtimes[_segment_index].aux0) return (50 + random8(100));  // shorter delay between strokes

  _segment_runtimes[_segment_index].step = 0;
  return (random8(255 - _segments[_segment_index].speed) * 100);                            // delay between strikes
}


// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
uint16_t WS2812FX::mode_pride_2015(void)
{
  uint16_t duration = 10 + _segments[_segment_index].speed;
  uint16_t sPseudotime = _segment_runtimes[_segment_index].step;
  uint16_t sHue16 = _segment_runtimes[_segment_index].aux0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for (uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);
    fastled_col = col_to_crgb(getPixelColor(i));

    nblend(fastled_col, newcolor, 64);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[_segment_index].step = sPseudotime;
  _segment_runtimes[_segment_index].aux0 = sHue16;
  return FRAMETIME;
}


//eight colored dots, weaving in and out of sync with each other
uint16_t WS2812FX::mode_juggle(void){
  fade_out(_segments[_segment_index].intensity);
  CRGB fastled_col;
  byte dothue = 0;
  for ( byte i = 0; i < 8; i++) {
    uint16_t index = 0 + beatsin88((128 + _segments[_segment_index].speed)*(i + 7), 0, SEGLEN -1);
    fastled_col = col_to_crgb(getPixelColor(index));
    fastled_col |= (_segments[_segment_index].palette==0)?CHSV(dothue, 220, 255):ColorFromPalette(currentPalette, dothue, 255);
    setPixelColor(index, fastled_col.red, fastled_col.green, fastled_col.blue);
    dothue += 32;
  }
  return FRAMETIME;
}


uint16_t WS2812FX::mode_palette()
{
  uint16_t counter = 0;
  if (_segments[_segment_index].speed != 0) 
  {
    counter = (now * ((_segments[_segment_index].speed >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && _segments[_segment_index].speed == 0));
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint8_t colorIndex = (i * 255 / SEGLEN) - counter;
    
    if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
    
    setPixelColor(i, color_from_palette(colorIndex, false, true, 255));
  }
  return FRAMETIME;
}


// WLED limitation: Analog Clock overlay will NOT work when Fire2012 is active
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
// in step 3 above) (Effect Intensity = Sparking).


uint16_t WS2812FX::mode_fire_2012()
{
  uint32_t it = now >> 5; //div 32

  if (!_segment_runtimes[_segment_index].allocateData(SEGLEN)) return mode_static(); //allocation failed
  
  byte* heat = _segment_runtimes[_segment_index].data;

  if (it != _segment_runtimes[_segment_index].step)
  {
    // Step 1.  Cool down every cell a little
    for (uint16_t i = 0; i < SEGLEN; i++) {
      _segment_runtimes[_segment_index].data[i] = qsub8(heat[i],  random8(0, (((20 + _segments[_segment_index].speed /3) * 10) / SEGLEN) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k= SEGLEN -1; k > 1; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() <= _segments[_segment_index].intensity) {
      uint8_t y = random8(7);
      if (y < SEGLEN) heat[y] = qadd8(heat[y], random8(160,255));
    }
    _segment_runtimes[_segment_index].step = it;
  }

  // Step 4.  Map from heat cells to LED colors
  for (uint16_t j = 0; j < SEGLEN; j++) {
    CRGB color = ColorFromPalette(currentPalette, MIN(heat[j],240), 255, LINEARBLEND);
    setPixelColor(j, color.red, color.green, color.blue);
  }
  return FRAMETIME;
}


// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
uint16_t WS2812FX::mode_colorwaves()
{
  uint16_t duration = 10 + _segments[_segment_index].speed;
  uint16_t sPseudotime = _segment_runtimes[_segment_index].step;
  uint16_t sHue16 = _segment_runtimes[_segment_index].aux0;

  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  // uint16_t hueinc16 = beatsin88(113, 300, 1500);
  uint16_t hueinc16 = beatsin88(113, 60, 300)*_segments[_segment_index].intensity*10/255;  // Use the Intensity Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for ( uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = ColorFromPalette(currentPalette, hue8, bri8);
    fastled_col = col_to_crgb(getPixelColor(i));

    nblend(fastled_col, newcolor, 128);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[_segment_index].step = sPseudotime;
  _segment_runtimes[_segment_index].aux0 = sHue16;
  return FRAMETIME;
}


// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
uint16_t WS2812FX::mode_bpm()
{
  CRGB fastled_col;
  uint32_t stp = (now / 20) & 0xFF;
  uint8_t beat = beatsin8(_segments[_segment_index].speed, 64, 255);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = ColorFromPalette(currentPalette, stp + (i * 2), beat - stp + (i * 10));
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  return FRAMETIME;
}




//based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
uint16_t WS2812FX::mode_colortwinkle()
{
  uint16_t dataSize = (SEGLEN+7) >> 3; //1 bit per LED
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
  
  CRGB fastled_col, prev;
  fract8 fadeUpAmount = 8 + (_segments[_segment_index].speed/4), fadeDownAmount = 5 + (_segments[_segment_index].speed/7);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = col_to_crgb(getPixelColor(i));
    prev = fastled_col;
    uint16_t index = i >> 3;
    uint8_t  bitNum = i & 0x07;
    bool fadeUp = bitRead(_segment_runtimes[_segment_index].data[index], bitNum);
    
    if (fadeUp) {
      CRGB incrementalColor = fastled_col;
      incrementalColor.nscale8_video( fadeUpAmount);
      fastled_col += incrementalColor;

      if (fastled_col.red == 255 || fastled_col.green == 255 || fastled_col.blue == 255) {
        bitWrite(_segment_runtimes[_segment_index].data[index], bitNum, false);
      }
      setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);

      if (col_to_crgb(getPixelColor(i)) == prev) //fix "stuck" pixels
      {
        fastled_col += fastled_col;
        setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
      }
    } else {
      fastled_col.nscale8( 255 - fadeDownAmount);
      setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
    }
  }

  for (uint16_t j = 0; j <= SEGLEN / 50; j++)
  {
    if (random8() <= _segments[_segment_index].intensity) {
      for (uint8_t times = 0; times < 5; times++) //attempt to spawn a new pixel 5 times
      {
        int i = random16(SEGLEN);
        if(getPixelColor(i) == 0) {
          fastled_col = ColorFromPalette(currentPalette, random8(), 64, NOBLEND);
          uint16_t index = i >> 3;
          uint8_t  bitNum = i & 0x07;
          bitWrite(_segment_runtimes[_segment_index].data[index], bitNum, true);
          setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  return FRAMETIME;
}


//Calm effect, like a lake at night
uint16_t WS2812FX::mode_lake() {
  uint8_t sp = _segments[_segment_index].speed/10;
  int wave1 = beatsin8(sp +2, -64,64);
  int wave2 = beatsin8(sp +1, -64,64);
  uint8_t wave3 = beatsin8(sp +2,   0,80);
  CRGB fastled_col;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    int index = cos8((i*15)+ wave1)/2 + cubicwave8((i*23)+ wave2)/2;           
    uint8_t lum = (index > wave3) ? index - wave3 : 0;
    fastled_col = ColorFromPalette(currentPalette, map(index,0,255,0,240), lum, LINEARBLEND);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  return FRAMETIME;
}


// meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
uint16_t WS2812FX::mode_meteor() {
  if (!_segment_runtimes[_segment_index].allocateData(SEGLEN)) return mode_static(); //allocation failed

  byte* trail = _segment_runtimes[_segment_index].data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) +8);
  uint16_t in = counter * SEGLEN >> 16;

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (random8() <= 255 - _segments[_segment_index].intensity)
    {
      byte meteorTrailDecay = 128 + random8(127);
      trail[i] = scale8(trail[i], meteorTrailDecay);
      setPixelColor(i, color_from_palette(trail[i], false, true, 255));
    }
  }

  // draw meteor
  for(int j = 0; j < meteorSize; j++) {
    uint16_t index = in + j;
    if(index >= SEGLEN) {
      index = (in + j - SEGLEN);
    }

    trail[index] = 240;
    setPixelColor(index, color_from_palette(trail[index], false, true, 255));
  }

  return FRAMETIME;
}


// smooth meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
uint16_t WS2812FX::mode_meteor_smooth() {
  if (!_segment_runtimes[_segment_index].allocateData(SEGLEN)) return mode_static(); //allocation failed

  byte* trail = _segment_runtimes[_segment_index].data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t in = map((_segment_runtimes[_segment_index].step >> 6 & 0xFF), 0, 255, 0, SEGLEN -1);

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (trail[i] != 0 && random8() <= 255 - _segments[_segment_index].intensity)
    {
      int change = 3 - random8(12); //change each time between -8 and +3
      trail[i] += change;
      if (trail[i] > 245) trail[i] = 0;
      if (trail[i] > 240) trail[i] = 240;
      setPixelColor(i, color_from_palette(trail[i], false, true, 255));
    }
  }
  
  // draw meteor
  for(int j = 0; j < meteorSize; j++) {  
    uint16_t index = in + j;   
    if(in + j >= SEGLEN) {
      index = (in + j - SEGLEN);
    }
    setPixelColor(index, color_blend(getPixelColor(index), color_from_palette(240, false, true, 255), 48));
    trail[index] = 240;
  }

  _segment_runtimes[_segment_index].step += _segments[_segment_index].speed +1;
  return FRAMETIME;
}


//Railway Crossing / Christmas Fairy lights
uint16_t WS2812FX::mode_railway()
{
  uint16_t dur = 40 + (255 - _segments[_segment_index].speed) * 10;
  uint16_t rampdur = (dur * _segments[_segment_index].intensity) >> 8;
  if (_segment_runtimes[_segment_index].step > dur)
  {
    //reverse direction
    _segment_runtimes[_segment_index].step = 0;
    _segment_runtimes[_segment_index].aux0 = !_segment_runtimes[_segment_index].aux0;
  }
  uint8_t pos = 255;
  if (rampdur != 0)
  {
    uint16_t p0 = (_segment_runtimes[_segment_index].step * 255) / rampdur;
    if (p0 < 255) pos = p0;
  }
  if (_segment_runtimes[_segment_index].aux0) pos = 255 - pos;
  for (uint16_t i = 0; i < SEGLEN; i += 2)
  {
    setPixelColor(i, color_from_palette(255 - pos, false, false, 255));
    if (i < SEGLEN -1)
    {
      setPixelColor(i + 1, color_from_palette(pos, false, false, 255));
    }
  }
  _segment_runtimes[_segment_index].step += FRAMETIME;
  return FRAMETIME;
}


//Water ripple
//propagation velocity from speed
//drop rate from intensity

//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;

uint16_t WS2812FX::ripple_base(bool rainbow)
{
  uint16_t maxRipples = 1 + (SEGLEN >> 2);
  if (maxRipples > 100) maxRipples = 100;
  uint16_t dataSize = sizeof(ripple) * maxRipples;

  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
 
  Ripple* ripples = reinterpret_cast<Ripple*>(_segment_runtimes[_segment_index].data);

  // ranbow background or chosen background, all very dim.
  if (rainbow) {
    if (_segment_runtimes[_segment_index].call ==0) {
      _segment_runtimes[_segment_index].aux0 = random8();
      _segment_runtimes[_segment_index].aux1 = random8();
    }
    if (_segment_runtimes[_segment_index].aux0 == _segment_runtimes[_segment_index].aux1) {
      _segment_runtimes[_segment_index].aux1 = random8();
    }
    else if (_segment_runtimes[_segment_index].aux1 > _segment_runtimes[_segment_index].aux0) {
      _segment_runtimes[_segment_index].aux0++;
    } else {
      _segment_runtimes[_segment_index].aux0--;
    }
    fill(color_blend(color_wheel(_segment_runtimes[_segment_index].aux0),BLACK,235));
  } else {
    fill(SEGCOLOR(1));
  }
  
  //draw wave
  for (uint16_t i = 0; i < maxRipples; i++)
  {
    uint16_t ripplestate = ripples[i].state;
    if (ripplestate)
    {
      uint8_t rippledecay = (_segments[_segment_index].speed >> 4) +1; //faster decay if faster propagation
      uint16_t rippleorigin = ripples[i].pos;
      uint32_t col = color_from_palette(ripples[i].color, false, false, 255);
      uint16_t propagation = ((ripplestate/rippledecay -1) * _segments[_segment_index].speed);
      int16_t propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      int16_t left = rippleorigin - propI -1;
      uint8_t amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      for (int16_t v = left; v < left +4; v++)
      {
        uint8_t mag = scale8(cubicwave8((propF>>2)+(v-left)*64), amp);
        if (v < SEGLEN && v >= 0)
        {
          setPixelColor(v, color_blend(getPixelColor(v), col, mag));
        }
        int16_t w = left + propI*2 + 3 -(v-left);
        if (w < SEGLEN && w >= 0)
        {
          setPixelColor(w, color_blend(getPixelColor(w), col, mag));
        }
      }  
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else //randomly create new wave
    {
      if (random16(IBN + 10000) <= _segments[_segment_index].intensity)
      {
        ripples[i].state = 1;
        ripples[i].pos = random16(SEGLEN);
        ripples[i].color = random8(); //color
      }
    }
  }
  return FRAMETIME;
}

uint16_t WS2812FX::mode_ripple(void) {
  return ripple_base(false);
}

uint16_t WS2812FX::mode_ripple_rainbow(void) {
  return ripple_base(true);
}



//  TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
//
//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette. Read more about this effect using the link above!

// If COOL_LIKE_INCANDESCENT is set to 1, colors will
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1

CRGB WS2812FX::twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  // Overall twinkle speed (changed)
  uint16_t ticks = ms / _segment_runtimes[_segment_index].aux0;
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  uint8_t twinkleDensity = (_segments[_segment_index].intensity >> 5) +1;

  uint8_t bright = 0;
  if (((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    uint8_t ph = fastcycle8;
    // This is like 'triwave8', which produces a
    // symmetrical up-and-down triangle sawtooth waveform, except that this
    // function produces a triangle wave with a faster attack and a slower decay
    if (cat) //twinklecat, variant where the leds instantly turn on
    {
      bright = 255 - ph;
    } else { //vanilla twinklefox
      if (ph < 86) {
      bright = ph * 3;
      } else {
        ph -= 86;
        bright = 255 - (ph + (ph/2));
      }
    }
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if (bright > 0) {
    c = ColorFromPalette(currentPalette, hue, bright, NOBLEND);
    if(COOL_LIKE_INCANDESCENT == 1) {
      // This code takes a pixel, and if its in the 'fading down'
      // part of the cycle, it adjusts the color a little bit like the
      // way that incandescent bulbs fade toward 'red' as they dim.
      if (fastcycle8 >= 128) 
      {
        uint8_t cooling = (fastcycle8 - 128) >> 4;
        c.g = qsub8(c.g, cooling);
        c.b = qsub8(c.b, cooling * 2);
      }
    }
  } else {
    c = HTMLColorCode::Black;
  }
  return c;
}

//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
uint16_t WS2812FX::twinklefox_base(bool cat)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed
  if (_segments[_segment_index].speed > 100) _segment_runtimes[_segment_index].aux0 = 3 + ((255 - _segments[_segment_index].speed) >> 3);
  else _segment_runtimes[_segment_index].aux0 = 22 + ((100 - _segments[_segment_index].speed) >> 1);

  // Set up the background color, "bg".
  CRGB bg;
  bg = col_to_crgb(SEGCOLOR(1));
  uint8_t bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (uint16_t i = 0; i < SEGLEN; i++) {
  
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((now * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = twinklefox_one_twinkle(myclock30, myunique8, cat);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      setPixelColor(i, c.red, c.green, c.blue);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      setPixelColor(i, color_blend(crgb_to_col(bg), crgb_to_col(c), deltabright * 8));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      setPixelColor(i, bg.r, bg.g, bg.b);
    }
  }
  return FRAMETIME;
}

uint16_t WS2812FX::mode_twinklefox()
{
  return twinklefox_base(false);
}

uint16_t WS2812FX::mode_twinklecat()
{
  return twinklefox_base(true);
}


// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectColorWipe
// LEDStrip Effect – Meteor Rain


//inspired by https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBlinkingHalloweenEyes
#define HALLOWEEN_EYE_SPACE 3
#define HALLOWEEN_EYE_WIDTH 1

uint16_t WS2812FX::mode_halloween_eyes()
{  
  uint16_t eyeLength = (2*HALLOWEEN_EYE_WIDTH) + HALLOWEEN_EYE_SPACE;
  if (eyeLength > SEGLEN) return mode_static(); //bail if segment too short

  fill(SEGCOLOR(1)); //fill background

  uint8_t state = _segment_runtimes[_segment_index].aux1 >> 8;
  uint16_t stateTime = _segment_runtimes[_segment_index].call;
  if (stateTime == 0) stateTime = 2000;

  if (state == 0) { //spawn eyes
    _segment_runtimes[_segment_index].aux0 = random16(0, SEGLEN - eyeLength); //start pos
    _segment_runtimes[_segment_index].aux1 = random8(); //color
    state = 1;
  }
  
  if (state < 2) { //fade eyes
    uint16_t startPos    = _segment_runtimes[_segment_index].aux0;
    uint16_t start2ndEye = startPos + HALLOWEEN_EYE_WIDTH + HALLOWEEN_EYE_SPACE;
    
    uint32_t fadestage = (now - _segment_runtimes[_segment_index].step)*255 / stateTime;
    if (fadestage > 255) fadestage = 255;
    uint32_t c = color_blend(color_from_palette(_segment_runtimes[_segment_index].aux1 & 0xFF, false, false, 0), SEGCOLOR(1), fadestage);
    
    for (uint16_t i = 0; i < HALLOWEEN_EYE_WIDTH; i++)
    {
      setPixelColor(startPos    + i, c);
      setPixelColor(start2ndEye + i, c);
    }
  }

  if (now - _segment_runtimes[_segment_index].step > stateTime)
  {
    state++;
    if (state > 2) state = 0;
    
    if (state < 2)
    {
      stateTime = 100 + (255 - _segments[_segment_index].intensity)*10; //eye fade time
    } else {
      uint16_t eyeOffTimeBase = (255 - _segments[_segment_index].speed)*10;
      stateTime = eyeOffTimeBase + random16(eyeOffTimeBase);
    }
    _segment_runtimes[_segment_index].step = now;
    _segment_runtimes[_segment_index].call = stateTime;
  }

  _segment_runtimes[_segment_index].aux1 = (_segment_runtimes[_segment_index].aux1 & 0xFF) + (state << 8); //save state
  
  return FRAMETIME;
}




uint16_t WS2812FX::spots_base(uint16_t threshold)
{
  fill(SEGCOLOR(1));
  
  uint16_t maxZones = SEGLEN >> 2;
  uint16_t zones = 1 + ((_segments[_segment_index].intensity * maxZones) >> 8);
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint16_t wave = triwave16((i * 0xFFFF) / zoneLen);
      if (wave > threshold) {
        uint16_t index = 0 + pos + i;
        uint8_t s = (wave - threshold)*255 / (0xFFFF - threshold);
        setPixelColor(index, color_blend(color_from_palette(index, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), 255-s));
      }
    }
  }
  
  return FRAMETIME;
}


//Intensity slider sets number of "lights", speed sets LEDs per light
uint16_t WS2812FX::mode_spots()
{
  return spots_base((255 - _segments[_segment_index].speed) << 8);
}


//Intensity slider sets number of "lights", LEDs per light fade in and out
uint16_t WS2812FX::mode_spots_fade()
{
  uint16_t counter = now * ((_segments[_segment_index].speed >> 2) +8);
  uint16_t t = triwave16(counter);
  uint16_t tr = (t >> 1) + (t >> 2);
  return spots_base(tr);
}


//each needs 12 bytes
//Spark type is used for popcorn and 1D fireworks
typedef struct Ball {
  unsigned long lastBounceTime;
  float impactVelocity;
  float height;
} ball;

/*
*  Bouncing Balls Effect
*/
uint16_t WS2812FX::mode_bouncing_balls(void) {
  //allocate segment data
  uint16_t maxNumBalls = 16; 
  uint16_t dataSize = sizeof(ball) * maxNumBalls;
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
  
  Ball* balls = reinterpret_cast<Ball*>(_segment_runtimes[_segment_index].data);
  
  // number of balls based on intensity setting to max of 7 (cycles colors)
  // non-chosen color is a random color
  uint8_t numBalls = int(((_segments[_segment_index].intensity * (maxNumBalls - 0.8f)) / 255) + 1);
  
  float gravity                           = -9.81; // standard value of gravity
  float impactVelocityStart               = sqrt( -2 * gravity);

  unsigned long time = millis();

  if (_segment_runtimes[_segment_index].call == 0) {
    for (uint8_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
  }
  
  bool hasCol2 = SEGCOLOR(2);
  fill(hasCol2 ? BLACK : SEGCOLOR(1));
  
  for (uint8_t i = 0; i < numBalls; i++) {
    float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-_segments[_segment_index].speed)*8/256 +1);
    balls[i].height = 0.5 * gravity * pow(timeSinceLastBounce/1000 , 2.0) + balls[i].impactVelocity * timeSinceLastBounce/1000;

    if (balls[i].height < 0) { //start bounce
      balls[i].height = 0;
      //damping for better effect using multiple balls
      float dampening = 0.90 - float(i)/pow(numBalls,2);
      balls[i].impactVelocity = dampening * balls[i].impactVelocity;
      balls[i].lastBounceTime = time;

      if (balls[i].impactVelocity < 0.015) {
        balls[i].impactVelocity = impactVelocityStart;
      }
    }
    
    uint32_t color = SEGCOLOR(0);
    if (_segments[_segment_index].palette) {
      color = color_wheel(i*(256/MAX(numBalls, 8)));
    } else if (hasCol2) {
      color = SEGCOLOR(i % NUM_COLORS);
    }

    uint16_t pos = round(balls[i].height * (SEGLEN - 1));
    setPixelColor(pos, color);
  }

  return FRAMETIME;
}


/*
* Sinelon stolen from FASTLED examples
*/
uint16_t WS2812FX::sinelon_base(bool dual, bool rainbow=false) {
  fade_out(_segments[_segment_index].intensity);
  uint16_t pos = beatsin16(_segments[_segment_index].speed/10,0,SEGLEN-1);
  if (_segment_runtimes[_segment_index].call == 0) _segment_runtimes[_segment_index].aux0 = pos;
  uint32_t color1 = color_from_palette(pos, true, false, 0);
  uint32_t color2 = SEGCOLOR(2);
  if (rainbow) {
    color1 = color_wheel((pos & 0x07) * 32);
  }
  setPixelColor(pos, color1);
  if (dual) {
    if (!color2) color2 = color_from_palette(pos, true, false, 0);
    if (rainbow) color2 = color1; //rainbow
    setPixelColor(SEGLEN-1-pos, color2);
  }
  if (_segment_runtimes[_segment_index].aux0 != pos) { 
    if (_segment_runtimes[_segment_index].aux0 < pos) {
      for (uint16_t i = _segment_runtimes[_segment_index].aux0; i < pos ; i++) {
        setPixelColor(i, color1);
        if (dual) setPixelColor(SEGLEN-1-i, color2);
      }
    } else {
      for (uint16_t i = _segment_runtimes[_segment_index].aux0; i > pos ; i--) {
        setPixelColor(i, color1);
        if (dual) setPixelColor(SEGLEN-1-i, color2);
      }
    }
    _segment_runtimes[_segment_index].aux0 = pos;
  }

  return FRAMETIME;
}

uint16_t WS2812FX::mode_sinelon(void) {
  return sinelon_base(false);
}

uint16_t WS2812FX::mode_sinelon_dual(void) {
  return sinelon_base(true);
}

uint16_t WS2812FX::mode_sinelon_rainbow(void) {
  return sinelon_base(true, true);
}


//Rainbow with glitter, inspired by https://gist.github.com/kriegsman/062e10f7f07ba8518af6
uint16_t WS2812FX::mode_glitter()
{
  mode_palette();

  if (_segments[_segment_index].intensity > random8())
  {
    setPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  
  return FRAMETIME;
}



/*
*  POPCORN
*  modified from https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Popcorn.h
*/
uint16_t WS2812FX::mode_popcorn(void) {
  //allocate segment data
  uint16_t maxNumPopcorn = 24; 
  uint16_t dataSize = sizeof(spark) * maxNumPopcorn;
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed
  
  Spark* popcorn = reinterpret_cast<Spark*>(_segment_runtimes[_segment_index].data);

  float gravity = -0.0001 - (_segments[_segment_index].speed/200000.0); // m/s/s
  gravity *= SEGLEN;

  bool hasCol2 = SEGCOLOR(2);
  fill(hasCol2 ? BLACK : SEGCOLOR(1));

  uint8_t numPopcorn = _segments[_segment_index].intensity*maxNumPopcorn/255;
  if (numPopcorn == 0) numPopcorn = 1;

  for(uint8_t i = 0; i < numPopcorn; i++) {
    bool isActive = popcorn[i].pos >= 0.0f;

    if (isActive) { // if kernel is active, update its position
      popcorn[i].pos += popcorn[i].vel;
      popcorn[i].vel += gravity;
      uint32_t col = color_wheel(popcorn[i].colIndex);
      if (!_segments[_segment_index].palette && popcorn[i].colIndex < NUM_COLORS) col = SEGCOLOR(popcorn[i].colIndex);
      
      uint16_t ledIndex = popcorn[i].pos;
      if (ledIndex < SEGLEN) setPixelColor(ledIndex, col);
    } else { // if kernel is inactive, randomly pop it
      if (random8() < 2) { // POP!!!
        popcorn[i].pos = 0.01f;
        
        uint16_t peakHeight = 128 + random8(128); //0-255
        peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
        popcorn[i].vel = sqrt(-2.0 * gravity * peakHeight);
        
        if (_segments[_segment_index].palette)
        {
          popcorn[i].colIndex = random8();
        } else {
          byte col = random8(0, NUM_COLORS);
          if (!hasCol2 || !SEGCOLOR(col)) col = 0;
          popcorn[i].colIndex = col;
        }
      }
    }
  }

  return FRAMETIME;
}


//values close to 100 produce 5Hz flicker, which looks very candle-y
//Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
//and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/

uint16_t WS2812FX::candle(bool multi)
{
multi=1;
    AddLog(LOG_LEVEL_TEST, PSTR("multi=%d"),multi);
  // DEBUG_LINE_HERE;
  if (multi)
  {
    //allocate segment data
    uint16_t dataSize = (SEGLEN -1) *3;
    #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
    AddLog(LOG_LEVEL_TEST, PSTR("WS2812FX::candle dataSize=%d"),dataSize);
    #endif
    if (!_segment_runtimes[_segment_index].allocateData(dataSize)){
    AddLog(LOG_LEVEL_TEST, PSTR("return candle(false); dataSize=%d"),dataSize);
 return candle(false); //allocation failed
    }
  }

  //max. flicker range controlled by intensity
  uint8_t valrange = _segments[_segment_index].intensity;
  uint8_t rndval = valrange >> 1;

  #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
  // AddLog(LOG_LEVEL_TEST, PSTR("step=%d"),_segment_runtimes[_segment_index].step);
  // AddLog(LOG_LEVEL_TEST, PSTR("valrange=%d"),valrange);
  // AddLog(LOG_LEVEL_TEST, PSTR("rndval=%d"),rndval);
  #endif

  //step (how much to move closer to target per frame) coarsely set by speed
  uint8_t speedFactor = 4;
  if (_segments[_segment_index].speed > 252) { //epilepsy
    speedFactor = 1;
  } else if (_segments[_segment_index].speed > 99) { //regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
    speedFactor = 2;
  } else if (_segments[_segment_index].speed > 49) { //slower fade
    speedFactor = 3;
  } //else 4 (slowest)

  uint16_t numCandles = (multi) ? SEGLEN : 1;

  for (uint16_t i = 0; i < numCandles; i++)
  {
    uint16_t d = 0; //data location

    uint8_t s = _segment_runtimes[_segment_index].aux0, s_target = _segment_runtimes[_segment_index].aux1, fadeStep = _segment_runtimes[_segment_index].step;
    if (i > 0) {
      d = (i-1) *3;
      s = _segment_runtimes[_segment_index].data[d]; s_target = _segment_runtimes[_segment_index].data[d+1]; fadeStep = _segment_runtimes[_segment_index].data[d+2];
    }
    if (fadeStep == 0) { //init vals
      s = 128; s_target = 130 + random8(4); fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { //fade up
  #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
  // AddLog(LOG_LEVEL_TEST, PSTR("fade up s_target > s %d=%d"),s_target,s);
  #endif

      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    } else {
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
      
  #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
  // AddLog(LOG_LEVEL_TEST, PSTR("fade down=%d"),s);
  #endif

    }

    if (newTarget) {
      s_target = random8(rndval) + random8(rndval);
      if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
      uint8_t offset = (255 - valrange) >> 1;
      s_target += offset;

      uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
      fadeStep = dif >> speedFactor;
      if (fadeStep == 0) fadeStep = 1;
    }

     if (i > 0) {
      setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), s));

      _segment_runtimes[_segment_index].data[d] = s; 
      _segment_runtimes[_segment_index].data[d+1] = s_target; 
      _segment_runtimes[_segment_index].data[d+2] = fadeStep;
    } else {
      for (uint16_t j = 0; j < SEGLEN; j++) {
        setPixelColor(j, color_blend(SEGCOLOR(1), color_from_palette(j, true, PALETTE_SOLID_WRAP, 0), s));
      }

      _segment_runtimes[_segment_index].aux0 = s; 
      _segment_runtimes[_segment_index].aux1 = s_target; 
      _segment_runtimes[_segment_index].step = fadeStep;
    }
  }

  return FRAMETIME;
}

uint16_t WS2812FX::mode_candle()
{
  return candle(false);
}


uint16_t WS2812FX::mode_candle_multi()
{
  return candle(true);
}



/*
 * Drip Effect
 * ported of: https://www.youtube.com/watch?v=sru2fXh4r7k
 */
uint16_t WS2812FX::mode_drip(void)
{
  //allocate segment data
  uint16_t numDrops = 4; 
  uint16_t dataSize = sizeof(spark) * numDrops;
  if (!_segment_runtimes[_segment_index].allocateData(dataSize)) return mode_static(); //allocation failed

  fill(SEGCOLOR(1));
  
  Spark* drops = reinterpret_cast<Spark*>(_segment_runtimes[_segment_index].data);

  numDrops = 1 + (_segments[_segment_index].intensity >> 6);

  float gravity = -0.001 - (_segments[_segment_index].speed/50000.0);
  gravity *= SEGLEN;
  int sourcedrop = 12;

  for (int j=0;j<numDrops;j++) {
    if (drops[j].colIndex == 0) { //init
      drops[j].pos = SEGLEN-1;    // start at end
      drops[j].vel = 0;           // speed
      drops[j].col = sourcedrop;  // brightness
      drops[j].colIndex = 1;      // drop state (0 init, 1 forming, 2 falling, 5 bouncing) 
    }
    
    setPixelColor(SEGLEN-1,color_blend(BLACK,SEGCOLOR(0), sourcedrop));// water source
    if (drops[j].colIndex==1) {
      if (drops[j].col>255) drops[j].col=255;
      setPixelColor(int(drops[j].pos),color_blend(BLACK,SEGCOLOR(0),drops[j].col));
      
      drops[j].col += map(_segments[_segment_index].speed, 0, 255, 1, 6); // swelling
      
      if (random8() < drops[j].col/10) {               // random drop
        drops[j].colIndex=2;               //fall
        drops[j].col=255;
      }
    }  
    if (drops[j].colIndex > 1) {           // falling
      if (drops[j].pos > 0) {              // fall until end of segment
        drops[j].pos += drops[j].vel;
        if (drops[j].pos < 0) drops[j].pos = 0;
        drops[j].vel += gravity;

        for (int i=1;i<7-drops[j].colIndex;i++) { // some minor math so we don't expand bouncing droplets
          setPixelColor(int(drops[j].pos)+i,color_blend(BLACK,SEGCOLOR(0),drops[j].col/i)); //spread pixel with fade while falling
        }
        
        if (drops[j].colIndex > 2) {       // during bounce, some water is on the floor
          setPixelColor(0,color_blend(SEGCOLOR(0),BLACK,drops[j].col));
        }
      } else {                             // we hit bottom
        if (drops[j].colIndex > 2) {       // already hit once, so back to forming
          drops[j].colIndex = 0;
          drops[j].col = sourcedrop;
          
        } else {

          if (drops[j].colIndex==2) {      // init bounce
            drops[j].vel = -drops[j].vel/4;// reverse velocity with damping 
            drops[j].pos += drops[j].vel;
          } 
          drops[j].col = sourcedrop*2;
          drops[j].colIndex = 5;           // bouncing
        }
      }
    }
  }
  return FRAMETIME;  
}


/*
/ Plasma Effect
/ adapted from https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino
*/
uint16_t WS2812FX::mode_plasma(void) {
  uint8_t thisPhase = beatsin8(6,-64,64);                       // Setting phase change for a couple of waves.
  uint8_t thatPhase = beatsin8(7,-64,64);

  for (int i = 0; i < SEGLEN; i++) {   // For each of the LED's in the strand, set color &  brightness based on a wave as follows:
    uint8_t colorIndex = cubicwave8((i*(1+ 3*(_segments[_segment_index].speed >> 5)))+(thisPhase) & 0xFF)/2   // factor=23 // Create a wave and add a phase change and add another wave with its own phase change.
                             + cos8((i*(1+ 2*(_segments[_segment_index].speed >> 5)))+(thatPhase) & 0xFF)/2;  // factor=15 // Hey, you can even change the frequencies if you wish.
    uint8_t thisBright = qsub8(colorIndex, beatsin8(6,0, (255 - _segments[_segment_index].intensity)|0x01 ));
    CRGB color = ColorFromPalette(currentPalette, colorIndex, thisBright, LINEARBLEND);
    setPixelColor(i, color.red, color.green, color.blue);
  }

  return FRAMETIME;
} 


/*
 * Percentage display
 * Intesity values from 0-100 turn on the leds.
 */
uint16_t WS2812FX::mode_percent(void) {

	uint8_t percent = MAX(0, MIN(200, _segments[_segment_index].intensity));
	uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  
  uint8_t size = (1 + ((_segments[_segment_index].speed * SEGLEN) >> 11));
  if (_segments[_segment_index].speed == 255) size = 255;
    
  if (percent < 100) {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < _segment_runtimes[_segment_index].step) {
        setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
	  	}
	  	else {
        setPixelColor(i, SEGCOLOR(1));
	  	}
	  }
  } else {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < (SEGLEN - _segment_runtimes[_segment_index].step)) {
        setPixelColor(i, SEGCOLOR(1));
	  	}
	  	else {
        setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
	  	}
	  }
  }

  if(active_leds > _segment_runtimes[_segment_index].step) {  // smooth transition to the target value
    _segment_runtimes[_segment_index].step += size;
    if (_segment_runtimes[_segment_index].step > active_leds) _segment_runtimes[_segment_index].step = active_leds;
  } else if (active_leds < _segment_runtimes[_segment_index].step) {
    if (_segment_runtimes[_segment_index].step > size) _segment_runtimes[_segment_index].step -= size; else _segment_runtimes[_segment_index].step = 0;
    if (_segment_runtimes[_segment_index].step < active_leds) _segment_runtimes[_segment_index].step = active_leds;
  }

 	return FRAMETIME;
}

/*
/ Modulates the brightness similar to a heartbeat
*/
uint16_t WS2812FX::mode_heartbeat(void) {
  uint8_t bpm = 40 + (_segments[_segment_index].speed >> 4);
  uint32_t msPerBeat = (60000 / bpm);
  uint32_t secondBeat = (msPerBeat / 3);

  uint32_t bri_lower = _segment_runtimes[_segment_index].aux1;
  bri_lower = bri_lower * 2042 / (2048 + _segments[_segment_index].intensity);
  _segment_runtimes[_segment_index].aux1 = bri_lower;

  unsigned long beatTimer = millis() - _segment_runtimes[_segment_index].step;
  if((beatTimer > secondBeat) && !_segment_runtimes[_segment_index].aux0) { // time for the second beat?
    _segment_runtimes[_segment_index].aux1 = UINT16_MAX; //full bri
    _segment_runtimes[_segment_index].aux0 = 1;
  }
  if(beatTimer > msPerBeat) { // time to reset the beat timer?
    _segment_runtimes[_segment_index].aux1 = UINT16_MAX; //full bri
    _segment_runtimes[_segment_index].aux0 = 0;
    _segment_runtimes[_segment_index].step = millis();
  }

  for (uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), 255 - (_segment_runtimes[_segment_index].aux1 >> 8)));
  }

  return FRAMETIME;
}


//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//
//
// In this animation, there are four "layers" of waves of light.  
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then 
// another filter is applied that adds "whitecaps" of brightness where the 
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
// Modified for WLED, based on https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino
//
uint16_t WS2812FX::mode_pacifica()
{
  CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
  CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
  CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

  if (_segments[_segment_index].palette) {
    pacifica_palette_1 = currentPalette;
    pacifica_palette_2 = currentPalette;
    pacifica_palette_3 = currentPalette;
  }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  uint16_t sCIStart1 = _segment_runtimes[_segment_index].aux0, sCIStart2 = _segment_runtimes[_segment_index].aux1, sCIStart3 = _segment_runtimes[_segment_index].step, sCIStart4 = _segment_runtimes[_segment_index].step >> 16;
  //static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  uint32_t deltams = 26 + (_segments[_segment_index].speed >> 3);
  
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  _segment_runtimes[_segment_index].aux0 = sCIStart1; _segment_runtimes[_segment_index].aux1 = sCIStart2;
  _segment_runtimes[_segment_index].step = sCIStart4; _segment_runtimes[_segment_index].step = (_segment_runtimes[_segment_index].step << 16) + sCIStart3;

  // Clear out the LED array to a dim background blue-green
  //fill(132618);

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < SEGLEN; i++) {
    CRGB c = CRGB(2, 6, 10);
    // Render each of four layers, with different scales and speeds, that vary over time
    c += pacifica_one_layer(i, pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0-beat16(301));
    c += pacifica_one_layer(i, pacifica_palette_2, sCIStart2, beatsin16(4,  6 * 256,  9 * 256), beatsin8(17, 40,  80),   beat16(401));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart3,                         6 * 256 , beatsin8(9, 10,38)   , 0-beat16(503));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart4,                         5 * 256 , beatsin8(8, 10,28)   ,   beat16(601));
    
    // Add extra 'white' to areas where the four layers of light have lined up brightly
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = c.getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      c += CRGB(overage, overage2, qadd8(overage2, overage2));
    }

    //deepen the blues and greens
    c.blue  = scale8(c.blue,  145); 
    c.green = scale8(c.green, 200); 
    c |= CRGB( 2, 5, 7);

    setPixelColor(i, c.red, c.green, c.blue);
  }

  return FRAMETIME;
}

// Add one layer of waves into the led array
CRGB WS2812FX::pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale >> 1) + 20;
  
  waveangle += ((120 + _segments[_segment_index].intensity) * i); //original 250 * i
  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return ColorFromPalette(p, sindex8, bri, LINEARBLEND);
}

//Solid colour background with glitter
uint16_t WS2812FX::mode_solid_glitter()
{
  fill(SEGCOLOR(0));

  if (_segments[_segment_index].intensity > random8())
  {
    setPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  return FRAMETIME;
}


/*
 * Mode simulates a gradual sunrise
 */
uint16_t WS2812FX::mode_sunrise() {
  //speed 0 - static sun
  //speed 1 - 60: sunrise time in minutes
  //speed 60 - 120 : sunset time in minutes - 60;
  //speed above: "breathing" rise and set
  if (_segment_runtimes[_segment_index].call == 0 || _segments[_segment_index].speed != _segment_runtimes[_segment_index].aux0) {
	  _segment_runtimes[_segment_index].step = millis(); //save starting time, millis() because now can change from sync
    _segment_runtimes[_segment_index].aux0 = _segments[_segment_index].speed;
  }
  
  fill(0);
  uint16_t stage = 0xFFFF;
  
  uint32_t s10SinceStart = (millis() - _segment_runtimes[_segment_index].step) /100; //tenths of seconds
  
  if (_segments[_segment_index].speed > 120) { //quick sunrise and sunset
	  uint16_t counter = (now >> 1) * (((_segments[_segment_index].speed -120) >> 1) +1);
	  stage = triwave16(counter);
  } else if (_segments[_segment_index].speed) { //sunrise
	  uint8_t durMins = _segments[_segment_index].speed;
	  if (durMins > 60) durMins -= 60;
	  uint32_t s10Target = durMins * 600;
	  if (s10SinceStart > s10Target) s10SinceStart = s10Target;
	  stage = map(s10SinceStart, 0, s10Target, 0, 0xFFFF);
	  if (_segments[_segment_index].speed > 60) stage = 0xFFFF - stage; //sunset
  }
  
  for (uint16_t i = 0; i <= SEGLEN/2; i++)
  {
    //default palette is Fire
    uint32_t c = color_from_palette(0, false, true, 255); //background

    uint16_t wave = triwave16((i * stage) / SEGLEN);

    wave = (wave >> 8) + ((wave * _segments[_segment_index].intensity) >> 15);

    if (wave > 240) { //clipped, full white sun
      c = color_from_palette( 240, false, true, 255);
    } else { //transition
      c = color_from_palette(wave, false, true, 255);
    }
    setPixelColor(i, c);
    setPixelColor(SEGLEN - i - 1, c);
  }

  return FRAMETIME;
}


// Sine waves that have controllable phase change speed, frequency and cutoff. By Andrew Tuline.
// _segments[_segment_index].speed ->Speed, _segments[_segment_index].intensity -> Frequency (_segments[_segment_index].fft1 -> Color change, _segments[_segment_index].fft2 -> PWM cutoff)
//
uint16_t WS2812FX::mode_sinewave(void) {             // Adjustable sinewave. By Andrew Tuline
  //#define qsuba(x, b)  ((x>b)?x-b:0)               // Analog Unsigned subtraction macro. if result <0, then => 0

  uint16_t colorIndex = now /32;//(256 - _segments[_segment_index].fft1);  // Amount of colour change.

  _segment_runtimes[_segment_index].step += _segments[_segment_index].speed/16;                   // Speed of animation.
  uint16_t freq = _segments[_segment_index].intensity/4;//_segments[_segment_index].fft2/8;                       // Frequency of the signal.

  for (int i=0; i<SEGLEN; i++) {                   // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int pixBri = cubicwave8((i*freq)+_segment_runtimes[_segment_index].step);//qsuba(cubicwave8((i*freq)+_segment_runtimes[_segment_index].step), (255-_segments[_segment_index].intensity)); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    //setPixCol(i, i*colorIndex/255, pixBri);
    setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i*colorIndex/255, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  return FRAMETIME;
}


/*
 * Best of both worlds from Palette and Spot effects. By Aircoookie
 */
uint16_t WS2812FX::mode_flow(void)
{
  uint16_t counter = 0;
  if (_segments[_segment_index].speed != 0) 
  {
    counter = now * ((_segments[_segment_index].speed >> 2) +1);
    counter = counter >> 8;
  }
  
  uint16_t maxZones = SEGLEN / 6; //only looks good if each zone has at least 6 LEDs
  uint16_t zones = (_segments[_segment_index].intensity * maxZones) >> 8;
  if (zones & 0x01) zones++; //zones must be even
  if (zones < 2) zones = 2;
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  fill(color_from_palette(-counter, false, true, 255));

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint8_t colorIndex = (i * 255 / zoneLen) - counter;
      uint16_t led = (z & 0x01) ? i : (zoneLen -1) -i;
      if (IS_REVERSE) led = (zoneLen -1) -led;
      setPixelColor(pos + led, color_from_palette(colorIndex, false, true, 255));
    }
  }

  return FRAMETIME;
}


/*
 * Dots waving around in a sine/pendulum motion.
 * Little pixel birds flying in a circle. By Aircoookie
 */
uint16_t WS2812FX::mode_chunchun(void)
{
  fill(SEGCOLOR(1));
  uint16_t counter = now*(6 + (_segments[_segment_index].speed >> 4));
  uint16_t numBirds = SEGLEN >> 2;
  uint16_t span = _segments[_segment_index].intensity << 8;

  for (uint16_t i = 0; i < numBirds; i++)
  {
    counter -= span/numBirds;
    int megumin = sin16(counter) + 0x8000;
    uint32_t bird = (megumin * SEGLEN) >> 16;
    uint32_t c = color_from_palette((i * 255)/ numBirds, false, true, 0);
    setPixelColor(bird, c);
  }
  return FRAMETIME;
}
#endif // ENABLE_ADVANCED_EFFECTS

#endif // USE_DRIVER
