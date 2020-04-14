#include "main.h"
#include "i2c.h"
#include "stm32f1xx_hal.h"

#define I2C_Delay_Times 5//延时的个数，5即为延时5Us

/**
* @name		void I2C_Start(void)
* @brief	发送I2C起始信号
* @details	当SCL为高时，SDA由高拉低，就会形成起始信号
* @param	None
* @retval	None
*/

static I2C_Delay_Us(uint16_t i)
{
	i*=50;
	while(i--);
}

void I2C_Start(void) {
	SDA_H;										//SDA拉高
	//I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_H;										//SCL拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SDA_L;										//SDA拉低
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_L;										//SCL拉低，钳住
}
/**
* @brief	发送I2C结束信号
* @details	当SCL为高时，SDA由低拉高，就会形成结束信号
* @param	None
* @retval	None
*/
void I2C_Stop(void) {
	SDA_L;										//SDA拉低
	SCL_L;										//SCL拉低
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_H;										//SCL拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SDA_H;										//SDA拉低
	I2C_Delay_Us((I2C_Delay_Times * 2));		//延时
}
/**
* @brief	通过I2C发送（写）数据
* @details	当SCL为高时，保持SDA（高为1，低位0）稳定，即可传输一位数据
* @param	byte：想要发送的数据（1字节，8位）
* @retval	None
*/
void I2C_SendByte(uint8_t sendByte) {
	SCL_L;										//先拉低时钟线
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	for (int i = 0; i < 8; i++)
	{
		if (sendByte & 0x80) {					//如果高位为1则拉高，否则拉低
			SDA_H;								//SDA拉高
		}
		else {
			SDA_L;								//SDA拉低
		}
		sendByte <<= 1;							//数据左移1位
		I2C_Delay_Us(I2C_Delay_Times);			//延时
		SCL_H;									//SCL拉高
		I2C_Delay_Us(I2C_Delay_Times);			//延时
		SCL_L;									//SCL拉低
	}
}
/**
* @brief	等待从机应答信号
* @details	主机将和SCL和SDA都拉高之后，释放SDA，然后读取SDA，为低则表示接收到从机的应答信号
* @param	timeout: 在规定时间内等待应答信号
* @retval	I2C_WaitAck_Succeed：	成功等到应答信号
* @retval	I2C_WaitAck_Fail：		在规定时间内未等到应答信号
*/
uint8_t I2C_WaitAck(uint16_t timeout) {
	SDA_H;										//SDA拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_H;										//SCL拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时

	//开始循环查询SDA线上是否有应答信号
	while (SDA_R)
	{
		//如果为1，则说明未产生应答
		if (!(--timeout)) {
			//如果时间超时
			//如果为非主机测量，则此处不能Stop
			//I2C_Stop();							//发出终止信号
			return I2C_Fail;					//返回失败信息
		}
		//如果时间未超时
		I2C_Delay_Us(1);						//延时一微秒
	}
	//如果收到了应答信号，将SCL拉低
	SCL_L;										//SCL拉低
	return I2C_Succeed;							//返回成功信息
}
/**
* @brief	向从机发送应答信号
* @details	在SDA持续为低时，SCL产生一个正脉冲即表示产生一个应答信号
* @param	None
* @retval	None
*/
void I2C_SendAck(void) {
	//重大bug 一定要先拉低scl再拉低sda，否则会读到0xFF
	SCL_L;										//SCL拉低
	SDA_L;										//SDA拉低
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_H;										//SCL拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_L;										//SCL拉低
}
/**
* @brief	向从机发送非应答信号
* @details	在SDA持续为高时，SCL产生一个正脉冲即表示产生一个非应答信号
* @param	None
* @retval	None
*/
void I2C_SendNAck(void) {
	SDA_H;										//SDA拉低
	SCL_L;										//SCL拉低
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_H;										//SCL拉高
	I2C_Delay_Us(I2C_Delay_Times);				//延时
	SCL_L;										//SCL拉低
}
/**
* @brief	读取SDA线上的数据
* @details	将SDA拉高后，不断产生正脉冲后读取SDA的高低，即为从机发送的数据
* @param	None
* @retval	readByte：读到的一字节数据
*/
uint8_t I2C_ReadByte(void) {
	uint8_t readByte = 0;
	SDA_H;
	for (uint8_t i = 0; i < 8; i++) {
		SCL_L;										//SCL拉低
		I2C_Delay_Us(I2C_Delay_Times);				//延时
		SCL_H;										//SCL拉高
		readByte <<= 1;
		if (SDA_R) {
			readByte++;
		}
		I2C_Delay_Us(I2C_Delay_Times);				//延时
	}
	return readByte;
}
