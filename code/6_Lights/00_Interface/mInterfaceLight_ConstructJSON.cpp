#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway

uint8_t mInterfaceLight::ConstructJSON_Settings(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Add_P(PM_JSON_TYPE, pCONT_set->Settings.light_settings.type);
  
  JsonBuilderI->Add_P(PM_JSON_HUE,pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getHue360());
  JsonBuilderI->Add_P(PM_JSON_SAT,pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getSat255());
  JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB,pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getBrightnessRGB255());

  
  // JBI->Array_AddArray(PM_JSON_RGB_COLOUR_ORDER, hardware_element_colour_order);
  // JBI->Array_AddArray(PM_JSON_RGB_COLOUR_ORDER, hardware_element_c12olour_order);


  JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_CCT,pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getBrightnessCCT255());


  // // JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_PERCENTAGE, animation.transition.pixels_to_update_as_percentage);
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  // JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_NUMBER, pCONT_lAni->SEGMENT_I(0).transition.pixels_to_update_as_number);
  // #endif // USE_MODULE_LIGHTS_ANIMATOR

  return JsonBuilderI->End();

}

/**
 * @brief Ovverview of what is being show regardless of animation/hardware.
 * I should be able to look here and get values needed for openhab/integration with other software
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mInterfaceLight::ConstructJSON_State(uint8_t json_method){

  // Active rgbcct palette used as scene

  char buffer[30];
  
  JsonBuilderI->Start();  

    JBI->Add("ColourPaletteID", pCONT_lAni->SEGMENT_I(0).palette.id);

    JBI->Add("Type", pCONT_set->Settings.light_settings.type);

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JsonBuilderI->Add_P(PM_JSON_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    JsonBuilderI->Add_P(PM_JSON_HUE, pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getHue360());
    JsonBuilderI->Add_P(PM_JSON_SAT, pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getSat255());
    JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getBrightnessRGB255());

    JsonBuilderI->Add_P(PM_JSON_TIME, (uint16_t)round(pCONT_lAni->SEGMENT_I(0).transition.time_ms/1000));
    JsonBuilderI->Add_P(PM_JSON_TIME_MS, pCONT_lAni->SEGMENT_I(0).transition.time_ms);


    #ifdef ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT
    mqtthandler__state__ifchanged.tRateSecs = 1; // force this to be 1 second for this debug message
    JBI->Array_Start("PWM_Channels_Read");
    for (uint8_t i = 0; i < 5; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) 
      {
        JBI->Add(analogRead(pCONT_pins->GetPin(GPIO_PWM1_ID, i)));
      }
    }
    JBI->Array_End();
    #endif  // ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT


  return JsonBuilderI->End();

}


#ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
uint8_t mInterfaceLight::ConstructJSON_Debug_Module_Config(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Level_Start("RgbcctController");
  
    // JsonBuilderI->Level_Start("raw");
    //   JsonBuilderI->Add("R", rgbcct_controller.R); 
    //   JsonBuilderI->Add("G", rgbcct_controller.G); 
    //   JsonBuilderI->Add("B", rgbcct_controller.B); 
    //   JsonBuilderI->Add("WW", rgbcct_controller.WW); 
    //   JsonBuilderI->Add("WC", rgbcct_controller.WC); 
    // JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start("type");
    
    JsonBuilderI->Add("R", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.r); 
    JsonBuilderI->Add("G", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.g); 
    JsonBuilderI->Add("B", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.b); 
    JsonBuilderI->Add("WW", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.w); 
    JsonBuilderI->Add("WC", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.c); 


    JsonBuilderI->Level_End();

    // JsonBuilderI->Add("mPaletteI->active_scene_palette_id",mPaletteI->active_scene_palette_id);

  JsonBuilderI->Level_End();


  if(pCONT_lAni->SEGMENT_I(0).palette.id == mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID)
  {

    JBI->Array_AddArray("encoded", pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map, ARRAY_SIZE(pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map));

  }


  // JsonBuilderI->Level_Start("singlecolour");
  //   JsonBuilderI->Add_P(PM_R", mode_singlecolour.colour.R);
  //   JsonBuilderI->Add_P(PM_G", mode_singlecolour.colour.G);
  //   JsonBuilderI->Add_P(PM_B", mode_singlecolour.colour.B);
  //   JsonBuilderI->Add_P(PM_WW", mode_singlecolour.colour.WW);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  // JsonBuilderI->Level_End();
  // JsonBuilderI->Level_Start("active_rgbcct_colour_p");
  //   JsonBuilderI->Add_P(PM_R", active_rgbcct_colour_p->R);
  //   JsonBuilderI->Add_P(PM_G", active_rgbcct_colour_p->G);
  //   JsonBuilderI->Add_P(PM_B", active_rgbcct_colour_p->B);
  //   JsonBuilderI->Add_P(PM_WW", active_rgbcct_colour_p->WW);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  // JsonBuilderI->Level_End();

  // JsonBuilderI->Array_AddArray("singlecolour", current_color, 5);   

  // JsonBuilderI->Level_Start("fade");
  //   JsonBuilderI->Add("running", fade.running);
  // JsonBuilderI->Level_End();

  return JsonBuilderI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG


#endif
