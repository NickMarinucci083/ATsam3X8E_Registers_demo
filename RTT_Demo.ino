/*
RTT_MR = Mode register, the configuration register for the whole RTT peripheral, how it operates
RTT_VR = Value register, the current value of the free-running counter incremented by hardware
RTT_AR = Alarm value register, holds the compare value, and when RTT_AR == RTT_VR, (hardware comparison, not done by CPU) it triggers alarm interupt
RTT_SR = The status register, holds flags such as alarm matched counter value, and counter-related events, but reading it clears these flags
*/

bool alarmHappened = false; //Bool flag to tell main code when interupt fires

void RTT_Handler() { //ATsam3X8E firmware knows to call this function when the alarm interupt of RTT peripheral fires.

  RTT->RTT_SR; //Read status register to clear the flags, which clears the interupt flag

  alarmHappened = true; //Tell main code that interupt fired, alarm went off
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(0xFFFFFFFF); //No serial timeout

  Serial.println("RTT demo");

  pmc_enable_periph_clk(ID_RTT); //Enable the RTT's clock
  NVIC_EnableIRQ(RTT_IRQn); //Allow the RTT interupt to interupt the CPU through Atsam3X8E's NVIC (Nested Vectored Interrupt Controller)

  RTT->RTT_MR = RTT_MR_RTPRES(32768) | RTT_MR_ALMIEN; //Configure the mode register to increment counter every 32768 clock cycles
  //As it uses slow clock, 32768/s, that means it will increment every second, and enable interupt for alarm

  uint32_t currentTime = RTT->RTT_VR; //Get the current value of the free-running counter

  RTT->RTT_AR = currentTime + 5; //Set the alarm value register to the current value of counter, plus 5

  Serial.println("Alarm will go off in 5 seconds");
}


void loop() {
  if (alarmHappened) { //If the alarmHappened is true, because alarm interupt triggered RTT_Handler, it made alarmHappened true:
    alarmHappened = false; //Reset it to avoid reprinting that it went off

    uint32_t now = RTT->RTT_VR; //Get the current value of free-running counter

    Serial.print("ALARM went off at time = ");
    Serial.println(now);

    Serial.println("Input a number and alarm will go off again after that many seconds");
    int number = Serial.parseInt();

    RTT->RTT_AR = now + number; //Set the alarm again to current time plus 3 seconds again, to go off again
    Serial.print("RTT alarm set to off in another ");
    Serial.print(number);
    Serial.println(" seconds");
  }
}