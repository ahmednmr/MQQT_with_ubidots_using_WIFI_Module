/*
 * ESP_MQQT.c
 *
 *  Created on: Aug 24, 2020
 *      Author: embeddedfab
 */

#include"ESP_MQQT.h"




char Rec_Data[DEFAULT_BUFFER_SIZE];
char Counter=0;


ISR (USART_RXC_vect)
{
	uint8_t oldsrg = SREG;
	cli();

	Rec_Data[Counter] = UDR;
	Counter++;

	if(Counter == DEFAULT_BUFFER_SIZE){
		Counter = 0; //pointer = 0;
	}
	SREG = oldsrg;
}





char Check_Respond(char * Expected_Respond)
{
	char Respond_Length=0;

	Respond_Length=strlen(Expected_Respond);

	if(strncmp(Rec_Data,Expected_Respond, Respond_Length)==0)
	{
		Clear_REC_Buffer();
		return TRUE;
	}
	return FALSE;


}


char Check_Word_in_Respond(char * Word)
{


	if(strstr(Rec_Data,Word) != 0)
	{
		Clear_REC_Buffer();
		return TRUE;
	}

	return FALSE;
}

void Clear_REC_Buffer(void)
{
	Counter=0;
	memset(Rec_Data,'\0',DEFAULT_BUFFER_SIZE);
}

