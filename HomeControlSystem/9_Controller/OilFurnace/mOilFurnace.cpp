#include "mOilFurnace.h"

#ifdef USE_MODULE_CUSTOM_OILFURNACE


// void CmndMqttUser(void)
// {

// }

// void (* const MqttCommand[])(void) PROGMEM = { &CmndMqttUser } ;




void mOilFurnace::init(void){

  init_ultrasonic_sensor_parameters();

}






float mOilFurnace::GetDistanceCMReading(void){
  return pCONT->mus->ultrasonic.duration*(0.034/2);
}
float mOilFurnace::GetDistanceMMReading(void){
  return pCONT->mus->ultrasonic.duration*(0.34/2);
}
float mOilFurnace::GetOilHeightCMReading(void){
  float distance_mm= (float)pCONT->mus->ultrasonic.duration*(0.034/2);
  return (TANK_HEIGHT_CM-distance_mm);
}
float mOilFurnace::GetOilHeightMMReading(void){
  float distance_mm= (float)pCONT->mus->ultrasonic.duration*(0.34/2);
  return (TANK_HEIGHT_MM-distance_mm);
}
float mOilFurnace::GetOilHeightMMReadingAdjustedFromTemp(void){
  float distance_mm= (float)pCONT->mus->GetDistanceMMReadingAdjustedForTemp();
  return (TANK_HEIGHT_MM-distance_mm);
}
float mOilFurnace::GetOilHeightCMReading(int duration){
  float distance_mm= (float)duration*(0.034/2);
  return (TANK_HEIGHT_CM-distance_mm);
}
float mOilFurnace::GetOilHeightMMReading(int duration){
  float distance_mm= (float)duration*(0.34/2);
  return (TANK_HEIGHT_MM-distance_mm);
}



float mOilFurnace::GetOilHeightMMReadingAdjustedFromTemp(int distance_from_top){
  return (TANK_HEIGHT_MM-distance_from_top);
}
float mOilFurnace::GetOilHeightCMReadingAdjustedFromTemp(int distance_from_top){
  return (TANK_HEIGHT_CM-distance_from_top);
}





// Will measure the sensors in 3 ways
// 1 exact value (instantaneous)
// X times per tick (fast smoothing)
// X times capturing 1 per tick (slow smoothing over time)
void mOilFurnace::SubTask_UltraSonicAverageToOilTank(){
  
  // instant
  oiltank.ptr = &oiltank.instant;
  oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT->mus->averaged.instant.final.distance_mm);
  oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT->mus->averaged.instant.final.distance_cm);
  oiltank.ptr->ischanged = true;
  oiltank.ptr->isvalid = true;
  oiltank.ptr->captured.tLastChanged = millis();

  // 1 minute
  oiltank.ptr = &oiltank.smooth_1m;
  oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT->mus->averaged.smooth_1m.final.distance_mm);
  oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT->mus->averaged.smooth_1m.final.distance_cm);
  oiltank.ptr->ischanged = true;
  oiltank.ptr->isvalid = true;
  oiltank.ptr->captured.tLastChanged = millis();

  // 1 hour
  oiltank.ptr = &oiltank.smooth_1hr;
  oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT->mus->averaged.smooth_1hr.final.distance_mm);
  oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT->mus->averaged.smooth_1hr.final.distance_cm);
  oiltank.ptr->ischanged = true;
  oiltank.ptr->isvalid = true;
  oiltank.ptr->captured.tLastChanged = millis();

}


void mOilFurnace::SubTask_CopyAveragedSensorValues(){

  oiltank.instant.ultrasonic_readings.distance_cm = pCONT->mus->averaged.instant.final.distance_cm;
  oiltank.smooth_1m.ultrasonic_readings.distance_cm = pCONT->mus->averaged.smooth_1m.final.distance_cm;
  oiltank.smooth_1hr.ultrasonic_readings.distance_cm = pCONT->mus->averaged.smooth_1hr.final.distance_cm;

}


