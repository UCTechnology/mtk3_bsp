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
 *	machine.h
 *
 *	Machine type definition (CY8CKIT-062S2-43012 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- CY8CKIT-062S2-43012 (CPU: CY8C624ABZI-S2D44A0) definition ----- */

#define CY8CKIT_CY8C6		1		/* Target system : CY8CKIT-062S2-43012 */
#define CPU_CY8C6		1		/* Target CPU : CY8C6 series */
#define CPU_CY8C624A		1		/* Target CPU : CY8C624A */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core : ARM Cortex-M4 */

#define TARGET_DIR		cy8ckit_cy8c6	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		cy8c6		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (CY8C624ABZI-S2D44A0)
 */
#include <sys/sysdepend/cpu/cy8c6/machine.h>


/*
 *	NOT YET SUPPORT DEVICE
 */
#undef USE_PTMR


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
