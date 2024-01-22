#include <NeoTee.h>

Print *justSerial[2] = { &Serial, &Serial };

NeoTee twice( justSerial, sizeof(justSerial)/sizeof(justSerial[0]) );

void setup()
{
  Serial.begin( 9600 );
  Serial.println( F("Enter any characters and they will be printed twice \n"
                    "  with one call to the NeoTee instance, like this:") );
  twice.write( "0123345789", 10 );
  Serial.println();
  twice.print( 123.45 );
}

void loop()
{
  if (Serial.available())
    twice.print( (char) Serial.read() );
}