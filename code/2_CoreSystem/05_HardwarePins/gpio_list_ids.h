#ifndef _mPin_GPIO_H_
#define _mPin_GPIO_H_

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/11_Languages/mLanguageDefault.h" // patch 3apr26

#include "gpio_progmem.h"

/*********************************************************************************************\
 * GPIO Function Group Packing
 *
 * PulSar GPIO functions are normally stored as enum values, for example:
 *
 *   GPIO_FUNCTION__MODEM_RX
 *   GPIO_FUNCTION__MODEM_TX
 *   GPIO_FUNCTION__I2C_SCL
 *   GPIO_FUNCTION__I2C_SDA
 *
 * Some GPIO functions are repeatable. Examples:
 *
 *   MODEM_RX  -> MODEM_RX, MODEM_RX2, MODEM_RX3
 *   MODEM_TX  -> MODEM_TX, MODEM_TX2, MODEM_TX3
 *   I2C_SCL   -> I2C_SCL,  I2C_SCL2
 *   I2C_SDA   -> I2C_SDA,  I2C_SDA2
 *
 * For repeatable groups, the enum values must be arranged contiguously:
 *
 *   GPIO_FUNCTION__MODEM_RX,     // index 0
 *   GPIO_FUNCTION__MODEM_RX2,    // index 1
 *   GPIO_FUNCTION__MODEM_RX3,    // index 2
 *
 *   GPIO_FUNCTION__MODEM_TX,     // index 0
 *   GPIO_FUNCTION__MODEM_TX2,    // index 1
 *   GPIO_FUNCTION__MODEM_TX3,    // index 2
 *
 * This allows code to resolve a specific instance using:
 *
 *   real_gpio = GPIO_FUNCTION__MODEM_RX + index;
 *
 * The packing macros below are used for GPIO function list / WebUI / naming expansion,
 * where one base enum entry can describe several indexed variants.
 *
 * Packed list format:
 *
 *   [ upper bits: GPIO base enum ][ lower 5 bits: max count - 1 ]
 *
 * Example:
 *
 *   PGPIO(GPIO_FUNCTION__MODEM_RX) + MGPIO(3)
 *
 * means:
 *
 *   base function : GPIO_FUNCTION__MODEM_RX
 *   variants      : 3
 *   expands to    : GPIO_FUNCTION__MODEM_RX + 0
 *                   GPIO_FUNCTION__MODEM_RX + 1
 *                   GPIO_FUNCTION__MODEM_RX + 2
 *
 * The lower 5 bits are metadata for the number of available indexed variants.
 * They are not the selected instance index. The selected instance is still derived
 * from base + index during lookup.
 *
 * Current staged migration model:
 *
 *   - PGPIO/MGPIO are used for grouped GPIO list entries.
 *   - Stored pin assignments may still use raw enum values.
 *   - Full packed storage can be migrated later once enough groups have been converted.
 *
 * Limits:
 *
 *   GPIO_INDEX_BITS = 5 gives 0..31 metadata values.
 *   Since MGPIO stores count - 1, this supports 1..32 variants per group.
\*********************************************************************************************/

#define GPIO_INDEX_BITS  5
#define GPIO_INDEX_MASK  0x001F
#define GPIO_BASE_MASK   0xFFE0

#define PGPIO(x)         ((uint16_t)((x) << GPIO_INDEX_BITS))          // Pack GPIO base/function
#define UGPIO(x)         ((uint16_t)((x) >> GPIO_INDEX_BITS))          // Unpack GPIO base/function
#define MGPIO(x)         ((uint16_t)((x) ? ((x) - 1) : 0))             // Encode max/count metadata as count-1
















#define GPIO_ANY           32   // Any GPIO

// Group by the interface they mostly use? i2c/serial etc?

/**
 * @brief Fixed GPIO function ID list.
 * @note This list is intentionally complete and static across builds/hardware.
 * @note Do not add #ifdef inside this enum. Feature filtering belongs in nicelist/metadata layers.
 * @note INV = inverted active level, low/ground is active.
 * @note NP  = no internal pullup/pulldown.
 * @note PD  = pulled down, hardware dependent.
 */
