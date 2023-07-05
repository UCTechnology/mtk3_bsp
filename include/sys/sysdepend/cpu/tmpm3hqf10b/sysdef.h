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
 *	System dependencies definition (TMPM3HQF10 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv7M(ARM Cortex-M3))
 */

#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)  0x20000400 - 0x2000FFFF
 */
#define INTERNAL_RAM_SIZE       (0x20010000-0x20000000)
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
#define CG_BASE		(0x400F3000ul)				/* CG-M3H(2)-D */
#define CGPROTECT	(_UW*)(CG_BASE+0x0000ul)	/* Write Protect */
#define CGOSCCR		(_UW*)(CG_BASE+0x0004ul)	/* Oscillation control */
#define CGSYSCR		(_UW*)(CG_BASE+0x0008ul)	/* System clock control */
#define CGSTBYCR	(_UW*)(CG_BASE+0x000Cul)	/* Standby control */
#define CGSCOCR		(_UW*)(CG_BASE+0x0010ul)	/* SCOUT output control */
#define CGPLL0SEL	(_UW*)(CG_BASE+0x0020ul)	/* PLL select for fsys */
#define CGWUPHCR	(_UW*)(CG_BASE+0x0030ul)	/* High-speed oscillation warm-up */
#define CGWUPLCR	(_UW*)(CG_BASE+0x0034ul)	/* Low-speed oscillation warm-up */
#define CGFSYSENA	(_UW*)(CG_BASE+0x0050ul)	/* fsys supply stop A */
#define CGFSYSENB	(_UW*)(CG_BASE+0x0054ul)	/* fsys supply stop B */
#define CGFCEN		(_UW*)(CG_BASE+0x0058ul)	/* fc supply stop */
#define CGSPCLKEN	(_UW*)(CG_BASE+0x005Cul)	/* ADC, trace clock supply stop */

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
#define CGSYSCR_GEARST_STAT	0x00070000ul	/* R:Gear selection of system clock (fsys) */
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
#define CGFSYSENB_UART5		0x00000400ul	/* UART ch5 enable */
#define CGFSYSENB_UART4		0x00000200ul	/* UART ch4 enable */
#define CGFSYSENB_UART3		0x00000100ul	/* UART ch3 enable */
#define CGFSYSENB_UART2		0x00000080ul	/* UART ch2 enable */
#define CGFSYSENB_UART1		0x00000040ul	/* UART ch1 enable */
#define CGFSYSENB_UART0		0x00000020ul	/* UART ch0 enable */


/*------ VALUE in CGFCEN Register ------*/
#define CGFCEN_FCIPEN07_MASK	0x00000008ul	/* Clock enable of DNF UnitA, UnitB */
#define CGFCEN_FCIPEN07_STOP	0x00000000ul	/* Clock stop */
#define CGFCEN_FCIPEN07_SUPPLY	0x00000008ul	/* Clock supply */

/*------ VALUE in CGSPCLKEN Register ------*/
#define CGSPCLKEN_ADCKEN_MASK	0x00010000ul	/* Conversion clock enable for AD converter */
#define CGSPCLKEN_ADCKEN_STOP	0x00000000ul	/* Clock stop */
#define CGSPCLKEN_ADCKEN_SUPPLY	0x00010000ul	/* Clock supply */
#define CGSPCLKEN_TRCKEN_MASK	0x00000001ul	/* Clock enable for trace function of debug circuit (ETM)*/
#define CGSPCLKEN_TRCKEN_STOP	0x00000000ul	/* Clock stop */
#define CGSPCLKEN_TRCKEN_SUPPLY	0x00000001ul	/* Clock supply */


/* Low-speed oscillation/power supply control */
#define RLM_BASE	(0x4003E400ul)			/* CG-M3H(2)-D */
#define RLMLOSCCR	(_UB*)(RLM_BASE+0x0000ul)	/* Low-speed oscillation control */
#define RLMSHTDNOP	(_UB*)(RLM_BASE+0x0001ul)	/* Power-off control */
#define RLMPROTECT	(_UB*)(RLM_BASE+0x000Ful)	/* RLM write protect */
	// !!!ATTENTION!!! Access these registers in BYTE units.

/*------ VALUE in RLMLOSCCR Register ------*/
#define RLMLOSCCR_XTEN_MASK	0x01ul		/* Operation selection of external low-speed oscillator */
#define RLMLOSCCR_XTEN_STOP	0x00ul		/* Stop */
#define RLMLOSCCR_XTEN_OSC	0x01ul		/* Oscillation */


/*------ VALUE in RLMSHTDNOP Register ------*/
/*------ VALUE in RLMPROTECT Register ------*/


/*
 * Watch Dog Timer
 * M3HQ is TYPE 1, it has only ch0.
 */
#define SIWDT_BASE	(0x400BB400ul)			/* SIWDT-A_ja */
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

//#define TMCLK			79.88		/* Timer clock input (MHz) */
#define TMCLK_KHz		(79880)		/* Timer clock input (kHz) */

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		166	/* Number of Interrupt vectors */
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
#define GPIO_BASE	(0x400C0000ul)			/* PORT-M3H(2) */
#define PA_BASE		(GPIO_BASE+0x0000ul)		/* A-H without I */
#define PJ_BASE		(GPIO_BASE+0x0800ul)		/* J-N without O */
#define PP_BASE		(GPIO_BASE+0x0D00ul)		/* P   without Q */
#define PR_BASE		(GPIO_BASE+0x0E00ul)		/* R   without S */
#define PT_BASE		(GPIO_BASE+0x0F00ul)		/* T-V */

#define PxBLOCK_B(x)	(((x)<'I')? PA_BASE:(((x)<'O')? PJ_BASE:(((x)<'Q')? PP_BASE:(((x)<'S')? PR_BASE:PT_BASE))))
#define PxBLOCK_C(x)	(((x)<'I')? 'A'    :(((x)<'O')? 'J'    :(((x)<'Q')? 'P'    :(((x)<'S')? 'R'    :'T'))))
#define PxBASE(x)	(PxBLOCK_B(x)+0x0100ul*((x)-PxBLOCK_C(x))) /* Px BASE (x = 'A' - 'V') */

