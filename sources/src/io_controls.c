/**
  ******************************************************************************
  * @file    io_controls.c
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V2.0.0
  * @date    29-07-2020
  * @brief   IO Routine
  *
  *
  * NOTE: This file is part of RadioDSP MINI project.
  *       See main.c file for additional project informations.
  * Platform: CortexM4
  ******************************************************************************/
#include "io_controls.h"
#include "stm32f4xx_conf.h"

/********************************************************************
 *
 *                   IO BUTTON AND LED SETUP
 *
 ********************************************************************/
void GPIO_Switch_Setup()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    // Initialize GPIOE (PE0) for switch
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitTypeDef GPIO_InitDef;
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	//Initialize pins
	GPIO_Init(GPIOE, &GPIO_InitDef);

    //GPIO_SetBits(GPIOG, GPIO_Pin_13);
    //GPIO_ResetBits(GPIOG, GPIO_Pin_13);

}

/********************************************************************
 *                   Time base configuration
 * Calculate is as:
 * Timer freq = timer_clock / ((TIM_Prescaler+1) * (TIM_Period+1))

 * Timer freq = timer_clock / ((TIM_Prescaler+1) * (TIM_Period+1))
 * Timer freq = 84000000 / (1500+1)*(1500+1) = 37 Hz
 * Timer freq = 84000000 / (8000)*(8000) = 5,25 Hz
 ********************************************************************/
void TIM3_Config(void){

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_TimeBaseInitStruct.TIM_Prescaler = 1500;
	TIM_TimeBaseInitStruct.TIM_Period = 1500;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	// Enable TIM3 interrupt
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	// Step 4: Initialize NVIC for timer interrupt
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}

/********************************************************************
 *                   Time base configuration
 * This is the timemillis timer that count milliseconds.
 ********************************************************************/
void TIM5_Config(void){

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 84000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
	// Enable TIM3 interrupt
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);

	// Step 4: Initialize NVIC for timer interrupt
	NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}

















