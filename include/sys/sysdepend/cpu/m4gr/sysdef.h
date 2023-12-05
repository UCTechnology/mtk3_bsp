/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (TMPM4GRF20FG depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv7M(ARM Cortex-M4))
 */

#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)  0x20000400 - 0x2003FFFF(Excluding Backup RAM)
 */
#define INTERNAL_RAM_SIZE       (0x20040000-0x20000000)
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_END        (INTERNAL_RAM_START + INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END


/* ------------------------------------------------------------------------ */
/*
 * System Clock
 */
/*------ Clock control and operation mode ------*/
#define CG_BASE		(0x40083000ul)			/* CG-M3H(2)-D */
#define CGPROTECT	(_UW*)(CG_BASE+0x0000ul)	/* Write Protect */
#define CGOSCCR		(_UW*)(CG_BASE+0x0004ul)	/* Oscillation control */
#define CGSYSCR		(_UW*)(CG_BASE+0x0008ul)	/* System clock control */
#define CGSTBYCR	(_UW*)(CG_BASE+0x000Cul)	/* Standby control */
#define CGPLL0SEL	(_UW*)(CG_BASE+0x0020ul)	/* PLL select for fsys */
#define CGWUPHCR	(_UW*)(CG_BASE+0x0030ul)	/* High-speed oscillation warm-up */
#define CGWUPLCR	(_UW*)(CG_BASE+0x0034ul)	/* Low-speed oscillation warm-up */
#define CGFSYSMENC	(_UW*)(CG_BASE+0x0044ul)	/* fsysm supply stop C */
#define CGFSYSMENA	(_UW*)(CG_BASE+0x0048ul)	/* fsysm supply stop A */
#define CGFSYSMENB	(_UW*)(CG_BASE+0x004Cul)	/* fsysm supply stop B */
#define CGFSYSENA	(_UW*)(CG_BASE+0x0050ul)	/* fsysh supply stop A */
#define CGFCEN		(_UW*)(CG_BASE+0x0058ul)	/* fc supply stop */
#define CGSPCLKEN	(_UW*)(CG_BASE+0x005Cul)	/* ADC, trace clock supply stop */
#define CGEXTEND2	(_UW*)(CG_BASE+0x0068ul)	/* Function expansion register 2 */

/*------ VALUE in CGPROTECT Register ------*/


/*------ VALUE in CGOSCCR Register ------*/
#define CGOSCCR_IHOSC2F_STAT	0x00080000ul	/* Internal oscillation stability flag for IHOSC2 */
#define CGOSCCR_IHOSC1F_STAT	0x00010000ul	/* Internal oscillation stability flag for IHOSC1 */
#define CGOSCCR_OSCF_STAT	0x00000200ul	/* High-speed oscillator selection status for fosc */
#define CGOSCCR_OSCSEL_MASK	0x00000100ul	/* High-speed oscillator selection for fosc */
#define CGOSCCR_OSCSEL_IHOSC1	0x00000000ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGOSCCR_OSCSEL_EHOSC	0x00000100ul	/* External high-speed oscillator (EHOSC) */
#define CGOSCCR_IHOSC2EN_MASK	0x00000008ul	/* Built-in high-speed oscillator for OFD (IHOSC2) */
#define CGOSCCR_IHOSC2EN_STOP	0x00000000ul	/* Stop */
#define CGOSCCR_IHOSC2EN_OSC	0x00000008ul	/* Oscillation */
#define CGOSCCR_EOSCEN_MASK	0x00000006ul	/* Operation selection of ext. high-speed oscillator (EHOSC) */
#define CGOSCCR_EOSCEN_NONE	0x00000000ul	/* External oscillation not used */
#define CGOSCCR_EOSCEN_EHOSC	0x00000002ul	/* External high-speed oscillation (EHOSC) */
#define CGOSCCR_EOSCEN_EHCLKIN	0x00000004ul	/* External clock input (EHCLKIN) */
#define CGOSCCR_IHOSC1EN_MASK	0x00000001ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGOSCCR_IHOSC1EN_STOP	0x00000000ul	/* Stop */
#define CGOSCCR_IHOSC1EN_OSC	0x00000001ul	/* Oscillation */

/*------ VALUE in CGSYSCR Register ------*/
#define CGSYSCR_PRCKST_STAT	0x0F000000ul	/* R:Prescaler clock (Ft0) selection */
#define CGSYSCR_MCKSELGST_STAT	0x00C00000ul	/* R:Gear selection of system clock (fsysh) */
#define CGSYSCR_GEARST_STAT	0x00070000ul	/* R:Gear selection of system clock (fsysh) */
#define CGSYSCR_PRCK_MASK	0x00000F00ul	/* Prescaler clock (Ft0) selection */
#define CGSYSCR_PRCK_D1		0x00000000ul	/* fc/1 = fc	*/
#define CGSYSCR_PRCK_D2		0x00000100ul	/* fc/2		*/
#define CGSYSCR_PRCK_D4		0x00000200ul	/* fc/4		*/
#define CGSYSCR_PRCK_D8		0x00000300ul	/* fc/8		*/
#define CGSYSCR_PRCK_D16	0x00000400ul	/* fc/16	*/
#define CGSYSCR_PRCK_D32	0x00000500ul	/* fc/32	*/
#define CGSYSCR_PRCK_D64	0x00000600ul	/* fc/64	*/
#define CGSYSCR_PRCK_D128	0x00000700ul	/* fc/128	*/
#define CGSYSCR_PRCK_D256	0x00000800ul	/* fc/256	*/
#define CGSYSCR_PRCK_D512	0x00000900ul	/* fc/512	*/
#define CGSYSCR_MCKSEL_D1	0x00000000ul	/* fc/1 = fc	*/
#define CGSYSCR_MCKSEL_D2	0x00000040ul	/* fc/2	*/
#define CGSYSCR_MCKSEL_D4	0x00000080ul	/* fc/4	*/
#define CGSYSCR_GEAR_MASK	0x00000007ul	/* Gear selection of system clock (fsys) */
#define CGSYSCR_GEAR_D1		0x00000000ul	/* fc/1 = fc	*/
#define CGSYSCR_GEAR_D2		0x00000001ul	/* fc/2		*/
#define CGSYSCR_GEAR_D4		0x00000002ul	/* fc/4		*/
#define CGSYSCR_GEAR_D8		0x00000003ul	/* fc/8		*/
#define CGSYSCR_GEAR_D16	0x00000004ul	/* fc/16	*/

/*------ VALUE in CGSYSCR Register ------*/
/*------ VALUE in CGSTBYCR Register ------*/
#define CGSTBYCR_STBY_MASK	0x00000003ul	/* Low power consumption mode selection */
#define CGSTBYCR_STBY_IDLE	0x00000000ul	/* IDLE */
#define CGSTBYCR_STBY_STOP1	0x00000001ul	/* STOP1 */
#define CGSTBYCR_STBY_STOP2	0x00000002ul	/* STOP2 */

/*------ VALUE in CGSCOCR Register ------*/
/*------ VALUE in CGPLL0SEL Register ------*/
#define CGPLL0SEL_PLL0SET_MASK	0xFFFFFFF0ul	/* PLL multiplication setting */
#define CGPLL0SEL_PLL0ST_STAT	0x00000004ul	/* Clock selection status for fsys */
#define CGPLL0SEL_PLL0ST_FOSC	0x00000000ul	/* fosc */
#define CGPLL0SEL_PLL0ST_FPLL	0x00000004ul	/* fPLL */
#define CGPLL0SEL_PLL0SEL_MASK	0x00000002ul	/* Clock selection for fsys */
#define CGPLL0SEL_PLL0SEL_FOSC	0x00000000ul	/* fosc */
#define CGPLL0SEL_PLL0SEL_FPLL	0x00000002ul	/* fPLL */
#define CGPLL0SEL_PLL0ON_MASK	0x00000001ul	/* PLL operation for fsys */
#define CGPLL0SEL_PLL0ON_STOP	0x00000000ul	/* Stop */
#define CGPLL0SEL_PLL0ON_OSC	0x00000001ul	/* Oscillation */

/*------ VALUE in CGWUPHCR Register ------*/
#define CGWUPHCR_WUPT_MASK	0xFFFF0000ul	/* Calculated value of warm-up timer */
#define CGWUPHCR_WUCLK_MASK	0x00000100ul	/* Warming-up clock selection */
#define CGWUPHCR_WUCLK_HIOSC1	0x00000000ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGWUPHCR_WUCLK_EHOSC	0x00000100ul	/* External high-speed oscillator (EHOSC) */
#define CGWUPHCR_WUEF_STAT	0x00000002ul	/* Warming-up timer status */
#define CGWUPHCR_WUON_MASK	0x00000001ul	/* Warming-up timer control */
#define CGWUPHCR_WUON_START	0x00000001ul	/* Warming up start */

/*------ VALUE in CGWUPLCR Register ------*/
#define CGWUPLCR_WUPTL_MASK	0x07FFFF00ul	/* Calculated value of warm-up timer */
#define CGWUPLCR_WULEF_STAT	0x00000002ul	/* Warm-up timer status */
#define CGWUPLCR_WULEF_END	0x00000000ul	/* End warming up */
#define CGWUPLCR_WULEF_WUP	0x00000002ul	/* During warm-up */
#define CGWUPLCR_WULON_MASK	0x00000001ul	/* Warming-up timer control */
#define CGWUPLCR_WULON_START	0x00000001ul	/* Warming up start */


/*------ VALUE in CGFSYSENA Register ------*/
/*------ VALUE in CGFSYSENB Register ------*/
#define CGFSYSMENA_UART5	0x01000000ul	/* UART ch5 enable */
#define CGFSYSMENA_UART4	0x00800000ul	/* UART ch4 enable */
#define CGFSYSMENA_UART3	0x00400000ul	/* UART ch3 enable */
#define CGFSYSMENA_UART2	0x00200000ul	/* UART ch2 enable */
#define CGFSYSMENA_UART1	0x00100000ul	/* UART ch1 enable */
#define CGFSYSMENA_UART0	0x00800000ul	/* UART ch0 enable */


/*------ VALUE in CGFCEN Register ------*/
#define CGFCEN_FCIPEN27_SUPPLY	0x08000000ul	/* Clock enable of DNF UnitB */
#define CGFCEN_FCIPEN26_SUPPLY	0x04000000ul	/* Clock enable of DNF UnitA */
#define CGFCEN_FCIPEN23_SUPPLY	0x00800000ul	/* Clock enable of OFD */

/*------ VALUE in CGSPCLKEN Register ------*/
#define CGSPCLKEN_ADCKEN_SUPPLY	0x00010000ul	/* Conversion clock enable for AD converter */
#define CGSPCLKEN_TRCKEN_SUPPLY	0x00000001ul	/* Clock enable for trace function of debug circuit (ETM)*/


/* Low-speed oscillation/power supply control */
#define RLM_BASE	(0x4003E400ul)			/* CG-M3H(2)-D */
#define RLMLOSCCR	(_UB*)(RLM_BASE+0x0000ul)	/* Low-speed oscillation control */
#define RLMSHTDNOP	(_UB*)(RLM_BASE+0x0001ul)	/* Power-off control */
#define RLMPROTECT	(_UB*)(RLM_BASE+0x000Ful)	/* RLM write protect */
	// !!!ATTENTION!!! Access these registers in BYTE units.

/*------ VALUE in RLMLOSCCR Register ------*/
#define RLMLOSCCR_DRCOSCL_ELCLKIN	0x00ul		/* ELCLKIN */
#define RLMLOSCCR_DRCOSCL_ELOSC		0x04ul		/* ELOSC */
#define RLMLOSCCR_XTEN_STOP		0x00ul		/* Stop */
#define RLMLOSCCR_XTEN_OSC		0x01ul		/* Oscillation */


/*------ VALUE in RLMSHTDNOP Register ------*/
/*------ VALUE in RLMPROTECT Register ------*/


/*
 * Watch Dog Timer
 * M4GQ is TYPE 2, it has only ch0.
 */
#define SIWDT_BASE	(0x400A0600ul)			/* SIWDT-A_ja */
#define SIWDxPRO(x)	(_UW*)(SIWDT_BASE+0x0000ul)	/* Protect */
#define SIWDxEN(x)	(_UW*)(SIWDT_BASE+0x0004ul)	/* Enable */
#define SIWDxCR(x)	(_UW*)(SIWDT_BASE+0x0008ul)	/* Control */
#define SIWDxMOD(x)	(_UW*)(SIWDT_BASE+0x000Cul)	/* Mode */
#define SIWDxMONI(x)	(_UW*)(SIWDT_BASE+0x0010ul)	/* Count monitor */
#define SIWDxOSCCR(x)	(_UW*)(SIWDT_BASE+0x0014ul)	/* Oscillation clock protect control */

/*------ VALUE in SIWDxPRO Register ------*/
#define SIWDxPRO_NONE		0x0000001Eul	/* No protection */
#define SIWDxPRO_A		0x000000A9ul	/* Protect A mode setting */
#define SIWDxPRO_B		0x00000074ul	/* Protect B mode setting */

/*------ VALUE in SIWDxEN Register ------*/
#define SIWDxEN_WDTF_STAT	0x00000002ul	/* R:Operation Status */
#define SIWDxEN_WDTE_MASK	0x00000001ul	/* Permit/prohibit control */
#define SIWDxEN_WDTE_DISABLE	0x00000000ul
#define SIWDxEN_WDTE_ENABLE	0x00000001ul

/*------ VALUE in SIWDxCR Register ------*/
#define SIWDxEN_WDCR_DISABLE	0x000000B1ul	/* Disable code */
#define SIWDxEN_WDCR_CLEAR	0x0000004Eul	/* Clear code */

//#define TMCLK			199.89		/* Timer clock input (MHz) */
#define TMCLK_KHz		(199890)	/* Timer clock input (kHz) */

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		175	/* Number of Interrupt vectors */
#define	N_SYSVEC		16	/* Number of System Exceptions */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		4


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		15	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define GPIO_BASE	(0x400E0000ul)			/* PORT-M4G */
#define PA_BASE		(GPIO_BASE+0x0000ul)		/* A-H without I */
#define PJ_BASE		(GPIO_BASE+0x0800ul)		/* J-N without O */
#define PP_BASE		(GPIO_BASE+0x0D00ul)		/* P   without Q */
#define PR_BASE		(GPIO_BASE+0x0E00ul)		/* R   without S */
#define PT_BASE		(GPIO_BASE+0x0F00ul)		/* T-W without X */
#define PY_BASE		(GPIO_BASE+0x1300ul)		/* Y */

#define PxBLOCK_B(x)	(((x)<'I')? PA_BASE: \
						 (((x)<'O')? PJ_BASE: \
						 (((x)<'Q')? PP_BASE: \
						 (((x)<'S')? PR_BASE: \
						 (((x)<'X')? PT_BASE:PY_BASE)))))
