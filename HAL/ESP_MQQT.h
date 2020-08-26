/*
 * ESP_MQQT.h
 *
 *  Created on: Aug 24, 2020
 *      Author: embeddedfab
 */

#ifndef HAL_ESP_MQQT_H_
#define HAL_ESP_MQQT_H_


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "../MCAL/UART.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>

#include "EF_LCD.h"

#define WIFI_USER_NAME "B68L-73EE"
#define WIFI_PASSWORD   "7BF3260E"

#define CONNECTION_TYPE      "TCP"
#define SERVER_IP            "industrial.api.ubidots.com"
#define SERVER_PORT			 "1883"

#define Client_ID      (const char *)"11232siy1weu1e21"        //16 digits
#define User_Name      (const char *)"BBFF-TshfvGSxzGLZykOkVcbT5yK9GpNCDy"                     //MQTT  35
#define Password       (const char *)" "


// publish topics
#define PUBLISH_TOPIC1_NAME     "/v1.6/devices/Demo"

#define PUBLISH_sub1_TOPIC      "{\"temp\":"
#define PUBLISH_sub2_TOPIC           "{\"led4\":"
#define ALARM_ON 1
#define ALARM_OFF 0


// subscription topics
#define subscribe_TOPIC1_NAME    (const char *)"/v1.6/devices/demo/led7/lv"   //26
#define subscribe_TOPIC2_NAME    (const char *)"/v1.6/devices/demo/led6/lv"   //26
#define subscribe_TOPIC3_NAME    (const char *)"/v1.6/devices/demo/position/lv"

#define TRUE 1
#define FALSE 0
#define DEFAULT_BUFFER_SIZE 100


typedef enum myenum
{
	ESP8266_connected_to_AP=2,
	ESP8266_created_TCP_connection,
	ESP8266_disconnected_TCP_connection,
	ESP8266_is_Not_connected_to_AP,

}connection_status;

#define topic_data_size 5

typedef struct
{
	char topic1_data[topic_data_size];
	char topic2_data[topic_data_size];
	char topic3_data[topic_data_size];

}data_from_server;

bool ESP_init(void);
bool ESP_connect_to_AP(char* USER_NAME,char* PASSWORD);
void ESP_error_handler(void);
char ESP_check_connection(void);
bool ESP_open_socket(char* connection_type,char* server_ip,char* server_port);
bool ESP_MQTT_connect(const char * client_id,const char * user_name,const char * password);
bool ESP_MQTT_publish(const char* _topic,const char* _sub_topic,unsigned int _data);
bool ESP_MQTT_ping(void);
void ESP_MQTT_subscribe_to_topics(const char* _topic1,const char* _topic2,const char* _topic3);
bool ESP_MQTT_subscribe(const char* _topic);
bool check_receiving_new_data_from_server(const char *  topic1,const char *  topic2,const char * topic3,data_from_server* topics);

char Check_Respond(char * Expected_Respond);
char Check_Word_in_Respond(char * Word);
void Clear_REC_Buffer(void);



#endif /* HAL_ESP_MQQT_H_ */
