#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR
#ifdef USE_MODULE_CORE_WEBSERVER
x
int8_t mAnimatorLight::Tasker_Web(uint8_t function){

  switch(function){    
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:

    break;
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:
      WebAppend_Root_ControlUI();
    break;
    // Generated in "InterfaceLighting" and populated in hardware classes
    case FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON:
      WebAppend_JSON_RootPage_LiveviewPixels();
    break;
  }

  return 0;

}

void mAnimatorLight::WebAppend_Root_ControlUI(){

  char buffer[50];

  BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Animation Mode Select"),"");

  uint8_t animation_mode_list_ids[] = {
    #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
    pCONT_iLight->ANIMATION_MODE_EFFECTS_ID, 
    #endif // ENABLE_PIXEL_FUNCTION_EFFECTS
    //pCONT_iLight->ANIMATION_MODE_SCENE_ID, 
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    ANIMATION_MODE_NOTIFICATIONS_ID,
    #endif    
    pCONT_iLight->ANIMATION_MODE_NONE_ID
  };

  BufferWriterI->Append_P(PSTR("{t}<tr>"));
  for(uint8_t button_id=0;button_id<sizeof(animation_mode_list_ids);button_id++){
    BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2,
                              100/sizeof(animation_mode_list_ids),
                              "", //no span
                              "animod" " " "buttonh",
                              D_JSON_ANIMATIONMODE, 
                              pCONT_iLight->GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
                              pCONT_iLight->GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
                              ""
                            );                   
  }
  BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));
  
}


//append to internal buffer if any root messages table
void mAnimatorLight::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("rgb_table");// Class name
  for(int row=0;row<8;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        case 0: JsonBuilderI->Add("ih",GetAnimationStatusCtr(buffer, sizeof(buffer))); break;
        case 1:
          if(!pCONT_iLight->auto_off_settings.time_decounter_secs){ //off
            JsonBuilderI->Add("ih","Unset");
          }else{
            JsonBuilderI->Add_FV("ih",PSTR("\"%d (%s)\""),
              pCONT_iLight->auto_off_settings.time_decounter_secs,"secs"
            );
          }
        break;
        case 2:
          JsonBuilderI->Add_FV("ih",PSTR("\"%d/%d (secs)\""), 
            pCONT_iLight->animation.transition.rate_ms/1000, 
            pCONT_iLight->animation.transition.time_ms.val/1000
          );
        break;
        case 3: 
          JsonBuilderI->Add_FV("ih",PSTR("\"%d%% [#%d]\""),
            pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val, 
            pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val)
          );
        break;
        case 4: JsonBuilderI->Add("ih",pCONT_iLight->GetTransitionOrderName(buffer, sizeof(buffer))); break;
        case 5: JsonBuilderI->Add("ih",pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));   break;
        #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
        case 6: JsonBuilderI->Add("ih",GetFlasherFunctionName(buffer, sizeof(buffer))); break;
        #endif
        case 7: JsonBuilderI->Add_FV("ih",PSTR("\"%d (%s) | %d (mA)\""), (int)power_rating.power,"W",123); break;
      } //switch
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  

  // char colour_button[8];
  // char button_ids[] = {ANIMATION_MODE_EFFECTS_ID, ANIMATION_MODE_SCENE_ID, 
  // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  // ANIMATION_MODE_NOTIFICATIONS_ID, 
  // #endif
  // ANIMATION_MODE_NONE_ID};
  
  // JsonBuilderI->Array_Start("animod");// Class name
  // for(int row=0;row<sizeof(button_ids);row++){
  //   if(button_ids[row] == pCONT_iLight->animation.mode_id){
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


