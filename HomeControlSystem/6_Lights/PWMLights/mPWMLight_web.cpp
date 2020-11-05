#include "mPWMLight.h"

#ifdef USE_MODULE_LIGHTS_PWM
// #ifdef USE_MODULE_CORE_WEBSERVER

int8_t mPWMLight::Tasker_Web(uint8_t function){


  switch(function){    
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();      
    break;
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
      JsonBuilderI->Add(WEB_HANDLE_LIVEPIXELS_SHARED_JSON,1000);
    break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:{
      WebAppend_Root_ControlUI();
    }break;
    case FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON:
      WebAppend_JSON_RootPage_LiveviewPixels();      
    break;
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      pCONT_web->WebAppend_Root_Draw_Table_dList(2, "rgb_table", kTitle_TableTitles_Root_PWM);
    break; 
//     // case FUNC_WEB_ADD_HANDLER:
//     //   WebPage_Root_AddHandlers();
//     // break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
  }

  return 0;

}


void mPWMLight::WebAppend_Root_ControlUI(){


  char colour_ctr[8];
  char scolor[8];
  char pwm_channel_chr[12];
  char stemp[15];

  // BufferWriterI->Append_P(PSTR("<div><b>PWM Testing</b></div>"));
  // for(int i=0;i<5;i++){
  //     // BufferWriterI->Append_P(PSTR("<div> PWM%d <span class='mil_pwm_%d'>%d (%d-%d)</span></div>"),i,i,pwm_channel_test[i],0,1023);

  //     pCONT_sup->GetTextIndexed_P(colour_ctr, sizeof(colour_ctr), i, kListPWM_TestColours);
  //     sprintf(pwm_channel_chr,"pwm%02d\0",i);

  //     //Red, Green, blue, cold white, warm white
  //     BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
  //       "pwm",               // c - Unique HTML id
  //       PSTR("#121212"), colour_ctr,//PSTR("#ff0000"),//"#fff",    // Black to White
  //       i,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
  //       0, 1023,  // Range 0/1 to 100%
  //       current_color[i],
  //       pwm_channel_chr
  //     ); 
  // }

  

  // RgbColor rgb = RgbColor(pCONT_iLight->current_color[0],pCONT_iLight->current_color[1],pCONT_iLight->current_color[2]);
  // HsbColor hsb = HsbColor(rgb);

  // uint16_t hue = 0;
  // uint8_t  sat = 0;
  // uint8_t  brt = 0;
  // pCONT_iLight->getHSB(&hue, &sat, &brt);

  // BufferWriterI->Append_P(PSTR("<tr><td><b>Colour</b></td></tr>"));//GetPaletteFriendlyName(),GetPixelsInMap(palettelist.ptr));
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT2,  // Hue
  //   "pwm_hue",             // b - Unique HTML id
  //   "#800", "#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800",  // Hue colors
  //   2,               // sl2 - Unique range HTML id - Used as source for Saturation end color
  //   0, 359,          // Range valid Hue
  //   hue,
  //   "pwm_hue", 0
  // );         // h0 - Value id

  // uint8_t dcolor = mSupport::changeUIntScale(100, 0, 100, 0, 255);
  // snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
  // snprintf_P(stemp, sizeof(stemp), PSTR("#%02X%02X%02X"), rgb.R, rgb.G, rgb.B);  // Saturation end color
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT2,  // Saturation
  //   "pwm_sat",             // s - Unique HTML id related to eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';
  //   scolor, stemp,   // Brightness to max current color
  //   3,               // sl3 - Unique range HTML id - Not used
  //   0, 100,          // Range 0 to 100%
  //   sat,
  //   "pwm_sat", 0
  // );         // n0 - Value id

  // // BufferWriterI->Append_P(PSTR("<div> Brightness <span class='hsb_brt_ttl'>%d %%</span></div>"),100);// BrtF2N(animation.brightness))
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
  //   "pwm_brt",               // c - Unique HTML id
  //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
  //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
  //   0, 100,  // Range 0/1 to 100%
  //   pCONT_iLight->_briRGB,
  //   "pwm_brt"
  // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));

  // BufferWriterI->Append_P(PSTR("<div> CCT Controls</div>"));// BrtF2N(animation.brightness))
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Cold Warm
  //   "pwm_cct",             // a - Unique HTML id
  //   "#fff", "#ff0",  // White to Yellow
  //   1,               // sl1
  //   153, 500,        // Range color temperature
  //   pCONT_iLight->LightGetColorTemp(),
  //   "pwm_cct"
  // );         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));

  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
  //   "pwm_cbrt",               // c - Unique HTML id
  //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
  //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
  //   0, 100,  // Range 0/1 to 100%
  //   pCONT_iLight->_briCT,
  //   "pwm_cbrt"
  // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));

  // BufferWriterI->Append_P(PSTR("%s"),PSTR("{t}<tr>"));
  // BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE, 
  //                                 36, PSTR("pwm_toggle"), 1, PSTR("PWM Power Toggle"), ""
  //                             );
  // BufferWriterI->Append_P(PSTR("%s"),PSTR("{t}<tr>"));
  // BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE, 
  //                                 36, PSTR("cct_rgb_linked"), 1, PSTR("RGB CCT Linked Toggle"), ""
  //                             );
  // BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));
  
} //END function


// void mPWMLight::WebAppend_Root_ControlUI(){

//   // char buffer[50];
//   // char scolor[8];
//   // BufferWriterI->Append_P(PSTR("<tr><td><b>Scene Colour</b></td></tr>"));//GetPaletteFriendlyName(),GetPixelsInMap(palettelist.ptr));

//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT2,  // Hue
//   //   "scn_hue",             // b - Unique HTML id
//   //   "#800", "#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800",  // Hue colors
//   //   2,               // sl2 - Unique range HTML id - Used as source for Saturation end color
//   //   1, 359,          // Range valid Hue
//   //   HueF2N(scene.colour.H),
//   //   "scn_hue", 0
//   // );         // h0 - Value id

//   // uint8_t dcolor = changeUIntScale(BrtF2N(scene.colour.B), 0, 100, 0, 255);
//   // snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
//   // //   uint8_t red, green, blue;
//   // //   LightHsToRgb(hue, 255, &red, &green, &blue);
//   // RgbColor rgb = HsbColor(scene.colour.H,1,1);
//   // snprintf_P(buffer, sizeof(buffer), PSTR("#%02X%02X%02X"), rgb.R,rgb.G,rgb.B);  // Saturation end color

//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT2,  // Saturation
//   //   "scn_sat",             // s - Unique HTML id related to eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';
//   //   scolor, buffer,   // Brightness to max current color
//   //   3,               // sl3 - Unique range HTML id - Not used
//   //   0, 100,          // Range 0 to 100%
//   //   changeUIntScale(SatF2N(scene.colour.S), 0, 255, 0, 100),
//   //   "scn_sat", 0
//   // );         // n0 - Value id

//   // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//   // // Can I use "ORANGE" with RGB as warm white? 
//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
//   //   "scn_clr_temp",             // a - Unique HTML id
//   //   "#fff", "#ff0",  // White to Yellow
//   //   1,               // sl1
//   //   153, 500,        // Range color temperature
//   //   200,//LightGetColorTemp(),
//   //   't', 0
//   // );         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));

//   // BufferWriterI->Append_P(PSTR("<div> Manual White Channel </div>"));
//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//   //   "scn_w_temp",               // c - Unique HTML id
//   //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
//   //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//   //   0, 100,  // Range 0/1 to 100%
//   //   BrtF2N(scene.colourW),
//   //   WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER
//   // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));
//   // #endif

//   // BufferWriterI->Append_P(PSTR("<div> Brightness <span class='hsb_brt_ttl'>%d %%</span></div>"), BrtF2N(animation.brightness));
//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//   //   "brt",               // c - Unique HTML id
//   //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
//   //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//   //   0, 100,  // Range 0/1 to 100%
//   //   BrtF2N(animation.brightness),
//   //   WEB_HANDLE_ANIMATION_BRIGHTNESS_SLIDER
//   // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));


//   // BufferWriterI->Append_P(PSTR("%s"),PSTR("{t}<tr>"));
//   // BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
//   //                           100/2, 
//   //                           "mainbut", //class type of button
//   //                           "rgb_toggle", 
//   //                           DEVICE_CONTROL_BUTTON_TOGGLE_ID, 
//   //                           PSTR("Light Power  "),
//   //                           animation.brightness > 0 ? "ON" : "OFF"
//   //                         );

//   // BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
//   //                           100/2, 
//   //                           "mainbut", //class type of button
//   //                           "animation_toggle", 
//   //                           DEVICE_CONTROL_BUTTON_TOGGLE_ID, 
//   //                           PSTR("Animation  "),
//   //                           animation.flags.fEnable_Animation ? "Enabled" : "Paused"
//   //                         );
//   // BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));

//   // // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Animation Mode Select"),"");
//   // char mode_select_ctr[10]; 
//   // BufferWriterI->Append_P(PSTR("%s"),PSTR("{t}<tr>"));
//   // uint8_t option_mode_id = 0;
//   // uint8_t option_mode_id_count = 4;
//   // #ifndef USE_TASK_RGBLIGHTING_NOTIFICATIONS
//   //   option_mode_id_count = 3;
//   // #endif

//   // for(uint8_t animation_mode_select_id=0;animation_mode_select_id<option_mode_id_count;animation_mode_select_id++){
//   //   memset(mode_select_ctr,0,sizeof(mode_select_ctr));
//   //   //send actual wanted mode_ids
//   //   switch(animation_mode_select_id){
//   //     case 0: option_mode_id = ANIMATION_MODE_FLASHER_ID; break;
//   //     case 1: option_mode_id = ANIMATION_MODE_SCENE_ID; break;
//   //     #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
//   //     case 2: option_mode_id = ANIMATION_MODE_NOTIFICATIONS_ID; break;
//   //     #endif
//   //     default: option_mode_id = ANIMATION_MODE_NONE_ID; break;
//   //   }

//   //   sprintf(mode_select_ctr,PSTR("animod%02d"),option_mode_id);
//   //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
//   //                                 100/option_mode_id_count, 
//   //                                 "animod",
//   //                                 mode_select_ctr, 
//   //                                 DEVICE_CONTROL_BUTTON_ON_ID, 
//   //                                 GetAnimationModeNameByID(option_mode_id, buffer), ""
//   //                               );
//   // }
//   // BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));
  
//   // pCONT_web->WebAppend_Button(PM_BUTTON_NAME_RGB_CONTROLS_CTR, PM_WEB_HANDLE_RGB_CONTROLS);
  

