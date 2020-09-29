#include "mPWMLight.h"

#ifdef USE_MODULE_LIGHTS_PWM // interface is the gateway

/*** 
 * * Dev notes
 * 
 * 
 * */

// // Used for timed on or off events
int8_t mPWMLight::Tasker(uint8_t function){

    // return 0;

    /************
     * INIT SECTION * 
    *******************/
//   if(function == FUNC_PRE_INIT){
//     //LightModuleInit();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }
    if(function == FUNC_INIT){
      Init();
    }

//   // Check if light is being handled by another function eg ws2812 (long term probably included into this as commands pipe into this)
  if(pCONT_set->light_type > LT_LIGHT_INTERFACE_END){
//     Serial.printf("Skipping light type %d\n\r",pCONT_set->light_type); 
    return FUNCTION_RESULT_MODULE_DISABLED_ID; 
  }

  switch(function){
    
    /************
     * SETTINGS SECTION * 
    *******************/
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_50_MSECOND:{
      LightAnimate();
    }break;

    case FUNC_LOOP:{

      
      if(mSupport::TimeReached(&tSavedAutoOff,1000)){
//   AddLog_P(LOG_LEVEL_TEST,PSTR("pwm"));
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "tSavedAutoOff [%d]"),pCONT_iLight->auto_time_off_secs);
        if(pCONT_iLight->auto_time_off_secs==1){ //if =1 then turn off and clear to 0
          
          pCONT_iLight->setBri(0); //set all off

          //#ifdef ENABLE_LOG_LEVEL_INFO
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "tSavedAutoOff TURN OFF"));
          //#endif
        }else
        if(pCONT_iLight->auto_time_off_secs>1){ //if =1 then turn off and clear to 0
          pCONT_iLight->auto_time_off_secs--; //decrease
        }
      }

      //setColorMode(LCM_BOTH);

      // if(mTime::TimeReached(&tSavedTest, 2000)){
        
      //   switch(test_index++){
      //     default : 
      //       test_index = 0;
      //     case 0: 
      //       setHS(0,255);
      //       setCW(0,0);
      //       // _briCT = 0;
      //       // _briRGB = 255;
      //     break;
      //     case 1:
      //       setHS(120,255);
      //       setCW(0,0);
      //     break;
      //     case 2:
      //       setHS(240,255);
      //       setCW(0,0);
      //     break;
      //     case 3:
      //       setHS(330,255);
      //       setCW(0,0);
      //     break;
      //     case 4: 
      //       setCW(0,255);
      //       setRGB(0,0,0);
      //       // _briCT = 255;
      //       // _briRGB = 0;
      //     break;
      //     case 5: 
      //       setCW(255,0);
      //       setRGB(0,0,0);
      //     break;
      //   }

      //   // fade_running = true;
      //   AddLog_P(LOG_LEVEL_TEST, PSTR("tSavedTest tick test_index=%d"),test_index);
      // }

      
  // Check if we need to run animation
  // if(!fade.running){
  // if (memcmp(last_color, new_color, subtype)) {
  //   // update = true;
  //   fade.running = true;
  //   AddLog_P(LOG_LEVEL_TEST,PSTR("fade.running=true    NEW/LAST Colour do not match"));   
  //   AddLog_P(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
  //     last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
  //     new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
  //     pCONT_set->power
  //   );

      // if (fade_running) {

        //ANIMATE SHOULD HANDLE THIS
      // if(fade.running){
      //   if (LightApplyFade2()) {    
      //       if(mTime::TimeReached(&tSavedTest3, 1)){
      //       //AddLog_P(LOG_LEVEL_TEST, PSTR("LightApplyFade from here"));
      //       LightSetOutputs(fade_cur_10);
      //     }
      //   }
      // }
      
      if (pCONT_iLight->fade.running) {
        if (LightApplyFade2()) {
          AddLog_P(LOG_LEVEL_INFO, PSTR("LightApplyFade2 %d %d %d %d %d"),
            fade_cur_10[0], fade_cur_10[1], fade_cur_10[2], fade_cur_10[3], fade_cur_10[4]);
          LightSetOutputs(fade_cur_10);
        }
      }

    }break;
//     case FUNC_EVERY_50_MSECOND:{

//       //SetPWMChannelSweepRange();
        
     
//       //LightAnimate();

//     }break;
//     case FUNC_EVERY_250_MSECOND:{

//         //  10 bits resolution
//       // update = false;

//       // uint16_t cur_col_10[LST_MAX];   // 10 bits resolution
//       // // first set 8 and 10 bits channels
//       for (uint32_t i = 0; i < LST_MAX; i++) {
//       //   // last_color[i] = pwm_tester_val;//new_color[i];
//       //   // Extend from 8 to 10 bits if no correction (in case no gamma correction is required)
//       //   cur_col_10[i] = pwm_channel_test[i];//change8to10(pwm_tester_val);//new_color[i]);
//         // pwm_channel_test[i] = pwm_tester_val;
//       }
//       // pwm_channel_test[0] = pwm_tester_val;
//       // pwm_channel_test[1] = pwm_channel_test[0];
//       // // pwm_channel_test[2] = pwm_channel_test[0];
//       // pwm_channel_test[3] = pwm_channel_test[0];
//       // pwm_channel_test[4] = pwm_channel_test[0];

//       // uint16_t tmp_colour[5];
//       // for(int i=0;i<5;i++){ tmp_colour[i] = current_color[i]; } 
  
//       // LightSetOutputs(tmp_colour);


//     }break;
//     case FUNC_EVERY_SECOND:{
    
//       uint16_t tmp_colour[5];
//       for(int i=0;i<5;i++){ tmp_colour[i] = current_color[i]; } 
  
//       LightSetOutputs(tmp_colour);


// HsbColor hsb = RgbColor(current_color[0],current_color[1],current_color[2]);

// AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "RGBCCT=%d,%d,%d,%d,%d HSB=%d,%d,%d"),
//   current_color[0],
//   current_color[1],
//   current_color[2],
//   current_color[3],
//   current_color[4],
//   pCONT_rgba->HueF2N(hsb.H),
//   pCONT_rgba->SatF2N(hsb.S),
//   pCONT_rgba->BrtF2N(hsb.B)
// );


//     }break;

    /************
     * COMMANDS SECTION * 
    *******************/

//     case FUNC_SET_POWER:
//       //LightSetPower();
//     break;
//     case FUNC_COMMAND:
//       // result = DecodeCommand(kLightCommands, LightCommand);
//       // if (!result) {
//       //   result = XlgtCall(FUNC_COMMAND);
//       // }
//     break;
    
    /************
     * MQTT SECTION * 
    *******************/
//     #ifdef USE_MQTT
// //     case FUNC_MQTT_SENDER:
// //      SubTasker_MQTTSender();
// //     break;
//     #endif


// //     case FUNC_JSON_COMMAND:
// //       parse_JSONCommand();
// //     break;

//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       char colour_ctr[8];
//       char pwm_channel_chr[12];
//       char stemp[15];

//       pCONT_web->AppendBuffer_PI2(PSTR("<div><b>PWM Testing</b></div>"));

//       for(int i=0;i<5;i++){
//           // pCONT_web->AppendBuffer_PI2(PSTR("<div> PWM%d <span class='mil_pwm_%d'>%d (%d-%d)</span></div>"),i,i,pwm_channel_test[i],0,1023);

//           pCONT_sup->GetTextIndexed_P(colour_ctr, sizeof(colour_ctr), i, kListPWM_TestColours);
//           sprintf(pwm_channel_chr,"pwm%d\0",i);

//           //Red, Green, blue, cold white, warm white
//           pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//             "pwm",               // c - Unique HTML id
//             PSTR("#121212"), colour_ctr,//PSTR("#ff0000"),//"#fff",    // Black to White
//             i,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//             0, 1023,  // Range 0/1 to 100%
//             current_color[i],
//             pwm_channel_chr
//           ); 
//       }

//       RgbColor rgb = RgbColor(current_color[0],current_color[1],current_color[2]);
//       HsbColor hsb = HsbColor(rgb);

//       pCONT_web->AppendBuffer_PI2(PSTR("<tr><td><b>Colour</b></td></tr>"));//GetPaletteFriendlyName(),GetPixelsInMap(palettelist.ptr));
//       pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_GRADIENT2,  // Hue
//         "pwm_hue",             // b - Unique HTML id
//         "#800", "#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800",  // Hue colors
//         2,               // sl2 - Unique range HTML id - Used as source for Saturation end color
//         1, 359,          // Range valid Hue
//         pCONT_rgba->HueF2N(hsb.H),
//         "pwm_hue", 0
//       );         // h0 - Value id


//       uint8_t dcolor = mSupport::changeUIntScale(100, 0, 100, 0, 255);
//       char scolor[8];
//       snprintf_P(scolor, sizeof(scolor), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
      
//       snprintf_P(stemp, sizeof(stemp), PSTR("#%02X%02X%02X"), rgb.R, rgb.G, rgb.B);  // Saturation end color
//       pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_GRADIENT2,  // Saturation
//         "pwm_sat",             // s - Unique HTML id related to eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';
//         scolor, stemp,   // Brightness to max current color
//         3,               // sl3 - Unique range HTML id - Not used
//         0, 100,          // Range 0 to 100%
//         pCONT_rgba->SatF2N(hsb.S),
//         "pwm_sat", 0
//       );         // n0 - Value id


//       // pCONT_web->AppendBuffer_PI2(PSTR("<div> Brightness <span class='hsb_brt_ttl'>%d %%</span></div>"),100);// BrtF2N(animation.brightness))
//       pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
//         "pwm_brt",               // c - Unique HTML id
//         PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
//         4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
//         0, 100,  // Range 0/1 to 100%
//         pCONT_rgba->BrtF2N(hsb.B),
//         "pwm_brt"
//       );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));

//         pCONT_web->WSBufferAppend_PI2(HTTP_MSG_SLIDER_GRADIENT3,  // Cold Warm
//           "pwm_cct",             // a - Unique HTML id
//           "#fff", "#ff0",  // White to Yellow
//           1,               // sl1
//           153, 500,        // Range color temperature
//           LightGetColorTemp(),
//           "pwm_cct"
//         );         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));


//       pCONT_web->AppendBuffer_PI2(PSTR("%s"),PSTR("{t}<tr>"));
//       pCONT_web->AppendBuffer_PI2(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE, 
//                                       36, PSTR("pwm_toggle"), 1, PSTR("PWM Power Toggle"), ""
//                                   );
//       pCONT_web->AppendBuffer_PI2("%s",PSTR("</tr>{t2}"));

//     }break;

  /************
   * WEBPAGE SECTION * 
  *******************/

//     case FUNC_WEB_COMMAND:
//       WebCommand_Parse();
//     break;

  } // end switch
  
  return Tasker_Web(function);

} // END function


void mPWMLight::Init(void) //LightInit(void)
{

//   memset(&pwm_channel_test,0,sizeof(pwm_channel_test));

//   device = pCONT_set->devices_present;
  
//   //subtype = (pCONT_set->light_type & 7) > LST_MAX ? LST_MAX : (pCONT_set->light_type & 7); // Always 0 - LST_MAX (5)
//   subtype = LST_RGBCW;
  
//   pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

//   pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

//   // pCONT_set->Settings.dimmer_hw_min = 0;
//   // pCONT_set->Settings.dimmer_hw_max =100;
//   // pCONT_set->Settings.light_correction = 0;
  
//   // hsb_colour = HsbColor(0);

//   pCONT_set->Settings.light_settings.light_fade = 1;
//   pCONT_set->Settings.light_settings.light_speed = 5*2;
//   pCONT_set->power = 1;

  memset(&fade_start_10,0,sizeof(fade_start_10));

}


// // On entry new_color[5] contains the color to be displayed
// // and last_color[5] the color currently displayed
// // power tells which lights or channels (SetOption68) are on/off
void mPWMLight::LightAnimate(void)
{
  
  uint16_t light_still_on = 0;
  bool power_off = false;

  // make sure we update CT range in case SetOption82 was changed
  // setAlexaCTRange(Settings.flag4.alexa_ct_range);
  pCONT_iLight->strip_timer_counter++;

  // set sleep parameter: either settings,
  // or set a maximum of PWM_MAX_SLEEP if light is on or Fade is running
  // if (pCONT_set->power || fade_running) {
  //   if (pCONT_set->sleep > PWM_MAX_SLEEP) {
  //     pCONT_set->sleep = PWM_MAX_SLEEP;      // set a maxumum value of 50 milliseconds to ensure that animations are smooth
  //   } else {
  //     pCONT_set->sleep = pCONT_set->Settings.sleep;     // or keep the current sleep if it's lower than 50
  //   }
  // } else {
  //   pCONT_set->sleep = pCONT_set->Settings.sleep;
  // }

  // AddLog_P(LOG_LEVEL_TEST, PSTR("LightAnimate pCONT_set->power=%d"),pCONT_set->power);//,pCONT_set->Settings.light_settings.light_scheme);

  // if (!pCONT_set->power) {                   // All channels powered off
  //   AddLog_P(LOG_LEVEL_TEST,PSTR("(!pCONT_set->power)"));

  //   pCONT_set->power = 1;

  //   strip_timer_counter = 0;
  //   if (pCONT_set->Settings.light_settings.light_scheme >= LS_MAX) {
  //     AddLog_P(LOG_LEVEL_TEST, PSTR("power_off = true;"));// pCONT_s
  //     power_off = true;
  //   }
  // } else {
    
    // AddLog_P(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
    //   last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
    //   new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
    //   pCONT_set->power
    // );

    // AddLog_P(LOG_LEVEL_TEST, PSTR("light_scheme=%d"),pCONT_set->Settings.light_settings.light_scheme);

    // switch (pCONT_set->Settings.light_settings.light_scheme) {
    //   case LS_POWER:
     // AddLog_P(LOG_LEVEL_TEST, PSTR("new_color[0]=%d"),new_color[0]);
        pCONT_iLight->calcLevels(pCONT_iLight->new_color);   // I am injecting directly to new_colour
      //AddLog_P(LOG_LEVEL_TEST, PSTR("new_color[0]=%d"),new_color[0]);
        // break;
//       case LS_WAKEUP:
//         if (2 == wakeup_active) {
//           wakeup_active = 1;
//           for (uint32_t i = 0; i < subtype; i++) {
//             new_color[i] = 0;
//           }
//           wakeup_counter = 0;
//           wakeup_dimmer = 0;
//         }
//         wakeup_counter++;
//         if (wakeup_counter > ((pCONT_set->Settings.light_settings.light_wakeup * STATES) / pCONT_set->Settings.light_settings.light_dimmer)) {
//           wakeup_counter = 0;
//           wakeup_dimmer++;
//           if (wakeup_dimmer <= pCONT_set->Settings.light_settings.light_dimmer) {
//             setDimmer(wakeup_dimmer);
//             calcLevels();
//             for (uint32_t i = 0; i < subtype; i++) {
//               new_color[i] = current_color[i];
//             }
//           } else {
// // /
// //             Response_P(PSTR("{\"" D_JSON_WAKEUP "\":\"" D_JSON_DONE "\"}"));
// //             MqttPublishPrefixTopic_P(TELE, PSTR(D_JSON_WAKEUP));
// // *
//             // Response_P(PSTR("{\"" D_JSON_WAKEUP "\":\"" D_JSON_DONE "\""));
//             // LightState(1);
//             // ResponseJsonEnd();
//             // MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_WAKEUP));
//             // XdrvRulesProcess();

//             wakeup_active = 0;
//             pCONT_set->Settings.light_settings.light_scheme = LS_POWER;
//           }
//         }
//         break;
//       case LS_CYCLEUP:
//       case LS_CYCLEDN:
//       case LS_RANDOM:
//         if (LS_CYCLEUP == pCONT_set->Settings.light_settings.light_scheme) {
//           LightCycleColor(1);
//         } else if (LS_CYCLEDN == pCONT_set->Settings.light_settings.light_scheme) {
//           LightCycleColor(-1);
//         } else {
//           LightCycleColor(0);
//         }
//         if (pwm_multi_channels) {     // See #8058
//           new_color[0] = mSupport::changeUIntScale(new_color[0], 0, 255, 0, pCONT_set->Settings.light_settings.light_color[0]);
//           new_color[1] = mSupport::changeUIntScale(new_color[1], 0, 255, 0, pCONT_set->Settings.light_settings.light_color[1]);
//           new_color[2] = mSupport::changeUIntScale(new_color[2], 0, 255, 0, pCONT_set->Settings.light_settings.light_color[2]);
//         }
//         break;
//       default:
//         // XlgtCall(FUNC_SET_SCHEME);

//       break;
    //}

//  }


  // if ((pCONT_set->Settings.light_settings.light_scheme < LS_MAX) || power_off) {  // exclude WS281X Neopixel schemes

  //     AddLog_P(LOG_LEVEL_TEST,PSTR(".light_scheme < LS_MAX) || power_off"));
    // Apply power modifiers to new_color
    // LightApplyPower(new_color, pCONT_set->power);

    // AddLog_P(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
    //   last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
    //   new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
    //   pCONT_set->power
    // );

    // if (memcmp(last_color, new_color, subtype)) {
    //   update = true;
    //   AddLog_P(LOG_LEVEL_TEST,PSTR("memcmp(last_color, new_color, subtype)) NEW COLOUR"));
    // }
    
      // AddLog_P(LOG_LEVEL_INFO,// &tSavedColourChange, 100,
      //   PSTR("HERE last_color (%d,%d,%d,%d,%d) new_color (%d,%d,%d,%d,%d) power %d"),
      //   last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
      //   new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
      //   pCONT_set->power
      // );


    if (memcmp(pCONT_iLight->last_color, pCONT_iLight->new_color, pCONT_iLight->subtype)) {
      pCONT_iLight->update = true;
      pCONT_iLight->fade.running = true;
      // AddLog_P(LOG_LEVEL_TEST,PSTR("fade.running=true    NEW/LAST Colour do not match"));

      
      // AddLog_P(LOG_LEVEL_INFO, &tSavedColourChange, 100,
      //   PSTR("last_color (%03d,%03d,%03d,%03d,%03d) new_color (%03d,%03d,%03d,%03d,%03d) power %d"),
      //   last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
      //   new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
      //   pCONT_set->power
      // );
      // AddLog_P(LOG_LEVEL_INFO, //&tSavedColourChange, 100,
      //   PSTR("last_color (%d,%d,%d,%d,%d) new_color (%d,%d,%d,%d,%d) power %d"),
      //   last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
      //   new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
      //   pCONT_set->power
      // );

    }
    if (pCONT_iLight->update) { 
      // AddLog_P(LOG_LEVEL_TEST,PSTR("if update"));

      uint16_t cur_col_10[LST_MAX];   // 10 bits resolution
      pCONT_iLight->update = false;

      // first set 8 and 10 bits channels
      for (uint32_t i = 0; i < LST_MAX; i++) {
        pCONT_iLight->last_color[i] = pCONT_iLight->new_color[i];
        // Extend from 8 to 10 bits if no correction (in case no gamma correction is required)
        cur_col_10[i] = pCONT_iLight->change8to10(pCONT_iLight->new_color[i]);
      }

// AddLog_Array(LOG_LEVEL_TEST,PSTR("new_color"),new_color,(uint8_t)5);
// AddLog_Array(LOG_LEVEL_TEST,PSTR("3cur_col_10"),cur_col_10,(uint16_t)5);

      // if (pwm_multi_channels) {
      //   calcGammaMultiChannels(cur_col_10);
      // } else {
        // calcGammaBulbs(cur_col_10);
    // // AddLog_P(LOG_LEVEL_TEST,PSTR("calcGammaBulbs else"));
    //     AddLog_Array(LOG_LEVEL_TEST,PSTR("cur_col_10"),cur_col_10,(uint16_t)5);

        // Now see if we need to mix RGB and True White
        // Valid only for LST_RGBW, LST_RGBCW, rgbwwTable[4] is zero, and white is zero (see doc)
        // if ((LST_RGBW <= subtype) && (0 == pCONT_set->Settings.light_settings.rgbwwTable[4]) && (0 == cur_col_10[3]+cur_col_10[4])) {
        //   uint32_t min_rgb_10 = min3(cur_col_10[0], cur_col_10[1], cur_col_10[2]);
        //   for (uint32_t i=0; i<3; i++) {
        //     // substract white and adjust according to rgbwwTable
        //     uint32_t adjust10 = change8to10(pCONT_set->Settings.light_settings.rgbwwTable[i]);
        //     cur_col_10[i] = mSupport::changeUIntScale(cur_col_10[i] - min_rgb_10, 0, 1023, 0, adjust10);
        //   }

        //   // compute the adjusted white levels for 10 and 8 bits
        //   uint32_t adjust_w_10 = mSupport::changeUIntScale(pCONT_set->Settings.light_settings.rgbwwTable[3], 0, 255, 0, 1023);
        //   uint32_t white_10 = mSupport::changeUIntScale(min_rgb_10, 0, 1023, 0, adjust_w_10);  // set white power down corrected with rgbwwTable[3]
        //   if (LST_RGBW == subtype) {
        //     // we simply set the white channel
        //     cur_col_10[3] = white_10;
        //   } else {  // LST_RGBCW
        //     // we distribute white between cold and warm according to CT value
        //     uint32_t ct = getCT10bits();
        //     cur_col_10[4] = mSupport::changeUIntScale(ct, 0, 1023, 0, white_10);
        //     cur_col_10[3] = white_10 - cur_col_10[4];
        //   }
        // }
      //}

      // Apply RGBWWTable only if Settings.rgbwwTable[4] != 0
      // if (0 != pCONT_set->Settings.light_settings.rgbwwTable[4]) {
      //   for (uint32_t i = 0; i<subtype; i++) {
      //     uint32_t adjust = change8to10(pCONT_set->Settings.light_settings.rgbwwTable[i]);
      //     cur_col_10[i] = mSupport::changeUIntScale(cur_col_10[i], 0, 1023, 0, adjust);
      //   }
      // }

      // // final adjusments for PMW, post-gamma correction
      // for (uint32_t i = 0; i < LST_MAX; i++) {
      //   // scale from 0..1023 to 0..pwm_range, but keep any non-zero value to at least 1
      //   cur_col_10[i] = (cur_col_10[i] > 0) ? mSupport::changeUIntScale(cur_col_10[i], 1, 1023, 1, pCONT_set->Settings.pwm_range) : 0;
      // }

      // apply port remapping on both 8 bits and 10 bits versions
      uint16_t orig_col_10bits[LST_MAX];
      memcpy(orig_col_10bits, cur_col_10, sizeof(orig_col_10bits));
      for (uint32_t i = 0; i < LST_MAX; i++) {
        cur_col_10[i] = orig_col_10bits[i];
      }

       

        // AddLog_Array(LOG_LEVEL_TEST,PSTR("4cur_col_10"),cur_col_10,(uint16_t)5);
      // 
        
      // if (
      //   // flag_test){//
      //   !pCONT_set->Settings.light_settings.light_fade || power_off || 

    if(pCONT_iLight->fade_initialized) { // no fade //skip_light_fade
      // AddLog_P(LOG_LEVEL_INFO, PSTR("fade_initialized"));
      // record the current value for a future Fade
      memcpy(fade_start_10, cur_col_10, sizeof(fade_start_10));
      // // push the final values at 8 and 10 bits resolution to the PWMs
      // AddLog_P(LOG_LEVEL_TEST,PSTR("push the final values at 8 and 10 bits resolution to the PWMs"));
      LightSetOutputs(cur_col_10);
      pCONT_iLight->fade.running = true;    
    } else {  // fade on
      pCONT_iLight->fade_initialized = true;      // it is now ok to fade
      // AddLog_Array(LOG_LEVEL_TEST,PSTR("\n\n\n\n\n\n\n\n\nhere cur_col_10"),cur_col_10,(uint16_t)5);
      if (pCONT_iLight->fade.running){ // if fade is running, we take the curring value as the start for the next fade
        memcpy(fade_start_10, fade_cur_10, sizeof(fade_start_10));
      }
      memcpy(fade_end_10, cur_col_10, sizeof(fade_start_10));
      pCONT_iLight->fade.running = true;
      pCONT_iLight->fade.duration_ms = 0;    // set the value to zero to force a recompute
      pCONT_iLight->fade.start_ms = 0;        
    }

    if (pCONT_iLight->fade.running) {
      if (LightApplyFade2()) {
        // AddLog_P(LOG_LEVEL_INFO, PSTR("LightApplyFade2 %d %d %d %d %d"),
        //   fade_cur_10[0], fade_cur_10[1], fade_cur_10[2], fade_cur_10[3], fade_cur_10[4]);
        LightSetOutputs(fade_cur_10);
      }
    }
    
  }
  
}




