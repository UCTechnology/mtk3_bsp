/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_XMC7200

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"


#if DEV_SER_ENABLE
/*
 *	ser_xmc7200.c
 *	Serial communication device driver
 *	System dependent processing for XMC7200
 */
#define RX_FIFO_TRIGGER	(63)
#define TX_FIFO_TRIGGER	(63)
#define FLOW_FIFO_TRIGGER	(63)

IMPORT	void	clock_set( UW clock_no, UW div_no, UW div_val );
IMPORT	W	get_fifo_size( void );

/*----------------------------------------------------------------------
 * Device register base address
 */
const LOCAL UW ba[DEV_SER_UNITNM] = { UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE, UART4_BASE,
									  UART5_BASE, UART6_BASE, UART7_BASE, UART8_BASE, UART9_BASE, UART10_BASE, };

/*----------------------------------------------------------------------
 * Device data
 */
const LOCAL struct {
	UW		clockno;		// COMM root clocks (Group 1). See chapter"20 Peripheral clocks"
	UINT		intno;		// Interrupt number
	PRI		intpri;		// Interrupt priority
} ll_devdat[DEV_SER_UNITNM] = {
	{ .clockno = 30,	.intno = INTNO_UART0,	.intpri = DEVCNF_UART0_INTPRI	},
	{ .clockno = 31,	.intno = INTNO_UART1,	.intpri = DEVCNF_UART1_INTPRI	},
	{ .clockno = 32,	.intno = INTNO_UART2,	.intpri = DEVCNF_UART2_INTPRI	},
	{ .clockno = 33,	.intno = INTNO_UART3,	.intpri = DEVCNF_UART3_INTPRI	},
	{ .clockno = 34,	.intno = INTNO_UART4,	.intpri = DEVCNF_UART4_INTPRI	},
	{ .clockno = 35,	.intno = INTNO_UART5,	.intpri = DEVCNF_UART5_INTPRI	},
	{ .clockno = 36,	.intno = INTNO_UART6,	.intpri = DEVCNF_UART6_INTPRI	},
	{ .clockno = 37,	.intno = INTNO_UART7,	.intpri = DEVCNF_UART7_INTPRI	},
	{ .clockno = 38,	.intno = INTNO_UART8,	.intpri = DEVCNF_UART8_INTPRI	},
	{ .clockno = 39,	.intno = INTNO_UART9,	.intpri = DEVCNF_UART9_INTPRI	},
	{ .clockno = 40,	.intno = INTNO_UART10,	.intpri = DEVCNF_UART10_INTPRI	},
};


const LOCAL struct {
	UW		baud;		// Baud rate
	UH		dv;			// Divider Value
} baud2dv[] = {
	{ .baud = 115200,	.dv = 212	},
	{ .baud = 57600,	.dv = 424	},
	{ .baud = 38400,	.dv = 634	},
	{ .baud = 19200,	.dv = 1275	},
	{ .baud = 9600,		.dv = 2551	},
	{ .baud = 4800,		.dv = 5103	},
	{ .baud = 2400,		.dv = 10207	},
	{ .baud = 1200,		.dv = 20415	},
	{ .baud = 0,		.dv = 212	},	// Terminator --> default : 115200 bps
};

LOCAL UW getDividerValue( UW baud )
{
	int	i;
	for( i = 0; baud2dv[i].baud != 0; i++ ){
		if( baud == baud2dv[i].baud ){
			break;
		}
	}
	return baud2dv[i].dv;		// The default baud rate is "115200 bps".
}


/*----------------------------------------------------------------------
 * Control SCBx_INTR_TX/RX_MASK register.
 *	1 : Enable Interrupt
 *	0 : Disable Interrupt (MASK)
 */

Inline void EnableTxInterrupt(UW unit, UW factor)
{
	*(_UW*)(ba[unit] + SCBxINTR_TX_MASK) |= factor;
}
Inline void DisableTxInterrupt(UW unit, UW factor)
{
	*(_UW*)(ba[unit] + SCBxINTR_TX_MASK) &= ~factor;
}
Inline void EnableAllTxInterrupt(UW unit)
{
	*(_UW*)(ba[unit] + SCBxINTR_TX_MASK) = SCB_TX_INTR_ALL_MASK;
}
Inline void DisableAllTxInterrupt(UW unit)
{
	*(_UW*)(ba[unit] + SCBxINTR_TX_MASK) = 0x00000000UL;
}

