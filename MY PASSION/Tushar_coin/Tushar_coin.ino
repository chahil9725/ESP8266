int coin = 13;
int led = 11;
int relay = 1;
int timer = 3;
int T1 = A0;
int T2 = A1;
int T3 = A2;
int T4 = A3;
void setup() {
//  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(coin, INPUT_PULLUP);
   pinMode(T1, INPUT_PULLUP);
    pinMode(T2, INPUT_PULLUP);
     pinMode(T3, INPUT_PULLUP);
      pinMode(T4, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(led, 0);
  digitalWrite(relay, 0);
}


void loop() {
 
  if (!digitalRead(coin) && digitalRead(T1) && digitalRead(T2) && digitalRead(T3) && digitalRead(T4))
  {
    digitalWrite(led, 1);
    digitalWrite(relay, 1);
    delay(60000);
    digitalWrite(led, 0);
    digitalWrite(relay, 0);
  }
  if (!digitalRead(coin) && !digitalRead(T1))
  {
    digitalWrite(led, 1);
    digitalWrite(relay, 1);
    delay(120000);
    digitalWrite(led, 0);
    digitalWrite(relay, 0);
  }
  if (!digitalRead(coin) && !digitalRead(T2))
  {
    digitalWrite(led, 1);
    digitalWrite(relay, 1);
    delay(180000);
    digitalWrite(led, 0);
    digitalWrite(relay, 0);
  }
  if (!digitalRead(coin) && !digitalRead(T3))
  {
    digitalWrite(led, 1);
    digitalWrite(relay, 1);
    delay(240000);
    digitalWrite(led, 0);
    digitalWrite(relay, 0);
  }
  if (!digitalRead(coin) && !digitalRead(T4))
  {
    digitalWrite(led, 1);
    digitalWrite(relay, 1);
    delay(300000);
    digitalWrite(led, 0);
    digitalWrite(relay, 0);
  }
  delay(1);        
}