enum GPIO_COMPLETE_STATIC_LIST_IDS {

  /****************************************
   * SECTION: Unassigned pin states
   ****************************************/

  GPIO_NONE,
  GPIO_UNUSED_FORCED_HIGH,
  GPIO_UNUSED_FORCED_LOW,


  /****************************************
   ****************************************
   ****************************************
   * SECTION: Core System
   ****************************************
   ****************************************
   ****************************************/

      /****************************************
       ****************************************
      * SECTION: Filesystem + SD Card 
      ****************************************
      ****************************************/
        
        GPIO_FUNCTION_SDCARD_VSPI_CSO, GPIO_FUNCTION_SDCARD_VSPI_CLK, 
        GPIO_FUNCTION_SDCARD_VSPI_MOSI, GPIO_FUNCTION_SDCARD_VSPI_MISO,
        
        GPIO_FUNCTION_SDCARD_HSPI_CSO, GPIO_FUNCTION_SDCARD_HSPI_CLK, 
        GPIO_FUNCTION_SDCARD_HSPI_MOSI, GPIO_FUNCTION_SDCARD_HSPI_MISO,

        GPIO_FUNCTION_SDCARD_MMC_CMD, GPIO_FUNCTION_SDCARD_MMC_CLK,
        GPIO_FUNCTION_SDCARD_MMC_D0, GPIO_FUNCTION_SDCARD_MMC_D1, GPIO_FUNCTION_SDCARD_MMC_D2, GPIO_FUNCTION_SDCARD_MMC_D3,

      /****************************************
       ****************************************
      * SECTION: UART 
      ****************************************
      ****************************************/

          /****************************************
           * UART: Generic hardware serial
           ****************************************/
          GPIO_HWSERIAL0_TX, GPIO_HWSERIAL0_RX,
          GPIO_HWSERIAL1_TX, GPIO_HWSERIAL1_RX,
          GPIO_HWSERIAL2_TX, GPIO_HWSERIAL2_RX,

          /****************************************
           * UART: Generic software serial
           ****************************************/
          GPIO_SWSERIAL0_TX, GPIO_SWSERIAL0_RX,

          /****************************************
           * UART: Ring-buffer serial modes (these are a type of HWSERIAL incoming stream, and likely should be removed as its own GPIO set, and instead have a switch with "GetData()" abstraction layer)
           ****************************************/ 
          GPIO_HWSERIAL0_RING_BUFFER_TX, GPIO_HWSERIAL0_RING_BUFFER_RX,
          GPIO_HWSERIAL1_RING_BUFFER_TX, GPIO_HWSERIAL1_RING_BUFFER_RX,
          GPIO_HWSERIAL2_RING_BUFFER_TX, GPIO_HWSERIAL2_RING_BUFFER_RX,

          /****************************************
           * UART: Debug serial (Really also needs it own debug stream abstraction layer, but for now just a fixed GPIO set)
           ****************************************/
          GPIO_SERIAL_DEBUG_TX, GPIO_SERIAL_DEBUG_RX,

          /****************************************
           * UART: Serial bridge
           ****************************************/
          GPIO_SBR_TX, GPIO_SBR_RX,
    
      /****************************************
       ****************************************
      * SECTION: I2C 
      ****************************************
      ****************************************/
      
      // Port I2C1
      GPIO_I2C_SCL, GPIO_I2C_SDA,
      // Port I2C2
      GPIO_I2C_SCL2, GPIO_I2C_SDA2,

      /****************************************
       ****************************************
      * SECTION: SPI 
      ****************************************
      ****************************************/

      // Hardware SPI
      GPIO_SPI_CS, GPIO_SPI_DC,
      // Software SPI
      GPIO_SSPI_MISO, GPIO_SSPI_MOSI, GPIO_SSPI_SCLK, GPIO_SSPI_CS, GPIO_SSPI_DC,

      /****************************************
       ****************************************
      * SECTION: ADC Internal (not external boards) 
      ****************************************
      ****************************************/

