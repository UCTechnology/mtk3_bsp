/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_M4GR

/*
 *	int.c
 *
 *	Interrupt controller (TMPM4GRF20FG )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include "../core/armv7m/int_armv7m.h"

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */

/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	EnableInt_nvic( intno, level);
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	DisableInt_nvic( intno);
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	ClearInt_nvic(intno);		/* Un-pends the associated interrupt */
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 */
EXPORT BOOL CheckInt( UINT intno )
{
	return CheckInt_nvic( intno);
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
}

#endif /* CPU_M4GR */
