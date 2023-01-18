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
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (CY8CKIT-062S2-43012 depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/cy8c6/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

/* Clock Frequency in "Device Configurator" */
#define CLK_FAST		(100)		/* MHz : --> Cortex-M4	*/
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