        GPIO_ADC0_TEMPERATURE,
        GPIO_ADC1_CH4,  // Should probably simple switch to 0 to X, and then have the esp32 map that internally to the actual channel, since that is how the API works, and it would be more portable across different hardware with different numbers of channels
        GPIO_ADC1_CH6,
        GPIO_ADC1_CH7,
        GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER,

      /****************************************
       ****************************************
      * SECTION: PWM 
      ****************************************
      ****************************************/
            
        GPIO_PWM1, GPIO_PWM2, GPIO_PWM3, GPIO_PWM4, GPIO_PWM5,
        GPIO_PWM1_INV, GPIO_PWM2_INV, GPIO_PWM3_INV, GPIO_PWM4_INV, GPIO_PWM5_INV,


  /****************************************
   ****************************************
   ****************************************
   * SECTION: Drivers
   ****************************************
   ****************************************
   ****************************************/

      /****************************************
       ****************************************
      * SECTION: LED 
      ****************************************
      ****************************************/
     
        GPIO_LED1,     GPIO_LED2,     GPIO_LED3,     GPIO_LED4,     GPIO_LED5,     GPIO_LED6,     GPIO_LED7,     GPIO_LED8,
        GPIO_LED1_INV, GPIO_LED2_INV, GPIO_LED3_INV, GPIO_LED4_INV, GPIO_LED5_INV, GPIO_LED6_INV, GPIO_LED7_INV, GPIO_LED8_INV,

      /****************************************
       ****************************************
      * SECTION: Relay 
      ****************************************
      ****************************************/
     
        GPIO_REL1, GPIO_REL2, GPIO_REL3, GPIO_REL4, GPIO_REL5, GPIO_REL6, GPIO_REL7, GPIO_REL8,
        GPIO_REL1_INV, GPIO_REL2_INV, GPIO_REL3_INV, GPIO_REL4_INV, GPIO_REL5_INV, GPIO_REL6_INV, GPIO_REL7_INV, GPIO_REL8_INV,

      /****************************************
       ****************************************
      * SECTION: IR 
      ****************************************
      ****************************************/
     
        GPIO_IRSEND, GPIO_IRRECV,

      /****************************************
       ****************************************
      * SECTION: Servos 
      ****************************************
      ****************************************/
     
        /****************************************
         * INPUT: X
         ****************************************/

      /****************************************
       ****************************************
      * SECTION: SAW RF 
      ****************************************
      ****************************************/
          
        GPIO_RFSEND, GPIO_RFRECV,
        GPIO_RF_433MHZ_RX, GPIO_RF_433MHZ_TX,

      /****************************************
       ****************************************
      * SECTION: HBridge/Motor 9110 
      ****************************************
      ****************************************/
     
        GPIO_HBRIDGE_L9110_IA, GPIO_HBRIDGE_L9110_IB, GPIO_HBRIDGE_L9110_OA, GPIO_HBRIDGE_L9110_OB,

      /****************************************
       ****************************************
      * SECTION: Shelly Dimmer 
      ****************************************
      ****************************************/
     
        GPIO_SHELLY2_SHD_BOOT0, GPIO_SHELLY2_SHD_RESET_INV,

      /****************************************
       ****************************************
      * SECTION: Buzzer 
      ****************************************
      ****************************************/
     
        GPIO_BUZZER, GPIO_BUZZER_INV,

      /****************************************
       ****************************************
      * SECTION: Cellular Modem 
      ****************************************
      ****************************************/
     
        GPIO_FUNCTION__MODEM_RX0, GPIO_FUNCTION__MODEM_TX0, 
        GPIO_FUNCTION__MODEM_RX1, GPIO_FUNCTION__MODEM_TX1, 
        GPIO_FUNCTION__MODEM_RX2, GPIO_FUNCTION__MODEM_TX2,
        



        GPIO_FUNCTION__MODEM_POWER_KEY,
        GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR,
        GPIO_FUNCTION__MODEM_RING_INDICATOR,
        GPIO_FUNCTION__MODEM_POWER_STATUS,
        GPIO_FUNCTION__MODEM_NETWORK_STATUS,

