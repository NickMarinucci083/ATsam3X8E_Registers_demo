/*
TRNG_CR = The TRNG Control Register, starts and enables the hardware RNG
TRNG_ISR = Interrupt Status Register, contains the DATRDY flag that indicates when a new number is ready
TRNG_ODATA = The TNRG output data register, holds the randomly genererated number
*/

void setup() {
  Serial.begin(115200);

  pmc_enable_periph_clk(ID_TRNG); //Enable the TRNG's clock
  TRNG->TRNG_CR = TRNG_CR_KEY('RNG') | TRNG_CR_ENABLE;
  //Unlocks the control register with the 'RNG' key, then enables TRNG to start running

  (void)TRUErandom(); //Throw out 1st couple readings, TRNG have the need
  (void)TRUErandom(); //to "warm up" to generate true, read random numbers.
}

unsigned long TRUErandom() {
  while (!TRNG_ISR_DATRDY); //While the DATRDY data ready flag is false, wait for it it to be true, and a number is ready
  return TRNG->TRNG_ODATA; //When it is ready, return the output data, the randomly generated number
}

void loop() {
  Serial.println(TRUErandom());
  delay(500);
}