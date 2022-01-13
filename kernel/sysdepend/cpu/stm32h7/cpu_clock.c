/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/12.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32H7

/*
 *	cpu_clock.c (STM32H7)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 */
EXPORT void startup_clock(void)
{	
	UW	clk_sw, pll_src;
	UW	f_ratency;

	/* Supply configuration update enable */
	out_w(PWR_CR3, (in_w(PWR_CR3)&~(0x00000003)) | PWR_CR3_LDOEN);
	while((in_w(PWR_CSR1) & PWR_CSR1_ACTVOSRDY) == 0);

	/* Configure the main internal regulator output voltage */
	out_w(PWR_D3CR, PWR_D3CR_INIT);
	in_w(PWR_D3CR);
	while((in_w(PWR_D3CR) & PWR_D3CR_VOSRDY) == 0);

	out_w(RCC_PLLCKSELR, RCC_PLLCKSELR_INIT1);	// PLLs clock source selection

	/* System clock configuration */
	*(_UW*)RCC_CR |= RCC_CR_HSEBYP;			// HSE clock bypass
	*(_UW*)RCC_CR |= RCC_CR_HSEON;			// HSE enable
	while( (*(_UW*)RCC_CR & RCC_CR_HSERDY) == 0 );	// Wait HSE ready

	/* HSI48 configuration */
	*(_UW*)RCC_CR |= RCC_CR_HSI48ON;			// HSI48 enable
	while( (*(_UW*)RCC_CR & RCC_CR_HSI48RDY) == 0 );	// Wait HSI48 ready

	/* PLL1 Configuration */
	*(_UW*)RCC_CR &= ~RCC_CR_PLL1ON;		// PLL1 disable
	while( (*((_UW*)RCC_CR) & RCC_CR_PLL1RDY) != 0 );	// Wait PLL1 ready

	out_w(RCC_PLLCKSELR, RCC_PLLCKSELR_INIT2);	// PLLs clock source selection
	out_w(RCC_PLL1DIVR, RCC_PLL1DIVR_INIT);		// PLL1 dividers configuration

	*(_UW*)RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL1FRACEN;	// PLL1 fractional latch disable
	
	out_w(RCC_PLL1FRACR, RCC_PLL1FRACR_INIT);	// PLL1 fractional divider
	out_w(RCC_PLLCFGR, RCC_PLLCFGR_INIT);		// PLLs Configuration
	
	*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLL1FRACEN;	// PLL1 fractional latch enable
	
	*(_UW*)RCC_CR |= RCC_CR_PLL1ON;			// PLL1 eneble
	while( (*((_UW*)RCC_CR) & RCC_CR_PLL1RDY) == 0 );	// Wait PLL1 ready

	out_w(RCC_D1CFGR, RCC_D1CFGR_INIT);	// D1 domain AHB & Core prescaler(HCLK & SYSCLK)
	out_w(RCC_D2CFGR, RCC_D2CFGR_INIT);	// D2 domain APB1&2 prescaler (PCLK1&2)
	out_w(RCC_D3CFGR, RCC_D3CFGR_INIT);	// D3 domain APB4 prescaler (D3PCLK1)

	out_w(RCC_CFGR, (in_w(RCC_CFGR) & ~RCC_CFGR_SW) | RCC_CFGR_SW_VAL);
	while((((*(_UW*)RCC_CFGR)>>3)&0x00000007) != RCC_CFGR_SW_VAL) ;

	out_w(FLASH_ACR, FLASH_ACR_INIT);	// FLASH Read latency

	/* Disable all interrupts */
	out_w(RCC_CIER, 0);
}

EXPORT void shutdown_clock(void)
{

}

#endif /* CPU_STM32H7 */
