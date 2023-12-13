/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the AdBun-M4NR.
 *    Modified by UC Technology at 2023/09/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7M

/*
 *	reset_hdl.c (ARMv7-M)
 *	Reset handler
 */

#include "kernel.h"
#include "../../../sysdepend.h"

#include <tm/tmonitor.h>

/* Low level memory manager information */
EXPORT	void	*knl_lowmem_top;		// Head of area (Low address)
EXPORT	void	*knl_lowmem_limit;		// End of area (High address)

#pragma location = ".intvec"
IMPORT const void (*vector_tbl[])();

#pragma section=".data_init"
#pragma section=".data"
#pragma section=".bss"
#pragma section=".noinit"

EXPORT void Reset_Handler(void)
{
	UW	*src, *top, *end;
	UW	reg;
	INT	i;

	/* Startup Hardware */
	knl_startup_hw();

	const void* __data_org = __section_begin(".data_init");

	const void* __data_start = __section_begin(".data");
	const void* __data_end = __section_end(".data");
	
	const void* __bss_start = __section_begin(".bss");
	const void* __bss_end = __section_end(".bss");

#if !USE_STATIC_IVT
	/* Load Vector Table from ROM to RAM */
	src = (UW*)vector_tbl;
	top = (UW*)exchdr_tbl;

	for(i=0; i < ((N_SYSVEC + N_INTVEC)); i++) {
		*top++ = *src++;
	}
	
	/* Set Vector Table offset to SRAM */
	*(_UW*)SCB_VTOR = (UW)exchdr_tbl;
#endif

	/* Load .data to ram */
	src = (UW*)__data_org;
	top = (UW*)__data_start;
	end = (UW*)__data_end;
	while(top != end) {
		*top++ = *src++;
	}

	/* Initialize .bss */
#if USE_NOINIT
	const void* __noinit_end = __section_end(".noinit");
	top = (UW*)__noinit_end;
#else 
	top = (UW*)__bss_start;
#endif
	for(i = ((INT)__bss_end - (INT)top)/sizeof(UW); i > 0 ; i--) {
		*top++ = 0;
	}

#if USE_IMALLOC
	/* Set System memory area */
	if(INTERNAL_RAM_START > SYSTEMAREA_TOP) {
		knl_lowmem_top = (UW*)INTERNAL_RAM_START;
	} else {
		knl_lowmem_top = (UW*)SYSTEMAREA_TOP;
	}
	if((UW)knl_lowmem_top < (UW)__bss_end) {
		knl_lowmem_top = (UW*)__bss_end;
	}

	if((SYSTEMAREA_END != 0) && (INTERNAL_RAM_END > CNF_SYSTEMAREA_END)) {
		knl_lowmem_limit = (UW*)(SYSTEMAREA_END - EXC_STACK_SIZE);
	} else {
		knl_lowmem_limit = (UW*)(INTERNAL_RAM_END - EXC_STACK_SIZE);
	}
#endif

	/* Configure exception priorities */
	reg = *(_UW*)SCB_AIRCR;
	reg = (reg & (~AIRCR_PRIGROUP7)) | AIRCR_PRIGROUP3;	// PRIGRP:SUBPRI = 4 : 4
	*(_UW*)SCB_AIRCR = (reg & 0x0000FFFF) | AIRCR_VECTKEY;
	
	*(_UW*)SCB_SHPR2 = SCB_SHPR2_VAL;		// SVC pri = 0
	*(_UW*)SCB_SHPR3 = SCB_SHPR3_VAL;		// SysTick = 1 , PendSV = INTPRI_PENDSV

#if USE_FPU
	/* Enable FPU */
	*(_UW*)FPU_CPACR |= FPU_CPACR_FPUENABLE;
	*(_UW*)FPU_FPCCR |= (FPU_FPCCR_ASPEN | FPU_FPCCR_LSPEN);
#endif /* USE_FPU */

	/* Startup Kernel */
	main();		/**** No return ****/
	while(1);	/* guard - infinite loops */
}

#endif	/* CPU_CORE_ARMV7M */
