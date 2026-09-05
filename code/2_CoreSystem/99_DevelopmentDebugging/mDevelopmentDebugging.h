#ifndef DEVELOPMENT_DEBUGGING_H
#define DEVELOPMENT_DEBUGGING_H 0.21
  
#include <Arduino.h>

#define D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID 2099 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"


/**
 * @brief 
 * 
 * 
 * Create new method here, look into how WLED "locks"/assigns pins.
 * I want a way here to define nicely what the pin is doing (Which module is attached, what function is it, ie digital, I2C, serial)
 * Then show which pins are available for use
 * make sure to clasify the limitations on a pin too with a "special note" mqtt message against the pin. 
 * 
 * tasmota gets around this by forcing restart after pin changes.
 * 
 */


 #ifdef DEBUG_PIN1_GPIO
 #define DEBUG_PIN1_INIT()     pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
 #define DEBUG_PIN1_SET(X)     digitalWrite(DEBUG_PIN1_GPIO, X);
 #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));
#else
 #define DEBUG_PIN1_INIT()
 #define DEBUG_PIN1_SET(X)
 #define DEBUG_PIN1_TOGGLE()
#endif
#ifdef DEBUG_PIN2_GPIO
 #define DEBUG_PIN2_INIT()     pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
 #define DEBUG_PIN2_SET(X)     digitalWrite(DEBUG_PIN2_GPIO, X);
 #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
#else
 #define DEBUG_PIN2_INIT()

 #define DEBUG_PIN2_SET(X)
 #define DEBUG_PIN2_TOGGLE()
#endif
#ifdef DEBUG_PIN3_GPIO
 #define DEBUG_PIN3_INIT()     pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
 #define DEBUG_PIN3_SET(X)     digitalWrite(DEBUG_PIN3_GPIO, X);
 #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));
#else 
 #define DEBUG_PIN3_INIT()
 #define DEBUG_PIN3_SET(X)
 #define DEBUG_PIN3_TOGGLE()
#endif
#ifdef DEBUG_PIN4_GPIO
 #define DEBUG_PIN4_INIT()     pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
 #define DEBUG_PIN4_SET(X)     digitalWrite(DEBUG_PIN4_GPIO, X);
 #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));
#else
 #define DEBUG_PIN4_INIT()
 #define DEBUG_PIN4_SET(X)
 #define DEBUG_PIN4_TOGGLE()
#endif
#ifdef DEBUG_PIN5_GPIO
 #define DEBUG_PIN5_INIT()     pinMode(DEBUG_PIN5_GPIO, OUTPUT); digitalWrite(DEBUG_PIN5_GPIO, HIGH);
 #define DEBUG_PIN5_SET(X)     digitalWrite(DEBUG_PIN5_GPIO, X);
 #define DEBUG_PIN5_TOGGLE()   digitalWrite(DEBUG_PIN5_GPIO, !digitalRead(DEBUG_PIN5_GPIO));
#else
 #define DEBUG_PIN5_INIT()
 #define DEBUG_PIN5_SET(X)
 #define DEBUG_PIN5_TOGGLE()
#endif
#ifdef DEBUG_PIN6_GPIO
 #define DEBUG_PIN6_INIT()     pinMode(DEBUG_PIN6_GPIO, OUTPUT); digitalWrite(DEBUG_PIN6_GPIO, HIGH);
 #define DEBUG_PIN6_SET(X)     digitalWrite(DEBUG_PIN6_GPIO, X);
 #define DEBUG_PIN6_TOGGLE()   digitalWrite(DEBUG_PIN6_GPIO, !digitalRead(DEBUG_PIN6_GPIO));
#else
 #define DEBUG_PIN6_INIT()
 #define DEBUG_PIN6_SET(X)
 #define DEBUG_PIN6_TOGGLE()
#endif
#ifdef DEBUG_PIN7_GPIO
 #define DEBUG_PIN7_INIT()     pinMode(DEBUG_PIN7_GPIO, OUTPUT); digitalWrite(DEBUG_PIN7_GPIO, HIGH);
 #define DEBUG_PIN7_SET(X)     digitalWrite(DEBUG_PIN7_GPIO, X);
 #define DEBUG_PIN7_TOGGLE()   digitalWrite(DEBUG_PIN7_GPIO, !digitalRead(DEBUG_PIN7_GPIO));
#else
 #define DEBUG_PIN7_INIT()
 #define DEBUG_PIN7_SET(X)
 #define DEBUG_PIN7_TOGGLE()
#endif
#ifdef DEBUG_PIN8_GPIO
 #define DEBUG_PIN8_INIT()     pinMode(DEBUG_PIN8_GPIO, OUTPUT); digitalWrite(DEBUG_PIN8_GPIO, HIGH);
 #define DEBUG_PIN8_SET(X)     digitalWrite(DEBUG_PIN8_GPIO, X);
 #define DEBUG_PIN8_TOGGLE()   digitalWrite(DEBUG_PIN8_GPIO, !digitalRead(DEBUG_PIN8_GPIO));
#else
 #define DEBUG_PIN8_INIT()
 #define DEBUG_PIN8_SET(X)
 #define DEBUG_PIN8_TOGGLE()
#endif


class mDevelopmentDebugging :
  public mTaskerInterface
{

  private:
  public:
    mDevelopmentDebugging(){};
    void init(void);

    struct DEBUG_DATA
    {
      float input_float1 = 0;
    }debug_data;
    void parse_JSONCommand(JsonParserObject obj);

    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    void Init_DebugPin();

    static constexpr const char* PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR = D_MODULE__CORE__DEVELOPMENT_DEBUGGING__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID; }
    
    void SubTask_Show_Defines_Ready_To_Phase_Out();

};
#endif
