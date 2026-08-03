/* Timer setup
----------------------------------------------------------------------------------------------------------------------
A timer is a hardware counter that increases its timer value at a fixed clock rate. It starts at 0 and
counts upward until it reaches a value defined by the RC register, at which point it
resets back to 0 and begins counting again. In waveform mode, during the count, the timer can compare its
current value against the RA and RB registers, which act as trigger points where actions
can occur, such as changing an output pin state (TIOA or TIOB timer outputs), generating an interrupt, or triggering
another peripheral. RA and RB do not reset the counter; they only define points within the
counting cycle where something happens. By setting RC, you control the timer period
(frequency), and by setting RA or RB, you can control when outputs change state, allowing
the timer to generate PWM signals or precise timing events automatically without
continuous CPU control.
The arduino DUE board (ATsam3X8E) has 9 timer channels, controlled by 3 seperate timer blocks, TC0, TC1, and TC2
TC0 0, TC0 1, TCO 2 TC1 0, TC1 1, TC1 2, TC2 0, TC2 1, TC2 2, 9 independent channels
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
----------------------------------------------------------------------------------------------------------------------
TC_RC = Called "Register C"

Defines the timer period before it restarts

Example: TC0->TC_CHANNEL[0].TC_RC = 42000;
Means restart the counter after 42000 clock cycles
Therefore with a 42MHz clock, it restarts 1000 times, 1KHz
Therefore, it controls the frequency
----------------------------------------------------------------------------------------------------------------------
TC_RA = Called "Register A"

Defines the compare point for TIOA

Example: TC0->TC_CHANNEL[0].TC_RA = 21000;

Means set TIOA to 21000, so when counter reaches 21000, (hardware comparison, not CPU), do TIOA event.
In this case, the event is to turn timer TIOA I/O output off, to control duty cycle
----------------------------------------------------------------------------------------------------------------------
TC=RB = Called "Register B"

Defines the compare point for TIOB

Example: TCO->TC_CHANNEL[0].TC_RB = xxxxx;

The same thing as TIOA, just another TIO output, called TIOB, you can make the other timer output go on or off
with TIOB getting reached by counter
----------------------------------------------------------------------------------------------------------------------
Example: TC0->TC_CHANNEL[0].TC_CV
Means read the current value of timer 0

Like "uint32_t count = TCO->TC_CHANNEL[0].TC_CV
----------------------------------------------------------------------------------------------------------------------
TC_SR = Status register

Example: TC0->TC_CHANNEL[0].TC_SR = Status of timer channel 0

Reports events and holds flags such as RC compare happened, RA compare happened, or timer overflow
Reading this register can clear some of these flags
Used mainly with interrupts, put in the handler, to clear flags after they triggered the interrupt
----------------------------------------------------------------------------------------------------------------------
*/

/* Connecting timer I/O output to physical pin on board with I/O multiplexers

Using PORTB on pins with DUE board, so controller is PIOB, PIOB->

PIO_PDR = Pin disable register, disables a pin from normal GPIO control of functions, like digitalWrite()

PIO_ABSR = Peripheral A or B select register, selects whether a pin is controlled by peripheral A or peripheral B
Pin 2 on the DUE supports timer output, and it does support

*/
#include <Arduino.h>

void setup() {
  pmc_enable_periph_clk(ID_TC0); //Enable timer block TC0 clock

  TC0->TC_CHANNEL[0].TC_CMR = //Write to the timer TC0 channel 0 control register the following settings:
    TC_CMR_TCCLKS_TIMER_CLOCK1 | //Set it to MAIN clock divided by 2 once, for 42MHz operation
    TC_CMR_WAVE | //Set it to waveform generation mode, to generate PWM, not measure a pulse with capture mode
    TC_CMR_WAVSEL_UP_RC | //Count mode, here make it is set to count up to RC setting, then reset to 0
    TC_CMR_ACPA_CLEAR | //When the counter reaches RA value, drive the timer TIOA I/O output LOW
    TC_CMR_ACPC_SET; //When the counter reaches RC value (the value it resets at), drive TIOA I/O output HIGH

  TC0->TC_CHANNEL[0].TC_RC = 42000; //Set restart value (RC) to 42000, which with a 42MHz clock, 1000 times a second

  TC0->TC_CHANNEL[0].TC_RA = 21000; //Set TIOA value to 21000, when timer reaches THIS value, do TIOA event, of turning I/O pin off

  TC0->TC_CHANNEL[0].TC_CCR = //The TC0 timer control register, controlling channel 0
    TC_CCR_SWTRG | //Reset the counter to whatever arbitrary value it powered up on
    TC_CCR_CLKEN;  //Enable the clock, so it starts running

  // Disable GPIO control of PB25
  PIOB->PIO_PDR |= 0b00000010000000000000000000000000; //Set bit 25, and bit 25 is disconnected from normal GPIO control

  // Select Peripheral B to control PB25 (TIOA0, timer block 0, timer output channel 0, TIOA I/O)
  PIOB->PIO_ABSR |= 0b00000010000000000000000000000000; //Set bit 25, meaning select peripheral B
  
  //PB25 (Port B, No.25) is pin 2 on the Arduino DUE board, so timer output is on pin 2
}


void loop() {
}