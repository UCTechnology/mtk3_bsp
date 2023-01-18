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
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CY8CKIT_CY8C6

/*
 *	devinit.c (CY8CKIT-062S2-43012)
 *	Device-Dependent Initialization
 */

#include <sys/sysdef.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

#include <kernel.h>
#include "sysdepend.h"

/* ------------------------------------------------------------------------ */

/*
 * Initialization before micro T-Kernel starts
 */

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
#undef DEVCNF_USE_ADC
#undef DEVCNF_USE_IIC
//#undef DEVCNF_USE_SER

#define SER_PORT_SCB1	1	/* "serb" : SCB1 : ARD_A[0:3]		*/
#define SER_PORT_SCB2	2	/* "serc" : SCB2 : ARD_A[8:11]		*/
#if !USE_TMONITOR
#define SER_PORT_SCB5	5	/* "serf" : SCB5 : ARD_D[0:3]		*/
#endif				/* SCB5 is assigned to T-Monitor.	*/

EXPORT ER knl_start_device( void )
{
#if USE_SDEV_DRV	// Use sample driver
	ER	err;

	/* A/D Converter unit.0 "adca" */
	#if DEVCNF_USE_ADC
		err = dev_init_adc(0);
		if(err < E_OK) return err;
	#endif

	/* I2C unit.0 "iica" */
	#if DEVCNF_USE_IIC
		err = dev_init_i2c(0);
		if(err < E_OK) return err;
	#endif

	/* The serial driver supports SCB1, SCB2, and SCB5. */
	#if DEVCNF_USE_SER
		err = dev_init_ser(SER_PORT_SCB1);
		if(err < E_OK) return err;
		err = dev_init_ser(SER_PORT_SCB2);
		if(err < E_OK) return err;
	#if !USE_TMONITOR
		err = dev_init_ser(SER_PORT_SCB5);
		if(err < E_OK) return err;
	#endif
	#endif

#endif

	return E_OK;
}

#if USE_SHUTDOWN
/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* USE_SHUTDOWN */

#endif /* CY8CKIT_CY8C6 */