// }


//append to internal buffer if any root messages table
void mPWMLight::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("rgb_table");// Class name
  for(int row=0;row<2;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        default:
        case 0: JsonBuilderI->Add("ih","Unset"); break;
        case 1: JsonBuilderI->Add_FP("ih",PSTR("\"%d secs\""), pCONT_iLight->auto_time_off_secs); break;

        
        // case 1:
        //   JsonBuilderI->Add_FP("ih",PSTR("\"%d/%d (secs)\""), 
        //     animation.transition.rate_ms.val/1000, 
        //     animation.transition.time_ms.val/1000
        //   );
        // break;
        // case 2: 
        //   JsonBuilderI->Add_FP("ih",PSTR("\"%d%% [#%d]\""),
        //     animation.transition.pixels_to_update_as_percentage.val, 
        //     GetPixelsToUpdateAsNumberFromPercentage(animation.transition.pixels_to_update_as_percentage.val)
        //   );
        // break;
        // case 3: JsonBuilderI->Add("ih",GetTransitionOrderName(buffer)); break;
        // case 4: JsonBuilderI->Add("ih",GetAnimationModeName(buffer));   break;
        // case 5: JsonBuilderI->Add("ih",GetFlasherFunctionName(buffer)); break;
        // case 6: JsonBuilderI->Add_FP("ih",PSTR("\"%d (%s)\""), (int)power_rating.power,"W"); break;
        // case 7:
        //   if(!animation.auto_time_off_secs){ //off
        //     JsonBuilderI->Add("ih","Unset");
        //   }else{
        //     JsonBuilderI->Add_FP("ih",PSTR("\"%d (%s)\""),
        //       animation.auto_time_off_secs,"secs"
        //     );
        //   }
        // break;
      } //switch
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  

  // char colour_button[8];
  // char button_ids[] = {ANIMATION_MODE_FLASHER_ID, ANIMATION_MODE_SCENE_ID, 
  // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  // ANIMATION_MODE_NOTIFICATIONS_ID, 
  // #endif
  // ANIMATION_MODE_NONE_ID};
  
  // JsonBuilderI->Array_Start("animod");// Class name
  // for(int row=0;row<sizeof(button_ids);row++){
  //   if(button_ids[row] == animation.mode_id){
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON_HIGHLIGHT); //selected
  //   }else{
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON); //NOT selected
  //   }        
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->Add("id",row);
  //     JsonBuilderI->Add("bc",colour_button);
  //   JsonBuilderI->Level_End();
  // }  
  // JsonBuilderI->Array_End();

}




// /*******************************************************************************************************************
// ********************************************************************************************************************
// ************ START OF WEBPAGE DEFINITIONS ********************************************************************************************
// ********************************************************************************************************************
// ********************************************************************************************************************/


// // uint32_t mPWMLight::WebColorFromColourMap(uint8_t i)
// // {
// //   RgbColor rgb = RgbColor(GetHsbColour(i));
// //   uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
// //   return tcolor;
// // }
// // uint32_t mPWMLight::WebColorFromColourType(RgbColor rgb)
// // {
// //   uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
// //   return tcolor;
// // }
// // uint32_t mPWMLight::WebColorFromColourType(HsbColor hsb)
// // {
// //   RgbColor rgb = RgbColor(hsb);
// //   uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
// //   return tcolor;
// // }
// // uint32_t mPWMLight::WebColorFromColourType(RgbwColor rgbw)
// // {
// //   uint32_t tcolor = (rgbw.R << 16) | (rgbw.G << 8) | rgbw.B;
// //   return tcolor;
// // }

void mPWMLight::WebCommand_Parse(void)
{
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "mPWMLight::WebCommand_Parse"));

//   char tmp[100];

//   //selectorlist.amount = 0;
//   uint8_t  arg_value = 0;
// //debug error
// // Serial.println();
//  if(pCONT_web->request_web_command == nullptr){
// Serial.println("nullptr"); 
// return; }
// // Serial.println();

//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

//   // check palette selector
//   sprintf_P(arg_ctr,PSTR("rgb_toggle\0"));
//   if (pCONT_web->request_web_command->hasParam("rgb_toggle")) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);

//     switch(fAnyLEDsOnOffCount){
//       case 0:
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
//         memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state
//         SetAnimationProfile(ANIMATION_PROFILE_TURN_ON_ID);        // Add this as "SAVE" state then "LOAD" state
//         fLEDStripOnOff = true;
//       break;
//       default:
//       case 1:
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
//         memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
//         SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
//         fLEDStripOnOff = false;
//       break;
//     }

//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   }

//   sprintf_P(arg_ctr,PSTR(WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     scene.colourW = BrtN2F(arg_value);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "scene.colourW=%d"),arg_value);

//     if(settings.flags.EnableSceneModeWithSliders){
//       animation.mode_id = ANIMATION_MODE_SCENE_ID;
//       scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
//     }

//     SetRefreshLEDs();
//   }



  
//   sprintf_P(arg_ctr,PSTR("scn_hue0"));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     uint16_t hue = (!strlen(tmp)) ? 0 : atoi(tmp);
//     scene.colour.H = HueN2F(hue);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     animation.mode_id = ANIMATION_MODE_SCENE_ID;
//     scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
        
//     SetRefreshLEDs();
//   }
//   sprintf_P(arg_ctr,PSTR("scn_sat0"));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     scene.colour.S = SatN2F(arg_value);
//     animation.mode_id = ANIMATION_MODE_SCENE_ID;
//     scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     SetRefreshLEDs();
//   }


  
//   sprintf_P(arg_ctr,PSTR(WEB_HANDLE_ANIMATION_BRIGHTNESS_SLIDER));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     animation.brightness = BrtN2F(arg_value);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//     SetRefreshLEDs();
//   }

//   sprintf_P(arg_ctr,PSTR("animation_toggle"));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     animation.flags.fEnable_Animation ^= 1;    
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "fEnable_Animation=%d"),animation.flags.fEnable_Animation);    
//   }

//   sprintf_P(arg_ctr,PSTR("pal_set"));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     animation.palette_id = arg_value;
//     SetRefreshLEDs();
//   }




//   // // check palette selector
//   // sprintf(arg_ctr,"relay0_toggle\0");
//   // if (pCONT_web->pWebServer->hasParam("relay0_toggle")) {
//   //   pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//   //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //   // animation.palette = arg_value;

//   //   ExecuteCommandPower(0,POWER_TOGGLE,SRC_MQTT);

//   //   // update_all = true; //refresh all
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_RELAYS "hasParam test"));//,arg_ctr,arg_value);
//   // }

//   for(uint8_t animation_mode_options_id=0;animation_mode_options_id<ANIMATION_MODE_LENGTH_ID;animation_mode_options_id++){
    
//     sprintf(arg_ctr,"%s%02d\0","animod",animation_mode_options_id);

//     if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       // animation.brightness = BrtN2F(arg_value);
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %d"),arg_ctr,arg_value,animation_mode_options_id);
//       // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//       // SetRefreshLEDs();
//       animation.mode_id = animation_mode_options_id;

//       if(animation.mode_id == ANIMATION_MODE_SCENE_ID){
//         scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;    // only needed until #Idea56
//       }
//       animation.flags.fForceUpdate=true;

//       // ExecuteCommandPower(relay_id,POWER_TOGGLE,SRC_MQTT);
//     }

  }//end function







// } //end function


// // Group all these websaves into one handler, separate from webcommand (ie consoles)
// void mPWMLight::WebSave_RGBColourSelector(void)
// {
  
// //   char tmp[100];

// // //  selectorlist.amount = 0;
// //   uint8_t  arg_value = 0;

// //   char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

// //   // check palette selector
// //   sprintf(arg_ctr,"g%d\0",99);
// //   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
// //     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
// //     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
// //     animation.palette_id = arg_value;
// //     update_all = true; //refresh all
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// //   }

// //   // Secondary edit location, paste to here
// //   //idea: write function that uses id to return pointer to struct
// //   SetPaletteListPtrFromID(animation.palette_id);

// //   // Only continue to write if the palette is editable type
// //   if(!CheckPaletteIsEditable(palettelist.ptr)){
// //     AddLog_P(LOG_LEVEL_TEST,PSTR(D_ERROR "Palette Not editable, leaving websave function"));
// //     return;
// //   }

// //   uint8_t new_amount = 0;
  
// //   for(int arg_id=0;arg_id<PRESET_COLOUR_MAP_INDEXES_MAX;arg_id++){
// //     sprintf(arg_ctr,"g%d\0",arg_id);
    
// //     if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
// //       pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
// //       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
// //       // Colour Add
// //       if(arg_value != COLOUR_MAP_NONE_ID){
// //         arg_value = arg_value < PRESET_COLOUR_MAP_INDEXES_MAX ? arg_value : 0; // Check it doesnt exceed array 
// //         palettelist.ptr->colour_map_id[new_amount++] = arg_value;
// //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d newamount=%d"),arg_ctr,arg_value,new_amount-1);
// //       }else{
// //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%s"),arg_ctr,"COLOUR_MAP_NONE_ID");
// //         palettelist.ptr->colour_map_id[arg_id] = COLOUR_MAP_NONE_ID; //set to unused
// //       }

// //     }else{
// //         //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "SKIPPING hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// //         palettelist.ptr->colour_map_id[arg_id] = COLOUR_MAP_NONE_ID;
// //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "NOT hasParam(\"%s\")=%s"),arg_ctr,"COLOUR_MAP_NONE_ID");
// //     }

// //   }

// //   // if new amount != 0 ie new values, stored new amount
// //   if(new_amount){
// //     palettelist.ptr->colour_map_size = new_amount-1;
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "colour_map_size=%d"),palettelist.ptr->colour_map_size);
// //   }

// //   // Erase others
// //   for(int erase_id=palettelist.ptr->colour_map_size;erase_id<PALETTELIST_COLOUR_AMOUNT_MAX;erase_id++){
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "erase_id %d=%s"),erase_id,"COLOUR_MAP_NONE_ID");
// //     palettelist.ptr->colour_map_id[erase_id] = COLOUR_MAP_NONE_ID; //set to unused
// //   }

