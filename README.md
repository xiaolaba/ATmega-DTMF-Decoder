# ATmega328-DTMF-Decoder  
copy http://qrp.valtellina.net/home/atmega328-DTMF-decoder, changed a bit for quick testing, no fork  
author's git https://github.com/qrpfun/DTMF_Decoder  


### no uses and did not have the LCD display, but required to install library of ug8lib in order to complie the code



### changed to uses 1.1V internal AREF, file ADC.ino

```
  // Select Vref=AVcc
  //ADMUX |= (1<<REFS0);
  //
  // 2021-07-01 xiaolaba test, done
  // Select Vref=1.1V internal VREF
  ADMUX |= (1<<REFS1) + (1<<REFS0);
  ```
  
  
  ### debug easy a bit, changed DTMF_decoder.ino  
  
  ```
    // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  //Serial.begin(38400);

  Serial.begin(115200);
  Serial.print("\nXiaolaba test, DTMF decoder\n");
  Serial.print("\nUses internal 1.1V ADC reference\n");

  ```
  
  
  
  ```
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
  ```
  
  
### prepare the own DTMF wave file, to play by PC and sound card output
https://www.audiocheck.net/audiocheck_dtmf.php
  
### how to test
connect arduino NANO, build the simple circuit  
PC / earphone output, L or R connect to 0.47uF capacitor as single input  
play the wave file, [dtmf_1234____1111_____9591____4567____9999.wav](dtmf_1234____1111_____9591____4567____9999.wav)  
open ARduino IDE and termial, see the output  
[DTMF_Decoder_test_done.JPG](DTMF_Decoder_test_done.JPG)  
  
  
  
