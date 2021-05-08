#ifndef _mHardwareTemplates2_H_
#define _mHardwareTemplates2_H_

// "HardwareTemplates" to be its own folder, with this main headers, plus 3 sub headers
// Split templates themselves into 3 files, for each esp8266, 8285 and 32 later

#include "stdint.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/mGlobalMacros.h"
// #include "2_CoreSystem/HardwarePins/mHardwarePins.h"
// #include "2_CoreSystem/HardwarePins/mHardwarePins_Esp32.h"
// #include "2_CoreSystem/HardwarePins/mHardwarePins_Esp82xx.h"

// #ifdef ESP8266
//   // #define DRX 3
//   #include "2_CoreSystem/HardwarePins/mHardwarePins_Esp82xx.h"
// #endif
// #ifdef ESP32
//   #include "2_CoreSystem/HardwarePins/mHardwarePins_Esp32.h"
// #endif

/**
 * @brief Enum list defining all GPIO functions that is fixed, unique and non changing across builds and hardware
 * @note This list will always be created in its full form
 * @note INV = Inverted pin, low/ground is active
 * @note NP  = No pullup/down 
 * @note Never add `#ifdef` into this enum, as I want all GPIO to always exist and be static for long term debug. `ifdef` should be considered around nicelist
 * @note PD = Pulled down (hardware dependant)
 */
enum GPIO_COMPLETE_STATIC_LIST_IDS {
  // Default for unused
  GPIO_NONE_ID,
  // DHT11/22 pins (enabled auto detect?)
  GPIO_DHT11_1OF2_ID, GPIO_DHT11_2OF2_ID,
  // DHT22
  GPIO_DHT22_1OF2_ID, GPIO_DHT22_2OF2_ID,
  // DallasTemperaure 1-wire sensor
  GPIO_DSB_1OF2_ID, GPIO_DSB_2OF2_ID,
  // I2C
  GPIO_I2C_SCL_ID, GPIO_I2C_SDA_ID,
  // Addressable RGB(w) leds
  GPIO_RGB_DATA_ID, GPIO_RGB_CLOCK_ID,
  // IR Transceiver
  GPIO_IRSEND_ID, GPIO_IRRECV_ID,
  // Switches = 8. User connected external switches (No longer using offset, making discrete options)
  GPIO_SWT1_ID,        GPIO_SWT2_ID,        GPIO_SWT3_ID, GPIO_SWT4_ID, GPIO_SWT5_ID, GPIO_SWT6_ID, GPIO_SWT7_ID, GPIO_SWT8_ID,
  GPIO_SWT1_INV_ID,    GPIO_SWT2_INV_ID,    GPIO_SWT3_INV_ID, GPIO_SWT4_INV_ID, GPIO_SWT5_INV_ID, GPIO_SWT6_INV_ID, GPIO_SWT7_INV_ID, GPIO_SWT8_INV_ID,
  GPIO_SWT1_NP_ID,     GPIO_SWT2_NP_ID,     GPIO_SWT3_NP_ID, GPIO_SWT4_NP_ID, GPIO_SWT5_NP_ID, GPIO_SWT6_NP_ID, GPIO_SWT7_NP_ID, GPIO_SWT8_NP_ID,
  GPIO_SWT1_INV_NP_ID, GPIO_SWT2_INV_NP_ID, GPIO_SWT3_INV_NP_ID, GPIO_SWT4_INV_NP_ID, GPIO_SWT5_INV_NP_ID, GPIO_SWT6_INV_NP_ID, GPIO_SWT7_INV_NP_ID, GPIO_SWT8_INV_NP_ID,
  // Buttons = 4 (rename keys to buttons)
  GPIO_KEY1_ID, GPIO_KEY2_ID, GPIO_KEY3_ID, GPIO_KEY4_ID,
  GPIO_KEY1_INV_ID, GPIO_KEY2_INV_ID, GPIO_KEY3_INV_ID, GPIO_KEY4_INV_ID,
  GPIO_KEY1_NP_ID, GPIO_KEY2_NP_ID, GPIO_KEY3_NP_ID, GPIO_KEY4_NP_ID, 
  GPIO_KEY1_INV_NP_ID, GPIO_KEY2_INV_NP_ID, GPIO_KEY3_INV_NP_ID, GPIO_KEY4_INV_NP_ID,
  //unknown button options?
  GPIO_CNTR1_ID, GPIO_CNTR2_ID, GPIO_CNTR3_ID, GPIO_CNTR4_ID,
  GPIO_CNTR1_NP_ID, GPIO_CNTR2_NP_ID, GPIO_CNTR3_NP_ID, GPIO_CNTR4_NP_ID,
  // Relays
  GPIO_REL1_ID, GPIO_REL2_ID, GPIO_REL3_ID, GPIO_REL4_ID, GPIO_REL5_ID, GPIO_REL6_ID, GPIO_REL7_ID, GPIO_REL8_ID,
  GPIO_REL1_INV_ID, GPIO_REL2_INV_ID, GPIO_REL3_INV_ID, GPIO_REL4_INV_ID, GPIO_REL5_INV_ID, GPIO_REL6_INV_ID, GPIO_REL7_INV_ID, GPIO_REL8_INV_ID,
  // PWM (Red   or C  Cold White, Green or CW Warm White, Blue, RGBW  (Cold) White, RGBCW Warm White)
  GPIO_PWM1_ID, GPIO_PWM2_ID, GPIO_PWM3_ID, GPIO_PWM4_ID, GPIO_PWM5_ID,
  GPIO_PWM1_INV_ID, GPIO_PWM2_INV_ID, GPIO_PWM3_INV_ID, GPIO_PWM4_INV_ID, GPIO_PWM5_INV_ID,
  // LEDs
  GPIO_LED1_ID, GPIO_LED2_ID, GPIO_LED3_ID, GPIO_LED4_ID,
  GPIO_LED1_INV_ID, GPIO_LED2_INV_ID, GPIO_LED3_INV_ID, GPIO_LED4_INV_ID,
  // MH-Z19 Serial interface
  GPIO_MHZ_TXD_ID, GPIO_MHZ_RXD_ID,
  // SenseAir Serial interface
  GPIO_SAIR_TX_ID, GPIO_SAIR_RX_ID,
  // SPI Chip Select, SPI Data Direction
  GPIO_SPI_CS_ID, GPIO_SPI_DC_ID,
  // Serial Bridge Serial interface
  GPIO_SBR_TX_ID, GPIO_SBR_RX_ID,
  // SR04 ultrasonic
  GPIO_SR04_TRIG_ID, GPIO_SR04_ECHO_ID,
  // SDMxx interfaces
  GPIO_SDM120_TX_ID, GPIO_SDM120_RX_ID, GPIO_SDM630_TX_ID, GPIO_SDM630_RX_ID,
  // PZEM0XX Serial interface, PZEM004T Serial interface, PZEM-014_ID,016 Serial Modbus interface, PZEM-003_ID,017 Serial Modbus interface 
  GPIO_PZEM0XX_TX_ID, GPIO_PZEM004_RX_ID, GPIO_PZEM0XX_MODBUS_RX_ID, GPIO_PZEM017_RX_ID,
  // HX711 Load Cell
  GPIO_HX711_SCK_ID, GPIO_HX711_DAT_ID,
  // RF Transceiver
  GPIO_RFSEND_ID, GPIO_RFRECV_ID,
  // Tuya Serial interface
  GPIO_TUYA_TX_ID, GPIO_TUYA_RX_ID,
  // Software SPI (Master Input Slave Output, Master Output Slave Input, Serial Clock, Chip Select, Data or Command)
  GPIO_SSPI_MISO_ID, GPIO_SSPI_MOSI_ID, GPIO_SSPI_SCLK_ID, GPIO_SSPI_CS_ID, GPIO_SSPI_DC_ID,
  // Energy monitor chip = MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_TX_ID, GPIO_MCP39F5_RX_ID, GPIO_MCP39F5_RST_ID,
  // CSE7766 Serial interface (S31 and Pow R2)
  GPIO_CSE7766_TX_ID, GPIO_CSE7766_RX_ID,
  // 433 MHz SAW Radio transceivers  
  GPIO_RF_433MHZ_RX_ID, GPIO_RF_433MHZ_TX_ID,
  // Serial pins (Auto detect hardware vs serial)
  // GPIO_SERIAL_TX_ID, GPIO_SERIAL_RX_ID, //phase out to more specific one below
  GPIO_HWSERIAL0_TX_ID, GPIO_HWSERIAL0_RX_ID,
  GPIO_HWSERIAL1_TX_ID, GPIO_HWSERIAL1_RX_ID,
  GPIO_HWSERIAL2_TX_ID, GPIO_HWSERIAL2_RX_ID,
  GPIO_SWSERIAL0_TX_ID, GPIO_SWSERIAL0_RX_ID,
  // RingBuffers are used on the receive data
  GPIO_HWSERIAL0_RING_BUFFER_TX_ID, GPIO_HWSERIAL0_RING_BUFFER_RX_ID,
  GPIO_HWSERIAL1_RING_BUFFER_TX_ID, GPIO_HWSERIAL1_RING_BUFFER_RX_ID,
  GPIO_HWSERIAL2_RING_BUFFER_TX_ID, GPIO_HWSERIAL2_RING_BUFFER_RX_ID,

