
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Shreeji1";
const char* password = "9998328464Mm";
//String ip = "192.168.43.213";
String ip = "192.168.4.2";
//String ip = "192.168.43.213";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";
void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);
  if (analogRead(A4) < 1000)
  {
    ip = "192.168.43.195";
       }
         Serial.begin(9600);
         WiFi.softAP("Shreeji1","9998328464Mm");
//         WiFi.begin(ssid, password);
//         while (WiFi.status() != WL_CONNECTED)
//       {
//         delay(500);
//         Serial.print('.');
//       }
//         digitalWrite(5,0);
//         Serial.println(" connected'");

         Udp.begin(localUdpPort);
         Serial.printf("'Now listening at IP % s, UDP port % d\n", WiFi.localIP().toString().c_str(), localUdpPort);
       }

         void loop() {

         if(analogRead(A4) < 1000)
       {
         Serial.println("DOWN");
         SendData('b');
       }
         if(analogRead(A4) > 3000)
       {
         Serial.println("UP");
         SendData('f');
       }
         //  while(analogRead(A5) < 1000)
         //  {
         //  Serial.println("LEFT");
         //  SendData('l');
         //  }
         //   while(analogRead(A5) > 3000)
         //   {
         //  Serial.println("RIGHT");
         //  SendData('r');
         //   }
         while(analogRead(A6) < 1000)
       {
         Serial.println("LEFT");
         SendData('l');
       }
         while(analogRead(A6) > 3000)
       {
         Serial.println("RIGHT");
         SendData('r');
       }
         //  if(analogRead(A7) < 1000)
         //  {
         //  Serial.println("DOWN");
         //  SendData('b');
         //  }
         //   if(analogRead(A7) > 3000)
         //   {
         //  Serial.println("UP");
         //  SendData('f');
         //   }
         //Serial.println(analogRead(A6));

         if( analogRead(A6) > 1700 && analogRead(A6) <2300 && analogRead(A5) > 1700 && analogRead(A5) <2300 && analogRead(A4) > 1700 && analogRead(A4) <2300 &&analogRead(A7) > 1700 && analogRead(A7) <2300 )
       {
         SendData('s');
         Serial.println("STOP");
       }

       }


         void SendData(char p)
       {
         delay(30);
         Udp.beginPacket(ip.c_str(), 4210);
         Udp.write(p);
         Udp.endPacket();
         Udp.endPacket();


       }
