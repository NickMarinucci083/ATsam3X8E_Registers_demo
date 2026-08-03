#define DAC DAC0 //DAC you want to test

void setup() {
  Serial.begin(9600);
  analogWriteResolution(12);  // 0–4095, highest resoloution
  Serial.println("Input value between 0 and 4095 and DAC will be written value");
  analogWrite(DAC, 0x000); //DAC off initally, 12 bit number
  Serial.setTimeout(999999999);  // Remove serial timeout rubbish
}

void loop() {
  if (Serial.available()) { //When serial gets data
    int value = Serial.parseInt();  // reads integer from serial

    // clamp to 12-bit range
    if (value < 0) {
      value = 0;
      Serial.println("Value below 0");
    }

    if (value > 4095){
      value = 4095;
      Serial.println("Value above 4095");
    }

    analogWrite(DAC, value);
    Serial.print("DAC0 = ");
    Serial.println(value);
  }
}