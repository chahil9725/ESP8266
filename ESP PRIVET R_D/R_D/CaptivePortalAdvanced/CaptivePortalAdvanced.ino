#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <Servo.h>
#include <EEPROM.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// aREST Pro key (that you can get at dashboard.arest.io)

 /* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "CHAHILXBOT";
const char *softAP_password = "12345678Cc";
char * key = "ynvthwtnjfywzhrd";

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[32] = "Shreeji";
char password[32] = "8866551417Cc";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;

/** Current WLAN status */
int status = WL_IDLE_STATUS;


void setup() {


   // prepare Motor Output Pins
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  
  // prepare GPIO5 relay 1
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);
  digitalWrite(D7,HIGH);
    digitalWrite(D8,HIGH);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
  pinMode(D6, OUTPUT); 
  pinMode(D5, OUTPUT);
  delay(1000);
  Serial.begin(9600);
  Serial.println();
   rest.setKey(key, client);

  // Set callback
  client.setCallback(callback);
   rest.set_name("XBoT");
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound ( handleNotFound );
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

  server.begin(); // Web server start
 
  Serial.println("HTTP server started");
  loadCredentials(); // Load WLAN credentials from network
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
    char* out_topic = rest.get_topic();
}

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
 
}

void loop() {

  if (connect) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        Serial.println ( "" );
        Serial.print ( "Connected to " );
        Serial.println ( ssid );
        Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );
 
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
  }
  // Do work:
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  rest.handle(client);
  server.handleClient();
 
     
}


 void motor_forward(){
  digitalWrite(D1,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  }
 void motor_stop(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  }
 void motor_back(){
   
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D4,LOW);
  }

 void motor_right(){
      digitalWrite(D1,LOW);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,HIGH);  
  }
 void motor_left(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
  }

void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}
