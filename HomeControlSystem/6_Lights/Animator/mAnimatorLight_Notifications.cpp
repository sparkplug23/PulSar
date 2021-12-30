#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

// Can this become its own segment??
// The data regarding the pixel effect can be saved as a typedeffed struct, commands will have to know its started.


#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS

void mAnimatorLight::init_Notifications(){

  notif.flags.fForcePanelUpdate = true;
  notif.flags.fEnableTimeoutAll = false;

  for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
    notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
    notif.pixel[i].colour = RgbColor(0);    
    notif.pixel[i].tRateUpdate = 1;
    notif.pixel[i].period_ms = 2000;
    notif.pixel[i].auto_time_off_secs = 60;//map(i,0,STRIP_NOTIFICATION_SIZE, 10,60);
    notif.pixel[i].brightness = 100;
    notif.pixel[i].speed = 100;
    notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
  }
  
} //end "init_Notifications"


/**
 * Linear blend 
 * 0 to 100% equals linear transformation eg fade
 * pulse
 * 0-50% = colour 1->2
 * 51-100% = colour 2->1
 * */
RgbcctColor mAnimatorLight::ApplySimpleEffectOnColour(RgbcctColor colour_start, RgbcctColor colour_end, float progress, uint8_t effect_type){

  RgbcctColor colour_out;

  switch(effect_type){
    default:
    case SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_ID:
      colour_out = RgbcctColor::LinearBlend(colour_start, colour_end, progress);
    break;
    case SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_AND_BACK_ID:{
      if(progress <= 0.5){ //0 to 0.5 ie 0 to 1.0
        colour_out = RgbcctColor::LinearBlend(colour_start, colour_end, progress*2.0f);
      }else{
        colour_out = RgbcctColor::LinearBlend(colour_start, colour_end, 2.0f*(1.0f-progress));
      }    
    }break;
    case SIMPLE_EFFECT_ON_COLOUR_BLINK5050_ID:
      colour_out = progress <= 0.5 ? colour_start : colour_end;
    break;
    case SIMPLE_EFFECT_ON_COLOUR_BLINK1090_ID:
      colour_out = progress <= 0.1 ? colour_start : colour_end;
    break;
  }

  return colour_out;

}


