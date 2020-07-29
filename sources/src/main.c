/**
  ******************************************************************************
  * @file    main.c
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V1.0.0
  * @date    29-07-2020
  * @brief   Main routine file
  *
  ******************************************************************************
  *
  *                     THE RADIO DSP MINI v. 1.0 - PROJECT - CORTEX M4
  *
  * This project RadioDSP define a experimental open platform to build active
  * DSP for shortwave receivers with dynamic filtering capability:
  * Pass Band Tuning, AGC and DNR.
  *
  * The project is on GitHub:
  *       https://github.com/gcallipo/RadioDSP-MINI-Stm32f407
  *
  * The RadioDSP openSource software is released under the license:
  *              Common Creative - Attribution 3.0
  ******************************************************************************/

#define EXP_SMOOTING

#include "stm32f4xx_conf.h"
#include "io_audio.h"

#include "arm_math.h"
#include "arm_const_structs.h"
#include <math.h>
#include <stdio.h>
#include <complex.h>

#include <filter_noise_reduction_lms.h>
#include "tm_enc/tm_stm32f4_rotary_encoder.h"

#include "general.h"
#include "dynamicFilters.h"
#include "agc_audio.h"

unsigned short iCurBuffIdx =0;

/*- Swithxch controls */
uint8_t        switchPush=1;
uint8_t        switchPush1=1;
uint8_t        switchPushEn=1;
uint8_t		   ilock = 0;
uint8_t        ilock1 = 0;
uint8_t        ilockEn = 0;
uint8_t        iFilterMode = 0; // 0 = width - 1 = width
uint8_t        iAGCMode = 0;    // 1 = AGC ON - 0 AGC OFF
uint8_t        ilockSwAgc = 0;

/* Rotary encoder data */
static TM_RE_t RE1_Data;
int 		   ictLast =0;

int iNrLevel =   0;
arm_fir_instance_f32 S;
arm_fir_instance_f32 S0;
arm_fir_instance_f32 SC;
arm_fir_instance_f32 SCWN;

float32_t fc1;
float32_t fc2;
int iCmdFlt = 0;
int ict;

#define M 2
#define L 2
#define ND 32
#define NI 48
arm_fir_decimate_instance_f32  D;
arm_fir_interpolate_instance_f32 IN;

#define N 24
float32_t firCoeffs32_H[N];
static float32_t firStateF32_H[BLOCK_SIZE/M + N - 1];

#define NC 16
float32_t firCoeffs32_HC[NC];
static float32_t firStateF32_HC[BLOCK_SIZE + NC - 1];

// Anti aliasis
#define N0 48
float32_t firCoeffs32_A[N0];
static float32_t firStateF32_A[BLOCK_SIZE + N0 - 1];

#define NCWN 24
float32_t firCoeffs32_CWN[NCWN];
static float32_t firStateF32_CWN[BLOCK_SIZE + NCWN - 1];

float32_t firCoeffs32_DEC[ND];
static float32_t firStateF32_DEC[BLOCK_SIZE + ND - 1];

float32_t firCoeffs32_INT[NI];
static float32_t firStateF32_INT[BLOCK_SIZE + NI - 1];

static float32_t BufferInD     [BLOCK_SIZE];
static float32_t BufferOutD    [BLOCK_SIZE];

/*- Bass Band Filter Setup */
/*- filter lower */
float32_t fl ;
/*- filter upper */
float32_t fu ;
/*- filter width */
float32_t fw ;
/*- filter center */
float32_t fc ;

/*- Initial settings */
float32_t cutStep;
float32_t minLowCut;
float32_t maxLowCut;
float32_t minHighCut;
float32_t maxHighCut;

/* Main samples frequencies */
float32_t fs =48000;
float32_t fs2 =24000;

