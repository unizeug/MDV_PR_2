/**
 *
 * \file adu.c
 * \author Thommy & Dirk
 * \date 26.04.2012
 * ------------------------------------------------------
 * \brief functions to access the adc
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>


ISR(ADC_vect) {

	uint16_t adu_wert = ADC; //ADCH und ADCL werden gemeinsam als 16-bit Wert ausgelesen


	//Alle LEDs aus
	PORTC |= (1<<PC5);
	PORTC |= (1<<PC4);
	PORTC |= (1<<PC1);


	//F: soll Rot an? A: JAAAAAAA
	PORTC &= ~(1<<PC5);

	//soll Grün an
	if(adu_wert >= 341){
		PORTC &= ~(1<<PC4);
	}

	//soll Orange an
	if(adu_wert >= 683){
		PORTC &= ~(1<<PC1);
	}


}




int main(void){


	//LEDs initialisieren
	//orange
	DDRC |= (1<<PC1);
	PORTC |= (1<<PC1);

	//Grün
	DDRC |= (1<<PC2);
	PORTC |= (1<<PC2);

	//Rot
	DDRC |= (1<<PC3);
	PORTC |= (1<<PC3);


	ADCSRA	|= (1<<ADEN); //ADU anschalten
//	ADMUX	|= (1<<ADLAR); //ergebnis links, damit nur ADCH gelesen werde muss (für 8-Bit genauigkeit)
	ADMUX	|= (1<<REFS1)|(1<<REFS0);	//2.56V Referenzspannung
	ADCSRA	|= (1<<ADPS2)|(1<<ADPS0);	//takt auf 1/32
	ADCSRA	|= (1<<ADIE);	//ADC Interrupt anschalten
	ADCSRA	|= (1<<ADATE); //auto trigger aktivieren
	// für freerunning muss ADTS0, ADTS1, ADTS2 auf Null sein

	ADCSRA	|= (1<<ADSC); // startet den ADU beim freerunner, start single conversion



	sei();
while(1){};
return 0;
}


	DDRC 	|= (1<<PC1);
	PORTC	|= (1<<PC1);
	TCCR1B 	|= (1<<CS10) | (1<<WGM12) | (1<<CS12);
	TIMSK1	|= (1<<OCIE1A);

	OCR1A 	 = 0x400;
