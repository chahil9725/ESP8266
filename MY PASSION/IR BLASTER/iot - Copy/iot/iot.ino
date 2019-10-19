#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

/*
  This a simple example of the aREST Library for the ESP8266 WiFi chip.
  This example illustrate the cloud part of aREST that makes the board accessible from anywhere
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>
#include <string.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED D2
// Clients
WiFiClient espClient;
PubSubClient client(espClient);





IRsend irsend(D2);
// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "chahil";

// WiFi parameters
const char* ssid = "Shreeji";
const char* password = "8866551417Cc";

// Variables to be exposed to the API

String local_ip = "";
int x;
// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{

  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API


  rest.variable("local_ip", &local_ip);
  rest.function("onbox", &onbox);
  rest.function("volu", &volu);
  rest.function("vold", &vold);
  rest.function("star", &star);
  rest.function("colors", &colors);
  rest.function("zee", &zee);
  rest.function("sub", &sub);
  rest.function("colorsguj", &colorsguj);
  rest.function("sub", &sub);
     rest.function("allinone", &allinone);
  rest.function("lang", &lang);



  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("chahil");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Local server started on IP:");

  // Print the IP address
  Serial.println(WiFi.localIP());
  local_ip = ipToString(WiFi.localIP());

}

void loop() {

  // Connect to the cloud
  rest.handle(client);

  // Handle Local aREST calls
  WiFiClient clientLocal = server.available();
  if (!clientLocal) {
    return;
  }
  while (!clientLocal.available()) {
    delay(1);
  }
  rest.handle(clientLocal);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}

// Convert IP address to String
String ipToString(IPAddress address)
{
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}



int onbox(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x200FF, 32);
#endif
  Serial.println("Setup Box Toggle");

}

int volu(String command)
{


  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x2F807, 32);
  delay(100);
  irsend.sendNEC(0x2F807, 32);
  delay(100);
  irsend.sendNEC(0x2F807, 32);
  delay(100);
#endif
  Serial.println("VOL+");

}

int vold(String command)
{


  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x202FD, 32);
  delay(100);
  irsend.sendNEC(0x202FD, 32);
  delay(100);
  irsend.sendNEC(0x202FD, 32);
  delay(100);
#endif
  Serial.println("VOL-");

}



int star(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
#endif
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
#endif
#if SEND_NEC
  irsend.sendNEC(0x2C03F, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");

}

int colors(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x2A05F, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");

}

int zee(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x2609F, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");

}


int sub(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x2906F, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");

}
int colorsguj(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x230CF, 32);
  delay(300);
  irsend.sendNEC(0x2C03F, 32);
  delay(300);
  irsend.sendNEC(0x2C03F, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");
}
int lang(String command)
{
  irsend.begin();
#if SEND_NEC
  irsend.sendNEC(0x2926D, 32);
  delay(300);
  irsend.sendNEC(0x228D7, 32);
  delay(300);
  irsend.sendNEC(0x2C837, 32);
  delay(300);
#endif
  Serial.println("Setup Box Toggle");
}




int allinone(String command){
int x = command.toInt();

Serial.print(x);
  
  irsend.begin();
  #if SEND_NEC
  switch(x)
  {
       case 1: irsend.sendNEC(0x228D7, 32);
      break;
       case 2: irsend.sendNEC(0x228D7, 32);
      break;
       case 3: irsend.sendNEC(0x228D7, 32);
      break;
       case 4: irsend.sendNEC(0x228D7, 32);
      break;
       case 5: irsend.sendNEC(0x228D7, 32);
      break;
       case 6: irsend.sendNEC(0x228D7, 32);
      break;
       case 7: irsend.sendNEC(0x228D7, 32);
      break;
       case 8: irsend.sendNEC(0x228D7, 32);
      break;
       case 9: irsend.sendNEC(0x228D7, 32);
      break;
       case 10: irsend.sendNEC(0x228D7, 32);
      break;
       case 11: irsend.sendNEC(0x228D7, 32);
      break;
       case 12: irsend.sendNEC(0x228D7, 32);
      break;
       case 13: irsend.sendNEC(0x228D7, 32);
      break;
       case 14: irsend.sendNEC(0x228D7, 32);
      break;
       case 15: irsend.sendNEC(0x228D7, 32);
      break;
       case 16: irsend.sendNEC(0x228D7, 32);
      break;
       case 17: irsend.sendNEC(0x228D7, 32);
      break;
       case 18: irsend.sendNEC(0x228D7, 32);
      break;
       case 19: irsend.sendNEC(0x228D7, 32);
      break;
       case 20: irsend.sendNEC(0x228D7, 32);
      break;

  }
  #endif
  return 1;
  }




