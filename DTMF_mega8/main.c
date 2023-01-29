

//
// DTMF 解码程序
// author: hendry
// date: 2008/11
//

// original author,
//https://www.amobbs.com/thread-3213853-1-1.html

// learning note, xiaolaba 2013
//https://xiaolaba.wordpress.com/2013/04/19/dtmf-%e9%9b%99%e9%9f%b3%e8%be%a8%e8%99%9f-%e8%a7%a3%e7%a2%bc-%e5%88%b0%e5%ba%95%e7%94%9a%e9%ba%bc%e6%98%af-%e9%a0%bb%e5%9f%9f-frequency-domain-%e5%92%8c-%e6%99%82%e5%9f%9f-time-domain/
// port to aur-gcc, and fix the code
#include <string.h> // c standard library, used for memset()
#include <stdlib.h>	// labs() need this header file



unsigned char s_dtmf_p;
long      s_dtmf_real[8];
long      s_dtmf_image[8];

const unsigned char g_sincos_len = 50;
const signed char g_cos_table[400] = {
(signed char)0x7E,(signed char)0x3A,(signed char)0xB7,(signed char)0x83,(signed char)0xD7,(signed char)0x57,(signed char)0x79,(signed char)0x18,(signed char)0x9D,(signed char)0x8D,(signed char)0xFA,(signed char)0x6D,(signed char)0x6A,(signed char)0xF4,(signed char)0x8B,(signed char)0xA1,(signed char)0x1E,(signed char)0x7A,(signed char)0x52,(signed char)0xD1,(signed char)0x83,(signed char)0xBC,(signed char)0x3F,(signed char)0x7E,(signed char)0x34,(signed char)0xB2,(signed char)0x84,(signed char)0xDD,(signed char)0x5B,(signed char)0x77,(signed char)0x12,(signed char)0x99,(signed char)0x90,(signed char)0x00,(signed char)0x70,(signed char)0x67,(signed char)0xEE,(signed char)0x89,(signed char)0xA5,(signed char)0x24,
(signed char)0x7C,(signed char)0x4E,(signed char)0xCB,(signed char)0x82,(signed char)0xC1,(signed char)0x44,(signed char)0x7D,(signed char)0x2F,(signed char)0xAD,(signed char)0x86,(signed char)0x7E,(signed char)0x2D,(signed char)0xA1,(signed char)0x91,(signed char)0x10,(signed char)0x7B,(signed char)0x47,(signed char)0xB8,(signed char)0x86,(signed char)0xF2,(signed char)0x70,(signed char)0x5D,(signed char)0xD2,(signed char)0x82,(signed char)0xD5,(signed char)0x60,(signed char)0x6E,(signed char)0xEE,(signed char)0x85,(signed char)0xBB,(signed char)0x4A,(signed char)0x7A,(signed char)0x0C,(signed char)0x8F,(signed char)0xA4,(signed char)0x30,(signed char)0x7E,(signed char)0x29,(signed char)0x9F,(signed char)0x93,
(signed char)0x14,(signed char)0x7B,(signed char)0x44,(signed char)0xB4,(signed char)0x87,(signed char)0xF6,(signed char)0x72,(signed char)0x5A,(signed char)0xCE,(signed char)0x82,(signed char)0xD9,(signed char)0x62,(signed char)0x6C,(signed char)0xEA,(signed char)0x84,(signed char)0xBE,(signed char)0x4D,(signed char)0x78,(signed char)0x08,(signed char)0x8D,(signed char)0x7E,(signed char)0x1D,(signed char)0x8F,(signed char)0xAF,(signed char)0x4B,(signed char)0x74,(signed char)0xEA,(signed char)0x82,(signed char)0xDC,(signed char)0x6D,(signed char)0x56,(signed char)0xBA,(signed char)0x8A,(signed char)0x0F,(signed char)0x7D,(signed char)0x2B,(signed char)0x96,(signed char)0xA5,(signed char)0x3F,(signed char)0x79,
(signed char)0xF8,(signed char)0x84,(signed char)0xCF,(signed char)0x65,(signed char)0x60,(signed char)0xC7,(signed char)0x86,(signed char)0x01,(signed char)0x7B,(signed char)0x38,(signed char)0x9F,(signed char)0x9B,(signed char)0x33,(signed char)0x7C,(signed char)0x06,(signed char)0x87,(signed char)0xC2,(signed char)0x5C,(signed char)0x69,(signed char)0xD4,(signed char)0x83,(signed char)0xF3,(signed char)0x77,(signed char)0x44,(signed char)0xA9,(signed char)0x94,(signed char)0x25,(signed char)0x7E,(signed char)0x14,(signed char)0x8C,(signed char)0x7E,(signed char)0x0C,(signed char)0x84,(signed char)0xDD,(signed char)0x75,(signed char)0x38,(signed char)0x95,(signed char)0xB4,(signed char)0x5D,(signed char)0x5D,
(signed char)0xB4,(signed char)0x95,(signed char)0x38,(signed char)0x76,(signed char)0xDE,(signed char)0x84,(signed char)0x0B,(signed char)0x7E,(signed char)0x0C,(signed char)0x84,(signed char)0xDD,(signed char)0x75,(signed char)0x39,(signed char)0x95,(signed char)0xB3,(signed char)0x5D,(signed char)0x5E,(signed char)0xB5,(signed char)0x94,(signed char)0x37,(signed char)0x76,(signed char)0xDF,(signed char)0x84,(signed char)0x0A,(signed char)0x7E,(signed char)0x0D,(signed char)0x84,(signed char)0xDC,(signed char)0x75,(signed char)0x39,(signed char)0x96,(signed char)0xB3,(signed char)0x5C,(signed char)0x5E,(signed char)0xB5,(signed char)0x94,(signed char)0x37,(signed char)0x76,(signed char)0xDF,(signed char)0x84,
(signed char)0x7E,(signed char)0xD7,(signed char)0x9C,(signed char)0x69,(signed char)0x20,(signed char)0x82,(signed char)0x31,(signed char)0x5E,(signed char)0x92,(signed char)0xE9,(signed char)0x7D,(signed char)0xC7,(signed char)0xA8,(signed char)0x72,(signed char)0x0F,(signed char)0x85,(signed char)0x41,(signed char)0x51,(signed char)0x8B,(signed char)0xFA,(signed char)0x79,(signed char)0xB8,(signed char)0xB6,(signed char)0x78,(signed char)0xFD,(signed char)0x8A,(signed char)0x4F,(signed char)0x43,(signed char)0x85,(signed char)0x0C,(signed char)0x73,(signed char)0xAA,(signed char)0xC5,(signed char)0x7C,(signed char)0xEB,(signed char)0x91,(signed char)0x5C,(signed char)0x33,(signed char)0x83,(signed char)0x1E,
(signed char)0x6A,(signed char)0x9E,(signed char)0xD5,(signed char)0x7E,(signed char)0xDA,(signed char)0x9B,(signed char)0x68,(signed char)0x23,(signed char)0x82,(signed char)0x2F,(signed char)0x7E,(signed char)0xC1,(signed char)0xC2,(signed char)0x7E,(signed char)0xBF,(signed char)0xC3,(signed char)0x7E,(signed char)0xBE,(signed char)0xC5,(signed char)0x7E,(signed char)0xBC,(signed char)0xC6,(signed char)0x7E,(signed char)0xBB,(signed char)0xC8,(signed char)0x7E,(signed char)0xBA,(signed char)0xC9,(signed char)0x7E,(signed char)0xB9,(signed char)0xCA,(signed char)0x7E,(signed char)0xB7,(signed char)0xCC,(signed char)0x7D,(signed char)0xB6,(signed char)0xCD,(signed char)0x7D,(signed char)0xB5,(signed char)0xCF,
(signed char)0x7D,(signed char)0xB3,(signed char)0xD0,(signed char)0x7D,(signed char)0xB2,(signed char)0xD2,(signed char)0x7D,(signed char)0xB1,(signed char)0xD3,(signed char)0x7C,(signed char)0xB0,(signed char)0xD5,(signed char)0x7C,(signed char)0xAE,(signed char)0xD6,(signed char)0x7C,(signed char)0xAD,(signed char)0xD8,(signed char)0x7B,(signed char)0xAC,(signed char)0x7E,(signed char)0xAA,(signed char)0xF7,(signed char)0x62,(signed char)0x83,(signed char)0x48,(signed char)0x1B,(signed char)0x93,(signed char)0x79,(signed char)0xC8,(signed char)0xD3,(signed char)0x75,(signed char)0x8E,(signed char)0x27,(signed char)0x3D,(signed char)0x86,(signed char)0x6A,(signed char)0xEB,(signed char)0xB4,(signed char)0x7D,
(signed char)0xA1,(signed char)0x03,(signed char)0x5A,(signed char)0x82,(signed char)0x52,(signed char)0x0F,(signed char)0x9A,(signed char)0x7C,(signed char)0xBD,(signed char)0xDF,(signed char)0x6F,(signed char)0x89,(signed char)0x33,(signed char)0x32,(signed char)0x89,(signed char)0x70,(signed char)0xDE,(signed char)0xBE,(signed char)0x7C,(signed char)0x9A,(signed char)0x10,(signed char)0x51,(signed char)0x82,(signed char)0x5B,(signed char)0x02,(signed char)0xA2,(signed char)0x7D,(signed char)0xB3,(signed char)0xEC,(signed char)0x69,(signed char)0x7E,(signed char)0x96,(signed char)0x33,(signed char)0x14,(signed char)0xAB,(signed char)0x7A,(signed char)0x88,(signed char)0x4F,(signed char)0xF3,(signed char)0xC6,
(signed char)0x6D,(signed char)0x82,(signed char)0x65,(signed char)0xD4,(signed char)0xE5,(signed char)0x5A,(signed char)0x85,(signed char)0x75,(signed char)0xB7,(signed char)0x05,(signed char)0x40,(signed char)0x8F,(signed char)0x7D,(signed char)0x9F,(signed char)0x25,(signed char)0x22,(signed char)0xA1,(signed char)0x7D,(signed char)0x8E,(signed char)0x43,(signed char)0x02,(signed char)0xBA,(signed char)0x74,(signed char)0x84,(signed char)0x5C,(signed char)0xE2,(signed char)0xD7,(signed char)0x63,(signed char)0x82,(signed char)0x6F,(signed char)0xC3,(signed char)0xF7,(signed char)0x4C,(signed char)0x89,(signed char)0x7B,(signed char)0xA9,(signed char)0x17,(signed char)0x30,(signed char)0x98,(signed char)0x7E
};
const signed char g_sin_table[400] = {
(signed char)0x00,(signed char)0x70,(signed char)0x67,(signed char)0xEE,(signed char)0x89,(signed char)0xA5,(signed char)0x24,(signed char)0x7C,(signed char)0x4E,(signed char)0xCC,(signed char)0x82,(signed char)0xC1,(signed char)0x44,(signed char)0x7D,(signed char)0x2F,(signed char)0xAD,(signed char)0x86,(signed char)0xE2,(signed char)0x5F,(signed char)0x75,(signed char)0x0C,(signed char)0x96,(signed char)0x93,(signed char)0x06,(signed char)0x73,(signed char)0x63,(signed char)0xE8,(signed char)0x87,(signed char)0xA9,(signed char)0x29,(signed char)0x7D,(signed char)0x49,(signed char)0xC6,(signed char)0x82,(signed char)0xC6,(signed char)0x49,(signed char)0x7D,(signed char)0x29,(signed char)0xA9,(signed char)0x87,
(signed char)0xE8,(signed char)0x63,(signed char)0x73,(signed char)0x06,(signed char)0x93,(signed char)0x96,(signed char)0x0C,(signed char)0x75,(signed char)0x5F,(signed char)0xE2,(signed char)0x00,(signed char)0x76,(signed char)0x53,(signed char)0xC5,(signed char)0x83,(signed char)0xE3,(signed char)0x68,(signed char)0x67,(signed char)0xE1,(signed char)0x83,(signed char)0xC7,(signed char)0x55,(signed char)0x75,(signed char)0xFE,(signed char)0x89,(signed char)0xAE,(signed char)0x3D,(signed char)0x7D,(signed char)0x1B,(signed char)0x97,(signed char)0x9A,(signed char)0x21,(signed char)0x7D,(signed char)0x37,(signed char)0xAA,(signed char)0x8C,(signed char)0x04,(signed char)0x77,(signed char)0x50,(signed char)0xC2,
(signed char)0x84,(signed char)0xE6,(signed char)0x6A,(signed char)0x65,(signed char)0xDD,(signed char)0x82,(signed char)0xCA,(signed char)0x58,(signed char)0x74,(signed char)0xFA,(signed char)0x88,(signed char)0xB1,(signed char)0x40,(signed char)0x7C,(signed char)0x18,(signed char)0x95,(signed char)0x9C,(signed char)0x25,(signed char)0x7E,(signed char)0x34,(signed char)0x00,(signed char)0x7B,(signed char)0x38,(signed char)0x9F,(signed char)0x9B,(signed char)0x32,(signed char)0x7C,(signed char)0x07,(signed char)0x87,(signed char)0xC1,(signed char)0x5C,(signed char)0x69,(signed char)0xD5,(signed char)0x83,(signed char)0xF2,(signed char)0x77,(signed char)0x45,(signed char)0xA9,(signed char)0x93,(signed char)0x25,
(signed char)0x7E,(signed char)0x15,(signed char)0x8C,(signed char)0xB5,(signed char)0x52,(signed char)0x70,(signed char)0xE2,(signed char)0x82,(signed char)0xE4,(signed char)0x71,(signed char)0x50,(signed char)0xB4,(signed char)0x8D,(signed char)0x17,(signed char)0x7E,(signed char)0x23,(signed char)0x92,(signed char)0xAA,(signed char)0x46,(signed char)0x76,(signed char)0xF0,(signed char)0x83,(signed char)0xD6,(signed char)0x6A,(signed char)0x5B,(signed char)0xC0,(signed char)0x88,(signed char)0x09,(signed char)0x7C,(signed char)0x31,(signed char)0x00,(signed char)0x7D,(signed char)0x17,(signed char)0x87,(signed char)0xD2,(signed char)0x71,(signed char)0x43,(signed char)0x9C,(signed char)0xAB,(signed char)0x55,
(signed char)0x65,(signed char)0xBE,(signed char)0x8F,(signed char)0x2D,(signed char)0x79,(signed char)0xE9,(signed char)0x82,(signed char)0xFF,(signed char)0x7D,(signed char)0x18,(signed char)0x87,(signed char)0xD2,(signed char)0x70,(signed char)0x43,(signed char)0x9C,(signed char)0xAA,(signed char)0x54,(signed char)0x65,(signed char)0xBF,(signed char)0x8F,(signed char)0x2D,(signed char)0x79,(signed char)0xEA,(signed char)0x82,(signed char)0xFF,(signed char)0x7D,(signed char)0x18,(signed char)0x87,(signed char)0xD1,(signed char)0x70,(signed char)0x44,(signed char)0x9C,(signed char)0xAA,(signed char)0x54,(signed char)0x65,(signed char)0xBF,(signed char)0x8F,(signed char)0x2C,(signed char)0x7A,(signed char)0xEB,
(signed char)0x00,(signed char)0x77,(signed char)0xB3,(signed char)0xBA,(signed char)0x7A,(signed char)0xF7,(signed char)0x8C,(signed char)0x54,(signed char)0x3E,(signed char)0x84,(signed char)0x12,(signed char)0x70,(signed char)0xA6,(signed char)0xCA,(signed char)0x7D,(signed char)0xE5,(signed char)0x94,(signed char)0x60,(signed char)0x2E,(signed char)0x82,(signed char)0x23,(signed char)0x67,(signed char)0x9A,(signed char)0xDA,(signed char)0x7E,(signed char)0xD4,(signed char)0x9E,(signed char)0x6B,(signed char)0x1D,(signed char)0x83,(signed char)0x34,(signed char)0x5C,(signed char)0x91,(signed char)0xEC,(signed char)0x7C,(signed char)0xC4,(signed char)0xAA,(signed char)0x73,(signed char)0x0B,(signed char)0x86,
(signed char)0x44,(signed char)0x4F,(signed char)0x8A,(signed char)0xFD,(signed char)0x78,(signed char)0xB5,(signed char)0xB8,(signed char)0x79,(signed char)0xFA,(signed char)0x8B,(signed char)0x00,(signed char)0x6D,(signed char)0x92,(signed char)0x02,(signed char)0x6C,(signed char)0x92,(signed char)0x03,(signed char)0x6B,(signed char)0x91,(signed char)0x05,(signed char)0x6A,(signed char)0x90,(signed char)0x06,(signed char)0x6A,(signed char)0x8F,(signed char)0x08,(signed char)0x69,(signed char)0x8F,(signed char)0x09,(signed char)0x68,(signed char)0x8E,(signed char)0x0B,(signed char)0x67,(signed char)0x8D,(signed char)0x0D,(signed char)0x66,(signed char)0x8D,(signed char)0x0E,(signed char)0x65,(signed char)0x8C,
(signed char)0x10,(signed char)0x64,(signed char)0x8B,(signed char)0x11,(signed char)0x63,(signed char)0x8B,(signed char)0x13,(signed char)0x62,(signed char)0x8A,(signed char)0x14,(signed char)0x61,(signed char)0x8A,(signed char)0x16,(signed char)0x60,(signed char)0x89,(signed char)0x18,(signed char)0x5F,(signed char)0x89,(signed char)0x19,(signed char)0x5E,(signed char)0x00,(signed char)0x5C,(signed char)0x82,(signed char)0x4F,(signed char)0x12,(signed char)0x98,(signed char)0x7B,(signed char)0xC0,(signed char)0xDC,(signed char)0x71,(signed char)0x8A,(signed char)0x30,(signed char)0x35,(signed char)0x88,(signed char)0x6E,(signed char)0xE2,(signed char)0xBB,(signed char)0x7C,(signed char)0x9C,(signed char)0x0C,
(signed char)0x53,(signed char)0x82,(signed char)0x58,(signed char)0x06,(signed char)0xA0,(signed char)0x7D,(signed char)0xB6,(signed char)0xE8,(signed char)0x6B,(signed char)0x86,(signed char)0x3B,(signed char)0x29,(signed char)0x8D,(signed char)0x74,(signed char)0xD6,(signed char)0xC6,(signed char)0x79,(signed char)0x95,(signed char)0x19,(signed char)0x4A,(signed char)0x83,(signed char)0x61,(signed char)0xF9,(signed char)0xA8,(signed char)0x7E,(signed char)0xAC,(signed char)0xF5,(signed char)0x64,(signed char)0x84,(signed char)0x46,(signed char)0x00,(signed char)0x45,(signed char)0x8D,(signed char)0x7C,(signed char)0xA3,(signed char)0x20,(signed char)0x27,(signed char)0x9E,(signed char)0x7D,(signed char)0x90,
(signed char)0x3E,(signed char)0x07,(signed char)0xB5,(signed char)0x76,(signed char)0x85,(signed char)0x58,(signed char)0xE7,(signed char)0xD2,(signed char)0x67,(signed char)0x82,(signed char)0x6C,(signed char)0xC8,(signed char)0xF1,(signed char)0x50,(signed char)0x88,(signed char)0x79,(signed char)0xAD,(signed char)0x12,(signed char)0x35,(signed char)0x95,(signed char)0x7E,(signed char)0x97,(signed char)0x31,(signed char)0x16,(signed char)0xAA,(signed char)0x7A,(signed char)0x89,(signed char)0x4D,(signed char)0xF5,(signed char)0xC5,(signed char)0x6E,(signed char)0x82,(signed char)0x64,(signed char)0xD6,(signed char)0xE3,(signed char)0x5B,(signed char)0x84,(signed char)0x74,(signed char)0xB9,(signed char)0x03
};



//
// dtmf decode initiation
//
void init_dtmf_decoder()
{
  s_dtmf_p = 0;
  memset(s_dtmf_real, 0, sizeof(long) * 8);
  memset(s_dtmf_image, 0, sizeof(long) * 8);
}

//
// dtmf decode program
//
// input: sample = DTMF wave sample value, 
//        sample rate = 4000 point per second
//      NOTE!! sample is "SIGNED" char, NOT "UNSIGNED" char !!
// output: 0 = decoded not completed yet
//       '0123456789*#ABCD' = dtmf code
//
unsigned char decode_dtmf(signed char sample)
{
  unsigned char i, low_freq, high_freq;
  unsigned short  offset;
  
  for (i = 0, offset = s_dtmf_p; i < 8; i ++, offset += g_sincos_len)
  {
    s_dtmf_real[i] += sample * g_cos_table[offset];
    s_dtmf_image[i] += sample * g_sin_table[offset];
  }

  s_dtmf_p ++;

  if (s_dtmf_p == g_sincos_len) // 50 sample has inputed, let's look at the result...
  {
    // calculate |R| + |I|
    for (i = 0; i < 8; i ++)
    {
      s_dtmf_real[i] = labs(s_dtmf_real[i]) + labs(s_dtmf_image[i]);
    }

    // find max value of low freq
    for (low_freq = 0, i = 1; i < 4; i ++)
    {
      if (s_dtmf_real[i] > s_dtmf_real[low_freq])
      {
        low_freq = i;
      }
    }

    // find max value of high freq
    for (high_freq = 4, i = 5; i < 8; i ++)
    {
      if (s_dtmf_real[i] > s_dtmf_real[high_freq])
      {
        high_freq = i;
      }
    }

    // clean up for continuous decoding
    s_dtmf_p = 0;
    memset(s_dtmf_real, 0, 32);
    memset(s_dtmf_image, 0, 32);
    

    // combine low and high frequency and decode out dtmf code
    if ((low_freq == 0) && (high_freq == 4))
    {
      return '1';
    }
    else if ((low_freq == 0) && (high_freq == 5))
    {
      return '2';
    }
    else if ((low_freq == 0) && (high_freq == 6))
    {
      return '3';
    }
    else if ((low_freq == 0) && (high_freq == 7))
    {
      return 'A';
    }
    else if ((low_freq == 1) && (high_freq == 4))
    {
      return '4';
    }
    else if ((low_freq == 1) && (high_freq == 5))
    {
      return '5';
    }
    else if ((low_freq == 1) && (high_freq == 6))
    {
      return '6';
    }
    else if ((low_freq == 1) && (high_freq == 7))
    {
      return 'B';
    }
    else if ((low_freq == 2) && (high_freq == 4))
    {
      return '7';
    }
    else if ((low_freq == 2) && (high_freq == 5))
    {
      return '8';
    }
    else if ((low_freq == 2) && (high_freq == 6))
    {
      return '9';
    }
    else if ((low_freq == 2) && (high_freq == 7))
    {
      return 'C';
    }
    else if ((low_freq == 3) && (high_freq == 4))
    {
      return '*';
    }
    else if ((low_freq == 3) && (high_freq == 5))
    {
      return '0';
    }
    else if ((low_freq == 3) && (high_freq == 6))
    {
      return '#';
    }
    else if ((low_freq == 3) && (high_freq == 7))
    {
      return 'D';
    }
  }

  return  0;
}

// add mian() to test compliation, done
// this is not working app
// xiaolaba 2013
int main(){
	init_dtmf_decoder();
	
	while(1){
		decode_dtmf(50);
	}

}
