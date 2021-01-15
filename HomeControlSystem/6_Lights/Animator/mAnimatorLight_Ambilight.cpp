#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT

/*******************************************************************************************************************
********************************************************************************************************************
************ START OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

// Limit ambilight to addressible type, else I will just use "scene"
void mAnimatorLight::init_Ambilight(){

  ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HueN2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HueN2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HueN2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

  
  ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
  ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));

}

void mAnimatorLight::SubTask_Ambilight_Main(){

  // Add mode to allow orientations, for when screens rotate so they respect top/bottom

  // if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
  // }

  
  // ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbColor(255,0,0);
  // ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbColor(0,255,0); 
  // ambilightsettings.screens[SCREEN_CENTRE].left.colour   = RgbColor(0,0,255);
  // ambilightsettings.screens[SCREEN_CENTRE].right.colour  = RgbColor(255,0,255);


  //switch modes : USE presets
  switch(ambilightsettings.ambilight_mode){
    case AMBILIGHT_PRESETS_ID:

    break;
    case AMBILIGHT_SIDES_ID: // IE DELL of dual tone from the past
      // not even splits, setting split point (ie bottom edge only 0%, 25% way up sides, half way 50%)
      if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
        Ambilight_Sides();
        StartAnimationAsBlendFromStartingColorToDesiredColor();
      }
    break;
    case AMBILIGHT_INPUT_STREAM_ID:{
      //serial input
    }break;
  }

} // END function


void mAnimatorLight::Ambilight_Sides(){
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::Ambilight_Sides()"));
  #endif

  float progress;
  HsbColor colour_tmp;

  uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
  uint8_t bottom_start = 0;
  for(int bottom=0;bottom<bottom_size;bottom++){
    animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
  }

  uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
  uint8_t left_start = 33;
  for(int left=0;left<left_size;left++){
    //if(ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage>=0){ //if set
      progress = (float)(left)/(float)(left_size);
      colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                         RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                         progress);
    //}
    animation_colours[left_start+left].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
  }

  uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
  uint8_t top_start = 52;
  for(int top=0;top<top_size;top++){
    animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
  }

  uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
  uint8_t right_start = 85;
  for(int right=0;right<right_size;right++){
    progress = (float)(right)/(float)(right_size);
    colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                       RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                       progress);
    animation_colours[right_start+right].DesiredColour = colour_tmp;
  }

}


// void mAnimatorLight::parsesub_ModeAmbilight(){

//    // Create local dereferenced variable
//   // JsonObject obj = (*_obj); 

//   int8_t tmp_id = 0;
  

//   char buffer[50];

// // // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// //   if(!obj[D_JSON_NAME].isNull()){ 
// //     const char* scenectr = obj[D_JSON_NAME];
// //     if((tmp_id=GetSceneIDbyName(scenectr))>=0){
// //       scene.name_id = tmp_id;
// //       pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
// //       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
// //       // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
// //       data_buffer.isserviced++;
// //     }else{
// //       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
// //     }
// //   }

// //   // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// //   // USe pointers
// //   //side
// //   // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
// //   // ambilightsettings.screens[SCREEN_CENTRE].top
// //   //screen
  
// //   if(!obj[F("top")][F(D_JSON_HUE)].isNull()){ 
// //     uint16_t hue = obj[F("top")][F(D_JSON_HUE)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = pCONT_iLight->HueN2F(hue);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("top")][F(D_JSON_SAT)].isNull()){ 
// //     uint8_t sat = obj[F("top")][F(D_JSON_SAT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = pCONT_iLight->SatN2F(sat);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("top")][F(D_JSON_BRT)].isNull()){ 
// //     uint8_t brt = obj[F("top")][F(D_JSON_BRT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     data_buffer.isserviced++;
// //   }



// //   if(!obj[F("bottom")][F(D_JSON_HUE)].isNull()){ 
// //     uint16_t hue = obj[F("bottom")][F(D_JSON_HUE)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = pCONT_iLight->HueN2F(hue);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("bottom")][F(D_JSON_SAT)].isNull()){ 
// //     uint8_t sat = obj[F("bottom")][F(D_JSON_SAT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = pCONT_iLight->SatN2F(sat);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("bottom")][F(D_JSON_BRT)].isNull()){ 
// //     uint8_t brt = obj[F("bottom")][F(D_JSON_BRT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     data_buffer.isserviced++;
// //   }




// //   if(!obj[D_JSON_RGB].isNull()){
// //     const char* rgbpacked = obj[D_JSON_RGB];
// //     uint32_t colour32bit = 0;
// //     if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
// //     }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
// //     RgbColor rgb;
// //     rgb.R = colour32bit >> 16; //RGB
// //     rgb.G = colour32bit >> 8 & 0xFF; //RGB
// //     rgb.B = colour32bit & 0xFF; //RGB
// //     // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// //     // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
// //     // // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
// //     // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
// //     // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// //   }

// //   // TIME with different units
// //   if(!obj[D_JSON_TIME].isNull()){ //default to secs
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time"];
// //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }else
// //   if(!obj[D_JSON_TIME].isNull()){
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time_secs"];
// //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }else
// //   if(!obj[D_JSON_TIME_MS].isNull()){
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time_ms"];
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }

  
// // Flash colour needs to NOT be a scene, but part of a manual direct mode
// // if(strstr(scenectr,"FLASHCOLOUR")){
// //       scene_reseting.name_id = scene.name_id; // remember scene to return to
// //       scene.name_id = MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
// //     }
// //     // Parse out flashcolour info if it exists
// //     if(scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID){

// //       if(tempctr = obj["hue"]){
// //         scene_reseting.hue = obj["hue"];
// //       }
// //       if(tempctr = obj["sat"]){
// //         scene_reseting.sat = obj["sat"];
// //       }
// //       if(tempctr = obj["brt"]){
// //         scene_reseting.brt = obj["brt"];
// //       }
// //       if(tempctr = obj["time"]){
// //         scene_reseting.tOnTime = obj["time"];
// //       }
// //     }
  
  



// //   // create easier names
// //   // char* topic_ctr = data_buffer.topic.ctr;
// //   // uint8_t topic_len = data_buffer.topic.len;
// //   // char* payload_ctr = data_buffer.payload.ctr;
// //   // uint8_t payload_len = data_buffer.payload.len;

// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/center",sizeof("/center")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /center");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     uint16_t index = 0;

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //     if(root["RGB"].is<JsonArray>()){

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB


// //         ambilightsettings.colour.found_idx++;
// //         index++;
// //       }
// //       pCONT->mso->MessagePrint("ENDambilightsettings.col.found_idx");
// //       pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //       pCONT->mso->MessagePrintln(index);

// //     }
// //     // else if(root["RGB"].is<const char*>()){ //one colour = string
// //     //
// //     //   const char* pixelcolour;
// //     //   pixelcolour = root["RGB"];
// //     //
// //     //   //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //     //   if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //     //   }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }
// //     //
// //     //   ambilightsettings.colour.r[ambilightsettings.colour.found_idx] = colour32bit >> 16; //RGB
// //     //   ambilightsettings.colour.g[ambilightsettings.colour.found_idx] = colour32bit >> 8 & 0xFF; //RGB
// //     //   ambilightsettings.colour.b[ambilightsettings.colour.found_idx] = colour32bit & 0xFF; //RGB
// //     //
// //     //   // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //     //   // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //     //   // Serial.println(parsed.col.b[parsed.col.found_idx]);
// //     //
// //     //   ambilightsettings.colour.found_idx++;
// //     // }

// //     pCONT->mso->MessagePrint("ambilightsettings.colour.found_idx");
// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->settings.light_size_count
// //     //ambilightsettings.colour.found_idx
// //     for(int i=0;i<index;i++){
// //         SetPixelColor(i, RgbColor(ambilightsettings.colour.rgb[i].R,ambilightsettings.colour.rgb[i].G,ambilightsettings.colour.rgb[i].B));
// //     }stripbus->Show();


// // } // END center


// // #ifdef DEVICE_RGBDELL

// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/left",sizeof("/left")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /left");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// //     if(root["RGB"].is<JsonArray>()){
// //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// //         ambilightsettings.colour.found_idx++;
// //       }

// //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// //       const char* pixelcolour;
// //       pixelcolour = root["RGB"];

// //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// //       ambilightsettings.colour.found_idx++;
// //     }

// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->settings.light_size_count
// //     //ambilightsettings.colour.found_idx
// //     //  for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// //     //    SetPixelColor(i,RgbColor(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// //     //  }
// //     // /stripbus->Show();

// //     uint32_t c; //colourrgb
// //     pinMode(RGB_DATA_LEFT_PIN,OUTPUT);
// //     pinMode(RGB_CLOCK_LEFT_PIN,OUTPUT);

// //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);
// //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// //     }
// //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);



// // } // END left



// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/right",sizeof("/right")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /right");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// //     if(root["RGB"].is<JsonArray>()){
// //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// //         ambilightsettings.colour.found_idx++;
// //       }

// //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// //       const char* pixelcolour;
// //       pixelcolour = root["RGB"];

// //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// //       ambilightsettings.colour.found_idx++;
// //     }

// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->settings.light_size_count
// //     //ambilightsettings.colour.found_idx
// //     // for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// //     //   mrgbneo_ani->setPixelColor(i,mrgbneo_ani->Color(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// //     // }
// //     // mrgbneo_ani->setBrightness(255);
// //     // mrgbneo_ani->show();

// //     uint32_t c; //colourrgb
// //     pinMode(RGB_DATA_RIGHT_PIN,OUTPUT);
// //     pinMode(RGB_CLOCK_RIGHT_PIN,OUTPUT);

// //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);
// //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// //     }
// //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);



// // } // END left

// //       //fShowPanelUpdate = true;
// //     //}


// // //  }

// //   //switch modes : USE serial input stream
// //   //send as json array?
// //   //RGB = [[r,g,b],[r,g,b],[r,g,b]]
// //   //SETTINGS = {pixel ratio, 17,10}{pixel direction, CW}{startposition, bottom right}{timeout,10}{minbrightness,10}{fade,0}

// //   pCONT_iLight->animation.mode_id = MODE_AMBILIGHT_ID;
// //   fForcePanelUpdate = true;

//   // return 0;

// } // END FUNCTION


/*******************************************************************************************************************
********************************************************************************************************************
************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT






#endif //USE_MODULE_LIGHTS_ANIMATOR




