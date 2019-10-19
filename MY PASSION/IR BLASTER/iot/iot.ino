#include <ESP8266mDNS.h>

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
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h> 

#ifndef UNIT_TEST

#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
// Clients
WiFiClient espClient;
PubSubClient client(espClient);
uint64_t code , type;
// Create aREST instance
aREST rest = aREST(client);

char c=0 ;

IRsend irsend(4); 

decode_results results;

// Unique ID to identify the device for cloud.arest.io
char* device_id = "chahil";

// WiFi parameters
//const char* ssid = "Shreeji";
//const char* password = "8866551417Cc";

// Variables to be exposed to the API

String local_ip = "";

MDNSResponder mdns;
// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{
   
 
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API

 
  rest.variable("local_ip", &local_ip);
  rest.variable("code", &code);
  //  rest.variable("type", &type);
    
  
  rest.function("ir", &ir);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("chahil");
 unsigned long startedAt = millis();
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  Serial.println("Opening configuration portal");

  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;  
  //sets timeout in seconds until configuration portal gets turned off.
  //If not specified device will remain in configuration mode until
  //switched off via webserver.
  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
  
  //it starts an access point 
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.startConfigPortal("ProjectXhome","8866551417Cc")) {//Delete these two parameters if you do not want a WiFi password on your configuration access point
     Serial.println("Not connected to WiFi but continuing anyway.");
  } else {
     //if you get here you have connected to the WiFi
     Serial.println("connected...yeey :)");
     }
 
      // For some unknown reason webserver can only be started once per boot up 
      // so webserver can not be used again in the sketch.
    
  Serial.print("After waiting ");
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("failed to connect, finishing setup anyway");
  } else{
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());

Serial.println("MDNS STARTED");
  }

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
  while(!clientLocal.available()){
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













int ir(String command)

{



irsend.sendNEC(c, 32);

 }


