#include "mSensorsDHT.h"

#ifdef USE_MODULE_SENSORS_DHT

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mSensorsDHT::WebAppend_Root_Status_Table_Draw(){

  for(int ii=0;ii<settings.sensor_active_count;ii++){ //add number in name? List needed? also hold user defined name?
    
      char name_buffer_tmp[25];
      // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

      DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID, ii, name_buffer_tmp, sizeof(name_buffer_tmp));

      uint8_t multiline_enabled = false;


    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
      JsonBuilderI->Append_P(PSTR("<td>DHT%s %s %s</td>"), "22",multiline_enabled?"Temperature":"Climate",name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
      
    if(multiline_enabled){
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
      JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
        JsonBuilderI->Append_P(PSTR("<td>DHT%s Humidity %s</td>"), "22", name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
    }
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
    JsonBuilderI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  }
}


//append to internal buffer if any root messages table
void mSensorsDHT::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;

  // JsonBuilderI->Append_P(PSTR("\"%s\":["),PSTR("tab_dht")); 
  
  JsonBuilderI->Array_Start("tab_dht");// Class name

  for(int row=0;row<(2*settings.sensor_active_count);row++){
    switch(row%2){
      default:
      case 0:{
        
        char float_ctr[10];
        char colour_ctr[10];
        char table_row[25]; memset(table_row,0,sizeof(table_row));       

        char value_ctr[8];
        pCONT_sup->dtostrfd(sensor[sensor_counter].instant.temperature,2,value_ctr);

        sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
        
        if(sensor[sensor_counter].instant.temperature<=25){
          sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
        }else
        if(sensor[sensor_counter].instant.temperature>25){
          sprintf(colour_ctr,"%s","#fcba03");
        }else{
          sprintf(colour_ctr,"%s","#ffffff");
        }
    
        // JsonBuilderI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
        //   table_row, colour_ctr
        // );
        
        JsonBuilderI->Level_Start();
          JsonBuilderI->Add("id",row);
          JsonBuilderI->Add("ih",table_row);
          JsonBuilderI->Add("fc",colour_ctr);
        JsonBuilderI->Level_End();

      }break;
      case 1:{      

        char float_ctr[10];
        char colour_ctr[10];
        char table_row[25]; memset(table_row,0,sizeof(table_row));        
        
        char value_ctr[8];
        pCONT_sup->dtostrfd(sensor[sensor_counter].instant.humidity,2,value_ctr);

        sprintf(table_row,"%s %%",value_ctr);
        
        if(sensor[sensor_counter].instant.humidity>70){
          sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
        }else
        {
          sprintf(colour_ctr,"%s","#ffffff");
        }
    
        // JsonBuilderI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
        //   table_row, colour_ctr
        // );
        
        JsonBuilderI->Level_Start();
          JsonBuilderI->Add("id",row);
          JsonBuilderI->Add("ih",table_row);
          JsonBuilderI->Add("fc",colour_ctr);
        JsonBuilderI->Level_End();

        sensor_counter++;
      }break;
    }
  }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // JsonBuilderI->Append_P(PSTR("],")); 

  JsonBuilderI->Array_End();
}


void mSensorsDHT::WebPage_Root_AddHandlers(){

  /**
   * Pages
   * */

}

#endif // USE_MODULE_NETWORK_WEBSERVER


#endif
