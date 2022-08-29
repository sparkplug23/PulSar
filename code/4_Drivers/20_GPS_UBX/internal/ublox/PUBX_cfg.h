#ifndef PUBX_CFG_H
#define PUBX_CFG_H

//------------------------------------------------------------
// Enable/disable the parsing of specific proprietary NMEA sentences.
//
// Configuring out a sentence prevents its fields from being parsed.
// However, the sentence type may still be recognized by /decode/ and 
// stored in member /nmeaMessage/.  No valid flags would be available.



#ifdef USE_MODULE_DRIVERS_GPS

#define NMEAGPS_PARSE_PUBX_00
//#define NMEAGPS_PARSE_PUBX_04

#endif



#endif //ifdef USE_MODULE_DRIVERS_GPS