void mAnimatorLight::SubTask_Notifications(){

  if(mTime::TimeReached(&notif.tSaved.ForceUpdate,120000)||(notif.flags.fForcePanelUpdate)){
    notif.flags.fForcePanelUpdate = true;
  }

  //Enable a flag to auto turn everything off if inactive for 2 minutes
  // This could probably be replaced by simply sending the colour again anyway
  // 
  if(notif.flags.fEnableTimeoutAll){
    if(mTime::TimeReached(&notif.tSaved.TimeoutCounter,30000)){  
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "Seconds until notif timeout: [%d/%d]"),(millis()-notif.tSaved.TimeoutCounter)/1000,120);
      #endif
    }
    if(mTime::TimeReached(&notif.tSaved.Timeout,120000)){
        #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tNotifPanelTimeout"));
      #endif
      TurnLEDsOff();
    }
  }

  char buffer[50];

  for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
    if(
        mTime::TimeReached(&notif.pixel[i].tSavedUpdate,notif.pixel[i].tRateUpdate)
        ||(notif.flags.fForcePanelUpdate)
      ){ notif.flags.fForcePanelUpdate = false;
      
      #ifdef ENABLE_LOG_LEVEL_INFO
      //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "Notif mode %d:%s"),i,GetNotificationModeNamebyID(notif.pixel[i].mode, buffer));
      #endif
      
      RgbcctColor colour = RgbcctColor(0);

      switch(notif.pixel[i].mode){
        case NOTIF_MODE_STATIC_ON_ID:
          colour = pCONT_lAni->ApplyBrightnesstoRgbcctColour(notif.pixel[i].colour, notif.pixel[i].brightness);
        break;  
        default:        
        case NOTIF_MODE_STATIC_OFF_ID:
          colour = RgbcctColor(0);
        break;     
        case NOTIF_MODE_FADE_ON_ID:
          colour = pCONT_lAni->ApplyBrightnesstoRgbcctColour(notif.pixel[i].colour, notif.pixel[i].brightness);
          colour = ApplySimpleEffectOnColour(
                    RgbColor(0),// GetPixelColor(i),
                    colour,
                    notif.pixel[i].progress/D_NOTIFICATION_PROGRESS_RANGE_F,
                    SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_ID
                  );

          if(notif.pixel[i].progress<D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].progress++;
          }else{
            notif.pixel[i].progress = 0;
          }
          // Serial.println(notif.pixel[i].progress);

          // Check if complete
          if(notif.pixel[i].progress < D_NOTIFICATION_PROGRESS_RANGE){            
            // convert D_NOTIFICATION_PROGRESS_RANGE steps into rate updates
            notif.pixel[i].tRateUpdate = notif.pixel[i].period_ms/D_NOTIFICATION_PROGRESS_RANGE;                        
          }
          if(notif.pixel[i].progress >= D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].mode = NOTIF_MODE_STATIC_ON_ID;
          }
        break;    
        case NOTIF_MODE_FADE_OFF_ID:
          colour = pCONT_lAni->ApplyBrightnesstoRgbcctColour(notif.pixel[i].colour, notif.pixel[i].brightness);
          colour = ApplySimpleEffectOnColour(
                    GetPixelColor(i),
                    RgbcctColor(0),
                    notif.pixel[i].progress/D_NOTIFICATION_PROGRESS_RANGE_F,
                    SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_ID
                  );

          if(notif.pixel[i].progress<D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].progress++;
          }else{
            notif.pixel[i].progress = 0;
          }
          // Serial.println(notif.pixel[i].progress);

          // Check if complete
          if(notif.pixel[i].progress < D_NOTIFICATION_PROGRESS_RANGE){            
            // convert D_NOTIFICATION_PROGRESS_RANGE steps into rate updates
            notif.pixel[i].tRateUpdate = notif.pixel[i].period_ms/D_NOTIFICATION_PROGRESS_RANGE;                        
          }
          if(notif.pixel[i].progress >= D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
          }
        break;
        case NOTIF_MODE_PULSING_ID:
          colour = pCONT_lAni->ApplyBrightnesstoRgbcctColour(notif.pixel[i].colour, notif.pixel[i].brightness);
          colour = ApplySimpleEffectOnColour(
                    RgbColor(0),// GetPixelColor(i),
                    colour,
                    notif.pixel[i].progress/D_NOTIFICATION_PROGRESS_RANGE_F,
                    SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_AND_BACK_ID
                  );

          if(notif.pixel[i].progress<D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].progress++;
          }else{
            notif.pixel[i].progress = 0;
          }
          // Serial.println(notif.pixel[i].progress);

          // map speed

          // Check if complete
          if(notif.pixel[i].progress < D_NOTIFICATION_PROGRESS_RANGE){            
            // convert D_NOTIFICATION_PROGRESS_RANGE steps into rate updates
            notif.pixel[i].tRateUpdate = notif.pixel[i].period_ms/D_NOTIFICATION_PROGRESS_RANGE;                        
          }

        break;
        case NOTIF_MODE_BLINKING_ID:
          colour = pCONT_lAni->ApplyBrightnesstoRgbcctColour(notif.pixel[i].colour, notif.pixel[i].brightness);
          colour = ApplySimpleEffectOnColour(
                    RgbColor(0),// GetPixelColor(i),
                    colour,
                    notif.pixel[i].progress/D_NOTIFICATION_PROGRESS_RANGE_F,
                    SIMPLE_EFFECT_ON_COLOUR_BLINK5050_ID
                  );

          if(notif.pixel[i].progress<D_NOTIFICATION_PROGRESS_RANGE){
            notif.pixel[i].progress++;
          }else{
            notif.pixel[i].progress = 0;
          }
          // Serial.println(notif.pixel[i].progress);

          // Check if complete
          if(notif.pixel[i].progress < D_NOTIFICATION_PROGRESS_RANGE){            
            // convert D_NOTIFICATION_PROGRESS_RANGE steps into rate updates
            notif.pixel[i].tRateUpdate = notif.pixel[i].period_ms/D_NOTIFICATION_PROGRESS_RANGE;                        
          }
        break;
      }
      SetPixelColor(i,colour);    
      notif.flags.fShowPanelUpdate = true;
    } //end if
  } //end timer check

  
            // AddLog(LOG_LEVEL_TEST, PSTR("notif.pixel[i].tRateUpdate=%d"),notif.pixel[i].tRateUpdate);

  //Auto turn off
  if(mTime::TimeReached(&notif.tSaved.AutoOff,1000)){// if 1 second past
    for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ //check all
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Notif tSaved.AutoOff [%d]"),notif.pixel[i].auto_time_off_secs);
      #endif
      if(notif.pixel[i].auto_time_off_secs==1){ //if =1 then turn off and clear to 0
        SetPixelColor(i,0);
        notif.pixel[i].auto_time_off_secs = 0;
        notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "Notif tSaved.AutoOff to OFF[%d]"),notif.pixel[i].auto_time_off_secs);
        #endif
      }else
      if(notif.pixel[i].auto_time_off_secs>1){ //if =1 then turn off and clear to 0
        notif.pixel[i].auto_time_off_secs--; //decrease
      }
    }// END for
  }

  // Update
  if(notif.flags.fShowPanelUpdate){notif.flags.fShowPanelUpdate=false;
    stripbus->Show();
    notif.tSaved.ForceUpdate = millis(); // so we dont have flasher clashes
  }


} // END FUNCTION


