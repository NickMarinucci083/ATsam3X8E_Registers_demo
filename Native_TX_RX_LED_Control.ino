//Manually use native TX and RX LEDs to show native USB traffic, they don't blink automatically
void setup() {
  pinMode(72, OUTPUT);  // RX LED
  pinMode(73, OUTPUT);  // TX LED

  digitalWrite(72, HIGH); //NOTE: LED'S ARE ACTIVE LOW!!!
  digitalWrite(73, HIGH);
}

void loop() {
  digitalWrite(72, LOW);   // RX LED ON
  delay(500);

  digitalWrite(72, HIGH);  // RX LED OFF
  delay(500);

  digitalWrite(73, LOW);   // TX LED ON
  delay(500);

  digitalWrite(73, HIGH);  // TX LED OFF
  delay(500);
}