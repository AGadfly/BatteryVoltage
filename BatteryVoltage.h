#define BATTERYLOW 4000
#define BATTERYFULL 6000

int batteryVoltage() {
	// The value og the ADC is normally adcValue = Vinput * 1023/Vreference
	// Vreference being normally Vcc~5V
	// Setting the internal bandgap reference voltage as input and the battery voltage as Verference
	// it's possible to calculate the battery voltage like:
	// adcValue = Vinput * 1023/Vreference
	// adcValue = V_BG   * 1023/Vbattery
	// Vbattery = V_BG   * 1023/adcValue
	// Vbattery = 1100 [mV] * 1023 / adcValue
	// Vbattery = 1125300 / adcValue
	//
	// source: https://physudo.blogspot.de/2014/08/spannung-uberwachen-am-arduino-atmega.html
	/*****************************************************************************/
	int adc_low, adc_high;					// temporary variables for ADC results
	long adc_result;						// ADC measurement result

	// setup ADC
	ADMUX &= (0 << ADLAR);					// result right adjusted
	ADMUX |= (1 << REFS0);					// set VCC as Vref for ADC
	ADMUX |= (1 << MUX3) | (1 << MUX2) | (1 << MUX1);		// set bandgap volage (1.1V) as input voltage for ADC
	delay(100);								// wait until voltages setteled
	ADCSRA |= (1 << ADEN);					// activate ADC

	// measure
	ADCSRA |= (1 << ADSC);					// start measurement

	while (bitRead(ADCSRA, ADSC));			// wait until measurement finished

	adc_low = ADCL;							// collect results. Important: ADCL first, then ADCH
	adc_high = ADCH;

	adc_result = (adc_high << 8) | adc_low;	// put ADC result together

	// calc & return batteryVoltage
	int vcc = 1125300L / adc_result;		// calculate battery voltage in mV (1100mV * 1023 = 1125300)
	return vcc;

}