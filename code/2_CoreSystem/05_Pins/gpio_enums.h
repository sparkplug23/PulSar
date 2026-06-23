#ifndef _GPIO_ENUMS_H_
#define _GPIO_ENUMS_H_

#include <stdint.h>

/*********************************************************************************************\
 * GPIO selectable base IDs
 *
 * This file defines the complete, static GPIO base-ID namespace used by PulSar.
 *
 * Rules:
 *   1. This enum is always complete. Do not wrap enum IDs in feature #ifdef blocks.
 *   2. This enum contains base IDs only. Do not add repeated numbered instance variants such as
 *      GPIO_REL2, GPIO_REL3, GPIO_MODEM_RX1, GPIO_MODEM_RX2, etc.
 *      A base ID may keep the human-facing first-instance suffix when that is the
 *      established function name, for example GPIO_REL1, GPIO_LED1, GPIO_KEY1.
 *   3. Runtime/module code calls the pin layer using base ID + index:
 *
 *        tkr_pins->Pin(GPIO_MODEM_RX, 0);   // Modem RX
 *        tkr_pins->Pin(GPIO_MODEM_RX, 1);   // Modem RX2
 *
 *   4. The pin layer, naming layer, and later gpio_bitpacked.h own all packing/index rules.
 *      External modules should not manually add indexes to enum values.
 *   5. Numeric strings that are part of a real device/protocol/signal name may remain, for
 *      example DHT11, DHT22, LD2410, L9110, ADC1_CH4. Numbered instance suffixes are removed.
 *   6. Semantic suffixes are preserved as separate base IDs, for example:
 *        GPIO_REL1     and GPIO_REL1_INV
 *        GPIO_KEY1     and GPIO_KEY1_INV_NP
 *        GPIO_SWT1     and GPIO_SWT1_NP
 *
\*********************************************************************************************/

enum SelectablePins_Base : uint16_t
{

  /****************************************
   * SECTION: Special assignment states
   ****************************************/

  GPIO_NONE = 0,   // No function assigned
  GPIO_USER = 1,   // User-configurable empty slot, mainly for template semantics

  /****************************************
   * SECTION: Real selectable GPIO base IDs
   ****************************************/

  // Force unused pin high
  GPIO_UNUSED_FORCED_HIGH,
  // Force unused pin low
  GPIO_UNUSED_FORCED_LOW,


  /****************************************
   * SECTION: Core System
   ****************************************/

      /****************************************
       * SECTION: Filesystem + SD Card
       ****************************************/

      // SD card using VSPI bus
      GPIO_SDCARD_VSPI_CSO, GPIO_SDCARD_VSPI_CLK,
      GPIO_SDCARD_VSPI_MOSI, GPIO_SDCARD_VSPI_MISO,

      // SD card using HSPI bus
      GPIO_SDCARD_HSPI_CSO, GPIO_SDCARD_HSPI_CLK,
      GPIO_SDCARD_HSPI_MOSI, GPIO_SDCARD_HSPI_MISO,

      // SD card using native MMC bus
      GPIO_SDCARD_MMC_CMD, GPIO_SDCARD_MMC_CLK, GPIO_SDCARD_MMC_D,

      /****************************************
       * SECTION: UART
       ****************************************/

      // Generic hardware serial
      GPIO_HWSERIAL_TX, GPIO_HWSERIAL_RX,
      // Generic software serial
      GPIO_SWSERIAL_TX, GPIO_SWSERIAL_RX,
      // Ring-buffer serial modes, currently separate but later should become HWSERIAL + mode/config
      GPIO_HWSERIAL_RING_BUFFER_TX, GPIO_HWSERIAL_RING_BUFFER_RX,
      // Debug serial stream
      GPIO_SERIAL_DEBUG_TX, GPIO_SERIAL_DEBUG_RX,
      // Serial bridge
      GPIO_SBR_TX, GPIO_SBR_RX,

      /****************************************
       * SECTION: I2C
       ****************************************/

