#include <FS.h>                  
#include <ESP8266WiFi.h>          
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         
#include <ArduinoJson.h>   
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>

       
String FIREBASE_HOST="irblaster-82aa4.firebaseio.com";
String A="/" ;
String a="" ;

String FIREBASE_AUTH="TyTLp9S800us9tYowbnaIAmMnq09Vru5dZQ2DDw5" ;      //Your Firebase Database Secret goes here
  int IR_LED=4   ;                           //Password of your wifi network 
IRKelvinatorAC kelvir(IR_LED);
void AC();
     
//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40];
char mqtt_port[6] = "8080";
char blynk_token[100] = "YOUR_BLYNK_TOKEN";
void run1();
//flag for saving data
bool shouldSaveConfig = false;
int i=1;
//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
pinMode(0, INPUT_PULLUP);
  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  //wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
 // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_blynk_token);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
//  wifiManager.setTimeout(120);
wifiManager.setBreakAfterConfig(true);
//wifiManager.setCaptivePortalEnable(false);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.setAPStaticIPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(blynk_token, custom_blynk_token.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
digitalWrite(LED_BUILTIN, HIGH); 
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {

if ( digitalRead(0) == LOW ) {
digitalWrite(LED_BUILTIN, HIGH);  
delay(500);                       
  digitalWrite(LED_BUILTIN, LOW);    
  digitalWrite(LED_BUILTIN, HIGH);  
delay(500);                       
  digitalWrite(LED_BUILTIN, LOW); 
  run1();
}
  
delay(200);
    a = blynk_token + A;


   if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }



  AC();
}
















void run1()
{
  WiFi.disconnect();


    Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  //wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
 // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_blynk_token);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
//  wifiManager.setTimeout(120);
//wifiManager.setBreakAfterConfig(true);
//wifiManager.setCaptivePortalEnable(false);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
    wifiManager.setAPStaticIPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(blynk_token, custom_blynk_token.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void printState() {
  // Display the settings.
  Serial.println("Kelvinator A/C remote is in the following state:");
  Serial.printf("  Basic\n  Power: %d,  Mode: %d, Temp: %dC, Fan Speed: %d\n",
                kelvir.getPower(), kelvir.getMode(), kelvir.getTemp(),
                kelvir.getFan());
  Serial.printf("  Options\n  X-Fan: %d,  Light: %d, Ion Filter: %d\n",
                kelvir.getXFan(), kelvir.getLight(), kelvir.getIonFilter());
  Serial.printf("  Swing (V): %d, Swing (H): %d, Turbo: %d, Quiet: %d\n",
                kelvir.getSwingVertical(), kelvir.getSwingHorizontal(),
                kelvir.getTurbo(), kelvir.getQuiet());

 
}



void AC()

{
  Firebase.setInt(a+"dec",0);
  int p = Firebase.getString(a+"push").toInt();  
 
  if (p == 1)
        {
   Serial.println("------------------------------------------------------------");
    int power = Firebase.getString(a+"Power").toInt();
    int fan = Firebase.getString(a+"fan").toInt();
    int Mod = Firebase.getString(a+"Mod").toInt();
    int temp = Firebase.getString(a+"temp").toInt();
    int swingv = Firebase.getString(a+"swingv").toInt();
    int swingh = Firebase.getString(a+"swingh").toInt();
    int setxfan = Firebase.getString(a+"setxfan").toInt();
    int filter = Firebase.getString(a+"filter").toInt();
    int turbo = Firebase.getString(a+"turbo").toInt();
    int light = Firebase.getString(a+"light").toInt();
    
     Serial.print("Tha Thempratere is");
    Serial.println(temp);
    kelvir.setFan(fan);
    if (power == 1)
    {
      kelvir.setPower(true);
      Serial.println("AC Set to On");
    }
    if (power == 0)
    {
      kelvir.setPower(false);
       Serial.println("AC Set to Off");
    }
             
     if (Mod==1)
    {
      kelvir.setMode(KELVINATOR_COOL);
       Serial.println("Mode Set to COOL");
    }
    if (Mod==2)
    {
      kelvir.setMode(KELVINATOR_DRY);
       Serial.println("Mode Set to DRY");
    }
    if (Mod==3)
    {
      kelvir.setMode(KELVINATOR_FAN);
       Serial.println("Mode Set to FAN");
    }

    if (swingv == 1)
    {
      kelvir.setSwingVertical(true);
       Serial.println("Vertical Swing On");
    }
    if (swingv == 0)
    {
      kelvir.setSwingVertical(false);
      Serial.println("Vertical Swing Off");
    }
    if (swingh == 1)
    {
      kelvir.setSwingHorizontal(true);
      Serial.println("Horizontal Swing On");
    }
    if (swingh == 0)
    {
      kelvir.setSwingHorizontal(false);
      Serial.println("Horizontal Swing Off");
    }
    if (light == 1)
    {
      kelvir.setLight(true);
      Serial.println("Light On");
    }
    if (light == 0)
    {
      kelvir.setLight(false);
      Serial.println("Light Off");
    }
    if (turbo == 1)
    {
      kelvir.setTurbo(true);
      Serial.println("Turbo On");
    }
    if (turbo == 0)
    {
      kelvir.setTurbo(false);
      Serial.println("Turbo Off");
    }
if(temp==16)
{
kelvir.setTemp(16);
}
if(temp==18)
{
kelvir.setTemp(18);
}
if(temp==19)
{
kelvir.setTemp(19);
}
if(temp==20)
{
kelvir.setTemp(20);
}
if(temp==21)
{
kelvir.setTemp(21);
}
if(temp==22)
{
kelvir.setTemp(22);
}
if(temp==23)
{
kelvir.setTemp(23);
}
if(temp==24)
{
kelvir.setTemp(24);
}
if(temp==25)
{
kelvir.setTemp(25);
}if(temp==26)
{
kelvir.setTemp(26);
}if(temp==27)
{
kelvir.setTemp(27);
}if(temp==28)
{
kelvir.setTemp(28);
}
if(temp==29)
{
kelvir.setTemp(29);
}
if(temp==30)
{
kelvir.setTemp(30);
}
#if SEND_KELVINATOR
    Serial.println("------------------------------------------------------------");
    kelvir.send();
#endif
   printState();

    Firebase.setInt(a+"push", 0);
  }

}

