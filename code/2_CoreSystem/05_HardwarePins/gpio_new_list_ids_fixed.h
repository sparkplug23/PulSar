#ifndef _GPIO_NEW_LIST_IDS_H_
#define _GPIO_NEW_LIST_IDS_H_

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "gpio_progmem.h"

/*********************************************************************************************\
 * Selectable GPIO base IDs and packed GPIO selection IDs
 *
 * SelectablePins_Base is the complete, static base-ID namespace. It must remain available in
 * every build so module code can refer to stable GPIO base functions regardless of which
 * features are compiled into a specific firmware.
 *
 * Indexed functions are represented by one base enum ID only. Do not add enum entries such as:
 *
 *   GPIO_REL1, GPIO_REL2, GPIO_REL3
 *   GPIO_MODEM_RX0, GPIO_MODEM_RX1, GPIO_MODEM_RX2
 *
 * Instead, keep only the base semantic function:
 *
 *   GPIO_REL
 *   GPIO_REL_INV
 *   GPIO_MODEM_RX
 *   GPIO_MODEM_TX
 *
 * The selected indexed function is represented internally as a packed ID:
 *
 *   packed_id = PGPIO(base_id) + index
 *
 * Example:
 *
 *   PGPIO(GPIO_MODEM_RX) + 0  -> Modem RX
 *   PGPIO(GPIO_MODEM_RX) + 1  -> Modem RX2
 *   PGPIO(GPIO_MODEM_RX) + 2  -> Modem RX3
 *
 * Public module code should not manually add indexes to enum values. Use the mPins API:
 *
 *   tkr_pins->Pin(GPIO_MODEM_RX, 0);
 *   tkr_pins->Pin(GPIO_MODEM_RX, 1);
 *
 * SelectablePins_BitPacked[] will later contain the build-specific selectable GPIO groups:
 *
 *   PGPIO(GPIO_MODEM_RX) + MGPIO(MAX_MODEM_UART)
 *   PGPIO(GPIO_MODEM_TX) + MGPIO(MAX_MODEM_UART)
 *
 * The lower 5 bits encode either:
 *
 *   - selected index, for stored/assigned GPIO packed IDs, PGPIO(base) + index
 *   - maximum selectable count minus one, for SelectablePins_BitPacked[] entries,
 *     PGPIO(base) + MGPIO(count)
 *
 * This avoids enum spacing/skipping problems and allows one base string to generate indexed
 * names such as "Relay", "Relay2", "Relay3" or "Modem RX", "Modem RX2", "Modem RX3".
\*********************************************************************************************/

#define GPIO_INDEX_BITS  5
#define GPIO_INDEX_MASK  0x001F
#define GPIO_BASE_MASK   0xFFE0

#define PGPIO(x)         ((uint16_t)((x) << GPIO_INDEX_BITS))
#define UGPIO(x)         ((uint16_t)((x) >> GPIO_INDEX_BITS))
#define MGPIO(x)         ((uint16_t)((x) ? ((x) - 1) : 0))

#define GPIO_ANY         32   // Any index / any GPIO variant

/**
 * @brief Complete static GPIO base function ID list.
 * @note This enum contains base IDs only, not indexed variants.
 * @note Do not add #ifdef inside this enum. Feature filtering belongs in SelectablePins_BitPacked[] and naming layers.
 * @note INV = inverted active level, low/ground is active.
 * @note NP  = no internal pullup/pulldown.
 * @note PD  = pulled down, hardware dependent.
 */
enum SelectablePins_Base {


  /****************************************
   * SECTION: Unassigned pin states
   ****************************************/

  gPIO_NONE,
  gPIO_UNUSED_FORCED_HIGH,
  gPIO_UNUSED_FORCED_LOW,


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
        
        gPIO_SDCARD_VSPI_CSO, gPIO_SDCARD_VSPI_CLK,
        gPIO_SDCARD_VSPI_MOSI, gPIO_SDCARD_VSPI_MISO,
        
        gPIO_SDCARD_HSPI_CSO, gPIO_SDCARD_HSPI_CLK,
        gPIO_SDCARD_HSPI_MOSI, gPIO_SDCARD_HSPI_MISO,

        gPIO_SDCARD_MMC_CMD, gPIO_SDCARD_MMC_CLK,
        gPIO_SDCARD_MMC_D0, gPIO_SDCARD_MMC_D1, gPIO_SDCARD_MMC_D2, gPIO_SDCARD_MMC_D3,

      /****************************************
       ****************************************
      * SECTION: UART 
      ****************************************
      ****************************************/

          /****************************************
           * UART: Generic hardware serial
           ****************************************/
          gPIO_HWSERIAL_TX, gPIO_HWSERIAL_RX,

          /****************************************
           * UART: Generic software serial
           ****************************************/
          gPIO_SWSERIAL_TX, gPIO_SWSERIAL_RX,

