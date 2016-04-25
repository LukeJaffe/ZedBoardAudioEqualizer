/**
 *@file audioTx.c
 *
 *@brief
 *  - transmission of audio
 *
 * Target:   TLL6537v1-1      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:    Rohan Kangralkar
 * @date 03/15/2009
 *
 * LastChange:
 * $Id: audioTx.h 812 2013-03-12 02:28:57Z ovaskevi $
 *
 *******************************************************************************/
#ifndef _AUDIO_TX_H_
#define _AUDIO_TX_H_

#include "bufferPool_d.h"
#include "adau1761.h"
#include "audioSample.h"


/***************************************************
            DEFINES
***************************************************/   
/**
 * @def AUDIOTX_QUEUE_DEPTH
 * @brief tx queue depth
 */
#define AUDIOTX_QUEUE_DEPTH 30
#define AUDIORX_QUEUE_DEPTH 30

/***************************************************
            DATA TYPES
***************************************************/

/** audio RX and TX objects
 */
typedef struct {
  QueueHandle_t rx_queue;  /* queue for received buffers */
  QueueHandle_t tx_queue;  /* queue for transmit buffers */
  chunk_d_t        *pPending; /* pointer to pending chunk just in receiving */
  bufferPool_d_t   *pBuffP; /* pointer to buffer pool */
  audioSample_t  audioSample;
  int              running; /* ISR/polling - which one should execute? */
} audioRxTx_t;


/***************************************************
            Access Methods 
***************************************************/

/** Initialize audio tx
 *    - get pointer to buffer pool
 *    - register interrupt handler
 *    - initialize TX queue

 * Parameters:
 * @param pThis  pointer to own object
 * @param pBuffP  pointer to buffer pool to take and return chunks from
 * @param pIsrDisp   pointer to interrupt dispatcher to get ISR registered
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRxTx_init(audioRxTx_t *pThis, bufferPool_d_t *pBuffP);

/** start audio tx
 *   - empthy for now
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRxTx_start(audioRxTx_t *pThis);


/** audio rtx isr  (to be called from dispatcher) 
 *   - get chunk from tx queue
 *    - if valid, release old pending chunk to buffer pool 
 *    - configure DMA 
 *    - if not valide, configure DMA to replay same chunk again
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return None 
 */
void audioRxTx_isr(void *pThis);

/** audioRxTx put
 *   copies filled pChunk into the TX queue for transmission
 *    if queue is full, then chunk is dropped 
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRxTx_put(audioRxTx_t *pThis, chunk_d_t *pChunk);


/** audioRxTx get
 *   copies a filled chunk into pChunk
 *   blocking call, blocks if queue is empty
 *     - get from queue
 *     - copy in to pChunk
 *     - release chunk to buffer pool
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int audioRxTx_get(audioRxTx_t *pThis, chunk_d_t **pChunk);



#endif
