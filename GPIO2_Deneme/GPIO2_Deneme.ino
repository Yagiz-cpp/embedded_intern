void setup(){
  Serial.begin(115200);
  delay(1000);
  pinMode(2, INPUT);
}

void loop(){
  Serial.println(digitalRead(2));
  delay(500);
}