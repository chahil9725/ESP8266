#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

String  httpurl;
HTTPClient http;

void setup()
{
  Serial.begin(9600);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 1);
  Serial.println("START");
  WiFi.begin("Master", "12345678");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print(".");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));

}


void loop()
{

  httpurl = "http://";
  httpurl += "10.1.1.1";
  httpurl += "/";
  httpurl += "led?value=1&msg=ON";
  http.begin(httpurl);
  if (http.GET() == 200)
    digitalWrite(D4, HIGH);
  http.end();

  delay(500);
  
  httpurl = "http://";
  httpurl += "10.1.1.1";
  httpurl += "/";
  httpurl += "led?value=0&msg=OFF";
  http.begin(httpurl);
  if (http.GET() == 200)
    digitalWrite(D4, LOW);
  // Serial.println(http.getString());
  http.end();


  delay(500);
 

}