/*******************************************************************************************************************
********************************************************************************************************************
************ START OF WEBPAGE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// void mAnimatorLight::WebCommand_Parse(void)
// {
//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "mAnimatorLight::WebCommand_Parse"));

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
//         memcpy(&pCONT_iLight->animation,&pCONT_iLight->animation_stored,sizeof(pCONT_iLight->animation));// RESTORE copy of state
//         pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_ON_ID);        // Add this as "SAVE" state then "LOAD" state
//         pCONT_iLight->light_power_state = true;
//       break;
//       default:
//       case 1:
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
//         memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation)); // STORE copy of state
//         pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_OFF_ID);
//         pCONT_iLight->light_power_state = false;
//       break;
//     }

//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   }

//   // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
//   // sprintf_P(arg_ctr,PSTR(WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER));
//   // if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//   //   pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//   //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //   scene.colourW = pCONT_iLight->BrtN2F(arg_value);
//   //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "scene.colourW=%d"),arg_value);

//   //   if(settings.flags.EnableSceneModeWithSliders){
//   //     pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
//   //     scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
//   //   }

//   //   SetRefreshLEDs();
//   // }
//   // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

//   sprintf_P(arg_ctr,PSTR("pal_set"));
//   if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     pCONT_iLight->animation.palette.id = arg_value;
//     SetRefreshLEDs();
//   }

// } //end function


// Group all these websaves into one handler, separate from webcommand (ie consoles)
void mAnimatorLight::WebSave_RGBColourSelector(void)
{
  
//   char tmp[100];

// //  selectorlist.amount = 0;
//   uint8_t  arg_value = 0;

//   char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

//   // check palette selector
//   sprintf(arg_ctr,"g%d\0",99);
//   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     pCONT_iLight->animation.palette.id = arg_value;
//     update_all = true; //refresh all
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   }

//   // Secondary edit location, paste to here
//   //idea: write function that uses id to return pointer to struct
//   SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

//   // Only continue to write if the palette is editable type
//   if(!CheckPaletteIsEditable(mPaletteI->palettelist.ptr)){
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_ERROR "Palette Not editable, leaving websave function"));
//     return;
//   }

//   uint8_t new_amount = 0;
  
//   for(int arg_id=0;arg_id<PRESET_COLOUR_MAP_INDEXES_MAX;arg_id++){
//     sprintf(arg_ctr,"g%d\0",arg_id);
    
//     if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       // Colour Add
//       if(arg_value != COLOUR_MAP_NONE_ID){
//         arg_value = arg_value < PRESET_COLOUR_MAP_INDEXES_MAX ? arg_value : 0; // Check it doesnt exceed array 
//         mPaletteI->palettelist.ptr->colour_map_id[new_amount++] = arg_value;
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d newamount=%d"),arg_ctr,arg_value,new_amount-1);
//       }else{
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%s"),arg_ctr,"COLOUR_MAP_NONE_ID");
//         mPaletteI->palettelist.ptr->colour_map_id[arg_id] = COLOUR_MAP_NONE_ID; //set to unused
//       }

//     }else{
//         //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "SKIPPING hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//         mPaletteI->palettelist.ptr->colour_map_id[arg_id] = COLOUR_MAP_NONE_ID;
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "NOT hasParam(\"%s\")=%s"),arg_ctr,"COLOUR_MAP_NONE_ID");
//     }

//   }

//   // if new amount != 0 ie new values, stored new amount
//   if(new_amount){
//     mPaletteI->palettelist.ptr->colour_map_size = new_amount-1;
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "colour_map_size=%d"),mPaletteI->palettelist.ptr->colour_map_size);
//   }

//   // Erase others
//   for(int erase_id=mPaletteI->palettelist.ptr->colour_map_size;erase_id<PALETTELIST_COLOUR_AMOUNT_MAX;erase_id++){
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "erase_id %d=%s"),erase_id,"COLOUR_MAP_NONE_ID");
//     mPaletteI->palettelist.ptr->colour_map_id[erase_id] = COLOUR_MAP_NONE_ID; //set to unused
//   }

//   memset(tmp,0,sizeof(tmp));
//   sprintf(arg_ctr,"%s\0",WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR);
//   if (pCONT_web->pWebServer->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//     //valid range (change to user "PaletteIsEditable()")
//     if(mPaletteI->palettelist.ptr->id<PALETTELIST_VARIABLE_USER_20_ID){
//       // Copy is variable is string
//       strlcpy(mPaletteI->palettelist.ptr->friendly_name_ctr, 
//         (!strlen(tmp)) ? mPaletteI->palettelist.ptr->friendly_name_ctr : tmp, 
//         sizeof(char)*20
//       );
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%s,%s"),arg_ctr,tmp,mPaletteI->palettelist.ptr->friendly_name_ctr);  
//     }
//   }


//   pCONT_iLight->animation.flags.fForceUpdate = true; // update leds now

}


// void mAnimatorLight::WebSave_RGBControls(AsyncWebServerRequest *request)
// {



//   char tmp[100]; memset(tmp,0,sizeof(tmp));
//   uint8_t  arg_value = 0;
//   char arg_ctr[10]; memset(arg_ctr,0,sizeof(arg_ctr));

//   // check palette selector
//   for(int ii=0;ii<WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE;ii++){
//     sprintf(arg_ctr,"g%d\0",ii);
//     if (request->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       update_all = true; //refresh all
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

//       switch(ii){
//         case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELSUPDATED_PERCENTAGE");
//   //         // from mapped value to real value
//   //         arg_value = PROGMEM pixels_to_update_as_percentage_map[arg_value];
//   //         pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = constrain(arg_value,0,100);
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
//         break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PIXELORDER");
//   //         pCONT_iLight->animation.transition.order_id = arg_value; // no map
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"RATE");
//   //         pCONT_iLight->animation.transition.rate_ms = PROGMEM rate_map_secs[arg_value]*1000; //seconds to milliseconds ra"
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,pCONT_iLight->animation.transition.rate_ms);
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PERIOD");
//   //         pCONT_iLight->animation.transition.time_ms.val = PROGMEM rate_map_secs[arg_value]*1000; //seconds to milliseconds
//   //         // If period > rate, increase period to rate
//   //         pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.time_ms.val>pCONT_iLight->animation.transition.rate_ms?pCONT_iLight->animation.transition.rate_ms:pCONT_iLight->animation.transition.time_ms.val;
//   //         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,pCONT_iLight->animation.transition.time_ms.val); 
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"TRANSITIONMETHOD");
//   //         pCONT_iLight->animation.transition.method_id = arg_value;
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetTransitionMethodName());      
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"ANIMATIONMODE");
//   //         pCONT_iLight->animation.mode_id = arg_value;
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetAnimationModeName());
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"PALETTE");
//   //         pCONT_iLight->animation.palette.id = arg_value;
//   //         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteFriendlyName());
//   //       break;
//   //     #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_EFFECTS: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
//   //         flashersettings.function = arg_value;  
//   //         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;  //restart pCONT_iLight->animation/function
//   //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "flasher.function = %d"),flashersettings.function);
//   //       break;
//   //       case WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %s"),arg_ctr,arg_value,"EFFECTS");
//   //         mixer.running_id = arg_value;          
//   //         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.mode.running_id = %d"),mixer.running_id);
//   //       break;
//   //       #endif
//   //       default: 
//   //         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "DEFAULT hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//   //       break;
//       } //switch
//     } //if
//   } //for

//   pCONT_iLight->animation.flags.fForceUpdate = true; // update leds now

// } // end function


/******** Page items *****
 * 
 * Colour Palette Editor
 * * Selection: Active Palette
 * Colour Palette Editor
 * * Selection: Change Palette
 *  
 * 
 * 
 * ***********************/
void mAnimatorLight::HandlePage_PaletteEditor(AsyncWebServerRequest *request)
{
  //if (!HttpCheckPriviledgedAccess()) { return; }

  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP D_CONFIGURE_LOGGING));

  // if (request->hasParam("save")) {
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
  //   WebSave_RGBColourSelector();
  //   //HandlePage_RGBLightSettings(); //return to main menu
  //   //pCONT_web->HandleRoot(); //return to main menu
  //   return;
  // }

  // D_DATA_BUFFER_CLEAR();
  // char *buf = data_buffer.payload.ctr;
  // char **buffer = &buf;
  // pCONT_web->buffer_writer_len = 0;

  // BuildPage_RGBPaletteEditor(buffer);

  // uint16_t freemem_start = ESP.getFreeHeap();  
  // AddLog_P(LOG_LEVEL_DEBUG,PSTR("WEB: url \"%s\" size [%d] FR %d"),"/",strlen(data_buffer.payload.ctr),freemem_start);

  // pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);

  // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_ASYNC "FreeRam %d"),freemem_start-ESP.getFreeHeap());
  
  // request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,PAGE_ROOT);//PAGE_rgb_colour_palette_editor);

  AsyncWebServerResponse *response = request->beginResponse_P(200, CONTENT_TYPE_TEXT_HTML_ID, PAGE_ROOT, PAGE_ROOT_L);

  response->addHeader("Content-Encoding","gzip");
  
  request->send(response);

//   if (pCONT_web->pWebServer->hasParam("loadpixels")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"loadpixels\")"));
//     WebSave_RGBColourSelectorOnly();
//     // Continue to send page
//   }
  
//   char colour_title_ctr[70];
//   uint8_t colour_id = 0;
//   uint8_t element_list_num = PALETTELIST_COLOUR_AMOUNT_MAX;
//   uint8_t row_id_selected[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
//   uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

//   //GetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
   
//   // LOAD VALUES 
//   uint8_t palette_selected_id = mPaletteI->palettelist.ptr->id;
//   // colours in palette
//   HsbColor set_colours[20];
//   for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
//     set_colours[i] = preset_colour_map[mPaletteI->palettelist.ptr->colour_map_id[i]]; 

// // GETCOLOURSFN

