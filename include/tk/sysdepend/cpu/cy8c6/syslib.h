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

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (CY8C6 depended)
 */

#ifndef __TK_SYSLIB_CPU_DEPEND_H__
#define __TK_SYSLIB_CPU_DEPEND_H__

#include "../core/armv7m/syslib.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control
 */

/*
 * Interrupt number
 */
#define MIN_NVIC_INTNO	0		/* Minimum value of NVIC interrupt number */
#define MAX_NVIC_INTNO	(N_INTVEC-1)	/* Maximum value of NVIC interrupt number */

/*
 * Interrupt mode ( Use SetIntMode )
 */
#define IM_EDGE		0x0000		/* Edge trigger */
#define IM_HI		0x0002		/* Interrupt at rising edge */
#define IM_LOW		0x0001		/* Interrupt at falling edge */
#define IM_BOTH		0x0003		/* Interrupt at both edge */


/* ------------------------------------------------------------------------ */
/*
 * I/O port access
 *	for memory mapped I/O
 */
Inline void out_w( UW port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( UW port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( UW port, UB data )
{
	*(_UB*)port = data;
}

Inline UW in_w( UW port )
{
	return *(_UW*)port;
}
Inline UH in_h( UW port )
{
	return *(_UH*)port;
}
Inline UB in_b( UW port )
{
	return *(_UB*)port;
}

Inline void and_w( UW port, UW data)
{
	*(_UW*)port &= data;
}
Inline void and_h( UW port, UH data)
{
	*(_UH*)port &= data;
}
Inline void and_b( UW port, UB data)
{
	*(_UB*)port &= data;
}

Inline void or_w( UW port, UW data)
{
	*(_UW*)port |= data;
}
Inline void or_h( UW port, UH data)
{
	*(_UH*)port |= data;
}
Inline void or_b( UW port, UB data)
{
	*(_UB*)port |= data;
}

#endif /* __TK_SYSLIB_DEPEND_H__ */
