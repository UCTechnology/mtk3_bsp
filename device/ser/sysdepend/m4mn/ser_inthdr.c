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
 *    Modifications: Adapted to the SBK-M4MN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_M4MN

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_inthdr.c
 *	Serial communication device driver
 *	Interrupt processing for TMPM4MN
 */

/*----------------------------------------------------------------------*/
/*	Register Base Address of UARTx					*/
/*	It is used to specify the register address with UART_XXX(x).	*/
/*----------------------------------------------------------------------*/
IMPORT CONST UW ba[DEV_SER_UNITNM];
IMPORT	UW	sio_error[DEV_SER_UNITNM];	/* Serial line states */

/*----------------------------------------------------------------------*/
/*	Interrupt Handler for UARTx					*/
/*----------------------------------------------------------------------*/
IMPORT	ID	uart_flgid;	/* Event flag ID for UART Interrupt */

EXPORT	W	*uartTxLen[DEV_SER_UNITNM] = { 0 }; /* <-- uct_serial_out() */
EXPORT	UB	*uartTxBuf[DEV_SER_UNITNM];	/* <-- uct_serial_out()	*/
EXPORT	UB	*uartTxBufEnd[DEV_SER_UNITNM];	/* <-- uct_serial_out()	*/

Inline	void	uartTxInthdr( W port )
{
	UB	*tbuf;		/* = uartTxBuf[] <-- serial_out() */
	UB	*tend;		/* = uartTxBufEnd[] <-- serial_out() */
	INT	cnt;		/* Free space in FIFO */
	_UW	sr, cr1;	/* Registers */

	sr  = *UART_SR(port);
	cr1 = *UART_CR1(port);

	if( (sr & UART_SR_TXEND_DONE)&&(cr1 & UART_CR1_INTTXWE_ENABLE) ){
	 /* Transmit data empty && Transmit interrupt enable */
		*UART_SR(port) = UART_SR_TXEND_CLR;	/* clear TXEND */
		tbuf = uartTxBuf[port];		/* Start address of Tx DATA */
		tend = uartTxBufEnd[port];	/* End address of Tx DATA */
		/*------------------------------------------------------*/
		/*	+------------------------------+		*/
		/*	|	    Tx DATA	       |		*/
		/*	+------------------------------+		*/
		/*	 |   --->|			|		*/
		/*	(1st)  tbuf = uartTxBuf[]	tend		*/
		/*					= uartTxBufEnd[]*/
		/*------------------------------------------------------*/
		if( tend <= tbuf ){		/* NO more DATA */
			/* --> Waiting for tx completion and Tx FIFO empty. */
			if( (*UART_SR(port) & (UART_SR_TXRUN_STAT|UART_SR_TLVL_STAT)) == 0 ){
				disableTxInterrupt(port);
				tk_set_flg( uart_flgid, FLG_TX_COMPLETE(port) );
			}
			return;
		}
		cnt = TX_FIFO_SIZE - getTxSizeInFifo(port);
		while( cnt-- > 0 ){
			*UART_DR(port) = (_UW)*tbuf++;	/* write to TxDR */
			if( tend <= tbuf ){		/* No More data */
				break;
			}
		}
		*uartTxLen[port] += tbuf - uartTxBuf[port];/* update transmitted data size */
		uartTxBuf[port] = tbuf;		/* update buf for next turn */
	}
}

Inline	void	uartRxInthdr( W port )
{
	W	freeSz;		/* free size of RxBuf */
	W	rdySz;		/* readable size in RX-FIFO */
	W	cnt;		/* Processed Rx Data size */
	_UW	sr, cr1;	/* Registers */

	sr  = *UART_SR(port);
	cr1 = *UART_CR1(port);

	if( (sr & UART_SR_RXEND_DONE)&&(cr1 & UART_CR1_INTRXWE_ENABLE) ){
	    /* Receive Data Full && Receive Interrupt Enable */
		*UART_SR(port) = UART_SR_RXEND_CLR;	/* clear RXEND */
		freeSz = getRxBufFreeSize(port);
		if( freeSz <= 0 ){			/* Buffer full */
			disableRxInterrupt(port);
			sio_error[port] |= DEV_SER_ERR_ROVR;		/* Buffer over flow */
		}else{
			rdySz = getRxSizeInFifo(port);
			cnt = setRxBufFromDR(port, UART_DR(port), rdySz );
			if( cnt < rdySz ){		/* Buffer full */
				disableRxInterrupt(port);
				sio_error[port] |= DEV_SER_ERR_ROVR;	/* Buffer over flow */
			}
		}
		tk_set_flg( uart_flgid, FLG_RX_READY(port) );
	}					/* --> NOTIFY : uct_serial_in() */
}

