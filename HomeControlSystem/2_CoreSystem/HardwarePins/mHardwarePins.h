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
    
    
    void parse_JSONCommand(JsonParserObject obj);

    // ifdefesp32


    // ifdef esp8266

    // set max gpio

    // Once and for all, at the bottom of this header, define all GPIO pins on both hardware, their functions, and limitations.

/****** GPIO ESP82xx
 * ---------------------------------------------------------------------------------------------------------------------------------------
 * Index   |  GPIO   |  NodeMCU  |     Input      |     Output     |    Hardware    |   Notes
 * -------------------------------------------------------------------------------------------------------------------------------------
 * [0]     |    0    |    D3     |    pulled up   |       OK       |    D3          |   Connected to FLASH button, boot fails if pulled LOW
 * [1]     |    1    |    TX     |     TX pin	    |       OK       |    TXD0        |   HIGH at boot, debug output at boot, boot fails if pulled LOW
 * [2]     |    2    |    D4     |    pulled up   |       OK       |    TXD1        |   HIGH at boot, connected to on-board LED1, boot fails if pulled LOW
 * [3]     |    3    |    RX     |       OK	      |     RX pin     |    RXD0        |   HIGH at boot
 * [4]     |    4    |    D2     |       OK       |       OK       |    SDA         |   Serial0
 * [5]     |    5    |    D1     |       OK       |       OK       |    SCL         | 
 * [6]     |    9    |    SD2    |       ?        |        ?       |                |
 * [7]     |    10   |    SD3    |       ?        |        ?       |                |
 * [8]     |    12   |    D6     |       OK       |       OK       |    MISO        |          
 * [9]     |    13   |    D7     |       OK       |       OK       |    MOSI, RXD2  |    
 * [10]    |    14   |    D5     |       OK       |       OK       |    SCLK        | 
 * [11]    |    15   |    D8     |   pulled down  |       OK       |    CS,   TXD2  |   Boot fails if pulled HIGH
 * [12]    |    16   |    D0     |  no Interrupt  |   no PWM/I2C   |    Wake        |   HIGH at boot, used to wake up from deep sleep
 * [13]    |    A0   |    A0     |                |                |                |   10bit                                      
 * ******/

// #define MAX_GPIO_PIN       40   // Number of supported GPIO
// #define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
// #define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
// #define WEMOS_MODULE       0    // Wemos module

/****** GPIO ESP32
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Index   |  GPIO   | DoitDevKit|      Input      |     Output     |    Hardware                                                                   |   Notes
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * [00]    |    0   |            |    pulled up    |       OK       | ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK                           | outputs PWM signal at boot
 * [01]    |    1   |            |     TX pin	     |       OK       | U0TXD, CLK_OUT3, EMAC_RXD2                                                    | debug output at boot
 * [02]    |    2   |            |       OK        |       OK       | ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0                     |  connected to on-board LED
 * [03]    |    3   |            |       OK	       |     RX pin     | U0RXD, CLK_OUT2                                                               |  HIGH at boot
 * [04]    |    4   |            |       OK        |       OK       | ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER         |
 * [05]    |    5   |            |       OK        |       OK       | VSPICS0, HS1_DATA6, EMAC_RX_CLK                                               |  outputs PWM signal at boot
 * [--]    |    6   |      x     |      Flash      |      Flash     | Flash CLK                                                                     |  connected to the integrated SPI flash
 * [--]    |    7   |      x     |      Flash      |      Flash     | Flash D0                                                                      |  connected to the integrated SPI flash
 * [--]    |    8   |      x     |      Flash      |      Flash     | Flash D1                                                                      |  connected to the integrated SPI flash
 * [06]    |    9   |      *     |      Flash      |      Flash     | Flash D2, U1RXD                                                               |  connected to the integrated SPI flash
 * [07]    |    10  |      *     |      Flash      |      Flash     | Flash D3, U1TXD                                                               |  connected to the integrated SPI flash
 * [--]    |    11  |      x     |      Flash      |      Flash     | Flash CMD                                                                     |  connected to the integrated SPI flash
 * [08]    |    12  |            |       OK        |       OK       | ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
 * [09]    |    13  |            |       OK        |       OK       | ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER    |
 * [10]    |    14  |            |       OK        |       OK       | ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2       |  outputs PWM signal at boot
 * [11]    |    15  |            |       OK        |       OK       | ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3       |  outputs PWM signal at boot
 * [12]    |    16  |            |       OK        |       OK       | HS1_DATA4, U2RXD, EMAC_CLK_OUT                                                |
 * [13]    |    17  |            |       OK        |       OK       | HS1_DATA5, U2TXD, EMAC_CLK_OUT_180                                            |
 * [14]    |    18  |            |       OK        |       OK       | VSPICLK, HS1_DATA7                                                            |
 * [15]    |    19  |            |       OK        |       OK       | VSPIQ, U0CTS, EMAC_TXD0                                                       |
 * [16]    |    21  |            |       OK        |       OK       | VSPIHD, EMAC_TX_EN                                                            |
 * [17]    |    22  |            |       OK        |       OK       | VSPIWP, U0RTS, EMAC_TXD1                                                      |
 * [18]    |    23  |            |       OK        |       OK       | VSPID, HS1_STROBE                                                             |
 * [--]    |    24  |            |       --        |       --       |                                                                               |
 * [19]    |    25  |            |       OK        |       OK       | DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0                                         |
 * [20]    |    26  |            |       OK        |       OK       | DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1                                         |
 * [21]    |    27  |            |       OK        |       OK       | ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV                                      |
 * [22]    |    28  |            |       --        |       --       |                                                                               |
 * [22]    |    29  |            |       --        |       --       |                                                                               |
 * [22]    |    30  |            |       --        |       --       |                                                                               |
 * [22]    |    31  |            |       --        |       --       |                                                                               |
 * [22]    |    32  |            |       OK        |       OK       | XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9 |
 * [23]    |    33  |            |       OK        |       OK       | XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8|
 * [24]    |    34  |            |       OK        |    input only  | ADC1_CH6, RTC_GPIO4                                                           |  NO PULLUP
 * [25]    |    35  |            |       OK        |    input only  | ADC1_CH7, RTC_GPIO5                                                           |  NO PULLUP
 * [26]    |    36  |            |       OK        |    input only  | SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0                                         |  NO PULLUP
 * [--]    |    37  |            |       --        |       --       | NO PULLUP
 * [--]    |    38  |            |       --        |       --       | NO PULLUP
 * [27]    |    39  |            |       OK        |    input only  | SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3                                         |  NO PULLUP
                              
 * ******/

    
  /**
   * Instead of saving pins against their function indexed (ie index[rely1]=pin3)
   * Save the function (16 bit) against a premapped gpio number indexes
   */
  uint16_t pin_functions[30];  




    // uint8_t works = 0;

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
    void DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state);

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

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void pre_init(void);

};

#endif 
