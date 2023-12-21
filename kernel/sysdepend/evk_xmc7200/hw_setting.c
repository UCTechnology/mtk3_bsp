/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
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
 *	hw_setting.c (XMC7200D-E272K8384)
 *	startup / shoutdown processing for hardware
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

#define UNUSED_INT	(0xFFU)

typedef struct irq_table
{
	UINT intno;
	INT level;
} IRQ_TABLE;

LOCAL const IRQ_TABLE nvic_table[CORE_EXT_INTVEC] =
{
	/* Interrupt priorities 1 and 7 are used by the OS and cannot be set. */
	{0U, 2U},	/* IRQ 0 */
	{1U, 3U},	/* IRQ 1 */
	{2U, 4U},	/* IRQ 2 */
	{3U, 5U},	/* IRQ 3 */
	{4U, 6U},	/* IRQ 4 */
	{UNUSED_INT, 7U},	/* IRQ 5 */
	{UNUSED_INT, 8U},	/* IRQ 6 */
	{UNUSED_INT, 9U},	/* IRQ 7 */
};

IMPORT void EnableInt_nvic( UINT intno, INT level );

LOCAL void enable_irq(void)
{
	UB loop_cnt;

	for(loop_cnt = 0U; loop_cnt < CORE_EXT_INTVEC; loop_cnt++)
	{
		if (nvic_table[loop_cnt].intno != UNUSED_INT)
		{
			EnableInt_nvic(nvic_table[loop_cnt].intno, nvic_table[loop_cnt].level);
		}
	}
}

LOCAL void port_initialize(void)
{
	/* P13_0:SCB3_RX, P13_1:SCB3_TX */
	*GPIO_PRT_CFG(13) = ((CFG_STRONG << 4U) | (CFG_IN_EN << 0U));
	*PORT_SEL0(13) = ((IO_SEL_ACT5 << 0U) | (IO_SEL_ACT5 << 8U));

	/* P13_2:SCB3_RTS, P13_3:SCB3_CTS */
	*GPIO_PRT_CFG(13) |= (CFG_IN_EN << 12U);
	*PORT_SEL0(13) |= ((IO_SEL_ACT5 << 16U) | (IO_SEL_ACT5 << 24U));

	/* P21_2:ECO_IN, P21_3:ECO_OUT */
	/* I/O pins that support an oscillator function (WCO or ECO) must be configured for high-impedance if the oscillator is enabled. */
	*GPIO_PRT_CFG(21) = ((CFG_HIGHZ << 8U) | (CFG_HIGHZ << 12U));
	*PORT_SEL0(21) = ((IO_SEL_GPIO << 16U) | (IO_SEL_GPIO << 24U));
}

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	/* Enable writing of ITCM, DTCM, VTOR registers */
	*CM7_0_CTL &= ~(PPB_LOCK_ITCMR | PPB_LOCK_DTCMR | PPB_LOCK_VTOR);
	*CM7_0_CTL |= (INIT_ITCM_EN | INIT_DTCM_EN | INIT_ITCM_RMW_EN | INIT_DTCM_RMW_EN);

	/* Enable IRQ Register */
	enable_irq();

	/* Startup clock */
	startup_clock();

	/* port */
	port_initialize();
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* EVK_XMC7200 */
