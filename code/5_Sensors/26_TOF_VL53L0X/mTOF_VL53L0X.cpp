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
        // pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
      break;
      case TASK_MQTT_SENDER:
        pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
      break;
      #endif //USE_MODULE_NETWORK_MQTT
    }

  return FUNCTION_RESULT_SUCCESS_ID;

}


/**
 * @brief Prepares the VL53L0X sensors before initialization.
 *
 * This function:
 *  - Scans for active VL53L0X sensors before initialization.
 *  - Disables all sensors by setting **XSHUT LOW** (if applicable).
 *  - Ensures sensors are powered off to prevent I2C conflicts during initialization.
 *
 * **Processing Logic:**
 *  - Iterates through **all possible VL53L0X sensors**.
 *  - If an XSHUT pin is detected, it is **set LOW** to disable the sensor.
 *  - The **XSHUT state is stored** (`VL53L0X_xshut = true`) if any XSHUT is found.
 *
 * @note This function is always executed **before Init()**, ensuring a clean startup.
 */
void mTOF_VL53L0X::Pre_Init(void) 
{
  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Pre-Init - Scanning I2C Bus"));

  module_state.devices = 0;

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, i)) {
      ALOG_INF(PSTR(D_LOG_TOF_VL53L0X "Disable%d p%d"),i,tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID,i));
      pinMode(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), OUTPUT);
      digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, 0), LOW);
      VL53L0X_xshut = true;
    }
  }

}


/**
 * @brief Initializes VL53L0X sensors, handling conflicts and multiple devices.
 *
 * This function:
 *  - Detects available VL53L0X sensors using `SearchForDevices()`.
 *  - Checks if **VL53L1X exists** to avoid conflicts on the I2C bus.
 *  - Assigns **new addresses** when multiple VL53L0X sensors are present.
 *  - Initializes each sensor and **sets continuous measurement mode**.
 *
 * **Processing Logic:**
 *  - Calls `SearchForDevices()` to detect all VL53L0X sensors.
 *  - Calls `tkr_tof_vl1x->SearchForDevices()` to check for VL53L1X.
 *  - **Setup 2 (Single VL53L0X):**  
 *    - If **only one sensor exists**, it stays at `0x29`.
 *  - **Setup 3 (VL53L0X + VL53L1X Conflict):**  
 *    - If **VL53L1X is present**, **VL53L0X moves to `0x78`** to prevent conflicts.
 *  - **Setup 4 (Multiple VL53L0X Sensors):**  
 *    - Each sensor gets a **unique alternate address** (`0x2A`, `0x2B`, etc.).
 *  - Initializes each sensor with **optimized settings** (`LONG_RANGE`, `HIGH_SPEED`, etc.).
 *  - Starts each sensor in **continuous measurement mode**.
 *
 * @note If XSHUT is used, it remains **HIGH** after switching addresses to prevent reset.
 */
void mTOF_VL53L0X::Init(void) 
{

  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X " Init"));
  /**************************
   * Detect Sensors Using SearchForDevices()
   **************************/
  uint8_t devices_found = SearchForDevices();
  ALOG_HGL(PSTR("devices_found %d"), devices_found);
  module_state.devices = devices_found;
  
  if (module_state.devices == 0) {
    ALOG_INF(PSTR("No VL53L0X sensors detected, skipping initialization."));
    return;
  }

  /**************************
   * Check if VL53L1X Exists
   **************************/
  uint8_t tof1x_devices = 0;
  #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  tof1x_devices = tkr_tof_vl1x->SearchForDevices();
  ALOG_INF(PSTR(D_LOG_TOF_VL53L0X "tof1x_devices already has %d"), tof1x_devices);
  #endif

  /**************************
   * Sensor Initialization Loop
   **************************/
  for (uint32_t i = 0; i < module_state.devices; i++) {
    bool use_xshut = tkr_pins->PinUsed(GPIO_VL53L0X_XSHUT1_ID, i);

    if (use_xshut) {
      digitalWrite(tkr_pins->Pin(GPIO_VL53L0X_XSHUT1_ID, i), HIGH);
      delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME); // XSHUT boot delay
    }

    ALOG_INF(PSTR("VL53L0X[%d] Init"), i);

    // **Check sensor at 0x29 or alternate address**
    if (!tkr_i2c->I2cSetDevice(VL53L0X_ADDRESS) && !tkr_i2c->I2cSetDevice((uint8_t)(VL53L0X_XSHUT_ADDRESS + i))) {
      ALOG_INF(PSTR("VL53L0X[%d] not detected"), i);
      continue;
    }

    if (VL53L0X_device[i].init()) {
      ALOG_INF(PSTR("VL53L0X %d detected"), i);

      // **Setup 3 (VL53L0X + VL53L1X Conflict)**
      if (tof1x_devices == 1 && devices_found == 1) {
        uint8_t new_address = VL53L0X_XSHUT_ADDRESS;
        VL53L0X_device[i].setAddress(new_address);
        Vl53l0x_data[i].address = new_address;
        ALOG_INF(PSTR("VL53L0X %d moved to 0x%02X to resolve conflict"), i, new_address);
      }

      // **Setup 4: Multiple Sensors, Assign Unique Addresses**
      else if (module_state.devices > 1 && use_xshut) {
        uint8_t new_address = VL53L0X_XSHUT_ADDRESS + i;
        VL53L0X_device[i].setAddress(new_address);
        Vl53l0x_data[i].address = new_address;
        ALOG_INF(PSTR("VL53L0X %d assigned new address: 0x%02X"), i, new_address);
      } else {
        Vl53l0x_data[i].address = VL53L0X_ADDRESS;
        ALOG_INF(PSTR("VL53L0X %d remains at default address (0x29)"), i);
      }

      uint8_t addr = VL53L0X_device[i].getAddress();
      tkr_i2c->I2cSetActive(addr);

      // **Apply VL53L0X-Specific Settings**
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

      // **Start Continuous Mode**
      VL53L0X_device[i].startContinuous();
      ALOG_INF(PSTR("VL53L0X[%d] started continuous mode"), i);

      Vl53l0x_data[i].ready = true;
      Vl53l0x_data[i].index = 0;
      VL53L0X_detected = true;
    } else {
        ALOG_INF(PSTR("VL53L0X[%d] failed to start"), i);
    }

    // **If using XSHUT, keep it HIGH to retain address**
    if (use_xshut) {
        ALOG_INF(PSTR("VL53L0X[%d] XSHUT HIGH to retain address"), i);
    }
  }

  if (module_state.devices > 0) {
      module_state.mode = ModuleStatus::Running;
  }

  ALOG_HGL(PSTR("END OF INIT"));
}


