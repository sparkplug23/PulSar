#ifndef _M_PALETTE_LOADED_H
#define _M_PALETTE_LOADED_H

#include "stdint.h"

#include "6_Lights/02_Palette/mPalette.h"

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/00_Interface/FastLED/FastLED.h"

#include <vector>

class mPaletteLoaded
{
	public:
	
    mPaletteLoaded(){};

    ~mPaletteLoaded(){
		deallocateData();
	};

	uint8_t loaded_palette_id = 0;

	std::vector<uint8_t> pData;

	struct CRGB16Palette16_PALETTE
	{
		CRGBPalette16 data;
		std::vector<uint8_t> encoded_index;
	};
	CRGB16Palette16_PALETTE CRGB16Palette16_Palette;

    void deallocateData()
    {
		// Clear vector by replacing it with an empty vector. The destructor for vector will properly remove the previous instance
		pData = std::vector<uint8_t>();
		CRGB16Palette16_Palette.encoded_index = std::vector<uint8_t>(); 
    };

};

#endif // _M_PALETTE_CONTAINER_H