/**
 *@file chunk_d.c
 *
 *@brief
 *  - dynamic chunk initializationand copy routenes
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 *          Oleg Vaskevich
 * @date 	05/14/2012
 *
 * LastChange:
 * $Id: chunk_d.c 1069 2013-02-25 18:19:21Z ovaskevi $
 *
 *******************************************************************************/
#include "chunk_d.h"
#include <stdio.h>

/** Initialize buffer chunk
 *    - set max size of buffer and the current fill level
 * Parameters:
 * @param pThis  pointer to own object
 * @param chunkSize	size of provided chunk
 *
 * @return PASS/Zero on success.
 * FAIL/Negative value on failure.
 */
int chunk_d_init(chunk_d_t *pThis, int chunkSize) {
	if (NULL == pThis->u32_buff) {
		return -1;
	}

	pThis->bytesMax  = chunkSize;
	pThis->bytesUsed = 0; // default not filled
	return 1;
}

/** copy on chunk into nother 
 *@param pSrc  pointer to source object (will not be modified)
 *@param pDst  pointer to destination object (will get the data of the src object)
 *
 *@return PASS/0 success, non-zero otherwise
 **/
int chunk_d_copy(chunk_d_t *pSrc, chunk_d_t *pDst) {
	unsigned int count;
	unsigned int len = pSrc->bytesUsed / 4;

	// manual copy still in place from previous architecture.
	// can probably be replaced with memcpy
	for (count = 0; len > count; count++) {
		pDst->u32_buff[count] = pSrc->u32_buff[count];
	}
	// update length of actual copied data
	pDst->bytesUsed = pSrc->bytesUsed;

	return 1;
}

