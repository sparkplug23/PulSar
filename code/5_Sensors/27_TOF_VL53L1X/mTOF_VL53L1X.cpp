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
 * @brief Prepares the VL53L1X sensors before initialization.
 *
 * This function:
 *  - Scans for active VL53L1X sensors before initialization.
 *  - Disables all sensors by setting **XSHUT LOW** (if applicable).
 *  - Ensures sensors are powered off to prevent I2C conflicts during initialization.
 *
 * **Processing Logic:**
 *  - Iterates through **all possible VL53L1X sensors**.
 *  - If an XSHUT pin is detected, it is **set LOW** to disable the sensor.
 *
 * @note This function is always executed **before Init()**, ensuring a clean startup.
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
 * @brief Initializes VL53L1X sensors, handling conflicts and multiple devices.
 *
 * This function:
 *  - Detects available VL53L1X sensors using `SearchForDevices()`.
 *  - **Filters out VL53L0X's already assigned address** (`0x78`) to prevent double counting.
 *  - Assigns **new addresses** when multiple VL53L1X sensors are present.
 *  - Initializes each sensor and **sets continuous measurement mode**.
 *
 * **Processing Logic:**
 *  - Calls `SearchForDevices()` to detect all VL53L1X sensors.
 *  - Calls `tkr_tof_vl0x->SearchForDevices()` to check for VL53L0X.
 *  - **Setup 2 (Single VL53L1X):**  
 *    - If **only one sensor exists**, it stays at `0x29`.
 *  - **Setup 4 (Multiple VL53L1X Sensors):**  
 *    - Each sensor gets a **unique alternate address** (`0x2A`, `0x2B`, etc.).
 *  - Initializes each sensor with **optimized settings** (`LONG_RANGE`, `HIGH_SPEED`, etc.).
 *  - Starts each sensor in **continuous measurement mode**.
 *
 * @note If XSHUT is used, it remains **HIGH** after switching addresses to prevent reset.
 */
