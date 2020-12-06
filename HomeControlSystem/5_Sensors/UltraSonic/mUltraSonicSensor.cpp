#include "mUltraSonicSensor.h"

#ifdef USE_MODULE_SENSORS_ULTRASONICS

void mUltraSonicSensor::Pre_Init(){ 

  if(pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID) && pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID)){
    pin_trig = pCONT_pins->GetPin(GPIO_SR04_TRIG_ID);
    pin_echo = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);
    pinMode(pin_trig, OUTPUT); // Sets the trigPin as an Output
    pinMode(pin_echo, INPUT); // Sets the echoPin as an Input
    settings.flags.EnableSensor = true;
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d"),pin_trig);
    settings.flags.EnableSensor = false;
  }

}


void mUltraSonicSensor::Init(void){

  memset(&averaged,0,sizeof(averaged));

  ultrasonic.threshold.narrowpercent = 2; // 5% percent of norminal
  ultrasonic.threshold.widepercent = 50; // 5% percent of norminal

  object_detected_static.tSavedCheck = millis();

  sprintf(presence_detect.detected_rtc_ctr,"--:--:--\0");
          
}

int mUltraSonicSensor::GetDurationReading(void){

  // If sensor is invalid, use shorter delay to read again (but not too fast)
  
  #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
  int blockingtime = ultrasonic.settings.blocking_time_ms;
  if(!ultrasonic.isvalid){
    blockingtime = ultrasonic.settings.blocking_time_ms*4; //retry 2 times longer is previous sample was wrong
  }

  // Return early
  if((abs(millis()-ultrasonic.tUltraSonicSensorReadLast)<blockingtime)){ // if not valid try again right away
   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "ultrasonic.tUltraSonicSensorReadLast<blockingtime"));
    return ultrasonic.duration;
  }
  #endif //   #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ULTRASONIC "mUltraSonicSensor::GetDurationReading"));

  float duration=0;

  // Clears the trigPin
  // digitalWrite(pin_trig, LOW);
  // delayMicroseconds(2);
  // Sets the ULTRA_TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(pin_echo, HIGH);//, 4500);//, ultrasonic.settings.duration_limit_max); //10000us 10ms //default 1 second timeout
  ultrasonic.duration_raw = duration;

  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "duration=%d"),(int)duration);

  #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
  //if outside possible range
  if((duration>ultrasonic.settings.duration_limit_min)&&(duration<ultrasonic.settings.duration_limit_max)){ 
    
    //pCONT->mso->MessagePrintln("[ULTRA] SAMPLING");
    
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "INSIDE DURATION"));

    // float lower = (float)ultrasonic.duration*(1-(ultrasonic.threshold.narrowpercent/100.0));//0.9;
    // float upper = (float)ultrasonic.duration*(1+(ultrasonic.threshold.narrowpercent/100.0));
    // ultrasonic.threshold.lowervalue = lower;
    // ultrasonic.threshold.uppervalue = upper;

    // // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "Lower<duration|Duration<Upper: [\t%d\t%d\t%d\t%d]"),(int)lower,(int)ultrasonic.duration,(int)duration,(int)upper);

    // // New method under test
    // // GET how new duration relates to previous
    // if(duration>ultrasonic.duration){ // Positive range
    //   ultrasonic.threshold.ratio_pos = duration/ultrasonic.threshold.uppervalue;
    //   ultrasonic.threshold.relative = ultrasonic.threshold.ratio_pos;  
    //   // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "duration > ultrasonic.duration"));
    // }else{ // Negative range
    //   ultrasonic.threshold.ratio_neg = -1*(ultrasonic.threshold.lowervalue/duration);
    //   ultrasonic.threshold.relative = ultrasonic.threshold.ratio_neg; 
    //   // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "duration < ultrasonic.duration"));
    // }

    // // Check if its within threshold
    // if(fabsf(ultrasonic.threshold.relative)>1){ // outside range
    //   ultrasonic.threshold.outsidecount++;
    //   ultrasonic.threshold.insidecount=0;
    // }else 
    // if(fabsf(ultrasonic.threshold.relative)<=1){ // inside range
    //   ultrasonic.threshold.insidecount++;
    //   ultrasonic.threshold.outsidecount=0;
    // }

    // // Adjust threshold limits if need be
    // if(ultrasonic.threshold.outsidecount>10){
    //   // Reduce set threshold if we can
    //   if(ultrasonic.threshold.setpercent < ultrasonic.threshold.widepercent){
    //     ultrasonic.threshold.setpercent+=5; // +10% // increase width faster to resolve errors
    //   }else{
    //     ultrasonic.threshold.setpercent = ultrasonic.threshold.widepercent;
    //   }
    //   //ultrasonic.threshold.outsidecount = 10; // freeze it at limit
    // }
    // if(ultrasonic.threshold.insidecount>10){
    //   // Reduce set threshold if we can
    //   if(ultrasonic.threshold.setpercent > ultrasonic.threshold.narrowpercent){
    //     ultrasonic.threshold.setpercent--; //-1% train in slowly
    //   }else{
    //     ultrasonic.threshold.setpercent = ultrasonic.threshold.narrowpercent;
    //   }
    //   //ultrasonic.threshold.insidecount = 10
    // }

    // //FORCED LIMITS
    // if((ultrasonic.threshold.setpercent>ultrasonic.threshold.widepercent)||(ultrasonic.threshold.setpercent<ultrasonic.threshold.narrowpercent)){ultrasonic.threshold.setpercent=20;}
    // if(ultrasonic.threshold.insidecount>200){ultrasonic.threshold.insidecount=200;}
    // if(ultrasonic.threshold.outsidecount>200){ultrasonic.threshold.outsidecount=200;}

    // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "fabsf(ultrasonic.threshold.relative) %f"),fabsf(ultrasonic.threshold.relative));

    // if(fabsf(ultrasonic.threshold.relative)<=1){ 
    //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "[ULTRA]if(abs(ultrasonic.threshold.relative)<=1) \t\t"));
      ultrasonic.isvalid = true;
    //   ultrasonic.ischanged = true;
    //   ultrasonic.accuracy.insidecount++;
    // }else{
    //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "[ULTRA] NOT if(abs(ultrasonic.threshold.relative)<=1)"));
    //   ultrasonic.isvalid = false;
    //   ultrasonic.ischanged = false;
    //   ultrasonic.accuracy.outsidecount++;
    // }

    ultrasonic.duration = duration; //update stored
    ultrasonic.tUltraSonicSensorReadLast = millis();

    // float total = ultrasonic.accuracy.insidecount+ultrasonic.accuracy.outsidecount;
    // if(total!=0){ // bottom of fraction reduces to only inside
    //   ultrasonic.accuracy.percent = (ultrasonic.accuracy.insidecount/total)*100;
    // }

    return ultrasonic.duration;
  }else{ // ==0 no response

  
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "OUTSIDE DURATION %d"),duration);
  
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "[ULTRA] Outside viable range"));
    //pCONT->mso->MessagePrintln("[ULTRA] Outside viable range");
    ultrasonic.isvalid = false;
    ultrasonic.ischanged = false;
    ultrasonic.accuracy.outsidecount++;

    
