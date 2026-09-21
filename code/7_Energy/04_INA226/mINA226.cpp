#include "mINA226.h"

#ifdef USE_MODULE_ENERGY_INA226

/************************************************************************************************
 * SECTION: Tasker
 * Date Modified: 06Jun26
 ************************************************************************************************/

int8_t mEnergyINA226::Tasker(uint8_t function, JsonParserObject obj)
{
  switch(function)
  {
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function)
  {
    /************
     * PERIODIC SECTION
     *******************/
    case TASK_EVERY_SECOND:
      ReadSensor();
    break;

     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__SUCCESS_ID;
}


void mEnergyINA226::Pre_Init()
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;
  sensor.clear();
}


void mEnergyINA226::Init(void)
{
  for(uint32_t i = 0; i < ARRAY_SIZE(ina226_addresses); i++)
  {
    uint8_t addr = ina226_addresses[i];

    if(tkr_i2c->I2cActive(addr))
    {
      ALOG_INF(PSTR(D_LOG_INA226 "Addr 0x%x not found"), addr);
      continue;
    }

    sensor.push_back(sensor_data_s());
    uint8_t sensor_id = sensor.size() - 1;
    sensor[sensor_id].address = addr;

    if(ConfigureDevice(sensor_id))
    {
      tkr_i2c->I2cSetActiveFound_P(addr, GetModuleName());
      module_state.devices++;
    }
    else
    {
      sensor.pop_back();
    }
  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_INA226 "Enabled"));
  }
}


void mEnergyINA226::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[120] = {0};

  if(module_state.devices)
  {
    for(uint8_t sensor_id = 0; sensor_id < sensor.size(); sensor_id++)
    {
      mSupport::appendToBuffer(buffer, sizeof(buffer), "0x%02X,", sensor[sensor_id].address);
    }
  }
  else
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");
  }

  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_INA226 "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


/************************************************************************************************
 * SECTION: Sensor Read
 * Date Modified: 06Jun26
 ************************************************************************************************/

void mEnergyINA226::ReadSensor()
{
  for(uint8_t sensor_id = 0; sensor_id < sensor.size(); sensor_id++)
  {
    sensor[sensor_id].bus_voltage_v    = GetBusVoltage_V(sensor_id);
    sensor[sensor_id].bus_voltage_mv   = sensor[sensor_id].bus_voltage_v * 1000.0f;
    sensor[sensor_id].shunt_voltage_mv = GetShuntVoltage_mV(sensor_id);
    sensor[sensor_id].load_voltage_mv  = sensor[sensor_id].bus_voltage_mv + sensor[sensor_id].shunt_voltage_mv;

    sensor[sensor_id].current_ma       = GetCurrent_mA(sensor_id);
    sensor[sensor_id].current_a        = sensor[sensor_id].current_ma / 1000.0f;
    sensor[sensor_id].power_mw         = GetPower_mW(sensor_id);
    sensor[sensor_id].power_w          = sensor[sensor_id].power_mw / 1000.0f;

    if(sensor[sensor_id].invert_reported_sign)
    {
      sensor[sensor_id].current_ma *= -1.0f;
      sensor[sensor_id].current_a  *= -1.0f;
      sensor[sensor_id].power_mw   *= -1.0f;
      sensor[sensor_id].power_w    *= -1.0f;
    }

    ReadAndClearFlags(sensor_id);

    ALOG_DBG(PSTR("INA226[%d] bus_mv=%d shunt_mv=%d current_ma=%d power_mw=%d"),
      sensor_id,
      (int)sensor[sensor_id].bus_voltage_mv,
      (int)sensor[sensor_id].shunt_voltage_mv,
      (int)sensor[sensor_id].current_ma,
      (int)sensor[sensor_id].power_mw
    );

    sensor[sensor_id].isvalid = true;
    sensor[sensor_id].ischanged = true;
  }
}


/************************************************************************************************
 * SECTION: Configuration
 * Date Modified: 06Jun26
 ************************************************************************************************/

bool mEnergyINA226::ConfigureDevice(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  Reset(sensor_id);
  delay(2);

  if(!SetResistorRange(sensor_id, INA226_DEFAULT_SHUNT_RESISTOR_OHMS, INA226_DEFAULT_CURRENT_RANGE_A)){ return false; }

  SetAverage(sensor_id, INA226_AVERAGE_16);
  SetConversionTime(sensor_id, INA226_CONV_TIME_1100);
  SetMeasureMode(sensor_id, INA226_CONTINUOUS);

  ReadSensor();

  return true;
}