  GPIO_SERIAL_DEBUG_TX_ID, GPIO_SERIAL_DEBUG_RX_ID,
  // GPS Serial0 on both esp devices (1 of 3)
  GPIO_GPS_SERIAL0_TX_ID, GPIO_GPS_SERIAL0_RX_ID,
  // Rotary switch
  GPIO_ROT1A_ID, GPIO_ROT1B_ID, GPIO_ROT2A_ID, GPIO_ROT2B_ID,
  // PIR Motion 
  GPIO_PIR_1_ID, GPIO_PIR_2_ID, GPIO_PIR_3_ID,         //pullup
  GPIO_PIR_1_NP_ID, GPIO_PIR_2_NP_ID, GPIO_PIR_3_NP_ID,
  GPIO_PIR_1_NP_INV_ID, GPIO_PIR_2_NP_INV_ID, GPIO_PIR_3_NP_INV_ID,
  GPIO_PIR_1_INV_ID, GPIO_PIR_2_INV_ID, GPIO_PIR_3_INV_ID, //pulldown
  // Link/Status led
  // Phase out link leds, it will default to led1
  // GPIO_LEDLNK_ID, GPIO_LEDLNK_INV_ID,     // Inverted link led
  // Arilux RF Receive input
  GPIO_ARIRFRCV_ID, GPIO_ARIRFSEL_ID,
  // Buzzer
  GPIO_BUZZER_ID, GPIO_BUZZER_INV_ID,
  // GPS Serial
  GPIO_GPS_NMEA_RX_ID, GPIO_GPS_NMEA_TX_ID,
  // Basic IO for door closed/lock position
  GPIO_DOOR_LOCK_ID, GPIO_DOOR_OPEN_ID,
  // Nextion Touch panels 
  GPIO_NEXTION_RX_ID, GPIO_NEXTION_TX_ID, GPIO_BACKLIGHT_ID,      // Display backlight control
  // Door bell chime (basic relay controls) - // Should be replaced as switch maybe?
  GPIO_CHIME_INPUT_ID, GPIO_CHIME_RINGER_ID,   // Relay silenoid
  // L9110 chip H-brige motor driver
  GPIO_HBRIDGE_L9110_IA_ID, GPIO_HBRIDGE_L9110_IB_ID, GPIO_HBRIDGE_L9110_OA_ID, GPIO_HBRIDGE_L9110_OB_ID, //GPIO_HBRIDGE_ANALOG_SENSE_ID, //analog pin to use for sensing   //change to use anolog sensor class that updates automatically
  // Analog measuring types
  GPIO_ANALOG_POSITION_ID,
  // PC Fan controls using PWM
  GPIO_FAN_PWM1_ID,
  // Shelly 2.5 Energy Sensor with I2C
  GPIO_ADE7953_IRQ_ID,
  // Shelly Dimmer 2 MCU
  GPIO_SHELLY2_SHD_BOOT0_ID, GPIO_SHELLY2_SHD_RESET_INV_ID,
  // ADC options
  GPIO_ADC0_TEMPERATURE_ID,

  GPIO_PULSE_COUNTER_ID,         // 
  GPIO_FAN_IRSEND_ID,        // Calls ceiling fan class to use IR sender

  // Displays
  GPIO_OLED_RESET_ID,


  /**
   * @note All esp32 only options below this point
   * */
  // Webcamera
  GPIO_WEBCAM_XCLK_ID,     // 0       (I)O                GPIO0, CAM_XCLK
  GPIO_WEBCAM_DATA1_ID, GPIO_WEBCAM_DATA2_ID, GPIO_WEBCAM_DATA3_ID, GPIO_WEBCAM_DATA4_ID, GPIO_WEBCAM_DATA5_ID, GPIO_WEBCAM_DATA6_ID, GPIO_WEBCAM_DATA7_ID, GPIO_WEBCAM_DATA8_ID,  // 35      I   NO PULLUP       GPIO35, CAM_DATA8
  GPIO_WEBCAM_PCLK_ID,     // 22      IO      LED         GPIO22, CAM_PCLK
  GPIO_WEBCAM_HREF_ID,     // 23      IO                  GPIO23, CAM_HREF
  GPIO_WEBCAM_VSYNC_ID,    // 25      IO                  GPIO25, CAM_VSYNC
  GPIO_WEBCAM_SIOD_ID,     // 26      IO                  GPIO26, CAM_SIOD
  GPIO_WEBCAM_SIOC_ID,     // 27      IO                  GPIO27, CAM_SIOC
  GPIO_WEBCAM_PWDN_ID,     // 32      IO                  GPIO32, CAM_PWDN
  // GPS Serial1 and 2 available on esp32 (2 of 3 ports)
  GPIO_GPS_SERIAL1_TX_ID, GPIO_GPS_SERIAL1_RX_ID,
  GPIO_GPS_SERIAL2_TX_ID, GPIO_GPS_SERIAL2_RX_ID,

  
  GPIO_SENSOR_END_ID 
}; // used 171of 250

/**
 * @brief Programmer selectable GPIO functionality
 */
enum ProgramSelectablePins {
  GPIO_FIX_START_ID = 251,
  GPIO_SPI_MISO_ID,       // SPI MISO library fixed pin GPIO12
  GPIO_SPI_MOSI_ID,       // SPI MOSI library fixed pin GPIO13
  GPIO_SPI_CLK_ID,        // SPI Clk library fixed pin GPIO14
  GPIO_USER_ID=9999,           // User configurable needs to be 255
  GPIO_MAX_ID };

/**
 * @brief Only the GPIOs that are enabled for this device (created from `GPIO_COMPLETE_STATIC_LIST_IDS`)
 */
