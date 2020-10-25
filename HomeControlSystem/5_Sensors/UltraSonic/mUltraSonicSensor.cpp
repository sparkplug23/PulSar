#include "mUltraSonicSensor.h"

#ifdef USE_MODULE_SENSORS_ULTRASONICS

void mUltraSonicSensor::Pin_Config(){ 

  if(pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID) && pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID)){
    pin_trig = pCONT_pins->GetPin(GPIO_SR04_TRIG_ID);
    pin_echo = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);

    pinMode(pin_trig, OUTPUT); // Sets the trigPin as an Output
    pinMode(pin_echo, INPUT); // Sets the echoPin as an Input
    fEnableSensor = true;
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d"),pin_trig);
    fEnableSensor = false;
  }

}


void mUltraSonicSensor::init(void){

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
   // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ULTRASONIC "ultrasonic.tUltraSonicSensorReadLast<blockingtime"));
    return ultrasonic.duration;
  }
  #endif //   #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ULTRASONIC "mUltraSonicSensor::GetDurationReading"));

  float duration=0;

  // Clears the trigPin
  digitalWrite(pin_trig, LOW);
  delayMicroseconds(2);
  // Sets the ULTRA_TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(pin_echo, HIGH);//, ultrasonic.settings.duration_limit_max); //10000us 10ms //default 1 second timeout
  ultrasonic.duration_raw = duration;

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ULTRASONIC "duration=%d"),(int)duration);

  #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
  //if outside possible range
  if((duration>ultrasonic.settings.duration_limit_min)&&(duration<ultrasonic.settings.duration_limit_max)){ //pCONT->mso->MessagePrintln("[ULTRA] SAMPLING");

    float lower = (float)ultrasonic.duration*(1-(ultrasonic.threshold.narrowpercent/100.0));//0.9;
    float upper = (float)ultrasonic.duration*(1+(ultrasonic.threshold.narrowpercent/100.0));
    ultrasonic.threshold.lowervalue = lower;
    ultrasonic.threshold.uppervalue = upper;

    // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "Lower<duration|Duration<Upper: [\t%d\t%d\t%d\t%d]"),(int)lower,(int)ultrasonic.duration,(int)duration,(int)upper);

    // New method under test
    // GET how new duration relates to previous
    if(duration>ultrasonic.duration){ // Positive range
      ultrasonic.threshold.ratio_pos = duration/ultrasonic.threshold.uppervalue;
      ultrasonic.threshold.relative = ultrasonic.threshold.ratio_pos;  
      // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "duration > ultrasonic.duration"));
    }else{ // Negative range
      ultrasonic.threshold.ratio_neg = -1*(ultrasonic.threshold.lowervalue/duration);
      ultrasonic.threshold.relative = ultrasonic.threshold.ratio_neg; 
      // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "duration < ultrasonic.duration"));
    }

    // Check if its within threshold
    if(fabsf(ultrasonic.threshold.relative)>1){ // outside range
      ultrasonic.threshold.outsidecount++;
      ultrasonic.threshold.insidecount=0;
    }else 
    if(fabsf(ultrasonic.threshold.relative)<=1){ // inside range
      ultrasonic.threshold.insidecount++;
      ultrasonic.threshold.outsidecount=0;
    }

    // Adjust threshold limits if need be
    if(ultrasonic.threshold.outsidecount>10){
      // Reduce set threshold if we can
      if(ultrasonic.threshold.setpercent < ultrasonic.threshold.widepercent){
        ultrasonic.threshold.setpercent+=5; // +10% // increase width faster to resolve errors
      }else{
        ultrasonic.threshold.setpercent = ultrasonic.threshold.widepercent;
      }
      //ultrasonic.threshold.outsidecount = 10; // freeze it at limit
    }
    if(ultrasonic.threshold.insidecount>10){
      // Reduce set threshold if we can
      if(ultrasonic.threshold.setpercent > ultrasonic.threshold.narrowpercent){
        ultrasonic.threshold.setpercent--; //-1% train in slowly
      }else{
        ultrasonic.threshold.setpercent = ultrasonic.threshold.narrowpercent;
      }
      //ultrasonic.threshold.insidecount = 10
    }

    //FORCED LIMITS
    if((ultrasonic.threshold.setpercent>ultrasonic.threshold.widepercent)||(ultrasonic.threshold.setpercent<ultrasonic.threshold.narrowpercent)){ultrasonic.threshold.setpercent=20;}
    if(ultrasonic.threshold.insidecount>200){ultrasonic.threshold.insidecount=200;}
    if(ultrasonic.threshold.outsidecount>200){ultrasonic.threshold.outsidecount=200;}

    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "fabsf(ultrasonic.threshold.relative) %f"),fabsf(ultrasonic.threshold.relative));

    if(fabsf(ultrasonic.threshold.relative)<=1){ 
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "[ULTRA]if(abs(ultrasonic.threshold.relative)<=1) \t\t"));
      ultrasonic.isvalid = true;
      ultrasonic.ischanged = true;
      ultrasonic.accuracy.insidecount++;
    }else{
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ULTRASONIC "[ULTRA] NOT if(abs(ultrasonic.threshold.relative)<=1)"));
      ultrasonic.isvalid = false;
      ultrasonic.ischanged = false;
      ultrasonic.accuracy.outsidecount++;
    }

    ultrasonic.duration = duration; //update stored
    ultrasonic.tUltraSonicSensorReadLast = millis();

    float total = ultrasonic.accuracy.insidecount+ultrasonic.accuracy.outsidecount;
    if(total!=0){ // bottom of fraction reduces to only inside
      ultrasonic.accuracy.percent = (ultrasonic.accuracy.insidecount/total)*100;
    }
    return ultrasonic.duration;
  }else{ // ==0 no response
  
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "[ULTRA] Outside viable range"));
    //pCONT->mso->MessagePrintln("[ULTRA] Outside viable range");
    ultrasonic.isvalid = false;
    ultrasonic.ischanged = false;
    ultrasonic.accuracy.outsidecount++;
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

