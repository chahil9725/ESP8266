/* Create a WiFi access point and provide a web server on it to receive command to control motor. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EEPROM.h>
#include <string.h>
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
/* Set these to your desired credentials. */
char auth[] = "e5c266cb1e7241bbbe67e467141b4569";

const char* hotspot = "WiBoT";
const char* password = "8866551417Cc";
const char* ssid1 = "Shreeji";
const char* pass1 = "8866551417Cc";
Servo servo;

Servo myservo;

int a,x,y;
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/plain", "hello from Robot!");
}

void motor_forward(){
  digitalWrite(D1,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  }
void motor_stop(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  }
void motor_back(){
   
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D4,LOW);
  }

void motor_right(){
      digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,HIGH);  
  }
void motor_left(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
  }

void servo90(){
servo.write(90);
myservo.write(90);

}
void servo0(){
servo.write(0);
myservo.write(180);

}
void servo180(){
servo.write(180);
myservo.write(0);

}





void setup() {
  // prepare Motor Output Pins
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  
  // prepare GPIO5 relay 1
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);
  digitalWrite(D7,HIGH);
    digitalWrite(D8,HIGH);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  pinMode(D6, OUTPUT); 
  pinMode(D5, OUTPUT);
 servo.attach(D6);
// digitalWrite(D6,1);
pinMode(D5,OUTPUT);
  myservo.attach(D5);
	delay(1000);
	Serial.begin(115200);
	Serial.println();
 WiFi.mode(WIFI_AP_STA);
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(hotspot, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
  Blynk.begin(auth, ssid1, pass1);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
	server.on("/", handleRoot);
 
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

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
   server.on("/90", []() {
   servo90();
    server.send(200, "text/plain", "90");
  });
   server.on("/0", []() {
    servo0();
    server.send(200, "text/plain", "0");
  });
   server.on("/180", []() {
    servo180();
    server.send(200, "text/plain", "180");
  });

	server.begin();
	Serial.println("HTTP server started");


  
}

void loop() {
  Blynk.run();
  
	server.handleClient();

if(a==1){
  Blynk.notify("XBoT Online");
a=a+1;
    
  }
  joystick();
}


 BLYNK_WRITE(V1)
{
  int x1 = param[0].asInt();
  x1=x;
  int y1 = param[1].asInt();
  y1=y;
  }

void joystick()

{
 
   if(x==1 && y==2)
  {
    motor_forward();
    }

     if(x==1 && y==0)
  {
    motor_back();
    }

    if(x==0 && y==1)
  {
    motor_left();
    }
     if(x==2 && y==1)
  {
    motor_right();
    }
     if(x==1 && y==1)
  {
    motor_stop();
    }
  
  }