#define PxDATA(x)	(_UW*)(PxBASE(x)+0x0000ul)	/* Data register */
#define PxCR(x)		(_UW*)(PxBASE(x)+0x0004ul)	/* Output control register */
#define PxFRn(x,n)	(_UW*)(PxBASE(x)+0x0008ul+4*((n)-1))	/* Function register n (n = 1 - 6) */
#define PxOD(x)		(_UW*)(PxBASE(x)+0x0028ul)	/* Open drain control register */
#define PxPUP(x)	(_UW*)(PxBASE(x)+0x002Cul)	/* Pull-up control register */
#define PxPDN(x)	(_UW*)(PxBASE(x)+0x0030ul)	/* pull-down control register */
#define PxIE(x)		(_UW*)(PxBASE(x)+0x0038ul)	/* Input control register */


/* Function Number of PortX */
/* Macros defined as NA are prohibited. */
/* Writing to the bit of NA has no meaning. */

#define NA			0	/* This means that it is not FR1-FR6. */

/* Port A */
#define PA0_INPUT		0	/*     0: Input Port	*/
#define PA0_OUTPUT		0	/*     0: Output Port	*/
#define PA0_UT0TXDB		1	/* PAFR1: UT0TXDB	*/
#define PA0_TSPI0SCK		3	/* PAFR3: TSPI0SCK	*/
#define PA0_T32A00OUTA		4	/* PAFR4: T32A00OUTA	*/
#define PA0_T32A00OUTC		5	/* PAFR5: T32A00OUTC	*/
#define PA0_ENC0A		6	/* PAFR6: ENC0A		*/

#define PA1_INPUT		0	/*     0: Input Port	*/
#define PA1_OUTPUT		0	/*     0: Output Port	*/
#define PA1_UT0TXDA		1	/* PAFR1: UT0TXDA	*/
#define PA1_UT0RXD		2	/* PAFR2: UT0RXD	*/
#define PA1_TSPI0TXD		3	/* PAFR3: TSPI0TXD	*/
#define PA1_T32A00INA0		4	/* PAFR4: T32A00INA0	*/
#define PA1_T32A00INC0		5	/* PAFR5: T32A00INC0	*/
#define PA1_ENC0B		6	/* PAFR6: ENC0B		*/

#define PA2_INPUT		0	/*     0: Input Port	*/
#define PA2_OUTPUT		0	/*     0: Output Port	*/
#define PA2_UT0RXD		1	/* PAFR1: UT0RXD	*/
#define PA2_UT0TXDA		2	/* PAFR2: UT0TXDA	*/
#define PA2_TSPI0RXD		3	/* PAFR3: TSPI0RXD	*/
#define PA2_T32A00INA1		4	/* PAFR4: T32A00INA1	*/
#define PA2_T32A00INC1		5	/* PAFR5: T32A00INC1	*/
#define PA2_ENC0Z		6	/* PAFR6: ENC0Z		*/

#define PA3_INPUT		0	/*     0: Input Port	*/
#define PA3_OUTPUT		0	/*     0: Output Port	*/
#define PA3_TSPI0CSIN		2	/* PAFR2: TSPI0CSIN	*/
#define PA3_TSPI0CS0		3	/* PAFR3: TSPI0CS0	*/
#define PA3_T32A00OUTB		4	/* PAFR4: T32A00OUTB	*/
#define PA3_TRGIN1		6	/* PAFR6: TRGIN1	*/

#define PA4_INPUT		0	/*     0: Input Port	*/
#define PA4_OUTPUT		0	/*     0: Output Port	*/
#define PA4_I2C1SCL		1	/* PAFR1: I2C1SCL	*/
#define PA4_TSPI0CS1		3	/* PAFR3: TSPI0CS1	*/
#define PA4_T32A00INB0		4	/* PAFR4: T32A00INB0	*/

#define PA5_INPUT		0	/*     0: Input Port	*/
#define PA5_OUTPUT		0	/*     0: Output Port	*/
#define PA5_I2C1SDA		1	/* PAFR1: I2C1SDA	*/
#define PA5_T32A00INB1		4	/* PAFR4: T32A00INB1	*/

#define PA6_INPUT		0	/*     0: Input Port	*/
#define PA6_OUTPUT		0	/*     0: Output Port	*/
#define PA6_INT07		0	/*     0: INT07		*/
#define PA6_UT3RXD		1	/* PAFR1: UT3RXD	*/
#define PA6_UT3TXDA		2	/* PAFR2: UT3TXDA	*/

#define PA7_INPUT		0	/*     0: Input Port	*/
#define PA7_OUTPUT		0	/*     0: Output Port	*/
#define PA7_INT11		0	/*     0: INT11		*/
#define PA7_UT3TXDA		1	/* PAFR1: UT3TXDA	*/
#define PA7_UT3RXD		2	/* PAFR2: UT3RXD	*/

/* Port B */
#define PB0_OUTPUT		0	/*     0: Output Port	*/
#define PB0_T32A01OUTA		4	/* PBFR4: T32A01OUTA	*/
#define PB0_T32A01OUTC		5	/* PBFR5: T32A01OUTC	*/
#define PB0_SCOUT		6	/* PBFR6: SCOUT		*/

#define PB1_INPUT		0	/*     0: Input Port	*/
#define PB1_OUTPUT		0	/*     0: Output Port	*/
#define PB1_INT03		0	/*     0: INT03		*/
#define PB1_RXIN0		1	/* PBFR1: RXIN0		*/
#define PB1_T32A01INA0		4	/* PBFR4: T32A01INA0	*/
#define PB1_T32A01INC0		5	/* PBFR5: T32A01INC0	*/
#define PB1_TRGIN0		6	/* PBFR6: TRGIN0	*/

#define PB2_INPUT		0	/*	0: Input Port	*/
#define PB2_OUTPUT		0	/*	0: Output Port	*/
#define PB2_UT2TXDA		1	/*  PBFR1: UT2TXDA	*/
#define PB2_UT2RXD		2	/*  PBFR2: UT2RXD	*/
#define PB2_TSPI1SCK		3	/*  PBFR3: TSPI1SCK	*/
#define PB2_T32A01INA1		4	/*  PBFR4: T32A01INA1	*/
#define PB2_T32A01INC1		5	/*  PBFR5: T32A01INC1	*/

#define PB3_INPUT		0	/*     0: Input Port	*/
#define PB3_OUTPUT		0	/*     0: Output Port	*/
#define PB3_UT2RXD		1	/* PBFR1: UT2RXD	*/
#define PB3_UT2TXDA		2	/* PBFR2: UT2TXDA	*/
#define PB3_TSPI1TXD		3	/* PBFR3: Output Port	*/
#define PB3_T32A01OUTB		4	/* PBFR4: Output Port	*/

