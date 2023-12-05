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
 *    Modifications: Adapted to the ADBUN-M4GR.
 *    Modified by UC Technology at 2023/09/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (ADBUN_M4GR depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- AdBun-M4GR:TMPM4GR Evaluation Board (CPU: TMPM4GRF20FG) definition ----- */

#define ADBUN_M4GR		1		/* Target system : ADBUN-M4GR */
#define CPU_M4GR		1		/* Target CPU : TMPM4GR */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core : ARM Cortex-M4F */

#define TARGET_DIR		adbun_m4gr	/* Sysdepend-Directory name */
#define TARGET_CPU_DIR		m4gr		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (TMPM4GR)
 */
#include <sys/sysdepend/cpu/m4gr/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
