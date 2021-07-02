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
 
#include "Goertzel.h"

/* Call this routine for every sample. */
int32_t ProcessGoertzel(uint8_t coefficient, uint8_t *ptr)
{
  int32_t Q0 = 0, Q1 = 0, Q2 = 0;
  int32_t magnitudeSquared;

  for (uint8_t idx = 0; idx < BUFLEN; idx++)
  {          
     Q0 = *ptr++ - Q2 + ((Q1*coefficient) >> 7) ; // << warning / 128
     Q2 = Q1;
     Q1 = Q0;        
  }
  /* Do the "basic Goertzel" processing. */
  magnitudeSquared = Q1 * Q1 + Q2 * Q2 - ((Q1 * coefficient) >> 7) * Q2;

  return(magnitudeSquared);
   
} // end of ProcessGoertzel()


int32_t DTMF_Decode( uint8_t *WorkingBuffer )
{
  int32_t MagnitudeSquared;
  
  char CodeFound=NULL;
  static char PreviousCode=NULL;

   
  MagnitudeSquared = ProcessGoertzel( COEF_0697,  WorkingBuffer ); //analyse buffer 1
  
  // FIRST ROW
  if( ProcessGoertzel( COEF_0697,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
  {
    if ( ProcessGoertzel( COEF_1209,  WorkingBuffer ) > GOERTZEL_THRESHOLD ) 
    {  // FOUND '1'
      CodeFound = '1'; 
    } else if ( ProcessGoertzel( COEF_1336,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '2'
      CodeFound = '2'; 
    } else if ( ProcessGoertzel( COEF_1477,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '3'
      CodeFound = '3'; 
    } else if ( ProcessGoertzel( COEF_1633,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND 'A'
      CodeFound = 'A'; 
    } 
  }   // SECOND ROW
  else if ( ProcessGoertzel( COEF_0770,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
  {
    if ( ProcessGoertzel( COEF_1209,  WorkingBuffer ) > GOERTZEL_THRESHOLD ) 
    {  // FOUND '4'
      CodeFound = '4'; 
    } else if ( ProcessGoertzel( COEF_1336,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '5'
      CodeFound = '5'; 
    } else if ( ProcessGoertzel( COEF_1477,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '6'
      CodeFound = '6'; 
    } else if ( ProcessGoertzel( COEF_1633,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND 'B'
      CodeFound = 'B'; 
    } 
  }   // THIRD ROW
  else if ( ProcessGoertzel( COEF_0852,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
  {
    if ( ProcessGoertzel( COEF_1209,  WorkingBuffer ) > GOERTZEL_THRESHOLD ) 
    {  // FOUND '7'
      CodeFound = '7'; 
    } else if ( ProcessGoertzel( COEF_1336,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '8'
      CodeFound = '8'; 
    } else if ( ProcessGoertzel( COEF_1477,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '9'
      CodeFound = '9'; 
    } else if ( ProcessGoertzel( COEF_1633,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND 'C'
      CodeFound = 'C'; 
    } 
  }   // FORTH ROW
  else if ( ProcessGoertzel( COEF_0941,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
  {
    if ( ProcessGoertzel( COEF_1209,  WorkingBuffer ) > GOERTZEL_THRESHOLD ) 
    {  // FOUND '#'
      CodeFound = '#'; 
    } else if ( ProcessGoertzel( COEF_1336,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '0'
      CodeFound = '0'; 
    } else if ( ProcessGoertzel( COEF_1477,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND '*'
      CodeFound = '*'; 
    } else if ( ProcessGoertzel( COEF_1633,  WorkingBuffer ) > GOERTZEL_THRESHOLD )
    {  // FOUND 'D'
      CodeFound = 'D'; 
    } 
  }   // THIRD ROW
 
  if( CodeFound == NULL &&  PreviousCode != NULL ) // Store last code found
  { 
    sprintf(CodeSequence,"%s%c", &CodeSequence[1],PreviousCode);
  
  }
  PreviousCode = CodeFound;

  return(MagnitudeSquared);
   
} // end of DTMF_Decodel()
