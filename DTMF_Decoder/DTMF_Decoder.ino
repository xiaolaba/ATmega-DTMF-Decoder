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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                             CPU Pin Definitions
//
//  Connected encoder and TWI OLED
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Arduino    PCB                                                                              PCB       Arduino
//  -------    ------                               ___________                                 ------    ----------
//             RESET-----------(PCINT14/RESET) PC6 |1        28| PC5 (ADC5/SCL/PCINT13)---------UNUSED    Dig19 Ana5 (SCL)
//  Dig0 (RX)  UNUSED------------(PCINT16/RXD) PD0 |2  o   i 27| PC4 (ADC4/SDA/PCINT12)---------UNUSED    Dig18 Ana4 (SDA)
//  Dig1 (TX)  UNUSED------------(PCINT17/TXD) PD1 |3  o   i 26| PC3 (ADC3/PCINT11)-------------UNUSED    Dig17 Ana3
//  Dig2       ENC_DWN----------(PCINT18/INT0) PD2 |4  o   i 25| PC2 (ADC2/PCINT10)-------------UNUSED    Dig16 Ana2
//  Dig3 (PWM) ENC_UP------(PCINT19/OC2B/INT1) PD3 |5  o   i 24| PC1 (ADC1/PCINT9)--------------UNUSED    Dig15 Ana1
//  Dig4       ENC_SW---------(PCINT20/XCK/T0) PD4 |6  o   i 23| PC0 (ADC0/PCINT8)--------------UNUSED    Dig14 Ana0
//             VCC-----------------------------VCC |7  o   o 22| GND ---------------------------AGND
//             GND-----------------------------GND |8  o   o 21| AREF---------------------------AREF
//             XTAL1------(PCINT6/XTAL1/TOSC1) PB6 |9  i   o 20| AVCC---------------------------AVCC
//             XTAL2------(PCINT7/XTAL2/TOSC2) PB7 |10 i   o 19| PB5 (SCK/PCINT5)---------------UNUSED    Dig13
//  Dig5 (PWM) UNUSED--------(PCINT21/OC0B/T1) PD5 |11 o   o 18| PB4 (MISO/PCINT4)--------------UNUSED    Dig12
//  Dig6 (PWM) UNUSED------(PCINT22/OC0A/AIN0) PD6 |12 o   o 17| PB3 (MOSI/OC2A/PCINT3)---------UNUSED    Dig11 (PWM)
//  Dig7       UNUSED-----------(PCINT23/AIN1) PD7 |13 o   o 16| PB2 (SS/OC1B/PCINT2)  ---------UNUSED    Dig10 (PWM)
//  Dig8       UNUSED-------(PCINT0/CLKO/ICP1) PB0 |14 o   o 15| PB1 (OC1A/PCINT1)--------------UNUSED    Dig9  (PWM)
//                                                  -----------
//                                                   ATmega328
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "Goertzel.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // Fast I2C / TWI 

volatile uint8_t flag=0;
uint8_t B1ready=0; 

static char dtostrfbuffer[80];

int marker1 = 11;   // marker output pin PB4
int marker2 = 12;   // marker output pin PB5
int freqOK  = 13;

void setup() {

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  u8g.setFont(u8g_font_unifont);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  //Serial.begin(38400);

  Serial.begin(115200);
  Serial.print("\nXiaolaba test, DTMF decoder\n");
  Serial.print("\nUses internal 1.1V ADC reference\n");

    
  pinMode(9, OUTPUT);      // OC1A = output
  pinMode(10, OUTPUT);     // OC1B = output
  pinMode(marker1, OUTPUT); // pin = output
  pinMode(marker2, OUTPUT); // pin = output
  pinMode(freqOK, OUTPUT); // pin = output
  DIDR0 = 0x3F;            // digital inputs disabled
  
  // initialize ADC
  InitADC();
  
  //initialize timer1
  InitTimer1();
  
  SetADCChannel(0);
  StartADC();
  //start timer1
  StartTimer1();
  
  sei();                   // set interrupt flag

  u8g.firstPage();
  do {
    u8g.drawStr( 0, 14, "Syncing DCF77");
    u8g.drawStr( 0, 30, "..............");
    u8g.drawStr( 0, 45, "Please wait...");
    u8g.drawStr( 0, 62, "2 minutes to go");
  } while( u8g.nextPage() ); 
  
} // end setup()

void loop() {
    char Line[64];
    uint8_t index = 0;
    
    int32_t magnitudeSquared;

    if( flag==1 && B1ready==0 )  // Buffer 1 ready
    {
         B1ready=1;
         /* Process the samples */
         bitSet(PORTB, 3);   // marker high
         magnitudeSquared = DTMF_Decode(  (uint8_t *) &Signal[0] );
         //sprintf(Line, "1rel mag^2=%ld\r\n", magnitudeSquared ); // WARNING write buf len
         //sprintf(Line, "1rel mag^2=%s\r\n", dtostrf(magnitudeSquared, 16, 2, dtostrfbuffer)); // WARNING write buf len
         //Serial.print(Line);
         
         bitClear(PORTB, 3); // marker low 
         //if( magnitudeSquared > 10000000 )
         //   bitSet(PORTB, 5);
         //else
         //   bitClear(PORTB, 5);         
    } 
    if( flag==0 && B1ready==1 ) // Buffer 0 ready
    {
          B1ready=0;
          /* Process the samples */
          bitSet(PORTB, 4);   // marker high
          magnitudeSquared = DTMF_Decode(  (uint8_t *) &Signal[BUFLEN] ); //analyse buffer 2
          //sprintf(Line, "2rel mag^2=%ld\r\n", magnitudeSquared ); // WARNING write buf len
          //sprintf(Line, "2rel mag^2=%s\r\n", dtostrf(magnitudeSquared, 16, 2, dtostrfbuffer) );
          //Serial.print(Line);

          bitClear(PORTB, 4); // marker low
          //if( magnitudeSquared > 10000000 )
          //  bitSet(PORTB, 5);
          //else
          //  bitClear(PORTB, 5);
    }
    
    if ( !strcmp("1234", CodeSequence) ) // if equal
    {
       // DO WHATEVER YOU WANT
       bitSet(PORTB, 5);
       sprintf(Line, "Found %s\r\n", CodeSequence );
       Serial.print(Line);
       strcpy(CodeSequence,"0000");
    } 
    if ( !strcmp("1111", CodeSequence) ) // if equal
    {
       // DO WHATEVER YOU WANT
       bitClear(PORTB, 5);
       sprintf(Line, "Found %s\r\n", CodeSequence );
       Serial.print(Line);
       strcpy(CodeSequence,"0000");
    }

    // 2021-07-01 xiaolaba test, done
    if ( !strcmp("9591", CodeSequence) ) // if equal
    {
       // DO WHATEVER YOU WANT
       bitClear(PORTB, 5);
       sprintf(Line, "Found %s\r\n", CodeSequence );
       Serial.print(Line);
       Serial.print(", hi xiaolaba\n");      
       strcpy(CodeSequence,"0000");
    }


   // picture loop
    u8g.firstPage();  
    do {
      // draw_4("        paperino",pippo,"IW2ESL",pluto); // drow 4 lines
      draw_1(CodeSequence);    // single line draw
    } while( u8g.nextPage() ); 
    
    //sprintf(Line, "Found %s\r\n", CodeSequence );
    //   Serial.print(Line);

} // END LOOP

void draw_4(char *stringPtr1,char *stringPtr2,char *stringPtr3,char *stringPtr4) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont); // se lo devo cambiare altrimenti lo faccio una sola volta nel setup
  //u8g.setFont(u8g_font_osb18);
  u8g.drawStr( 0, 14, stringPtr1);
  
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 30, stringPtr2);
  
  u8g.drawStr( 0, 45, stringPtr3);
  u8g.drawStr( 0, 62, stringPtr4);
}

void draw_1(char *stringPtr1) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont); // se lo devo cambiare altrimenti lo faccio una sola volta nel setup
  //u8g.setFont(u8g_font_osb18);
  //u8g.drawStr( 0, 14, stringPtr1);
  
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 30, stringPtr1);
  
  //u8g.drawStr( 0, 45, stringPtr3);
  //u8g.drawStr( 0, 62, stringPtr4);
}

void draw_2(char *stringPtr1, char *stringPtr2) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont); // se lo devo cambiare altrimenti lo faccio una sola volta nel setup
  //u8g.setFont(u8g_font_osb18);
  //u8g.drawStr( 0, 14, stringPtr1);
  
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 30, stringPtr1);
  
  u8g.drawStr( 0, 45, stringPtr2);
  
  //u8g.drawStr( 0, 62, stringPtr4);
}
