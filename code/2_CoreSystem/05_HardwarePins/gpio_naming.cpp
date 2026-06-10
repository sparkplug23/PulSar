#include "mPins.h"

/****
 * Since this is a long list, the GPIO from ID to name and back is defined in a separate file.
 * Keep this file in the same order as gpio_list_ids.h.
 */

// Function: Checks for gpio function as stored in progmem, returning its identifier if true, else -1
// Returns : #>=0:id found, -1:not found
// Notes   : "else" is intentionally not used because returns exit early.
//
int16_t mPins::GetGPIOFunctionIDbyName(const char* c)
{

  if(*c=='\0'){ return -1; }

  char buffer[50];


  /******************************************************************************************************************
   * Unassigned pin states
   *******************************************************************************************************************/

  if(strcmp_P(c, PM_GPIO_FUNCTION_NONE_CTR)==0){ return GPIO_NONE; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR)==0){ return GPIO_UNUSED_FORCED_HIGH; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR)==0){ return GPIO_UNUSED_FORCED_LOW; }


  /******************************************************************************************************************
   * Core System / Filesystem + SD Card
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_SDCARD
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR)==0){ return GPIO_FUNCTION_SDCARD_VSPI_CSO; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR)==0){ return GPIO_FUNCTION_SDCARD_VSPI_CLK; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR)==0){ return GPIO_FUNCTION_SDCARD_VSPI_MOSI; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR)==0){ return GPIO_FUNCTION_SDCARD_VSPI_MISO; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR)==0){ return GPIO_FUNCTION_SDCARD_HSPI_CSO; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR)==0){ return GPIO_FUNCTION_SDCARD_HSPI_CLK; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR)==0){ return GPIO_FUNCTION_SDCARD_HSPI_MOSI; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR)==0){ return GPIO_FUNCTION_SDCARD_HSPI_MISO; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_CLK_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_CLK; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_CMD_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_CMD; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_D0_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_D0; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_D1_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_D1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_D2_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_D2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SDCARD_MMC_D3_CTR)==0){ return GPIO_FUNCTION_SDCARD_MMC_D3; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Hardware Serial0
   *******************************************************************************************************************/

  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)==0){ return GPIO_HWSERIAL0_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)==0){ return GPIO_HWSERIAL0_RX; }


  /******************************************************************************************************************
   * Core System / UART: Hardware Serial1/2
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE__SERIAL
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR)==0){ return GPIO_HWSERIAL1_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR)==0){ return GPIO_HWSERIAL1_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR)==0){ return GPIO_HWSERIAL2_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR)==0){ return GPIO_HWSERIAL2_RX; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Ring-buffer serial modes
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_HWSERIAL0_RING_BUFFER_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_HWSERIAL0_RING_BUFFER_TX; }
  // TODO: add PROGMEM name macro for GPIO_HWSERIAL0_RING_BUFFER_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_HWSERIAL0_RING_BUFFER_RX; }
  #endif
  #ifdef ESP32
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR)==0){ return GPIO_HWSERIAL1_RING_BUFFER_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR)==0){ return GPIO_HWSERIAL1_RING_BUFFER_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR)==0){ return GPIO_HWSERIAL2_RING_BUFFER_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR)==0){ return GPIO_HWSERIAL2_RING_BUFFER_RX; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Debug serial
   *******************************************************************************************************************/

  if(strcmp_P(c, PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)==0){ return GPIO_SERIAL_DEBUG_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)==0){ return GPIO_SERIAL_DEBUG_RX; }


  /******************************************************************************************************************
   * Core System / UART: Software serial and serial bridge placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SWSERIAL0_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SWSERIAL0_TX; }
  // TODO: add PROGMEM name macro for GPIO_SWSERIAL0_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SWSERIAL0_RX; }
  // TODO: add PROGMEM name macro for GPIO_SBR_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SBR_TX; }
  // TODO: add PROGMEM name macro for GPIO_SBR_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SBR_RX; }
  #endif


  /******************************************************************************************************************
   * Core System / I2C
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE_I2C
  if(strcmp_P(c, PM_GPIO_FUNCTION_I2C_SCL_CTR)==0){ return GPIO_I2C_SCL; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_I2C_SDA_CTR)==0){ return GPIO_I2C_SDA; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_I2C_SCL2_CTR)==0){ return GPIO_I2C_SCL2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_I2C_SDA2_CTR)==0){ return GPIO_I2C_SDA2; }
  #endif

  /******************************************************************************************************************
   * Core System / SPI placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SPI_CS
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SPI_CS; }
  // TODO: add PROGMEM name macro for GPIO_SPI_DC
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SPI_DC; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_MISO
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SSPI_MISO; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_MOSI
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SSPI_MOSI; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_SCLK
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SSPI_SCLK; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_CS
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SSPI_CS; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_DC
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SSPI_DC; }
  #endif


  /******************************************************************************************************************
   * Core System / ADC
   *******************************************************************************************************************/

  if(strcmp_P(c, PM_GPIO_FUNCTION_ADC1_CH4_CTR)==0){ return GPIO_ADC1_CH4; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_ADC1_CH6_CTR)==0){ return GPIO_ADC1_CH6; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_ADC1_CH7_CTR)==0){ return GPIO_ADC1_CH7; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR)==0){ return GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER; }


  /******************************************************************************************************************
   * Core System / ADC placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_ADC0_TEMPERATURE
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_ADC0_TEMPERATURE; }
  #endif


  /******************************************************************************************************************
   * Core System / PWM
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE_PWM
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM1_CTR)==0){ return GPIO_PWM1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM2_CTR)==0){ return GPIO_PWM2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM3_CTR)==0){ return GPIO_PWM3; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM4_CTR)==0){ return GPIO_PWM4; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM5_CTR)==0){ return GPIO_PWM5; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM1_INV_CTR)==0){ return GPIO_PWM1_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM2_INV_CTR)==0){ return GPIO_PWM2_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM3_INV_CTR)==0){ return GPIO_PWM3_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM4_INV_CTR)==0){ return GPIO_PWM4_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PWM5_INV_CTR)==0){ return GPIO_PWM5_INV; }
  #endif


  /******************************************************************************************************************
   * Drivers / LED
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_LEDS
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_LED1 + (i - 1); }
  }
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_INV_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_LED1_INV + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Drivers / Relay
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_RELAY
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_REL_NUM_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_REL1 + (i - 1); }
  }
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_REL_NUM_INV_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_REL1_INV + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Drivers / IR placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_IRSEND
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_IRSEND; }
  // TODO: add PROGMEM name macro for GPIO_IRRECV
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_IRRECV; }
  #endif


  /******************************************************************************************************************
   * Drivers / SAW RF placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_RFSEND
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_RFSEND; }
  // TODO: add PROGMEM name macro for GPIO_RFRECV
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_RFRECV; }
  #endif


  /******************************************************************************************************************
   * Drivers / 433 MHz RF
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_RF433_CODES
  if(strcmp_P(c, PM_GPIO_FUNCTION__RF_433MHZ_RX__CTR)==0){ return GPIO_RF_433MHZ_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__RF_433MHZ_TX__CTR)==0){ return GPIO_RF_433MHZ_TX; }
  #endif


  /******************************************************************************************************************
   * Drivers / HBridge/Motor L9110
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  if(strcmp_P(c, PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){ return GPIO_HBRIDGE_L9110_IA; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){ return GPIO_HBRIDGE_L9110_IB; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){ return GPIO_HBRIDGE_L9110_OA; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){ return GPIO_HBRIDGE_L9110_OB; }
  #endif


  /******************************************************************************************************************
   * Drivers / Shelly Dimmer
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  if(strcmp_P(c, PM_GPIO_FUNCTION_SHELLY2_SHD_BOOT0_CTR)==0){ return GPIO_SHELLY2_SHD_BOOT0; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SHELLY2_SHD_RESET_INV_CTR)==0){ return GPIO_SHELLY2_SHD_RESET_INV; }
  #endif


  /******************************************************************************************************************
   * Drivers / Buzzer
   *******************************************************************************************************************/

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  if(strcmp_P(c, PM_GPIO_FUNCTION_BUZZER_CTR)==0){ return GPIO_BUZZER; }
  #endif


  /******************************************************************************************************************
   * Drivers / Buzzer inverted placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_BUZZER_INV
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_BUZZER_INV; }
  #endif


  /******************************************************************************************************************
   * Drivers / Cellular Modem
   *******************************************************************************************************************/

  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_RX0__CTR)==0){ return GPIO_FUNCTION__MODEM_RX0; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_TX0__CTR)==0){ return GPIO_FUNCTION__MODEM_TX0; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_RX1__CTR)==0){ return GPIO_FUNCTION__MODEM_RX1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_TX1__CTR)==0){ return GPIO_FUNCTION__MODEM_TX1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_RX2__CTR)==0){ return GPIO_FUNCTION__MODEM_RX2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_TX2__CTR)==0){ return GPIO_FUNCTION__MODEM_TX2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__MODEM_POWER_KEY__CTR)==0){ return GPIO_FUNCTION__MODEM_POWER_KEY; }

  /******************************************************************************************************************
   * Drivers / Cellular Modem placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_RING_INDICATOR
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_FUNCTION__MODEM_RING_INDICATOR; }
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_POWER_STATUS
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_FUNCTION__MODEM_POWER_STATUS; }
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_NETWORK_STATUS
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_FUNCTION__MODEM_NETWORK_STATUS; }
  #endif


  /******************************************************************************************************************
   * Drivers / Cameras
   *******************************************************************************************************************/

  #ifdef ESP32
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)==0){ return GPIO_WEBCAM_XCLK; }
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_WEBCAM_DATA_NUM_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_WEBCAM_DATA1 + (i - 1); }
  }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)==0){ return GPIO_WEBCAM_PCLK; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)==0){ return GPIO_WEBCAM_HREF; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)==0){ return GPIO_WEBCAM_VSYNC; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)==0){ return GPIO_WEBCAM_SIOD; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)==0){ return GPIO_WEBCAM_SIOC; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)==0){ return GPIO_WEBCAM_PWDN; }
  #endif


  /******************************************************************************************************************
   * Drivers / Camera reset placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_WEBCAM_RESET
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_WEBCAM_RESET; }
  #endif


  /******************************************************************************************************************
   * Sensors / Switches
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_SWITCHES
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_SWT1 + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_SWT1_INV + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_SWT1_NP + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_SWT1_INV_NP + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Sensors / Buttons
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_BUTTONS
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_KEY1 + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_KEY1_INV + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_KEY1_NP + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_KEY1_INV_NP + (i - 1); }
  }
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_KEY1_TOUCH + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Sensors / DS18X20
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DS18X20
  if(strcmp_P(c, PM_GPIO_FUNCTION_DS18X20_1_CTR)==0){ return GPIO_DSB_1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_DS18X20_2_CTR)==0){ return GPIO_DSB_2; }
  #endif


  /******************************************************************************************************************
   * Sensors / DHT11/DHT22
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DHT
  if(strcmp_P(c, PM_GPIO_FUNCTION_DHT11_1_CTR)==0){ return GPIO_DHT11_1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_DHT11_2_CTR)==0){ return GPIO_DHT11_2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_DHT22_1_CTR)==0){ return GPIO_DHT22_1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_DHT22_2_CTR)==0){ return GPIO_DHT22_2; }
  #endif


  /******************************************************************************************************************
   * Sensors / PIR
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_PIR
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_1_CTR)==0){ return GPIO_PIR_1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_2_CTR)==0){ return GPIO_PIR_2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_3_CTR)==0){ return GPIO_PIR_3; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_4_CTR)==0){ return GPIO_PIR_4; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){ return GPIO_PIR_1_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){ return GPIO_PIR_2_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){ return GPIO_PIR_3_INV; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PIR_4_INV_CTR)==0){ return GPIO_PIR_4_INV; }
  #endif


  /******************************************************************************************************************
   * Sensors / Door
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DOOR
  if(strcmp_P(c, PM_GPIO_FUNCTION_DOOR_LOCK_CTR)==0){ return GPIO_DOOR_LOCK; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_DOOR_OPEN_CTR)==0){ return GPIO_DOOR_OPEN; }
  #endif


  /******************************************************************************************************************
   * Sensors / LDR Analog + Digital Trigger
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  if(strcmp_P(c, PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR)==0){ return GPIO_LDR_BASIC_DIGITAL1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR)==0){ return GPIO_LDR_BASIC_ANALOG1; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR)==0){ return GPIO_LDR_BASIC_DIGITAL2; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR)==0){ return GPIO_LDR_BASIC_ANALOG2; }
  #endif


  /******************************************************************************************************************
   * Sensors / Moisture Analog
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_MOISTURE
  if(strcmp_P(c, PM_GPIO_FUNCTION_MOISTURE_ANALOG_CTR)==0){ return GPIO_MOISTURE_ANALOG; }
  #endif


  /******************************************************************************************************************
   * Sensors / SR04
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_SR04
  if(strcmp_P(c, PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){ return GPIO_SR04_TRIG; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){ return GPIO_SR04_ECHO; }
  #endif


  /******************************************************************************************************************
   * Sensors / Pulse Counter placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_1
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_1; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_2
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_2; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_3
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_3; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_4
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_4; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_1_NP
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_1_NP; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_2_NP
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_2_NP; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_3_NP
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_3_NP; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_4_NP
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PULSE_COUNTER_4_NP; }
  #endif


  /******************************************************************************************************************
   * Sensors / Rotary Encoder placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_A_1
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO__ROTARY_ENCODER_A_1; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_B_1
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO__ROTARY_ENCODER_B_1; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_A_2
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO__ROTARY_ENCODER_A_2; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_B_2
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO__ROTARY_ENCODER_B_2; }
  #endif


  /******************************************************************************************************************
   * Sensors / TOF VL53L0X
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L0X_XSHUTNUM__CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_VL53L0X_XSHUT1 + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Sensors / TOF VL53L1X
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L1X_XSHUTNUM__CTR, i);
    if(strcmp_P(c, buffer)==0){ return GPIO_VL53L1X_XSHUT1 + (i - 1); }
  }
  #endif


  /******************************************************************************************************************
   * Sensors / Radar HLK LD2410
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  if(strcmp_P(c, PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR)==0){ return GPIO_LD2410_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR)==0){ return GPIO_LD2410_TX; }
  #endif


  /******************************************************************************************************************
   * Sensors / GPS Serial
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c, PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)==0){ return GPIO_GPS_SERIAL0_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)==0){ return GPIO_GPS_SERIAL0_RX; }
  #endif


  /******************************************************************************************************************
   * Energy / PZEM
   *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  if(strcmp_P(c, PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){ return GPIO_PZEM0XX_TX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){ return GPIO_PZEM004_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR)==0){ return GPIO_PZEM0XX_RX_MODBUS; }
  #endif


  /******************************************************************************************************************
   * Energy / PZEM017 placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_PZEM017_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_PZEM017_RX; }
  #endif


  /******************************************************************************************************************
   * Energy / SDM serial meters placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SDM120_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SDM120_TX; }
  // TODO: add PROGMEM name macro for GPIO_SDM120_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SDM120_RX; }
  // TODO: add PROGMEM name macro for GPIO_SDM630_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SDM630_TX; }
  // TODO: add PROGMEM name macro for GPIO_SDM630_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_SDM630_RX; }
  #endif


  /******************************************************************************************************************
   * Energy / MCP39F5 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_MCP39F5_TX; }
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_MCP39F5_RX; }
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_RST
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_MCP39F5_RST; }
  #endif


  /******************************************************************************************************************
   * Energy / CSE7766 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_CSE7766_TX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_CSE7766_TX; }
  // TODO: add PROGMEM name macro for GPIO_CSE7766_RX
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_CSE7766_RX; }
  #endif


  /******************************************************************************************************************
   * Energy / ADE7953
   *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  if(strcmp_P(c, PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)==0){ return GPIO_ADE7953_IRQ; }
  #endif


  /******************************************************************************************************************
   * Displays / Backlight placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_BACKLIGHT
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_BACKLIGHT; }
  #endif


  /******************************************************************************************************************
   * Displays / OLED reset
   *******************************************************************************************************************/

  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  if(strcmp_P(c, PM_GPIO_FUNCTION_OLED_RESET_CTR)==0){ return GPIO_OLED_RESET; }
  #endif


  /******************************************************************************************************************
   * Displays / Nextion
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if(strcmp_P(c, PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){ return GPIO_NEXTION_RX; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){ return GPIO_NEXTION_TX; }
  #endif


  /******************************************************************************************************************
   * Displays / Nextion port 2 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_NEXTION_RX2
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_NEXTION_RX2; }
  // TODO: add PROGMEM name macro for GPIO_NEXTION_TX2
  if(strcmp_P(c, PM_GPIO_FUNCTION_TODO_CTR)==0){ return GPIO_NEXTION_TX2; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Fan
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CONTROLLER_FAN
  if(strcmp_P(c, PM_GPIO_FUNCTION_FAN_PWM1_CTR)==0){ return GPIO_FAN_PWM1; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Door / lock / chime
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  if(strcmp_P(c, PM_GPIO_FUNCTION_CHIME_INPUT_CTR)==0){ return GPIO_CHIME_INPUT; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_CHIME_RINGER_CTR)==0){ return GPIO_CHIME_RINGER; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Sampling / sync signals
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c, PM_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR)==0){ return GPIO_FUNCTION_RXON_SAMPLING_ENABLED; }
  if(strcmp_P(c, PM_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR)==0){ return GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL; }
  #endif
  return -1;


}

/**
 * @brief Gets the name of a GPIO function by its ID.
 *
 * Some names are stored directly in PROGMEM and copied into buffer at the end.
 * Numbered families use snprintf_P and return immediately.
 *
 * @param id GPIO function ID
 * @param buffer Output buffer
 * @param buflen Output buffer length
 * @return const char*
 */