// //debug testing
//     ultrasonic.isvalid = true;
//     ultrasonic.ischanged = true;


    return ultrasonic.duration;
  }

  #else
  
    ultrasonic.duration = duration; //update stored
    ultrasonic.tUltraSonicSensorReadLast = millis();
    ultrasonic.isvalid = true;
    ultrasonic.ischanged = true;
    ultrasonic.accuracy.insidecount++;

  #endif // ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

}

float mUltraSonicSensor::GetDistanceCMReading(void){
  return ultrasonic.duration*(0.034/2);
}
float mUltraSonicSensor::GetDistanceMMReading(void){
  return ultrasonic.duration*(0.34/2);
}


// Instead of using 343m/s for speed of sound at 20 degrees celcius, if valid,
// use the tank temperature sensor, to get speed of sound, else return 343
float mUltraSonicSensor::GetSpeedOfSoundInMetres(){

    float speedofsound_inmps = 0;

    #ifdef USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
        #ifdef USE_MODULE_SENSORS_DS18B20
        int tempsensorid = -1;
        float ambient_temperature;
        if((tempsensorid=pCONT_set->GetDeviceIDbyName("SpeedOfSound_Ambient",0,(int8_t)D_MODULE_SENSORS_DB18S20_ID))>=0){
            if(pCONT_msdb18->sensor[tempsensorid].reading.isvalid){
            ambient_temperature = pCONT_msdb18->sensor[tempsensorid].reading.val;
            
          AddLog_P(LOG_LEVEL_ERROR, PSTR("ambient_temperature=%d"),(int)ambient_temperature);
            // Reduce frequency of updates to stop data jumps
            if(abs(millis()-ultrasonic.tPermitTempUpdate)>60000){ultrasonic.tPermitTempUpdate = millis();
                ultrasonic.temperature = ambient_temperature;
            }
            // float insidesqrt = (ultrasonic.temperature/273.15)+1;
            // speedofsound_inmps = 331.227599 * sqrt(insidesqrt);
            speedofsound_inmps = 331.3 + (0.606 * ultrasonic.temperature); //https://en.wikipedia.org/wiki/Speed_of_sound
            }
        }else{
          AddLog_P(LOG_LEVEL_ERROR, PSTR("SOS missing"));
        }
        #endif
    #endif

    // Use default
    if(speedofsound_inmps==0){
      speedofsound_inmps = 343;
      #ifdef USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_ULTRASONIC "speedofsound_inmps = 343"));
      #endif
    }

    ultrasonic.speedofsound = speedofsound_inmps;
    
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("speedofsound=%d"),(int)ultrasonic.speedofsound);

    return speedofsound_inmps;

}