#define PB4_INPUT		0	/*     0: Input Port	*/
#define PB4_OUTPUT		0	/*     0: Output Port	*/
#define PB4_UT2CTS_N		1	/* PBFR1: UT2CTS_N	*/
#define PB4_UT2RTS_N		2	/* PBFR2: UT2RTS_N	*/
#define PB4_TSPI1RXD		3	/* PBFR3: TSPI1RXD	*/
#define PB4_T32A01INB0		4	/* PBFR4: T32A01INB0	*/

#define PB5_INPUT		0	/*     0: Input Port	*/
#define PB5_OUTPUT		0	/*     0: Output Port	*/
#define PB5_UT2RTS_N		1	/* PBFR1: UT2RTS_N	*/
#define PB5_UT2CTS_N		2	/* PBFR2: T2CTS_N	*/
#define PB5_TSPI1CS0		3	/* PBFR3: TSPI1CS0	*/
#define PB5_T32A01INB1		4	/* PBFR4: T32A01INB1	*/
#define PB5_TSPI1CSIN		5	/* PBFR5: TSPI1CSIN	*/

#define PB6_INPUT		0	/*     0: Input Port	*/
#define PB6_OUTPUT		0	/*     0: Output Port	*/
#define PB6_TSPI1CS1		3	/* PBFR3: TSPI1CS1	*/

#define PB7_INPUT		NA	/* N/A: Input Port	*/
#define PB7_OUTPUT		NA	/* N/A: Output Port	*/
#define PB7_INT16		NA	/* N/A: INT16		*/

/* Port C */
#define PC0_INPUT		0	/*     0: Input Port	*/
#define PC0_OUTPUT		0	/*     0: Output Port	*/
#define PC0_INT00		0	/*     0: INT00		*/
#define PC0_I2C0SCL		1	/* PCFR1: I2C0SCL	*/
#define PC0_T32A02OUTA		3	/* PCFR3: T32A02OUTA	*/
#define PC0_T32A02OUTC		4	/* PCFR4: T32A02OUTC	*/

#define PC1_INPUT		0	/*     0: Input Port	*/
#define PC1_OUTPUT		0	/*     0: Output Port	*/
#define PC1_INT01		0	/*     0: INT01		*/
#define PC1_I2C0SDA		1	/* PCFR1: ISC0SDA	*/
#define PC1_T32A02INA0		3	/* PCFR3: T32A02INA0	*/
#define PC1_T32A02INC0		4	/* PCFR4: T32A02INC0	*/

#define PC2_INPUT		0	/*     0: Input Port	*/
#define PC2_OUTPUT		0	/*     0: Output Port	*/
#define PC2_INT02		0	/*     0: INT02		*/
#define PC2_UT4TXDB		1	/* PCFR1: UT4TXDB	*/
#define PC2_T32A02INA1		3	/* PCFR3: T32A02INA1	*/
#define PC2_T32A02INC1		4	/* PCFR4: T32A02INC1	*/
#define PC2_RTCOUT		5	/* PCFR5: RTCOUT	*/

#define PC3_INPUT		0	/*     0: Input Port	*/
#define PC3_OUTPUT		0	/*     0: Output Port	*/
#define PC3_UT4TXDA		1	/* PCFR1: UT4TXDA	*/
#define PC3_UT4RXD		2	/* PCFR2: UT4RXD	*/
#define PC3_T32A02OUTB		3	/* PCFR3: T32A2OUTB	*/

#define PC4_INPUT		0	/*     0: Input Port	*/
#define PC4_OUTPUT		0	/*     0: Output Port	*/
#define PC4_UT4RXD		1	/* PCFR1: UT4RXD	*/
#define PC4_UT4TXDA		2	/* PCFR2: UT4TXDA	*/
#define PC4_T32A02INB0		3	/* PCFR3: T32A2INB0	*/

#define PC5_INPUT		0	/*     0: Input Port	*/
#define PC5_OUTPUT		0	/*     0: Output Port	*/
#define PC5_UT4CTS_N		1	/* PCFR1: UT4CTS_N	*/
#define PC5_UT4RTS_N		2	/* PCFR2: UT4RTS_N	*/
#define PC5_T32A02INB1		3	/* PCFR3: T32A2INB1	*/

#define PC6_INPUT		0	/*     0: Input Port	*/
#define PC6_OUTPUT		0	/*     0: Output Port	*/
#define PC6_UT4RTS_N		1	/* PCFR1: UT4RTS_N	*/
#define PC6_UT4CTS_N		2	/* PCFR2: UT4CTS_N	*/

/* Port D */
#define PD0_INPUT		NA	/* N/A: Input Port	*/
#define PD0_OUTPUT		NA	/* N/A: Output Port	*/
#define PD0_AINA00		NA	/* N/A: AINA00		*/

#define PD1_INPUT		NA	/* N/A: Input Port	*/
#define PD1_OUTPUT		NA	/* N/A: Output Port	*/
#define PD1_AINA01		NA	/* N/A: AINA01		*/

#define PD2_INPUT		NA	/* N/A: Input Port	*/
#define PD2_OUTPUT		NA	/* N/A: Output Port	*/
#define PD2_AINA02		NA	/* N/A: AINA02		*/

#define PD3_INPUT		NA	/* N/A: Input Port	*/
#define PD3_OUTPUT		NA	/* N/A: Output Port	*/
#define PD3_AINA03		NA	/* N/A: AINA03		*/

#define PD4_INPUT		NA	/* N/A: Input Port	*/
#define PD4_OUTPUT		NA	/* N/A: Output Port	*/
#define PD4_AINA19		NA	/* N/A: AINA19		*/

/* Port D */
#define PD5_INPUT		NA	/* N/A: Input Port	*/
#define PD5_OUTPUT		NA	/* N/A: Output Port	*/
#define PD5_AINA20		NA	/* N/A: AINA20		*/

/* Port E */
#define PE0_INPUT		NA	/* N/A: Input Port	*/
#define PE0_OUTPUT		NA	/* N/A: Output Port	*/
#define PE0_AINA04		NA	/* N/A: AINA04		*/

#define PE1_INPUT		NA	/* N/A: Input Port	*/
#define PE1_OUTPUT		NA	/* N/A: Output Port	*/
#define PE1_AINA05		NA	/* N/A: AINA05		*/

#define PE2_INPUT		NA	/* N/A: Input Port	*/
#define PE2_OUTPUT		NA	/* N/A: Output Port	*/
#define PE2_AINA06		NA	/* N/A: AINA06		*/