const uint8_t kList_Selectable_Function_IDs[] PROGMEM = {
  GPIO_NONE_ID,           // Not used
  GPIO_KEY1_ID,           // Buttons
  GPIO_KEY1_NP_ID,
  GPIO_KEY1_INV_ID,
  GPIO_KEY1_INV_NP_ID,
  GPIO_KEY2_ID,
  GPIO_KEY2_NP_ID,
  GPIO_KEY2_INV_ID,
  GPIO_KEY2_INV_NP_ID,
  GPIO_KEY3_ID,
  GPIO_KEY3_NP_ID,
  GPIO_KEY3_INV_ID,
  GPIO_KEY3_INV_NP_ID,
  GPIO_KEY4_ID,
  GPIO_KEY4_NP_ID,
  GPIO_KEY4_INV_ID,
  GPIO_KEY4_INV_NP_ID,
  GPIO_SWT1_ID,           // User connected external switches
  GPIO_SWT1_NP_ID,
  GPIO_SWT2_ID,
  GPIO_SWT2_NP_ID,
  GPIO_SWT3_ID,
  GPIO_SWT3_NP_ID,
  GPIO_SWT4_ID,
  GPIO_SWT4_NP_ID,
  GPIO_SWT5_ID,
  GPIO_SWT5_NP_ID,
  GPIO_SWT6_ID,
  GPIO_SWT6_NP_ID,
  GPIO_SWT7_ID,
  GPIO_SWT7_NP_ID,
  GPIO_SWT8_ID,
  GPIO_SWT8_NP_ID,
  GPIO_REL1_ID,           // Relays
  GPIO_REL1_INV_ID,
  GPIO_REL2_ID,
  GPIO_REL2_INV_ID,
  GPIO_REL3_ID,
  GPIO_REL3_INV_ID,
  GPIO_REL4_ID,
  GPIO_REL4_INV_ID,
  GPIO_REL5_ID,
  GPIO_REL5_INV_ID,
  GPIO_REL6_ID,
  GPIO_REL6_INV_ID,
  GPIO_REL7_ID,
  GPIO_REL7_INV_ID,
  GPIO_REL8_ID,
  GPIO_REL8_INV_ID,
  GPIO_LED1_ID,           // Leds
  GPIO_LED1_INV_ID,
  GPIO_LED2_ID,
  GPIO_LED2_INV_ID,
  GPIO_LED3_ID,
  GPIO_LED3_INV_ID,
  GPIO_LED4_ID,
  GPIO_LED4_INV_ID,
  GPIO_PWM1_ID,           // RGB   Red   or C  Cold White
  GPIO_PWM1_INV_ID,
  GPIO_PWM2_ID,           // RGB   Green or CW Warm White
  GPIO_PWM2_INV_ID,
  GPIO_PWM3_ID,           // RGB   Blue
  GPIO_PWM3_INV_ID,
  GPIO_PWM4_ID,           // RGBW  (Cold) White
  GPIO_PWM4_INV_ID,
  GPIO_PWM5_ID,           // RGBCW Warm White
  GPIO_PWM5_INV_ID,
  GPIO_CNTR1_ID,          // Counters
  GPIO_CNTR1_NP_ID,
  GPIO_CNTR2_ID,
  GPIO_CNTR2_NP_ID,
  GPIO_CNTR3_ID,
  GPIO_CNTR3_NP_ID,
  GPIO_CNTR4_ID,
  GPIO_CNTR4_NP_ID,
  GPIO_HWSERIAL0_TX_ID,            // Serial interface
  GPIO_HWSERIAL0_RX_ID,            // Serial interface
  GPIO_HWSERIAL1_TX_ID,            // Serial interface
  GPIO_HWSERIAL1_RX_ID,            // Serial interface
  GPIO_HWSERIAL2_TX_ID,            // Serial interface
  GPIO_HWSERIAL2_RX_ID,            // Serial interface
  GPIO_SWSERIAL0_TX_ID,            // Serial interface
  GPIO_SWSERIAL0_RX_ID,            // Serial interface
#ifdef USE_I2C
  GPIO_I2C_SCL_ID,        // I2C SCL
  GPIO_I2C_SDA_ID,        // I2C SDA
#endif
#ifdef USE_SPI
  GPIO_SPI_CS_ID,         // SPI Chip Select
  GPIO_SPI_DC_ID,         // SPI Data Direction
  GPIO_SSPI_MISO_ID,      // Software SPI Master Input Slave Output
  GPIO_SSPI_MOSI_ID,      // Software SPI Master Output Slave Input
  GPIO_SSPI_SCLK_ID,      // Software SPI Serial Clock
  GPIO_SSPI_CS_ID,        // Software SPI Chip Select
  GPIO_SSPI_DC_ID,        // Software SPI Data or Command
#endif
#ifdef USE_DISPLAY
  GPIO_BACKLIGHT_ID,      // Display backlight control
#endif
#ifdef USE_MODULE_DRIVER_BUZZER
  GPIO_BUZZER_ID,         // Buzzer
  GPIO_BUZZER_INV_ID,     // Inverted buzzer
#endif
#ifdef USE_MODULE_SENSORS_DHT
  GPIO_DHT11_1OF2_ID,          // DHT11
  GPIO_DHT11_2OF2_ID,          // DHT11
  GPIO_DHT22_1OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
  GPIO_DHT22_2OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
#endif
#if defined(USE_DS18B20) || defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
  GPIO_DSB_1OF2_ID,            // Single wire DS18B20 or DS18S20
  GPIO_DSB_2OF2_ID,            // Single wire DS18B20 or DS18S20
#endif
#ifdef USE_WS2812
  GPIO_RGB_DATA_ID,         // WS2812 Led string
  GPIO_RGB_CLOCK_ID,         // WS2812 Led string
#endif
#ifdef USE_IR_REMOTE
  GPIO_IRSEND_ID,         // IR remote
#ifdef USE_IR_RECEIVE
  GPIO_IRRECV_ID,         // IR receiver
#endif
#endif
#ifdef USE_RC_SWITCH
  GPIO_RFSEND_ID,         // RF transmitter
  GPIO_RFRECV_ID,         // RF receiver
#endif
#ifdef USE_RF_SENSOR
  GPIO_RF_433MHZ_RX_ID,      // Rf receiver with sensor decoding
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  GPIO_SR04_TRIG_ID,      // SR04 Trigger pin
  GPIO_SR04_ECHO_ID,      // SR04 Echo pin
#endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
//   GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
//   GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
//   GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
//   GPIO_HLW_CF_ID,         // HLW8012 CF power
//   GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
// #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_I2C) && defined(USE_ADE7953)
  GPIO_ADE7953_IRQ_ID,    // ADE7953 IRQ
// #endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
  GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
#endif
// #if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_DC)
  GPIO_PZEM0XX_TX_ID,     // PZEM0XX Serial interface
// #endif
// #ifdef USE_PZEM004T
  GPIO_PZEM004_RX_ID,     // PZEM004T Serial interface
// #endif
// #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  GPIO_PZEM0XX_MODBUS_RX_ID,     // PZEM-014_ID,016 Serial Modbus interface
// #endif
// #ifdef USE_PZEM_DC
  GPIO_PZEM017_RX_ID,     // PZEM-003_ID,017 Serial Modbus interface
// #endif
#ifdef USE_SDM120
  GPIO_SDM120_TX_ID,      // SDM120 Serial interface
  GPIO_SDM120_RX_ID,      // SDM120 Serial interface
#endif
#ifdef USE_SDM630
  GPIO_SDM630_TX_ID,      // SDM630 Serial interface
  GPIO_SDM630_RX_ID,      // SDM630 Serial interface
#endif
#ifdef USE_SERIAL_BRIDGE
  GPIO_SBR_TX_ID,         // Serial Bridge Serial interface
  GPIO_SBR_RX_ID,         // Serial Bridge Serial interface
#endif
#ifdef USE_SENSEAIR
  GPIO_SAIR_TX_ID,        // SenseAir Serial interface
  GPIO_SAIR_RX_ID,        // SenseAir Serial interface
#endif
#ifdef USE_NOVA_SDS
  GPIO_SDS0X1_TX_ID,      // Nova Fitness SDS011 Serial interface
  GPIO_SDS0X1_RX_ID,      // Nova Fitness SDS011 Serial interface
#endif
#ifdef USE_PMS5003
  GPIO_PMS5003_ID,        // Plantower PMS5003 Serial interface
#endif
#ifdef USE_TX20_WIND_SENSOR
  GPIO_TX20_TXD_BLACK_ID, // TX20 Transmission Pin
#endif
#ifdef USE_MP3_PLAYER
  GPIO_MP3_DFR562_ID,     // RB-DFR-562_ID, DFPlayer Mini MP3 Player Serial interface
#endif
#ifdef USE_TUYA_DIMMER
  GPIO_TUYA_TX_ID,        // Tuya Serial interface
  GPIO_TUYA_RX_ID,        // Tuya Serial interface
#endif
#ifdef USE_AZ7798
  GPIO_AZ_TXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
  GPIO_AZ_RXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
#endif
#ifdef USE_PN532_HSU
  GPIO_PN532_TXD_ID,      // PN532 HSU Tx
  GPIO_PN532_RXD_ID,      // PN532 HSU Rx
#endif
#ifdef USE_MGC3130
  GPIO_MGC3130_XFER_ID,
  GPIO_MGC3130_RESET_ID,
#endif
#ifdef USE_MAX31855
  GPIO_MAX31855CS_ID,     // MAX31855 Serial interface
  GPIO_MAX31855CLK_ID,    // MAX31855 Serial interface
  GPIO_MAX31855DO_ID,     // MAX31855 Serial interface
#endif
#ifdef USE_SM16716
  GPIO_SM16716_CLK_ID,    // SM16716 CLOCK
  GPIO_SM16716_DAT_ID,    // SM16716 DATA
  GPIO_SM16716_SEL_ID,    // SM16716 SELECT
#endif // USE_SM16716
#ifdef ROTARY_V1
  GPIO_ROT1A_ID,          // Rotary switch1 A Pin
  GPIO_ROT1B_ID,          // Rotary switch1 B Pin
  GPIO_ROT2A_ID,          // Rotary switch2 A Pin
  GPIO_ROT2B_ID,          // Rotary switch2 B Pin
#endif
#ifdef USE_ARILUX_RF
  GPIO_ARIRFRCV_ID,       // AliLux RF Receive input
#endif
#ifdef USE_HRE
  GPIO_HRE_CLOCK_ID,
  GPIO_HRE_DATA
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  GPIO_DOOR_OPEN_ID,
#endif
#ifdef USE_SENSOR_DOOR_LOCK
  GPIO_DOOR_LOCK_ID,
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
   GPIO_HBRIDGE_L9110_IA_ID,
   GPIO_HBRIDGE_L9110_IA_ID,
   GPIO_HBRIDGE_L9110_OA_ID,
   GPIO_HBRIDGE_L9110_OA_ID,
   GPIO_HBRIDGE_ANALOG_SENSE_ID,
#endif
};

