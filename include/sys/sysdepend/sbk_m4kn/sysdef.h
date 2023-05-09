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
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (SBK-M4KN depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/m4kn/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

/* Clock frequency */
#define TMCLK			(160)		/* System timer clock input (MHz) */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */

/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
