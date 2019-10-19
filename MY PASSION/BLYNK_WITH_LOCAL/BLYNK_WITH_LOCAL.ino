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
//#include <Servo.h>
#include <EEPROM.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>



#include <ESP8266WiFi.h>


#include <NewPing.h>

#define TRIGGER_PIN  D6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     D7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int dis=112;
int dispara=0;
//int x=1, y=1, a;
int m=200,t=100;

const char* ssid1 = "Shreeji1";
const char* pass1 = "886651417Cc";

const char* ssid2 = "CHAHIL9725";
const char* pass2 = "8866551417Cc";


ESP8266WebServer server(80);

int x,y,a,b,c,d,e;
void handleRoot() {
  server.send(200, "text/plain", "hello from Robot!");
}
void setup(void) {

   pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);


  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);

  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
 pinMode(D5, INPUT);
  pinMode(D8, INPUT);

  Serial.begin(115200);
 //= WiFi.mode(WIFI_AP_STA);
   Serial.println("");
  WiFi.begin(ssid1,pass1);
 
 
  Serial.print("Connected to ");
  Serial.println(ssid1);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 //WiFi.softAP(ssid2, pass2);
//IPAddress myIP = WiFi.softAPIP();
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
server.on("/x/fw", []() {
    motor_forwardx();
   server.send(200, "text/plain", "Forward");
  });
  server.on("/x/bk", []() {
    motor_backx();
   server.send(200, "text/plain", "Back");
  });

  server.on("/x/st", []() {
  //  motor_stop();
   server.send(200, "text/plain", "Stop");
  });
  server.on("/x/lt", []() {
    motor_leftx();
  server.send(200, "text/plain", "Left");
  });
  server.on("/x/rt", []() {
    motor_rightx();
    server.send(200, "text/plain", "Right");
  });
 server.on("/x/d", []() {
   dispara=1;
    dist();
    server.send(200, "text/plain", String("") + dis + "");
  });
   server.on("/d", []() {
    dist();
   dispara=1;
    server.send(200, "text/plain", String("") + dis + "");

  });
   server.on("/d0", []() {
    dist();
  dispara=0;
    server.send(200, "text/plain", String("") + dis + "");

  });
   server.on("/x/d0", []() {
    dist();
   dispara=0;
    server.send(200, "text/plain", String("") + dis + "");

  });
   server.on("/r", []() {
 int r = digitalRead(D5);
 Serial.print(r);
    server.send(200, "text/plain",String("") + r + "");
  });
   server.on("/l", []() {
int l = digitalRead(D7);
Serial.print(l);
    server.send(200, "text/plain",String("") + l + "");
  });
  
  server.begin();
  Serial.println("HTTP server started");
 
}


void loop()
{
  server.handleClient();
 
}




void motor_right() {
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

void motor_left() {
   digitalWrite(D2, HIGH);
   digitalWrite(D3, HIGH);
   digitalWrite(D1, LOW);
   digitalWrite(D4, LOW);

}

void motor_forward() {
   digitalWrite(D1, HIGH);
   digitalWrite(D2, LOW);
   digitalWrite(D3, HIGH);
   digitalWrite(D4, LOW);
}
void motor_back() {
   digitalWrite(D1, LOW);
 digitalWrite(D2, HIGH);
 digitalWrite(D3, LOW);
 digitalWrite(D4, HIGH);
}

void motor_leftx() {
   digitalWrite(D2, HIGH);
   digitalWrite(D3, HIGH);
   digitalWrite(D1, LOW);
   digitalWrite(D4, LOW);
delay(t);
motor_stop();
}

void motor_forwardx() {
   digitalWrite(D1, HIGH);
   digitalWrite(D2, LOW);
   digitalWrite(D3, HIGH);
   digitalWrite(D4, LOW);
   delay(m);
   motor_stop();
}
void motor_backx() {
   digitalWrite(D1, LOW);
 digitalWrite(D2, HIGH);
 digitalWrite(D3, LOW);
 digitalWrite(D4, HIGH);
 delay(m);
 motor_stop();
}

void motor_rightx() {
 digitalWrite(D1, HIGH);
 digitalWrite(D4, HIGH);
 digitalWrite(D2, LOW);
 digitalWrite(D3, LOW);
delay(t);
motor_stop();
}

void dist(){
  
  dis=sonar.ping_cm();
  }
