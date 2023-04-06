#include "2_CoreSystem/06_Support/SupportESP32.h"

#ifdef ESP32

#include "Arduino.h"


uint32_t SupportESP32::ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}


String SupportESP32::GetDeviceHardware(void) {
  // https://www.espressif.com/en/products/socs

/*
Source: esp-idf esp_system.h and esptool

typedef enum {
    CHIP_ESP32  = 1, //!< ESP32
    CHIP_ESP32S2 = 2, //!< ESP32-S2
    CHIP_ESP32S3 = 9, //!< ESP32-S3
    CHIP_ESP32C3 = 5, //!< ESP32-C3
    CHIP_ESP32H2 = 6, //!< ESP32-H2
    CHIP_ESP32C2 = 12, //!< ESP32-C2
} esp_chip_model_t;

// Chip feature flags, used in esp_chip_info_t
#define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
#define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
#define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
#define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic
#define CHIP_FEATURE_IEEE802154     BIT(6)      //!< Chip has IEEE 802.15.4
#define CHIP_FEATURE_EMB_PSRAM      BIT(7)      //!< Chip has embedded psram


// The structure represents information about the chip
typedef struct {
    esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
    uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
    uint8_t cores;           //!< number of CPU cores
    uint8_t revision;        //!< chip revision number
} esp_chip_info_t;

*/
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  uint32_t chip_model = chip_info.model;
  uint32_t chip_revision = chip_info.revision;
//  uint32_t chip_revision = ESP.getChipRevision();
  bool rev3 = (3 == chip_revision);
//  bool single_core = (1 == ESP.getChipCores());
  bool single_core = (1 == chip_info.cores);

  if (chip_model < 2) {  // ESP32
    /*
    ESP32 Series
    - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth/Bluetooth LE
    - Two or one CPU core(s) with adjustable clock frequency, ranging from 80 MHz to 240 MHz
    - +19.5 dBm output power ensures a good physical range
    - Classic Bluetooth for legacy connections, also supporting L2CAP, SDP, GAP, SMP, AVDTP, AVCTP, A2DP (SNK) and AVRCP (CT)
    - Support for Bluetooth Low Energy (Bluetooth LE) profiles including L2CAP, GAP, GATT, SMP, and GATT-based profiles like BluFi, SPP-like, etc
    - Bluetooth Low Energy (Bluetooth LE) connects to smart phones, broadcasting low-energy beacons for easy detection
    - Sleep current is less than 5 μA, making it suitable for battery-powered and wearable-electronics applications
    - Peripherals include capacitive touch sensors, Hall sensor, SD card interface, Ethernet, high-speed SPI, UART, I2S and I2C
    */
#ifdef CONFIG_IDF_TARGET_ESP32
/* esptool:
    def get_pkg_version(self):
        word3 = self.read_efuse(3)
        pkg_version = (word3 >> 9) & 0x07
        pkg_version += ((word3 >> 2) & 0x1) << 3
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
    uint32_t pkg_version = chip_ver & 0x7;

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:
        if (single_core) { return F("ESP32-S0WDQ6"); }     // Max 240MHz, Single core, QFN 6*6
        else if (rev3)   { return F("ESP32-D0WDQ6-V3"); }  // Max 240MHz, Dual core, QFN 6*6
        else {             return F("ESP32-D0WDQ6"); }     // Max 240MHz, Dual core, QFN 6*6
      case 1:
        if (single_core) { return F("ESP32-S0WD"); }       // Max 160MHz, Single core, QFN 5*5, ESP32-SOLO-1, ESP32-DevKitC
        else if (rev3)   { return F("ESP32-D0WD-V3"); }    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
        else {             return F("ESP32-D0WD"); }       // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32D, ESP32_WROVER-B, ESP32-DevKitC
      case 2:              return F("ESP32-D2WD");         // Max 160MHz, Dual core, QFN 5*5, 2MB embedded flash
      case 3:
        if (single_core) { return F("ESP32-S0WD-OEM"); }   // Max 160MHz, Single core, QFN 5*5, Xiaomi Yeelight
        else {             return F("ESP32-D0WD-OEM"); }   // Max 240MHz, Dual core, QFN 5*5
      case 4:
        if (single_core) { return F("ESP32-U4WDH-S"); }    // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-MINI-1, ESP32-DevKitM-1
        else {             return F("ESP32-U4WDH-D"); }    // Max 240MHz, Dual core, QFN 5*5, 4MB embedded flash
      case 5:
        if (rev3)        { return F("ESP32-PICO-V3"); }    // Max 240MHz, Dual core, LGA 7*7, ESP32-PICO-V3-ZERO, ESP32-PICO-V3-ZERO-DevKit
        else {             return F("ESP32-PICO-D4"); }    // Max 240MHz, Dual core, LGA 7*7, 4MB embedded flash, ESP32-PICO-KIT
      case 6:              return F("ESP32-PICO-V3-02");   // Max 240MHz, Dual core, LGA 7*7, 8MB embedded flash, 2MB embedded PSRAM, ESP32-PICO-MINI-02, ESP32-PICO-DevKitM-2
      case 7:              return F("ESP32-D0WDR2-V3");    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
    }
#endif  // CONFIG_IDF_TARGET_ESP32
    return F("ESP32");
  }
  else if (2 == chip_model) {  // ESP32-S2
    /*
    ESP32-S2 Series
    - 32-bit MCU & 2.4 GHz Wi-Fi
    - High-performance 240 MHz single-core CPU
    - Ultra-low-power performance: fine-grained clock gating, dynamic voltage and frequency scaling
    - Security features: eFuse、flash encryption, secure boot, signature verification, integrated AES, SHA and RSA algorithms
    - Peripherals include 43 GPIOs, 1 full-speed USB OTG interface, SPI, I2S, UART, I2C, LED PWM, LCD interface, camera interface, ADC, DAC, touch sensor, temperature sensor
    - Availability of common cloud connectivity agents and common product features shortens the time to market
    */
#ifdef CONFIG_IDF_TARGET_ESP32S2
/* esptool:
    def get_flash_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version

    def get_psram_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 28) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_FLASH_VERSION);
    uint32_t psram_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PSRAM_VERSION);
    uint32_t pkg_version = (chip_ver & 0xF) + ((psram_ver & 0xF) * 100);

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-S2");           // Max 240MHz, Single core, QFN 7*7, ESP32-S2-WROOM, ESP32-S2-WROVER, ESP32-S2-Saola-1, ESP32-S2-Kaluga-1
      case 1:              return F("ESP32-S2FH2");        // Max 240MHz, Single core, QFN 7*7, 2MB embedded flash, ESP32-S2-MINI-1, ESP32-S2-DevKitM-1
      case 2:              return F("ESP32-S2FH4");        // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash
      case 3:              return F("ESP32-S2FN4R2");      // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , ESP32-S2-MINI-1U, ESP32-S2-DevKitM-1U
      case 100:            return F("ESP32-S2R2");
      case 102:            return F("ESP32-S2FNR2");       // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , Lolin S2 mini
    }
#endif  // CONFIG_IDF_TARGET_ESP32S2
    return F("ESP32-S2");
  }
  else if (4 == chip_model) {  // ESP32-S3(beta2)
    return F("ESP32-S3");
  }
  else if (5 == chip_model) {  // ESP32-C3 = ESP8685
    /*
    ESP32-C3 Series
    - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
    - 32-bit RISC-V single-core processor with a four-stage pipeline that operates at up to 160 MHz
    - State-of-the-art power and RF performance
    - 400 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, and QPI interfaces that allow connection to flash
    - Reliable security features ensured by RSA-3072-based secure boot, AES-128-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, hardware acceleration support for cryptographic algorithms
    - Rich set of peripheral interfaces and GPIOs, ideal for various scenarios and complex applications
    */
#ifdef CONFIG_IDF_TARGET_ESP32C3
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-C3");           // Max 160MHz, Single core, QFN 5*5, ESP32-C3-WROOM-02, ESP32-C3-DevKitC-02
      case 1:              return F("ESP32-C3FH4");        // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
    }
#endif  // CONFIG_IDF_TARGET_ESP32C3
    return F("ESP32-C3");
  }
  else if (6 == chip_model) {  // ESP32-S3(beta3)
    return F("ESP32-S3");
  }
  else if (7 == chip_model) {  // ESP32-C6(beta)
#ifdef CONFIG_IDF_TARGET_ESP32C6
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-C6");
    }
