//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

// Set these to run example.
#define FIREBASE_HOST "irtest-91159.firebaseio.com"
#define FIREBASE_AUTH "CFOPrqPydw3ofA7clu7WMKUBdnC4qs7Q6RKjz3CT"
#define WIFI_SSID "Shreeji"
#define WIFI_PASSWORD "8866551417Cc"

void setup() {
  Serial.begin(9600);
 irsend.begin();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;
String abcd;
void loop() {
 
//  Firebase.setFloat("number", 42.0);
 
 // Firebase.setFloat("number", 43.0);
  
 // Serial.println(Firebase.getFloat("number"));

 // Firebase.remove("number");
irsendRAW();
irsendSONY();
irsendNEC();
}








void irsendNEC()
{
int nec = Firebase.getString("nec").toInt(); 
int necon = Firebase.getString("necon").toInt(); 
  if(necon==1){
#if SEND_NEC
   irsend.sendNEC(nec, 32);
#endif 
Firebase.setInt("necon", 0); 
  }
  
  }


  void irsendSONY()
{
int sonyon = Firebase.getString("sonyon").toInt(); 
int sony = Firebase.getString("sony").toInt(); 
  if(sonyon==1){
#if SEND_SONY
   irsend.sendSony(sony, 32);
#endif 
Firebase.setInt("sonyon", 0); 
  }
  
  }

  void irsendRAW()
{
int rawon = Firebase.getString("rawon").toInt(); 
uint16_t raw[300] = Firebase.getString("raw").toInt(); 
int numofraw = Firebase.getString("numofraw").toInt(); 
  if(rawon==1){
#if SEND_RAW
   irsend.sendRaw(raw,numofraw,38);
#endif 
Firebase.setInt("rawon", 0); 
  }
  
  }