// Friendly names of gpio functions (add ifdefs later)

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_NONE_CTR)              D_GPIO_FUNCTION_NONE_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DHT11_1_CTR)           D_GPIO_FUNCTION_DHT11_1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DHT11_2_CTR)           D_GPIO_FUNCTION_DHT11_2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DHT22_1_CTR)           D_GPIO_FUNCTION_DHT22_1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DHT22_2_CTR)           D_GPIO_FUNCTION_DHT22_2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_AM2301_1_CTR)          D_GPIO_FUNCTION_AM2301_CTR "_1";
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_AM2301_2_CTR)          D_GPIO_FUNCTION_AM2301_CTR "_2";
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DS18X20_1_CTR)         D_GPIO_FUNCTION_DS18X20_1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DS18X20_2_CTR)         D_GPIO_FUNCTION_DS18X20_2_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_I2C_SCL_CTR)           D_GPIO_FUNCTION_I2C_SCL_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_I2C_SDA_CTR)           D_GPIO_FUNCTION_I2C_SDA_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_OLED_RESET_CTR)           D_GPIO_FUNCTION_OLED_RESET_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_RGB_DATA_CTR)          D_GPIO_FUNCTION_RGB_DATA_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_RGB_CLOCK_CTR)         D_GPIO_FUNCTION_RGB_CLOCK_CTR;

// #ifdef USE_MODULE_SENSORS_MOTION
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_1_CTR)             D_GPIO_FUNCTION_PIR_1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_2_CTR)             D_GPIO_FUNCTION_PIR_2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_3_CTR)             D_GPIO_FUNCTION_PIR_3_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_1_INV_CTR)         D_GPIO_FUNCTION_PIR_1_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_2_INV_CTR)         D_GPIO_FUNCTION_PIR_2_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_3_INV_CTR)         D_GPIO_FUNCTION_PIR_3_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_1_NP_CTR)          D_GPIO_FUNCTION_PIR_1_NP_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_2_NP_CTR)          D_GPIO_FUNCTION_PIR_2_NP_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_3_NP_CTR)          D_GPIO_FUNCTION_PIR_3_NP_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_1_NP_INV_CTR)          D_GPIO_FUNCTION_PIR_1_NP_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_2_NP_INV_CTR)          D_GPIO_FUNCTION_PIR_2_NP_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PIR_3_NP_INV_CTR)          D_GPIO_FUNCTION_PIR_3_NP_INV_CTR;
// #endif
// #ifdef USE_MODULE_SENSORS_DOOR
// DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DOOR_DETECT_CTR)       D_GPIO_FUNCTION_DOOR_DETECT_CTR;
// #endif

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_CHIME_INPUT_CTR)       D_GPIO_FUNCTION_CHIME_INPUT_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_CHIME_RINGER_CTR)      D_GPIO_FUNCTION_CHIME_RINGER_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_IRSEND_CTR)            D_GPIO_FUNCTION_IRSEND_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_FAN_IRSEND_CTR)        D_GPIO_FUNCTION_FAN_IRSEND_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SWT1_CTR)         D_GPIO_FUNCTION_SWT1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SWT2_CTR)         D_GPIO_FUNCTION_SWT2_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SWT1_NP_CTR)         D_GPIO_FUNCTION_SWT1_NP_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SWT2_NP_CTR)         D_GPIO_FUNCTION_SWT2_NP_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL1_CTR)          D_GPIO_FUNCTION_REL1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL2_CTR)          D_GPIO_FUNCTION_REL2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL3_CTR)          D_GPIO_FUNCTION_REL3_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL4_CTR)          D_GPIO_FUNCTION_REL4_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL1_INV_CTR)          D_GPIO_FUNCTION_REL1_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL2_INV_CTR)          D_GPIO_FUNCTION_REL2_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL3_INV_CTR)          D_GPIO_FUNCTION_REL3_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_REL4_INV_CTR)          D_GPIO_FUNCTION_REL4_INV_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_FAN_PWM1_CTR)          D_GPIO_FUNCTION_FAN_PWM1_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM1_CTR)          D_GPIO_FUNCTION_PWM1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM2_CTR)          D_GPIO_FUNCTION_PWM2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM3_CTR)          D_GPIO_FUNCTION_PWM3_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM4_CTR)          D_GPIO_FUNCTION_PWM4_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM5_CTR)          D_GPIO_FUNCTION_PWM5_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM1_INV_CTR)          D_GPIO_FUNCTION_PWM1_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM2_INV_CTR)          D_GPIO_FUNCTION_PWM2_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM3_INV_CTR)          D_GPIO_FUNCTION_PWM3_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM4_INV_CTR)          D_GPIO_FUNCTION_PWM4_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PWM5_INV_CTR)          D_GPIO_FUNCTION_PWM5_INV_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)   D_GPIO_FUNCTION_HWSERIAL0_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)   D_GPIO_FUNCTION_HWSERIAL0_RX_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR) D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR) D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR;
#ifdef ESP32
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR)   D_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR;
#endif // ESP32


//DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LEDLNK_CTR)         D_GPIO_FUNCTION_LEDLNK_CTR;
//DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LEDLNK_INV_CTR)         D_GPIO_FUNCTION_LED1_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED1_CTR)          D_GPIO_FUNCTION_LED1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED2_CTR)          D_GPIO_FUNCTION_LED2_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED3_CTR)          D_GPIO_FUNCTION_LED3_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED4_CTR)          D_GPIO_FUNCTION_LED4_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED5_CTR)          D_GPIO_FUNCTION_LED5_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED1_INV_CTR)     D_GPIO_FUNCTION_LED1_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED2_INV_CTR)     D_GPIO_FUNCTION_LED2_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED3_INV_CTR)     D_GPIO_FUNCTION_LED3_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED4_INV_CTR)     D_GPIO_FUNCTION_LED4_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_LED5_INV_CTR)     D_GPIO_FUNCTION_LED5_INV_CTR;

// #if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_DC)
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)     D_GPIO_FUNCTION_PZEM0XX_TX_CTR;
// #endif
// #ifdef USE_MODULE_ENERGY_PZEM004T_V3
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR)     D_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR;
// #endif
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PZEM004_RX_CTR)     D_GPIO_FUNCTION_PZEM004_RX_CTR;

// #ifdef USE_MODULE_SENSORS_ULTRASONICS
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SR04_ECHO_CTR)     D_GPIO_FUNCTION_SR04_ECHO_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SR04_TRIG_CTR)     D_GPIO_FUNCTION_SR04_TRIG_CTR;
// #endif

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_NEXTION_TX_CTR)     D_GPIO_FUNCTION_NEXTION_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_NEXTION_RX_CTR)     D_GPIO_FUNCTION_NEXTION_RX_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_KEY1_CTR)      D_GPIO_FUNCTION_KEY1_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_KEY2_CTR)      D_GPIO_FUNCTION_KEY2_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_KEY1_INV_CTR)      D_GPIO_FUNCTION_KEY1_INV_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_KEY2_INV_CTR)      D_GPIO_FUNCTION_KEY2_INV_CTR;


DEFINE_PGM_CTR(PM_GPIO_FUNCTION_KEY1_NP_CTR)      D_GPIO_FUNCTION_KEY1_NP_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DOOR_OPEN_CTR)           D_GPIO_FUNCTION_DOOR_OPEN_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_DOOR_LOCK_CTR)           D_GPIO_FUNCTION_DOOR_LOCK_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)  D_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)  D_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR;


DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR) D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR) D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR) D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR;
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR) D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_ANALOG_POSITION_CTR) D_GPIO_FUNCTION_ANALOG_POSITION_CTR;

DEFINE_PGM_CTR(PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)  D_GPIO_FUNCTION_ADE7953_IRQ_CTR;

#ifdef ESP32

  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)            D_GPIO_WEBCAM_XCLK_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR)            D_GPIO_WEBCAM_DATA1_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR)            D_GPIO_WEBCAM_DATA2_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR)            D_GPIO_WEBCAM_DATA3_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR)            D_GPIO_WEBCAM_DATA4_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR)            D_GPIO_WEBCAM_DATA5_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR)           D_GPIO_WEBCAM_DATA6_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR)            D_GPIO_WEBCAM_DATA7_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR)            D_GPIO_WEBCAM_DATA8_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)            D_GPIO_WEBCAM_PCLK_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)            D_GPIO_WEBCAM_HREF_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)            D_GPIO_WEBCAM_VSYNC_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)            D_GPIO_WEBCAM_SIOD_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)            D_GPIO_WEBCAM_SIOC_CTR;
  DEFINE_PGM_CTR(PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)            D_GPIO_WEBCAM_PWDN_CTR;


#endif

/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/
/**********MODULE CONFIG**********************************************************************************/

// Create ifdefs to include certain module templates
#define USE_MODULE_TEMPLATE_SONOFF_BASIC
#define USE_MODULE_TEMPLATE_H801
#define USE_MODULE_TEMPLATE_MAGICHOME
#define USE_MODULE_TEMPLATE_SHELLY1
#define USE_MODULE_TEMPLATE_SHELLY_2P5
#define USE_MODULE_TEMPLATE_SONOFF_IFAN03
#define USE_MODULE_TEMPLATE_SHELLY_DIMMER2