#define UART_ERR_OBPF	(UART_ERR_OVRERR_ERR|UART_ERR_PERR_ERR|UART_ERR_FERR_ERR|UART_ERR_BERR_ERR)
#define UART_ERR_ALL	(UART_ERR_TRGERR_ERR|UART_ERR_OBPF)
Inline	void	uartErrInthdr( W port )
{
	_UW	err, cr1;

	/*------ ERROR DETECT & CLEAR ------*/
	err = *UART_ERR(port);
	cr1 = *UART_CR1(port);

	if( (err & UART_ERR_ALL)&&(cr1 & UART_CR1_INTERR_ENABLE) ){
	      /* Error Occurred && Error Interrupt Enable */
		*UART_ERR(port) = UART_ERR_ALL;		/* Reset All Error Bits */
		/* Set Flags for Driver */
		sio_error[port] |= (err & UART_ERR_OBPF);
		/* These are cleared by reading the DN_RSSTAT record. */
	}
}


/*----------------------------------------------------------------------*/
/*	Information of UART Interrupt Handler				*/
/*----------------------------------------------------------------------*/
LOCAL	const	UW	uart_IRQ_tx[DEV_SER_UNITNM] = {
	UART0_TX_IRQ_NUMBER, UART1_TX_IRQ_NUMBER, UART2_TX_IRQ_NUMBER, UART3_TX_IRQ_NUMBER,
};
LOCAL	const	UW	uart_IRQ_rx[DEV_SER_UNITNM] = {
	UART0_RX_IRQ_NUMBER, UART1_RX_IRQ_NUMBER, UART2_RX_IRQ_NUMBER, UART3_RX_IRQ_NUMBER,
};
LOCAL	const	UW	uart_IRQ_err[DEV_SER_UNITNM] = {
	UART0_ER_IRQ_NUMBER, UART1_ER_IRQ_NUMBER, UART2_ER_IRQ_NUMBER, UART3_ER_IRQ_NUMBER,
};

/*
 *	Initialize IRQ for UART_
 */
EXPORT	void	uartInitIRQ( W port )
{
const	INT	level_tx[DEV_SER_UNITNM] = {
			INTPRI_GROUP(UART0_TX_IRQ_PRI, UART0_TX_IRQ_SUBPRI),
			INTPRI_GROUP(UART1_TX_IRQ_PRI, UART1_TX_IRQ_SUBPRI),
			INTPRI_GROUP(UART2_TX_IRQ_PRI, UART2_TX_IRQ_SUBPRI),
			INTPRI_GROUP(UART3_TX_IRQ_PRI, UART3_TX_IRQ_SUBPRI),
		};
const	INT	level_rx[DEV_SER_UNITNM] = {
			INTPRI_GROUP(UART0_RX_IRQ_PRI, UART0_RX_IRQ_SUBPRI),
			INTPRI_GROUP(UART1_RX_IRQ_PRI, UART1_RX_IRQ_SUBPRI),
			INTPRI_GROUP(UART2_RX_IRQ_PRI, UART2_RX_IRQ_SUBPRI),
			INTPRI_GROUP(UART3_RX_IRQ_PRI, UART3_RX_IRQ_SUBPRI),
		};
const	INT	level_err[DEV_SER_UNITNM] = {
			INTPRI_GROUP(UART0_ERR_IRQ_PRI, UART0_ERR_IRQ_SUBPRI),
			INTPRI_GROUP(UART1_ERR_IRQ_PRI, UART1_ERR_IRQ_SUBPRI),
			INTPRI_GROUP(UART2_ERR_IRQ_PRI, UART2_ERR_IRQ_SUBPRI),
			INTPRI_GROUP(UART3_ERR_IRQ_PRI, UART3_ERR_IRQ_SUBPRI),
		};

	ClearInt( uart_IRQ_tx[port] );
	EnableInt( uart_IRQ_tx[port], level_tx[port] );

	ClearInt( uart_IRQ_rx[port] );
	EnableInt( uart_IRQ_rx[port], level_rx[port] );

	ClearInt( uart_IRQ_err[port] );
	EnableInt( uart_IRQ_err[port], level_err[port] );
}

