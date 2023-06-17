#ifndef _mHardwarePins_H_
#define _mHardwarePins_H_

#define   D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID 0

// MapConfig serves the purpose of feeding the template methods until successful saving of states are archieved
// In the near future, this mode will be tested by feeding the template via http command

#include "1_TaskerManager/mTaskerManager.h"

#include "../03_HardwareTemplates/mHardwareTemplates.h"

#ifdef ESP8266
  // #define DRX 3
  #include "mHardwarePins_Esp82xx.h"
#endif
#ifdef ESP32
  #include "mHardwarePins_Esp32.h"
#endif

#include "1_TaskerManager/mTaskerInterface.h"
  

  #include "jsmn.h"


#ifndef ENABLE_FEATURE_DEBUG_PINS // Nicely set them to nothing so they can be used inline without worring about ifdef gaurds each time

  // #define DEBUG_PIN1_GPIO     32
  #define DEBUG_PIN1_INIT()   //pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   //digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   //digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  // #define DEBUG_PIN2_GPIO     33
  #define DEBUG_PIN2_INIT()   //pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   //digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   //digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  // #define DEBUG_PIN3_GPIO     25
  #define DEBUG_PIN3_INIT()   //pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   //digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   //digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  // #define DEBUG_PIN4_GPIO     14
  #define DEBUG_PIN4_INIT()   //pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   //digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   //digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  // #define DEBUG_PIN5_GPIO     12
  #define DEBUG_PIN5_INIT()   //pinMode(DEBUG_PIN5_GPIO, OUTPUT); digitalWrite(DEBUG_PIN5_GPIO, HIGH);
  #define DEBUG_PIN5_SET(X)   //digitalWrite(DEBUG_PIN5_GPIO, X);
  #define DEBUG_PIN5_TOGGLE()   //digitalWrite(DEBUG_PIN5_GPIO, !digitalRead(DEBUG_PIN5_GPIO));

  // #define DEBUG_PIN6_GPIO     13
  #define DEBUG_PIN6_INIT()   //pinMode(DEBUG_PIN6_GPIO, OUTPUT); digitalWrite(DEBUG_PIN6_GPIO, HIGH);
  #define DEBUG_PIN6_SET(X)   //digitalWrite(DEBUG_PIN6_GPIO, X);
  #define DEBUG_PIN6_TOGGLE()   //digitalWrite(DEBUG_PIN6_GPIO, !digitalRead(DEBUG_PIN6_GPIO));

#endif // ENABLE_FEATURE_DEBUG_PINS


class mHardwarePins :
  public mTaskerInterface
{
  public:
    mHardwarePins(){}; //inline, no cpp needed
        
    static const char* PM_MODULE_CORE_HARDWAREPINS_CTR;
    static const char* PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_HARDWAREPINS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mHardwarePins);
    };
    #endif
    
    
    void parse_JSONCommand(JsonParserObject obj);

    
// #ifdef USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD


int8_t GetPinByIndex(uint8_t index);
int8_t GetPinIndexedLocation(uint8_t pin_number);
bool SetPinFunction(int8_t gpio_pin_number, int8_t pin_function);

// #endif // USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD

bool flag_serial_set_tx_set = false;

    
  /**
   * Module that is attached to the pin, indexed by ascending order of pins available on chip
   */
  uint16_t pin_attached_gpio_functions[MAX_USER_PINS] = {0};  



int8_t ConvertRealPinToIndexPin(uint8_t real_pin);

    // uint8_t works = 0;

    void ModuleSettings_FlashSerial();
    
// int jsoneq(const char *json, jsmntok_t *tok, const char *s);
    
    void ParseModuleTemplate();


    void ModuleSettings_ShowTemplateLog();

    void Template_Load();

    int8_t GetRealPinNumberFromName(const char* c);
    int16_t GetGPIOFunctionIDbyName(const char* c);

    void ModuleSettings_ShowActiveTemplate();
    
    const char* GetModuleNameByID(uint8_t id, char* buffer);

    int16_t GetModuleIDbyName(const char* c);
    
    bool UsuableGPIOPin(uint8_t pin);
    // int8_t UsablePinToTemplateArrayIndex(uint8_t pin);

    const char* GetGPIOFunctionNamebyID_P(uint16_t id, char* buffer);
    PGM_P GetGPIOFunctionNamebyID_P(uint16_t id);

    bool ValidUserGPIOFunction(uint8_t* pin_array, uint8_t index);
    bool ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index);


    int16_t IRAM_ATTR Pin(uint32_t gpio, uint32_t index = 0);
    boolean PinUsed(uint32_t gpio, uint32_t index = 0);
    void SetPin(uint32_t lpin, uint32_t gpio);


    int16_t GetPinWithGPIO(uint16_t gpio, uint8_t index = 0);
    uint32_t GetPin(uint32_t gpio, uint32_t index = 0);

    void DigitalWrite(uint32_t gpio_pin, uint32_t state);
    void DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state);

    uint8_t ModuleNr();
    bool ValidTemplateModule(uint8_t index);
    bool ValidModule(uint8_t index);
    const char* AnyModuleName2(uint8_t index);

    // int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;
    const char* AnyModuleName(uint8_t index, char* buffer, uint8_t buflen);

    const char* ModuleName(char* buffer, uint8_t buflen);
    void GpioInit(void);


    void TemplateGPIOs(myio *gp);
    // void TemplateGPIOs(myio *gp, uint8_t module_id);


    gpio_flag ModuleFlag();
    void ModuleDefault(uint8_t module);
    void SetModuleType();
    uint16_t ValidPin_AdjustGPIO(uint8_t pin, uint16_t gpio);
    bool ValidGPIO(uint8_t pin, uint16_t gpio);
    bool GetUsedInModule(uint8_t val, uint8_t *arr);
    bool JsonTemplate(const char* dataBuf);
    void TemplateJson();

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init(void);




    bool ReadModuleTemplateFromProgmem();
    void ModuleTemplateJsonParser(char* buffer);

    int8_t ConvertIndexPinToRealPin(uint8_t real_pin);









};

#endif 
