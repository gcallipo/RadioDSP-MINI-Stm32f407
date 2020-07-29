/**
 *  Defines for your entire project at one place
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */


#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/***********************************************************************

   Customized by Giuseppe Callipo - 2018/2019/2020- ik8yfw

 EXAMPLE DISPLAY DEFAULT PINOUT - Default define for SP3
  LCD BOARD	    DISCOVERY BOARD	                      DESCRIPTION	NOTES
  SDO (MISO)	PF8	Output from LCD for SPI	          Not used, can be left
  LED	        3.3V	                              Backlight
  SCK	        PF7	                                  SPI clock
  SDI (MOSI)	PF9	                                  SPI master output
  D/C	        PD13	                              Data/Command register
  RESET	        PD12	                              Reset LCD	Not used on discovery board
  CS	        PC2	                                  Chip select for SPI
  GND	        GND	                                  Ground
  VCC	        3.3V	                              Positive power supply

************************************************************************/
#define STM32F407VG
/* Put your global defines for all libraries here used in your project */

#endif
