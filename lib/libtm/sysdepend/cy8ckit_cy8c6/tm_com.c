/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (CY8CKIT-062S2-43012)
 */

#include <tk/typedef.h>
#include <sys/sysdef.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef CY8CKIT_CY8C6
#if TM_COM_SERIAL_DEV

#include "cybsp.h"
#include "cycfg_clocks.h"

LOCAL	CySCB_Type		*scb = SCB5;
#define SCB_INTR_TX_EMPTY	(0x00000010)
#define SCB_INTR_TX_NOT_FULL	(0x00000002)
#define SCB_INTR_RX_NOT_EMPTY	(0x00000004)
#define SCB_RX_FIFO_STATUS_USED	(0x000000FF)

/* Communication speed */
#define UART_BAUD		(115200)		/* 115200 bps */

#if   UART_BAUD==115200
#define DIVIDER_SCB5		(108U)
#elif UART_BAUD==57600
#define DIVIDER_SCB5		(216U)
#elif UART_BAUD==38400
#define DIVIDER_SCB5		(325U)
#elif UART_BAUD==19200
#define DIVIDER_SCB5		(650U)
#elif UART_BAUD==9600
#define DIVIDER_SCB5		(1301U)
#elif UART_BAUD==2400
#define DIVIDER_SCB5		(5207U)
#elif UART_BAUD==1200
#define DIVIDER_SCB5		(10416U)
#endif

LOCAL	W	fifoNo;		// Number of FIFO entries
EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( Cy_SCB_GetNumInTxFifo(scb_5_HW) >= fifoNo );	// Wait for free space in FIFO.
		scb->TX_FIFO_WR = *b;
	}
	while( Cy_SCB_GetNumInTxFifo(scb_5_HW) != 0 );			// Wait for Tx to complete.
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while( (scb->INTR_RX & SCB_INTR_RX_NOT_EMPTY) == 0 );	// wait for Receive
		*buf = (UB)(scb->RX_FIFO_RD);
		if( (scb->RX_FIFO_STATUS & SCB_RX_FIFO_STATUS_USED) == 0 ){ // RX FOFO Empty?
			scb->INTR_RX = SCB_INTR_RX_NOT_EMPTY;		//  --> Clear Flag
		}
	}
}

cy_stc_scb_uart_context_t CYBSP_UART_context;		// UART context structure
EXPORT	void	tm_com_init(void)
{
	/* Set to "16 bit Divider 0" to specify the SCB5 baud rate. */
	Cy_SysClk_PeriphDisableDivider(	CY_SYSCLK_DIV_16_BIT, 0U);
	Cy_SysClk_PeriphSetDivider(	CY_SYSCLK_DIV_16_BIT, 0U, DIVIDER_SCB5);
	Cy_SysClk_PeriphEnableDivider(	CY_SYSCLK_DIV_16_BIT, 0U);

	/* Configure and enable the UART peripheral */
	Cy_SCB_UART_Init(scb_5_HW, &scb_5_config, NULL);
	Cy_SCB_UART_Enable(scb_5_HW);
	/*----------------------------------------------------------------------*/
	/* SCB : Serial Communication Block					*/
	/* Cy_SCB_UART_Init() and Cy_SCB_UART_Enable() are generated for SCB5	*/
	/* using Device Configurator.						*/
	/* "Device Configurator" is a tool of ModusToolbox.			*/
	/*----------------------------------------------------------------------*/

	fifoNo = Cy_SCB_GetFifoSize(scb_5_HW);
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* CY8CKIT_CY8C6 */
#endif /* USE_TMONITOR */
