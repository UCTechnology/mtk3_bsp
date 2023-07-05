/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/08.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_TMPM3HQF10B

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_tmpm3hqf10.c
 *	Serial communication device driver
 *	System dependent processing for TMPM3HQF10B
 */

LOCAL ER calc_baud(UW baud);

/*----------------------------------------------------------------------
 * Device register base address
 */
const LOCAL UW ba[DEV_SER_UNITNM] = { UART0_BASE};

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
    UINT	inttxno;		// Interrupt number(TX)
    UINT	intrxno;		// Interrupt number(RX)
    UINT	interno;		// Interrupt number(ERR)
    PRI		inttxpri;		// Interrupt priority(TX)
    PRI		intrxpri;		// Interrupt priority(RX)
    PRI		interpri;		// Interrupt priority(ERR)
} ll_devdat[DEV_SER_UNITNM] = {
	{	/* UART0 */
		.inttxno		= INTNO_UART0_TX,
		.intrxno		= INTNO_UART0_RX,
		.interno		= INTNO_UART0_ER,
		.inttxpri		= DEVCNF_UART0_TX_INTPRI,
		.intrxpri		= DEVCNF_UART0_RX_INTPRI,
		.interpri		= DEVCNF_UART0_ER_INTPRI,
	},
};

typedef	struct	_brdTbl {
	INT	baud;		/*   <-- Baud Rate */
	UB	ken;		/*   --> [UARTxBRD]<ken[23:23]> */
	UB	brk;		/* K --> [UARTxBRD]<brk[21:16]> */
	UH	brn;		/* N --> [UARTxBRD]<brn[15:0]> */
} BRDTBL;

const LOCAL BRDTBL	brdTbl[] = {
	{115200, 1, 0x2A, 0x2B  }, {57600, 1, 0x15,   0x56 }, {38400, 1, 0x3F,  0x82 },
	{ 19200, 1, 0x3E, 0x104 }, { 9600, 1, 0x3D,  0x208 }, { 4800, 1, 0x39, 0x410 },
	{  2400, 1, 0x33, 0x820 }, { 1200, 1, 0x25, 0x1040 }, {0,0,0,0}
};

LOCAL UW baud_reg_val = 0;

/*----------------------------------------------------------------------
 * Device low-level control data
*/
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Speed (bit rate)
} T_DEV_SER_LLDEVCB;

LOCAL T_DEV_SER_LLDEVCB		ll_devcb[DEV_SER_UNITNM];


#define RX_FIFO_SIZE		8
#define TX_FIFO_SIZE		8
/*
 *	Get Readable Data Size in Rx FIFO.
 *	Call this function after confirming that there is received data.
 */
Inline	W	getRxSizeInFifo( W unit )
{
	return (in_w( ba[unit] + UARTxSR) & UARTxSR_RLVL_STAT);
				/* [SR]<RLVL>[3:0] */
}

/*
 *	Get Data Size in Tx FIFO to wait Transmit.
 */
Inline	W	getTxSizeInFifo( W unit )
{
	return  ((in_w( ba[unit] + UARTxSR) & UARTxSR_TLVL_STAT) >> 8);
				/* [SR]<TLVL>[11:8] */
}

/*----------------------------------------------------------------------
 * Interrupt handler
 */
/*
 * TXI: Transmit data empty && Transmit interrupt enable
 */
void uart_txi_inthdr( UINT intno)
{
	UW	data;
	W	unit;
	W	cnt;		/* Free space in FIFO */

	unit = (intno -INTNO_UART0_RX)/3;

	if( (in_w( ba[unit] + UARTxSR) & UARTxSR_TXFF_MASK)&&(in_w( ba[unit] + UARTxCR1) & UARTxCR1_INTTXFE_ENABLE) ){
		 /* Transmit data empty && Transmit interrupt enable */
		out_w( ba[unit] + UARTxSR, UARTxSR_TXFF_CLR );	/* clear TXEND */

		cnt = TX_FIFO_SIZE - getTxSizeInFifo(unit);
		while( cnt-- > 0 ){
			if( !dev_ser_get_snddat(unit, &data))  break;
			out_w( ba[unit] + UARTxDR, data );
		}	
	}
}
/*
 * RXI: Receive Data Full && Receive Interrupt Enable
 */
