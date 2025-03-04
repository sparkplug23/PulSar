/**
 * @file mTOF_VL53L0X.cpp VL53L0X time of flight multiple sensors support
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2025
 */ 
/*********************************************************************************************\
 * VL53L0x time of flight sensor
 *
 * I2C Addres: 0x29
 *********************************************************************************************
 *
 * Note: When using multiple VL53L0X, it is required to also wire the XSHUT pin of all those sensors
 * in order to let Tasmota change by software the I2C address of those and give them an unique address
 * for operation. The sensor don't save its address, so this procedure of changing its address is needed
 * to be performed every restart. The Addresses used for this are 120 (0x78) to 127 (0x7F). In the I2c
 * Standard (https://i2cdevices.org/addresses) those addresses are used by the PCA9685, so take into
 * account they won't work together.
 *
 * The default value of VL53LXX_MAX_SENSORS is set in the file tasmota.h
 * Changing that is backwards incompatible - Max supported devices by this driver are 8
 *
 **********************************************************************************************
 *
 * How to install this sensor: https://www.st.com/resource/en/datasheet/vl53l0x.pdf
 *
 * If you are going to use long I2C wires read this:
 * https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/
 *
\*********************************************************************************************/

#include "mTOF_VL53L0X.h"

#ifdef USE_MODULE_SENSORS__TOF_VL53L0X

int8_t mTOF_VL53L0X::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      // BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

    switch(function){
      /************
       * PERIODIC SECTION * 
      *******************/
      case TASK_LOOP: 
        // Loop();
      break;
      case TASK_EVERY_SECOND:
        ReadSensor();
      break;
      /************
       * MQTT SECTION * 
      *******************/
      #ifdef USE_MODULE_NETWORK_MQTT
      case TASK_MQTT_HANDLERS_INIT:
        MQTTHandler_Init();
      break;
      case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
        pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
      break;
      case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
        pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
      break;
      case TASK_MQTT_SENDER:
        pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
      break;
      #endif //USE_MODULE_NETWORK_MQTT
    }

  return FUNCTION_RESULT_SUCCESS_ID;

}

/********************************************************************************************/
/**
 * @brief Pre-scans the I2C bus to detect active VL53L0X sensors before initialization.
 *
 * This function:
 *  - Disables all sensors using XSHUT if available.
 *  - Ensures only one sensor is enabled at a time if using XSHUT.
 */
/**
 * @brief Pre-scans the I2C bus to detect active VL53L0X sensors before initialization.
 *
 * This function:
 *  - Scans for sensors at **default (0x29) and alternate (0x2A, 0x2B, ...) addresses**.
 *  - Stores the **number of detected sensors** to determine if we need **XSHUT toggling**.
 *  - Ensures only one sensor is enabled at a time if using XSHUT.
 */
void mTOF_VL53L0X::Pre_Init(void) { 
  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Pre-Init - Scanning I2C Bus"));

  #ifdef ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29
  if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, 0))
  {
    digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, 0), LOW);
  }
  ALOG_WRN(PSTR("TMP DISABLING ANY XSHUT 1X or 0X"));

  // this can be fixed by keeping this in preinit, and moving all code below into the init.

  #endif

}



/**
 * @brief Initializes VL53L0X sensors after detecting them in `Pre_Init()`.
 *
 * This function:
 *  - Uses `module_state.devices` (set in `Pre_Init()`) to determine the number of sensors.
 *  - If **no XSHUT pins are defined**, runs **continuous mode** at `0x29`.
 *  - If **any XSHUT pin is defined**, runs **single-shot mode** at `0x29` (to avoid conflicts with VL53L1X).
 *  - If **multiple sensors**, assigns **alternate I2C addresses** and runs **single-shot mode**.
 */
