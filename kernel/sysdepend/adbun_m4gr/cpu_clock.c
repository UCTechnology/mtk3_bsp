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
#ifdef ADBUN_M4GR

/*
 *	cpu_clock.c (AdBun-M4GR)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 * Clock Generator (CG) Configuration
 */
#define SYSCR_Val	(CGSYSCR_PRCK_D1|CGSYSCR_MCKSEL_D2|CGSYSCR_GEAR_D1)	/* T0h/1, fsysm/2, fsysh/1 */
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
   
#define PLL0SEL_Ready	(0x0036D521UL<<8U)	/* fPLL = 199.89MHz((12MHz * 33.3150) / 2) */

#if defined (__ICCARM__)
#pragma optimize=low
#else
#error	It is necessary to disable optimization.
#endif
/*
 *  Startup System Clock
 *    Input clock = 12MHz  Output clock(fsysh) = 199.89MHz  Output clock(fsysm) = 99.945MHz
 */
EXPORT void startup_clock(void)
{
	/* Clock(external) Setup */
	*CGSYSCR  = SYSCR_Val;

	/* external oscillation (EHOSC) */
	*CGOSCCR |= CGOSCCR_EOSCEN_EHOSC;
	/* high-speed oscillation warming up clock selection
	   A warming-up timer is set to  oscillator stable time */
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_EHOSC;
	/* High-speed oscillation warming up is started.  */
	*CGWUPHCR = WUPHCR_WUPT_EXT | CGWUPHCR_WUCLK_EHOSC | CGWUPHCR_WUON_START;
	/* It waits until it becomes the termination of high-speed oscillation warming up(= 0). */
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );

	/* It is high-speed oscillation selection for fosc to the exterior (EHOSC). */
	*CGOSCCR |= CGOSCCR_OSCSEL_EHOSC;
	/* Confirm OSCF=="1" */
	while( !(*CGOSCCR & CGOSCCR_OSCF_STAT) );
	/* An internal high-speed oscillator1 is suspended. */
	*CGOSCCR &= ~CGOSCCR_IHOSC1EN_MASK;

	*CGWUPHCR = WUPHCR_INIT_PLL|CGWUPHCR_WUCLK_EHOSC;
	/* PLL-->fOsc */
	*CGPLL0SEL &= ~CGPLL0SEL_PLL0SEL_MASK;
	*CGPLL0SEL &= ~CGPLL0SEL_PLL0ON_MASK;
	/* A PLL multiplication value setup is chosen. */
	*CGPLL0SEL  = PLL0SEL_Ready;
	/* Latency time after a multiplication setup (100us) */
	*CGWUPHCR = WUPHCR_INIT_PLL|CGWUPHCR_WUCLK_EHOSC|CGWUPHCR_WUON_START;
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );

	*CGWUPHCR = WUPHCR_LUPT_PLL|CGWUPHCR_WUCLK_EHOSC;
	/* PLL operation for fsys is carried out to an oscillation. */
	*CGPLL0SEL |= CGPLL0SEL_PLL0ON_OSC;

	*CGSTBYCR = STBYCR_Val;
	/* PLL output clock stable latency time (400us) */
	*CGWUPHCR = WUPHCR_LUPT_PLL|CGWUPHCR_WUCLK_EHOSC|CGWUPHCR_WUON_START;
	while( *CGWUPHCR & CGWUPHCR_WUEF_STAT );

	/* PLL selection for fsys is carried out to PLL use (fPLL ). */
	*CGPLL0SEL |= CGPLL0SEL_PLL0SEL_FPLL;
	/* Confirm CGPLL0SEL<PLL0ST> = "1" */
	while( !(*CGPLL0SEL & CGPLL0SEL_PLL0ST_STAT) );

	return;
}


EXPORT void shutdown_clock(void)
{

}

#endif /* ADBUN_M4GR */