// float CurrentProgress(uint8_t notif)
// {
//     return (float)(_duration - _remaining) / (float)_duration;
// }



void mAnimatorLight::parsesub_Notifications(JsonParserObject obj){
  
  struct PARSED{
    // pixel index being targetted
    struct PIXELNUMS{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }pixelnum;
    // Hues and brightness with sat 100
    struct HUES{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }hue;
    struct RgbcctColours{
      RgbcctColor colour[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }rgbcct;
    struct BRIGHTNESS{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }brightness;
    // EFFECTS code be rolled in using enums to donete the type of effect to show!
    struct ACTIVE_NOTIF_MODE{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }notif_mode;
    struct SPEED{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }speed;
    // Set timer until led automatically turns off
    struct TIMEON_MS{ 
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }timeon_ms;
  }parsed;

  Serial.printf("parsed struct size = %d\n\r", sizeof(parsed));

  memset(&parsed,0,sizeof(parsed)); // clear parsing struct
    
  JsonParserToken jtok = 0; 

  /**************************************************************************************************
  PIXELNUM: formats "all",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_PIXELNUM]){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_PIXELNUM));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    if(jtok.isArray()){  
     JsonParserArray arrobj = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_PIXELNUM];
      for(auto v : arrobj) {
        if(parsed.pixelnum.found_idx > STRIP_NOTIFICATION_SIZE){ break; }
        parsed.pixelnum.val[parsed.pixelnum.found_idx++] = v.getInt();
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,parsed.pixelnum.val[parsed.pixelnum.found_idx-1]);      
      }
    }else
    if(jtok.isStr()){
      if(strstr(jtok.getStr(),"all")){
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));    
        #endif // ENABLE_LOG_LEVEL_COMMANDS 
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      }
      data_buffer.isserviced++;
    }else 
    if(jtok.isNum()){
      parsed.pixelnum.val[parsed.pixelnum.found_idx++] = jtok.getInt();
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,parsed.pixelnum.val[parsed.pixelnum.found_idx-1]);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS
      data_buffer.isserviced++;
    }else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
    }
  } //end pixelnum
  
  
  /**************************************************************************************************
  Rgbcct: formats "Rgbwc" (string,array of strings) 
  ***************************************************************************************************/
 
  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_COLOUR]){
    
    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_COLOUR));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS


    // AddLog(LOG_LEVEL_INFO, PSTR("jtok.size()=%d"),jtok.size());
    uint8_t jsonpair_count = jtok.size();


    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){
      jtok.nextOne(); //skip start of object

      JsonParserToken jtok_colour_array = jtok;
      uint8_t subjsonpair_count = jtok.size();
      
      AddLog(LOG_LEVEL_INFO, PSTR("jtok_colour_array.size()=%d,%d"),jtok_colour_array.size(), subjsonpair_count);
      // uint8_t jsonpair_count = jtok.size();

      RgbcctColor colour_tmp = RgbcctColor(0);
      for(int subpair_index = 0; subpair_index < subjsonpair_count; subpair_index++){
        jtok.nextOne(); //skip start of object
        colour_tmp[subpair_index] = jtok.getInt();
        AddLog(LOG_LEVEL_INFO, PSTR("colour_tmp[%d]=%d"), subpair_index, colour_tmp[subpair_index]);
      }

      parsed.rgbcct.colour[parsed.rgbcct.found_idx++] = colour_tmp;
      
    }

    AddLog(LOG_LEVEL_TEST, PSTR("parsed.rgbcct.found_idx=%d"), parsed.rgbcct.found_idx);


    // if(jtok.isArray()){  
    //  JsonParserArray arrobj = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_PIXELNUM];
    //   for(auto v : arrobj) {
    //     if(parsed.pixelnum.found_idx > STRIP_NOTIFICATION_SIZE){ break; }
    //     parsed.pixelnum.val[parsed.pixelnum.found_idx++] = v.getInt();
    //     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,parsed.pixelnum.val[parsed.pixelnum.found_idx-1]);      
    //   }
    // }else
    // if(jtok.isStr()){
    //   if(strstr(jtok.getStr(),"all")){
    //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
    //     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));    
    //     #endif // ENABLE_LOG_LEVEL_COMMANDS 
    //     for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
    //   }
    //   data_buffer.isserviced++;
    // }else 
    // if(jtok.isNum()){
    //   parsed.pixelnum.val[parsed.pixelnum.found_idx++] = jtok.getInt();
    //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
    //   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,parsed.pixelnum.val[parsed.pixelnum.found_idx-1]);    
    //   #endif // ENABLE_LOG_LEVEL_COMMANDS
    //   data_buffer.isserviced++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
    // }


  } //end pixelnum
  

  /**************************************************************************************************
  HUE: formats "white/green/blue",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_HUE]){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_HUE));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    if(jtok.isArray()){  
     JsonParserArray arrobj = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_HUE];
      for(auto v : arrobj) {
        if(parsed.hue.found_idx > STRIP_NOTIFICATION_SIZE){ break; }
        parsed.hue.val[parsed.hue.found_idx++] = v.getInt();
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " JsonArray " " [i%d:v%d]"),parsed.hue.found_idx-1,parsed.hue.val[parsed.hue.found_idx-1]);      
      }
    }else
    if(jtok.isStr()){
      if(strstr(jtok.getStr(),"all")){
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " const char* " "all"));    
        #endif // ENABLE_LOG_LEVEL_COMMANDS 
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.hue.val[parsed.hue.found_idx++] = i; }
      }
      data_buffer.isserviced++;
    }else 
    if(jtok.isNum()){
      parsed.hue.val[parsed.hue.found_idx++] = jtok.getInt();
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " int" " [i%d:v%d]"),parsed.hue.found_idx-1,parsed.hue.val[parsed.hue.found_idx-1]);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS
      data_buffer.isserviced++;
    }else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE "Not Found"));
    }
  } //end hue

  /**************************************************************************************************
  Brightness: formats "low/med/high",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_BRIGHTNESS]){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_BRIGHTNESS));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    if(jtok.isArray()){  
     JsonParserArray arrobj = jtok;//obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_BRIGHTNESS];
      for(auto v : arrobj) {
        if(parsed.brightness.found_idx > STRIP_NOTIFICATION_SIZE){ break; }
        parsed.brightness.val[parsed.brightness.found_idx++] = v.getInt();
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRIGHTNESS " JsonArray " " [i%d:v%d]"),parsed.brightness.found_idx-1,parsed.brightness.val[parsed.brightness.found_idx-1]);      
      }
    }else
    if(jtok.isStr()){
      if(strstr(jtok.getStr(),"all")){
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRIGHTNESS " const char* " "all"));    
        #endif // ENABLE_LOG_LEVEL_COMMANDS 
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.brightness.val[parsed.brightness.found_idx++] = i; }
      }
      data_buffer.isserviced++;
    }else 
    if(jtok.isNum()){
      parsed.brightness.val[parsed.brightness.found_idx++] = jtok.getInt();
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRIGHTNESS " int" " [i%d:v%d]"),parsed.brightness.found_idx-1,parsed.brightness.val[parsed.brightness.found_idx-1]);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS
      data_buffer.isserviced++;
    }else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRIGHTNESS "Not Found"));
    }
  } //end brightness


  /**************************************************************************************************
  Speed: formats "low/med/high",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_SPEED]){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_SPEED));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    if(jtok.isArray()){  
     JsonParserArray arrobj = jtok;
      for(auto v : arrobj) {
        if(parsed.speed.found_idx > STRIP_NOTIFICATION_SIZE){ break; }
        parsed.speed.val[parsed.speed.found_idx++] = v.getInt();
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SPEED " JsonArray " " [i%d:v%d]"),parsed.speed.found_idx-1,parsed.speed.val[parsed.speed.found_idx-1]);      
      }
    }else
    if(jtok.isStr()){
      if(strstr(jtok.getStr(),"all")){
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SPEED " const char* " "all"));    
        #endif // ENABLE_LOG_LEVEL_COMMANDS 
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.speed.val[parsed.speed.found_idx++] = i; }
      }
      data_buffer.isserviced++;
    }else 
    if(jtok.isNum()){
      parsed.speed.val[parsed.speed.found_idx++] = jtok.getInt();
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SPEED " int" " [i%d:v%d]"),parsed.speed.found_idx-1,parsed.speed.val[parsed.speed.found_idx-1]);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS
      data_buffer.isserviced++;
    }else{
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SPEED "Not Found"));
    }
  } //end speed

  /**************************************************************************************************
  NOTIF_MODE: formats "slow/fast",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_MODE]){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_MODE));  
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    if(jtok.isArray()){  
     JsonParserArray arrobj = jtok;//obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_BRIGHTNESS];
      for(auto v : arrobj) {
        if(parsed.notif_mode.found_idx > STRIP_NOTIFICATION_SIZE){ break; }

        // Check if num


        // Check if string

        parsed.notif_mode.val[parsed.notif_mode.found_idx++] = v.getInt();
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_MODE " JsonArray " " [i%d:v%d]"),parsed.notif_mode.found_idx-1,parsed.notif_mode.val[parsed.notif_mode.found_idx-1]);      
      }
    }else
    if(jtok.isStr()){
      // if(strstr(jtok.getStr(),"all")){
      //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
      //   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRIGHTNESS " const char* " "all"));    
      //   #endif // ENABLE_LOG_LEVEL_COMMANDS 
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.brightness.val[parsed.brightness.found_idx++] = i; }
      // }
      data_buffer.isserviced++;
    }else 
    if(jtok.isNum()){
      parsed.notif_mode.val[parsed.notif_mode.found_idx++] = jtok.getInt();
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_MODE " int" " [i%d:v%d]"),parsed.notif_mode.found_idx-1,parsed.notif_mode.val[parsed.notif_mode.found_idx-1]);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS
      data_buffer.isserviced++;
    }else
    {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_MODE "Not Found"));
    }
  } //end brightness
  
  /**************************************************************************************************
  TIMEON: "time_secs" or "time_ms" formats 1,[1] (int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(jtok = obj[PM_JSON_NOTIFICATIONS].getObject()[PM_JSON_TIME_SECS]){
  
    AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_SECS));  
    // Arrays
    if(jtok.isArray()){       
      JsonParserArray array = jtok;
      for(auto v : array) {
        parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = v.getInt()*1000; //secs2ms
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,v.getInt());    
      }
      data_buffer.isserviced++;
    }else
    if(jtok.isNum()){
      parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = jtok.getInt()*1000; //secs2ms
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,jtok.getInt());    
    }

  }
  // else
  //   //no string command
  //   if(obj[D_JSON_TIME_SECS].is<int>()){
  //     int val = obj[D_JSON_TIME_SECS];
  //     parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
  //     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
  //     data_buffer.isserviced++;
  //   }else{
  //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS "Not Found"));
  //   }
  // }else


  // if(!obj[D_JSON_TIME_MS].isNull()){
  //   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_MS));  
  //   // Arrays
  //   if(obj[D_JSON_TIME_MS].is<JsonArray>()){   
  //     JsonArrayConst array = obj[D_JSON_TIME_MS];
  //     for(JsonVariantConst v : array) {
  //       int val = v.as<int>();
  //       parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
  //       AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
  //     }
  //     data_buffer.isserviced++;
  //   }
  //   // else
  //   // //no string command
  //   // if(obj[D_JSON_TIME_MS].is<int>()){
  //   //   int val = obj[D_JSON_TIME_MS];
  //   //   parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
  //   //   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
  //   //   data_buffer.isserviced++;
  //   // }else{
  //   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS "Not Found"));
  //   // }
  // } 
  
  
  /***********************************************************************************************************************************************************
   * ***********************************************************************************************************************************************************
   * Fill commands using parsed structs *************************************************************************************************************************
   * ***********************************************************************************************************************************************************
   * ***********************************************************************************************************************************************************/

  
  AddLog_Array(LOG_LEVEL_TEST, "parsed.pixelnum.val", parsed.pixelnum.val, STRIP_NOTIFICATION_SIZE);
  AddLog_Array(LOG_LEVEL_TEST, "parsed.brightness.val", parsed.brightness.val, STRIP_NOTIFICATION_SIZE);
  AddLog_Array(LOG_LEVEL_TEST, "parsed.notif_mode.val", parsed.notif_mode.val, STRIP_NOTIFICATION_SIZE);

  int pixelidx = 0;
  int subidx = 0;

  if(parsed.hue.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour = HsbColor(pCONT_iLight->HueN2F(parsed.hue.val[subidx]),1,1);
      if(subidx<parsed.hue.found_idx-1){subidx++;}
    }
  }
  
  if(parsed.rgbcct.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.rgbcct.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour = parsed.rgbcct.colour[subidx];
      if(subidx<parsed.rgbcct.found_idx-1){subidx++;}
    }
  }

  if(parsed.brightness.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].brightness = parsed.brightness.val[subidx];
      if(subidx<parsed.brightness.found_idx-1){subidx++;}
    }
  }

  if(parsed.speed.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].speed = parsed.speed.val[subidx];
      if(subidx<parsed.speed.found_idx-1){subidx++;}
    }
  }
    
  if(parsed.notif_mode.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].mode = parsed.notif_mode.val[subidx]; 
      if(subidx<parsed.notif_mode.found_idx-1){subidx++;}
    }
  }

  char buffer[30];

  // If neither flasher or pulser defined, set as static as default
  if(!parsed.notif_mode.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].period_ms = 1000; // not used, but set to something reasonable 
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; 
    }
  }

  if(parsed.timeon_ms.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS "timeon_ms [%d i%d:v%d]"),pixelidx,parsed.timeon_ms.found_idx,parsed.pixelnum.val[pixelidx]);    
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].auto_time_off_secs = (parsed.timeon_ms.val[subidx]/1000); 
      if(subidx<parsed.timeon_ms.found_idx-1){subidx++;}
      //also reset timer millis so they align and run together
      notif.pixel[pixelnum].tSavedUpdate = millis();
    }
  }

  // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  //   //you diAddLog_mP2(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_NEO_MODE_NOTIFICATIONS "Index [%d] RgbColor %d\t%d\t%d\t flash [%d]"),i,setcolour[i].r,setcolour[i].g,setcolour[i].b,setcolour[i].flash_enabled);
  // }