bool mEnergyINA226::Reset(uint8_t sensor_id)
{
  return WriteRegister(sensor_id, INA226_REG_CONFIG, INA226_CONFIG_RESET);
}


bool mEnergyINA226::SetCorrectionFactor(uint8_t sensor_id, float correction_factor)
{
  if(sensor_id >= sensor.size()){ return false; }

  sensor[sensor_id].correction_factor = correction_factor;
  uint16_t corrected_cal = (uint16_t)((float)sensor[sensor_id].cal_value * sensor[sensor_id].correction_factor);
  return WriteRegister(sensor_id, INA226_REG_CALIBRATION, corrected_cal);
}


bool mEnergyINA226::SetAverage(uint8_t sensor_id, INA226_Averages averages)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t config = ReadRegister(sensor_id, INA226_REG_CONFIG);
  config &= ~(0x0E00);
  config |= averages;
  return WriteRegister(sensor_id, INA226_REG_CONFIG, config);
}


bool mEnergyINA226::SetConversionTime(uint8_t sensor_id, INA226_ConversionTime conv_time)
{
  return SetConversionTime(sensor_id, conv_time, conv_time);
}


bool mEnergyINA226::SetConversionTime(uint8_t sensor_id, INA226_ConversionTime shunt_conv_time, INA226_ConversionTime bus_conv_time)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t config = ReadRegister(sensor_id, INA226_REG_CONFIG);
  config &= ~(0x01C0);
  config &= ~(0x0038);
  config |= (((uint16_t)shunt_conv_time) << 3);
  config |= (((uint16_t)bus_conv_time) << 6);
  return WriteRegister(sensor_id, INA226_REG_CONFIG, config);
}


bool mEnergyINA226::SetMeasureMode(uint8_t sensor_id, INA226_MeasureMode mode)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t config = ReadRegister(sensor_id, INA226_REG_CONFIG);
  config &= ~(0x0007);
  config |= (uint16_t)mode;
  return WriteRegister(sensor_id, INA226_REG_CONFIG, config);
}


bool mEnergyINA226::SetResistorRange(uint8_t sensor_id, float resistor_ohms, float current_range_a)
{
  if(sensor_id >= sensor.size()){ return false; }
  if(resistor_ohms <= 0.0f){ return false; }

  if(current_range_a < 0.0f)
  {
    current_range_a = 0.0819175f / resistor_ohms;
  }

  float current_lsb_a = current_range_a / 32768.0f;
  if(current_lsb_a <= 0.0f){ return false; }

  float cal_float = 0.00512f / (current_lsb_a * resistor_ohms);
  if(cal_float < 1.0f){ cal_float = 1.0f; }
  if(cal_float > 65535.0f){ cal_float = 65535.0f; }

  sensor[sensor_id].cal_value = (uint16_t)cal_float;
  sensor[sensor_id].current_divider_ma = 0.001f / current_lsb_a;
  sensor[sensor_id].power_multiplier_mw = 1000.0f * 25.0f * current_lsb_a;

  return WriteRegister(sensor_id, INA226_REG_CALIBRATION, sensor[sensor_id].cal_value);
}


/************************************************************************************************
 * SECTION: Measurement Helpers
 * Date Modified: 06Jun26
 ************************************************************************************************/

float mEnergyINA226::GetShuntVoltage_mV(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return 0.0f; }

  int16_t value = ReadRegisterS16(sensor_id, INA226_REG_SHUNT);
  return ((float)value * INA226_SHUNT_LSB_MV * sensor[sensor_id].correction_factor);
}


float mEnergyINA226::GetBusVoltage_mV(uint8_t sensor_id)
{
  return GetBusVoltage_V(sensor_id) * 1000.0f;
}


float mEnergyINA226::GetBusVoltage_V(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return 0.0f; }

  uint16_t value = ReadRegister(sensor_id, INA226_REG_BUS);
  return ((float)value * INA226_BUS_LSB_V);
}


