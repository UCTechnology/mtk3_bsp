/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 *    Modified by UC Technology at 2023/02/01
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
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

IMPORT UW __ramVectors[];
#define exchdr_tbl	__ramVectors

typedef UW	cy_rslt_t;
#define CY_RSLT_SUCCESS		((cy_rslt_t)0x00000000U)
cy_rslt_t cybsp_init(void);

/* Low level memory manager information */
EXPORT	void	*knl_lowmem_top;		// Head of area (Low address)
EXPORT	void	*knl_lowmem_limit;		// End of area (High address)

IMPORT	const void *__data_org;
IMPORT	const void *__data_start;
IMPORT	const void *__data_end;
IMPORT	const void *__bss_start;
IMPORT	const void *__bss_end;
#if USE_NOINIT
IMPORT	const void *__noinit_end;
#endif

IMPORT const void (*vector_tbl[])();

// system_psoc6_cm4.c by ModusToolbox
IMPORT	void	SystemInit(void);
IMPORT	void	Cy_SystemInitFpuEnable(void);

/* Allocate the .utk_mgt section to the SYSTEM MEMORY AREA.	*/
extern const void*	__utk_mgt_start__;	// TOP of .utk_mgt section
extern const void*	__utk_mgt_end__;	// END of .utk_mgt section
						//   --> cy8c6xxa_cm4_dual.ld
EXPORT void Reset_Handler(void)
{
	UW	*src, *top, *end;
	UW	reg;
	INT	i;

	/* Startup Hardware */
	knl_startup_hw();

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
	src = (UW*)&__data_org;
	top = (UW*)&__data_start;
	end = (UW*)&__data_end;
	while(top != end) {
		*top++ = *src++;
	}

	/* Initialize .bss */
#if USE_NOINIT
	top = (UW*)&__noinit_end;
#else 
	top = (UW*)&__bss_start;
#endif
	for(i = ((INT)&__bss_end - (INT)&__bss_start)/sizeof(UW); i > 0 ; i--) {
		*top++ = 0;
	}

	SystemInit();			// --> system_psoc6_cm4.c by ModusToolbox

#if USE_IMALLOC
	/* set SYSTEM MEMORY AREA */
	knl_lowmem_top = (UW*)&__utk_mgt_start__;
	knl_lowmem_limit = (UW*)&__utk_mgt_end__;
#endif

	/* Configure exception priorities */
	reg = *(_UW*)SCB_AIRCR;
	reg = (reg & (~AIRCR_PRIGROUP7)) | AIRCR_PRIGROUP3;	// PRIGRP:SUBPRI = 4 : 4
	*(_UW*)SCB_AIRCR = (reg & 0x0000FFFF) | AIRCR_VECTKEY;
	
	*(_UW*)SCB_SHPR2 = SCB_SHPR2_VAL;		// SVC pri = 0
	*(_UW*)SCB_SHPR3 = SCB_SHPR3_VAL;		// SysTick = 1 , PendSV = 7

#if USE_FPU
	/* Enable FPU */
	Cy_SystemInitFpuEnable();	// --> system_psoc6_cm4.c by ModusToolbox
#endif /* USE_FPU */

	/* Initialize the device and board peripherals */
	cy_rslt_t result = cybsp_init();
	if(result != CY_RSLT_SUCCESS){
		while(1);
	}

	/* Startup Kernel */
	main();		/**** No return ****/
	while(1);	/* guard - infinite loops */
}

#endif	/* CPU_CORE_ARMV7M */
