#ifndef NMEAGPS_H
#define NMEAGPS_H

//  Copyright (C) 2014-2017, SlashDevin
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

#include <Arduino.h>
#ifdef __AVR__
  #include <avr/interrupt.h>
#endif

#include "GPSfix.h"
#include "configs/NMEAGPS_cfg.h"

#define PGM_P const char *
#define __PROGMEM PROGMEM

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
    NMEAGPS & operator =( const NMEAGPS & );
    NMEAGPS( const NMEAGPS & );

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
    //
    // IMPORTANT DESIGN NOTE
    // ---------------------
    // This function is the ORIGINAL high-level "feed point" used by NeoGPS for
    // normal FIX-oriented operation.
    //
    // In the typical library usage, the sketch does not manually read bytes from
    // the UART. Instead, it calls:
    //
    //    while (gps.available(Serial1)) {
    //      gps_fix fix = gps.read();
    //      ...
    //    }
    //
    // and this function internally:
    //   1) reads bytes from the serial port,
    //   2) passes each byte into handle(...),
    //   3) handle(...) calls decode(...),
    //   4) decode(...) runs the protocol state machine,
    //   5) when a complete valid message is decoded, handle(...) calls storeFix(),
    //   6) available() stops once a FULL FIX is available.
    //
    // ----------------------------------------------------------------------
    // VERY IMPORTANT: "available()" is FIX-oriented, NOT PACKET-oriented.
    // ----------------------------------------------------------------------
    // This function does NOT return when every valid NMEA sentence or UBX packet
    // completes. Instead, it returns when _available() becomes true, meaning a
    // completed FIX is ready according to the library's coherency / interval logic.
    //
    // For NMEA:
    //   - many sentences may be received (GGA, RMC, GSA, GSV, etc.)
    //   - the parser merges or stores fields as configured
    //   - _available() becomes true only when the configured interval is complete
    //
    // For UBX:
    //   - many UBX packets may be received (NAV-PVT, NAV-DOP, NAV-STATUS, etc.)
    //   - decode(...) completes EACH valid packet individually
    //   - BUT _available() only becomes true when the configured
    //     "last message in interval" arrives
    //
    // So:
    //
    //   DECODE_COMPLETED  = one valid packet / sentence just finished
    //   _available()      = one coherent fix is now available to read()
    //
    // Those are NOT the same event.
    //
    // ----------------------------------------------------------------------
    // Call chain summary
    // ----------------------------------------------------------------------
    // available(Stream&)
    //   -> handle(byte)
    //       -> decode(byte)
    //           -> protocol-specific state machine
    //
    // For plain NMEAGPS:
    //   handle(...) -> NMEAGPS::decode(...)
    //
    // For ubloxGPS (derived override):
    //   handle(...) -> ubloxGPS::decode(...)
    //
    // The UBX override first tries to parse the byte as binary UBX.
    // If that byte/sequence is not valid UBX framing, it FALLS BACK to
    // NMEAGPS::decode(c). Therefore, the UBX parser path can still consume
    // mixed streams containing both UBX and NMEA.
    //
    // In other words:
    //
    //   ubloxGPS::decode(byte)
    //      tries UBX first
    //      if not valid UBX -> delegates to NMEAGPS::decode(byte)
    //
    // This is why a mixed NMEA + UBX stream can still be handled through the
    // ubloxGPS parser class.
    //
    // ----------------------------------------------------------------------
    // Why this matters for future development
    // ----------------------------------------------------------------------
    // We established that:
    //
    // 1) This function is useful for normal "give me the next full fix" behavior.
    // 2) It is NOT the right hook for per-packet activity timing or message age.
    // 3) Packet-level metrics should instead key from handle(...) returning
    //    DECODE_COMPLETED, because that happens for EACH successfully decoded
    //    sentence/packet.
    // 4) For packet timing / message activity UI, higher-level module code should
    //    probably drain bytes manually and inspect handle(...) results directly,
    //    instead of relying on available(Stream&).
    //
    // Therefore:
    //
    //   available(Stream&) = FIX-level API
    //   handle(byte)       = PACKET-level event source
    //
    // ----------------------------------------------------------------------
    // Memory / orchestration notes
    // ----------------------------------------------------------------------
    // Two possible architectures for tracking packet timing:
    //
    // A) Low-level parser stores a queue/vector of completed packet metadata
    // B) Module-level code reads bytes, calls handle(...), and reacts immediately
    //
    // The preferred direction was B:
    //   - keep parser logic simpler
    //   - keep counters / timestamps / UI state outside the core parser
    //   - optionally expose only minimal "last packet metadata" from the parser
    //
    // This available(Stream&) function remains useful for the original fix-based
    // usage pattern, but packet-observer/debug mode is better handled outside it.
    //
    // ----------------------------------------------------------------------
    // Processing style note
    // ----------------------------------------------------------------------
    // In PS_POLLING mode, this function consumes bytes in a loop until:
    //   - the port runs out of bytes, or
    //   - _available() becomes true, meaning at least one full fix is ready
    //
    // It does NOT break on every decoded packet.
    // It breaks only when the FIX-level availability condition is reached.
    //
    // If you need packet timing / "which UBX messages are
    // active right now?" / age-per-message, do NOT start here first.
    // Start at:
    //   - handle(...)
    //   - decode(...)
    //   - DECODE_COMPLETED
    // and use module-level logic for counters/timestamps.
    //

    //.......................................................................
    // The available(...) functions return the number of *fixes* that
    //   are available to be "read" from the fix buffer.  The GPS port
    //   object is passed in so a char can be read if port.available().

    uint8_t available( Stream & port )
    {
      // Debug note:
      // processing_style controls whether this object actively polls the port
      // here, or whether bytes are expected to be fed from somewhere else.
      //
      // In PS_POLLING mode, available(Stream&) acts as the main byte-draining
      // loop for the parser.

      // Serial.printf("av ######################################## %d\n\r", processing_style);

      if (processing_style == PS_POLLING)
      {
        while (port.available())
        {
          // Feed ONE byte into the parser stack.
          //
          // Internally:
          //   handle(port.read())
          //       -> decode(byte)
          //           -> NMEA decode
          //           OR UBX override decode
          //           OR UBX decode fallback to NMEA decode
          //
          // If decode(...) returns DECODE_COMPLETED, handle(...) may call storeFix().
          // However, this outer loop does NOT break on DECODE_COMPLETED directly.
          // It breaks only when _available() says a FULL FIX is ready.
          handle( port.read() );

          // _available() is a FIX-buffer/coherency condition, not a packet event.
          //
          // This becomes true when the library determines a complete fix can be
          // read(), typically after the configured interval boundary message has
          // been received.
          if (_available())
            break; // return when one full fix happens
        }
      }

      // Return how many completed fixes are available, not how many packets
      // or sentences were decoded.
      uint8_t av = _available();

      // if(av) Serial.printf("av >>>>>>>>>>>>>>?????????????????????? %d\n\r", av);

      return av;
    }
    uint8_t available() const volatile { return _available(); };

    //.......................................................................
    // Return the next available fix.  When no more fixes
    //   are available, it returns an empty fix.

    const gps_fix read();

    //.......................................................................
    //  The OVERRUN flag is set whenever a fix is not read by the time
    //  the next update interval starts.  You must clear it when you
    //  detect the condition.

    bool overrun() const { return _overrun; }
    void overrun( bool val ) { _overrun = val; }

    //.......................................................................
    // New metadata struct to capture info about the most recently completed packet/sentence.
    struct completed_packet_meta_t
    {
      uint8_t  is_ubx = 0;       // 0 = NMEA, 1 = UBX
      uint8_t  msg_class = 0;    // UBX class when is_ubx=1, else 0
      uint8_t  msg_id = 0;       // UBX msg_id when is_ubx=1, else nmeaMessage enum
      uint16_t payload_length = 0; // UBX payload length, 0 for NMEA
    }completed_packet_meta;
    
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

    //.......................................................................
    //  Current fix accessor.
    //    *** MOST APPLICATIONS SHOULD USE read() TO GET THE CURRENT FIX  ***
    //    /fix/ will be constantly changing as characters are received.
    //
    //  For example, fix().longitude() may return nonsense data if
    //  characters for that field are currently being processed in /decode/.

    gps_fix & fix() { return m_fix; };

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

    static const msg_table_t  nmea_msg_table PROGMEM;

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
    gps_fix m_fix;

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

    uint8_t _available() const volatile { return _fixesAvailable; };

    //.......................................................................
    //  Buffered fixes.

    #if (NMEAGPS_FIX_MAX > 0)
      gps_fix buffer[ NMEAGPS_FIX_MAX ]; // could be empty, see NMEAGPS_cfg.h
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

    bool parseFloat( gps_fix::whole_frac & val, char chr, uint8_t max_decimal );

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