void mUltraSonicSensor::AddPresenceEventStatusSum(uint8_t status){
  presence_event_status_sum.arr[presence_event_status_sum.idx++] = status;
  if(presence_event_status_sum.idx > PRESENCE_EVENT_STATUS_SUM_MAX){presence_event_status_sum.idx=0;}
}
uint8_t mUltraSonicSensor::GetPresenceEventStatusSum(){
  uint8_t sum = 0;
  for(uint8_t ii=0;ii<PRESENCE_EVENT_STATUS_SUM_MAX;ii++){
    sum += presence_event_status_sum.arr[ii]; 
  }
  if((sum == PRESENCE_EVENT_STATUS_SUM_MAX) || (sum == 0)){
    return 1; // return true if all are the same ie all events are the same
  }
  return 0;
}





float mUltraSonicSensor::GetDistanceMMReadingAdjustedForTemp(void){

  //The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
  float speedofsound_inmetres = GetSpeedOfSoundInMetres();//@20 degrees cerr    0.34;

  //float speedofsound_inmillimetres = (speedofsound_inmetres*1000)/1000000; *1000 from m->mm, divide by 1E6 for Us
  float speedofsound_inmillimetres_permicroseconds = (speedofsound_inmetres)/1000;

  float distance_mm= (float)ultrasonic.duration*(speedofsound_inmillimetres_permicroseconds/2);
  return distance_mm;
}


