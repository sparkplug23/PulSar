/**
 * @file gpio_names.cpp
 * @brief GPIO base-name and packed-ID naming helpers
 *
 * Packed GPIO naming model:
 * - Base IDs are defined in gpio_enums.h.
 * - Selectable packed groups are defined in gpio_bitpacked.cpp.
 * - Stored/selected IDs are packed as PGPIO(base_id) + index.
 * - Base strings come only from D_GPIO_*_CTR language defines.
 * - Indexed strings are generated dynamically, e.g. Relay1, Relay2, Relay3.
 *
 * Do not add per-index language strings here. Add only base D_GPIO_*_CTR defines
 * in mLanguageDefault.h and let this file append the numeric suffix.
 **/

#include "mPins.h"
#include "gpio_enums.h"
#include "gpio_bitpacked.h"

#include <string.h>

#ifdef ESP8266
#include <pgmspace.h>
#endif

#ifdef ESP32
#include <pgmspace.h>
#endif

/*********************************************************************************************\
 * Local helpers
\*********************************************************************************************/

uint16_t mPins::GPIOPacked_Make(uint16_t base_id, uint8_t index)
{
  return PGPIO(base_id) + (index & GPIO_INDEX_MASK);
}

static uint16_t GPIOPacked_Base(uint16_t packed_id)
{
  return UGPIO(packed_id);
}

static uint8_t GPIOPacked_Index(uint16_t packed_id)
{
  return packed_id & GPIO_INDEX_MASK;
}

static uint16_t GPIOEntry_Base(uint16_t packed_entry)
{
  return UGPIO(packed_entry);
}

static uint8_t GPIOEntry_Count(uint16_t packed_entry)
{
  return (packed_entry & GPIO_INDEX_MASK) + 1;
}

static bool StringEqualsNoCase(const char* a, const char* b)
{
  if (!a || !b) { return false; }
  return strcasecmp(a, b) == 0;
}
static bool StringStartsWithNoCase(const char* text, const char* prefix)
{
  if (!text || !prefix)
  {
    return false;
  }

  const size_t prefix_len = strlen(prefix);

  if (prefix_len == 0)
  {
    return false;
  }

  if (strlen(text) < prefix_len)
  {
    return false;
  }

  return strncasecmp(text, prefix, prefix_len) == 0;

}

static bool GPIOBaseName_UsesOneBasedIndex(uint16_t base_id)
{
  switch (base_id)
  {
    case GPIO_PWM1:
    case GPIO_PWM1_INV:

    case GPIO_LED1:
    case GPIO_LED1_INV:

    case GPIO_REL1:
    case GPIO_REL1_INV:

    case GPIO_SWT1:
    case GPIO_SWT1_INV:
    case GPIO_SWT1_NP:
    case GPIO_SWT1_INV_NP:

    case GPIO_KEY1:
    case GPIO_KEY1_INV:
    case GPIO_KEY1_NP:
    case GPIO_KEY1_INV_NP:
    case GPIO_KEY1_TOUCH:

    case GPIO_PULSE_COUNTER1:
    case GPIO_PULSE_COUNTER1_NP:
      return true;

    default:
      return false;
  }
}

/*********************************************************************************************\
 * Base name lookup
 *
 * This switch deliberately uses D_GPIO_*_CTR defines directly.
 * Missing language defines should fail at compile time so they can be added to
 * mLanguageDefault.h. Do not add literal display strings here.
\*********************************************************************************************/