void mTOF_VL53L0X::Init(void) { 

  
  if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, 0))
  {
    digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, 0), HIGH);
  }
  delay(10);
  
  module_state.devices = 0;
  VL53L0X_xshut_enabled = false;

  // Check if a sensor is at the default I2C address (0x29)
  if (tkr_i2c->I2cSetDevice(VL53L0X_ADDRESS)) {
      ALOG_INF(PSTR("VL53L0X found at 0x29"));
      module_state.devices++;
  }

  // Check if sensors exist at alternate addresses (0x2A - 0x2F)
  for (uint8_t addr = VL53L0X_XSHUT_ADDRESS; addr < (VL53L0X_XSHUT_ADDRESS + VL53LXX_MAX_SENSORS); addr++) {
      if (tkr_i2c->I2cSetDevice(addr)) {
          ALOG_INF(PSTR("VL53L0X found at 0x%02X"), addr);
          module_state.devices++;
      }
  }

  // Determine if any XSHUT pin is used
  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
      if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, i)) {
          VL53L0X_xshut_enabled = true;
          break;
      }
  }

  // Log the number of detected sensors
  ALOG_INF(PSTR("Total VL53L0X sensors detected: %d"), module_state.devices);



  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init"));

  if (module_state.devices == 0) {
      ALOG_INF(PSTR("No VL53L0X sensors detected, skipping initialization."));
      return;
  }
  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init2"));


  for (uint32_t i = 0; i < module_state.devices; i++) {
      bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, i);

      if (use_xshut) {
          digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), HIGH);
          delay(2); // XSHUT boot delay (max 1.2 ms in datasheet)
      }

      ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init3"));

      ALOG_INF(PSTR("VL53L0X[%d] Init"), i);

      if (VL53L0X_device[i].init()) {
        ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init4"));
      
          ALOG_INF(PSTR("VL53L0X %d detected"), i);

          // **Assign new addresses only if multiple sensors exist**
          if (module_state.devices > 1) {
            ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init5"));
          
              uint8_t new_address = VL53L0X_XSHUT_ADDRESS + i;
              VL53L0X_device[i].setAddress(new_address);
              Vl53l0x_data[i].address = new_address;
              ALOG_INF(PSTR("VL53L0X %d assigned new address: 0x%02X"), i, new_address);
              
              // **Re-initialize sensor after changing address**
              if (!VL53L0X_device[i].init()) {
                  ALOG_INF(PSTR("VL53L0X %d failed to re-init after address change"), i);
                  continue;
              }
          } else {
            ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init6"));
          
              // Single sensor case
              Vl53l0x_data[i].address = VL53L0X_ADDRESS;
              ALOG_INF(PSTR("VL53L0X %d remains at default address (0x29)"), i);
          }

          ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init7"));

          uint8_t addr = VL53L0X_device[i].getAddress();
          ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init8"));
        
          tkr_i2c->I2cSetActive(addr);

          ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init9"));

          // **VL53L0X Specific Settings**
          VL53L0X_device[i].setTimeout(500);
          #if defined VL53L0X_LONG_RANGE
          VL53L0X_device[i].setSignalRateLimit(0.1);
          VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
          VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
          #endif
          #if defined VL53L0X_HIGH_SPEED
          VL53L0X_device[i].setMeasurementTimingBudget(20000);
          #elif defined VL53L0X_HIGH_ACCURACY
          VL53L0X_device[i].setMeasurementTimingBudget(200000);
          #endif
          ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init10"));
        

          // **Determine whether to use continuous or single-shot mode**
          if (module_state.devices == 1 && !VL53L0X_xshut_enabled) {
              VL53L0X_device[i].startContinuous();
              ALOG_INF(PSTR("VL53L0X %d: Using CONTINUOUS measurement mode (No XSHUT, No Conflict)"), i);
          } else {
              ALOG_INF(PSTR("VL53L0X %d: Using SINGLE measurement mode"), i);
          }

          Vl53l0x_data[i].ready = true;
          Vl53l0x_data[i].index = 0;
          VL53L0X_detected = true;
      }else{
          ALOG_INF(PSTR("VL53L0X %d failed to start"), i);
      }

      // If using XSHUT, disable the sensor after setup
      if (use_xshut) {
          digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), LOW);
          ALOG_INF(PSTR("VL53L0X %d disabled after setup"), i);
      }
  }

  if (module_state.devices > 0) {
      module_state.mode = ModuleStatus::Running;
  }

  ALOG_HGL(PSTR("END OF INIT"));
}


/**
 * @brief Reads distance measurements from VL53L0X sensors.
 *
 * This function:
 *  - Uses `VL53L0X_xshut_enabled` to determine if **single-shot or continuous mode** is used.
 *  - In **continuous mode**, reads directly from the sensor.
 *  - In **single-shot mode**, enables XSHUT, takes a measurement, then disables XSHUT to free the I2C bus.
 */
