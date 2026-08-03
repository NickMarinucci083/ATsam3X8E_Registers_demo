/*
__WFE stands for "wait for event"
When the CPU execeutes this line of code, it stops executing instructions
and enters the sleep state selected with SCB and PMC registers
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  RTTinit();
  pinMode(LED_BUILTIN, OUTPUT);

  SUPC->SUPC_WUMR |= SUPC_WUMR_RTTEN; //Enable the RTT alarm to wake the CPU from backup
  //Normally, its through the NVIC, but NVIC is shut down, as 1.8V core power is lost
  //Therefore, you must configure the interrupt to go through the supply controller register
  //which is still powered on, of which this time, instead of going to a interrupt pin
  //on a powered down CPU, it goes to supply controller, to turn back on 1.8V CPU power
  //and peform a system reset, which gets board out of backup mode, and restores CPU power

  Serial.println("B̲A̲C̲K̲U̲P̲ M̲O̲D̲E̲ D̲E̲M̲O̲");
  Serial.println();
  Serial.println("The LED on pin 13 will toggle when CPU is awake, and doing work.");
  Serial.println("When CPU is asleep, LED will not toggle, as CPU is not awake execeuting digitalWrite()");
  Serial.println();

  Serial.println("Setting RTT alarm to 5 seconds ahead, alarm interupt will wake board");
  uint32_t currentTime = RTT->RTT_VR; //Get the current value of RTT
  RTT->RTT_AR = currentTime + 5; //Set the alarm value register to the current value of counter, plus 5
  
  Serial.println("After board wake, board will restart, as CPU will lose power. loop() will never run.");
  Serial.println();
  Serial.print("Going into backup mode...");
  Serial.println();

  Serial.end(); //Turn off serial, so it doesn't generate interupts to wake CPU prematurely
  NVIC_ClearPendingIRQ(RTT_IRQn); //Clear any pending interupts that may have already fired
  
  backup();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

/*
Backup mode: The Supply Controller disables the internal 1.8 V regulator, removing power from the CPU core,
SRAM, Flash, and normal peripherals. Only the backup domain remains powered, including RTC, RTT, backup registers,
Slow Clock, and wake-up circuitry. A wake-up event causes a complete reset; execution does not resume after __WFE().

The CPU cannot resume execution because core power is removed. Wake-up sources include WKUP pins, RTC alarm, RTT alarm,
supply monitor, and firmware wake-up input. These events cause a full reset and restart the program once CPU restarts as power
is turned back on SRAM and register configurations are lost, as power was lost then restored, program restarts from setup().
*/

void backup() {
  /*

  Mode         |SLEEPDEEP bit    |PMC LPM bit    |Instruction   |CPU has power 
  -------------|-----------------|---------------|--------------|--------------
  Sleep mode   |0                |0              |WFI or WFE    |Yes           
  Wait mode    |0                |1              |WFE           |Yes           
  Backup mode  |1                |0              |WFE           |No            <-- Put CPU into the deep sleep mode, and then the supply controller will turn off the CPU's 1.8V power, and entire core domain will lose power. Only a bit set in SUPC->SUPC_WUMR enabling a interrupt in backup domain power areas, can turn CPU's power back on, causing a system reset.

  */

    // Enable deep sleep bit
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Request 1.8V regulator shutdown, once CPU is in deep sleep
    SUPC->SUPC_CR = SUPC_CR_KEY(0xA5) | SUPC_CR_VROFF;

    // Wait for event (interrupt) to wake board, of which, once CPU has power back, board will restart
    __WFE();
}



//RTT alarm stuff
void RTTinit() {
  pmc_enable_periph_clk(ID_RTT); //Enable the RTT's clock
  NVIC_EnableIRQ(RTT_IRQn); //Allow the RTT interupt to interupt the CPU through Atsam3X8E's NVIC (Nested Vectored Interrupt Controller)

  RTT->RTT_MR = RTT_MR_RTPRES(32768) | RTT_MR_ALMIEN; //Configure the mode register to increment counter every 32768 clock cycles
  //As it uses slow clock, 32768/s, that means it will increment every second, and enable interupt for alarm
  RTT->RTT_SR; // clear any pending RTT status flags
}

void RTT_Handler() { //This code runs on RTT interrupt

  RTT->RTT_SR; //Read status register to clear the flags, which clears the interupt flag

  Serial.begin(115200); //Turn serial back on once alarm fires, and CPU wakes up

  Serial.println("RTT Alarm fired, this interupt should wake CPU, and now it should be in loop, toggling LED");
}