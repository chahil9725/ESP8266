/**************************************
  This Code is Written By Chahil Patel
  and Allowed To Modifiy .
  Contect:chahil9725@gmail.com         *
***************************************/


#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>
#include <ir_Toshiba.h>
#include <ir_Mitsubishi.h>
#include <ir_Argo.h>

const char* APssid = "AC REMOTE";   //Config Portal SSID
const char* APpassword = "12345678"; //Config Portal Password

//String token = "";    //User Token Comes From WiFi
String firebaseurl = ""; //Defiend in Code Hear
String firebasetoken = ""; // Defiend in Code Hear
int irled = D2;          // IR LED PIN
int button = 0;         // Config portal Pin

IRKelvinatorAC kelvir(irled);
IRToshibaAC toshibair(irled);
IRMitsubishiAC mitsubir(irled);
IRArgoAC argoir(irled);

String st;         // Web Server Variable
String content;   // Web Server Variable
int statusCode;   // Web Server Variable

ESP8266WebServer server(80); //Define Webserver

int eepromandwifi();      //Read from EEPROM  End Return True/False and start wifi
int testwifi();    // Test WiFi End Return True/False
void startAP();     //Start SoftAP And Launch Config() As Config Portal
void Config();      //Take Value From Web Server And Store In EEPROM
void wifiac();     // Start Smart AC on WiFi Local
void led(int, int); //Led Blynker led(numberoftime,delay);
void AC(int, int, int, int, int, int, int, int);

void setup() {
  Serial.begin(115200);
  kelvir.begin();
  mitsubir.begin();
  toshibair.begin();
  argoir.begin();
  WiFi.mode(WIFI_AP_STA);
  pinMode(button, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  wifiac();

  if (eepromandwifi() == 1) // reading frome eeprom and connecting wifi if eeprom fail it lunch config portal
  {

    if (testwifi() == 0) // testing wifi  conectivitiy if fails it launched config portal
    {
      Serial.println("Lunching Config Portal");
      led(5, 100);
      startAP(); // start config portal + SoftAP

    }
  }
  if (eepromandwifi() == 0)
  {
    Serial.println("Lunching Config Portal");
    led(5, 100);
    startAP();

  }
  startAP();
}

void loop()
{
  server.handleClient();
  if (digitalRead(button) == LOW)
  {
    Serial.print("Booted IN To ConFig Mode");
    delay(2000);
    led(5, 100);
    startAP();
  }

}
void led(int a, int d)
{
  for (int i = 0; i > a; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
  }
}

int testwifi()
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("Local IP: ");
      Serial.println(WiFi.localIP());
      return 1;
    }
    delay(400);
    Serial.print(".");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return 0;

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
    return 1;
  }
  if ( essid.length() < 1 && epassword.length() < 1 && token.length() < 1 )
  {
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

void Config()
{
  server.on("/", []() {

    server.send(200, "text/html", "Hello From Chahil");
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
    led(3, 200);
    server.send(200, "text/plain", "Restart in 5 Second");
    delay(5000);
    ESP.reset();

  });
}


void wifiac()
{

  server.on("/ac", []() {
    int power = server.arg("power").toInt();
    int fan = server.arg("fan").toInt();
    int Mod = server.arg("mode").toInt();
    int temp = server.arg("temp").toInt();
    int turbo = server.arg("turbo").toInt();
    int light = server.arg("light").toInt();
    int push = server.arg("push").toInt();
    int ac = server.arg("acb").toInt();
    AC(power, fan, Mod, temp, turbo, light, push,ac);
    server.send(200, "text/html", "Done");

  });

}

void AC(int power, int fan, int Mod, int temp, int turbo, int light, int push, int ac)
{

  if (push == 1)
  {
    Serial.println("......................................................");

    if (power == 1)
    {
      kelvir.setPower(true);
      toshibair.setPower(true);
       mitsubir.on();
       argoir.setPower(true);
      Serial.println("                AC Set to On");
    }
    if (power == 0)
    {
      kelvir.setPower(false);
      toshibair.setPower(false);
       mitsubir.off();
       argoir.setPower(false);
      Serial.println("                AC Set to Off");
    }

    if (Mod == 1)
    {
      kelvir.setMode(KELVINATOR_COOL);
      toshibair.setMode(TOSHIBA_AC_COOL);
      mitsubir.setMode(MITSUBISHI_AC_COOL);
       argoir.setCoolMode(ARGO_COOL_AUTO);
      Serial.println("                Mode Set to COOL");
    }
    if (Mod == 2)
    {
      kelvir.setMode(KELVINATOR_DRY);
      toshibair.setMode(TOSHIBA_AC_DRY);
      mitsubir.setMode(MITSUBISHI_AC_DRY);
       
      Serial.println("                Mode Set to DRY");
    }
    if (Mod == 3)
    {
      kelvir.setMode(KELVINATOR_FAN);
     argoir.setMode(ARGO_FAN_AUTO);
      Serial.println("                Mode Set to FAN");
    }
    if (Mod == 4)
    {
      kelvir.setMode(KELVINATOR_HEAT);
      toshibair.setMode(TOSHIBA_AC_HEAT);
       mitsubir.setMode(MITSUBISHI_AC_HEAT);
      Serial.println("                Mode Set to HEAT");
    }
     if (Mod == 5)
    {
      kelvir.setMode(KELVINATOR_HEAT);
      toshibair.setMode(TOSHIBA_AC_HEAT);
       mitsubir.setMode(MITSUBISHI_AC_HEAT);
      Serial.println("                Mode Set to HEAT");
    }
    if (light == 1)
    {
      kelvir.setLight(true);
      //toshibair.setLight(true);
      Serial.println("                Light On");
    }
    if (light == 0)
    {
      kelvir.setLight(false);
   //   toshibair.setLight(false);
      Serial.println("                Light Off");
    }
    if (turbo == 1)
    {
      kelvir.setTurbo(true);
   //   toshibair.setTurbo(true);
      Serial.println("                Turbo On");
    }
    if (turbo == 0)
    {
      kelvir.setTurbo(false);
    //  toshibair.setTurbo(false);
      Serial.println("                Turbo On");
    }
    if (temp == 16)
    {
      kelvir.setTemp(16);
      toshibair.setTemp(16);
      mitsubir.setTemp(16);
    }
    if (temp == 17)
    {
      kelvir.setTemp(17);
      toshibair.setTemp(17);
      mitsubir.setTemp(17);
    }
    if (temp == 18)
    {
      kelvir.setTemp(18);
      toshibair.setTemp(18);
      mitsubir.setTemp(18);
    }
    if (temp == 19)
    {
      kelvir.setTemp(19);
      toshibair.setTemp(19);
      mitsubir.setTemp(19);
    }
    if (temp == 20)
    {
      kelvir.setTemp(20);
      toshibair.setTemp(20);
      mitsubir.setTemp(20);
    }
    if (temp == 21)
    {
      kelvir.setTemp(21);
      toshibair.setTemp(21);
      mitsubir.setTemp(21);
    }
    if (temp == 22)
    {
      kelvir.setTemp(22);
      toshibair.setTemp(22);
      mitsubir.setTemp(22);
    }
    if (temp == 23)
    {
      kelvir.setTemp(23);
      toshibair.setTemp(23);
      mitsubir.setTemp(23);
    }
    if (temp == 24)
    {
      kelvir.setTemp(24);
      toshibair.setTemp(24);
      mitsubir.setTemp(24);
    }
    if (temp == 25)
    {
      kelvir.setTemp(25);
      toshibair.setTemp(25);
      mitsubir.setTemp(25);
    } 
    if (temp == 26)
    {
      kelvir.setTemp(26);
      toshibair.setTemp(26);
      mitsubir.setTemp(26);
    }
    if (temp == 27)
    {
      kelvir.setTemp(27);
      toshibair.setTemp(27);
      mitsubir.setTemp(27);
    } 
    if (temp == 28)
    {
      toshibair.setTemp(28);
      kelvir.setTemp(28);
      mitsubir.setTemp(28);
    }
    if (temp == 29)
    {
      kelvir.setTemp(29);
      toshibair.setTemp(29);
      mitsubir.setTemp(29);
    }
    if (temp == 30)
    {
      kelvir.setTemp(30);
      toshibair.setTemp(30);
      mitsubir.setTemp(30);
    }
    if (fan == 0)
    {
      kelvir.setFan(0);
      toshibair.setFan(0);

    }
    if (fan == 1)
    {
      kelvir.setFan(1);
      toshibair.setFan(1);
    }
    if (fan == 2)
    {
      kelvir.setFan(2);
      toshibair.setFan(2);
    }
    if (fan == 3)
    {
      kelvir.setFan(3);
      toshibair.setFan(3);


    }
    Serial.print("                TEMPRATURE is ");
    Serial.print(kelvir.getTemp());
    Serial.println(" ");
    Serial.print("                FAN : ");
    Serial.print(kelvir.getFan());
    Serial.println(" ");
  }
  if (ac == 1)
  {
#if SEND_KELVINATOR
 Serial.println("SEND_KELVINATOR");
    kelvir.send();
#endif
  }
  if (ac == 2)
  {
#if SEND_TOSHIBA_AC
    Serial.println("SEND_TOSHIBA_AC");
    toshibair.send();
#endif 
  }
   if (ac == 3)
  {
#if SEND_MITSUBISHI_AC
  Serial.println("SEND_MITSUBISHI_AC");
  mitsubir.send();
#endif
  }
}




void printState() {

  Serial.println("Toshiba A/C remote is in the following state:");
  Serial.printf("  Power: %d,  Mode: %d, Temp: %dC, Fan Speed: %d\n",
                toshibair.getPower(), toshibair.getMode(), toshibair.getTemp(),
                toshibair.getFan());
  // Display the encoded IR sequence.
  unsigned char* ir_code = toshibair.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < TOSHIBA_AC_STATE_LENGTH; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