int main(void)
 {
    /** REFERENCE FILTERS
	 MORSE,       ID_BANDPASS,  450.0,    950.0, W_HAMMING
	 MORSEDECODE, ID_BANDPASS,  450.0,    950.0, W_HAMMING
	 LOW_PASS,    ID_LOWPASS,  1000.0,      0.0, W_HAMMING
	 HI_PASS,     ID_HIGHPASS, 1000.0,      0.0, W_HAMMING
	 SSB,         ID_BANDPASS,  300.0,   2700.0, W_HAMMING
	 DUAL,        ID_BANDPASS,  450.0,    950.0, W_HAMMING
	 PASSTHRU,    ID_BANDPASS,   60.0,  20000.0, W_HAMMING
	**/
	 cutStep = 500;
	 minLowCut =  0;
	 maxLowCut =  650;
	 minHighCut = 3000;
	 maxHighCut = 8000;

	 fl = 200;
	 fu = 8000;
	 iFilterMode = 0;
	 iCmdFlt =0;
	 ict=0;
	 ictLast =0;
	 iNrLevel= 0;

	audioFilter(firCoeffs32_A, N0, ID_LOWPASS, W_HAMMING, 24000, 0, fs);
	arm_fir_init_f32(&S0,  N0,   (float32_t *)&firCoeffs32_A[0],   &firStateF32_A[0],   BLOCK_SIZE);

	/* init the decimator */
    audioFilter(firCoeffs32_DEC, ND, ID_LOWPASS, W_HAMMING, 36000, 0, fs);
	arm_fir_decimate_init_f32	(&D, ND, M, firCoeffs32_DEC, firStateF32_DEC, BLOCK_SIZE/M);

	audioFilter(firCoeffs32_INT, NI, ID_LOWPASS, W_HAMMING, 36000, 0, fs);
	arm_fir_interpolate_init_f32	(&IN, L, NI,	firCoeffs32_INT, firStateF32_INT,	BLOCK_SIZE);

    /* Call FIR init function to initialize the instance structure. */
	audioFilter(firCoeffs32_H, N, ID_BANDPASS, W_HAMMING, fl, fu, fs2);
	arm_fir_init_f32(&S,  N,   (float32_t *)&firCoeffs32_H[0],   &firStateF32_H[0],   BLOCK_SIZE/M);

    /* Call FIR init function to initialize the instance structure. */
  	audioFilter(firCoeffs32_HC, NC, ID_BANDPASS, W_BLACKMAN, 400, 4500, fs2);
    arm_fir_init_f32(&SC,  NC,   (float32_t *)&firCoeffs32_HC[0],   &firStateF32_HC[0],   BLOCK_SIZE/M);

    audioFilter(firCoeffs32_CWN, NCWN, ID_BANDPASS, W_BLACKMAN,900, 2000, fs2);
    arm_fir_init_f32(&SCWN,  NCWN,   (float32_t *)&firCoeffs32_CWN[0],   &firStateF32_CWN[0],   BLOCK_SIZE/M);


    /*- Initialize LMS Noise Reduction */
    Init_LMS_NR (10);


    /*- Initialize ADC and DAC */
    ADC1_CH6_DMA_Config(BUFFER_SIZE);
	DAC1_Config(BUFFER_SIZE);

	/*- Initialize push buttons menu */
    GPIO_Switch_Setup();

    // OFF
 	GPIO_SetBits(GPIOE, GPIO_Pin_5);
    // OFF
  	GPIO_SetBits(GPIOE, GPIO_Pin_6);

    /* Initialize Rotary encoder 1, pin A = PB6, pin B = PB7 */
  	TM_RE_Init(&RE1_Data, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);

  	/* Set rotation mode for rotary 1 */
  	TM_RE_SetMode(&RE1_Data, TM_RE_Mode_One);

	/*- Start ADC master timer */
	TIM2_Config();

	/*- Start services timer */
    TIM3_Config();

    /*- Start AGC timer */
    TIM5_Config();

    /*- wait forever ...*/
	while(1)
	{
	    // Infinite loop ...
	}
}

float gain_in = 1.00;
float gain_out = 2.50;
//float mute = 1.00;