#define PE3_INPUT		NA	/* N/A: Input Port	*/
#define PE3_OUTPUT		NA	/* N/A: Output Port	*/
#define PE3_AINA07		NA	/* N/A: AINA07		*/

#define PE4_INPUT		NA	/* N/A: Input Port	*/
#define PE4_OUTPUT		NA	/* N/A: Output Port	*/
#define PE4_AINA08		NA	/* N/A: AINA08		*/

#define PE5_INPUT		NA	/* N/A: Input Port	*/
#define PE5_OUTPUT		NA	/* N/A: Output Port	*/
#define PE5_AINA09		NA	/* N/A: AINA09		*/

#define PE6_INPUT		NA	/* N/A: Input Port	*/
#define PE6_OUTPUT		NA	/* N/A: Output Port	*/
#define PE6_AINA10		NA	/* N/A: AINA10		*/

/* Port F */
#define PF0_INPUT		NA	/* N/A: Input Port	*/
#define PF0_OUTPUT		NA	/* N/A: Output Port	*/
#define PF0_AINA11		NA	/* N/A: AINA11		*/

#define PF1_INPUT		NA	/* N/A: Input Port	*/
#define PF1_OUTPUT		NA	/* N/A: Output Port	*/
#define PF1_AINA12		NA	/* N/A: AINA12		*/

#define PF2_INPUT		NA	/* N/A: Input Port	*/
#define PF2_OUTPUT		NA	/* N/A: Output Port	*/
#define PF2_AINA13		NA	/* N/A: AINA13		*/

#define PF3_INPUT		NA	/* N/A: Input Port	*/
#define PF3_OUTPUT		NA	/* N/A: Output Port	*/
#define PF3_AINA14		NA	/* N/A: AINA14		*/

#define PF4_INPUT		NA	/* N/A: Input Port	*/
#define PF4_OUTPUT		NA	/* N/A: Output Port	*/
#define PF4_AINA15		NA	/* N/A: AINA15		*/

#define PF5_INPUT		NA	/* N/A: Input Port	*/
#define PF5_OUTPUT		NA	/* N/A: Output Port	*/
#define PF5_AINA16		NA	/* N/A: AINA16		*/

#define PF6_INPUT		NA	/* N/A: Input Port	*/
#define PF6_OUTPUT		NA	/* N/A: Output Port	*/
#define PF6_AINA17		NA	/* N/A: AINA17		*/

/* Port F */
#define PF7_INPUT		NA	/* N/A: Input Port	*/
#define PF7_OUTPUT		NA	/* N/A: Output Port	*/
#define PF7_AINA18		NA	/* N/A: AINA18		*/

/* Port G */
#define PG0_INPUT		NA	/* N/A: Input Port	*/
#define PG0_OUTPUT		NA	/* N/A: Output Port	*/
#define PG0_DAC0		NA	/* N/A: DAC0		*/

#define PG1_INPUT		NA	/* N/A: Input Port	*/
#define PG1_OUTPUT		NA	/* N/A: Output Port	*/
#define PG1_DAC1		NA	/* N/A: DAC1		*/

#define PG2_INPUT		0	/*     0: Input Port	*/
#define PG2_OUTPUT		0	/*     0: Output Port	*/
#define PG2_INT27		0	/*     0: INT27		*/
#define PG2_UT3RXD		1	/* PGFR1: UT3RXD	*/
#define PG2_UT3TXDA		2	/* PGFR2: UT3TXDA	*/
#define PG2_T32A07OUTA		3	/* PGFR3: T32A07OUTA	*/
#define PG2_T32A07OUTC		4	/* PGFR4: T32A07OUTC	*/

#define PG3_INPUT		0	/*     0: Input Port	*/
#define PG3_OUTPUT		0	/*     0: Output Port	*/
#define PG3_INT28		0	/*     0: INT27		*/
#define PG3_UT3TXDA		1	/* PGFR1: UT3TXDA	*/
#define PG3_UT3RXD		2	/* PGFR2: UT3RXD	*/
#define PG3_T32A07INA0		3	/* PGFR3: T32A07INA0	*/
#define PG3_T32A07INC0		4	/* PGFR4: T32A07INC0	*/

#define PG4_INPUT		0	/*     0: Input Port	*/
#define PG4_OUTPUT		0	/*     0: Output Port	*/
#define PG4_UT3TXDB		1	/* PGFR1: UT3TXDB	*/
#define PG4_T32A07INA1		3	/* PGFR3: T32A07INA1	*/
#define PG4_T32A07INC1		4	/* PGFR4: T32A07INC1	*/

#define PG5_INPUT		0	/*     0: Input Port	*/
#define PG5_OUTPUT		0	/*     0: Output Port	*/
#define PG5_T32A07OUTB		3	/* PGFR3: T32A07OUTB	*/

#define PG6_INPUT		0	/*     0: Input Port	*/
#define PG6_OUTPUT		0	/*     0: Output Port	*/
#define PG6_T32A07INB0		3	/* PGFR3: T32A07INB0	*/

#define PG7_INPUT		0	/*     0: Input Port	*/
#define PG7_OUTPUT		0	/*     0: Output Port	*/
#define PG7_T32A07INB1		3	/* PGFR3: T32A07INB1	*/

/* Port H */
#define PH0_INPUT		NA	/* N/A: Input Port	*/
#define PH0_X1			NA	/* N/A: X1		*/
#define PH0_EHCLKIN		NA	/* N/A: EHCLKIN		*/

#define PH1_INPUT		NA	/* N/A Input Port	*/
#define PH1_X2			NA	/* N/A X2		*/

#define PH2_INPUT		NA	/* N/A: Input Port	*/
#define PH2_XT1			NA	/* N/A: XT1		*/

#define PH3_INPUT		NA	/* N/A: Input Port	*/
#define PH3_XT2			NA	/* N/A: XT2		*/
#define PH3_INT06		NA	/* N/A: INT06		*/

#define PH4_INPUT		0	/*     0: Input Port	*/
#define PH4_OUTPUT		0	/*     0: Output Port	*/
#define PH4_INT19		0	/*     0: INT19		*/
#define PH4_TSPI4SCK		1	/* PHFR1: TSPI4SCK	*/

#define PH5_INPUT		0	/*     0: Input Port	*/
#define PH5_OUTPUT		0	/*     0: Output Port	*/
#define PH5_INT20		0	/*     0: INT19		*/
#define PH5_TSPI4TXD		1	/* PHFR1: TSPI4TXD	*/