float mUltraSonicSensor::GetDistanceMMReadingAdjustedForTemp(int duration){

  //The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
  float speedofsound_inmetres = GetSpeedOfSoundInMetres();//@20 degrees cerr    0.34;

  //stored_speedofsound_inmetres = speedofsound_inmetres;
  //Serial.print("[ULTRA] speedofsound_inmetres>> "); Serial.println(speedofsound_inmetres);
  //float speedofsound_inmillimetres = (speedofsound_inmetres*1000)/1000000; *1000 from m->mm, divide by 1E6 for Us
  float speedofsound_inmillimetres_permicroseconds = (speedofsound_inmetres)/1000;

  //Serial.print("[ULTRA] speedofsound_inmillimetres_permicroseconds>> "); Serial.println(speedofsound_inmillimetres_permicroseconds);
  //float sos = GetSpeedOfSoundInMetres();

  float distance_mm= (float)duration*(speedofsound_inmillimetres_permicroseconds/2);
  return distance_mm;
}


float mUltraSonicSensor::GetDistanceCMReadingAdjustedForTemp(void){
  return GetDistanceMMReadingAdjustedForTemp()/10;
}


// Will measure the sensors in 3 ways
// 1 exact value (instantaneous)
// X times per tick (fast smoothing)
// X times capturing 1 per tick (slow smoothing over time)
void mUltraSonicSensor::SubTask_UltraSonicAverage(){

  // 1 SECOND
  if(abs(millis()-averaged.instant.captured.tSaved)>1000){averaged.instant.captured.tSaved=millis();
    averaged.instant.final.distance_mm = GetDistanceMMReadingAdjustedForTemp(); //pin32

    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_ULTRASONIC "distance_mm=%d"),(int)averaged.instant.final.distance_mm);
    #endif // ENABLE_LOG_LEVEL_DEBUG

    averaged.instant.final.distance_cm = averaged.instant.final.distance_mm/10; //pin32
    averaged.instant.isvalid = true;
    averaged.instant.captured.tLastChanged = millis();
  }

  // 1 MINUTE
  averaged.ptr = &averaged.smooth_1m;
  if(abs(millis()-averaged.ptr->captured.tSaved)>1000){averaged.ptr->captured.tSaved=millis(); //ONE MINUTE

    // Store new reading
    int temp = 0;
    averaged.ptr->captured.readings[averaged.ptr->captured.readIndex++] = GetDistanceMMReadingAdjustedForTemp();

    // Handle index
    if(averaged.ptr->captured.readIndex >= ADCSENSORS_SMOOTHSLOW_NUMREADINGS){averaged.ptr->captured.readIndex = 0; averaged.ptr->isvalid = true;}
    // Calculate original average
    averaged.ptr->captured.total=0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / ADCSENSORS_SMOOTHSLOW_NUMREADINGS;  // calculate the average:
    // Calculate deviation of original readings from average: deviation = |value-average|
    averaged.ptr->averaging.deviationarray[ADCSENSORS_SMOOTHSLOW_NUMREADINGS];
    averaged.ptr->averaging.deviationsum = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      averaged.ptr->averaging.deviationarray[ii] = abs(averaged.ptr->captured.readings[ii]-averaged.ptr->captured.average);
      averaged.ptr->averaging.deviationsum += averaged.ptr->averaging.deviationarray[ii];
    }
    // calculate the average deviation: averageDeviation = (sum of deviation)/number
    averaged.ptr->averaging.deviationaverage = averaged.ptr->averaging.deviationsum / ADCSENSORS_SMOOTHSLOW_NUMREADINGS;
    // Discard any outliers that have high deviations from original average: deviation > 2*averageDeviation
    averaged.ptr->averaging.isoutlier[ADCSENSORS_SMOOTHSLOW_NUMREADINGS];
    averaged.ptr->averaging.outliercount = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      if(averaged.ptr->averaging.deviationarray[ii]>1*averaged.ptr->averaging.deviationaverage){ //if an outlier
        averaged.ptr->averaging.isoutlier[ii] = 1;
        averaged.ptr->averaging.outliercount++;
      }else{
        averaged.ptr->averaging.isoutlier[ii] = 0;
      }
    }
    // Recalculate new average without outliers
    averaged.ptr->averaging.usablecount = 0;
    averaged.ptr->captured.total = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      if(!averaged.ptr->averaging.isoutlier[ii]){
        averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
        averaged.ptr->averaging.usablecount++;
      }
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / averaged.ptr->averaging.usablecount;  // calculate the average:
    averaged.ptr->averaging.usableratio = (averaged.ptr->averaging.usablecount/ADCSENSORS_SMOOTHSLOW_NUMREADINGS)*100;

    // Set average to invalid if avergae deviation is too large
    // if(averaged.ptr->averaging.deviationaverage<10){
    //   averaged.ptr->isvalid = true;
    // }else{
    //   averaged.ptr->isvalid = false;
    // }

    //store
    averaged.ptr->final.distance_mm = averaged.ptr->captured.average;
    averaged.ptr->final.distance_cm = averaged.ptr->captured.average/10;
    averaged.ptr->ischanged = true;

    averaged.ptr->captured.tLastChanged = millis();

  }

  // 1 HOUR
  averaged.ptr = &averaged.smooth_1hr;
  if(abs(millis()-averaged.ptr->captured.tSaved)>60000){averaged.ptr->captured.tSaved=millis(); //ONE MINUTE

    // Store new reading
    averaged.ptr->captured.readings[averaged.ptr->captured.readIndex++] = GetDistanceMMReadingAdjustedForTemp();

    // Handle index
    if(averaged.ptr->captured.readIndex >= ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS){averaged.ptr->captured.readIndex = 0; averaged.ptr->isvalid = true;}
    // Calculate original average
    averaged.ptr->captured.total=0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;  // calculate the average:
    // Calculate deviation of original readings from average: deviation = |value-average|
    averaged.ptr->averaging.deviationarray[ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS];
    averaged.ptr->averaging.deviationsum = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      averaged.ptr->averaging.deviationarray[ii] = abs(averaged.ptr->captured.readings[ii]-averaged.ptr->captured.average);
      averaged.ptr->averaging.deviationsum += averaged.ptr->averaging.deviationarray[ii];
    }
    // calculate the average deviation: averageDeviation = (sum of deviation)/number
    averaged.ptr->averaging.deviationaverage = averaged.ptr->averaging.deviationsum / ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;
    // Discard any outliers that have high deviations from original average: deviation > 2*averageDeviation
    averaged.ptr->averaging.isoutlier[ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS];
    averaged.ptr->averaging.outliercount = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      if(averaged.ptr->averaging.deviationarray[ii]>1*averaged.ptr->averaging.deviationaverage){ //if an outlier
        averaged.ptr->averaging.isoutlier[ii] = 1;
        averaged.ptr->averaging.outliercount++;
      }else{
        averaged.ptr->averaging.isoutlier[ii] = 0;
      }
    }
    // Recalculate new average without outliers
    averaged.ptr->averaging.usablecount = 0;
    averaged.ptr->captured.total = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      if(!averaged.ptr->averaging.isoutlier[ii]){
        averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
        averaged.ptr->averaging.usablecount++;
      }
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / averaged.ptr->averaging.usablecount;  // calculate the average:
    averaged.ptr->averaging.usableratio = (averaged.ptr->averaging.usablecount/ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS)*100;

    // Set average to invalid if avergae deviation is too large
    // if(averaged.ptr->averaging.deviationaverage<10){
    //   averaged.ptr->isvalid = true;
    // }else{
    //   averaged.ptr->isvalid = false;
    // }

    //store
    averaged.ptr->final.distance_mm = averaged.ptr->captured.average;
    averaged.ptr->final.distance_cm = averaged.ptr->captured.average/10;
    averaged.ptr->ischanged = true;

    averaged.ptr->captured.tLastChanged = millis();

  }

} // END function averaging




