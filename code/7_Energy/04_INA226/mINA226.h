#ifndef MSENSORSINA226_H
#define MSENSORSINA226_H

#define D_UNIQUE_MODULE_ENERGY_INA226_ID 7004 // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_ENERGY_INA226

#include <Wire.h>
#include "1_TaskerManager/mTaskerInterface.h"

#ifndef D_LOG_INA226
#define D_LOG_INA226 "INA226 "
#endif

#ifndef D_MODULE_ENERGY_INA226_CTR
#define D_MODULE_ENERGY_INA226_CTR "INA226"
#endif

class mEnergyINA226 :
  public mTaskerInterface
{
  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     * Date Modified: 06Jun26
     ************************************************************************************************/
    mEnergyINA226(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();

    static constexpr const char* PM_MODULE_ENERGY_INA226_CTR = D_MODULE_ENERGY_INA226_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_ENERGY_INA226_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_INA226_ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    } module_state;

    /************************************************************************************************
     * SECTION: INA226 Register Definitions
     * Date Modified: 06Jun26
     ************************************************************************************************/

    static constexpr uint8_t INA226_ADDRESS1         = 0x40;
    static constexpr uint8_t INA226_ADDRESS2         = 0x41;
    static constexpr uint8_t INA226_ADDRESS3         = 0x44;
    static constexpr uint8_t INA226_ADDRESS4         = 0x45;

    static constexpr uint8_t INA226_REG_CONFIG       = 0x00;
    static constexpr uint8_t INA226_REG_SHUNT        = 0x01;
    static constexpr uint8_t INA226_REG_BUS          = 0x02;
    static constexpr uint8_t INA226_REG_POWER        = 0x03;
    static constexpr uint8_t INA226_REG_CURRENT      = 0x04;
    static constexpr uint8_t INA226_REG_CALIBRATION  = 0x05;
    static constexpr uint8_t INA226_REG_MASK_ENABLE  = 0x06;
    static constexpr uint8_t INA226_REG_ALERT_LIMIT  = 0x07;
    static constexpr uint8_t INA226_REG_MAN_ID       = 0xFE;
    static constexpr uint8_t INA226_REG_DIE_ID       = 0xFF;

    static constexpr uint16_t INA226_CONFIG_RESET    = 0x8000;
    static constexpr uint16_t INA226_MASK_AFF        = 0x0010;
    static constexpr uint16_t INA226_MASK_CVRF       = 0x0008;
    static constexpr uint16_t INA226_MASK_OVF        = 0x0004;
    static constexpr uint16_t INA226_MASK_ALERT_POL  = 0x0002;
    static constexpr uint16_t INA226_MASK_LATCH_EN   = 0x0001;

    enum INA226_Averages
    {
      INA226_AVERAGE_1       = 0x0000,
      INA226_AVERAGE_4       = 0x0200,
      INA226_AVERAGE_16      = 0x0400,
      INA226_AVERAGE_64      = 0x0600,
      INA226_AVERAGE_128     = 0x0800,
      INA226_AVERAGE_256     = 0x0A00,
      INA226_AVERAGE_512     = 0x0C00,
      INA226_AVERAGE_1024    = 0x0E00
    };

    enum INA226_ConversionTime
    {
      INA226_CONV_TIME_140   = 0x00,
      INA226_CONV_TIME_204   = 0x01,
      INA226_CONV_TIME_332   = 0x02,
      INA226_CONV_TIME_588   = 0x03,
      INA226_CONV_TIME_1100  = 0x04,
      INA226_CONV_TIME_2116  = 0x05,
      INA226_CONV_TIME_4156  = 0x06,
      INA226_CONV_TIME_8244  = 0x07
    };

    enum INA226_MeasureMode
    {
      INA226_POWER_DOWN                = 0x00,
      INA226_TRIGGERED_CURRENT_ONLY    = 0x01,
      INA226_TRIGGERED_BUS_ONLY        = 0x02,
      INA226_TRIGGERED                 = 0x03,
      INA226_POWER_DOWN_2              = 0x04,
      INA226_CONTINUOUS_CURRENT_ONLY   = 0x05,
      INA226_CONTINUOUS_BUS_ONLY       = 0x06,
      INA226_CONTINUOUS                = 0x07
    };

    enum INA226_AlertType
    {
      INA226_SHUNT_OVER    = 0x8000,
      INA226_SHUNT_UNDER   = 0x4000,
      INA226_BUS_OVER      = 0x2000,
      INA226_BUS_UNDER     = 0x1000,
      INA226_POWER_OVER    = 0x0800,
      INA226_CURRENT_OVER  = 0xFFFE,
      INA226_CURRENT_UNDER = 0xFFFF
    };

    #define INA226_DEFAULT_SHUNT_RESISTOR_OHMS (0.100f)
    #define INA226_DEFAULT_CURRENT_RANGE_A      (-1.0f)   // -1 = use full shunt range, 81.9175 mV / Rshunt
    #define INA226_SHUNT_LSB_MV                 (0.0025f) // 2.5 uV
    #define INA226_BUS_LSB_V                    (0.00125f)
    #define MAX_SENSORS                         4

    const uint8_t ina226_addresses[4] = { INA226_ADDRESS1, INA226_ADDRESS2, INA226_ADDRESS3, INA226_ADDRESS4 };

    /************************************************************************************************
     * SECTION: Runtime Data
     * Date Modified: 06Jun26
     ************************************************************************************************/

    struct sensor_data_s
    {
      uint8_t address = 0;
      uint8_t isvalid = false;
      uint8_t ischanged = false;
      uint32_t tSavedMeasure = 0;

      float bus_voltage_v = 0.0f;
      float bus_voltage_mv = 0.0f;
      float shunt_voltage_mv = 0.0f;
      float load_voltage_mv = 0.0f;
      float current_ma = 0.0f;
      float current_a = 0.0f;
      float power_mw = 0.0f;
      float power_w = 0.0f;

      bool invert_reported_sign = false;

      uint16_t cal_value = 2048;
      uint16_t config_copy = 0;
      float correction_factor = 1.0f;
      float current_divider_ma = 40.0f;
      float power_multiplier_mw = 0.625f;
      bool overflow = false;
      bool conversion_ready = false;
      bool limit_alert = false;
    };
    std::vector<sensor_data_s> sensor;

    /************************************************************************************************
     * SECTION: Internal Functions
     * Date Modified: 06Jun26
     ************************************************************************************************/

    void ReadSensor();

    bool ConfigureDevice(uint8_t sensor_id);
    bool Reset(uint8_t sensor_id);
    bool SetAverage(uint8_t sensor_id, INA226_Averages averages);
    bool SetConversionTime(uint8_t sensor_id, INA226_ConversionTime conv_time);
    bool SetConversionTime(uint8_t sensor_id, INA226_ConversionTime shunt_conv_time, INA226_ConversionTime bus_conv_time);
    bool SetMeasureMode(uint8_t sensor_id, INA226_MeasureMode mode);
    bool SetResistorRange(uint8_t sensor_id, float resistor_ohms, float current_range_a = INA226_DEFAULT_CURRENT_RANGE_A);
    bool SetCorrectionFactor(uint8_t sensor_id, float correction_factor);

    float GetShuntVoltage_mV(uint8_t sensor_id);
    float GetBusVoltage_V(uint8_t sensor_id);
    float GetBusVoltage_mV(uint8_t sensor_id);
    float GetCurrent_mA(uint8_t sensor_id);
    float GetCurrent_A(uint8_t sensor_id);
    float GetPower_mW(uint8_t sensor_id);
    float GetPower_W(uint8_t sensor_id);

    bool StartSingleMeasurement(uint8_t sensor_id, bool wait_for_completion = true);
    bool IsBusy(uint8_t sensor_id);
    void WaitUntilConversionCompleted(uint8_t sensor_id);
    bool PowerDown(uint8_t sensor_id);
    bool PowerUp(uint8_t sensor_id);

    bool SetAlertPinActiveHigh(uint8_t sensor_id);
    bool EnableAlertLatch(uint8_t sensor_id);
    bool EnableConvReadyAlert(uint8_t sensor_id);
    bool SetAlertType(uint8_t sensor_id, INA226_AlertType type, float limit);
    void ReadAndClearFlags(uint8_t sensor_id);

    uint16_t ReadRegister(uint8_t sensor_id, uint8_t reg);
    int16_t ReadRegisterS16(uint8_t sensor_id, uint8_t reg);
    bool WriteRegister(uint8_t sensor_id, uint8_t reg, uint16_t value);

    /************************************************************************************************
     * SECTION: Unified Reporting
     * Date Modified: 06Jun26
     ************************************************************************************************/

    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }

    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index >= sensor.size()) { value->sensor_type.push_back(0); return; }
      value->sensor_type.push_back(SENSOR_TYPE_VOLTAGE_ID);       value->data_f.push_back(sensor[index].bus_voltage_v);
      value->sensor_type.push_back(SENSOR_TYPE_CURRENT_ID);       value->data_f.push_back(sensor[index].current_a);
      value->sensor_type.push_back(SENSOR_TYPE_ACTIVE_POWER_ID);  value->data_f.push_back(sensor[index].power_w);
      value->sensor_id = index;
    };

    /************************************************************************************************
     * SECTION: Construct Messages
     * Date Modified: 06Jun26
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECTION: MQTT
     * Date Modified: 06Jun26
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mEnergyINA226>*> mqtthandler_list;
    struct handler<mEnergyINA226> mqtthandler_settings;
    struct handler<mEnergyINA226> mqtthandler_sensor_ifchanged;
    struct handler<mEnergyINA226> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT
};

#endif // USE_MODULE_ENERGY_INA226

#endif // MSENSORSINA226_H
