#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EEPROM.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//int x=1, y=1, a;
char auth[] = "e5c266cb1e7241bbbe67e467141b4569";
const char* ssid1 = "Shreeji";
const char* pass1 = "8866551417Cc";

const char* ssid = "CHAHIL9725";
const char* pass = "8866551417Cc";

int val1,val2,val3,val4,val5;
ESP8266WebServer server(80);



void setup(void) {

   pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);


  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);

  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);


  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  Blynk.config(auth);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  WiFi.begin(ssid1,pass1);
  Serial.print("Connected to ");
  Serial.println(ssid1);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.softAP(ssid, pass);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/fw", []() {
    motor_forward();
    server.send(200, "text/plain", "Forward");
  });
  server.on("/bk", []() {
    motor_back();
    server.send(200, "text/plain", "Back");
  });

  server.on("/st", []() {
    motor_stop();
    server.send(200, "text/plain", "Stop");
  });
  server.on("/lt", []() {
    motor_left();
    server.send(200, "text/plain", "Left");
  });
  server.on("/rt", []() {
    motor_right();
    server.send(200, "text/plain", "Right");
  });

  server.begin();
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();
 Blynk.run();
  
 
  }

BLYNK_WRITE(V0)
{
  int x = param[0].asInt();

  int y = param[1].asInt();


   if (x == 1 && y == 2)
  {
    motor_forward();
  }

  if (x == 1 && y == 0)
  {
    motor_back();
  }

  if (x == 0 && y == 1)
  {
    motor_left();
  }
  if (x == 2 && y == 1)
  {
    motor_right();
  }
  if (x == 1 && y == 1)
  {
    motor_stop();
  }
}

void handleRoot() {
  server.send(200, "text/plain", "hello from Robot!");
}

void motor_forward() {
  digitalWrite(D1, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);

}
void motor_stop() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}
void motor_back() {
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D1, LOW);
  digitalWrite(D4, LOW);

}

void motor_left() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}
void motor_right() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}



