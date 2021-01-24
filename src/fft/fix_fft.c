

#include "fix_fft.h"

#include <stdint.h>

/* fix_fft.c - Fixed-point in-place Fast Fourier Transform  */
/*
  All data are fixed-point short integers, in which -32768
  to +32768 represent -1.0 to +1.0 respectively. Integer
  arithmetic is used for speed, instead of the more natural
  floating-point.

  For the forward FFT (time -> freq), fixed scaling is
  performed to prevent arithmetic overflow, and to map a 0dB
  sine/cosine wave (i.e. amplitude = 32767) to two -6dB freq
  coefficients. The return value is always 0.

  For the inverse FFT (freq -> time), fixed scaling cannot be
  done, as two 0dB coefficients would sum to a peak amplitude
  of 64K, overflowing the 32k range of the fixed-point integers.
  Thus, the fix_fft() routine performs variable scaling, and
  returns a value which is the number of bits LEFT by which
  the output must be shifted to get the actual amplitude
  (i.e. if fix_fft() returns 3, each value of fr[] and fi[]
  must be multiplied by 8 (2**3) for proper scaling.
  Clearly, this cannot be done within fixed-point short
  integers. In practice, if the result is to be used as a
  filter, the scale_shift can usually be ignored, as the
  result will be approximately correctly normalized as is.

  Written by:  Tom Roberts  11/8/89
  Made portable:  Malcolm Slaney 12/15/94 malcolm@interval.com
  Enhanced:  Dimitrios P. Bouras  14 Jun 2006 dbouras@ieee.org
  Modified for 8bit values David Keller  10.10.2010
*/


#define N_WAVE	1024    /* full length of Sinewave[] */
#define LOG2_N_WAVE 10	/* log2(N_WAVE) */




/*
  Since we only use 3/4 of N_WAVE, we define only
  this many samples, in order to conserve data space.
*/
const int8_t Sinewave[768] = {
0,0,1,2,3,3,4,5,6,7,7,8,9,10,10,11,12,13,14,14,15,16,17,18,18,19,20,21,21,22,23,24,24,25,26,27,28,28,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,40,41,42,
43,43,44,45,46,46,47,48,48,49,50,51,51,52,53,54,54,55,56,56,57,58,58,59,60,61,61,62,63,63,64,65,65,66,67,67,68,69,69,70,71,71,72,73,73,74,74,75,76,76,77,78,
78,79,79,80,81,81,82,83,83,84,84,85,85,86,87,87,88,88,89,89,90,91,91,92,92,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,
105,105,105,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,112,113,113,113,114,114,115,115,115,116,116,116,117,117,117,117,118,118,118,119,
119,119,119,120,120,120,121,121,121,121,122,122,122,122,122,123,123,123,123,123,124,124,124,124,124,125,125,125,125,125,125,125,126,126,126,126,126,126,126,
126,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
127,127,127,126,126,126,126,126,126,126,126,125,125,125,125,125,125,125,124,124,124,124,124,123,123,123,123,123,122,122,122,122,122,121,121,121,121,120,120,
120,119,119,119,119,118,118,118,117,117,117,117,116,116,116,115,115,115,114,114,113,113,113,112,112,112,111,111,110,110,110,109,109,108,108,108,107,107,106,
106,105,105,105,104,104,103,103,102,102,101,101,100,100,99,99,98,98,97,97,96,96,95,95,94,94,93,93,92,92,91,91,90,89,89,88,88,87,87,86,85,85,84,84,83,83,82,
81,81,80,79,79,78,78,77,76,76,75,74,74,73,73,72,71,71,70,69,69,68,67,67,66,65,65,64,63,63,62,61,61,60,59,58,58,57,56,56,55,54,54,53,52,51,51,50,49,48,48,47,
46,46,45,44,43,43,42,41,40,40,39,38,37,37,36,35,34,34,33,32,31,31,30,29,28,28,27,26,25,24,24,23,22,21,21,20,19,18,18,17,16,15,14,14,13,12,11,10,10,9,8,7,7,6,
5,4,3,3,2,1,0,0,-1,-2,-3,-4,-4,-5,-6,-7,-8,-8,-9,-10,-11,-11,-12,-13,-14,-15,-15,-16,-17,-18,-18,-19,-20,-21,-22,-22,-23,-24,-25,-25,-26,-27,-28,-29,-29,-30,
-31,-32,-32,-33,-34,-35,-35,-36,-37,-38,-38,-39,-40,-41,-41,-42,-43,-44,-44,-45,-46,-47,-47,-48,-49,-49,-50,-51,-52,-52,-53,-54,-55,-55,-56,-57,-57,-58,-59,
-59,-60,-61,-62,-62,-63,-64,-64,-65,-66,-66,-67,-68,-68,-69,-70,-70,-71,-72,-72,-73,-74,-74,-75,-75,-76,-77,-77,-78,-79,-79,-80,-80,-81,-82,-82,-83,-84,-84,
-85,-85,-86,-86,-87,-88,-88,-89,-89,-90,-90,-91,-92,-92,-93,-93,-94,-94,-95,-95,-96,-96,-97,-97,-98,-98,-99,-99,-100,-100,-101,-101,-102,-102,-103,-103,-104,
-104,-105,-105,-106,-106,-106,-107,-107,-108,-108,-109,-109,-109,-110,-110,-111,-111,-111,-112,-112,-113,-113,-113,-114,-114,-114,-115,-115,-116,-116,-116,-117,
-117,-117,-118,-118,-118,-118,-119,-119,-119,-120,-120,-120,-120,-121,-121,-121,-122,-122,-122,-122,-123,-123,-123,-123,-123,-124,-124,-124,-124,-124,-125,-125,-125,-125,-125,-126,-126,-126,-126,
-126,-126,-126,-127,-127,-127,-127,-127,-127,-127,-127,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128};


unsigned short findDecimationIndex(unsigned short index,int bitNum)
{

    volatile unsigned short i = 0;

    unsigned short result = 0;
    for (; i < bitNum; i++)
    {
        result = result << 1;
        if (index & 0x01)
        {

            result |= 0x01;
        }
        index = index >> 1;
    }

    return result;
}


/*
  FIX_MPY() - fixed-point multiplication & scaling.
  Substitute inline assembly for hardware-specific
  optimization suited to a particluar DSP processor.
  Scaling ensures that result remains 16-bit.
*/
signed char FIX_MPY(signed char a, signed char b)
{
  
  //Serial.println(a);
 //Serial.println(b);

  
    /* shift right one less bit (i.e. 15-1) */
    int c = ((int)a * (int)b) >> 6;  //6
    /* last bit shifted out = rounding-bit */
    b = c & 0x01;
    /* last shift + rounding bit */
    a = (c >> 1) + b;

	  /*
	  Serial.println(Sinewave[3]);
	  Serial.println(c);
	  Serial.println(a);
	  while(1);*/
    __no_operation();
    return a;
}


/*
  fix_fft() - perform forward/inverse fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
  0 for forward transform (FFT), or 1 for iFFT.
*/
int fix_fft(signed char fr[], signed char fi[], int m, int inverse)
{
    int mr, nn, i, j, l, k, istep, n, scale, shift;
    signed char qr, qi, tr, ti, wr, wi;

    n = 1 << m;

    /* max FFT size = N_WAVE */
    if (n > N_WAVE)
	  return -1;

    mr = 0;
    nn = n - 1;
    scale = 0;

    /* decimation in time - re-order data */
    /*
    for (m=1; m<=nn; ++m) {
	  l = n;
	  do {
		l >>= 1;
	  } while (mr+l > nn);
	  mr = (mr & (l-1)) + l;

	  if (mr <= m)
		continue;



	  tr = fr[m];
	  fr[m] = fr[mr];
	  fr[mr] = tr;

	  if(inverse){
          ti = fi[m];
          fi[m] = fi[mr];
          fi[mr] = ti;
	  }
    }
    */

    l = 1;
    k = LOG2_N_WAVE-1;
    while (l < n) {
	  if (inverse) {
		/* variable scaling, depending upon data */
		shift = 0;
		for (i=0; i<n; ++i) {
		    j = fr[i];
		    if (j < 0)
			  j = -j;
		    m = fi[i];
		    if (m < 0)
			  m = -m;
		    if (j > 16383 || m > 16383) {
			  shift = 1;
			  break;
		    }
		}
		if (shift)
		    ++scale;
	  } else {
		/*
		  fixed scaling, for proper normalization --
		  there will be log2(n) passes, so this results
		  in an overall factor of 1/n, distributed to
		  maximize arithmetic accuracy.
		*/
		shift = 1;
	  }
	  /*
	    it may not be obvious, but the shift will be
	    performed on each data point exactly once,
	    during this pass.
	  */
	  istep = l << 1;
	  for (m=0; m<l; ++m) {
		j = m << k;
		/* 0 <= j < N_WAVE/2 */
		wr = Sinewave[j+N_WAVE/4];
		wi = Sinewave[j];
		if (inverse)
		    wi = -wi;
		if (shift) {
		    wr >>= 1;
		    wi >>= 1;
		}
		for (i=m; i<n; i+=istep) {
		    j = i + l;
		    tr = FIX_MPY(wr,fr[j]) - FIX_MPY(wi,fi[j]);
		    ti = FIX_MPY(wr,fi[j]) + FIX_MPY(wi,fr[j]);
		    qr = fr[i];
		    qi = fi[i];
		    if (shift) {
			  qr >>= 1;
			  qi >>= 1;
		    }
		    fr[j] = qr - tr;
		    fi[j] = qi - ti;
		    fr[i] = qr + tr;
		    fi[i] = qi + ti;
		}
	  }
	  --k;
	  l = istep;
    }


    return scale;
}

