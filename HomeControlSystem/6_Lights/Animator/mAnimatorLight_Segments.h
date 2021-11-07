// #ifndef _AnimatorDALIGHTNEOPIXEL7_H
// #define _AnimatorDALIGHTNEOPIXEL7_H 7.0

// #include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR


//     #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//   /**
//    * Create a duplicate of this in HACS, and slowly move HACS to follow this basic layout ("layout" = struct of what animation runs in that section)
//    * */

//   /**
//    * how a section (single animation) is configured -- my HACS needs to change to conform to this
//    * */
  
// // options
// // bit    7: segment is in transition mode
// // bits 4-6: TBD
// // bit    3: mirror effect within segment
// // bit    2: segment is on
// // bit    1: reverse segment
// // bit    0: segment is selected
// #define NO_OPTIONS2   (uint8_t)0x00
// #define TRANSITIONAL2 (uint8_t)0x80
// #define MIRROR2       (uint8_t)0x08
// #define SEGMENT_ON2   (uint8_t)0x04
// #define REVERSE2      (uint8_t)0x02
// #define SELECTED2     (uint8_t)0x01
// // #define IS_TRANSITIONAL2 ((_segments[_segment_index].options & TRANSITIONAL2) == TRANSITIONAL2)
// // #define IS_MIRROR2       ((_segments[_segment_index].options & MIRROR2      ) == MIRROR2      )
// // #define IS_SEGMENT_ON2   ((_segments[_segment_index].options & SEGMENT_ON2  ) == SEGMENT_ON2  )
// // #define IS_REVERSE2      ((_segments[_segment_index].options & REVERSE2     ) == REVERSE2     )
// // #define IS_SELECTED2     ((_segments[_segment_index].options & SELECTED2    ) == SELECTED2    )

// #define DEFAULT_BRIGHTNESS2 (uint8_t)127
// #define DEFAULT_MODE2       (uint8_t)0
// #define DEFAULT_SPEED2      (uint8_t)128
// #define DEFAULT_COLOR2      (uint32_t)0xFFAA00

// // #define NUM_COLORS       3 /* number of colors per segment */
// #define SEGCOLOR2(x)      _segments[_segment_index].colors[x]

//     // #define SEGCOLOR(x)      gamma32(_segments[_segment_index].colors[x])

// // #define SEGLEN           _virtualSegmentLength
// // #define SPEED_FORMULA_L  5 + (50*(255 - _segments[_segment_index].speed))/_virtualSegmentLength
// // #define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))
// /**
//  * First animations to be ported to new method, they shall both be displayed on string together
//  * Slow Glow (50%)
//  * Solid (50%) with random colour changes
//  * */

//     void showWLED(void);

//     void fill(uint32_t c);


//     uint16_t mode_static(void);
//     uint16_t mode_static_pattern();
//     uint16_t mode_tri_static_pattern();
//     uint16_t color_wipe(bool rev, bool useRandomColors);
//     uint16_t mode_color_wipe(void);
//     uint16_t mode_color_wipe_random(void);
//     uint16_t mode_color_sweep(void);
//     uint16_t mode_color_sweep_random(void);

//     #define NUM_COLORS2       3 /* number of colors per segment */



    
//     typedef struct Segment { // 24 bytes
//       uint16_t start_pixel_index;
//       uint16_t stop_pixel_index; //segment invalid if stop == 0
//       uint8_t speed; // rate/blend time
//       uint8_t intensity; // amount of animations in it
//       uint8_t palette;
//       uint8_t mode; //animation_id
//       uint8_t options; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
//       uint8_t grouping;
//       uint8_t spacing;
//       uint8_t opacity;
//       uint32_t colors[NUM_COLORS2];
//       void setOption(uint8_t n, bool val)
//       {
//         if (val) {
//           options |= 0x01 << n;
//         } else
//         {
//           options &= ~(0x01 << n);
//         }
//       }
//       bool getOption(uint8_t n)
//       {
//         return ((options >> n) & 0x01);
//       }
//       bool isSelected()
//       {
//         return getOption(0);
//       }
//       bool isActive()
//       {
//         return stop_pixel_index > start_pixel_index;
//       }
//       uint16_t length()
//       {
//         return stop_pixel_index - start_pixel_index;
//       }
//       uint16_t groupLength()
//       {
//         return grouping + spacing;
//       }
//       uint16_t virtualLength()
//       {
//         uint16_t groupLen = groupLength();
//         uint16_t vLength = (length() + groupLen - 1) / groupLen;
//         if (options & MIRROR2)
//           vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
//         return vLength;
//       }
//     } segment;
//     uint8_t _segment_index = 0;
//     uint8_t _segment_index_palette_last = 99;
//     segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
//       // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
//       { 0, 7, DEFAULT_SPEED2, 128, 0, DEFAULT_MODE2, NO_OPTIONS2, 1, 0, 255, {DEFAULT_COLOR2}}
//     };

//     /**
//      * Runtime data for that segment, ie holds animation data
//      * */
//     typedef struct Segment_runtime { // 28 bytes
//       unsigned long next_time;
//       uint32_t step;
//       uint32_t call;
//       uint16_t aux0;
//       uint16_t aux1;
//       byte* data = nullptr; // note: how all my previous hardcorded structs will need to conform to this
//       bool allocateData(uint16_t len){
//         if (data && _dataLen == len) return true; //already allocated
//         deallocateData();
//         if (_usedSegmentData + len > MAX_SEGMENT_DATA) return false; //not enough memory
//         data = new (std::nothrow) byte[len];
//         if (!data) return false; //allocation failed
//         _usedSegmentData += len;
//         _dataLen = len;
//         memset(data, 0, len);
//         return true;
//       }
//       void deallocateData(){
//         delete[] data;
//         data = nullptr;
//         _usedSegmentData -= _dataLen;
//         _dataLen = 0;
//       }
//       void reset(){next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; deallocateData();}
//       private:
//         uint16_t _dataLen = 0;
//     }segment_runtime;
      
//     segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element
    
    
//     uint32_t _lastPaletteChange = 0;
//     uint32_t _lastShow = 0;
    
//     uint32_t timebase;
//     uint32_t now;
//     static uint16_t _usedSegmentData;
//     bool
//       _useRgbw = false,
//       _skipFirstMode,
//       _triggered;
//     uint8_t
//       mainSegment = 0,
//       rgbwMode = 3,
//       paletteFade = 0,
//       paletteBlend = 0;

//     uint16_t _length, _lengthRaw, _virtualSegmentLength;
      

//     void resetSegments();
//     void handle_palette(void);
//     CRGBPalette16 currentPalette;
//     CRGBPalette16 targetPalette;

//     #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS



// #endif

// #endif
