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
 *	devinit.c (SBK-M4KN)
 *	Device-Dependent Initialization
 */

#include <sys/sysdef.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

#include <kernel.h>
#include "sysdepend.h"

/* ------------------------------------------------------------------------ */
EXPORT	void	set_port_func(INT port, INT no, UW func, UW inout)
{
	UW	pat;
	INT	i;

	pat = 0x00000001ul << no;
	/*------ select FUNCTION ------*/
	if( 0 < func ){				// func = NA(0), 1, 2, 3, 4, 5, 6, 7
		for( i = 1; i <= 7; i++ ){
			*PxFRn(port,i) &= ~pat;
		}
		*PxFRn(port,func) |= pat;
	}

	/*------ select INPUT/OUTPUT ------*/
	*PxCR(port) &= ~pat;			// Disable Output
	*PxIE(port) &= ~pat;			// Disable Input
	if( inout & PORT_IN ){
		*PxIE(port) |= pat;		// Enable Input
	}
	if( inout & PORT_OUT ){
		*PxCR(port) |= pat;		// Enable Output
	}
}

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

	/* Serial ch.2 "serb" */
	#if DEVCNF_USE_SER
		err = dev_init_ser(0);
		if(err < E_OK) return err;
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

#endif /* SBK_M4KN */
