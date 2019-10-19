#include"fingerprint.cpp"
#include<HardwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Shreeji1";
const char* password = "12345678";

WebServer server(80);
HardwareSerial mySerial(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
bool flag = 0;
void setup()
{
  pinMode(19, INPUT_PULLUP);
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("");

  // Wait for connection
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());
  //
  //  if (MDNS.begin("esp32")) {
  //    Serial.println("MDNS responder started");
  //  }

  server.on("/", handleRoot);
  server.on("/enroll", enroll);
  server.on("/readFinger", readFinger);
  server.begin();
}

void loop() {

  server.handleClient();
  if (!digitalRead(19)) {
    while (!digitalRead(19));
    flag = !flag;
  }
  if (flag) {
    int fingid = getFingerprintIDez();
    if (fingid != -1)
    {

      Serial.print("ID"); Serial.println(fingid);
      flag = !flag;
    }
  }
}



void handleRoot() {

  server.send(200, "text/plain", "hello from esp8266!");

}

void enroll() {
  int id = server.arg("id").toInt();
  String enroll = server.arg("enroll");
  String Name = server.arg("name");
  if (id != 0)
  {
    Serial.print("New Finger Enroll With Id "); Serial.println(id);
    Serial.print("Name:"); Serial.println(Name);
    Serial.print("Enrollment:"); Serial.println(enroll);
    String Send = Name + " \n" + enroll;
    server.send(200, "text/plain", Send);
    getFingerprintEnroll(id);
    server.send(200, "text/plain", Send);

  }
}

void readFinger()
{

}

//int fingid = getFingerprintIDez();
// if (fingid != -1)
//  {
//    database(fingid);
//  }
//getFingerprintEnroll(id);
//deleteFingerprint(id);
//finger.emptyDatabase();
/************************************************FingerPrint***********************************************************************/
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" --> ");
  // Serial.print(" with confidence of ");
  // Serial.println(finger.confidence);
  return finger.fingerID;
}


uint8_t getFingerprintEnroll(int id) {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        // Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //  Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}
/********************************************************************************************************************************/
