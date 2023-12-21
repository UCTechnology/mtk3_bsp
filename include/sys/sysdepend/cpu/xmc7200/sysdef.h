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
 *	sysdef.h
 *
 *	System dependencies definition (XMC7200 depended)
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

/* XMC7200 Internal SRAM	0x28000000 - 0x280FFFFF	1,024KB	*/
/*		for Cortex-M0+	0x28000000 - 0x28004000		16KB	*/
/*		for Cortex-M7_0	0x28004000 - 0x280F0000		944KB	*/
/*		for Cortex-M7_1	0x280F0000 - 0x28100000		64KB	*/
#define INTERNAL_RAM_SIZE       0x000EC000
#define INTERNAL_RAM_START      0x28004000
#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

/* ------------------------------------------------------------------------ */
/*
 * CPU subsystem (CPUSS)
 */
#define	CPUSS_BASE	(0x40200000)
#define	CM7_0_CTL	(_UW*)(CPUSS_BASE+0x000Cul)    /* CM7 0 control */
#define	CM7_0_INT_STATUS(x)	(_UW*)(CPUSS_BASE+0x0100ul+((x)*0x04ul))    /* CM7 0 interrupt status */
#define	RAM0_CTL	(_UW*)(CPUSS_BASE+0x1300ul)    /* RAM 0 control */
#define	RAM1_CTL	(_UW*)(CPUSS_BASE+0x1380ul)    /* RAM 1 control */
#define	RAM2_CTL	(_UW*)(CPUSS_BASE+0x13A0ul)    /* RAM 2 control */
#define	ROM_CTL		(_UW*)(CPUSS_BASE+0x13C4ul)    /* ROM control */
#define	CM7_0_SYSTEM_INT_CTL(x)	(_UW*)(CPUSS_BASE+0xA000ul+((x)*0x04ul))    /* CM7 0 interrupt status */

#define	PPB_LOCK_ITCMR	(1U << 0)
#define	PPB_LOCK_DTCMR	(1U << 1)
#define	PPB_LOCK_VTOR	(1U << 3)
#define	INIT_ITCM_EN	(1U << 8)
#define	INIT_DTCM_EN	(1U << 9)
#define	INIT_ITCM_RMW_EN	(1U << 10)
#define	INIT_DTCM_RMW_EN	(1U << 11)

#define	SYSTEM_INT_IDX_MASK	(0x000003FFU)
#define	SYSTEM_INT_VALID	(1U << 31)
#define SYSTEM_INT_VALID_Pos	(31U)

#define	CPU_INT_IDX(x)	((x) << 0)
#define	CPU_INT_VALID	(1U << 31)

#define SLOW_WS(x)	((x) << 0)	/* Memory wait states for the slow clock domain ('clk_slow').  */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	GPIO_BASE	(0x40310000)
#define	GPIO_PRT_OUT(x)	(_UW*)(GPIO_BASE+0x0000ul+((x)*0x80ul))
#define	GPIO_PRT_OUT_CLR(x)	(_UW*)(GPIO_BASE+0x0004ul+((x)*0x80ul))
#define	GPIO_PRT_OUT_SET(x)	(_UW*)(GPIO_BASE+0x0008ul+((x)*0x80ul))
#define	GPIO_PRT_OUT_INV(x)	(_UW*)(GPIO_BASE+0x000Cul+((x)*0x80ul))
#define	GPIO_PRT_IN(x)	(_UW*)(GPIO_BASE+0x0010ul+((x)*0x80ul))
#define	GPIO_PRT_CFG(x)	(_UW*)(GPIO_BASE+0x0044ul+((x)*0x80ul))

#define	CFG_HIGHZ	(0U)		/* Strong D_OUTput buffer */
#define	CFG_STRONG	(6U)		/* Strong D_OUTput buffer */
#define	CFG_IN_EN	(8U)		/* Enables the input buffer for IO. */

/* ------------------------------------------------------------------------ */
/*
 * High Speed IO Matrix (HSIOM)
 */
#define	HSIOM_BASE	(0x40300000)
#define	PORT_SEL0(x)	(_UW*)(HSIOM_BASE+0x0000ul+((x)*0x10ul))
#define	PORT_SEL1(x)	(_UW*)(HSIOM_BASE+0x0004ul+((x)*0x10ul))

#define	IO_SEL_GPIO		(0U)		/* GPIO controls 'out' */
#define	IO_SEL_ACT5		(17U)		/* Active functionality 0 */

/* ------------------------------------------------------------------------ */
/*
 * Watch dog (WDT)
 */
#define	WDT_BASE	(0x4026C000)
#define	WDT_CTL		(_UW*)(WDT_BASE+0x0000ul)	/* WDT Control Register */

/* ------------------------------------------------------------------------ */
/*
 * Peripheral PCLK groups (PERI_PCLK)
 */