/*- IRQ for incoming samples */
void DMA2_Stream0_IRQHandler()
{
	iCurBuffIdx=!iCurBuffIdx;

    /*- converter ADC to float */
	arm_q15_to_float(&ADC_ConvertedValue[iCurBuffIdx][0],&BufferAudioIn[0],BUFFER_SIZE);

//	// Elaborate ...
	for (int i= 0;i< N_BLOCKS; i++ ){

			if (iNrLevel==0){ //BYPASS
				for (int j = 0;j<BLOCK_SIZE; j++){
					BufferAudioWork[j]= BufferAudioIn[j+((BLOCK_SIZE)*i)] *gain_in;
				 }
			}else{
				for (int j = 0;j<BLOCK_SIZE; j++){
					Buffer1[j]= BufferAudioIn[j+((BLOCK_SIZE)*i)] *gain_in;
				}

				/*- noise reduction LMS */
				processing_noise_reduction(&Buffer1[0], &BufferAudioWork[0]);
			}

			// anti alias pre-filtering
			arm_fir_f32(&S0,BufferAudioWork,BufferIn, BLOCK_SIZE);

			// decimate
			arm_fir_decimate_f32	(&D, BufferIn, BufferInD, BLOCK_SIZE);

			 if (iCmdFlt == 0){
				 // AM/SSB Filtering
				 arm_fir_f32(&S,BufferInD,BufferOutD,BLOCK_SIZE/M);
			 } else if (iCmdFlt==1){
				 // CW two stage filtering
				arm_fir_f32(&SC,BufferInD,Buffer1,BLOCK_SIZE/M);
				arm_fir_f32(&S,Buffer1,BufferOutD,BLOCK_SIZE/M);
			 }
			else { // CW N
				arm_fir_f32(&SC,BufferInD,Buffer1,BLOCK_SIZE/M);
				arm_fir_f32(&SCWN,Buffer1,BufferOutD, BLOCK_SIZE/M);
			}

			// interpolate
			arm_fir_interpolate_f32	(&IN, BufferOutD, Buffer1, BLOCK_SIZE/L);

		    for (int j = 0;j<BLOCK_SIZE; j++){
		    	 BufferAudioOut[j+((BLOCK_SIZE)*i)] = Buffer1[j];
			}

		}

	/*- back converter from float to dac */
	arm_float_to_q15(&BufferAudioOut[0],&OutputBuffer[iCurBuffIdx][0],BUFFER_SIZE);

    /*- cleat irq */
    DMA_ClearFlag(DMA2_Stream0, DMA_IT_TC);
}


/* TM EXTI Handler for all EXTI lines */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* Check RE pin 1 */
	if (GPIO_Pin == RE1_Data.GPIO_PIN_A) {
		/* Process data */
		TM_RE_Process(&RE1_Data);
	}
}

void filterLowerCut(){

	if (ict < ictLast){
		fl = (fl-(cutStep/4))<minLowCut? fl : (fl-(cutStep/4));
	}

	if (ict > ictLast){
		fl = (fl+(cutStep/4))>maxLowCut? fl : (fl+(cutStep/4));
	}

	ictLast = ict;

}


void filterHightCut(){

	if (ict < ictLast){
		fu = (fu-cutStep)<minHighCut? fu : (fu-cutStep);
	}

	if (ict > ictLast){
		fu = (fu+cutStep)>maxHighCut? fu : (fu+cutStep);
	}

	ictLast = ict;

}

void updateFilter(){

	if (iCmdFlt != 2){
		if (iFilterMode==1){
			filterLowerCut();
		}else{
			filterHightCut();
		}
	}else{
		// SHIFT CW NARROW
		if (ict < ictLast){
			fl = (fl-(cutStep))<minLowCut? fl : (fl-(cutStep));
		}

		if (ict > ictLast){
			fl = (fl+(cutStep))>maxLowCut? fl : (fl+(cutStep));
		}

		fu = fl + 1100;

		ictLast = ict;
	}
}

/*- Time milliseconds timer handler for AGC */
int          ilockAgc = 0;        // assure single action
void TIM5_IRQHandler()
{

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
    	if (iAGCMode==0){
    		gain_in = 1.0;
    	}else{
    		if (ilockAgc == 0) {
			 ilockAgc = 1;

			 //Processing AGC
			 gain_in = AGC_Processing(&BufferAudioOut[0], gain_in, iNrLevel);


			 ilockAgc = 0;
		  }
		}
    }
}


