/**************************************
  This Code is Written By Chahil Patel
  and Allowed To Modifiy .
  Contect:chahil9725@gmail.com         *
***************************************/


#include <FS.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>
#include<FirebaseArduino.h>
#include <ESP8266Ping.h>
String FIREBASE_HOST = "custum-57f41.firebaseio.com";        //Your Firebase Project URL goes here without "http:" , "\" and "/"
String FIREBASE_AUTH = "A9EqCIzFnrTWQaGNGtxfA2KJKFte5BnjTax4fDen" ;    //Your Firebase Database Secret goes here



const char* ssid = "Shreeji";
const char* password = "9998328464Mm";


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

  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  digitalWrite(D5, HIGH);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    flag=digitalRead(D1);
}


void loop()
{
  if (digitalRead(D1) == 0 && flag)
  {
    SendAlert(1);
    flag = !flag;
    Serial.println(" On");
  }
  if (digitalRead(D1) == 1 && !flag)
  {
    SendAlert(0);
    flag = 1;
    Serial.println(" OFF");

  }
  firebasereconnect();
}
void SendAlert(int x)
{
  if (!x) {
    Firebase.setString("door", "OFF");
   
    int i = Firebase.getString("lock").toInt();
    if ( i == 1)
    {
      Firebase.setString("b1", "1");
      Firebase.setString("b", "0.1");
     
      Serial.println("lock Off");
    }
  }
  if (x)
  {
    Firebase.setString("door", "ON");
    Firebase.setString("r1", "1");
    Firebase.setString("r2", "1");
    int i = Firebase.getString("lock").toInt();
    if ( i == 1)
    { 
      Serial.println("lock ON");
      Firebase.setString("b1", "1");
      Firebase.setString("b", "0.4");
         }
  }
   Firebase.setString("push", "1");
}
void firebasereconnect()
{
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }
}
