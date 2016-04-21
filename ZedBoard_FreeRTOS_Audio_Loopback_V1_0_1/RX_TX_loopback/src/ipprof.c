/*
 * ipprof.c
 *
 *  Created on: Apr 21, 2016
 *      Author: jaffe.lu
 */

#include "ipprof.h"

const static float freqs[] =
{
	[0]  	16.35,
	[1]		32.70,
	[2]		65.41,
	[3]		130.81,
	[4]		261.63,
	[5]		523.25,
	[6]		1046.50,
	[7]		2093.00,
	[8]		4186.01
};

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