// AddLog(LOG_LEVEL_TEST, PSTR("\t\tnotif.tSaved.Timeout = millis()"));
  notif.tSaved.Timeout = millis(); // resets timeout counter
  notif.tSaved.TimeoutCounter = millis();
  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_NOTIFICATIONS_ID;
  notif.flags.fForcePanelUpdate = true;



} // END FUNCTION

// const char* mAnimatorLight::GetNotificationModeNamebyID(uint8_t id, char* buffer){
//   // return 
//   //     (id == NOTIF_MODE_STATIC_OFF_ID          ? D_NOTIF_MODE_OFF_NAME_CTR  :
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
//   //   return NOTIF_MODE_STATIC_OFF_ID;
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
uint8_t mAnimatorLight::ConstructJSON_Notifications(uint8_t json_level){

  JsonBuilderI->Start();  
    
    JsonBuilderI->Array_Start("mode");
    for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ JsonBuilderI->Add(notif.pixel[i].mode); }
    JsonBuilderI->Array_End();

    JsonBuilderI->Array_Start("brightness");
    for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ JsonBuilderI->Add(notif.pixel[i].brightness); }
    JsonBuilderI->Array_End();

    JsonBuilderI->Array_Start("speed");
    for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ JsonBuilderI->Add(notif.pixel[i].speed); }
    JsonBuilderI->Array_End();


    
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

  return JsonBuilderI->End();

}

#endif

#endif //USE_MODULE_LIGHTS_ANIMATOR
