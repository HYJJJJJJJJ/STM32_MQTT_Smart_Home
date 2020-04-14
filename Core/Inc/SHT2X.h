#ifndef __SHT2X_H
#define __SHT2X_H

//SHT20地址
#define SHT20_ADDRESS  0x40
//SHT20写指令
#define SHT20_ADDRESS_W  0x80
//SHT20读指令
#define SHT20_ADDRESS_R  0x81
//读寄存器
#define SHT20_READ_REG  0xE7
//写寄存器
#define SHT20_WRITE_REG  0xE6
//非主机模式触发T（温度）测量、触发RH（湿度）测量
#define SHT20_N_T_CMD 0xF3
#define SHT20_N_RH_CMD 0xF5
//主机模式触发T（温度）测量、触发RH（湿度）测量
#define SHT20_T_CMD 0xE3
#define SHT20_RH_CMD 0xE5
/*测量分辨率 RH-T*/
//12bit-14bit
#define RH_T_12_14_Bit	0x00
//8bit-12bit
#define RH_T_8_12_Bit	0x01
//10bit-13bit
#define RH_T_10_13_Bit	0x80
//11bit-11bit
#define RH_T_11_11_Bit	0x81
/*测量分辨率 RH-T*/
//片上加热器开启（1）或关闭（0）
#define SHT20_HOT 0

#define SHT20_RH_WAIT_TIME_MS 29
void SHT20_Call(void);
float SHT20_Measure(char MeasureTorH);
float SHT20_Measure_Humidity(void);
float SHT20_Measure_Temperature(void);
void SHT20_Reset(void);
uint8_t SHT20_ReadRegister(void);
uint8_t SHT20_WriteRegister(uint8_t WaitWrite);
uint8_t SHT20_CheckSum_CRC8(char* Result);
float SHT20_Calculate(char TorRH, uint16_t data);
void SHT20_Print(char* str);
#endif /* __SHT20_H */
