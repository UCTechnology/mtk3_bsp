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
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (ADBUN_M4NR depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- AdBun-M4NR:TMPM4NR Evaluation Board (CPU: TMPM4NRF20FG) definition ----- */

#define ADBUN_M4NR		1		/* Target system : ADBUN-M4NR */
#define CPU_M4NR		1		/* Target CPU : TMPM4NR */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core : ARM Cortex-M4 */

#define TARGET_DIR		adbun_m4nr	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		m4nr		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (TMPM4NR)
 */
#include <sys/sysdepend/cpu/m4nr/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