void mOilFurnace::SubTask_CalculateOilVolume(){

  // Read in the averages

  // Only fill using one set of time measurements
  for(int i=0;i<3;i++){

    switch(i){
      case 0: oiltank.ptr = &oiltank.instant; break;
      case 1: oiltank.ptr = &oiltank.smooth_1m; break;
      case 2: oiltank.ptr = &oiltank.smooth_1hr; break;
    }

    // Above tank radius split
    if(oiltank.ptr->final.distance_from_bottom_mm>=TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM){
      // Need to account for smaller tank diameter on bottom AND nozzle height
      oiltank.ptr->final.volume_of_oil_cm3 = TANK_MAIN_RADIUSSQUARED_TIMES_PI_CM * ((oiltank.ptr->final.distance_from_bottom_mm-TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM)/10);
      oiltank.ptr->final.volume_of_oil_mm3 = TANK_MAIN_RADIUSSQUARED_TIMES_PI_MM * (oiltank.ptr->final.distance_from_bottom_mm-TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM);
      // Bottom section ADD on
      oiltank.ptr->final.volume_of_oil_cm3 += TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_CM * (TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM/10);
      oiltank.ptr->final.volume_of_oil_mm3 += TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_MM * (TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM);
    }else{
      // Can just use height, since its below 170 anyway
      oiltank.ptr->final.volume_of_oil_cm3 = TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_CM * (oiltank.ptr->final.distance_from_bottom_mm/10);
      oiltank.ptr->final.volume_of_oil_mm3 = TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_MM * (oiltank.ptr->final.distance_from_bottom_mm);
    }

    // 1000 cm3 or cc, is 1 Litre
    oiltank.ptr->final.litres_in_tank = oiltank.ptr->final.volume_of_oil_cm3/1000;
    oiltank.ptr->final.millilitres_in_tank = oiltank.ptr->final.volume_of_oil_mm3/1000;

    oiltank.ptr->final.litres_of_usable_oil = oiltank.ptr->final.litres_in_tank - TANK_LITRES_BELOW_PIPE;
    oiltank.ptr->final.tLastChanged = millis();

  }

} // end of


void mOilFurnace::SubTask_RecordLitresOverDays(void){

  // Record litre level at midnight, everyday, and save to eeprom

  // Things to save
  // 365 days
  // Last saved index 

  // save values
  if(abs(millis()-tRecordLitresOverDays)>=5000){tRecordLitresOverDays=millis();

    for(int i=0;i<5;i++){
      //EEPROM.put(EEPROM_GET_DAYS_ADDRESS(i), i*100); //add,int
    }

  }

  // Record litres today
   if(abs(millis()-tCheckTime)>=10000){ tCheckTime=millis(); // only check for time larger than checkbetween time

      // Check to within 5 seconds
      if(pCONT->mt->CheckBetweenSOD(pCONT->mt->ConvertHHMMSStoSOD(14,0,0),pCONT->mt->ConvertHHMMSStoSOD(14,0,11))){ // MIDNIGHT
      //  for(int i=0;i<5;i++){pCONT->mso->MessagePrint("\t INSIDE TIME");pCONT->mso->MessagePrintln(pCONT->mt->RtcTime.Wday);}
        if(oiltank.smooth_1m.isvalid){
        EEPROM.put(EEPROM_GET_DAYS_ADDRESS(pCONT->mt->RtcTime.Wday),(int)oiltank.smooth_1m.final.litres_in_tank); //add,int
        EEPROM.commit();
        }
      }else{
      //  pCONT->mso->MessagePrint("\t OUTSIDE TIME");
      }

  }


  if(abs(millis()-tReadStoredLitres)>5000){tReadStoredLitres=millis();

    int val;

      for(int i=0;i<7;i++){
         EEPROM.get(EEPROM_GET_DAYS_ADDRESS(i),val);
         history.perweekdays[i].litres_in_tank = val;
      }

     //
     // EEPROM.get(0,val);
     // history.perweekdays[0].litres_in_tank = val;

      history.perweekdays[0].litres_change = history.perweekdays[0].litres_in_tank-history.perweekdays[6].litres_in_tank;
      history.perweekdays[1].litres_change = history.perweekdays[1].litres_in_tank-history.perweekdays[0].litres_in_tank;
      history.perweekdays[2].litres_change = history.perweekdays[2].litres_in_tank-history.perweekdays[1].litres_in_tank;
      history.perweekdays[3].litres_change = history.perweekdays[3].litres_in_tank-history.perweekdays[2].litres_in_tank;
      history.perweekdays[4].litres_change = history.perweekdays[4].litres_in_tank-history.perweekdays[3].litres_in_tank;
      history.perweekdays[5].litres_change = history.perweekdays[5].litres_in_tank-history.perweekdays[4].litres_in_tank;
      history.perweekdays[6].litres_change = history.perweekdays[6].litres_in_tank-history.perweekdays[5].litres_in_tank;

      uint8_t today = pCONT->mt->RtcTime.Wday;
      uint8_t yday;
      if(pCONT->mt->RtcTime.Wday==0){yday=6;}else{yday=pCONT->mt->RtcTime.Wday-1;}
      uint8_t tomorrow = pCONT->mt->RtcTime.Wday+1;
      if(tomorrow>6){tomorrow=0;}

      history.overview.litres_change = history.perweekdays[tomorrow].litres_in_tank-history.perweekdays[today].litres_in_tank;
      history.litres_per_day = history.overview.litres_change/7;
      history.days_remaining = oiltank.smooth_1m.final.litres_of_usable_oil/history.litres_per_day;

  }

}







