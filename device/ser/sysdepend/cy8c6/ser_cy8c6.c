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
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#define PROHIBIT_DEF_SIZE_T
typedef __SIZE_TYPE__ size_t;
// "size_t" is multiple define
//	include/tk/syslib.h:125
//	c:\infineon\tools\modustoolbox\tools_2.4\gcc\lib\gcc\arm-none-eabi\10.3.1\include\stddef.h:209
// In order to cancel the duplicate definition of "size_t", the definition in uT-Kernel 3.0 is canceled.

#include <sys/machine.h>
#ifdef CPU_CY8C6

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"

#undef	SCB_SCR
// "SCB_SCR" is multiple define
//	include/sys/sysdepend/cpu/cy8c6/../core/armv7m/sysdef.h:59
//	../mtb_shared/mtb-pdl-cat1/release-v2.4.0/devices/COMPONENT_CAT1A/include/cy_device.h:447
// In order to cancel the duplicate definition of "SCB_SCR", the definition in uT-Kernel 3.0 is canceled.

#include "cybsp.h"
#include "cycfg_clocks.h"

#if DEV_SER_ENABLE
/*
 *	ser_cy8c6.c
 *	Serial communication device driver
 *	System dependent processing for CY8C6
 */

/*----------------------------------------------------------------------
 * Device data
 */
const LOCAL struct {
	CySCB_Type*	scb;		// SCB address
	INT		dno;		// 16 bit DIVIDER Number (for BAUDRATE settings)
	UINT		intno;		// Interrupt number
	PRI		intpri;		// Interrupt priority
} ll_devdat[DEV_SER_UNITNM] = {
	{ .scb = SCB0,	.dno = 0,	.intno = INTNO_UART0,	.intpri = DEVCNF_UART0_INTPRI	},
/*RDY*/	{ .scb = SCB1,	.dno = 1,	.intno = INTNO_UART1,	.intpri = DEVCNF_UART1_INTPRI	},
/*RDY*/	{ .scb = SCB2,	.dno = 2,	.intno = INTNO_UART2,	.intpri = DEVCNF_UART2_INTPRI	},
	{ .scb = SCB3,	.dno = 0,	.intno = INTNO_UART3,	.intpri = DEVCNF_UART3_INTPRI	},
	{ .scb = SCB4,	.dno = 0,	.intno = INTNO_UART4,	.intpri = DEVCNF_UART4_INTPRI	},
/*RDY*/	{ .scb = SCB5,	.dno = 0,	.intno = INTNO_UART5,	.intpri = DEVCNF_UART5_INTPRI	},
	{ .scb = SCB6,	.dno = 0,	.intno = INTNO_UART6,	.intpri = DEVCNF_UART6_INTPRI	},
	{ .scb = SCB7,	.dno = 0,	.intno = INTNO_UART7,	.intpri = DEVCNF_UART7_INTPRI	},
	{ .scb = SCB8,	.dno = 0,	.intno = INTNO_UART8,	.intpri = DEVCNF_UART8_INTPRI	},
	{ .scb = SCB9,	.dno = 0,	.intno = INTNO_UART9,	.intpri = DEVCNF_UART9_INTPRI	},
	{ .scb = SCB10,	.dno = 0,	.intno = INTNO_UART10,	.intpri = DEVCNF_UART10_INTPRI	},
	{ .scb = SCB11,	.dno = 0,	.intno = INTNO_UART11,	.intpri = DEVCNF_UART11_INTPRI	},
	{ .scb = SCB12,	.dno = 0,	.intno = INTNO_UART12,	.intpri = DEVCNF_UART12_INTPRI	},
};


/*----------------------------------------------------------------------
 * 
 */
const LOCAL struct {
	UW		baud;		// Baud rate
	uint32_t	dv;		// Divider Value --> Cy_SysClk_PeriphSetDivider(,,dv)
} baud2dv[] = {
	{ .baud = 115200,	.dv = 108	},
	{ .baud = 57600,	.dv = 216	},
	{ .baud = 38400,	.dv = 325	},
	{ .baud = 19200,	.dv = 650	},
	{ .baud = 9600,		.dv = 1301	},
	{ .baud = 2400,		.dv = 5207	},
	{ .baud = 1200,		.dv = 10416	},
	{ .baud = 0,		.dv = 108	},	// Terminator --> default : 115200 bps
};

