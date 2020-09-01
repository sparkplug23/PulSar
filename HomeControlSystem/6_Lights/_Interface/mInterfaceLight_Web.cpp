#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway


int8_t mInterfaceLight::Tasker_Web(uint8_t function){

  switch(function){  
    
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;

    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      // #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // pCONT_ladd->
      WebAppend_Root_Draw_Table();
    
    //  WebAppend_Root_Draw_RGBLive();
      // #endif
    break; 
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
      JsonBuilderI->Add(WEB_HANDLE_LIVEPIXELS_SHARED_JSON,liveview.refresh_rate);
    break;

    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;  
  }
  return 0;

}


void mInterfaceLight::WebAppend_Root_Draw_Table(){

  WebAppend_Root_Draw_RGBLive();
  // if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
    //WebAppend_Root_Draw_RGBPalette();
  // }
  // WebAppend_Root_Draw_PaletteSelect_Placeholder();
  // pCONT_web->WebAppend_Root_Draw_Table_dList(8,"rgb_table", kTitle_TableTitles_Root);
  
}


void mInterfaceLight::WebSend_JSON_RootPage_LiveviewPixels(AsyncWebServerRequest *request)
{

  if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  JsonBuilderI->Start();

    pCONT->Tasker_Interface(FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON);

  // uint16_t leds_max_to_show = STRIP_SIZE_MAX<MAX_LIVE_LEDS?STRIP_SIZE_MAX:MAX_LIVE_LEDS;
  // uint8_t number_of_pixels = map(liveview.pixel_resolution_percentage,0,100,0,STRIP_SIZE_MAX); //only serve every n'th LED if count over MAX_LIVE_LEDS
  // uint8_t pixels_to_iter = ((STRIP_SIZE_MAX-1)/number_of_pixels)+1;

  // char type_ctr[5];
  // switch(liveview.show_type){
  //   default: 
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID: sprintf(type_ctr,"%s","bclg");  break;
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID: sprintf(type_ctr,"%s","bcls");  break;
  // }

  // JsonBuilderI->Array_Start(WEB_CLASS_RGB_LIVE);// Class name
  //   JsonBuilderI->Level_Start();

  //     JsonBuilderI->Array_Start(type_ctr);
  //     RgbTypeColor c;
  //     for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
  //       c = GetPixelColor(i);
  //       JsonBuilderI->Add_FP(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
  //     }  
  //     JsonBuilderI->Array_End();

  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();

  JsonBuilderI->End();

  pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer2.payload.ctr);  

}

void mInterfaceLight::WebPage_Root_AddHandlers(){
  
  /**
   * Root Page 
   * */
  pCONT_web->pWebServer->on(WEB_HANDLE_LIVEPIXELS_SHARED_JSON, [this](AsyncWebServerRequest *request){
    WebSend_JSON_RootPage_LiveviewPixels(request);     
  });


  // pCONT_web->pWebServer->on("/draw/palette_selector.json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Web_Root_Draw_PaletteSelect(request);
  // });



}


void mInterfaceLight::WebAppend_Root_Draw_RGBLive(){
  char listheading[30];
  BufferWriterI->Append_P(PSTR("{t}"));
  BufferWriterI->Append_P(PSTR("<tr><td>Liveview<span class='%s'>%d</span></td></tr>"), "rgb_live_ttl", liveview.refresh_rate);//GetPaletteFriendlyName(),GetPixelsInMap(palettelist.ptr));
  BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_live' style='width:100%%;height:%dpx';></div></td></tr>"),
                            map(liveview.height_as_percentage,0,100,1,300)); //pixel height option for liveview
  BufferWriterI->Append_P(PSTR("{t2}"));
}




void mInterfaceLight::GetPixelColor(uint16_t indexPixel, uint8_t* _r,uint8_t* _g, uint8_t* _b,uint8_t* _w1, uint8_t* _w2)
{

//if colour is ww or cw, still represent in RGB hex code

// if(*_wc != nullptr)

  // DEBUG_LINE;
  // if(stripbus == nullptr){    
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
  //   #endif
  // }
  // RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);
  // RgbTypeColor color_internal = color_hardware; // To catch white element if present
  // DEBUG_LINE;

  // switch (settings.pixel_hardware_color_order_id){
  //   case  PIXEL_HARDWARE_COLOR_ORDER_GRB_ID:  //0 = GRB, default
  //     color_internal.G = color_hardware.G; 
  //     color_internal.R = color_hardware.R; 
  //     color_internal.B = color_hardware.B; 
  //   break;
  //   case  PIXEL_HARDWARE_COLOR_ORDER_RGB_ID:  //1 = RGB, common for WS2811
  //     color_internal.R = color_hardware.G; 
  //     color_internal.G = color_hardware.R; 
  //     color_internal.B = color_hardware.B; 
  //   break;
  //   case  2: color_internal.B = color_hardware.G; color_hardware.R = color_hardware.R; color_internal.G = color_hardware.B; break; //2 = BRG
  //   case  3: color_internal.R = color_hardware.G; color_internal.B = color_hardware.R; color_internal.G = color_hardware.B; break; //3 = RBG
  //   case  4: color_internal.B = color_hardware.G; color_internal.G = color_hardware.R; color_internal.R = color_hardware.B; break; //4 = BGR
  //   default: color_internal.G = color_hardware.G; color_internal.B = color_hardware.R; color_internal.R = color_hardware.B; break; //5 = GBR
  // }

  // return color_internal;
}








void mInterfaceLight::WebCommand_Parse(void)
{
  char tmp[100];

  uint16_t  arg_value = 0;
  char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

  for (int i=0;i<5;i++){
    sprintf(arg_ctr,"pwm%02d\0",i);
    if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
      pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
      arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

      // new_color[i] = arg_value;

      // // use current as start point
      // memcpy(&fade_start_10,&current_color,sizeof(fade_start_10));

        arg_value = map(arg_value,0,1023,0,255);
      switch(i){//}==0){
        default:
        case 0:  setRGB(arg_value,_g,_b); break;
        case 1:  setRGB(_r,arg_value,_b); break;
        case 2:  setRGB(_r,_g,arg_value); break;
      }

      calcLevels();

      // memset(entry_color,0,sizeof(entry_color));

      // entry_color[0] = arg_value;


      // changeChannels(entry_color);

      // if (fade_running) {
      //     // if fade is running, we take the curring value as the start for the next fade
      //     memcpy(fade_start_10, fade_cur_10, sizeof(fade_start_10));
      //   }
      //   // memcpy(fade_end_10, cur_col_10, sizeof(fade_start_10));
      //   fade_running = true;
      //   fade_duration = 0;    // set the value to zero to force a recompute
      //   fade_start = 0;

      // // fade_start_10[i] = arg_value;
      // // fade_running = 1;
      // // UpdateSetOutputs();
      // // RecalculateHsbfromRgb();

      // pCONT_set->power = 1;

    }
  }

  // HSB COLOURS
  sprintf(arg_ctr,"pwm_hue0\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
   // SetColour_Hue(arg_value);
changeHSB_Hue(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }

  sprintf(arg_ctr,"pwm_sat0\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);


arg_value = map(arg_value, 0,100, 0,255);

    changeHSB_Sat(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }

  sprintf(arg_ctr,"pwm_brt\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
arg_value = map(arg_value, 0,100, 0,255);
    changeBriRGB(arg_value);
    
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }


  sprintf(arg_ctr,"pwm_cct\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
    LightSetColorTemp(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }
  
  sprintf(arg_ctr,"pwm_cbrt\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
arg_value = map(arg_value, 0,100, 0,255);
    changeBriCT(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }

  sprintf(arg_ctr,"pwm_light_toggle\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// arg_value = map(arg_value, 0,100, 0,255);
//     changeBriCT(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }


  sprintf(arg_ctr,"cct_rgb_linked\0");
  if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
    pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
    arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// arg_value = map(arg_value, 0,100, 0,255);
    if(isCTRGBLinked()){
        setCTRGBLinked(false);
    }else{
        setCTRGBLinked(true);
    }
    // changeBriCT(arg_value);
    // UpdateSetOutputs();
    // LightSetOutputs2();
  }

}




#endif