uint8_t mUltraSonicSensor::WITHINLIMITS(int minv, float var, int maxv){
  //if((var>=minv)&&(var<=maxv)&&(!isnan(var))){

  //Serial.println(isnan(var));
  if(isnan(var)){
    return false;
  }

  if((var>=minv)&&(var<=maxv)){
      return true;
  }else{
      return false;
  }

  //return  ? 1 : 0;  //ternary operator (condition) ? (if_true) : (if_false)
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
        int tempsensorid;
        float Tcelius;
        if((tempsensorid=pCONT_msdb18->getIDbyName("tank1"))>=0){
            if(pCONT_msdb18->db18_sensor[tempsensorid].reading.isvalid){
            Tcelius = pCONT_msdb18->db18_sensor[tempsensorid].reading.val;
            // Reduce frequency of updates to stop data jumps
            if(abs(millis()-ultrasonic.tPermitTempUpdate)>60000){ultrasonic.tPermitTempUpdate = millis();
                ultrasonic.temperature = Tcelius;
            }
            // float insidesqrt = (ultrasonic.temperature/273.15)+1;
            // speedofsound_inmps = 331.227599 * sqrt(insidesqrt);
            speedofsound_inmps = 331.3 + (0.606 * ultrasonic.temperature); //https://en.wikipedia.org/wiki/Speed_of_sound
            }
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

  //stored_speedofsound_inmetres = speedofsound_inmetres;
  //Serial.print("[ULTRA] speedofsound_inmetres>> "); Serial.println(speedofsound_inmetres);
  //float speedofsound_inmillimetres = (speedofsound_inmetres*1000)/1000000; *1000 from m->mm, divide by 1E6 for Us
  float speedofsound_inmillimetres_permicroseconds = (speedofsound_inmetres)/1000;

  //Serial.print("[ULTRA] speedofsound_inmillimetres_permicroseconds>> "); Serial.println(speedofsound_inmillimetres_permicroseconds);
  //float sos = GetSpeedOfSoundInMetres();

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

    AddLog_P(LOG_LEVEL_TEST,PSTR("final.distance_mm=%d"),(int)averaged.instant.final.distance_mm);
    AddLog_P(LOG_LEVEL_TEST,PSTR("tank.distance_mm=%d"),(int)(1160-averaged.instant.final.distance_mm));

    //pCONT->mso->MessagePrint("[ULTRA] averaged.instant.final.distance_mm  >>");
    //pCONT->mso->MessagePrintln(averaged.instant.final.distance_mm);
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

    memset(&data_buffer2,0,sizeof(data_buffer2));

    StaticJsonDocument<300> doc;
    JsonObject root = doc.to<JsonObject>();
    #ifdef MOTIONALERT_PAYLOAD1_CTR
      root["location"] = MOTIONALERT_PAYLOAD1_CTR;
    #endif
    root["time"] = pCONT->mt->mtime.hhmmss_ctr;

    data_buffer2.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer2.payload.ctr);

    if(presence_detect.isactive){
      pCONT->mqt->ppublish("status/presence/detected",data_buffer2.payload.ctr,false);
    }else{
      pCONT->mqt->ppublish("status/presence/over",data_buffer2.payload.ctr,false);
    }

  }

}



