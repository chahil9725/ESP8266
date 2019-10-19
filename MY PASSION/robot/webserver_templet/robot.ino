/**************************************
  This Code is Written By Chahil Patel
  and Allowed To Modifiy .
  Contect:chahil9725@gmail.com         *
***************************************/


#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include<FirebaseArduino.h>
#include <PubSubClient.h>

#include <aREST.h>
#include <ESP8266Ping.h>
String FIREBASE_HOST = "custum-57f41.firebaseio.com";        //Your Firebase Project URL goes here without "http:" , "\" and "/"
String FIREBASE_AUTH = "A9EqCIzFnrTWQaGNGtxfA2KJKFte5BnjTax4fDen" ;    //Your Firebase Database Secret goes here


const char* APssid = "AC REMOTE";
const char* APpassword = "12345678";

const char* remote_host = "www.google.com";

String st;         // Web Server Variable
String content;   // Web Server Variable
int statusCode;   // Web Server Variable
String HOSTNAME;

ESP8266WebServer server(80); //Define Webserver
WiFiClient espClient;
PubSubClient client(espClient);

aREST rest = aREST(client);
char* device_id = "chahil";//six Char uniq id

int eepromandwifi();      //Read from EEPROM  End Return True/False and start wifi
int testwifi();    // Test WiFi End Return True/False
void startAP();     //Start SoftAP And Launch Config() As Config Portal
void Config();      //Take Value From Web Server And Store In EEPROM
void callback(char* topic, byte* payload, unsigned int length);
void led(int, int);

String avg_time_ms;
int i, c = 1;
int net = 0, j = 1;

void setup() {
  Serial.begin(115200);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");






  eepromandwifi();// Read WiFi Password From Program Memory
  startAP(); // start config portal + SoftAP
  delay(3000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" ");
    Serial.print("WiFi IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println(" ");
    internet();
  }



}

void loop()
{
  server.handleClient();
  internet();

  if (net == 1)
  {
    rest.handle(client);
    firebasereconnect();
    firebase();
  }

}

void internet()
{
  if (WiFi.status() == WL_CONNECTED) {
    if (j == 1)
    {
      j = 2;
      Serial.println(" ");
      Serial.print("WiFi IP address: ");
      Serial.print(WiFi.localIP());
      Serial.println(" ");
      Serial.print("Pinging host ");  // Just for troubleshot purpose.
      Serial.println(remote_host);
      if (Ping.ping(remote_host))
      {
        Ping.ping(remote_host, 3);  //10 time ping to google, You can change value to higher or lower
        i = Ping.averageTime();
        avg_time_ms = Ping.averageTime(); // reading string and Int for easy display integration.
        Serial.println(i);
        if (i < 99)
        {
          Serial.println("Good");
          net = 1;
          WiFi.hostname(HOSTNAME);
          Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
          client.setCallback(callback);
          rest.function("allon", &allon);
          rest.function("alloff", &alloff);

          rest.set_id(device_id);
          rest.set_name("chahil");
          char* out_topic = rest.get_topic();
        }
        else
        {
          Serial.println("Bad");
          net = 0;

        }
      }
    }
  }
}
void firebasereconnect()
{
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }

}

void firebase()
{
  int push = Firebase.getString("push").toInt();
  if (push == 1  ) {
    Firebase.setInt("push", 0);
    int relay1 = Firebase.getString("r1").toInt();
    int relay2 = Firebase.getString("r2").toInt();
    int relay3 = Firebase.getString("r3").toInt();
    int relay4 = Firebase.getString("r4").toInt();
    int all = Firebase.getString("all").toInt();
    if (relay1 == 1)
    {
      digitalWrite(D1, HIGH);
    }
    if (relay2 == 1)
    {
      digitalWrite(D2, HIGH);
    }
    if (relay3 == 1)
    {
      digitalWrite(D3, HIGH);
    }
    if (relay4 == 1)
    {
      digitalWrite(D4, HIGH);
    }
    if (relay1 == 0)
    {
      digitalWrite(D1, LOW);
    }
    if (relay2 == 0)
    {
      digitalWrite(D2, LOW);
    }
    if (relay3 == 0)
    {
      digitalWrite(D3, LOW);
    }
    if (relay4 == 0)
    {
      digitalWrite(D4, LOW);
    }
    if (all == 1)
    {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);
      Firebase.setInt("all", 0);
    }
    if (all == 2)
    {
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      digitalWrite(D4, LOW);
      Firebase.setInt("all", 0);
    }

    Firebase.setInt("s1", digitalRead(D1));
    Firebase.setInt("s2", digitalRead(D2));
    Firebase.setInt("s3", digitalRead(D3));
    Firebase.setInt("s4", digitalRead(D4));
  }

  if (push == 12)
  {
    Firebase.setInt("push", 0);
    Firebase.setInt("s1", digitalRead(D1));
    Firebase.setInt("s2", digitalRead(D2));
    Firebase.setInt("s3", digitalRead(D3));
    Firebase.setInt("s4", digitalRead(D4));

  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}




