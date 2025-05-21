#include "mHardwarePins.h"

/****
 * Since this is a long list, the GPIO from ID to name and back is defined in a separate file.
 */

 
// Function: Checks for gpio function as stored in progmem, returning its identifier if true, else -1
// Returns : #>=0:id found, -1: not found
// Notes: "else" not included in if structure as return is used 
//
int16_t mHardwarePins::GetGPIOFunctionIDbyName(const char* c){
  if(*c=='\0'){ return -1; }

  char buffer[50];  // Buffer to hold the formatted string (e.g., "SWT32" or "SWT1 Inv")
  
  if(strcmp_P(c,PM_GPIO_FUNCTION_NONE_CTR)==0){       return GPIO_NONE_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_DATA1_CTR)==0){   return GPIO_RGB_DATA_ID; } // force legacy pin version for now
  // if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_DATA_CTR)==0){   return GPIO_RGB_DATA_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_CLOCK_CTR)==0){  return GPIO_RGB_CLOCK_ID; }

  if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR)==0){   return GPIO_UNUSED_FORCED_HIGH_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR)==0){  return GPIO_UNUSED_FORCED_LOW_ID; }

  



  #ifdef USE_MODULE_SENSORS_DHT
  if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_1_CTR)==0){  return GPIO_DHT22_1OF2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_2_CTR)==0){  return GPIO_DHT22_2OF2_ID; }
  #endif

  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_1_CTR)==0){  return GPIO_DSB_1OF2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_2_CTR)==0){  return GPIO_DSB_2OF2_ID; }
  #endif

  
  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L0X_XSHUTNUM__CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_VL53L0X_XSHUT1_ID + (i - 1);
    }
  }
  #endif // USE_MODULE_SENSORS__TOF_VL53L1X
  #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  for (int i = 1; i <= 8; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION__TOF_VL53L1X_XSHUTNUM__CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_VL53L1X_XSHUT1_ID + (i - 1);
    }
  }
  #endif // USE_MODULE_SENSORS__TOF_VL53L1X

  
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR)==0){   return GPIO_LDR_BASIC_ANALOG1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR)==0){   return GPIO_LDR_BASIC_ANALOG2_ID; }
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  #ifdef USE_MODULE_SENSORS_SWITCHES
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_NP_CTR)==0){  return GPIO_SWT1_NP_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_NP_CTR)==0){  return GPIO_SWT2_NP_ID; }

  
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_INV_CTR)==0){  return GPIO_SWT1_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_INV_CTR)==0){  return GPIO_SWT2_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT3_INV_CTR)==0){  return GPIO_SWT3_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT4_INV_CTR)==0){  return GPIO_SWT4_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT5_INV_CTR)==0){  return GPIO_SWT5_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT6_INV_CTR)==0){  return GPIO_SWT6_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT7_INV_CTR)==0){  return GPIO_SWT7_INV_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT8_INV_CTR)==0){  return GPIO_SWT8_INV_ID; }

  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_CTR)==0){  return GPIO_SWT1_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_CTR)==0){  return GPIO_SWT2_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT3_CTR)==0){  return GPIO_SWT3_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT4_CTR)==0){  return GPIO_SWT4_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT5_CTR)==0){  return GPIO_SWT5_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT6_CTR)==0){  return GPIO_SWT6_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT7_CTR)==0){  return GPIO_SWT7_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SWT8_CTR)==0){  return GPIO_SWT8_ID; }

  // Check for "SWT1" to "SWT32"
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      Serial.printf("SWT1_INV_NP_CTR %s\n\r", buffer);
      return GPIO_SWT1_INV_NP_ID + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      Serial.printf("GPIO_SWT1_NP_ID %s\n\r", buffer);
      return GPIO_SWT1_NP_ID + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_INV_ID + (i - 1);
    }
  
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_ID + (i - 1);
    }
  }
  #endif

  #ifdef USE_MODULE_DRIVERS_RELAY
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_INV_CTR)==0){  return GPIO_REL1_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_INV_CTR)==0){  return GPIO_REL2_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_INV_CTR)==0){  return GPIO_REL3_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_INV_CTR)==0){  return GPIO_REL4_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_CTR)==0){       return GPIO_REL1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_CTR)==0){       return GPIO_REL2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_CTR)==0){       return GPIO_REL3_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_CTR)==0){       return GPIO_REL4_ID; }
  #endif
  
  #ifdef USE_MODULE_CONTROLLER_FAN
  if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_PWM1_CTR)==0){  return GPIO_FAN_PWM1_ID; }
  #endif

  #ifdef USE_MODULE_CORE_I2C
  if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SCL_CTR)==0){  return GPIO_I2C_SCL_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SDA_CTR)==0){  return GPIO_I2C_SDA_ID; }
  #endif

  
  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  if(strcmp_P(c,PM_GPIO_FUNCTION_OLED_RESET_CTR)==0){  return GPIO_OLED_RESET_ID; }
  #endif

  #ifdef USE_MODULE_SENSORS_PIR
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_CTR)==0){  return GPIO_PIR_1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_CTR)==0){  return GPIO_PIR_2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_CTR)==0){  return GPIO_PIR_3_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){  return GPIO_PIR_1_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){  return GPIO_PIR_2_INV_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){  return GPIO_PIR_3_INV_ID; }
  #endif
  
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_INPUT_CTR)==0){  return GPIO_CHIME_INPUT_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_RINGER_CTR)==0){  return GPIO_CHIME_RINGER_ID; }
  #endif

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  if(strcmp_P(c,PM_GPIO_FUNCTION_BUZZER_CTR)==0){  return GPIO_BUZZER_ID; }
  #endif


  




  // if(strcmp_P(c,PM_GPIO_FUNCTION_IRSEND_CTR)==0){  return GPIO_IRSEND_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_IRSEND_CTR)==0){  return GPIO_FAN_IRSEND_ID; }


  #ifdef USE_MODULE_DRIVERS_PWM
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM1_CTR)==0){  return GPIO_PWM1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM2_CTR)==0){  return GPIO_PWM2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM3_CTR)==0){  return GPIO_PWM3_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM4_CTR)==0){  return GPIO_PWM4_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PWM5_CTR)==0){  return GPIO_PWM5_ID; }
  #endif

  #ifdef USE_MODULE_CORE__SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)==0){  return GPIO_HWSERIAL0_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)==0){  return GPIO_HWSERIAL0_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR)==0){  return GPIO_HWSERIAL1_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR)==0){  return GPIO_HWSERIAL1_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR)==0){  return GPIO_HWSERIAL2_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR)==0){  return GPIO_HWSERIAL2_RX_ID; }
  #endif

  #ifdef ESP32
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_TX_ID; }
  #endif
  // if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)==0){  return GPIO_GPS_SERIAL0_TX_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)==0){  return GPIO_GPS_SERIAL0_RX_ID; }
  #ifdef ESP32
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR)==0){  return GPIO_GPS_SERIAL1_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR)==0){  return GPIO_GPS_SERIAL1_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR)==0){  return GPIO_GPS_SERIAL2_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR)==0){  return GPIO_GPS_SERIAL2_RX_ID; }
  #endif // 
  #endif

  
  //if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_CTR)==0){  return GPIO_LEDLNK_ID; }
  //if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_INV_CTR)==0){  return GPIO_LEDLNK_INV_ID; }


  #ifdef USE_MODULE_SENSORS_DOOR
  if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_OPEN_CTR)==0){  return GPIO_DOOR_OPEN_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_LOCK_CTR)==0){  return GPIO_DOOR_LOCK_ID; }
  #endif

  // if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)==0){  return GPIO_SERIAL_DEBUG_TX_ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)==0){  return GPIO_SERIAL_DEBUG_RX_ID; }


  


  #ifdef USE_MODULE_DRIVERS_LEDS
  for (int i = 1; i <= 8; i++) {
    // Check normal LEDs "LED1" to "LED8"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1_ID + (i - 1);  // Return the corresponding LED ID
    }

    // Check inverted LEDs "LED1 Inv" to "LED8 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1_INV_ID + (i - 1);  // Return the corresponding inverted LED ID
    }
  }
  #endif
  

  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){  return GPIO_PZEM0XX_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){  return GPIO_PZEM004_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR)==0){  return GPIO_PZEM0XX_RX_MODBUS_ID; }
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
  if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){  return GPIO_SR04_ECHO_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){  return GPIO_SR04_TRIG_ID; }
  #endif
  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  if(strcmp_P(c,PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR)==0){  return GPIO_LD2410_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR)==0){  return GPIO_LD2410_TX_ID; }
  #endif
  // #ifdef USE_MODULE_SENSORS_ULTRASONICS
  // if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR)==0){  return GPIO__ROTARY_ENCODER_A__ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR)==0){  return GPIO__ROTARY_ENCODER_B__ID; }
  // #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS

  for (int i = 1; i <= 32; i++) {
    // Check normal keys "KEY1" to "KEY32"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_ID + (i - 1);  // Return the corresponding key ID
    }

    // Check inverted keys "KEY1 Inv" to "KEY32 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV_ID + (i - 1);  // Return the corresponding inverted key ID
    }

    // Check non-pull keys "KEY1_NP" to "KEY32_NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_NP_ID + (i - 1);  // Return the corresponding non-pull key ID
    }

    // Check inverted non-pull keys "KEY1 Inv NP" to "KEY32 Inv NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV_NP_ID + (i - 1);  // Return the corresponding inverted non-pull key ID
    }

    // Check touch keys "KEY1_TOUCH" to "KEY32_TOUCH"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, i);
    ALOG_INF(PSTR("c=%s to %s"),c,buffer);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_TOUCH_ID + (i - 1);  // Return the corresponding touch key ID
    }
  }

  #endif

  // if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_PULLDOWN_CTR)==0){  return GPIO_KEY1_PULLDOWN_ID; }


  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_RX__CTR)==0){  return GPIO_RF_433MHZ_RX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_TX__CTR)==0){  return GPIO_RF_433MHZ_TX_ID; }
  #endif

  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){  return GPIO_NEXTION_TX_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){  return GPIO_NEXTION_RX_ID; }
  #endif

  #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_KEY__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_KEY__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_STATUS__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_NETWORK_STATUS__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RESET__CTR)==0){  return GPIO_FUNCTION__FONA_RESET__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_TX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_TX__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_RX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_RX__ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR)==0){  return GPIO_FUNCTION__FONA_RING_INDICATOR__ID; }
  #endif // 

  
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR)==0){  return GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_TX__CTR)==0){  return GPIO_FUNCTION__MODEM_TX__ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_RX__CTR)==0){  return GPIO_FUNCTION__MODEM_RX__ID; }
  // if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_POWER__CTR)==0){  return GPIO_FUNCTION__MODEM_POWER__ID; }



  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){  return GPIO_HBRIDGE_L9110_IA_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){  return GPIO_HBRIDGE_L9110_IB_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){  return GPIO_HBRIDGE_L9110_OA_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){  return GPIO_HBRIDGE_L9110_OB_ID; }
  #endif


  // if(strcmp_P(c,PM_GPIO_FUNCTION_ANALOG_POSITION_CTR)==0){  return GPIO_ANALOG_POSITION_ID; }

  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)==0){  return GPIO_ADE7953_IRQ_ID; }
  #endif

  
  #ifdef ESP32
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH4_CTR)==0){  return GPIO_ADC1_CH4_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH6_CTR)==0){  return GPIO_ADC1_CH6_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH7_CTR)==0){  return GPIO_ADC1_CH7_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR)==0){  return GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID; }

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  if(strcmp_P(c,PM_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR)==0){  return GPIO_FUNCTION_RXON_SAMPLING_ENABLED_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR)==0){  return GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_ID; }
  #endif //
  

  // Webcam
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR)==0){  return GPIO_WEBCAM_DATA1_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR)==0){  return GPIO_WEBCAM_DATA2_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR)==0){  return GPIO_WEBCAM_DATA3_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR)==0){  return GPIO_WEBCAM_DATA4_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR)==0){  return GPIO_WEBCAM_DATA5_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR)==0){  return GPIO_WEBCAM_DATA6_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR)==0){  return GPIO_WEBCAM_DATA7_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR)==0){  return GPIO_WEBCAM_DATA8_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)==0){   return GPIO_WEBCAM_XCLK_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)==0){   return GPIO_WEBCAM_PCLK_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)==0){   return GPIO_WEBCAM_HREF_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)==0){  return GPIO_WEBCAM_VSYNC_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)==0){   return GPIO_WEBCAM_SIOD_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)==0){   return GPIO_WEBCAM_SIOC_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)==0){   return GPIO_WEBCAM_PWDN_ID; }

  /**
   * SD Card
   **/
  #ifdef USE_MODULE_DRIVERS_SDCARD
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CSO_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CLK_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MISO_ID; }

  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CSO_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CLK_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MOSI_ID; }
  if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MISO_ID; }
  #endif // USE_MODULE_DRIVERS_SDCARD


  #endif // ESP32


  return -1;

}


