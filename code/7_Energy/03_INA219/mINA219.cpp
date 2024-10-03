#include "mINA219.h"

#ifdef USE_MODULE_ENERGY_INA219

int8_t mEnergyINA219::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
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

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
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


void mEnergyINA219::Pre_Init(){

  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

}

void mEnergyINA219::Init(void)
{

  for (uint32_t i = 0; i < ARRAY_SIZE(ina219_addresses); i++) {
    uint16_t addr = ina219_addresses[i];
    if (pCONT_i2c->I2cActive(addr)) 
    { 
      ALOG_INF(PSTR(D_LOG_INA219 "Addr 0x%x not found"), addr); 
      continue; 
    }
    if (SetCalibration(ina219_mode, addr)) {
      pCONT_i2c->I2cSetActiveFound_P(addr, GetModuleName());
      // Create new sensor
      sensor.push_back(sensor_data_s());
      sensor[module_state.devices].address = addr;
      // Add sensor count
      module_state.devices++;
    }
  }
  
  if(module_state.devices){
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_INA219 "Enabled"));
  }

}


void mEnergyINA219::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(module_state.devices)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", module_state.devices);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<module_state.devices; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d:\"%s\" Addr 0x%x, ", sensor_id, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)), sensor[sensor_id].address);    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_INA219 "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


void mEnergyINA219::ReadSensor()
{

  for(uint8_t sensor_id = 0; sensor_id<sensor.size(); sensor_id++)
  {
    uint16_t addr = ina219_addresses[sensor_id];

    sensor[sensor_id].bus_voltage_mv = GetBusVoltage_mV(addr);
    sensor[sensor_id].shunt_voltage_mv = GetShuntVoltage_mV(addr);   
    sensor[sensor_id].direct_current_ma = GetCurrent_mA(addr);
    sensor[sensor_id].direct_power_mw = GetPower_mW(addr);
    sensor[sensor_id].load_voltage_mv = sensor[sensor_id].bus_voltage_mv + sensor[sensor_id].shunt_voltage_mv; 
    // we return the power-supply-side voltage (as bus_voltage register provides the load-side voltage)

    // The following multiplier is used to convert shunt voltage (in mV) to current (in A)
    sensor[sensor_id].current_ma = sensor[sensor_id].shunt_voltage_mv * ina219_current_multiplier * 1000;
    sensor[sensor_id].power_mw  = (sensor[sensor_id].current_ma/1000) * sensor[sensor_id].load_voltage_mv;
    
    ALOG_DBG( PSTR("sensor[%d].load_voltage_mv=%d"), sensor_id, (int)sensor[sensor_id].load_voltage_mv);
    ALOG_DBG( PSTR("sensor[%d].current_ma=%d"),      sensor_id, (int)sensor[sensor_id].current_ma);   
    ALOG_DBG( PSTR("sensor[%d].power_mw=%d"),        sensor_id, (int)sensor[sensor_id].power_mw);   

    sensor[sensor_id].isvalid = true;
    sensor[sensor_id].ischanged = true;

  }


}


float mEnergyINA219::GetShuntVoltage_mV(uint16_t addr)
{
  // raw shunt voltage (16-bit signed integer, so +-32767)
  int16_t value = pCONT_i2c->I2cReadS16(addr, INA219_REG_SHUNTVOLTAGE);
  // //DEBUG_SENSOR_LOG("GetShuntVoltage_mV: ShReg = 0x%04X",value);
  // // convert to shunt voltage in mV (so +-327mV) (LSB=10µV=0.01mV)
  return value * 0.01;
}


float mEnergyINA219::GetBusVoltage_mV(uint16_t addr){
  return GetBusVoltage_V(addr) * 1000;
}
float mEnergyINA219::GetBusVoltage_V(uint16_t addr)
{
  // Shift 3 to the right to drop CNVR and OVF as unsigned
  uint16_t value = pCONT_i2c->I2cRead16(addr, INA219_REG_BUSVOLTAGE) >> 3;
  //ALOG_DBG( PSTR("GetBusVoltage_V: BusReg = 0x%04X"),value);
  // and multiply by LSB raw bus voltage to return bus voltage in volts (LSB=4mV=0.004V)
  return value * 0.004;
}


float mEnergyINA219::GetCurrent_mA(uint16_t addr)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step

  ina219_calValue = 4096; //default full range
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)
 
 pCONT_i2c->I2cWrite16(addr, INA219_REG_CALIBRATION, ina219_calValue);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = pCONT_i2c->I2cReadS16(addr, INA219_REG_CURRENT);
  value /= ina219_currentDivider_mA;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}


