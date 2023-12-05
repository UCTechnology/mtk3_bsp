/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef ADBUN_M4NR

/*
 *	cpu_clock.c (AdBun-M4NR)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 * Semi-Independent Watchdog Timer (SIWDT) Configuration
 */
#define SIWDEN_Val	(SIWDxEN_WDTE_DISABLE)
#define SIWDCR_Val	(SIWDxEN_WDCR_DISABLE)

/*
 * Clock Generator (CG) Configuration
 */
#define SYSCR_PG_Val	(CGSYSCR_PRCK_D1|CGSYSCR_GEAR_D1)
#define SYSCR_M_Val	(CGSYSCR_MCKSEL_D2)
#define SYSCR_M_Stat	(CGSYSCR_MCKSELPST_D2|CGSYSCR_MCKSELGST_D2)
#define STBYCR_Val	(CGSTBYCR_STBY_IDLE)

#define HZ_1M		(1000000UL)
#define WU_TIME_EXT	(5000UL)		/* warm-up time for EXT is 5ms   */
#define INIT_TIME_PLL	(100UL)			/* Initial time for PLL is 100us */
#define LOCKUP_TIME_PLL	(400UL)			/* Lockup time for PLL is 400us  */
#define EXTALH		(12000000ul)		/* External high-speed oscillator freq */
#define WUPHCR_WUPT_EXT	((UW)(((((UD)WU_TIME_EXT * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
				/* OSCCR<WUPT11:0> = warm-up time(us) * EXTALH / 16 */
#define WUPHCR_INIT_PLL	((UW)(((((UD)INIT_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))
#define WUPHCR_LUPT_PLL	((UW)(((((UD)LOCKUP_TIME_PLL * EXTALH / HZ_1M) - 16UL) /16UL) << 20U))

#if 1
#define PLL0SEL_Ready	(0x0036D521ul<<8U)	/* fPLL = 12MHz * 33.3150 = 199.89 MHz (MAX) */
#else
#define PLL0SEL_Ready	(0x002E9028ul<<8U)	/* fPLL = 10MHz * 1/2 * 40.0000 = 200.00 MHz (MAX) */
#endif

#if defined (__ICCARM__)
#pragma optimize=low
#else
#error	It is necessary to disable optimization.
#endif
EXPORT void startup_clock(void)
{
	/* CPU DEPENDENT : WDT & CLOCK */
		/* SIWD Disable */
	*SIWDxEN(0) = SIWDEN_Val;			// TSB_SIWD0->EN = SIWDEN_Val;
	*SIWDxCR(0) = SIWDCR_Val;			// TSB_SIWD0->CR = SIWDCR_Val;

#if (USE_FPU == 1)	/* FPU setting */
	// --> kernel/sysdepend/core/cm4/icrt0.S	// SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
							/* set CP10 and CP11 Full Access */
#endif
	/* Disable CG Register protect */
	*CGPROTECT = 0xC1;
	
	/* Reset CG --> Wait IHOSC1 is stable */
	*CGOSCCR  = CGOSCCR_IHOSC1EN_OSC;
	while( !(*CGOSCCR & CGOSCCR_IHOSC1F_STAT) );

	/* Clock(external) Setup */
	*CGSYSCR  = SYSCR_PG_Val;			// TSB_CG->SYSCR = SYSCR_GEAR_Val;
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_IHOSC1;
		// TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | EXT_CG_WUPHCR_WUCLK_SET);
	*CGOSCCR |= CGOSCCR_EOSCEN_EHOSC;		// TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_SET;
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_IHOSC1 | CGWUPHCR_WUON_START;
		// TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | EXT_CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
	/* Warm-up */
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );	// while(TSB_CG_WUPHCR_WUEF);

	*CGOSCCR |= CGOSCCR_OSCSEL_EHOSC;		// TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
	/* Confirm CGOSCCR<OSCF>="1" */
	while( !(*CGOSCCR & CGOSCCR_OSCF_STAT) );	// while(!TSB_CG_OSCCR_OSCF);
	*CGOSCCR &= ~CGOSCCR_IHOSC1EN_MASK;		// TSB_CG->OSCCR &= CG_OSCCR_IHOSC1EN_CLEAR ;

	/* set <MCKSEL> */
	*CGSYSCR = SYSCR_PG_Val | SYSCR_M_Val;		// TSB_CG->SYSCR = (SYSCR_GEAR_Val | CG_SYSCR_MCKSEL_SET);
	while( (*CGSYSCR & (CGSYSCR_MCKSELPST_STAT|CGSYSCR_MCKSELGST_STAT)) != SYSCR_M_Stat );

	*CGWUPHCR = WUPHCR_INIT_PLL|CGWUPHCR_WUCLK_EHOSC;	// TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET);
	/* PLL-->fOsc */
	*CGPLL0SEL &= ~CGPLL0SEL_PLL0SEL_MASK;		// TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0SEL_CLEAR;
	*CGPLL0SEL &= ~CGPLL0SEL_PLL0ON_MASK;		// TSB_CG->PLL0SEL &= CG_PLL0SEL_PLL0ON_CLEAR;
	*CGPLL0SEL  = PLL0SEL_Ready;			// TSB_CG->PLL0SEL = PLL0SEL_Ready;

	*CGWUPHCR = WUPHCR_INIT_PLL|CGWUPHCR_WUCLK_EHOSC|CGWUPHCR_WUON_START;
					// TSB_CG->WUPHCR = (WUPHCR_INIT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
	/* Warm-up */
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );	// while(TSB_CG_WUPHCR_WUEF);

	*CGWUPHCR = WUPHCR_LUPT_PLL|CGWUPHCR_WUCLK_EHOSC; // TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET);
	*CGPLL0SEL |= CGPLL0SEL_PLL0ON_OSC;		// TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0ON_SET;
							/* PLL enabled */
	*CGSTBYCR = STBYCR_Val;				// TSB_CG->STBYCR = STBYCR_Val;
	*CGWUPHCR = WUPHCR_LUPT_PLL|CGWUPHCR_WUCLK_EHOSC|CGWUPHCR_WUON_START;
					// TSB_CG->WUPHCR = (WUPHCR_LUPT_PLL | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
	/* Lockup */
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );	// while(TSB_CG_WUPHCR_WUEF);
	*CGPLL0SEL |= CGPLL0SEL_PLL0SEL_FPLL;		// TSB_CG->PLL0SEL |= CG_PLL0SEL_PLL0SEL_SET;
	/*Confirm CGPLL0SEL<PLL0ST> = "1" */
	while( !(*CGPLL0SEL & CGPLL0SEL_PLL0ST_STAT) );	// while(!TSB_CG_PLL0SEL_PLL0ST);
	
	/* Enable CG Register protect. */
	*CGPROTECT = 0x00;
}

EXPORT void shutdown_clock(void)
{

}

#endif /* ADBUN_M4NR */
