

/**************************************
  This Code is Written By Chahil Patel
  and Allowed To Modifiy .
  Contect:chahil9725@gmail.com         *
***************************************/

#include <Ticker.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include<FirebaseArduino.h>
#include <PubSubClient.h>

#include <aREST.h>
#include <ESP8266Ping.h>
String FIREBASE_HOST = "custum-57f41.firebaseio.com";        //Your Firebase Project URL goes here without "http:" , "\" and "/"
String FIREBASE_AUTH = "A9EqCIzFnrTWQaGNGtxfA2KJKFte5BnjTax4fDen" ;    //Your Firebase Database Secret goes here


const char* ssid = "Shreeji";
const char* password = "9998328464Mm";

String HOSTNAME;
Ticker Buzzer;
WiFiClient espClient;
PubSubClient client(espClient);

aREST rest = aREST(client);
char* device_id = "be2019";
void callback(char* topic, byte* payload, unsigned int length);




int net = 0, j = 1;
ESP8266WebServer server(80);
void setup() {
  Serial.begin(115200);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  client.setCallback(callback);
  rest.set_id(device_id);
  rest.set_name("esp8266");
 
  WiFi.begin(ssid, password);
WiFi.softAP("Home","12345678Cc");
Webserver();
  while (WiFi.status() != WL_CONNECTED) {
     server.handleClient();
    //Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  digitalWrite(D4, LOW);
  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  char* out_topic = rest.get_topic();
Firebase.setString("push", "1");
}

void loop()
{
 
  rest.handle(client);
    server.handleClient();
    firebasereconnect();
    firebase();
 

}
void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}


void firebasereconnect()
{
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }

}
void Buzz()
{
  static int c=0;
  c++;
  digitalWrite(D3,!digitalRead(D3));
  if(c==40)
  { c=0;
    Buzzer.detach();
  }
}
void firebase()
{
  int push =  Firebase.getString("push").toInt();
  if ( push == 1) {
    Firebase.setString("push", "0");
    digitalWrite(D5, Firebase.getString("r1").toInt());
    digitalWrite(D6, Firebase.getString("r2").toInt());
    digitalWrite(D7, Firebase.getString("r3").toInt());
    digitalWrite(D8, Firebase.getString("r4").toInt());
    if(Firebase.getString("b1").toInt())
    {
    Firebase.setString("b1", "0");
     Buzzer.attach(Firebase.getString("b").toFloat(), Buzz);
    }
    int all = Firebase.getString("all").toInt();
    if (all == 1 )
    {
      digitalWrite(D5, HIGH);
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(D8, HIGH);
         Firebase.setString("r1", "1");
      Firebase.setString("r2", "1");
      Firebase.setString("r3", "1");
      Firebase.setString("r4", "1");
      Firebase.setString("all", "7");
    }
    if (all == 0)
    {
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D8, LOW);
      Firebase.setString("all", "7");
      Firebase.setString("r1", "0");
      Firebase.setString("r2", "0");
      Firebase.setString("r3", "0");
      Firebase.setString("r4", "0");
    }

    Firebase.setInt("s1", digitalRead(D5));
    Firebase.setInt("s2", digitalRead(D6));
    Firebase.setInt("s3", digitalRead(D7));
    Firebase.setInt("s4", digitalRead(D8));
  }

  if (push == 12)
  {
    Firebase.setInt("push", 0);
    Firebase.setInt("s1", digitalRead(D5));
    Firebase.setInt("s2", digitalRead(D6));
    Firebase.setInt("s3", digitalRead(D7));
    Firebase.setInt("s4", digitalRead(D8));

  }
}





void Webserver()
{
  server.on("/relay", []() {
    int relay = server.arg("relay").toInt();
    int relaystatus = server.arg("status").toInt();
    digitalWrite(relay, relaystatus);
    int a = digitalRead(D5);
    int b = digitalRead(D6);
    int c = digitalRead(D7);
    int d = digitalRead(D8);
    server.send(200, "text/plain", String("") + a + " " + b + " " + c + " " + d + " "  );
  });
  server.on("/ip", []() {
    
    server.send(200, "text/plain", "");
  });
  server.on("/allon", []() {
    digitalWrite(D5, 1);
    digitalWrite(D6, 1);
    digitalWrite(D7, 1);
    digitalWrite(D8, 1);
    server.send(200, "text/plain", "allon");
  });
  server.on("/alloff", []() {
    digitalWrite(D5, 0);
    digitalWrite(D6, 0);
    digitalWrite(D7, 0);
    digitalWrite(D8, 0);
    server.send(200, "text/plain", "alloff");
  });
  server.begin();
}