      /****************************************
       ****************************************
      * SECTION: Cameras 
      ****************************************
      ****************************************/

        GPIO_WEBCAM_XCLK,
        GPIO_WEBCAM_DATA1, GPIO_WEBCAM_DATA2, GPIO_WEBCAM_DATA3, GPIO_WEBCAM_DATA4,GPIO_WEBCAM_DATA5, GPIO_WEBCAM_DATA6, GPIO_WEBCAM_DATA7, GPIO_WEBCAM_DATA8,
        GPIO_WEBCAM_PCLK,
        GPIO_WEBCAM_HREF,
        GPIO_WEBCAM_VSYNC,
        GPIO_WEBCAM_SIOD,
        GPIO_WEBCAM_SIOC,
        GPIO_WEBCAM_PWDN,
        GPIO_WEBCAM_RESET,

      /****************************************
       ****************************************
      * SECTION: MAVLink 
      ****************************************
      ****************************************/
     
        /****************************************
         * INPUT: X
         ****************************************/



  /****************************************
   ****************************************
   ****************************************
   * SECTION: Sensors
   ****************************************
   ****************************************
   ****************************************/

      /****************************************
       ****************************************
      * SECTION: Switches 
      ****************************************
      ****************************************/

        /****************************************
         * INPUT: Switches, normal
         ****************************************/
        GPIO_SWT1, GPIO_SWT2, GPIO_SWT3, GPIO_SWT4, GPIO_SWT5, GPIO_SWT6, GPIO_SWT7, GPIO_SWT8,
        GPIO_SWT9, GPIO_SWT10, GPIO_SWT11, GPIO_SWT12, GPIO_SWT13, GPIO_SWT14, GPIO_SWT15, GPIO_SWT16,
        GPIO_SWT17, GPIO_SWT18, GPIO_SWT19, GPIO_SWT20, GPIO_SWT21, GPIO_SWT22, GPIO_SWT23, GPIO_SWT24,
        GPIO_SWT25, GPIO_SWT26, GPIO_SWT27, GPIO_SWT28, GPIO_SWT29, GPIO_SWT30, GPIO_SWT31, GPIO_SWT32,

        /****************************************
         * INPUT: Switches, inverted
         ****************************************/
        GPIO_SWT1_INV, GPIO_SWT2_INV, GPIO_SWT3_INV, GPIO_SWT4_INV, GPIO_SWT5_INV, GPIO_SWT6_INV, GPIO_SWT7_INV, GPIO_SWT8_INV,
        GPIO_SWT9_INV, GPIO_SWT10_INV, GPIO_SWT11_INV, GPIO_SWT12_INV, GPIO_SWT13_INV, GPIO_SWT14_INV, GPIO_SWT15_INV, GPIO_SWT16_INV,
        GPIO_SWT17_INV, GPIO_SWT18_INV, GPIO_SWT19_INV, GPIO_SWT20_INV, GPIO_SWT21_INV, GPIO_SWT22_INV, GPIO_SWT23_INV, GPIO_SWT24_INV,
        GPIO_SWT25_INV, GPIO_SWT26_INV, GPIO_SWT27_INV, GPIO_SWT28_INV, GPIO_SWT29_INV, GPIO_SWT30_INV, GPIO_SWT31_INV, GPIO_SWT32_INV,

        /****************************************
         * INPUT: Switches, no pull
         ****************************************/
        GPIO_SWT1_NP, GPIO_SWT2_NP, GPIO_SWT3_NP, GPIO_SWT4_NP, GPIO_SWT5_NP, GPIO_SWT6_NP, GPIO_SWT7_NP, GPIO_SWT8_NP,
        GPIO_SWT9_NP, GPIO_SWT10_NP, GPIO_SWT11_NP, GPIO_SWT12_NP, GPIO_SWT13_NP, GPIO_SWT14_NP, GPIO_SWT15_NP, GPIO_SWT16_NP,
        GPIO_SWT17_NP, GPIO_SWT18_NP, GPIO_SWT19_NP, GPIO_SWT20_NP, GPIO_SWT21_NP, GPIO_SWT22_NP, GPIO_SWT23_NP, GPIO_SWT24_NP,
        GPIO_SWT25_NP, GPIO_SWT26_NP, GPIO_SWT27_NP, GPIO_SWT28_NP, GPIO_SWT29_NP, GPIO_SWT30_NP, GPIO_SWT31_NP, GPIO_SWT32_NP,

