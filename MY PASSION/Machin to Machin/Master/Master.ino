

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress ip(10, 1, 1, 1);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
void setup()
{
  pinMode(D1, OUTPUT);
  
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, 0);
  digitalWrite(D2, 0);
  digitalWrite(D3, 0);
  digitalWrite(D4, 0);
  Serial.begin(9600);
   Serial.println("START");
     WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP("Master", "12345678");

  server.on("/led", led);

  server.begin();
}


void loop()
{
  server.handleClient();

}


void led()
{
int value=server.arg("value").toInt();

String msg=server.arg("msg");
Serial.println(msg);
digitalWrite(D4,!value);
server.send(200,"text/plain","OK");
}
