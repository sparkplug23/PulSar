#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "1_TaskerManager/mTaskerManager.h"


class mPWMLight :
  public mTaskerInterface
{
  private:
    RgbcctColor output_colour;
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);
    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);
    
  public:
    mPWMLight(){};
    void Init();
    int8_t Tasker(uint8_t function); //TBR
    void parse_JSONCommand(void);    //TBR
    
    static const char* PM_MODULE_DRIVERS_PWM_CTR;
    static const char* PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_PWM_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR; }

    void SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware);
    RgbcctColor GetPixelColorHardware(uint16_t index);
    void ShowHardware();

};
#endif

#endif

