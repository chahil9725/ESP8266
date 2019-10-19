/* 
 * Mert Arduino and Tech - YouTube
 * WiFi Robot controlled by Phone (WiFibot/Android/IoT/ESP8266)
 * NodeMCU ESP8266 Tutorial 03
 * Please Subscribe for Support - https://goo.gl/RLT6DT
 */


/* include library */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EEPROM.h>
#include <string.h>


/*
 * If you have no idea about this library, you can watch the tutorial below
 * NodeMCU ESP8266 Tutorial 01: Programming NodeMCU ESP-12E Using Arduino IDE
 * https://youtu.be/NtyFx1frYB0
 */

/* define port */
WiFiClient client;
WiFiServer server(80);
ESP8266WebServer serverAP(8000);


/* WIFI settings */
const char* ssid = "Shreeji";
const char* password = "8866551417Cc";

const char* myssid = "WiBoT"; //SSID AP MODE
const char* mypassword = "8320128053"; //Password AP MODE


/* data received from application */
String  data =""; 

 IPAddress ip;
Servo servo;



/* define L298N or L293D enable pins */

void setup()
{
  Serial.begin(115200);
  /* initialize motor control pins as output */
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT); 
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);

  /* initialize motor enable pins as output */
  pinMode(D6, OUTPUT); 
  servo.attach(D6);
 
WiFi.mode(WIFI_AP_STA);

//AP
IPAddress myIP =WiFi.softAP(myssid, mypassword);
serverAP.begin();

WiFi.begin(ssid, password);
ip = WiFi.localIP();
Serial.print("ip"); 
  /* start server communication */
  server.begin();
}

void loop()
{
    /* If the server available, run the "checkClient" function */  
    client = server.available();
    if (!client) return; 
    data = checkClient ();
    ip = WiFi.localIP();
Serial.print("ip");

/************************ Run function according to incoming data from application *************************/

    /* If the incoming data is "forward", run the "MotorForward" function */
    if (data == "forward") MotorForward();
    else if (data == "90")  servo.write(90);
    else if (data == "180")  servo.write(180);
    else if (data == "0") servo.write(10);
    /* If the incoming data is "backward", run the "MotorBackward" function */
    else if (data == "backward") MotorBackward();
    /* If the incoming data is "left", run the "TurnLeft" function */
    else if (data == "left") TurnLeft();
    /* If the incoming data is "right", run the "TurnRight" function */
    else if (data == "right") TurnRight();
    /* If the incoming data is "stop", run the "MotorStop" function */
    else if (data == "stop") MotorStop();

} 

/********************************************* FORWARD *****************************************************/
void MotorForward(void)   
{
  
  digitalWrite(D1,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  delay(300);
  MotorStop();
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
 
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D4,LOW);
    delay(300);
    MotorStop();
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
  digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,HIGH);  
    delay(100);
    MotorStop();
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  
  digitalWrite(D1,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
    delay(100);
    MotorStop();
}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{

  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
}

/********************************** RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
