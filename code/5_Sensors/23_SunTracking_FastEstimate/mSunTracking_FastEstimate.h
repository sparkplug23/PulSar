#ifndef MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_H
#define MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_H

#define D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_ID 5023

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE

#include "1_TaskerManager/mTaskerInterface.h"
#include <math.h>

class mSunTracking_FastEstimate :
public mTaskerInterface
{

    public:

    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSunTracking_FastEstimate(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(void);

    static constexpr const char* PM_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_CTR = "SunTracking_FastEstimate";
    PGM_P GetModuleName(){ return PM_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_ID; }

    struct ClassState
    {
    uint8_t devices = 0;
    uint8_t mode = ModuleStatus::Initialising;
    }module_state;


    /************************************************************************************************
     * SECTION: Solar Position
     ************************************************************************************************/
    struct SunPosition
    {
    float azimuth = 0.0f;
    float elevation = 0.0f;
    };

    struct SolarRuntime
    {
    SunPosition position;

    float max_elevation = 0.0f;
    float min_elevation = 0.0f;

    float sunrise_azimuth = 0.0f;
    float sunset_azimuth = 0.0f;
    float dawn_azimuth = 0.0f;
    float dusk_azimuth = 0.0f;

    uint32_t tUpdated_millis = 0;

    bool is_daytime = false;
    bool isvalid = false;
    }calc;


    /************************************************************************************************
     * SECTION: Daily Precalculated Table
     ************************************************************************************************/
    static constexpr uint8_t SUN_TABLE_POINTS = 25;

    /*
    * 25 points:
    *
    * [0]  = 00:00
    * [1]  = 01:00
    * ...
    * [23] = 23:00
    * [24] = 24:00
    *
    * Point 24 allows interpolation during 23:00-23:59 without a special case.
    */
    struct SolarDayTable
    {
    SunPosition hour[SUN_TABLE_POINTS];

    float max_elevation = -90.0f;
    float min_elevation = 90.0f;

    float sunrise_azimuth = 0.0f;
    float sunset_azimuth = 0.0f;
    float dawn_azimuth = 0.0f;
    float dusk_azimuth = 0.0f;

    float solar_noon_minutes = 0.0f;
    float sunrise_minutes = 0.0f;
    float sunset_minutes = 0.0f;
    float dawn_minutes = 0.0f;
    float dusk_minutes = 0.0f;

    uint16_t day_of_year = 0;

    bool valid = false;
    };


    /************************************************************************************************
     * SECTION: Daily Build Scheduler
     ************************************************************************************************/
    enum class BuildStage : uint8_t
    {
    IDLE = 0,
    PREPARE,
    CALCULATE_EVENTS,
    BUILD_POINTS,
    COMMIT
    };

    struct SolarBuildState
    {
    BuildStage stage = BuildStage::IDLE;

    uint8_t point_index = 0;
    uint32_t next_step_millis = 0;
    uint16_t day_of_year = 0;

    float latitude_deg = 0.0f;
    float longitude_deg = 0.0f;
    float timezone_hours = 0.0f;

    /*
    * Values calculated once for the entire day.
    */
    float equation_of_time_minutes = 0.0f;
    float declination_rad = 0.0f;
    float latitude_rad = 0.0f;

    float sin_latitude = 0.0f;
    float cos_latitude = 0.0f;

    float sin_declination = 0.0f;
    float cos_declination = 0.0f;
    float tan_declination = 0.0f;

    bool requested = false;
    }build;


    /************************************************************************************************
     * SECTION: Public API
     ************************************************************************************************/
    float Get_Azimuth() const{ return calc.position.azimuth; }
    float Get_Elevation() const{ return calc.position.elevation; }
    float Get_Elevation_Max() const{ return calc.max_elevation; }
    float Get_Elevation_Min() const{ return calc.min_elevation; }
    bool Valid() const{ return calc.isvalid; }
    bool IsDaytime() const{ return calc.is_daytime; }
    const SolarDayTable& GetDayTable() const{ return table_active; }


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
    void ScheduleDailyRebuild(void);
    void ProcessDailyRebuild(void);
    void PrepareDailyCalculation(void);
    void CalculateTablePoint(uint8_t point_index);
    void CommitDailyTable(void);
    void UpdateInterpolatedPosition(void);
    void CalculateDailyEvents(void);
    SunPosition CalculatePositionAtLocalMinutes(float local_minutes);


    private:

    /*
    * Active table is never modified while a new table is being generated.
    */
    SolarDayTable table_active;

    /*
    * Temporary build table.
    */
    SolarDayTable table_build;

    /*
    * Delay between expensive build operations.
    *
    * 50 ms × 25 points gives roughly 1.25 seconds to generate a day,
    * but the work is distributed across normal loop execution.
    */
    static constexpr uint32_t BUILD_STEP_BACKOFF_MS = 50;

    uint32_t LocalTime(void);
    uint32_t UtcTime(void);
    float GetTimezoneOffsetHours(void);
    static float WrapDegrees360(float angle);
    static float InterpolateAzimuth(float start, float end, float fraction);

};

#endif // USE_MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE

#endif // MODULE_SENSORS_SUN_TRACKING_FAST_ESTIMATE_H