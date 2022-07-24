#include "mRelays.h"

#ifdef USE_MODULE_DRIVERS_RELAY_V2


// #ifndef DISABLE_WEBSERVER
// void mRelays::WebAppend_Root_Add_Buttons(){

//   if(!settings.relays_connected){
//     return;
//   }

//   char button_text_ctr[30];
//   char relay_name_ctr[30];
//   char dlist_json_template[100];
  
//   //PSTR CRASHED!!
//   BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,"Relay Controls","");//PSTR("Relay Controls"),"");

//   BufferWriterI->Append_P(PSTR("{t}<tr>"));
//     for(uint8_t button_id=0;button_id<settings.relays_connected;button_id++){
//       // Create json template
//       snprintf(dlist_json_template, sizeof(dlist_json_template), 
//         "{\\\"" D_JSON_POWERNAME "\\\":\\\"%s\\\",\\\"" D_JSON_ONOFF "\\\":\\\"%s\\\"}",
//         GetRelayNamebyIDCtr(button_id, relay_name_ctr, sizeof(relay_name_ctr)),
//         "%s"
//       );
//       // Build button
//       BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_KEY_TEMPLATED_VARIABLE_INSERTS_HANDLE_IHR2, 
//                                 100/settings.relays_connected,
//                                 "", 
//                                 "buttonh " "reltog",
//                                 dlist_json_template, 
//                                 D_DEVICE_CONTROL_BUTTON_TOGGLE_CTR, //button_value_ctr
//                                 GetRelayNameWithStateLongbyIDCtr(button_id, button_text_ctr, sizeof(button_text_ctr)),
//                                 ""
//                               );
//     }
//   BufferWriterI->Append_P(PSTR("</tr>{t2}"));

// }



// void mRelays::WebAppend_Root_Draw_PageTable(){

//   if(!settings.relays_connected){
//     return;
//   }
//   char buffer[50];
  
//   DEBUG_LINE;
//   if(settings.fShowTable){
//     for(int ii=0;ii<settings.relays_connected;ii++){
//       BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//         BufferWriterI->Append_P(PSTR("<td>%s</td>"), GetRelayNamebyIDCtr(ii,buffer,sizeof(buffer)));
//         BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"relpow_tab","?");   
//       BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//     }    
//   }

// }


// //append to internal buffer if any root messages table
// void mRelays::WebAppend_Root_Status_Table(){
  
//   if(!settings.relays_connected){
//     return;
//   }
//   char buffer[20]; memset(buffer,0,sizeof(buffer));

//   if(settings.fShowTable){
//     JsonBuilderI->Array_Start("relpow_tab");// Class name
//     for(int row=0;row<settings.relays_connected;row++){  
//       JsonBuilderI->Level_Start();
//         JsonBuilderI->Add("id",row);
//         JsonBuilderI->Add("ih",CommandGet_Relay_Power(row) ? PSTR("ON") : PSTR("Off"));
//       JsonBuilderI->Level_End();
//     }
//     JsonBuilderI->Array_End();  
//   }

//   JsonBuilderI->Array_Start("reltog");// Class name
//   for(int row=0;row<settings.relays_connected;row++){  
//     JsonBuilderI->Level_Start();
//       JsonBuilderI->Add("id",row);
//       JsonBuilderI->Add("ih",GetRelayNameWithStateLongbyIDCtr(row, buffer, sizeof(buffer)));
//       JsonBuilderI->Add("bc",CommandGet_Relay_Power(row) ? "#00ff00" : "#ee2200");    
//     JsonBuilderI->Level_End();
//   }  
//   JsonBuilderI->Array_End();

// }
// #endif // DISABLE_WEBSERVER




#endif

