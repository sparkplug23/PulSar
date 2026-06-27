/* PulSar ESP8266 Flash Split: 1M + 64K FS
 *
 * Flash:  1 MB
 * Sketch: ~936 KB
 * FS:     64 KB
 * EEPROM: 20 KB
 *
 * Layout:
 *   Sketch starts at 0x40201010
 *   FS starts at     0x402EB000
 *   FS ends at       0x402FB000
 *   EEPROM region follows, 20 KB
 *
 * Intended use:
 *   - constrained ESP8266 filesystem validation
 *   - possible intermediate-flash / minimal web/config tests
 */

MEMORY
{
  dport0_0_seg : org = 0x3FF00000, len = 0x10
  dram0_0_seg  : org = 0x3FFE8000, len = 0x14000
  iram1_0_seg  : org = 0x40100000, len = 0x8000
  irom0_0_seg  : org = 0x40201010, len = 0xE9FF0
}

/* 64 KB filesystem partition */
PROVIDE ( _FS_start = 0x402EB000 );
PROVIDE ( _FS_end   = 0x402FB000 );
PROVIDE ( _FS_page  = 0x100 );
PROVIDE ( _FS_block = 0x2000 );

/* Legacy aliases, kept for older/Tasmota-style code paths */
PROVIDE ( _SPIFFS_start = _FS_start );
PROVIDE ( _SPIFFS_end   = _FS_end );
PROVIDE ( _SPIFFS_page  = _FS_page );
PROVIDE ( _SPIFFS_block = _FS_block );

INCLUDE "C:\Users\micha\.platformio\packages\framework-arduinoespressif8266\tools\sdk\ld\eagle.flash.1m.ld"