// //   memset(tmp,0,sizeof(tmp));
// //   sprintf(arg_ctr,"%s\0",WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR);
// //   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
// //     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
// //     //valid range (change to user "PaletteIsEditable()")
// //     if(palettelist.ptr->id<PALETTELIST_VARIABLE_USER_20_ID){
// //       // Copy is variable is string
// //       strlcpy(palettelist.ptr->friendly_name_ctr, 
// //         (!strlen(tmp)) ? palettelist.ptr->friendly_name_ctr : tmp, 
// //         sizeof(char)*20
// //       );
// //       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%s,%s"),arg_ctr,tmp,palettelist.ptr->friendly_name_ctr);  
// //     }
// //   }


// //   animation.flags.fForceUpdate = true; // update leds now

// }

// void mPWMLight::WebSave_RGBColourSelectorOnly(void)
// {
// //   char tmp[100];

// // //  selectorlist.amount = 0;
// //   uint8_t  arg_value = 0;

// //   char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

// //   // check palette selector
// //   sprintf(arg_ctr,"g%d\0",99);
// //   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
// //     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
// //     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
// //     animation.palette_id = arg_value;
// //     update_all = true; //refresh all
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// //   }


// }


// void mPWMLight::WebSave_RGBControls(void)
// {
//   // char tmp[100]; memset(tmp,0,sizeof(tmp));
//   // uint8_t  arg_value = 0;
//   // char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

//   // // check palette selector
//   // for(int ii=0;ii<WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE;ii++){
//   //   sprintf(arg_ctr,"g%d\0",ii);
//   //   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
//   //     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//   //     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //     update_all = true; //refresh all
//   //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

//   //     switch(ii){
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELSUPDATED_PERCENTAGE");
//   //         // from mapped value to real value
//   //         arg_value = PROGMEM pixels_to_update_as_percentage_map[arg_value];
//   //         animation.transition.pixels_to_update_as_percentage.val = constrain(arg_value,0,100);
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELORDER");
//   //         animation.transition.order_id = arg_value; // no map
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"RATE");
//   //         animation.transition.rate_ms.val = PROGMEM rate_map_secs[arg_value]*1000; //seconds to milliseconds ra"
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.rate_ms.val);
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PERIOD");
//   //         animation.transition.time_ms.val = PROGMEM rate_map_secs[arg_value]*1000; //seconds to milliseconds
//   //         // If period > rate, increase period to rate
//   //         animation.transition.time_ms.val = animation.transition.time_ms.val>animation.transition.rate_ms.val?animation.transition.rate_ms.val:animation.transition.time_ms.val;
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms.val); 
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"TRANSITIONMETHOD");
//   //         animation.transition.method_id = arg_value;
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetTransitionMethodName());      
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"ANIMATIONMODE");
//   //         animation.mode_id = arg_value;
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetAnimationModeName());
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PALETTE");
//   //         animation.palette_id = arg_value;
//   //         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteFriendlyName());
//   //       break;
//   //     #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_FLASHER: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"FLASHER");
//   //         flashersettings.function = arg_value;  
//   //         flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;  //restart animation/function
//   //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "flasher.function = %d"),flashersettings.function);
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"FLASHER");
//   //         mixer.running_id = arg_value;          
//   //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.mode.running_id = %d"),mixer.running_id);
//   //       break;
//   //       #endif
//   //       default: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "DEFAULT hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   //       break;
//   //     } //switch
//   //   } //if
//   // } //for

//   animation.flags.fForceUpdate = true; // update leds now

// } // end function


// /******** Page items *****
//  * 
//  * Colour Palette Editor
//  * * Selection: Active Palette
//  * Colour Palette Editor
//  * * Selection: Change Palette
//  *  
//  * 
//  * 
//  * ***********************/
// void mPWMLight::HandlePage_PaletteEditor(AsyncWebServerRequest *request)
// {
//   //if (!HttpCheckPriviledgedAccess()) { return; }

//   // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

//   // if (request->hasParam("save")) {
//   //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
//   //   WebSave_RGBColourSelector();
//   //   //HandlePage_RGBLightSettings(); //return to main menu
//   //   //pCONT_web->HandleRoot(); //return to main menu
//   //   return;
//   // }

//   // memset(&data_buffer,0,sizeof(data_buffer));
//   // char *buf = data_buffer.payload.ctr;
//   // char **buffer = &buf;
//   // pCONT_web->buffer_writer_len = 0;

//   // BuildPage_RGBPaletteEditor(buffer);

//   // uint16_t freemem_start = ESP.getFreeHeap();  
//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR("WEB: url \"%s\" size [%d] FR %d"),"/",strlen(data_buffer.payload.ctr),freemem_start);

//   // pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);

//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_ASYNC "FreeRam %d"),freemem_start-ESP.getFreeHeap());
  
//   request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,PAGE_rgb_addressable_settings);//PAGE_rgb_colour_palette_editor);


// //   if (pCONT_web->pWebServer->hasParam("loadpixels")) {
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"loadpixels\")"));
// //     WebSave_RGBColourSelectorOnly();
// //     // Continue to send page
// //   }
  
// //   char colour_title_ctr[70];
// //   uint8_t colour_id = 0;
// //   uint8_t element_list_num = PALETTELIST_COLOUR_AMOUNT_MAX;
// //   uint8_t row_id_selected[PALETTELIST_COLOUR_AMOUNT_MAX];
// //   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
// //   uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
// //   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

// //   //GetPaletteListPtrFromID(animation.palette_id);
// //   SetPaletteListPtrFromID(animation.palette_id);
   
// //   // LOAD VALUES 
// //   uint8_t palette_selected_id = palettelist.ptr->id;
// //   // colours in palette
// //   HsbColor set_colours[20];
// //   for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
// //     set_colours[i] = preset_colour_map[palettelist.ptr->colour_map_id[i]]; 

// // // GETCOLOURSFN

// //     //if(palettelist.ptr->flags.fColours_from_map){
// //       colour_selected_id[i] = palettelist.ptr->colour_map_id[i];    
// //     //}else{
// //       // USE * for random colours not within colour_map
// //     //
// //     //}
// //   } // inactive colours set elsewhere   
  
// //   // "m" = send palette name
// //   // Send all the options
// //   if (pCONT_web->pWebServer->hasParam("m")) {
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"m\")"));
// //     pCONT_web->WSContentBegin(200, CT_PLAIN);
// //     for (uint8_t row_id = 0; row_id < PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
// //       BufferWriterI->Append_P(
// //         HTTP_MODULE_TEMPLATE_REPLACE3, 
// //         row_id, 
// //         GetPaletteFriendlyNameByID(row_id)
// //       );
// //     }
// //     pCONT_web->WSContentEnd();
// //     return;
// //   }

// //   // "g" = send colour name
// //   // Send all the options
// //   if (pCONT_web->pWebServer->hasParam("g")) {
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"g\")"));
// //     pCONT_web->WSContentBegin(200, CT_PLAIN);
// //     for (uint8_t item_id = 0; item_id < COLOUR_MAP_PALETTE_OPTIONS_END_ID; item_id++) {      //also send none
// //       BufferWriterI->Append_P(HTTP_MODULE_TEMPLATE_REPLACE2,  // Option list with parameter (template,row number,row font colour, row text, row bracket number)
// //         item_id, 
// //         WebColorFromColourMap(item_id),
// //         GetColourMapNamebyID(item_id)
// //         #ifdef USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
// //           ,HueF2N(preset_colour_map[item_id].H),SatF2N(preset_colour_map[item_id].S)
// //         #endif
// //       );
// //     }
// //     pCONT_web->WSContentEnd();
// //     return;
// //   }

// //   /*** 
// //    * Send Page WITH fields that have been selected, palette type then colours set
// //    * */

// //   pCONT_web->WSStartAppend_P(S_COLOUR_PALETTE_EDITOR);
// //   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE_TEMPLATE);
// //   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE1c, palette_selected_id); //active palette

  
// //   element_list_num = CheckPaletteIsEditable(palettelist.ptr) ? 
// //         PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(palettelist.ptr);

// //   for (uint8_t menu_id = 0; menu_id < element_list_num; menu_id++) {
// //     BufferWriterI->Append_P(PSTR("sk(%d,%d);"),colour_selected_id[menu_id],menu_id); // active colours
// //   }
// //   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE2c);
// //   pCONT_web->WSContentSendStyle();

//   // BufferWriterI->Append_P(HTTP_FORM_RGB_COLOUR_SELECTOR, GetPaletteFriendlyName());

//   // BufferWriterI->Append_P(HTTP_FORM_RGB_LOAD_PIXELS);

//   // if(CheckPaletteIsEditable(&palettelist.users[animation.palette_id])){

//   //   BufferWriterI->Append_P(HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
//   //                             "Edit Name",
//   //                             GetPaletteFriendlyNameByID(animation.palette_id), //previous name
//   //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   //                             "Enter New Palette Name (maximum 20 characters)",
//   //                             GetPaletteFriendlyNameByID(animation.palette_id)    // News a new "getfriendlyname option" for when user editable names exist
//   //                           );

//   // }

// //     //   palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?
// //     //     GetColourMapNamebyID(colour_selected_id[row_id]):GetEstimatedColourMapNamebyColour()
      
      
      
// //     // ID(colour_selected_id[row_id]),
  
// //   // if not users, then only show number of static fields
// //   element_list_num = CheckPaletteIsEditable(palettelist.ptr) ? 
// //         PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(palettelist.ptr);

// //   // Send Table
// //   BufferWriterI->Append_P(PSTR("<br/><p></p><b>" "Pixel Colour (Saturation %)" "</b><table>"));
// //   for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {

// //     switch(palettelist.ptr->flags.fMapIDs_Type){
// //       case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
// //         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
// //           WebColorFromColourType(set_colours[row_id]),
// //           row_id+1, //to show 1-20 pixel number
// //           GetColourMapNamebyID(colour_selected_id[row_id]),// "??"
// //           ""
// //           // palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?"":"*"
// //         );

// //       break;
// //       default:{
// //         int16_t pixel_num = 0;
// //         HsbColor colour = GetColourFromPalette(palettelist.ptr,row_id,&pixel_num);

// //         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
// //           WebColorFromColourType(colour),
// //           row_id+1,//to show 1-20 pixel number
// //           GetColourMapNameNearestbyColour(colour),// "??"
// //           "*"
// //         );
// //       }
// //       break;

// //     }


// //     BufferWriterI->Append_P(
// //       PSTR("<tr><td style='width:200px'><b>%s</b></td><td style='width:216px'><select id='g%d' name='g%d'></select></td></tr>"), //190/176 to 120/346
// //         colour_title_ctr, 
// //         row_id_selected[row_id], 
// //         row_id_selected[row_id]
// //     );
// //   }
// //   BufferWriterI->Append_P(PSTR("</table>"));
// //   if(palettelist.ptr->flags.fMapIDs_Type!=MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID){
// //     BufferWriterI->Append_P(PSTR("<tr>%s</tr>"),"* Indicates colour is not from selected options, showing nearest colour.");
// //   }