//#define ENABLE_FADING

bool mPWMLight::LightApplyFade2(void) {   // did the value chanegd and needs to be applied

/***
 * 
 * flag_fade_started
 *  
 * 
 */

  #ifdef ENABLE_FADING

  // if this was not running, calculate transition
  if(fade.duration_ms == 0){

    // Remember start time
    fade.start_ms = millis();

    // compute the distance between start and and color (max of distance for each channel)
    // Get the greatest channel distance
    uint32_t distance = 0;
    for (uint32_t i = 0; i < subtype; i++) {
      int32_t channel_distance = fade_end_10[i] - fade_start_10[i]; ///fadeGammaReverse(i, fade_end_10[i]) - fadeGammaReverse(i, fade_start_10[i]);
      if (channel_distance < 0) {  //invert value
        channel_distance = -channel_distance; 
      }
      if (channel_distance > distance) { 
        distance = channel_distance; 
      } 
    }
    AddLog_P(LOG_LEVEL_TEST, PSTR("Greatest overall distance = %d"),distance);

    // If we have to transition
    if(distance > 0){
      // compute the duration of the animation
      // Note: Settings.light_speed is the number of half-seconds for a 100% fade,
      // i.e. light_speed=1 means 1024 steps in 500ms
      pCONT_set->Settings.light_settings.light_speed = 4; // ie 2 seconds
      fade.duration_ms = (distance * pCONT_set->Settings.light_settings.light_speed * 500) / 1023;
      AddLog_P(LOG_LEVEL_TEST, PSTR("fade.duration_ms = %d"),fade.duration_ms);
    }else{
      fade.running = false;
      AddLog_P(LOG_LEVEL_TEST, PSTR("distance was equal, stop animation"));
      return false; // don't continue
    }

    //remember end time
    fade.end_ms = fade.start_ms + fade.duration_ms;
  }

  uint16_t fade_current = millis() - fade.start_ms;   // number of milliseconds since start of fade
  // If fade is not finished, calculate next step
  if(mTime::MillisReached(&fade.end_ms) != 1){
    for (uint32_t i = 0; i < subtype; i++) {
      // Use longest time scale, to map between the start and end colour
      fade_cur_10[i] = mSupport::changeUIntScale(fade_current,           // Progression through maximum time scale
                                                  0, fade.duration_ms,   // Maximum time scale
                                                  fade_start_10[i], fade_end_10[i]  // Start to end colour
                                                );
      AddLog_P(LOG_LEVEL_TEST, &tSavedFadeProgress, 1000,
        PSTR("start,cur,end progress %d,%d,%d %d/%d"), 
          fade_start_10[i],
          fade_cur_10[i],
          fade_end_10[i],
          fade_current,
          fade.end_ms - millis()
      );      
    }
  }
  //We have reached the end of duration
  else{
    // stop fade
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("----------------------------------------------------------------Stop fade"));
    fade.running = false;
    fade.start_ms = 0;
    fade.duration_ms = 0;
    // set light to target value/end value
    memcpy(fade_cur_10, fade_end_10, sizeof(fade_end_10));
    // record the last value for next start
    memcpy(fade_start_10, fade_end_10, sizeof(fade_start_10));
  }

  #else 
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("------------------------------DISABLED fade")); 

    // AddLog_Array(LOG_LEVEL_TEST,PSTR("fade_end_10"),fade_end_10,(uint16_t)5);
    // AddLog_Array(LOG_LEVEL_TEST,PSTR("fade_start_10"),fade_start_10,(uint16_t)5);
    // AddLog_Array(LOG_LEVEL_TEST,PSTR("cur_col_10"),fade_cur_10,(uint16_t)5);

    // set to be end
    memcpy(fade_cur_10, fade_end_10, sizeof(fade_end_10));
    // record the last value for next start
    memcpy(fade_start_10, fade_end_10, sizeof(fade_start_10));
    //disable fade as completed

    // AddLog_Array(LOG_LEVEL_TEST,PSTR("2fade_end_10"),fade_end_10,(uint16_t)5);
    // AddLog_Array(LOG_LEVEL_TEST,PSTR("2fade_start_10"),fade_start_10,(uint16_t)5);
    // AddLog_Array(LOG_LEVEL_TEST,PSTR("2cur_col_10"),fade_cur_10,(uint16_t)5);

    //test to make it stop
    memcpy(pCONT_iLight->last_color, pCONT_iLight->new_color, sizeof(pCONT_iLight->new_color));

    pCONT_iLight->fade.running = false;
    pCONT_iLight->fade.start_ms = 0;
    pCONT_iLight->fade.duration_ms = 0;
    pCONT_iLight->fade_initialized ^= 1; // set back to not running

    LightSetOutputs(fade_cur_10);
  #endif


  return true;
}






// void mPWMLight::SetPWMChannelSweepRange(){

//   // if(pwm_tester_dir){
//   //   if(pwm_tester_val<1023){ 
//   //     pwm_tester_val+=50;         
//   //   }else{
//   //     pwm_tester_dir = 0; 
//   //   }
//   // }else{
//   //   if(pwm_tester_val>0){ 
//   //     pwm_tester_val-=50;
//   //   }else{
//   //     pwm_tester_dir = 1; 
//   //   }
//   // }

//   // for (uint32_t i = 0; i < LST_MAX; i++) {
//   //   pwm_channel_test[i] = pwm_tester_val;
//   // }

// }


void mPWMLight::LightSetOutputs(const uint16_t *cur_col_10) {
  
//  AddLog_Array(LOG_LEVEL_TEST, &tSavedLightSetOutputs, 100, PSTR("LightSetOutputs cur_col_10"),(uint16_t*)cur_col_10,(uint16_t)5);

  uint16_t cur_col;
  uint32_t pin_num;
  uint16_t pwm_value;

  // now apply the actual PWM values, adjusted and remapped 10-bits range
  if (pCONT_set->light_type < LT_PWM6) {   // only for direct PWM lights, not for Tuya, Armtronix...
    for (uint8_t i = 0; i < (pCONT_iLight->subtype - pCONT_iLight->pwm_offset); i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) {
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        cur_col = cur_col_10[i + pCONT_iLight->pwm_offset]; //leak chance
        if(!pCONT_iLight->isChannelCT(i)) {   // if CT don't use pwm_min and pwm_max
          cur_col = cur_col > 0 ? mSupport::changeUIntScale(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
        }
        pin_num = pCONT_pins->Pin(GPIO_PWM1_ID, i);
        pwm_value = bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col;
        // Serial.printf("%d pin_num=%d, %d/%d, %d\n\r", i, pin_num, cur_col, pCONT_set->Settings.pwm_range, pwm_value);
        analogWrite(pin_num, pwm_value);
      } //pin used
    } //subtype
  } //light type

}

// void mPWMLight::LightSetOutputs(const uint16_t *cur_col_10) {
  
//   // now apply the actual PWM values, adjusted and remapped 10-bits range
//   if (pCONT_set->light_type < LT_PWM6) {   // only for direct PWM lights, not for Tuya, Armtronix...
//     for (uint32_t i = 0; i < (subtype - pwm_offset); i++) {
//       if (pCONT_sup->PinUsed(GPIO_PWM1_ID, i)) {
//         // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
//         uint16_t cur_col = cur_col_10[i + pwm_offset]; //leak chance
//         if (!isChannelCT(i)) {   // if CT don't use pwm_min and pwm_max
//           //cur_col = cur_col > 0 ? mSupport::changeUIntScale(cur_col, 0, Settings.pwm_range, pwm_min, pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
//         }
//         uint32_t pin_num = pCONT_sup->Pin(GPIO_PWM1_ID, i);
//         // Serial.printf("%d cur_col=%d, %d/%d, %d\n\r",i,pin_num,cur_col,pCONT_set->Settings.pwm_range,
//         //   bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col
//         // );
//         analogWrite(pin_num, bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col);
//       } //pin used
//     } //subtype
//   } //light type

// //  char msg[24];
// //  AddLog_P(LOG_LEVEL_DEBUG, PSTR("LGT: Channels %s"), ToHex_P((const unsigned char *)cur_col_10, 10, msg, sizeof(msg)));

//   // uint8_t cur_col[LST_MAX];
//   // for (uint32_t i = 0; i < LST_MAX; i++) {
//   //   cur_col[i] = change10to8(cur_col_10[i]);
//   // }
//   // Some devices need scaled RGB like Sonoff L1
//   // TODO, should be probably moved to the Sonoff L1 support code
//   // uint8_t scale_col[3];
//   // uint32_t max = (cur_col[0] > cur_col[1] && cur_col[0] > cur_col[2]) ? cur_col[0] : (cur_col[1] > cur_col[2]) ? cur_col[1] : cur_col[2];   // 0..255
//   // for (uint32_t i = 0; i < 3; i++) {
//   //   scale_col[i] = (0 == max) ? 255 : (255 > max) ? mSupport::changeUIntScale(cur_col[i], 0, max, 0, 255) : cur_col[i];
//   // }

//   // char *tmp_data = XdrvMailbox.data;
//   // char *tmp_topic = XdrvMailbox.topic;
//   // XdrvMailbox.data = (char*)cur_col;
//   // XdrvMailbox.topic = (char*)scale_col;
//   // if (XlgtCall(FUNC_SET_CHANNELS)) { /* Serviced */ }      // To call sub-interfaces
//   // else if (XdrvCall(FUNC_SET_CHANNELS)) { /* Serviced */ }
//   // XdrvMailbox.data = tmp_data;
//   // XdrvMailbox.topic = tmp_topic;
// }










// void mPWMLight::WebCommand_Parse(void)
// {
//   char tmp[100];

//   uint16_t  arg_value = 0;
//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

//   for (int i=0;i<5;i++){
//     sprintf(arg_ctr,"pwm%d\0",i);
//     if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//       current_color[i] = arg_value;
//       UpdateSetOutputs();
//     }
//   }

//   // HSB COLOURS
//   sprintf(arg_ctr,"pwm_hue0\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     SetColour_Hue(arg_value);
//     UpdateSetOutputs();
//   }

//   sprintf(arg_ctr,"pwm_sat0\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     SetColour_Sat(arg_value);
//     UpdateSetOutputs();
//   }

//   sprintf(arg_ctr,"pwm_brt\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     SetColour_Brt(arg_value);
//     UpdateSetOutputs();
//   }

// }


// void mPWMLight::UpdateSetOutputs(){

//   uint16_t tmp_colour[5];
//   for(int i=0;i<5;i++){ tmp_colour[i] = current_color[i]; } 

//   LightSetOutputs(tmp_colour);

// }


// // /*********************************************************************************************
// // * MQTT ******************************************************************
// // *************************************************************************************************/


// // //relay:1,[1,2],"all",["fountain,bulbs,led"]
// // void mRelays::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
// //   // Check if instruction is for me
// //   if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/relays")>=0){
// //       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //   }else{
// //     return; // not meant for here
// //   }

// //   AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

// //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer2.payload.ctr);

// //   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
// //   DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
// //   JsonObject obj = doc.as<JsonObject>();

// //   uint8_t name_num=-1,state=-1;    

// //   if(obj.containsKey("name")){ Serial.println("containsKey [\"name\"]");
// //     //Check type then resolve to its id number
// //     if(const char* test = obj["name"]){  Serial.println("const char* test");
// //       if((name_num = GetRelayIDbyName(obj["name"]))<0){
// //           Serial.println("!GetRelayIDbyName");
// //         return;
// //       }else{
// //         Serial.println("GetRelayIDbyName");
// //       }//else success
// //     }else if(obj["name"].is<int>()){ Serial.println("[\"name\"].is<int>()");
// //       name_num  = obj["name"];
// //     }

// //   }
// //   if(obj.containsKey("onoff")){ Serial.println("containsKey [\"onoff\"]");

// //     if(const char* test = obj["onoff"]){ Serial.println("obj[\"onoff\"].is<char>()");
// //       if(strstr(obj["onoff"],"ON")){  Serial.println("obj[\"onoff\"].is<char>()");
// //         state = 1;
// //       }else if(strstr(obj["onoff"],"OFF")){  Serial.println("obj[\"onoff\"].is<char>()");
// //         state = 0;
// //       }
// //     }else if(obj["onoff"].is<int>()){ Serial.println("obj[\"onoff\"].is<int>()");
// //       state  = obj["onoff"];
// //     }
    
// //   }

// //   if((name_num>-1)&&(state>-1)){
// //     SetRelay(name_num,state);





// //   }

// // }



// // // NEW METHOD -- first senders then on internals
// // void mRelays::SubTasker_MQTTSender(){

