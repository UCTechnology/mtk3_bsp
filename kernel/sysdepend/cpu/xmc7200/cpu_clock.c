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

#include <sys/machine.h>
#ifdef EVK_XMC7200

/*
 *	cpu_clock.c (XMC7200)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

#define CLK_PATH_MAX	(5)	/* XMC7200 has 5 CLK_PATHs */
#define PLL_WAIT		(6)

#define ECO_ENABLE		(1)
#define IMO_ENABLE		(0)

/*
 *  memory setting.
 */
LOCAL void memory_initialize(void)
{
	/* Set worst case memory wait states. */
	*ROM_CTL |= SLOW_WS(1);
	*RAM0_CTL |= SLOW_WS(1);
	*RAM1_CTL |= SLOW_WS(1);
	*RAM2_CTL |= SLOW_WS(1);
	*FLASH_CTL |= WS(1);
}

/*
 *  ECO setting.
 */
LOCAL void eco_enable(void)
{
	*CLK_ECO_CONFIG = (ECO_CONFIG_AGC_EN | ECO_CONFIG_ECO_EN);
	while((*CLK_ECO_STATUS & (ECO_STATUS_ECO_OK | ECO_STATUS_ECO_READY)) != (ECO_STATUS_ECO_OK | ECO_STATUS_ECO_READY));
}

/*
 *  CLK_PATH Select.
 */
LOCAL void clk_path_select(void)
{
	_UB cnt;

	/* CLK_PATH Select */
	for (cnt = 0U; cnt < CLK_PATH_MAX; cnt++)
	{
#if IMO_ENABLE
		*CLK_PATH_SELECT(cnt) = PATH_MUX_IMO;
#elif ECO_ENABLE
		*CLK_PATH_SELECT(cnt) = PATH_MUX_ECO;
#endif
	}
}

/*
 *  CLK_ROOT Select. HF clocks
 */
