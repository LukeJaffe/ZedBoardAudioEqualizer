/**
 *@file gpio_interrupt.h
 *
 *@brief
 *  - Introduction lab for the TLL6527 platform
 *	- GPIO Pushbutton example
 *
 *
 * Target:   TLL6527v1-1
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 *@author    Rohan Kangralkar, ECE, Northeastern University
 *@date      07/08/2010
 *
 * LastChange:
 * $Id$
 *
 *******************************************************************************/
#ifndef GPIO_INTERRUPT_H
#define GPIO_INTERRUPT_H

#include "zedboard_freertos.h"
#include "audioPlayer.h"

static void gpio_intrHandler(void *arg);
int gpio_init(QueueHandle_t* qptr);

/* macros for zedboard buttons */
#define BTN_LEFT	(0x01<<16)
#define BTN_CENTER	(0x01<<17)
#define BTN_DOWN	(0x01<<18)
#define BTN_RIGHT	(0x01<<19)
#define BTN_UP		(0x01<<20)
#define BTN_ALL		(BTN_LEFT | BTN_CENTER | BTN_DOWN | BTN_RIGHT | BTN_UP)

// /** Base address for GPIO peripheral*/
#define GPIO_BASE 0xe000a000

/// /** Output Data (GPIO Bank2, EMIO) */

/* DATA: This register controls the value to be output when the 
  GPIO signal is configured as an output. All 32 bits of this 
  register are written at one time. */
#define GPIO_DATA_2 (GPIO_BASE + 0x48)

/* DATA: Input Data (GPIO Bank2, EMIO) */
#define GPIO_DATA_RO_2 (GPIO_BASE + 0x68)


/* DIRM: Direction Mode. This controls whether the I/O pin is 
acting as an input or an output. Since the input logic is 
always enabled, this effectively enables/disables the output driver. 
When DIRM[x]==0, the output driver is disabled. */
#define GPIO_DIRM_2 (GPIO_BASE + 0x284)

/* OEN: Output Enable. When the I/O is configured as an output, 
this controls whether the output is enabled or not. 
When the output is disabled, the pin is 3-stated. 
When OEN[x]==0, the output driver is disabled. */
#define GPIO_OEN_2 (GPIO_BASE + 0x288)

/*This register is used to enable or unmask a GPIO input 
for use as an interrupt source. Writing a 1 to any bit of 
this register enables/unmasks that signal for interrupts.
 Reading from this register returns an unpredictable value. 
0: no change
1: clear interrupt mask
*/
#define GPIO_INT_EN_2 (GPIO_BASE + 0x00000290)

#define GPIO_INT_MASK_2 (GPIO_BASE + 0x0000028C)
/*
This register controls
 whether the interrupt is edge sensitive or level sensitive.
 0: level-sensitive 1: edge-sensitive
 */
#define GPIO_INT_TYPE_2 (GPIO_BASE + 0x0000029C)

/* This register controls whether the interrupt 
is active-low or active high
 (or falling-edge sensitive or rising-edge sensitive).
 0: active low or falling edge 1: active high or rising edge

 */
#define GPIO_INT_POLARITY_2 (GPIO_BASE + 0x000002A0)


/* If INT_TYPE is set to edge sensitive, then this register enables an 
interrupt event on both rising and falling edges. 
This register is ignored if INT_TYPE is set to level sensitive.
0: trigger on single edge, using configured interrupt polarity
1: trigger on both edges
*/

#define GPIO_INT_ANY_2 (GPIO_BASE + 0x000002A4)

/* This registers shows if an interrupt event has
occurred or not. Writing a 1 to a bit in this register
clears the interrupt status for that bit. Writing a 0 to a bit in
this register is ignored.
 */
#define GPIO_INT_STAT_2 (GPIO_BASE + 0x00000298)

/*This register is used to disable or mask a GPIO
input for use as an interrupt source. Writing a 1 to any bit
of this register disables/masks that signal for interrupts.
Reading from this register returns an unpredictable value.
*/

#define GPIO_INT_DIS_2 (GPIO_BASE + 0x00000294)



/*
 * The following constants map to the names of the hardware instances that
 * were created in the EDK XPS system.  They are defined here such that
 * the user can easily change all the needed device IDs in one place.
 */
#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define GPIO_INTERRUPT_ID	XPAR_XGPIOPS_0_INTR

/* The following constants define the GPIO banks that are used. */
#define INPUT_BANK	XGPIOPS_BANK0  /* Bank 0 of the GPIO Device */
#define OUTPUT_BANK	XGPIOPS_BANK1  /* Bank 1 of the GPIO Device */

/* The following constants define the positions of the buttons of the GPIO. */
#define GPIO_ALL_BUTTONS	0xFFFF

/*
 * The following constant determines which buttons must be pressed to cause
 * interrupt processing to stop.
 */
#define GPIO_EXIT_CONTROL_VALUE	0x1F


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

int gpio_init(QueueHandle_t* qptr)
{
    // OutEnable for LEDs which is top 8 bits need to be set to 1
    *(volatile u32 *)GPIO_DIRM_2 = 0x00000000;
    *(volatile u32 *)GPIO_OEN_2 =  0x00000000;

    // disable interrupts before configuring new ints
    *(volatile u32 *)GPIO_INT_DIS_2 = 0xFFFFFFFF;

    *(volatile u32 *)GPIO_INT_TYPE_2 = BTN_ALL;
    *(volatile u32 *)GPIO_INT_POLARITY_2 = BTN_ALL;
    *(volatile u32 *)GPIO_INT_ANY_2 = BTN_ALL; // or 0x00000000

    // enable input bits
    *(volatile u32 *)GPIO_INT_EN_2 = BTN_ALL;
    // pointer to driver structure
    XScuGic *pGIC;
    // get pointer to GIC (already initialized at OS startup
    pGIC = prvGetInterruptControllerInstance();
    // connect own interrupt handler to GIC handler
    XScuGic_Connect(pGIC, GPIO_INTERRUPT_ID,
    (Xil_ExceptionHandler) gpio_intrHandler,(void *) qptr);
    // Enable interrupt at GIC
    XScuGic_Enable(pGIC, GPIO_INTERRUPT_ID);
    // Enable IRQ at core (should be enabled anyway)
    Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

    // Enable IRQ in processor core

    return XST_SUCCESS;
}

static void gpio_intrHandler(void *arg)
{
	QueueHandle_t queue = *((QueueHandle_t*)arg);

    // read interrupt status
    u32 int_assert = (*(volatile u32 *)GPIO_INT_STAT_2) & ~(*(volatile u32 *)GPIO_INT_MASK_2);

    // clear interrupts
    (*(volatile u32 *)GPIO_INT_STAT_2) = int_assert;

    // send interrupt status to queue
    xQueueSendFromISR( queue,( void * ) &int_assert, NULL);

    // temporarily disable interrupts
    *(volatile u32 *)GPIO_INT_DIS_2 = 0xFFFFFFFF;

    //unsigned int n;
    //for ( n=0; n < 5000000; n++ )
    //{
    //    asm("nop;");
    //}
}


#endif /* GPIO_INTERRUPT_H_ */