#endif  // CONFIG_IDF_TARGET_ESP32C6
    return F("ESP32-C6");
  }
  else if (9 == chip_model) {  // ESP32-S3
    /*
    ESP32-S3 Series
    - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
    - Xtensa® 32-bit LX7 dual-core processor that operates at up to 240 MHz
    - 512 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, Octal SPI, QPI, and OPI interfaces that allow connection to flash and external RAM
    - Additional support for vector instructions in the MCU, which provides acceleration for neural network computing and signal processing workloads
    - Peripherals include 45 programmable GPIOs, SPI, I2S, I2C, PWM, RMT, ADC and UART, SD/MMC host and TWAITM
    - Reliable security features ensured by RSA-based secure boot, AES-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, “World Controller”
    */
#ifdef CONFIG_IDF_TARGET_ESP32S3
    // no variants for now
#endif  // CONFIG_IDF_TARGET_ESP32S3
    return F("ESP32-S3");                                  // Max 240MHz, Dual core, QFN 7*7, ESP32-S3-WROOM-1, ESP32-S3-DevKitC-1
  }
  else if (10 == chip_model) {  // ESP32-H2(beta1)
#ifdef CONFIG_IDF_TARGET_ESP32H2
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-H2");
    }
#endif  // CONFIG_IDF_TARGET_ESP32H2
    return F("ESP32-H2");
  }
  else if (12 == chip_model) {  // ESP32-C2 = ESP8684
    /*
    ESP32-C2 Series
    - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
    - 32-bit RISC-V single-core processor that operates at up to 120 MHz
    - State-of-the-art power and RF performance
    - 576 KB ROM, 272 KB SRAM (16 KB for cache) on the chip
    - 14 programmable GPIOs: SPI, UART, I2C, LED PWM controller, General DMA controller (GDMA), SAR ADC, Temperature sensor
    */

    return F("ESP32-C2");
  }
  else if (13 == chip_model) {  // ESP32-C6
    /*
    ESP32-C6 Series
    - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) & IEEE 802.15.4
    - 32-bit RISC-V single-core processor that operates at up to 160 MHz
    - State-of-the-art power and RF performance
    - 320 KB ROM, 512 KB SRAM, 16 KB Low-power SRAM on the chip, and works with external flash
    - 30 (QFN40) or 22 (QFN32) programmable GPIOs, with support for SPI, UART, I2C, I2S, RMT, TWAI and PWM
    */

    return F("ESP32-C6");
  }
  else if (14 == chip_model) {  // ESP32-H2(beta2)
    return F("ESP32-H2");
  }
  return F("ESP32");
}



