/**
  ******************************************************************************
  * @file    agc_audio.c
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
#include "stm32f4xx_conf.h"
#include <agc_audio.h>
#include "general.h"


float        AGCgain = 1;           // Initial AGC gain. 1 = unity gain, 32768.0 max, 0.00004 min
float        AGCMAX = 1.1;
float        AGCnomVal = 0.020;            // Nominal Output (32768 max)
int 	     AGCattack = 2;      // AGC Hang time (milliseconds) before reducing gain
int          AGChang   = 40;     // AGC Hang time before increasing gain
float        AGCslope  = 1.005;   // Relative gain change
static float sample[20];           // A ringbuffer of samples (has to be larger than AGCattack)
static int hangtimer=0;

float AGC_Processing(float* bufferIn, float last_gain, int iNrLevel){

	float gain_in =last_gain;

	float32_t rms_Value,temp, samp;
	static int ii=0;
	int k=0;
	uint32_t idx;

	//process sample data to find max
	arm_max_f32(bufferIn, BLOCK_SIZE, &samp , & idx);

	// Highest sample within 1 millisecond
	// AGC: Collect current 1ms peak at output and feed to a ringbuffer
	sample[ii] = samp;
	ii++;

	if(ii >= AGCattack) {
		ii = 0;
	}

	// init variables
	k =0;
	temp=0;

	// Check if we need to reduce gain
	for(int j=0;j<AGCattack;j++)
	{
		if (sample[j] > AGCnomVal){
		k++;
		}
	}

	// We need to reduce gain
	if ((k == AGCattack) || ((k>0) && (hangtimer>=AGChang)))  // Activate AGCattack
	{
		// find largest value
		temp = 0;
		arm_max_f32(sample, AGCattack, &temp , & idx);

		// Instant reduction to appropriate value
		AGCgain = AGCgain * AGCnomVal/temp;

		// Reset hang timer
		hangtimer = 0;
	}
	else
	if(samp < AGCnomVal) {
			// Increment hangtimer while level is lower than nominal
			hangtimer++;
	}

	if (hangtimer >= AGChang)  // We need to ramp up the gain
	{
		AGCgain = (AGCgain * AGCslope);
	}

	// Adjust AGC gain
	gain_in += (AGCgain - gain_in) * 0.005;

	// add a small gain to compensate loss in nr (only NLMS
	if (iNrLevel>0 && iNrLevel<3){
		gain_in *= 1.05;
	}

	// limit the gain and reset gain
	if (gain_in < 0.5 ) {
		gain_in = 0.5;
		AGCgain = 1;

	}
	if (gain_in > AGCMAX) gain_in=AGCMAX;

	return gain_in;
}



/**************************************END OF FILE****/