int8_t mUltraSonicSensor::Tasker(uint8_t function){

  //  AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_ULTRASONIC "mUltraSonicSensor::Tasker = %d"),function);
        
  // Run even when sensor is disabled (Will set fEnableSensor)
  switch(function){
    case FUNC_PRE_INIT:
      Pin_Config();
    break;
  }
// Serial.println("!fEnableSensor1");
  if(!fEnableSensor){ return 0 ; }
  
  switch(function){
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: //AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_ULTRASONIC D_DEBUG_FUNCTION "mUltraSonicSensor::Tasker"));
     
  //  AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_ULTRASONIC "mUltraSonicSensor::Tasker = %d"),function);
  ultrasonic.settings.measure_rate_ms = 1000;
        
      if(abs(millis()-ultrasonic.tReadLast)>=ultrasonic.settings.measure_rate_ms){
        //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ULTRASONIC "ultrasonic.tReadLast %d"),ultrasonic.duration);
        GetDurationReading();
        if(ultrasonic.isvalid){ // stop trying
          ultrasonic.tReadLast = millis();
          fUpdateCalculations = true;
          //AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_ULTRASONIC "duration = %d"),ultrasonic.duration);
        }
      }else{
          //AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_ULTRASONIC "duration2 = %d"),ultrasonic.duration);

      }

      if(ultrasonic.isvalid){
        SubTask_UltraSonicAverage();
        pCONT->Tasker_Interface(FUNC_SENSOR_UPDATED); // Tell other dependent modules we have changed
      }
      
      #ifdef DEVICE_LIVINGROOMSENSOR
      //Alerts if sensor value changes from previous
      if(mSupport::TimeReached(&object_detected_static.tSavedCheck,10000)){
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
          pCONT->mqt->ppublish("status/presence/event",object_detected_static.ispresent?"Present":"Not Present",false);
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

      }else{
      
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "abs=%d"),abs(object_detected_static.tSavedCheck-millis()));
// delay(500);
      
      
      }//end presence detect

      #endif
      



      // if(ultrasonic.isvalid&&fUpdateCalculations){ fUpdateCalculations = false;
      //   // SubTask_CalculateOilVolume();
      // }
      

    break;
    case FUNC_EVERY_SECOND:
    
    break;

    

    /************
     * WEBPAGE SECTION * 
    *******************/
    // #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ROOT_SEND_STYLE: // Change to sending outside of script as url
      // WebPage_Root_SendStyle(); 
    break;
    case FUNC_WEB_ROOT_SEND_BODY:      //add tables etc 
      //WebPage_Root_SendBody(); 
    break;    
    // case FUNC_WEB_COMMAND:
    //   WebCommand_Parse();      
    // break;
    // case FUNC_WEB_SHOW_PARAMETERS:{

    //   // /****
    //   //  *  Show LED power 
    //   //  * ****/
    //   // uint8_t fsize = 32;
    //   // char onoff_ctr[30];
      
    //   // char float_ctr[10];
    //   // memset(float_ctr,0,sizeof(float_ctr));
    //   // dtostrf(GetDistanceCMReading()/100,3,2,float_ctr);

    //   // sprintf(onoff_ctr,"%s (m)",float_ctr);
        
    //   // pCONT_web->WSBufferAppend_PI2(HTTP_SNS_GENERIC, 
    //   //   "Ultrasonic Distance",
    //   //   onoff_ctr
    //   // );

    //   // pCONT_web->WSBufferAppend_PI2(HTTP_SNS_GENERIC, 
    //   //   "Ultrasonic Threshold Last Changed",
    //   //   presence_detect.detected_rtc_ctr
    //   // );

    // }
    // break;
    // case FUNC_WEB_SHOW_PARAMETERS:{
    //   //WebPage_Root_SendStatus();      
    // }break;
    case FUNC_WEB_ADD_MAIN_BUTTON:{
      //WebPage_Root_AddMainButtons();
    }
    break;
    case FUNC_WEB_ROOT_SEND_SCRIPT:
      //WebPage_Root_SendScripts();
    break;
    case FUNC_WEB_ADD_JSON_DATA_FETCH_URL:
      //WebPage_Root_SendJSONFetchURL();
    break;   
    // case FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING:
    //   WebPage_Root_SendParseJSONScripts();
    // break; 
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
    //
        //JsonBuilder_Add(WEB_HANLDE_JSON_WEB_TOP_BAR,1000); 
    //   BufferWriterI->Append_P(PSTR("\"%s\","),"/fetch/tab_ult_sensor.json");
    // break;
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_RATES:
    //   BufferWriterI->Append_P(PSTR("%d,"),1001);
    // break;
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    case FUNC_WEB_ADD_ROOT_SCRIPT:
      // pCONT_web->WSContentSend_PI("<script type='text/javascript' src='/script/rgb/table_script.js'></script>");
      //pCONT_web->WSContentSend_PI("<link rel='stylesheet' href='/style/rgb/root.css'>"); 
    break;
    case FUNC_WEB_ADD_ROOT_STYLE:
      // WSBufferAppend_P(pCONT_web->response_stream, "<script type='text/javascript' src='/script/rgb/script.js'></script>");
    break;
    // case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:
    //   WebPage_Root_SendInformationModule();
    // break;
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    // #endif //USE_MODULE_CORE_WEBSERVER
    
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_RESET:
      // Reset to the initial parameters
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      //MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_JSON_COMMAND:
      parse_JSONCommand();
    break;  
    case FUNC_MQTT_SENDER:  
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      //mqttConnected();
    break;
    case FUNC_MQTT_DISCONNECTED:
      //mqttDisconnected();
    break;


  }


}