      // Generic I2C bus pins
      GPIO_I2C_SCL, GPIO_I2C_SDA,

      /****************************************
       * SECTION: SPI
       ****************************************/

      // Generic hardware SPI pins
      GPIO_SPI_CS, GPIO_SPI_DC,
      // Generic software SPI pins
      GPIO_SSPI_MISO, GPIO_SSPI_MOSI, GPIO_SSPI_SCLK, GPIO_SSPI_CS, GPIO_SSPI_DC,

      /****************************************
       * SECTION: ADC Internal
       ****************************************/

      // ESP ADC internal temperature input
      GPIO_ADC0_TEMPERATURE,
      // ESP ADC1 hardware channels
      GPIO_ADC1_CH4, GPIO_ADC1_CH6, GPIO_ADC1_CH7,
      // ADC external interrupt/trigger input
      GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER,
      GPIO_ADC_INPUT,

      /****************************************
       * SECTION: PWM
       ****************************************/

      // PWM output
      GPIO_PWM1,
      // PWM output, inverted
      GPIO_PWM1_INV,


  /****************************************
   * SECTION: Drivers
   ****************************************/

      /****************************************
       * SECTION: LED
       ****************************************/

      // LED output
      GPIO_LED1,
      // LED output, inverted
      GPIO_LED1_INV,

      /****************************************
       * SECTION: Relay
       ****************************************/

      // Relay output
      GPIO_REL1,
      // Relay output, inverted
      GPIO_REL1_INV,

      /****************************************
       * SECTION: IR
       ****************************************/

      // Infrared send/receive
      GPIO_IRSEND, GPIO_IRRECV,

      /****************************************
       * SECTION: Shutters
       ****************************************/

      // Servo output
      GPIO_SHUTTER_OPEN, GPIO_SHUTTER_CLOSE,
      // Shutter button input
      GPIO_SHUTTER_BUTTON,

      /****************************************
       * SECTION: Servos
       ****************************************/

      // Servo output
      GPIO_SERVO,

      /****************************************
       * SECTION: SAW RF
       ****************************************/

      // Generic RF send/receive
      GPIO_RFSEND, GPIO_RFRECV,
      // 433 MHz RF send/receive
      GPIO_RF_433MHZ_RX, GPIO_RF_433MHZ_TX,

      /****************************************
       * SECTION: HBridge/Motor L9110
       ****************************************/

      // L9110 H-bridge control pins
      GPIO_HBRIDGE_L9110_IA, GPIO_HBRIDGE_L9110_IB,
      GPIO_HBRIDGE_L9110_OA, GPIO_HBRIDGE_L9110_OB,

      /****************************************
       * SECTION: Shelly Dimmer
       ****************************************/

      // Shelly 2 dimmer control pins
      GPIO_SHELLY2_SHD_BOOT0, GPIO_SHELLY2_SHD_RESET_INV,

      /****************************************
       * SECTION: Buzzer
       ****************************************/

      // Buzzer output
      GPIO_BUZZER,
      // Buzzer output, inverted
      GPIO_BUZZER_INV,

      /****************************************
       * SECTION: Cellular Modem
       ****************************************/

      // Cellular modem UART
      GPIO_MODEM_RX, GPIO_MODEM_TX,
      // Cellular modem control/status pins
      GPIO_MODEM_POWER_KEY,
      GPIO_MODEM_DATA_TERMINAL_READY_DTR,
      GPIO_MODEM_RING_INDICATOR,
      GPIO_MODEM_POWER_STATUS,
      GPIO_MODEM_NETWORK_STATUS,

      /****************************************
       * SECTION: Cameras
       ****************************************/

      // Camera external clock
      GPIO_WEBCAM_XCLK,
      // Camera parallel data bus
      GPIO_WEBCAM_DATA,
      // Camera timing/control pins
      GPIO_WEBCAM_PCLK,
      GPIO_WEBCAM_HREF,
      GPIO_WEBCAM_VSYNC,
      // Camera SCCB/I2C pins
      GPIO_WEBCAM_SIOD,
      GPIO_WEBCAM_SIOC,
      // Camera power/reset pins
      GPIO_WEBCAM_PWDN,
      GPIO_WEBCAM_RESET,