float mEnergyINA219::GetPower_mW(uint16_t addr)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step

  ina219_calValue = 4096; //default full range
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)
 
 pCONT_i2c->I2cWrite16(addr, INA219_REG_CALIBRATION, ina219_calValue);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = pCONT_i2c->I2cReadS16(addr, INA219_REG_POWER);
  value *= ina219_powerMultiplier_mW;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}

/*********************************************************************************************\
 * Calculate current multiplier depending on the selected mode
 * For mode = 0, 1, 2 : legacy modes simplified as Vmax: 32V, Imax: 3.2A range
 * For mode = 10..255 : specify Rshunt encoded as RRM where resistor value is RR * 10^M milliOhms
 *                      Vmax: 32V, Imax: 0.320 / Rshunt
 * Exemple:
 *     10: Rshunt = 1 * 10^0 =  1 millOhms   => Max current = 320A !
 *     11: Rshunt = 1 * 10^1 = 10 milliOhms  => Max current = 32A
 *     21: Rshunt = 2 * 10^1 = 20 milliOhms  => Max current = 16A
 *     12: Rshunt = 1 * 10^2 = 100 milliOhms => Max current = 3.2A == mode 0,1,2
 *     13: Rshunt = 1 * 10^3 =   1 Ohms      => Max current = 320mA
 * Note that some shunt values can be represented by 2 different encoded values such as
 *     11 or 100 both present 10 milliOhms
 * Because it is difficult to make a range check on such encoded value, none is performed
\*********************************************************************************************/
bool mEnergyINA219::SetCalibration(uint8_t mode, uint16_t addr)
{
  uint16_t config = 0;

  ALOG_DBG( "SetCalibration: mode=%d",mode);
  if (mode < 5)
  {
    // All legacy modes 0..2 are handled the same and consider default 0.1 shunt resistor
    ina219_current_multiplier = 1.0 / INA219_DEFAULT_SHUNT_RESISTOR_MILLIOHMS;
  }
  else if (mode >= 10)
  {
    int mult = mode % 10;
    int shunt_milliOhms = mode / 10;
    for ( ; mult > 0 ; mult-- )
      shunt_milliOhms *= 10;
    ina219_current_multiplier = 1.0 / shunt_milliOhms;
  }
  config = INA219_CONFIG_BVOLTAGERANGE_32V
         | INA219_CONFIG_GAIN_8_320MV               // Use max scale
         | INA219_CONFIG_BADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_SADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Set Config register to take into account the settings above
  return pCONT_i2c->I2cWrite16(addr, INA219_REG_CONFIG, config);
}
/*!
 *  @brief  Configures to INA219 to be able to measure up to 32V and 2A
 *          of current.  Each unit of current corresponds to 100uA, and
 *          each unit of power corresponds to 2mW. Counter overflow
 *          occurs at 3.2A.
 *  @note   These calculations assume a 0.1 ohm resistor is present
 */
void mEnergyINA219::setCalibration_32V_2A() {
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.

  // VBUS_MAX = 32V             (Assumes 32V, can also be set to 16V)
  // VSHUNT_MAX = 0.32          (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04) 
  // RSHUNT = 0.1               (Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 3.2A

  // 2. Determine max expected current
  // MaxExpected_I = 2.0A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.000061              (61uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0,000488              (488uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.0001 (100uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 4096 (0x1000)

  ina219_calValue = 4096;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.002 (2mW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 3.2767A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.32V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 3.2 * 32V
  // MaximumPower = 102.4W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);

  // // Set Config register to take into account the settings above
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V 
                  | INA219_CONFIG_GAIN_8_320MV 
                  | INA219_CONFIG_BADCRES_12BIT 
                  | INA219_CONFIG_SADCRES_12BIT_1S_532US
                  | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);





}

/*!
 *  @brief  Set power save mode according to parameters
 *  @param  on
 *          boolean value
 */
// void mEnergyINA219::powerSave(bool on) {
//   Adafruit_BusIO_Register config_reg =
//       Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);

//   Adafruit_BusIO_RegisterBits mode_bits =
//       Adafruit_BusIO_RegisterBits(&config_reg, 3, 0);
//   if (on) {
//     mode_bits.write(INA219_CONFIG_MODE_POWERDOWN);
//   } else {
//     mode_bits.write(INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS);
//   }
// }

