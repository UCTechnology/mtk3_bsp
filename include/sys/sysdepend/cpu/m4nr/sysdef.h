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
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (TMPM4NR depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

#include "../../../machine.h"

/* CPU Core-dependent definition */
#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */
/* TMPM4NR Internal RAM0/1/2/3/4/5  0x20000000 to 0x20040000  (Size 256KB = 64KB+64KB+32KB+32KB+32KB+32KB) */
#if CPU_M4NR
#define INTERNAL_RAM_SIZE       (0x20040000-0x20000000)
#define INTERNAL_RAM_START      0x20000000
#endif

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

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
#define CG_BASE		(0x40083000ul)			/* CG-M4M(1)-E */
#define CGPROTECT	(_UW*)(CG_BASE+0x0000ul)	/* Write Protect */
#define CGOSCCR		(_UW*)(CG_BASE+0x0004ul)	/* Oscillation control */
#define CGSYSCR		(_UW*)(CG_BASE+0x0008ul)	/* System clock control */
#define CGSTBYCR	(_UW*)(CG_BASE+0x000Cul)	/* Standby control */
#define CGPLL0SEL	(_UW*)(CG_BASE+0x0020ul)	/* PLL select for fsys */
#define CGWUPHCR	(_UW*)(CG_BASE+0x0030ul)	/* High-speed oscillation warm-up */
#define CGFSYSMENC	(_UW*)(CG_BASE+0x0044ul)	/* fsysm supply stop C */
#define CGFSYSMENA	(_UW*)(CG_BASE+0x0048ul)	/* fsysm supply stop A */
#define CGFSYSMENB	(_UW*)(CG_BASE+0x004Cul)	/* fsysm supply stop B */
#define CGFSYSENA	(_UW*)(CG_BASE+0x0050ul)	/* fsys supply stop A */
#define CGFCEN		(_UW*)(CG_BASE+0x0058ul)	/* fc supply stop */
#define CGSPCLKEN	(_UW*)(CG_BASE+0x005Cul)	/* ADC, trace clock supply stop */

/*------ VALUE in CGPROTECT Register ------*/


/*------ VALUE in CGOSCCR Register ------*/
#define CGOSCCR_IHOSC1F_STAT	0x00010000ul	/* IHOSC1 is stable  0:stop/wup, 1:stable */
#define CGOSCCR_OSCF_STAT	0x00000200ul	/* High-speed oscillator selection status for fosc */
#define CGOSCCR_OSCSEL_MASK	0x00000100ul	/* High-speed oscillator selection for fosc */
#define CGOSCCR_OSCSEL_IHOSC1	0x00000000ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGOSCCR_OSCSEL_EHOSC	0x00000100ul	/* External high-speed oscillator (EHOSC) */
#define CGOSCCR_EOSCEN_MASK	0x00000006ul	/* Operation selection of ext. high-speed oscillator (EHOSC) */
#define CGOSCCR_EOSCEN_NONE	0x00000000ul	/* External oscillation not used */
#define CGOSCCR_EOSCEN_EHOSC	0x00000002ul	/* External high-speed oscillation (EHOSC) */
#define CGOSCCR_EOSCEN_EHCLKIN	0x00000004ul	/* External clock input (EHCLKIN) */
#define CGOSCCR_IHOSC1EN_MASK	0x00000001ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGOSCCR_IHOSC1EN_STOP	0x00000000ul	/* Stop */
#define CGOSCCR_IHOSC1EN_OSC	0x00000001ul	/* Oscillation */

/*------ VALUE in CGSYSCR Register ------*/
#define CGSYSCR_MCKSELPST_STAT	0xC0000000ul	/* R:Ft0 select status */
#define CGSYSCR_MCKSELPST_D1	0x00000000ul	/*	/1	*/
#define CGSYSCR_MCKSELPST_D2	0x40000000ul	/*	/2	*/
#define CGSYSCR_MCKSELPST_D4	0x80000000ul	/*	/4	*/
#define CGSYSCR_PRCKST_STAT	0x0F000000ul	/* R:Prescaler clock (Ft0) selection */
#define CGSYSCR_MCKSELGST_STAT	0x00C00000ul	/* R:fsysm select status */
#define CGSYSCR_MCKSELGST_D1	0x00000000ul	/*	fsys/1	*/
#define CGSYSCR_MCKSELGST_D2	0x00400000ul	/*	fsys/2	*/
#define CGSYSCR_MCKSELGST_D4	0x00800000ul	/*	fsys/4	*/
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
#define CGSYSCR_MCKSEL_MASK	0x000000C0ul	/* fsysm Ft0 select : Max 80 MHz */
#define CGSYSCR_MCKSEL_D1	0x00000000ul	/* GEAR,PRCK/1	*/
#define CGSYSCR_MCKSEL_D2	0x00000040ul	/* GEAR,PRCK/2	*/
#define CGSYSCR_MCKSEL_D4	0x00000080ul	/* GEAR,PRCK/4	*/
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
#define CGWUPHCR_WUCLK_IHOSC1	0x00000000ul	/* Built-in high-speed oscillator (IHOSC1) */
#define CGWUPHCR_WUCLK_EHOSC	0x00000100ul	/* External high-speed oscillator (EHOSC) */
#define CGWUPHCR_WUEF_STAT	0x00000002ul	/* Warming-up timer status */
#define CGWUPHCR_WUON_MASK	0x00000001ul	/* Warming-up timer control */
#define CGWUPHCR_WUON_START	0x00000001ul	/* Warming up start */

/*------ VALUE in CGFSYSMENA Register ------*/
#define CGFSYSMENA_UART5	0x10000000ul	/* UART ch5 enable */
#define CGFSYSMENA_UART4	0x08000000ul	/* UART ch4 enable */
#define CGFSYSMENA_UART3	0x04000000ul	/* UART ch3 enable */
#define CGFSYSMENA_UART2	0x02000000ul	/* UART ch2 enable */
#define CGFSYSMENA_UART1	0x01000000ul	/* UART ch1 enable */
#define CGFSYSMENA_UART0	0x00800000ul	/* UART ch0 enable : CONSOLE */

/*------ VALUE in CGFSYSMENB Register ------*/
#define CGFSYSMENB_PY		0x00200000ul	/* Port Y enable */
#define CGFSYSMENB_PW		0x00100000ul	/* Port W enable : LED1,LED2,LED3,LED4 */
#define CGFSYSMENB_PV		0x00080000ul	/* Port V enable */
#define CGFSYSMENB_PU		0x00040000ul	/* Port U enable */
#define CGFSYSMENB_PT		0x00020000ul	/* Port T enable */
#define CGFSYSMENB_PR		0x00010000ul	/* Port R enable */
#define CGFSYSMENB_PP		0x00008000ul	/* Port P enable */
#define CGFSYSMENB_PN		0x00004000ul	/* Port N enable */
#define CGFSYSMENB_PM		0x00002000ul	/* Port M enable */
#define CGFSYSMENB_PL		0x00001000ul	/* Port L enable */
#define CGFSYSMENB_PK		0x00000800ul	/* Port K enable */
#define CGFSYSMENB_PJ		0x00000400ul	/* Port J enable */
#define CGFSYSMENB_PH		0x00000200ul	/* Port H enable */
#define CGFSYSMENB_PG		0x00000100ul	/* Port G enable */
#define CGFSYSMENB_PF		0x00000080ul	/* Port F enable */
#define CGFSYSMENB_PE		0x00000040ul	/* Port E enable : UART0 PE3=TxD, PE2=RxD */
#define CGFSYSMENB_PD		0x00000020ul	/* Port D enable */
#define CGFSYSMENB_PC		0x00000010ul	/* Port C enable */
#define CGFSYSMENB_PB		0x00000008ul	/* Port B enable */
#define CGFSYSMENB_PA		0x00000004ul	/* Port A enable */

/*------ VALUE in CGFSYSMENC Register ------*/
#define CGFSYSMENC_ETHMA	0x00000400ul	/* ETHM unit A Enable */

/*------ VALUE in CGFSYSENA Register ------*/

/*------ VALUE in CGFCEN Register ------*/
#define CGFCEN_FCIPEN27_MASK	0x08000000ul	/* Clock enable of DNF UnitB */
#define CGFCEN_FCIPEN27_STOP	0x00000000ul	/*	STOP		*/
#define CGFCEN_FCIPEN27_SUPPLY	0x08000000ul	/*	SUPPLY		*/
#define CGFCEN_FCIPEN26_MASK	0x04000000ul	/* Clock enable of DNF UnitB */
#define CGFCEN_FCIPEN26_STOP	0x00000000ul	/*	STOP		*/
#define CGFCEN_FCIPEN26_SUPPLY	0x04000000ul	/*	SUPPLY		*/
#define CGFCEN_FCIPEN23_MASK	0x00800000ul	/* Clock enable of OFD	*/
#define CGFCEN_FCIPEN23_STOP	0x00000000ul	/*	STOP		*/
#define CGFCEN_FCIPEN23_SUPPLY	0x00800000ul	/*	SUPPLY		*/
// aliases
#define CGFCEN_DNFB_MASK	CGFCEN_FCIPEN27_MASK
#define CGFCEN_DNFB_STOP	CGFCEN_FCIPEN27_STOP
#define CGFCEN_DNFB_SUPPLY	CGFCEN_FCIPEN27_SUPPLY
#define CGFCEN_DNFA_MASK	CGFCEN_FCIPEN26_MASK
#define CGFCEN_DNFA_STOP	CGFCEN_FCIPEN26_STOP
#define CGFCEN_DNFA_SUPPLY	CGFCEN_FCIPEN26_SUPPLY
#define CGFCEN_OFD_MASK		CGFCEN_FCIPEN23_MASK
#define CGFCEN_OFD_STOP		CGFCEN_FCIPEN23_STOP
#define CGFCEN_OFD_SUPPLY	CGFCEN_FCIPEN23_SUPPLY

/*------ VALUE in CGSPCLKEN Register ------*/
#define CGSPCLKEN_ADCKEN_MASK	0x00010000ul	/* Conversion clock enable for AD converter */
#define CGSPCLKEN_ADCKEN_STOP	0x00000000ul	/*	STOP		*/
#define CGSPCLKEN_ADCKEN_SUPPLY	0x00010000ul	/*	SUPPLY		*/
#define CGSPCLKEN_TRCKEN_MASK	0x00000001ul	/* Clock enable for trace function of debug circuit (ETM)*/
#define CGSPCLKEN_TRCKEN_STOP	0x00000000ul	/*	STOP		*/
#define CGSPCLKEN_TRCKEN_SUPPLY	0x00000001ul	/*	SUPPLY		*/

/*
 * Watch Dog Timer
 * TMPM4KN has only ch0.		TYPE2 : "PINFO-M4M(1)" (p.46)
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

/*
 * GPIO PORT
 */
#define GPIO_BASE	(0x400E0000ul)			/* PORT-M4M	 */
#define PA_BASE		(GPIO_BASE+0x0000ul)		/* A-H */
#define PJ_BASE		(GPIO_BASE+0x0800ul)		/* J-N */
#define PU_BASE		(GPIO_BASE+0x1000ul)		/* U-V */

#define PxBLOCK_B(x)	(((x)<'I')? PA_BASE:(((x)<'O')? PJ_BASE:PU_BASE))
#define PxBLOCK_C(x)	(((x)<'I')? 'A'    :(((x)<'O')? 'J'    :'U'))
#define PxBASE(x)	(PxBLOCK_B(x)+0x0100ul*((x)-PxBLOCK_C(x))) /* Px BASE (x = 'A' - 'V') */

#define PxDATA(x)	(_UW*)(PxBASE(x)+0x0000ul)	/* Data register */
#define PxCR(x)		(_UW*)(PxBASE(x)+0x0004ul)	/* Output control register */
#define PxFRn(x,n)	(_UW*)(PxBASE(x)+0x0008ul+4*((n)-1)) /* Function register n (n = 1 - 8) */
#define PxOD(x)		(_UW*)(PxBASE(x)+0x0028ul)	/* Open drain control register */
#define PxPUP(x)	(_UW*)(PxBASE(x)+0x002Cul)	/* Pull-up control register */
#define PxPDN(x)	(_UW*)(PxBASE(x)+0x0030ul)	/* pull-down control register */
#define PxIE(x)		(_UW*)(PxBASE(x)+0x0038ul)	/* Input control register */

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
#define N_INTVEC		157	/* Number of Interrupt vectors */
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
 * EXTI (Extended interrupt controller)
 */
#define EXTI_BASE	0x40010400

#define	EXTI_IMR1	(EXTI_BASE + 0x00)
#define	EXTI_EMR1	(EXTI_BASE + 0x04)
#define	EXTI_RTSR1	(EXTI_BASE + 0x08)
#define	EXTI_FTSR1	(EXTI_BASE + 0x0C)
#define	EXTI_SWIER1	(EXTI_BASE + 0x10)
#define	EXTI_PR1	(EXTI_BASE + 0x14)
#define	EXTI_IMR2	(EXTI_BASE + 0x20)
#define	EXTI_EMR2	(EXTI_BASE + 0x24)
#define	EXTI_RTSR2	(EXTI_BASE + 0x28)
#define	EXTI_FTSR2	(EXTI_BASE + 0x2C)
#define	EXTI_SWIER2	(EXTI_BASE + 0x30)
#define	EXTI_PR2	(EXTI_BASE + 0x34)

/* ------------------------------------------------------------------------ */
/*
 * Watchdog Timer
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	GPIOA_BASE	0x400E0000
#define	GPIOB_BASE	0x400E0100
#define	GPIOC_BASE	0x400E0200
#define	GPIOD_BASE	0x400E0300
#define	GPIOE_BASE	0x400E0400
#define	GPIOF_BASE	0x400E0500
#define	GPIOG_BASE	0x400E0600
#define	GPIOH_BASE	0x400E0700
#define	GPIOJ_BASE	0x400E0800
#define	GPIOK_BASE	0x400E0900
#define	GPIOL_BASE	0x400E0A00
#define	GPIOM_BASE	0x400E0B00
#define	GPION_BASE	0x400E0C00
#define	GPIOU_BASE	0x400E1000
#define	GPIOV_BASE	0x400E1100
#define	GPIOW_BASE	0x400E1200

#define GPIO_DATA(n)	(_UW*)(GPIO##n##_BASE + 0x00)	/* Data */
#define GPIO_CR(n)	(_UW*)(GPIO##n##_BASE + 0x04)	/* Output Control */
#define GPIO_FR1(n)	(_UW*)(GPIO##n##_BASE + 0x08)	/* Function1 */
#define GPIO_FR2(n)	(_UW*)(GPIO##n##_BASE + 0x0C)	/* Function2 */
#define GPIO_FR3(n)	(_UW*)(GPIO##n##_BASE + 0x10)	/* Function3 */
#define GPIO_FR4(n)	(_UW*)(GPIO##n##_BASE + 0x14)	/* Function4 */
#define GPIO_FR5(n)	(_UW*)(GPIO##n##_BASE + 0x18)	/* Function5 */
#define GPIO_FR6(n)	(_UW*)(GPIO##n##_BASE + 0x1C)	/* Function6 */
#define GPIO_FR7(n)	(_UW*)(GPIO##n##_BASE + 0x20)	/* Function7 */
#define GPIO_OD(n)	(_UW*)(GPIO##n##_BASE + 0x28)	/* Open Drain Control */
#define GPIO_PUP(n)	(_UW*)(GPIO##n##_BASE + 0x2C)	/* Pull-up Control */
#define GPIO_PDN(n)	(_UW*)(GPIO##n##_BASE + 0x30)	/* Pull-down Control */
#define GPIO_IE(n)	(_UW*)(GPIO##n##_BASE + 0x38)	/* Input Conotrol */

/* ------------------------------------------------------------------------ */
/*
 *	UART GPIO Definition for T-Monitor
 */
#define PE2_UT0RXD		7	/* PEFR7: UT0RXD	*/
#define PE3_UT0TXDA		7	/* PEFR7: UT0TXDA	*/

#define PORT_IN			0x00000001ul
#define PORT_OUT		0x00000002ul
#define PORT_INOUT		(PORT_IN|PORT_OUT)
#define PORT_NOINOUT		0x00000000ul

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for TMPM4KN)
 */
#define	CPU_HAS_PTMR	(1)

	/* NOT SUPPORTED */

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