#define PxBLOCK_C(x)	(((x)<'I')? 'A'    : \
						(((x)<'O')? 'J'    : \
						(((x)<'Q')? 'P'    : \
						(((x)<'S')? 'R'    : \
						(((x)<'X')? 'T'    :'Y')))))
#define PxBASE(x)	(PxBLOCK_B(x)+0x0100ul*((x)-PxBLOCK_C(x))) /* Px BASE (x = 'A' - 'Y') */

#define PxDATA(x)	(_UW*)(PxBASE(x)+0x0000ul)		/* Data register */
#define PxCR(x)		(_UW*)(PxBASE(x)+0x0004ul)		/* Output control register */
#define PxFRn(x,n)	(_UW*)(PxBASE(x)+0x0008ul+4*((n)-1))	/* Function register n (n = 1 - 6) */
#define PxOD(x)		(_UW*)(PxBASE(x)+0x0028ul)		/* Open drain control register */
#define PxPUP(x)	(_UW*)(PxBASE(x)+0x002Cul)		/* Pull-up control register */
#define PxPDN(x)	(_UW*)(PxBASE(x)+0x0030ul)		/* pull-down control register */
#define PxIE(x)		(_UW*)(PxBASE(x)+0x0038ul)		/* Input control register */


/* Function Number of PortX */
/* Macros defined as NA are prohibited. */
/* Writing to the bit of NA has no meaning. */

#define NA		0	/* This means that it is not FR1-FR6. */

#define PE2_INPUT	0	/* 0: Input Port	*/
#define PE2_OUTPUT	0	/* 0: Output Port	*/
#define PE2_UT_RX	7	/* 7: UT0RXD 		*/

#define PE3_INPUT	0	/* 0: Input Port	*/
#define PE3_OUTPUT	0	/* 0: Output Port	*/
#define PE3_UT_TX	7	/* 7: UT0TXDA		*/

#define PORT_IN		0x00000001ul
#define PORT_OUT	0x00000002ul
#define PORT_INOUT	(PORT_IN|PORT_OUT)
#define PORT_NOINOUT	0x00000000ul

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for TMPM4GRF20FG T32A)
 */
#define	CPU_HAS_PTMR	(1)

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU	1
#define CPU_HAS_DPS	0

/*
 *  Number of coprocessors to use. 
 *    There is no coprocessor for this microcomputer.
 */
#define NUM_COPROCESSOR		1

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */

