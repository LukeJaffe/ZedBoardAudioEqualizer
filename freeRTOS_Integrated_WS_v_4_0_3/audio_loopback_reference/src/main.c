/**
 *@file main.c
 *
 *@brief
 *  - Audio TX from sound samples (pre-recorded audio data)
 * 
 * 1. Configure I2C/I2S/FIFO to communicate with Codec
 * 2. Read "sound sample" from file and store into the available chunks.
 * 3. Begin Transfer of the filled chunk to the FIFO.
 * 4. Loop the process and transfer to FIFO via Tx ISR.
 *
 * Target:   Zynq Zedboard
 * IDE: Xilinx SDK 2015.4
 *
 * @author    Rohan Kangralkar, ECE, Northeastern University  (03/11/09)
 * @date 03/23/2016
 *
 * LastChange:
 * $Id: main.c 1009 2016-04-03 20:00:02Z surya2891 $
 *
 *******************************************************************************/

#include "audioPlayer.h"
#include "audioSample.h"
#include "bufferPool_d.h"
#include "zedboard_freertos.h"

#define VOLUME_MIN (0x2F)
#define numChunks 561
#define chunkSize 511

/******************************************************************************
 *                     GLOBALS
 *****************************************************************************/
/**
 * @var audioPlayer
 * @brief  global audio player object - Resolve the instance for more information.
 */
    
audioPlayer_t            audioPlayer;


int main(void)
{
    printf("[MAIN]: Starting Audio Player\r\n");

    audioPlayer_init(&audioPlayer);

    audioPlayer_start(&audioPlayer);

	// start the OS scheduler to kick off the tasks.
	vTaskStartScheduler();
	return(0);

}
