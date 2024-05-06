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

	/**
	 * @brief When set, this will be used to map the length of the palette to the known values stored here
	 * ie [10.0,15.0,20.0] would be 0 to 255 from the palette
	 * Using the float values as nonlinear, will let me get non-linear palette
	 * ie [10, 14,15,16, 20] would be 255/14 255/15 etc, so the centre region would span a wider colour range (compression/expansion)
	 * 
	 * 
	 * the new encoded heatmap grads should be in here. 
	 * This would limit one gradient per segment that is encoded, which should be enough to show on that segment (anything else can use presets)
	 * 
	 */
	std::vector<float> mapping_values;
	std::vector<uint8_t> pData;

	struct CRGB16Palette16_PALETTE
	{
		CRGBPalette16 data;                 
		std::vector<uint8_t> encoded_index;
	};
	CRGB16Palette16_PALETTE CRGB16Palette16_Palette;

    void deallocateData()
    {	// Clear vector by replacing it with an empty vector. The destructor for vector will properly remove the previous instance
		pData = std::vector<uint8_t>();
		CRGB16Palette16_Palette.encoded_index = std::vector<uint8_t>(); 
    };

};

#endif // _M_PALETTE_CONTAINER_H