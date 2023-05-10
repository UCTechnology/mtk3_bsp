/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the SBK-M4MN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_M4MN

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_m4mn.c
 *	I2C device driver
 *	System-dependent definition for TMPM4MN
 */

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_i2c_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb)
{
	return E_NOSPT;
}

#endif	/* DEV_IIC_ENABLE */
#endif	/* CPU_M4MN */
