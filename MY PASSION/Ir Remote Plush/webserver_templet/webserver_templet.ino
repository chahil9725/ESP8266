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
#include <WiFiUdp.h>

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

int  j = 0, k,p=1;

decode_results results;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[750];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


//const char* essid = "";    //SSID  to Connect WiFi
//const char* epassword = ""; //Password TO Connect WiFi
#define IR_LED D7 // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);
const char* APssid = "AC REMOTE";   //Config Portal SSID
const char* APpassword = "12345678"; //Config Portal Password



String st;         // Web Server Variable
String content;   // Web Server Variable
int statusCode;   // Web Server Variable
String code;


void setup() {

  delay(100);
  IPAddress ip(10, 1, 1, 1);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(APssid, APpassword);
    Serial.begin(115200);
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
 
 
  
#if DECODE_HASH

  irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
  irrecv.enableIRIn();
  irsend.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("...........................................");
  Serial.println("                   Start                   ");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

 
}

void loop()
{
getData();
  String code1 = "";
if(p==1)
{
  if (irrecv.decode(&results)) {
    code = "";
    int i = 1;

    if (results.overflow)
      Serial.printf("WARNING: IR code is too big for buffer (>= %d). "
                    "This result shouldn't be trusted until this is resolved. "
                    "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                    CAPTURE_BUFFER_SIZE);


    Serial.println("....................");
    while (results.rawlen != i)
    {
      code += code1 + results.rawbuf[i] * 2 + ",";
      i++;
    }
    Serial.println(code);
    yield();
    
  }
  }
}
void getData()
{
  int packetSize = Udp.parsePacket();
   if (packetSize)
  {
    int len = Udp.read(incomingPacket, 750);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);

    switch (incomingPacket[0]) {
      case 's':
      {
        Serial.println(code);
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print(code);
        Udp.endPacket();
        code="";
        break;
      }
    default :
    String data(incomingPacket);
    p=0;
    Serial.println(parseStringAndSendGC(irsend ,data));
    
    p=1;
    break;
  }
}}
bool parseStringAndSendGC(IRsend irsend, const String str) {
  uint16_t count;
  uint16_t *code_array;
  String tmp_str;

  // Remove the leading "1:1,1," if present.
  if (str.startsWith("1:1,1,"))
    tmp_str = str.substring(6);
  else
    tmp_str = str;

  // Find out how many items there are in the string.
  count = countValuesInStr(tmp_str, ',');

  // Now we know how many there are, allocate the memory to store them all.
  code_array = newCodeArray(count);

  // Now convert the strings to integers and place them in code_array.
  count = 0;
  uint16_t start_from = 0;
  int16_t index = -1;
  do {
    index = tmp_str.indexOf(',', start_from);
    code_array[count] = tmp_str.substring(start_from, index).toInt();
    start_from = index + 1;
    count++;
  } while (index != -1);

   irsend.sendRaw(code_array, count, 38);
  free(code_array);  // Free up the memory allocated.
  if (count > 0)
    return true;  // We sent something.
  return false;  // We probably didn't.
}
uint16_t countValuesInStr(const String str, char sep) {
  int16_t index = -1;
  uint16_t count = 1;
  do {
    index = str.indexOf(sep, index + 1);
    count++;
  } while (index != -1);
  return count;
}

uint16_t * newCodeArray(const uint16_t size) {
  uint16_t *result;

  result = reinterpret_cast<uint16_t*>(malloc(size * sizeof(uint16_t)));
  // Check we malloc'ed successfully.
  if (result == NULL) {  // malloc failed, so give up.
    Serial.printf("\nCan't allocate %d bytes. (%d bytes free)\n",
                  size * sizeof(uint16_t), ESP.getFreeHeap());
    Serial.println("Giving up & forcing a reboot.");
    ESP.restart();  // Reboot.
    delay(500);  // Wait for the restart to happen.
    return result;  // Should never get here, but just in case.
  }
  return result;
}
