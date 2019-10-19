#include <ESP8266HTTPClient.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <esp8266httpClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <esp8266httpclient.h>
#include <EEPROM.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//int x=1, y=1, a;
char auth[] = "ca28cebfa53f44d9bc24da1474eabf1b";
char auth2[] = "e5c266cb1e7241bbbe67e467141b4569";
const char* ssid1 = "Shreeji";
const char* pass1 = "8866551417Cc";

const char* ssid = "CHAHIL9725-Controller";
const char* pass = "8866551417Cc";
WidgetBridge bridge1(V0);
BlynkTimer timer;
BLYNK_CONNECTED() {
  bridge1.setAuthToken(auth2); // Token of the hardware B
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(D6,INPUT);
pinMode(D1,INPUT);
pinMode(D2,INPUT);
pinMode(D3,INPUT);
pinMode(D4,INPUT);
pinMode(D5,INPUT);
pinMode(D8,INPUT);

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
 bridge1.digitalWrite(D0,1);
 delay(200);
    bridge1.digitalWrite(D0,0);
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}



int A=1;

void loop() {
HTTPClient http;  
  Blynk.run();
  int a,b,c,d,e,f,g;
 a=digitalRead(D6);
 b=digitalRead(D1);
 c=digitalRead(D2);
 d=digitalRead(D3);
 e=digitalRead(D4);
 f=digitalRead(D5);
 g=digitalRead(D8);

if (A==1)
{
  A++;
  Serial.print("heybhbahsb");
  }









 if(c==1 && e==0){
motor_right();


   Serial.println("Button C is Pressed");
  }
else if(b==1 && e==0){

 motor_back();
  
   Serial.println("Button B is Pressed");
  }
else if (a==1 && e==0){
  motor_left();
;

       Serial.println("Button A is Pressed");
  }
else if (d==1 && e==0){
 motor_forward();

    Serial.println("Button D is Pressed");
  }
  else{
 motor_stop();
  }


 if(c==1 && e==1){

http.begin("http://cloud.arest.io/chahil/digital/3/1");
 http.GET();

 

 //  Serial.println(http.GET(););
  }
else if(b==1 && e==1){

http.begin("https://cloud.arest.io/chahil/digital/2/1");
 int httpCode = http.GET();
 
  
   Serial.println(httpCode);
  }
else if (a==1 && e==1){
  
http.begin("http://cloud.arest.io/chahil/digital/1/1");
 int httpCode = http.GET();
 String payload = http.getString();
http.end(); 
       Serial.println(payload);
  }
else if (d==1 && e==1){

http.begin("http://cloud.arest.io/chahil/digital/4/1");
 int httpCode = http.GET();
 String payload = http.getString();
 http.end(); 
    Serial.println(payload);
  }
  else{
// motor_stop();
  }


}
 /*void myTimerEvent()
{

int A=digitalRead(D6);
int B=digitalRead(D1);
int C=digitalRead(D2);
int D=digitalRead(D3);
int E=digitalRead(D4);
int F=digitalRead(D5);
int G=digitalRead(D8);
  
 bridge1.virtualWrite(V1,A);
  bridge1.virtualWrite(V2,B);
   bridge1.virtualWrite(V3,C);
    bridge1.virtualWrite(V4,D);
     bridge1.virtualWrite(V5,E);
}


*/

void motor_forward() {
  bridge1.digitalWrite(D1, HIGH);
  bridge1.digitalWrite(D4, HIGH);
  bridge1.digitalWrite(D2, LOW);
  bridge1.digitalWrite(D3, LOW);

}
void motor_stop() {
  bridge1.digitalWrite(D1, LOW);
    bridge1.digitalWrite(D2, LOW);
    bridge1.digitalWrite(D3, LOW);
    bridge1.digitalWrite(D4, LOW);
}
void motor_back() {
    bridge1.digitalWrite(D2, HIGH);
    bridge1.digitalWrite(D3, HIGH);
    bridge1.digitalWrite(D1, LOW);
    bridge1.digitalWrite(D4, LOW);

}

void motor_left() {
    bridge1.digitalWrite(D1, HIGH);
    bridge1.digitalWrite(D2, LOW);
    bridge1.digitalWrite(D3, HIGH);
    bridge1.digitalWrite(D4, LOW);
}
void motor_right() {
    bridge1.digitalWrite(D1, LOW);
  bridge1.digitalWrite(D2, HIGH);
  bridge1.digitalWrite(D3, LOW);
  bridge1.digitalWrite(D4, HIGH);
}
