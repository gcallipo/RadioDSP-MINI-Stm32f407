/**
  ******************************************************************************
  * @file    agc_audio.h
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V1.0.0
  * @date    29-07-2020
  * @brief   AGC Routines
  *
  *
  * NOTE: This file is part of RadioDSP MINI project.
  *       See main.c file for additional project informations.
  * Platform: CortexM4
  ******************************************************************************/


#ifndef AGC_AUDIO_H_INCLUDED
#define AGC_AUDIO_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "arm_math.h"

/********************************************************************
 *
 *                   VARIABLES
 *
 ********************************************************************/

//extern int ict;

//float32_t maxValue;
//uint32_t maxIndex;

//float32_t maxValue_OLD;
//uint32_t  maxIndex_OLD;

//extern float32_t FFTBufferMagPlot[BLOCK_SIZE];
//extern float32_t FFTBufferMagPlot_OLD[BLOCK_SIZE];
//extern float32_t FLTBufferTmp[BLOCK_SIZE*2];

 float AGC_Processing(float* bufferIn, float last_gain,int iNrLevel);

#ifdef __cplusplus
}
#endif


#endif /* AGC_AUDIO_H_INCLUDED */

/**************************************END OF FILE****/