#define PH6_INPUT		0	/*     0: Input Port	*/
#define PH6_OUTPUT		0	/*     0: Output Port	*/
#define PH6_INT21		0	/*     0: INT21		*/
#define PH6_TSPI4RXD		1	/* PHFR1: TSPI4RXD	*/

#define PH7_INPUT		NA	/* N/A: Input Port	*/
#define PH7_OUTPUT		NA	/* N/A: Output Port	*/
#define PH7_INT22		NA	/* N/A: INT22		*/

/* Port J */
#define PJ0_INPUT		0	/*     0: Input Port	*/
#define PJ0_OUTPUT		0	/*     0: Output Port	*/
#define PJ0_UT1TXDB		1	/* PJFR1: UT1TXDB	*/
#define PJ0_T32A03OUTA		3	/* PJFR3: T32A03OUTA	*/
#define PJ0_T32A03OUTC		4	/* PJFR4: T32A03OUTC	*/
#define PJ0_UO0			5	/* PJFR5: UO0		*/

#define PJ1_INPUT		0	/*     0: Input Port	*/
#define PJ1_OUTPUT		0	/*     0: Output Port	*/
#define PJ1_UT1TXDA		1	/* PJFR1: UT1TXDA	*/
#define PJ1_UT1RXD		2	/* PJFR2: UT1RXD	*/
#define PJ1_T32A03INA0		3	/* PJFR3: T32A03INA0	*/
#define PJ1_T32A03INC0		4	/* PJFR4: T32A03INC0	*/
#define PJ1_XO0			5	/* PJFR5: XO0		*/

#define PJ2_INPUT		0	/*     0: Input Port	*/
#define PJ2_OUTPUT		0	/*     0: Output Port	*/
#define PJ2_UT1RXD		1	/* PJFR1: UT1RXD	*/
#define PJ2_UT1TXDA		2	/* PJFR2: UT1TXDA	*/
#define PJ2_T32A03INA1		3	/* PJFR3: T32A03INA1	*/
#define PJ2_T32A03INC1		4	/* PJFR4: T32A03INC1	*/
#define PJ2_VO0			5	/* PJFR5: VO0		*/

#define PJ3_INPUT		0	/*     0: Input Port	*/
#define PJ3_OUTPUT		0	/*     0: Output Port	*/
#define PJ3_UT1CTS_N		1	/* PJFR1: UT1CTS_N	*/
#define PJ3_UT1RTS_N		2	/* PJFR2: UT1RTS_N	*/
#define PJ3_T32A03OUTB		3	/* PJFR3: T32A03OUTB	*/
#define PJ3_YO0			5	/* PJFR5: YO0		*/

#define PJ4_INPUT		0	/*     0: Input Port	*/
#define PJ4_OUTPUT		0	/*     0: Output Port	*/
#define PJ4_INT04		0	/*     0: INT04		*/
#define PJ4_UT1RTS_N		1	/* PJFR1: UT1RTS_N	*/
#define PJ4_UT1CTS_N		2	/* PJFR2: UT1CTS_N	*/
#define PJ4_T32A03INB0		3	/* PJFR3: T32A03INB0	*/
#define PJ4_WO0			5	/* PJFR5: WO0		*/

#define PJ5_INPUT		0	/* 0: Input Port	*/
#define PJ5_OUTPUT		0	/* 0: Output Port	*/
#define PJ5_T32A03INB1		3	/* PJFR3: T32A03INB1	*/
#define PJ5_ZO0			5	/* PJFR5: ZO0		*/

/* Port K */
#define PK0_INPUT		0	/*     0: Input Port	*/
#define PK0_OUTPUT		0	/*     0: Output Port	*/
#define PK0_UT1TXDB		1	/* PKFR1: UT1TXDB	*/
#define PK0_EMG0		5	/* PKFR5: EMG0		*/

#define PK1_INPUT		0	/*     0: Input Port	*/
#define PK1_OUTPUT		0	/*     0: Output Port	*/
#define PK1_INT05		0	/*     0: INT05		*/
#define PK1_UT1TXDA		1	/* PKFR1: UT1TXDA	*/
#define PK1_UT1RXD		2	/* PKFR2: UT1RXD	*/
#define PK1_OVV0		5	/* PKFR5: OVV0		*/

#define PK2_INPUT		0	/*     0: Input Port	*/
#define PK2_OUTPUT		0	/*     0: Output Port	*/
#define PK2_UT1RXD		1	/* PKFR1: UT1RXD	*/
#define PK2_UT1TXDA		2	/* PKFR2: UT1TXDA	*/
#define PK2_T32A04OUTA		3	/* PKFR3: T32A04OUTA	*/
#define PK2_T32A04OUTC		4	/* PKFR4: T32A04OUTC	*/

#define PK3_INPUT		0	/*     0: Input Port	*/
#define PK3_OUTPUT		0	/*     0: Output Port	*/
#define PK3_UT1CTS_N		1	/* PKFR1: UT1CTS_N	*/
#define PK3_UT1RTS_N		2	/* PKFR2: UT1RTS_N	*/
#define PK3_T32A04INA0		3	/* PKFR3: T32A04INA0	*/
#define PK3_T32A04INC0		4	/* PKFR4: T32A04INC0	*/

#define PK4_INPUT		0	/*     0: Input Port	*/
#define PK4_OUTPUT		0	/*     0: Output Port	*/
#define PK4_UT1RTS_N		1	/* PKFR1: UT1RTS_N	*/
#define PK4_UT1CTS_N		2	/* PKFR2: UT1CTS_N	*/
#define PK4_T32A04INA1		3	/* PKFR3: T32A4INA1 2	*/
#define PK4_T32A04INC1		4	/* PKFR4: T32A4INC1	*/

#define PK5_INPUT		0	/*     0: Input Port	*/
#define PK5_OUTPUT		0	/*     0: Output Port	*/
#define PK5_T32A04OUTB		3	/* PKFR3: T32A04OUTB	*/

#define PK6_INPUT		0	/*     0: Input Port	*/
#define PK6_OUTPUT		0	/*     0: Output Port	*/
#define PK6_T32A04INB0		3	/* PKFR3: T32A04INB0	*/

#define PK7_INPUT		0	/*     0: Input Port	*/
#define PK7_OUTPUT		0	/*     0: Output Port	*/
#define PK7_INT13		0	/*     0: INT13		*/
#define PK7_T32A04INB1		3	/* PKFR3: T32A04INB1	*/

