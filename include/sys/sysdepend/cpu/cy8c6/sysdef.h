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
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (CY8C6 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

#include "../../../machine.h"

/* CPU Core-dependent definition */
#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Disable the definition in uT-Kernel 3.0 and use the functions of ModusToolbox.
 */
#undef SCB_SCR


/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* CY8C624ABZI Internal SRAM	0x08000000 - 0x080FFFFF	1,024KB	*/
/*		for Cortex-M0+	0x08000000 - 0x08002000	    8KB	*/
/*		for Cortex-M4	0x08002000 - 0x080FF800	1,014KB	*/
/*		for System	0x080FF800 - 0x08100000	    2KB	*/
#if CPU_CY8C624A
#define INTERNAL_RAM_SIZE       0x000FD800
#define INTERNAL_RAM_START      0x08002000
#endif

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END


/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 */

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		168	/* Number of Interrupt vectors */
#define	N_SYSVEC		16	/* Number of System Exceptions */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		3


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
//				2	/* TCPWM (Timer,Counter,PWM) */
//				3	/* Sample Drivers (Ser, ADC, I2C) */
#define INTPRI_PENDSV		7	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for CY8C6)
 */
#define	CPU_HAS_PTMR		(32)

/* Physical timer interrupt number */
#define INTNO_TCPWM000		123	/* TCPWM #0, Counter #0		*/
#define INTNO_TCPWM007		130	/* TCPWM #0, Counter #7		*/
#define INTNO_TCPWM100		131	/* TCPWM #1, Counter #0		*/
#define INTNO_TCPWM123		154	/* TCPWM #1, Counter #23	*/

/* Physical timer interrupt priority */
#define INTPRI_TCPWM000		2	/* TCPWM #0, Counter #0		*/
#define INTPRI_TCPWM007		2	/* TCPWM #0, Counter #7		*/
#define INTPRI_TCPWM100		2	/* TCPWM #1, Counter #0		*/
#define INTPRI_TCPWM123		2	/* TCPWM #1, Counter #23	*/

/* Phycail timer Maximum count */
#define TCPWM_MAX_CNT16		(0x0000FFFF)
#define TCPWM_MAX_CNT32		(0xFFFFFFFF)

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		1
#define CPU_HAS_DPS		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
