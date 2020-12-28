#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "1_TaskerManager/mTaskerManager.h"
#include "6_Lights/PWMLights/mPWMLight_web.h"


class mPWMLight{
  public:
    mPWMLight(){};
    void Init();
    int8_t Tasker(uint8_t function);

    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);

    void FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0));
                
    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(void);

    #define LST_MAX 5
    
    struct AnimationColours
    {
      RgbcctColor StartingColor;
      RgbcctColor DesiredColour;
    };
    AnimationColours animation_colours;

    void BlendAnimUpdate(const AnimationParam& param);

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    RgbcctColor output_colour;
    void LightUpdate();
    void SubTask_Animation();
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);


    #ifdef USE_MODULE_CORE_WEBSERVER
      void WebCommand_Parse();
      int8_t Tasker_Web(uint8_t function);
      void WebAppend_JSON_RootPage_LiveviewPixels();
      void WebAppend_Root_ControlUI();
      void WebAppend_Root_Status_Table();
    #endif // USE_MODULE_CORE_WEBSERVER

};
#endif

#endif

