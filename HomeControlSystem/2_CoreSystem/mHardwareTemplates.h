#ifndef _mHardwareTemplates2_H_
#define _mHardwareTemplates2_H_

#include "stdint.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/mGlobalMacros.h"

/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/**********GPIO FUNCTIONS CONFIG**********************************************************************************/
/***
 * INV = Inverted pin, low/ground is active
 * NP  = No pullup/down 
 * PD = Pulled down (hardware dependant)
 * */
enum USER_SELECTABLE_PINS_StaticCompleteList_IDS {
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
  GPIO_BUZZER, GPIO_BUZZER_INV,
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
  
  
  GPIO_PULSE_COUNTER_ID,         // 
  GPIO_FAN_IRSEND_ID,        // Calls ceiling fan class to use IR sender
  GPIO_SENSOR_END_ID 
}; // used 171of 250

  
// enum UserSelectablePins {
//   GPIO_NONE,                           // Not used
//   GPIO_KEY1, GPIO_KEY1_NP, GPIO_KEY1_INV, GPIO_KEY1_INV_NP, // 4 x Button
//   GPIO_SWT1, GPIO_SWT1_NP,             // 8 x User connected external switches
//   GPIO_REL1, GPIO_REL1_INV,            // 8 x Relays
//   GPIO_LED1, GPIO_LED1_INV,            // 4 x Leds
//   GPIO_CNTR1, GPIO_CNTR1_NP,           // 4 x Counter
//   GPIO_PWM1, GPIO_PWM1_INV,            // 5 x PWM
//   GPIO_BUZZER, GPIO_BUZZER_INV,        // Buzzer
//   GPIO_LEDLNK, GPIO_LEDLNK_INV,        // Link led
//   GPIO_I2C_SCL, GPIO_I2C_SDA,          // Software I2C
//   GPIO_SPI_MISO, GPIO_SPI_MOSI, GPIO_SPI_CLK, GPIO_SPI_CS, GPIO_SPI_DC,        // Hardware SPI
//   GPIO_SSPI_MISO, GPIO_SSPI_MOSI, GPIO_SSPI_SCLK, GPIO_SSPI_CS, GPIO_SSPI_DC,  // Software SPI
//   GPIO_BACKLIGHT,                      // Display backlight control
//   GPIO_OLED_RESET,                     // OLED Display Reset
//   GPIO_IRSEND, GPIO_IRRECV,            // IR interface
//   GPIO_RFSEND, GPIO_RFRECV,            // RF interface
//   GPIO_DHT11, GPIO_DHT22, GPIO_SI7021, GPIO_DHT11_OUT,  // DHT11, DHT21, DHT22, AM2301, AM2302, AM2321
//   GPIO_DSB, GPIO_DSB_OUT,              // DS18B20 or DS18S20
//   GPIO_WS2812,                         // WS2812 Led string
//   GPIO_MHZ_TXD, GPIO_MHZ_RXD,          // MH-Z19 Serial interface
//   GPIO_PZEM0XX_TX, GPIO_PZEM004_RX, GPIO_PZEM016_RX, GPIO_PZEM017_RX, // PZEM Serial Modbus interface
//   GPIO_SAIR_TX, GPIO_SAIR_RX,          // SenseAir Serial interface
//   GPIO_PMS5003_TX, GPIO_PMS5003_RX,    // Plantower PMS5003 Serial interface
//   GPIO_SDS0X1_TX, GPIO_SDS0X1_RX,      // Nova Fitness SDS011 Serial interface
//   GPIO_SBR_TX, GPIO_SBR_RX,            // Serial Bridge Serial interface
//   GPIO_SR04_TRIG, GPIO_SR04_ECHO,      // SR04 interface
//   GPIO_SDM120_TX, GPIO_SDM120_RX,      // SDM120 Serial interface
//   GPIO_SDM630_TX, GPIO_SDM630_RX,      // SDM630 Serial interface
//   GPIO_TM16CLK, GPIO_TM16DIO, GPIO_TM16STB,  // TM1638 interface
//   GPIO_MP3_DFR562,                     // RB-DFR-562, DFPlayer Mini MP3 Player
//   GPIO_HX711_SCK, GPIO_HX711_DAT,      // HX711 Load Cell interface
//   GPIO_TX2X_TXD_BLACK,                 // TX20/TX23 Transmission Pin
//   GPIO_TUYA_TX, GPIO_TUYA_RX,          // Tuya Serial interface
//   GPIO_MGC3130_XFER, GPIO_MGC3130_RESET,  // MGC3130 interface
//   GPIO_RF_SENSOR,                      // Rf receiver with sensor decoding
//   GPIO_AZ_TXD, GPIO_AZ_RXD,            // AZ-Instrument 7798 Serial interface
//   GPIO_MAX31855CS, GPIO_MAX31855CLK, GPIO_MAX31855DO,  // MAX31855 Serial interface
//   GPIO_NRG_SEL, GPIO_NRG_SEL_INV, GPIO_NRG_CF1, GPIO_HLW_CF, GPIO_HJL_CF,  // HLW8012/HJL-01/BL0937 energy monitoring
//   GPIO_MCP39F5_TX, GPIO_MCP39F5_RX, GPIO_MCP39F5_RST,  // MCP39F501 Energy monitoring (Shelly2)
//   GPIO_PN532_TXD, GPIO_PN532_RXD,      // PN532 NFC Serial interface
//   GPIO_SM16716_CLK, GPIO_SM16716_DAT, GPIO_SM16716_SEL,  // SM16716 SELECT
//   GPIO_DI, GPIO_DCKI,                  // my92x1 PWM controller
//   GPIO_CSE7766_TX, GPIO_CSE7766_RX,    // CSE7766 Serial interface (S31 and Pow R2)
//   GPIO_ARIRFRCV, GPIO_ARIRFSEL,        // Arilux RF Receive input
//   GPIO_TXD, GPIO_RXD,                  // Serial interface
//   GPIO_ROT1A, GPIO_ROT1B, GPIO_ROT2A, GPIO_ROT2B,  // Rotary switch
//   GPIO_HRE_CLOCK, GPIO_HRE_DATA,       // HR-E Water Meter
//   GPIO_ADE7953_IRQ,                    // ADE7953 IRQ
//   GPIO_SOLAXX1_TX, GPIO_SOLAXX1_RX,    // Solax Inverter Serial interface
//   GPIO_ZIGBEE_TX, GPIO_ZIGBEE_RX,      // Zigbee Serial interface
//   GPIO_RDM6300_RX,                     // RDM6300 RX
//   GPIO_IBEACON_TX, GPIO_IBEACON_RX,    // HM17 IBEACON Serial interface
//   GPIO_A4988_DIR, GPIO_A4988_STP, GPIO_A4988_ENA,  // A4988 interface
//   GPIO_A4988_MS1, GPIO_A4988_MS2, GPIO_A4988_MS3,  // A4988 microstep
//   GPIO_DDS2382_TX, GPIO_DDS2382_RX,    // DDS2382 Serial interface
//   GPIO_DDSU666_TX, GPIO_DDSU666_RX,    // DDSU666 Serial interface
//   GPIO_SM2135_CLK, GPIO_SM2135_DAT,    // SM2135 PWM controller
//   GPIO_DEEPSLEEP,                      // Kill switch for deepsleep
//   GPIO_EXS_ENABLE,                     // EXS MCU Enable
//   GPIO_TASMOTACLIENT_TXD, GPIO_TASMOTACLIENT_RXD,      // Client Serial interface
//   GPIO_TASMOTACLIENT_RST, GPIO_TASMOTACLIENT_RST_INV,  // Client Reset
//   GPIO_HPMA_RX, GPIO_HPMA_TX,          // Honeywell HPMA115S0 Serial interface
//   GPIO_GPS_RX, GPIO_GPS_TX,            // GPS Serial interface
//   GPIO_HM10_RX, GPIO_HM10_TX,          // HM10-BLE-Mijia-bridge Serial interface
//   GPIO_LE01MR_RX, GPIO_LE01MR_TX,      // F&F LE-01MR energy meter
//   GPIO_CC1101_GDO0, GPIO_CC1101_GDO2,  // CC1101 Serial interface
//   GPIO_HRXL_RX,                        // Data from MaxBotix HRXL sonar range sensor
//   GPIO_ELECTRIQ_MOODL_TX,              // ElectriQ iQ-wifiMOODL Serial TX
//   GPIO_AS3935,
//   ADC0_INPUT,                          // Analog input
//   ADC0_TEMP,                           // Analog Thermistor
//   ADC0_LIGHT,                          // Analog Light sensor
//   ADC0_BUTTON, ADC0_BUTTON_INV,        // Analog Button
//   ADC0_RANGE,                          // Analog Range
//   ADC0_CT_POWER,                       // ANalog Current
//   GPIO_WEBCAM_PWDN, GPIO_WEBCAM_RESET, GPIO_WEBCAM_XCLK,  // Webcam
//   GPIO_WEBCAM_SIOD, GPIO_WEBCAM_SIOC,  // Webcam I2C
//   GPIO_WEBCAM_DATA,
//   GPIO_WEBCAM_VSYNC, GPIO_WEBCAM_HREF, GPIO_WEBCAM_PCLK,
//   GPIO_WEBCAM_PSCLK,
//   GPIO_WEBCAM_HSD,
//   GPIO_WEBCAM_PSRCS,
//   GPIO_BOILER_OT_RX, GPIO_BOILER_OT_TX,  // OpenTherm Boiler TX pin
//   GPIO_WINDMETER_SPEED,                // WindMeter speed counter pin
//   GPIO_KEY1_TC,                        // Touch pin as button
//   GPIO_BL0940_RX,                      // BL0940 serial interface
//   GPIO_TCP_TX, GPIO_TCP_RX,            // TCP to serial bridge
//   GPIO_ETH_PHY_POWER, GPIO_ETH_PHY_MDC, GPIO_ETH_PHY_MDIO,  // Ethernet
//   GPIO_TELEINFO_RX,                    // Teleinfo telemetry data receive pin
//   GPIO_TELEINFO_ENABLE,                // Teleinfo Enable Receive Pin
//   GPIO_LMT01,                          // LMT01 input counting pin
//   GPIO_SENSOR_END };

