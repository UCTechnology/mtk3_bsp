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
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef SBK_M4KN

/*
 *	hw_setting.c (SBK-M4KN TMPM4KN)
 *	startup / shoutdown processing for hardware
 *	
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	/* Startup clock */
	startup_clock();

	/* Startup module clock */
	*CGPROTECT = 0x0000001C;		// UNLOCK, CG register write protection
	*CGFSYSMENA |= (CGFSYSMENA_PC|CGFSYSMENA_UART0);
						// Port C enable : UART0 PC0=TxD, PC1=RxD
						// UART ch0 enable --> CONSOLE
	*CGPROTECT = 0x00000000;		// LOCK, CG register write protection

	/* Setup Pin Function (for UART0) */
	set_port_func('C', 0, PC0_UT0TXDA, PORT_OUT);
	set_port_func('C', 1, PC1_UT0RXD,  PORT_IN );
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


#endif /* SBK_M4KN */