LOCAL	void	uart0TxInthdr( UINT intno ){ uartTxInthdr(0); };
LOCAL	void	uart1TxInthdr( UINT intno ){ uartTxInthdr(1); };
LOCAL	void	uart2TxInthdr( UINT intno ){ uartTxInthdr(2); };
LOCAL	void	uart3TxInthdr( UINT intno ){ uartTxInthdr(3); };

LOCAL	void	uart0RxInthdr( UINT intno ){ uartRxInthdr(0); };
LOCAL	void	uart1RxInthdr( UINT intno ){ uartRxInthdr(1); };
LOCAL	void	uart2RxInthdr( UINT intno ){ uartRxInthdr(2); };
LOCAL	void	uart3RxInthdr( UINT intno ){ uartRxInthdr(3); };

LOCAL	void	uart0ErrInthdr( UINT intno ){ uartErrInthdr(0); };
LOCAL	void	uart1ErrInthdr( UINT intno ){ uartErrInthdr(1); };
LOCAL	void	uart2ErrInthdr( UINT intno ){ uartErrInthdr(2); };
LOCAL	void	uart3ErrInthdr( UINT intno ){ uartErrInthdr(3); };

/*
 *	Register an interrupt handler in the system.
 */
EXPORT	ER	defIntHandler( W port )
{
#if !USE_STATIC_IVT
const	FP	UART_InthdrTx[DEV_SER_UNITNM] = {
		(FP)uart0TxInthdr, (FP)uart1TxInthdr, (FP)uart2TxInthdr, (FP)uart3TxInthdr,
	};
const	FP	UART_InthdrRx[DEV_SER_UNITNM] = {
		(FP)uart0RxInthdr, (FP)uart1RxInthdr, (FP)uart2RxInthdr, (FP)uart3RxInthdr,
	};
const	FP	UART_InthdrErr[DEV_SER_UNITNM] = {
		(FP)uart0ErrInthdr, (FP)uart1ErrInthdr, (FP)uart2ErrInthdr, (FP)uart3ErrInthdr,
	};
	T_DINT	dint = { TA_HLNG, (FP)NULL };
	ER	ercd;

	dint.inthdr = UART_InthdrTx[port];
	ercd = tk_def_int( uart_IRQ_tx[port], &dint );
	if( ercd < 0 ){
		return ercd;
	}

	dint.inthdr = UART_InthdrRx[port];
	ercd = tk_def_int( uart_IRQ_rx[port], &dint );
	if( ercd < 0 ){
		return ercd;
	}

	dint.inthdr = UART_InthdrErr[port];
	return tk_def_int( uart_IRQ_err[port], &dint );
#else
	return E_OK;
#endif
}

/*
 *	Cancel registration of interrupt handler.
 */
EXPORT	ER	undefIntHandler( W port )
{
#if !USE_STATIC_IVT
	ER	ercdTx, ercdRx, ercdEr;
#endif

	DisableInt(uart_IRQ_tx[port]);
//	SetPendingInt(uart_IRQ_tx[port]);
//	 --> To generate an interrupt when the interrupt prohibition is released.

	DisableInt(uart_IRQ_rx[port]);
//	SetPendingInt(uart_IRQ_rx[port]);

	DisableInt(uart_IRQ_err[port]);
//	SetPendingInt(uart_IRQ_err[port]);

#if !USE_STATIC_IVT
	ercdTx = tk_def_int( uart_IRQ_tx[port],  NULL );
	ercdRx = tk_def_int( uart_IRQ_rx[port],  NULL );
	ercdEr = tk_def_int( uart_IRQ_err[port], NULL );
	return (ercdTx < 0)? ercdTx:((ercdRx < 0)? ercdRx:ercdEr);
#else
	return E_OK;
#endif
}

/*
 *	set Interrupt Data to DCB
 */
EXPORT	UINT	getIntNoSnd( W port ){	return uart_IRQ_tx[port];	}
EXPORT	UINT	getIntNoRcv( W port ){	return uart_IRQ_rx[port];	}
EXPORT	UINT	getIntNoErr( W port ){	return uart_IRQ_err[port];	}
EXPORT	UINT	getIntPri( W port ){	return INTPRI_GROUP(UART0_TX_IRQ_PRI, UART0_TX_IRQ_SUBPRI);	}

