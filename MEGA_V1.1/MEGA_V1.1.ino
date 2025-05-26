//////////////////////////////////////////////////////////////////////////////////////////////////////
// Robot tagliaerba Arduino                                                                         //
// Versione modificata da Bedendo Marco                                                             //
// Versione con Driver motori BTS7960 e motori E192-2S.12.180                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Libraries for Perimeter Wire Receiver
#include <Arduino.h>
#include <Wire.h>

#include "config.h"

/************************************************************************************************************/

void setup()  {
  mower_setup();
}

void loop()  {
  mower_loop();
}