LOCAL uint32_t getDividerValue( UW baud )
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
#if	0
#define CY_SCB_TX_INTR_MASK     (CY_SCB_TX_INTR_LEVEL     | CY_SCB_TX_INTR_NOT_FULL  | CY_SCB_TX_INTR_EMPTY     | \
                                 CY_SCB_TX_INTR_OVERFLOW  | CY_SCB_TX_INTR_UNDERFLOW | CY_SCB_TX_INTR_UART_DONE | \
                                 CY_SCB_TX_INTR_UART_NACK | CY_SCB_TX_INTR_UART_ARB_LOST)

#define CY_SCB_RX_INTR_MASK     (CY_SCB_RX_INTR_LEVEL             | CY_SCB_RX_INTR_NOT_EMPTY | CY_SCB_RX_INTR_FULL | \
                                 CY_SCB_RX_INTR_OVERFLOW          | CY_SCB_RX_INTR_UNDERFLOW                       | \
                                 CY_SCB_RX_INTR_UART_FRAME_ERROR  | CY_SCB_RX_INTR_UART_PARITY_ERROR               | \
                                 CY_SCB_RX_INTR_UART_BREAK_DETECT)
#endif
Inline void EnableTxInterrupt(CySCB_Type* scb, UW factor)
{
	Cy_SCB_SetTxInterruptMask(scb,Cy_SCB_GetTxInterruptMask(scb)|factor);
}
Inline void DisableTxInterrupt(CySCB_Type* scb, UW factor)
{
	Cy_SCB_SetTxInterruptMask(scb,Cy_SCB_GetTxInterruptMask(scb)&(!factor));
}
Inline void EnableAllTxInterrupt(CySCB_Type* scb)
{
	Cy_SCB_SetTxInterruptMask(scb,CY_SCB_TX_INTR_MASK);
}
Inline void DisableAllTxInterrupt(CySCB_Type* scb)
{
	Cy_SCB_SetTxInterruptMask(scb,0x00000000);	// 0 : Disable Interrupt (MASK)
}

