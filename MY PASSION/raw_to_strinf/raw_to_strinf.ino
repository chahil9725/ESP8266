/*
 * IRremoteESP8266: IRrecvDumpV2 - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 *
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Copyright 2017 David Conran
 *
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 *
 * Changes:
 *   Version 0.3 November, 2017
 *     - Support for A/C decoding for some protcols.
 *   Version 0.2 April, 2017
 *     - Decode from a copy of the data so we can start capturing faster thus
 *       reduce the likelihood of miscaptures.
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>
#endif  // DECODE_AC


#define RECV_PIN D5


#define BAUD_RATE 115200

#define CAPTURE_BUFFER_SIZE 1024

#if DECODE_AC
#define TIMEOUT 50U  // Some A/C units have gaps in their protocols of ~40ms.
                     // e.g. Kelvinator
                     // A value this large may swallow repeats of some protocols
#else  // DECODE_AC
#define TIMEOUT 15U  // Suits most messages, while not swallowing many repeats.
#endif  // DECODE_AC

#define MIN_UNKNOWN_SIZE 12



IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);

int  j = 0, k;

decode_results results;  


void setup() {
  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial) 
    delay(50);
  Serial.println();
  Serial.print("IRrecvDumpV2 is now running and waiting for IR input on Pin ");
  Serial.println(RECV_PIN);

#if DECODE_HASH

  irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  
}


void loop() { 
  
String code, code1="";

  if (irrecv.decode(&results)) {
  
    int i=1;
   
    if (results.overflow)
      Serial.printf("WARNING: IR code is too big for buffer (>= %d). "
                    "This result shouldn't be trusted until this is resolved. "
                    "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                    CAPTURE_BUFFER_SIZE);
   

    Serial.println("....................");  
           while (results.rawlen != i)
           {
     code +=results.rawbuf[i] * 2 + code1 +" "  ;
     i++;
           }
           Serial.println(code);
    yield();  
    delay(100);
  }
}
