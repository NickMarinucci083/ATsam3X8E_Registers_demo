void setPLL(int MULA, int DIVA) {

  /*
  PMC (Power management control) controls system clock generation and distribution. All related registers are accessed via PMC->.
  CKGR_PLLAR = Configures PLLA (Phase-Locked Loop A), which uses MAINCK (12 MHz) as its input reference and generates a multiplied/divided high-speed clock
  PMC_MCKR = Controls the Master Clock (MCK) source for the CPU and most peripherals. Available sources are MAIN clock of 12MHz, slow 32.768KHz, or PLLA clock
  PMC_SR = Status register, holds flags such as master clock ready and PLLA locked.
  */

  //--------------------------------------------------
  // 1. Switch CPU clock to MAIN CLOCK (12 MHz)
  //--------------------------------------------------
  uint32_t reg = PMC->PMC_MCKR;   //Read current register value
  reg &= ~PMC_MCKR_CSS_Msk;       //Clear only the clock source bits with clock source bits bitmask, this register holds other stuff
  reg |= PMC_MCKR_CSS_MAIN_CLK;   //Set clock source to MAIN clock, 12MHz
  PMC->PMC_MCKR = reg;            //Write it back

  while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0); //Wait until MCKRDY becomes true, when it does, that means clock source for chip has changed fully
  //The & symbol is to get the MCKRDY bit only, from the whole 32 bit PMC_SR register

  //--------------------------------------------------
  // 2. Reprogram PLLA, redefine the multipler and divider
  // Example: 12 MHz × (6+1) / 1 = 84 MHz. Origional clock, but change MULA and DIVA to change clock speed with formula
  //--------------------------------------------------
  PMC->CKGR_PLLAR = //PLLA config register
    CKGR_PLLAR_ONE //This bit the register must always be written a "1" by mentioning it here, to enable PLLA to accept the write
  | CKGR_PLLAR_MULA(MULA) //Multipler
  | CKGR_PLLAR_DIVA(DIVA) //Divider
  | CKGR_PLLAR_PLLACOUNT(63); //Wait 63 SLCK cycles before it starts checking if PLLA has locked

  //--------------------------------------------------
  // 3. Wait for PLLA to lock
  //--------------------------------------------------
  while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0); //Wait until LOCKA status bit in PMC_SR is set, which indicates PLLA has stabilised
  //Again, use the & symbol to get the specifig LOCKA bit from the PMC_SR register

  //--------------------------------------------------
  // 4. Switch CPU back to PLLA
  //--------------------------------------------------
  reg = PMC->PMC_MCKR; //get MCKR
  reg &= ~PMC_MCKR_CSS_Msk; //Clear clock source bits again with clock source bitmask
  reg |= PMC_MCKR_CSS_PLLA_CLK; //Put clock source bits back to PLLA again
  PMC->PMC_MCKR = reg; //Write value back to register

  while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0); //Wait until MCKRDY is true again, clock switches fully

  //---------------------------------------------------
  //6. Update system clock variables so timing is correct
  //---------------------------------------------------
  SystemCoreClockUpdate(); //Update global clock speed variable
  SysTick_Config(SystemCoreClock / 1000); //Also the system tick
}

uint32_t getPLLclock() {
    uint32_t pllar = PMC->CKGR_PLLAR; //Get PLLA config register (pllar)

    uint32_t mula = (pllar & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos; //Read MULA and DIVA registers
    uint32_t diva = (pllar & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos;

    // DIVA = 0 means PLLA is disabled
    if (diva == 0)
        return 0;

    uint32_t pll_clock = 12000000 * (mula + 1) / diva; //Calculate clock from config registers, return in MHz

    return pll_clock; //in Hz, not MHz or KHz
}


void setPrescaler(uint8_t pres) {
    uint32_t reg;

    switch (pres) {
      case 1:  reg = PMC_MCKR_PRES_CLK_1;  break;
      case 2:  reg = PMC_MCKR_PRES_CLK_2;  break;
      case 3:  reg = PMC_MCKR_PRES_CLK_3;  break;
      case 4:  reg = PMC_MCKR_PRES_CLK_4;  break;
      case 8:  reg = PMC_MCKR_PRES_CLK_8;  break;
      case 16: reg = PMC_MCKR_PRES_CLK_16; break;
      case 32: reg = PMC_MCKR_PRES_CLK_32; break;
      case 64: reg = PMC_MCKR_PRES_CLK_64; break;
      default: reg = PMC_MCKR_PRES_CLK_1;  break; //Safe fallback
    }

    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | reg;

    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0); //Wait until prescaler changes sucessfully with master clock ready flag
}

uint32_t getPrescaler() {
  uint32_t pres_bits = (PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos; //Read prescaler config bits in power controller

  switch(pres_bits) { //Convert 0-6 to a binary power of dividing
    case 0: return 1;
    case 1: return 2;
    case 2: return 4;
    case 3: return 8;
    case 4: return 16;
    case 5: return 32;
    case 6: return 64;
  }
}


void setClockSource(char source) { //s=slow clock, m=main clock, p=PLLA clock
    uint32_t reg;

    switch (source) {
        case 's': reg = PMC_MCKR_CSS_SLOW_CLK; break;
        case 'm': reg = PMC_MCKR_CSS_MAIN_CLK; break;
        case 'p': reg = PMC_MCKR_CSS_PLLA_CLK; break;
        default: return; // Invalid source, get out of this function without changing anything
    }

    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | reg;

    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0); //Wait until master clock ready
}

uint8_t getClockSource() {
  uint32_t source = PMC->PMC_MCKR & PMC_MCKR_CSS_Msk;

  if (source == PMC_MCKR_CSS_SLOW_CLK){
    return 1;
  }
  else if (source == PMC_MCKR_CSS_MAIN_CLK) {
    return 2;
  }
  else if (source == PMC_MCKR_CSS_PLLA_CLK) {
    return 3;
  }
}

void updateTiming() {
  SystemCoreClockUpdate(); //Update system core clock
  SysTick_Config(SystemCoreClock / 1000); //As well as system tick interrupt
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  setPLL(13, 1); //Multiplier for PLL, Divider for PLL
  setPrescaler(2);
  setClockSource('p'); //PLL

  updateTiming(); //Update arduino timing system tick interrupts, so millis() and delay() work properly

  Serial.begin(115200); //Start serial AFTER clock stuff has changed, if you change clock after serial start, baud rates wont work


  Serial.print("Clock source: ");

  if (getClockSource() == 1) {Serial.print("SLCLK = 32.768KHz");}
  if (getClockSource() == 2) {Serial.print("MAINCLK = 12MHz");}
  if (getClockSource() == 3) {
    Serial.print("PLL = ");
    Serial.print(getPLLclock());
    Serial.print("Hz");
  }

  Serial.print(" | Prescaler: ");
  Serial.print(getPrescaler());
  Serial.print(" | Clock speed: ");
  Serial.print(SystemCoreClock);
  Serial.print(" Hz / ");
  Serial.print((float)SystemCoreClock / 1000.0, 3); //Floats into serial
  Serial.print(" kHz / ");
  Serial.print((float)SystemCoreClock / 1000000.0, 3);
  Serial.print(" MHz");

  Serial.println();
  Serial.println();
}

void loop() {
  Serial.println("Did you see this message through serial, even if clock speed is changed?");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("Is the LED on pin 13 toggling?");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}