float mEnergyINA226::GetCurrent_mA(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return 0.0f; }

  // The INA226 CURRENT and POWER registers depend on the calibration register.
  // Re-write calibration before reading, matching the safety pattern used in mINA219.
  WriteRegister(sensor_id, INA226_REG_CALIBRATION, sensor[sensor_id].cal_value);

  int16_t value = ReadRegisterS16(sensor_id, INA226_REG_CURRENT);
  return ((float)value / sensor[sensor_id].current_divider_ma);
}


float mEnergyINA226::GetCurrent_A(uint8_t sensor_id)
{
  return GetCurrent_mA(sensor_id) / 1000.0f;
}


float mEnergyINA226::GetPower_mW(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return 0.0f; }

  WriteRegister(sensor_id, INA226_REG_CALIBRATION, sensor[sensor_id].cal_value);

  uint16_t value = ReadRegister(sensor_id, INA226_REG_POWER);
  return ((float)value * sensor[sensor_id].power_multiplier_mw);
}


float mEnergyINA226::GetPower_W(uint8_t sensor_id)
{
  return GetPower_mW(sensor_id) / 1000.0f;
}


/************************************************************************************************
 * SECTION: Single Conversion / Power State
 * Date Modified: 06Jun26
 ************************************************************************************************/

bool mEnergyINA226::StartSingleMeasurement(uint8_t sensor_id, bool wait_for_completion)
{
  if(sensor_id >= sensor.size()){ return false; }

  ReadRegister(sensor_id, INA226_REG_MASK_ENABLE); // Clear CVRF.
  uint16_t config = ReadRegister(sensor_id, INA226_REG_CONFIG);
  bool result = WriteRegister(sensor_id, INA226_REG_CONFIG, config);

  if(wait_for_completion)
  {
    uint32_t conv_start = millis();
    while(IsBusy(sensor_id) && ((millis() - conv_start) < 2000))
    {
      delay(1);
    }
  }

  return result;
}


bool mEnergyINA226::IsBusy(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  return !(ReadRegister(sensor_id, INA226_REG_MASK_ENABLE) & INA226_MASK_CVRF);
}


void mEnergyINA226::WaitUntilConversionCompleted(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return; }

  ReadRegister(sensor_id, INA226_REG_MASK_ENABLE); // Clear CVRF.
  while(IsBusy(sensor_id))
  {
    delay(1);
  }
}


bool mEnergyINA226::PowerDown(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  sensor[sensor_id].config_copy = ReadRegister(sensor_id, INA226_REG_CONFIG);
  return SetMeasureMode(sensor_id, INA226_POWER_DOWN);
}


bool mEnergyINA226::PowerUp(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  bool result = WriteRegister(sensor_id, INA226_REG_CONFIG, sensor[sensor_id].config_copy);
  delayMicroseconds(40);
  return result;
}


/************************************************************************************************
 * SECTION: Alert Helpers
 * Date Modified: 06Jun26
 ************************************************************************************************/

bool mEnergyINA226::SetAlertPinActiveHigh(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t value = ReadRegister(sensor_id, INA226_REG_MASK_ENABLE);
  value |= INA226_MASK_ALERT_POL;
  return WriteRegister(sensor_id, INA226_REG_MASK_ENABLE, value);
}


bool mEnergyINA226::EnableAlertLatch(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t value = ReadRegister(sensor_id, INA226_REG_MASK_ENABLE);
  value |= INA226_MASK_LATCH_EN;
  return WriteRegister(sensor_id, INA226_REG_MASK_ENABLE, value);
}


bool mEnergyINA226::EnableConvReadyAlert(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t value = ReadRegister(sensor_id, INA226_REG_MASK_ENABLE);
  value |= 0x0400;
  return WriteRegister(sensor_id, INA226_REG_MASK_ENABLE, value);
}