void mUltraSonicSensor::MQQTSendObjectDetected(void){

  if(presence_detect.ischanged){ presence_detect.ischanged=false;

    D_DATA_BUFFER_CLEAR();

    StaticJsonDocument<300> doc;
    JsonObject root = doc.to<JsonObject>();
    #ifdef MOTIONALERT_PAYLOAD1_CTR
      root["location"] = MOTIONALERT_PAYLOAD1_CTR;
    #endif
    root["time"] = pCONT->mt->mtime.hhmmss_ctr;

    data_buffer.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer.payload.ctr);

    if(presence_detect.isactive){
      pCONT_mqtt->ppublish("status/presence/detected",data_buffer.payload.ctr,false);
    }else{
      pCONT_mqtt->ppublish("status/presence/over",data_buffer.payload.ctr,false);
    }

  }

}



int8_t mUltraSonicSensor::Tasker(uint8_t function){

  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }
  
  if(!settings.flags.EnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  
  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
            
      if(mTime::TimeReachedNonReset(&ultrasonic.tReadLast,1000)){//ultrasonic.settings.measure_rate_ms)){
        GetDurationReading(); 
        
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_ULTRASONIC "duration = %d %dms"),ultrasonic.duration,ultrasonic.settings.measure_rate_ms);
        
        if(ultrasonic.isvalid){ // stop trying
          ultrasonic.tReadLast = millis();
          fUpdateCalculations = true;
         }
      }

      if(ultrasonic.isvalid){
        SubTask_UltraSonicAverage();
        pCONT->Tasker_Interface(FUNC_SENSOR_UPDATED); // Tell other dependent modules we have changed
      }

      SubTask_DetectMotion();
         
    break;
    case FUNC_EVERY_SECOND:
    
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break; 
    case FUNC_MQTT_SENDER:  
      MQTTHandler_Sender();
    break;

    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    #endif //USE_MODULE_CORE_WEBSERVER
  }

} // END Tasker


