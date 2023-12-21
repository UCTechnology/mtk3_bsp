/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_XMC7200
/*
 *	ptimer_xmc7200.c
 *
 *	Physical timer (for XMC7200)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#if USE_PTMR

typedef struct {
	UW	baddr;		// Register Base Address
	UINT	mode;		// Timer mode
	UW	limit;		// Counter Limit
	FP	ptmrhdr;	// Timer Hnadler
	PRI	intpri;		// Interrupt priority
	UW	psc;		// Prescaler value
	BOOL	tim32;		// 32-bit timer?
	void	*exinf;		// Extended information
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
};

/*
 * Physical timer interrupt handler
 */
LOCAL void ptmr_int_main( UINT intno, T_PTMRCB *p_cb)
{
}

LOCAL void (* const inthdr_tbl[])() = {
};

LOCAL const UINT intno_tbl[] = {
};

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	return E_OK;
}


IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	return E_OK;
}


EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	return E_OK;
}


EXPORT ER GetPhysicalTimerConfig(UINT ptmrno, T_RPTMR *pk_rptmr)
{
	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_XMC7200 */
