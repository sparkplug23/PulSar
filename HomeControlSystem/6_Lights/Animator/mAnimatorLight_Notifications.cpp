#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


/*******************************************************************************************************************
********************************************************************************************************************
************ START OF NOTIFICATION DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS

// void mAnimatorLight::init_NotificationPanel(){

//   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//     notif.flags.fForcePanelUpdate = true; //clear presets
//     notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
//     notif.pixel[i].colour.R = map(i,0,STRIP_NOTIFICATION_SIZE,0,255);
//     notif.pixel[i].colour.G = map(i,0,STRIP_NOTIFICATION_SIZE,255,0);
//     // notif.pixel[i].colour.S = 1;
//     // notif.pixel[i].colour.B = 1;
//     // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//     // notif.pixel[i].colourWhite = 0;
//     // #endif
//   }
//   notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero = true;

// } //end "init_NotificationPanel"

// //void mAnimatorLight::SubTask_HeatingPanel(){
// void mAnimatorLight::SubTask_NotificationPanel(){

//   if(mTime::TimeReached(&notif.tSaved.ForceUpdate,120000)||(notif.flags.fForcePanelUpdate)){
//     notif.flags.fForcePanelUpdate = true;
//   }

//   //Enable a flag to auto turn everything off if inactive for 2 minutes
//   if(notif.flags.fEnableTimeoutAll){
//     if(mTime::TimeReached(&notif.tSaved.TimeoutCounter,30000)){  
//       #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "Seconds until notif timeout: [%d/%d]"),(millis()-notif.tSaved.TimeoutCounter)/1000,120);
//       #endif
//     }
//     if(mTime::TimeReached(&notif.tSaved.Timeout,120000)){
//         #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tNotifPanelTimeout"));
//       #endif
//       TurnLEDsOff();
//     }
//   }

//   char buffer[50];

//   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//     if(
//         mTime::TimeReached(&notif.pixel[i].tSavedUpdate,notif.pixel[i].tRateUpdate)
//         ||(notif.flags.fForcePanelUpdate)
//       ){ notif.flags.fForcePanelUpdate = false;
      
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "Notif mode %d:%s"),i,GetNotificationModeNamebyID(notif.pixel[i].mode, buffer));
//         #endif
      
//       RgbcctColor colour = RgbcctColor(0);

//       switch(notif.pixel[i].mode){
//         default:
//         case NOTIF_MODE_OFF_ID:
//         case NOTIF_MODE_STATIC_OFF_ID:
//           colour = RgbcctColor(0);
//         break;
//         case NOTIF_MODE_STATIC_ON_ID:{
          
//           colour = notif.pixel[i].colour;

//           // colour = HsbColor(notif.pixel[i].colour);
//           // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//           //   colour.W = notif.pixel[i].colourWhite;
//           // #endif


//         }break;
//         case NOTIF_MODE_FADE_ON_ID: //slow transition to static ON
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_STATIC_ON_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING on progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = BrtN2F(notif.pixel[i].transition_progess);

//           Change_RgbcctColor_Hue

          
//           colour = HsbColor(notif.pixel[i].colour);
//           #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//             colour.W = map(notif.pixel[i].transition_progess,0,100,0,notif.pixel[i].colourWhite);
//           #endif
//         break;
//         case NOTIF_MODE_FADE_OFF_ID:{
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING off progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = (100-notif.pixel[i].transition_progess)/100.0f;
//           colour = HsbColor(notif.pixel[i].colour);
//           #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//             colour.W = map(notif.pixel[i].transition_progess,0,100,notif.pixel[i].colourWhite,0);
//           #endif  
//         }break;
//         case NOTIF_MODE_BLINKING_OFF_ID:
//           colour = RgbcctColor(0);
//           notif.pixel[i].mode = NOTIF_MODE_BLINKING_ON_ID;
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
//         break;
//         case NOTIF_MODE_BLINKING_ON_ID:

//           colour = notif.pixel[i].colour;

//           notif.pixel[i].mode = NOTIF_MODE_BLINKING_OFF_ID;
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
//           break;
//         case NOTIF_MODE_PULSING_OFF_ID:
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_PULSING_ON_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
//           colour = notif.pixel[i].colour;
//         break;
//         case NOTIF_MODE_PULSING_ON_ID:
//           if(notif.pixel[i].transition_progess>0){
//             notif.pixel[i].transition_progess--;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_PULSING_OFF_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
//           colour = notif.pixel[i].colour;            
//         break;
//       }
//       SetPixelColor(i,colour);    
//       notif.flags.fShowPanelUpdate = true;
//     } //end if
//   } //end timer check

//   //Auto turn off
//   if(mTime::TimeReached(&notif.tSaved.AutoOff,1000)){// if 1 second past
//     for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ //check all
//       #ifdef ENABLE_LOG_LEVEL_DEBUG
//       AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Notif tSaved.AutoOff [%d]"),notif.pixel[i].auto_time_off_secs);
//       #endif
//       if(notif.pixel[i].auto_time_off_secs==1){ //if =1 then turn off and clear to 0
//         SetPixelColor(i,0);
//         notif.pixel[i].auto_time_off_secs = 0;
//         notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "Notif tSaved.AutoOff to OFF[%d]"),notif.pixel[i].auto_time_off_secs);
//         #endif
//       }else
//       if(notif.pixel[i].auto_time_off_secs>1){ //if =1 then turn off and clear to 0
//         notif.pixel[i].auto_time_off_secs--; //decrease
//       }
//     }// END for
//   }

//   // Update
//   if(notif.flags.fShowPanelUpdate){notif.flags.fShowPanelUpdate=false;
//     stripbus->Show();
//     notif.tSaved.ForceUpdate = millis(); // so we dont have flasher clashes
//   }

// } // END FUNCTION

// /** Sets to control per pixel for notification purposes. Examples of setting:
//  * {"pixel_num":[0],"hue":[0],"brt":[0],  "flasher_ms":[0]  ,"pulser_ms":[1]}
//  * {"pixel_num":[0,1],"hue":[0],"brt":[0],"flasher_secs":[0],"pulser_secs":[1]}
//  * {"pixel_num":0,"hue":[0],"brt":[0],    D_JSON_EFFECTS:[0]}
//  * {"pixel_num":"all","hue":[0],"brt":[0],D_JSON_EFFECTS:[0]}
//  * 
//  * 
//  * */
// void mAnimatorLight::parsesub_NotificationPanel(JsonObjectConst obj){ 

