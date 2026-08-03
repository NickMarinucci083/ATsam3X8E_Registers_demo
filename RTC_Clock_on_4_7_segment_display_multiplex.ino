//NOTE: THis clock will not actually be accurate as the RTC uses the inbuilt R/C clock inside chip, not dedicated 32.768KHz
//crystal, therefore timing will be off, to actually make this a working long time clock. write to the supply controllers
//of the Atsam3X8E chip to use the crystal, not inbuilt R/C timer. Default is inbuilt timer, DUE clones don't have crystal.

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

#define colon 52

uint16_t colonBlink = 0;

void setup() { 
  displayInit();

  pmc_enable_periph_clk(ID_RTC);

  setRTC(0x19, 0x18); //Hours in BCD, minutes in BCD

  pinMode(colon, OUTPUT);
  digitalWrite(colon, LOW);
}

void loop() {
  uint32_t timr = RTC->RTC_TIMR;

  uint8_t hours = BCDtoDec((timr & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos); //Convert RTC values to decimal on the fly
  uint8_t minutes = BCDtoDec((timr & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos);

  uint16_t numberstring = hours * 100 + minutes; //Merge numbers together mathematically

  display(numberstring);
  
  colonBlink++;
  if(colonBlink >= 2500) { //Blink colon every 2500 loop cycles
    colonBlink = 0;
    digitalWrite(colon, !digitalRead(colon));
  }
}


uint8_t BCDtoDec(uint8_t bcd) { //Convert a BCD number into decimal number
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
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

void displayInit() {
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


void setRTC(uint8_t hours, uint8_t minutes) {
    RTC->RTC_CR = RTC_CR_UPDTIM; //Enter time and date update mode with control register, time needed only


  // Wait until RTC enters update mode, it takes time. When it does, RTC_SR_ACKUPD goes true.
  while ((RTC->RTC_SR & RTC_SR_ACKUPD) == 0) {}

  //Set time
  RTC->RTC_TIMR = //BCD values, easy to use
      RTC_TIMR_HOUR(hours) | //Hour
      RTC_TIMR_MIN(minutes)  | //Minute
      RTC_TIMR_SEC(0x00);   //Second


  //Exit update mode, make entire control register off.
  RTC->RTC_CR = 0;
}