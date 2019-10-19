#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include<Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <FS.h>
#include <FirebaseArduino.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFiClient.h>
#include <NTPtimeESP.h>

#define DEBUG_ON
#define FIREBASE_HOST "dbms-9ff54.firebaseio.com"
#define FIREBASE_AUTH "iF0UC09YicY9oZ9N6EEct5gAdSkf8mxSkYqkjFgk"

NTPtime NTPch("asia.pool.ntp.org");
strDateTime dateTime;

const char* ssid = "Shreeji";
const char* pass = "8866551417Cc";
const char* apssid = "Biomatrix";
const char* appass = "8866551417Cc";

IPAddress ip(10, 1, 1, 1);
IPAddress gateway(10, 1, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server1(80); //Define Webserver
SoftwareSerial mySerial(D6, D7);
WiFiServer server(80);
WiFiClient client;

MySQL_Connection conn((Client *)&client);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
IPAddress server_addr(85, 10, 205, 173);    // MySQL server IP
char user[] = "chahilpatel";           // MySQL user
char password[] = "8866551417";      // MySQL password

int first, last;
String ReadEnroll = "";
String ReadName = "";
String Time = "";
String Date = "";
String myString;
char webpage[] PROGMEM = R"=====(<html>
<div class="form-style-6">
<h1>Fingerprint Management</h1>
<form action="/enroll" method="get">
<input type="text" name="name" placeholder="Your Name" />
<input type="text" name="enroll" placeholder="Enrollment" />
<input type="text" name="id" placeholder="ID" />
<input type="submit" value="Done" />
</form>
</div>
</html>
<style type="text/css">
.form-style-6{
    font: 95% Arial, Helvetica, sans-serif;
    max-width: 400px;
    margin: 10px auto;
    padding: 16px;
    background: #F7F7F7;
}
.form-style-6 h1{
    background: #43D1AF;
    padding: 20px 0;
    font-size: 140%;
    font-weight: 300;
    text-align: center;
    color: #fff;
    margin: -16px -16px 16px -16px;
}
.form-style-6 input[type="text"],
.form-style-6 input[type="date"],
.form-style-6 input[type="datetime"],
.form-style-6 input[type="email"],
.form-style-6 input[type="number"],
.form-style-6 input[type="search"],
.form-style-6 input[type="time"],
.form-style-6 input[type="url"],
.form-style-6 textarea,
.form-style-6 select 
{
    -webkit-transition: all 0.30s ease-in-out;
    -moz-transition: all 0.30s ease-in-out;
    -ms-transition: all 0.30s ease-in-out;
    -o-transition: all 0.30s ease-in-out;
    outline: none;
    box-sizing: border-box;
    -webkit-box-sizing: border-box;
    -moz-box-sizing: border-box;
    width: 100%;
    background: #fff;
    margin-bottom: 4%;
    border: 1px solid #ccc;
    padding: 3%;
    color: #555;
    font: 95% Arial, Helvetica, sans-serif;
}
.form-style-6 input[type="text"]:focus,
.form-style-6 input[type="date"]:focus,
.form-style-6 input[type="datetime"]:focus,
.form-style-6 input[type="email"]:focus,
.form-style-6 input[type="number"]:focus,
.form-style-6 input[type="search"]:focus,
.form-style-6 input[type="time"]:focus,
.form-style-6 input[type="url"]:focus,
.form-style-6 textarea:focus,
.form-style-6 select:focus
{
    box-shadow: 0 0 5px #43D1AF;
    padding: 3%;
    border: 1px solid #43D1AF;
}

.form-style-6 input[type="submit"],
.form-style-6 input[type="button"]{
    box-sizing: border-box;
    -webkit-box-sizing: border-box;
    -moz-box-sizing: border-box;
    width: 100%;
    padding: 3%;
    background: #43D1AF;
    border-bottom: 2px solid #30C29E;
    border-top-style: none;
    border-right-style: none;
    border-left-style: none;    
    color: #fff;
}
.form-style-6 input[type="submit"]:hover,
.form-style-6 input[type="button"]:hover{
    background: #2EBC99;
}
</style>)=====";
void setup()
{

  /* pinMode(D1, INPUT_PULLUP);
    pinMode(D2, INPUT_PULLUP);
    pinMode(D3, INPUT_PULLUP);
    pinMode(D4, INPUT_PULLUP);*/
  Serial.begin(9600);
  SPIFFS.begin();
  Serial.println("START");
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");

  WiFi.begin(ssid, pass);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(apssid, appass);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));
  while (conn.connect(server_addr, 3306, user, password) != true) {
    delay(200);
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("Connected to SQL Server!");

  serveron();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  int fingid = getFingerprintIDez();
  digitalWrite(D4, 1);
  if (fingid != -1)
  {
    database(fingid);
  }

  server1.handleClient();
  first = millis();
  if (first - last > 2500)
  {
    last = millis();
    GetTime();

  }
}
/************************************************WebServer***********************************************************************/
void serveron()
{
  server1.on("/", root);
  server1.on("/enroll", enroll);
  server1.on("/delete", deletefig);
  server1.on("/deleteall", deleteall);
  server1.on("led", led);
  server1.begin();
}

