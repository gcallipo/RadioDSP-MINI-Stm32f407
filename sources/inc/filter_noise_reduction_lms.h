/**
  ******************************************************************************
  * @file    filter_noise_reduction_lms.h
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V1.0.0
  * @date    29-07-2020
  * @brief   Noise Reduction routines
  *
  *
  * NOTE: This file is part of RadioDSP MINI project.
  *       See main.c file for additional project informations.
  ******************************************************************************/

#ifndef FILTER_NOISE_REDUCTION_H_INCLUDED
#define FILTER_NOISE_REDUCTION_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif



// ordinary LMS noise reduction
#define MAX_LMS_TAPS    96
#define MAX_LMS_DELAY   128

float32_t   LMS_errsig1[256 + 10];
arm_lms_norm_instance_f32  LMS_Norm_instance;
arm_lms_instance_f32      LMS_instance;
//
float32_t  LMS_StateF32[MAX_LMS_TAPS + MAX_LMS_DELAY];
float32_t  LMS_NormCoeff_f32[MAX_LMS_TAPS + MAX_LMS_DELAY];
float32_t  LMS_nr_delay[256 + MAX_LMS_DELAY];

//
//
//
///********************************************************************
// *
// *                   VARIABLES
// *
// ********************************************************************/
//
//float32_t FFTBufferOut[BLOCK_SIZE*2];
//float32_t FFTBufferTmp[BLOCK_SIZE*2];
//float32_t FFTBufferMag[BLOCK_SIZE*2];
//float32_t bufferPreOut[BLOCK_SIZE];
//
//

/********************************************************************
 *
 *                   DENOISE FNC
 *
 ********************************************************************/
void processing_noise_reduction(float* bufferIn, float* bufferOut2);
void doToneDetect(float32_t level);
void Init_LMS_NR (int strenght);


#ifdef __cplusplus
}
#endif

#endif /* FILTER_NOISE_REDUCTION_H_INCLUDED */

/**************************************END OF FILE****/