      /****************************************
       * SECTION: MAVLink
       ****************************************/

      // MAVLink serial pins
      GPIO_MAVLINK_TX, GPIO_MAVLINK_RX,


  /****************************************
   * SECTION: Sensors
   ****************************************/

      /****************************************
       * SECTION: Switches
       ****************************************/

      // Switch input
      GPIO_SWT1,
      // Switch input, inverted
      GPIO_SWT1_INV,
      // Switch input, no pull
      GPIO_SWT1_NP,
      // Switch input, inverted no pull
      GPIO_SWT1_INV_NP,

      /****************************************
       * SECTION: Buttons
       ****************************************/

      // Button input
      GPIO_KEY1,
      // Button input, inverted
      GPIO_KEY1_INV,
      // Button input, no pull
      GPIO_KEY1_NP,
      // Button input, inverted no pull
      GPIO_KEY1_INV_NP,
      // Touch button input
      GPIO_KEY1_TOUCH,

      /****************************************
       * SECTION: BME
       ****************************************/

      // BME sensor uses I2C, no dedicated GPIO function

      /****************************************
       * SECTION: DB18X20
       ****************************************/

      // DS18x20 one-wire data pin
      GPIO_DS18B20,

      /****************************************
       * SECTION: DHT11/DHT22
       ****************************************/

      // DHT11 data pin
      GPIO_DHT11,
      // DHT22 data pin
      GPIO_DHT22,

      /****************************************
       * SECTION: BH1750
       ****************************************/

      // BH1750 uses I2C, no dedicated GPIO function

      /****************************************
       * SECTION: PIR
       ****************************************/

      // PIR input
      GPIO_PIR,
      // PIR input, inverted
      GPIO_PIR_INV,

      /****************************************
       * SECTION: Door
       ****************************************/

      // Door lock/open inputs or outputs
      GPIO_DOOR_LOCK, GPIO_DOOR_OPEN,

      /****************************************
       * SECTION: L3GD20
       ****************************************/

      // L3GD20 uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: LDR Analog + Digital Trigger
       ****************************************/

      // LDR digital trigger input
      GPIO_LDR_BASIC_DIGITAL,
      // LDR analog input
      GPIO_LDR_BASIC_ANALOG,

      /****************************************
       * SECTION: LSM303D
       ****************************************/

      // LSM303D uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: Moisture Analog
       ****************************************/

      // Moisture analog input
      GPIO_MOISTURE_ANALOG,

      /****************************************
       * SECTION: SR04
       ****************************************/

      // Ultrasonic trigger/echo pins
      GPIO_SR04_TRIG, GPIO_SR04_ECHO,

      /****************************************
       * SECTION: MPU9250
       ****************************************/

      // MPU9250 uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: Pulse Counter
       ****************************************/

      // Pulse counter input
      GPIO_PULSE_COUNTER1,
      // Pulse counter input, no pull
      GPIO_PULSE_COUNTER1_NP,


      /****************************************
       * SECTION: Remote Device
       ****************************************/

      // Virtual GPIO representing remote device/link input
      GPIO_REMOTE_DEVICE,

      /****************************************
       * SECTION: Rotary Encoder
       ****************************************/

      // Rotary encoder A/B inputs
      GPIO_ROTARY_ENCODER_A, GPIO_ROTARY_ENCODER_B,

      /****************************************
       * SECTION: Sun Tracking
       ****************************************/

      // Virtual sun tracking function, no physical GPIO
      GPIO_SUN_TRACKING,

      /****************************************
       * SECTION: Moon Tracking
       ****************************************/

      // Virtual moon tracking function, no physical GPIO
      GPIO_MOON_TRACKING,

      /****************************************
       * SECTION: TOF Sensors
       ****************************************/

      // VL53L0X XSHUT pin
      GPIO_VL53L0X_XSHUT,
      // VL53L1X XSHUT pin
      GPIO_VL53L1X_XSHUT,

