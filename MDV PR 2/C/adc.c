 /*
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
// wir initialisieren eine 32-Bit Zählvariable um später die Anzahl der noch
// durchzuführenden Wandlungen zu speichern.
uint32_t SAMPLE_COUNTER = 0;


/*** INTERRUPT ROUTINES ***/
ISR(TIMER1_COMPA_vect) {
}

ISR(TIMER1_COMPB_vect) {
}


// dieser interrupt wird ausgeführt, wenn der ADU mit einer Umsetzung feritg ist
ISR(ADC_vect) {

	// der Wert des ADU wird ausgelesen
	uint16_t adu_wert = ADC; //ADCH und ADCL werden gemeinsam als 16-bit Wert ausgelesen

	// Speichern des ADU-Wertes in einem Buffer
	filterWrite2Buf(adu_wert -512) ;

	// die in adcStart gesetzte Anzahl an Umsetzungen wird decrementiert
	SAMPLE_COUNTER--;

	// Überprüugn ob noch Umsetzungen übrig sind. Falls nicht, werden die Interrups
	// ausgeschaltet, sodass diese Funktion nicht weiter aufgerufen wird.
	//(der ADU läuft aber weiter)
	if (SAMPLE_COUNTER == 0){
		cli();
	}


}

void adcInit() {

	ADCSRA	|= (1<<ADEN);                //ADU anschalten
	ADMUX	|= (1<<REFS1)|(1<<REFS0);    //2.56V Referenzspannung
	ADCSRA	|= (1<<ADPS2)|(1<<ADPS0);    //takt auf 1/32
	ADCSRA	|= (1<<ADIE);                //ADC Interrupt anschalten
	ADCSRA	|= (1<<ADATE);               //auto trigger aktivieren
	// Für den Free-running Modus müssen ADTS0,1,2 im Register ADCSRB auf null
	// gesetzt sein, da aber schon normal auf null kein Problem
	// die default Eigenschaft ist, dass der ADC0 Channel benutzt wird
    // für den Single-Ended-Modus mit Kanal ADC0 muss ADMUX muss MUX4:0 muss
    // auf 0 gesetzt werden, MUX0,1,2,3
	ADCSRA	|= (1<<ADSC);                 // startet den ADU beim freerunner, start single conversion
    ADCSRB  |= (1<<ADTS2)|(1<<ADTS0);     //Umsetzung startet durch einen Compare Match von Timer1B

}



void adcStart(uint16_t sampleRateCode, uint32_t sampleCount, trigger_t triggerMode, int16_t triggerLevel) {

    // Interrups ausschalten, damit hier nicht unterbrochen wird
	cli();

	// sampleCounter auf den gewünschten wert setzen
	SAMPLE_COUNTER = sampleCount;


	// die semplerate auf den übergebenen wert setzen
	// löst den CTC aus
	OCR1A =  sampleRateCode;
    
    // löst den Interrupt aus um um zu setzen
    OCR1B =  sampleRateCode;


	// Clock des Timers auf non-prescaling (betreiben mit CPU-Takt)
	TCCR1B |=(1<<CS10);

	// CTC Mode (Clear on Compare)
	// Timer1 wird auf null gesetzt wenn er == OCR1A ist
	TCCR1B |=(1<<WGM12);

	// Timer startet bei 0
	TCNT1 = 0 ;

	// Timer Output Compare Interrupt B aktivieren,
	// sodass wenn der timer den Wert in 
	TIMSK1 = (1<<OCIE1A);

	// kein Output/Compare
	TCCR1A = 0;

	// Interrupts anschalten, damit die umgesetzten Werte gespeichert werden
	sei();
}

uint8_t adcIsRunning() {

	// Überprüfung ob aktuell noch umsetzungen gemacht werden müssen
	if(SAMPLE_COUNTER==0){
		return 0;
	} else {
		return 1;
	}
}
