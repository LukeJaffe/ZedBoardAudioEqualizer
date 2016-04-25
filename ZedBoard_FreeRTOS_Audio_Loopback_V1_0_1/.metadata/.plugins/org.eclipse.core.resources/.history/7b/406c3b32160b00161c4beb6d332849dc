/*
 * ipprof.c
 *
 *  Created on: Apr 21, 2016
 *      Author: jaffe.lu
 */

#include <stdio.h>
#include <math.h>
#include "kiss_fft.h"
#include "ipprof.h"

const static float freqs[] =
{
	0.0,
	16.35,
	32.70,
	65.41,
	130.81,
	261.63,
	523.25,
	1046.50,
	2093.00,
	4186.01,
    FSAMPLE,
};

const int num_freqs = sizeof(freqs)/sizeof(float);

void avg(int bands[10], kiss_fft_cpx output[OUTPUT_SIZE]);

int main()
{
	printf("num_freqs: %d\n", num_freqs);
    printf("K = %f, tot = %f\n", K, freqs[num_freqs-1]/K);
    int bands[num_freqs-1];
    int i, j = 0;
    float k;

    for (i = 0; i < num_freqs-1; i++)
    {
		bands[i] = 0;
        for (k = freqs[i]; k < freqs[i+1] && j < OUTPUT_SIZE; k = j*K, j++)
        {
            bands[i]++; 
        }
    } 

    int sum = 0;
    for (i = 0; i < num_freqs-1; i++)
    {
        sum += bands[i];
        printf("band %d: %d\n", i, bands[i]);
    }
    printf("tot: %d\n", sum);

    // calc ip
    //kiss_fft_cpx output[OUTPUT_SIZE];
    //kiss_fft_scalar s = sqrt(output[0].r*output[0].r* + output[0].i*output[0].i);

    avg(bands, output);
    return 0;
}

/*
void avg(int bands[num_freqs-1], kiss_fft_cpx output[SCALING_SIZE])
{
    float ip, avg[num_freqs-3];
    int i, j;
    for (i = 1; i < num_freqs-1; i++)
    {
        for (j = bands[i]; j < bands[i+1]; j++)
		{
			ip = sqrt(output[j].r*output[j].r* + output[j].i*output[j].i);
            avg[i-1] += ip; 
		}
        avg[i-1] /= (float)(output[j]-output[bands[i]]);
    }
}
*/

/*
void ipprof_init(ipprof_t* this)
{
	int i, j = 0;
	float k = 0.0;
	printf("K: %f\n", K);
	for (i = 0; i < IP_BANDS; i++)
	{
		for (; k < freqs[i]; k += K, j++);
		this->band_idx[i] = j;
		printf("band_idx[%d] = %d\n", i, j);
	}
}

void ipprof_getBands(ipprof_t* this, kiss_fft_cpx fft_output[SCALING_SIZE])
{
	float fout[SCALING_SIZE-1];

	int i;
	for (i = 1; i < SCALING_SIZE; i++)
		fout[i-1] = (float)i*K;
}
*/