      /****************************************
       * SECTION: Radar HLK LD2410
       ****************************************/

      // LD2410 UART pins
      GPIO_LD2410_RX, GPIO_LD2410_TX,

      /****************************************
       * SECTION: GPS Serial
       ****************************************/

      // GPS serial pins
      GPIO_GPS_SERIAL_TX, GPIO_GPS_SERIAL_RX,


  /****************************************
   * SECTION: Energy
   ****************************************/

      /****************************************
       * SECTION: PZEM
       ****************************************/

      // PZEM serial pins
      GPIO_PZEM0XX_TX, GPIO_PZEM004_RX, GPIO_PZEM0XX_RX_MODBUS, GPIO_PZEM017_RX,

      /****************************************
       * SECTION: SDM serial meters
       ****************************************/

      // SDM120 serial pins
      GPIO_SDM120_TX, GPIO_SDM120_RX,
      // SDM630 serial pins
      GPIO_SDM630_TX, GPIO_SDM630_RX,

      /****************************************
       * SECTION: MCP39F5
       ****************************************/

      // MCP39F5 serial/reset pins
      GPIO_MCP39F5_TX, GPIO_MCP39F5_RX, GPIO_MCP39F5_RST,

      /****************************************
       * SECTION: CSE7766
       ****************************************/

      // CSE7766 serial pins
      GPIO_CSE7766_TX, GPIO_CSE7766_RX,

      /****************************************
       * SECTION: ADE7953
       ****************************************/

      // ADE7953 interrupt pin
      GPIO_ADE7953_IRQ,


  /****************************************
   * SECTION: Displays
   ****************************************/

      // Display backlight output
      GPIO_BACKLIGHT,
      // OLED reset pin
      GPIO_OLED_RESET,

      /****************************************
       * SECTION: Nextion
       ****************************************/

      // Nextion serial pins
      GPIO_NEXTION_RX, GPIO_NEXTION_TX,

      /****************************************
       * SECTION: SSD1306 OLED
       ****************************************/

      // SSD1306 uses I2C/SPI, no dedicated GPIO function currently

      /****************************************
       * SECTION: SH1106 OLED
       ****************************************/

      // SH1106 uses I2C/SPI, no dedicated GPIO function currently


  /****************************************
   * SECTION: Controllers and Usermods
   ****************************************/

      // Fan PWM output
      GPIO_FAN_PWM,

      /****************************************
       * SECTION: Door / Lock / Chime
       ****************************************/

      // Chime input/ringer pins
      GPIO_CHIME_INPUT, GPIO_CHIME_RINGER,

      /****************************************
       * SECTION: Sampling / Sync Signals
       ****************************************/

      // Sampling enable signal
      GPIO_RXON_SAMPLING_ENABLED,
      // CC1110 sync pulse signal
      GPIO_CC1110_SYNC_PULSE_SIGNAL,
      
/*********************************************************************************************\
 * Lighting GPIO functions
 *
 * Generic lighting allocation roles.
 * These do not describe the LED protocol, only the pin role owned by lighting.
\*********************************************************************************************/

GPIO_LIGHTING_DIGITAL,       // Digital lighting data pin, up to 16 outputs
GPIO_LIGHTING_CLOCK,         // Optional clock pin for 2-pin digital protocols, up to 4 outputs
GPIO_LIGHTING_PWM,           // PWM lighting channel pin, up to 10 channels
GPIO_LIGHTING_ONOFF,         // Simple binary lighting/output pin, up to 5 outputs

  /****************************************
   * SECTION: End Markers
   ****************************************/

  // End marker for selectable GPIO base IDs
  GPIO_MAX
};

/**
  uint16_t total bits = 16
  index bits          = 5
  base ID bits        = 11
  So the maximum packable base ID is:
  2^11 - 1 = 2047
 * 
 */
static_assert(GPIO_MAX < 2047, "Too many SelectablePins_Base IDs");

#endif  // _GPIO_ENUMS_H_
