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

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "chahil";

// WiFi parameters
const char* ssid = "Shreeji";
const char* password = "8866551417Cc";

// Variables to be exposed to the API

String local_ip = "";

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(D4,INPUT);
  digitalWrite(D1,0);
    digitalWrite(D2,0);
      digitalWrite(D3,0);
        digitalWrite(D4,0);
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API

 
  rest.variable("local_ip", &local_ip);
  rest.function("allon", &allon);
  rest.function("alloff", &alloff);

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




int allon(String command)
{
   digitalWrite(D1,1);
    digitalWrite(D2,1);
      digitalWrite(D3,1);
        digitalWrite(D4,1);

}
  
  
int  alloff(String command)
{
   digitalWrite(D1,1);
    digitalWrite(D2,1);
      digitalWrite(D3,1);
        digitalWrite(D4,1);

}