extern "C" {
  bool esp_spiram_is_initialized(void);
}

// this function is a replacement for `psramFound()`.
// `psramFound()` can return true even if no PSRAM is actually installed
// This new version also checks `esp_spiram_is_initialized` to know if the PSRAM is initialized
bool SupportESP32::FoundPSRAM(void) {
#if CONFIG_IDF_TARGET_ESP32C3
  return psramFound();
#else
  return psramFound() && esp_spiram_is_initialized();
#endif
}

// // new function to check whether PSRAM is present and supported (i.e. required pacthes are present)
// bool UsePSRAM(void) {
//   static bool can_use_psram = CanUsePSRAM();
//   return FoundPSRAM() && can_use_psram;
// }

// void *special_malloc(uint32_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return malloc(size);
//   }
// }
// void *special_realloc(void *ptr, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return realloc(ptr, size);
//   }
// }
// void *special_calloc(size_t num, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return calloc(num, size);
//   }
// }

// // Variants for IRAM heap, which need all accesses to be 32 bits aligned
// void *special_malloc32(uint32_t size) {
//   return heap_caps_malloc(size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_realloc32(void *ptr, size_t size) {
//   return heap_caps_realloc(ptr, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_calloc32(size_t num, size_t size) {
//   return heap_caps_calloc(num, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }






#endif







