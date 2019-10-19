
#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>
#include <NTPtimeESP.h>

#define DEBUG_ON


NTPtime NTPch("ch.pool.ntp.org"); 
#define IR_LED 4
String FIREBASE_HOST="grcrobocon.firebaseio.com"           ;          //Your Firebase Project URL goes here without "http:" , "\" and "/"
String FIREBASE_AUTH="iRITkIM1ezSQ1RKqltrpdQDhY6HzwZT8ZIM5PaT0" ;      //Your Firebase Database Secret goes here
#define WIFI_SSID "Shreeji"                                               //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "8866551417Cc"  

IRKelvinatorAC kelvir(IR_LED);
void AC();
//String a="abcd/";
strDateTime dateTime;
void setup()
{
  kelvir.begin();
  Serial.begin(115200);
  delay(200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   int dec = Firebase.getString("dec").toInt();
   if(dec==1)
   {
    Firebase.setInt("Power", 0);
    Firebase.setInt("fan", 3);
    Firebase.setInt("Mod",1);
    Firebase.setInt("temp", 22);
    Firebase.setInt("swingv", 1);
    Firebase.setInt("swingh", 0);
    Firebase.setInt("setxfan", 0);
    Firebase.setInt("filter", 0);
    Firebase.setInt("light", 1);
    Firebase.setInt("turbo", 0);
    Firebase.setInt("push", 0);
    Firebase.setInt("dec",0);
    
   }
  

}
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }


 // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment; (contributed by viewwer, not tested by me)
  dateTime = NTPch.getNTPtime(5.5, 0);

  // check dateTime.valid before using the returned time
  // Use "setSendInterval" or "setRecvTimeout" if required
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

   
  }


  AC();

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



void AC(){
 
int t = Firebase.getString("time").toInt();
int t1=dateTime.minute.toInt();
  if(t == t1){
  int p = Firebase.getString("push").toInt();  

  if (p == 1)
        {
   Serial.println("------------------------------------------------------------");
    int power = Firebase.getString("Power").toInt();
    int fan = Firebase.getString("fan").toInt();
    int Mod = Firebase.getString("Mod").toInt();
    int temp = Firebase.getString("temp").toInt();
    int swingv = Firebase.getString("swingv").toInt();
    int swingh = Firebase.getString("swingh").toInt();
    int setxfan = Firebase.getString("setxfan").toInt();
    int filter = Firebase.getString("filter").toInt();
    int turbo = Firebase.getString("turbo").toInt();
    int light = Firebase.getString("light").toInt();
    
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

    Firebase.setInt("push", 0);
  }
  }
}


