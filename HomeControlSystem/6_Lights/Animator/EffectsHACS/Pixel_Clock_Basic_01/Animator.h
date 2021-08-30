#pragma once

// void SubTask_Flasher_Animate_Function__Slow_Glow();
// void ConstructJSONBody_Animation_Progress__Slow_Glow();


#ifdef ENABLE_DEVFEATURE_RGB_CLOCK

byte displayMode = 1;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)

byte lastSecond = 0;
uint16_t testnum = 0;
byte startColor = 0;                             // "index" for the palette color used for drawing
// byte displayMode = 0;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
byte colorOffset = 29;//32;                           // default distance between colors on the color palette used between digits/leds (in overlayMode)


void LCDDisplay_colorOverlay() ;
void LCDDisplay_updateDisplay(byte color, byte colorSpacing) ;


#define LED_PWR_LIMIT 750                        // 750mA - Power limit in mA (voltage is set in setup() to 5v)
#define LED_DIGITS 4                             // 4 or 6 digits, can only be an even number as...
// notice 3 less below since I soldered, no single strip
#define LED_PER_DIGITS_STRIP 44//47                  // ...two digits are made out of one piece of led strip with 47 leds...
#define LED_BETWEEN_DIGITS_STRIPS 5              // 5 leds between above strips - and all this gives us LED_COUNT... :D
#define LED_COUNT ( LED_DIGITS / 2 ) * LED_PER_DIGITS_STRIP + ( LED_DIGITS / 3 ) * LED_BETWEEN_DIGITS_STRIPS

byte segGroups[14][2] = {         // 14 segments per strip, each segment has 1-x led(s). So lets assign them in a way we get something similar for both digits
  // right (seen from front) digit. This is which led(s) can be seen in which of the 7 segments (two numbers: First and last led inside the segment, same on TE):
  {  13,  15 },                     // top, a
  {  10, 12 },                     // top right, b
  { 6, 8 },                     // bottom right, c
  { 3, 5 },                     // bottom, d
  { 0, 2 },                     // bottom left, e
  {  16,  18 },                     // top left, f
  {  19,  21 },                     // center, g
  // left (seen from front) digit
  { 35, 37 },                     // top, a
  { 38, 40 },                     // top right, b
  { 22, 24 },                     // bottom right, c
  { 25, 27 },                     // bottom, d
  { 28, 30 },                     // bottom left, e
  { 32, 34 },                     // top left, f
  { 41, 43 }                      // center, g
};


// Using above arrays it's very easy to "talk" to the segments. Simply use 0-6 for the first 7 segments, add 7 (7-13) for the following ones per strip/two digits
byte digits[14][7] = {                    // Lets define 10 numbers (0-9) with 7 segments each, 1 = segment is on, 0 = segment is off
  {   1,   1,   1,   1,   1,   1,   0 },  // 0 -> Show segments a - f, don't show g (center one)
  {   0,   1,   1,   0,   0,   0,   0 },  // 1 -> Show segments b + c (top and bottom right), nothing else
  {   1,   1,   0,   1,   1,   0,   1 },  // 2 -> and so on...
  {   1,   1,   1,   1,   0,   0,   1 },  // 3
  {   0,   1,   1,   0,   0,   1,   1 },  // 4
  {   1,   0,   1,   1,   0,   1,   1 },  // 5
  {   1,   0,   1,   1,   1,   1,   1 },  // 6
  {   1,   1,   1,   0,   0,   0,   0 },  // 7
  {   1,   1,   1,   1,   1,   1,   1 },  // 8
  {   1,   1,   1,   1,   0,   1,   1 },  // 9
  {   0,   0,   0,   1,   1,   1,   1 },  // t -> some letters from here on (index 10-13, so this won't interfere with using digits 0-9 by using index 0-9
  {   0,   0,   0,   0,   1,   0,   1 },  // r
  {   0,   1,   1,   1,   0,   1,   1 },  // y
  {   0,   1,   1,   1,   1,   0,   1 }   // d
};

  void SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01();
  void SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02();
  void SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
  void LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing);
  void LCDDisplay_showDigit(byte digit, byte color, byte pos);
  void LCDDisplay_showSegment(byte segment, byte color, byte segDisplay);
  void LCDDisplay_showDots(byte dots, byte color);
  RgbcctColor ColorFromPalette(uint16_t palette_id, uint8_t index, bool apply_global_brightness = true);
  uint8_t tempcol = 0;
  uint16_t lcd_display_show_number = 0;

  #endif // ENABLE_DEVFEATURE_RGB_CLOCK