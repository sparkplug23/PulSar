

#ifndef _MMYJSON_H_
#define _MMYJSON_H_

#include <stdint.h>

#include <Arduino.h>
// #include "2_CoreSystem/JSON/internal/namespace.hpp"

// #include "2_CoreSystem/JSON/internal/is_char_type.hpp"
// #include "2_CoreSystem/JSON/internal/is_string_type.hpp"
// #include "2_CoreSystem/JSON/internal/is_number_type.hpp"
// #include "2_CoreSystem/JSON/internal/is_float_type.hpp"

#include "internal/is_char_type.hpp"
#include "internal/is_string_type.hpp"
#include "internal/is_number_type.hpp"
#include "internal/is_signed_number_type.hpp"
#include "internal/is_unsigned_number_type.hpp"
#include "internal/is_float_type.hpp"

#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/01_Settings/mSettings.h"

// Added 2 lines to try get AddLog into header, might cause issues
// #include "2_CoreSystem/05_Logging/mLogging.h"
#include "2_CoreSystem/mGlobalMacros.h"

// #define DEBUG_JSON_BUILDER

enum IDENTIFIER_IDS{
  IDENTIFIER_UNKNOWN_ID=0,
  IDENTIFIER_NUMBER_ID,
  IDENTIFIER_STRING_ID,
  IDENTIFIER_CHAR_ID,
  IDENTIFIER_FLOAT_ID
};

char* GetIndentifierNameByID(uint8_t id, char* buffer);
char* dtostrfd2(double number, unsigned char prec, char *s);

#define D_IDENTIFIER_NUMBER_CTR "is_number"
#define D_IDENTIFIER_STRING_CTR "is_string"
#define D_IDENTIFIER_CHAR_CTR "is_char"
#define D_IDENTIFIER_FLOAT_CTR "is_float"
DEF_PGM_CTR(PM_IDENTIFIER_NUMBER_CTR) D_IDENTIFIER_NUMBER_CTR;
DEF_PGM_CTR(PM_IDENTIFIER_STRING_CTR) D_IDENTIFIER_STRING_CTR;
DEF_PGM_CTR(PM_IDENTIFIER_CHAR_CTR)   D_IDENTIFIER_CHAR_CTR;
DEF_PGM_CTR(PM_IDENTIFIER_FLOAT_CTR)  D_IDENTIFIER_FLOAT_CTR;

class JsonBuilder{
    
  private:
    /* Prevent others from being created */
    JsonBuilder(JsonBuilder const& other) = delete;
    JsonBuilder(JsonBuilder&& other) = delete;
    /* Private constructor to prevent instancing. */
    JsonBuilder(){};
    
    struct WRITER_POINTERS{
      char*     buffer = nullptr;
      uint16_t  buffer_size = 0;
      uint16_t length;// = nullptr;
    }writer;

  public:
    // External function to get instance
    static JsonBuilder* GetInstance();
    /* Here will be the instance stored. */
    static JsonBuilder* instance;
    
    char* GetBufferPtr();
    char* GetPtr();
    uint16_t GetLength();
    uint16_t* GetLengthPtr();
    uint16_t GetBufferSize();

    void Start(char* _buffer, uint16_t _length, uint16_t _buffer_size);
    void Start();
    bool End();

    void Level_Start(const char* key);
    void Level_Start_P(const char* keyP, ...);
    void Level_Start_F(const char* keyP, ...);
    void Level_End();

    void Start_NoMemClear();

    void Array_Start();
    void Array_Start(const char* key);
    // void Array_Start_P(const char* key);
    void Array_Start_P(const char* keyP, ...);

    void Array_End();
    
    void Add_P_FV(const char* key, const char* formatP_value, ...);
    void Add_FV(const char* key, const char* formatP_value, ...);
    void Add_FV(const char* formatP_value, ...);


    void AppendBuffer(const char* formatP, ...);
    void AddKey(const char* key);
    void Level_Start(); //no key name 

    bool BufferValid(){
      if((writer.buffer == nullptr)||(writer.buffer_size == 0))
      {
        return false;
      }
      return true;
    }

    uint16_t Length(){
      return writer.length;
    };