//     //if(mPaletteI->palettelist.ptr->flags.fColours_from_map){
//       colour_selected_id[i] = mPaletteI->palettelist.ptr->colour_map_id[i];    
//     //}else{
//       // USE * for random colours not within colour_map
//     //
//     //}
//   } // inactive colours set elsewhere   
  
//   // "m" = send palette name
//   // Send all the options
//   if (pCONT_web->pWebServer->hasParam("m")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"m\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id < PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//       BufferWriterI->Append_P(
//         HTTP_MODULE_TEMPLATE_REPLACE3, 
//         row_id, 
//         pCONT_iLight->GetPaletteFriendlyNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   // "g" = send colour name
//   // Send all the options
//   if (pCONT_web->pWebServer->hasParam("g")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"g\")"));
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t item_id = 0; item_id < COLOUR_MAP_PALETTE_OPTIONS_END_ID; item_id++) {      //also send none
//       BufferWriterI->Append_P(HTTP_MODULE_TEMPLATE_REPLACE2,  // Option list with parameter (template,row number,row font colour, row text, row bracket number)
//         item_id, 
//         WebColorFromColourMap(item_id),
//         GetColourMapNamebyID(item_id)
//         #ifdef USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
//           ,HueF2N(preset_colour_map[item_id].H),SatF2N(preset_colour_map[item_id].S)
//         #endif
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   /*** 
//    * Send Page WITH fields that have been selected, palette type then colours set
//    * */

//   pCONT_web->WSStartAppend_P(S_COLOUR_PALETTE_EDITOR);
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE_TEMPLATE);
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE1c, palette_selected_id); //active palette

  
//   element_list_num = CheckPaletteIsEditable(mPaletteI->palettelist.ptr) ? 
//         PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(mPaletteI->palettelist.ptr);

//   for (uint8_t menu_id = 0; menu_id < element_list_num; menu_id++) {
//     BufferWriterI->Append_P(PSTR("sk(%d,%d);"),colour_selected_id[menu_id],menu_id); // active colours
//   }
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE2c);
//   pCONT_web->WSContentSendStyle();

  // BufferWriterI->Append_P(HTTP_FORM_RGB_COLOUR_SELECTOR, GetPaletteFriendlyName());

  // BufferWriterI->Append_P(HTTP_FORM_RGB_LOAD_PIXELS);

  // if(CheckPaletteIsEditable(&mPaletteI->palettelist.users[pCONT_iLight->animation.palette.id])){

  //   BufferWriterI->Append_P(HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
  //                             "Edit Name",
  //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id), //previous name
  //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
  //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
  //                             "Enter New Palette Name (maximum 20 characters)",
  //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id)    // News a new "getfriendlyname option" for when user editable names exist
  //                           );

  // }

//     //   mPaletteI->palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?
//     //     GetColourMapNamebyID(colour_selected_id[row_id]):GetEstimatedColourMapNamebyColour()
      
      
      
//     // ID(colour_selected_id[row_id]),
  
//   // if not users, then only show number of static fields
//   element_list_num = CheckPaletteIsEditable(mPaletteI->palettelist.ptr) ? 
//         PALETTELIST_COLOUR_AMOUNT_MAX : GetPixelsInMap(palettelist.ptr);

//   // Send Table
//   BufferWriterI->Append_P(PSTR("<br/><p></p><b>" "Pixel Colour (Saturation %)" "</b><table>"));
//   for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {

//     switch(palettelist.ptr->flags.fMapIDs_Type){
//       case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
//         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
//           WebColorFromColourType(set_colours[row_id]),
//           row_id+1, //to show 1-20 pixel number
//           GetColourMapNamebyID(colour_selected_id[row_id]),// "??"
//           ""
//           // palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?"":"*"
//         );

//       break;
//       default:{
//         int16_t pixel_num = 0;
//         HsbColor colour = GetColourFromPalette(palettelist.ptr,row_id,&pixel_num);

//         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
//           WebColorFromColourType(colour),
//           row_id+1,//to show 1-20 pixel number
//           GetColourMapNameNearestbyColour(colour),// "??"
//           "*"
//         );
//       }
//       break;

//     }


//     BufferWriterI->Append_P(
//       PSTR("<tr><td style='width:200px'><b>%s</b></td><td style='width:216px'><select id='g%d' name='g%d'></select></td></tr>"), //190/176 to 120/346
//         colour_title_ctr, 
//         row_id_selected[row_id], 
//         row_id_selected[row_id]
//     );
//   }
//   BufferWriterI->Append_P(PSTR("</table>"));
//   if(palettelist.ptr->flags.fMapIDs_Type!=MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID){
//     BufferWriterI->Append_P(PSTR("<tr>%s</tr>"),"* Indicates colour is not from selected options, showing nearest colour.");
//   }

//   // BufferWriterI->Append_P(HTTP_FORM_RGB_SELECTOR2, pCONT_set->Settings.syslog_host, pCONT_set->Settings.syslog_port, pCONT_set->Settings.tele_period);
      
      
//   if(CheckPaletteIsEditable(palettelist.ptr)){
//     BufferWriterI->Append_P(HTTP_FORM_END);
//   }else{
//     BufferWriterI->Append_P(HTTP_FORM_END_NOSAVE);
//   }

//   pCONT_web->WSContentSpaceButton(BUTTON_MAIN);
//   pCONT_web->WSContentStop();
}




// /**
//  * Send minimal script
//  * Send minimal style of page
//  * Send all needed script asks
//  * Send all needed javascript asks 
//  * */
// void mAnimatorLight::BuildPage_RGBPaletteEditor(char** buffer)
// {
  
//   char colour_title_ctr[70];
//   uint8_t colour_id = 0;
//   uint8_t element_list_num = 2;//PALETTELIST_COLOUR_AMOUNT_MAX;
//   uint8_t row_id_selected[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
//   uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
//   for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

//   //GetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
   
//   // LOAD VALUES 
//   uint8_t palette_selected_id = mPaletteI->palettelist.ptr->id;
//   // colours in palette
//   HsbColor set_colours[20];
//   for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
//     set_colours[i] = HsbColor(0);//preset_colour_map[palettelist.ptr->colour_map_id[i]]; 

// // GETCOLOURSFN

//     //if(palettelist.ptr->flags.fColours_from_map){
//       colour_selected_id[i] = mPaletteI->palettelist.ptr->colour_map_id[i];    
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
//                                                   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val,
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
//                                                   pCONT_iLight->animation.transition.rate_ms/1000
//                                                  );
                                         
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.rate_ms/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );

//   uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   pCONT_iLight->animation.transition.time_ms.val/1000
//                                                  );
                                            
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.time_ms.val/1000,
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
  
//   // // if(CheckPaletteIsEditable(&palettelist.users[pCONT_iLight->animation.palette.id])){

//   // //   pCONT_web->WSBufferAppend_P2(buffer, HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
//   // //                             "Edit Name",
//   // //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id), //previous name
//   // //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   // //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
//   // //                             "Enter New Palette Name (maximum 20 characters)",
//   // //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id)    // News a new "getfriendlyname option" for when user editable names exist
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




