/**
 *@file chunk_d.h

 *
 *@brief 
 *  - dynamic sized chunk
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author  Gunar Schirner
 *          Rohan Kangralkar
 *          Oleg Vaskevich
 * @date 	03/15/2009
 *
 * LastChange:
 * $Id: chunk_d.h 1069 2013-02-25 18:19:21Z ovaskevi $
 *
 *******************************************************************************/

#ifndef _CHUNK_D_H_
#define _CHUNK_D_H_

/**
 * Chunk status enumeration 
 */
typedef enum {
	START, /** startup */
	PROGRESS, /** in progress -- don't touch otherwise */
	COMPLETE, /** finished processing */
	FREE
/** free */
} e_buff_status_d_t;

/** Chunk Object
 */
typedef struct {
	/* define a union to have different acess to same data in chunk */
	union {
		unsigned char *u08_buff; /** Unsigned Data Chunk */
		unsigned short *u16_buff;
		unsigned int *u32_buff;
		signed char *s08_buff; /** Signed Data Chunk */
		signed short *s16_buff;
		signed int *s32_buff;
	};
	int bytesMax; /** total number bytes in chunk */
	int bytesUsed; /** used bytes in chunk (fill level) */
	e_buff_status_d_t e_status; /** status */

} chunk_d_t;

/** Initialize buffer chunk
 *    - set max size of buffer and the current fill level
 * Parameters:
 * @param pThis  pointer to own object
 *
 * @return PASS/Zero on success.
 * FAIL/Negative value on failure.
 */
int chunk_d_init(chunk_d_t *pThis, int chunkSize);

/** copy on chunk into nother 
 *@param pSrc  pointer to source object (will not be modified)
 *@param pDst  pointer to destination object (will get the data of the src object)
 *
 *@return PASS/0 success, non-zero otherwise
 **/
int chunk_d_copy(chunk_d_t *pSrc, chunk_d_t *pDst);

#endif