/* Port L */
#define PL0_INPUT		0	/*     0: Input Port	*/
#define PL0_OUTPUT		0	/*     0: Output Port	*/
#define PL0_UT2TXDA		1	/* PLFR1: UT2TXDA	*/
#define PL0_UT2RXD		2	/* PLFR2: UT2RXD	*/
#define PL0_I2C2SCL		3	/* PLFR3: I2C2SCL	*/

#define PL1_INPUT		0	/*     0: Input Port	*/
#define PL1_OUTPUT		0	/*     0: Output Port	*/
#define PL1_UT2RXD		1	/* PLFR1: UT2RXD	*/
#define PL1_UT2TXDA		2	/* PLFR2: UT2TXDA	*/
#define PL1_I2C2SDA		3	/* PLFR3: I2C2SDA	*/

#define PL2_INPUT		0	/*     0: Input Port	*/
#define PL2_OUTPUT		0	/*     0: Output Port	*/
#define PL2_UT2CTS_N		1	/* PLFR1: UT2CTS_N	*/
#define PL2_UT2RTS_N		2	/* PLFR2: UT2RTS_N	*/
#define PL2_T32A06OUTB		3	/* PLFR3: T32A06OUTB	*/

#define PL3_INPUT		0	/*     0: Input Port	*/
#define PL3_OUTPUT		0	/*     0: Output Port	*/
#define PL3_INT08		0	/*     0: INT08		*/
#define PL3_UT2RTS_N		1	/* PLFR1: UT2RTS_N	*/
#define PL3_UT2CTS_N		2	/* PLFR2: UT2CTS_N	*/
#define PL3_T32A06INB0		3	/* PLFR3: T32A06INB0	*/

#define PL4_INPUT		0	/*     0: Input Port	*/
#define PL4_OUTPUT		0	/*     0: Output Port	*/
#define PL4_INT12		0	/*     0: IN112		*/
#define PL4_T32A06INB1		3	/* PLFR3: T32A06INB1	*/

#define PL5_INPUT		0	/* 0: Input Port	*/
#define PL5_OUTPUT		0	/* 0: Output Port	*/
#define PL5_T32A06OUTA		3	/* PLFR3: T32A06OUTA	*/
#define PL5_T32A06OUTC		4	/* PLFR4: T32A06OUTC	*/

#define PL6_INPUT		0	/* 0: Input Port	*/
#define PL6_OUTPUT		0	/* 0: Output Port	*/
#define PL6_T32A06INA0		3	/* PLFR3: T32A06INA0	*/
#define PL6_T32A06INC0		4	/* PLFR4: T32A06INC0	*/

#define PL7_INPUT		0	/* 0: Input Port	*/
#define PL7_OUTPUT		0	/* 0: Output Port	*/
#define PL7_T32A06INA1		3	/* PLFR3: T32A06INA1	*/
#define PL7_T32A06INC1		4	/* PLFR4: T32A06INC1	*/

/* Port M */
#define PM0_INPUT		0	/*     0: Input Port	*/
#define PM0_OUTPUT		0	/*     0: Output Port	*/
#define PM0_UT0TXDB		1	/* PMFR1: UT0TXDB	*/
#define PM0_TSPI0SCK		3	/* PMFR3: TSPI0SCK	*/
#define PM0_T32A00OUTA		4	/* PMFR4: T32A00OUTA	*/
#define PM0_T32A00OUTC		5	/* PMFR5: T32A00OUTC	*/
#define PM0_TRACECLK		6	/* PMFR6: TRACECLK	*/

#define PM1_INPUT		0	/*     0: Input Port	*/
#define PM1_OUTPUT		0	/*     0: Output Port	*/
#define PM1_UT0TXDA		1	/* PMFR1: UT0TXDA	*/
#define PM1_UT0RXD		2	/* PMFR2: UT0RXD	*/
#define PM1_TSPI0TXD		3	/* PMFR3: TSPI0TXD	*/
#define PM1_T32A00INA0		4	/* PMFR4: T32A00INA0	*/
#define PM1_T32A00INC0		5	/* PMFR5: T32A00INC0	*/
#define PM1_TRACEDATA0		6	/* PMFR6: TRACEDATA0	*/

#define PM2_INPUT		0	/*     0: Input Port	*/
#define PM2_OUTPUT		0	/*     0: Output Port	*/
#define PM2_INT09		0	/*     0: Output Port	*/
#define PM2_UT0RXD		1	/* PMFR1: UT0RXD	*/
#define PM2_UT0TXDA		2	/* PMFR2: UT0TXDA	*/
#define PM2_TSPI0RXD		3	/* PMFR3: TSPI0RXD	*/
#define PM2_T32A00INA1		4	/* PMFR4: T32A00INA1	*/
#define PM2_T32A00INC1		5	/* PMFR5: T32A00INC1	*/
#define PM2_TRACEDATA1		6	/* PMFR6: TRACEDATA1	*/

#define PM3_INPUT		0	/*     0: Input Port	*/
#define PM3_OUTPUT		0	/*     0: Output Port	*/
#define PM3_UT0CTS_N		1	/* PMFR1: UT0CTS_N	*/
#define PM3_UT0RTS_N		2	/* PMFR2: UT0RTS_N	*/
#define PM3_TSPI0CS0		3	/* PMFR3: TSPI0CS0	*/
#define PM3_T32A00OUTB		4	/* PMFR4: T32A00OUTB	*/
#define PM3_TSPI0CSIN		5	/* PMFR5: TSPI0CSIN	*/
#define PM3_TRACEDATA2		6	/* PMFR6: TRACEDATA2	*/

#define PM4_INPUT		0	/*     0: Input Port	*/
#define PM4_OUTPUT		0	/*     0: Output Port	*/
#define PM4_UT0RTS_N		1	/* PMFR1: UT0RTS_N	*/
#define PM4_UT0CTS_N		2	/* PMFR2: UT0CTS_N	*/
#define PM4_TSPI0CS1		3	/* PMFR3: TSPI0CS1	*/
#define PM4_T32A00INB0		4	/* PMFR4: T32A00INB0	*/
#define PM4_TRACEDATA3		6	/* PMFR6: TRACEDATA3	*/

#define PM5_INPUT		0	/*     0: Input Port	*/
#define PM5_OUTPUT		0	/*     0: Output Port	*/
#define PM5_T32A00INB1		4	/* PMFR4: T32A00INB1	*/

#define PM6_INPUT		NA	/* N/A: Input Port	*/
#define PM6_OUTPUT		NA	/* N/A: Output Port	*/
#define PM6_INT15		NA	/* N/A: INT15		*/