void mTOF_VL53L0X::ReadSensor(void) {

  #ifdef USE_DEEPSLEEP
      if (VL53L0X_standby) return;
  #endif

  for (uint32_t i = 0; i < module_state.devices; i++) {
      if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, i) || (!VL53L0X_xshut)) {

          uint16_t dist = 0;

          if (VL53L0X_xshut_enabled) {
              // Enable XSHUT before reading
              digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), HIGH);
              delay(2); // XSHUT boot delay

              // Read single-shot measurement
              dist = VL53L0X_device[i].readRangeSingleMillimeters();

              // Disable XSHUT after reading
              digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), LOW);
          } else {
              // Direct read in continuous mode
              dist = VL53L0X_device[i].readRangeContinuousMillimeters();
          }

          // Handle invalid readings
          if (!dist || dist > 2200) {
              dist = 9999;
          }

          Vl53l0x_data[i].distance = dist;

          ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - Distance: %d mm"), i+1, Vl53l0x_data[i].distance);
      }else{
          ALOG_INF(PSTR("VL53L0X %d failed to initialize"), i);
      }
  }
  ALOG_INF(PSTR("END OF READ"));
}


// void mTOF_VL53L0X::Pre_Init(void) { 
//   ALOG_INF(PSTR(D_LOG_TOF_VL53L0X "Pre_Init - Disabling all TOF sensors"));

//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//       if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i)) {
//           pinMode(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), OUTPUT);
//           digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), LOW); // Disable VL53L0X
//       }
//   }

//   // If VL53L1X is in another module, it should also be disabled there.
//   ALOG_INF(PSTR("Pre_Init - All sensors disabled"));
// }


// void mTOF_VL53L0X::Init(void) { 
//   ALOG_INF(PSTR(D_LOG_TOF_VL53L0X "Init"));

//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//       if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i)) {
//           pinMode(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), OUTPUT);
//           digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), HIGH); // Enable VL53L0X
//           delay(10); // Allow sensor bootup
//       }

//       if (tkr_i2c->I2cSetDevice(VL53L0X_ADDRESS) || tkr_i2c->I2cSetDevice((uint8_t)(VL53L0X_XSHUT_ADDRESS+i))) { 
//           if (VL53L0X_device[i].init()) {
//               ALOG_INF(PSTR("VL53L0X %d detected"), i);

//               if (VL53L0X_xshut) {
//                   VL53L0X_device[i].setAddress((uint8_t)(VL53L0X_XSHUT_ADDRESS+i));
//               }

//               uint8_t addr = VL53L0X_device[i].getAddress();
//               ALOG_INF(PSTR("VL53L0X %d detected - New address 0x%02X"), i, addr);

//               tkr_i2c->I2cSetActive(addr);
//               AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);

//               VL53L0X_device[i].setTimeout(500);
//               #if defined VL53L0X_LONG_RANGE
//               VL53L0X_device[i].setSignalRateLimit(0.1);
//               VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//               VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//               #endif
//               #if defined VL53L0X_HIGH_SPEED
//               VL53L0X_device[i].setMeasurementTimingBudget(20000);
//               #elif defined VL53L0X_HIGH_ACCURACY
//               VL53L0X_device[i].setMeasurementTimingBudget(200000);
//               #endif
//               VL53L0X_device[i].startContinuous();

//               Vl53l0x_data[i].ready = true;
//               Vl53l0x_data[i].index = 0;
//               VL53L0X_detected = true;
//           }
//       }
      
//       // Disable XSHUT again after initialization
//       digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), LOW);
//   }
  
//   ALOG_HGL(PSTR("END OF INIT"));
// }
// void mTOF_VL53L0X::ReadSensor(void) {

//   #ifdef USE_DEEPSLEEP
//     // Prevent updating measurements once VL53L0X has been put to sleep
//     if (VL53L0X_standby) return;
//   #endif
  
//     for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//       if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i) || (!VL53L0X_xshut)) {
        
//         // Enable XSHUT before reading
//         digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), HIGH);
//         delay(5); // Allow sensor to stabilize
  
//         uint16_t dist = VL53L0X_device[i].readRangeContinuousMillimeters();
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_DISTANCE " %d mm"), i+1, dist);
        
//         // Disable XSHUT after reading
//         digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), LOW);
  
//         // Handle invalid readings
//         if (dist == 65535) {
//           ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " READING FAILED"));
//         }
//         if ((dist == 0) || (dist > 2200)) {
//           dist = 9999;
//         }
  
//   #ifdef USE_VL_MEDIAN
//         // Store in ring buffer
//         Vl53l0x_data[i].buffer[Vl53l0x_data[i].index] = dist;
//         Vl53l0x_data[i].index++;
//         if (Vl53l0x_data[i].index >= USE_VL_MEDIAN_SIZE) {
//           Vl53l0x_data[i].index = 0;
//         }
  