/*----------------------------------------------------------------------*/
/*		IN/OUT/ERR						*/
/*----------------------------------------------------------------------*/
EXPORT	ER	serial_in( W port, UB *buf, W len, W *alen, W tmout )
{
	W	done;		/* Size read in one function call */
	UINT	flgptn;
	ER	ercd;

	/*--------------------------------------------------------------*/
	/*	Just get the number of data in the receive buffer.	*/
	/*	Received data is NOT read.				*/
	/*--------------------------------------------------------------*/
	if( len <= 0 ){
		*alen = getDataSizeInRxBuf(port);
		return E_OK;
	}

	*alen = 0;
#if	0
	/*--------------------------------------------------------------*/
	/*	SUSPENDING NOT WORKING STATUS				*/
	/*--------------------------------------------------------------*/
	if( (sio_working_flag[port] == RESET_FLAG) || (sio_suspend_flag[port] == SET_FLAG) ){
		return E_IO;
	}
#endif

	/*--------------------------------------------------------------*/
	/*	The abort request already set is canceled.		*/
	/*--------------------------------------------------------------*/
	ercd = tk_clr_flg( uart_flgid, FLG_RX_ABORT(port) );
	if( ercd < 0 ){
		return ercd;
	}

	/*--------------------------------------------------------------*/
	/*	Read data from RX buffer (sio_rxbuf.c)			*/
	/*--------------------------------------------------------------*/
	while( len > 0 ){
		done = getDataFromRxBuf(port, (UB*)buf, len);	/* done = 0, 1, 2, ..., len */
		if( tmout == TMO_POL ){	/* No wait or retry required */
			*alen = done;
			break;
		}
		if( done > 0 ){		/* I got rx data(s)! */
			buf   += done;
			len   -= done;
			*alen += done;
			continue;
		}

		/* NO DATA --> wait Receiving Data or Abort */
		ercd = tk_wai_flg( uart_flgid, FLG_RX_READY(port)|FLG_RX_ABORT(port),
						TWF_ORW|TWF_BITCLR, &flgptn, tmout );
		if( ercd < 0 ){
			break;
		}else{
			if( flgptn & FLG_RX_ABORT(port) ){
				ercd = E_IO;		/* ABORT! */
				break;
			}
			/* flgptn = FLG_RX_READY(port) */	/* received data */
			/* --> READ DATA */
		}
	}

	return ercd;
}

EXPORT	ER	serial_out( W port, UB *buf, W len, W *alen, W tmout )
{
	UINT	flgptn;
	ER	ercd = E_OK;

	*alen = 0;
	if( len <= 0 ){
		return E_OK;
	}

#if	0
	/*--------------------------------------------------------------*/
	/*	SUSPENDING NOT WORKING STATUS				*/
	/*--------------------------------------------------------------*/
	if( (sio_working_flag[port] == RESET_FLAG) || (sio_suspend_flag[port] == SET_FLAG) ){
		return E_IO;
	}
#endif

	/*--------------------------------------------------------------*/
	/*	The abort request already set is canceled.		*/
	/*--------------------------------------------------------------*/
	ercd = tk_clr_flg( uart_flgid, FLG_TX_ABORT(port) );
	if( ercd < 0 ){
		return ercd;
	}

	/*--------------------------------------------------------------*/
	/*	Transmit Start						*/
	/*--------------------------------------------------------------*/
#if	0
	/* If an interrupt occurs when the transmit interrupt is set to Enable */
	uartTxLen[port]    = alen;		/* Number of data written in FIFO */
	uartTxBuf[port]    = buf;		/* Start address of Tx DATA */
	uartTxBufEnd[port] = buf + len;		/* End address of Tx DATA */

	enableTxInterrupt(port);
#else
	/* If no interrupt is generated even if the transmit interrupt is set to Enable */
	uartTxLen[port]    = alen;		/* Number of data written in FIFO */
	uartTxBuf[port]    = buf + 1;		/* Start address of Tx DATA */
	uartTxBufEnd[port] = buf + len;		/* End address of Tx DATA */

	enableTxInterrupt(port);

	*UART_DR(port) = buf[0];		/* Start transmission */
#endif

	ercd = tk_wai_flg( uart_flgid, FLG_TX_COMPLETE(port)|FLG_TX_ABORT(port),
					TWF_ORW|TWF_BITCLR, &flgptn, tmout );

	disableTxInterrupt(port);		/* Disable transmission interrupt anyway */
	uartTxLen[port] = NULL;

	return ercd;
}

EXPORT	ER	serial_err( W port )
{
	UW	com_err;

	com_err = sio_error[port];
	sio_error[port] = 0;

	return com_err;
}

#endif	/* DEV_SER_ENABLE */
#endif	/* CPU_M4MN */