// Supported hardware modules. Leave completed list
enum SupportedModules_StaticCompleteList {
  MODULE_WEMOS_ID,
  MODULE_SONOFF_BASIC_ID,
  MODULE_H801_ID,
  MODULE_MAGICHOME_ID,
  MODULE_SHELLY1_ID,
  MODULE_SHELLY2P5_ID,
  MODULE_SHELLY_DIMMER2_ID,
  MODULE_SONOFF_IFAN03_ID,
  // Last module
  MODULE_MAXMODULE 
};


// define list
  #define            D_MODULE_NAME_GENERIC_CTR            "Generic"
  DEFINE_PGM_CTR(PM_MODULE_NAME_GENERIC_CTR)          D_MODULE_NAME_GENERIC_CTR;
  #define            D_MODULE_NAME_NODEMCU_CTR            "NodeMCU"
  DEFINE_PGM_CTR(PM_MODULE_NAME_NODEMCU_CTR)          D_MODULE_NAME_NODEMCU_CTR;
  #define            D_MODULE_NAME_USERMODULE_CTR            "USERMODULE-Temp" //to enable tas code to work, but I need to move to another flag instead
  DEFINE_PGM_CTR(PM_MODULE_NAME_USERMODULE_CTR)          D_MODULE_NAME_USERMODULE_CTR;
#ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
  #define            D_MODULE_NAME_SONOFF_BASIC_CTR       "Sonoff Basic"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_BASIC_CTR)     D_MODULE_NAME_SONOFF_BASIC_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_H801
  #define            D_MODULE_NAME_H801_CTR               "H801"
  DEFINE_PGM_CTR(PM_MODULE_NAME_H801_CTR)             D_MODULE_NAME_H801_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_MAGICHOME
  #define            D_MODULE_NAME_MAGICHOME_CTR          "MagicHome"
  DEFINE_PGM_CTR(PM_MODULE_NAME_MAGICHOME_CTR)        D_MODULE_NAME_MAGICHOME_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY1
  #define            D_MODULE_NAME_SHELLY1_CTR       "Shelly 1"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY1_CTR)     D_MODULE_NAME_SHELLY1_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
  #define            D_MODULE_NAME_SHELLY2P5_CTR       "Shelly 2"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY2P5_CTR)     D_MODULE_NAME_SHELLY2P5_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  #define            D_MODULE_NAME_SHELLY_DIMMER2_CTR       "Shelly Dimmer 2"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY_DIMMER2_CTR)     D_MODULE_NAME_SHELLY_DIMMER2_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  #define            D_MODULE_NAME_SONOFF_IFAN03_CTR       "Sonoff IFAN03"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_IFAN03_CTR)     D_MODULE_NAME_SONOFF_IFAN03_CTR;
#endif


/********************************************************************************************/

#define USER_MODULE        255

#ifdef ESP8266
#define MAX_GPIO_PIN       17   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      13   // MAX_GPIO_PIN - MIN_FLASH_PINS  // GPIO12 + A0
#define WEMOS_MODULE       0    // Wemos module ie GERNIC IS FIRST ALWAYS


// New method ESP8266
// #ifdef ESP8266

//to be moved
#define MAX_GPI8_PIN       17   // Number of supported GPIO (0..16)
#define FLASH_PINS         6    // Number of flash chip pins

// #define MAX_GPIO_PIN       18   // Number of supported GPIO (0..16 + ADC0)
#define ADC0_PIN           17   // Pin number of ADC0
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
// #define MAX_USER_PINS      14   // MAX_GPIO_PIN - MIN_FLASH_PINS
// #define WEMOS_MODULE       17   // Wemos module


typedef struct MYIO8 {
  uint16_t      io[MAX_GPI8_PIN];
} myio8;                        // 17 bytes

typedef struct MYCFGIO8285 {
  uint16_t      io[MAX_GPI8_PIN - MIN_FLASH_PINS];
} mycfgio8285;                  // 13 bytes

typedef struct MYTMPLT8285 {
  mycfgio8285  gp;
  uint8_t      flag;
} mytmplt8285;                  // 14 bytes

typedef struct MYCFGIO8266 {
  uint16_t      io[MAX_GPI8_PIN - 4];//temp fix FLASH_PINS];
} mycfgio8266;                  // 11 bytes

typedef struct MYTMPLT8266 {
  mycfgio8266  gp;
  uint16_t      flag;
} mytmplt8266;                  // 12 bytes

#endif // ESP8266


#ifdef ESP32


#define MAX_GPIO_PIN       40   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021222324252627282930313233343536373839
const char PINS_WEMOS[] PROGMEM = "IOTXIORXIOIOflashcFLFLolIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOAOAOIAIAIAIAIAIA";


#endif


// ESP32?
typedef struct MYIO {
  uint16_t      io[MAX_GPIO_PIN];
} myio;                         // 18 * 2 = 36 bytes / 40 * 2 = 80 bytes

typedef struct MYCFGIO {
  uint16_t      io[MAX_USER_PINS];
} mycfgio;                      // 14 * 2 = 28 bytes / 36 * 2 = 72 bytes




/*-------------------------------------------------------------------------------------------*\
 * ESP8266 single Analog / Digital converter input
\*-------------------------------------------------------------------------------------------*/

#ifdef ESP8266
// const uint16_t kAdcNiceList[] PROGMEM = {
//   GPIO_NONE,                              // Not used
//   GPIO_ADC_INPUT),                  // Analog inputs
//   GPIO_ADC_TEMP),                   // Thermistor
//   GPIO_ADC_LIGHT),                  // Light sensor
//   GPIO_ADC_BUTTON) + MAX_KEYS,      // Button
//   GPIO_ADC_BUTTON_INV) + MAX_KEYS,
//   GPIO_ADC_RANGE),                  // Range
//   GPIO_ADC_CT_POWER),               // Current
//   GPIO_ADC_JOY),                    // Joystick
//   GPIO_ADC_PH),                     // Analog PH Sensor
// };
#endif  // ESP8266

// User selectable ADC functionality
enum UserSelectableAdc {
  ADC_NONE,           // Not used
  ADC_INPUT,          // Analog input
  ADC_TEMP,           // Thermistor
  ADC_LIGHT,          // Light sensor
  ADC_BUTTON,         // Button
  ADC_BUTTON_INV,
  ADC_RANGE,          // Range
  ADC_CT_POWER,       // Current
  ADC_JOY,            // Joystick
  ADC_PH,             // Analog PH Sensor
//  ADC_SWITCH,         // Switch
//  ADC_SWITCH_INV,
  ADC_END };


#define GPIO_FLAG_USED         2  // Currently two flags used

#define GPIO_FLAG_ADC0_ID      1  // Allow ADC0 when define USE_ADC_VCC is disabled
#define GPIO_FLAG_ADC0_TEMP    2  // Allow ADC0 as Temperature sensor when define USE_ADC_VCC is disabled
#define GPIO_FLAG_SPARE02      4
#define GPIO_FLAG_SPARE03      8
#define GPIO_FLAG_SPARE04     16
#define GPIO_FLAG_SPARE05     32
#define GPIO_FLAG_SPARE06     64
#define GPIO_FLAG_SPARE07    128


