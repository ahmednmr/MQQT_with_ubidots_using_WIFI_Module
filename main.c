/*
 * main.c
 *
 *  Created on: 5 Oct 2015
 *      Author: EmbeddedFab
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "MCAL/UART.h"
#include "MCAL/ADC.h"
#include <avr/interrupt.h>

#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "HAL/ESP_MQQT.h"
#include "HAL/EF_LCD.h"



int main()
{
	unsigned int i=0;
	char temp_value=22,k=0;
	data_from_server topics_data;

	ADC_init();
	DDRD |=(1<<PD5)|(1<<PD4)|(1<<PD7)|(1<<PD6);
	PORTD |=(1<<PD4);
	PORTD |=(1<<PD5);

	sei();

	EF_void_LCD_init();
	EF_void_LCD_Clear_then_print("start");

	ESP_init();

	while(!ESP_connect_to_AP(WIFI_USER_NAME,WIFI_PASSWORD))
	{
		EF_void_LCD_Clear_then_print("ESP Not Connected to AP");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("ESP Connected to AP");

	while(!ESP_open_socket(CONNECTION_TYPE,SERVER_IP, SERVER_PORT))
	{
		EF_void_LCD_Clear_then_print("MQTT couldn't Open Socket");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("MQTT Opened Socket");

	while(!ESP_MQTT_connect(Client_ID,User_Name,Password))
	{
		EF_void_LCD_Clear_then_print("MQTT couldn't connect to broker");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("MQTT connected to broker");

	ESP_MQTT_subscribe_to_topics(subscribe_TOPIC1_NAME,subscribe_TOPIC2_NAME,subscribe_TOPIC3_NAME);


	memset(topics_data.topic1_data,'\0',topic_data_size);
	memset(topics_data.topic2_data,'\0',topic_data_size);
	memset(topics_data.topic3_data,'\0',topic_data_size);
	EF_void_LCD_Clear_then_print("System is Ready");
	while(1)
	{

		i++;
		_delay_ms(1);
		if(i>=60000)
		{
			i=0;
			k++;
			if(!ESP_MQTT_ping())          ESP_error_handler();

		}

		if(k==4)
		{
			k=0;
			temp_value=ADC_Get_Temp_reading(1);

			if(!ESP_MQTT_publish(PUBLISH_TOPIC1_NAME,PUBLISH_sub1_TOPIC,(unsigned int)temp_value))		ESP_error_handler();

			if(temp_value>30)
			{
				ESP_MQTT_publish(PUBLISH_TOPIC1_NAME,PUBLISH_sub2_TOPIC,ALARM_ON);
			}
			else
			{
				ESP_MQTT_publish(PUBLISH_TOPIC1_NAME,PUBLISH_sub2_TOPIC,ALARM_OFF);

			}




		}

		if(check_receiving_new_data_from_server(subscribe_TOPIC1_NAME,subscribe_TOPIC2_NAME,subscribe_TOPIC3_NAME,&topics_data))
		{


			EF_void_LCD_Clear_Screen();
			EF_void_LCD_print("LED7 LED6 angle");
			EF_void_LCD_goto(2,1);
			if(!strncmp(topics_data.topic1_data,"1",strlen("1")))       EF_void_LCD_print("ON");
			else if(!strncmp(topics_data.topic1_data,"0",strlen("0")))  EF_void_LCD_print("OFF");
			EF_void_LCD_goto(2,6);
			if(!strncmp(topics_data.topic2_data,"1",strlen("1")))        EF_void_LCD_print("ON");
			else if(!strncmp(topics_data.topic2_data,"0",strlen("0")))   EF_void_LCD_print("OFF");
			EF_void_LCD_goto(2,11);
			EF_void_LCD_print(topics_data.topic3_data);

		}
	}


	return 0;
}



//
