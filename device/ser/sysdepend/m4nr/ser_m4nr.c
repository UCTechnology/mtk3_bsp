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
 *	ser_m4nr.c
 *	Serial communication device driver
 *	System dependent processing for TMPM4KN
 */

/*----------------------------------------------------------------------
 * Device register base address
 */
EXPORT CONST UW ba[DEV_SER_UNITNM] = {
	UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE, UART4_BASE, UART5_BASE
};

/*----------------------------------------------------------------------
 * Device low-level control data
*/
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Spped (bit rate)
} T_DEV_SER_LLDEVCB;

LOCAL T_DEV_SER_LLDEVCB		ll_devcb[DEV_SER_UNITNM];

/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL ER start_com( UW unit, UW mode, UW baud )
{
const	BRDTBL	brdTbl[] = {  /* baud  KEN BRK BRN */
	{115200, 1, 48,   54 }, {57600, 1, 32,  108 }, {38400, 1, 15,  162 },
	{ 19200, 1, 31,  325 }, { 9600, 1, 61,  651 }, { 4800, 1, 59, 1302 },
	{  2400, 1, 53, 2596 }, { 1200, 1, 43, 5208 }, {0,0,0,0}  };
	INT	idx;

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

	/*------------------------------*/
	/*	SW Reset & FIFO CLR	*/
	/*------------------------------*/
	while( (*UART_SWRST(unit) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );
	*UART_SWRST(unit) = UART_SWRST_SWRST_1ST;
	*UART_SWRST(unit) = UART_SWRST_SWRST_2ND;
	while( (*UART_SWRST(unit) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );

	*UART_FIFOCLR(unit) = UART_FIFOCLR_TFCLR | UART_FIFOCLR_RFCLR;

	/*------------------------------*/
	/*	BAUDRATE		*/
	/*------------------------------*/
	*UART_CLK(unit) = UART_CLK_PRSEL_D1;	/* FTx = FT0/1 */
	*UART_BRD(unit) = ((UW)brdTbl[idx].ken << 23)
			| ((UW)brdTbl[idx].brk << 16)
			|  (UW)brdTbl[idx].brn;

	/*------------------------------*/
	/*	PARAMETERS		*/
	/*------------------------------*/
	*UART_CR0(unit) = UART_CR0_HBSEN_DISABLE | UART_CR0_LPB_DISABLE | UART_CR0_NF_NONE
			| UART_CR0_CTSE_DISABLE | UART_CR0_RTSE_DISABLE | UART_CR0_WU_DISABLE
			| UART_CR0_IV_DISABLE | UART_CR0_DIR_LSB
			| mode;

	/*------------------------------*/
	/*	INTERRUPTS -> disable	*/
	/*------------------------------*/
	*UART_CR1(unit) = UART_CR1_TIL_0 | UART_CR1_RIL_1
			| UART_CR1_INTTXFE_DISABLE | UART_CR1_INTTXWE_DISABLE
			| UART_CR1_INTRXFE_DISABLE | UART_CR1_INTRXWE_DISABLE
			| UART_CR1_INTERR_DISABLE
			| UART_CR1_DMATE_DISABLE | UART_CR1_DMARE_DISABLE;

	/* Enable Rx interrupt, but Tx interrupt is NOT ENABLE.	*/
	/* Tx interrupt is enabled when there is a Tx request.	*/
	enableRxRelatedInterrupt(unit);

	enableSndAndRcv(unit);

	return E_OK;
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if(unit != DEVCNF_SER_DBGUN) {
		disableSndAndRcv(unit);
	} else {	/* Used by T-Monitor */
		enableSndAndRcv(unit);
	}
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm )
{
	ER	err = E_OK;

	switch(cmd) {
	case LLD_SER_MODE:	/* Set Communication mode */
		if(unit == 0){	// For UART0, disable RTS/CTS control.
			parm &= ~(DEV_SER_MODE_CTSEN | DEV_SER_MODE_RTSEN);
		}		// UART0 connected via USB2 cannot use RTS/CTS hardware-wise.
		ll_devcb[unit].mode = parm;
		break;
	
	case LLD_SER_SPEED:	/* Set Communication Speed */
		ll_devcb[unit].speed = parm;
		break;
	
	case LLD_SER_START:	/* Start communication */
		err = start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed );
		/* Enable INTERRUPT? */
		break;

	case LLD_SER_STOP:
		disableSndAndRcv(unit);
		undefIntHandler(unit);
		stop_com( unit );
		break;

	case LLD_SER_SEND:	// Write 1st Data & make Tx Interrupt.
		if( *(UART_SR(unit)) & UART_SR_TXRUN_RUN ) {	// Tx in operation?
			err = E_BUSY;
		} else {
			*UART_DR(unit) = parm;			// Set Transmission data
			enableTxInterrupt(unit);		// Enable TXE interrupt
			err = E_OK;
		}
		break;

	case LLD_SER_BREAK:	/* Send Break */
		if( parm ){
			sendBreakSignal(unit);
		}else{
			stopBreakSignal(unit);
		}
		break;
	}

	return err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT	UW	uart_flg_pat = 0;
EXPORT	ID	uart_flgid = 0;			/* Event flag ID for UART Interrupt */
EXPORT ER dev_ser_llinit( T_SER_DCB *p_dcb )
{
CONST	T_CFLG	cflg = { NULL, TA_TPRI|TA_WMUL, 0 };
	W	unit;
	ER	ercd;

	unit = p_dcb->unit;

	disableSndAndRcv(unit);

	if( uart_flgid == 0 ){
		ercd = tk_cre_flg( &cflg );	/* Flag for NOTIFY	*/
		if( ercd < 0 ){
			return ercd;
		}
		uart_flgid = (ID)ercd;
	}
	uart_flg_pat |= 0x00000001ul << unit;

	ercd = initRxBuf(unit);		/* for Rx buffer */
	if( ercd < 0 ){
		return ercd;
	}

	initSioParam(unit);		/* Initialize UART Parameters */
	ercd = systemInit(unit);	/* Set SYSTEM REGISTERS necessary for UARTx operation. */
	if( ercd < 0 ){			/* with Initialize INTERRUPT */
		return ercd;
	}

#if	0
	/* Setting a flag for managing the operational state. */
	sio_working_flag[unit] = SET_FLAG;	/* = able to Tx and Rx */
	sio_suspend_flag[unit] = RESET_FLAG;	/* = able to work (NORMAL) */
#endif

	/* Device Control block Initizlize */
	p_dcb->intno_snd = getIntNoSnd(unit);
	p_dcb->intno_rcv = getIntNoRcv(unit);
	p_dcb->intno_err = getIntNoErr(unit);
	p_dcb->int_pri   = getIntPri(unit);

	/* define Interrupt handler for UARTx */
	/* INTERRUPT is already initialized in systemInit() */
	ercd = defIntHandler(unit);
	if( ercd < 0 ){
		return ercd;
	}

	enableSndAndRcv(unit);		/* Enable UARTx */

	/* Enable Rx interrupt, but Tx interrupt is NOT ENABLE.	*/
	/* Tx interrupt is enabled when there is a Tx request.	*/
	enableRxRelatedInterrupt(unit);

	return ercd;
}

#endif	/* DEV_SER_ENABLE */
#endif	/* CPU_M4NR */
