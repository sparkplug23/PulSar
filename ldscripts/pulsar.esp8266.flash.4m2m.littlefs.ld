/* PulSar ESP8266 Flash Split: 4M + ~2M FS
 *
 * Flash:  4 MB
 * Sketch: ~1019 KB
 * Gap:    ~1024 KB
 * FS:     ~2028 KB
 * EEPROM: 20 KB
 *
 * Layout:
 *   Sketch starts at 0x40201010
 *   Sketch ends at   0x40300000
 *   FS starts at     0x40400000
 *   FS ends at       0x405FB000
 *   EEPROM region follows, 20 KB
 *
 * Intended use:
 *   - NodeMCU / 4M ESP8266 validation
 *   - LittleFS
 *   - web editor
 *   - JSON config/template export
 */

MEMORY
{
  dport0_0_seg : org = 0x3FF00000, len = 0x10
  dram0_0_seg  : org = 0x3FFE8000, len = 0x14000
  iram1_0_seg  : org = 0x40100000, len = 0x8000
  irom0_0_seg  : org = 0x40201010, len = 0xFEFF0
}

/* ~2028 KB filesystem partition */
PROVIDE ( _SPIFFS_start = 0x40400000 );
PROVIDE ( _SPIFFS_end   = 0x405FB000 );
PROVIDE ( _SPIFFS_page  = 0x100 );
PROVIDE ( _SPIFFS_block = 0x2000 );

INCLUDE "C:\Users\micha\.platformio\packages\framework-arduinoespressif8266\tools\sdk\ld\eagle.flash.1m.ld"