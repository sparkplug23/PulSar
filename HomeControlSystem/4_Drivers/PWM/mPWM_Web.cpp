
#include "mPWM.h"

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_DRIVERS_PWM

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mPWM::WebAppend_Root_Draw_Table(){

  const char kTitle_TableTitles_Root[] = 
    "Switch 0" "|" 
    "Switch 1" "|" 
    "Switch 2" "|" 
    "Switch 3" "|" 
    "Switch 4" "|" 
    "Switch 5" "|" 
    "Switch 6" "|" 
    "Switch 7" "|" 
    "Switch 8" "|" ;

 pCONT_web->WebAppend_Root_Draw_Table_dList(settings.switches_found,"switch_table", kTitle_TableTitles_Root); //add flag (or another function) that draws names with numbers after it

}

//append to internal buffer if any root messages table
void mPWM::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JsonBuilderI->Array_Start("switch_table");// Class name
  for(int row=0;row<settings.switches_found;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      JsonBuilderI->Add_FV("ih","\"%s\"", IsSwitchActive(row)?"On":"Off");
      if(IsSwitchActive(row)){
        JsonBuilderI->Add("fc","#00ff00");
      }else{
        JsonBuilderI->Add("fc","#ff0000");
      }
    
    JsonBuilderI->Level_End();
  }
  JsonBuilderI->Array_End();
  
}


#endif// USE_MODULE_NETWORK_WEBSERVER

#endif