// //   // BufferWriterI->Append_P(HTTP_FORM_RGB_SELECTOR2, pCONT_set->Settings.syslog_host, pCONT_set->Settings.syslog_port, pCONT_set->Settings.tele_period);
      
      
// //   if(CheckPaletteIsEditable(palettelist.ptr)){
// //     BufferWriterI->Append_P(HTTP_FORM_END);
// //   }else{
// //     BufferWriterI->Append_P(HTTP_FORM_END_NOSAVE);
// //   }

// //   pCONT_web->WSContentSpaceButton(BUTTON_MAIN);
// //   pCONT_web->WSContentStop();
// }




// /**
//  * Send minimal script
//  * Send minimal style of page
//  * Send all needed script asks
//  * Send all needed javascript asks 
//  * */
// void mPWMLight::BuildPage_RGBPaletteEditor(char** buffer)
// {
  
//   char colour_title_ctr[70];
//   uint8_t colour_id = 0;
//   uint8_t element_list_num = 2;//PALETTELIST_COLOUR_AMOUNT_MAX;
//   uint8_t row_id_selected[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
//   uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

//   //GetPaletteListPtrFromID(animation.palette_id);
//   SetPaletteListPtrFromID(animation.palette_id);
   
//   // LOAD VALUES 
//   uint8_t palette_selected_id = palettelist.ptr->id;
//   // colours in palette
//   HsbColor set_colours[20];
//   for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
//     set_colours[i] = HsbColor(0);//preset_colour_map[palettelist.ptr->colour_map_id[i]]; 

// // GETCOLOURSFN

//     //if(palettelist.ptr->flags.fColours_from_map){
//       colour_selected_id[i] = palettelist.ptr->colour_map_id[i];    
//     //}else{
//       // USE * for random colours not within colour_map
//     //
//     //}
//   } // inactive colours set elsewhere   
  
//   /*** 
//    * Send Page WITH fields that have been selected, palette type then colours set
//    * */

//   // uint8_t element_list_num = 9;
//   /***
//    * 0 = Amount number
//             as % 0-10-25-50-75-100 of max leds ie 50% of 50 leds would be 25
//    * 1 = Amount Percentage
//             % 0-10-25-50-75-100
//    * 2 = Animate Order
//             inorder-random-rotate
//    * 3 = Animate Rate/Frequency
//             1-2-3-4-5-10-15-20-30
//    * 4 = Animate Time
//             1-2-3-4-5-10-15-20-30
//    * 5 = Animate Transition Method
//             instant-blend
//    * 6 = Animation Mode
//             presets-flasher-scenes-notifications
//    * 7 = Palette
//             list as before
//    * 8 = Flasher Method
//             Xmas flashers, only show when mode as active
//    * */

//   // find nearest percentage value from its map, get the index (ie 100% is the 10th index)
//   int16_t perc_index = mSupport::FindNearestValueIndexUInt8(PROGMEM pixels_to_update_as_percentage_map,
//                                                   sizeof(PROGMEM pixels_to_update_as_percentage_map),
//                                                   animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.pixels_to_update_as_percentage.val,
//     perc_index,
//     PROGMEM pixels_to_update_as_percentage_map[perc_index],
//     sizeof(PROGMEM pixels_to_update_as_percentage_map)
//   );

//   if(perc_index<0){
//     //not found
//     perc_index = 0;// send 0
//   }
  
//   uint8_t rate_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.rate_ms.val/1000
//                                                  );
                                         
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.rate_ms.val/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );

//   uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.time_ms.val/1000
//                                                  );
                                            
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.time_ms.val/1000,
//     time_index,
//     PROGMEM rate_map_secs[time_index],
//     sizeof(PROGMEM rate_map_secs)
//   );


//   // pCONT_web->WSStartAppend_P2(buffer, S_COLOUR_PALETTE_EDITOR);
  
//   // pCONT_web->WSBufferAppend_P2(buffer, "<style>");
//   // pCONT_web->WSBufferAppend_P2(buffer, PM_HTTP_HEAD_STYLE1_MINIMAL,
//   //   pCONT_web->WebColor(pCONT_set->COL_FORM), 
//   //   pCONT_web->WebColor(pCONT_set->COL_BACKGROUND),
//   //   pCONT_web->WebColor(pCONT_set->COL_BUTTON),  pCONT_web->WebColor(pCONT_set->COL_BUTTON_TEXT)
//   // );
//   // pCONT_web->WSBufferAppend_P2(buffer, "</style>");
  
//   // pCONT_web->WSBufferAppend_P2(buffer, "</head>" "<body>");

//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_STYLE_TITLE_CONTAINER_HEAD, 
//   //   pCONT_web->WebColor(pCONT_set->COL_TEXT),
//   //   pCONT_web->WebColor(pCONT_set->COL_TEXT_MODULE_TITLE), 
//   //   PROJECT_NAME_CTR, pCONT_set->Settings.system_name.friendly
//   // );

//   // // pCONT_web->WSBufferAppend_P2(buffer, HTTP_FORM_RGB_COLOUR_SELECTOR, GetPaletteFriendlyName());

//   // // pCONT_web->WSBufferAppend_P2(buffer, HTTP_FORM_RGB_LOAD_PIXELS);
  
//   // // if(CheckPaletteIsEditable(&palettelist.users[animation.palette_id])){

//   // //   pCONT_web->WSBufferAppend_P2(buffer, HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
//   // //                             "Edit Name",
//   // //                             GetPaletteFriendlyNameByID(animation.palette_id), //previous name
//   // //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   // //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   // //                             "Enter New Palette Name (maximum 20 characters)",
//   // //                             GetPaletteFriendlyNameByID(animation.palette_id)    // News a new "getfriendlyname option" for when user editable names exist
//   // //                           );

//   // // }

//   // //   //   palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?
//   // //   //     GetColourMapNamebyID(colour_selected_id[row_id]):GetEstimatedColourMapNamebyColour()
      
      
      
//   // //   // ID(colour_selected_id[row_id]),
  
//   // // // if not users, then only show number of static fields
//   // // element_list_num = CheckPaletteIsEditable(palettelist.ptr) ? 
//   // //       PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(palettelist.ptr);

//   // // // Send Table
//   // // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<br/><p></p><b>" "Pixel Colour (Saturation %)" "</b><table>"));
//   // // for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {

//   // //   switch(palettelist.ptr->flags.fMapIDs_Type){
//   // //     case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
//   // //       snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
//   // //         WebColorFromColourType(set_colours[row_id]),
//   // //         row_id+1, //to show 1-20 pixel number
//   // //         GetColourMapNamebyID(colour_selected_id[row_id]),// "??"
//   // //         ""
//   // //         // palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?"":"*"
//   // //       );

//   // //     break;
//   // //     default:{
//   // //       int16_t pixel_num = 0;
//   // //       HsbColor colour = GetColourFromPalette(palettelist.ptr,row_id,&pixel_num);

//   // //       snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
//   // //         WebColorFromColourType(colour),
//   // //         row_id+1,//to show 1-20 pixel number
//   // //         GetColourMapNameNearestbyColour(colour),// "??"
//   // //         "*"
//   // //       );
//   // //     }
//   // //     break;

//   // //   }


//   // //   pCONT_web->WSBufferAppend_P2(buffer, 
//   // //     PSTR("<tr><td style='width:200px'><b>%s</b></td><td style='width:216px'><select id='g%d' name='g%d'></select></td></tr>"), //190/176 to 120/346
//   // //       colour_title_ctr, 
//   // //       row_id_selected[row_id], 
//   // //       row_id_selected[row_id]
//   // //   );
//   // // }
//   // // pCONT_web->WSBufferAppend_P2(buffer, PSTR("</table>"));
//   // // if(palettelist.ptr->flags.fMapIDs_Type!=MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID){
//   // //   BufferWriterI->Append_P(buffer, PSTR("<tr>%s</tr>"),"* Indicates colour is not from selected options, showing nearest colour.");
//   // // }

//   // // BufferWriterI->Append_P(HTTP_FORM_RGB_SELECTOR2, pCONT_set->Settings.syslog_host, pCONT_set->Settings.syslog_port, pCONT_set->Settings.tele_period);
      
      
//   // // if(CheckPaletteIsEditable(palettelist.ptr)){
//   // //   BufferWriterI->Append_P(HTTP_FORM_END);
//   // // }else{
//   // //   BufferWriterI->Append_P(HTTP_FORM_END_NOSAVE);
//   // // }
//   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<div class='ani_pal_edit_con'></div>")); //animations control container
  
//   // // pCONT_web->WSButtonAppend2(buffer, BUTTON_COLOUR_PALETTE_EDITOR);
//   // // pCONT_web->WSButtonAppend2(buffer, BUTTON_COLOUR_MIXER_EDITOR);

//   // pCONT_web->WSSpaceButtonAppend2(buffer, BUTTON_MAIN);

//   // pCONT_web->WSBufferAppend_SystemVersionBar(buffer);

//   // // #ifdef SEND_MINIMAL_STYLESHEET1 // Minimal
//   //   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<link rel='stylesheet' href='" WEB_HANDLER_STYLE_WEB_BASIC_ROOT "'>")); 
//   // // #endif
  
//   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<script type='text/javascript' src='"
//   //   "/" WEB_HANDLE_RGB_COLOUR_PALETTE_EDITOR "/load_script.js" "'></script>"));

//   // pCONT_web->WSBufferAppend_P2(buffer, "</body></html>");

// } // end function




// void mPWMLight::HandlePage_RGBLightSettings(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandlePage_RGBLightSettings"));
  
//   // if (HandleParameters_RGBLightSettings(request)) {
//   //   AddLog_P(LOG_LEVEL_TEST,PSTR("HandleParameters_RGBLightSettings(request) FOUND SO STOP"));
//   //   return;
//   // }
  
//   //   if (pCONT_web->pWebServer->hasParam("save")) {
//   //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
//   //     WebSave_RGBControls();
//   //     pCONT_web->HandleRoot(); // return to main menu
//   //     return;
//   //   }
  
//   request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,PAGE_rgb_addressable_settings);

// }



// /**
//  * Send minimal script
//  * Send minimal style of page
//  * Send all needed script asks
//  * Send all needed javascript asks 
//  * */
// void mPWMLight::BuildPage_RGBLightSettings(char** buffer)
// {
  

