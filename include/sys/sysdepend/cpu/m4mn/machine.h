/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the SBK-M4MN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	machine_depend.h
 *
 *	Machine type definition (TMPM4MN depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_CPU_H__
#define __SYS_SYSDEPEND_MACHINE_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv7M(ARM Cortex-M4F))
 */

#include "../core/armv7m/machine.h"

/*
 **** CPU-depeneded profile (TMPM4MN)
 */

#define	TARGET_CPU_DIR		m4mn		/* Sysdepend-CPU-Directory name */


#endif /* __SYS_SYSDEPEND_MACHINE_CPU_H__ */
