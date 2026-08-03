#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Software rest demo");

  // RAW reset status register
  uint32_t resetReason = (RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> 8; //The Reset controller register of Atsam3X8E,
  // the part that holds the reason of reset, then shift them down 8 bits from 32 bit register to bits 0-2, instead of 8-10
  Serial.print("Reset reason code: ");
  Serial.println(resetReason);

  Serial.print("Reset reason code meaning: ");
  if (resetReason == 0) {Serial.println("Power on reset");}
  else if (resetReason == 1) {Serial.println("Backup domain reset");}
  else if (resetReason == 2) {Serial.println("Watchdog reset");}
  else if (resetReason == 3) {Serial.println("Software triggered reset");}
  else if (resetReason == 4) {Serial.println("Physical RESET pin on chip was activated");}

}

void loop() {
  delay(5000);

  Serial.println("Rebooting via NVIC_SystemReset()...");

  delay(500);

  NVIC_SystemReset();

  while (1) {
    Serial.println("If you see this message your board didn't reset, its after NVIC_SystemReset() is called");
  }
}