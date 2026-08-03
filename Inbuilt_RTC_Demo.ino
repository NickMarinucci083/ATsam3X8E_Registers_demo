/*
RTC_TIMR = The RTC register that holds its current time, write to it to set time
RTC_CALR = The RTC register that holds its current date, write to it to set date
These registers hold multiple info in the 1 32 bit register, so to extract just minutes, from whole TIMR/CALR
register, extract those bits with a mask and shift them down enough the right amount based on their position

RTC_SR = RTC status register, holds flags such as RTC entered update mode, it takes time
RTC_CR = The main RTC control register, used here to put it into time setting mode, then enable it after time is set, etc
*/

#include <Arduino.h>

void setup() {

  Serial.begin(115200);
  while(!Serial);

  pmc_enable_periph_clk(ID_RTC);

  Serial.println("RTC demo");
  Serial.println("Setting time...");


  RTC->RTC_CR = RTC_CR_UPDTIM | RTC_CR_UPDCAL; //Enter time and date update mode with control register


  // Wait until RTC enters update mode, it takes time. When it does, RTC_SR_ACKUPD goes true.
  while ((RTC->RTC_SR & RTC_SR_ACKUPD) == 0) {}

  //Set time
  RTC->RTC_TIMR = //BCD values, easy to use
      RTC_TIMR_HOUR(0x2) | //Hour
      RTC_TIMR_MIN(0x41)  | //Minute
      RTC_TIMR_SEC(0x05);   //Second


  //Set date
  RTC->RTC_CALR = //Again, easy BCD values
      RTC_CALR_CENT(0x20) |   //Century
      RTC_CALR_YEAR(0x26) |   //Year
      RTC_CALR_MONTH(0x07) |  //Month
      RTC_CALR_DATE(0x23) |   //Date
      RTC_CALR_DAY(4);        //Day, 1-7


  //Exit update mode, make entire control register off.
  RTC->RTC_CR = 0;


  Serial.println("Time/date set.");
  Serial.println("Printing RTC every second...");
}


void loop() {

  uint32_t timr = RTC->RTC_TIMR;
  uint32_t calr = RTC->RTC_CALR;


  uint8_t seconds = (timr & RTC_TIMR_SEC_Msk) >> RTC_TIMR_SEC_Pos;
  uint8_t minutes = (timr & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
  uint8_t hours = (timr & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
  uint8_t day = (calr & RTC_CALR_DAY_Msk) >> RTC_CALR_DAY_Pos;
  uint8_t date = (calr & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos;
  uint8_t month = (calr & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos;
  uint8_t year = (calr & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos;
  uint8_t century = (calr & RTC_CALR_CENT_Msk) >> RTC_CALR_CENT_Pos;


  Serial.print(hours, HEX);
  Serial.print(":");
  Serial.print(minutes, HEX);
  Serial.print(":");
  Serial.print(seconds, HEX);


  Serial.print(", ");

  switch (day) {
    case 1: Serial.print("Monday, ");  break;
    case 2: Serial.print("Tuesday, ");  break;
    case 3: Serial.print("Wednesday, ");  break;
    case 4: Serial.print("Thursday, ");  break;
    case 5: Serial.print("Friday, "); break;
    case 6: Serial.print("Saturday, "); break;
    case 7: Serial.print("Sunday, "); break;
  }

  Serial.print(date, HEX);
  Serial.print("/");

  Serial.print(month, HEX);
  Serial.print("/");

  Serial.print(century, HEX);


  if (year < 10) { Serial.print("0"); } //Correct formatting
  Serial.print(year, HEX);

  Serial.println();

  delay(1000);
}