const char* mPins::GetGPIOFunctionNamebyID(uint16_t id, char* buffer, uint8_t buflen)
{

  const char *p = nullptr;


  /******************************************************************************************************************
   * Unassigned pin states
   *******************************************************************************************************************/

  if(GPIO_NONE == id){ p = PM_GPIO_FUNCTION_NONE_CTR; }
  if(GPIO_UNUSED_FORCED_HIGH == id){ p = PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR; }
  if(GPIO_UNUSED_FORCED_LOW == id){ p = PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR; }


  /******************************************************************************************************************
   * Core System / Filesystem + SD Card
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_SDCARD
  if(GPIO_FUNCTION_SDCARD_VSPI_CSO == id){ p = PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR; }
  if(GPIO_FUNCTION_SDCARD_VSPI_CLK == id){ p = PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR; }
  if(GPIO_FUNCTION_SDCARD_VSPI_MOSI == id){ p = PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR; }
  if(GPIO_FUNCTION_SDCARD_VSPI_MISO == id){ p = PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR; }
  if(GPIO_FUNCTION_SDCARD_HSPI_CSO == id){ p = PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR; }
  if(GPIO_FUNCTION_SDCARD_HSPI_CLK == id){ p = PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR; }
  if(GPIO_FUNCTION_SDCARD_HSPI_MOSI == id){ p = PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR; }
  if(GPIO_FUNCTION_SDCARD_HSPI_MISO == id){ p = PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_CMD == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_CMD_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_CLK == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_CLK_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_D0 == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_D0_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_D1 == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_D1_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_D2 == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_D2_CTR; }
  if(GPIO_FUNCTION_SDCARD_MMC_D3 == id){ p = PM_GPIO_FUNCTION_SDCARD_MMC_D3_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Hardware Serial0
   *******************************************************************************************************************/

  if(GPIO_HWSERIAL0_TX == id){ p = PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR; }
  if(GPIO_HWSERIAL0_RX == id){ p = PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR; }


  /******************************************************************************************************************
   * Core System / UART: Hardware Serial1/2
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE__SERIAL
  if(GPIO_HWSERIAL1_TX == id){ p = PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR; }
  if(GPIO_HWSERIAL1_RX == id){ p = PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR; }
  if(GPIO_HWSERIAL2_TX == id){ p = PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR; }
  if(GPIO_HWSERIAL2_RX == id){ p = PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Ring-buffer serial modes
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_HWSERIAL0_RING_BUFFER_TX
  if(GPIO_HWSERIAL0_RING_BUFFER_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_HWSERIAL0_RING_BUFFER_RX
  if(GPIO_HWSERIAL0_RING_BUFFER_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif
  #ifdef ESP32
  if(GPIO_HWSERIAL1_RING_BUFFER_TX == id){ p = PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR; }
  if(GPIO_HWSERIAL1_RING_BUFFER_RX == id){ p = PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR; }
  if(GPIO_HWSERIAL2_RING_BUFFER_TX == id){ p = PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR; }
  if(GPIO_HWSERIAL2_RING_BUFFER_RX == id){ p = PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / UART: Debug serial
   *******************************************************************************************************************/

  if(GPIO_SERIAL_DEBUG_TX == id){ p = PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR; }
  if(GPIO_SERIAL_DEBUG_RX == id){ p = PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR; }


  /******************************************************************************************************************
   * Core System / UART: Software serial and serial bridge placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SWSERIAL0_TX
  if(GPIO_SWSERIAL0_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SWSERIAL0_RX
  if(GPIO_SWSERIAL0_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SBR_TX
  if(GPIO_SBR_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SBR_RX
  if(GPIO_SBR_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / I2C
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE_I2C
  if(GPIO_I2C_SCL == id){ p = PM_GPIO_FUNCTION_I2C_SCL_CTR; }
  if(GPIO_I2C_SDA == id){ p = PM_GPIO_FUNCTION_I2C_SDA_CTR; }
  if(GPIO_I2C_SCL2 == id){ p = PM_GPIO_FUNCTION_I2C_SCL2_CTR; }
  if(GPIO_I2C_SDA2 == id){ p = PM_GPIO_FUNCTION_I2C_SDA2_CTR; }
  #endif

  /******************************************************************************************************************
   * Core System / SPI placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SPI_CS
  if(GPIO_SPI_CS == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SPI_DC
  if(GPIO_SPI_DC == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_MISO
  if(GPIO_SSPI_MISO == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_MOSI
  if(GPIO_SSPI_MOSI == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_SCLK
  if(GPIO_SSPI_SCLK == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_CS
  if(GPIO_SSPI_CS == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SSPI_DC
  if(GPIO_SSPI_DC == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / ADC
   *******************************************************************************************************************/

  if(GPIO_ADC1_CH4 == id){ p = PM_GPIO_FUNCTION_ADC1_CH4_CTR; }
  if(GPIO_ADC1_CH6 == id){ p = PM_GPIO_FUNCTION_ADC1_CH6_CTR; }
  if(GPIO_ADC1_CH7 == id){ p = PM_GPIO_FUNCTION_ADC1_CH7_CTR; }
  if(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER == id){ p = PM_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR; }


  /******************************************************************************************************************
   * Core System / ADC placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_ADC0_TEMPERATURE
  if(GPIO_ADC0_TEMPERATURE == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Core System / PWM
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE_PWM
  if(GPIO_PWM1 == id){ p = PM_GPIO_FUNCTION_PWM1_CTR; }
  if(GPIO_PWM2 == id){ p = PM_GPIO_FUNCTION_PWM2_CTR; }
  if(GPIO_PWM3 == id){ p = PM_GPIO_FUNCTION_PWM3_CTR; }
  if(GPIO_PWM4 == id){ p = PM_GPIO_FUNCTION_PWM4_CTR; }
  if(GPIO_PWM5 == id){ p = PM_GPIO_FUNCTION_PWM5_CTR; }
  if(GPIO_PWM1_INV == id){ p = PM_GPIO_FUNCTION_PWM1_INV_CTR; }
  if(GPIO_PWM2_INV == id){ p = PM_GPIO_FUNCTION_PWM2_INV_CTR; }
  if(GPIO_PWM3_INV == id){ p = PM_GPIO_FUNCTION_PWM3_INV_CTR; }
  if(GPIO_PWM4_INV == id){ p = PM_GPIO_FUNCTION_PWM4_INV_CTR; }
  if(GPIO_PWM5_INV == id){ p = PM_GPIO_FUNCTION_PWM5_INV_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / LED
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_LEDS
  if(id >= GPIO_LED1 && id <= GPIO_LED8){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_CTR, id - GPIO_LED1 + 1); return buffer; }
  if(id >= GPIO_LED1_INV && id <= GPIO_LED8_INV){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_INV_CTR, id - GPIO_LED1_INV + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Drivers / Relay
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_RELAY
  if(id >= GPIO_REL1 && id <= GPIO_REL8){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_REL_NUM_CTR, id - GPIO_REL1 + 1); return buffer; }
  if(id >= GPIO_REL1_INV && id <= GPIO_REL8_INV){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_REL_NUM_INV_CTR, id - GPIO_REL1_INV + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Drivers / IR placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_IRSEND
  if(GPIO_IRSEND == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_IRRECV
  if(GPIO_IRRECV == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / SAW RF placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_RFSEND
  if(GPIO_RFSEND == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_RFRECV
  if(GPIO_RFRECV == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / 433 MHz RF
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_RF433_CODES
  if(GPIO_RF_433MHZ_RX == id){ p = PM_GPIO_FUNCTION__RF_433MHZ_RX__CTR; }
  if(GPIO_RF_433MHZ_TX == id){ p = PM_GPIO_FUNCTION__RF_433MHZ_TX__CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / HBridge/Motor L9110
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  if(GPIO_HBRIDGE_L9110_IA == id){ p = PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR; }
  if(GPIO_HBRIDGE_L9110_IB == id){ p = PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR; }
  if(GPIO_HBRIDGE_L9110_OA == id){ p = PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR; }
  if(GPIO_HBRIDGE_L9110_OB == id){ p = PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Shelly Dimmer
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  if(GPIO_SHELLY2_SHD_BOOT0 == id){ p = PM_GPIO_FUNCTION_SHELLY2_SHD_BOOT0_CTR; }
  if(GPIO_SHELLY2_SHD_RESET_INV == id){ p = PM_GPIO_FUNCTION_SHELLY2_SHD_RESET_INV_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Buzzer
   *******************************************************************************************************************/

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  if(GPIO_BUZZER == id){ p = PM_GPIO_FUNCTION_BUZZER_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Buzzer inverted placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_BUZZER_INV
  if(GPIO_BUZZER_INV == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Cellular Modem
   *******************************************************************************************************************/

   
  if(GPIO_FUNCTION__MODEM_RX0 == id){ p = PM_GPIO_FUNCTION__MODEM_RX0__CTR; }
  if(GPIO_FUNCTION__MODEM_TX0 == id){ p = PM_GPIO_FUNCTION__MODEM_TX0__CTR; }
  if(GPIO_FUNCTION__MODEM_RX1 == id){ p = PM_GPIO_FUNCTION__MODEM_RX1__CTR; }
  if(GPIO_FUNCTION__MODEM_TX1 == id){ p = PM_GPIO_FUNCTION__MODEM_TX1__CTR; }
  if(GPIO_FUNCTION__MODEM_RX2 == id){ p = PM_GPIO_FUNCTION__MODEM_RX2__CTR; }
  if(GPIO_FUNCTION__MODEM_TX2 == id){ p = PM_GPIO_FUNCTION__MODEM_TX2__CTR; }
  if(GPIO_FUNCTION__MODEM_POWER_KEY == id){ p = PM_GPIO_FUNCTION__MODEM_POWER_KEY__CTR; }
  
  /******************************************************************************************************************
   * Drivers / Cellular Modem placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_RING_INDICATOR
  if(GPIO_FUNCTION__MODEM_RING_INDICATOR == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_POWER_STATUS
  if(GPIO_FUNCTION__MODEM_POWER_STATUS == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_FUNCTION__MODEM_NETWORK_STATUS
  if(GPIO_FUNCTION__MODEM_NETWORK_STATUS == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Cameras
   *******************************************************************************************************************/

  #ifdef ESP32
  if(GPIO_WEBCAM_XCLK == id){ p = PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR; }
  if(id >= GPIO_WEBCAM_DATA1 && id <= GPIO_WEBCAM_DATA8){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_WEBCAM_DATA_NUM_CTR, id - GPIO_WEBCAM_DATA1 + 1); return buffer; }
  if(GPIO_WEBCAM_PCLK == id){ p = PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR; }
  if(GPIO_WEBCAM_HREF == id){ p = PM_GPIO_FUNCTION_WEBCAM_HREF_CTR; }
  if(GPIO_WEBCAM_VSYNC == id){ p = PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR; }
  if(GPIO_WEBCAM_SIOD == id){ p = PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR; }
  if(GPIO_WEBCAM_SIOC == id){ p = PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR; }
  if(GPIO_WEBCAM_PWDN == id){ p = PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR; }
  #endif


  /******************************************************************************************************************
   * Drivers / Camera reset placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_WEBCAM_RESET
  if(GPIO_WEBCAM_RESET == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / Switches
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_SWITCHES
  if(id >= GPIO_SWT1 && id <= GPIO_SWT32){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_CTR, id - GPIO_SWT1 + 1); return buffer; }
  if(id >= GPIO_SWT1_INV && id <= GPIO_SWT32_INV){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, id - GPIO_SWT1_INV + 1); return buffer; }
  if(id >= GPIO_SWT1_NP && id <= GPIO_SWT32_NP){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, id - GPIO_SWT1_NP + 1); return buffer; }
  if(id >= GPIO_SWT1_INV_NP && id <= GPIO_SWT32_INV_NP){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, id - GPIO_SWT1_INV_NP + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Sensors / Buttons
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_BUTTONS
  if(id >= GPIO_KEY1 && id <= GPIO_KEY32){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_CTR, id - GPIO_KEY1 + 1); return buffer; }
  if(id >= GPIO_KEY1_INV && id <= GPIO_KEY32_INV){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, id - GPIO_KEY1_INV + 1); return buffer; }
  if(id >= GPIO_KEY1_NP && id <= GPIO_KEY32_NP){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, id - GPIO_KEY1_NP + 1); return buffer; }
  if(id >= GPIO_KEY1_INV_NP && id <= GPIO_KEY32_INV_NP){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, id - GPIO_KEY1_INV_NP + 1); return buffer; }
  if(id >= GPIO_KEY1_TOUCH && id <= GPIO_KEY32_TOUCH){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, id - GPIO_KEY1_TOUCH + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Sensors / DS18X20
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DS18X20
  if(GPIO_DSB_1 == id){ p = PM_GPIO_FUNCTION_DS18X20_1_CTR; }
  if(GPIO_DSB_2 == id){ p = PM_GPIO_FUNCTION_DS18X20_2_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / DHT11/DHT22
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DHT
  if(GPIO_DHT11_1 == id){ p = PM_GPIO_FUNCTION_DHT11_1_CTR; }
  if(GPIO_DHT11_2 == id){ p = PM_GPIO_FUNCTION_DHT11_2_CTR; }
  if(GPIO_DHT22_1 == id){ p = PM_GPIO_FUNCTION_DHT22_1_CTR; }
  if(GPIO_DHT22_2 == id){ p = PM_GPIO_FUNCTION_DHT22_2_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / PIR
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_PIR
  if(GPIO_PIR_1 == id){ p = PM_GPIO_FUNCTION_PIR_1_CTR; }
  if(GPIO_PIR_2 == id){ p = PM_GPIO_FUNCTION_PIR_2_CTR; }
  if(GPIO_PIR_3 == id){ p = PM_GPIO_FUNCTION_PIR_3_CTR; }
  if(GPIO_PIR_4 == id){ p = PM_GPIO_FUNCTION_PIR_4_CTR; }
  if(GPIO_PIR_1_INV == id){ p = PM_GPIO_FUNCTION_PIR_1_INV_CTR; }
  if(GPIO_PIR_2_INV == id){ p = PM_GPIO_FUNCTION_PIR_2_INV_CTR; }
  if(GPIO_PIR_3_INV == id){ p = PM_GPIO_FUNCTION_PIR_3_INV_CTR; }
  if(GPIO_PIR_4_INV == id){ p = PM_GPIO_FUNCTION_PIR_4_INV_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / Door
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_DOOR
  if(GPIO_DOOR_LOCK == id){ p = PM_GPIO_FUNCTION_DOOR_LOCK_CTR; }
  if(GPIO_DOOR_OPEN == id){ p = PM_GPIO_FUNCTION_DOOR_OPEN_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / LDR Analog + Digital Trigger
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  if(GPIO_LDR_BASIC_DIGITAL1 == id){ p = PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR; }
  if(GPIO_LDR_BASIC_ANALOG1 == id){ p = PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR; }
  if(GPIO_LDR_BASIC_DIGITAL2 == id){ p = PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR; }
  if(GPIO_LDR_BASIC_ANALOG2 == id){ p = PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / Moisture Analog
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_MOISTURE
  if(GPIO_MOISTURE_ANALOG == id){ p = PM_GPIO_FUNCTION_MOISTURE_ANALOG_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / SR04
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_SR04
  if(GPIO_SR04_TRIG == id){ p = PM_GPIO_FUNCTION_SR04_TRIG_CTR; }
  if(GPIO_SR04_ECHO == id){ p = PM_GPIO_FUNCTION_SR04_ECHO_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / Pulse Counter placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_1
  if(GPIO_PULSE_COUNTER_1 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_2
  if(GPIO_PULSE_COUNTER_2 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_3
  if(GPIO_PULSE_COUNTER_3 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_4
  if(GPIO_PULSE_COUNTER_4 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_1_NP
  if(GPIO_PULSE_COUNTER_1_NP == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_2_NP
  if(GPIO_PULSE_COUNTER_2_NP == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_3_NP
  if(GPIO_PULSE_COUNTER_3_NP == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_PULSE_COUNTER_4_NP
  if(GPIO_PULSE_COUNTER_4_NP == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / Rotary Encoder placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_A_1
  if(GPIO__ROTARY_ENCODER_A_1 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_B_1
  if(GPIO__ROTARY_ENCODER_B_1 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_A_2
  if(GPIO__ROTARY_ENCODER_A_2 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO__ROTARY_ENCODER_B_2
  if(GPIO__ROTARY_ENCODER_B_2 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / TOF VL53L0X
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  if(id >= GPIO_VL53L0X_XSHUT1 && id <= GPIO_VL53L0X_XSHUT8){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION__TOF_VL53L0X_XSHUTNUM__CTR, id - GPIO_VL53L0X_XSHUT1 + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Sensors / TOF VL53L1X
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  if(id >= GPIO_VL53L1X_XSHUT1 && id <= GPIO_VL53L1X_XSHUT8){ snprintf_P(buffer, buflen, PM_GPIO_FUNCTION__TOF_VL53L1X_XSHUTNUM__CTR, id - GPIO_VL53L1X_XSHUT1 + 1); return buffer; }
  #endif


  /******************************************************************************************************************
   * Sensors / Radar HLK LD2410
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  if(GPIO_LD2410_RX == id){ p = PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR; }
  if(GPIO_LD2410_TX == id){ p = PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR; }
  #endif


  /******************************************************************************************************************
   * Sensors / GPS Serial
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(GPIO_GPS_SERIAL0_TX == id){ p = PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR; }
  if(GPIO_GPS_SERIAL0_RX == id){ p = PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / PZEM
   *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  if(GPIO_PZEM0XX_TX == id){ p = PM_GPIO_FUNCTION_PZEM0XX_TX_CTR; }
  if(GPIO_PZEM004_RX == id){ p = PM_GPIO_FUNCTION_PZEM004_RX_CTR; }
  if(GPIO_PZEM0XX_RX_MODBUS == id){ p = PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / PZEM017 placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_PZEM017_RX
  if(GPIO_PZEM017_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / SDM serial meters placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_SDM120_TX
  if(GPIO_SDM120_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SDM120_RX
  if(GPIO_SDM120_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SDM630_TX
  if(GPIO_SDM630_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_SDM630_RX
  if(GPIO_SDM630_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / MCP39F5 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_TX
  if(GPIO_MCP39F5_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_RX
  if(GPIO_MCP39F5_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_MCP39F5_RST
  if(GPIO_MCP39F5_RST == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / CSE7766 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_CSE7766_TX
  if(GPIO_CSE7766_TX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_CSE7766_RX
  if(GPIO_CSE7766_RX == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Energy / ADE7953
   *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  if(GPIO_ADE7953_IRQ == id){ p = PM_GPIO_FUNCTION_ADE7953_IRQ_CTR; }
  #endif


  /******************************************************************************************************************
   * Displays / Backlight placeholder
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_BACKLIGHT
  if(GPIO_BACKLIGHT == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Displays / OLED reset
   *******************************************************************************************************************/

  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  if(GPIO_OLED_RESET == id){ p = PM_GPIO_FUNCTION_OLED_RESET_CTR; }
  #endif


  /******************************************************************************************************************
   * Displays / Nextion
   *******************************************************************************************************************/

  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if(GPIO_NEXTION_RX == id){ p = PM_GPIO_FUNCTION_NEXTION_RX_CTR; }
  if(GPIO_NEXTION_TX == id){ p = PM_GPIO_FUNCTION_NEXTION_TX_CTR; }
  #endif


  /******************************************************************************************************************
   * Displays / Nextion port 2 placeholders
   *******************************************************************************************************************/

  #if 0
  // TODO: add PROGMEM name macro for GPIO_NEXTION_RX2
  if(GPIO_NEXTION_RX2 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  // TODO: add PROGMEM name macro for GPIO_NEXTION_TX2
  if(GPIO_NEXTION_TX2 == id){ p = PM_GPIO_FUNCTION_TODO_CTR; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Fan
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CONTROLLER_FAN
  if(GPIO_FAN_PWM1 == id){ p = PM_GPIO_FUNCTION_FAN_PWM1_CTR; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Door / lock / chime
   *******************************************************************************************************************/

  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  if(GPIO_CHIME_INPUT == id){ p = PM_GPIO_FUNCTION_CHIME_INPUT_CTR; }
  if(GPIO_CHIME_RINGER == id){ p = PM_GPIO_FUNCTION_CHIME_RINGER_CTR; }
  #endif


  /******************************************************************************************************************
   * Controllers and Usermods / Sampling / sync signals
   *******************************************************************************************************************/

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(GPIO_FUNCTION_RXON_SAMPLING_ENABLED == id){ p = PM_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR; }
  if(GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL == id){ p = PM_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR; }
  #endif
  if(p != nullptr){
    strncpy_P(buffer, p, buflen - 1);
    buffer[buflen - 1] = '\0';
    return buffer;
  }

  snprintf_P(buffer, buflen, PSTR("GPIO%dNameMissing"), id);
  ALOG_ERR(PSTR(D_LOG_PINS "%s"), buffer);

  return buffer;

}