//   /*** 
//    * Send Page WITH fields that have been selected, palette type then colours set
//    * */

//   uint8_t element_list_num = 9;
//   /***
//    * 0 = Amount number
//             as % 0-10-25-50-75-100 of max leds ie 50% of 50 leds would be 25
//    * 1 = Amount Percentage
//             % 0-10-25-50-75-100
//    * 2 = Animate Order
//             inorder-random-rotate
//    * 3 = Animate Rate/Frequency
//             1-2-3-4-5-10-15-20-30
//    * 4 = Animate Time
//             1-2-3-4-5-10-15-20-30
//    * 5 = Animate Transition Method
//             instant-blend
//    * 6 = Animation Mode
//             presets-flasher-scenes-notifications
//    * 7 = Palette
//             list as before
//    * 8 = Flasher Method
//             Xmas flashers, only show when mode as active
//    * */

//   // find nearest percentage value from its map, get the index (ie 100% is the 10th index)
//   int16_t perc_index = mSupport::FindNearestValueIndexUInt8(PROGMEM pixels_to_update_as_percentage_map,
//                                                   sizeof(PROGMEM pixels_to_update_as_percentage_map),
//                                                   animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.pixels_to_update_as_percentage.val,
//     perc_index,
//     PROGMEM pixels_to_update_as_percentage_map[perc_index],
//     sizeof(PROGMEM pixels_to_update_as_percentage_map)
//   );

//   if(perc_index<0){
//     //not found
//     perc_index = 0;// send 0
//   }
  
//   uint8_t rate_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.rate_ms.val/1000
//                                                  );
                                         
//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//   //   animation.transition.rate_ms.val/1000,
//   //   rate_index,
//   //   PROGMEM rate_map_secs[rate_index],
//   //   sizeof(PROGMEM rate_map_secs)
//   // );

//   // uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//   //                                                 sizeof(PROGMEM rate_map_secs),
//   //                                                 animation.transition.time_ms.val/1000
//   //                                                );
                                            
//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//   //   animation.transition.time_ms.val/1000,
//   //   time_index,
//   //   PROGMEM rate_map_secs[time_index],
//   //   sizeof(PROGMEM rate_map_secs)
//   // );


//   // pCONT_web->WSStartAppend_P2(buffer, "LED Light Controls1");
  
//   // pCONT_web->WSBufferAppend_P2(buffer, "<style>");
//   // pCONT_web->WSBufferAppend_P2(buffer, PM_HTTP_HEAD_STYLE1_MINIMAL,
//   //   pCONT_web->WebColor(pCONT_set->COL_FORM), 
//   //   pCONT_web->WebColor(pCONT_set->COL_BACKGROUND),
//   //   pCONT_web->WebColor(pCONT_set->COL_BUTTON),  pCONT_web->WebColor(pCONT_set->COL_BUTTON_TEXT)
//   // );
//   // pCONT_web->WSBufferAppend_P2(buffer, "</style>");
  
//   // pCONT_web->WSBufferAppend_P2(buffer, "</head>" "<body>");

//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_STYLE_TITLE_CONTAINER_HEAD, 
//   //   pCONT_web->WebColor(pCONT_set->COL_TEXT),
//   //   pCONT_web->WebColor(pCONT_set->COL_TEXT_MODULE_TITLE), 
//   //   PROJECT_NAME_CTR, pCONT_set->Settings.system_name.friendly
//   // );
  
//   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<div class='ani_controls_con'></div>")); //animations control container
  
//   // pCONT_web->WSButtonAppend2(buffer, BUTTON_COLOUR_PALETTE_EDITOR);
//   // pCONT_web->WSButtonAppend2(buffer, BUTTON_COLOUR_MIXER_EDITOR);

//   // pCONT_web->WSSpaceButtonAppend2(buffer, BUTTON_MAIN);

//   // pCONT_web->WSBufferAppend_SystemVersionBar(buffer);

//   // // #ifdef SEND_MINIMAL_STYLESHEET1 // Minimal
//   //   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<link rel='stylesheet' href='" WEB_HANDLER_STYLE_WEB_BASIC_ROOT "'>")); 
//   // // #endif
  
//   // pCONT_web->WSBufferAppend_P2(buffer, PSTR("<script type='text/javascript' src='"
//   //   "/" D_WEB_HANDLE_RGB_CONTROLS "/load_script.js" "'></script>"));

//   // pCONT_web->WSBufferAppend_P2(buffer, "</body></html>");

// } // end function



// void mPWMLight::HandleParameters_RGBLightSettings(AsyncWebServerRequest *request)
// {

  
// }

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF WEBPAGE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF ANIMATION/FLASHER/MIXER DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// #ifdef ENABLE_PIXEL_FUNCTION_FLASHER

// /******** Page items *****
//  * 
//  * Colour Palette Editor
//  * * Selection: Active Palette
//  * Colour Palette Editor
//  * * Selection: Change Palette
//  *  
//  * 
//  * ***********************/
// void mPWMLight::HandleMixerEditor(void)
// {
 
//   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

//   if (pCONT_web->pWebServer->hasParam("save")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
//     WebSave_RGBControls();
//     pCONT_web->HandleRoot(); // return to main menu
//     return;
//   }

//   char colour_title_ctr[70];
//   uint8_t row_id_selected[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}

//   SetPaletteListPtrFromID(animation.palette_id);
//   uint8_t palette_selected_id = palettelist.ptr->id;

//   /***
//    * Send list parameters if requested, then exit
//    * */


//   if (pCONT_web->pWebServer->hasParam("po")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pu\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= TRANSITION_ORDER_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         GetTransitionOrderNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("ra")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pa\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3_NUM, 
//         row_id, 
//         PROGMEM rate_map_secs[row_id]
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("pr")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pr\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3_NUM, 
//         row_id, 
//         PROGMEM rate_map_secs[row_id]
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("tm")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pa\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= TRANSITION_METHOD_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         GetTransitionMethodNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("am")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pa\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= ANIMATION_MODE_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         GetAnimationModeNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }
  
//   if (pCONT_web->pWebServer->hasParam("pa")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"pa\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         GetPaletteFriendlyNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("fl")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"%s\")"),"fl");
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= FLASHER_FUNCTION_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         GetFlasherFunctionNamebyID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("mi")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"%s\")"),"mi");
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= 6; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         "mixer.mode.running_id"
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   /*** 
//    * Send Page WITH fields that have been selected, palette type then colours set
//    * */

//   uint8_t element_list_num = 9;
//   /***
//    * 0 = Amount number
//             as % 0-10-25-50-75-100 of max leds ie 50% of 50 leds would be 25
//    * 1 = Amount Percentage
//             % 0-10-25-50-75-100
//    * 2 = Animate Order
//             inorder-random-rotate
//    * 3 = Animate Rate/Frequency
//             1-2-3-4-5-10-15-20-30
//    * 4 = Animate Time
//             1-2-3-4-5-10-15-20-30
//    * 5 = Animate Transition Method
//             instant-blend
//    * 6 = Animation Mode
//             presets-flasher-scenes-notifications
//    * 7 = Palette
//             list as before
//    * 8 = Flasher Method
//             Xmas flashers, only show when mode as active
//    * */

//   // find nearest percentage value from its map, get the index (ie 100% is the 10th index)
//   int16_t perc_index = mSupport::FindNearestValueIndexUInt8(PROGMEM pixels_to_update_as_percentage_map,
//                                                   sizeof(PROGMEM pixels_to_update_as_percentage_map),
//                                                   animation.transition.pixels_to_update_as_percentage
//                                                  );

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.pixels_to_update_as_percentage,
//     perc_index,
//     PROGMEM pixels_to_update_as_percentage_map[perc_index],
//     sizeof(PROGMEM pixels_to_update_as_percentage_map)
//   );

//   if(perc_index<0){
//     //not found
//     perc_index = 0;// send 0
//   }

  
//   uint8_t rate_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.rate_ms/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.rate_ms/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );


//   pCONT_web->WSStartAppend_P("LED Light Controls");
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE_TEMPLATE_MIXER_EDITOR);
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE1_MIXER_EDITOR, 
//                               perc_index,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
//                               animation.transition.order_id,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
//                               rate_index,    // "ld('rgb_controls?ra=1',x2);" // ra = rate
//                               (uint8_t)round(animation.transition.time_ms/1000),    // "ld('rgb_controls?pr=1',x3);" // pr = period time
//                               animation.transition.method_id,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
//                               animation.mode_id,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
//                               animation.palette_id,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
//                               flashersettings.function,                             // "ld('rgb_controls?fl=1',x7);" // fl  = flasher  
//                               mixer.running_id                                 // "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id                          
//                             ); // Send lists
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE2c_MIXER_EDITOR);
//   pCONT_web->WSContentSendStyle();

  

//   // Section
//   BufferWriterI->Append_P(HTTP_FORM_RGB_COLOUR_SELECTOR_MIXER_EDITOR);

//   BufferWriterI->Append_P(HTTP_FORM_COMPLETE_MIXER_EDITOR_SELECTOR, "colour_palette_name_ctr");

//   BufferWriterI->Append_P(PSTR("<br>"));  BufferWriterI->Append_P(PSTR("</br>"));


//   //BufferWriterI->Append_P(HTTP_FORM_RGB_LOAD_PIXELS);

//   char listheading[25];
//   BufferWriterI->Append_P(PSTR("<table>"));
//   for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
//     snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%s</font>"),
//       WebColorFromColourType(preset_colour_map[COLOUR_MAP_WHITE_ID]),
//       pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), row_id, kTitle_ListHeadings_Mixer_Editor)
//     );
//     AddLog_P(LOG_LEVEL_TEST,PSTR("kTitle=%s"),pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), row_id, kTitle_ListHeadings_Mixer_Editor));
//     BufferWriterI->Append_P(
//       PSTR("<tr><td style='width:200px'><b>%s</b></td><td style='width:216px'><select id='g%d' name='g%d'></select></td></tr>"), //190/176 to 120/346
//         colour_title_ctr, 
//         row_id_selected[row_id], 
//         row_id_selected[row_id]
//     );
//   }
//   BufferWriterI->Append_P(PSTR("</table>"));
  
//   //BufferWriterI->Append_P(HTTP_FORM_RGB_SELECTOR2, pCONT_set->Settings.syslog_host, pCONT_set->Settings.syslog_port, pCONT_set->Settings.tele_period);
    
//   BufferWriterI->Append_P(HTTP_FORM_END);


