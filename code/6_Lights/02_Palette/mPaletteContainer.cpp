
#include "6_Lights/02_Palette/mPaletteContainer.h"


void mPaletteContainer::LoadPaletteI()
{




//  Serial.println("mPaletteContainer::LoadPaletteI()");
 



}



// /**
//  * @brief Internally load palette into container
//  * 
//  * @return true  If the palette ID was correctly found within container
//  * @return false If the palette ID was not in container and load failed (i.e. should be done by another external method)
//  */
// bool mPaletteContainer::LoadPaletteI(uint8_t palette_id)
// {

//  Serial.println("TRYING TO LOAD"); Serial.flush(); //delay(4000);
 
//   if(
//       (palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID) &&
//       (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID)
//   ){  
//     mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);    
//     pData.assign(ptr->data, ptr->data + ptr->data_length);
//     return true; // load succesful
//   }
//   return false; // Out of range, not loaded

// }


// mPaletteContainer::mPaletteContainer(){

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

//     // pCONT_lAni->LoadPalette(0, pCONT_lAni->getCurrSegmentId());



// };