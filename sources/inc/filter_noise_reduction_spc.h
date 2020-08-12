/**
  ******************************************************************************
  * @file    filter_noise_reduction.h
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V1.0.0
  * @date    22-04-2018
  * @brief   Noise Reduction routines
  *
  *
  * NOTE: This file is part of RadioDSP project.
  *       See main.c file for additional project informations.
  ******************************************************************************/

#ifndef FILTER_NOISE_REDUCTION_SPC_H_INCLUDED
#define FILTER_NOISE_REDUCTION_SPC_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif


/********************************************************************
 *
 *                   DENOISE FNC
 *
 ********************************************************************/
void spectral_noise_reduction_init();
void processing_noise_reduction_spc(int ilevel, float* bufferIn, float* bufferOut2);


#ifdef __cplusplus
}
#endif

#endif /* FILTER_NOISE_REDUCTION_SPC_H_INCLUDED */

/**************************************END OF FILE****/
