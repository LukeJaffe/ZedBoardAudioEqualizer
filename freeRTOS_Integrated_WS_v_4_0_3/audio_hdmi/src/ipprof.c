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

#define IP_BANDS	(8)

void ipprof_get_ip(kiss_fft_cpx output[SCALING_SIZE])
{
	int i, j;
	float ip;
	float fbands[IP_BANDS];
	for (i = 0; i < IP_BANDS; i++)
		fbands[i] = 1.0;

    for (i = 0; i < num_freqs; i++)
    {
    	int j_min = (1<<i) - 1;
    	int j_max = (1<<(i+1)) - 1;
    	for (j = j_min; j< j_max && (j < SCALING_SIZE); j++)
    	{
			if (i < IP_BANDS)
			{
				ip = sqrt(output[j].r*output[j].r* + output[j].i*output[j].i);
				fbands[i] += ip;
			}
			else
			{
				fbands[i] += 1.0; //???
			}
			fbands[i] /= (float)(j_max - j_min);
			ipbands[i] = (int)fbands[i];
    	}
    }

    printf("Frequencies assigned\n");
}