static const char* GPIOBaseName_ByID(uint16_t base_id, char* B, uint8_t L)
{
  if (!B || !L) { return nullptr; }

  B[0] = '\0';

  switch (base_id)
  {


  /****************************************
   * SECTION: Unassigned pin states
   ****************************************/

  // Pin not assigned to any function
    case GPIO_NONE:                              snprintf_P(B, L, PSTR(D_GPIO_NONE_CTR)); return B;
  // Force unused pin high
    case GPIO_UNUSED_FORCED_HIGH:                snprintf_P(B, L, PSTR(D_GPIO_UNUSED_FORCED_HIGH_CTR)); return B;
  // Force unused pin low
    case GPIO_UNUSED_FORCED_LOW:                 snprintf_P(B, L, PSTR(D_GPIO_UNUSED_FORCED_LOW_CTR)); return B;


  /****************************************
   * SECTION: Core System
   ****************************************/

      /****************************************
       * SECTION: Filesystem + SD Card
       ****************************************/

      // SD card using VSPI bus
    case GPIO_SDCARD_VSPI_CSO:                   snprintf_P(B, L, PSTR(D_GPIO_SDCARD_VSPI_CSO_CTR)); return B;
    case GPIO_SDCARD_VSPI_CLK:                   snprintf_P(B, L, PSTR(D_GPIO_SDCARD_VSPI_CLK_CTR)); return B;
    case GPIO_SDCARD_VSPI_MOSI:                  snprintf_P(B, L, PSTR(D_GPIO_SDCARD_VSPI_MOSI_CTR)); return B;
    case GPIO_SDCARD_VSPI_MISO:                  snprintf_P(B, L, PSTR(D_GPIO_SDCARD_VSPI_MISO_CTR)); return B;

      // SD card using HSPI bus
    case GPIO_SDCARD_HSPI_CSO:                   snprintf_P(B, L, PSTR(D_GPIO_SDCARD_HSPI_CSO_CTR)); return B;
    case GPIO_SDCARD_HSPI_CLK:                   snprintf_P(B, L, PSTR(D_GPIO_SDCARD_HSPI_CLK_CTR)); return B;
    case GPIO_SDCARD_HSPI_MOSI:                  snprintf_P(B, L, PSTR(D_GPIO_SDCARD_HSPI_MOSI_CTR)); return B;
    case GPIO_SDCARD_HSPI_MISO:                  snprintf_P(B, L, PSTR(D_GPIO_SDCARD_HSPI_MISO_CTR)); return B;

      // SD card using native MMC bus
    case GPIO_SDCARD_MMC_CMD:                    snprintf_P(B, L, PSTR(D_GPIO_SDCARD_MMC_CMD_CTR)); return B;
    case GPIO_SDCARD_MMC_CLK:                    snprintf_P(B, L, PSTR(D_GPIO_SDCARD_MMC_CLK_CTR)); return B;
    case GPIO_SDCARD_MMC_D:                      snprintf_P(B, L, PSTR(D_GPIO_SDCARD_MMC_D_CTR)); return B;

      /****************************************
       * SECTION: UART
       ****************************************/

      // Generic hardware serial
    case GPIO_HWSERIAL_TX:                       snprintf_P(B, L, PSTR(D_GPIO_HWSERIAL_TX_CTR)); return B;
    case GPIO_HWSERIAL_RX:                       snprintf_P(B, L, PSTR(D_GPIO_HWSERIAL_RX_CTR)); return B;
      // Generic software serial
    case GPIO_SWSERIAL_TX:                       snprintf_P(B, L, PSTR(D_GPIO_SWSERIAL_TX_CTR)); return B;
    case GPIO_SWSERIAL_RX:                       snprintf_P(B, L, PSTR(D_GPIO_SWSERIAL_RX_CTR)); return B;
      // Ring-buffer serial modes, currently separate but later should become HWSERIAL + mode/config
    case GPIO_HWSERIAL_RING_BUFFER_TX:           snprintf_P(B, L, PSTR(D_GPIO_HWSERIAL_RING_BUFFER_TX_CTR)); return B;
    case GPIO_HWSERIAL_RING_BUFFER_RX:           snprintf_P(B, L, PSTR(D_GPIO_HWSERIAL_RING_BUFFER_RX_CTR)); return B;
      // Debug serial stream
    case GPIO_SERIAL_DEBUG_TX:                   snprintf_P(B, L, PSTR(D_GPIO_SERIAL_DEBUG_TX_CTR)); return B;
    case GPIO_SERIAL_DEBUG_RX:                   snprintf_P(B, L, PSTR(D_GPIO_SERIAL_DEBUG_RX_CTR)); return B;
      // Serial bridge
    case GPIO_SBR_TX:                            snprintf_P(B, L, PSTR(D_GPIO_SBR_TX_CTR)); return B;
    case GPIO_SBR_RX:                            snprintf_P(B, L, PSTR(D_GPIO_SBR_RX_CTR)); return B;

      /****************************************
       * SECTION: I2C
       ****************************************/

      // Generic I2C bus pins
    case GPIO_I2C_SCL:                           snprintf_P(B, L, PSTR(D_GPIO_I2C_SCL_CTR)); return B;
    case GPIO_I2C_SDA:                           snprintf_P(B, L, PSTR(D_GPIO_I2C_SDA_CTR)); return B;

      /****************************************
       * SECTION: SPI
       ****************************************/

      // Generic hardware SPI pins
    case GPIO_SPI_CS:                            snprintf_P(B, L, PSTR(D_GPIO_SPI_CS_CTR)); return B;
    case GPIO_SPI_DC:                            snprintf_P(B, L, PSTR(D_GPIO_SPI_DC_CTR)); return B;
      // Generic software SPI pins
    case GPIO_SSPI_MISO:                         snprintf_P(B, L, PSTR(D_GPIO_SSPI_MISO_CTR)); return B;
    case GPIO_SSPI_MOSI:                         snprintf_P(B, L, PSTR(D_GPIO_SSPI_MOSI_CTR)); return B;
    case GPIO_SSPI_SCLK:                         snprintf_P(B, L, PSTR(D_GPIO_SSPI_SCLK_CTR)); return B;
    case GPIO_SSPI_CS:                           snprintf_P(B, L, PSTR(D_GPIO_SSPI_CS_CTR)); return B;
    case GPIO_SSPI_DC:                           snprintf_P(B, L, PSTR(D_GPIO_SSPI_DC_CTR)); return B;

      /****************************************
       * SECTION: ADC Internal
       ****************************************/

      // ESP ADC internal temperature input
    case GPIO_ADC0_TEMPERATURE:                  snprintf_P(B, L, PSTR(D_GPIO_ADC0_TEMPERATURE_CTR)); return B;
      // ESP ADC1 hardware channels
    case GPIO_ADC1_CH4:                          snprintf_P(B, L, PSTR(D_GPIO_ADC1_CH4_CTR)); return B;
    case GPIO_ADC1_CH6:                          snprintf_P(B, L, PSTR(D_GPIO_ADC1_CH6_CTR)); return B;
    case GPIO_ADC1_CH7:                          snprintf_P(B, L, PSTR(D_GPIO_ADC1_CH7_CTR)); return B;
      // ADC external interrupt/trigger input
    case GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER:   snprintf_P(B, L, PSTR(D_GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_CTR)); return B;

      /****************************************
       * SECTION: PWM
       ****************************************/

      // PWM output
    case GPIO_PWM1:                               snprintf_P(B, L, PSTR(D_GPIO_PWM1_CTR)); return B;
      // PWM output, inverted
    case GPIO_PWM1_INV:                           snprintf_P(B, L, PSTR(D_GPIO_PWM1_INV_CTR)); return B;


  /****************************************
   * SECTION: Drivers
   ****************************************/

      /****************************************
       * SECTION: LED
       ****************************************/

      // LED output
    case GPIO_LED1:                               snprintf_P(B, L, PSTR(D_GPIO_LED1_CTR)); return B;
      // LED output, inverted
    case GPIO_LED1_INV:                           snprintf_P(B, L, PSTR(D_GPIO_LED1_INV_CTR)); return B;

      /****************************************
       * SECTION: Relay
       ****************************************/

      // Relay output
    case GPIO_REL1:                               snprintf_P(B, L, PSTR(D_GPIO_REL1_CTR)); return B;
      // Relay output, inverted
    case GPIO_REL1_INV:                           snprintf_P(B, L, PSTR(D_GPIO_REL1_INV_CTR)); return B;

      /****************************************
       * SECTION: IR
       ****************************************/

      // Infrared send/receive
    case GPIO_IRSEND:                            snprintf_P(B, L, PSTR(D_GPIO_IRSEND_CTR)); return B;
    case GPIO_IRRECV:                            snprintf_P(B, L, PSTR(D_GPIO_IRRECV_CTR)); return B;

      /****************************************
       * SECTION: Servos
       ****************************************/

      // Servo output
    case GPIO_SERVO:                             snprintf_P(B, L, PSTR(D_GPIO_SERVO_CTR)); return B;

      /****************************************
       * SECTION: SAW RF
       ****************************************/

      // Generic RF send/receive
    case GPIO_RFSEND:                            snprintf_P(B, L, PSTR(D_GPIO_RFSEND_CTR)); return B;
    case GPIO_RFRECV:                            snprintf_P(B, L, PSTR(D_GPIO_RFRECV_CTR)); return B;
      // 433 MHz RF send/receive
    case GPIO_RF_433MHZ_RX:                      snprintf_P(B, L, PSTR(D_GPIO_RF_433MHZ_RX_CTR)); return B;
    case GPIO_RF_433MHZ_TX:                      snprintf_P(B, L, PSTR(D_GPIO_RF_433MHZ_TX_CTR)); return B;

      /****************************************
       * SECTION: HBridge/Motor L9110
       ****************************************/

      // L9110 H-bridge control pins
    case GPIO_HBRIDGE_L9110_IA:                  snprintf_P(B, L, PSTR(D_GPIO_HBRIDGE_L9110_IA_CTR)); return B;
    case GPIO_HBRIDGE_L9110_IB:                  snprintf_P(B, L, PSTR(D_GPIO_HBRIDGE_L9110_IB_CTR)); return B;
    case GPIO_HBRIDGE_L9110_OA:                  snprintf_P(B, L, PSTR(D_GPIO_HBRIDGE_L9110_OA_CTR)); return B;
    case GPIO_HBRIDGE_L9110_OB:                  snprintf_P(B, L, PSTR(D_GPIO_HBRIDGE_L9110_OB_CTR)); return B;

      /****************************************
       * SECTION: Shelly Dimmer
       ****************************************/

      // Shelly 2 dimmer control pins
    case GPIO_SHELLY2_SHD_BOOT0:                 snprintf_P(B, L, PSTR(D_GPIO_SHELLY2_SHD_BOOT0_CTR)); return B;
    case GPIO_SHELLY2_SHD_RESET_INV:             snprintf_P(B, L, PSTR(D_GPIO_SHELLY2_SHD_RESET_INV_CTR)); return B;

      /****************************************
       * SECTION: Buzzer
       ****************************************/

      // Buzzer output
    case GPIO_BUZZER:                            snprintf_P(B, L, PSTR(D_GPIO_BUZZER_CTR)); return B;
      // Buzzer output, inverted
    case GPIO_BUZZER_INV:                        snprintf_P(B, L, PSTR(D_GPIO_BUZZER_INV_CTR)); return B;

      /****************************************
       * SECTION: Cellular Modem
       ****************************************/

      // Cellular modem UART
    case GPIO_MODEM_RX:                          snprintf_P(B, L, PSTR(D_GPIO_MODEM_RX_CTR)); return B;
    case GPIO_MODEM_TX:                          snprintf_P(B, L, PSTR(D_GPIO_MODEM_TX_CTR)); return B;
      // Cellular modem control/status pins
    case GPIO_MODEM_POWER_KEY:                   snprintf_P(B, L, PSTR(D_GPIO_MODEM_POWER_KEY_CTR)); return B;
    case GPIO_MODEM_DATA_TERMINAL_READY_DTR:     snprintf_P(B, L, PSTR(D_GPIO_MODEM_DATA_TERMINAL_READY_DTR_CTR)); return B;
    case GPIO_MODEM_RING_INDICATOR:              snprintf_P(B, L, PSTR(D_GPIO_MODEM_RING_INDICATOR_CTR)); return B;
    case GPIO_MODEM_POWER_STATUS:                snprintf_P(B, L, PSTR(D_GPIO_MODEM_POWER_STATUS_CTR)); return B;
    case GPIO_MODEM_NETWORK_STATUS:              snprintf_P(B, L, PSTR(D_GPIO_MODEM_NETWORK_STATUS_CTR)); return B;

      /****************************************
       * SECTION: Cameras
       ****************************************/

      // Camera external clock
    case GPIO_WEBCAM_XCLK:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_XCLK_CTR)); return B;
      // Camera parallel data bus
    case GPIO_WEBCAM_DATA:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_DATA_CTR)); return B;
      // Camera timing/control pins
    case GPIO_WEBCAM_PCLK:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_PCLK_CTR)); return B;
    case GPIO_WEBCAM_HREF:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_HREF_CTR)); return B;
    case GPIO_WEBCAM_VSYNC:                      snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_VSYNC_CTR)); return B;
      // Camera SCCB/I2C pins
    case GPIO_WEBCAM_SIOD:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_SIOD_CTR)); return B;
    case GPIO_WEBCAM_SIOC:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_SIOC_CTR)); return B;
      // Camera power/reset pins
    case GPIO_WEBCAM_PWDN:                       snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_PWDN_CTR)); return B;
    case GPIO_WEBCAM_RESET:                      snprintf_P(B, L, PSTR(D_GPIO_WEBCAM_RESET_CTR)); return B;

      /****************************************
       * SECTION: MAVLink
       ****************************************/

      // MAVLink serial pins
    case GPIO_MAVLINK_TX:                        snprintf_P(B, L, PSTR(D_GPIO_MAVLINK_TX_CTR)); return B;
    case GPIO_MAVLINK_RX:                        snprintf_P(B, L, PSTR(D_GPIO_MAVLINK_RX_CTR)); return B;


  /****************************************
   * SECTION: Sensors
   ****************************************/

      /****************************************
       * SECTION: Switches
       ****************************************/

      // Switch input
    case GPIO_SWT1:                               snprintf_P(B, L, PSTR(D_GPIO_SWT1_CTR)); return B;
      // Switch input, inverted
    case GPIO_SWT1_INV:                           snprintf_P(B, L, PSTR(D_GPIO_SWT1_INV_CTR)); return B;
      // Switch input, no pull
    case GPIO_SWT1_NP:                            snprintf_P(B, L, PSTR(D_GPIO_SWT1_NP_CTR)); return B;
      // Switch input, inverted no pull
    case GPIO_SWT1_INV_NP:                        snprintf_P(B, L, PSTR(D_GPIO_SWT1_INV_NP_CTR)); return B;

      /****************************************
       * SECTION: Buttons
       ****************************************/

      // Button input
    case GPIO_KEY1:                               snprintf_P(B, L, PSTR(D_GPIO_KEY1_CTR)); return B;
      // Button input, inverted
    case GPIO_KEY1_INV:                           snprintf_P(B, L, PSTR(D_GPIO_KEY1_INV_CTR)); return B;
      // Button input, no pull
    case GPIO_KEY1_NP:                            snprintf_P(B, L, PSTR(D_GPIO_KEY1_NP_CTR)); return B;
      // Button input, inverted no pull
    case GPIO_KEY1_INV_NP:                        snprintf_P(B, L, PSTR(D_GPIO_KEY1_INV_NP_CTR)); return B;
      // Touch button input
    case GPIO_KEY1_TOUCH:                         snprintf_P(B, L, PSTR(D_GPIO_KEY1_TOUCH_CTR)); return B;

      /****************************************
       * SECTION: BME
       ****************************************/

      // BME sensor uses I2C, no dedicated GPIO function

      /****************************************
       * SECTION: DB18X20
       ****************************************/

      // DS18x20 one-wire data pin
    case GPIO_DS18B20:                               snprintf_P(B, L, PSTR(D_GPIO_DS18B20_CTR)); return B;

      /****************************************
       * SECTION: DHT11/DHT22
       ****************************************/

      // DHT11 data pin
    case GPIO_DHT11:                             snprintf_P(B, L, PSTR(D_GPIO_DHT11_CTR)); return B;
      // DHT22 data pin
    case GPIO_DHT22:                             snprintf_P(B, L, PSTR(D_GPIO_DHT22_CTR)); return B;

      /****************************************
       * SECTION: BH1750
       ****************************************/

      // BH1750 uses I2C, no dedicated GPIO function

      /****************************************
       * SECTION: PIR
       ****************************************/

      // PIR input
    case GPIO_PIR:                               snprintf_P(B, L, PSTR(D_GPIO_PIR_CTR)); return B;
      // PIR input, inverted
    case GPIO_PIR_INV:                           snprintf_P(B, L, PSTR(D_GPIO_PIR_INV_CTR)); return B;

      /****************************************
       * SECTION: Door
       ****************************************/

      // Door lock/open inputs or outputs
    case GPIO_DOOR_LOCK:                         snprintf_P(B, L, PSTR(D_GPIO_DOOR_LOCK_CTR)); return B;
    case GPIO_DOOR_OPEN:                         snprintf_P(B, L, PSTR(D_GPIO_DOOR_OPEN_CTR)); return B;

      /****************************************
       * SECTION: L3GD20
       ****************************************/

      // L3GD20 uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: LDR Analog + Digital Trigger
       ****************************************/

      // LDR digital trigger input
    case GPIO_LDR_BASIC_DIGITAL:                 snprintf_P(B, L, PSTR(D_GPIO_LDR_BASIC_DIGITAL_CTR)); return B;
      // LDR analog input
    case GPIO_LDR_BASIC_ANALOG:                  snprintf_P(B, L, PSTR(D_GPIO_LDR_BASIC_ANALOG_CTR)); return B;

      /****************************************
       * SECTION: LSM303D
       ****************************************/

      // LSM303D uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: Moisture Analog
       ****************************************/

      // Moisture analog input
    case GPIO_MOISTURE_ANALOG:                   snprintf_P(B, L, PSTR(D_GPIO_MOISTURE_ANALOG_CTR)); return B;

      /****************************************
       * SECTION: SR04
       ****************************************/

      // Ultrasonic trigger/echo pins
    case GPIO_SR04_TRIG:                         snprintf_P(B, L, PSTR(D_GPIO_SR04_TRIG_CTR)); return B;
    case GPIO_SR04_ECHO:                         snprintf_P(B, L, PSTR(D_GPIO_SR04_ECHO_CTR)); return B;

      /****************************************
       * SECTION: MPU9250
       ****************************************/

      // MPU9250 uses SPI/I2C, no dedicated GPIO function currently

      /****************************************
       * SECTION: Pulse Counter
       ****************************************/

      // Pulse counter input
    case GPIO_PULSE_COUNTER1:                     snprintf_P(B, L, PSTR(D_GPIO_PULSE_COUNTER1_CTR)); return B;
      // Pulse counter input, no pull
    case GPIO_PULSE_COUNTER1_NP:                  snprintf_P(B, L, PSTR(D_GPIO_PULSE_COUNTER1_NP_CTR)); return B;

      /****************************************
       * SECTION: Remote Device
       ****************************************/

      // Virtual GPIO representing remote device/link input
    case GPIO_REMOTE_DEVICE:                     snprintf_P(B, L, PSTR(D_GPIO_REMOTE_DEVICE_CTR)); return B;

      /****************************************
       * SECTION: Rotary Encoder
       ****************************************/

      // Rotary encoder A/B inputs
    case GPIO_ROTARY_ENCODER_A:                  snprintf_P(B, L, PSTR(D_GPIO_ROTARY_ENCODER_A_CTR)); return B;
    case GPIO_ROTARY_ENCODER_B:                  snprintf_P(B, L, PSTR(D_GPIO_ROTARY_ENCODER_B_CTR)); return B;

      /****************************************
       * SECTION: Sun Tracking
       ****************************************/

      // Virtual sun tracking function, no physical GPIO
    case GPIO_SUN_TRACKING:                      snprintf_P(B, L, PSTR(D_GPIO_SUN_TRACKING_CTR)); return B;

      /****************************************
       * SECTION: Moon Tracking
       ****************************************/

      // Virtual moon tracking function, no physical GPIO
    case GPIO_MOON_TRACKING:                     snprintf_P(B, L, PSTR(D_GPIO_MOON_TRACKING_CTR)); return B;

      /****************************************
       * SECTION: TOF Sensors
       ****************************************/

      // VL53L0X XSHUT pin
    case GPIO_VL53L0X_XSHUT1:                     snprintf_P(B, L, PSTR(D_GPIO_VL53L0X_XSHUT_CTR)); return B;
      // VL53L1X XSHUT pin
    case GPIO_VL53L1X_XSHUT1:                     snprintf_P(B, L, PSTR(D_GPIO_VL53L1X_XSHUT_CTR)); return B;

      /****************************************
       * SECTION: Radar HLK LD2410
       ****************************************/

      // LD2410 UART pins
    case GPIO_LD2410_RX:                         snprintf_P(B, L, PSTR(D_GPIO_LD2410_RX_CTR)); return B;
    case GPIO_LD2410_TX:                         snprintf_P(B, L, PSTR(D_GPIO_LD2410_TX_CTR)); return B;

      /****************************************
       * SECTION: GPS Serial
       ****************************************/

      // GPS serial pins
    case GPIO_GPS_SERIAL_TX:                     snprintf_P(B, L, PSTR(D_GPIO_GPS_SERIAL_TX_CTR)); return B;
    case GPIO_GPS_SERIAL_RX:                     snprintf_P(B, L, PSTR(D_GPIO_GPS_SERIAL_RX_CTR)); return B;


  /****************************************
   * SECTION: Energy
   ****************************************/

      /****************************************
       * SECTION: PZEM
       ****************************************/

      // PZEM serial pins
    case GPIO_PZEM0XX_TX:                        snprintf_P(B, L, PSTR(D_GPIO_PZEM0XX_TX_CTR)); return B;
    case GPIO_PZEM004_RX:                        snprintf_P(B, L, PSTR(D_GPIO_PZEM004_RX_CTR)); return B;
    case GPIO_PZEM0XX_RX_MODBUS:                 snprintf_P(B, L, PSTR(D_GPIO_PZEM0XX_RX_MODBUS_CTR)); return B;
    case GPIO_PZEM017_RX:                        snprintf_P(B, L, PSTR(D_GPIO_PZEM017_RX_CTR)); return B;

      /****************************************
       * SECTION: SDM serial meters
       ****************************************/

      // SDM120 serial pins
    case GPIO_SDM120_TX:                         snprintf_P(B, L, PSTR(D_GPIO_SDM120_TX_CTR)); return B;
    case GPIO_SDM120_RX:                         snprintf_P(B, L, PSTR(D_GPIO_SDM120_RX_CTR)); return B;
      // SDM630 serial pins
    case GPIO_SDM630_TX:                         snprintf_P(B, L, PSTR(D_GPIO_SDM630_TX_CTR)); return B;
    case GPIO_SDM630_RX:                         snprintf_P(B, L, PSTR(D_GPIO_SDM630_RX_CTR)); return B;

      /****************************************
       * SECTION: MCP39F5
       ****************************************/

      // MCP39F5 serial/reset pins
    case GPIO_MCP39F5_TX:                        snprintf_P(B, L, PSTR(D_GPIO_MCP39F5_TX_CTR)); return B;
    case GPIO_MCP39F5_RX:                        snprintf_P(B, L, PSTR(D_GPIO_MCP39F5_RX_CTR)); return B;
    case GPIO_MCP39F5_RST:                       snprintf_P(B, L, PSTR(D_GPIO_MCP39F5_RST_CTR)); return B;

      /****************************************
       * SECTION: CSE7766
       ****************************************/

      // CSE7766 serial pins
    case GPIO_CSE7766_TX:                        snprintf_P(B, L, PSTR(D_GPIO_CSE7766_TX_CTR)); return B;
    case GPIO_CSE7766_RX:                        snprintf_P(B, L, PSTR(D_GPIO_CSE7766_RX_CTR)); return B;

      /****************************************
       * SECTION: ADE7953
       ****************************************/

      // ADE7953 interrupt pin
    case GPIO_ADE7953_IRQ:                       snprintf_P(B, L, PSTR(D_GPIO_ADE7953_IRQ_CTR)); return B;


  /****************************************
   * SECTION: Displays
   ****************************************/

      // Display backlight output
    case GPIO_BACKLIGHT:                         snprintf_P(B, L, PSTR(D_GPIO_BACKLIGHT_CTR)); return B;
      // OLED reset pin
    case GPIO_OLED_RESET:                        snprintf_P(B, L, PSTR(D_GPIO_OLED_RESET_CTR)); return B;

      /****************************************
       * SECTION: Nextion
       ****************************************/

      // Nextion serial pins
    case GPIO_NEXTION_RX:                        snprintf_P(B, L, PSTR(D_GPIO_NEXTION_RX_CTR)); return B;
    case GPIO_NEXTION_TX:                        snprintf_P(B, L, PSTR(D_GPIO_NEXTION_TX_CTR)); return B;

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
    case GPIO_FAN_PWM:                           snprintf_P(B, L, PSTR(D_GPIO_FAN_PWM_CTR)); return B;

      /****************************************
       * SECTION: Door / Lock / Chime
       ****************************************/

      // Chime input/ringer pins
    case GPIO_CHIME_INPUT:                       snprintf_P(B, L, PSTR(D_GPIO_CHIME_INPUT_CTR)); return B;
    case GPIO_CHIME_RINGER:                      snprintf_P(B, L, PSTR(D_GPIO_CHIME_RINGER_CTR)); return B;

      /****************************************
       * SECTION: Sampling / Sync Signals
       ****************************************/

      // Sampling enable signal
    case GPIO_RXON_SAMPLING_ENABLED:             snprintf_P(B, L, PSTR(D_GPIO_RXON_SAMPLING_ENABLED_CTR)); return B;
      // CC1110 sync pulse signal
    case GPIO_CC1110_SYNC_PULSE_SIGNAL:          snprintf_P(B, L, PSTR(D_GPIO_CC1110_SYNC_PULSE_SIGNAL_CTR)); return B;

    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    case GPIO_LIGHTING_DIGITAL:                  snprintf_P(B, L, PSTR(D_GPIO_LIGHTING_DIGITAL_CTR)); return B;
    case GPIO_LIGHTING_CLOCK:                    snprintf_P(B, L, PSTR(D_GPIO_LIGHTING_CLOCK_CTR)); return B;
    case GPIO_LIGHTING_PWM:                      snprintf_P(B, L, PSTR(D_GPIO_LIGHTING_PWM_CTR)); return B;
    case GPIO_LIGHTING_ONOFF:                    snprintf_P(B, L, PSTR(D_GPIO_LIGHTING_ONOFF_CTR)); return B;
    #endif

  /****************************************
   * SECTION: End Markers
   ****************************************/

  // End marker for selectable GPIO base IDs
  // Transitional compatibility alias for legacy code that still checks GPIO_SELECTABLE_END

    default:
      return nullptr;
  }
}

