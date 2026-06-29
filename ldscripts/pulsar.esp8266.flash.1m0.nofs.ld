/* PulSar ESP8266 Flash Split: 1M NoFS
 *
 * Flash:  1 MB
 * Sketch: ~999 KB
 * FS:     0 KB
 * EEPROM: 20 KB
 *
 * Intended use:
 *   - legacy Sonoff/Shelly/ESP-01-class devices
 *   - compiled defaults / template override only
 *   - no LittleFS/SPIFFS mount attempt
 */

MEMORY
{
  dport0_0_seg : org = 0x3FF00000, len = 0x10
  dram0_0_seg  : org = 0x3FFE8000, len = 0x14000
  iram1_0_seg  : org = 0x40100000, len = 0x8000
  irom0_0_seg  : org = 0x40201010, len = 0xF9FF0
}

/* No filesystem partition */
PROVIDE ( _SPIFFS_start = 0x402FB000 );
PROVIDE ( _SPIFFS_end   = 0x402FB000 );
PROVIDE ( _SPIFFS_page  = 0 );
PROVIDE ( _SPIFFS_block = 0 );

INCLUDE "C:\Users\micha\.platformio\packages\framework-arduinoespressif8266\tools\sdk\ld\eagle.flash.1m.ld"