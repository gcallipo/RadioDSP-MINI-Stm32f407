# RadioDSP-MINI-Stm32f407
DSP for shortwave receivers with dynamic filtering capability - Pass Band Tuning, AGC and DNR

The dsp mini radio allows you to add a set of filters and audio processing capabilities 
to your rtx or receiver. It allows you to filter, reduce signal noise and control the level 
with an AGC (Automatic Gain Control) to reduce the effects of signal overload.

The dsp, can use any stm32f407 Cortex-M4 equipped board, but the pre-compiled binary was 
optimized for the STM32F407VET6 processor (check the Schematic.jpg file for connection details).

#### Features:

	* AGC on/off
	* BPF AM/SSB filter from 3k to 9k Hz with Low/High cut
	* BPF Digital/CW filter from 500 to 3000 Hz with Low/High cut
	* BPF 500 Hz CW filter with Shift tuning capabilities
	* DNR 4 levels (digital noise reduction)

#### Commands: 

2 Led, 2 Push buttons and Encoder with switch (check Commands.jpg file for details).

#### How it works:

Referring to the ElaborationPath.jpg file, the audio input signal is sampled by an ADC at 48 KHz, 
then it is subjected to DNR and then it is decimated by a factor of 2 to lower the sampling 
frequency to 24 kHz (this is enough to processing up to 12KHz audio signal), after the decimation 
process, a dynamic variable filters will be applied with the possibility of varying the lower or 
upper bandwidth. The output signal is interpolated to increasing sampling frequency at 48 KHz. 
Finally, the signal is processed with an AGC module that sends a feedback control signal to adjust 
the input amplification.

#### How to use it (Operations manual):

Referring to the Controls.jpg file, when the RadioDSP MINI switched on, it is presetted as follows:

* M1 as H-CUT command;
* DNR turn off;
* AGC turn on;
* 9kHz speech filter.

The M1 control allows you to adjust the bandwidth of the selected filter by acting on 
the upper (hi cut) or lower (low-cut) part. To switch from Hi-cut to Low-cut adjustment, 
just press the button once and the selection will be notified by the L1 led lighting up. 
To return to the Hi-cut selection, simply press the button again.
When the narrow CW filter is selected (last selection of the P1 button), the M1 command 
works as SHIFT for the CW filter, and make it possible to center the received CW signal.

By holding down the M1 command and simultaneously pressing the P2 button, 
you enter mode (second function), which will allow you to activate or deactivate the AGC.

In order to activate the DNR function, you can press the P2 button repeatedly, each time 
you press a gradually increasing reduction level will be selected. 
The activity of the DNR function is signaled by the lighting of LED L2.

In order to select the filter modes, push button P1 repeatedly, 
the three filters will be selected: Speech / CW / CWN.

#### INPORTANT NOTE: 

To upload pre compiled firmware (pre-compiled/test3.bin), please referring to the RadioDSP DNR 
project instructions, this project use the same board.

#### Here some testing videos:

RadioDsp Mini test SSB filter & NR
https://www.youtube.com/watch?v=gY6wyI6ZqnY


RadioDsp Mini test cw filter
https://www.youtube.com/watch?v=BbC8seuAkEg


The project is work in progress and may be subject to change without notice.

	The RadioDSP MINI openSource software is released under the license: 
				Common Creative - Attribution 3.0
				
Vy 73 de Giuseppe Callipo - IK8YFW
