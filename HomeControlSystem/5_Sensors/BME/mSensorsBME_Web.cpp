
#include "mSensorsBME.h"

    #ifdef USE_MODULE_NETWORK_WEBSERVER
void mSensorsBME::WebAppend_Root_Status_Table_Draw(){

  char value_ctr[8];
  uint8_t sensor_counter = 0;
    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>BME%s Temperature</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>BME%s Humidity</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>BME%s Pressure</td>"), "280");
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_bme",pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  
}


//append to internal buffer if any root messages table
void mSensorsBME::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  char value_ctr[8];
  char colour_ctr[10];

  JsonBuilderI->Array_Start("tab_bme");// Class name
  // BufferWriterI->Append_P(PSTR("\"%s\":["),PSTR("tab_bme")); 
  for(int row=0;row<3;row++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",row);
      switch(row){
        default:
        case 0:
        
          pCONT_sup->dtostrfd(sensor[sensor_counter].temperature,2,value_ctr);
          JsonBuilderI->Add_FV("ih",PSTR("\"%s&deg;%c\""),value_ctr,pCONT_sup->TempUnit());

          if(sensor[sensor_counter].temperature<=25){
            sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
          }else
          if(sensor[sensor_counter].temperature>25){
            sprintf(colour_ctr,"%s","#fcba03");
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }      
          JsonBuilderI->Add("fc",colour_ctr);

        break;
        case 1:    

          pCONT_sup->dtostrfd(sensor[sensor_counter].humidity,2,value_ctr);
          JsonBuilderI->Add_FV("ih",PSTR("\"%s%%\""),value_ctr);
       
          if(sensor[sensor_counter].humidity>70){
            sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }
          JsonBuilderI->Add("fc",colour_ctr);

        break;
        case 2:     
          
          pCONT_sup->dtostrfd(sensor[sensor_counter].pressure,2,value_ctr);
          JsonBuilderI->Add_FV("ih",PSTR("\"%s hPa\""),value_ctr);
          
          if(sensor[sensor_counter].pressure>1000){
            sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
          }else{
            sprintf(colour_ctr,"%s","#ffffff");
          }
          JsonBuilderI->Add("fc",colour_ctr);

        break;
      }
      JsonBuilderI->Level_End();
 
  }
  
  JsonBuilderI->Array_End();

}
#endif // USE_MODULE_NETWORK_WEBSERVER
