/**
 * @file mTOF_VL53L1X.cpp VL53L1X time of flight multiple sensors support
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2025
 */ 
/*********************************************************************************************\
 * VL53L1X time of flight sensor
 *
 * I2C Addres: 0x29
 *********************************************************************************************
 *
 * Note: When using multiple VL53L1X, it is required to also wire the XSHUT pin of all those sensors
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
 * How to install this sensor: https://www.st.com/resource/en/datasheet/vl53l1x.pdf
 *
 * If you are going to use long I2C wires read this:
 * https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/
 *
\*********************************************************************************************/

#include "mTOF_VL53L1X.h"

#ifdef USE_MODULE_SENSORS__TOF_VL53L1X

int8_t mTOF_VL53L1X::Tasker(uint8_t function, JsonParserObject obj){

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
      Loop();
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
      // pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}

void mTOF_VL53L1X::Loop()
{  
  // if(mTime::TimeReached(&tSaved_ReadSensor, 50)){
    // ReadSensor();
  // }
}

/**
 * @brief Pre-scans the I2C bus to detect active VL53L1X sensors before initialization.
 *
 * This function:
 *  - Scans for sensors at **default (0x29) and alternate (0x2A, 0x2B, ...) addresses**.
 *  - Stores the **number of detected sensors** to determine if we need **XSHUT toggling**.
 *  - Ensures only one sensor is enabled at a time if using XSHUT.
 */
void mTOF_VL53L1X::Pre_Init(void) 
{
  ALOG_INF(PSTR(D_LOG_TOF_VL53L1X "Pre-Init"));

  module_state.devices = 0;

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)) {
      ALOG_INF(PSTR(D_LOG_TOF_VL53L1X "Disable%d p%d"),i,tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID,i));
      pinMode(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), OUTPUT);
      digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, 0), LOW);
    }
  }

}

/**
 * @brief Initializes VL53L1X sensors after detecting them in `Pre_Init()`.
 *
 * This function:
 *  - Uses `module_state.devices` (set in `Pre_Init()`) to determine the number of sensors.
 *  - If **no XSHUT pins are defined**, runs **continuous mode** at `0x29`.
 *  - If **any XSHUT pin is defined**, runs **single-shot mode** at `0x29` (to avoid conflicts with VL53L0X).
 *  - If **multiple sensors**, assigns **alternate I2C addresses** and runs **single-shot mode**.
 * 
 * Needs made into repeatable functions.
 * 
 * 
 * If either TOF is used, and no XSHUT, then disable continuous mode (single-shot only) for both
 *  XSHUT pin is required to read
 * 
 * If XSHUT 1-8 is set, then alternative address is used (make this a function). 
 * for pins
 *  if pin
 *    change address
 * 
 * At this point, continuous or single shot method and ADDRESS is known, so init based on that mode
 *  Init_StartSingleShot
 *  Init_Continuous
 * 
 * ReadSensor will use switch, single/continuous mode. 
 *  Single will use XSHUT to control it
 * 
 * Split modes are possible, but not really any practical use for them.
 * 
 * Most probably mode will be single TOR, default 0x29, continuous mode. 
 * 
 * 
 * 
 * 
 * 
 * 
 */

 