Inline void EnableAllRxInterrupt(UW unit)
{
	*(_UW*)(ba[unit] + SCBxINTR_RX_MASK) = SCB_RX_INTR_ALL_MASK;
}
Inline void DisableAllRxInterrupt(UW unit)
{
	*(_UW*)(ba[unit] + SCBxINTR_RX_MASK) = 0x00000000UL;
}

Inline UW GetInterruptCause(UW unit)
{
    return (*(_UW*)(ba[unit] + SCBxINTR_CAUSE));
}
Inline UW GetRxInterruptStatus(UW unit)
{
    return (*(_UW*)(ba[unit] + SCBxINTR_RX) & SCB_RX_INTR_ALL_MASK);
}
Inline UW GetTxInterruptStatus(UW unit)
{
    return (*(_UW*)(ba[unit] + SCBxINTR_TX) & SCB_TX_INTR_ALL_MASK);
}

Inline void ClearRxInterrupt(UW unit, UW interruptMask)
{
    *(_UW*)(ba[unit] + SCBxINTR_RX) = interruptMask;
    (void)*(_UW*)(ba[unit] + SCBxINTR_RX);
}
Inline void ClearTxInterrupt(UW unit, UW interruptMask)
{
    *(_UW*)(ba[unit] + SCBxINTR_TX) = interruptMask;
    (void)*(_UW*)(ba[unit] + SCBxINTR_TX);
}

/*----------------------------------------------------------------------
 * TX/RX FIFO register.
 */
Inline void WriteTxFifo(UW unit, UW data)
{
	*(_UW*)(ba[unit] + SCBxTX_FIFO_WR) = data;
}
Inline UW GetNumInTxFifo(UW unit)
{
    return ((*(_UW*)(ba[unit] + SCBxTX_FIFO_STATUS)) & SCB_TX_FIFO_STATUS_USED);
}
Inline UW ReadRxFifo(UW unit)
{
	return *(_UW*)(ba[unit] + SCBxRX_FIFO_RD);
}

/*----------------------------------------------------------------------
 * Device low-level control data
 */
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Spped (bit rate)
	W	fifoNo;		// Number of FIFO entries
} T_DEV_SER_LLDEVCB;

LOCAL T_DEV_SER_LLDEVCB		ll_devcb[DEV_SER_UNITNM];

/*----------------------------------------------------------------------
 * Interrupt handler
 */

LOCAL void uart_inthdr(UINT intno)
{
	W		unit;
	UW		isr;
	UW		isrRx;
	UW		isrTx;
	UW		data;
	UINT	ext_intno;

	ext_intno = (*CM7_0_INT_STATUS(intno) & SYSTEM_INT_IDX_MASK);
	if ( ((INTNO_UART1 <= ext_intno) && (ext_intno <= INTNO_UART10)) ) {
	    unit = (ext_intno - (INTNO_UART1 - 1U));
	} else if (INTNO_UART0 == ext_intno) {
	    unit = (ext_intno - INTNO_UART0);
	} else {
		ClearInt(intno);
		return;
	}

	isr   = GetInterruptCause(unit);		// Get interrupt factor
	isrRx = GetRxInterruptStatus(unit);
	isrTx = GetTxInterruptStatus(unit);

	ClearRxInterrupt(unit, SCB_RX_INTR_ALL_MASK);	// Clear SCB Rx Interrupt
	ClearTxInterrupt(unit, SCB_TX_INTR_ALL_MASK);	// Clear SCB Tx Interrupt
	ClearInt(intno);				// Clear NVIC interrupts

	/* Reception process */
	if( isr & SCB_INTR_CAUSE_RX ){
		if( isrRx & SCB_INTR_RX_NOT_EMPTY ){
			data = ReadRxFifo(unit);		// Read from RX FIFO
			dev_ser_notify_rcv(unit, data);		// Notify the main process of data reception.
			ClearRxInterrupt(unit, SCB_INTR_RX_NOT_EMPTY);	// Clear NOT_EMPTY Interrupt
		}
		if( isrRx &= RX_INTR_ERR_MASK ){
			dev_ser_notify_err(unit, isrRx);	// Notify the main process of this error.
			ClearRxInterrupt(unit, RX_INTR_ERR_MASK);	// Clear ERR Interrupt
		}
	}

	/* Transmission process */
	if( isr & SCB_INTR_CAUSE_TX ) {
		if( isrTx & SCB_INTR_TX_NOT_FULL ){		// TX FIFO is NOT FULL
			if( GetNumInTxFifo(unit) < ll_devcb[unit].fifoNo ){ 
				if( dev_ser_get_snddat(unit, &data) ){	// Get Data
					WriteTxFifo(unit, data);	// Write to TX FIFO
				}else{					// NO MORE Tx DATA
					DisableAllTxInterrupt(unit);
				}
			}
		}
	}

}

/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void start_com(UW unit, UW mode, UW speed)
{
	UW	dataWidth;
	UW	parity;
	UW	stop;
	UW	cts;
	UW	rts;

	dataWidth = ((mode & DEV_SER_MODE_DLMASK) >> 28) + 4;	// 5,6,7,8 (DATA_WIDTH + 1)
	switch( mode & DEV_SER_MODE_PBMASK ){
	  case	DEV_SER_MODE_PODD:	parity = (UART_RX_CTRL_ODD_PARITY | UART_RX_CTRL_PARITY_ENABLE);	break;
	  case	DEV_SER_MODE_PEVEN:	parity = (UART_RX_CTRL_EVEN_PARITY | UART_RX_CTRL_PARITY_ENABLE);	break;
	  case	DEV_SER_MODE_PNON:
	  default:			parity = UART_RX_CTRL_PARITY_DISABLE;	break;
	}
	switch( mode & DEV_SER_MODE_SBMASK ){
	  case	DEV_SER_MODE_1_5STOP:	stop = UART_RX_CTRL_STOP_BIT(2);	break;
	  case	DEV_SER_MODE_2STOP:	stop = UART_RX_CTRL_STOP_BIT(3);		break;
	  case	DEV_SER_MODE_1STOP:
	  default:			stop = UART_RX_CTRL_STOP_BIT(1);		break;
	}

	if((mode & DEV_SER_MODE_CTSEN) == DEV_SER_MODE_CTSEN)
	{
		cts = UART_FLOW_CTRL_CTS_ENABLE;
		*GPIO_PRT_CFG(13) |= (CFG_IN_EN << 12U);
	}
	else
	{
		cts = UART_FLOW_CTRL_CTS_DISABLE;
		*GPIO_PRT_CFG(13) &= ~(CFG_IN_EN << 12U);
	}
	if((mode & DEV_SER_MODE_RTSEN) == DEV_SER_MODE_RTSEN)
	{
		rts = FLOW_FIFO_TRIGGER;
		*GPIO_PRT_CFG(13) |= (CFG_STRONG << 8U);
	}
	else
	{
		rts = 0UL;	// 0UL means RTS is Disable
		*GPIO_PRT_CFG(13) &= ~(CFG_STRONG << 8U);
	}

	/* Disable Interrupt */
	DisableAllTxInterrupt(unit);
	DisableAllRxInterrupt(unit);

	/* Clear Interrupt */
	*(_UW*)(ba[unit] + SCBxINTR_RX) = SCB_RX_INTR_ALL_MASK;
	*(_UW*)(ba[unit] + SCBxINTR_TX) = SCB_TX_INTR_ALL_MASK;
	ClearInt(ll_devdat[unit].intno);	// Clear NVIC interrupts

	/* Setup CLOCK for BAUDRATE */
	clock_set(ll_devdat[unit].clockno, 0, getDividerValue(speed));

	/* Configure and enable the UART peripheral */
	/* Rx configurations  */
    *(_UW*)(ba[unit] + SCBxUART_RX_CTRL) = (stop | parity | UART_RX_CTRL_BREAK_WIDTH(10));
    *(_UW*)(ba[unit] + SCBxRX_CTRL) = (dataWidth | RX_CTRL_LSB_FIRST);
    *(_UW*)(ba[unit] + SCBxRX_FIFO_CTRL) = RX_FIFO_CTRL_LEVEL(RX_FIFO_TRIGGER);
    
    /* Tx configurations */
    *(_UW*)(ba[unit] + SCBxTX_CTRL) = (dataWidth | TX_CTRL_LSB_FIRST);
    *(_UW*)(ba[unit] + SCBxUART_TX_CTRL) = (stop | parity);
    *(_UW*)(ba[unit] + SCBxTX_FIFO_CTRL) = TX_FIFO_CTRL_LEVEL(TX_FIFO_TRIGGER);

    /* flow control configurations */
	*(_UW*)(ba[unit] + SCBxUART_FLOW_CTRL) = (cts | UART_FLOW_CTRL_RTS_LOW | UART_FLOW_CTRL_CTS_LOW | UART_FLOW_CTRL_TRIGGER_LEVEL(rts));
	*(_UW*)(ba[unit] + SCBxCTRL) |= CTRL_ENABLED;

    /* FIFO Clear */
	*(_UW*)(ba[unit] + SCBxRX_FIFO_CTRL) |= RX_FIFO_CTRL_CLEAR;
	*(_UW*)(ba[unit] + SCBxRX_FIFO_CTRL) &= ~RX_FIFO_CTRL_CLEAR;
	*(_UW*)(ba[unit] + SCBxTX_FIFO_CTRL) |= TX_FIFO_CTRL_CLEAR;
	*(_UW*)(ba[unit] + SCBxTX_FIFO_CTRL) &= ~TX_FIFO_CTRL_CLEAR;

	/* FIFO Entries */
	ll_devcb[unit].fifoNo = get_fifo_size();
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if( unit != DEVCNF_SER_DBGUN ){
		*(_UW*)(ba[unit] + SCBxCTRL) &= ~CTRL_ENABLED;
	}
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	UINT		intno;
	PRI		intpri;
	ER		err = E_OK;

	intno	= ll_devdat[unit].intno;
	intpri	= ll_devdat[unit].intpri;
	switch(cmd) {
	  case LLD_SER_MODE:	/* Set Communication mode */
		ll_devcb[unit].mode = parm;
		break;
	
	  case LLD_SER_SPEED:	/* Set Communication Speed */
		ll_devcb[unit].speed = parm;			// baud rate
		break;

	  case LLD_SER_START:	/* Start communication */
		start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
		EnableAllRxInterrupt(unit);			// Enable All Rx Interrupt Requests
		EnableInt(intno, intpri);			// Enable Interrupt
		break;

	  case LLD_SER_STOP:
		DisableInt(intno);
		stop_com(unit);
		break;

	  case LLD_SER_SEND:
	    if( GetNumInTxFifo(unit) < ll_devcb[unit].fifoNo ){ // Tx FIFO is not full.
			WriteTxFifo(unit, parm);		// Set Transmission data to FIFO
			EnableAllTxInterrupt(unit);		// Enable All Tx Interrupt Requests
			EnableInt(intno, intpri);		// Enable Interrupt
			err = E_OK;
		} else {
			EnableTxInterrupt(unit, SCB_INTR_TX_NOT_FULL);
			EnableInt(intno, intpri);		// Enable Interrupt
			err = E_BUSY;
		}
		break;

	  case LLD_SER_BREAK:	/* Send Break */
		err = E_NOSPT;
		break;

	  default:		/* UNKNOWN Low-level device control command */
		err = E_PAR;
		break;
	}

	return err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_ser_llinit( T_SER_DCB *p_dcb )
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= uart_inthdr,
	};
	UW	unit;
	ER	err;

	unit = p_dcb->unit;

	if( !(unit == 3) ){
		return E_NOSPT;
	}

	/* UART device initialize (Disable UART & Disable all interrupt) */
	stop_com(unit);

	/* Device Control block Initizlize */
	p_dcb->intno_rcv = p_dcb->intno_snd = ll_devdat[unit].intno;
	p_dcb->int_pri = ll_devdat[unit].intpri;

	/* Interrupt handler definition */
	err = tk_def_int(ll_devdat[unit].intno, &dint);

	return err;
}

#endif		/* DEV_SER_ENABLE */
#endif		/* CPU_XMC7200 */