/*!
 *  @brief  Configures to INA219 to be able to measure up to 32V and 1A
 *          of current.  Each unit of current corresponds to 40uA, and each
 *          unit of power corresponds to 800uW. Counter overflow occurs at
 *          1.3A.
 *  @note   These calculations assume a 0.1 ohm resistor is present
 */
void mEnergyINA219::setCalibration_32V_1A() {
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.

  // VBUS_MAX = 32V		(Assumes 32V, can also be set to 16V)
  // VSHUNT_MAX = 0.32	(Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
  // RSHUNT = 0.1			(Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 3.2A

  // 2. Determine max expected current
  // MaxExpected_I = 1.0A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.0000305             (30.5uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0.000244              (244uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.0000400 (40uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 10240 (0x2800)

  ina219_calValue = 10240;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.0008 (800uW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 1.31068A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // ... In this case, we're good though since Max_Current is less than
  // MaxPossible_I
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.131068V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 1.31068 * 32V
  // MaximumPower = 41.94176W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 25;    // Current LSB = 40uA per bit (1000/40 = 25)
  ina219_powerMultiplier_mW = 0.8f; // Power LSB = 800uW per bit

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);

  // // Set Config register to take into account the settings above
  // uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
  //                   INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
  //                   INA219_CONFIG_SADCRES_12BIT_1S_532US |
  //                   INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);
}

/*!
 *  @brief set device to alibration which uses the highest precision for
 *     current measurement (0.1mA), at the expense of
 *     only supporting 16V at 400mA max.
 */
void mEnergyINA219::setCalibration_16V_400mA() {

  // Calibration which uses the highest precision for
  // current measurement (0.1mA), at the expense of
  // only supporting 16V at 400mA max.

  // VBUS_MAX = 16V
  // VSHUNT_MAX = 0.04          (Assumes Gain 1, 40mV)
  // RSHUNT = 0.1               (Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 0.4A

  // 2. Determine max expected current
  // MaxExpected_I = 0.4A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.0000122              (12uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0.0000977              (98uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.00005 (50uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 8192 (0x2000)

  ina219_calValue = 8192;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.001 (1mW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 1.63835A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_Current_Before_Overflow = MaxPossible_I
  // Max_Current_Before_Overflow = 0.4
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.04V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If
  //
  // Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Max_ShuntVoltage_Before_Overflow = 0.04V

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 0.4 * 16V
  // MaximumPower = 6.4W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 20;    // Current LSB = 50uA per bit (1000/50 = 20)
  ina219_powerMultiplier_mW = 1.0f; // Power LSB = 1mW per bit

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);
  // // Set Config register to take into account the settings above
  // uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
  //                   INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
  //                   INA219_CONFIG_SADCRES_12BIT_1S_532US |
  //                   INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);
}

/************************************************************************************************
 * SECTION: Construct Messages
 ************************************************************************************************/
    
uint8_t mEnergyINA219::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, module_state.devices);
  return JBI->End();

}

uint8_t mEnergyINA219::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  char buffer[50];

  JBI->Start();

  for(uint8_t sensor_id = 0;sensor_id<sensor.size();sensor_id++)
  {
    JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer))); 
      JBI->Add("ina219_current_multiplier",ina219_current_multiplier);
      JBI->Add("Bus" D_VOLTAGE,   sensor[sensor_id].bus_voltage_mv);
      JBI->Add("Shunt" D_VOLTAGE, sensor[sensor_id].shunt_voltage_mv);
      JBI->Add("Load" D_VOLTAGE,  sensor[sensor_id].load_voltage_mv);
      JBI->Add(D_CURRENT,         sensor[sensor_id].current_ma);
      JBI->Add("Direct" D_CURRENT,sensor[sensor_id].direct_current_ma);
      JBI->Add(D_POWER,           sensor[sensor_id].power_mw);
      JBI->Add("Direct" D_POWER,  sensor[sensor_id].direct_power_mw);
    JBI->Object_End();
  }

  ALOG_INF(PSTR(D_LOG_INA219 "%s"), JBI->GetBuffer());
  
  return JBI->End();

}

/************************************************************************************************
 * SECITON: MQTT
 ************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT
void mEnergyINA219::MQTTHandler_Init(){

  struct handler<mEnergyINA219>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} //end "MQTTHandler_Init"
#endif // USE_MODULE_NETWORK_MQTT

#endif
