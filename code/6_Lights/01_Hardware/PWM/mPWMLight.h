#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#define D_UNIQUE_MODULE_DRIVERS_PWM_ID 46

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "1_TaskerManager/mTaskerManager.h"


class mPWMLight :
  public mTaskerInterface
{
  private:
    RgbcctColor output_colour;
    RgbcctColor transition_colour;
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);
    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);
    
  public:
    mPWMLight(){};
    void Init();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0); //TBR
    void parse_JSONCommand(JsonParserObject obj);    //TBR
    
    static const char* PM_MODULE_DRIVERS_PWM_CTR;
    static const char* PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_PWM_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_PWM_ID; }
     #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mPWMLight);
    };
    #endif

    void SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware);
    RgbcctColor GetPixelColorHardware(uint16_t index);
    void ShowHardware();

};
#endif

#endif

