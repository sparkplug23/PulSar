#pragma once

    // // Bits 15,14,13,12
    // #define PALETTE_ENCODING_INCLUDES_R       0x8000 // 1000 0000 0000 0000
    // #define PALETTE_ENCODING_INCLUDES_G       0x4000 // 0100 0000 0000 0000
    // #define PALETTE_ENCODING_INCLUDES_B       0x2000 // 0010 0000 0000 0000
    // #define PALETTE_ENCODING_INCLUDES_W1      0x2000 // 0001 0000 0000 0000   // If two whites are present, always assume W1=cold, and W2=warm
    // // Bits 11,10,9,8
    // #define PALETTE_ENCODING_INCLUDES_W2      0x0800 // 0000 1000 0000 0000
    // #define PALETTE_ENCODING_TYPE_HSB_IDS     0x0400 // 0000 0100 0000 0000 
    // //
    // //
    // // Bits 7,6,5,4
    // #define PALETTE_ENCODING_INDEX_EXACT                 0x0080 // 0000 0000 1000 0000
    // #define PALETTE_ENCODING_INDEX_SCALED                0x0040 // 0000 0000 0000 0000
    // #define PALETTE_ENCODING_INDEX_AS_VALUE_EXACT        0x0020 // 0000 0000 0010 0000
    // #define PALETTE_ENCODING_INDEX_AS_VALUE_SCALED100    0x0010 // 0000 0000 0001 0000   // If two whites are present, always assume W1=cold, and W2=warm
    // // Bits 3,2,1,0 - Addition of WLED types CRGB16, CRGB256 etc
    // #define PALETTE_ENCODING_TYPE_CRGBPalette16          0x8000 // 0000 0000 0000 1000
    // #define PALETTE_ENCODING_TYPE_CRGBPalette256         0x0080 // 0000 0000 0000 0100
    // #define PALETTE_ENCODING_TYPE_CRGBPaletteUNSET          0x2000 // 0000 0000 0000 0010
    // #define PALETTE_ENCODING_TYPE_CRGBPaletteUNSET          0x0000 // 0000 0000 0000 0001   // If two whites are present, always assume W1=cold, and W2=warm
    


    // // Group setting
    // #define PALETTE_ENCODING_INCLUDES_RGB     0xE000 // 1110 0000 0000 0000
    // #define PALETTE_ENCODING_INCLUDES_RGBW    0xF000 // 1111 0000 0000 0000
    // #define PALETTE_ENCODING_INCLUDES_RGBCCT  0xF800 // 1111 1000 0000 0000



//////////////////////////////





    // Bits 3,2,1,0
    #define PALETTE_ENCODING_INCLUDES_R       0x0001        // 0000 0000 0000 0001
    #define PALETTE_ENCODING_INCLUDES_G       0x0002        // 0000 0000 0000 0010
    #define PALETTE_ENCODING_INCLUDES_B       0x0004        // 0000 0000 0000 0100
    #define PALETTE_ENCODING_INCLUDES_W1      0x0008        // 0000 0000 0000 1000   // If two whites are present, always assume W1=cold, and W2=warm
    // Bits 7,6,5,4
    #define PALETTE_ENCODING_INCLUDES_W2       0x0010       // 0000 0000 0001 0000   // If two whites are present, always assume W1=cold, and W2=warm
    #define PALETTE_ENCODING_TYPE_RESERVED1    0x0040       // 0000 0000 0010 0000 
    #define PALETTE_ENCODING_TYPE_RESERVED2    0x0080       // 0000 0000 0100 0000 
    #define PALETTE_ENCODING_TYPE_RESERVED2    0x0080       // 0000 0000 0000 0000 
    // Bits 11,10,9,8
    #define PALETTE_ENCODING_INDEX_EXACT                 0x0100 // 0000 0001 0000 0000
    #define PALETTE_ENCODING_INDEX_SCALED                0x0200 // 0000 0010 0000 0000
    #define PALETTE_ENCODING_INDEX_AS_VALUE_EXACT        0x0400 // 0000 0100 0000 0000
    #define PALETTE_ENCODING_INDEX_AS_VALUE_SCALED100    0x0800 // 0000 1000 0000 0000   
    // Bits 15,14,13,12
    #define PALETTE_ENCODING_TYPE_HSBID                  0x0020 // 0001 0000 0000 0000 
    #define PALETTE_ENCODING_TYPE_CRGBPalette16          0x1000 // 0010 0000 0000 0000
    #define PALETTE_ENCODING_TYPE_CRGBPalette256         0x2000 // 0100 0000 0000 0000
    #define PALETTE_ENCODING_TYPE_RESERVED3              0x4000 // 1000 0000 0000 0000
    

    #define PALETTE_ENCODING_INDEX_NONE              0x0000


    // Group setting
    #define PALETTE_ENCODING_INCLUDES_RGB     0x0007 // 0000 0000 0000 0111
    #define PALETTE_ENCODING_INCLUDES_RGBW    0x000F // 0000 0000 0000 1111
    #define PALETTE_ENCODING_INCLUDES_RGBCCT  0x001F // 0000 0000 0001 1111

    #define PALETTE_ENCODING_INCLUDES_RGBCCT_NO_INDEX  0x001F // 0000 0000 0001 1111

    #define PALETTE_ENCODING_TYPE_HSBID_WITHINDEX_GRADIENT       (PALETTE_ENCODING_TYPE_HSBID | PALETTE_ENCODING_INDEX_SCALED) // temp name to align with enum before