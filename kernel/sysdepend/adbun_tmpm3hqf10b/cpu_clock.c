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
#include <sys/machine.h>
#ifdef CPU_TMPM3HQF10B

/*
 *	cpu_clock.c (TMPM3HQF10B)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 * Clock Generator (CG) Configuration
 */
#define SYSCR_Val	(CGSYSCR_PRCK_D1|CGSYSCR_GEAR_D1)
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
   
#define PLL0SEL_Ready	(0x0036EA1AUL<<8U)	/* fPLL = 79.88MHz((12MHz * 26.6250) / 4) */

#if defined (__ICCARM__)
#pragma optimize=low
#else
#error	It is necessary to disable optimization.
#endif
/*
 *  Startup System Clock
 *    Input clock = 12MHz  Output clock = 79.88MHz
 */
EXPORT void startup_clock(void)
{
	/* Clock(external) Setup */
	*CGSYSCR  = SYSCR_Val;				// TSB_CG->SYSCR = SYSCR_Val;
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_EHOSC;
		// TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET);
	*CGOSCCR |= CGOSCCR_EOSCEN_EHOSC;		// TSB_CG->OSCCR |= CG_OSCCR_EOSCEN_SET;
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_EHOSC | CGWUPHCR_WUON_START;
		// TSB_CG->WUPHCR = (WUPHCR_WUPT_EXT | CG_WUPHCR_WUCLK_SET | CG_WUPHCR_WUON_START_SET);
	/* Warm-up */
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );	// while(TSB_CG_WUPHCR_WUEF);

	*CGOSCCR |= CGOSCCR_OSCSEL_EHOSC;		// TSB_CG->OSCCR |= CG_OSCCR_OSCSEL_SET;
	/* Confirm OSCF=="1" */
	while( !(*CGOSCCR & CGOSCCR_OSCF_STAT) );	// while(!TSB_CG_OSCCR_OSCF);
	*CGOSCCR &= ~CGOSCCR_IHOSC1EN_MASK;		// TSB_CG->OSCCR &= CG_OSCCR_IHOSC1EN_CLEAR ;

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
	
	return;
	
}


EXPORT void shutdown_clock(void)
{

}

#endif /* CPU_TMPM3HQF10B */