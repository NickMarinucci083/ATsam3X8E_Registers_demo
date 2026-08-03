#define IR_PIN 13

void setup() {
  pinMode(IR_PIN, INPUT);
  SerialUSB.begin(115200);

  Keyboard.begin();
  delay(800); //Give PC time to recognise device
}

uint32_t readIR() {

  static uint32_t lastCode = 0;  // MUST persist across calls

  if (digitalRead(IR_PIN) == HIGH)
    return 0;

  // Measure leading LOW pulse
  unsigned long start = micros();
  while (digitalRead(IR_PIN) == LOW);
  unsigned long lowTime = micros() - start;

  if (lowTime < 8500 || lowTime > 9500)
    return 0;

  // Measure leading HIGH pulse
  start = micros();
  while (digitalRead(IR_PIN) == HIGH);
  unsigned long highTime = micros() - start;

  // NEC repeat code (button held)
  // Usually ~2.25ms HIGH after 9ms LOW
  if (highTime > 2000 && highTime < 2800) {
    // consume remaining frame time if needed
    lastCode = lastCode;   // unchanged, just explicit
    return lastCode;
  }

  // Normal NEC frame
  if (highTime < 4000 || highTime > 5000)
    return 0;

  uint32_t data = 0;

  for (int i = 0; i < 32; i++) {

    while (digitalRead(IR_PIN) == LOW);

    start = micros();
    while (digitalRead(IR_PIN) == HIGH);
    unsigned long pulse = micros() - start;

    data <<= 1;

    if (pulse > 1000)
      data |= 1;
  }

  lastCode = data;   // update stored code
  return data;
}

void loop() {
  uint32_t code = readIR(); //readIR returns 32 bit code

  if (code != 0) { //Ignore if no code detected

    Serial.print("Raw hex code: 0x");
    Serial.print(code, HEX);
    Serial.print(" | Raw binary 32-bit code: ");
    Serial.print(code, BIN);
    Serial.print(" | Button: ");

    if (code == 0xFF08F7) { Serial.println("Power");} //Labels for Dad's Medion internet radio remote
    else if (code == 0xFF30CF) { Serial.println("Mute");}
    else if (code == 0xFF40BF) { Serial.println("Foward");}
    else if (code == 0xFF827D) { Serial.println("Stop");}
    else if (code == 0xFF38C7) { Serial.println("Play/Pause");}
    else if (code == 0xFFA05F) { Serial.println("Back");}
    else if (code == 0xFFB04F) { Serial.println("Star");}
    else if (code == 0xFF807F) { Serial.println("Scan");}
    else if (code == 0xFF58A7) { Serial.println("EQ");}
    else if (code == 0xFF4AB5) { Serial.println("Info");}
    else if (code == 0xFF609F) { Serial.println("Mode");}
    else if (code == 0xFFE817) { Serial.println("Menu");}
    else if (code == 0xFF48B7) { Serial.println("Up");}
    else if (code == 0xFF02FD) { Serial.println("Left");}
    else if (code == 0xFF32CD) { Serial.println("OK");}
    else if (code == 0xFF20DF) { Serial.println("Right");}
    else if (code == 0xFF7887) { Serial.println("Down");}
    else if (code == 0xFF28D7) { Serial.println("Volume down");}
    else if (code == 0xFFF00F) { Serial.println("Volume up");}
    else if (code == 0xFF906F) { Serial.println("1");}
    else if (code == 0xFFB847) { Serial.println("2");}
    else if (code == 0xFFF807) { Serial.println("3");}
    else if (code == 0xFF9867) { Serial.println("4");}
    else if (code == 0xFFD827) { Serial.println("5");}
    else if (code == 0xFFB24D) { Serial.println("6");}
    else if (code == 0xFF00FF) { Serial.println("7");}
    else if (code == 0xFF12ED) { Serial.println("8");}
    else if (code == 0xFF2AD5) { Serial.println("9");}
    else if (code == 0xFFC03F) { Serial.println("Snooze");}
    else if (code == 0xFFA857) { Serial.println("0");}
    else if (code == 0xFF42BD) { Serial.println("Shuffle");}
    else { Serial.println("Unknown button or bad IR receiver");}
  }
}