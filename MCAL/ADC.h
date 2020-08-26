/*
 * ADC.h
 *
 *  Created on: Nov 6, 2019
 *      Author: embeddedfab
 */

#ifndef ADC_H_
#define ADC_H_
#include<avr/io.h>

#define CH0 0b00000000
#define CH1 0b00000001
#define CH2 0b00000010
#define CH3 0b00000011

void ADC_init(void);
unsigned int ADC_Get_reading_mv(char ch);
char ADC_Get_Temp_reading(char ch);

#endif /* ADC_H_ */
