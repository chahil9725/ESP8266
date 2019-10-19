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
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>
#endif  // DECODE_AC
#define RECV_PIN D5
#define BAUD_RATE 115200
#define CAPTURE_BUFFER_SIZE 1024
#if DECODE_AC
#define TIMEOUT 50U
#else
#define TIMEOUT 15U
#endif
#define MIN_UNKNOWN_SIZE 12



IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);

int  j = 0, k, p = 1;

decode_results results;

long int first;

//const char* essid = "";    //SSID  to Connect WiFi
//const char* epassword = ""; //Password TO Connect WiFi
#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);
const char* APssid = "AC REMOTE";   //Config Portal SSID
const char* APpassword = "12345678"; //Config Portal Password



String st;         // Web Server Variable
String content;   // Web Server Variable
int statusCode;   // Web Server Variable
String code;
String Type;
int Data;
int Bits;
ESP8266WebServer server(80); //Define Webserver

int eepromandwifi();      //Read from EEPROM  End Return True/False and start wifi
int testwifi();    // Test WiFi End Return True/False
void startAP();     //Start SoftAP And Launch Config() As Config Portal
void Config();      //Take Value From Web Server And Store In EEPROM
void led(int, int); //Led Blynker led(numberoftime,delay);
void root();


void setup() {
  pinMode(D4,OUTPUT);
  digitalWrite(D4,1);
  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
#if DECODE_HASH

  irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
  irrecv.enableIRIn();
  irsend.begin();
  WiFi.mode(WIFI_AP_STA);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  eepromandwifi();

  startAP(); // start config portal + SoftAP



}

