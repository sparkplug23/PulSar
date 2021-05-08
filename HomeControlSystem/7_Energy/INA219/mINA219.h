#ifndef MSENSORSINA219_H
#define MSENSORSINA219_H 0.1

#define D_UNIQUE_MODULE_ENERGY_INA219_ID    132
#define D_GROUP_MODULE_ENERGY_INA219_ID      2

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_ENERGY_INA219

#include <Wire.h>
#include <SPI.h>

#include "1_TaskerManager/mTaskerManager.h"




#define TABLE_ROW_ITEMS_COUNT 5
DEFINE_PGM_CTR(PM_DLIM_LIST_TABLE_HEADERS_INA219)
"BusVoltage (mV)" "|" 
"ShuntVoltage (mV)" "|" 
"LoadVoltage (mV)" "|" 
"Current (mA)" "|" 
"Power (mW)";



/*=========================================================================
    CONFIG REGISTER (R/W)
**************************************************************************/

/** config register address **/
#define INA219_REG_CONFIG (0x00)

/** reset bit **/
#define INA219_CONFIG_RESET (0x8000) // Reset Bit

/** mask for bus voltage range **/
#define INA219_CONFIG_BVOLTAGERANGE_MASK (0x2000) // Bus Voltage Range Mask

/** bus voltage range values **/
enum {
  INA219_CONFIG_BVOLTAGERANGE_16V = (0x0000), // 0-16V Range
  INA219_CONFIG_BVOLTAGERANGE_32V = (0x2000), // 0-32V Range
};

/** mask for gain bits **/
#define INA219_CONFIG_GAIN_MASK (0x1800) // Gain Mask

/** values for gain bits **/
enum {
  INA219_CONFIG_GAIN_1_40MV = (0x0000),  // Gain 1, 40mV Range
  INA219_CONFIG_GAIN_2_80MV = (0x0800),  // Gain 2, 80mV Range
  INA219_CONFIG_GAIN_4_160MV = (0x1000), // Gain 4, 160mV Range
  INA219_CONFIG_GAIN_8_320MV = (0x1800), // Gain 8, 320mV Range
};

/** mask for bus ADC resolution bits **/
#define INA219_CONFIG_BADCRES_MASK (0x0780)

/** values for bus ADC resolution **/
enum {
  INA219_CONFIG_BADCRES_9BIT = (0x0000),  // 9-bit bus res = 0..511
  INA219_CONFIG_BADCRES_10BIT = (0x0080), // 10-bit bus res = 0..1023
  INA219_CONFIG_BADCRES_11BIT = (0x0100), // 11-bit bus res = 0..2047
  INA219_CONFIG_BADCRES_12BIT = (0x0180), // 12-bit bus res = 0..4097
  INA219_CONFIG_BADCRES_12BIT_2S_1060US =
      (0x0480), // 2 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_4S_2130US =
      (0x0500), // 4 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_8S_4260US =
      (0x0580), // 8 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_16S_8510US =
      (0x0600), // 16 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_32S_17MS =
      (0x0680), // 32 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_64S_34MS =
      (0x0700), // 64 x 12-bit bus samples averaged together
  INA219_CONFIG_BADCRES_12BIT_128S_69MS =
      (0x0780), // 128 x 12-bit bus samples averaged together

};

/** mask for shunt ADC resolution bits **/
#define INA219_CONFIG_SADCRES_MASK                                             \
  (0x0078) // Shunt ADC Resolution and Averaging Mask

/** values for shunt ADC resolution **/
enum {
  INA219_CONFIG_SADCRES_9BIT_1S_84US = (0x0000),   // 1 x 9-bit shunt sample
  INA219_CONFIG_SADCRES_10BIT_1S_148US = (0x0008), // 1 x 10-bit shunt sample
  INA219_CONFIG_SADCRES_11BIT_1S_276US = (0x0010), // 1 x 11-bit shunt sample
  INA219_CONFIG_SADCRES_12BIT_1S_532US = (0x0018), // 1 x 12-bit shunt sample
  INA219_CONFIG_SADCRES_12BIT_2S_1060US =
      (0x0048), // 2 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_4S_2130US =
      (0x0050), // 4 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_8S_4260US =
      (0x0058), // 8 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_16S_8510US =
      (0x0060), // 16 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_32S_17MS =
      (0x0068), // 32 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_64S_34MS =
      (0x0070), // 64 x 12-bit shunt samples averaged together
  INA219_CONFIG_SADCRES_12BIT_128S_69MS =
      (0x0078), // 128 x 12-bit shunt samples averaged together
};

