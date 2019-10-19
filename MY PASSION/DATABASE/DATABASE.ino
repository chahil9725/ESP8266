#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


char ssid[] = "Shreeji";                 // Network Name
char pass[] = "8866551417Cc";                 // Network Password
byte mac[6];

WiFiServer server(80);


WiFiClient client;
MySQL_Connection conn((Client *)&client);

char INSERT_SQL[] = "INSERT INTO sql12257030.student (enroll,name,id,time1,date) VALUES (170120116047,'Chahil Patel',3,CURTIME(),CURDATE())"; //"INSERT INTO sql12257030.DBMS  VALUE(17047,'Chahil Patel',3,12:12:12,2018-9-12)";
char query[128]="SET time_zone = '+05:30';";

IPAddress server_addr(52,76,27,242);          // MySQL server IP
char user[] = "sql12257030";           // MySQL user
char password[] = "SGgCSXCu6V";       // MySQL password

void setup() {

  Serial.begin(9600);



  Serial.println("Initialising connection");
  Serial.print(F("Setting static ip to : "));
//  Serial.println(ip);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
 // WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  Serial.println("");
  Serial.print("Assigned IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  Serial.println("Connecting to database");
int first=millis();
  while (conn.connect(server_addr, 3306, user, password) != true) {
    delay(200);
    Serial.print ( "." );
  }

  Serial.println("");
  Serial.println("Connected to SQL Server!");  
  Serial.println(millis()-first);

}

void loop() {

 
 // sprintf(query, INSERT_SQL, soil_hum);
 
int first1=millis();
  Serial.println("Recording data.");
  Serial.println(INSERT_SQL);
  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query);
  cur_mem->execute(INSERT_SQL);

  delete cur_mem;
  Serial.println(millis()-first1);
delay(5000);
}