//   // pCONT_web->WSContentSpaceButton(BUTTON_MAIN);
//   // pCONT_web->WSContentStop();


//   // WSContentButton(BUTTON_WIFI);

//   // pCONT->Tasker_Interface(FUNC_WEB_ADD_BUTTON);

//   // WSContentButton(BUTTON_LOGGING);
//   // WSContentButton(BUTTON_OTHER);
//   // WSContentButton(BUTTON_TEMPLATE);

//   // WSContentSpaceButton(BUTTON_RESET_CONFIGURATION);
//   // WSContentButton(BUTTON_BACKUP);
//   // WSContentButton(BUTTON_RESTORE);
  

//   pCONT_web->WSContentButton(BUTTON_COLOUR_PALETTE_EDITOR);
//   pCONT_web->WSContentButton(BUTTON_COLOUR_MIXER_EDITOR); //page for creating all mixer settings (instead of hardcoded)

//   pCONT_web->WSContentSpaceButton(BUTTON_MAIN);
//   pCONT_web->WSContentStop();
// }






// void mPWMLight::WebPage_Root_SendStyle(){

 
 

// }

// void mPWMLight::WebPage_Root_SendBody(){

//   WebSendBody_Liveview();
//   WebSendBody_Palette();
//   WebSendBody_InfoTable(); //send_body_tables = new type so sensors add here?

// }


// void mPWMLight::WebPage_Root_SendStatus(){

//   /****
//    *  Show LED power 
//    * ****/
//   if(liveview.show_type == RGB_VIEW_SHOW_TYPE_NONE_ID){
//     uint8_t onoff = fAnyLEDsOnOffCount;
//     uint8_t fsize = 32;
//     char onoff_ctr[30];
//     if(onoff){
//       sprintf(onoff_ctr,PSTR("Lights ON (%d)"),fAnyLEDsOnOffCount);
//     }else{
//       sprintf(onoff_ctr,PSTR("Lights OFF"));
//     }
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//     BufferWriterI->Append_P(HTTP_DEVICE_STATE, 
//       100 / 1, //FULL WIDTH 1 item for length
//       onoff ? "bold" : "normal", 
//       fsize, 
//       onoff_ctr
//     );
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//     // Animation info
//     memset(onoff_ctr,0,sizeof(onoff_ctr));
//     fsize = 20;
//   }
 
// }





// void mPWMLight::WebPage_Root_SendInformationModule(){

//   #ifdef ADA_TEST
//     char str[10];
//     memset(str,0,sizeof(str));
//     dtostrf(_ADALIGHTNEOPIXEL_H, 4, 2, str );

//     BufferWriterI->Append_P(PSTR("}1" D_MODULE_LIGHTS_ADDRESSABLE_CTR "}2%s%s"), "Version ", str);
//     BufferWriterI->Append_P(PSTR("}1" "Version" "}2%s"), str);
//   #endif

// }


// void mPWMLight::WebPage_Root_AddHandlers(){
  
