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
 * Example:
 *
 *   PGPIO(GPIO_REL1) + MGPIO(MAX_RELAYS_SET)
 *
 * expands to:
 *
 *   Relay1
 *   Relay2
 *   Relay3
 *   ...
\*********************************************************************************************/

const uint16_t SelectablePins_BitPacked[] PROGMEM =
{
  
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

  // Buzzer
  PGPIO(GPIO_BUZZER),
  PGPIO(GPIO_BUZZER_INV),

  /****************************************
   * SECTION: I2C
   ****************************************/

  // I2C bus
  PGPIO(GPIO_I2C_SCL) + MGPIO(2),
  PGPIO(GPIO_I2C_SDA) + MGPIO(2),


  /****************************************
   * SECTION: Modem
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
   * SECTION: Relays / LEDs / Inputs
   ****************************************/

  // Relays
  PGPIO(GPIO_REL1) + MGPIO(MAX_RELAYS_SET),
  PGPIO(GPIO_REL1_INV) + MGPIO(MAX_RELAYS_SET),

  // LEDs
  PGPIO(GPIO_LED1) + MGPIO(MAX_DIGITAL_LEDS),
  PGPIO(GPIO_LED1_INV) + MGPIO(MAX_DIGITAL_LEDS),

  // Switches
  PGPIO(GPIO_SWT1) + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_INV) + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_NP) + MGPIO(MAX_SWITCHES_SET),
  PGPIO(GPIO_SWT1_INV_NP) + MGPIO(MAX_SWITCHES_SET),

  // Buttons / keys
  PGPIO(GPIO_KEY1) + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_INV) + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_NP) + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_INV_NP) + MGPIO(MAX_KEYS_SET),
  PGPIO(GPIO_KEY1_TOUCH) + MGPIO(MAX_KEYS_SET),


  /****************************************
   * SECTION: PWM / Counters / ADC
   ****************************************/

  // PWM
  PGPIO(GPIO_PWM1) + MGPIO(MAX_PWMS),
  PGPIO(GPIO_PWM1_INV) + MGPIO(MAX_PWMS),

  // Counters
  PGPIO(GPIO_PULSE_COUNTER1) + MGPIO(MAX_COUNTERS),
  PGPIO(GPIO_PULSE_COUNTER1_NP) + MGPIO(MAX_COUNTERS),

  // ADC
  PGPIO(GPIO_ADC_INPUT) + MGPIO(MAX_ADCS),


  /****************************************
   * SECTION: IR / Shutters
   ****************************************/

  // IR send
  PGPIO(GPIO_IRSEND) + MGPIO(MAX_IRSEND),

  // Shutters
  PGPIO(GPIO_SHUTTER_OPEN) + MGPIO(MAX_SHUTTERS),
  PGPIO(GPIO_SHUTTER_CLOSE) + MGPIO(MAX_SHUTTERS),
  PGPIO(GPIO_SHUTTER_BUTTON) + MGPIO(MAX_SHUTTER_KEYS),

  /****************************************
   * SECTION: SD Card
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
   * SECTION: Sensors
   ****************************************/

    /****************************************
     * SECTION: DS18X20
     ****************************************/
    
    PGPIO(GPIO_DS18B20) + MGPIO(2),

    /****************************************
     * SECTION: DHT
     ****************************************/
    
    PGPIO(GPIO_DHT11) + MGPIO(2),
    PGPIO(GPIO_DHT22) + MGPIO(2),


  /****************************************
   * SECTION: Displays
   ****************************************/
  
  PGPIO(GPIO_NEXTION_TX) + MGPIO(3),
  PGPIO(GPIO_NEXTION_RX) + MGPIO(3),


  PGPIO(GPIO_LIGHTING_DIGITAL) + MGPIO(16),
  PGPIO(GPIO_LIGHTING_CLOCK)   + MGPIO(4),
  PGPIO(GPIO_LIGHTING_PWM)     + MGPIO(10),
  PGPIO(GPIO_LIGHTING_ONOFF)   + MGPIO(5),

  // Generic hardware serial
  PGPIO(GPIO_UNUSED_FORCED_HIGH) + MGPIO(16),
  PGPIO(GPIO_UNUSED_FORCED_LOW) + MGPIO(16),



};

const uint16_t SelectablePins_BitPacked_Count =
  sizeof(SelectablePins_BitPacked) / sizeof(SelectablePins_BitPacked[0]);