/**
 * @brief Initializes VL53L1X sensors, handling address conflicts and multi-sensor setups.
 *
 * This function detects, configures, and initializes VL53L1X sensors based on four predefined setups:
 *
 * **Supported Setups:**
 *  1. **Setup 1: Single VL53L1X Sensor**
 *     - Only one VL53L1X is connected.
 *     - No address conflicts, operates at default **0x29**.
 *     - No XSHUT control is required.
 *
 *  2. **Setup 2: Single VL53L0X Sensor**
 *     - Only one VL53L0X is connected.
 *     - VL53L1X code is not active in this setup.
 *     - VL53L0X remains at default **0x29**.
 *
 *  3. **Setup 3: One VL53L1X + One VL53L0X**
 *     - Both VL53L0X and VL53L1X are detected.
 *     - **Conflict:** Both sensors start at **0x29**.
 *     - VL53L1X is **moved to `0x2A`** to resolve the conflict.
 *     - XSHUT is used once to switch the address but remains HIGH to retain the change.
 *
 *  4. **Setup 4: Multiple VL53L1X Sensors**
 *     - Only VL53L1X sensors are connected (up to 8).
 *     - The first sensor remains at **0x29**.
 *     - Additional sensors are assigned addresses **`0x2A`, `0x2B`, ..., `0x30`**.
 *     - XSHUT is required for each additional sensor to set and retain their addresses.
 *
 * **Function Workflow:**
 *  - Calls `SearchForDevices()` to detect VL53L1X sensors.
 *  - If VL53L0X is present, VL53L1X moves to **0x2A** (Setup 3).
 *  - If multiple VL53L1X sensors exist, assigns unique addresses using XSHUT (Setup 4).
 *  - Initializes each sensor, sets parameters, and starts continuous measurement.
 *  - Ensures all sensors enter `Continuous Mode` without requiring further toggling.
 *
 * @note XSHUT remains HIGH after setting addresses to prevent reset.
 */
void mTOF_VL53L1X::Init(void) 
{
  /**************************
  * Check for device on primary address, or secondary.
  **************************/
  uint8_t devices_found = SearchForDevices();
  ALOG_HGL(PSTR("devices_found %d"), devices_found);
  module_state.devices = devices_found;

  uint8_t tof1x_devices = 1;
  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  tof1x_devices = tkr_tof_vl0x->SearchForDevices();
  #endif

  // **Setup 3: VL53L0X + VL53L1X Conflict**
  bool setup_3 = (tof1x_devices == 1) && (devices_found == 1);
 
  /**************************
  * Sensor Initialization Loop
  **************************/
  uint32_t xshut_mask = 1;
  for (uint32_t i = 0; i < module_state.devices; i++, xshut_mask <<= 1) {
      bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i);

      if (use_xshut) {
          digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
          delay(2); // XSHUT boot delay
      }

      // Check if the sensor responds at 0x29
      if (!tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS)) {
          ALOG_INF(PSTR("VL53L1X[%d] not responding at 0x29"), i);
          continue;
      }

      ALOG_INF(PSTR("VL53L1X[%d] Init"), i);

      if (vl53l1x_device[i].init()) {
          ALOG_INF(PSTR("VL53L1X %d detected at 0x%02X"), i, VL53L1X_ADDRESS);

          // **Setup 3: Conflict Detected, Move VL53L1X to Secondary Address**
          if (setup_3 && i == 0) {
              uint8_t new_address = VL53L1X_XSHUT_ADDRESS;
              vl53l1x_device[i].setAddress(new_address);
              vl53l1x_data[i].address = new_address;
              ALOG_INF(PSTR("VL53L1X %d moved to 0x%02X to resolve conflict"), i, new_address);
          }

          // **Setup 4: Multiple Sensors, Assign Unique Addresses**
          else if (module_state.devices > 1 && use_xshut) {
              uint8_t new_address = VL53L1X_XSHUT_ADDRESS + i;
              vl53l1x_device[i].setAddress(new_address);
              vl53l1x_data[i].address = new_address;
              ALOG_INF(PSTR("VL53L1X %d assigned new address: 0x%02X"), i, new_address);
          } else {
              vl53l1x_data[i].address = VL53L1X_ADDRESS;
          }

          uint8_t addr = vl53l1x_device[i].getAddress();
          tkr_i2c->I2cSetActive(addr);

          // **Set Sensor Parameters**
          vl53l1x_device[i].setTimeout(500);
          vl53l1x_device[i].setDistanceMode(VL53L1X::VL53L1X_DISTANCE_MODE);
          vl53l1x_device[i].setMeasurementTimingBudget(140000);
          vl53l1x_device[i].startContinuous(50);

          VL53L1X_detected_bitmapped |= xshut_mask;

          ALOG_INF(PSTR("VL53L1X[%d] successfully initialized"), i);
      } else {
          ALOG_INF(PSTR("VL53L1X %d failed to initialize"), i);
      }
  }
 
  if (module_state.devices > 0) {
    module_state.mode = ModuleStatus::Running;
  }
  ALOG_HGL(PSTR("END OF INIT"));
}
 