const char* mHardwarePins::GetGPIOFunctionNamebyID(uint16_t id, char* buffer, uint8_t buflen) 
{

    // ALOG_INF(PSTR("---------------id %d"),id);

    #ifdef USE_MODULE_SENSORS_BUTTONS
    // Check for KEY1 to KEY32
    if (id >= GPIO_KEY1_ID && id <= GPIO_KEY32_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_CTR, id - GPIO_KEY1_ID + 1);
      return buffer;
    }
    // Check for KEY1_INV to KEY32_INV
    else if (id >= GPIO_KEY1_INV_ID && id <= GPIO_KEY32_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, id - GPIO_KEY1_INV_ID + 1);
      return buffer;
    }
    // Check for KEY1_NP to KEY32_NP
    else if (id >= GPIO_KEY1_NP_ID && id <= GPIO_KEY32_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, id - GPIO_KEY1_NP_ID + 1);
      return buffer;
    }
    // Check for KEY1_INV_NP to KEY32_INV_NP
    else if (id >= GPIO_KEY1_INV_NP_ID && id <= GPIO_KEY32_INV_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, id - GPIO_KEY1_INV_NP_ID + 1);
      return buffer;
    }
    // Check for KEY1_TOUCH to KEY32_TOUCH
    else if (id >= GPIO_KEY1_TOUCH_ID && id <= GPIO_KEY32_TOUCH_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, id - GPIO_KEY1_TOUCH_ID + 1);
      return buffer;
    }
    #endif // USE_MODULE_SENSORS_BUTTONS

    #ifdef USE_MODULE_SENSORS_SWITCHES
    // Check for SWT1 to SWT32
    if (id >= GPIO_SWT1_ID && id <= GPIO_SWT32_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_CTR, id - GPIO_SWT1_ID + 1);
      return buffer;
    }    
    // Check for SWT1_INV to SWT32_INV
    else if (id >= GPIO_SWT1_INV_ID && id <= GPIO_SWT32_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, id - GPIO_SWT1_INV_ID + 1);
      return buffer;
    }    
    // Check for SWT1_NP to SWT32_NP
    else if (id >= GPIO_SWT1_NP_ID && id <= GPIO_SWT32_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, id - GPIO_SWT1_NP_ID + 1);
      return buffer;
    }    
    // Check for SWT1_INV_NP to SWT32_INV_NP
    else if (id >= GPIO_SWT1_INV_NP_ID && id <= GPIO_SWT32_INV_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, id - GPIO_SWT1_INV_NP_ID + 1);
      return buffer;
    }
    
    #endif // USE_MODULE_SENSORS_SWITCHES

    #ifdef USE_MODULE_DRIVERS_LEDS
    // Check for LED1 to LED8
    if (id >= GPIO_LED1_ID && id <= GPIO_LED8_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_CTR, id - GPIO_LED1_ID + 1);
      return buffer;
    }

    // Check for LED1_INV to LED8_INV
    else if (id >= GPIO_LED1_INV_ID && id <= GPIO_LED8_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_INV_CTR, id - GPIO_LED1_INV_ID + 1);
      return buffer;
    }
    #endif


    // Step 2: Use the progmem function for other IDs
    PGM_P progmem_str = GetGPIOFunctionNamebyID_P(id);
    if (progmem_str != PM_SEARCH_NOMATCH) {
        // Copy the progmem string to the buffer
        strncpy_P(buffer, progmem_str, buflen - 1);
        buffer[buflen - 1] = '\0'; // Ensure null-termination
        return buffer;
    }
    
    // Default case if no match found
    snprintf_P(buffer, buflen, PSTR("No Match"));
    return buffer;
}