//    // Create local dereferenced variable
//   // JsonObject obj = (*_obj); 

//   struct PARSED{
//     // pixel index being targetted
//     struct PIXELNUMS{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }pixelnum;
//     // Setting as Hsb directly (sent as either rgb or hsb but stored as hsb)
//     struct COL{
//       HsbColor val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx = 0;
//     }hsb;
//     // Hues and brightness with sat 100
//     struct HUES{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }hue;
//     struct SATURATION{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }sat;
//     struct BRIGHTNESS{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }brt;
//     #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//     struct RGB_WHITE{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }white;
//     #endif
//     // Set blink mode with val being period
//     struct EFFECTS{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }flasher;
//     // Set pulse mode with val being period
//     struct PULSER{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }pulser;
//     // Set fade on/off to gradually change between static modes
//     struct FADER{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }fade;
//     // Set timer until led automatically turns off
//     struct TIMEON_MS{ 
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }timeon_ms;
//   }parsed;

//   Serial.printf("parsed struct size = %d", sizeof(parsed));

//   memset(&parsed,0,sizeof(parsed)); // clear parsing struct
  
  

//   // StaticJsonDocument<300> doc;
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   // JsonObject obj = doc.as<JsonObject>();

//   /**************************************************************************************************
//   PIXELNUM: formats "all",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_PIXELNUM].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_PIXELNUM));  
//     if(obj[D_JSON_PIXELNUM].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_PIXELNUM];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);          
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_PIXELNUM].is<const char*>()){
//       const char* name = obj[D_JSON_PIXELNUM];
//       if(strstr(name,"all")){
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));     
//         for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_PIXELNUM].is<int>()){
//       int val = obj[D_JSON_PIXELNUM];
//       parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
//     }
//   } //end pixelnum
  
//   /**************************************************************************************************
//   COLOUR: formats "RRGGBB" (string,array of strings)
//   ***************************************************************************************************/
// //rgbw? either or scenario
//   if(!obj[D_JSON_RGB].isNull()){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_RGB)); 
//     #endif
//     uint32_t colour32bit;  
//     // Arrays
//     if(obj[D_JSON_RGB].is<JsonArray>()){ 
//       JsonArrayConst array = obj[D_JSON_RGB];
//       for(JsonVariantConst v : array) {
//         const char* val = v.as<const char*>();
//         if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
//         parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
//         #endif
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_RGB].is<const char*>()){
//       const char* val = obj[D_JSON_RGB];
//       if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
//       parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
//       #endif
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB "Not Found"));
//       #endif
//     }
//   } //end hue


