

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager
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
int a=1,x,y;
/* Set these to your desired credentials. */
char auth[] = "e5c266cb1e7241bbbe67e467141b4569";
const int TRIGGER_PIN = D0; // D3 on NodeMCU and WeMos.
ESP8266WebServer server(80);


// Indicates whether ESP has WiFi credentials saved from previous session
bool initialConfig = false;

void setup() {
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);

  // prepare GPIO5 relay 1
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);

  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  pinMode(D6, OUTPUT);
  pinMode(D5, OUTPUT);
  
  // digitalWrite(D6,1);
  pinMode(D5, OUTPUT);

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

 Blynk.config(auth);
  Serial.begin(115200);
  Serial.println("\n Starting");
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  if (WiFi.SSID() == "") {
    Serial.println("We haven't got any access point credentials, so get them now");
    initialConfig = true;
  }
  else {

    WiFi.mode(WIFI_AP_STA);
    unsigned long startedAt = millis();
    Serial.print("After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis() - startedAt);
    Serial.print(waited / 1000);
    Serial.print(" secs in setup() connection result is ");
    Serial.println(connRes);
  }
  pinMode(TRIGGER_PIN, INPUT);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("failed to connect, finishing setup anyway");
  } else {
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
  }
}


void loop() {

  if ((digitalRead(TRIGGER_PIN) == HIGH)  || (initialConfig)) {
    Serial.println("Configuration portal requested.");

    WiFiManager wifiManager;


    if (!wifiManager.startConfigPortal("CHAHIL9725", "12345678Cc")) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }

   // ESP.reset();
   // delay(5000);
  }


  Blynk.run();

  server.handleClient();

  if (a == 1) {
    Blynk.notify("XBoT Online");
    a = a + 1;

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

void motor_right() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}
void motor_left() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}
