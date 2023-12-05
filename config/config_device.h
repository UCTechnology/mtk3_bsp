/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the AdBun-M4GR.
 *    Modified by UC Technology at 2023/09/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */


/*
 *	config_device.h
 *	Device configuration definition
 */

#ifndef	__DEV_CONFIG_H__
#define	__DEV_CONFIG_H__

/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */

#if !defined(DEVCNF_USE_SER)			// defined in IDE
#define DEVCNF_USE_SER		1		// Serial communication device 
#endif
#define DEVCNF_USE_ADC		0		// A/D conversion device
#define DEVCNF_USE_IIC		0		// I2C communication device

#endif	/* __DEV_CONFIG_H__ */
