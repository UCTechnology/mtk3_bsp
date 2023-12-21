/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_XMC7200

/*
 *	vector_tbl.c (XMC7200)
 *	Exception/Interrupt Vector Table
 */

#include "kernel.h"
#include "../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Exception/Interrupt Vector Table
 * 
 */

void (* const vector_tbl[])()  __attribute__((section(".vector"))) = {
	(void(*)()) (INITIAL_SP),		/* 0: Top of Stack */
	Reset_Handler,				/* 1: Reset Handler */
	NMI_Handler,				/* 2: NMI Handler */
	HardFault_Handler,			/* 3: Hard Fault Handler */
	MemManage_Handler,			/* 4: MPU Fault Handler */
	BusFault_Handler,			/* 5: Bus Fault Handler */
	UsageFault_Handler,			/* 6: Usage Fault Handler */
	0,					/* 7: Reserved */
	0,					/* 8: Reserved */
	0,					/* 9: Reserved */
	0,					/* 10: Reserved */
	Svcall_Handler,				/* 11: Svcall */
	0,					/* 12: Reserved */
	0,					/* 13: Reserved */
	knl_dispatch_entry,			/* 14: Pend SV */
	knl_systim_inthdr,			/* 15: Systick */

	/* External Interrupts */
	CM7_CpuIntr0_Handler,			/* IRQ 0 (CPU User Interrupt #0) */
	CM7_CpuIntr1_Handler,			/* IRQ 1 (CPU User Interrupt #1) */
	CM7_CpuIntr2_Handler,			/* IRQ 2 (CPU User Interrupt #2) */
	CM7_CpuIntr3_Handler,			/* IRQ 3 (CPU User Interrupt #3) */
	CM7_CpuIntr4_Handler,			/* IRQ 4 (CPU User Interrupt #4) */
	CM7_CpuIntr5_Handler,			/* IRQ 5 (CPU User Interrupt #5) */
	CM7_CpuIntr6_Handler,			/* IRQ 6 (CPU User Interrupt #6) */
	CM7_CpuIntr7_Handler,			/* IRQ 7 (CPU User Interrupt #7) */
	CM7_CpuIntr8_Handler,			/* IRQ 8 (Internal Software Interrupt #0) */
	CM7_CpuIntr9_Handler,			/* IRQ 9 (Internal Software Interrupt #1) */
	CM7_CpuIntr10_Handler,			/* IRQ 10 (Internal Software Interrupt #2) */
	CM7_CpuIntr11_Handler,			/* IRQ 11 (Internal Software Interrupt #3) */
	CM7_CpuIntr12_Handler,			/* IRQ 12 (Internal Software Interrupt #4) */
	CM7_CpuIntr13_Handler,			/* IRQ 13 (Internal Software Interrupt #5) */
	CM7_CpuIntr14_Handler,			/* IRQ 14 (Internal Software Interrupt #6) */
	CM7_CpuIntr15_Handler,			/* IRQ 15 (Internal Software Interrupt #7) */
};

#endif /* CPU_XMC7200 */