// void mTOF_VL53L1X::Init(void) 
// {
//   /**************************
//    * Check for device on primary address, or secondary. Use single-shot bit toggling if required
//    **************************/
//   uint8_t devices_found = SearchForDevices();

//   ALOG_HGL(PSTR("devices_found %d"), devices_found);

//   module_state.devices = devices_found;

//   uint8_t tof1x_devices = 1;
//   #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
//   tof1x_devices = tkr_tof_vl0x->SearchForDevices();
//   #endif
  
//   if ((tof1x_devices == 1) && (devices_found == 1)) {
//     // Conflict: both VL53L0X and VL53L1X detected at 0x29
//     // Move VL53L1X device 0 to an alternate address
//     SwitchDeviceAddress(0, VL53L1X_XSHUT_ADDRESS);
//   }

//   // Now re-run SearchForDevices to confirm the change
//   devices_found = SearchForDevices();
//   ALOG_HGL(PSTR("Post-switch devices_found %d"), devices_found);


  
// ALOG_INF(PSTR("END OF PREINIT"));
// delay(5000);
  
//   if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, 0))
//   {
//     digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, 0), HIGH);
//   }
//   delay(10);

//   // Check if a sensor is at the default I2C address (0x29)
//   if (tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS)) {
//     ALOG_INF(PSTR("VL53L1X found at 0x29"));
//     module_state.devices++;
//   }

// // Check if sensors exist at alternate addresses (0x2A - 0x2F)
// for (uint8_t addr = VL53L1X_XSHUT_ADDRESS; addr < (VL53L1X_XSHUT_ADDRESS + VL53LXX_MAX_SENSORS); addr++) {
//     if (tkr_i2c->I2cSetDevice(addr)) {
//         ALOG_INF(PSTR("VL53L1X found at 0x%02X"), addr);
//         module_state.devices++;
//     }
// }

// // Log the number of detected sensors
// ALOG_INF(PSTR("Total VL53L1X sensors detected: %d"), module_state.devices);



//   ALOG_INF(PSTR(D_LOG_TOF_VL53L1X " Init"));

//   if (module_state.devices == 0) {
    
//   if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, 0))
//   {
//     digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, 0), LOW);
//   }


//       ALOG_INF(PSTR("No VL53L1X sensors detected, skipping initialization."));
//       return;
//   }

//   // Determine if any XSHUT pin is used
//   VL53L1X_xshut_enabled = false;
//   for (uint32_t i = 0; i < module_state.devices; i++) {
//       if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)) {
//         VL53L1X_xshut_enabled = true;
//           break;
//       }
//   }

//   for (uint32_t i = 0; i < module_state.devices; i++) {
//       bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i);

//       if (use_xshut) {
//           digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
//           delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME); // Allow boot
//       }

//       // Check if the sensor responds at the default address (0x29)
//       if (!tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS)) {
//           continue;
//       }

//       ALOG_INF(PSTR("VL53L1X[%d] Init"), i);

//       if (vl53l1x_device[i].init()) {
//           ALOG_INF(PSTR("VL53L1X %d detected"), i);

//           // **Assign new addresses only if multiple sensors exist**
//           if (module_state.devices > 1) {
//               uint8_t new_address = VL53L1X_XSHUT_ADDRESS + i;
//               vl53l1x_device[i].setAddress(new_address);
//               vl53l1x_data[i].address = new_address;
//               ALOG_INF(PSTR("VL53L1X %d assigned new address: 0x%02X"), i, new_address);
              
//               // **Re-initialize sensor after changing address**
//               if (!vl53l1x_device[i].init()) {
//                   ALOG_INF(PSTR("VL53L1X %d failed to re-init after address change"), i);
//                   continue;
//               }
//           } else {
//               // Single sensor case
//               vl53l1x_data[i].address = VL53L1X_ADDRESS;
//               ALOG_INF(PSTR("VL53L1X %d remains at default address (0x29)"), i);
//           }

