/**
 *@file snd_sample.h
 *
 *@brief
 *  - Contains the audio data samples
 *    PCM Samples 16bit mono 8kHz
 *
 * @author  Rohan Kangralkar
 *
 * LastChange:
 * $Id$
 *
 *******************************************************************************/
#include "chunk_d.h"
#include <string.h>
#include "audioSample.h"
#include <assert.h>
//#include "snd_sample.c"

extern unsigned int snd_samples[];
extern unsigned int snd_samples_nSamples;

/**
 * Initialize the audioSample structure
 *  - set the memory start
 *  - set the size of the memory that has sound samples
 *  - set the initial count
 */
int audioSample_init(audioSample_t *pThis) {

  pThis->pmem   = snd_samples;
  pThis->size   = snd_samples_nSamples * sizeof(unsigned int);
  pThis->count  = 0;

  return 1;
}

/**
 * Initialize the audioSample structure
 *  - set the memory start
 *  - set the size of the memory that has sound samples
 *  - set the initial count
 */
int audioSample_get(audioSample_t *pThis, chunk_d_t *pchunk_rx) {

  int count = 0;

  /** Check if we are not copying more than our audio sample */
  pThis->count += pchunk_rx->bytesMax;
  if ( pThis->count < pThis->size ) {
    /** If we can copy entire chunk then we set count to chunk size */
    count = pchunk_rx->bytesMax;
  } else {
    /* If we cannot copy entire chunk we set count tothe number of bytes that can be read */
    count = pThis->size - (pThis->count - pchunk_rx->bytesMax);
    pThis->count = pThis->size;
  }

  /** Copy the sound samples to chunk */
  memcpy( &pchunk_rx->u32_buff[0], &pThis->pmem[pThis->count - count], count);

  /*
  int i =0;
  for(i=0; i<(count/4); i++){
  printf("The chunk data[%d] is %d\n",i, pchunk_rx->u32_buff[i]);
  } */
  pchunk_rx->bytesUsed = count;
  
  if (pThis->count >= pThis->size)
    pThis->count = 0;

  return count;
}

