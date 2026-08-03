void setup() {
  Serial.begin(115200);
  CPUcoreTempInit();
}

void CPUcoreTempInit() {
  ADC->ADC_ACR |= ADC_ACR_TSON;
  delayMicroseconds(100);
  analogReadResolution(12);
  ADC->ADC_CHER = ADC_CHER_CH15;
}

float CPUcoreTempGet() {
  if (ADC->ADC_CHSR & ADC_CHER_CH15) { //Only meausure temprature if CPUcoreTempInit was called, and CH15 was set up
    //Start a convertion with the ADC control register
    ADC->ADC_CR = ADC_CR_START;
    //Wait for channel 15 convertion to complete, by reading the convertion complete flag in ADC_ISR status register
    while ((ADC->ADC_ISR & ADC_ISR_EOC15) == false);
    //Read result from ADC channel data register, channel 15
    uint16_t raw = ADC->ADC_CDR[15];
    //ChatGPT did the math work here to calculate the temprature in degrees celsius, from raw ADC reading
    float celsius = ((((raw * 3300.0) / 4095.0) - 800.0) * 0.37736) + 25.5;
    //Return reading in degrees celsius (it's a float)
    return celsius;
  }
  else { return 0; }
}


void loop() {
  Serial.print("CPU core temp: ");
  Serial.print(CPUcoreTempGet());
  Serial.println("°C");
  delay(5000);
}