bool ESP_init(void)
{
	char connection_status=0; unsigned int time_out=0;

	init_UART();
	_delay_ms(2000);
	Clear_REC_Buffer();



	UART_SEND_string("ATE0\r\n");
	while(!((time_out>2000)||(Check_Respond("\r\nOK\r\n"))||(Check_Respond("ATE0\r\r\n\r\nOK\r\n"))))
	{
		_delay_ms(1);
		time_out++;
	}


	connection_status=ESP_check_connection();

	if(connection_status==ESP8266_created_TCP_connection)
	{

		UART_SEND_string("AT+CIPCLOSE\r\n");                                    //close TCP Connection
		_delay_ms(400);
		Clear_REC_Buffer();
	}

	else if((connection_status==ESP8266_connected_to_AP)||(connection_status==ESP8266_disconnected_TCP_connection))
	{

		UART_SEND_string("AT+CWQAP\r\n");                                    //close wifi Connection
		_delay_ms(400);
		Clear_REC_Buffer();
	}



	time_out=0;
	UART_SEND_string("AT+CWMODE=3\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}

	time_out=0;
	UART_SEND_string("AT+CIPMUX=0\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}

	time_out=0;
	UART_SEND_string("AT+CIPMODE=0\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}

	return TRUE;

}



	bool ESP_connect_to_AP(char* USER_NAME,char* PASSWORD)
	{
		unsigned int time_out=0;

		UART_SEND_string( "AT+CWJAP=\"");
		UART_SEND_string( USER_NAME);
		UART_SEND_string( "\",\"");
		UART_SEND_string( PASSWORD);
		UART_SEND_string( "\"\r\n");



	while(!((time_out>10000)||(Check_Respond("WIFI DISCONNECT\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))||(Check_Respond("WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))))
	{
		_delay_ms(1);
		time_out++;

	}

	if (time_out>10000)
	{
		if(ESP_check_connection()==ESP8266_connected_to_AP)
		{
			return TRUE;
		}

		return FALSE;
	}
	else                 return TRUE;
}

void ESP_error_handler(void)
{
	unsigned char connection_status=0;

	connection_status=ESP_check_connection();

	if(connection_status==ESP8266_created_TCP_connection)
	{

		UART_SEND_string("AT+CIPCLOSE\r\n");                                    //close TCP Connection
		_delay_ms(400);
		Clear_REC_Buffer();
	}

	if((connection_status==ESP8266_connected_to_AP)||(connection_status==ESP8266_disconnected_TCP_connection))
	{

		UART_SEND_string("AT+CWQAP\r\n");                                    //close wifi Connection
		_delay_ms(400);
		Clear_REC_Buffer();
	}


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


}



char ESP_check_connection(void)
{
	unsigned int time_out=0;
	Clear_REC_Buffer();
	UART_SEND_string( "AT+CIPSTATUS\r\n");

	while(!((time_out>2000)||Check_Word_in_Respond("STATUS")))
	{
		_delay_ms(1);
		time_out++;
	}


	_delay_ms(50);

	if(Check_Respond(":2\r\n"))
	{
		return ESP8266_connected_to_AP;
	}
	else if(Check_Respond(":3\r\n"))
	{
		return ESP8266_created_TCP_connection;
	}
	else if(Check_Respond(":4\r\n"))
	{
		return ESP8266_disconnected_TCP_connection;
	}
	else if(Check_Respond(":5\r\n"))
	{
		return ESP8266_is_Not_connected_to_AP;
	}

	return FALSE;

}

bool ESP_open_socket(char* connection_type,char* server_ip,char* server_port)
{
	unsigned int time_out=0;

	UART_SEND_string( "AT+CIPSTART=\"");
	UART_SEND_string( connection_type);
	UART_SEND_string("\",\"");
	UART_SEND_string( SERVER_IP);
	UART_SEND_string( "\",");
	UART_SEND_string( SERVER_PORT);
	UART_SEND_string( "\r\n");



	while(!((time_out>8000)||Check_Respond("CONNECT\r\n\r\nOK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}

	_delay_ms(300);

	if (time_out>8000)
	{
		if(ESP_check_connection()==ESP8266_created_TCP_connection)
		{
			return TRUE;
		}

		return FALSE;
	}
	else                 return TRUE;
}


bool ESP_MQTT_connect(const char * client_id,const char * user_name,const char * password)
{
	unsigned int time_out=0;
	unsigned char client_id_length=0,user_name_length=0,password_length=0;
	unsigned char remeaning_length_byte=0,frame_length=0;

	client_id_length=strlen(client_id);
	user_name_length=strlen(user_name);
	password_length=strlen(password);

	remeaning_length_byte=client_id_length+user_name_length+password_length+16;
	frame_length=remeaning_length_byte+2;

	Clear_REC_Buffer();

	UART_SEND_string("AT+CIPSEND=");
	UART_SEND_number(frame_length);
	UART_SEND_string("\r\n");

	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n> ")))
	{
		_delay_ms(1);
		time_out++;
	}

	UART_SendChar(0x10);
	UART_SendChar(remeaning_length_byte);  // 68 char will be send
	UART_SendChar(0x00);
	UART_SendChar(0x04);
	UART_SEND_string("MQTT");       //4
	UART_SendChar(0x04);
	UART_SendChar(0xC2);     //C2   CA
	UART_SendChar(0x8C);    // keep alive 10 hours
	UART_SendChar(0xA0);    // keep alive 10 hours
	UART_SendChar(0x00);
	UART_SendChar(client_id_length);
	UART_SEND_string((char *)Client_ID);   //16 char
	UART_SendChar(0x00);
	UART_SendChar(user_name_length);
	UART_SEND_string((char *)User_Name);  // 35 char
	UART_SendChar(0x00);
	UART_SendChar(password_length);
	UART_SEND_string((char *)Password);   //1 char

	while(!((time_out>2000)||Check_Word_in_Respond("+IPD")))
	{
		_delay_ms(1);
		time_out++;
	}
	_delay_ms(300);

	if (time_out>2000)  return FALSE;
	else                 return TRUE;

}

bool ESP_MQTT_ping(void)
{
	unsigned int time_out=0;
	UART_SEND_string("AT+CIPSEND=2\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n> ")))
	{
		_delay_ms(1);
		time_out++;
	}

	UART_SendChar(0xC0);
	UART_SendChar(0x00);

	while(!((time_out>2000)||Check_Word_in_Respond("+IPD")))
	{
		_delay_ms(1);
		time_out++;
	}
	_delay_ms(300);
	Clear_REC_Buffer();

	if (time_out>2000)  return FALSE;
	else                 return TRUE;

}


bool ESP_MQTT_publish(const char* _topic,const char* _sub_topic,unsigned int _data)
{
	unsigned char topic_length_byte=0,remeaning_length_byte=0,data_length=0,frame_length=0;
	unsigned char sub_topic_length=0;
	char buffer[10];
	unsigned int time_out=0;

	itoa(_data,buffer,10);

	topic_length_byte=strlen(_topic);
	data_length      =strlen(buffer);
	sub_topic_length =strlen(_sub_topic);
	remeaning_length_byte=topic_length_byte+data_length+3+sub_topic_length;
	frame_length=remeaning_length_byte+2;

	Clear_REC_Buffer();
	UART_SEND_string("AT+CIPSEND=");
	UART_SEND_number(frame_length);
	UART_SEND_string("\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n> ")))
	{
		_delay_ms(1);
		time_out++;
	}


	UART_SendChar(0x30);    //0x30   32
	UART_SendChar(remeaning_length_byte);     //remaining data length  48
	UART_SendChar(0x00);
	UART_SendChar(topic_length_byte);     //topic length

	UART_SEND_string((char*)_topic);   //18 char
	UART_SEND_string((char*)_sub_topic);
	UART_SEND_number(_data);    //28 char
	UART_SEND_string("}");    //28 char

	while(!((time_out>2000)||Check_Word_in_Respond("SEND OK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}
	_delay_ms(300);
	Clear_REC_Buffer();

	if (time_out>2000)  return FALSE;
	else                 return TRUE;

}


void ESP_MQTT_subscribe_to_topics(const char* _topic1,const char* _topic2,const char* _topic3)
{

	while(!ESP_MQTT_subscribe(_topic1))
	{
		EF_void_LCD_Clear_then_print("couldn't S TOPIC1");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("MQTT S topic1");

	while(!ESP_MQTT_subscribe(_topic2))
	{
		EF_void_LCD_Clear_then_print("couldn't S TOPIC2");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("MQTT S topic2");

	while(!ESP_MQTT_subscribe(_topic3))
	{
		EF_void_LCD_Clear_then_print("couldn't S TOPIC3");
		Clear_REC_Buffer();
	};
	EF_void_LCD_Clear_then_print("MQTT S topic3");


	_delay_ms(2000);
	Clear_REC_Buffer();
}



bool ESP_MQTT_subscribe(const char* _topic)
{
	unsigned char topic_length_byte=0,remeaning_length_byte=0,frame_length=0;

	unsigned int time_out=0;


	topic_length_byte=strlen(_topic);
	remeaning_length_byte=topic_length_byte+5;
	frame_length=remeaning_length_byte+2;



	Clear_REC_Buffer();
	UART_SEND_string("AT+CIPSEND=");
	UART_SEND_number(frame_length);
	UART_SEND_string("\r\n");
	while(!((time_out>2000)||Check_Respond("\r\nOK\r\n> ")))
	{
		_delay_ms(1);
		time_out++;
	}

	UART_SendChar(0x82);
	UART_SendChar(remeaning_length_byte);     //31
	UART_SendChar(0x00);
	UART_SendChar(0x0A);
	UART_SendChar(0x00);
	UART_SendChar(topic_length_byte);


	UART_SEND_string((char*)_topic);  //26
	UART_SendChar(0x00);

	while(!((time_out>3000)||Check_Word_in_Respond("SEND OK\r\n")))
	{
		_delay_ms(1);
		time_out++;
	}
	_delay_ms(300);
	Clear_REC_Buffer();

	if (time_out>3000)  return FALSE;
	else                 return TRUE;

}


bool check_receiving_new_data_from_server(const char * topic1,const char * topic2,const char * topic3,data_from_server* topics)
{
	unsigned char i=0,j=0,k=0;
	unsigned char topic_name_length1=0;
	unsigned char topic_name_length2=0;
	unsigned char topic_name_length3=0;
	unsigned char correct_charracters_1st_topic=0;
	unsigned char correct_charracters_2nd_topic=0;
	unsigned char correct_charracters_3rd_topic=0;
	bool found_topic=FALSE;

	topic_name_length1=strlen(topic1);
	topic_name_length2=strlen(topic2);
	topic_name_length3=strlen(topic3);

	char *ptr_detect_new_data=strstr(Rec_Data,"\r\n+IPD");
	char *ptr_detect_lost_connection=strstr(Rec_Data,"CLOSED\r\n");
	if(ptr_detect_new_data!=0)
	{

		for(i=0;i<100;i++)
		{

			if(topic1!="0000")
			{
				correct_charracters_1st_topic=0;
				for(j=0;j<topic_name_length1;j++)
				{
					if(Rec_Data[i+j]==topic1[j])
					{
						correct_charracters_1st_topic++;
						if(correct_charracters_1st_topic==topic_name_length1)
						{
							found_topic=TRUE;

							k=0;
							while((Rec_Data[i+j]!='\0')&&(k<topic_data_size))
							{
								j++;
								if((Rec_Data[i+j]==0x30)||(Rec_Data[i+j]==0x31))
								topics->topic1_data[k++]=Rec_Data[i+j];
							}



							Clear_REC_Buffer();
						}

					}


				}
			}
			if(topic2!="0000")
			{
				correct_charracters_2nd_topic=0;
				for(j=0;j<topic_name_length2;j++)
				{

					if(Rec_Data[i+j]==topic2[j])
					{
						correct_charracters_2nd_topic++;
						if(correct_charracters_2nd_topic==topic_name_length2)
						{
							found_topic=TRUE;

							k=0;
							while((Rec_Data[i+j]!='\0')&&(k<topic_data_size))
							{
								j++;
								if((Rec_Data[i+j]==0x30)||(Rec_Data[i+j]==0x31))
								topics->topic2_data[k++]=Rec_Data[i+j];
							}




							Clear_REC_Buffer();
						}



					}


				}


			}

			if(topic3!="0000")
			{
				correct_charracters_3rd_topic=0;
				for(j=0;j<topic_name_length3;j++)
				{

					if(Rec_Data[i+j]==topic3[j])
					{
						correct_charracters_3rd_topic++;
						if(correct_charracters_3rd_topic==topic_name_length3)
						{
							found_topic=TRUE;
							k=0;
							while((Rec_Data[i+j]!='\0')&&(k<topic_data_size))
							{
								j++;
								if((Rec_Data[i+j]>=0x30)&&(Rec_Data[i+j]<=0x39))
								topics->topic3_data[k++]=Rec_Data[i+j];
							}

							Clear_REC_Buffer();
						}
					}
				}
			}

			if(found_topic==TRUE)
			{
				return TRUE;
			}
		}

	}

	if(ptr_detect_lost_connection!=0)
	{

		_delay_ms(4000);
		Clear_REC_Buffer();
		ESP_error_handler();


	}

	return FALSE;

}

