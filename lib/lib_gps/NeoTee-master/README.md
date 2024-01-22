# NeoTee
Copy one Arduino `Print` stream to multiple `Print` streams

This library is like the UNIX "tee" command, which splits an output stream into 
two streams, but with N branches.  On the Arduino, an output stream is instance of the 
Arduino Print class.  If you need the flush method, you must use NeoTeeStream instead.

Simply declare an array of streams:

    Print *streams[2];

... and pass the array and its size to the NeoTee constructor:

    NeoTee tee( streams, sizeof(streams)/sizeof(streams[0]) );

Any `Print` operation on "tee" will be duplicated on all `Print` streams in the array.

    tee.println( F("This is sent to all Stream instances in the array.") );

Because there is no base class with "begin" and "end" methods, you
must call those explicitly on the individual Streams (e.g., Serial.begin).

Although it is questionable, `read` and `peek` are implemented in NeoTeeStream.
This kind of random "merging" of an input stream is not a UNIX feature.  :)  They 
have to be provided because they are marked as pure virtual in the Stream class

NOTE: You can mix different kinds of `Print` streams in the array:

    NeoSWSerial gps_port( 3, 4 );
    Print *justSerial[2] = { &Serial, &gps_port };
    