// //   if(abs(millis()-tSavedForceUpdate)>(60000*10)){tSavedForceUpdate = millis();Serial.println("tSavedForceUpdate");
// //     fForceMQTTUpdate = true;
// //   }

// //   // if(abs(millis()-tSavedTick)>1000){tSavedTick = millis();//Serial.println("MQTTSendRelayStatesIfChanged");

// //   // }

// //   MQTTSendRelayStatesIfChanged();

// //   fForceMQTTUpdate = false;
  
// //     // Serial.println("nulled-a");
// //     // Serial.flush();
// //     // delay(2000);

// //     // Serial.printf("TEST %s\n\r\n\r\n\r\n\r",pCONT->mt->cSavedMinutes);

// //   // if(pCONT->mqt == nullptr){
// //   //   Serial.println("nulled");
// //   //   return;
// //   // }else{
// //   //   Serial.println("NOT nulled");
// //   // }

// // }


// // void mRelays::MQTTSendRelayStatesIfChanged(void){
// //   ConstructJSON_RelayStates(); //create data
// //   if(data_buffer2.payload.len>3){
// //     pCONT->mqt->ppublish("status/relays/ifchanged/state",data_buffer2.payload.ctr,false);
// //   }
// // }
// // void mRelays::ConstructJSON_RelayStates(){

// //   memset(&data_buffer2,0,sizeof(data_buffer2));

// //   DynamicJsonDocument doc(MQTT_MAX_PACKET_SIZE);
// //   JsonObject root = doc.to<JsonObject>();

// //   for(int device_id=0;device_id<RELAYS_CONNECTED;device_id++){

// //     if(relay_status[device_id].ischanged||fForceMQTTUpdate){ relay_status[device_id].ischanged=false;

// //       JsonObject relayobj = root.createNestedObject(GetRelayNamebyIDCtr(device_id));

// //       relayobj["onoff"] = GetRelay(device_id);
// //       relayobj["friendlyname"] = GetRelayNamebyIDCtr(device_id);

// //       char time_ctr[40];
// //       memset(time_ctr,'\0',sizeof(time_ctr));
// //       sprintf(time_ctr, "%02d:%02d:%02d",relay_status[device_id].ontime.hour,relay_status[device_id].ontime.minute,relay_status[device_id].ontime.second);
// //       relayobj["ontime"] = time_ctr;//pCONT->mt->getFormattedTime();

// //       memset(time_ctr,'\0',sizeof(time_ctr));
// //       sprintf(time_ctr, "%02d:%02d:%02d",relay_status[device_id].offtime.hour,relay_status[device_id].offtime.minute,relay_status[device_id].offtime.second);
// //       relayobj["offtime"] = time_ctr;//pCONT->mt->getFormattedTime();

// //     }
// //   }


// // //need to add a build this flag if changes were made (previously called json_pairs)
// //   data_buffer2.payload.len = measureJson(root)+1;
// //   serializeJson(doc,data_buffer2.payload.ctr);

// // }
// // //#endif





// // /*********************************************************************************************
// // * Input & Output Interface ******************************************************************
// // *************************************************************************************************/

// // //tmp fix
// // void mRelays::SetRelay(uint8_t state){
// //   SetRelay(0,state);
// // }

// //   //flip args around so the single arg means state of relay 0
// // void mRelays::SetRelay(uint8_t num, uint8_t state){

// //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " " D_JSON_COMMAND_NVALUE " " D_JSON_COMMAND_NVALUE)
// //     ,"SetRelay","num",num,"state",state);     
  
// //   //error patch  within vaible range
// //   if(!((num>=0)&&(num<=RELAYS_CONNECTED))){
// //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " Command Invalid"),"SetRelay");
// //     return;
// //   }

// //   if(GetRelay(num)==state){
// //     relay_status[num].ischanged = false;
// //     return;
// //   }else{
// //     relay_status[num].ischanged = true;
// //   }

// //   relay_status[num].onoff = state;

// //   if(state){ relay_status[num].ontime = pCONT->mt->mtime;
// //   }else{ relay_status[num].offtime = pCONT->mt->mtime; }


// //   ExecuteCommandPower(num,state,SRC_MQTT);

// //   // switch(num){
// //   //   #ifdef RELAY_0_PIN
// //   //     case RELAY_0_ID: return RELAY_0_SET(state);
// //   //   #endif
// //   //   #ifdef RELAY_1_PIN
// //   //     case RELAY_1_ID: return RELAY_1_SET(state);
// //   //   #endif
// //   //   #ifdef RELAY_2_PIN
// //   //     case RELAY_2_ID: return RELAY_2_SET(state);
// //   //   #endif
// //   //   #ifdef RELAY_3_PIN
// //   //     case RELAY_3_ID: return RELAY_3_SET(state);
// //   //   #endif
// //   // }

// // }

// // uint8_t mRelays::GetRelay(uint8_t num){


// //   return bitRead(pCONT_set->power, num);



// //   // switch(num){
// //   //   #ifdef RELAY_0_PIN
// //   //     case RELAY_0_ID: return RELAY_0_ONOFF();
// //   //   #endif
// //   //   #ifdef RELAY_1_PIN
// //   //     case RELAY_1_ID: return RELAY_1_ONOFF();
// //   //   #endif
// //   //   #ifdef RELAY_2_PIN
// //   //     case RELAY_2_ID: return RELAY_2_ONOFF();
// //   //   #endif
// //   //   #ifdef RELAY_3_PIN
// //   //     case RELAY_3_ID: return RELAY_3_ONOFF();
// //   //   #endif
// //   // }
// //   return 0;
// // }

// // /*********************************************************************************************
// // * HELPER & CONVERSION FUNCTIONS ******************************************************************
// // *************************************************************************************************/

// // const char* mRelays::GetRelayNamebyIDCtr(uint8_t device_id){
// //   switch(device_id){
// //     #ifdef RELAY_0_PIN
// //       case RELAY_0_ID: return D_DEVICE_RELAY_0_NAME;
// //     #endif
// //     #ifdef RELAY_1_PIN
// //       case RELAY_1_ID: return D_DEVICE_RELAY_1_NAME;
// //     #endif
// //     #ifdef RELAY_2_PIN
// //       case RELAY_2_ID: return D_DEVICE_RELAY_2_NAME;
// //     #endif
// //     #ifdef RELAY_3_PIN
// //       case RELAY_3_ID: return D_DEVICE_RELAY_3_NAME;
// //     #endif
// //   }
// //   return '\0'; //return null
// // }
// // const char* mRelays::GetRelayNamebyIDCtr(uint8_t device_id){
// //   switch(device_id){
// //     #ifdef D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG
// //       case RELAY_0_ID: return D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG;
// //     #endif
// //     #ifdef D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG
// //       case RELAY_1_ID: return D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG;
// //     #endif
// //     #ifdef D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG
// //       case RELAY_2_ID: return D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG;
// //     #endif
// //     #ifdef D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG
// //       case RELAY_3_ID: return D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG;
// //     #endif
// //   }
// //   return '\0'; //return null
// // }
// // int8_t mRelays::GetRelayIDbyName(const char* c){
// //   if(c=='\0'){ return -1; }  

// //   #ifdef RELAY_0_PIN
// //     if(strstr(c,D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG)||strstr(c,D_DEVICE_RELAY_0_NAME)){
// //       return RELAY_0_ID;
// //     }
// //   #endif
// //   #ifdef RELAY_1_PIN
// //     else if(strstr(c,D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG)||strstr(c,D_DEVICE_RELAY_1_NAME)){
// //       return RELAY_1_ID;
// //     }
// //   #endif
// //   #ifdef RELAY_2_PIN
// //     else if(strstr(c,D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG)||strstr(c,D_DEVICE_RELAY_2_NAME)){
// //       return RELAY_2_ID;
// //     }
// //   #endif
// //   #ifdef RELAY_3_PIN
// //     else if(strstr(c,D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG)||strstr(c,D_DEVICE_RELAY_3_NAME)){
// //       return RELAY_3_ID;
// //     }
// //   #endif
// //     else{
// //       return -1;
// //     }
// // }





// // /********************************************************************************************/

// // void mRelays::SetLatchingRelay(power_t lpower, uint32_t state)
// // {
// //   // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
// //   // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
// //   // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
// //   // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On

// //   if (state && !pCONT_set->latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
// //     latching_power = lpower;
// //     pCONT_set->latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
// //   }

// //   for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
// //     uint32_t port = (i << 1) + ((latching_power >> i) &1);
// //     pCONT_sup->DigitalWrite(GPIO_REL1 +port, bitRead(rel_inverted, port) ? !state : state);
// //   }
// // }

// // void mRelays::SetDevicePower(power_t rpower, uint32_t source)
// // {
// //   pCONT_sup->ShowSource(source);
// //   pCONT_set->last_source = source;

// //   if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {  // All on and stay on
// //     pCONT_set->power = (1 << pCONT_set->devices_present) -1;
// //     rpower = pCONT_set->power;
// //   }

// //   // if (pCONT_set->Settings.flag_system_phaseout.interlock) {          // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
// //   //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
// //   //     power_t mask = 1;
// //   //     uint32_t count = 0;
// //   //     for (uint32_t j = 0; j < pCONT_set->devices_present; j++) {
// //   //       if ((pCONT_set->Settings.interlock[i] & mask) && (rpower & mask)) {
// //   //         count++;
// //   //       }
// //   //       mask <<= 1;
// //   //     }
// //   //     if (count > 1) {
// //   //       mask = ~pCONT_set->Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
// //   //       power &= mask;
// //   //       rpower &= mask;
// //   //     }
// //   //   }
// //   // }

// //   if (rpower) {                           // Any power set
// //     last_power = rpower;
// //   }

// //   pCONT_set->XdrvMailbox.index = rpower;
// //   pCONT->Tasker_Interface(FUNC_SET_POWER);               // Signal power state

// //   pCONT_set->XdrvMailbox.index = rpower;
// //   pCONT_set->XdrvMailbox.payload = source;

// //   // if (pCONT->Tasker_Interface(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
// //   //   // Serviced
// //   // }
// //   // else if ((MODULE_SONOFF_DUAL == pCONT_set->my_module_type) || (MODULE_CH4 == pCONT_set->my_module_type)) {
// //   //   Serial.write(0xA0);
// //   //   Serial.write(0x04);
// //   //   Serial.write(rpower &0xFF);
// //   //   Serial.write(0xA1);
// //   //   Serial.write('\n');
// //   //   Serial.flush();
// //   // }
// //   // else if (MODULE_EXS_RELAY == pCONT_set->my_module_type) {
// //   //   SetLatchingRelay(rpower, 1);
// //   // }
// //   // else {
// //     for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
// //       power_t state = rpower &1;
// //       if (i < MAX_RELAYS) {
// //         pCONT_sup->DigitalWrite(GPIO_REL1 +i, bitRead(rel_inverted, i) ? !state : state);
// //       }
// //       rpower >>= 1;
// //     }
// //   //}
// // }

// // void mRelays::RestorePower(bool publish_power, uint32_t source)
// // {
// //   if (pCONT_set->power != last_power) {
// //     SetDevicePower(last_power, source);
// //     if (publish_power) {
// //       //MqttPublishAllPowerState();
// //     }
// //   }
// // }


// // void mRelays::SetAllPower(uint32_t state, uint32_t source)
// // {
// // // state 0 = POWER_OFF = Relay Off
// // // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// // // state 2 = POWER_TOGGLE = Toggle relay
// // // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// // // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// // // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// // // state 16 = POWER_SHOW_STATE = Show power state

// //   bool publish_power = true;
// //   if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
// //     state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
// //     publish_power = false;
// //   }
// //   if ((state >= POWER_OFF) && (state <= POWER_TOGGLE)) {
// //     power_t all_on = (1 << pCONT_set->devices_present) -1;
// //     switch (state) {
// //     case POWER_OFF:
// //       pCONT_set->power = 0;
// //       break;
// //     case POWER_ON:
// //       pCONT_set->power = all_on;
// //       break;
// //     case POWER_TOGGLE:
// //       pCONT_set->power ^= all_on;                    // Complement current state
// //     }
// //     SetDevicePower(pCONT_set->power, source);
// //   }
// //   if (publish_power) {
// //     //MqttPublishAllPowerState();
// //   }
// // }

// // void mRelays::SetPowerOnState(void)
// // {
// //   // if (MODULE_MOTOR == pCONT_set->my_module_type) {
// //   //   pCONT_set->Settings.poweronstate = POWER_ALL_ON;   // Needs always on else in limbo!
// //   // }
// //   // if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {
// //   //   SetDevicePower(1, SRC_RESTART);
// //   // } else {
// //   //   if ((pCONT_sup->ResetReason() == REASON_DEFAULT_RST) || (pCONT_sup->ResetReason() == REASON_EXT_SYS_RST)) {
// //       switch (pCONT_set->Settings.poweronstate) {
// //       case POWER_ALL_OFF:
// //       case POWER_ALL_OFF_PULSETIME_ON:
// //         pCONT_set->power = 0;
// //         SetDevicePower(pCONT_set->power, SRC_RESTART);
// //         break;
// //       case POWER_ALL_ON:  // All on
// //         pCONT_set->power = (1 << pCONT_set->devices_present) -1;
// //         SetDevicePower(pCONT_set->power, SRC_RESTART);
// //         break;
// //       case POWER_ALL_SAVED_TOGGLE:
// //         pCONT_set->power = (pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) -1)) ^ POWER_MASK;
// //         if (pCONT_set->Settings.flag_system_phaseout.save_state) {  // SetOption0 - Save power state and use after restart
// //           SetDevicePower(pCONT_set->power, SRC_RESTART);
// //         }
// //         break;
// //       case POWER_ALL_SAVED:
// //         pCONT_set->power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) -1);
// //         if (pCONT_set->Settings.flag_system_phaseout.save_state) {  // SetOption0 - Save power state and use after restart
// //           SetDevicePower(pCONT_set->power, SRC_RESTART);
// //         }
// //         break;
// //       }
// //     // } else {
// //     //   power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) -1);
// //     //   if (pCONT_set->Settings.flag_system_phaseout.save_state) {    // SetOption0 - Save power state and use after restart
// //     //     SetDevicePower(pCONT_set->power, SRC_RESTART);
// //     //   }
// //     // }
// //   //}

// //   // Issue #526 and #909
// //   // for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
// //   //   if (!pCONT_set->Settings.flag_network_phaseout.no_power_feedback) {  // SetOption63 - Don't scan relay pCONT_set->power state at restart - #5594 and #5663
// //   //     if ((i < MAX_RELAYS) && (pCONT_set->pin[GPIO_REL1 +i] < 99)) {
// //   //       bitWrite(pCONT_set->power, i, digitalRead(pCONT_set->pin[GPIO_REL1 +i]) ^ bitRead(pCONT_set->rel_inverted, i));
// //   //     }
// //   //   }
// //   //   if ((i < MAX_PULSETIMERS) && (bitRead(pCONT_set->power, i) || (POWER_ALL_OFF_PULSETIME_ON == pCONT_set->Settings.poweronstate))) {
// //   //     pCONT_sup->SetPulseTimer(i, pCONT_set->Settings.pulse_timer[i]);
// //   //   }
// //   // }
// //   blink_powersave = pCONT_set->power;
// // }




// // void mRelays::ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
// // {
// // // device  = Relay number 1 and up
// // // state 0 = POWER_OFF = Relay Off
// // // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// // // state 2 = POWER_TOGGLE = Toggle relay
// // // state 3 = POWER_BLINK = Blink relay
// // // state 4 = POWER_BLINK_STOP = Stop blinking relay
// // // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// // // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// // // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// // // state 16 = POWER_SHOW_STATE = Show power state

// // //  ShowSource(source);

// // // #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
// // //   if (IsModuleIfan()) {
// // //     blink_mask &= 1;                 // No blinking on the fan relays
// // //     Settings.flag_system_phaseout.interlock = 0;     // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
// // //     Settings.pulse_timer[1] = 0;     // No pulsetimers on the fan relays
// // //     Settings.pulse_timer[2] = 0;
// // //     Settings.pulse_timer[3] = 0;
// // //   }
// // // #endif  // USE_MODULE_CUSTOM_SONOFF_IFAN

// // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_RELAYS "ExecuteCommandPower(%d,%d,%d)"),device,state,source);

// //   bool publish_power = true;
// //   if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
// //     state &= 3;                      // POWER_OFF, POWER_ON or POWER_TOGGLE
// //     publish_power = false;
// //   }

// //   if ((device < 1) || (device > pCONT_set->devices_present)) {
// //     device = 1;
// //   }
// //   active_device = device;

// //   // if (device <= MAX_PULSETIMERS) {
// //   //   SetPulseTimer(device -1, 0);
// //   // }
// //   power_t mask = 1 << (device -1);        // Device to control
// //   if (state <= POWER_TOGGLE) {
// //     // if ((blink_mask & mask)) {
// //     //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
// //     //   MqttPublishPowerBlinkState(device);
// //     // }

// //     // if (Settings.flag_system_phaseout.interlock &&        // CMND_INTERLOCK - Enable/disable interlock
// //     //     !interlock_mutex &&
// //     //     ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(power & mask)))
// //     //    ) {
// //     //   interlock_mutex = true;                           // Clear all but masked relay in interlock group if new set requested
// //     //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
// //     //     if (Settings.interlock[i] & mask) {             // Find interlock group
// //     //       for (uint32_t j = 0; j < devices_present; j++) {
// //     //         power_t imask = 1 << j;
// //     //         if ((Settings.interlock[i] & imask) && (power & imask) && (mask != imask)) {
// //     //           ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
// //     //           delay(50);                                // Add some delay to make sure never have more than one relay on
// //     //         }
// //     //       }
// //     //       break;                                        // An interlocked relay is only present in one group so quit
// //     //     }
// //     //   }
// //     //   interlock_mutex = false;
// //     // }

