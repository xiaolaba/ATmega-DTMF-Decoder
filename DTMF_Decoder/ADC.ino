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

void InitADC()
{  

  // Mux Analog channel 0000 => ADC0
  ADMUX=0;

  // Select Vref=AVcc
  //ADMUX |= (1<<REFS0);

  // 2021-07-01 xiaolaba test, done
  // Select Vref=1.1V internal VREF
  ADMUX |= (1<<REFS1) + (1<<REFS0);

  // Set left adjust result
  ADMUX |= (1<<ADLAR); 
  
  ADCSRA = 0;
  ADCSRB = 0;
  
  // 10-bit successive approximation ADC
  // AD-auto-trigger-enable, AD-interrupt-enabled, AD-converter on, prescaler = 64
  // ADC clock set to 16x10^6/64 = 250Khz => 4us (NOT sutable for 10bit convertion)
  // so single convertion takes 13.5 ADC clock cycles = 54us
  ADCSRA |= (1<<ADATE)|(1<<ADIE)|(1<<ADEN)| (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);

  ADCSRB |= (1<<ACME) | (1<<ADTS2) | (1<<ADTS0); //Setting compare match B as trigger source   
}

void SetADCChannel(uint8_t ADCchannel)
{
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
}

void StartADC(void)
{
   ADCSRA |= (1<<ADSC);
}

//disable ADC
void DisableADC(void)
{
   ADCSRA &= ~((1<<ADEN)|(1<<ADIE));
}

/*** Interrupt routine ADC ready ***/
// ISR takes about 1us !!
ISR(ADC_vect) {  
  //bitSet(PORTB, 4);   // marker high
  TIFR1 =(1<<OCF1B);  //Manually clear OCF1B timer compare flag
  
  // while left adjusted only read ADCH in e uint8_t vector
  // othrwise all 10bits must be read inside an uint16_t type
  //aval = ADCL;        // store lower byte ADC
  //aval += ADCH << 8;  // store higher bytes ADC
  if (flag==1 )
  {
    *bufp++ = ADCH;
    ii++;
    if( ii == 2*BUFLEN)
      {
        bufp = &Signal[0];
        flag = 0; // Set seecond buffer full and reset buffer index
        ii =0;
      }
  }
  else if ( flag==0 )
  {
    *bufp++ = ADCH;
    ii++;
    if( ii == BUFLEN)
      {
        flag = 1; // Set first buffer full
      }
  }
  
  //delayMicroseconds(20);  // slow down for debugging
  //bitClear(PORTB, 4); // marker low
}