void mAnimatorLight::HandlePage_RGBLightSettings(AsyncWebServerRequest *request)
{
  // if (!HttpCheckPriviledgedAccess()) { return; }

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandlePage_RGBLightSettings"));
    #endif// ENABLE_LOG_LEVEL_INFO
  
  // if (HandleParameters_RGBLightSettings(request)) {
  //   AddLog_P(LOG_LEVEL_TEST,PSTR("HandleParameters_RGBLightSettings(request) FOUND SO STOP"));
  //   return;
  // }
  
    // if (pCONT_web->pWebServer->hasParam("save")) {
    //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"save\")"));
    //   WebSave_RGBControls(request);
    //   pCONT_web->HandlePage_Root(); // return to main menu
    //   return;
    // }
  
  // request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,PAGE_ROOT);

  AsyncWebServerResponse *response = request->beginResponse_P(200, CONTENT_TYPE_TEXT_HTML_ID, PAGE_ROOT, PAGE_ROOT_L);

  response->addHeader("Content-Encoding","gzip");
  
  request->send(response);
}



// /**
//  * Send minimal script
//  * Send minimal style of page
//  * Send all needed script asks
//  * Send all needed javascript asks 
//  * */
// void mAnimatorLight::BuildPage_RGBLightSettings(char** buffer)
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
//                                                   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val,
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
//                                                   pCONT_iLight->animation.transition.rate_ms/1000
//                                                  );
                                         
//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//   //   pCONT_iLight->animation.transition.rate_ms/1000,
//   //   rate_index,
//   //   PROGMEM rate_map_secs[rate_index],
//   //   sizeof(PROGMEM rate_map_secs)
//   // );

//   // uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//   //                                                 sizeof(PROGMEM rate_map_secs),
//   //                                                 pCONT_iLight->animation.transition.time_ms.val/1000
//   //                                                );
                                            
//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//   //   pCONT_iLight->animation.transition.time_ms.val/1000,
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



void mAnimatorLight::HandleParameters_RGBLightSettings(AsyncWebServerRequest *request)
{

  
}

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF WEBPAGE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF ANIMATION/EFFECTS/MIXER DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS

// /******** Page items *****
//  * 
//  * Colour Palette Editor
//  * * Selection: Active Palette
//  * Colour Palette Editor
//  * * Selection: Change Palette
//  *  
//  * 
//  * ***********************/
// void mAnimatorLight::HandleMixerEditor(void)
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

//   SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
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
//         pCONT_iLight->GetPaletteFriendlyNameByID(row_id)
//       );
//     }
//     pCONT_web->WSContentEnd();
//     return;
//   }

//   if (pCONT_web->pWebServer->hasParam("fl")) {
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pWebServer->hasParam(\"%s\")"),"fl");
//     pCONT_web->WSContentBegin(200, CT_PLAIN);
//     for (uint8_t row_id = 0; row_id <= EFFECTS_FUNCTION_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
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
//                                                   pCONT_iLight->animation.transition.pixels_to_update_as_percentage
//                                                  );

//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.pixels_to_update_as_percentage,
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
//                                                   pCONT_iLight->animation.transition.rate_ms/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.rate_ms/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );


//   pCONT_web->WSStartAppend_P("LED Light Controls");
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE_TEMPLATE_MIXER_EDITOR);
//   BufferWriterI->Append_P(HTTP_SCRIPT_MODULE1_MIXER_EDITOR, 
//                               perc_index,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
//                               pCONT_iLight->animation.transition.order_id,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
//                               rate_index,    // "ld('rgb_controls?ra=1',x2);" // ra = rate
//                               (uint8_t)round(pCONT_iLight->animation.transition.time_ms/1000),    // "ld('rgb_controls?pr=1',x3);" // pr = period time
//                               pCONT_iLight->animation.transition.method_id,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
//                               pCONT_iLight->animation.mode_id,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
//                               pCONT_iLight->animation.palette.id,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
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






// void mAnimatorLight::WebPage_Root_SendStyle(){

 
 

// }

// void mAnimatorLight::WebPage_Root_SendBody(){

//   WebSendBody_Liveview();
//   WebSendBody_Palette();
//   WebSendBody_InfoTable(); //send_body_tables = new type so sensors add here?

// }


// void mAnimatorLight::WebPage_Root_SendStatus(){

//   /****
//    *  Show LED power 
//    * ****/
//   if(pCONT_iLight->liveview.show_type == RGB_VIEW_SHOW_TYPE_NONE_ID){
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





void mAnimatorLight::WebPage_Root_SendInformationModule(){

  #ifdef ADA_TEST
    char str[10];
    memset(str,0,sizeof(str));
    dtostrf(_ADALIGHTNEOPIXEL_H, 4, 2, str );

    BufferWriterI->Append_P(PSTR("}1" D_MODULE_LIGHTS_ADDRESSABLE_CTR "}2%s%s"), "Version ", str);
    BufferWriterI->Append_P(PSTR("}1" "Version" "}2%s"), str);
  #endif

}


void mAnimatorLight::WebPage_Root_AddHandlers(){
  
  /**
   * Root Page 
   * */
  // pCONT_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
  //   WebSend_JSON_RootPage_LiveviewPixels(request); 
  // });
  
  
  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
  /**
   * RGB LIGHT SETTINGS
   * */
  pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS_PAGE, [this](AsyncWebServerRequest *request){ //"/page"
    HandlePage_RGBLightSettings(request); 
  });
//   pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", [this](AsyncWebServerRequest *request){ //"/page"
//     // HandlePage_RGBLightSettings(request); 

//     Serial.printf("GET\n\r\n\r\n\r\n\r\n\r\n\r");

//     // request->client->

// //  char* buffertemp = request->_tempObject;

// // WebSave_RGBControls(request);
// // int params = request->params();
// // for(int i=0;i<params;i++){
// //   AsyncWebParameter* p = request->getParam(i);
// //   if(p->isFile()){ //p->isPost() is also true
// //     Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
// //   } else if(p->isPost()){
// //     Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
// //   } else {
// //     Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
// //   }
// // }


//   request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,"PAGE_ROOT");//PAGE_rgb_colour_palette_editor);


//   });

  //D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls"
  // pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", HTTP_POST, [this](AsyncWebServerRequest *request){


  //   int params = request->params();
  //   for(int i=0;i<params;i++){
  //     AsyncWebParameter* p = request->getParam(i);
  //     if(p->isFile()){
  //       Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
  //     } else 
  //     if(p->isPost()){
  //       Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
  //     }
  //      else {
  //       Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
  //     }
  //   }

  //   request->send(200, CONTENT_TYPE_TEXT_HTML_ID, "end");

  //   //  if (request->method() == HTTP_POST && request->url() == "/michael") {
  //   //    Serial.println("HTTP_POST && request->url()");
  //   //   // Shoudl be already handled by handleBody(..) at this point.
  //   //   return;
  //   // }
  //   // Web_Base_Page_Draw(request);
  // });
  #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES

  pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls", HTTP_POST, [](AsyncWebServerRequest *request){
    //nothing and dont remove it
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.println("onRequestBody " D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls" );
    if ((request->url() == D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls") && //"/rest/api/v2/test") &&
        (request->method() == HTTP_POST))
    {
      
#ifdef ENABLE_DEVFEATURE_ARDUINOJSON
        const size_t        JSON_DOC_SIZE   = 512U;
        DynamicJsonDocument jsonDoc(JSON_DOC_SIZE);
        
        if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char*)data))
        {
            JsonObject obj = jsonDoc.as<JsonObject>();

    #ifndef ENABLE_DEVFEATURE_JSONPARSER
            pCONT_ladd->parse_JSONCommand(jsonDoc.as<JsonObjectConst>());
    #endif // ENABLE_DEVFEATURE_JSONPARSER

            for (JsonPair keyValue : obj) {
              // AddLog_P(LOG_LEVEL_INFO, PSTR("key[\"%s\"]=%s"),keyValue.key().c_str(),keyValue.value().as<char*>());
              if(keyValue.value().as<char*>()){
                Serial.print(keyValue.key().c_str()); Serial.print("\t"); Serial.println(keyValue.value().as<char*>());
              }else{
                Serial.print(keyValue.key().c_str()); Serial.print("\t"); Serial.println(keyValue.value().as<int>());
              }
            }

        }
        
        request->send(200, CONTENT_TYPE_APPLICATION_JSON_ID, "{ \"status\": 0 }");
        
#endif // ENABLE_DEVFEATURE_ARDUINOJSON
    }
  });

  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
  pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    Web_RGBLightSettings_Draw(request);
  });
  pCONT_web->pWebServer->on(D_WEB_HANDLE_RGB_CONTROLS "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    Web_RGBLightSettings_UpdateURLs(request);
  }); 
  pCONT_web->pWebServer->on("/fetch/animation_control_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    Web_RGBLightSettings_ListOptions(request);
  }); 


  /**
   * PALETTE EDTIOR
   * */
  pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_PAGE_CTR, [this](AsyncWebServerRequest *request){ //"/page"
    HandlePage_PaletteEditor(request); 
  });
  pCONT_web->pWebServer->on("/fetch/colour_palette_list_options.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    Web_PaletteEditor_ListOptions(request);
  }); 
  pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_CTR "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    Web_PaletteEditor_UpdateURLs(request);
  }); 
  pCONT_web->pWebServer->on(D_WEB_HANDLE_PALETTE_EDITOR_CTR "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    Web_PaletteEditor_Draw(request);
  });
  #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES


} //add handlers


  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
void mAnimatorLight::Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request){
  
  JsonBuilderI->Start();
    JsonBuilderI->Level_Start("function");
      JsonBuilderI->Level_Start("Parse_Urls");        
        JsonBuilderI->Add("/fetch/animation_control_list_options.json",-1);
      JsonBuilderI->Level_End();
    JsonBuilderI->Level_End();
  JsonBuilderI->End();

  request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
}

void mAnimatorLight::Web_RGBLightSettings_ListOptions(AsyncWebServerRequest *request){
  
  JsonBuilderI->Start();    
    WebAppend_RGBLightSettings_FillOptions_Controls();
  JsonBuilderI->End();

  request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
}



void mAnimatorLight::Web_RGBLightSettings_Draw(AsyncWebServerRequest *request){
        
  if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  JsonBuilderI->Start();
    WebAppend_RGBLightSettings_Draw_Animation_Options();
    WebAppend_RGBLightSettings_Draw_Flasher_Options();
    WebAppend_RGBLightSettings_Draw_PageButtons();
  JsonBuilderI->End();

  pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

} //end function


void mAnimatorLight::WebAppend_RGBLightSettings_Draw_Animation_Options(){

  // create command list
  char dlist[200]; memset(dlist,0,sizeof(dlist));
  pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_PIXELS_UPDATE_PERCENTAGE);
  pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_ORDER);
  pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_RATE);
  pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_TIME);
  pCONT_sup->AppendDList(dlist, D_JSON_TRANSITION "," D_JSON_METHOD);
  pCONT_sup->AppendDList(dlist, D_JSON_MODE);

  JsonBuilderI->Array_Start("container_1");//animation_options_container");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");

        JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_START_VARIABLE_JSON, ("Animation Controls"), "json_command_form", D_WEB_HANDLE_RGB_CONTROLS "/save_animation_controls");
          char dlist_result[40];
          char title_ctr[70];
          uint8_t element_list_num = 6;
          uint8_t row_id_selected[element_list_num];
          for(int j=0;j<element_list_num;j++){row_id_selected[j] = j;}

          JsonBuilderI->AppendBuffer(PSTR("{t}"));
          for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
            snprintf_P(title_ctr, sizeof(title_ctr), pCONT_sup->GetTextIndexed_P(dlist_result, sizeof(dlist_result), row_id, kTitle_TableTitles_Root));
            JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
              JsonBuilderI->AppendBuffer(PSTR("<td style='width:200px'><b>%s</b></td>"),title_ctr);
              JsonBuilderI->AppendBuffer(PSTR("<td style='width:216px'><select id='g%d' name='%s'></select></td>"),
                row_id_selected[row_id],
                pCONT_sup->GetTextIndexed_P(dlist_result, sizeof(dlist_result), row_id, dlist)  // should this be _P?
              );
            JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
          }
          JsonBuilderI->AppendBuffer(PSTR("{t2}"));

        JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE_VARIABLE_JSON, "bgrn", ("Save"));

      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  JsonBuilderI->Array_Start("function");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("Parse_AddScript");
        JsonBuilderI->AppendBuffer("\"");
        JsonBuilderI->AppendBuffer(PSTR(
          "start_form_listener();"
        ));
      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


}

void mAnimatorLight::WebAppend_RGBLightSettings_Draw_Flasher_Options(){

  JsonBuilderI->Array_Start("container_2");//flasher_options_container");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");
        // pCONT->Tasker_Interface(FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER);
      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
}

void mAnimatorLight::WebAppend_RGBLightSettings_Draw_PageButtons(){

  JsonBuilderI->Array_Start("container_3");//page_button_container");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");
        pCONT_web->WebAppend_Button_Spaced(PM_BUTTON_NAME_RGB_PALETTE_EDITOR_CTR, D_WEB_HANDLE_PALETTE_EDITOR_PAGE_CTR);
        pCONT_web->WebAppend_Button(PM_BUTTON_NAME_RGB_ANIMATION_MIXER_EDITOR_CTR, PM_WEB_HANDLE_RGB_ANIMATION_MIXER_EDITOR_CTR);
        // pCONT_web->WebAppend_Button_Spaced(BUTTON_MAIN);
      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();

}

#endif // USE_WEBSERVER_ADVANCED_MULTIPAGES


/***************
 * 
 * 
 * 
 * Palette editor: Page x/y
 * 
 * 
 * *********/

#ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES

void mAnimatorLight::Web_PaletteEditor_Draw(AsyncWebServerRequest *request){
        
  if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  JsonBuilderI->Start();
    WebAppend_PaletteEditor_Draw_Editor_Form();
  JsonBuilderI->End();

  pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

} //end function


void mAnimatorLight::WebAppend_PaletteEditor_Draw_Editor_Form(){


  JsonBuilderI->Array_Start("container_1");//animation_options_container");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");

        JsonBuilderI->AppendBuffer(PM_HTTP_FORM_START_VARIABLE,PSTR("Colour Palette Editor"),PSTR("rgb_pal"));
          char listheading[25];
          char title_ctr[70];
          uint8_t element_list_num = 6;
          uint8_t row_id_selected[element_list_num];
          for(int j=0;j<element_list_num;j++){row_id_selected[j] = j;}

          // JsonBuilderI->AppendBuffer(PSTR("{t}"));
          // for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
          //   snprintf_P(title_ctr, sizeof(title_ctr), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), row_id, kTitle_ListHeadings));
          //   JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
          //     JsonBuilderI->AppendBuffer(PSTR("<td style='width:200px'><b>%s</b></td>"),title_ctr);
          //     JsonBuilderI->AppendBuffer(PSTR("<td style='width:216px'><select id='g%d' name='g%d'></select></td>"),row_id_selected[row_id],row_id_selected[row_id]);
          //   JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
          // }
          // JsonBuilderI->AppendBuffer(PSTR("{t2}"));

  // char listheading[25];
 
  char buffer[50];

  char colour_title_ctr[70];
  uint8_t colour_id = 0;
  element_list_num = 2;
  for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){row_id_selected[j] = j;}
  uint8_t colour_selected_id[PALETTELIST_COLOUR_AMOUNT_MAX];
  for(int j=0;j<PALETTELIST_COLOUR_AMOUNT_MAX;j++){colour_selected_id[j] = random(0,PALETTELIST_COLOUR_AMOUNT_MAX);}

  //GetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  
  // LOAD VALUES 
  uint8_t palette_selected_id = mPaletteI->palettelist.ptr->id;
  // colours in palette
  HsbColor set_colours[20];
  for(int i=0;i<PALETTELIST_COLOUR_AMOUNT_MAX;i++){ 
    set_colours[i] = HsbColor(0);//preset_colour_map[mPaletteI->palettelist.ptr->colour_map_id[i]]; 
    //if(mPaletteI->palettelist.ptr->flags.fColours_from_map){
      colour_selected_id[i] = mPaletteI->palettelist.ptr->colour_map_id[i];    
    //}else{
      // USE * for random colours not within colour_map
    //
    //}
  } // inactive colours set elsewhere   

  
  JsonBuilderI->AppendBuffer(PSTR("<b>" "Select Palette" "</b> (Active: %s)<br/>"), pCONT_iLight->GetPaletteFriendlyName(buffer, sizeof(buffer)));
  JsonBuilderI->AppendBuffer(PSTR("<select id='g99' name='g99'></select><br/>"));
  pCONT_web->WebAppend_Button2(PSTR("Load Palette"),"loadpixels","bora");


  // if(pCONT_iLight->CheckPaletteIsEditable(&mPaletteI->palettelist.users[pCONT_iLight->animation.palette.id])){
  //   JsonBuilderI->AppendBuffer(HTTP_FORM_TEXTBOX_EDIT_VARIABLE_HANDLE, 
  //                             PSTR("Edit Name"),
  //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id), //previous name
  //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
  //                             WEB_HANDLE_RGB_COLOUR_PALETTE_NAME_EDITOR,
  //                             PSTR("Enter New Palette Name (maximum 20 characters)"),
  //                             pCONT_iLight->GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id)    // News a new "getfriendlyname option" for when user editable names exist
  //                           );
  // }

  //create json function/command that "sets select to value"

  JsonBuilderI->AppendBuffer(
    PSTR(
      "<br/>"
        "<p></p>"
        "<b>" "Pixel Colour (Saturation %%)" "</b>"
    )
  );

  JsonBuilderI->AppendBuffer("%s",PSTR("{t}"));

  // if not users, then only show number of static fields
  element_list_num = pCONT_iLight->CheckPaletteIsEditable(mPaletteI->palettelist.ptr) ? 
        PALETTELIST_COLOUR_AMOUNT_MAX : mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);

        element_list_num = 20;

  for (uint8_t row_id = 0; row_id < element_list_num; row_id++) {
    snprintf_P(title_ctr, sizeof(title_ctr), "%02d None", row_id+1);
    JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_START_0V);
      JsonBuilderI->AppendBuffer(PSTR("{sw}200px'><b>%s</b></td>"),title_ctr);
      JsonBuilderI->AppendBuffer(PSTR("{sw}216px'>{si}g%d' name='collist g%d{si2}"),row_id_selected[row_id],row_id_selected[row_id]);
    JsonBuilderI->AppendBuffer(PM_WEBAPPEND_TABLE_ROW_END_0V);
  }





//     switch(palettelist.ptr->flags.fMapIDs_Type){
//       case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
//         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("{f1}#%06x'>%02d %s%s{f2}"),
//           WebColorFromColourType(set_colours[row_id]),
//           row_id+1, //to show 1-20 pixel number
//           GetColourMapNamebyID(colour_selected_id[row_id], buffer),// "??"
//           ""
//           // palettelist.ptr->flags.fMapIDs_Type==MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID?"":"*"
//         );
// //149
//       break;
//       default:{
//         int16_t pixel_num = 0;
//         HsbColor colour = GetColourFromPalette(palettelist.ptr,row_id,&pixel_num);

//         snprintf_P(colour_title_ctr, sizeof(colour_title_ctr), PSTR("<font color='#%06x'>%02d %s%s</font>"),
//           WebColorFromColourType(colour),
//           row_id+1,//to show 1-20 pixel number
//           GetColourMapNameNearestbyColour(colour, buffer),// "??"
//           "*"
//         );
//       }
//       break;

