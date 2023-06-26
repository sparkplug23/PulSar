#ifndef _M_PALETTE_CONTAINER_H
#define _M_PALETTE_CONTAINER_H

#include "stdint.h"

#include "6_Lights/02_Palette/mPalette.h"

#include <vector>

// uint16_t mPaletteContainer::_usedSegmentData = 0;

// #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

class mPaletteContainer
{
	public:
// private:
    /* data */
    // uint8_t* data = nullptr;
    // uint16_t _dataLen = 0;
	
	uint8_t palette_id = 0;

// public:
    mPaletteContainer(uint16_t buffer_size = 0)
	{

		// allocateData(buffer_size);
		// Serial.println(buffer_size);

		// Init_LoadedPaletted();
		LoadPaletteI();
	};
    mPaletteContainer(){};

    ~mPaletteContainer(){};

	std::vector<uint8_t> pData;

	void LoadPaletteI();


    bool allocateData(uint16_t len)
    {
		pData.clear();
		pData.push_back(0);
		pData.push_back(1);
		pData.push_back(2);

		return true;
    };
    void deallocateData()
    {
		// Clear vector by replacing it with an empty vector. The destructor for vector will properly remove the previous instance
		pData = std::vector<uint8_t>();
    };



};

// #endif // ENABLE_DEVFEATURE_PALETTECONTAINER

#endif // _M_PALETTE_CONTAINER_H