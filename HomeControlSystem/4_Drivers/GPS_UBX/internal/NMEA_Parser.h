#ifndef NMEAGPS_H
#define NMEAGPS_H

//  Copyright (C) 2014-2017, SlashDevin
//  Modified           2021, Sparkplug Michael
//
//  This file is part of NeoGPS
//
//  NeoGPS is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  NeoGPS is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with NeoGPS.  If not, see <http://www.gnu.org/licenses/>.


// NMEA is the primary parser, gpsfix holds the saved data


/**
 * Move all "internal" ubx into its own library??
 * 
 * 
 * 
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_GPS



 * */

// #include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_DRIVERS_GPS


#define PGM_P const char *
typedef PGM_P str_P;
#define __PROGMEM PROGMEM

#include <Arduino.h>

#include "GPSfix.h"
#include "4_Drivers/GPS_UBX/internal/configs/NMEAGPS_cfg.h"

// #include "2_CoreSystem/Logging/mLogging.h"

//------------------------------------------------------
//
// NMEA 0183 Parser for generic GPS Modules.
//
// As bytes are received from the device, they affect the
// internal FSM and set various members of the current /fix/.
// As multiple sentences are received, they are (optionally)
// merged into a single fix.  When the last sentence in a time
// interval (usually 1 second) is received, the fix is stored
// in the (optional) buffer of fixes.
//
// Only these NMEA messages are parsed:
//      GGA, GLL, GSA, GST, GSV, RMC, VTG, and ZDA.

class NMEAGPS
{
    // NMEAGPS & operator =( const NMEAGPS & );
    // NMEAGPS( const NMEAGPS & );

public:

    NMEAGPS();

    //.......................................................................
    // NMEA standard message types (aka "sentences")

    enum nmea_msg_t {
        NMEA_UNKNOWN,

        #if defined(NMEAGPS_PARSE_GGA) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_GGA,
        #endif

        #if defined(NMEAGPS_PARSE_GLL) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_GLL,
        #endif

        #if defined(NMEAGPS_PARSE_GSA) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_GSA,
        #endif

        #if defined(NMEAGPS_PARSE_GST) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_GST,
        #endif

        #if defined(NMEAGPS_PARSE_GSV) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_GSV,
        #endif

        #if defined(NMEAGPS_PARSE_RMC) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_RMC,
        #endif

        #if defined(NMEAGPS_PARSE_VTG) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_VTG,
        #endif

        #if defined(NMEAGPS_PARSE_ZDA) | defined(NMEAGPS_RECOGNIZE_ALL)
          NMEA_ZDA,
        #endif

        NMEAMSG_END // a bookend that tells how many enums there were
      };

    CONST_CLASS_DATA nmea_msg_t NMEA_FIRST_MSG = (nmea_msg_t) (NMEA_UNKNOWN+1);
    CONST_CLASS_DATA nmea_msg_t NMEA_LAST_MSG  = (nmea_msg_t) (NMEAMSG_END-1);

    //=======================================================================
    // FIX-ORIENTED methods: available, read, overrun and handle
    //=======================================================================
    // The typical sketch should have something like this in loop():
    //
    //    while (gps.available( Serial1 )) {
    //      GPS_FIX fix = gps.read();
    //      if (fix.valid.location) {
    //         ...
    //      }
    //    }

    //.......................................................................
    // The available(...) functions return the number of *fixes* that
    //   are available to be "read" from the fix buffer.  The GPS port
    //   object is passed in so a char can be read if port.available().

    uint8_t available( Stream & port )
    {
      if (processing_style == PS_POLLING){
        // Serial.printf("processing_style=%d\n\r",processing_style);
        // if(port.available()>5){
        //   // Serial.printf("port.available()=%d\n\r",port.available());
        // }
        while (port.available()){
          handle( port.read() );
          // Serial.print( F("handle: ") );
        }
      }
      // if(_available()){
      //   Serial.println("available");
      // }
      return _available();
    }
    uint8_t available() const volatile { return _available(); };

/**
    // Need a new method from "available" that is more describtive
*
*
**/

