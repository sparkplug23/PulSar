#include "mAnimatorLight.h"

/**
 * @brief Temporary file
 * 
 * Working out how to manage time/rate and intensity/speed
 * 
 * 
 * 
 * 
 * 
 * ManualTransitionMode
 *  - Setting TimeMs and RateMs directly
 * 
 * 
 * SimpleTransitionMode
 *  - Using Intensity/Speed to calculate TimeMs/RateMs when effects need it
 * 
 * 
 * Speed/Intensity functions should remain in segments struct for direct access, as they are not manipulated by time/rate
 * 
 * time/rate changes depending on mode, ??
 * 
 */

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS



#else

// void mAnimatorLight::CommandSet_Transition




      // uint8_t speed() //legacy for wled effects, made up from time/rate
      // {
      //   return transition.speed;
      // }
      // void set_speed(uint8_t v) //legacy for wled effects, made up from time/rate
      // {
      //   transition.speed = v;
      //   // return 127;
      // }

      // void set_intensity(uint8_t v) //legacy for wled effects, made up from time/rate
      // {
      //   transition.intensity_value = v;
      // }
      // uint8_t intensity() //legacy for wled effects, made up from time/rate
      // {
      //   return transition.intensity_value; // amount of animations in it
      // }




#endif // ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS

#endif //USE_MODULE_LIGHTS_ANIMATOR




