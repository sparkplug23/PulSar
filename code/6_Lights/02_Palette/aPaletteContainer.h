#ifndef _M_PALETTE_CONTAINER_H
#define _M_PALETTE_CONTAINER_H

#include "stdint.h"

#include "6_Lights/02_Palette/mPalette.h"

#include <vector>

// uint16_t aPaletteContainer::_usedSegmentData = 0;

// #ifdef ENABLE_DEVFEATURE_NEWPALETTE_CONTAINER

class aPaletteContainer
{
	public:
// private:
    /* data */
    // uint8_t* data = nullptr;
    // uint16_t _dataLen = 0;
	
	uint8_t palette_id = 0;

// public:
    aPaletteContainer(uint16_t buffer_size)
	{

		// allocateData(buffer_size);
		Serial.println(buffer_size);

		test();

		// Init_LoadedPaletted();
		LoadPaletteI();
	};
    aPaletteContainer(){};

    ~aPaletteContainer(){};

	std::vector<uint8_t> pData;

	void LoadPaletteI();



    // bool allocateData(uint16_t len)
    // {
	// 	pData.clear();
	// 	pData.push_back(0);
	// 	pData.push_back(1);

	// 	return true;
    // }

	void test();

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



	// uint8_t* GetDataPtr()
	// {
	// 	return data;
	// };
	// uint16_t GetDataLength()
	// {
	// 	return _dataLen;
	// };

};

// #endif // ENABLE_DEVFEATURE_PALETTECONTAINER

#endif // _M_PALETTE_CONTAINER_H