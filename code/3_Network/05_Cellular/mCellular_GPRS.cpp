/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR

// General Packet Radio Service



void mCellular::GPRS_Disable()
{
  
  gprs.enabled = false;
}


void mCellular::ModemUpdate_GPRS()
{

  if(mTime::TimeReached(&gprs.tReached_Update, 20))
  {
    
    if(gprs.enabled)
    {
      
      /**
       * @brief Check connection is still active
       */              
      DEBUG_LINE_HERE;
      
      if (modem->isGprsConnected()) 
      {

      DEBUG_LINE_HERE;
        GPRS_UpdateConnectionState(true);
      DEBUG_LINE_HERE;

        gprs.signal_quality_raw = modem->getSignalQuality();

        if(gprs.signal_quality_raw == 99)
        {
          gprs.signal_quality_rssi_dbm = -150;
          pCONT_interface_network->data.cellular_state.isvalid = false;
          pCONT_interface_network->data.mqtt_state.isvalid = false;
        }
        else
        {
          gprs.signal_quality_rssi_dbm = mSupport::mapfloat(gprs.signal_quality_raw, 0, 31, -113, -51);
          pCONT_interface_network->data.cellular_state.isvalid = true;
        }

        ALOG_DBM(PSTR(D_LOG_CELLULAR "GPRS: Connected %d dBm (%d)"), int(gprs.signal_quality_rssi_dbm), gprs.signal_quality_raw);

        gprs.last_comms_millis_updated = millis();

            
        // String ccid = modem->getSimCCID();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

        // String imei = modem->getIMEI();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

        // String cop = modem->getOperator();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

        // IPAddress local = modem->localIP();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

      } 
      else 
      {
      DEBUG_LINE_HERE;
        GPRS_UpdateConnectionState(false);
      DEBUG_LINE_HERE;
        ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS status: NOT connected Downtime: %d secs"), gprs.downtime_secs);
        DataNetwork__StartConnection();
      }

    }
    else
    {
      DataNetwork__InitConfig();
    }

  }

}


/**
 * @brief Function is called anytime the connection is checked, and will update the saved variable but configure flags when needed
 * 
 * @param state 
 */
void mCellular::GPRS_UpdateConnectionState(bool state)
{

  /**
   * @brief State changed
   **/
  if(state != gprs.connected)
  {

    #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
    if(state)
    {
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(FUNC_CELLULAR_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(FUNC_NETWORK_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      gprs.reconnect_init_counts++;
    }
    else
    {
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(FUNC_CELLULAR_CONNECTION_LOST));
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(FUNC_NETWORK_CONNECTION_LOST));
  DEBUG_LINE_HERE;
    }
    #endif //ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    gprs.connected = state; // Save new state
  }

  if(state)
  {
    gprs.connected_seconds++;
    gprs.downtime_secs = 0;
  }
  else
  {
    gprs.connected_seconds = 0;
    gprs.downtime_secs++;
  }

}



#endif // USE_MODULE_NETWORK_CELLULAR