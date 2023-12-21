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

#include <sys/machine.h>
#include <tk/tkernel.h>
#include "kernel.h"
#include "../../../../lib/libtk/sysdepend/cpu/core/armv7m/int_armv7m.h"
#ifdef EVK_XMC7200

Noinit(IMPORT FP knl_inthdr_tbl[N_INTVEC]);	/* HLL Interrupt Handler Table */

/*
 * The handler for CPU interrupt.
 * The system interrupt mapped to CPU interrupt will be fetched and executed
 */
void CM7_CpuIntr_Handler(UINT intno)
{
	UW	system_int_idx;
	FP	inthdr;

	ENTER_TASK_INDEPENDENT;

	/* Check module interrupts with CPUSS_CM7_X_INT_STATUS of XMC7200. */
	if((*CM7_0_INT_STATUS(intno) & SYSTEM_INT_VALID) == SYSTEM_INT_VALID)
	{
		system_int_idx = (*CM7_0_INT_STATUS(intno) & SYSTEM_INT_IDX_MASK);
		inthdr = knl_inthdr_tbl[system_int_idx];
		inthdr(); // jump to system interrupt handler
	}
	ClearInt_nvic(intno);

	LEAVE_TASK_INDEPENDENT;
}

/*
 * The handler for Software interrupt.
 * The system interrupt mapped to CPU interrupt will be fetched and executed
 */
void CM7_SoftIntr_Handler(UINT intno)
{
	UW	system_int_idx;
	FP	inthdr;

	ENTER_TASK_INDEPENDENT;

	/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
	system_int_idx = (N_INTVEC - (CORE_EXT_INTVEC + CORE_SOFT_INTVEC) + intno);

	inthdr = knl_inthdr_tbl[system_int_idx];
	inthdr(); // jump to system interrupt handler

	ClearInt_nvic(intno);

	LEAVE_TASK_INDEPENDENT;
}

/*
 * The Handler is called when the CPU interrupt0 occurs.
 */
void CM7_CpuIntr0_Handler(void)
{
	CM7_CpuIntr_Handler(0);
}

/*
 * The Handler is called when the CPU interrupt1 occurs.
 */
void CM7_CpuIntr1_Handler(void)
{
	CM7_CpuIntr_Handler(1);
}

/*
 * The Handler is called when the CPU interrupt2 occurs.
 */
void CM7_CpuIntr2_Handler(void)
{
	CM7_CpuIntr_Handler(2);
}

/*
 * The Handler is called when the CPU interrupt3 occurs.
 */
void CM7_CpuIntr3_Handler(void)
{
	CM7_CpuIntr_Handler(3);
}

/*
 * The Handler is called when the CPU interrupt4 occurs.
 */
void CM7_CpuIntr4_Handler(void)
{
	CM7_CpuIntr_Handler(4);
}

/*
 * The Handler is called when the CPU interrupt5 occurs.
 */
void CM7_CpuIntr5_Handler(void)
{
	CM7_CpuIntr_Handler(5);
}

/*
 * The Handler is called when the CPU interrupt6 occurs.
 */
void CM7_CpuIntr6_Handler(void)
{
	CM7_CpuIntr_Handler(6);
}

/*
 * The Handler is called when the CPU interrupt7 occurs.
 */
void CM7_CpuIntr7_Handler(void)
{
	CM7_CpuIntr_Handler(7);
}

/*
 * The Handler is called when the CPU interrupt8 occurs.
 */
void CM7_CpuIntr8_Handler(void)
{
	CM7_SoftIntr_Handler(8);
}

/*
 * The Handler is called when the CPU interrupt9 occurs.
 */
void CM7_CpuIntr9_Handler(void)
{
	CM7_SoftIntr_Handler(9);
}

/*
 * The Handler is called when the CPU interrupt10 occurs.
 */
void CM7_CpuIntr10_Handler(void)
{
	CM7_SoftIntr_Handler(10);
}

/*
 * The Handler is called when the CPU interrupt11 occurs.
 */
void CM7_CpuIntr11_Handler(void)
{
	CM7_SoftIntr_Handler(11);
}

/*
 * The Handler is called when the CPU interrupt12 occurs.
 */
void CM7_CpuIntr12_Handler(void)
{
	CM7_SoftIntr_Handler(12);
}

/*
 * The Handler is called when the CPU interrupt13 occurs.
 */
void CM7_CpuIntr13_Handler(void)
{
	CM7_SoftIntr_Handler(13);
}

/*
 * The Handler is called when the CPU interrupt14 occurs.
 */
void CM7_CpuIntr14_Handler(void)
{
	CM7_SoftIntr_Handler(14);
}

/*
 * The Handler is called when the CPU interrupt15 occurs.
 */
void CM7_CpuIntr15_Handler(void)
{
	CM7_SoftIntr_Handler(15);
}

#endif /* EVK_XMC7200 */
