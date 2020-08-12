/**
  ******************************************************************************
  * @file    filter_noise_reduction_.h

  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V2.0.0
  * @date    25-09-2018
  * @brief   IO Routine
  *
  *
  * NOTE: This file is part of RadioDSP project.
  *       See main.c file for additional project informations.
  *      // spectral weighting noise reduction
		// based on:
		// Kim, H.-G. & D. Ruwisch (2002): Speech enhancement in non-stationary noise environments. – 7th International Conference on Spoken Language Processing [ICSLP 2002]. – ISCA Archive (http://www.isca-speech.org/archive)
  *
  *
  * Platform: CortexM4
  ******************************************************************************/
#include "stm32f4xx_conf.h"
#include "io_audio.h"
#include "arm_math.h"
#include "math_helper.h"
#include "arm_const_structs.h"
#include <math.h>

#include <stdio.h>
#include <complex.h>
#include "filter_noise_reduction_spc.h"

#define NR_FFT_L   512

float32_t NR_FFT_buffer[NR_FFT_L];
static float32_t NR_last_iFFT_result [NR_FFT_L / 2];
static float32_t NR_last_sample_buffer_L[NR_FFT_L / 2];

void spectral_noise_reduction_init()
{


}

//***********************************************

void processing_noise_reduction_spc(int ilevel, float* bufferIn, float* bufferOut2){

    // Filtro 2
	processing_noise_reduction_spc_sub_3(ilevel, bufferIn, bufferOut2);

}

// array of squareroot von Hann coefficients [256]
const float32_t sqrtHann[256] = {0, 0.01231966, 0.024637449, 0.036951499, 0.049259941, 0.061560906, 0.073852527,
							     0.086132939, 0.098400278, 0.110652682, 0.122888291, 0.135105247, 0.147301698,
								 0.159475791, 0.171625679, 0.183749518, 0.195845467, 0.207911691, 0.219946358,
								 0.231947641, 0.24391372, 0.255842778, 0.267733003, 0.279582593, 0.291389747,
								 0.303152674, 0.314869589, 0.326538713, 0.338158275, 0.349726511, 0.361241666,
								 0.372701992, 0.384105749, 0.395451207, 0.406736643, 0.417960345, 0.429120609,
								 0.440215741, 0.451244057, 0.462203884, 0.473093557, 0.483911424, 0.494655843,
								 0.505325184, 0.515917826, 0.526432163, 0.536866598, 0.547219547, 0.557489439,
								 0.567674716, 0.577773831, 0.587785252, 0.597707459, 0.607538946, 0.617278221,
								 0.626923806, 0.636474236, 0.645928062, 0.65528385, 0.664540179, 0.673695644,
								 0.682748855, 0.691698439, 0.700543038, 0.709281308, 0.717911923, 0.726433574,
								 0.734844967, 0.743144825, 0.75133189, 0.759404917, 0.767362681, 0.775203976,
								 0.78292761, 0.790532412, 0.798017227, 0.805380919, 0.812622371, 0.819740483,
								 0.826734175, 0.833602385, 0.840344072, 0.846958211, 0.853443799, 0.859799851,
								 0.866025404, 0.872119511, 0.878081248, 0.88390971, 0.889604013, 0.895163291,
								 0.900586702, 0.905873422, 0.911022649, 0.916033601, 0.920905518, 0.92563766,
								 0.930229309, 0.934679767, 0.938988361, 0.943154434, 0.947177357, 0.951056516,
								 0.954791325, 0.958381215, 0.961825643, 0.965124085, 0.968276041, 0.971281032,
								 0.974138602, 0.976848318, 0.979409768, 0.981822563, 0.984086337, 0.986200747,
								 0.988165472, 0.989980213, 0.991644696, 0.993158666, 0.994521895, 0.995734176,
								 0.996795325, 0.99770518, 0.998463604, 0.999070481, 0.99952572, 0.99982925,
								 0.999981027, 0.999981027, 0.99982925, 0.99952572, 0.999070481, 0.998463604,
								 0.99770518, 0.996795325, 0.995734176, 0.994521895, 0.993158666, 0.991644696,
								 0.989980213, 0.988165472, 0.986200747, 0.984086337, 0.981822563, 0.979409768,
								 0.976848318, 0.974138602, 0.971281032, 0.968276041, 0.965124085, 0.961825643,
								 0.958381215, 0.954791325, 0.951056516, 0.947177357, 0.943154434, 0.938988361,
								 0.934679767, 0.930229309, 0.92563766, 0.920905518, 0.916033601, 0.911022649,
								 0.905873422, 0.900586702, 0.895163291, 0.889604013, 0.88390971, 0.878081248,
								 0.872119511, 0.866025404, 0.859799851, 0.853443799, 0.846958211, 0.840344072,
								 0.833602385, 0.826734175, 0.819740483, 0.812622371, 0.805380919, 0.798017227,
								 0.790532412, 0.78292761, 0.775203976, 0.767362681, 0.759404917, 0.75133189,
								 0.743144825, 0.734844967, 0.726433574, 0.717911923, 0.709281308, 0.700543038,
								 0.691698439, 0.682748855, 0.673695644, 0.664540179, 0.65528385, 0.645928062,
								 0.636474236, 0.626923806, 0.617278221, 0.607538946, 0.597707459, 0.587785252,
								 0.577773831, 0.567674716, 0.557489439, 0.547219547, 0.536866598, 0.526432163,
								 0.515917826, 0.505325184, 0.494655843, 0.483911424, 0.473093557, 0.462203884,
								 0.451244057, 0.440215741, 0.429120609, 0.417960345, 0.406736643, 0.395451207,
								 0.384105749, 0.372701992, 0.361241666, 0.349726511, 0.338158275, 0.326538713,
								 0.314869589, 0.303152674, 0.291389747, 0.279582593, 0.267733003, 0.255842778,
								 0.24391372, 0.231947641, 0.219946358, 0.207911691, 0.195845467, 0.183749518,
								 0.171625679, 0.159475791, 0.147301698, 0.135105247, 0.122888291, 0.110652682,
								 0.098400278, 0.086132939, 0.073852527, 0.061560906, 0.049259941, 0.036951499,
								 0.024637449, 0.01231966, 0};


