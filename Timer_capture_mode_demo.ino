/*
A timer is a hardware counter that increases its timer value at a fixed clock rate. It starts at 0 and
counts upward continuously. In capture mode, RA and RB are storage registers that automatically
save the current timer value when a configured event occurs on the TIOA or TIOB input. These
captured values act as timestamps, allowing the time between input edges, pulse widths, or
frequencies to be calculated by the CPU. RA and RB no longer define trigger points for output changes, they
only store the timer value when an input event happens. By setting the timer clock speed, you
control the timing resolution, allowing the timer to accurately measure external signals without
continuous CPU work, CPU only calculates time when timer captures pulses/signals by itself.
----------------------------------------------------------------------------------------------------------------------
TC_CCR = Controls the timer, starting, and stopping. Accessed with: TC0->TC_CHANNEL[<channel number 0-8>].TC_CCR
TC_CCR_CLKEN = Enable timer clock
TC_CCR_CLKDIS = Disable timer clock
TC_CCR_SWTRG = Reset counter

Used like:

TC0->TC_CHANNEL[0].TC_CCR = Means "On timer channel 0"
  TC_CCR_SWTRG | Reset the counter
  TC_CCR_CLKEN;  Enable the clock (start counting)
----------------------------------------------------------------------------------------------------------------------
TC_CMR = Channel mode register, main config register
Accessed with: TC0->TC_CHANNEL[0].TC_CMR (Here it controls timer 0)

Controls the clock source, Capture/Waveform mode, Counting mode and TIOA/TIOB behaviour

For waveform mode, you write something like:
  TC_CMR_TCCLKS_TIMER_CLOCK1 | <- Clock setting
  TC_CMR_WAVE | <- Wave mode
  TC_CMR_WAVSEL_UP_RC | <- Wave counting mode
  TC_CMR_ACPA_CLEAR | <- TIOA events on counter value reaching something
  TC_CMR_ACPC_SET; <- TIOA events on counter value reaching something

For capture mode, you write something like this:
  TC_CMR_TCCLKS_TIMER_CLOCK1 | <- Clock setting
  TC_CMR_LDRA_RISING | <- Event when TIOA goes HIGH
  TC_CMR_LDRB_FALLING; <- Event when TIOB goes LOW
----------------------------------------------------------------------------------------------------------------------
TC_RC = Called "Register C"

Defines the timer period before it restarts

Example: TC0->TC_CHANNEL[0].TC_RC = 42000;
Defines the counter limit in modes where the timer uses RC as a reset value. In waveform mode with WAVSEL_UP_RC,
the counter resets when it reaches RC. In capture mode, RC can still limit the counter range, but it is not 
normally used for measuring input signals.
----------------------------------------------------------------------------------------------------------------------
TC_RA = Called "Register A"

Holds counter value when TIO event happens

Example: This happens automatically, when you write "TC_CMR_LDRA_RISING" in config, it auto-stores counter value
when TIOA I/O goes HIGH
----------------------------------------------------------------------------------------------------------------------
TC=RB = Called "Register B"

Holds counter value when TIO event happens

Example: Again, configured in configuration register, "TC_CMR_LDRB_FALLING" means it auto-stores counter value
when TIO event happens, here when TIO goes LOW
----------------------------------------------------------------------------------------------------------------------
Example: TC0->TC_CHANNEL[0].TC_CV
Means read the current value of timer 0

Like "uint32_t count = TCO->TC_CHANNEL[0].TC_CV
----------------------------------------------------------------------------------------------------------------------
TC_SR = Status register

Example: TC0->TC_CHANNEL[0].TC_SR = Status of timer channel 0

Reports events and holds flags such as RC compare happened, RA or RB capture happened, or timer overflow
Reading this register can clear some of these flags
Used mainly with interrupts, put in the handler, to clear flags after they triggered the interrupt
----------------------------------------------------------------------------------------------------------------------
TC_SR_LDRAS/TC_SR_LDRBS = Status flags that are true when RA or RB registers receive a value

Uses so that the CPU can get the values or RA or RB, only when they have a value in them, to avoid reading
blank registers.
Reading these register bits clears the flags, so that CPU doesnt read them over and over again, it only reads them
when regisers get updated again, setting the flags high, which means new data avaliable
*/

#include <Arduino.h>

uint32_t startTime;
uint32_t status;
uint32_t endTime;
uint32_t pulseWidth;

void setup() {

  pmc_enable_periph_clk(ID_TC0); //Turn on timer block 0 clock

  TC0->TC_CHANNEL[0].TC_CMR = //Mode regiser
    TC_CMR_TCCLKS_TIMER_CLOCK1 | //Divide 84MHz main clock once, so 42MHz clock for timer
    TC_CMR_LDRA_RISING | //Put timer value into RA regiser, when TIO goes HIGH
    TC_CMR_LDRB_FALLING; //Put the timer value into RB register, when TIO goes LOW

  TC0->TC_CHANNEL[0].TC_CCR = //Control register
    TC_CCR_SWTRG | //Reset counter to whatever arbitrary value it got when powered up
    TC_CCR_CLKEN;  //Enable channel 0's clock to start up counter

  // Disable GPIO control of PB25 (D2)
  PIOB->PIO_PDR |= (1 << 25);

  // Select Peripheral B (TIOA0)
  PIOB->PIO_ABSR |= (1 << 25);

  Serial.begin(115200);
  pinMode(3, OUTPUT);

//For timer capture mode, always use TIOA, connect your pulse source to TIOA, which is internally now connected
//to pin 2 on the Arduino DUE board. In capture mode, only TIOA can be used to capture pulses/signals/etc
}


void loop() {
  
  digitalWrite(3, LOW);
  delay(2);
  digitalWrite(3, HIGH);
  delay(10);

  status = TC0->TC_CHANNEL[0].TC_SR; //Get status register (need to read RA and RB ready flags)


  if (status & TC_SR_LDRAS) { //Use the loaded register A status bit mask, to get the 1 bit

    startTime = TC0->TC_CHANNEL[0].TC_RA; //If data is in it, get it and put into start time
  }


  if (status & TC_SR_LDRBS) { //Same thing to get the other register's data

    endTime = TC0->TC_CHANNEL[0].TC_RB; //Put it into endTime value

    pulseWidth = endTime - startTime; //Calculate pulse width, we now have both values

    Serial.print("Pulse width: "); //Print it
    Serial.print(pulseWidth);
    Serial.println(" timer ticks");
  }
}