// //     switch (state) {
// //     case POWER_OFF: {
// //       pCONT_set->power &= (POWER_MASK ^ mask);
// //       break; }
// //     case POWER_ON:
// //       pCONT_set->power |= mask;
// //       break;
// //     case POWER_TOGGLE:
// //       pCONT_set->power ^= mask;
// //     }
// //     SetDevicePower(pCONT_set->power, source);
// // // #ifdef USE_DOMOTICZ
// // //     DomoticzUpdatePowerState(device);
// // // #endif  // USE_DOMOTICZ
// // // #ifdef USE_KNX
// // //     KnxUpdatePowerState(device, power);
// // // #endif  // USE_KNX
// //     // if (publish_power && Settings.flag_network_phaseout.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
// //     //   MqttPublishTeleState();
// //     // }
// //     // if (device <= MAX_PULSETIMERS) {  // Restart PulseTime if powered On
// //     //   SetPulseTimer(device -1, (((POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? ~power : power) & mask) ? Settings.pulse_timer[device -1] : 0);
// //     // }
// //   }
// //   // else if (POWER_BLINK == state) {
// //   //   if (!(blink_mask & mask)) {
// //   //     blink_powersave = (blink_powersave & (POWER_MASK ^ mask)) | (power & mask);  // Save state
// //   //     blink_power = (power >> (device -1))&1;  // Prep to Toggle
// //   //   }
// //   //   blink_timer = millis() + 100;
// //   //   blink_counter = ((!Settings.blinkcount) ? 64000 : (Settings.blinkcount *2)) +1;
// //   //   blink_mask |= mask;  // Set device mask
// //   //   MqttPublishPowerBlinkState(device);
// //   //   return;
// //   // }
// //   // else if (POWER_BLINK_STOP == state) {
// //   //   bool flag = (blink_mask & mask);
// //   //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
// //   //   MqttPublishPowerBlinkState(device);
// //   //   if (flag) {
// //   //     ExecuteCommandPower(device, (blink_powersave >> (device -1))&1, SRC_IGNORE);  // Restore state
// //   //   }
// //   //   return;
// //   // }
// //   // if (publish_power) {
// //   //   MqttPublishPowerState(device);
// //   // }
// // }






// // /*********************************************************************************************************************************************
// // ******** MQTT Stuff **************************************************************************************************************************************
// // **********************************************************************************************************************************************
// // ********************************************************************************************************************************************/
// // ////////////////////// START OF MQTT /////////////////////////

// // void mEnergy::MQTTHandler_Init(){

// //   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
// //   mqtthandler_ptr->tSavedLastSent = millis();
// //   mqtthandler_ptr->fPeriodicEnabled = true;
// //   mqtthandler_ptr->fSendNow = true;
// //   mqtthandler_ptr->tRateSecs = 60; 
// //   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
// //   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
// //   mqtthandler_ptr->postfix_topic = postfix_topic_settings;
// //   mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Settings;

// //   mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
// //   mqtthandler_ptr->tSavedLastSent = millis();
// //   mqtthandler_ptr->fPeriodicEnabled = true;
// //   mqtthandler_ptr->fSendNow = true;
// //   mqtthandler_ptr->tRateSecs = 60; 
// //   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
// //   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
// //   mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
// //   mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Sensor;

// //   mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
// //   mqtthandler_ptr->tSavedLastSent = millis();
// //   mqtthandler_ptr->fPeriodicEnabled = true;
// //   mqtthandler_ptr->fSendNow = true;
// //   mqtthandler_ptr->tRateSecs = 1; 
// //   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
// //   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
// //   mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
// //   mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Sensor;
  
// //   mqtthandler_ptr = &mqtthandler_energystats_teleperiod;
// //   mqtthandler_ptr->tSavedLastSent = millis();
// //   mqtthandler_ptr->fPeriodicEnabled = true;
// //   mqtthandler_ptr->fSendNow = true;
// //   mqtthandler_ptr->tRateSecs = 60; 
// //   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
// //   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
// //   mqtthandler_ptr->postfix_topic = postfix_topic_energystats;
// //   mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_EnergyStats;
  
// //   mqtthandler_ptr = &mqtthandler_energystats_ifchanged;
// //   mqtthandler_ptr->tSavedLastSent = millis();
// //   mqtthandler_ptr->fPeriodicEnabled = true;
// //   mqtthandler_ptr->fSendNow = true;
// //   mqtthandler_ptr->tRateSecs = 1; 
// //   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
// //   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
// //   mqtthandler_ptr->postfix_topic = postfix_topic_energystats;
// //   mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_EnergyStats;

// // } //end "MQTTHandler_Init"


// // void mEnergy::MQTTHandler_Set_fSendNow(){

// //   mqtthandler_settings_teleperiod.fSendNow = true;
// //   mqtthandler_sensor_ifchanged.fSendNow = true;
// //   mqtthandler_sensor_teleperiod.fSendNow = true;
// //   mqtthandler_energystats_ifchanged.fSendNow = true;
// //   mqtthandler_energystats_teleperiod.fSendNow = true;

// // } //end "MQTTHandler_Init"


// // void mEnergy::MQTTHandler_Set_TelePeriod(){

// //   mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
// //   mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
// //   mqtthandler_energystats_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// // } //end "MQTTHandler_Set_TelePeriod"



// // void mEnergy::MQTTHandler_Sender(uint8_t mqtt_handler_id){

// //   uint8_t flag_handle_all = false, handler_found = false;
// //   if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

// //   do{

// //     switch(mqtt_handler_id){
// //       case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
// //       case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
// //       case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
// //       case MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID:   handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_ifchanged; break;
// //       case MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID:  handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_teleperiod; break;
// //       default: handler_found=false; break; // nothing 
// //     } // switch

// //     // Pass handlers into command to test and (ifneeded) execute
// //     if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_ENERGY_ID,mqtthandler_ptr); }

// //     // stop searching
// //     if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

// //   }while(flag_handle_all);

// // }


// // ////////////////////// END OF MQTT /////////////////////////





// // // #endif


// // /*
// //   xdrv_04_ino - PWM, WS2812 and sonoff led support for Tasmota

// //   Copyright (C) 2020  Theo Arends

// //   This program is free software: you can redistribute it and/or modify
// //   it under the terms of the GNU General Public License as published by
// //   the Free Software Foundation, either version 3 of the License, or
// //   (at your option) any later version.

// //   This program is distributed in the hope that it will be useful,
// //   but WITHOUT ANY WARRANTY; without even the implied warranty of
// //   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// //   GNU General Public License for more details.

// //   You should have received a copy of the GNU General Public License
// //   along with this program.  If not, see <http://www.gnu.org/licenses/>.
// // */

// // // #ifdef USE_LIGHT
// // /*********************************************************************************************\
// //  * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
// //  *
// //  * light_type  Module     Color  ColorTemp  Modules
// //  * ----------  ---------  -----  ---------  ----------------------------
// //  *  0          -                 no         (Sonoff Basic)
// //  *  1          PWM1       W      no         (Sonoff BN-SZ)
// //  *  2          PWM2       CW     yes        (Sonoff Led)
// //  *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
// //  *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
// //  *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
// //  *  9          reserved          no
// //  * 10          reserved          yes
// //  * 11          +WS2812    RGB    no         (One WS2812 RGB or RGBW ledstrip)
// //  * 12          AiLight    RGBW   no
// //  * 13          Sonoff B1  RGBCW  yes
// //  *
// //  * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
// //  * ------------  ------  ---------  ----------  -----------------
// //  *  0            yes     yes        yes         Color On/Off
// //  *  1            yes     yes        yes         Wakeup light
// //  *  2            yes     yes        no          Color cycle RGB
// //  *  3            yes     yes        no          Color cycle RBG
// //  *  4            yes     yes        no          Random RGB colors
// //  *  5            yes     no         no          Clock
// //  *  6            yes     no         no          Incandescent
// //  *  7            yes     no         no          RGB
// //  *  8            yes     no         no          Christmas
// //  *  9            yes     no         no          Hanukkah
// //  * 10            yes     no         no          Kwanzaa
// //  * 11            yes     no         no          Rainbow
// //  * 12            yes     no         no          Fire
// //  *
// // \*********************************************************************************************/

// // /*********************************************************************************************\
// //  *
// //  * Light management has been refactored to provide a cleaner class-based interface.
// //  * Also, now all values are stored as integer, no more floats that could generate
// //  * rounding errors.
// //  *
// //  * Two singletons are now used to control the state of the 
// //  *  - light_state (LightStateClass) stores the color / white temperature and
// //  *    brightness. Use this object to READ only.
// //  *  - light_controller (LightControllerClass) is used to change light state
// //  *    and adjust all Settings and levels accordingly.
// //  *    Always use this object to change light status.
// //  *
// //  * As there have been lots of changes in light control, here is a summary out
// //  * the whole flow from setting colors to drving the PMW pins.
// //  *
// //  * 1.  To change colors, always use 'light_controller' object.
// //  *     'light_state' is only to be used to read current state.
// //  *  .a For color bulbs, set color via changeRGB() or changeHS() for Hue/Sat.
// //  *     Set the overall brightness changeBri(0..255) or changeDimmer(0..100%)
// //  *     RGB and Hue/Sat are always kept in sync. Internally, RGB are stored at
// //  *     full range (max brightness) so that when you reduce brightness and
// //  *     raise it back again, colors don't change due to rounding errors.
// //  *  .b For white bulbs with Cold/Warm colortone, use changeCW() or changeCT()
// //  *     to change color-tone. Set overall brightness separately.
// //  *     Color-tone temperature can range from 153 (Cold) to 500 (Warm).
// //  *     SetOption82 can expand the rendering from 200-380 due to Alexa reduced range.
// //  *     CW channels are stored at full brightness to avoid rounding errors.
// //  *  .c Alternatively, you can set all 5 channels at once with changeChannels(),
// //  *     in this case it will also set the corresponding brightness.
// //  *
// //  * 2.a After any change, the Settings object is updated so that changes
// //  *     survive a reboot and can be stored in flash - in saveSettings()
// //  *  .b Actual channel values are computed from RGB or CT combined with brightness.
// //  *     Range is still 0..255 (8 bits) - in getActualRGBCW()
// //  *  .c The 5 internal channels RGBWC are mapped to the actual channels supported
// //  *     by the light_type: in calcLevels()
// //  *     1 channel  - 0:Brightness
// //  *     2 channels - 0:Coldwhite 1:Warmwhite
// //  *     3 channels - 0:Red 1:Green 2:Blue
// //  *     4 chennels - 0:Red 1:Green 2:Blue 3:White
// //  *     5 chennels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
// //  *
// //  * 3.  In LightAnimate(), final PWM values are computed at next tick.
// //  *  .a If color did not change since last tick - ignore.
// //  *  .b Extend resolution from 8 bits to 10 bits, which makes a significant
// //  *     difference when applying gamma correction at low brightness.
// //  *  .c Apply Gamma Correction if LedTable==1 (by default).
// //  *     Gamma Correction uses an adaptative resolution table from 11 to 8 bits.
// //  *  .d For Warm/Cold-white channels, Gamma correction is calculated in combined mode.
// //  *     Ie. total white brightness (C+W) is used for Gamma correction and gives
// //  *     the overall light power required. Then this light power is split among
// //  *     Wamr/Cold channels.
// //  *  .e Gamma correction is still applied to 8 bits channels for compatibility
// //  *     with other non-PMW modules.
// //  *  .f Apply color balance correction from rgbwwTable[].
// //  *     Note: correction is done after Gamma correction, it is meant
// //  *     to adjust leds with different power
// //  *  .g If rgbwwTable[4] is zero, blend RGB with White and adjust the level of
// //  *     White channel according to rgbwwTable[3]
// //  *  .h Scale ranges from 10 bits to 0..PWMRange (by default 1023) so no change
// //  *     by default.
// //  *  .i Apply port remapping from Option37
// //  *  .j Invert PWM value if port is of type PMWxi instead of PMWx
// //  *  .k Apply PWM value with analogWrite() - if pin is configured
// //  *
// // \*********************************************************************************************/

// // #define XDRV_04              4
// // // #define DEBUG_LIGHT


// // const uint8_t LIGHT_COLOR_SIZE = 25;   // Char array scolor size

//                         // const char kLightCommands[] PROGMEM = "|"  // No prefix
//                         //   D_JSON_COLOR "|" D_JSON_COLORTEMPERATURE "|" D_JSON_DIMMER "|" D_JSON_DIMMER_RANGE "|" D_JSON_LEDTABLE "|" D_JSON_FADE "|"
//                         //   D_JSON_RGBWWTABLE "|" D_JSON_SCHEME "|" D_JSON_SPEED "|" D_JSON_WAKEUP "|" D_JSON_WAKEUPDURATION "|"
//                         //   D_JSON_WHITE "|" D_JSON_CHANNEL "|" D_JSON_HSBCOLOR
//                         // #ifdef USE_LIGHT_PALETTE
//                         //   "|" D_JSON_PALETTE
//                         // #endif  // USE_LIGHT_PALETTE
//                         // #ifdef USE_DGR_LIGHT_SEQUENCE
//                         //   "|" D_JSON_SEQUENCE_OFFSET
//                         // #endif  // USE_DGR_LIGHT_SEQUENCE
//                         //    "|UNDOCA" ;

//                         // void (* const LightCommand[])(void) PROGMEM = {
//                         //   &CmndColor, &CmndColorTemperature, &CmndDimmer, &CmndDimmerRange, &CmndLedTable, &CmndFade,
//                         //   &CmndRgbwwTable, &CmndScheme, &CmndSpeed, &CmndWakeup, &CmndWakeupDuration,
//                         //   &CmndWhite, &CmndChannel, &CmndHsbColor,
//                         // #ifdef USE_LIGHT_PALETTE
//                         //   &CmndPalette,
//                         // #endif  // USE_LIGHT_PALETTE
//                         // #ifdef USE_DGR_LIGHT_SEQUENCE
//                         //   &CmndSequenceOffset,
//                         // #endif  // USE_DGR_LIGHT_SEQUENCE
//                         //   &CmndUndocA };

// // power_t LightPower(void)
// // {
// //   return power;                     // Make external
// // }

// // // IRAM variant for rotary
// // #ifndef ARDUINO_ESP8266_RELEASE_2_3_0      // Fix core 2.5.x ISR not in IRAM Exception
// // power_t LightPowerIRAM(void) ICACHE_RAM_ATTR;
// // #endif  // ARDUINO_ESP8266_RELEASE_2_3_0

// // power_t LightPowerIRAM(void)
// // {
// //   return power;                     // Make external
// // }

// // uint8_t LightDevice(void)
// // {
// //   return device;                    // Make external
// // }

// // static uint32_t min3(uint32_t a, uint32_t b, uint32_t c) {
// //   return (a < b && a < c) ? a : (b < c) ? b : c;
// // }

// // //
// // // LightStateClass
// // // This class is an abstraction of the current light state.
// // // It allows for b/w, full colors, or white colortone
// // //
// // // This class has 2 independant slots
// // // 1/ Brightness 0.255, dimmer controls both RGB and WC (warm-cold)
// // // 1/ RGB and Hue/Sat - always kept in sync and stored at full brightness,
// // //    i.e. R G or B are 255
// // //    briRGB specifies the brightness for the RGB slot.
// // //    If Brightness is 0, it is equivalent to Off (for compatibility)
// // //    Dimmer is Brightness converted to range 0..100
// // // 2/ White with colortone - or CW (Cold / Warm)
// // //    ct is 153..500 temperature (153=cold, 500=warm)
// // //    briCT specifies the brightness for white channel
// // //
// // // Dimmer (0.100) is automatically derived from brightness
// // //
// // // INVARIANTS:
// // // 1.  RGB components are always stored at full brightness and modulated with briRGB
// // //     ((R == 255) || (G == 255) || (B == 255))
// // // 2.  RGB and Hue/Sat are always kept in sync whether you use setRGB() or setHS()
// // // 3.  Warm/Cold white channels are always stored at full brightness
// // //     ((WW == 255) || (WC == 255))
// // // 4.  WC/WW and CT are always kept in sync.
// // //     Note: if you use setCT() then WC+WW == 255 (both channels are linked)
// // //     but if you use setCW() both channels can be set independantly
// // // 5.  If RGB or CT channels are deactivated, then corresponding brightness is zero
// // //     if (colot_tone == LCM_RGB) then briCT = 0
// // //     if (color_tone == LCM_CT)  then briRGB = 0
// // //     if (colot_tone == LCM_BOTH) then briRGB and briCT can have any values
// // //
// // // Note:  If you want the actual RGB, you need to multiply with Bri, or use getActualRGBCW()
// // // Note: all values are stored as unsigned integer, no floats.
// // // Note: you can query vaules from this singleton. But to change values,
// // //   use the LightController - changing this object will have no effect on actual 
// // //

// // class LightStateClass {
// //   private:
   
// //   public:
// //     LightStateClass() {
// //       //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
// //     }

// //     void setSubType(uint8_t sub_type) {
// //       _subtype = sub_type;    // set sub_type at initialization, shoudln't be changed afterwards
// //     }

// //     // This function is a bit hairy, it will try to match the rerquired
// //     // colormode with the features of the device:
// //     //   LST_NONE:      LCM_RGB
// //     //   LST_SINGLE:    LCM_RGB
// //     //   LST_COLDWARM:  LCM_CT
// //     //   LST_RGB:       LCM_RGB
// //     //   LST_RGBW:      LCM_RGB, LCM_CT or LCM_BOTH
// //     //   LST_RGBCW:     LCM_RGB, LCM_CT or LCM_BOTH
// //     uint8_t setColorMode(uint8_t cm) {
// //       uint8_t prev_cm = _color_mode;
// //       if (cm < LCM_RGB) { cm = LCM_RGB; }
// //       if (cm > LCM_BOTH) { cm = LCM_BOTH; }
// //       uint8_t maxbri = (_briRGB >= _briCT) ? _briRGB : _briCT;

// //       switch (_subtype) {
// //         case LST_COLDWARM:
// //           _color_mode = LCM_CT;
// //           break;

// //         case LST_NONE:
// //         case LST_SINGLE:
// //         case LST_RGB:
// //         default:
// //           _color_mode = LCM_RGB;
// //           break;

// //         case LST_RGBW:
// //         case LST_RGBCW:
// //           _color_mode = cm;
// //           break;
// //       }
// //       if (LCM_RGB == _color_mode) {
// //         _briCT = 0;
// //         if (0 == _briRGB) { _briRGB = maxbri; }
// //       }
// //       if (LCM_CT == _color_mode) {
// //         _briRGB = 0;
// //         if (0 == _briCT) { _briCT = maxbri; }
// //       }
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setColorMode prev_cm (%d) req_cm (%d) new_cm (%d)", prev_cm, cm, _color_mode);
// // #endif
// //       return prev_cm;
// //     }

// //     inline 
// uint8_t mPWMLight::getColorMode() {
//   return _color_mode;
// }

// //     void addRGBMode() {
// //       setColorMode(_color_mode | LCM_RGB);
// //     }
// //     void addCTMode() {
// //       setColorMode(_color_mode | LCM_CT);
// //     }

// //     // Get RGB color, always at full brightness (ie. one of the components is 255)
// //     void getRGB(uint8_t *r, uint8_t *g, uint8_t *b) {
// //       if (r) { *r = _r; }
// //       if (g) { *g = _g; }
// //       if (b) { *b = _b; }
// //     }