          /****************************************
           * UART: Ring-buffer serial modes (these are a type of HWSERIAL incoming stream, and likely should be removed as its own GPIO set, and instead have a switch with "GetData()" abstraction layer)
           ****************************************/ 
          gPIO_HWSERIAL_RING_BUFFER_TX, gPIO_HWSERIAL_RING_BUFFER_RX,

          /****************************************
           * UART: Debug serial (Really also needs it own debug stream abstraction layer, but for now just a fixed GPIO set)
           ****************************************/
          gPIO_SERIAL_DEBUG_TX, gPIO_SERIAL_DEBUG_RX,

          /****************************************
           * UART: Serial bridge
           ****************************************/
          gPIO_SBR_TX, gPIO_SBR_RX,
    
      /****************************************
       ****************************************
      * SECTION: I2C 
      ****************************************
      ****************************************/
      
      // Port I2C1
      gPIO_I2C_SCL, gPIO_I2C_SDA,
      // Port I2C2

      /****************************************
       ****************************************
      * SECTION: SPI 
      ****************************************
      ****************************************/

      // Hardware SPI
      gPIO_SPI_CS, gPIO_SPI_DC,
      // Software SPI
      gPIO_SSPI_MISO, gPIO_SSPI_MOSI, gPIO_SSPI_SCLK, gPIO_SSPI_CS, gPIO_SSPI_DC,

      /****************************************
       ****************************************
      * SECTION: ADC Internal (not external boards) 
      ****************************************
      ****************************************/

        gPIO_ADC0_TEMPERATURE,
        gPIO_ADC1_CH4, // Should probably simple switch to 0 to X, and then have the esp32 map that internally to the actual channel, since that is how the API works, and it would be more portable across different hardware with different numbers of channels
        gPIO_ADC1_CH6,
        gPIO_ADC1_CH7,
        gPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER,

      /****************************************
       ****************************************
      * SECTION: PWM 
      ****************************************
      ****************************************/
            
        gPIO_PWM,
        gPIO_PWM_INV,


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
     
        gPIO_LED,
        gPIO_LED_INV,

      /****************************************
       ****************************************
      * SECTION: Relay 
      ****************************************
      ****************************************/
     
        gPIO_REL,
        gPIO_REL_INV,

      /****************************************
       ****************************************
      * SECTION: IR 
      ****************************************
      ****************************************/
     
        gPIO_IRSEND, gPIO_IRRECV,

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
          
        gPIO_RFSEND, gPIO_RFRECV,
        gPIO_RF_433MHZ_RX, gPIO_RF_433MHZ_TX,

      /****************************************
       ****************************************
      * SECTION: HBridge/Motor 9110 
      ****************************************
      ****************************************/
     
        gPIO_HBRIDGE_L9110_IA, gPIO_HBRIDGE_L9110_IB, gPIO_HBRIDGE_L9110_OA, gPIO_HBRIDGE_L9110_OB,

      /****************************************
       ****************************************
      * SECTION: Shelly Dimmer 
      ****************************************
      ****************************************/
     
        gPIO_SHELLY2_SHD_BOOT0, gPIO_SHELLY2_SHD_RESET_INV,

      /****************************************
       ****************************************
      * SECTION: Buzzer 
      ****************************************
      ****************************************/
     
        gPIO_BUZZER, gPIO_BUZZER_INV,

      /****************************************
       ****************************************
      * SECTION: Cellular Modem 
      ****************************************
      ****************************************/
     
        gPIO_MODEM_RX, gPIO_MODEM_TX,


        gPIO_MODEM_POWER_KEY,
        gPIO_MODEM_DATA_TERMINAL_READY_DTR,
        gPIO_MODEM_RING_INDICATOR,
        gPIO_MODEM_POWER_STATUS,
        gPIO_MODEM_NETWORK_STATUS,

      /****************************************
       ****************************************
      * SECTION: Cameras 
      ****************************************
      ****************************************/

        gPIO_WEBCAM_XCLK,
        gPIO_WEBCAM_DATA1, gPIO_WEBCAM_DATA2, gPIO_WEBCAM_DATA3, gPIO_WEBCAM_DATA4, gPIO_WEBCAM_DATA5, gPIO_WEBCAM_DATA6, gPIO_WEBCAM_DATA7, gPIO_WEBCAM_DATA8,
        gPIO_WEBCAM_PCLK,
        gPIO_WEBCAM_HREF,
        gPIO_WEBCAM_VSYNC,
        gPIO_WEBCAM_SIOD,
        gPIO_WEBCAM_SIOC,
        gPIO_WEBCAM_PWDN,
        gPIO_WEBCAM_RESET,

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
        gPIO_SWT,

        /****************************************
         * INPUT: Switches, inverted
         ****************************************/
        gPIO_SWT_INV,

