#include "mTankVolume.h"

#ifdef USE_MODULE_CONTROLLER_TANKVOLUME


uint8_t mTankVolume::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mTankVolume::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    // JBI->Object_Start(D_JSON_RFRECEIVED);
  
      // JBI->Add(D_JSON_DATA, rx_pkt.data);
      // JBI->Add(D_JSON_RF_BITS, rx_pkt.bit_length);
      // JBI->Add(D_JSON_RF_PROTOCOL, rx_pkt.protocol);
      // JBI->Add(D_JSON_RF_PULSE, rx_pkt.delay);   
      // JBI->Add(D_JSON_MILLIS, rx_pkt.received_time_millis);   
      // JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    // JBI->Object_End();
  
  

  return JBI->End();

}



  // enum DATABUILDER_JSON_METHOD{ //in order of importantance
  //   JSON_METHOD_NONE=0,
  //   JSON_METHOD_IFCHANGED,
  //   JSON_METHOD_SHORT,
  //   JSON_METHOD_DETAILED,
  //   JSON_METHOD_ALL=0
  // };
// Send all litres calculations from all methods
uint8_t mTankVolume::ConstructJSON_Litres(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();

    JBI->Object_Start("Litres");

    JBI->Add("Total", tank.volume_litres);
    JBI->Add("Usuable", tank.volume_litres_usable);
    JBI->Add("NotUsuable", tank.volume_litres_notusable);
    JBI->Add("OilHeight_cm", tank.height_of_tank_cm - pCONT_sr04->readings.average_EMA.distance_cm);

    JBI->Object_End();

  if(json_level >= JSON_LEVEL_SHORT){

    // JsonBuilderI->Add("isvalid", oiltank.smooth_1hr.isvalid);
    // JsonBuilderI->Add("litres_in_tank", oiltank.smooth_1hr.final.litres_in_tank);
    // JsonBuilderI->Add("litres_of_usable_oil", oiltank.smooth_1hr.final.litres_of_usable_oil);
    // JsonBuilderI->Add("litres_of_usable_oil_rounded", mSupport::roundfloat(oiltank.smooth_1hr.final.litres_of_usable_oil,1));
    // JsonBuilderI->Add("height_cm", (int)oiltank.smooth_1hr.final.distance_from_bottom_cm);

    // JsonBuilderI->Add("litres_per_day", history.litres_per_day);
    // JsonBuilderI->Add("days_remaining", floor(history.days_remaining));
    // JsonBuilderI->Add("weekly_usage",   floor(history.overview.litres_change));
    // JsonBuilderI->Add("daily_usage",    floor(history.overview.litres_change));

  }

  // // Only send when more than short is asked for
  // if(json_method > JSON_LEVEL_SHORT){

  // JBI->Object_Start("instant");
  //   JsonBuilderI->Add("isvalid", mSupport::roundfloat(oiltank.instant.isvalid,1));
  //   JsonBuilderI->Add("height_cm", mSupport::roundfloat(oiltank.instant.final.distance_from_bottom_cm,1));
  //   JsonBuilderI->Add("litres_usable", mSupport::roundfloat(oiltank.instant.final.litres_of_usable_oil,1));
  //   if(json_method <= JSON_LEVEL_DETAILED){
  //     JsonBuilderI->Add("height_mm", mSupport::roundfloat(oiltank.instant.final.distance_from_bottom_mm,1));
  //     JsonBuilderI->Add("litres_total", mSupport::roundfloat(oiltank.instant.final.litres_in_tank,1));
  //     JsonBuilderI->Add("last_changed", abs(millis()-oiltank.instant.final.tLastChanged));
  //   }
  // JBI->Object_End();

  // JsonObject smooth1mobj = root.createNestedObject("smooth_1m"));
  //   smooth1mobj["isvalid", oiltank.smooth_1m.isvalid);
  //   smooth1mobj["height_cm", mSupport::roundfloat(oiltank.smooth_1m.final.distance_from_bottom_cm,1));
  //   smooth1mobj["litres_usable", mSupport::roundfloat(oiltank.smooth_1m.final.litres_of_usable_oil,1));
  //   if(json_method <= JSON_LEVEL_DETAILED){
  //     smooth1mobj["height_mm", mSupport::roundfloat(oiltank.smooth_1m.final.distance_from_bottom_mm,1));
  //     smooth1mobj["litres_total", mSupport::roundfloat(oiltank.smooth_1m.final.litres_in_tank,1));
  //     smooth1mobj["last_changed", abs(millis()-oiltank.smooth_1m.final.tLastChanged));
  //   }

  // JsonObject smooth1hrobj = root.createNestedObject("smooth_1hr"));
  //   smooth1hrobj["isvalid", oiltank.smooth_1hr.isvalid);
  //   smooth1hrobj["litres_usable", mSupport::roundfloat(oiltank.smooth_1hr.final.litres_of_usable_oil,1));
  //   smooth1hrobj["height_cm", mSupport::roundfloat(oiltank.smooth_1hr.final.distance_from_bottom_cm,1));
  //   if(json_method <= JSON_LEVEL_DETAILED){
  //     smooth1hrobj["litres_total", mSupport::roundfloat(oiltank.smooth_1hr.final.litres_in_tank,1));
  //     smooth1hrobj["last_changed", abs(millis()-oiltank.smooth_1hr.final.tLastChanged));
  //     smooth1hrobj["height_mm", mSupport::roundfloat(oiltank.smooth_1hr.final.distance_from_bottom_mm,1));
  //   }
  // }

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  
  return JsonBuilderI->End();
}


uint8_t mTankVolume::ConstructJSON_Furnace(uint8_t json_level, bool json_appending){

//     D_DATA_BUFFER_CLEAR();
//     StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//     JsonObject root = doc.to<JsonObject>();

//   uint8_t ischanged=false;
  

//   // #ifdef USE_MODULE_SENSORS_DS18X
//   // for(int i=0;i<pCONT_db18->db18_sensors_active;i++){
//   //   if((pCONT_db18->db18_sensor[i].reading.ischanged)||(pCONT->mqt->fSendSingleFunctionData)){
//   //     JsonObject sensorobj = root.createNestedObject(pCONT_db18->db18_sensor[i].name.ctr);
//   //     sensorobj["temp", pCONT_db18->db18_sensor[i].reading.val;
//   //     sensorobj["isvalid"]= pCONT_db18->db18_sensor[i].reading.isvalid;
//   //   }
//   // }
//   // #endif

// // #ifdef FURNACE_MONITOR_PIN
// //   if(furnace_detect.ischanged||(pCONT->mqt->fSendSingleFunctionData)){ furnace_detect.ischanged = false;// TEMP FIX
//     root["furnace_onoff", FURNACEACTIVECTR;//mdio_mqt->input_state_detect[0].isactive;
//     root["furnace_state", FURNACEACTIVE()?1:0;//mdio_mqt->input_state_detect[0].isactive;
//     // data_buffer.payload.json_pairs++;
//   // }
//   // #endif

//   //if(data_buffer.payload.json_pairs>0){
//     data_buffer.payload.len = measureJson(root)+1;
//     serializeJson(doc,data_buffer.payload.ctr);

  JsonBuilderI->Start();

    JsonBuilderI->Add(D_JSON_CHANNELCOUNT,         0);
  
  return JsonBuilderI->End();
    
}


#endif // USE_MODULE_ENERGY_PZEM004T_V3
