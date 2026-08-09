#include "mSunTracking_FastEstimate.h"


#ifdef USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE


/************************************************************************************************
 * SECTION: Tasker
 ************************************************************************************************/

int8_t mSunTracking_FastEstimate::Tasker(
    uint8_t function,
    JsonParserObject obj
)
{

    /*
     * Initialization tasks must be allowed before module_state becomes Running.
     */
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


    if (module_state.mode != ModuleStatus::Running)
    {
        return TASKER_RESULT__MODULE_DISABLED_ID;
    }


    switch (function)
    {

        /*
         * The staged table builder is deliberately serviced from TASK_LOOP.
         *
         * ScheduleDailyRebuild() itself performs no expensive calculation.
         */
        case TASK_LOOP:
        {
            ProcessDailyRebuild();
        }
        break;


        /*
         * Runtime solar position only needs updated once per second.
         *
         * LED effects can read calc.position as often as they want without
         * causing any astronomical calculation.
         */
        case TASK_EVERY_SECOND:
        {
            UpdateInterpolatedPosition();


            /*
             * Defensive fallback:
             *
             * TASK_EVERY_MIDNIGHT should normally request this. However,
             * if the midnight event is missed, detect a day change here.
             */
            if (
                build.stage == BuildStage::IDLE &&
                tkr_time->RtcTime.valid
            )
            {
                const uint16_t current_day =
                    tkr_time->RtcTime.day_of_year;

                if (
                    !table_active.valid ||
                    table_active.day_of_year != current_day
                )
                {
                    ScheduleDailyRebuild();
                }
            }
        }
        break;


        /*
         * Midnight performs NO astronomy.
         *
         * It merely requests a table rebuild. TASK_LOOP then spreads the
         * calculation across multiple loop iterations.
         */
        case TASK_EVERY_MIDNIGHT:
        {
            ScheduleDailyRebuild();
        }
        break;

    }


    return 1;
}


/************************************************************************************************
 * SECTION: Init
 ************************************************************************************************/

void mSunTracking_FastEstimate::Pre_Init()
{
    module_state.mode = ModuleStatus::Initialising;
}


void mSunTracking_FastEstimate::Init()
{
    calc = SolarRuntime();

    table_active = SolarDayTable();
    table_build  = SolarDayTable();
    build        = SolarBuildState();


    module_state.mode = ModuleStatus::Running;


    /*
     * Do not calculate anything expensive here.
     *
     * Request the first table and allow TASK_LOOP to generate it.
     */
    ScheduleDailyRebuild();
}


void mSunTracking_FastEstimate::BootMessage()
{
    ALOG_IMP(
        PSTR("SunTracking FastEstimate: daily table interpolation")
    );
}


/************************************************************************************************
 * SECTION: Time Access
 ************************************************************************************************/

uint32_t mSunTracking_FastEstimate::LocalTime()
{
    return tkr_time->LocalTime();
}


uint32_t mSunTracking_FastEstimate::UtcTime()
{
    return tkr_time->UtcTime();
}


/**
 * @brief Determine the currently active local UTC offset.
 *
 * This automatically includes DST because it compares LocalTime against UTC.
 */
float mSunTracking_FastEstimate::GetTimezoneOffsetHours()
{
    int32_t local_time = (int32_t)LocalTime();
    int32_t utc_time   = (int32_t)UtcTime();

    int32_t difference = local_time - utc_time;


    /*
     * Defensive correction if the implementation represents local time
     * wrapped around a day boundary.
     */
    if (difference > 43200)
    {
        difference -= 86400;
    }
    else
    if (difference < -43200)
    {
        difference += 86400;
    }


    return ((float)difference / 3600.0f);
}


/************************************************************************************************
 * SECTION: Daily Build Scheduler
 ************************************************************************************************/

void mSunTracking_FastEstimate::ScheduleDailyRebuild()
{
    /*
     * This function must remain extremely cheap.
     *
     * It does not perform any solar calculation.
     */

    if (!tkr_time->RtcTime.valid)
    {
        return;
    }


    /*
     * If already building, do not repeatedly restart the build.
     */
    if (build.stage != BuildStage::IDLE)
    {
        return;
    }


    build.requested = true;

    build.stage = BuildStage::PREPARE;

    build.point_index = 0;

    build.next_step_millis =
        millis() + BUILD_STEP_BACKOFF_MS;
}


/************************************************************************************************
 * SECTION: Staged Daily Builder
 ************************************************************************************************/