/*- working variables */
static int  energyChkPeriod_idx = 0;
float       TH_VALUE            = 0.0;
float       POW_VALUE           = 0.0;
float       POW_VALUE_min       = 0.0;
float       level               = 0.5;
float       mediumValue         = 0;
float32_t   Threshold_max       = 0.0;
int         indexVAD            = 500;
static float   POW_VALUE_max    = 0.0;
float32_t FFTBufferMag[NR_FFT_L/2];

void processing_noise_reduction_spc_sub_3(int ilevel, float* bufferIn, float* bufferOut2)
{

  int j=0;

	  switch(ilevel){
		  case 0: {
					break;
				  }
		  case 1: {
			  level = 1;
					break;
				  }
		  case 2: {
			  level = 4;
					break;
				  }
	  }

		 // fill first half of FFT_buffer with last events audio samples
		 for (int i = 0; i < NR_FFT_L / 4; i++)
		 {
		   NR_FFT_buffer[i * 2] = NR_last_sample_buffer_L[i]; // real
		   NR_FFT_buffer[(i * 2) + 1] = 0.0; // imaginary
		 }

		 // copy recent samples to last_sample_buffer for next time!
		 for (int i = 0; i < NR_FFT_L  / 4; i++)
		 {
		   NR_last_sample_buffer_L [i] = bufferIn[i];
		 }

		 // now fill recent audio samples into second half of FFT_buffer
		 for (int i = 0; i < NR_FFT_L / 4; i++)
		 {
		   NR_FFT_buffer[(NR_FFT_L/2) + (i * 2)] = bufferIn[i]; // real
		   NR_FFT_buffer[(NR_FFT_L/2) + (i * 2) + 1] = 0.0;
		 }

	     // perform windowing on 256 real samples in the NR_FFT_buffer
	     for (int idx = 0; idx < NR_FFT_L/2; idx++)
		 { // sqrt Hann window
		   NR_FFT_buffer[idx * 2] *= sqrtHann[idx];
		 }

	    // NR_FFT
	    /* Process the data through the CFFT/CIFFT module */
	    arm_cfft_f32(&arm_cfft_sR_f32_len256, NR_FFT_buffer, 0, 1);

	    //*********************************************************************************************************
	    //****************************        SPECTRAL SUBT               *****************************************

	    /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
	    arm_cmplx_mag_f32(NR_FFT_buffer, FFTBufferMag, NR_FFT_L/2);

	    /*- Voice Activity Detector */
	    /* Evaluate Noise  */
	    energyChkPeriod_idx++;
	    if (energyChkPeriod_idx>=indexVAD){
	        POW_VALUE_max = 0;
	        energyChkPeriod_idx=0;
	    }
	    POW_VALUE=0;
	    float TH_VALUE_new = 0.0;
	    int low = 2; int high =(NR_FFT_L/4);
	    for (j =low; j< high; j++)
	    {
	       TH_VALUE_new += FFTBufferMag[j];
	    }

	        // Only for internal voice bins ...
	        for (j =2; j< 40; j++)
	        {     // Evaluate power spectrum ...
	              POW_VALUE += FFTBufferMag[j]*FFTBufferMag[j];
	        }
	        // ... and update the current max values ...
	        if (POW_VALUE>POW_VALUE_max){
	               POW_VALUE_max = POW_VALUE;
	        }


	    mediumValue = (TH_VALUE_new/(high-low)); // MEDIUM
	    float TH_VALUE_low = (0.8+(level/10)) * mediumValue;
	    TH_VALUE = level*mediumValue*(1.0-(POW_VALUE_max*10));
	    if (TH_VALUE<TH_VALUE_low){
	            TH_VALUE = TH_VALUE_low;
	    }

		/* SUBCTRACTION & Rebuild the fft complex signal from subctracted magnitude & original signal phase */
		int k =1;
		int s= 0;
		for (j =0; j< NR_FFT_L/2; j++)
		{
			 /*- Subtract threshold noise */
			 if (FFTBufferMag[j]<=TH_VALUE){
				FFTBufferMag[j] = 0;
			 }else{
				FFTBufferMag[j] =  FFTBufferMag[j]- TH_VALUE;
			 }

              // Rebuild ..
			  float32_t r1 = NR_FFT_buffer[s]; float32_t i1 = NR_FFT_buffer[k];
			  complex float z= r1 + i1*I;
			  float32_t phi = cargf(z);
			  float32_t r2 = FFTBufferMag[j]*arm_cos_f32(phi);
			  float32_t i2 = FFTBufferMag[j]*arm_sin_f32(phi);

			  NR_FFT_buffer[s] = r2;
			  NR_FFT_buffer[k] = i2;
			  s=s+2;
			  k=s+1;
	     }

		 //*********************************************************************************************************

	    // NR_iFFT
        // perform iFFT (in-place)
        // Compute the inverse FFT ...
        arm_cfft_f32(&arm_cfft_sR_f32_len256, NR_FFT_buffer,1,1);

        // perform windowing on 256 real samples in the NR_FFT_buffer
        for (int idx = 0; idx < NR_FFT_L/2; idx++)
        { // sqrt Hann window
          NR_FFT_buffer[idx * 2] *= sqrtHann[idx];
        }

        // Rebuild and clean the signal if need - ed overlap and add ( 50 % overlapped )
        for (int i = 0; i < NR_FFT_L/4; i++)
        {
        	// take real part of first half of current iFFT result and add to 2nd half of last iFFT_result
        	bufferOut2[i] = NR_FFT_buffer[i * 2] + NR_last_iFFT_result[i];
        }

        // Save half buffer for the next time ...
        for (int i = 0; i < NR_FFT_L/4; i++)
        {
            NR_last_iFFT_result[i] = NR_FFT_buffer[(NR_FFT_L/2) + (i * 2)];
        }


} // end of spectral weight subcraction algorithm