// Programmer selectable GPIO functionality
enum ProgramSelectablePins {
  GPIO_FIX_START_ID = 251,
  GPIO_SPI_MISO_ID,       // SPI MISO library fixed pin GPIO12
  GPIO_SPI_MOSI_ID,       // SPI MOSI library fixed pin GPIO13
  GPIO_SPI_CLK_ID,        // SPI Clk library fixed pin GPIO14
  GPIO_USER_ID,           // User configurable needs to be 255
  GPIO_MAX_ID };

// List of function IDs, showing only what is enabled by build type
const uint8_t kList_Selectable_Function_IDs[] PROGMEM = {
  GPIO_NONE_ID,         // Not used
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
#ifdef USE_BUZZER
  GPIO_BUZZER,         // Buzzer
  GPIO_BUZZER_INV,     // Inverted buzzer
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
#if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
  GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
  GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
  GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
  GPIO_HLW_CF_ID,         // HLW8012 CF power
  GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_I2C) && defined(USE_ADE7953)
  GPIO_ADE7953_IRQ_ID,    // ADE7953 IRQ
#endif
#if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
  GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
  GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
#endif
// #if defined(USE_MODULE_ENERGY_PZEM004T_MODBUS) || defined(USE_PZEM_DC)
  GPIO_PZEM0XX_TX_ID,     // PZEM0XX Serial interface
// #endif
// #ifdef USE_PZEM004T
  GPIO_PZEM004_RX_ID,     // PZEM004T Serial interface
// #endif
// #ifdef USE_MODULE_ENERGY_PZEM004T_MODBUS
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

// #if defined(USE_MODULE_ENERGY_PZEM004T_MODBUS) || defined(USE_PZEM_DC)
DEFINE_PGM_CTR(PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)     D_GPIO_FUNCTION_PZEM0XX_TX_CTR;
// #endif
// #ifdef USE_MODULE_ENERGY_PZEM004T_MODBUS
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
#define USE_MODULE_TEMPLATE_SHELLY2P5
#define USE_MODULE_TEMPLATE_SONOFF_IFAN03

// Supported hardware modules. Leave completed list
enum SupportedModules_StaticCompleteList {
  MODULE_WEMOS_ID,
  MODULE_SONOFF_BASIC_ID,
  MODULE_H801_ID,
  MODULE_MAGICHOME_ID,
  MODULE_SHELLY1_ID,
  MODULE_SHELLY2P5_ID,
  MODULE_SONOFF_IFAN03_ID,

  // MODULE_SONOFF_RF_ID,
  // MODULE_SONOFF_SV_ID,
  // MODULE_SONOFF_TH_ID,
  // MODULE_SONOFF_DUAL_ID,
  // MODULE_SONOFF_POW_ID,
  // MODULE_SONOFF_4CH_ID,
  // MODULE_SONOFF_S2X_ID,
  // MODULE_SONOFF_TOUCH_ID,
  // MODULE_SONOFF_LED_ID,
  // MODULE_CH1_ID,
  // MODULE_CH4_ID,
  // MODULE_SONOFF_SC_ID,
  // MODULE_SONOFF_BN_ID,
  // MODULE_SONOFF_4CHPRO_ID,
  // MODULE_HUAFAN_SS_ID,
  // MODULE_SONOFF_BRIDGE_ID,
  // MODULE_SONOFF_B1_ID,
  // MODULE_AILIGHT_ID,
  // MODULE_SONOFF_T11_ID,
  // MODULE_SONOFF_T12_ID,
  // MODULE_SONOFF_T13_ID,
  // MODULE_SUPLA1_ID,
  // MODULE_WITTY_ID,
  // MODULE_YUNSHAN_ID,
  // MODULE_LUANIHVIO_ID,
  // MODULE_KMC_70011_ID,
  // MODULE_ARILUX_LC01_ID,
  // MODULE_ARILUX_LC11_ID,
  // MODULE_SONOFF_DUAL_R2_ID,
  // MODULE_ARILUX_LC06_ID,
  // MODULE_SONOFF_S31_ID,
  // MODULE_ZENGGE_ZF_WF017_ID,
  // MODULE_SONOFF_POW_R2_ID,
  // MODULE_SONOFF_IFAN02_ID,
  // MODULE_BLITZWOLF_BWSHP_ID,
  // MODULE_PHILIPS_ID,
  // MODULE_NEO_COOLCAM_ID,
  // MODULE_ESP_SWITCH_ID,
  // MODULE_OBI_ID,
  // MODULE_TECKIN_ID,
  // MODULE_APLIC_WDP303075_ID,
  // MODULE_TUYA_DIMMER_ID,
  // MODULE_GOSUND_ID,
  // MODULE_ARMTRONIX_DIMMERS_ID,
  // MODULE_SK03_TUYA_ID,
  // MODULE_PS_16_DZ_ID,
  // MODULE_TECKIN_US_ID,
  // MODULE_MANZOKU_EU_4_ID,
  // MODULE_OBI2_ID,
  // MODULE_YTF_IR_BRIDGE_ID,
  // MODULE_DIGOO_ID,
  // MODULE_KA10_ID,
  // MODULE_ZX2820_ID,
  // MODULE_MI_DESK_LAMP_ID,
  // MODULE_SP10_ID,
  // MODULE_WAGA_ID,
  // MODULE_SYF05_ID,