void mSunTracking_FastEstimate::ProcessDailyRebuild()
{
    if (build.stage == BuildStage::IDLE)
    {
        return;
    }


    const uint32_t now = millis();


    /*
     * millis() wrap-safe deadline comparison.
     */
    if ((int32_t)(now - build.next_step_millis) < 0)
    {
        return;
    }


    switch (build.stage)
    {

        /********************************************************************************************
         * Stage 1:
         *
         * Capture location/time and calculate values which remain effectively
         * constant throughout this entire day.
         ********************************************************************************************/
        case BuildStage::PREPARE:
        {
            PrepareDailyCalculation();

            build.point_index = 0;

            build.stage = BuildStage::BUILD_POINTS;

            build.next_step_millis =
                now + BUILD_STEP_BACKOFF_MS;
        }
        break;


        /********************************************************************************************
         * Stage 2:
         *
         * Calculate ONE hourly table point per scheduler pass.
         ********************************************************************************************/
        case BuildStage::BUILD_POINTS:
        {
            CalculateTablePoint(
                build.point_index
            );


            build.point_index++;


            if (build.point_index >= SUN_TABLE_POINTS)
            {
                build.stage = BuildStage::COMMIT;
            }


            build.next_step_millis =
                now + BUILD_STEP_BACKOFF_MS;
        }
        break;


        /********************************************************************************************
         * Stage 3:
         *
         * Atomically replace the active table after the entire new table exists.
         ********************************************************************************************/
        case BuildStage::COMMIT:
        {
            CommitDailyTable();

            build.stage = BuildStage::IDLE;

            build.requested = false;

            build.next_step_millis = 0;
        }
        break;


        default:
        {
            build.stage = BuildStage::IDLE;
        }
        break;
    }
}


/************************************************************************************************
 * SECTION: Daily Solar Constants
 ************************************************************************************************/

void mSunTracking_FastEstimate::PrepareDailyCalculation()
{
    /*
     * Start with a completely fresh build table.
     */
    table_build = SolarDayTable();


    build.day_of_year =
        tkr_time->RtcTime.day_of_year;


    /*
     * Existing full SunTracking already gets system location from here.
     */
    build.latitude_deg =
        tkr_iSensors->system_location.latitude;

    build.longitude_deg =
        tkr_iSensors->system_location.longitude;


    build.timezone_hours =
        GetTimezoneOffsetHours();


    table_build.day_of_year =
        build.day_of_year;


    /********************************************************************************************
     * Fractional year.
     *
     * Only evaluated once per day.
     ********************************************************************************************/

    const float gamma =
        TWO_PI *
        ((float)(build.day_of_year - 1) / 365.0f);


    /********************************************************************************************
     * Equation of time.
     *
     * Result is in minutes.
     ********************************************************************************************/

    build.equation_of_time_minutes =
        229.18f *
        (
             0.000075f
           + 0.001868f * cosf(gamma)
           - 0.032077f * sinf(gamma)
           - 0.014615f * cosf(2.0f * gamma)
           - 0.040849f * sinf(2.0f * gamma)
        );


    /********************************************************************************************
     * Solar declination.
     *
     * Result remains in radians.
     ********************************************************************************************/

    build.declination_rad =
          0.006918f
        - 0.399912f * cosf(gamma)
        + 0.070257f * sinf(gamma)
        - 0.006758f * cosf(2.0f * gamma)
        + 0.000907f * sinf(2.0f * gamma)
        - 0.002697f * cosf(3.0f * gamma)
        + 0.001480f * sinf(3.0f * gamma);


    build.latitude_rad =
        build.latitude_deg * DEG_TO_RAD;


    /*
     * Cache all values which would otherwise be recalculated for
     * every hourly position.
     */
    build.sin_latitude =
        sinf(build.latitude_rad);

    build.cos_latitude =
        cosf(build.latitude_rad);

    build.sin_declination =
        sinf(build.declination_rad);

    build.cos_declination =
        cosf(build.declination_rad);

    build.tan_declination =
        tanf(build.declination_rad);


    table_build.max_elevation = -90.0f;
    table_build.min_elevation =  90.0f;
}


/************************************************************************************************
 * SECTION: Hourly Position Calculation
 ************************************************************************************************/

