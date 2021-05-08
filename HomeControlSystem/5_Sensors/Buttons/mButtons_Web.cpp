
#include "mButtons.h"

#ifdef USE_MODULE_SENSORS_BUTTONS
    #ifdef USE_MODULE_NETWORK_WEBSERVER

void mButtons::WebAppend_Root_Draw_Table(){

  pCONT_web->WebAppend_Root_Draw_Table_Repeat_Row_Name_Numbers(buttons_found,"button_table", "Button");


}

//append to internal buffer if any root messages table
void mButtons::WebAppend_Root_Status_Table(){

  char buffer[20];

  JsonBuilderI->Array_Start_P(PM_WEB_HANDLE_DIV_NAME_BUTTON_TABLE_CTR);// Class name
  for(int row=0;row<buttons_found;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,row);
      JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_IH,IsButtonActiveCtr(row, buffer, sizeof(buffer)));//"\"%s\"", IsButtonActiveCtr(row, buffer, sizeof(buffer)));
      if(IsButtonActive(row)){
        JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#00ff00"); //make webcolours dlist in progmem!
      }else{
        JsonBuilderI->Add_P(PM_WEB_JSON_FORMAT_KEY_FC,"#ff0000");
      }
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  
}

    #endif // USE_MODULE_NETWORK_WEBSERVER

#endif