LOCAL void clk_root_select(void)
{
	/* CLK_ROOT Select. XMC7200 has 7 CLK_ROOTs */
	/* PATH is up to 5. */
	*CLK_ROOT_SELECT(0) = (ROOT_MUX(0) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH0 (FLL) */
	*CLK_ROOT_SELECT(1) = (ROOT_MUX(1) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH1 (PLL400M0) */
	*CLK_ROOT_SELECT(2) = (ROOT_MUX(2) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH2 (PLL400M1) */
	*CLK_ROOT_SELECT(3) = (ROOT_MUX(3) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH3 (PLL2) */
	*CLK_ROOT_SELECT(4) = (ROOT_MUX(4) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH4 (PLL3) */
	*CLK_ROOT_SELECT(5) = (ROOT_MUX(5) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH5 (IMO) */
	*CLK_ROOT_SELECT(6) = (ROOT_MUX(5) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH5 (IMO) */
	*CLK_ROOT_SELECT(7) = (ROOT_MUX(5) | ROOT_DIRECT_MUX | ROOT_ENABLE);	/* CLK_PATH5 (IMO) */
}

/*
 *  FLL Disable
 */
LOCAL void fll_disable(void)
{
	/* FLL Disable */
	*CLK_FLL_CONFIG(2) &= ~FLL_CONFIG3_BYPASS_MASK;
	*CLK_FLL_CONFIG(2) |= FLL_CONFIG3_BYPASS_FLL_REF;
	/* Wait until BYPASS bits are changed */
	while((*CLK_FLL_CONFIG(2) & FLL_CONFIG3_BYPASS_FLL_REF) != FLL_CONFIG3_BYPASS_FLL_REF);

	*CLK_FLL_CONFIG(0) &= ~FLL_CONFIG_ENANLE;
	*CLK_FLL_CONFIG(3) &= ~FLL_CONFIG4_CCO_ENANLE;
}

/*
 *  FLL Enable
 */
LOCAL void fll_enable(void)
{
	/* Ffll = (FLL_MULT) * (Fref / REFERENCE_DIV) / (OUTPUT_DIV+1) */
	/* Ffll = (500) * (8M / 40) / (1 + 1) */
	/* Ffll = 50MHz */
#if IMO_ENABLE
	*CLK_FLL_CONFIG(0) = (FLL_CONFIG_MULT(500) | FLL_CONFIG_OUTPUT_DIV(1));
	*CLK_FLL_CONFIG(1) = (FLL_CONFIG2_FLL_REF_DIV(40) | FLL_CONFIG2_LOCK_TOL(34));
	*CLK_FLL_CONFIG(2) = (FLL_CONFIG3_LF_IGAIN(9) | FLL_CONFIG3_LF_PGAIN(0) | FLL_CONFIG3_SETTLING_COUNT(8) | FLL_CONFIG3_BYPASS_FLL_OUT);
	*CLK_FLL_CONFIG(3) = (FLL_CONFIG4_CCO_LIMIT(0xFF) | FLL_CONFIG4_CCO_RANGE(2) | FLL_CONFIG4_CCO_FREQ(0xFE));
#elif ECO_ENABLE
	/* Ffll = (500) * (16M / 80) / (1 + 1) */
	*CLK_FLL_CONFIG(0) = (FLL_CONFIG_MULT(500) | FLL_CONFIG_OUTPUT_DIV(1));
	*CLK_FLL_CONFIG(1) = (FLL_CONFIG2_FLL_REF_DIV(80) | FLL_CONFIG2_LOCK_TOL(2));
	*CLK_FLL_CONFIG(2) = (FLL_CONFIG3_LF_IGAIN(9) | FLL_CONFIG3_LF_PGAIN(0) | FLL_CONFIG3_SETTLING_COUNT(16) | FLL_CONFIG3_BYPASS_FLL_OUT);
	*CLK_FLL_CONFIG(3) = (FLL_CONFIG4_CCO_LIMIT(0xFF) | FLL_CONFIG4_CCO_RANGE(2) | FLL_CONFIG4_CCO_FREQ(0xDA));
#endif
	*CLK_FLL_CONFIG(3) |= FLL_CONFIG4_CCO_ENANLE;

	/* Wait until CCO is ready */
	while((*CLK_FLL_STATUS & FLL_STATUS_CCO_READY) != FLL_STATUS_CCO_READY);
	/* Set the FLL bypass mode to FLL_REF */
	*CLK_FLL_CONFIG(2) &= ~(FLL_CONFIG3_BYPASS_MASK);
	*CLK_FLL_CONFIG(2) |= (FLL_CONFIG3_BYPASS_FLL_REF);

	*CLK_FLL_CONFIG(0) |= FLL_CONFIG_ENANLE;

	/* Wait until LOCKED is ready */
	while((*CLK_FLL_STATUS & FLL_STATUS_LOCKED) != FLL_STATUS_LOCKED);
	/* Set the FLL bypass mode to FLL_OUT (ignoring lock indicator) */
	*CLK_FLL_CONFIG(2) &= ~(FLL_CONFIG3_BYPASS_MASK);
	*CLK_FLL_CONFIG(2) |= (FLL_CONFIG3_BYPASS_FLL_OUT);
}

/*
 *  PLL Disable
 */
LOCAL void pll_disable(void)
{
	_UB wait;

	/* PLL400M0 Disable */
	*CLK_PLL400M_0_CONFIG(0) &= ~PLL400M_CONFIG_BYPASS_MASK;
	*CLK_PLL400M_0_CONFIG(0) |= PLL400M_CONFIG_BYPASS_PLL_REF;
	/* Wait at least 6 PLL clock cycles */
	for(wait = 0; wait < PLL_WAIT; wait++);
	*CLK_PLL400M_0_CONFIG(0) &= ~PLL400M_CONFIG_ENANLE;

	/* PLL400M1 Disable */
	*CLK_PLL400M_1_CONFIG(0) &= ~PLL400M_CONFIG_BYPASS_MASK;
	*CLK_PLL400M_1_CONFIG(0) |= PLL400M_CONFIG_BYPASS_PLL_REF;
	/* Wait at least 6 PLL clock cycles */
	for(wait = 0; wait < PLL_WAIT; wait++);
	*CLK_PLL400M_1_CONFIG(0) &= ~PLL400M_CONFIG_ENANLE;

	/* PLL200M Disable */
	*CLK_PLL_CONFIG(0) &= ~PLL_CONFIG_BYPASS_MASK;
	*CLK_PLL_CONFIG(0) |= PLL_CONFIG_BYPASS_PLL_REF;
	/* Wait at least 6 PLL clock cycles */
	for(wait = 0; wait < PLL_WAIT; wait++);
	*CLK_PLL_CONFIG(0) &= ~PLL_CONFIG_ENANLE;

	/* PLL3 Disable */
	*CLK_PLL_CONFIG(1) &= ~PLL_CONFIG_BYPASS_MASK;
	*CLK_PLL_CONFIG(1) |= PLL_CONFIG_BYPASS_PLL_REF;
	/* Wait at least 6 PLL clock cycles */
	for(wait = 0; wait < PLL_WAIT; wait++);
	*CLK_PLL_CONFIG(1) &= ~PLL_CONFIG_ENANLE;
}

/*
 *  PLL Enable
 */
LOCAL void pll_enable(void)
{
    /* PLL400M0 */
	/* fOUT = (FEEDBACK_DIV + (FRAC_EN * FRAC_DIV / 2^24)) * (fREF / REFERENCE_DIV) / (OUTPUT_DIV) */
	/* fOUT = (85 + (1 * 0) * (8M / 1 / 2) */
	/* fOUT = 340MHz (Max 340MHz when spreading on) */
#if IMO_ENABLE
	*CLK_PLL400M_0_CONFIG(0) = (PLL400M_CONFIG_FEEDBACK_DIV(85) | PLL400M_CONFIG_REFERENCE_DIV(1) | PLL400M_CONFIG_OUTPUT_DIV(2));
	*CLK_PLL400M_0_CONFIG(1) = (PLL400M_CONFIG2_FRAC_DIV(0) | PLL400M_CONFIG2_ENANLE);
#elif ECO_ENABLE
	/* fOUT = (42 + (1 * 0.5) * (16M / 1 / 2) */
	*CLK_PLL400M_0_CONFIG(0) = (PLL400M_CONFIG_FEEDBACK_DIV(42) | PLL400M_CONFIG_REFERENCE_DIV(1) | PLL400M_CONFIG_OUTPUT_DIV(2));
	*CLK_PLL400M_0_CONFIG(1) = (PLL400M_CONFIG2_FRAC_DIV(0x800000) | PLL400M_CONFIG2_ENANLE);
#endif
	*CLK_PLL400M_0_CONFIG(0) |= PLL400M_CONFIG_ENANLE;
	while((*CLK_PLL400M_0_STATUS & PLL400M_STATUS_LOCKED) != 0U);

	/* PLL400M1 */
	/* fOUT = (FEEDBACK_DIV + (FRAC_EN * FRAC_DIV / 2^24)) * (fREF / REFERENCE_DIV) / (OUTPUT_DIV) */
	/* fOUT = (73 + (1 * 8388608 / 2^24) * (8M / 1 / 3) */
	/* fOUT = (73 + 0.5) * (2.666) */
	/* fOUT = 196MHz */
#if IMO_ENABLE
	*CLK_PLL400M_1_CONFIG(0) = (PLL400M_CONFIG_FEEDBACK_DIV(73) | PLL400M_CONFIG_REFERENCE_DIV(1) | PLL400M_CONFIG_OUTPUT_DIV(3));
	*CLK_PLL400M_1_CONFIG(1) = (PLL400M_CONFIG2_FRAC_DIV(0x800000) | PLL400M_CONFIG2_ENANLE);
#elif ECO_ENABLE
	/* fOUT = (36 + (1 * 12582912 / 2^24) * (16M / 1 / 3) */
	/* fOUT = (36 + 0.75) * (5.333) */
	*CLK_PLL400M_1_CONFIG(0) = (PLL400M_CONFIG_FEEDBACK_DIV(36) | PLL400M_CONFIG_REFERENCE_DIV(1) | PLL400M_CONFIG_OUTPUT_DIV(3));
	*CLK_PLL400M_1_CONFIG(1) = (PLL400M_CONFIG2_FRAC_DIV(0xC00000) | PLL400M_CONFIG2_ENANLE);
#endif
	*CLK_PLL400M_1_CONFIG(0) |= PLL400M_CONFIG_ENANLE;
	while((*CLK_PLL400M_1_STATUS & PLL400M_STATUS_LOCKED) != 0U);

	/* PLL2 */
	/* Fpll = (FEEDBACK_DIV) * (Fref / REFERENCE_DIV) / (OUTPUT_DIV) */
	/* Fpll = (36) * (8M / 1 / 2) */
	/* Fpll = 144MHz */
#if IMO_ENABLE
	*CLK_PLL_CONFIG(0)  = (PLL_CONFIG_FEEDBACK_DIV(36) | PLL_CONFIG_REFERENCE_DIV(1) | PLL_CONFIG_OUTPUT_DIV(2));
#elif ECO_ENABLE
	/* Fpll = (36) * (16M / 2 / 2) */
	*CLK_PLL_CONFIG(0)  = (PLL_CONFIG_FEEDBACK_DIV(36) | PLL_CONFIG_REFERENCE_DIV(2) | PLL_CONFIG_OUTPUT_DIV(2));
#endif
	*CLK_PLL_CONFIG(0) |= PLL_CONFIG_ENANLE;
	while((*CLK_PLL_STATUS(0) & PLL_STATUS_LOCKED) != 0U);

	/* PLL3 */
	/* Fpll = (FEEDBACK_DIV) * (Fref / REFERENCE_DIV) / (OUTPUT_DIV) */
	/* Fpll = (25) * (8M / 1 / 2) */
	/* Fpll = 100MHz */
#if IMO_ENABLE
	*CLK_PLL_CONFIG(1)  = (PLL_CONFIG_FEEDBACK_DIV(25) | PLL_CONFIG_REFERENCE_DIV(1) | PLL_CONFIG_OUTPUT_DIV(2));
#elif ECO_ENABLE
	/* Fpll = (25) * (16M / 2 / 2) */
	*CLK_PLL_CONFIG(1)  = (PLL_CONFIG_FEEDBACK_DIV(25) | PLL_CONFIG_REFERENCE_DIV(2) | PLL_CONFIG_OUTPUT_DIV(2));
#endif
    *CLK_PLL_CONFIG(1) |= PLL_CONFIG_ENANLE;
	while((*CLK_PLL_STATUS(1) & PLL_STATUS_LOCKED) != 0U);
}

/*
 *  Startup System Clock
 */
EXPORT void startup_clock(void)
{
	memory_initialize();
#if ECO_ENABLE
	eco_enable();
#endif
	fll_disable();
	clk_path_select();
	pll_disable();
	pll_enable();
	clk_root_select();
	fll_enable();
}

EXPORT void shutdown_clock(void)
{

}

#endif /* EVK_XMC7200 */
