#ifndef mAddressableLight2_H
#define mAddressableLight2_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_ADDRESSABLE_ID 142

// Functions in here will be identical between hardware types, with get, set, configs that init the interface and animator level depending on the hardware type

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

#include "1_TaskerManager/mTaskerManager.h"
// #include "6_Lights/Hardware/Addressable/"


class mAddressableLight :
  public mTaskerInterface
{
  public:
    mAddressableLight(){};
    void Init();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static const char* PM_MODULE_LIGHTS_ADDRESSABLE_CTR;
    static const char* PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ADDRESSABLE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ADDRESSABLE_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mAddressableLight);
    };
    #endif


    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);

    void FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0));
                
    void parse_JSONCommand(JsonParserObject obj);

    void SetPixelColorHardware(uint16_t index, RgbcctColor colour_hardware, bool flag_replicate_for_total_pixel_length = false);
    RgbcctColor GetPixelColorHardware(uint16_t index);
    void ShowHardware();


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


    #ifdef USE_MODULE_NETWORK_WEBSERVER
      void WebCommand_Parse();
      int8_t Tasker_Web(uint8_t function);
      void WebAppend_JSON_RootPage_LiveviewPixels();
      void WebAppend_Root_ControlUI();
      void WebAppend_Root_Status_Table();
    #endif // USE_MODULE_NETWORK_WEBSERVER

};
#endif

#endif