void mTOF_VL53L1X::Init(void) 
{
  
  /**************************
  * Detect Sensors Using SearchForDevices()
  **************************/
  uint8_t devices_found = SearchForDevices();
  ALOG_HGL(PSTR("devices_found %d"), devices_found);
  module_state.devices = devices_found;

  /**************************
  * Exclude VL53L0X's Already Assigned Address (0x78)
  **************************/
  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  if (tkr_tof_vl0x->module_state.devices == 1 && devices_found > 0) {
      ALOG_INF(PSTR("Adjusting devices_found to exclude VL53L0X alternate address"));
      devices_found -= 1; // Remove VL53L0X's already assigned alternate address
      module_state.devices = devices_found;
  }
  #endif

  ALOG_HGL(PSTR("devices_found after filtering: %d"), devices_found);


  if (module_state.devices == 0) {
      ALOG_INF(PSTR("No VL53L1X sensors detected, skipping initialization."));
      return;
  }

  /**************************
  * Sensor Initialization Loop
  **************************/
  uint32_t xshut_mask = 1;
  for (uint32_t i = 0; i < module_state.devices; i++, xshut_mask <<= 1) {
      bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i);

      if (use_xshut) {
          digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
          delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME); // XSHUT boot delay
      }

      // Check if the sensor responds at 0x29
      if (!tkr_i2c->I2cSetDevice(VL53L1X_ADDRESS)) {
          ALOG_INF(PSTR("VL53L1X[%d] not responding at 0x29"), i);
          continue;
      }

      ALOG_INF(PSTR("VL53L1X[%d] Init"), i);

      if (vl53l1x_device[i].init()) {
          ALOG_INF(PSTR("VL53L1X %d detected at 0x%02X"), i, VL53L1X_ADDRESS);

          // **Setup 4: Multiple Sensors, Assign Unique Addresses**
          if (module_state.devices > 1 && use_xshut) {
              uint8_t new_address = VL53L1X_XSHUT_ADDRESS + i;
              vl53l1x_device[i].setAddress(new_address);
              vl53l1x_data[i].address = new_address;
              ALOG_INF(PSTR("VL53L1X %d assigned new address: 0x%02X"), i, new_address);
          } else {
              vl53l1x_data[i].address = VL53L1X_ADDRESS;
              ALOG_INF(PSTR("VL53L1X %d remains at default address (0x29)"), i);
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


/**
 * @brief Reads distance measurements from VL53L1X sensors.
 *
 * This function:
 *  - Iterates through all **active VL53L1X sensors** using **bitmask-based detection**.
 *  - Reads the **continuous distance measurement** from each sensor.
 *  - Applies an optional **median filter** (`USE_VL_MEDIAN`) to improve accuracy.
 *  - Handles **invalid readings** by setting distances over **4000 mm** to `9999`.
 *
 * **Processing Logic:**
 *  - **Bitmasking (`VL53L1X_detected_bitmapped`)** ensures efficient sensor tracking.
 *  - **Continuous mode reading:**  
 *    - Each sensor reads `read()`.
 *  - **Handles invalid readings:**  
 *    - If **`0` or `> 4000 mm`**, distance is set to `9999`.
 *  - **Median filtering (Optional):**  
 *    - Stores values in a **ring buffer**.
 *    - Sorts the buffer and selects the **median value**.
 *  - Exits early if **no XSHUT-controlled sensors are left** (`VL53L1X_xshut_bitmapped`).
 *
 * @note If median filtering is disabled, the **raw distance value** is stored.
 */
void mTOF_VL53L1X::ReadSensor(void) {
  
  #ifdef USE_DEEPSLEEP
      if (VL53L1X_standby) return;
  #endif

  int32_t i, xshut;
  for (i = 0, xshut = 1; i < VL53LXX_MAX_SENSORS; i++, xshut <<= 1) {
    if (xshut & VL53L1X_detected_bitmapped) {
      
      uint16_t dist = vl53l1x_device[i].read();

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

      ALOG_INF(PSTR("VL53L1X[%d] Distance: %d mm"), i, vl53l1x_data[i].distance);
    } // if detected

    // Exit loop early if no XSHUT-based devices remain
    if (0 == VL53L1X_xshut_bitmapped) break;
  } // for
}


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

/**
 * @brief Scans for VL53L1X sensors on the I2C bus.
 *
 * This function:
 *  - Enables each sensor (if XSHUT is available).
 *  - Checks for sensors at **default (`0x29`) and alternate addresses (`0x2A-0x2F`)**.
 *  - Stores detected devices and their addresses.
 *
 * **Processing Logic:**
 *  - Iterates through **all possible VL53L1X sensors**.
 *  - **Setup 2 (Single VL53L1X):**  
 *    - If a sensor is found at `0x29`, it is added to the list.
 *  - **Setup 4 (Multiple VL53L1X Sensors):**  
 *    - Sensors at alternate addresses (`0x2A-0x2F`) are added.
 *  - If **XSHUT is available**, it **enables** and **disables** sensors during scanning.
 *
 * @return The number of detected VL53L1X sensors.
 */
 uint8_t mTOF_VL53L1X::SearchForDevices()
 {
   uint8_t devices = 0;
 
   for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) 
   {
     // Enable device
     if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)){
       digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
       delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME);
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
       delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME);
     }
 
   }
 
   return devices;
 }


/**
 * @brief Changes the I2C address of a VL53L1X sensor to avoid conflicts.
 *
 * This function:
 *  - Enables **XSHUT** (if available) to ensure the sensor is active.
 *  - Initializes the sensor at its default address **before switching**.
 *  - Changes the device address using `setAddress()`.
 *  - Verifies the new address assignment.
 *  - Disables XSHUT (if needed) to free the I2C bus.
 *
 * **Processing Logic:**
 *  - Checks if the given `device_id` is valid.
 *  - If an **XSHUT pin is available**, it **enables the sensor** before switching.
 *  - **Ensures the sensor responds** at `0x29` before attempting an address change.
 *  - **Re-initializes** the sensor **before switching to the new address**.
 *  - Changes the I2C address using `setAddress()`.
 *  - **Verifies the change** by ensuring the sensor now responds at the new address.
 *  - **Disables XSHUT** (if applicable) after the switch.
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
      delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME); // XSHUT boot delay
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