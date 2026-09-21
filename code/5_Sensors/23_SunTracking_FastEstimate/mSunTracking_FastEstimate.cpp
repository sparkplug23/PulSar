#include "mSunTracking_FastEstimate.h"

#ifdef USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE


/************************************************************************************************
 * SECTION: Tasker
 ************************************************************************************************/
int8_t mSunTracking_FastEstimate::Tasker(uint8_t function, JsonParserObject obj)
{
  switch (function)
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

  if (module_state.mode != ModuleStatus::Running) return TASKER_RESULT__MODULE_DISABLED_ID;

  switch (function)
  {
    case TASK_LOOP:
      ProcessDailyRebuild();
    break;

    case TASK_EVERY_SECOND:
    {
      UpdateInterpolatedPosition();

      /*
       * Defensive fallback for missed midnight, boot after midnight or
       * time synchronization changing the calendar date.
       */
      if (build.stage == BuildStage::IDLE && tkr_time->RtcTime.valid)
      {
        const uint16_t current_day = tkr_time->RtcTime.day_of_year;
        if (!table_active.valid || table_active.day_of_year != current_day) ScheduleDailyRebuild();
      }
    }
    break;

    case TASK_EVERY_MIDNIGHT:
      ScheduleDailyRebuild();
    break;
  }

  return 1;
}


/************************************************************************************************
 * SECTION: Init
 ************************************************************************************************/
void mSunTracking_FastEstimate::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mSunTracking_FastEstimate::Init(void)
{
  calc = SolarRuntime();
  table_active = SolarDayTable();
  table_build = SolarDayTable();
  build = SolarBuildState();

  module_state.mode = ModuleStatus::Running;

  /*
   * Do not perform the complete calculation here.
   * Just request the first table.
   */
  ScheduleDailyRebuild();
}


void mSunTracking_FastEstimate::BootMessage(void)
{
  ALOG_IMP(PSTR("SunTracking FastEstimate: daily precalculated table"));
}


/************************************************************************************************
 * SECTION: Time Access
 ************************************************************************************************/
uint32_t mSunTracking_FastEstimate::LocalTime(void)
{
  return tkr_time->LocalTime();
}


uint32_t mSunTracking_FastEstimate::UtcTime(void)
{
  return tkr_time->UtcTime();
}


/**
 * @brief Return current local UTC offset in hours.
 *
 * LocalTime already contains the current timezone/DST adjustment.
 * Comparing LocalTime against UtcTime provides the offset needed to convert
 * solar time back into local clock time.
 */
float mSunTracking_FastEstimate::GetTimezoneOffsetHours(void)
{
  int32_t local_time = (int32_t)LocalTime();
  int32_t utc_time = (int32_t)UtcTime();
  int32_t difference = local_time - utc_time;

  if (difference > 43200) difference -= 86400;
  else if (difference < -43200) difference += 86400;

  return (float)difference / 3600.0f;
}


/************************************************************************************************
 * SECTION: Daily Build Scheduler
 ************************************************************************************************/
void mSunTracking_FastEstimate::ScheduleDailyRebuild(void)
{
  if (!tkr_time->RtcTime.valid) return;
  if (build.stage != BuildStage::IDLE) return;

  build.requested = true;
  build.stage = BuildStage::PREPARE;
  build.point_index = 0;
  build.next_step_millis = millis() + BUILD_STEP_BACKOFF_MS;
}


/************************************************************************************************
 * SECTION: Staged Daily Builder
 ************************************************************************************************/
void mSunTracking_FastEstimate::ProcessDailyRebuild(void)
{
  if (build.stage == BuildStage::IDLE) return;

  const uint32_t now = millis();

  /*
   * Wrap-safe millis deadline test.
   */
  if ((int32_t)(now - build.next_step_millis) < 0) return;

  switch (build.stage)
  {
    case BuildStage::PREPARE:
      PrepareDailyCalculation();
      build.stage = BuildStage::CALCULATE_EVENTS;
      build.next_step_millis = now + BUILD_STEP_BACKOFF_MS;
    break;

    case BuildStage::CALCULATE_EVENTS:
      CalculateDailyEvents();
      build.point_index = 0;
      build.stage = BuildStage::BUILD_POINTS;
      build.next_step_millis = now + BUILD_STEP_BACKOFF_MS;
    break;

    case BuildStage::BUILD_POINTS:
      CalculateTablePoint(build.point_index);
      build.point_index++;

      if (build.point_index >= SUN_TABLE_POINTS) build.stage = BuildStage::COMMIT;

      build.next_step_millis = now + BUILD_STEP_BACKOFF_MS;
    break;

    case BuildStage::COMMIT:
      CommitDailyTable();
      build.stage = BuildStage::IDLE;
      build.requested = false;
      build.next_step_millis = 0;
    break;

    default:
      build.stage = BuildStage::IDLE;
      build.requested = false;
    break;
  }
}


/************************************************************************************************
 * SECTION: Daily Solar Constants
 ************************************************************************************************/