//           uint8_t addr = vl53l1x_device[i].getAddress();
//           tkr_i2c->I2cSetActive(addr);

//           // Common settings for all sensors
//           vl53l1x_device[i].setTimeout(500);
//           vl53l1x_device[i].setDistanceMode(VL53L1X::VL53L1X_DISTANCE_MODE);
//           vl53l1x_device[i].setMeasurementTimingBudget(140000);

//           // **Determine whether to use continuous or single-shot mode**
//           if (module_state.devices == 1 && !VL53L1X_xshut_enabled) {
//               vl53l1x_device[i].startContinuous(50);
//               ALOG_INF(PSTR("VL53L1X %d: Using CONTINUOUS measurement mode (No XSHUT, No Conflict)"), i);
//           } else {
//               ALOG_INF(PSTR("VL53L1X %d: Using SINGLE measurement mode"), i);
//           }

//           vl53l1x_data[i].valid = true;
//           VL53L1X_detected_bitmapped |= (1 << i);
//       } else {
//           ALOG_INF(PSTR("VL53L1X %d failed to initialize"), i);
//       }

//       // If using XSHUT, disable the sensor after setup
//       if (use_xshut) {
//           digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), LOW);
//       }
//   }

//   if (module_state.devices > 0) {
//       module_state.mode = ModuleStatus::Running;
//   }

//   ALOG_HGL(PSTR("END OF INIT"));
// }



 /**
  * @brief Return if this sensor has been found
  * 
  * @return true 
  * @return false 
  */
 uint8_t mTOF_VL53L1X::SearchForDevices()
 {
   uint8_t devices = 0;
 
   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) 
   {
     // Enable device
     if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)){
       digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
       delay(2);
     }
 
     // Primary
     if(i==0)
     {
       if (tkr_i2c->I2cDevice_IsConnected(VL53L1X_ADDRESS)) {
         ALOG_INF(PSTR("VL53L1X found at 0x29"));
 
         vl53l1x_data[devices].address = VL53L1X_ADDRESS;
 
         devices++;
       }
     }
 
     if (tkr_i2c->I2cDevice_IsConnected(VL53L1X_XSHUT_ADDRESS+i)) {
       ALOG_INF(PSTR("VL53L1X found at %02X"),VL53L1X_XSHUT_ADDRESS+i);
       
       vl53l1x_data[devices].address = VL53L1X_XSHUT_ADDRESS+i;
 
       devices++;
     }
     
     // Disable device
     if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)){
       digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), LOW);
       delay(2);
     }
 
   }
 
   return devices;
 }


 /**
 * @brief Changes the I2C address of a VL53L1X sensor to avoid conflicts.
 *
 * This function:
 *  - Enables XSHUT (if available) to ensure sensor is active.
 *  - Initializes the sensor at default address `0x29` BEFORE switching.
 *  - Changes the device address using `setAddress()`.
 *  - Verifies the new address assignment.
 *  - Disables XSHUT (if needed) to free the I2C bus.
 *
 * @param device_id The index of the sensor in `vl53l1x_data[]`.
 * @param new_address The new I2C address to assign to the sensor.
 * @return `true` if the address change was successful, `false` otherwise.
 */
bool mTOF_VL53L1X::SwitchDeviceAddress(uint8_t device_id, uint8_t new_address) {
  ALOG_INF(PSTR("Switching VL53L1X[%d] to new address 0x%02X"), device_id, new_address);

  // Ensure device_id is valid
  if (device_id >= VL53LXX_MAX_SENSORS) {
      ALOG_INF(PSTR("Invalid device_id: %d"), device_id);
      return false;
  }

  bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, device_id);

  if (use_xshut) {
      ALOG_INF(PSTR("Enabling XSHUT for VL53L1X[%d]"), device_id);
      digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, device_id), HIGH);
      delay(2); // XSHUT boot delay
  }

  // Ensure the sensor is responding at its default address (0x29)
  if (!tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS)) {
      ALOG_INF(PSTR("VL53L1X[%d] not responding at 0x29"), device_id);
      return false;
  }

  // **Initialize the sensor at 0x29 BEFORE changing the address**
  if (!vl53l1x_device[device_id].init()) {
      ALOG_INF(PSTR("VL53L1X[%d] failed to initialize at 0x29"), device_id);
      return false;
  }

  // Set the new address
  vl53l1x_device[device_id].setAddress(new_address);
  vl53l1x_data[device_id].address = new_address;

  // Verify address change
  if (!tkr_i2c->I2cSetDevice(new_address)) {
      ALOG_INF(PSTR("VL53L1X[%d] failed to switch to 0x%02X"), device_id, new_address);
      return false;
  }

  ALOG_INF(PSTR("VL53L1X[%d] successfully switched to 0x%02X"), device_id, new_address);

  if (use_xshut) {
      digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, device_id), LOW);
  }

  return true;
}

 


