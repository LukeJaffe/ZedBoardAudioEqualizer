/**
 *@file bufferPool_d.c
 *
 *@brief
 *  - Dynamic global buffer pool divided into chunks and kept on the free list
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 *          Oleg Vaskevich
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: bufferPool_d.c 1069 2013-02-25 18:19:21Z ovaskevi $
 *
 *******************************************************************************/
#include <stdlib.h>
#include "bufferPool_d.h"
#include "zedboard_freertos.h"


#define malloc(size) pvPortMalloc(size)

/** Initialize buffer pool 
 *    - initialize freeList, populate with chunks
 *
 * Parameters:
 * @param pThis  pointer to buffer pool data structure
 *
 * @return PASS/Zero on success.
 * FAIL/Negative value on failure.
 */
int bufferPool_d_init(bufferPool_d_t *pThis, int numChunks, int chunkSize) {
	int count = 0;

	pThis->bytesPerChunk = chunkSize;

	/* allocate memory for all chunk data structures*/
	pThis->buffer = (chunk_d_t*) malloc(numChunks * sizeof(chunk_d_t));

	// Init freelist queue
	// Note: queue will contain pointer to chunk structure
	pThis->freeList = xQueueCreate( numChunks, sizeof(chunk_d_t*) );

	if (pThis->freeList == 0) {
		printf("[BP_d]: Failed to initialize free list\n");
		return -1;
	}

	/* We put all the chunk on the free list */
	for (count = 0; count < numChunks; count++) {
		/* pointer to currently operated on chunk */
		chunk_d_t *pChunk = &pThis->buffer[count];

		/* allocate memory for data buffer for this chunk */
		pChunk->u08_buff = malloc(chunkSize);

		// init chunk
		if (-1 == chunk_d_init(pChunk, chunkSize)) {
			printf("Failed to initialize chunk %d/%d \n", count, numChunks);
			return -1;
		}

		// put initialized chunk into queue
		if(xQueueSend( pThis->freeList, &pChunk, NULL ) != pdPASS) {
			printf("Failed to put chunk %d/%d \n", count, numChunks);
			return -1;
		}
	}

	printf("[BP_d]: Initialized\n");
	return PASS;
}

/** Get a chunk from the  buffer pool 
 *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param ppChunk  pointer pointer to chunk acquired (null if empty)
 * @param chunkSize the size of each chunk
 *
 * @return PASS/Zero on success.
 * Negative FAIL/value on failure.
 */
int bufferPool_d_acquire(bufferPool_d_t *pThis, chunk_d_t **ppChunk) {
	if (NULL == pThis || NULL == ppChunk) {
		printf("[BP_d]: Acquire failed\n");
		return -1;
	}

	if( !xQueueReceive( pThis->freeList, ppChunk, ( TickType_t ) 0)) {
		//printf("[BP_d]: No free buffer pool samples avaialable\n");
		*ppChunk = NULL;
		return -1;
	}
	/* declare that chunk is  empty */
	(*ppChunk)->bytesMax  = pThis->bytesPerChunk;
	(*ppChunk)->bytesUsed = 0;
	return 1;
}


int bufferPool_d_acquire_ISR(bufferPool_d_t *pThis, chunk_d_t **ppChunk ) {
	if (NULL == pThis || NULL == ppChunk) {
		printf("[BP_d ISR ]: Acquire failed\n");
		return -1;
	}

	if( !xQueueReceiveFromISR( pThis->freeList, ppChunk, NULL)) {
		printf("[BP_d ISR]: No free buffer pool samples available\n");
		*ppChunk = NULL;
		return -1;
	}
	/* declare that chunk is  empty */
	(*ppChunk)->bytesMax  = pThis->bytesPerChunk;
	(*ppChunk)->bytesUsed = 0;
	return 1;
}

/** Release chunk into the free list 
 *    - non blocking 
 *    - error on null passed 
 *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param pChunk    pointer to chunk to release
 *
 * @return PASS/Zero on success.
 * FAIL/Negative value on failure.
 */
int bufferPool_d_release(bufferPool_d_t *pThis, chunk_d_t *pChunk) {
	if (NULL == pThis || NULL == pChunk) {
		printf("[BP_d]: Acquire failed\n");
		return -1;
	}

	if(xQueueSend( pThis->freeList, &pChunk, ( TickType_t ) 10 ) != pdPASS) {
		printf("Error in releasing the chunk to the freelist\n");
		pChunk = NULL;
		return -1;
	}
	return 1;
}

/** Release chunk into the free list (called from ISR only)
 *    - non blocking
 *    - error on null passed
  *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param pChunk    pointer to chunk to release
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_d_release_from_ISR(bufferPool_d_t *pThis, chunk_d_t *pChunk) {
	if (NULL == pThis || NULL == pChunk) {
		printf("[BP_d]: Acquire failed\n");
		return -1;
	}

	if(xQueueSendFromISR( pThis->freeList, &pChunk, ( TickType_t ) 10 ) != pdPASS) {
		pChunk = NULL;
		return -1;
	}
	return 1;
}
/** Returns true if buffer pool is empty
 *
 *
 * Parameters:
 * @param pThis  pointer to queue data structure 
 *
 * @return   true (non-zero) if emtpy, 0 if chunks available
 */
int bufferPool_is_empty(bufferPool_d_t *pThis) {
	if (NULL == pThis) {
		printf("[BP_d]: bufferPool_d_is_empty failed \n");
		return -1;
	}

	if (pdFALSE != xQueueIsQueueEmptyFromISR(&pThis->freeList)) {
		printf("[BP_d]: The buffer has free chunks \n");
		return 0;
	}

	return 1;
}