//   /**************************************************************************************************
//   HUE: formats "white/green/blue",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_HUE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_HUE));  
//     // Arrays
//     if(obj[D_JSON_HUE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_HUE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.hue.val[parsed.hue.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " JsonArray " " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_HUE].is<const char*>()){
//       const char* colour = obj[D_JSON_HUE];
//       // FUTURE : Add a get hue by colour name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_HUE].is<int>()){
//       int val = obj[D_JSON_HUE];
//       parsed.hue.val[parsed.hue.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " int" " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE "Not Found"));
//       #endif
//     }
//   } //end hue
  

//   /**************************************************************************************************
//   SAT: formats 1,[1] (int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_SAT].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_SAT));  
//     // Arrays
//     if(obj[D_JSON_SAT].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_SAT];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.sat.val[parsed.sat.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_BRT].is<const char*>()){
//       const char* name = obj[D_JSON_BRT];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_SAT].is<int>()){
//       int val = obj[D_JSON_SAT];
//       parsed.sat.val[parsed.sat.found_idx++] = val;
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT " int" " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
//       #endif
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT "Not Found"));
//       #endif
//     }
//   } //end brt
  
//   /**************************************************************************************************
//   BRT: formats "low/med/high",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_BRT].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_BRT));  
//     // Arrays
//     if(obj[D_JSON_BRT].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_BRT];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.brt.val[parsed.brt.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_BRT].is<const char*>()){
//       const char* name = obj[D_JSON_BRT];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_BRT].is<int>()){
//       int val = obj[D_JSON_BRT];
//       parsed.brt.val[parsed.brt.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " int" " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT "Not Found"));
//       #endif
//     }
//   } //end brt
  
//   #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//   /**************************************************************************************************
//   WHITE of rgbW: formats "low/med/high",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_WHITE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_WHITE));  
//     // Arrays
//     if(obj[D_JSON_WHITE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_WHITE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         // Serial.println(parsed.white.found_idx);
//         parsed.white.val[parsed.white.found_idx++] = val;//map(val,0,100,0,255); //only 0-100 accepted
//         // Serial.println(parsed.white.found_idx);
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " JsonArray " " [i%d:v%d]"),parsed.white.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_WHITE].is<const char*>()){
//       const char* name = obj[D_JSON_WHITE];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_WHITE].is<int>()){
//       int val = obj[D_JSON_WHITE];
//       parsed.white.val[parsed.white.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " int" " [i%d:v%d]"),parsed.white.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE "Not Found"));
//       #endif
//     }
//   } //end brt
//   #endif
  

//   /**************************************************************************************************
//   EFFECTS: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_EFFECTS].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_EFFECTS));  
//     // Arrays
//     if(obj[D_JSON_EFFECTS].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_EFFECTS];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.flasher.val[parsed.flasher.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_EFFECTS " JsonArray " " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_EFFECTS].is<int>()){
//       int val = obj[D_JSON_EFFECTS];
//       parsed.flasher.val[parsed.flasher.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_EFFECTS " int" " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_EFFECTS "Not Found"));
//       #endif
//     }
//   } //end flasher
  

//   /**************************************************************************************************
//   PULSER: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_PULSER].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_PULSER));  
//     // Arrays
//     if(obj[D_JSON_PULSER].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_PULSER];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.pulser.val[parsed.pulser.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " JsonArray " " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_PULSER].is<int>()){
//       int val = obj[D_JSON_PULSER];
//       parsed.pulser.val[parsed.pulser.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " int" " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER "Not Found"));
//     }
//   } //end pulser
  

//   /**************************************************************************************************
//   FADE: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_FADE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_FADE));  
//     // Arrays
//     if(obj[D_JSON_FADE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_FADE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.fade.val[parsed.fade.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " JsonArray " " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_FADE].is<int>()){
//       int val = obj[D_JSON_FADE];
//       parsed.fade.val[parsed.fade.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " int" " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE "Not Found"));
//     }
//   } //end fade
  

//   /**************************************************************************************************
//   TIMEON: "time_secs" or "time_ms" formats 1,[1] (int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_TIME_SECS].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_SECS));  
//     // Arrays
//     if(obj[D_JSON_TIME_SECS].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_TIME_SECS];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     //no string command
//     if(obj[D_JSON_TIME_SECS].is<int>()){
//       int val = obj[D_JSON_TIME_SECS];
//       parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS "Not Found"));
//     }
//   }else
//   if(!obj[D_JSON_TIME_MS].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_MS));  
//     // Arrays
//     if(obj[D_JSON_TIME_MS].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_TIME_MS];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     //no string command
//     if(obj[D_JSON_TIME_MS].is<int>()){
//       int val = obj[D_JSON_TIME_MS];
//       parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS "Not Found"));
//     }
//   } 
  
  
//   /***********************************************************************************************************************************************************
//    * ***********************************************************************************************************************************************************
//    * Fill commands using parsed structs *************************************************************************************************************************
//    * ***********************************************************************************************************************************************************
//    * ***********************************************************************************************************************************************************/

//   int pixelidx = 0;
//   int subidx = 0;

//   if(parsed.hsb.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){  // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour = parsed.hsb.val[subidx];
//       if(subidx<parsed.hsb.found_idx-1){subidx++;}
//     }
//   }
  
//   if(parsed.hue.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.H = HueN2F(parsed.hue.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.hue.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.sat.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.S = SatN2F(parsed.sat.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.sat.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.brt.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.B = BrtN2F(parsed.brt.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.brt.found_idx-1){subidx++;}
//     }
//   }
  
//   // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//   // // RGB method - white part
//   // if(parsed.white.found_idx){
//   //   pixelidx = 0; subidx = 0;
//   //   while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//   //     int pixelnum = parsed.pixelnum.val[pixelidx++];
//   //     notif.pixel[pixelnum].colourWhite = parsed.white.val[subidx];
//   //     notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//   //     if(subidx<parsed.white.found_idx-1){subidx++;}
//   //     if(notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero){
//   //       if(!parsed.brt.found_idx){ //no brightness defined
//   //         notif.pixel[pixelnum].colour.B = 0;        
//   //       }
//   //     }

//   //   }
//   // }
//   // #endif

//   if(parsed.flasher.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.flasher.val[subidx]; 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_BLINKING_ON_ID; 
//       // Step across the subset indexes if they exist, else last/only is used for all pixelnum
//       if(subidx<parsed.flasher.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.pulser.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.pulser.val[subidx]; 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_PULSING_ON_ID; 
//       if(subidx<parsed.pulser.found_idx-1){subidx++;}
//     }
//   }

//   char buffer[30];

//   if(parsed.fade.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.fade.val[subidx];
//       notif.pixel[pixelnum].period_ms = constrain(notif.pixel[pixelnum].period_ms,100,65000);
//       // If pixel was off, then turn on else turn off
      
//       Serial.printf("notif.pixel[%d].mode = %s %d;\n\r",pixelnum,GetNotificationModeNamebyID(notif.pixel[pixelnum].mode, buffer),notif.pixel[pixelnum].period_ms);

//       if(
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_STATIC_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_PULSING_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_FADE_OFF_ID)
//         // (notif.pixel[pixelnum].colour.B)
//         // ||(notif.pixel[pixelnum].colourWhite)
//         ){
//           notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_ON_ID; 
//           Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_ON_ID;\n\r",pixelnum);
//         }else{
//           notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_OFF_ID; 
//           Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_OFF_ID;\n\r",pixelnum);
//         }
//         notif.pixel[pixelnum].transition_progess = 0; //0 to 100%
//       if(subidx<parsed.fade.found_idx-1){subidx++;}
//     }
//   }

//   // If neither flasher or pulser defined, set as static as default
//   if((!parsed.flasher.found_idx)&&(!parsed.pulser.found_idx)&&(!parsed.fade.found_idx)){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = 1000; // not used, but set to something reasonable 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; 
//     }
//   }

//   if(parsed.timeon_ms.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels

//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS "timeon_ms [%d i%d:v%d]"),pixelidx,parsed.timeon_ms.found_idx,parsed.pixelnum.val[pixelidx]);    
      
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].auto_time_off_secs = (parsed.timeon_ms.val[subidx]/1000); 
//       if(subidx<parsed.timeon_ms.found_idx-1){subidx++;}

//       //also reset timer millis so they align and run together
//       notif.pixel[pixelnum].tSavedUpdate = millis();
//     }
//   }else{
//     Serial.println("parsed.timeon_ms.found_idx NOT FOUND");
//   }

