/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06.B0
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
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
 *	sysdepend.h (XMC7200)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_SYSDEPEND_
#define _SYSDEPEND_CPU_SYSDEPEND_

#include "../core/armv7m/sysdepend.h"

/*
 *	Convert Symbol of Section in link script.
 */
#define __data_org	__etext
#define __data_start	__data_start__
#define __data_end	__data_end__
#define __bss_start	__bss_start__
#define __bss_end	__bss_end__

/*
 *    External Interrupt Handler (ext_hdr.c)
 */
IMPORT void CM7_CpuIntr0_Handler(void);
IMPORT void CM7_CpuIntr1_Handler(void);
IMPORT void CM7_CpuIntr2_Handler(void);
IMPORT void CM7_CpuIntr3_Handler(void);
IMPORT void CM7_CpuIntr4_Handler(void);
IMPORT void CM7_CpuIntr5_Handler(void);
IMPORT void CM7_CpuIntr6_Handler(void);
IMPORT void CM7_CpuIntr7_Handler(void);
IMPORT void CM7_CpuIntr8_Handler(void);
IMPORT void CM7_CpuIntr9_Handler(void);
IMPORT void CM7_CpuIntr10_Handler(void);
IMPORT void CM7_CpuIntr11_Handler(void);
IMPORT void CM7_CpuIntr12_Handler(void);
IMPORT void CM7_CpuIntr13_Handler(void);
IMPORT void CM7_CpuIntr14_Handler(void);
IMPORT void CM7_CpuIntr15_Handler(void);
IMPORT void CM7_CpuIntr_Handler(UINT intno);
IMPORT void CM7_SoftIntr_Handler(UINT intno);

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(void);
IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_CPU_SYSDEPEND_ */
