#include "audioTestbench.h"
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "math.h"

#define NUM_CHANNELS 2
#define NUM_FREQS 8
#define PI 3.1415916

#define FSS_PREVIOUS_INPUT 0
#define FSS_CROSS_INPUT 1
#define FSS_THIS_INPUT 2

void processInput(short *input, short *output, short channel);
void frequencyScale(float *input, float *output, int len, float *scalars);
void configureFft();
void configureScalars(float * input);
void configureWindow();
