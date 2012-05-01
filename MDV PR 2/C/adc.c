*
 * \addtogroup adc
 *
 * \file adc.c
 * \author: Maggie Busuru, Dirk Babendererde, Thomas Kapa
 * \27.04.2012
 * ------------------------------------------------------
 * \brief functions to access the adc
 */

#define F_CPU 7372800UL
#include "adc.h"
#include "serial.h"
#include "filter.h"
#include <inttypes.h>


/*** GLOBAL VARIABLES ***/
uint32_t SAMPLE_COUNTER = 0;


/*** INTERRUPT ROUTINES ***/
ISR(TIMER1_COMPA_vect) {


}

ISR(TIMER1_COMPB_vect) {
}

ISR(ADC_vect) {

	uint16_t adu_wert = ADC; //ADCH und ADCL werden gemeinsam als 16-bit Wert ausgelesen
	filterWrite2Buf(adu_wert -512) ;

	SAMPLE_COUNTER--;
	if (SAMPLE_COUNTER == 0){
		cli();
	}


}

void adcInit() {

	ADCSRA	|= (1<<ADEN); //ADU anschalten
	ADMUX	|= (1<<REFS1)|(1<<REFS0);	//2.56V Referenzspannung
	ADCSRA	|= (1<<ADPS2)|(1<<ADPS0);	//takt auf 1/32
	ADCSRA	|= (1<<ADIE);	//ADC Interrupt anschalten
	ADCSRA	|= (1<<ADATE); //auto trigger aktivieren
	//Für den Free-running Modus müssen ADTS0,1,2 im Register ADCSRB auf null gesetzt sein, da aber schon normal auf null kein Problem
	//die default Eigenschaft ist, dass der ADC0 Channel benutzt wird
    //für den Single-Ended-Modus mit Kanal ADC0 muss ADMUX muss MUX4:0 muss auf 0 gesetzt werden, MUX0,1,2,3
	ADCSRA	|= (1<<ADSC); // startet den ADU beim freerunner, start single conversion
    ADCSRB  |= (1<<ADTS2)|(1<<ADTS0); //Umsetzung startet durch einen Compare Match von Timer1

}



void adcStart(uint16_t sampleRateCode, uint32_t sampleCount, trigger_t triggerMode, int16_t triggerLevel) {
	cli();

	SAMPLE_COUNTER = sampleCount; //sampleCounter auf den gewünschten wert setzen

	OCR1A =  sampleRateCode; // die semplerate auf den richtigen wert setzen


	TCCR1B |=(1<<CS10); // Clock auf non-prescaling, betrieben mit CPU-Takt
	TCCR1B |=(1<<WGM12); //CTC Mode



	TCNT1 = 0 ;
	// Timer s t a r t e t b e i 0

	TIMSK1 = (1<<OCIE1A);
	// Timerâˆ’Outputâˆ’Compareâˆ’I n t e r r u p t B a k t i v i e r e n

	TCCR1A = 0;
	// k e i n Output /Compare

	sei();
}

uint8_t adcIsRunning() {

if(SAMPLE_COUNTER==0){
	return 0;
	} else {
	return 1;
	}
}
