/*
 * my_func.c
 *
 *  Created on: Feb 9, 2020
 *      Author: �˹�����
 */

#include "my_func.h"
#include "usart.h"
#include "adc.h"
#include "string.h"
#include "tim.h"

SensorData sen_data= {25,0,50};
CtrlDuty dutys= {0,0,0,0};
extern USART_REC_MESSAGE uart3_message;

u8 ESP_8266_CHECK(void)
{
    u3_printf("NETWORK CHECK\r\n");
    HAL_Delay(200);
    if(uart3_message.USART_RX_STA&0x8000)
    {
        uart3_message.USART_RX_BUF[uart3_message.USART_RX_STA&0x7fff]=0;
        uart3_message.USART_RX_STA=0;
        if(strstr((const char*)uart3_message.USART_RX_BUF,"OK"))
            return 1;
    }
    HAL_Delay(1000);
    return 0;
}
//
void Get_Sensor_Data(void)
{
#ifdef TESTMODE
    if(sen_data.temperature>=40)
        sen_data.temperature=-30;
    else sen_data.temperature++;
    if(sen_data.humidity>=100)
        sen_data.humidity=0;
    else sen_data.humidity++;
    if((sen_data.brightness+40)>=4095)
        sen_data.brightness=0;
    else sen_data.brightness+=40;
#else
    sen_data.humidity = SHT20_Measure('H');
    sen_data.temperature = SHT20_Measure('T');
//	if (sen_data.humidity) {
//		sprintf(show_str, "RH:%.2f", sen_data.humidity);
//		OLED_ShowString(0, 2, show_str);
//	}
//	if (sen_data.temperature) {
//		sprintf(show_str, "T :%.2f", sen_data.temperature);
//		OLED_ShowString(0, 4, show_str);
//	}
    sen_data.brightness = Get_Adc_Average(3,5);		//ͨ��3������5��
#endif
}
//
//
void publish(char *topic,int payload)
{
    u3_printf("%s,%d\r\n",topic,payload);
}

void Data_publish(void)
{
	HAL_NVIC_DisableIRQ(USART3_IRQn);
//    printf("�¶ȣ� %d\tʪ��: %d\t���� %d\r\n", sen_data.temperature, sen_data.humidity, sen_data.brightness);
//    printf("RGB�� %d--%d--%d\t\t����: %d\r\n", dutys.r_duty,dutys.g_duty,dutys.b_duty,dutys.fan_duty);
    publish("wd",sen_data.temperature);
    HAL_Delay(10);
    publish("sd",sen_data.humidity);
    HAL_Delay(10);
    publish("gz",(4095 - sen_data.brightness) / 40);
    HAL_Delay(10);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}
//
void Oled_show(void)
{
    u8 str[64];
    //OLED_Clear();
    sprintf((char*)str, "Bright  %d", (4095 - sen_data.brightness) / 40);
    OLED_ShowString(0, 0, str);
    sprintf((char*)str, "Temp  %.2f", sen_data.temperature);
    OLED_ShowString(0, 2, str);
    sprintf((char*)str, "Humidity  %.2f", sen_data.humidity);
    OLED_ShowString(0, 4, str);

}
//
static Message translate(char *rec_data)
{
    Message message,error_m= {"error"};
    char *strx;
    strx = strstr(rec_data, ",");
    if(strx)
    {
        *strx = 0;
        sprintf(message.topic, "%s", rec_data);
        sprintf(message.payload, "%s", (strx + 1));
        return message;
    }
    else return error_m;
}

u8 Get_Ctrl_Data()
{
    Message message;
    u32 led_duty=0;
    if(uart3_message.USART_RX_STA&0x8000)
    {
        uart3_message.USART_RX_BUF[uart3_message.USART_RX_STA&0x7fff]=0;
        printf("u3rec: %s\r\n",uart3_message.USART_RX_BUF);
//		memset(uart3_message.RX_pData,0,sizeof(uart3_message));
        uart3_message.USART_RX_STA=0;
        message=translate((char*)uart3_message.USART_RX_BUF);
        if(!strstr(message.topic,"error")) {
            if(message.topic[2]!='e') {
                printf("topic:%s\r\npayload:%s\r\n",message.topic,message.payload);
                if(message.topic[2]=='l')
                {
                    led_duty=atol(message.payload);
                    dutys.r_duty=led_duty>>16;
                    dutys.g_duty=(led_duty>>8)&0xff;
                    dutys.b_duty=led_duty&0xff;
                }
                else if(message.topic[2]=='f')
                    dutys.fan_duty=atoi(message.payload);
                memset(uart3_message.USART_RX_BUF,0,sizeof(uart3_message));
                uart3_message.USART_RX_STA=0;
                return 1;
            }
            else
            {
                printf("error message:%s\r\n",uart3_message.USART_RX_BUF);
                memset(uart3_message.USART_RX_BUF,0,sizeof(uart3_message));
                uart3_message.USART_RX_STA=0;

            }
        }
    }
    return 0;
}

void Ctrl_task()
{
    LEDR_ON(dutys.r_duty);
    LEDG_ON(dutys.g_duty);
    LEDB_ON(dutys.b_duty);
    FAN_ON((u8)(255-dutys.fan_duty*2.5));
}