void uart_rxi_inthdr( UINT intno)
{
	UW	data, err;
	W	unit;
	W	cnt;		/* Processed Rx Data size */

	unit = (intno -INTNO_UART0_RX)/3;
	
	if( (in_w( ba[unit] + UARTxSR) & UARTxSR_RXEND_DONE)&&(in_w( ba[unit] + UARTxCR1) & UARTxCR1_INTRXWE_ENABLE) ){
	    /* Receive Data Full && Receive Interrupt Enable */
		out_w( ba[unit] + UARTxSR, UARTxSR_RXEND_CLR );	/* clear RXEND */

		cnt = getRxSizeInFifo(unit);

		while( cnt-- > 0 ){
			data	= in_w(ba[unit] + UARTxDR);
			err	= data >> 16;
			
			if (err) {
				dev_ser_notify_err(unit, err);		/* Notify the main process of the communication error. */
				out_w( ba[unit] + UARTxERR, 0 );	/* Clear error */
				data &= 0x000000FF;
			}
			dev_ser_notify_rcv(unit, data);	/* Notify the main process of data reception. */
		}
	}
}
/*
 * ERI: Error Occurred && Error Interrupt Enable
 */
#define UARTxERR_ALL	(UARTxERR_TRGERR_ERR|UARTxERR_OVRERR_ERR|UARTxERR_PERR_ERR|UARTxERR_FERR_ERR|UARTxERR_BERR_ERR)
void uart_eri_inthdr( UINT intno)
{
	UW	err;
	W	unit;

	unit = (intno -INTNO_UART0_RX)/3;

	err  = in_w( ba[unit] + UARTxERR);
	
	if( (err & UARTxERR_ALL)&&(in_w( ba[unit] + UARTxCR1) & UARTxCR1_INTERR_ENABLE) ){
	    /* Error Occurred && Error Interrupt Enable */
		out_w( ba[unit] + UARTxERR, UARTxERR_ALL );	/* Reset All Error Bits */
	}
}

/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void start_com(UW unit, UW mode, UW speed)
{
	/* Set communication Speed */
	out_w( ba[unit] + UARTxBRD, speed );

	/* Set communication mode */
	out_w( ba[unit] + UARTxCR0, (mode & (UARTxCR0_SM_MASK | UARTxCR0_SBLEN_MASK
		| UARTxCR0_PE_MASK | UARTxCR0_EVEN_MASK)) );

	/* Start communication */
	*(UW*)(ba[unit] + UARTxTRANS) |= (UARTxTRANS_TXE_ENABLE | UARTxTRANS_RXE_ENABLE);
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if(unit != DEVCNF_SER_DBGUN) {
		out_w( ba[unit] + UARTxTRANS, 0 );
	} else {	/* Used by T-Monitor */
		out_w( ba[unit] + UARTxTRANS, UARTxTRANS_TXE_ENABLE | UARTxTRANS_RXE_ENABLE );
	}
}

/*----------------------------------------------------------------------
 * Calcurate Baudrate
 */
