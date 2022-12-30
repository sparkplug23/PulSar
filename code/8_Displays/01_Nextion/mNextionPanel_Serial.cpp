
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

//serial related stuff for easier clean up




/****************************
 * 
 * 
 * Serial handlers to allow different hardware/software serial types
 * 
 * 
 **************************************/





void mNextionPanel::init_serial()
{ 



}


int mNextionPanel::serial_available()
{ 

#ifdef USE_NEXTION_SOFTWARE_SERIAL
return swSer->available();
#else
return SERIAL_NEXTION_RX.available();
#endif


}

int mNextionPanel::serial_read()
{ 

#ifdef USE_NEXTION_SOFTWARE_SERIAL
return swSer->read();
#else
return SERIAL_NEXTION_RX.read();
#endif

}

void mNextionPanel::serial_print(const char* c_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

  swSer->print(c_str);
  #else
  SERIAL_NEXTION_TX.print(c_str);
#endif


}

void mNextionPanel::serial_print(String s_str)
{ 

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
  swSer->print(s_str);
  #else
  SERIAL_NEXTION_TX.print(s_str);
  #endif

}

void mNextionPanel::serial_print_suffix(){

  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
    SERIAL_NEXTION_TX.flush();
  #endif

}
void mNextionPanel::serial_print_suffixed(const char* c_str){

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
  swSer->print(c_str);
  #else
  SERIAL_NEXTION_TX.print(c_str);
  #endif

  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL

    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  #endif

}


#endif