typedef union {
  uint8_t data;
  struct {
    uint8_t adc0 : 1;             // Allow ADC0 when define USE_ADC_VCC is disabled
    uint8_t adc0_temp : 1;        // Allow ADC0 as Temperature sensor when define USE_ADC_VCC is disabled
    uint8_t spare02 : 1;
    uint8_t spare03 : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} gpio_flag;


typedef struct MYTMPLT {
  mycfgio      gp;
  gpio_flag    flag;
} mytmplt;


// ID list
const uint8_t kModuleNiceList_IDS[] PROGMEM = {
  MODULE_WEMOS_ID, //esp8266
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
    MODULE_SONOFF_BASIC_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801  
    MODULE_H801_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
    MODULE_MAGICHOME_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
    MODULE_SHELLY1_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    MODULE_SHELLY2P5_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    MODULE_SHELLY_DIMMER2_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    MODULE_SONOFF_IFAN03_ID
  #endif
};

DEFINE_PGM_CTR(kModules_Name_list_ctr)
{
  D_MODULE_NAME_GENERIC_CTR "|"
  D_MODULE_NAME_SONOFF_BASIC_CTR  "|"
  D_MODULE_NAME_H801_CTR          "|"
  D_MODULE_NAME_MAGICHOME_CTR     "|"
  D_MODULE_NAME_SHELLY1_CTR       "|"
  D_MODULE_NAME_SHELLY2P5_CTR     "|"
  D_MODULE_NAME_SHELLY_DIMMER2_CTR "|"
  D_MODULE_NAME_SONOFF_IFAN03_CTR 
};

#ifdef ESP8266

/*********************************************************************************************\
 * Templates with 12 usable pins (ESP8266)
\*********************************************************************************************/

const mytmplt8266 kModules[MODULE_MAXMODULE] PROGMEM = {
  // Generic option always first
  { //MODULE_WEMOS_ID,         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
    GPIO_USER_ID,        // GPIO00 D3 Wemos Button Shield
    GPIO_USER_ID,        // GPIO01 TX Serial RXD
    GPIO_USER_ID,        // GPIO02 D4 Wemos DHT Shield
    GPIO_USER_ID,        // GPIO03 RX Serial TXD and Optional sensor
    GPIO_USER_ID,        // GPIO04 D2 Wemos I2C SDA
    GPIO_USER_ID,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_USER_ID,        // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    GPIO_USER_ID,        // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER_ID,        // GPIO12 D6
    GPIO_USER_ID,        // GPIO13 D7
    GPIO_USER_ID,        // GPIO14 D5
    GPIO_USER_ID,        // GPIO15 D8
    GPIO_USER_ID,        // GPIO16 D0 Wemos Wake
    GPIO_FLAG_ADC0_ID    // ADC0 A0 Analog input
  },
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
  { // MODULE_SONOFF_BASIC_ID    // Sonoff Basic (ESP8266)
    GPIO_KEY1_ID,     // GPIO00 Button
    GPIO_USER_ID,     // GPIO01 Serial RXD and Optional sensor
    GPIO_LED2_INV_ID,     // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_USER_ID,     // GPIO03 Serial TXD and Optional sensor
    GPIO_USER_ID,     // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1_ID,     // GPIO12 Red Led and Relay (0 = Off_ID, 1 = On)
    GPIO_LED1_INV_ID, // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER_ID,     // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801
  { // MODULE_H801_ID,            // Lixada H801 Wifi (ESP8266)
    GPIO_USER_ID,        // GPIO00 E-FW Button
    GPIO_LED1_ID,        // GPIO01 Green LED - Link and Power status
    GPIO_USER_ID,        // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
    GPIO_USER_ID,        // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
    GPIO_PWM5_ID,        // GPIO04 W2 - PWM5
    GPIO_LED2_INV_ID,    // GPIO05 Red LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3_ID,        // GPIO12 Blue
    GPIO_PWM2_ID,        // GPIO13 Green
    GPIO_PWM4_ID,        // GPIO14 W1 - PWM4
    GPIO_PWM1_ID,        // GPIO15 Red
    0, // GPIO16
    0 // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
  { //MODULE_MAGICHOME_ID,       // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                      // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
    0,
    GPIO_USER_ID,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LED1_INV_ID,    // GPIO02 Blue onboard LED - Link and Power status
    GPIO_USER_ID,        // GPIO03 Serial TXD and Optional sensor
    GPIO_ARIRFRCV_ID,    // GPIO04 IR or RF receiver (optional) (Arilux LC10)
    GPIO_PWM2_ID,        // GPIO05 RGB LED Green
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3_ID,        // GPIO12 RGB LED Blue
    GPIO_USER_ID,        // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
    GPIO_PWM1_ID,        // GPIO14 RGB LED Red
    GPIO_LED4_INV_ID,    // GPIO15 RF receiver control (Arilux LC10)
    0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
  { //MODULE_SHELLY1_ID,        // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
     0,                // GPIO00 - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,                // GPIO01 Serial RXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,
     0,                // GPIO03 Serial TXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     GPIO_REL1_ID,        // GPIO04 Relay (0 = Off, 1 = On)
     GPIO_SWT1_NP_ID,     // GPIO05 SW pin
                       // GPIO06 (SD_CLK   Flash)
                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
     0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
     0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                       // GPIO11 (SD_CMD   Flash)
     0, 0, 0, 0, 0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5  
  {  //MODULE_SHELLY2P5_ID,        // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
    GPIO_LED1_INV_ID,     // GPIO00 LED1i
    0,                    // GPIO01 None
    GPIO_KEY1_ID,         // GPIO02 Button1
    0,                    // GPIO03 None
    GPIO_REL1_ID,         // GPIO04 Relay1
    GPIO_SWT2_NP_ID,      // GPIO05 Switch2n
                          // GPIO06 (SD_CLK   Flash)
                          // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                          // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                    // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                    // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                          // GPIO11 (SD_CMD   Flash)
    GPIO_I2C_SDA_ID,      // GPIO12 I2C_SDA
    GPIO_SWT1_NP_ID,      // GPIO13 Switch1n
    GPIO_I2C_SCL_ID,      // GPIO14 I2C_SCL
    GPIO_REL2_ID,         // GPIO15 Relay2
    GPIO_ADE7953_IRQ_ID,  // GPIO16 ADE7953_IRQ
    GPIO_FLAG_ADC0_TEMP   // GPIOA0 Tempurature
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  {      //MODULE_SHELLY_DIMMER2_ID                      // SHELLY_DIMMER2 - Shelly Dimmer 2 (ESP8285)
    0,                         // GPIO00 None
    GPIO_HWSERIAL0_TX_ID,      // GPIO01 ESP_TXD Serial RXD connection to Dimmer MCU
    0,                         // GPIO02 None 
    GPIO_HWSERIAL0_RX_ID,      // GPIO03 ESP_RXD Serial TXD connection to Dimmer MCU
    GPIO_SHELLY2_SHD_BOOT0_ID, // GPIO04 SHD Boot 0 
    GPIO_SHELLY2_SHD_RESET_INV_ID, // GPIO05 SHD Reset
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                         // GPIO09 None 
    0,                         // GPIO10 None 
                               // GPIO11 (SD_CMD   Flash)
    GPIO_SWT2_NP_ID,           // GPIO12 Switch2n 
    0,                         // GPIO13 None 
    GPIO_SWT1_NP_ID,           // GPIO14 Switch1n 
    0,                         // GPIO15 None
    GPIO_LED1_INV_ID,          // GPIO16 Led1i 
    GPIO_FLAG_ADC0_TEMP        // ADC Temperature
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  {          //MODULE_SONOFF_IFAN03_ID                  // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
    GPIO_KEY1_ID,              // GPIO00 WIFI_KEY0 Button 1
    GPIO_HWSERIAL0_TX_ID,      // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                         // GPIO02 ESP_LOG
    GPIO_HWSERIAL0_RX_ID,      // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                         // GPIO04 DEBUG_RX
    0,                         // GPIO05 DEBUG_TX
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_REL1_INV_ID,          // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    GPIO_BUZZER_INV_ID,        // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                               // GPIO11 (SD_CMD   Flash)
    GPIO_REL3_ID,              // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_LED1_INV_ID,          // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_REL2_ID,              // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    GPIO_REL4_ID,              // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0,                         // GPIO16 None 
    0                          // A0
  }
  #endif
};


/*********************************************************************************************\
 * Templates with 14 usable pins (ESP8285)
\*********************************************************************************************/

// const mytmplt8285 kModules8285[TMP_MAXMODULE_8266 - TMP_WEMOS] PROGMEM = {
//   {                     // WEMOS - Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
//     GPI8_USER,          // GPIO00 D3 Wemos Button Shield
//     GPI8_USER,          // GPIO01 TX Serial RXD
//     GPI8_USER,          // GPIO02 D4 Wemos DHT Shield
//     GPI8_USER,          // GPIO03 RX Serial TXD and Optional sensor
//     GPI8_USER,          // GPIO04 D2 Wemos I2C SDA
//     GPI8_USER,          // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
//                         // GPIO06 (SD_CLK   Flash)
//                         // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                         // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//     GPI8_USER,          // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//     GPI8_USER,          // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                         // GPIO11 (SD_CMD   Flash)
//     GPI8_USER,          // GPIO12 D6
//     GPI8_USER,          // GPIO13 D7
//     GPI8_USER,          // GPIO14 D5
//     GPI8_USER,          // GPIO15 D8
//     GPI8_USER,          // GPIO16 D0 Wemos Wake
//     GPI8_USER           // ADC0 A0 Analog input
//   },
//   {                     // SONOFF_4CH - Sonoff 4CH (ESP8285)
//     GPI8_KEY1,          // GPIO00 Button 1
//     GPI8_USER,          // GPIO01 Serial RXD and Optional sensor
//     GPI8_USER,          // GPIO02 Optional sensor
//     GPI8_USER,          // GPIO03 Serial TXD and Optional sensor
//     GPI8_REL3,          // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
//     GPI8_REL2,          // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
//                         // GPIO06 (SD_CLK   Flash)
//                         // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                         // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//     GPI8_KEY2,          // GPIO09 Button 2
//     GPI8_KEY3,          // GPIO10 Button 3
//                         // GPIO11 (SD_CMD   Flash)
//     GPI8_REL1,          // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
//     GPI8_LED1_INV,      // GPIO13 Blue Led (0 = On, 1 = Off)
//     GPI8_KEY4,          // GPIO14 Button 4
//     GPI8_REL4,          // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
//     0, 0
//   },
//   {                     // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
//     GPI8_KEY1,          // GPIO00 WIFI_KEY0 Button 1
//     GPI8_TXD,           // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
//     0,                  // GPIO02 ESP_LOG
//     GPI8_RXD,           // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
//     0,                  // GPIO04 DEBUG_RX
//     0,                  // GPIO05 DEBUG_TX
//                         // GPIO06 (SD_CLK   Flash)
//                         // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                         // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//     GPI8_REL1_INV,      // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
//     GPI8_BUZZER_INV,    // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
//                         // GPIO11 (SD_CMD   Flash)
//     GPI8_REL3,          // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
//     GPI8_LED1_INV,      // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
//     GPI8_REL2,          // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
//     GPI8_REL4,          // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
//     0, 0
//   }
// };

#endif // ESP8266


#ifdef ESP32
/********************************************************************************************\
 * ESP32 Module templates
\********************************************************************************************/

#define USER_MODULE        255

#define USE_MODULE_TEMPLATE_CAM_AITHINKER

// Supported hardware modules
enum SupportedModules {
  DOIT_DEVKIT,
  MODULE_CAM_AITHINKER_ID,
  // WEMOS,
  // ESP32_CAM_AITHINKER,
  // ODROID_GO,
  // ESP32_SOLO,
  // WT32_ETH01,
  // TTGO_WATCH,
  // M5STACK_CORE2,
  MAXMODULE 
  };

#ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  #define            D_MODULE_NAME_CAM_AITHINKER_CTR          "Webcam AIThinker"
  DEFINE_PGM_CTR(PM_MODULE_NAME_CAM_AITHINKER_CTR)        D_MODULE_NAME_CAM_AITHINKER_CTR;
#endif // USE_MODULE_TEMPLATE_CAM_AITHINKER

// Default module settings
const uint8_t kModuleNiceList[] PROGMEM = {
  DOIT_DEVKIT,
#ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  MODULE_CAM_AITHINKER_ID,
#endif  // USE_MODULE_TEMPLATE_CAM_AITHINKER
// #ifdef USE_ODROID_GO
//   ODROID_GO,
// #endif  // USE_ODROID_GO
// #ifdef USE_ESP32_SOLO
// //  ESP32_SOLO,                // To be defined
// #endif  // USE_ESP32_SOLO
// #ifdef USE_WT32_ETH01
//   WT32_ETH01,
// #endif  // USE_WT32_ETH01
// #ifdef USE_TTGO_WATCH
// //  TTGO_WATCH,                // To be defined
// #endif  // USE_TTGO_WATCH
// #ifdef USE_M5STACK_CORE2
//   M5STACK_CORE2,
// #endif  // USE_M5STACK_CORE2
};

// !!! Update this list in the same order as kModuleNiceList !!!
const char kModuleNames[] PROGMEM =
  "ESP32-DevKit|"
#ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  D_MODULE_NAME_CAM_AITHINKER_CTR "|"
#endif  // USE_MODULE_TEMPLATE_CAM_AITHINKER
// #ifdef USE_ODROID_GO
//   "Odroid Go|"
// #endif  // USE_ODROID_GO
// #ifdef USE_ESP32_SOLO
// //  "ESP32-Solo|"              // To be defined
// #endif  // USE_ESP32_SOLO
// #ifdef USE_WT32_ETH01
//   "WT32-Eth01|"
// #endif  // USE_WT32_ETH01
// #ifdef USE_TTGO_WATCH
// //  "TTGO Watch|"              // To be defined
// #endif  // USE_TTGO_WATCH
// #ifdef USE_M5STACK_CORE2
//   "M5Stack Core2|"
// #endif  // USE_M5STACK_CORE2
  ;

// !!! Update this list in the same order as SupportedModules !!!
const mytmplt kModules[] PROGMEM = {
  {                              // WEMOS - Espressif ESP32-DevKitC - Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
    GPIO_USER_ID,            // 0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
    GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
    GPIO_USER_ID,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
    GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
    GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
    GPIO_USER_ID,            // 5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
                                 // 6       IO                  GPIO6, Flash CLK
                                 // 7       IO                  GPIO7, Flash D0
                                 // 8       IO                  GPIO8, Flash D1
    GPIO_USER_ID,            // 9       IO                  GPIO9, Flash D2, U1RXD
    GPIO_USER_ID,            // 10      IO                  GPIO10, Flash D3, U1TXD
                                 // 11      IO                  GPIO11, Flash CMD
    GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
    GPIO_USER_ID,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
    GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
    GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
    GPIO_USER_ID,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
    GPIO_USER_ID,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
    GPIO_USER_ID,            // 18      IO                  GPIO18, VSPICLK, HS1_DATA7
    GPIO_USER_ID,            // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
    0,                           // 20
    GPIO_USER_ID,            // 21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
    GPIO_USER_ID,            // 22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
    GPIO_USER_ID,            // 23      IO                  GPIO23, VSPID, HS1_STROBE
    0,                           // 24
    GPIO_USER_ID,            // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
    GPIO_USER_ID,            // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
    GPIO_USER_ID,            // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
    0,                           // 28
    0,                           // 29
    0,                           // 30
    0,                           // 31
    GPIO_USER_ID,            // 32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
    GPIO_USER_ID,            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
    GPIO_USER_ID,            // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
    GPIO_USER_ID,            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
    GPIO_USER_ID,            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
    0,                           // 37          NO PULLUP
    0,                           // 38          NO PULLUP
    GPIO_USER_ID,            // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
    0                            // Flag
  },
  #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  {                              // ESP32_CAM_AITHINKER - Any ESP32 device with webcam (ESP32)
    GPIO_WEBCAM_XCLK_ID,     // 0       (I)O                GPIO0, CAM_XCLK
    GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
    GPIO_USER_ID,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
    GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
    GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
    GPIO_WEBCAM_DATA1_ID,     // 5       IO                  GPIO5, CAM_DATA1
                                 // 6       IO                  GPIO6, Flash CLK
                                 // 7       IO                  GPIO7, Flash D0
                                 // 8       IO                  GPIO8, Flash D1
    GPIO_USER_ID,            // 9       IO                  GPIO9, Flash D2, U1RXD
    GPIO_USER_ID,            // 10      IO                  GPIO10, Flash D3, U1TXD
                                 // 11      IO                  GPIO11, Flash CMD
    GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
    GPIO_USER_ID,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
    GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
    GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
    GPIO_USER_ID,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
    GPIO_USER_ID,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
    GPIO_WEBCAM_DATA2_ID,  // 18      IO                  GPIO18, CAM_DATA2
    GPIO_WEBCAM_DATA3_ID,  // 19      IO                  GPIO19, CAM_DATA3
    0,                           // 20
    GPIO_WEBCAM_DATA4_ID,  // 21      IO                  GPIO21, CAM_DATA4
    GPIO_WEBCAM_PCLK_ID,     // 22      IO      LED         GPIO22, CAM_PCLK
    GPIO_WEBCAM_HREF_ID,     // 23      IO                  GPIO23, CAM_HREF
    0,                           // 24
    GPIO_WEBCAM_VSYNC_ID,    // 25      IO                  GPIO25, CAM_VSYNC
    GPIO_WEBCAM_SIOD_ID,     // 26      IO                  GPIO26, CAM_SIOD
    GPIO_WEBCAM_SIOC_ID,     // 27      IO                  GPIO27, CAM_SIOC
    0,                           // 28
    0,                           // 29
    0,                           // 30
    0,                           // 31
    GPIO_WEBCAM_PWDN_ID,     // 32      IO                  GPIO32, CAM_PWDN
    GPIO_USER_ID,            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
    GPIO_WEBCAM_DATA7_ID,  // 34      I   NO PULLUP       GPIO34, CAM_DATA7
    GPIO_WEBCAM_DATA8_ID,  // 35      I   NO PULLUP       GPIO35, CAM_DATA8
    GPIO_WEBCAM_DATA5_ID,  // 36      I   NO PULLUP       GPIO36, CAM_DATA5
    0,                           // 37          NO PULLUP
    0,                           // 38          NO PULLUP
    GPIO_WEBCAM_DATA6_ID,  // 39      I   NO PULLUP       GPIO39, CAM_DATA6
    0                            // Flag
  },
  #endif  // USE_MODULE_TEMPLATE_CAM_AITHINKER

// #ifdef USE_ODROID_GO
//   {                              // ODROID_GO - (ESP32)
//     GPIO_KEY1),            // 0       (I)O                GPIO0, BTN-VOLUME
//     GPIO_TXD),             // 1       IO     TXD0         GPIO1, TXD0
//     GPIO_LEDLNK),          // 2       IO                  GPIO2, STATUS LED
//     GPIO_RXD),             // 3       IO     RXD0         GPIO3, RXD0
//     GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
//     GPIO_ILI9341_CS),      // 5       IO                  GPIO5, VSPI_CS0_LCD
//                                  // 6       IO                  GPIO6, Flash CLK
//                                  // 7       IO                  GPIO7, Flash D0
//                                  // 8       IO                  GPIO8, Flash D1
//     0,                           // 9       IO                  GPIO9, Flash D2, U1RXD
//     0,                           // 10      IO                  GPIO10, Flash D3, U1TXD
//                                  // 11      IO                  GPIO11, Flash CMD
//     GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
//     GPIO_KEY1) +1,         // 13      IO                  GPIO13, BTN-MENU
//     GPIO_PWM1),            // 14      IO                  GPIO14, LCD Backlight
//     GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
//     GPIO_USER_ID,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
//     GPIO_USER_ID,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
//     GPIO_SPI_CLK),         // 18      IO                  GPIO18, VSPI_CLK
//     GPIO_SPI_MISO),        // 19      IO                  GPIO19, VSPI_MISO
//     0,                           // 20
//     GPIO_ILI9341_DC),      // 21      IO                  GPIO21, SPI_DC_LCD
//     GPIO_SDCARD_CS),       // 22      IO      LED         GPIO22, VSPI_CS1_TFLASH
//     GPIO_SPI_MOSI),        // 23      IO                  GPIO23, VSPI_MOSI
//     0,                           // 24
//     0,                           // 25      IO                  GPIO25, DAC_1 (PAM8304A)
//     0,                           // 26      IO                  GPIO26, DAC_2 (PAM8304A)
//     GPIO_KEY1) +2,         // 27      IO                  GPIO27, BTN-SELECT
//     0,                           // 28
//     0,                           // 29
//     0,                           // 30
//     0,                           // 31
//     GPIO_SWT1) +4,         // 32      IO                  GPIO32, BTN-A
//     GPIO_SWT1) +5,         // 33      IO                  GPIO33, BTN-B
//     GPIO_ADC_JOY),         // 34      I   NO PULLUP       GPIO34, JOY-X (LEFT-RIGHT)
//     GPIO_ADC_JOY) +1,      // 35      I   NO PULLUP       GPIO35, JOY-Y (UP-DOWN)
//     GPIO_ADC_RANGE) +2,    // 36      I   NO PULLUP       GPIO36, SENSOR_VP (BATTERY CARGER)
//     0,                           // 37          NO PULLUP
//     0,                           // 38          NO PULLUP
//     GPIO_KEY1) +3,         // 39      I   NO PULLUP       GPIO39, BTN-START
//     0                            // Flag
//   },
// #endif  // USE_ODROID_GO

// #ifdef USE_ESP32_SOLO
// //  {                              // ESP32 Solo (ESP32) - To be defined
// //  },
// #endif  // USE_ESP32_SOLO

// #ifdef USE_WT32_ETH01
//   {                              // WT32_ETH01 - (ESP32)
//     0,                           // 0       (I)O                GPIO0, Ethernet EMAC_REF_CLK
//     GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
//     GPIO_USER_ID,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
//     GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
//     GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
//     GPIO_USER_ID,            // 5       IO                  GPIO5, RXD Led green
//                                  // 6       IO                  GPIO6, Flash CLK
//                                  // 7       IO                  GPIO7, Flash D0
//                                  // 8       IO                  GPIO8, Flash D1
//     0,                           // 9       IO                  GPIO9, Flash D2, U1RXD
//     0,                           // 10      IO                  GPIO10, Flash D3, U1TXD
//                                  // 11      IO                  GPIO11, Flash CMD
//     GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
//     0,                           // 13      IO                  GPIO13, Ethernet EMAC_RX_ER
//     GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
//     GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
//     GPIO_OUTPUT_HI),       // 16      IO                  GPIO16, Ethernet OSC_ENA
//     GPIO_LEDLNK_INV),      // 17      IO                  GPIO17, Network link led (green)
//     GPIO_ETH_PHY_MDIO),    // 18      IO                  GPIO18, Ethernet MDIO
//     0,                           // 19      IO                  GPIO19, Ethernet TXD0
//     0,                           // 20
//     0,                           // 21      IO                  GPIO21, Ethernet EMAC_TX_EN
//     0,                           // 22      IO      LED         GPIO22, Ethernet EMAC_TXD1
//     GPIO_ETH_PHY_MDC),     // 23      IO                  GPIO23, Ethernet MDC
//     0,                           // 24
//     0,                           // 25      IO                  GPIO25, Ethernet EMAC_RXD0
//     0,                           // 26      IO                  GPIO26, Ethernet EMAC_RXD1
//     0,                           // 27      IO                  GPIO27, Ethernet EMAC_RX_DV
//     0,                           // 28
//     0,                           // 29
//     0,                           // 30
//     0,                           // 31
//     GPIO_USER_ID,            // 32      IO                  GPIO32, CFG
//     GPIO_USER_ID,            // 33      IO                  GPIO33, 485_EN
//     0,                           // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
//     GPIO_USER_ID,            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
//     GPIO_USER_ID,            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
//     0,                           // 37          NO PULLUP
//     0,                           // 38          NO PULLUP
//     GPIO_USER_ID,            // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
//     0                            // Flag
//   },
// #endif  // USE_WT32_ETH01

// #ifdef USE_TTGO_WATCH
// //  {                              // TTGO Watch (ESP32) - To be defined
// //  },
// #endif  // USE_TTGO_WATCH

// #ifdef USE_M5STACK_CORE2
//   {                              // M5STACK CORE2 - (ESP32)
//     GPIO_USER_ID,            // 0       (I)O                GPIO0, SPKR_LRCK
//     GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD
//     GPIO_USER_ID,            // 2       IO                  GPIO2, SPKR_DATA
//     GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD
//     GPIO_SDCARD_CS),       // 4       IO                  GPIO4, SPI_CS_CARD
//     GPIO_ILI9341_CS),      // 5       IO                  GPIO5, SPI_CS_LCD
//                                  // 6       IO                  GPIO6, Flash CLK
//                                  // 7       IO                  GPIO7, Flash D0
//                                  // 8       IO                  GPIO8, Flash D1
//     0,                           // 9       IO                  GPIO9, Flash D2, PSRAM_D3
//     0,                           // 10      IO                  GPIO10, Flash D3, PSRAM_D2
//                                  // 11      IO                  GPIO11, Flash CMD
//     0,                           // 12      (I)O                GPIO12, SPKR_CLK
//     GPIO_USER_ID,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
//     GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
//     GPIO_ILI9341_DC),      // 15      (I)O                GPIO15, SPI_DC_LCD
//     0,                           // 16      IO                  GPIO16, PSRAM_CS
//     0,                           // 17      IO                  GPIO17, PSRAM_CLK
//     GPIO_SPI_CLK),         // 18      IO                  GPIO18, SPI_CLK
//     GPIO_USER_ID,            // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
//     0,                           // 20
//     0,                           // 21      IO                  GPIO21, I2C_SDA_INTERNAL
//     0,                           // 22      IO      LED         GPIO22, I2C_SCL_INTERNAL
//     GPIO_SPI_MOSI),        // 23      IO                  GPIO23, SPI_MOSI
//     0,                           // 24
//     GPIO_USER_ID,            // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
//     GPIO_USER_ID,            // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
//     GPIO_USER_ID,            // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
//     0,                           // 28
//     0,                           // 29
//     0,                           // 30
//     0,                           // 31
//     GPIO_I2C_SDA),         // 32      IO                  GPIO32, I2C_SDA
//     GPIO_I2C_SCL),         // 33      IO                  GPIO33, I2C_SCL
//     GPIO_USER_ID,            // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
//     GPIO_USER_ID,            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
//     GPIO_USER_ID,            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
//     0,                           // 37          NO PULLUP
//     GPIO_SPI_MISO),        // 38          NO PULLUP       GPIO38, SPI_MISO
//     0,                           // 39      I   NO PULLUP       GPIO39, INT_TOUCHPAD
//     0                            // Flag
//   }
// #endif  // USE_M5STACK_CORE2
};

/*********************************************************************************************\
 Known templates

{"NAME":"AITHINKER CAM","GPIO":[4992,1,1,1,1,5088,1,1,1,1,1,1,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":1}
{"NAME":"Olimex ESP32-PoE","GPIO":[1,1,1,1,1,1,0,0,5536,1,1,1,1,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
{"NAME":"wESP32","GPIO":[1,1,1,1,1,1,0,0,0,1,1,1,5568,5600,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
{"NAME":"Denky (Teleinfo)","GPIO":[1,1,1,1,5664,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1376,1,1,0,0,0,0,1,5632,1,1,1,0,0,1],"FLAG":0,"BASE":1}

\*********************************************************************************************/

#endif  // ESP32



#endif  // _mHardwareTemplates_H_