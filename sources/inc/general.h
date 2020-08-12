/**
  ******************************************************************************
  * @file    general.h
  * @author  Giuseppe Callipo - IK8YFW - ik8yfw@libero.it
  * @version V2.0.0
  * @date    29-07-2020
  * @brief   IO Routine
  *
  *
  * NOTE: This file is part of RadioDSP MINI project.
  *       See main.c file for additional project informations.
  ******************************************************************************/

#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif

#define SW_VERSION      "1.0.1"

#define N_BLOCKS 8
/*- Size of sampleing block and fft */
#define BLOCK_SIZE		         128
#define BUFFER_SIZE		         BLOCK_SIZE * N_BLOCKS
//#define FFT_SIZE                 128


//#define EXP_SMOOTING

/********************************************************************
 *
 *                   VARIABLES
 *
 ********************************************************************/
/*- Global working buffers */
static float32_t BufferAudioIn     [BUFFER_SIZE];
static float32_t BufferAudioOut    [BUFFER_SIZE];
static float32_t BufferAudioWork    [BUFFER_SIZE];

static float32_t BufferIn     [BLOCK_SIZE];
static float32_t BufferOut    [BLOCK_SIZE];
static float32_t Buffer1      [BLOCK_SIZE];
static float32_t Buffer2      [BLOCK_SIZE];

/********************************************************************
 *
 *                   METHODS
 *
 ********************************************************************/
uint32_t millis(void);

#ifdef __cplusplus
}
#endif

#endif /* GENERAL_H_INCLUDED */

/**************************************END OF FILE****/