        /****************************************
         * INPUT: Switches, inverted no pull
         ****************************************/
        GPIO_SWT1_INV_NP, GPIO_SWT2_INV_NP, GPIO_SWT3_INV_NP, GPIO_SWT4_INV_NP, GPIO_SWT5_INV_NP, GPIO_SWT6_INV_NP, GPIO_SWT7_INV_NP, GPIO_SWT8_INV_NP,
        GPIO_SWT9_INV_NP, GPIO_SWT10_INV_NP, GPIO_SWT11_INV_NP, GPIO_SWT12_INV_NP, GPIO_SWT13_INV_NP, GPIO_SWT14_INV_NP, GPIO_SWT15_INV_NP, GPIO_SWT16_INV_NP,
        GPIO_SWT17_INV_NP, GPIO_SWT18_INV_NP, GPIO_SWT19_INV_NP, GPIO_SWT20_INV_NP, GPIO_SWT21_INV_NP, GPIO_SWT22_INV_NP, GPIO_SWT23_INV_NP, GPIO_SWT24_INV_NP,
        GPIO_SWT25_INV_NP, GPIO_SWT26_INV_NP, GPIO_SWT27_INV_NP, GPIO_SWT28_INV_NP, GPIO_SWT29_INV_NP, GPIO_SWT30_INV_NP, GPIO_SWT31_INV_NP, GPIO_SWT32_INV_NP,

      /****************************************
       ****************************************
      * SECTION: Buttons 
      ****************************************
      ****************************************/
            
        /****************************************
         * INPUT: Buttons, normal
         ****************************************/
        GPIO_KEY1, GPIO_KEY2, GPIO_KEY3, GPIO_KEY4, GPIO_KEY5, GPIO_KEY6, GPIO_KEY7, GPIO_KEY8,
        GPIO_KEY9, GPIO_KEY10, GPIO_KEY11, GPIO_KEY12, GPIO_KEY13, GPIO_KEY14, GPIO_KEY15, GPIO_KEY16,
        GPIO_KEY17, GPIO_KEY18, GPIO_KEY19, GPIO_KEY20, GPIO_KEY21, GPIO_KEY22, GPIO_KEY23, GPIO_KEY24,
        GPIO_KEY25, GPIO_KEY26, GPIO_KEY27, GPIO_KEY28, GPIO_KEY29, GPIO_KEY30, GPIO_KEY31, GPIO_KEY32,

        /****************************************
         * INPUT: Buttons, inverted
         ****************************************/
        GPIO_KEY1_INV, GPIO_KEY2_INV, GPIO_KEY3_INV, GPIO_KEY4_INV, GPIO_KEY5_INV, GPIO_KEY6_INV, GPIO_KEY7_INV, GPIO_KEY8_INV,
        GPIO_KEY9_INV, GPIO_KEY10_INV, GPIO_KEY11_INV, GPIO_KEY12_INV, GPIO_KEY13_INV, GPIO_KEY14_INV, GPIO_KEY15_INV, GPIO_KEY16_INV,
        GPIO_KEY17_INV, GPIO_KEY18_INV, GPIO_KEY19_INV, GPIO_KEY20_INV, GPIO_KEY21_INV, GPIO_KEY22_INV, GPIO_KEY23_INV, GPIO_KEY24_INV,
        GPIO_KEY25_INV, GPIO_KEY26_INV, GPIO_KEY27_INV, GPIO_KEY28_INV, GPIO_KEY29_INV, GPIO_KEY30_INV, GPIO_KEY31_INV, GPIO_KEY32_INV,

