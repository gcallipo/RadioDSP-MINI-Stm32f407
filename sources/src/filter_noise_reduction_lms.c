/**
  ******************************************************************************
  * @file    filter_noise_reduction.c
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V2.0.0
  * @date    25-09-2018
  * @brief   IO Routine
  *
  *
  * NOTE: This file is part of RadioDSP MINI project.
  *       See main.c file for additional project informations.
  * Platform: CortexM4
  ******************************************************************************/
#include "stm32f4xx_conf.h"
#include "io_audio.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include <math.h>

#include <stdio.h>
#include <complex.h>
#include <filter_noise_reduction_lms.h>

int k = 0;
int h= 1;
int init = 0;

// Initialize LMS (DSP Noise reduction) filter
void Init_LMS_NR (int LMS_nr_strength)
{

 // LMS_nr_strength = 20;
  uint16_t  calc_taps = 48;
  float32_t mu_calc;

  LMS_Norm_instance.numTaps = calc_taps;
  LMS_Norm_instance.pCoeffs = LMS_NormCoeff_f32;
  LMS_Norm_instance.pState = LMS_StateF32;

  // Calculate "mu" (convergence rate) from user "DSP Strength" setting.  This needs to be significantly de-linearized to
  // squeeze a wide range of adjustment (e.g. several magnitudes) into a fairly small numerical range.
  mu_calc = LMS_nr_strength;   // get user setting

  // New DSP NR "mu" calculation method as of 0.0.214
  mu_calc /= 2; // scale input value
  mu_calc += 2; // offset zero value
  mu_calc /= 10;  // convert from "bels" to "deci-bels"
  mu_calc = powf(10, mu_calc);  // convert to ratio
  mu_calc = 1 / mu_calc;    // invert to fraction
  LMS_Norm_instance.mu = mu_calc;

  arm_fill_f32(0.0, LMS_nr_delay, 256 + 128);
  arm_fill_f32(0.0, LMS_StateF32, calc_taps + 128);

  // use "canned" init to initialize the filter coefficients
  arm_lms_norm_init_f32(&LMS_Norm_instance, calc_taps, &LMS_NormCoeff_f32[0], &LMS_StateF32[0], mu_calc, 128);

}


void processing_noise_reduction(float* bufferIn, float* bufferOut2){
    LMS_NoiseReduction (128, bufferIn, bufferOut2);
}

void LMS_NoiseReduction(int16_t blockSize, float32_t *nrbuffer, float32_t *outPut)
{
  static ulong    lms1_inbuf = 0, lms1_outbuf = 0;

  // put new data into the delay buffer
  arm_copy_f32(nrbuffer, &LMS_nr_delay[lms1_inbuf], blockSize);
  arm_lms_norm_f32(&LMS_Norm_instance, nrbuffer, &LMS_nr_delay[lms1_outbuf], outPut, LMS_errsig1, blockSize);

  // do noise reduction
  lms1_inbuf += blockSize;  // bump input to the next location in our de-correlation buffer
  lms1_outbuf = lms1_inbuf + blockSize; // advance output to same distance ahead of input
  lms1_inbuf %= 256;
  lms1_outbuf %= 256;
}


/**************************************END OF FILE****/
