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
  GPIO_PZEM0XX_TX_ID, GPIO_PZEM004_RX_ID, GPIO_PZEM016_RX_ID, GPIO_PZEM017_RX_ID,
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
  GPIO_SERIAL_DEBUG_TX_ID, GPIO_SERIAL_DEBUG_RX_ID,
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
  GPIO_PZEM016_RX_ID,     // PZEM-014_ID,016 Serial Modbus interface
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
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PZEM016_RX_CTR)     D_GPIO_FUNCTION_PZEM016_RX_CTR;
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
//   AGPIO(GPIO_ADC_INPUT),                  // Analog inputs
//   AGPIO(GPIO_ADC_TEMP),                   // Thermistor
//   AGPIO(GPIO_ADC_LIGHT),                  // Light sensor
//   AGPIO(GPIO_ADC_BUTTON) + MAX_KEYS,      // Button
//   AGPIO(GPIO_ADC_BUTTON_INV) + MAX_KEYS,
//   AGPIO(GPIO_ADC_RANGE),                  // Range
//   AGPIO(GPIO_ADC_CT_POWER),               // Current
//   AGPIO(GPIO_ADC_JOY),                    // Joystick
//   AGPIO(GPIO_ADC_PH),                     // Analog PH Sensor
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



#endif  // _mHardwareTemplates_H_