int eepromandwifi()
{
  EEPROM.begin(512);
  delay(10);
  Serial.println("Reading EEPROM ssid");
  String epassword = "";
  String essid;
  String token;
  for (int i = 0; i < 32; ++i)
  {
    essid += char(EEPROM.read(i));
  }
  Serial.println("SSID: ");
  Serial.println(essid);

  Serial.print("Reading EEPROM Password");
  for (int i = 32; i < 96; ++i)
  {
    epassword += char(EEPROM.read(i));
  }
  Serial.println("PASS: ");
  Serial.println(epassword);
  Serial.print("Reading EEPROM token");
  for (int i = 96; i < 160; ++i)
  {
    token += char(EEPROM.read(i));
  }
  Serial.println("Token: ");
  Serial.println(token);
  if ( essid.length() > 1 && epassword.length() > 1 && token.length() > 1 )
  {
    WiFi.begin(essid.c_str(), epassword.c_str());
    // WiFi.hostname(HOSTNAME);
    HOSTNAME = token.c_str();
    return 1;
  }
  if ( essid.length() < 1 && epassword.length() < 1 && token.length() < 1 )
  {
    Serial.println(" ");
    Serial.print("WiFi is Not Connected  ");
    return 0;
  }
}

void startAP()
{
  // WiFi.mode(WIFI_STA);
  // WiFi.disconnect();
  delay(100);
  IPAddress ip(10, 1, 1, 1);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(APssid, APpassword);
  Serial.println("................SoftAP Mode...................");
  Serial.println("  ");
  Serial.println("SSID: ");
  Serial.println(APssid);
  Serial.print("  ");
  Serial.println("Password: ");
  Serial.println(APpassword);
  Serial.print("  ");
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("  ");
  server.begin();
  Serial.println("Server started");
  Config();

}

String ipToString(IPAddress address)
{
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

void Config()
{
  server.on("/relay", []() {
    int relay = server.arg("relay").toInt();
    int relaystatus = server.arg("status").toInt();
    digitalWrite(relay, relaystatus);
    int a = digitalRead(D1);
    int b = digitalRead(D2);
    int c = digitalRead(D3);
    int d = digitalRead(D4);
    server.send(200, "text/plain", String("") + a + " " + b + " " + c + " " + d + " "  );
  });
  server.on("/ip", []() {
    String  local_ip = ipToString(WiFi.localIP());
    server.send(200, "text/plain", local_ip);
  });
  server.on("/setting", []() {
    String qssid = server.arg("ssid");
    String qpass = server.arg("pass");
    String qtoken = server.arg("token");

    if (qssid.length() > 0 && qpass.length() > 0 && qtoken.length() > 0)
    {
      Serial.println("clearing eeprom");
      for (int i = 0; i < 160; ++i)
      {
        EEPROM.write(i, 0);
      }

      Serial.println(qssid);
      Serial.println("");
      Serial.println(qpass);
      Serial.println("");
      Serial.println(qtoken);
      Serial.println("");

      Serial.println("writing eeprom ssid:");
      for (int i = 0; i < qssid.length(); ++i)
      {
        EEPROM.write(i, qssid[i]);
        Serial.print("Wrote: ");
        Serial.println(qssid[i]);
      }
      Serial.println("writing eeprom pass:");
      for (int i = 0; i < qpass.length(); ++i)
      {
        EEPROM.write(32 + i, qpass[i]);
        Serial.print("Wrote: ");
        Serial.println(qpass[i]);
      }
      Serial.println("writing eeprom token:");
      for (int i = 0; i < qtoken.length(); ++i)
      {
        EEPROM.write(96 + i, qtoken[i]);
        Serial.print("Wrote: ");
        Serial.println(qtoken[i]);
      }

      EEPROM.commit();
      content = "Done";
      statusCode = 200;
    }
    else {
      content = "Error";
      statusCode = 404;
      Serial.println("Error");
    }

    server.send(statusCode, "text/plain", content);
  });

  server.on("/cleareeprom", []() {
    content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<p>Clearing the EEPROM</p></html>";
    server.send(200, "text/html", content);
    Serial.println("clearing eeprom");
    for (int i = 0; i < 160; ++i) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
  });

  server.on("/restart", []() {
    server.send(200, "text/plain", "Restart in 5 Second");
    delay(5000);
    ESP.reset();

  });
  server.on("/allon", []() {
    digitalWrite(D1, 1);
    digitalWrite(D2, 1);
    digitalWrite(D3, 1);
    digitalWrite(D4, 1);
    server.send(200, "text/plain", "allon");


  });
  server.on("/alloff", []() {
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);
    digitalWrite(D4, 0);
    server.send(200, "text/plain", "alloff");


  });
}

int allon(String command)
{
  digitalWrite(D1, 1);
  digitalWrite(D2, 1);
  digitalWrite(D3, 1);
  digitalWrite(D4, 1);

}


int  alloff(String command)
{
  digitalWrite(D1, 0);
  digitalWrite(D2, 0);
  digitalWrite(D3, 0);
  digitalWrite(D4, 0);

}

