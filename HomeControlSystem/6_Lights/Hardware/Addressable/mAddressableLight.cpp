#include "mAddressableLight.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE 

// SetPixelHardware
/**
 * colour_hardware will already have colour_order mapped
 * */
void mAddressableLight::SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware, bool flag_replicate_for_total_pixel_length){

//addressible will put the colour into the bus

// pwm will put it into an array (so I can get it back) and set via pwm code

  if(flag_replicate_for_total_pixel_length){ // Replicate for all pixels
    // for(int ii=0;ii<pCONT_iLight->settings.light_size_count;ii++){ // Use strip set? or clear
      pCONT_lAni->stripbus->ClearTo(colour_hardware);
      // pCONT_lAni->stripbus->SetPixelColor(ii,colour_hardware); // Index is default
    // }
  }else{
    pCONT_lAni->stripbus->SetPixelColor(index,colour_hardware); // Index is default
  }

}


RgbcctColor mAddressableLight::GetPixelColorHardware(uint16_t index){

//addressible will put the colour into the bus

// pwm will put it into an array (so I can get it back) and set via pwm code


  return pCONT_lAni->stripbus->GetPixelColor(index);

}

void mAddressableLight::ShowHardware(){

  pCONT_lAni->stripbus->Show();



}


int8_t mAddressableLight::Tasker(uint8_t function){}
void mAddressableLight::parse_JSONCommand(void){}

#endif // USE_DRIVER
