/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
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

/*
 *	i2c_sysdep.h
 *	I2C device driver
 *	System-dependent definition for CY8C6
 */

#ifndef	__DEV_I2C_CY8C6_H__
#define	__DEV_I2C_CY8C6_H__

#define	DEV_I2C_UNITNM		(0)	/* Number of devive units */
#define	DEV_I2C_UNIT0		(0)	/* SCB3, P6[0,1], USB-I2C */

/* I2C device driver operating state */
#define	I2C_STS_START		0x0000
#define	I2C_STS_RESTART		0x0001
#define	I2C_STS_STOP		0x0003
#define	I2C_STS_SEND		0x0004
#define	I2C_STS_RECV		0x0005
#define	I2C_STS_TOP		0x8000

/*----------------------------------------------------------------------
 * I2C interrupt number
 */
#define	INTNO_I2C		42	/* Use SCB3 in I2C mode */

#endif		/* __DEV_I2C_CY8C6_H__ */