// void mOilFurnace::parse_JSONCommand(){


//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/oilfurnace")>=0){
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return; // not meant for here
//   }


// return;
//   // u
//   // int8_t tmp_id = 0;

//   // #ifdef JSONDOCUMENT_STATIC
//   //   StaticJsonDocument<800> doc;
//   // #else
//   //   DynamicJsonDocument doc(600);
//   // #endif
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  
//   // if(error){
//   //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
//   //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
//   //   return 0;
//   // }
//   // JsonObject obj = doc.as<JsonObject>();
  
//   // if(!obj["command"].isNull()){ 
//   //   const char* command = obj["command"];
//   //   if(strstr(command,"system_send_all")){ 
//   //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
//   //     MQTTHandler_Set_fSendNow();
//   //     isserviced++;
//   //   }
//   //   else{
//   //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
//   //   }
//   // }

//   // 

// } // END FUNCTION

void mOilFurnace::init_ultrasonic_sensor_parameters(){

// #ifdef USE_MODULE_SENSORS_DS18B20
  // pCONT->mus->ultrasonic.settings.measure_rate_ms = 2000;
  // pCONT->mus->ultrasonic.settings.blocking_time_ms = 1000;
  // pCONT->mus->ultrasonic.settings.duration_limit_max = 10000;
  // pCONT->mus->ultrasonic.settings.duration_limit_min = 4000;
// #endif

}


