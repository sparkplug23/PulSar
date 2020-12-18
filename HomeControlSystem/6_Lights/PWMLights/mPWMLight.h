#ifndef MPWMLIGHT_H
#define MPWMLIGHT_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_LIGHTS_PWM

#include "1_TaskerManager/mTaskerManager.h"

const char kTitle_TableTitles_Root_PWM[] PROGMEM = 
  "Animation Status" "|" 
  "Lights Auto Off Timer";

class mPWMLight{//} : public mInterfaceLight{
  public:
    mPWMLight(){};
    void Init();
    int8_t Tasker(uint8_t function);

    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);

    void FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0));
        
        
int8_t CheckAndExecute_JSONCommands();
void parse_JSONCommand(void);


    #define LST_MAX 5
    // uint16_t fade_start_10[LST_MAX] = {0,0,0,0,0};
    // uint16_t fade_cur_10[LST_MAX];
    // uint16_t fade_end_10[LST_MAX];         // 10 bits resolution target channel values
    uint16_t fade_duration = 0;            // duration of fade in milliseconds
    uint32_t fade_start = 0;               // fade start time in milliseconds, compared to millis()
    
    // what is stored for state is specific to the need, in this case, the colors.
    // Basically what ever you need inside the animation update function
    struct AnimationColours
    {
      RgbcctColor StartingColor;
      RgbcctColor DesiredColour;
    };
    AnimationColours animation_colours;

    void BlendAnimUpdate(const AnimationParam& param);

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    RgbcctColor output_colour; // what is shown
    
    // #ifndef ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE
    // NeoPixelAnimator* animator_controller = nullptr;    //move on animator object into interface, using pointer, change its size when needed
    // #endif // ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE

    void LightUpdate();

    void WebCommand_Parse();
    int8_t Tasker_Web(uint8_t function);

    uint32_t tSavedFadeProgress = 0;
    uint32_t tSavedLightSetOutputs = 0;
    uint32_t tSavedColourChange = 0;
    uint32_t tSavedAutoOff = 0;

    void WebAppend_JSON_RootPage_LiveviewPixels();
    void WebAppend_Root_ControlUI();
    void WebAppend_Root_Status_Table();

    void LightAnimate(void);
    void SubTask_Animation();
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);
    // bool LightApplyFade2(void);

};
#endif

#endif

