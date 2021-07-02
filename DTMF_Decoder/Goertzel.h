/*
 *   Copyright (C) 2015,2016 by Giampiero Raschetti IW2ESL
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 
  2015/12/25 - Rev 1.0 Arduino DTMF Decoder               
 
 */
 
// ensure this library description is only included once
#ifndef Goertzel_h
#define Goertzel_h

// include types & constants of Wiring core API
#include "Arduino.h"

#define BUFLEN 205

#define SAMPLING_RATE 8000.0  //8kHz

#define DUEPI 6.2831853071795865
#define PI 3.14159266

#define COEF_0697  218
#define COEF_0770  210
#define COEF_0852  200
#define COEF_0941  189

#define COEF_1209  149
#define COEF_1336  127
#define COEF_1477  102
#define COEF_1633   73

#define PRESENT 1
#define NOT_PRESENT 0
#define CODE_LEN 4
#define GOERTZEL_THRESHOLD 10000000

// HERE GLOBAL VARIABLES

volatile uint8_t Signal[2*BUFLEN];
volatile uint8_t *bufp = Signal;
volatile uint16_t ii=0;

uint8_t CodeStatus = NOT_PRESENT;
char CodeSequence[] = "0000";  // PAY ATTENTION TO LENGHT

#endif
