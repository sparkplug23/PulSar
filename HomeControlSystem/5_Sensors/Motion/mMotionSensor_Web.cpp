#include "mMotionSensor.h"

#ifdef USE_MODULE_SENSORS_MOTION

#ifndef DISABLE_WEBSERVER
void mMotionSensor::WebAppend_Root_Draw_PageTable(){
char buffer[50];
for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){       
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>PIR Motion %s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_pir","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
}
}


void mMotionSensor::WebPage_Root_AddHandlers(){
  // Not needed
}

//append to internal buffer if any root messages table
void mMotionSensor::WebAppend_Root_Status_Table(){

  char buffer[20];

  JsonBuilderI->Array_Start("tab_pir");// Class name
  for(int sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

        char colour_ctr[8];
        uint32_t millis_elapsed = mTime::MillisElapsed(&pir_detect[sensor_id].tEndedTime);
        // Motion in progress
        if(pir_detect[sensor_id].isactive){
          sprintf_P(colour_ctr,PSTR("#00ff00"));
        }else
        // If movement event has just finished
        if(millis_elapsed<(1000*60)){
          // Show colour as fading back to white over X seconds SINCE EVENT OVER
          uint8_t colour_G = constrain(
                                map(millis_elapsed,0,(1000*60),0,255)
                                ,0,255 //increases with time
                              );
                              //Serial.printf("colour_G=%d\n\r",colour_G);
          // sprintf(colour_ctr,"%s",
          pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
          //);
        }
        // no event show, just white
        else{
          sprintf_P(colour_ctr,PSTR("#ffffff"));
        }


      JsonBuilderI->Add("ih",
        pCONT_time->ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      
      //detected_rtc_ctr);
      JsonBuilderI->Add("fc",colour_ctr);    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_End();

}
#endif // DISABLE_WEBSERVER

#endif // USE_MODULE_SENSORS_MOTION