    uint8_t parser_byte_in(uint8_t c)
    {
      // Serial.print(c);
      if (processing_style == PS_POLLING){
        handle( c );
      }
      return _available();
    }




    //.......................................................................
    // Return the next available fix.  When no more fixes
    //   are available, it returns an empty fix.

    const GPS_FIX read();
    GPS_FIX* read2();

    //.......................................................................
    //  The OVERRUN flag is set whenever a fix is not read by the time
    //  the next update interval starts.  You must clear it when you
    //  detect the condition.

    bool overrun() const { return _overrun; }
    void overrun( bool val ) { _overrun = val; }

    //.......................................................................
    // As characters are processed, they can be categorized as
    // INVALID (not part of this protocol), OK (accepted),
    // or COMPLETED (end-of-message).

    enum decode_t { DECODE_CHR_INVALID, DECODE_CHR_OK, DECODE_COMPLETED };

    //.......................................................................
    //  Process one character, possibly saving a buffered fix.
    //    It implements merging and coherency.
    //    This can be called from an ISR.

    decode_t handle( uint8_t c );

    //=======================================================================
    // CHARACTER-ORIENTED methods: decode, fix and is_safe
    //=======================================================================
    //
    //    *** MOST APPLICATIONS SHOULD USE THE FIX-ORIENTED METHODS ***
    //
    //    Using `decode` is only necessary if you want finer control
    //    on how fix information is filtered and merged.
    //
    // Process one character of an NMEA GPS sentence.  The internal state
    // machine tracks what part of the sentence has been received.  As the
    // sentence is received, members of the /fix/ structure are updated.
    // This character-oriented method *does not* buffer any fixes, and
    // /read()/ will always return an empty fix.
    //
    // @return DECODE_COMPLETED when a sentence has been completely received.

    NMEAGPS_VIRTUAL decode_t decode( char c );

    #ifdef ENABLE_DEVFEATURE_DECODE_FUNCTION_AS_CALLBACK


    #else


    #endif

    //.......................................................................
    //  Current fix accessor.
    //    *** MOST APPLICATIONS SHOULD USE read() TO GET THE CURRENT FIX  ***
    //    /fix/ will be constantly changing as characters are received.
    //
    //  For example, fix().longitude() may return nonsense data if
    //  characters for that field are currently being processed in /decode/.

    GPS_FIX & fix() { return m_fix; };

    //  NOTE: /is_safe/ *must* be checked before accessing members of /fix/.
    //  If you need access to the current /fix/ at any time, you should
    //  use the FIX-ORIENTED methods.

    //.......................................................................
    //  Determine whether the members of /fix/ are "currently" safe.
    //  It will return true when a complete sentence and the CRC characters
    //  have been received (or after a CR if no CRC is present).
    //  It will return false after a new sentence starts.

    bool is_safe() const volatile { return (rxState == NMEA_IDLE); }

    //  NOTE:  When INTERRUPT_PROCESSING is enabled, is_safe()
    //  and fix() could change at any time (i.e., they should be
    //  considered /volatile/).

    //=======================================================================
    // DATA MEMBER accessors and mutators
    //=======================================================================

    //.......................................................................
    //  Convert a nmea_msg_t to a PROGMEM string.
    //    Useful for printing the sentence type instead of a number.
    //    This can return "UNK" if the message is not a valid number.

    const __FlashStringHelper *string_for( nmea_msg_t msg ) const;

    //.......................................................................
    // Most recent NMEA sentence type received.

    nmea_msg_t nmeaMessage NEOGPS_BF(8);

    //.......................................................................
    //  Storage for Talker and Manufacturer IDs

    #ifdef NMEAGPS_SAVE_TALKER_ID
      char talker_id[2];
    #endif

    #ifdef NMEAGPS_SAVE_MFR_ID
      char mfr_id[3];
    #endif

  #define NMEAGPS_PARSE_SAVE_MILLIS

