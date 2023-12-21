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
 *	machine.h
 *
 *	Machine type definition (XMC7200D-E272K8384 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- KIT_XMC72_EVK XMC7200 (CPU: XMC7200D-E272K8384) definition ----- */

#define EVK_XMC7200		1		/* Target system : XMC7200D-E272K8384 */
#define CPU_XMC7200		1		/* Target CPU : XMC7200 series */
#define CPU_XMC7200D		1		/* Target CPU : XMC7200D */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM7		1		/* Target CPU-Core : ARM Cortex-M7 */

#define TARGET_DIR		evk_xmc7200	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		xmc7200		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (XMC7200D-E272K8384)
 */
#include <sys/sysdepend/cpu/xmc7200/machine.h>


/*
 *	NOT YET SUPPORT DEVICE
 */
#undef USE_PTMR


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