PGM_P mHardwarePins::GetGPIOFunctionNamebyID_P(uint16_t id){

ALOG_ERR(PSTR("GetGPIOFunctionNamebyID_P GPIO needs moved %d"), id);

  switch(id){
    default:           return PM_SEARCH_NOMATCH;
    case GPIO_NONE_ID: return PM_GPIO_FUNCTION_NONE_CTR;
    // case GPIO_NONE_ID: return PM_GPIO_NONE_CTR;

    // case GPIO_RGB_DATA_ID: return PM_GPIO_FUNCTION_RGB_DATA_CTR;



    #ifdef USE_MODULE_SENSORS_BUTTONS
    // case GPIO_KEY1_ID: return PM_GPIO_FUNCTION_KEY1_CTR;
    // case GPIO_KEY2_ID: return PM_GPIO_FUNCTION_KEY2_CTR;
    // case GPIO_KEY3_ID: return PM_GPIO_FUNCTION_KEY3_CTR;
    
    // case GPIO_KEY1_INV_ID: return PM_GPIO_FUNCTION_KEY1_INV_CTR;
    // case GPIO_KEY2_INV_ID: return PM_GPIO_FUNCTION_KEY2_INV_CTR;
    // case GPIO_KEY3_INV_ID: return PM_GPIO_FUNCTION_KEY3_INV_CTR;
    // case GPIO_KEY4_INV_ID: return PM_GPIO_FUNCTION_KEY4_INV_CTR;
    // case GPIO_KEY5_INV_ID: return PM_GPIO_FUNCTION_KEY5_INV_CTR;
    // case GPIO_KEY6_INV_ID: return PM_GPIO_FUNCTION_KEY6_INV_CTR;
    // case GPIO_KEY7_INV_ID: return PM_GPIO_FUNCTION_KEY7_INV_CTR;
    // case GPIO_KEY8_INV_ID: return PM_GPIO_FUNCTION_KEY8_INV_CTR;

    // case GPIO_KEY1_TOUCH_ID: return PM_GPIO_FUNCTION_KEY1_TOUCH_CTR;
    // case GPIO_KEY2_TOUCH_ID: return PM_GPIO_FUNCTION_KEY2_TOUCH_CTR;
    // case GPIO_KEY3_TOUCH_ID: return PM_GPIO_FUNCTION_KEY3_TOUCH_CTR;
    // case GPIO_KEY4_TOUCH_ID: return PM_GPIO_FUNCTION_KEY4_TOUCH_CTR;
    // case GPIO_KEY5_TOUCH_ID: return PM_GPIO_FUNCTION_KEY5_TOUCH_CTR;
    // case GPIO_KEY6_TOUCH_ID: return PM_GPIO_FUNCTION_KEY6_TOUCH_CTR;
    // case GPIO_KEY7_TOUCH_ID: return PM_GPIO_FUNCTION_KEY7_TOUCH_CTR;
    // case GPIO_KEY8_TOUCH_ID: return PM_GPIO_FUNCTION_KEY8_TOUCH_CTR;
    #endif

    #ifdef USE_MODULE_SENSORS_SWITCHES
    // case GPIO_SWT1_ID: return PM_GPIO_FUNCTION_SWT1_CTR; 
    // case GPIO_SWT2_ID: return PM_GPIO_FUNCTION_SWT2_CTR; 
    // case GPIO_SWT3_ID: return PM_GPIO_FUNCTION_SWT3_CTR; 


    // case  GPIO_SWT1_NP_ID: return PM_GPIO_FUNCTION_SWT1_NP_CTR;        
    //   GPIO_SWT1_NP_ID,
    //   GPIO_SWT2_ID,
    // case  GPIO_SWT2_NP_ID: return PM_GPIO_FUNCTION_SWT2_NP_CTR;        
    //   GPIO_SWT3_ID,
    //   GPIO_SWT3_NP_ID,
    //   GPIO_SWT4_ID,
    //   GPIO_SWT4_NP_ID,
    //   GPIO_SWT5_ID,
    //   GPIO_SWT5_NP_ID,
    //   GPIO_SWT6_ID,
    //   GPIO_SWT6_NP_ID,
    //   GPIO_SWT7_ID,
    //   GPIO_SWT7_NP_ID,
    //   GPIO_SWT8_ID,
    //   GPIO_SWT8_NP_ID,

    #endif


    #ifdef USE_MODULE_DRIVERS_RELAY
    case  GPIO_REL1_ID: return PM_GPIO_FUNCTION_REL1_CTR;           // Relays
    case  GPIO_REL1_INV_ID: return PM_GPIO_FUNCTION_REL1_INV_CTR;           // Relays
    case  GPIO_REL2_ID: return PM_GPIO_FUNCTION_REL2_CTR;           // Relays
    case  GPIO_REL2_INV_ID: return PM_GPIO_FUNCTION_REL2_INV_CTR;           // Relays
    case  GPIO_REL3_ID: return PM_GPIO_FUNCTION_REL3_CTR;           // Relays
    case  GPIO_REL3_INV_ID: return PM_GPIO_FUNCTION_REL3_INV_CTR;           // Relays
    case  GPIO_REL4_ID: return PM_GPIO_FUNCTION_REL4_CTR;           // Relays
    case  GPIO_REL4_INV_ID: return PM_GPIO_FUNCTION_REL4_INV_CTR;           // Relays
    #endif


    #ifdef USE_MODULE_CONTROLLER_FAN
    case  GPIO_FAN_PWM1_ID: return PM_GPIO_FUNCTION_FAN_PWM1_CTR;           // Relays
    #endif 
    
    #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
    case  GPIO_OLED_RESET_ID: return PM_GPIO_FUNCTION_OLED_RESET_CTR;           // Relays
    #endif

    
    #ifdef USE_MODULE_DRIVERS_PWM
    case GPIO_PWM1_ID:      return PM_GPIO_FUNCTION_PWM1_CTR;
    case GPIO_PWM1_INV_ID:  return PM_GPIO_FUNCTION_PWM1_INV_CTR;
    case GPIO_PWM2_ID:      return PM_GPIO_FUNCTION_PWM2_CTR;
    case GPIO_PWM2_INV_ID:  return PM_GPIO_FUNCTION_PWM2_INV_CTR;
    case GPIO_PWM3_ID:      return PM_GPIO_FUNCTION_PWM3_CTR;
    case GPIO_PWM3_INV_ID:  return PM_GPIO_FUNCTION_PWM3_INV_CTR;
    case GPIO_PWM4_ID:      return PM_GPIO_FUNCTION_PWM4_CTR;
    case GPIO_PWM4_INV_ID:  return PM_GPIO_FUNCTION_PWM4_INV_CTR;
    case GPIO_PWM5_ID:      return PM_GPIO_FUNCTION_PWM5_CTR;
    case GPIO_PWM5_INV_ID:  return PM_GPIO_FUNCTION_PWM5_INV_CTR;
    #endif



//   GPIO_CNTR1_ID,          // Counters
//   GPIO_CNTR1_NP_ID,
//   GPIO_CNTR2_ID,
//   GPIO_CNTR2_NP_ID,
//   GPIO_CNTR3_ID,
//   GPIO_CNTR3_NP_ID,
//   GPIO_CNTR4_ID,
//   GPIO_CNTR4_NP_ID,


#ifdef USE_MODULE_CORE_I2C
  case GPIO_I2C_SCL_ID: return PM_GPIO_FUNCTION_I2C_SCL_CTR;
  case GPIO_I2C_SDA_ID: return PM_GPIO_FUNCTION_I2C_SDA_CTR;
#endif
// #ifdef USE_SPI
//   GPIO_SPI_CS_ID,         // SPI Chip Select
//   GPIO_SPI_DC_ID,         // SPI Data Direction
//   GPIO_SSPI_MISO_ID,      // Software SPI Master Input Slave Output
//   GPIO_SSPI_MOSI_ID,      // Software SPI Master Output Slave Input
//   GPIO_SSPI_SCLK_ID,      // Software SPI Serial Clock
//   GPIO_SSPI_CS_ID,        // Software SPI Chip Select
//   GPIO_SSPI_DC_ID,        // Software SPI Data or Command
// #endif
// #ifdef USE_DISPLAY
//   GPIO_BACKLIGHT_ID,      // Display backlight control
// #endif

  // case GPIO_ANALOG_ID: return PM_GPIO_FUNCTION_ANALOG_CTR;

  #ifdef USE_MODULE_SENSORS_DHT
  case GPIO_DHT11_1OF2_ID: return PM_GPIO_FUNCTION_DHT11_1_CTR;
  case GPIO_DHT11_2OF2_ID: return PM_GPIO_FUNCTION_DHT11_2_CTR;
  case GPIO_DHT22_1OF2_ID: return PM_GPIO_FUNCTION_DHT22_1_CTR;
  case GPIO_DHT22_2OF2_ID: return PM_GPIO_FUNCTION_DHT22_2_CTR;
  #endif // USE_MODULE_SENSORS_DHT

//   GPIO_SI7021_ID,         // iTead SI7021

  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  case GPIO_DSB_1OF2_ID: return PM_GPIO_FUNCTION_DS18X20_1_CTR;
  case GPIO_DSB_2OF2_ID: return PM_GPIO_FUNCTION_DS18X20_2_CTR;
  #endif

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  case GPIO_LDR_BASIC_DIGITAL1_ID: return PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR;
  case GPIO_LDR_BASIC_ANALOG1_ID:  return PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR;
  case GPIO_LDR_BASIC_DIGITAL2_ID: return PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR;
  case GPIO_LDR_BASIC_ANALOG2_ID:  return PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR;
  #endif // USE_MODULE_SENSORS_LDR_BASIC

  
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  case GPIO_BUZZER_ID: return PM_GPIO_FUNCTION_BUZZER_CTR;
  #endif

  #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  case GPIO_LD2410_RX_ID: return PM_GPIO_FUNCTION__HLK_LD2410_RX__CTR;
  case GPIO_LD2410_TX_ID: return PM_GPIO_FUNCTION__HLK_LD2410_TX__CTR;
  #endif



// #ifdef USE_WS2812
//   GPIO_RGB_DATA_ID,         // WS2812 Led string
//   GPIO_RGB_CLOCK_ID,         // WS2812 Led string
// #endif
// #ifdef USE_IR_REMOTE
//   GPIO_IRSEND_ID,         // IR remote
// #ifdef USE_IR_RECEIVE
//   GPIO_IRRECV_ID,         // IR receiver
// #endif
// #endif
// #ifdef USE_RC_SWITCH
//   GPIO_RFSEND_ID,         // RF transmitter
//   GPIO_RFRECV_ID,         // RF receiver
// #endif
// #ifdef USE_RF_SENSOR
//   GPIO_RF_SENSOR_ID,      // Rf receiver with sensor decodingGPIO_RF_433MHZ_RX_ID
// #endif
// #ifdef USE_SR04
//   GPIO_SR04_TRIG_ID,      // SR04 Trigger pin
//   GPIO_SR04_ECHO_ID,      // SR04 Echo pin
// #endif
// #ifdef USE_TM1638
//   GPIO_TM16CLK_ID,        // TM1638 Clock
//   GPIO_TM16DIO_ID,        // TM1638 Data I/O
//   GPIO_TM16STB_ID,        // TM1638 Strobe
// #endif
// #ifdef USE_HX711
//   GPIO_HX711_SCK_ID,      // HX711 Load Cell clock
//   GPIO_HX711_DAT_ID,      // HX711 Load Cell data
// #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
//   GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
//   GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
//   GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
//   GPIO_HLW_CF_ID,         // HLW8012 CF power
//   GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
// #endif


//   GPIO_CSE7766_TX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
//   GPIO_CSE7766_RX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
// #if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
//   GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
// #endif


#if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_AC) || defined(USE_PZEM_DC)
  case GPIO_PZEM0XX_TX_ID: return PM_GPIO_FUNCTION_PZEM0XX_TX_CTR;
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  case  GPIO_PZEM004_RX_ID: return PM_GPIO_FUNCTION_PZEM004_RX_CTR;     
#endif   
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  case  GPIO_PZEM0XX_RX_MODBUS_ID: return PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR;  
#endif
// // #ifdef USE_PZEM_DC
//   GPIO_PZEM017_RX_ID,     // PZEM-003_ID,017 Serial Modbus interface
// // #endif


// #ifdef USE_SDM120
//   GPIO_SDM120_TX_ID,      // SDM120 Serial interface
//   GPIO_SDM120_RX_ID,      // SDM120 Serial interface
// #endif
// #ifdef USE_SDM630
//   GPIO_SDM630_TX_ID,      // SDM630 Serial interface
//   GPIO_SDM630_RX_ID,      // SDM630 Serial interface
// #endif
// #ifdef USE_SERIAL_BRIDGE
//   GPIO_SBR_TX_ID,         // Serial Bridge Serial interface
//   GPIO_SBR_RX_ID,         // Serial Bridge Serial interface
// #endif
// #ifdef USE_MHZ19
//   GPIO_MHZ_TXD_ID,        // MH-Z19 Serial interface
//   GPIO_MHZ_RXD_ID,        // MH-Z19 Serial interface
// #endif
// #ifdef USE_SENSEAIR
//   GPIO_SAIR_TX_ID,        // SenseAir Serial interface
//   GPIO_SAIR_RX_ID,        // SenseAir Serial interface
// #endif
// #ifdef USE_NOVA_SDS
//   GPIO_SDS0X1_TX_ID,      // Nova Fitness SDS011 Serial interface
//   GPIO_SDS0X1_RX_ID,      // Nova Fitness SDS011 Serial interface
// #endif
// #ifdef USE_PMS5003
//   GPIO_PMS5003_ID,        // Plantower PMS5003 Serial interface
// #endif
// #ifdef USE_TX20_WIND_SENSOR
//   GPIO_TX20_TXD_BLACK_ID, // TX20 Transmission Pin
// #endif
// #ifdef USE_MP3_PLAYER
//   GPIO_MP3_DFR562_ID,     // RB-DFR-562_ID, DFPlayer Mini MP3 Player Serial interface
// #endif
// #ifdef USE_TUYA_DIMMER
//   GPIO_TUYA_TX_ID,        // Tuya Serial interface
//   GPIO_TUYA_RX_ID,        // Tuya Serial interface
// #endif
// #ifdef USE_AZ7798
//   GPIO_AZ_TXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
//   GPIO_AZ_RXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
// #endif
// #ifdef USE_PN532_HSU
//   GPIO_PN532_TXD_ID,      // PN532 HSU Tx
//   GPIO_PN532_RXD_ID,      // PN532 HSU Rx
// #endif
// #ifdef USE_MGC3130
//   GPIO_MGC3130_XFER_ID,
//   GPIO_MGC3130_RESET_ID,
// #endif
// #ifdef USE_MAX31855
//   GPIO_MAX31855CS_ID,     // MAX31855 Serial interface
//   GPIO_MAX31855CLK_ID,    // MAX31855 Serial interface
//   GPIO_MAX31855DO_ID,     // MAX31855 Serial interface
// #endif
//   GPIO_DI_ID,             // my92x1 PWM input
//   GPIO_DCKI_ID,           // my92x1 CLK input
// #ifdef USE_SM16716
//   GPIO_SM16716_CLK_ID,    // SM16716 CLOCK
//   GPIO_SM16716_DAT_ID,    // SM16716 DATA
//   GPIO_SM16716_SEL_ID,    // SM16716 SELECT
// #endif // USE_SM16716
// #ifdef ROTARY_V1
//   GPIO_ROT1A_ID,          // Rotary switch1 A Pin
//   GPIO_ROT1B_ID,          // Rotary switch1 B Pin
//   GPIO_ROT2A_ID,          // Rotary switch2 A Pin
//   GPIO_ROT2B_ID,          // Rotary switch2 B Pin
// #endif
// #ifdef USE_ARILUX_RF
//   GPIO_ARIRFRCV_ID,       // AliLux RF Receive input
// #endif
// #ifdef USE_HRE
//   GPIO_HRE_CLOCK_ID,
//   GPIO_HRE_DATA
// #endif
// #ifdef USE_SENSOR_DOOR_LOCK
//   GPIO_DOOR_LOCK_ID,
// #endif
// #ifdef USE_MODULE_DRIVERS_HBRIDGE
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_ANALOG_SENSE_ID,
// #endif

  /******************************************************************************************************************
   * Network
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Drivers
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Sensors
  *******************************************************************************************************************/

#ifdef USE_MODULE_SENSORS_DOOR
    case GPIO_DOOR_OPEN_ID: return PM_GPIO_FUNCTION_DOOR_OPEN_CTR;
#endif
    // case GPIO_PIR_1_ID: return PM_GPIO_FUNCTION_PIR_1_CTR;
    // case GPIO_PIR_1_NP_ID: return PM_GPIO_FUNCTION_PIR_1_NP_CTR;
    // case GPIO_PIR_1_INV_ID: return PM_GPIO_FUNCTION_PIR_1_INV_CTR;

    // case GPIO_PIR_2_ID: return PM_GPIO_FUNCTION_PIR_2_CTR;
    // case GPIO_PIR_2_NP_ID: return PM_GPIO_FUNCTION_PIR_2_NP_CTR;
    // case GPIO_PIR_2_INV_ID: return PM_GPIO_FUNCTION_PIR_2_INV_CTR;

    // case GPIO_PIR_3_ID: return PM_GPIO_FUNCTION_PIR_3_CTR;
    // case GPIO_PIR_3_NP_ID: return PM_GPIO_FUNCTION_PIR_3_NP_CTR;
    // case GPIO_PIR_3_INV_ID: return PM_GPIO_FUNCTION_PIR_3_INV_CTR;

  /******************************************************************************************************************
   * Lights
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  case  GPIO_ADE7953_IRQ_ID: return PM_GPIO_FUNCTION_ADE7953_IRQ_CTR;        // ADE7953 IRQ
  #endif


  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  case GPIO_NEXTION_RX_ID: return PM_GPIO_FUNCTION_NEXTION_RX_CTR;
  case GPIO_NEXTION_TX_ID: return PM_GPIO_FUNCTION_NEXTION_TX_CTR;
  #endif // USE_MODULE_DISPLAYS_NEXTION


  // case GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__ID: return PM_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR;
  // case GPIO_FUNCTION__MODEM_TX__ID: return PM_GPIO_FUNCTION__MODEM_TX__CTR;
  // case GPIO_FUNCTION__MODEM_RX__ID: return PM_GPIO_FUNCTION__MODEM_RX__CTR;
  // case GPIO_FUNCTION__MODEM_POWER__ID: return PM_GPIO_FUNCTION__MODEM_POWER__CTR;







  #ifdef ESP32

  
  case GPIO_HWSERIAL0_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR;
  case GPIO_HWSERIAL0_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR;
  case GPIO_HWSERIAL1_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR;
  case GPIO_HWSERIAL1_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR;
  case GPIO_HWSERIAL2_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR;
  case GPIO_HWSERIAL2_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR;



  case GPIO_HWSERIAL1_RING_BUFFER_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR;
  case GPIO_HWSERIAL1_RING_BUFFER_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR;
  case GPIO_HWSERIAL2_RING_BUFFER_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR;
  case GPIO_HWSERIAL2_RING_BUFFER_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR;






  // Webcam
  case GPIO_WEBCAM_DATA1_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR;
  case GPIO_WEBCAM_DATA2_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR;
  case GPIO_WEBCAM_DATA3_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR;
  case GPIO_WEBCAM_DATA4_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR;
  case GPIO_WEBCAM_DATA5_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR;
  case GPIO_WEBCAM_DATA6_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR;
  case GPIO_WEBCAM_DATA7_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR;
  case GPIO_WEBCAM_DATA8_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR;
  case GPIO_WEBCAM_XCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR;
  case GPIO_WEBCAM_PCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR;
  case GPIO_WEBCAM_HREF_ID: return PM_GPIO_FUNCTION_WEBCAM_HREF_CTR;
  case GPIO_WEBCAM_VSYNC_ID: return PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR;
  case GPIO_WEBCAM_SIOD_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR;
  case GPIO_WEBCAM_SIOC_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR;
  case GPIO_WEBCAM_PWDN_ID: return PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR;


  /**
   * SD Card
   **/
  #ifdef USE_MODULE_DRIVERS_SDCARD
  case GPIO_FUNCTION_SDCARD_VSPI_CSO_ID:    return PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_CLK_ID:    return PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID:   return PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_MISO_ID:   return PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_CSO_ID:    return PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_CLK_ID:    return PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_MOSI_ID:   return PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_MISO_ID:   return PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR;
  #endif


  #endif // ESP32


//    case GPIO_LEDLNK_INV_ID: return PM_GPIO_FUNCTION_LEDLNK_INV_CTR;

  }
}