/*- IRQ for services: display and controls */
 void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		/*- Read switch */
        switchPushEn = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
        switchPush = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
        switchPush1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);

        if (switchPush==0 && switchPushEn==1) {
        	if (ilock==0){
        		iNrLevel = (iNrLevel +1)%5;
        		ilock=1;

        		//TODO: Led NR on/off
        		if (iNrLevel==0){
        		  	// OFF
        		    GPIO_SetBits(GPIOE, GPIO_Pin_6);
        		}else{
        		    // ON
        		    GPIO_ResetBits(GPIOE, GPIO_Pin_6);
        		}

        		if (iNrLevel==1){
        		    /*- Initialize LMS Noise Reduction */
        		    Init_LMS_NR (10);
        		}

        		if (iNrLevel==2){
        		    /*- Initialize LMS Noise Reduction */
        		    Init_LMS_NR (15);
        		}

        		if (iNrLevel==3){
        		    /*- Initialize LMS Noise Reduction */
        			Init_LMS_NR (20);
        		}

        		if (iNrLevel==4){
        		    /*- Initialize LMS Noise Reduction */
        		    Init_LMS_NR (25);
        		}

        	}
        }else {
        	ilock=0;
        }

        /*- Read switch */

         if (switchPush1==0) {
         if (ilock1==0){
        	 iCmdFlt = (iCmdFlt +1)%3;
             ilock1=1;

             if (iCmdFlt==2){
            	// CW - N
            	// ONLY SHIFT
            	cutStep = 100;

            	minLowCut =  0;
            	maxLowCut =  1000;

            	fl = 900;
            	fu = 2000;

                audioFilter(firCoeffs32_CWN, NCWN, ID_BANDPASS, W_BLACKMAN,fl, fu, fs2);

             }
             else
             if (iCmdFlt==1){
				// CW -
				 cutStep = 200;
				 minLowCut =  0;
				 maxLowCut =  1800;
				 minHighCut = 1800;
				 maxHighCut = 5400;

				 fl = 600;
				 fu = 3200;

				 audioFilter(firCoeffs32_H, N, ID_BANDPASS, W_BLACKMAN, fl, fu, fs2);
			 }else if (iCmdFlt==0){
			    // SSB -
				 cutStep = 500;
				 minLowCut =  0;
				 maxLowCut =  2500;
				 minHighCut = 2500;
				 maxHighCut = 10000;

				 fl = 0;
				 fu = 6000;
				 audioFilter(firCoeffs32_H, N, ID_BANDPASS, W_HAMMING, fl, fu, fs2);
			 }

           }
         }else {
             ilock1=0;
         }

         /*- Read switch */
         if (switchPushEn==0 && switchPush==1) {
         if (ilockEn==0){
        	iFilterMode = (iFilterMode == 1)? 0 : 1;
            ilockEn=1;

            // TODO: Led HiCut/LowCut on/off
            if (iFilterMode==0){
            	// OFF
            	GPIO_SetBits(GPIOE, GPIO_Pin_5);
            }else{
            	// ON
            	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
            }

			}
		 }else {
			ilockEn=0;
         }

         // AGC / OFF
         if (switchPush==0 && switchPushEn== 0) {
             if (ilockSwAgc==0){
        	      iAGCMode = (iAGCMode == 1)? 0 : 1;
        	      ilockSwAgc=1;

        	 }
         }else {
        	 ilockSwAgc=0;
        }


         /* Get new encoder rotation */
         TM_RE_Get(&RE1_Data);
         ict = RE1_Data.Absolute;
         if (ict!=ictLast) {

        	 updateFilter();
        	 if (iCmdFlt == 0){
         		 //mute = mute - 0.05;
        		 audioFilter(firCoeffs32_H, N, ID_BANDPASS, W_HAMMING, fl, fu, fs2);
        	 }else if (iCmdFlt == 1){

         		 //mute = mute - 0.05;
        		 audioFilter(firCoeffs32_H, N, ID_BANDPASS, W_BLACKMAN, fl, fu, fs2);
        	 }else if (iCmdFlt == 2){

         		  //mute = mute - 0.05;
        		  audioFilter(firCoeffs32_CWN, NCWN, ID_BANDPASS, W_BLACKMAN,fl, fu, fs2);
        	 }

         }
    }
}


/**************************************END OF FILE****/