LOCAL ER calc_baud(UW baud)
{
	ER err = E_OK;
	INT idx;
	/*------------------------------*/
	/*	Check Baudrate		*/
	/*------------------------------*/
	for( idx = 0; brdTbl[idx].baud != 0; idx++ ){
		if( baud == brdTbl[idx].baud ){
			break;
		}
	}
	if( brdTbl[idx].baud == 0 ){
		return E_PAR;		/* Baudrate Error */
	}

	baud_reg_val = (((UW)brdTbl[idx].ken << 23)| ((UW)brdTbl[idx].brk << 16)| (UW)brdTbl[idx].brn);

    return err;
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	ER	err	= E_OK;

	switch(cmd) {
	case LLD_SER_MODE:	/* Set Communication mode */
		ll_devcb[unit].mode = parm;
		break;
	
	case LLD_SER_SPEED:	/* Set Communication Speed */
		err = calc_baud(parm);
		if (err == E_OK) {
			ll_devcb[unit].speed = baud_reg_val;
		}
		break;
	
	case LLD_SER_START:	/* Start communication */
		out_w( ba[unit] + UARTxTRANS, 0);
		out_w( ba[unit] + UARTxCR0,   0 );
		out_w( ba[unit] + UARTxCR1,   0);
		out_w( ba[unit] + UARTxERR,   0 );
		out_w( ba[unit] + UARTxSR,    UARTxSR_TXEND_CLR | UARTxSR_TXFF_CLR | UARTxSR_RXFF_CLR | UARTxSR_RXEND_CLR );
                out_w( ba[unit] + UARTxFIFOCLR,    UARTxFIFOCLR_TFCLR | UARTxFIFOCLR_RFCLR );

		ClearInt(ll_devdat[unit].inttxno);
		ClearInt(ll_devdat[unit].intrxno);
		ClearInt(ll_devdat[unit].interno);

		out_w( ba[unit] + UARTxCR1,   
			UARTxCR1_TIL_0 | UARTxCR1_RIL_1
			| UARTxCR1_INTTXFE_ENABLE
		        | UARTxCR1_INTRXWE_ENABLE
			| UARTxCR1_INTERR_ENABLE
			| UARTxCR1_DMATE_ENABLE | UARTxCR1_DMARE_ENABLE );

		EnableInt(ll_devdat[unit].inttxno, ll_devdat[unit].inttxpri);	/* Enable Interrupt(TX) */
		EnableInt(ll_devdat[unit].intrxno, ll_devdat[unit].intrxpri);	/* Enable Interrupt(RX) */
		EnableInt(ll_devdat[unit].interno, ll_devdat[unit].interpri);	/* Enable Interrupt(ERR) */
		start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
		break;
	
	case LLD_SER_STOP:
		DisableInt(ll_devdat[unit].inttxno);
		DisableInt(ll_devdat[unit].intrxno);
		DisableInt(ll_devdat[unit].interno);
		stop_com(unit);
		break;

	case LLD_SER_SEND:
		if((TX_FIFO_SIZE - getTxSizeInFifo(unit)) > 0) {
			out_w(ba[unit] + UARTxDR, parm);			/* Set Transmission data */
			err = E_OK;
		} else {
			err = E_BUSY;
		}
		break;
	case LLD_SER_BREAK:	/* Send Break */
		if(parm) {
			*(UW*)(ba[unit] + UARTxTRANS) |= UARTxTRANS_BK_SEND;
		} else {
			*(UW*)(ba[unit] + UARTxTRANS) &= ~UARTxTRANS_BK_SEND;
		}
		break;
	}

	return err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_ser_llinit( T_SER_DCB *p_dcb)
{
	T_DINT		dint;
	UW	unit;
	ER	err;

	unit = p_dcb->unit;

	/* UART device initialize */
	out_w( ba[unit] + UARTxCR0, DEV_SER_MODE_8BIT );
	out_w( ba[unit] + UARTxCR1, UARTxCR1_TIL_0 | UARTxCR1_RIL_1 );
	out_w( ba[unit] + UARTxERR, 0 );
	out_w( ba[unit] + UARTxSR,  UARTxSR_TXEND_CLR | UARTxSR_TXFF_CLR | UARTxSR_RXFF_CLR | UARTxSR_RXEND_CLR );
	ClearInt(ll_devdat[unit].inttxno);
	ClearInt(ll_devdat[unit].intrxno);
	ClearInt(ll_devdat[unit].interno);

	/* Device Control block Initizlize */
	p_dcb->intno_rcv = ll_devdat[unit].intrxno;
	p_dcb->intno_snd = ll_devdat[unit].inttxno;
	p_dcb->int_pri   = ll_devdat[unit].inttxpri;

	/* Interrupt handler definition */
	dint.intatr	= TA_HLNG;
	dint.inthdr	= uart_txi_inthdr;
	err = tk_def_int(ll_devdat[unit].inttxno, &dint);
	dint.inthdr	= uart_rxi_inthdr;
	err = tk_def_int(ll_devdat[unit].intrxno, &dint);
	dint.inthdr	= uart_eri_inthdr;
	err = tk_def_int(ll_devdat[unit].interno, &dint);

	stop_com(unit);

	return err;
}

#endif		/* DEV_SER_ENABLE */
#endif		/* CPU_TMPM3HQF10B */