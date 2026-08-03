#include <Mouse.h>

#define xPIN A0
#define yPIN A1

int raw_x = 0;
int raw_y = 0;
int x = 0;
int y = 0;

const int x_callibrate = 1; //My specific joystick was a little off on x axis
const int y_callibrate = 0;

int direction = 0;

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(8); //Lower resolution
  Mouse.begin(); //Give PC time to recognise device
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  raw_x = analogRead(xPIN) + x_callibrate;
  raw_y = analogRead(yPIN) + y_callibrate;


  x = map(raw_x, 0, 255, -10, 10);
  y = map(raw_y, 0, 255, -10, 10);

  if (raw_y < 240) {
    Mouse.move(x, y, 0);
  }
  else {Mouse.click(MOUSE_LEFT); delay(250);} //My joystick shorts Vcc and Y signal together when button is pressed, instead of just having another signal wire.

  delay(10);
}