// //     // get full brightness values for warm and cold channels.
// //     // either w=c=0 (off) or w+c >= 255
// //     void getCW(uint8_t *rc, uint8_t *rw) {
// //       if (rc) { *rc = _wc; }
// //       if (rw) { *rw = _ww; }
// //     }

// // Get the actual values for each channel, ie multiply with brightness
// void mPWMLight::getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w) {

//   bool rgb_channels_on = _color_mode & LCM_RGB;
//   bool ct_channels_on = _color_mode & LCM_CT;

//   if (r) { *r = rgb_channels_on ? mSupport::changeUIntScale(_r, 0, 255, 0, _briRGB) : 0; }
//   if (g) { *g = rgb_channels_on ? mSupport::changeUIntScale(_g, 0, 255, 0, _briRGB) : 0; }
//   if (b) { *b = rgb_channels_on ? mSupport::changeUIntScale(_b, 0, 255, 0, _briRGB) : 0; }

//   if (c) { *c = ct_channels_on  ? mSupport::changeUIntScale(_wc, 0, 255, 0, _briCT) : 0; }
//   if (w) { *w = ct_channels_on  ? mSupport::changeUIntScale(_ww, 0, 255, 0, _briCT) : 0; }

// }

// uint8_t mPWMLight::getChannels(uint8_t *channels) {
//   getActualRGBCW(&channels[0], &channels[1], &channels[2], &channels[3], &channels[4]);
// }

// void mPWMLight::getChannelsRaw(uint8_t *channels) {
//     channels[0] = _r;
//     channels[1] = _g;
//     channels[2] = _b;
//     channels[3] = _wc;
//     channels[4] = _ww;
// }

// //     void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
// //       if (hue) { *hue = _hue; }
// //       if (sat) { *sat = _sat; }
// //       if (bri) { *bri = _briRGB; }
// //     }

// //     // getBri() is guaranteed to give the same result as setBri() - no rounding errors.
// //     uint8_t getBri(void) {
// //       // return the max of _briCT and _briRGB
// //       return (_briRGB >= _briCT) ? _briRGB : _briCT;
// //     }

// //     // get the white Brightness
// //     inline uint8_t getBriCT() {
// //       return _briCT;
// //     }

// // static inline 
// uint8_t mPWMLight::DimmerToBri(uint8_t dimmer) {
//   return mSupport::changeUIntScale(dimmer, 0, 100, 0, 255);  // 0..255
// }
// //static 
// uint8_t mPWMLight::BriToDimmer(uint8_t bri) {
//   uint8_t dimmer = mSupport::changeUIntScale(bri, 0, 255, 0, 100);
//   // if brightness is non zero, force dimmer to be non-zero too
//   if ((dimmer == 0) && (bri > 0)) { dimmer = 1; }
//   return dimmer;
// }

// //     uint8_t getDimmer(uint32_t mode = 0) {
// //       uint8_t bri;
// //       switch (mode) {
// //         case 1:
// //           bri = getBriRGB();
// //           break;
// //         case 2:
// //           bri = getBriCT();
// //           break;
// //         default:
// //           bri = getBri();
// //           break;
// //       }
// //       return BriToDimmer(bri);
// //     }

// //     inline 
// uint16_t mPWMLight::getCT(){//} const {
//   return _ct; // 153..500, or CT_MIN..CT_MAX
// }

// //     // get the CT value within the range into a 10 bits 0..1023 value
// //     uint16_t getCT10bits() const {
// //       return mSupport::changeUIntScale(_ct, _ct_min_range, _ct_max_range, 0, 1023);
// //     }

// //     inline void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) {
// //       _ct_min_range = ct_min_range;
// //       _ct_max_range = ct_max_range;
// //     }

// //     inline void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) const {
// //       if (ct_min_range) { *ct_min_range = _ct_min_range; }
// //       if (ct_max_range) { *ct_max_range = _ct_max_range; }
// //     }

// //     // get current color in XY format
// //     void getXY(float *x, float *y) {
// //       RgbToXy(_r, _g, _b, x, y);
// //     }

// //     // setters -- do not use directly, use the light_controller instead
// //     // sets both master Bri and whiteBri
// //     void setBri(uint8_t bri) {
// //       setBriRGB(_color_mode & LCM_RGB ? bri : 0);
// //       setBriCT(_color_mode & LCM_CT ? bri : 0);
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
// // #endif
// // #ifdef USE_PWM_DIMMER
// //   if (PWM_DIMMER == my_module_type) PWMDimmerSetBrightnessLeds(0);
// // #endif  // USE_PWM_DIMMER
// //     }

// //     // changes the RGB brightness alone
// //     uint8_t setBriRGB(uint8_t bri_rgb) {
// //       uint8_t prev_bri = _briRGB;
// //       _briRGB = bri_rgb;
// //       if (bri_rgb > 0) { addRGBMode(); }
// //       return prev_bri;
// //     }

// //     // changes the white brightness alone
// //     uint8_t setBriCT(uint8_t bri_ct) {
// //       uint8_t prev_bri = _briCT;
// //       _briCT = bri_ct;
// //       if (bri_ct > 0) { addCTMode(); }
// //       return prev_bri;
// //     }

// //inline 
// uint8_t mPWMLight::getBriRGB() {
//   return _briRGB;
// }

// void mPWMLight::setDimmer(uint8_t dimmer) {
//   setBri(DimmerToBri(dimmer));
// }

// void mPWMLight::setCT(uint16_t ct) {
// //       if (0 == ct) {
// //         // disable ct mode
// //         setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
// //       } else {
// //         ct = (ct < CT_MIN ? CT_MIN : (ct > CT_MAX ? CT_MAX : ct));
// //         _ww = mSupport::changeUIntScale(ct, _ct_min_range, _ct_max_range, 0, 255);
// //         _wc = 255 - _ww;
// //         _ct = ct;
// //         addCTMode();
// //       }
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)", _r, _g, _b, _hue, _sat, _briRGB, _briCT, _ct);
// // #endif
// }

// //     // Manually set Cold/Warm channels.
// //     // There are two modes:
// //     // 1. (free_range == false, default)
// //     //    In this mode there is only one virtual white channel with color temperature
// //     //    As a side effect, WC+WW = 255. It means also that the sum of light power
// //     //    from white LEDs is always equal to briCT. It is not possible here
// //     //    to set both white LEDs at full power, hence protecting power supplies
// //     //    from overlaoding.
// //     // 2. (free_range == true)
// //     //    In this mode, values of WC and WW are free -- both channels can be set
// //     //    at full power.
// //     //    In this mode, we always scale both channels so that one at least is 255.
// //     //
// //     // We automatically adjust briCT to have the right values of channels
// //     void setCW(uint8_t c, uint8_t w, bool free_range = false) {
// //       uint16_t max = (w > c) ? w : c;   // 0..255
// //       uint16_t sum = c + w;
// //       if (sum <= 257) { free_range = false; }    // if we don't allow free range or if sum is below 255 (with tolerance of 2)

// //       if (0 == max) {
// //         _briCT = 0;       // brightness set to null
// //         setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
// //       } else {
// //         if (!free_range) {
// //           // we need to normalize to sum = 255
// //           _ww = mSupport::changeUIntScale(w, 0, sum, 0, 255);
// //           _wc = 255 - _ww;
// //         } else {  // we normalize to max = 255
// //           _ww = mSupport::changeUIntScale(w, 0, max, 0, 255);
// //           _wc = mSupport::changeUIntScale(c, 0, max, 0, 255);
// //         }
// //         _ct = mSupport::changeUIntScale(w, 0, sum, _ct_min_range, _ct_max_range);
// //         addCTMode();   // activate CT mode if needed
// //         if (_color_mode & LCM_CT) { _briCT = free_range ? max : (sum > 255 ? 255 : sum); }
// //       }
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCW CW (%d %d) CT (%d) briCT (%d)", c, w, _ct, _briCT);
// // #endif
// //     }

// //     // sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
// //     uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
// //       uint16_t hue;
// //       uint8_t  sat;
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB input (%d %d %d)", r, g, b);
// // #endif

// //       uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

// //       if (0 == max) {
// //         r = g = b = 255;
// //         setColorMode(LCM_CT);   // try deactivating RGB, setColorMode() will check if this is legal
// //       } else {
// //         if (255 > max) {
// //           // we need to normalize rgb
// //           r = mSupport::changeUIntScale(r, 0, max, 0, 255);
// //           g = mSupport::changeUIntScale(g, 0, max, 0, 255);
// //           b = mSupport::changeUIntScale(b, 0, max, 0, 255);
// //         }
// //         addRGBMode();
// //       }
// //       if (!keep_bri) {
// //         _briRGB = (_color_mode & LCM_RGB) ? max : 0;
// //       }

// //       RgbToHsb(r, g, b, &hue, &sat, nullptr);
// //       _r = r;
// //       _g = g;
// //       _b = b;
// //       _hue = hue;
// //       _sat = sat;
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
// // #endif
// //       return max;
// //     }

// //     void setHS(uint16_t hue, uint8_t sat) {
// //       uint8_t r, g, b;
// //       HsToRgb(hue, sat, &r, &g, &b);
// //       _r = r;
// //       _g = g;
// //       _b = b;
// //       _hue = hue;
// //       _sat = sat;
// //       addRGBMode();
// // #ifdef DEBUG_LIGHT
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS HS (%d %d) rgb (%d %d %d)", hue, sat, r, g, b);
// //       AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
// // #endif
// //   }

// //   // set all 5 channels at once, don't modify the values in ANY way
// //   // Channels are: R G B CW WW
// //   void setChannelsRaw(uint8_t *channels) {
// //     _r = channels[0];
// //     _g = channels[1];
// //     _b = channels[2];
// //     _wc = channels[3];
// //     _ww = channels[4];
// // }

// //   // set all 5 channels at once.
// //   // Channels are: R G B CW WW
// //   // Brightness is automatically recalculated to adjust channels to the desired values
// //   void setChannels(uint8_t *channels) {
// //     setRGB(channels[0], channels[1], channels[2]);
// //     setCW(channels[3], channels[4], true);  // free range for WC and WW
// // #ifdef DEBUG_LIGHT
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels (%d %d %d %d %d)",
// //       channels[0], channels[1], channels[2], channels[3], channels[4]);
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels CT (%d) briRGB (%d) briCT (%d)", _ct, _briRGB, _briCT);
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels Actuals (%d %d %d %d %d)",
// //       _r, _g, _b, _wc, _ww);
// // #endif
// //   }

// //     // new version of RGB to HSB with only integer calculation
// //     static void RgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri);
// //     static void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
// //     static void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
// //     static void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);

// // };


// // /*********************************************************************************************\
// //  * LightStateClass implementation
// // \*********************************************************************************************/

// // // new version with only integer computing
// // // brightness is not needed, it is controlled via Dimmer
// // void LightStateClass::RgbToHsb(uint8_t ir, uint8_t ig, uint8_t ib, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri) {
// //   uint32_t r = ir;
// //   uint32_t g = ig;
// //   uint32_t b = ib;
// //   uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255
// //   uint32_t min = (r < g && r < b) ? r : (g < b) ? g : b;   // 0..255
// //   uint32_t d = max - min;   // 0..255

// //   uint16_t hue = 0;   // hue value in degrees ranges from 0 to 359
// //   uint8_t sat = 0;    // 0..255
// //   uint8_t bri = max;  // 0..255

// //   if (d != 0) {
// //     sat = mSupport::changeUIntScale(d, 0, max, 0, 255);
// //     if (r == max) {
// //       hue = (g > b) ?       mSupport::changeUIntScale(g-b,0,d,0,60) : 360 - mSupport::changeUIntScale(b-g,0,d,0,60);
// //     } else if (g == max) {
// //       hue = (b > r) ? 120 + mSupport::changeUIntScale(b-r,0,d,0,60) : 120 - mSupport::changeUIntScale(r-b,0,d,0,60);
// //     } else {
// //       hue = (r > g) ? 240 + mSupport::changeUIntScale(r-g,0,d,0,60) : 240 - mSupport::changeUIntScale(g-r,0,d,0,60);
// //     }
// //     hue = hue % 360;    // 0..359
// //   }

// //   if (r_hue) *r_hue = hue;
// //   if (r_sat) *r_sat = sat;
// //   if (r_bri) *r_bri = bri;
// //   //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "RgbToHsb rgb (%d %d %d) hsb (%d %d %d)", r, g, b, hue, sat, bri);
// // }

// // void LightStateClass::HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
// //   uint32_t r = 255;  // default to white
// //   uint32_t g = 255;
// //   uint32_t b = 255;
// //   // we take brightness at 100%, brightness should be set separately
// //   hue = hue % 360;  // normalize to 0..359

// //   if (sat > 0) {
// //     uint32_t i = hue / 60;   // quadrant 0..5
// //     uint32_t f = hue % 60;   // 0..59
// //     uint32_t q = 255 - mSupport::changeUIntScale(f, 0, 60, 0, sat);  // 0..59
// //     uint32_t p = 255 - sat;
// //     uint32_t t = 255 - mSupport::changeUIntScale(60 - f, 0, 60, 0, sat);

// //     switch (i) {
// //       case 0:
// //         //r = 255;
// //         g = t;
// //         b = p;
// //         break;
// //       case 1:
// //         r = q;
// //         //g = 255;
// //         b = p;
// //         break;
// //       case 2:
// //         r = p;
// //         //g = 255;
// //         b = t;
// //         break;
// //       case 3:
// //         r = p;
// //         g = q;
// //         //b = 255;
// //         break;
// //       case 4:
// //         r = t;
// //         g = p;
// //         //b = 255;
// //         break;
// //       default:
// //         //r = 255;
// //         g = p;
// //         b = q;
// //         break;
// //       }
// //     }
// //   if (r_r)  *r_r = r;
// //   if (r_g)  *r_g = g;
// //   if (r_b)  *r_b = b;
// // }

// // #define POW FastPrecisePowf

// // //
// // // Matrix 3x3 multiplied to a 3 vector, result in a 3 vector
// // //
// // void mat3x3(const float *mat33, const float *vec3, float *res3) {
// //   for (uint32_t i = 0; i < 3; i++) {
// //     const float * v = vec3;
// //     *res3 = 0.0f;
// //     for (uint32_t j = 0; j < 3; j++) {
// //       *res3 += *mat33++ * *v++;
// //     }
// //     res3++;
// //   }
// // }

// // void LightStateClass::RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y) {
// //   float x = 0.31271f;   // default medium white
// //   float y = 0.32902f;

// //   if (i_r + i_b + i_g > 0) {
// //     float rgb[3] = { (float)i_r, (float)i_g, (float)i_b };
// //     // https://gist.github.com/popcorn245/30afa0f98eea1c2fd34d
// //     // Gamma correction
// //     for (uint32_t i = 0; i < 3; i++) {
// //       rgb[i] = rgb[i] / 255.0f;
// //       rgb[i] = (rgb[i] > 0.04045f) ? POW((rgb[i] + 0.055f) / (1.0f + 0.055f), 2.4f) : (rgb[i] / 12.92f);
// //     }

// //     // conversion to X, Y, Z
// //     // Y is also the Luminance
// //     float XYZ[3];
// //     static const float XYZ_factors[] = {  0.649926f, 0.103455f, 0.197109f,
// //                                           0.234327f, 0.743075f, 0.022598f,
// //                                           0.000000f, 0.053077f, 1.035763f };
// //     mat3x3(XYZ_factors, rgb, XYZ);

// //     float XYZ_sum = XYZ[0] + XYZ[1] + XYZ[2];
// //     x = XYZ[0] / XYZ_sum;
// //     y = XYZ[1] / XYZ_sum;
// //     // we keep the raw gamut, one nice thing could be to convert to a narrower gamut
// //   }
// //   if (r_x)  *r_x = x;
// //   if (r_y)  *r_y = y;
// // }

// // void LightStateClass::XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb)
// // {
// //   float XYZ[3], rgb[3];
// //   x = (x > 0.99f ? 0.99f : (x < 0.01f ? 0.01f : x));
// //   y = (y > 0.99f ? 0.99f : (y < 0.01f ? 0.01f : y));
// //   float z = 1.0f - x - y;
// //   XYZ[0] = x / y;
// //   XYZ[1] = 1.0f;
// //   XYZ[2] = z / y;

// //   static const float rgb_factors[] = {  3.2406f, -1.5372f, -0.4986f,
// //                                        -0.9689f,  1.8758f,  0.0415f,
// //                                         0.0557f, -0.2040f,  1.0570f };
// //   mat3x3(rgb_factors, XYZ, rgb);
// //   float max = (rgb[0] > rgb[1] && rgb[0] > rgb[2]) ? rgb[0] : (rgb[1] > rgb[2]) ? rgb[1] : rgb[2];
 
// //   for (uint32_t i = 0; i < 3; i++) {
// //     rgb[i] = rgb[i] / max; // normalize to max == 1.0
// //     rgb[i] = (rgb[i] <= 0.0031308f) ? 12.92f * rgb[i] : 1.055f * POW(rgb[i], (1.0f / 2.4f)) - 0.055f; // gamma
// //   }

// //   int32_t irgb[3];
// //   for (uint32_t i = 0; i < 3; i++) {
// //     irgb[i] = rgb[i] * 255.0f + 0.5f;
// //   }

// //   if (rr) { *rr = (irgb[0] > 255 ? 255: (irgb[0] < 0 ? 0 : irgb[0])); }
// //   if (rg) { *rg = (irgb[1] > 255 ? 255: (irgb[1] < 0 ? 0 : irgb[1])); }
// //   if (rb) { *rb = (irgb[2] > 255 ? 255: (irgb[2] < 0 ? 0 : irgb[2])); }
// // }

// //   void setSubType(uint8_t sub_type) {
// //     _state->setSubType(sub_type);
// //   }

// //   inline bool setCTRGBLinked(bool ct_rgb_linked) {
// //     bool prev = _ct_rgb_linked;
// //     if (_pwm_multi_channels) {
// //       _ct_rgb_linked = false;   // force to false if _pwm_multi_channels is set
// //     } else {
// //       _ct_rgb_linked = ct_rgb_linked;
// //     }
// //     return prev;
// //   }

// //   void setAlexaCTRange(bool alexa_ct_range) {
// //     // depending on SetOption82, full or limited CT range
// //     if (alexa_ct_range) {
// //       _state->setCTRange(CT_MIN_ALEXA, CT_MAX_ALEXA);   // 200..380
// //     } else {
// //       _state->setCTRange(CT_MIN, CT_MAX);               // 153..500
// //     }
// //   }

// //   inline bool isCTRGBLinked() {
// //     return _ct_rgb_linked;
// //   }

