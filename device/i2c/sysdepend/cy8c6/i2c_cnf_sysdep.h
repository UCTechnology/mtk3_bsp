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
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for CY8C6
 */
#ifndef	__DEV_I2C_CNF_CY8C6_H__
#define	__DEV_I2C_CNF_CY8C6_H__

/* Interrupt priority */
#define DEVCNF_I2C_INTPRI	(3)		/* SCB3 : P6[0:1] --> ARD_SCL, ARD_SDA */

/* Communication timeout */
#define DEVCNF_I2C_TMO		(1000)

#endif		/* __DEV_I2C_CNF_CY8C6_H__ */