int8_t mOilFurnace::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    //pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 

      #ifdef USE_MODULE_SENSORS_DS18B20
      if(pCONT->mus->ultrasonic.isvalid&&fUpdateCalculations){ fUpdateCalculations = false;
        //AddLog_P(LOG_LEVEL_TEST,PSTR("OilFurnace::isvalid"));
        SubTask_CopyAveragedSensorValues();
        SubTask_UltraSonicAverageToOilTank();
        SubTask_CalculateOilVolume();
      }
      #endif
      // //SubTask_RecordLitresOverDays(); EEPROM not active

      // #ifdef FURNACE_MONITOR_PIN
      //   if(FURNACEACTIVE()!=furnace_detect.state){
      //     furnace_detect.state = FURNACEACTIVE(); //tDetectTime = millis();
      //     if(furnace_detect.state){ Serial.print("Active high");
      //       furnace_detect.isactive = true;
      //       furnace_detect.tDetectTime = millis();
      //     }else{ Serial.print("Active low");
      //       furnace_detect.isactive = false;
      //     }
      //     furnace_detect.ischanged = true;
      //   }
      // #endif
      
      // if(mTime::TimeReached(&tSavedSendMQTTIfChanged,10*1000)){
      //   MQTTHandler_Set_fSendNow();
      //   // mqtthandler_.flags.SendNow = true;
      //   // mqtthandler_ifchanged_detailed.flags.SendNow = true;
      // }

    break;
    case FUNC_EVERY_SECOND:
    
    break;

    case FUNC_SENSOR_UPDATED: // called when ultrasonic is updated -- when a sensor feeds a system status, set flags only, non blocking
      fUpdateCalculations = true; // Only flags, loop will still handle the change
    break;


    // case FUNC_JSON_COMMAND:
    //   parse_JSONCommand();
    // break;  

    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); // Reset to the initial parameters
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
  }

  /************
   * WEBPAGE SECTION * 
  *******************/
  //
  // #ifdef USE_MODULE_CORE_WEBSERVER
  // return Tasker_Web(function);
  // #endif // USE_MODULE_CORE_WEBSERVER

  // switch(function){

  //   // case FUNC_WEB_SHOW_PARAMETERS:{
      
  //   //   uint8_t fsize = 16;
  //   //   char name_ctr[20];
  //   //   char value_ctr[10];
  //   //   char float_ctr[20];

  //   //   memset(value_ctr,0,sizeof(value_ctr));
  //   //   memset(name_ctr,0,sizeof(name_ctr));
        

  //   //   dtostrf(oiltank.instant.final.litres_of_usable_oil,4,1,float_ctr);
      
  //   //   sprintf(name_ctr,"Oil Remaining");
      
  //   //   sprintf(value_ctr,"%s %s",float_ctr,"Litres");

  //   //   BufferWriterI->Append_P(HTTP_SNS_GENERIC, 
  //   //     name_ctr,
  //   //     value_ctr
  //   //   );

  //   // }break;
  //   case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
  //     // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
  //     //   BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Tank Height");//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
  //     //   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_oil","?");   
  //     // BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  //   break; 
  //   // case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
  //   //   // JsonBuilder_Add("/fetch/tab_oil_specific.json",1000);
  //   // break;

  //   case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
  //     // JsonBuilderI->Add("/draw/palette_selector.json",-500);
  //   break;
  //   case FUNC_WEB_ADD_HANDLER:
  //     WebPage_Root_AddHandlers();
  //   break;
  //   case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
  //     WebAppend_Root_Draw_Table();
  //   break; 
  //   case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
  //     WebAppend_Root_Status_Table();
  //   break;
  //   case FUNC_WEB_APPEND_ROOT_BUTTONS:
  //     // WebAppend_Root_ControlUI();
  //   break;
  //   // Generated in "InterfaceLighting" and populated in hardware classes
  // }// END switch

}
int8_t mOilFurnace::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}

void mOilFurnace::parsesub_CheckAll(JsonObjectConst obj){
  // parsesub_ModeManual(obj);
  // parsesub_ProgramTimers(obj);
  // parsesub_ProgramTemps(obj);
}

int8_t mOilFurnace::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/oilfurnace")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parse_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mOilFurnace::parse_JSONCommand(JsonObjectConst obj){
  
  int8_t isserviced = false;
  
  // if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/manual",sizeof("/manual")-1)>=0){
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "manual"));    
  //   #endif
  //   isserviced += parsesub_ModeManual(obj);
  // }else
  // if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/programs/timers",sizeof("/programs/timers")-1)>=0){
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "/programs/timers"));    
  //   #endif
  //   isserviced += parsesub_ProgramTimers(obj);
  // }else
  // if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/programs/temps",sizeof("/programs/temps")-1)>=0){
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "/programs/temps"));    
  //   #endif
  //   isserviced += parsesub_ProgramTemps(obj);
  // }

}



void mOilFurnace::WebAppend_Root_Draw_Table(){

  //WebAppend_Root_Draw_RGBLive();
  // if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
  //   WebAppend_Root_Draw_RGBPalette();
  // // }
  // WebAppend_Root_Draw_PaletteSelect_Placeholder();
  pCONT_web->WebAppend_Root_Draw_Table_dList(8,"oil_table", kTitle_TableTitles_Root);
  
}


//append to internal buffer if any root messages table
void mOilFurnace::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("oil_table");// Class name
  for(int row=0;row<8;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        default:
        case 0: JsonBuilderI->Add("ih","GetAnimationStatusCtr(buffer)"); break;
        // case 1:
        //   JsonBuilderI->Add_FV("ih",PSTR("\"%d/%d (secs)\""), 
        //     animation.transition.rate_ms.val/1000, 
        //     animation.transition.time_ms.val/1000
        //   );
        // break;
        // case 2: 
        //   JsonBuilderI->Add_FV("ih",PSTR("\"%d%% [#%d]\""),
        //     animation.transition.pixels_to_update_as_percentage.val, 
        //     GetPixelsToUpdateAsNumberFromPercentage(animation.transition.pixels_to_update_as_percentage.val)
        //   );
        // break;
        // case 3: JsonBuilderI->Add("ih",GetTransitionOrderName(buffer)); break;
        // case 4: JsonBuilderI->Add("ih",GetAnimationModeName(buffer, sizeof(buffer)));   break;
        // case 5: JsonBuilderI->Add("ih",GetFlasherFunctionName(buffer)); break;
        // case 6: JsonBuilderI->Add_FV("ih",PSTR("\"%d (%s)\""), (int)power_rating.power,"W"); break;
        // case 7:
        //   if(!animation.auto_time_off_secs){ //off
        //     JsonBuilderI->Add("ih","Unset");
        //   }else{
        //     JsonBuilderI->Add_FV("ih",PSTR("\"%d (%s)\""),
        //       animation.auto_time_off_secs,"secs"
        //     );
        //   }
        // break;
      } //switch
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  

  // char colour_button[8];
  // char button_ids[] = {ANIMATION_MODE_FLASHER_ID, ANIMATION_MODE_SCENE_ID, 
  // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  // ANIMATION_MODE_NOTIFICATIONS_ID, 
  // #endif
  // ANIMATION_MODE_NONE_ID};
  
  // JsonBuilderI->Array_Start("animod");// Class name
  // for(int row=0;row<sizeof(button_ids);row++){
  //   if(button_ids[row] == animation.mode_id){
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON_HIGHLIGHT); //selected
  //   }else{
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON); //NOT selected
  //   }        
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->Add("id",row);
  //     JsonBuilderI->Add("bc",colour_button);
  //   JsonBuilderI->Level_End();
  // }  
  // JsonBuilderI->Array_End();

}


void mOilFurnace::WebAppend_Root_ControlUI(){

  // char buffer[50];

  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Animation Mode Select"),"");

  // uint8_t animation_mode_list_ids[] = {
  //   ANIMATION_MODE_FLASHER_ID, ANIMATION_MODE_SCENE_ID, 
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  //   ANIMATION_MODE_NOTIFICATIONS_ID,
  //   #endif    
  //   ANIMATION_MODE_NONE_ID
  // };

  // BufferWriterI->Append_P(PSTR("{t}<tr>"));
  // for(uint8_t button_id=0;button_id<sizeof(animation_mode_list_ids);button_id++){
  //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2,
  //                             100/sizeof(animation_mode_list_ids),
  //                             "", //no span
  //                             "animod" " " "buttonh",
  //                             D_JSON_ANIMATIONMODE, 
  //                             GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
  //                             GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
  //                             ""
  //                           );                   
  // }
  // BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));
  
}



