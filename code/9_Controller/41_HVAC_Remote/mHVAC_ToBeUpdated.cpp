/*
  File to be deleted, but hidding things to be redone here
*/
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

// This function detects if the traditional heating system has been activated.
// Methods I can use:
//  - furnace active sensor without my own relays on, which means I need to listen for data from oilfurnace
//  - if temp is above a limit (ie radiators when the pulse on and off) and if +ve ROC exceeds a value
int8_t mHVAC::Tasker_PredictManualHeating(){

//if ROC > X with programs off = on

// /if(pCONT->mhs->watertemps.tank_top.ischanged){
//
//   // test for rising tank temps
//   if(watertemps.tank_top.roc1m.val>0.2){
//     pCONT->mso->MessagePrintln("watertemps.tank_top.roc1m.val>0.2");
//   }
//
// }

}


// // returns > 0 for events added
// int8_t mHVAC::SubContructCtr_HardwareStatus(){

//   //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
//   uint8_t fNotFirstItem = 0;
//   char buffer[100];
//   // memset(&status_message,0,sizeof(status_message));

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].timers.state){
//   //     sprintf(&status_message.ctr[status_message.len],"%s timers %s",
//   //       GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state, buffer, sizeof(buffer)));
//   //     status_message.len = strlen(status_message.ctr);
//   //     SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
//   //   }
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
//   //     sprintf(&status_message.ctr[status_message.len],"%s temps %s",
//   //       GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state, buffer, sizeof(buffer)));
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
//   //     sprintf(&status_message.ctr[status_message.len],"%s relays %d",
//   //       GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//   //       // GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state, buffer, sizeof(buffer)),
//   //       pCONT_mry->relay_status[device_id].time_seconds_on
//   //     );
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // if(!status_message.len){
//   //   sprintf(&status_message.ctr[status_message.len],"%s","Nothing Running");
//   //   status_message.len = strlen(status_message.ctr);
//   // }

// }

// // returns > 0 for events added
// int8_t mHVAC::SubContructCtr_HardwareStatus_Long(){

//   //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
//   uint8_t fNotFirstItem = 0;

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].timers.state){
//   //     sprintf(&status_message.ctr[status_message.len],"%s Timer %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //     SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
//   //   }
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
//   //     sprintf(&status_message.ctr[status_message.len],"%s Temp %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
//   //     sprintf(&status_message.ctr[status_message.len],"%s relays %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // if(!status_message.len){
//   //   status_message.len += sprintf(&status_message.ctr[status_message.len],"Online");
//   // }

// } //end function



int mHVAC::mapHeatingTempToHueColour(int val){
   int val2 = mSupport::mapfloat(val,15.0,50.0,180,0); //float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
   if(val2<0){
     if(val2<-60){ //limit so we stay within purple and never looping as far as blue
       val2 = 300;
     }else{
       val2 += 360;
     }
   }
  return val2;
}

int mHVAC::mapHeatingTempToBrightness(int temp){

  int brightness;

  if(temp<25){
    brightness = map(temp,15,25,1,10); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,9,10);
  }else{
    brightness = map(temp,25,50,10,100); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,10,100);
  }

  return brightness;

}



// Send all water sensors by [temp,huebytemp(0-360),brightnessbytemp(0-100)]
// ds:{temp:0,hue:0,bright:0},
// us:{temp:0,hue:0,bright:0},
// EFFECTS to be send seperate when programs change state

// I should roll this into sensor interface? to allow me to get any sensor as a colour if enabled? with selection on map type. Just leave for long term 

// uint8_t mHVAC::ConstructSON_PipeTempsByColours(uint8_t json_level){

//   // StaticJsonDocument<800> doc;
//   // JsonObject obj = doc.to<JsonObject>();
//   // D_DATA_BUFFER_CLEAR();

//   // uint8_t ischanged = false;

//   // for(uint8_t device_id=0;device_id<8;device_id++){

//   //   switch(device_id){
//   //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//   //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//   //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//   //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//   //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//   //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//   //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//   //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//   //   }

//   //   if(stored_new.temp[device_id] != pCONT->mhs->watertemps.ptr->raw.val){
//   //     stored_new.temp[device_id] = pCONT->mhs->watertemps.ptr->raw.val;
//   //     ischanged = 1;
//   //   }
//   //   #ifdef DEBUG_SENDALL
//   //     ischanged=1; //ALL
//   //   #endif
//   //   if(ischanged){ ischanged = 0;
//   //     //data_buffer.payload.json_pairs++;
//   //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(device_id));
//   //     json1[D_JSON_TEMP] = pCONT->mhs->watertemps.ptr->raw.val;
//   //     json1[D_JSON_HUE] = mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val);
//   //     json1[D_JSON_BRT] = mapHeatingTempToBrightness(pCONT->mhs->watertemps.ptr->raw.val);

//   //     char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
//   //     RgbColor c = HsbColor(mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val)/360.0f,100/100.0f,100/100.0f);
//   //     sprintf(tmpctr,"%02X%02X%02X",c.R,c.G,c.B);
//   //     json1[D_JSON_RGB] = tmpctr;

//   //     switch(device_id){
//   //       case ID_DB18_DS: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_DS_ID); break;
//   //       case ID_DB18_US: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_US_ID); break;
//   //       case ID_DB18_WB: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_WB_ID); break;
//   //       case ID_DB18_IH: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_IH_ID); break;
//   //       default: break; //nothing
//   //     }
//   //   }

//   // }

//   // // if(data_buffer.payload.json_pairs>0){
//   //   data_buffer.payload.len = measureJson(obj)+1;
//   //   serializeJson(doc,data_buffer.payload.ctr);
//   //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATINGPANEL D_PAYLOAD " \"%s\""),data_buffer.payload.ctr);
//   // // }

//   return 0;// data_buffer.payload.len>3?1:0;

// }



#endif
