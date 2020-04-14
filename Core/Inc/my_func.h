/*
 * my_func.h
 *
 *  Created on: Feb 9, 2020
 *      Author: �˹�����
 */
 
#ifndef __MY_FUNC_H
#define __MY_FUNC_H

#include "main.h"
#include "SHT2X.h"

//#define TESTMODE

enum Network_State
{
	online,
	offline
};

extern enum Network_State network_state;

typedef struct SensorData{
	float temperature;
	float humidity;
	u16 brightness;
}SensorData;

extern SensorData sen_data;

typedef struct CtrlDuty{
	u8 r_duty;
	u8 g_duty;
	u8 b_duty;
	float fan_duty;
}CtrlDuty;

typedef struct Message{
	char topic[20];
	char payload[20];
}Message;

u8 ESP_8266_CHECK(void);
void Get_Sensor_Data(void);
void Data_publish(void);
void publish(char *topic,int payload);
void Oled_show(void);
u8 Get_Ctrl_Data(void);
void Ctrl_task(void);

#endif