/**
 * @brief Function should be made here that reads one sensor
 * Internally it will handle which mode, single/continuous
 * 
 */


/**
 * @brief Reads distance measurements from VL53L1X sensors.
 *
 * This function:
 *  - Uses `VL53L1X_xshut_enabled` to determine if **single-shot or continuous mode** is used.
 *  - In **continuous mode**, reads directly from the sensor.
 *  - In **single-shot mode**, enables XSHUT, takes a measurement, then disables XSHUT to free the I2C bus.
 */
void mTOF_VL53L1X::ReadSensor(void) {
  
  #ifdef USE_DEEPSLEEP
      if (VL53L1X_standby) return;
  #endif

  for (uint32_t i = 0; i < module_state.devices; i++) {
      // if (VL53L1X_detected_bitmapped & (1 << i)) {

          uint16_t dist = 0;

          // if (VL53L1X_xshut_enabled) {
          //     // **Single-shot mode: Enable XSHUT, read, then disable XSHUT**
          //     digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
          //     delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME); // Allow sensor boot-up

          //     // Start a single-shot measurement
          //     vl53l1x_device[i].setTimeout(500);
          //     vl53l1x_device[i].writeReg(VL53L1X::SYSTEM__MODE_START, 0x10); // **Correct way to start single measurement**

          //     // Wait for data to be ready
          //     uint32_t timeout = millis();
          //     while (!vl53l1x_device[i].dataReady()){ 
          //       if((millis()-timeout)>10) // max 100ms
          //       {
          //         return;
          //       }
          //       delay(1); 
          //     }

          //     // Read measurement
          //     dist = vl53l1x_device[i].read();

          //     // Clear interrupt for next measurement
          //     vl53l1x_device[i].writeReg(VL53L1X::SYSTEM__INTERRUPT_CLEAR, 0x01); // **Correct way to clear interrupt**

          //     // Disable sensor after reading
          //     digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), LOW);

          // } else {
              // **Continuous mode: Direct read**
              dist = vl53l1x_device[i].read();
          // }

          // Handle invalid distances
          if (!dist || dist > 4000) {
              dist = 9999;
          }

  #ifdef USE_VL_MEDIAN
          // Store in ring buffer
          vl53l1x_data[i].buffer[vl53l1x_data[i].index] = dist;
          vl53l1x_data[i].index++;
          if (vl53l1x_data[i].index >= USE_VL_MEDIAN_SIZE) {
              vl53l1x_data[i].index = 0;
          }

          // Sort list and take median
          uint16_t tbuff[USE_VL_MEDIAN_SIZE];
          memmove(tbuff, vl53l1x_data[i].buffer, sizeof(tbuff));
          std::sort(tbuff, tbuff + USE_VL_MEDIAN_SIZE);
          vl53l1x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE - 1) / 2];
  #else
          vl53l1x_data[i].distance = dist;
  #endif

          ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L1X %d - Distance: %d mm"), i+1, vl53l1x_data[i].distance);
      // }
  }
}

  



// /********************************************************************************************/
// void mTOF_VL53L1X::Pre_Init(void) { 
//   ALOG_INF(PSTR(D_LOG_TOF_VL53L1X "Pre_Init - Disabling all VL53L1X sensors"));


