#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

// Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
void mAnimatorLight::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
#ifdef ENABLE_EFFECTS_ANIMATE_FUNCTION_FADE_GRADIENT

  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  // Apply green gradient, brightest at start

  uint16_t start = pCONT_iLight->settings.light_size_count/2;
  uint16_t end = pCONT_iLight->settings.light_size_count; 
  RgbTypeColor colour_gradient = HsbColor(
                                          pCONT_iLight->HueN2F(120),
                                          pCONT_iLight->SatN2F(100),
                                          pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
                                          );
  RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
  HsbColor colour_random_adjusted = HsbColor(RgbColor(0));
  uint8_t gradient_end_percentage = 75;
  uint16_t strip_size_gradient = pCONT_iLight->settings.light_size_count*(gradient_end_percentage/100.0f);
  uint16_t strip_size_single   = pCONT_iLight->settings.light_size_count*(75/100.0f);
  
  start = 0;
  end = pCONT_iLight->settings.light_size_count;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
  }
  
  //0 to 75% 
  start = 0;
  end = map(75,0,100,0,pCONT_iLight->settings.light_size_count);
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
    animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
    animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
  }

  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
  uint8_t desired_pixel;
  
  // 25 to 100%
  start = map(25,0,100,0,pCONT_iLight->settings.light_size_count);
  end = pCONT_iLight->settings.light_size_count;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    desired_pixel = random(0,pixels-1);
    colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);
    if((ledout.index%3)==0){
      colour_random_adjusted = RgbTypeColor(colour_random);
      colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
      animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
    }
  }

  #endif

}

#endif