void mSunTracking_FastEstimate::CalculateTablePoint(
    uint8_t point_index
)
{
    if (point_index >= SUN_TABLE_POINTS)
    {
        return;
    }


    /*
     * point 0  = 00:00
     * point 12 = 12:00
     * point 24 = 24:00
     */
    const float local_clock_minutes =
        ((float)point_index * 60.0f);


    /********************************************************************************************
     * Solar time correction.
     *
     * longitude:
     *      east positive
     *      west negative
     *
     * timezone:
     *      UTC+1 => +1
     ********************************************************************************************/

    const float time_correction_minutes =
          build.equation_of_time_minutes
        + (4.0f * build.longitude_deg)
        - (60.0f * build.timezone_hours);


    float solar_minutes =
        local_clock_minutes +
        time_correction_minutes;


    /*
     * Keep within a sensible day-sized range.
     */
    while (solar_minutes < 0.0f)
    {
        solar_minutes += 1440.0f;
    }

    while (solar_minutes >= 1440.0f)
    {
        solar_minutes -= 1440.0f;
    }


    /********************************************************************************************
     * Hour angle.
     *
     * Solar noon = 0 degrees.
     * Morning    = negative.
     * Afternoon  = positive.
     ********************************************************************************************/

    const float hour_angle_deg =
        (solar_minutes / 4.0f) - 180.0f;


    const float hour_angle_rad =
        hour_angle_deg * DEG_TO_RAD;


    const float sin_hour_angle =
        sinf(hour_angle_rad);

    const float cos_hour_angle =
        cosf(hour_angle_rad);


    /********************************************************************************************
     * Elevation
     ********************************************************************************************/

    float sin_elevation =
          (build.sin_latitude * build.sin_declination)
        + (build.cos_latitude *
           build.cos_declination *
           cos_hour_angle);


    /*
     * asin safety against floating point rounding.
     */
    if (sin_elevation > 1.0f)
    {
        sin_elevation = 1.0f;
    }
    else
    if (sin_elevation < -1.0f)
    {
        sin_elevation = -1.0f;
    }


    const float elevation_rad =
        asinf(sin_elevation);


    const float elevation_deg =
        elevation_rad * RAD_TO_DEG;


    /********************************************************************************************
     * Azimuth
     *
     * Output convention:
     *
     *      0   North
     *      90  East
     *      180 South
     *      270 West
     ********************************************************************************************/

    float azimuth_deg =
        atan2f(
            sin_hour_angle,

            (cos_hour_angle * build.sin_latitude)
            -
            (build.tan_declination * build.cos_latitude)
        )
        * RAD_TO_DEG;


    azimuth_deg += 180.0f;

    azimuth_deg =
        WrapDegrees360(azimuth_deg);


    /********************************************************************************************
     * Store
     ********************************************************************************************/

    table_build.hour[point_index].azimuth =
        azimuth_deg;

    table_build.hour[point_index].elevation =
        elevation_deg;


    if (elevation_deg > table_build.max_elevation)
    {
        table_build.max_elevation =
            elevation_deg;
    }


    if (elevation_deg < table_build.min_elevation)
    {
        table_build.min_elevation =
            elevation_deg;
    }
}


/************************************************************************************************
 * SECTION: Commit Daily Table
 ************************************************************************************************/

void mSunTracking_FastEstimate::CommitDailyTable()
{
    table_build.valid = true;


    /*
     * Copy the completed table only after all 25 points exist.
     *
     * Until this point the previous day's active table remained untouched.
     */
    table_active = table_build;


    calc.max_elevation =
        table_active.max_elevation;

    calc.min_elevation =
        table_active.min_elevation;


    /*
     * Immediately update current position from the newly committed table.
     */
    UpdateInterpolatedPosition();


    ALOG_DBM(
        PSTR(
            "SunFast table complete day=%d max=%d min=%d"
        ),
        table_active.day_of_year,
        (int)table_active.max_elevation,
        (int)table_active.min_elevation
    );
}


/************************************************************************************************
 * SECTION: Runtime Interpolation
 ************************************************************************************************/

void mSunTracking_FastEstimate::UpdateInterpolatedPosition()
{
    if (!table_active.valid)
    {
        calc.isvalid = false;
        return;
    }


    const uint32_t local_time =
        LocalTime();


    const uint32_t seconds_today =
        local_time % 86400UL;


    /*
     * 0..23
     */
    uint8_t current_hour =
        seconds_today / 3600UL;


    if (current_hour > 23)
    {
        current_hour = 23;
    }


    /*
     * Position within the hour.
     *
     * Range:
     *      0.0 -> exactly on current hour
     *      1.0 -> approaching next hour
     */
    const uint32_t seconds_into_hour =
        seconds_today % 3600UL;


    const float fraction =
        (float)seconds_into_hour / 3600.0f;


    const SunPosition& start =
        table_active.hour[current_hour];

    const SunPosition& end =
        table_active.hour[current_hour + 1];


    /********************************************************************************************
     * Elevation can use ordinary interpolation.
     ********************************************************************************************/

    calc.position.elevation =
        start.elevation +
        ((end.elevation - start.elevation) * fraction);


    /********************************************************************************************
     * Azimuth requires wrap-aware interpolation.
     *
     * Example:
     *
     *      359 -> 1
     *
     * must travel +2 degrees, not -358 degrees.
     ********************************************************************************************/

    calc.position.azimuth =
        InterpolateAzimuth(
            start.azimuth,
            end.azimuth,
            fraction
        );


    calc.max_elevation =
        table_active.max_elevation;

    calc.min_elevation =
        table_active.min_elevation;


    calc.is_daytime =
        (calc.position.elevation > 0.0f);


    calc.tUpdated_millis =
        millis();


    calc.isvalid = true;
}


/************************************************************************************************
 * SECTION: Helpers
 ************************************************************************************************/

float mSunTracking_FastEstimate::WrapDegrees360(
    float angle
)
{
    while (angle >= 360.0f)
    {
        angle -= 360.0f;
    }


    while (angle < 0.0f)
    {
        angle += 360.0f;
    }


    return angle;
}


float mSunTracking_FastEstimate::InterpolateAzimuth(
    float start,
    float end,
    float fraction
)
{
    float difference =
        end - start;


    /*
     * Always interpolate using the shortest direction around the circle.
     */
    if (difference > 180.0f)
    {
        difference -= 360.0f;
    }
    else
    if (difference < -180.0f)
    {
        difference += 360.0f;
    }


    return WrapDegrees360(
        start + (difference * fraction)
    );
}


#endif // USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE