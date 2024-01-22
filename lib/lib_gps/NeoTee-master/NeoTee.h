#ifndef _NEOTEE_H_
#define _NEOTEE_H_

//------------------------------------------------------
// NeoTee
// Copyright (C) 2016, SlashDevin
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//

#include <Print.h>

// This library is like the UNIX "tee" command, which splits an output stream 
// into two streams, but with N branches.  On the Arduino, an output stream is
// instance of the Arduino Print class, and the flush method is in Stream.
//
// Simply declare your own array of streams:
//
//     Print *streams[2];
//
// Pass the array and its size in to the NeoTee constructor:
//
//     NeoTee tee( streams, sizeof(streams)/sizeof(streams[0]) );
//
// Any Print stream operation on "tee" will be duplicated on all elements of the array.
//
//     tee.println( F("This is sent to all array elements.") );

class NeoTee : public Print
{
  Print  **_Streams;
  uint8_t  _NumStreams;

public:

  NeoTee( Print **streams, uint8_t numStreams )
    {
      _Streams    = streams;
      _NumStreams = numStreams;
    }

  virtual size_t write(uint8_t c)
    {
      size_t minWritten = 0;
      for (uint8_t i=0; i<_NumStreams; i++) {
        size_t written = _Streams[i]->write( c );
        if ((i == 0) || (minWritten > written))
          minWritten = written;
      }
      return minWritten;
    }

  using Print::write; // make the other overloads visible

  Print **Streams() const { return _Streams; };
  
  uint8_t NumStreams() const { return _NumStreams; };
  void NumStreams( uint8_t numStreams ) { _NumStreams = numStreams; };
     // CAUTION: do not set numStreams longer than the original array!
};

#endif