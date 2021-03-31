#include "mSonoffIFan.h"

#ifdef USE_MODULE_CUSTOM_SONOFF_IFAN



// int8_t mSonoffIFan::Tasker_Web(uint8_t function){


//   switch(function){
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:{

//       // create command list
//       char dlist[100]; memset(dlist,0,sizeof(dlist));
//       pCONT_sup->AppendDList(dlist, D_JSON_LIGHTPOWER);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);
//       pCONT_sup->AppendDList(dlist, D_JSON_FANSPEED);

//       uint8_t button_values[5] = {2, 0, 1, 2, 3}; //toggle, fanspeed0-3
          
//       BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,"Ceiling Fan Controls","");

//       char button_value_ctr[10];
//       char button_key_ctr[50];
//       char button_text_ctr[30];

//       BufferWriterI->Append_P(PSTR("{t}<tr>"));
//         for(uint8_t button_id=0;button_id<5;button_id++){
//           /*
          
// "<td{sw1}%d
// %%'{cs}%s
// '{bc}'%s
// '{djk}%s
// '{va}%s
// '>%s%s
// {bc2}";

//           */
//           BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2, 
//                                     100/(button_id==0?1:4),
//                                     button_id==0?"4":"", 
//                                     "buttonh",
//                                     pCONT_sup->GetTextIndexed_P(button_key_ctr, sizeof(button_key_ctr), button_id, dlist), 
//                                     pCONT_sup->p_snprintf(button_value_ctr, sizeof(button_value_ctr), "%d", button_values[button_id]),
//                                     pCONT_sup->GetTextIndexed_P(button_text_ctr, sizeof(button_text_ctr), button_id, kListFanControls),
//                                     ""
//                                 );
//           // LightPower button gets its own row
//           if(button_id==0){ BufferWriterI->Append_P(PSTR("</tr><tr>")); }
//         }
//       BufferWriterI->Append_P(PSTR("</tr>{t2}"));

//     }break;
//   }

// }


#endif // USE_MODULE_LIGHTS_INTERFACE