//     }

  //}
      JsonBuilderI->AppendBuffer("%s",PSTR("{t2}"));

      if(mPaletteI->palettelist.ptr->flags.fMapIDs_Type!=MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID){
        JsonBuilderI->AppendBuffer( PSTR("<tr>* Indicates colour is not from selected options, showing nearest colour.</tr>"));
      }

      if(pCONT_iLight->CheckPaletteIsEditable(mPaletteI->palettelist.ptr)){
        JsonBuilderI->AppendBuffer(PM_HTTP_FORM_SELECTOR_END_WITH_SAVE);
      }else{
        JsonBuilderI->AppendBuffer("%s",HTTP_FORM_END_NOSAVE);
      }

      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  // If I add a delimeter to the name, I can target many different values with the same data
  //JsonBuilderI->Array_Start("container_1|container_2|container_3");
  // would all take the same data/ihr value
  // I need to append actice urls, not erase the old ones, so I can keep static ones eg page refresh top bar
    

  JsonBuilderI->Array_Start("container_3");//page_button_container");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");
        pCONT_web->WebAppend_Button_Spaced(BUTTON_MAIN);
      JsonBuilderI->AppendBuffer("\"");
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();



}




void mAnimatorLight::Web_PaletteEditor_UpdateURLs(AsyncWebServerRequest *request){
  
  JsonBuilderI->Start();
    JsonBuilderI->Level_Start("function");
      JsonBuilderI->Level_Start("Parse_Urls");        
        JsonBuilderI->Add("/fetch/colour_palette_list_options.json",-1);
      JsonBuilderI->Level_End();
    JsonBuilderI->Level_End();
  JsonBuilderI->End();

  request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
}


void mAnimatorLight::Web_PaletteEditor_ListOptions(AsyncWebServerRequest *request){
  
  JsonBuilderI->Start();    
    WebAppend_PaletteEditor_FillOptions_Controls();
  JsonBuilderI->End();

  request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
}

void mAnimatorLight::WebAppend_PaletteEditor_FillOptions_Controls(){


  char buffer[50];

  
  JsonBuilderI->Array_Start("g99"); // Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("eihr"); // function
        JsonBuilderI->AppendBuffer("\"");      
        for (uint8_t row_id = 0; row_id < pCONT_iLight->PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
          JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
            row_id, 
            pCONT_iLight->GetPaletteFriendlyNameByID(row_id, buffer, sizeof(buffer))
          );
        }
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Add("evl",1);
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
  


  // JsonBuilderI->Array_Start("g0"); // Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr"); // function
  //       JsonBuilderI->AppendBuffer("\"");      
  //       for (uint8_t row_id = 0; row_id < sizeof(PROGMEM pixels_to_update_as_percentage_map); row_id++) {  
  //         sprintf(buffer,"%d (%d %%)",
  //           GetPixelsToUpdateAsNumberFromPercentage(PROGMEM pixels_to_update_as_percentage_map[row_id]),
  //           PROGMEM pixels_to_update_as_percentage_map[row_id]
  //         );
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
  //           row_id, buffer
  //         );
  //       }

  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",1);
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();



  // BufferWriterI->Append_P("\"%s\":[{\"eihr\":\"","g1"); //group rates

  // HsbColor colour;
  // char buffer[50];

  // for (uint8_t item_id = 0; item_id < 80; item_id++) {      //also send none
  //   colour = GetHsbColour(item_id);
  //   BufferWriterI->Append_P(HTTP_MODULE_TEMPLATE_REPLACE2,  // Option list with parameter (template,row number,row font colour, row text, row bracket number)
  //     item_id, 
  //     WebColorFromColourMap(item_id),
  //     GetColourMapNamebyID(item_id, buffer)
  //     #ifdef USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
  //       ,HueF2N(colour.H),SatF2N(colour.S)
  //     #endif
  //   );
  // }




  




}


#endif // USE_WEBSERVER_ADVANCED_MULTIPAGES




  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES

void mAnimatorLight::Web_RGBLightSettings_RunTimeScript(AsyncWebServerRequest *request){

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
//                                                   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val
//                                                  );

//                                                  // ADD getpercetnage that checks flag to mode its in?

//   // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//   //   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val,
//   //   perc_index,
//   //   PROGMEM pixels_to_update_as_percentage_map[perc_index],
//   //   sizeof(PROGMEM pixels_to_update_as_percentage_map)
//   // );

//   if(perc_index<0){
//     //not found
//     perc_index = 0;// send 0
//   }

  
  
//   uint8_t rate_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   pCONT_iLight->animation.transition.rate_ms/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.rate_ms/1000,
//     rate_index,
//     PROGMEM rate_map_secs[rate_index],
//     sizeof(PROGMEM rate_map_secs)
//   );


  
//   uint8_t time_index = mSupport::FindNearestValueIndexUInt8(PROGMEM rate_map_secs,
//                                                   sizeof(PROGMEM rate_map_secs),
//                                                   pCONT_iLight->animation.transition.time_ms.val/1000
//                                                  );

                                                 
//   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_HTTP "Searched for %d and got index %d for mapped value %d sizeof(%d)"),
//     pCONT_iLight->animation.transition.time_ms.val/1000,
//     time_index,
//     PROGMEM rate_map_secs[time_index],
//     sizeof(PROGMEM rate_map_secs)
//   );

//     // pCONT_web->send_mP(request, 200, CONTENT_TYPE_TEXT_JAVASCRIPT_ID, HTTP_SCRIPT_MODULE_TEMPLATEb,
// // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE_TEMPLATEb);  
  
//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE1b, 
//                             //   perc_index,  // "ld('rgb_controls?up=1',x0);" // up = update percentage
//                             //   pCONT_iLight->animation.transition.order_id,                           // "ld('rgb_controls?po=1',x1);" // po = pixel order
//                             //   rate_index,    // "ld('rgb_controls?ra=1',x2);" // ra = rate
//                             //   time_index,    // "ld('rgb_controls?pr=1',x3);" // pr = period time
//                             //   pCONT_iLight->animation.transition.method_id,                          // "ld('rgb_controls?tm=1',x4);" // tm = transition mode
//                             //   pCONT_iLight->animation.mode_id,                                       // "ld('rgb_controls?am=1',x5);" // am = animaiton mode
//                             //   pCONT_iLight->animation.palette.id,                                    // "ld('rgb_controls?pa=1',x6);" // pa  = palette
//                             // //   flashersettings.function,                             // "ld('rgb_controls?fl=1',x7);" // fl  = flasher  
//                             // //   mixer.running_id                                 // "ld('rgb_controls?mi=1',x8);" // mi  = mixer running id                          
//                             // // #else
//                             // 0,0
//                             // ); // Send lists

//   // pCONT_web->WSBufferAppend_P2(buffer, HTTP_SCRIPT_MODULE2cb);

  }