void loop()
{
  server.handleClient();

if (first<=millis()+200)
digitalWrite(D4,1);
  String code1 = "";
  if (p == 1)
  {
    if (irrecv.decode(&results)) {
      dump(&results);
      Serial.println("....................");
    }
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

  Serial.println("Server started");
  server.on("/", root);
  server.on("/setting", setting);
  server.on("/restart", restart);
  server.on("/copyir", copyir);
  server.on("/cleareeprom", cleareeprom);
  server.begin();

}


void root()
{


  String myString =server.arg("Data");
  int commaIndex = myString.indexOf(':');
  int secondCommaIndex = myString.indexOf(':', commaIndex + 1);
  String type = myString.substring(0, commaIndex);
  int data = myString.substring(commaIndex + 1, secondCommaIndex).toInt();
  int bits = myString.substring(secondCommaIndex + 1).toInt();
  Serial.println(type);
  Serial.println(data);
  Serial.println(bits);
 SendIr(type,data,bits);

  server.send(200, "text/html", "done");

}

void setting()
{
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
}


void cleareeprom()
{
  content = "<!DOCTYPE HTML>\r\n<html>";
  content += "<p>Clearing the EEPROM</p></html>";
  server.send(200, "text/html", content);
  Serial.println("clearing eeprom");
  for (int i = 0; i < 160; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void restart()
{
  led(3, 200);
  server.send(200, "text/plain", "Restart in 5 Second");
  delay(5000);
  ESP.reset();

}

void copyir()
{
  led(3, 200);
  code =  String(Type) + ":" + String(Data) + ":" + String(Bits);
  Serial.println(code);
  server.send(200, "text/plain", code);
  //  code = "";

}


/*******************************************************************************************************************************/


void dump(decode_results *results) {

  uint16_t count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Type = "UNKNOWN";
    Serial.print("Unknown encoding: ");
    digitalWrite(D4,0);
    first = millis();
     } else if (results->decode_type == NEC) {
    Type = "NEC";
    Serial.print("Decoded NEC: ");
  } else if (results->decode_type == SONY) {
    Type = "SONY";
    Serial.print("Decoded SONY: ");
  } else if (results->decode_type == RC5) {
    Type = "RC5";
    Serial.print("Decoded RC5: ");
  } else if (results->decode_type == RC5X) {
    Type = "RC5X";
    Serial.print("Decoded RC5X: ");
  } else if (results->decode_type == RC6) {
    Type = "RC6";
    Serial.print("Decoded RC6: ");
  } else if (results->decode_type == RCMM) {
    Type = "RCMM";
    Serial.print("Decoded RCMM: ");
  } else if (results->decode_type == PANASONIC) {
    Type = "PANASONIC";
    Data = results->address;
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  } else if (results->decode_type == LG) {
    Type = "LG";
    Serial.print("Decoded LG: ");
  } else if (results->decode_type == JVC) {
    Type = "JVC";
    Serial.print("Decoded JVC: ");
  } else if (results->decode_type == AIWA_RC_T501) {
    Type = "AIWA_RC_T501";
    Serial.print("Decoded AIWA RC T501: ");
  } else if (results->decode_type == WHYNTER) {
    Type = "WHYNTER";
    Serial.print("Decoded Whynter: ");
  } else if (results->decode_type == NIKAI) {
    Type = "NIKAI";
    Serial.print("Decoded Nikai: ");
  }
  Data = results->value;
  Bits = results->bits;
  Serial.println(Data);
  Serial.println(Bits);
  delay(300);
  // serialPrintUint64(results->value);
  // Serial.print(" (");
  // Serial.print(results->bits, DEC);
  // Serial.println(" bits)");
  // Serial.print("Raw (");
  // Serial.print(count, DEC);
  //  Serial.print("): {");

  /* for (uint16_t i = 1; i < count; i++) {
      if (i % 100 == 0)
        yield();  // Preemptive yield every 100th entry to feed the WDT.
      if (i & 1) {
        Serial.print(results->rawbuf[i] * RAWTICK, DEC);
      } else {
        Serial.print(", ");
        Serial.print((uint32_t) results->rawbuf[i] * RAWTICK, DEC);
      }
    }
    Serial.println("};");*/
}


void SendIr(String type,int data,int bits)

{
 if (type == "NEC")
  {
#if SEND_NEC
    Serial.println("NEC");
    irsend.sendNEC(data, bits);
#endif  // SEND_NEC

  }
  if (type == "SONY")
  {
#if SEND_SONY
    Serial.println("SONY");
    irsend.sendSony(data, bits, 1);
#endif  // SEND_NEC
  }


 if (type == "RC5")
  {
#if SEND_RC5
    Serial.println("RC5");
    irsend.sendRC5(data, bits, 1);
#endif  
  }

if (type == "RC5X")
  {
#if SEND_RC5
    Serial.println("RC5X");
    irsend.sendRC5(data, bits, 1);
#endif  
  }
  if (type == "RC6")
  {
#if SEND_RC6
    Serial.println("RC6");
    irsend.sendRC6(data, bits, 1);
#endif  
  }
  if (type == "RCMM")
  {
#if SEND_RCMM
    Serial.println("RCMM");
    irsend.sendRCMM(data, bits, 1);
#endif  
  }
  if (type == "PANASONIC")
  {
#if SEND_PANASONIC
    Serial.println("PANASONIC");
    irsend.sendPanasonic(data, bits, 1);
#endif  
  }
  if (type == "LG")
  {
#if SEND_LG
    Serial.println("LG");
    irsend.sendLG(data, bits, 1);
#endif  
  }
  if (type == "JVC")
  {
#if SEND_JVC
    Serial.println("");
    irsend.sendJVC(data, bits, 1);
#endif  
  }
  if (type == "AIWA_RC_T501")
  {
#if SEND_AIWA_RC_T501
    Serial.println("AIWA_RC_T501");
    irsend.sendAiwaRCT501(data, bits, 1);
#endif  
  }

if (type == "WHYNTER")
  {
#if SEND_WHYNTER
    Serial.println("WHYNTER");
    irsend.sendWhynter(data, bits, 1);
#endif  
  }

if (type == "NIKAI")
  {
#if SEND_NIKAI
    Serial.println("NIKAI");
    irsend.sendNikai(data, bits, 1);
#endif  
  }


}
