#ifndef VERSION_H
#define VERSION_H

#include "stdint.h"


enum FIRMWARE_VERSION_BRANCH_TYPE_IDS{
  FIRMWARE_VERSION_TYPE_RELEASE_ID = 0,  
  FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID,
  FIRMWARE_VERSION_TYPE_BETA_ID,  
  FIRMWARE_VERSION_TYPE_DEVELOPING_ID
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Firmware Version //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FIRMWARE_VERSION_TYPE     FIRMWARE_VERSION_TYPE_DEVELOPING_ID

#define FIRMWARE_VERSION_MAJOR    0 // Reserved for webserver working, settings saving, and being able to export (as json AND bytes) that settings

/**@@@@@ Minor Changes - Aim for Quartely Milestones : Try describe development in this stage, when increased, assumed previous was done.
 * #136 : [26v1] Created 13June26. {alreadydone: Bitpack gpio, templates, mqtt, all networks, huge rewrite which may need certain flags working. Sd card, filesystem, settings, also all redone. Pin mapping in general redone}
 * #135 : [26Q2] Created 4April26. Worked on: Addlog, Network connections and mqtt switched to multi-access structure. MQTT can latch on to any, and its its job to handle that. Filesystem, Settings, SDCard (inside Filesystem).
 * #134 : Created Feb26. Fixed RTC leak causing crashing on solar, new panic_handler added for postcrash decoding
 * #133 : New testbeds created for permenant structured developed.
 * #132 : [26Q1] 24Dec25 Live palettes refactored. New Wifi2, webserver. three html build scripts (lights,webserver,submodules). Ethernet, LTE, SMS. Lights will be either complete/beta/2d only, add new defines within these, not the user config. RMT added back in (eg coneH). Most esp32 versions (s3,c3, working). crash_report and add2line parsing.
 * #131 : Oct25 In Colorado
 * #130 : Aug25 Moving towards FIRMWARE_DEFAULT__LIGHTING_CONFIG__## and merging longterm working lighting options. From now on, unless testing a new feature inside FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA (where it, then inside it will be a test ifdef to keep it centralised) all lighting will be defined in the ini, and use FIRMWARE_DEFAULT__LIGHTING_CONFIG__## to define the lighting config. 
 * #129 : Jun25 Esp32, c3, s3, pin mapping templates updated
 * #128 : May25 SuperMiniC3 compiled, updating to allow new esp32 variants.
 * #127 : Jan25 Switched to RgbwwColor, removed Rgbcct and reverted to latest unedited neopixelbus. Added new dynamic colourbuffer to segments.
 * #126 : During Colorado
 * #125 : Rework of MQTT
 * #124 : 11Aug24, optimising starting and partial NPB update but using older methods with double buffer.
 * #122 : April 2024, lighting refactor and 2D
 * #121 : Jan 2024, starting with LTE tracing
 * #120 : XMAS 2023
 * #119 : Updated to latest neopixelbus library, and added support for 8/16 pin parallel output.
 * #118 : Nextion support added. TFT Upload via WebUI Async Webserver.
 * #117 : Introduction of first lighting testgroup (ie 6 candidate template default options to provide new users)
 * #116 : Multipin support on I2S (dual neopixel) and PWM. Introduction of TestGroups.
 * #115 : Improvements from networks and lighting merged into dev_main. dev_main will be primary focus of developing until LTE and multipin are stable.
 * #114 : Cleaned up mqtt topic for interface_## to keep all interface types together. Requires updating path.
 * #113 : Main development branch for 2023
 * #112 : After Colorado, still the lighting only branch
 * #111 : Working on lighting as its own branch. Removing any WLED palette usage until my palatte works!
 * #110 : Before colorado 2022
 */
#define FIRMWARE_VERSION_MINOR    136 // Update "ChangeLogManual.md" when incrementing

/**@@@@@ Core Changes
 * #00 : 
 */
#define FIRMWARE_VERSION_CORE     5

/**@@@@@ Module changes
 * #00 : 
 * In the future, if core remains 0, this can be used to iter one level up.
 */
#define FIRMWARE_VERSION_MODULE   0


#define FIRMWARE_BRANCH_NAME_STRING "development"


// Project version is bit-packed as follows
// bits 31-30  (2)  (Range 0-4)     Type/Branch (e.g. Stable, Developing)
// bits 29-25  (6)  (Range 0-77)    Major version 
// bits 24-16  (8)  (Range 0-255)   Minor version 
// bits 15-8   (8)  (Range 0-255)   System version (e.g. Wifi, Support)
// bits 7-0    (8)  (Range 0-255)   Module version (e.g. Sensors, Drivers)


// Generate Version AFTER Hardware defaults have been called, this allows undef VERSION_TYPE for overriding
const uint32_t PROJECT_VERSION         = ((FIRMWARE_VERSION_TYPE & 0x03) << 30) | ((FIRMWARE_VERSION_MAJOR & 0x3F) << 24) | (FIRMWARE_VERSION_MINOR << 16) | (FIRMWARE_VERSION_CORE << 8) | (FIRMWARE_VERSION_MODULE);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// OPTIONAL DEPRECIATION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #define ENABLE_FEATURE_FIRMWARE_VERSION__INCLUDE_MIN_MAX_VERSIONING

/*********** EXAMPLE
 *
#if FIRMWARE_VERSION_MAX(0, 140)
  // deprecated code still allowed
#else
  #error "Deprecated code expired — must be removed"
#endif
*
*/

// Pack MAJOR.MINOR into a single comparable integer (preprocessor-safe)
#define FW_VER_PACK(major, minor) \
  (((major) << 16) | ((minor) & 0xFFFF))

#define FW_VER_CURRENT \
  FW_VER_PACK(FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR)

#ifdef ENABLE_FEATURE_FIRMWARE_VERSION__INCLUDE_MIN_MAX_VERSIONING
  #define FIRMWARE_VERSION_MIN(major, minor) \
    (FW_VER_CURRENT >= FW_VER_PACK((major),(minor)))
  #define FIRMWARE_VERSION_MAX(major, minor) \
    (FW_VER_CURRENT <= FW_VER_PACK((major),(minor)))
#else
  // Version gating disabled → always include
  #define FIRMWARE_VERSION_MIN(major, minor)  (1)
  #define FIRMWARE_VERSION_MAX(major, minor)  (1)
#endif

#define FIRMWARE_VERSION_IN_RANGE(minMaj,minMin,maxMaj,maxMin) \
  (FIRMWARE_VERSION_MIN((minMaj),(minMin)) && \
   FIRMWARE_VERSION_MAX((maxMaj),(maxMin)))


#endif