/**
 * @brief Scans for VL53L0X sensors on the I2C bus.
 *
 * This function:
 *  - Enables each sensor (if XSHUT is available).
 *  - Checks for sensors at **default (`0x29`) and alternate addresses (`0x2A-0x2F`)**.
 *  - Stores detected devices and their addresses.
 *
 * **Processing Logic:**
 *  - Iterates through **all possible VL53L0X sensors**.
 *  - **Setup 2 (Single VL53L0X):**  
 *    - If a sensor is found at `0x29`, it is added to the list.
 *  - **Setup 4 (Multiple VL53L0X Sensors):**  
 *    - Sensors at alternate addresses (`0x2A-0x2F`) are added.
 *  - If **XSHUT is available**, it **enables** and **disables** sensors during scanning.
 *
 * @return The number of detected VL53L0X sensors.
 */
uint8_t mTOF_VL53L0X::SearchForDevices()
{
  uint8_t devices = 0;

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) 
  {
    // Enable device
    if (tkr_pins->PinUsed(GPIO_VL53L1X_XSHUT1_ID, i)){
      digitalWrite(tkr_pins->Pin(GPIO_VL53L1X_XSHUT1_ID, i), HIGH);
      delay(XSHUT_SET_HIGH_BOOT_UNTIL_VALID_DATA_WAKE_TIME);
    }

    uint8_t address = VL53L0X_ADDRESS;

    // Primary
    if(i==0)
    {
      if (tkr_i2c->I2cDevice_IsConnected(address)) {
        ALOG_INF(PSTR("VL53L1X found at 0x%02X"), address); 
        Vl53l0x_data[devices].address = address; 
        devices++;
      }
    }

    address = VL53L0X_XSHUT_ADDRESS+i;
    if (tkr_i2c->I2cDevice_IsConnected(address)) {
      ALOG_INF(PSTR("VL53L1X found at %02X"),address);       
      Vl53l0x_data[devices].address = address; 
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
 * @brief Reads distance measurements from VL53L0X sensors.
 *
 * This function:
 *  - Iterates through all **active VL53L0X sensors** using **bitmask-based detection**.
 *  - Reads the **continuous distance measurement** from each sensor.
 *  - Applies an optional **median filter** (`USE_VL_MEDIAN`) to improve accuracy.
 *  - Handles **invalid readings** by setting distances over **2200 mm** to `9999`.
 *
 * **Processing Logic:**
 *  - **Bitmasking (`VL53L0X_detected_bitmapped`)** ensures efficient sensor tracking.
 *  - **Continuous mode reading:**  
 *    - Each sensor reads `readRangeContinuousMillimeters()`.
 *  - **Handles invalid readings:**  
 *    - If **`0` or `> 2200 mm`**, distance is set to `9999`.
 *  - **Median filtering (Optional):**  
 *    - Stores values in a **ring buffer**.
 *    - Sorts the buffer and selects the **median value**.
 *  - Exits early if **no XSHUT-controlled sensors are left** (`VL53L0X_xshut_bitmapped`).
 *
 * @note If median filtering is disabled, the **raw distance value** is stored.
 */
void mTOF_VL53L0X::ReadSensor(void) {

  #ifdef USE_DEEPSLEEP
    if (VL53L0X_standby) return;
  #endif

  uint32_t i, xshut;
  for (i = 0, xshut = 1; i < VL53LXX_MAX_SENSORS; i++, xshut <<= 1) {
    if (xshut & VL53L0X_detected) {

      // **Read distance in continuous mode**
      uint16_t dist = VL53L0X_device[i].readRangeContinuousMillimeters();

      // **Handle invalid readings**
      if ((0 == dist) || (dist > 2200)) {
          dist = 9999;
      }

      #ifdef USE_VL_MEDIAN
        // Store in ring buffer
        Vl53l0x_data[i].buffer[Vl53l0x_data[i].index] = dist;
        Vl53l0x_data[i].index++;
        if (Vl53l0x_data[i].index >= USE_VL_MEDIAN_SIZE) {
            Vl53l0x_data[i].index = 0;
        }

        // Sort list and take median
        uint16_t tbuff[USE_VL_MEDIAN_SIZE];
        memmove(tbuff, Vl53l0x_data[i].buffer, sizeof(tbuff));
        std::sort(tbuff, tbuff + USE_VL_MEDIAN_SIZE);
        Vl53l0x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE - 1) / 2];
      #else
        Vl53l0x_data[i].distance = dist;
      #endif

      ALOG_INF(PSTR("VL53L0X[%d] Distance: %d mm"), i, Vl53l0x_data[i].distance);
    }

    // **Exit loop early if no XSHUT-based devices remain**
    if (0 == VL53L0X_xshut) break;
  }

}




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
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mTOF_VL53L0X::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif// USE_MODULE_NETWORK_MQTT


#endif