//   /**
//    * Root Page 
//    * */
//   pCONT_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
//     WebSend_JSON_RootPage_LiveviewPixels(request); 
//   });
//   pCONT_web->pWebServer->on("/draw/palette_selector.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Root_Draw_PaletteSelect(request);
//   });

  
//   /**
//    * RGB LIGHT SETTINGS
//    * */
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS, [this](AsyncWebServerRequest *request){ //"/page"
//     HandlePage_RGBLightSettings(request); 
//   });
//   pCONT_web->pWebServer->on("/fetch/animation_control_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_RGBLightSettings_ListOptions(request);
//   }); 
//   pCONT_web->pWebServer->on("/rgb_addressable_settings/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_RGBLightSettings_UpdateURLs(request);
//   }); 
//   pCONT_web->pWebServer->on("/rgb_addressable_settings/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_RGBLightSettings_Draw(request);
//   });


//   /**
//    * PALETTE EDTIOR
//    * */
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_CTR, [this](AsyncWebServerRequest *request){ //"/page"
//     HandlePage_PaletteEditor(request); 
//   });
//   pCONT_web->pWebServer->on("/fetch/colour_palette_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_PaletteEditor_ListOptions(request);
//   }); 
//   pCONT_web->pWebServer->on("/rgb_colour_palette_editor/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_PaletteEditor_UpdateURLs(request);
//   }); 
//   pCONT_web->pWebServer->on("/rgb_colour_palette_editor/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_PaletteEditor_Draw(request);
//   });


// } //add handlers


// void mPWMLight::Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Level_Start("function");
//       JsonBuilderI->Level_Start("Parse_Urls");        
//         JsonBuilderI->Add("/fetch/animation_control_list_options.json",-1);
//       JsonBuilderI->Level_End();
//     JsonBuilderI->Level_End();
//   JsonBuilderI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }

// void mPWMLight::Web_RGBLightSettings_ListOptions(AsyncWebServerRequest *request){
  
//   JsonBuilderI->Start();    
//     WebAppend_RGBLightSettings_FillOptions_Controls();
//   JsonBuilderI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }



// void mPWMLight::Web_RGBLightSettings_Draw(AsyncWebServerRequest *request){
        
//   if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JsonBuilderI->Start();
//     WebAppend_RGBLightSettings_Draw_Animation_Options();
//     WebAppend_RGBLightSettings_Draw_Flasher_Options();
//     WebAppend_RGBLightSettings_Draw_PageButtons();
//   JsonBuilderI->End();

//   pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mPWMLight::WebAppend_RGBLightSettings_Draw_Animation_Options(){

//   JsonBuilderI->Array_Start("container_1");//animation_options_container");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");

//         JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_LIST_START);
//           char listheading[25];
//           char title_ctr[70];
//           uint8_t element_list_num = 6;
//           uint8_t row_id_selected[element_list_num];
//           for(int j=0;j<element_list_num;j++){row_id_selected[j] = j;}

//           JsonBuilderI->AppendBuffer(PSTR("{t}"));
//           for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
//             snprintf_P(title_ctr, sizeof(title_ctr), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), row_id, kTitle_ListHeadings));
//             JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
//               JsonBuilderI->AppendBuffer(PSTR("<td style='width:200px'><b>%s</b></td>"),title_ctr);
//               JsonBuilderI->AppendBuffer(PSTR("<td style='width:216px'><select id='g%d' name='g%d'></select></td>"),row_id_selected[row_id],row_id_selected[row_id]);
//             JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
//           }
//           JsonBuilderI->AppendBuffer(PSTR("{t2}"));

//         JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE);

//       JsonBuilderI->AppendBuffer("\"");
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
// }

// void mPWMLight::WebAppend_RGBLightSettings_Draw_Flasher_Options(){

//   JsonBuilderI->Array_Start("container_2");//flasher_options_container");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");
//         // pCONT->Tasker_Interface(FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER);
//       JsonBuilderI->AppendBuffer("\"");
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
// }

// void mPWMLight::WebAppend_RGBLightSettings_Draw_PageButtons(){

//   JsonBuilderI->Array_Start("container_3");//page_button_container");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");
//         pCONT_web->WebAppend_Button_Spaced(PM_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR, PM_WEB_HANDLE_PALETTE_EDITOR_CTR);
//         pCONT_web->WebAppend_Button(PM_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR, PM_WEB_HANDLE_RGB_ANIMATION_MIXER_EDITOR_CTR);
//         pCONT_web->WebAppend_Button_Spaced(BUTTON_MAIN);
//       JsonBuilderI->AppendBuffer("\"");
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();

// }



// /***************
//  * 
//  * 
//  * 
//  * Palette editor: Page x/y
//  * 
//  * 
//  * *********/

// void mPWMLight::Web_PaletteEditor_Draw(AsyncWebServerRequest *request){
        
//   if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JsonBuilderI->Start();
//     WebAppend_PaletteEditor_Draw_Editor_Form();
//   JsonBuilderI->End();

//   pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mPWMLight::WebAppend_PaletteEditor_Draw_Editor_Form(){


//   JsonBuilderI->Array_Start("container_1");//animation_options_container");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");

//         JsonBuilderI->AppendBuffer(PM_HTTP_FORM_START_VARIABLE,PSTR("Colour Palette Editor"),PSTR("rgb_pal"));
//           char listheading[25];
//           char title_ctr[70];
//           uint8_t element_list_num = 6;
//           uint8_t row_id_selected[element_list_num];
//           for(int j=0;j<element_list_num;j++){row_id_selected[j] = j;}

//           // JsonBuilderI->AppendBuffer(PSTR("{t}"));
//           // for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
//           //   snprintf_P(title_ctr, sizeof(title_ctr), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), row_id, kTitle_ListHeadings));
//           //   JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
//           //     JsonBuilderI->AppendBuffer(PSTR("<td style='width:200px'><b>%s</b></td>"),title_ctr);
//           //     JsonBuilderI->AppendBuffer(PSTR("<td style='width:216px'><select id='g%d' name='g%d'></select></td>"),row_id_selected[row_id],row_id_selected[row_id]);
//           //   JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
//           // }
//           // JsonBuilderI->AppendBuffer(PSTR("{t2}"));

//   // char listheading[25];
 
//   char buffer[50];

//   char colour_title_ctr[70];
//   uint8_t colour_id = 0;
//   element_list_num = 2;
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
//   uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

//   //GetPaletteListPtrFromID(animation.palette_id);
//   SetPaletteListPtrFromID(animation.palette_id);
  
//   // LOAD VALUES 
//   uint8_t palette_selected_id = palettelist.ptr->id;
//   // colours in palette
//   HsbColor set_colours[20];
//   for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
//     set_colours[i] = HsbColor(0);//preset_colour_map[palettelist.ptr->colour_map_id[i]]; 
//     //if(palettelist.ptr->flags.fColours_from_map){
//       colour_selected_id[i] = palettelist.ptr->colour_map_id[i];    
//     //}else{
//       // USE * for random colours not within colour_map
//     //
//     //}
//   } // inactive colours set elsewhere   

  
//   JsonBuilderI->AppendBuffer(PSTR("<b>" "Select Palette" "</b> (Active: %s)<br/>"), GetPaletteFriendlyName(buffer));
//   JsonBuilderI->AppendBuffer(PSTR("<select id='g99' name='g99'></select><br/>"));
//   pCONT_web->WebAppend_Button2(PSTR("Load Palette"),"loadpixels","bora");


//   if(CheckPaletteIsEditable(&palettelist.users[animation.palette_id])){
//     JsonBuilderI->AppendBuffer(HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
//                               PSTR("Edit Name"),
//                               GetPaletteFriendlyNameByID(animation.palette_id), //previous name
//                               WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//                               WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//                               PSTR("Enter New Palette Name (maximum 20 characters)"),
//                               GetPaletteFriendlyNameByID(animation.palette_id)    // News a new "getfriendlyname option" for when user editable names exist
//                             );
//   }

//   JsonBuilderI->AppendBuffer(
//     PSTR(
//       "<br/>"
//         "<p></p>"
//         "<b>" "Pixel Colour (Saturation %%)" "</b>"
//     )
//   );

//   JsonBuilderI->AppendBuffer("%s",PSTR("{t}"));

//   // if not users, then only show number of static fields
//   element_list_num = CheckPaletteIsEditable(palettelist.ptr) ? 
//         PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(palettelist.ptr);

//   for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
//     snprintf_P(title_ctr, sizeof(title_ctr), "%02d None", row_id+1);
//     JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       JsonBuilderI->AppendBuffer(PSTR("{sw}200px'><b>%s</b></td>"),title_ctr);
//       JsonBuilderI->AppendBuffer(PSTR("{sw}216px'>{si}g%d' name='g%d{si2}"),row_id_selected[row_id],row_id_selected[row_id]);
//     JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }





// //     switch(palettelist.ptr->flags.fMapIDs_Type){
// //       case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
// //         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("{f1}#%06x'>%02d %s%s{f2}"),
// //           WebColorFromColourType(set_colours[row_id]),
// //           row_id+1, //to show 1-20 pixel number
// //           GetColourMapNamebyID(colour_selected_id[row_id], buffer),// "??"
// //           ""
// //           // palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?"":"*"
// //         );
// // //149
// //       break;
// //       default:{
// //         int16_t pixel_num = 0;
// //         HsbColor colour = GetColourFromPalette(palettelist.ptr,row_id,&pixel_num);

// //         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
// //           WebColorFromColourType(colour),
// //           row_id+1,//to show 1-20 pixel number
// //           GetColourMapNameNearestbyColour(colour, buffer),// "??"
// //           "*"
// //         );
// //       }
// //       break;

// //     }

//   //}
//       JsonBuilderI->AppendBuffer("%s",PSTR("{t2}"));

//       if(palettelist.ptr->flags.fMapIDs_Type!=MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID){
//         JsonBuilderI->AppendBuffer( PSTR("<tr>%s</tr>"),"* Indicates colour is not from selected options, showing nearest colour.");
//       }

//       if(CheckPaletteIsEditable(palettelist.ptr)){
//         JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE);
//       }else{
//         JsonBuilderI->AppendBuffer("%s",HTTP_FORM_END_NOSAVE);
//       }

//       JsonBuilderI->AppendBuffer("\"");
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();


//   // If I add a delimeter to the name, I can target many different values with the same data
//   //JsonBuilderI->Array_Start("container_1|container_2|container_3");
//   // would all take the same data/ihr value
//   // I need to append actice urls, not erase the old ones, so I can keep static ones eg page refresh top bar
    

//   JsonBuilderI->Array_Start("container_3");//page_button_container");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");
//         pCONT_web->WebAppend_Button_Spaced(BUTTON_MAIN);
//       JsonBuilderI->AppendBuffer("\"");
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();



// }




// void mPWMLight::Web_PaletteEditor_UpdateURLs(AsyncWebServerRequest *request){
  
//   JsonBuilderI->Start();
//     JsonBuilderI->Level_Start("function");
//       JsonBuilderI->Level_Start("Parse_Urls");        
//         JsonBuilderI->Add("/fetch/colour_palette_list_options.json",-1);
//       JsonBuilderI->Level_End();
//     JsonBuilderI->Level_End();
//   JsonBuilderI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }


// void mPWMLight::Web_PaletteEditor_ListOptions(AsyncWebServerRequest *request){
  
//   JsonBuilderI->Start();    
//     WebAppend_PaletteEditor_FillOptions_Controls();
//   JsonBuilderI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }

// void mPWMLight::WebAppend_PaletteEditor_FillOptions_Controls(){


//   char buffer[50];

  
//   JsonBuilderI->Array_Start("g99"); // Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr"); // function
//         JsonBuilderI->AppendBuffer("\"");      
//         for (uint8_t row_id = 0; row_id < PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, 
//             GetPaletteFriendlyNameByID(row_id, buffer)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
  


//   JsonBuilderI->Array_Start("g0"); // Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr"); // function
//         JsonBuilderI->AppendBuffer("\"");      
//         for (uint8_t row_id = 0; row_id < sizeof(PROGMEM pixels_to_update_as_percentage_map); row_id++) {  
//           sprintf(buffer,"%d (%d %%)",
//             GetPixelsToUpdateAsNumberFromPercentage(PROGMEM pixels_to_update_as_percentage_map[row_id]),
//             PROGMEM pixels_to_update_as_percentage_map[row_id]
//           );
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, buffer
//           );
//         }



//   // BufferWriterI->Append_P("\"%s\":[{\"eihr\":\"","g1"); //group rates

//   // HsbColor colour;
//   // char buffer[50];

//   // for (uint8_t item_id = 0; item_id < 80; item_id++) {      //also send none
//   //   colour = GetHsbColour(item_id);
//   //   BufferWriterI->Append_P(HTTP_MODULE_TEMPLATE_REPLACE2,  // Option list with parameter (template,row number,row font colour, row text, row bracket number)
//   //     item_id, 
//   //     WebColorFromColourMap(item_id),
//   //     GetColourMapNamebyID(item_id, buffer)
//   //     #ifdef USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
//   //       ,HueF2N(colour.H),SatF2N(colour.S)
//   //     #endif
//   //   );
//   // }


//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();



  




// }






// void mPWMLight::Web_RGBLightSettings_RunTimeScript(AsyncWebServerRequest *request){

//   uint8_t element_list_num = 9;
//   /***
//    * 0 = Amount number
//             as % 0-10-25-50-75-100 of max leds ie 50% of 50 leds would be 25
//    * 1 = Amount Percentage
//             % 0-10-25-50-75-100
//    * 2 = Animate Order
//             inorder-random-rotate
//    * 3 = Animate Rate/Frequency
//             1-2-3-4-5-10-15-20-30
//    * 4 = Animate Time
//             1-2-3-4-5-10-15-20-30
//    * 5 = Animate Transition Method
//             instant-blend
//    * 6 = Animation Mode
//             presets-flasher-scenes-notifications
//    * 7 = Palette
//             list as before
//    * 8 = Flasher Method
//             Xmas flashers, only show when mode as active
//    * */

//   // find nearest percentage value from its map, get the index (ie 100% is the 10th index)
//   int16_t perc_index = mSupport::FindNearestValueIndexUInt8(PROGMEM pixels_to_update_as_percentage_map,
//                                                   sizeof(PROGMEM pixels_to_update_as_percentage_map),
//                                                   animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.pixels_to_update_as_percentage.val,
//     perc_index,
//     PROGMEM pixels_to_update_as_percentage_map[perc_index],
//     sizeof(PROGMEM pixels_to_update_as_percentage_map)
//   );

//   if(perc_index<0){
//     //not found
//     perc_index = 0;// send 0
//   }

  
  
//   uint8_t rate_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.rate_ms.val/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.rate_ms.val/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );


  
//   uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   animation.transition.time_ms.val/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     animation.transition.time_ms.val/1000,
//     time_index,
//     PROGMEM rate_map_secs[time_index],
//     sizeof(PROGMEM rate_map_secs)
//   );

//     pCONT_web->send_mP(request, 200, CONTENT_TYPE_TEXT_JAVASCRIPT_ID, HTTP_SCRIPT_MODULE_TEMPLATEb,
// // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE_TEMPLATEb);  
  
//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE1b, 
//                               perc_index,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
//                               animation.transition.order_id,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
//                               rate_index,    // "ld('rgb_controls?ra=1',x2);" // ra = rate
//                               time_index,    // "ld('rgb_controls?pr=1',x3);" // pr = period time
//                               animation.transition.method_id,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
//                               animation.mode_id,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
//                               animation.palette_id,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
//                             //   flashersettings.function,                             // "ld('rgb_controls?fl=1',x7);" // fl  = flasher  
//                             //   mixer.running_id                                 // "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id                          
//                             // #else
//                             0,0
//                             ); // Send lists

//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE2cb);

//   }




// void mPWMLight::WebAppend_RGBLightSettings_FillOptions_Controls(){

//   char buffer[50];

//   JsonBuilderI->Array_Start("g0"); // Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr"); // function
//         JsonBuilderI->AppendBuffer("\"");      
//         for (uint8_t row_id = 0; row_id < sizeof(PROGMEM pixels_to_update_as_percentage_map); row_id++) {  
//           sprintf(buffer,"%d (%d %%)",
//             GetPixelsToUpdateAsNumberFromPercentage(PROGMEM pixels_to_update_as_percentage_map[row_id]),
//             PROGMEM pixels_to_update_as_percentage_map[row_id]
//           );
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, buffer
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
  

//   JsonBuilderI->Array_Start("g1");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");
//         for (uint8_t row_id = 0; row_id < TRANSITION_ORDER_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
//             row_id, 
//             GetTransitionOrderNameByID(row_id, buffer)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",2);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
  

//   JsonBuilderI->Array_Start("g2");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");      
//         for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM, 
//             row_id, 
//             PROGMEM rate_map_secs[row_id]
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",3);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();

 
//   JsonBuilderI->Array_Start("g3");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");    
//         for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM, 
//             row_id, 
//             PROGMEM rate_map_secs[row_id]
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",4);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
  

//   JsonBuilderI->Array_Start("g4");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");    
//         for (uint8_t row_id = 0; row_id < TRANSITION_METHOD_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, 
//             GetTransitionMethodNameByID(row_id, buffer)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();


//   JsonBuilderI->Array_Start("g5");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");    
//         for (uint8_t row_id = 0; row_id < ANIMATION_MODE_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, 
//             GetAnimationModeNameByID(row_id, buffer)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();


//   JsonBuilderI->Array_Start("g6");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\"");    
//         for (uint8_t row_id = 0; row_id < PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, 
//             GetPaletteFriendlyNameByID(row_id, buffer)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();


//   #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
//   JsonBuilderI->Array_Start("g7");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\""); 
//         for (uint8_t row_id = 0; row_id < FLASHER_FUNCTION_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
//             row_id, 
//             GetPaletteFriendlyNameByID(row_id)
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();

//   JsonBuilderI->Array_Start("g8");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("eihr");           // function
//         JsonBuilderI->AppendBuffer("\""); 
//         for (uint8_t row_id = 0; row_id < 6; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//           JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
//             row_id, 
//             "running_id"
//           );
//         }
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Add("evl",1);
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
//   #endif

// }



// void mPWMLight::Web_Root_Draw_PaletteSelect(AsyncWebServerRequest *request){
        
//   JsonBuilderI->Start();  
//   JsonBuilderI->Array_Start("rgb_palsel_draw");// Class name
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->AddKey("ihr");           // function
//         JsonBuilderI->AppendBuffer("\"");
//         WebAppend_Root_Draw_PaletteSelect();
//         JsonBuilderI->AppendBuffer("\"");
//       JsonBuilderI->Level_End();
//     JsonBuilderI->Array_End();
//   JsonBuilderI->End();

//   pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mPWMLight::WebAppend_Root_Draw_Table(){

//   WebAppend_Root_Draw_RGBLive();
//   // if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
//     WebAppend_Root_Draw_RGBPalette();
//   // }
//   WebAppend_Root_Draw_PaletteSelect_Placeholder();
//   WebAppend_Root_Draw_RGBTable();
  
// }

// void mPWMLight::WebAppend_Root_Draw_RGBTable(){
//   char listheading[30];
//   BufferWriterI->Append_P("%s",PSTR("{t}"));
//   for(int ii=0;ii<8;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
//       BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"rgb_table"," ");   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P("%s",PSTR("{t2}"));
// }

// void mPWMLight::WebAppend_Root_Draw_RGBLive(){
//   char listheading[30];
//   BufferWriterI->Append_P(PSTR("{t}"));
//   BufferWriterI->Append_P(PSTR("<tr><td> Liveview <span class='%s'>%d</span></td></tr>"), "rgb_live_ttl", liveview.refresh_rate);//GetPaletteFriendlyName(),GetPixelsInMap(palettelist.ptr));
//   BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_live' style='width:100%%;height:%dpx';></div></td></tr>"),
//                             map(liveview.height_as_percentage,0,100,1,300)); //pixel height option for liveview
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }


// void mPWMLight::WebAppend_Root_Draw_PaletteSelect_Placeholder(){
//   BufferWriterI->Append_P(PSTR("{t}"));
//   BufferWriterI->Append_P(PSTR("<tr><div class='%s';></div></tr>"),"rgb_palsel_draw");
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }


