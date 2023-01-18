/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CY8C6

/*
 *	int_cy8c6.c
 *
 *	Interrupt controller (CY8C6)
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
	EnableInt_nvic(intno, level);
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	DisableInt_nvic(intno);
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	ClearInt_nvic(intno);
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
#if TK_SUPPORT_INTMODE
EXPORT void SetIntMode(UINT intno, UINT mode)
{
}
#endif

#endif /* CPU_CY8C6 */
