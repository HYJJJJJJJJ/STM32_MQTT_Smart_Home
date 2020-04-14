/**
 ******************************************************************************
 * File Name          : USART.c
 * Description        : This file provides code for the configuration
 *                      of the USART instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
USART_REC_MESSAGE uart1_message = { 0 }, uart3_message = { 0 };
u8 USART3_TX_BUF[64];
u8 aRxBuffer[1];//HAL库使用的串口接收缓冲
u8 Res;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void) {
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}

}
/* USART3 init function */

void MX_USART3_UART_Init(void) {

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	HAL_UART_Receive_IT(&huart3, (u8 *)aRxBuffer, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	} else if (uartHandle->Instance == USART3) {
		/* USER CODE BEGIN USART3_MspInit 0 */

		/* USER CODE END USART3_MspInit 0 */
		/* USART3 clock enable */
		__HAL_RCC_USART3_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**USART3 GPIO Configuration
		 PB10     ------> USART3_TX
		 PB11     ------> USART3_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART3 interrupt Init */
		HAL_NVIC_SetPriority(USART3_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		/* USER CODE BEGIN USART3_MspInit 1 */

		/* USER CODE END USART3_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {

	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	} else if (uartHandle->Instance == USART3) {
		/* USER CODE BEGIN USART3_MspDeInit 0 */

		/* USER CODE END USART3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART3_CLK_DISABLE();

		/**USART3 GPIO Configuration
		 PB10     ------> USART3_TX
		 PB11     ------> USART3_RX
		 */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

		/* USART3 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART3_IRQn);
		/* USER CODE BEGIN USART3_MspDeInit 1 */

		/* USER CODE END USART3_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */
//void USART1_IRQHandler(void) {
//	u8 Res;
//	if ((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) != RESET)) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		HAL_UART_Receive(&huart1, &Res, 1, 1000);
//		if ((USART_RX_STA & 0x8000) == 0)  //接收未完成
//				{
//			if (USART_RX_STA & 0x4000)  //接收到了0x0d
//					{
//				if (Res != 0x0a)
//					USART_RX_STA = 0;  //接收错误,重新开始
//				else
//					USART_RX_STA |= 0x8000;	//接收完成了
//			} else //还没收到0X0D
//			{
//				if (Res == 0x0d)
//					USART_RX_STA |= 0x4000;
//				else {
//					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
//					USART_RX_STA++;
//					if (USART_RX_STA > (USART_REC_LEN - 1))
//						USART_RX_STA = 0; //接收数据错误,重新开始接收
//				}
//			}
//		}
//	}
//	HAL_UART_IRQHandler(&huart1);
//}
/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void) {

	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&huart3);	//调用HAL库中断处理公用函数

	timeout=0;
    while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>maxDelay) break;
	}

	timeout=0;
	while(HAL_UART_Receive_IT(&huart3, (u8 *)aRxBuffer, 1) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>maxDelay) break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)//如果是串口1
	{
		if((uart3_message.USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(uart3_message.USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)uart3_message.USART_RX_STA=0;//接收错误,重新开始
				else uart3_message.USART_RX_STA|=0x8000;	//接收完成了
			}
			else //还没收到0X0D
			{
				if(aRxBuffer[0]==0x0d)uart3_message.USART_RX_STA|=0x4000;
				else
				{
					uart3_message.USART_RX_BUF[uart3_message.USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					uart3_message.USART_RX_STA++;
					if(uart3_message.USART_RX_STA>(64-1))uart3_message.USART_RX_STA=0;//接收数据错误,重新开始接收
				}
			}
		}
	}
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char *fmt, ...) {
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char*) USART3_TX_BUF, fmt, ap);
	va_end(ap);
	HAL_UART_Transmit(&huart3, "AB", 2, 0xffff);
	i = strlen((const char*) USART3_TX_BUF);		//此次发送数据的长度
//    for(j=0; j<i; j++)							//循环发送数据
//    {
//        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
//        USART_SendData(USART3,USART3_TX_BUF[j]);
//    }
	HAL_UART_Transmit(&huart3, (uint8_t*) USART3_TX_BUF, i, 0xffff);

}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
