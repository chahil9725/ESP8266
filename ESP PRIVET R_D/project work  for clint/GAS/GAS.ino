/**************************************
  This Code is Written By Chahil Patel
  and Allowed To Modifiy .
  Contect:chahil9725@gmail.com         *
***************************************/


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

const char* remote_host = "www.google.com";

String st;         // Web Server Variable
String content;   // Web Server Variable
int statusCode;   // Web Server Variable
String HOSTNAME;

ESP8266WebServer server(80); //Define Webserver

int WIFI();      //Read from EEPROM  End Return True/False and start wifi
int testwifi();    // Test WiFi End Return True/False
void startAP();     //Start SoftAP And Launch Webserver() As Webserver Portal
void Webserver();      //Take Value From Web Server And Store In EEPROM

void led(int, int);

String avg_time_ms;
int i, c = 1;
int net = 0, flag = 0, j = 1;

void setup() {
  Serial.begin(115200);

  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
   digitalWrite(D4, HIGH);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop()
{
  delay(500);
  int analog = analogRead(A0);
  if (analog > 400 && flag)
  {
    SendAlert(1);
    flag = !flag;

    delay(300);
  }
  delay(3);
  if (analog < 400 && !flag)
  {
    SendAlert(0);
    flag = 1;

    delay(300);
  }


  Firebase.setString("Analog", (String)analog);
 
}
void SendAlert(int x)
{
  if (!x)
    Firebase.setString("gas", "OFF");
  Firebase.setString("b1", "0");
  Firebase.setString("push", "1");
  if (x)
    Firebase.setString("gas", "ON");
  Firebase.setString("b1", "1");
  Firebase.setString("push", "1");
}
void internet()
{
  if (WiFi.status() == WL_CONNECTED) {
    if (j == 1)
    {
      j = 2;
      Serial.println(" ");
      Serial.print("WiFi IP address: ");
      Serial.print(WiFi.localIP());
      Serial.println(" ");
      Serial.print("Pinging host ");  // Just for troubleshot purpose.
      Serial.println(remote_host);
      if (Ping.ping(remote_host)) {
        Serial.println("Good");
        net = 1;
        digitalWrite(D5, 1);
        digitalWrite(D4, 0);
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

        Serial.println("Success!!");
      } else {
        net = 0;
        Serial.println("Error :(");

      }
    }
  }
}

void firebasereconnect()
{
  if (Firebase.failed())
  {
    static int c = 0;
    c++;

    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    delay(200);
    if (c > 40)
      ESP.reset();
  }

}