    // Add millis() count for when the above parser occur and are valid, so I know age of NMEA message
    #ifdef NMEAGPS_PARSE_SAVE_MILLIS
    struct ACTIVE_MILLIS{
      uint32_t GGA=0;
      uint32_t GLL=0;
      uint32_t GSA=0;
      uint32_t GST=0;
      uint32_t GSV=0;
      uint32_t RMC=0;
      uint32_t VTG=0;
      uint32_t ZDA=0;
    }active_millis;
    #endif
    //.......................................................................
    //  Various parsing statistics

    #ifdef NMEAGPS_STATS
      struct statistics_t {
          uint32_t ok;     // count of successfully parsed sentences
          uint32_t errors; // NMEA checksum or other message errors
          uint32_t chars;
          void init()
            {
              ok     = 0L;
              errors = 0L;
              chars  = 0L;

              // active_millis.GGA = millis();
              // active_millis.GLL = millis();
              // active_millis.GSA = millis();
              // active_millis.GST = millis();
              // active_millis.GSV = millis();
              // active_millis.RMC = millis();
              // active_millis.VTG = millis();
              // active_millis.ZDA = millis();

            }
      } statistics;
    #endif

    //.......................................................................
    // SATELLITE VIEW array

    #ifdef NMEAGPS_PARSE_SATELLITES
      struct satellite_view_t
      {
        uint8_t    id;
        #ifdef NMEAGPS_PARSE_SATELLITE_INFO
          uint8_t  elevation; // 0..99 deg
          uint16_t azimuth;   // 0..359 deg
          uint8_t  snr     NEOGPS_BF(7); // 0..99 dBHz
          bool     tracked NEOGPS_BF(1);
        #endif
      } NEOGPS_PACKED;

      satellite_view_t satellites[ NMEAGPS_MAX_SATELLITES ];
      uint8_t          sat_count; // in the above array

      bool satellites_valid() const { return (sat_count >= m_fix.satellites); }
    #endif

    //.......................................................................
    // Reset the parsing process.
    //   This is used internally after a CS error, or could be used
    //   externally to abort processing if it has been too long
    //   since any data was received.

    void reset()
    {
      rxState = NMEA_IDLE;
    }

    //=======================================================================
    // CORRELATING Arduino micros() WITH UTC.
    //=======================================================================

    #if defined(NMEAGPS_TIMESTAMP_FROM_PPS) |  \
        defined(NMEAGPS_TIMESTAMP_FROM_INTERVAL)
      protected:
        uint32_t _UTCsecondStart;
        #if defined(NMEAGPS_TIMESTAMP_FROM_INTERVAL) & \
            ( defined(GPS_FIX_DATE) | defined(GPS_FIX_TIME) )
          uint32_t _IntervalStart; // quiet time just ended
        #endif
      public:

        // The micros() value when the current UTC second started
        uint32_t UTCsecondStart() const
          { lock();
              uint32_t ret = _UTCsecondStart;
            unlock();
            return ret;
          };
        void     UTCsecondStart( uint32_t us ) { _UTCsecondStart = us; };

        // The elapsed time since the start of the current UTC second
        uint32_t UTCus() const { return micros() - UTCsecondStart(); };
        uint32_t UTCms() const { return UTCus() / 1000UL; };

        // If you have attached a Pin Change interrupt routine to the PPS pin:
        //
        //     const int PPSpin = 5;
        //     void PPSisr() { gps.UTCsecondStart( micros() ); };
        //     void setup()
        //     {
        //       attachInterrupt( digitalPinToInterrupt(PPSpin), PPSisr, RISING );
        //     }
        //
        // If you are using an Input Capture pin, calculate the elapsed
        //   microseconds since the capture time (based on the TIMER
        //   frequency):
        //
        // void savePPSus() // called as an ISR or from a test in loop
        // {
        //    uint32_t elapsedUS = (currentCount - captureCount) * countUS;
        //    gps.UTCsecondStart( micros() - elapsedUS );
        // }
    #endif

    //=======================================================================
    // COMMUNICATING WITH THE GPS DEVICE: poll, send and send_P
    //=======================================================================

    //.......................................................................
    // Request the specified NMEA sentence.  Not all devices will respond.

    static void poll( Stream *device, nmea_msg_t msg );

    //.......................................................................
    // Send a message to the GPS device.
    // The '$' is optional, and the '*' and CS will be added automatically.

    static void send( Stream *device, const char *msg );
    static void send_P( Stream *device, const __FlashStringHelper *msg );

//lets just place it back here for clarity
  // #include "NMEAGPSprivate.h"

  
  protected:
    //.......................................................................
    // Table entry for NMEA sentence type string and its offset
    // in enumerated nmea_msg_t.  Proprietary sentences can be implemented
    // in derived classes by adding a second table.  Additional tables
    // can be singly-linked through the /previous/ member.  The instantiated
    // class's table is the head, and should be returned by the derived
    // /msg_table/ function.  Tables should be sorted alphabetically.

    struct msg_table_t {
      uint8_t             offset;  // nmea_msg_t enum starting value
      const msg_table_t  *previous;
      uint8_t             size;    // number of entries in table array
      const char * const *table;   // array of NMEA sentence strings
    };

    static const msg_table_t  nmea_msg_table __PROGMEM;

    NMEAGPS_VIRTUAL const msg_table_t *msg_table() const
      { return &nmea_msg_table; };

    //.......................................................................
    //  These virtual methods can accept or reject
    //    the talker ID (for standard sentences) or
    //    the manufacturer ID (for proprietary sentences).
    //  The default is to accept *all* IDs.
    //  Override them if you want to reject certain IDs, or you want
    //    to handle COMPLETED sentences from certain IDs differently.

    #ifdef NMEAGPS_PARSE_TALKER_ID
      NMEAGPS_VIRTUAL bool parseTalkerID( char ) { return true; };
    #endif

    #ifdef NMEAGPS_PARSE_PROPRIETARY
      #ifdef NMEAGPS_PARSE_MFR_ID
        NMEAGPS_VIRTUAL bool parseMfrID( char ) { return true; };
      #endif
    #endif

  public:
    //.......................................................................
    // Set all parsed data to initial values.

    void data_init()
    {
      fix().init();

      #ifdef NMEAGPS_PARSE_SATELLITES
        sat_count = 0;
      #endif
    }

    //.......................................................................

    enum merging_t { NO_MERGING, EXPLICIT_MERGING, IMPLICIT_MERGING };
    static const merging_t
      merging = NMEAGPS_MERGING; // see NMEAGPS_cfg.h

    enum processing_style_t { PS_POLLING, PS_INTERRUPT };
    static const processing_style_t
      processing_style = NMEAGPS_PROCESSING_STYLE;  // see NMEAGPS_cfg.h

    static const bool keepNewestFixes = NMEAGPS_KEEP_NEWEST_FIXES;

    static const bool validateChars () { return NMEAGPS_VALIDATE_CHARS; }
    static const bool validateFields() { return NMEAGPS_VALIDATE_FIELDS; }

    //.......................................................................
    //  Control access to this object.  This preserves atomicity when
    //     the processing style is interrupt-driven.

    void lock() const
      {
        if (processing_style == PS_INTERRUPT)
          noInterrupts();
      }

    void unlock() const
      {
        if (processing_style == PS_INTERRUPT)
          interrupts();
      }

  protected:
    //=======================================================================
    //   PARSING FINITE-STATE MACHINE
    //=======================================================================

    //  Current fix
    GPS_FIX m_fix;

    // Current parser state
    uint8_t      crc;            // accumulated CRC in the sentence
    uint8_t      fieldIndex;     // index of current field in the sentence
    uint8_t      chrCount;       // index of current character in current field
    uint8_t      decimal;        // digits received after the decimal point
    struct {
      bool     negative          NEOGPS_BF(1); // field had a leading '-'
      bool     _comma_needed     NEOGPS_BF(1); // field needs a comma to finish parsing
      bool     group_valid       NEOGPS_BF(1); // multi-field group valid
      bool     _overrun          NEOGPS_BF(1); // an entire fix was dropped
      bool     _intervalComplete NEOGPS_BF(1); // automatically set after LAST received
      #if (NMEAGPS_FIX_MAX == 0)
        bool   _fixesAvailable   NEOGPS_BF(1);
      #endif
      #ifdef NMEAGPS_PARSE_PROPRIETARY
        bool   proprietary       NEOGPS_BF(1); // receiving proprietary message
      #endif
    } NEOGPS_PACKED;

    #ifdef NMEAGPS_PARSING_SCRATCHPAD
      union {
        uint32_t U4;
        uint16_t U2[2];
        uint8_t  U1[4];
      } scratchpad;
    #endif

    bool comma_needed()
    {
      #ifdef NMEAGPS_COMMA_NEEDED
        return _comma_needed;
      #else
        return false;
      #endif
    }

    void comma_needed( bool value )
    {
      #ifdef NMEAGPS_COMMA_NEEDED
        _comma_needed = value;
      #endif
    }

    // Internal FSM states
    enum rxState_t {
        NMEA_IDLE,             // Waiting for initial '$'
        NMEA_RECEIVING_HEADER, // Parsing sentence type field
        NMEA_RECEIVING_DATA,   // Parsing fields up to the terminating '*'
        NMEA_RECEIVING_CRC     // Receiving two-byte transmitted CRC
    };
    CONST_CLASS_DATA uint8_t NMEA_FIRST_STATE = NMEA_IDLE;
    CONST_CLASS_DATA uint8_t NMEA_LAST_STATE  = NMEA_RECEIVING_CRC;

    rxState_t rxState NEOGPS_BF(8);

    //.......................................................................

    uint8_t _available() const volatile { 
      
    // Serial.print( F("handle: ") );
    if(_fixesAvailable){
// Serial.printf( "_fixesAvailable: %d\n\r",_fixesAvailable );
    }
    return _fixesAvailable; 
    
    };

    //.......................................................................
    //  Buffered fixes.

    #if (NMEAGPS_FIX_MAX > 0)
      GPS_FIX buffer[ NMEAGPS_FIX_MAX ]; // could be empty, see NMEAGPS_cfg.h
      uint8_t _fixesAvailable;
      uint8_t _firstFix;
      uint8_t _currentFix;
    #endif

    //.......................................................................
    // Indicate that the next sentence should initialize the internal data.
    //    This is useful for coherency or custom filtering.

    bool intervalComplete() const { return _intervalComplete; }
    void intervalComplete( bool val ) { _intervalComplete = val; }

    //.......................................................................
    //  Identify when an update interval is completed, according to the
    //  most recently-received sentence.  In this base class, it just
    //  looks at the nmeaMessage member.  Derived classes may have
    //  more complex, specific conditions.

    NMEAGPS_VIRTUAL bool intervalCompleted() const
      { return (nmeaMessage == LAST_SENTENCE_IN_INTERVAL); }
                               // see NMEAGPS_cfg.h

    //.......................................................................
    //  When a fix has been fully assembled from a batch of sentences, as
    //  determined by the configured merging technique and ending with the
    //  LAST_SENTENCE_IN_INTERVAL, it is stored in the (optional) buffer
    //  of fixes.  They are removed with /read()/.

    void storeFix();

    //=======================================================================
    //   PARSING METHODS
    //=======================================================================

    //.......................................................................
    // Try to recognize an NMEA sentence type, after the IDs have been accepted.

    decode_t parseCommand( char c );
    decode_t parseCommand( const msg_table_t *msgs, uint8_t cmdCount, char c );

    //.......................................................................
    // Parse various NMEA sentences

    bool parseGGA( char chr );
    bool parseGLL( char chr );
    bool parseGSA( char chr );
    bool parseGST( char chr );
    bool parseGSV( char chr );
    bool parseRMC( char chr );
    bool parseVTG( char chr );
    bool parseZDA( char chr );


    //.......................................................................
    // Depending on the NMEA sentence type, parse one field of an expected type.

    NMEAGPS_VIRTUAL bool parseField( char chr );

    //.......................................................................
    // Parse the primary NMEA field types into /fix/ members.

    bool parseFix        ( char chr ); // aka STATUS or MODE
    bool parseTime       ( char chr );
    bool parseDDMMYY     ( char chr );
    bool parseLat        ( char chr );
    bool parseNS         ( char chr );
    bool parseLon        ( char chr );
    bool parseEW         ( char chr );
    bool parseSpeed      ( char chr );
    bool parseSpeedKph   ( char chr );
    bool parseHeading    ( char chr );
    bool parseAlt        ( char chr );
    bool parseGeoidHeight( char chr );
    bool parseHDOP       ( char chr );
    bool parseVDOP       ( char chr );
    bool parsePDOP       ( char chr );
    bool parse_lat_err   ( char chr );
    bool parse_lon_err   ( char chr );
    bool parse_alt_err   ( char chr );
    bool parseSatellites ( char chr );

    // Helper macro for parsing the 4 consecutive fields of a location
    #define PARSE_LOC(i) case i: return parseLat( chr );\
      case i+1: return parseNS ( chr ); \
      case i+2: return parseLon( chr ); \
      case i+3: return parseEW ( chr );

    //.......................................................................
    // Parse floating-point numbers into a /whole_frac/
    // @return true when the value is fully populated.

    bool parseFloat( GPS_FIX::whole_frac & val, char chr, uint8_t max_decimal );

    //.......................................................................
    // Parse floating-point numbers into a uint16_t
    // @return true when the value is fully populated.

    bool parseFloat( uint16_t & val, char chr, uint8_t max_decimal );

    //.......................................................................
    // Parse NMEA lat/lon dddmm.mmmm degrees

    bool parseDDDMM
      (
        #if defined( GPS_FIX_LOCATION )
          int32_t & val,
        #endif
        #if defined( GPS_FIX_LOCATION_DMS )
          DMS_t & dms,
        #endif
        char chr
      );

    //.......................................................................
    // Parse integer into 8-bit int
    // @return true when non-empty value

    bool parseInt( uint8_t &val, uint8_t chr )
    {
      negative = false;
      bool is_comma = (chr == ',');

      if (chrCount == 0) {
        if (is_comma)
          return false; // empty field!

        if (((validateChars() || validateFields()) && (chr == '-')) ||
            (validateChars() && !isdigit( chr )))
          sentenceInvalid();
        else
          val = (chr - '0');

      } else if (!is_comma) {

        if (validateChars() && !isdigit( chr ))
          sentenceInvalid();
        else
          val = (val*10) + (chr - '0');
      }
      return true;
    }

    //.......................................................................
    // Parse integer into signed 8-bit int
    // @return true when non-empty value

    bool parseInt( int8_t &val, uint8_t chr )
    {
      bool is_comma = (chr == ',');

      if (chrCount == 0) {
        if (is_comma)
          return false; // empty field!

        negative = (chr == '-');
        if (negative) {
          comma_needed( true ); // to negate
          val = 0;
        } else if (validateChars() && !isdigit( chr )) {
          sentenceInvalid();
        } else {
          val = (chr - '0');
        }
      } else if (!is_comma) {
        val = (val*10) + (chr - '0');

      } else if (negative) {
        val = -val;
      }

      return true;
    }

    //.......................................................................
    // Parse integer into 16-bit int
    // @return true when non-empty value

    bool parseInt( uint16_t &val, uint8_t chr )
    {
      negative = false;

      bool is_comma = (chr == ',');
      if (chrCount == 0) {
        if (is_comma)
          return false; // empty field!

        if (((validateChars() || validateFields()) && (chr == '-')) ||
            (validateChars() && !isdigit( chr )))
          sentenceInvalid();
        else
          val = (chr - '0');

      } else if (!is_comma) {

        if (validateChars() && !isdigit( chr ))
          sentenceInvalid();
        else
          val = (val*10) + (chr - '0');
      }
      return true;
    }

    //.......................................................................
    // Parse integer into 32-bit int
    // @return true when non-empty value

    bool parseInt( uint32_t &val, uint8_t chr )
    {
      negative = false;

      bool is_comma = (chr == ',');
      if (chrCount == 0) {
        if (is_comma)
          return false; // empty field!

        if (((validateChars() || validateFields()) && (chr == '-')) ||
            (validateChars() && !isdigit( chr )))
          sentenceInvalid();
        else
          val = (chr - '0');

      } else if (!is_comma) {

        if (validateChars() && !isdigit( chr ))
          sentenceInvalid();
        else
          val = (val*10) + (chr - '0');
      }
      return true;
    }

  private:
    void sentenceBegin       ();
    void sentenceOk          ();
    void sentenceInvalid     ();
    void sentenceUnrecognized();
    void headerReceived      ();



} NEOGPS_PACKED;

#endif


//https://www.trimble.com/OEM_ReceiverHelp/V4.44/en/NMEA-0183messages_MessageOverview.html


/**
 * @note Details of each packet
*/

/** NMEAGPS_PARSE_RMC
 * @brief RMC
 * @note Position, Velocity, and Time
 * 
 * The RMC string is:
    $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

    GPRMC message fields
    Field	Meaning
    0	Message ID $GPRMC
    1	UTC of position fix
    2	Status A=active or V=void
    3	Latitude
    4	Longitude
    5	Speed over the ground in knots
    6	Track angle in degrees (True)
    7	Date
    8	Magnetic variation in degrees
    9	The checksum data, always begins with

  * */

/** NMEAGPS_PARSE_GLL
 * @brief GLL
 * @note Position data: position fix, time of position fix, and status
 * 
 * The RMC string is:
    $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

    GPRMC message fields
    Field	Meaning
    0	Message ID $GPRMC
    1	UTC of position fix
    2	Status A=active or V=void
    3	Latitude
    4	Longitude
    5	Speed over the ground in knots
    6	Track angle in degrees (True)
    7	Date
    8	Magnetic variation in degrees
    9	The checksum data, always begins with

  * */

/** NMEAGPS_PARSE_GGA
 * @brief GGA
 * @note Time, position, and fix related data
 * 
 * The RMC string is:
    $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

    Field	Meaning
    0	Message ID $GPGGA
    1	UTC of position fix
    2	Latitude
    3	Direction of latitude:
      N: North
      S: South
    4	Longitude
    5	Direction of longitude:
      E: East
      W: West
    6	GPS Quality indicator:
      0: Fix not valid
      1: GPS fix
      2: Differential GPS fix, OmniSTAR VBS
      4: Real-Time Kinematic, fixed integers
      5: Real-Time Kinematic, float integers, OmniSTAR XP/HP or Location RTK
    7	Number of SVs in use, range from 00 through to 24+
    8	HDOP
    9	Orthometric height (MSL reference)
    10	M: unit of measure for orthometric height is meters
    11	Geoid separation
    12	M: geoid separation measured in meters
    13	Age of differential GPS data record, Type 1 or Type 9. Null field when DGPS is not used.
    14	Reference station ID, range 0000-4095. A null field when any reference station ID is selected and no corrections are received1.
    15	
    The checksum data, always begins with *

    Note – If a user-defined geoid model, or an inclined plane is loaded into the receiver, then the height output in the NMEA GGA string 
    is always the orthometric height (height above a geoid). The orthometric height is output even if no user-defined geoid is loaded 
    (there is a simplified default geoid in the receiver), or if a user-defined geoid is loaded, or if an inclined plane is used.

  * */


/** NMEAGPS_PARSE_GSA
 * @brief GSA
 * @note GPS DOP and active satellites
 * 
 * The GSA string is:
    $GPGSA,<1>,<2>,<3>,<3>,,,,,<3>,<3>,<3>,<4>,<5>,<6>*<7><CR><LF>

  GSA message fields
  Field	Meaning
  0	Message ID $GPGSA
  1	Mode 1, M = manual, A = automatic
  2	Mode 2, Fix type, 1 = not available, 2 = 2D, 3 = 3D
  3	PRN number, 01 through 32 for GPS, 33 through 64 for SBAS, 64+ for GLONASS
  4	PDOP: 0.5 through 99.9
  5	HDOP: 0.5 through 99.9
  6	VDOP: 0.5 through 99.9
  7	The checksum data, always begins with *

  * */


/** NMEAGPS_PARSE_GSV
 * @brief GSV
 * @note GSV 	Number of SVs in view, PRN, elevation, azimuth, and SNR
 * 
 Satellite information
  The GSV message string identifies the number of SVs in view, the PRN numbers, elevations, azimuths, and SNR values. An example of the GSV message string is:

  $GPGSV,4,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*67

  GSV message fields
  Field	Meaning
  0	Message ID $GPGSV
  1	Total number of messages of this type in this cycle
  2	Message number
  3	Total number of SVs visible
  4	SV PRN number
  5	Elevation, in degrees, 90° maximum
  6	Azimuth, degrees from True North, 000° through 359°
  7	SNR, 00 through 99 dB (null when not tracking)
  8–11	Information about second SV, same format as fields 4 through 7
  12–15	Information about third SV, same format as fields 4 through 7
  16–19	Information about fourth SV, same format as fields 4 through 7
  20	The checksum data, always begins with *

  * */

/** NMEAGPS_PARSE_GST
 * @brief GST
 * @note GST 	Position error statistics
 * 
 Position error statistics
  An example of the GST message string is:

  $GPGST,172814.0,0.006,0.023,0.020,273.6,0.023,0.020,0.031*6A

  The Talker ID ($--) will vary depending on the satellite system used for the position solution:

  $GP - GPS only
  $GL - GLONASS only
  $GN - Combined
  GST message fields
  Field	Meaning
  0	Message ID $GPGST
  1	UTC of position fix
  2	RMS value of the pseudorange residuals; includes carrier phase residuals during periods of RTK (float) and RTK (fixed) processing
  3	Error ellipse semi-major axis 1 sigma error, in meters
  4	Error ellipse semi-minor axis 1 sigma error, in meters
  5	Error ellipse orientation, degrees from true north
  6	Latitude 1 sigma error, in meters
  7	Longitude 1 sigma error, in meters
  8	Height 1 sigma error, in meters
  9	The checksum data, always begins with *

  * */

/** NMEAGPS_PARSE_VTG
 * @brief VTG
 * @note VTG 	Actual track made good and speed over ground
 * 
 Track made good and speed over ground
  An example of the VTG message string is:

  $GPVTG,,T,,M,0.00,N,0.00,K*4E

  VTG message fields
  Field	Meaning
  0	Message ID $GPVTG
  1	Track made good (degrees true)
  2	T: track made good is relative to true north
  3	Track made good (degrees magnetic)
  4	M: track made good is relative to magnetic north
  5	Speed, in knots
  6	N: speed is measured in knots
  7	Speed over ground in kilometers/hour (kph)
  8	K: speed over ground is measured in kph
  9	The checksum data, always begins with *
 
  * */


/** NMEAGPS_PARSE_ZDA
 * @brief ZDA
 * @note VTG 		UTC day, month, and year, and local time zone offset
 * 
 UTC day, month, and year, and local time zone offset
    An example of the ZDA message string is:

    $GPZDA,172809.456,12,07,1996,00,00*45

    ZDA message fields
    Field	Meaning
    0	Message ID $GPZDA
    1	UTC
    2	Day, ranging between 01 and 31
    3	Month, ranging between 01 and 12
    4	Year
    5	Local time zone offset from GMT, ranging from 00 through ±13 hours
    6	Local time zone offset from GMT, ranging from 00 through 59 minutes
    7	The checksum data, always begins with *
  * */





















// **/

// #endif 