        /****************************************
         * INPUT: Buttons, no pull
         ****************************************/
        GPIO_KEY1_NP, GPIO_KEY2_NP, GPIO_KEY3_NP, GPIO_KEY4_NP, GPIO_KEY5_NP, GPIO_KEY6_NP, GPIO_KEY7_NP, GPIO_KEY8_NP,
        GPIO_KEY9_NP, GPIO_KEY10_NP, GPIO_KEY11_NP, GPIO_KEY12_NP, GPIO_KEY13_NP, GPIO_KEY14_NP, GPIO_KEY15_NP, GPIO_KEY16_NP,
        GPIO_KEY17_NP, GPIO_KEY18_NP, GPIO_KEY19_NP, GPIO_KEY20_NP, GPIO_KEY21_NP, GPIO_KEY22_NP, GPIO_KEY23_NP, GPIO_KEY24_NP,
        GPIO_KEY25_NP, GPIO_KEY26_NP, GPIO_KEY27_NP, GPIO_KEY28_NP, GPIO_KEY29_NP, GPIO_KEY30_NP, GPIO_KEY31_NP, GPIO_KEY32_NP,

        /****************************************
         * INPUT: Buttons, inverted no pull
         ****************************************/
        GPIO_KEY1_INV_NP, GPIO_KEY2_INV_NP, GPIO_KEY3_INV_NP, GPIO_KEY4_INV_NP, GPIO_KEY5_INV_NP, GPIO_KEY6_INV_NP, GPIO_KEY7_INV_NP, GPIO_KEY8_INV_NP,
        GPIO_KEY9_INV_NP, GPIO_KEY10_INV_NP, GPIO_KEY11_INV_NP, GPIO_KEY12_INV_NP, GPIO_KEY13_INV_NP, GPIO_KEY14_INV_NP, GPIO_KEY15_INV_NP, GPIO_KEY16_INV_NP,
        GPIO_KEY17_INV_NP, GPIO_KEY18_INV_NP, GPIO_KEY19_INV_NP, GPIO_KEY20_INV_NP, GPIO_KEY21_INV_NP, GPIO_KEY22_INV_NP, GPIO_KEY23_INV_NP, GPIO_KEY24_INV_NP,
        GPIO_KEY25_INV_NP, GPIO_KEY26_INV_NP, GPIO_KEY27_INV_NP, GPIO_KEY28_INV_NP, GPIO_KEY29_INV_NP, GPIO_KEY30_INV_NP, GPIO_KEY31_INV_NP, GPIO_KEY32_INV_NP,

        /****************************************
         * INPUT: Buttons, touch
         ****************************************/
        GPIO_KEY1_TOUCH, GPIO_KEY2_TOUCH, GPIO_KEY3_TOUCH, GPIO_KEY4_TOUCH, GPIO_KEY5_TOUCH, GPIO_KEY6_TOUCH, GPIO_KEY7_TOUCH, GPIO_KEY8_TOUCH,
        GPIO_KEY9_TOUCH, GPIO_KEY10_TOUCH, GPIO_KEY11_TOUCH, GPIO_KEY12_TOUCH, GPIO_KEY13_TOUCH, GPIO_KEY14_TOUCH, GPIO_KEY15_TOUCH, GPIO_KEY16_TOUCH,
        GPIO_KEY17_TOUCH, GPIO_KEY18_TOUCH, GPIO_KEY19_TOUCH, GPIO_KEY20_TOUCH, GPIO_KEY21_TOUCH, GPIO_KEY22_TOUCH, GPIO_KEY23_TOUCH, GPIO_KEY24_TOUCH,
        GPIO_KEY25_TOUCH, GPIO_KEY26_TOUCH, GPIO_KEY27_TOUCH, GPIO_KEY28_TOUCH, GPIO_KEY29_TOUCH, GPIO_KEY30_TOUCH, GPIO_KEY31_TOUCH, GPIO_KEY32_TOUCH,

      /****************************************
       ****************************************
      * SECTION: BME 
      ****************************************
      ****************************************/

        // I2C 0x76, 0x77 // Two Wire

      /****************************************
       ****************************************
      * SECTION: DB18X20 
       ****************************************
       ****************************************/ 
      