/** mask for operating mode bits **/
#define INA219_CONFIG_MODE_MASK (0x0007) // Operating Mode Mask

/** values for operating mode **/
enum {
  INA219_CONFIG_MODE_POWERDOWN = 0x00,       /**< power down */
  INA219_CONFIG_MODE_SVOLT_TRIGGERED = 0x01, /**< shunt voltage triggered */
  INA219_CONFIG_MODE_BVOLT_TRIGGERED = 0x02, /**< bus voltage triggered */
  INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED =
      0x03,                         /**< shunt and bus voltage triggered */
  INA219_CONFIG_MODE_ADCOFF = 0x04, /**< ADC off */
  INA219_CONFIG_MODE_SVOLT_CONTINUOUS = 0x05, /**< shunt voltage continuous */
  INA219_CONFIG_MODE_BVOLT_CONTINUOUS = 0x06, /**< bus voltage continuous */
  INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS =
      0x07, /**< shunt and bus voltage continuous */
};

/** shunt voltage register **/
#define INA219_REG_SHUNTVOLTAGE (0x01)

/** bus voltage register **/
#define INA219_REG_BUSVOLTAGE (0x02)

/** power register **/
#define INA219_REG_POWER (0x03)

/** current register **/
#define INA219_REG_CURRENT (0x04)

/** calibration register **/
#define INA219_REG_CALIBRATION (0x05)


#include "1_TaskerManager/mTaskerInterface.h"

class mEnergyINA219 :
  public mTaskerInterface
{
  private:
  public:
    mEnergyINA219(){};
    void Pre_Init(void);
    void init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;// MODULE_STATUS_NOTSET;
      uint16_t measure_rate_ms = 1000;
    }settings;
    
    void AddToHardwareMessage();

    void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);

    bool SetCalibration(uint8_t mode, uint16_t addr);
    float GetShuntVoltage_mV(uint16_t addr);
    float GetBusVoltage_V(uint16_t addr);
    float GetBusVoltage_mV(uint16_t addr);
    float GetCurrent_mA(uint16_t addr);
    float GetPower_mW(uint16_t addr);

    void setCalibration_32V_2A();
    void setCalibration_32V_1A();
    void setCalibration_16V_400mA();

    uint8_t fWithinLimit;
    unsigned long tWithinLimit;

    TwoWire* Wire = nullptr;

    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 1
    void EveryLoop();


  uint32_t ina219_calValue;
  // The following multipliers are used to convert raw current and power
  // values to mA and mW, taking into account the current config settings
  uint32_t ina219_currentDivider_mA;
  float ina219_powerMultiplier_mW;
    // The following multiplier is used to convert shunt voltage (in mV) to current (in A)
    // Current_A = ShuntVoltage_mV / ShuntResistor_milliOhms = ShuntVoltage_mV * ina219_current_multiplier
    // ina219_current_multiplier = 1 / ShuntResistor_milliOhms
    float ina219_current_multiplier;

    // uint8_t ina219_valid[4] = {0,0,0,0};
    // float ina219_voltage[4] = {0,0,0,0};
    // float ina219_current[4] = {0,0,0,0};

    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();
    struct SENSORDATA{
      uint8_t isvalid=false;
      uint8_t ischanged=false;
      uint32_t tSavedMeasure;
      uint8_t sReadSensor;

      float bus_voltage_mv;
      float shunt_voltage_mv;
      float load_voltage_mv;
      float current_ma;
      float direct_current_ma;
      float power_mw;
      float direct_power_mw;
      
    }sensor[MAX_SENSORS];

    // Temp method, averaged
    struct SENSOR_AVERAGES{
      float bus_voltage_mv = 0;
    }sensor_averages[MAX_SENSORS];



    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mEnergyINA219>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mEnergyINA219> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mEnergyINA219> mqtthandler_sensor_ifchanged;
    struct handler<mEnergyINA219> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif


  
