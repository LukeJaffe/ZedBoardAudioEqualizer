/*
 * fprof.c
 *
 *  Created on: Apr 20, 2016
 *      Author: jaffe.lu
 */
#include "fprof.h"

#define SAMPLES_PER_BAND	(FFT_SAMPLES/NUM_BANDS)

#define INPUT_SIZE      	(ARRAY_SIZE)
#define OUTPUT_SIZE    		(SCALING_SIZE)
#define K			((float)FSAMPLE/(float)OUTPUT_SIZE)

void fprof_set_freqs(fprof_t* this)
{
	int i, j;

    for (i = 0; i < num_freqs; i++)
    {
    	for (j = (1<<i) - 1; (j < (1<<(i+1)) - 1) && (j < SCALING_SIZE); j++)
    	{
			if (i < 8)
			{
				frequency_scalars[j] = (float)this->bands[i]/(float)MID_BAND_VAL;
			}
			else
			{
				frequency_scalars[j] = 1.0;
			}
    	}
    }

    //printf("Frequencies assigned\n");
}

void fprof_init(fprof_t* this)
{
    //for (i = 0; i < NUM_BANDS; i++)
    //  this->bands[i] = 4;

    this->bands[0] = 8;
    this->bands[1] = 8;
    this->bands[2] = 8;
    this->bands[3] = 8;
    this->bands[4] = 8;
    this->bands[5] = 8;
    this->bands[6] = 8;
    this->bands[7] = 8;

    this->selected = 3;

    fprof_set_freqs(this);
}

void fprof_incSel(fprof_t* this)
{
    if (this->selected < NUM_BANDS-1)
    {
        this->selected++;
       // printf("Selected band: %d\n", this->selected);
    }
}

void fprof_decSel(fprof_t* this)
{
    if (this->selected > 0)
    {
        this->selected--;
    	//printf("Selected band: %d\n", this->selected);
    }
}

void fprof_incBand(fprof_t* this)
{
	int i;
    if (this->bands[this->selected] < MAX_BAND_VAL)
    {
        this->bands[this->selected]++;
        fprof_set_freqs(this);
//        for (i = 0; i < NUM_BANDS; i++)
//        	printf("%d ", this->bands[i]);
//        printf("\n");
    }
}

void fprof_decBand(fprof_t* this)
{
	int i;
    if (this->bands[this->selected] > MIN_BAND_VAL)
    {
        this->bands[this->selected]--;
        fprof_set_freqs(this);
//        for (i = 0; i < NUM_BANDS; i++)
//        	printf("%d ", this->bands[i]);
//        printf("\n");
    }
}

void fprof_getBands(fprof_t* this, char buffer[12])
{
    int i;
    buffer[0] = 'S';
    buffer[1] = this->selected + '0';
    for (i = 2; i < 2+NUM_BANDS; i++)
        buffer[i] = this->bands[i-2] + '0';
    buffer[NUM_BANDS+2] = 'E';
    buffer[NUM_BANDS+3] = '\0';
}

void fprof_getScalars(fprof_t* this, float scalars[ARRAY_SIZE])
{
	int i, j;
	float diff, inc, val;
	for (i = 0; i < NUM_BANDS-1; i++)
	{
		diff = (float)(this->bands[i+1] - this->bands[i]);
		inc = diff/(float)NUM_SUB_BANDS;
		for (j = 0; j < NUM_SUB_BANDS; j++)
		{
			val = ((float)this->bands[i]) + ((float)j)*inc;
			scalars[i*NUM_SUB_BANDS + j] = val;
		}
	}
}