void mOilFurnace::WebPage_Root_AddHandlers(){

  /**
   * Pages
   * */


  /**
   * Styles
   * */

  // pCONT_web->pWebServer->on(WEB_HANDLER_STYLE_TOPBAR, HTTP_GET, [this](AsyncWebServerRequest *request){ 
  //   Handle_Style_TopBar(request);
  // });


  /**
   * Javascripts (code)
   * */

  // pWebServer->on(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER, HTTP_GET, [this](AsyncWebServerRequest *request){ 
  //   Serial.println(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER); Serial.flush();      
  //   request->send_P(200,"text/javascript",SCRIPT_WEB_DATA_FETCHER);
  // });

  // Root page listeners
  // pCONT_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
  //   WebSend_JSON_RootPage_LiveviewPixels(request); 
  // });
  // pCONT_web->pWebServer->on("/fetch/rgb_palette.json", [this](AsyncWebServerRequest *request){  //works
  //   Serial.println("/fetch/rgb/palette.json"); Serial.flush();
  //   WebSend_JSON_RootPage_Palette_AJ(request); 
  // });

  
  /**
   * JSON data refresh
   * */

  // pCONT_web->pWebServer->on(WEB_HANDLE_JSON_OIL_SENSOR_TABLE, HTTP_GET, [this](AsyncWebServerRequest *request){ 
  //   WebSend_JSON_Table(request);    
  // });

  // pWebServer->on(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER, HTTP_GET, [this](AsyncWebServerRequest *request){ 
  //   Serial.println(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER); Serial.flush();      
  //   request->send_P(200,"text/javascript",SCRIPT_WEB_DATA_FETCHER);
  // });




}


/**
 * Send only the needed message (where another will call all modules to group send)
 * */
void mOilFurnace::WebSend_JSON_Table(AsyncWebServerRequest *request){
    
  if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  uint16_t freemem_start = ESP.getFreeHeap();  
  DynamicJsonDocument doc(400);  

  JsonObject root = doc.to<JsonObject>();
  
  ConstructRoot_JSON_Table(root);
  
  memset(data_buffer.payload.ctr,0,sizeof(data_buffer.payload.ctr));
  serializeJson(doc,data_buffer.payload.ctr);
  request->send(200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);
    
  uint16_t freemem_used = freemem_start - ESP.getFreeHeap();
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_ASYNC WEB_HANLDE_JSON_WEB_TOP_BAR " Ef::%s=%d"),"freemem_used",freemem_used);
  //freemem_usage.WebSend_JSON_WebServer_TopBar = freemem_used;

} // end function


void mOilFurnace::ConstructRoot_JSON_Table(JsonObject root){

	JsonArray classname_obj = root.createNestedArray("tab_oil");

  for(int row=0;row<1;row++){
    switch(row){
      case 0:{        
        char table_row[25]; memset(table_row,0,sizeof(table_row));        
        // sprintf(table_row,"%s&deg;%c",climate.ptr->temperature_ctr,pCONT_sup->TempUnit());
        
        char float_ctr[10];
        memset(float_ctr,0,sizeof(float_ctr));
        
      #ifdef USE_MODULE_SENSORS_DS18B20
        float height = 120-pCONT->mus->GetDistanceCMReading();
        #else
        float height = 120-0;


        #endif
        dtostrf(height,4,2,float_ctr);
        sprintf(table_row,"%s (cm)",float_ctr);

        JsonObject element_obj = classname_obj.createNestedObject();
          element_obj["id"] = row;
          element_obj["ih"] = table_row;
          // Make function that takes range_limit_array and returns colours assigned to them eg colour = GetColourForRange(range_array,current_number)
          // if(climate.ptr->temperature<=25){
          //   element_obj["fc"] = "#00ff00"; //create variable/use webcolour ids
          // }else
          // if(climate.ptr->temperature>25){
          //   element_obj["fc"] = "#fcba03";
          // }else{
          //   element_obj["fc"] = "#0000003";
          // }
      }break;
      // case 1:{        
      //   char table_row[25]; memset(table_row,0,sizeof(table_row));       
      //   sprintf(table_row,"%s",presence_detect.detected_rtc_ctr);
      //   JsonObject element_obj = classname_obj.createNestedObject();
      //     element_obj["id"] = row;
      //     element_obj["ih"] = table_row;
      //     // if(climate.ptr->humidity>75){
      //     //   element_obj["fc"] = "#ff0000"; //create variable/use webcolour ids
      //     // }else
      //     // {
      //     //   element_obj["fc"] = "#0ff000";
      //     // }
      // }break;
    } //end switch 
  } // end for

} // end function





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
// Send all litres calculations from all methods
uint8_t mOilFurnace::ConstructJSON_Litres(uint8_t json_method){

  D_DATA_BUFFER_CLEAR();
  StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  JsonObject root = doc.to<JsonObject>();

  if(json_method >= JSON_LEVEL_SHORT){

    root["isvalid"] = oiltank.smooth_1hr.isvalid;
    root["litres_in_tank"] = oiltank.smooth_1hr.final.litres_in_tank;
    root["litres_of_usable_oil"] = oiltank.smooth_1hr.final.litres_of_usable_oil;
    root["litres_of_usable_oil_rounded"] = mSupport::roundfloat(oiltank.smooth_1hr.final.litres_of_usable_oil,1);
    root["height_cm"] = (int)oiltank.smooth_1hr.final.distance_from_bottom_cm;

    root["litres_per_day"] = history.litres_per_day;
    root["days_remaining"] = floor(history.days_remaining);
    root["weekly_usage"] =   floor(history.overview.litres_change);
    root["daily_usage"] =    floor(history.overview.litres_change);

  }

  // Only send when more than short is asked for
  if(json_method > JSON_LEVEL_SHORT){

  JsonObject instantobj = root.createNestedObject("instant");
    instantobj["isvalid"] = mSupport::roundfloat(oiltank.instant.isvalid,1);
    instantobj["height_cm"] = mSupport::roundfloat(oiltank.instant.final.distance_from_bottom_cm,1);
    instantobj["litres_usable"] = mSupport::roundfloat(oiltank.instant.final.litres_of_usable_oil,1);
    if(json_method <= JSON_LEVEL_DETAILED){
      instantobj["height_mm"] = mSupport::roundfloat(oiltank.instant.final.distance_from_bottom_mm,1);
      instantobj["litres_total"] = mSupport::roundfloat(oiltank.instant.final.litres_in_tank,1);
      instantobj["last_changed"] = abs(millis()-oiltank.instant.final.tLastChanged);
    }

  JsonObject smooth1mobj = root.createNestedObject("smooth_1m");
    smooth1mobj["isvalid"] = oiltank.smooth_1m.isvalid;
    smooth1mobj["height_cm"] = mSupport::roundfloat(oiltank.smooth_1m.final.distance_from_bottom_cm,1);
    smooth1mobj["litres_usable"] = mSupport::roundfloat(oiltank.smooth_1m.final.litres_of_usable_oil,1);
    if(json_method <= JSON_LEVEL_DETAILED){
      smooth1mobj["height_mm"] = mSupport::roundfloat(oiltank.smooth_1m.final.distance_from_bottom_mm,1);
      smooth1mobj["litres_total"] = mSupport::roundfloat(oiltank.smooth_1m.final.litres_in_tank,1);
      smooth1mobj["last_changed"] = abs(millis()-oiltank.smooth_1m.final.tLastChanged);
    }

  JsonObject smooth1hrobj = root.createNestedObject("smooth_1hr");
    smooth1hrobj["isvalid"] = oiltank.smooth_1hr.isvalid;
    smooth1hrobj["litres_usable"] = mSupport::roundfloat(oiltank.smooth_1hr.final.litres_of_usable_oil,1);
    smooth1hrobj["height_cm"] = mSupport::roundfloat(oiltank.smooth_1hr.final.distance_from_bottom_cm,1);
    if(json_method <= JSON_LEVEL_DETAILED){
      smooth1hrobj["litres_total"] = mSupport::roundfloat(oiltank.smooth_1hr.final.litres_in_tank,1);
      smooth1hrobj["last_changed"] = abs(millis()-oiltank.smooth_1hr.final.tLastChanged);
      smooth1hrobj["height_mm"] = mSupport::roundfloat(oiltank.smooth_1hr.final.distance_from_bottom_mm,1);
    }
  }

  data_buffer.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer.payload.ctr);

  return 1;
}



uint8_t mOilFurnace::ConstructJSON_Settings(uint8_t json_method){

    D_DATA_BUFFER_CLEAR();
    StaticJsonDocument<400> doc;
    JsonObject root = doc.to<JsonObject>();

    // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();

    data_buffer.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer.payload.ctr);

    return 1;

}


uint8_t mOilFurnace::ConstructJSON_Furnace(uint8_t json_method){

    D_DATA_BUFFER_CLEAR();
    StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();

  uint8_t ischanged=false;
  

  // #ifdef USE_MODULE_SENSORS_DS18B20
  // for(int i=0;i<pCONT_msdb18->db18_sensors_active;i++){
  //   if((pCONT_msdb18->db18_sensor[i].reading.ischanged)||(pCONT->mqt->fSendSingleFunctionData)){
  //     JsonObject sensorobj = root.createNestedObject(pCONT_msdb18->db18_sensor[i].name.ctr);
  //     sensorobj["temp"] = pCONT_msdb18->db18_sensor[i].reading.val;
  //     sensorobj["isvalid"]= pCONT_msdb18->db18_sensor[i].reading.isvalid;
  //   }
  // }
  // #endif

// #ifdef FURNACE_MONITOR_PIN
//   if(furnace_detect.ischanged||(pCONT->mqt->fSendSingleFunctionData)){ furnace_detect.ischanged = false;// TEMP FIX
    root["furnace_onoff"] = FURNACEACTIVECTR;//mdio_mqt->input_state_detect[0].isactive;
    root["furnace_state"] = FURNACEACTIVE()?1:0;//mdio_mqt->input_state_detect[0].isactive;
    // data_buffer.payload.json_pairs++;
  // }
  // #endif

  //if(data_buffer.payload.json_pairs>0){
    data_buffer.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer.payload.ctr);

    return 1;
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

////////////////////// START OF MQTT /////////////////////////

void mOilFurnace::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOilFurnace::ConstructJSON_Settings;
  
  mqtthandler_ptr = &mqtthandler_litres_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LITRES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOilFurnace::ConstructJSON_Litres;
  
  mqtthandler_ptr = &mqtthandler_litres_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LITRES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOilFurnace::ConstructJSON_Litres;

  mqtthandler_ptr = &mqtthandler_furnace_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FURNACE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOilFurnace::ConstructJSON_Furnace;
  
  mqtthandler_ptr = &mqtthandler_furnace_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FURNACE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOilFurnace::ConstructJSON_Furnace;

} //end "MQTTHandler_Init"


void mOilFurnace::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_litres_ifchanged.flags.SendNow = true;
  mqtthandler_litres_teleperiod.flags.SendNow = true;
  mqtthandler_furnace_ifchanged.flags.SendNow = true;
  mqtthandler_furnace_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mOilFurnace::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_litres_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_furnace_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"



void mOilFurnace::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_MODULE_LITRES_IFCHANGED_ID, 
    MQTT_HANDLER_MODULE_LITRES_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_FURNACE_IFCHANGED_ID,
    MQTT_HANDLER_MODULE_FURNACE_TELEPERIOD_ID
  };
  
  struct handler<mOilFurnace>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_litres_ifchanged,
    &mqtthandler_litres_teleperiod,
    &mqtthandler_furnace_ifchanged,
    &mqtthandler_furnace_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_CUSTOM_OILFURNACE_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}


////////////////////// END OF MQTT /////////////////////////



#endif