// //   inline bool setPWMMultiChannel(bool pwm_multi_channels) {
// //     bool prev = _pwm_multi_channels;
// //     _pwm_multi_channels = pwm_multi_channels;
// //     if (pwm_multi_channels)  setCTRGBLinked(false);    // if pwm multi channel, then unlink RGB and CT
// //     return prev;
// //   }

// //   inline bool isPWMMultiChannel(void) {
// //     return _pwm_multi_channels;
// //   }

// // #ifdef DEBUG_LIGHT
// //   void debugLogs() {
// //     uint8_t r,g,b,c,w;
// //     _state->getActualRGBCW(&r,&g,&b,&c,&w);
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs rgb (%d %d %d) cw (%d %d)",
// //       r, g, b, c, w);
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs lightCurrent (%d %d %d %d %d)",
// //       current_color[0], current_color[1], current_color[2],
// //       current_color[3], current_color[4]);
// //   }
// // #endif

// //   void loadSettings() {
// // #ifdef DEBUG_LIGHT
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings Settings.light_color (%d %d %d %d %d - %d)",
// //       Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
// //       Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
// //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings light_type/sub (%d %d)",
// //       light_type, subtype);
// // #endif
// //     if (_pwm_multi_channels) {
// //     //   _state->setChannelsRaw(Settings.LightSettings.light_color);
// //     } else {
// //       // first try setting CW, if zero, it select RGB mode
// //     //   _state->setCW(Settings.light_color[3], Settings.light_color[4], true);
// //     //   _state->setRGB(Settings.light_color[0], Settings.light_color[1], Settings.light_color[2]);

// //       // only if non-multi channel
// //       // We apply dimmer in priority to RGB
// //     //   uint8_t bri = _state->DimmerToBri(Settings.light_dimmer);

// //     //   // The default values are #FFFFFFFFFF, in this case we avoid setting all channels
// //     //   // at the same time, see #6534 and #8120
// //     //   if ((DEFAULT_LIGHT_COMPONENT == Settings.light_color[0]) &&
// //     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[1]) &&
// //     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[2]) &&
// //     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[3]) &&
// //     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[4]) &&
// //     //       (DEFAULT_LIGHT_DIMMER    == Settings.light_dimmer) ) {
// //     //     if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
// //     //       _state->setCW(255, 0);       // avoid having both white channels at 100%, zero second channel (#see 8120)
// //     //     }
// //     //     _state->setBriCT(bri);
// //     //     _state->setBriRGB(bri);
// //     //     _state->setColorMode(LCM_RGB);
// //       }

// //     //   if (Settings.light_color[0] + Settings.light_color[1] + Settings.light_color[2] > 0) {
// //     //     _state->setBriRGB(bri);
// //     //   } else {
// //     //     _state->setBriCT(bri);
// //     //   }
// //     //}
// //   }

// //   void changeCTB(uint16_t new_ct, uint8_t briCT) {
// //     /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
// //      *
// //      * ct = 153 = 6500K = Cold = CCWW = FF00
// //      * ct = 500 = 2000K = Warm = CCWW = 00FF
// //      */
// //     // don't set CT if not supported
// //     if ((LST_COLDWARM != subtype) && (LST_RGBW > subtype)) {
// //       return;
// //     }
// //     // _state->setCT(new_ct);
// //     // _state->setBriCT(briCT);
// //     // if (_ct_rgb_linked) { _state->setColorMode(LCM_CT); }   // try to force CT
// //     // saveSettings();
// //     // calcLevels();
// //     //debugLogs();
// //   }

// //   void changeDimmer(uint8_t dimmer, uint32_t mode = 0) {
// //     // uint8_t bri = mSupport::changeUIntScale(dimmer, 0, 100, 0, 255);
// //     // switch (mode) {
// //     //   case 1:
// //     //     changeBriRGB(bri);
// //     //     if (_ct_rgb_linked) { _state->setColorMode(LCM_RGB); }   // try to force CT
// //     //     break;
// //     //   case 2:
// //     //     changeBriCT(bri);
// //     //     if (_ct_rgb_linked) { _state->setColorMode(LCM_CT); }   // try to force CT
// //     //     break;
// //     //   default:
// //     //     changeBri(bri);
// //     //     break;
// //     // }
// //   }

// //   void changeBri(uint8_t bri) {
// //     // _state->setBri(bri);
// //     // saveSettings();
// //     // calcLevels();
// //   }

// //   void changeBriRGB(uint8_t bri) {
// //     // _state->setBriRGB(bri);
// //     // saveSettings();
// //     // calcLevels();
// //   }

// //   void changeBriCT(uint8_t bri) {
// //     // _state->setBriCT(bri);
// //     // saveSettings();
// //     // calcLevels();
// //   }

// //   void changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
// //     // _state->setRGB(r, g, b, keep_bri);
// //     // if (_ct_rgb_linked) { _state->setColorMode(LCM_RGB); }   // try to force RGB
// //     // saveSettings();
// //     // calcLevels();
// //   }

// //   calculate the levels for each channel
// //   if no parameter, results are stored in current_color
// void mPWMLight::calcLevels(uint8_t *current_color) {

//   uint8_t r,g,b,c,w,briRGB,briCT;
//   if (current_color == nullptr) { current_color = current_color; }

//   // if (_pwm_multi_channels) { // if PWM multi channel, no more transformation required
//   //   _state->getChannelsRaw(current_color);
//   //   return;
//   // }

//   getActualRGBCW(&r,&g,&b,&c,&w);
//   briRGB = getBriRGB();
//   briCT  = getBriCT();

//   current_color[0] = current_color[1] = current_color[2] = 0;
//   current_color[3] = current_color[4] = 0;
//   switch (subtype) {
//     case LST_NONE:
//       current_color[0] = 255;
//       break;
//     case LST_SINGLE:
//       current_color[0] = briRGB;
//       break;
//     case LST_COLDWARM:
//       current_color[0] = c;
//       current_color[1] = w;
//       break;
//     case LST_RGBW:
//     case LST_RGBCW:
//       if (LST_RGBCW == subtype) {
//         current_color[3] = c;
//         current_color[4] = w;
//       } else {
//         current_color[3] = briCT;
//       }
//       // continue
//     case LST_RGB:
//       current_color[0] = r;
//       current_color[1] = g;
//       current_color[2] = b;
//       break;
//   }
// }

// void mPWMLight::changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB) {
//   setHS(hue, sat);
//   setBriRGB(briRGB);
//   if (_ct_rgb_linked) { setColorMode(LCM_RGB); }   // try to force RGB
//   saveSettings();
//   calcLevels();
// }

// // save the current light state to Settings.
// void mPWMLight::saveSettings() {
// // //     if (pwm_multi_channels) {
// // //       // simply save each channel
// // //       _state->getChannelsRaw(Settings.light_color);
// // //       Settings.light_dimmer = 100;    // arbitrary value, unused in this mode
// // //     } else {
// // //       uint8_t cm = _state->getColorMode();

// // //       memset(&Settings.light_color[0], 0, sizeof(Settings.light_color));    // clear all channels
// // //       if (LCM_RGB & cm) {   // can be either LCM_RGB or LCM_BOTH
// // //         _state->getRGB(&Settings.light_color[0], &Settings.light_color[1], &Settings.light_color[2]);
// // //         Settings.light_dimmer = _state->BriToDimmer(_state->getBriRGB());
// // //         // anyways we always store RGB with BrightnessRGB
// // //         if (LCM_BOTH == cm) {
// // //           // then store at actual brightness CW/WW if dual mode
// // //           _state->getActualRGBCW(nullptr, nullptr, nullptr, &Settings.light_color[3], &Settings.light_color[4]);
// // //         }
// // //       } else if (LCM_CT == cm) {    // cm can only be LCM_CT
// // //         _state->getCW(&Settings.light_color[3], &Settings.light_color[4]);
// // //         Settings.light_dimmer = _state->BriToDimmer(_state->getBriCT());
// // //       }
// // //     }
// // // #ifdef DEBUG_LIGHT
// // //     AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::saveSettings Settings.light_color (%d %d %d %d %d - %d)",
// // //       Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
// // //       Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
// // // #endif
// }

// //   // set all 5 channels at once.
// //   // Channels are: R G B CW WW
// //   // Brightness is automatically recalculated to adjust channels to the desired values
// //   void changeChannels(uint8_t *channels) {
// //     // if (pwm_multi_channels) {
// //     //   _state->setChannelsRaw(channels);
// //     // } else if (LST_COLDWARM == subtype) {
// //     //   // remap channels 0-1 to 3-4 if cold/warm
// //     //   uint8_t remapped_channels[5] = {0,0,0,channels[0],channels[1]};
// //     //   _state->setChannels(remapped_channels);
// //     // } else {
// //     //   _state->setChannels(channels);
// //     // }

// //     // saveSettings();
// //     // calcLevels();
// //   }

// // }; // END CLASS


// // // the singletons for light state and Light Controller
// // LightStateClass light_state = LightStateClass();
// // LightControllerClass light_controller = LightControllerClass(light_state);

// /*********************************************************************************************\
//  * Change scales from 8 bits to 10 bits and vice versa
// \*********************************************************************************************/
// // 8 to 10 to 8 is garanteed to give the same result
// uint16_t mPWMLight::change8to10(uint8_t v) {
//   return mSupport::changeUIntScale(v, 0, 255, 0, 1023);
// }
// // change from 10 bits to 8 bits, but any non-zero input will be non-zero
// uint8_t mPWMLight::change10to8(uint16_t v) {
//   return (0 == v) ? 0 : mSupport::changeUIntScale(v, 4, 1023, 1, 255);
// }

// // /*********************************************************************************************\
// //  * Gamma correction
// // \*********************************************************************************************/
// // // Calculate the gamma corrected value for LEDS
// // uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
// //   uint16_t from_src = 0;
// //   uint16_t from_gamma = 0;

// //   for (const gamma_table_t *gt = gt_ptr; ; gt++) {
// //     uint16_t to_src = gt->to_src;
// //     uint16_t to_gamma = gt->to_gamma;
// //     if (v <= to_src) {
// //       return mSupport::changeUIntScale(v, from_src, to_src, from_gamma, to_gamma);
// //     }
// //     from_src = to_src;
// //     from_gamma = to_gamma;
// //   }
// // }
// // // Calculate the reverse gamma value for LEDS
// // uint16_t ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr) {
// //   uint16_t from_src = 0;
// //   uint16_t from_gamma = 0;

// //   for (const gamma_table_t *gt = gt_ptr; ; gt++) {
// //     uint16_t to_src = gt->to_src;
// //     uint16_t to_gamma = gt->to_gamma;
// //     if (vg <= to_gamma) {
// //       return mSupport::changeUIntScale(vg, from_gamma, to_gamma, from_src, to_src);
// //     }
// //     from_src = to_src;
// //     from_gamma = to_gamma;
// //   }
// // }

// // // 10 bits in, 10 bits out
// // uint16_t ledGamma10_10(uint16_t v) {
// //   return ledGamma_internal(v, gamma_table);
// // }
// // // 10 bits resolution, 8 bits in
// // uint16_t ledGamma10(uint8_t v) {
// //   return ledGamma10_10(change8to10(v));
// // }

// // // Legacy function
// // uint8_t ledGamma(uint8_t v) {
// //   return change10to8(ledGamma10(v));
// // }

// // /********************************************************************************************/

// // void LightPwmOffset(uint32_t offset)
// // {
// //   pwm_offset = offset;
// // }


// // // compute actual PWM min/max values from DimmerRange
// // // must be called when DimmerRange is changed or LedTable
// // void LightCalcPWMRange(void) {
// //   uint16_t pwm_min, pwm_max;

// //   pwm_min = change8to10(LightStateClass::DimmerToBri(Settings.dimmer_hw_min));   // default 0
// //   pwm_max = change8to10(LightStateClass::DimmerToBri(Settings.dimmer_hw_max));   // default 100
// //   if (Settings.light_correction) {
// //     pwm_min = ledGamma10_10(pwm_min);       // apply gamma correction
// //     pwm_max = ledGamma10_10(pwm_max);       // 0..1023
// //   }
// //   pwm_min = pwm_min > 0 ? mSupport::changeUIntScale(pwm_min, 1, 1023, 1, Settings.pwm_range) : 0;  // adapt range but keep zero and non-zero values
// //   pwm_max = mSupport::changeUIntScale(pwm_max, 1, 1023, 1, Settings.pwm_range);  // pwm_max cannot be zero

// //   pwm_min = pwm_min;
// //   pwm_max = pwm_max;
// //   //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("LightCalcPWMRange %d %d - %d %d"), Settings.dimmer_hw_min, Settings.dimmer_hw_max, pwm_min, pwm_max);
// // }

// void mPWMLight::Init(void) //LightInit(void)
// {

//   memset(&pwm_channel_test,0,sizeof(pwm_channel_test));

//   device = pCONT_set->devices_present;
//   subtype = (pCONT_set->light_type & 7) > LST_MAX ? LST_MAX : (pCONT_set->light_type & 7); // Always 0 - LST_MAX (5)

//   subtype = LST_RGBCW;
  
//   pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

//   pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

// //   if (LST_RGBW <= subtype) {
// //     // only change if RGBW or RGBCW
// //     // do not allow independant RGB and WC colors
// //     bool ct_rgb_linked = !(Settings.param[P_RGB_REMAP] & 128);
// //     light_controller.setCTRGBLinked(ct_rgb_linked);
// //   }

// //   if ((LST_SINGLE <= subtype) && pwm_multi_channels) {
// //     // we treat each PWM channel as an independant one, hence we switch to
// //     light_controller.setPWMMultiChannel(true);
// //     device = devices_present - subtype + 1; // adjust if we also have relays
// //   } else if (!light_controller.isCTRGBLinked()) {
// //     // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately
// //     device--;   // we take the last two devices as lights
// //   }
// //   LightCalcPWMRange();
// // #ifdef DEBUG_LIGHT
// //   AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightInit pwm_multi_channels=%d subtype=%d device=%d devices_present=%d",
// //     pwm_multi_channels, subtype, device, devices_present);
// // #endif

// //   light_controller.setSubType(subtype);
// //   light_controller.loadSettings();
// //   light_controller.setAlexaCTRange(Settings.flag4.alexa_ct_range);
// //   light_controller.calcLevels();    // calculate the initial values (#8058)

// //   if (LST_SINGLE == subtype) {
// //     Settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
// //   }
// //   if (light_type < LT_PWM6) {           // PWM
// //     for (uint32_t i = 0; i < light_type; i++) {
// //       Settings.pwm_value[i] = 0;        // Disable direct PWM control
// //       if (PinUsed(GPIO_PWM1, i)) {
// //         pinMode(Pin(GPIO_PWM1, i), OUTPUT);
// //       }
// //     }
// //     if (PinUsed(GPIO_ARIRFRCV)) {
// //       if (PinUsed(GPIO_ARIRFSEL)) {
// //         pinMode(Pin(GPIO_ARIRFSEL), OUTPUT);
// //         digitalWrite(Pin(GPIO_ARIRFSEL), 1);  // Turn off RF
// //       }
// //     }
// //   }

// //   uint32_t max_scheme = max_scheme;
// //   if (subtype < LST_RGB) {
// //     max_scheme = LS_POWER;
// //   }
// //   if ((LS_WAKEUP == Settings.light_scheme) || (Settings.light_scheme > max_scheme)) {
// //     Settings.light_scheme = LS_POWER;
// //   }
// //   power = 0;
// //   update = true;
// //   wakeup_active = 0;
// //   if (Settings.flag4.fade_at_startup) {
// //     fade_initialized = true;      // consider fade intialized starting from black
// //   }

// //   LightUpdateColorMapping();


// } //light_init

// // void LightUpdateColorMapping(void)
// // {
// //   uint8_t param = Settings.param[P_RGB_REMAP] & 127;
// //   if (param > 119){ param = 0; }

// //   uint8_t tmp[] = {0,1,2,3,4};
// //   color_remap[0] = tmp[param / 24];
// //   for (uint32_t i = param / 24; i<4; ++i){
// //     tmp[i] = tmp[i+1];
// //   }
// //   param = param % 24;
// //   color_remap[1] = tmp[(param / 6)];
// //   for (uint32_t i = param / 6; i<3; ++i){
// //     tmp[i] = tmp[i+1];
// //   }
// //   param = param % 6;
// //   color_remap[2] = tmp[(param / 2)];
// //   for (uint32_t i = param / 2; i<2; ++i){
// //     tmp[i] = tmp[i+1];
// //   }
// //   param = param % 2;
// //   color_remap[3] = tmp[param];
// //   color_remap[4] = tmp[1-param];

// //   update = true;
// //   //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%d colors: %d %d %d %d %d") ,Settings.param[P_RGB_REMAP], color_remap[0],color_remap[1],color_remap[2],color_remap[3],color_remap[4]);
// // }

// // uint8_t LightGetDimmer(uint8_t dimmer) {
// //   return light_state.getDimmer(dimmer);
// // }

// // void LightSetDimmer(uint8_t dimmer) {
// //   light_controller.changeDimmer(dimmer);
// // }

// // void LightGetHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
// //   light_state.getHSB(hue, sat, bri);
// // }

// // void LightGetXY(float *X, float *Y) {
// //   light_state.getXY(X, Y);
// // }

// // void LightHsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
// //   light_state.HsToRgb(hue, sat, r_r, r_g, r_b);
// // }

// // // If SetOption68 is set, get the brightness for a specific device
// // uint8_t LightGetBri(uint8_t device) {
// //   uint8_t bri = 254;   // default value if relay
// //   if (pwm_multi_channels) {
// //     if ((device >= device) && (device < device + LST_MAX) && (device <= devices_present)) {
// //       bri = current_color[device - device];
// //     }
// //   } else if (light_controller.isCTRGBLinked()) {   // standard behavior
// //     if (device == device) {
// //       bri = light_state.getBri();
// //     }
// //   } else {    // unlinked
// //     if (device == device) {
// //       bri = light_state.getBriRGB();
// //     } else if (device == device + 1) {
// //       bri = light_state.getBriCT();
// //     }
// //   }
// //   return bri;
// // }

// // // If SetOption68 is set, set the brightness for a specific device
// // void LightSetBri(uint8_t device, uint8_t bri) {
// //   if (pwm_multi_channels) {
// //     if ((device >= device) && (device < device + LST_MAX) && (device <= devices_present)) {
// //       current_color[device - device] = bri;
// //       light_controller.changeChannels(current_color);
// //     }
// //   } else if (light_controller.isCTRGBLinked()) {  // standard
// //     if (device == device) {
// //       light_controller.changeBri(bri);
// //     }
// //   } else {  // unlinked
// //     if (device == device) {
// //       light_controller.changeBriRGB(bri);
// //     } else if (device == device + 1) {
// //       light_controller.changeBriCT(bri);
// //     }
// //   }
// // }