void mUltraSonicSensor::WebAppend_Root_Status_Table_Draw(){
  
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Distance");
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Event");
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);

}


void mUltraSonicSensor::WebAppend_Root_Status_Table_Data(){
  
  char float_ctr[10];

  JsonBuilderI->Array_Start("tab_ult");// Class name
  for(int row=0;row<2;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        default:
        case 0:
          dtostrf(GetDistanceCMReading()/100,3,2,float_ctr);
          JsonBuilderI->Add_FP("ih",PSTR("\"%s m\""),float_ctr);
        break;
        case 1: JsonBuilderI->Add_P("ih",presence_detect.detected_rtc_ctr); break;
      }  
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();

}


void mUltraSonicSensor::SubTask_DetectMotion(){
  
  #ifdef DEVICE_LIVINGROOMSENSOR
    //Alerts if sensor value changes from previous
    if(mTime::TimeReached(&object_detected_static.tSavedCheck,10000)){
      float distancecm = GetDistanceCMReading();
      
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "distancecm=%d"),(int)distancecm);
      // in limits
      // if(WITHINLIMITS(object_detected_static.trigger_cm_min,
      //                 distancecm,
      //                 object_detected_static.trigger_cm_max)){
      if(distancecm<250){
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "object_detected_static \"%s\""),"within");
        object_detected_static.ispresent = true;
        AddPresenceEventStatusSum(true);
      }else{
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "object_detected_static \"%s\""),"outside");
        object_detected_static.ispresent = false;
        AddPresenceEventStatusSum(false);
      }

      // if(!GetPresenceEventStatusSum()){ // If consecutive events are the same, then return true
      //   return;
      // }
      
      if(presence_detect.state!=object_detected_static.ispresent){
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "IF presence_detect"));
        pCONT_mqtt->ppublish("status/presence/event",object_detected_static.ispresent?"Present":"Not Present",false);
        presence_detect.state = object_detected_static.ispresent;
        presence_detect.tDetectTime = millis();

        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        
        if(presence_detect.state){ 
          presence_detect.isactive = true;
          // presence_detect.wasactive = false; //toggle as "previous state"
          memcpy(presence_detect.detected_rtc_ctr,pCONT->mt->mtime.hhmmss_ctr,sizeof(pCONT->mt->mtime.hhmmss_ctr));
        }else{
          presence_detect.isactive = false;
          // presence_detect.wasactive = true; //toggle as "previous state"
        }

        // If this and previous state where the same
        // if(presence_detect.wasactive ++ presence_detect.wasactive){

        // }



        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "presence_detect"));
        presence_detect.ischanged = true;
      }else{
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "ELSE presence_detect"));

      }

    }
  #endif

}