//   // pinMode(tkr_pins->Pin(GPIO_VL53L1X_XSHUT_ID, 0), OUTPUT);
//   // digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT_ID, 0), HIGH); // ENABLE
//   pinMode(26, OUTPUT);
//   digitalWrite(26, HIGH); // ENABLE


//   // #ifdef USE_DEBUGFEATURE__TOF_VL53L1X__TEMPORARY_FORCE_I2C_START
//   // // Wire.begin();
//   // // Wire.setClock(400000); // use 400 kHz I2C
//   // Wire = *tkr_i2c->wire;
//   // #endif
//   // ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO

//   // for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//   //   ALOG_INF(PSTR("VL53L1X %d pin%d - Disabling"), i, tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i)); Serial.flush();
//   //     if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)) {
//   //         pinMode(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), OUTPUT);
//   //         digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), LOW); // Disable VL53L1X
//   //     }
//   // }

//   ALOG_INF(PSTR("Pre_Init - All VL53L1X sensors disabled"));

// }

// // char mqtt_data[300];
// // tkr_i2c->I2cScan(mqtt_data, sizeof(mqtt_data));
// // ALOG_INF(PSTR("I2C Scan: %s"), mqtt_data);

// void mTOF_VL53L1X::Init(void) { 
//   ALOG_INF(PSTR(D_LOG_TOF_VL53L1X "Init"));

//   module_state.devices = 0;

//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//       if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)) {

//           ALOG_INF(PSTR("VL53L1X %d - Enabling"), i); Serial.flush();
          
//           // Enable the sensor
//           // digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
//           // delay(500); // Allow sensor bootup

//           // Check if the sensor is responding
//           if (!tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS) && !tkr_i2c->I2cSetDevice((uint8_t)(VL53L1X_XSHUT_ADDRESS+i))) 
//           {            
//             continue;
//           }

//           ALOG_INF(PSTR("vl53l1x_device[%d].init()"), i);  Serial.flush();

//           if (vl53l1x_device[i].init()) {
//               ALOG_INF(PSTR("VL53L1X %d detected"), i);

//               if (VL53L1X_xshut_bitmapped) {
//                   vl53l1x_device[i].setAddress((uint8_t)(VL53L1X_XSHUT_ADDRESS+i));
//               }

//               uint8_t addr = vl53l1x_device[i].getAddress();
//               ALOG_INF(PSTR("VL53L1X %d detected - New address 0x%02X"), i, addr);

//               tkr_i2c->I2cSetActive(addr);
//               AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L1X-%d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);

//               vl53l1x_device[i].setTimeout(500);
//               vl53l1x_device[i].setDistanceMode(VL53L1X::VL53L1X_DISTANCE_MODE); // Could be Short, Medium, Long
//               vl53l1x_device[i].setMeasurementTimingBudget(140000);
//               vl53l1x_device[i].startContinuous(50);

//               vl53l1x_data[i].valid = true;
//               VL53L1X_detected_bitmapped |= (1 << i);

//               module_state.devices++;

//           }else{
//               ALOG_INF(PSTR("VL53L1X %d failed to start"), i);

              
//               vl53l1x_device[i].setDistanceMode(VL53L1X::Long);
//               vl53l1x_device[i].setMeasurementTimingBudget(50000);
//               vl53l1x_device[i].startContinuous(50);
//               Serial.print(vl53l1x_device[i].read());
//               if (vl53l1x_device[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }




//           }
//       }

      

//       // Disable XSHUT after configuration to free up I2C
//       // digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), LOW);
//   }

//   if(module_state.devices)
//   {
//     module_state.mode = ModuleStatus::Running;
//   }

//   ALOG_HGL(PSTR("END OF INIT"));
// }

// void mTOF_VL53L1X::ReadSensor(void) {

//   #ifdef USE_DEEPSLEEP
//     if (VL53L1X_standby) return;
//   #endif
  
//     for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//       if (VL53L1X_detected_bitmapped & (1 << i)) {
        
//         // // Enable XSHUT before reading
//         // digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT_ID, i), HIGH);
//         // delay(5); // Allow sensor to stabilize
  
//         uint16_t dist = vl53l1x_device[i].read();
        