// void mPWMLight::LightSetColorTemp(uint16_t ct)
// {
// /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
//  *
//  * ct = 153 = 6500K = Cold = CCWW = FF00
//  * ct = 600 = 2000K = Warm = CCWW = 00FF
//  */
//   // don't set CT if not supported
//   if ((LST_COLDWARM != subtype) && (LST_RGBCW != subtype)) {
//     return;
//   }
//   changeCTB(ct, getBriCT());
// }

// uint16_t mPWMLight::LightGetColorTemp(void)
// {
//   // don't calculate CT for unsupported devices
//   if ((LST_COLDWARM != subtype) && (LST_RGBCW != subtype)) {
//     return 0;
//   }
//   return (getColorMode() & LCM_CT) ? getCT() : 0;
// }

// // void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value)
// // {
// // /* lo - below lo is green
// //    hi - above hi is red
// // */
// //   if (Settings.flag.light_signal) {  // SetOption18 - Pair light signal with CO2 sensor
// //     uint16_t signal = mSupport::changeUIntScale(value, lo, hi, 0, 255);  // 0..255
// // //    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Light signal %d"), signal);
// //     light_controller.changeRGB(signal, 255 - signal, 0, true);  // keep bri
// //     Settings.light_scheme = 0;
// //     if (0 == light_state.getBri()) {
// //       light_controller.changeBri(50);
// //     }
// //   }
// // }

// // // convert channels to string, use Option 17 to foce decimal, unless force_hex
// // char* LightGetColor(char* scolor, boolean force_hex = false)
// // {
// //   if ((0 == Settings.light_scheme) || (!pwm_multi_channels)) {
// //     light_controller.calcLevels();      // recalculate levels only if Scheme 0, otherwise we mess up levels
// //   }
// //   scolor[0] = '\0';
// //   for (uint32_t i = 0; i < subtype; i++) {
// //     if (!force_hex && Settings.flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
// //       snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", current_color[i]);
// //     } else {
// //       snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%02X"), scolor, current_color[i]);
// //     }
// //   }
// //   return scolor;
// // }

// // void LightPowerOn(void)
// // {
// //   if (light_state.getBri() && !(power)) {
// //     ExecuteCommandPower(device, POWER_ON, SRC_LIGHT);
// //   }
// // }

// // void LightState(uint8_t append)
// // {
// //   char scolor[LIGHT_COLOR_SIZE];
// //   char scommand[33];
// //   bool unlinked = !light_controller.isCTRGBLinked() && (subtype >= LST_RGBW);  // there are 2 power and dimmers for RGB and White

// //   if (append) {
// //     ResponseAppend_P(PSTR(","));
// //   } else {
// //     Response_P(PSTR("{"));
// //   }
// //   if (!pwm_multi_channels) {
// //     if (unlinked) {
// //       // RGB and W are unlinked, we display the second Power/Dimmer
// //       ResponseAppend_P(PSTR("\"" D_RSLT_POWER "%d\":\"%s\",\"" D_JSON_DIMMER "%d\":%d"
// //                            ",\"" D_RSLT_POWER "%d\":\"%s\",\"" D_JSON_DIMMER "%d\":%d"),
// //                             device, GetStateText(power & 1), device, light_state.getDimmer(1),
// //                             device + 1, GetStateText(power & 2 ? 1 : 0), device + 1, light_state.getDimmer(2));
// //     } else {
// //       GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable);  // SetOption26 - Switch between POWER or POWER1
// //       ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_JSON_DIMMER "\":%d"), scommand, GetStateText(power & 1),
// //                       light_state.getDimmer());
// //     }


// //     if (subtype > LST_SINGLE) {
// //       ResponseAppend_P(PSTR(",\"" D_JSON_COLOR "\":\"%s\""), LightGetColor(scolor));
// //       if (LST_RGB <= subtype) {
// //         uint16_t hue;
// //         uint8_t  sat, bri;
// //         light_state.getHSB(&hue, &sat, &bri);
// //         sat = mSupport::changeUIntScale(sat, 0, 255, 0, 100);
// //         bri = mSupport::changeUIntScale(bri, 0, 255, 0, 100);

// //         ResponseAppend_P(PSTR(",\"" D_JSON_HSBCOLOR "\":\"%d,%d,%d\""), hue,sat,bri);
// //       }
// //       // Add White level
// //       if ((LST_COLDWARM == subtype) || (LST_RGBW <= subtype)) {
// //         ResponseAppend_P(PSTR(",\"" D_JSON_WHITE "\":%d"), light_state.getDimmer(2));
// //       }
// //       // Add CT
// //       if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
// //         ResponseAppend_P(PSTR(",\"" D_JSON_COLORTEMPERATURE "\":%d"), light_state.getCT());
// //       }
// //       // Add status for each channel
// //       ResponseAppend_P(PSTR(",\"" D_JSON_CHANNEL "\":[" ));
// //       for (uint32_t i = 0; i < subtype; i++) {
// //         uint8_t channel_raw = current_color[i];
// //         uint8_t channel = mSupport::changeUIntScale(channel_raw,0,255,0,100);
// //         // if non null, force to be at least 1
// //         if ((0 == channel) && (channel_raw > 0)) { channel = 1; }
// //         ResponseAppend_P(PSTR("%s%d" ), (i > 0 ? "," : ""), channel);
// //       }
// //       ResponseAppend_P(PSTR("]"));
// //     }

// //     if (append) {
// //       if (subtype >= LST_RGB) {
// //         ResponseAppend_P(PSTR(",\"" D_JSON_SCHEME "\":%d"), Settings.light_scheme);
// //       }
// //       if (max_scheme > LS_MAX) {
// //         ResponseAppend_P(PSTR(",\"" D_JSON_WIDTH "\":%d"), Settings.light_width);
// //       }
// //       ResponseAppend_P(PSTR(",\"" D_JSON_FADE "\":\"%s\",\"" D_JSON_SPEED "\":%d,\"" D_JSON_LEDTABLE "\":\"%s\""),
// //         GetStateText(Settings.light_fade), Settings.light_speed, GetStateText(Settings.light_correction));
// //     }
// //   } else {  // pwm_multi_channels
// //     for (uint32_t i = 0; i < subtype; i++) {
// //       GetPowerDevice(scommand, device + i, sizeof(scommand), 1);
// //       uint32_t light_power_masked = power & (1 << i);    // the power value for this device
// //       light_power_masked = light_power_masked ? 1 : 0;                    // convert to on/off
// //       ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_JSON_CHANNEL "%d\":%d,"), scommand, GetStateText(light_power_masked), device + i,
// //         mSupport::changeUIntScale(current_color[i], 0, 255, 0, 100));
// //     }
// //     ResponseAppend_P(PSTR("\"" D_JSON_COLOR "\":\"%s\""), LightGetColor(scolor));
// //   }   // pwm_multi_channels

// //   if (!append) {
// //     ResponseJsonEnd();
// //   }
// // }

// // void LightPreparePower(power_t channels = 0xFFFFFFFF) {    // 1 = only RGB, 2 = only CT, 3 = both RGB and CT
// // #ifdef DEBUG_LIGHT
// //   AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower power=%d power=%d", power, power);
// // #endif
// //   // If multi-channels, then we only switch off channels with a value of zero
// //   if (pwm_multi_channels) {
// //     for (uint32_t i = 0; i < subtype; i++) {
// //       if (bitRead(channels, i)) {
// //         // if channel is non-null, channel is supposed to be on, but it is off, do Power On
// //         if ((current_color[i]) && (!bitRead(power, i))) {
// //           if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
// //             ExecuteCommandPower(device + i, POWER_ON_NO_STATE, SRC_LIGHT);
// //           }
// //         } else {
// //           // if channel is zero and channel is on, set it off
// //           if ((0 == current_color[i]) && bitRead(power, i)) {
// //             ExecuteCommandPower(device + i, POWER_OFF_NO_STATE, SRC_LIGHT);
// //           }
// //         }
// //   #ifdef USE_DOMOTICZ
// //         DomoticzUpdatePowerState(device + i);
// //   #endif  // USE_DOMOTICZ
// //       }
// //     }
// //   } else {
// //     if (light_controller.isCTRGBLinked()) {   // linked, standard
// //       if (light_state.getBri() && !(power)) {
// //         if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
// //           ExecuteCommandPower(device, POWER_ON_NO_STATE, SRC_LIGHT);
// //         }
// //       } else if (!light_state.getBri() && power) {
// //         ExecuteCommandPower(device, POWER_OFF_NO_STATE, SRC_LIGHT);
// //       }
// //     } else {
// //       // RGB
// //       if (channels & 1) {
// //         if (light_state.getBriRGB() && !(power & 1)) {
// //           if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
// //             ExecuteCommandPower(device, POWER_ON_NO_STATE, SRC_LIGHT);
// //           }
// //         } else if (!light_state.getBriRGB() && (power & 1)) {
// //           ExecuteCommandPower(device, POWER_OFF_NO_STATE, SRC_LIGHT);
// //         }
// //       }
// //       // White CT
// //       if (channels & 2) {
// //         if (light_state.getBriCT() && !(power & 2)) {
// //           if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
// //             ExecuteCommandPower(device + 1, POWER_ON_NO_STATE, SRC_LIGHT);
// //           }
// //         } else if (!light_state.getBriCT() && (power & 2)) {
// //           ExecuteCommandPower(device + 1, POWER_OFF_NO_STATE, SRC_LIGHT);
// //         }
// //       }
// //     }
// // #ifdef USE_DOMOTICZ
// //     DomoticzUpdatePowerState(device);
// // #endif  // USE_DOMOTICZ
// //   }

// //   if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
// //     MqttPublishTeleState();
// //   }

// // #ifdef DEBUG_LIGHT
// //   AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower End power=%d power=%d", power, power);
// // #endif
// //   power = power >> (device - 1);  // reset next state, works also with unlinked RGB/CT
// //   LightState(0);
// // }

// // #ifdef USE_LIGHT_PALETTE
// // void LightSetPaletteEntry(void)
// // {
// //   uint8_t bri = light_state.getBri();
// //   uint8_t * palette_entry = &palette[wheel * LST_MAX];
// //   for (int i = 0; i < LST_MAX; i++) {
// //     new_color[i] = mSupport::changeUIntScale(palette_entry[i], 0, 255, 0, bri);
// //   }
// //   light_state.setChannelsRaw(new_color);
// //   if (!pwm_multi_channels) {
// //     light_state.setCW(new_color[3], new_color[4], true);
// //     if (new_color[0] || new_color[1] || new_color[2]) light_state.addRGBMode();
// //   }
// // }
// // #endif  // USE_LIGHT_PALETTE

// void mPWMLight::LightCycleColor(int8_t direction)
// {
// // //  if (strip_timer_counter % (Settings.light_speed * 2)) { return; }  // Speed 1: 24sec, 2: 48sec, 3: 72sec, etc
// //   if (Settings.light_speed > 3) {
// //     if (strip_timer_counter % (Settings.light_speed - 2)) { return; }  // Speed 4: 24sec, 5: 36sec, 6: 48sec, etc
// //   }

// // #ifdef USE_LIGHT_PALETTE
// //   if (palette_count) {
// //     if (!fade_running) {
// //       if (0 == direction) {
// //         wheel = random(palette_count);
// //       }
// //       else {
// //         wheel += direction;
// //         if (wheel >= palette_count) {
// //           wheel = 0;
// //           if (direction < 0) wheel = palette_count - 1;
// //         }
// //       }
// //       LightSetPaletteEntry();
// //     }
// //     return;
// //   }
// // #endif  // USE_LIGHT_PALETTE

// //   if (0 == direction) {
// //     if (random == wheel) {
// //       random = random(255);

// //       uint8_t my_dir = (random < wheel -128) ? 1 :
// //                        (random < wheel     ) ? 0 :
// //                        (random > wheel +128) ? 0 : 1;  // Increment or Decrement and roll-over
// //       random = (random & 0xFE) | my_dir;

// // //      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LGT: random %d"), random);
// //     }
// // //    direction = (random < wheel) ? -1 : 1;
// //     direction = (random &0x01) ? 1 : -1;
// //   }

// // //  if (Settings.light_speed < 3) { direction <<= (3 - Settings.light_speed); }  // Speed 1: 12/4=3sec, 2: 12/2=6sec, 3: 12sec
// //   if (Settings.light_speed < 3) { direction *= (4 - Settings.light_speed); }  // Speed 1: 12/3=4sec, 2: 12/2=6sec, 3: 12sec
// //   wheel += direction;
// //   uint16_t hue = mSupport::changeUIntScale(wheel, 0, 255, 0, 359);  // Scale to hue to keep amount of steps low (max 255 instead of 359)

// // //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LGT: random %d, wheel %d, hue %d"), random, wheel, hue);

// //   if (!pwm_multi_channels) {
// //   uint8_t sat;
// //   light_state.getHSB(nullptr, &sat, nullptr);  // Allow user control over Saturation
// //   light_state.setHS(hue, sat);
// //   } else {
// //     light_state.setHS(hue, 255);
// //     light_state.setBri(255);        // If multi-channel, force bri to max, it will be later dimmed to correct value
// //   }
// //   light_controller.calcLevels(new_color);
// }

// void mPWMLight::LightSetPower(void)
// {
// // //  power = XdrvMailbox.index;
// //   old_power = power;
// //   //power = bitRead(XdrvMailbox.index, device -1);
// //   uint32_t mask = 1;  // default mask
// //   if (pwm_multi_channels) {
// //     mask = (1 << subtype) - 1;   // wider mask
// //   } else if (!light_controller.isCTRGBLinked()) {
// //     mask = 3;   // we got 2 devices, for RGB and White
// //   }
// //   uint32_t shift = device - 1;
// //   // If PWM multi_channels
// //   // Ex: 3 Relays and 4 PWM - devices_present = 7, device = 4, subtype = 4
// //   // Result: mask = 0b00001111 = 0x0F, shift = 3.
// //   // Power bits we consider are: 0b01111000 = 0x78
// //   // If regular situation: devices_present == subtype
// //   power = (XdrvMailbox.index & (mask << shift)) >> shift;
// //   if (wakeup_active) {
// //     wakeup_active--;
// //   }
// // #ifdef DEBUG_LIGHT
// //   AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightSetPower XdrvMailbox.index=%d old_power=%d power=%d mask=%d shift=%d",
// //     XdrvMailbox.index, old_power, power, mask, shift);
// // #endif
// //   if (power != old_power) {
// //     update = true;
// //   }
// //   LightAnimate();
// }

// // // On entry new_color[5] contains the color to be displayed
// // // and last_color[5] the color currently displayed
// // // power tells which lights or channels (SetOption68) are on/off
// void mPWMLight::LightAnimate(void)
// {
// //   uint16_t light_still_on = 0;
// //   bool power_off = false;

// //   // make sure we update CT range in case SetOption82 was changed
// //   light_controller.setAlexaCTRange(Settings.flag4.alexa_ct_range);
// //   strip_timer_counter++;

// //   // set sleep parameter: either settings,
// //   // or set a maximum of PWM_MAX_SLEEP if light is on or Fade is running
// //   if (power || fade_running) {
// //     if (Settings.sleep > PWM_MAX_SLEEP) {
// //       ssleep = PWM_MAX_SLEEP;      // set a maxumum value of 50 milliseconds to ensure that animations are smooth
// //     } else {
// //       ssleep = Settings.sleep;     // or keep the current sleep if it's lower than 50
// //     }
// //   } else {
// //     ssleep = Settings.sleep;
// //   }

// //   if (!power) {                   // All channels powered off
// //     strip_timer_counter = 0;
// //     if (Settings.light_scheme >= LS_MAX) {
// //       power_off = true;
// //     }
// //   } else {
// //     switch (Settings.light_scheme) {
// //       case LS_POWER:
// //         light_controller.calcLevels(new_color);
// //         break;
// //       case LS_WAKEUP:
// //         if (2 == wakeup_active) {
// //           wakeup_active = 1;
// //           for (uint32_t i = 0; i < subtype; i++) {
// //             new_color[i] = 0;
// //           }
// //           wakeup_counter = 0;
// //           wakeup_dimmer = 0;
// //         }
// //         wakeup_counter++;
// //         if (wakeup_counter > ((Settings.light_wakeup * STATES) / Settings.light_dimmer)) {
// //           wakeup_counter = 0;
// //           wakeup_dimmer++;
// //           if (wakeup_dimmer <= Settings.light_dimmer) {
// //             light_state.setDimmer(wakeup_dimmer);
// //             light_controller.calcLevels();
// //             for (uint32_t i = 0; i < subtype; i++) {
// //               new_color[i] = current_color[i];
// //             }
// //           } else {
// // /*
// //             Response_P(PSTR("{\"" D_JSON_WAKEUP "\":\"" D_JSON_DONE "\"}"));
// //             MqttPublishPrefixTopic_P(TELE, PSTR(D_JSON_WAKEUP));
// // */
// //             Response_P(PSTR("{\"" D_JSON_WAKEUP "\":\"" D_JSON_DONE "\""));
// //             LightState(1);
// //             ResponseJsonEnd();
// //             MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_WAKEUP));
// //             XdrvRulesProcess();

// //             wakeup_active = 0;
// //             Settings.light_scheme = LS_POWER;
// //           }
// //         }
// //         break;
// //       case LS_CYCLEUP:
// //       case LS_CYCLEDN:
// //       case LS_RANDOM:
// //         if (LS_CYCLEUP == Settings.light_scheme) {
// //           LightCycleColor(1);
// //         } else if (LS_CYCLEDN == Settings.light_scheme) {
// //           LightCycleColor(-1);
// //         } else {
// //           LightCycleColor(0);
// //         }
// //         if (pwm_multi_channels) {     // See #8058
// //           new_color[0] = mSupport::changeUIntScale(new_color[0], 0, 255, 0, Settings.light_color[0]);
// //           new_color[1] = mSupport::changeUIntScale(new_color[1], 0, 255, 0, Settings.light_color[1]);
// //           new_color[2] = mSupport::changeUIntScale(new_color[2], 0, 255, 0, Settings.light_color[2]);
// //         }
// //         break;
// //       default:
// //         XlgtCall(FUNC_SET_SCHEME);
// //     }

// // #ifdef USE_DEVICE_GROUPS
// //     if (Settings.light_scheme != last_scheme) {
// //       last_scheme = Settings.light_scheme;
// //       SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_SCHEME, Settings.light_scheme);
// //       devgrp_no_channels_out = false;
// //     }
// // #endif  // USE_DEVICE_GROUPS
// //   }