void mAnimatorLight::WebAppend_RGBLightSettings_FillOptions_Controls(){

  char buffer[50];
  char buffer2[50];

  // JsonBuilderI->Array_Start("g0"); // Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr"); // function
  //       JsonBuilderI->AppendBuffer("\"");      
  //       for (uint8_t row_id = 0; row_id < sizeof(PROGMEM pixels_to_update_as_percentage_map); row_id++) {  
  //         sprintf_P(buffer,PSTR("%d (%d %%)"),
  //           GetPixelsToUpdateAsNumberFromPercentage(PROGMEM pixels_to_update_as_percentage_map[row_id]),
  //           PROGMEM pixels_to_update_as_percentage_map[row_id]
  //         );
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_CTR,
  //           GetPixelsToUpdateAsNumberFromPercentage(PROGMEM pixels_to_update_as_percentage_map[row_id]), 
  //           buffer
  //         );
  //       }
  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val));
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();


  JsonBuilderI->Array_Start("g1");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("eihr");           // function
        JsonBuilderI->AppendBuffer("\"");
        for (uint8_t row_id = 0; row_id < TRANSITION_ORDER_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
          JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR, 
            pCONT_iLight->GetTransitionOrderNameByID(row_id, buffer, sizeof(buffer)), 
            pCONT_iLight->GetTransitionOrderNameByID(row_id, buffer2, sizeof(buffer2))
          );
        }
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Add("evl", pCONT_iLight->GetTransitionOrderName(buffer, sizeof(buffer)));
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
  

  // JsonBuilderI->Array_Start("g2");// Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr");           // function
  //       JsonBuilderI->AppendBuffer("\"");      
  //       for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_NUM, 
  //           PROGMEM rate_map_secs[row_id], 
  //           PROGMEM rate_map_secs[row_id]
  //         );
  //       }
  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",(uint16_t)round(pCONT_iLight->animation.transition.rate_ms/1000));
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();

 
  // JsonBuilderI->Array_Start("g3");// Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr");           // function
  //       JsonBuilderI->AppendBuffer("\"");    
  //       for (uint8_t row_id = 0; row_id < sizeof(PROGMEM rate_map_secs); row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_NUM, 
  //           PROGMEM rate_map_secs[row_id], 
  //           PROGMEM rate_map_secs[row_id]
  //         );
  //       }
  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",(uint16_t)round(pCONT_iLight->animation.transition.time_ms.val/1000));
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();
  

  JsonBuilderI->Array_Start("g4");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("eihr");           // function
        JsonBuilderI->AppendBuffer("\"");    
        for (uint8_t row_id = 0; row_id < TRANSITION_METHOD_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
          JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR,
            pCONT_iLight->GetTransitionMethodNameByID(row_id, buffer, sizeof(buffer)),
            pCONT_iLight->GetTransitionMethodNameByID(row_id, buffer2, sizeof(buffer))
          );
        }
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Add("evl", pCONT_iLight->GetTransitionMethodName(buffer, sizeof(buffer)));
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  JsonBuilderI->Array_Start("g5");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("eihr");           // function
        JsonBuilderI->AppendBuffer("\"");    
        for (uint8_t row_id = 0; row_id < pCONT_iLight->ANIMATION_MODE_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
          JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR,
            pCONT_iLight->GetAnimationModeNameByID(row_id, buffer, sizeof(buffer)), 
            pCONT_iLight->GetAnimationModeNameByID(row_id, buffer, sizeof(buffer))
          );
        }
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Add("evl",pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  JsonBuilderI->Array_Start("g6");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("eihr");           // function
        JsonBuilderI->AppendBuffer("\"");    
        for (uint8_t row_id = 0; row_id < pCONT_iLight->PALETTELIST_STATIC_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
          JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR,
            pCONT_iLight->GetPaletteFriendlyNameByID(row_id, buffer, sizeof(buffer)),
            pCONT_iLight->GetPaletteFriendlyNameByID(row_id, buffer2, sizeof(buffer2))
          );
        }
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Add("evl",pCONT_iLight->GetPaletteFriendlyName(buffer, sizeof(buffer)));
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  // JsonBuilderI->Array_Start("g7");// Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr");           // function
  //       JsonBuilderI->AppendBuffer("\""); 
  //       for (uint8_t row_id = 0; row_id < EFFECTS_FUNCTION_LENGTH_ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR,
  //           row_id, 
  //           pCONT_iLight->GetPaletteFriendlyNameByID(row_id)
  //         );
  //       }
  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",1);
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();

  // JsonBuilderI->Array_Start("g8");// Class name
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->AddKey("eihr");           // function
  //       JsonBuilderI->AppendBuffer("\""); 
  //       for (uint8_t row_id = 0; row_id < 6; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
  //         JsonBuilderI->AppendBuffer(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
  //           row_id, 
  //           "running_id"
  //         );
  //       }
  //       JsonBuilderI->AppendBuffer("\"");
  //     JsonBuilderI->Add("evl",1);
  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();
  #endif

}

#endif // USE_WEBSERVER_ADVANCED_MULTIPAGES



void mAnimatorLight::WebAppend_Root_Draw_Table(){

  pCONT_web->WebAppend_Root_Draw_Table_dList_P(8,"rgb_table", kTitle_TableTitles_Root);
  
}



void mAnimatorLight::WebAppend_JSON_RootPage_LiveviewPixels()//{//AsyncWebServerRequest *request)
{

  uint16_t leds_max_to_show = pCONT_iLight->settings.light_size_count<MAX_LIVE_LEDS?pCONT_iLight->settings.light_size_count:MAX_LIVE_LEDS;
  uint8_t number_of_pixels = map(pCONT_iLight->liveview.pixel_resolution_percentage,0,100,0,pCONT_iLight->settings.light_size_count); //only serve every n'th LED if count over MAX_LIVE_LEDS

   
  // uint8_t number_of_pixels = pCONT_iLight->settings.light_size_count;
  // if(pCONT_iLight->settings.light_size_count>MAX_LIVE_LEDS){
  //   number_of_pixels = map(pCONT_iLight->liveview.pixel_resolution_percentage,0,100,0,pCONT_iLight->settings.light_size_count); //only serve every n'th LED if count over MAX_LIVE_LEDS
  // }


  uint8_t pixels_to_iter = ((pCONT_iLight->settings.light_size_count-1)/number_of_pixels)+1;

  char type_ctr[5];
  switch(pCONT_iLight->liveview.show_type){
    default: 
    case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID: sprintf(type_ctr,"%s","bclg");  break;
    case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID: sprintf(type_ctr,"%s","bcls");  break;
  }

  // if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  // JsonBuilderI->Start();
  
  JsonBuilderI->Array_Start(WEB_CLASS_RGB_LIVE);// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->Array_Start(type_ctr);
      RgbTypeColor c;
      for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
        c = GetPixelColor(i);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
      }  
      JsonBuilderI->Array_End();
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();


  #ifdef ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW
  JsonBuilderI->Array_Start("wrapper"); // Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->Array_Start(type_ctr);
      RgbTypeColor c2;
      for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
        c2 = GetPixelColor(i);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),c2.R,c2.G,c2.B);
      }  
      JsonBuilderI->Array_End();
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
  #endif // ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW



  // JsonBuilderI->End();

  // pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

}





#endif
#endif // USE_MODULE_CORE_WEBSERVER