#define	PERI_PCLK_GR0_BASE		(0x40040000)
#define	PERI_PCLK_GR0_DIV_CMD	(_UW*)(PERI_PCLK_GR0_BASE+0x0000ul)		/* Divider command Register */


#define	PERI_PCLK_GR1_BASE			(0x40042000)
#define	PERI_PCLK_GR1_DIV_CMD		(_UW*)(PERI_PCLK_GR1_BASE+0x0000ul)				/* Divider command Register */
#define	PERI_PCLK_GR1_CLOCK_CTL(x)	(_UW*)(PERI_PCLK_GR1_BASE+0x0C00ul+((x)*0x4ul))	/* Divider command Register */
#define	PERI_PCLK_GR1_DIV_16_CTL(x)	(_UW*)(PERI_PCLK_GR1_BASE+0x1400ul+((x)*0x4ul))	/* Divider control Register */

#define	DIV_SEL(x)			((x) << 0)	/* Specifies one of the dividers of the divider type specified by TYPE_SEL. */
#define	DIV_SEL_MASK		(0x000000FF)
#define	TYPE_SEL_8			(0x0 << 8)	/* Specifies divider type:8.0 (integer) clock dividers. */
#define	TYPE_SEL_16			(0x1 << 8)	/* Specifies divider type:16.0 (integer) clock dividers. */
#define	TYPE_SEL_16_5		(0x2 << 8)	/* Specifies divider type:16.5 (fractional) clock dividers. */
#define	TYPE_SEL_24_5		(0x3 << 8)	/* Specifies divider type:24.5 (fractional) clock dividers. */
#define	TYPE_SEL_MASK		(0x00000300)
#define	DIV_CMD_DISABLE		(1U << 30)
#define	DIV_CMD_ENABLE		(1U << 31)

#define	DIV_16_CTRL_EN		(1U << 0)	/* Divider enabled. */
#define	DIV_16_CTRL_INT16_DIV(x)	((x) << 8)	/* Integer division by (1+INT16_DIV). */

/* ------------------------------------------------------------------------ */
/*
 * Flash controller
 */
#define	FLASHC_BASE	(0x40240000)
#define	FLASH_CTL	(_UW*)(FLASHC_BASE+0x0000ul)		/* control Register */
#define WS(x)	((x) << 0)	/* FLASH macro wait states (same for main and work interfaces) */

/* ------------------------------------------------------------------------ */
/*
 * SRSS Core Registers (ver3)
 */
#define	SRSS_BASE	(0x40260000)
#define	CLK_PATH_SELECT(x)	(_UW*)(SRSS_BASE+0x1200ul+((x)*0x4ul))		/* Divider control Register */
#define	CLK_ROOT_SELECT(x)	(_UW*)(SRSS_BASE+0x1240ul+((x)*0x4ul))		/* Clock Root Select Register */
#define	CLK_ECO_CONFIG		(_UW*)(SRSS_BASE+0x151Cul)		/* ECO Configuration Register */
#define	CLK_ECO_CONFIG2		(_UW*)(SRSS_BASE+0x1544ul)		/* ECO Configuration Register2 */
#define	CLK_ECO_PRESCALE	(_UW*)(SRSS_BASE+0x1520ul)		/* ECO Prescaler Configuration Register */
#define	CLK_ECO_STATUS		(_UW*)(SRSS_BASE+0x1524ul)		/* ECO Status Registerr */
#define	CLK_FLL_CONFIG(x)	(_UW*)(SRSS_BASE+0x1530ul+((x)*0x4ul))		/* FLL Configuration Register */
#define	CLK_FLL_STATUS		(_UW*)(SRSS_BASE+0x1540ul)		/* FLL Status Register */
#define	CLK_PLL_CONFIG(x)	(_UW*)(SRSS_BASE+0x1600ul+((x)*0x4ul))		/* PLL Configuration Register */
#define	CLK_PLL_STATUS(x)	(_UW*)(SRSS_BASE+0x1640ul+((x)*0x4ul))		/* PLL Status Register */
#define	CLK_PLL400M_0_CONFIG(x)	(_UW*)(SRSS_BASE+0x1900ul+((x)*0x4ul))	/* 400MHz PLL Configuration Register */
#define	CLK_PLL400M_0_STATUS	(_UW*)(SRSS_BASE+0x190Cul)	/* 400MHz PLL Status Register */
#define	CLK_PLL400M_1_CONFIG(x)	(_UW*)(SRSS_BASE+0x1910ul+((x)*0x4ul))	/* 400MHz PLL Configuration Register */
#define	CLK_PLL400M_1_STATUS	(_UW*)(SRSS_BASE+0x191Cul)	/* 400MHz PLL Status Register */

#define PATH_MUX_IMO	(0U << 0)	/* IMO - Internal R/C Oscillator */
#define PATH_MUX_ECO	(2U << 0)	/* ECO - External-Crystal Oscillator */

