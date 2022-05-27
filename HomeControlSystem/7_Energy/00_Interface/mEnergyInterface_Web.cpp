#include "mEnergyInterface.h"

#ifdef USE_MODULE_ENERGY_INTERFACE


  #ifdef USE_MODULE_NETWORK_WEBSERVER
void mEnergyInterface::WebAppend_Root_Draw_Table(){
  char buffer[30];
  BufferWriterI->Append_P(PSTR("{t}"));
  // BufferWriterI->Append_P(PSTR("<table style='border: 1px solid white;border-collapse:collapse;'>"));
  //headers
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
  for(int col=0;col<Energy.phase_count+1;col++){
    if(col==0){ //first column blank
      // BufferWriterI->Append_P(PSTR("<th>    Parameter /\nDevice      </th>"));
      BufferWriterI->Append_P(PSTR("<th></th>"));
    }else{
      // BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), ii, PM_DLIM_LIST_TABLE_HEADERS));
        BufferWriterI->Append_P(PSTR("<td>%s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_DRIVERS_ENERGY_ID,col-1,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex    
    }
  }    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  //rows
  for(int row=0;row<7;row++){
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    for(int col=0;col<Energy.phase_count+1;col++){
      if(col==0){ //row name
        BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), row, PM_DLIM_LIST_TABLE_HEADERS));
        // BufferWriterI->Append_P(PSTR("<td>%s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_DRIVERS_ENERGY_ID,row,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex
      }else{
        BufferWriterI->Append_P(PSTR("<td>{dc}%s'>" D_DEFAULT_HTML_VALUE "</div></td>"),"ener_tab");  
      }
    }
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
  }
  BufferWriterI->Append_P(PSTR("{t2}")); 
}

//append to internal buffer if any root messages table
void mEnergyInterface::WebAppend_Root_Status_Table(){

  char buffer[50];
  uint8_t count = 0;
  
  JsonBuilderI->Array_Start("ener_tab");// Class name
  for(int row=0;row<6;row++){
    for(int device=0;device<Energy.phase_count;device++){//Energy.phase_count;device++){
      JsonBuilderI->Level_Start();
        JsonBuilderI->Add("id",count++);
        //add for flag, to highlight row where power is "in use"/high
        if(pCONT_pzem->data_modbus[device].active_power>30){
          JsonBuilderI->Add("fc","#ff0000");
        }else{
          JsonBuilderI->Add("fc","#ffffff");
        }
        // JsonBuilderI->Add_FV("ih","\"c%d d%d\"", count-1, device);
        switch(row){
          case 0: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].voltage); break;
          case 1: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].current); break;
          case 2: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].active_power); break;
          case 3: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].frequency); break;
          case 4: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].power_factor); break;
          case 5: JsonBuilderI->Add("ih",pCONT_pzem->data_modbus[device].energy); break;
        } //switch      
      JsonBuilderI->Level_End();
    }
  }//end for
  JsonBuilderI->Array_End();
  

  // char colour_button[8];
  // char button_ids[] = {ANIMATION_MODE_EFFECTS_ID, ANIMATION_MODE_SCENE_ID, 
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


  #endif// USE_MODULE_NETWORK_WEBSERVER



// const char HTTP_ENERGY_SNS1[] PROGMEM =
//   "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
//   "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
//   "{s}" D_POWER_FACTOR "{m}%s{e}";

// const char HTTP_ENERGY_SNS2[] PROGMEM =
//   "{s}" D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
//   "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
//   "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

// const char HTTP_ENERGY_SNS3[] PROGMEM =
//   "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";


#endif // USE_MODULE_SENSORS_INTERFACE
