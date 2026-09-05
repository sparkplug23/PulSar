#ifndef _mHardwarePins_H_
#define _mHardwarePins_H_

#define   D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID 2005

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ESP8266
  #include "mPins_Esp82xx.h"
#endif
#ifdef ESP32
  #include "mPins_Esp32.h"
#endif
  
#include "jsmn.h"

#include "gpio_enums.h"
#include "gpio_bitpacked.h"

#include "2_CoreSystem/11_Languages/mLanguageDefault.h" // patch 3apr26


#define USE_MODULE_TEMPLATE_SONOFF_BASIC
#define USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
#define USE_MODULE_TEMPLATE_H801
#define USE_MODULE_TEMPLATE_MAGICHOME
#define USE_MODULE_TEMPLATE_SHELLY1
#define USE_MODULE_TEMPLATE_SHELLY_2P5
#define USE_MODULE_TEMPLATE_SHELLY_DIMMER2
#define USE_MODULE_TEMPLATE_SONOFF_BASIC_NODEMCU

#define USE_MODULE_TEMPLATE_DEFAULT_WEMOS
#define USE_MODULE_TEMPLATE_SONOFF_IFAN03
#define USE_MODULE_TEMPLATE_SONOFF_4CHPRO


DEFINE_PGM_CTR(PM_MODULE_NAME_NODEMCU_CTR)          D_MODULE_NAME_NODEMCU_CTR;
DEFINE_PGM_CTR(PM_MODULE_NAME_USERMODULE_CTR)          D_MODULE_NAME_USERMODULE_CTR;
#ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_BASIC_CTR)     D_MODULE_NAME_SONOFF_BASIC_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR)     D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_H801
  DEFINE_PGM_CTR(PM_MODULE_NAME_H801_CTR)             D_MODULE_NAME_H801_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_MAGICHOME
  DEFINE_PGM_CTR(PM_MODULE_NAME_MAGICHOME_CTR)        D_MODULE_NAME_MAGICHOME_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY1
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY1_CTR)     D_MODULE_NAME_SHELLY1_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY2P5_CTR)     D_MODULE_NAME_SHELLY2P5_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY_DIMMER2_CTR)     D_MODULE_NAME_SHELLY_DIMMER2_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_NODEMCU
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_BASIC_NODEMCU_CTR)     D_MODULE_NAME_SONOFF_BASIC_NODEMCU_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_4CHPRO_CTR)     D_MODULE_NAME_SONOFF_4CHPRO_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_IFAN03_CTR)     D_MODULE_NAME_SONOFF_IFAN03_CTR;
#endif


    #define GPIO_FLAG_ADC0_ID      1  // Allow ADC0 when define USE_ADC_VCC is disabled
    #define GPIO_FLAG_ADC0_TEMP    2  // Allow ADC0 as Temperature sensor when define USE_ADC_VCC is disabled
    


#include "1_TaskerManager/mTaskerInterface.h"

