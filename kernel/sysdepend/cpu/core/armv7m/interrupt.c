/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7M
/*
 *	interrupt.c (ARMv7-M)
 *	Interrupt control
 */

#include "kernel.h"
#include "../../../sysdepend.h"

#if !USE_STATIC_IVT
/* ------------------------------------------------------------------------ */
/*
 * Exception handler table (RAM)
 */
#pragma location = ".data_vector"
EXPORT UW exchdr_tbl[N_SYSVEC + N_INTVEC];

#endif

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler
 */

Noinit(LOCAL FP knl_inthdr_tbl[N_INTVEC]);	/* HLL Interrupt Handler Table */

EXPORT void knl_hll_inthdr(void)
{
	FP	inthdr;
	UW	intno;

	ENTER_TASK_INDEPENDENT;

	intno	= knl_get_ipsr() - 16;
	inthdr	= knl_inthdr_tbl[intno];

	(*inthdr)(intno);

	LEAVE_TASK_INDEPENDENT;
}


/* ------------------------------------------------------------------------ */
/*
 * System-timer Interrupt handler
 */
EXPORT void knl_systim_inthdr(void)
{
	ENTER_TASK_INDEPENDENT;

	knl_timer_handler();

	LEAVE_TASK_INDEPENDENT;
}

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	volatile FP	*intvet;

	if(inthdr != NULL) {
		if ( (intatr & TA_HLNG) != 0 ) {
			knl_inthdr_tbl[intno] = inthdr;
			inthdr = knl_hll_inthdr;
		}		
	} else 	{	/* Clear interrupt handler */
		inthdr = Default_Handler;
	}
	intvet = (FP*)&exchdr_tbl[N_SYSVEC];
	intvet[intno] = inthdr;

	return E_OK;
}

/* ----------------------------------------------------------------------- */
/*
 * Return interrupt handler (Used in tk_ret_int())
 */
EXPORT void knl_return_inthdr(void)
{
	/* No processing in ARM. */
	return;
}

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	/* Register exception handler used on OS */

	return E_OK;
}



#endif	/* CPU_CORE_ARMV7M */