Inline void EnableRxInterrupt(CySCB_Type* scb, UW factor)
{
	Cy_SCB_SetRxInterruptMask(scb,Cy_SCB_GetRxInterruptMask(scb)|factor);
}
Inline void DisableRxInterrupt(CySCB_Type* scb, UW factor)
{
	Cy_SCB_SetRxInterruptMask(scb,Cy_SCB_GetRxInterruptMask(scb)&(!factor));
}
Inline void EnableAllRxInterrupt(CySCB_Type* scb)
{
	Cy_SCB_SetRxInterruptMask(scb,CY_SCB_RX_INTR_MASK);
}
Inline void DisableAllRxInterrupt(CySCB_Type* scb)
{
	Cy_SCB_SetRxInterruptMask(scb,0x00000000);	// 0 : Disable Interrupt (MASK)
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
#define RX_INTR_ERR	(CY_SCB_RX_INTR_OVERFLOW|CY_SCB_RX_INTR_UART_FRAME_ERROR|CY_SCB_RX_INTR_UART_PARITY_ERROR|CY_SCB_RX_INTR_UART_BREAK_DETECT)
LOCAL void uart_inthdr(UINT intno)
{
	CySCB_Type	*scb;
	W		unit;
	UW		isr;
	UW		isrRx;
	UW		isrTx;
	UW		data;

	if( ((INTNO_UART1 <= intno)&&(intno <= INTNO_UART12)) ){
		unit = intno - INTNO_UART0;
	}else{
		ClearInt(intno);
		return;
	}

	scb = ll_devdat[unit].scb;

	isr   = Cy_SCB_GetInterruptCause(scb);		// Get interrupt factor
	isrRx = Cy_SCB_GetRxInterruptStatus(scb);
	isrTx = Cy_SCB_GetTxInterruptStatus(scb);

	Cy_SCB_ClearRxInterrupt(scb,CY_SCB_RX_INTR_MASK);	// Clear SCB Rx Interrupt
	Cy_SCB_ClearTxInterrupt(scb,CY_SCB_TX_INTR_MASK);	// Clear SCB Tx Interrupt
	ClearInt(intno);				// Clear NVIC interrupts

	/* Reception process */
	if( isr & SCB_INTR_CAUSE_RX_Msk ){
		if( isrRx & CY_SCB_RX_INTR_NOT_EMPTY ){
			data = Cy_SCB_ReadRxFifo(scb);		// Read from RX FIFO
			dev_ser_notify_rcv(unit, data);		// Notify the main process of data reception.
			Cy_SCB_ClearRxInterrupt(scb,CY_SCB_RX_INTR_NOT_EMPTY);	// Clear NOT_EMPTY Interrupt
		}
		if( isrRx &= RX_INTR_ERR ){
			dev_ser_notify_err(unit, isrRx);	// Notify the main process of this error.
			Cy_SCB_ClearRxInterrupt(scb,RX_INTR_ERR);	// Clear ERR Interrupt
		}
	}

	/* Transmission process */
	if( isr & SCB_INTR_CAUSE_TX_Msk ) {
		if( isrTx & CY_SCB_TX_INTR_NOT_FULL ){		// TX FIFO is NOT FULL
			if( dev_ser_get_snddat(unit, &data) ){	// Get Data
				Cy_SCB_WriteTxFifo(scb,data);	// Write to TX FIFO
			}else{					// NO MORE Tx DATA
				DisableTxInterrupt(scb,CY_SCB_TX_INTR_NOT_FULL);
			}
		}
	}
}

/*----------------------------------------------------------------------
 * Make SCB Configuration Data
 *	see libs/TARGET_CY8CKIT-062S2-43012/COMPONENT_BSP_DESIGN_MODUS/GeneratedSource/cycfg_peripherals.c
 */
void	makeScbConfig( cy_stc_scb_uart_config_t *scb_config, UW unit, UW mode )
{
	UW	dataWidth;
	UW	parity;
	UW	stop;
	BOOL	cts;
	UW	rts;

	dataWidth = ((mode & DEV_SER_MODE_DLMASK) >> 28) + 5;	// 5,6,7,8
	switch( mode & DEV_SER_MODE_PBMASK ){
	  case	DEV_SER_MODE_PODD:	parity = CY_SCB_UART_PARITY_ODD;	break;
	  case	DEV_SER_MODE_PEVEN:	parity = CY_SCB_UART_PARITY_EVEN;	break;
	  case	DEV_SER_MODE_PNON:
	  default:			parity = CY_SCB_UART_PARITY_NONE;	break;
	}
	switch( mode & DEV_SER_MODE_SBMASK ){
	  case	DEV_SER_MODE_1_5STOP:	stop = CY_SCB_UART_STOP_BITS_1_5;	break;
	  case	DEV_SER_MODE_2STOP:	stop = CY_SCB_UART_STOP_BITS_2;		break;
	  case	DEV_SER_MODE_1STOP:
	  default:			stop = CY_SCB_UART_STOP_BITS_1;		break;
	}
	cts = (mode & DEV_SER_MODE_CTSEN)? true:false;
	rts = (mode & DEV_SER_MODE_RTSEN)? 63UL:0UL;	// 0UL means RTS is Disable

	// Set SCB Configuration Structure.
	scb_config->uartMode			= CY_SCB_UART_STANDARD;
	scb_config->enableMutliProcessorMode	= false;
	scb_config->smartCardRetryOnNack	= false;
	scb_config->irdaInvertRx		= false;
	scb_config->irdaEnableLowPowerReceiver	= false;
	scb_config->oversample			= 8;
	scb_config->enableMsbFirst		= false;
	scb_config->dataWidth			= dataWidth;		// Data Width
	scb_config->parity			= parity;		// Parity Bit(s)
	scb_config->stopBits			= stop;			// Stop Bit(s)
	scb_config->enableInputFilter		= false;
	scb_config->breakWidth			= 11UL;
	scb_config->dropOnFrameError		= false;
	scb_config->dropOnParityError		= false;
	scb_config->receiverAddress		= 0x0UL;
	scb_config->receiverAddressMask		= 0x0UL;
	scb_config->acceptAddrInFifo		= false;
	scb_config->enableCts			= cts;			// CTS (false,true)
	scb_config->ctsPolarity			= CY_SCB_UART_ACTIVE_LOW;
	scb_config->rtsRxFifoLevel		= rts;			// RTS (0,1-63)
	scb_config->rtsPolarity			= CY_SCB_UART_ACTIVE_LOW;
	scb_config->rxFifoTriggerLevel		= 63UL;
	scb_config->rxFifoIntEnableMask		= 0UL;
	scb_config->txFifoTriggerLevel		= 63UL;
	scb_config->txFifoIntEnableMask		= 0UL;
}


/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void init_scb(UW unit, UW mode, UW speed)
{
	cy_stc_scb_uart_config_t	scb_config;

	/* Setup CLOCK for BAUDRATE */
	Cy_SysClk_PeriphDisableDivider(	CY_SYSCLK_DIV_16_BIT, ll_devdat[unit].dno );
	Cy_SysClk_PeriphSetDivider(	CY_SYSCLK_DIV_16_BIT, ll_devdat[unit].dno, getDividerValue(speed));
	Cy_SysClk_PeriphEnableDivider(	CY_SYSCLK_DIV_16_BIT, ll_devdat[unit].dno );

	/* Configure and enable the UART peripheral */
	makeScbConfig( &scb_config, unit, mode );
	Cy_SCB_UART_Init(ll_devdat[unit].scb, &scb_config, NULL);
	Cy_SCB_UART_Enable(ll_devdat[unit].scb);

	/* FIFO Entries */
	ll_devcb[unit].fifoNo = Cy_SCB_GetFifoSize(ll_devdat[unit].scb);
}

LOCAL void start_com(UW unit)
{
	Cy_SCB_ClearRxInterrupt(ll_devdat[unit].scb,CY_SCB_RX_INTR_MASK);
	Cy_SCB_ClearTxInterrupt(ll_devdat[unit].scb,CY_SCB_TX_INTR_MASK);
	ClearInt(ll_devdat[unit].intno);	// Clear NVIC interrupts

	init_scb( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if( unit != DEVCNF_SER_DBGUN ){
		Cy_SCB_UART_Disable(ll_devdat[unit].scb,NULL);
	}
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	CySCB_Type	*scb;
	UINT		intno;
	PRI		intpri;
	ER		err = E_OK;

	scb	= ll_devdat[unit].scb;
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
		start_com(unit);
		EnableAllRxInterrupt(scb);			// Enable All Rx Interrupt Requests
		EnableInt(intno, intpri);			// Enable Interrupt
		break;

	  case LLD_SER_STOP:
		DisableInt(intno);
		stop_com(unit);
		break;

	  case LLD_SER_SEND:
		if( Cy_SCB_GetNumInTxFifo(scb) < ll_devcb[unit].fifoNo ){ // Tx FIFO is not full.
			Cy_SCB_WriteTxFifo(scb,parm);		// Set Transmission data to FIFO
			EnableAllTxInterrupt(scb);		// Enable All Tx Interrupt Requests
			EnableInt(intno, intpri);		// Enable Interrupt
			err = E_OK;
		} else {
			EnableTxInterrupt(scb,CY_SCB_TX_INTR_NOT_FULL);
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

	// Now that you have changed the parameters, restart SCB.
	if( (cmd == LLD_SER_MODE)||(cmd == LLD_SER_SPEED) ){
		DisableInt(intno);		// Same as LLD_SER_STOP
		stop_com(unit);
		start_com(unit);		// Same as LLD_SER_START
		EnableAllRxInterrupt(scb);
		EnableInt(intno, intpri);
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

	// Only SCB1, SCB2 and SCB5 are available for CY8CKIT-CY8C6.
	if( !((unit == 1)||(unit == 2)||(unit == 5)) ){
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
#endif		/* CPU_CY8C6 */
