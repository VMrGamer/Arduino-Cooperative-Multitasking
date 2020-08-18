void setup() {
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
}
int x = 0;
void loop() {
  if(digitalRead(9)){
    x = !x;
    digitalWrite(13, x);
    delay(100);
    while(digitalRead(9));
  }
}
