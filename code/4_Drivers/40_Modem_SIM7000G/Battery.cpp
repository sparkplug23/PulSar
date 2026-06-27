#include "_mSIM7000G.h"

#ifdef USE_MODULE_DRIVERS_MODEM_7000G



void mSIM7000G::SMS_BatteryDetailed()
{

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending Battery"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



void mSIM7000G::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR(D_LOG_CELLULAR "{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
    modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);

  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  tkr_batt_modem->readings.battery.volts_mv = modem_status.battery.volts_mv;
  tkr_batt_modem->readings.battery.percentage = modem_status.battery.percentage;
  #endif

}



#endif