#pragma once

#include <stdint.h>

#define COLOUR_ORDER_DISABLED 7
#define COLOUR_ORDER_INIT_DISABLED 0b0111111111111111 //7,7,7,7,7
typedef union {
  uint16_t data; // allows full manipulating using union memory overlap
  struct { 
    /** 3 bits of memory = 9 combinations
     *  0 - channel 0 eg [R----]
     *  1 - channel 1 eg [-G---]
     *  2 - channel 2 eg [--B--]
     *  3 - channel 3 eg [---WC-]
     *  4 - channel 4 eg [----WW]
     *  5 - not used - future led colour (eg another white, amber)
     *  6 - not used - future led colour (eg another white, amber)
     *  7 - DISABLED
     * 
     * default R,G,B,none,none : 0,1,2,7,7 dec : 000 001 010 111 111 R bits : 0000 0101 0111 111R bits and in reverse order R111 1110 1010 0000
     * */
    uint16_t red : 3;
    uint16_t green : 3;
    uint16_t blue : 3;
    uint16_t white_cold : 3;      // Cold will take the FIRST white, as it is most likely as the 4th colour, warm_white will likely only exist when both whites do
    uint16_t white_warm : 3;
    uint16_t reserved : 1; //amber? which cant exist as 1 bit
  };
} 
COLOUR_ORDER_T;

