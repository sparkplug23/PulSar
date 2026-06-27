
#pragma once

#include <stdint.h>


typedef struct event_motion_s{
  uint8_t  state     = false;
  uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  bool isActiveLow = true;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  uint32_t detected_time;
  struct devove_name_t{
    uint16_t unique_module_id = 0;  // unique, not "em_##_id"
    uint8_t device_id = 0;
  }device_name;
}event_motion_t;


/**
 * @brief Unified system location state.
 *
 * This struct represents the currently selected/best known location for the
 * whole device. Individual GPS/GNSS drivers should keep their own detailed
 * source-specific state locally, then publish a common subset into this struct
 * when their fix is accepted by the sensor/interface layer.
 */
typedef struct sensorset_location_s
{
  // -------------------------------------------------------------------------
  // Validity / source
  // -------------------------------------------------------------------------
  bool     isvalid          = false;  // True only when this contains an accepted fix
  uint8_t  source_id        = 0;      // Source enum/id, e.g. SIM7000G, UBLOX, manual, unknown
  uint8_t  fix_type         = 0;      // Generic fix type, 0=none, 1=2D, 2=3D, source-mapped
  uint8_t  fix_quality      = 0;      // Optional generic quality score, 0=unknown/poor

  // -------------------------------------------------------------------------
  // Timing
  // -------------------------------------------------------------------------
  uint32_t updated_millis   = 0;      // millis() when this struct was last updated
  uint32_t fix_age_ms       = 0;      // Age of fix when accepted, 0 if live/unknown

  uint16_t year             = 0;
  uint8_t  month            = 0;
  uint8_t  day              = 0;
  uint8_t  hour             = 0;
  uint8_t  minute           = 0;
  uint8_t  second           = 0;
  uint32_t utc_time_secs = 0;  // Unix UTC epoch seconds from GNSS date/time
  bool     time_valid       = false;  // True if date/time fields are valid GNSS/network time

  // -------------------------------------------------------------------------
  // Position
  // -------------------------------------------------------------------------
  double   latitude         = 0.0;    // Degrees
  double   longitude        = 0.0;    // Degrees
  float    altitude         = 0.0f;   // Metres above mean sea level, source-dependent

  // -------------------------------------------------------------------------
  // Motion
  // -------------------------------------------------------------------------
  float    speed            = 0.0f;   // Usually km/h from TinyGSM/SIM7000G, document per source
  float    course           = 0.0f;   // Degrees, 0=N, 90=E

  // -------------------------------------------------------------------------
  // Accuracy / dilution
  // -------------------------------------------------------------------------
  float    accuracy         = 0.0f;   // Horizontal accuracy estimate, metres if known
  float    accuracy_vertical= 0.0f;   // Vertical accuracy estimate, metres if known
  float    hdop             = 0.0f;   // Horizontal dilution of precision, 0 if unknown
  float    vdop             = 0.0f;   // Vertical dilution of precision, 0 if unknown
  float    pdop             = 0.0f;   // Position dilution of precision, 0 if unknown  
  
  // -------------------------------------------------------------------------
  // Source-specific accuracy / precision extensions
  // -------------------------------------------------------------------------
  float    accuracy_position  = 0.0f; // Position accuracy estimate, source-dependent
  int16_t  hpa_raw            = 0;    // Horizontal position accuracy/raw precision from modem
  int16_t  vpa_raw            = 0;    // Vertical position accuracy/raw precision from modem

  // -------------------------------------------------------------------------
  // Satellite summary
  // -------------------------------------------------------------------------
  uint8_t  satellites_used  = 0;      // Satellites used in fix
  uint8_t  satellites_view  = 0;      // Satellites visible/in view
  uint8_t  satellites_gps   = 0;      // Optional source-mapped GPS count
  uint8_t  satellites_glonass = 0;    // Optional source-mapped GLONASS count
  uint8_t  cno_max          = 0;      // Max carrier-to-noise ratio if source provides it

  // -------------------------------------------------------------------------
  // Internal acceptance metadata
  // -------------------------------------------------------------------------
  bool     is_stale         = true;   // Set by interface if updated_millis exceeds timeout
  uint8_t  priority         = 0;      // Source priority used when selecting system location

} sensorset_location_t;

enum location_source_id_t : uint8_t
{
  LOCATION_SOURCE_UNKNOWN       = 0,
  LOCATION_SOURCE_SIM7000G_GNSS = 1,
  LOCATION_SOURCE_SIM800L       = 2,
  LOCATION_SOURCE_UBLOX_SERIAL  = 3,
  LOCATION_SOURCE_MANUAL        = 4,
  LOCATION_SOURCE_NETWORK       = 5
};