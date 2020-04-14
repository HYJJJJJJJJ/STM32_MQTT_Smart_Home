#ifndef __I2C_H
#define __I2C_H

#include "stdint.h"
#include "main.h"
//可以将I2C的微秒延时函数替换成其他自定义的延时函数
//#define I2C_Delay_Us   My_UsDelay
#define I2C_Delay_Ms   HAL_Delay
//#define I2C_Delay_S   My_SDelay

#define I2C_Fail 0
#define I2C_Succeed 1

//定义SCL输出的宏定义
#define SCL_H HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port,I2C1_SCL_Pin,GPIO_PIN_SET)
#define SCL_L HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port,I2C1_SCL_Pin,GPIO_PIN_RESET)
//定义SDA输出、读取的宏定义
#define SDA_H HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port,I2C1_SDA_Pin,GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port,I2C1_SDA_Pin,GPIO_PIN_RESET)
#define SDA_R HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port,I2C1_SDA_Pin)



//定义I2C相关函数
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t sendByte);
uint8_t I2C_WaitAck(uint16_t timeout);
void I2C_SendAck(void);
void I2C_SendNAck(void);
uint8_t I2C_ReadByte(void);

#endif /* __I2C_H */
