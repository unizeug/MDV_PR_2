/**
 *
 * \file adu.c
 * \author Thommy & Dirk
 * \date 26.04.2012
 * ------------------------------------------------------
 * \brief functions to access the adc
 */

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>


ISR(ADC_vect) {
	//ADCH und ADCL werden gemeinsam als 16-bit Wert ausgelesen
	uint16_t adu_wert = ADC;


	//Alle LEDs aus
	PORTC |= (1<<PC5);
	PORTC |= (1<<PC4);
	PORTC |= (1<<PC1);


	//F: soll Rot an? A: JAAAAAAA
	PORTC &= ~(1<<PC5);

	//soll Gruen an
	if(adu_wert >= 341){
		PORTC &= ~(1<<PC4);
	}

	//soll Orange an
	if(adu_wert >= 683){
		PORTC &= ~(1<<PC1);

		//Interruptroutine kann nicht unterbrochen werden
	}


}




int main(void){


	//LEDs initialisieren
	//orange
	DDRC |= (1<<PC1);
	PORTC |= (1<<PC1);

	//Grün
	DDRC |= (1<<PC4);
	PORTC |= (1<<PC4);

	//Rot
	DDRC |= (1<<PC5);
	PORTC |= (1<<PC5);


	ADCSRA	|= (1<<ADEN);                //ADU anschalten
	ADMUX	|= (1<<REFS1)|(1<<REFS0);    //2.56V Referenzspannung
	ADCSRA	|= (1<<ADPS2)|(1<<ADPS0);    //takt auf 1/32
	ADCSRA	|= (1<<ADIE);                //ADC Interrupt anschalten
	ADCSRA	|= (1<<ADATE);               //auto trigger aktivieren
	// für freerunning muss ADTS0, ADTS1, ADTS2 auf Null bleiben

	//ergebnis links
	//damit nur ADCH gelesen werde muss (für 8-Bit genauigkeit)
	//	ADMUX	|= (1<<ADLAR);

	// startet den ADU beim freerunner, start single conversion sonst
	ADCSRA	|= (1<<ADSC);



	sei();
while(1){};

return 0;

}
