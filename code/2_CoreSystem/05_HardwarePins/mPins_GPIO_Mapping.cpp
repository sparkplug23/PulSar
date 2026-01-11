#include "mPins.h"

/****
 * Since this is a long list, the GPIO from ID to name and back is defined in a separate file.
 */

 
// Function: Checks for gpio function as stored in progmem, returning its identifier if true, else -1
// Returns : #>=0:id found, -1: not found
// Notes: "else" not included in if structure as return is used 
//
int16_t mPins::GetGPIOFunctionIDbyName(const char* c)
{

  if(*c=='\0'){ return -1; }

  char buffer[50];  // Buffer to hold the formatted string (e.g., "SWT32" or "SWT1 Inv")
  
  if(strcmp_P(c,PM_GPIO_FUNCTION_NONE_CTR)==0){       return GPIO_NONE; }
  
  /******************************************************************************************************************
   * Core Peripherals
  *******************************************************************************************************************/

  #ifdef USE_MODULE_CORE_I2C
  if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SCL_CTR)==0){  return GPIO_I2C_SCL; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SDA_CTR)==0){  return GPIO_I2C_SDA; }
  #endif

  if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR)==0){   return GPIO_UNUSED_FORCED_HIGH; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR)==0){  return GPIO_UNUSED_FORCED_LOW; }
  
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)==0){  return GPIO_HWSERIAL0_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)==0){  return GPIO_HWSERIAL0_RX; }
  #ifdef USE_MODULE_CORE__SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR)==0){  return GPIO_HWSERIAL1_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR)==0){  return GPIO_HWSERIAL1_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR)==0){  return GPIO_HWSERIAL2_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR)==0){  return GPIO_HWSERIAL2_RX; }
  #endif

  /******************************************************************************************************************
   * Network
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Drivers
  *******************************************************************************************************************/
  
  #ifdef USE_MODULE_DRIVERS_RELAY  
  for (int i = 1; i <= 8; i++) {
    // Check normal RELs "REL1" to "REL8"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_REL_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_REL1 + (i - 1);  // Return the corresponding REL ID
    }
    // Check inverted RELs "REL1 Inv" to "REL8 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_REL_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_REL1_INV + (i - 1);  // Return the corresponding inverted REL ID
    }
  }
  #endif
  
  
  /******************************************************************************************************************
   * Sensors
  *******************************************************************************************************************/
  
  #ifdef USE_MODULE_SENSORS_DHT
  if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_1_CTR)==0){  return GPIO_DHT22_1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_2_CTR)==0){  return GPIO_DHT22_2; }
  #endif

  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_1_CTR)==0){  return GPIO_DSB_1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_2_CTR)==0){  return GPIO_DSB_2; }
  #endif
  
  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L0X_XSHUTNUM__CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_VL53L0X_XSHUT1 + (i - 1);
    }
  }
  #endif // USE_MODULE_SENSORS__TOF_VL53L1X
  #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L1X_XSHUTNUM__CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_VL53L1X_XSHUT1 + (i - 1);
    }
  }
  #endif // USE_MODULE_SENSORS__TOF_VL53L1X

  
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR)==0){   return GPIO_LDR_BASIC_ANALOG1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL2; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR)==0){   return GPIO_LDR_BASIC_ANALOG2; }
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  #ifdef USE_MODULE_SENSORS_SWITCHES

  // Check for "SWT1" to "SWT32"
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      Serial.printf("SWT1_INV_NP_CTR %s\n\r", buffer);
      return GPIO_SWT1_INV_NP + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      Serial.printf("GPIO_SWT1_NP %s\n\r", buffer);
      return GPIO_SWT1_NP + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_INV + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1 + (i - 1);
    }
  }
  #endif

  #ifdef USE_MODULE_CONTROLLER_FAN
  if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_PWM1_CTR)==0){  return GPIO_FAN_PWM1; }
  #endif

  
  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  if(strcmp_P(c,PM_GPIO_FUNCTION_OLED_RESET_CTR)==0){  return GPIO_OLED_RESET; }
  #endif

  #ifdef USE_MODULE_SENSORS_PIR
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_CTR)==0){  return GPIO_PIR_1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_CTR)==0){  return GPIO_PIR_2; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_CTR)==0){  return GPIO_PIR_3; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_4_CTR)==0){  return GPIO_PIR_4; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){  return GPIO_PIR_1_INV; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){  return GPIO_PIR_2_INV; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){  return GPIO_PIR_3_INV; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_4_INV_CTR)==0){  return GPIO_PIR_4_INV; }
  #endif
  

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  if(strcmp_P(c,PM_GPIO_FUNCTION_BUZZER_CTR)==0){  return GPIO_BUZZER; }
  #endif



  #ifdef USE_MODULE_DRIVERS_PWM
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM1_CTR)==0){  return GPIO_PWM1; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM2_CTR)==0){  return GPIO_PWM2; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM3_CTR)==0){  return GPIO_PWM3; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM4_CTR)==0){  return GPIO_PWM4; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM5_CTR)==0){  return GPIO_PWM5; }
  #endif

  #ifdef ESP32
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_TX; }
  #endif
  // if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)==0){  return GPIO_GPS_SERIAL0_TX; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)==0){  return GPIO_GPS_SERIAL0_RX; }
  #ifdef ESP32
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR)==0){  return GPIO_GPS_SERIAL1_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR)==0){  return GPIO_GPS_SERIAL1_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR)==0){  return GPIO_GPS_SERIAL2_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR)==0){  return GPIO_GPS_SERIAL2_RX; }
  #endif // 
  #endif

  
  //if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_CTR)==0){  return GPIO_LEDLNK; }
  //if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_INV_CTR)==0){  return GPIO_LEDLNK_INV; }


  #ifdef USE_MODULE_SENSORS_DOOR
  if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_OPEN_CTR)==0){  return GPIO_DOOR_OPEN; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_LOCK_CTR)==0){  return GPIO_DOOR_LOCK; }
  #endif

  // if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)==0){  return GPIO_SERIAL_DEBUG_TX; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)==0){  return GPIO_SERIAL_DEBUG_RX; }


  


  #ifdef USE_MODULE_DRIVERS_LEDS
  for (int i = 1; i <= 8; i++) {
    // Check normal LEDs "LED1" to "LED8"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1 + (i - 1);  // Return the corresponding LED ID
    }

    // Check inverted LEDs "LED1 Inv" to "LED8 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1_INV + (i - 1);  // Return the corresponding inverted LED ID
    }
  }
  #endif
  

  #ifdef USE_MODULE_SENSORS_SR04
  if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){  return GPIO_SR04_ECHO; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){  return GPIO_SR04_TRIG; }
  #endif
  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  if(strcmp_P(c,PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR)==0){  return GPIO_LD2410_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR)==0){  return GPIO_LD2410_TX; }
  #endif
  // #ifdef USE_MODULE_SENSORS_ULTRASONICS
  // if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR)==0){  return GPIO__ROTARY_ENCODER_A_; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR)==0){  return GPIO__ROTARY_ENCODER_B_; }
  // #endif

  #ifdef USE_MODULE_SENSORS_BUTTONS
  for (int i = 1; i <= 32; i++) {
    ALOG_INF(PSTR("Checking key %d"), i);
    // Check normal keys "KEY1" to "KEY32"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1 + (i - 1);  // Return the corresponding key ID
    }

    // Check inverted keys "KEY1 Inv" to "KEY32 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV + (i - 1);  // Return the corresponding inverted key ID
    }

    // Check non-pull keys "KEY1_NP" to "KEY32_NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_NP + (i - 1);  // Return the corresponding non-pull key ID
    }

    // Check inverted non-pull keys "KEY1 Inv NP" to "KEY32 Inv NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV_NP + (i - 1);  // Return the corresponding inverted non-pull key ID
    }

    // Check touch keys "KEY1_TOUCH" to "KEY32_TOUCH"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, i);
    ALOG_INF(PSTR("c=%s to %s"),c,buffer);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_TOUCH + (i - 1);  // Return the corresponding touch key ID
    }
  }
  #endif

  // if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_PULLDOWN_CTR)==0){  return GPIO_KEY1_PULLDOWN; }


  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_RX__CTR)==0){  return GPIO_RF_433MHZ_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_TX__CTR)==0){  return GPIO_RF_433MHZ_TX; }
  #endif

  #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_KEY__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_KEY_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_STATUS_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_NETWORK_STATUS_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RESET__CTR)==0){  return GPIO_FUNCTION__FONA_RESET_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_TX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_TX_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_RX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_RX_; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR)==0){  return GPIO_FUNCTION__FONA_RING_INDICATOR_; }
  #endif // 

  
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR)==0){  return GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR_; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_TX__CTR)==0){  return GPIO_FUNCTION__MODEM_TX_; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_RX__CTR)==0){  return GPIO_FUNCTION__MODEM_RX_; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_POWER__CTR)==0){  return GPIO_FUNCTION__MODEM_POWER_; }



  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){  return GPIO_HBRIDGE_L9110_IA; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){  return GPIO_HBRIDGE_L9110_IB; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){  return GPIO_HBRIDGE_L9110_OA; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){  return GPIO_HBRIDGE_L9110_OB; }
  #endif

  // Webcam
  #ifdef ESP32
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_WEBCAM_DATA_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_WEBCAM_DATA1 + (i - 1);  // Return the corresponding key ID
    }
  }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)==0){   return GPIO_WEBCAM_XCLK; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)==0){   return GPIO_WEBCAM_PCLK; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)==0){   return GPIO_WEBCAM_HREF; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)==0){  return GPIO_WEBCAM_VSYNC; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)==0){   return GPIO_WEBCAM_SIOD; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)==0){   return GPIO_WEBCAM_SIOC; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)==0){   return GPIO_WEBCAM_PWDN; }
  #endif // ESP32

  #ifdef USE_MODULE_DRIVERS_SDCARD
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CSO; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CLK; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MOSI; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MISO; }

  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CSO; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CLK; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MOSI; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MISO; }
  #endif // USE_MODULE_DRIVERS_SDCARD

  // if(strcmp_P(c,PM_GPIO_FUNCTION_ANALOG_POSITION_CTR)==0){  return GPIO_ANALOG_POSITION; }
  
  /******************************************************************************************************************
   * Lights
  *******************************************************************************************************************/


  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)==0){  return GPIO_ADE7953_IRQ; }
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){  return GPIO_PZEM0XX_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){  return GPIO_PZEM004_RX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR)==0){  return GPIO_PZEM0XX_RX_MODBUS; }
  #endif

  
  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){  return GPIO_NEXTION_TX; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){  return GPIO_NEXTION_RX; }
  #endif

  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH4_CTR)==0){  return GPIO_ADC1_CH4; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH6_CTR)==0){  return GPIO_ADC1_CH6; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH7_CTR)==0){  return GPIO_ADC1_CH7; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR)==0){  return GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER; }

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR)==0){  return GPIO_FUNCTION_RXON_SAMPLING_ENABLED; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR)==0){  return GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL; }
  #endif //
  


  
  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  
  /******************************************************************************************************************
   * Controller
  *******************************************************************************************************************/
  
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_INPUT_CTR)==0){  return GPIO_CHIME_INPUT; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_RINGER_CTR)==0){  return GPIO_CHIME_RINGER; }
  #endif

  return -1;

}

