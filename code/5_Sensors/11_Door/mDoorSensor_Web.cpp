#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mDoorSensor::WebAppend_Root_Status_Table_Draw(){

  char buffer[10];
    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>Door Position</td>"));
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_door", IsDoorOpen_Ctr(buffer, sizeof(buffer)));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  
}


//append to internal buffer if any root messages table
void mDoorSensor::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  char value_ctr[8];
  char colour_ctr[10];
  char inner_html[100];
  char door_pos_ctr[20];
  char time_ctr[20];

  JsonBuilderI->Array_Start("tab_door");// Class name
  
  for(int sensor_id=0;sensor_id<1;sensor_id++){
    
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

      char colour_ctr[8];
      uint32_t millis_elapsed = mTime::MillisElapsed(&door_detect.tEndedTime);
      // Motion in progress
      if(door_detect.isactive){
        sprintf_P(colour_ctr,PSTR("#00ff00"));
      }else
      // If movement event has just finished
      if(millis_elapsed<(1000*60)){
        // Show colour as fading back to white over X seconds SINCE EVENT OVER
        uint8_t colour_G = constrain(
                              map(millis_elapsed,0,(1000*60),0,255)
                              ,0,255 //increases with time
                            );
        pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
      }
      // no event show, just white
      else{
        sprintf(colour_ctr,"#ffffff");
      }

      // sprintf(inner_html,"%s %s",IsDoorOpen_Ctr(door_pos_ctr,sizeof(door_pos_ctr)),
      //   mTime::ConvertShortTime_HHMMSS(&door_detect.detected_time, time_ctr, sizeof(time_ctr)));
    
      JsonBuilderI->Add("ih",inner_html);
      JsonBuilderI->Add("fc",colour_ctr);
    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_End();

}

#endif // USE_MODULE_NETWORK_WEBSERVER


#endif


