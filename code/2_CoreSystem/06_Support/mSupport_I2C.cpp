#include "mSupport.h"


#ifdef USE_I2C

bool mSupport::I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  i2c_buffer = 0;
  do {
    wire->beginTransmission(addr);                       // start transmission to device
    wire->write(reg);                                    // sends register address to read from
    if (0 == wire->endTransmission(false)) {             // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      wire->requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (wire->available() == size) {
        for (uint8_t i = 0; i < size; i++) {
          i2c_buffer = i2c_buffer << 8 | wire->read();   // receive DATA
        }
      }
    }
    x--;
  } while (wire->endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool mSupport::I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 1);
  *data = (uint8_t)i2c_buffer;
  return status;
}

bool mSupport::I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (uint16_t)i2c_buffer;
  return status;
}

bool mSupport::I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (int16_t)i2c_buffer;
  return status;
}

bool mSupport::I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16(&ldata, addr, reg);
  *data = (ldata >> 8) | (ldata << 8);
  return status;
}

bool mSupport::I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16LE(&ldata, addr, reg);
  *data = (int16_t)ldata;
  return status;
}

bool mSupport::I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 3);
  *data = i2c_buffer;
  return status;
}

uint8_t mSupport::I2cRead8(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 1);
  return (uint8_t)i2c_buffer;
}

uint16_t mSupport::I2cRead16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (uint16_t)i2c_buffer;
}

int16_t mSupport::I2cReadS16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (int16_t)i2c_buffer;
}

uint16_t mSupport::I2cRead16LE(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  uint16_t temp = (uint16_t)i2c_buffer;
  return (temp >> 8) | (temp << 8);
}

int16_t mSupport::I2cReadS16_LE(uint8_t addr, uint8_t reg)
{
  return (int16_t)I2cRead16LE(addr, reg);
}

int32_t mSupport::I2cRead24(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 3);
  return i2c_buffer;
}



bool mSupport::I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  do {
    wire->beginTransmission((uint8_t)addr);              // start transmission to device
    wire->write(reg);                                    // sends register address to write to
    uint8_t bytes = size;
    while (bytes--) {
      wire->write((val >> (8 * bytes)) & 0xFF);          // write data
    }
    x--;
  } while (wire->endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool mSupport::I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 1);
}

bool mSupport::I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 2);
}

int8_t mSupport::I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  wire->beginTransmission((uint8_t)addr);
  wire->write((uint8_t)reg);
  wire->endTransmission();
  if (len != wire->requestFrom((uint8_t)addr, (uint8_t)len)) {
    return 1;
  }
  while (len--) {
    *reg_data = (uint8_t)wire->read();
    reg_data++;
  }
  return 0;
}

int8_t mSupport::I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  wire->beginTransmission((uint8_t)addr);
  wire->write((uint8_t)reg);
  while (len--) {
    wire->write(*reg_data);
    reg_data++;
  }
  wire->endTransmission();
  return 0;
}

void mSupport::I2cScan(char *devs, unsigned int devs_len)
{
  // Return error codes defined in twi.h and core_esp8266_si2c.c
  // I2C_OK                      0
  // I2C_SCL_HELD_LOW            1 = SCL held low by another device, no procedure available to recover
  // I2C_SCL_HELD_LOW_AFTER_READ 2 = I2C bus error. SCL held low beyond slave clock stretch time
  // I2C_SDA_HELD_LOW            3 = I2C bus error. SDA line held low by slave/another_master after n bits
  // I2C_SDA_HELD_LOW_AFTER_INIT 4 = line busy. SDA again held low by another device. 2nd master?


// #ifdef ESP8266

// DEBUG_LINE_HERE;
// delay(5000);

  uint8_t error = 0;
  uint8_t address = 0;
  uint8_t any = 0;

  snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"" D_JSON_I2CSCAN_DEVICES_FOUND_AT));

  for (address = 1; address <= 127; address++) {
    wire->beginTransmission(address);
    error = wire->endTransmission();
    if (0 == error) {
      any = 1;
      snprintf_P(devs, devs_len, PSTR("%s 0x%02x"), devs, address);
    }
    else if (error != 2) {  // Seems to happen anyway using this scan
      any = 2;
      snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"Error %d at 0x%02x"), error, address);
      break;
    }
  }
  if (any) {
    strncat(devs, "\"}", devs_len - strlen(devs) -1);
  }
  else {
    snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"" D_JSON_I2CSCAN_NO_DEVICES_FOUND "\"}"));
  }