/**
 * @brief Gets the name of a GPIO function by its ID.
 * 
 * Some are stored in progmem, these use a pointer to write into the buffer at the end of the function.
 * Others are done effeciently by using snprintf_P to write into the buffer and return immediately.
 * 
 * @param id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
const char* mPins::GetGPIOFunctionNamebyID(uint16_t id, char* buffer, uint8_t buflen) 
{

  const char *p = nullptr;

  if(GPIO_NONE == id)            p = PM_GPIO_FUNCTION_NONE_CTR; 
  
  /******************************************************************************************************************
   * Core Peripherals
  *******************************************************************************************************************/

  if(GPIO_I2C_SCL == id)                        p = PM_GPIO_FUNCTION_I2C_SCL_CTR;
  if(GPIO_I2C_SDA == id)                        p = PM_GPIO_FUNCTION_I2C_SDA_CTR;

  if(GPIO_UNUSED_FORCED_HIGH == id)             p = PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR;
  if(GPIO_UNUSED_FORCED_LOW == id)              p = PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR;


  /******************************************************************************************************************
   * Network
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Drivers
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DRIVERS_PWM
  if(GPIO_PWM1 == id)                        p = PM_GPIO_FUNCTION_PWM1_CTR;
  if(GPIO_PWM1_INV == id)                        p = PM_GPIO_FUNCTION_PWM1_INV_CTR;
  if(GPIO_PWM2 == id)                        p = PM_GPIO_FUNCTION_PWM2_CTR;
  if(GPIO_PWM2_INV == id)                        p = PM_GPIO_FUNCTION_PWM2_INV_CTR;
  if(GPIO_PWM3 == id)                        p = PM_GPIO_FUNCTION_PWM3_CTR;
  if(GPIO_PWM3_INV == id)                        p = PM_GPIO_FUNCTION_PWM3_INV_CTR;
  if(GPIO_PWM4 == id)                        p = PM_GPIO_FUNCTION_PWM4_CTR;
  if(GPIO_PWM4_INV == id)                        p = PM_GPIO_FUNCTION_PWM4_INV_CTR;
  if(GPIO_PWM5 == id)                        p = PM_GPIO_FUNCTION_PWM5_CTR;
  if(GPIO_PWM5_INV == id)                        p = PM_GPIO_FUNCTION_PWM5_INV_CTR;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
  if (id >= GPIO_REL1 && id <= GPIO_REL8){                 snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_REL_NUM_CTR,        id - GPIO_REL1 + 1);         return buffer; }
  if (id >= GPIO_REL1_INV && id <= GPIO_REL8_INV){         snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_REL_NUM_INV_CTR,    id - GPIO_REL1_INV + 1);     return buffer; }
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
  if (id >= GPIO_LED1 && id <= GPIO_LED8){                 snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_CTR,        id - GPIO_LED1 + 1);         return buffer; }
  if (id >= GPIO_LED1_INV && id <= GPIO_LED8_INV){         snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_INV_CTR,    id - GPIO_LED1_INV + 1);     return buffer; }
  #endif

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  if(GPIO_BUZZER == id)                        p = PM_GPIO_FUNCTION_BUZZER_CTR;
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  if(GPIO_SHELLY2_SHD_BOOT0 == id)                        p = PM_GPIO_FUNCTION_SHELLY2_SHD_BOOT0_CTR;
  if(GPIO_SHELLY2_SHD_RESET_INV == id)                    p = PM_GPIO_FUNCTION_SHELLY2_SHD_RESET_INV_CTR;
  #endif


  /******************************************************************************************************************
   * Sensors
  *******************************************************************************************************************/
  #ifdef USE_MODULE_SENSORS_BUTTONS
  if (id >= GPIO_KEY1 && id <= GPIO_KEY32){                 snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_CTR,            id - GPIO_KEY1 + 1); return buffer; }
  if (id >= GPIO_KEY1_INV && id <= GPIO_KEY32_INV){         snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_CTR,        id - GPIO_KEY1_INV + 1); return buffer; }
  if (id >= GPIO_KEY1_NP && id <= GPIO_KEY32_NP){           snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_NP_CTR,         id - GPIO_KEY1_NP + 1); return buffer; }
  if (id >= GPIO_KEY1_INV_NP && id <= GPIO_KEY32_INV_NP){   snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR,     id - GPIO_KEY1_INV_NP + 1); return buffer; }
  if (id >= GPIO_KEY1_TOUCH && id <= GPIO_KEY32_TOUCH){     snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR,      id - GPIO_KEY1_TOUCH + 1); return buffer; }
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
  if (id >= GPIO_SWT1 && id <= GPIO_SWT32){                 snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_CTR,         id - GPIO_SWT1 + 1);         return buffer; }
  if (id >= GPIO_SWT1_INV && id <= GPIO_SWT32_INV){         snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_CTR,     id - GPIO_SWT1_INV + 1);     return buffer; }
  if (id >= GPIO_SWT1_NP && id <= GPIO_SWT32_NP){           snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_NP_CTR,      id - GPIO_SWT1_NP + 1);       return buffer; }
  if (id >= GPIO_SWT1_INV_NP && id <= GPIO_SWT32_INV_NP){   snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR,  id - GPIO_SWT1_INV_NP + 1);  return buffer; }
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
  if(GPIO_DHT11_1OF2 == id)                        p = PM_GPIO_FUNCTION_DHT11_1_CTR;
  if(GPIO_DHT11_2OF2 == id)                        p = PM_GPIO_FUNCTION_DHT11_2_CTR;
  if(GPIO_DHT22_1OF2 == id)                        p = PM_GPIO_FUNCTION_DHT22_1_CTR;
  if(GPIO_DHT22_2OF2 == id)                        p = PM_GPIO_FUNCTION_DHT22_2_CTR;
  #endif
  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  if(GPIO_LD2410_RX == id)                        p = PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR;
  if(GPIO_LD2410_TX == id)                        p = PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR;
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
  if(GPIO_SR04_TRIG == id)                        p = PM_GPIO_FUNCTION_SR04_TRIG_CTR;
  if(GPIO_SR04_ECHO == id)                        p = PM_GPIO_FUNCTION_SR04_ECHO_CTR;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
  if(GPIO_DOOR_OPEN == id)                        p = PM_GPIO_FUNCTION_DOOR_OPEN_CTR;
  #endif
  #ifdef USE_MODULE_SENSORS_PIR
  if(GPIO_PIR_1 == id)                        p = PM_GPIO_FUNCTION_PIR_1_CTR;
  if(GPIO_PIR_2 == id)                        p = PM_GPIO_FUNCTION_PIR_2_CTR;
  if(GPIO_PIR_3 == id)                        p = PM_GPIO_FUNCTION_PIR_3_CTR;
  if(GPIO_PIR_4 == id)                        p = PM_GPIO_FUNCTION_PIR_4_CTR;
  if(GPIO_PIR_1_INV == id)                        p = PM_GPIO_FUNCTION_PIR_1_INV_CTR;
  if(GPIO_PIR_2_INV == id)                        p = PM_GPIO_FUNCTION_PIR_2_INV_CTR;
  if(GPIO_PIR_3_INV == id)                        p = PM_GPIO_FUNCTION_PIR_3_INV_CTR;
  if(GPIO_PIR_4_INV == id)                        p = PM_GPIO_FUNCTION_PIR_4_INV_CTR;
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  if(GPIO_DSB_1 == id)                        p = PM_GPIO_FUNCTION_DS18X20_1_CTR;
  if(GPIO_DSB_2 == id)                        p = PM_GPIO_FUNCTION_DS18X20_2_CTR;
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  if(GPIO_LDR_BASIC_DIGITAL1 == id)                        p = PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR;
  if(GPIO_LDR_BASIC_ANALOG1 == id)                        p = PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR;
  if(GPIO_LDR_BASIC_DIGITAL2 == id)                        p = PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR;
  if(GPIO_LDR_BASIC_ANALOG2 == id)                        p = PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR;
  #endif // USE_MODULE_SENSORS_LDR_BASIC

  /******************************************************************************************************************
   * Lights
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/
  
  #ifdef USE_MODULE_ENERGY_ADE7953
  if(GPIO_ADE7953_IRQ == id)                        p = PM_GPIO_FUNCTION_ADE7953_IRQ_CTR;
  #endif

  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if(GPIO_NEXTION_RX == id)                        p = PM_GPIO_FUNCTION_NEXTION_RX_CTR;
  if(GPIO_NEXTION_TX == id)                        p = PM_GPIO_FUNCTION_NEXTION_TX_CTR;
  #endif
  if(GPIO_HWSERIAL0_RX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR;
  if(GPIO_HWSERIAL0_TX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR;
  #ifdef ESP32
  if(GPIO_HWSERIAL1_RX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR;
  if(GPIO_HWSERIAL1_TX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR;
  if(GPIO_HWSERIAL2_RX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR;
  if(GPIO_HWSERIAL2_TX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR;
  if(GPIO_HWSERIAL1_RING_BUFFER_RX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR;
  if(GPIO_HWSERIAL1_RING_BUFFER_TX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR;
  if(GPIO_HWSERIAL2_RING_BUFFER_RX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR;
  if(GPIO_HWSERIAL2_RING_BUFFER_TX == id)                        p = PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR;
  if (id >= GPIO_WEBCAM_DATA1 && id <= GPIO_WEBCAM_DATA8){                 snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_WEBCAM_DATA_NUM_CTR,            id - GPIO_WEBCAM_DATA1 + 1); return buffer; }
  if(GPIO_WEBCAM_XCLK == id)                        p = PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR;
  if(GPIO_WEBCAM_PCLK == id)                        p = PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR;
  if(GPIO_WEBCAM_HREF == id)                        p = PM_GPIO_FUNCTION_WEBCAM_HREF_CTR;
  if(GPIO_WEBCAM_VSYNC == id)                        p = PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR;
  if(GPIO_WEBCAM_SIOD == id)                        p = PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR;
  if(GPIO_WEBCAM_SIOC == id)                        p = PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR;
  if(GPIO_WEBCAM_PWDN == id)                        p = PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR;
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
  if(GPIO_FUNCTION_SDCARD_VSPI_CSO == id)                        p = PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR;
  if(GPIO_FUNCTION_SDCARD_VSPI_CLK == id)                        p = PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR;
  if(GPIO_FUNCTION_SDCARD_VSPI_MOSI == id)                        p = PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR;
  if(GPIO_FUNCTION_SDCARD_VSPI_MISO == id)                        p = PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR;
  if(GPIO_FUNCTION_SDCARD_HSPI_CSO == id)                        p = PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR;
  if(GPIO_FUNCTION_SDCARD_HSPI_CLK == id)                        p = PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR;
  if(GPIO_FUNCTION_SDCARD_HSPI_MOSI == id)                        p = PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR;
  if(GPIO_FUNCTION_SDCARD_HSPI_MISO == id)                        p = PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR;
  #endif
  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  if(GPIO_OLED_RESET == id)                        p = PM_GPIO_FUNCTION_OLED_RESET_CTR;
  #endif

  /******************************************************************************************************************
   * Controller
  *******************************************************************************************************************/
  
  #ifdef USE_MODULE_CONTROLLER_FAN
  if(GPIO_FAN_PWM1 == id)                        p = PM_GPIO_FUNCTION_FAN_PWM1_CTR;
  #endif


  if(p != nullptr){
    // Copy the progmem string to the buffer
    strncpy_P(buffer, p, buflen - 1);
    buffer[buflen - 1] = '\0'; // Ensure null-termination
    return buffer;
  }

  // Default case if no match found
  snprintf_P(buffer, buflen, PSTR("GPIO%dNameMissing"),id);

  ALOG_ERR(PSTR(D_LOG_PINS "%s"), buffer);

  return buffer;

}