void mUltraSonicSensor::parse_JSONCommand(){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/ultrasonic")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return; // not meant for here
  } 

} // END FUNCTION


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mUltraSonicSensor::ConstructJSON_Settings(uint8_t json_level){

  JsonBuilderI->Start();  

    // // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();

  return JsonBuilderI->End();

}

uint8_t mUltraSonicSensor::ConstructJSON_Sensors(uint8_t json_level){

  JsonBuilderI->Start(); 
    JsonBuilderI->Level_Start(D_JSON_SENSOR);
      JsonBuilderI->Add(D_JSON_ISVALID, ultrasonic.isvalid);
      JsonBuilderI->Add(D_JSON_DURATION, ultrasonic.duration);
      JsonBuilderI->Add(D_JSON_DURATION_RAW, ultrasonic.duration_raw);
      JsonBuilderI->Add(D_JSON_TEMPERATURE, ultrasonic.temperature);
      JsonBuilderI->Add(D_JSON_TEMPERATURE "_Age", abs(millis()-ultrasonic.tPermitTempUpdate));
      JsonBuilderI->Add(D_JSON_SPEEDOFSOUND, ultrasonic.speedofsound);
      JsonBuilderI->Add(D_JSON_LASTREAD, abs(millis()-ultrasonic.tUltraSonicSensorReadLast));
      if(json_level >= JSON_LEVEL_DETAILED){
        JsonBuilderI->Level_Start(D_JSON_ACCURACY);
          JsonBuilderI->Add(D_JSON_INSIDE, ultrasonic.accuracy.insidecount);
          JsonBuilderI->Add(D_JSON_OUTSIDE, ultrasonic.accuracy.outsidecount);
          JsonBuilderI->Add(D_JSON_PERCENTAGE, ultrasonic.accuracy.percent);
        JsonBuilderI->Level_End(); // D_JSON_ACCURACY
        JsonBuilderI->Level_Start_P(  PSTR(D_JSON_THRESHOLD));
          JsonBuilderI->Add_P(          PSTR(D_JSON_SET     D_JSON_PERCENT), ultrasonic.threshold.setpercent);
          JsonBuilderI->Add_P(          PSTR(D_JSON_NARROW  D_JSON_PERCENT), ultrasonic.threshold.narrowpercent);
          JsonBuilderI->Add_P(          PSTR(D_JSON_WIDE    D_JSON_PERCENT), ultrasonic.threshold.widepercent);
          JsonBuilderI->Add_P(          PSTR(D_JSON_LOWER   D_JSON_VALUE),   ultrasonic.threshold.lowervalue);
          JsonBuilderI->Add_P(          PSTR(D_JSON_UPPER   D_JSON_VALUE),   ultrasonic.threshold.uppervalue);
          JsonBuilderI->Add_P(          PSTR(D_JSON_INSIDE  D_JSON_COUNT),   ultrasonic.threshold.insidecount);
          JsonBuilderI->Add_P(          PSTR(D_JSON_OUTSIDE D_JSON_COUNT),   ultrasonic.threshold.outsidecount);
          JsonBuilderI->Level_Start_P(  PSTR(D_JSON_RATIO));
            JsonBuilderI->Add_P(          PSTR(D_JSON_POSITIVE),   ultrasonic.threshold.ratio_pos);
            JsonBuilderI->Add_P(          PSTR(D_JSON_NEGATIVE),   ultrasonic.threshold.ratio_pos);
            JsonBuilderI->Add_P(          PSTR(D_JSON_RELATIVE),   ultrasonic.threshold.ratio_pos);
          JsonBuilderI->Level_End(); // D_JSON_RATIO
        JsonBuilderI->Level_End(); // D_JSON_THRESHOLD
      }
    JsonBuilderI->Level_End(); // D_JSON_SENSOR
    JsonBuilderI->Level_Start(D_JSON_INSTANT);
      JsonBuilderI->Add(D_JSON_ISVALID, averaged.instant.isvalid);
      JsonBuilderI->Add(D_JSON_DISTANCE "_mm", averaged.instant.final.distance_mm);
    JsonBuilderI->Level_End();   // D_JSON_INSTANT 
    JsonBuilderI->Level_Start(D_JSON_SMOOTH "_1m");
      JsonBuilderI->Add(D_JSON_ISVALID, averaged.smooth_1m.isvalid);
      JsonBuilderI->Add(D_JSON_DISTANCE "_mm", averaged.smooth_1m.final.distance_mm);
      if(json_level >= JSON_LEVEL_DETAILED){
      JsonBuilderI->Add(D_JSON_DEVIATION, averaged.smooth_1m.averaging.deviationaverage);
      JsonBuilderI->Add(D_JSON_OUTLIERS, averaged.smooth_1m.averaging.outliercount);
      JsonBuilderI->Add(D_JSON_RATIO, averaged.smooth_1m.averaging.usableratio);
      }
    JsonBuilderI->Level_End();  // D_JSON_SMOOTH "_1m"
    JsonBuilderI->Level_Start(D_JSON_SMOOTH "_1hr");
      JsonBuilderI->Add(D_JSON_ISVALID, averaged.smooth_1hr.isvalid);
      JsonBuilderI->Add(D_JSON_DISTANCE "_mm", averaged.smooth_1hr.final.distance_mm);
      if(json_level >= JSON_LEVEL_DETAILED){
      JsonBuilderI->Add(D_JSON_DEVIATION, averaged.smooth_1hr.averaging.deviationaverage);
      JsonBuilderI->Add(D_JSON_OUTLIERS, averaged.smooth_1hr.averaging.outliercount);
      JsonBuilderI->Add(D_JSON_RATIO, averaged.smooth_1hr.averaging.usableratio);
      }
    JsonBuilderI->Level_End(); // D_JSON_SMOOTH "_1hr"

  return JsonBuilderI->End();

}

uint8_t mUltraSonicSensor::ConstructJSON_SensorsAveraged(uint8_t json_level){

  JsonBuilderI->Start();  
  // Serial.println("ConstructJSON_SensorsAveraged");

  // D_DATA_BUFFER_CLEAR();
  // DynamicJsonDocument doc(200);
  // JsonObject root = doc.to<JsonObject>();

  // // root["json_teleperiod_level"] = "test";//pCONT_set->GetTelePeriodJsonLevelCtr();

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return JsonBuilderI->End();
}

/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mUltraSonicSensor::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Sensors;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Sensors;
  
  mqtthandler_ptr = &mqtthandler_averaged_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_AVERAGED_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_SensorsAveraged;
  
  mqtthandler_ptr = &mqtthandler_averaged_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_AVERAGED_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_SensorsAveraged;

} //end "MQTTHandler_Init"


void mUltraSonicSensor::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mUltraSonicSensor::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mUltraSonicSensor::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_AVERAGED_IFCHANGED_ID
  };
  
  struct handler<mUltraSonicSensor>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod,
    &mqtthandler_averaged_ifchanged
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_ULTRASONIC_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );
  
}


////////////////////// END OF MQTT /////////////////////////

#endif