void mUltraSonicSensor::WebAppend_Root_Status_Table_Draw(){
  
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Distance");//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Event");//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
}


//append to internal buffer if any root messages table
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


void mUltraSonicSensor::WebPage_Root_AddHandlers(){

  /**
   * Pages
   * */

}







void mUltraSonicSensor::parse_JSONCommand(){


  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/ultrasonic")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return; // not meant for here
  }


// WRONG
  // u
  int8_t tmp_id = 0;

  // #ifdef JSONDOCUMENT_STATIC
  //   StaticJsonDocument<800> doc;
  // #else
  //   DynamicJsonDocument doc(600);
  // #endif
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  
  // if(error){
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();
  
  // if(!obj["command"].isNull()){ 
  //   const char* command = obj["command"];
  //   if(strstr(command,"system_send_all")){ 
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
  //     MQTTHandler_Set_fSendNow();
  //     isserviced++;
  //   }
  //   else{
  //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
  //   }
  // }

  

} // END FUNCTION


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/






  // enum DATABUILDER_JSON_METHOD{ //in order of importantance
  //   JSON_METHOD_NONE=0,
  //   JSON_METHOD_IFCHANGED,
  //   JSON_METHOD_SHORT,
  //   JSON_METHOD_DETAILED,
  //   JSON_METHOD_ALL=0
  // };