#define PM7_INPUT		NA	/* N/A: Input Port	*/
#define PM7_OUTPUT		NA	/* N/A: Output Port	*/

/* Port N */
#define PN0_INPUT		0	/*     0: Input Port	*/
#define PN0_OUTPUT		0	/*     0: Output Port	*/
#define PN0_UT5RTS_N		1	/* PNFR1: UT5RTS_N	*/
#define PN0_UT5CTS_N		2	/* PNFR2: UT5CTS_N	*/
#define PN0_T32A05OUTA		3	/* PNFR3: T32A05OUTA	*/
#define PN0_T32A05OUTC		4	/* PNFR4: T32A05OUTC	*/

#define PN1_INPUT		0	/*     0: Input Port	*/
#define PN1_OUTPUT		0	/*     0: Output Port	*/
#define PN1_UT5CTS_N		1	/* PNFR1: UT5CTS_N	*/
#define PN1_UT5RTS_N		2	/* PNFR2: UT5RTS_N	*/
#define PN1_T32A05INA0		3	/* PNFR3: T32A05INA0	*/
#define PN1_T32A05INC0		4	/* PNFR4: T32A05INC0	*/

#define PN2_INPUT		0	/*     0: Input Port	*/
#define PN2_OUTPUT		0	/*     0: Output Port	*/
#define PN2_UT5RXD		1	/* PNFR1: UT5RXD	*/
#define PN2_UT5TXDA		2	/* PNFR2: UT5TXDA	*/
#define PN2_T32A05INA1		3	/* PNFR3: T32A05INA1	*/
#define PN2_T32A05INC1		4	/* PNFR4: T32A05INC1	*/

#define PN3_INPUT		0	/*     0: Input Port	*/
#define PN3_OUTPUT		0	/*     0: Output Port	*/
#define PN3_INT10		0	/*     0: INT10		*/
#define PN3_UT5TXDA		1	/* PNFR1: UT5TXDA	*/
#define PN3_UT5RXD		2	/* PNFR2: UT5RXD	*/
#define PN3_T32A05OUTB		3	/* PNFR3: T32A05OUTB	*/
#define PN3_TRGIN2		5	/* PNFR5: TRGIN2	*/

#define PN4_INPUT		0	/*     0: Input Port	*/
#define PN4_OUTPUT		0	/*     0: Output Port	*/
#define PN4_UT5TXDB		1	/* PNFR1: UT5TXDB	*/
#define PN4_T32A05INB0		3	/* PNFR3: T32A05INB0	*/

#define PN5_INPUT		0	/*     0: Input Port	*/
#define PN5_OUTPUT		0	/*     0: Output Port	*/
#define PN5_T32A05INB1		3	/* PNFR3: T32A05INB1	*/

/* Port P */
#define PP0_INPUT		0	/*     0: Input Port	*/
#define PP0_OUTPUT		0	/*     0: Output Port	*/
#define PP0_TSPI2SCK		2	/* PPFR2: TSPI2SCK	*/
#define PP0_T32A01OUTA		3	/* PPFR3: T32A01OUTA	*/
#define PP0_T32A01OUTC		4	/* PPFR4: T32A01OUTC	*/

#define PP1_INPUT		0	/*     0: Input Port	*/
#define PP1_OUTPUT		0	/*     0: Output Port	*/
#define PP1_TSPI2TXD		2	/* PPGFR2: TSPI2TXD	*/
#define PP1_T32A01INA0		3	/* PPFR3: T32A01INA0	*/
#define PP1_T32A01INC0		4	/* PPFR4: T32A01INC0	*/

#define PP2_INPUT		0	/*     0: Input Port	*/
#define PP2_OUTPUT		0	/*     0: Output Port	*/
#define PP2_TSPI2RXD		2	/* PPFR2: TSPI2RXD	*/
#define PP2_T32A01INA1		3	/* PPFR3: T32A01INA1	*/
#define PP2_T32A01INC1		4	/* PPFR4: T32A01INC1	*/

#define PP3_INPUT		0	/*     0: Input Port	*/
#define PP3_OUTPUT		0	/*     0: Output Port	*/
#define PP3_INT14		0	/*     0: INT14		*/
#define PP3_TSPI3RXD		1	/* PPFR1: TSPI3RXD	*/

#define PP4_INPUT		0	/*     0: Input Port	*/
#define PP4_OUTPUT		0	/*     0: Output Port	*/
#define PP4_TSPI3TXD		1	/* PPFR1: TSPI3TXD	*/

#define PP5_INPUT		0	/*     0: Input Port	*/
#define PP5_OUTPUT		0	/*     0: Output Port	*/
#define PP5_TSPI3SCK		1	/* PPFR1: TSPI3SCK	*/

#define PP6_INPUT		0	/*     0: Input Port	*/
#define PP6_OUTPUT		0	/*     0: Output Port	*/
#define PP6_TSPI3CS0		1	/* PPFR1: TSPI3CS0	*/
#define PP6_TSPI3CSIN		2	/* PPFR2: TSPI3CSIN	*/
#define PP6_PMD0DBG		3	/* PPFR3: PMD0DBG	*/

#define PP7_INPUT		0	/*     0: Input Port	*/
#define PP7_OUTPUT		0	/*     0: Output Port	*/
#define PP7_TSPI3CS1		1	/* PPFR1: TSPI3SCK	*/

/* Port R */
#define PR0_INPUT		0	/*     0: Input Port	*/
#define PR0_OUTPUT		0	/*     0: Output Port	*/
#define PR0_T32A02OUTA		3	/* PRFR3: T32A02OUTA	*/
#define PR0_T32A02OUTC		4	/* PRFR4: T32A02OUTC	*/

#define PR1_INPUT		0	/*     0: Input Port	*/
#define PR1_OUTPUT		0	/*     0: Output Port	*/
#define PR1_T32A02INA0		3	/* PRFR3: T32A02INA0	*/
#define PR1_T32A02INC0		4	/* PRFR4: T32A02INC0	*/

#define PR2_INPUT		0	/*     0: Input Port	*/
#define PR2_OUTPUT		0	/*     0: Output Port	*/
#define PR2_T32A02INA1		3	/* PRFR3: T32A02INA1	*/
#define PR2_T32A02INC1		4	/* PRFR4: T32A02INC1	*/

#define PR3_INPUT		0	/*     0: Input Port	*/
#define PR3_OUTPUT		0	/*     0: Output Port	*/

#define PR4_INPUT		0	/*     0: Input Port	*/
#define PR4_OUTPUT		0	/*     0: Output Port	*/