//   // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   //   //you diAddLog_mP2(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_NEO_MODE_NOTIFICATIONS "Index [%d] RgbColor %d\t%d\t%d\t flash [%d]"),i,setcolour[i].r,setcolour[i].g,setcolour[i].b,setcolour[i].flash_enabled);
//   // }

//   notif.tSaved.Timeout = millis(); // resets timeout counter
//   notif.tSaved.TimeoutCounter = millis();
//   pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_NOTIFICATIONS_ID;
//   notif.flags.fForcePanelUpdate = true;

  


// } // END FUNCTION

// const char* mAnimatorLight::GetNotificationModeNamebyID(uint8_t id, char* buffer){
//   // return 
//   //     (id == NOTIF_MODE_OFF_ID          ? D_NOTIF_MODE_OFF_NAME_CTR  :
//   //     (id == NOTIF_MODE_STATIC_OFF_ID   ? D_NOTIF_MODE_STATIC_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_STATIC_ON_ID    ? D_NOTIF_MODE_STATIC_ON_NAME_CTR :
//   //     (id == NOTIF_MODE_BLINKING_OFF_ID ? D_NOTIF_MODE_BLINKING_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_BLINKING_ON_ID  ? D_NOTIF_MODE_BLINKING_ON_NAME_CTR :
//   //     (id == NOTIF_MODE_PULSING_OFF_ID  ? D_NOTIF_MODE_PULSING_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_PULSING_ON_ID   ? D_NOTIF_MODE_PULSING_ON_NAME_CTR :
//   //     "NoMatch")))))));
//   return WARNING_NOTHANDLED_CTR;
// }
// int8_t mAnimatorLight::GetNotificationModeIDbyName(const char* c){
//   if(c=='\0'){ 
//     return -1;
//   }
//   // if(strstr(c,D_NOTIF_MODE_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_STATIC_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_STATIC_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_STATIC_ON_NAME_CTR)){
//   //   return NOTIF_MODE_STATIC_ON_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_BLINKING_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_BLINKING_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_BLINKING_ON_NAME_CTR)){
//   //   return NOTIF_MODE_BLINKING_ON_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_PULSING_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_PULSING_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_PULSING_ON_NAME_CTR)){
//   //   return NOTIF_MODE_PULSING_ON_ID;
//   // }
//   else{
//     return -1;
//   }
// }

// // Update struct that shows overview, always sends
// uint8_t mAnimatorLight::ConstructJSON_Notifications(uint8_t json_level){

//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Notifications"));
//   #endif

//   // DynamicJsonDocument doc(1400);
//   // JsonObject root = doc.to<JsonObject>();

//   // root["fForcePanelUpdate"] = notif.flags.fForcePanelUpdate;
//   // root["fShowPanelUpdate"] = notif.flags.fShowPanelUpdate;
//   // root["fEnableTimeoutAll"] = notif.flags.fEnableTimeoutAll;
      
      
      
//   // JsonArray mode_arr = root.createNestedArray("pCONT_iLight->auto_time_off_secs");
//   // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   //   mode_arr.add(notif.pixel[i].pCONT_iLight->auto_time_off_secs);
//   // }


//   // // JsonArray mode_arr = root.createNestedArray("mode");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   mode_arr.add(GetNotificationModeNamebyID(notif.pixel[i].mode));
//   // // }
//   // // JsonArray period_ms_arr = root.createNestedArray("period_ms");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   period_ms_arr.add(notif.pixel[i].period_ms);
//   // // }
//   // // JsonArray transition_progess_arr = root.createNestedArray(PSTR("transition_progess"));
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   transition_progess_arr.add(notif.pixel[i].transition_progess);
//   // // }
//   // // JsonArray hsb_arr = root.createNestedArray("hsb");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ 
//   // //   char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
//   // //   sprintf(tmpctr,PSTR("%d,%d,%d"),HueF2N(notif.pixel[i].colour.H),
//   // //                             SatF2N(notif.pixel[i].colour.S),
//   // //                             BrtF2N(notif.pixel[i].colour.B));
//   // //   hsb_arr.add(tmpctr);
//   // // }
//   // //     pixelobj["tSavedUpdate"] = notif.pixel[i].tSavedUpdate;
//   // //     pixelobj["tRateUpdate"] = notif.pixel[i].tRateUpdate;

//   // data_buffer.payload.len = measureJson(root)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   return 0;

// }
#endif

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF NOTIFICATION DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/




#endif //USE_MODULE_LIGHTS_ANIMATOR




