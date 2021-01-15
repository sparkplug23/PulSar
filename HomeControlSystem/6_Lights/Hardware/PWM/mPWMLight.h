#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "1_TaskerManager/mTaskerManager.h"


class mPWMLight{
  private:
    RgbcctColor output_colour;
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);
    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);
    
  public:
    mPWMLight(){};
    void Init();
    int8_t Tasker(uint8_t function); //TBR
    void parse_JSONCommand(void);    //TBR

    void SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware);
    RgbcctColor GetPixelColorHardware(uint16_t index);
    void ShowHardware();

};
#endif

#endif

