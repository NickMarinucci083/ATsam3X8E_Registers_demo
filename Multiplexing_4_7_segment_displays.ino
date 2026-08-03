#define NO1 29
#define NO2 27
#define NO3 25
#define NO4 23

#define A 51
#define B 53
#define C 35
#define D 37
#define E 39
#define F 49
#define G 47

uint16_t value;

void setup() { 
  startupCall();
  Serial.begin(115200);
  Serial.setTimeout(1); //Shortest timeout so display keeps being multiplexed, or else Serial.parseInt() blocks
}

void loop() {
  if (Serial.available() > 0) {
    uint16_t newValue = Serial.parseInt();

    if (newValue != 0) { //Only accept value if its not 0, serial returns 0 by default
      value = newValue;
    }
  }

  display(value);
}

void display(uint16_t number) {
  // Limit to 0-9999, for a 4 digit display
  number = constrain(number, 0, 9999);

  character((number / 1000) % 10);  // Thousands
  selectNumeral(1);
  delayMicroseconds(100);
  selectNumeral(0);

  character((number / 100) % 10);   // Hundreds
  selectNumeral(2);
  delayMicroseconds(100);
  selectNumeral(0);

  character((number / 10) % 10);    // Tens
  selectNumeral(3);
  delayMicroseconds(100);
  selectNumeral(0);

  character(number % 10);           // Ones
  selectNumeral(4);
  delayMicroseconds(100);
  selectNumeral(0);
}

void character(uint8_t character) {
  switch (character) {
    case 0 : setSegments(1,1,1,1,1,1,0); break;
    case 1 : setSegments(0,1,1,0,0,0,0); break;
    case 2 : setSegments(1,1,0,1,1,0,1); break;
    case 3 : setSegments(1,1,1,1,0,0,1); break;
    case 4 : setSegments(0,1,1,0,0,1,1); break;
    case 5 : setSegments(1,0,1,1,0,1,1); break;
    case 6 : setSegments(1,0,1,1,1,1,1); break;
    case 7 : setSegments(1,1,1,0,0,0,0); break;
    case 8 : setSegments(1,1,1,1,1,1,1); break;
    case 9 : setSegments(1,1,1,1,0,1,1); break;
  }
}

void startupCall() {
  pinMode(NO1, OUTPUT);
  pinMode(NO2, OUTPUT);
  pinMode(NO3, OUTPUT);
  pinMode(NO4, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  digitalWrite(NO1, HIGH);
  digitalWrite(NO2, HIGH);
  digitalWrite(NO3, HIGH);
  digitalWrite(NO4, HIGH);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void selectNumeral(int position) {
  if (position == 1) { digitalWrite(NO1, LOW); } 

  else if (position == 2) { digitalWrite(NO2, LOW);} 

  else if (position == 3) { digitalWrite(NO3, LOW);}

  else if (position == 4) {digitalWrite(NO4, LOW);}

  else {
    digitalWrite(NO1, HIGH);
    digitalWrite(NO2, HIGH);
    digitalWrite(NO3, HIGH);
    digitalWrite(NO4, HIGH);
  }
}

void setSegments(bool a, bool b, bool c, bool d, bool e, bool f, bool g) {
  if (a == true) {digitalWrite(A, HIGH);} else {digitalWrite(A, LOW);}
  if (b == true) {digitalWrite(B, HIGH);} else {digitalWrite(B, LOW);}
  if (c == true) {digitalWrite(C, HIGH);} else {digitalWrite(C, LOW);}
  if (d == true) {digitalWrite(D, HIGH);} else {digitalWrite(D, LOW);}
  if (e == true) {digitalWrite(E, HIGH);} else {digitalWrite(E, LOW);}
  if (f == true) {digitalWrite(F, HIGH);} else {digitalWrite(F, LOW);}
  if (g == true) {digitalWrite(G, HIGH);} else {digitalWrite(G, LOW);}
}