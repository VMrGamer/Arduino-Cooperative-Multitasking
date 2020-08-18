void setup() {
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  Serial.begin(9600);
}
int x = 0;
void loop() {
  Serial.print("9: ");  Serial.println(digitalRead(9));
  Serial.print("8: ");  Serial.println(digitalRead(8));
  Serial.print("7: ");  Serial.println(digitalRead(7));
  Serial.print("6: ");  Serial.println(digitalRead(6));
  Serial.println("");
  delay(500);
}