/*********************************************************************************************\
 * INA219 - Low voltage (max 32V!) Current sensor
 *
 * Source: Adafruit Industries
 *
 * I2C Address: 0x40, 0x41 0x44 or 0x45
\*********************************************************************************************/

#define XSNS_13                                 13
#define XI2C_14                                 14        // See I2CDEVICES.md

#define INA219_ADDRESS1                         (0x40)    // 1000000 (A0+A1=GND)
#define INA219_ADDRESS2                         (0x41)    // 1000000 (A0=Vcc, A1=GND)
#define INA219_ADDRESS3                         (0x44)    // 1000000 (A0=GND, A1=Vcc)
#define INA219_ADDRESS4                         (0x45)    // 1000000 (A0+A1=Vcc)

#define INA219_READ                             (0x01)
#define INA219_REG_CONFIG                       (0x00)

#define INA219_CONFIG_RESET                     (0x8000)  // Reset Bit

#define INA219_CONFIG_BVOLTAGERANGE_MASK        (0x2000)  // Bus Voltage Range Mask
#define INA219_CONFIG_BVOLTAGERANGE_16V         (0x0000)  // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V         (0x2000)  // 0-32V Range

#define INA219_CONFIG_GAIN_MASK                 (0x1800)  // Gain Mask
#define INA219_CONFIG_GAIN_1_40MV               (0x0000)  // Gain 1, 40mV Range
#define INA219_CONFIG_GAIN_2_80MV               (0x0800)  // Gain 2, 80mV Range
#define INA219_CONFIG_GAIN_4_160MV              (0x1000)  // Gain 4, 160mV Range
#define INA219_CONFIG_GAIN_8_320MV              (0x1800)  // Gain 8, 320mV Range

#define INA219_CONFIG_BADCRES_MASK              (0x0780)  // Bus ADC Resolution Mask
#define INA219_CONFIG_BADCRES_9BIT_1S_84US      (0x0<<7)  // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT_1S_148US    (0x1<<7)  // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT_1S_276US    (0x2<<7)  // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT_1S_532US    (0x3<<7)  // 12-bit bus res = 0..4097
#define INA219_CONFIG_BADCRES_12BIT_2S_1060US   (0x9<<7)  // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_4S_2130US   (0xA<<7)  // 4 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_8S_4260US   (0xB<<7)  // 8 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_16S_8510US  (0xC<<7)  // 16 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_32S_17MS    (0xD<<7)  // 32 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_64S_34MS    (0xE<<7)  // 64 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_128S_69MS   (0xF<<7)  // 128 x 12-bit bus samples averaged together

#define INA219_CONFIG_SADCRES_MASK              (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define INA219_CONFIG_SADCRES_9BIT_1S_84US      (0x0<<3)  // 1 x 9-bit shunt sample
#define INA219_CONFIG_SADCRES_10BIT_1S_148US    (0x1<<3)  // 1 x 10-bit shunt sample
#define INA219_CONFIG_SADCRES_11BIT_1S_276US    (0x2<<3)  // 1 x 11-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US    (0x3<<3)  // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US   (0x9<<3)  // 2 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US   (0xA<<3)  // 4 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US   (0xB<<3)  // 8 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US  (0xC<<3)  // 16 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS    (0xD<<3)  // 32 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS    (0xE<<3)  // 64 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS   (0xF<<3)  // 128 x 12-bit shunt samples averaged together

#define INA219_CONFIG_MODE_MASK                 (0x0007)  // Operating Mode Mask
#define INA219_CONFIG_MODE_POWERDOWN            (0x0000)
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED      (0x0001)
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED      (0x0002)
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define INA219_CONFIG_MODE_ADCOFF               (0x0004)
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS     (0x0005)
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS     (0x0006)
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)

#define INA219_REG_SHUNTVOLTAGE                 (0x01)
#define INA219_REG_BUSVOLTAGE                   (0x02)
#define INA219_REG_POWER                        (0x03)
#define INA219_REG_CURRENT                      (0x04)
#define INA219_REG_CALIBRATION                  (0x05)

#define INA219_DEFAULT_SHUNT_RESISTOR_MILLIOHMS (100.0) // 0.1 Ohm





};
#endif

#endif
