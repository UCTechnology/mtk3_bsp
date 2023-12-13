/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_M4NR

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_base.c
 *	Serial communication device driver
 *	Basic functions for UART control.
 */

EXPORT UW	sio_mode[DEV_SER_UNITNM];	/* Serial device mode (Communication Parameters) */
EXPORT UW	sio_baud[DEV_SER_UNITNM];	/* Serial Speed (Baudrate) */
EXPORT UW	sio_flow[DEV_SER_UNITNM];	/* Serial Flow control */
EXPORT UW	sio_error[DEV_SER_UNITNM];	/* Serial line states */
//EXPORT UW	sio_break[DEV_SER_UNITNM];	/* Send BREAK (break signal length(ms)) */

//EXPORT TMO	sio_TxTmo[MAX_UART_NO];		/* DN_RSSNDTMO : Tx Timeout */
//EXPORT TMO	sio_RxTmo[MAX_UART_NO];		/* DN_RSRCVTMO : Rx Timeout */

/*
 *	Initialize communication parameters
 */
EXPORT	void	initSioParam( W port )
{
	sio_baud[port] = 115200;
	sio_mode[port] = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;	/* 8N1 */
	sio_flow[port] = UART_HW_FLOW_DISABLE;	

#if	0
	sio_break[port]	= 2*1000;		/* msec for sending BREAK signal */

	/* Initialize the timeout interval when transmitting 1 byte. */
	sio_TxTmo[port] = TMO_FEVR;		/* DN_RSSNDTMO : Tx Timeout */
	sio_RxTmo[port] = TMO_FEVR;		/* DN_RSRCVTMO : Rx Timeout */
#endif
}

/*
 *	Initialize UARTx
 */
EXPORT	ER	initUartBase( W port )
{
	UW	wl;		/* Data Word Length	DEV_SER_MODE_[7|8]BIT */
	UW	par;		/* Parity		DEV_SER_MODE_[PODD|PEVEN|PNON] */
	UW	sb;		/* Stop Bit		DEV_SER_MODE_[1|2]STOP */
	UW	flow;		/* Flow Control		DEV_SER_MODE_[CTS|RTS]EN */
							/* --> ser_mode_sysdep.h */
	wl   = sio_mode[port] &  UART_CR0_SM_MASK;
	par  = sio_mode[port] & (UART_CR0_EVEN_MASK|UART_CR0_PE_MASK);
	sb   = sio_mode[port] &  UART_CR0_SBLEN_MASK;
	flow = sio_flow[port] & (UART_CR0_CTSE_MASK|UART_CR0_RTSE_MASK);

	initUartParam( port, wl, par, sb, sio_baud[port], flow );

	return E_OK;
}

/*
 *	Set SYSTEM REGISTERS necessary for UARTx operation.
 */
EXPORT	ER	systemInit( W port )
{
	ER	ercd;

	uartInitIRQ(port);		/* Initialize IRQ for UARTx	*/

	ercd = initUartRcc(port);	/* Initialize RCC to use UARTx	*/
	if( ercd < 0 ){
		return ercd;
	}

	ercd = initUartGpio(port);	/* Initialize GPIO to use UARTx	*/
	if( ercd < 0 ){
		return ercd;
	}

	ercd = initUartBase(port);	/* Initialize UARTx		*/
	return ercd;
}

#endif	/* DEV_SER_ENABLE */
#endif	/* CPU_M4NR */