void mSunTracking_FastEstimate::PrepareDailyCalculation(void)
{
  table_build = SolarDayTable();

  build.day_of_year = tkr_time->RtcTime.day_of_year;
  table_build.day_of_year = build.day_of_year;

  build.latitude_deg = tkr_iSensors->system_location.latitude;
  build.longitude_deg = tkr_iSensors->system_location.longitude;
  build.timezone_hours = GetTimezoneOffsetHours();

  /*
   * Fractional year.
   * Fast estimate only needs this once per day.
   */
  const float gamma = TWO_PI * ((float)(build.day_of_year - 1) / 365.0f);

  /*
   * Equation of Time, minutes.
   */
  build.equation_of_time_minutes =
    229.18f *
    (
        0.000075f
      + 0.001868f * cosf(gamma)
      - 0.032077f * sinf(gamma)
      - 0.014615f * cosf(2.0f * gamma)
      - 0.040849f * sinf(2.0f * gamma)
    );

  /*
   * Solar declination, radians.
   */
  build.declination_rad =
      0.006918f
    - 0.399912f * cosf(gamma)
    + 0.070257f * sinf(gamma)
    - 0.006758f * cosf(2.0f * gamma)
    + 0.000907f * sinf(2.0f * gamma)
    - 0.002697f * cosf(3.0f * gamma)
    + 0.001480f * sinf(3.0f * gamma);

  build.latitude_rad = build.latitude_deg * DEG_TO_RAD;

  build.sin_latitude = sinf(build.latitude_rad);
  build.cos_latitude = cosf(build.latitude_rad);

  build.sin_declination = sinf(build.declination_rad);
  build.cos_declination = cosf(build.declination_rad);
  build.tan_declination = tanf(build.declination_rad);

  table_build.max_elevation = -90.0f;
  table_build.min_elevation = 90.0f;
}


/************************************************************************************************
 * SECTION: Daily Solar Events
 ************************************************************************************************/
void mSunTracking_FastEstimate::CalculateDailyEvents(void)
{
  constexpr float SUNRISE_ELEVATION_DEG = -0.833f;
  constexpr float CIVIL_TWILIGHT_ELEVATION_DEG = -6.0f;

  auto CalculateHourAngleForElevation = [&](float elevation_deg) -> float
  {
    const float elevation_rad = elevation_deg * DEG_TO_RAD;

    float cos_hour_angle =
      (
        sinf(elevation_rad) -
        (build.sin_latitude * build.sin_declination)
      )
      /
      (build.cos_latitude * build.cos_declination);

    if (cos_hour_angle > 1.0f) cos_hour_angle = 1.0f;
    else if (cos_hour_angle < -1.0f) cos_hour_angle = -1.0f;

    return acosf(cos_hour_angle) * RAD_TO_DEG;
  };

  const float sunrise_hour_angle_deg = CalculateHourAngleForElevation(SUNRISE_ELEVATION_DEG);
  const float twilight_hour_angle_deg = CalculateHourAngleForElevation(CIVIL_TWILIGHT_ELEVATION_DEG);

  /*
   * true solar time =
   * local clock minutes + equation of time + 4*longitude - 60*timezone
   */
  const float time_correction_minutes =
      build.equation_of_time_minutes
    + (4.0f * build.longitude_deg)
    - (60.0f * build.timezone_hours);

  /*
   * True solar noon occurs at 720 solar minutes.
   */
  table_build.solar_noon_minutes = 720.0f - time_correction_minutes;

  /*
   * 15 degrees/hour = 4 minutes per degree.
   */
  table_build.sunrise_minutes = table_build.solar_noon_minutes - (sunrise_hour_angle_deg * 4.0f);
  table_build.sunset_minutes = table_build.solar_noon_minutes + (sunrise_hour_angle_deg * 4.0f);
  table_build.dawn_minutes = table_build.solar_noon_minutes - (twilight_hour_angle_deg * 4.0f);
  table_build.dusk_minutes = table_build.solar_noon_minutes + (twilight_hour_angle_deg * 4.0f);

  table_build.sunrise_azimuth = CalculatePositionAtLocalMinutes(table_build.sunrise_minutes).azimuth;
  table_build.sunset_azimuth = CalculatePositionAtLocalMinutes(table_build.sunset_minutes).azimuth;
  table_build.dawn_azimuth = CalculatePositionAtLocalMinutes(table_build.dawn_minutes).azimuth;
  table_build.dusk_azimuth = CalculatePositionAtLocalMinutes(table_build.dusk_minutes).azimuth;
}


/************************************************************************************************
 * SECTION: Hourly Position Calculation
 ************************************************************************************************/
void mSunTracking_FastEstimate::CalculateTablePoint(uint8_t point_index)
{
  if (point_index >= SUN_TABLE_POINTS) return;

  /*
   * point 0  = 00:00
   * point 1  = 01:00
   * ...
   * point 24 = 24:00
   */
  const float local_minutes = (float)point_index * 60.0f;
  const SunPosition position = CalculatePositionAtLocalMinutes(local_minutes);

  table_build.hour[point_index] = position;

  if (position.elevation > table_build.max_elevation) table_build.max_elevation = position.elevation;
  if (position.elevation < table_build.min_elevation) table_build.min_elevation = position.elevation;
}


