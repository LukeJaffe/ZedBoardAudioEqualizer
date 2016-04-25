#include "frequencyScaling.h"

int bandBoundaries[NUM_FREQS][2] = 
    {{0, 2},
     {2, 4},
     {4, 8},
     {8, 16},
     {16, 32},
     {32, 64},
     {64, 128},
     {128, SCALING_SIZE}};

// First half of this array is previous sample set
// Second half of this array is current sample set
short inputBuffer[NUM_CHANNELS][ARRAY_SIZE * 2];

float windowedCrossSampleArray[NUM_CHANNELS][ARRAY_SIZE];
float windowedInputArray[NUM_CHANNELS][ARRAY_SIZE];

/* Store results of windowed frequency scaling in alternating buffers */
float frequencyScaledSamples[NUM_CHANNELS][3][ARRAY_SIZE];

float hanningWindow[ARRAY_SIZE];

float * frequencyScalars;

kiss_fftr_cfg fftr_cfg;
kiss_fftr_cfg ifftr_cfg;

void configureFft()
{
    fftr_cfg = kiss_fftr_alloc(ARRAY_SIZE, 0, NULL, NULL);
    ifftr_cfg = kiss_fftr_alloc(ARRAY_SIZE, 1, NULL, NULL);
}

void configureScalars(float *input)
{
    frequencyScalars = input;
}

void configureWindow()
{
    int i;
    
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        hanningWindow[i] = 0.5 * (1 - cos((2 * PI * i) / (ARRAY_SIZE - 1)));
    }
}

void applyWindow(short *input, float *output)
{
    int i;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        output[i] = input[i] * hanningWindow[i];
    }
}

// Applies the FFT-scale-IFFT algorithm to an input array 
// Also applies window before frequency scaling
void frequencyScale(float *input, float *output, int len, float *scalars, short calculateIp, unsigned int *ip)
{
    int i;

    kiss_fft_scalar inputArray[len];
    kiss_fft_cpx transformedArray[len / 2 + 1];

    for (i = 0; i < len; i++)
    {
        //inputComplex[i].r = input[i];
        //inputComplex[i].i = 0;
        inputArray[i] = input[i];
    }

    kiss_fftr(fftr_cfg, inputArray, transformedArray);

    for (i = 0; i < SCALING_SIZE; i++)
    {
        transformedArray[i].r *= scalars[i];
        transformedArray[i].i *= scalars[i];
    }

    kiss_fftri(ifftr_cfg, transformedArray, inputArray);

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        output[i] = inputArray[i] / (1.0 * ARRAY_SIZE);
    }

    if (calculateIp == CALCULATE_IP)
    {
        int j;
        for (i = 0; i < NUM_FREQS; i++)
        {
            float thisBandIp = 0;

            for (j = bandBoundaries[i][0]; j < bandBoundaries[i][1]; j++)
            {
                thisBandIp += sqrt(transformedArray[i].r * 
                    transformedArray[i].r + transformedArray[i].i * 
                    transformedArray[i].i) / 10000000 * IP_MAX_HEIGHT;
            }

            ip[i] = thisBandIp / (bandBoundaries[i][1] - bandBoundaries[i][0]);
        }
    }
}

// Note: this function will have half-buffer phase shift effect
void processInput(short *input, short *output, short channel, unsigned int * ip)
{
    int i;

    // Read in the input buffer
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        inputBuffer[channel][ARRAY_SIZE + i] = input[i];
    }

    // Apply windows to the midpoint buffer and input buffer
    applyWindow(inputBuffer[channel] + (ARRAY_SIZE / 2), windowedCrossSampleArray[channel]);
    applyWindow(inputBuffer[channel] + ARRAY_SIZE, windowedInputArray[channel]);

    frequencyScale(windowedCrossSampleArray[channel], frequencyScaledSamples[channel][1], ARRAY_SIZE, frequencyScalars, DO_NOT_CALCULATE_IP, ip);

    frequencyScale(windowedInputArray[channel], frequencyScaledSamples[channel][2], ARRAY_SIZE, frequencyScalars, CALCULATE_IP, ip);

    // Sum overlaps to produce the output array
    for (i = 0; i < ARRAY_SIZE / 2; i++)
    {
        output[i] = frequencyScaledSamples[channel][FSS_PREVIOUS_INPUT][ARRAY_SIZE / 2 + i] + frequencyScaledSamples[channel][FSS_CROSS_INPUT][i];
    }

    for (i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++)
    {
        // float -> short conversion should be handled automatically
        output[i] = frequencyScaledSamples[channel][FSS_CROSS_INPUT][i] + frequencyScaledSamples[channel][FSS_THIS_INPUT][i - (ARRAY_SIZE / 2)];
    }

    // Store input and scaled version of input for use in next iteration
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        inputBuffer[channel][i] = inputBuffer[channel][i + ARRAY_SIZE];
        frequencyScaledSamples[channel][FSS_PREVIOUS_INPUT][i] = frequencyScaledSamples[channel][FSS_THIS_INPUT][i];
    }

    /*
    for (i = 0; i < ARRAY_SIZE / 2; i++)
    {
        printf("%d, %d\n", input[i], output[i + ARRAY_SIZE / 2]);
    }
    printf("\n\n");
    */
    
}
