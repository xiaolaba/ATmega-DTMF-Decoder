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

//initialize timer1 Compare Match B
void InitTimer1(void)
{
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  // Set Initial Timer counter value
  TCNT1 = 0;
  
  // prescaler set to 8
  // on 16Mhz clock -> timet clock = 16/8 = 2Mhz -> so 0.5us time period x249 = 124,5 us => 8.000 hz !!!
  // ADC sampling will be at 8Khz nice for 4Khz BW
  // WARNING: OCF1B MUST BE CLEARED BY MANUALLY !! TIFR1 =(1<<OCF1B)
  ICR1 = 249;  // <<<== Compare match B occurs on register !!
    
  // Make toggle PB2/OC1B pin on compare match
  TCCR1A |= (1 << COM1B0); // WARNING: MUST BE CLEARED BY MANUALLY !!
  
  // turn on CTC mode compare match B
  TCCR1B |= (1 << WGM13) | (1 << WGM12); // >>>>> warning: if compb no !!
}

//start timer0 with prescaller 8
void StartTimer1(void)
{
  // Set CS12 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);  
}

void StopTimer1(void)
{
  // Zero CS12 and CS10 bits for 64 prescaler
  TCCR1B &= ~(1 << CS11) & ~(1 << CS10);  

  TIMSK1 &=~(1<<OCIE1A);
}
