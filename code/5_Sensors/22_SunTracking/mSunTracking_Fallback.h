#ifndef MODULE_SENSORS_SUN_TRACKING_SIMULATED_H2
#define MODULE_SENSORS_SUN_TRACKING_SIMULATED_H2

/**
 * High performance minimal particular for the palettes in lighting. 
 * This using time of day to create estimated sun positions suitable for LIVE PALETTES.
 **/

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE2
#include "3_Network/10_MQTT/mMQTT.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mSunTracking :
  public mTaskerInterface
{
  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSunTracking(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0)
    {
        switch(function)
        {
            case TASK_EVERY_10_SECONDS:
                Update_Solar_Tracking_Data();
            break;
        }
        return 1;
    };
    void BootMessage();

    static constexpr const char* PM_MODULE_SENSORS_SUN_TRACKING_CTR = D_MODULE_SENSORS_SUN_TRACKING_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SUN_TRACKING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID; }  

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Running; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    uint32_t LocalTime(); // Only function in cpp to access mTime

    struct SolarEventTimes 
    {
      float azimuth;
      float elevation;      
      uint32_t tUpdated_millis = 0;
      bool isvalid = false;
    }calc;

    #define ELEVATION_MAX_RANGE  45.0f
    #define ELEVATION_MIN_RANGE -45.0f

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
    int32_t Get_SolarTimeShiftSeconds_FromLongitude(float longitude_deg)
    {
        // 360 degrees = 24 hours = 86400 seconds → 1 degree = 240 seconds
        // Positive = East of Greenwich → solar noon is earlier
        // Negative = West of Greenwich → solar noon is later
        return (int32_t)(longitude_deg * 240.0f);
    }
    
    /**
     * IMPORTANT: These are based on LocalTime, which include Daylight Savings 
     **/
    #define SUNRISE_HOUR 6.0f   // Define sunrise as 6:00 AM
    #define SUNSET_HOUR  21.0f  // Define sunset as 6:00 PM

    void Update_Solar_Tracking_Data()
    {
        // Serial.println("Tracking");

        constexpr float ELEVATION_RANGE = (ELEVATION_MAX_RANGE - ELEVATION_MIN_RANGE);
        constexpr float DAY_SECONDS     = 86400.0f;

        // Step 1: Get time of day (already in local time)
        uint32_t tod_seconds = LocalTime() % 86400;

        // Step 2: No solar shift applied (already in local time)
        uint32_t shifted_tod = tod_seconds;

        // Step 3: Convert sunrise/sunset to seconds
        const float sunrise_sec     = SUNRISE_HOUR * 3600.0f;
        const float sunset_sec      = SUNSET_HOUR  * 3600.0f;
        const float solar_noon_sec  = 0.5f * (sunrise_sec + sunset_sec);

        // Step 4: Calculate day_fraction centered on solar noon
        float day_fraction = (shifted_tod - solar_noon_sec) / DAY_SECONDS; // [-0.5 .. +0.5]
        float angle_rad    = 2.0f * PI * day_fraction;
        float elevation    = ELEVATION_RANGE * sinf(angle_rad) * 0.5f + ELEVATION_MIN_RANGE;

        // Step 5: Full-circle azimuth: 0–360° across the day
        float azimuth = fmodf((shifted_tod / DAY_SECONDS) * 360.0f, 360.0f);

        // Step 6: Store result
        calc.azimuth          = azimuth;
        calc.elevation        = elevation;
        calc.tUpdated_millis  = millis();
        calc.isvalid          = true;

        // Serial.println(shifted_tod);
        // Serial.println(tod_seconds);
        // Serial.println(calc.azimuth);
        // Serial.println(calc.elevation);
    }

    float Get_Azimuth()
    {
        return calc.azimuth;
    }
    float Get_Elevation()
    {
        return calc.elevation;
    }

    int Get_Elevation_Max(){
        return ELEVATION_MAX_RANGE;
    }
    int Get_Elevation_Min(){
        return ELEVATION_MIN_RANGE;
    }

    bool Valid()
    {
      return calc.isvalid;
    }

};

#endif // USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE

#endif // HEADER GUARD
