#ifndef _MHBRIDGE_H
#define _MHBRIDGE_H 0.1

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_DRIVERS_HBRIDGE


#include "1_TaskerManager/mTaskerManager.h"


// Input	Output
// IA	IB	OA	IB	Description
// L	L 	L	L	    Off
// H	L	  H	L	    Forward
// L	H 	L	H	    Reverse
// H	H 	H	H	    Off
//We recommend using input 1A to control the speed 
//of each motor and input 1B to control the direction.

// #include "soc/soc.h"
// #include "soc/rtc_cntl_reg.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/adc.h"
// #include "esp_adc_cal.h"

// #define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
// #define NO_OF_SAMPLES   64          //Multisampling

// static esp_adc_cal_characteristics_t *adc_chars;
// static const adc_channel_t channel = ADC_CHANNEL_4;     //GPIO34 if ADC1, GPIO14 if ADC2
// static const adc_atten_t atten = ADC_ATTEN_DB_0;
// static const adc_unit_t unit = ADC_UNIT_1;

// #define ISACTIVE() !(digitalRead(BLINDS_MOTOR_IA_PIN)&&digitalRead(BLINDS_MOTOR_IB_PIN))  //boht low, motor off

#define D_TASKNAME_BLINDS "blinds"

class mHBridge{

  public:
    mHBridge(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);


//A-1A,A-1B
//B-1A,B-1B


    void Pre_Init(void);
    int8_t pin_A_IA = -1;
    int8_t pin_A_IB = -1;
    int8_t pin_B_IA = -1;
    int8_t pin_B_IB = -1;
    // int8_t pin_OA = -1;
    // int8_t pin_OB = -1;
    int8_t pin_AnalogSense = -1;
    int8_t init_success = false;


    typedef struct MOTOR_SETTINGS{
      int8_t fEnabled = false;


    };
    MOTOR_SETTINGS motorA;
    MOTOR_SETTINGS motorB;

    void MoveMotorPulse(uint8_t direction, uint16_t milliseconds);

    void SetStop();
    void SetMotorUp(uint8_t speed = 100);
    void SetMotorDown(uint8_t speed = 100);
    void SetBlindPosition(uint8_t percentage);
    void SendMQTT_Event(String key, String pair);

    // Used as complete maximums
    #define MOTOR_ADC_MAX_BOUNDARY_LOWER 2500
    #define MOTOR_ADC_MAX_BOUNDARY_UPPER 3500

    // To be adjusted when max and min openings are set, detect new value and update limit
    uint16_t motor_adc_boundary_lower = 3090;
    uint16_t motor_adc_boundary_upper = 3250;

    // #define READINGSTOTAL 1000
    // #define SAMPLE_PERIOD 1
    // #define BLINDS_ROC_LIMIT 1
    // #define BLINDS_PERCENTAGE_ACCURACY 5 // deviation from set point to stop at
    // struct ADCREADING{
    //   uint16_t index;
    //   uint16_t raw[READINGSTOTAL];
    //   uint32_t total;
    //   uint16_t average;
    //   uint32_t tSavedSampleTime;
    //   struct ROC{
    //     float val;
    //     float stored;
    //     uint32_t tSaved;
    //     uint32_t tSavedMQTTSent;
    //   }roc;
    // }adcreading;
    uint32_t tSavedTransitionTime;
    void init_ADC();
    void SubTasker_ADC_Measure();
    void UpdateBlindsADC_RefreshALLUntilComplete();
    float PositionPercentage();
    float UpdateBlindsROC();
    uint8_t GetBlindsROCExceeded();
    void UpdateBlindsADC();
    void SendMQTT_ROCExceeded();

    void SendMQTT_StatusPositionRapid();




    enum FAN_MODE_IDS{
      MANUAL=1,
      AUTO_TEMP,
      AUTO_REMOTE
    };



    uint8_t fan_mode = AUTO_TEMP;
    const char* FanModeCtr();

    uint32_t tTick,tCheckForMaxTemp,tSaved;

    void ConstructJSON_FanStatus();
    void MQQTSendFanStatus();
    //void parse_JSONCommand(mpkt_t* mpt, char* topic, char* payload);
    void parse_JSONCommand();

    void AddToJsonObject_AddHardware(JsonObject root);

};

#endif //DEVICE_RADIATORFAN

#endif
