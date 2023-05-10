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
 *    Modifications: Adapted to the SBK-M4MN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (SBK-M4MN depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Nucleo-64 STM32L467 (CPU: STM32L476) definition ----- */

#define SBK_M4MN		1		/* Target system : SBK-M4MN */
#define CPU_M4MN		1		/* Target CPU : TMPM4MN */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core : ARM Cortex-M4 */

#define TARGET_DIR		sbk_m4mn	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		m4mn		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (TMPM4MN)
 */
#include <sys/sysdepend/cpu/m4mn/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
