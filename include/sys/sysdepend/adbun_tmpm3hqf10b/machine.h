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
 */

/*
 *	machine.h
 *
 *	Machine type definition (ADBUN TMPM3HQF10B depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- ADBUN TMPM3HQF10B (CPU: TMPM3HQF10B) definition ----- */

#define ADBUN_TMPM3HQF10B		1				/* Target system : ADBUN TMPM3HQF10B */
#define CPU_TMPM3HQF10B		1				/* Target CPU : Toshiba TMPM3HQF10B */
#define CPU_CORE_ARMV7M		1				/* Target CPU-Core type : ARMv7-M */
//#define CPU_CORE_ACM3		1				/* Target CPU-Core : ARM Cortex-M3 */

#define TARGET_DIR		adbun_tmpm3hqf10b			/* Sysdepend-Directory name */

/*
 **** CPU-depeneded profile (TMPM3HQF10B)
 */
#include "../cpu/tmpm3hqf10b/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
