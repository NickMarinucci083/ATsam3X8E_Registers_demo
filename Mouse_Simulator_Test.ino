#include <Mouse.h> //Mouse emulation over native USB

void setup() {
  // put your setup code here, to run once:
  pinMode(72, OUTPUT);  // TX NATIVE LED
  pinMode(73, OUTPUT);  // RX NATIVE LED

  digitalWrite(72, HIGH);
  digitalWrite(73, HIGH);

  Mouse.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(73, LOW); //Show Native USB traffic, LED's are active low
  delay(10); //Allow LED to shine so user sees board sending data as mouse moves

  Mouse.move(random(-100, 100), random(-100, 100), 0);   // (x, y, scroll)
  
  digitalWrite(73, HIGH);

  delay(1000);
}
