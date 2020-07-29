/**
  ******************************************************************************
  * @file    io_display.h
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V2.0.0
  * @date    25-07-2020
  * @brief   IO Routine
  *
  *
  * NOTE: This file is part of RadioDSP project.
  *       See main.c file for additional project informations.
  ******************************************************************************/

#ifndef IO_CONTROLS_H_INCLUDED
#define IO_CONTROLS_H_INCLUDED


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "arm_math.h"
#include "general.h"

/********************************************************************
 *
 *                   VARIABLES
 *
 ********************************************************************/
extern int     ict;
extern uint8_t switchPush;
extern uint8_t switchPush1;

/********************************************************************
 *
 *                   DISPLAY FNC
 *
 ********************************************************************/
void TIM3_Config(void);
void GPIO_Switch_Setup();

#ifdef __cplusplus
}
#endif


#endif /* IO_CONTROLS_H_INCLUDED */

/**************************************END OF FILE****/
