/*
 * ADC.c
 *
 *  Created on: Nov 6, 2019
 *      Author: embeddedfab
 */

#include"ADC.h"




void ADC_init(void)
{

	ADMUX=0b01000000;     // vcc refrence - ADLAR=0/CH0
	ADCSRA=0b10000111;    //Enable ADC - Prescaler "/128"

}



unsigned int ADC_Get_reading_mv(char ch)
{

ADMUX =(ADMUX&0xF8)|ch;
	ADCSRA |=(1<<ADSC);  // start conversion
	while(!(ADCSRA&(1<<ADIF)));

	return ADC;


}

char ADC_Get_Temp_reading(char ch)
{
	char temp=0;
	unsigned int ADC_Value=0,Reading_MV=0;
	ADC_Value=ADC_Get_reading_mv(ch);

	Reading_MV=ADC_Value*4.88;
	temp=Reading_MV/10;



return temp;
}