//         // Sort list and take median
//         uint16_t tbuff[USE_VL_MEDIAN_SIZE];
//         memmove(tbuff, Vl53l0x_data[i].buffer, sizeof(tbuff));
//         uint16_t tmp;
//         uint8_t flag;
//         for (uint32_t ocnt = 0; ocnt < USE_VL_MEDIAN_SIZE; ocnt++) {
//           flag = 0;
//           for (uint32_t count = 0; count < USE_VL_MEDIAN_SIZE - 1; count++) {
//             if (tbuff[count] > tbuff[count + 1]) {
//               tmp = tbuff[count];
//               tbuff[count] = tbuff[count + 1];
//               tbuff[count + 1] = tmp;
//               flag = 1;
//             }
//           }
//           if (!flag) { break; }
//         }
//         Vl53l0x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE - 1) / 2];
//   #else
//         Vl53l0x_data[i].distance = dist;
//   #endif
  
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_DISTANCE " %d mm"), i+1, Vl53l0x_data[i].distance);
//       }
  
//       if (!VL53L0X_xshut) { break; } // Exit if using a single shared sensor
//     }
//   }
  


// void mTOF_VL53L0X::Init(void) { // Detect

//   ALOG_INF(PSTR(D_LOG_TOF_VL53L0X "Init"));

//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//     if ( tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i)) {
//       pinMode(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), OUTPUT);
//       digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), i==0 ? 1 : 0);
//       VL53L0X_xshut = true;
//       ALOG_INF(PSTR("VL53L0X %d XSHUT %d"), i, i==0 ? 1 : 0);
//     }
//   }

//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//     if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i) || (!VL53L0X_xshut)) {
//         if (VL53L0X_xshut) { pinMode(tkr_pins->Pin(GPIO_VL53L0X_XSHUT_ID, i), INPUT); delay(1); }
//         if (!tkr_i2c->I2cSetDevice(VL53L0X_ADDRESS) && !tkr_i2c->I2cSetDevice((uint8_t)(VL53L0X_XSHUT_ADDRESS+i))) { return; } // Detection for unconfigured OR configured sensor
//         if (VL53L0X_device[i].init()) 
//         {
//           ALOG_INF(PSTR("VL53L0X %d detected"), i);
        
//           if (VL53L0X_xshut) { VL53L0X_device[i].setAddress((uint8_t)(VL53L0X_XSHUT_ADDRESS+i)); }
//           uint8_t addr = VL53L0X_device[i].getAddress();
//           ALOG_INF(PSTR("VL53L0X %d detected - New address 0x%02X"), i, addr);
//           if (VL53L0X_xshut) {
//             tkr_i2c->I2cSetActive(addr);
//               AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);
//           } else {
//             tkr_i2c->I2cSetActiveFound(addr, "VL53L0X");
//           }
//           VL53L0X_device[i].setTimeout(500);

//           #if defined VL53L0X_LONG_RANGE
//           // lower the return signal rate limit (default is 0.25 MCPS)
//           VL53L0X_device[i].setSignalRateLimit(0.1);
//           // increase laser pulse periods (defaults are 14 and 10 PCLKs)
//           VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
//           VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
//           #endif
//           #if defined VL53L0X_HIGH_SPEED
//           // reduce timing budget to 20 ms (default is about 33 ms)
//           VL53L0X_device[i].setMeasurementTimingBudget(20000);
//           #elif defined VL53L0X_HIGH_ACCURACY
//           // increase timing budget to 200 ms
//           VL53L0X_device[i].setMeasurementTimingBudget(200000);
//           #endif
//           // Start continuous back-to-back mode (take readings as
//           // fast as possible).  To use continuous timed mode
//           // instead, provide a desired inter-measurement period in
//           // ms (e.g. sensor.startContinuous(100)).
//           VL53L0X_device[i].startContinuous();

//           Vl53l0x_data[i].ready = true;
//           Vl53l0x_data[i].index = 0;
//           VL53L0X_detected = true;
//           if (!VL53L0X_xshut) { break; }

//         } else {
//             AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_FAILED_TO_START), i+1);
//         }
//     }
//   }

//   ALOG_HGL(PSTR("END OF INIT"));
  
// }


  // Serial.print(VL53L0X_device[0].readRangeSingleMillimeters());
  // if (VL53L0X_device[0].timeoutOccurred()) { Serial.print(" TIMEOUT"); }




// void mTOF_VL53L0X::ReadSensor(void) {
  