//         // Disable XSHUT after reading
//         // digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT_ID, i), LOW);
  
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L1X %d - " D_DISTANCE " %d mm"), i+1, dist);
  
//         // Handle invalid readings
//         if (!dist || dist > 4000) {
//           dist = 9999;
//         }
  
//   #ifdef USE_VL_MEDIAN
//         // Store in ring buffer
//         vl53l1x_data[i].buffer[vl53l1x_data[i].index] = dist;
//         vl53l1x_data[i].index++;
//         if (vl53l1x_data[i].index >= USE_VL_MEDIAN_SIZE) {
//           vl53l1x_data[i].index = 0;
//         }
  
//         // Sort list and take median
//         uint16_t tbuff[USE_VL_MEDIAN_SIZE];
//         memmove(tbuff, vl53l1x_data[i].buffer, sizeof(tbuff));
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
//         vl53l1x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE - 1) / 2];
//   #else
//         vl53l1x_data[i].distance = dist;
//   #endif
  
//         ALOG_INF(PSTR(D_LOG_I2C D_SENSOR " VL53L1X %d - " D_DISTANCE " %d mm"), i+1, vl53l1x_data[i].distance);
//       }
//     }
//   }
  
  
// void Vl53l0Show(boolean json) {
//   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
//     char types[12] = "VL53L1X";
//     if (VL53L1X_xshut) {
//       snprintf_P(types, sizeof(types), PSTR("VL53L1X%c%d"), IndexSeparator(), i +1);
//     }
//     if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L1X_xshut)) {
//       float distance = (VL53L1X_data[i].distance == 9999) ? NAN : (float)VL53L1X_data[i].distance / 10;  // cm
//       if (json) {
//         ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
// #ifdef USE_WEBSERVER
//       } else {
//         WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
// #endif
//       }
//     }
//     if (VL53L1X_device[i].timeoutOccurred()) {
//       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "Timeout waiting for %s"), types);
//     }
//     if (!VL53L1X_xshut) { break; }
//   }
// #ifdef USE_DOMOTICZ
//   if (json && (0 == TasmotaGlobal.tele_period)){
//     float distance = (float)VL53L1X_data[0].distance / 10;  // cm
//     DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
//   }
// #endif  // USE_DOMOTICZ
// }

#ifdef USE_DEEPSLEEP

void VL53L0EnterStandby(void) {
  if (DeepSleepEnabled()) {
    for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
      if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L1X_xshut)) {
        if (VL53L1X_data[i].ready) {
          // VL53L1X_device[i].stopContinuous();
          // Calling stopContinuous() does not lead to a stable standby state.
          // The current is approx. 300 µA, but should be much lower.
          // Restart is bumpy and sometimes blocks the startup sequence completely.
          VL53L1X_device[i].init();
          VL53L1X_data[i].ready = false;
        }
      }
    }
    VL53L1X_standby = true;
  }
}

#endif // USE_DEEPSLEEP

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mTOF_VL53L1X::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_SENSOR_COUNT, module_state.devices);
  return JsonBuilderI->End();

}

uint8_t mTOF_VL53L1X::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[100];

  JsonBuilderI->Start();

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) 
  {
    if(bitRead(VL53L1X_detected_bitmapped, i))
    {
      // JBI->Add(D_SENSOR, 
      DLI->GetDeviceName_WithModuleUniqueID(D_UNIQUE_MODULE_SENSORS__TOF_VL53L1X__ID, i, buffer, sizeof(buffer));
      JBI->Object_Start(buffer);

      float distance = (vl53l1x_data[i].distance == 9999) ? NAN : (float)vl53l1x_data[i].distance / 10;  // cm
      JBI->Add(D_DISTANCE, distance);
      JBI->Add(D_DISTANCE "_mm", vl53l1x_data[i].distance);

      JBI->Object_End();

    }
  }

  return JsonBuilderI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mTOF_VL53L1X::MQTTHandler_Init(){

  struct handler<mTOF_VL53L1X>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTOF_VL53L1X::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mTOF_VL53L1X::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif// USE_MODULE_NETWORK_MQTT


#endif