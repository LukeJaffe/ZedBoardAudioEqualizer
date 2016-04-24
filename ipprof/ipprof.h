/*
 * ipprof.h
 *
 *  Created on: Apr 21, 2016
 *      Author: jaffe.lu
 */

#ifndef IPPROF_H_
#define IPPROF_H_

//#include "fprof.h"
//#include "kiss_fftr.h"

#define INPUT_SIZE      	(4096)
#define OUTPUT_SIZE    		(INPUT_SIZE/2 + 1)
#define IP_BANDS	    	(8)
#define FSAMPLE		    	(16000)
#define K			((float)FSAMPLE/(float)OUTPUT_SIZE)

/*
kiss_fft_cpx fft_output[SCALING_SIZE];

typedef struct
{
    int bands[IP_BANDS];
    int band_idx[IP_BANDS];
    //float fout[ARRAY_SIZE];
} ipprof_t;

void ipprof_init(ipprof_t* this);

void ipprof_getBands(ipprof_t* this, kiss_fft_cpx fft_output[ARRAY_SIZE]);
*/

#endif /* IPPROF_H_ */
