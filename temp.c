#include <msp430.h>
#include "temp.h"

/*
 * Sample period must be >30 us.
 * Vtemp = 0.00355(TEMP C) + 0.986
 * TEMP C = (Vtemp - 0.968)/0.00355
 */


void temp_init(void)
{
	ADC10CTL1 |= INCH_10 + ADC10DIV_3;
	ADC10CTL0 |= REFON | ADC10ON | SREF_1;
}

#define TEMP_SCALE 3.6352 // V/(deg C)
#define TEMP_OFFSET 277.746478873 // deg C
#define VREF 1.5 // V
void temp_sample(int16_t *temp)
{
	ADC10CTL0 &= ~ADC10IFG;
	ADC10CTL0 |= ENC | ADC10SC;
	while (!(ADC10CTL0 & ADC10IFG));
	ADC10CTL0 &= ~ADC10IFG;
	*temp = (int16_t)(100*(ADC10MEM * VREF / TEMP_SCALE - TEMP_OFFSET));
}