  // Last module
  MODULE_NODEMCU_ID,
  MODULE_MAXMODULE };


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
#ifdef USE_MODULE_TEMPLATE_SHELLY2P5
  #define            D_MODULE_NAME_SHELLY2P5_CTR       "Shelly 2"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SHELLY2P5_CTR)     D_MODULE_NAME_SHELLY2P5_CTR;
#endif
#ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  #define            D_MODULE_NAME_SONOFF_IFAN03_CTR       "Sonoff IFAN03"
  DEFINE_PGM_CTR(PM_MODULE_NAME_SONOFF_IFAN03_CTR)     D_MODULE_NAME_SONOFF_IFAN03_CTR;
#endif


/********************************************************************************************/

#define USER_MODULE        255

#define MAX_GPIO_PIN       17   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6_ID, 7_ID, 8 and 11)

// List of pins
typedef struct MYIO {
  uint8_t      io[MAX_GPIO_PIN];
} myio;

typedef struct MYCFGIO {
  uint8_t      io[MAX_GPIO_PIN - MIN_FLASH_PINS];
} mycfgio;

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


//#define FRIENDLY_NAME_TEMPLATE_MAX_LENGTH 20
typedef struct MYTMPLT {
  //char         name[FRIENDLY_NAME_TEMPLATE_MAX_LENGTH];  //freindly name
  mycfgio      gp;
  gpio_flag    flag;
} mytmplt;


// ID list
const uint8_t kModuleNiceList[] PROGMEM = {
  MODULE_WEMOS_ID,   //esp8282
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
    MODULE_SONOFF_BASIC_ID,        // Sonoff Relay Devices
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801  
    MODULE_H801_ID,                // Light Devices
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
    MODULE_MAGICHOME_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
    MODULE_SHELLY1_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY2P5
    MODULE_SHELLY2P5_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    MODULE_SONOFF_IFAN03_ID,        // Sonoff Relay Devices
  #endif
//   MODULE_SONOFF_RF_ID,
//   MODULE_SONOFF_SV_ID,
  //  MODULE_SONOFF_TH_ID,
  //  MODULE_SONOFF_DUAL_ID,
  //  MODULE_SONOFF_DUAL_R2_ID,
  //  MODULE_SONOFF_POW_ID,
  //  MODULE_SONOFF_POW_R2_ID,
  //  MODULE_SONOFF_4CH_ID,
  //  MODULE_SONOFF_4CHPRO_ID,
  //  MODULE_SONOFF_S31_ID,          // Sonoff Socket Relay Devices with Energy Monitoring
  //  MODULE_SONOFF_S2X_ID,          // Sonoff Socket Relay Devices
  //  MODULE_SONOFF_TOUCH_ID,        // Sonoff Switch Devices
  //  MODULE_SONOFF_T11_ID,
  //  MODULE_SONOFF_T12_ID,
  //  MODULE_SONOFF_T13_ID,
  //  MODULE_SONOFF_LED_ID,          // Sonoff Light Devices
  //  MODULE_SONOFF_BN_ID,
  //  MODULE_SONOFF_B1_ID,           // Sonoff Light Bulbs
  //  MODULE_SLAMPHER_ID,
  //  MODULE_SONOFF_SC_ID,           // Sonoff Environmemtal Sensor
  //  MODULE_SONOFF_IFAN02_ID,       // Sonoff Fan
  //  MODULE_SONOFF_BRIDGE_ID,       // Sonoff Bridge
  //  MODULE_SONOFF_SV_ID,           // Sonoff Development Devices
  //  MODULE_SONOFF_DEV_ID,
  //  MODULE_CH1_ID,                 // Relay Devices
  //  MODULE_CH4_ID,
  //  MODULE_MOTOR_ID,
  //  MODULE_ELECTRODRAGON_ID,
  //  MODULE_EXS_RELAY_ID,
  //  MODULE_SUPLA1_ID,
  //  MODULE_LUANIHVIO_ID,
  //  MODULE_YUNSHAN_ID,
  //  MODULE_WION_ID,
  //  MODULE_BLITZWOLF_BWSHP_ID,     // Socket Relay Devices with Energy Monitoring
  //  MODULE_TECKIN_ID,
  //  MODULE_TECKIN_US_ID,
  //  MODULE_APLIC_WDP303075_ID,
  //  MODULE_GOSUND_ID,
  //  MODULE_ZX2820_ID,
  //  MODULE_SK03_TUYA_ID,
  //  MODULE_DIGOO_ID,
  //  MODULE_KA10_ID,
  //  MODULE_SP10_ID,
  //  MODULE_WAGA_ID,
  //  MODULE_NEO_COOLCAM_ID,         // Socket Relay Devices
  //  MODULE_OBI_ID,
  //  MODULE_OBI2_ID,
  //  MODULE_MANZOKU_EU_4_ID,
  //  MODULE_ESP_SWITCH_ID,          // Switch Devices
  //  #ifdef USE_TUYA_DIMMER
  //  MODULE_TUYA_DIMMER_ID,         // Dimmer Devices
  //  #endif
  //  #ifdef USE_ARMTRONIX_DIMMERS
  //  MODULE_ARMTRONIX_DIMMERS_ID,
  //  #endif
  //  #ifdef USE_PS_16_DZ
  //  MODULE_PS_16_DZ_ID,
  //  #endif
  //  MODULE_ARILUX_LC01_ID,
  //  MODULE_ARILUX_LC06_ID,
  //  MODULE_ARILUX_LC11_ID,
  //  MODULE_ZENGGE_ZF_WF017_ID,
  //  MODULE_HUAFAN_SS_ID,
  //  #ifdef ROTARY_V1
  //  MODULE_MI_DESK_LAMP_ID,
  //  #endif
  //  MODULE_KMC_70011_ID,
  //  MODULE_AILIGHT_ID,             // Light Bulbs
  //  MODULE_PHILIPS_ID,
  //  MODULE_SYF05_ID,
  //  MODULE_YTF_IR_BRIDGE_ID,
  //  MODULE_WITTY_ID,               // Development Devices
  MODULE_NODEMCU_ID //esp8266
};

DEFINE_PGM_CTR(kModules_Name_list)
{
  D_MODULE_NAME_GENERIC_CTR       "|"
  D_MODULE_NAME_SONOFF_BASIC_CTR  "|"
  D_MODULE_NAME_H801_CTR          "|"
  D_MODULE_NAME_MAGICHOME_CTR     "|"
  D_MODULE_NAME_SHELLY1_CTR       "|"
  D_MODULE_NAME_SHELLY2P5_CTR     "|"
  D_MODULE_NAME_SONOFF_IFAN03_CTR "|"
};


