#include "main.h"
#include "i2c.h"
#include "SHT2X.h"
#include "usart.h"
#include "string.h"

//开启调试打印
#define PRINTF_SHT20_INFO

/*===========================================================
* @name		void SHT20_Call(void)
* @brief	测试通讯正常与否
* @details	发送IIC起始信号和SHT20的地址，SHT20会做出相应
* @param	None
* @retval	None
===========================================================*/
void SHT20_Call(void) {
	I2C_Start();
	uint8_t addr = SHT20_ADDRESS << 1;
	I2C_SendByte(addr);//写地址
	if (I2C_WaitAck(2000)) {//等待2毫秒，从机发送应答信号
		I2C_Stop();
#ifdef PRINTF_SHT20_INFO
		uint8_t info[] = "sht20应答成功\r\n";
		HAL_UART_Transmit(&huart1, info, sizeof(info) - 1, 0xffff);
#endif//PRINTF_SHT20_INFO
	}
	else {
#ifdef PRINTF_SHT20_INFO
		uint8_t info[] = "sht20应答失败\r\n";
		HAL_UART_Transmit(&huart1, info, sizeof(info) - 1, 0xffff);
#endif//PRINTF_SHT20_INFO
	}
}
/*===========================================================
* @name		float SHT20_Measure_Humidity(void)
* @brief	测量相对湿度
* @details	调用测量函数，向测量函数传入参数'H'表示测量相对湿度
* @param	None
* @retval	float类型的测量结果
===========================================================*/
float SHT20_Measure_Humidity(void) {
	return SHT20_Measure('H');
}
/*===========================================================
* @name		float SHT20_Measure_Temperature(void)
* @brief	测量温度
* @details	调用测量函数，向测量函数传入参数'T'表示测量温度
* @param	None
* @retval	float类型的测量结果
===========================================================*/
float SHT20_Measure_Temperature(void) {
	return SHT20_Measure('T');
}
/*===========================================================
* @name		float SHT20_Measure(char MeasureTorH)
* @brief	测量函数
* @details	通过接受参数测量相对湿度或温度
* @param	MeasureTorH：'H'表示测量相对湿度，'T'表示温度
* @retval	float类型的测量结果
===========================================================*/
float SHT20_Measure(char MeasureTorH) {
	//SHT20_Reset();//可以注释掉，非必须
	//I2C_Delay_S(2);//可以注释掉，非必须

	I2C_Start();//开始
	I2C_SendByte(SHT20_ADDRESS_W);//写地址

	if (I2C_WaitAck(50000)) {
		if (MeasureTorH == 'H') {//如果是测量相对湿度
			I2C_SendByte(SHT20_N_RH_CMD);
		}
		else {//如果是测量温度
			I2C_SendByte(SHT20_N_T_CMD);
		}

		if (I2C_WaitAck(50000)) {
			I2C_Start();
			I2C_SendByte(SHT20_ADDRESS_R);
			while (!I2C_WaitAck(50000))
			{
				I2C_Start();//开始信号
				I2C_SendByte(SHT20_ADDRESS_R);//写地址
			}//循环等待测量结束
			I2C_Delay_Ms(70);
			char RHresult[3] = { 0 };
			RHresult[0] = I2C_ReadByte();//MSB
			I2C_SendAck();
			RHresult[1] = I2C_ReadByte();//LSB
			I2C_SendAck();
			RHresult[2] = I2C_ReadByte();//校验和
			I2C_SendNAck();
			I2C_Stop();
			if (!SHT20_CheckSum_CRC8(RHresult)) {//进行CRC8校验
#ifdef PRINTF_SHT20_INFO
				SHT20_Print("CRC8检验失败");
#endif//PRINTF_SHT20_INFO
			}
			else if (MeasureTorH == 'H') {
				return SHT20_Calculate('H', (RHresult[0] << 8) + RHresult[1]);//进行换算
			}
			else {
				return SHT20_Calculate('T', (RHresult[0] << 8) + RHresult[1]);//进行换算
			}
		}
		else {
#ifdef PRINTF_SHT20_INFO
			SHT20_Print("I2C通讯异常");
#endif//PRINTF_SHT20_INFO
			return I2C_Fail;
		}
	}
	else {
#ifdef PRINTF_SHT20_INFO
		SHT20_Print("I2C通讯异常");
#endif//PRINTF_SHT20_INFO
		return I2C_Fail;
	}

	return I2C_Fail;

}
/*===========================================================
* @name		void SHT20_Reset(void)
* @brief	SHT20软复位函数
* @details	通过写入软复位命令进行复位
* @param	None
* @retval	None
===========================================================*/
void SHT20_Reset(void) {
	unsigned char addr = 0;
	addr = SHT20_ADDRESS << 1;
	I2C_Start();
	I2C_SendByte(addr);
	if (!I2C_WaitAck(50000)) {
#ifdef PRINTF_SHT20_INFO
		SHT20_Print("SHT20软复位失败");
#endif//PRINTF_SHT20_INFO
	}
	I2C_SendByte(0xFE);
	if (!I2C_WaitAck(50000)) {
#ifdef PRINTF_SHT20_INFO
		SHT20_Print("SHT20软复位失败");
#endif//PRINTF_SHT20_INFO
	}
	I2C_Stop();
}
/*===========================================================
* @name		uint8_t SHT20_ReadRegister(void)
* @brief	读寄存器
* @details	读取SHT20的寄存器值
* @param	None
* @retval	ReadResult：返回当前SHT20寄存器的值
===========================================================*/
uint8_t SHT20_ReadRegister(void) {
	SHT20_Reset();
	I2C_Delay_Ms(1000);
	I2C_Delay_Ms(1000);
	I2C_Start();//开始
	I2C_SendByte(SHT20_ADDRESS_W);//写地址

	if (I2C_WaitAck(50000)) {
		I2C_SendByte(SHT20_READ_REG);
		if (I2C_WaitAck(50000)) {
			I2C_Start();
			I2C_SendByte(SHT20_ADDRESS_R);
			if (I2C_WaitAck(50000)) {
				uint8_t ReadResult = I2C_ReadByte();
				I2C_SendNAck();
				I2C_Start();
				I2C_SendByte(SHT20_ADDRESS_W);
				if (I2C_WaitAck(50000)) {
					I2C_SendByte(SHT20_WRITE_REG);
					if (I2C_WaitAck(50000)) {
						I2C_SendByte(ReadResult);
						if (I2C_WaitAck(50000)) {
							I2C_Stop();
							return ReadResult;
						}
					}
				}
			}
		}
	}
	return 0;
}
/*===========================================================
* @name		uint8_t SHT20_WriteRegister(uint8_t WaitWrite)
* @brief	写寄存器
* @details	读取SHT20的寄存器值
* @param	WaitWrite：写入寄存器的值
* @retval	写入成功返回1，失败返回0
===========================================================*/
uint8_t SHT20_WriteRegister(uint8_t WaitWrite) {
	SHT20_Reset();
	I2C_Delay_S(2);
	I2C_Start();//开始
	I2C_SendByte(SHT20_ADDRESS_W);//写地址

	if (I2C_WaitAck(50000)) {
		I2C_SendByte(SHT20_READ_REG);
		if (I2C_WaitAck(50000)) {
			I2C_Start();
			I2C_SendByte(SHT20_ADDRESS_R);
			if (I2C_WaitAck(50000)) {
				uint8_t ReadResult = I2C_ReadByte();
				I2C_SendNAck();
				I2C_Start();
				I2C_SendByte(SHT20_ADDRESS_W);
				if (I2C_WaitAck(50000)) {
					I2C_SendByte(SHT20_WRITE_REG);
					if (I2C_WaitAck(50000)) {
						I2C_SendByte(WaitWrite);
						if (I2C_WaitAck(50000)) {
							I2C_Stop();
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
/*===========================================================
* @name		uint8_t SHT20_CheckSum_CRC8(char* Result)
* @brief	CRC8检验
* @details	对测量结果的MSB和LSB进行校验，判断是否等于接收到的检验和
* @param	Result：测量结果所在数组的头指针
* @retval	检验成功返回1，失败返回0
===========================================================*/
uint8_t SHT20_CheckSum_CRC8(char* Result) {
	char data[2];
	data[0] = Result[0];
	data[1] = Result[1];

	uint32_t POLYNOMIAL = 0x131;
	char crc = 0;
	char bit = 0;
	char byteCtr = 0;

	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < 2; ++byteCtr)
	{
		crc ^= (data[byteCtr]);
		for (bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
			else crc = (crc << 1);
		}
	}
	if (crc == Result[2]) {
		return 1;
	}
	else {
		return 0;
	}

}
/*===========================================================
* @name		float SHT20_Calculate(char TorR, uint16_t data)
* @brief	相对湿度和温度的换算
* @details	根据接受到的数据进行换算
* @param	TorR：由'H'还是'T'选择进行相对湿度的换算还是温度的换算
* @param	data：测量结果
* @retval	float类型的换算结果，亦是最终结果
===========================================================*/
float SHT20_Calculate(char TorR, uint16_t data) {
	data &= 0xfffc;
	if (TorR == 'H') {
		return (data *125.0 / 65536.0) - 6;
	}
	else {
		return (data *175.72 / 65536.0) - 46.85;
	}
}

void SHT20_Print(char* str)
{
	uint16_t len = 0;
	char* str_temp = str;
	while (*str_temp++ != '\0') {
		len++;
	}
	HAL_UART_Transmit(&huart1, (uint8_t *)str, len, 0xffff);
	HAL_UART_Transmit(&huart1, (uint8_t *)("\r\n"), 2, 0xffff);
}