        /****************************************
         * INPUT: Switches, no pull
         ****************************************/
        gPIO_SWT_NP,

        /****************************************
         * INPUT: Switches, inverted no pull
         ****************************************/
        gPIO_SWT_INV_NP,

      /****************************************
       ****************************************
      * SECTION: Buttons 
      ****************************************
      ****************************************/
            
        /****************************************
         * INPUT: Buttons, normal
         ****************************************/
        gPIO_KEY,

        /****************************************
         * INPUT: Buttons, inverted
         ****************************************/
        gPIO_KEY_INV,

        /****************************************
         * INPUT: Buttons, no pull
         ****************************************/
        gPIO_KEY_NP,

        /****************************************
         * INPUT: Buttons, inverted no pull
         ****************************************/
        gPIO_KEY_INV_NP,

        /****************************************
         * INPUT: Buttons, touch
         ****************************************/
        gPIO_KEY_TOUCH,

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
      
        gPIO_DSB, // One Wire
        
      /****************************************
       ****************************************
      * SECTION: DHT11/DHT22 
       ****************************************
       ****************************************/ 
                      
        gPIO_DHT11,
        gPIO_DHT22,
      
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

        gPIO_PIR,
        gPIO_PIR_INV,

      /****************************************
       ****************************************
      * SECTION: Door 
       ****************************************
       ****************************************/ 

        // Switch as input, with rule linked to motion event
        gPIO_DOOR_LOCK, gPIO_DOOR_OPEN,

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
            
        gPIO_LDR_BASIC_DIGITAL, gPIO_LDR_BASIC_ANALOG,

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

        gPIO_MOISTURE_ANALOG,

      /****************************************
       ****************************************
      * SECTION: SR04 
       ****************************************
       ****************************************/ 
      
        gPIO_SR04_TRIG, gPIO_SR04_ECHO,

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
            
        gPIO_PULSE_COUNTER,
        gPIO_PULSE_COUNTER_NP,

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
            
        gPIO_ROTARY_ENCODER_A, gPIO_ROTARY_ENCODER_B,

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
            
        gPIO_VL53L0X_XSHUT,
        gPIO_VL53L1X_XSHUT,

      /****************************************
       ****************************************
      * SECTION: Radar HLK LD2410
       ****************************************
       ****************************************/ 

        gPIO_LD2410_RX, gPIO_LD2410_TX,

      /****************************************
       ****************************************
      * SECTION: GPS Serial
       ****************************************
       ****************************************/ 

        gPIO_GPS_SERIAL_TX, gPIO_GPS_SERIAL_RX,


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

        gPIO_PZEM0XX_TX, gPIO_PZEM004_RX, gPIO_PZEM0XX_RX_MODBUS, gPIO_PZEM017_RX,

      /****************************************
       ****************************************
      * SECTION: SDM serial meters 
      ****************************************
      ****************************************/

        gPIO_SDM120_TX, gPIO_SDM120_RX,
        gPIO_SDM630_TX, gPIO_SDM630_RX,

      /****************************************
       ****************************************
      * SECTION: MCP39F5
      ****************************************
      ****************************************/

        gPIO_MCP39F5_TX, gPIO_MCP39F5_RX, gPIO_MCP39F5_RST,

      /****************************************
       ****************************************
      * SECTION: CSE7766
      ****************************************
      ****************************************/
     
        gPIO_CSE7766_TX, gPIO_CSE7766_RX,

      /****************************************
       ****************************************
      * SECTION: ADE7953
      ****************************************
      ****************************************/
     
        gPIO_ADE7953_IRQ,

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
      
      gPIO_BACKLIGHT,
      gPIO_OLED_RESET,

      /****************************************
       ****************************************
      * SECTION: Nextion 
      ****************************************
      ****************************************/
          
        gPIO_NEXTION_RX, gPIO_NEXTION_TX,

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
      
      gPIO_FAN_PWM,

      /****************************************
       * CONTROLLERS: Door / lock / chime
       ****************************************/
      gPIO_CHIME_INPUT, gPIO_CHIME_RINGER,
      
      /****************************************
       * SPECIAL: Sampling / sync signals
       ****************************************/
      gPIO_RXON_SAMPLING_ENABLED,
      gPIO_CC1110_SYNC_PULSE_SIGNAL,
            
  gPIO_SENSOR_END,
};

// Error as warning to rethink GPIO usage with max 2045
static_assert(gPIO_SENSOR_END < 2000, "Too many SelectablePins_Base entries");

/**
 * @brief Programmer selectable GPIO functionality
 */
enum ProgramSelectablePins {
  gPIO_FIX_START = 2046,
  gPIO_USER = 9999,
  gPIO_MAX
};

#endif  // _gPIO_NEW_LIST_IDS_H_