// Move names OUT of kModule to klist style
// Default module settings
const mytmplt kModules[MODULE_MAXMODULE] PROGMEM = {
  { //D_MODULE_NAME_GENERIC_CTR,         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
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
  { //D_MODULE_NAME_SONOFF_BASIC_CTR,    // Sonoff Basic (ESP8266)
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
  { //D_MODULE_NAME_H801_CTR,            // Lixada H801 Wifi (ESP8266)
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
    0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
  { //D_MODULE_NAME_MAGICHOME_CTR,       // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
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
  { //D_MODULE_NAME_SHELLY1_CTR,        // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
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
  #ifdef USE_MODULE_TEMPLATE_SHELLY2P5  
  {  //D_MODULE_NAME_SHELLY2P5_CTR,        // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
     0,
     0,//GPIO_MCP39F5_TX_ID,  // GPIO01 MCP39F501 Serial input
     0,//GPIO_KEY1_ID,
     0,//GPIO_MCP39F5_RX_ID,  // GPIO03 MCP39F501 Serial output
     GPIO_REL1_ID,        // GPIO04
     GPIO_SWT2_NP_ID,        // GPIO05
                       // GPIO06 (SD_CLK   Flash)
                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
     0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
     0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                       // GPIO11 (SD_CMD   Flash)
     0,//GPIO_I2C_SDA_ID,        // GPIO12
     GPIO_SWT1_NP_ID,
     0,//GPIO_I2C_SCL_ID,        // GPIO14
     GPIO_REL2_ID,//GPIO_MCP39F5_RST_ID, // GPIO15 MCP39F501 Reset
     0,
     0//GPIO_SWT1_ID
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  { //D_MODULE_NAME_SONOFF_IFAN03_CTR,    // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
    GPIO_KEY1_ID,          // GPIO00 WIFI_KEY0 Button 1
    GPIO_HWSERIAL0_TX_ID,           // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                         // GPIO02 ESP_LOG
    GPIO_HWSERIAL0_RX_ID,           // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                         // GPIO04 DEBUG_RX
    0,                         // GPIO05 DEBUG_TX
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_REL1_INV_ID,      // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    255,//GPIO_BUZZER_INV_ID,    // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                               // GPIO11 (SD_CMD   Flash)
    GPIO_REL3_ID,       // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_LED1_INV_ID,      // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_REL2_ID,       // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    GPIO_REL4_ID,       // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0, 0
  },
  #endif



//   { "Sonoff RF",       // Sonoff RF (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 Optional sensor
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_USER,        // GPIO14 Optional sensor
//      0, 0, 0
//   },
//   { "Sonoff SV",       // Sonoff SV (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 Optional sensor
//      GPIO_USER,        // GPIO05 Optional sensor
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_USER,        // GPIO14 Optional sensor
//      0, 0,
//      GPIO_FLAG_ADC0    // ADC0 Analog input
//   },
//   { "Sonoff TH",       // Sonoff TH10/16 (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 Optional sensor
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_USER,        // GPIO14 Optional sensor
//      0, 0, 0
//   },
//   { "Sonoff Dual",     // Sonoff Dual (ESP8266)
//      0,
//      GPIO_TXD,         // GPIO01 Relay control
//      0,
//      GPIO_RXD,         // GPIO03 Relay control
//      GPIO_USER,        // GPIO04 Optional sensor
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0,
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_USER,        // GPIO14 Optional sensor
//      0, 0, 0
//   },
//   { "Sonoff Pow",      // Sonoff Pow (ESP8266 - HLW8012)
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0, 0,
//      GPIO_NRG_SEL,     // GPIO05 HLW8012 Sel output (1 = Voltage)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_NRG_CF1,     // GPIO13 HLW8012 CF1 voltage / current
//      GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
//      GPIO_LED1,        // GPIO15 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0
//   },
//   { "Sonoff 4CH",      // Sonoff 4CH (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button 1
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
//      GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_KEY2,        // GPIO09 Button 2
//      GPIO_KEY3,        // GPIO10 Button 3
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
//      GPIO_KEY4,        // GPIO14 Button 4
//      GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
//      0, 0
//   },
//   { "Sonoff S2X",      // Sonoff S20, S22 and S26 Smart Socket (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On) - Link and Power status
//      GPIO_LED1_INV,    // GPIO13 Green/Blue Led (0 = On, 1 = Off)
//      0, 0, 0, 0
//   },
//   { "Slampher",        // Slampher (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff Touch",    // Sonoff Touch (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On) - Link and Power status
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off)
//      0, 0, 0, 0
//   },
// // 10 below
//   { "Sonoff LED",      // Sonoff LED (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0,
//      GPIO_USER,        // GPIO04 Optional sensor (PWM3 Green)
//      GPIO_USER,        // GPIO05 Optional sensor (PWM2 Red)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM1,        // GPIO12 Cold light (PWM0 Cold)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_PWM2,        // GPIO14 Warm light (PWM1 Warm)
//      GPIO_USER,        // GPIO15 Optional sensor (PWM4 Blue)
//      0, 0
//   },
//   { "1 Channel",       // 1 Channel Inching/Latching Relay using (PSA-B01 - ESP8266 and PSF-B01 - ESP8285)
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0, 0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "4 Channel",       // 4 Channel Inching/Latching Relays (ESP8266)
//      0,
//      GPIO_TXD,         // GPIO01 Relay control
//      0,
//      GPIO_RXD,         // GPIO03 Relay control
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0,
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Motor C/AC",      // Motor Clockwise / Anti clockwise (PSA-B01 - ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0, 0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "ElectroDragon",   // ElectroDragon IoT Relay Board (ESP8266)
//      GPIO_KEY2,        // GPIO00 Button 2
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_KEY1,        // GPIO02 Button 1
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 Optional sensor
//      GPIO_USER,        // GPIO05 Optional sensor
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL2,        // GPIO12 Red Led and Relay 2 (0 = Off, 1 = On)
//      GPIO_REL1,        // GPIO13 Red Led and Relay 1 (0 = Off, 1 = On)
//      GPIO_USER,        // GPIO14 Optional sensor
//      GPIO_USER,        // GPIO15 Optional sensor
//      GPIO_LED1,        // GPIO16 Green/Blue Led (1 = On, 0 = Off) - Link and Power status
//      GPIO_FLAG_ADC0    // ADC0   A0 Analog input
//   },
//   { "EXS Relay(s)",    // ES-Store Latching relay(s) (ESP8266)
//                        // https://ex-store.de/ESP8266-WiFi-Relay-V31
//                        //   V3.1 Module Pin 1 VCC 3V3, Module Pin 6 GND
//                        // https://ex-store.de/2-Kanal-WiFi-WLan-Relay-V5-Blackline-fuer-Unterputzmontage
//      GPIO_USER,        // GPIO00 V3.1 Module Pin 8 - V5.0 Module Pin 4
//      GPIO_USER,        // GPIO01 UART0_TXD V3.1 Module Pin 2 - V5.0 Module Pin 3
//      GPIO_USER,        // GPIO02 V3.1 Module Pin 7
//      GPIO_USER,        // GPIO03 UART0_RXD V3.1 Module Pin 3
//      GPIO_USER,        // GPIO04 V3.1 Module Pin 10 - V5.0 Module Pin 2
//      GPIO_USER,        // GPIO05 V3.1 Module Pin 9 - V5.0 Module Pin 1
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Relay1 ( 1 = Off)
//      GPIO_REL2,        // GPIO13 Relay1 ( 1 = On)
//      GPIO_USER,        // GPIO14 V3.1 Module Pin 5 - V5.0 GPIO_REL3_INV Relay2 ( 1 = Off)
//      GPIO_LED1,        // GPIO15 V5.0 LED1 - Link and Power status
//      GPIO_USER,        // GPIO16 V3.1 Module Pin 4 - V5.0 GPIO_REL4_INV Relay2 ( 1 = On)
//      0
//   },
//   { "WiOn",            // Indoor Tap (ESP8266)
//                        // https://www.amazon.com/gp/product/B00ZYLUBJU/ref=s9_acsd_al_bw_c_x_3_w
//      GPIO_USER,        // GPIO00 Optional sensor (pm clock)
//      0,
//      GPIO_LED1,        // GPIO02 Green Led (1 = On, 0 = Off) - Link and Power status
//      0, 0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,        // GPIO12 Optional sensor (pm data)
//      GPIO_KEY1,        // GPIO13 Button
//      0,
//      GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
//      0, 0
//   },
// 17 below
//   ,
//   { "Sonoff Dev",      // Sonoff Dev (ESP8266)
//      GPIO_KEY1,        // GPIO00 E-FW Button
//      GPIO_USER,        // GPIO01 TX Serial RXD and Optional sensor
//      0,                // GPIO02
//      GPIO_USER,        // GPIO03 RX Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 Optional sensor
//      GPIO_USER,        // GPIO05 Optional sensor
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,        // GPIO12
//      GPIO_USER,        // GPIO13 BLUE LED
//      GPIO_USER,        // GPIO14 Optional sensor
//      0,                // GPIO15
//      0,                // GPIO16
//      GPIO_FLAG_ADC0    // ADC0 A0 Analog input
//   },
//   { "Sonoff SC",       // Sonoff SC (ESP8266)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_TXD,         // GPIO01 RXD to ATMEGA328P
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_RXD,         // GPIO03 TXD to ATMEGA328P
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0,
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff BN-SZ",    // Sonoff BN-SZ01 Ceiling led (ESP8285)
//      0, 0, 0, 0, 0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM1,        // GPIO12 Light
//      GPIO_LED1_INV,    // GPIO13 Red Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff 4CH Pro",  // Sonoff 4CH Pro (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button 1
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_REL3,        // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
//      GPIO_REL2,        // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_KEY2,        // GPIO09 Button 2
//      GPIO_KEY3,        // GPIO10 Button 3
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_KEY4,        // GPIO14 Button 4
//      GPIO_REL4,        // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
//      0, 0
//   },
//   { "Huafan SS",       // Hua Fan Smart Socket (ESP8266) - like Sonoff Pow
//      GPIO_LED1_INV,    // GPIO00 Blue Led (0 = On, 1 = Off) - Link status
//      0, 0,
//      GPIO_LED2_INV,    // GPIO03 Red Led (0 = On, 1 = Off) - Power status
//      GPIO_KEY1,        // GPIO04 Button
//      GPIO_REL1_INV,    // GPIO05 Relay (0 = On, 1 = Off)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_CF1,     // GPIO12 HLW8012 CF1 voltage / current
//      GPIO_NRG_SEL,     // GPIO13 HLW8012 Sel output (1 = Voltage)
//      GPIO_HLW_CF,      // GPIO14 HLW8012 CF power
//      0, 0, 0
//   },
//   { "Sonoff Bridge",   // Sonoff RF Bridge 433 (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_TXD,         // GPIO01 RF bridge control
//      GPIO_USER,        // GPIO02 Optional sensor
//      GPIO_RXD,         // GPIO03 RF bridge control
//      GPIO_USER,        // GPIO04 Optional sensor
//      GPIO_USER,        // GPIO05 Optional sensor
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,        // GPIO12 Optional sensor
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      GPIO_USER,        // GPIO14 Optional sensor
//      0, 0, 0
//   },
//   { "Sonoff B1",       // Sonoff B1 (ESP8285 - my9231)
//      GPIO_KEY1,        // GPIO00 Pad
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
//      GPIO_USER,        // GPIO02 Optional sensor SDA pad
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_DI,          // GPIO12 my9231 DI
//      0,
//      GPIO_DCKI,        // GPIO14 my9231 DCKI
//      0, 0, 0
//   },
//   { "AiLight",         // Ai-Thinker RGBW led (ESP8266 - my9291)
//      GPIO_KEY1,        // GPIO00 Pad
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor pad
//      GPIO_USER,        // GPIO02 Optional sensor SDA pad
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor pad
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0,
//      GPIO_DI,          // GPIO13 my9291 DI
//      0,
//      GPIO_DCKI,        // GPIO15 my9291 DCKI
//      0, 0
//   },
//   { "Sonoff T1 1CH",   // Sonoff T1 1CH (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button 1
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff T1 2CH",   // Sonoff T1 2CH (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button 1
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0,
//      GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_KEY2,        // GPIO09 Button 2
//      0,                // GPIO10
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff T1 3CH",   // Sonoff T1 3CH (ESP8285)
//      GPIO_KEY1,        // GPIO00 Button 1
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional Sensor (J3 Pin 5)
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_REL3,        // GPIO04 Blue Led and Relay 3 (0 = Off, 1 = On)
//      GPIO_REL2,        // GPIO05 Blue Led and Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_KEY2,        // GPIO09 Button 2
//      GPIO_KEY3,        // GPIO10 Button 3
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Blue Led and Relay 1 (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Supla Espablo",   // Supla Espablo (ESP8266)
//                        // http://www.wykop.pl/ramka/3325399/diy-supla-do-puszki-instalacyjnej-podtynkowej-supla-org/
//      0,                // GPIO00 Flash jumper
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_DSB1,         // GPIO02 DS18B20 sensor
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_KEY1,        // GPIO04 Button 1
//      GPIO_REL1,        // GPIO05 Relay 1 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,        // GPIO12 Optional sensor
//      GPIO_REL2,        // GPIO13 Relay 2 (0 = Off, 1 = On)
//      GPIO_USER,        // GPIO14 Optional sensor
//      0,
//      GPIO_LED1,        // GPIO16 Led (1 = On, 0 = Off) - Link and Power status
//      GPIO_FLAG_ADC0    // ADC0 A0 Analog input
//   },
//   { "Witty Cloud",     // Witty Cloud Dev Board (ESP8266)
//                        // https://www.aliexpress.com/item/ESP8266-serial-WIFI-Witty-cloud-Development-Board-ESP-12F-module-MINI-nodemcu/32643464555.html
//      GPIO_USER,        // GPIO00 D3 flash push button on interface board
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_LED1_INV,    // GPIO02 D4 Blue Led (0 = On, 1 = Off) on ESP-12F - Link and Power status
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_KEY1,        // GPIO04 D2 push button on ESP-12F board
//      GPIO_USER,        // GPIO05 D1 optional sensor
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM2,        // GPIO12 D6 RGB LED Green
//      GPIO_PWM3,        // GPIO13 D7 RGB LED Blue
//      GPIO_USER,        // GPIO14 D5 optional sensor
//      GPIO_PWM1,        // GPIO15 D8 RGB LED Red
//      GPIO_USER,        // GPIO16 D0 optional sensor
//      GPIO_FLAG_ADC0    // ADC0 A0 Light sensor / Requires USE_ADC_VCC in user_config.h to be disabled
//   },
//   { "Yunshan Relay",   // Yunshan Wifi Relay (ESP8266)
//                        // https://www.ebay.com/p/Esp8266-220v-10a-Network-Relay-WiFi-Module/1369583381
//                        // Schematics and Info https://ucexperiment.wordpress.com/2016/12/18/yunshan-esp8266-250v-15a-acdc-network-wifi-relay-module/
//      0,                // GPIO00 Flash jumper - Module Pin 8
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor - Module Pin 2
//      GPIO_LED1_INV,    // GPIO02 Blue Led (0 = On, 1 = Off) on ESP-12F - Module Pin 7 - Link and Power status
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor - Module Pin 3
//      GPIO_REL1,        // GPIO04 Red Led and Relay (0 = Off, 1 = On) - Module Pin 10
//      GPIO_KEY1,        // GPIO05 Blue Led and OptoCoupler input - Module Pin 9
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0, 0, 0, 0, 0, 0
//   },
//   ,
//   { "Luani HVIO",      // ESP8266_HVIO
//                        // https://luani.de/projekte/esp8266-hvio/
//      0,                // GPIO00 Flash jumper
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Optional sensor / I2C SDA pad
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_REL1,        // GPIO04 Relay 1 (0 = Off, 1 = On)
//      GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_SWT1,        // GPIO12 External input 1 (0 = On, 1 = Off)
//      GPIO_SWT2,        // GPIO13 External input 2 (0 = On, 1 = Off)
//      GPIO_USER,        // GPIO14 Optional sensor / I2C SCL pad
//      GPIO_LED1,        // GPIO15 Led (1 = On, 0 = Off) - Link and Power status
//      0,
//      GPIO_FLAG_ADC0    // ADC0 A0 Analog input
//   },
//   { "KMC 70011",       // KMC 70011
//                        // https://www.amazon.com/KMC-Timing-Monitoring-Network-125V-240V/dp/B06XRX2GTQ
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0,
//      GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
//      GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 voltage / current
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL,     // GPIO12 HLW8012 SEL (1 = Voltage)
//      GPIO_LED1_INV,    // GPIO13 Green Led - Link and Power status
//      GPIO_REL1,        // GPIO14 Relay
//      0, 0, 0
//   },
//   { "Arilux LC01",     // Arilux AL-LC01 (ESP8285)
//                        // https://www.banggood.com/nl/ARILUX-AL-LC01-Super-Mini-LED-WIFI-Smart-RGB-Controller-For-RGB-LED-Strip-Light-DC-9-12V-p-1058603.html
//                        //  (PwmFrequency 1111Hz)
//      GPIO_KEY1,        // GPIO00 Optional Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_LED4_INV,    // GPIO02 RF receiver control
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_ARIRFRCV,    // GPIO04 IR or RF receiver (optional)
//      GPIO_PWM1,        // GPIO05 RGB LED Red
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM2,        // GPIO12 RGB LED Green
//      GPIO_PWM3,        // GPIO13 RGB LED Blue
//      GPIO_USER,        // GPIO14 RGBW LED White (optional - set to PWM4 for Cold White or Warm White)
//      0, 0, 0
//   },
//   { "Arilux LC11",     // Arilux AL-LC11 (ESP8266)
//                        // https://www.banggood.com/nl/ARILUX-AL-LC11-Super-Mini-LED-WIFI-APP-Controller-RF-Remote-Control-For-RGBWW-LED-Strip-DC9-28V-p-1085112.html
//                        //  (PwmFrequency 540Hz)
//      GPIO_KEY1,        // GPIO00 Optional Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_LED4_INV,    // GPIO02 RF receiver control
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_PWM2,        // GPIO04 RGB LED Green
//      GPIO_PWM1,        // GPIO05 RGB LED Red
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM5,        // GPIO12 RGBCW LED Warm
//      GPIO_PWM4,        // GPIO13 RGBW LED Cold
//      GPIO_PWM3,        // GPIO14 RGB LED Blue
//      GPIO_ARIRFRCV,    // GPIO15 RF receiver input
//      0, 0
//   },
//   { "Sonoff Dual R2",  // Sonoff Dual R2 (ESP8285)
//      GPIO_USER,        // GPIO00 Button 0 on header (0 = On, 1 = Off)
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0,
//      GPIO_REL2,        // GPIO05 Relay 2 (0 = Off, 1 = On)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_USER,        // GPIO09 Button 1 on header (0 = On, 1 = Off)
//      GPIO_KEY1,        // GPIO10 Button on casing
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Relay 1 (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Arilux LC06",     // Arilux AL-LC06 (ESP8285)
//                        // https://www.banggood.com/ARILUX-AL-LC06-LED-WIFI-Smartphone-Controller-Romote-5-Channels-DC12-24V-For-RGBWW-Strip-light-p-1061476.html
//      GPIO_KEY1,        // GPIO00 Optional Button
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_USER,        // GPIO02 Empty pad
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_USER,        // GPIO04 W2 - PWM5
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM2,        // GPIO12 RGB LED Green
//      GPIO_PWM3,        // GPIO13 RGB LED Blue
//      GPIO_PWM1,        // GPIO14 RGB LED Red
//      GPIO_USER,        // GPIO15 RGBW LED White
//      0, 0
//   },
//   { "Sonoff S31",      // Sonoff S31 (ESP8266 - CSE7766)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_CSE7766_TX,  // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
//      0,
//      GPIO_CSE7766_RX,  // GPIO03 Serial TXD
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Zengge WF017",    // Zenggee ZJ-WF017-A (ESP12S))
//                        // https://www.ebay.com/p/Smartphone-Android-IOS-WiFi-Music-Controller-for-RGB-5050-3528-LED-Strip-Light/534446632?_trksid=p2047675.l2644
//      GPIO_KEY1,        // GPIO00 Optional Button
//      0,
//      GPIO_USER,        // GPIO02 Empty pad
//      0,
//      GPIO_USER,        // GPIO04 W2 - PWM5
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM2,        // GPIO12 RGB LED Green
//      GPIO_PWM1,        // GPIO13 RGB LED Red
//      GPIO_PWM3,        // GPIO14 RGB LED Blue
//      0, 0, 0
//   },
//   { "Sonoff Pow R2",   // Sonoff Pow R2 (ESP8285 - CSE7766)
//      GPIO_KEY1,        // GPIO00 Button
//      GPIO_CSE7766_TX,  // GPIO01 Serial RXD 4800 baud 8E1 CSE7766 energy sensor
//      0,
//      GPIO_CSE7766_RX,  // GPIO03 Serial TXD
//      0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link and Power status
//      0, 0, 0, 0
//   },
//   { "Sonoff iFan02",   // Sonoff iFan02 (ESP8285)
//      GPIO_KEY1,        // GPIO00 WIFI_KEY0 Virtual button 1 as feedback from RC
//      GPIO_USER,        // GPIO01 ESP_TXD Serial RXD and Optional sensor
//      0,                // GPIO02 ESP_LOG
//      GPIO_USER,        // GPIO03 ESP_RXD Serial TXD and Optional sensor
//      GPIO_REL3,        // GPIO04 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
//      GPIO_REL2,        // GPIO05 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      GPIO_KEY2,        // GPIO09 WIFI_KEY1 Virtual button 2 as feedback from RC
//      GPIO_KEY3,        // GPIO10 WIFI_KEY2 Virtual button 3 as feedback from RC
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
//      GPIO_LED1_INV,    // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
//      GPIO_KEY4,        // GPIO14 WIFI_KEY3 Virtual button 4 as feedback from RC
//      GPIO_REL4,        // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
//      0, 0
//   },
//   { "BlitzWolf SHP",   // BlitzWolf BW-SHP2 and BW-SHP6 (ESP8285 - BL0937 or HJL-01 Energy Monitoring)
//                        // https://www.banggood.com/BlitzWolf-BW-SHP2-Smart-WIFI-Socket-EU-Plug-220V-16A-Work-with-Amazon-Alexa-Google-Assistant-p-1292899.html
//                        // https://www.amazon.de/Steckdose-Homecube-intelligente-Verbrauchsanzeige-funktioniert/dp/B076Q2LKHG/ref=sr_1_fkmr0_1
//                        // https://www.amazon.de/Intelligente-Stromverbrauch-Fernsteurung-Schaltbare-Energieklasse/dp/B076WZQS4S/ref=sr_1_1
//                        // https://www.aliexpress.com/store/product/BlitzWolf-BW-SHP6-EU-Plug-Metering-Version-WIFI-Smart-Socket-220V-240V-10A-Work-with-Amazon/1965360_32945504669.html
//      GPIO_LED2_INV,    // GPIO00 Red Led (1 = On, 0 = Off) - Power status
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_LED1_INV,    // GPIO02 Blue Led (1 = On, 0 = Off) - Link status
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      0,
//      GPIO_HJL_CF,      // GPIO05 BL0937 or HJL-01 CF power
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
//      GPIO_KEY1,        // GPIO13 Button
//      GPIO_NRG_CF1,     // GPIO14 BL0937 or HJL-01 CF1 current / voltage
//      GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
//      0, 0
//   },
//   { "Xiaomi Philips",  // Xiaomi Philips bulb (ESP8266)
//      0, 0, 0, 0, 0, 0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_PWM2,        // GPIO12 cold/warm light
//      0, 0,
//      GPIO_PWM1,        // GPIO15 light intensity
//      0, 0
//   },
//   { "Neo Coolcam",     // Neo Coolcam (ESP8266)
//                        // https://www.banggood.com/NEO-COOLCAM-WiFi-Mini-Smart-Plug-APP-Remote-Control-Timing-Smart-Socket-EU-Plug-p-1288562.html?cur_warehouse=CN
//      0, 0, 0, 0,
//      GPIO_LED1_INV,    // GPIO04 Red Led (0 = On, 1 = Off) - Link and Power status
//      0,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off, 1 = On)
//      GPIO_KEY1,        // GPIO13 Button
//      0, 0, 0, 0
//   },
//   { "ESP Switch",      // Michael Haustein 4 channel wall switch (ESP07 = ESP8266)
//                        // Use rules for further actions like - rule on power1#state do publish cmnd/other_device/power %value% endon
//      GPIO_KEY2,        // GPIO00 Button 2
//      GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
//      GPIO_REL3_INV,    // GPIO02 Yellow Led 3 (0 = On, 1 = Off)
//      GPIO_USER,        // GPIO03 Serial TXD and Optional sensor
//      GPIO_KEY1,        // GPIO04 Button 1
//      GPIO_REL2_INV,    // GPIO05 Red Led 2 (0 = On, 1 = Off)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL4_INV,    // GPIO12 Blue Led 4 (0 = On, 1 = Off)
//      GPIO_KEY4,        // GPIO13 Button 4
//      GPIO_KEY3,        // GPIO14 Button 3
//      GPIO_LED1,        // GPIO15 Optional sensor
//      GPIO_REL1_INV,    // GPIO16 Green Led 1 (0 = On, 1 = Off)
//      0
//   },
//   { "OBI Socket",      // OBI socket (ESP8266) - https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706
//      GPIO_USER,        // GPIO00
//      GPIO_USER,        // GPIO01 Serial RXD
//      0,
//      GPIO_USER,        // GPIO03 Serial TXD
//      GPIO_LED1,        // GPIO04 Blue LED - Link and Power status
//      GPIO_REL1,        // GPIO05 (Relay OFF, but used as Relay Switch)
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_LED3,        // GPIO12 (Relay ON, but set to LOW, so we can switch with GPIO05)
//      GPIO_USER,        // GPIO13
//      GPIO_KEY1,        // GPIO14 Button
//      0,
//      GPIO_USER,        // GPIO16
//      GPIO_FLAG_ADC0    // ADC0   A0 Analog input
//   },
//   { "Teckin",          // https://www.amazon.de/gp/product/B07D5V139R
//      0,
//      GPIO_KEY1,        // GPIO01 Serial TXD and Button
//      0,
//      GPIO_LED2_INV,    // GPIO03 Serial RXD and Red Led (0 = On, 1 = Off) - Power status
//      GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
//      GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
//      GPIO_LED1_INV,    // GPIO13 Blue Led (0 = On, 1 = Off) - Link status
//      GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
//      0, 0, 0
//   },
//   { "AplicWDP303075",  // Aplic WDP 303075 (ESP8285 - HLW8012 Energy Monitoring)
//                        // https://www.amazon.de/dp/B07CNWVNJ2
//      0, 0, 0,
//      GPIO_KEY1,        // GPIO03 Button
//      GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
//      GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
//      GPIO_LED1_INV,    // GPIO13 LED (0 = On, 1 = Off) - Link and Power status
//      GPIO_REL1,        // GPIO14 Relay SRU 5VDC SDA (0 = Off, 1 = On )
//      0, 0, 0
//   },
//   { "Tuya Dimmer",     // Tuya Dimmer (ESP8266 w/ separate MCU dimmer)
//                        // https://www.amazon.com/gp/product/B07CTNSZZ8/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1
//      GPIO_USER,        // Virtual Button (controlled by MCU)
//      GPIO_USER,        // GPIO01 MCU serial control
//      GPIO_USER,
//      GPIO_USER,        // GPIO03 MCU serial control
//      GPIO_USER,
//      GPIO_USER,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,
//      GPIO_USER,
//      GPIO_USER,        // GPIO14 Green Led
//      GPIO_USER,
//      GPIO_USER,
//      0
//   },
//   { "Gosund SP1 v23",  // https://www.amazon.de/gp/product/B0777BWS1P
//      0,
//      GPIO_LED1_INV,    // GPIO01 Serial RXD and LED1 (blue) inv - Link status
//      0,
//      GPIO_KEY1,        // GPIO03 Serial TXD and Button
//      GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
//      GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
//      GPIO_LED2_INV,    // GPIO13 LED2 (red) inv - Power status
//      GPIO_REL1,        // GPIO14 Relay (0 = Off, 1 = On)
//      0, 0, 0
//   },
//   { "ARMTR Dimmer",    // ARMTRONIX Dimmer, one or two channel (ESP8266 w/ separate MCU dimmer)
//                        // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-two-triac-board/
//                        // https://www.tindie.com/products/Armtronix/wifi-ac-dimmer-esp8266-one-triac-board-alexaecho/
//      GPIO_USER,
//      GPIO_TXD,         // GPIO01 MCU serial control
//      GPIO_USER,
//      GPIO_RXD,         // GPIO03 MCU serial control
//      GPIO_USER,
//      GPIO_USER,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,
//      GPIO_USER,
//      GPIO_USER,
//      GPIO_USER,
//      GPIO_USER,
//      0
//   },
//   { "SK03 Outdoor",    // Outdoor smart plug with power monitoring HLW8012 chip - https://www.amazon.com/gp/product/B07CG7MBPV
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0,
//      GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
//      GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 current / voltage
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 HLW8012 CF Sel output (0 = Voltage)
//      GPIO_LED2_INV,    // GPIO13 Red Led (0 = On, 1 = Off) - Power status
//      GPIO_LED1_INV,    // GPIO14 Blue Led (0 = On, 1 = Off) - Link status
//      GPIO_REL1,        // GPIO15 Relay (0 = Off, 1 = On)
//      0, 0
//   },
//   { "PS-16-DZ",        // PS-16-DZ Dimmer (ESP8266 w/ separate Nuvoton MCU dimmer)
//                        // https://www.aliexpress.com/item/SM-Smart-WIFI-Wall-Dimmer-Light-Switch-US-Ewelink-APP-Remote-Control-Wi-Fi-Wirele-Work/32871151902.html
//      GPIO_USER,
//      GPIO_TXD,         // GPIO01 MCU serial control
//      GPIO_USER,
//      GPIO_RXD,         // GPIO03 MCU serial control
//      GPIO_USER,
//      GPIO_USER,
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,
//      GPIO_LED1,        // GPIO13 WiFi LED - Link and Power status
//      GPIO_USER,
//      GPIO_USER,
//      GPIO_USER,
//      0
//   },
//   { "Manzoku strip",   // "MANZOKU" labeled power strip, EU version
//                        // https://www.amazon.de/Steckdosenleiste-AOFO-Mehrfachsteckdose-Überspannungsschutz-Sprachsteuerung/dp/B07GBSD11P/
//                        // https://www.amazon.de/Steckdosenleiste-Geekbes-USB-Anschluss-Kompatibel-gesteuert/dp/B078W23BW9/
//      0,                // GPIO00
//      0,                // GPIO01 Serial RXD
//      0,
//      GPIO_KEY1,        // GPIO03 Serial TXD + Button
//      GPIO_REL2,        // GPIO04 Relay 2
//      GPIO_REL1,        // GPIO05 Relay 1
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL3,        // GPIO12 Relay 3
//      GPIO_REL4,        // GPIO13 Relay 4
//      GPIO_USER,        // GPIO14
//      0,
//      GPIO_USER,        // GPIO16
//      0
//   },
//   { "YTF IR Bridge",   // https://www.aliexpress.com/item/Tuya-universal-Smart-IR-Hub-remote-control-Voice-Control-AC-TV-Work-With-Alexa-Google-Home/32951202513.html
//      GPIO_USER,        // GPIO00
//      GPIO_USER,        // GPIO01 Serial RXD
//      GPIO_USER,        // GPIO02
//      GPIO_USER,        // GPIO03 Serial TXD
//      GPIO_LED1_INV,    // GPIO04 Blue Led - Link status
//      GPIO_IRRECV,      // GPIO05 IR Receiver
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      0,                // GPIO12
//      GPIO_KEY1,        // GPIO13 Button
//      GPIO_IRSEND,      // GPIO14 IR Transmitter
//      0, 0, 0
//   },
//   { "Digoo DG-SP202",  // Digoo DG-SP202
//                        // https://www.banggood.com/DIGOO-DG-SP202-Dual-EU-Plug-Smart-WIFI-Socket-Individual-Controllable-Energy-Monitor-Remote-Control-Timing-Smart-Home-Outlet-let-p-1375323.html
//      GPIO_KEY1,        // GPIO00 Button1
//      0,                // GPIO01 Serial RXD
//      0,                // GPIO02
//      0,                // GPIO03 Serial TXD
//      GPIO_HJL_CF,      // GPIO04 BL0937 or HJL-01 CF power
//      GPIO_NRG_CF1,     // GPIO05 BL0937 or HJL-01 CF1 current / voltage
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 or HJL-01 Sel output (0 = Voltage)
//      GPIO_LED1,        // GPIO13 Blue Leds - Link Status
//      GPIO_REL2,        // GPIO14 Relay2 (0 = Off, 1 = On) and Red Led
//      GPIO_REL1,        // GPIO15 Relay1 (0 = Off, 1 = On) and Red Led
//      GPIO_KEY2_NP,     // GPIO16 Button2, externally pulled up
//      0
//   },
//   { "KA10",            // SMANERGY KA10 (ESP8285 - BL0937 Energy Monitoring) - https://www.amazon.es/dp/B07MBTCH2Y
//      0,                // GPIO00
//      GPIO_LED1_INV,    // GPIO01 Blue LED - Link status
//      0,                // GPIO02
//      GPIO_KEY1,        // GPIO03 Button
//      GPIO_HJL_CF,      // GPIO04 BL0937 CF power
//      GPIO_NRG_CF1,     // GPIO05 BL0937 CF1 voltage / current
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 Sel output (1 = Voltage)
//      GPIO_LED2,        // GPIO13 Red LED - Power status
//      GPIO_REL1,        // GPIO14 Relay 1
//      0, 0, 0
//   },
//   { "Luminea ZX2820",
//      GPIO_KEY1,        // GPIO00 Button
//      0, 0, 0,
//      GPIO_HLW_CF,      // GPIO04 HLW8012 CF power
//      GPIO_NRG_CF1,     // GPIO05 HLW8012 CF1 voltage / current
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 HLW8012 SEL (0 = Voltage)
//      GPIO_LED1_INV,    // GPIO13 Green Led - Link and Power status
//      GPIO_REL1,        // GPIO14 Relay
//      0, 0, 0
//   },
//   { "Mi Desk Lamp",    // Mi LED Desk Lamp - https://www.mi.com/global/smartlamp/
//      0, 0,
//      GPIO_KEY1,        // GPIO02 Button
//      0,
//      GPIO_PWM1,        // GPIO04 Cold White
//      GPIO_PWM2,        // GPIO05 Warm White
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_ROT1A,       // GPIO12 Rotary switch A pin
//      GPIO_ROT1B,       // GPIO13 Rotary switch B pin
//      0, 0, 0, 0
//   },
//   { "SP10",            // Tuya SP10 (BL0937 Energy Monitoring)
//                        // https://www.aliexpress.com/item/Smart-Mini-WiFi-Plug-Outlet-Switch-Work-With-ForEcho-Alexa-Google-Home-Remote-EU-Smart-Socket/32963670423.html
//      0,                // GPIO00
//      GPIO_PWM1,        // GPIO01 Nightlight
//      0,                // GPIO02
//      GPIO_KEY1,        // GPIO03 Button
//      GPIO_HJL_CF,      // GPIO04 BL0937 CF power
//      GPIO_NRG_CF1,     // GPIO05 BL0937 CF1 voltage / current
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_NRG_SEL_INV, // GPIO12 BL0937 Sel output (1 = Voltage)
//      GPIO_LED1,        // GPIO13 Blue LED - Link status
//      GPIO_REL1,        // GPIO14 Relay and red LED
//      0, 0, 0
//   },
//   { "WAGA CHCZ02MB",   // WAGA life CHCZ02MB (HJL-01 Energy Monitoring)
//                        // https://www.ebay.com/itm/332595697006
//      GPIO_LED2_INV,    // GPIO00 Red LED
//      0,                // GPIO01 Serial RXD
//      0,                // GPIO02
//      GPIO_NRG_SEL_INV, // GPIO03 HJL-01 Sel output (1 = Voltage)
//      0,                // GPIO04
//      GPIO_HJL_CF,      // GPIO05 HJL-01 CF power
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_REL1,        // GPIO12 Relay
//      GPIO_KEY1,        // GPIO13 Button
//      GPIO_NRG_CF1,     // GPIO14 HJL-01 CF1 voltage / current
//      GPIO_LED1_INV,    // GPIO15 Blue LED - Link status
//      0, 0
//   },
//   { "SYF05",           // Sunyesmart SYF05 (a.k.a. Fcmila) = TYWE3S + SM16726
//                        // Also works with Merkury 904 RGBW Bulbs with 13 set to GPIO_SM16716_SEL
//                        // https://www.flipkart.com/fc-mila-bxav-xs-ad-smart-bulb/p/itmf85zgs45fzr7n
//                        // https://docs.tuya.com/en/hardware/WiFi-module/wifi-e3s-module.html
//                        // http://www.datasheet-pdf.com/PDF/SM16716-Datasheet-Sunmoon-932771
//      GPIO_USER,        // GPIO00 N.C.
//      0,                // GPIO01 Serial RXD
//      GPIO_USER,        // GPIO02 N.C.
//      0,                // GPIO03 Serial TXD
//      GPIO_SM16716_CLK, // GPIO04 SM16716 Clock
//      GPIO_PWM1,        // GPIO05 White
//                        // GPIO06 (SD_CLK   Flash)
//                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//      0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//      0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                        // GPIO11 (SD_CMD   Flash)
//      GPIO_USER,        // GPIO12 Alt. White on some devices
//      GPIO_USER,        // GPIO13 SM16716 Enable on some devices
//      GPIO_SM16716_DAT, // GPIO14 SM16716 Data
//      0,                // GPIO15 wired to GND
//      GPIO_USER,        // GPIO16 N.C.
//      GPIO_FLAG_ADC0    // ADC0 A0 Analog input
//   }
  { //D_MODULE_NAME_NODEMCU_CTR,         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
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
  }
};

#endif  // _mHardwareTemplates_H_