#define PR5_INPUT		0	/*     0: Input Port	*/
#define PR5_OUTPUT		0	/*     0: Output Port	*/

#define PR6_INPUT		0	/*     0: Input Port	*/
#define PR6_OUTPUT		0	/*     0: Output Port	*/

#define PR7_INPUT		0	/*     0: Input Port	*/
#define PR7_OUTPUT		0	/*     0: Output Port	*/

/* Port T */
#define PT0_INPUT		0	/*     0: Input Port	*/
#define PT0_OUTPUT		0	/*     0: Output Port	*/
#define PT0_INT23		0	/*     0: INT23		*/
#define PT0_I2C3SDA		1	/* PTFR1: I2C3SDA	*/
#define PT0_TSPI2CS1		2	/* PTFR2: TSPI2CS1	*/

#define PT1_INPUT		0	/*     0: Input Port	*/
#define PT1_OUTPUT		0	/*     0: Output Port	*/
#define PT1_INT24		0	/*     0: INT24		*/
#define PT1_I2C3SCL		1	/* PTFR1: I2C3SCL	*/
#define PT1_TSPI2CS0		2	/* PTFR2: TSPI2CS0	*/
#define PT1_TSPI2CSIN		3	/* PTFR3: TSPI2CSIN	*/

#define PT2_INPUT		0	/*     0: Input Port	*/
#define PT2_OUTPUT		0	/*     0: Output Port	*/
#define PT2_INT25		0	/*     0: INT25		*/
#define PT2_TSPI2SCK		1	/* PTFR1: TSPI2SCK	*/
#define PT2_T32A06OUTB		3	/* PTFR3: T32A06OUTB	*/

#define PT3_INPUT		0	/*     0: Input Port	*/
#define PT3_OUTPUT		0	/*     0: Output Port	*/
#define PT3_INT26		0	/*     0: INT26		*/
#define PT3_TSPI2TXD		1	/* PTFR1: TSPI2TXD	*/
#define PT3_T32A06INB0		3	/* PTFR3: T32A06INB0	*/

#define PT4_INPUT		0	/*     0: Input Port	*/
#define PT4_OUTPUT		0	/*     0: Output Port	*/
#define PT4_TSPI2RXD		1	/* PTFR1: TSPI2RXD	*/
#define PT4_T32A06INB1		3	/* PTFR3: T32A06INB1	*/

#define PT5_INPUT		0	/*     0: Input Port	*/
#define PT5_OUTPUT		0	/*     0: Output Port	*/
#define PT5_T32A06OUTA		3	/* PTFR3: T32A06OUTA	*/
#define PT5_T32A06OUTC		4	/* PTFR4: T32A06OUTC	*/

#define PT6_INPUT		0	/*     0: Input Port	*/
#define PT6_OUTPUT		0	/*     0: Output Port	*/
#define PT6_T32A06INA0		3	/* PTFR3: T32A06INA0	*/
#define PT6_T32A06INC0		4	/* PTFR4: T32A06INC0	*/

#define PT7_INPUT		0	/*     0: Input Port	*/
#define PT7_OUTPUT		0	/*     0: Output Port	*/
#define PT7_INT29		0	/*     0: INT29		*/
#define PT7_T32A06INA1		3	/* PTFR3: T32A06INA1	*/
#define PT7_T32A06INC1		4	/* PTFR4: T32A06INC1	*/

/* Port U */
#define PU0_INPUT		NA	/* N/A: Input Port	*/
#define PU0_OUTPUT		NA	/* N/A: Output Port	*/
#define PU0_INT30		NA	/* N/A: INT30		*/

#define PU1_INPUT		NA	/* N/A: Input Port	*/
#define PU1_OUTPUT		NA	/* N/A: Output Port	*/
#define PU1_INT31		NA	/* N/A: INT31		*/

#define PU2_INPUT		NA	/* N/A: Input Port	*/
#define PU2_OUTPUT		NA	/* N/A: Output Port	*/

#define PU3_INPUT		NA	/* N/A: Input Port	*/
#define PU3_OUTPUT		NA	/* N/A: Output Port	*/

#define PU4_INPUT		NA	/* N/A: Input Port	*/
#define PU4_OUTPUT		NA	/* N/A: Output Port	*/

#define PU5_INPUT		NA	/* N/A: Input Port	*/
#define PU5_OUTPUT		NA	/* N/A: Output Port	*/

/* Port V */
#define PV0_INPUT		0	/*   0: Input Port	*/
#define PV0_OUTPUT		0	/*   0: Output Port	*/

#define PV1_INPUT		0	/*     0: Input Port	*/
#define PV1_OUTPUT		0	/*     0: Output Port	*/

#define PV2_INPUT		0	/*     0: Input Port	*/
#define PV2_OUTPUT		0	/*     0: Output Port	*/
#define PV2_INT17		0	/*     0: INT17		*/

#define PV3_INPUT		0	/*     0: Input Port	*/
#define PV3_OUTPUT		0	/*     0: Output Port	*/
#define PV3_INT18		0	/*     0: INT18		*/

#define PV4_INPUT		NA	/* N/A: Input Port	*/
#define PV4_OUTPUT		NA	/* N/A: Output Port	*/

#define PV5_INPUT		0	/*     0: Input Port	*/
#define PV5_OUTPUT		0	/*     0: Output Port	*/
#define PV5_UT4TXDB		1	/* PVFR1: UT4TXDB	*/

#define PV6_INPUT		0	/*     0: Input Port	*/
#define PV6_OUTPUT		0	/*     0: Output Port	*/
#define PV6_UT4TXDA		1	/* PVFR1: UT4TXDA	*/
#define PV6_UT4RXD		2	/* PVFR2: UT4RXD	*/

#define PV7_INPUT		0	/*     0: Input Port	*/
#define PV7_OUTPUT		0	/*     0: Output Port	*/
#define PV7_UT4RXD		1	/* PVFR1: UTRXD		*/
#define PV7_UT4TXDA		2	/* PVFR2: UT4TXDA	*/

#define PORT_IN			0x00000001ul
#define PORT_OUT		0x00000002ul
#define PORT_INOUT		(PORT_IN|PORT_OUT)
#define PORT_NOINOUT		0x00000000ul

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for TMPM3HQF10 TMRB)
 */
#define	CPU_HAS_PTMR		(1)

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		0
#define CPU_HAS_DPS		0

/*
 *  Number of coprocessors to use. 
 *    There is no coprocessor for this microcomputer.
 */
#define NUM_COPROCESSOR		0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */

