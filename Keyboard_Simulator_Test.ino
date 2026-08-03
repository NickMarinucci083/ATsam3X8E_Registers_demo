#include <Keyboard.h> //keyboard emulation over native USB

void setup() {
  // put your setup code here, to run once:

  Keyboard.begin();
  delay(800); //Give PC time to recognise device

  openNotepad();
  delay(1000); //Give notepad time to load

  Keyboard.print("Your Arduino DUE board just acted as a keyboard to your OS and typed this automated message");
  delay(1000);
  Keyboard.print("!");
}

void loop() {}

void openNotepad() {
  // Press Windows key + R
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');

  delay(100);

  Keyboard.releaseAll();
  delay(300);

  Keyboard.println("Notepad.exe"); //Run notepad
}
