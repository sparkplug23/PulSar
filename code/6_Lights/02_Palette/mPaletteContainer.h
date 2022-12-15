#pragma once


#include "stdint.h"

// uint16_t mPaletteContainer::_usedSegmentData = 0;

#ifdef ENABLE_DEVFEATURE_PALETTECONTAINER

class mPaletteContainer
{
private:
    /* data */
    uint8_t* data = nullptr;
    uint16_t _dataLen = 0;
public:
    mPaletteContainer(uint16_t buffer_size){-
			allocateData(buffer_size);
		};
    ~mPaletteContainer(){};

    bool allocateData(uint16_t len)
    {
			if (data && _dataLen == len) return true; //already allocated
			deallocateData();

			// if possible use SPI RAM on ESP32
			#if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
			if (psramFound())
					data = (uint8_t*) ps_malloc(len);
			else
			#endif
					data = (uint8_t*) malloc(len);
					
			if (!data) return false; //allocation failed
			// _usedSegmentData += len;
			_dataLen = len;
			memset(data, 0, len);
			return true;
    }
    void deallocateData()
    {
			// delete[] data;
			free(data);
			data = nullptr;
			// _usedSegmentData -= _dataLen;
			_dataLen = 0;
    }

		uint8_t* GetDataPtr()
		{
			return data;
		}
		uint16_t GetDataLength()
		{
			return _dataLen;
		}




};

#endif // ENABLE_DEVFEATURE_PALETTECONTAINER