/*********************************************************************************************\
 * Generated indexed name
\*********************************************************************************************/

static const char* GPIOName_Build(uint16_t base_id, uint8_t index, uint8_t count, char* B, uint8_t L)
{
  if (!B || !L)
  {
    return "";
  }

  B[0] = '\0';

  char base_name[64];
  GPIOBaseName_ByID(base_id, base_name, sizeof(base_name));

  if (base_name[0] == '\0')
  {
    ALOG_ERR(PSTR("GPIONameBuild: missing base name base=%u index=%u count=%u"), base_id, index, count);
    snprintf_P(B, L, PSTR("GPIO_%u"), base_id);
    return B;
  }

  if (count > 1)
  {
    const uint8_t display_index = GPIOBaseName_UsesOneBasedIndex(base_id) ? (index + 1u) : index;
    snprintf_P(B, L, PSTR("%s%u"), base_name, display_index);
  }
  else
  {
    snprintf_P(B, L, PSTR("%s"), base_name);
  }

  return B;
}

/*********************************************************************************************\
 * Public API: packed ID -> name
\*********************************************************************************************/

const char* mPins::GetGPIOFunctionNamebyID(uint16_t id, char* B, uint8_t L)
{
  if (!B || !L)
  {
    return nullptr;
  }

  B[0] = '\0';

  const uint16_t base_id = GPIOPacked_Base(id);
  const uint8_t  index   = GPIOPacked_Index(id);

  for (uint16_t entry_i = 0; entry_i < SelectablePins_BitPacked_Count; entry_i++)
  {
    const uint16_t entry      = pgm_read_word(&SelectablePins_BitPacked[entry_i]);
    const uint16_t entry_base = GPIOEntry_Base(entry);

    if (entry_base == base_id)
    {
      const uint8_t count = GPIOEntry_Count(entry);

      if (index >= count)
      {
        snprintf_P(B, L, PSTR("GPIOInvalid0x%04X"), id);
        return B;
      }

      return GPIOName_Build(base_id, index, count, B, L);
    }
  }

  snprintf_P(B, L, PSTR("GPIOUnknown0x%04X"), id);
  return B;
}
/*********************************************************************************************\
 * Helper: base IDs where numeric suffix is part of a zero-based signal name
 *
 * Normal repeated GPIO functions use 1-based external suffixes:
 *   Relay1 -> index 0
 *   Relay2 -> index 1
 *   DHT221 -> index 0
 *   DHT222 -> index 1
 *
 * Some protocol/signal names are inherently zero-based:
 *   SDCard MMC D0 -> index 0
 *   SDCard MMC D1 -> index 1
 *   SDCard MMC D2 -> index 2
 *   SDCard MMC D3 -> index 3
\*********************************************************************************************/
bool mPins::GPIOBase_UsesZeroBasedSuffix(uint16_t base_id)
{
  switch (base_id)
  {
    case GPIO_SDCARD_MMC_D:
      return true;

    default:
      return false;
  }
}