// #else

// snprintf_P(devs, devs_len, PSTR("{\"" D_JSON_I2CSCAN "\":\"not on esp32 yet\"}" ));
  

// #endif

}

//old function that seems to be changed
bool mSupport::I2cDevice(uint8_t addr) // This checks ALL, not just the desired address so is slow
{

  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "I2cDevice(%x)=starting"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO

  for (uint8_t address = 1; address <= 127; address++) {
      // AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x|%x)=for"),address,addr);
    wire->beginTransmission(address);
    if (!wire->endTransmission() && (address == addr)) {
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x)=true"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO
      return true;
    }else
    if (!wire->endTransmission() && (address != addr)){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x) also found %x"),address,addr);
    #endif// ENABLE_LOG_LEVEL_INFO
    }
  }
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x)=FALSE"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO
  return false;
}

bool mSupport::I2cDevice_IsConnected(uint8_t addr) // This checks ALL, not just the desired address so is slow
{

  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "I2cDevice(%x)=starting"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO

  // for (
    uint8_t address = addr;
  //  address <= 127; address++) {
      AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x)=for"),addr);
    wire->beginTransmission(address);
    if (!wire->endTransmission() && (address == addr)) {
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x)=true"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO
      return true;
    }else
    if (!wire->endTransmission() && (address != addr)){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x) also found %x"),address,addr);
    #endif// ENABLE_LOG_LEVEL_INFO
    }
  // }
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("I2cDevice(%x)=FALSE"),addr);
    #endif// ENABLE_LOG_LEVEL_INFO
  return false;
}


void mSupport::I2cResetActive(uint32_t addr, uint32_t count)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] &= ~(1 << (addr % 32));
    addr++;
  }
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3]);
    #endif // ENABLE_LOG_LEVEL_INFO
}

void mSupport::I2cSetActive(uint32_t addr, uint32_t count)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] |= (1 << (addr % 32));
    addr++;
  }
  ALOG_DBG( PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3] );
}

void mSupport::I2cSetActiveFound(uint32_t addr, const char *types)
{
  I2cSetActive(addr);
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO, S_LOG_I2C_FOUND_AT, types, addr);
  #endif // ENABLE_LOG_LEVEL_INFO
}

bool mSupport::I2cActive(uint32_t addr)
{
  addr &= 0x7F;         // Max I2C address is 127
  if (i2c_active[addr / 32] & (1 << (addr % 32))) {
    return true;
  }
  return false;
}

bool mSupport::I2cSetDevice(uint32_t addr)
{
  addr &= 0x7F;         // Max I2C address is 127
  if (I2cActive(addr)) {
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2cSetDevice already active"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;       // If already active report as not present;
  }
    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
ALOG_DBM( PSTR("I2cSetDevice beginning"));
    #endif //  ENABLE_LOG_LEVEL_INFO
  wire->beginTransmission((uint8_t)addr);
  return (0 == wire->endTransmission());
}

bool mSupport::I2cEnabled(uint32_t i2c_index)
{

// Serial.println("I2cEnabled"); Serial.flush();
// delay(3000);
bool val = (pCONT_set->i2c_enabled && bitRead(pCONT_set->Settings.i2c_drivers[i2c_index / 32], i2c_index % 32));


return true; // force all working for now until i2cdrivers are configured

// Serial.println(val); Serial.flush();
  return val;

// Serial.println("val"); Serial.flush();
// Serial.println(val); Serial.flush();
// delay(3000);

  // return val;
}


#endif  // USE_I2C