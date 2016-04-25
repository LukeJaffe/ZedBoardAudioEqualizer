/*  Project:  ZedboardAudioEqualizer
    Authors:  Luke Jaffe, Sean Kerr, Dan Labove
   
    Test code for running the frequency scaling code on a PC
    */

#include <stdlib.h>

#include "audioTestbench.h"
#include "frequencyScaling.h"
#include "bandpassFrequencyScaling.h"

// ARRAY_SIZE

float * frequencyScalars;
unsigned int ip[8];

void fullSystemTest()
{

    SNDFILE *sf;
    SF_INFO info;
    int num_channels;
    int num, num_items;
    short *buf;
    short *outBuf;
    short *leftBuf;
    short *rightBuf;
    short *leftOutBuf;
    short *rightOutBuf;
    int f,sr,c;
    int i,j;
    FILE *out;
    
    /* Open the WAV file. */
    info.format = 0;
    sf = sf_open("resistance.wav",SFM_READ,&info);
    if (sf == NULL)
        {
        printf("Failed to open the file.\n");
        exit(-1);
        }
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    printf("frames=%d\n",f);
    printf("samplerate=%d\n",sr);
    printf("channels=%d\n",c);
    num_items = f*c;
    printf("num_items=%d\n",num_items);
    /* Allocate space for the data to be read, then read it. */
    buf = (short *) malloc(num_items*sizeof(short));
    leftBuf = (short *) malloc(num_items / 2 * sizeof(short));
    rightBuf = (short *) malloc(num_items / 2 * sizeof(short));
    num = sf_read_short(sf,buf,num_items);
    sf_close(sf);
    printf("Read %d items\n",num);

    initBuffers();

    /* Buffer for processed data */
    outBuf = (short *) malloc(num_items*sizeof(short));
    leftOutBuf = (short *) malloc(num_items / 2 * sizeof(short));
    rightOutBuf = (short *) malloc(num_items / 2 * sizeof(short));

    /* Initialize everything in frequencyScaling.c */
    configureFft();
    configureWindow();

    for (i = 0; i < num; i++)
    {
        if (!(i % 2))
        {
            leftBuf[i/2] = buf[i];
        }
        else
        {
            rightBuf[i/2] = buf[i];
        }
    }

    for (i = 0; i < num / ARRAY_SIZE / 2 ; i++)
    {
        processInput(leftBuf + (i * ARRAY_SIZE), leftOutBuf + (i * ARRAY_SIZE), 0, ip);
        processInput(rightBuf + (i * ARRAY_SIZE), rightOutBuf + (i * ARRAY_SIZE), 1, ip);

        int j;
        for (j = 0; j < 8; j++)
        {
            printf("%d, ", ip[j]);
        }
        printf("\n");
        //processInputBandpass(leftBuf + (i * ARRAY_SIZE), leftOutBuf + (i * ARRAY_SIZE), ARRAY_SIZE);
        //processInputBandpass(rightBuf + (i * ARRAY_SIZE), rightOutBuf + (i * ARRAY_SIZE), ARRAY_SIZE);

        if (!(i%100))
        {
            printf("%d\n", i);
        }
    }

    for (i = 0; i < num; i++)
    {
        if (!(i % 2))
        {
            outBuf[i] = leftOutBuf[i/2];
        }
        else
        {
            outBuf[i] = rightOutBuf[i/2];
        }
    }

    sf = sf_open("resistanceOutput.wav", SFM_WRITE, &info);
    sf_write_short(sf, outBuf, num_items);
    sf_close(sf);

    free(buf);
    free(outBuf);
    free(leftBuf);
    free(rightBuf);
    free(leftOutBuf);
    free(rightOutBuf);
    free(frequencyScalars);
}

void configureAllPassTest()
{
    frequencyScalars = malloc(SCALING_SIZE * sizeof(float));

    int i;

    for (i = 0; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = 1.0;
    }

    for (i = 15; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = 0.0;
    }

    configureScalars(frequencyScalars);
}

void configureLowPassTest()
{

    int i;

    for (i = 0; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = 1.0 - (1.0 * i / SCALING_SIZE);
    }

    configureScalars(frequencyScalars);
}

void configureHighPassTest()
{

    int i;

    for (i = 0; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = (1.0 * i / SCALING_SIZE);
    }

    configureScalars(frequencyScalars);
}

void configureCrazyHighPassTest()
{
    int i;

    for (i = 0; i < SCALING_SIZE / 2; i++)
    {
        frequencyScalars[i] = 0.0;
    }

    for (i = SCALING_SIZE / 2; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = 1.0;
    }

    configureScalars(frequencyScalars);
}

void configureCrazyLowPassTest()
{
    int i;

    for (i = 0; i < 50; i++)
    {
        frequencyScalars[i] = 1.0;
    }

    for (i = 50; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = 0.0;
    }

    configureScalars(frequencyScalars);
}

void configureOctavePassTest(float *octaves)
{
    int i;

    // First Ocatave
    for (i = 0; i < 4; i++)
    {
        frequencyScalars[i] = octaves[0];
    }

    // Second Ocatave
    for (i = 4; i < 7; i++)
    {
        frequencyScalars[i] = octaves[1];
    }

    // Third Ocatave
    for (i = 7; i < 14; i++)
    {
        frequencyScalars[i] = octaves[2];
    }

    // Fourth Ocatave
    for (i = 14; i < 27; i++)
    {
        frequencyScalars[i] = octaves[3];
    }

    // Fifth Ocatave
    for (i = 27; i < 53; i++)
    {
        frequencyScalars[i] = octaves[4];
    }

    // Sixth Ocatave
    for (i = 53; i < 105; i++)
    {
        frequencyScalars[i] = octaves[5];
    }

    // Seventh Ocatave
    for (i = 105; i < 210; i++)
    {
        frequencyScalars[i] = octaves[6];
    }

    // Eighth Ocatave
    for (i = 210; i < SCALING_SIZE; i++)
    {
        frequencyScalars[i] = octaves[7];
    }

    configureScalars(frequencyScalars);
}

int main()
{
    float octaves[8];

    octaves[0] = 1.0;
    octaves[1] = 1.0;
    octaves[2] = 1.0;
    octaves[3] = 1.0;
    octaves[4] = 1.0;
    octaves[5] = 1.0;
    octaves[6] = 1.0; 
    octaves[7] = 1.0;
    //configureOctavePassTest(octaves);

    configureAllPassTest();
    fullSystemTest();
}