// #ifdef USE_DEEPSLEEP
//   // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
//   if (VL53L0X_standby) return;
// #endif
//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//     if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT_ID, i) || (!VL53L0X_xshut)) {
//         uint16_t dist = VL53L0X_device[i].readRangeContinuousMillimeters();
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L0X dist %d - " D_DISTANCE " %d mm"), i+1, Vl53l0x_data[i].distance);
        
//         if(dist==65535){
//           ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " READING FAILED"));
//         }
//         if ((0 == dist) || (dist > 2200)) {
//             dist = 9999;
//         }

// #ifdef USE_VL_MEDIAN
//         // store in ring buffer
//         Vl53l0x_data[i].buffer[Vl53l0x_data[i].index] = dist;
//         Vl53l0x_data[i].index++;
//         if (Vl53l0x_data[i].index >= USE_VL_MEDIAN_SIZE) {
//             Vl53l0x_data[i].index = 0;
//         }

//         // sort list and take median
//         uint16_t tbuff[USE_VL_MEDIAN_SIZE];
//         memmove(tbuff, Vl53l0x_data[i].buffer, sizeof(tbuff));
//         uint16_t tmp;
//         uint8_t flag;
//         for (uint32_t ocnt = 0; ocnt < USE_VL_MEDIAN_SIZE; ocnt++) {
//             flag = 0;
//             for (uint32_t count = 0; count < USE_VL_MEDIAN_SIZE -1; count++) {
//             if (tbuff[count] > tbuff[count +1]) {
//                 tmp = tbuff[count];
//                 tbuff[count] = tbuff[count +1];
//                 tbuff[count +1] = tmp;
//                 flag = 1;
//             }
//             }
//             if (!flag) { break; }
//         }
//         Vl53l0x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE -1) / 2];
// #else
//         Vl53l0x_data[i].distance = dist;
// #endif
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_DISTANCE " %d mm"), i+1, Vl53l0x_data[i].distance);
//     }
//     if (!VL53L0X_xshut) { break; }
//   }
// }

#ifdef USE_DOMOTICZ
void Vl53l0Every_Second(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
  if (VL53L0X_standby) return;
#endif
  if (abs(Vl53l0x_data[0].distance - Vl53l0x_data[0].distance_prev) > 8) {
    Vl53l0x_data[0].distance_prev = Vl53l0x_data[0].distance;
    float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
    DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
  }
}
#endif  // USE_DOMOTICZ

// void Vl53l0Show(boolean json) {
//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//     char types[12] = "VL53L0X";
//     if (VL53L0X_xshut) {
//       snprintf_P(types, sizeof(types), PSTR("VL53L0X%c%d"), IndexSeparator(), i +1);
//     }
//     if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
//       float distance = (Vl53l0x_data[i].distance == 9999) ? NAN : (float)Vl53l0x_data[i].distance / 10;  // cm
//       if (json) {
//         ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
// #ifdef USE_WEBSERVER
//       } else {
//         WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
// #endif
//       }
//     }
//     if (VL53L0X_device[i].timeoutOccurred()) {
//       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "Timeout waiting for %s"), types);
//     }
//     if (!VL53L0X_xshut) { break; }
//   }
// #ifdef USE_DOMOTICZ
//   if (json && (0 == TasmotaGlobal.tele_period)){
//     float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
//     DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
//   }
// #endif  // USE_DOMOTICZ
// }

#ifdef USE_DEEPSLEEP

void VL53L0EnterStandby(void) {
  if (DeepSleepEnabled()) {
    for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
      if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        if (Vl53l0x_data[i].ready) {
          // VL53L0X_device[i].stopContinuous();
          // Calling stopContinuous() does not lead to a stable standby state.
          // The current is approx. 300 µA, but should be much lower.
          // Restart is bumpy and sometimes blocks the startup sequence completely.
          VL53L0X_device[i].init();
          Vl53l0x_data[i].ready = false;
        }
      }
    }
    VL53L0X_standby = true;
  }
}

#endif // USE_DEEPSLEEP

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mTOF_VL53L0X::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_SENSOR_COUNT, module_state.devices);
  return JsonBuilderI->End();

}

uint8_t mTOF_VL53L0X::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();



  return JsonBuilderI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mTOF_VL53L0X::MQTTHandler_Init(){

  struct handler<mTOF_VL53L0X>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTOF_VL53L0X::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mTOF_VL53L0X::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif// USE_MODULE_NETWORK_MQTT


#endif