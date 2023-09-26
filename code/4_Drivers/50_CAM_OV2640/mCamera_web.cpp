#include "mCamera.h"

#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
#ifdef USE_MODULE_NETWORK_WEBSERVER23

int8_t mCameraOV2640::Tasker_Web(uint8_t function){

  switch(function){    
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:

    // break;
    // case FUNC_WEB_ADD_HANDLER:
    //   WebPage_Root_AddHandlers();
    // break;
    // case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
    //   WebAppend_Root_Draw_Table();
    // break; 
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table();
    // break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:
      // WebAppend_Root_ControlUI();
      WcShowStream();
    break;
    // Generated in "InterfaceLighting" and populated in hardware classes
    // case FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON:
    //   WebAppend_JSON_RootPage_LiveviewPixels();
    // break;
  }

  return 0;

}



#endif // USE_MODULE_DRIVERS_CAMERA_OV2640
#endif // USE_MODULE_NETWORK_WEBSERVER23