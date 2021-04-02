#ifndef _mHardwarePins_H_
#define _mHardwarePins_H_

#define   D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID 0

// MapConfig serves the purpose of feeding the template methods until successful saving of states are archieved
// In the near future, this mode will be tested by feeding the template via http command

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/mHardwareTemplates.h"

#ifdef ESP8266
  #define DRX 3
#endif

  #define   pCONT_pins                              static_cast<mHardwarePins*>(pCONT->pModule[EM_MODULE_CORE_HARDWAREPINS_ID])

#include "1_TaskerManager/mTaskerInterface.h"
  

  #include "jsmn.h"

class mHardwarePins :
  public mTaskerInterface
{
  public:
    mHardwarePins(){}; //inline, no cpp needed
        
    static const char* PM_MODULE_CORE_HARDWAREPINS_CTR;
    static const char* PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_HARDWAREPINS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mHardwarePins);
    };
    #endif

    uint8_t works = 0;

    void ModuleSettings_FlashSerial();
    
// int jsoneq(const char *json, jsmntok_t *tok, const char *s);
    
    void ParseModuleTemplate();

    void ReadModuleTemplateFromProgmem();

    void ModuleSettings_ShowTemplateLog();

    void Template_Load();

    int8_t GetGPIONumberFromName(const char* c);
    int16_t GetGPIOFunctionIDbyName(const char* c);

    void ModuleSettings_ShowActiveTemplate();
    
const char* GetModuleNameByID(uint8_t id, char* buffer);

    int16_t GetModuleIDbyName(const char* c);
    
    bool UsuableGPIOPin(uint8_t pin);
    int8_t UsablePinToTemplateArrayIndex(uint8_t pin);

    const char* GetGPIOFunctionNamebyID_P(uint8_t id, char* buffer);
    PGM_P GetGPIOFunctionNamebyID_P(uint8_t id);

bool ValidUserGPIOFunction(uint8_t* pin_array, uint8_t index);


uint32_t Pin(uint32_t gpio, uint32_t index = 0);
boolean PinUsed(uint32_t gpio, uint32_t index = 0);
void SetPin(uint32_t lpin, uint32_t gpio);

uint32_t GetPin(uint32_t gpio, uint32_t index = 0);


    void DigitalWrite(uint32_t gpio_pin, uint32_t state);

    uint8_t ModuleNr();
    bool ValidTemplateModule(uint8_t index);
    bool ValidModule(uint8_t index);
    const char* AnyModuleName2(uint8_t index);
    

int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;
    const char* AnyModuleName(uint8_t index, char* buffer, uint8_t buflen);

    const char* ModuleName(char* buffer, uint8_t buflen);
    void GpioInit(void);
    void ModuleGpios(myio *gp);
    void ModuleGpios(myio *gp, uint8_t module_id);
    gpio_flag ModuleFlag();
    void ModuleDefault(uint8_t module);
    void SetModuleType();
    uint8_t ValidPin(uint8_t pin, uint8_t gpio);
    bool ValidGPIO(uint8_t pin, uint8_t gpio);
    bool GetUsedInModule(uint8_t val, uint8_t *arr);
    bool JsonTemplate(const char* dataBuf);
    void TemplateJson();

    int8_t Tasker(uint8_t function);
    void pre_init(void);

};

#endif 
