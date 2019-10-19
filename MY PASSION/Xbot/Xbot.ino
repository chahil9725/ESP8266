#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char* ssid = "GITWIFI";
const char* password = "12345678Cc";

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  String Status=server.arg("status");
  if(Status=="fw")
  forward();
  if(Status=="bk")
  back();
  if(Status=="rt")
  right();
  if(Status=="lt")
  left();
  if(Status=="st")
  stop();
  //   server.send(200, "text/plain", "200");
}
void forward(){
  digitalWrite(D1,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  Serial.println("FORWARD");
  server.send(200, "text/plain", "FORWARD");
  }
void stop(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
    Serial.println("STOP");
  server.send(200, "text/plain", "STOP");
  }
void back(){
   
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D4,LOW);
    Serial.println("BACK");
  server.send(200, "text/plain", "BACK");
  }

void left(){
      digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,HIGH);
  Serial.println("RIGHT");
  server.send(200, "text/plain", "RIGHT");
  }
void right(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
    Serial.println("LEFT");
  server.send(200, "text/plain", "LEFT");
  }

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
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
  Serial.begin(115200);
  //WiFi.mode(WIFI_AP);
Serial.print("Connecting to ");
  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//WiFi.eraseConfig();
 WiFi.disconnect();

  WiFi.softAP(ssid, password);
 
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.println("WiFi connected");
Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/fw", forward);
  server.on("/bk", back);
  server.on("/rt", right);
 server.on("/lt",left);
  server.on("/st",stop);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void) {
  server.handleClient();

  int static first = millis();

  if(millis() - first >=2000)
  {
    //stop();
first = millis();
//Serial.println("Stop");
  }
  }
  
  