    template <typename T>
    uint8_t getIdentifierID(T var)
    {
      bool value = 0;
      if(value = is_number_type<T>::value){ return IDENTIFIER_NUMBER_ID; }
      if(value = is_char_type<T>::value){   return IDENTIFIER_CHAR_ID;   }
      if(value = is_string_type<T>::value){ return IDENTIFIER_STRING_ID; }
      if(value = is_float_type<T>::value){    return IDENTIFIER_FLOAT_ID;}
      return IDENTIFIER_UNKNOWN_ID;
    }


    template <typename T>
    void Add(T value){
      if((writer.buffer == nullptr)||(writer.buffer_size == 0))
        return;
      
      if(
        (writer.length>1)&&
        (writer.buffer[writer.length-1]!='{')&&
        (writer.buffer[writer.length-1]!='[')      
      ){   
        writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,","); 
      }

      #ifdef DEBUG_JSON_BUILDER
      char buffer_id[50];
      uint8_t id = getIdentifierID4(value);
      GetIndentifierNameByID(id, buffer_id);
      DEBUG_PRINTF("%s id=%d %s \n\t", key, id, buffer_id);
      #endif
      
      /**
       * @brief String first
       * 
       */
      if(is_unsigned_number_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%lu",value);
      }else
      if(is_signed_number_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%d",value);
      }else
      if(is_string_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\"",value);
      }else
      if(is_char_type<T>::value){   
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"'%c'",value);
      }
      // #ifndef USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION
      else
      if(is_float_type<T>::value){ // Maybe phase out?
        // float f = static_cast<float>(value);
        float f = 0;//12.35;            
       memcpy(&f,&value,sizeof(f)); // causing crashing "stack smashing"
        char fvalue[40] = {0};         
        // dtostrfd2(f,3,fvalue);
        dtostrf(f, 5, 2, fvalue);
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s",fvalue);
      }
      // #endif // USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION

    }

    #ifdef USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION
    void Addf(float value){
      if((writer.buffer == nullptr)||(writer.buffer_size == 0))
        return;
      
      if(
        (writer.length>1)&&
        (writer.buffer[writer.length-1]!='{')&&
        (writer.buffer[writer.length-1]!='[')      
      ){         
        writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,","); 
      }

      char fvalue[40] = {0}; 
      dtostrf(value, 5, 2, fvalue);
      writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s",fvalue);
    

    }
    #endif // USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION


    template <typename T>
    void Add(const char* key, T value)
    {

      if((writer.buffer == nullptr)||(writer.buffer_size == 0))
        return;
      
      if(
        (writer.length>1)&&
        (writer.buffer[writer.length-1]!='{')&&
        (writer.buffer[writer.length-1]!='[')      
      ){ 
        writer.length += sprintf_P(&writer.buffer[writer.length],","); }

        #ifdef DEBUG_JSON_BUILDER
        char buffer_id[50];
        uint8_t id = getIdentifierID4(value);
        GetIndentifierNameByID(id, buffer_id);
        DEBUG_PRINTF("%s id=%d %s \n\t", key, id, buffer_id);
        #endif
        
          // float f = reinterpret_cast<float>(value);
        
        /**
         * @brief Order is important
         * - pointer of strings needs to be found first
         */
        if(is_unsigned_number_type<T>::value){ 
          writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":%lu",key,value);
        }else
        if(is_signed_number_type<T>::value){ 
          writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":%d",key,value);
        }else
        if(is_string_type<T>::value){ 
          writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":\"%s\"",key,value);
        }else
        if(is_char_type<T>::value){   
          writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":'%c'",key,value);
        }else
        if (is_float_type<T>::value){ 
          float f = 0;     memcpy(&f,&value,sizeof(f));
          char fvalue[20]; dtostrfd2(f,3,fvalue);
          writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\":%s",key,fvalue);
        }

        /**
         * @brief debug size
         **/
        // if(writer.length>(DATA_BUFFER_PAYLOAD_MAX_LENGTH*0.9)) // If 90% full
        // {
        //   //DEBUG_PRINTF(PSTR("WRN: writer_length = %d\n\r"), writer.length);
        // }

      }



// #ifdef TEMPLATE_JSON
    // template <typename T>
    // void Add_P_P(const char* key, T value){
//P1 is key in prgm, p2 is value in pgm


