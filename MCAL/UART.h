/*
 * UART.h
 *
 *  Created on: Oct 11, 2019
 *      Author: Nmr
 */

#ifndef UART_H_
#define UART_H_


#define FOSC 8000000// Clock Speed
#define BAUD 115200

void init_UART(void);
void UART_SendChar(char data);
char UART_getChar(void);
void UART_SEND_string(char *array);
void UART_SEND_number(unsigned int number);


#endif /* UART_H_ */