        GPIO_DSB_1, GPIO_DSB_2, // One Wire
        
      /****************************************
       ****************************************
      * SECTION: DHT11/DHT22 
       ****************************************
       ****************************************/ 
                      
        GPIO_DHT11_1, GPIO_DHT11_2,
        GPIO_DHT22_1, GPIO_DHT22_2,
      
      /****************************************
       ****************************************
      * SECTION: BH1750 
       ****************************************
       ****************************************/ 
      
        // I2C 0x23, 0x5C // Two Wire

      /****************************************
       ****************************************
      * SECTION: PIR 
       ****************************************
       ****************************************/ 

        GPIO_PIR_1, GPIO_PIR_2, GPIO_PIR_3, GPIO_PIR_4,
        GPIO_PIR_1_INV, GPIO_PIR_2_INV, GPIO_PIR_3_INV, GPIO_PIR_4_INV,

      /****************************************
       ****************************************
      * SECTION: Door 
       ****************************************
       ****************************************/ 

        // Switch as input, with rule linked to motion event
        GPIO_DOOR_LOCK, GPIO_DOOR_OPEN,

      /****************************************
       ****************************************
      * SECTION: L3GD20 
       ****************************************
       ****************************************/ 

        // SPI or I2C, 0x6B, 0x69

      /****************************************
       ****************************************
      * SECTION: LDR Analog + Digital Trigger 
       ****************************************
       ****************************************/ 
            
        GPIO_LDR_BASIC_DIGITAL1, GPIO_LDR_BASIC_ANALOG1,
        GPIO_LDR_BASIC_DIGITAL2, GPIO_LDR_BASIC_ANALOG2,

      /****************************************
       ****************************************
      * SECTION: LSM303D 
       ****************************************
       ****************************************/ 

        // SPI or I2C, 0x1D, 0x1E

      /****************************************
       ****************************************
      * SECTION: Moisture Analog 
       ****************************************
       ****************************************/ 

        GPIO_MOISTURE_ANALOG,

      /****************************************
       ****************************************
      * SECTION: SR04 
       ****************************************
       ****************************************/ 
      
        GPIO_SR04_TRIG, GPIO_SR04_ECHO,

      /****************************************
       ****************************************
      * SECTION: MPU9250 
       ****************************************
       ****************************************/ 

        // SPI or I2C, 0x68, 0x69

      /****************************************
       ****************************************
      * SECTION: Pulse Counter
       ****************************************
       ****************************************/ 
            
        GPIO_PULSE_COUNTER_1, GPIO_PULSE_COUNTER_2, GPIO_PULSE_COUNTER_3, GPIO_PULSE_COUNTER_4,
        GPIO_PULSE_COUNTER_1_NP, GPIO_PULSE_COUNTER_2_NP, GPIO_PULSE_COUNTER_3_NP, GPIO_PULSE_COUNTER_4_NP,

      /****************************************
       ****************************************
      * SECTION: Remote Device
       ****************************************
       ****************************************/ 

        // Virtual GPIOs representing remote devices, with rules to link them to actual GPIOs or other triggers
        
      /****************************************
       ****************************************
      * SECTION: Rotary Encoder
       ****************************************
       ****************************************/ 
            
        GPIO__ROTARY_ENCODER_A_1, GPIO__ROTARY_ENCODER_B_1,
        GPIO__ROTARY_ENCODER_A_2, GPIO__ROTARY_ENCODER_B_2,

      /****************************************
       ****************************************
      * SECTION: Sun Tracking
       ****************************************
       ****************************************/ 
            
        // Virtual, no GPIO

      /****************************************
       ****************************************
      * SECTION: Moon Tracking
       ****************************************
       ****************************************/ 
            
        // Virtual, no GPIO

      /****************************************
       ****************************************
      * SECTION: TOF Sensors
       ****************************************
       ****************************************/ 
            