// Send all lit


uint8_t mUltraSonicSensor::ConstructJSON_Settings(uint8_t json_level){

    // memset(&data_buffer2,0,sizeof(data_buffer2));
    // DynamicJsonDocument doc(200);
    // JsonObject root = doc.to<JsonObject>();

    // // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();

    // data_buffer2.payload.len = measureJson(root)+1;
    // serializeJson(doc,data_buffer2.payload.ctr);

    return 0;

}

uint8_t mUltraSonicSensor::ConstructJSON_Sensors(uint8_t json_level){

    memset(&data_buffer2,0,sizeof(data_buffer2));
    DynamicJsonDocument doc(2000);
    JsonObject root = doc.to<JsonObject>();

    JsonObject sensorobj = root.createNestedObject("sensor");
    sensorobj["isvalid"] = ultrasonic.isvalid;
    sensorobj["duration"] = ultrasonic.duration;
    sensorobj["duration_raw"] = ultrasonic.duration_raw;
    sensorobj["temp"] = ultrasonic.temperature;
    sensorobj["temp_age"] = abs(millis()-ultrasonic.tPermitTempUpdate);
    sensorobj["speedofsound"] = mSupport::roundfloat(ultrasonic.speedofsound,1);
    sensorobj["lastread"] = abs(millis()-ultrasonic.tUltraSonicSensorReadLast);
      // JsonObject accuracyobj = sensorobj.createNestedObject("accuracy");
      //   accuracyobj["inside"] = ultrasonic.accuracy.insidecount;
      //   accuracyobj["outside"] = ultrasonic.accuracy.outsidecount;
      //   accuracyobj["percent"] = ultrasonic.accuracy.percent;
      // JsonObject thresholdobj = sensorobj.createNestedObject("threshold");
      //   thresholdobj["setpercent"] = ultrasonic.threshold.setpercent;
      //   thresholdobj["narrowpercent"] = ultrasonic.threshold.narrowpercent;
      //   thresholdobj["widepercent"] = ultrasonic.threshold.widepercent;
      //   thresholdobj["lowervalue"] = ultrasonic.threshold.lowervalue;
      //   thresholdobj["uppervalue"] = ultrasonic.threshold.uppervalue;
        // thresholdobj["insidecount"] = ultrasonic.threshold.insidecount;
        // thresholdobj["outsidecount"] = ultrasonic.threshold.outsidecount;
        // thresholdobj["ratiopos"] = mSupport::roundfloat(ultrasonic.threshold.ratio_pos,1);
        // thresholdobj["rationeg"] = mSupport::roundfloat(ultrasonic.threshold.ratio_neg,1);
        // thresholdobj["relative"] = mSupport::roundfloat(ultrasonic.threshold.relative,1);

    JsonObject instantobj = root.createNestedObject("instant");
      instantobj["isvalid"] = averaged.instant.isvalid;
      instantobj["distance_mm"] = mSupport::roundfloat(averaged.instant.final.distance_mm,1);

    JsonObject smooth1mobj = root.createNestedObject("smooth_1m");
      smooth1mobj["isvalid"] = averaged.smooth_1m.isvalid;
      smooth1mobj["distance_mm"] = mSupport::roundfloat(averaged.smooth_1m.final.distance_mm,1);
      smooth1mobj["deviation"] = averaged.smooth_1m.averaging.deviationaverage;
      smooth1mobj["outliers"] = averaged.smooth_1m.averaging.outliercount;
      smooth1mobj["ratio"] = averaged.smooth_1m.averaging.usableratio;

    JsonObject smooth1hrobj = root.createNestedObject("smooth_1hr");
      smooth1hrobj["isvalid"] = averaged.smooth_1hr.isvalid;
      smooth1hrobj["distance_mm"] = mSupport::roundfloat(averaged.smooth_1hr.final.distance_mm,1);
      smooth1hrobj["deviation"] = averaged.smooth_1hr.averaging.deviationaverage;
      smooth1hrobj["outliers"] = averaged.smooth_1hr.averaging.outliercount;
      smooth1hrobj["ratio"] = averaged.smooth_1hr.averaging.usableratio;
      
    // JsonObject instantfixedobj = root.createNestedObject("instantfixedsos");
    //   instantfixedobj["isvalid"] = averaged.instant.isvalid;
    //   instantfixedobj["distance_mm"] = mSupport::roundfloat(GetDistanceMMReadingAdjustedForTemp(),1);


    data_buffer2.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer2.payload.ctr);

    return 1;

}

