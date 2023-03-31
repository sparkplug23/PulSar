#include "mGPS.h"
#include "5_Sensors/50_GPS_Serial/mGPS_UBlox.h"

/***
 * Modes: Polling, Interrupt, Being feed from another module aka ringbuffers
 * 
 * */

#ifdef USE_MODULE_DRIVERS_GPS

MyGPS gps( &gpsPort );

void mGPS::send_UBX_enable_messages()
{
  bool enabled_msg_with_time = false;

  #if defined(UBLOX_PARSE_POSLLH)
    if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH ))
      DEBUG_PORT.println( F("enable POSLLH failed!") );

    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_PVT)
    if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_PVT ))
      DEBUG_PORT.println( F("enable PVT failed!") );

    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_VELNED)
    if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED ))
      DEBUG_PORT.println( F("enable VELNED failed!") );

    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_DOP)
    if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP ))
      DEBUG_PORT.println( F("enable DOP failed!") );
    else
      DEBUG_PORT.println( F("enabled DOP.") );

    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_SVINFO)
    if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_SVINFO ))
      DEBUG_PORT.println( F("enable SVINFO failed!") );
    
    enabled_msg_with_time = true;
  #endif

  #if defined(UBLOX_PARSE_TIMEUTC)

    #if defined(GPS_FIX_TIME) & defined(GPS_FIX_DATE)
      if (enabled_msg_with_time &&
          !ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
        DEBUG_PORT.println( F("disable TIMEUTC failed!") );

    #elif defined(GPS_FIX_TIME) | defined(GPS_FIX_DATE)
      // If both aren't defined, we can't convert TOW to UTC,
      // so ask for the separate UTC message.
      if (!ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC ))
        DEBUG_PORT.println( F("enable TIMEUTC failed!") );
    #endif

  #endif

  // trace_header( DEBUG_PORT );

} // start_running


void mGPS::disableUBX()
{
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  ubx_parser->disable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
}

void mGPS::enableUBX()
{
  ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
}

uint8_t mGPS::enableUBX_RequiredOnlyFor3DTracking()
{
  bool messages_confirmed_as_enabled_by_readback = false;
  messages_confirmed_as_enabled_by_readback += ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEGPS );
  messages_confirmed_as_enabled_by_readback += ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_TIMEUTC );
  messages_confirmed_as_enabled_by_readback += ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_VELNED );
  messages_confirmed_as_enabled_by_readback += ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_POSLLH );
  messages_confirmed_as_enabled_by_readback += ubx_parser->enable_msg( ublox::UBX_NAV, ublox::UBX_NAV_DOP );
  return messages_confirmed_as_enabled_by_readback;
}





void mGPS::sendUBX( const unsigned char *progmemBytes, size_t len )
{
  gpsPort.write( 0xB5 ); // SYNC1
  gpsPort.write( 0x62 ); // SYNC2

  uint8_t a = 0, b = 0;
  while (len-- > 0) {
    uint8_t c = pgm_read_byte( progmemBytes++ );
    a += c;
    b += a;
    gpsPort.write( c );
  }

  gpsPort.write( a ); // CHECKSUM A
  gpsPort.write( b ); // CHECKSUM B

} // sendUBX



#endif
