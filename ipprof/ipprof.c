/*
 * ipprof.c
 *
 *  Created on: Apr 21, 2016
 *      Author: jaffe.lu
 */

#include <stdio.h>
#include "ipprof.h"

const static float freqs[] =
{
    [0]     0.0,
	[1]  	16.35,
	[2]		32.70,
	[3]		65.41,
	[4]		130.81,
	[5]		261.63,
	[6]		523.25,
	[7]		1046.50,
	[8]		2093.00,
	[9]		4186.01,
    [10]    FSAMPLE,
};

int main()
{
    printf("K = %f, tot = %f\n", K, freqs[10]/K);
    int bands[10];
    int i, j = 0;
    float k;
    for (i = 0; i <= 10; i++)
    {
        bands[i] = 0;
        printf("%d\n", i);
        for (k = freqs[i]; k < freqs[i+1]; k = j*K, j++)
        {
            bands[i]++; 
        }
    } 

    int sum = 0;
    for (i = 0; i < 10; i++)
    {
        sum += bands[i];
        printf("band %d: %d\n", i, bands[i]);
    }
    printf("tot: %d\n", sum);
}

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
