/**
 *@file bufferPool.h
 *
 *@brief
 *  - manages a fixed size buffer pool for chunks
 *
 * Target:   TLL6537v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author    Rohan Kangralkar
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: bufferPool_d.h 1069 2013-02-25 18:19:21Z ovaskevi $
 *
 *******************************************************************************/
#ifndef _BUFFER_POOL_D_H_
#define _BUFFER_POOL_D_H_

//#include "queue_d.h"
//#include "isrDisp.h"
#include "chunk_d.h"
#include "zedboard_freertos.h"

/***************************************************
            DEFINES
***************************************************/   

/***************************************************
            DATA TYPES
***************************************************/

/** bufferPool object
 */
typedef struct {
	QueueHandle_t freeList;
    chunk_d_t    *buffer;
    unsigned int  bytesPerChunk;
} bufferPool_d_t;


/***************************************************
            Access Methods 
***************************************************/

/** Initialize buffer pool 
 *    - initialize freeList, populate with chunks
  *
 * Parameters:
 * @param pThis  pointer to buffer pool
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_d_init(bufferPool_d_t *pThis, int numChunks, int chunkSize);


/** Get a chunk from the  buffer pool 
 *
 * Parameters:
 * @param pThis    pointer to queue data structure
 * @param ppChunk  pointer pointer to chunk aqcuired (null if emtpy)
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int bufferPool_d_acquire(bufferPool_d_t *pThis, chunk_d_t **ppChunk);

int bufferPool_d_acquire_ISR(bufferPool_d_t *pThis, chunk_d_t **ppChunk);
/** Release chunk into the free list 
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
int bufferPool_d_release(bufferPool_d_t *pThis, chunk_d_t *pChunk);

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
int bufferPool_d_release_from_ISR(bufferPool_d_t *pThis, chunk_d_t *pChunk);
/** Returns true if buffer pool is empty
 *
 *
 * Parameters:
 * @param pThis  pointer to queue data structure 
 *
 * @return   true (non-zero) if emtpy, 0 if no chunks available 
 */
int bufferPool_d_is_empty(bufferPool_d_t *pThis );

#endif