// void mPWMLight::WebAppend_Root_Draw_PaletteSelect(){
//   char listheading[30];
//   // BufferWriterI->Append_P("\"%s\":[{\"ihr\":\"","rgb_palsel_draw");  
  
//       // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,"<b>Palette List Select</b>","");

//   // BufferWriterI->Append_P(PSTR("<tr><td><b>Palette List Select</b></td></tr>"));
//   BufferWriterI->Append_P(PSTR("<p><form>"));
//     BufferWriterI->Append_P(PSTR("<select name='pal_set' id='pal_set' onchange='send_value(id);'>"));
//       BufferWriterI->Append_P(PSTR("<optgroup label='User Editable'>"));
      
//       char name_ctr[20];
      
//       for (uint8_t row_id = PALETTELIST_VARIABLE_USER_01_ID; row_id < PALETTELIST_VARIABLE_USER_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//         BufferWriterI->Append_P(
//           PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
//           row_id, 
//           GetPaletteFriendlyNameByID(row_id,name_ctr,20)
//         );
//       }
//       BufferWriterI->Append_P(PSTR("</optgroup>"));
//       BufferWriterI->Append_P(PSTR("<optgroup label='Preset'>"));
//       for (uint8_t row_id = PALETTELIST_VARIABLE_USER_LENGTH_ID; row_id < PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//         BufferWriterI->Append_P(
//           PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
//           row_id, 
//           GetPaletteFriendlyNameByID(row_id)
//         );
//       }
//       BufferWriterI->Append_P(PSTR("</optgroup>"));
//     BufferWriterI->Append_P(PSTR("</select>"));
//     // BufferWriterI->Append_P(PSTR("<br><br>"));
//     // BufferWriterI->Append_P(PSTR("<input type='submit' value='Set'>"));
//   BufferWriterI->Append_P(PSTR("</form></p>"));


// }

// void mPWMLight::WebAppend_Root_Draw_RGBPalette(){
//   char listheading[30];
//   BufferWriterI->Append_P("%s",PSTR("{t}"));
//     BufferWriterI->Append_P(PSTR("<tr><td> Palette: <span class='rgb_pal_title'>?</span></td></tr>"));
//     BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_pal'></div></td></tr>"));       
//   BufferWriterI->Append_P("%s",PSTR("{t2}"));
// }





void mPWMLight::WebAppend_JSON_RootPage_LiveviewPixels()//(AsyncWebServerRequest *request)
{

  uint16_t leds_max_to_show = 2;//STRIP_SIZE_MAX<MAX_LIVE_LEDS?STRIP_SIZE_MAX:MAX_LIVE_LEDS;
  uint8_t number_of_pixels = 1;//map(liveview.pixel_resolution_percentage,0,100,0,STRIP_SIZE_MAX); //only serve every n'th LED if count over MAX_LIVE_LEDS
  uint8_t pixels_to_iter = 1;//((STRIP_SIZE_MAX-1)/number_of_pixels)+1;

  char type_ctr[5];
  // switch(liveview.show_type){
  //   default: 
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID: 
    sprintf(type_ctr,"%s","bclg");  
    // sprintf(type_ctr,"%s","bc");  
  //   break;
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID: sprintf(type_ctr,"%s","bcls");  break;
  // }

  uint8_t r,g,b,cw,ww;

  // if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  // JsonBuilderI->Start();
  JsonBuilderI->Array_Start(WEB_CLASS_RGB_LIVE);// Class name
    JsonBuilderI->Level_Start();

      JsonBuilderI->Array_Start(type_ctr);
      // RgbTypeColor c;
      for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
        // c = GetPixelColorAll(i);

        pCONT_iLight->getActualRGBCW(&r,&g,&b,&cw,&ww);

        // If RGB colours are active, show them
        if((r>0)||(g>0)||(b>0)){
          JsonBuilderI->Add_FP(PSTR("\"%02X%02X%02X\""),r,g,b);
        }else
        // If no colours, check for CCT
        if((cw>0)||(ww>0)){
          //needs fixing
          JsonBuilderI->Add_FP(PSTR("\"%02X%02X%02X\""),cw,ww,0);
        }

      }  
      JsonBuilderI->Array_End();

    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
  // JsonBuilderI->End();

  // pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

}


// void mPWMLight::WebSend_JSON_RootPage_LiveviewPixels(AsyncWebServerRequest *request)
// {

//   uint16_t leds_max_to_show = STRIP_SIZE_MAX<MAX_LIVE_LEDS?STRIP_SIZE_MAX:MAX_LIVE_LEDS;
//   uint8_t number_of_pixels = map(liveview.pixel_resolution_percentage,0,100,0,STRIP_SIZE_MAX); //only serve every n'th LED if count over MAX_LIVE_LEDS
//   uint8_t pixels_to_iter = ((STRIP_SIZE_MAX-1)/number_of_pixels)+1;

//   char type_ctr[5];
//   switch(liveview.show_type){
//     default: 
//     case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID: sprintf(type_ctr,"%s","bclg");  break;
//     case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID: sprintf(type_ctr,"%s","bcls");  break;
//   }

//   if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JsonBuilderI->Start();
//   JsonBuilderI->Array_Start(WEB_CLASS_RGB_LIVE);// Class name
//     JsonBuilderI->Level_Start();

//       JsonBuilderI->Array_Start(type_ctr);
//       RgbTypeColor c;
//       for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
//         c = GetPixelColor(i);
//         JsonBuilderI->Add_FP(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
//       }  
//       JsonBuilderI->Array_End();

//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
//   JsonBuilderI->End();

//   pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// }



// void mPWMLight::WebAppend_Root_RGBPalette()
// {

//   SetPaletteListPtrFromID(animation.palette_id);

//   uint8_t length = GetPixelsInMap(palettelist.ptr); //pixelsinmap + name + length
//   uint8_t pal_length = GetPixelsInMap(palettelist.ptr); //pixelsinmap + name + length
  
//   JsonBuilderI->Array_Start("rgb_pal_title");// Class name
//     JsonBuilderI->Level_Start();
//     char title_ctr[30];
//     if(palettelist.ptr->flags.fMapIDs_Type == MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID){
//       JsonBuilderI->Add_FP("ih",PSTR("\"%s (Gradient)\""),GetPaletteFriendlyNameByID(palettelist.ptr->id,title_ctr,sizeof(title_ctr)));
//     }else{
//       JsonBuilderI->Add_FP("ih",PSTR("\"%s (#%d)\""),GetPaletteFriendlyNameByID(palettelist.ptr->id,title_ctr,sizeof(title_ctr)),GetPixelsInMap(palettelist.ptr));
//     }
//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();

//   char colourtype[5];
//   switch(palette_view.show_type){
//     default:
//     case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID:
//       length = pal_length; //only send title and true length
//       sprintf(colourtype,"%s","bclg");
//     break;
//     case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID:
//       sprintf(colourtype,"%s","bcbl");
//     break;
//   }
  
//   JsonBuilderI->Array_Start("rgb_pal");// Class name
//     JsonBuilderI->Level_Start();

//       JsonBuilderI->Array_Start(colourtype);
//       RgbTypeColor c;
//       int16_t pixel_position = -2;
//       for (uint16_t i= 0; i < length;i++){
//         if(i < pal_length){
//           c = GetColourFromPalette(palettelist.ptr,i,&pixel_position);
//         }else{      
//           c = RgbColor(0,0,0);//default black
//         }
//         JsonBuilderI->Add_FP(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
//       }
//       JsonBuilderI->Array_End();

//     JsonBuilderI->Level_End();
//   JsonBuilderI->Array_End();
  
// }


// void mPWMLight::WebSendBody_Palette(){
//   BufferWriterI->Append_P(PSTR("<div class='%s'></div>"),"rgb_pal_draw"); 
//   BufferWriterI->Append_P(PSTR("<div class='%s'></div>"),"rgb_palsel_draw"); 
// }
// void mPWMLight::WebSendBody_InfoTable(){
//   BufferWriterI->Append_P(PSTR("<div class='%s'></div>"),"rgb_table_draw"); 
// }
// void mPWMLight::WebSendBody_Liveview(){
//   BufferWriterI->Append_P(PSTR("<div class='%s'></div>"),"rgb_live_draw"); 
// }


#endif