bool mEnergyINA226::SetAlertType(uint8_t sensor_id, INA226_AlertType type, float limit)
{
  if(sensor_id >= sensor.size()){ return false; }

  uint16_t alert_limit = 0;
  uint16_t device_alert_type = (uint16_t)type;

  switch(type)
  {
    case INA226_SHUNT_OVER:
    case INA226_SHUNT_UNDER:
      alert_limit = (uint16_t)(limit * 400.0f); // limit in mV, 2.5 uV LSB.
    break;

    case INA226_CURRENT_OVER:
      device_alert_type = INA226_SHUNT_OVER;
      alert_limit = (uint16_t)(limit * 2048.0f * sensor[sensor_id].current_divider_ma / sensor[sensor_id].cal_value);
    break;

    case INA226_CURRENT_UNDER:
      device_alert_type = INA226_SHUNT_UNDER;
      alert_limit = (uint16_t)(limit * 2048.0f * sensor[sensor_id].current_divider_ma / sensor[sensor_id].cal_value);
    break;

    case INA226_BUS_OVER:
    case INA226_BUS_UNDER:
      alert_limit = (uint16_t)(limit * 800.0f); // limit in V, 1.25 mV LSB.
    break;

    case INA226_POWER_OVER:
      alert_limit = (uint16_t)(limit / sensor[sensor_id].power_multiplier_mw);
    break;
  }

  WriteRegister(sensor_id, INA226_REG_ALERT_LIMIT, alert_limit);

  uint16_t value = ReadRegister(sensor_id, INA226_REG_MASK_ENABLE);
  value &= ~(0xF800);
  value |= device_alert_type;
  return WriteRegister(sensor_id, INA226_REG_MASK_ENABLE, value);
}


void mEnergyINA226::ReadAndClearFlags(uint8_t sensor_id)
{
  if(sensor_id >= sensor.size()){ return; }

  uint16_t value = ReadRegister(sensor_id, INA226_REG_MASK_ENABLE);
  sensor[sensor_id].overflow = (value & INA226_MASK_OVF) != 0;
  sensor[sensor_id].conversion_ready = (value & INA226_MASK_CVRF) != 0;
  sensor[sensor_id].limit_alert = (value & INA226_MASK_AFF) != 0;
}


/************************************************************************************************
 * SECTION: Register Access
 * Date Modified: 06Jun26
 ************************************************************************************************/

uint16_t mEnergyINA226::ReadRegister(uint8_t sensor_id, uint8_t reg)
{
  if(sensor_id >= sensor.size()){ return 0; }

  return tkr_i2c->I2cRead16(sensor[sensor_id].address, reg);
}


int16_t mEnergyINA226::ReadRegisterS16(uint8_t sensor_id, uint8_t reg)
{
  if(sensor_id >= sensor.size()){ return 0; }

  return tkr_i2c->I2cReadS16(sensor[sensor_id].address, reg);
}


bool mEnergyINA226::WriteRegister(uint8_t sensor_id, uint8_t reg, uint16_t value)
{
  if(sensor_id >= sensor.size()){ return false; }

  return tkr_i2c->I2cWrite16(sensor[sensor_id].address, reg, value);
}


/************************************************************************************************
 * SECTION: Construct Messages
 * Date Modified: 06Jun26
 ************************************************************************************************/

uint8_t mEnergyINA226::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, module_state.devices);
  return JBI->End();
}


uint8_t mEnergyINA226::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  char buffer[50];

  JBI->Start();

  for(uint8_t sensor_id = 0; sensor_id < sensor.size(); sensor_id++)
  {
    JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));
      JBI->Add("Address", sensor[sensor_id].address);
      JBI->Add("Bus" D_VOLTAGE, sensor[sensor_id].bus_voltage_mv);
      JBI->Add("Shunt" D_VOLTAGE, sensor[sensor_id].shunt_voltage_mv);
      JBI->Add("Load" D_VOLTAGE, sensor[sensor_id].load_voltage_mv);
      JBI->Add(D_CURRENT, sensor[sensor_id].current_ma);
      JBI->Add(D_POWER, sensor[sensor_id].power_mw);
      JBI->Add("Cal", sensor[sensor_id].cal_value);
      JBI->Add("CurrentDivider_mA", sensor[sensor_id].current_divider_ma);
      JBI->Add("PowerMultiplier_mW", sensor[sensor_id].power_multiplier_mw);
      JBI->Add("Overflow", sensor[sensor_id].overflow);
      JBI->Add("ConvReady", sensor[sensor_id].conversion_ready);
      JBI->Add("LimitAlert", sensor[sensor_id].limit_alert);
    JBI->Object_End();
  }

  ALOG_INF(PSTR(D_LOG_INA226 "%s"), JBI->GetBuffer());

  return JBI->End();
}


/************************************************************************************************
 * SECTION: MQTT
 * Date Modified: 06Jun26
 ************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT
void mEnergyINA226::Telemetry_Init()
{
  struct telemetry_handler<mEnergyINA226>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA226::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA226::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA226::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);
}
#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_ENERGY_INA226
