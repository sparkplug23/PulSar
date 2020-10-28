
#ifndef _GLOBAL_MACROS_H_
#define _GLOBAL_MACROS_H_ 1


#define DEFINE_PROGMEM_CTR(X) \
  const char X[] PROGMEM =
#define DEFINE_PGM_CTR(X) \
  const char X[] PROGMEM =
  

#define DEFINE_PROGMEM_CTR2(X,Y) \
  const char X[] PROGMEM = Y;

#define PRINT_FLUSHED(X) Serial.printf(X); \
                        Serial.println(); \
                        Serial.flush();

#define DEBUG_INSERT_PAGE_BREAK "\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r"

                      
#define SUBDEFINE_GPIO_TEMPLATE_PIN(X,Y) "\"" X "\":\"" Y "\","            
#define SUBDEFINE_GPIO_TEMPLATE_PIN_END(X,Y) "\"" X "\":\"" Y "\""


#define DEFINE_APP_SVALUE(X,Y) "\"" X "\":\"" Y "\","    //append
#define DEFINE_END_SVALUE(X,Y) "\"" X "\":\"" Y "\""   


#define STR_HELPER2(x) #x
#ifndef STR2
#define STR2(x) STR_HELPER2(x)
#endif


#define DEF_PGM_CTR(X) \
  const char X[] PROGMEM =
#define DEF_PGM_UINT8(X) \
  const uint8_t X[] PROGMEM =


#define SET_BUFFER_AS_GLOBAL_OR_LOCAL(X,Y) \
          if(Y == nullptr){ \
            X = pCONT_sup->global_buffer; \
          }else{ \
            X = Y; \
          }

#define BUFFER_ISREADY() strlen(data_buffer2.payload.ctr)?1:0


#endif