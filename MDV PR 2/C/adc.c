/**
 * \addtogroup adc
 *
 * \file adc.c
 * \author your name
 * \date day-of-writing
 * ------------------------------------------------------
 * \brief functions to access the adc
 */

#include "adc.h"
#include "serial.h"
#include "filter.h"

/*** GLOBAL VARIABLES ***/

ADCSRA










/*** INTERRUPT ROUTINES ***/
ISR(TIMER1_COMPA_vect) {
}

ISR(TIMER1_COMPB_vect) { 
}

/**
 * \fn ISR(ADC_vect)
 * \author Juergen Funck <juergen.funck@tu-berlin.de>
 * \date 2011-03-25
 * \brief Interrupt-Routine for the ADC-Interrupt. 
 *		  Gets called when an analog-to-digital conversion is complete
 */
ISR(ADC_vect) {
}

/*** FUNCTIONS ***/
void adcInit() {
}

void adcStart(uint16_t sampleRateCode, uint32_t sampleCount, trigger_t triggerMode, int16_t triggerLevel) {
}

uint8_t adcIsRunning() {
}