#define ROOT_MUX(x)		((x) << 0)	/* Selects a clock path as the root of HFCLK<k> and for SRSS DSI input <k>.  */
									/* Select PATH0 (can be configured for FLL) */
									/* Select PATH1-5 (can be configured for PLL0-4 */
#define ROOT_DIRECT_MUX	(1U << 8)	/* Direct selection mux that allows IMO to bypass most of the clock mux structure. */
#define ROOT_ENABLE		(1U << 31)	/* Enable for this clock root.  */

#define ECO_CONFIG_AGC_EN	(1U << 1)	/* Automatic Gain Control (AGC) enable. */
#define ECO_CONFIG_ECO_EN	(1U << 31)	/* Master enable for ECO oscillator. */

#define ECO_CONFIG2_WDTRIM(x)	((x) << 0)	/* Watch Dog Trim. */
#define ECO_CONFIG2_ATRIM(x)	((x) << 4)	/* Amplitude trim. */
#define ECO_CONFIG2_FTRIM(x)	((x) << 8)	/* Filter Trim - 3rd harmonic oscillation */
#define ECO_CONFIG2_RTRIM(x)	((x) << 10)	/* Feedback resistor Trim */
#define ECO_CONFIG2_GTRIM(x)	((x) << 12)	/* Gain Trim - Startup time. */

#define ECO_STATUS_ECO_OK	(1U << 0)	/* Indicates the ECO internal oscillator circuit has sufficient amplitude. */
#define ECO_STATUS_ECO_READY	(1U << 1)	/* Indicates the ECO internal oscillator circuit has had enough time to fully stabilize. */

#define FLL_CONFIG_MULT(x)	((x) << 0)
#define FLL_CONFIG_OUTPUT_DIV(x)	((x) << 24)
#define FLL_CONFIG_ENANLE	(1U << 31)

#define FLL_CONFIG2_FLL_REF_DIV(x)	((x) << 0)
#define FLL_CONFIG2_LOCK_TOL(x)	((x) << 16)

#define FLL_CONFIG3_LF_IGAIN(x)	((x) << 0)
#define FLL_CONFIG3_LF_PGAIN(x)	((x) << 4)
#define FLL_CONFIG3_SETTLING_COUNT(x)	((x) << 8)
#define FLL_CONFIG3_BYPASS_MASK	(0x30000000)
#define FLL_CONFIG3_BYPASS_FLL_REF	(2U << 28)
#define FLL_CONFIG3_BYPASS_FLL_OUT	(3U << 28)

#define FLL_CONFIG4_CCO_LIMIT(x)	((x) << 0)
#define FLL_CONFIG4_CCO_RANGE(x)	((x) << 8)
#define FLL_CONFIG4_CCO_FREQ(x)	((x) << 16)
#define FLL_CONFIG4_CCO_ENANLE	(1U << 31)

#define FLL_STATUS_LOCKED	(1U << 0)
#define FLL_STATUS_CCO_READY	(1U << 2)

#define PLL_CONFIG_FEEDBACK_DIV(x)	((x) << 0)
#define PLL_CONFIG_REFERENCE_DIV(x)	((x) << 8)
#define PLL_CONFIG_OUTPUT_DIV(x)	((x) << 16)
#define PLL_CONFIG_BYPASS_MASK	(0x30000000)
#define PLL_CONFIG_BYPASS_PLL_REF	(2U << 28)
#define PLL_CONFIG_ENANLE	(1U << 31)

#define PLL_STATUS_LOCKED	(1U << 0)

#define PLL400M_CONFIG_FEEDBACK_DIV(x)	((x) << 0)
#define PLL400M_CONFIG_REFERENCE_DIV(x)	((x) << 8)
#define PLL400M_CONFIG_OUTPUT_DIV(x)	((x) << 16)
#define PLL400M_CONFIG_BYPASS_MASK	(0x30000000)
#define PLL400M_CONFIG_BYPASS_PLL_REF	(2U << 28)
#define PLL400M_CONFIG_ENANLE	(1U << 31)

#define PLL400M_CONFIG2_FRAC_DIV(x)	((x) << 0)
#define PLL400M_CONFIG2_ENANLE	(1U << 31)

#define PLL400M_STATUS_LOCKED	(1U << 0)


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
#define N_INTVEC		575	/* Number of Interrupt vectors */
							/* External Interrupt 567(0-566). Software Interrupt 8. */
							/* 567- is assigned to Software Interrupt */
#define	N_SYSVEC		16	/* Number of System Exceptions */

#define	CORE_EXT_INTVEC	8	/* Externel Interrupt number is 0-7. */
#define	CORE_SOFT_INTVEC	8	/* Software Interrupt number is 8-15. */

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
#define INTPRI_PENDSV		7	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for XMC7200)
 */
#define	CPU_HAS_PTMR		(118)

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