// //   if ((Settings.light_scheme < LS_MAX) || power_off) {  // exclude WS281X Neopixel schemes

// //     // Apply power modifiers to new_color
// //     LightApplyPower(new_color, power);

// //     // AddLog_P2(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
// //     // last_color[0], last_color[1], last_color[2], last_color[3], last_color[4],
// //     // new_color[0], new_color[1], new_color[2], new_color[3], new_color[4],
// //     // power
// //     // );

// //     if (memcmp(last_color, new_color, subtype)) {
// //       update = true;
// //     }
// //     if (update) {
// // #ifdef USE_DEVICE_GROUPS
// //       if (power) LightSendDeviceGroupStatus(false);
// // #endif  // USE_DEVICE_GROUPS

// //       uint16_t cur_col_10[LST_MAX];   // 10 bits resolution
// //       update = false;

// //       // first set 8 and 10 bits channels
// //       for (uint32_t i = 0; i < LST_MAX; i++) {
// //         last_color[i] = new_color[i];
// //         // Extend from 8 to 10 bits if no correction (in case no gamma correction is required)
// //         cur_col_10[i] = change8to10(new_color[i]);
// //       }

// //       if (pwm_multi_channels) {
// //         calcGammaMultiChannels(cur_col_10);
// //       } else {
// //         calcGammaBulbs(cur_col_10);

// //         // Now see if we need to mix RGB and True White
// //         // Valid only for LST_RGBW, LST_RGBCW, rgbwwTable[4] is zero, and white is zero (see doc)
// //         if ((LST_RGBW <= subtype) && (0 == Settings.rgbwwTable[4]) && (0 == cur_col_10[3]+cur_col_10[4])) {
// //           uint32_t min_rgb_10 = min3(cur_col_10[0], cur_col_10[1], cur_col_10[2]);
// //           for (uint32_t i=0; i<3; i++) {
// //             // substract white and adjust according to rgbwwTable
// //             uint32_t adjust10 = change8to10(Settings.rgbwwTable[i]);
// //             cur_col_10[i] = mSupport::changeUIntScale(cur_col_10[i] - min_rgb_10, 0, 1023, 0, adjust10);
// //           }

// //           // compute the adjusted white levels for 10 and 8 bits
// //           uint32_t adjust_w_10 = mSupport::changeUIntScale(Settings.rgbwwTable[3], 0, 255, 0, 1023);
// //           uint32_t white_10 = mSupport::changeUIntScale(min_rgb_10, 0, 1023, 0, adjust_w_10);  // set white power down corrected with rgbwwTable[3]
// //           if (LST_RGBW == subtype) {
// //             // we simply set the white channel
// //             cur_col_10[3] = white_10;
// //           } else {  // LST_RGBCW
// //             // we distribute white between cold and warm according to CT value
// //             uint32_t ct = light_state.getCT10bits();
// //             cur_col_10[4] = mSupport::changeUIntScale(ct, 0, 1023, 0, white_10);
// //             cur_col_10[3] = white_10 - cur_col_10[4];
// //           }
// //         }
// //       }

// //       // Apply RGBWWTable only if Settings.rgbwwTable[4] != 0
// //       if (0 != Settings.rgbwwTable[4]) {
// //         for (uint32_t i = 0; i<subtype; i++) {
// //           uint32_t adjust = change8to10(Settings.rgbwwTable[i]);
// //           cur_col_10[i] = mSupport::changeUIntScale(cur_col_10[i], 0, 1023, 0, adjust);
// //         }
// //       }

// //       // final adjusments for PMW, post-gamma correction
// //       for (uint32_t i = 0; i < LST_MAX; i++) {
// //         // scale from 0..1023 to 0..pwm_range, but keep any non-zero value to at least 1
// //         cur_col_10[i] = (cur_col_10[i] > 0) ? mSupport::changeUIntScale(cur_col_10[i], 1, 1023, 1, Settings.pwm_range) : 0;
// //       }

// //       // apply port remapping on both 8 bits and 10 bits versions
// //       uint16_t orig_col_10bits[LST_MAX];
// //       memcpy(orig_col_10bits, cur_col_10, sizeof(orig_col_10bits));
// //       for (uint32_t i = 0; i < LST_MAX; i++) {
// //         cur_col_10[i] = orig_col_10bits[color_remap[i]];
// //       }

// //       if (!Settings.light_fade || skip_light_fade || power_off || (!fade_initialized)) { // no fade
// //         // record the current value for a future Fade
// //         memcpy(fade_start_10, cur_col_10, sizeof(fade_start_10));
// //         // push the final values at 8 and 10 bits resolution to the PWMs
// //         LightSetOutputs(cur_col_10);
// //         fade_initialized = true;      // it is now ok to fade
// //       } else {  // fade on
// //         if (fade_running) {
// //           // if fade is running, we take the curring value as the start for the next fade
// //           memcpy(fade_start_10, fade_cur_10, sizeof(fade_start_10));
// //         }
// //         memcpy(fade_end_10, cur_col_10, sizeof(fade_start_10));
// //         fade_running = true;
// //         fade_duration = 0;    // set the value to zero to force a recompute
// //         fade_start = 0;
// //         // Fade will applied immediately below
// //       }
// //     }
// //     if (fade_running) {
// //       if (LightApplyFade()) {
// //         // AddLog_P2(LOG_LEVEL_INFO, PSTR("LightApplyFade %d %d %d %d %d"),
// //         //   fade_cur_10[0], fade_cur_10[1], fade_cur_10[2], fade_cur_10[3], fade_cur_10[4]);

// //         LightSetOutputs(fade_cur_10);
// //       }
// //     }
// // #ifdef USE_PWM_DIMMER
// //     // If the power is off and the fade is done, turn the relay off.
// //     if (PWM_DIMMER == my_module_type && !power && !fade_running) PWMDimmerSetPower();
// // #endif  // USE_PWM_DIMMER
// //   }
// }


// bool mPWMLight::LightApplyFade(void) {   // did the value chanegd and needs to be applied
// //   static uint32_t last_millis = 0;
// //   uint32_t now = millis();

// //   if ((now - last_millis) <= 5) {
// //     return false;     // the value was not changed in the last 5 milliseconds, ignore
// //   }
// //   last_millis = now;

// //   // Check if we need to calculate the duration
// //   if (0 == fade_duration) {
// //     fade_start = now;
// //     // compute the distance between start and and color (max of distance for each channel)
// //     uint32_t distance = 0;
// //     for (uint32_t i = 0; i < subtype; i++) {
// //       int32_t channel_distance = fadeGammaReverse(i, fade_end_10[i]) - fadeGammaReverse(i, fade_start_10[i]);
// //       if (channel_distance < 0) { channel_distance = - channel_distance; }
// //       if (channel_distance > distance) { distance = channel_distance; }
// //     }
// //     if (distance > 0) {
// //       // compute the duration of the animation
// //       // Note: Settings.light_speed is the number of half-seconds for a 100% fade,
// //       // i.e. light_speed=1 means 1024 steps in 500ms
// //       fade_duration = (distance * Settings.light_speed * 500) / 1023;
// //       if (Settings.save_data) {
// //         // Also postpone the save_data for the duration of the Fade (in seconds)
// //         uint32_t delay_seconds = 1 + (fade_duration + 999) / 1000;   // add one more second
// //         // AddLog_P2(LOG_LEVEL_INFO, PSTR("delay_seconds %d, save_data_counter %d"), delay_seconds, save_data_counter);
// //         if (save_data_counter < delay_seconds) {
// //           save_data_counter = delay_seconds;      // pospone
// //         }
// //       }
// //     } else {
// //       // no fade needed, we keep the duration at zero, it will fallback directly to end of fade
// //       fade_running = false;
// //     }
// //   }

// //   uint16_t fade_current = now - fade_start;   // number of milliseconds since start of fade
// //   if (fade_current <= fade_duration) {    // fade not finished
// //     //Serial.printf("Fade: %d / %d - ", fade_current, fade_duration);
// //     for (uint32_t i = 0; i < subtype; i++) {
// //       fade_cur_10[i] = fadeGamma(i,
// //                                 mSupport::changeUIntScale(fadeGammaReverse(i, fade_current),
// //                                              0, fade_duration,
// //                                              fadeGammaReverse(i, fade_start_10[i]),
// //                                              fadeGammaReverse(i, fade_end_10[i])));
// //       // fade_cur_10[i] = mSupport::changeUIntScale(fade_current,
// //       //                                        0, fade_duration,
// //       //                                        fade_start_10[i], fade_end_10[i]);
// //     }
// //   } else {
// //     // stop fade
// // //AddLop_P2(LOG_LEVEL_DEBUG, PSTR("Stop fade"));
// //     fade_running = false;
// //     fade_start = 0;
// //     fade_duration = 0;
// //     // set light to target value
// //     memcpy(fade_cur_10, fade_end_10, sizeof(fade_end_10));
// //     // record the last value for next start
// //     memcpy(fade_start_10, fade_end_10, sizeof(fade_start_10));
// //   }
// //   return true;
// }

// // On entry we take the 5 channels 8 bits entry, and we apply Power modifiers
// // I.e. shut down channels that are powered down
// void LightApplyPower(uint8_t new_color[LST_MAX], power_t power) {
//   // If SetOption68, multi_channels
//   // if (pwm_multi_channels) {
//   //   // if multi-channels, specifically apply the power bits
//   //   for (uint32_t i = 0; i < LST_MAX; i++) {
//   //     if (0 == bitRead(power,i)) {  // if power down bit is zero
//   //       new_color[i] = 0;   // shut down this channel
//   //     }
//   //   }
//   //   // #ifdef DEBUG_LIGHT
//   //   // AddLog_P2(LOG_LEVEL_DEBUG_MORE, "Animate>> power=%d new_color=[%d,%d,%d,%d,%d]",
//   //   //   power, new_color[0], new_color[1], new_color[2],
//   //   //   new_color[3], new_color[4]);
//   //   // #endif
//   // } else {
//   //   // if (!light_controller.isCTRGBLinked()) {
//   //   //   // we have 2 power bits for RGB and White
//   //   //   if (0 == (power & 1)) {
//   //   //     new_color[0] = new_color[1] = new_color[2] = 0;
//   //   //   }
//   //   //   if (0 == (power & 2)) {
//   //   //     new_color[3] = new_color[4] = 0;
//   //   //   }
//   //   // } else if (!power) {
//   //   //   for (uint32_t i = 0; i < LST_MAX; i++) {
//   //   //     new_color[i] = 0;
//   //   //   }
//   //   // }
//   // }
// }


// // // Just apply basic Gamma to each channel
// // void calcGammaMultiChannels(uint16_t cur_col_10[5]) {
// //   // Apply gamma correction for 8 and 10 bits resolutions, if needed
// //   if (Settings.light_correction) {
// //     for (uint32_t i = 0; i < LST_MAX; i++) {
// //       cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
// //     }
// //   }
// // }

// // void calcGammaBulbs(uint16_t cur_col_10[5]) {
// //   // Apply gamma correction for 8 and 10 bits resolutions, if needed

// //   // First apply combined correction to the overall white power
// //   if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
// //     // channels for white are always the last two channels
// //     uint32_t cw1 = subtype - 1;       // address for the ColorTone PWM
// //     uint32_t cw0 = subtype - 2;       // address for the White Brightness PWM
// //     uint16_t white_bri10 = cur_col_10[cw0] + cur_col_10[cw1];   // cumulated brightness
// //     uint16_t white_bri10_1023 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023

// // #ifdef ESP8266
// //     if ((PHILIPS == my_module_type) || (Settings.flag4.pwm_ct_mode)) {   // channel 1 is the color tone, mapped to cold channel (0..255)
// //       // Xiaomi Philips bulbs follow a different scheme:
// //       cur_col_10[cw1] = light_state.getCT10bits();
// //       // channel 0=intensity, channel1=temperature
// //       if (Settings.light_correction) { // gamma correction
// //         cur_col_10[cw0] = ledGamma10_10(white_bri10_1023);    // 10 bits gamma correction
// //       } else {
// //         cur_col_10[cw0] = white_bri10_1023;  // no gamma, extend to 10 bits
// //       }
// //     } else
// // #endif  // ESP8266
// //     if (Settings.light_correction) {
// //       // if sum of both channels is > 255, then channels are probably uncorrelated
// //       if (white_bri10 <= 1031) {      // take a margin of 8 above 1023 to account for rounding errors
// //         // we calculate the gamma corrected sum of CW + WW
// //         uint16_t white_bri_gamma10 = ledGamma10_10(white_bri10_1023);
// //         // then we split the total energy among the cold and warm leds
// //         cur_col_10[cw0] = mSupport::changeUIntScale(cur_col_10[cw0], 0, white_bri10_1023, 0, white_bri_gamma10);
// //         cur_col_10[cw1] = mSupport::changeUIntScale(cur_col_10[cw1], 0, white_bri10_1023, 0, white_bri_gamma10);
// //       } else {
// //         cur_col_10[cw0] = ledGamma10_10(cur_col_10[cw0]);
// //         cur_col_10[cw1] = ledGamma10_10(cur_col_10[cw1]);
// //       }
// //     }
// //   }

// //   if (Settings.light_correction) {
// //     // then apply gamma correction to RGB channels
// //     if (LST_RGB <= subtype) {
// //       for (uint32_t i = 0; i < 3; i++) {
// //         cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
// //       }
// //     }
// //     // If RGBW or Single channel, also adjust White channel
// //     if ((LST_SINGLE == subtype) || (LST_RGBW == subtype)) {
// //       cur_col_10[subtype - 1] = ledGamma10_10(cur_col_10[subtype - 1]);
// //     }
// //   }
// // }



// /****************** ***
//  * 
//  * 
//  * 
//  * HELPER FUNCTIONS TO SORT
//  * 
//  * 
//  * 
//  * 
//  * *******************/


// bool mPWMLight::isChannelGammaCorrected(uint32_t channel) {
// //   if (!Settings.light_correction) { return false; }   // Gamma correction not activated
// //   if (channel >= subtype) { return false; }     // Out of range
// // #ifdef ESP8266
// //   if ((PHILIPS == my_module_type) || (Settings.flag4.pwm_ct_mode)) {
// //     if ((LST_COLDWARM == subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
// //     if ((LST_RGBCW == subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
// //   }
// // #endif  // ESP8266
// //   return true;
// }

// // is the channel a regular PWM or ColorTemp control
// bool mPWMLight::isChannelCT(uint32_t channel) {
// // #ifdef ESP8266
// //   if ((PHILIPS == my_module_type) || (Settings.flag4.pwm_ct_mode)) {
// //     if ((LST_COLDWARM == subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
// //     if ((LST_RGBCW == subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
// //   }
// // #endif  // ESP8266
//   return false;
// }

// // // Calculate the Gamma correction, if any, for fading, using the fast Gamma curve (10 bits in+out)
// // uint16_t fadeGamma(uint32_t channel, uint16_t v) {
// //   if (isChannelGammaCorrected(channel)) {
// //     return ledGamma_internal(v, gamma_table_fast);
// //   } else {
// //     return v;
// //   }
// // }
// // uint16_t fadeGammaReverse(uint32_t channel, uint16_t vg) {
// //   if (isChannelGammaCorrected(channel)) {
// //     return ledGammaReverse_internal(vg, gamma_table_fast);
// //   } else {
// //     return vg;
// //   }
// // }


// /****
//  * 
//  * 
//  * Helper colour functions
//  * RGBCCT is the primary way of storing the colours
//  * Functions will Read and Set in HSBSpace, but then store as rgb
//  * 
//  * ***********************************************************/

// // Take HSB in, and store in RGB colour space
// void mPWMLight::SetColour_HSB(HsbColor hsb){

//   RgbColor rgb = RgbColor(hsb);

//   current_color[COLOUR_RED_ID]    = rgb.R;
//   current_color[COLOUR_GREEN_ID]  = rgb.G;
//   current_color[COLOUR_BLUE_ID]   = rgb.B;

//   // Work out white and warm white if needed

// }

// // Take HSB in, and store in RGB colour space
// void mPWMLight::SetColour_Hue(uint16_t hue){

//   // Get saved hsb using rgb variables
//   HsbColor hsb_saved = HsbColor(
//                         RgbColor(current_color[COLOUR_RED_ID],
//                                  current_color[COLOUR_GREEN_ID],
//                                  current_color[COLOUR_BLUE_ID]
//                                 )
//                       );
//   // Update hue
//   hsb_saved.H = hue;

//   // Save as rgb space again
//   RgbColor rgb_new = HsbColor(hsb_saved);
//   current_color[COLOUR_RED_ID]    = rgb_new.R;
//   current_color[COLOUR_GREEN_ID]  = rgb_new.G;
//   current_color[COLOUR_BLUE_ID]   = rgb_new.B;

//   // Work out white and warm white if needed

// }

// // Take HSB in, and store in RGB colour space
// void mPWMLight::SetColour_Sat(uint8_t sat){

//   // Get saved hsb using rgb variables
//   HsbColor hsb_saved = HsbColor(
//                         RgbColor(current_color[COLOUR_RED_ID],
//                                  current_color[COLOUR_GREEN_ID],
//                                  current_color[COLOUR_BLUE_ID]
//                                 )
//                       );
//   // Update hue
//   hsb_saved.S = sat;

//   // Save as rgb space again
//   RgbColor rgb_new = HsbColor(hsb_saved);
//   current_color[COLOUR_RED_ID]    = rgb_new.R;
//   current_color[COLOUR_GREEN_ID]  = rgb_new.G;
//   current_color[COLOUR_BLUE_ID]   = rgb_new.B;

//   // Work out white and warm white if needed

// }

// // Take HSB in, and store in RGB colour space
// void mPWMLight::SetColour_Brt(uint8_t brt){

//   // Get saved hsb using rgb variables
//   HsbColor hsb_saved = HsbColor(
//                         RgbColor(current_color[COLOUR_RED_ID],
//                                  current_color[COLOUR_GREEN_ID],
//                                  current_color[COLOUR_BLUE_ID]
//                                 )
//                       );
//   // Update hue
//   hsb_saved.B = brt;

//   // Save as rgb space again
//   RgbColor rgb_new = HsbColor(hsb_saved);
//   current_color[COLOUR_RED_ID]    = rgb_new.R;
//   current_color[COLOUR_GREEN_ID]  = rgb_new.G;
//   current_color[COLOUR_BLUE_ID]   = rgb_new.B;

//   // Work out white and warm white if needed

// }







#endif // USE_DRIVER

// #endif // HEADER_GUARD