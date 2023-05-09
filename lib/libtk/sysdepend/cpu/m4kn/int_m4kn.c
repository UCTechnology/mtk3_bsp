/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
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
#ifdef CPU_M4KN

/*
 *	int_m4kn.c
 *
 *	Interrupt controller (TMPM4KN)
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


	ClearInt_nvic(intno);

	/* In M367, additional GC-related processing is performed. */






















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
	/* In the case of M367, CG-related processing is being executed. */








































}

#endif /* CPU_M4KN */
