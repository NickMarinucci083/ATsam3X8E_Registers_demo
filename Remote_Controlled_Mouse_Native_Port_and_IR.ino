#include <Mouse.h>
#include <Keyboard.h>

#define IR_PIN 13

bool speed = false;

void setup() {
  pinMode(IR_PIN, INPUT);
  //SerialUSB.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  Mouse.begin();
  Keyboard.begin();
  delay(1000); //Give PC time to recognise device acting as a standard USB mouse and Keyboard
  openOSK(); //Now user can type remotely too
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

void openOSK() {
  // Press Ctrl + Win + O
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_GUI);   // Windows key
  Keyboard.press('o');

  delay(100);

  Keyboard.releaseAll();
}

void loop() {
  uint32_t code = readIR(); //readIR returns 32 bit code

  if (code != 0) { //Ignore if no code detected

    if (code == 0xFFE817) {
      //SerialUSB.println("Right click");
      Mouse.click(MOUSE_RIGHT);
    }

    else if (code == 0xFF48B7) {
      //SerialUSB.println("Up");
      if (speed == false) {Mouse.move(0, -5, 0);}
      else {Mouse.move(0, -10, 0);}
    }

    else if (code == 0xFF02FD) {
      //SerialUSB.println("Left");
      if (speed == false) {Mouse.move(-5, 0, 0);}
      else {Mouse.move(-10, 0, 0);}
    }

    else if (code == 0xFF32CD) {
      //SerialUSB.println("Left click");
      Mouse.click(MOUSE_LEFT);
    }

    else if (code == 0xFF20DF) {
      //SerialUSB.println("Right");
      if (speed == false) {Mouse.move(5, 0, 0);}
      else {Mouse.move(10, 0, 0);}
    }

    else if (code == 0xFF7887) {
      //SerialUSB.println("Down");
      if (speed == false) {Mouse.move(0, 5, 0);}
      else {Mouse.move(0, 10, 0);}
    }

    else if (code == 0xFF28D7) {
      //SerialUSB.println("Scroll down");
      if (speed == false) {Mouse.move(0, 0, -1);}
      else {Mouse.move(0, 0, -5);}
    }

    else if (code == 0xFFF00F) {
      //SerialUSB.println("Scroll up");
      if (speed == false) {Mouse.move(0, 0, 1);}
      else {Mouse.move(0, 0, 5);}
    }

    else if (code == 0xFF609F) {
      //SerialUSB.println("Speed toggled");
      speed = !speed;
      code = 0;
      delay(100);
    }

    else {
      //SerialUSB.println("Unknown button or bad IR receiver");
    }
  }
}