/************************************************************************************************
 * SECTION: Position Calculation
 ************************************************************************************************/
mSunTracking_FastEstimate::SunPosition mSunTracking_FastEstimate::CalculatePositionAtLocalMinutes(float local_minutes)
{
  SunPosition position;

  const float time_correction_minutes =
      build.equation_of_time_minutes
    + (4.0f * build.longitude_deg)
    - (60.0f * build.timezone_hours);

  float solar_minutes = local_minutes + time_correction_minutes;

  while (solar_minutes < 0.0f) solar_minutes += 1440.0f;
  while (solar_minutes >= 1440.0f) solar_minutes -= 1440.0f;

  /*
   * Solar noon = 0 degrees.
   * Morning = negative.
   * Afternoon = positive.
   */
  const float hour_angle_deg = (solar_minutes / 4.0f) - 180.0f;
  const float hour_angle_rad = hour_angle_deg * DEG_TO_RAD;

  const float sin_hour_angle = sinf(hour_angle_rad);
  const float cos_hour_angle = cosf(hour_angle_rad);

  float sin_elevation =
      (build.sin_latitude * build.sin_declination)
    + (build.cos_latitude * build.cos_declination * cos_hour_angle);

  if (sin_elevation > 1.0f) sin_elevation = 1.0f;
  else if (sin_elevation < -1.0f) sin_elevation = -1.0f;

  const float elevation_rad = asinf(sin_elevation);
  position.elevation = elevation_rad * RAD_TO_DEG;

  /*
   * Azimuth convention:
   * 0   = North
   * 90  = East
   * 180 = South
   * 270 = West
   */
  const float azimuth =
    atan2f(
      sin_hour_angle,
      (cos_hour_angle * build.sin_latitude) - (build.tan_declination * build.cos_latitude)
    ) * RAD_TO_DEG;

  position.azimuth = WrapDegrees360(azimuth + 180.0f);

  return position;
}


/************************************************************************************************
 * SECTION: Commit Daily Table
 ************************************************************************************************/
void mSunTracking_FastEstimate::CommitDailyTable(void)
{
  table_build.valid = true;

  /*
   * Only replace the active table once the complete new day exists.
   */
  table_active = table_build;

  calc.max_elevation = table_active.max_elevation;
  calc.min_elevation = table_active.min_elevation;

  calc.sunrise_azimuth = table_active.sunrise_azimuth;
  calc.sunset_azimuth = table_active.sunset_azimuth;
  calc.dawn_azimuth = table_active.dawn_azimuth;
  calc.dusk_azimuth = table_active.dusk_azimuth;

  /*
   * Immediately update current position against the newly committed table.
   */
  UpdateInterpolatedPosition();

  ALOG_DBM(
    PSTR("SunFast table day=%d max=%d min=%d az dawn/rise/set/dusk=%d/%d/%d/%d"),
    table_active.day_of_year,
    (int)table_active.max_elevation,
    (int)table_active.min_elevation,
    (int)table_active.dawn_azimuth,
    (int)table_active.sunrise_azimuth,
    (int)table_active.sunset_azimuth,
    (int)table_active.dusk_azimuth
  );
}


/************************************************************************************************
 * SECTION: Runtime Interpolation
 ************************************************************************************************/
void mSunTracking_FastEstimate::UpdateInterpolatedPosition(void)
{
  if (!table_active.valid)
  {
    calc.isvalid = false;
    return;
  }

  const uint32_t local_time = LocalTime();
  const uint32_t seconds_today = local_time % 86400UL;

  uint8_t current_hour = seconds_today / 3600UL;
  if (current_hour > 23) current_hour = 23;

  const uint32_t seconds_into_hour = seconds_today % 3600UL;
  const float fraction = (float)seconds_into_hour / 3600.0f;

  const SunPosition& start = table_active.hour[current_hour];
  const SunPosition& end = table_active.hour[current_hour + 1];

  calc.position.elevation = start.elevation + ((end.elevation - start.elevation) * fraction);
  calc.position.azimuth = InterpolateAzimuth(start.azimuth, end.azimuth, fraction);

  calc.max_elevation = table_active.max_elevation;
  calc.min_elevation = table_active.min_elevation;

  calc.is_daytime = calc.position.elevation > 0.0f;
  calc.tUpdated_millis = millis();
  calc.isvalid = true;
}


/************************************************************************************************
 * SECTION: Helpers
 ************************************************************************************************/
float mSunTracking_FastEstimate::WrapDegrees360(float angle)
{
  while (angle >= 360.0f) angle -= 360.0f;
  while (angle < 0.0f) angle += 360.0f;
  return angle;
}


/************************************************************************************************
 * SECTION: Azimuth Interpolation
 ************************************************************************************************/
float mSunTracking_FastEstimate::InterpolateAzimuth(float start, float end, float fraction)
{
  float difference = end - start;

  /*
   * 359 -> 1 should interpolate +2 degrees rather than -358 degrees.
   */
  if (difference > 180.0f) difference -= 360.0f;
  else if (difference < -180.0f) difference += 360.0f;

  return WrapDegrees360(start + (difference * fraction));
}


#endif // USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE