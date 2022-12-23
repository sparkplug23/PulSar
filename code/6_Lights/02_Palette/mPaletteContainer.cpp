
#include "6_Lights/02_Palette/aPaletteContainer.h"

void aPaletteContainer::test()
{

    
	// Serial.println(mPalette::PALETTELIST_VARIABLE_RGBCCT_LENGTH__ID);


}

void aPaletteContainer::LoadPaletteI()
{




 Serial.println("TRYING TO LOAD"); Serial.flush(); //delay(4000);

//  delay(10000);

 
  if(
      (palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID) &&
      (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID)
  ){  
    mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);    
    pData.assign(ptr->data, ptr->data + ptr->data_length);
  }







}



// aPaletteContainer::aPaletteContainer(){

//     // Should palette id be moved into this, or at least copy of active ID?

//     Serial.println("LOADING DEFAULT ===================================================="); delay(3000);

//     mPalette::GetInstance()->test_value = 0;

//     /**
//      * @brief Set default
//      **/
//     // if(
//     // 	(palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID) &&
//     // 	(palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID)
//     // ){  
//     // 	mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);    
//     // 	pData.assign(ptr->data, ptr->data + ptr->data_length);
//     // }
    
//     pData.push_back(0); //minimum test, HSB assumed
//     pData.push_back(1);
//     pData.push_back(2);

//     // pCONT_lAni->loadPalette_Michael(0, pCONT_lAni->strip->getCurrSegmentId());



// };