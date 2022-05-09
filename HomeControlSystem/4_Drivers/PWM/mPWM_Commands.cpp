
#include "mPWM.h"

#ifdef USE_MODULE_DRIVERS_PWM

void mPWM::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  char buffer[20];

  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {

    snprintf(buffer, sizeof(buffer), "PWM%d\0", i);
    
    if(jtok = obj["PWM_Manual"].getObject()[buffer]){
      pwm[i].value = jtok.getInt();
      ALOG_INF(PSTR("PWM%d=%d"), i, pwm[i].value);
    }
    
  }

  uint16_t blend_duration = 1000;


  if(jtok = obj["PWM_Manual"].getObject()["Duration"]){
    blend_duration = jtok.getInt();
    // var_blend->StartAnimation(0, 5000);
    // ALOG_INF(PSTR("GetTarget=%d"), var_blend->GetTarget());
  }



  // if(jtok = obj["PWM_Manual"].getObject()["Target_f"]){
  //   var_blend_f->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //   ALOG_INF(PSTR("GetTarget=%d"), var_blend_f->GetTarget());
  // }
  // if(jtok = obj["PWM_Manual"].getObject()["Target_u8"]){
  //   var_blend_u8->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //   ALOG_INF(PSTR("GetTarget=%d"), var_blend_u8->GetTarget());
  // }


/**
 * @brief 
 * New method of setting the PWM channels to change slowly over time, with ability later to set timescale (this can be changed at runtime?)
 * 
 */

    // uint16_t blend_duration_arr[MAX_PWM_PINS] = {0}; // default to 1 second for now

  // uint16_t blend_duration = 0; // default to 1 second for now

  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {   
    
  //   blend_duration = 0; // default to 1 second for now

  //   snprintf(buffer, sizeof(buffer), "Duration%d\0", i);    
  //   if(jtok = obj["PWM_Blend"].getObject()[buffer]){
  //     blend_duration = jtok.getInt();
  //     ALOG_INF(PSTR("PWM%d blend_duration=%d"), i, blend_duration);
  //   }

  //   snprintf(buffer, sizeof(buffer), "Value%d\0", i);    
  //   if(jtok = obj["PWM_Blend"].getObject()[buffer]){
  //     pwm[i].blended_value->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //     ALOG_INF(PSTR("PWM%d GetTarget()=%d for %d (ms)"), i, pwm[i].blended_value->GetTarget(), blend_duration);
  //   }   

  // }


  if(jtok = obj["PWM_Blend"]){ 

    ALOG_INF(PSTR("PWM_Blend is found"));

    JsonParserToken jtok_duration = 0; 
    JsonParserToken jtok_value = 0; 
    
    uint16_t duration_array[5] = {0};
    uint8_t duration_arrlen = 0;
    uint16_t value_array[5] = {0};
    uint8_t value_arrlen = 0;

    if(jtok_duration = jtok.getObject()["Duration"])
    {
        
      if(jtok_duration.isArray()){
        JsonParserArray arrobj = jtok_duration;
        for(auto v : arrobj) {
          if(duration_arrlen > 5){ break; }
          duration_array[duration_arrlen++] = v.getInt();
        }
      }

    }

    if(jtok_value = jtok.getObject()["Value"])
    {        
      if(jtok_value.isArray()){
        JsonParserArray arrobj = jtok_value;
        for(auto v : arrobj) {
          if(value_arrlen > 5){ break; }
          value_array[value_arrlen++] = v.getInt();
          // ALOG_INF(PSTR("PWM_Blend value_array[%d] =%d"), value_arrlen-1, value_array[value_arrlen-1]);
        }
      }
    }
    if(jtok_value = jtok.getObject()["ValuePercentage"])
    {        
      if(jtok_value.isArray()){
        JsonParserArray arrobj = jtok_value;
        for(auto v : arrobj) {
          if(value_arrlen > 5){ break; }
          value_array[value_arrlen] = map(v.getInt(), 0,100, pwm[value_arrlen].range_min, pwm[value_arrlen].range_max);
          // ALOG_INF(PSTR("PWM_Blend value_array[%d] =%d"), value_arrlen-1, value_array[value_arrlen-1]);
          value_arrlen++;;
        }
      }
    }

    /**
     * @brief Start blending
     * 
     */
    for(int i=0;i<5;i++)
    {
      pwm[i].blended_value->SetTargetAndStartBlend(value_array[i], duration_array[i]);
      ALOG_INF(PSTR("PWM%d GetTarget()=%d for %d (ms)"), i, pwm[i].blended_value->GetTarget(), pwm[i].blended_value->BlendDuration());
    }

  }








}




#endif // USE_MODULE_DRIVERS_RELAY