class mPins :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mPins(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init(void);
        
    static constexpr const char* PM_MODULE_CORE_HARDWAREPINS_CTR = D_MODULE__CORE__HARDWAREPINS__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_HARDWAREPINS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECITON: TEMPLATES Boards
     ************************************************************************************************/
    
    static const uint16_t GPIO_Function_EnabledList_IDs[] PROGMEM;

    /**
     * @brief This contains indexing for esp8266 THEN esp8285, the index (value) is important for retrieving template from either esp8266 or esp8285 arrays
     **/
    #ifdef ESP8266
      // Supported hardware modules. Leave completed list
      enum SupportedTemplatesESP8266 {
        MODULE_SONOFF_BASIC,
        MODULE_SONOFF_BASIC_EXTERNAL, // tmp solution until both fixed modules can also have extra gpio added on top of default templates eg adding key2 to basic
        MODULE_H801,
        MODULE_MAGICHOME,
        MODULE_SHELLY1,
        MODULE_SHELLY2P5,
        MODULE_SHELLY_DIMMER2,
        MODULE_SONOFF_BASIC_NODEMCU,
        MODULE_MAXMODULE_8266 // moved to dynamic ID list so it allows changing array size
      };
      static const mytmplt8266  module_template__gpio_map_ESP8266[] PROGMEM;

      enum SupportedTemplates82685 {
        MODULE_GENERIC = MODULE_MAXMODULE_8266, // Generic is esp8285 (and not esp8266 group) to enable pin 9/10 options
        MODULE_SONOFF_IFAN03,
        MODULE_SONOFF_4CHPRO,         
        MODULE_MAXMODULE_8285 
      };
      static const mytmplt8285  module_template__gpio_map_ESP8285[] PROGMEM;


      #define MODULE_DEFAULT mPins::MODULE_GENERIC // should become the WEMO in esp8285
      static const uint16_t module_template__ids[]      PROGMEM;
      static const char     module_template__names[]      PROGMEM;
      
      void GetInternalTemplate(void* ptr, uint32_t module, uint32_t option = 0);
    #endif

  



    #ifdef ESP32
    #if CONFIG_IDF_TARGET_ESP32C2
    /********************************************************************************************\
     * ESP32-C2 Module templates
    \********************************************************************************************/
    
    #define USER_MODULE        255
    
    // Supported hardware modules
    enum SupportedModulesESP32C2 {
      WEMOS, // not really correct, a placeholder for now
      MAXMODULE };
    
    // Default module settings
    const uint8_t kModuleNiceList[] PROGMEM = {
      WEMOS,
    };
    
    // !!! Update this list in the same order as kModuleNiceList !!!
    const char kModuleNames[] PROGMEM =
      "ESP32C2|"
      ;
    
    // !!! Update this list in the same order as SupportedModulesESP32C2 !!!
    const mytmplt kModules[] PROGMEM = {
      {                              // Generic ESP32C2 device
        GPIO_USER,              // 0       IO                  GPIO0, ADC1_CH0,  RTC
        GPIO_USER,              // 1       IO                  GPIO1, ADC1_CH1,  RTC
        GPIO_USER,              // 2       IO                  GPIO2, ADC1_CH2,  RTC
        GPIO_USER,              // 3       IO                  GPIO3, ADC1_CH3,  RTC 
        GPIO_USER,              // 4       IO                  GPIO4, ADC1_CH4,  RTC 
        GPIO_USER,              // 5       IO                  GPIO5, RTC 
        GPIO_USER,              // 6       IO                  GPIO6,
        GPIO_USER,              // 7       IO                  GPIO7, 
        GPIO_USER,              // 8       IO                  GPIO8, Strapping
        GPIO_USER,              // 9       IO                  GPIO9, Strapping
        GPIO_USER,              // 10      IO                  GPIO10
        0,                           // 11      IO                  GPIO11, output power supply for flash
        0,                           // 12      IO                  GPIO12, SPIHD - Free if flash DIO/DOUT
        0,                           // 13      IO                  GPIO13, SPIWP - Free if flash DIO/DOUT
        0,                           // 14      IO                  GPIO14, SPICS0
        0,                           // 15      IO                  GPIO15, SPICLK
        0,                           // 16      IO                  GPIO16, SPID
        0,                           // 17      IO                  GPIO17, SPIQ
        GPIO_USER,              // 18      IO                  GPIO18,
        GPIO_USER,              // 19      IO     RXD0         GPIO19, U0RXD
        GPIO_USER,              // 20      IO     TXD0         GPIO20, U0TXD
        0                            // Flag
      },
    };
    
    /*********************************************************************************************\
     Known templates
    \*********************************************************************************************/
    
    #elif CONFIG_IDF_TARGET_ESP32C3
    /********************************************************************************************\
     * ESP32-C3 Module templates
    \********************************************************************************************/
    
    #define USER_MODULE        255
    
    // Supported hardware modules
    enum SupportedModulesESP32C3 {
      MODULE_GENERIC,
      MAXMODULE };
    
    // // Default module settings
    // const uint8_t kModuleNiceList[] PROGMEM = {
    //   WEMOS,
    // };
    
    // // !!! Update this list in the same order as kModuleNiceList !!!
    // const char kModuleNames[] PROGMEM =
    //   "ESP32C3|"
    //   ;
    
    static const uint8_t module_template__ids[]  PROGMEM;
    static const char     module_template__names[]  PROGMEM;
    static const mytmplt  module_template__gpio_map[] PROGMEM;

    #define MODULE_DEFAULT mPins::MODULE_GENERIC // should become the WEMO in esp8285
    
    
    /*********************************************************************************************\
     Known templates
    \*********************************************************************************************/
    
    #elif CONFIG_IDF_TARGET_ESP32C6
    /********************************************************************************************\
     * ESP32-C6 Module templates
    \********************************************************************************************/
    
    #define USER_MODULE        255
    
    // Supported hardware modules
    enum SupportedModulesESP32C6 {
      WEMOS, // not really correct, a placeholder for now
      MAXMODULE };
    
    // Default module settings
    const uint8_t kModuleNiceList[] PROGMEM = {
      WEMOS,
    };
    
    // !!! Update this list in the same order as kModuleNiceList !!!
    const char kModuleNames[] PROGMEM =
      "ESP32C6|"
      ;
    
    // !!! Update this list in the same order as SupportedModulesESP32C6 !!!
    const mytmplt kModules[] PROGMEM = {
      {                              // Generic ESP32C6 device
        GPIO_USER,              // 0       IO                  GPIO0, ADC1_CH0, LP_GPIO0
        GPIO_USER,              // 1       IO                  GPIO1, ADC1_CH1, LP_GPIO1
        GPIO_USER,              // 2       IO                  GPIO2, ADC1_CH2, LP_GPIO2
        GPIO_USER,              // 3       IO                  GPIO3, ADC1_CH3, LP_GPIO3 
        GPIO_USER,              // 4       IO                  GPIO4, ADC1_CH4, LP_GPIO4, Strapping
        GPIO_USER,              // 5       IO                  GPIO5, ADC1_CH5, LP_GPIO5, Strapping
        GPIO_USER,              // 6       IO                  GPIO6, ADC1_CH6, LP_GPIO6
        GPIO_USER,              // 7       IO                  GPIO7,           LP_GPIO7
        GPIO_USER,              // 8       IO                  GPIO8, Strapping
        GPIO_USER,              // 9       IO                  GPIO9, Strapping
        GPIO_USER,              // 10      IO                  GPIO10 (QFN40 only)
        GPIO_USER,              // 11      IO                  GPIO11 (QFN40 only)
        GPIO_USER,              // 12      IO                  GPIO12, USB-JTAG
        GPIO_USER,              // 13      IO                  GPIO13, USB-JTAG
        GPIO_USER,              // 14      IO                  GPIO14 (QFN32 only)
        GPIO_USER,              // 15      IO                  GPIO15, Strapping
        GPIO_USER,              // 16      IO     TXD0         GPIO16, U0TXD
        GPIO_USER,              // 17      IO     RXD0         GPIO17, U0RXD
        GPIO_USER,              // 18      IO                  GPIO18, SDIO_CMD
        GPIO_USER,              // 19      IO                  GPIO19, SDIO_CLK
        GPIO_USER,              // 20      IO                  GPIO20, SDIO_DATA0
        GPIO_USER,              // 21      IO                  GPIO21, SDIO_DATA1
        GPIO_USER,              // 22      IO                  GPIO22, SDIO_DATA2
        GPIO_USER,              // 23      IO                  GPIO23, SDIO_DATA3
        0,                           // 24      IO                  GPIO24, SPICS1, PSRAM
        0,                           // 25      IO                  GPIO25, SPIQ
        0,                           // 26      IO                  GPIO26, SPIWP - Free if flash DIO/DOUT
        0,                           // 27      IO                  GPIO27, SPIVDD
        0,                           // 28      IO                  GPIO28, SPIHD - Free if flash DIO/DOUT
        0,                           // 29      IO                  GPIO29, SPICLK
        0,                           // 30      IO                  GPIO30, SPID
        0                            // Flag
      },
    };
    
    /*********************************************************************************************\
     Known templates
    \*********************************************************************************************/
    
    #elif CONFIG_IDF_TARGET_ESP32S2
    /********************************************************************************************\
     * ESP32-S2 Module templates
    \********************************************************************************************/
    
    #define USER_MODULE        255
    
    // Supported hardware modules
    enum SupportedModulesESP32S2 {
      WEMOS,
      MAXMODULE };
    
    // Default module settings
    const uint8_t kModuleNiceList[] PROGMEM = {
      WEMOS,
    };
    
    // !!! Update this list in the same order as kModuleNiceList !!!
    const char kModuleNames[] PROGMEM =
      "ESP32S2|"
      ;
    
    // !!! Update this list in the same order as SupportedModulesESP32S2 !!!
    const mytmplt kModules[] PROGMEM = {
      {                              // Generic ESP32C3 device
        GPIO_USER,              // 0       IO                  GPIO0, RTC_GPIO0, Strapping
        GPIO_USER,              // 1       AO                  GPIO1, ADC1_CH0, RTC_GPIO1
        GPIO_USER,              // 2       AO                  GPIO2, ADC1_CH1, RTC_GPIO2
        GPIO_USER,              // 3       AO                  GPIO3, ADC1_CH2, RTC_GPIO3
        GPIO_USER,              // 4       AO                  GPIO4, ADC1_CH3, RTC_GPIO4
        GPIO_USER,              // 5       AO                  GPIO5, ADC1_CH4, RTC_GPIO5
        GPIO_USER,              // 6       AO                  GPIO6, ADC1_CH5, RTC_GPIO6
        GPIO_USER,              // 7       AO                  GPIO7, ADC1_CH6, RTC_GPIO7
        GPIO_USER,              // 8       AO                  GPIO8, ADC1_CH7, RTC_GPIO8
        GPIO_USER,              // 9       AO                  GPIO9, ADC1_CH8, RTC_GPIO9
        GPIO_USER,              // 10      AO                  GPIO10, ADC1_CH9, RTC_GPIO10
        GPIO_USER,              // 11      AO                  GPIO11, ADC2_CH0, RTC_GPIO11
        GPIO_USER,              // 12      AO                  GPIO12, ADC2_CH1, RTC_GPIO12
        GPIO_USER,              // 13      AO                  GPIO13, ADC2_CH2, RTC_GPIO13
        GPIO_USER,              // 14      AO                  GPIO14, ADC2_CH3, RTC_GPIO14
        GPIO_USER,              // 15      AO                  GPIO15, ADC2_CH4, RTC_GPIO15, XTAL_32K_P
        GPIO_USER,              // 16      AO                  GPIO16, ADC2_CH5, RTC_GPIO16, XTAL_32K_N
        GPIO_USER,              // 17      AO                  GPIO17, ADC2_CH6, RTC_GPIO17, DAC_1
        GPIO_USER,              // 18      AO                  GPIO18, ADC2_CH7, RTC_GPIO18, DAC_2
        GPIO_USER,              // 19      AO                  GPIO19, ADC2_CH8, RTC_GPIO19
        GPIO_USER,              // 20      AO                  GPIO20, ADC2_CH9, RTC_GPIO20
        GPIO_USER,              // 21      IO                  GPIO21, RTC_GPIO21
                                 // 22      --                  Unused
                                 // 23      --                  Unused
                                 // 24      --                  Unused
                                 // 25      --                  Unused
                                 // 26      FL                  SPICS1, PSRAM
                                 // 27      FL                  SPIHD
                                 // 28      FL                  SPIWP
                                 // 29      FL                  SPICS0
                                 // 30      FL                  SPICLK
                                 // 31      FL                  SPIQ
                                 // 32      FL                  SPID
        GPIO_USER,              // 33      IO                  GPIO33
        GPIO_USER,              // 34      IO                  GPIO34
        GPIO_USER,              // 35      IO                  GPIO35
        GPIO_USER,              // 36      IO                  GPIO36
        GPIO_USER,              // 37      IO                  GPIO37
        GPIO_USER,              // 38      IO                  GPIO38
        GPIO_USER,              // 39      IO                  GPIO39, JTAG MTCK
        GPIO_USER,              // 40      IO                  GPIO40, JTAG MTDO
        GPIO_USER,              // 41      IO                  GPIO41, JTAG MTDI
        GPIO_USER,              // 42      IO                  GPIO42, JTAG MTMS
        GPIO_USER,              // 43      IO                  GPIO43, U0TXD
        GPIO_USER,              // 44      IO                  GPIO44, U0RXD
        GPIO_USER,              // 45      IO                  GPIO45, Strapping
        GPIO_USER,              // 46      I                   GPIO46, Input only, Strapping
        0                            // Flag
      },
    };
    
    /*********************************************************************************************\
     Known templates
    \*********************************************************************************************/
    
    #elif CONFIG_IDF_TARGET_ESP32S3
      /********************************************************************************************\
       * ESP32-S3 Module templates
      \********************************************************************************************/
      
      #define USER_MODULE        255
      
      enum SupportedModulesESP32S3 {
        MODULE_GENERIC,
        #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
        MODULE__CAMERA_XIAO_ESP32S3_SENSE__ID,
        #endif      
        MAXMODULE 
      };
      
      static const uint8_t module_template__ids[]  PROGMEM;
      static const char     module_template__names[]  PROGMEM;
      static const mytmplt  module_template__gpio_map[] PROGMEM;

      #define MODULE_DEFAULT mPins::MODULE_GENERIC // should become the WEMO in esp8285

    /*********************************************************************************************\
     Known templates
    \*********************************************************************************************/
    
    #else  // not CONFIG_IDF_TARGET_ESP32C2/C3/C6 nor CONFIG_IDF_TARGET_ESP32S2 - ESP32
      /********************************************************************************************\
       * ESP32 Module templates
      \********************************************************************************************/
      
      enum SupportedModules_ESP32_Base {
        MODULE_DOIT_DEVKIT,
        #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
        MODULE_CAM_AITHINKER_ID,
        #endif
        #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
        MODULE__CAMERA_FREENOVE_WROOVER__ID,
        #endif
        MAXMODULE 
      };

      #define USER_MODULE        255

      #define MODULE_DEFAULT mPins::MODULE_DOIT_DEVKIT
      static const uint16_t module_template__ids[]  PROGMEM;
      static const char     module_template__names[]  PROGMEM;
      static const mytmplt  module_template__gpio_map[] PROGMEM;
  
    #endif // standard esp32
  #endif //end esp32 all

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    uint32_t ModuleTemplate(uint32_t module);

    int8_t  GetRealPinNumberFromName(const char* c);
    int32_t GetGPIOFunctionIDbyName(const char* c);

    void ModuleSettings_ShowActiveTemplate();

    int16_t     GetModuleIDbyName(const char* c);
    const char* GetModuleNameByID(uint8_t id);

    const char* GetGPIOFunctionNamebyID(uint16_t id, char* B, uint8_t L);

    bool ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index);

    int16_t IRAM_ATTR Pin(uint32_t gpio, uint32_t index = 0);
    int16_t IRAM_ATTR GetPin(uint32_t gpio, uint32_t index = 0) { return Pin(gpio, index); }
    bool PinUsed(uint32_t gpio, uint32_t index = 0);

    // Stores a packed selected GPIO function ID, i.e. PGPIO(base_id) + index.
    void SetPin_GPIOFunction(uint32_t real_pin, uint16_t packed_gpio);

    bool GPIOBase_UsesZeroBasedSuffix(uint16_t base_id);

    bool CheckPhysicalPinIsFlashPin(uint32_t pin);

    void DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state);
    bool DigitalRead(uint32_t gpio_pin, uint32_t index = 0);

    uint8_t ModuleNr();
    bool ValidTemplateModule(uint8_t index);
    bool ValidModule(uint8_t index);

    const char* AnyModuleName(uint8_t index);
    const char* ModuleName();

    void GpioInit(void);
    void TemplateGPIOs(myio *gp);
    gpio_flag ModuleFlag();
    void ModuleDefault(uint8_t module);
    void SetModuleType();

    uint16_t ValidPin_AdjustGPIO(uint8_t real_pin, uint16_t gpio);
    bool ValidGPIO(uint8_t real_pin, uint16_t gpio);
    bool JsonTemplate(const char* dataBuf);

    void ModuleTemplate__ParseCJSONBuffer(char* buffer);

    int8_t TemplateIndexToRealPin(uint8_t template_index);

    bool flag_serial_set_tx_set = false;

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Pin Allocation / Physical Pin Model
     *
     * Purpose:
     *   1. PinPhysicalInfo represents every physical GPIO number for the compiled MCU.
     *   2. PinAllocation represents only user-configurable pins from gpio_pin_by_index[MAX_USER_PINS].
     *   3. pin_index_by_physical[MAX_GPIO_PIN] maps physical GPIO number back to allocation index.
     *
     * Notes:
     *   - This is intentionally not fully tied into boot/template config yet.
     *   - Init_InsertDummyPinAllocations() will initially populate useful demo allocations.
     *   - PinViewer will later read these records directly through helper accessors.
     ************************************************************************************************/

    struct PinPhysicalFlags
    {
      union
      {
        uint16_t data = 0;

        struct
        {

          uint16_t input_only        : 1;  // Hardware input-only, e.g. ESP32 GPIO34-39
          uint16_t output_only       : 1;  // Hardware output-only/unsafe-to-read, rare
          uint16_t input_capable     : 1;  // Can be used as digital input
          uint16_t output_capable    : 1;  // Can be used as digital output

          uint16_t pullup_capable    : 1;  // Internal pull-up supported/allowed
          uint16_t pulldown_capable  : 1;  // Internal pull-down supported/allowed

          uint16_t analog_capable    : 1;  // ADC/analog capable
          uint16_t pwm_capable       : 1;  // PWM/LEDC/RMT/software PWM capable
          uint16_t touch_capable     : 1;  // Touch capable where applicable

          uint16_t user_configurable : 1;  // Exposed to template/user GPIO configuration
          uint16_t boot_sensitive    : 1;  // Strap/boot-mode sensitive pin
          uint16_t flash_reserved    : 1;  // Flash/PSRAM/SPI flash related pin
          uint16_t usb_reserved      : 1;  // USB/JTAG/native debug related pin

          uint16_t reserved13        : 1;
          uint16_t reserved14        : 1;
          uint16_t reserved15        : 1;
        };
      };
    };

    struct PinAllocationFlags
    {
      union
      {
        uint16_t data = 0;

        struct
        {
          uint16_t allocated          : 1;  // Pin currently has assigned function/owner
          uint16_t locked             : 1;  // Cannot be changed without override
          uint16_t unavailable        : 1;  // Intentionally unavailable for allocation/use

          uint16_t grouped            : 1;  // Part of grouped function, I2C/SPI/UART/camera/RGB
          uint16_t shared             : 1;  // Bus-style shared ownership

          uint16_t inverted           : 1;  // Logical inverted behaviour
          uint16_t sensitive_to_probe : 1;  // Do not digitalRead/probe

          uint16_t conflict           : 1;

          uint16_t reserved8          : 1;
          uint16_t reserved9          : 1;
          uint16_t reserved10         : 1;
          uint16_t reserved11         : 1;
          uint16_t reserved12         : 1;
          uint16_t reserved13         : 1;
          uint16_t reserved14         : 1;
          uint16_t reserved15         : 1;
        };
      };
    };


    /************************************************************************************************
     * SECTION: Pin Runtime Data
     ************************************************************************************************/

    struct PinTable
    {
      PinPhysicalFlags physical;    // Hardware/MCU/board capability
      PinAllocationFlags allocation; // Runtime/template/module allocation state
      uint16_t gpio_function = GPIO_NONE;   // Packed GPIO function, or GPIO_NONE/GPIO_USER
      uint16_t unique_module_owner_id    = 0; // Primary owner, e.g. template, relay, camera, I2C0

      bool IsConfigured(void) const
      {
        return (gpio_function != GPIO_NONE) && (gpio_function != GPIO_USER);
      }

      bool IsAllocated(void) const
      {
        return allocation.allocated;
      }

      bool IsLocked(void) const
      {
        return allocation.locked;
      }

      bool HasConflict(void) const
      {
        return allocation.conflict;
      }

      bool IsUnavailable(void) const
      {
        return allocation.unavailable;
      }

      bool IsReserved(void) const
      {
        return allocation.unavailable ||
              physical.flash_reserved ||
              physical.usb_reserved ||
              physical.boot_sensitive;
      }

      bool IsUsable(void) const
      {
        if(allocation.unavailable) return false;
        if(physical.flash_reserved) return false;
        if(physical.usb_reserved) return false;
        return true;
      }

      uint16_t FunctionID(void) const
      {
        return gpio_function;
      }

      uint16_t OwnerID(void) const
      {
        return unique_module_owner_id;
      }

      uint16_t FunctionBaseID(void) const
      {
        if(!IsConfigured()) return 0;
        return UGPIO(gpio_function);
      }

      uint8_t FunctionIndex(void) const
      {
        if(!IsConfigured()) return 0;
        return gpio_function & GPIO_INDEX_MASK;
      }

      bool IsGrouped(void) const
      {
        return allocation.grouped;
      }

      bool IsShared(void) const
      {
        return allocation.shared;
      }

      uint8_t GroupID(void) const
      {
        if(!IsConfigured()) return 0;
        if(!allocation.grouped) return 0;
        return (uint8_t)UGPIO(gpio_function);
      }

      void Clear(void)
      {
        physical.data = 0;
        allocation.data = 0;
        gpio_function = GPIO_NONE;
        unique_module_owner_id = 0;
      }

      void SetFunction(uint16_t function_id)
      {
        gpio_function = function_id;
      }

      void SetOwner(uint16_t owner_id)
      {
        unique_module_owner_id = owner_id;
      }
    };
    PinTable pin[MAX_GPIO_PIN];

    /************************************************************************************************
     * SECTION: Pin Table Runtime
     ************************************************************************************************/

    void PinTable_InitSafeDefaults(void);
    bool SetPinOwnerIfAllowed(uint8_t real_pin, uint16_t new_owner_id);
    bool AllocatePin(uint8_t real_pin, uint16_t gpio_function, uint16_t owner_id, PinAllocationFlags flags);

    const char* PinTable_GetFunctionName(uint8_t real_pin, char* buffer, uint8_t buflen);
    const char* PinTable_GetOwnerName(uint8_t real_pin, char* buffer, uint8_t buflen);
    const char* PinTable_GetGroupName(uint8_t real_pin, char* buffer, uint8_t buflen);

    uint16_t GPIOPacked_Make(uint16_t base_id, uint8_t index);

    void PinTable_SerialPrint(const char* label = nullptr);
};

#endif 