// #ifdef TEMPLATE_JSON
    template <typename T>
    void Add_P(const char* key, T value){
  if((writer.buffer == nullptr)||(writer.buffer_size == 0))
    return;
  
  if(
    (writer.length>1)&&
    (writer.buffer[writer.length-1]!='{')&&
    (writer.buffer[writer.length-1]!='[')      
  ){ writer.length += sprintf_P(&writer.buffer[writer.length],","); }

  #ifdef DEBUG_JSON_BUILDER
  char buffer_id[50];
  uint8_t id = getIdentifierID4(value);
  GetIndentifierNameByID(id, buffer_id);
  DEBUG_PRINTF("%s id=%d %s \n\t", key, id, buffer_id);
  #endif
  
  if(is_unsigned_number_type<T>::value){ 
    writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":%lu",key,value);
  }else
  if(is_signed_number_type<T>::value){ 
    writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":%d",key,value);
  }else
  if(is_string_type<T>::value){ 
    writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":\"%s\"",key,value);
  }else
  if(is_char_type<T>::value){   
    writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":'%c'",key,value);
  }else
  if(is_float_type<T>::value){ 
    float f = 0;     memcpy(&f,&value,sizeof(f));
    char fvalue[20]; dtostrfd2(f,3,fvalue);
    writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":%s",key,fvalue);

    // float f = static_cast<float>(value);
    // char fvalue[20]; dtostrfd2(f,3,fvalue);
    // writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":%s",key,fvalue);
  }

  // DEBUG_PRINTF("TEST TIME = %s\n\r",writer.buffer[0]);

}



  template <typename T, typename U>
  void Array_AddValue(T* value_arr, U value_arr_len){
  
    if(!BufferValid()){ return; }

    for(uint16_t index=0;index<value_arr_len;index++){

      // Add comma for any value after first
      if(index){ writer.length += sprintf_P(&writer.buffer[writer.length],"%s",","); }

      if(is_string_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"\"%s\"",value_arr[index]);
      }else
      if(is_signed_number_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%d",value_arr[index]);
      }else
      if(is_unsigned_number_type<T>::value){ 
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%lu",value_arr[index]);
      }else
      if(is_char_type<T>::value){   
        writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"'%c'",value_arr[index]);
      }


    //   else
    //   if(is_float_type<T>::value){ 
    //     float f = 0;  memcpy(&f,&value_arr[index],sizeof(f));
    //     char ctr[10]; dtostrfd2(f,3,ctr);
    //     writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%f",12.34);  

    // // float f = static_cast<float>(value_arr[index]);
    // // char fvalue[20]; dtostrfd2(f,3,fvalue);
    // // // writer.length += snprintf_P(&writer.buffer[writer.length],writer.buffer_size,"\"%S\":%s",key,fvalue);
    // //     writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s",fvalue);  


    //     // float f = reinterpret_cast<float>(value_arr[index*4]);
    //     // // float f = 0;  memcpy(&f,&value_arr[index],sizeof(f));
    //     // char ctr[10]; dtostrfd2(f,3,ctr);
    //     // writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s",ctr);  
    //   }
    }

  }

  
  template <typename U>
  void Array_AddValue_F(float* value_arr, U value_arr_len){
  
    if(!BufferValid()){ return; }
    char ctr[10]; 

    for(uint16_t index=0;index<value_arr_len;index++){
      if(index){ writer.length += sprintf(&writer.buffer[writer.length],"%s",","); }      // Add comma for any value after first
      writer.length += snprintf(&writer.buffer[writer.length],writer.buffer_size,"%s",dtostrfd2(value_arr[index],3,ctr));  
    }

  }


  template <typename T, typename U>
  void Array_AddArray(const char* key, T* value_arr, U value_arr_len){
    Array_Start(key);
    Array_AddValue(value_arr,value_arr_len);
    Array_End();
  }
  template <typename U>
  void Array_AddArray_F(const char* key, float* value_arr, U value_arr_len){
    Array_Start(key);
    Array_AddValue_F(value_arr,value_arr_len);
    Array_End();
  }
  // #endif

  void Append(const char* buff);
  void Append_P(const char* formatP, ...);

};

#define JsonBuilderI JsonBuilder::GetInstance()
#define JBI JsonBuilderI



#endif