/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef TIM4_CHHandler;	    //定时器3通道4句柄

/* TIM4 init function */
void MX_TIM4_Init(void)
{
	htim4.Instance=TIM4;            //定时器3
	htim4.Init.Prescaler=71;       //定时器分频
    htim4.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    htim4.Init.Period=255;          //自动重装载值
    htim4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim4);       //初始化PWM

    TIM4_CHHandler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CHHandler.Pulse=0;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CHHandler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&htim4,&TIM4_CHHandler,TIM_CHANNEL_1);//配置TIM3通道4
    HAL_TIM_PWM_ConfigChannel(&htim4,&TIM4_CHHandler,TIM_CHANNEL_2);//配置TIM3通道4
    HAL_TIM_PWM_ConfigChannel(&htim4,&TIM4_CHHandler,TIM_CHANNEL_3);//配置TIM3通道4
    HAL_TIM_PWM_ConfigChannel(&htim4,&TIM4_CHHandler,TIM_CHANNEL_4);//配置TIM3通道4

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	LEDR_ON(255);LEDG_ON(255);LEDB_ON(255);


}

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
//{
//
//  if(tim_baseHandle->Instance==TIM4)
//  {
//  /* USER CODE BEGIN TIM4_MspInit 0 */
//
//  /* USER CODE END TIM4_MspInit 0 */
//    /* TIM4 clock enable */
//    __HAL_RCC_TIM4_CLK_ENABLE();
//  /* USER CODE BEGIN TIM4_MspInit 1 */
//
//  /* USER CODE END TIM4_MspInit 1 */
//  }
//}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspPostInit 0 */
	  __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE END TIM4_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM4 GPIO Configuration    
    PB6     ------> TIM4_CH1
    PB7     ------> TIM4_CH2
    PB8     ------> TIM4_CH3
    PB9     ------> TIM4_CH4 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);

  /* USER CODE BEGIN TIM4_MspPostInit 1 */

  /* USER CODE END TIM4_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