/*
  fix_fftr() - forward/inverse FFT on array of real numbers.
  Real FFT/iFFT using half-size complex FFT by distributing
  even/odd samples into real/imaginary arrays respectively.
  In order to save data space (i.e. to avoid two arrays, one
  for real, one for imaginary samples), we proceed in the
  following two steps: a) samples are rearranged in the real
  array so that all even samples are in places 0-(N/2-1) and
  all imaginary samples in places (N/2)-(N-1), and b) fix_fft
  is called with fr and fi pointing to index 0 and index N/2
  respectively in the original array. The above guarantees
  that fix_fft "sees" consecutive real samples as alternating
  real and imaginary samples in the complex array.
*/
int fix_fftr(signed char f[], int m, int inverse)
{
    int i, N = 1<<(m-1), scale = 0;
    signed char tt, *fr=f, *fi=&f[N];

    if (inverse)
	  scale = fix_fft(fi, fr, m-1, inverse);
    for (i=1; i<N; i+=2) {
	  tt = f[N+i-1];
	  f[N+i-1] = f[i];
	  f[i] = tt;
    }
    if (! inverse)
	  scale = fix_fft(fi, fr, m-1, inverse);
    return scale;
} 



unsigned char spectrum(signed char fr[], signed char fi[]){

    fix_fft(fr, fi, 8, 0);
    volatile unsigned int den = 0;
    volatile int i = 0;
    __no_operation();

    unsigned char max_mag = 0;

    volatile unsigned short b = 0;
    for (i=0; i< 256 ;i++){
        b = 0;
        b = fr[i] * fr[i];
        b += fi[i] * fi[i];



        //b = b>>4;

        if(b>max_mag)
            max_mag = b;


        fr[i] = (char)b;
        //printf("%d ",data[i]);
    }


    return max_mag;

}

unsigned short magnitude(signed char fr[], signed char fi[]){
    unsigned short den = 0;
    unsigned short max_f=0, max=0;
    unsigned short x = 0;
    int i =0;

    for (; i < 512; i++)
    {
        den = fr[i] * fr[i] + fi[i] * fi[i];
        //den >> 1;
        if (den > 127)
            den = 127;

        fr[i] = (char)den;

        if(den>max){
            max_f = 59*i;

            max = den;
        }
        //printf("%d ", den);
    }


/*
    printf("\nMAX_F : %d\n",max_f);
    printf("\nMAG: \n\n");
    for(int i=0;i<512;i++)
        printf("%d ",fr[i]);
*/
    return max_f;
}