void root()
{
  server1.send_P(200, "text/html", webpage);
  Serial.println("Webpage");
}

void enroll()
{

  SPIFFS.begin();
  digitalWrite(D4, 1);
  int id = server1.arg("id").toInt();
  String enroll = server1.arg("enroll");
  String Name = server1.arg("name");
  if (id != 0)
  {
    Serial.print("New Finger Enroll With Id "); Serial.println(id);
    Serial.print("Name:"); Serial.println(Name);
    Serial.print("Enrollment:"); Serial.println(enroll);
    String Send = Name + " \n" + enroll;
    server1.send(200, "text/plain", Send);
    getFingerprintEnroll(id);
    digitalWrite(D4, 0);
    store(id, Name, enroll);

  }

}
void deletefig()
{
  Serial.println("delete");
  int id = server1.arg("id").toInt();
  deleteFingerprint(id);
  String Send = "Deleted The ID# ";
  server1.send(200, "text/plain", Send);
  Serial.print("deleted the #ID : "); Serial.println(id);
}
void led ()
{

  Serial.println("led");
}
void deleteall()
{
  SPIFFS.begin();
  Serial.println("Deleteing All Id ");
  finger.emptyDatabase();
  SPIFFS.format();
  Serial.println("Deleted All IDs");
  server1.send(200, "text/plain", "Deleted All ID");
  // SPIFFS.end();
}

/********************************************************************************************************************************/



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

void store(int id1, String Name, String Enroll )
{
  SPIFFS.begin();
  const char*  id = '/' + id1 + ".txt";
  Serial.println(id);
  File f = SPIFFS.open(id, "w");

  if (!f) {
    Serial.println("file open failed");
    File f = SPIFFS.open(id, "w");
  }
  f.print(Enroll);
  f.print(":");
  f.print(Name);
  f.close();


}
void readAll(int id)
{
  SPIFFS.begin();
  const char*  idname = '/' + id + ".txt";
  Serial.println(idname);
  File f = SPIFFS.open(idname, "r");

  if (!f) {
    Serial.println("file open failed");
    File f = SPIFFS.open(idname, "r");
  }
  if (f.available())
  {
    String myString = f.readString();
    int ind1;
    int ind2;
    ind1 = myString.indexOf(':');
    ReadEnroll = myString.substring(0, ind1);
    ind2 = myString.indexOf(':', ind1 + 1 );
    ReadName = myString.substring(ind1 + 1, ind2);
  }
  f.close();

}





/*************************Time*******************************/

void gettime()

{

  dateTime = NTPch.getNTPtime(4.5, 2.5);
  if (dateTime.valid) {
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    byte actualsecond = dateTime.second;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday = dateTime.day;
    byte actualdayofWeek = dateTime.dayofWeek;
  }
}
void GetTime()
{
  dateTime = NTPch.getNTPtime(4.5, 2.5);
  if (dateTime.valid) {
    String Day = String(dateTime.day);
    String Month = String(dateTime.month);
    String Year = String(dateTime.year);
    String Minit = String(dateTime.minute);
    String Hour = String(dateTime.hour);
    String Second = String(dateTime.second);
    Time = Hour + ":" + Minit + ":" + Second;
    Date = Year + "-" + Month + "-" + Day ;
  }
}


/******************************************************************/

void database(int id)
{
  int first = millis();
  readAll(id);
  GetTime();
  Serial.print(ReadName);
  Serial.print(":");
  Serial.print(ReadEnroll);
  Serial.print("  -->");
  Serial.print(Time + "    ");
  Serial.println(Date);
  String SQL = "INSERT INTO esp8266.studentdata (enroll,name,id,time1,date1) VALUES (" + ReadEnroll + ",'" + ReadName + "'," + id + ",'" + Time + "','" + Date + "'" + ")"; //"INSERT INTO sql12257030.DBMS  VALUE(17047,'Chahil Patel',3,12:12:12,2018-9-12)";
  char INSERT_SQL [SQL.length() + 5];
  SQL.toCharArray(INSERT_SQL, SQL.length() + 5);
  if (Firebase.getString(ReadEnroll + "/P").toInt() == 3)
  {
    Firebase.setString("Room/R", "1");

  }
  // Firebase.setString(ReadEnroll + "/" + "Attendace/" + Date + "/" + Time, "1" );
  //Firebase.setInt(ReadEnroll + "/" + "FingerID", id  );
  //Firebase.setString(ReadEnroll + "/" + "Name", ReadName );
  Serial.println(INSERT_SQL);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL);
  delete cur_mem;
  Serial.println(millis() - first);
  ReadName = "";
  ReadEnroll = "";
}