/*********************************************************************************************\
 * Public API: name -> packed ID
\*********************************************************************************************/
int32_t mPins::GetGPIOFunctionIDbyName(const char* c)
{
  if (!c || !c[0])
  {
    ALOG_ERR(PSTR("GPIONameSearch: invalid input c=%p"), c);
    return -1;
  }

  ALOG_DBG(PSTR("GPIONameSearch: search=\"%s\" count=%u"), c, SelectablePins_BitPacked_Count);

  uint16_t best_base_id = 0;
  uint8_t  best_count = 0;
  uint8_t  best_index = 0;
  size_t   best_base_len = 0;
  bool     best_found = false;

  for (uint16_t entry_i = 0; entry_i < SelectablePins_BitPacked_Count; entry_i++)
  {
    const uint16_t entry   = pgm_read_word(&SelectablePins_BitPacked[entry_i]);
    const uint16_t base_id = GPIOEntry_Base(entry);
    const uint8_t  count   = GPIOEntry_Count(entry);

    char base_name[64] = {0};
    GPIOBaseName_ByID(base_id, base_name, sizeof(base_name));

    ALOG_DBM(PSTR("GPIONameSearch: entry[%u] entry=0x%04X base=%u count=%u base_name=\"%s\""), entry_i, entry, base_id, count, base_name);

    if (base_name[0] == '\0')
    {
      ALOG_ERR(PSTR("GPIONameSearch: missing base name base=%u entry=0x%04X"), base_id, entry);
      continue;
    }

    const size_t base_len = strlen(base_name);

    /*
     * Decode rule:
     *   input = base_name + optional numeric suffix
     *
     * Most repeated functions use 1-based suffixes:
     *   DHT22    -> base DHT22, index 0
     *   DHT221   -> base DHT22, index 0
     *   DHT222   -> base DHT22, index 1
     *   DSB1     -> base DSB,   index 0
     *   DSB2     -> base DSB,   index 1
     *
     * Some signal-lane functions use zero-based suffixes:
     *   SDCard MMC D0 -> base SDCard MMC D, index 0
     *   SDCard MMC D1 -> base SDCard MMC D, index 1
     *   SDCard MMC D2 -> base SDCard MMC D, index 2
     *   SDCard MMC D3 -> base SDCard MMC D, index 3
     */
    if (!StringStartsWithNoCase(c, base_name))
    {
      continue;
    }

    /*
     * Longest base-name wins.
     *
     * Required for base names that already contain or end in digits:
     *   DHT222    -> DHT22   + 2
     *   DS18B202  -> DS18B20 + 2
     *   BME2802   -> BME280  + 2
     */
    if (base_len < best_base_len)
    {
      continue;
    }

    const char* suffix = &c[base_len];

    uint8_t decoded_index = 0;

    if (suffix[0] == '\0')
    {
      decoded_index = 0;
    }
    else
    {
      uint16_t suffix_value = 0;
      bool suffix_valid = true;

      for (uint8_t i = 0; suffix[i]; i++)
      {
        if ((suffix[i] < '0') || (suffix[i] > '9'))
        {
          suffix_valid = false;
          break;
        }

        suffix_value = (suffix_value * 10) + (suffix[i] - '0');

        if (suffix_value > 255)
        {
          suffix_valid = false;
          break;
        }
      }

      if (!suffix_valid)
      {
        continue;
      }

      if (GPIOBase_UsesZeroBasedSuffix(base_id))
      {
        /*
         * Zero-based suffix:
         *   suffix 0 -> internal index 0
         *   suffix 1 -> internal index 1
         *   suffix 2 -> internal index 2
         */
        decoded_index = (uint8_t)suffix_value;
      }
      else
      {
        /*
         * Normal 1-based suffix:
         *   suffix 1 -> internal index 0
         *   suffix 2 -> internal index 1
         *
         * Suffix 0 is rejected here to avoid two names for the same indexed GPIO.
         */
        if (suffix_value == 0)
        {
          continue;
        }

        decoded_index = (uint8_t)(suffix_value - 1);
      }
    }

    if (decoded_index >= count)
    {
      continue;
    }

    best_base_id = base_id;
    best_count = count;
    best_index = decoded_index;
    best_base_len = base_len;
    best_found = true;

    ALOG_DBM(PSTR("GPIONameSearch: candidate search=\"%s\" base_name=\"%s\" base=%u index=%u count=%u base_len=%u zero_based=%u"), c, base_name, base_id, decoded_index, count, (uint16_t)base_len, GPIOBase_UsesZeroBasedSuffix(base_id));
  }

  if (best_found)
  {
    const uint16_t packed_id = GPIOPacked_Make(best_base_id, best_index);
    ALOG_INF(PSTR("GPIONameSearch: MATCH search=\"%s\" base=%u index=%u count=%u packed=0x%04X return=%d"), c, best_base_id, best_index, best_count, packed_id, (int32_t)packed_id);
    return packed_id;
  }

  ALOG_ERR(PSTR("GPIONameSearch: NO MATCH search=\"%s\""), c);

  return -1;
}