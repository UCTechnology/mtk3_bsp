/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
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

/*
 *	sysdef.h
 *
 *	System dependencies definition (XMC7200D-E272K8384 depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/xmc7200/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

#define CLK_FAST		(340)		/* MHz : --> Cortex-M7	*/
#define CLK_PERI		(100)		/* MHz : --> Peripheral	*/
#define CLK_SLOW		(100)		/* MHz : --> Cortex-M0+	*/

/* Clock frequency */
#define	SYSCLK			(CLK_FAST)	/* System clock */
#define PCLK			(CLK_PERI)	/* Peripheral Clock */
#define TMCLK			(SYSCLK)	/* Core Clock */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */

/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
