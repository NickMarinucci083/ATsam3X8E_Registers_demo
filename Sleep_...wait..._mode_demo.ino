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

  Serial.println("W̲A̲I̲T̲ M̲O̲D̲E̲ D̲E̲M̲O̲");
  Serial.println();
  Serial.println("The LED on pin 13 will toggle when CPU is awake, and doing work.");
  Serial.println("When CPU is asleep, LED will not toggle, as CPU is not awake execeuting digitalWrite()");
  Serial.println();

  Serial.println("Setting RTT alarm to 5 seconds ahead, alarm interupt will wake board");
  uint32_t currentTime = RTT->RTT_VR; //Get the current value of RTT
  RTT->RTT_AR = currentTime + 5; //Set the alarm value register to the current value of counter, plus 5

  Serial.print("Going into wait mode...");
  Serial.println();

  Serial.end(); //Turn off serial, so it doesn't generate interupts to wake CPU prematurely
  NVIC_ClearPendingIRQ(RTT_IRQn); //Clear any pending interupts that may have already fired
  
  wait();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

/*
Wait mode: The Cortex-M3 enters Deep Sleep, power is to CPU is still on, preserving SRAM, registers,
and peripheral configuration. The main clock, PLLs, Flash interface, and most peripheral clocks are stopped to reduce power.
The Slow Clock and wake-up logic remain active. An interrupt can wake the CPU, and execution resumes after __WFI().

The CPU can be awakened by enabled wake-up sources such as external interrupts, RTC/RTT alarms, peripheral interrupts,
or Supply Controller wake events. SRAM and CPU state are retained, allowing execution to continue after __WFE().
*/
void wait() {
  /*

  Mode         |SLEEPDEEP bit    |PMC LPM bit    |Instruction   |CPU has power 
  -------------|-----------------|---------------|--------------|--------------
  Sleep mode   |0                |0              |WFI or WFE    |Yes           
  Wait mode    |0                |1              |WFE           |Yes           <-- Not deep sleep, but the power managememt is going to go put the CPU into a low power mode, called wait. It will wake CPU, and restore full power/functionality when a event (interrupt) happens
  Backup mode  |1                |0              |WFE           |No            

  */

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //Turn off system tick interrupt

    // Do NOT set SLEEPDEEP, SEEPSLEEP is only used for backup, before CPU loses 1.8V power
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    // Select Wait mode, with the power management control register
    PMC->PMC_FSMR |= PMC_FSMR_LPM;

    __WFE(); // Wait mode uses WFE, not WFI. Wait for event, instead of wait for interrupt

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // When woken up, enable system tick again
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