        GPIO_VL53L0X_XSHUT1, GPIO_VL53L0X_XSHUT2, GPIO_VL53L0X_XSHUT3, GPIO_VL53L0X_XSHUT4, GPIO_VL53L0X_XSHUT5, GPIO_VL53L0X_XSHUT6, GPIO_VL53L0X_XSHUT7, GPIO_VL53L0X_XSHUT8,
        GPIO_VL53L1X_XSHUT1, GPIO_VL53L1X_XSHUT2, GPIO_VL53L1X_XSHUT3, GPIO_VL53L1X_XSHUT4, GPIO_VL53L1X_XSHUT5, GPIO_VL53L1X_XSHUT6, GPIO_VL53L1X_XSHUT7, GPIO_VL53L1X_XSHUT8,

      /****************************************
       ****************************************
      * SECTION: Radar HLK LD2410
       ****************************************
       ****************************************/ 

        GPIO_LD2410_RX, GPIO_LD2410_TX,

      /****************************************
       ****************************************
      * SECTION: GPS Serial
       ****************************************
       ****************************************/ 

        GPIO_GPS_SERIAL0_TX, GPIO_GPS_SERIAL0_RX,


  /****************************************
   ****************************************
   ****************************************
   * SECTION: Energy
   ****************************************
   ****************************************
   ****************************************/

      /****************************************
       ****************************************
      * SECTION: PZEM 
      ****************************************
      ****************************************/

        GPIO_PZEM0XX_TX, GPIO_PZEM004_RX, GPIO_PZEM0XX_RX_MODBUS, GPIO_PZEM017_RX,

      /****************************************
       ****************************************
      * SECTION: SDM serial meters 
      ****************************************
      ****************************************/

        GPIO_SDM120_TX, GPIO_SDM120_RX,
        GPIO_SDM630_TX, GPIO_SDM630_RX,

      /****************************************
       ****************************************
      * SECTION: MCP39F5
      ****************************************
      ****************************************/

        GPIO_MCP39F5_TX, GPIO_MCP39F5_RX, GPIO_MCP39F5_RST,

      /****************************************
       ****************************************
      * SECTION: CSE7766
      ****************************************
      ****************************************/
     
        GPIO_CSE7766_TX, GPIO_CSE7766_RX,

      /****************************************
       ****************************************
      * SECTION: ADE7953
      ****************************************
      ****************************************/
     
        GPIO_ADE7953_IRQ,

      /****************************************
       ****************************************
      * SECTION: SDM serial meters 
      ****************************************
      ****************************************/

  /****************************************
   ****************************************
   ****************************************
   * SECTION: Displays
   ****************************************
   ****************************************
   ****************************************/
      
      GPIO_BACKLIGHT,
      GPIO_OLED_RESET,

      /****************************************
       ****************************************
      * SECTION: Nextion 
      ****************************************
      ****************************************/
          
        GPIO_NEXTION_RX, GPIO_NEXTION_TX,
        GPIO_NEXTION_RX2, GPIO_NEXTION_TX2,

      /****************************************
       ****************************************
      * SECTION: SSD1306 OLED 
      ****************************************
      ****************************************/

      /****************************************
       ****************************************
      * SECTION: SH1106 OLED 
      ****************************************
      ****************************************/


  /****************************************
   ****************************************
   ****************************************
   * SECTION: Controllers and Usermods
   ****************************************
   ****************************************
   ****************************************/
      
      GPIO_FAN_PWM1,

      /****************************************
       * CONTROLLERS: Door / lock / chime
       ****************************************/
      GPIO_CHIME_INPUT, GPIO_CHIME_RINGER,
      
      /****************************************
       * SPECIAL: Sampling / sync signals
       ****************************************/
      GPIO_FUNCTION_RXON_SAMPLING_ENABLED,
      GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL,
            
  GPIO_SENSOR_END
};



// Error as warning to rethink GPIO usage with max 2045
static_assert(GPIO_SENSOR_END < 2000, "Too many UserSelectablePins");



/**
 * @brief Programmer selectable GPIO functionality
 */
enum ProgramSelectablePins {
  GPIO_FIX_START = 2046,
  GPIO_USER=9999,
  GPIO_MAX };


#endif  // mPin_GPIO

