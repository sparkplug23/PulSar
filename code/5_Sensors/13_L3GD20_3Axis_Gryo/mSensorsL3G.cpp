#include "mSensorsL3G.h"

#ifdef USE_MODULE_SENSORS_L3G // L3GD20

int8_t mSensorsL3G::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;   
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return TASKER_RESULT__SUCCESS_ID;

}


void mSensorsL3G::Pre_Init()
{

  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  // in futre use array to store L3G type found (L3G_280_ID, L3G_180_ID) etc
  // if(tkr_pins->PinUsed(GPIO_I2C_SCL_ID) && tkr_pins->PinUsed(GPIO_I2C_SDA_ID)){

  // if(tkr_sup->I2cDevice(0x76) || tkr_sup->I2cDevice(0x77)){

    // Wire = new TwoWire();//tkr_pins->GetPin(GPIO_I2C_SCL_ID),tkr_pins->GetPin(GPIO_I2C_SDA_ID));
  
    // sensor[settings.fSensorCount].L3G = new Adafruit_L3G280();
    // if (sensor[settings.fSensorCount].L3G->begin(0x77, tkr_i2c->wire)) {
    //   ALOG_INF(PSTR(D_LOG_L3G "L3G280 sensor detected"));// Serial.flush();
    //   settings.fSensorCount++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_L3G "L3G280 sensor not detected"));
    // }

  // Mine were LSM - 0x1D, L3G - 0x6B, BMP - 0x77
  gyro = new L3G(tkr_i2c->wire, 0);

}


void mSensorsL3G::Init(void)
{

  if (gyro->init())
  {
    gyro->enableDefault();
    ALOG_INF(PSTR( "L3G sensor detected"));// Serial.flush();
    module_state.devices++;
  }  

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "L3G Sensor Enabled"));
  }

  settings.measure_rate_ms = 60000;
  
}


void mSensorsL3G::EveryLoop()
{
  
  if(mTime::TimeReachedNonReset(&tSavedMeasure,settings.measure_rate_ms))
  {
    // ReadSensor();
    
    gyro->read();

    //   #ifdef ENABLE_SENSOR_LSM303D_READING_AVERAGING
    // sensor.mag.average.x->Add(compass->m.x);
    // sensor.mag.average.y->Add(compass->m.y);
    // sensor.mag.average.z->Add(compass->m.z);
    //   #endif // ENABLE_SENSOR_LSM303D_READING_AVERAGING

    sensor.gyro.instant.x = gyro->g.x;
    sensor.gyro.instant.y = gyro->g.y;
    sensor.gyro.instant.z = gyro->g.z;

  }
  
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSensorsL3G::parse_JSONCommand(JsonParserObject obj)
{

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsL3G::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, GetSensorCount());
  return JBI->End();

}

uint8_t mSensorsL3G::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
  
    JBI->Object_Start("Gyro");
      JBI->Object_Start("Instant");
        JBI->Add("x", sensor.gyro.instant.x);
        JBI->Add("y", sensor.gyro.instant.y);
        JBI->Add("z", sensor.gyro.instant.z);
      JBI->Object_End();
    JBI->Object_End();


  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsL3G::MQTTHandler_Init(){

  struct handler<mSensorsL3G>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsL3G::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT


#endif
