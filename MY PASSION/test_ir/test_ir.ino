void setup() {
 pinMode(D2,OUTPUT);
}

void loop() {
  digitalWrite(D2,HIGH);
 delayMicroseconds(12);
  digitalWrite(D2,LOW);
  delayMicroseconds(12);
}