uint8_t mUltraSonicSensor::ConstructJSON_SensorsAveraged(uint8_t json_level){

  // Serial.println("ConstructJSON_SensorsAveraged");

  // memset(&data_buffer2,0,sizeof(data_buffer2));
  // DynamicJsonDocument doc(200);
  // JsonObject root = doc.to<JsonObject>();

  // // root["json_teleperiod_level"] = "test";//pCONT_set->GetTelePeriodJsonLevelCtr();

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  return 0;

}


// void mUltraSonicSensor::ConstructJSON_Averaged(uint8_t json_method){

//     memset(&data_buffer2,0,sizeof(data_buffer2));
//     StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//     JsonObject root = doc.to<JsonObject>();

//   uint8_t ischanged=false;
  

// root["test"] = 0;

//   #ifdef USE_MODULE_SENSORS_DS18B20
//   for(int i=0;i<pCONT_msdb18->db18_sensors_active;i++){
//     //if((pCONT_msdb18->db18_sensor[i].reading.ischanged)||(pCONT->mqt->fSendSingleFunctionData)){
//       JsonObject sensorobj = root.createNestedObject(pCONT_msdb18->db18_sensor[i].name.ctr);
//       sensorobj["temp"] = pCONT_msdb18->db18_sensor[i].reading.val;
//       sensorobj["isvalid"]= pCONT_msdb18->db18_sensor[i].reading.isvalid;
//     //}
//   }
//   #endif

// // #ifdef FURNACE_MONITOR_PIN
// //   if(furnace_detect.ischanged||(pCONT->mqt->fSendSingleFunctionData)){ furnace_detect.ischanged = false;// TEMP FIX
//     // root["furnace_onoff"] = FURNACEACTIVECTR;//mdio_mqt->input_state_detect[0].isactive;
//     // root["furnace_state"] = FURNACEACTIVE()?1:0;//mdio_mqt->input_state_detect[0].isactive;
//     // data_buffer2.payload.json_pairs++;
//   // }
//   // #endif

//   //if(data_buffer2.payload.json_pairs>0){
//     data_buffer2.payload.len = measureJson(root)+1;
//     serializeJson(doc,data_buffer2.payload.ctr);
    
// }





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
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
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

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{
    // Serial.printf("mqtt_handler_id = %d\n\r",mqtt_handler_id);

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod;  break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged;     break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod;    break;      
      case MQTT_HANDLER_MODULE_AVERAGED_IFCHANGED_ID:      handler_found=true; mqtthandler_ptr=&mqtthandler_averaged_ifchanged;   break;
      // case MQTT_HANDLER_MODULE_AVERAGED_TELEPERIOD_ID:     handler_found=true; mqtthandler_ptr=&mqtthandler_averaged_teleperiod; break; //crash error
      // No specialised needed
      default: handler_found=false; mqtthandler_ptr = nullptr; break; // nothing 
    } // switch


    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_ULTRASONIC_ID,mqtthandler_ptr);  }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////










#endif
