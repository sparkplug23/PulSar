#include "mPWMLight.h"

#ifdef USE_MODULE_LIGHTS_PWM 


int8_t mPWMLight::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_INIT){
    Init();
  }

  if(pCONT_set->Settings.light_settings.type > LT_LIGHT_INTERFACE_END){
    return FUNCTION_RESULT_MODULE_DISABLED_ID; 
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      SubTask_Animation();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;
  } // end switch
    
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END function


void mPWMLight::Init(void){

  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object

}



int8_t mPWMLight::CheckAndExecute_JSONCommands(){

  // Check if instruction is for me
  if(mSupport::SetTopicMatch_P(data_buffer.topic.ctr,PM_MODULE_LIGHTS_PWM_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_MODULE_LIGHTS_PWM_FRIENDLY_CTR));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mPWMLight::parse_JSONCommand(void){

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_ERROR, PM_JSON_DESERIALIZATION_ERROR);
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

}

void mPWMLight::SubTask_Animation(){
  
  if (pCONT_iLight->animator_controller->IsAnimating()){ 
    pCONT_iLight->animator_controller->UpdateAnimations();
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog_P(LOG_LEVEL_DEBUG,PSTR("AnimUpdateCallback output_colour\t\t=%d"),output_colour.R);
    #endif // ENABLE_LOG_LEVEL_DEBUG
    LightUpdate();
    pCONT_iLight->animation.flags.fRunning = true; 
    pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
  }else{
    pCONT_iLight->animation.flags.fRunning = false;
    pCONT_set->Settings.enable_sleep = true;
  }

}


void mPWMLight::LightUpdate(){

  LightSetPWMOutputsRgbcctColor(output_colour);

}


// optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// If unset (as defined in header) the pCONT_iLight->animation will blend from current pixel colours with getpixel
void mPWMLight::FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 
  // colour already adjusted the brightness
  #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(  "\n\r\n\r\n" D_LOG_NEO "FadeToNewColour"));
  #endif

  RgbcctColor fromcolor_npb = output_colour;//GetOutputColour();//GetPixelColor(pixel);
  RgbcctColor targetColor_npb = targetColor;

  animation_colours.StartingColor = fromcolor_npb;
  animation_colours.DesiredColour = targetColor_npb;
  
  pCONT_iLight->animation_changed_millis = millis();

  uint16_t time_tmp = _time_to_newcolour;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
  //   case TRANSITION_METHOD_BLEND_ID:   time_tmp = _time_to_newcolour; break;
  // }

  if(pCONT_iLight->animation_override.time_ms){
    AddLog_P(LOG_LEVEL_TEST, PSTR("animation_override.time_ms ENABLED = %d"),pCONT_iLight->animation_override.time_ms);
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0; //reset overwrite
  }

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000; // ms to seconds
    }
  }
  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );
  
} // END function



void mPWMLight::BlendAnimUpdate(const AnimationParam& param)
{    
  output_colour = RgbcctColor::LinearBlend(
      animation_colours.StartingColor,
      animation_colours.DesiredColour,
      param.progress);
}


void mPWMLight::LightSetPWMOutputsRgbcctColor(RgbcctColor colour){

  uint16_t tmp_colour[5];

  tmp_colour[0] = pCONT_iLight->change8to10(colour.R);
  tmp_colour[1] = pCONT_iLight->change8to10(colour.G);
  tmp_colour[2] = pCONT_iLight->change8to10(colour.B);
  tmp_colour[3] = pCONT_iLight->change8to10(colour.WC);
  tmp_colour[4] = pCONT_iLight->change8to10(colour.WW); 

  // AddLog_Array(LOG_LEVEL_TEST,PSTR("tmp_colour"),tmp_colour,(uint16_t)5);

  LightSetPWMOutputsArray10bit(tmp_colour);

}

/**
 * Kept using array for sutypes of other colours
 * */
void mPWMLight::LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10) {
  
  uint16_t cur_col;
  uint16_t pin_num;
  uint16_t pwm_value;

  // now apply the actual PWM values, adjusted and remapped 10-bits range
  if (pCONT_set->Settings.light_settings.type < LT_PWM6) {
    for (uint8_t i = 0; i < (pCONT_iLight->subtype - pCONT_iLight->pwm_offset); i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) {
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        cur_col = cur_col_10[i + pCONT_iLight->pwm_offset]; //leak chance
        if(!pCONT_iLight->isChannelCT(i)) {   // if CT don't use pwm_min and pwm_max
          cur_col = cur_col > 0 ? mapvalue(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
        }
        pin_num = pCONT_pins->Pin(GPIO_PWM1_ID, i);
        pwm_value = bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col;
        // Serial.printf("%d pin_num=%d, %d/%d, %d\n\r", i, pin_num, cur_col, pCONT_set->Settings.pwm_range, pwm_value);
        analogWrite(pin_num, pwm_value);
      } //pin used
    } //subtype
  } //light type

}

#endif // USE_DRIVER
