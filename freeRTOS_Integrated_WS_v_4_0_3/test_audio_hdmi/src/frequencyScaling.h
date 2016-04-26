#ifndef FREQ_SCALE
#define FREQ_SCALE
//#include "audioTestbench.h"
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "math.h"
#include "fprof.h"

#define NUM_CHANNELS 2
#define NUM_FREQS 8
#define PI 3.1415916

#define FSS_PREVIOUS_INPUT 0
#define FSS_CROSS_INPUT 1
#define FSS_THIS_INPUT 2

#if FFT_SAMPLES == 4096
	#define MEMNEEDED	(41236)
#elif FFT_SAMPLES == 2048
	#define MEMNEEDED	(20756)
#elif FFT_SAMPLES == 1024
	#define MEMNEEDED	(10516)
#elif FFT_SAMPLES == 512
	#define MEMNEEDED	(5396)
#elif FFT_SAMPLES == 256
	#define MEMNEEDED	(2836)
#else
	#error ERROR: Invalid value for CHUNK_BYTES
	//#define MEMNEEDED		(1)
#endif

void processInput(short *input, short *output, short channel);
void frequencyScale(float *input, float *output, int len, float *scalars);
void configureFft();
void configureScalars(float * input);
void configureWindow();
//void fft(short input[ARRAY_SIZE], kiss_fft_cpx output[SCALING_SIZE]);
#endif
