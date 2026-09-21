#include "gpio_bitpacked.h"

#ifdef ESP8266
#include <pgmspace.h>
#endif

#ifdef ESP32
#include <pgmspace.h>
#endif

/*********************************************************************************************\
 * Build-specific selectable GPIO function groups
 *
 * This replaces the old explicit indexed GPIO function list.
 *
 * Add base functions only. Indexed names are generated dynamically by gpio_names.cpp.
 *
 * Packing:
 *
 *   PGPIO(GPIO_REL1) + MGPIO(MAX_RELAYS_SET)
 *
 * expands conceptually to:
 *
 *   Relay1
 *   Relay2
 *   Relay3
 *   ...
 *
 * This table should include every selectable GPIO base enum from SelectablePins_Base.
 * Do not add GPIO_MAX. It is an enum boundary marker, not a selectable function.
\*********************************************************************************************/

const uint16_t SelectablePins_BitPacked[] PROGMEM =
{

  /****************************************
   * SECTION: Special assignment states
   ****************************************/

  PGPIO(GPIO_NONE),
  PGPIO(GPIO_USER),


  /****************************************
   * SECTION: Forced unused states
   ****************************************/

  PGPIO(GPIO_UNUSED_FORCED_HIGH) + MGPIO(16),
  PGPIO(GPIO_UNUSED_FORCED_LOW)  + MGPIO(16),


  /****************************************
   * SECTION: Filesystem / SD Card
   ****************************************/

  // SD card using VSPI
  PGPIO(GPIO_SDCARD_VSPI_CSO),
  PGPIO(GPIO_SDCARD_VSPI_CLK),
  PGPIO(GPIO_SDCARD_VSPI_MOSI),
  PGPIO(GPIO_SDCARD_VSPI_MISO),

  // SD card using HSPI
  PGPIO(GPIO_SDCARD_HSPI_CSO),
  PGPIO(GPIO_SDCARD_HSPI_CLK),
  PGPIO(GPIO_SDCARD_HSPI_MOSI),
  PGPIO(GPIO_SDCARD_HSPI_MISO),

  // SD card using native MMC bus
  PGPIO(GPIO_SDCARD_MMC_CMD),
  PGPIO(GPIO_SDCARD_MMC_CLK),
  PGPIO(GPIO_SDCARD_MMC_D) + MGPIO(4),


  /****************************************
   * SECTION: UART
   ****************************************/

  // Generic hardware serial
  PGPIO(GPIO_HWSERIAL_TX) + MGPIO(3),
  PGPIO(GPIO_HWSERIAL_RX) + MGPIO(3),

  // Generic software serial
  PGPIO(GPIO_SWSERIAL_TX),
  PGPIO(GPIO_SWSERIAL_RX),

  // Ring-buffer serial modes
  PGPIO(GPIO_HWSERIAL_RING_BUFFER_TX) + MGPIO(3),
  PGPIO(GPIO_HWSERIAL_RING_BUFFER_RX) + MGPIO(3),

  // Debug serial
  PGPIO(GPIO_SERIAL_DEBUG_TX),
  PGPIO(GPIO_SERIAL_DEBUG_RX),

  // Serial bridge
  PGPIO(GPIO_SBR_TX),
  PGPIO(GPIO_SBR_RX),


  /****************************************
   * SECTION: I2C
   ****************************************/

  PGPIO(GPIO_I2C_SCL) + MGPIO(2),
  PGPIO(GPIO_I2C_SDA) + MGPIO(2),


  /****************************************
   * SECTION: SPI
   ****************************************/

  // Generic hardware SPI pins
  PGPIO(GPIO_SPI_CS),
  PGPIO(GPIO_SPI_DC),

  // Generic software SPI pins
  PGPIO(GPIO_SSPI_MISO),
  PGPIO(GPIO_SSPI_MOSI),
  PGPIO(GPIO_SSPI_SCLK),
  PGPIO(GPIO_SSPI_CS),
  PGPIO(GPIO_SSPI_DC),


  /****************************************
   * SECTION: ADC / Analog
   ****************************************/

  // Internal ADC sources / hardware ADC channels
  PGPIO(GPIO_ADC0_TEMPERATURE),
  PGPIO(GPIO_ADC1_CH4),
  PGPIO(GPIO_ADC1_CH6),
  PGPIO(GPIO_ADC1_CH7),

  // ADC external trigger/input
  PGPIO(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER),
  PGPIO(GPIO_ADC_INPUT) + MGPIO(MAX_ADCS),


  /****************************************
   * SECTION: PWM
   ****************************************/

  PGPIO(GPIO_PWM1)     + MGPIO(MAX_PWMS),
  PGPIO(GPIO_PWM1_INV) + MGPIO(MAX_PWMS),


  /****************************************
   * SECTION: LEDs
   ****************************************/

  PGPIO(GPIO_LED1)     + MGPIO(MAX_DIGITAL_LEDS),
  PGPIO(GPIO_LED1_INV) + MGPIO(MAX_DIGITAL_LEDS),


  /****************************************
   * SECTION: Relays
   ****************************************/

  PGPIO(GPIO_REL1)     + MGPIO(MAX_RELAYS_SET),
  PGPIO(GPIO_REL1_INV) + MGPIO(MAX_RELAYS_SET),


  /****************************************
   * SECTION: Switches
   ****************************************/

  PGPIO(GPIO_SWT1)        + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_INV)    + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_NP)     + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_INV_NP) + MGPIO(MAX_SWITCHES_SET),


  /****************************************
   * SECTION: Buttons / Keys
   ****************************************/

  PGPIO(GPIO_KEY1)        + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_INV)    + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_NP)     + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_INV_NP) + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_TOUCH)  + MGPIO(MAX_KEYS_SET),


  /****************************************
   * SECTION: Buzzer
   ****************************************/

  PGPIO(GPIO_BUZZER),
  PGPIO(GPIO_BUZZER_INV),


  /****************************************
   * SECTION: IR / RF
   ****************************************/

  // Infrared
  PGPIO(GPIO_IRSEND) + MGPIO(MAX_IRSEND),
  PGPIO(GPIO_IRRECV),

  // Generic RF
  PGPIO(GPIO_RFSEND),
  PGPIO(GPIO_RFRECV),

  // 433 MHz RF
  PGPIO(GPIO_RF_433MHZ_RX),
  PGPIO(GPIO_RF_433MHZ_TX),


  /****************************************
   * SECTION: Shutters / Servo / Motors
   ****************************************/

  // Shutters
  PGPIO(GPIO_SHUTTER_OPEN)   + MGPIO(MAX_SHUTTERS),
  PGPIO(GPIO_SHUTTER_CLOSE)  + MGPIO(MAX_SHUTTERS),
  PGPIO(GPIO_SHUTTER_BUTTON) + MGPIO(MAX_SHUTTER_KEYS),

  // Servos
  PGPIO(GPIO_SERVO),

  // L9110 H-bridge
  PGPIO(GPIO_HBRIDGE_L9110_IA),
  PGPIO(GPIO_HBRIDGE_L9110_IB),
  PGPIO(GPIO_HBRIDGE_L9110_OA),
  PGPIO(GPIO_HBRIDGE_L9110_OB),


  /****************************************
   * SECTION: Cellular Modem
   ****************************************/

  // Modem UART
  PGPIO(GPIO_MODEM_RX) + MGPIO(3),
  PGPIO(GPIO_MODEM_TX) + MGPIO(3),

  // Modem control/status
  PGPIO(GPIO_MODEM_POWER_KEY),
  PGPIO(GPIO_MODEM_DATA_TERMINAL_READY_DTR),
  PGPIO(GPIO_MODEM_RING_INDICATOR),
  PGPIO(GPIO_MODEM_POWER_STATUS),
  PGPIO(GPIO_MODEM_NETWORK_STATUS),


  /****************************************
   * SECTION: Cameras
   ****************************************/

  // Camera external clock
  PGPIO(GPIO_WEBCAM_XCLK),

  // Camera parallel data bus
  PGPIO(GPIO_WEBCAM_DATA) + MGPIO(8),

  // Camera timing/control pins
  PGPIO(GPIO_WEBCAM_PCLK),
  PGPIO(GPIO_WEBCAM_HREF),
  PGPIO(GPIO_WEBCAM_VSYNC),

  // Camera SCCB/I2C pins
  PGPIO(GPIO_WEBCAM_SIOD),
  PGPIO(GPIO_WEBCAM_SIOC),

  // Camera power/reset pins
  PGPIO(GPIO_WEBCAM_PWDN),
  PGPIO(GPIO_WEBCAM_RESET),


  /****************************************
   * SECTION: MAVLink
   ****************************************/

  PGPIO(GPIO_MAVLINK_TX),
  PGPIO(GPIO_MAVLINK_RX),


  /****************************************
   * SECTION: Sensors - One-wire / DHT / PIR
   ****************************************/

  // DS18x20
  PGPIO(GPIO_DS18B20) + MGPIO(2),

  // DHT
  PGPIO(GPIO_DHT11) + MGPIO(2),
  PGPIO(GPIO_DHT22) + MGPIO(2),

  // PIR
  PGPIO(GPIO_PIR)     + MGPIO(8),
  PGPIO(GPIO_PIR_INV) + MGPIO(8),


  /****************************************
   * SECTION: Sensors - Door / LDR / Moisture
   ****************************************/

  // Door lock/open
  PGPIO(GPIO_DOOR_LOCK),
  PGPIO(GPIO_DOOR_OPEN),

  // LDR
  PGPIO(GPIO_LDR_BASIC_DIGITAL),
  PGPIO(GPIO_LDR_BASIC_ANALOG),

  // Moisture
  PGPIO(GPIO_MOISTURE_ANALOG),


  /****************************************
   * SECTION: Sensors - SR04 / Pulse Counter
   ****************************************/

  // Ultrasonic
  PGPIO(GPIO_SR04_TRIG) + MGPIO(3),
  PGPIO(GPIO_SR04_ECHO) + MGPIO(3),

  // Pulse counters
  PGPIO(GPIO_PULSE_COUNTER1)    + MGPIO(MAX_COUNTERS),
  PGPIO(GPIO_PULSE_COUNTER1_NP) + MGPIO(MAX_COUNTERS),


  /****************************************
   * SECTION: Sensors - Remote / Rotary / Tracking
   ****************************************/

  // Virtual/remote device link
  PGPIO(GPIO_REMOTE_DEVICE),

  // Rotary encoder
  PGPIO(GPIO_ROTARY_ENCODER_A),
  PGPIO(GPIO_ROTARY_ENCODER_B),

  // Virtual tracking roles
  PGPIO(GPIO_SUN_TRACKING),
  PGPIO(GPIO_MOON_TRACKING),


  /****************************************
   * SECTION: Sensors - TOF
   ****************************************/

  PGPIO(GPIO_VL53L0X_XSHUT1),
  PGPIO(GPIO_VL53L1X_XSHUT1),


  /****************************************
   * SECTION: Sensors - Radar / GPS
   ****************************************/

  // HLK LD2410
  PGPIO(GPIO_LD2410_TX) + MGPIO(3),
  PGPIO(GPIO_LD2410_RX) + MGPIO(3),

  // GPS serial
  PGPIO(GPIO_GPS_SERIAL_TX),
  PGPIO(GPIO_GPS_SERIAL_RX),


  /****************************************
   * SECTION: Energy - PZEM
   ****************************************/

  PGPIO(GPIO_PZEM0XX_TX)        + MGPIO(3),
  PGPIO(GPIO_PZEM004_RX)        + MGPIO(3),
  PGPIO(GPIO_PZEM0XX_RX_MODBUS),
  PGPIO(GPIO_PZEM017_RX),


  /****************************************
   * SECTION: Energy - SDM
   ****************************************/

  PGPIO(GPIO_SDM120_TX),
  PGPIO(GPIO_SDM120_RX),

  PGPIO(GPIO_SDM630_TX),
  PGPIO(GPIO_SDM630_RX),


  /****************************************
   * SECTION: Energy - MCP39F5 / CSE7766 / ADE7953
   ****************************************/

  PGPIO(GPIO_MCP39F5_TX),
  PGPIO(GPIO_MCP39F5_RX),
  PGPIO(GPIO_MCP39F5_RST),

  PGPIO(GPIO_CSE7766_TX),
  PGPIO(GPIO_CSE7766_RX),

  PGPIO(GPIO_ADE7953_IRQ),


  /****************************************
   * SECTION: Displays
   ****************************************/

  // Generic display control
  PGPIO(GPIO_BACKLIGHT),
  PGPIO(GPIO_OLED_RESET),

  // Nextion
  PGPIO(GPIO_NEXTION_TX) + MGPIO(3),
  PGPIO(GPIO_NEXTION_RX) + MGPIO(3),


  /****************************************
   * SECTION: Controllers / Usermods
   ****************************************/

  // Fan
  PGPIO(GPIO_FAN_PWM),

  // Door / lock / chime
  PGPIO(GPIO_CHIME_INPUT),
  PGPIO(GPIO_CHIME_RINGER),

  // Sampling / sync signals
  PGPIO(GPIO_RXON_SAMPLING_ENABLED),
  PGPIO(GPIO_CC1110_SYNC_PULSE_SIGNAL),


  /****************************************
   * SECTION: Lighting
   ****************************************/

  PGPIO(GPIO_LIGHTING_DIGITAL) + MGPIO(16),
  PGPIO(GPIO_LIGHTING_CLOCK)   + MGPIO(4),
  PGPIO(GPIO_LIGHTING_PWM)     + MGPIO(10),
  PGPIO(GPIO_LIGHTING_ONOFF)   + MGPIO(5),


  /****************************************
   * SECTION: Shelly Dimmer
   ****************************************/

  PGPIO(GPIO_SHELLY2_SHD_BOOT0),
  PGPIO(GPIO_SHELLY2_SHD_RESET_INV),

};

const uint16_t SelectablePins_BitPacked_Count =
  sizeof(SelectablePins_BitPacked) / sizeof(SelectablePins_BitPacked[0]);